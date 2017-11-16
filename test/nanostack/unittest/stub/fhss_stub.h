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
#ifndef __FHSS_STUB_H__
#define __FHSS_STUB_H__

#ifdef __cplusplus
extern "C" {
#endif

typedef struct {
    int int_value;
    int8_t int8_value;
    bool bool_value;
    bool bad_channel_bool_value;
    bool channel_type_bool_value;
    bool tx_allowed_bool_value;
    fhss_structure_t fhss_struct;
    fhss_beacon_info_t beacon_info;
    fhss_failed_tx_t fhss_failed_tx;
} fhss_stub_def;

extern fhss_stub_def fhss_stub;

uint32_t fhss_get_remaining_slots_stub(struct fhss_api *fhss_api);

#ifdef __cplusplus
}
#endif

#endif // __FHSS_STUB_H__
