/* mbed Microcontroller Library
 * Copyright (c) 2021 ARM Limited
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

#if MBED_CONF_MBED_TRACE_ENABLE

#include "wsf_types.h"
#include "wsf_trace.h"
#include "mbed-trace/mbed_trace.h"
#include "mbed_wsf_trace.h"
#include "platform/mbed_critical.h"

#define TRACE_GROUP "BLCO"

static bool_t wsf_mbed_trace_printf_cb(const uint8_t *pBuf, uint32_t len)
{
#if defined(MBED_TRAP_ERRORS_ENABLED) && MBED_TRAP_ERRORS_ENABLED && defined(MBED_CONF_RTOS_PRESENT)
    if (core_util_is_isr_active() || !core_util_are_interrupts_enabled()) {
        return FALSE;
    }
#endif
    /* we need to strip the newline at the end*/
    if (len > 1) {
        tr_debug("%.*s", (int) (len - 1), pBuf);
    }
    return TRUE;
}

void wsf_mbed_trace_init()
{
    tr_info("WSF Cordio tracing enabled");
    WsfTraceEnable(true);
    WsfTraceRegisterHandler(&wsf_mbed_trace_printf_cb);
}

#if MBED_CONF_CORDIO_TRACE_HCI_PACKETS
#define TRACE_GROUP "BLHC"
void wsf_mbed_trace_hci(const uint8_t *pBuf, uint32_t len, const uint8_t *prefix)
{
    tr_debug("LL->HOST %s %s", prefix, trace_array(pBuf, len));
}
#endif // MBED_CONF_CORDIO_TRACE_HCI_PACKETS

#endif // MBED_CONF_MBED_TRACE_ENABLE
