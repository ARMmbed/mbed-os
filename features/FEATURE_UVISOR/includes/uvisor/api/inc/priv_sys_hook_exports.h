/*
 * Copyright (c) 2016, ARM Limited, All Rights Reserved
 * SPDX-License-Identifier: Apache-2.0
 *
 * Licensed under the Apache License, Version 2.0 (the "License"); you may
 * not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS, WITHOUT
 * WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
#ifndef __UVISOR_API_PRIV_SYS_HOOK_EXPORTS_H__
#define __UVISOR_API_PRIV_SYS_HOOK_EXPORTS_H__

/* Predeclaration */
typedef struct uvisor_semaphore UvisorSemaphore;

/*
 * Privileged system hooks
 *
 * In this version of uVisor, uVisor lives alongside an RTOS that requires
 * running privileged code. In order for the RTOS to run any privileged code,
 * uVisor must allow the RTOS to handle a subset of privileged system
 * interrupts or system calls. Only the following system interrupts and system
 * calls are hookable. Code called by these hooks circumvents uVisor security.
 * HANDLE WITH CARE. */
typedef struct {
    void (*priv_svc_0)(void);
    void (*priv_pendsv)(void);
    void (*priv_systick)(void);
    uint32_t (*priv_os_suspend)(void);
    int (*priv_uvisor_semaphore_post)(UvisorSemaphore * semaphore);
} UvisorPrivSystemHooks;

/* Use this macro to register privileged system IRQ hooks. If you don't want to
 * register a particular privileged system IRQ hook, you can supply NULL for
 * that hook parameter. */
#define UVISOR_SET_PRIV_SYS_HOOKS(priv_svc_0_, priv_pendsv_, priv_systick_, priv_os_suspend_, priv_uvisor_semaphore_post_) \
    UVISOR_EXTERN_C_BEGIN \
    const UvisorPrivSystemHooks __uvisor_priv_sys_hooks = { \
        .priv_svc_0 = priv_svc_0_, \
        .priv_pendsv = priv_pendsv_, \
        .priv_systick = priv_systick_, \
        .priv_os_suspend = priv_os_suspend_, \
        .priv_uvisor_semaphore_post = priv_uvisor_semaphore_post_, \
    }; \
    UVISOR_EXTERN_C_END

#endif
