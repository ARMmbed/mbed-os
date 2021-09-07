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

/** @file
 *  Defines common constants used with WHD within src folder
 *
 */

#ifndef INCLUDED_WHD_TYPES_INT_H_
#define INCLUDED_WHD_TYPES_INT_H_

#include <stdint.h>
#include <stddef.h>
#include <inttypes.h>

#ifdef __cplusplus
extern "C"
{
#endif

/******************************************************
*                      Macros
******************************************************/
#define MAX_BUS_HEADER_SIZE 4

/**
 * The size of an Ethernet header
 */
#define WHD_ETHERNET_SIZE         (14)

/**
 * Ethernet Ethertypes
 */
#define WHD_ETHERTYPE_IPv4    0x0800
#define WHD_ETHERTYPE_IPv6    0x86DD
#define WHD_ETHERTYPE_ARP     0x0806
#define WHD_ETHERTYPE_RARP    0x8035
#define WHD_ETHERTYPE_EAPOL   0x888E
#define WHD_ETHERTYPE_DOT1AS  0x88F7
#define WHD_ETHERTYPE_8021Q   0x8100

/* bss_info_cap_t flags */
#define WL_BSS_FLAGS_FROM_BEACON        0x01    /* bss_info derived from beacon */
#define WL_BSS_FLAGS_FROM_CACHE         0x02    /* bss_info collected from cache */
#define WL_BSS_FLAGS_RSSI_ONCHANNEL     0x04    /* rssi info received on channel (vs offchannel) */
#define WL_BSS_FLAGS_HS20               0x08    /* hotspot 2.0 capable */
#define WL_BSS_FLAGS_RSSI_INVALID       0x10    /* BSS contains invalid RSSI */
#define WL_BSS_FLAGS_RSSI_INACCURATE    0x20    /* BSS contains inaccurate RSSI */
#define WL_BSS_FLAGS_SNR_INVALID        0x40    /* BSS contains invalid SNR */
#define WL_BSS_FLAGS_NF_INVALID         0x80    /* BSS contains invalid noise floor */

#define HT_CAPABILITIES_IE_LENGTH       (26)
#define DOT11_OUI_LEN                   (3)/** Length in bytes of 802.11 OUI*/

#define WHD_ETHER_ADDR_STR_LEN          (18)
#define WHD_ETHER_ADDR_LEN              (6)

#define CHECK_IOCTL_BUFFER(buff)  if (buff == \
                                      NULL){ WPRINT_WHD_ERROR( ("Buffer alloc failed in function %s at line %d \n", \
                                                                __func__, __LINE__) ); \
                                             return WHD_BUFFER_ALLOC_FAIL; }
#define CHECK_RETURN(expr)  { \
        whd_result_t check_res = (expr); \
        if (check_res != WHD_SUCCESS) \
        { \
            WPRINT_WHD_ERROR( ("Function %s failed at line %d checkres = %u \n", \
                               __func__, __LINE__, \
                               (unsigned int)check_res) ); \
            return check_res; \
        } \
}

#define CHECK_RETURN_UNSUPPORTED_OK(expr)  { whd_result_t check_res = (expr);  \
                                             if (check_res != WHD_SUCCESS) \
                                             { \
                                                 return check_res; \
                                             } \
}
#define CHECK_RETURN_UNSUPPORTED_CONTINUE(expr)  { whd_result_t check_res = (expr);                                 \
                                                   if (check_res != WHD_SUCCESS && check_res != WHD_WLAN_UNSUPPORTED) \
                                                   { \
                                                       return check_res; \
                                                   } \
}
#define RETURN_WITH_ASSERT(expr)  { whd_result_t check_res = (expr); \
                                    whd_assert("Command failed\n", check_res == WHD_SUCCESS); \
                                    return check_res; }

#define CHECK_RETURN_IGNORE(expr)  { whd_result_t check_res = (expr);  \
                                     if (check_res != WHD_SUCCESS) \
                                     { \
                                     } \
}

#define CHECK_IFP_NULL(ifp)  if (ifp == \
                                 NULL){ WPRINT_WHD_ERROR( ( \
                                                              "Interface is not up/NULL and failed in function %s at line %d \n", \
                                                              __func__, __LINE__) ); \
                                        return WHD_UNKNOWN_INTERFACE; }

