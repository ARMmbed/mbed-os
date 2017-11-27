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
#include "test_adaptation_interface.h"
#include <string.h>
#include "6LoWPAN/lowpan_adaptation_interface.h"
#include "NWK_INTERFACE/Include/protocol.h"
#include "MAC/rf_driver_storage.h"
#include "ns_error_types.h"
#include "mle.h"
#include "mlme.h"
#include "mac_api.h"

#include "address_stub.h"
#include "mle_stub.h"
#include "protocol_core_stub.h"
#include "mac_helper_stub.h"
#include "socket_stub.h"
#include "mesh_stub.h"
#include "rpl_data_stub.h"
#include "nsdynmemLIB_stub.h"
#include "buffer_dyn_stub.h"
#include "common_functions_stub.h"

static void mcps_data_req_cb(const mac_api_t* api, const mcps_data_req_t *data)
{

}

bool test_lowpan_adaptation_interface_init()
{
    if (-2 != lowpan_adaptation_interface_init(0, 0)) {
        return false;
    }

    if (-1 != lowpan_adaptation_interface_init(0, 127)) {
        return false;
    }

    nsdynmemlib_stub.returnCounter = 0;

    if (-1 != lowpan_adaptation_interface_init(0, 127)) {
        return false;
    }
    nsdynmemlib_stub.returnCounter = 1;
    if (-1 != lowpan_adaptation_interface_init(0, 127)) {
        return false;
    }

    nsdynmemlib_stub.returnCounter = 2;
    if (0 != lowpan_adaptation_interface_init(0, 127)) {
        return false;
    }

    nsdynmemlib_stub.returnCounter = 2;
    if (0 != lowpan_adaptation_interface_init(0, 127)) {
        return false;
    }

    lowpan_adaptation_interface_free(0);

    return true;
}

bool test_lowpan_adaptation_interface_free()
{
    if (-1 != lowpan_adaptation_interface_free(0)) {
        return false;
    }
    nsdynmemlib_stub.returnCounter = 2;
    lowpan_adaptation_interface_init(0, 127);

    if (-1 != lowpan_adaptation_interface_free(1)) {
        return false;
    }

    if (0 != lowpan_adaptation_interface_free(0)) {
        return false;
    }

    return true;
}

bool test_lowpan_adaptation_interface_reset()
{

    mac_api_t api;
    protocol_interface_info_entry_t entry;
    memset(&entry, 0, sizeof(entry));
    arm_15_4_mac_parameters_t params;
    params.mac_next_key_index = 1;
    entry.mac_parameters = &params;
    entry.mac_api = &api;
    api.mcps_data_req = &mcps_data_req_cb;
    buffer_dyn_stub.buffer_ptr = NULL;

    buffer_t buf;
    memset(&buf, 0, sizeof(buf));
    buffer_dyn_stub.buffer_ptr = &buf;
    buf.dst_sa.addr_type = ADDR_802_15_4_SHORT;
    protocol_core_stub.entry_ptr = &entry;
    buf.buf_ptr = 10;
    buf.buf_end = 80;

    if (-1 != lowpan_adaptation_interface_reset(0)) {
        return false;
    }

    nsdynmemlib_stub.returnCounter = 2;
    lowpan_adaptation_interface_init(0, 127);

    //Set 2 direct and 2 indirect buffer
    buf.link_specific.ieee802_15_4.indirectTxProcess = false;
    lowpan_adaptation_interface_tx(&entry, &buf);
    lowpan_adaptation_interface_tx(&entry, &buf);
    lowpan_adaptation_interface_tx(&entry, &buf);
    buf.link_specific.ieee802_15_4.indirectTxProcess = true;
    nsdynmemlib_stub.returnCounter = 2;
    lowpan_adaptation_interface_tx(&entry, &buf);
    lowpan_adaptation_interface_tx(&entry, &buf);

    if (0 != lowpan_adaptation_interface_reset(0)) {
        return false;
    }
    lowpan_adaptation_interface_free(0);

    return true;
}

