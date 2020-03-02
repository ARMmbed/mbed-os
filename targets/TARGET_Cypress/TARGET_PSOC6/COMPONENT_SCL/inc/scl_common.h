/*
 * Copyright 2018-2020 Cypress Semiconductor Corporation
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

/** @file scl_common.h
 *  Defines common data types used in SCL
 */

#include <stdint.h>
#include "cy_result.h"

#ifndef INCLUDED_SCL_COMMON_H_
#define INCLUDED_SCL_COMMON_H_

#ifdef __cplusplus
extern "C"
{
#endif

/******************************************************
*                    Constants
******************************************************/

#define SCL_LOG_ENABLE          false /**< Flag to enable SCL debug logs */
#define SCL_LOG(x)              if (SCL_LOG_ENABLE) \
                                    { printf x; } /**< SCL log interface */
#define CHECK_BUFFER_NULL(buf)  if (buf == NULL)\
                                    { SCL_LOG(("Buffer pointer is null\n")); \
                                      return SCL_BADARG; }  /**< Helper macro to check if the input buffer pointer is null */

#define MODULE_BASE_CODE        (0x0080U) /**< Base code for the SCL error status */
#define SCL_RESULT_TYPE         (0)   /**< SCL Result type */

/*
 * scl_result_t Error code format
 * |31-18 (14 bit) for module id|17-16 (2 bit) for result type|15-0 for SCL Error code|
 * for example, for Error code 1026, the result of SCL_RESULT_CREATE is 33555458.
 */
#define SCL_RESULT_CREATE(x) CY_RSLT_CREATE(SCL_RESULT_TYPE, MODULE_BASE_CODE, (x) ) /**< Create a result value from the specified type, module, and result code */

#define SCL_SUCCESS                       (0)                       /**< IPC success */
#define SCL_ERROR                         (100)                     /**< IPC error */
#define SCL_PENDING                       SCL_RESULT_CREATE(1)      /**< IPC Pending */
#define SCL_TIMEOUT                       SCL_RESULT_CREATE(2)      /**< Timeout */
#define SCL_BADARG                        SCL_RESULT_CREATE(5)      /**< Bad Arguments */
#define SCL_UNFINISHED                    SCL_RESULT_CREATE(10)     /**< Operation not finished yet  (maybe aborted) */
#define SCL_PARTIAL_RESULTS               SCL_RESULT_CREATE(1003)   /**< Partial results */
#define SCL_INVALID_KEY                   SCL_RESULT_CREATE(1004)   /**< Invalid key */
#define SCL_DOES_NOT_EXIST                SCL_RESULT_CREATE(1005)   /**< Does not exist */
#define SCL_NOT_AUTHENTICATED             SCL_RESULT_CREATE(1006)   /**< Not authenticated */
#define SCL_NOT_KEYED                     SCL_RESULT_CREATE(1007)   /**< Not keyed */
#define SCL_IOCTL_FAIL                    SCL_RESULT_CREATE(1008)   /**< IOCTL fail */
#define SCL_BUFFER_UNAVAILABLE_TEMPORARY  SCL_RESULT_CREATE(1009)   /**< Buffer unavailable temporarily */
#define SCL_BUFFER_UNAVAILABLE_PERMANENT  SCL_RESULT_CREATE(1010)   /**< Buffer unavailable permanently */
#define SCL_CONNECTION_LOST               SCL_RESULT_CREATE(1012)   /**< Connection lost */
#define SCL_OUT_OF_EVENT_HANDLER_SPACE    SCL_RESULT_CREATE(1013)   /**< Cannot add extra event handler */
#define SCL_SEMAPHORE_ERROR               SCL_RESULT_CREATE(1014)   /**< Error manipulating a semaphore */
#define SCL_FLOW_CONTROLLED               SCL_RESULT_CREATE(1015)   /**< Packet retrieval cancelled due to flow control */
#define SCL_NO_CREDITS                    SCL_RESULT_CREATE(1016)   /**< Packet retrieval cancelled due to lack of bus credits */
#define SCL_NO_PACKET_TO_SEND             SCL_RESULT_CREATE(1017)   /**< Packet retrieval cancelled due to no pending packets */
#define SCL_CORE_CLOCK_NOT_ENABLED        SCL_RESULT_CREATE(1018)   /**< Core disabled due to no clock */
#define SCL_CORE_IN_RESET                 SCL_RESULT_CREATE(1019)   /**< Core disabled - in reset */
#define SCL_UNSUPPORTED                   SCL_RESULT_CREATE(1020)   /**< Unsupported function */
#define SCL_BUS_WRITE_REGISTER_ERROR      SCL_RESULT_CREATE(1021)   /**< Error writing to WLAN register */
#define SCL_SDIO_BUS_UP_FAIL              SCL_RESULT_CREATE(1022)   /**< SDIO bus failed to come up */
#define SCL_JOIN_IN_PROGRESS              SCL_RESULT_CREATE(1023)   /**< Join not finished yet */
#define SCL_NETWORK_NOT_FOUND             SCL_RESULT_CREATE(1024)   /**< Specified network was not found */
#define SCL_INVALID_JOIN_STATUS           SCL_RESULT_CREATE(1025)   /**< Join status error */
#define SCL_UNKNOWN_INTERFACE             SCL_RESULT_CREATE(1026)   /**< Unknown interface specified */
#define SCL_SDIO_RX_FAIL                  SCL_RESULT_CREATE(1027)   /**< Error during SDIO receive */
#define SCL_HWTAG_MISMATCH                SCL_RESULT_CREATE(1028)   /**< Hardware tag header corrupt */
#define SCL_RX_BUFFER_ALLOC_FAIL          SCL_RESULT_CREATE(1029)   /**< Failed to allocate a buffer to receive into */
#define SCL_BUS_READ_REGISTER_ERROR       SCL_RESULT_CREATE(1030)   /**< Error reading a bus hardware register */
#define SCL_THREAD_CREATE_FAILED          SCL_RESULT_CREATE(1031)   /**< Failed to create a new thread */
#define SCL_QUEUE_ERROR                   SCL_RESULT_CREATE(1032)   /**< Error manipulating a queue */
#define SCL_BUFFER_POINTER_MOVE_ERROR     SCL_RESULT_CREATE(1033)   /**< Error moving the current pointer of a packet buffer  */
#define SCL_BUFFER_SIZE_SET_ERROR         SCL_RESULT_CREATE(1034)   /**< Error setting size of packet buffer */
#define SCL_THREAD_STACK_NULL             SCL_RESULT_CREATE(1035)   /**< Null stack pointer passed when non null was required */
#define SCL_THREAD_DELETE_FAIL            SCL_RESULT_CREATE(1036)   /**< Error deleting a thread */
#define SCL_SLEEP_ERROR                   SCL_RESULT_CREATE(1037)   /**< Error sleeping a thread */
#define SCL_BUFFER_ALLOC_FAIL             SCL_RESULT_CREATE(1038)   /**< Failed to allocate a packet buffer */
#define SCL_NO_PACKET_TO_RECEIVE          SCL_RESULT_CREATE(1039)   /**< No Packets waiting to be received */
#define SCL_INTERFACE_NOT_UP              SCL_RESULT_CREATE(1040)   /**< Requested interface is not active */
#define SCL_DELAY_TOO_LONG                SCL_RESULT_CREATE(1041)   /**< Requested delay is too long */
#define SCL_INVALID_DUTY_CYCLE            SCL_RESULT_CREATE(1042)   /**< Duty cycle is outside limit 0 to 100 */
#define SCL_PMK_WRONG_LENGTH              SCL_RESULT_CREATE(1043)   /**< Returned pmk was the wrong length */
#define SCL_UNKNOWN_SECURITY_TYPE         SCL_RESULT_CREATE(1044)   /**< AP security type was unknown */
#define SCL_WEP_NOT_ALLOWED               SCL_RESULT_CREATE(1045)   /**< AP not allowed to use WEP - it is not secure - use Open instead */
#define SCL_WPA_KEYLEN_BAD                SCL_RESULT_CREATE(1046)   /**< WPA / WPA2 key length must be between 8 & 64 bytes */
#define SCL_FILTER_NOT_FOUND              SCL_RESULT_CREATE(1047)   /**< Specified filter id not found */
#define SCL_SPI_ID_READ_FAIL              SCL_RESULT_CREATE(1048)   /**< Failed to read 0xfeedbead SPI id from chip */
#define SCL_SPI_SIZE_MISMATCH             SCL_RESULT_CREATE(1049)   /**< Mismatch in sizes between SPI header and SDPCM header */
#define SCL_ADDRESS_ALREADY_REGISTERED    SCL_RESULT_CREATE(1050)   /**< Attempt to register a multicast address twice */
#define SCL_SDIO_RETRIES_EXCEEDED         SCL_RESULT_CREATE(1051)   /**< SDIO transfer failed too many times. */
#define SCL_NULL_PTR_ARG                  SCL_RESULT_CREATE(1052)   /**< Null Pointer argument passed to function. */
#define SCL_THREAD_FINISH_FAIL            SCL_RESULT_CREATE(1053)   /**< Error deleting a thread */
#define SCL_WAIT_ABORTED                  SCL_RESULT_CREATE(1054)   /**< Semaphore/mutex wait has been aborted */
#define SCL_SET_BLOCK_ACK_WINDOW_FAIL     SCL_RESULT_CREATE(1055)   /**< Failed to set block ack window */
#define SCL_DELAY_TOO_SHORT               SCL_RESULT_CREATE(1056)   /**< Requested delay is too short */
#define SCL_INVALID_INTERFACE             SCL_RESULT_CREATE(1057)   /**< Invalid interface provided */
#define SCL_WEP_KEYLEN_BAD                SCL_RESULT_CREATE(1058)   /**< WEP / WEP_SHARED key length must be 5 or 13 bytes */
#define SCL_HANDLER_ALREADY_REGISTERED    SCL_RESULT_CREATE(1059)   /**< EAPOL handler already registered */
#define SCL_AP_ALREADY_UP                 SCL_RESULT_CREATE(1060)   /**< Soft AP or P2P group owner already up */
#define SCL_EAPOL_KEY_PACKET_M1_TIMEOUT   SCL_RESULT_CREATE(1061)   /**< Timeout occurred while waiting for EAPOL packet M1 from AP */
#define SCL_EAPOL_KEY_PACKET_M3_TIMEOUT   SCL_RESULT_CREATE(1062)   /**< Timeout occurred while waiting for EAPOL packet M3 from AP which may indicate incorrect WPA2/WPA passphrase */
#define SCL_EAPOL_KEY_PACKET_G1_TIMEOUT   SCL_RESULT_CREATE(1063)   /**< Timeout occurred while waiting for EAPOL packet G1 from AP */
#define SCL_EAPOL_KEY_FAILURE             SCL_RESULT_CREATE(1064)   /**< Unknown failure occurred during the EAPOL key handshake */
#define SCL_MALLOC_FAILURE                SCL_RESULT_CREATE(1065)   /**< Memory allocation failure */
#define SCL_ACCESS_POINT_NOT_FOUND        SCL_RESULT_CREATE(1066)   /**< Access point not found */
#define SCL_RTOS_ERROR                    SCL_RESULT_CREATE(1067)   /**< RTOS operation failed */
#define SCL_CLM_BLOB_DLOAD_ERROR          SCL_RESULT_CREATE(1068)   /**< CLM blob download failed */
#define SCL_HAL_ERROR                     SCL_RESULT_CREATE(1069)   /**< SCL HAL Error */
#define SCL_RTOS_STATIC_MEM_LIMIT         SCL_RESULT_CREATE(1070)   /**< Exceeding the RTOS static objects memory */

/* Application uses the following constants to allocate the buffer pool: */

#define BDC_HEADER_WITH_PAD 6  /**< BDC Header with padding 4 + 2 */

#define SCL_PAYLOAD_MTU           (1500) /**< The maximum size, in bytes, of the data part of an Ethernet frame */

/******************************************************
*                   Type Definitions
******************************************************/
/**
 * Typedef for SCL buffer pointer
 */
typedef void *scl_buffer_t;

/**
 * Typedef for SCL result
 */
typedef uint32_t scl_result_t;

/******************************************************
*               Structures and Enumerations
******************************************************/

/**
 * Typedef for SCL boolean flags
 */
typedef enum {
    SCL_FALSE = 0, /**< Boolean False */
    SCL_TRUE  = 1 /**< Boolean True */
} scl_bool_t;

/**
 * Typedef for SCL interface roles
 */
typedef enum {
    SCL_INVALID_ROLE           = 0,         /**< Invalid role */
    SCL_STA_ROLE               = 1,         /**< STA or Client Interface */
    SCL_AP_ROLE                = 2,         /**< softAP Interface */
    SCL_P2P_ROLE               = 3          /**< P2P Interface */
} scl_interface_role_t;

/**
 * Typedef for SCL IPC receive index
 */
typedef enum {
    SCL_RX_DATA                  = 0,      /**< Received buffer */
    SCL_RX_TEST_MSG              = 1,      /**< Test message */
    SCL_RX_GET_BUFFER            = 2,      /**< Get the buffer */
    SCL_RX_GET_CONNECTION_STATUS = 3       /**< Get the connection status */
} scl_ipc_rx_t;

/**
 * Typedef for SCL IPC transmit index
 */
typedef enum {
    SCL_TX_TEST_MSG                    = 1,  /**< Test Message */
    SCL_TX_WIFI_INIT                   = 2,  /**< Initialize Wi-Fi */
    SCL_TX_CONFIG_PARAMETERS           = 3,  /**< Configuration parameters */
    SCL_TX_GET_MAC                     = 4,  /**< Get MAC address */
    SCL_TX_REGISTER_MULTICAST_ADDRESS  = 5,  /**< Register multicast address */
    SCL_TX_SEND_OUT                    = 6,  /**< Transmit buffer */
    SCL_TX_TRANSCEIVE_READY            = 7,  /**< Wi-Fi transmit/receive ready */
    SCL_TX_WIFI_ON                     = 8,  /**< Wi-Fi on */
    SCL_TX_WIFI_SET_UP                 = 9,  /**< Wi-Fi setup */
    SCL_TX_WIFI_NW_PARAM               = 10, /**< Get network parameters */
    SCL_TX_WIFI_GET_RSSI               = 11, /**< Get RSSI */
    SCL_TX_WIFI_GET_BSSID              = 12, /**< Get BSSID */
    SCL_TX_CONNECT                     = 13, /**< Wi-Fi connect */
    SCL_TX_DISCONNECT                  = 14, /**< Wi-Fi disconnect */
    SCL_TX_CONNECTION_STATUS           = 15  /**< Transmit connection status */
} scl_ipc_tx_t;


/**
 * Structure for storing a MAC address (Wi-Fi Media Access Control address).
 */
typedef struct {
    uint8_t octet[6]; /**< Unique 6-byte MAC address */
} scl_mac_t;

#ifdef __cplusplus
}     /* extern "C" */
#endif
#endif /* ifndef INCLUDED_SCL_COMMON_H_ */
