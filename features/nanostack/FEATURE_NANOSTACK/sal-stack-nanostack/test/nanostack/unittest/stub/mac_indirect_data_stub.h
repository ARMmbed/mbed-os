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


#ifndef __MAC_INDIRECT_DATA_STUB_H__
#define __MAC_INDIRECT_DATA_STUB_H__

#ifdef __cplusplus
extern "C" {
#endif

typedef struct {
    void *indirect_buf;
    uint8_t value_uint8;
} indirect_data_stub_def_t;

extern indirect_data_stub_def_t indirect_data_stub_def;


#ifdef __cplusplus
}
#endif

#endif // __MAC_INDIRECT_DATA_STUB_H__
