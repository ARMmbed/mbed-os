/*
 * Copyright (c) 2015 ARM Limited. All rights reserved.
 * SPDX-License-Identifier: Apache-2.0
 * Licensed under the Apache License, Version 2.0 (the License); you may
 * not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an AS IS BASIS, WITHOUT
 * WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
#include "m2mtimer_stub.h"

bool m2mtimer_stub::bool_value;
bool m2mtimer_stub::total_bool_value;

void m2mtimer_stub::clear()
{
    bool_value = false;
    total_bool_value = false;
}

// Prevents the use of assignment operator
M2MTimer& M2MTimer::operator=(const M2MTimer& /*other*/)
{
    return *this;
}

// Prevents the use of copy constructor
M2MTimer::M2MTimer(const M2MTimer& other)
: _observer(other._observer)
{
    *this = other;
}

M2MTimer::M2MTimer(M2MTimerObserver& observer)
: _observer(observer)
{
}

M2MTimer::~M2MTimer()
{
}

void M2MTimer::start_timer(uint64_t /*interval*/,
                           M2MTimerObserver::Type /*type*/,
                           bool /*single_shot*/)
{
}

void M2MTimer::start_dtls_timer(uint64_t , uint64_t , M2MTimerObserver::Type )
{
}

void M2MTimer::stop_timer()
{
}


bool M2MTimer::is_intermediate_interval_passed(){
    return m2mtimer_stub::bool_value;
}

bool M2MTimer::is_total_interval_passed(){
    return m2mtimer_stub::total_bool_value;
}
