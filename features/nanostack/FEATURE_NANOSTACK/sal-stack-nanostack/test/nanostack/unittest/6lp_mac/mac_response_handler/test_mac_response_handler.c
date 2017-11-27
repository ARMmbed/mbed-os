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
#include "test_mac_response_handler.h"
#include "mac_api.h"
#include <string.h>
#include "NWK_INTERFACE/Include/protocol.h"
#include "Core/include/ns_buffer.h"

#include "mle.h"
#include "mle_stub.h"
#include "mac_helper_stub.h"
#include "buffer_dyn_stub.h"
#include "common_functions_stub.h"
#include "protocol_core_stub.h"
#include "thread_common_stub.h"

bool visited = false;

bool test_mcps_data_confirm_handler()
{
    mac_api_t api;

    mcps_data_confirm_handler(&api, NULL);

    return true;
}

static void mac_security_key_update_cb(protocol_interface_info_entry_t *cur, const mlme_security_t *security_params)
{
    (void) cur;
    (void) security_params;
}

bool test_mcps_data_indication_handler()
{
    mac_api_t api;
    protocol_interface_info_entry_t entry;
    arm_15_4_mac_parameters_t params;
    params.mac_next_key_index = 1;
    entry.mac_parameters = &params;
    entry.mac_security_key_usage_update_cb = mac_security_key_update_cb;
    mcps_data_ind_t data_ind;
    buffer_dyn_stub.buffer_ptr = NULL;
    mcps_data_indication_handler( &api, &data_ind );

    buffer_t buf;
    buffer_dyn_stub.buffer_ptr = &buf;
    uint8_t ubuf[8];
    common_functions_stub.uint8_ptr = &ubuf;
    protocol_core_stub.entry_ptr = &entry;
    data_ind.DstAddrMode = ADDR_802_15_4_SHORT;
    buf.dst_sa.address[2] = 0xff;
    buf.dst_sa.address[3] = 0xff;

    data_ind.Key.SecurityLevel = 1;
    data_ind.Key.KeyIndex = 1;
    thread_info_t info;
    thread_stub.info_ptr = &info;

    mcps_data_indication_handler( &api, &data_ind );

    data_ind.DstAddrMode = ADDR_802_15_4_LONG;
    buf.dst_sa.address[0] = 0xff;
    buf.dst_sa.address[1] = 0xff;
    mcps_data_indication_handler( &api, &data_ind );

    data_ind.Key.SecurityLevel = 0;
    mac_helper_stub.uint8_value = 1;
    mle_neigh_table_entry_t tent;
    mle_stub.mle_neigh_table_entry_ptr = &tent;
    tent.handshakeReady = 1;
    mcps_data_indication_handler( &api, &data_ind );
    if (!buf.options.ll_security_bypass_rx ) {
        return false;
    }

    mac_helper_stub.uint8_value = 0;
    mle_stub.mle_neigh_table_entry_ptr = NULL;
    mcps_data_indication_handler( &api, &data_ind );
    if (!buf.options.ll_security_bypass_rx ) {
        return false;
    }
    buf.options.ll_security_bypass_rx = false;
    mle_stub.mle_neigh_table_entry_ptr = &tent;
    mcps_data_indication_handler( &api, &data_ind );
    if (buf.options.ll_security_bypass_rx ) {
        return false;
    }
    return true;
}

void scan_callback(int8_t if_id, const mlme_scan_conf_t* conf)
{
    visited = true;
}

void mlme_test_req(const mac_api_t* api, mlme_primitive id, const void *data)
{
    visited = true;
}

