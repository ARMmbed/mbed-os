/* mbed Microcontroller Library
 * Copyright(C) Toshiba Electronic Device Solutions Corporation 2020
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
#include "device.h"
#include "mbed_error.h"
#include "PeripheralNames.h"
#include "pinmap.h"
#include "reset_reason_api.h"

#define CLEAR_FLAGS     (0)
//reset reason flag bit positions
#define PORSTF_FLAG     (1<<0)
#define PINRSTF_FLAG    (1<<3)
#define LVDRSTF_FLAG    (1<<5)

#define SYSRSTF_FLAG    (1<<0)
#define LOCKRSTF_FLAG   (1<<1)
#define WDTRSTF_FLAG    (1<<2)
#define OFDRSTF_FLAG    (1<<3)


reset_reason_t hal_reset_reason_get(void)
{
    reset_reason_t reason = RESET_REASON_UNKNOWN;

    //check if its power on reset, as it clears other flags to 0 or undefined state.
    if (TSB_RLM->RSTFLG0 & PORSTF_FLAG) {
        //set PINRSTF_FLAG and LVDRSTF_FLAG flags to 0 as they may be in undefined state after POR
        TSB_RLM->RSTFLG0 &= ~(PINRSTF_FLAG | LVDRSTF_FLAG);
        reason = RESET_REASON_POWER_ON;
    } else {
        //multiple reset reasons might occur if flags are not cleared in previous reset
        //hence check all flags.

        if (TSB_RLM->RSTFLG0 & PINRSTF_FLAG) {
            reason = RESET_REASON_PIN_RESET;
        }

        if (TSB_RLM->RSTFLG0 & LVDRSTF_FLAG) {
            reason = (reason == RESET_REASON_UNKNOWN) ? RESET_REASON_BROWN_OUT : RESET_REASON_MULTIPLE;
        }

        // check 2nd reset reason register

        if (TSB_RLM->RSTFLG1 & SYSRSTF_FLAG) {
            reason = (reason == RESET_REASON_UNKNOWN) ? RESET_REASON_SOFTWARE : RESET_REASON_MULTIPLE;
        }

        if (TSB_RLM->RSTFLG1 & LOCKRSTF_FLAG) {
            reason = (reason == RESET_REASON_UNKNOWN) ? RESET_REASON_LOCKUP : RESET_REASON_MULTIPLE;
        }

        if (TSB_RLM->RSTFLG1 & WDTRSTF_FLAG) {
            reason = (reason == RESET_REASON_UNKNOWN) ? RESET_REASON_WATCHDOG : RESET_REASON_MULTIPLE;
        }

        if (TSB_RLM->RSTFLG1 & OFDRSTF_FLAG) {
            reason = (reason == RESET_REASON_UNKNOWN) ? RESET_REASON_PLATFORM : RESET_REASON_MULTIPLE;
        }
    }

    return reason;
}

void hal_reset_reason_get_capabilities(reset_reason_capabilities_t *cap)
{
    cap->reasons = 1 << RESET_REASON_UNKNOWN;
    cap->reasons |= 1 << RESET_REASON_POWER_ON;
    cap->reasons |= 1 << RESET_REASON_PIN_RESET;
    cap->reasons |= 1 << RESET_REASON_BROWN_OUT;
    cap->reasons |= 1 << RESET_REASON_SOFTWARE;
    cap->reasons |= 1 << RESET_REASON_LOCKUP;
    cap->reasons |= 1 << RESET_REASON_WATCHDOG;
    cap->reasons |= 1 << RESET_REASON_PLATFORM;
    cap->reasons |= 1 << RESET_REASON_MULTIPLE;
}

uint32_t hal_reset_reason_get_raw(void)
{
    uint32_t reason_raw = 0;
    //check if its power on reset, as it clears other flags to 0 or undefined state.
    if (TSB_RLM->RSTFLG0 & PORSTF_FLAG) {
        //set PINRSTF_FLAG and LVDRSTF_FLAG flags to 0 as they may be in undefined state after POR
        TSB_RLM->RSTFLG0 &= ~(PINRSTF_FLAG | LVDRSTF_FLAG);
    }

    //concatenating RSTFLG1 and RSTFLG0 register contents
    reason_raw = ((TSB_RLM->RSTFLG1) << 8) | TSB_RLM->RSTFLG0 ;
    return reason_raw;
}



void hal_reset_reason_clear(void)
{
    //clear both reset reason registers RSTFLG1 and RSTFLG0
    TSB_RLM->RSTFLG0 = CLEAR_FLAGS;
    TSB_RLM->RSTFLG1 = CLEAR_FLAGS;
}


