/*
 * Copyright (c) 2018-2019, Nuvoton Technology Corporation
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef __REGION_DEFS_H__
#define __REGION_DEFS_H__

#include "partition_M2351_mem.h"

#define S_CODE_START    (NU_ROM_START_S)
#define S_CODE_SIZE     (NU_ROM_SIZE_S)

#define S_DATA_START    (NU_RAM_START_S)
#define S_DATA_SIZE     (NU_RAM_SIZE_S)
#define S_DATA_LIMIT    (NU_RAM_START_S + NU_RAM_SIZE_S - 1)

#define NS_CODE_START   (NU_ROM_START_NS)
#define NS_CODE_SIZE    (NU_ROM_SIZE_NS)
#define NS_CODE_LIMIT   (NU_ROM_START_NS + NU_ROM_SIZE_NS - 1)

#define NS_DATA_START   (NU_RAM_START_NS)
#define NS_DATA_SIZE    (NU_RAM_SIZE_NS)
#define NS_DATA_LIMIT   (NU_RAM_START_NS + NU_RAM_SIZE_NS - 1)

#endif /* __REGION_DEFS_H__ */
