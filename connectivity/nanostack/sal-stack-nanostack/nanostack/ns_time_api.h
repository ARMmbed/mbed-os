/*
 * Copyright (c) 2020, Arm Limited and affiliates.
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

/**
 * \file ns_time_api.h
 * \brief Nanostack time API
 *
 * This is Nanostack time API.
 *
 */

#ifndef NS_TIME_API_H_
#define NS_TIME_API_H_

#include "ns_types.h"

/**
 * System time callback.
 *
 * Callback shall return the system time in seconds after 1970.
 *
 * \param seconds system time in seconds
 *
 */
typedef uint64_t ns_time_api_system_time_callback(void);

/**
 * System time callback set.
 *
 * Sets callback for the system time.
 *
 * \param callback system time callback
 *
 */
void ns_time_api_system_time_callback_set(ns_time_api_system_time_callback callback);

#endif /* NS_TIME_API_H_ */
