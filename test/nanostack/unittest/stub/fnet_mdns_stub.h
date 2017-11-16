/*
 * Copyright (c) 2017, Arm Limited and affiliates.
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

#ifndef __FNET_MDNS_STUB_H__
#define __FNET_MDNS_STUB_H__

#ifdef __cplusplus
extern "C" {
#endif

typedef struct {
    fnet_mdns_desc_t fnet_mdns_descriptor;
    fnet_mdns_service_desc_t fnet_mdns_service_descriptor;
    int announce_cnt;
    int release_cnt;
    int unregister_cnt;
} fnet_mdns_data_stub_t;

extern fnet_mdns_data_stub_t fnet_mdns_data_stub;

#ifdef __cplusplus
}
#endif

#endif // __FNET_MDNS_STUB_H__
