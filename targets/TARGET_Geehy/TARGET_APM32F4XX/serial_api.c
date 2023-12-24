/* mbed Microcontroller Library
 * Copyright (c) 2006-2023 ARM Limited
 * Copyright (c) 2023 Geehy Semiconductor Inc.
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

#include "mbed_assert.h"
#include "serial_api.h"

#if DEVICE_SERIAL

#include "cmsis.h"
#include "pinmap.h"
#include "mbed_error.h"
#include <string.h>
#include "PeripheralPins.h"

#define USART_NUM   6

static uint32_t serial_irq_ids[USART_NUM] = {0};
static USART_T* uart_clk[USART_NUM] = {USART1, USART2, USART3, UART4, UART5, USART6};
static IRQn_Type uart_irq_n[USART_NUM] = {USART1_IRQn, USART2_IRQn, USART3_IRQn, UART4_IRQn, UART5_IRQn, USART6_IRQn};

static uart_irq_handler irq_handler;
int stdio_uart_inited = 0;
serial_t stdio_uart;


#if DEVICE_SERIAL_ASYNCH
#define GET_SERIAL_S(obj) (&((obj)->serial))
#else
#define GET_SERIAL_S(obj) (obj)
#endif /* DEVICE_SERIAL_ASYNCH */

 /*!
 * @brief     Get the pin name from the port's pin number
 *
 * @param     obj_s : The serial object
 *
 * @retval    The pin name for the port's pin number.BIT[7:4] port number,BIT[3:0] pin number.
 */
static void usart_init(struct serial_s *obj_s)
{
    USART_T *usart = (USART_T*)obj_s->uart;
    if (obj_s->index > USART_NUM)
    {
        return;
    }

    USART_Reset((USART_T*)obj_s->uart);

    USART_Config_T usartConfig;
    usartConfig.baudRate = obj_s->baudrate;
    usartConfig.mode = USART_MODE_TX_RX;
    usartConfig.parity = obj_s->parity;
    usartConfig.stopBits = obj_s->stopbits;
    usartConfig.wordLength = obj_s->databits;

#if DEVICE_SERIAL_FC
    switch (obj_s->hw_flow_ctl)
    {
        case USART_HARDWARE_FLOW_NONE:
            usartConfig.hardwareFlow = USART_HARDWARE_FLOW_NONE;
            break;
        case USART_HARDWARE_FLOW_RTS:
            usartConfig.hardwareFlow = USART_HARDWARE_FLOW_RTS;
            break;
        case USART_HARDWARE_FLOW_CTS:
            usartConfig.hardwareFlow = USART_HARDWARE_FLOW_CTS;
            break;
        case USART_HARDWARE_FLOW_RTS_CTS:
            usartConfig.hardwareFlow = USART_HARDWARE_FLOW_RTS_CTS;
            break;
    }
#endif /* DEVICE_SERIAL_FC */
    USART_Config(usart,&usartConfig);
    USART_Enable(usart);
    
}

 /*!
 * @brief     Initialize the serial peripheral. It sets the default parameters for serial.
 *
 * @param     obj_s : The serial object
 *
 * @param     tx : The TX pin name
 *
 * @param     tx : The RX pin name
 *
 * @retval    None
 */
void serial_init(serial_t *obj, PinName tx, PinName rx)
{
    struct serial_s *per_obj = GET_SERIAL_S(obj);

    UARTName usart_tx = (UARTName)pinmap_peripheral(tx,PinMap_UART_TX);
    UARTName usart_rx = (UARTName)pinmap_peripheral(rx,PinMap_UART_RX);

    per_obj->uart = (UARTName)pinmap_merge(usart_tx,usart_rx);
    MBED_ASSERT(per_obj->uart != (UARTName)NC);

    /* Enable USART Clock */
    switch (per_obj->uart)
    {
        case UART_1:
            RCM_EnableAPB2PeriphClock(RCM_APB2_PERIPH_USART1);
            per_obj->index = 0;
            break;
        case UART_2:
            RCM_EnableAPB1PeriphClock(RCM_APB1_PERIPH_USART2);
            per_obj->index = 1;
            break;
        case UART_3:
            RCM_EnableAPB1PeriphClock(RCM_APB1_PERIPH_USART3);
            per_obj->index = 2;
            break;
        case UART_4:
            RCM_EnableAPB1PeriphClock(RCM_APB1_PERIPH_UART4);
            per_obj->index = 3;
            break;
        case UART_5:
            RCM_EnableAPB1PeriphClock(RCM_APB1_PERIPH_UART5);
            per_obj->index = 4;
            break;
        case UART_6:
             RCM_EnableAPB2PeriphClock(RCM_APB2_PERIPH_USART6);
            per_obj->index = 5;
            break;
    }

    /* Config the pins */
    pinmap_pinout(tx,PinMap_UART_TX);
    pinmap_pinout(rx,PinMap_UART_RX);

    /* Config default parameters */
    per_obj->baudrate = 9600U;
    per_obj->databits = USART_WORD_LEN_8B;
    per_obj->stopbits = USART_STOP_BIT_1;
    per_obj->parity = USART_PARITY_NONE;

#if DEVICE_SERIAL_FC
    per_obj->hw_flow_ctl = USART_HARDWARE_FLOW_NONE;
#endif /* DEVICE_SERIAL_FC */

    per_obj->pin_tx = tx;
    per_obj->pin_rx = rx;

    per_obj->tx_state = OP_STATE_BUSY;
    per_obj->rx_state = OP_STATE_BUSY;

    usart_init(per_obj);

    per_obj->tx_state = OP_STATE_READY;
    per_obj->rx_state = OP_STATE_READY;
    
    if (per_obj->uart == STDIO_UART)
    {
        stdio_uart_inited = 1;
        memcpy(&stdio_uart,obj,sizeof(serial_t));
    }
}

 /*!
 * @brief     Release the serial peripheral, not currently invoked. It requires further resource management.
 *
 * @param     obj_s : The serial object
 *
 * @retval    None
 */
