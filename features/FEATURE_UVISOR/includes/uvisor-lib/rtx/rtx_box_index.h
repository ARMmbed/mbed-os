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
#ifndef __RTX_BOX_INDEX_H__
#define __RTX_BOX_INDEX_H__

#include "cmsis_os.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct
{
    /* The uvisor box index must be placed at the beginning */
    UvisorBoxIndex index;

    /* Id of the mutex */
    osMutexId mutex_id;
    /* Pointer to the data of the mutex */
    osMutexDef_t mutex;
    /* Internal data of the mutex */
    int32_t mutex_data[4];
} RtxBoxIndex;

#ifdef __cplusplus
}   /* extern "C" */
#endif

#endif  /* __RTX_BOX_INDEX_H__ */
