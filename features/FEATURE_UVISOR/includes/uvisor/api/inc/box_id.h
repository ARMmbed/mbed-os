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
#ifndef __UVISOR_API_BOX_ID_H__
#define __UVISOR_API_BOX_ID_H__

#include "api/inc/api.h"

UVISOR_EXTERN_C_BEGIN

/* Return the numeric box ID of the current box. */
int uvisor_box_id_self(void);

/* Return the numeric box ID of the box that is calling through the most recent
 * secure gateway. Return -1 if there is no secure gateway calling box. */
int uvisor_box_id_caller(void) UVISOR_DEPRECATED;

/* Copy the box namespace of the specified box ID to the memory provided by
 * box_namespace. The box_namespace's length must be at least
 * MAX_BOX_NAMESPACE_LENGTH bytes. Return how many bytes were copied into
 * box_namespace. Return UVISOR_ERROR_INVALID_BOX_ID if the provided box ID is
 * invalid. Return UVISOR_ERROR_BUFFER_TOO_SMALL if the provided box_namespace
 * is too small to hold MAX_BOX_NAMESPACE_LENGTH bytes. Return
 * UVISOR_ERROR_BOX_NAMESPACE_ANONYMOUS if the box is anonymous. */
static UVISOR_FORCEINLINE int uvisor_box_namespace(int box_id, char *box_namespace, size_t length)
{
    return uvisor_api.box_namespace(box_id, box_namespace, length);
}

UVISOR_EXTERN_C_END

#endif /* __UVISOR_API_BOX_ID_H__ */
