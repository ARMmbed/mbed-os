//*****************************************************************************
//
//  am_bsp_pins.c
//! @file
//!
//! @brief BSP pin configuration definitions.
//!
//! @addtogroup BSP Board Support Package (BSP)
//! @addtogroup apollo3_evb_bsp BSP for the Apollo3 Engineering Board
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
// SPDX-License-Identifier: BSD-3-Clause

#include "am_bsp.h"

//*****************************************************************************
//
//  UART0_TX_1 pin: UART0 Tx Pin.
//
//*****************************************************************************
const am_hal_gpio_pincfg_t g_AP3_PER_UART0_TX_1 = {
    .uFuncSel            = AM_HAL_PIN_1_UART0TX,
    .eDriveStrength      = AM_HAL_GPIO_PIN_DRIVESTRENGTH_2MA
};

//*****************************************************************************
//
//  UART0_TX_7 pin: UART0 Tx Pin.
//
//*****************************************************************************
const am_hal_gpio_pincfg_t g_AP3_PER_UART0_TX_7 = {
    .uFuncSel            = AM_HAL_PIN_7_UART0TX,
    .eDriveStrength      = AM_HAL_GPIO_PIN_DRIVESTRENGTH_2MA
};

//*****************************************************************************
//
//  UART0_TX_16 pin: UART0 Tx Pin.
//
//*****************************************************************************
const am_hal_gpio_pincfg_t g_AP3_PER_UART0_TX_16 = {
    .uFuncSel            = AM_HAL_PIN_16_UART0TX,
    .eDriveStrength      = AM_HAL_GPIO_PIN_DRIVESTRENGTH_2MA
};

//*****************************************************************************
//
//  UART0_TX_20 pin: UART0 Tx Pin.
//
//*****************************************************************************
const am_hal_gpio_pincfg_t g_AP3_PER_UART0_TX_20 = {
    .uFuncSel            = AM_HAL_PIN_20_UART0TX,
    .eDriveStrength      = AM_HAL_GPIO_PIN_DRIVESTRENGTH_2MA
};

//*****************************************************************************
//
//  UART0_TX_22 pin: UART0 Tx Pin.
//
//*****************************************************************************
const am_hal_gpio_pincfg_t g_AP3_PER_UART0_TX_22 = {
    .uFuncSel            = AM_HAL_PIN_22_UART0TX,
    .eDriveStrength      = AM_HAL_GPIO_PIN_DRIVESTRENGTH_2MA
};

//*****************************************************************************
//
//  UART0_TX_26 pin: UART0 Tx Pin.
//
//*****************************************************************************
const am_hal_gpio_pincfg_t g_AP3_PER_UART0_TX_26 = {
    .uFuncSel            = AM_HAL_PIN_26_UART0TX,
    .eDriveStrength      = AM_HAL_GPIO_PIN_DRIVESTRENGTH_2MA
};

//*****************************************************************************
//
//  UART0_TX_28 pin: UART0 Tx Pin.
//
//*****************************************************************************
const am_hal_gpio_pincfg_t g_AP3_PER_UART0_TX_28 = {
    .uFuncSel            = AM_HAL_PIN_28_UART0TX,
    .eDriveStrength      = AM_HAL_GPIO_PIN_DRIVESTRENGTH_2MA
};

//*****************************************************************************
//
//  UART0_TX_30 pin: UART0 Tx Pin.
//
//*****************************************************************************
#if defined (AM_PACKAGE_BGA)
const am_hal_gpio_pincfg_t g_AP3_PER_UART0_TX_30 = {
    .uFuncSel            = AM_HAL_PIN_30_UART0TX,
    .eDriveStrength      = AM_HAL_GPIO_PIN_DRIVESTRENGTH_2MA
};
#endif // AM_PACKAGE_BGA

//*****************************************************************************
//
//  UART0_TX_39 pin: UART0 Tx Pin.
//
//*****************************************************************************
const am_hal_gpio_pincfg_t g_AP3_PER_UART0_TX_39 = {
    .uFuncSel            = AM_HAL_PIN_39_UART0TX,
    .eDriveStrength      = AM_HAL_GPIO_PIN_DRIVESTRENGTH_2MA
};

//*****************************************************************************
//
//  UART0_TX_41 pin: UART0 Tx Pin.
//
//*****************************************************************************
const am_hal_gpio_pincfg_t g_AP3_PER_UART0_TX_41 = {
    .uFuncSel            = AM_HAL_PIN_41_UART0TX,
    .eDriveStrength      = AM_HAL_GPIO_PIN_DRIVESTRENGTH_2MA
};

//*****************************************************************************
//
//  UART0_TX_44 pin: UART0 Tx Pin.
//
//*****************************************************************************
const am_hal_gpio_pincfg_t g_AP3_PER_UART0_TX_44 = {
    .uFuncSel            = AM_HAL_PIN_44_UART0TX,
    .eDriveStrength      = AM_HAL_GPIO_PIN_DRIVESTRENGTH_2MA
};

//*****************************************************************************
//
//  UART0_TX_48 pin: UART0 Tx Pin.
//
//*****************************************************************************
const am_hal_gpio_pincfg_t g_AP3_PER_UART0_TX_48 = {
    .uFuncSel            = AM_HAL_PIN_48_UART0TX,
    .eDriveStrength      = AM_HAL_GPIO_PIN_DRIVESTRENGTH_2MA
};

//*****************************************************************************
//
//  UART1_TX_8 pin: UART1 Tx Pin.
//
//*****************************************************************************
const am_hal_gpio_pincfg_t g_AP3_PER_UART1_TX_8 = {
    .uFuncSel            = AM_HAL_PIN_8_UART1TX,
    .eDriveStrength      = AM_HAL_GPIO_PIN_DRIVESTRENGTH_2MA
};

//*****************************************************************************
//
//  UART1_TX_10 pin: UART1 Tx Pin.
//
//*****************************************************************************
const am_hal_gpio_pincfg_t g_AP3_PER_UART1_TX_10 = {
    .uFuncSel            = AM_HAL_PIN_10_UART1TX,
    .eDriveStrength      = AM_HAL_GPIO_PIN_DRIVESTRENGTH_2MA
};

//*****************************************************************************
//
//  UART1_TX_12 pin: UART1 Tx Pin.
//
//*****************************************************************************
const am_hal_gpio_pincfg_t g_AP3_PER_UART1_TX_12 = {
    .uFuncSel            = AM_HAL_PIN_12_UART1TX,
    .eDriveStrength      = AM_HAL_GPIO_PIN_DRIVESTRENGTH_2MA
};

//*****************************************************************************
//
//  UART1_TX_14 pin: UART1 Tx Pin.
//
//*****************************************************************************
const am_hal_gpio_pincfg_t g_AP3_PER_UART1_TX_14 = {
    .uFuncSel            = AM_HAL_PIN_14_UART1TX,
    .eDriveStrength      = AM_HAL_GPIO_PIN_DRIVESTRENGTH_2MA
};

//*****************************************************************************
//
//  UART1_TX_18 pin: UART1 Tx Pin.
//
//*****************************************************************************
const am_hal_gpio_pincfg_t g_AP3_PER_UART1_TX_18 = {
    .uFuncSel            = AM_HAL_PIN_18_UART1TX,
    .eDriveStrength      = AM_HAL_GPIO_PIN_DRIVESTRENGTH_2MA
};

//*****************************************************************************
//
//  UART1_TX_20 pin: UART1 Tx Pin.
//
//*****************************************************************************
const am_hal_gpio_pincfg_t g_AP3_PER_UART1_TX_20 = {
    .uFuncSel            = AM_HAL_PIN_20_UART1TX,
    .eDriveStrength      = AM_HAL_GPIO_PIN_DRIVESTRENGTH_2MA
};

//*****************************************************************************
//
//  UART1_TX_24 pin: UART1 Tx Pin.
//
//*****************************************************************************
const am_hal_gpio_pincfg_t g_AP3_PER_UART1_TX_24 = {
    .uFuncSel            = AM_HAL_PIN_24_UART1TX,
    .eDriveStrength      = AM_HAL_GPIO_PIN_DRIVESTRENGTH_2MA
};

//*****************************************************************************
//
//  UART1_TX_35 pin: UART1 Tx Pin.
//
//*****************************************************************************
#if defined (AM_PACKAGE_BGA)
const am_hal_gpio_pincfg_t g_AP3_PER_UART1_TX_35 = {
    .uFuncSel            = AM_HAL_PIN_35_UART1TX,
    .eDriveStrength      = AM_HAL_GPIO_PIN_DRIVESTRENGTH_2MA
};
#endif // AM_PACKAGE_BGA

//*****************************************************************************
//
//  UART1_TX_37 pin: UART1 Tx Pin.
//
//*****************************************************************************
#if defined (AM_PACKAGE_BGA)
const am_hal_gpio_pincfg_t g_AP3_PER_UART1_TX_37 = {
    .uFuncSel            = AM_HAL_PIN_37_UART1TX,
    .eDriveStrength      = AM_HAL_GPIO_PIN_DRIVESTRENGTH_2MA
};
#endif // AM_PACKAGE_BGA

//*****************************************************************************
//
//  UART1_TX_39 pin: UART1 Tx Pin.
//
//*****************************************************************************
const am_hal_gpio_pincfg_t g_AP3_PER_UART1_TX_39 = {
    .uFuncSel            = AM_HAL_PIN_39_UART1TX,
    .eDriveStrength      = AM_HAL_GPIO_PIN_DRIVESTRENGTH_2MA
};

//*****************************************************************************
//
//  UART1_TX_42 pin: UART1 Tx Pin.
//
//*****************************************************************************
#if defined (AM_PACKAGE_BGA)
const am_hal_gpio_pincfg_t g_AP3_PER_UART1_TX_42 = {
    .uFuncSel            = AM_HAL_PIN_42_UART1TX,
    .eDriveStrength      = AM_HAL_GPIO_PIN_DRIVESTRENGTH_2MA
};
#endif // AM_PACKAGE_BGA

//*****************************************************************************
//
//  UART1_TX_46 pin: UART1 Tx Pin.
//
//*****************************************************************************
#if defined (AM_PACKAGE_BGA)
const am_hal_gpio_pincfg_t g_AP3_PER_UART1_TX_46 = {
    .uFuncSel            = AM_HAL_PIN_46_UART1TX,
    .eDriveStrength      = AM_HAL_GPIO_PIN_DRIVESTRENGTH_2MA
};
#endif // AM_PACKAGE_BGA

