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
 * \file mac_mcps_sap.c
 * \brief Add short description about this file!!!
 *
 */
#include "nsconfig.h"
#include "ns_types.h"
#include "eventOS_event.h"
#include "eventOS_scheduler.h"
#include "eventOS_callback_timer.h"
#include "string.h"
#include "ns_trace.h"
#include "nsdynmemLIB.h"
#include "randLIB.h"
#include "ccmLIB.h"
#include "mlme.h"
#include "mac_api.h"
#include "MAC/IEEE802_15_4/mac_defines.h"
#include "MAC/rf_driver_storage.h"
#include "MAC/IEEE802_15_4/mac_header_helper_functions.h"
#include "MAC/IEEE802_15_4/mac_timer.h"
#include "MAC/IEEE802_15_4/mac_security_mib.h"
#include "6LoWPAN/MAC/mac_data_poll.h"
#include "MAC/IEEE802_15_4/mac_mlme.h"
#include "MAC/IEEE802_15_4/mac_filter.h"
#include "MAC/IEEE802_15_4/mac_pd_sap.h"
#include "MAC/IEEE802_15_4/mac_mcps_sap.h"
#include "MAC/IEEE802_15_4/mac_indirect_data.h"
#include "NWK_INTERFACE/Include/protocol_stats.h"
#include "platform/arm_hal_interrupt.h"
#include "common_functions.h"
#include "sw_mac.h"

#include "mac_mcps_sap_stub.h"

mac_mcps_sap_stub_def mac_mcps_sap_stub;

void mcps_sap_data_req_handler(protocol_interface_rf_mac_setup_s *rf_mac_setup , const mcps_data_req_t *data_req )
{
}

void mac_mcps_trig_buffer_from_queue(protocol_interface_rf_mac_setup_s *rf_mac_setup)
{
}

int8_t mac_mcps_sap_tasklet_init(void)
{
    return mac_mcps_sap_stub.int8_value;
}

mac_pre_build_frame_t * mcps_sap_prebuild_frame_buffer_get(uint16_t payload_size)
{
    return mac_mcps_sap_stub.frame_ptr;
}

void mcps_sap_prebuild_frame_buffer_free(mac_pre_build_frame_t *buffer)
{
}

void mac_data_wait_timer_start(protocol_interface_rf_mac_setup_s *rf_mac_setup)
{
}

void mcps_sap_pd_req_queue_write(protocol_interface_rf_mac_setup_s *rf_mac_setup, mac_pre_build_frame_t *buffer)
{

}

void mcps_sap_pd_indirect_queue_write(protocol_interface_rf_mac_setup_s *rf_mac_setup, mac_pre_build_frame_t *buffer)
{
}

mac_pre_build_frame_t * mcps_sap_pd_indirect_queue_poll(protocol_interface_rf_mac_setup_s *rf_mac_setup, uint8_t address_type, uint8_t *address_ptr)
{
    return mac_mcps_sap_stub.frame_ptr;
}

void mcps_sap_pre_parsed_frame_buffer_free(mac_pre_parsed_frame_t *buf)
{
}

mac_pre_parsed_frame_t * mcps_sap_pre_parsed_frame_buffer_get(const uint8_t *data_ptr, uint16_t frame_length)
{
    if (mac_mcps_sap_stub.pre_parsed) {
        memset(mac_mcps_sap_stub.pre_parsed, 0, sizeof(mac_pre_parsed_frame_t) + frame_length);
        mac_mcps_sap_stub.pre_parsed->frameLength = frame_length;
        memcpy(mac_header_message_start_pointer(mac_mcps_sap_stub.pre_parsed), data_ptr, frame_length);
    }
    return mac_mcps_sap_stub.pre_parsed;
}

int8_t mcps_sap_pd_ind(mac_pre_parsed_frame_t *buffer)
{
    return mac_mcps_sap_stub.int8_value;
}

void mcps_sap_pd_confirm(void *mac_ptr)
{
}


void mac_generic_event_trig(uint8_t event_type, void *mac_ptr, bool low_latency)
{
}

void mac_mcps_buffer_queue_free(protocol_interface_rf_mac_setup_s *rf_mac_setup)
{
}

int8_t mac_virtual_sap_data_cb(void *identifier, arm_phy_sap_msg_t *message)
{
    return mac_mcps_sap_stub.int8_value;
}

void mcps_sap_trig_tx(void *mac_ptr)
{
}

void mcps_sap_purge_reg_handler(protocol_interface_rf_mac_setup_s *rf_mac_setup, const mcps_purge_t *data)
{
}

int mac_convert_frame_type_to_fhss(uint8_t frame_type)
{
    return 0;
}

bool mac_is_ack_request_set(mac_pre_build_frame_t *buffer)
{
    return true;
}
