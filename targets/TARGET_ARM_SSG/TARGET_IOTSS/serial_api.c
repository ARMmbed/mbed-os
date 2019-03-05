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
// math.h required for floating point operations for baud rate calculation
#include <math.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "serial_api.h"
#include "cmsis.h"
#include "pinmap.h"
#include "mbed_error.h"
#include "gpio_api.h"

/******************************************************************************
 * INITIALIZATION
 ******************************************************************************/

static const PinMap PinMap_UART_TX[] = {
    {USBTX     , UART_0, 0},
    {XB_TX   , UART_1, 0},
    {SH0_TX  , UART_2, 0},
    {SH1_TX  , UART_3, 0},
    {NC      , NC    , 0}
};

static const PinMap PinMap_UART_RX[] = {
    {USBRX     , UART_0, 0},
    {XB_RX   , UART_1, 0},
    {SH0_RX  , UART_2, 0},
    {SH1_RX  , UART_3, 0},
    {NC      , NC    , 0}
};

#define UART_NUM    4

static uart_irq_handler irq_handler;

int stdio_uart_inited = 0;
serial_t stdio_uart;

struct serial_global_data_s {
    uint32_t serial_irq_id;
    gpio_t sw_rts, sw_cts;
    uint8_t count, rx_irq_set_flow, rx_irq_set_api;
};

static struct serial_global_data_s uart_data[UART_NUM];

void serial_init(serial_t *obj, PinName tx, PinName rx) {
    int is_stdio_uart = 0;
    
    // determine the UART to use
    UARTName uart_tx = (UARTName)pinmap_peripheral(tx, PinMap_UART_TX);
    UARTName uart_rx = (UARTName)pinmap_peripheral(rx, PinMap_UART_RX);
    UARTName uart = (UARTName)pinmap_merge(uart_tx, uart_rx);
    if ((int)uart == NC) {
        error("Serial pinout mapping failed");
    }
    
    obj->uart = (CMSDK_UART_TypeDef *)uart;
    //set baud rate and enable Uart in normarl mode (RX and TX enabled)
    switch (uart)
    {
        case UART_0:     //UART HEADER
        {
          CMSDK_UART1->CTRL = 0x0;    // Disable UART when changing configuration
          if((int)tx != NC)
          {
            CMSDK_UART1->CTRL |= 0x01;   // TX enable 
          } else {
            CMSDK_UART1->CTRL &= 0xFFFE;  // TX disable
          }
          if((int)rx != NC)
          {
            CMSDK_UART1->CTRL |= 0x02;  // RX enable
          } else {
            CMSDK_UART1->CTRL &= 0xFFFD;  // RX disable
          } 
        }
        break;
        case UART_1:     //XBEE SOCKET UART
        {
          CMSDK_UART3->CTRL = 0x00;    // Disable UART when changing configuration
          if((int)tx != NC)
          {
            CMSDK_UART3->CTRL = 0x1;   // TX enable 
            CMSDK_GPIO4->ALTFUNCSET |= 0x0100;
          }
          if((int)rx != NC)
          {
            CMSDK_UART3->CTRL |= 0x2;  // RX enable 
            CMSDK_GPIO4->ALTFUNCSET |= 0x0080;
          }
        }
        break;
        case UART_2:     //Sheild0 UART
        {
          CMSDK_UART0->CTRL = 0x00;    // Disable UART when changing configuration
          if((int)uart_tx != NC)
          {
            CMSDK_UART0->CTRL |= 0x01;  // TX enable 
                        CMSDK_GPIO0->ALTFUNCSET |= 0x0002;
          }
          
          if((int)uart_rx != NC)
          {
                        CMSDK_UART0->CTRL |= 0x02;  // RX enable  
                        CMSDK_GPIO0->ALTFUNCSET |= 0x0001;
          }
        }
        break;
        case UART_3:     //Sheild1 UART
        {     
          CMSDK_UART2->CTRL = 0x00;    // Disable UART when changing configuration
          if((int)tx != NC)
          {
            CMSDK_UART2->CTRL = 0x1;   // TX enable 
                        CMSDK_GPIO1->ALTFUNCSET |= 0x0002;
          }
          if((int)rx != NC)
          {
            CMSDK_UART2->CTRL |= 0x2;  // RX enable 
                        CMSDK_GPIO1->ALTFUNCSET |= 0x0001;
          }
        } 
        break;
    }

    // set default baud rate and format
    serial_baud  (obj, 9600);
    
    // pinout the chosen uart
    pinmap_pinout(tx, PinMap_UART_TX);
    pinmap_pinout(rx, PinMap_UART_RX);
    
    switch (uart) {
        case UART_0: obj->index = 0; break;
        case UART_1: obj->index = 1; break;
        case UART_2: obj->index = 2; break;
        case UART_3: obj->index = 3; break;
    }
    uart_data[obj->index].sw_rts.pin = NC;
    uart_data[obj->index].sw_cts.pin = NC;
    serial_set_flow_control(obj, FlowControlNone, NC, NC);
    
    is_stdio_uart = (uart == STDIO_UART) ? (1) : (0);
    
    if (is_stdio_uart) {
        stdio_uart_inited = 1;
        memcpy(&stdio_uart, obj, sizeof(serial_t));
    }
}

