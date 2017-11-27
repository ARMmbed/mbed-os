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
#include "fhss_config.h"
#include "Service_Libs/fhss/fhss.h"
#include "Service_Libs/fhss/fhss_beacon.h"
#include "fhss_stub.h"

fhss_stub_def fhss_stub;

int8_t fhss_enable(fhss_api_t *fhss_api, const fhss_configuration_t *fhss_configuration, const fhss_timer_t *fhss_timer, fhss_statistics_t *fhss_statistics)
{
    return fhss_stub.int8_value;
}

int8_t fhss_disable(fhss_structure_t *fhss_structure)
{
    return fhss_stub.int8_value;
}

int8_t fhss_set_synch_configuration(fhss_structure_t *fhss_structure, const fhss_synch_configuration_t *fhss_synch_configuration)
{
    return fhss_stub.int8_value;
}

bool fhss_check_bad_channel(fhss_structure_t *fhss_structure, uint8_t handle)
{
    return fhss_stub.bad_channel_bool_value;
}

bool fhss_check_channel_type(fhss_structure_t *fhss_structure, bool is_bc, int frame_type)
{

    return fhss_stub.channel_type_bool_value;
}

bool fhss_check_tx_allowed(fhss_structure_t *fhss_structure, bool is_bc, uint16_t frame_length, int frame_type, uint8_t phy_header_length, uint8_t phy_tail_length)
{
    return fhss_stub.tx_allowed_bool_value;
}

fhss_structure_t *fhss_get_object_with_api(const fhss_api_t *fhss_api)
{
    if (fhss_stub.bool_value == false) {
        return NULL;
    }
    fhss_stub.fhss_struct.fhss_api = fhss_api;
    return &fhss_stub.fhss_struct;
}

int8_t fhss_set_datarate(fhss_structure_t *fhss_structure, uint32_t datarate)
{
    return 0;
}

int fhss_add_beacon_info(fhss_structure_t *fhss_structure, uint16_t pan_id, uint8_t *source_address, uint32_t timestamp, uint8_t *synch_info)
{
    return 0;
}

int fhss_compare_with_synch_parent_address(fhss_structure_t *fhss_structure, const uint8_t *source_addr)
{
    return fhss_stub.int8_value;
}

fhss_beacon_info_t *fhss_get_beacon_info(fhss_structure_t *fhss_structure, uint16_t pan_id)
{
	if (fhss_stub.beacon_info.pan_id == pan_id) {
		return &fhss_stub.beacon_info;
	} else {
		return NULL;
	}
}

int fhss_get_parent_address(fhss_structure_t *fhss_structure, uint8_t *p_addr)
{
    return fhss_stub.int_value;
}

int fhss_flush_beacon_info_storage(fhss_structure_t *fhss_structure)
{
    return 0;
}

int fhss_update_synch_parent_address(fhss_structure_t *fhss_structure)
{
    return 0;
}

int fhss_down(fhss_structure_t *fhss_structure)
{
    return 0;
}

fhss_failed_tx_t *fhss_failed_handle_find(fhss_structure_t *fhss_structure, uint8_t handle)
{
    if (fhss_stub.fhss_failed_tx.handle == handle) {
        return &fhss_stub.fhss_failed_tx;
    } else {
        return NULL;
    }
}

int fhss_failed_handle_add(fhss_structure_t *fhss_structure, uint8_t handle)
{
    fhss_stub.fhss_failed_tx.retries_done = 0;
    fhss_stub.fhss_failed_tx.handle = handle;
    return 0;
}

int fhss_failed_handle_remove(fhss_structure_t *fhss_structure, uint8_t handle)
{
    if (fhss_stub.fhss_failed_tx.handle == handle) {
        fhss_stub.fhss_failed_tx.handle = 255;
    }
    return 0;
}

bool fhss_is_synch_root(fhss_structure_t *fhss_structure)
{
	if (fhss_stub.fhss_struct.own_hop == 0) {
		return true;
	}
    return false;
}

void fhss_superframe_handler(const fhss_api_t *fhss_api, uint16_t delay)
{

}

void fhss_start_timer(fhss_structure_t *fhss_structure, uint32_t time, void (*callback)(const fhss_api_t *fhss_api, uint16_t))
{

}

uint32_t fhss_get_remaining_time_to_next_superframe(const fhss_structure_t *fhss_structure)
{
    return 0;
}

uint32_t fhss_get_tx_time(fhss_structure_t *fhss_structure, uint16_t bytes_to_send, uint8_t phy_header_length, uint8_t phy_tail_length)
{
    return 0;
}

int fhss_timeout_start(fhss_structure_t *fhss_structure, uint32_t time)
{
    return 0;
}

int fhss_timeout_stop(fhss_structure_t *fhss_structure)
{
    return 0;
}

int fhss_sync_with_beacon(fhss_structure_t *fhss_structure, const fhss_synchronization_beacon_payload_s *payload)
{
    return 0;
}

void fhss_clear_active_event(fhss_structure_t *fhss_structure, uint8_t event_type)
{

}

void fhss_update_beacon_info_lifetimes(fhss_structure_t *fhss_structure, uint32_t timestamp)
{

}
