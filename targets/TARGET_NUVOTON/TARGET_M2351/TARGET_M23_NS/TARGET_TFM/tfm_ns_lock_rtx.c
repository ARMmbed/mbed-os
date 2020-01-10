/*
 * Copyright (c) 2019, Nuvoton Technology Corporation
 *
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

#include <stdint.h>
#include <stdbool.h>
#include "cmsis.h"
#include "cmsis_os2.h"
#include "tfm_ns_lock.h"
#include "mbed_critical.h"
#include "mbed_assert.h"
#include "mbed_error.h"

/* Approach to serialization of NS secure calls required by TF-M secure world
 *
 * Default implementation of NS interface uses mutex to meet the requirement,
 * but it cannot support NS secure call in interrupt-disabled context. Instead,
 * in this override, NS secure call is guaranteed to be non-preemptive during
 * this period by locking kernel scheduler. Apparently, this approach has one
 * weakness: all other threads are also locked during this period. Until there's
 * a better approach coming out, we can just use this expedient one.
 *
 * For the 'lock kernel scheduler' approach to work thoroughly, we must also
 * address some side issues:
 *
 * - Prohibit NS secure call from ISR except SVC, so non-preemptive doesn't break.
 * - Allow NS secure call in SVC context because it is synchronous. Here, we lock
 *   interrupt instead of kernel scheduler because svcRtxKernelLock()/svcRtxKernelRestoreLock(...)
 *   are inaccessible outside rtx_kernel.c. Currently, this is rare case and would cause
 *   little trouble (see known paths below).
 * - Call into secure world straight in interrupt-disabled context. When in
 *   interrupt-disabled context, NS secure call is guaranteed to be non-preemptive
 *   naturally.
 * - Call into secure world straight at pre-rtos stage. When at pre-rtos stage,
 *   NS secure call is guaranteed to be non-preemptive naturally.
 * - osKernelLock() will error when kernel state is 'osKernelSuspended'. Address
 *   it separately. Known path of NS secure call when kernel state is 'osKernelSuspended':
 *   - default idle thread > osKernelSuspend() > lp_ticker_init > SYS_ResetModule_S/
 *     CLK_SetModuleClock_S/CLK_EnableModuleClock_S
 *
 * Known paths of NS secure call in interrupt-disabled context:
 * - mbed-os/platform/mbed_sleep_manager.c > sleep_manager_sleep_auto >
 *   hal_sleep/hal_deepsleep > nu_idle_s/nu_powerdown_s
 * - mbed-os/hal/LowPowerTickerWrapper.cpp > LowPowerTickerWrapper::init > 
 *   lp_ticker_init > SYS_ResetModule_S/CLK_SetModuleClock_S/CLK_EnableModuleClock_S
 * - mbed-os/platform/mbed_board.c > mbed_die > pin_function_s
 * - mbed-os-tests-mbed_hal-rtc > rtc_write_read_test > rtc_write >
 *   CLK_IsRTCClockEnabled_S
 *
 * Known paths of NS secure call in SVC context:
 * - In tickless mode, osKernelStart > svcRtxKernelStart > OS_Tick_Enable >
 *   us_ticker_init/lp_ticker_init > SYS_ResetModule_S/CLK_SetModuleClock_S/
 *   CLK_EnableModuleClock_S
 */

struct ns_interface_state
{
    bool        init;       // Flag if kernel has initialized (and then scheduler
                            // has started)
};

static struct ns_interface_state ns_interface = {
    .init = false
};

/* Override tfm_ns_lock_init()
 *
 * On Mbed OS, we expect this function is called before kernel scheduler is
 * started so that we can distinguish pre-rtos and rtos stage to meet the
 * requirement of serialization of NS secure calls.
 */
enum tfm_status_e tfm_ns_lock_init()
{
    if (!ns_interface.init) {
        osKernelState_t kernel_state = osKernelGetState();
        MBED_ASSERT(kernel_state == osKernelInactive || kernel_state == osKernelReady);

        ns_interface.init = true;
    }

    return TFM_SUCCESS;
}

/* Override tfm_ns_lock_dispatch(...) */
uint32_t tfm_ns_lock_dispatch(veneer_fn fn,
                              uint32_t arg0, uint32_t arg1,
                              uint32_t arg2, uint32_t arg3)
{
    /* Prohibit NS secure call from ISR except SVC, so non-preemptive doesn't break */
    uint32_t ipsr = __get_IPSR();
    if (ipsr == 11U) {
        /* Allow NS secure call in SVC context because it is synchronous. Here,
         * we lock interrupt instead of kernel scheduler because svcRtxKernelLock()/
         * svcRtxKernelRestoreLock(...) are inaccessible outside rtx_kernel.c. */
        core_util_critical_section_enter();
        uint32_t result = fn(arg0, arg1, arg2, arg3);
        core_util_critical_section_exit();

        return result;
    } else if (ipsr) {
        MBED_ERROR1(MBED_MAKE_ERROR(MBED_MODULE_KERNEL, MBED_ERROR_CODE_PROHIBITED_IN_ISR_CONTEXT), "Prohibited in ISR context", (uintptr_t) fn);
    }

    /* Call into secure world straight in interrupt-disabled context because
     * NS secure call is non-preemptive naturally */
    if (!core_util_are_interrupts_enabled()) {
        return fn(arg0, arg1, arg2, arg3);
    }

    /* Call into secure world straight at pre-rtos stage because NS secure
     * call is non-preemptive naturally */
    if (!ns_interface.init) {
        return fn(arg0, arg1, arg2, arg3);
    }

    /* osKernelLock() will error when kernel state is 'osKernelSuspended'. Address
     * it separately. */
    osKernelState_t kernel_state = osKernelGetState();
    if (kernel_state == osKernelSuspended) {
        return fn(arg0, arg1, arg2, arg3);
    } else if (kernel_state == osKernelError) {
        MBED_ERROR1(MBED_MAKE_ERROR(MBED_MODULE_KERNEL, MBED_ERROR_CODE_UNKNOWN), "RTX kernel state error", (uintptr_t) fn);
    }

    /* Lock kernel scheduler and save previous lock state for restore */
    int32_t lock_state = osKernelLock();
    if (lock_state == osError) {
        MBED_ERROR1(MBED_MAKE_ERROR(MBED_MODULE_KERNEL, MBED_ERROR_CODE_UNKNOWN), "Unknown RTX error", (uintptr_t) fn);
    }
    MBED_ASSERT(lock_state >= 0);

    /* NS secure call is non-preemptive because kernel scheduler is locked */
    uint32_t result = fn(arg0, arg1, arg2, arg3);

    /* Restore previous lock state */
    lock_state = osKernelRestoreLock(lock_state);
    if (lock_state == osError) {
        MBED_ERROR1(MBED_MAKE_ERROR(MBED_MODULE_KERNEL, MBED_ERROR_CODE_UNKNOWN), "Unknown RTX error", (uintptr_t) fn);
    }
    MBED_ASSERT(lock_state >= 0);

    return result;
}