void serial_free(serial_t *obj) {
      uart_data[obj->index].serial_irq_id = 0;
}

// serial_baud
// set the baud rate, taking in to account the current SystemFrequency
void serial_baud(serial_t *obj, int baudrate) {
    // The MPS2 has a simple divider to control the baud rate. The formula is:
    //
    // Baudrate = PCLK / BAUDDIV
        //
        // PCLK = 24 Mhz
        // so for a desired baud rate of 9600 
        //  24000000 / 9600 = 2500
    //
    //check to see if minimum baud value entered
    int baudrate_div = 0;
    baudrate_div = SystemCoreClock / baudrate;
    if(baudrate >= 16){
    switch ((int)obj->uart) {
        case UART_0: CMSDK_UART1->BAUDDIV = baudrate_div; break;
        case UART_1: CMSDK_UART3->BAUDDIV = baudrate_div; break;
        case UART_2: CMSDK_UART0->BAUDDIV = baudrate_div; break;
        case UART_3: CMSDK_UART2->BAUDDIV = baudrate_div; break;
        default: error("serial_baud"); break;
    }
    } else {
        error("serial_baud");
    }

}

void serial_format(serial_t *obj, int data_bits, SerialParity parity, int stop_bits) {
}

/******************************************************************************
 * INTERRUPTS HANDLING
 ******************************************************************************/
static inline void uart_irq(uint32_t intstatus, uint32_t index, CMSDK_UART_TypeDef *puart) {
    SerialIrq irq_type;
    switch (intstatus)
    {
        case 1:
        {
            irq_type = TxIrq;
        }
        break;

        case 2:
        {
            irq_type = RxIrq;
        }
        break;

        default: return;
    }   /* End of Switch */

    if ((RxIrq == irq_type) && (NC != uart_data[index].sw_rts.pin)) 
    {
        gpio_write(&uart_data[index].sw_rts, 1);
        // Disable interrupt if it wasn't enabled by other part of the application
        if (!uart_data[index].rx_irq_set_api)
        {
            /* Disable Rx interrupt */
            puart->CTRL &= ~(CMSDK_UART_CTRL_RXIRQEN_Msk);
        }
    }

    if (uart_data[index].serial_irq_id != 0)
    {
        if ((irq_type != RxIrq) || (uart_data[index].rx_irq_set_api))
        {
            irq_handler(uart_data[index].serial_irq_id, irq_type);
        }
    }

    if( irq_type == TxIrq )
    {
        /* Clear the TX interrupt Flag */
        puart->INTCLEAR |= 0x01;
    }
    else
    {
        /* Clear the Rx interupt Flag */
        puart->INTCLEAR |= 0x02;
    }
}

