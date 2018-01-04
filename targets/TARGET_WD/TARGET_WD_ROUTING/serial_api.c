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
#include "mbed_assert.h"
#include "serial_api.h"

#if DEVICE_SERIAL

#include "cmsis.h"
#include "pinmap.h"
#include "mbed_error.h"
#include <string.h>
#include "PeripheralPins.h"
#include "wd_logging.h"

#define UART_NUM (5)

static uint32_t serial_irq_ids[UART_NUM] = {0};
UART_HandleTypeDef uart_handlers[UART_NUM];

static uart_irq_handler irq_handler;

int stdio_uart_inited = 0;
serial_t stdio_uart;

#if DEVICE_SERIAL_ASYNCH_DMA

static const uint32_t DMA_UartRx_Channel[UART_NUM] = {
	DMA1_Channel5, DMA1_Channel6, DMA1_Channel3, DMA2_Channel3
};
	
static const uint32_t DMA_UartTx_Channel[UART_NUM] = {
	DMA1_Channel4, DMA1_Channel7, DMA1_Channel2, DMA2_Channel5
};
	
DMA_HandleTypeDef DmaTxHandle[UART_NUM];
DMA_HandleTypeDef DmaRxHandle[UART_NUM];

#endif

#if DEVICE_SERIAL_ASYNCH
	#define SERIAL_S(obj) (&((obj)->serial))
#else
	#define SERIAL_S(obj) (obj)
#endif

#if DEVICE_SERIAL_ASYNCH_DMA
static void init_dma(serial_t *obj)
{
	struct serial_s *obj_s = SERIAL_S(obj);
	UART_HandleTypeDef *huart = &uart_handlers[obj_s->index];
	
	if (obj_s->pin_tx != NC) {
		// set DMA in the UartHandle
		DMA_HandleTypeDef *hdma_tx = &DmaTxHandle[obj_s->index];
		/* Configure the DMA handler for Transmission process */
		hdma_tx->Instance                 = (DMA_Channel_TypeDef *)DMA_UartTx_Channel[obj_s->index];
		hdma_tx->Init.Direction           = DMA_MEMORY_TO_PERIPH;
		hdma_tx->Init.PeriphInc           = DMA_PINC_DISABLE;
		hdma_tx->Init.MemInc              = DMA_MINC_ENABLE;
		hdma_tx->Init.PeriphDataAlignment = DMA_PDATAALIGN_BYTE;
		hdma_tx->Init.MemDataAlignment    = DMA_MDATAALIGN_BYTE;
		hdma_tx->Init.Mode                = DMA_NORMAL;
		hdma_tx->Init.Priority            = DMA_PRIORITY_HIGH;
		
		if (HAL_DMA_Init(hdma_tx) != HAL_OK) {
			error("Cannot initialize TX DMA\n");
		}
		
		/* Associate the initialized DMA handle to the UART handle */
		huart->hdmatx = hdma_tx;
		hdma_tx->Parent = huart;
	}
	
	if (obj_s->pin_rx != NC) {
		/* Configure the DMA handler for reception process */
		DMA_HandleTypeDef *hdma_rx = &DmaRxHandle[obj_s->index];
		hdma_rx->Instance                 = (DMA_Channel_TypeDef *)DMA_UartRx_Channel[obj_s->index];
		hdma_rx->Init.Direction           = DMA_PERIPH_TO_MEMORY;
		hdma_rx->Init.PeriphInc           = DMA_PINC_DISABLE;
		hdma_rx->Init.MemInc              = DMA_MINC_ENABLE;
		hdma_rx->Init.PeriphDataAlignment = DMA_PDATAALIGN_BYTE;
		hdma_rx->Init.MemDataAlignment    = DMA_MDATAALIGN_BYTE;
		hdma_rx->Init.Mode                = DMA_CIRCULAR;
		hdma_rx->Init.Priority            = DMA_PRIORITY_HIGH;

		if (HAL_DMA_Init(hdma_rx) != HAL_OK) {
			error("Cannot initialize RX DMA\n");
		}

		/* Associate the initialized DMA handle to the UART handle */
		huart->hdmarx = hdma_rx;
		hdma_rx->Parent = huart;
	}
}
#endif

