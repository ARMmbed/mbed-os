/* mbed Microcontroller Library
 * Copyright (c) 2015-2016 Nuvoton
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

/* MBED TARGET LIST: NUMAKER_PFM_M453 */

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
#define NU_PINNAME_BIND(PINNAME, modname)           NU_PINNAME_BIND_(NU_PINPORT(PINNAME), NU_PININDEX(PINNAME), modname)
#define NU_PINNAME_BIND_(PORT, PIN, modname)        ((((unsigned int)(PORT)) << NU_PINPORT_Pos) | (((unsigned int)(PIN)) << NU_PININDEX_Pos) | (NU_MODINDEX(modname) << NU_PIN_MODINDEX_Pos) | NU_PIN_BIND_Msk)

#define NU_PORT_BASE(port)                          ((GPIO_T *)(((uint32_t) GPIOA_BASE) + 0x40 * port))
#define NU_MFP_POS(pin)                             ((pin % 8) * 4)
#define NU_MFP_MSK(pin)                             (0xful << NU_MFP_POS(pin))

// LEGACY
#define NU_PINNAME_TO_PIN(PINNAME)                  NU_PININDEX(PINNAME)
#define NU_PINNAME_TO_PORT(PINNAME)                 NU_PINPORT(PINNAME)
#define NU_PINNAME_TO_MODSUBINDEX(PINNAME)          NU_PIN_MODINDEX(PINNAME)
#define NU_PORT_N_PIN_TO_PINNAME(PORT, PIN)         NU_PINNAME((PORT), (PIN))

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
    // Not connected
    NC = (int)0xFFFFFFFF,
    
    // Generic naming
    PA_0    = NU_PORT_N_PIN_TO_PINNAME(0, 0), PA_1, PA_2, PA_3, PA_4, PA_5, PA_6, PA_7, PA_8, PA_9, PA_10, PA_11, PA_12, PA_13, PA_14, PA_15,

#if defined(TARGET_M45xD_M45xC)
    PB_0    = NU_PORT_N_PIN_TO_PINNAME(1, 0), PB_1, PB_2, PB_3, PB_4, PB_5, PB_6, PB_7, PB_8, PB_11 = PB_8 + 3, PB_12, PB_13, PB_14, PB_15,
#elif defined(TARGET_M45xG_M45xE)
    PB_0    = NU_PORT_N_PIN_TO_PINNAME(1, 0), PB_1, PB_2, PB_3, PB_4, PB_5, PB_6, PB_7, PB_8, PB_9, PB_10, PB_11, PB_12, PB_13, PB_14, PB_15,
#endif

#if defined(TARGET_M45xD_M45xC)
    PC_0    = NU_PORT_N_PIN_TO_PINNAME(2, 0), PC_1, PC_2, PC_3, PC_4, PC_5, PC_6, PC_7, PC_8, PC_10 = PC_8 + 2, PC_11, PC_12, PC_13,
#elif defined(TARGET_M45xG_M45xE)
    PC_0    = NU_PORT_N_PIN_TO_PINNAME(2, 0), PC_1, PC_2, PC_3, PC_4, PC_5, PC_6, PC_7, PC_8, PC_9, PC_10, PC_11, PC_12, PC_13, PC_14, PC_15,
#endif

#if defined(TARGET_M45xD_M45xC)
    PD_0    = NU_PORT_N_PIN_TO_PINNAME(3, 0), PD_1, PD_2, PD_3, PD_4, PD_5, PD_6, PD_7, PD_8, PD_9, PD_12 = PD_9 + 3, PD_13, PD_14, PD_15,
#elif defined(TARGET_M45xG_M45xE)
    PD_0    = NU_PORT_N_PIN_TO_PINNAME(3, 0), PD_1, PD_2, PD_3, PD_4, PD_5, PD_6, PD_7, PD_8, PD_9, PD_10, PD_11, PD_12, PD_13, PD_14, PD_15,
#endif

#if defined(TARGET_M45xD_M45xC)
    PE_0    = NU_PORT_N_PIN_TO_PINNAME(4, 0), PE_1, PE_3 = PE_1 + 2, PE_4, PE_5, PE_8 = PE_5 + 3, PE_9, PE_10, PE_11, PE_12, PE_13,
#elif defined(TARGET_M45xG_M45xE)
    PE_0    = NU_PORT_N_PIN_TO_PINNAME(4, 0), PE_1, PE_2, PE_3, PE_4, PE_5, PE_6, PE_7, PE_8, PE_9, PE_10, PE_11, PE_12, PE_13, PE_14,
#endif
    
    PF_0    = NU_PORT_N_PIN_TO_PINNAME(5, 0), PF_1, PF_2, PF_3, PF_4, PF_5, PF_6, PF_7,
    
    // Arduino UNO naming
    A0 = PB_0,
    A1 = PB_1,
    A2 = PB_2,
    A3 = PB_3,
    A4 = PB_4,
    A5 = PB_8,
    A6 = PB_9,
    A7 = PB_10,

    D0 = PD_6,
    D1 = PD_1,
    D2 = PC_6,
    D3 = PC_7,
    D4 = PC_11,
    D5 = PC_12,
    D6 = PC_13,
    D7 = PC_14,
    D8 = PC_0,
    D9 = PC_1,
    D10 = PC_2,
    D11 = PC_3,
    D12 = PC_4,
    D13 = PC_5,
    D14 = PE_5,
    D15 = PE_4,

    I2C_SCL = D15,
    I2C_SDA = D14,
    
    // NOTE: board-specific naming
    // UART naming
    CONSOLE_TX = PA_8,
    CONSOLE_RX = PA_9,
#if defined(MBED_CONF_TARGET_STDIO_UART_TX)
    STDIO_UART_TX   = MBED_CONF_TARGET_STDIO_UART_TX,
#else
    STDIO_UART_TX   = CONSOLE_TX,
#endif
#if defined(MBED_CONF_TARGET_STDIO_UART_RX)
    STDIO_UART_RX   = MBED_CONF_TARGET_STDIO_UART_RX,
#else
    STDIO_UART_RX   = CONSOLE_RX,
#endif
    SERIAL_TX = CONSOLE_TX,
    SERIAL_RX = CONSOLE_RX,
    // LED naming
    LED1 = PD_2,
    LED2 = PD_3,
    LED3 = PD_7,
    LED4 = LED1,    // No real LED. Just for passing ATS.
    LED_RED = LED2,
    LED_GREEN = LED3,
    LED_BLUE = LED1,
    // Button naming
    SW2 = PA_15,
    SW3 = PA_14,
    BUTTON1 = SW2,
    BUTTON2 = SW3,

    // Force PinName to 32-bit required by NU_PINNAME_BIND(...)
    FORCE_ENUM_PINNAME_32BIT = 0x7FFFFFFF,

} PinName;

#ifdef __cplusplus
}
#endif

#endif // MBED_PINNAMES_H
