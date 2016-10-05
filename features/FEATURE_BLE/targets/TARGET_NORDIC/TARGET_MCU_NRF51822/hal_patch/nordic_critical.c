/*
 * Copyright (c) 2015-2016, ARM Limited, All Rights Reserved
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

#include <stdint.h>                  // uint32_t, UINT32_MAX
#include <assert.h>                  // uint32_t, UINT32_MAX
#include "cmsis.h"
#include "nrf_soc.h"
#include "nrf_sdm.h"

static union {
    uint32_t _PRIMASK_state;
    uint8_t  _sd_state;
} _state = { 0 } ;
static volatile uint32_t _entry_count = 0;
static bool _use_softdevice_routine = false;

void core_util_critical_section_enter()
{
    // if a critical section has already been entered, just update the counter
    if (_entry_count) {
        ++_entry_count;
        return;
    }

    // in this path, a critical section has never been entered
    uint32_t primask = __get_PRIMASK();

    // if interrupts are enabled, try to use the soft device
    uint8_t sd_enabled;
    if ((primask == 0) && (sd_softdevice_is_enabled(&sd_enabled) == NRF_SUCCESS) && sd_enabled == 1) {
        // if the soft device can be use, use it
        sd_nvic_critical_region_enter(&_state._sd_state);
        _use_softdevice_routine = true;
    } else {
        // if interrupts where enabled, disable them
        if(primask == 0) {
            __disable_irq();
        }

        // store the PRIMASK state, it will be restored at the end of the critical section
        _state._PRIMASK_state = primask;
        _use_softdevice_routine = false;
    }

    assert(_entry_count == 0); // entry count should always be equal to 0 at this point
    ++_entry_count;
}

void core_util_critical_section_exit()
{
    assert(_entry_count > 0);
    --_entry_count;

    // If their is other segments which have entered the critical section, just leave
    if (_entry_count) {
        return;
    }

    // This is the last segment of the critical section, state should be restored as before entering
    // the critical section
    if (_use_softdevice_routine) {
        sd_nvic_critical_region_exit(_state._sd_state);
    } else {
        __set_PRIMASK(_state._PRIMASK_state);
    }
}
