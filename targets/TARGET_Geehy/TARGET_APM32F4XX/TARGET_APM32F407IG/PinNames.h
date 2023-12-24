/* mbed Microcontroller Library
 * Copyright (c) 2006-2023 ARM Limited
 * Copyright (c) 2023 Geehy Semiconductor Inc.
 *
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

/* MBED TARGET LIST: APM32_F407IG */

#ifndef MBED_PINNAMES_H
#define MBED_PINNAMES_H

#include "cmsis.h"

#ifdef __cplusplus
extern "C" {
#endif

/* multiplex GPIO flag */
typedef enum {
    MUL0  = 0x100,
    MUL1  = 0x200,
    MUL2  = 0x300,
    MUL3  = 0x400
} MULx;

typedef enum {
    PA_0  = 0x00,
    PA_0_MUL0 = PA_0 | MUL0,
    PA_0_MUL1 = PA_0 | MUL1,
    PA_1  = 0x01,
    PA_1_MUL0 = PA_1 | MUL0,
    PA_1_MUL1 = PA_1 | MUL1,
    PA_2  = 0x02,
    PA_2_MUL0 = PA_2 | MUL0,
    PA_2_MUL1 = PA_2 | MUL1,
    PA_3  = 0x03,
    PA_3_MUL0 = PA_3 | MUL0,
    PA_3_MUL1 = PA_3 | MUL1,
    PA_4  = 0x04,
    PA_4_MUL0 = PA_4 | MUL0,
    PA_5  = 0x05,
    PA_5_MUL0 = PA_5 | MUL0,
    PA_6  = 0x06,
    PA_6_MUL0 = PA_6 | MUL0,
    PA_6_MUL1 = PA_6 | MUL1,
    PA_6_MUL2 = PA_6 | MUL2,
    PA_7  = 0x07,
    PA_7_MUL0 = PA_7 | MUL0,
    PA_7_MUL1 = PA_7 | MUL1,
    PA_7_MUL2 = PA_7 | MUL2,
    PA_8  = 0x08,
    PA_9  = 0x09,
    PA_10 = 0x0A,
    PA_11 = 0x0B,
    PA_12 = 0x0C,
    PA_13 = 0x0D,
    PA_14 = 0x0E,
    PA_15 = 0x0F,
    PA_15_MUL0 = PA_15 | MUL0,

    PB_0  = 0x10,
    PB_0_MUL0 = PB_0 | MUL0,
    PB_0_MUL1 = PB_0 | MUL1,
    PB_1  = 0x11,
    PB_1_MUL0 = PB_1 | MUL0,
    PB_1_MUL1 = PB_1 | MUL1,
    PB_2  = 0x12,
    PB_3  = 0x13,
    PB_3_MUL0 = PB_3 | MUL0,
    PB_4  = 0x14,
    PB_4_MUL0 = PB_4 | MUL0,
    PB_5  = 0x15,
    PB_5_MUL0 = PB_5 | MUL0,
    PB_6  = 0x16,
    PB_7  = 0x17,
    PB_8  = 0x18,
    PB_8_MUL0 = PB_8 | MUL0,
    PB_9  = 0x19,
    PB_9_MUL0 = PB_9 | MUL0,
    PB_10 = 0x1A,
    PB_11 = 0x1B,
    PB_12 = 0x1C,
    PB_13 = 0x1D,
    PB_14 = 0x1E,
    PB_14_MUL0 = PB_14 | MUL0,
    PB_14_MUL1 = PB_14 | MUL1,
    PB_15 = 0x1F,
    PB_15_MUL0 = PB_15 | MUL0,
    PB_15_MUL1 = PB_15 | MUL1,

    PC_0  = 0x20,
    PC_0_MUL0 = PC_0 | MUL0,
    PC_0_MUL1 = PC_0 | MUL1,
    PC_1  = 0x21,
    PC_1_MUL0 = PC_1 | MUL0,
    PC_1_MUL1 = PC_1 | MUL1,
    PC_2  = 0x22,
    PC_2_MUL0 = PC_2 | MUL0,
    PC_2_MUL1 = PC_2 | MUL1,
    PC_3  = 0x23,
    PC_3_MUL0 = PC_3 | MUL0,
    PC_3_MUL1 = PC_3 | MUL1,
    PC_4  = 0x24,
    PC_4_MUL0 = PC_4 | MUL0,
    PC_5  = 0x25,
    PC_5_MUL0 = PC_5 | MUL0,
    PC_6  = 0x26,
    PC_6_MUL0 = PC_6 | MUL0,
    PC_7  = 0x27,
    PC_7_MUL0 = PC_7 | MUL0,
    PC_8  = 0x28,
    PC_8_MUL0 = PC_8 | MUL0,
    PC_9  = 0x29,
    PC_9_MUL0 = PC_9 | MUL0,
    PC_10 = 0x2A,
    PC_10_MUL0 = PC_10 | MUL0,
    PC_11 = 0x2B,
    PC_11_MUL0 = PC_11 | MUL0,
    PC_12 = 0x2C,
    PC_13 = 0x2D,
    PC_14 = 0x2E,
    PC_15 = 0x2F,

    PD_0  = 0x30,
    PD_1  = 0x31,
    PD_2  = 0x32,
    PD_3  = 0x33,
    PD_4  = 0x34,
    PD_5  = 0x35,
    PD_6  = 0x36,
    PD_7  = 0x37,
    PD_8  = 0x38,
    PD_9  = 0x39,
    PD_10 = 0x3A,
    PD_11 = 0x3B,
    PD_12 = 0x3C,
    PD_13 = 0x3D,
    PD_14 = 0x3E,
    PD_15 = 0x3F,

    PE_0  = 0x40,
    PE_1  = 0x41,
    PE_2  = 0x42,
    PE_3  = 0x43,
    PE_4  = 0x44,
    PE_5  = 0x45,
    PE_6  = 0x46,
    PE_7  = 0x47,
    PE_8  = 0x48,
    PE_9  = 0x49,
    PE_10 = 0x4A,
    PE_11 = 0x4B,
    PE_12 = 0x4C,
    PE_13 = 0x4D,
    PE_14 = 0x4E,
    PE_15 = 0x4F,

    PF_0  = 0x50,
    PF_1  = 0x51,
    PF_2  = 0x52,
    PF_3  = 0x53,
    PF_4  = 0x54,
    PF_4_MUL0 = PF_4 | MUL0,
    PF_5  = 0x55,
    PF_5_MUL0 = PF_5 | MUL0,
    PF_6  = 0x56,
    PF_6_MUL0 = PF_6 | MUL0,
    PF_7  = 0x57,
    PF_7_MUL0 = PF_7 | MUL0,
    PF_8  = 0x58,
    PF_8_MUL0 = PF_8 | MUL0,
    PF_9  = 0x59,
    PF_9_MUL0 = PF_9 | MUL0,
    PF_10 = 0x5A,
    PF_10_MUL0 = PF_10 | MUL0,
    PF_11 = 0x5B,
    PF_12 = 0x5C,
    PF_13 = 0x5D,
    PF_14 = 0x5E,
    PF_15 = 0x5F,

    PG_0  = 0x60,
    PG_1  = 0x61,
    PG_2  = 0x62,
    PG_3  = 0x63,
    PG_4  = 0x64,
    PG_5  = 0x65,
    PG_6  = 0x66,
    PG_7  = 0x67,
    PG_8  = 0x68,
    PG_9  = 0x69,
    PG_10 = 0x6A,
    PG_11 = 0x6B,
    PG_12 = 0x6C,
    PG_13 = 0x6D,
    PG_14 = 0x6E,
    PG_15 = 0x6F,

    PH_0  = 0x70,
    PH_1  = 0x71,
    PH_2  = 0x72,
    PH_3  = 0x73,
    PH_4  = 0x74,
    PH_5  = 0x75,
    PH_6  = 0x76,
    PH_7  = 0x77,
    PH_8  = 0x78,
    PH_9  = 0x79,
    PH_10 = 0x7A,
    PH_11 = 0x7B,
    PH_12 = 0x7C,
    PH_13 = 0x7D,
    PH_14 = 0x7E,
    PH_15 = 0x7F,

    PI_0  = 0x80,
    PI_1  = 0x81,
    PI_2  = 0x82,
    PI_3  = 0x83,
    PI_4  = 0x84,
    PI_5  = 0x85,
    PI_6  = 0x86,
    PI_7  = 0x87,
    PI_8  = 0x88,
    PI_9  = 0x89,
    PI_10 = 0x8A,
    PI_11 = 0x8B,
    PI_12 = 0x8C,
    PI_13 = 0x8D,
    PI_14 = 0x8E,
    PI_15 = 0x8F,

    /* ADC internal channels */
    ADC_TEMP = 0xF0,
    ADC_VREF = 0xF1,
    ADC_VBAT = 0xF2,

    // /* Arduino connector pin definitions */
    ARDUINO_UNO_A0          = PC_3,
    ARDUINO_UNO_A1          = PA_0,
    ARDUINO_UNO_A2          = PA_3,
    ARDUINO_UNO_A3          = PA_4,
    ARDUINO_UNO_A4          = PA_5,      /* PORTB_11 */
    ARDUINO_UNO_A5          = PA_6,      /* PORTB_10 */
    ARDUINO_UNO_D0          = PC_7,
    ARDUINO_UNO_D1          = PC_6,
    ARDUINO_UNO_D2          = PC_8,
    ARDUINO_UNO_D3          = PC_9,
    ARDUINO_UNO_D4          = PA_8,
    ARDUINO_UNO_D5          = PA_11,
    ARDUINO_UNO_D6          = PH_13,
    ARDUINO_UNO_D7          = PA_12,
    ARDUINO_UNO_D8          = PH_14,
    ARDUINO_UNO_D9          = PH_15,
    ARDUINO_UNO_D10         = PI_0,      /* PORTI_0 */
    ARDUINO_UNO_D11         = PI_3,      /* PORTI_3 */
    ARDUINO_UNO_D12         = PI_2,
    ARDUINO_UNO_D13         = PI_1,
    ARDUINO_UNO_D14         = PB_7,
    ARDUINO_UNO_D15         = PB_6,

    LED2        = PE_6,
    LED3        = PE_5,

    KEY1        = PC_10,
    KEY2        = PC_11,

    BUTTON1     = KEY1,
    BUTTON2     = KEY2,

    SERIAL_TX   = PC_6,
    SERIAL_RX   = PC_7,
    CONSOLE_TX  = SERIAL_TX,
    CONSOLE_RX  = SERIAL_RX,

    USBFS_VBUS  = PA_9,
    USBFS_DM    = PA_11,
    USBFS_DP    = PA_12,
    USBFS_ID    = PA_10,

    USBHS_VBUS  = PB_13,
    USBHS_DM    = PB_14,
    USBHS_DP    = PB_15,
    USBHS_ID    = PB_12,

    NC = (int)0xFFFFFFFF
} PinName;

typedef enum {
    /* pin mode */
    PIN_MODE_INPUT  = 0,
    PIN_MODE_OUTPUT = 1,
    PIN_MODE_AF     = 2,
    PIN_MODE_ANALOG = 3,

    /* pin output type */
    PIN_OTYPE_PP = 0,
    PIN_OTYPE_OD = 1,

    /* pin pull up/down */
    PIN_PUPD_NONE     = 0,
    PIN_PUPD_PULLUP   = 1,
    PIN_PUPD_PULLDOWN = 2,

    /* pin output speed */
    PIN_OSPEED_200MHZ   = 1,
    PIN_OSPEED_50MHZ    = 0,    /* set 0 as 50M speed */
    PIN_OSPEED_25MHZ    = 2,
    PIN_OSPEED_2MHZ     = 3,

    /* pin AFs */
    PIN_AF_0    = 0,
    PIN_AF_1    = 1,
    PIN_AF_2    = 2,
    PIN_AF_3    = 3,
    PIN_AF_4    = 4,
    PIN_AF_5    = 5,
    PIN_AF_6    = 6,
    PIN_AF_7    = 7,
    PIN_AF_8    = 8,
    PIN_AF_9    = 9,
    PIN_AF_10   = 10,
    PIN_AF_11   = 11,
    PIN_AF_12   = 12,
    PIN_AF_13   = 13,
    PIN_AF_14   = 14,
    PIN_AF_15   = 15,
} PinFunction;

/* BIT[7:4] port number (0=PORTA, 1=PORTB, 2=PORTC, 3=PORTD, 4=PORTE, 5=PORTD, 6=PORTF, 7=PORTH, 8=PORTI)
   BIT[3:0] pin number */
#define APM_PORT_GET(X) (((uint32_t)(X) >> 4) & 0xF)
#define APM_PIN_GET(X)  (((uint32_t)(X) & 0xF))

/* pin function set and get define start */
/* Get mode, output mode, pull, speed, af function, channel of GPIO pin */
/* void pin_function(PinName pin, int function);
   configure the mode, output mode, pull, speed, af function of pins
   the parameter function contains the configuration information,show as below
   bit 0:2     gpio mode input / output / af / analog
   bit 3       output push-pull / open drain
   bit 5:4     no pull, pull-up, pull-down
   bit 9:6     channel af function
   bit 11:10   gpio speed
   bit 16:12   channel of adc/timer/dac
   bit 17      PWM channel-ON
   bit 31:18   reserved
*/
typedef enum {
    /* pin mode */
    PIN_MODE_SHIFT = 0,
    PIN_MODE_MASK  = 0x07,

    /* pin oty */
    PIN_OUTPUT_MODE_SHIFT = 3,
    PIN_OUTPUT_MODE_MASK = 0x01,

    /* pin pull_up_down */
    PIN_PULL_SHIFT = 4,
    PIN_PULL_MASK  = 0x03,

    /* pin seed */
    PIN_SPEED_SHIFT = 10,
    PIN_SPEED_MASK = 0x03,

    /* pin AF */
    PIN_REMAP_SHIFT = 6,
    PIN_REMAP_MASK  = 0x0F,

    /* pin channel */
    PIN_CHANNEL_SHIFT = 12,
    PIN_CHANNEL_MASK  = 0x1F,

    /* pin PWM channel-ON state */
    PIN_CHON_SHIFT = 17,
    PIN_CHON_MASK = 0x01,
} PinFunctionDivide;

#define SET_PIN_FUNCTION_MODE(PIN_FUNCTION_MODE)        ((int) PIN_FUNCTION_MODE&PIN_MODE_MASK)
#define SET_PIN_FUNCTION_SPEED(PIN_FUNCTION_SPEED)      ((int)(PIN_FUNCTION_SPEED&PIN_SPEED_MASK)<<PIN_SPEED_SHIFT)
#define SET_PIN_FUNCTION_AF(PIN_FUNCTION_AF)            ((int)(PIN_FUNCTION_AF&PIN_REMAP_MASK)<<PIN_REMAP_SHIFT)
#define SET_PIN_FUNCTION_CHANNEL(PIN_FUNCTION_CHANNEL)  ((int)(PIN_FUNCTION_CHANNEL&PIN_CHANNEL_MASK)<<PIN_CHANNEL_SHIFT)

/* configure ADC channel */
#define SET_PIN_FUNCTION_ADC_CHANNEL(PIN_FUNCTION_CHANNEL) (SET_PIN_FUNCTION_CHANNEL(PIN_FUNCTION_CHANNEL) | \
                                                            SET_PIN_FUNCTION_MODE(PIN_MODE_ANALOG) )