static void init_uart(serial_t *obj)
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

    if (HAL_UART_Init(huart) != HAL_OK) {
        error("Cannot initialize UART\n");
    }
}

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
		#if DEVICE_SERIAL_ASYNCH_DMA
		__HAL_RCC_DMA1_CLK_ENABLE();
		#endif
    }
    if (obj_s->uart == UART_2) {
        __HAL_RCC_USART2_FORCE_RESET();
        __HAL_RCC_USART2_RELEASE_RESET();
        __HAL_RCC_USART2_CLK_ENABLE();
        obj_s->index = 1;
		#if DEVICE_SERIAL_ASYNCH_DMA
		__HAL_RCC_DMA1_CLK_ENABLE();
		#endif
    }
    if (obj_s->uart == UART_3) {
        __HAL_RCC_USART3_FORCE_RESET();
        __HAL_RCC_USART3_RELEASE_RESET();
        __HAL_RCC_USART3_CLK_ENABLE();
        obj_s->index = 2;
		#if DEVICE_SERIAL_ASYNCH_DMA
		__HAL_RCC_DMA1_CLK_ENABLE();
		#endif
    }
	if (obj_s->uart == UART_4) {
        __HAL_RCC_UART4_FORCE_RESET();
        __HAL_RCC_UART4_RELEASE_RESET();
        __HAL_RCC_UART4_CLK_ENABLE();
        obj_s->index = 3;
		#if DEVICE_SERIAL_ASYNCH_DMA
		__HAL_RCC_DMA2_CLK_ENABLE();
		#endif
    }
	if (obj_s->uart == UART_5) {
        __HAL_RCC_UART5_FORCE_RESET();
        __HAL_RCC_UART5_RELEASE_RESET();
        __HAL_RCC_UART5_CLK_ENABLE();
        obj_s->index = 4;
		// PFR: no DMA!
    }

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
    obj_s->baudrate = MBED_CONF_PLATFORM_DEFAULT_SERIAL_BAUD_RATE;
    obj_s->databits = UART_WORDLENGTH_8B;
    obj_s->stopbits = UART_STOPBITS_1;
    obj_s->parity   = UART_PARITY_NONE;
    
#if DEVICE_SERIAL_FC
    obj_s->hw_flow_ctl = UART_HWCONTROL_NONE;
#endif

    obj_s->pin_tx = tx;
    obj_s->pin_rx = rx;

    init_uart(obj);

	#if DEVICE_SERIAL_ASYNCH_DMA
	init_dma(obj);
	#endif

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
        __USART1_FORCE_RESET();
        __USART1_RELEASE_RESET();
        __USART1_CLK_DISABLE();
		#if DEVICE_SERIAL_ASYNCH_DMA
		__HAL_RCC_DMA1_CLK_DISABLE();
		#endif
    }
    if (obj_s->uart == UART_2) {
        __USART2_FORCE_RESET();
        __USART2_RELEASE_RESET();
        __USART2_CLK_DISABLE();
		#if DEVICE_SERIAL_ASYNCH_DMA
		__HAL_RCC_DMA1_CLK_DISABLE();
		#endif
    }
    if (obj_s->uart == UART_3) {
        __USART3_FORCE_RESET();
        __USART3_RELEASE_RESET();
        __USART3_CLK_DISABLE();
		#if DEVICE_SERIAL_ASYNCH_DMA
		__HAL_RCC_DMA1_CLK_DISABLE();
		#endif
    }
	if (obj_s->uart == UART_4) {
        __UART4_FORCE_RESET();
        __UART4_RELEASE_RESET();
        __UART4_CLK_DISABLE();
		#if DEVICE_SERIAL_ASYNCH_DMA
		__HAL_RCC_DMA2_CLK_DISABLE();
		#endif
    }
	if (obj_s->uart == UART_5) {
        __UART5_FORCE_RESET();
        __UART5_RELEASE_RESET();
        __UART5_CLK_DISABLE();
    }

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

void serial_format(serial_t *obj, int data_bits, SerialParity parity, int stop_bits)
{
    struct serial_s *obj_s = SERIAL_S(obj);

    if (data_bits == 9) {
        obj_s->databits = UART_WORDLENGTH_9B;
    } else {
        obj_s->databits = UART_WORDLENGTH_8B;
    }

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

    if (stop_bits == 2) {
        obj_s->stopbits = UART_STOPBITS_2;
    } else {
        obj_s->stopbits = UART_STOPBITS_1;
    }

    init_uart(obj);
}

