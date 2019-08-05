/*************************************************************************************************/
/*!
 *  \file
 *
 *  \brief      Audio board definition.
 *
 *  Copyright (c) 2018-2019 Arm Ltd. All Rights Reserved.
 *  Arm Ltd. confidential and proprietary.
 *
 *  Licensed under the Apache License, Version 2.0 (the "License");
 *  you may not use this file except in compliance with the License.
 *  You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 *  Unless required by applicable law or agreed to in writing, software
 *  distributed under the License is distributed on an "AS IS" BASIS,
 *  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *  See the License for the specific language governing permissions and
 *  limitations under the License.
 */
/*************************************************************************************************/

#ifndef AUDIOBOARD_H
#define AUDIOBOARD_H

#include "stack/platform/include/pal_types.h"
#include "nrf_gpio.h"

#ifdef __cplusplus
extern "C" {
#endif

/**************************************************************************************************
  Macros
**************************************************************************************************/

#define AUDIO_LED_1          NRF_GPIO_PIN_MAP(1,1)
#define AUDIO_LED_2          NRF_GPIO_PIN_MAP(1,2)
#define AUDIO_LED_3          NRF_GPIO_PIN_MAP(1,3)
#define AUDIO_LED_4          NRF_GPIO_PIN_MAP(1,4)
#define AUDIO_LED_5          NRF_GPIO_PIN_MAP(1,5)
#define AUDIO_LED_6          NRF_GPIO_PIN_MAP(1,6)

#ifdef __cplusplus
};
#endif

#endif /* AUDIOBOARD_H */
