/*
 * Copyright (c) 2018-2019 Arm Limited and affiliates.
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

#include "ResetReason.h"

#ifdef DEVICE_RESET_REASON

namespace mbed {

reset_reason_t ResetReason::get()
{
    // Store the reason statically so it can be accessed after the first call to
    // this function resets it.
    const static reset_reason_t reason = hal_reset_reason_get();

    // Call get raw to cache the reset reason before clearing the registers.
    ResetReason::get_raw();

    hal_reset_reason_clear();

    return reason;
}

uint32_t ResetReason::get_raw()
{
    const static uint32_t reason = hal_reset_reason_get_raw();

    return reason;
}

} // namespace mbed

#endif // DEVICE_RESET_REASON
