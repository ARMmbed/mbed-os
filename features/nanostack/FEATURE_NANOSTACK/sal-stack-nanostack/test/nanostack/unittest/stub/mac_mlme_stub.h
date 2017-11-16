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
#ifndef __MAC_MLME_STUB_H__
#define __MAC_MLME_STUB_H__

#ifdef __cplusplus
extern "C" {
#endif

#include "inttypes.h"
#include "mlme.h"
#include "MAC/IEEE802_15_4/mac_defines.h"
#include "MAC/rf_driver_storage.h"

typedef struct {
    bool bool_value;
    int int_value;
    int8_t int8_value;
    uint8_t uint8_value;
    uint16_t uint16_value;
    protocol_interface_rf_mac_setup_s *setup_ptr;
    uint8_t pollConfirm;
    //Beacon ind
    mlme_beacon_ind_t beacon_ind;
    uint8_t beacon_payload_buffer[120];
} mac_mlme_stub_def;

extern mac_mlme_stub_def mac_mlme_stub;

#ifdef __cplusplus
}
#endif

#endif // __MAC_MLME_STUB_H__
