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
#ifndef __UVISOR_API_UVISOR_SEMAPHORE_H__
#define __UVISOR_API_UVISOR_SEMAPHORE_H__

#include "api/inc/uvisor_semaphore_exports.h"

UVISOR_EXTERN int __uvisor_semaphore_init(UvisorSemaphore * semaphore, int32_t count);

/* This function is not safe to call from interrupt context, even if the
 * timeout is zero. */
UVISOR_EXTERN int __uvisor_semaphore_pend(UvisorSemaphore * semaphore, uint32_t timeout_ms);

/* This function is safe to call from interrupt context. */
UVISOR_EXTERN int __uvisor_semaphore_post(UvisorSemaphore * semaphore);

#endif
