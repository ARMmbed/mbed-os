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

#include <limits.h>
#include "whd_int.h"
#include "whd_cdc_bdc.h"
#include "whd_events_int.h"
#include "cyabs_rtos.h"
#include "whd_network_types.h"
#include "whd_types_int.h"
#include "whd_wlioctl.h"
#include "whd_thread_internal.h"
#include "whd_buffer_api.h"
#include "whd_network_if.h"

/******************************************************
*        Constants
******************************************************/

#define BDC_PROTO_VER                  (2)      /** Version number of BDC header */
#define BDC_FLAG_VER_SHIFT             (4)      /** Number of bits to shift BDC version number in the flags field */
#define BDC_FLAG2_IF_MASK           (0x0f)

#define ETHER_TYPE_BRCM           (0x886C)      /** Broadcom Ethertype for identifying event packets - Copied from DHD include/proto/ethernet.h */
#define BRCM_OUI            "\x00\x10\x18"      /** Broadcom OUI (Organizationally Unique Identifier): Used in the proprietary(221) IE (Information Element) in all Broadcom devices */

/* QoS related definitions (type of service) */
#define IPV4_DSCP_OFFSET              (15)      /** Offset for finding the DSCP field in an IPv4 header */

#define IOCTL_OFFSET (sizeof(whd_buffer_header_t) + 12 + 16)
#define WHD_IOCTL_PACKET_TIMEOUT      (0xFFFFFFFF)
#define WHD_IOCTL_TIMEOUT_MS         (5000)     /** Need to give enough time for coming out of Deep sleep (was 400) */
#define WHD_IOCTL_MAX_TX_PKT_LEN     (1500)

/******************************************************
*             Macros
******************************************************/

/******************************************************
*             Local Structures
******************************************************/

/******************************************************
*             Static Variables
******************************************************/

static const uint8_t dscp_to_wmm_qos[] =
{ 0, 0, 0, 0, 0, 0, 0, 0,                                       /* 0  - 7 */
  1, 1, 1, 1, 1, 1, 1,                                          /* 8  - 14 */
  1, 1, 1, 1, 1, 1, 1,                                          /* 15 - 21 */
  1, 1, 0, 0, 0, 0, 0,                                          /* 22 - 28 */
  0, 0, 0, 5, 5, 5, 5,                                          /* 29 - 35 */
  5, 5, 5, 5, 5, 5, 5,                                          /* 36 - 42 */
  5, 5, 5, 5, 5, 7, 7,                                          /* 43 - 49 */
  7, 7, 7, 7, 7, 7, 7,                                          /* 50 - 56 */
  7, 7, 7, 7, 7, 7, 7,                                          /* 57 - 63 */
};

/******************************************************
*             Static Function Prototypes
******************************************************/

static uint8_t         whd_map_dscp_to_priority(whd_driver_t whd_driver, uint8_t dscp_val);

/******************************************************
*             Static Functions
******************************************************/

/** Map a DSCP value from an IP header to a WMM QoS priority
 *
 * @param dscp_val : DSCP value from IP header
 *
 * @return wmm_qos : WMM priority
 *
 */
static uint8_t whd_map_dscp_to_priority(whd_driver_t whd_driver, uint8_t val)
{
    uint8_t dscp_val = (uint8_t)(val >> 2); /* DSCP field is the high 6 bits of the second byte of an IPv4 header */

    return dscp_to_wmm_qos[dscp_val];
}

void whd_cdc_bdc_info_deinit(whd_driver_t whd_driver)
{
    whd_cdc_bdc_info_t *cdc_bdc_info = &whd_driver->cdc_bdc_info;

    /* Delete the sleep mutex */
    (void)cy_rtos_deinit_semaphore(&cdc_bdc_info->ioctl_sleep);

    /* Delete the queue mutex.  */
    (void)cy_rtos_deinit_semaphore(&cdc_bdc_info->ioctl_mutex);

    /* Delete the event list management mutex */
    (void)cy_rtos_deinit_semaphore(&cdc_bdc_info->event_list_mutex);
}