bool test_lowpan_adapatation_data_process_tx_preprocess()
{
    mac_api_t api;
    protocol_interface_info_entry_t entry;
    arm_15_4_mac_parameters_t params;
    params.mac_next_key_index = 1;
    entry.mac_parameters = &params;
    entry.mac_api = &api;
    entry.id = 0;
    api.mcps_data_req = &mcps_data_req_cb;
    mle_stub.mle_neigh_table_entry_ptr = NULL;

    buffer_t buf;
    memset(&buf, 0, sizeof(buf));
    buf.dst_sa.addr_type = ADDR_NONE;
    protocol_core_stub.entry_ptr = &entry;
    buffer_dyn_stub.buffer_ptr = &buf;
    buf.buf_ptr = 10;
    buf.buf_end = 80;
    socket_stub.buffer_ptr = NULL;
    if (lowpan_adaptation_data_process_tx_preprocess(&entry, &buf)) {
        return false;
    }

    mle_neigh_table_entry_t mle;
    mle_stub.mle_neigh_table_entry_ptr = &mle;
    mle.mode = 8;
    mle.handshakeReady = false;
    mle.thread_commission = false;
    buf.dst_sa.addr_type = ADDR_802_15_4_SHORT;
    buf.options.ll_security_bypass_tx = false;
    socket_stub.buffer_ptr = &buf;
    if (lowpan_adaptation_data_process_tx_preprocess(&entry, &buf)) {
        return false;
    }

    //Test Broadcast
    buf.dst_sa.addr_type = ADDR_802_15_4_SHORT;
    mle.handshakeReady = true;
    address_stub.uint8_value = 0;
    if (!lowpan_adaptation_data_process_tx_preprocess(&entry, &buf)) {
        return false;
    }

    if (buf.link_specific.ieee802_15_4.indirectTxProcess
            || buf.link_specific.ieee802_15_4.requestAck) {

        return false;
    }

    address_stub.uint8_value = 2;
    buf.dst_sa.addr_type = ADDR_802_15_4_LONG;
    buf.link_specific.ieee802_15_4.key_id_mode = B_SECURITY_KEY_ID_MODE_DEFAULT;

    mle_stub.mle_neigh_table_entry_ptr = NULL;
    if (!lowpan_adaptation_data_process_tx_preprocess(&entry, &buf)) {
        return false;
    }

    if (buf.link_specific.ieee802_15_4.indirectTxProcess
            || !buf.link_specific.ieee802_15_4.requestAck
            || buf.link_specific.ieee802_15_4.key_id_mode
                    == B_SECURITY_KEY_ID_MODE_DEFAULT) {
        return false;
    }

    mle_stub.mle_neigh_table_entry_ptr = &mle;

    if (!lowpan_adaptation_data_process_tx_preprocess(&entry, &buf)) {
        return false;
    }

    if (buf.link_specific.ieee802_15_4.indirectTxProcess
            || !buf.link_specific.ieee802_15_4.requestAck
            || !buf.link_specific.ieee802_15_4.key_id_mode
                    == B_SECURITY_KEY_ID_MODE_DEFAULT) {
        return false;
    }
    mle.mode = 0;

    if (!lowpan_adaptation_data_process_tx_preprocess(&entry, &buf)) {
        return false;
    }

    if (!buf.link_specific.ieee802_15_4.indirectTxProcess
            || !buf.link_specific.ieee802_15_4.requestAck
            || !buf.link_specific.ieee802_15_4.key_id_mode
                    == B_SECURITY_KEY_ID_MODE_DEFAULT) {
        return false;
    }

    return true;
}

static void tester_mcps_purge_request(const mac_api_t* api,
                                      const mcps_purge_t *data)
{
    (void) api;
    (void) data;
}

