/* mbed Microcontroller Library
 '*******************************************************************************
 * Copyright (c) 2016, STMicroelectronics
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

#include "PeripheralPins.h"

//*** SERIAL ***

const PinMap PinMap_UART_TX[] = {
    {PB_6, UART_1, STM_PIN_DATA(STM_MODE_AF_PP, GPIO_PULLUP, 3)}, // Tx0 - GPIO_Remap_USART1
    {PA_2, UART_2, STM_PIN_DATA(STM_MODE_AF_PP, GPIO_PULLUP, 0)},   // RS485_Tx1
    {PC_10, UART_4, STM_PIN_DATA(STM_MODE_AF_PP, GPIO_PULLUP, 0)}, // DBG_UART_TX
    {NC,    NC,    0}
};

const PinMap PinMap_UART_RX[] = {
    {PB_7, UART_1, STM_PIN_DATA(STM_MODE_INPUT, GPIO_PULLUP, 3)}, // Rx0 - GPIO_Remap_USART1
    {PA_3, UART_2, STM_PIN_DATA(STM_MODE_INPUT, GPIO_PULLUP, 0)},   // RS485_Rx1
    {PC_11, UART_4, STM_PIN_DATA(STM_MODE_INPUT, GPIO_PULLUP, 0)}, // DBG_UART_RX
    {NC,    NC,    0}
};

const PinMap PinMap_UART_RTS[] = {
    {PA_1,  UART_2, STM_PIN_DATA(STM_MODE_AF_PP, GPIO_PULLUP, 0)}, // RS485_En
    {NC,    NC,     0}
};

const PinMap PinMap_UART_CTS[] = {
    {NC,    NC,     0}
};

//*** SPI ***

const PinMap PinMap_SPI_MOSI[] = {
    { PB_15, SPI_2, STM_PIN_DATA(STM_MODE_AF_PP, GPIO_PULLUP, 0) },
    { NC, NC, 0 }
};

const PinMap PinMap_SPI_MISO[] = {
    { PB_14, SPI_2, STM_PIN_DATA(STM_MODE_AF_PP, GPIO_PULLUP, 0) },
    { NC, NC, 0 }
};

const PinMap PinMap_SPI_SCLK[] = {
    { PB_13, SPI_2, STM_PIN_DATA(STM_MODE_AF_PP, GPIO_PULLUP, 0) },
    { NC, NC, 0 }
};

const PinMap PinMap_SPI_SSEL[] = {
    { NC, NC, 0 }
};