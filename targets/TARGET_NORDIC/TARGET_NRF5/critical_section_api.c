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

#include <stdbool.h>
#include <stdint.h> // uint32_t, UINT32_MAX

static uint8_t _sd_state = 0;
static volatile bool _state_saved = false;

void hal_critical_section_enter(void)
{
    uint8_t temp_state = 0;
    sd_nvic_critical_region_enter(&temp_state);

    if (_state_saved == true) {
        return;
    }

    _sd_state = temp_state;
    _state_saved = true;
}

void hal_critical_section_exit(void)
{
    _state_saved = false;

    sd_nvic_critical_region_exit(_sd_state);
}
