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
#include "ns_types.h"
#include "eventOS_event.h"
#include "string.h"
#include "ns_trace.h"
#include "ns_list.h"
#include "randLIB.h"
#include "nsdynmemLIB.h"
#include "Core/include/address.h"
#include "Core/include/socket.h"
#include "randLIB.h"
#include "mac_api.h"
#include "mac_mcps.h"
#include "mac_common_defines.h"
#include "common_functions.h"
#include "NWK_INTERFACE/Include/protocol.h"
#include "NWK_INTERFACE/Include/protocol_stats.h"
#include "6LoWPAN/IPHC_Decode/cipv6.h"
#include "NWK_INTERFACE/Include/protocol_timer.h"
#include "Service_Libs/etx/etx.h"
#include "6LoWPAN/MAC/mac_helper.h"
#include "6LoWPAN/Mesh/mesh.h"
#include "6LoWPAN/IPHC_Decode/iphc_decompress.h"
#include "6LoWPAN/lowpan_adaptation_interface.h"
#include "MLE/mle.h"

#include "adaptation_interface_stub.h"

adaptation_interface_stub_def adaptation_interface_stub;

int8_t lowpan_adaptation_interface_init(int8_t interface_id, uint16_t mac_mtu_size)
{
    return adaptation_interface_stub.expectedInt8;
}

int8_t lowpan_adaptation_interface_free(int8_t interface_id)
{
    return adaptation_interface_stub.expectedInt8;
}

int8_t lowpan_adaptation_interface_reset(int8_t interface_id)
{
    return adaptation_interface_stub.expectedInt8;
}

/**
 * \brief call this before normatl TX. This function prepare buffer link spesific metadata and verify packet destination
 */
struct buffer * lowpan_adaptation_data_process_tx_preprocess(struct protocol_interface_info_entry *cur, struct buffer *buf)
{
    return NULL;
}

int8_t lowpan_adaptation_interface_tx(struct protocol_interface_info_entry *cur, struct buffer *buf)
{
    return adaptation_interface_stub.expectedInt8;
}

int8_t lowpan_adaptation_interface_tx_confirm(struct protocol_interface_info_entry *cur, const struct mcps_data_conf_s *confirm)
{
    return adaptation_interface_stub.expectedInt8;
}

bool lowpan_adaptation_tx_active(int8_t interface_id)
{
    return false;
}

int8_t lowpan_adaptation_indirect_free_messages_from_queues_by_address(struct protocol_interface_info_entry *cur, uint8_t *address_ptr, addrtype_t adr_type)
{
    return 0;
}
