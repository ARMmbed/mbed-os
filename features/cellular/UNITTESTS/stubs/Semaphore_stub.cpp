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

int32_t Semaphore::wait(uint32_t millisec) 
{
    return 0;
}

int32_t Semaphore::wait_until(uint64_t millisec) 
{
    return 0;
}

osStatus Semaphore::release(void) 
{
    return 0;
}

Semaphore::~Semaphore() 
{

}

}
