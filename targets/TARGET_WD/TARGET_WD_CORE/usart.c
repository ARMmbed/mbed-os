/**
  ******************************************************************************
  * File Name          : USART.c
  * Description        : This file provides code for the configuration
  *                      of the USART instances.
  ******************************************************************************
  * This notice applies to any and all portions of this file
  * that are not between comment pairs USER CODE BEGIN and
  * USER CODE END. Other portions of this file, whether
  * inserted by the user or by software development tools
  * are owned by their respective copyright owners.
  *
  * Copyright (c) 2018 STMicroelectronics International N.V.
  * All rights reserved.
  *
  * Redistribution and use in source and binary forms, with or without
  * modification, are permitted, provided that the following conditions are met:
  *
  * 1. Redistribution of source code must retain the above copyright notice,
  *    this list of conditions and the following disclaimer.
  * 2. Redistributions in binary form must reproduce the above copyright notice,
  *    this list of conditions and the following disclaimer in the documentation
  *    and/or other materials provided with the distribution.
  * 3. Neither the name of STMicroelectronics nor the names of other
  *    contributors to this software may be used to endorse or promote products
  *    derived from this software without specific written permission.
  * 4. This software, including modifications and/or derivative works of this
  *    software, must execute solely and exclusively on microcontroller or
  *    microprocessor devices manufactured by or for STMicroelectronics.
  * 5. Redistribution and use of this software other than as permitted under
  *    this license is void and will automatically terminate your rights under
  *    this license.
  *
  * THIS SOFTWARE IS PROVIDED BY STMICROELECTRONICS AND CONTRIBUTORS "AS IS"
  * AND ANY EXPRESS, IMPLIED OR STATUTORY WARRANTIES, INCLUDING, BUT NOT
  * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY, FITNESS FOR A
  * PARTICULAR PURPOSE AND NON-INFRINGEMENT OF THIRD PARTY INTELLECTUAL PROPERTY
  * RIGHTS ARE DISCLAIMED TO THE FULLEST EXTENT PERMITTED BY LAW. IN NO EVENT
  * SHALL STMICROELECTRONICS OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
  * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
  * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA,
  * OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
  * LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
  * NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
  * EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
  *
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "usart.h"

/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

DMA_HandleTypeDef hdma_uart4_rx;
DMA_HandleTypeDef hdma_uart4_tx;
DMA_HandleTypeDef hdma_usart2_rx;
DMA_HandleTypeDef hdma_usart2_tx;

void HAL_UART_MspInit(UART_HandleTypeDef *uartHandle)
{

    // GPIO_InitTypeDef GPIO_InitStruct;
    if (uartHandle->Instance == UART4) {
        /* USER CODE BEGIN UART4_MspInit 0 */

        /* USER CODE END UART4_MspInit 0 */
        /* UART4 clock enable */
        __HAL_RCC_UART4_CLK_ENABLE();

        /**UART4 GPIO Configuration
        PC10     ------> UART4_TX
        PC11     ------> UART4_RX
        */
        // GPIO_InitStruct.Pin = BUS_TxD0|BUS_RxD0;
        // GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
        // GPIO_InitStruct.Pull = GPIO_PULLUP;
        // GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
        // GPIO_InitStruct.Alternate = GPIO_AF8_UART4;
        // HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

        /* UART4 DMA Init */
        /* UART4_RX Init */
        hdma_uart4_rx.Instance = DMA1_Stream2;
        hdma_uart4_rx.Init.Channel = DMA_CHANNEL_4;
        hdma_uart4_rx.Init.Direction = DMA_PERIPH_TO_MEMORY;
        hdma_uart4_rx.Init.PeriphInc = DMA_PINC_DISABLE;
        hdma_uart4_rx.Init.MemInc = DMA_MINC_ENABLE;
        hdma_uart4_rx.Init.PeriphDataAlignment = DMA_PDATAALIGN_BYTE;
        hdma_uart4_rx.Init.MemDataAlignment = DMA_MDATAALIGN_BYTE;
        hdma_uart4_rx.Init.Mode = DMA_CIRCULAR;
        hdma_uart4_rx.Init.Priority = DMA_PRIORITY_HIGH;
        hdma_uart4_rx.Init.FIFOMode = DMA_FIFOMODE_DISABLE;
        if (HAL_DMA_Init(&hdma_uart4_rx) != HAL_OK) {
            // _Error_Handler(__FILE__, __LINE__);
        }

        __HAL_LINKDMA(uartHandle, hdmarx, hdma_uart4_rx);

        /* UART4_TX Init */
        hdma_uart4_tx.Instance = DMA1_Stream4;
        hdma_uart4_tx.Init.Channel = DMA_CHANNEL_4;
        hdma_uart4_tx.Init.Direction = DMA_MEMORY_TO_PERIPH;
        hdma_uart4_tx.Init.PeriphInc = DMA_PINC_DISABLE;
        hdma_uart4_tx.Init.MemInc = DMA_MINC_ENABLE;
        hdma_uart4_tx.Init.PeriphDataAlignment = DMA_PDATAALIGN_BYTE;
        hdma_uart4_tx.Init.MemDataAlignment = DMA_MDATAALIGN_BYTE;
        hdma_uart4_tx.Init.Mode = DMA_NORMAL;
        hdma_uart4_tx.Init.Priority = DMA_PRIORITY_LOW;
        hdma_uart4_tx.Init.FIFOMode = DMA_FIFOMODE_DISABLE;
        if (HAL_DMA_Init(&hdma_uart4_tx) != HAL_OK) {
            // _Error_Handler(__FILE__, __LINE__);
        }

        __HAL_LINKDMA(uartHandle, hdmatx, hdma_uart4_tx);

        /* USER CODE BEGIN UART4_MspInit 1 */

        /* USER CODE END UART4_MspInit 1 */
    } else if (uartHandle->Instance == UART5) {
        /* USER CODE BEGIN UART5_MspInit 0 */

        /* USER CODE END UART5_MspInit 0 */
        /* UART5 clock enable */
        __HAL_RCC_UART5_CLK_ENABLE();

        /**UART5 GPIO Configuration
        PC12     ------> UART5_TX
        PD2     ------> UART5_RX
        */
        // GPIO_InitStruct.Pin = TXT4;
        // GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
        // GPIO_InitStruct.Pull = GPIO_PULLUP;
        // GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
        // GPIO_InitStruct.Alternate = GPIO_AF8_UART5;
        // HAL_GPIO_Init(TXT4_GPIO_Port, &GPIO_InitStruct);

        // GPIO_InitStruct.Pin = RXT4;
        // GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
        // GPIO_InitStruct.Pull = GPIO_PULLUP;
        // GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
        // GPIO_InitStruct.Alternate = GPIO_AF8_UART5;
        // HAL_GPIO_Init(RXT4_GPIO_Port, &GPIO_InitStruct);

        /* USER CODE BEGIN UART5_MspInit 1 */

        /* USER CODE END UART5_MspInit 1 */
    } else if (uartHandle->Instance == UART7) {
        /* USER CODE BEGIN UART7_MspInit 0 */

        /* USER CODE END UART7_MspInit 0 */
        /* UART7 clock enable */
        __HAL_RCC_UART7_CLK_ENABLE();

        /**UART7 GPIO Configuration
        PE7     ------> UART7_RX
        PE8     ------> UART7_TX
        */
        // GPIO_InitStruct.Pin = DBG_RX|DBG_TX;
        // GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
        // GPIO_InitStruct.Pull = GPIO_PULLUP;
        // GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
        // GPIO_InitStruct.Alternate = GPIO_AF8_UART7;
        // HAL_GPIO_Init(GPIOE, &GPIO_InitStruct);

        /* USER CODE BEGIN UART7_MspInit 1 */

        /* USER CODE END UART7_MspInit 1 */
    } else if (uartHandle->Instance == UART8) {
        /* USER CODE BEGIN UART8_MspInit 0 */

        /* USER CODE END UART8_MspInit 0 */
        /* UART8 clock enable */
        __HAL_RCC_UART8_CLK_ENABLE();

        /**UART8 GPIO Configuration
        PE0     ------> UART8_RX
        PE1     ------> UART8_TX
        */
        // GPIO_InitStruct.Pin = RX1|TX1;
        // GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
        // GPIO_InitStruct.Pull = GPIO_PULLUP;
        // GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
        // GPIO_InitStruct.Alternate = GPIO_AF8_UART8;
        // HAL_GPIO_Init(GPIOE, &GPIO_InitStruct);

        /* USER CODE BEGIN UART8_MspInit 1 */

        /* USER CODE END UART8_MspInit 1 */
    } else if (uartHandle->Instance == USART1) {
        /* USER CODE BEGIN USART1_MspInit 0 */

        /* USER CODE END USART1_MspInit 0 */
        /* USART1 clock enable */
        __HAL_RCC_USART1_CLK_ENABLE();

        /**USART1 GPIO Configuration
        PB6     ------> USART1_TX
        PB7     ------> USART1_RX
        */
        // GPIO_InitStruct.Pin = Tx2|Rx2;
        // GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
        // GPIO_InitStruct.Pull = GPIO_PULLUP;
        // GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
        // GPIO_InitStruct.Alternate = GPIO_AF7_USART1;
        // HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

        /* USER CODE BEGIN USART1_MspInit 1 */

        /* USER CODE END USART1_MspInit 1 */
    } else if (uartHandle->Instance == USART2) {
        /* USER CODE BEGIN USART2_MspInit 0 */

        /* USER CODE END USART2_MspInit 0 */
        /* USART2 clock enable */
        __HAL_RCC_USART2_CLK_ENABLE();

        /**USART2 GPIO Configuration
        PA0/WKUP     ------> USART2_CTS
        PA3     ------> USART2_RX
        PD4     ------> USART2_RTS
        PD5     ------> USART2_TX
        */
        // GPIO_InitStruct.Pin = GSM_CTS;
        // GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
        // GPIO_InitStruct.Pull = GPIO_NOPULL;
        // GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
        // GPIO_InitStruct.Alternate = GPIO_AF7_USART2;
        // HAL_GPIO_Init(GSM_CTS_GPIO_Port, &GPIO_InitStruct);

        // GPIO_InitStruct.Pin = GSM_RXD;
        // GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
        // GPIO_InitStruct.Pull = GPIO_PULLUP;
        // GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
        // GPIO_InitStruct.Alternate = GPIO_AF7_USART2;
        // HAL_GPIO_Init(GSM_RXD_GPIO_Port, &GPIO_InitStruct);

        // GPIO_InitStruct.Pin = GSM_RTS;
        // GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
        // GPIO_InitStruct.Pull = GPIO_NOPULL;
        // GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
        // GPIO_InitStruct.Alternate = GPIO_AF7_USART2;
        // HAL_GPIO_Init(GSM_RTS_GPIO_Port, &GPIO_InitStruct);

        // GPIO_InitStruct.Pin = GSM_TXD;
        // GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
        // GPIO_InitStruct.Pull = GPIO_PULLUP;
        // GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
        // GPIO_InitStruct.Alternate = GPIO_AF7_USART2;
        // HAL_GPIO_Init(GSM_TXD_GPIO_Port, &GPIO_InitStruct);

        /* USART2 DMA Init */
        /* USART2_RX Init */
        hdma_usart2_rx.Instance = DMA1_Stream5;
        hdma_usart2_rx.Init.Channel = DMA_CHANNEL_4;
        hdma_usart2_rx.Init.Direction = DMA_PERIPH_TO_MEMORY;
        hdma_usart2_rx.Init.PeriphInc = DMA_PINC_DISABLE;
        hdma_usart2_rx.Init.MemInc = DMA_MINC_ENABLE;
        hdma_usart2_rx.Init.PeriphDataAlignment = DMA_PDATAALIGN_BYTE;
        hdma_usart2_rx.Init.MemDataAlignment = DMA_MDATAALIGN_BYTE;
        hdma_usart2_rx.Init.Mode = DMA_CIRCULAR;
        hdma_usart2_rx.Init.Priority = DMA_PRIORITY_HIGH;
        hdma_usart2_rx.Init.FIFOMode = DMA_FIFOMODE_DISABLE;
        if (HAL_DMA_Init(&hdma_usart2_rx) != HAL_OK) {
            // _Error_Handler(__FILE__, __LINE__);
        }

        __HAL_LINKDMA(uartHandle, hdmarx, hdma_usart2_rx);

        /* USART2_TX Init */
        hdma_usart2_tx.Instance = DMA1_Stream6;
        hdma_usart2_tx.Init.Channel = DMA_CHANNEL_4;
        hdma_usart2_tx.Init.Direction = DMA_MEMORY_TO_PERIPH;
        hdma_usart2_tx.Init.PeriphInc = DMA_PINC_DISABLE;
        hdma_usart2_tx.Init.MemInc = DMA_MINC_ENABLE;
        hdma_usart2_tx.Init.PeriphDataAlignment = DMA_PDATAALIGN_BYTE;
        hdma_usart2_tx.Init.MemDataAlignment = DMA_MDATAALIGN_BYTE;
        hdma_usart2_tx.Init.Mode = DMA_NORMAL;
        hdma_usart2_tx.Init.Priority = DMA_PRIORITY_LOW;
        hdma_usart2_tx.Init.FIFOMode = DMA_FIFOMODE_DISABLE;
        if (HAL_DMA_Init(&hdma_usart2_tx) != HAL_OK) {
            // _Error_Handler(__FILE__, __LINE__);
        }

        __HAL_LINKDMA(uartHandle, hdmatx, hdma_usart2_tx);

        /* USER CODE BEGIN USART2_MspInit 1 */

        /* USER CODE END USART2_MspInit 1 */
    }
}

