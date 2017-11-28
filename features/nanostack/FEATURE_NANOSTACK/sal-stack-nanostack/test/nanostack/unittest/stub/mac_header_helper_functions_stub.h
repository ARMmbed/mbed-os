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

#ifndef __MAC_HEADER_HELPER_FUNCTIONS_STUB_H__
#define __MAC_HEADER_HELPER_FUNCTIONS_STUB_H__

#include "inttypes.h"
#include "mac_common_defines.h"

typedef struct mac_header_helper_functions {
    uint8_t uint8_value;
    uint16_t uint16_value;
    uint32_t uint32_value;
    uint8_t *uint8_ptr;
    uint16_t pan_src;
    mac_fcf_sequence_t fcf;
    mac_aux_security_header_t security_header;
    uint8_t dst_address[8];
    uint8_t src_address[8];
}mac_header_helper_functions_def;

extern mac_header_helper_functions_def mac_header_helper_functions_stub;

#endif //__MAC_HEADER_HELPER_FUNCTIONS_STUB_H__
