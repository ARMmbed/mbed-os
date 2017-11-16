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
#include "nsconfig.h"
#include <string.h>
#include "nsdynmemLIB.h"
#include "fhss_api.h"
#include "mac_api.h"
#include "sw_mac.h"
#include "MAC/IEEE802_15_4/mac_mlme.h"
#include "MAC/IEEE802_15_4/mac_defines.h"
#include "MAC/rf_driver_storage.h"
#include "MAC/IEEE802_15_4/mac_mcps_sap.h"
#include "MAC/IEEE802_15_4/mac_pd_sap.h"
#include "MAC/rf_driver_storage.h"
#include "MAC/IEEE802_15_4/sw_mac_internal.h"
#include "sw_mac_stub.h"

sw_mac_stub_def sw_mac_stub;

mac_api_t *ns_sw_mac_create(int8_t rf_driver_id, mac_description_storage_size_t *storage_sizes)
{
    return sw_mac_stub.mac_api_ptr;
}

mac_api_t *get_sw_mac_api(protocol_interface_rf_mac_setup_s *setup)
{
    return sw_mac_stub.mac_api_ptr;
}

protocol_interface_rf_mac_setup_s *get_sw_mac_ptr_by_mac_api(mac_api_t *api)
{
    return sw_mac_stub.rf_mac_ptr;
}

protocol_interface_rf_mac_setup_s *get_sw_mac_ptr_by_fhss_api(const fhss_api_t *api)
{
    return sw_mac_stub.rf_mac_ptr;
}

protocol_interface_rf_mac_setup_s *get_sw_mac_ptr_by_driver_id(int8_t id)
{
    return sw_mac_stub.rf_mac_ptr;
}

void mlme_req(const mac_api_t* api, mlme_primitive id, const void *data)
{
}

void mcps_req(const mac_api_t* api, const mcps_data_req_t *data)
{
}


protocol_interface_rf_mac_setup_s *get_sw_mac_ptr_by_timer(int8_t id, arm_nwk_timer_id_e type)
{
    return sw_mac_stub.rf_mac_ptr;
}

void sw_mac_stats_update(protocol_interface_rf_mac_setup_s *setup, mac_stats_type_t type, uint32_t update_val)
{

}
