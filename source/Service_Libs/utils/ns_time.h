/*
 * Copyright (c) 2021, Pelion and affiliates.
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

#ifndef _NS_TIME_H_
#define _NS_TIME_H_

/**
 * \file ns_time.h
 * \brief Nanostack internal time handling API.
 */
#include "ns_time_api.h"

/**
 * Write new time as a platform time
 *
 * Write a new time to platform provided time system.
 * Platform time callbacks must be set by using method ns_time_api_system_time_callbacks_set.
 *
 * \param time_write time to be written as a new system time.
 *
 * \return 0 in success.
 * \return <0 in case of errors.
 *
 */
int ns_time_system_time_write(uint64_t time_write);

/**
 * Read platform time from a time callback
 *
 * Read a new time from time system provided by the platform.
 * Platform time callbacks must be set by using the method ns_time_api_system_time_callbacks_set.
 *
 * \param time_read Address to variable where new time will be written.
 *
 * \return 0 in success.
 * \return <0 in case of errors.
 *
 */
int ns_time_system_time_read(uint64_t *time_read);

/**
 * Notify Time zone and daylight saving time information
 *
 * \param info_ptr time zone information pointer.
 *
 * \return 0 in success.
 * \return <0 in case of errors.
 *
 */
int ns_time_system_timezone_info_notify(timezone_info_t *info_ptr);

/**
 * Set system time acquired
 *
 * Sets system time acquired (e.g. has been acquired from network)
 *
 */
void ns_time_system_time_acquired_set(void);

/**
 * Get system time acquired
 *
 * Checks whether system time has been acquired (e.g. has been written)
 *
 * \return TRUE system time has been acquired, FALSE system time has not been acquired
 *
 */
bool ns_time_system_time_acquired_get(void);

#endif /* _NS_TIME_H_ */
