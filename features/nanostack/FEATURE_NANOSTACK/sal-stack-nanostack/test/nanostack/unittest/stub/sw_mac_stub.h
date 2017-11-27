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
#ifndef __SW_MAC_STUB_H__
#define __SW_MAC_STUB_H__

#ifdef __cplusplus
extern "C" {
#endif

#include "mac_api.h"
#include "MAC/IEEE802_15_4/mac_defines.h"
#include "MAC/rf_driver_storage.h"

typedef struct {
    mac_api_t *mac_api_ptr;
    protocol_interface_rf_mac_setup_s* rf_mac_ptr;
} sw_mac_stub_def;

extern sw_mac_stub_def sw_mac_stub;

#ifdef __cplusplus
}
#endif

#endif // __SW_MAC_STUB_H__
