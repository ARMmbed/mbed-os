/*
 * Copyright (c) 2020, Pelion and affiliates.
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

#ifndef WS_PAE_TIME_H_
#define WS_PAE_TIME_H_

/*
 * Port access entity time functions.
 *
 */

typedef enum {
    TIME_FORMAT_SECONDS = 0,
    TIME_FORMAT_MINUTES,
    TIME_FORMAT_HOURS,
    TIME_FORMAT_DAYS,
} time_format_t;

#define STIME_TIME_BITS             14
#define STIME_TIME_MAX              0x3FFF
#define STIME_TIME_MASK             STIME_TIME_MAX

#define STIME_FORMAT_GET(stime) (stime >> STIME_TIME_BITS)
#define STIME_TIME_GET(stime) (stime & STIME_TIME_MASK)

// Maximum difference in stored and indicated system time
#define SYSTEM_TIME_MAXIMUM_DIFF   (60 * 60 * 24 * 30 * 12)

/**
 * ws_pae_time_to_short_convert convert time to short format
 *
 * \param time time in seconds to convert
 *
 * \return < 0 failure
 * \return >= 0 success
 *
 */
uint16_t ws_pae_time_to_short_convert(uint32_t time);

/**
 * ws_pae_time_from_short_convert convert short time to time format
 *
 * \param short_time short_time to convert
 *
 * \return time in seconds
 *
 */
uint32_t ws_pae_time_from_short_convert(uint16_t short_time);

/**
 * ws_pae_time_from_short_time_compare compare two times in short format
 *
 * \param short_time1 time 1 to compare
 * \param short_time1 time 2 to compare
 *
 * \return true times are equal
 * \return false times are not equal
 *
 */
bool ws_pae_time_from_short_time_compare(uint16_t short_time1, uint16_t short_time2);

/**
 * ws_pae_time_diff_calc calculates difference between two times
 *
 * \param curr_time current time
 * \param comp_time time which is compared
 * \param time_diff returns time difference
 * \param future compared time should in future
 *
 * \return < 0 failure
 * \return >= 0 success
 *
 */
int8_t ws_pae_time_diff_calc(uint64_t curr_time, uint64_t comp_time, uint32_t *time_diff, bool future);

/**
 * ws_pae_current_time_get gets current time
 *
 * \return current time in seconds after 1970
 *
 */
uint64_t ws_pae_current_time_get(void);

/**
 * ws_pae_current_time_get updates current time
 *
 * \param seconds seconds to be added to current time
 *
 */
void ws_pae_current_time_update(uint16_t seconds);

/**
 * ws_pae_current_time_set sets current time
 *
 * \param time new time
 *
 * \return < 0 failure
 * \return >= 0 success
 */
int8_t ws_pae_current_time_set(uint64_t time);

#endif /* WS_PAE_KEY_STORAGE_H_ */
