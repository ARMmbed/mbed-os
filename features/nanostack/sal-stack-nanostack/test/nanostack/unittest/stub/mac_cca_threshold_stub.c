/*
 * Copyright (c) 2020, Arm Limited and affiliates.
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

#include "nsconfig.h"
#include <string.h>
#include "ns_types.h"
#include "ns_list.h"
#include "ns_trace.h"
#include "mac_defines.h"

int8_t mac_cca_threshold_update(protocol_interface_rf_mac_setup_s *rf_ptr, uint16_t event_data)
{
    return 0;
}

int8_t mac_cca_thr_deinit(protocol_interface_rf_mac_setup_s *rf_ptr)
{
    return 0;
}

int8_t mac_cca_thr_init(protocol_interface_rf_mac_setup_s *rf_ptr, uint8_t number_of_channels, int8_t default_dbm, int8_t high_limit, int8_t low_limit)
{
    return 0;
}

int8_t mac_cca_thr_get_dbm(protocol_interface_rf_mac_setup_s *rf_ptr, uint8_t channel)
{
    return 0;
}
