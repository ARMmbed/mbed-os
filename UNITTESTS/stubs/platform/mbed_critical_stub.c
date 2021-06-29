/*
 * Copyright (c) 2017, Arm Limited and affiliates.
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

/* Declare __STDC_LIMIT_MACROS so stdint.h defines UINT32_MAX when using C++ */
#define __STDC_LIMIT_MACROS
#include "hal/critical_section_api.h"

#include "cmsis.h"
#include "platform/mbed_assert.h"
#include "platform/mbed_critical.h"
#include "platform/mbed_toolchain.h"

bool core_util_are_interrupts_enabled(void)
{
    return false;
}

bool core_util_is_isr_active(void)
{
    return false;
}

bool core_util_in_critical_section(void)
{
    return false;
}

void core_util_critical_section_enter(void)
{
}

void core_util_critical_section_exit(void)
{
}
