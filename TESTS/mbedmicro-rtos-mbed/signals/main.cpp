/* mbed Microcontroller Library
 * Copyright (c) 2017 ARM Limited
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
#include "mbed.h"
#include "greentea-client/test_env.h"
#include "rtos.h"

#if defined(MBED_RTOS_SINGLE_THREAD)
  #error [NOT_SUPPORTED] test not supported
#endif

#define TEST_STACK_SIZE 512

#define SIGNAL_SET_VALUE    0x01
const int SIGNALS_TO_EMIT = 100;
const int SIGNAL_HANDLE_DELEY = 25;

DigitalOut led(LED1);
int signal_counter = 0;

void led_thread() {
    while (true) {
        // Signal flags that are reported as event are automatically cleared.
        Thread::signal_wait(SIGNAL_SET_VALUE);
        led = !led;
        signal_counter++;
    }
}

int main (void) {
    GREENTEA_SETUP(20, "default_auto");

    Thread thread(osPriorityNormal, TEST_STACK_SIZE);
    thread.start(led_thread);

    bool result = false;

    printf("Handling %d signals...\r\n", SIGNALS_TO_EMIT);

    while (true) {
        Thread::wait(2 * SIGNAL_HANDLE_DELEY);
        thread.signal_set(SIGNAL_SET_VALUE);
        if (signal_counter == SIGNALS_TO_EMIT) {
            printf("Handled %d signals\r\n", signal_counter);
            result = true;
            break;
        }
    }
    GREENTEA_TESTSUITE_RESULT(result);
    return 0;
}