bool test_lowpan_adaptation_indirect_purge()
{
    mac_api_t api;
    protocol_interface_info_entry_t entry;
    arm_15_4_mac_parameters_t params;
    params.mac_next_key_index = 1;
    params.mac_channel = 11;
    entry.mac_parameters = &params;
    entry.mac_api = &api;
    entry.id = 0;
    api.mcps_data_req = &mcps_data_req_cb;
    api.mcps_purge_req = &tester_mcps_purge_request;
    mle_stub.mle_neigh_table_entry_ptr = NULL;

    buffer_t *test_buf = malloc(sizeof(buffer_t) + 2100);
    memset(test_buf, 0, sizeof(buffer_t));

    test_buf->dst_sa.addr_type = ADDR_802_15_4_SHORT;
    protocol_core_stub.entry_ptr = &entry;
    buffer_dyn_stub.buffer_ptr = test_buf;
    test_buf->buf_ptr = 10;
    test_buf->buf_end = 70;

    socket_stub.buffer_ptr = test_buf;

    entry.mac_api = &api;
    entry.id = 0;
    api.mcps_data_req = &mcps_data_req_cb;

    //Test Indirect data allocation fail
    nsdynmemlib_stub.returnCounter = 2;
    lowpan_adaptation_interface_init(0, 127);

    test_buf->link_specific.ieee802_15_4.indirectTxProcess = true;
    nsdynmemlib_stub.returnCounter = 2;
    if (0 != lowpan_adaptation_interface_tx(&entry, test_buf)) {
        return false;
    }

    buffer_t *test_buf2 = malloc(sizeof(buffer_t) + 2100);
    memset(test_buf2, 0, sizeof(buffer_t));
    test_buf2->buf_ptr = 10;
    test_buf2->buf_end = 70;

    test_buf2->dst_sa.addr_type = ADDR_802_15_4_LONG;
    test_buf2->link_specific.ieee802_15_4.indirectTxProcess = true;
    nsdynmemlib_stub.returnCounter = 2;
    if (0 != lowpan_adaptation_interface_tx(&entry, test_buf2)) {
        return false;
    }
    entry.id = 2;
    if (-1
            != lowpan_adaptation_indirect_free_messages_from_queues_by_address(
                    &entry, test_buf->dst_sa.address,
                    test_buf->dst_sa.addr_type)) {
        return false;
    }
    entry.id = 0;
    if (-0
            != lowpan_adaptation_indirect_free_messages_from_queues_by_address(
                    &entry, test_buf->dst_sa.address,
                    test_buf->dst_sa.addr_type)) {
        return false;
    }

    if (-0
            != lowpan_adaptation_indirect_free_messages_from_queues_by_address(
                    &entry, test_buf2->dst_sa.address, ADDR_NONE)) {
        return false;
    }

    // purge with addr none
    test_buf2->dst_sa.addr_type = ADDR_NONE;
    if (-0
            != lowpan_adaptation_indirect_free_messages_from_queues_by_address(
                    &entry, test_buf2->dst_sa.address, ADDR_NONE)) {
        return false;
    }

    test_buf2->dst_sa.addr_type = ADDR_802_15_4_LONG;

    test_buf2->dst_sa.address[2] = 2;
    if (-0
            != lowpan_adaptation_indirect_free_messages_from_queues_by_address(
                    &entry, test_buf2->dst_sa.address,
                    test_buf2->dst_sa.addr_type)) {
        return false;
    }

    test_buf2->dst_sa.address[2] = 0;

    if (-0
            != lowpan_adaptation_indirect_free_messages_from_queues_by_address(
                    &entry, test_buf2->dst_sa.address,
                    test_buf2->dst_sa.addr_type)) {
        return false;
    }

    lowpan_adaptation_interface_free(0);

    nsdynmemlib_stub.returnCounter = 0;
    free(test_buf);
    free(test_buf2);
    return true;

}

