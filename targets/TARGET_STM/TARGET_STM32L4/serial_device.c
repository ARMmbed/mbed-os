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
#include "serial_api.h"
#include "serial_api_hal.h"

#if DEVICE_SERIAL

#include "cmsis.h"
#include "pinmap.h"
#include "mbed_error.h"
#include <string.h>
#include "PeripheralPins.h"

#define UART_NUM (6)

static uint32_t serial_irq_ids[UART_NUM] = {0};
UART_HandleTypeDef uart_handlers[UART_NUM];

static uart_irq_handler irq_handler;

int stdio_uart_inited = 0;
serial_t stdio_uart;

void serial_init(serial_t *obj, PinName tx, PinName rx)
{
    struct serial_s *obj_s = SERIAL_S(obj);
    
    // Determine the UART to use (UART_1, UART_2, ...)
    UARTName uart_tx = (UARTName)pinmap_peripheral(tx, PinMap_UART_TX);
    UARTName uart_rx = (UARTName)pinmap_peripheral(rx, PinMap_UART_RX);

    // Get the peripheral name (UART_1, UART_2, ...) from the pin and assign it to the object
    obj_s->uart = (UARTName)pinmap_merge(uart_tx, uart_rx);
    MBED_ASSERT(obj_s->uart != (UARTName)NC);

    // Enable USART clock
    if (obj_s->uart == UART_1) {
        __HAL_RCC_USART1_FORCE_RESET();
        __HAL_RCC_USART1_RELEASE_RESET();
        __HAL_RCC_USART1_CLK_ENABLE();
        obj_s->index = 0;
    }
    if (obj_s->uart == UART_2) {
        __HAL_RCC_USART2_FORCE_RESET();
        __HAL_RCC_USART2_RELEASE_RESET();
        __HAL_RCC_USART2_CLK_ENABLE();
        obj_s->index = 1;
    }

#if defined(USART3_BASE)
    if (obj_s->uart == UART_3) {
        __HAL_RCC_USART3_FORCE_RESET();
        __HAL_RCC_USART3_RELEASE_RESET();
        __HAL_RCC_USART3_CLK_ENABLE();
        obj_s->index = 2;
    }
#endif

#if defined(UART4_BASE)
    if (obj_s->uart == UART_4) {
        __HAL_RCC_UART4_FORCE_RESET();
        __HAL_RCC_UART4_RELEASE_RESET();
        __HAL_RCC_UART4_CLK_ENABLE();
        obj_s->index = 3;
    }
#endif

#if defined(UART5_BASE)
    if (obj_s->uart == UART_5) {
        __HAL_RCC_UART5_FORCE_RESET();
        __HAL_RCC_UART5_RELEASE_RESET();
        __HAL_RCC_UART5_CLK_ENABLE();
        obj_s->index = 4;
    }
#endif

#if defined(LPUART1_BASE)
    if (obj_s->uart == LPUART_1) {
        __HAL_RCC_LPUART1_FORCE_RESET();
        __HAL_RCC_LPUART1_RELEASE_RESET();
        __HAL_RCC_LPUART1_CLK_ENABLE();
        obj_s->index = 5;
    }
#endif

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
    obj_s->baudrate = 9600;
    obj_s->databits = UART_WORDLENGTH_8B;
    obj_s->stopbits = UART_STOPBITS_1;
    obj_s->parity   = UART_PARITY_NONE;
    
#if DEVICE_SERIAL_FC
    obj_s->hw_flow_ctl = UART_HWCONTROL_NONE;
#endif

    obj_s->pin_tx = tx;
    obj_s->pin_rx = rx;

    init_uart(obj);

    // For stdio management
    if (obj_s->uart == STDIO_UART) {
        stdio_uart_inited = 1;
        memcpy(&stdio_uart, obj, sizeof(serial_t));
    }
}

