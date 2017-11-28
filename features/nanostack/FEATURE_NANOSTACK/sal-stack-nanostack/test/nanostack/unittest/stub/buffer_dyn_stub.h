/*
 * Copyright (c) 2016, Arm Limited and affiliates.
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

#ifndef __BUFFER_DYN_STUB_H__
#define __BUFFER_DYN_STUB_H__

#ifdef __cplusplus
extern "C" {
#endif

#include "Core/include/ns_buffer.h"

typedef struct {
    uint16_t uint16_value;
    buffer_t *buffer_ptr;
} buffer_dyn_stub_def;

extern buffer_dyn_stub_def buffer_dyn_stub;

#ifdef __cplusplus
}
#endif

#endif // __BUFFER_DYN_STUB_H__
