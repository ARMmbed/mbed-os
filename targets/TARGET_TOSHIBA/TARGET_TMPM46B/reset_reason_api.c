/* mbed Microcontroller Library
 * (C)Copyright TOSHIBA ELECTRONIC DEVICES & STORAGE CORPORATION 2018 All rights reserved
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

#include "TMPM46B.h"
#include <stdbool.h>

static uint8_t set_bit_count(uint32_t reg);
static uint8_t bit_pos(uint32_t reg);

static reset_reason_t reset_reason[7] = {
    RESET_REASON_PIN_RESET,
    RESET_REASON_UNKNOWN,
    RESET_REASON_WATCHDOG,
    RESET_REASON_WAKE_LOW_POWER,
    RESET_REASON_SOFTWARE,
    RESET_REASON_UNKNOWN,
    RESET_REASON_BROWN_OUT
};

void hal_reset_reason_clear(void)
{
    TSB_CG->RSTFLG = 0x00;
}

uint32_t hal_reset_reason_get_raw(void)
{
    uint32_t ret = 0;

    ret = TSB_CG->RSTFLG;

    return ret;
}

reset_reason_t hal_reset_reason_get(void)
{
    reset_reason_t ret;

    uint8_t NoOfSetBitCountReg = set_bit_count(TSB_CG->RSTFLG);

    if (NoOfSetBitCountReg != 0x00) {
        if (NoOfSetBitCountReg > 0x01) {
            ret = RESET_REASON_MULTIPLE;
        } else {
            ret = reset_reason[bit_pos(TSB_CG->RSTFLG)];
        }
    } else {
        ret = RESET_REASON_UNKNOWN;
    }

    return ret;
}

void hal_reset_reason_get_capabilities(reset_reason_capabilities_t *cap)
{
    cap->reasons = 1 << RESET_REASON_UNKNOWN;
    cap->reasons |= 1 << RESET_REASON_MULTIPLE;
}

static uint8_t set_bit_count(uint32_t reg)
{
    uint8_t count = 0;
    uint8_t index = 0;

    for (index = 0; index < (sizeof(uint32_t) * 8); index++) {
        if ((reg & (1 << index)) && index != 1) {
            count++;
        }
    }

    return count;
}

static uint8_t bit_pos(uint32_t reg)
{
    uint8_t bit_no = 0;

    for (bit_no = 0; bit_no < (sizeof(uint32_t) * 8); bit_no++) {
        if ((reg & (1 << bit_no)) && bit_no != 1) {
            return bit_no;
        }
    }

    return 0;
}

#endif // DEVICE_RESET_REASON