void serial_free(serial_t *obj)
{
    struct serial_s *obj_s = SERIAL_S(obj);
      
    // Reset UART and disable clock
    if (obj_s->uart == UART_1) {
        __HAL_RCC_USART1_FORCE_RESET();
        __HAL_RCC_USART1_RELEASE_RESET();
        __HAL_RCC_USART1_CLK_DISABLE();
    }

    if (obj_s->uart == UART_2) {
        __HAL_RCC_USART2_FORCE_RESET();
        __HAL_RCC_USART2_RELEASE_RESET();
        __HAL_RCC_USART2_CLK_DISABLE();
    }

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

#if defined(UART5_BASE)
    if (obj_s->uart == UART_5) {
        __HAL_RCC_UART5_FORCE_RESET();
        __HAL_RCC_UART5_RELEASE_RESET();
        __HAL_RCC_UART5_CLK_DISABLE();
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
    init_uart(obj);
}

/******************************************************************************
 * INTERRUPTS HANDLING
 ******************************************************************************/

static void uart_irq(int id)
{
    UART_HandleTypeDef * huart = &uart_handlers[id];
    
    if (serial_irq_ids[id] != 0) {
        if (__HAL_UART_GET_FLAG(huart, UART_FLAG_TXE) != RESET) {
            if (__HAL_UART_GET_IT(huart, UART_IT_TXE) != RESET) {
                irq_handler(serial_irq_ids[id], TxIrq);
            }
        }
        if (__HAL_UART_GET_FLAG(huart, UART_FLAG_RXNE) != RESET) {
            if (__HAL_UART_GET_IT(huart, UART_IT_RXNE) != RESET) {
                irq_handler(serial_irq_ids[id], RxIrq);
                /*  Flag has been cleared when reading the content */
            }
        }
        if (__HAL_UART_GET_FLAG(huart, UART_FLAG_ORE) != RESET) {
            if (__HAL_UART_GET_IT(huart, UART_IT_ORE) != RESET) {
                volatile uint32_t tmpval __attribute__((unused)) = huart->Instance->RDR; // Clear ORE flag
            }
        }
    }
}

static void uart1_irq(void)
{
    uart_irq(0);
}

static void uart2_irq(void)
{
    uart_irq(1);
}

#if defined(USART3_BASE)
static void uart3_irq(void)
{
    uart_irq(2);
}
#endif

#if defined(UART4_BASE)
static void uart4_irq(void)
{
    uart_irq(3);
}
#endif

#if defined(UART5_BASE)
static void uart5_irq(void)
{
    uart_irq(4);
}
#endif

#if defined(LPUART1_BASE)
static void lpuart1_irq(void)
{
    uart_irq(5);
}
#endif

void serial_irq_handler(serial_t *obj, uart_irq_handler handler, uint32_t id)
{
    struct serial_s *obj_s = SERIAL_S(obj);
  
    irq_handler = handler;
    serial_irq_ids[obj_s->index] = id;
}

void serial_irq_set(serial_t *obj, SerialIrq irq, uint32_t enable)
{
    struct serial_s *obj_s = SERIAL_S(obj);
    UART_HandleTypeDef *huart = &uart_handlers[obj_s->index];
    IRQn_Type irq_n = (IRQn_Type)0;
    uint32_t vector = 0;

    if (obj_s->uart == UART_1) {
        irq_n = USART1_IRQn;
        vector = (uint32_t)&uart1_irq;
    }

    if (obj_s->uart == UART_2) {
        irq_n = USART2_IRQn;
        vector = (uint32_t)&uart2_irq;
    }
#if defined(USART3_BASE)
    if (obj_s->uart == UART_3) {
        irq_n = USART3_IRQn;
        vector = (uint32_t)&uart3_irq;
    }
#endif
#if defined(UART4_BASE)
    if (obj_s->uart == UART_4) {
        irq_n = UART4_IRQn;
        vector = (uint32_t)&uart4_irq;
    }
#endif

#if defined(UART5_BASE)
    if (obj_s->uart == UART_5) {
        irq_n = UART5_IRQn;
        vector = (uint32_t)&uart5_irq;
    }
#endif

#if defined(LPUART1_BASE)
    if (obj_s->uart == LPUART_1) {
        irq_n = LPUART1_IRQn;
        vector = (uint32_t)&lpuart1_irq;
    }
#endif

    if (enable) {
        if (irq == RxIrq) {
            __HAL_UART_ENABLE_IT(huart, UART_IT_RXNE);
        } else { // TxIrq
            __HAL_UART_ENABLE_IT(huart, UART_IT_TXE);
        }
        NVIC_SetVector(irq_n, vector);
        NVIC_EnableIRQ(irq_n);

    } else { // disable
        int all_disabled = 0;
        if (irq == RxIrq) {
            __HAL_UART_DISABLE_IT(huart, UART_IT_RXNE);
            // Check if TxIrq is disabled too
            if ((huart->Instance->CR1 & USART_CR1_TXEIE) == 0) {
                all_disabled = 1;
            }
        } else { // TxIrq
            __HAL_UART_DISABLE_IT(huart, UART_IT_TXE);
            // Check if RxIrq is disabled too
            if ((huart->Instance->CR1 & USART_CR1_RXNEIE) == 0) {
                all_disabled = 1;
            }
        }

        if (all_disabled) {
            NVIC_DisableIRQ(irq_n);
        }
    }
}

/******************************************************************************
 * READ/WRITE
 ******************************************************************************/

int serial_getc(serial_t *obj)
{
    struct serial_s *obj_s = SERIAL_S(obj);
    UART_HandleTypeDef *huart = &uart_handlers[obj_s->index];

    while (!serial_readable(obj));
    if (obj_s->databits == UART_WORDLENGTH_8B) {
        return (int)(huart->Instance->RDR & (uint8_t)0xFF);
    } else {
        return (int)(huart->Instance->RDR & (uint16_t)0x1FF);
    }
}

void serial_putc(serial_t *obj, int c)
{
    struct serial_s *obj_s = SERIAL_S(obj);
    UART_HandleTypeDef *huart = &uart_handlers[obj_s->index];

    while (!serial_writable(obj));
    if (obj_s->databits == UART_WORDLENGTH_8B) {
        huart->Instance->TDR = (uint8_t)(c & (uint8_t)0xFF);
    } else {
        huart->Instance->TDR = (uint16_t)(c & (uint16_t)0x1FF);
    }
}

void serial_clear(serial_t *obj)
{
    struct serial_s *obj_s = SERIAL_S(obj);
    UART_HandleTypeDef *huart = &uart_handlers[obj_s->index];

    huart->TxXferCount = 0;
    huart->RxXferCount = 0;
}

void serial_break_set(serial_t *obj)
{
    struct serial_s *obj_s = SERIAL_S(obj);
    UART_HandleTypeDef *huart = &uart_handlers[obj_s->index];

    HAL_LIN_SendBreak(huart);
}

#if DEVICE_SERIAL_ASYNCH

/******************************************************************************
 * LOCAL HELPER FUNCTIONS
 ******************************************************************************/

/** 
 * Configure the TX buffer for an asynchronous write serial transaction
 *
 * @param obj       The serial object.
 * @param tx        The buffer for sending.
 * @param tx_length The number of words to transmit.
 */
static void serial_tx_buffer_set(serial_t *obj, void *tx, int tx_length, uint8_t width)
{
    (void)width;

    // Exit if a transmit is already on-going
    if (serial_tx_active(obj)) {
        return;
    }

    obj->tx_buff.buffer = tx;
    obj->tx_buff.length = tx_length;
    obj->tx_buff.pos = 0;
}
  
/**
 * Configure the RX buffer for an asynchronous write serial transaction
 *
 * @param obj       The serial object.
 * @param tx        The buffer for sending.
 * @param tx_length The number of words to transmit.
 */
static void serial_rx_buffer_set(serial_t *obj, void *rx, int rx_length, uint8_t width)
{
    (void)width;

    // Exit if a reception is already on-going
    if (serial_rx_active(obj)) {
        return;
    }

    obj->rx_buff.buffer = rx;
    obj->rx_buff.length = rx_length;
    obj->rx_buff.pos = 0;
}

/** 
 * Configure events
 *
 * @param obj    The serial object
 * @param event  The logical OR of the events to configure
 * @param enable Set to non-zero to enable events, or zero to disable them
 */
static void serial_enable_event(serial_t *obj, int event, uint8_t enable)
{  
    struct serial_s *obj_s = SERIAL_S(obj);
    
    // Shouldn't have to enable interrupt here, just need to keep track of the requested events.
    if (enable) {
        obj_s->events |= event;
    } else {
        obj_s->events &= ~event;
    }
}


/**
* Get index of serial object TX IRQ, relating it to the physical peripheral.
*
* @param obj pointer to serial object
* @return internal NVIC TX IRQ index of U(S)ART peripheral
*/
static IRQn_Type serial_get_irq_n(serial_t *obj)
{
    struct serial_s *obj_s = SERIAL_S(obj);
    IRQn_Type irq_n;

    switch (obj_s->index) {
        case 0:
            irq_n = USART1_IRQn;
            break;

        case 1:
            irq_n = USART2_IRQn;
            break;

#if defined(UART3_BASE)
        case 2:
            irq_n = USART3_IRQn;
            break;
#endif
#if defined(UART4_BASE)
        case 3:
            irq_n = UART4_IRQn;
            break;
#endif
#if defined(UART5_BASE)
        case 4:
            irq_n = UART5_IRQn;
            break;
#endif
#if defined(LPUART1_BASE)
        case 5:
            irq_n = LPUART1_IRQn;
            break;
#endif

        default:
            irq_n = (IRQn_Type)0;
    }
    
    return irq_n;
}

/******************************************************************************
 * MBED API FUNCTIONS
 ******************************************************************************/

/** 
 * Begin asynchronous TX transfer. The used buffer is specified in the serial
 * object, tx_buff
 *
 * @param obj       The serial object
 * @param tx        The buffer for sending
 * @param tx_length The number of words to transmit
 * @param tx_width  The bit width of buffer word
 * @param handler   The serial handler
 * @param event     The logical OR of events to be registered
 * @param hint      A suggestion for how to use DMA with this transfer
 * @return Returns number of data transfered, or 0 otherwise
 */
int serial_tx_asynch(serial_t *obj, const void *tx, size_t tx_length, uint8_t tx_width, uint32_t handler, uint32_t event, DMAUsage hint)
{    
    // TODO: DMA usage is currently ignored
    (void) hint;
    
    // Check buffer is ok
    MBED_ASSERT(tx != (void*)0);
    MBED_ASSERT(tx_width == 8); // support only 8b width
    
    struct serial_s *obj_s = SERIAL_S(obj);
    UART_HandleTypeDef * huart = &uart_handlers[obj_s->index];

    if (tx_length == 0) {
        return 0;
    }
  
    // Set up buffer
    serial_tx_buffer_set(obj, (void *)tx, tx_length, tx_width);
  
    // Set up events
    serial_enable_event(obj, SERIAL_EVENT_TX_ALL, 0); // Clear all events
    serial_enable_event(obj, event, 1); // Set only the wanted events
    
    // Enable interrupt
    IRQn_Type irq_n = serial_get_irq_n(obj);
    NVIC_ClearPendingIRQ(irq_n);
    NVIC_DisableIRQ(irq_n);
    NVIC_SetPriority(irq_n, 1);
    NVIC_SetVector(irq_n, (uint32_t)handler);
    NVIC_EnableIRQ(irq_n);

    // the following function will enable UART_IT_TXE and error interrupts
    if (HAL_UART_Transmit_IT(huart, (uint8_t*)tx, tx_length) != HAL_OK) {
        return 0;
    }
    
    return tx_length;
}

/** 
 * Begin asynchronous RX transfer (enable interrupt for data collecting)
 * The used buffer is specified in the serial object, rx_buff
 *
 * @param obj        The serial object
 * @param rx         The buffer for sending
 * @param rx_length  The number of words to transmit
 * @param rx_width   The bit width of buffer word
 * @param handler    The serial handler
 * @param event      The logical OR of events to be registered
 * @param handler    The serial handler
 * @param char_match A character in range 0-254 to be matched
 * @param hint       A suggestion for how to use DMA with this transfer
 */
void serial_rx_asynch(serial_t *obj, void *rx, size_t rx_length, uint8_t rx_width, uint32_t handler, uint32_t event, uint8_t char_match, DMAUsage hint)
{
    // TODO: DMA usage is currently ignored
    (void) hint;

    /* Sanity check arguments */
    MBED_ASSERT(obj);
    MBED_ASSERT(rx != (void*)0);
    MBED_ASSERT(rx_width == 8); // support only 8b width
    
    struct serial_s *obj_s = SERIAL_S(obj);
    UART_HandleTypeDef *huart = &uart_handlers[obj_s->index];

    serial_enable_event(obj, SERIAL_EVENT_RX_ALL, 0);
    serial_enable_event(obj, event, 1);
    
    // set CharMatch
    obj->char_match = char_match;
    
    serial_rx_buffer_set(obj, rx, rx_length, rx_width);

    IRQn_Type irq_n = serial_get_irq_n(obj);
    NVIC_ClearPendingIRQ(irq_n);
    NVIC_DisableIRQ(irq_n);
    NVIC_SetPriority(irq_n, 0);
    NVIC_SetVector(irq_n, (uint32_t)handler);
    NVIC_EnableIRQ(irq_n);

    // following HAL function will enable the RXNE interrupt + error interrupts    
    HAL_UART_Receive_IT(huart, (uint8_t*)rx, rx_length);
}

/**
 * Attempts to determine if the serial peripheral is already in use for TX
 *
 * @param obj The serial object
 * @return Non-zero if the TX transaction is ongoing, 0 otherwise
 */
uint8_t serial_tx_active(serial_t *obj)
{
    MBED_ASSERT(obj);
    
    struct serial_s *obj_s = SERIAL_S(obj);
    UART_HandleTypeDef *huart = &uart_handlers[obj_s->index];
    
    return ((HAL_UART_GetState(huart) == HAL_UART_STATE_BUSY_TX) ? 1 : 0);
}

/**
 * Attempts to determine if the serial peripheral is already in use for RX
 *
 * @param obj The serial object
 * @return Non-zero if the RX transaction is ongoing, 0 otherwise
 */
uint8_t serial_rx_active(serial_t *obj)
{
    MBED_ASSERT(obj);
    
    struct serial_s *obj_s = SERIAL_S(obj);
    UART_HandleTypeDef *huart = &uart_handlers[obj_s->index];
    
    return ((HAL_UART_GetState(huart) == HAL_UART_STATE_BUSY_RX) ? 1 : 0);
}

void HAL_UART_TxCpltCallback(UART_HandleTypeDef *huart) {
    if (__HAL_UART_GET_FLAG(huart, UART_FLAG_TC) != RESET) {
        __HAL_UART_CLEAR_FLAG(huart, UART_FLAG_TC);
}
}

void HAL_UART_ErrorCallback(UART_HandleTypeDef *huart) {
    if (__HAL_UART_GET_FLAG(huart, UART_FLAG_PE) != RESET) {
        volatile uint32_t tmpval __attribute__((unused)) = huart->Instance->RDR; // Clear PE flag
    } else if (__HAL_UART_GET_FLAG(huart, UART_FLAG_FE) != RESET) {
        volatile uint32_t tmpval __attribute__((unused)) = huart->Instance->RDR; // Clear FE flag
    } else if (__HAL_UART_GET_FLAG(huart, UART_FLAG_NE) != RESET) {
        volatile uint32_t tmpval __attribute__((unused)) = huart->Instance->RDR; // Clear NE flag
    } else if (__HAL_UART_GET_FLAG(huart, UART_FLAG_ORE) != RESET) {
        volatile uint32_t tmpval __attribute__((unused)) = huart->Instance->RDR; // Clear ORE flag
    }
}

/**
 * The asynchronous TX and RX handler.
 *
 * @param obj The serial object
 * @return Returns event flags if a TX/RX transfer termination condition was met or 0 otherwise
 */
int serial_irq_handler_asynch(serial_t *obj)
{
    struct serial_s *obj_s = SERIAL_S(obj);
    UART_HandleTypeDef *huart = &uart_handlers[obj_s->index];
    
    volatile int return_event = 0;
    uint8_t *buf = (uint8_t*)(obj->rx_buff.buffer);
    uint8_t i = 0;
    
    // TX PART:
    if (__HAL_UART_GET_FLAG(huart, UART_FLAG_TC) != RESET) {
        if (__HAL_UART_GET_IT_SOURCE(huart, UART_IT_TC) != RESET) {
            // Return event SERIAL_EVENT_TX_COMPLETE if requested
            if ((obj_s->events & SERIAL_EVENT_TX_COMPLETE ) != 0) {
                return_event |= (SERIAL_EVENT_TX_COMPLETE & obj_s->events);
            }
        }
    }
    
    // Handle error events
    if (__HAL_UART_GET_FLAG(huart, UART_FLAG_PE) != RESET) {
        if (__HAL_UART_GET_IT(huart, UART_IT_PE) != RESET) {
            return_event |= (SERIAL_EVENT_RX_PARITY_ERROR & obj_s->events);
        }
}

    if (__HAL_UART_GET_FLAG(huart, UART_FLAG_FE) != RESET) {
        if (__HAL_UART_GET_IT(huart, UART_IT_FE) != RESET) {
            return_event |= (SERIAL_EVENT_RX_FRAMING_ERROR & obj_s->events);
        }
    }
    
    if (__HAL_UART_GET_FLAG(huart, UART_FLAG_ORE) != RESET) {
        if (__HAL_UART_GET_IT(huart, UART_IT_ORE) != RESET) {
            return_event |= (SERIAL_EVENT_RX_OVERRUN_ERROR & obj_s->events);
        }
    }
    
    HAL_UART_IRQHandler(huart);
    
    // Abort if an error occurs
    if ((return_event & SERIAL_EVENT_RX_PARITY_ERROR) ||
        (return_event & SERIAL_EVENT_RX_FRAMING_ERROR) ||
        (return_event & SERIAL_EVENT_RX_OVERRUN_ERROR)) {
        return return_event;
    }
    
    //RX PART
    if (huart->RxXferSize != 0) {
        obj->rx_buff.pos = huart->RxXferSize - huart->RxXferCount;
    }
    if ((huart->RxXferCount == 0) && (obj->rx_buff.pos >= (obj->rx_buff.length - 1))) {
        return_event |= (SERIAL_EVENT_RX_COMPLETE & obj_s->events);
    }
    
    // Check if char_match is present
    if (obj_s->events & SERIAL_EVENT_RX_CHARACTER_MATCH) {
        if (buf != NULL) {
            for (i = 0; i < obj->rx_buff.pos; i++) {
                if (buf[i] == obj->char_match) {
                    obj->rx_buff.pos = i;
                    return_event |= (SERIAL_EVENT_RX_CHARACTER_MATCH & obj_s->events);
                    serial_rx_abort_asynch(obj);
                    break;
                }
            }
        }
    }
    
    return return_event;  
}

/** 
 * Abort the ongoing TX transaction. It disables the enabled interupt for TX and
 * flush TX hardware buffer if TX FIFO is used
 *
 * @param obj The serial object
 */
void serial_tx_abort_asynch(serial_t *obj)
{
    struct serial_s *obj_s = SERIAL_S(obj);
    UART_HandleTypeDef *huart = &uart_handlers[obj_s->index];
    
    __HAL_UART_DISABLE_IT(huart, UART_IT_TC);
    __HAL_UART_DISABLE_IT(huart, UART_IT_TXE);
    
    // clear flags
    __HAL_UART_CLEAR_FLAG(huart, UART_FLAG_TC);
    
    // reset states
    huart->TxXferCount = 0;
    // update handle state
    if(huart->gState == HAL_UART_STATE_BUSY_TX_RX) {
        huart->gState = HAL_UART_STATE_BUSY_RX;
    } else {
        huart->gState = HAL_UART_STATE_READY;
    }
}

/**
 * Abort the ongoing RX transaction It disables the enabled interrupt for RX and
 * flush RX hardware buffer if RX FIFO is used
 *
 * @param obj The serial object
 */
void serial_rx_abort_asynch(serial_t *obj)
{
    struct serial_s *obj_s = SERIAL_S(obj);
    UART_HandleTypeDef *huart = &uart_handlers[obj_s->index];
    
    // disable interrupts
    __HAL_UART_DISABLE_IT(huart, UART_IT_RXNE);
    __HAL_UART_DISABLE_IT(huart, UART_IT_PE);
    __HAL_UART_DISABLE_IT(huart, UART_IT_ERR);
    
    // clear flags
    __HAL_UART_CLEAR_FLAG(huart, UART_FLAG_RXNE);
    volatile uint32_t tmpval __attribute__((unused)) = huart->Instance->RDR; // Clear errors flag
    
    // reset states
    huart->RxXferCount = 0;
    // update handle state
    if(huart->RxState == HAL_UART_STATE_BUSY_TX_RX) {
        huart->RxState = HAL_UART_STATE_BUSY_TX;
    } else {
        huart->RxState = HAL_UART_STATE_READY;
    }
}

#endif

#if DEVICE_SERIAL_FC

/**
 * Set HW Control Flow
 * @param obj    The serial object
 * @param type   The Control Flow type (FlowControlNone, FlowControlRTS, FlowControlCTS, FlowControlRTSCTS)
 * @param rxflow Pin for the rxflow
 * @param txflow Pin for the txflow
 */
void serial_set_flow_control(serial_t *obj, FlowControl type, PinName rxflow, PinName txflow)
{
    struct serial_s *obj_s = SERIAL_S(obj);

    // Determine the UART to use (UART_1, UART_2, ...)
    UARTName uart_rts = (UARTName)pinmap_peripheral(rxflow, PinMap_UART_RTS);
    UARTName uart_cts = (UARTName)pinmap_peripheral(txflow, PinMap_UART_CTS);

    // Get the peripheral name (UART_1, UART_2, ...) from the pin and assign it to the object
    obj_s->uart = (UARTName)pinmap_merge(uart_cts, uart_rts);
    MBED_ASSERT(obj_s->uart != (UARTName)NC);

    if(type == FlowControlNone) {
        // Disable hardware flow control
      obj_s->hw_flow_ctl = UART_HWCONTROL_NONE;
    }
    if (type == FlowControlRTS) {
        // Enable RTS
        MBED_ASSERT(uart_rts != (UARTName)NC);
        obj_s->hw_flow_ctl = UART_HWCONTROL_RTS;
        obj_s->pin_rts = rxflow;
        // Enable the pin for RTS function
        pinmap_pinout(rxflow, PinMap_UART_RTS);
    }
    if (type == FlowControlCTS) {
        // Enable CTS
        MBED_ASSERT(uart_cts != (UARTName)NC);
        obj_s->hw_flow_ctl = UART_HWCONTROL_CTS;
        obj_s->pin_cts = txflow;
        // Enable the pin for CTS function
        pinmap_pinout(txflow, PinMap_UART_CTS);
    }
    if (type == FlowControlRTSCTS) {
        // Enable CTS & RTS
        MBED_ASSERT(uart_rts != (UARTName)NC);
        MBED_ASSERT(uart_cts != (UARTName)NC);
        obj_s->hw_flow_ctl = UART_HWCONTROL_RTS_CTS;
        obj_s->pin_rts = rxflow;
        obj_s->pin_cts = txflow;
        // Enable the pin for CTS function
        pinmap_pinout(txflow, PinMap_UART_CTS);
        // Enable the pin for RTS function
        pinmap_pinout(rxflow, PinMap_UART_RTS);
    }
    
    init_uart(obj);
}

#endif

#endif
