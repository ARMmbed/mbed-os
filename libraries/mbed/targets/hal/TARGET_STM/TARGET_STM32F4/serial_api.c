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

#if DEVICE_SERIAL

#include "cmsis.h"
#include "pinmap.h"
#include <string.h>
#include "PeripheralPins.h"
#include "mbed_error.h"

#define UART_NUM (8)

static uint32_t serial_irq_ids[UART_NUM] = {0, 0, 0, 0, 0, 0, 0, 0};

static uart_irq_handler irq_handler;

UART_HandleTypeDef UartHandle;

int stdio_uart_inited = 0;
serial_t stdio_uart;

#if DEVICE_SERIAL_ASYNCH
#define _SERIAL_OBJ(X) (obj->serial.X)
#else
#define _SERIAL_OBJ(X) (obj->X)
#endif

static void init_uart(serial_t *obj)
{
    UartHandle.Instance = (USART_TypeDef *)(_SERIAL_OBJ(uart));

    UartHandle.Init.BaudRate     = _SERIAL_OBJ(baudrate);
    UartHandle.Init.WordLength   = _SERIAL_OBJ(databits);
    UartHandle.Init.StopBits     = _SERIAL_OBJ(stopbits);
    UartHandle.Init.Parity       = _SERIAL_OBJ(parity);
    UartHandle.Init.HwFlowCtl    = UART_HWCONTROL_NONE;
    UartHandle.Init.OverSampling = UART_OVERSAMPLING_16;

    if (_SERIAL_OBJ(pin_rx) == NC) {
        UartHandle.Init.Mode = UART_MODE_TX;
    } else if (_SERIAL_OBJ(pin_tx) == NC) {
        UartHandle.Init.Mode = UART_MODE_RX;
    } else {
        UartHandle.Init.Mode = UART_MODE_TX_RX;
    }

    if (HAL_UART_Init(&UartHandle) != HAL_OK) {
        error("Cannot initialize UART\n");
    }
}

void serial_init(serial_t *obj, PinName tx, PinName rx)
{
    // Determine the UART to use (UART_1, UART_2, ...)
    UARTName uart_tx = (UARTName)pinmap_peripheral(tx, PinMap_UART_TX);
    UARTName uart_rx = (UARTName)pinmap_peripheral(rx, PinMap_UART_RX);

    // Get the peripheral name (UART_1, UART_2, ...) from the pin and assign it to the object
    _SERIAL_OBJ(uart) = (UARTName)pinmap_merge(uart_tx, uart_rx);
    
    MBED_ASSERT(_SERIAL_OBJ(uart) != (UARTName)NC);

    // Enable USART clock
    switch (_SERIAL_OBJ(uart)) {
        case UART_1:
            __HAL_RCC_USART1_CLK_ENABLE();
            _SERIAL_OBJ(index) = 0;
            break;
        case UART_2:
            __HAL_RCC_USART2_CLK_ENABLE();
            _SERIAL_OBJ(index) = 1;
            break;
#if defined(USART3_BASE)
        case UART_3:
            __HAL_RCC_USART3_CLK_ENABLE();
            _SERIAL_OBJ(index) = 2;
            break;
#endif
#if defined(UART4_BASE)
        case UART_4:
            __HAL_RCC_UART4_CLK_ENABLE();
            _SERIAL_OBJ(index) = 3;
            break;
#endif
#if defined(UART5_BASE)
        case UART_5:
            __HAL_RCC_UART5_CLK_ENABLE();
            _SERIAL_OBJ(index) = 4;
            break;
#endif
#if defined(USART6_BASE)
        case UART_6:
            __HAL_RCC_USART6_CLK_ENABLE();
            _SERIAL_OBJ(index) = 5;
            break;
#endif
#if defined(UART7_BASE)
        case UART_7:
            __HAL_RCC_UART7_CLK_ENABLE();
            _SERIAL_OBJ(index) = 6;
            break;
#endif
#if defined(UART8_BASE)
        case UART_8:
            __HAL_RCC_UART8_CLK_ENABLE();
            _SERIAL_OBJ(index) = 7;
            break;
#endif
    }

    // Configure the UART pins
    pinmap_pinout(tx, PinMap_UART_TX);
    pinmap_pinout(rx, PinMap_UART_RX);
    
    if (tx != NC) {
        pin_mode(tx, PullUp);
    }
    if (rx != NC) {
        pin_mode(rx, PullUp);
    }

    // Configure UART
    _SERIAL_OBJ(baudrate) = 9600;
    _SERIAL_OBJ(databits) = UART_WORDLENGTH_8B;
    _SERIAL_OBJ(stopbits) = UART_STOPBITS_1;
    _SERIAL_OBJ(parity)   = UART_PARITY_NONE;

    _SERIAL_OBJ(pin_tx) = tx;
    _SERIAL_OBJ(pin_rx) = rx;

    init_uart(obj);

    // For stdio management
    if (_SERIAL_OBJ(uart) == STDIO_UART) {
        stdio_uart_inited = 1;
        memcpy(&stdio_uart, obj, sizeof(serial_t));
    }
}