//*****************************************************************************
//
//  UART0_RX_2 pin: UART0 Rx Pin.
//
//*****************************************************************************
const am_hal_gpio_pincfg_t g_AP3_PER_UART0_RX_2 = {
    .uFuncSel            = AM_HAL_PIN_2_UART0RX
};

//*****************************************************************************
//
//  UART0_RX_11 pin: UART0 Rx Pin.
//
//*****************************************************************************
const am_hal_gpio_pincfg_t g_AP3_PER_UART0_RX_11 = {
    .uFuncSel            = AM_HAL_PIN_11_UART0RX
};

//*****************************************************************************
//
//  UART0_RX_17 pin: UART0 Rx Pin.
//
//*****************************************************************************
const am_hal_gpio_pincfg_t g_AP3_PER_UART0_RX_17 = {
    .uFuncSel            = AM_HAL_PIN_17_UART0RX
};

//*****************************************************************************
//
//  UART0_RX_21 pin: UART0 Rx Pin.
//
//*****************************************************************************
const am_hal_gpio_pincfg_t g_AP3_PER_UART0_RX_21 = {
    .uFuncSel            = AM_HAL_PIN_21_UART0RX
};

//*****************************************************************************
//
//  UART0_RX_23 pin: UART0 Rx Pin.
//
//*****************************************************************************
const am_hal_gpio_pincfg_t g_AP3_PER_UART0_RX_23 = {
    .uFuncSel            = AM_HAL_PIN_23_UART0RX
};

//*****************************************************************************
//
//  UART0_RX_27 pin: UART0 Rx Pin.
//
//*****************************************************************************
const am_hal_gpio_pincfg_t g_AP3_PER_UART0_RX_27 = {
    .uFuncSel            = AM_HAL_PIN_27_UART0RX
};

//*****************************************************************************
//
//  UART0_RX_29 pin: UART0 Rx Pin.
//
//*****************************************************************************
const am_hal_gpio_pincfg_t g_AP3_PER_UART0_RX_29 = {
    .uFuncSel            = AM_HAL_PIN_29_UART0RX
};

//*****************************************************************************
//
//  UART0_RX_31 pin: UART0 Rx Pin.
//
//*****************************************************************************
#if defined (AM_PACKAGE_BGA)
const am_hal_gpio_pincfg_t g_AP3_PER_UART0_RX_31 = {
    .uFuncSel            = AM_HAL_PIN_31_UART0RX
};
#endif // AM_PACKAGE_BGA

//*****************************************************************************
//
//  UART0_RX_34 pin: UART0 Rx Pin.
//
//*****************************************************************************
#if defined (AM_PACKAGE_BGA)
const am_hal_gpio_pincfg_t g_AP3_PER_UART0_RX_34 = {
    .uFuncSel            = AM_HAL_PIN_34_UART0RX
};
#endif // AM_PACKAGE_BGA

//*****************************************************************************
//
//  UART0_RX_40 pin: UART0 Rx Pin.
//
//*****************************************************************************
const am_hal_gpio_pincfg_t g_AP3_PER_UART0_RX_40 = {
    .uFuncSel            = AM_HAL_PIN_40_UART0RX
};

//*****************************************************************************
//
//  UART0_RX_45 pin: UART0 Rx Pin.
//
//*****************************************************************************
#if defined (AM_PACKAGE_BGA)
const am_hal_gpio_pincfg_t g_AP3_PER_UART0_RX_45 = {
    .uFuncSel            = AM_HAL_PIN_45_UART0RX
};
#endif // AM_PACKAGE_BGA

//*****************************************************************************
//
//  UART0_RX_49 pin: UART0 Rx Pin.
//
//*****************************************************************************
const am_hal_gpio_pincfg_t g_AP3_PER_UART0_RX_49 = {
    .uFuncSel            = AM_HAL_PIN_49_UART0RX
};

//*****************************************************************************
//
//  UART1_RX_2 pin: UART1 Rx Pin.
//
//*****************************************************************************
const am_hal_gpio_pincfg_t g_AP3_PER_UART1_RX_2 = {
    .uFuncSel            = AM_HAL_PIN_2_UART1RX
};

//*****************************************************************************
//
//  UART1_RX_4 pin: UART1 Rx Pin.
//
//*****************************************************************************
const am_hal_gpio_pincfg_t g_AP3_PER_UART1_RX_4 = {
    .uFuncSel            = AM_HAL_PIN_4_UART1RX
};

//*****************************************************************************
//
//  UART1_RX_9 pin: UART1 Rx Pin.
//
//*****************************************************************************
const am_hal_gpio_pincfg_t g_AP3_PER_UART1_RX_9 = {
    .uFuncSel            = AM_HAL_PIN_9_UART1RX
};

//*****************************************************************************
//
//  UART1_RX_13 pin: UART1 Rx Pin.
//
//*****************************************************************************
const am_hal_gpio_pincfg_t g_AP3_PER_UART1_RX_13 = {
    .uFuncSel            = AM_HAL_PIN_13_UART1RX
};

//*****************************************************************************
//
//  UART1_RX_15 pin: UART1 Rx Pin.
//
//*****************************************************************************
const am_hal_gpio_pincfg_t g_AP3_PER_UART1_RX_15 = {
    .uFuncSel            = AM_HAL_PIN_15_UART1RX
};

//*****************************************************************************
//
//  UART1_RX_19 pin: UART1 Rx Pin.
//
//*****************************************************************************
const am_hal_gpio_pincfg_t g_AP3_PER_UART1_RX_19 = {
    .uFuncSel            = AM_HAL_PIN_19_UART1RX
};

//*****************************************************************************
//
//  UART1_RX_21 pin: UART1 Rx Pin.
//
//*****************************************************************************
const am_hal_gpio_pincfg_t g_AP3_PER_UART1_RX_21 = {
    .uFuncSel            = AM_HAL_PIN_21_UART1RX
};

//*****************************************************************************
//
//  UART1_RX_25 pin: UART1 Rx Pin.
//
//*****************************************************************************
const am_hal_gpio_pincfg_t g_AP3_PER_UART1_RX_25 = {
    .uFuncSel            = AM_HAL_PIN_25_UART1RX
};

//*****************************************************************************
//
//  UART1_RX_36 pin: UART1 Rx Pin.
//
//*****************************************************************************
#if defined (AM_PACKAGE_BGA)
const am_hal_gpio_pincfg_t g_AP3_PER_UART1_RX_36 = {
    .uFuncSel            = AM_HAL_PIN_36_UART1RX
};
#endif // AM_PACKAGE_BGA

//*****************************************************************************
//
//  UART1_RX_38 pin: UART1 Rx Pin.
//
//*****************************************************************************
#if defined (AM_PACKAGE_BGA)
const am_hal_gpio_pincfg_t g_AP3_PER_UART1_RX_38 = {
    .uFuncSel            = AM_HAL_PIN_38_UART1RX
};
#endif // AM_PACKAGE_BGA

//*****************************************************************************
//
//  UART1_RX_40 pin: UART1 Rx Pin.
//
//*****************************************************************************
const am_hal_gpio_pincfg_t g_AP3_PER_UART1_RX_40 = {
    .uFuncSel            = AM_HAL_PIN_40_UART1RX
};

//*****************************************************************************
//
//  UART1_RX_43 pin: UART1 Rx Pin.
//
//*****************************************************************************
#if defined (AM_PACKAGE_BGA)
const am_hal_gpio_pincfg_t g_AP3_PER_UART1_RX_43 = {
    .uFuncSel            = AM_HAL_PIN_43_UART1RX
};
#endif // AM_PACKAGE_BGA

//*****************************************************************************
//
//  UART1_RX_47 pin: UART1 Rx Pin.
//
//*****************************************************************************
const am_hal_gpio_pincfg_t g_AP3_PER_UART1_RX_47 = {
    .uFuncSel            = AM_HAL_PIN_47_UART1RX
};

//*****************************************************************************
//
//  UART0_RTS_3 pin: UART0 RTS Pin.
//
//*****************************************************************************
const am_hal_gpio_pincfg_t g_AP3_PER_UART0_RTS_3 = {
    .uFuncSel            = AM_HAL_PIN_3_UART0RTS,
    .eDriveStrength      = AM_HAL_GPIO_PIN_DRIVESTRENGTH_2MA
};

//*****************************************************************************
//
//  UART0_RTS_5 pin: UART0 RTS Pin.
//
//*****************************************************************************
const am_hal_gpio_pincfg_t g_AP3_PER_UART0_RTS_5 = {
    .uFuncSel            = AM_HAL_PIN_5_UART0RTS,
    .eDriveStrength      = AM_HAL_GPIO_PIN_DRIVESTRENGTH_2MA
};

//*****************************************************************************
//
//  UART0_RTS_13 pin: UART0 RTS Pin.
//
//*****************************************************************************
const am_hal_gpio_pincfg_t g_AP3_PER_UART0_RTS_13 = {
    .uFuncSel            = AM_HAL_PIN_13_UART0RTS,
    .eDriveStrength      = AM_HAL_GPIO_PIN_DRIVESTRENGTH_2MA
};

//*****************************************************************************
//
//  UART0_RTS_18 pin: UART0 RTS Pin.
//
//*****************************************************************************
const am_hal_gpio_pincfg_t g_AP3_PER_UART0_RTS_18 = {
    .uFuncSel            = AM_HAL_PIN_18_UART0RTS,
    .eDriveStrength      = AM_HAL_GPIO_PIN_DRIVESTRENGTH_2MA
};

//*****************************************************************************
//
//  UART0_RTS_34 pin: UART0 RTS Pin.
//
//*****************************************************************************
#if defined (AM_PACKAGE_BGA)
const am_hal_gpio_pincfg_t g_AP3_PER_UART0_RTS_34 = {
    .uFuncSel            = AM_HAL_PIN_34_UART0RTS,
    .eDriveStrength      = AM_HAL_GPIO_PIN_DRIVESTRENGTH_2MA
};
#endif // AM_PACKAGE_BGA

//*****************************************************************************
//
//  UART0_RTS_35 pin: UART0 RTS Pin.
//
//*****************************************************************************
#if defined (AM_PACKAGE_BGA)
const am_hal_gpio_pincfg_t g_AP3_PER_UART0_RTS_35 = {
    .uFuncSel            = AM_HAL_PIN_35_UART0RTS,
    .eDriveStrength      = AM_HAL_GPIO_PIN_DRIVESTRENGTH_2MA
};
#endif // AM_PACKAGE_BGA