/******************************************************************************
 * INTERRUPTS HANDLING
 ******************************************************************************/

static void uart_irq(int id)
{
    UART_HandleTypeDef * huart = &uart_handlers[id];
    
    if (serial_irq_ids[id] != 0) {
        if (__HAL_UART_GET_FLAG(huart, UART_FLAG_TC) != RESET) {
            if (__HAL_UART_GET_IT_SOURCE(huart, UART_IT_TC) != RESET) {
                irq_handler(serial_irq_ids[id], TxIrq);
                __HAL_UART_CLEAR_FLAG(huart, UART_FLAG_TC);
            }
        }
        if (__HAL_UART_GET_FLAG(huart, UART_FLAG_RXNE) != RESET) {
            if (__HAL_UART_GET_IT_SOURCE(huart, UART_IT_RXNE) != RESET) {
                irq_handler(serial_irq_ids[id], RxIrq);
                __HAL_UART_CLEAR_FLAG(huart, UART_FLAG_RXNE);
            }
        }
		if (__HAL_UART_GET_FLAG(huart, UART_FLAG_IDLE) != RESET) {
			if (__HAL_UART_GET_IT_SOURCE(huart, UART_IT_IDLE) != RESET) {
				irq_handler(serial_irq_ids[id], RxIrq);
				__HAL_UART_CLEAR_FLAG(huart, UART_FLAG_IDLE);
			}
		}
        if (__HAL_UART_GET_FLAG(huart, UART_FLAG_ORE) != RESET) {
            if (__HAL_UART_GET_IT_SOURCE(huart, UART_IT_ERR) != RESET) {
                volatile uint32_t tmpval = huart->Instance->DR; // Clear ORE flag
            }
        }
    }
}

#if DEVICE_SERIAL_ASYNCH_DMA
static void dma_irq(int id, SerialIrq txrxirq) {

	DMA_HandleTypeDef * hdma = txrxirq == RxIrq ? &DmaRxHandle[id] : &DmaTxHandle[id];

	// get transfer complete flag of DMA handle
	if (serial_irq_ids[id] != 0) {
		if (txrxirq == RxIrq) {
			if (__HAL_DMA_GET_TC_FLAG_INDEX(hdma) != RESET) {
				irq_handler(serial_irq_ids[id], RxIrq);
				__HAL_DMA_CLEAR_FLAG(hdma, __HAL_DMA_GET_TC_FLAG_INDEX(hdma));
			}
		} else {
			if (__HAL_DMA_GET_TC_FLAG_INDEX(hdma) != RESET) {
				irq_handler(serial_irq_ids[id], TxIrq);
				__HAL_DMA_CLEAR_FLAG(hdma, __HAL_DMA_GET_TC_FLAG_INDEX(hdma));
			}
		}
	}
}
#endif // DEVICE_SERIAL_ASYNCH_DMA

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

#if DEVICE_SERIAL_ASYNCH_DMA

#if defined(USART3_BASE)
static void dma1_channel3_irq(void)
{
	dma_irq(2 , RxIrq); // uart3_rx
}
#endif

#if defined(UART4_BASE)
static void dma2_channel3_irq(void)
{
	dma_irq(3, RxIrq); // uart4_rx
}
#endif

#if defined(USART3_BASE)
static void dma1_channel2_irq(void)
{
	dma_irq(2, TxIrq); // uart3_tx
}
#endif

#if defined(UART4_BASE)
static void dma2_channel5_irq(void)
{
	dma_irq(3, TxIrq); // uart4_tx
}
#endif

#if defined(USART2_BASE)
static void dma1_channel6_irq(void)
{
	dma_irq(1, RxIrq); // uart2_rx
}
#endif

#if defined(USART2_BASE)
static void dma1_channel7_irq(void)
{
	dma_irq(1, TxIrq); // uart2_tx
}
#endif

static void dma1_channel5_irq(void)
{
	dma_irq(0, RxIrq); // uart1_rx
}

static void dma1_channel4_irq(void)
{
	dma_irq(0, TxIrq); // uart1_tx
}

