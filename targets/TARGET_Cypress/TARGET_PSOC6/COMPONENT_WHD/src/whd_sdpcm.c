/*
 * Copyright 2019 Cypress Semiconductor Corporation
 * SPDX-License-Identifier: Apache-2.0
 * 
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 * 
 *     http://www.apache.org/licenses/LICENSE-2.0
 * 
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

/** @file
 *  Provides an implementation of the Broadcom SDPCM protocol.
 *  The Broadcom SDPCM protocol provides multiplexing of Wireless Data frames,
 *  I/O Control functions (IOCTL), and Asynchronous Event signalling.
 *  It is required when communicating with Broadcom 802.11 devices.
 *
 */
#include "whd_sdpcm.h"
#include "bus_protocols/whd_bus_protocol_interface.h"
#include "whd_endian.h"
#include "whd_chip_constants.h"
#include "whd_chip.h"
#include "whd_thread_internal.h"
#include "whd_debug.h"
#include "whd_events_int.h"
#include "whd_wifi_api.h"
#include "whd_buffer_api.h"
#include "whd_network_if.h"
#include "whd_wlioctl.h"
#include "whd_types_int.h"
#include "whd_endian.h"

/******************************************************
* @cond       Constants
******************************************************/

#define ETHER_TYPE_BRCM           (0x886C)      /** Broadcom Ethertype for identifying event packets - Copied from DHD include/proto/ethernet.h */
#define BRCM_OUI            "\x00\x10\x18"      /** Broadcom OUI (Organizationally Unique Identifier): Used in the proprietary(221) IE (Information Element) in all Broadcom devices */
#define BCM_MSG_IFNAME_MAX            (16)      /** Maximum length of an interface name in a wl_event_msg_t structure*/

#define BDC_FLAG2_IF_MASK           (0x0f)

#define SDPCM_HEADER_LEN              (12)

/* Event flags */
#define WLC_EVENT_MSG_LINK      (0x01)    /** link is up */
#define WLC_EVENT_MSG_FLUSHTXQ  (0x02)    /** flush tx queue on MIC error */
#define WLC_EVENT_MSG_GROUP     (0x04)    /** group MIC error */
#define WLC_EVENT_MSG_UNKBSS    (0x08)    /** unknown source bsscfg */
#define WLC_EVENT_MSG_UNKIF     (0x10)    /** unknown source OS i/f */

/******************************************************
*             Macros
******************************************************/

/******************************************************
*             Local Structures
******************************************************/

#pragma pack(1)

/*TODO: Keep this typedef? (in preference to the defines above */
#if 0
typedef struct
{
    uint16_t control_id;
    uint8_t interface_index : 4;
    uint16_t reserved        : 10;
    uint8_t set             : 1;
    uint8_t error           : 1;
}sdpcm_cdc_flags_t;
#endif /* if 0 */
typedef struct
{
    uint8_t sequence;
    uint8_t channel_and_flags;
    uint8_t next_length;
    uint8_t header_length;
    uint8_t wireless_flow_control;
    uint8_t bus_data_credit;
    uint8_t _reserved[2];
} sdpcm_sw_header_t;

/*
 * SDPCM header definitions
 */
typedef struct
{
    uint16_t frametag[2];
    sdpcm_sw_header_t sw_header;
} sdpcm_header_t;

typedef struct bcmeth_hdr
{
    uint16_t subtype;      /** Vendor specific..32769 */
    uint16_t length;
    uint8_t version;       /** Version is 0 */
    uint8_t oui[3];        /** Broadcom OUI */
    uint16_t usr_subtype;  /** user specific Data */
} sdpcm_bcmeth_header_t;

/* these fields are stored in network order */
typedef struct
{

    uint16_t version;                         /** Version 1 has fields up to ifname.
                                               * Version 2 has all fields including ifidx and bss_cfg_idx */
    uint16_t flags;                           /** see flags */
    uint32_t event_type;                      /** Message */
    uint32_t status;                          /** Status code */
    uint32_t reason;                          /** Reason code (if applicable) */
    uint32_t auth_type;                       /** WLC_E_AUTH */
    uint32_t datalen;                         /** data buf */
    whd_mac_t addr;                         /** Station address (if applicable) */
    char ifname[BCM_MSG_IFNAME_MAX];          /** name of the packet incoming interface */
    uint8_t ifidx;                            /** destination OS i/f index */
    uint8_t bss_cfg_idx;                      /** source bsscfg index */
} sdpcm_raw_event_header_t;

