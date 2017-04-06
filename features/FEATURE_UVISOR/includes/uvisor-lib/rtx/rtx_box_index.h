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

#include "cmsis_os2.h"
#include "rtx_os.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct
{
    /* The uvisor box index must be placed at the beginning */
    UvisorBoxIndex index;

    /* ID of the mutex */
    osMutexId_t mutex_id;

    /* Attribute of the mutex */
    osMutexAttr_t mutex_attr;

    /* Internal data of the mutex */
    osRtxMutex_t mutex_data;
} RtxBoxIndex;

#ifdef __cplusplus
}   /* extern "C" */
#endif

#endif  /* __RTX_BOX_INDEX_H__ */
