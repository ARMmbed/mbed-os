/* mbed Microcontroller Library
 * Copyright (c) 2018-2019 ARM Limited
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

#include "platform/platform.h"
#include "platform/mbed_critical.h"
#include "platform/mbed_assert.h"
#include "platform/mbed_error.h"


#include "platform/internal/CThunkBase.h"

MBED_STATIC_ASSERT(MBED_CONF_PLATFORM_CTHUNK_COUNT_MAX < 256, "MBED_CONF_PLATFORM_CTHUNK_COUNT_MAX must be less than 256");
MBED_STATIC_ASSERT(MBED_CONF_PLATFORM_CTHUNK_COUNT_MAX > 0, "MBED_CONF_PLATFORM_CTHUNK_COUNT_MAX must be greater than 0");

#define ENABLE_N(N) ((MBED_CONF_PLATFORM_CTHUNK_COUNT_MAX & N) ? 1 : 0)

#define START_128   0
#define START_64    (START_128 + ENABLE_N(128) * 128)
#define START_32    (START_64  + ENABLE_N(64)  * 64)
#define START_16    (START_32  + ENABLE_N(32)  * 32)
#define START_8     (START_16  + ENABLE_N(16)  * 16)
#define START_4     (START_8   + ENABLE_N(8)   * 8)
#define START_2     (START_4   + ENABLE_N(4)   * 4)
#define START_1     (START_2   + ENABLE_N(2)   * 2)

#define DECLARE_THUNK128(start) \
    DECLARE_THUNK64(start),     \
    DECLARE_THUNK64(start + 64)
#define DECLARE_THUNK64(start) \
    DECLARE_THUNK32(start),     \
    DECLARE_THUNK32(start + 32)
#define DECLARE_THUNK32(start) \
    DECLARE_THUNK16(start),     \
    DECLARE_THUNK16(start + 16)
#define DECLARE_THUNK16(start) \
    DECLARE_THUNK8(start),      \
    DECLARE_THUNK8(start + 8)
#define DECLARE_THUNK8(start)  \
    DECLARE_THUNK4(start),      \
    DECLARE_THUNK4(start + 4)
#define DECLARE_THUNK4(start)  \
    DECLARE_THUNK2(start),      \
    DECLARE_THUNK2(start + 2)
#define DECLARE_THUNK2(start)  \
    DECLARE_THUNK1(start),       \
    DECLARE_THUNK1(start + 1)
#define DECLARE_THUNK1(index)  &CThunkBase::thunk_entry<index>

const CThunkEntry CThunkBase::_thunk_table[MBED_CONF_PLATFORM_CTHUNK_COUNT_MAX] = {
#if ENABLE_N(128)
    DECLARE_THUNK128(START_128),
#endif
#if ENABLE_N(64)
    DECLARE_THUNK64(START_64),
#endif
#if ENABLE_N(32)
    DECLARE_THUNK32(START_32),
#endif
#if ENABLE_N(16)
    DECLARE_THUNK16(START_16),
#endif
#if ENABLE_N(8)
    DECLARE_THUNK8(START_8),
#endif
#if ENABLE_N(4)
    DECLARE_THUNK4(START_4),
#endif
#if ENABLE_N(2)
    DECLARE_THUNK2(START_2),
#endif
#if ENABLE_N(1)
    DECLARE_THUNK1(START_1)
#endif
};

CThunkBase *CThunkBase::_thunk_storage[MBED_CONF_PLATFORM_CTHUNK_COUNT_MAX];

CThunkBase::CthunkFree CThunkBase::_cthunk_free_real = NULL;

CThunkEntry CThunkBase::cthunk_alloc(CThunkBase *cthunk)
{
    // Atomically allocate one entry
    core_util_critical_section_enter();
    CThunkEntry entry = NULL;
    for (int i = 0; i < MBED_CONF_PLATFORM_CTHUNK_COUNT_MAX; i++) {
        if (_thunk_storage[i] == NULL) {
            _thunk_storage[i] = cthunk;
            entry = _thunk_table[i];
            break;
        }
    }
    core_util_critical_section_exit();

    if (entry == NULL) {
        MBED_ERROR(MBED_MAKE_ERROR(MBED_MODULE_PLATFORM, MBED_ERROR_CODE_OUT_OF_RESOURCES), "Ran out of CThunk entries. Increase MBED_CONF_PLATFORM_CTHUNK_COUNT_MAX to fix this error");
    }

    // Set function pointer on first use. This allows _thunk_table
    // and _thunk_storage to get removed by the linker if
    // cthunk_alloc is never used.
    _cthunk_free_real = &cthunk_free_real;

    return entry;
}

void CThunkBase::cthunk_free(CThunkEntry item)
{
    if (_cthunk_free_real) {
        _cthunk_free_real(item);
    }
}

void CThunkBase::cthunk_free_real(CThunkEntry item)
{
    bool found = false;

    core_util_critical_section_enter();
    for (int i = 0; i < MBED_CONF_PLATFORM_CTHUNK_COUNT_MAX; i++) {
        if (_thunk_table[i] == item) {
            _thunk_storage[i] = NULL;
            found = true;
            break;
        }
    }
    core_util_critical_section_exit();

    if (!found) {
        MBED_ERROR(MBED_MAKE_ERROR(MBED_MODULE_PLATFORM, MBED_ERROR_CODE_INVALID_ARGUMENT), "Tried to free invalid CThunkEntry");
    }

}
