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
#include "mlme.h"
#include "common_functions.h"
#include "ns_trace.h"
#include "NWK_INTERFACE/Include/protocol_abstract.h"
#include "NWK_INTERFACE/Include/protocol_timer.h"
#include "Service_Libs/mle_service/mle_service_api.h"
#include "Core/include/address.h"
#include "Core/include/socket.h"
#include "6LoWPAN/Thread/thread_common.h"
#include "6LoWPAN/Thread/thread_management_internal.h"
#include "6LoWPAN/MAC/mac_helper.h"
#include "MLE/mle.h"
#include "mac_mcps.h"
#include "6LoWPAN/MAC/mac_data_poll.h"
#include "6LoWPAN/MAC/mac_response_handler.h"
#include "6LoWPAN/lowpan_adaptation_interface.h"

static bool mac_data_is_broadcast_addr(const sockaddr_t *addr);

#define TRACE_GROUP "MRsH"


static void mac_mlme_device_table_confirmation_handle(protocol_interface_info_entry_t *info_entry, mlme_get_conf_t *confirmation) {
    if (confirmation->value_size != sizeof(mlme_device_descriptor_t)) {
        return;
    }

    mlme_device_descriptor_t *descpription = (mlme_device_descriptor_t*)confirmation->value_pointer;

    tr_debug("Dev stable get confirmation %x", confirmation->status);

    if (confirmation->status == MLME_SUCCESS) {
        //GET ME table by extended mac64 address
        mle_neigh_table_entry_t * entry = mle_class_get_by_link_address(info_entry->id, descpription->ExtAddress, ADDR_802_15_4_LONG);
        if (!entry) {
            return;
        }

        if (entry->short_adr != descpription->ShortAddress) {
            //Refresh Short ADDRESS
            mlme_set_t set_request;
            descpription->ShortAddress = entry->short_adr;

            //CALL MLME-SET
            set_request.attr = macDeviceTable;
            set_request.attr_index = confirmation->attr_index;
            set_request.value_pointer = descpription;
            set_request.value_size = confirmation->value_size;
            info_entry->mac_api->mlme_req(info_entry->mac_api, MLME_SET, &set_request);
        }

    }
}

static void mac_mlme_frame_counter_confirmation_handle(protocol_interface_info_entry_t *info_entry, mlme_get_conf_t *confirmation) {
    if (confirmation->value_size != 4) {
        return;
    }
    uint32_t *temp_ptr = (uint32_t*)confirmation->value_pointer;
    info_entry->mac_parameters->security_frame_counter = *temp_ptr;
}

static void mac_mlme_get_confirmation_handler(protocol_interface_info_entry_t *info_entry, mlme_get_conf_t *confirmation) {

    if (!confirmation) {
        return;
    }
    switch (confirmation->attr) {
        case macDeviceTable:
            mac_mlme_device_table_confirmation_handle(info_entry, confirmation);
            break;

        case macFrameCounter:
            mac_mlme_frame_counter_confirmation_handle(info_entry, confirmation);
            break;

        default:

            break;

    }
}

void mcps_data_confirm_handler( const mac_api_t* api, const mcps_data_conf_t *data )
{
    protocol_interface_info_entry_t *info_entry = protocol_stack_interface_info_get_by_id(api->parent_id);
    //TODO: create buffer_t and call correct function
    //Update protocol_status
    lowpan_adaptation_interface_tx_confirm(info_entry, data);
}

static bool mcps_data_indication_neighbor_validate(int8_t interface_id, const sockaddr_t *addr)
{
    /* If MLE is enabled, we will talk if we have an MLE association */
    mle_neigh_table_entry_t *mle_entry = mle_class_get_by_link_address(interface_id, addr->address + 2, addr->addr_type);
    if (mle_entry && (mle_entry->handshakeReady ||  mle_entry->thread_commission)) {
        return true;
    }

    /* Otherwise, we don't know them */
    return false;

}

void mcps_data_indication_handler( const mac_api_t* api, const mcps_data_ind_t *data_ind )
{
    protocol_interface_info_entry_t *info_entry = protocol_stack_interface_info_get_by_id(api->parent_id);
    buffer_t *buf = buffer_get(data_ind->msduLength);
    if (!buf || !info_entry) {
        return;
    }
    uint8_t *ptr;
    buffer_data_add(buf, data_ind->msdu_ptr, data_ind->msduLength);
    //tr_debug("MAC Paylod size %u %s",data_ind->msduLength, trace_array(data_ind->msdu_ptr, 8));
    buf->options.lqi = data_ind->mpduLinkQuality;
    buf->options.dbm = data_ind->signal_dbm;
    buf->src_sa.addr_type = (addrtype_t)data_ind->SrcAddrMode;
    ptr = common_write_16_bit(data_ind->SrcPANId, buf->src_sa.address);
    memcpy(ptr, data_ind->SrcAddr, 8);
    buf->dst_sa.addr_type = (addrtype_t)data_ind->DstAddrMode;
    ptr = common_write_16_bit(data_ind->DstPANId, buf->dst_sa.address);
    memcpy(ptr, data_ind->DstAddr, 8);
    //Set Link spesific stuff to seperately
    buf->link_specific.ieee802_15_4.srcPanId = data_ind->SrcPANId;
    buf->link_specific.ieee802_15_4.dstPanId = data_ind->DstPANId;

    if (mac_data_is_broadcast_addr(&buf->dst_sa)) {
        buf->options.ll_broadcast_rx = true;
    }
    buf->interface = info_entry;
    if (data_ind->Key.SecurityLevel) {
        buf->link_specific.ieee802_15_4.fc_security = true;

        if (info_entry->mac_security_key_usage_update_cb) {
            info_entry->mac_security_key_usage_update_cb(info_entry, &data_ind->Key);
        }
    } else {
        buf->link_specific.ieee802_15_4.fc_security = false;
        if (mac_helper_default_security_level_get(info_entry) ||
                !mcps_data_indication_neighbor_validate(info_entry->id, &buf->src_sa)) {
            //SET By Pass
            buf->options.ll_security_bypass_rx = true;
        }
    }

    buf->info = (buffer_info_t)(B_TO_IPV6_TXRX | B_FROM_MAC | B_DIR_UP);
    protocol_push(buf);
}

