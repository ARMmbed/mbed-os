/* mbed Microcontroller Library
 * Copyright (c) 2006-2015 ARM Limited
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

/***************************************************************** Includes **/
#include "mbed_assert.h"
#include "serial_api.h"

#include "cmsis.h"
#include "pinmap.h"
#include "PeripheralPins.h"

#include "xmc_gpio.h"
#include "xmc_uart.h"

#include "xmc4_helper.h"

/****************************************************************** Defines **/
#ifdef USIC2
#define UART_NUM 6
#elif USIC1
#define UART_NUM 4
#else
#define UART_NUM 2
#endif

/****************************************************************** Globals **/
int stdio_uart_inited = 0;
serial_t stdio_uart;

static uint32_t serial_irq_ids[UART_NUM] = {0};
static uart_irq_handler irq_handler;

/******************************************************* Internal Functions **/

/* @internal uart_irq
 *
 * @param channel UART Channel
 * @param index ISR Index
 *
 */
static void uart_irq(XMC_USIC_CH_t *const channel, uint32_t index) {

    /* Check if interrupt is in registry */
    if (serial_irq_ids[index] != 0) {
        /* Get UART status flag */
        uint32_t status = XMC_UART_CH_GetStatusFlag(channel);
        /* Check if receive or transmit interrupt */
        if((status & XMC_UART_CH_STATUS_FLAG_RECEIVE_INDICATION) ||
                (status & XMC_UART_CH_STATUS_FLAG_ALTERNATIVE_RECEIVE_INDICATION)) {
            /* Rx Interrupt */
            irq_handler(serial_irq_ids[index], RxIrq);
        } else if(status & XMC_UART_CH_STATUS_FLAG_TRANSMIT_BUFFER_INDICATION ) {
            /* Tx Interrupt */
            irq_handler(serial_irq_ids[index], TxIrq);
        }
    }
}

/************************************************************** ISR Handler **/

/**
 * ISR Handler for UART_1
 */
void uart0_irq(void) {

    uart_irq((XMC_USIC_CH_t *)UART_0, 0);
}

/**
 * ISR Handler for UART_2
 */
void uart1_irq(void) {

    uart_irq((XMC_USIC_CH_t *)UART_1, 1);
}

/**
 * ISR Handler for UART_3
 */
void uart2_irq(void) {

    uart_irq((XMC_USIC_CH_t *)UART_2, 2);
}

/**
 * ISR Handler for UART_4
 */
void uart3_irq(void) {

    uart_irq((XMC_USIC_CH_t *)UART_3, 3);
}

/**
 * ISR Handler for UART_5
 */
void uart4_irq(void) {

    uart_irq((XMC_USIC_CH_t *)UART_4, 4);
}

/**
 * ISR Handler for UART_6
 */
void uart5_irq(void) {

    uart_irq((XMC_USIC_CH_t *)UART_5, 5);
}

/**************************************************************** Functions **/

/** Initialize the serial peripheral. It sets the default parameters for serial
 *  peripheral, and configures its specifieds pins.
 *
 * @param obj The serial object
 * @param tx  The TX pin name
 * @param rx  The RX pin name
 */
