/* mbed Microcontroller Library
 * Copyright (c) 2018 GigaDevice Semiconductor Inc.
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
    PORTA_0  = 0x00,
    PORTA_0_MUL0 = PORTA_0 | MUL0,
    PORTA_0_MUL1 = PORTA_0 | MUL1,
    PORTA_1  = 0x01,
    PORTA_1_MUL0 = PORTA_1 | MUL0,
    PORTA_1_MUL1 = PORTA_1 | MUL1,
    PORTA_2  = 0x02,
    PORTA_2_MUL0 = PORTA_2 | MUL0,
    PORTA_2_MUL1 = PORTA_2 | MUL1,
    PORTA_3  = 0x03,
    PORTA_3_MUL0 = PORTA_3 | MUL0,
    PORTA_3_MUL1 = PORTA_3 | MUL1,
    PORTA_4  = 0x04,
    PORTA_4_MUL0 = PORTA_4 | MUL0,
    PORTA_5  = 0x05,
    PORTA_5_MUL0 = PORTA_5 | MUL0,
    PORTA_6  = 0x06,
    PORTA_6_MUL0 = PORTA_6 | MUL0,
    PORTA_7  = 0x07,
    PORTA_7_MUL0 = PORTA_7 | MUL0,
    PORTA_7_MUL1 = PORTA_7 | MUL1,
    PORTA_7_MUL2 = PORTA_7 | MUL2,
    PORTA_8  = 0x08,
    PORTA_9  = 0x09,
    PORTA_10 = 0x0A,
    PORTA_11 = 0x0B,
    PORTA_12 = 0x0C,
    PORTA_13 = 0x0D,
    PORTA_14 = 0x0E,
    PORTA_15 = 0x0F,
    PORTA_15_MUL0 = PORTA_15 | MUL0,

    PORTB_0  = 0x10,
    PORTB_0_MUL0 = PORTB_0 | MUL0,
    PORTB_0_MUL1 = PORTB_0 | MUL1,
    PORTB_1  = 0x11,
    PORTB_1_MUL0 = PORTB_1 | MUL0,
    PORTB_1_MUL1 = PORTB_1 | MUL1,
    PORTB_2  = 0x12,
    PORTB_3  = 0x13,
    PORTB_3_MUL0 = PORTB_3 | MUL0,
    PORTB_4  = 0x14,
    PORTB_4_MUL0 = PORTB_4 | MUL0,
    PORTB_5  = 0x15,
    PORTB_5_MUL0 = PORTB_5 | MUL0,
    PORTB_6  = 0x16,
    PORTB_7  = 0x17,
    PORTB_8  = 0x18,
    PORTB_8_MUL0 = PORTB_8 | MUL0,
    PORTB_9  = 0x19,
    PORTB_9_MUL0 = PORTB_9 | MUL0,
    PORTB_10 = 0x1A,
    PORTB_11 = 0x1B,
    PORTB_12 = 0x1C,
    PORTB_13 = 0x1D,
    PORTB_14 = 0x1E,
    PORTB_14_MUL0 = PORTB_14 | MUL0,
    PORTB_14_MUL1 = PORTB_14 | MUL1,
    PORTB_15 = 0x1F,
    PORTB_15_MUL0 = PORTB_15 | MUL0,
    PORTB_15_MUL1 = PORTB_15 | MUL1,

    PORTC_0  = 0x20,
    PORTC_0_MUL0 = PORTC_0 | MUL0,
    PORTC_0_MUL1 = PORTC_0 | MUL1,
    PORTC_1  = 0x21,
    PORTC_1_MUL0 = PORTC_1 | MUL0,
    PORTC_1_MUL1 = PORTC_1 | MUL1,
    PORTC_2  = 0x22,
    PORTC_2_MUL0 = PORTC_2 | MUL0,
    PORTC_2_MUL1 = PORTC_2 | MUL1,
    PORTC_3  = 0x23,
    PORTC_3_MUL0 = PORTC_3 | MUL0,
    PORTC_3_MUL1 = PORTC_3 | MUL1,
    PORTC_4  = 0x24,
    PORTC_4_MUL0 = PORTC_4 | MUL0,
    PORTC_5  = 0x25,
    PORTC_5_MUL0 = PORTC_5 | MUL0,
    PORTC_6  = 0x26,
    PORTC_6_MUL0 = PORTC_6 | MUL0,
    PORTC_7  = 0x27,
    PORTC_7_MUL0 = PORTC_7 | MUL0,
    PORTC_8  = 0x28,
    PORTC_8_MUL0 = PORTC_8 | MUL0,
    PORTC_9  = 0x29,
    PORTC_9_MUL0 = PORTC_9 | MUL0,
    PORTC_10 = 0x2A,
    PORTC_10_MUL0 = PORTC_10 | MUL0,
    PORTC_11 = 0x2B,
    PORTC_11_MUL0 = PORTC_11 | MUL0,
    PORTC_12 = 0x2C,
    PORTC_13 = 0x2D,
    PORTC_14 = 0x2E,
    PORTC_15 = 0x2F,

    PORTD_0  = 0x30,
    PORTD_1  = 0x31,
    PORTD_2  = 0x32,
    PORTD_3  = 0x33,
    PORTD_4  = 0x34,
    PORTD_5  = 0x35,
    PORTD_6  = 0x36,
    PORTD_7  = 0x37,
    PORTD_8  = 0x38,
    PORTD_9  = 0x39,
    PORTD_10 = 0x3A,
    PORTD_11 = 0x3B,
    PORTD_12 = 0x3C,
    PORTD_13 = 0x3D,
    PORTD_14 = 0x3E,
    PORTD_15 = 0x3F,

    PORTE_0  = 0x40,
    PORTE_1  = 0x41,
    PORTE_2  = 0x42,
    PORTE_3  = 0x43,
    PORTE_4  = 0x44,
    PORTE_5  = 0x45,
    PORTE_6  = 0x46,
    PORTE_7  = 0x47,
    PORTE_8  = 0x48,
    PORTE_9  = 0x49,
    PORTE_10 = 0x4A,
    PORTE_11 = 0x4B,
    PORTE_12 = 0x4C,
    PORTE_13 = 0x4D,
    PORTE_14 = 0x4E,
    PORTE_15 = 0x4F,

    PORTF_0  = 0x50,
    PORTF_1  = 0x51,
    PORTF_2  = 0x52,
    PORTF_3  = 0x53,
    PORTF_4  = 0x54,
    PORTF_5  = 0x55,
    PORTF_6  = 0x56,
    PORTF_7  = 0x57,
    PORTF_8  = 0x58,
    PORTF_9  = 0x59,
    PORTF_10 = 0x5A,
    PORTF_11 = 0x5B,
    PORTF_12 = 0x5C,
    PORTF_13 = 0x5D,
    PORTF_14 = 0x5E,
    PORTF_15 = 0x5F,

    PORTG_0  = 0x60,
    PORTG_1  = 0x61,
    PORTG_2  = 0x62,
    PORTG_3  = 0x63,
    PORTG_4  = 0x64,
    PORTG_5  = 0x65,
    PORTG_6  = 0x66,
    PORTG_7  = 0x67,
    PORTG_8  = 0x68,
    PORTG_9  = 0x69,
    PORTG_10 = 0x6A,
    PORTG_11 = 0x6B,
    PORTG_12 = 0x6C,
    PORTG_13 = 0x6D,
    PORTG_14 = 0x6E,
    PORTG_15 = 0x6F,

    PORTH_0  = 0x70,
    PORTH_1  = 0x71,

    /* ADC internal channels */
    ADC_TEMP = 0xF0,
    ADC_VREF = 0xF1,
    ADC_VBAT = 0xF2,

    /* Arduino connector pin definitions */
    A0          = PORTC_0,
    A1          = PORTA_5,
    A2          = PORTC_2,
    A3          = PORTC_3,
    A4          = PORTA_0,      /* PORTB_11 */
    A5          = PORTC_5,      /* PORTB_10 */
    D0          = PORTC_11,
    D1          = PORTC_10,
    D2          = PORTA_15,
    D3          = PORTD_12,
    D4          = PORTC_8,
    D5          = PORTC_7,
    D6          = PORTB_0,
    D7          = PORTC_12,
    D8          = PORTD_13,
    D9          = PORTA_8,
    D10         = PORTA_1,
    D11         = PORTB_15,
    D12         = PORTB_14,
    D13         = PORTB_13,
    D14         = PORTB_9,
    D15         = PORTB_8,

    LED1        = PORTF_6,
    LED2        = PORTC_13,
    LED3        = PORTE_6,

    KEY1        = PORTE_5,
    KEY2        = PORTE_4,

    BUTTON1     = KEY1,
    BUTTON2     = KEY2,

    SERIAL_TX   = PORTC_12,
    SERIAL_RX   = PORTD_2,
    USBTX       = SERIAL_TX,
    USBRX       = SERIAL_RX,

    I2C_SCL     = PORTB_6,
    I2C_SDA     = PORTB_7,
    SPI_MOSI    = PORTB_5,
    SPI_MISO    = PORTB_4,
    SPI_SCK     = PORTB_3,
    SPI_CS      = PORTE_2,
    PWM_OUT     = D9,

    USBFS_VBUS  = PORTA_9,
    USBFS_DM    = PORTA_11,
    USBFS_DP    = PORTA_12,
    USBFS_ID    = PORTA_10,

    RMII_TX_EN  = PORTB_11,
    RMII_TXD0   = PORTB_12,
    RMII_TXD1   = PORTB_13,
    RMII_RXD0   = PORTC_4,
    RMII_RXD1   = PORTC_5,
    RMII_CRS_DV = PORTA_7,
    RMII_MDC    = PORTC_1,
    RMII_MDIO   = PORTA_2,
    RMII_INT    = PORTB_0,
    RMII_REF_CLK = PORTA_1,

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