bool test_lowpan_adaptation_interface_tx()
{
    mac_api_t api;
    protocol_interface_info_entry_t entry;
    arm_15_4_mac_parameters_t params;
    params.mac_next_key_index = 1;
    params.mac_channel = 11;
    entry.mac_parameters = &params;
    entry.mac_api = &api;
    entry.id = 0;
    api.mcps_data_req = &mcps_data_req_cb;
    api.mcps_purge_req = &tester_mcps_purge_request;
    mle_stub.mle_neigh_table_entry_ptr = NULL;

    buffer_t *test_buf = malloc(sizeof(buffer_t) + 2100);
    memset(test_buf, 0, sizeof(buffer_t) + 2100);

    test_buf->dst_sa.addr_type = ADDR_802_15_4_SHORT;
    protocol_core_stub.entry_ptr = &entry;
    buffer_dyn_stub.buffer_ptr = test_buf;
    test_buf->buf_ptr = 10;
    test_buf->buf_end = 70;

    socket_stub.buffer_ptr = NULL;

    if (-1 != lowpan_adaptation_interface_tx(&entry, NULL)) {
        return false;
    }

    if (-1 != lowpan_adaptation_interface_tx(NULL, test_buf)) {
        return false;
    }

    entry.mac_api = NULL;
    socket_stub.buffer_ptr = test_buf;
    if (-1 != lowpan_adaptation_interface_tx(&entry, test_buf)) {
        return false;
    }

    entry.mac_api = &api;
    api.mcps_data_req = NULL;
    socket_stub.buffer_ptr = NULL;
    if (-1 != lowpan_adaptation_interface_tx(&entry, test_buf)) {
        return false;
    }

    entry.id = 0;
    api.mcps_data_req = &mcps_data_req_cb;
    if (-1 != lowpan_adaptation_interface_tx(&entry, test_buf)) {
        return false;
    }

    //Test Indirect data allocation fail
    nsdynmemlib_stub.returnCounter = 2;
    lowpan_adaptation_interface_init(0, 127);

    if (0 != lowpan_adaptation_indirect_queue_params_set(&entry, 106, 1, 2)) {
        return false;
    }

    test_buf->link_specific.ieee802_15_4.indirectTxProcess = true;
    if (-1 != lowpan_adaptation_interface_tx(&entry, test_buf)) {
        return false;
    }

    test_buf->buf_end = 2100;
    nsdynmemlib_stub.returnCounter = 1;
    test_buf->link_specific.ieee802_15_4.indirectTxProcess = true;
    if (-1 != lowpan_adaptation_interface_tx(&entry, test_buf)) {
        return false;
    }

    nsdynmemlib_stub.returnCounter = 2;
    //Fragmented
    mac_helper_stub.uint16_value = 69;
    if (-1 != lowpan_adaptation_interface_tx(&entry, test_buf)) {
        return false;
    }

    test_buf->buf_end = 300;

    uint8_t *ptr = test_buf->buf + test_buf->buf_ptr;
    *ptr = 0x80;
    ptr += 4;
    *ptr = 0x50;
    mesh_stub.uint8_value = 66;

    nsdynmemlib_stub.returnCounter = 2;
    if (-1 != lowpan_adaptation_interface_tx(&entry, test_buf)) {
        return false;
    }

    mesh_stub.uint8_value = 4;

    test_buf->buf_ptr = 10;
    nsdynmemlib_stub.returnCounter = 2;
    if (0 != lowpan_adaptation_interface_tx(&entry, test_buf)) {
        return false;
    }

    if (test_buf->buf_ptr != 16) {
        return false;
    }

    // check force security on fragmented packets
    test_buf->options.ll_security_bypass_tx = true;
    test_buf->options.ll_sec_bypass_frag_deny = true;
    test_buf->buf_end = 300;
    nsdynmemlib_stub.returnCounter = 2;
    if (0 != lowpan_adaptation_interface_tx(&entry, test_buf)) {
        return false;
    }
    if (test_buf->options.ll_security_bypass_tx) {
        return false;
    }

    //Not fragmented
    test_buf->link_specific.ieee802_15_4.requestAck = true;
    mac_helper_stub.uint16_value = 110;
    test_buf->buf_end = 90;
    nsdynmemlib_stub.returnCounter = 1;
    if (0 != lowpan_adaptation_interface_tx(&entry, test_buf)) {
        return false;
    }

    mac_helper_stub.uint8_value = 4;
    test_buf->link_specific.ieee802_15_4.key_id_mode =
            B_SECURITY_KEY_ID_IMPLICIT;
    nsdynmemlib_stub.returnCounter = 1;
    if (0 != lowpan_adaptation_interface_tx(&entry, test_buf)) {
        return false;
    }

    test_buf->link_specific.ieee802_15_4.key_id_mode =
            B_SECURITY_KEY_ID_MODE_DEFAULT;
    nsdynmemlib_stub.returnCounter = 1;
    if (0 != lowpan_adaptation_interface_tx(&entry, test_buf)) {
        return false;
    }

    test_buf->link_specific.ieee802_15_4.key_id_mode = B_SECURITY_KEY_ID_2;
    nsdynmemlib_stub.returnCounter = 1;
    if (0 != lowpan_adaptation_interface_tx(&entry, test_buf)) {
        return false;
    }

    test_buf->link_specific.ieee802_15_4.useDefaultPanId = false;
    test_buf->link_specific.ieee802_15_4.dstPanId = 0xffff;
    test_buf->link_specific.ieee802_15_4.requestAck = false;
    test_buf->link_specific.ieee802_15_4.indirectTxProcess = false;
    test_buf->link_specific.ieee802_15_4.rf_channel_switch = true;
    test_buf->link_specific.ieee802_15_4.selected_channel = 12;

    if (0 != lowpan_adaptation_interface_tx(&entry, test_buf)
            && test_buf->link_specific.ieee802_15_4.selected_channel == 12) {
        return false;
    }

    test_buf->link_specific.ieee802_15_4.useDefaultPanId = true;
    test_buf->link_specific.ieee802_15_4.requestAck = true;

    if (0 != lowpan_adaptation_interface_tx(&entry, test_buf)) {
        return false;
    }

    buffer_t *test_buf2 = malloc(sizeof(buffer_t) + 2100);
    memset(test_buf2, 0, sizeof(buffer_t));

    test_buf2->dst_sa.addr_type = ADDR_802_15_4_SHORT;

    test_buf2->link_specific.ieee802_15_4.useDefaultPanId = true;
    test_buf2->link_specific.ieee802_15_4.requestAck = true;
    test_buf2->link_specific.ieee802_15_4.indirectTxProcess = false;
    test_buf2->link_specific.ieee802_15_4.rf_channel_switch = true;
    test_buf2->link_specific.ieee802_15_4.selected_channel = 12;
    test_buf2->buf_end = 90;
    test_buf2->priority = QOS_HIGH;

    if (0 != lowpan_adaptation_interface_tx(&entry, test_buf2)) {
        return false;
    }

    buffer_t *test_buf3 = malloc(sizeof(buffer_t) + 2100);
    memset(test_buf3, 0, sizeof(buffer_t) + 2100);

    test_buf3->dst_sa.addr_type = ADDR_802_15_4_SHORT;

    test_buf3->link_specific.ieee802_15_4.requestAck = true;
    test_buf3->buf_end = 90;
    test_buf3->priority = QOS_HIGH;

    if (0 != lowpan_adaptation_interface_tx(&entry, test_buf3)) {
        return false;
    }

    lowpan_adaptation_interface_free(0);
    nsdynmemlib_stub.returnCounter = 0;
    free(test_buf);
    free(test_buf2);
    free(test_buf3);
    return true;
}

