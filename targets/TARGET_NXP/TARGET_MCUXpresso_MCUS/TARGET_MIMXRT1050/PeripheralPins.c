/* mbed Microcontroller Library
 * Copyright (c) 2006-2013 ARM Limited
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

#include "PeripheralPins.h"

// Note on MIMXRT pin functions:
// The MIMXRT's pin function system is pretty dang complicated, and Mbed's pin function data for this chip
// (the 3rd element in each pinmap entry) has to be pretty complicated to match.
// It's a 32-bit bitfield with the following format:
// __________________________________________________________________________________________________
// |                               |                            |              |                     |
// |  Daisy Reg Value (bits 19:16) | Daisy Register (bits 15:4) | SION (bit 3) | Mux Mode (bits 2:0) |
// |_______________________________|____________________________|______________|_____________________|
//
// Mux mode:
//    This value gets written to the IOMUXC_SW_MUX_CTL_PAD_xxx.MUX_MODE bitfield for the given pin.
//    It's a number from 0 to 7 that selects the possible mux mode.
//    See Table 10-1 in the datasheet for the possible muxing options
//
// SION:
//    This is a somewhat unusual setting used to "force the pin mode to input regardless of MUX_MODE
//    functionality".  It's a setting needed for certain peripherals to work that use pins in input mode.
//    I'm not quite sure of the logic for when to use it...
//
// Daisy Register:
//    If nonzero, this field specifies the offset for a "daisy chain register" to use when setting up the pin
//    function.  "Daisy chain" is actually kinda a misnomer, this register is used to select which of multiple
//    pin options a peripheral is connected to, it doesn't daisy chain pins together.  It would be better to
//    call it "top-level mux register" or something.
//
//    The daisy register is specified as an offset relative to the IOMUXC peripheral base.  For example, for the
//    LPI2C3 peripheral, the daisy chain register for SCL is IOMUXC_LPI2C3_SDA_SELECT_INPUT.  So, since the address
//    of that register is IOMUXC + 0x4E0, I'd put 0x4E0 as the daisy register.
//
// Daisy Reg Value:
//    Numeric option to select in the above daisy register, if the address is given.
//

/************RTC***************/
const PinMap PinMap_RTC[] = {
    {NC, OSC32KCLK, 0},
};

/************ADC***************/
const PinMap PinMap_ADC[] = {
    {GPIO_AD_B1_10, ADC1_15, 5},
    {GPIO_AD_B1_11, ADC2_0,  5},
    {GPIO_AD_B1_04, ADC1_9,  5},
    {GPIO_AD_B1_05, ADC1_10, 5},
    {GPIO_AD_B1_01, ADC1_6,  5},
    {GPIO_AD_B1_00, ADC1_5,  5},
    {NC   , NC       , 0}
};

/************DAC***************/
const PinMap PinMap_DAC[] = {
    {NC      , NC   , 0}
};