whd_result_t whd_cdc_bdc_info_init(whd_driver_t whd_driver)
{
    whd_cdc_bdc_info_t *cdc_bdc_info = &whd_driver->cdc_bdc_info;

    /* Create the mutex protecting the packet send queue */
    if (cy_rtos_init_semaphore(&cdc_bdc_info->ioctl_mutex, 1, 0) != WHD_SUCCESS)
    {
        return WHD_SEMAPHORE_ERROR;
    }
    if (cy_rtos_set_semaphore(&cdc_bdc_info->ioctl_mutex, WHD_FALSE) != WHD_SUCCESS)
    {
        WPRINT_WHD_ERROR( ("Error setting semaphore in %s at %d \n", __func__, __LINE__) );
        return WHD_SEMAPHORE_ERROR;
    }

    /* Create the event flag which signals the whd thread needs to wake up */
    if (cy_rtos_init_semaphore(&cdc_bdc_info->ioctl_sleep, 1, 0) != WHD_SUCCESS)
    {
        cy_rtos_deinit_semaphore(&cdc_bdc_info->ioctl_mutex);
        return WHD_SEMAPHORE_ERROR;
    }

    /* Create semaphore to protect event list management */
    if (cy_rtos_init_semaphore(&cdc_bdc_info->event_list_mutex, 1, 0) != WHD_SUCCESS)
    {
        cy_rtos_deinit_semaphore(&cdc_bdc_info->ioctl_sleep);
        cy_rtos_deinit_semaphore(&cdc_bdc_info->ioctl_mutex);
        return WHD_SEMAPHORE_ERROR;
    }
    if (cy_rtos_set_semaphore(&cdc_bdc_info->event_list_mutex, WHD_FALSE) != WHD_SUCCESS)
    {
        WPRINT_WHD_ERROR( ("Error setting semaphore in %s at %d \n", __func__, __LINE__) );
        return WHD_SEMAPHORE_ERROR;
    }

    /* Initialise the list of event handler functions */
    memset(cdc_bdc_info->whd_event_list, 0, sizeof(cdc_bdc_info->whd_event_list) );

    return WHD_SUCCESS;
}

/** Sends an IOCTL command
 *
 *  Sends a I/O Control command to the Broadcom 802.11 device.
 *  The data which is set or retrieved must be in a format structure which is appropriate for the particular
 *  I/O control being sent. These structures can only be found in the DHD source code such as wl/exe/wlu.c.
 *  The I/O control will always respond with a packet buffer which may contain data in a format specific to
 *  the I/O control being used.
 *
 *  @Note: The caller is responsible for releasing the response buffer.
 *  @Note: The function blocks until the IOCTL has completed
 *  @Note: Only one IOCTL may happen simultaneously.
 *
 *  @param type       : CDC_SET or CDC_GET - indicating whether to set or get the I/O control
 *  @param send_buffer_hnd : A handle for a packet buffer containing the data value to be sent.
 *  @param response_buffer_hnd : A pointer which will receive the handle for the packet buffer
 *                               containing the response data value received.
 *  @param interface : Which interface to send the iovar to (WHD_STA_INTERFACE or WHD_AP_INTERFACE)
 *
 *  @return    WHD result code
 */
