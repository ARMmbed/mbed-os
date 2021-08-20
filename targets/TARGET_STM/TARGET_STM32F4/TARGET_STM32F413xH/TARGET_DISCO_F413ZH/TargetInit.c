/* mbed Microcontroller Library
 * SPDX-License-Identifier: BSD-3-Clause
 ******************************************************************************
 *
 * Copyright (c) 2015-2021 STMicroelectronics.
 * All rights reserved.
 *
 * This software component is licensed by ST under BSD 3-Clause license,
 * the "License"; You may not use this file except in compliance with the
 * License. You may obtain a copy of the License at:
 *                        opensource.org/licenses/BSD-3-Clause
 *
 ******************************************************************************
 */

#include "stm32f4xx.h"

void TargetBSP_Init(void)
{
    /* In DISCO_F413ZH board, Arduino connector and Wifi embeded module are sharing the same SPI pins */
    /* We need to set the default SPI SS pin for the Wifi module to the inactive state i.e. 1 */
    /* See board User Manual: WIFI_SPI_CS = PG_11*/
    __HAL_RCC_GPIOG_CLK_ENABLE();
    GPIO_InitTypeDef  GPIO_InitStruct;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FAST;
    GPIO_InitStruct.Pin = GPIO_PIN_11;
    HAL_GPIO_Init(GPIOG, &GPIO_InitStruct);
    HAL_GPIO_WritePin(GPIOG, GPIO_PIN_11, GPIO_PIN_SET);
}

