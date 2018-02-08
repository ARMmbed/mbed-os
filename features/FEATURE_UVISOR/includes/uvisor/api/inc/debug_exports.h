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
#ifndef __UVISOR_API_DEBUG_EXPORTS_H__
#define __UVISOR_API_DEBUG_EXPORTS_H__

#include "api/inc/halt_exports.h"
#include <stdint.h>
#include "api/inc/vmpu_exports.h"


#define UVISOR_DEBUG_BOX_VERSION    (1)


/* Debug box driver
 * A constant instance of this struct must be instantiated by the unprivileged
 * code to setup a debug box.*/
typedef struct TUvisorDebugDriver {
    const uint32_t magic;
    const uint32_t version;
    const UvisorBoxConfig * const box_cfg_ptr;
    void (*halt_error)(THaltError, const THaltInfo *);
} TUvisorDebugDriver;

/* Number of handlers in the debug box driver */
#define DEBUG_BOX_HANDLERS_NUMBER (sizeof(TUvisorDebugDriver) / sizeof(void *))

#endif /* __UVISOR_API_DEBUG_EXPORTS_H__ */
