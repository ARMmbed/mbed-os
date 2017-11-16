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
#include "string.h"
#include "ns_types.h"
#include "ns_trace.h"
#include "eventOS_event.h"
#include "eventOS_scheduler.h"
#include "eventOS_callback_timer.h"
#include "nsdynmemLIB.h"
#include "NWK_INTERFACE/Include/protocol.h"
#include "MAC/IEEE802_15_4/mac_indirect_data.h"
#include "6LoWPAN/Thread/thread_common.h"
#ifndef NO_MLE
#include "MLE/mle.h"
#endif
#include "NWK_INTERFACE/Include/protocol_stats.h"
#include "common_functions.h"
#include "MAC/IEEE802_15_4/mac_defines.h"
#include "MAC/rf_driver_storage.h"
#include "MAC/IEEE802_15_4/mac_mcps_sap.h"

#include "mac_indirect_data_stub.h"

indirect_data_stub_def_t indirect_data_stub_def;

void mac_indirect_data_ttl_handle(protocol_interface_rf_mac_setup_s *cur, uint16_t tick_value)
{
}

uint8_t mac_indirect_data_req_handle(mac_pre_parsed_frame_t *buf, protocol_interface_rf_mac_setup_s *mac_ptr)
{
    return indirect_data_stub_def.value_uint8;
}

void mac_indirect_queue_write(protocol_interface_rf_mac_setup_s *rf_mac_setup, mac_pre_build_frame_t *buffer)
{
    indirect_data_stub_def.indirect_buf = (void*)buffer;
    buffer->next = rf_mac_setup->indirect_pd_data_request_queue;
    rf_mac_setup->indirect_pd_data_request_queue = buffer;
}
