/*
 * Copyright (c) 2013-2019, Arm Limited and affiliates.
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
#include "MLE/mle.h"


#include "ns_types.h"
#include "eventOS_event.h"
#include "eventOS_event_timer.h"
#include "socket_api.h"
#include "Core/include/ns_socket.h"
#include "nsdynmemLIB.h"
#include "ns_trace.h"
#include "string.h"
#include "NWK_INTERFACE/Include/protocol.h"
#include "Common_Protocols/udp.h"
#include "6LoWPAN/Thread/thread_common.h"
#include "6LoWPAN/Thread/thread_bootstrap.h"
#include "6LoWPAN/Bootstraps/protocol_6lowpan.h"
#include "6LoWPAN/Bootstraps/protocol_6lowpan_bootstrap.h"
#include "platform/arm_hal_interrupt.h"
#include "platform/topo_trace.h"
#include "common_functions.h"
#include "MLE/mle_tlv.h"
#include "NWK_INTERFACE/Include/protocol_timer.h"
#include "Common_Protocols/ipv6_constants.h"
#include "Service_Libs/mle_service/mle_service_api.h"
#include "Service_Libs/etx/etx.h"
#include "6LoWPAN/MAC/mac_helper.h"
#include "mac_api.h"
#include "6LoWPAN/MAC/mac_data_poll.h"
#include "6LoWPAN/lowpan_adaptation_interface.h"

#define TRACE_GROUP "mle"



int16_t mle_class_free_entry_count_get(struct protocol_interface_info_entry *cur)
{
    mac_neighbor_table_list_t *mac_table_free_list = &mac_neighbor_info(cur)->free_list;
    return ns_list_count(mac_table_free_list);

}

int16_t mle_class_sleepy_entry_count_get(struct protocol_interface_info_entry *cur)
{
    mac_neighbor_table_list_t *mac_table_list = &mac_neighbor_info(cur)->neighbour_list;

    uint16_t count = 0;

    ns_list_foreach(mac_neighbor_table_entry_t, entry, mac_table_list) {
        if (!entry->rx_on_idle) {
            count++;
        }
    }

    return count;
}

int16_t mle_class_rfd_entry_count_get(struct protocol_interface_info_entry *cur)
{
    mac_neighbor_table_list_t *mac_table_list = &mac_neighbor_info(cur)->neighbour_list;

    uint16_t count = 0;

    ns_list_foreach(mac_neighbor_table_entry_t, entry, mac_table_list) {
        if (!entry->ffd_device) {
            count++;
        }
    }

    return count;
}

uint16_t mle_class_active_neigh_counter(protocol_interface_info_entry_t *cur)
{
    mac_neighbor_table_list_t *mac_table_list = &mac_neighbor_info(cur)->neighbour_list;

    return ns_list_count(mac_table_list);
}

#ifndef NO_MLE

int8_t mle_class_set_new_key_pending(struct protocol_interface_info_entry *cur)
{
    mac_neighbor_table_list_t *mac_table_list = &mac_neighbor_info(cur)->neighbour_list;

    ns_list_foreach_safe(mac_neighbor_table_entry_t, cur_entry, mac_table_list) {
        mle_service_frame_counter_entry_new_key_pending_set(cur->id, cur_entry->index);
    }

    return 0;
}

uint8_t *mle_general_write_source_address(uint8_t *ptr, protocol_interface_info_entry_t *cur)
{
    if (cur->global_address_available) {
        uint16_t mac16 = mac_helper_mac16_address_get(cur);
        if (mac16 < 0xfffe) {
            ptr = mle_tlv_write_source_address(ptr, mac16);
        }
    }
    return ptr;
}

uint8_t *mle_general_write_link_layer_framecounter(uint8_t *ptr, protocol_interface_info_entry_t *cur)
{
    uint32_t temp_counter;
    mac_helper_link_frame_counter_read(cur->id, &temp_counter);
    return mle_tlv_write_link_layer_framecount(ptr, temp_counter);
}

bool mle_neigh_entry_frame_counter_update(struct mac_neighbor_table_entry *entry_temp, uint8_t *tlv_ptr, uint16_t tlv_length, protocol_interface_info_entry_t *cur, uint8_t key_id)
{
    mle_tlv_info_t mle_tlv_info;
    uint32_t frame_counter;
    if (mle_tlv_option_discover(tlv_ptr, tlv_length, MLE_TYPE_LL_FRAME_COUNTER, &mle_tlv_info)  != 4) {
        if (cur->mac_parameters->SecurityEnabled) {
            return false;
        } else {
            frame_counter = 0;
        }
    } else {
        frame_counter = common_read_32_bit(mle_tlv_info.dataPtr);
    }
    mlme_device_descriptor_t device_desc;
    mac_helper_device_description_write(cur, &device_desc, entry_temp->mac64, entry_temp->mac16, frame_counter, false);
    mac_helper_devicetable_set(&device_desc, cur, entry_temp->index, key_id, false);
    return true;
}


void mle_mode_parse_to_mac_entry(mac_neighbor_table_entry_t *mac_entry, uint8_t mode)
{
    mac_entry->rx_on_idle = mode & MLE_RX_ON_IDLE;
    mac_entry->ffd_device = mode & MLE_FFD_DEV;
}

uint8_t  mle_mode_write_from_mac_entry(mac_neighbor_table_entry_t *mac_entry)
{
    uint8_t mode = 0;
    if (mac_entry->rx_on_idle) {
        mode |= MLE_RX_ON_IDLE;
    }

    if (mac_entry->ffd_device) {
        mode |= MLE_FFD_DEV;
    }
    return mode;
}


#endif /* NO_MLE */