void serial_free(serial_t *obj)
{
    struct serial_s *per_obj = GET_SERIAL_S(obj);
    USART_T *usart = (USART_T*)per_obj->uart;
    USART_T* uart = uart_clk[per_obj->index];

    USART_Reset(usart);

    /* Disable USART Clock */
    switch ((uint32_t)uart)
    {
        case UART_1:
            RCM_DisableAPB2PeriphClock(RCM_APB2_PERIPH_USART1);
            per_obj->index = 0;
            break;
        case UART_2:
            RCM_DisableAPB1PeriphClock(RCM_APB1_PERIPH_USART2);
            per_obj->index = 1;
            break;
        case UART_3:
            RCM_DisableAPB1PeriphClock(RCM_APB1_PERIPH_USART3);
            per_obj->index = 2;
            break;
        case UART_4:
            RCM_DisableAPB1PeriphClock(RCM_APB1_PERIPH_UART4);
            per_obj->index = 3;
            break;
        case UART_5:
            RCM_DisableAPB1PeriphClock(RCM_APB1_PERIPH_UART5);
            per_obj->index = 4;
            break;
        case UART_6:
            RCM_DisableAPB2PeriphClock(RCM_APB2_PERIPH_USART6);
            per_obj->index = 5;
            break;
    }

    serial_irq_ids[per_obj->index] = 0;

    /* Reset the gpio state */
    pin_function(per_obj->pin_tx,PullNone);
    pin_function(per_obj->pin_rx,PullNone);
}

 /*!
 * @brief     Configure the baud rate
 *
 * @param     obj_s : The serial object
 *
 * @param     baudrate : The baud rate to be configured
 *
 * @retval    None
 */
void serial_baud(serial_t *obj, int baudrate)
{
    struct serial_s *per_obj = GET_SERIAL_S(obj);
    USART_T *usart = (USART_T*)per_obj->uart;
    per_obj->baudrate = baudrate;

    USART_Disable(usart);

    USART_Config_T usartConfig;
    usartConfig.baudRate = baudrate;
    usartConfig.hardwareFlow = per_obj->hw_flow_ctl;
    usartConfig.mode = USART_MODE_TX_RX;
    usartConfig.parity = per_obj->parity;
    usartConfig.stopBits = per_obj->stopbits;
    usartConfig.wordLength = per_obj->databits;
    USART_Config(usart,&usartConfig);

    USART_Enable(usart);
}

 /*!
 * @brief     Configure the format. Set the number of bits, parity and the number of stop bits
 *
 * @param     obj_s : The serial object
 *
 * @param     data_bits : The number of data bits
 *
 * @param     parity : The parity
 *
 * @param     stop_bits : The number of stop bits
 *
 * @retval    None
 */
void serial_format(serial_t *obj, int data_bits, SerialParity parity, int stop_bits)
{
    struct serial_s *per_obj = GET_SERIAL_S(obj);
    USART_T *usart = (USART_T*)per_obj->uart;

    USART_Disable(usart);

    USART_Config_T usartConfig;
    usartConfig.baudRate = per_obj->baudrate;
    usartConfig.hardwareFlow = per_obj->hw_flow_ctl;
    usartConfig.mode = USART_MODE_TX_RX;

    /* Change the parity value */
    switch (parity)
    {
        case ParityOdd:
            per_obj->parity = USART_PARITY_ODD;
            usartConfig.parity = USART_PARITY_ODD;
            break;
        case ParityEven:
            per_obj->parity = USART_PARITY_EVEN;
            usartConfig.parity = USART_PARITY_EVEN;
            break;
        case ParityForced0:
        case ParityForced1:
        default:
            per_obj->parity = USART_PARITY_NONE;
            usartConfig.parity = USART_PARITY_NONE;
            break;
    }

    /* Change the data_bits value */
    switch (data_bits)
    {
        case 7:
            if (parity != USART_PARITY_NONE)
            {
                per_obj->parity = USART_WORD_LEN_8B;
                usartConfig.wordLength = USART_WORD_LEN_8B;
            }
            else
            {            
                error("7-bit data format without parity is not supported");
            }
            break;
        case 8:
            if (parity != USART_PARITY_NONE)
            {
                per_obj->parity = USART_WORD_LEN_9B;
                usartConfig.wordLength = USART_WORD_LEN_9B;
            }
            else
            {
                per_obj->parity = USART_WORD_LEN_8B;
                usartConfig.wordLength = USART_WORD_LEN_8B;
            }
            break;
        case 9:
            if (parity != USART_PARITY_NONE)
            {
                error("9-bit data format without parity is not supported");
            }
            else
            {   
                per_obj->parity = USART_WORD_LEN_9B;
                usartConfig.wordLength = USART_WORD_LEN_9B;
            }
            break;
        default:
            error("Only 7, 8 or 9-bit data formats are supported");
            break;
    }

    /* Change the stop_bits value */
    if (stop_bits == 2)
    {
        usartConfig.stopBits = USART_STOP_BIT_2;
    }
    else
    {
        usartConfig.stopBits = USART_STOP_BIT_1;
    }

    USART_Config(usart,&usartConfig);
    USART_Enable(usart);
}

 /*!
 * @brief     The serial interrupt handler registration
 *
 * @param     obj : The serial object
 *
 * @param     handler : The interrupt handler which will be invoked when the interrupt fires
 *
 * @param     id : The SerialBase object
 *
 * @retval    None
 */
