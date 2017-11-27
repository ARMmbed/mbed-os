/*
 * Copyright (c) 2016, Arm Limited and affiliates.
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
#include "test_tcp.h"
#include <string.h>
#include "nsconfig.h"
#include "NWK_INTERFACE/Include/protocol.h"
#include "Common_Protocols/tcp.h"

static tcp_session_t tcp_session;

bool test_tcp_session_close()
{
    tcp_session.state = TCP_STATE_CLOSED;
    tcp_session_close(&tcp_session);
    return true;
}

