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
#include "cmsis.h"
#include "nrf_error.h"
#include "nrf_sdm.h"
#include "nrf_soc.h"

#include <stdbool.h>
#include <stdint.h>

static union {
    uint32_t _PRIMASK_state;
    uint8_t _sd_state;
} _state = {0};

static bool _use_softdevice_routine = false;
static bool _state_saved = false;

void hal_critical_section_enter(void)
{
    // Fetch the current state of interrupts
    uint32_t primask = __get_PRIMASK();
    uint8_t temp_state = 0;

    // If interrupts are enabled, try to use the soft device
    uint8_t sd_enabled;
    if ((primask == 0) &&
        (sd_softdevice_is_enabled(&sd_enabled) == NRF_SUCCESS) &&
        (sd_enabled == 1)) {
        // If the softdevice can be used, use it.
        sd_nvic_critical_region_enter(&temp_state);
        _use_softdevice_routine = true;

        if (_state_saved == false) {
            _state._sd_state = temp_state;
        }
    } else {
        // If interrupts are enabled, disable them.
        if (primask == 0) {
            __disable_irq();
        }

        // Store PRIMASK state, it will be restored when exiting critical
        // section.
        _use_softdevice_routine = false;

        if (_state_saved == false) {
            _state._PRIMASK_state = primask;
        }
    }

    _state_saved = true;
}

void hal_critical_section_exit(void)
{
    _state_saved = false;

    // Restore the state as it was prior to entering the critical section.
    if (_use_softdevice_routine) {
        sd_nvic_critical_region_exit(_state._sd_state);
    } else {
        __set_PRIMASK(_state._PRIMASK_state);
    }
}

bool hal_in_critical_section(void)
{
    return (_state_saved == true);
}