void serial_irq_handler(serial_t *obj, uart_irq_handler handler, uint32_t id)
{
    struct serial_s *per_obj = GET_SERIAL_S(obj);

    irq_handler = handler;
    serial_irq_ids[per_obj->index] = id;
}

 /*!
 * @brief     This function handles USART interrupt handler
 *
 * @param     usart_index : The index of UART
 *
 * @param     usart_periph : The index of UART
 *
 * @retval    None
 */
static void usart_irq(int usart_index, USART_T* usart_periph)
{
    if (serial_irq_ids[usart_index] != 0)
    {
        if (USART_ReadIntFlag(usart_periph,USART_INT_TXBE) != RESET)
        {
            irq_handler(serial_irq_ids[usart_index], TxIrq);
        }

        if (USART_ReadIntFlag(usart_periph,USART_INT_RXBNE) != RESET)
        {
            irq_handler(serial_irq_ids[usart_index], RxIrq);
        }

        if (USART_ReadIntFlag(usart_periph,USART_INT_OVRE_ER) != RESET)
        {
            (void)(usart_periph->DATA);
        }

        if (USART_ReadIntFlag(usart_periph,USART_INT_NE) != RESET)
        {
            (void)(usart_periph->DATA);
        }

        if (USART_ReadIntFlag(usart_periph,USART_INT_FE) != RESET)
        {
            (void)(usart_periph->DATA);
        }

        if (USART_ReadIntFlag(usart_periph,USART_INT_PE) != RESET)
        {
            (void)(usart_periph->DATA);
        }
    }
} 

 /*!
 * @brief     This function handles USART1 interrupt handler
 *
 * @param     None
 *
 * @retval    None
 */
static void usart1_irq(void)
{
    usart_irq(0, USART1);
}

 /*!
 * @brief     This function handles USART2 interrupt handler
 *
 * @param     None
 *
 * @retval    None
 */
static void usart2_irq(void)
{
    usart_irq(1, USART2);
}

 /*!
 * @brief     This function handles USART3 interrupt handler
 *
 * @param     None
 *
 * @retval    None
 */
static void usart3_irq(void)
{
    usart_irq(2, USART3);
}

 /*!
 * @brief     This function handles USART4 interrupt handler
 *
 * @param     None
 *
 * @retval    None
 */
static void uart4_irq(void)
{
    usart_irq(3, UART4);
}

 /*!
 * @brief     This function handles USART5 interrupt handler
 *
 * @param     None
 *
 * @retval    None
 */
static void uart5_irq(void)
{
    usart_irq(4, UART5);
}

 /*!
 * @brief     This function handles USART6 interrupt handler
 *
 * @param     None
 *
 * @retval    None
 */
static void usart6_irq(void)
{
    usart_irq(5, USART6);
}

 /*!
 * @brief     Configure serial interrupt. This function is used for word-approach
 *
 * @param     obj : The serial object
 *
 * @param     irq : The serial IRQ type (RX or TX)
 *
 * @param     enable : Set to non-zero to enable events, or zero to disable them
 *
 * @retval    None
 */
void serial_irq_set(serial_t *obj, SerialIrq irq, uint32_t enable)
{
    struct serial_s *per_obj = GET_SERIAL_S(obj);
    USART_T *usart = (USART_T*)per_obj->uart;
    IRQn_Type irq_n = 0;
    uint32_t vector = 0;

    switch (per_obj->uart)
    {
        case UART_1:
            irq_n = USART1_IRQn;
            vector = (uint32_t)&usart1_irq;
            break;
        case UART_2:
            irq_n = USART2_IRQn;
            vector = (uint32_t)&usart2_irq;
            break;
        case UART_3:
            irq_n = USART3_IRQn;
            vector = (uint32_t)&usart3_irq;
            break;
        case UART_4:
            irq_n = UART4_IRQn;
            vector = (uint32_t)&uart4_irq;
            break;
        case UART_5:
            irq_n = UART5_IRQn;
            vector = (uint32_t)&uart5_irq;
            break;
        case UART_6:
            irq_n = USART6_IRQn;
            vector = (uint32_t)&usart6_irq;
            break;
    }

    if (enable)
    {
        if (irq == RxIrq)
        {
            /* Enable USART RX IRQ */
            USART_EnableInterrupt(usart,USART_INT_RXBNE);
        }
        else
        {
            /* Enable USART TX IRQ */
            USART_EnableInterrupt(usart,USART_INT_TXBE);
        }
        
        NVIC_SetVector(irq_n,vector);
        NVIC_EnableIRQ(irq_n);
    }
    else
    {
        if (irq == RxIrq)
        {
            USART_DisableInterrupt(usart,USART_INT_RXBNE);
        }
        else
        {
            USART_DisableInterrupt(usart,USART_INT_TXBE);
        }
    }
}

 /*!
 * @brief     Get character. This is a blocking call, waiting for a character
 *
 * @param     obj : The serial object
 *
 * @retval    None
 */