void serial_free(serial_t *obj)
{
    // Reset UART and disable clock
    switch (_SERIAL_OBJ(uart)) {
        case UART_1:
            __USART1_FORCE_RESET();
            __USART1_RELEASE_RESET();
            __USART1_CLK_DISABLE();
            break;
        case UART_2:
            __USART2_FORCE_RESET();
            __USART2_RELEASE_RESET();
            __USART2_CLK_DISABLE();
            break;
#if defined(USART3_BASE)
        case UART_3:
            __USART3_FORCE_RESET();
            __USART3_RELEASE_RESET();
            __USART3_CLK_DISABLE();
            break;
#endif
#if defined(UART4_BASE)
        case UART_4:
            __UART4_FORCE_RESET();
            __UART4_RELEASE_RESET();
            __UART4_CLK_DISABLE();
            break;
#endif
#if defined(UART5_BASE)
        case UART_5:
            __UART5_FORCE_RESET();
            __UART5_RELEASE_RESET();
            __UART5_CLK_DISABLE();
            break;
#endif
#if defined(USART6_BASE)
        case UART_6:
            __USART6_FORCE_RESET();
            __USART6_RELEASE_RESET();
            __USART6_CLK_DISABLE();
            break;
#endif
#if defined(UART7_BASE)
        case UART_7:
            __UART7_FORCE_RESET();
            __UART7_RELEASE_RESET();
            __UART7_CLK_DISABLE();
            break;
#endif
#if defined(UART8_BASE)
        case UART_8:
            __UART8_FORCE_RESET();
            __UART8_RELEASE_RESET();
            __UART8_CLK_DISABLE();
            break;
#endif
    }
    // Configure GPIOs
    pin_function(_SERIAL_OBJ(pin_tx), STM_PIN_DATA(STM_MODE_INPUT, GPIO_NOPULL, 0));
    pin_function(_SERIAL_OBJ(pin_rx), STM_PIN_DATA(STM_MODE_INPUT, GPIO_NOPULL, 0));

    serial_irq_ids[_SERIAL_OBJ(index)] = 0;
}

void serial_baud(serial_t *obj, int baudrate)
{
    _SERIAL_OBJ(baudrate) = baudrate;
    init_uart(obj);
}

void serial_format(serial_t *obj, int data_bits, SerialParity parity, int stop_bits)
{
    if (data_bits == 9) {
        _SERIAL_OBJ(databits) = UART_WORDLENGTH_9B;
    } else {
        _SERIAL_OBJ(databits) = UART_WORDLENGTH_8B;
    }

    switch (parity) {
        case ParityOdd:
        case ParityForced0:
            _SERIAL_OBJ(parity) = UART_PARITY_ODD;
            break;
        case ParityEven:
        case ParityForced1:
            _SERIAL_OBJ(parity) = UART_PARITY_EVEN;
            break;
        default: // ParityNone
            _SERIAL_OBJ(parity) = UART_PARITY_NONE;
            break;
    }

    if (stop_bits == 2) {
        _SERIAL_OBJ(stopbits) = UART_STOPBITS_2;
    } else {
        _SERIAL_OBJ(stopbits) = UART_STOPBITS_1;
    }

    init_uart(obj);
}

/******************************************************************************
 * INTERRUPTS HANDLING
 ******************************************************************************/

