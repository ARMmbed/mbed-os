/* mbed Microcontroller Library
 *******************************************************************************
 * Copyright (c) XXX
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice,
 *    this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 * 3. Neither the name of ARM Limited nor the names of its contributors
 *    may be used to endorse or promote products derived from this software
 *    without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
 * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
 * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
 * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *******************************************************************************
 */

#ifndef MBED_PINNAMES_H
#define MBED_PINNAMES_H

#include "cmsis.h"
#include "PinNamesTypes.h"
#include "PortNames.h"

#ifdef __cplusplus
extern "C" {
#endif

#define CY_UART_RX P5_0
#define CY_UART_TX P5_1

// PinName[15-0] = Port[15-8] + Pin[7-0]
typedef enum {
    P0_0    = (Port0 << 8) + 0x00,
    P0_1    = (Port0 << 8) + 0x01,
    P0_2    = (Port0 << 8) + 0x02,
    P0_3    = (Port0 << 8) + 0x03,
    P0_4    = (Port0 << 8) + 0x04,
    P0_5    = (Port0 << 8) + 0x05,
    P0_6    = (Port0 << 8) + 0x06,
    P0_7    = (Port0 << 8) + 0x07,

    P1_0    = (Port1 << 8) + 0x00,
    P1_1    = (Port1 << 8) + 0x01,
    P1_2    = (Port1 << 8) + 0x02,
    P1_3    = (Port1 << 8) + 0x03,
    P1_4    = (Port1 << 8) + 0x04,
    P1_5    = (Port1 << 8) + 0x05,
    P1_6    = (Port1 << 8) + 0x06,
    P1_7    = (Port1 << 8) + 0x07,

    P2_0    = (Port2 << 8) + 0x00,
    P2_1    = (Port2 << 8) + 0x01,
    P2_2    = (Port2 << 8) + 0x02,
    P2_3    = (Port2 << 8) + 0x03,
    P2_4    = (Port2 << 8) + 0x04,
    P2_5    = (Port2 << 8) + 0x05,
    P2_6    = (Port2 << 8) + 0x06,
    P2_7    = (Port2 << 8) + 0x07,

    P3_0    = (Port3 << 8) + 0x00,
    P3_1    = (Port3 << 8) + 0x01,
    P3_2    = (Port3 << 8) + 0x02,
    P3_3    = (Port3 << 8) + 0x03,
    P3_4    = (Port3 << 8) + 0x04,
    P3_5    = (Port3 << 8) + 0x05,
    P3_6    = (Port3 << 8) + 0x06,
    P3_7    = (Port3 << 8) + 0x07,

    P4_0    = (Port4 << 8) + 0x00,
    P4_1    = (Port4 << 8) + 0x01,
    P4_2    = (Port4 << 8) + 0x02,
    P4_3    = (Port4 << 8) + 0x03,
    P4_4    = (Port4 << 8) + 0x04,
    P4_5    = (Port4 << 8) + 0x05,
    P4_6    = (Port4 << 8) + 0x06,
    P4_7    = (Port4 << 8) + 0x07,

    P5_0    = (Port5 << 8) + 0x00,
    P5_1    = (Port5 << 8) + 0x01,
    P5_2    = (Port5 << 8) + 0x02,
    P5_3    = (Port5 << 8) + 0x03,
    P5_4    = (Port5 << 8) + 0x04,
    P5_5    = (Port5 << 8) + 0x05,
    P5_6    = (Port5 << 8) + 0x06,
    P5_7    = (Port5 << 8) + 0x07,

    P6_0    = (Port6 << 8) + 0x00,
    P6_1    = (Port6 << 8) + 0x01,
    P6_2    = (Port6 << 8) + 0x02,
    P6_3    = (Port6 << 8) + 0x03,
    P6_4    = (Port6 << 8) + 0x04,
    P6_5    = (Port6 << 8) + 0x05,
    P6_6    = (Port6 << 8) + 0x06,
    P6_7    = (Port6 << 8) + 0x07,

    P7_0    = (Port7 << 8) + 0x00,
    P7_1    = (Port7 << 8) + 0x01,
    P7_2    = (Port7 << 8) + 0x02,
    P7_3    = (Port7 << 8) + 0x03,
    P7_4    = (Port7 << 8) + 0x04,
    P7_5    = (Port7 << 8) + 0x05,
    P7_6    = (Port7 << 8) + 0x06,
    P7_7    = (Port7 << 8) + 0x07,

    P8_0    = (Port8 << 8) + 0x00,
    P8_1    = (Port8 << 8) + 0x01,
    P8_2    = (Port8 << 8) + 0x02,
    P8_3    = (Port8 << 8) + 0x03,
    P8_4    = (Port8 << 8) + 0x04,
    P8_5    = (Port8 << 8) + 0x05,
    P8_6    = (Port8 << 8) + 0x06,
    P8_7    = (Port8 << 8) + 0x07,

    P9_0    = (Port9 << 8) + 0x00,
    P9_1    = (Port9 << 8) + 0x01,
    P9_2    = (Port9 << 8) + 0x02,
    P9_3    = (Port9 << 8) + 0x03,
    P9_4    = (Port9 << 8) + 0x04,
    P9_5    = (Port9 << 8) + 0x05,
    P9_6    = (Port9 << 8) + 0x06,
    P9_7    = (Port9 << 8) + 0x07,

    P10_0   = (Port10 << 8) + 0x00,
    P10_1   = (Port10 << 8) + 0x01,
    P10_2   = (Port10 << 8) + 0x02,
    P10_3   = (Port10 << 8) + 0x03,
    P10_4   = (Port10 << 8) + 0x04,
    P10_5   = (Port10 << 8) + 0x05,
    P10_6   = (Port10 << 8) + 0x06,
    P10_7   = (Port10 << 8) + 0x07,

    P11_0   = (Port11 << 8) + 0x00,
    P11_1   = (Port11 << 8) + 0x01,
    P11_2   = (Port11 << 8) + 0x02,
    P11_3   = (Port11 << 8) + 0x03,
    P11_4   = (Port11 << 8) + 0x04,
    P11_5   = (Port11 << 8) + 0x05,
    P11_6   = (Port11 << 8) + 0x06,
    P11_7   = (Port11 << 8) + 0x07,

    P12_0   = (Port12 << 8) + 0x00,
    P12_1   = (Port12 << 8) + 0x01,
    P12_2   = (Port12 << 8) + 0x02,
    P12_3   = (Port12 << 8) + 0x03,
    P12_4   = (Port12 << 8) + 0x04,
    P12_5   = (Port12 << 8) + 0x05,
    P12_6   = (Port12 << 8) + 0x06,
    P12_7   = (Port12 << 8) + 0x07,

    P13_0   = (Port13 << 8) + 0x00,
    P13_1   = (Port13 << 8) + 0x01,
    P13_2   = (Port13 << 8) + 0x02,
    P13_3   = (Port13 << 8) + 0x03,
    P13_4   = (Port13 << 8) + 0x04,
    P13_5   = (Port13 << 8) + 0x05,
    P13_6   = (Port13 << 8) + 0x06,
    P13_7   = (Port13 << 8) + 0x07,

    P14_0   = (Port14 << 8) + 0x00,
    P14_1   = (Port14 << 8) + 0x01,
    P14_2   = (Port14 << 8) + 0x02,
    P14_3   = (Port14 << 8) + 0x03,
    P14_4   = (Port14 << 8) + 0x04,
    P14_5   = (Port14 << 8) + 0x05,
    P14_6   = (Port14 << 8) + 0x06,
    P14_7   = (Port14 << 8) + 0x07,

    P15_0   = (Port15 << 8) + 0x00,
    P15_1   = (Port15 << 8) + 0x01,
    P15_2   = (Port15 << 8) + 0x02,
    P15_3   = (Port15 << 8) + 0x03,
    P15_4   = (Port15 << 8) + 0x04,
    P15_5   = (Port15 << 8) + 0x05,
    P15_6   = (Port15 << 8) + 0x06,
    P15_7   = (Port15 << 8) + 0x07,

    // Arduino connector namings
    A0          = P10_0,
    A1          = P10_1,
    A2          = P10_2,
    A3          = P10_3,
    A4          = P10_4,
    A5          = P10_5,

    D0          = P5_0,
    D1          = P5_1,
    D2          = P5_2,
    D3          = P5_3,
    D4          = P5_4,
    D5          = P5_5,
    D6          = P5_6,
    D7          = P0_2,
    D8          = P13_0,
    D9          = P13_1,
    D10         = P12_3,
    D11         = P12_0,
    D12         = P12_1,
    D13         = P12_2,

    // Generic signals namings

    LED_RED     = P0_3,
    LED_GREEN   = P1_1,
    LED_BLUE    = P11_1,

    SWITCH2     = P0_4,

    LED1        = LED_RED,
    LED2        = LED_GREEN,
    LED3        = LED_BLUE,
    LED4        = LED_RED,

    USER_BUTTON = SWITCH2,
    BUTTON1     = USER_BUTTON,

    // Standardized interfaces names
    SERIAL_TX   = CY_UART_TX,
    SERIAL_RX   = CY_UART_RX,
    USBTX       = CY_UART_TX,
    USBRX       = CY_UART_RX,
    // I2C_SCL     = PB_8,
    // I2C_SDA     = PB_9,
    // SPI_MOSI    = PA_7,
    // SPI_MISO    = PA_6,
    // SPI_SCK     = PA_5,
    // SPI_CS      = PB_6,
    // PWM_OUT     = PB_3,

    // Not connected
    NC = (int)0xFFFFFFFF
} PinName;

#ifdef __cplusplus
}
#endif

#endif
