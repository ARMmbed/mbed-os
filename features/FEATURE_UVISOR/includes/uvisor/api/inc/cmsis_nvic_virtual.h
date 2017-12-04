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
#ifndef __UVISOR_API_NVIC_VIRTUAL_H__
#define __UVISOR_API_NVIC_VIRTUAL_H__

#include "api/inc/interrupts.h"
#include "api/inc/virq_exports.h"

/* The NVIC APIs are only wrapped on ARMv7-M. */
#if !defined(ARCH_CORE_ARMv8M) && !defined(TARGET_M33)

#define NVIC_SetPriorityGrouping __NVIC_SetPriorityGrouping
#define NVIC_GetPriorityGrouping __NVIC_GetPriorityGrouping
#define NVIC_EnableIRQ           vIRQ_EnableIRQ
#define NVIC_DisableIRQ          vIRQ_DisableIRQ
#define NVIC_GetPendingIRQ       vIRQ_GetPendingIRQ
#define NVIC_SetPendingIRQ       vIRQ_SetPendingIRQ
#define NVIC_ClearPendingIRQ     vIRQ_ClearPendingIRQ
#define NVIC_GetActive           __NVIC_GetActive
#define NVIC_SetPriority         vIRQ_SetPriority
#define NVIC_GetPriority         vIRQ_GetPriority
#define NVIC_SystemReset()       vIRQ_SystemReset(RESET_REASON_NO_REASON)

#else

#define NVIC_SetPriorityGrouping __NVIC_SetPriorityGrouping
#define NVIC_GetPriorityGrouping __NVIC_GetPriorityGrouping
#define NVIC_EnableIRQ           __NVIC_EnableIRQ
#define NVIC_DisableIRQ          __NVIC_DisableIRQ
#define NVIC_GetPendingIRQ       __NVIC_GetPendingIRQ
#define NVIC_SetPendingIRQ       __NVIC_SetPendingIRQ
#define NVIC_ClearPendingIRQ     __NVIC_ClearPendingIRQ
#define NVIC_GetActive           __NVIC_GetActive
#define NVIC_SetPriority         __NVIC_SetPriority
#define NVIC_GetPriority         __NVIC_GetPriority
#define NVIC_SystemReset()       __NVIC_SystemReset()

#endif

#endif /* __UVISOR_API_NVIC_VIRTUAL_H__ */
