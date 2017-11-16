/*
 * Copyright (c) 2015-2017, Arm Limited and affiliates.
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

#ifndef __MLE_STUB_H__
#define __MLE_STUB_H__

#ifdef __cplusplus
extern "C" {
#endif

typedef struct {
    bool bool_value;
    uint8_t expectedUint8;
    uint16_t expectedUint16;
    uint8_t *expectedUint8ptr;
    mle_neigh_table_entry_t *mle_neigh_table_entry_ptr;
} mle_stub_def;

extern mle_stub_def mle_stub;

#ifdef __cplusplus
}
#endif

#endif // __MLE_STUB_H__
