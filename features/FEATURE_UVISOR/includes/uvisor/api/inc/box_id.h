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

#include "api/inc/uvisor_exports.h"

/* Return the numeric box ID of the current box. */
UVISOR_EXTERN int uvisor_box_id_self(void);

/* Return the numeric box ID of the box that is calling through the most recent
 * secure gateway. Return -1 if there is no secure gateway calling box. */
UVISOR_EXTERN int uvisor_box_id_caller(void) UVISOR_DEPRECATED;

#endif /* __UVISOR_API_BOX_ID_H__ */
