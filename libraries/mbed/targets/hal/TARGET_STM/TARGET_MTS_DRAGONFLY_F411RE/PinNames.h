/* mbed Microcontroller Library
 *******************************************************************************
 * Copyright (c) 2014, STMicroelectronics
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
 * 3. Neither the name of STMicroelectronics nor the names of its contributors
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

// See stm32f4xx_hal_gpio.h and stm32f4xx_hal_gpio_ex.h for values of MODE, PUPD and AFNUM
#define STM_PIN_DATA(MODE, PUPD, AFNUM)  ((int)(((AFNUM) << 7) | ((PUPD) << 4) | ((MODE) << 0)))
#define STM_PIN_MODE(X)   (((X) >> 0) & 0x0F)
#define STM_PIN_PUPD(X)   (((X) >> 4) & 0x07)
#define STM_PIN_AFNUM(X)  (((X) >> 7) & 0x0F)
#define STM_MODE_INPUT              (0)
#define STM_MODE_OUTPUT_PP          (1)
#define STM_MODE_OUTPUT_OD          (2)
#define STM_MODE_AF_PP              (3)
#define STM_MODE_AF_OD              (4)
#define STM_MODE_ANALOG             (5)
#define STM_MODE_IT_RISING          (6)
#define STM_MODE_IT_FALLING         (7)
#define STM_MODE_IT_RISING_FALLING  (8)
#define STM_MODE_EVT_RISING         (9)
#define STM_MODE_EVT_FALLING        (10)
#define STM_MODE_EVT_RISING_FALLING (11)
#define STM_MODE_IT_EVT_RESET       (12)

// High nibble = port number (0=A, 1=B, 2=C, 3=D, 4=E, 5=F, 6=G, 7=H)
// Low nibble  = pin number
#define STM_PORT(X) (((uint32_t)(X) >> 4) & 0xF)
#define STM_PIN(X)  ((uint32_t)(X) & 0xF)

typedef enum {
    PIN_INPUT,
    PIN_OUTPUT
} PinDirection;

typedef enum {
    PA_0  = 0x00,
    PA_1  = 0x01,
    PA_2  = 0x02,
    PA_3  = 0x03,
    PA_4  = 0x04,
<<<<<<< HEAD
    PA_5  = 0x05, 
=======
    PA_5  = 0x05, //0x2001,//vince orig 0x05,
>>>>>>> 2e18da74199fa17e036058f3d1a73c635370049c
    PA_6  = 0x06,
    PA_7  = 0x07,
    PA_8  = 0x08,
    PA_9  = 0x09,
    PA_10 = 0x0A,
    PA_11 = 0x0B,
<<<<<<< HEAD
    PA_12 = 0x0C,
=======
    PA_12 = 0x0C,//ORIG vince 0x0C,
>>>>>>> 2e18da74199fa17e036058f3d1a73c635370049c
    PA_13 = 0x0D,
    PA_14 = 0x0E,
    PA_15 = 0x0F,

    PB_0  = 0x10,
    PB_1  = 0x11,
    PB_2  = 0x12,
    PB_3  = 0x13,
    PB_4  = 0x14,
    PB_5  = 0x15,
    PB_6  = 0x16,
    PB_7  = 0x17,
    PB_8  = 0x18,
    PB_9  = 0x19,
<<<<<<< HEAD
    PB_10 = 0x1A, 
=======
    PB_10 = 0x1A, //vince orig PB_10
>>>>>>> 2e18da74199fa17e036058f3d1a73c635370049c
    PB_12 = 0x1C,
    PB_13 = 0x1D,
    PB_14 = 0x1E,
    PB_15 = 0x1F,

    PC_0  = 0x20,
    PC_1  = 0x21,
    PC_2  = 0x22,
    PC_3  = 0x23,
    PC_4  = 0x24,
    PC_5  = 0x25,
    PC_6  = 0x26,
<<<<<<< HEAD
    PC_7  = 0x27, 
    PC_8  = 0x28,
    PC_9  = 0x29, 
    PC_10 = 0x2A,
    PC_11 = 0x2B, 
=======
    PC_7  = 0x27, // 0x2003, //vince orig 0x27,
    PC_8  = 0x28,
    PC_9  = 0x29, //0x2004, //vince orig 0x29,
    PC_10 = 0x2A,
    PC_11 = 0x2B, //0x2005, //vince orig 0x2B,
>>>>>>> 2e18da74199fa17e036058f3d1a73c635370049c
    PC_12 = 0x2C,
    PC_13 = 0x2D,
    PC_14 = 0x2E,
    PC_15 = 0x2F,

<<<<<<< HEAD
    PD_2  = 0x32, 
=======
    PD_2  = 0x32, //0x2007,//vince orig 0x32,
>>>>>>> 2e18da74199fa17e036058f3d1a73c635370049c

    PH_0  = 0x70,
    PH_1  = 0x71,

    // Arduino connector namings
    A0          = PA_0,
    A1          = PA_1,
<<<<<<< HEAD
    A2          = PC_4, 
    A3          = PB_0,
    A4          = PC_1,
    A5          = PC_0,
    D0          = PA_3,
    D1          = PA_2,
    D2          = PB_12, 
    D3          = PB_10, 
    D4          = PA_8, 
    D5          = PA_7, 
    D6          = PB_13, 
    D7          = PC_2, 
    D8          = PA_9,
    D9          = PB_1, 
    D10         = PC_8, 
=======
    A2          = PC_4, //vince orig PA_4,
    A3          = PB_0,
    A4          = PC_1,
    A5          = PC_0,
    D0          = PA_3,//vince orig PA_3,
    D1          = PA_2,//vince orig PA_2,
    //vince orig D2          = PA_10,
    D2          = PB_12, //vince add
    //vince orig D3          = PB_3,
    D3          = PB_10, //vince a dd
    D4          = PA_8, //vince add
    D5          = PA_7, //vince add
    //vince orig D4          = PB_5,
    //vince orig D5          = PB_4,
    D6          = PB_13, //vince orig PB_10
    // vince orig D7          = PA_8,
    D7          = PC_2, //vince add
    D8          = PA_9,
    //vince orig D9          = PC_7,
    D9          = PB_1, //vince add
    //vince orig D10         = PB_6,
    D10         = PC_8, //vince add
    //vince orig D11         = PA_7,
>>>>>>> 2e18da74199fa17e036058f3d1a73c635370049c
    D11         = PB_5,
    D12         = PA_6,
    D13         = PA_5,
    D14         = PB_9,
    D15         = PB_8,

    // Generic signals namings
<<<<<<< HEAD
    LED1        = PB_10, 
=======
    LED1        = PB_10, /* vince was PA_5 , next 3 lines below */