void mcps_purge_confirm_handler( const mac_api_t* api, mcps_purge_conf_t *data )
{
    (void)api;
    tr_info("MCPS Data Purge confirm status %u, for handle %u", data->status, data->msduHandle);
}

static void stop_bootstrap_timer(protocol_interface_info_entry_t *info_entry)
{
    if (info_entry->bootsrap_mode == ARM_NWK_BOOTSRAP_MODE_6LoWPAN_BORDER_ROUTER) {
        protocol_timer_stop(PROTOCOL_TIMER_BOOTSTRAP_TIM);
    }
}

void mlme_confirm_handler( const mac_api_t* api, mlme_primitive id, const void *data )
{
    protocol_interface_info_entry_t *info_entry = protocol_stack_interface_info_get_by_id(api->parent_id);
    if (!info_entry) {
        return;
    }
    //TODO: create buffer_t and call correct function
    switch(id){
        case MLME_ASSOCIATE:{
            //Unsupported
            break;
        }
        case MLME_DISASSOCIATE:{
            //Unsupported
            break;
        }
        case MLME_GET:{
            mlme_get_conf_t *dat = (mlme_get_conf_t*)data;
            mac_mlme_get_confirmation_handler(info_entry, dat);
            break;
        }
        case MLME_GTS:{
            //Unsupported
            break;
        }
        case MLME_RESET:{
//            mlme_reset_conf_t *dat = (mlme_reset_conf_t*)data;
            break;
        }
        case MLME_RX_ENABLE:{
            //Unsupported
            break;
        }
        case MLME_SCAN:{
            const mlme_scan_conf_t *dat = (mlme_scan_conf_t*)data;
            stop_bootstrap_timer(info_entry);
            info_entry->scan_cb(api->parent_id, dat);
            break;
        }
        case MLME_SET:{
//            mlme_set_conf_t *dat = (mlme_set_conf_t*)data;
            break;
        }
        case MLME_START:{
//            mlme_start_conf_t *dat = (mlme_start_conf_t*)data;
            stop_bootstrap_timer(info_entry);
            break;
        }
        case MLME_POLL:{
            const mlme_poll_conf_t *dat = (mlme_poll_conf_t*)data;
            mac_mlme_poll_confirm(info_entry, dat);
            break;
        }
        case MLME_BEACON_NOTIFY:
        case MLME_ORPHAN:
        case MLME_COMM_STATUS:
        case MLME_SYNC:
        case MLME_SYNC_LOSS:
        default:{
            tr_error("Invalid state in mlme_confirm_handler(): %d", id);
            break;
        }
    }
}

void mlme_indication_handler( const mac_api_t* api, mlme_primitive id, const void *data )
{
    switch(id){
        case MLME_ASSOCIATE:{
            //Unsupported
            //mlme_associate_ind_t *dat = (mlme_associate_ind_t*)data;
            break;
        }
        case MLME_DISASSOCIATE:{
            //Unsupported
            //mlme_disassociate_ind_t *dat = (mlme_disassociate_ind_t*)data;
            break;
        }
        case MLME_BEACON_NOTIFY:{
            const mlme_beacon_ind_t *dat = (mlme_beacon_ind_t*)data;
            protocol_interface_info_entry_t *info_entry = protocol_stack_interface_info_get_by_id(api->parent_id);
            if( info_entry && info_entry->beacon_cb ){
                info_entry->beacon_cb(api->parent_id, dat);
            }
            break;
        }
        case MLME_GTS:{
            //Unsupported
            break;
        }
        case MLME_ORPHAN:{
            //Unsupported
            break;
        }
        case MLME_COMM_STATUS:{
            mlme_comm_status_t *dat = (mlme_comm_status_t*)data;
            protocol_interface_info_entry_t *info_entry = protocol_stack_interface_info_get_by_id(api->parent_id);
            if( info_entry && info_entry->comm_status_ind_cb ){
                info_entry->comm_status_ind_cb(api->parent_id, dat);
            }

            break;
        }
        case MLME_SYNC_LOSS:{
            mlme_sync_loss_t *dat = (mlme_sync_loss_t*)data;
            protocol_interface_info_entry_t *info_entry = protocol_stack_interface_info_get_by_id(api->parent_id);
            if (info_entry) {
                if (dat->LossReason == BEACON_LOST) {
                    nwk_bootsrap_state_update(ARM_NWK_NWK_SCAN_FAIL, info_entry);
                }
            }
            break;
        }
        case MLME_GET:
        case MLME_RESET:
        case MLME_RX_ENABLE:
        case MLME_SCAN:
        case MLME_SET:
        case MLME_START:
        case MLME_SYNC:
        case MLME_POLL:
        default:{
            tr_error("Invalid state in mlme_indication_handler(): %d", id);
            break;
        }
    }
}

bool mac_data_is_broadcast_addr(const sockaddr_t *addr)
{
    return (addr->addr_type == ADDR_802_15_4_SHORT) &&
           (addr->address[2] == 0xFF && addr->address[3] == 0xFF);
}

