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

#include "CellularLog.h"

#if MBED_CONF_MBED_TRACE_ENABLE

namespace mbed_cellular_trace {

typedef struct trace_s {
    void (*mutex_wait_f)(void);
    void (*mutex_release_f)(void);
} trace_t;

static trace_t m_trace = {};

void mutex_wait_function_set(void (*mutex_wait_f)(void))
{
    m_trace.mutex_wait_f = mutex_wait_f;
}

void mutex_release_function_set(void (*mutex_release_f)(void))
{
    m_trace.mutex_release_f = mutex_release_f;
}


void mutex_wait()
{
    if (m_trace.mutex_wait_f) {
        m_trace.mutex_wait_f();
    }
}

void mutex_release()
{
    if (m_trace.mutex_release_f) {
        m_trace.mutex_release_f();
    }
}

} // namespace mbed_cellular_trace

#endif // MBED_CONF_MBED_TRACE_ENABLE
