/* mbed Microcontroller Library 
 *******************************************************************************
 * Copyright (c) 2015 WIZnet Co.,Ltd. All rights reserved.
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
#define WIZ_PIN_DATA(MODE, PUPD, AFNUM)  ((int)(((MODE) << 8) | ((PUPD) << 4) | ((AFNUM) << 0)))
#define WIZ_PIN_PUPD(X)       (((X) >> 4) & 0x0F)
#define WIZ_PIN_AFNUM(X)      (((X) >> 0) & 0x0F)
#define WIZ_PIN_MODE(X)       (((X) >> 8) & 0x0F)
#define WIZ_MODE_INPUT              (0)
#define WIZ_MODE_OUTPUT             (1)
#define WIZ_MODE_AF                 (2)

#define WIZ_GPIO_NOPULL             (0)   /*!< No Pull-up or Pull-down activation  */
#define WIZ_GPIO_PULLDOWN           (1)   /*!< Pull-down activation                */
#define WIZ_GPIO_PULLUP             (2)   /*!< Pull-up activation                  */
#define WIZ_GPIO_OPEN_DRAIN         (3)   /*!< Open Drain activation               */


#define WIZ_PORT(X) (((uint32_t)(X) >> 4) & 0xF)    // port number (0=A, 1=B, 2=C, 3=D)
#define WIZ_PIN_NUM(X)  ((uint32_t)(X) & 0xF)    // pin number
#define WIZ_PIN_INDEX(X)  (1 << ((uint32_t)(X) & 0xF))    // pin index : flag bit 


typedef enum {
    PIN_INPUT,
    PIN_OUTPUT
} PinDirection;


typedef enum {
    // W7500x PORT[5:4] + PIN[3:0])
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
              
    PB_0  = 0x010, 
    PB_1  = 0x011, 
    PB_2  = 0x012, 
    PB_3  = 0x013, 
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
              
    PC_0  = 0x020,
    PC_1  = 0x021,
    PC_2  = 0x022,
    PC_3  = 0x023,
    PC_4  = 0x024,
    PC_5  = 0x025,
    PC_6  = 0x026,
    PC_7  = 0x027,
    PC_8  = 0x028,
    PC_9  = 0x029,
              
    PC_10 = 0x02A,
    PC_11 = 0x02B,
    PC_12 = 0x02C,
    PC_13 = 0x02D,
    PC_14 = 0x02E,
    PC_15 = 0x02F,
              
    PD_0  = 0x030,
    PD_1  = 0x031,
    PD_2  = 0x032,
    PD_3  = 0x033,
    PD_4  = 0x034,

    PA_00  = PA_0,
    PA_01  = PA_1,
    PA_02  = PA_2,
    PA_03  = PA_3,
    PA_04  = PA_4,
    PA_05  = PA_5,
    PA_06  = PA_6,
    PA_07  = PA_7,
    PA_08  = PA_8,
    PA_09  = PA_9,

    PB_00  = PB_0,
    PB_01  = PB_1,
    PB_02  = PB_2,
    PB_03  = PB_3,
    PB_04  = PB_4,
    PB_05  = PB_5,
    PB_06  = PB_6,
    PB_07  = PB_7,
    PB_08  = PB_8,
    PB_09  = PB_9,

    PC_00  = PC_0,
    PC_01  = PC_1,
    PC_02  = PC_2,
    PC_03  = PC_3,
    PC_04  = PC_4,
    PC_05  = PC_5,
    PC_06  = PC_6,
    PC_07  = PC_7,
    PC_08  = PC_8,
    PC_09  = PC_9,

    PD_00  = PD_0,
    PD_01  = PD_1,
    PD_02  = PD_2,
    PD_03  = PD_3,
    PD_04  = PD_4,
    
    // Arduino connector namings
    A0          = PC_15,    // AIN0
    A1          = PC_14,    // AIN1
    A2          = PC_13,    // AIN2
    A3          = PC_12,    // AIN3
    A4          = PC_9,    // AIN4
    A5          = PC_8,    // AIN5
    
    D0          = PA_14,
    D1          = PA_13,
    D2          = PC_3,
    D3          = PC_2,
    D4          = PA_2,
    D5          = PA_1,
    D6          = PA_0,
    D7          = PA_12,
    D8          = PA_11,
    D9          = PC_1,
    D10         = PA_5,
    D11         = PA_8,
    D12         = PA_7,
    D13         = PA_6,
    D14         = PA_10,
    D15         = PA_9,

    // Generic signals namings
    LED_RED     = PC_0,
    LED_GREEN   = PC_4,
    LED_BLUE    = PC_5,

    LED1        = LED_RED,
    LED2        = LED_GREEN,
    LED3        = LED_BLUE,
    LED4        = LED_BLUE,

    LEDR        = LED_RED,
    LEDG        = LED_GREEN,
    LEDB        = LED_BLUE,

    USBTX       = PC_10,
    USBRX       = PC_11,

    //Use SPI1
    SD_SEL      = PB_0,         // SPI1_CS
    SD_CLK      = PB_1,         // SPI1_CLK
    SD_MISO     = PB_2,         // MOSI1
    SD_MOSI     = PB_3,         // MISO1

    // Not connected
    NC = (int)0xFFFFFFFF
} PinName;

typedef enum {
    PullNone  = 0,
    PullDown  = 1,
    PullUp    = 2,
    PullDefault = PullNone
} PinMode;


#ifdef __cplusplus
}
#endif

#endif