void HAL_UART_MspDeInit(UART_HandleTypeDef *uartHandle)
{

    if (uartHandle->Instance == UART4) {
        /* USER CODE BEGIN UART4_MspDeInit 0 */

        /* USER CODE END UART4_MspDeInit 0 */
        /* Peripheral clock disable */
        __HAL_RCC_UART4_CLK_DISABLE();

        /**UART4 GPIO Configuration
        PC10     ------> UART4_TX
        PC11     ------> UART4_RX
        */
        // HAL_GPIO_DeInit(GPIOC, BUS_TxD0|BUS_RxD0);

        /* UART4 DMA DeInit */
        HAL_DMA_DeInit(uartHandle->hdmarx);
        HAL_DMA_DeInit(uartHandle->hdmatx);
        /* USER CODE BEGIN UART4_MspDeInit 1 */

        /* USER CODE END UART4_MspDeInit 1 */
    } else if (uartHandle->Instance == UART5) {
        /* USER CODE BEGIN UART5_MspDeInit 0 */

        /* USER CODE END UART5_MspDeInit 0 */
        /* Peripheral clock disable */
        __HAL_RCC_UART5_CLK_DISABLE();

        /**UART5 GPIO Configuration
        PC12     ------> UART5_TX
        PD2     ------> UART5_RX
        */
        // HAL_GPIO_DeInit(TXT4_GPIO_Port, TXT4);

        // HAL_GPIO_DeInit(RXT4_GPIO_Port, RXT4);

        /* USER CODE BEGIN UART5_MspDeInit 1 */

        /* USER CODE END UART5_MspDeInit 1 */
    } else if (uartHandle->Instance == UART7) {
        /* USER CODE BEGIN UART7_MspDeInit 0 */

        /* USER CODE END UART7_MspDeInit 0 */
        /* Peripheral clock disable */
        __HAL_RCC_UART7_CLK_DISABLE();

        /**UART7 GPIO Configuration
        PE7     ------> UART7_RX
        PE8     ------> UART7_TX
        */
        // HAL_GPIO_DeInit(GPIOE, DBG_RX|DBG_TX);

        /* USER CODE BEGIN UART7_MspDeInit 1 */

        /* USER CODE END UART7_MspDeInit 1 */
    } else if (uartHandle->Instance == UART8) {
        /* USER CODE BEGIN UART8_MspDeInit 0 */

        /* USER CODE END UART8_MspDeInit 0 */
        /* Peripheral clock disable */
        __HAL_RCC_UART8_CLK_DISABLE();

        /**UART8 GPIO Configuration
        PE0     ------> UART8_RX
        PE1     ------> UART8_TX
        */
        // HAL_GPIO_DeInit(GPIOE, RX1|TX1);

        /* USER CODE BEGIN UART8_MspDeInit 1 */

        /* USER CODE END UART8_MspDeInit 1 */
    } else if (uartHandle->Instance == USART1) {
        /* USER CODE BEGIN USART1_MspDeInit 0 */

        /* USER CODE END USART1_MspDeInit 0 */
        /* Peripheral clock disable */
        __HAL_RCC_USART1_CLK_DISABLE();

        /**USART1 GPIO Configuration
        PB6     ------> USART1_TX
        PB7     ------> USART1_RX
        */
        // HAL_GPIO_DeInit(GPIOB, Tx2|Rx2);

        /* USER CODE BEGIN USART1_MspDeInit 1 */

        /* USER CODE END USART1_MspDeInit 1 */
    } else if (uartHandle->Instance == USART2) {
        /* USER CODE BEGIN USART2_MspDeInit 0 */

        /* USER CODE END USART2_MspDeInit 0 */
        /* Peripheral clock disable */
        __HAL_RCC_USART2_CLK_DISABLE();

        /**USART2 GPIO Configuration
        PA0/WKUP     ------> USART2_CTS
        PA3     ------> USART2_RX
        PD4     ------> USART2_RTS
        PD5     ------> USART2_TX
        */
        // HAL_GPIO_DeInit(GPIOA, GSM_CTS|GSM_RXD);

        // HAL_GPIO_DeInit(GPIOD, GSM_RTS|GSM_TXD);

        /* USART2 DMA DeInit */
        HAL_DMA_DeInit(uartHandle->hdmarx);
        HAL_DMA_DeInit(uartHandle->hdmatx);
        /* USER CODE BEGIN USART2_MspDeInit 1 */

        /* USER CODE END USART2_MspDeInit 1 */
    }
}

/* USER CODE BEGIN 1 */

/* USER CODE END 1 */

/**
  * @}
  */

/**
  * @}
  */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
