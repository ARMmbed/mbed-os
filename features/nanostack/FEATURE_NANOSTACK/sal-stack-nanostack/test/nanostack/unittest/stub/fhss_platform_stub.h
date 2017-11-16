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
#ifndef __FHSS_PLATFORM_STUB_H__
#define __FHSS_PLATFORM_STUB_H__

#ifdef __cplusplus
extern "C" {
#endif

typedef struct {
    bool channel_bool_value;
    uint32_t uint32_value;
    uint8_t address[8];
    int int_value;
} fhss_platform_stub_def;

extern fhss_platform_stub_def fhss_platform_stub;

uint32_t fhss_get_remaining_slots_stub(struct fhss_api *fhss_api);
uint32_t fhss_get_timestamp_stub(struct fhss_api *fhss_api);
int fhss_timer_stop_stub(struct fhss_api *fhss_api);
int fhss_timer_start(uint32_t slots, void (*fhss_timer_callback)(const fhss_api_t *api, uint16_t), const fhss_api_t *api);
int app_get_parent_mac_address_dummy(uint8_t mac_address[8]);

#ifdef __cplusplus
}
#endif

#endif // __FHSS_PLATFORM_STUB_H__
