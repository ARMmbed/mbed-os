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
#include "ns_types.h"
#include "fhss_api.h"
#include "fhss_config.h"
#include "Service_Libs/fhss/fhss.h"
#include "Service_Libs/fhss/fhss_channel.h"
#include "Service_Libs/fhss/fhss_beacon.h"
#include "platform/arm_hal_interrupt.h"
#include "randLIB.h"
#include "ns_trace.h"

#define TRACE_GROUP "fhss"


bool fhss_is_broadcast_channel_cb(const fhss_api_t *api)
{
    fhss_structure_t *fhss_structure = fhss_get_object_with_api(api);
    if (!fhss_structure) {
        return false;
    }
    // FHSS is unsynchronized, broadcasts allowed
    if (fhss_structure->fhss_state == FHSS_UNSYNCHRONIZED) {
        return true;
    }
    return fhss_is_current_channel_broadcast(fhss_structure);
}

bool fhss_use_broadcast_queue_cb(const fhss_api_t *api, bool is_broadcast_addr, int frame_type)
{
    fhss_structure_t *fhss_structure = fhss_get_object_with_api(api);
    if (!fhss_structure) {
        return false;
    }
    // Synch requests are always stored in unicast queue
    if (frame_type == FHSS_SYNCH_REQUEST_FRAME) {
        return false;
    }
    // Broadcast packets are stored in broadcast queue
    return is_broadcast_addr;
}

int fhss_tx_handle_cb(const fhss_api_t *api, bool is_broadcast_addr, uint8_t *destination_address, int frame_type, uint8_t *synch_info, uint16_t frame_length, uint8_t phy_header_length, uint8_t phy_tail_length)
{
    fhss_structure_t *fhss_structure = fhss_get_object_with_api(api);
    if (!fhss_structure) {
        return -2;
    }
    // TODO: needs some more logic to push buffer back to queue
    if (frame_type == FHSS_DATA_FRAME) {
        if (is_broadcast_addr == true) {
            if (fhss_is_current_channel_broadcast(fhss_structure) == false) {
                tr_info("Broadcast on UC channel -> Back to queue");
                return -3;
            }
        }
    }
    if (frame_type == FHSS_SYNCH_FRAME) {
        if (!synch_info) {
            return -4;
        }
        fhss_beacon_build(fhss_structure, synch_info);
    } else if (fhss_check_tx_allowed(fhss_structure, is_broadcast_addr, frame_length, frame_type, phy_header_length, phy_tail_length) == false) {
        return -1;
    }
    // If sending Beacon request on parents Unicast channel
    if (frame_type == FHSS_SYNCH_REQUEST_FRAME && fhss_structure->fhss_state == FHSS_SYNCHRONIZED) {
        fhss_change_to_parent_channel(fhss_structure);
    } else if (frame_type == FHSS_DATA_FRAME) {
        fhss_change_to_tx_channel(fhss_structure, destination_address);
    }
    return 0;
}

bool fhss_check_tx_conditions_cb(const fhss_api_t *api, bool is_broadcast_addr, uint8_t handle, int frame_type, uint16_t frame_length, uint8_t phy_header_length, uint8_t phy_tail_length)
{
    fhss_structure_t *fhss_structure = fhss_get_object_with_api(api);
    if (!fhss_structure) {
        return false;
    }
    // This condition will check that message is not sent on bad channel
    if (fhss_check_bad_channel(fhss_structure, handle) == false) {
        return false;
    }

    // This condition will check that broadcast messages are sent only broadcast channels
    if (fhss_check_channel_type(fhss_structure, is_broadcast_addr, frame_type) == false) {
        return false;
    }

    // This condition will check that FHSS is on TX slot and there is enough time to transmit before channel or slot change
    if (fhss_check_tx_allowed(fhss_structure, is_broadcast_addr, frame_length, frame_type, phy_header_length, phy_tail_length) == false) {
        return false;
    }

    return true;
}

