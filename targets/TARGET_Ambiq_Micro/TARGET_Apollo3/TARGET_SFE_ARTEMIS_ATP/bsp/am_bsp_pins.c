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
//  LED_BLUE pin: The BLUE LED labelled 5.
//
//*****************************************************************************
const am_hal_gpio_pincfg_t g_AM_BSP_GPIO_LED_BLUE =
{
    .uFuncSel            = AM_HAL_PIN_5_GPIO,
    .eDriveStrength      = AM_HAL_GPIO_PIN_DRIVESTRENGTH_12MA
};

//*****************************************************************************
//
//  MIC_DATA pin: Data line for PDM microphones.
//
//*****************************************************************************
const am_hal_gpio_pincfg_t g_AM_BSP_GPIO_MIC_DATA =
{
    .uFuncSel            = AM_HAL_PIN_36_PDMDATA
};

//*****************************************************************************
//
//  MIC_CLK pin: Clock line for PDM microphones.
//
//*****************************************************************************
const am_hal_gpio_pincfg_t g_AM_BSP_GPIO_MIC_CLK =
{
    .uFuncSel            = AM_HAL_PIN_37_PDMCLK
};

//*****************************************************************************
//
//  COM_UART_TX pin: This pin is the COM_UART transmit pin.
//
//*****************************************************************************
const am_hal_gpio_pincfg_t g_AM_BSP_GPIO_COM_UART_TX =
{
    .uFuncSel            = AM_HAL_PIN_48_UART0TX,
    .eDriveStrength      = AM_HAL_GPIO_PIN_DRIVESTRENGTH_2MA
};

//*****************************************************************************
//
//  COM_UART_RX pin: This pin is the COM_UART receive pin.
//
//*****************************************************************************
const am_hal_gpio_pincfg_t g_AM_BSP_GPIO_COM_UART_RX =
{
    .uFuncSel            = AM_HAL_PIN_49_UART0RX
};

