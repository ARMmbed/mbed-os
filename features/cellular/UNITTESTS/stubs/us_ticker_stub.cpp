/*
 * Copyright (c) , Arm Limited and affiliates.
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


#include "stdlib.h"
#include "us_ticker_api.h"

const ticker_data_t* get_us_ticker_data(void)
{
    return NULL;
}

void us_ticker_irq_handler(void)
{
}

void us_ticker_init(void)
{
}

uint32_t us_ticker_read(void)
{
    return 0;
}

void us_ticker_set_interrupt(timestamp_t timestamp)
{
}

void us_ticker_disable_interrupt(void)
{
}

void us_ticker_clear_interrupt(void)
{
}

void us_ticker_fire_interrupt(void)
{
}