/************I2C***************/
const PinMap PinMap_I2C_SDA[] = {
    {GPIO_SD_B1_05, I2C_1, ((0U << DAISY_REG_VALUE_SHIFT) | (0x4D0 << DAISY_REG_SHIFT) | (1U << SION_BIT_SHIFT) | 2)},
    {GPIO_AD_B1_01, I2C_1, ((1U << DAISY_REG_VALUE_SHIFT) | (0x4D0 << DAISY_REG_SHIFT) | (1U << SION_BIT_SHIFT) | 3)},

    {GPIO_B0_05, I2C_2, ((1U << DAISY_REG_VALUE_SHIFT) | (0x4D8 << DAISY_REG_SHIFT) | (1U << SION_BIT_SHIFT) | 2)},
    {GPIO_SD_B1_10, I2C_2, ((0U << DAISY_REG_VALUE_SHIFT) | (0x4D8 << DAISY_REG_SHIFT) | (1U << SION_BIT_SHIFT) | 3)},

    {GPIO_AD_B1_06, I2C_3, ((2U << DAISY_REG_VALUE_SHIFT) | (0x4E0 << DAISY_REG_SHIFT) | (1U << SION_BIT_SHIFT) | 1)},
    {GPIO_EMC_21, I2C_3, ((0U << DAISY_REG_VALUE_SHIFT) | (0x4E0 << DAISY_REG_SHIFT) | (1U << SION_BIT_SHIFT) | 2)},
    {GPIO_SD_B0_01, I2C_3, ((1U << DAISY_REG_VALUE_SHIFT) | (0x4E0 << DAISY_REG_SHIFT) | (1U << SION_BIT_SHIFT) | 2)},

    {GPIO_EMC_11, I2C_4, ((0U << DAISY_REG_VALUE_SHIFT) | (0x4E8 << DAISY_REG_SHIFT) | (1U << SION_BIT_SHIFT) | 2)},
    {GPIO_AD_B0_13, I2C_4, ((1U << DAISY_REG_VALUE_SHIFT) | (0x4E8 << DAISY_REG_SHIFT) | (1U << SION_BIT_SHIFT) | 0)},

    {NC   , NC   , 0}
};

const PinMap PinMap_I2C_SCL[] = {
    {GPIO_SD_B1_04, I2C_1, ((0U << DAISY_REG_VALUE_SHIFT) | (0x4CC << DAISY_REG_SHIFT) | (1U << SION_BIT_SHIFT) | 2)},
    {GPIO_AD_B1_00, I2C_1, ((1U << DAISY_REG_VALUE_SHIFT) | (0x4CC << DAISY_REG_SHIFT) | (1U << SION_BIT_SHIFT) | 3)},

    {GPIO_SD_B1_11, I2C_2, ((0U << DAISY_REG_VALUE_SHIFT) | (0x4D4 << DAISY_REG_SHIFT) | (1U << SION_BIT_SHIFT) | 3)},
    {GPIO_B0_04, I2C_2, ((1U << DAISY_REG_VALUE_SHIFT) | (0x4D4 << DAISY_REG_SHIFT) | (1U << SION_BIT_SHIFT) | 2)},

    {GPIO_EMC_22, I2C_3, ((0U << DAISY_REG_VALUE_SHIFT) | (0x4DC << DAISY_REG_SHIFT) | (1U << SION_BIT_SHIFT) | 2)},
    {GPIO_SD_B0_00, I2C_3, ((1U << DAISY_REG_VALUE_SHIFT) | (0x4DC << DAISY_REG_SHIFT) | (1U << SION_BIT_SHIFT) | 2)},
    {GPIO_AD_B1_07, I2C_3, ((2U << DAISY_REG_VALUE_SHIFT) | (0x4DC << DAISY_REG_SHIFT) | (1U << SION_BIT_SHIFT) | 1)},

    {GPIO_EMC_12, I2C_4, ((0U << DAISY_REG_VALUE_SHIFT) | (0x4E4 << DAISY_REG_SHIFT) | (1U << SION_BIT_SHIFT) | 2)},
    {GPIO_AD_B0_12, I2C_4, ((1U << DAISY_REG_VALUE_SHIFT) | (0x4E4 << DAISY_REG_SHIFT) | (1U << SION_BIT_SHIFT) | 0)},

    {NC   , NC   , 0}
};

/************UART***************/
const PinMap PinMap_UART_TX[] = {
    {GPIO_AD_B0_12, UART_1, 2},
    {GPIO_AD_B1_06, UART_3, ((0 << DAISY_REG_VALUE_SHIFT) | (0x53C << DAISY_REG_SHIFT) | 2)},
    {GPIO_AD_B0_02, UART_6, ((1 << DAISY_REG_VALUE_SHIFT) | (0x554 << DAISY_REG_SHIFT) | 2)},
    {NC  ,  NC    , 0}
};