#define CHECK_DRIVER_NULL(driver)  if (driver == \
                                       NULL){ WPRINT_WHD_ERROR( ( \
                                                                    "WHD driver is not up/NULL and failed in function %s at line %d \n", \
                                                                    __func__, __LINE__) ); \
                                              return WHD_DOES_NOT_EXIST; }

#define MIN_OF(x, y) ( (x) < (y) ? (x) : (y) )
#define MAX_OF(x, y)  ( (x) > (y) ? (x) : (y) )

#ifndef ROUND_UP
#define ROUND_UP(x, y)    ( (x) % (y) ? (x) + (y) - ( (x) % (y) ) : (x) )
#endif

#ifndef DIV_ROUND_UP
#define DIV_ROUND_UP(m, n)    ( ( (m) + (n) - 1 ) / (n) )
#endif

#define WHD_WRITE_16(pointer, value)      (*( (uint16_t *)pointer ) = value)
#define WHD_WRITE_32(pointer, value)      (*( (uint32_t *)pointer ) = value)
#define WHD_READ_16(pointer)              *( (uint16_t *)pointer )
#define WHD_READ_32(pointer)              *( (uint32_t *)pointer )

/**
 *  Macro for checking for NULL MAC addresses
 */
#define NULL_MAC(a)  ( ( ( ( (unsigned char *)a )[0] ) == 0 ) && \
                       ( ( ( (unsigned char *)a )[1] ) == 0 ) && \
                       ( ( ( (unsigned char *)a )[2] ) == 0 ) && \
                       ( ( ( (unsigned char *)a )[3] ) == 0 ) && \
                       ( ( ( (unsigned char *)a )[4] ) == 0 ) && \
                       ( ( ( (unsigned char *)a )[5] ) == 0 ) )

/**
 *	Macro for checking for Broadcast address
 */
#define BROADCAST_ID(a)  ( ( ( ( (unsigned char *)a )[0] ) == 255 ) && \
                           ( ( ( (unsigned char *)a )[1] ) == 255 ) && \
                           ( ( ( (unsigned char *)a )[2] ) == 255 ) && \
                           ( ( ( (unsigned char *)a )[3] ) == 255 ) && \
                           ( ( ( (unsigned char *)a )[4] ) == 255 ) && \
                           ( ( ( (unsigned char *)a )[5] ) == 255 ) )

/* Suppress unused variable warning occurring due to an assert which is disabled in release mode */
#define REFERENCE_DEBUG_ONLY_VARIABLE(x) ( (void)(x) )

/* Suppress unused parameter warning */
#define UNUSED_PARAMETER(x) ( (void)(x) )

/* Suppress unused variable warning */
#define UNUSED_VARIABLE(x) ( (void)(x) )

#if defined (__IAR_SYSTEMS_ICC__)
#define DISABLE_COMPILER_WARNING(x) _Pragma(#x)
#define ENABLE_COMPILER_WARNING(x) _Pragma(#x)
#else
#define DISABLE_COMPILER_WARNING(x)
#define ENABLE_COMPILER_WARNING(x)
#endif

/******************************************************
*                 Type Definitions
******************************************************/

/******************************************************
*                    Constants
******************************************************/

/******************************************************
*             Structures and Enumerations
******************************************************/
#pragma pack(1)
typedef struct
{
    whd_buffer_queue_ptr_t queue_next;
    char bus_header[MAX_BUS_HEADER_SIZE];
} whd_buffer_header_t;
#pragma pack()

