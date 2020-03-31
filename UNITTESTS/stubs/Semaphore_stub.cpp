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

#include "Semaphore.h"
#include "Semaphore_stub.h"

int Semaphore_stub::wait_return_value = 0;
bool Semaphore_stub::acquire_return_value;

namespace rtos {

Semaphore::Semaphore(int32_t count)
{

}

Semaphore::Semaphore(int32_t count, uint16_t max_count)
{

}

void Semaphore::constructor(int32_t count, uint16_t max_count)
{

}

void Semaphore::acquire()
{

}

bool Semaphore::try_acquire()
{
    return Semaphore_stub::acquire_return_value;
}

bool Semaphore::try_acquire_for(uint32_t millisec)
{
    return Semaphore_stub::acquire_return_value;
}

bool Semaphore::try_acquire_until(uint64_t millisec)
{
    return Semaphore_stub::acquire_return_value;
}

osStatus Semaphore::release(void)
{
    return 0;
}

Semaphore::~Semaphore()
{

}

}
