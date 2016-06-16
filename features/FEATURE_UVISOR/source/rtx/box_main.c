/*
 * Copyright (c) 2016, ARM Limited, All Rights Reserved
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
#include "uvisor-lib/uvisor-lib.h"
#include "mbed_interface.h"
#include "cmsis_os.h"
#include <stdint.h>
#include <stdlib.h>

/* This function is called by uVisor in unprivileged mode to create box main
 * threads. */
void __uvisor_lib_box_main_handler(
    void (*function)(void const *),
    int32_t priority,
    uint32_t stack_pointer,
    uint32_t stack_size)
{
    osThreadId thread_id;
    osThreadDef_t thread_def;
    thread_def.pthread = function;
    thread_def.tpriority = priority;
    thread_def.stacksize = stack_size;
    thread_def.stack_pointer = malloc(stack_size); /* XXX */

    thread_id = osThreadCreate(&thread_def, NULL);

    if (thread_id == NULL) {
        mbed_die();
    }
}
