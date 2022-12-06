/* mbed Microcontroller Library
 * Copyright(C) Toshiba Electronic Device Solutions Corporation 2022
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
#include "reset_reason_api.h"

#ifdef DEVICE_RESET_REASON

#include "TMPM4GR.h"
#include <stdbool.h>

static uint8_t set_bit_count(uint32_t reg);

typedef enum {
    POWER_ON_BIT_POS  = 1 << 0,
    PIN_RESET_BIT_POS = 1 << 3,
    LOW_POWER_BIT_POS = 1 << 4,
    BROWN_OUT_BIT_POS = 1 << 5,
} reason_flag0;

typedef enum {
    SOFTWARE_BIT_POS  = 1 << 0,
    LOCK_BIT_POS      = 1 << 1,
    WATCHDOG_BIT_POS  = 1 << 2,
    PLATFORM_BIT_POS  = 1 << 3,
} reason_flag1;

void hal_reset_reason_clear(void)
{
    TSB_RLM->RSTFLG0 = 0;
    TSB_RLM->RSTFLG1 = 0;
}

uint32_t hal_reset_reason_get_raw(void)
{
    uint32_t ret = 0;

    ret = (((TSB_RLM->RSTFLG1 & 0xFF) << 8) | (TSB_RLM->RSTFLG0 & 0xFF));

    return ret;
}

reset_reason_t hal_reset_reason_get(void)
{
    reset_reason_t ret;

    uint8_t NoOfSetBitCountReg1 = set_bit_count(TSB_RLM->RSTFLG0);
    uint8_t NoOfSetBitCountReg2 = set_bit_count(TSB_RLM->RSTFLG1);

    if (NoOfSetBitCountReg1 != 0x00) {
        if (NoOfSetBitCountReg1 > 0x01) {
            ret = RESET_REASON_MULTIPLE;
        } else {
            if (TSB_RLM->RSTFLG0 & POWER_ON_BIT_POS) {
                ret = RESET_REASON_POWER_ON;
            } else if (TSB_RLM->RSTFLG0 & PIN_RESET_BIT_POS) {
                ret = RESET_REASON_PIN_RESET;
            } else if (TSB_RLM->RSTFLG0 & LOW_POWER_BIT_POS) {
                ret = RESET_REASON_WAKE_LOW_POWER;
            } else if (TSB_RLM->RSTFLG0 & BROWN_OUT_BIT_POS) {
                ret = RESET_REASON_BROWN_OUT;
            } else {
                ret = RESET_REASON_UNKNOWN;
            }
        }
    } else if (NoOfSetBitCountReg2 != 0x00) {
        if (NoOfSetBitCountReg2 > 0x01) {
            ret = RESET_REASON_MULTIPLE;
        } else {
            if (TSB_RLM->RSTFLG1 & SOFTWARE_BIT_POS) {
                ret = RESET_REASON_SOFTWARE;
            } else if (TSB_RLM->RSTFLG1 & LOCK_BIT_POS) {
                ret = RESET_REASON_LOCKUP;
            } else if (TSB_RLM->RSTFLG1 & WATCHDOG_BIT_POS) {
                ret = RESET_REASON_WATCHDOG;
            } else if (TSB_RLM->RSTFLG1 & PLATFORM_BIT_POS) {
                ret = RESET_REASON_PLATFORM;
            } else {
                ret = RESET_REASON_UNKNOWN;
            }
        }
    } else {
        ret = RESET_REASON_UNKNOWN;
    }

    return ret;
}

void hal_reset_reason_get_capabilities(reset_reason_capabilities_t *cap)
{
    cap->reasons = 1 << RESET_REASON_UNKNOWN;
    cap->reasons |= 1 << RESET_REASON_POWER_ON;
    cap->reasons |= 1 << RESET_REASON_PIN_RESET;
    cap->reasons |= 1 << RESET_REASON_WAKE_LOW_POWER;
    cap->reasons |= 1 << RESET_REASON_BROWN_OUT;
    cap->reasons |= 1 << RESET_REASON_SOFTWARE;
    cap->reasons |= 1 << RESET_REASON_LOCKUP;
    cap->reasons |= 1 << RESET_REASON_WATCHDOG;
    cap->reasons |= 1 << RESET_REASON_PLATFORM;
    cap->reasons |= 1 << RESET_REASON_MULTIPLE;

}

static uint8_t set_bit_count(uint32_t reg)
{
    uint8_t count = 0;
    uint8_t index = 0;

    for (index = 0; index < (sizeof(uint32_t) * 8); index++) {
        if (reg & (1 << index)) {
            count++;
            if (count > 0x01) {
                break;
            }
        }
    }

    return count;
}

#endif // DEVICE_RESET_REASON