whd_result_t whd_cdc_send_ioctl(whd_interface_t ifp, cdc_command_type_t type, uint32_t command,
                                whd_buffer_t send_buffer_hnd,
                                whd_buffer_t *response_buffer_hnd)
{

    uint32_t data_length;
    uint32_t flags;
    uint32_t requested_ioctl_id;
    whd_result_t retval;
    control_header_t *send_packet;
    cdc_header_t *cdc_header;
    uint32_t bss_index = ifp->bsscfgidx;
    whd_driver_t whd_driver = ifp->whd_driver;
    whd_cdc_bdc_info_t *cdc_bdc_info = &whd_driver->cdc_bdc_info;

    /* Validate the command value */
    if (command > INT_MAX)
    {
        WPRINT_WHD_ERROR( ("The ioctl command value is invalid\n") );
        return WHD_BADARG;
    }

    /* Acquire mutex which prevents multiple simultaneous IOCTLs */
    retval = cy_rtos_get_semaphore(&cdc_bdc_info->ioctl_mutex, CY_RTOS_NEVER_TIMEOUT, WHD_FALSE);
    if (retval != WHD_SUCCESS)
    {
        CHECK_RETURN(whd_buffer_release(whd_driver, send_buffer_hnd, WHD_NETWORK_TX) );
        return retval;
    }

    /* Count request ioctl ID after acquiring ioctl mutex */
    requested_ioctl_id = (uint32_t)(++cdc_bdc_info->requested_ioctl_id);

    /* Get the data length and cast packet to a CDC BUS header */
    data_length =
        (uint32_t)(whd_buffer_get_current_piece_size(whd_driver,
                                                     send_buffer_hnd) - sizeof(bus_common_header_t) -
                   sizeof(cdc_header_t) );

    send_packet = (control_header_t *)whd_buffer_get_current_piece_data_pointer(whd_driver, send_buffer_hnd);

    WHD_IOCTL_LOG_ADD(ifp->whd_driver, command, send_buffer_hnd);

    /* Check if IOCTL is actually IOVAR */
    if ( (command == WLC_SET_VAR) || (command == WLC_GET_VAR) )
    {
        uint8_t *data = (uint8_t *)DATA_AFTER_HEADER(send_packet);
        uint8_t *ptr = data;

        /* Calculate the offset added to compensate for IOVAR string creating unaligned data section */
        while (*ptr == 0)
        {
            ptr++;
        }
        if (data != ptr)
        {
            data_length -= (uint32_t)(ptr - data);
            memmove(data, ptr, data_length);
            CHECK_RETURN(whd_buffer_set_size(whd_driver, send_buffer_hnd,
                                             (uint16_t)(data_length + sizeof(bus_common_header_t) +
                                                        sizeof(cdc_header_t) ) ) );
        }
    }

    /* Prepare the CDC header */
    send_packet->cdc_header.cmd    = htod32(command);
    send_packet->cdc_header.len    = htod32(data_length);

    send_packet->cdc_header.flags  = ( (requested_ioctl_id << CDCF_IOC_ID_SHIFT)
                                       & CDCF_IOC_ID_MASK ) | type | bss_index << CDCF_IOC_IF_SHIFT;
    send_packet->cdc_header.flags = htod32(send_packet->cdc_header.flags);

    send_packet->cdc_header.status = 0;

    /* Manufacturing test can receive big buffers, but sending big buffers causes a wlan firmware error */
    /* Even though data portion needs to be truncated, cdc_header should have the actual length of the ioctl packet */
    if (whd_buffer_get_current_piece_size(whd_driver, send_buffer_hnd) > WHD_IOCTL_MAX_TX_PKT_LEN)
    {
        CHECK_RETURN(whd_buffer_set_size(whd_driver, send_buffer_hnd, WHD_IOCTL_MAX_TX_PKT_LEN) );
    }

    /* Store the length of the data and the IO control header and pass "down" */
    whd_send_to_bus(whd_driver, send_buffer_hnd, CONTROL_HEADER);


    /* Wait till response has been received  */
    retval = cy_rtos_get_semaphore(&cdc_bdc_info->ioctl_sleep, (uint32_t)WHD_IOCTL_TIMEOUT_MS, WHD_FALSE);
    if (retval != WHD_SUCCESS)
    {
        /* Release the mutex since ioctl response will no longer be referenced. */
        CHECK_RETURN(cy_rtos_set_semaphore(&cdc_bdc_info->ioctl_mutex, WHD_FALSE) );
        return retval;
    }

    cdc_header    = (cdc_header_t *)whd_buffer_get_current_piece_data_pointer(whd_driver, cdc_bdc_info->ioctl_response);
    flags         = dtoh32(cdc_header->flags);

    retval = (whd_result_t)(WLAN_ENUM_OFFSET -  dtoh32(cdc_header->status) );

    /* Check if the caller wants the response */
    if (response_buffer_hnd != NULL)
    {
        *response_buffer_hnd = cdc_bdc_info->ioctl_response;
        CHECK_RETURN(whd_buffer_add_remove_at_front(whd_driver, response_buffer_hnd, sizeof(cdc_header_t) ) );
    }
    else
    {
        CHECK_RETURN(whd_buffer_release(whd_driver, cdc_bdc_info->ioctl_response, WHD_NETWORK_RX) );
    }

    cdc_bdc_info->ioctl_response = NULL;

    /* Release the mutex since ioctl response will no longer be referenced. */
    CHECK_RETURN(cy_rtos_set_semaphore(&cdc_bdc_info->ioctl_mutex, WHD_FALSE) );

    /* Check whether the IOCTL response indicates it failed. */
    if ( (flags & CDCF_IOC_ERROR) != 0 )
    {
        if (response_buffer_hnd != NULL)
        {
            CHECK_RETURN(whd_buffer_release(whd_driver, *response_buffer_hnd, WHD_NETWORK_RX) );
            *response_buffer_hnd = NULL;
        }
        whd_minor_assert("IOCTL failed\n", 0 != 0);
        return retval;
    }

    return WHD_SUCCESS;
}

