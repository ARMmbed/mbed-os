/*
 * Copyright (c) 2019, Arm Limited and affiliates.
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
#ifndef MBED_OS_UNITTESTS_STUBS_CONDITIONVARIABLE_STUB_CPP_
#define MBED_OS_UNITTESTS_STUBS_CONDITIONVARIABLE_STUB_CPP_

#include "ConditionVariable_stub.h"
#include "mbed_error.h"
#include "mbed_assert.h"

bool ConditionVariable_stub::time_out = false;
#ifdef MBED_CONF_RTOS_PRESENT
using namespace rtos;

ConditionVariable::ConditionVariable(Mutex &mutex): _mutex(mutex), _wait_list(0)
{
    // No initialization to do
}

ConditionVariable::~ConditionVariable()
{
}

void ConditionVariable::notify_one()
{

}

void ConditionVariable::wait()
{
}

bool ConditionVariable::wait_for(uint32_t millisec)
{
    return ConditionVariable_stub::time_out;
}

void ConditionVariable::notify_all()
{
}
#endif
#endif /* MBED_OS_UNITTESTS_STUBS_CONDITIONVARIABLE_STUB_CPP_ */
