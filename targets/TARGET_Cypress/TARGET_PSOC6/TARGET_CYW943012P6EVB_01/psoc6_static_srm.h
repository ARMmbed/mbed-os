/*
 * mbed Microcontroller Library
 * Copyright (c) 2017-2018 Future Electronics
 * Copyright (c) 2019 Cypress Semiconductor Corporation
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

/*
 * This file defines hardware resources reserved by device-generated code. These
 * resources are accessed directly by the Peripheral Driver library (PDL).
 *
 * There are four classes of resources that must be declared here:
 *  1 CYCFG_ASSIGNED_PORTS macro defines which ports and pins are reserved.
 *    You define these as a colon separated list of ports and pins reserved
 *    using macro SRM_PORT(port_num, pins), once for each reserved port.
 *    SRM_PORT macro arguments are port number (in the range 0 ... 14) and
 *    pins, which is a hex value with a bit set for each reserved pin on a port.
 *
 *  2 CYCFG_ASSIGNED_DIVIDERS macro defines which clock dividers are reserved.
 *    You define these as a colon separated list of dividers reserved
 *    using macro SRM_DIVIDER(type, reservations), once for each required
 *    divider type.
 *    SRM_DIVIDER arguments are divider type (one of cy_en_divider_types_t
 *    values) and reservations, which is a hex mask value with a bit set for 
 *    each reserved divider of a given type.
 *
 *  3 CYCFG_ASSIGNED_SCBS macro defines which SCB blocks are reserved.
 *    You define these as a colon separated list of SCBs reserved using
 *    macro SRM_SCB(n), which argument is SCB number in a range 0 ... 7.
 *
 *  4 CYCFG_ASSIGNED_TCPWM macro defines which TCPWM blocks are reserved.
 *    You define these as a colon separated list of TCPWMs reserved using
 *    macro SRM_TCPWM(n), which argument is TCPWM number in a range 0 ... 31.
 *
 * Examples:
 *    #define CYCFG_ASSIGNED_PORTS		SRM_PORT(0, 0x30), SRM_PORT(5, 0x03)
 *
 *    #define CYCFG_ASSIGNED_DIVIDERS	SRM_DIVIDER(CY_SYSCLK_DIV_8_BIT, 0x01)
 *
 *	  #define CYCFG_ASSIGNED_SCBS		SRM_SCB(2)
 *
 *    #define CYCFG_ASSIGNED_TCPWMS
 *
 */

/* P0_0 and P0_1 reserved for WCO, 
*  P1_0 reserved for CSD TX,
*  P2_0 ... P2_5 reserved for SDHC 
*  P6-4, P6-6 and P6_7 reserved for SWD,
*  P7_0 reserved for Debug trace CLK, 
*  P7_1, P7_2 and P7_7 reserved for CSD Capacitors
*  P8_1 ... P8_7 reserved for CSD Buttons and Slider
*  P9_0 ... P9_3 reserved for Debug Trace 
*  P11_2 ... P11_7 reserved for QSPI
*  P12_6 ... P12_7 reserved for ECO
*  P14_0 ... P14_1 reserved for USB
*/
#define CYCFG_ASSIGNED_PORTS      SRM_PORT(0, 0x03), SRM_PORT(1, 0x01),\
                                  SRM_PORT(2, 0x3f), SRM_PORT(6, 0xd0),\
                                  SRM_PORT(7, 0x86), SRM_PORT(8, 0xfe),\
                                  SRM_PORT(9, 0x0f), SRM_PORT(11, 0xfc),\
                                  SRM_PORT(12, 0xc0), SRM_PORT(14, 0x03)

/*
*  8-bit divider 0 reserved for UDB
*  8-bit divider 4 reserved for CSD
*  16-bit divider 0 reserved for USB
*/
#define CYCFG_ASSIGNED_DIVIDERS   SRM_DIVIDER(CY_SYSCLK_DIV_8_BIT, 0x11), \
                                  SRM_DIVIDER(CY_SYSCLK_DIV_16_BIT, 0x01)
                               
#define CYCFG_ASSIGNED_SCBS

#define CYCFG_ASSIGNED_TCPWMS

/* End of File */
