/*
 * Copyright (c) 2020 ARM Limited. All rights reserved.
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

// ----------------------------------------------------------- Includes -----------------------------------------------------------

#include "KVStore.h"
namespace mbed {


bool KVStore::is_valid_key(const char *key) const {
    if (!key || !strlen(key) || (strlen(key) > MAX_KEY_SIZE)) {
        return false;
    }

    if (strpbrk(key, " */?:;\"|<>\\")) {
        return false;
    }
    return true;
}

bool KVStore::_is_valid_flags(InitModeFlags flags) {
    // Check that only valid bits are here at all
    if ((~(InitModeFlags::AllFlags) & flags) == InitModeFlags::NoFlags) {
        return false;
    }
    
    // Need at least one of the Read or Write bits set
    if ((flags & InitModeFlags::ReadWrite) == InitModeFlags::NoFlags) {
        return false;
    }

    // Check for only one set of the WriteOpenFlags
    const uint32_t wo_flags = static_cast<uint32_t>(flags & InitModeFlags::WriteOpenFlags);
    // Fails for zero, the conditional afterwards finds this
    if (wo_flags & (wo_flags - 1)) {
        return false;
    }
    // Only allow blank Append, etc. if no writing
    if (((flags & InitModeFlags::Write) == InitModeFlags::Write) && (wo_flags == 0)) {
        return false;
    }

    return true;
}

bool KVStore::_has_flags_any(InitModeFlags flags) const {
    return !(((_flags & flags)) == InitModeFlags::NoFlags);
}

}