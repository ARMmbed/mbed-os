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

#ifndef __S1SBP6A_RTC_H
#define __S1SBP6A_RTC_H
#include <stdbool.h>

struct rtc_bcd_s {
    uint8_t sec;
    uint8_t min;
    uint8_t hour;
    uint8_t day;
    uint8_t mon;
    uint16_t year;
};

int bp6a_rtc_bcd2bin(uint8_t bcd);
uint32_t bp6a_rtc_bin2bcd(uint8_t bin);
void bp6a_rtc_getdatetime(struct rtc_bcd_s *rtc);
void bp6a_rtc_setdatetime(struct rtc_bcd_s *rtc);
void bp6a_rtc_init(void);
void bp6a_set_rtc_delay(uint32_t delay);
uint32_t bp6a_rtc_read_offset(uint32_t *flag);
void bp6a_rtc_offset_write(uint32_t offset, uint32_t flag);
#endif  /*__S1SBP6A_RTC_H */