void uart0_irq() {uart_irq(CMSDK_UART1->INTSTATUS & 0x3, 0, (CMSDK_UART_TypeDef*)CMSDK_UART1);}
void uart1_irq() {uart_irq(CMSDK_UART3->INTSTATUS & 0x3, 1, (CMSDK_UART_TypeDef*)CMSDK_UART3);}
void uart2_irq() {uart_irq(CMSDK_UART0->INTSTATUS & 0x3, 2, (CMSDK_UART_TypeDef*)CMSDK_UART0);}
void uart3_irq() {uart_irq(CMSDK_UART2->INTSTATUS & 0x3, 3, (CMSDK_UART_TypeDef*)CMSDK_UART2);}

void serial_irq_handler(serial_t *obj, uart_irq_handler handler, uint32_t id) {
    irq_handler = handler;
    uart_data[obj->index].serial_irq_id = id;
}

static void serial_irq_set_internal(serial_t *obj, SerialIrq irq, uint32_t enable) {
    IRQn_Type irq_n = (IRQn_Type)0;
    uint32_t vector = 0;
    switch ((int)obj->uart)
    {
        case UART_0:
            irq_n = UART0_IRQn;
            vector = (uint32_t)&uart0_irq;
        break;
        case UART_1:
            irq_n = UART1_IRQn;
            vector = (uint32_t)&uart1_irq;
        break;
        case UART_2:
        {
            irq_n = UART2_IRQn;
            vector  = (uint32_t)&uart2_irq;
        }
        break;
        case UART_3:
        {
            irq_n = UART3_IRQn;
            vector  = (uint32_t)&uart3_irq;
        }
        break;
    }

    if (enable)
    {
        if (irq == TxIrq)
        {
            /* set TX interrupt enable in CTRL REG */
            obj->uart->CTRL |= CMSDK_UART_CTRL_TXIRQEN_Msk;
        }
        else
        {
            /* set Rx interrupt on in CTRL REG */
            obj->uart->CTRL |= CMSDK_UART_CTRL_RXIRQEN_Msk;
        }
        NVIC_SetVector(irq_n, vector);
        NVIC_EnableIRQ(irq_n);
    }
    else if ((irq == TxIrq) || (uart_data[obj->index].rx_irq_set_api + uart_data[obj->index].rx_irq_set_flow == 0))
    {   /*      Disable   IRQ   */
        int all_disabled = 0;
        SerialIrq other_irq = (irq == RxIrq) ? (TxIrq) : (RxIrq);
        
        obj->uart->CTRL &= ~(1 << (irq + 2));

        all_disabled = (obj->uart->CTRL & (1 << (other_irq + 2))) == 0;
        
        if (all_disabled)
        {
            NVIC_DisableIRQ(irq_n);   
        }
    }
}

void serial_irq_set(serial_t *obj, SerialIrq irq, uint32_t enable) {
    if (RxIrq == irq)
        uart_data[obj->index].rx_irq_set_api = enable;
    serial_irq_set_internal(obj, irq, enable);
}

/******************************************************************************
 * READ/WRITE
 ******************************************************************************/
int serial_getc(serial_t *obj) {
    while (serial_readable(obj) == 0);
    int data = obj->uart->DATA;
    return data;
}

void serial_putc(serial_t *obj, int c) {
    while (serial_writable(obj));
    obj->uart->DATA = c;
}

int serial_readable(serial_t *obj) {
    return obj->uart->STATE & 0x2;
}

int serial_writable(serial_t *obj) {
    return obj->uart->STATE & 0x1;
}

void serial_clear(serial_t *obj) {
    obj->uart->DATA = 0x00;
}

void serial_pinout_tx(PinName tx) {
    pinmap_pinout(tx, PinMap_UART_TX);
}

void serial_break_set(serial_t *obj) {
}

void serial_break_clear(serial_t *obj) {
}
void serial_set_flow_control(serial_t *obj, FlowControl type, PinName rxflow, PinName txflow) {
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

