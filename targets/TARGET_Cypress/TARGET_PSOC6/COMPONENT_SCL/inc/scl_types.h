/*
 * (c) (2019-2020), Cypress Semiconductor Corporation. All rights reserved.
 *
 * This software, including source code, documentation and related materials
 * ("Software"), is owned by Cypress Semiconductor Corporation or one of its
 * subsidiaries ("Cypress") and is protected by and subject to worldwide patent
 * protection (United States and foreign), United States copyright laws and
 * international treaty provisions. Therefore, you may use this Software only
 * as provided in the license agreement accompanying the software package from
 * which you obtained this Software ("EULA").
 *
 * If no EULA applies, Cypress hereby grants you a personal, non-exclusive,
 * non-transferable license to copy, modify, and compile the Software source
 * code solely for use in connection with Cypress's integrated circuit products.
 * Any reproduction, modification, translation, compilation, or representation
 * of this Software except as specified above is prohibited without the express
 * written permission of Cypress.
 *
 * Disclaimer: THIS SOFTWARE IS PROVIDED AS-IS, WITH NO WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING, BUT NOT LIMITED TO, NONINFRINGEMENT, IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE. Cypress
 * reserves the right to make changes to the Software without notice. Cypress
 * does not assume any liability arising out of the application or use of the
 * Software or any product or circuit described in the Software. Cypress does
 * not authorize its products for use in any products where a malfunction or
 * failure of the Cypress product may reasonably be expected to result in
 * significant property damage, injury or death ("High Risk Product"). By
 * including Cypress's product in a High Risk Product, the manufacturer of such
 * system or application assumes all risk of such use and in doing so agrees to
 * indemnify Cypress against all liability.
 */

/** @file scl_types.h
 *  Defines common data types used with SCL
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

#define NO_POWERSAVE_MODE           (0) /**< No Powersave mode */
#define PM1_POWERSAVE_MODE          (1) /**< Powersave mode on specified interface without regard for throughput reduction */
#define PM2_POWERSAVE_MODE          (2) /**< Powersave mode on specified interface with High throughput */

/**
 * Suppress unused parameter warning
 */
#define UNUSED_PARAMETER(x) ( (void)(x) )

/**
 * Suppress unused variable warning
 */
#define UNUSED_VARIABLE(x) ( (void)(x) )

/**
 * Suppress unused variable warning occurring due to an assert which is disabled in release mode
 */
#define REFERENCE_DEBUG_ONLY_VARIABLE(x) ( (void)(x) )

/******************************************************
*                    Constants
******************************************************/

/******************************************************
*               Structures and  Enumerations
******************************************************/

/**
 * Enumeration of Wi-Fi security modes
 */
typedef enum
{
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
    
    SCL_SECURITY_UNKNOWN          = -1,                                                                /**< May be returned by scan function if security is unknown. Do not pass this to the join function! */
    
    SCL_SECURITY_FORCE_32_BIT     = 0x7fffffff                                                         /**< Exists only to force scl_security_t type to 32 bits */
} scl_security_t;

#ifdef __cplusplus
}     /* extern "C" */
#endif
#endif /* ifndef INCLUDED_SCL_TYPES_H_ */