int serial_getc(serial_t *obj)
{
    int ret = 0;
    struct serial_s *per_obj = GET_SERIAL_S(obj);
    USART_T *usart = (USART_T*)per_obj->uart;

    while (!serial_readable(obj));
    if (per_obj->parity == USART_PARITY_NONE)
    {
        ret = (int)(usart->DATA & 0x1ff);
    }
    else
    {
        if (per_obj->databits == USART_WORD_LEN_8B)
        {
            ret = (int)(usart->DATA & 0x7f);
        }
        else
        {
            ret = (int)(usart->DATA & 0xff);
        }
    }
    return ret;
}

 /*!
 * @brief     Send a character. This is a blocking call, waiting for a peripheral to be available for writing.
 *
 * @param     obj : The serial object
 *
 * @param     c : The character to be sent
 *
 * @retval    None
 */
void serial_putc(serial_t *obj, int c)
{
    struct serial_s *per_obj = GET_SERIAL_S(obj);
    USART_T *usart = (USART_T*)per_obj->uart;

    while (!serial_writable(obj));
    usart->DATA = (uint32_t)(c & 0x1ff);
}

 /*!
 * @brief     Check if the serial peripheral is readable
 *
 * @param     obj : The serial object
 *
 * @retval    Non-zero value if a character can be read, 0 if nothing to read
 */
int serial_readable(serial_t *obj)
{
    struct serial_s *per_obj = GET_SERIAL_S(obj);
    USART_T *usart = (USART_T*)per_obj->uart;

    return (USART_ReadStatusFlag(usart, USART_FLAG_RXBNE) != RESET) ? 1 : 0;
}

 /*!
 * @brief     Check if the serial peripheral is writable
 *
 * @param     obj : The serial object
 *
 * @retval    Non-zero value if a character can be written, 0 otherwise.
 */
int serial_writable(serial_t *obj)
{
    struct serial_s *per_obj = GET_SERIAL_S(obj);
    USART_T *usart = (USART_T*)per_obj->uart;

    return (USART_ReadStatusFlag(usart, USART_FLAG_TXBE) != RESET) ? 1 : 0;
}

 /*!
 * @brief     Clear the serial peripheral
 *
 * @param     obj : The serial object
 *
 * @retval    None
 */
void serial_clear(serial_t *obj)
{
    struct serial_s *per_obj = GET_SERIAL_S(obj);

    per_obj->tx_count = 0U;
    per_obj->rx_count = 0U;
}

 /*!
 * @brief     Set the break
 *
 * @param     obj : The serial object
 *
 * @retval    None
 */
void serial_break_set(serial_t *obj)
{
    struct serial_s *per_obj = GET_SERIAL_S(obj);
    USART_T *usart = (USART_T*)per_obj->uart;

    USART_TxBreak(usart);
}

 /*!
 * @brief     Clear the break
 *
 * @param     obj : The serial object
 *
 * @retval    None
 */
void serial_break_clear(serial_t *obj)
{
    /* do nothing */
}

 /*!
 * @brief     Configure the TX pin for UART function.
 *
 * @param     tx : The pin name used for TX
 *
 * @retval    None
 */
void serial_pinout_tx(PinName tx)
{
    pinmap_pinout(tx, PinMap_UART_TX);
}

const PinMap *serial_tx_pinmap()
{
    return PinMap_UART_TX;
}

const PinMap *serial_rx_pinmap()
{
    return PinMap_UART_RX;
}

const PinMap *serial_cts_pinmap()
{
#if !DEVICE_SERIAL_FC
    static const PinMap PinMap_UART_CTS[] = {
        {NC, NC, 0}
    };
#endif

    return PinMap_UART_CTS;
}

const PinMap *serial_rts_pinmap()
{
#if !DEVICE_SERIAL_FC
    static const PinMap PinMap_UART_RTS[] = {
        {NC, NC, 0}
    };
#endif

    return PinMap_UART_RTS;
}

#if DEVICE_SERIAL_ASYNCH

 /*!
 * @brief     Enable the serial events
 *
 * @param     obj : The serial object
 *
 * @param     event : The events to be configured
 *
 * @retval    None
 */
static void serial_event_enable(serial_t *obj, int event)
{
    struct serial_s *per_obj = GET_SERIAL_S(obj);

    per_obj->events |= event;
}

 /*!
 * @brief     Disable the serial events
 *
 * @param     obj : The serial object
 *
 * @param     event : The events to be configured
 *
 * @retval    None
 */
static void serial_event_disable(serial_t *obj, int event)
{
    struct serial_s *per_obj = GET_SERIAL_S(obj);

    per_obj->events &= ~event;
}

 /*!
 * @brief     Preprocess the USART tx interrupt
 *
 * @param     obj_s : The serial object
 *
 * @param     p_Data : Pointer to tx buffer
 *
 * @param     size : Size of tx buffer
 *
 * @retval    Returns the status
 */