bool test_lowpan_adaptation_interface_tx_confirm()
{
    mac_api_t api;
    mcps_data_conf_t data_confirm;
    protocol_interface_info_entry_t entry;
    arm_15_4_mac_parameters_t params;
    params.mac_next_key_index = 1;
    params.mac_channel = 11;
    memset(&entry, 0, sizeof(entry));
    entry.mac_parameters = &params;
    entry.mac_api = &api;
    entry.id = 0;
    api.mcps_data_req = &mcps_data_req_cb;
    mle_stub.mle_neigh_table_entry_ptr = NULL;

    mac_helper_stub.uint16_value = 110;
    mac_helper_stub.uint8_value = 4;

    memset(&data_confirm, 0, sizeof(mcps_data_conf_t));

    buffer_t *test_buf = malloc(sizeof(buffer_t) + 2100);
    memset(test_buf, 0, sizeof(buffer_t) + 2100);

    entry.mac_parameters->mac_in_direct_entry_timeout = 14000;
    test_buf->link_specific.ieee802_15_4.useDefaultPanId = true;
    test_buf->dst_sa.addr_type = ADDR_802_15_4_SHORT;
    protocol_core_stub.entry_ptr = &entry;
    buffer_dyn_stub.buffer_ptr = test_buf;
    test_buf->buf_ptr = 10;
    test_buf->buf_end = 70;
    test_buf->link_specific.ieee802_15_4.requestAck = true;
    test_buf->link_specific.ieee802_15_4.key_id_mode =
            B_SECURITY_KEY_ID_MODE_DEFAULT;
    test_buf->link_specific.ieee802_15_4.rf_channel_switch = true;
    test_buf->link_specific.ieee802_15_4.selected_channel = 11;

    buffer_t *test_buf2 = malloc(sizeof(buffer_t) + 2100);
    memset(test_buf2, 0, sizeof(buffer_t) + 2100);
    test_buf2->dst_sa.addr_type = ADDR_802_15_4_SHORT;
    test_buf2->buf_ptr = 10;
    test_buf2->buf_end = 70;
    test_buf2->link_specific.ieee802_15_4.requestAck = true;
    test_buf2->link_specific.ieee802_15_4.rf_channel_switch = true;
    test_buf2->link_specific.ieee802_15_4.selected_channel = 12;

    buffer_t *test_buf3 = malloc(sizeof(buffer_t) + 2100);
    memset(test_buf3, 0, sizeof(buffer_t) + 2100);
    test_buf3->dst_sa.addr_type = ADDR_802_15_4_SHORT;
    test_buf3->buf_ptr = 10;
    test_buf3->buf_end = 70;
    test_buf3->link_specific.ieee802_15_4.requestAck = true;
    test_buf3->link_specific.ieee802_15_4.indirectTxProcess = true;

    entry.mac_api = &api;

    entry.id = 0;
    api.mcps_data_req = &mcps_data_req_cb;

    if (-1 != lowpan_adaptation_interface_tx_confirm(NULL, &data_confirm)) {
        lowpan_adaptation_interface_free(0);
        free(test_buf);
        free(test_buf2);
        free(test_buf3);
        return false;
    }

    if (-1 != lowpan_adaptation_interface_tx_confirm(&entry, NULL)) {
        lowpan_adaptation_interface_free(0);
        free(test_buf);
        free(test_buf2);
        free(test_buf3);
        return false;
    }

    if (-1 != lowpan_adaptation_interface_tx_confirm(&entry, &data_confirm)) {
        lowpan_adaptation_interface_free(0);
        free(test_buf);
        free(test_buf2);
        free(test_buf3);
        return false;
    }
    nsdynmemlib_stub.returnCounter = 2;
    lowpan_adaptation_interface_init(0, 127);

    if (-1 != lowpan_adaptation_interface_tx_confirm(&entry, &data_confirm)) {
        lowpan_adaptation_interface_free(0);
        free(test_buf);
        free(test_buf2);
        free(test_buf3);
        return false;
    }
    test_buf2->ip_routed_up = true;

    lowpan_adaptation_interface_tx(&entry, test_buf);
    lowpan_adaptation_interface_tx(&entry, test_buf2);
    nsdynmemlib_stub.returnCounter = 2;
    lowpan_adaptation_interface_tx(&entry, test_buf3);

    data_confirm.msduHandle = 10;
    if (-1 != lowpan_adaptation_interface_tx_confirm(&entry, &data_confirm)) {
        lowpan_adaptation_interface_free(0);
        free(test_buf);
        free(test_buf2);
        free(test_buf3);
        return false;
    }

    data_confirm.msduHandle = test_buf->seq;
    data_confirm.status = MLME_SUCCESS;
    if (0 != lowpan_adaptation_interface_tx_confirm(&entry, &data_confirm)) {
        lowpan_adaptation_interface_free(0);
        free(test_buf);
        free(test_buf2);
        free(test_buf3);
        return false;
    }

    data_confirm.msduHandle = test_buf2->seq;
    data_confirm.status = MLME_BUSY_CHAN;
    if (0 != lowpan_adaptation_interface_tx_confirm(&entry, &data_confirm)) {
        lowpan_adaptation_interface_free(0);
        free(test_buf);
        free(test_buf2);
        free(test_buf3);
        return false;
    }

    data_confirm.msduHandle = test_buf2->seq;
    data_confirm.status = MLME_SUCCESS;
    if (0 != lowpan_adaptation_interface_tx_confirm(&entry, &data_confirm)) {
        lowpan_adaptation_interface_free(0);
        free(test_buf);
        free(test_buf2);
        free(test_buf3);
        return false;
    }

    data_confirm.msduHandle = test_buf3->seq;
    data_confirm.status = MLME_TRANSACTION_EXPIRED;
    if (0 != lowpan_adaptation_interface_tx_confirm(&entry, &data_confirm)) {
        lowpan_adaptation_interface_free(0);
        free(test_buf);
        free(test_buf2);
        free(test_buf3);
        return false;
    }

    data_confirm.msduHandle = test_buf3->seq;
    data_confirm.status = MLME_SUCCESS;
    if (0 != lowpan_adaptation_interface_tx_confirm(&entry, &data_confirm)) {
        lowpan_adaptation_interface_free(0);
        free(test_buf);
        free(test_buf2);
        free(test_buf3);
        return false;
    }
    buffer_routing_info_t buf_route;
    test_buf2->route = &buf_route;
    rpl_data_stub.bool_value = true;
    lowpan_adaptation_interface_tx(&entry, test_buf2);
    data_confirm.msduHandle = test_buf2->seq;
    data_confirm.status = MLME_TX_NO_ACK;
    if (0 != lowpan_adaptation_interface_tx_confirm(&entry, &data_confirm)) {
        lowpan_adaptation_interface_free(0);
        free(test_buf);
        free(test_buf2);
        free(test_buf3);
        return false;
    }

    test_buf2->buf_end = 200;
    test_buf2->ip_routed_up = false;
    lowpan_adaptation_interface_tx(&entry, test_buf2);
    data_confirm.msduHandle = test_buf2->seq;
    data_confirm.status = MLME_SUCCESS;

    if (0 != lowpan_adaptation_interface_tx_confirm(&entry, &data_confirm)) {
        lowpan_adaptation_interface_free(0);
        free(test_buf);
        free(test_buf2);
        free(test_buf3);
        return false;
    }

    if (0 != lowpan_adaptation_interface_tx_confirm(&entry, &data_confirm)) {
        lowpan_adaptation_interface_free(0);
        free(test_buf);
        free(test_buf2);
        free(test_buf3);
        return false;
    }

    if (test_buf2->buf_end != test_buf2->buf_ptr) {
        lowpan_adaptation_interface_free(0);
        free(test_buf);
        free(test_buf2);
        free(test_buf3);
        return false;
    }

    if (-1 != lowpan_adaptation_interface_tx_confirm(&entry, &data_confirm)) {
        lowpan_adaptation_interface_free(0);
        free(test_buf);
        free(test_buf2);
        free(test_buf3);
        return false;
    }
    //Test Fragmentation fail
    test_buf2->buf_ptr = 10;
    test_buf2->buf_end = 200;
    socket_stub.buffer_ptr = test_buf2;
    lowpan_adaptation_interface_tx(&entry, test_buf2);
    data_confirm.msduHandle = test_buf2->seq;
    data_confirm.status = MLME_SUCCESS;

    if (0 != lowpan_adaptation_interface_tx_confirm(&entry, &data_confirm)) {
        lowpan_adaptation_interface_free(0);
        free(test_buf);
        free(test_buf2);
        free(test_buf3);
        return false;
    }
    data_confirm.status = MLME_TX_NO_ACK;
    if (0 != lowpan_adaptation_interface_tx_confirm(&entry, &data_confirm)) {
        lowpan_adaptation_interface_free(0);
        free(test_buf);
        free(test_buf2);
        free(test_buf3);
        return false;
    }
    socket_stub.buffer_ptr = NULL;

    test_buf3->buf_end = 300;
    nsdynmemlib_stub.returnCounter = 2;
    lowpan_adaptation_interface_tx(&entry, test_buf3);
    data_confirm.msduHandle = test_buf3->seq;
    data_confirm.status = MLME_SUCCESS;

    if (0 != lowpan_adaptation_interface_tx_confirm(&entry, &data_confirm)) {
        lowpan_adaptation_interface_free(0);
        free(test_buf);
        free(test_buf2);
        free(test_buf3);
        return false;
    }
    data_confirm.status = MLME_BUSY_CHAN;
    if (0 != lowpan_adaptation_interface_tx_confirm(&entry, &data_confirm)) {
        lowpan_adaptation_interface_free(0);
        free(test_buf);
        free(test_buf2);
        free(test_buf3);
        return false;
    }
    data_confirm.status = MLME_SUCCESS;

    if (0 != lowpan_adaptation_interface_tx_confirm(&entry, &data_confirm)) {
        lowpan_adaptation_interface_free(0);
        free(test_buf);
        free(test_buf2);
        free(test_buf3);
        return false;
    }

    if (0 != lowpan_adaptation_interface_tx_confirm(&entry, &data_confirm)) {
        lowpan_adaptation_interface_free(0);
        free(test_buf);
        free(test_buf2);
        free(test_buf3);
        return false;
    }

    // check route_up with fragmentation
    nsdynmemlib_stub.returnCounter = 2;
    test_buf3->buf_end += 200;
    test_buf3->ip_routed_up = true;
    lowpan_adaptation_interface_tx(&entry, test_buf3);
    data_confirm.msduHandle = test_buf3->seq;
    data_confirm.status = MLME_SUCCESS;
    if (0 != lowpan_adaptation_interface_tx_confirm(&entry, &data_confirm)) {
        lowpan_adaptation_interface_free(0);
        free(test_buf);
        free(test_buf2);
        free(test_buf3);
        return false;
    }
    if (0 != lowpan_adaptation_interface_tx_confirm(&entry, &data_confirm)) {
        lowpan_adaptation_interface_free(0);
        free(test_buf);
        free(test_buf2);
        free(test_buf3);
        return false;
    }

    lowpan_adaptation_interface_free(0);
    free(test_buf);
    free(test_buf2);
    free(test_buf3);
    return true;

}