static void uart_irq(UARTName name, int id)
{
    UartHandle.Instance = (USART_TypeDef *)name;
    if (serial_irq_ids[id] != 0) {
        if (__HAL_UART_GET_FLAG(&UartHandle, UART_FLAG_TC) != RESET) {
            irq_handler(serial_irq_ids[id], TxIrq);
            __HAL_UART_CLEAR_FLAG(&UartHandle, UART_FLAG_TC);
        }
        if (__HAL_UART_GET_FLAG(&UartHandle, UART_FLAG_RXNE) != RESET) {
            irq_handler(serial_irq_ids[id], RxIrq);
            __HAL_UART_CLEAR_FLAG(&UartHandle, UART_FLAG_RXNE);
        }
    }
}

static void uart1_irq(void)
{
    uart_irq(UART_1, 0);
}

static void uart2_irq(void)
{
    uart_irq(UART_2, 1);
}

#if defined(USART3_BASE)
static void uart3_irq(void)
{
    uart_irq(UART_3, 2);
}
#endif

#if defined(UART4_BASE)
static void uart4_irq(void)
{
    uart_irq(UART_4, 3);
}
#endif

#if defined(UART5_BASE)
static void uart5_irq(void)
{
    uart_irq(UART_5, 4);
}
#endif

#if defined(USART6_BASE)
static void uart6_irq(void)
{
    uart_irq(UART_6, 5);
}
#endif

#if defined(UART7_BASE)
static void uart7_irq(void)
{
    uart_irq(UART_7, 6);
}
#endif

#if defined(UART8_BASE)
static void uart8_irq(void)
{
    uart_irq(UART_8, 7);
}
#endif

void serial_irq_handler(serial_t *obj, uart_irq_handler handler, uint32_t id)
{
    irq_handler = handler;
    serial_irq_ids[_SERIAL_OBJ(index)] = id;
}

void serial_irq_set(serial_t *obj, SerialIrq irq, uint32_t enable)
{
    IRQn_Type irq_n = (IRQn_Type)0;
    uint32_t vector = 0;

    UartHandle.Instance = (USART_TypeDef *)_SERIAL_OBJ(uart);

    switch (_SERIAL_OBJ(uart)) {
        case UART_1:
            irq_n = USART1_IRQn;
            vector = (uint32_t)&uart1_irq;
            break;

        case UART_2:
            irq_n = USART2_IRQn;
            vector = (uint32_t)&uart2_irq;
            break;
#if defined(USART3_BASE)
        case UART_3:
            irq_n = USART3_IRQn;
            vector = (uint32_t)&uart3_irq;
            break;
#endif
#if defined(UART4_BASE)
        case UART_4:
            irq_n = UART4_IRQn;
            vector = (uint32_t)&uart4_irq;
            break;
#endif
#if defined(UART5_BASE)
        case UART_5:
            irq_n = UART5_IRQn;
            vector = (uint32_t)&uart5_irq;
            break;
#endif
#if defined(USART6_BASE)
        case UART_6:
            irq_n = USART6_IRQn;
            vector = (uint32_t)&uart6_irq;
            break;
#endif
#if defined(UART7_BASE)
        case UART_7:
            irq_n = UART7_IRQn;
            vector = (uint32_t)&uart7_irq;
            break;
#endif
#if defined(UART8_BASE)
        case UART_8:
            irq_n = UART8_IRQn;
            vector = (uint32_t)&uart8_irq;
            break;
#endif
    }

    if (enable) {

        if (irq == RxIrq) {
            __HAL_UART_ENABLE_IT(&UartHandle, UART_IT_RXNE);
        } else { // TxIrq
            __HAL_UART_ENABLE_IT(&UartHandle, UART_IT_TC);
        }

        NVIC_SetVector(irq_n, vector);
        NVIC_EnableIRQ(irq_n);

    } else { // disable

        int all_disabled = 0;

        if (irq == RxIrq) {
            __HAL_UART_DISABLE_IT(&UartHandle, UART_IT_RXNE);
            // Check if TxIrq is disabled too
            if ((UartHandle.Instance->CR1 & USART_CR1_TXEIE) == 0) all_disabled = 1;
        } else { // TxIrq
            __HAL_UART_DISABLE_IT(&UartHandle, UART_IT_TXE);
            // Check if RxIrq is disabled too
            if ((UartHandle.Instance->CR1 & USART_CR1_RXNEIE) == 0) all_disabled = 1;
        }

        if (all_disabled) NVIC_DisableIRQ(irq_n);

    }
}

/******************************************************************************
 * READ/WRITE
 ******************************************************************************/

