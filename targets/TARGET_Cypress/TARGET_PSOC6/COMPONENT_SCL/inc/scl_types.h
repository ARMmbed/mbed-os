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

/** @file scl_types.h
 *  Defines common data types used in SCL
 *
 */

#include <stdint.h>
#include "cy_result.h"

#ifndef INCLUDED_SCL_TYPES_H_
#define INCLUDED_SCL_TYPES_H_

#ifdef __cplusplus
extern "C"
{
#endif
/******************************************************
*                      Macros
******************************************************/
#define SSID_NAME_SIZE           (32)  /**< SSID Length */

#define WEP_ENABLED            0x0001  /**< Flag to enable WEP Security        */
#define TKIP_ENABLED           0x0002  /**< Flag to enable TKIP Encryption     */
#define AES_ENABLED            0x0004  /**< Flag to enable AES Encryption      */
#define SHARED_ENABLED     0x00008000  /**< Flag to enable Shared key Security */
#define WPA_SECURITY       0x00200000  /**< Flag to enable WPA Security        */
#define WPA2_SECURITY      0x00400000  /**< Flag to enable WPA2 Security       */
#define WPA3_SECURITY      0x01000000  /**< Flag to enable WPA3 PSK Security   */

#define ENTERPRISE_ENABLED 0x02000000  /**< Flag to enable Enterprise Security */
#define WPS_ENABLED        0x10000000  /**< Flag to enable WPS Security        */
#define IBSS_ENABLED       0x20000000  /**< Flag to enable IBSS mode           */
#define FBT_ENABLED        0x40000000  /**< Flag to enable FBT                 */

#define NO_POWERSAVE_MODE           (0) /**< No Power save mode */
#define PM1_POWERSAVE_MODE          (1) /**< Power save mode on specified interface without regard for throughput reduction */
#define PM2_POWERSAVE_MODE          (2) /**< Power save mode on specified interface with High throughput */

/**
 * Suppresses unused parameter warning
 */
#define UNUSED_PARAMETER(x) ( (void)(x) )

/**
 * Suppresses unused variable warning
 */
#define UNUSED_VARIABLE(x) ( (void)(x) )

/**
 * Suppresses unused variable warning that occurs due to an assert being disabled in release mode
 */
#define REFERENCE_DEBUG_ONLY_VARIABLE(x) ( (void)(x) )

/******************************************************
*                    Constants
******************************************************/

/******************************************************
*               Structures and Enumerations
******************************************************/

/**
 * Enumeration of Wi-Fi security modes
 */
typedef enum {
    SCL_SECURITY_OPEN             = 0,                                                                 /**< Open security                                         */
    SCL_SECURITY_WEP_PSK          = WEP_ENABLED,                                                       /**< WEP PSK Security with open authentication             */
    SCL_SECURITY_WEP_SHARED       = (WEP_ENABLED | SHARED_ENABLED),                                    /**< WEP PSK Security with shared authentication           */
    SCL_SECURITY_WPA_TKIP_PSK     = (WPA_SECURITY | TKIP_ENABLED),                                     /**< WPA PSK Security with TKIP                            */
    SCL_SECURITY_WPA_AES_PSK      = (WPA_SECURITY | AES_ENABLED),                                      /**< WPA PSK Security with AES                             */
    SCL_SECURITY_WPA_MIXED_PSK    = (WPA_SECURITY | AES_ENABLED | TKIP_ENABLED),                       /**< WPA PSK Security with AES & TKIP                      */
    SCL_SECURITY_WPA2_AES_PSK     = (WPA2_SECURITY | AES_ENABLED),                                     /**< WPA2 PSK Security with AES                            */
    SCL_SECURITY_WPA2_TKIP_PSK    = (WPA2_SECURITY | TKIP_ENABLED),                                    /**< WPA2 PSK Security with TKIP                           */
    SCL_SECURITY_WPA2_MIXED_PSK   = (WPA2_SECURITY | AES_ENABLED | TKIP_ENABLED),                      /**< WPA2 PSK Security with AES & TKIP                     */
    SCL_SECURITY_WPA2_FBT_PSK     = (WPA2_SECURITY | AES_ENABLED | FBT_ENABLED),                       /**< WPA2 FBT PSK Security with AES & TKIP */
    SCL_SECURITY_WPA3_SAE         = (WPA3_SECURITY | AES_ENABLED),                                     /**< WPA3 Security with AES */
    SCL_SECURITY_WPA3_WPA2_PSK    = (WPA3_SECURITY | WPA2_SECURITY | AES_ENABLED),                     /**< WPA3 WPA2 PSK Security with AES */

    SCL_SECURITY_WPA_TKIP_ENT     = (ENTERPRISE_ENABLED | WPA_SECURITY | TKIP_ENABLED),                /**< WPA Enterprise Security with TKIP                     */
    SCL_SECURITY_WPA_AES_ENT      = (ENTERPRISE_ENABLED | WPA_SECURITY | AES_ENABLED),                 /**< WPA Enterprise Security with AES                      */
    SCL_SECURITY_WPA_MIXED_ENT    = (ENTERPRISE_ENABLED | WPA_SECURITY | AES_ENABLED | TKIP_ENABLED),  /**< WPA Enterprise Security with AES & TKIP               */
    SCL_SECURITY_WPA2_TKIP_ENT    = (ENTERPRISE_ENABLED | WPA2_SECURITY | TKIP_ENABLED),               /**< WPA2 Enterprise Security with TKIP                    */
    SCL_SECURITY_WPA2_AES_ENT     = (ENTERPRISE_ENABLED | WPA2_SECURITY | AES_ENABLED),                /**< WPA2 Enterprise Security with AES                     */
    SCL_SECURITY_WPA2_MIXED_ENT   = (ENTERPRISE_ENABLED | WPA2_SECURITY | AES_ENABLED | TKIP_ENABLED), /**< WPA2 Enterprise Security with AES & TKIP              */
    SCL_SECURITY_WPA2_FBT_ENT     = (ENTERPRISE_ENABLED | WPA2_SECURITY | AES_ENABLED | FBT_ENABLED),  /**< WPA2 Enterprise Security with AES & FBT               */

    SCL_SECURITY_IBSS_OPEN        = (IBSS_ENABLED),                                                    /**< Open security on IBSS ad-hoc network                  */
    SCL_SECURITY_WPS_OPEN         = (WPS_ENABLED),                                                     /**< WPS with open security                                */
    SCL_SECURITY_WPS_SECURE       = (WPS_ENABLED | AES_ENABLED),                                       /**< WPS with AES security                                 */

    SCL_SECURITY_UNKNOWN          = -1,                                                                /**< Returned by scan function if security is unknown. Do not pass this to the join function! */

    SCL_SECURITY_FORCE_32_BIT     = 0x7fffffff                                                         /**< Exists only to force scl_security_t type to 32 bits */
} scl_security_t;

#ifdef __cplusplus
}     /* extern "C" */
#endif
#endif /* ifndef INCLUDED_SCL_TYPES_H_ */