#define SET_PIN_FUNCTION_DAC_CHANNEL SET_PIN_FUNCTION_ADC_CHANNEL

#define SET_PIN_FUNCTION(PIN_FUNCTION_MODE, PIN_FUNCTION_OUTPUT_MODE, PIN_FUNCTION_PULL, PIN_FUNCTION_REMAP) \
                                                        ((int)(PIN_FUNCTION_MODE&PIN_MODE_MASK)| \
                                                        ((PIN_FUNCTION_OUTPUT_MODE&PIN_OUTPUT_MODE_MASK)<<PIN_OUTPUT_MODE_SHIFT) | \
                                                        ((PIN_FUNCTION_PULL&PIN_PULL_MASK)<<PIN_PULL_SHIFT) | \
                                                        ((PIN_FUNCTION_REMAP&PIN_REMAP_MASK)<<PIN_REMAP_SHIFT))

#define SET_PWM_PIN_FUNCTION(PIN_FUNCTION_MODE, PIN_FUNCTION_OUTPUT_MODE, PIN_FUNCTION_PULL, PIN_FUNCTION_REMAP, CHANNEL, CHON) \
                                                        ((int)(PIN_FUNCTION_MODE&PIN_MODE_MASK) | \
                                                         ((PIN_FUNCTION_OUTPUT_MODE&PIN_OUTPUT_MODE_MASK)<<PIN_OUTPUT_MODE_SHIFT) | \
                                                         ((PIN_FUNCTION_PULL&PIN_PULL_MASK)<<PIN_PULL_SHIFT) | \
                                                         ((PIN_FUNCTION_REMAP&PIN_REMAP_MASK)<<PIN_REMAP_SHIFT) | \
                                                         ((CHANNEL&PIN_CHANNEL_MASK)<<PIN_CHANNEL_SHIFT) | \
                                                         ((CHON&PIN_CHON_MASK)<<PIN_CHON_SHIFT))

