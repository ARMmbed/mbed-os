/*
 * Copyright 2021, Cypress Semiconductor Corporation (an Infineon company)
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

/** @file whd_events.h
 *  Header for Event detection
 *
 *  Provides constants and prototypes for functions that allow
 *  user applications to receive event callbacks and set event handlers
 */
#ifndef INCLUDED_WHD_EVENTS_API_H
#define INCLUDED_WHD_EVENTS_API_H

#include "whd.h"

#ifdef __cplusplus
extern "C"
{
#endif

/* List of events */
#define WLC_E_NONE                         (0x7FFFFFFE) /**< Indicates the end of the event array list */

#define WLC_E_SET_SSID                     0 /**< Indicates status of set SSID. This event occurs when STA tries to join the AP*/
#define WLC_E_AUTH                         3 /**< 802.11 AUTH request event occurs when STA tries to get authenticated with the AP  */
#define WLC_E_DEAUTH                       5 /**< 802.11 DEAUTH request event occurs when the the SOFTAP is stopped to deuthenticate the connected stations*/
#define WLC_E_DEAUTH_IND                   6 /**< 802.11 DEAUTH indication event occurs when the STA gets deauthenticated by the AP */
#define WLC_E_ASSOC                        7 /**< 802.11 ASSOC request event occurs when STA joins the AP */
#define WLC_E_ASSOC_IND                    8 /**< 802.11 ASSOC indication occurs when a station joins the SOFTAP that is started */
#define WLC_E_REASSOC                      9 /**< 802.11 REASSOC request event when the STA again gets associated with the AP */
#define WLC_E_REASSOC_IND                 10 /**< 802.11 REASSOC indication occurs when a station again reassociates with the SOFTAP*/
#define WLC_E_DISASSOC                    11 /**< 802.11 DISASSOC request occurs when the STA the tries to leave the AP*/
#define WLC_E_DISASSOC_IND                12 /**< 802.11 DISASSOC indication occurs when the connected station gets disassociates from SOFTAP,
                                                  also when STA gets diassociated by the AP*/
#define WLC_E_LINK                        16 /**< generic link indication */
#define WLC_E_PROBREQ_MSG                 44 /**< Indicates probe request received for the SOFTAP started*/
#define WLC_E_PSK_SUP                     46 /**< WPA Handshake fail during association*/
#define WLC_E_ACTION_FRAME                59 /**< Indicates Action frame Rx */
#define WLC_E_ACTION_FRAME_COMPLETE       60 /**< Indicates Action frame Tx complete */
#define WLC_E_ESCAN_RESULT                69 /**< escan result event occurs when we scan for the networks */

/* List of status codes - Applicable for any event type */
#define WLC_E_STATUS_SUCCESS        0   /**< operation was successful */
#define WLC_E_STATUS_FAIL           1   /**< operation failed */
#define WLC_E_STATUS_TIMEOUT        2   /**< operation timed out */
#define WLC_E_STATUS_NO_NETWORKS    3   /**< failed due to no matching network found */
#define WLC_E_STATUS_ABORT          4   /**< operation was aborted */
#define WLC_E_STATUS_NO_ACK         5   /**< protocol failure: packet not ack'd */
#define WLC_E_STATUS_UNSOLICITED    6   /**< AUTH or ASSOC packet was unsolicited */
#define WLC_E_STATUS_ATTEMPT        7   /**< attempt to assoc to an auto auth configuration */
#define WLC_E_STATUS_PARTIAL        8   /**< scan results are incomplete */
#define WLC_E_STATUS_NEWSCAN        9   /**< scan aborted by another scan */
#define WLC_E_STATUS_NEWASSOC       10  /**< scan aborted due to assoc in progress */
#define WLC_E_STATUS_11HQUIET       11  /**< 802.11h quiet period started */
#define WLC_E_STATUS_SUPPRESS       12  /**< user disabled scanning (WLC_SET_SCANSUPPRESS) */
#define WLC_E_STATUS_NOCHANS        13  /**< no allowable channels to scan */
#define WLC_E_STATUS_CCXFASTRM      14  /**< scan aborted due to CCX fast roam */
#define WLC_E_STATUS_CS_ABORT       15  /**< abort channel select */
#define WLC_E_STATUS_ERROR          16  /**< request failed due to error */
#define WLC_E_STATUS_INVALID        0xff /**< Invalid status code to init variables. */

#define WLC_SUP_STATUS_OFFSET      (256) /**< Status offset added to the status codes to match the values from firmware. */

/**
 * @brief Status code for event WLC_E_PSK_SUP
 *
 * -Basic supplicant authentication states
 *
 + WLC_SUP_DISCONNECTED
 *     + WLC_SUP_CONNECTING
 *     + WLC_SUP_IDREQUIRED
 *     + WLC_SUP_AUTHENTICATING
 *     + WLC_SUP_AUTHENTICATED
 *     + WLC_SUP_KEYXCHANGE
 *     + WLC_SUP_KEYED
 *     + WLC_SUP_TIMEOUT
 *     + WLC_SUP_LAST_BASIC_STATE
 *  -Extended supplicant authentication states
 *     + WLC_SUP_KEYXCHANGE_WAIT_M1
 *     + WLC_SUP_KEYXCHANGE_PREP_M2
 *     + WLC_SUP_KEYXCHANGE_WAIT_M3
 *     + WLC_SUP_KEYXCHANGE_PREP_M4
 *     + WLC_SUP_KEYXCHANGE_WAIT_G1
 *     + WLC_SUP_KEYXCHANGE_PREP_G2
 */
typedef enum sup_auth_status
{
    WLC_SUP_DISCONNECTED = 0 + WLC_SUP_STATUS_OFFSET,        /**< Disconnected */
    WLC_SUP_CONNECTING = 1 + WLC_SUP_STATUS_OFFSET,          /**< Connecting */
    WLC_SUP_IDREQUIRED = 2 + WLC_SUP_STATUS_OFFSET,          /**< ID Required */
    WLC_SUP_AUTHENTICATING = 3 + WLC_SUP_STATUS_OFFSET,      /**< Authenticating */
    WLC_SUP_AUTHENTICATED = 4 + WLC_SUP_STATUS_OFFSET,       /**< Authenticated */
    WLC_SUP_KEYXCHANGE = 5 + WLC_SUP_STATUS_OFFSET,          /**< Key Exchange */
    WLC_SUP_KEYED = 6 + WLC_SUP_STATUS_OFFSET,               /**< Key Exchanged */
    WLC_SUP_TIMEOUT = 7 + WLC_SUP_STATUS_OFFSET,             /**< Timeout */
    WLC_SUP_LAST_BASIC_STATE = 8 + WLC_SUP_STATUS_OFFSET,    /**< Last Basic State */
    WLC_SUP_KEYXCHANGE_WAIT_M1 = WLC_SUP_AUTHENTICATED,      /**< Waiting to receive handshake msg M1 */
    WLC_SUP_KEYXCHANGE_PREP_M2 = WLC_SUP_KEYXCHANGE,         /**< Preparing to send handshake msg M2 */
    WLC_SUP_KEYXCHANGE_WAIT_M3 = WLC_SUP_LAST_BASIC_STATE,   /**< Waiting to receive handshake msg M3 */
    WLC_SUP_KEYXCHANGE_PREP_M4 = 9 + WLC_SUP_STATUS_OFFSET,  /**< Preparing to send handshake msg M4 */
    WLC_SUP_KEYXCHANGE_WAIT_G1 = 10 + WLC_SUP_STATUS_OFFSET, /**< Waiting to receive handshake msg G1 */
    WLC_SUP_KEYXCHANGE_PREP_G2  = 11 + WLC_SUP_STATUS_OFFSET /**< Preparing to send handshake msg G2 */
} sup_auth_status_t;

#define WHD_MSG_IFNAME_MAX 16 /**< Max length of Interface name */

#pragma pack(1)

/**
 * Structure to store ethernet header fields in event packets
 */
typedef struct whd_event_eth_hdr
{
    uint16_t subtype;      /**< Vendor specific..32769 */
    uint16_t length;       /**< Length of ethernet header*/
    uint8_t version;       /**< Version is 0 */
    uint8_t oui[3];        /**< Organizationally Unique Identifier */
    uint16_t usr_subtype;  /**< User specific data */
} whd_event_eth_hdr_t;

/**
 * Structure to store ethernet destination, source and ethertype in event packets
 */
typedef struct whd_event_ether_header
{
    whd_mac_t destination_address; /**< Ethernet destination address */
    whd_mac_t source_address;      /**< Ethernet source address */
    uint16_t ethertype;            /**< Ethertype for identifying event packets */
} whd_event_ether_header_t;

/**
 *  Structure to store fields after ethernet header in event message
 */
struct whd_event_msg
{
    uint16_t version;               /**< Version */
    uint16_t flags;                 /**< see flags below */
    uint32_t event_type;            /**< Event type indicating a response from firmware for IOCTLs/IOVARs sent */
    uint32_t status;                /**< Status code corresponding to any event type */
    uint32_t reason;                /**< Reason code associated with the event occurred */
    uint32_t auth_type;             /**< WLC_E_AUTH: 802.11 AUTH request */
    uint32_t datalen;               /**< Length of data in event message */
    whd_mac_t addr;                 /**< Station address (if applicable) */
    char ifname[WHD_MSG_IFNAME_MAX];               /**< name of the incoming packet interface */
    uint8_t ifidx;                                 /**< destination OS i/f index */
    uint8_t bsscfgidx;                             /**< source bsscfg index */
};

/**
 * Event structure used by driver msgs
 */
typedef struct whd_event
{
    whd_event_ether_header_t eth;    /**< Variable to store ethernet destination, source and ethertype in event packets */
    whd_event_eth_hdr_t eth_evt_hdr; /**< Variable to store ethernet header fields in event message */
    whd_event_header_t whd_event;    /**< Variable to store rest of the event packet fields after ethernet header */
    /* data portion follows */
} whd_event_t;

#pragma pack()

/** @addtogroup event WHD Event handling API
 *  Functions that allow user applications to receive event callbacks and set event handlers
 *  @{
 */
/** Event handler prototype definition
 *
 *  @param  ifp                  Pointer to handle instance of whd interface
 *  @param  event_header         whd event header
 *  @param  event_data           event data
 *  @param  handler_user_data    semaphore data
 */
typedef void *(*whd_event_handler_t)(whd_interface_t ifp, const whd_event_header_t *event_header,
                                     const uint8_t *event_data, void *handler_user_data);
/** @addtogroup event WHD Error handling API
 *  Functions that allow user applications to receive error callbacks and set error handlers
 *  @{
 */
/** Error handler prototype definition
 *
 *  @param  whd_driver           Pointer to handle instance of whd driver
 *  @param  error_type           whd error type
 *  @param  event_data           event data
 *  @param  handler_user_data    semaphore data
 */
typedef void *(*whd_error_handler_t)(whd_driver_t whd_driver, const uint8_t *error_type,
                                     const uint8_t *event_data, void *handler_user_data);
/** Registers a handler to receive event callbacks.
 *
 *  This function registers a callback handler to be notified when
 *  a particular event is received.
 *
 *
 *  @note   Currently each event may only be registered to one handler and there is a limit to the number of simultaneously
 *          registered events. Maximum of 5 event handlers can registered simultaneously, this also includes the internal
 *          event handler registration which happens during scan, join and starting an AP.
 *
 *  @param  ifp                Pointer to handle instance of whd interface
 *  @param  event_type         Pointer to the event list array
 *  @param  handler_func       A function pointer to the handler callback
 *  @param  handler_user_data  A pointer value which will be passed to the event handler function
 *                             at the time an event is triggered (NULL is allowed)
 *  @param  event_index        Entry where the event handler is registered in the list
 *
 *  @return WHD_SUCCESS or Error code
 */
uint32_t whd_wifi_set_event_handler(whd_interface_t ifp, const uint32_t *event_type, whd_event_handler_t handler_func,
                                    void *handler_user_data, uint16_t *event_index);
/** Registers a handler to receive error callbacks.
 *
 *  This function registers a callback handler to be notified when
 *  a particular event is received.
 *
 *
 *  @note   Currently each event may only be registered to one handler and there is a limit to the number of simultaneously
 *          registered events. Maximum of 5 event handlers can registered simultaneously, this also includes the internal
 *          event handler registration which happens during scan, join and starting an AP.
 *
 *  @param  ifp                Pointer to handle instance of whd interface
 *  @param  error_nums         Pointer to the event list as WLC_ERR_BUS and WLC_ERR_FW
 *  @param  handler_func       A function pointer to the handler callback
 *  @param  handler_user_data  A pointer value which will be passed to the event handler function
 *                             at the time an event is triggered (NULL is allowed)
 *  @param  error_index        Entry where the error handler is registered in the list
 *
 *  @return WHD_SUCCESS or Error code
 */
uint32_t whd_wifi_set_error_handler(whd_interface_t ifp, const uint8_t *error_nums, whd_error_handler_t handler_func,
                                    void *handler_user_data, uint16_t *error_index);

/*  @} */

/** Delete/Deregister the event entry where callback is registered
 *
 *  @param  ifp                Pointer to handle instance of whd interface
 *  @param  event_index        Event index obtained during registration by whd_wifi_set_event_handler
 *
 *  @return WHD_SUCCESS or Error code
 */

uint32_t whd_wifi_deregister_event_handler(whd_interface_t ifp, uint16_t event_index);
/** Delete/Deregister the error entry where callback is registered
 *
 *  @param  ifp                Pointer to handle instance of whd interface
 *  @param  error_index        Error index obtained during registration by whd_wifi_set_error_handler
 *
 *  @return WHD_SUCCESS or Error code
 */

uint32_t whd_wifi_deregister_error_handler(whd_interface_t ifp, uint16_t error_index);

#ifdef __cplusplus
} /* extern "C" */
#endif
#endif /* ifndef  */