int serial_getc(serial_t *obj)
{
    USART_TypeDef *uart = (USART_TypeDef *)(_SERIAL_OBJ(uart));
    while (!serial_readable(obj));
    return (int)(uart->DR & 0x1FF);
}

void serial_putc(serial_t *obj, int c)
{
    USART_TypeDef *uart = (USART_TypeDef *)(_SERIAL_OBJ(uart));
    while (!serial_writable(obj));
    uart->DR = (uint32_t)(c & 0x1FF);
}

int serial_readable(serial_t *obj)
{
    int status;
    UartHandle.Instance = (USART_TypeDef *)(_SERIAL_OBJ(uart));
    // Check if data is received
    status = ((__HAL_UART_GET_FLAG(&UartHandle, UART_FLAG_RXNE) != RESET) ? 1 : 0);
    return status;
}

int serial_writable(serial_t *obj)
{
    int status;
    UartHandle.Instance = (USART_TypeDef *)(_SERIAL_OBJ(uart));
    // Check if data is transmitted
    status = ((__HAL_UART_GET_FLAG(&UartHandle, UART_FLAG_TXE) != RESET) ? 1 : 0);
    return status;
}

void serial_clear(serial_t *obj)
{
    UartHandle.Instance = (USART_TypeDef *)(_SERIAL_OBJ(uart));
    __HAL_UART_CLEAR_FLAG(&UartHandle, UART_FLAG_TXE);
    __HAL_UART_CLEAR_FLAG(&UartHandle, UART_FLAG_RXNE);
}

void serial_pinout_tx(PinName tx)
{
    pinmap_pinout(tx, PinMap_UART_TX);
}

void serial_break_set(serial_t *obj)
{
    UartHandle.Instance = (USART_TypeDef *)(_SERIAL_OBJ(uart));
    HAL_LIN_SendBreak(&UartHandle);
}

void serial_break_clear(serial_t *obj)
{
}

//########################################################################################

#if DEVICE_SERIAL_ASYNCH

//----------------------------------------------------------------------------------------
// LOCAL HELPER FUNCTIONS
//----------------------------------------------------------------------------------------

/** Configure the TX buffer for an asynchronous write serial transaction
 *
 * @param obj       The serial object.
 * @param tx        The buffer for sending.
 * @param tx_length The number of words to transmit.
 */
void h_serial_tx_buffer_set(serial_t *obj, void *tx, int tx_length, uint8_t width)
{
    // We only support byte buffers for now
    MBED_ASSERT(width == 8);
    UartHandle.Instance = (USART_TypeDef *)_SERIAL_OBJ(uart);
  
    // Exit if a transmit is already on-going
    if (serial_tx_active(obj)) return;

    obj->tx_buff.buffer = tx;
    obj->tx_buff.length = tx_length;
    obj->tx_buff.pos = 0;

    return;
}

/** Configure the RX buffer for an asynchronous write serial transaction
 *
 * @param obj       The serial object.
 * @param tx        The buffer for sending.
 * @param tx_length The number of words to transmit.
 */
void h_serial_rx_buffer_set(serial_t *obj, void *rx, int rx_length, uint8_t width)
{
    /* Sanity check arguments */
    MBED_ASSERT(obj);
    MBED_ASSERT(rx != (void*)0);
    // We only support byte buffers for now
    MBED_ASSERT(width == 8);

    // Exit if a reception is already on-going
    if (serial_rx_active(obj)) return;

    obj->rx_buff.buffer = rx;
    obj->rx_buff.length = rx_length;
    obj->rx_buff.pos = 0;

    return;
}

/** Configure TX events
 *
 * @param obj    The serial object
 * @param event  The logical OR of the TX events to configure
 * @param enable Set to non-zero to enable events, or zero to disable them
 */
void h_serial_tx_enable_event(serial_t *obj, int event, uint8_t enable)
{
    // Shouldn't have to enable TX interrupt here, just need to keep track of the requested events.
    if (enable) _SERIAL_OBJ(events) |= event;
    else _SERIAL_OBJ(events) &= ~event;
}

/** Configure RX events
 *
 * @param obj    The serial object
 * @param event  The logical OR of the RX events to configure
 * @param enable Set to non-zero to enable events, or zero to disable them
 */
void h_serial_rx_enable_event(serial_t *obj, int event, uint8_t enable)
{
    // Shouldn't have to enable RX interrupt here, just need to keep track of the requested events.
    if (enable) _SERIAL_OBJ(events) |= event;
    else _SERIAL_OBJ(events) &= ~event;
}

