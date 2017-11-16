/*
 * Copyright (c) 2014-2017, Arm Limited and affiliates.
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

/*
 * \file mac_indirect_data.c
 * \brief Add short description about this file!!!
 *
 */
#include "nsconfig.h"
#include "string.h"
#include "ns_types.h"
#include "ns_trace.h"
#include "eventOS_event.h"
#include "eventOS_scheduler.h"
#include "eventOS_callback_timer.h"
#include "nsdynmemLIB.h"
#include "common_functions.h"
#include "sw_mac.h"
#include "mac_api.h"
#include "MAC/IEEE802_15_4/sw_mac_internal.h"
#include "MAC/IEEE802_15_4/mac_defines.h"
#include "MAC/IEEE802_15_4/mac_indirect_data.h"
#include "MAC/IEEE802_15_4/mac_header_helper_functions.h"
#include "MAC/IEEE802_15_4/mac_mcps_sap.h"
#include "MAC/rf_driver_storage.h"

#define TRACE_GROUP_MAC_INDIR "mInD"
#define TRACE_GROUP "mInD"

void mac_indirect_data_ttl_handle(protocol_interface_rf_mac_setup_s *cur, uint16_t tick_value)
{
    if (!cur || !cur->dev_driver ) {
        return;
    }
    mcps_data_conf_t confirm;
    memset(&confirm, 0, sizeof(mcps_data_conf_t));

    phy_device_driver_s *dev_driver = cur->dev_driver->phy_driver;
    if (!cur->indirect_pd_data_request_queue) {
        uint8_t value = 0;
        if( dev_driver && dev_driver->extension ){
            dev_driver->extension(PHY_EXTENSION_CTRL_PENDING_BIT, &value);
        }
        return;
    }
    mac_pre_build_frame_t *buf, *buf_prev = 0, *buf_temp = 0;

    mac_api_t *api = get_sw_mac_api(cur);
    if( !api ){
        return;
    }

    buf = cur->indirect_pd_data_request_queue;

    tick_value /= 20; //Covert time ms
    if (tick_value == 0) {
        tick_value = 1;
    }

    while (buf) {
        if (buf->buffer_ttl > tick_value) {
            buf->buffer_ttl -= tick_value;
            buf_prev = buf;
            buf = buf->next;
        } else {
            buf->buffer_ttl = 0;
            if (buf_prev) {
                buf_prev->next = buf->next;
            } else {
                cur->indirect_pd_data_request_queue = buf->next;
            }

            confirm.msduHandle = buf->msduHandle;
            buf_temp = buf;
            buf = buf->next;
            buf_temp->next = NULL;
            cur->indirect_pending_bytes -= buf_temp->mac_payload_length;

            confirm.status = MLME_TRANSACTION_EXPIRED;

            mcps_sap_prebuild_frame_buffer_free(buf_temp);
            if( api->data_conf_cb ) {
                api->data_conf_cb(api, &confirm);
            }
        }
    }

    eventOS_callback_timer_stop(cur->mac_mcps_timer);
    eventOS_callback_timer_start(cur->mac_mcps_timer, MAC_INDIRECT_TICK_IN_MS * 20);
}

