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
#ifndef __UVISOR_API_DEBUG_H__
#define __UVISOR_API_DEBUG_H__

#include "api/inc/debug_exports.h"
#include "api/inc/uvisor_exports.h"

UVISOR_EXTERN_C_BEGIN

static UVISOR_FORCEINLINE void uvisor_debug_init(const TUvisorDebugDriver * const driver)
{
    uvisor_api.debug_init(driver);
}

UVISOR_EXTERN_C_END

#endif /* __UVISOR_API_DEBUG_H__ */