/**
* Get index of serial object TX IRQ, relating it to the physical peripheral.
*
* @param obj pointer to serial object
* @return internal NVIC TX IRQ index of U(S)ART peripheral
*/
IRQn_Type h_serial_get_irq_index(serial_t *obj)
{
    IRQn_Type irq_n = (IRQn_Type)0;

    UartHandle.Instance = (USART_TypeDef *)_SERIAL_OBJ(uart);

    switch (_SERIAL_OBJ(uart)) {
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
            irq_n = USART3_IRQn;
            break;
#endif
#if defined(UART4_BASE)
        case UART_4:
            irq_n = UART4_IRQn;
            break;
#endif
#if defined(UART5_BASE)
        case UART_5:
            irq_n = UART5_IRQn;
            break;
#endif
#if defined(USART6_BASE)
        case UART_6:
            irq_n = USART6_IRQn;
            break;
#endif
#if defined(UART7_BASE)
        case UART_7:
            irq_n = UART7_IRQn;
            break;
#endif
#if defined(UART8_BASE)
        case UART_8:
            irq_n = UART8_IRQn;
            break;
#endif
        default:
            irq_n = (IRQn_Type)0;
    }
    
    return irq_n;
}

//----------------------------------------------------------------------------------------
// MBED API FUNCTIONS
//----------------------------------------------------------------------------------------

/** Begin asynchronous TX transfer. The used buffer is specified in the serial object,
 *  tx_buff
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
  uint32_t tmpstatus = 0;
  
  // Check buffer is ok
  MBED_ASSERT(tx != (void*)0);
  if (tx_length == 0) return 0;
  
  // Set up buffer
  h_serial_tx_buffer_set(obj, (void *)tx, tx_length, tx_width);
  
  // Set up events
  h_serial_tx_enable_event(obj, SERIAL_EVENT_TX_ALL, 0); // Clear all events
  h_serial_tx_enable_event(obj, event, 1); // Set only the wanted events
  
  // TODO: SETUP SLEEP MODE (wake up on uart interrupt)
  
  // Enable interrupt
  IRQn_Type irqn = h_serial_get_irq_index(obj);
  NVIC_ClearPendingIRQ(irqn);
  NVIC_DisableIRQ(irqn);
  NVIC_SetPriority(irqn, 1);
  NVIC_SetVector(irqn, (uint32_t)handler);
  UartHandle.Instance = (USART_TypeDef *)_SERIAL_OBJ(uart);
  NVIC_EnableIRQ(irqn);
    
  // the following function will enable UART_IT_TXE and error interrupts
  if (HAL_UART_Transmit_IT(&UartHandle, (uint8_t*)tx, tx_length) != HAL_OK)
  {
    /* Transfer error in transmission process */
    return 0;
  }
  
  while ((tmpstatus == HAL_UART_STATE_BUSY_TX) || (tmpstatus == HAL_UART_STATE_BUSY_TX_RX)){
    tmpstatus = HAL_UART_GetState(&UartHandle);
  }
  
  return tx_length;
}

/** Begin asynchronous RX transfer (enable interrupt for data collecting)
 *  The used buffer is specified in the serial object - rx_buff
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
      /* Sanity check arguments */
    MBED_ASSERT(obj);
    MBED_ASSERT(rx != (void*)0);

    h_serial_rx_enable_event(obj, SERIAL_EVENT_RX_ALL, 0);
    h_serial_rx_enable_event(obj, event, 1);
    // set CharMatch
    if (char_match != SERIAL_RESERVED_CHAR_MATCH) {
        obj->char_match = char_match;
    }
    h_serial_rx_buffer_set(obj, rx, rx_length, rx_width);

    IRQn_Type irqn = h_serial_get_irq_index(obj);
    NVIC_ClearPendingIRQ(irqn);
    NVIC_DisableIRQ(irqn);
    NVIC_SetVector(irqn, (uint32_t)handler);
    NVIC_EnableIRQ(irqn);

    UartHandle.Instance = (USART_TypeDef *)_SERIAL_OBJ(uart);

    // following HAL function will enable the RXNE interrupt + error interrupts    
    if (HAL_UART_Receive_IT(&UartHandle, (uint8_t*)rx, rx_length) != HAL_OK)
    {
      /* Transfer error in transmission process */
      return;
    }

    // TO DO setup sleep mode
    // TO DO setup DMA part
    return;
}

