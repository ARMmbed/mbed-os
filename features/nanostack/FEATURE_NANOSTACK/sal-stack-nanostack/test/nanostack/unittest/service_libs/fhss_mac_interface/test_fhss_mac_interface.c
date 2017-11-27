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
#include "mac_api.h"
#include "fhss_api.h"
#include "fhss_config.h"
#include "net_fhss.h"
#include "Service_Libs/fhss/fhss.h"
#include "Service_Libs/fhss/fhss_mac_interface.h"
#include "MAC/IEEE802_15_4/mac_fhss_callbacks.h"
#include "test_fhss_mac_interface.h"
#include "nsdynmemLIB_stub.h"
#include "fhss_stub.h"
#include "fhss_channel_stub.h"
#include "fhss_platform_stub.h"

bool test_fhss_is_broadcast_channel_cb()
{
    fhss_api_t fhss_api;

    // By setting bool value false, fhss_struct can not be found
    fhss_stub.bool_value = false;
    if (fhss_is_broadcast_channel_cb(&fhss_api) == true) {
        return false;
    }
    // By setting bool value true, fhss_struct can be found
    fhss_stub.bool_value = true;
    // Should return true
    fhss_channel_stub.channel_bool_value = true;
    if (fhss_is_broadcast_channel_cb(&fhss_api) == false) {
        return false;
    }
    // Should return false
    fhss_channel_stub.channel_bool_value = false;
    if (fhss_is_broadcast_channel_cb(&fhss_api) == true) {
        return false;
    }
    // Should return true when unsynchronized
    fhss_stub.fhss_struct.fhss_state = FHSS_UNSYNCHRONIZED;
    if (fhss_is_broadcast_channel_cb(&fhss_api) == false) {
        return false;
    }
    return true;
}
bool test_fhss_use_broadcast_queue_cb()
{
    fhss_api_t fhss_api;

    // By setting bool value false, fhss_struct can not be found
    fhss_stub.bool_value = false;
    if (fhss_use_broadcast_queue_cb(&fhss_api, true, FHSS_DATA_FRAME) == true) {
        return false;
    }
    // By setting bool value true, fhss_struct can be found
    fhss_stub.bool_value = true;
    // Synchronization request frame should go to unicast queue
    if (fhss_use_broadcast_queue_cb(&fhss_api, true, FHSS_SYNCH_REQUEST_FRAME) == true) {
        return false;
    }
    // Broadcast frame should go to broadcast queue
    if (fhss_use_broadcast_queue_cb(&fhss_api, true, FHSS_DATA_FRAME) == false) {
        return false;
    }
    return true;
}
bool test_fhss_tx_handle_cb()
{
    fhss_api_t fhss_api;
    uint8_t synch_info[FHSS_SYNCH_INFO_LENGTH];

    // By setting bool value false, fhss_struct can not be found
    fhss_stub.bool_value = false;
    if (fhss_tx_handle_cb(&fhss_api, true, NULL, FHSS_DATA_FRAME, NULL, 0, 0, 0) != -2) {
        return false;
    }
    // By setting bool value true, fhss_struct can be found
    fhss_stub.bool_value = true;
    // Broadcast data on unicast channel should return -3
    fhss_channel_stub.channel_bool_value = false;
    if (fhss_tx_handle_cb(&fhss_api, true, NULL, FHSS_DATA_FRAME, NULL, 0, 0, 0) != -3) {
        return false;
    }
    // Synch frame without synch info should return -4
    if (fhss_tx_handle_cb(&fhss_api, true, NULL, FHSS_SYNCH_FRAME, NULL, 0, 0, 0) != -4) {
        return false;
    }
    // Test sending of Beacon
    if (fhss_tx_handle_cb(&fhss_api, true, NULL, FHSS_SYNCH_FRAME, synch_info, 0, 0, 0) != 0) {
        return false;
    }
    // When TX is not allowed, should return -1 for data frame
    fhss_stub.tx_allowed_bool_value = false;
    if (fhss_tx_handle_cb(&fhss_api, false, NULL, FHSS_DATA_FRAME, synch_info, 0, 0, 0) != -1) {
        return false;
    }
    // When TX is allowed, should return 0 for data frame
    fhss_stub.tx_allowed_bool_value = true;
    if (fhss_tx_handle_cb(&fhss_api, false, NULL, FHSS_DATA_FRAME, synch_info, 0, 0, 0) != 0) {
        return false;
    }
    // Test changing to parent channel to send synch request
    fhss_stub.fhss_struct.fhss_state = FHSS_SYNCHRONIZED;
    if (fhss_tx_handle_cb(&fhss_api, false, NULL, FHSS_SYNCH_REQUEST_FRAME, synch_info, 0, 0, 0) != 0) {
        return false;
    }
    return true;
}
bool test_fhss_check_tx_conditions_cb()
{
    fhss_api_t fhss_api;

    // By setting bool value false, fhss_struct can not be found
    fhss_stub.bool_value = false;
    if (fhss_check_tx_conditions_cb(&fhss_api, true, 1, FHSS_DATA_FRAME, 100, 0, 0) == true) {
        return false;
    }
    // By setting bool value true, fhss_struct can be found
    fhss_stub.bool_value = true;

    // Test failing bad channel check
    fhss_stub.bad_channel_bool_value = false;
    fhss_stub.channel_type_bool_value = true;
    fhss_stub.tx_allowed_bool_value = true;
    if (fhss_check_tx_conditions_cb(&fhss_api, true, 1, FHSS_DATA_FRAME, 100, 0, 0) == true) {
        return false;
    }
    // Test failing channel type check
    fhss_stub.bad_channel_bool_value = true;
    fhss_stub.channel_type_bool_value = false;
    fhss_stub.tx_allowed_bool_value = true;
    if (fhss_check_tx_conditions_cb(&fhss_api, true, 1, FHSS_DATA_FRAME, 100, 0, 0) == true) {
        return false;
    }
    // Test failing TX allowed check
    fhss_stub.bad_channel_bool_value = true;
    fhss_stub.channel_type_bool_value = true;
    fhss_stub.tx_allowed_bool_value = false;
    if (fhss_check_tx_conditions_cb(&fhss_api, true, 1, FHSS_DATA_FRAME, 100, 0, 0) == true) {
        return false;
    }
    // Test TX allowed
    fhss_stub.bad_channel_bool_value = true;
    fhss_stub.channel_type_bool_value = true;
    fhss_stub.tx_allowed_bool_value = true;
    if (fhss_check_tx_conditions_cb(&fhss_api, true, 1, FHSS_DATA_FRAME, 100, 0, 0) == false) {
        return false;
    }
    return true;
}
bool test_fhss_receive_frame_cb()
{
    fhss_api_t fhss_api;
    fhss_timer_t platform_functions;
    uint8_t source_address[8] = {0, 1, 2, 3, 4, 5, 6, 7};

    platform_functions.fhss_get_timestamp = fhss_get_timestamp_stub;
    fhss_stub.fhss_struct.platform_functions = platform_functions;
    fhss_api.read_timestamp = fhss_read_timestamp_cb;

    // By setting bool value false, fhss_struct can not be found
    fhss_stub.bool_value = false;
    fhss_receive_frame_cb(&fhss_api, 0, NULL, 0, NULL, FHSS_SYNCH_FRAME);
    // By setting bool value true, fhss_struct can be found
    fhss_stub.bool_value = true;
    // Test adding Beacon info when not synchronized
    fhss_stub.fhss_struct.fhss_state = FHSS_UNSYNCHRONIZED;
    fhss_receive_frame_cb(&fhss_api, 0, NULL, 0, NULL, FHSS_SYNCH_FRAME);
    // Beacon received when synchronized
    fhss_stub.fhss_struct.fhss_state = FHSS_SYNCHRONIZED;
    fhss_receive_frame_cb(&fhss_api, 0, source_address, 0, NULL, FHSS_SYNCH_FRAME);
    // Beacon request received
    fhss_channel_stub.channel_bool_value = false;
    fhss_stub.fhss_struct.fhss_state = FHSS_SYNCHRONIZED;

    fhss_stub.fhss_struct.send_synch_info_on_next_broadcast_channel = false;
    fhss_receive_frame_cb(&fhss_api, 0, NULL, 0, NULL, FHSS_SYNCH_REQUEST_FRAME);
    if (fhss_stub.fhss_struct.send_synch_info_on_next_broadcast_channel != true) {
        return false;
    }
    // Test Synch info limitation
    fhss_stub.fhss_struct.send_synch_info_on_next_broadcast_channel = false;
    fhss_stub.fhss_struct.synch_infos_sent_counter = MAX_SYNCH_INFOS_PER_CHANNEL_LIST;
    fhss_receive_frame_cb(&fhss_api, 0, NULL, 0, NULL, FHSS_SYNCH_REQUEST_FRAME);
    if (fhss_stub.fhss_struct.send_synch_info_on_next_broadcast_channel != false) {
        return false;
    }

    return true;
}
bool test_fhss_data_tx_done_cb()
{
    fhss_api_t fhss_api;

    // By setting bool value false, fhss_struct can not be found
    fhss_stub.bool_value = false;
    fhss_data_tx_done_cb(&fhss_api, true, true, 0);
    // By setting bool value true, fhss_struct can be found
    fhss_stub.bool_value = true;
    fhss_data_tx_done_cb(&fhss_api, false, true, 0);
    // Should find failed TX handle
    fhss_stub.fhss_failed_tx.handle = 10;
    fhss_data_tx_done_cb(&fhss_api, false, true, 10);
    // Verify removed handle (255 means removed in this test)
    if (fhss_stub.fhss_failed_tx.handle != 255) {
        return false;
    }
    return true;
}
bool test_fhss_data_tx_fail_cb()
{
    fhss_api_t fhss_api;
    uint8_t handle = 1;

    // By setting bool value false, fhss_struct can not be found
    fhss_stub.bool_value = false;
    fhss_data_tx_fail_cb(&fhss_api, handle, FHSS_DATA_FRAME);
    // By setting bool value true, fhss_struct can be found
    fhss_stub.bool_value = true;

    // No channel retries should return false
    fhss_stub.fhss_struct.fhss_configuration.fhss_number_of_channel_retries = 0;
    if (fhss_data_tx_fail_cb(&fhss_api, handle, FHSS_DATA_FRAME) != false) {
    	return false;
    }
    // In this test 255 means no handle added
    fhss_stub.fhss_failed_tx.handle = 255;
    fhss_stub.fhss_struct.fhss_configuration.fhss_number_of_channel_retries = 2;
    // This call should add new failed handle
    if (fhss_data_tx_fail_cb(&fhss_api, handle, FHSS_DATA_FRAME) != true) {
    	return false;
    }
    // Verify added handle
    if (fhss_stub.fhss_failed_tx.handle != handle) {
        return false;
    }
    // After first retransmission should return true because one more retry left
    if (fhss_data_tx_fail_cb(&fhss_api, handle, FHSS_DATA_FRAME) != true) {
        return false;
    }
    // After last retransmission should return false
    if (fhss_data_tx_fail_cb(&fhss_api, handle, FHSS_DATA_FRAME) != false) {
        return false;
    }

    return true;
}
bool test_fhss_synch_state_set_cb()
{
	uint16_t panid = 0xacdc;
    fhss_api_t fhss_api;
    fhss_timer_t platform_functions;
    fhss_stub.fhss_struct.callbacks.read_datarate = mac_read_phy_datarate;
    fhss_stub.fhss_struct.callbacks.read_mac_address = mac_read_64bit_mac_address;
    fhss_stub.fhss_struct.synch_panid = 0;
    fhss_stub.fhss_struct.own_hop = 1;
    platform_functions.fhss_get_timestamp = fhss_get_timestamp_stub;
    fhss_stub.fhss_struct.platform_functions = platform_functions;
    fhss_api.read_timestamp = fhss_read_timestamp_cb;

    // By setting bool value false, fhss_struct can not be found
    fhss_stub.bool_value = false;
    fhss_synch_state_set_cb(&fhss_api, FHSS_UNSYNCHRONIZED, 0);
    // By setting bool value true, fhss_struct can be found
    fhss_stub.bool_value = true;
    // Test setting current state again
    fhss_stub.fhss_struct.fhss_state = FHSS_SYNCHRONIZED;
    fhss_synch_state_set_cb(&fhss_api, FHSS_SYNCHRONIZED, 0);
    // Test setting unsynchronized state
    fhss_synch_state_set_cb(&fhss_api, FHSS_UNSYNCHRONIZED, 0);
    // Test setting synchronized state
    fhss_stub.fhss_struct.fhss_state = FHSS_UNSYNCHRONIZED;
    fhss_synch_state_set_cb(&fhss_api, FHSS_SYNCHRONIZED, 0);
    // Test finding beacon info
    fhss_stub.beacon_info.pan_id = panid;
    fhss_synch_state_set_cb(&fhss_api, FHSS_SYNCHRONIZED, panid);
    // Test if device is synch root
    fhss_stub.fhss_struct.own_hop = 0;
    fhss_stub.fhss_struct.synch_panid = 0;
    fhss_stub.beacon_info.pan_id = 1;
    fhss_stub.fhss_struct.fhss_state = FHSS_UNSYNCHRONIZED;
    fhss_synch_state_set_cb(&fhss_api, FHSS_SYNCHRONIZED, panid);

    return true;
}
bool test_fhss_read_timestamp_cb()
{
    fhss_api_t fhss_api;

    // By setting bool value false, fhss_struct can not be found
    fhss_stub.bool_value = false;
    if (fhss_read_timestamp_cb(&fhss_api) != 0) {
        return false;
    }
    // By setting bool value true, fhss_struct can be found
    fhss_stub.bool_value = true;
    return true;
}
bool test_fhss_init_callbacks_cb()
{
    fhss_api_t fhss_api;
    fhss_callback_t callbacks;

    // By setting bool value false, fhss_struct can not be found
    fhss_stub.bool_value = false;
    fhss_init_callbacks_cb(&fhss_api, NULL);
    // By setting bool value true, fhss_struct can be found
    fhss_stub.bool_value = true;
    fhss_init_callbacks_cb(&fhss_api, &callbacks);
    return true;
}
bool test_fhss_get_retry_period_cb()
{
	fhss_api_t fhss_api;
	if (fhss_get_retry_period_cb(NULL, NULL, 1) != 1) {
        return false;
	}
	if (fhss_get_retry_period_cb(NULL, NULL, 128) != 1) {
        return false;
	}
	if (fhss_get_retry_period_cb(NULL, NULL, 256) != 1) {
        return false;
	}
    // By setting bool value true, fhss_struct can be found
    fhss_stub.bool_value = true;

    // With the given datarate and MTU, callback should return 896
    fhss_stub.fhss_struct.datarate = 250000;
    fhss_stub.int8_value = 0;
	if (fhss_get_retry_period_cb(&fhss_api, NULL, 600) != 896) {
        return false;
	}
    return true;
}