//*****************************************************************************
//
//  IOM0_CS pin: I/O Master 0 chip select.
//
//*****************************************************************************
const am_hal_gpio_pincfg_t g_AM_BSP_GPIO_IOM0_CS =
{
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
const am_hal_gpio_pincfg_t g_AM_BSP_GPIO_IOM0_CS3 =
{
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
const am_hal_gpio_pincfg_t g_AM_BSP_GPIO_IOM0_MISO =
{
    .uFuncSel            = AM_HAL_PIN_6_M0MISO,
    .uIOMnum             = 0
};

//*****************************************************************************
//
//  IOM0_MOSI pin: I/O Master 0 SPI MOSI signal.
//
//*****************************************************************************
const am_hal_gpio_pincfg_t g_AM_BSP_GPIO_IOM0_MOSI =
{
    .uFuncSel            = AM_HAL_PIN_7_M0MOSI,
    .eDriveStrength      = AM_HAL_GPIO_PIN_DRIVESTRENGTH_12MA,
    .uIOMnum             = 0
};

//*****************************************************************************
//
//  IOM0_SCK pin: I/O Master 0 SPI SCK signal.
//
//*****************************************************************************
const am_hal_gpio_pincfg_t g_AM_BSP_GPIO_IOM0_SCK =
{
    .uFuncSel            = AM_HAL_PIN_5_M0SCK,
    .eDriveStrength      = AM_HAL_GPIO_PIN_DRIVESTRENGTH_12MA,
    .uIOMnum             = 0
};

//*****************************************************************************
//
//  IOM0_SCL pin: I/O Master 0 I2C clock signal.
//
//*****************************************************************************
const am_hal_gpio_pincfg_t g_AM_BSP_GPIO_IOM0_SCL =
{
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
const am_hal_gpio_pincfg_t g_AM_BSP_GPIO_IOM0_SDA =
{
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
const am_hal_gpio_pincfg_t g_AM_BSP_GPIO_IOM1_CS =
{
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
const am_hal_gpio_pincfg_t g_AM_BSP_GPIO_IOM1_MISO =
{
    .uFuncSel            = AM_HAL_PIN_9_M1MISO,
    .uIOMnum             = 1
};

//*****************************************************************************
//
//  IOM1_MOSI pin: I/O Master 1 SPI MOSI signal.
//
//*****************************************************************************
const am_hal_gpio_pincfg_t g_AM_BSP_GPIO_IOM1_MOSI =
{
    .uFuncSel            = AM_HAL_PIN_10_M1MOSI,
    .eDriveStrength      = AM_HAL_GPIO_PIN_DRIVESTRENGTH_12MA,
    .uIOMnum             = 1
};

//*****************************************************************************
//
//  IOM1_SCK pin: I/O Master 1 SPI SCK signal.
//
//*****************************************************************************
const am_hal_gpio_pincfg_t g_AM_BSP_GPIO_IOM1_SCK =
{
    .uFuncSel            = AM_HAL_PIN_8_M1SCK,
    .eDriveStrength      = AM_HAL_GPIO_PIN_DRIVESTRENGTH_12MA,
    .uIOMnum             = 1
};

//*****************************************************************************
//
//  IOM1_SCL pin: I/O Master 1 I2C clock signal.
//
//*****************************************************************************
const am_hal_gpio_pincfg_t g_AM_BSP_GPIO_IOM1_SCL =
{
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
const am_hal_gpio_pincfg_t g_AM_BSP_GPIO_IOM1_SDA =
{
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
const am_hal_gpio_pincfg_t g_AM_BSP_GPIO_IOM2_CS =
{
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
const am_hal_gpio_pincfg_t g_AM_BSP_GPIO_IOM2_MISO =
{
    .uFuncSel            = AM_HAL_PIN_25_M2MISO,
    .uIOMnum             = 2
};

//*****************************************************************************
//
//  IOM2_MOSI pin: I/O Master 2 SPI MOSI signal.
//
//*****************************************************************************
const am_hal_gpio_pincfg_t g_AM_BSP_GPIO_IOM2_MOSI =
{
    .uFuncSel            = AM_HAL_PIN_28_M2MOSI,
    .eDriveStrength      = AM_HAL_GPIO_PIN_DRIVESTRENGTH_12MA,
    .uIOMnum             = 2
};

//*****************************************************************************
//
//  IOM2_SCK pin: I/O Master 2 SPI SCK signal.
//
//*****************************************************************************
const am_hal_gpio_pincfg_t g_AM_BSP_GPIO_IOM2_SCK =
{
    .uFuncSel            = AM_HAL_PIN_27_M2SCK,
    .eDriveStrength      = AM_HAL_GPIO_PIN_DRIVESTRENGTH_12MA,
    .uIOMnum             = 2
};

//*****************************************************************************
//
//  IOM2_SCL pin: I/O Master 2 I2C clock signal.
//
//*****************************************************************************
const am_hal_gpio_pincfg_t g_AM_BSP_GPIO_IOM2_SCL =
{
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
const am_hal_gpio_pincfg_t g_AM_BSP_GPIO_IOM2_SDA =
{
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
const am_hal_gpio_pincfg_t g_AM_BSP_GPIO_IOM3_CS =
{
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
const am_hal_gpio_pincfg_t g_AM_BSP_GPIO_IOM3_MISO =
{
    .uFuncSel            = AM_HAL_PIN_43_M3MISO,
    .uIOMnum             = 3
};

//*****************************************************************************
//
//  IOM3_MOSI pin: I/O Master 3 SPI MOSI signal.
//
//*****************************************************************************
const am_hal_gpio_pincfg_t g_AM_BSP_GPIO_IOM3_MOSI =
{
    .uFuncSel            = AM_HAL_PIN_38_M3MOSI,
    .eDriveStrength      = AM_HAL_GPIO_PIN_DRIVESTRENGTH_12MA,
    .uIOMnum             = 3
};

//*****************************************************************************
//
//  IOM3_SCK pin: I/O Master 3 SPI SCK signal.
//
//*****************************************************************************
const am_hal_gpio_pincfg_t g_AM_BSP_GPIO_IOM3_SCK =
{
    .uFuncSel            = AM_HAL_PIN_42_M3SCK,
    .eDriveStrength      = AM_HAL_GPIO_PIN_DRIVESTRENGTH_12MA,
    .uIOMnum             = 3
};

//*****************************************************************************
//
//  IOM3_SCL pin: I/O Master 3 I2C clock signal.
//
//*****************************************************************************
const am_hal_gpio_pincfg_t g_AM_BSP_GPIO_IOM3_SCL =
{
    .uFuncSel            = AM_HAL_PIN_42_M3SCL,
    .ePullup             = AM_HAL_GPIO_PIN_PULLUP_1_5K,
    .eDriveStrength      = AM_HAL_GPIO_PIN_DRIVESTRENGTH_12MA,
    .eGPOutcfg           = AM_HAL_GPIO_PIN_OUTCFG_OPENDRAIN,
    .uIOMnum             = 3
};

//*****************************************************************************
//
//  IOM3_SDA pin: I/O Master 3 I2C data signal.
//
//*****************************************************************************
const am_hal_gpio_pincfg_t g_AM_BSP_GPIO_IOM3_SDA =
{
    .uFuncSel            = AM_HAL_PIN_43_M3SDAWIR3,
    .ePullup             = AM_HAL_GPIO_PIN_PULLUP_1_5K,
    .eDriveStrength      = AM_HAL_GPIO_PIN_DRIVESTRENGTH_12MA,
    .eGPOutcfg           = AM_HAL_GPIO_PIN_OUTCFG_OPENDRAIN,
    .uIOMnum             = 3
};

//*****************************************************************************
//
//  IOM4_CS pin: I/O Master 4 chip select.
//
//*****************************************************************************
const am_hal_gpio_pincfg_t g_AM_BSP_GPIO_IOM4_CS =
{
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
const am_hal_gpio_pincfg_t g_AM_BSP_GPIO_IOM4_MISO =
{
    .uFuncSel            = AM_HAL_PIN_40_M4MISO,
    .uIOMnum             = 4
};

//*****************************************************************************
//
//  IOM4_MOSI pin: I/O Master 4 SPI MOSI signal.
//
//*****************************************************************************
const am_hal_gpio_pincfg_t g_AM_BSP_GPIO_IOM4_MOSI =
{
    .uFuncSel            = AM_HAL_PIN_44_M4MOSI,
    .eDriveStrength      = AM_HAL_GPIO_PIN_DRIVESTRENGTH_12MA,
    .uIOMnum             = 4
};

//*****************************************************************************
//
//  IOM4_SCK pin: I/O Master 4 SPI SCK signal.
//
//*****************************************************************************
const am_hal_gpio_pincfg_t g_AM_BSP_GPIO_IOM4_SCK =
{
    .uFuncSel            = AM_HAL_PIN_39_M4SCK,
    .eDriveStrength      = AM_HAL_GPIO_PIN_DRIVESTRENGTH_12MA,
    .uIOMnum             = 4
};

//*****************************************************************************
//
//  IOM4_SCL pin: I/O Master 4 I2C clock signal.
//
//*****************************************************************************
const am_hal_gpio_pincfg_t g_AM_BSP_GPIO_IOM4_SCL =
{
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
const am_hal_gpio_pincfg_t g_AM_BSP_GPIO_IOM4_SDA =
{
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
const am_hal_gpio_pincfg_t g_AM_BSP_GPIO_IOM5_CS =
{
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
const am_hal_gpio_pincfg_t g_AM_BSP_GPIO_IOM5_MISO =
{
    .uFuncSel            = AM_HAL_PIN_49_M5MISO,
    .uIOMnum             = 5
};

//*****************************************************************************
//
//  IOM5_MOSI pin: I/O Master 5 SPI MOSI signal.
//
//*****************************************************************************
const am_hal_gpio_pincfg_t g_AM_BSP_GPIO_IOM5_MOSI =
{
    .uFuncSel            = AM_HAL_PIN_47_M5MOSI,
    .eDriveStrength      = AM_HAL_GPIO_PIN_DRIVESTRENGTH_12MA,
    .uIOMnum             = 5
};

//*****************************************************************************
//
//  IOM5_SCK pin: I/O Master 5 SPI SCK signal.
//
//*****************************************************************************
const am_hal_gpio_pincfg_t g_AM_BSP_GPIO_IOM5_SCK =
{
    .uFuncSel            = AM_HAL_PIN_48_M5SCK,
    .eDriveStrength      = AM_HAL_GPIO_PIN_DRIVESTRENGTH_12MA,
    .uIOMnum             = 5
};

//*****************************************************************************
//
//  IOM5_SCL pin: I/O Master 5 I2C clock signal.
//
//*****************************************************************************
const am_hal_gpio_pincfg_t g_AM_BSP_GPIO_IOM5_SCL =
{
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
const am_hal_gpio_pincfg_t g_AM_BSP_GPIO_IOM5_SDA =
{
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
const am_hal_gpio_pincfg_t g_AM_BSP_GPIO_MSPI_CE0 =
{
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
const am_hal_gpio_pincfg_t g_AM_BSP_GPIO_MSPI_CE1 =
{
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
const am_hal_gpio_pincfg_t g_AM_BSP_GPIO_MSPI_D0 =
{
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
const am_hal_gpio_pincfg_t g_AM_BSP_GPIO_MSPI_D1 =
{
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
const am_hal_gpio_pincfg_t g_AM_BSP_GPIO_MSPI_D2 =
{
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
const am_hal_gpio_pincfg_t g_AM_BSP_GPIO_MSPI_D3 =
{
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
const am_hal_gpio_pincfg_t g_AM_BSP_GPIO_MSPI_D4 =
{
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
const am_hal_gpio_pincfg_t g_AM_BSP_GPIO_MSPI_D5 =
{
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
const am_hal_gpio_pincfg_t g_AM_BSP_GPIO_MSPI_D6 =
{
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
const am_hal_gpio_pincfg_t g_AM_BSP_GPIO_MSPI_D7 =
{
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
const am_hal_gpio_pincfg_t g_AM_BSP_GPIO_MSPI_SCK =
{
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
const am_hal_gpio_pincfg_t g_AM_BSP_GPIO_IOS_CE =
{
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
const am_hal_gpio_pincfg_t g_AM_BSP_GPIO_IOS_MISO =
{
    .uFuncSel            = AM_HAL_PIN_2_SLMISO,
    .eDriveStrength      = AM_HAL_GPIO_PIN_DRIVESTRENGTH_12MA
};

//*****************************************************************************
//
//  IOS_MOSI pin: I/O Slave SPI MOSI signal.
//
//*****************************************************************************
const am_hal_gpio_pincfg_t g_AM_BSP_GPIO_IOS_MOSI =
{
    .uFuncSel            = AM_HAL_PIN_1_SLMOSI,
    .eGPInput            = AM_HAL_GPIO_PIN_INPUT_ENABLE
};

//*****************************************************************************
//
//  IOS_SCK pin: I/O Slave SPI SCK signal.
//
//*****************************************************************************
const am_hal_gpio_pincfg_t g_AM_BSP_GPIO_IOS_SCK =
{
    .uFuncSel            = AM_HAL_PIN_0_SLSCK,
    .eGPInput            = AM_HAL_GPIO_PIN_INPUT_ENABLE
};

//*****************************************************************************
//
//  IOS_SCL pin: I/O Slave I2C clock signal.
//
//*****************************************************************************
const am_hal_gpio_pincfg_t g_AM_BSP_GPIO_IOS_SCL =
{
    .uFuncSel            = AM_HAL_PIN_0_SLSCL,
    .eGPInput            = AM_HAL_GPIO_PIN_INPUT_ENABLE
};

//*****************************************************************************
//
//  IOS_SDA pin: I/O Slave I2C data signal.
//
//*****************************************************************************
const am_hal_gpio_pincfg_t g_AM_BSP_GPIO_IOS_SDA =
{
    .uFuncSel            = AM_HAL_PIN_1_SLSDAWIR3,
    .ePullup             = AM_HAL_GPIO_PIN_PULLUP_1_5K,
    .eGPOutcfg           = AM_HAL_GPIO_PIN_OUTCFG_OPENDRAIN
};

//*****************************************************************************
//
//  ITM_SWO pin: ITM Serial Wire Output.
//
//*****************************************************************************
const am_hal_gpio_pincfg_t g_AM_BSP_GPIO_ITM_SWO =
{
    .uFuncSel            = AM_HAL_PIN_33_SWO,
    .eDriveStrength      = AM_HAL_GPIO_PIN_DRIVESTRENGTH_2MA
};

//*****************************************************************************
//
//  SWDCK pin: Cortex Serial Wire DCK.
//
//*****************************************************************************
const am_hal_gpio_pincfg_t g_AM_BSP_GPIO_SWDCK =
{
    .uFuncSel            = AM_HAL_PIN_20_SWDCK
};

//*****************************************************************************
//
//  SWDIO pin: Cortex Serial Wire DIO.
//
//*****************************************************************************
const am_hal_gpio_pincfg_t g_AM_BSP_GPIO_SWDIO =
{
    .uFuncSel            = AM_HAL_PIN_21_SWDIO
};

//*****************************************************************************
//
// End Doxygen group.
//! @}
//
//*****************************************************************************