/* used by driver msgs */
typedef struct bcm_event
{
    ethernet_header_t ether;
    sdpcm_bcmeth_header_t bcmeth;
    union
    {
        whd_event_header_t whd;
        sdpcm_raw_event_header_t raw;
    } event;
} sdpcm_bcm_event_t;

#pragma pack()

/******************************************************
*             Static Variables
******************************************************/

/******************************************************
*             SDPCM Logging
*
* Enable this section to allow logging of SDPCM packets
* into a buffer for later perusal
*
* See sdpcm_log  and  next_sdpcm_log_pos
*
******************************************************/
/** @cond */

#if 0

#define SDPCM_LOG_SIZE 30
#define SDPCM_LOG_HEADER_SIZE (0x60)

typedef enum { UNUSED, LOG_TX, LOG_RX } sdpcm_log_direction_t;
typedef enum { IOCTL, DATA, EVENT } sdpcm_log_type_t;

typedef struct SDPCM_log_entry_struct
{
    sdpcm_log_direction_t direction;
    sdpcm_log_type_t type;
    unsigned long time;
    unsigned long length;
    unsigned char header[SDPCM_LOG_HEADER_SIZE];
}sdpcm_log_entry_t;

static int next_sdpcm_log_pos = 0;
static sdpcm_log_entry_t sdpcm_log[SDPCM_LOG_SIZE];

static void add_sdpcm_log_entry(sdpcm_log_direction_t dir, sdpcm_log_type_t type, unsigned long length, char *eth_data)
{

    sdpcm_log[next_sdpcm_log_pos].direction = dir;
    sdpcm_log[next_sdpcm_log_pos].type = type;
    cy_rtos_get_time(&sdpcm_log[next_sdpcm_log_pos].time);
    sdpcm_log[next_sdpcm_log_pos].length = length;
    memcpy(sdpcm_log[next_sdpcm_log_pos].header, eth_data, SDPCM_LOG_HEADER_SIZE);
    next_sdpcm_log_pos++;
    if (next_sdpcm_log_pos >= SDPCM_LOG_SIZE)
    {
        next_sdpcm_log_pos = 0;
    }
}

#else
#define add_sdpcm_log_entry(dir, type, length, eth_data)
#endif

/** @endcond */

/******************************************************
*             Static Function Prototypes
******************************************************/
static whd_buffer_t  whd_sdpcm_get_next_buffer_in_queue(whd_driver_t whd_driver, whd_buffer_t buffer);
static void            whd_sdpcm_set_next_buffer_in_queue(whd_driver_t whd_driver, whd_buffer_t buffer,
                                                          whd_buffer_t prev_buffer);
extern void whd_wifi_log_event(whd_driver_t whd_driver, const whd_event_header_t *event_header,
                               const uint8_t *event_data);
/******************************************************
*             Function definitions
******************************************************/

/** Initialises the SDPCM protocol handler
 *
 *  Initialises mutex and semaphore flags needed by the SDPCM handler.
 *  Also initialises the list of event handlers. This function is called
 *  from the @ref whd_thread_init function.
 *
 * @return    WHD result code
 */

whd_result_t whd_sdpcm_init(whd_driver_t whd_driver)
{
    whd_sdpcm_info_t *sdpcm_info = &whd_driver->sdpcm_info;

    /* Create the sdpcm packet queue semaphore */
    if (cy_rtos_init_semaphore(&sdpcm_info->send_queue_mutex, 1, 0) != WHD_SUCCESS)
    {
        return WHD_SEMAPHORE_ERROR;
    }
    if (cy_rtos_set_semaphore(&sdpcm_info->send_queue_mutex, WHD_FALSE) != WHD_SUCCESS)
    {
        WPRINT_WHD_ERROR( ("Error setting semaphore in %s at %d \n", __func__, __LINE__) );
        return WHD_SEMAPHORE_ERROR;
    }

    /* Packet send queue variables */
    sdpcm_info->send_queue_head   = (whd_buffer_t)NULL;
    sdpcm_info->send_queue_tail   = (whd_buffer_t)NULL;

    whd_sdpcm_bus_vars_init(whd_driver);

    return WHD_SUCCESS;
}

