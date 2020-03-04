/* mbed Microcontroller Library
 * SPDX-License-Identifier: BSD-3-Clause
 ******************************************************************************
 *
 * Copyright (c) 2017 STMicroelectronics.
 * All rights reserved.
 *
 * This software component is licensed by ST under BSD 3-Clause license,
 * the "License"; You may not use this file except in compliance with the
 * License. You may obtain a copy of the License at:
 *                        opensource.org/licenses/BSD-3-Clause
 *
 ******************************************************************************
 */

#if DEVICE_SERIAL

#include "serial_api_hal.h"

#define UART_NUM (5)


uint32_t serial_irq_ids[UART_NUM] = {0};
UART_HandleTypeDef uart_handlers[UART_NUM];

static uart_irq_handler irq_handler;

// Defined in serial_api.c
extern int8_t get_uart_index(UARTName uart_name);

/******************************************************************************
 * INTERRUPTS HANDLING
 ******************************************************************************/

static void uart_irq(UARTName uart_name)
{
    int8_t id = get_uart_index(uart_name);

    if (id >= 0) {
        UART_HandleTypeDef *huart = &uart_handlers[id];
        if (serial_irq_ids[id] != 0) {
            if (__HAL_UART_GET_FLAG(huart, UART_FLAG_TXE) != RESET) {
                if (__HAL_UART_GET_IT(huart, UART_IT_TXE) != RESET && __HAL_UART_GET_IT_SOURCE(huart, UART_IT_TXE)) {
                    irq_handler(serial_irq_ids[id], TxIrq);
                }
            }
            if (__HAL_UART_GET_FLAG(huart, UART_FLAG_RXNE) != RESET) {
                if (__HAL_UART_GET_IT(huart, UART_IT_RXNE) != RESET && __HAL_UART_GET_IT_SOURCE(huart, UART_IT_RXNE)) {
                    irq_handler(serial_irq_ids[id], RxIrq);
                    /* Flag has been cleared when reading the content */
                }
            }
            if (__HAL_UART_GET_FLAG(huart, UART_FLAG_ORE) != RESET) {
                if (__HAL_UART_GET_IT(huart, UART_IT_ORE) != RESET) {
                    __HAL_UART_CLEAR_FLAG(huart, UART_CLEAR_OREF);
                }
            }
        }
    }
}

#if defined(USART1_BASE)
static void uart1_irq(void)
{
    uart_irq(UART_1);
}
#endif

#if defined(USART2_BASE)
static void uart2_irq(void)
{
    uart_irq(UART_2);
}
#endif

#if defined(USART3_BASE)
static void uart3_irq(void)
{
    uart_irq(UART_3);
}
#endif

#if defined(USART4_BASE)
static void uart4_irq(void)
{
    uart_irq(UART_4);
}
#endif