APM_STATUS_T usart_tx_pre_interrupt(struct serial_s *obj_s, uint8_t *p_Data, uint16_t size)
{
    USART_T *usart = (USART_T*)obj_s->uart;
    if (obj_s->tx_state == OP_STATE_READY)
    {
        if ((p_Data == NULL) || (size == 0U))
        {
            return APM_ERROR;
        }

        obj_s->tx_buffer_ptr = p_Data;
        obj_s->tx_count      = size;
        obj_s->error_code    = USART_ERROR_CODE_NONE;
        obj_s->tx_state      = OP_STATE_BUSY_TX;

        USART_EnableInterrupt(usart, USART_INT_TXBE);

        return APM_OK;
    }
    else
    {
        return APM_BUSY;
    }
}

 /*!
 * @brief     Preprocess the USART tx interrupt
 *
 * @param     obj_s : The serial object
 *
 * @param     p_Data : Pointer to tx buffer
 *
 * @param     size : Size of tx buffer
 *
 * @retval    Returns the status
 */
APM_STATUS_T usart_rx_pre_interrupt(struct serial_s *obj_s, uint8_t *p_Data, uint16_t size)
{
    USART_T *usart = (USART_T*)obj_s->uart;
    if (obj_s->rx_state == OP_STATE_READY)
    {
        if ((p_Data == NULL) || (size == 0U))
        {
            return APM_ERROR;
        }

        obj_s->rx_buffer_ptr = p_Data;
        obj_s->rx_size       = size;
        obj_s->rx_count      = size;
        obj_s->error_code    = USART_ERROR_CODE_NONE;
        obj_s->rx_state      = OP_STATE_BUSY_RX;

        USART_EnableInterrupt(usart, USART_INT_PE);
        USART_EnableInterrupt(usart, USART_INT_ERR);
        USART_EnableInterrupt(usart, USART_INT_RXBNE);

        return APM_OK;
    }
    else
    {
        return APM_BUSY;
    }
}

 /*!
 * @brief     Begin asynchronous TX transfer. The used buffer is specified in the serial object,tx_buff.
 *
 * @param     obj : The serial object
 *
 * @param     tx : The transmit buffer
 *
 * @param     tx_length : The number of bytes to transmit
 *
 * @param     tx_width : Deprecated argument
 *
 * @param     handler : The serial handler
 *
 * @param     event : The logical OR of events to be registered
 *
 * @param     hint : A suggestion for how to use DMA with this transfer
 *
 * @retval    Returns number of data transfered, otherwise returns 0
 */
int serial_tx_asynch(serial_t *obj, const void *tx, size_t tx_length, uint8_t tx_width, uint32_t handler, uint32_t event, DMAUsage hint)
{
    (void) hint;

    MBED_ASSERT(tx != (void *)0);
    MBED_ASSERT(tx_width == 8);//support only 8b width

    struct serial_s *per_obj = GET_SERIAL_S(obj);
    IRQn_Type irq = uart_irq_n[per_obj->index];

    if (tx_length == 0)
    {
        return 0;
    }

    if (serial_tx_active(obj))
    {
        return 0;
    }

    obj->tx_buff.buffer = (void *)tx;
    obj->tx_buff.length = tx_length;
    obj->tx_buff.pos = 0;

    /* Disable all events */
    serial_event_disable(obj,SERIAL_EVENT_TX_ALL);
    /* Enable the specific event */
    serial_event_enable(obj,event);

    /* Enable Interrupt */
    NVIC_ClearPendingIRQ(irq);
    /* Disable the irq */
    NVIC_DisableIRQ(irq);
    /* Config the priority and vector */
    NVIC_SetPriority(irq,1);
    NVIC_SetVector(irq,(uint32_t)handler);
    /* Enable the irq */
    NVIC_EnableIRQ(irq);

    if (usart_tx_pre_interrupt(per_obj,(uint8_t *)tx,tx_length) != APM_OK)
    {
        return 0;
    }
    return tx_length;
}

 /*!
 * @brief     Begin asynchronous TX transfer. The used buffer is specified in the serial object,tx_buff.
 *
 * @param     obj : The serial object
 *
 * @param     rx : The receive buffer
 *
 * @param     rx_len : The number of bytes to receive
 *
 * @param     rx_wid : Deprecated argument
 *
 * @param     handler : The serial handler
 *
 * @param     events : The logical OR of events to be registered
 *
 * @param     char_match : A character in range 0-254 to be matched
 *
 * @param     hints : A suggestion for how to use DMA with this transfer
 *
 * @retval    None
 */
void serial_rx_asynch(serial_t *obj, void *rx, size_t rx_length, uint8_t rx_width,
                      uint32_t handler, uint32_t event, uint8_t char_match, DMAUsage hint)
{
    (void) hint;

    MBED_ASSERT(obj);
    MBED_ASSERT(rx != (void *)0);
    MBED_ASSERT(rx_width == 8); // support only 8b width

    struct serial_s *per_obj = GET_SERIAL_S(obj);

    IRQn_Type irq = uart_irq_n[per_obj->index];

    if (rx_length == 0)
    {
        return ;
    }

    /* Disable all events */
    serial_event_disable(obj,SERIAL_EVENT_RX_ALL);
    /* Enable the specific event */
    serial_event_enable(obj,event);

    obj->char_match = char_match;
    
    if (serial_rx_active(obj))
    {
        return ;
    }

    obj->rx_buff.buffer = rx;
    obj->rx_buff.length = rx_length;
    obj->rx_buff.pos = 0;

    /* Enable Interrupt */
    NVIC_ClearPendingIRQ(irq);

    /* Enable Interrupt */
    NVIC_ClearPendingIRQ(irq);
    /* Disable the irq */
    NVIC_DisableIRQ(irq);
    /* Config the priority and vector */
    NVIC_SetPriority(irq,0);
    NVIC_SetVector(irq,(uint32_t)handler);
    /* Enable the irq */
    NVIC_EnableIRQ(irq);

    usart_rx_pre_interrupt(per_obj,(uint8_t *)rx,rx_length);
}

 /*!
 * @brief     Attempts to determine if the serial peripheral is already in use for TX
 *
 * @param     obj : The serial object
 *
 * @retval    Non-zero if the TX transaction is ongoing, 0 otherwise
 */
