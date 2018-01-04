/* mbed Microcontroller Library
 *******************************************************************************
 * Copyright (c) 2014, STMicroelectronics
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
#ifdef YOTTA_CFG_MBED_OS
#include "target_config.h"
#include "uvisor-lib/uvisor-lib.h"
#include "mbed-drivers/mbed_assert.h"
#else
#include "mbed_assert.h"
#endif
#include "serial_api.h"

#if DEVICE_SERIAL

#include "cmsis.h"
#include "pinmap.h"
#include <string.h>
#include "PeripheralPins.h"
#include "wd_logging.h"
#ifdef YOTTA_CFG_MBED_OS
#include "mbed-drivers/mbed_error.h"
#else
#include "mbed_error.h"
#endif

#define DEBUG_STDIO 0

#ifndef DEBUG_STDIO
#   define DEBUG_STDIO 0
#endif

#if DEBUG_STDIO
#   include <stdio.h>
#   define DEBUG_PRINTF(...) do { printf(__VA_ARGS__); } while(0)
#else
#   define DEBUG_PRINTF(...) {}
#endif

#define UART_NUM (8)
#define UART_STATE_RX_ACTIVE 0x20
#define UART_STATE_TX_ACTIVE 0x10

#if DEVICE_SERIAL_ASYNCH_DMA

//#ifndef USE_STM32F4XX_HAL_DMA__FIX
//#error Please use stm32f4xx_hal_dma__FIX.c
//#endif

static const uint32_t DMA_UartRx_Channel[UART_NUM] = {DMA_CHANNEL_4, DMA_CHANNEL_4, DMA_CHANNEL_4, DMA_CHANNEL_4, \
                                                      DMA_CHANNEL_4, DMA_CHANNEL_5, DMA_CHANNEL_5, DMA_CHANNEL_5};
DMA_Stream_TypeDef *DMA_UartRx_Stream[UART_NUM] = {
  DMA2_Stream5, DMA1_Stream5, DMA1_Stream1, \
  DMA1_Stream2, DMA1_Stream0, DMA2_Stream1, \
  DMA1_Stream3, DMA1_Stream6
};
static const uint32_t DMA_UartTx_Channel[UART_NUM] = {DMA_CHANNEL_4, DMA_CHANNEL_4, DMA_CHANNEL_4, DMA_CHANNEL_4, \
                                                      DMA_CHANNEL_4, DMA_CHANNEL_5, DMA_CHANNEL_5, DMA_CHANNEL_5};
DMA_Stream_TypeDef *DMA_UartTx_Stream[UART_NUM] = {
    DMA2_Stream7, DMA1_Stream6, DMA1_Stream3, \
    DMA1_Stream4, DMA1_Stream7, DMA2_Stream6,\
    DMA1_Stream1, DMA1_Stream0
};
DMA_HandleTypeDef DmaHandle;

DMA_HandleTypeDef DmaTxHandle[UART_NUM];
DMA_HandleTypeDef DmaRxHandle[UART_NUM];
#endif
// DEVICE_SERIAL_ASYNCH_DMA

uint32_t serial_irq_ids[UART_NUM] = {0, 0, 0, 0, 0, 0, 0, 0};
static uart_irq_handler irq_handler;

UART_HandleTypeDef UartHandle[UART_NUM];

int stdio_uart_inited = 0;
serial_t stdio_uart;

#if DEVICE_SERIAL_ASYNCH
#define SERIAL_OBJ(X) (obj->serial.X)
#else
#define SERIAL_OBJ(X) (obj->X)
#endif

#if DEVICE_SERIAL_ASYNCH_DMA
static void init_dma(serial_t *obj)
{
    UART_HandleTypeDef *serial_handle = &UartHandle[SERIAL_OBJ(index)];
    
    if (SERIAL_OBJ(pin_tx) != NC) {
        // set DMA in the UartHandle
        DMA_HandleTypeDef *hdma_tx = &DmaTxHandle[SERIAL_OBJ(index)];
        /* Configure the DMA handler for Transmission process */
        hdma_tx->Instance                 = (DMA_Stream_TypeDef *)DMA_UartTx_Stream[SERIAL_OBJ(index)];
        hdma_tx->Init.Channel             = DMA_UartTx_Channel[SERIAL_OBJ(index)];
        hdma_tx->Init.Direction           = DMA_MEMORY_TO_PERIPH;
        hdma_tx->Init.PeriphInc           = DMA_PINC_DISABLE;
        hdma_tx->Init.MemInc              = DMA_MINC_ENABLE;
        hdma_tx->Init.PeriphDataAlignment = DMA_PDATAALIGN_BYTE;
        hdma_tx->Init.MemDataAlignment    = DMA_MDATAALIGN_BYTE;
        hdma_tx->Init.Mode                = DMA_NORMAL;
	    hdma_tx->Init.Priority            = DMA_PRIORITY_LOW;
        hdma_tx->Init.FIFOMode            = DMA_FIFOMODE_DISABLE;
        hdma_tx->Init.FIFOThreshold       = DMA_FIFO_THRESHOLD_HALFFULL;
        hdma_tx->Init.MemBurst            = DMA_MBURST_INC4;
        hdma_tx->Init.PeriphBurst         = DMA_PBURST_INC4;

        HAL_DMA_Init(hdma_tx);

        /* Associate the initialized DMA handle to the UART handle */
        serial_handle->hdmatx = hdma_tx;
        hdma_tx->Parent = serial_handle;
    }
    
    if (SERIAL_OBJ(pin_rx) != NC) {
        /* Configure the DMA handler for reception process */
        DMA_HandleTypeDef *hdma_rx = &DmaRxHandle[SERIAL_OBJ(index)];
        hdma_rx->Instance                 = (DMA_Stream_TypeDef *)DMA_UartRx_Stream[SERIAL_OBJ(index)];
        hdma_rx->Init.Channel             = DMA_UartRx_Channel[SERIAL_OBJ(index)];
        hdma_rx->Init.Direction           = DMA_PERIPH_TO_MEMORY;
        hdma_rx->Init.PeriphInc           = DMA_PINC_DISABLE;
        hdma_rx->Init.MemInc              = DMA_MINC_ENABLE;
        hdma_rx->Init.PeriphDataAlignment = DMA_PDATAALIGN_BYTE;
        hdma_rx->Init.MemDataAlignment    = DMA_MDATAALIGN_BYTE;
        hdma_rx->Init.Mode                = DMA_CIRCULAR;
        hdma_rx->Init.Priority            = DMA_PRIORITY_HIGH;
        hdma_rx->Init.FIFOMode            = DMA_FIFOMODE_DISABLE;
        hdma_rx->Init.FIFOThreshold       = DMA_FIFO_THRESHOLD_HALFFULL;
        hdma_rx->Init.MemBurst            = DMA_MBURST_SINGLE;
        hdma_rx->Init.PeriphBurst         = DMA_PBURST_SINGLE;

        HAL_DMA_Init(hdma_rx);

        /* Associate the initialized DMA handle to the UART handle */
        serial_handle->hdmarx = hdma_rx;
        hdma_rx->Parent = serial_handle;
    }
}
#endif
// DEVICE_SERIAL_ASYNCH_DMA