/** Sets/Gets an I/O Variable (IOVar)
 *
 *  This function either sets or retrieves the value of an I/O variable from the Broadcom 802.11 device.
 *  The data which is set or retrieved must be in a format structure which is appropriate for the particular
 *  I/O variable being accessed. These structures can only be found in the DHD source code such as wl/exe/wlu.c.
 *
 *  @Note: The function blocks until the I/O variable read/write has completed
 *
 * @param type       : CDC_SET or CDC_GET - indicating whether to set or get the I/O variable value
 * @param send_buffer_hnd : A handle for a packet buffer containing the data value to be sent.
 * @param response_buffer_hnd : A pointer which will receive the handle for the packet buffer
 *                              containing the response data value received.
 * @param interface : Which interface to send the iovar to (AP or STA)
 *
 * @return    WHD result code
 */
whd_result_t whd_cdc_send_iovar(whd_interface_t ifp, cdc_command_type_t type,
                                whd_buffer_t send_buffer_hnd,
                                whd_buffer_t *response_buffer_hnd)
{
    if (type == CDC_SET)
    {
        return whd_cdc_send_ioctl(ifp, CDC_SET, (uint32_t)WLC_SET_VAR, send_buffer_hnd, response_buffer_hnd);
    }
    else
    {
        return whd_cdc_send_ioctl(ifp, CDC_GET, (uint32_t)WLC_GET_VAR, send_buffer_hnd, response_buffer_hnd);
    }
}

/** A helper function to easily acquire and initialise a buffer destined for use as an iovar
 *
 * @param  buffer      : A pointer to a whd_buffer_t object where the created buffer will be stored
 * @param  data_length : The length of space reserved for user data
 * @param  name        : The name of the iovar
 *
 * @return A pointer to the start of user data with data_length space available
 */
void *whd_cdc_get_iovar_buffer(whd_driver_t whd_driver,
                               whd_buffer_t *buffer,
                               uint16_t data_length,
                               const char *name)
{
    uint32_t name_length = (uint32_t)strlen(name) + 1;    /* + 1 for terminating null */
    uint32_t name_length_alignment_offset = (64 - name_length) % sizeof(uint32_t);

    if (whd_host_buffer_get(whd_driver, buffer, WHD_NETWORK_TX,
                            (unsigned short)(IOCTL_OFFSET + data_length + name_length + name_length_alignment_offset),
                            (unsigned long)WHD_IOCTL_PACKET_TIMEOUT) == WHD_SUCCESS)
    {
        uint8_t *data = (whd_buffer_get_current_piece_data_pointer(whd_driver, *buffer) + IOCTL_OFFSET);
        memset(data, 0, name_length_alignment_offset);
        memcpy(data + name_length_alignment_offset, name, name_length);
        return (data + name_length + name_length_alignment_offset);
    }
    else
    {
        WPRINT_WHD_ERROR( ("Error - failed to allocate a packet buffer for IOVAR\n") );
        return NULL;
    }
}

/** Sends a data packet.
 *
 *  This function should be called by the bottom of the network stack in order for it
 *  to send an ethernet frame.
 *  The function prepends a BDC header, before sending to @ref whd_send_to_bus where
 *  the BUS header will be added
 *
 * @param buffer  : The ethernet packet buffer to be sent
 * @param interface : the interface over which to send the packet (AP or STA)
 *
 */
