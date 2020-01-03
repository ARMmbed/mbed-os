/* mbed Microcontroller Library
 * (C)Copyright TOSHIBA ELECTRONIC DEVICES & STORAGE CORPORATION 2017 All rights reserved
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

#include "TMPM066.h"
#include <stdbool.h>

#define MAXRSTREASON        6

static uint8_t set_bit_count(uint32_t reg);
static uint8_t bit_pos(uint32_t reg);
static bool bit_status(uint32_t reg, uint8_t bit_no);

static reset_reason_t reset_reason1[MAXRSTREASON] = {
    RESET_REASON_POWER_ON,
    RESET_REASON_UNKNOWN,
    RESET_REASON_UNKNOWN,
    RESET_REASON_PIN_RESET,
    RESET_REASON_UNKNOWN,
    RESET_REASON_BROWN_OUT
};

static reset_reason_t reset_reason2[MAXRSTREASON] = {
    RESET_REASON_SOFTWARE,
    RESET_REASON_UNKNOWN,
    RESET_REASON_WATCHDOG
};

void hal_reset_reason_clear(void)
{
    TSB_AOREG->RSTFLG  = 0x00;
    TSB_AOREG->RSTFLG1 = 0x00;
}

uint32_t hal_reset_reason_get_raw(void)
{
    uint32_t ret = 0x00;

    ret = (((TSB_AOREG->RSTFLG1 & 0xFF) << 8) | (TSB_AOREG->RSTFLG & 0xFF));

    return ret;
}

reset_reason_t hal_reset_reason_get(void)
{
    reset_reason_t ret;

    uint8_t NoOfSetBitCountReg1 = set_bit_count(TSB_AOREG->RSTFLG);
    uint8_t NoOfSetBitCountReg2 = set_bit_count(TSB_AOREG->RSTFLG1);

    if (NoOfSetBitCountReg1 != 0x00) {
        if (NoOfSetBitCountReg1 > 0x01) {
            if (bit_status(TSB_AOREG->RSTFLG, 0) && bit_status(TSB_AOREG->RSTFLG, 3)) {
                ret = RESET_REASON_POWER_ON;
            } else {
                ret = RESET_REASON_MULTIPLE;
            }
        } else {
            ret = reset_reason1[bit_pos(TSB_AOREG->RSTFLG)];
        }
    } else if (NoOfSetBitCountReg2 != 0x00) {
        if (NoOfSetBitCountReg2 > 0x01) {
            ret = RESET_REASON_MULTIPLE;
        } else {
            ret = reset_reason2[bit_pos(TSB_AOREG->RSTFLG1)];
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
    cap->reasons |= 1 << RESET_REASON_MULTIPLE;
}

static bool bit_status(uint32_t reg, uint8_t bit_no)
{
    bool status = false;

    if (reg & (1 << bit_no)) {
        status = true;
    }

    return status;
}

static uint8_t set_bit_count(uint32_t reg)
{
    uint8_t count = 0;
    int8_t index  = 0;

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

static uint8_t bit_pos(uint32_t reg)
{
    uint8_t bit_no = 0;

    for (bit_no = 0; bit_no < (sizeof(uint32_t) * 8); bit_no++) {
        if (reg & (1 << bit_no)) {
            return bit_no;
        }
    }
}

#endif // DEVICE_RESET_REASON