/* Re-initialize the bus variables after deep sleep */
void whd_sdpcm_bus_vars_init(whd_driver_t whd_driver)
{
    whd_sdpcm_info_t *sdpcm_info = &whd_driver->sdpcm_info;

    /* Bus data credit variables */
    sdpcm_info->credit_diff                     = 0;
    sdpcm_info->largest_credit_diff             = 0;

    sdpcm_info->packet_transmit_sequence_number = 0;
    sdpcm_info->last_bus_data_credit            = (uint8_t)1;
}

/** Initialises the SDPCM protocol handler
 *
 *  De-initialises mutex and semaphore flags needed by the SDPCM handler.
 *  This function is called from the @ref whd_thread_func function when it is exiting.
 */
void whd_sdpcm_quit(whd_driver_t whd_driver)
{
    whd_sdpcm_info_t *sdpcm_info = &whd_driver->sdpcm_info;
    whd_result_t result;

    /* Delete the SDPCM queue mutex */
    (void)cy_rtos_deinit_semaphore(&sdpcm_info->send_queue_mutex);    /* Ignore return - not much can be done about failure */

    /* Free any left over packets in the queue */
    while (sdpcm_info->send_queue_head != NULL)
    {
        whd_buffer_t buf = whd_sdpcm_get_next_buffer_in_queue(whd_driver, sdpcm_info->send_queue_head);
        result = whd_buffer_release(whd_driver, sdpcm_info->send_queue_head, WHD_NETWORK_TX);
        if (result != WHD_SUCCESS)
            WPRINT_WHD_ERROR( ("buffer release failed in %s at %d \n", __func__, __LINE__) );
        sdpcm_info->send_queue_head = buf;
    }
}

void whd_sdpcm_update_credit(whd_driver_t whd_driver, uint8_t *data)
{
    sdpcm_sw_header_t *header = (sdpcm_sw_header_t *)(data + 4);
    whd_sdpcm_info_t *sdpcm_info = &whd_driver->sdpcm_info;

    if ( (header->channel_and_flags & 0x0f) < (uint8_t)3 )
    {
        sdpcm_info->credit_diff = (uint8_t)(header->bus_data_credit - sdpcm_info->last_bus_data_credit);
        WPRINT_WHD_DATA_LOG( ("credit update =%d\n ", header->bus_data_credit) );
        if (sdpcm_info->credit_diff <= GET_C_VAR(whd_driver, BUS_CREDIT_DIFF) )
        {
            sdpcm_info->last_bus_data_credit = header->bus_data_credit;
        }
        else
        {
            if (sdpcm_info->credit_diff > sdpcm_info->largest_credit_diff)
            {
                sdpcm_info->largest_credit_diff = sdpcm_info->credit_diff;
            }
        }
    }

    whd_bus_set_flow_control(whd_driver, header->wireless_flow_control);
}

/** Processes and directs incoming SDPCM packets
 *
 *  This function receives SDPCM packets from the Broadcom 802.11 device and decodes the SDPCM header
 *  to determine where the packet should be directed.
 *
 *  Control packets (IOCTL/IOVAR) / Data Packets/ Event Packets are passed to CDC/BDC layer
 *  and the appropriate event handler is called.
 *
 * @param buffer  : The SDPCM packet buffer received from the Broadcom 802.11 device
 *
 */