//*****************************************************************************
//
//  UART0_RTS_37 pin: UART0 RTS Pin.
//
//*****************************************************************************
#if defined (AM_PACKAGE_BGA)
const am_hal_gpio_pincfg_t g_AP3_PER_UART0_RTS_37 = {
    .uFuncSel            = AM_HAL_PIN_37_UART0RTS,
    .eDriveStrength      = AM_HAL_GPIO_PIN_DRIVESTRENGTH_2MA
};
#endif // AM_PACKAGE_BGA

//*****************************************************************************
//
//  UART0_RTS_41 pin: UART0 RTS Pin.
//
//*****************************************************************************
const am_hal_gpio_pincfg_t g_AP3_PER_UART0_RTS_41 = {
    .uFuncSel            = AM_HAL_PIN_41_UART0RTS,
    .eDriveStrength      = AM_HAL_GPIO_PIN_DRIVESTRENGTH_2MA
};

//*****************************************************************************
//
//  UART1_RTS_10 pin: UART1 RTS Pin.
//
//*****************************************************************************
const am_hal_gpio_pincfg_t g_AP3_PER_UART1_RTS_10 = {
    .uFuncSel            = AM_HAL_PIN_10_UART1RTS,
    .eDriveStrength      = AM_HAL_GPIO_PIN_DRIVESTRENGTH_2MA
};

//*****************************************************************************
//
//  UART1_RTS_16 pin: UART1 RTS Pin.
//
//*****************************************************************************
const am_hal_gpio_pincfg_t g_AP3_PER_UART1_RTS_16 = {
    .uFuncSel            = AM_HAL_PIN_16_UART1RTS,
    .eDriveStrength      = AM_HAL_GPIO_PIN_DRIVESTRENGTH_2MA
};

//*****************************************************************************
//
//  UART1_RTS_20 pin: UART1 RTS Pin.
//
//*****************************************************************************
const am_hal_gpio_pincfg_t g_AP3_PER_UART1_RTS_20 = {
    .uFuncSel            = AM_HAL_PIN_20_UART1RTS,
    .eDriveStrength      = AM_HAL_GPIO_PIN_DRIVESTRENGTH_2MA
};

//*****************************************************************************
//
//  UART1_RTS_30 pin: UART1 RTS Pin.
//
//*****************************************************************************
#if defined (AM_PACKAGE_BGA)
const am_hal_gpio_pincfg_t g_AP3_PER_UART1_RTS_30 = {
    .uFuncSel            = AM_HAL_PIN_30_UART1RTS,
    .eDriveStrength      = AM_HAL_GPIO_PIN_DRIVESTRENGTH_2MA
};
#endif // AM_PACKAGE_BGA

//*****************************************************************************
//
//  UART1_RTS_31 pin: UART1 RTS Pin.
//
//*****************************************************************************
#if defined (AM_PACKAGE_BGA)
const am_hal_gpio_pincfg_t g_AP3_PER_UART1_RTS_31 = {
    .uFuncSel            = AM_HAL_PIN_31_UART1RTS,
    .eDriveStrength      = AM_HAL_GPIO_PIN_DRIVESTRENGTH_2MA
};
#endif // AM_PACKAGE_BGA

//*****************************************************************************
//
//  UART1_RTS_34 pin: UART1 RTS Pin.
//
//*****************************************************************************
#if defined (AM_PACKAGE_BGA)
const am_hal_gpio_pincfg_t g_AP3_PER_UART1_RTS_34 = {
    .uFuncSel            = AM_HAL_PIN_34_UART1RTS,
    .eDriveStrength      = AM_HAL_GPIO_PIN_DRIVESTRENGTH_2MA
};
#endif // AM_PACKAGE_BGA

//*****************************************************************************
//
//  UART1_RTS_41 pin: UART1 RTS Pin.
//
//*****************************************************************************
const am_hal_gpio_pincfg_t g_AP3_PER_UART1_RTS_41 = {
    .uFuncSel            = AM_HAL_PIN_41_UART1RTS,
    .eDriveStrength      = AM_HAL_GPIO_PIN_DRIVESTRENGTH_2MA
};

//*****************************************************************************
//
//  UART1_RTS_44 pin: UART1 RTS Pin.
//
//*****************************************************************************
const am_hal_gpio_pincfg_t g_AP3_PER_UART1_RTS_44 = {
    .uFuncSel            = AM_HAL_PIN_44_UART1RTS,
    .eDriveStrength      = AM_HAL_GPIO_PIN_DRIVESTRENGTH_2MA
};

//*****************************************************************************
//
//  UART0_CTS_4 pin: UART0 CTS Pin.
//
//*****************************************************************************
const am_hal_gpio_pincfg_t g_AP3_PER_UART0_CTS_4 = {
    .uFuncSel            = AM_HAL_PIN_4_UART0CTS
};

//*****************************************************************************
//
//  UART0_CTS_6 pin: UART0 CTS Pin.
//
//*****************************************************************************
const am_hal_gpio_pincfg_t g_AP3_PER_UART0_CTS_6 = {
    .uFuncSel            = AM_HAL_PIN_6_UART0CTS
};

//*****************************************************************************
//
//  UART0_CTS_12 pin: UART0 CTS Pin.
//
//*****************************************************************************
const am_hal_gpio_pincfg_t g_AP3_PER_UART0_CTS_12 = {
    .uFuncSel            = AM_HAL_PIN_12_UART0CTS
};

//*****************************************************************************
//
//  UART0_CTS_24 pin: UART0 CTS Pin.
//
//*****************************************************************************
const am_hal_gpio_pincfg_t g_AP3_PER_UART0_CTS_24 = {
    .uFuncSel            = AM_HAL_PIN_24_UART0CTS
};

//*****************************************************************************
//
//  UART0_CTS_29 pin: UART0 CTS Pin.
//
//*****************************************************************************
const am_hal_gpio_pincfg_t g_AP3_PER_UART0_CTS_29 = {
    .uFuncSel            = AM_HAL_PIN_29_UART0CTS
};

//*****************************************************************************
//
//  UART0_CTS_33 pin: UART0 CTS Pin.
//
//*****************************************************************************
#if defined (AM_PACKAGE_BGA)
const am_hal_gpio_pincfg_t g_AP3_PER_UART0_CTS_33 = {
    .uFuncSel            = AM_HAL_PIN_33_UART0CTS
};
#endif // AM_PACKAGE_BGA

//*****************************************************************************
//
//  UART0_CTS_36 pin: UART0 CTS Pin.
//
//*****************************************************************************
#if defined (AM_PACKAGE_BGA)
const am_hal_gpio_pincfg_t g_AP3_PER_UART0_CTS_36 = {
    .uFuncSel            = AM_HAL_PIN_36_UART0CTS
};
#endif // AM_PACKAGE_BGA

//*****************************************************************************
//
//  UART0_CTS_38 pin: UART0 CTS Pin.
//
//*****************************************************************************
#if defined (AM_PACKAGE_BGA)
const am_hal_gpio_pincfg_t g_AP3_PER_UART0_CTS_38 = {
    .uFuncSel            = AM_HAL_PIN_38_UART0CTS
};
#endif // AM_PACKAGE_BGA

//*****************************************************************************
//
//  UART1_CTS_11 pin: UART1 CTS Pin.
//
//*****************************************************************************
const am_hal_gpio_pincfg_t g_AP3_PER_UART1_CTS_11 = {
    .uFuncSel            = AM_HAL_PIN_11_UART1CTS
};

//*****************************************************************************
//
//  UART1_CTS_17 pin: UART1 CTS Pin.
//
//*****************************************************************************
const am_hal_gpio_pincfg_t g_AP3_PER_UART1_CTS_17 = {
    .uFuncSel            = AM_HAL_PIN_17_UART1CTS
};

//*****************************************************************************
//
//  UART1_CTS_21 pin: UART1 CTS Pin.
//
//*****************************************************************************
const am_hal_gpio_pincfg_t g_AP3_PER_UART1_CTS_21 = {
    .uFuncSel            = AM_HAL_PIN_21_UART1CTS
};

//*****************************************************************************
//
//  UART1_CTS_26 pin: UART1 CTS Pin.
//
//*****************************************************************************
const am_hal_gpio_pincfg_t g_AP3_PER_UART1_CTS_26 = {
    .uFuncSel            = AM_HAL_PIN_26_UART1CTS
};

//*****************************************************************************
//
//  UART1_CTS_29 pin: UART1 CTS Pin.
//
//*****************************************************************************
const am_hal_gpio_pincfg_t g_AP3_PER_UART1_CTS_29 = {
    .uFuncSel            = AM_HAL_PIN_29_UART1CTS
};

//*****************************************************************************
//
//  UART1_CTS_32 pin: UART1 CTS Pin.
//
//*****************************************************************************
#if defined (AM_PACKAGE_BGA)
const am_hal_gpio_pincfg_t g_AP3_PER_UART1_CTS_32 = {
    .uFuncSel            = AM_HAL_PIN_32_UART1CTS
};
#endif // AM_PACKAGE_BGA

//*****************************************************************************
//
//  UART1_CTS_36 pin: UART1 CTS Pin.
//
//*****************************************************************************
#if defined (AM_PACKAGE_BGA)
const am_hal_gpio_pincfg_t g_AP3_PER_UART1_CTS_36 = {
    .uFuncSel            = AM_HAL_PIN_36_UART1CTS
};
#endif // AM_PACKAGE_BGA

//*****************************************************************************
//
//  UART1_CTS_45 pin: UART1 CTS Pin.
//
//*****************************************************************************
#if defined (AM_PACKAGE_BGA)
const am_hal_gpio_pincfg_t g_AP3_PER_UART1_CTS_45 = {
    .uFuncSel            = AM_HAL_PIN_45_UART1CTS
};
#endif // AM_PACKAGE_BGA

