/****************************************************************************
 *
 * Copyright 2020 Samsung Electronics All Rights Reserved.
 * SPDX-License-Identifier: Apache-2.0
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing,
 * software distributed under the License is distributed on an
 * "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND,
 * either express or implied. See the License for the specific
 * language governing permissions and limitations under the License.
 *
 ****************************************************************************/

#include <stdbool.h>
#include "s1sbp6a.h"
#include "s1sbp6a_type.h"
#include "s1sbp6a_rtc.h"
#include "mbed_debug.h"

static uint32_t grtc_load_delay = 1500;

static void bp6a_rtc_unlock(bool unlock)
{
    BP_SYSCON->KA_ACCESS = unlock;
}

int bp6a_rtc_bcd2bin(uint8_t bcd)
{
    return ((bcd & 0xF0u) >> 4u) * 10 + (bcd & 0x0Fu);
}

uint32_t bp6a_rtc_bin2bcd(uint8_t bin)
{
    return ((bin / 10u) << 4u) + (bin % 10);
}

void bp6a_rtc_offset_write(uint32_t offset, uint32_t flag)
{
    uint8_t i;

    bp6a_rtc_unlock(true);
    for (i = 0; i < 4; i++)
        putreg32(0x40019000 + 0xB8 + i * 4, ((offset >> (i * 8)) & 0xFF));

    putreg32(0x40019000 + 0x90, flag);
    bp6a_rtc_unlock(false);
}

uint32_t bp6a_rtc_read_offset(uint32_t *flag)
{
    uint8_t i;
    uint32_t offset = 0;

    bp6a_rtc_unlock(true);

    for (i = 0; i < 4; i++)
        offset |= getreg32(0x40019000 + 0xB8 + i * 4)  << (i * 8);

    *flag = getreg32(0x40019000 + 0x90);

    bp6a_rtc_unlock(false);

    return offset;
}

void bp6a_rtc_getdatetime(struct rtc_bcd_s *rtc)
{
    bp6a_rtc_unlock(true);
    BP_RTC->CALENDAR_TIME_STAMP = 1;
    _Wait(grtc_load_delay);

    /* read bcd counters */
    do {
        rtc->sec = BP_RTC->BCD_R.SEC;
        rtc->min = BP_RTC->BCD_R.MIN;
        rtc->hour = BP_RTC->BCD_R.HOUR;
        rtc->day = BP_RTC->BCD_R.DAY;
        rtc->mon = (BP_RTC->BCD_R.MON >> 3) & 0x0F;
        rtc->year = BP_RTC->BCD_R.YEAR & 0x3Fu;
    } while (rtc->sec != BP_RTC->BCD_R.SEC);

    bp6a_rtc_unlock(false);

}

void bp6a_rtc_setdatetime(struct rtc_bcd_s *rtc)
{
    bp6a_rtc_unlock(true);

    /* update BCD counters */
    BP_RTC->BCD_W.SEC = rtc->sec;
    BP_RTC->BCD_W.MIN = rtc->min;
    BP_RTC->BCD_W.HOUR = rtc->hour;
    BP_RTC->BCD_W.DAY = rtc->day;
    BP_RTC->BCD_W.MON = ((uint32_t)rtc->mon << 4);
    BP_RTC->BCD_W.YEAR = rtc->year & 0x3Fu;
    BP_RTC->LOAD_COUNT_SIG = 0x7F;
    _Wait(grtc_load_delay);

    bp6a_rtc_unlock(false);
}

void bp6a_set_rtc_delay(uint32_t delay)
{
    grtc_load_delay = delay;
}

void bp6a_rtc_init(void)
{
    bp6a_rtc_unlock(true);
    BP_RTC->ALARM_ENABLE = 0;
    BP_RTC->INT_ENABLE_CPU = 0;
    BP_RTC->INT_CLEAR_CPU = 0;
    BP_RTC->CALENDAR_MODE = 0x03;
    bp6a_rtc_unlock(false);
}
