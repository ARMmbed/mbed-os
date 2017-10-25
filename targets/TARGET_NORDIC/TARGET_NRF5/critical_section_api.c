/*
 * Copyright (c) 2015-2017, ARM Limited, All Rights Reserved
 * SPDX-License-Identifier: Apache-2.0
 *
 * Licensed under the Apache License, Version 2.0 (the "License"); you may
 * not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS, WITHOUT
 * WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
#include "nrf_nvic.h"

#include <stdint.h> // uint32_t, UINT32_MAX

static uint8_t _sd_state = 0;

void hal_critical_section_enter(void)
{
    sd_nvic_critical_region_enter(&_sd_state);
}

void hal_critical_section_exit(void)
{
    sd_nvic_critical_region_exit(_sd_state);
}
