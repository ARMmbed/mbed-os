/*
 * Copyright (c) 2016, 2018, Arm Limited and affiliates.
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

#include "eventOS_event_timer.h"

static void (*test_callback)(void *);
static void *test_args;

timeout_t *eventOS_timeout_ms(void (*callback)(void *), uint32_t ms, void *arg)
{
    test_callback = callback;
    test_args = arg;
    return NULL;
}

void eventOS_timeout_cancel(timeout_t *t)
{

}

void test_eventOS_timeout_trigger()
{
    test_callback(test_args);
}

