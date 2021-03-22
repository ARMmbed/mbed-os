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

/**
  * @brief  Sets F469 "Awake" signal (PK3 pin) to turn on daughterboard power supplies
  * @param  None
  * @retval None
  *
  */
void TargetBSP_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStruct;

    /* Enable peripheral clock */
    __HAL_RCC_GPIOK_CLK_ENABLE();

    /* GPIO Configuration */
    GPIO_InitStruct.Pin = GPIO_PIN_3;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    HAL_GPIO_Init(GPIOK, &GPIO_InitStruct);

    /* Enable AWAKE pin */
    HAL_GPIO_WritePin(GPIOK, GPIO_PIN_3, GPIO_PIN_SET);
}