/* 802.11 Information Element Identification Numbers (as per section 8.4.2.1 of 802.11-2012) */
typedef enum
{
    DOT11_IE_ID_SSID                                 = 0,
    DOT11_IE_ID_SUPPORTED_RATES                      = 1,
    DOT11_IE_ID_FH_PARAMETER_SET                     = 2,
    DOT11_IE_ID_DSSS_PARAMETER_SET                   = 3,
    DOT11_IE_ID_CF_PARAMETER_SET                     = 4,
    DOT11_IE_ID_TIM                                  = 5,
    DOT11_IE_ID_IBSS_PARAMETER_SET                   = 6,
    DOT11_IE_ID_COUNTRY                              = 7,
    DOT11_IE_ID_HOPPING_PATTERN_PARAMETERS           = 8,
    DOT11_IE_ID_HOPPING_PATTERN_TABLE                = 9,
    DOT11_IE_ID_REQUEST                              = 10,
    DOT11_IE_ID_BSS_LOAD                             = 11,
    DOT11_IE_ID_EDCA_PARAMETER_SET                   = 12,
    DOT11_IE_ID_TSPEC                                = 13,
    DOT11_IE_ID_TCLAS                                = 14,
    DOT11_IE_ID_SCHEDULE                             = 15,
    DOT11_IE_ID_CHALLENGE_TEXT                       = 16,
    /* 17-31 Reserved */
    DOT11_IE_ID_POWER_CONSTRAINT                     = 32,
    DOT11_IE_ID_POWER_CAPABILITY                     = 33,
    DOT11_IE_ID_TPC_REQUEST                          = 34,
    DOT11_IE_ID_TPC_REPORT                           = 35,
    DOT11_IE_ID_SUPPORTED_CHANNELS                   = 36,
    DOT11_IE_ID_CHANNEL_SWITCH_ANNOUNCEMENT          = 37,
    DOT11_IE_ID_MEASUREMENT_REQUEST                  = 38,
    DOT11_IE_ID_MEASUREMENT_REPORT                   = 39,
    DOT11_IE_ID_QUIET                                = 40,
    DOT11_IE_ID_IBSS_DFS                             = 41,
    DOT11_IE_ID_ERP                                  = 42,
    DOT11_IE_ID_TS_DELAY                             = 43,
    DOT11_IE_ID_TCLAS_PROCESSING                     = 44,
    DOT11_IE_ID_HT_CAPABILITIES                      = 45,
    DOT11_IE_ID_QOS_CAPABILITY                       = 46,
    /* 47 Reserved */
    DOT11_IE_ID_RSN                                  = 48,
    /* 49 Reserved */
    DOT11_IE_ID_EXTENDED_SUPPORTED_RATES             = 50,
    DOT11_IE_ID_AP_CHANNEL_REPORT                    = 51,
    DOT11_IE_ID_NEIGHBOR_REPORT                      = 52,
    DOT11_IE_ID_RCPI                                 = 53,
    DOT11_IE_ID_MOBILITY_DOMAIN                      = 54,
    DOT11_IE_ID_FAST_BSS_TRANSITION                  = 55,
    DOT11_IE_ID_TIMEOUT_INTERVAL                     = 56,
    DOT11_IE_ID_RIC_DATA                             = 57,
    DOT11_IE_ID_DSE_REGISTERED_LOCATION              = 58,
    DOT11_IE_ID_SUPPORTED_OPERATING_CLASSES          = 59,
    DOT11_IE_ID_EXTENDED_CHANNEL_SWITCH_ANNOUNCEMENT = 60,
    DOT11_IE_ID_HT_OPERATION                         = 61,
    DOT11_IE_ID_SECONDARY_CHANNEL_OFFSET             = 62,
    DOT11_IE_ID_BSS_AVERAGE_ACCESS_DELAY             = 63,
    DOT11_IE_ID_ANTENNA                              = 64,
    DOT11_IE_ID_RSNI                                 = 65,
    DOT11_IE_ID_MEASUREMENT_PILOT_TRANSMISSION       = 66,
    DOT11_IE_ID_BSS_AVAILABLE_ADMISSION_CAPACITY     = 67,
    DOT11_IE_ID_BSS_AC_ACCESS_DELAY                  = 68,
    DOT11_IE_ID_TIME_ADVERTISEMENT                   = 69,
    DOT11_IE_ID_RM_ENABLED_CAPABILITIES              = 70,
    DOT11_IE_ID_MULTIPLE_BSSID                       = 71,
    DOT11_IE_ID_20_40_BSS_COEXISTENCE                = 72,
    DOT11_IE_ID_20_40_BSS_INTOLERANT_CHANNEL_REPORT  = 73,
    DOT11_IE_ID_OVERLAPPING_BSS_SCAN_PARAMETERS      = 74,
    DOT11_IE_ID_RIC_DESCRIPTOR                       = 75,
    DOT11_IE_ID_MANAGEMENT_MIC                       = 76,
    DOT11_IE_ID_EVENT_REQUEST                        = 78,
    DOT11_IE_ID_EVENT_REPORT                         = 79,
    DOT11_IE_ID_DIAGNOSTIC_REQUEST                   = 80,
    DOT11_IE_ID_DIAGNOSTIC_REPORT                    = 81,
    DOT11_IE_ID_LOCATION_PARAMETERS                  = 82,
    DOT11_IE_ID_NONTRANSMITTED_BSSID_CAPABILITY      = 83,
    DOT11_IE_ID_SSID_LIST                            = 84,
    DOT11_IE_ID_MULTIPLE_BSSID_INDEX                 = 85,
    DOT11_IE_ID_FMS_DESCRIPTOR                       = 86,
    DOT11_IE_ID_FMS_REQUEST                          = 87,
    DOT11_IE_ID_FMS_RESPONSE                         = 88,
    DOT11_IE_ID_QOS_TRAFFIC_CAPABILITY               = 89,
    DOT11_IE_ID_BSS_MAX_IDLE_PERIOD                  = 90,
    DOT11_IE_ID_TFS_REQUEST                          = 91,
    DOT11_IE_ID_TFS_RESPONSE                         = 92,
    DOT11_IE_ID_WNM_SLEEP_MODE                       = 93,
    DOT11_IE_ID_TIM_BROADCAST_REQUEST                = 94,
    DOT11_IE_ID_TIM_BROADCAST_RESPONSE               = 95,
    DOT11_IE_ID_COLLOCATED_INTERFERENCE_REPORT       = 96,
    DOT11_IE_ID_CHANNEL_USAGE                        = 97,
    DOT11_IE_ID_TIME_ZONE                            = 98,
    DOT11_IE_ID_DMS_REQUEST                          = 99,
    DOT11_IE_ID_DMS_RESPONSE                         = 100,
    DOT11_IE_ID_LINK_IDENTIFIER                      = 101,
    DOT11_IE_ID_WAKEUP_SCHEDULE                      = 102,
    /* 103 Reserved */
    DOT11_IE_ID_CHANNEL_SWITCH_TIMING                = 104,
    DOT11_IE_ID_PTI_CONTROL                          = 105,
    DOT11_IE_ID_TPU_BUFFER_STATUS                    = 106,
    DOT11_IE_ID_INTERWORKING                         = 107,
    DOT11_IE_ID_ADVERTISMENT_PROTOCOL                = 108,
    DOT11_IE_ID_EXPEDITED_BANDWIDTH_REQUEST          = 109,
    DOT11_IE_ID_QOS_MAP_SET                          = 110,
    DOT11_IE_ID_ROAMING_CONSORTIUM                   = 111,
    DOT11_IE_ID_EMERGENCY_ALERT_IDENTIFIER           = 112,
    DOT11_IE_ID_MESH_CONFIGURATION                   = 113,
    DOT11_IE_ID_MESH_ID                              = 114,
    DOT11_IE_ID_MESH_LINK_METRIC_REPORT              = 115,
    DOT11_IE_ID_CONGESTION_NOTIFICATION              = 116,
    DOT11_IE_ID_MESH_PEERING_MANAGEMENT              = 117,
    DOT11_IE_ID_MESH_CHANNEL_SWITCH_PARAMETERS       = 118,
    DOT11_IE_ID_MESH_AWAKE_WINDOW                    = 119,
    DOT11_IE_ID_BEACON_TIMING                        = 120,
    DOT11_IE_ID_MCCAOP_SETUP_REQUEST                 = 121,
    DOT11_IE_ID_MCCAOP_SETUP_REPLY                   = 122,
    DOT11_IE_ID_MCCAOP_ADVERTISMENT                  = 123,
    DOT11_IE_ID_MCCAOP_TEARDOWN                      = 124,
    DOT11_IE_ID_GANN                                 = 125,
    DOT11_IE_ID_RANN                                 = 126,
    DOT11_IE_ID_EXTENDED_CAPABILITIES                = 127,
    /* 128-129 Reserved */
    DOT11_IE_ID_PREQ                                 = 130,
    DOT11_IE_ID_PREP                                 = 131,
    DOT11_IE_ID_PERR                                 = 132,
    /* 133-136 Reserved */
    DOT11_IE_ID_PXU                                  = 137,
    DOT11_IE_ID_PXUC                                 = 138,
    DOT11_IE_ID_AUTHENTICATED_MESH_PEERING_EXCHANGE  = 139,
    DOT11_IE_ID_MIC                                  = 140,
    DOT11_IE_ID_DESTINATION_URI                      = 141,
    DOT11_IE_ID_U_APSD_COEXISTENCE                   = 142,
    /* 143-173 Reserved */
    DOT11_IE_ID_MCCAOP_ADVERTISMENT_OVERVIEW         = 174,
    /* 175-220 Reserved */
    DOT11_IE_ID_VENDOR_SPECIFIC                      = 221,
    /* 222-255 Reserved */
} dot11_ie_id_t;

