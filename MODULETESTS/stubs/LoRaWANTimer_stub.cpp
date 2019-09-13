/*
 * Copyright (c) , Arm Limited and affiliates.
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

#include "mbed_assert.h"
#include "LoRaWANTimer.h"

#include "LoRaWANTimer_stub.h"

lorawan_time_t LoRaWANTimer_stub::time_value = 0;
bool LoRaWANTimer_stub::call_cb_immediately = false;

LoRaWANTimeHandler::LoRaWANTimeHandler()
    : _queue(NULL)
{
}

LoRaWANTimeHandler::~LoRaWANTimeHandler()
{
}

void LoRaWANTimeHandler::activate_timer_subsystem(events::EventQueue *queue)
{
}

lorawan_time_t LoRaWANTimeHandler::get_current_time(void)
{
    return LoRaWANTimer_stub::time_value;
}

lorawan_time_t LoRaWANTimeHandler::get_elapsed_time(lorawan_time_t saved_time)
{
    return LoRaWANTimer_stub::time_value;
}

void LoRaWANTimeHandler::init(timer_event_t &obj, mbed::Callback<void()> callback)
{
    if (callback && LoRaWANTimer_stub::call_cb_immediately) {
        callback();
    }
    obj.timer_id = 1;
}

void LoRaWANTimeHandler::start(timer_event_t &obj, const uint32_t timeout)
{
}

void LoRaWANTimeHandler::stop(timer_event_t &obj)
{
    obj.timer_id = 0;
}