void serial_init(serial_t *obj, PinName tx, PinName rx) {

    /* Determine corresponding USIC module & channel */
    UARTName uart_tx = (UARTName)pinmap_peripheral(tx, PinMap_UART_TX);
    UARTName uart_rx = (UARTName)pinmap_peripheral(rx, PinMap_UART_RX);
    UARTName uart = (UARTName)pinmap_merge(uart_tx, uart_rx);
    /* Check if both pins are a possible UART pair */
    MBED_ASSERT((int)uart != NC);

    /* Get TX pin alternative function */
    uint8_t tx_function = (uint8_t)pinmap_function(tx, PinMap_UART_TX);

    /* CFG for TX Pin */
    XMC_GPIO_CONFIG_t uart_tx_cfg =
        {
            .mode = XMC_GPIO_MODE_OUTPUT_PUSH_PULL | (tx_function << PORT0_IOCR0_PC0_Pos),
            .output_strength = XMC_GPIO_OUTPUT_STRENGTH_MEDIUM
        };
    /* CFG for RX Pin */
    XMC_GPIO_CONFIG_t uart_rx_cfg =
        {
            .mode = XMC_GPIO_MODE_INPUT_TRISTATE
        };

    /* Set default UART configuration */
    XMC_UART_CH_CONFIG_t uart_config =
        {
            .data_bits = 8U,
            .stop_bits = 1U,
            .baudrate = 9600U
        };

    /* Get USIC BASE address */
    XMC_USIC_CH_t *channel = ((XMC_USIC_CH_t *)uart);
    obj->channel = channel;
    /* Get RX pin function */
    pin_io_function_t *rx_function = get_pin_function(rx);

    /* Set UART index */
    switch ((int)obj->channel) {
        case UART_0:
            obj->index = 0;
            obj->dx0 = rx_function->usic0_ch0_dx0;
            break;
        case UART_1:
            obj->index = 1;
            obj->dx0 = rx_function->usic0_ch1_dx0;
            break;
#ifdef USIC1

        case UART_2:
            obj->index = 2;
            obj->dx0 = rx_function->usic1_ch0_dx0;
            break;
        case UART_3:
            obj->index = 3;
            obj->dx0 = rx_function->usic1_ch1_dx0;
            break;
#endif
#ifdef USIC2

        case UART_4:
            obj->index = 4;
            obj->dx0 = rx_function->usic2_ch0_dx0;
            break;
        case UART_5:
            obj->index = 5;
            obj->dx0 = rx_function->usic2_ch1_dx0;
            break;
#endif

    }

    /* Get Port Base address */
    XMC_GPIO_PORT_t *port_tx = (XMC_GPIO_PORT_t *)XMC4_GET_PORT_BASE_FROM_NAME(tx);
    XMC_GPIO_PORT_t *port_rx = (XMC_GPIO_PORT_t *)XMC4_GET_PORT_BASE_FROM_NAME(rx);

    /* Initialize UART Channel */
    XMC_UART_CH_Init(channel, &uart_config);
    XMC_UART_CH_SetInputSource(channel, XMC_UART_CH_INPUT_RXD, obj->dx0);
    XMC_UART_CH_Start(channel);
    XMC_GPIO_Init(port_tx, XMC4_GET_PIN_NBR_FROM_NAME(tx), &uart_tx_cfg);
    XMC_GPIO_Init(port_rx, XMC4_GET_PIN_NBR_FROM_NAME(rx), &uart_rx_cfg);

    if (uart == STDIO_UART) {
        stdio_uart_inited = 1;
        memcpy(&stdio_uart, obj, sizeof(serial_t));
    }
}

/** Release the serial peripheral, not currently invoked. It requires further
 *  resource management.
 *
 * @param obj The serial object
 */
void serial_free(serial_t *obj) {

    serial_irq_ids[obj->index] = 0;
}

/** Configure the baud rate
 *
 * @param obj      The serial object
 * @param baudrate The baud rate to be configured
 */
void serial_baud(serial_t *obj, int baudrate) {

    XMC_UART_CH_SetBaudrate(obj->channel, baudrate, 16);
}

/** Configure the format. Set the number of bits, parity and the number of stop bits
 *
 * @param obj       The serial object
 * @param data_bits The number of data bits
 * @param parity    The parity
 * @param stop_bits The number of stop bits
 */
void serial_format(serial_t *obj, int data_bits, SerialParity parity, int stop_bits) {

    XMC_UART_CH_CONFIG_t uart_config;

    /* Set parity mode */
    switch(parity) {
        case ParityNone:
            uart_config.parity_mode = XMC_USIC_CH_PARITY_MODE_NONE;
            break;
        case ParityOdd:
            uart_config.parity_mode = XMC_USIC_CH_PARITY_MODE_EVEN;
            break;
        case ParityEven:
            uart_config.parity_mode = XMC_USIC_CH_PARITY_MODE_ODD;
            break;
        default:
            uart_config.parity_mode = XMC_USIC_CH_PARITY_MODE_NONE;
            break;
    }

    /* Set data bits */
    uart_config.data_bits = data_bits;
    /* Set stop bits */
    uart_config.stop_bits = stop_bits;

    /* Stop UART Channel */
    XMC_UART_CH_Stop(obj->channel);
    /* Initialize & start UART Channel */
    XMC_UART_CH_Init(obj->channel, &uart_config);
    XMC_UART_CH_SetInputSource(obj->channel, XMC_UART_CH_INPUT_RXD, obj->dx0);
    XMC_UART_CH_Start(obj->channel);
}

