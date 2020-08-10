//*****************************************************************************
//
//  am_bsp_pins.h
//! @file
//!
//! @brief BSP pin configuration definitions.
//!
//! @addtogroup BSP Board Support Package (BSP)
//! @addtogroup apollo3_bsp BSP for the Apollo3 EVB.
//! @ingroup BSP
//! @{
//
//*****************************************************************************

//*****************************************************************************
//
// Copyright (c) 2019, Ambiq Micro
// All rights reserved.
// 
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions are met:
// 
// 1. Redistributions of source code must retain the above copyright notice,
// this list of conditions and the following disclaimer.
// 
// 2. Redistributions in binary form must reproduce the above copyright
// notice, this list of conditions and the following disclaimer in the
// documentation and/or other materials provided with the distribution.
// 
// 3. Neither the name of the copyright holder nor the names of its
// contributors may be used to endorse or promote products derived from this
// software without specific prior written permission.
// 
// Third party software included in this distribution is subject to the
// additional license terms as defined in the /docs/licenses directory.
// 
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
// AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
// IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
// ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
// LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
// CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
// SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
// INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
// CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
// ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
// POSSIBILITY OF SUCH DAMAGE.
//
// This is part of revision 2.2.0-hotfix-2.2.1 of the AmbiqSuite Development Package.
//
//*****************************************************************************

#ifndef _APOLLO3_PERIPHERAL_PIN_CONFIGS_H_
#define _APOLLO3_PERIPHERAL_PIN_CONFIGS_H_

#include <stdint.h>
#include <stdbool.h>
#include "am_mcu_apollo.h"

