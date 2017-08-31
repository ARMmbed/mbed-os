/*
 * Copyright (c) 2015-2017 ARM Limited. All Rights Reserved.
 *
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
#ifndef __EVENTOS_EVENT_STUB_H__
#define __EVENTOS_EVENT_STUB_H__

#ifdef __cplusplus
extern "C" {
#endif

#include <inttypes.h>
#include "eventOS_event.h"

typedef struct {
    void (*event_ptr)(arm_event_s *);
    int8_t int8_value;
} eventOs_event_stub_def;

extern eventOs_event_stub_def eventOs_event_stub;

#ifdef __cplusplus
}
#endif

#endif // __EVENTOS_EVENT_STUB_H__
