/*
 * Copyright (c) 2015, 2017, Arm Limited and affiliates.
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
#include "ns_list.h"
#include "ns_trace.h"
#include "common_functions.h"
#include "nsdynmemLIB.h"
#include "randLIB.h"
#include <string.h>
#include "Core/include/ns_buffer.h"
#include "NWK_INTERFACE/Include/protocol.h"
#include "NWK_INTERFACE/Include/protocol_timer.h"
#include "Common_Protocols/ipv6.h"
#include "Common_Protocols/icmpv6.h"
#include "Service_Libs/Trickle/trickle.h"
#include "6LoWPAN/MAC/mac_helper.h"
#include "6LoWPAN/Thread/thread_common.h"
#include "MPL/mpl.h"

#define MPL_TICK_MS 50

#define MS_TO_TICKS(t) (((t) + MPL_TICK_MS - 1) / MPL_TICK_MS)

/* Timing is in 50 ms (1/20 s) ticks */
const trickle_params_t rfc7731_default_data_message_trickle_params = {
    .Imin = MS_TO_TICKS(512),   /* RFC 7731 says 10 * expected link latency; ZigBee IP says 512 ms */
    .Imax = MS_TO_TICKS(512),   /* RFC 7731 says equal to Imin; ZigBee IP says 512 ms */
    .k = 1,                     /* RFC 7731 says 1; ZigBee IP says infinite */
    .TimerExpirations = 3       /* RFC 7731 says 3; ZigBee IP says 2 for routers, 0 for hosts */
};

const trickle_params_t rfc7731_default_control_message_trickle_params = {
    .Imin = MS_TO_TICKS(512),       /* RFC 7731 says 10 * worst-case link latency */
    .Imax = MS_TO_TICKS(300000),    /* 5 minutes, as per RFC 7731 */
    .k = 1,
    .TimerExpirations = 10
};

mpl_domain_t *mpl_domain_create(protocol_interface_info_entry_t *cur, const uint8_t address[16],
                                const uint8_t *seed_id, multicast_mpl_seed_id_mode_e seed_id_mode,
                                int_fast8_t proactive_forwarding,
                                uint16_t seed_set_entry_lifetime,
                                const trickle_params_t *data_trickle_params,
                                const trickle_params_t *control_trickle_params)
{
    return NULL;
}

buffer_t *mpl_control_handler(buffer_t *buf, protocol_interface_info_entry_t *cur)
{
    return NULL;
}

void mpl_slow_timer(uint16_t seconds)
{
}
