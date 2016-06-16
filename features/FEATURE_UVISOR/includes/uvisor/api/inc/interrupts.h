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

#include "api/inc/uvisor_exports.h"
#include <stdint.h>

UVISOR_EXTERN void vIRQ_SetVector(uint32_t irqn, uint32_t vector);
UVISOR_EXTERN uint32_t vIRQ_GetVector(uint32_t irqn);
UVISOR_EXTERN void vIRQ_EnableIRQ(uint32_t irqn);
UVISOR_EXTERN void vIRQ_DisableIRQ(uint32_t irqn);
UVISOR_EXTERN void vIRQ_ClearPendingIRQ(uint32_t irqn);
UVISOR_EXTERN void vIRQ_SetPendingIRQ(uint32_t irqn);
UVISOR_EXTERN uint32_t vIRQ_GetPendingIRQ(uint32_t irqn);
UVISOR_EXTERN void vIRQ_SetPriority(uint32_t irqn, uint32_t priority);
UVISOR_EXTERN uint32_t vIRQ_GetPriority(uint32_t irqn);
UVISOR_EXTERN int vIRQ_GetLevel(void);

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
UVISOR_EXTERN void vIRQ_DisableAll(void);

/** Re-enable all interrupts that were previously disabled for the currently
 *  active box.
 *
 * This function only re-enables interrupt if the uVisor internal counter is set
 * to 0, to make sure that nested disabling of IRQs is still effective. See
 * ::vIRQ_DisableAll for more information. */
UVISOR_EXTERN void vIRQ_EnableAll(void);

#endif /* __UVISOR_API_INTERRUPTS_H__ */
