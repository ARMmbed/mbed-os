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

#include <string.h>
#include <stdio.h>
#include "ns_types.h"
#include "ns_time_api.h" //ns_time_api_system_time_callback

static ns_time_api_system_time_callback *system_time_read_callback = NULL;
static ns_time_api_system_time_write_callback *system_time_write_callback = NULL;
static ns_time_api_time_configuration_notify_callback *system_time_configuration_notify_callback = NULL;

static bool system_time_acquired = false;

void ns_time_api_system_time_callback_set(ns_time_api_system_time_callback callback_rd)
{
    system_time_read_callback = callback_rd;
}

void ns_time_api_system_time_write_callback_set(ns_time_api_system_time_write_callback callback_wr)
{
    system_time_write_callback = callback_wr;
}

void ns_time_api_time_configuration_notify_callback_set(ns_time_api_time_configuration_notify_callback callback_wr)
{
    system_time_configuration_notify_callback = callback_wr;
}

int ns_time_system_time_write(uint64_t time_write)
{
    if (system_time_write_callback) {
        system_time_write_callback(time_write);
        system_time_acquired = true;
        return 0;
    }

    return -1;
}

int ns_time_system_time_read(uint64_t *time_read)
{
    if (system_time_read_callback && time_read) {
        uint64_t new_time = system_time_read_callback();
        *time_read = new_time;
        return 0;
    }

    return -1;
}

int ns_time_system_timezone_info_notify(timezone_info_t *info_ptr)
{
    if (system_time_configuration_notify_callback && info_ptr) {
        system_time_configuration_notify_callback(info_ptr);
        return 0;
    }

    return -1;
}

void ns_time_system_time_acquired_set(void)
{
    system_time_acquired = true;
}

bool ns_time_system_time_acquired_get(void)
{
    return system_time_acquired;
}