/** The serial interrupt handler registration
 *
 * @param obj     The serial object
 * @param handler The interrupt handler which will be invoked when the interrupt fires
 * @param id      The SerialBase object
 */
void serial_irq_handler(serial_t *obj, uart_irq_handler handler, uint32_t id) {

    irq_handler = handler;
    serial_irq_ids[obj->index] = id;
}

/** Configure serial interrupt. This function is used for word-approach
 *
 * @param obj    The serial object
 * @param irq    The serial IRQ type (RX or TX)
 * @param enable Set to non-zero to enable events, or zero to disable them
 */
void serial_irq_set(serial_t *obj, SerialIrq irq, uint32_t enable) {

    IRQn_Type irq_n = (IRQn_Type)0;			///> interrupt number
    uint32_t vector = 0;					///> interrupt vector
    uint32_t service_request_nbr = 0;		///> interrupt request line (0-5)

    /* Set interrupt variables for the specific UART channel */
    switch ((int)obj->channel) {
        case UART_0:
            irq_n = USIC0_0_IRQn;
            service_request_nbr = 0;
            vector = (uint32_t)&uart0_irq;
            break;
        case UART_1:
            irq_n = USIC0_1_IRQn;
            service_request_nbr = 1;
            vector = (uint32_t)&uart1_irq;
            break;
#ifdef USIC1

        case UART_2:
            irq_n = USIC1_0_IRQn;
            service_request_nbr = 0;
            vector = (uint32_t)&uart2_irq;
            break;
        case UART_3:
            irq_n = USIC1_1_IRQn;
            service_request_nbr = 1;
            vector = (uint32_t)&uart3_irq;
            break;
#endif
#ifdef USIC2

        case UART_4:
            irq_n = USIC2_0_IRQn;
            service_request_nbr = 0;
            vector = (uint32_t)&uart4_irq;
            break;
        case UART_5:
            irq_n = USIC2_1_IRQn;
            service_request_nbr = 1;
            vector = (uint32_t)&uart5_irq;
            break;
#endif

    }

    /* Enable specific UART interrupt */
    if (enable) {
        switch (irq) {
            case RxIrq:
                /* Enable receive events */
                XMC_UART_CH_EnableEvent(obj->channel, XMC_UART_CH_EVENT_STANDARD_RECEIVE);
                XMC_UART_CH_EnableEvent(obj->channel, XMC_UART_CH_EVENT_ALTERNATIVE_RECEIVE);
                break;
            case TxIrq:
                /* Enable transmit events */
                XMC_UART_CH_EnableEvent(obj->channel, XMC_UART_CH_EVENT_TRANSMIT_BUFFER);
                break;
        }
        /* Set interrupt node pointer */
        XMC_USIC_CH_SetInterruptNodePointer(obj->channel, XMC_USIC_CH_INTERRUPT_NODE_POINTER_RECEIVE, service_request_nbr);
        XMC_USIC_CH_SetInterruptNodePointer(obj->channel, XMC_USIC_CH_INTERRUPT_NODE_POINTER_ALTERNATE_RECEIVE, service_request_nbr);
        /* Set NVIC vector and enable interrupt */
        NVIC_SetVector(irq_n, vector);
        NVIC_EnableIRQ(irq_n);
    } else { /* Disable specific UART interrupt */
        switch (irq) {
            case RxIrq:
                /* Disable receive events */
                XMC_UART_CH_DisableEvent(obj->channel, XMC_UART_CH_EVENT_STANDARD_RECEIVE);
                XMC_UART_CH_DisableEvent(obj->channel, XMC_UART_CH_EVENT_ALTERNATIVE_RECEIVE);
                break;
            case TxIrq:
                /* Disable transmit events */
                XMC_UART_CH_DisableEvent(obj->channel, XMC_UART_CH_EVENT_TRANSMIT_BUFFER);
                break;
        }
        /* NVIC disable interrupt */
        NVIC_DisableIRQ(irq_n);
    }
}