void fhss_receive_frame_cb(const fhss_api_t *api, uint16_t pan_id, uint8_t *source_address, uint32_t timestamp, uint8_t *synch_info, int frame_type)
{
    fhss_structure_t *fhss_structure = fhss_get_object_with_api(api);
    if (!fhss_structure) {
        return;
    }
    if (FHSS_SYNCH_FRAME == frame_type) {
        if ((fhss_structure->fhss_state == FHSS_UNSYNCHRONIZED) || fhss_structure->synch_panid != pan_id) {
            fhss_add_beacon_info(fhss_structure, pan_id, source_address, timestamp, synch_info);
        } else {
            if (!fhss_compare_with_synch_parent_address(fhss_structure, source_address)) {
                // Synch parent address needs to be updated in case parent has changed
                fhss_update_synch_parent_address(fhss_structure);
                platform_enter_critical();
                // Calculate time since the Beacon was received
                uint32_t elapsed_time = api->read_timestamp(api) - timestamp;
                // Synchronize to given PAN
                fhss_beacon_received(fhss_structure, synch_info, elapsed_time);
                platform_exit_critical();
            }
        }
    } else if (FHSS_SYNCH_REQUEST_FRAME == frame_type) {
        // If current channel is broadcast, we don't need to send another synch info on next broadcast channel.
        // Only send number of MAX_SYNCH_INFOS_PER_CHANNEL_LIST synch infos per one channel list cycle
        if ((fhss_structure->fhss_state == FHSS_SYNCHRONIZED) && (fhss_is_current_channel_broadcast(fhss_structure) == false)
                && (fhss_structure->synch_infos_sent_counter < MAX_SYNCH_INFOS_PER_CHANNEL_LIST)) {
            fhss_structure->send_synch_info_on_next_broadcast_channel = true;
        }
    }
}

void fhss_data_tx_done_cb(const fhss_api_t *api, bool waiting_ack, bool tx_completed, uint8_t handle)
{
    fhss_structure_t *fhss_structure = fhss_get_object_with_api(api);
    if (!fhss_structure) {
        return;
    }
    if (waiting_ack == false) {
        fhss_change_to_rx_channel(fhss_structure);
    }
    // Buffer was successfully transmitted. Remove stored failure handle if exists.
    if (tx_completed == true) {
        fhss_failed_tx_t *fhss_failed_tx = fhss_failed_handle_find(fhss_structure, handle);
        if (fhss_failed_tx) {
            fhss_failed_handle_remove(fhss_structure, handle);
        }
    }
}

bool fhss_data_tx_fail_cb(const fhss_api_t *api, uint8_t handle, int frame_type)
{
    fhss_structure_t *fhss_structure = fhss_get_object_with_api(api);
    if (!fhss_structure) {
        return false;
    }
    // Only use channel retries when device is synchronized
    if (fhss_structure->fhss_state == FHSS_UNSYNCHRONIZED) {
        return false;
    }
    // Channel retries are disabled -> return
    if (fhss_structure->fhss_configuration.fhss_number_of_channel_retries == 0) {
        return false;
    }
    // Use channel retries only for data frames
    if (FHSS_DATA_FRAME != frame_type) {
        return false;
    }

    fhss_failed_tx_t *fhss_failed_tx = fhss_failed_handle_find(fhss_structure, handle);
    if (fhss_failed_tx) {
        fhss_failed_tx->retries_done++;
        if (fhss_failed_tx->retries_done >= fhss_structure->fhss_configuration.fhss_number_of_channel_retries) {
            // No more retries. Return false to stop retransmitting.
            fhss_failed_handle_remove(fhss_structure, handle);
            return false;
        }
    } else {
        // Create new failure handle and return true to retransmit
        fhss_failed_handle_add(fhss_structure, handle);
    }
    return true;
}