/* Returns immediately - whd_buffer_tx_completed will be called once the transmission has finished */
void whd_network_send_ethernet_data(whd_interface_t ifp, whd_buffer_t buffer)
{
    data_header_t *packet;
    whd_result_t result;
    uint8_t *dscp = NULL;
    uint8_t priority = 0;
    uint8_t whd_tos_map[8] = { 0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07 };
    whd_driver_t whd_driver = ifp->whd_driver;
    ethernet_header_t *ethernet_header = (ethernet_header_t *)whd_buffer_get_current_piece_data_pointer(
        whd_driver, buffer);
    uint16_t ether_type;

    ether_type = ntoh16(ethernet_header->ethertype);
    if ( (ether_type == WHD_ETHERTYPE_IPv4) || (ether_type == WHD_ETHERTYPE_DOT1AS) )
    {
        dscp = (uint8_t *)whd_buffer_get_current_piece_data_pointer(whd_driver, buffer) + IPV4_DSCP_OFFSET;
    }

    WPRINT_WHD_DATA_LOG( ("Wcd:> DATA pkt 0x%08lX len %d\n", (unsigned long)buffer,
                          (int)whd_buffer_get_current_piece_size(whd_driver, buffer) ) );


    /* Add link space at front of packet */
    result = whd_buffer_add_remove_at_front(whd_driver, &buffer, -(int)(sizeof(data_header_t) ) );
    if (result != WHD_SUCCESS)
    {
        WPRINT_WHD_DEBUG( ("Unable to adjust header space\n") );
        result = whd_buffer_release(ifp->whd_driver, buffer, WHD_NETWORK_TX);
        if (result != WHD_SUCCESS)
            WPRINT_WHD_ERROR( ("buffer release failed in %s at %d \n", __func__, __LINE__) );
        return;
    }

    packet = (data_header_t *)whd_buffer_get_current_piece_data_pointer(whd_driver, buffer);

    if (ifp->bsscfgidx > WHD_INTERFACE_MAX)
    {
        WPRINT_WHD_DEBUG( ("No interface for packet send\n") );
        result = whd_buffer_release(ifp->whd_driver, buffer, WHD_NETWORK_TX);
        if (result != WHD_SUCCESS)
            WPRINT_WHD_ERROR( ("buffer release failed in %s at %d \n", __func__, __LINE__) );
        return;
    }

    /* Prepare the BDC header */
    packet->bdc_header.flags    = 0;
    packet->bdc_header.flags    = (uint8_t)(BDC_PROTO_VER << BDC_FLAG_VER_SHIFT);
    /* If it's an IPv4 packet set the BDC header priority based on the DSCP field */
    if ( ( (ether_type == WHD_ETHERTYPE_IPv4) || (ether_type == WHD_ETHERTYPE_DOT1AS) ) && (dscp != NULL) )
    {
        if (*dscp != 0) /* If it's equal 0 then it's best effort traffic and nothing needs to be done */
        {
            priority = whd_map_dscp_to_priority(whd_driver, *dscp);
        }
    }

    /* If STA interface, re-map prio to the prio allowed by the AP, regardless of whether it's an IPv4 packet */
    if (ifp->role == WHD_STA_ROLE)
    {
        packet->bdc_header.priority = whd_tos_map[priority];
    }
    else
    {
        packet->bdc_header.priority = priority;
    }

    packet->bdc_header.flags2   = ifp->bsscfgidx;
    packet->bdc_header.data_offset = 0;

    /* Add the length of the BDC header and pass "down" */
    whd_send_to_bus(whd_driver, buffer, DATA_HEADER);

}

/** A helper function to easily acquire and initialise a buffer destined for use as an ioctl
 *
 * @param  buffer      : A pointer to a whd_buffer_t object where the created buffer will be stored
 * @param  data_length : The length of space reserved for user data
 *
 * @return A pointer to the start of user data with data_length space available
 */
void *whd_cdc_get_ioctl_buffer(whd_driver_t whd_driver,
                               whd_buffer_t *buffer,
                               uint16_t data_length)
{
    if ((uint32_t)IOCTL_OFFSET + data_length > USHRT_MAX)
    {
        WPRINT_WHD_ERROR( ("The reserved ioctl buffer length is over %u\n", USHRT_MAX) );
        return NULL;
    }
    if (whd_host_buffer_get(whd_driver, buffer, WHD_NETWORK_TX, (unsigned short)(IOCTL_OFFSET + data_length),
                            (unsigned long)WHD_IOCTL_PACKET_TIMEOUT) == WHD_SUCCESS)
    {
        return (whd_buffer_get_current_piece_data_pointer(whd_driver, *buffer) + IOCTL_OFFSET);
    }
    else
    {
        WPRINT_WHD_ERROR( ("Error - failed to allocate a packet buffer for IOCTL\n") );
        return NULL;
    }
}

