/*
 * Copyright (c) 2016-2018, Arm Limited and affiliates.
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
#include "nsdynmemLIB.h"
#include "NWK_INTERFACE/Include/protocol.h"
#include "Service_Libs/mle_service/mle_service_api.h"

#include "net_test_api.h"
#include "Common_Protocols/tcp.h"

#include "ns_trace.h"

#define TRACE_GROUP "nTES"

bool validate_tcp_state_name(int state)
{
    switch (state) {
        case TCP_STATE_LISTEN:
        case TCP_STATE_SYN_SENT:
        case TCP_STATE_SYN_RECEIVED:
        case TCP_STATE_ESTABLISHED:
        case TCP_STATE_CLOSE_WAIT:
        case TCP_STATE_LAST_ACK:
        case TCP_STATE_FIN_WAIT_1:
        case TCP_STATE_FIN_WAIT_2:
        case TCP_STATE_CLOSING:
        case TCP_STATE_TIME_WAIT:
            return true;
        default:
            return false;
    }
}

int8_t arm_nwk_test_tcp_drop_tx(int state, uint8_t count)
{
    int8_t retcode = -1;
    if (validate_tcp_state_name(state)) {
        if (count > 200) {
            count = 200;
        }
        retcode = tcp_test_drop_tx(state, count);
    }

    return retcode;
}

int8_t arm_nwk_test_tcp_drop_rx(int state, uint8_t count)
{
    int8_t retcode = -1;
    if (validate_tcp_state_name(state)) {
        if (count > 200) {
            count = 200;
        }
        retcode = tcp_test_drop_rx(state, count);
    }

    return retcode;
}

void arm_nwk_test_tcp_drop_reset()
{
    tcp_test_drop_reset();
}

void arm_nwk_test_mle_receive_filter_set(mle_service_filter_cb *response_filter_cb)
{
    mle_service_receive_filter_cb_set(response_filter_cb);
}
