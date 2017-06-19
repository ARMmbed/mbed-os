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
#ifndef __UVISOR_API_VECTAB_VIRTUAL_H__
#define __UVISOR_API_VECTAB_VIRTUAL_H__

#include "api/inc/interrupts.h"

/* The NVIC APIs are only wrapped on ARMv7-M. */
#if !defined(ARCH_CORE_ARMv8M) && !defined(TARGET_M33)

#define NVIC_SetVector vIRQ_SetVector
#define NVIC_GetVector vIRQ_GetVector

#else

#define NVIC_SetVector __NVIC_SetVector
#define NVIC_GetVector __NVIC_GetVector

#endif

#endif /* __UVISOR_API_VECTAB_VIRTUAL_H__ */