/** Get character. This is a blocking call, waiting for a character
 *
 * @param obj The serial object
 */
int  serial_getc(serial_t *obj) {

    return XMC_UART_CH_GetReceivedData(obj->channel);
}

/** Send a character. This is a blocking call, waiting for a peripheral to be available
 *  for writing
 *
 * @param obj The serial object
 * @param c   The character to be sent
 */
void serial_putc(serial_t *obj, int c) {

    XMC_UART_CH_Transmit(obj->channel, (uint16_t)c);
}

/** Check if the serial peripheral is readable
 *
 * @param obj The serial object
 * @return Non-zero value if a character can be read, 0 if nothing to read
 */
int  serial_readable(serial_t *obj) {

    /* Get UART channel status */
    uint32_t status = XMC_UART_CH_GetStatusFlag(obj->channel);
    if((status & XMC_UART_CH_STATUS_FLAG_RECEIVE_INDICATION) ||
            (status & XMC_UART_CH_STATUS_FLAG_ALTERNATIVE_RECEIVE_INDICATION)) {

        /* Clear status flag */
        XMC_UART_CH_ClearStatusFlag(obj->channel, XMC_UART_CH_STATUS_FLAG_RECEIVE_INDICATION | XMC_UART_CH_STATUS_FLAG_ALTERNATIVE_RECEIVE_INDICATION);
        /* Read data available */
        return 1;
    }
    /* No Read data available */
    return 0;
}

/** Check if the serial peripheral is writable
 *
 * @param obj The serial object
 * @return Non-zero value if a character can be written, 0 otherwise.
 */
int  serial_writable(serial_t *obj) {

    /* Get UART channel status */
    uint32_t status = XMC_UART_CH_GetStatusFlag(obj->channel);
    if(status == XMC_UART_CH_STATUS_FLAG_TRANSMISSION_IDLE ) {
        /* UART is in idle state */
        return 1;
    }
    /* UART is busy */
    return 0;
}

/** Clear the serial peripheral
 *
 * @param obj The serial object
 */
void serial_clear(serial_t *obj) {

    /* Stop UART Channel */
    XMC_UART_CH_Stop(obj->channel);
}

/** Set the break
 *
 * @param obj The serial object
 */
void serial_break_set(serial_t *obj) {
    /* Not implemented */
}

/** Clear the break
 *
 * @param obj The serial object
 */
void serial_break_clear(serial_t *obj) {
    /* Not implemented */
}

/** Configure the TX pin for UART function.
 *
 * @param tx The pin name used for TX
 */
void serial_pinout_tx(PinName tx) {

    /* Get TX pin alternative function */
    uint8_t tx_function = (uint8_t)pinmap_function(tx, PinMap_UART_TX);
    /* CFG for TX Pin */
    XMC_GPIO_CONFIG_t uart_tx_cfg =
        {
            .mode = XMC_GPIO_MODE_OUTPUT_PUSH_PULL | (tx_function << PORT0_IOCR0_PC0_Pos),
            .output_strength = XMC_GPIO_OUTPUT_STRENGTH_MEDIUM
        };

    /* Get Port Base address */
    XMC_GPIO_PORT_t *port_tx = (XMC_GPIO_PORT_t *)XMC4_GET_PORT_BASE_FROM_NAME(tx);
    /* Initialize Tx Pin */
    XMC_GPIO_Init(port_tx, XMC4_GET_PIN_NBR_FROM_NAME(tx), &uart_tx_cfg);
}

/** Configure the serial for the flow control. It sets flow control in the hardware
 *  if a serial peripheral supports it, otherwise software emulation is used.
 *
 * @param obj    The serial object
 * @param type   The type of the flow control. Look at the available FlowControl types.
 * @param rxflow The TX pin name
 * @param txflow The RX pin name
 */
void serial_set_flow_control(serial_t *obj, FlowControl type, PinName rxflow, PinName txflow) {
    /* Not implemented */
}

/*EOF*/