//*****************************************************************************
//
//  IOM0_CS pin: I/O Master 0 chip select.
//
//*****************************************************************************
const am_hal_gpio_pincfg_t g_AP3_PER_IOM0_CS = {
    .uFuncSel            = AM_HAL_PIN_11_NCE11,
    .eDriveStrength      = AM_HAL_GPIO_PIN_DRIVESTRENGTH_12MA,
    .eGPOutcfg           = AM_HAL_GPIO_PIN_OUTCFG_PUSHPULL,
    .eGPInput            = AM_HAL_GPIO_PIN_INPUT_NONE,
    .eIntDir             = AM_HAL_GPIO_PIN_INTDIR_LO2HI,
    .uIOMnum             = 0,
    .uNCE                = 0,
    .eCEpol              = AM_HAL_GPIO_PIN_CEPOL_ACTIVELOW
};

//*****************************************************************************
//
//  IOM0_CS3 pin: I/O Master 0 chip select.
//
//*****************************************************************************
const am_hal_gpio_pincfg_t g_AP3_PER_IOM0_CS3 = {
    .uFuncSel            = AM_HAL_PIN_15_NCE15,
    .eDriveStrength      = AM_HAL_GPIO_PIN_DRIVESTRENGTH_12MA,
    .eGPOutcfg           = AM_HAL_GPIO_PIN_OUTCFG_PUSHPULL,
    .eGPInput            = AM_HAL_GPIO_PIN_INPUT_NONE,
    .eIntDir             = AM_HAL_GPIO_PIN_INTDIR_LO2HI,
    .uIOMnum             = 0,
    .uNCE                = 3,
    .eCEpol              = AM_HAL_GPIO_PIN_CEPOL_ACTIVELOW
};

//*****************************************************************************
//
//  IOM0_MISO pin: I/O Master 0 SPI MISO signal.
//
//*****************************************************************************
const am_hal_gpio_pincfg_t g_AP3_PER_IOM0_MISO = {
    .uFuncSel            = AM_HAL_PIN_6_M0MISO,
    .uIOMnum             = 0
};

//*****************************************************************************
//
//  IOM0_MOSI pin: I/O Master 0 SPI MOSI signal.
//
//*****************************************************************************
const am_hal_gpio_pincfg_t g_AP3_PER_IOM0_MOSI = {
    .uFuncSel            = AM_HAL_PIN_7_M0MOSI,
    .eDriveStrength      = AM_HAL_GPIO_PIN_DRIVESTRENGTH_12MA,
    .uIOMnum             = 0
};

//*****************************************************************************
//
//  IOM0_SCK pin: I/O Master 0 SPI SCK signal.
//
//*****************************************************************************
const am_hal_gpio_pincfg_t g_AP3_PER_IOM0_SCK = {
    .uFuncSel            = AM_HAL_PIN_5_M0SCK,
    .eDriveStrength      = AM_HAL_GPIO_PIN_DRIVESTRENGTH_12MA,
    .uIOMnum             = 0
};

//*****************************************************************************
//
//  IOM0_SCL pin: I/O Master 0 I2C clock signal.
//
//*****************************************************************************
const am_hal_gpio_pincfg_t g_AP3_PER_IOM0_SCL = {
    .uFuncSel            = AM_HAL_PIN_5_M0SCL,
    .ePullup             = AM_HAL_GPIO_PIN_PULLUP_1_5K,
    .eDriveStrength      = AM_HAL_GPIO_PIN_DRIVESTRENGTH_12MA,
    .eGPOutcfg           = AM_HAL_GPIO_PIN_OUTCFG_OPENDRAIN,
    .uIOMnum             = 0
};

//*****************************************************************************
//
//  IOM0_SDA pin: I/O Master 0 I2C data signal.
//
//*****************************************************************************
const am_hal_gpio_pincfg_t g_AP3_PER_IOM0_SDA = {
    .uFuncSel            = AM_HAL_PIN_6_M0SDAWIR3,
    .ePullup             = AM_HAL_GPIO_PIN_PULLUP_1_5K,
    .eDriveStrength      = AM_HAL_GPIO_PIN_DRIVESTRENGTH_12MA,
    .eGPOutcfg           = AM_HAL_GPIO_PIN_OUTCFG_OPENDRAIN,
    .uIOMnum             = 0
};

//*****************************************************************************
//
//  IOM1_CS pin: I/O Master 1 chip select.
//
//*****************************************************************************
const am_hal_gpio_pincfg_t g_AP3_PER_IOM1_CS = {
    .uFuncSel            = AM_HAL_PIN_14_NCE14,
    .eDriveStrength      = AM_HAL_GPIO_PIN_DRIVESTRENGTH_12MA,
    .eGPOutcfg           = AM_HAL_GPIO_PIN_OUTCFG_PUSHPULL,
    .eGPInput            = AM_HAL_GPIO_PIN_INPUT_NONE,
    .eIntDir             = AM_HAL_GPIO_PIN_INTDIR_LO2HI,
    .uIOMnum             = 1,
    .uNCE                = 2,
    .eCEpol              = AM_HAL_GPIO_PIN_CEPOL_ACTIVELOW
};

//*****************************************************************************
//
//  IOM1_MISO pin: I/O Master 1 SPI MISO signal.
//
//*****************************************************************************
const am_hal_gpio_pincfg_t g_AP3_PER_IOM1_MISO = {
    .uFuncSel            = AM_HAL_PIN_9_M1MISO,
    .uIOMnum             = 1
};

//*****************************************************************************
//
//  IOM1_MOSI pin: I/O Master 1 SPI MOSI signal.
//
//*****************************************************************************
const am_hal_gpio_pincfg_t g_AP3_PER_IOM1_MOSI = {
    .uFuncSel            = AM_HAL_PIN_10_M1MOSI,
    .eDriveStrength      = AM_HAL_GPIO_PIN_DRIVESTRENGTH_12MA,
    .uIOMnum             = 1
};

//*****************************************************************************
//
//  IOM1_SCK pin: I/O Master 1 SPI SCK signal.
//
//*****************************************************************************
const am_hal_gpio_pincfg_t g_AP3_PER_IOM1_SCK = {
    .uFuncSel            = AM_HAL_PIN_8_M1SCK,
    .eDriveStrength      = AM_HAL_GPIO_PIN_DRIVESTRENGTH_12MA,
    .uIOMnum             = 1
};

//*****************************************************************************
//
//  IOM1_SCL pin: I/O Master 1 I2C clock signal.
//
//*****************************************************************************
const am_hal_gpio_pincfg_t g_AP3_PER_IOM1_SCL = {
    .uFuncSel            = AM_HAL_PIN_8_M1SCL,
    .ePullup             = AM_HAL_GPIO_PIN_PULLUP_1_5K,
    .eDriveStrength      = AM_HAL_GPIO_PIN_DRIVESTRENGTH_12MA,
    .eGPOutcfg           = AM_HAL_GPIO_PIN_OUTCFG_OPENDRAIN,
    .uIOMnum             = 1
};

//*****************************************************************************
//
//  IOM1_SDA pin: I/O Master 1 I2C data signal.
//
//*****************************************************************************
const am_hal_gpio_pincfg_t g_AP3_PER_IOM1_SDA = {
    .uFuncSel            = AM_HAL_PIN_9_M1SDAWIR3,
    .ePullup             = AM_HAL_GPIO_PIN_PULLUP_1_5K,
    .eDriveStrength      = AM_HAL_GPIO_PIN_DRIVESTRENGTH_12MA,
    .eGPOutcfg           = AM_HAL_GPIO_PIN_OUTCFG_OPENDRAIN,
    .uIOMnum             = 1
};

//*****************************************************************************
//
//  IOM2_CS pin: I/O Master 2 chip select.
//
//*****************************************************************************
const am_hal_gpio_pincfg_t g_AP3_PER_IOM2_CS = {
    .uFuncSel            = AM_HAL_PIN_15_NCE15,
    .eDriveStrength      = AM_HAL_GPIO_PIN_DRIVESTRENGTH_12MA,
    .eGPOutcfg           = AM_HAL_GPIO_PIN_OUTCFG_PUSHPULL,
    .eGPInput            = AM_HAL_GPIO_PIN_INPUT_NONE,
    .eIntDir             = AM_HAL_GPIO_PIN_INTDIR_LO2HI,
    .uIOMnum             = 2,
    .uNCE                = 3,
    .eCEpol              = AM_HAL_GPIO_PIN_CEPOL_ACTIVELOW
};

//*****************************************************************************
//
//  IOM2_MISO pin: I/O Master 2 SPI MISO signal.
//
//*****************************************************************************
const am_hal_gpio_pincfg_t g_AP3_PER_IOM2_MISO = {
    .uFuncSel            = AM_HAL_PIN_25_M2MISO,
    .uIOMnum             = 2
};

//*****************************************************************************
//
//  IOM2_MOSI pin: I/O Master 2 SPI MOSI signal.
//
//*****************************************************************************
const am_hal_gpio_pincfg_t g_AP3_PER_IOM2_MOSI = {
    .uFuncSel            = AM_HAL_PIN_28_M2MOSI,
    .eDriveStrength      = AM_HAL_GPIO_PIN_DRIVESTRENGTH_12MA,
    .uIOMnum             = 2
};

//*****************************************************************************
//
//  IOM2_SCK pin: I/O Master 2 SPI SCK signal.
//
//*****************************************************************************
const am_hal_gpio_pincfg_t g_AP3_PER_IOM2_SCK = {
    .uFuncSel            = AM_HAL_PIN_27_M2SCK,
    .eDriveStrength      = AM_HAL_GPIO_PIN_DRIVESTRENGTH_12MA,
    .uIOMnum             = 2
};

//*****************************************************************************
//
//  IOM2_SCL pin: I/O Master 2 I2C clock signal.
//
//*****************************************************************************
const am_hal_gpio_pincfg_t g_AP3_PER_IOM2_SCL = {
    .uFuncSel            = AM_HAL_PIN_27_M2SCL,
    .ePullup             = AM_HAL_GPIO_PIN_PULLUP_1_5K,
    .eDriveStrength      = AM_HAL_GPIO_PIN_DRIVESTRENGTH_12MA,
    .eGPOutcfg           = AM_HAL_GPIO_PIN_OUTCFG_OPENDRAIN,
    .uIOMnum             = 2
};

//*****************************************************************************
//
//  IOM2_SDA pin: I/O Master 2 I2C data signal.
//
//*****************************************************************************
const am_hal_gpio_pincfg_t g_AP3_PER_IOM2_SDA = {
    .uFuncSel            = AM_HAL_PIN_25_M2SDAWIR3,
    .ePullup             = AM_HAL_GPIO_PIN_PULLUP_1_5K,
    .eDriveStrength      = AM_HAL_GPIO_PIN_DRIVESTRENGTH_12MA,
    .eGPOutcfg           = AM_HAL_GPIO_PIN_OUTCFG_OPENDRAIN,
    .uIOMnum             = 2
};

