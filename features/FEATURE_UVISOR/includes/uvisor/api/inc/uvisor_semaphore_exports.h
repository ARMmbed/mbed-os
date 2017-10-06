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
#ifndef __UVISOR_API_UVISOR_SEMAPHORE_EXPORTS_H__
#define __UVISOR_API_UVISOR_SEMAPHORE_EXPORTS_H__

#include "api/inc/uvisor_exports.h"

/* This must be big enough for all operating systems uVisor runs on. */
#define UVISOR_SEMAPHORE_INTERNAL_SIZE (36)

/* An opaque structure, that one knows the size of so that they can allocate
 * memory. */
typedef struct uvisor_semaphore {
    uint8_t internal[UVISOR_SEMAPHORE_INTERNAL_SIZE];
} UVISOR_ALIGN(4) UvisorSemaphore;

#endif