void serial_init(serial_t *obj, PinName tx, PinName rx)
{
    // Determine the UART to use (UART_1, UART_2, ...)
    UARTName uart_tx = (UARTName)pinmap_peripheral(tx, PinMap_UART_TX);
    UARTName uart_rx = (UARTName)pinmap_peripheral(rx, PinMap_UART_RX);

    // Get the peripheral name (UART_1, UART_2, ...) from the pin and assign it to the object
    UARTName instance = (UARTName)pinmap_merge(uart_tx, uart_rx);
    MBED_ASSERT(instance != (UARTName)NC);

    // Enable USART clock
    switch (instance) {
        case UART_1:
            __HAL_RCC_USART1_CLK_ENABLE();
            SERIAL_OBJ(index) = 0;
#if DEVICE_SERIAL_ASYNCH_DMA
            __HAL_RCC_DMA2_CLK_ENABLE();
#endif
            break;
#if defined(USART2_BASE)
        case UART_2:
            __HAL_RCC_USART2_CLK_ENABLE();
            SERIAL_OBJ(index) = 1;
#if DEVICE_SERIAL_ASYNCH_DMA
            __HAL_RCC_DMA1_CLK_ENABLE();
#endif
            break;
#endif
#if defined(USART3_BASE)
        case UART_3:
            __HAL_RCC_USART3_CLK_ENABLE();
            SERIAL_OBJ(index) = 2;
#if DEVICE_SERIAL_ASYNCH_DMA
            __HAL_RCC_DMA1_CLK_ENABLE();
#endif
            break;
#endif
#if defined(UART4_BASE)
        case UART_4:
            __HAL_RCC_UART4_CLK_ENABLE();
            SERIAL_OBJ(index) = 3;
#if DEVICE_SERIAL_ASYNCH_DMA
            __HAL_RCC_DMA1_CLK_ENABLE();
#endif
            break;
#endif
#if defined(UART5_BASE)
        case UART_5:
            __HAL_RCC_UART5_CLK_ENABLE();
            SERIAL_OBJ(index) = 4;
#if DEVICE_SERIAL_ASYNCH_DMA
            __HAL_RCC_DMA1_CLK_ENABLE();
#endif
            break;
#endif
#if defined(USART6_BASE)
        case UART_6:
            __HAL_RCC_USART6_CLK_ENABLE();
            SERIAL_OBJ(index) = 5;
#if DEVICE_SERIAL_ASYNCH_DMA
            __HAL_RCC_DMA2_CLK_ENABLE();
#endif
            break;
#endif
#if defined(UART7_BASE)
        case UART_7:
            __HAL_RCC_UART7_CLK_ENABLE();
            SERIAL_OBJ(index) = 6;
#if DEVICE_SERIAL_ASYNCH_DMA
            __HAL_RCC_DMA1_CLK_ENABLE();
#endif
            break;
#endif
#if defined(UART8_BASE)
        case UART_8:
            __HAL_RCC_UART8_CLK_ENABLE();
            SERIAL_OBJ(index) = 7;
#if DEVICE_SERIAL_ASYNCH_DMA
            __HAL_RCC_DMA1_CLK_ENABLE();
#endif
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
    SERIAL_OBJ(pin_tx) = tx;
    SERIAL_OBJ(pin_rx) = rx;

    // Configure UART
    UART_HandleTypeDef *handle = &UartHandle[SERIAL_OBJ(index)];
    handle->Instance          = (USART_TypeDef *)instance;
    handle->Init.BaudRate     = MBED_CONF_PLATFORM_DEFAULT_SERIAL_BAUD_RATE;
    handle->Init.WordLength   = UART_WORDLENGTH_8B;
    handle->Init.StopBits     = UART_STOPBITS_1;
    handle->Init.Parity       = UART_PARITY_NONE;
    if (rx == NC) {
      handle->Init.Mode = UART_MODE_TX;
    } else if (tx == NC) {
      handle->Init.Mode = UART_MODE_RX;
    } else {
      handle->Init.Mode = UART_MODE_TX_RX;
    }
    handle->Init.HwFlowCtl    = UART_HWCONTROL_NONE;
    handle->Init.OverSampling = UART_OVERSAMPLING_16;
    handle->TxXferCount       = 0;
    handle->TxXferSize        = 0;
    handle->RxXferCount       = 0;
    handle->RxXferSize        = 0;

#ifdef YOTTA_CFG_MBED_OS
    if (tx == STDIO_UART_TX && rx == STDIO_UART_RX) {
        handle->Init.BaudRate = YOTTA_CFG_MBED_OS_STDIO_DEFAULT_BAUD;
    }
#endif

    if (HAL_UART_Init(handle) != HAL_OK) {
        error("Cannot initialize UART%u\n", SERIAL_OBJ(index+1));
    }

#if DEVICE_SERIAL_ASYNCH_DMA
    init_dma(obj);
#endif

#ifndef YOTTA_CFG_MBED_OS // For mbed Classic only
    // For stdio management
    if ((int)(UartHandle[SERIAL_OBJ(index)].Instance) == STDIO_UART) {
        stdio_uart_inited = 1;
        memcpy(&stdio_uart, obj, sizeof(serial_t));
    }
#endif
    
    DEBUG_PRINTF("UART%u: Init\n", SERIAL_OBJ(index+1));
}

void serial_free(serial_t *obj)
{
    // Reset UART and disable clock
    switch (SERIAL_OBJ(index)) {
        case 0:
            __USART1_FORCE_RESET();
            __USART1_RELEASE_RESET();
            __USART1_CLK_DISABLE();
            break;
#if defined(USART2_BASE)
        case 1:
            __USART2_FORCE_RESET();
            __USART2_RELEASE_RESET();
            __USART2_CLK_DISABLE();
#if DEVICE_SERIAL_ASYNCH_DMA
            __HAL_RCC_DMA1_CLK_DISABLE();
#endif
            break;
#endif
#if defined(USART3_BASE)
        case 2:
            __USART3_FORCE_RESET();
            __USART3_RELEASE_RESET();
            __USART3_CLK_DISABLE();
            break;
#endif
#if defined(UART4_BASE)
        case 3:
            __UART4_FORCE_RESET();
            __UART4_RELEASE_RESET();
            __UART4_CLK_DISABLE();
#if DEVICE_SERIAL_ASYNCH_DMA
            __HAL_RCC_DMA1_CLK_DISABLE();
#endif
            break;
#endif
#if defined(UART5_BASE)
        case 4:
            __UART5_FORCE_RESET();
            __UART5_RELEASE_RESET();
            __UART5_CLK_DISABLE();
            break;
#endif
#if defined(USART6_BASE)
        case 5:
            __USART6_FORCE_RESET();
            __USART6_RELEASE_RESET();
            __USART6_CLK_DISABLE();
            break;
#endif
#if defined(UART7_BASE)
        case 6:
            __UART7_FORCE_RESET();
            __UART7_RELEASE_RESET();
            __UART7_CLK_DISABLE();
            break;
#endif
#if defined(UART8_BASE)
        case 7:
            __UART8_FORCE_RESET();
            __UART8_RELEASE_RESET();
            __UART8_CLK_DISABLE();
            break;
#endif
    }
    
    // Configure GPIOs
    pin_function(SERIAL_OBJ(pin_tx), STM_PIN_DATA(STM_MODE_INPUT, GPIO_NOPULL, 0));
    pin_function(SERIAL_OBJ(pin_rx), STM_PIN_DATA(STM_MODE_INPUT, GPIO_NOPULL, 0));

    serial_irq_ids[SERIAL_OBJ(index)] = 0;

    DEBUG_PRINTF("UART%u: Free\n", SERIAL_OBJ(index+1));
}

void serial_baud(serial_t *obj, int baudrate)
{
    UART_HandleTypeDef *handle = &UartHandle[SERIAL_OBJ(index)];
    
    handle->Init.BaudRate = baudrate;
    
    if (HAL_UART_Init(handle) != HAL_OK) {
           error("Cannot set UART%u baudrate\n", SERIAL_OBJ(index+1));
    }
    
    DEBUG_PRINTF("UART%u: Baudrate: %u\n", SERIAL_OBJ(index+1), baudrate);
}

void serial_format(serial_t *obj, int data_bits, SerialParity parity, int stop_bits)
{
    UART_HandleTypeDef *handle = &UartHandle[SERIAL_OBJ(index)];
  
    if (data_bits == 9) {
        handle->Init.WordLength = UART_WORDLENGTH_9B;
    } else {
        handle->Init.WordLength = UART_WORDLENGTH_8B;
    }

    switch (parity) {
        case ParityOdd:
            handle->Init.Parity = UART_PARITY_ODD;
            break;
        case ParityEven:
            handle->Init.Parity = UART_PARITY_EVEN;
            break;
        default: // ParityNone
        case ParityForced0: // unsupported!
        case ParityForced1: // unsupported!
            handle->Init.Parity = UART_PARITY_NONE;
            break;
    }

    if (stop_bits == 2) {
        handle->Init.StopBits = UART_STOPBITS_2;
    } else {
        handle->Init.StopBits = UART_STOPBITS_1;
    }

    if (HAL_UART_Init(handle) != HAL_OK) {
           error("Cannot format UART%u\n", SERIAL_OBJ(index+1));
    }

    DEBUG_PRINTF("UART%u: Format: %u, %u, %u\n", SERIAL_OBJ(index+1), data_bits, parity, stop_bits);
}

/******************************************************************************
 * INTERRUPTS HANDLING
 ******************************************************************************/

static void uart_irq(int id)
{
  UART_HandleTypeDef *handle = &UartHandle[id];
    if (serial_irq_ids[id] != 0) {
        if (__HAL_UART_GET_FLAG(handle, UART_FLAG_TC) != RESET) {
            irq_handler(serial_irq_ids[id], TxIrq);
            __HAL_UART_CLEAR_FLAG(handle, UART_FLAG_TC);
        }
        if (__HAL_UART_GET_FLAG(handle, UART_FLAG_RXNE) != RESET) {
            irq_handler(serial_irq_ids[id], RxIrq);
            __HAL_UART_CLEAR_FLAG(handle, UART_FLAG_RXNE);
        }
		if (__HAL_UART_GET_FLAG(handle, UART_FLAG_IDLE) != RESET) {
			if (__HAL_UART_GET_IT_SOURCE(handle, UART_IT_IDLE) != RESET) {
				irq_handler(serial_irq_ids[id], RxIrq);
				__HAL_UART_CLEAR_FLAG(handle, UART_FLAG_IDLE);
			}
		}
    }
}

#if DEVICE_SERIAL_ASYNCH_DMA
static void dma_irq(DMAName name, int id, SerialIrq txrxirq)
{
	DMA_HandleTypeDef * hdma = txrxirq == RxIrq ? &DmaRxHandle[id] : &DmaTxHandle[id];

	if (serial_irq_ids[id] != 0) {
		if (txrxirq == RxIrq) {
			if (__HAL_DMA_GET_TC_FLAG_INDEX(hdma) != RESET) {
				irq_handler(serial_irq_ids[id], RxIrq);
				__HAL_DMA_CLEAR_FLAG(hdma, DMA_FLAG_TCIF2_6);
			}
			} else {
			if (__HAL_DMA_GET_TC_FLAG_INDEX(hdma) != RESET) {
				irq_handler(serial_irq_ids[id], TxIrq);
				__HAL_DMA_CLEAR_FLAG(hdma, DMA_FLAG_TCIF0_4);
			}
		}
	}

}
#endif
// DEVICE_SERIAL_ASYNCH_DMA

static void uart1_irq(void)
{
    uart_irq(0);
}

#if defined(USART2_BASE)
static void uart2_irq(void)
{
    uart_irq(1);
}
#endif

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

#if DEVICE_SERIAL_ASYNCH_DMA

#if defined(UART5_BASE)
static void dma1_stream0_irq(void)
{
    dma_irq(DMA_1, 4, RxIrq); // uart5_rx
}
#endif

#if defined(USART3_BASE)
static void dma1_stream1_irq(void)
{
    dma_irq(DMA_1, 2, RxIrq); // uart3_rx
}
#endif

#if defined(UART4_BASE)
static void dma1_stream2_irq(void)
{
    dma_irq(DMA_1, 3, RxIrq); // uart4_rx
}
#endif

#if defined(USART3_BASE)
static void dma1_stream3_irq(void)
{
    dma_irq(DMA_1, 2, TxIrq); // uart3_tx
}
#endif

#if defined(UART4_BASE)
static void dma1_stream4_irq(void)
{
    dma_irq(DMA_1, 3, TxIrq); // uart4_tx
}
#endif

#if defined(USART2_BASE)
static void dma1_stream5_irq(void)
{
    dma_irq(DMA_1, 1, RxIrq); // uart2_rx
}
#endif

#if defined(USART2_BASE)
static void dma1_stream6_irq(void)
{
    dma_irq(DMA_1, 1, TxIrq); // uart2_tx
}
#endif

#if defined(UART5_BASE)
static void dma1_stream7_irq(void)
{
    dma_irq(DMA_1, 4, TxIrq); // uart5_tx
}
#endif

#if defined(USART6_BASE)
static void dma2_stream1_irq(void)
{
    dma_irq(DMA_2, 5, RxIrq); // uart6_rx
}
#endif

static void dma2_stream5_irq(void)
{
    dma_irq(DMA_2, 0, RxIrq); // uart1_rx
}

#if defined(USART6_BASE)
static void dma2_stream6_irq(void)
{
    dma_irq(DMA_2, 5, TxIrq); // uart6_tx
}
#endif

static void dma2_stream7_irq(void)
{
    dma_irq(DMA_2, 0, TxIrq); // uart1_tx
}

#endif
// DEVICE_SERIAL_ASYNCH_DMA

#if defined(UART5_BASE)
static void uart5_irq(void)
{
    uart_irq(4);
}
#endif

#if defined(USART6_BASE)
static void uart6_irq(void)
{
    uart_irq(5);
}
#endif

#if defined(UART7_BASE)
static void uart7_irq(void)
{
    uart_irq(6);
}
#endif

#if defined(UART8_BASE)
static void uart8_irq(void)
{
    uart_irq(7);
}
#endif

void serial_irq_handler(serial_t *obj, uart_irq_handler handler, uint32_t id)
{
    irq_handler = handler;
    serial_irq_ids[SERIAL_OBJ(index)] = id;
}

void serial_irq_set(serial_t *obj, SerialIrq irq, uint32_t enable)
{
    IRQn_Type irq_n = (IRQn_Type)0;
    uint32_t vector = 0;
#if DEVICE_SERIAL_ASYNCH_DMA
    IRQn_Type irqn_dma = (IRQn_Type)0;
    uint32_t vector_dma = 0;
#endif

    UART_HandleTypeDef *handle = &UartHandle[SERIAL_OBJ(index)];

    switch (SERIAL_OBJ(index)) {
        case 0:
            irq_n = USART1_IRQn;
            vector = (uint32_t)&uart1_irq;
#if DEVICE_SERIAL_ASYNCH_DMA
            if (irq == RxIrq) {
                irqn_dma = DMA2_Stream5_IRQn;
                vector_dma = (uint32_t)&dma2_stream5_irq;
            } else {
                irqn_dma = DMA2_Stream7_IRQn;
                vector_dma = (uint32_t)&dma2_stream7_irq;
            }
#endif
            break;

        case 1:
            irq_n = USART2_IRQn;
            vector = (uint32_t)&uart2_irq;
#if DEVICE_SERIAL_ASYNCH_DMA
            if (irq == RxIrq) {
                irqn_dma = DMA1_Stream5_IRQn;
                vector_dma = (uint32_t)&dma1_stream5_irq;
            } else {
                irqn_dma = DMA1_Stream6_IRQn;
                vector_dma = (uint32_t)&dma1_stream6_irq;
            }
#endif
            break;
#if defined(USART3_BASE)
        case 2:
            irq_n = USART3_IRQn;
            vector = (uint32_t)&uart3_irq;
#if DEVICE_SERIAL_ASYNCH_DMA
            if (irq == RxIrq) {
                irqn_dma = DMA1_Stream1_IRQn;
                vector_dma = (uint32_t)&dma1_stream1_irq;
            } else {
                irqn_dma = DMA1_Stream3_IRQn;
                vector_dma = (uint32_t)&dma1_stream3_irq;
            }
#endif
            break;
#endif
#if defined(UART4_BASE)
        case 3:
            irq_n = UART4_IRQn;
            vector = (uint32_t)&uart4_irq;
#if DEVICE_SERIAL_ASYNCH_DMA
            if (irq == RxIrq) {
                irqn_dma = DMA1_Stream2_IRQn;
                vector_dma = (uint32_t)&dma1_stream2_irq;
            } else {
                irqn_dma = DMA1_Stream4_IRQn;
                vector_dma = (uint32_t)&dma1_stream4_irq;
            }
#endif
            break;
#endif
#if defined(UART5_BASE)
        case 4:
            irq_n = UART5_IRQn;
            vector = (uint32_t)&uart5_irq;
#if DEVICE_SERIAL_ASYNCH_DMA
            if (irq == RxIrq) {
                irqn_dma = DMA1_Stream0_IRQn;
                vector_dma = (uint32_t)&dma1_stream0_irq;
            } else {
                irqn_dma = DMA1_Stream4_IRQn;
                vector_dma = (uint32_t)&dma1_stream7_irq;
            }
#endif
            break;
#endif
#if defined(USART6_BASE)
        case 5:
            irq_n = USART6_IRQn;
            vector = (uint32_t)&uart6_irq;
#if DEVICE_SERIAL_ASYNCH_DMA
            if (irq == RxIrq) {
                irqn_dma = DMA2_Stream1_IRQn;
                vector_dma = (uint32_t)&dma2_stream1_irq;
            } else {
                irqn_dma = DMA2_Stream6_IRQn;
                vector_dma = (uint32_t)&dma2_stream6_irq;
            }
#endif
            break;
#endif
#if defined(UART7_BASE)
        case 6:
            irq_n = UART7_IRQn;
            vector = (uint32_t)&uart7_irq;
            break;
#endif
#if defined(UART8_BASE)
        case 7:
            irq_n = UART8_IRQn;
            vector = (uint32_t)&uart8_irq;
            break;
#endif
    }

    if (enable) {

        if (irq == RxIrq) {
            __HAL_UART_ENABLE_IT(handle, UART_IT_RXNE);
			__HAL_UART_ENABLE_IT(handle, UART_IT_IDLE);
	        NVIC_SetVector(irq_n, vector_dma);
            NVIC_EnableIRQ(irq_n);
#if DEVICE_SERIAL_ASYNCH_DMA
            NVIC_SetVector(irqn_dma, vector_dma);
            NVIC_EnableIRQ(irqn_dma);
#endif
        } else { // TxIrq
            __HAL_UART_ENABLE_IT(handle, UART_IT_TC);
            NVIC_SetVector(irq_n, vector);
            NVIC_EnableIRQ(irq_n);
#if DEVICE_SERIAL_ASYNCH_DMA
            NVIC_SetVector(irqn_dma, vector_dma);
            NVIC_EnableIRQ(irqn_dma);
#endif
        }
    } else { // disable

        int all_disabled = 0;

        if (irq == RxIrq) {
            __HAL_UART_DISABLE_IT(handle, UART_IT_RXNE);
			__HAL_UART_DISABLE_IT(handle, UART_IT_IDLE);
            // Check if TxIrq is disabled too
            if ((handle->Instance->CR1 & USART_CR1_TXEIE) == 0) all_disabled = 1;
        } else { // TxIrq
            __HAL_UART_DISABLE_IT(handle, UART_IT_TXE);
            // Check if RxIrq is disabled too
            if ((handle->Instance->CR1 & USART_CR1_RXNEIE) == 0) all_disabled = 1;
        }

        if (all_disabled) {
          NVIC_DisableIRQ(irq_n);
#if DEVICE_SERIAL_ASYNCH_DMA
          NVIC_DisableIRQ(irqn_dma);
#endif
        }

    }
}

/******************************************************************************
 * READ/WRITE
 ******************************************************************************/

int serial_getc(serial_t *obj)
{
    UART_HandleTypeDef *handle = &UartHandle[SERIAL_OBJ(index)];
    while (!serial_readable(obj));
    return (int)(handle->Instance->DR & 0x1FF);
}

void serial_putc(serial_t *obj, int c)
{
    UART_HandleTypeDef *handle = &UartHandle[SERIAL_OBJ(index)];
    while (!serial_writable(obj));
    handle->Instance->DR = (uint32_t)(c & 0x1FF);
}

int serial_readable(serial_t *obj)
{
    int status;
    UART_HandleTypeDef *handle = &UartHandle[SERIAL_OBJ(index)];
    // Check if data is received
    status = ((__HAL_UART_GET_FLAG(handle, UART_FLAG_RXNE) != RESET) ? 1 : 0);
    return status;
}

int serial_writable(serial_t *obj)
{
    int status;
    UART_HandleTypeDef *handle = &UartHandle[SERIAL_OBJ(index)];
    // Check if data is transmitted
    status = ((__HAL_UART_GET_FLAG(handle, UART_FLAG_TXE) != RESET) ? 1 : 0);
    return status;
}

void serial_clear(serial_t *obj)
{
    UART_HandleTypeDef *handle = &UartHandle[SERIAL_OBJ(index)];
    __HAL_UART_CLEAR_FLAG(handle, UART_FLAG_TXE);
    __HAL_UART_CLEAR_FLAG(handle, UART_FLAG_RXNE);
}

void serial_pinout_tx(PinName tx)
{
    pinmap_pinout(tx, PinMap_UART_TX);
}

void serial_break_set(serial_t *obj)
{
    UART_HandleTypeDef *uart = &UartHandle[SERIAL_OBJ(index)];
    HAL_LIN_SendBreak(uart);
}

void serial_break_clear(serial_t *obj)
{
    (void)obj;
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
static void h_serial_tx_buffer_set(serial_t *obj, void *tx, int tx_length, uint8_t width)
{
    // We only support byte buffers for now
    (void)width;
    MBED_ASSERT(width == 8);
  
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
static void h_serial_rx_buffer_set(serial_t *obj, void *rx, int rx_length, uint8_t width)
{
    /* Sanity check arguments */
    MBED_ASSERT(obj);
    MBED_ASSERT(rx != (void*)0);
    // We only support byte buffers for now
    (void)width;
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
static void h_serial_tx_enable_event(serial_t *obj, int event, uint8_t enable)
{
    // Shouldn't have to enable TX interrupt here, just need to keep track of the requested events.
    if (enable) SERIAL_OBJ(events) |= event;
    else SERIAL_OBJ(events) &= ~event;
}

/** Configure RX events
 *
 * @param obj    The serial object
 * @param event  The logical OR of the RX events to configure
 * @param enable Set to non-zero to enable events, or zero to disable them
 */
static void h_serial_rx_enable_event(serial_t *obj, int event, uint8_t enable)
{
    // Shouldn't have to enable RX interrupt here, just need to keep track of the requested events.
    if (enable) SERIAL_OBJ(events) |= event;
    else SERIAL_OBJ(events) &= ~event;
}

/**
* Get index of serial object TX IRQ, relating it to the physical peripheral.
*
* @param obj pointer to serial object
* @return internal NVIC TX IRQ index of U(S)ART peripheral
*/
static IRQn_Type h_serial_get_irq_index(serial_t *obj)
{
    IRQn_Type irq_n = (IRQn_Type)0;

    switch (SERIAL_OBJ(index)) {
#if defined(USART1_BASE)
        case 0:
            irq_n = USART1_IRQn;
            break;
#endif
#if defined(USART2_BASE)
        case 1:
            irq_n = USART2_IRQn;
            break;
#endif
#if defined(USART3_BASE)
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
#if defined(USART6_BASE)
        case 5:
            irq_n = USART6_IRQn;
            break;
#endif
#if defined(UART7_BASE)
        case 6:
            irq_n = UART7_IRQn;
            break;
#endif
#if defined(UART8_BASE)
        case 7:
            irq_n = UART8_IRQn;
            break;
#endif
        default:
            irq_n = (IRQn_Type)0;
    }
    
    return irq_n;
}

#if DEVICE_SERIAL_ASYNCH_DMA
/**
* Get index of serial object TX DMA IRQ, relating it to the physical peripheral.
*
* @param obj pointer to serial object
* @return internal NVIC TX DMA IRQ index of U(S)ART peripheral
*/
static IRQn_Type h_serial_tx_get_irqdma_index(serial_t *obj)
{
    IRQn_Type irq_n = (IRQn_Type)0;

    switch (SERIAL_OBJ(index)) {
#if defined(USART1_BASE)
        case 0:
            irq_n = DMA2_Stream7_IRQn;
            break;
#endif
#if defined(USART2_BASE)
        case 1:
            irq_n = DMA1_Stream6_IRQn;
            break;
#endif
#if defined(USART3_BASE)
        case 2:
            irq_n = DMA1_Stream3_IRQn;
            break;
#endif
#if defined(UART4_BASE)
        case 3:
            irq_n = DMA1_Stream4_IRQn;
            break;
#endif
#if defined(UART5_BASE)
        case 4:
            irq_n = DMA1_Stream7_IRQn;
            break;
#endif
#if defined(USART6_BASE)
        case 5:
            irq_n = DMA2_Stream6_IRQn;
            break;
#endif
#if defined(UART7_BASE)
        case 6:
            irq_n = DMA1_Stream1_IRQn;
            break;
#endif
#if defined(UART8_BASE)
        case 7:
            irq_n = DMA1_Stream0_IRQn;
            break;
#endif
        default:
            irq_n = (IRQn_Type)0;
    }

    return irq_n;
}

/**
* Get index of serial object RX DMA IRQ, relating it to the physical peripheral.
*
* @param obj pointer to serial object
* @return internal NVIC RX DMA IRQ index of U(S)ART peripheral
*/
static IRQn_Type h_serial_rx_get_irqdma_index(serial_t *obj)
{
    IRQn_Type irq_n = (IRQn_Type)0;

    switch (SERIAL_OBJ(index)) {
#if defined(USART1_BASE)
        case 0:
            irq_n = DMA2_Stream5_IRQn;
            break;
#endif
#if defined(USART2_BASE)
        case 1:
            irq_n = DMA1_Stream5_IRQn;
            break;
#endif
#if defined(USART3_BASE)
        case 2:
            irq_n = DMA1_Stream1_IRQn;
            break;
#endif
#if defined(UART4_BASE)
        case 3:
            irq_n = DMA1_Stream2_IRQn;
            break;
#endif
#if defined(UART5_BASE)
        case 4:
            irq_n = DMA1_Stream0_IRQn;
            break;
#endif
#if defined(USART6_BASE)
        case 5:
            irq_n = DMA2_Stream1_IRQn;
            break;
#endif
#if defined(UART7_BASE)
        case 6:
            irq_n = DMA1_Stream3_IRQn;
            break;
#endif
#if defined(UART8_BASE)
        case 7:
            irq_n = DMA1_Stream6_IRQn;
            break;
#endif
        default:
            irq_n = (IRQn_Type)0;
    }

    return irq_n;
}
#endif
// DEVICE_SERIAL_ASYNCH_DMA

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
#ifdef YOTTA_CFG_MBED_OS
int serial_tx_asynch(serial_t *obj, void *tx, size_t tx_length, uint8_t tx_width, uint32_t handler, uint32_t event, DMAUsage hint)
#else
int serial_tx_asynch(serial_t *obj, const void *tx, size_t tx_length, uint8_t tx_width, uint32_t handler, uint32_t event, DMAUsage hint)
#endif
{
    // TODO: DMA usage is currently ignored
    (void) hint;
    
    // Check buffer is ok
    MBED_ASSERT(tx != (void*)0);
    MBED_ASSERT(tx_width == 8); // support only 8b width

    if (tx_length == 0) return 0;
  
    // Set up buffer
    h_serial_tx_buffer_set(obj, (void *)tx, tx_length, tx_width);
  
    // Set up events
    h_serial_tx_enable_event(obj, SERIAL_EVENT_TX_ALL, 0); // Clear all events
    h_serial_tx_enable_event(obj, event, 1); // Set only the wanted events
  
    UART_HandleTypeDef *handle = &UartHandle[SERIAL_OBJ(index)];
    // Enable interrupt
    IRQn_Type irqn = h_serial_get_irq_index(obj);
    NVIC_ClearPendingIRQ(irqn);
    NVIC_DisableIRQ(irqn);
    NVIC_SetPriority(irqn, 1);
    NVIC_SetVector(irqn, (uint32_t)handler);
    NVIC_EnableIRQ(irqn);

#if DEVICE_SERIAL_ASYNCH_DMA
    // Enable DMA interrupt
    irqn = h_serial_tx_get_irqdma_index(obj);
    NVIC_ClearPendingIRQ(irqn);
    NVIC_DisableIRQ(irqn);
    NVIC_SetPriority(irqn, 1);
    NVIC_SetVector(irqn, (uint32_t)handler);
    NVIC_EnableIRQ(irqn);

    // the following function will enable program and enable the DMA transfer
    if (HAL_UART_Transmit_DMA(handle, (uint8_t*)tx, tx_length) != HAL_OK)
    {
      /* Transfer error in transmission process */
      return 0;
    }
#else
    // the following function will enable UART_IT_TXE and error interrupts
    if (HAL_UART_Transmit_IT(handle, (uint8_t*)tx, tx_length) != HAL_OK)
    {
      /* Transfer error in transmission process */
      return 0;
    }
#endif

    DEBUG_PRINTF("UART%u: Tx: 0=(%u, %u) %x\n", SERIAL_OBJ(index+1), tx_length, tx_width, HAL_UART_GetState(handle));

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
    // TODO: DMA usage is currently ignored
    (void) hint;

    /* Sanity check arguments */
    MBED_ASSERT(obj);
    MBED_ASSERT(rx != (void*)0);
    MBED_ASSERT(rx_width == 8); // support only 8b width

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
    NVIC_SetPriority(irqn, 0);
    NVIC_SetVector(irqn, (uint32_t)handler);
    NVIC_EnableIRQ(irqn);


    UART_HandleTypeDef *handle = &UartHandle[SERIAL_OBJ(index)];
    // flush current data + error flags
    __HAL_UART_CLEAR_PEFLAG(handle);
#if DEVICE_SERIAL_ASYNCH_DMA
    // Enable DMA interrupt
    irqn = h_serial_rx_get_irqdma_index(obj);
    NVIC_ClearPendingIRQ(irqn);
    NVIC_DisableIRQ(irqn);
    NVIC_SetPriority(irqn, 1);
    NVIC_SetVector(irqn, (uint32_t)handler);
    NVIC_EnableIRQ(irqn);
    // following HAL function will program and enable the DMA transfer
    HAL_UART_Receive_DMA(handle, (uint8_t*)rx, rx_length);    
	    
    /* Enable the UART Idle line interrupt: */
    __HAL_UART_ENABLE_IT(handle, UART_IT_IDLE);

#else
    // following HAL function will enable the RXNE interrupt + error interrupts    
    HAL_UART_Receive_IT(handle, (uint8_t*)rx, rx_length);
#endif

    /* Enable the UART Error Interrupt: (Frame error, noise error, overrun error) */
    __HAL_UART_ENABLE_IT(handle, UART_IT_ERR);

    DEBUG_PRINTF("UART%u: Rx: 0=(%u, %u, %u) %x\n", SERIAL_OBJ(index+1), rx_length, rx_width, char_match, HAL_UART_GetState(handle));
}

/** Attempts to determine if the serial peripheral is already in use for TX
 *
 * @param obj The serial object
 * @return Non-zero if the TX transaction is ongoing, 0 otherwise
 */
uint8_t serial_tx_active(serial_t *obj)
{
    MBED_ASSERT(obj);
    UART_HandleTypeDef *handle = &UartHandle[SERIAL_OBJ(index)];
	return ((HAL_UART_GetState(handle) == HAL_UART_STATE_BUSY_TX) ? 1 : 0);
}

/** Attempts to determine if the serial peripheral is already in use for RX
 *
 * @param obj The serial object
 * @return Non-zero if the RX transaction is ongoing, 0 otherwise
 */
uint8_t serial_rx_active(serial_t *obj)
{
    MBED_ASSERT(obj);
    UART_HandleTypeDef *handle = &UartHandle[SERIAL_OBJ(index)];
	return ((HAL_UART_GetState(handle) == HAL_UART_STATE_BUSY_RX) ? 1 : 0);
}

void HAL_UART_ErrorCallback(UART_HandleTypeDef *huart) {

	wd_log_debug("HAL_UART_ErrorCallback -> Uart: %x, ErrorCode: %d", huart, huart->ErrorCode);
	
    if (__HAL_UART_GET_FLAG(huart, UART_FLAG_PE) != RESET) {
        volatile uint32_t tmpval = huart->Instance->DR; // Clear PE flag
    } else if (__HAL_UART_GET_FLAG(huart, UART_FLAG_FE) != RESET) {
        volatile uint32_t tmpval = huart->Instance->DR; // Clear FE flag
    } else if (__HAL_UART_GET_FLAG(huart, UART_FLAG_NE) != RESET) {
        volatile uint32_t tmpval = huart->Instance->DR; // Clear NE flag
    } else if (__HAL_UART_GET_FLAG(huart, UART_FLAG_ORE) != RESET) {
        volatile uint32_t tmpval = huart->Instance->DR; // Clear ORE flag
    }
}

/** The asynchronous TX and RX handler.
 *
 * @param obj The serial object
 * @return Returns event flags if a TX/RX transfer termination condition was met or 0 otherwise
 */
int serial_irq_handler_asynch(serial_t *obj)
{
    volatile int return_event = 0;
    uint8_t *buf = (uint8_t*)obj->rx_buff.buffer;
    uint8_t i = 0;

    // Irq handler is common to Tx and Rx
    UART_HandleTypeDef *handle = &UartHandle[SERIAL_OBJ(index)];
#if DEVICE_SERIAL_ASYNCH_DMA
    if ((handle->Instance->CR3 & USART_CR3_DMAT) !=0) {
        // call dma tx interrupt
        HAL_DMA_IRQHandler(handle->hdmatx);
    }
    if ((handle->Instance->CR3 & USART_CR3_DMAR) !=0) {
        // call dma rx interrupt
        HAL_DMA_IRQHandler(handle->hdmarx);
    }
#endif
    
    // TX PART:
	if (__HAL_UART_GET_FLAG(handle, UART_FLAG_TC) != RESET) {
        if (__HAL_UART_GET_IT_SOURCE(handle, UART_IT_TC) != RESET) {
            // Return event SERIAL_EVENT_TX_COMPLETE if requested
            if ((SERIAL_OBJ(events) & SERIAL_EVENT_TX_COMPLETE ) != 0){
            return_event |= SERIAL_EVENT_TX_COMPLETE & obj->serial.events;
        }
        }
    }
	
    // handle error events:
    if (__HAL_UART_GET_FLAG(handle, UART_FLAG_PE) != RESET) {
        if (__HAL_UART_GET_IT_SOURCE(handle, USART_IT_ERR) != RESET) {
			return_event |= SERIAL_EVENT_RX_PARITY_ERROR & obj->serial.events;
		}
	}
	
    if (__HAL_UART_GET_FLAG(handle, HAL_UART_ERROR_NE)||(handle->ErrorCode & HAL_UART_ERROR_NE)!=0) {
      if (__HAL_UART_GET_IT_SOURCE(handle, USART_IT_ERR) != RESET) {
            // not supported by mbed
        }
    }
	
    if (__HAL_UART_GET_FLAG(handle, UART_FLAG_FE) != RESET) {
        if (__HAL_UART_GET_IT_SOURCE(handle, USART_IT_ERR) != RESET) {
			return_event |= SERIAL_EVENT_RX_FRAMING_ERROR & SERIAL_OBJ(events);
	    }
    }
	
    if (__HAL_UART_GET_FLAG(handle, UART_FLAG_ORE) != RESET) {
        if (__HAL_UART_GET_IT_SOURCE(handle, USART_IT_ERR) != RESET) {
			return_event |= SERIAL_EVENT_RX_OVERRUN_ERROR & SERIAL_OBJ(events);
	    }
    }

	// read flags for return event first as they are cleared in IRQ handler
	HAL_UART_IRQHandler(handle);	
	
    // Abort if an error occurs
    if (return_event & SERIAL_EVENT_RX_PARITY_ERROR ||
            return_event & SERIAL_EVENT_RX_FRAMING_ERROR ||
            return_event & SERIAL_EVENT_RX_OVERRUN_ERROR) {
        return return_event;
    }
	
	//RX PART
	if (handle->RxXferSize != 0) {
		obj->rx_buff.pos = handle->RxXferSize - handle->RxXferCount;
	}
	if ((handle->RxXferCount == 0) && (obj->rx_buff.pos >= (obj->rx_buff.length - 1))) {
		return_event |= SERIAL_EVENT_RX_COMPLETE & SERIAL_OBJ(events);
	}

	// Check if char_match is present
	if (SERIAL_OBJ(events) & SERIAL_EVENT_RX_CHARACTER_MATCH) {
		if (buf != NULL) {
			for (i = 0; i < obj->rx_buff.pos; i++) {
				if (buf[i] == obj->char_match) {
					obj->rx_buff.pos = i;
					return_event |= (SERIAL_EVENT_RX_CHARACTER_MATCH & SERIAL_OBJ(events));
					serial_rx_abort_asynch(obj);
					break;
				}
			}
		}
	}
	
	// Idle line interrupt
	if (__HAL_UART_GET_FLAG(handle, UART_FLAG_IDLE) != RESET) {
		if (__HAL_UART_GET_IT_SOURCE(handle, UART_IT_IDLE) != RESET) {
			__HAL_UART_CLEAR_IDLEFLAG(handle);
			return_event |= (SERIAL_EVENT_RX_IDLE & SERIAL_OBJ(events));
		}
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
    UART_HandleTypeDef *handle = &UartHandle[SERIAL_OBJ(index)];
    __HAL_UART_DISABLE_IT(handle, UART_IT_TC|UART_IT_TXE);
    // clear flags
    __HAL_UART_CLEAR_PEFLAG(handle);
    // reset states
    handle->TxXferCount = 0;
	
	#if DEVICE_SERIAL_ASYNCH_DMA
	if (handle->hdmatx != NULL)
	{
		//HAL_DMA_Abort(handle->hdmatx);
	}
	#endif
	
    // update handle state
    if(handle->gState == HAL_UART_STATE_BUSY_TX_RX) {
        handle->gState = HAL_UART_STATE_BUSY_RX;
    } else {
        handle->gState = HAL_UART_STATE_READY;
    }
}

/** Abort the ongoing RX transaction It disables the enabled interrupt for RX and
 *  flush RX hardware buffer if RX FIFO is used
 *
 * @param obj The serial object
 */
void serial_rx_abort_asynch(serial_t *obj)
{
    UART_HandleTypeDef *handle = &UartHandle[SERIAL_OBJ(index)];
    __HAL_UART_DISABLE_IT(handle, UART_IT_RXNE);
//    __HAL_UART_DISABLE_IT(handle, UART_IT_IDLE);
    // clear flags
    __HAL_UART_CLEAR_PEFLAG(handle);
//	__HAL_UART_CLEAR_IDLEFLAG(handle);
    // reset states
    handle->RxXferCount = 0;
    
	#if DEVICE_SERIAL_ASYNCH_DMA
	if (handle->hdmarx != NULL)
	{
		//HAL_DMA_Abort(handle->hdmarx);
	}
	#endif
	
	// update handle state
    if(handle->RxState == HAL_UART_STATE_BUSY_TX_RX) {
        handle->RxState = HAL_UART_STATE_BUSY_TX;
    } else {
        handle->RxState = HAL_UART_STATE_READY;
    }
}

#endif
// DEVICE_SERIAL_ASYNCH

#if DEVICE_SERIAL_FC
/** Set HW Control Flow
 * @param obj    The serial object
 * @param type   The Control Flow type (FlowControlNone, FlowControlRTS, FlowControlCTS, FlowControlRTSCTS)
 * @param rxflow Pin for the rxflow
 * @param txflow Pin for the txflow
 */
void serial_set_flow_control(serial_t *obj, FlowControl type, PinName rxflow, PinName txflow)
{
    // Determine the UART to use (UART_1, UART_2, ...)
    UARTName uart_rts = (UARTName)pinmap_peripheral(rxflow, PinMap_UART_RTS);
    UARTName uart_cts = (UARTName)pinmap_peripheral(txflow, PinMap_UART_CTS);

    // Get the peripheral name (UART_1, UART_2, ...) from the rts/cts pins
    UARTName uart_rtscts = (UARTName)pinmap_merge(uart_cts, uart_rts);
    MBED_ASSERT(uart_rtscts != (UARTName)NC);
    // Check if the same UART is used
    if ((UARTName)(UartHandle[SERIAL_OBJ(index)].Instance) != uart_rtscts) {
        error("UART%u flow control pins are not correct\n", SERIAL_OBJ(index+1));
    }
    
    // Configure UART
    UART_HandleTypeDef *handle = &UartHandle[SERIAL_OBJ(index)];

    switch (type) {
        case FlowControlRTS:
            MBED_ASSERT(uart_rts != (UARTName)NC);
            // Enable the pin for RTS function
            pinmap_pinout(rxflow, PinMap_UART_RTS);
            handle->Init.HwFlowCtl = UART_HWCONTROL_RTS;
            break;
        case FlowControlCTS:
            MBED_ASSERT(uart_cts != (UARTName)NC);
            // Enable the pin for CTS function
            pinmap_pinout(txflow, PinMap_UART_CTS);
            handle->Init.HwFlowCtl = UART_HWCONTROL_CTS;
            break;
        case FlowControlRTSCTS:
            MBED_ASSERT(uart_rts != (UARTName)NC);
            MBED_ASSERT(uart_cts != (UARTName)NC);
            // Enable the pins for RTS and CTS functions
            pinmap_pinout(rxflow, PinMap_UART_RTS);
            pinmap_pinout(txflow, PinMap_UART_CTS);
            handle->Init.HwFlowCtl = UART_HWCONTROL_RTS_CTS;
            break;
        case FlowControlNone:
        default:
            handle->Init.HwFlowCtl = UART_HWCONTROL_NONE;
            break;
    }
    
    if (HAL_UART_Init(handle) != HAL_OK) {
        error("Cannot set UART%u flow control\n", SERIAL_OBJ(index+1));
    }
    
    DEBUG_PRINTF("UART%u: flow control set\n", SERIAL_OBJ(index+1));
}
#endif
// DEVICE_SERIAL_FC

#endif