#if defined(LPUART1_BASE)
static void lpuart1_irq(void)
{
    uart_irq(LPUART_1);
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

#if defined(USART1_BASE)
    if (obj_s->uart == UART_1) {
        irq_n = USART1_IRQn;
        vector = (uint32_t)&uart1_irq;
    }
#endif

#if defined(USART2_BASE)
    if (obj_s->uart == UART_2) {
        irq_n = USART2_IRQn;
        vector = (uint32_t)&uart2_irq;
    }
#endif

#if defined(USART3_BASE)
    if (obj_s->uart == UART_3) {
        irq_n = USART3_4_LPUART1_IRQn;
        vector = (uint32_t)&uart3_irq;
    }
#endif

#if defined(USART4_BASE)
    if (obj_s->uart == UART_4) {
        irq_n = USART3_4_LPUART1_IRQn;
        vector = (uint32_t)&uart4_irq;
    }
#endif

#if defined(LPUART1_BASE)
    if (obj_s->uart == LPUART_1) {
        irq_n = USART3_4_LPUART1_IRQn;
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
#if defined(STM32G0)
#define USART_CR1_TXEIE   USART_CR1_TXEIE_TXFNFIE
#endif
            if ((huart->Instance->CR1 & USART_CR1_TXEIE) == 0) {
                all_disabled = 1;
            }
        } else { // TxIrq
            __HAL_UART_DISABLE_IT(huart, UART_IT_TXE);
            // Check if RxIrq is disabled too
#if defined(STM32G0)
#define USART_CR1_RXNEIE   USART_CR1_RXNEIE_RXFNEIE
#endif
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

    /* Computation of UART mask to apply to RDR register */
    UART_MASK_COMPUTATION(huart);
    uint16_t uhMask = huart->Mask;

    while (!serial_readable(obj));
    /* When receiving with the parity enabled, the value read in the MSB bit
     * is the received parity bit.
     */
    return (int)(huart->Instance->RDR & uhMask);
}

void serial_putc(serial_t *obj, int c)
{
    struct serial_s *obj_s = SERIAL_S(obj);
    UART_HandleTypeDef *huart = &uart_handlers[obj_s->index];

    while (!serial_writable(obj));
    /* When transmitting with the parity enabled (PCE bit set to 1 in the
     * USART_CR1 register), the value written in the MSB (bit 7 or bit 8
     * depending on the data length) has no effect because it is replaced
     * by the parity.
     */
    huart->Instance->TDR = (uint16_t)(c & 0x1FFU);
}

void serial_clear(serial_t *obj)
{
    struct serial_s *obj_s = SERIAL_S(obj);
    UART_HandleTypeDef *huart = &uart_handlers[obj_s->index];

    /* Clear RXNE and error flags */
    volatile uint32_t tmpval __attribute__((unused)) = huart->Instance->RDR;
    HAL_UART_ErrorCallback(huart);
}

void serial_break_set(serial_t *obj)
{
    struct serial_s *obj_s = SERIAL_S(obj);
    UART_HandleTypeDef *huart = &uart_handlers[obj_s->index];

    __HAL_UART_SEND_REQ(huart, UART_SENDBREAK_REQUEST);
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
* @param uart_name i.e. UART_1, UART_2, ...
* @return internal NVIC TX IRQ index of U(S)ART peripheral
*/
static IRQn_Type serial_get_irq_n(UARTName uart_name)
{
    IRQn_Type irq_n;

    switch (uart_name) {
#if defined(USART1_BASE)
        case UART_1:
            irq_n = USART1_IRQn;
            break;
#endif
#if defined(USART2_BASE)
        case UART_2:
            irq_n = USART2_IRQn;
            break;
#endif
#if defined(USART3_BASE)
        case UART_3:
            irq_n = USART3_4_LPUART1_IRQn;
            break;
#endif
#if defined(USART4_BASE)
        case UART_4:
            irq_n = USART3_4_LPUART1_IRQn;
            break;
#endif

#if defined(LPUART1_BASE)
        case LPUART_1:
            irq_n = USART3_4_LPUART1_IRQn;
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
    MBED_ASSERT(tx != (void *)0);
    MBED_ASSERT(tx_width == 8); // support only 8b width

    struct serial_s *obj_s = SERIAL_S(obj);
    UART_HandleTypeDef *huart = &uart_handlers[obj_s->index];

    if (tx_length == 0) {
        return 0;
    }

    // Set up buffer
    serial_tx_buffer_set(obj, (void *)tx, tx_length, tx_width);

    // Set up events
    serial_enable_event(obj, SERIAL_EVENT_TX_ALL, 0); // Clear all events
    serial_enable_event(obj, event, 1); // Set only the wanted events

    // Enable interrupt
    IRQn_Type irq_n = serial_get_irq_n(obj_s->uart);
    NVIC_ClearPendingIRQ(irq_n);
    NVIC_DisableIRQ(irq_n);
    NVIC_SetPriority(irq_n, 1);
    NVIC_SetVector(irq_n, (uint32_t)handler);
    NVIC_EnableIRQ(irq_n);

    // the following function will enable UART_IT_TXE and error interrupts
    if (HAL_UART_Transmit_IT(huart, (uint8_t *)tx, tx_length) != HAL_OK) {
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
    MBED_ASSERT(rx != (void *)0);
    MBED_ASSERT(rx_width == 8); // support only 8b width

    struct serial_s *obj_s = SERIAL_S(obj);
    UART_HandleTypeDef *huart = &uart_handlers[obj_s->index];

    serial_enable_event(obj, SERIAL_EVENT_RX_ALL, 0);
    serial_enable_event(obj, event, 1);

    // set CharMatch
    obj->char_match = char_match;

    serial_rx_buffer_set(obj, rx, rx_length, rx_width);

    IRQn_Type irq_n = serial_get_irq_n(obj_s->uart);
    NVIC_ClearPendingIRQ(irq_n);
    NVIC_DisableIRQ(irq_n);
    NVIC_SetPriority(irq_n, 0);
    NVIC_SetVector(irq_n, (uint32_t)handler);
    NVIC_EnableIRQ(irq_n);

    // following HAL function will enable the RXNE interrupt + error interrupts
    HAL_UART_Receive_IT(huart, (uint8_t *)rx, rx_length);
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

    return (((HAL_UART_GetState(huart) & HAL_UART_STATE_BUSY_TX) == HAL_UART_STATE_BUSY_TX) ? 1 : 0);
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

    return (((HAL_UART_GetState(huart) & HAL_UART_STATE_BUSY_RX) == HAL_UART_STATE_BUSY_RX) ? 1 : 0);
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
    uint8_t *buf = (uint8_t *)(obj->rx_buff.buffer);
    size_t i = 0;

    // TX PART:
    if (__HAL_UART_GET_FLAG(huart, UART_FLAG_TC) != RESET) {
        if (__HAL_UART_GET_IT_SOURCE(huart, UART_IT_TC) != RESET) {
            // Return event SERIAL_EVENT_TX_COMPLETE if requested
            if ((obj_s->events & SERIAL_EVENT_TX_COMPLETE) != 0) {
                return_event |= (SERIAL_EVENT_TX_COMPLETE & obj_s->events);
            }
        }
    }

    // Handle error events
    if (__HAL_UART_GET_FLAG(huart, UART_FLAG_PE) != RESET) {
        __HAL_UART_CLEAR_FLAG(huart, UART_CLEAR_PEF);
        if (__HAL_UART_GET_IT(huart, USART_IT_ERR) != RESET) {
            return_event |= (SERIAL_EVENT_RX_PARITY_ERROR & obj_s->events);
        }
    }

    if (__HAL_UART_GET_FLAG(huart, UART_FLAG_FE) != RESET) {
        if (__HAL_UART_GET_IT(huart, UART_IT_FE) != RESET) {
            __HAL_UART_CLEAR_FLAG(huart, UART_CLEAR_FEF);
            return_event |= (SERIAL_EVENT_RX_FRAMING_ERROR & obj_s->events);
        }
    }

    if (__HAL_UART_GET_FLAG(huart, UART_FLAG_NE) != RESET) {
        if (__HAL_UART_GET_IT(huart, UART_IT_NE) != RESET) {
            __HAL_UART_CLEAR_FLAG(huart, UART_CLEAR_NEF);
        }
    }

    if (__HAL_UART_GET_FLAG(huart, UART_FLAG_ORE) != RESET) {
        if (__HAL_UART_GET_IT(huart, UART_IT_ORE) != RESET) {
            __HAL_UART_CLEAR_FLAG(huart, UART_CLEAR_OREF);
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

    // reset states
    huart->TxXferCount = 0;
    // update handle state
    if (huart->gState == HAL_UART_STATE_BUSY_TX_RX) {
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
    __HAL_UART_CLEAR_FLAG(huart, UART_CLEAR_PEF | UART_CLEAR_FEF | UART_CLEAR_OREF);
    volatile uint32_t tmpval __attribute__((unused)) = huart->Instance->RDR; // Clear RXNE flag

    // reset states
    huart->RxXferCount = 0;
    // update handle state
    if (huart->RxState == HAL_UART_STATE_BUSY_TX_RX) {
        huart->RxState = HAL_UART_STATE_BUSY_TX;
    } else {
        huart->RxState = HAL_UART_STATE_READY;
    }
}

#endif /* DEVICE_SERIAL_ASYNCH */

#if DEVICE_SERIAL_FC

/**
 * Set HW Control Flow
 * @param obj    The serial object
 * @param type   The Control Flow type (FlowControlNone, FlowControlRTS, FlowControlCTS, FlowControlRTSCTS)
 * @param pinmap Pointer to structure which holds static pinmap
 */
#if STATIC_PINMAP_READY
#define SERIAL_SET_FC_DIRECT serial_set_flow_control_direct
void serial_set_flow_control_direct(serial_t *obj, FlowControl type, const serial_fc_pinmap_t *pinmap)
#else
#define SERIAL_SET_FC_DIRECT _serial_set_flow_control_direct
static void _serial_set_flow_control_direct(serial_t *obj, FlowControl type, const serial_fc_pinmap_t *pinmap)
#endif
{
    struct serial_s *obj_s = SERIAL_S(obj);

    if (type == FlowControlNone) {
        // Disable hardware flow control
        obj_s->hw_flow_ctl = UART_HWCONTROL_NONE;
    }
    if (type == FlowControlRTS) {
        // Enable RTS
        MBED_ASSERT(pinmap->rx_flow_pin != (UARTName)NC);
        obj_s->hw_flow_ctl = UART_HWCONTROL_RTS;
        obj_s->pin_rts = pinmap->rx_flow_pin;
        // Enable the pin for RTS function
        pin_function(pinmap->rx_flow_pin, pinmap->rx_flow_function);
        pin_mode(pinmap->rx_flow_pin, PullNone);
    }
    if (type == FlowControlCTS) {
        // Enable CTS
        MBED_ASSERT(pinmap->tx_flow_pin != (UARTName)NC);
        obj_s->hw_flow_ctl = UART_HWCONTROL_CTS;
        obj_s->pin_cts = pinmap->tx_flow_pin;
        // Enable the pin for CTS function
        pin_function(pinmap->tx_flow_pin, pinmap->tx_flow_function);
        pin_mode(pinmap->tx_flow_pin, PullNone);
    }
    if (type == FlowControlRTSCTS) {
        // Enable CTS & RTS
        MBED_ASSERT(pinmap->rx_flow_pin != (UARTName)NC);
        MBED_ASSERT(pinmap->tx_flow_pin != (UARTName)NC);
        obj_s->hw_flow_ctl = UART_HWCONTROL_RTS_CTS;
        obj_s->pin_rts = pinmap->rx_flow_pin;;
        obj_s->pin_cts = pinmap->tx_flow_pin;;
        // Enable the pin for CTS function
        pin_function(pinmap->tx_flow_pin, pinmap->tx_flow_function);
        pin_mode(pinmap->tx_flow_pin, PullNone);
        // Enable the pin for RTS function
        pin_function(pinmap->rx_flow_pin, pinmap->rx_flow_function);
        pin_mode(pinmap->rx_flow_pin, PullNone);
    }

    init_uart(obj);
}


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

    UARTName uart_rts = (UARTName)pinmap_peripheral(rxflow, PinMap_UART_RTS);
    UARTName uart_cts = (UARTName)pinmap_peripheral(txflow, PinMap_UART_CTS);

    if (((UARTName)pinmap_merge(uart_rts, obj_s->uart) == (UARTName)NC) || ((UARTName)pinmap_merge(uart_cts, obj_s->uart) == (UARTName)NC)) {
        MBED_ASSERT(0);
        return;
    }

    int peripheral = (int)pinmap_merge(uart_rts, uart_cts);

    int tx_flow_function = (int)pinmap_find_function(txflow, PinMap_UART_CTS);
    int rx_flow_function = (int)pinmap_find_function(rxflow, PinMap_UART_RTS);

    const serial_fc_pinmap_t explicit_uart_fc_pinmap = {peripheral, txflow, tx_flow_function, rxflow, rx_flow_function};

    SERIAL_SET_FC_DIRECT(obj, type, &explicit_uart_fc_pinmap);
}

#endif /* DEVICE_SERIAL_FC */

#endif /* DEVICE_SERIAL */
