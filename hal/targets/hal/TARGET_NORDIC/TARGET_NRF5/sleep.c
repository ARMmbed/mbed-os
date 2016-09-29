/* mbed Microcontroller Library
 * Copyright (c) 2006-2013 ARM Limited
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
#include "sleep_api.h"
#include "cmsis.h"
#include "mbed_interface.h"
#include "nrf_soc.h"
#include "nrf_sdm.h"

// Mask of reserved bits of the register ICSR in the System Control Block peripheral
// In this case, bits which are equal to 0 are the bits reserved in this register
#define SCB_ICSR_RESERVED_BITS_MASK     0x9E43F03F

void sleep(void)
{
    // ensure debug is disconnected if semihost is enabled....

    SCB->SCR |= SCB_SCR_SEVONPEND_Msk; /* send an event when an interrupt is pending.
                                        * This helps with the wakeup from the following app_evt_wait(). */

    uint8_t sd_enabled;

    // look if exceptions are enabled or not, if they are, it is possible to make an SVC call
    // and check if the soft device is running
    if ((__get_PRIMASK() == 0) && (sd_softdevice_is_enabled(&sd_enabled) == NRF_SUCCESS) && (sd_enabled == 1)) {
        // soft device is enabled, use the primitives from the soft device to go to sleep
        sd_power_mode_set(NRF_POWER_MODE_LOWPWR);
        sd_app_evt_wait();
    } else {
        NRF_POWER->TASKS_LOWPWR = 1;

        // Note: it is not possible to just use WFE at this stage because WFE
        // will read the event register (not accessible) and if an event occured,
        // in the past, it will just clear the event register and continue execution.
        // SVC call like sd_softdevice_is_enabled set the event register to 1.
        // This means that a making an SVC call followed by WFE will never put the
        // CPU to sleep.
        // Our startegy here is to clear the event register then, if there is any
        // interrupt, return from here. If no interrupts are pending, just call
        // WFE.

        // Set an event and wake up whatsoever, this will clear the event
        // register from all previous events set (SVC call included)
        __SEV();
        __WFE();

        // Test if there is an interrupt pending (mask reserved regions)
        if (SCB->ICSR & (SCB_ICSR_RESERVED_BITS_MASK)) {
            // Ok, there is an interrut pending, no need to go to sleep
            return;
        } else {
            // next event will wakeup the CPU
            // If an interrupt occured between the test of SCB->ICSR and this
            // instruction, WFE will just not put the CPU to sleep
            __WFE();
        }
    }
}

void deepsleep(void)
{
    sleep();
    //   NRF_POWER->SYSTEMOFF=1;
}