void fhss_synch_state_set_cb(const fhss_api_t *api, fhss_states fhss_state, uint16_t pan_id)
{
    fhss_structure_t *fhss_structure = fhss_get_object_with_api(api);
    if (!fhss_structure) {
        return;
    }

    // State is already set
    if (fhss_structure->fhss_state == fhss_state) {
        tr_debug("Synch same state %u", fhss_state);
        return;
    }

    if (fhss_state == FHSS_UNSYNCHRONIZED) {
        tr_debug("FHSS down");
        fhss_down(fhss_structure);
    } else {
        // Do not synchronize to current pan
        if (fhss_structure->synch_panid == pan_id) {
            tr_debug("Synch same panid %u", pan_id);
            return;
        }
        uint32_t datarate = fhss_structure->callbacks.read_datarate(api);
        fhss_set_datarate(fhss_structure, datarate);
        uint8_t mac_address[8];
        fhss_structure->callbacks.read_mac_address(fhss_structure->fhss_api, mac_address);
        fhss_structure->uc_channel_index = fhss_get_offset(fhss_structure, mac_address);
        // Get Beacon info from storage
        fhss_beacon_info_t *beacon_info = fhss_get_beacon_info(fhss_structure, pan_id);
        if (beacon_info) {
            memcpy(fhss_structure->synch_parent, beacon_info->source_address, 8);
            platform_enter_critical();
            // Calculate time since the Beacon was received
            uint32_t elapsed_time = api->read_timestamp(api) - beacon_info->timestamp;
            // Synchronize to given PAN
            fhss_beacon_received(fhss_structure, beacon_info->synch_info, elapsed_time);
            platform_exit_critical();
            // Delete stored Beacon infos
            fhss_flush_beacon_info_storage(fhss_structure);
            fhss_structure->synch_panid = pan_id;
        } else if (fhss_is_synch_root(fhss_structure) == true) {
            // Synch root will start new network
            fhss_start_timer(fhss_structure, fhss_structure->synch_configuration.fhss_superframe_length, fhss_superframe_handler);
        } else {
            tr_error("Synch info not find");
        }
    }
    fhss_structure->fhss_state = fhss_state;
}

uint32_t fhss_read_timestamp_cb(const fhss_api_t *api)
{
    fhss_structure_t *fhss_structure = fhss_get_object_with_api(api);
    if (!fhss_structure) {
        return 0;
    }
    return (fhss_structure->platform_functions.fhss_get_timestamp(api) * fhss_structure->platform_functions.fhss_resolution_divider);
}

uint16_t fhss_get_retry_period_cb(const fhss_api_t *api, uint8_t *destination_address, uint16_t phy_mtu)
{
    uint16_t retry_period = 0;
    uint16_t random_number = randLIB_get_16bit();
    uint16_t rnd_mask;

    /* Generate retry back-off period. FHSS is using the known synchronization parent info to delay retransmissions upstream.
     *
     */
    if (phy_mtu < 128) {
        // Max. random when PHY MTU below 128 is 6.4ms
        rnd_mask = 0x7f;
    } else if (phy_mtu < 256) {
        // Max. random when PHY MTU below 256 is 12.8ms
        rnd_mask = 0xff;
    } else {
        // Max. random when PHY MTU above 255 is 25.6ms
        rnd_mask = 0x1ff;
    }

    fhss_structure_t *fhss_structure = fhss_get_object_with_api(api);
    if (fhss_structure) {
        uint32_t datarate = fhss_structure->datarate;
        uint16_t max_tx_length;

        if (datarate && phy_mtu) {
            if (fhss_compare_with_synch_parent_address(fhss_structure, destination_address) == 0) {
                // E.g. (1000000 / (250000bit/s / 8 bits)) * 255 bytes = 8160us
                max_tx_length = ((1000000 / (datarate / 8)) * phy_mtu);
                /* Retrying upstream: delay the transmission until assumed hidden node has retried downstream:
                 * Static period: max random + max tx length
                 * 50 comes from MAC timer resolution (50us)
                 */
                retry_period = (rnd_mask + (max_tx_length / 50));
            }
        }
    }

    // Add 1 to not to ever return zero value.
    retry_period += ((random_number & rnd_mask) + 1);
    return retry_period;
}

int fhss_init_callbacks_cb(const fhss_api_t *api, fhss_callback_t *callbacks)
{
    fhss_structure_t *fhss_structure = fhss_get_object_with_api(api);
    if (!fhss_structure || !callbacks) {
        return -1;
    }
    fhss_structure->callbacks = *callbacks;
    return 0;
}
