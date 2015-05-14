/* mbed Microcontroller Library
 * CMSIS-style functionality to support dynamic vectors
 *******************************************************************************
 * Copyright (c) 2011 ARM Limited. All rights reserved.
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

#ifdef __cplusplus
extern "C" {
#endif


// See W7500x_hal_gpio.h for values of MODE, PUPD and AFNUM
#define WIZ_PIN_DATA(MODE, PUPD, AFNUM)  ((int)(((AFNUM) << 8) | ((PUPD) << 4) | ((MODE) << 0)))
#define WIZ_PIN_MODE(X)       (((X) >> 0) & 0x0F)
#define WIZ_PIN_PUPD(X)       (((X) >> 4) & 0x0F)
#define WIZ_PIN_AFNUM(X)      (((X) >> 8) & 0x0F)
#define WIZ_MODE_INPUT              (0)
#define WIZ_MODE_OUTPUT             (1)
#define WIZ_MODE_AF                 (2)

#define WIZ_GPIO_NOPULL             (0)   /*!< No Pull-up or Pull-down activation  */
#define WIZ_GPIO_PULLDOWN           (1)   /*!< Pull-down activation                */
#define WIZ_GPIO_PULLUP             (2)   /*!< Pull-up activation                  */
#define WIZ_GPIO_OPEN_DRAIN         (3)   /*!< Open Drain activation               */


#define WIZ_AFNUM(X)(((uint32_t)(X) >> 8) & 0xF)    // AF number   (0=AF0, 1=AF1, 2=AF2, 3=AF3)
#define WIZ_PORT(X) (((uint32_t)(X) >> 4) & 0xF)    // port number (0=A, 1=B, 2=C, 3=D)
#define WIZ_PIN(X)  ((uint32_t)(X) & 0xF)           // pin number


typedef enum {
    PIN_INPUT,
    PIN_OUTPUT
} PinDirection;


typedef enum {
    // W7500x Pin Names (AF[9:8] + PORT[5:4] + PIN[3:0])

    PA_0  = 0x000,
    PA_1  = 0x001,
    PA_2  = 0x002,
    PA_3  = 0x003,
    PA_4  = 0x004,
    PA_5  = 0x005,
    PA_6  = 0x006,
    PA_7  = 0x007,
    PA_8  = 0x008,
    PA_9  = 0x009,
    PA_10 = 0x00A,
    PA_11 = 0x00B,
    PA_12 = 0x00C,
    PA_13 = 0x00D,
    PA_14 = 0x00E,
    PA_15 = 0x00F,

    PB_0  = 0x010, //SSEL1/SD_SEL
    PB_1  = 0x011, //SCLK1/SD_CLK
    PB_2  = 0x012, //MISO1/SD_MISO
    PB_3  = 0x013, //MOSI1/SD_MOSI
    PB_4  = 0x014,
    PB_5  = 0x015,
    PB_6  = 0x016,
    PB_7  = 0x017,
    PB_8  = 0x018,
    PB_9  = 0x019,
    PB_10 = 0x01A,
    PB_11 = 0x01B,
    PB_12 = 0x01C,
    PB_13 = 0x01D,
    PB_14 = 0x01E,
    PB_15 = 0x01F,

    PC_0  = 0x120,  // 0xx:U_CTS1, 1xx:GPIOC_0, 2xx:PWM0
    PC_1  = 0x121,  // 0xx:U_RTS1, 1xx:GPIOC_1, 2xx:PWM1
    PC_2  = 0x022,
    PC_3  = 0x023,
    PC_4  = 0x124,  // 0xx:SDA1, 1xx:GPIOC_4, 2xx:PWM4
    PC_5  = 0x025,
    PC_6  = 0x026,
    PC_7  = 0x027,
    PC_8  = 0x128,  // 0xx:PWM0,   1xx:GPIOC_8,  2xx:SCL0,  3xx:AIN7
    PC_9  = 0x129,  // 0xx:PWM1,   1xx:GPIOC_9,  2xx:SDA0,  3xx:AIN6

    PC_10 = 0x32A,  // 0xx:U_TXD2, 1xx:GPIOC_10, 2xx:PWM2,  3xx:AIN5
    PC_11 = 0x32B,  // 0xx:U_RXD2, 1xx:GPIOC_11, 2xx:PWM3,  3xx:AIN4
    PC_12 = 0x32C,  // 0xx:AIN3,   1xx:GPIOC_12, 2xx:SSEL0, 3xx:AIN3
    PC_13 = 0x32D,  // 0xx:AIN2,   1xx:GPIOC_13, 2xx:SCLK0, 3xx:AIN2
    PC_14 = 0x32E,  // 0xx:AIN1,   1xx:GPIOC_14, 2xx:MISO0, 3xx:AIN1
    PC_15 = 0x32F,  // 0xx:AIN0,   1xx:GPIOC_15, 2xx:MOSI0, 3xx:AIN0

    PD_0  = 0x030,
    PD_1  = 0x031,
    PD_2  = 0x032,
    PD_3  = 0x033,
    PD_4  = 0x034,


    // Arduino connector namings
    A0          = PC_15,			// AIN0
    A1          = PC_14,			// AIN1
    A2          = PC_13,			// AIN2
    A3          = PC_12,			// AIN3
    A4          = PC_11,			// AIN4
    A5          = PC_10,			// AIN5
    D0          = PA_14,			// U_TXD0
    D1          = PA_13,			// U_RXD0
    D2          = PC_1,			// GPIOC_1, EXTI
    D3          = PC_0,			// GPIOC_0, EXTI
    D4          = PA_2,			// GPIOA_2,
    D5          = PA_1,			// GPIOA_1,
    D6          = PA_0,			// GPIOA_0,
    D7          = PC_6,			// GPIOC_6, EXTI
    D8          = PC_7,			// GPIOC_7, EXTI
    D9          = PC_4,			// GPIOC_4, EXTI
    D10         = PA_5,			// SSEL0
    D11         = PA_8,			// MOSI0
    D12         = PA_7,			// MISO0
    D13         = PA_6,			// SCLK0
    D14         = PA_10,			// SDA0
    D15         = PA_9,			// SCL0

    // Generic signals namings
	LED_RED      = PC_8,
	LED_GREEN    = PC_9,
	LED_BLUE     = PC_5,

    LED1        = LED_RED,
    LED2        = LED_GREEN,
    LED3        = LED_BLUE,
    LED4        = LED_BLUE,

//    USER_BUTTON = PC_13,
    SERIAL_TX   = PC_2,			// U_TXD1
    SERIAL_RX   = PC_3,			// U_RXD1
    USBTX       = SERIAL_TX,
    USBRX       = SERIAL_RX,
    I2C_SCL     = PA_9,         // SCL0
    I2C_SDA     = PA_10,        // SDA0
    SPI_MOSI    = PA_8,         // MISO0
    SPI_MISO    = PA_7,         // MOSI0
    SPI_SCK     = PA_9,         // SCL0
    SPI_CS      = PA_5,         // SSEL0
    SD_SEL      = PB_0,         // SSEL1
    SD_CLK      = PB_1,         // SCL1
    SD_MISO     = PB_2,         // MOSI1
    SD_MOSI     = PB_3,         // MISO1
//    PWM_OUT     = PC_7,

    // Not connected
    NC = (int)0xFFFFFFFF
} PinName;

typedef enum {
    PullNone  = 0,
    PullDown  = 1,
    PullUp    = 2,
    OpenDrain = 3,
    PullDefault = PullNone
} PinMode;


#ifdef __cplusplus
}
#endif

#endif