void whd_sdpcm_process_rx_packet(whd_driver_t whd_driver, whd_buffer_t buffer)
{
    bus_common_header_t *packet;
    uint16_t size;
    uint16_t size_inv;
    sdpcm_header_t sdpcm_header;
    whd_result_t result;

    packet = (bus_common_header_t *)whd_buffer_get_current_piece_data_pointer(whd_driver, buffer);

    memcpy(&sdpcm_header, packet->bus_header, BUS_HEADER_LEN);

    sdpcm_header.frametag[0] = dtoh16(sdpcm_header.frametag[0]);
    sdpcm_header.frametag[1] = dtoh16(sdpcm_header.frametag[1]);

    /* Extract the total SDPCM packet size from the first two frametag bytes */
    size = sdpcm_header.frametag[0];

    /* Check that the second two frametag bytes are the binary inverse of the size */
    size_inv = (uint16_t) ~size;  /* Separate variable due to GCC Bug 38341 */
    if (sdpcm_header.frametag[1] != size_inv)
    {
        WPRINT_WHD_DEBUG( ("Received a packet with a frametag which is wrong\n") );
        result = whd_buffer_release(whd_driver, buffer, WHD_NETWORK_RX);
        if (result != WHD_SUCCESS)
            WPRINT_WHD_ERROR( ("buffer release failed in %s at %d \n", __func__, __LINE__) );
        return;
    }

    /* Check whether the packet is big enough to contain the SDPCM header (or) it's too big to handle */
    if ( (size < (uint16_t)SDPCM_HEADER_LEN) || (size > whd_buffer_get_current_piece_size(whd_driver, buffer) ) )
    {
        whd_minor_assert("Packet size invalid", 0 == 1);
        WPRINT_WHD_DEBUG( (
                              "Received a packet that is too small to contain anything useful (or) too big. Packet Size = [%d]\n",
                              size) );
        result = whd_buffer_release(whd_driver, buffer, WHD_NETWORK_RX);
        if (result != WHD_SUCCESS)
            WPRINT_WHD_ERROR( ("buffer release failed in %s at %d \n", __func__, __LINE__) );
        return;
    }

    /* Get address of packet->sdpcm_header.frametag indirectly to avoid IAR's unaligned address warning */
    whd_sdpcm_update_credit(whd_driver,
                            (uint8_t *)&sdpcm_header.sw_header - sizeof(sdpcm_header.frametag) );

    if (size == (uint16_t)SDPCM_HEADER_LEN)
    {
        /* This is a flow control update packet with no data - release it. */
        result = whd_buffer_release(whd_driver, buffer, WHD_NETWORK_RX);
        if (result != WHD_SUCCESS)
            WPRINT_WHD_ERROR( ("buffer release failed in %s at %d \n", __func__, __LINE__) );

        return;
    }

    /* Check the SDPCM channel to decide what to do with packet. */
    switch (sdpcm_header.sw_header.channel_and_flags & 0x0f)
    {
        case CONTROL_HEADER:  /* IOCTL/IOVAR reply packet */
        {
            add_sdpcm_log_entry(LOG_RX, IOCTL, whd_buffer_get_current_piece_size(whd_driver, buffer),
                                (char *)whd_buffer_get_current_piece_data_pointer(whd_driver, buffer) );

            /* Check that packet size is big enough to contain the CDC header as well as the SDPCM header */
            if (sdpcm_header.frametag[0] <
                (sizeof(sdpcm_header.frametag) + sizeof(sdpcm_sw_header_t) + sizeof(cdc_header_t) ) )
            {
                /* Received a too-short SDPCM packet! */
                WPRINT_WHD_DEBUG( ("Received a too-short SDPCM packet!\n") );
                result = whd_buffer_release(whd_driver, buffer, WHD_NETWORK_RX);
                if (result != WHD_SUCCESS)
                    WPRINT_WHD_ERROR( ("buffer release failed in %s at %d \n", __func__, __LINE__) );

                break;
            }

            /* Move SDPCM header and Buffer header to pass onto next layer */
            whd_buffer_add_remove_at_front(whd_driver, &buffer,
                                           (int32_t)(sizeof(whd_buffer_header_t) +
                                                     sdpcm_header.sw_header.header_length) );

            whd_process_cdc(whd_driver, buffer);
        }

        break;

        case DATA_HEADER:
        {
            /* Check that the packet is big enough to contain SDPCM & BDC headers */
            if (sdpcm_header.frametag[0] <=
                (sizeof(sdpcm_header.frametag) + sizeof(sdpcm_sw_header_t) + sizeof(bdc_header_t) ) )
            {
                WPRINT_WHD_ERROR( ("Packet too small to contain SDPCM + BDC headers\n") );
                result = whd_buffer_release(whd_driver, buffer, WHD_NETWORK_RX);
                if (result != WHD_SUCCESS)
                    WPRINT_WHD_ERROR( ("buffer release failed in %s at %d \n", __func__, __LINE__) );

                break;
            }

            /* Move SDPCM header and Buffer header to pass onto next layer */
            whd_buffer_add_remove_at_front(whd_driver, &buffer,
                                           (int32_t)(sizeof(whd_buffer_header_t) +
                                                     sdpcm_header.sw_header.header_length) );

            whd_process_bdc(whd_driver, buffer);

        }
        break;

        case ASYNCEVENT_HEADER:
        {

            /* Move SDPCM header and Buffer header to pass onto next layer */
            whd_buffer_add_remove_at_front(whd_driver, &buffer,
                                           (int32_t)(sizeof(whd_buffer_header_t) +
                                                     sdpcm_header.sw_header.header_length) );

            whd_process_bdc_event(whd_driver, buffer, size);
        }
        break;

        default:
            whd_minor_assert("SDPCM packet of unknown channel received - dropping packet", 0 != 0);
            result = whd_buffer_release(whd_driver, buffer, WHD_NETWORK_RX);
            if (result != WHD_SUCCESS)
                WPRINT_WHD_ERROR( ("buffer release failed in %s at %d \n", __func__, __LINE__) );
            break;
    }
}