static buffer_t * test_fragment_first_96(buffer_t *buf, uint8_t index)
{
    common_functions_stub.readuint16_from_queue = 2;
    common_functions_stub.uint16_value_queue[1] = 232;
    common_functions_stub.uint16_value_queue[0] = 100 + index;
    buf->buf_ptr = 0;
    buf->buf_end = 100;

    uint8_t *ptr = buf->buf + buf->buf_ptr;
    *ptr = 0xc0; //first sequency
    return buf;

}

static buffer_t * test_fragment_last_96(buffer_t *buf, uint8_t index)
{
    common_functions_stub.readuint16_from_queue = 2;
    common_functions_stub.uint16_value_queue[1] = 232;
    common_functions_stub.uint16_value_queue[0] = 100 + index;
    buf->buf_ptr = 0;
    buf->buf_end = 101;

    uint8_t *ptr = buf->buf + buf->buf_ptr;
    *ptr = 0x20; //sub sequency
    ptr += 4;
    *ptr = (96 + 40) / 8; //Set offset
    return buf;

}

bool test_lowpan_adaptation_tx_active()
{
    mac_api_t api;
    protocol_interface_info_entry_t entry;
    arm_15_4_mac_parameters_t params;
    params.mac_next_key_index = 1;
    entry.mac_parameters = &params;
    entry.mac_api = &api;
    entry.id = 0;
    api.mcps_data_req = &mcps_data_req_cb;
    if (lowpan_adaptation_tx_active(0)) {
        return false;
    }

    nsdynmemlib_stub.returnCounter = 2;
    lowpan_adaptation_interface_init(0, 127);
    if (lowpan_adaptation_tx_active(0)) {
        return false;
    }
    buffer_t buf;
    memset(&buf, 0, sizeof(buf));
    buffer_dyn_stub.buffer_ptr = &buf;
    buf.dst_sa.addr_type = ADDR_802_15_4_SHORT;
    protocol_core_stub.entry_ptr = &entry;
    buf.buf_ptr = 10;
    buf.buf_end = 80;
    buf.link_specific.ieee802_15_4.indirectTxProcess = false;
    lowpan_adaptation_interface_tx(&entry, &buf);
    if (!lowpan_adaptation_tx_active(0)) {
        return false;
    }

    lowpan_adaptation_interface_free(0);
    return true;
}
