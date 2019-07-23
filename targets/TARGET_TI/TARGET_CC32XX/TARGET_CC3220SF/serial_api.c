/* mbed Microcontroller Library
 * Copyright (c) 2006-2018 ARM Limited
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
// math.h required for floating point operations for baud rate calculation
#include <math.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "mbed_assert.h"

#include "serial_api.h"
#include "serial_object.h"
#include "cmsis.h"
#include "pinmap.h"
#include "mbed_error.h"
#include "gpio_api.h"
#include "PeripheralPins.h"

#include <ti/devices/cc32xx/inc/hw_memmap.h>
#include <ti/devices/cc32xx/inc/hw_ocp_shared.h>
#include <ti/devices/cc32xx/inc/hw_ints.h>
#include <ti/devices/cc32xx/inc/hw_types.h>
#include <ti/devices/cc32xx/inc/hw_uart.h>
#include <ti/devices/cc32xx/inc/hw_common_reg.h>
#include <ti/devices/cc32xx/driverlib/rom.h>
#include <ti/devices/cc32xx/driverlib/rom_map.h>
#include <ti/devices/cc32xx/driverlib/uart.h>
#include <ti/devices/cc32xx/driverlib/pin.h>
#include <ti/devices/cc32xx/driverlib/prcm.h>
#include <ti/devices/cc32xx/driverlib/interrupt.h>
#include <ti/devices/cc32xx/driverlib/utils.h>

/******************************************************************************
 * INITIALIZATION
 ******************************************************************************/
#define UART_NUM    2

static const uint32_t dataLength[] = {
    UART_CONFIG_WLEN_5,     /* UART_LEN_5 */
    UART_CONFIG_WLEN_6,     /* UART_LEN_6 */
    UART_CONFIG_WLEN_7,     /* UART_LEN_7 */
    UART_CONFIG_WLEN_8      /* UART_LEN_8 */
};

static const uint32_t stopBits[] = {
    UART_CONFIG_STOP_ONE,   /* UART_STOP_ONE */
    UART_CONFIG_STOP_TWO    /* UART_STOP_TWO */
};

static const uint32_t parityType[] = {
    UART_CONFIG_PAR_NONE,   /* UART_PAR_NONE */
    UART_CONFIG_PAR_EVEN,   /* UART_PAR_EVEN */
    UART_CONFIG_PAR_ODD,    /* UART_PAR_ODD */
    UART_CONFIG_PAR_ZERO,   /* UART_PAR_ZERO */
    UART_CONFIG_PAR_ONE     /* UART_PAR_ONE */
};

static uart_irq_handler irq_handler;

int stdio_uart_inited = 0;
serial_t stdio_uart;

struct serial_global_data_s {
    uint32_t serial_irq_id;
    gpio_t sw_rts, sw_cts;
    uint8_t count, rx_irq_set_flow, rx_irq_set_api;
};

static struct serial_global_data_s uart_data[UART_NUM];

void serial_init(serial_t *obj, PinName tx, PinName rx)
{
    int is_stdio_uart = 0;

    // determine the UART to use
    UARTName uart_tx = (UARTName)pinmap_peripheral(tx, PinMap_UART_TX);
    UARTName uart_rx = (UARTName)pinmap_peripheral(rx, PinMap_UART_RX);
    UARTName uart = (UARTName)pinmap_merge(uart_tx, uart_rx);
    MBED_ASSERT((int)uart != NC);

    obj->uart = (CC3220SF_UART_TypeDef *)uart;

    // Set default values for UART
    obj->dataLength = UART_LEN_8;
    obj->stopBits = UART_STOP_ONE;
    obj->parityType = UART_PAR_NONE;

    switch (uart) {
        case UART_0: {
            obj->index = 0;
            obj->baseAddr = CC3220SF_UARTA0_BASE;
            obj->powerMgrId = 12; /*!< Resource ID: UART 0 */
            obj->intNum = INT_UARTA0_IRQn;
            obj->peripheralId = PRCM_UARTA0;
        }
        break;
        case UART_1: {
            obj->index = 1;
            obj->baseAddr = CC3220SF_UARTA1_BASE;
            obj->powerMgrId = 13; /*!< Resource ID: UART 1 */
            obj->intNum = INT_UARTA1_IRQn;
            obj->peripheralId = PRCM_UARTA1;
        }
        break;
    }

    PRCMPeripheralClkEnable(obj->peripheralId, PRCM_RUN_MODE_CLK);

    // Pinout the chosen uart
    pinmap_pinout(tx, PinMap_UART_TX);
    pinmap_pinout(rx, PinMap_UART_RX);

    MAP_PinTypeUART(tx, pinmap_function(tx, PinMap_UART_TX));
    MAP_PinTypeUART(rx, pinmap_function(rx, PinMap_UART_RX));

    MAP_UARTEnable(obj->baseAddr);

    // Set default baud rate and format
    serial_baud(obj, 9600);
    serial_format(obj, 8, ParityNone, 1);

    // set rx/tx pins in PullUp mode
    if (tx != NC) {
        pin_mode(tx, PullUp);
    }
    if (rx != NC) {
        pin_mode(rx, PullUp);
    }

    /* Set flow control */
    uart_data[obj->index].sw_rts.pin = NC;
    uart_data[obj->index].sw_cts.pin = NC;
    serial_set_flow_control(obj, FlowControlNone, NC, NC);

    is_stdio_uart = (uart == STDIO_UART) ? (1) : (0);

    if (is_stdio_uart) {
        stdio_uart_inited = 1;
        memcpy(&stdio_uart, obj, sizeof(serial_t));
    }
}