//*****************************************************************************
//
//  IOM3_CS pin: I/O Master 3 chip select.
//
//*****************************************************************************
const am_hal_gpio_pincfg_t g_AP3_PER_IOM3_CS = {
    .uFuncSel            = AM_HAL_PIN_12_NCE12,
    .eDriveStrength      = AM_HAL_GPIO_PIN_DRIVESTRENGTH_12MA,
    .eGPOutcfg           = AM_HAL_GPIO_PIN_OUTCFG_PUSHPULL,
    .eGPInput            = AM_HAL_GPIO_PIN_INPUT_NONE,
    .eIntDir             = AM_HAL_GPIO_PIN_INTDIR_LO2HI,
    .uIOMnum             = 3,
    .uNCE                = 0,
    .eCEpol              = AM_HAL_GPIO_PIN_CEPOL_ACTIVELOW
};

//*****************************************************************************
//
//  IOM3_MISO pin: I/O Master 3 SPI MISO signal.
//
//*****************************************************************************
#if defined (AM_PACKAGE_BGA)
const am_hal_gpio_pincfg_t g_AP3_PER_IOM3_MISO = {
    .uFuncSel            = AM_HAL_PIN_43_M3MISO,
    .uIOMnum             = 3
};
#endif // AM_PACKAGE_BGA

//*****************************************************************************
//
//  IOM3_MOSI pin: I/O Master 3 SPI MOSI signal.
//
//*****************************************************************************
#if defined (AM_PACKAGE_BGA)
const am_hal_gpio_pincfg_t g_AP3_PER_IOM3_MOSI = {
    .uFuncSel            = AM_HAL_PIN_38_M3MOSI,
    .eDriveStrength      = AM_HAL_GPIO_PIN_DRIVESTRENGTH_12MA,
    .uIOMnum             = 3
};
#endif // AM_PACKAGE_BGA

//*****************************************************************************
//
//  IOM3_SCK pin: I/O Master 3 SPI SCK signal.
//
//*****************************************************************************
#if defined (AM_PACKAGE_BGA)
const am_hal_gpio_pincfg_t g_AP3_PER_IOM3_SCK = {
    .uFuncSel            = AM_HAL_PIN_42_M3SCK,
    .eDriveStrength      = AM_HAL_GPIO_PIN_DRIVESTRENGTH_12MA,
    .uIOMnum             = 3
};
#endif // AM_PACKAGE_BGA

//*****************************************************************************
//
//  IOM3_SCL pin: I/O Master 3 I2C clock signal.
//
//*****************************************************************************
#if defined (AM_PACKAGE_BGA)
const am_hal_gpio_pincfg_t g_AP3_PER_IOM3_SCL = {
    .uFuncSel            = AM_HAL_PIN_42_M3SCL,
    .ePullup             = AM_HAL_GPIO_PIN_PULLUP_1_5K,
    .eDriveStrength      = AM_HAL_GPIO_PIN_DRIVESTRENGTH_12MA,
    .eGPOutcfg           = AM_HAL_GPIO_PIN_OUTCFG_OPENDRAIN,
    .uIOMnum             = 3
};
#endif // AM_PACKAGE_BGA

//*****************************************************************************
//
//  IOM3_SDA pin: I/O Master 3 I2C data signal.
//
//*****************************************************************************
#if defined (AM_PACKAGE_BGA)
const am_hal_gpio_pincfg_t g_AP3_PER_IOM3_SDA = {
    .uFuncSel            = AM_HAL_PIN_43_M3SDAWIR3,
    .ePullup             = AM_HAL_GPIO_PIN_PULLUP_1_5K,
    .eDriveStrength      = AM_HAL_GPIO_PIN_DRIVESTRENGTH_12MA,
    .eGPOutcfg           = AM_HAL_GPIO_PIN_OUTCFG_OPENDRAIN,
    .uIOMnum             = 3
};
#endif // AM_PACKAGE_BGA

//*****************************************************************************
//
//  IOM4_CS pin: I/O Master 4 chip select.
//
//*****************************************************************************
const am_hal_gpio_pincfg_t g_AP3_PER_IOM4_CS = {
    .uFuncSel            = AM_HAL_PIN_13_NCE13,
    .eDriveStrength      = AM_HAL_GPIO_PIN_DRIVESTRENGTH_12MA,
    .eGPOutcfg           = AM_HAL_GPIO_PIN_OUTCFG_PUSHPULL,
    .eGPInput            = AM_HAL_GPIO_PIN_INPUT_NONE,
    .eIntDir             = AM_HAL_GPIO_PIN_INTDIR_LO2HI,
    .uIOMnum             = 4,
    .uNCE                = 1,
    .eCEpol              = AM_HAL_GPIO_PIN_CEPOL_ACTIVELOW
};

//*****************************************************************************
//
//  IOM4_MISO pin: I/O Master 4 SPI MISO signal.
//
//*****************************************************************************
const am_hal_gpio_pincfg_t g_AP3_PER_IOM4_MISO = {
    .uFuncSel            = AM_HAL_PIN_40_M4MISO,
    .uIOMnum             = 4
};

//*****************************************************************************
//
//  IOM4_MOSI pin: I/O Master 4 SPI MOSI signal.
//
//*****************************************************************************
const am_hal_gpio_pincfg_t g_AP3_PER_IOM4_MOSI = {
    .uFuncSel            = AM_HAL_PIN_44_M4MOSI,
    .eDriveStrength      = AM_HAL_GPIO_PIN_DRIVESTRENGTH_12MA,
    .uIOMnum             = 4
};

//*****************************************************************************
//
//  IOM4_SCK pin: I/O Master 4 SPI SCK signal.
//
//*****************************************************************************
const am_hal_gpio_pincfg_t g_AP3_PER_IOM4_SCK = {
    .uFuncSel            = AM_HAL_PIN_39_M4SCK,
    .eDriveStrength      = AM_HAL_GPIO_PIN_DRIVESTRENGTH_12MA,
    .uIOMnum             = 4
};

//*****************************************************************************
//
//  IOM4_SCL pin: I/O Master 4 I2C clock signal.
//
//*****************************************************************************
const am_hal_gpio_pincfg_t g_AP3_PER_IOM4_SCL = {
    .uFuncSel            = AM_HAL_PIN_39_M4SCL,
    .ePullup             = AM_HAL_GPIO_PIN_PULLUP_1_5K,
    .eDriveStrength      = AM_HAL_GPIO_PIN_DRIVESTRENGTH_12MA,
    .eGPOutcfg           = AM_HAL_GPIO_PIN_OUTCFG_OPENDRAIN,
    .uIOMnum             = 4
};

//*****************************************************************************
//
//  IOM4_SDA pin: I/O Master 4 I2C data signal.
//
//*****************************************************************************
const am_hal_gpio_pincfg_t g_AP3_PER_IOM4_SDA = {
    .uFuncSel            = AM_HAL_PIN_40_M4SDAWIR3,
    .ePullup             = AM_HAL_GPIO_PIN_PULLUP_1_5K,
    .eDriveStrength      = AM_HAL_GPIO_PIN_DRIVESTRENGTH_12MA,
    .eGPOutcfg           = AM_HAL_GPIO_PIN_OUTCFG_OPENDRAIN,
    .uIOMnum             = 4
};

//*****************************************************************************
//
//  IOM5_CS pin: I/O Master 5 chip select.
//
//*****************************************************************************
const am_hal_gpio_pincfg_t g_AP3_PER_IOM5_CS = {
    .uFuncSel            = AM_HAL_PIN_16_NCE16,
    .eDriveStrength      = AM_HAL_GPIO_PIN_DRIVESTRENGTH_12MA,
    .eGPOutcfg           = AM_HAL_GPIO_PIN_OUTCFG_PUSHPULL,
    .eGPInput            = AM_HAL_GPIO_PIN_INPUT_NONE,
    .eIntDir             = AM_HAL_GPIO_PIN_INTDIR_LO2HI,
    .uIOMnum             = 5,
    .uNCE                = 0,
    .eCEpol              = AM_HAL_GPIO_PIN_CEPOL_ACTIVELOW
};

//*****************************************************************************
//
//  IOM5_MISO pin: I/O Master 5 SPI MISO signal.
//
//*****************************************************************************
const am_hal_gpio_pincfg_t g_AP3_PER_IOM5_MISO = {
    .uFuncSel            = AM_HAL_PIN_49_M5MISO,
    .uIOMnum             = 5
};

//*****************************************************************************
//
//  IOM5_MOSI pin: I/O Master 5 SPI MOSI signal.
//
//*****************************************************************************
const am_hal_gpio_pincfg_t g_AP3_PER_IOM5_MOSI = {
    .uFuncSel            = AM_HAL_PIN_47_M5MOSI,
    .eDriveStrength      = AM_HAL_GPIO_PIN_DRIVESTRENGTH_12MA,
    .uIOMnum             = 5
};

//*****************************************************************************
//
//  IOM5_SCK pin: I/O Master 5 SPI SCK signal.
//
//*****************************************************************************
const am_hal_gpio_pincfg_t g_AP3_PER_IOM5_SCK = {
    .uFuncSel            = AM_HAL_PIN_48_M5SCK,
    .eDriveStrength      = AM_HAL_GPIO_PIN_DRIVESTRENGTH_12MA,
    .uIOMnum             = 5
};

//*****************************************************************************
//
//  IOM5_SCL pin: I/O Master 5 I2C clock signal.
//
//*****************************************************************************
const am_hal_gpio_pincfg_t g_AP3_PER_IOM5_SCL = {
    .uFuncSel            = AM_HAL_PIN_48_M5SCL,
    .ePullup             = AM_HAL_GPIO_PIN_PULLUP_1_5K,
    .eDriveStrength      = AM_HAL_GPIO_PIN_DRIVESTRENGTH_12MA,
    .eGPOutcfg           = AM_HAL_GPIO_PIN_OUTCFG_OPENDRAIN,
    .uIOMnum             = 5
};