whd_bool_t whd_sdpcm_has_tx_packet(whd_driver_t whd_driver)
{
    if (whd_driver->sdpcm_info.send_queue_head != NULL)
    {
        return WHD_TRUE;
    }

    return WHD_FALSE;
}

whd_result_t whd_sdpcm_get_packet_to_send(whd_driver_t whd_driver, whd_buffer_t *buffer)
{
    bus_common_header_t *packet;
    sdpcm_header_t sdpcm_header;
    whd_sdpcm_info_t *sdpcm_info = &whd_driver->sdpcm_info;
    whd_result_t result;

    if (sdpcm_info->send_queue_head != NULL)
    {
        /* Check if we're being flow controlled */
        if (whd_bus_is_flow_controlled(whd_driver) == WHD_TRUE)
        {
            WHD_STATS_INCREMENT_VARIABLE(whd_driver, flow_control);
            return WHD_FLOW_CONTROLLED;
        }

        /* Check if we have enough bus data credits spare */
        if (sdpcm_info->packet_transmit_sequence_number == sdpcm_info->last_bus_data_credit)
        {
            WHD_STATS_INCREMENT_VARIABLE(whd_driver, no_credit);
            return WHD_NO_CREDITS;
        }

        /* There is a packet waiting to be sent - send it then fix up queue and release packet */
        if (cy_rtos_get_semaphore(&sdpcm_info->send_queue_mutex, CY_RTOS_NEVER_TIMEOUT, WHD_FALSE) != WHD_SUCCESS)
        {
            /* Could not obtain mutex, push back the flow control semaphore */
            WPRINT_WHD_ERROR( ("Error manipulating a semaphore, %s failed at %d \n", __func__, __LINE__) );
            return WHD_SEMAPHORE_ERROR;
        }

        /* Pop the head off and set the new send_queue head */
        *buffer = sdpcm_info->send_queue_head;
        sdpcm_info->send_queue_head = whd_sdpcm_get_next_buffer_in_queue(whd_driver, *buffer);
        if (sdpcm_info->send_queue_head == NULL)
        {
            sdpcm_info->send_queue_tail = NULL;
        }
        result = cy_rtos_set_semaphore(&sdpcm_info->send_queue_mutex, WHD_FALSE);
        if (result != WHD_SUCCESS)
            WPRINT_WHD_ERROR( ("Error setting semaphore in %s at %d \n", __func__, __LINE__) );


        /* Set the sequence number */
        packet = (bus_common_header_t *)whd_buffer_get_current_piece_data_pointer(whd_driver, *buffer);
        memcpy(&sdpcm_header, packet->bus_header, BUS_HEADER_LEN);
        sdpcm_header.sw_header.sequence = sdpcm_info->packet_transmit_sequence_number;
        memcpy(packet->bus_header, &sdpcm_header, BUS_HEADER_LEN);
        sdpcm_info->packet_transmit_sequence_number++;

        return WHD_SUCCESS;
    }
    else
    {
        return WHD_NO_PACKET_TO_SEND;
    }
}

/** Returns the number of bus credits available
 *
 * @return The number of bus credits available
 */
uint8_t whd_sdpcm_get_available_credits(whd_driver_t whd_driver)
{
    return (uint8_t)(whd_driver->sdpcm_info.last_bus_data_credit -
                     whd_driver->sdpcm_info.packet_transmit_sequence_number);
}

/** Writes SDPCM headers and sends packet to WHD Thread
 *
 *  Prepends the given packet with a new SDPCM header,
 *  then passes the packet to the WHD thread via a queue
 *
 *  This function is called by @ref whd_network_send_ethernet_data and @ref whd_cdc_send_ioctl
 *
 *  @param buffer     : The handle of the packet buffer to send
 *  @param header_type  : DATA_HEADER, ASYNCEVENT_HEADER or CONTROL_HEADER - indicating what type of SDPCM packet this is.
 */

