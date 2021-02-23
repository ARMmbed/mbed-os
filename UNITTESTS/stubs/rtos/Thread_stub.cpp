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

#include "Thread_stub.h"

using namespace rtos;

osStatus Thread_stub::osStatus_value = osOK;

osStatus Thread::terminate()
{
    return 0;
}

osStatus Thread::start(mbed::Callback<void()> task)
{
    return Thread_stub::osStatus_value;
}

void Thread::constructor(osPriority priority, uint32_t stack_size, unsigned char *stack_mem, const char *name)
{
}

Thread::~Thread()
{
}
