/* mbed Microcontroller Library
 *******************************************************************************
 * Copyright (c) 2017, STMicroelectronics
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

#if DEVICE_SERIAL

#include "serial_api_hal.h"

int stdio_uart_inited = 0; // used in platform/mbed_board.c and platform/mbed_retarget.cpp
serial_t stdio_uart;

extern UART_HandleTypeDef uart_handlers[];
extern uint32_t serial_irq_ids[];

// Utility functions
HAL_StatusTypeDef init_uart(serial_t *obj);
int8_t get_uart_index(UARTName uart_name);

void serial_init(serial_t *obj, PinName tx, PinName rx)
{
    struct serial_s *obj_s = SERIAL_S(obj);
    uint8_t stdio_config = 0;

    // Determine the UART to use (UART_1, UART_2, ...)
    UARTName uart_tx = (UARTName)pinmap_peripheral(tx, PinMap_UART_TX);
    UARTName uart_rx = (UARTName)pinmap_peripheral(rx, PinMap_UART_RX);

    // Get the peripheral name (UART_1, UART_2, ...) from the pin and assign it to the object
    obj_s->uart = (UARTName)pinmap_merge(uart_tx, uart_rx);
    MBED_ASSERT(obj_s->uart != (UARTName)NC);

    if ((tx == STDIO_UART_TX) || (rx == STDIO_UART_RX)) {
        stdio_config = 1;
    }
    else {
        if (uart_tx == pinmap_peripheral(STDIO_UART_TX, PinMap_UART_TX)) {
            error("Error: new serial object is using same UART as STDIO");
        }
    }

    // Reset and enable clock
#if defined(USART1_BASE)
    if (obj_s->uart == UART_1) {
        __HAL_RCC_USART1_FORCE_RESET();
        __HAL_RCC_USART1_RELEASE_RESET();
        __HAL_RCC_USART1_CLK_ENABLE();
    }
#endif

#if defined (USART2_BASE)
    if (obj_s->uart == UART_2) {
        __HAL_RCC_USART2_FORCE_RESET();
        __HAL_RCC_USART2_RELEASE_RESET();
        __HAL_RCC_USART2_CLK_ENABLE();
    }
#endif

#if defined(USART3_BASE)
    if (obj_s->uart == UART_3) {
        __HAL_RCC_USART3_FORCE_RESET();
        __HAL_RCC_USART3_RELEASE_RESET();
        __HAL_RCC_USART3_CLK_ENABLE();
    }
#endif

#if defined(UART4_BASE)
    if (obj_s->uart == UART_4) {
        __HAL_RCC_UART4_FORCE_RESET();
        __HAL_RCC_UART4_RELEASE_RESET();
        __HAL_RCC_UART4_CLK_ENABLE();
    }
#endif

#if defined(USART4_BASE)
    if (obj_s->uart == UART_4) {
        __HAL_RCC_USART4_FORCE_RESET();
        __HAL_RCC_USART4_RELEASE_RESET();
        __HAL_RCC_USART4_CLK_ENABLE();
    }
#endif

#if defined(UART5_BASE)
    if (obj_s->uart == UART_5) {
        __HAL_RCC_UART5_FORCE_RESET();
        __HAL_RCC_UART5_RELEASE_RESET();
        __HAL_RCC_UART5_CLK_ENABLE();
    }
#endif

#if defined(USART5_BASE)
    if (obj_s->uart == UART_5) {
        __HAL_RCC_USART5_FORCE_RESET();
        __HAL_RCC_USART5_RELEASE_RESET();
        __HAL_RCC_USART5_CLK_ENABLE();
    }
#endif

#if defined(USART6_BASE)
    if (obj_s->uart == UART_6) {
        __HAL_RCC_USART6_FORCE_RESET();
        __HAL_RCC_USART6_RELEASE_RESET();
        __HAL_RCC_USART6_CLK_ENABLE();
    }
#endif

#if defined(UART7_BASE)
    if (obj_s->uart == UART_7) {
        __HAL_RCC_UART7_FORCE_RESET();
        __HAL_RCC_UART7_RELEASE_RESET();
        __HAL_RCC_UART7_CLK_ENABLE();
    }
#endif

#if defined(USART7_BASE)
    if (obj_s->uart == UART_7) {
        __HAL_RCC_USART7_FORCE_RESET();
        __HAL_RCC_USART7_RELEASE_RESET();
        __HAL_RCC_USART7_CLK_ENABLE();
    }
#endif

#if defined(UART8_BASE)
    if (obj_s->uart == UART_8) {
        __HAL_RCC_UART8_FORCE_RESET();
        __HAL_RCC_UART8_RELEASE_RESET();
        __HAL_RCC_UART8_CLK_ENABLE();
    }
#endif

#if defined(USART8_BASE)
    if (obj_s->uart == UART_8) {
        __HAL_RCC_USART8_FORCE_RESET();
        __HAL_RCC_USART8_RELEASE_RESET();
        __HAL_RCC_USART8_CLK_ENABLE();
    }
#endif

#if defined(UART9_BASE)
    if (obj_s->uart == UART_9) {
        __HAL_RCC_UART9_FORCE_RESET();
        __HAL_RCC_UART9_RELEASE_RESET();
        __HAL_RCC_UART9_CLK_ENABLE();
    }
#endif

#if defined(UART10_BASE)
    if (obj_s->uart == UART_10) {
        __HAL_RCC_UART10_FORCE_RESET();
        __HAL_RCC_UART10_RELEASE_RESET();
        __HAL_RCC_UART10_CLK_ENABLE();
    }
#endif

#if defined(LPUART1_BASE)
    if (obj_s->uart == LPUART_1) {
        __HAL_RCC_LPUART1_FORCE_RESET();
        __HAL_RCC_LPUART1_RELEASE_RESET();
        __HAL_RCC_LPUART1_CLK_ENABLE();
    }
#endif

    // Assign serial object index
    obj_s->index = get_uart_index(obj_s->uart);
    MBED_ASSERT(obj_s->index >= 0);

    // Configure UART pins
    pinmap_pinout(tx, PinMap_UART_TX);
    pinmap_pinout(rx, PinMap_UART_RX);

    if (tx != NC) {
        pin_mode(tx, PullUp);
    }
    if (rx != NC) {
        pin_mode(rx, PullUp);
    }

    // Configure UART
    obj_s->baudrate = 9600; // baudrate default value
    if (stdio_config) {
#if MBED_CONF_PLATFORM_STDIO_BAUD_RATE
        obj_s->baudrate = MBED_CONF_PLATFORM_STDIO_BAUD_RATE; // baudrate takes value from platform/mbed_lib.json
#endif /* MBED_CONF_PLATFORM_STDIO_BAUD_RATE */
    }
    else {
#if MBED_CONF_PLATFORM_DEFAULT_SERIAL_BAUD_RATE
        obj_s->baudrate = MBED_CONF_PLATFORM_DEFAULT_SERIAL_BAUD_RATE; // baudrate takes value from platform/mbed_lib.json
#endif /* MBED_CONF_PLATFORM_DEFAULT_SERIAL_BAUD_RATE */
    }
    obj_s->databits = UART_WORDLENGTH_8B;
    obj_s->stopbits = UART_STOPBITS_1;
    obj_s->parity   = UART_PARITY_NONE;

