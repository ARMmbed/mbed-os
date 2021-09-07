/*
 * Copyright 2020 Arduino SA
 * SPDX-License-Identifier: Apache-2.0
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

/** @file
 *  Provides configuration for WHD driver on Arduino Portenta H7
 */

#ifndef __WHD_CONFIG__
#define __WHD_CONFIG__

#include "whd_types.h"
#include "stm32h7xx_hal.h"
#include <stdint.h>

/* please define your configuration , either SDIO or SPI */
#define CY_WHD_CONFIG_USE_SDIO
//#define CY_WHD_CONFIG_USE_SPI

/* select resource implementation */
#define USES_RESOURCE_GENERIC_FILESYSTEM

/* if not defined default value is 2 */
#define CY_WIFI_OOB_INTR_PRIORITY 0

#define CYBSP_WIFI_HOST_WAKE_IRQ_EVENT CYHAL_GPIO_IRQ_FALL
#define CYBSP_WIFI_HOST_WAKE CYBSP_SDIO_OOB_IRQ

#define BSP_LED1            {GPIOK,{.Pin= GPIO_PIN_5 , .Mode = GPIO_MODE_OUTPUT_PP , .Pull = GPIO_NOPULL , .Speed= GPIO_SPEED_FREQ_LOW}}
#define BSP_LED2            {GPIOK,{.Pin= GPIO_PIN_6 , .Mode = GPIO_MODE_OUTPUT_PP , .Pull = GPIO_NOPULL , .Speed= GPIO_SPEED_FREQ_LOW}}
#define BSP_LED3            {GPIOK,{.Pin= GPIO_PIN_7 , .Mode = GPIO_MODE_OUTPUT_PP , .Pull = GPIO_NOPULL , .Speed= GPIO_SPEED_FREQ_LOW}}

/* power pin */
#define WIFI_WL_REG_ON      {GPIOJ,{.Pin= GPIO_PIN_1, .Mode = GPIO_MODE_OUTPUT_PP , .Pull = GPIO_NOPULL , .Speed= GPIO_SPEED_FREQ_LOW}}
//#define WIFI_32K_CLK      {GPIOA,{.Pin= GPIO_PIN_8, .Mode = GPIO_MODE_AF_PP , .Pull = GPIO_NOPULL , .Speed= GPIO_SPEED_FREQ_LOW , .Alternate = GPIO_AF0_MCO}}

#define WIFI_SDIO_CMD       {GPIOD,{.Pin= GPIO_PIN_2 , .Mode = GPIO_MODE_AF_PP , .Pull = GPIO_NOPULL , .Speed= GPIO_SPEED_FREQ_VERY_HIGH, .Alternate = GPIO_AF12_SDIO1}}
#define WIFI_SDIO_CLK       {GPIOC,{.Pin= GPIO_PIN_12, .Mode = GPIO_MODE_AF_PP , .Pull = GPIO_NOPULL , .Speed= GPIO_SPEED_FREQ_VERY_HIGH, .Alternate = GPIO_AF12_SDIO1}}
#define WIFI_SDIO_D0        {GPIOC,{.Pin= GPIO_PIN_8 , .Mode = GPIO_MODE_AF_PP , .Pull = GPIO_NOPULL , .Speed= GPIO_SPEED_FREQ_VERY_HIGH, .Alternate = GPIO_AF12_SDIO1}}
#define WIFI_SDIO_D1        {GPIOC,{.Pin= GPIO_PIN_9 , .Mode = GPIO_MODE_AF_PP , .Pull = GPIO_NOPULL , .Speed= GPIO_SPEED_FREQ_VERY_HIGH, .Alternate = GPIO_AF12_SDIO1}}
#define WIFI_SDIO_D2        {GPIOC,{.Pin= GPIO_PIN_10, .Mode = GPIO_MODE_AF_PP , .Pull = GPIO_NOPULL , .Speed= GPIO_SPEED_FREQ_VERY_HIGH, .Alternate = GPIO_AF12_SDIO1}}
#define WIFI_SDIO_D3        {GPIOC,{.Pin= GPIO_PIN_11, .Mode = GPIO_MODE_AF_PP , .Pull = GPIO_NOPULL , .Speed= GPIO_SPEED_FREQ_VERY_HIGH, .Alternate = GPIO_AF12_SDIO1}}
#define WIFI_SDIO_OOB_IRQ   {GPIOJ,{.Pin= GPIO_PIN_5,  .Mode = GPIO_MODE_IT_FALLING , .Pull = GPIO_NOPULL , .Speed= GPIO_SPEED_FREQ_VERY_HIGH}}

#endif
