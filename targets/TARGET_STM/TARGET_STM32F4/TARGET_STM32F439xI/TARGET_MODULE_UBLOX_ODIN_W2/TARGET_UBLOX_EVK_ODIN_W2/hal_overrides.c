/*---------------------------------------------------------------------------
 * Copyright (c) 2017, u-blox Malmö, All Rights Reserved
 * SPDX-License-Identifier: LicenseRef-PBL
 *
 * This file and the related binary are licensed under the
 * Permissive Binary License, Version 1.0 (the "License");
 * you may not use these files except in compliance with the License.
 *
 * You may obtain a copy of the License here:
 * LICENSE-permissive-binary-license-1.0.txt and at
 * https://www.mbed.com/licenses/PBL-1.0
 *
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 * Component   : HAL
 * File        : hal_overrides.c
 *
 * Description : Placeholder for HAL overrides.
 *-------------------------------------------------------------------------*/

#include "stm32f4xx_hal.h"
#include "stm32f4xx_hal_rcc.h"
#include "stm32f4xx_hal_gpio.h"

void HAL_MspInit(void)
{
    __HAL_RCC_GPIOB_CLK_ENABLE();
    __HAL_RCC_GPIOE_CLK_ENABLE();

    GPIO_InitTypeDef GPIO_InitDef;

    GPIO_InitDef.Pin = GPIO_PIN_6 | GPIO_PIN_8;
    GPIO_InitDef.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitDef.Pull = GPIO_NOPULL;
    GPIO_InitDef.Speed = GPIO_SPEED_FREQ_HIGH;
    HAL_GPIO_Init(GPIOB, &GPIO_InitDef);

    GPIO_InitDef.Pin = GPIO_PIN_0;
    HAL_GPIO_Init(GPIOE, &GPIO_InitDef);

    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_6, GPIO_PIN_SET);
    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_8, GPIO_PIN_SET);
    HAL_GPIO_WritePin(GPIOE, GPIO_PIN_0, GPIO_PIN_SET);
}