>>>>>>> 2e18da74199fa17e036058f3d1a73c635370049c
    LED2        = PB_10,
    LED3        = PB_10,
    LED4        = PB_10,
    USER_BUTTON = PC_13,
<<<<<<< HEAD
    SERIAL_TX   = PB_6, 
    SERIAL_RX   = PB_7, 
    USBTX       = PB_6,
    USBRX       = PB_7,
    I2C_SCL     = PB_8,
    I2C_SDA     = PB_9,
    SPI_MOSI    = PB_5,
    SPI_MISO    = PA_6,
    SPI_SCK     = PA_5,
    SPI_CS      = PA_4,
    PWM_OUT     = PB_3, 
=======
    SERIAL_TX   = PB_6, //vince orig PA_2
    SERIAL_RX   = PB_7, //vince orig PA_3
    USBTX       = PB_6,//PA_11, //vince orig PA_2,
    USBRX       = PB_7,//PA_12, //vince orig PA_3,
    I2C_SCL     = PB_8,
    I2C_SDA     = PB_9,
    SPI_MOSI    = PB_5,//vince orig PA_7,
    SPI_MISO    = PA_6,
    SPI_SCK     = PA_5,
    SPI_CS      = PA_4,//vince orig PB_6,
    PWM_OUT     = PB_3, //vince TODO check on this 
>>>>>>> 2e18da74199fa17e036058f3d1a73c635370049c

    // Not connected
    NC = (int)0xFFFFFFFF
} PinName;

typedef enum {
    PullNone  = 0,
    PullUp    = 1,
    PullDown  = 2,
    OpenDrain = 3,
    PullDefault = PullNone
} PinMode;

#ifdef __cplusplus
}
#endif

#endif
