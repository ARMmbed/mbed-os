/** @file
 * Copyright (c) 2018, Arm Limited or its affiliates. All rights reserved.
 * SPDX-License-Identifier : Apache-2.0
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *  http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
**/

#ifndef _PAL_INTERNAL_TRUSTED_STORAGE_INTF_H_
#define _PAL_INTERNAL_TRUSTED_STORAGE_INTF_H_

#include <stdarg.h>
#include "pal_common.h"

#if PSA_INTERNAL_TRUSTED_STORAGE_IMPLEMENTED
#include "psa/internal_trusted_storage.h"
#endif

enum its_function_code {
    PAL_ITS_SET                         = 0x1,
    PAL_ITS_GET                         = 0x2,
    PAL_ITS_GET_INFO                    = 0x3,
    PAL_ITS_REMOVE                      = 0x4,
};

uint32_t pal_its_function(int type, va_list valist);
#endif /* _PAL_INTERNAL_TRUSTED_STORAGE_INTF_H_ */
