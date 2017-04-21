/*
 * Copyright (c) 2013-2015, ARM Limited, All Rights Reserved
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
#ifndef __UVISOR_API_INTERRUPTS_H__
#define __UVISOR_API_INTERRUPTS_H__

#include "api/inc/unvic_exports.h"
#include "api/inc/uvisor_exports.h"
#include "api/inc/api.h"
#include <stdint.h>

UVISOR_EXTERN_C_BEGIN

static UVISOR_FORCEINLINE void vIRQ_SetVector(uint32_t irqn, uint32_t vector)
{
    uvisor_api.irq_set_vector(irqn, vector);
}

static UVISOR_FORCEINLINE uint32_t vIRQ_GetVector(uint32_t irqn)
{
    return uvisor_api.irq_get_vector(irqn);
}

static UVISOR_FORCEINLINE void vIRQ_EnableIRQ(uint32_t irqn)
{
    uvisor_api.irq_enable(irqn);
}

static UVISOR_FORCEINLINE void vIRQ_DisableIRQ(uint32_t irqn)
{
    uvisor_api.irq_disable(irqn);
}

static UVISOR_FORCEINLINE void vIRQ_ClearPendingIRQ(uint32_t irqn)
{
    uvisor_api.irq_clear_pending(irqn);
}

static UVISOR_FORCEINLINE void vIRQ_SetPendingIRQ(uint32_t irqn)
{
    uvisor_api.irq_set_pending(irqn);
}

static UVISOR_FORCEINLINE uint32_t vIRQ_GetPendingIRQ(uint32_t irqn)
{
    return uvisor_api.irq_get_pending(irqn);
}

static UVISOR_FORCEINLINE void vIRQ_SetPriority(uint32_t irqn, uint32_t priority)
{
    uvisor_api.irq_set_priority(irqn, priority);
}

static UVISOR_FORCEINLINE uint32_t vIRQ_GetPriority(uint32_t irqn)
{
    return uvisor_api.irq_get_priority(irqn);
}

static UVISOR_FORCEINLINE int vIRQ_GetLevel(void)
{
    return uvisor_api.irq_get_level();
}

/** Disable all interrupts for the currently active box.
 *
 * Calling this function from a box only affects the interrupts of that box.
 * System interrupts and interrupts owned by other boxes are left untouched.
 *
 * Successive calls to this function increase an internal counter that is used
 * by uVisor to decide when to re-enable IRQs. The related call
 * ::vIRQ_EnableIRQ() decreases this counter. Only when the counter is 0 the
 * interrupts are re-enabled for that box.
 *
 * This guarantees that code that disables IRQs will not accidentally have them
 * re-enabled by a nested function that it calls before the expected call to
 * ::vIRQ_EnableAll(). Example:
 *
 * vIRQ_DisableAll();  counter = 1; IRQs are now disabled.
 * some_function();    counter = 2, then counter = 1; IRQs are still disabled.
 * vIRQ_EnableAll();   counter = 0; IRQs are now re-enabled.
 *
 * where some_function() also has a disable/enable pair. */
static UVISOR_FORCEINLINE void vIRQ_DisableAll(void)
{
    uvisor_api.irq_disable_all();
}

/** Re-enable all interrupts that were previously disabled for the currently
 *  active box.
 *
 * This function only re-enables interrupt if the uVisor internal counter is set
 * to 0, to make sure that nested disabling of IRQs is still effective. See
 * ::vIRQ_DisableAll for more information. */
static UVISOR_FORCEINLINE void vIRQ_EnableAll(void)
{
    uvisor_api.irq_enable_all();
}

/** Reset the device.
 * @warning Currently only the debug box can reset the device.
 * @param reason[in]    Reason for rebooting. Currently not used.
 */
static UVISOR_FORCEINLINE void vIRQ_SystemReset(TResetReason reason)
{
    return uvisor_api.irq_system_reset(reason);
}

UVISOR_EXTERN_C_END

#endif /* __UVISOR_API_INTERRUPTS_H__ */
