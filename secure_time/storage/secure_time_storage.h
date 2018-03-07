/* Copyright (c) 2018 ARM Limited
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
#ifndef __SECURE_TIME_STORAGE_H__
#define __SECURE_TIME_STORAGE_H__

#include <stdint.h>
#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif

// Storage latency defines
#define SECURE_TIME_MIN_STORAGE_FORWARD_LATENCY_SEC     (100000UL)
#define SECURE_TIME_MIN_STORAGE_BACKWARD_LATENCY_SEC    (100UL)
#define SECURE_TIME_MIN_STORAGE_IDLE_LATENCY_SEC        (500000UL)

/*
 * Set storage entry of last set time.
 *
 * @param[in] new_time  Time value in seconds since EPOCH.
 */
void secure_time_set_stored_time(uint64_t new_time);

/*
 * Get storage entry of last set time.
 *
 * @param[out] stored_time  Time value in seconds since EPOCH, 0 if not exists.
 */
void secure_time_get_stored_time(uint64_t *stored_time);

/*
 * Set storage entry of last set-backwards time.
 *
 * @param[in] new_time  Time value in seconds since EPOCH.
 */
void secure_time_set_stored_back_time(uint64_t new_time);

/*
 * Get storage entry of last set-backwards time.
 *
 * @param[out] stored_back_time  Time value in seconds since EPOCH, 0 if not exists.
 */
void secure_time_get_stored_back_time(uint64_t *stored_back_time);

#ifdef __cplusplus
}
#endif

#endif // __SECURE_TIME_STORAGE_H__