/* BIT[7:4] port number (0=PORTA, 1=PORTB, 2=PORTC, 3=PORTD, 4=PORTE, 5=PORTD, 6=PORTF, 7=PORTH, )
   BIT[3:0] pin number */
#define GD_PORT_GET(X) (((uint32_t)(X) >> 4) & 0xF)
#define GD_PIN_GET(X)  (((uint32_t)(X) & 0xF))

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

#define GD_PIN_MODE_GET(X)              ( X & PIN_MODE_MASK)
#define GD_PIN_OUTPUT_MODE_GET(X)       ((X >> PIN_OUTPUT_MODE_SHIFT) & PIN_OUTPUT_MODE_MASK)
#define GD_PIN_PULL_STATE_GET(X)        ((X >> PIN_PULL_SHIFT) & PIN_PULL_MASK)
#define GD_PIN_SPEED_GET(X)             ((X >> PIN_SPEED_SHIFT) & PIN_SPEED_MASK)
#define GD_PIN_REMAP_GET(X)             ((X >> PIN_REMAP_SHIFT) & PIN_REMAP_MASK)
#define GD_PIN_CHANNEL_GET(X)           ((X >> PIN_CHANNEL_SHIFT) & PIN_CHANNEL_MASK)
#define GD_PIN_CHON_GET(X)              ((X >> PIN_CHON_SHIFT) & PIN_CHON_MASK)

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