#ifdef __cplusplus
extern "C"
{
#endif

//*****************************************************************************
//
//  UART0_TX_1 pin: UART0 Tx Pin.
//
//*****************************************************************************
#define AP3_PER_UART0_TX_1          	1
extern const am_hal_gpio_pincfg_t       g_AP3_PER_UART0_TX_1;

//*****************************************************************************
//
//  UART0_TX_7 pin: UART0 Tx Pin.
//
//*****************************************************************************
#define AP3_PER_UART0_TX_7          	7
extern const am_hal_gpio_pincfg_t       g_AP3_PER_UART0_TX_7;

//*****************************************************************************
//
//  UART0_TX_16 pin: UART0 Tx Pin.
//
//*****************************************************************************
#define AP3_PER_UART0_TX_16         	16
extern const am_hal_gpio_pincfg_t       g_AP3_PER_UART0_TX_16;

//*****************************************************************************
//
//  UART0_TX_20 pin: UART0 Tx Pin.
//
//*****************************************************************************
#define AP3_PER_UART0_TX_20         	20
extern const am_hal_gpio_pincfg_t       g_AP3_PER_UART0_TX_20;

//*****************************************************************************
//
//  UART0_TX_22 pin: UART0 Tx Pin.
//
//*****************************************************************************
#define AP3_PER_UART0_TX_22         	22
extern const am_hal_gpio_pincfg_t       g_AP3_PER_UART0_TX_22;

//*****************************************************************************
//
//  UART0_TX_26 pin: UART0 Tx Pin.
//
//*****************************************************************************
#define AP3_PER_UART0_TX_26         	26
extern const am_hal_gpio_pincfg_t       g_AP3_PER_UART0_TX_26;

//*****************************************************************************
//
//  UART0_TX_28 pin: UART0 Tx Pin.
//
//*****************************************************************************
#define AP3_PER_UART0_TX_28         	28
extern const am_hal_gpio_pincfg_t       g_AP3_PER_UART0_TX_28;

//*****************************************************************************
//
//  UART0_TX_30 pin: UART0 Tx Pin.
//
//*****************************************************************************
#if defined (AM_PACKAGE_BGA)
#define AP3_PER_UART0_TX_30         	30
extern const am_hal_gpio_pincfg_t       g_AP3_PER_UART0_TX_30;
#endif // AM_PACKAGE_BGA

//*****************************************************************************
//
//  UART0_TX_39 pin: UART0 Tx Pin.
//
//*****************************************************************************
#define AP3_PER_UART0_TX_39         	39
extern const am_hal_gpio_pincfg_t       g_AP3_PER_UART0_TX_39;

//*****************************************************************************
//
//  UART0_TX_41 pin: UART0 Tx Pin.
//
//*****************************************************************************
#define AP3_PER_UART0_TX_41         	41
extern const am_hal_gpio_pincfg_t       g_AP3_PER_UART0_TX_41;

//*****************************************************************************
//
//  UART0_TX_44 pin: UART0 Tx Pin.
//
//*****************************************************************************
#define AP3_PER_UART0_TX_44         	44
extern const am_hal_gpio_pincfg_t       g_AP3_PER_UART0_TX_44;

//*****************************************************************************
//
//  UART0_TX_48 pin: UART0 Tx Pin.
//
//*****************************************************************************
#define AP3_PER_UART0_TX_48         	48
extern const am_hal_gpio_pincfg_t       g_AP3_PER_UART0_TX_48;

//*****************************************************************************
//
//  UART1_TX_8 pin: UART1 Tx Pin.
//
//*****************************************************************************
#define AP3_PER_UART1_TX_8          	8
extern const am_hal_gpio_pincfg_t       g_AP3_PER_UART1_TX_8;

//*****************************************************************************
//
//  UART1_TX_10 pin: UART1 Tx Pin.
//
//*****************************************************************************
#define AP3_PER_UART1_TX_10         	10
extern const am_hal_gpio_pincfg_t       g_AP3_PER_UART1_TX_10;

//*****************************************************************************
//
//  UART1_TX_12 pin: UART1 Tx Pin.
//
//*****************************************************************************
#define AP3_PER_UART1_TX_12         	12
extern const am_hal_gpio_pincfg_t       g_AP3_PER_UART1_TX_12;

//*****************************************************************************
//
//  UART1_TX_14 pin: UART1 Tx Pin.
//
//*****************************************************************************
#define AP3_PER_UART1_TX_14         	14
extern const am_hal_gpio_pincfg_t       g_AP3_PER_UART1_TX_14;

//*****************************************************************************
//
//  UART1_TX_18 pin: UART1 Tx Pin.
//
//*****************************************************************************
#define AP3_PER_UART1_TX_18         	18
extern const am_hal_gpio_pincfg_t       g_AP3_PER_UART1_TX_18;

//*****************************************************************************
//
//  UART1_TX_20 pin: UART1 Tx Pin.
//
//*****************************************************************************
#define AP3_PER_UART1_TX_20         	20
extern const am_hal_gpio_pincfg_t       g_AP3_PER_UART1_TX_20;

//*****************************************************************************
//
//  UART1_TX_24 pin: UART1 Tx Pin.
//
//*****************************************************************************
#define AP3_PER_UART1_TX_24         	24
extern const am_hal_gpio_pincfg_t       g_AP3_PER_UART1_TX_24;

//*****************************************************************************
//
//  UART1_TX_35 pin: UART1 Tx Pin.
//
//*****************************************************************************
#if defined (AM_PACKAGE_BGA)
#define AP3_PER_UART1_TX_35         	35
extern const am_hal_gpio_pincfg_t       g_AP3_PER_UART1_TX_35;
#endif // AM_PACKAGE_BGA

//*****************************************************************************
//
//  UART1_TX_37 pin: UART1 Tx Pin.
//
//*****************************************************************************
#if defined (AM_PACKAGE_BGA)
#define AP3_PER_UART1_TX_37         	37
extern const am_hal_gpio_pincfg_t       g_AP3_PER_UART1_TX_37;
#endif // AM_PACKAGE_BGA

//*****************************************************************************
//
//  UART1_TX_39 pin: UART1 Tx Pin.
//
//*****************************************************************************
#define AP3_PER_UART1_TX_39         	39
extern const am_hal_gpio_pincfg_t       g_AP3_PER_UART1_TX_39;

//*****************************************************************************
//
//  UART1_TX_42 pin: UART1 Tx Pin.
//
//*****************************************************************************
#if defined (AM_PACKAGE_BGA)
#define AP3_PER_UART1_TX_42         	42
extern const am_hal_gpio_pincfg_t       g_AP3_PER_UART1_TX_42;
#endif // AM_PACKAGE_BGA

//*****************************************************************************
//
//  UART1_TX_46 pin: UART1 Tx Pin.
//
//*****************************************************************************
#if defined (AM_PACKAGE_BGA)
#define AP3_PER_UART1_TX_46         	46
extern const am_hal_gpio_pincfg_t       g_AP3_PER_UART1_TX_46;
#endif // AM_PACKAGE_BGA

//*****************************************************************************
//
//  UART0_RX_2 pin: UART0 Rx Pin.
//
//*****************************************************************************
#define AP3_PER_UART0_RX_2          	2
extern const am_hal_gpio_pincfg_t       g_AP3_PER_UART0_RX_2;

//*****************************************************************************
//
//  UART0_RX_11 pin: UART0 Rx Pin.
//
//*****************************************************************************
#define AP3_PER_UART0_RX_11         	11
extern const am_hal_gpio_pincfg_t       g_AP3_PER_UART0_RX_11;

//*****************************************************************************
//
//  UART0_RX_17 pin: UART0 Rx Pin.
//
//*****************************************************************************
#define AP3_PER_UART0_RX_17         	17
extern const am_hal_gpio_pincfg_t       g_AP3_PER_UART0_RX_17;

//*****************************************************************************
//
//  UART0_RX_21 pin: UART0 Rx Pin.
//
//*****************************************************************************
#define AP3_PER_UART0_RX_21         	21
extern const am_hal_gpio_pincfg_t       g_AP3_PER_UART0_RX_21;

//*****************************************************************************
//
//  UART0_RX_23 pin: UART0 Rx Pin.
//
//*****************************************************************************
#define AP3_PER_UART0_RX_23         	23
extern const am_hal_gpio_pincfg_t       g_AP3_PER_UART0_RX_23;

//*****************************************************************************
//
//  UART0_RX_27 pin: UART0 Rx Pin.
//
//*****************************************************************************
#define AP3_PER_UART0_RX_27         	27
extern const am_hal_gpio_pincfg_t       g_AP3_PER_UART0_RX_27;

//*****************************************************************************
//
//  UART0_RX_29 pin: UART0 Rx Pin.
//
//*****************************************************************************
#define AP3_PER_UART0_RX_29         	29
extern const am_hal_gpio_pincfg_t       g_AP3_PER_UART0_RX_29;

//*****************************************************************************
//
//  UART0_RX_31 pin: UART0 Rx Pin.
//
//*****************************************************************************
#if defined (AM_PACKAGE_BGA)
#define AP3_PER_UART0_RX_31         	31
extern const am_hal_gpio_pincfg_t       g_AP3_PER_UART0_RX_31;
#endif // AM_PACKAGE_BGA

//*****************************************************************************
//
//  UART0_RX_34 pin: UART0 Rx Pin.
//
//*****************************************************************************
#if defined (AM_PACKAGE_BGA)
#define AP3_PER_UART0_RX_34         	34
extern const am_hal_gpio_pincfg_t       g_AP3_PER_UART0_RX_34;
#endif // AM_PACKAGE_BGA

//*****************************************************************************
//
//  UART0_RX_40 pin: UART0 Rx Pin.
//
//*****************************************************************************
#define AP3_PER_UART0_RX_40         	40
extern const am_hal_gpio_pincfg_t       g_AP3_PER_UART0_RX_40;

//*****************************************************************************
//
//  UART0_RX_45 pin: UART0 Rx Pin.
//
//*****************************************************************************
#if defined (AM_PACKAGE_BGA)
#define AP3_PER_UART0_RX_45         	45
extern const am_hal_gpio_pincfg_t       g_AP3_PER_UART0_RX_45;
#endif // AM_PACKAGE_BGA

//*****************************************************************************
//
//  UART0_RX_49 pin: UART0 Rx Pin.
//
//*****************************************************************************
#define AP3_PER_UART0_RX_49         	49
extern const am_hal_gpio_pincfg_t       g_AP3_PER_UART0_RX_49;

//*****************************************************************************
//
//  UART1_RX_2 pin: UART1 Rx Pin.
//
//*****************************************************************************
#define AP3_PER_UART1_RX_2          	2
extern const am_hal_gpio_pincfg_t       g_AP3_PER_UART1_RX_2;

//*****************************************************************************
//
//  UART1_RX_4 pin: UART1 Rx Pin.
//
//*****************************************************************************
#define AP3_PER_UART1_RX_4          	4
extern const am_hal_gpio_pincfg_t       g_AP3_PER_UART1_RX_4;

//*****************************************************************************
//
//  UART1_RX_9 pin: UART1 Rx Pin.
//
//*****************************************************************************
#define AP3_PER_UART1_RX_9          	9
extern const am_hal_gpio_pincfg_t       g_AP3_PER_UART1_RX_9;

//*****************************************************************************
//
//  UART1_RX_13 pin: UART1 Rx Pin.
//
//*****************************************************************************
#define AP3_PER_UART1_RX_13         	13
extern const am_hal_gpio_pincfg_t       g_AP3_PER_UART1_RX_13;

//*****************************************************************************
//
//  UART1_RX_15 pin: UART1 Rx Pin.
//
//*****************************************************************************
#define AP3_PER_UART1_RX_15         	15
extern const am_hal_gpio_pincfg_t       g_AP3_PER_UART1_RX_15;

//*****************************************************************************
//
//  UART1_RX_19 pin: UART1 Rx Pin.
//
//*****************************************************************************
#define AP3_PER_UART1_RX_19         	19
extern const am_hal_gpio_pincfg_t       g_AP3_PER_UART1_RX_19;

//*****************************************************************************
//
//  UART1_RX_21 pin: UART1 Rx Pin.
//
//*****************************************************************************
#define AP3_PER_UART1_RX_21         	21
extern const am_hal_gpio_pincfg_t       g_AP3_PER_UART1_RX_21;

//*****************************************************************************
//
//  UART1_RX_25 pin: UART1 Rx Pin.
//
//*****************************************************************************
#define AP3_PER_UART1_RX_25         	25
extern const am_hal_gpio_pincfg_t       g_AP3_PER_UART1_RX_25;

//*****************************************************************************
//
//  UART1_RX_36 pin: UART1 Rx Pin.
//
//*****************************************************************************
#if defined (AM_PACKAGE_BGA)
#define AP3_PER_UART1_RX_36         	36
extern const am_hal_gpio_pincfg_t       g_AP3_PER_UART1_RX_36;
#endif // AM_PACKAGE_BGA

//*****************************************************************************
//
//  UART1_RX_38 pin: UART1 Rx Pin.
//
//*****************************************************************************
#if defined (AM_PACKAGE_BGA)
#define AP3_PER_UART1_RX_38         	38
extern const am_hal_gpio_pincfg_t       g_AP3_PER_UART1_RX_38;
#endif // AM_PACKAGE_BGA

//*****************************************************************************
//
//  UART1_RX_40 pin: UART1 Rx Pin.
//
//*****************************************************************************
#define AP3_PER_UART1_RX_40         	40
extern const am_hal_gpio_pincfg_t       g_AP3_PER_UART1_RX_40;

//*****************************************************************************
//
//  UART1_RX_43 pin: UART1 Rx Pin.
//
//*****************************************************************************
#if defined (AM_PACKAGE_BGA)
#define AP3_PER_UART1_RX_43         	43
extern const am_hal_gpio_pincfg_t       g_AP3_PER_UART1_RX_43;
#endif // AM_PACKAGE_BGA

//*****************************************************************************
//
//  UART1_RX_47 pin: UART1 Rx Pin.
//
//*****************************************************************************
#define AP3_PER_UART1_RX_47         	47
extern const am_hal_gpio_pincfg_t       g_AP3_PER_UART1_RX_47;

//*****************************************************************************
//
//  UART0_RTS_3 pin: UART0 RTS Pin.
//
//*****************************************************************************
#define AP3_PER_UART0_RTS_3         	3
extern const am_hal_gpio_pincfg_t       g_AP3_PER_UART0_RTS_3;

//*****************************************************************************
//
//  UART0_RTS_5 pin: UART0 RTS Pin.
//
//*****************************************************************************
#define AP3_PER_UART0_RTS_5         	5
extern const am_hal_gpio_pincfg_t       g_AP3_PER_UART0_RTS_5;

//*****************************************************************************
//
//  UART0_RTS_13 pin: UART0 RTS Pin.
//
//*****************************************************************************
#define AP3_PER_UART0_RTS_13        	13
extern const am_hal_gpio_pincfg_t       g_AP3_PER_UART0_RTS_13;

//*****************************************************************************
//
//  UART0_RTS_18 pin: UART0 RTS Pin.
//
//*****************************************************************************
#define AP3_PER_UART0_RTS_18        	18
extern const am_hal_gpio_pincfg_t       g_AP3_PER_UART0_RTS_18;

//*****************************************************************************
//
//  UART0_RTS_34 pin: UART0 RTS Pin.
//
//*****************************************************************************
#if defined (AM_PACKAGE_BGA)
#define AP3_PER_UART0_RTS_34        	34
extern const am_hal_gpio_pincfg_t       g_AP3_PER_UART0_RTS_34;
#endif // AM_PACKAGE_BGA

//*****************************************************************************
//
//  UART0_RTS_35 pin: UART0 RTS Pin.
//
//*****************************************************************************
#if defined (AM_PACKAGE_BGA)
#define AP3_PER_UART0_RTS_35        	35
extern const am_hal_gpio_pincfg_t       g_AP3_PER_UART0_RTS_35;
#endif // AM_PACKAGE_BGA

//*****************************************************************************
//
//  UART0_RTS_37 pin: UART0 RTS Pin.
//
//*****************************************************************************
#if defined (AM_PACKAGE_BGA)
#define AP3_PER_UART0_RTS_37        	37
extern const am_hal_gpio_pincfg_t       g_AP3_PER_UART0_RTS_37;
#endif // AM_PACKAGE_BGA

//*****************************************************************************
//
//  UART0_RTS_41 pin: UART0 RTS Pin.
//
//*****************************************************************************
#define AP3_PER_UART0_RTS_41        	41
extern const am_hal_gpio_pincfg_t       g_AP3_PER_UART0_RTS_41;

//*****************************************************************************
//
//  UART1_RTS_10 pin: UART1 RTS Pin.
//
//*****************************************************************************
#define AP3_PER_UART1_RTS_10        	10
extern const am_hal_gpio_pincfg_t       g_AP3_PER_UART1_RTS_10;

//*****************************************************************************
//
//  UART1_RTS_16 pin: UART1 RTS Pin.
//
//*****************************************************************************
#define AP3_PER_UART1_RTS_16        	16
extern const am_hal_gpio_pincfg_t       g_AP3_PER_UART1_RTS_16;

//*****************************************************************************
//
//  UART1_RTS_20 pin: UART1 RTS Pin.
//
//*****************************************************************************
#define AP3_PER_UART1_RTS_20        	20
extern const am_hal_gpio_pincfg_t       g_AP3_PER_UART1_RTS_20;

//*****************************************************************************
//
//  UART1_RTS_30 pin: UART1 RTS Pin.
//
//*****************************************************************************
#if defined (AM_PACKAGE_BGA)
#define AP3_PER_UART1_RTS_30        	30
extern const am_hal_gpio_pincfg_t       g_AP3_PER_UART1_RTS_30;
#endif // AM_PACKAGE_BGA

//*****************************************************************************
//
//  UART1_RTS_31 pin: UART1 RTS Pin.
//
//*****************************************************************************
#if defined (AM_PACKAGE_BGA)
#define AP3_PER_UART1_RTS_31        	31
extern const am_hal_gpio_pincfg_t       g_AP3_PER_UART1_RTS_31;
#endif // AM_PACKAGE_BGA

//*****************************************************************************
//
//  UART1_RTS_34 pin: UART1 RTS Pin.
//
//*****************************************************************************
#if defined (AM_PACKAGE_BGA)
#define AP3_PER_UART1_RTS_34        	34
extern const am_hal_gpio_pincfg_t       g_AP3_PER_UART1_RTS_34;
#endif // AM_PACKAGE_BGA

//*****************************************************************************
//
//  UART1_RTS_41 pin: UART1 RTS Pin.
//
//*****************************************************************************
#define AP3_PER_UART1_RTS_41        	41
extern const am_hal_gpio_pincfg_t       g_AP3_PER_UART1_RTS_41;

//*****************************************************************************
//
//  UART1_RTS_44 pin: UART1 RTS Pin.
//
//*****************************************************************************
#define AP3_PER_UART1_RTS_44        	44
extern const am_hal_gpio_pincfg_t       g_AP3_PER_UART1_RTS_44;

//*****************************************************************************
//
//  UART0_CTS_4 pin: UART0 CTS Pin.
//
//*****************************************************************************
#define AP3_PER_UART0_CTS_4         	4
extern const am_hal_gpio_pincfg_t       g_AP3_PER_UART0_CTS_4;

//*****************************************************************************
//
//  UART0_CTS_6 pin: UART0 CTS Pin.
//
//*****************************************************************************
#define AP3_PER_UART0_CTS_6         	6
extern const am_hal_gpio_pincfg_t       g_AP3_PER_UART0_CTS_6;

//*****************************************************************************
//
//  UART0_CTS_12 pin: UART0 CTS Pin.
//
//*****************************************************************************
#define AP3_PER_UART0_CTS_12        	12
extern const am_hal_gpio_pincfg_t       g_AP3_PER_UART0_CTS_12;

//*****************************************************************************
//
//  UART0_CTS_24 pin: UART0 CTS Pin.
//
//*****************************************************************************
#define AP3_PER_UART0_CTS_24        	24
extern const am_hal_gpio_pincfg_t       g_AP3_PER_UART0_CTS_24;

//*****************************************************************************
//
//  UART0_CTS_29 pin: UART0 CTS Pin.
//
//*****************************************************************************
#define AP3_PER_UART0_CTS_29        	29
extern const am_hal_gpio_pincfg_t       g_AP3_PER_UART0_CTS_29;

//*****************************************************************************
//
//  UART0_CTS_33 pin: UART0 CTS Pin.
//
//*****************************************************************************
#if defined (AM_PACKAGE_BGA)
#define AP3_PER_UART0_CTS_33        	33
extern const am_hal_gpio_pincfg_t       g_AP3_PER_UART0_CTS_33;
#endif // AM_PACKAGE_BGA

//*****************************************************************************
//
//  UART0_CTS_36 pin: UART0 CTS Pin.
//
//*****************************************************************************
#if defined (AM_PACKAGE_BGA)
#define AP3_PER_UART0_CTS_36        	36
extern const am_hal_gpio_pincfg_t       g_AP3_PER_UART0_CTS_36;
#endif // AM_PACKAGE_BGA

//*****************************************************************************
//
//  UART0_CTS_38 pin: UART0 CTS Pin.
//
//*****************************************************************************
#if defined (AM_PACKAGE_BGA)
#define AP3_PER_UART0_CTS_38        	38
extern const am_hal_gpio_pincfg_t       g_AP3_PER_UART0_CTS_38;
#endif // AM_PACKAGE_BGA

//*****************************************************************************
//
//  UART1_CTS_11 pin: UART1 CTS Pin.
//
//*****************************************************************************
#define AP3_PER_UART1_CTS_11        	11
extern const am_hal_gpio_pincfg_t       g_AP3_PER_UART1_CTS_11;

//*****************************************************************************
//
//  UART1_CTS_17 pin: UART1 CTS Pin.
//
//*****************************************************************************
#define AP3_PER_UART1_CTS_17        	17
extern const am_hal_gpio_pincfg_t       g_AP3_PER_UART1_CTS_17;

//*****************************************************************************
//
//  UART1_CTS_21 pin: UART1 CTS Pin.
//
//*****************************************************************************
#define AP3_PER_UART1_CTS_21        	21
extern const am_hal_gpio_pincfg_t       g_AP3_PER_UART1_CTS_21;

//*****************************************************************************
//
//  UART1_CTS_26 pin: UART1 CTS Pin.
//
//*****************************************************************************
#define AP3_PER_UART1_CTS_26        	26
extern const am_hal_gpio_pincfg_t       g_AP3_PER_UART1_CTS_26;

//*****************************************************************************
//
//  UART1_CTS_29 pin: UART1 CTS Pin.
//
//*****************************************************************************
#define AP3_PER_UART1_CTS_29        	29
extern const am_hal_gpio_pincfg_t       g_AP3_PER_UART1_CTS_29;

//*****************************************************************************
//
//  UART1_CTS_32 pin: UART1 CTS Pin.
//
//*****************************************************************************
#if defined (AM_PACKAGE_BGA)
#define AP3_PER_UART1_CTS_32        	32
extern const am_hal_gpio_pincfg_t       g_AP3_PER_UART1_CTS_32;
#endif // AM_PACKAGE_BGA

//*****************************************************************************
//
//  UART1_CTS_36 pin: UART1 CTS Pin.
//
//*****************************************************************************
#if defined (AM_PACKAGE_BGA)
#define AP3_PER_UART1_CTS_36        	36
extern const am_hal_gpio_pincfg_t       g_AP3_PER_UART1_CTS_36;
#endif // AM_PACKAGE_BGA

//*****************************************************************************
//
//  UART1_CTS_45 pin: UART1 CTS Pin.
//
//*****************************************************************************
#if defined (AM_PACKAGE_BGA)
#define AP3_PER_UART1_CTS_45        	45
extern const am_hal_gpio_pincfg_t       g_AP3_PER_UART1_CTS_45;
#endif // AM_PACKAGE_BGA

//*****************************************************************************
//
//  IOM0_CS pin: I/O Master 0 chip select.
//
//*****************************************************************************
#define AP3_PER_IOM0_CS             	11
extern const am_hal_gpio_pincfg_t       g_AP3_PER_IOM0_CS;
#define AP3_PER_IOM0_CS_CHNL            0

//*****************************************************************************
//
//  IOM0_CS3 pin: I/O Master 0 chip select.
//
//*****************************************************************************
#define AP3_PER_IOM0_CS3            	15
extern const am_hal_gpio_pincfg_t       g_AP3_PER_IOM0_CS3;
#define AP3_PER_IOM0_CS3_CHNL           3

//*****************************************************************************
//
//  IOM0_MISO pin: I/O Master 0 SPI MISO signal.
//
//*****************************************************************************
#define AP3_PER_IOM0_MISO           	6
extern const am_hal_gpio_pincfg_t       g_AP3_PER_IOM0_MISO;

//*****************************************************************************
//
//  IOM0_MOSI pin: I/O Master 0 SPI MOSI signal.
//
//*****************************************************************************
#define AP3_PER_IOM0_MOSI           	7
extern const am_hal_gpio_pincfg_t       g_AP3_PER_IOM0_MOSI;

//*****************************************************************************
//
//  IOM0_SCK pin: I/O Master 0 SPI SCK signal.
//
//*****************************************************************************
#define AP3_PER_IOM0_SCK            	5
extern const am_hal_gpio_pincfg_t       g_AP3_PER_IOM0_SCK;

//*****************************************************************************
//
//  IOM0_SCL pin: I/O Master 0 I2C clock signal.
//
//*****************************************************************************
#define AP3_PER_IOM0_SCL            	5
extern const am_hal_gpio_pincfg_t       g_AP3_PER_IOM0_SCL;

//*****************************************************************************
//
//  IOM0_SDA pin: I/O Master 0 I2C data signal.
//
//*****************************************************************************
#define AP3_PER_IOM0_SDA            	6
extern const am_hal_gpio_pincfg_t       g_AP3_PER_IOM0_SDA;

//*****************************************************************************
//
//  IOM1_CS pin: I/O Master 1 chip select.
//
//*****************************************************************************
#define AP3_PER_IOM1_CS             	14
extern const am_hal_gpio_pincfg_t       g_AP3_PER_IOM1_CS;
#define AP3_PER_IOM1_CS_CHNL            2

//*****************************************************************************
//
//  IOM1_MISO pin: I/O Master 1 SPI MISO signal.
//
//*****************************************************************************
#define AP3_PER_IOM1_MISO           	9
extern const am_hal_gpio_pincfg_t       g_AP3_PER_IOM1_MISO;

//*****************************************************************************
//
//  IOM1_MOSI pin: I/O Master 1 SPI MOSI signal.
//
//*****************************************************************************
#define AP3_PER_IOM1_MOSI           	10
extern const am_hal_gpio_pincfg_t       g_AP3_PER_IOM1_MOSI;

//*****************************************************************************
//
//  IOM1_SCK pin: I/O Master 1 SPI SCK signal.
//
//*****************************************************************************
#define AP3_PER_IOM1_SCK            	8
extern const am_hal_gpio_pincfg_t       g_AP3_PER_IOM1_SCK;

//*****************************************************************************
//
//  IOM1_SCL pin: I/O Master 1 I2C clock signal.
//
//*****************************************************************************
#define AP3_PER_IOM1_SCL            	8
extern const am_hal_gpio_pincfg_t       g_AP3_PER_IOM1_SCL;

//*****************************************************************************
//
//  IOM1_SDA pin: I/O Master 1 I2C data signal.
//
//*****************************************************************************
#define AP3_PER_IOM1_SDA            	9
extern const am_hal_gpio_pincfg_t       g_AP3_PER_IOM1_SDA;

//*****************************************************************************
//
//  IOM2_CS pin: I/O Master 2 chip select.
//
//*****************************************************************************
#define AP3_PER_IOM2_CS             	15
extern const am_hal_gpio_pincfg_t       g_AP3_PER_IOM2_CS;
#define AP3_PER_IOM2_CS_CHNL            3

//*****************************************************************************
//
//  IOM2_MISO pin: I/O Master 2 SPI MISO signal.
//
//*****************************************************************************
#define AP3_PER_IOM2_MISO           	25
extern const am_hal_gpio_pincfg_t       g_AP3_PER_IOM2_MISO;

//*****************************************************************************
//
//  IOM2_MOSI pin: I/O Master 2 SPI MOSI signal.
//
//*****************************************************************************
#define AP3_PER_IOM2_MOSI           	28
extern const am_hal_gpio_pincfg_t       g_AP3_PER_IOM2_MOSI;

//*****************************************************************************
//
//  IOM2_SCK pin: I/O Master 2 SPI SCK signal.
//
//*****************************************************************************
#define AP3_PER_IOM2_SCK            	27
extern const am_hal_gpio_pincfg_t       g_AP3_PER_IOM2_SCK;

//*****************************************************************************
//
//  IOM2_SCL pin: I/O Master 2 I2C clock signal.
//
//*****************************************************************************
#define AP3_PER_IOM2_SCL            	27
extern const am_hal_gpio_pincfg_t       g_AP3_PER_IOM2_SCL;

//*****************************************************************************
//
//  IOM2_SDA pin: I/O Master 2 I2C data signal.
//
//*****************************************************************************
#define AP3_PER_IOM2_SDA            	25
extern const am_hal_gpio_pincfg_t       g_AP3_PER_IOM2_SDA;

//*****************************************************************************
//
//  IOM3_CS pin: I/O Master 3 chip select.
//
//*****************************************************************************
#define AP3_PER_IOM3_CS             	12
extern const am_hal_gpio_pincfg_t       g_AP3_PER_IOM3_CS;
#define AP3_PER_IOM3_CS_CHNL            0

//*****************************************************************************
//
//  IOM3_MISO pin: I/O Master 3 SPI MISO signal.
//
//*****************************************************************************
#if defined (AM_PACKAGE_BGA)
#define AP3_PER_IOM3_MISO           	43
extern const am_hal_gpio_pincfg_t       g_AP3_PER_IOM3_MISO;
#endif // AM_PACKAGE_BGA

//*****************************************************************************
//
//  IOM3_MOSI pin: I/O Master 3 SPI MOSI signal.
//
//*****************************************************************************
#if defined (AM_PACKAGE_BGA)
#define AP3_PER_IOM3_MOSI           	38
extern const am_hal_gpio_pincfg_t       g_AP3_PER_IOM3_MOSI;
#endif // AM_PACKAGE_BGA

//*****************************************************************************
//
//  IOM3_SCK pin: I/O Master 3 SPI SCK signal.
//
//*****************************************************************************
#if defined (AM_PACKAGE_BGA)
#define AP3_PER_IOM3_SCK            	42
extern const am_hal_gpio_pincfg_t       g_AP3_PER_IOM3_SCK;
#endif // AM_PACKAGE_BGA

//*****************************************************************************
//
//  IOM3_SCL pin: I/O Master 3 I2C clock signal.
//
//*****************************************************************************
#if defined (AM_PACKAGE_BGA)
#define AP3_PER_IOM3_SCL            	42
extern const am_hal_gpio_pincfg_t       g_AP3_PER_IOM3_SCL;
#endif // AM_PACKAGE_BGA

//*****************************************************************************
//
//  IOM3_SDA pin: I/O Master 3 I2C data signal.
//
//*****************************************************************************
#if defined (AM_PACKAGE_BGA)
#define AP3_PER_IOM3_SDA            	43
extern const am_hal_gpio_pincfg_t       g_AP3_PER_IOM3_SDA;
#endif // AM_PACKAGE_BGA

//*****************************************************************************
//
//  IOM4_CS pin: I/O Master 4 chip select.
//
//*****************************************************************************
#define AP3_PER_IOM4_CS             	13
extern const am_hal_gpio_pincfg_t       g_AP3_PER_IOM4_CS;
#define AP3_PER_IOM4_CS_CHNL            1

//*****************************************************************************
//
//  IOM4_MISO pin: I/O Master 4 SPI MISO signal.
//
//*****************************************************************************
#define AP3_PER_IOM4_MISO           	40
extern const am_hal_gpio_pincfg_t       g_AP3_PER_IOM4_MISO;

//*****************************************************************************
//
//  IOM4_MOSI pin: I/O Master 4 SPI MOSI signal.
//
//*****************************************************************************
#define AP3_PER_IOM4_MOSI           	44
extern const am_hal_gpio_pincfg_t       g_AP3_PER_IOM4_MOSI;

//*****************************************************************************
//
//  IOM4_SCK pin: I/O Master 4 SPI SCK signal.
//
//*****************************************************************************
#define AP3_PER_IOM4_SCK            	39
extern const am_hal_gpio_pincfg_t       g_AP3_PER_IOM4_SCK;

//*****************************************************************************
//
//  IOM4_SCL pin: I/O Master 4 I2C clock signal.
//
//*****************************************************************************
#define AP3_PER_IOM4_SCL            	39
extern const am_hal_gpio_pincfg_t       g_AP3_PER_IOM4_SCL;

//*****************************************************************************
//
//  IOM4_SDA pin: I/O Master 4 I2C data signal.
//
//*****************************************************************************
#define AP3_PER_IOM4_SDA            	40
extern const am_hal_gpio_pincfg_t       g_AP3_PER_IOM4_SDA;

//*****************************************************************************
//
//  IOM5_CS pin: I/O Master 5 chip select.
//
//*****************************************************************************
#define AP3_PER_IOM5_CS             	16
extern const am_hal_gpio_pincfg_t       g_AP3_PER_IOM5_CS;
#define AP3_PER_IOM5_CS_CHNL            0

//*****************************************************************************
//
//  IOM5_MISO pin: I/O Master 5 SPI MISO signal.
//
//*****************************************************************************
#define AP3_PER_IOM5_MISO           	49
extern const am_hal_gpio_pincfg_t       g_AP3_PER_IOM5_MISO;

//*****************************************************************************
//
//  IOM5_MOSI pin: I/O Master 5 SPI MOSI signal.
//
//*****************************************************************************
#define AP3_PER_IOM5_MOSI           	47
extern const am_hal_gpio_pincfg_t       g_AP3_PER_IOM5_MOSI;

//*****************************************************************************
//
//  IOM5_SCK pin: I/O Master 5 SPI SCK signal.
//
//*****************************************************************************
#define AP3_PER_IOM5_SCK            	48
extern const am_hal_gpio_pincfg_t       g_AP3_PER_IOM5_SCK;

//*****************************************************************************
//
//  IOM5_SCL pin: I/O Master 5 I2C clock signal.
//
//*****************************************************************************
#define AP3_PER_IOM5_SCL            	48
extern const am_hal_gpio_pincfg_t       g_AP3_PER_IOM5_SCL;

//*****************************************************************************
//
//  IOM5_SDA pin: I/O Master 5 I2C data signal.
//
//*****************************************************************************
#define AP3_PER_IOM5_SDA            	49
extern const am_hal_gpio_pincfg_t       g_AP3_PER_IOM5_SDA;

//*****************************************************************************
//
//  MSPI_CE0 pin: MSPI chip select.
//
//*****************************************************************************
#define AP3_PER_MSPI_CE0            	19
extern const am_hal_gpio_pincfg_t       g_AP3_PER_MSPI_CE0;
#define AP3_PER_MSPI_CE0_CHNL           0

//*****************************************************************************
//
//  MSPI_CE1 pin: MSPI chip select.
//
//*****************************************************************************
#define AP3_PER_MSPI_CE1            	41
extern const am_hal_gpio_pincfg_t       g_AP3_PER_MSPI_CE1;
#define AP3_PER_MSPI_CE1_CHNL           1

//*****************************************************************************
//
//  MSPI_D0 pin: MSPI data 0.
//
//*****************************************************************************
#define AP3_PER_MSPI_D0             	22
extern const am_hal_gpio_pincfg_t       g_AP3_PER_MSPI_D0;

//*****************************************************************************
//
//  MSPI_D1 pin: MSPI data 1.
//
//*****************************************************************************
#define AP3_PER_MSPI_D1             	26
extern const am_hal_gpio_pincfg_t       g_AP3_PER_MSPI_D1;

//*****************************************************************************
//
//  MSPI_D2 pin: MSPI data 2.
//
//*****************************************************************************
#define AP3_PER_MSPI_D2             	4
extern const am_hal_gpio_pincfg_t       g_AP3_PER_MSPI_D2;

//*****************************************************************************
//
//  MSPI_D3 pin: MSPI data 3.
//
//*****************************************************************************
#define AP3_PER_MSPI_D3             	23
extern const am_hal_gpio_pincfg_t       g_AP3_PER_MSPI_D3;

//*****************************************************************************
//
//  MSPI_D4 pin: MSPI data 4.
//
//*****************************************************************************
#define AP3_PER_MSPI_D4             	0
extern const am_hal_gpio_pincfg_t       g_AP3_PER_MSPI_D4;

//*****************************************************************************
//
//  MSPI_D5 pin: MSPI data 5.
//
//*****************************************************************************
#define AP3_PER_MSPI_D5             	1
extern const am_hal_gpio_pincfg_t       g_AP3_PER_MSPI_D5;

//*****************************************************************************
//
//  MSPI_D6 pin: MSPI data 6.
//
//*****************************************************************************
#define AP3_PER_MSPI_D6             	2
extern const am_hal_gpio_pincfg_t       g_AP3_PER_MSPI_D6;

//*****************************************************************************
//
//  MSPI_D7 pin: MSPI data 7.
//
//*****************************************************************************
#define AP3_PER_MSPI_D7             	3
extern const am_hal_gpio_pincfg_t       g_AP3_PER_MSPI_D7;

//*****************************************************************************
//
//  MSPI_SCK pin: MSPI clock.
//
//*****************************************************************************
#define AP3_PER_MSPI_SCK            	24
extern const am_hal_gpio_pincfg_t       g_AP3_PER_MSPI_SCK;

//*****************************************************************************
//
//  IOS_CE pin: I/O Slave chip select.
//
//*****************************************************************************
#define AP3_PER_IOS_CE              	3
extern const am_hal_gpio_pincfg_t       g_AP3_PER_IOS_CE;
#define AP3_PER_IOS_CE_CHNL             0

//*****************************************************************************
//
//  IOS_MISO pin: I/O Slave SPI MISO signal.
//
//*****************************************************************************
#define AP3_PER_IOS_MISO            	2
extern const am_hal_gpio_pincfg_t       g_AP3_PER_IOS_MISO;

//*****************************************************************************
//
//  IOS_MOSI pin: I/O Slave SPI MOSI signal.
//
//*****************************************************************************
#define AP3_PER_IOS_MOSI            	1
extern const am_hal_gpio_pincfg_t       g_AP3_PER_IOS_MOSI;

//*****************************************************************************
//
//  IOS_SCK pin: I/O Slave SPI SCK signal.
//
//*****************************************************************************
#define AP3_PER_IOS_SCK             	0
extern const am_hal_gpio_pincfg_t       g_AP3_PER_IOS_SCK;

//*****************************************************************************
//
//  IOS_SCL pin: I/O Slave I2C clock signal.
//
//*****************************************************************************
#define AP3_PER_IOS_SCL             	0
extern const am_hal_gpio_pincfg_t       g_AP3_PER_IOS_SCL;

//*****************************************************************************
//
//  IOS_SDA pin: I/O Slave I2C data signal.
//
//*****************************************************************************
#define AP3_PER_IOS_SDA             	1
extern const am_hal_gpio_pincfg_t       g_AP3_PER_IOS_SDA;

//*****************************************************************************
//
//  NCE_0 pin: NCE Pin.
//
//*****************************************************************************
#define AP3_PER_NCE_0               	0
extern const am_hal_gpio_pincfg_t       g_AP3_PER_NCE_0;

//*****************************************************************************
//
//  NCE_1 pin: NCE Pin.
//
//*****************************************************************************
#define AP3_PER_NCE_1               	1
extern const am_hal_gpio_pincfg_t       g_AP3_PER_NCE_1;

//*****************************************************************************
//
//  NCE_2 pin: NCE Pin.
//
//*****************************************************************************
#define AP3_PER_NCE_2               	2
extern const am_hal_gpio_pincfg_t       g_AP3_PER_NCE_2;

//*****************************************************************************
//
//  NCE_3 pin: NCE Pin.
//
//*****************************************************************************
#define AP3_PER_NCE_3               	3
extern const am_hal_gpio_pincfg_t       g_AP3_PER_NCE_3;

//*****************************************************************************
//
//  NCE_4 pin: NCE Pin.
//
//*****************************************************************************
#define AP3_PER_NCE_4               	4
extern const am_hal_gpio_pincfg_t       g_AP3_PER_NCE_4;

//*****************************************************************************
//
//  NCE_7 pin: NCE Pin.
//
//*****************************************************************************
#define AP3_PER_NCE_7               	7
extern const am_hal_gpio_pincfg_t       g_AP3_PER_NCE_7;

//*****************************************************************************
//
//  NCE_8 pin: NCE Pin.
//
//*****************************************************************************
#define AP3_PER_NCE_8               	8
extern const am_hal_gpio_pincfg_t       g_AP3_PER_NCE_8;

//*****************************************************************************
//
//  NCE_9 pin: NCE Pin.
//
//*****************************************************************************
#define AP3_PER_NCE_9               	9
extern const am_hal_gpio_pincfg_t       g_AP3_PER_NCE_9;

//*****************************************************************************
//
//  NCE_10 pin: NCE Pin.
//
//*****************************************************************************
#define AP3_PER_NCE_10              	10
extern const am_hal_gpio_pincfg_t       g_AP3_PER_NCE_10;

//*****************************************************************************
//
//  NCE_11 pin: NCE Pin.
//
//*****************************************************************************
#define AP3_PER_NCE_11              	11
extern const am_hal_gpio_pincfg_t       g_AP3_PER_NCE_11;

//*****************************************************************************
//
//  NCE_12 pin: NCE Pin.
//
//*****************************************************************************
#define AP3_PER_NCE_12              	12
extern const am_hal_gpio_pincfg_t       g_AP3_PER_NCE_12;

//*****************************************************************************
//
//  NCE_13 pin: NCE Pin.
//
//*****************************************************************************
#define AP3_PER_NCE_13              	13
extern const am_hal_gpio_pincfg_t       g_AP3_PER_NCE_13;

//*****************************************************************************
//
//  NCE_14 pin: NCE Pin.
//
//*****************************************************************************
#define AP3_PER_NCE_14              	14
extern const am_hal_gpio_pincfg_t       g_AP3_PER_NCE_14;

//*****************************************************************************
//
//  NCE_15 pin: NCE Pin.
//
//*****************************************************************************
#define AP3_PER_NCE_15              	15
extern const am_hal_gpio_pincfg_t       g_AP3_PER_NCE_15;

//*****************************************************************************
//
//  NCE_16 pin: NCE Pin.
//
//*****************************************************************************
#define AP3_PER_NCE_16              	16
extern const am_hal_gpio_pincfg_t       g_AP3_PER_NCE_16;

//*****************************************************************************
//
//  NCE_17 pin: NCE Pin.
//
//*****************************************************************************
#define AP3_PER_NCE_17              	17
extern const am_hal_gpio_pincfg_t       g_AP3_PER_NCE_17;

//*****************************************************************************
//
//  NCE_18 pin: NCE Pin.
//
//*****************************************************************************
#define AP3_PER_NCE_18              	18
extern const am_hal_gpio_pincfg_t       g_AP3_PER_NCE_18;

//*****************************************************************************
//
//  NCE_19 pin: NCE Pin.
//
//*****************************************************************************
#define AP3_PER_NCE_19              	19
extern const am_hal_gpio_pincfg_t       g_AP3_PER_NCE_19;

//*****************************************************************************
//
//  NCE_20 pin: NCE Pin.
//
//*****************************************************************************
#define AP3_PER_NCE_20              	20
extern const am_hal_gpio_pincfg_t       g_AP3_PER_NCE_20;

//*****************************************************************************
//
//  NCE_21 pin: NCE Pin.
//
//*****************************************************************************
#define AP3_PER_NCE_21              	21
extern const am_hal_gpio_pincfg_t       g_AP3_PER_NCE_21;

//*****************************************************************************
//
//  NCE_22 pin: NCE Pin.
//
//*****************************************************************************
#define AP3_PER_NCE_22              	22
extern const am_hal_gpio_pincfg_t       g_AP3_PER_NCE_22;

//*****************************************************************************
//
//  NCE_23 pin: NCE Pin.
//
//*****************************************************************************
#define AP3_PER_NCE_23              	23
extern const am_hal_gpio_pincfg_t       g_AP3_PER_NCE_23;

//*****************************************************************************
//
//  NCE_24 pin: NCE Pin.
//
//*****************************************************************************
#define AP3_PER_NCE_24              	24
extern const am_hal_gpio_pincfg_t       g_AP3_PER_NCE_24;

//*****************************************************************************
//
//  NCE_25 pin: NCE Pin.
//
//*****************************************************************************
#define AP3_PER_NCE_25              	25
extern const am_hal_gpio_pincfg_t       g_AP3_PER_NCE_25;

//*****************************************************************************
//
//  NCE_26 pin: NCE Pin.
//
//*****************************************************************************
#define AP3_PER_NCE_26              	26
extern const am_hal_gpio_pincfg_t       g_AP3_PER_NCE_26;

//*****************************************************************************
//
//  NCE_27 pin: NCE Pin.
//
//*****************************************************************************
#define AP3_PER_NCE_27              	27
extern const am_hal_gpio_pincfg_t       g_AP3_PER_NCE_27;

//*****************************************************************************
//
//  NCE_28 pin: NCE Pin.
//
//*****************************************************************************
#define AP3_PER_NCE_28              	28
extern const am_hal_gpio_pincfg_t       g_AP3_PER_NCE_28;

//*****************************************************************************
//
//  NCE_29 pin: NCE Pin.
//
//*****************************************************************************
#define AP3_PER_NCE_29              	29
extern const am_hal_gpio_pincfg_t       g_AP3_PER_NCE_29;

//*****************************************************************************
//
//  NCE_30 pin: NCE Pin.
//
//*****************************************************************************
#if defined (AM_PACKAGE_BGA)
#define AP3_PER_NCE_30              	30
extern const am_hal_gpio_pincfg_t       g_AP3_PER_NCE_30;
#endif // AM_PACKAGE_BGA

//*****************************************************************************
//
//  NCE_31 pin: NCE Pin.
//
//*****************************************************************************
#if defined (AM_PACKAGE_BGA)
#define AP3_PER_NCE_31              	31
extern const am_hal_gpio_pincfg_t       g_AP3_PER_NCE_31;
#endif // AM_PACKAGE_BGA

//*****************************************************************************
//
//  NCE_32 pin: NCE Pin.
//
//*****************************************************************************
#if defined (AM_PACKAGE_BGA)
#define AP3_PER_NCE_32              	32
extern const am_hal_gpio_pincfg_t       g_AP3_PER_NCE_32;
#endif // AM_PACKAGE_BGA

//*****************************************************************************
//
//  NCE_33 pin: NCE Pin.
//
//*****************************************************************************
#if defined (AM_PACKAGE_BGA)
#define AP3_PER_NCE_33              	33
extern const am_hal_gpio_pincfg_t       g_AP3_PER_NCE_33;
#endif // AM_PACKAGE_BGA

//*****************************************************************************
//
//  NCE_34 pin: NCE Pin.
//
//*****************************************************************************
#if defined (AM_PACKAGE_BGA)
#define AP3_PER_NCE_34              	34
extern const am_hal_gpio_pincfg_t       g_AP3_PER_NCE_34;
#endif // AM_PACKAGE_BGA

//*****************************************************************************
//
//  NCE_35 pin: NCE Pin.
//
//*****************************************************************************
#if defined (AM_PACKAGE_BGA)
#define AP3_PER_NCE_35              	35
extern const am_hal_gpio_pincfg_t       g_AP3_PER_NCE_35;
#endif // AM_PACKAGE_BGA

//*****************************************************************************
//
//  NCE_36 pin: NCE Pin.
//
//*****************************************************************************
#if defined (AM_PACKAGE_BGA)
#define AP3_PER_NCE_36              	36
extern const am_hal_gpio_pincfg_t       g_AP3_PER_NCE_36;
#endif // AM_PACKAGE_BGA

//*****************************************************************************
//
//  NCE_37 pin: NCE Pin.
//
//*****************************************************************************
#if defined (AM_PACKAGE_BGA)
#define AP3_PER_NCE_37              	37
extern const am_hal_gpio_pincfg_t       g_AP3_PER_NCE_37;
#endif // AM_PACKAGE_BGA

//*****************************************************************************
//
//  NCE_38 pin: NCE Pin.
//
//*****************************************************************************
#if defined (AM_PACKAGE_BGA)
#define AP3_PER_NCE_38              	38
extern const am_hal_gpio_pincfg_t       g_AP3_PER_NCE_38;
#endif // AM_PACKAGE_BGA

//*****************************************************************************
//
//  NCE_41 pin: NCE Pin.
//
//*****************************************************************************
#define AP3_PER_NCE_41              	41
extern const am_hal_gpio_pincfg_t       g_AP3_PER_NCE_41;

//*****************************************************************************
//
//  NCE_42 pin: NCE Pin.
//
//*****************************************************************************
#if defined (AM_PACKAGE_BGA)
#define AP3_PER_NCE_42              	42
extern const am_hal_gpio_pincfg_t       g_AP3_PER_NCE_42;
#endif // AM_PACKAGE_BGA

//*****************************************************************************
//
//  NCE_43 pin: NCE Pin.
//
//*****************************************************************************
#if defined (AM_PACKAGE_BGA)
#define AP3_PER_NCE_43              	43
extern const am_hal_gpio_pincfg_t       g_AP3_PER_NCE_43;
#endif // AM_PACKAGE_BGA

//*****************************************************************************
//
//  NCE_44 pin: NCE Pin.
//
//*****************************************************************************
#define AP3_PER_NCE_44              	44
extern const am_hal_gpio_pincfg_t       g_AP3_PER_NCE_44;

//*****************************************************************************
//
//  NCE_45 pin: NCE Pin.
//
//*****************************************************************************
#if defined (AM_PACKAGE_BGA)
#define AP3_PER_NCE_45              	45
extern const am_hal_gpio_pincfg_t       g_AP3_PER_NCE_45;
#endif // AM_PACKAGE_BGA

//*****************************************************************************
//
//  NCE_46 pin: NCE Pin.
//
//*****************************************************************************
#if defined (AM_PACKAGE_BGA)
#define AP3_PER_NCE_46              	46
extern const am_hal_gpio_pincfg_t       g_AP3_PER_NCE_46;
#endif // AM_PACKAGE_BGA

//*****************************************************************************
//
//  NCE_47 pin: NCE Pin.
//
//*****************************************************************************
#define AP3_PER_NCE_47              	47
extern const am_hal_gpio_pincfg_t       g_AP3_PER_NCE_47;

//*****************************************************************************
//
//  NCE_48 pin: NCE Pin.
//
//*****************************************************************************
#define AP3_PER_NCE_48              	48
extern const am_hal_gpio_pincfg_t       g_AP3_PER_NCE_48;

//*****************************************************************************
//
//  NCE_49 pin: NCE Pin.
//
//*****************************************************************************
#define AP3_PER_NCE_49              	49
extern const am_hal_gpio_pincfg_t       g_AP3_PER_NCE_49;

//*****************************************************************************
//
//  PDM_DATA_11 pin: Data line for PDM microphones.
//
//*****************************************************************************
#define AP3_PER_PDM_DATA_11         	11
extern const am_hal_gpio_pincfg_t       g_AP3_PER_PDM_DATA_11;

//*****************************************************************************
//
//  PDM_DATA_15 pin: Data line for PDM microphones.
//
//*****************************************************************************
#define AP3_PER_PDM_DATA_15         	15
extern const am_hal_gpio_pincfg_t       g_AP3_PER_PDM_DATA_15;

//*****************************************************************************
//
//  PDM_DATA_29 pin: Data line for PDM microphones.
//
//*****************************************************************************
#define AP3_PER_PDM_DATA_29         	29
extern const am_hal_gpio_pincfg_t       g_AP3_PER_PDM_DATA_29;

//*****************************************************************************
//
//  PDM_DATA_34 pin: Data line for PDM microphones.
//
//*****************************************************************************
#if defined (AM_PACKAGE_BGA)
#define AP3_PER_PDM_DATA_34         	34
extern const am_hal_gpio_pincfg_t       g_AP3_PER_PDM_DATA_34;
#endif // AM_PACKAGE_BGA

//*****************************************************************************
//
//  PDM_DATA_36 pin: Data line for PDM microphones.
//
//*****************************************************************************
#if defined (AM_PACKAGE_BGA)
#define AP3_PER_PDM_DATA_36         	36
extern const am_hal_gpio_pincfg_t       g_AP3_PER_PDM_DATA_36;
#endif // AM_PACKAGE_BGA

//*****************************************************************************
//
//  PDM_DATA_45 pin: Data line for PDM microphones.
//
//*****************************************************************************
#if defined (AM_PACKAGE_BGA)
#define AP3_PER_PDM_DATA_45         	45
extern const am_hal_gpio_pincfg_t       g_AP3_PER_PDM_DATA_45;
#endif // AM_PACKAGE_BGA

//*****************************************************************************
//
//  PDM_CLK_10 pin: Clock line for PDM microphones.
//
//*****************************************************************************
#define AP3_PER_PDM_CLK_10          	10
extern const am_hal_gpio_pincfg_t       g_AP3_PER_PDM_CLK_10;

//*****************************************************************************
//
//  PDM_CLK_12 pin: Clock line for PDM microphones.
//
//*****************************************************************************
#define AP3_PER_PDM_CLK_12          	12
extern const am_hal_gpio_pincfg_t       g_AP3_PER_PDM_CLK_12;

//*****************************************************************************
//
//  PDM_CLK_14 pin: Clock line for PDM microphones.
//
//*****************************************************************************
#define AP3_PER_PDM_CLK_14          	14
extern const am_hal_gpio_pincfg_t       g_AP3_PER_PDM_CLK_14;

//*****************************************************************************
//
//  PDM_CLK_22 pin: Clock line for PDM microphones.
//
//*****************************************************************************
#define AP3_PER_PDM_CLK_22          	22
extern const am_hal_gpio_pincfg_t       g_AP3_PER_PDM_CLK_22;

//*****************************************************************************
//
//  PDM_CLK_37 pin: Clock line for PDM microphones.
//
//*****************************************************************************
#if defined (AM_PACKAGE_BGA)
#define AP3_PER_PDM_CLK_37          	37
extern const am_hal_gpio_pincfg_t       g_AP3_PER_PDM_CLK_37;
#endif // AM_PACKAGE_BGA

//*****************************************************************************
//
//  PDM_CLK_46 pin: Clock line for PDM microphones.
//
//*****************************************************************************
#if defined (AM_PACKAGE_BGA)
#define AP3_PER_PDM_CLK_46          	46
extern const am_hal_gpio_pincfg_t       g_AP3_PER_PDM_CLK_46;
#endif // AM_PACKAGE_BGA

//*****************************************************************************
//
//  ITM_SWO_15 pin: ITM Serial Wire Output.
//
//*****************************************************************************
#define AP3_PER_ITM_SWO_15          	15
extern const am_hal_gpio_pincfg_t       g_AP3_PER_ITM_SWO_15;

//*****************************************************************************
//
//  ITM_SWO pin: ITM Serial Wire Output.
//
//*****************************************************************************
#define AP3_PER_ITM_SWO             	22
extern const am_hal_gpio_pincfg_t       g_AP3_PER_ITM_SWO;

//*****************************************************************************
//
//  ITM_SWO_24 pin: ITM Serial Wire Output.
//
//*****************************************************************************
#define AP3_PER_ITM_SWO_24          	24
extern const am_hal_gpio_pincfg_t       g_AP3_PER_ITM_SWO_24;

//*****************************************************************************
//
//  ITM_SWO_33 pin: ITM Serial Wire Output.
//
//*****************************************************************************
#if defined (AM_PACKAGE_BGA)
#define AP3_PER_ITM_SWO_33          	33
extern const am_hal_gpio_pincfg_t       g_AP3_PER_ITM_SWO_33;
#endif // AM_PACKAGE_BGA

//*****************************************************************************
//
//  ITM_SWO_41 pin: ITM Serial Wire Output.
//
//*****************************************************************************
#define AP3_PER_ITM_SWO_41          	41
extern const am_hal_gpio_pincfg_t       g_AP3_PER_ITM_SWO_41;

//*****************************************************************************
//
//  ITM_SWO_45 pin: ITM Serial Wire Output.
//
//*****************************************************************************
#if defined (AM_PACKAGE_BGA)
#define AP3_PER_ITM_SWO_45          	45
extern const am_hal_gpio_pincfg_t       g_AP3_PER_ITM_SWO_45;
#endif // AM_PACKAGE_BGA

//*****************************************************************************
//
//  ITM_SWO_46 pin: ITM Serial Wire Output.
//
//*****************************************************************************
#if defined (AM_PACKAGE_BGA)
#define AP3_PER_ITM_SWO_46          	46
extern const am_hal_gpio_pincfg_t       g_AP3_PER_ITM_SWO_46;
#endif // AM_PACKAGE_BGA

//*****************************************************************************
//
//  CORE_SWDCK_14 pin: Cortex Serial Wire Debug Clock.
//
//*****************************************************************************
#define AP3_PER_CORE_SWDCK_14       	14
extern const am_hal_gpio_pincfg_t       g_AP3_PER_CORE_SWDCK_14;

//*****************************************************************************
//
//  CORE_SWDCK_20 pin: Cortex Serial Wire Debug Clock.
//
//*****************************************************************************
#define AP3_PER_CORE_SWDCK_20       	20
extern const am_hal_gpio_pincfg_t       g_AP3_PER_CORE_SWDCK_20;

//*****************************************************************************
//
//  CORE_SWDIO_15 pin: Cortex Serial Wire Debug I/O.
//
//*****************************************************************************
#define AP3_PER_CORE_SWDIO_15       	15
extern const am_hal_gpio_pincfg_t       g_AP3_PER_CORE_SWDIO_15;

//*****************************************************************************
//
//  CORE_SWDIO_21 pin: Cortex Serial Wire Debug I/O.
//
//*****************************************************************************
#define AP3_PER_CORE_SWDIO_21       	21
extern const am_hal_gpio_pincfg_t       g_AP3_PER_CORE_SWDIO_21;


#ifdef __cplusplus
}
#endif

#endif // _APOLLO3_PERIPHERAL_PIN_CONFIGS_H_

//*****************************************************************************
//
// End Doxygen group.
//! @}
//
//*****************************************************************************