void serial_free(serial_t *obj)
{
    uart_data[obj->index].serial_irq_id = 0;
}

void serial_baud(serial_t *obj, int baudrate)
{
    obj->baudRate = baudrate;

    MAP_UARTConfigSetExpClk(obj->baseAddr, MAP_PRCMPeripheralClockGet(obj->peripheralId),
                            obj->baudRate, (dataLength[obj->dataLength] |
                                            stopBits[obj->stopBits] | parityType[obj->parityType]));
}

void serial_format(serial_t *obj, int data_bits, SerialParity parity, int stop_bits)
{
    MBED_ASSERT((stop_bits == 1) || (stop_bits == 2)); // 0: 1 stop bits, 1: 2 stop bits
    MBED_ASSERT((data_bits > 4) && (data_bits < 9)); // 0: 5 data bits ... 3: 8 data bits
    MBED_ASSERT((parity == ParityNone) || (parity == ParityOdd) || (parity == ParityEven) ||
                (parity == ParityForced1) || (parity == ParityForced0));

    stop_bits -= 1;
    data_bits -= 5;

    switch (parity) {
        case ParityNone:
            obj->parityType = UART_PAR_NONE;
            break;
        case ParityOdd :
            obj->parityType = UART_PAR_ODD;
            break;
        case ParityEven:
            obj->parityType = UART_PAR_EVEN;
            break;
        case ParityForced1:
            obj->parityType = UART_PAR_ONE;
            break;
        case ParityForced0:
            obj->parityType = UART_PAR_ZERO;
            break;
        default:
            obj->parityType = UART_PAR_NONE;
            break;
    }

    switch (data_bits) {
        case 0:
            obj->dataLength = UART_LEN_5;
            break;
        case 1:
            obj->dataLength = UART_LEN_6;
            break;
        case 2:
            obj->dataLength = UART_LEN_7;
            break;
        case 3:
            obj->dataLength = UART_LEN_8;
            break;
        default:
            obj->dataLength = UART_LEN_8;
            break;
    }

    switch (stop_bits) {
        case 0:
            obj->stopBits = UART_STOP_ONE;
            break;
        case 1:
            obj->stopBits = UART_STOP_TWO;
            break;
        default:
            obj->stopBits = UART_STOP_ONE;
            break;
    }

    MAP_UARTConfigSetExpClk(obj->baseAddr, MAP_PRCMPeripheralClockGet(obj->peripheralId),
                            obj->baudRate, (dataLength[obj->dataLength] |
                                            stopBits[obj->stopBits] | parityType[obj->parityType]));
}

/******************************************************************************
 * INTERRUPTS HANDLING
 ******************************************************************************/
static inline void uart_irq(uint32_t intstatus, uint32_t index, CC3220SF_UART_TypeDef *puart)
{
    SerialIrq irq_type;
    if (intstatus & UART_INT_TX) {
        irq_type = TxIrq;
    } else {
        irq_type = RxIrq;
    }
    uint32_t rxErrors = puart->RSR & 0x0000000F;
    if (rxErrors) {
        puart->ECR = 0;
    }
    if ((RxIrq == irq_type) && (NC != uart_data[index].sw_rts.pin)) {
        gpio_write(&uart_data[index].sw_rts, 1);
        // Disable interrupt if it wasn't enabled by other part of the application
        if (!uart_data[index].rx_irq_set_api) {
            puart->IM &= ~(UART_INT_RX | UART_INT_RT);
        }
    }
    if (uart_data[index].serial_irq_id != 0) {
        if ((irq_type != RxIrq) || (uart_data[index].rx_irq_set_api)) {
            irq_handler(uart_data[index].serial_irq_id, irq_type);
        }
    }
    if (irq_type == TxIrq) {
        puart->ICR = UART_INT_TX; // clear TX interrupt
    } else {
        puart->ICR = UART_INT_RX; // clear RX interrupt
    }
}