//*****************************************************************************
//
//  IOM5_SDA pin: I/O Master 5 I2C data signal.
//
//*****************************************************************************
const am_hal_gpio_pincfg_t g_AP3_PER_IOM5_SDA = {
    .uFuncSel            = AM_HAL_PIN_49_M5SDAWIR3,
    .ePullup             = AM_HAL_GPIO_PIN_PULLUP_1_5K,
    .eDriveStrength      = AM_HAL_GPIO_PIN_DRIVESTRENGTH_12MA,
    .eGPOutcfg           = AM_HAL_GPIO_PIN_OUTCFG_OPENDRAIN,
    .uIOMnum             = 5
};

//*****************************************************************************
//
//  MSPI_CE0 pin: MSPI chip select.
//
//*****************************************************************************
const am_hal_gpio_pincfg_t g_AP3_PER_MSPI_CE0 = {
    .uFuncSel            = AM_HAL_PIN_19_NCE19,
    .eDriveStrength      = AM_HAL_GPIO_PIN_DRIVESTRENGTH_12MA,
    .eGPOutcfg           = AM_HAL_GPIO_PIN_OUTCFG_PUSHPULL,
    .eGPInput            = AM_HAL_GPIO_PIN_INPUT_NONE,
    .eIntDir             = AM_HAL_GPIO_PIN_INTDIR_LO2HI,
    .uIOMnum             = 6,
    .uNCE                = 0,
    .eCEpol              = AM_HAL_GPIO_PIN_CEPOL_ACTIVELOW
};

//*****************************************************************************
//
//  MSPI_CE1 pin: MSPI chip select.
//
//*****************************************************************************
const am_hal_gpio_pincfg_t g_AP3_PER_MSPI_CE1 = {
    .uFuncSel            = AM_HAL_PIN_41_NCE41,
    .eDriveStrength      = AM_HAL_GPIO_PIN_DRIVESTRENGTH_12MA,
    .eGPOutcfg           = AM_HAL_GPIO_PIN_OUTCFG_PUSHPULL,
    .eGPInput            = AM_HAL_GPIO_PIN_INPUT_NONE,
    .eIntDir             = AM_HAL_GPIO_PIN_INTDIR_LO2HI,
    .uIOMnum             = 6,
    .uNCE                = 1,
    .eCEpol              = AM_HAL_GPIO_PIN_CEPOL_ACTIVELOW
};

//*****************************************************************************
//
//  MSPI_D0 pin: MSPI data 0.
//
//*****************************************************************************
const am_hal_gpio_pincfg_t g_AP3_PER_MSPI_D0 = {
    .uFuncSel            = AM_HAL_PIN_22_MSPI0,
    .eDriveStrength      = AM_HAL_GPIO_PIN_DRIVESTRENGTH_8MA,
    .eIntDir             = AM_HAL_GPIO_PIN_INTDIR_LO2HI,
    .uIOMnum             = 6
};

//*****************************************************************************
//
//  MSPI_D1 pin: MSPI data 1.
//
//*****************************************************************************
const am_hal_gpio_pincfg_t g_AP3_PER_MSPI_D1 = {
    .uFuncSel            = AM_HAL_PIN_26_MSPI1,
    .eDriveStrength      = AM_HAL_GPIO_PIN_DRIVESTRENGTH_8MA,
    .eIntDir             = AM_HAL_GPIO_PIN_INTDIR_LO2HI,
    .uIOMnum             = 6
};

//*****************************************************************************
//
//  MSPI_D2 pin: MSPI data 2.
//
//*****************************************************************************
const am_hal_gpio_pincfg_t g_AP3_PER_MSPI_D2 = {
    .uFuncSel            = AM_HAL_PIN_4_MSPI2,
    .eDriveStrength      = AM_HAL_GPIO_PIN_DRIVESTRENGTH_8MA,
    .eIntDir             = AM_HAL_GPIO_PIN_INTDIR_LO2HI,
    .uIOMnum             = 6
};

//*****************************************************************************
//
//  MSPI_D3 pin: MSPI data 3.
//
//*****************************************************************************
const am_hal_gpio_pincfg_t g_AP3_PER_MSPI_D3 = {
    .uFuncSel            = AM_HAL_PIN_23_MSPI13,
    .eDriveStrength      = AM_HAL_GPIO_PIN_DRIVESTRENGTH_8MA,
    .eIntDir             = AM_HAL_GPIO_PIN_INTDIR_LO2HI,
    .uIOMnum             = 6
};

//*****************************************************************************
//
//  MSPI_D4 pin: MSPI data 4.
//
//*****************************************************************************
const am_hal_gpio_pincfg_t g_AP3_PER_MSPI_D4 = {
    .uFuncSel            = AM_HAL_PIN_0_MSPI4,
    .eDriveStrength      = AM_HAL_GPIO_PIN_DRIVESTRENGTH_8MA,
    .eIntDir             = AM_HAL_GPIO_PIN_INTDIR_LO2HI,
    .uIOMnum             = 6
};

//*****************************************************************************
//
//  MSPI_D5 pin: MSPI data 5.
//
//*****************************************************************************
const am_hal_gpio_pincfg_t g_AP3_PER_MSPI_D5 = {
    .uFuncSel            = AM_HAL_PIN_1_MSPI5,
    .eDriveStrength      = AM_HAL_GPIO_PIN_DRIVESTRENGTH_8MA,
    .eIntDir             = AM_HAL_GPIO_PIN_INTDIR_LO2HI,
    .uIOMnum             = 6
};

//*****************************************************************************
//
//  MSPI_D6 pin: MSPI data 6.
//
//*****************************************************************************
const am_hal_gpio_pincfg_t g_AP3_PER_MSPI_D6 = {
    .uFuncSel            = AM_HAL_PIN_2_MSPI6,
    .eDriveStrength      = AM_HAL_GPIO_PIN_DRIVESTRENGTH_8MA,
    .eIntDir             = AM_HAL_GPIO_PIN_INTDIR_LO2HI,
    .uIOMnum             = 6
};

//*****************************************************************************
//
//  MSPI_D7 pin: MSPI data 7.
//
//*****************************************************************************
const am_hal_gpio_pincfg_t g_AP3_PER_MSPI_D7 = {
    .uFuncSel            = AM_HAL_PIN_3_MSPI7,
    .eDriveStrength      = AM_HAL_GPIO_PIN_DRIVESTRENGTH_8MA,
    .eIntDir             = AM_HAL_GPIO_PIN_INTDIR_LO2HI,
    .uIOMnum             = 6
};

//*****************************************************************************
//
//  MSPI_SCK pin: MSPI clock.
//
//*****************************************************************************
const am_hal_gpio_pincfg_t g_AP3_PER_MSPI_SCK = {
    .uFuncSel            = AM_HAL_PIN_24_MSPI8,
    .eDriveStrength      = AM_HAL_GPIO_PIN_DRIVESTRENGTH_12MA,
    .eIntDir             = AM_HAL_GPIO_PIN_INTDIR_LO2HI,
    .uIOMnum             = 6
};

//*****************************************************************************
//
//  IOS_CE pin: I/O Slave chip select.
//
//*****************************************************************************
const am_hal_gpio_pincfg_t g_AP3_PER_IOS_CE = {
    .uFuncSel            = AM_HAL_PIN_3_SLnCE,
    .eGPInput            = AM_HAL_GPIO_PIN_INPUT_ENABLE,
    .uNCE                = 0,
    .eCEpol              = AM_HAL_GPIO_PIN_CEPOL_ACTIVELOW
};

//*****************************************************************************
//
//  IOS_MISO pin: I/O Slave SPI MISO signal.
//
//*****************************************************************************
const am_hal_gpio_pincfg_t g_AP3_PER_IOS_MISO = {
    .uFuncSel            = AM_HAL_PIN_2_SLMISO,
    .eDriveStrength      = AM_HAL_GPIO_PIN_DRIVESTRENGTH_12MA
};

//*****************************************************************************
//
//  IOS_MOSI pin: I/O Slave SPI MOSI signal.
//
//*****************************************************************************
const am_hal_gpio_pincfg_t g_AP3_PER_IOS_MOSI = {
    .uFuncSel            = AM_HAL_PIN_1_SLMOSI,
    .eGPInput            = AM_HAL_GPIO_PIN_INPUT_ENABLE
};

//*****************************************************************************
//
//  IOS_SCK pin: I/O Slave SPI SCK signal.
//
//*****************************************************************************
const am_hal_gpio_pincfg_t g_AP3_PER_IOS_SCK = {
    .uFuncSel            = AM_HAL_PIN_0_SLSCK,
    .eGPInput            = AM_HAL_GPIO_PIN_INPUT_ENABLE
};

//*****************************************************************************
//
//  IOS_SCL pin: I/O Slave I2C clock signal.
//
//*****************************************************************************
const am_hal_gpio_pincfg_t g_AP3_PER_IOS_SCL = {
    .uFuncSel            = AM_HAL_PIN_0_SLSCL,
    .eGPInput            = AM_HAL_GPIO_PIN_INPUT_ENABLE
};

//*****************************************************************************
//
//  IOS_SDA pin: I/O Slave I2C data signal.
//
//*****************************************************************************
const am_hal_gpio_pincfg_t g_AP3_PER_IOS_SDA = {
    .uFuncSel            = AM_HAL_PIN_1_SLSDAWIR3,
    .ePullup             = AM_HAL_GPIO_PIN_PULLUP_1_5K,
    .eGPOutcfg           = AM_HAL_GPIO_PIN_OUTCFG_OPENDRAIN
};

//*****************************************************************************
//
//  NCE_0 pin: NCE Pin.
//
//*****************************************************************************
const am_hal_gpio_pincfg_t g_AP3_PER_NCE_0 = {
    .uFuncSel            = AM_HAL_PIN_0_NCE0
};

//*****************************************************************************
//
//  NCE_1 pin: NCE Pin.
//
//*****************************************************************************
const am_hal_gpio_pincfg_t g_AP3_PER_NCE_1 = {
    .uFuncSel            = AM_HAL_PIN_1_NCE1
};

//*****************************************************************************
//
//  NCE_2 pin: NCE Pin.
//
//*****************************************************************************
const am_hal_gpio_pincfg_t g_AP3_PER_NCE_2 = {
    .uFuncSel            = AM_HAL_PIN_2_NCE2
};

//*****************************************************************************
//
//  NCE_3 pin: NCE Pin.
//
//*****************************************************************************
const am_hal_gpio_pincfg_t g_AP3_PER_NCE_3 = {
    .uFuncSel            = AM_HAL_PIN_3_NCE3
};

