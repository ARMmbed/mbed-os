/* mbed Microcontroller Library
 * Copyright (c) 2006-2019 ARM Limited
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
#ifndef MBED_US_TICKER_DEFINES_H_
#define MBED_US_TICKER_DEFINES_H_

#include "us_ticker_data.h"

/* All devices have 1MHz us_ticker */
#define US_TICKER_PERIOD_NUM    1
#define US_TICKER_PERIOD_DEN    1

#if TIM_MST_BIT_WIDTH == 16
#define US_TICKER_MASK          0xFFFF
#elif TIM_MST_BIT_WIDTH == 32
#define US_TICKER_MASK          0xFFFFFFFF
#else
#error "Bad TIM_MST_BIT_WIDTH"
#endif

/* Macro-optimised form of us_ticker_read */
#define us_ticker_read() (TIM_MST->CNT)

#endif /* MBED_US_TICKER_DEFINES_H_ */
