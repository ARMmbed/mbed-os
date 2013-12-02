/* mbed Microcontroller Library
 * Copyright (c) 2006-2013 ARM Limited
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
//==============================================================================
// STM32F103
//==============================================================================  
#include "serial_api.h"
#include "cmsis.h"
#include "pinmap.h"
#include "error.h"
#include <string.h>

/******************************************************************************
 * INITIALIZATION
 ******************************************************************************/

static const PinMap PinMap_UART_TX[] = {
    {PA_9,  UART_1, STM_PIN_DATA(GPIO_Mode_AF_PP, 0)},
    {PA_2,  UART_2, STM_PIN_DATA(GPIO_Mode_AF_PP, 0)},
    {NC,    NC,     0}
};

static const PinMap PinMap_UART_RX[] = {
    {PA_10, UART_1, STM_PIN_DATA(GPIO_Mode_IN_FLOATING, 0)},
    {PA_3,  UART_2, STM_PIN_DATA(GPIO_Mode_IN_FLOATING, 0)},
    {NC,    NC,     0}
};

#define UART_NUM (2)

static uint32_t serial_irq_ids[UART_NUM] = {0};

static uart_irq_handler irq_handler;

int stdio_uart_inited = 0;
serial_t stdio_uart;

void serial_init(serial_t *obj, PinName tx, PinName rx) {
    
    USART_TypeDef *usart;
    USART_InitTypeDef USART_InitStructure;
  
    // Determine the UART to use (UART_1, UART_2, ...)
    UARTName uart_tx = (UARTName)pinmap_peripheral(tx, PinMap_UART_TX);
    UARTName uart_rx = (UARTName)pinmap_peripheral(rx, PinMap_UART_RX);
  
    // Get the peripheral name (UART_1, UART_2, ...) from the pin and assign it to the object
    obj->uart = (UARTName)pinmap_merge(uart_tx, uart_rx);

    if (obj->uart == (UARTName)NC) {
        error("Serial pinout mapping failed");
    }
    
    // Get UART registers structure address
    usart = (USART_TypeDef *)(obj->uart);
  
    // Enable USART clock
    if (obj->uart == UART_1) {
        RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE); 
    }
    if (obj->uart == UART_2) {
        RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE); 
    }
            
    // Configure the UART pins
    pinmap_pinout(tx, PinMap_UART_TX);
    pinmap_pinout(rx, PinMap_UART_RX);

    // Configure UART
    obj->baudrate = 9600;
    obj->databits = USART_WordLength_8b;
    obj->stopbits = USART_StopBits_1;
    obj->parity = USART_Parity_No;    

    USART_InitStructure.USART_BaudRate = obj->baudrate;
    USART_InitStructure.USART_WordLength = obj->databits;
    USART_InitStructure.USART_StopBits = obj->stopbits;
    USART_InitStructure.USART_Parity = obj->parity;
    USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
    USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
    USART_Init(usart, &USART_InitStructure);
    
    USART_Cmd(usart, ENABLE);

    // The index is used by irq
    if (obj->uart == UART_1) obj->index = 0;
    if (obj->uart == UART_2) obj->index = 1;
    
    // For stdio management
    if (obj->uart == STDIO_UART) {
        stdio_uart_inited = 1;
        memcpy(&stdio_uart, obj, sizeof(serial_t));
    }
    
}

void serial_free(serial_t *obj) {
    serial_irq_ids[obj->index] = 0;
}

void serial_baud(serial_t *obj, int baudrate) {
    USART_TypeDef *usart = (USART_TypeDef *)(obj->uart);
    USART_InitTypeDef USART_InitStructure;
    
    // Save new value
    obj->baudrate = baudrate;

    USART_Cmd(usart, DISABLE);

    USART_InitStructure.USART_BaudRate = obj->baudrate;
    USART_InitStructure.USART_WordLength = obj->databits;
    USART_InitStructure.USART_StopBits = obj->stopbits;
    USART_InitStructure.USART_Parity = obj->parity;
    USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
    USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
    USART_Init(usart, &USART_InitStructure);
    
    USART_Cmd(usart, ENABLE);
}

void serial_format(serial_t *obj, int data_bits, SerialParity parity, int stop_bits) {
    USART_TypeDef *usart = (USART_TypeDef *)(obj->uart);
    USART_InitTypeDef USART_InitStructure;

    // Save new values
  
    if (data_bits == 8) {
        obj->databits = USART_WordLength_8b;
    }
    else {
        obj->databits = USART_WordLength_9b;
    }

    switch (parity) {
      case ParityOdd:
      case ParityForced0:
          obj->parity = USART_Parity_Odd;
      break;
      case ParityEven:
      case ParityForced1:        
          obj->parity = USART_Parity_Even;
      break;
      default: // ParityNone
          obj->parity = USART_Parity_No;
      break;
    }
    
    if (stop_bits == 2) {
        obj->stopbits = USART_StopBits_2;
    }
    else {
        obj->stopbits = USART_StopBits_1;
    }

    USART_Cmd(usart, DISABLE);
      
    USART_InitStructure.USART_BaudRate = obj->baudrate;
    USART_InitStructure.USART_WordLength = obj->databits;
    USART_InitStructure.USART_StopBits = obj->stopbits;
    USART_InitStructure.USART_Parity = obj->parity;
    USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
    USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
    USART_Init(usart, &USART_InitStructure);
    
    USART_Cmd(usart, ENABLE);
}