//*****************************************************************************
//
//  NCE_4 pin: NCE Pin.
//
//*****************************************************************************
const am_hal_gpio_pincfg_t g_AP3_PER_NCE_4 = {
    .uFuncSel            = AM_HAL_PIN_4_NCE4
};

//*****************************************************************************
//
//  NCE_7 pin: NCE Pin.
//
//*****************************************************************************
const am_hal_gpio_pincfg_t g_AP3_PER_NCE_7 = {
    .uFuncSel            = AM_HAL_PIN_7_NCE7
};

//*****************************************************************************
//
//  NCE_8 pin: NCE Pin.
//
//*****************************************************************************
const am_hal_gpio_pincfg_t g_AP3_PER_NCE_8 = {
    .uFuncSel            = AM_HAL_PIN_8_NCE8
};

//*****************************************************************************
//
//  NCE_9 pin: NCE Pin.
//
//*****************************************************************************
const am_hal_gpio_pincfg_t g_AP3_PER_NCE_9 = {
    .uFuncSel            = AM_HAL_PIN_9_NCE9
};

//*****************************************************************************
//
//  NCE_10 pin: NCE Pin.
//
//*****************************************************************************
const am_hal_gpio_pincfg_t g_AP3_PER_NCE_10 = {
    .uFuncSel            = AM_HAL_PIN_10_NCE10
};

//*****************************************************************************
//
//  NCE_11 pin: NCE Pin.
//
//*****************************************************************************
const am_hal_gpio_pincfg_t g_AP3_PER_NCE_11 = {
    .uFuncSel            = AM_HAL_PIN_11_NCE11
};

//*****************************************************************************
//
//  NCE_12 pin: NCE Pin.
//
//*****************************************************************************
const am_hal_gpio_pincfg_t g_AP3_PER_NCE_12 = {
    .uFuncSel            = AM_HAL_PIN_12_NCE12
};

//*****************************************************************************
//
//  NCE_13 pin: NCE Pin.
//
//*****************************************************************************
const am_hal_gpio_pincfg_t g_AP3_PER_NCE_13 = {
    .uFuncSel            = AM_HAL_PIN_13_NCE13
};

//*****************************************************************************
//
//  NCE_14 pin: NCE Pin.
//
//*****************************************************************************
const am_hal_gpio_pincfg_t g_AP3_PER_NCE_14 = {
    .uFuncSel            = AM_HAL_PIN_14_NCE14
};

//*****************************************************************************
//
//  NCE_15 pin: NCE Pin.
//
//*****************************************************************************
const am_hal_gpio_pincfg_t g_AP3_PER_NCE_15 = {
    .uFuncSel            = AM_HAL_PIN_15_NCE15
};

//*****************************************************************************
//
//  NCE_16 pin: NCE Pin.
//
//*****************************************************************************
const am_hal_gpio_pincfg_t g_AP3_PER_NCE_16 = {
    .uFuncSel            = AM_HAL_PIN_16_NCE16
};

//*****************************************************************************
//
//  NCE_17 pin: NCE Pin.
//
//*****************************************************************************
const am_hal_gpio_pincfg_t g_AP3_PER_NCE_17 = {
    .uFuncSel            = AM_HAL_PIN_17_NCE17
};

//*****************************************************************************
//
//  NCE_18 pin: NCE Pin.
//
//*****************************************************************************
const am_hal_gpio_pincfg_t g_AP3_PER_NCE_18 = {
    .uFuncSel            = AM_HAL_PIN_18_NCE18
};

//*****************************************************************************
//
//  NCE_19 pin: NCE Pin.
//
//*****************************************************************************
const am_hal_gpio_pincfg_t g_AP3_PER_NCE_19 = {
    .uFuncSel            = AM_HAL_PIN_19_NCE19
};

//*****************************************************************************
//
//  NCE_20 pin: NCE Pin.
//
//*****************************************************************************
const am_hal_gpio_pincfg_t g_AP3_PER_NCE_20 = {
    .uFuncSel            = AM_HAL_PIN_20_NCE20
};

//*****************************************************************************
//
//  NCE_21 pin: NCE Pin.
//
//*****************************************************************************
const am_hal_gpio_pincfg_t g_AP3_PER_NCE_21 = {
    .uFuncSel            = AM_HAL_PIN_21_NCE21
};

//*****************************************************************************
//
//  NCE_22 pin: NCE Pin.
//
//*****************************************************************************
const am_hal_gpio_pincfg_t g_AP3_PER_NCE_22 = {
    .uFuncSel            = AM_HAL_PIN_22_NCE22
};

//*****************************************************************************
//
//  NCE_23 pin: NCE Pin.
//
//*****************************************************************************
const am_hal_gpio_pincfg_t g_AP3_PER_NCE_23 = {
    .uFuncSel            = AM_HAL_PIN_23_NCE23
};

//*****************************************************************************
//
//  NCE_24 pin: NCE Pin.
//
//*****************************************************************************
const am_hal_gpio_pincfg_t g_AP3_PER_NCE_24 = {
    .uFuncSel            = AM_HAL_PIN_24_NCE24
};

//*****************************************************************************
//
//  NCE_25 pin: NCE Pin.
//
//*****************************************************************************
const am_hal_gpio_pincfg_t g_AP3_PER_NCE_25 = {
    .uFuncSel            = AM_HAL_PIN_25_NCE25
};

//*****************************************************************************
//
//  NCE_26 pin: NCE Pin.
//
//*****************************************************************************
const am_hal_gpio_pincfg_t g_AP3_PER_NCE_26 = {
    .uFuncSel            = AM_HAL_PIN_26_NCE26
};

//*****************************************************************************
//
//  NCE_27 pin: NCE Pin.
//
//*****************************************************************************
const am_hal_gpio_pincfg_t g_AP3_PER_NCE_27 = {
    .uFuncSel            = AM_HAL_PIN_27_NCE27
};

//*****************************************************************************
//
//  NCE_28 pin: NCE Pin.
//
//*****************************************************************************
const am_hal_gpio_pincfg_t g_AP3_PER_NCE_28 = {
    .uFuncSel            = AM_HAL_PIN_28_NCE28
};

//*****************************************************************************
//
//  NCE_29 pin: NCE Pin.
//
//*****************************************************************************
const am_hal_gpio_pincfg_t g_AP3_PER_NCE_29 = {
    .uFuncSel            = AM_HAL_PIN_29_NCE29
};

//*****************************************************************************
//
//  NCE_30 pin: NCE Pin.
//
//*****************************************************************************
#if defined (AM_PACKAGE_BGA)
const am_hal_gpio_pincfg_t g_AP3_PER_NCE_30 = {
    .uFuncSel            = AM_HAL_PIN_30_NCE30
};
#endif // AM_PACKAGE_BGA

//*****************************************************************************
//
//  NCE_31 pin: NCE Pin.
//
//*****************************************************************************
#if defined (AM_PACKAGE_BGA)
const am_hal_gpio_pincfg_t g_AP3_PER_NCE_31 = {
    .uFuncSel            = AM_HAL_PIN_31_NCE31
};
#endif // AM_PACKAGE_BGA

//*****************************************************************************
//
//  NCE_32 pin: NCE Pin.
//
//*****************************************************************************
#if defined (AM_PACKAGE_BGA)
const am_hal_gpio_pincfg_t g_AP3_PER_NCE_32 = {
    .uFuncSel            = AM_HAL_PIN_32_NCE32
};
#endif // AM_PACKAGE_BGA

//*****************************************************************************
//
//  NCE_33 pin: NCE Pin.
//
//*****************************************************************************
#if defined (AM_PACKAGE_BGA)
const am_hal_gpio_pincfg_t g_AP3_PER_NCE_33 = {
    .uFuncSel            = AM_HAL_PIN_33_NCE33
};
#endif // AM_PACKAGE_BGA

//*****************************************************************************
//
//  NCE_34 pin: NCE Pin.
//
//*****************************************************************************
#if defined (AM_PACKAGE_BGA)
const am_hal_gpio_pincfg_t g_AP3_PER_NCE_34 = {
    .uFuncSel            = AM_HAL_PIN_34_NCE34
};
#endif // AM_PACKAGE_BGA

//*****************************************************************************
//
//  NCE_35 pin: NCE Pin.
//
//*****************************************************************************
#if defined (AM_PACKAGE_BGA)
const am_hal_gpio_pincfg_t g_AP3_PER_NCE_35 = {
    .uFuncSel            = AM_HAL_PIN_35_NCE35
};
#endif // AM_PACKAGE_BGA

//*****************************************************************************
//
//  NCE_36 pin: NCE Pin.
//
//*****************************************************************************
#if defined (AM_PACKAGE_BGA)
const am_hal_gpio_pincfg_t g_AP3_PER_NCE_36 = {
    .uFuncSel            = AM_HAL_PIN_36_NCE36
};
#endif // AM_PACKAGE_BGA

//*****************************************************************************
//
//  NCE_37 pin: NCE Pin.
//
//*****************************************************************************
#if defined (AM_PACKAGE_BGA)
const am_hal_gpio_pincfg_t g_AP3_PER_NCE_37 = {
    .uFuncSel            = AM_HAL_PIN_37_NCE37
};
#endif // AM_PACKAGE_BGA

//*****************************************************************************
//
//  NCE_38 pin: NCE Pin.
//
//*****************************************************************************
#if defined (AM_PACKAGE_BGA)
const am_hal_gpio_pincfg_t g_AP3_PER_NCE_38 = {
    .uFuncSel            = AM_HAL_PIN_38_NCE38
};
#endif // AM_PACKAGE_BGA

//*****************************************************************************
//
//  NCE_41 pin: NCE Pin.
//
//*****************************************************************************
const am_hal_gpio_pincfg_t g_AP3_PER_NCE_41 = {
    .uFuncSel            = AM_HAL_PIN_41_NCE41
};

//*****************************************************************************
//
//  NCE_42 pin: NCE Pin.
//
//*****************************************************************************
#if defined (AM_PACKAGE_BGA)
const am_hal_gpio_pincfg_t g_AP3_PER_NCE_42 = {
    .uFuncSel            = AM_HAL_PIN_42_NCE42
};
#endif // AM_PACKAGE_BGA

//*****************************************************************************
//
//  NCE_43 pin: NCE Pin.
//
//*****************************************************************************
#if defined (AM_PACKAGE_BGA)
const am_hal_gpio_pincfg_t g_AP3_PER_NCE_43 = {
    .uFuncSel            = AM_HAL_PIN_43_NCE43
};
#endif // AM_PACKAGE_BGA