#if DEVICE_SERIAL_FC
    obj_s->hw_flow_ctl = UART_HWCONTROL_NONE;
#endif

    obj_s->pin_tx = tx;
    obj_s->pin_rx = rx;

    init_uart(obj); /* init_uart will be called again in serial_baud function, so don't worry if init_uart returns HAL_ERROR */

    // For stdio management in platform/mbed_board.c and platform/mbed_retarget.cpp
    if (stdio_config) {
        stdio_uart_inited = 1;
        memcpy(&stdio_uart, obj, sizeof(serial_t));
    }
}

void serial_free(serial_t *obj)
{
    struct serial_s *obj_s = SERIAL_S(obj);

    // Reset UART and disable clock
#if defined(USART1_BASE)
    if (obj_s->uart == UART_1) {
        __HAL_RCC_USART1_FORCE_RESET();
        __HAL_RCC_USART1_RELEASE_RESET();
        __HAL_RCC_USART1_CLK_DISABLE();
    }
#endif

#if defined(USART2_BASE)
    if (obj_s->uart == UART_2) {
        __HAL_RCC_USART2_FORCE_RESET();
        __HAL_RCC_USART2_RELEASE_RESET();
        __HAL_RCC_USART2_CLK_DISABLE();
    }
#endif

#if defined(USART3_BASE)
    if (obj_s->uart == UART_3) {
        __HAL_RCC_USART3_FORCE_RESET();
        __HAL_RCC_USART3_RELEASE_RESET();
        __HAL_RCC_USART3_CLK_DISABLE();
    }
#endif

#if defined(UART4_BASE)
    if (obj_s->uart == UART_4) {
        __HAL_RCC_UART4_FORCE_RESET();
        __HAL_RCC_UART4_RELEASE_RESET();
        __HAL_RCC_UART4_CLK_DISABLE();
    }
#endif

#if defined(USART4_BASE)
    if (obj_s->uart == UART_4) {
        __HAL_RCC_USART4_FORCE_RESET();
        __HAL_RCC_USART4_RELEASE_RESET();
        __HAL_RCC_USART4_CLK_DISABLE();
    }
#endif

#if defined(UART5_BASE)
    if (obj_s->uart == UART_5) {
        __HAL_RCC_UART5_FORCE_RESET();
        __HAL_RCC_UART5_RELEASE_RESET();
        __HAL_RCC_UART5_CLK_DISABLE();
    }
#endif

#if defined(USART5_BASE)
    if (obj_s->uart == UART_5) {
        __HAL_RCC_USART5_FORCE_RESET();
        __HAL_RCC_USART5_RELEASE_RESET();
        __HAL_RCC_USART5_CLK_DISABLE();
    }
#endif

#if defined(USART6_BASE)
    if (obj_s->uart == UART_6) {
        __HAL_RCC_USART6_FORCE_RESET();
        __HAL_RCC_USART6_RELEASE_RESET();
        __HAL_RCC_USART6_CLK_DISABLE();
    }
#endif

#if defined(UART7_BASE)
    if (obj_s->uart == UART_7) {
        __HAL_RCC_UART7_FORCE_RESET();
        __HAL_RCC_UART7_RELEASE_RESET();
        __HAL_RCC_UART7_CLK_DISABLE();
    }
#endif

#if defined(USART7_BASE)
    if (obj_s->uart == UART_7) {
        __HAL_RCC_USART7_FORCE_RESET();
        __HAL_RCC_USART7_RELEASE_RESET();
        __HAL_RCC_USART7_CLK_DISABLE();
    }
#endif

#if defined(UART8_BASE)
    if (obj_s->uart == UART_8) {
        __HAL_RCC_UART8_FORCE_RESET();
        __HAL_RCC_UART8_RELEASE_RESET();
        __HAL_RCC_UART8_CLK_DISABLE();
    }
#endif

#if defined(USART8_BASE)
    if (obj_s->uart == UART_8) {
        __HAL_RCC_USART8_FORCE_RESET();
        __HAL_RCC_USART8_RELEASE_RESET();
        __HAL_RCC_USART8_CLK_DISABLE();
    }
#endif

#if defined(UART9_BASE)
    if (obj_s->uart == UART_9) {
        __HAL_RCC_UART9_FORCE_RESET();
        __HAL_RCC_UART9_RELEASE_RESET();
        __HAL_RCC_UART9_CLK_DISABLE();
    }
#endif

#if defined(UART10_BASE)
    if (obj_s->uart == UART_10) {
        __HAL_RCC_UART10_FORCE_RESET();
        __HAL_RCC_UART10_RELEASE_RESET();
        __HAL_RCC_UART10_CLK_DISABLE();
    }
#endif

#if defined(LPUART1_BASE)
    if (obj_s->uart == LPUART_1) {
        __HAL_RCC_LPUART1_FORCE_RESET();
        __HAL_RCC_LPUART1_RELEASE_RESET();
        __HAL_RCC_LPUART1_CLK_DISABLE();
    }
#endif

    // Configure GPIOs
    pin_function(obj_s->pin_tx, STM_PIN_DATA(STM_MODE_INPUT, GPIO_NOPULL, 0));
    pin_function(obj_s->pin_rx, STM_PIN_DATA(STM_MODE_INPUT, GPIO_NOPULL, 0));

    serial_irq_ids[obj_s->index] = 0;
}