uint32_t whd_wifi_get_iovar_value(whd_interface_t ifp, const char *iovar, uint32_t *value);
uint32_t whd_wifi_set_iovar_buffers(whd_interface_t ifp, const char *iovar, const void **in_buffers,
                                    const uint16_t *lengths, const uint8_t num_buffers);
uint32_t whd_wifi_set_iovar_value(whd_interface_t ifp, const char *iovar, uint32_t value);

/** Sends an IOVAR command
 *
 *  @param  ifp                 : Pointer to handle instance of whd interface
 *  @param  iovar               : IOVAR name
 *
 *  @return WHD_SUCCESS or Error code
 */
extern uint32_t whd_wifi_set_iovar_void(whd_interface_t ifp, const char *iovar);

/** Sends an IOVAR command
 *
 *  @param  ifp                 : Pointer to handle instance of whd interface
 *  @param  iovar               : IOVAR name
 *  @param  buffer              : Handle for a packet buffer containing the data value to be sent.
 *  @param  buffer_length       : Length of out_buffer
 *
 *  @return WHD_SUCCESS or Error code
 */
extern uint32_t whd_wifi_set_iovar_buffer(whd_interface_t ifp, const char *iovar, void *buffer, uint16_t buffer_length);

/** Sends an IOVAR command
 *
 *  @param  ifp                 : Pointer to handle instance of whd interface
 *  @param  iovar               : IOVAR name
 *  @param  in_buffers          : Handle for a packet buffers containing the data value to be sent.
 *  @param  in_buffer_lengths   : Length of in_buffers
 *  @param  num_buffers         : Number of handle buffers
 *
 *  @return WHD_SUCCESS or Error code
 */
