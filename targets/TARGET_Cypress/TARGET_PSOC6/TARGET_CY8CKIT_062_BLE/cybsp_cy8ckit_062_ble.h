/***************************************************************************//**
* \file cybsp_cy8ckit_062_ble.h
*
* Description:
* Provides APIs for interacting with the hardware contained on the Cypress
* CY8CKIT-062-BLE pioneer kit.
*
********************************************************************************
* \copyright
* Copyright 2018-2019 Cypress Semiconductor Corporation
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
* \addtogroup group_bsp_cy8ckit_062_ble CY8CKIT-062-BLE
* \ingroup group_bsp
* \{
* The PSoC 6 BLE Pioneer Kit is a low-cost hardware platform
* that enables design and debug of the PSoC 63 MCU (CY8C6347BZI-BLD53).
*
* <div class="category">Kit Features:</div>
* <ul>
* <li>BLE v5.0</li>
* <li>Serial memory interface</li>
* <li>PDM-PCM digital microphone interface</li>
* <li>Industry-leading CapSense</li>
* </ul>
*
* <div class="category">Kit Contents:</div>
* <ul>
* <li>CY8CKIT-062-BLE evaluation board</li>
* <li>E-Ink display shield with an ultra-low-power 2.7" E-ink display, thermistor, 6-axis motion sensor, and digital microphone</li>
* <li>USB cable</li>
* </ul>
*
* \defgroup group_bsp_cy8ckit_062_ble_macros Macros
* \defgroup group_bsp_cy8ckit_062_ble_functions Functions
* \defgroup group_bsp_cy8ckit_062_ble_enums Enumerated Types
*/

#pragma once

#include "cybsp_api_core.h"

#if defined(__cplusplus)
extern "C" {
#endif

/** \cond INTERNAL */

// HAL HW configuration data
extern cyhal_qspi_t cybsp_qspi;
extern cyhal_uart_t cybsp_bt_uart;
extern cyhal_uart_t cybsp_uart;
extern cyhal_i2c_t cybsp_i2c;
extern cyhal_rtc_t cybsp_rtc;

/** \endcond */

#if defined(__cplusplus)
}
#endif

/** \} group_bsp_cy8ckit_062_ble */
