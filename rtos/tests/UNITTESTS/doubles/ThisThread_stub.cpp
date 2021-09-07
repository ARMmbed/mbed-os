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

#include "rtos/ThisThread.h"

namespace rtos {

void ThisThread::sleep_until(uint64_t millisec)
{
}

void ThisThread::sleep_until(Kernel::Clock::time_point abs_time)
{
}

void ThisThread::sleep_for(uint32_t millisec)
{
}

void ThisThread::sleep_for(Kernel::Clock::duration_u32 rel_time)
{
}

}