/** Processes CDC header information received in the RX packet and sets IOCTL response buffer
 *
 * @param  whd_driver : WHD driver instance
 * @param  buffer     : A pointer to a whd_buffer_t object where the created buffer will be stored
 */

void whd_process_cdc(whd_driver_t whd_driver, whd_buffer_t buffer)
{
    uint32_t flags;
    uint16_t id;
    whd_cdc_bdc_info_t *cdc_bdc_info = &whd_driver->cdc_bdc_info;
    whd_result_t result;
    cdc_header_t *cdc_header = (cdc_header_t *)whd_buffer_get_current_piece_data_pointer(whd_driver, buffer);
    whd_result_t ioctl_mutex_res;

    flags         = dtoh32(cdc_header->flags);
    id            = (uint16_t)( (flags & CDCF_IOC_ID_MASK) >> CDCF_IOC_ID_SHIFT );

    /* Validate request ioctl ID and check if whd_cdc_send_ioctl is still waiting for response*/
    if ( ( (ioctl_mutex_res = cy_rtos_get_semaphore(&cdc_bdc_info->ioctl_mutex, 0, WHD_FALSE) ) != WHD_SUCCESS ) &&
         (id == cdc_bdc_info->requested_ioctl_id) )
    {
        /* Save the response packet in a variable */
        cdc_bdc_info->ioctl_response = buffer;

        WPRINT_WHD_DATA_LOG( ("Wcd:< Procd pkt 0x%08lX: IOCTL Response (%d bytes)\n", (unsigned long)buffer,
                              size) );

        /* Wake the thread which sent the IOCTL/IOVAR so that it will resume */
        result = cy_rtos_set_semaphore(&cdc_bdc_info->ioctl_sleep, WHD_FALSE);
        if (result != WHD_SUCCESS)
            WPRINT_WHD_ERROR( ("Error setting semaphore in %s at %d \n", __func__, __LINE__) );

    }
    else
    {
        WPRINT_WHD_ERROR( ("Received buffer request ID: %d (expectation: %d)\n",
                           id, cdc_bdc_info->requested_ioctl_id) );
        if (ioctl_mutex_res == WHD_SUCCESS)
        {
            WPRINT_WHD_ERROR( ("whd_cdc_send_ioctl is already timed out, drop the buffer\n") );
            result = cy_rtos_set_semaphore(&cdc_bdc_info->ioctl_mutex, WHD_FALSE);
            if (result != WHD_SUCCESS)
            {
                WPRINT_WHD_ERROR( ("Error setting semaphore in %s at %d \n", __func__, __LINE__) );
            }
        }
        else
        {
            WPRINT_WHD_ERROR( ("Received a response for a different IOCTL - retry\n") );
        }

        result = whd_buffer_release(whd_driver, buffer, WHD_NETWORK_RX);
        if (result != WHD_SUCCESS)
            WPRINT_WHD_ERROR( ("buffer release failed in %s at %d \n", __func__, __LINE__) );
    }
}

/** Processes BDC header information received in the RX packet and sends data to network stack
 *
 * @param  whd_driver : WHD driver instance
 * @param  buffer     : A pointer to a whd_buffer_t object where the created buffer will be stored
 */