//*****************************************************************************
//
//  NCE_44 pin: NCE Pin.
//
//*****************************************************************************
const am_hal_gpio_pincfg_t g_AP3_PER_NCE_44 = {
    .uFuncSel            = AM_HAL_PIN_44_NCE44
};

//*****************************************************************************
//
//  NCE_45 pin: NCE Pin.
//
//*****************************************************************************
#if defined (AM_PACKAGE_BGA)
const am_hal_gpio_pincfg_t g_AP3_PER_NCE_45 = {
    .uFuncSel            = AM_HAL_PIN_45_NCE45
};
#endif // AM_PACKAGE_BGA

//*****************************************************************************
//
//  NCE_46 pin: NCE Pin.
//
//*****************************************************************************
#if defined (AM_PACKAGE_BGA)
const am_hal_gpio_pincfg_t g_AP3_PER_NCE_46 = {
    .uFuncSel            = AM_HAL_PIN_46_NCE46
};
#endif // AM_PACKAGE_BGA

//*****************************************************************************
//
//  NCE_47 pin: NCE Pin.
//
//*****************************************************************************
const am_hal_gpio_pincfg_t g_AP3_PER_NCE_47 = {
    .uFuncSel            = AM_HAL_PIN_47_NCE47
};

//*****************************************************************************
//
//  NCE_48 pin: NCE Pin.
//
//*****************************************************************************
const am_hal_gpio_pincfg_t g_AP3_PER_NCE_48 = {
    .uFuncSel            = AM_HAL_PIN_48_NCE48
};

//*****************************************************************************
//
//  NCE_49 pin: NCE Pin.
//
//*****************************************************************************
const am_hal_gpio_pincfg_t g_AP3_PER_NCE_49 = {
    .uFuncSel            = AM_HAL_PIN_49_NCE49
};

//*****************************************************************************
//
//  PDM_DATA_11 pin: Data line for PDM microphones.
//
//*****************************************************************************
const am_hal_gpio_pincfg_t g_AP3_PER_PDM_DATA_11 = {
    .uFuncSel            = AM_HAL_PIN_11_PDMDATA
};

//*****************************************************************************
//
//  PDM_DATA_15 pin: Data line for PDM microphones.
//
//*****************************************************************************
const am_hal_gpio_pincfg_t g_AP3_PER_PDM_DATA_15 = {
    .uFuncSel            = AM_HAL_PIN_15_PDMDATA
};

//*****************************************************************************
//
//  PDM_DATA_29 pin: Data line for PDM microphones.
//
//*****************************************************************************
const am_hal_gpio_pincfg_t g_AP3_PER_PDM_DATA_29 = {
    .uFuncSel            = AM_HAL_PIN_29_PDMDATA
};

//*****************************************************************************
//
//  PDM_DATA_34 pin: Data line for PDM microphones.
//
//*****************************************************************************
#if defined (AM_PACKAGE_BGA)
const am_hal_gpio_pincfg_t g_AP3_PER_PDM_DATA_34 = {
    .uFuncSel            = AM_HAL_PIN_34_PDMDATA
};
#endif // AM_PACKAGE_BGA

//*****************************************************************************
//
//  PDM_DATA_36 pin: Data line for PDM microphones.
//
//*****************************************************************************
#if defined (AM_PACKAGE_BGA)
const am_hal_gpio_pincfg_t g_AP3_PER_PDM_DATA_36 = {
    .uFuncSel            = AM_HAL_PIN_36_PDMDATA
};
#endif // AM_PACKAGE_BGA

//*****************************************************************************
//
//  PDM_DATA_45 pin: Data line for PDM microphones.
//
//*****************************************************************************
#if defined (AM_PACKAGE_BGA)
const am_hal_gpio_pincfg_t g_AP3_PER_PDM_DATA_45 = {
    .uFuncSel            = AM_HAL_PIN_45_PDMDATA
};
#endif // AM_PACKAGE_BGA

//*****************************************************************************
//
//  PDM_CLK_10 pin: Clock line for PDM microphones.
//
//*****************************************************************************
const am_hal_gpio_pincfg_t g_AP3_PER_PDM_CLK_10 = {
    .uFuncSel            = AM_HAL_PIN_10_PDMCLK
};

//*****************************************************************************
//
//  PDM_CLK_12 pin: Clock line for PDM microphones.
//
//*****************************************************************************
const am_hal_gpio_pincfg_t g_AP3_PER_PDM_CLK_12 = {
    .uFuncSel            = AM_HAL_PIN_12_PDMCLK
};

//*****************************************************************************
//
//  PDM_CLK_14 pin: Clock line for PDM microphones.
//
//*****************************************************************************
const am_hal_gpio_pincfg_t g_AP3_PER_PDM_CLK_14 = {
    .uFuncSel            = AM_HAL_PIN_14_PDMCLK
};

//*****************************************************************************
//
//  PDM_CLK_22 pin: Clock line for PDM microphones.
//
//*****************************************************************************
const am_hal_gpio_pincfg_t g_AP3_PER_PDM_CLK_22 = {
    .uFuncSel            = AM_HAL_PIN_22_PDMCLK
};

//*****************************************************************************
//
//  PDM_CLK_37 pin: Clock line for PDM microphones.
//
//*****************************************************************************
#if defined (AM_PACKAGE_BGA)
const am_hal_gpio_pincfg_t g_AP3_PER_PDM_CLK_37 = {
    .uFuncSel            = AM_HAL_PIN_37_PDMCLK
};
#endif // AM_PACKAGE_BGA

//*****************************************************************************
//
//  PDM_CLK_46 pin: Clock line for PDM microphones.
//
//*****************************************************************************
#if defined (AM_PACKAGE_BGA)
const am_hal_gpio_pincfg_t g_AP3_PER_PDM_CLK_46 = {
    .uFuncSel            = AM_HAL_PIN_46_PDMCLK
};
#endif // AM_PACKAGE_BGA

//*****************************************************************************
//
//  ITM_SWO_15 pin: ITM Serial Wire Output.
//
//*****************************************************************************
const am_hal_gpio_pincfg_t g_AP3_PER_ITM_SWO_15 = {
    .uFuncSel            = AM_HAL_PIN_15_SWO,
    .eDriveStrength      = AM_HAL_GPIO_PIN_DRIVESTRENGTH_2MA
};

//*****************************************************************************
//
//  ITM_SWO pin: ITM Serial Wire Output.
//
//*****************************************************************************
const am_hal_gpio_pincfg_t g_AP3_PER_ITM_SWO = {
    .uFuncSel            = AM_HAL_PIN_22_SWO,
    .eDriveStrength      = AM_HAL_GPIO_PIN_DRIVESTRENGTH_2MA
};

//*****************************************************************************
//
//  ITM_SWO_24 pin: ITM Serial Wire Output.
//
//*****************************************************************************
const am_hal_gpio_pincfg_t g_AP3_PER_ITM_SWO_24 = {
    .uFuncSel            = AM_HAL_PIN_24_SWO,
    .eDriveStrength      = AM_HAL_GPIO_PIN_DRIVESTRENGTH_2MA
};

//*****************************************************************************
//
//  ITM_SWO_33 pin: ITM Serial Wire Output.
//
//*****************************************************************************
#if defined (AM_PACKAGE_BGA)
const am_hal_gpio_pincfg_t g_AP3_PER_ITM_SWO_33 = {
    .uFuncSel            = AM_HAL_PIN_33_SWO,
    .eDriveStrength      = AM_HAL_GPIO_PIN_DRIVESTRENGTH_2MA
};
#endif // AM_PACKAGE_BGA

//*****************************************************************************
//
//  ITM_SWO_41 pin: ITM Serial Wire Output.
//
//*****************************************************************************
const am_hal_gpio_pincfg_t g_AP3_PER_ITM_SWO_41 = {
    .uFuncSel            = AM_HAL_PIN_41_SWO,
    .eDriveStrength      = AM_HAL_GPIO_PIN_DRIVESTRENGTH_2MA
};

//*****************************************************************************
//
//  ITM_SWO_45 pin: ITM Serial Wire Output.
//
//*****************************************************************************
#if defined (AM_PACKAGE_BGA)
const am_hal_gpio_pincfg_t g_AP3_PER_ITM_SWO_45 = {
    .uFuncSel            = AM_HAL_PIN_45_SWO,
    .eDriveStrength      = AM_HAL_GPIO_PIN_DRIVESTRENGTH_2MA
};
#endif // AM_PACKAGE_BGA

//*****************************************************************************
//
//  ITM_SWO_46 pin: ITM Serial Wire Output.
//
//*****************************************************************************
#if defined (AM_PACKAGE_BGA)
const am_hal_gpio_pincfg_t g_AP3_PER_ITM_SWO_46 = {
    .uFuncSel            = AM_HAL_PIN_46_SWO,
    .eDriveStrength      = AM_HAL_GPIO_PIN_DRIVESTRENGTH_2MA
};
#endif // AM_PACKAGE_BGA

//*****************************************************************************
//
//  CORE_SWDCK_14 pin: Cortex Serial Wire Debug Clock.
//
//*****************************************************************************
const am_hal_gpio_pincfg_t g_AP3_PER_CORE_SWDCK_14 = {
    .uFuncSel            = AM_HAL_PIN_14_SWDCK
};

//*****************************************************************************
//
//  CORE_SWDCK_20 pin: Cortex Serial Wire Debug Clock.
//
//*****************************************************************************
const am_hal_gpio_pincfg_t g_AP3_PER_CORE_SWDCK_20 = {
    .uFuncSel            = AM_HAL_PIN_20_SWDCK
};

//*****************************************************************************
//
//  CORE_SWDIO_15 pin: Cortex Serial Wire Debug I/O.
//
//*****************************************************************************
const am_hal_gpio_pincfg_t g_AP3_PER_CORE_SWDIO_15 = {
    .uFuncSel            = AM_HAL_PIN_15_SWDIO
};

//*****************************************************************************
//
//  CORE_SWDIO_21 pin: Cortex Serial Wire Debug I/O.
//
//*****************************************************************************
const am_hal_gpio_pincfg_t g_AP3_PER_CORE_SWDIO_21 = {
    .uFuncSel            = AM_HAL_PIN_21_SWDIO
};

//*****************************************************************************
//
// End Doxygen group.
//! @}
//
//*****************************************************************************
