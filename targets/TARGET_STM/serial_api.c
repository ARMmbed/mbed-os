/* mbed Microcontroller Library
 *******************************************************************************
 * Copyright (c) 2015, STMicroelectronics
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
#include "mbed_assert.h"
#include "mbed_error.h"
#include "serial_api.h"
#include "serial_api_hal.h"
#include "PeripheralPins.h"

#if DEVICE_SERIAL

void init_uart(serial_t *obj)
{
    struct serial_s *obj_s = SERIAL_S(obj);
    UART_HandleTypeDef *huart = &uart_handlers[obj_s->index];
    huart->Instance = (USART_TypeDef *)(obj_s->uart);

    huart->Init.BaudRate     = obj_s->baudrate;
    huart->Init.WordLength   = obj_s->databits;
    huart->Init.StopBits     = obj_s->stopbits;
    huart->Init.Parity       = obj_s->parity;
#if DEVICE_SERIAL_FC
    huart->Init.HwFlowCtl    = obj_s->hw_flow_ctl;
#else
    huart->Init.HwFlowCtl    = UART_HWCONTROL_NONE;
#endif
    huart->Init.OverSampling = UART_OVERSAMPLING_16;
    huart->TxXferCount       = 0;
    huart->TxXferSize        = 0;
    huart->RxXferCount       = 0;
    huart->RxXferSize        = 0;

    if (obj_s->pin_rx == NC) {
        huart->Init.Mode = UART_MODE_TX;
    } else if (obj_s->pin_tx == NC) {
        huart->Init.Mode = UART_MODE_RX;
    } else {
        huart->Init.Mode = UART_MODE_TX_RX;
    }

    if (HAL_UART_Init(huart) != HAL_OK) {
        error("Cannot initialize UART\n");
    }
}

void serial_format(serial_t *obj, int data_bits, SerialParity parity, int stop_bits)
{
    struct serial_s *obj_s = SERIAL_S(obj);

    switch (parity) {
        case ParityOdd:
            obj_s->parity = UART_PARITY_ODD;
            break;
        case ParityEven:
            obj_s->parity = UART_PARITY_EVEN;
            break;
        default: // ParityNone
        case ParityForced0: // unsupported!
        case ParityForced1: // unsupported!
            obj_s->parity = UART_PARITY_NONE;
            break;
    }

    switch (data_bits) {
        case 9:
            MBED_ASSERT(parity == UART_PARITY_NONE);
            obj_s->databits = UART_WORDLENGTH_9B;
            break;
        default:
        case 8:
            if (parity != UART_PARITY_NONE) {
                obj_s->databits = UART_WORDLENGTH_9B;
            } else {
                obj_s->databits = UART_WORDLENGTH_8B;
            }
            break;
#if defined UART_WORDLENGTH_7B
        case 7:
            if (parity != UART_PARITY_NONE) {
                obj_s->databits = UART_WORDLENGTH_8B;
            } else {
                obj_s->databits = UART_WORDLENGTH_7B;
            }
            break;
#endif
    }

    if (stop_bits == 2) {
        obj_s->stopbits = UART_STOPBITS_2;
    } else {
        obj_s->stopbits = UART_STOPBITS_1;
    }

    init_uart(obj);
}

/******************************************************************************
 * READ/WRITE
 ******************************************************************************/

int serial_readable(serial_t *obj)
{
    struct serial_s *obj_s = SERIAL_S(obj);
    UART_HandleTypeDef *huart = &uart_handlers[obj_s->index];
    /*  To avoid a target blocking case, let's check for
     *  possible OVERRUN error and discard it
     */
    if(__HAL_UART_GET_FLAG(huart, UART_FLAG_ORE)) {
        __HAL_UART_CLEAR_OREFLAG(huart);
    }
    // Check if data is received
    return (__HAL_UART_GET_FLAG(huart, UART_FLAG_RXNE) != RESET) ? 1 : 0;
}

int serial_writable(serial_t *obj)
{
    struct serial_s *obj_s = SERIAL_S(obj);
    UART_HandleTypeDef *huart = &uart_handlers[obj_s->index];

    // Check if data is transmitted
    return (__HAL_UART_GET_FLAG(huart, UART_FLAG_TXE) != RESET) ? 1 : 0;
}

void serial_pinout_tx(PinName tx)
{
    pinmap_pinout(tx, PinMap_UART_TX);
}

void serial_break_clear(serial_t *obj)
{
    (void)obj;
}

#endif /* DEVICE_SERIAL */
