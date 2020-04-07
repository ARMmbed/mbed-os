/***************************************************************************//**
* \file cyhal_pin_package.h
*
* Description:
* Provides definitions for the pinout for each supported device.
*
********************************************************************************
* \copyright
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
*******************************************************************************/

/**
* \addtogroup group_hal_psoc6_pin_package Pins
* \ingroup group_hal_psoc6
* \{
* Definitions for the pinout for each supported device
*/

#pragma once

#include "cy_gpio.h"

#if defined(__cplusplus)
extern "C" {
#endif /* __cplusplus */

/** Port definitions that individual pins can belong to. */
typedef enum {
    CYHAL_PORT_0  = 0x00,
    CYHAL_PORT_1  = 0x01,
    CYHAL_PORT_2  = 0x02,
    CYHAL_PORT_3  = 0x03,
    CYHAL_PORT_4  = 0x04,
    CYHAL_PORT_5  = 0x05,
    CYHAL_PORT_6  = 0x06,
    CYHAL_PORT_7  = 0x07,
    CYHAL_PORT_8  = 0x08,
    CYHAL_PORT_9  = 0x09,
    CYHAL_PORT_10 = 0x0A,
    CYHAL_PORT_11 = 0x0B,
    CYHAL_PORT_12 = 0x0C,
    CYHAL_PORT_13 = 0x0D,
    CYHAL_PORT_14 = 0x0E,
    CYHAL_PORT_15 = 0x0F,
    CYHAL_PORT_16 = 0x10,
    CYHAL_PORT_17 = 0x11,
    CYHAL_PORT_18 = 0x12,
    CYHAL_PORT_19 = 0x13,
    CYHAL_PORT_20 = 0x14,
} cyhal_port_t;

/** Bitfield representing the configuration of a GPIO (hsiom selection and mode).
  * Use the CY_GPIO_CFG_GET_MODE and CY_GPIO_CFG_GET_HSIOM to extract the
  * individual field values.
  */
typedef uint16_t cyhal_gpio_mapping_cfg_t; // 8bit hsiom, 8bit mode

/** Extract the GPIO mode setting from a cyhal_gpio_mapping_cfg_t */
#define CY_GPIO_CFG_GET_MODE(x)  ((uint8_t)((x) & 0xFF))
/** Extract the HSIOM selection from a cyhal_gpio_mapping_cfg_t */
#define CY_GPIO_CFG_GET_HSIOM(x) ((en_hsiom_sel_t)(((x) >> 8) & 0xFF))

/** \cond INTERNAL */
#define CY_GPIO_CFG_CREATE(hsiom, mode)  ((cyhal_gpio_mapping_cfg_t)(((hsiom) << 8) + (mode)))

#define CYHAL_PIN_OUT_FUNCTION(hsiom)           CY_GPIO_CFG_CREATE(hsiom, CY_GPIO_DM_STRONG_IN_OFF)
#define CYHAL_PIN_OUT_BUF_FUNCTION(hsiom)       CY_GPIO_CFG_CREATE(hsiom, CY_GPIO_DM_STRONG)
#define CYHAL_PIN_OD_FUNCTION(hsiom)            CY_GPIO_CFG_CREATE(hsiom, CY_GPIO_DM_OD_DRIVESLOW_IN_OFF)
#define CYHAL_PIN_IN_FUNCTION(hsiom)            CY_GPIO_CFG_CREATE(hsiom, CY_GPIO_DM_HIGHZ)
#define CYHAL_PIN_PULLUP_FUNCTION(hsiom)        CY_GPIO_CFG_CREATE(hsiom, CY_GPIO_DM_PULLUP)
#define CYHAL_PIN_ANALOG_FUNCTION(hsiom)        CY_GPIO_CFG_CREATE(HSIOM_SEL_GPIO, CY_GPIO_DM_ANALOG)
#define CYHAL_PIN_AUX_FUNCTION(hsiom)           CY_GPIO_CFG_CREATE(HSIOM_SEL_GPIO, CY_GPIO_DM_ANALOG)

/** \endcond */

#include "cy_device.h"

#if defined(_GPIO_PSOC6_01_104_M_CSP_BLE_H_)
#include "pin_packages/cyhal_psoc6_01_104_m_csp_ble.h"
#elif defined(_GPIO_PSOC6_01_104_M_CSP_BLE_USB_H_)
#include "pin_packages/cyhal_psoc6_01_104_m_csp_ble_usb.h"
#elif defined(_GPIO_PSOC6_01_116_BGA_BLE_H_)
#include "pin_packages/cyhal_psoc6_01_116_bga_ble.h"
#elif defined(_GPIO_PSOC6_01_116_BGA_USB_H_)
#include "pin_packages/cyhal_psoc6_01_116_bga_usb.h"
#elif defined(_GPIO_PSOC6_01_124_BGA_H_)
#include "pin_packages/cyhal_psoc6_01_124_bga.h"
#elif defined(_GPIO_PSOC6_01_124_BGA_SIP_H_)
#include "pin_packages/cyhal_psoc6_01_124_bga_sip.h"
#elif defined(_GPIO_PSOC6_01_43_SMT_H_)
#include "pin_packages/cyhal_psoc6_01_43_smt.h"
#elif defined(_GPIO_PSOC6_01_68_QFN_BLE_H_)
#include "pin_packages/cyhal_psoc6_01_68_qfn_ble.h"
#elif defined(_GPIO_PSOC6_01_80_WLCSP_H_)
#include "pin_packages/cyhal_psoc6_01_80_wlcsp.h"
#elif defined(_GPIO_PSOC6_02_100_WLCSP_H_)
#include "pin_packages/cyhal_psoc6_02_100_wlcsp.h"
#elif defined(_GPIO_PSOC6_02_124_BGA_H_)
#include "pin_packages/cyhal_psoc6_02_124_bga.h"
#elif defined(_GPIO_PSOC6_02_128_TQFP_H_)
#include "pin_packages/cyhal_psoc6_02_128_tqfp.h"
#elif defined(_GPIO_PSOC6_02_68_QFN_H_)
#include "pin_packages/cyhal_psoc6_02_68_qfn.h"
#elif defined(_GPIO_PSOC6_03_100_TQFP_H_)
#include "pin_packages/cyhal_psoc6_03_100_tqfp.h"
#elif defined(_GPIO_PSOC6_03_49_WLCSP_H_)
#include "pin_packages/cyhal_psoc6_03_49_wlcsp.h"
#elif defined(_GPIO_PSOC6_03_68_QFN_H_)
#include "pin_packages/cyhal_psoc6_03_68_qfn.h"
#elif defined(_GPIO_PLAYER_128_TQFP_H_)
#include "pin_packages/cyhal_mxs28playermcuss_128_tqfp.h"
#else
#error "Unhandled Device/PinPackage combination"
#endif

#if defined(__cplusplus)
}
#endif /* __cplusplus */

/** \} group_hal_psoc6 */
