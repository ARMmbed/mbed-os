/*
 * Copyright (c) 2013-2017, ARM Limited, All Rights Reserved
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
 */

#include "mbed.h"
#include "greentea-client/test_env.h"
#include "rtos.h"

#if defined(MBED_RTOS_SINGLE_THREAD)
  #error [NOT_SUPPORTED] test not supported
#endif

#define TEST_STACK_SIZE 512

#define   EVENT_SET_VALUE    0x01
const int EVENT_TO_EMIT = 100;
const int EVENT_HANDLE_DELAY = 25;

DigitalOut led(LED1);
EventFlags event_flags;

int events_counter = 0;

void led_thread() {
    while (true) {
        event_flags.wait_all(EVENT_SET_VALUE);
        led = !led;
        events_counter++;
    }
}

int main (void) {
    GREENTEA_SETUP(10, "default_auto");

    Thread thread(osPriorityNormal, TEST_STACK_SIZE);
    thread.start(led_thread);

    bool result = false;

    while (true) {
        Thread::wait(2 * EVENT_HANDLE_DELAY);
        event_flags.set(EVENT_SET_VALUE);
        if (events_counter == EVENT_TO_EMIT) {
            result = true;
            break;
        }
    }
    GREENTEA_TESTSUITE_RESULT(result);
    return 0;
}