uint8_t serial_tx_active(serial_t *obj)
{
    uint8_t ret = 0;
    struct serial_s *per_obj = GET_SERIAL_S(obj);

    ret = (per_obj->tx_state == OP_STATE_BUSY_TX) ? 1 : 0;

    return ret;
}

 /*!
 * @brief     Attempts to determine if the serial peripheral is already in use for RX
 *
 * @param     obj : The serial object
 *
 * @retval    Non-zero if the RX transaction is ongoing, 0 otherwise
 */
uint8_t serial_rx_active(serial_t *obj)
{
    uint8_t ret = 0;
    struct serial_s *per_obj = GET_SERIAL_S(obj);

    ret = (per_obj->rx_state == OP_STATE_BUSY_RX) ? 1 : 0;

    return ret;
}

 /*!
 * @brief     Handle the serial rx interrupt
 *
 * @param     obj : The serial object
 *
 * @retval    Returns the status
 */
static APM_STATUS_T usart_rx_interrupt(struct serial_s *obj_s)
{
    USART_T *usart = (USART_T*)obj_s->uart;
    uint16_t *temp;

    if (obj_s->rx_state == OP_STATE_BUSY_RX)
    {
        if (obj_s->databits == USART_WORD_LEN_9B)
        {
            temp = (uint16_t *)obj_s->rx_buffer_ptr;
            if (obj_s->parity == USART_PARITY_NONE)
            {
                /* 9-bit data, none parity bit */
                *temp = (uint16_t)(usart->DATA & 0x1ff);
                obj_s->rx_buffer_ptr += 2U;
            }
            else
            {
                /* 9-bit data, with parity bit */
                *temp = (uint16_t)(usart->DATA & 0xff);
                obj_s->rx_buffer_ptr += 1U;
            }
        }
        else
        {
            temp = (uint16_t *)obj_s->rx_buffer_ptr;
            if (obj_s->parity == USART_PARITY_NONE)
            {
                /* 8-bit data, none parity bit */
                *temp = (uint16_t)(usart->DATA & 0xff);
                obj_s->rx_buffer_ptr += 1U;
            }
            else
            {
                /* 8-bit data, with parity bit */
                *temp = (uint16_t)(usart->DATA & 0x7f);
                obj_s->rx_buffer_ptr += 1U;
            }
        }

        if (--obj_s->rx_count == 0U)
        {
            USART_DisableInterrupt(usart,USART_INT_RXBNE);
            USART_DisableInterrupt(usart,USART_INT_PE);
            USART_DisableInterrupt(usart,USART_INT_ERR);
            
            obj_s->rx_state = OP_STATE_READY;
        }

        return APM_OK;
    }
    else
    {
        return APM_BUSY;
    }
}

 /*!
 * @brief     Handle the serial tx interrupt
 *
 * @param     obj : The serial object
 *
 * @retval    Returns the status
 */
static APM_STATUS_T usart_tx_interrupt(struct serial_s *obj_s)
{
    USART_T *usart = (USART_T*)obj_s->uart;
    uint16_t *temp;

    if (obj_s->tx_state == OP_STATE_BUSY_TX)
    {
        if (obj_s->databits == USART_WORD_LEN_9B)
        {
            temp = (uint16_t *)obj_s->tx_buffer_ptr;
            usart->DATA = (uint16_t)(*temp & (uint16_t)0x01ff);

            if (obj_s->parity == USART_PARITY_NONE)
            {
                /* 9-bit data, none parity bit */
                obj_s->tx_buffer_ptr += 2U;
            }
            else
            {
                /* 9-bit data, with parity bit */
                obj_s->tx_buffer_ptr += 1U;
            }
        }
        else
        {
            temp = (uint16_t *)obj_s->tx_buffer_ptr;
            usart->DATA = (uint16_t)(*temp & (uint16_t)0x00ff);

            obj_s->tx_buffer_ptr += 1U;
        }
        if (--obj_s->tx_count == 0U)
        {
            USART_DisableInterrupt(usart,USART_INT_TXBE);

            USART_EnableInterrupt(usart,USART_INT_TXC);
        }
        return APM_OK;
    }
    return APM_BUSY;
}

 /*!
 * @brief     Handle the serial tx complete interrupt
 *
 * @param     obj : The serial object
 *
 * @retval    None
 */
void usart_tx_complete_interrupt(struct serial_s *obj_s)
{
    USART_T *usart = (USART_T*)obj_s->uart;

    USART_DisableInterrupt(usart,USART_INT_TXC);

    obj_s->tx_state = OP_STATE_READY;
}

 /*!
 * @brief     Handle all the serial interrupt request
 *
 * @param     obj : The serial object
 *
 * @retval    None
 */