void serial_baud(serial_t *obj, int baudrate)
{
    struct serial_s *obj_s = SERIAL_S(obj);

    obj_s->baudrate = baudrate;
#if defined(LPUART1_BASE)
        /* Note that LPUART clock source must be in the range [3 x baud rate, 4096 x baud rate], check Ref Manual */
    if (obj_s->uart == LPUART_1) {
        /* If baudrate is lower than 9600 try to change to LSE */
        RCC_PeriphCLKInitTypeDef PeriphClkInitStruct = {0};
        if (baudrate <= 9600 && __HAL_RCC_GET_FLAG(RCC_FLAG_LSERDY)) {
            PeriphClkInitStruct.PeriphClockSelection = RCC_PERIPHCLK_LPUART1;
            PeriphClkInitStruct.Lpuart1ClockSelection = RCC_LPUART1CLKSOURCE_LSE;
            HAL_RCCEx_PeriphCLKConfig(&PeriphClkInitStruct);
        } else {
            PeriphClkInitStruct.PeriphClockSelection = RCC_PERIPHCLK_LPUART1;
            PeriphClkInitStruct.Lpuart1ClockSelection = RCC_LPUART1CLKSOURCE_PCLK1;
            HAL_RCCEx_PeriphCLKConfig(&PeriphClkInitStruct);
        }
        if (init_uart(obj) == HAL_OK) {
                return;
        }
        /* Change LPUART clock source and try again */
        PeriphClkInitStruct.PeriphClockSelection = RCC_PERIPHCLK_LPUART1;
        PeriphClkInitStruct.Lpuart1ClockSelection = RCC_LPUART1CLKSOURCE_SYSCLK;
        HAL_RCCEx_PeriphCLKConfig(&PeriphClkInitStruct);
    }
#endif /* LPUART1_BASE */
    if (init_uart(obj) != HAL_OK) {
        debug("Cannot initialize UART with baud rate %u\n", baudrate);
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
        case 7:
            if (parity != UART_PARITY_NONE) {
                obj_s->databits = UART_WORDLENGTH_8B;
            } else {
#if defined UART_WORDLENGTH_7B
                obj_s->databits = UART_WORDLENGTH_7B;
#else
                error("7-bit data format without parity is not supported");
#endif
            }
            break;
        case 8:
            if (parity != UART_PARITY_NONE) {
                obj_s->databits = UART_WORDLENGTH_9B;
            } else {
                obj_s->databits = UART_WORDLENGTH_8B;
            }
            break;
        case 9:
            if (parity != UART_PARITY_NONE) {
                error("Parity is not supported with 9-bit data format");
            } else {
                obj_s->databits = UART_WORDLENGTH_9B;
            }
            break;
        default:
            error("Only 7, 8 or 9-bit data formats are supported");
            break;
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

/******************************************************************************
 * UTILITY FUNCTIONS
 ******************************************************************************/

HAL_StatusTypeDef init_uart(serial_t *obj)
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

#if defined(LPUART1_BASE)
    if (huart->Instance == LPUART1) {
        if (obj_s->baudrate <= 9600) {
            HAL_UARTEx_EnableClockStopMode(huart);
            HAL_UARTEx_EnableStopMode(huart);
        } else {
            HAL_UARTEx_DisableClockStopMode(huart);
            HAL_UARTEx_DisableStopMode(huart);
        }
    }
#endif

    return HAL_UART_Init(huart);
}

int8_t get_uart_index(UARTName uart_name)
{
    uint8_t index = 0;

#if defined(USART1_BASE)
    if (uart_name == UART_1) {
        return index;
    }
    index++;
#endif

#if defined(USART2_BASE)
    if (uart_name == UART_2) {
        return index;
    }
    index++;
#endif

#if defined(USART3_BASE)
    if (uart_name == UART_3) {
        return index;
    }
    index++;
#endif

#if defined(UART4_BASE)
    if (uart_name == UART_4) {
        return index;
    }
    index++;
#endif

#if defined(USART4_BASE)
    if (uart_name == UART_4) {
        return index;
    }
    index++;
#endif

#if defined(UART5_BASE)
    if (uart_name == UART_5) {
        return index;
    }
    index++;
#endif

#if defined(USART5_BASE)
    if (uart_name == UART_5) {
        return index;
    }
    index++;
#endif

#if defined(USART6_BASE)
    if (uart_name == UART_6) {
        return index;
    }
    index++;
#endif

#if defined(UART7_BASE)
    if (uart_name == UART_7) {
        return index;
    }
    index++;
#endif

#if defined(USART7_BASE)
    if (uart_name == UART_7) {
        return index;
    }
    index++;
#endif

#if defined(UART8_BASE)
    if (uart_name == UART_8) {
        return index;
    }
    index++;
#endif

#if defined(USART8_BASE)
    if (uart_name == UART_8) {
        return index;
    }
    index++;
#endif

#if defined(UART9_BASE)
    if (uart_name == UART_9) {
        return index;
    }
    index++;
#endif

#if defined(UART10_BASE)
    if (uart_name == UART_10) {
        return index;
    }
    index++;
#endif

#if defined(LPUART1_BASE)
    if (uart_name == LPUART_1) {
        return index;
    }
    index++;
#endif

    return -1;
}

#endif /* DEVICE_SERIAL */