void whd_process_bdc(whd_driver_t whd_driver, whd_buffer_t buffer)
{
    int32_t headers_len_below_payload;
    uint32_t ip_data_start_add;
    uint32_t bssid_index;
    whd_interface_t ifp;
    whd_result_t result;
    bdc_header_t *bdc_header = (bdc_header_t *)whd_buffer_get_current_piece_data_pointer(whd_driver, buffer);

    /* Calculate where the payload is */
    headers_len_below_payload =
        (int32_t)( (int32_t)BDC_HEADER_LEN + (int32_t)(bdc_header->data_offset << 2) );

    /* Move buffer pointer past gSPI, BUS, BCD headers and padding,
     * so that the network stack or 802.11 monitor sees only the payload */
    if (WHD_SUCCESS != whd_buffer_add_remove_at_front(whd_driver, &buffer, headers_len_below_payload) )
    {
        WPRINT_WHD_ERROR( ("No space for headers without chaining. this should never happen\n") );
        result = whd_buffer_release(whd_driver, buffer, WHD_NETWORK_RX);
        if (result != WHD_SUCCESS)
            WPRINT_WHD_ERROR( ("buffer release failed in %s at %d \n", __func__, __LINE__) );

        return;
    }

    /* It is preferable to have IP data at address aligned to 4 bytes. IP data startes after ethernet header */
    ip_data_start_add =
        (uint32_t )whd_buffer_get_current_piece_data_pointer(whd_driver, buffer) + WHD_ETHERNET_SIZE;
    if ( ( (ip_data_start_add >> 2) << 2 ) != ip_data_start_add )
    {
        WPRINT_WHD_DATA_LOG( ("IP data not aligned to 4 bytes %lx\n", ip_data_start_add) );
    }

    WPRINT_WHD_DATA_LOG( ("Wcd:< Procd pkt 0x%08lX: Data (%d bytes)\n", (unsigned long)buffer, size) );
    bssid_index = (uint32_t)(bdc_header->flags2 & BDC_FLAG2_IF_MASK);
    ifp = whd_driver->iflist[bssid_index];

    /* Send packet to bottom of network stack */
    result = whd_network_process_ethernet_data(ifp, buffer);
    if (result != WHD_SUCCESS)
        WPRINT_WHD_ERROR( ("%s failed at %d \n", __func__, __LINE__) );
}

/** Processes BDC header information and extracts the event packets
 * Event Packets are decoded to determine which event occurred, and the event handler list is consulted
 * and the appropriate event handler is called
 *
 * @param  whd_driver : WHD driver instance
 * @param  buffer     : A pointer to a whd_buffer_t object where the created buffer will be stored
 * @param  size       : Size of the complete packet received from WLAN device
 */