uint8_t mac_indirect_data_req_handle(mac_pre_parsed_frame_t *buf, protocol_interface_rf_mac_setup_s *mac_ptr)
{

    if (!mac_ptr || !buf) {
        return 1;
    }

    if (buf->fcf_dsn.DstAddrMode == MAC_ADDR_MODE_NONE || buf->fcf_dsn.SrcAddrMode == MAC_ADDR_MODE_NONE) {
        return 1;
    }

    uint8_t srcAddress[8];
    memset(&srcAddress, 0, 8);

    mac_header_get_src_address(&buf->fcf_dsn, mac_header_message_start_pointer(buf), srcAddress);

    //Call COMM status
    mac_api_t *mac_api = get_sw_mac_api(mac_ptr);
    if (mac_api) {
        mlme_comm_status_t comm_status;
        memset(&comm_status, 0, sizeof(mlme_comm_status_t));

        comm_status.status = MLME_DATA_POLL_NOTIFICATION;
        //Call com status
        comm_status.PANId = mac_header_get_dst_panid(&buf->fcf_dsn, mac_header_message_start_pointer(buf));
        comm_status.DstAddrMode = buf->fcf_dsn.DstAddrMode;;
        mac_header_get_dst_address(&buf->fcf_dsn, mac_header_message_start_pointer(buf), comm_status.DstAddr);
        comm_status.SrcAddrMode = buf->fcf_dsn.SrcAddrMode;
        mac_header_get_src_address(&buf->fcf_dsn, mac_header_message_start_pointer(buf), comm_status.SrcAddr);
        mac_header_security_components_read(buf, &comm_status.Key);
        if( mac_api->mlme_ind_cb ){
            mac_api->mlme_ind_cb(mac_api, MLME_COMM_STATUS, &comm_status);
        }
    }


    /* If the Ack we sent for the Data Request didn't have frame pending set, we shouldn't transmit - child may have slept */
    if (!buf->ack_pendinfg_status) {
        //tr_debug("Drop by pending");
        if (mac_ptr->indirect_pd_data_request_queue) {
            tr_error("Wrongly dropped");
        }
        //Free Buffer
        return 1;
    }

    uint8_t address_cmp_ok = 0;
    uint8_t len;
    mac_pre_build_frame_t *b_prev = NULL;

    if (buf->fcf_dsn.SrcAddrMode == MAC_ADDR_MODE_16_BIT) {
        len = 2;
    } else {
        len = 8;
    }

    mac_pre_build_frame_t *b = mac_ptr->indirect_pd_data_request_queue;
    while (b) {

        if (buf->neigh_info) {
            uint16_t compare_short;
            if (b->fcf_dsn.DstAddrMode == MAC_ADDR_MODE_16_BIT) {

                compare_short = common_read_16_bit(b->DstAddr);
                if (compare_short == buf->neigh_info->ShortAddress) {
                    address_cmp_ok = 1;
                }
            } else {
                if (memcmp(b->DstAddr, buf->neigh_info->ExtAddress, 8) == 0) {
                    address_cmp_ok = 1;
                }
            }
        } else {
            if (b->fcf_dsn.DstAddrMode == buf->fcf_dsn.SrcAddrMode) {
                if (memcmp(b->DstAddr, srcAddress, len) == 0) {
                    address_cmp_ok = 1;
                }
            }
        }

        if (address_cmp_ok) {
            if (b_prev) {
                b_prev->next = b->next;

            } else {
                mac_ptr->indirect_pd_data_request_queue = b->next;
            }
            b->next = NULL;
            b->priority = MAC_PD_DATA_MEDIUM_PRIORITY;
            mcps_sap_pd_req_queue_write(mac_ptr, b);
            return 1;
        } else {
            b_prev = b;
            b = b->next;
        }
    }
    return 0;
}

void mac_indirect_queue_write(protocol_interface_rf_mac_setup_s *rf_mac_setup, mac_pre_build_frame_t *buffer)
{
    if( !rf_mac_setup || ! buffer ){
        return;
    }
    rf_mac_setup->indirect_pending_bytes += buffer->mac_payload_length;
    buffer->next = NULL;
    buffer->buffer_ttl = 7100;
    //Push to queue
    if (!rf_mac_setup->indirect_pd_data_request_queue) {
        rf_mac_setup->indirect_pd_data_request_queue = buffer;
        //Trig timer and set pending flag to radio
        eventOS_callback_timer_stop(rf_mac_setup->mac_mcps_timer);
        eventOS_callback_timer_start(rf_mac_setup->mac_mcps_timer, MAC_INDIRECT_TICK_IN_MS * 20);
        if (rf_mac_setup->dev_driver->phy_driver->extension)
        {
            uint8_t value = 1;
            rf_mac_setup->dev_driver->phy_driver->extension(PHY_EXTENSION_CTRL_PENDING_BIT, &value);
        }
        return;
    }

    mac_pre_build_frame_t *cur = rf_mac_setup->indirect_pd_data_request_queue;
    while(cur) {
        if( cur->next == NULL) {
            cur->next = buffer;
            cur = NULL;
        } else {
            cur = cur->next;
        }
    }
}