#endif // DEVICE_SERIAL_ASYNCH_DMA


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

	#if DEVICE_SERIAL_ASYNCH_DMA
	IRQn_Type irqn_dma = (IRQn_Type)0;
	uint32_t vector_dma = 0;
	#endif

    switch (obj_s->index) {
        case 0:
            irq_n = USART1_IRQn;
            vector = (uint32_t)&uart1_irq;
			#if DEVICE_SERIAL_ASYNCH_DMA
			if (irq == RxIrq) {
				irqn_dma = DMA1_Channel5_IRQn;
				vector_dma = (uint32_t)&dma1_channel5_irq;
			} else {
				irqn_dma = DMA1_Channel4_IRQn;
				vector_dma = (uint32_t)&dma1_channel4_irq;
			}
			#endif
            break;

        case 1:
            irq_n = USART2_IRQn;
            vector = (uint32_t)&uart2_irq;
			#if DEVICE_SERIAL_ASYNCH_DMA
			if (irq == RxIrq) {
				irqn_dma = DMA1_Channel6_IRQn;
				vector_dma = (uint32_t)&dma1_channel6_irq;
			} else {
				irqn_dma = DMA1_Channel7_IRQn;
				vector_dma = (uint32_t)&dma1_channel7_irq;
			}
			#endif
            break;
			
		#if defined(USART3_BASE)
        case 2:
            irq_n = USART3_IRQn;
            vector = (uint32_t)&uart3_irq;
			#if DEVICE_SERIAL_ASYNCH_DMA
			if (irq == RxIrq) {
				irqn_dma = DMA1_Channel3_IRQn;
				vector_dma = (uint32_t)&dma1_channel3_irq;
				} else {
				irqn_dma = DMA1_Channel2_IRQn;
				vector_dma = (uint32_t)&dma1_channel2_irq;
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
				irqn_dma = DMA2_Channel3_IRQn;
				vector_dma = (uint32_t)&dma2_channel3_irq;
				} else {
				irqn_dma = DMA2_Channel4_5_IRQn;
				vector_dma = (uint32_t)&dma2_channel5_irq;
			}
			#endif
            break;
		#endif
		
		#if defined(UART5_BASE)
        case 4:
            irq_n = UART5_IRQn;
            vector = (uint32_t)&uart5_irq;
            break;
		#endif
    }

    if (enable) {
        if (irq == RxIrq) {
            __HAL_UART_ENABLE_IT(huart, UART_IT_RXNE);
			__HAL_UART_ENABLE_IT(huart, UART_IT_IDLE);
			#if DEVICE_SERIAL_ASYNCH_DMA
			NVIC_SetVector(irq_n, vector_dma);
			NVIC_EnableIRQ(irq_n);
			NVIC_SetVector(irqn_dma, vector_dma);
			NVIC_EnableIRQ(irqn_dma);
			#else
			NVIC_SetVector(irq_n, vector);
			NVIC_EnableIRQ(irq_n);
			#endif
			
        } else { // TxIrq
            __HAL_UART_ENABLE_IT(huart, UART_IT_TC);
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
            __HAL_UART_DISABLE_IT(huart, UART_IT_RXNE);
			__HAL_UART_DISABLE_IT(huart, UART_IT_IDLE);
            // Check if TxIrq is disabled too
            if ((huart->Instance->CR1 & USART_CR1_TXEIE) == 0) {
                all_disabled = 1;
            }
        } else { // TxIrq
            __HAL_UART_DISABLE_IT(huart, UART_IT_TC);
            // Check if RxIrq is disabled too
            if ((huart->Instance->CR1 & USART_CR1_RXNEIE) == 0) {
                all_disabled = 1;
            }
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
    struct serial_s *obj_s = SERIAL_S(obj);
    UART_HandleTypeDef *huart = &uart_handlers[obj_s->index];

    while (!serial_readable(obj));
    if (obj_s->databits == UART_WORDLENGTH_8B) {
        return (int)(huart->Instance->DR & (uint8_t)0xFF);
    } else {
        return (int)(huart->Instance->DR & (uint16_t)0x1FF);
    }
}

void serial_putc(serial_t *obj, int c)
{
    struct serial_s *obj_s = SERIAL_S(obj);
    UART_HandleTypeDef *huart = &uart_handlers[obj_s->index];

    while (!serial_writable(obj));
    if (obj_s->databits == UART_WORDLENGTH_8B) {
        huart->Instance->DR = (uint8_t)(c & (uint8_t)0xFF);
    } else {
        huart->Instance->DR = (uint16_t)(c & (uint16_t)0x1FF);
    }
}

int serial_readable(serial_t *obj)
{
    struct serial_s *obj_s = SERIAL_S(obj);
    UART_HandleTypeDef *huart = &uart_handlers[obj_s->index];
    
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

void serial_clear(serial_t *obj)
{
    struct serial_s *obj_s = SERIAL_S(obj);
    UART_HandleTypeDef *huart = &uart_handlers[obj_s->index];
    
    huart->TxXferCount = 0;
    huart->RxXferCount = 0;
}

void serial_pinout_tx(PinName tx)
{
    pinmap_pinout(tx, PinMap_UART_TX);
}

void serial_break_set(serial_t *obj)
{
    struct serial_s *obj_s = SERIAL_S(obj);
    UART_HandleTypeDef *huart = &uart_handlers[obj_s->index];
    
    HAL_LIN_SendBreak(huart);
}

void serial_break_clear(serial_t *obj)
{
    (void)obj;
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
#if defined(USART5_BASE)
        case 4:
            irq_n = UART5_IRQn;
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
static IRQn_Type serial_tx_get_irqdma_n(serial_t *obj)
{
	struct serial_s *obj_s = SERIAL_S(obj);
	IRQn_Type irq_n = (IRQn_Type)0;

	switch (obj_s->index) {
		#if defined(USART1_BASE)
		case 0:
		irq_n = DMA1_Channel4_IRQn;
		break;
		#endif
		#if defined(USART2_BASE)
		case 1:
		irq_n = DMA1_Channel7_IRQn;
		break;
		#endif
		#if defined(USART3_BASE)
		case 2:
		irq_n = DMA1_Channel2_IRQn;
		break;
		#endif
		#if defined(UART4_BASE)
		case 3:
		irq_n = DMA2_Channel4_5_IRQn;
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
static IRQn_Type serial_rx_get_irqdma_n(serial_t *obj)
{
	struct serial_s *obj_s = SERIAL_S(obj);
	IRQn_Type irq_n = (IRQn_Type)0;

	switch (obj_s->index) {
		#if defined(USART1_BASE)
		case 0:
		irq_n = DMA1_Channel5_IRQn;
		break;
		#endif
		#if defined(USART2_BASE)
		case 1:
		irq_n = DMA1_Channel6_IRQn;
		break;
		#endif
		#if defined(USART3_BASE)
		case 2:
		irq_n = DMA1_Channel3_IRQn;
		break;
		#endif
		#if defined(UART4_BASE)
		case 3:
		irq_n = DMA2_Channel3_IRQn;
		break;
		#endif
		default:
		irq_n = (IRQn_Type)0;
	}

	return irq_n;
}

#endif // DEVICE_SERIAL_ASYNCH_DMA

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

    #if DEVICE_SERIAL_ASYNCH_DMA
    // Enable DMA interrupt
    irq_n = serial_tx_get_irqdma_n(obj);
    NVIC_ClearPendingIRQ(irq_n);
    NVIC_DisableIRQ(irq_n);
    NVIC_SetPriority(irq_n, 1);
    NVIC_SetVector(irq_n, (uint32_t)handler);
    NVIC_EnableIRQ(irq_n);

    // the following function will enable program and enable the DMA transfer
    if (HAL_UART_Transmit_DMA(huart, (uint8_t*)tx, tx_length) != HAL_OK)
    {
	    /* Transfer error in transmission process */
	    return 0;
    }
    #else
    // the following function will enable UART_IT_TXE and error interrupts
    if (HAL_UART_Transmit_IT(huart, (uint8_t*)tx, tx_length) != HAL_OK)
    {
	    /* Transfer error in transmission process */
	    return 0;
    }
    #endif
    
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
    //(void) hint;

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
	
	__HAL_UART_CLEAR_PEFLAG(huart);
	#if DEVICE_SERIAL_ASYNCH_DMA
	// Enable DMA interrupt
	irq_n = serial_rx_get_irqdma_n(obj);
	NVIC_ClearPendingIRQ(irq_n);
	NVIC_DisableIRQ(irq_n);
	NVIC_SetPriority(irq_n, 1);
	NVIC_SetVector(irq_n, (uint32_t)handler);
	NVIC_EnableIRQ(irq_n);
	// following HAL function will program and enable the DMA transfer
	HAL_UART_Receive_DMA(huart, (uint8_t*)rx, rx_length);
	
	/* Enable the UART Idle line interrupt: */
	__HAL_UART_ENABLE_IT(huart, UART_IT_IDLE);
	#else
	// following HAL function will enable the RXNE interrupt + error interrupts
	HAL_UART_Receive_IT(huart, (uint8_t*)rx, rx_length);
	#endif
	
	/* Enable the UART Error Interrupt: (Frame error, noise error, overrun error) */
	__HAL_UART_ENABLE_IT(huart, UART_IT_ERR);

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
    
	// Irq handler is common to Tx and Rx
	#if DEVICE_SERIAL_ASYNCH_DMA
	if ((huart->Instance->CR3 & USART_CR3_DMAT) !=0) {
		// call dma tx interrupt
		HAL_DMA_IRQHandler(huart->hdmatx);
	}
	if ((huart->Instance->CR3 & USART_CR3_DMAR) !=0) {
		// call dma rx interrupt
		HAL_DMA_IRQHandler(huart->hdmarx);
	}
	#endif
	
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
        if (__HAL_UART_GET_IT_SOURCE(huart, USART_IT_ERR) != RESET) {
            return_event |= (SERIAL_EVENT_RX_PARITY_ERROR & obj_s->events);
        }
	}
	
	if (__HAL_UART_GET_FLAG(huart, UART_FLAG_NE) != RESET || (huart->ErrorCode & UART_FLAG_NE)!=0) {
        if (__HAL_UART_GET_IT_SOURCE(huart, USART_IT_ERR) != RESET) {
            // not supported by mbed
        }
	}

    if (__HAL_UART_GET_FLAG(huart, UART_FLAG_FE) != RESET) {
        if (__HAL_UART_GET_IT_SOURCE(huart, USART_IT_ERR) != RESET) {
            return_event |= (SERIAL_EVENT_RX_FRAMING_ERROR & obj_s->events);
        }
    }
    
    if (__HAL_UART_GET_FLAG(huart, UART_FLAG_ORE) != RESET) {
        if (__HAL_UART_GET_IT_SOURCE(huart, USART_IT_ERR) != RESET) {
            return_event |= (SERIAL_EVENT_RX_OVERRUN_ERROR & obj_s->events);
        }
    }
    
	// read flags for return event first as they are cleared in IRQ handler
	HAL_UART_IRQHandler(huart);	
	
    // Abort if an error occurs
    if (return_event & SERIAL_EVENT_RX_PARITY_ERROR ||
            return_event & SERIAL_EVENT_RX_FRAMING_ERROR ||
            return_event & SERIAL_EVENT_RX_OVERRUN_ERROR) {
        return return_event;
    }
    
    // RX PART:
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
	 
	 if (__HAL_UART_GET_FLAG(huart, UART_FLAG_IDLE) != RESET) {
		 if (__HAL_UART_GET_IT_SOURCE(huart, UART_IT_IDLE) != RESET) {
			 __HAL_UART_CLEAR_IDLEFLAG(huart);
			 return_event |= (SERIAL_EVENT_RX_IDLE & obj_s->events);
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
    
#if DEVICE_SERIAL_ASYNCH_DMA
	if (huart->hdmatx != NULL)
	{
//		HAL_DMA_Abort(huart->hdmatx);
	}
	#endif
	
	// update handle state
    if(huart->State == HAL_UART_STATE_BUSY_TX_RX) {
        huart->State = HAL_UART_STATE_BUSY_RX;
    } else {
        huart->State = HAL_UART_STATE_READY;
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
	__HAL_UART_DISABLE_IT(huart, UART_IT_IDLE);
    __HAL_UART_DISABLE_IT(huart, UART_IT_PE);
    __HAL_UART_DISABLE_IT(huart, UART_IT_ERR);
    
    // clear flags
    __HAL_UART_CLEAR_FLAG(huart, UART_FLAG_RXNE);
	__HAL_UART_CLEAR_FLAG(huart, UART_FLAG_IDLE);
    volatile uint32_t tmpval = huart->Instance->DR; // Clear errors flag
    
	#if DEVICE_SERIAL_ASYNCH_DMA
	if (huart->hdmarx != NULL)
	{
//		HAL_DMA_Abort(huart->hdmarx);
	}
	#endif
	
    // reset states
    huart->RxXferCount = 0;
    // update handle state
    if(huart->State == HAL_UART_STATE_BUSY_TX_RX) {
        huart->State = HAL_UART_STATE_BUSY_TX;
    } else {
        huart->State = HAL_UART_STATE_READY;
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
