/***************************************************************************//**
* \file CY8CKIT-062S2-43012/cybsp_cy8ckit_062s2_43012.h
*
* Description:
* Provides APIs for interacting with the hardware contained on the Cypress
* CY8CKIT-062S2-43012 pioneer kit.
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
* \addtogroup group_bsp_cy8ckit_062s2_43012 CY8CKIT-062S2-43012
* \ingroup group_bsp
* \{
* The CY8CKIT-062S2-43012 PSoC 6 Wi-Fi BT Pioneer Kit is a low-cost hardware platform that enables design and debug of PSoC 6 MCUs.
* It comes with a Murata LBEE5KL1DX module, based on the CYW43012 combo device, industry-leading CapSense for touch buttons and slider, on-board debugger/programmer with KitProg3, microSD card interface, 512-Mb Quad-SPI NOR flash, PDM-PCM microphone, and a thermistor. This kit is designed with a snap-away form-factor, allowing the user to separate the different components and features that come with this kit and use independently.
* In addition, support for Digilent's Pmod interface is also provided with this kit.
*
* <div class="category">Kit Features:</div>
* <ul>
* <li>Support of up to 2MB Flash and 1MB SRAM</li>
* <li>Dedicated SDHC to interface with WICED wireless devices.</li>
* <li>Delivers dual-cores, with a 150-MHz Arm Cortex-M4 as the primary application processor and a 100-MHz Arm Cortex-M0+ as the secondary processor for low-power operations.</li>
* <li>Supports Full-Speed USB, capacitive-sensing with CapSense, a PDM-PCM digital microphone interface, a Quad-SPI interface, 13 serial communication blocks, 7 programmable analog blocks, and 56 programmable digital blocks.</li>
* </ul>
*
* <div class="category">Kit Contents:</div>
* <ul>
* <li>PSoC 6 Wi-Fi BT Pioneer Board</li>
* <li>USB Type-A to Micro-B cable</li>
* <li>Quick Start Guide</li>
* </ul>
*
* \defgroup group_bsp_cy8ckit_062s2_43012_macros Macros
* \defgroup group_bsp_cy8ckit_062s2_43012_functions Functions
* \defgroup group_bsp_cy8ckit_062s2_43012_enums Enumerated Types
*/


#pragma once

#include "cybsp_api_core.h"
#ifdef __MBED__
#include "cybsp_api_wifi.h"
#else 
#include "cybsp_retarget.h"
#include "cybsp_serial_flash.h"
#endif /* __MBED__ */

#if defined(__cplusplus)
extern "C" {
#endif


#if defined(__cplusplus)
}
#endif

/** \} group_bsp_cy8ckit_062s2_43012 */
