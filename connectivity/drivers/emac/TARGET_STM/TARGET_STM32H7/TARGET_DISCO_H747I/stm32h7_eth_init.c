/* mbed Microcontroller Library
 * Copyright (c) 2018, STMicroelectronics
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
 */

#define ETHERNET 1
#if (MBED_CONF_TARGET_NETWORK_DEFAULT_INTERFACE_TYPE == ETHERNET)
#if !defined(DISCO_H747I_ETHERNET_SOLDERBRIGE)
#error Hardware modifications are required for Ethernet on DISCO_H747I. see https://os.mbed.com/teams/ST/wiki/DISCO_H747I-modifications-for-Ethernet
#endif
#endif

#ifndef USE_USER_DEFINED_HAL_ETH_MSPINIT

#include "stm32h7xx_hal.h"

#define ETH_TX_EN_Pin GPIO_PIN_11
#define ETH_TX_EN_GPIO_Port GPIOG
#define ETH_TXD1_Pin GPIO_PIN_12
#define ETH_TXD1_GPIO_Port GPIOG
#define ETH_TXD0_Pin GPIO_PIN_13
#define ETH_TXD0_GPIO_Port GPIOG
#define ETH_MDC_SAI4_D1_Pin GPIO_PIN_1
#define ETH_MDC_SAI4_D1_GPIO_Port GPIOC
#define ETH_MDIO_Pin GPIO_PIN_2
#define ETH_MDIO_GPIO_Port GPIOA
#define ETH_REF_CLK_Pin GPIO_PIN_1
#define ETH_REF_CLK_GPIO_Port GPIOA
#define ETH_CRS_DV_Pin GPIO_PIN_7
#define ETH_CRS_DV_GPIO_Port GPIOA
#define ETH_RXD0_Pin GPIO_PIN_4
#define ETH_RXD0_GPIO_Port GPIOC
#define ETH_RXD1_Pin GPIO_PIN_5
#define ETH_RXD1_GPIO_Port GPIOC


/**
 * Override HAL Eth Init function
 */
void HAL_ETH_MspInit(ETH_HandleTypeDef *heth)
{
    GPIO_InitTypeDef GPIO_InitStruct;
    if (heth->Instance == ETH) {
#if defined(CORE_CM7)
        /* Disable DCache for STM32H7 family */
        SCB_DisableDCache();
#endif

        /* GPIO Ports Clock Enable */
        __HAL_RCC_GPIOA_CLK_ENABLE();
        // __HAL_RCC_GPIOB_CLK_ENABLE();
        __HAL_RCC_GPIOC_CLK_ENABLE();
        __HAL_RCC_GPIOG_CLK_ENABLE();
        // __HAL_RCC_GPIOH_CLK_ENABLE();

        /* Enable Peripheral clock */
        __HAL_RCC_ETH1MAC_CLK_ENABLE();
        __HAL_RCC_ETH1TX_CLK_ENABLE();
        __HAL_RCC_ETH1RX_CLK_ENABLE();

        /**ETH GPIO Configuration
        PG11     ------> ETH_TX_EN
        PG12     ------> ETH_TXD1
        PG13     ------> ETH_TXD0
        PC1     ------> ETH_MDC
        PA2     ------> ETH_MDIO
        PA1     ------> ETH_REF_CLK
        PA7     ------> ETH_CRS_DV
        PC4     ------> ETH_RXD0
        PC5     ------> ETH_RXD1
        */
        GPIO_InitStruct.Pin = ETH_TX_EN_Pin | ETH_TXD1_Pin | ETH_TXD0_Pin;
        GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
        GPIO_InitStruct.Pull = GPIO_NOPULL;
        GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
        GPIO_InitStruct.Alternate = GPIO_AF11_ETH;
        HAL_GPIO_Init(GPIOG, &GPIO_InitStruct);

        GPIO_InitStruct.Pin = ETH_MDC_SAI4_D1_Pin | ETH_RXD0_Pin | ETH_RXD1_Pin;
        GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
        GPIO_InitStruct.Pull = GPIO_NOPULL;
        GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
        GPIO_InitStruct.Alternate = GPIO_AF11_ETH;
        HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

        GPIO_InitStruct.Pin = ETH_MDIO_Pin | ETH_REF_CLK_Pin | ETH_CRS_DV_Pin;
        GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
        GPIO_InitStruct.Pull = GPIO_NOPULL;
        GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
        GPIO_InitStruct.Alternate = GPIO_AF11_ETH;
        HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

    }
}

/**
 * Override HAL Eth DeInit function
 */
void HAL_ETH_MspDeInit(ETH_HandleTypeDef *heth)
{
    if (heth->Instance == ETH) {
        /* Peripheral clock disable */
        __HAL_RCC_ETH1MAC_CLK_DISABLE();
        __HAL_RCC_ETH1TX_CLK_DISABLE();
        __HAL_RCC_ETH1RX_CLK_DISABLE();

        /**ETH GPIO Configuration
        PG11     ------> ETH_TX_EN
        PG12     ------> ETH_TXD1
        PG13     ------> ETH_TXD0
        PC1     ------> ETH_MDC
        PA2     ------> ETH_MDIO
        PA1     ------> ETH_REF_CLK
        PA7     ------> ETH_CRS_DV
        PC4     ------> ETH_RXD0
        PC5     ------> ETH_RXD1
        */
        HAL_GPIO_DeInit(GPIOG, ETH_TX_EN_Pin | ETH_TXD1_Pin | ETH_TXD0_Pin);

        HAL_GPIO_DeInit(GPIOC, ETH_MDC_SAI4_D1_Pin | ETH_RXD0_Pin | ETH_RXD1_Pin);

        HAL_GPIO_DeInit(GPIOA, ETH_MDIO_Pin | ETH_REF_CLK_Pin | ETH_CRS_DV_Pin);
    }
}

#endif /* USE_USER_DEFINED_HAL_ETH_MSPINIT */