void whd_process_bdc_event(whd_driver_t whd_driver, whd_buffer_t buffer, uint16_t size)
{
    uint16_t ether_type;
    whd_event_header_t *whd_event;
    whd_event_t *event;
    whd_cdc_bdc_info_t *cdc_bdc_info = &whd_driver->cdc_bdc_info;
    whd_result_t result;
    bdc_header_t *bdc_header = (bdc_header_t *)whd_buffer_get_current_piece_data_pointer(whd_driver, buffer);
    uint16_t i;
    uint16_t j;

    event = (whd_event_t *)&bdc_header[bdc_header->data_offset + 1];

    ether_type = ntoh16(event->eth.ethertype);

    /* If frame is truly an event, it should have EtherType equal to the Broadcom type. */
    if (ether_type != (uint16_t)ETHER_TYPE_BRCM)
    {
        WPRINT_WHD_DEBUG( ("Error - received a channel 1 packet which was not BRCM ethertype\n") );
        result = whd_buffer_release(whd_driver, buffer, WHD_NETWORK_RX);
        if (result != WHD_SUCCESS)
            WPRINT_WHD_ERROR( ("buffer release failed in %s at %d \n", __func__, __LINE__) );
        return;
    }

    /* If ethertype is correct, the contents of the ethernet packet
     * are a structure of type bcm_event_t
     */

    /* Check that the OUI matches the Broadcom OUI */
    if (0 != memcmp(BRCM_OUI, &event->eth_evt_hdr.oui[0], (size_t)DOT11_OUI_LEN) )
    {
        WPRINT_WHD_DEBUG( ("Event OUI mismatch\n") );
        result = whd_buffer_release(whd_driver, buffer, WHD_NETWORK_RX);
        if (result != WHD_SUCCESS)
            WPRINT_WHD_ERROR( ("buffer release failed in %s at %d \n", __func__, __LINE__) );
        return;
    }

    whd_event = &event->whd_event;

    /* Search for the event type in the list of event handler functions
     * event data is stored in network endianness
     */
    whd_event->flags      =                        ntoh16(whd_event->flags);
    whd_event->event_type = (whd_event_num_t)ntoh32(whd_event->event_type);
    whd_event->status     = (whd_event_status_t)ntoh32(whd_event->status);
    whd_event->reason     = (whd_event_reason_t)ntoh32(whd_event->reason);
    whd_event->auth_type  =                        ntoh32(whd_event->auth_type);
    whd_event->datalen    =                        ntoh32(whd_event->datalen);

    /* Ensure data length is correct */
    if (whd_event->datalen >
        (uint32_t)(size - ( (char *)DATA_AFTER_HEADER(event) - (char *)bdc_header ) ) )
    {
        WPRINT_WHD_ERROR( (
                              "Error - (data length received [%d] > expected data length [%d]). Bus header packet size = [%d]. Ignoring the packet\n",
                              (int)whd_event->datalen,
                              size - ( (char *)DATA_AFTER_HEADER(event) - (char *)bdc_header ),
                              size) );
        result = whd_buffer_release(whd_driver, buffer, WHD_NETWORK_RX);
        if (result != WHD_SUCCESS)
            WPRINT_WHD_ERROR( ("buffer release failed in %s at %d \n", __func__, __LINE__) );

        return;
    }

    /* This is necessary because people who defined event statuses and reasons overlapped values. */
    if (whd_event->event_type == WLC_E_PSK_SUP)
    {
        whd_event->status = (whd_event_status_t)( (int)whd_event->status + WLC_SUP_STATUS_OFFSET );
        whd_event->reason = (whd_event_reason_t)( (int)whd_event->reason + WLC_E_SUP_REASON_OFFSET );
    }
    else if (whd_event->event_type == WLC_E_PRUNE)
    {
        whd_event->reason = (whd_event_reason_t)( (int)whd_event->reason + WLC_E_PRUNE_REASON_OFFSET );
    }
    else if ( (whd_event->event_type == WLC_E_DISASSOC) || (whd_event->event_type == WLC_E_DEAUTH) )
    {
        whd_event->status = (whd_event_status_t)( (int)whd_event->status + WLC_DOT11_SC_STATUS_OFFSET );
        whd_event->reason = (whd_event_reason_t)( (int)whd_event->reason + WLC_E_DOT11_RC_REASON_OFFSET );
    }

    /* do any needed debug logging of event */
    WHD_IOCTL_LOG_ADD_EVENT(whd_driver, whd_event->event_type, whd_event->status,
                            whd_event->reason);

    if (cy_rtos_get_semaphore(&cdc_bdc_info->event_list_mutex, CY_RTOS_NEVER_TIMEOUT, WHD_FALSE) != WHD_SUCCESS)
    {
        WPRINT_WHD_DEBUG( ("Failed to obtain mutex for event list access!\n") );
        result = whd_buffer_release(whd_driver, buffer, WHD_NETWORK_RX);
        if (result != WHD_SUCCESS)
            WPRINT_WHD_ERROR( ("buffer release failed in %s at %d \n", __func__, __LINE__) );
        return;
    }

    for (i = 0; i < (uint16_t)WHD_EVENT_HANDLER_LIST_SIZE; i++)
    {
        if (cdc_bdc_info->whd_event_list[i].event_set)
        {
            for (j = 0; cdc_bdc_info->whd_event_list[i].events[j] != WLC_E_NONE; ++j)
            {
                if ( (cdc_bdc_info->whd_event_list[i].events[j] == whd_event->event_type) &&
                     (cdc_bdc_info->whd_event_list[i].ifidx == whd_event->ifidx) )
                {
                    /* Correct event type has been found - call the handler function and exit loop */
                    cdc_bdc_info->whd_event_list[i].handler_user_data =
                        cdc_bdc_info->whd_event_list[i].handler(whd_driver->iflist[whd_event->bsscfgidx],
                                                                whd_event,
                                                                (uint8_t *)DATA_AFTER_HEADER(
                                                                    event),
                                                                cdc_bdc_info->whd_event_list[i].handler_user_data);
                    break;
                }
            }
        }
    }

    result = cy_rtos_set_semaphore(&cdc_bdc_info->event_list_mutex, WHD_FALSE);
    if (result != WHD_SUCCESS)
        WPRINT_WHD_ERROR( ("Error setting semaphore in %s at %d \n", __func__, __LINE__) );

    WPRINT_WHD_DATA_LOG( ("Wcd:< Procd pkt 0x%08lX: Evnt %d (%d bytes)\n", (unsigned long)buffer,
                          (int)whd_event->event_type, size) );

    /* Release the event packet buffer */
    result = whd_buffer_release(whd_driver, buffer, WHD_NETWORK_RX);
    if (result != WHD_SUCCESS)
        WPRINT_WHD_ERROR( ("buffer release failed in %s at %d \n", __func__, __LINE__) );

}

