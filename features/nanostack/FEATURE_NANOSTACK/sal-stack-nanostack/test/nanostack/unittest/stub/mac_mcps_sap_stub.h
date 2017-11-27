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

#ifndef __MAC_MCPS_SAP_STUB_H__
#define __MAC_MCPS_SAP_STUB_H__

#ifdef __cplusplus
extern "C" {
#endif

#include "inttypes.h"
#include "mac_mcps.h"
#include "MAC/IEEE802_15_4/mac_mcps_sap.h"

typedef struct {
    int8_t int8_value;
    mac_pre_build_frame_t *frame_ptr;
    mac_pre_parsed_frame_t *pre_parsed;
    mcps_data_req_t *data_ptr;
} mac_mcps_sap_stub_def;

extern mac_mcps_sap_stub_def mac_mcps_sap_stub;

#ifdef __cplusplus
}
#endif

#endif // __MAC_MCPS_SAP_STUB_H__
