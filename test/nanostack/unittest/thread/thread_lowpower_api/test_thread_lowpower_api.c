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
#include "test_thread_lowpower_api.h"
#include "thread_lowpower_api.h"
#include <string.h>
#include "ns_types.h"
#include "NWK_INTERFACE/Include/protocol.h"
#include "event_stub.h"
#include "6LoWPAN/Thread/thread_common.h"
#include "protocol_core_stub.h"
#include "mle_service_stub.h"

bool test_thread_lowpower_test_probe_send()
{
    int8_t interface_id = 1;
    uint8_t address[16]; 
    uint8_t *metrics; 
    uint8_t metrics_length;
    int data_response_cb;
    thread_info_t thread_info;
    thread_info.version = 1;
    thread_info.thread_attached_state = THREAD_STATE_CONNECTED;
    thread_info.native_commissioner_port = 10;
    mle_service_msg_buf_def mle_service_msg_buf;
    mle_service_msg_buf.msg_id = 0;

    if (-1 != thread_lowpower_test_probe_send(interface_id, address, metrics, metrics_length, data_response_cb)) {
        return false;
    }
    return true;
}