void whd_send_to_bus(whd_driver_t whd_driver, whd_buffer_t buffer,
                     sdpcm_header_type_t header_type)
{
    uint16_t size;
    bus_common_header_t *packet =
        (bus_common_header_t *)whd_buffer_get_current_piece_data_pointer(whd_driver, buffer);
    sdpcm_header_t sdpcm_header;
    whd_sdpcm_info_t *sdpcm_info = &whd_driver->sdpcm_info;
    whd_result_t result;

    size = whd_buffer_get_current_piece_size(whd_driver, buffer);

    size = (uint16_t)(size - (uint16_t)sizeof(whd_buffer_header_t) );

    /* Prepare the SDPCM header */
    memset( (uint8_t *)&sdpcm_header, 0, sizeof(sdpcm_header_t) );
    sdpcm_header.sw_header.channel_and_flags = (uint8_t)header_type;
    sdpcm_header.sw_header.header_length =
        (header_type == DATA_HEADER) ? sizeof(sdpcm_header_t) + 2 : sizeof(sdpcm_header_t);
    sdpcm_header.sw_header.sequence = 0; /* Note: The real sequence will be written later */
    sdpcm_header.frametag[0] = size;
    sdpcm_header.frametag[1] = (uint16_t) ~size;

    memcpy(packet->bus_header, &sdpcm_header, BUS_HEADER_LEN);

    add_sdpcm_log_entry(LOG_TX, (header_type == DATA_HEADER) ? DATA : (header_type == CONTROL_HEADER) ? IOCTL : EVENT,
                        whd_buffer_get_current_piece_size(whd_driver, buffer),
                        (char *)whd_buffer_get_current_piece_data_pointer(whd_driver, buffer) );

    /* Add the length of the SDPCM header and pass "down" */
    if (cy_rtos_get_semaphore(&sdpcm_info->send_queue_mutex, CY_RTOS_NEVER_TIMEOUT, WHD_FALSE) != WHD_SUCCESS)
    {
        /* Could not obtain mutex */
        /* Fatal error */
        result = whd_buffer_release(whd_driver, buffer, WHD_NETWORK_TX);
        if (result != WHD_SUCCESS)
            WPRINT_WHD_ERROR( ("buffer release failed in %s at %d \n", __func__, __LINE__) )
            return;
    }

    whd_sdpcm_set_next_buffer_in_queue(whd_driver, NULL, buffer);
    if (sdpcm_info->send_queue_tail != NULL)
    {
        whd_sdpcm_set_next_buffer_in_queue(whd_driver, buffer, sdpcm_info->send_queue_tail);
    }
    sdpcm_info->send_queue_tail = buffer;
    if (sdpcm_info->send_queue_head == NULL)
    {
        sdpcm_info->send_queue_head = buffer;
    }
    result = cy_rtos_set_semaphore(&sdpcm_info->send_queue_mutex, WHD_FALSE);
    if (result != WHD_SUCCESS)
        WPRINT_WHD_ERROR( ("Error setting semaphore in %s at %d \n", __func__, __LINE__) );

    whd_thread_notify(whd_driver);
}

/******************************************************
*             Static Functions
******************************************************/

static whd_buffer_t whd_sdpcm_get_next_buffer_in_queue(whd_driver_t whd_driver, whd_buffer_t buffer)
{
    whd_buffer_header_t *packet = (whd_buffer_header_t *)whd_buffer_get_current_piece_data_pointer(whd_driver, buffer);
    return packet->queue_next;
}

/** Sets the next buffer in the send queue
 *
 *  The send queue is a linked list of packet buffers where the 'next' pointer
 *  is stored in the first 4 bytes of the buffer content.
 *  This function sets that pointer.
 *
 * @param buffer       : handle of packet in the send queue
 *        prev_buffer  : handle of new packet whose 'next' pointer will point to 'buffer'
 */
static void whd_sdpcm_set_next_buffer_in_queue(whd_driver_t whd_driver, whd_buffer_t buffer, whd_buffer_t prev_buffer)
{
    whd_buffer_header_t *packet =
        (whd_buffer_header_t *)whd_buffer_get_current_piece_data_pointer(whd_driver, prev_buffer);
    packet->queue_next = buffer;
}