bool test_mlme_confirm_handler()
{
    mac_api_t api;
    api.mlme_req = mlme_test_req;
    protocol_core_stub.entry_ptr = NULL;
    mlme_confirm_handler(&api, 0, NULL);
    protocol_interface_info_entry_t entry;
    arm_15_4_mac_parameters_t params;
    entry.mac_parameters = &params;
    entry.mac_api = &api;
    protocol_core_stub.entry_ptr = &entry;
    mlme_confirm_handler(&api, MLME_ASSOCIATE, NULL);
    mlme_confirm_handler(&api, MLME_DISASSOCIATE, NULL);

    mlme_get_conf_t dat;
    dat.attr = 254;
    mlme_confirm_handler(&api, MLME_GET, NULL);

    mlme_confirm_handler(&api, MLME_GET, &dat);

    dat.attr = macDeviceTable;
    dat.value_size = sizeof(mlme_device_descriptor_t) - 4;
    mlme_confirm_handler(&api, MLME_GET, &dat);

    mlme_device_descriptor_t desc;
    dat.value_pointer = &desc;
    dat.value_size = sizeof(mlme_device_descriptor_t);
    dat.status = MLME_SUCCESS;
    mle_stub.mle_neigh_table_entry_ptr = NULL;
    mlme_confirm_handler(&api, MLME_GET, &dat);

    mle_neigh_table_entry_t tent;
    mle_stub.mle_neigh_table_entry_ptr = &tent;
    desc.ShortAddress = 14;
    tent.short_adr = 15;
    visited = false;
    mlme_confirm_handler(&api, MLME_GET, &dat);
    if( !visited ){
        return false;
    }

    dat.attr = macFrameCounter;
    dat.value_size = 3;
    mlme_confirm_handler(&api, MLME_GET, &dat);

    dat.value_size = 4;
    mlme_confirm_handler(&api, MLME_GET, &dat);

    mlme_confirm_handler(&api, MLME_GTS, NULL);
    mlme_confirm_handler(&api, MLME_RESET, NULL);
    mlme_confirm_handler(&api, MLME_RX_ENABLE, NULL);

    visited = false;
    entry.scan_cb = &scan_callback;
    mlme_confirm_handler(&api, MLME_SCAN, NULL);
    if( !visited ){
        return false;
    }

    mlme_confirm_handler(&api, MLME_SET, NULL);
    mlme_confirm_handler(&api, MLME_START, NULL);

    mlme_confirm_handler(&api, MLME_POLL, NULL);

    mlme_confirm_handler(&api, MLME_BEACON_NOTIFY, NULL);
    mlme_confirm_handler(&api, MLME_ORPHAN, NULL);
    mlme_confirm_handler(&api, MLME_COMM_STATUS, NULL);
    mlme_confirm_handler(&api, MLME_SYNC, NULL);
    mlme_confirm_handler(&api, MLME_SYNC_LOSS, NULL);
    mlme_confirm_handler(&api, 254, NULL);
    return true;
}

void beacon_ind_cb(int8_t if_id, const mlme_beacon_ind_t* conf)
{
    visited = true;
}

void comm_status_cb(int8_t if_id, const mlme_comm_status_t* status)
{
    visited = true;
}

bool test_mlme_indication_handler()
{
    mlme_indication_handler(NULL, MLME_ASSOCIATE, NULL);
    mlme_indication_handler(NULL, MLME_DISASSOCIATE, NULL);

    mac_api_t api;
    visited = false;
    protocol_interface_info_entry_t entry;
    entry.beacon_cb = beacon_ind_cb;
    protocol_core_stub.entry_ptr = &entry;
    mlme_indication_handler(&api, MLME_BEACON_NOTIFY, NULL);
    if( !visited ){
        return false;
    }

    mlme_indication_handler(NULL, MLME_GTS, NULL);
    mlme_indication_handler(NULL, MLME_ORPHAN, NULL);

    mlme_comm_status_t data;
    visited = false;
    entry.comm_status_ind_cb = &comm_status_cb;
    mlme_indication_handler(&api, MLME_COMM_STATUS, &data);
    if( !visited ){
        return false;
    }

    const mlme_sync_loss_t sync_loss = {
        .LossReason = BEACON_LOST
    };
    mlme_indication_handler(&api, MLME_SYNC_LOSS, &sync_loss);
    mlme_indication_handler(NULL, MLME_GET, NULL);
    mlme_indication_handler(NULL, MLME_RESET, NULL);
    mlme_indication_handler(NULL, MLME_RX_ENABLE, NULL);
    mlme_indication_handler(NULL, MLME_SCAN, NULL);
    mlme_indication_handler(NULL, MLME_SET, NULL);
    mlme_indication_handler(NULL, MLME_START, NULL);
    mlme_indication_handler(NULL, MLME_SYNC, NULL);
    mlme_indication_handler(NULL, MLME_POLL, NULL);
    mlme_indication_handler(NULL, 254, NULL);
    return true;
}
