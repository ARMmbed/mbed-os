/*
 * Copyright (c) 2019, Nuvoton Technology Corporation
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

#define NU_PININDEX_Pos                             0
#define NU_PININDEX_Msk                             (0xFFul << NU_PININDEX_Pos)
#define NU_PINPORT_Pos                              8
#define NU_PINPORT_Msk                              (0xFul << NU_PINPORT_Pos)
#define NU_PIN_MODINDEX_Pos                         12
#define NU_PIN_MODINDEX_Msk                         (0xFul << NU_PIN_MODINDEX_Pos)
#define NU_PIN_BIND_Pos                             16
#define NU_PIN_BIND_Msk                             (0x1ul << NU_PIN_BIND_Pos)

#define NU_PININDEX(PINNAME)                        (((unsigned int)(PINNAME) & NU_PININDEX_Msk) >> NU_PININDEX_Pos)
#define NU_PINPORT(PINNAME)                         (((unsigned int)(PINNAME) & NU_PINPORT_Msk) >> NU_PINPORT_Pos)
#define NU_PIN_BIND(PINNAME)                        (((unsigned int)(PINNAME) & NU_PIN_BIND_Msk) >> NU_PIN_BIND_Pos)
#define NU_PIN_MODINDEX(PINNAME)                    (((unsigned int)(PINNAME) & NU_PIN_MODINDEX_Msk) >> NU_PIN_MODINDEX_Pos)
#define NU_PINNAME(PORT, PIN)                       ((((unsigned int) (PORT)) << (NU_PINPORT_Pos)) | (((unsigned int) (PIN)) << NU_PININDEX_Pos))
#define NU_PINNAME_BIND(PINNAME, modname)           ((PinName) NU_PINNAME_BIND_(NU_PINPORT(PINNAME), NU_PININDEX(PINNAME), modname))
#define NU_PINNAME_BIND_(PORT, PIN, modname)        ((((unsigned int)(PORT)) << NU_PINPORT_Pos) | (((unsigned int)(PIN)) << NU_PININDEX_Pos) | (NU_MODINDEX(modname) << NU_PIN_MODINDEX_Pos) | NU_PIN_BIND_Msk)

#define NU_PORT_BASE(PORT)                          ((GPIO_T *)(((uint32_t) GPIOA_BASE) + 0x40 * PORT))
#define NU_MFP_POS(pin)                             ((pin % 8) * 4)
#define NU_MFP_MSK(pin)                             (0xful << NU_MFP_POS(pin))

typedef enum {
    PIN_INPUT,
    PIN_OUTPUT
} PinDirection;

typedef enum {
    /* Input pull mode */
    PullNone = 0,
    PullDown,
    PullUp,
    
    /* I/O mode */
    InputOnly,
    PushPullOutput,
    OpenDrain,
    QuasiBidirectional,
    
    /* Default input pull mode */
    PullDefault = PullUp
} PinMode;

typedef enum {
    /* Not connected */
    NC = (int)0xFFFFFFFF,
    
    /* Generic naming */
    PA_0    = NU_PINNAME(0, 0), PA_1, PA_2, PA_3, PA_4, PA_5, PA_6, PA_7, PA_8, PA_9, PA_10, PA_11, PA_12, PA_13, PA_14, PA_15,
    PB_0    = NU_PINNAME(1, 0), PB_1, PB_2, PB_3, PB_4, PB_5, PB_6, PB_7, PB_8, PB_9, PB_10, PB_11, PB_12, PB_13, PB_14, PB_15,
    PC_0    = NU_PINNAME(2, 0), PC_1, PC_2, PC_3, PC_4, PC_5, PC_6, PC_7, PC_8, PC_9, PC_10, PC_11, PC_12, PC_14 = PC_12 + 2,
    PD_0    = NU_PINNAME(3, 0), PD_1, PD_2, PD_3, PD_4, PD_5, PD_6, PD_7, PD_8, PD_9, PD_10, PD_11, PD_12, PD_13, PD_15 = PD_13 + 2,
    PE_0    = NU_PINNAME(4, 0), PE_1, PE_2, PE_3, PE_4, PE_5, PE_6, PE_7, PE_8, PE_9, PE_10, PE_11, PE_12, PE_13, PE_14, PE_15,
    PF_0    = NU_PINNAME(5, 0), PF_1, PF_2, PF_3, PF_4, PF_5, PF_6, PF_7, PF_14 = PF_7 + 7,

    /* Arduino UNO naming */
    A0 = PB_7,
    A1 = PB_6,
    A2 = PB_5,
    A3 = PB_4,
    A4 = PB_0,
    A5 = PB_1,

    D0 = PB_2,
    D1 = PB_3,
    D2 = PC_4,
    D3 = PC_5,
    D4 = PC_3,
    D5 = PC_2,
    D6 = PA_7,
    D7 = PA_6,
    D8 = PA_5,
    D9 = PA_4,
    D10 = PA_3,
    D11 = PA_0,
    D12 = PA_1,
    D13 = PA_2,
    D14 = PC_0,
    D15 = PC_1,
    
    /* Other board-specific naming */
    
    /* UART naming */
    USBTX = PB_13,
    USBRX = PB_12,
    STDIO_UART_TX   = USBTX,
    STDIO_UART_RX   = USBRX,
    SERIAL_TX       = USBTX,
    SERIAL_RX       = USBRX,

    /* I2C naming */
    I2C_SCL = D15,
    I2C_SDA = D14,
    
    /* LED naming */
    LED_RED = PB_14,
    LED1    = LED_RED,
    LED2    = LED1,     // No real LED. Just for passing ATS.
    LED3    = LED1,     // No real LED. Just for passing ATS.
    LED4    = LED1,     // No real LED. Just for passing ATS.

    /* Button naming */
    
    /* Force PinName to 32-bit required by NU_PINNAME_BIND(...) */
    FORCE_ENUM_PINNAME_32BIT = 0x7FFFFFFF,

} PinName;

#ifdef __cplusplus
}
#endif

#endif // MBED_PINNAMES_H