void uart0_irq()
{
    uart_irq(CC3220SF_UART0->MIS, 0, (CC3220SF_UART_TypeDef *)CC3220SF_UART0);
}
void uart1_irq()
{
    uart_irq(CC3220SF_UART1->MIS, 1, (CC3220SF_UART_TypeDef *)CC3220SF_UART1);
}

void serial_irq_handler(serial_t *obj, uart_irq_handler handler, uint32_t id)
{
    irq_handler = handler;
    uart_data[obj->index].serial_irq_id = id;
}

void serial_irq_set_internal(serial_t *obj, SerialIrq irq, uint32_t enable)
{
    IRQn_Type irq_n = (IRQn_Type)0;
    uint32_t vector = 0;

    /* Clear interrupts */
    uint32_t status = MAP_UARTIntStatus(obj->baseAddr, true);
    MAP_UARTIntClear(obj->baseAddr, status);

    switch ((int)obj->uart) {
        case UART_0:
            irq_n = INT_UARTA0_IRQn;
            vector = (uint32_t)&uart0_irq;
            break;
        case UART_1:
            irq_n = INT_UARTA1_IRQn;
            vector = (uint32_t)&uart1_irq;
            break;
    }

    if (enable) {
        if (irq == TxIrq) {
            MAP_UARTIntEnable(obj->baseAddr, UART_INT_TX);
        } else {
            MAP_UARTIntEnable(obj->baseAddr, UART_INT_RX);
        }
        NVIC_SetVector(irq_n, vector);
        NVIC_EnableIRQ(irq_n);
    } else {
        /* Disable IRQ */
        MAP_UARTIntDisable(obj->baseAddr, UART_INT_TX | UART_INT_RX);
        NVIC_DisableIRQ(irq_n);
    }
}

void serial_irq_set(serial_t *obj, SerialIrq irq, uint32_t enable)
{
    if (RxIrq == irq) {
        uart_data[obj->index].rx_irq_set_api = enable;
    }
    serial_irq_set_internal(obj, irq, enable);
}

/*static void serial_flow_irq_set(serial_t *obj, uint32_t enable)
{
    uart_data[obj->index].rx_irq_set_flow = enable;
    serial_irq_set_internal(obj, RxIrq, enable);
}*/

/******************************************************************************
 * READ/WRITE
 ******************************************************************************/
int serial_getc(serial_t *obj)
{
    while (!serial_readable(obj));
    return obj->uart->DR;
}

void serial_putc(serial_t *obj, int c)
{
    while (!serial_writable(obj));
    obj->uart->DR = c;
}

int serial_readable(serial_t *obj)
{
    return ((obj->uart->FR & UART_FR_RXFE) ? 0 : 1);
}

int serial_writable(serial_t *obj)
{
    return ((obj->uart->FR & UART_FR_TXFF) ? 0 : 1);
}

void serial_clear(serial_t *obj)
{
    obj->uart->DR = 0x00;
}

void serial_pinout_tx(PinName tx)
{
    pinmap_pinout(tx, PinMap_UART_TX);
}

void serial_break_set(serial_t *obj)
{
    MAP_UARTBreakCtl(obj->baseAddr, true);
}

void serial_break_clear(serial_t *obj)
{
    MAP_UARTBreakCtl(obj->baseAddr, false);
}

void serial_set_flow_control(serial_t *obj, FlowControl type, PinName rxflow, PinName txflow)
{
    switch (type) {
        case FlowControlRTS:
            pinmap_pinout(rxflow, PinMap_UART_RTS);
            MAP_UARTFlowControlSet(obj->baseAddr, UART_FLOWCONTROL_RX);
            MAP_UARTModemControlSet(obj->baseAddr, UART_OUTPUT_RTS);
            break;

        case FlowControlCTS:
            pinmap_pinout(txflow, PinMap_UART_CTS);
            MAP_UARTFlowControlSet(obj->baseAddr, UART_FLOWCONTROL_TX);
            MAP_UARTModemControlClear(obj->baseAddr, UART_OUTPUT_RTS);
            break;

        case FlowControlRTSCTS:
            pinmap_pinout(rxflow, PinMap_UART_RTS);
            pinmap_pinout(txflow, PinMap_UART_CTS);
            MAP_UARTFlowControlSet(obj->baseAddr, UART_FLOWCONTROL_TX | UART_FLOWCONTROL_RX);
            MAP_UARTModemControlSet(obj->baseAddr, UART_OUTPUT_RTS);
            break;

        case FlowControlNone:
            MAP_UARTFlowControlSet(obj->baseAddr, UART_FLOWCONTROL_NONE);
            MAP_UARTModemControlClear(obj->baseAddr, UART_OUTPUT_RTS);
            break;
    }
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