void usart_irq_handler(struct serial_s *obj_s)
{
    uint32_t error = 0U;
    USART_T *usart = (USART_T*)obj_s->uart;

    error = (usart->STS) & (uint32_t)((USART_FLAG_PE | USART_FLAG_FE | USART_FLAG_NE | USART_FLAG_OVRE));

    if (error == 0U)
    {
        if (USART_ReadIntFlag(usart,USART_INT_RXBNE))
        {
            usart_rx_interrupt(obj_s);

            return;
        }
    }
    if (error != 0U)
    {
        if (USART_ReadIntFlag(usart,USART_INT_PE))
        {
            obj_s->error_code |= USART_ERROR_CODE_PERR;
        }
        if (USART_ReadIntFlag(usart,USART_INT_FE))
        {
            obj_s->error_code |= USART_ERROR_CODE_FERR;
        }
        if (USART_ReadIntFlag(usart,USART_INT_NE))
        {
            obj_s->error_code |= USART_ERROR_CODE_NERR;
        }
        if (USART_ReadIntFlag(usart,USART_INT_OVRE_ER))
        {
            obj_s->error_code |= USART_ERROR_CODE_OVER;
        }
        if (obj_s->error_code != USART_ERROR_CODE_NONE)
        {
            if (USART_ReadIntFlag(usart,USART_INT_RXBNE))
            {
                usart_rx_interrupt(obj_s);
            }
        }
        return ;
    }

    if (USART_ReadIntFlag(usart,USART_INT_TXBE))
    {
        usart_tx_interrupt(obj_s);
        return;
    }

    if (USART_ReadIntFlag(usart,USART_INT_TXC))
    {
        usart_tx_complete_interrupt(obj_s);
        return;
    }
}

 /*!
 * @brief     The asynchronous TX and RX handler.
 *
 * @param     obj : The serial object
 *
 * @retval    Returns event flags if an RX transfer termination condition was met; otherwise returns 0
 */
int serial_irq_handler_asynch(serial_t *obj)
{
    struct serial_s *per_obj = GET_SERIAL_S(obj);
    USART_T *usart = (USART_T*)per_obj->uart;
    volatile uint8_t i = 0;
    volatile int ret = 0;
    uint8_t *p_buf = (uint8_t *)(obj->rx_buff.buffer);

    if (USART_ReadIntFlag(usart,USART_INT_PE))
    {
        USART_ClearIntFlag(usart,USART_INT_PE);
        ret |= (SERIAL_EVENT_RX_PARITY_ERROR & per_obj->events);
        per_obj->error_code |= USART_ERROR_CODE_PERR;
    }

    if (USART_ReadIntFlag(usart,USART_INT_FE))
    {
        USART_ClearIntFlag(usart,USART_INT_FE);
        ret |= (SERIAL_EVENT_RX_FRAMING_ERROR & per_obj->events);
        per_obj->error_code |= USART_ERROR_CODE_FERR;
    }

    if (USART_ReadIntFlag(usart,USART_INT_OVRE_ER))
    {
        USART_ClearIntFlag(usart,USART_INT_OVRE_ER);
        ret |= (SERIAL_EVENT_RX_OVERRUN_ERROR & per_obj->events);
        per_obj->error_code |= USART_ERROR_CODE_OVER;
    }

    if (ret & (SERIAL_EVENT_RX_PARITY_ERROR | SERIAL_EVENT_RX_FRAMING_ERROR |
                      SERIAL_EVENT_RX_OVERRUN_ERROR)) {
        return ret;
    }

    if (USART_ReadIntFlag(usart,USART_INT_TXC))
    {
        if ((per_obj->events & SERIAL_EVENT_TX_COMPLETE) != 0)
        {
            ret |= (SERIAL_EVENT_TX_COMPLETE & per_obj->events);
        }
    }

    usart_irq_handler(per_obj);

    if (per_obj->rx_size != 0)
    {
        obj->rx_buff.pos = per_obj->rx_size - per_obj->rx_count;
    }
    if ((per_obj->rx_count == 0) && (obj->rx_buff.pos >= (obj->rx_buff.length - 1)))
    {
        ret |= (SERIAL_EVENT_RX_COMPLETE & per_obj->events);
    }
    if (per_obj->events & SERIAL_EVENT_RX_CHARACTER_MATCH)
    {
        if (p_buf != NULL)
        {
            for (i = 0; i < obj->rx_buff.pos; i++)
            {
                if (p_buf[i] == obj->char_match)
                {
                    obj->rx_buff.pos = i;
                    ret |= (SERIAL_EVENT_RX_CHARACTER_MATCH & per_obj->events);
                    serial_rx_abort_asynch(obj);
                }
            }
        }
    }

    return ret;
}

 /*!
 * @brief     Abort the ongoing TX transaction. It disables the enabled interupt for TX and flushes
 *            the TX hardware buffer if TX FIFO is used.
 * @param     obj : The serial object
 *
 * @retval    None
 */
void serial_tx_abort_asynch(serial_t *obj)
{
    struct serial_s *per_obj = GET_SERIAL_S(obj);
    USART_T *usart = (USART_T*)per_obj->uart;

    USART_DisableInterrupt(usart,USART_INT_TXC);
    USART_DisableInterrupt(usart,USART_INT_TXBE);

    USART_ClearIntFlag(usart,USART_INT_TXC);

    per_obj->tx_count = 0;
    per_obj->tx_state = OP_STATE_READY;
}

 /*!
 * @brief     Abort the ongoing RX transaction. It disables the enabled interrupt for RX and flushes
 *            the RX hardware buffer if RX FIFO is used.
 * @param     obj : The serial object
 *
 * @retval    None
 */
