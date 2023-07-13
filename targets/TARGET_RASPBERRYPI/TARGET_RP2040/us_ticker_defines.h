/* mbed Microcontroller Library
 * Copyright (c) 2006-2023 ARM Limited
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

#ifndef MBED_OS_US_TICKER_DEFINES_H
#define MBED_OS_US_TICKER_DEFINES_H

#include "hardware/timer.h"

// Provide a macro-optimized version of us_ticker_read()
#define us_ticker_read() ((timer_hw_t *)TIMER_BASE)->timerawl

#endif //MBED_OS_US_TICKER_DEFINES_H
