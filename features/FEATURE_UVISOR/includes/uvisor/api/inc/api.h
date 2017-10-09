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
#ifndef __UVISOR_API_H__
#define __UVISOR_API_H__

#include "rt_OsEventObserver.h"
#include "api/inc/uvisor_exports.h"
#include "api/inc/virq_exports.h"
#include "api/inc/debug_exports.h"
#include "api/inc/halt_exports.h"
#include "api/inc/pool_queue_exports.h"
#include "api/inc/page_allocator_exports.h"
#include "api/inc/uvisor_spinlock_exports.h"
#include <stdint.h>

#define UVISOR_API_MAGIC 0x5C9411B4
#define UVISOR_API_VERSION (10)

UVISOR_EXTERN_C_BEGIN

extern void uvisor_init(void);
/* This method is an api method which initializes the g_semihosting_magic variable.
 * This function will only work if called in privileged mode or from secure code,
 * it is only intended to be called by debugger after reset.
 */
void debug_semihosting_enable(void);

typedef struct {
    uint32_t magic;
    uint32_t (*get_version)(uint32_t);

    void (*init)(uint32_t caller);

    void     (*irq_enable)(uint32_t irqn);
    void     (*irq_disable)(uint32_t irqn);
    void     (*irq_disable_all)(void);
    void     (*irq_enable_all)(void);
    void     (*irq_set_vector)(uint32_t irqn, uint32_t vector);
    uint32_t (*irq_get_vector)(uint32_t irqn);
    void     (*irq_set_priority)(uint32_t irqn, uint32_t priority);
    uint32_t (*irq_get_priority)(uint32_t irqn);
    void     (*irq_set_pending)(uint32_t irqn);
    uint32_t (*irq_get_pending)(uint32_t irqn);
    void     (*irq_clear_pending)(uint32_t irqn);
    int      (*irq_get_level)(void);
    void     (*irq_system_reset)(TResetReason reason);

    int (*page_malloc)(UvisorPageTable * const table);
    int (*page_free)(const UvisorPageTable * const table);

    int (*box_namespace)(int box_id, char *box_namespace, size_t length);
    int (*box_id_for_namespace)(int * const box_id, const char * const box_namespace);

    void (*error)(THaltUserError reason);
    void (*start)(void);
    void (*vmpu_mem_invalidate)(void);

    int                (*pool_init)(uvisor_pool_t *, void *, size_t, size_t);
    int                (*pool_queue_init)(uvisor_pool_queue_t *, uvisor_pool_t *, void *, size_t, size_t);
    uvisor_pool_slot_t (*pool_allocate)(uvisor_pool_t *);
    uvisor_pool_slot_t (*pool_try_allocate)(uvisor_pool_t *);
    uvisor_pool_slot_t (*pool_queue_enqueue)(uvisor_pool_queue_t *, uvisor_pool_slot_t);
    uvisor_pool_slot_t (*pool_queue_try_enqueue)(uvisor_pool_queue_t *, uvisor_pool_slot_t);
    uvisor_pool_slot_t (*pool_free)(uvisor_pool_t *, uvisor_pool_slot_t);
    uvisor_pool_slot_t (*pool_try_free)(uvisor_pool_t *, uvisor_pool_slot_t);
    uvisor_pool_slot_t (*pool_queue_dequeue)(uvisor_pool_queue_t *, uvisor_pool_slot_t);
    uvisor_pool_slot_t (*pool_queue_try_dequeue)(uvisor_pool_queue_t *, uvisor_pool_slot_t);
    uvisor_pool_slot_t (*pool_queue_dequeue_first)(uvisor_pool_queue_t *);
    uvisor_pool_slot_t (*pool_queue_try_dequeue_first)(uvisor_pool_queue_t *);
    uvisor_pool_slot_t (*pool_queue_find_first)(uvisor_pool_queue_t *, TQueryFN_Ptr, void *);
    uvisor_pool_slot_t (*pool_queue_try_find_first)(uvisor_pool_queue_t *, TQueryFN_Ptr, void *);

    void (*spin_init)(UvisorSpinlock * spinlock);
    bool (*spin_trylock)(UvisorSpinlock * spinlock);
    void (*spin_lock)(UvisorSpinlock * spinlock);
    void (*spin_unlock)(UvisorSpinlock * spinlock);

    void (*debug_semihosting_enable)(void);

    OsEventObserver os_event_observer;
} UVISOR_PACKED UvisorApi;

extern UvisorApi uvisor_api;

static UVISOR_FORCEINLINE uint32_t uvisor_get_version(void)
{
    return uvisor_api.get_version(UVISOR_API_VERSION);
}

UVISOR_EXTERN_C_END

#endif /* __UVISOR_API_H__ */