const PinMap PinMap_UART_RX[] = {
    {GPIO_AD_B0_13, UART_1, 2},
    {GPIO_AD_B1_07, UART_3, ((0 << DAISY_REG_VALUE_SHIFT) | (0x538 << DAISY_REG_SHIFT) | 2)},
    {GPIO_AD_B0_03, UART_6, ((1 << DAISY_REG_VALUE_SHIFT) | (0x550 << DAISY_REG_SHIFT) | 2)},
    {NC  ,  NC    , 0}
};

/************SPI***************/
const PinMap PinMap_SPI_SCLK[] = {
    {GPIO_SD_B0_00, SPI_1, ((1U << DAISY_REG_VALUE_SHIFT) | (0x4F0 << DAISY_REG_SHIFT) | 4)},
    {GPIO_AD_B0_00, SPI_3, ((0U << DAISY_REG_VALUE_SHIFT) | (0x510 << DAISY_REG_SHIFT) | 7)},
    {GPIO_B0_03,    SPI_4, ((0U << DAISY_REG_VALUE_SHIFT) | (0x520 << DAISY_REG_SHIFT) | 3)},
    {NC   , NC   , 0}
};

const PinMap PinMap_SPI_MOSI[] = {
    {GPIO_SD_B0_02, SPI_1, ((1U << DAISY_REG_VALUE_SHIFT) | (0x4F8 << DAISY_REG_SHIFT) | 4)},
    {GPIO_AD_B0_01, SPI_3, ((0U << DAISY_REG_VALUE_SHIFT) | (0x518 << DAISY_REG_SHIFT) | 7)},
    {GPIO_B0_02,    SPI_4, ((0U << DAISY_REG_VALUE_SHIFT) | (0x528 << DAISY_REG_SHIFT) | 3)},
    {NC   , NC   , 0}
};

const PinMap PinMap_SPI_MISO[] = {
    {GPIO_SD_B0_03, SPI_1, ((1U << DAISY_REG_VALUE_SHIFT) | (0x4F4 << DAISY_REG_SHIFT) | 4)},
    {GPIO_AD_B0_02, SPI_3, ((0U << DAISY_REG_VALUE_SHIFT) | (0x514 << DAISY_REG_SHIFT) | 7)},
    {GPIO_B0_01,    SPI_4, ((0U << DAISY_REG_VALUE_SHIFT) | (0x524 << DAISY_REG_SHIFT) | 3)},
    {NC   , NC   , 0}
};

const PinMap PinMap_SPI_SSEL[] = {
    {GPIO_SD_B0_01, SPI_1, ((0U << DAISY_REG_VALUE_SHIFT) | (0x4EC << DAISY_REG_SHIFT) | 4)},
    {GPIO_AD_B0_03, SPI_3, ((0U << DAISY_REG_VALUE_SHIFT) | (0x50C << DAISY_REG_SHIFT) | 7)},
    {GPIO_B0_00,    SPI_4, ((0U << DAISY_REG_VALUE_SHIFT) | (0x51C << DAISY_REG_SHIFT) | 3)},
    {NC   , NC   , 0}
};

/************PWM***************/
const PinMap PinMap_PWM[] = {
    {GPIO_AD_B0_10, PWM_7, ((3U << DAISY_REG_VALUE_SHIFT) | (0x454 << DAISY_REG_SHIFT) | 1)},
    {GPIO_AD_B0_11, PWM_8, ((3U << DAISY_REG_VALUE_SHIFT) | (0x464 << DAISY_REG_SHIFT) | 1)},
    {GPIO_AD_B1_08, PWM_25, ((1U << DAISY_REG_VALUE_SHIFT) | (0x494 << DAISY_REG_SHIFT) | 1)},
    {GPIO_SD_B0_00, PWM_1, ((1U << DAISY_REG_VALUE_SHIFT) | (0x458 << DAISY_REG_SHIFT) | 1)},
    {GPIO_SD_B0_01, PWM_2, ((1U << DAISY_REG_VALUE_SHIFT) | (0x468 << DAISY_REG_SHIFT) | 1)},
    {NC   , NC    , 0}
};

