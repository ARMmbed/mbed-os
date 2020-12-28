/* Socket
 * Copyright (c) 2015 ARM Limited
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

#include "netsocket/UDPSocket.h"
#include "Timer.h"
#include "mbed_assert.h"

UDPSocket::UDPSocket()
{
    _socket_stats.stats_update_proto(this, NSAPI_UDP);
}

nsapi_protocol_t UDPSocket::get_proto()
{
    return NSAPI_UDP;
}
