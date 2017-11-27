/*
 * Copyright (c) 2016-2017, Arm Limited and affiliates.
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
#ifndef __MAC_HELPER_STUB_H__
#define __MAC_HELPER_STUB_H__

#ifdef __cplusplus
extern "C" {
#endif

#include "net_nwk_scan.h"

typedef struct {
    bool enable_storing;
    bool bool_value;
    int8_t int8_value;
    uint8_t uint8_value;
    uint8_t *uint8_ptr;
    uint16_t uint16_value;
    uint32_t uint32_value;
    nwk_pan_descriptor_t *desc_ptr;
} mac_helper_stub_def;

extern mac_helper_stub_def mac_helper_stub;

#ifdef __cplusplus
}
#endif

#endif // __MAC_HELPER_STUB_H__
