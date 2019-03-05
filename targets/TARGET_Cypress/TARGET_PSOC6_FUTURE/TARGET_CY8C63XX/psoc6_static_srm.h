/*
 * mbed Microcontroller Library
 * Copyright (c) 2017-2018 Future Electronics
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
 * This file defines hardware resources statically allocated to M0 core
 * when static resource managemnt is used.
 *
 * There are 4 classes of resources that must be declared here:
 *  - M0_ASSIGNED_PORTS macro defines which ports and pins are reserved
 *    for M0 core use.
 *    You define these as a colon separated list of ports and pins reserved
 *    using macro SRM_PORT(port_num, pins), one time for each reserved port.
 *    SRM_PORT macro arguments are port number, in the range 0 .. 14 and
 *    pins is a hex value with a bit set for each reserved pin on a port.
 *
 *  - M0_ASSIGNED_DIVIDERS macro defines which clock dividers are reserved
 *    for M0 core use.
 *    You define these as a colon separated list of dividers reserved
 *    using macro SRM_DIVIDER(type, reservations), one time for each required
 *    devider type.
 *    SRM_DIVIDER arguments are divider type, one of cy_en_divider_types_t
 *    values and reservations is a hex mask value with a bit set for each
 *    reserved divider of a given type.
 *
 *  - M0_ASSIGNED_SCBS macro defines which SCB blocks are reserved
 *    for M0 core use.
 *    You define these as a colon separated list of SCBs reserved using
 *    macro SRM_SCB(n), which argument is SCB number in a range 0 .. 7.
 *
 *  - M0_ASSIGNED_TCPWM macro defines which TCPWM blocks are reserved
 *    for M0 core use.
 *    You define these as a colon separated list of TCPWMs reserved using
 *    macro SRM_TCPWM(n), which argument is TCPWM number in a range 0 .. 31.
 *
 * If a particular resource class is not used at all by M0 core you can
 * skip defining relevant M0_ASSIGNED_* macro or define it as an empty one.
 *
 * Examples:
 *    #define M0_ASSIGNED_PORTS		SRM_PORT(0, 0x30), SRM_PORT(5, 0x03)
 *
 *    #define M0_ASSIGNED_DIVIDERS	SRM_DIVIDER(CY_SYSCLK_DIV_8_BIT, 0x01)
 *
 *	  #define M0_ASSIGNED_SCBS		SRM_SCB(2)
 *
 *    #define M0_ASSIGNED_TCPWMS
 *
 */

// Reservations below apply to default M0 hex image.

// P0_0 and p0_1 reserved for WCO, P6-6 and P6_7 reserved for SWD
#define M0_ASSIGNED_PORTS       SRM_PORT(0, 0x03), SRM_PORT(6, 0xc0)
// 8-bit divider 0 reserved for us ticker
#define M0_ASSIGNED_DIVIDERS    SRM_DIVIDER(CY_SYSCLK_DIV_8_BIT, 0x01)
#define M0_ASSIGNED_SCBS
// TCPWM 0,0 used for us_ticker
#define M0_ASSIGNED_TCPWMS      SRM_TCPWM(0)

/* End of File */
