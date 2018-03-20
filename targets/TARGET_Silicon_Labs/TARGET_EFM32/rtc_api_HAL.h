/***************************************************************************//**
 * @file rtc_api_HAL.h
 *******************************************************************************
 * @section License
 * <b>(C) Copyright 2015 Silicon Labs, http://www.silabs.com</b>
 *******************************************************************************
 *
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
 *
 ******************************************************************************/

#ifndef MBED_RTC_API_HAL_H
#define MBED_RTC_API_HAL_H

#include <stdint.h>
#include "rtc_api.h"
#include "clocking.h"

#define RTC_INIT_LPTIMER (1U << 1)
#define RTC_INIT_RTC     (1U << 0)

#if defined(RTCC_PRESENT)
#define RTC_BITS (32U)
#define RTC_MAX_VALUE (0xFFFFFFFFUL)
#elif defined(RTC_PRESENT)
#define RTC_BITS (24U)
#define RTC_MAX_VALUE (0xFFFFFFUL)
#endif

#ifdef __cplusplus
extern "C" {
#endif

/* Purpose of this file: extend rtc_api.h to include EFM-specific stuff*/
void rtc_set_comp0_handler(uint32_t handler);
void rtc_enable_comp0(bool enable);
void rtc_set_comp0_value(uint64_t value, bool enable);
void rtc_force_comp0(void);

uint64_t rtc_get_full(void);

void rtc_init_real(uint32_t flags);
void rtc_free_real(uint32_t flags);

#ifdef __cplusplus
}
#endif

#endif