#define APM_PIN_MODE_GET(X)              ( X & PIN_MODE_MASK)
#define APM_PIN_OUTPUT_MODE_GET(X)       ((X >> PIN_OUTPUT_MODE_SHIFT) & PIN_OUTPUT_MODE_MASK)
#define APM_PIN_PULL_STATE_GET(X)        ((X >> PIN_PULL_SHIFT) & PIN_PULL_MASK)
#define APM_PIN_SPEED_GET(X)             ((X >> PIN_SPEED_SHIFT) & PIN_SPEED_MASK)
#define APM_PIN_REMAP_GET(X)             ((X >> PIN_REMAP_SHIFT) & PIN_REMAP_MASK)
#define APM_PIN_CHANNEL_GET(X)           ((X >> PIN_CHANNEL_SHIFT) & PIN_CHANNEL_MASK)
#define APM_PIN_CHON_GET(X)              ((X >> PIN_CHON_SHIFT) & PIN_CHON_MASK)

/* pin function set and get define end */

/* defines GPIO pin direction */
typedef enum {
    PIN_INPUT = 0,
    PIN_OUTPUT
} PinDirection;

/* defines mode types of GPIO pin */
typedef enum {
    PullNone          = 0,
    PullUp            = 1,
    PullDown          = 2,
    OpenDrainPullUp   = 3,
    OpenDrainNoPull   = 4,
    OpenDrainPullDown = 5,
    PushPullNoPull    = PullNone,
    PushPullPullUp    = PullUp,
    PushPullPullDown  = PullDown,
    OpenDrain         = OpenDrainPullUp,
    PullDefault       = PullNone
} PinMode;

#ifdef __cplusplus
}
#endif

#endif