/******************************************************************************
 * INTERRUPTS HANDLING
 ******************************************************************************/

// not api
void uart1_irq(void) {
    USART_TypeDef *usart = (USART_TypeDef *)UART_1;  
    if (serial_irq_ids[0] != 0) {
        if (USART_GetITStatus(usart, USART_IT_TXE) != RESET) {
            irq_handler(serial_irq_ids[0], TxIrq);
        }
        if (USART_GetITStatus(usart, USART_IT_RXNE) != RESET) {
            irq_handler(serial_irq_ids[0], RxIrq);
        }
    }
}

// not api
void uart2_irq(void) {
    USART_TypeDef *usart = (USART_TypeDef *)UART_2;  
    if (serial_irq_ids[1] != 0) {
        if (USART_GetITStatus(usart, USART_IT_TXE) != RESET) {
            irq_handler(serial_irq_ids[1], TxIrq);
        }
        if (USART_GetITStatus(usart, USART_IT_RXNE) != RESET) {
            irq_handler(serial_irq_ids[1], RxIrq);
        }
    }
}

void serial_irq_handler(serial_t *obj, uart_irq_handler handler, uint32_t id) {
    irq_handler = handler;
    serial_irq_ids[obj->index] = id;
}

void serial_irq_set(serial_t *obj, SerialIrq irq, uint32_t enable) {
    IRQn_Type irq_n = (IRQn_Type)0;
    uint32_t vector = 0;
    USART_TypeDef *usart = (USART_TypeDef *)(obj->uart);

    if (obj->uart == UART_1) {
      irq_n = USART1_IRQn;
      vector = (uint32_t)&uart1_irq;
    }
  
    if (obj->uart == UART_2) {
      irq_n = USART2_IRQn;
      vector = (uint32_t)&uart2_irq;
    }
    
    if (enable) {
      
        if (irq == RxIrq) {
            USART_ITConfig(usart, USART_IT_RXNE, ENABLE);
        }
        else { // TxIrq
            USART_ITConfig(usart, USART_IT_TXE, ENABLE);
        }        
        
        NVIC_SetVector(irq_n, vector);
        NVIC_EnableIRQ(irq_n);
        
    } else { // disable
      
        int all_disabled = 0;
        
        if (irq == RxIrq) {
            USART_ITConfig(usart, USART_IT_RXNE, DISABLE);
            // Check if TxIrq is disabled too
            if ((usart->CR1 & USART_CR1_TXEIE) == 0) all_disabled = 1;
        }
        else { // TxIrq
            USART_ITConfig(usart, USART_IT_TXE, DISABLE);
            // Check if RxIrq is disabled too
            if ((usart->CR1 & USART_CR1_RXNEIE) == 0) all_disabled = 1;          
        }
        
        if (all_disabled) NVIC_DisableIRQ(irq_n);
        
    }    
}

/******************************************************************************
 * READ/WRITE
 ******************************************************************************/

int serial_getc(serial_t *obj) {
    USART_TypeDef *usart = (USART_TypeDef *)(obj->uart);
    while (!serial_readable(obj));
    return (int)(USART_ReceiveData(usart));
}

void serial_putc(serial_t *obj, int c) {
    USART_TypeDef *usart = (USART_TypeDef *)(obj->uart);
    while (!serial_writable(obj));
    USART_SendData(usart, (uint16_t)c);
}

int serial_readable(serial_t *obj) {
    int status;
    USART_TypeDef *usart = (USART_TypeDef *)(obj->uart);
    // Check if data is received
    status = ((USART_GetFlagStatus(usart, USART_FLAG_RXNE) != RESET) ? 1 : 0);
    return status;
}

int serial_writable(serial_t *obj) {
    int status;
    USART_TypeDef *usart = (USART_TypeDef *)(obj->uart);
    // Check if data is transmitted
    status = ((USART_GetFlagStatus(usart, USART_FLAG_TXE) != RESET) ? 1 : 0);
    return status;
}

void serial_clear(serial_t *obj) {
    USART_TypeDef *usart = (USART_TypeDef *)(obj->uart);
    USART_ClearFlag(usart, USART_FLAG_TXE);
    USART_ClearFlag(usart, USART_FLAG_RXNE);
}

void serial_pinout_tx(PinName tx) {
    pinmap_pinout(tx, PinMap_UART_TX);
}

void serial_break_set(serial_t *obj) {
    USART_TypeDef *usart = (USART_TypeDef *)(obj->uart);
    USART_SendBreak(usart);
}

void serial_break_clear(serial_t *obj) {
}