extern uint32_t whd_wifi_set_iovar_buffers(whd_interface_t ifp, const char *iovar, const void **in_buffers,
                                           const uint16_t *in_buffer_lengths, const uint8_t num_buffers);

/** Sends an IOVAR command
 *
 *  @param  ifp                 : Pointer to handle instance of whd interface
 *  @param  iovar               : IOVAR name
 *  @param  out_buffer          : Pointer to receive the handle for the packet buffer containing the response data value received
 *  @param  out_length          : Length of out_buffer
 *
 *  @return WHD_SUCCESS or Error code
 */
extern uint32_t whd_wifi_get_iovar_buffer(whd_interface_t ifp, const char *iovar_name, uint8_t *out_buffer,
                                          uint16_t out_length);

/** Sends an IOVAR command
 *
 *  @param  ifp                 : Pointer to handle instance of whd interface
 *  @param  iovar               : IOVAR name
 *  @param  buffer              : Handle for a packet buffer containing the data value to be sent.
 *  @param  buffer_length       : Length of out_buffer
 *
 *  @return WHD_SUCCESS or Error code
 */
extern uint32_t whd_wifi_set_iovar_buffer(whd_interface_t ifp, const char *iovar, void *buffer, uint16_t buffer_length);

/** Sends an IOVAR command
 *
 *  @param  ifp                 : Pointer to handle instance of whd interface
 *  @param  iovar               : IOVAR name
 *  @param  in_buffers          : Handle for a packet buffers containing the data value to be sent.
 *  @param  in_buffer_lengths   : Length of in_buffers
 *  @param  num_buffers         : Number of handle buffers
 *
 *  @return WHD_SUCCESS or Error code
 */
extern uint32_t whd_wifi_set_iovar_buffers(whd_interface_t ifp, const char *iovar, const void **in_buffers,
                                           const uint16_t *in_buffer_lengths, const uint8_t num_buffers);

extern uint32_t whd_wifi_set_mac_address(whd_interface_t ifp, whd_mac_t mac);

#ifdef __cplusplus
}     /* extern "C" */
#endif
#endif /* ifndef INCLUDED_WHD_TYPES_INT_H_ */