/** Attempts to determine if the serial peripheral is already in use for TX
 *
 * @param obj The serial object
 * @return Non-zero if the TX transaction is ongoing, 0 otherwise
 */
uint8_t serial_tx_active(serial_t *obj)
{
    MBED_ASSERT(obj);
    UartHandle.Instance = (USART_TypeDef *)(_SERIAL_OBJ(uart));
    return (UartHandle.Instance->CR1 & USART_CR1_TXEIE ? 1 : 0);
}

/** Attempts to determine if the serial peripheral is already in use for RX
 *
 * @param obj The serial object
 * @return Non-zero if the RX transaction is ongoing, 0 otherwise
 */
uint8_t serial_rx_active(serial_t *obj)
{
    MBED_ASSERT(obj);
    UartHandle.Instance = (USART_TypeDef *)(_SERIAL_OBJ(uart));
    return (UartHandle.Instance->CR1 & USART_CR1_RXNEIE ? 1 : 0);
}

/** The asynchronous TX and RX handler.
 *
 * @param obj The serial object
 * @return Returns event flags if a TX/RX transfer termination condition was met or 0 otherwise
 */
int serial_irq_handler_asynch(serial_t *obj)
{
  int return_event = 0;
  uint8_t *buf = (uint8_t*)obj->rx_buff.buffer;
  // Irq handler is common to Tx and Rx
    UartHandle.Instance = (USART_TypeDef *)(_SERIAL_OBJ(uart));
    HAL_UART_IRQHandler(&UartHandle);
  // TX PART:
    if (__HAL_UART_GET_FLAG(&UartHandle, UART_FLAG_TC) != RESET) {
        __HAL_UART_CLEAR_FLAG(&UartHandle, UART_FLAG_TC);
        // return event SERIAL_EVENT_TX_COMPLETE if requested
        if ((_SERIAL_OBJ(events) & SERIAL_EVENT_TX_COMPLETE ) != 0){
            return_event |= SERIAL_EVENT_TX_COMPLETE & obj->serial.events;
        }
    }
    //RX PART
    // handle error events:
    if (__HAL_UART_GET_FLAG(&UartHandle, HAL_UART_ERROR_PE)) {
        return_event |= SERIAL_EVENT_RX_PARITY_ERROR & obj->serial.events;
    }
    if (__HAL_UART_GET_FLAG(&UartHandle, HAL_UART_ERROR_NE)) {
      // not supported by mbed
    }
    if (__HAL_UART_GET_FLAG(&UartHandle, HAL_UART_ERROR_FE)) {
        return_event |= SERIAL_EVENT_RX_FRAMING_ERROR & obj->serial.events;
    }
    if (__HAL_UART_GET_FLAG(&UartHandle, HAL_UART_ERROR_ORE)) {
        return_event |= SERIAL_EVENT_RX_OVERRUN_ERROR & obj->serial.events;
    }
    
    if (UartHandle.RxXferCount==0) {
        return_event |= SERIAL_EVENT_RX_COMPLETE & obj->serial.events;
    }

    if ((buf != NULL) && (buf[obj->rx_buff.pos-1] == obj->char_match) && (_SERIAL_OBJ(events) & SERIAL_EVENT_RX_CHARACTER_MATCH)) {
      return_event |= SERIAL_EVENT_RX_CHARACTER_MATCH & obj->serial.events;
    }
    return return_event;  
}

/** Abort the ongoing TX transaction. It disables the enabled interupt for TX and
 *  flush TX hardware buffer if TX FIFO is used
 *
 * @param obj The serial object
 */
void serial_tx_abort_asynch(serial_t *obj)
{
  // TODO
  UartHandle.Instance = (USART_TypeDef *)(_SERIAL_OBJ(uart));
  __HAL_UART_CLEAR_FLAG(&UartHandle, UART_FLAG_TC);
  
  obj->tx_buff.buffer = 0;
  obj->tx_buff.length = 0;
  // TODO : handle sleep mode
  
}

/** Abort the ongoing RX transaction It disables the enabled interrupt for RX and
 *  flush RX hardware buffer if RX FIFO is used
 *
 * @param obj The serial object
 */
void serial_rx_abort_asynch(serial_t *obj)
{
  // TODO
}

#endif

#endif