void serial_rx_abort_asynch(serial_t *obj)
{
    struct serial_s *per_obj = GET_SERIAL_S(obj);
    USART_T *usart = (USART_T*)per_obj->uart;

    USART_DisableInterrupt(usart,USART_INT_RXBNE);
    USART_DisableInterrupt(usart,USART_INT_PE);
    USART_DisableInterrupt(usart,USART_INT_ERR);

    USART_ClearIntFlag(usart,USART_INT_RXBNE);

    USART_ClearIntFlag(usart,USART_INT_PE);
    USART_ClearIntFlag(usart,USART_INT_FE);
    USART_ClearIntFlag(usart,USART_INT_OVRE_ER);

    /* Clear RXBNE register */
    (void)(usart->DATA);

    per_obj->rx_count = 0;
    per_obj->rx_state = OP_STATE_READY;
}

#endif /* DEVICE_SERIAL_ASYNCH */

#if DEVICE_SERIAL_FC
 /*!
 * @brief     Configure the serial for the flow control. It sets flow control in the hardware
 *            if a serial peripheral supports it, otherwise software emulation is used.
 * @param     obj : The serial object
 *
 * @param     type : The type of the flow control. Look at the available FlowControl types.
 *
 * @param     rxflow : The TX pin name
 *
 * @param     txflow : The RX pin name
 *
 * @retval    None
 */
void serial_set_flow_control(serial_t *obj, FlowControl type, PinName rxflow, PinName txflow)
{
    struct serial_s *per_obj = GET_SERIAL_S(obj);
    USART_T *usart = (USART_T*)per_obj->uart;

    UARTName uart_rts = (UARTName)pinmap_peripheral(rxflow, PinMap_UART_RTS);
    UARTName uart_cts = (UARTName)pinmap_peripheral(txflow, PinMap_UART_CTS);

    per_obj->uart = (UARTName)pinmap_merge(uart_cts, uart_rts);

    MBED_ASSERT(per_obj->uart != (UARTName)NC);

    USART_Disable(usart);

    USART_Config_T usartConfig;
    usartConfig.baudRate = per_obj->baudrate;
    usartConfig.mode = USART_MODE_TX_RX;
    usartConfig.parity = per_obj->parity;
    usartConfig.stopBits = per_obj->stopbits;
    usartConfig.wordLength = per_obj->databits;

    switch (type)
    {
        case FlowControlNone:
            per_obj->hw_flow_ctl = USART_HARDWARE_FLOW_NONE;
            usartConfig.hardwareFlow = USART_HARDWARE_FLOW_NONE;
            break;
        case FlowControlRTS:
            MBED_ASSERT(uart_rts != (UARTName)NC);
            per_obj->hw_flow_ctl = USART_HARDWARE_FLOW_RTS;
            per_obj->pin_rts = rxflow;
            pinmap_pinout(rxflow, PinMap_UART_RTS);
            usartConfig.hardwareFlow = USART_HARDWARE_FLOW_RTS;
            break;
        case FlowControlCTS:
            MBED_ASSERT(uart_cts != (UARTName)NC);
            per_obj->hw_flow_ctl = USART_HARDWARE_FLOW_CTS;
            per_obj->pin_cts = txflow;
            pinmap_pinout(txflow, PinMap_UART_CTS);
            usartConfig.hardwareFlow = USART_HARDWARE_FLOW_CTS;
            break;
        case FlowControlRTSCTS:
            MBED_ASSERT(uart_rts != (UARTName)NC);
            MBED_ASSERT(uart_cts != (UARTName)NC);
            per_obj->pin_rts = rxflow;
            per_obj->pin_cts = txflow;
            pinmap_pinout(rxflow, PinMap_UART_RTS);
            pinmap_pinout(txflow, PinMap_UART_CTS);
            per_obj->hw_flow_ctl = USART_HARDWARE_FLOW_RTS_CTS;
            usartConfig.hardwareFlow = USART_HARDWARE_FLOW_RTS_CTS;
            break;
    }

    USART_Config(usart,&usartConfig);
    USART_Enable(usart);
}

#endif /* DEVICE_SERIAL_FC */

#if DEVICE_SLEEP
/** Check whether the serial is in busy state
 *
 * @return 0: all the serial is free to use, 1: some serial is in busy in transfer
 */
 /*!
 * @brief     Check whether the serial is in busy state
 *
 * @param     NOne
 *
 * @retval    0: all the serial is free to use, 1: some serial is in busy in transfer
 */
int serial_busy_state_check(void)
{
#if defined(USART1)
    if (USART1->CTRL1_B.UEN && !(USART1->STS_B.TXCFLG))
    {
        return 1;
    }
#endif
#if defined(USART2)
    if (USART2->CTRL1_B.UEN && !(USART2->STS_B.TXCFLG))
    {
        return 1;
    }
#endif
#if defined(USART3)
    if (USART3->CTRL1_B.UEN && !(USART3->STS_B.TXCFLG))
    {
        return 1;
    }
#endif
#if defined(UART4)
    if (UART4->CTRL1_B.UEN && !(UART4->STS_B.TXCFLG))
    {
        return 1;
    }
#endif
#if defined(UART5)
    if (UART5->CTRL1_B.UEN && !(UART5->STS_B.TXCFLG))
    {
        return 1;
    }
#endif
#if defined(USART6)
    if (USART6->CTRL1_B.UEN && !(USART6->STS_B.TXCFLG))
    {
        return 1;
    }
#endif
    return 0;
}
#endif /* DEVICE_SLEEP */

#endif /* DEVICE_SERIAL */
