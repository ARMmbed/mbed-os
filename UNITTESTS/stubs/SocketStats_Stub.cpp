/* mbed Microcontroller Library
 * Copyright (c) 2018 ARM Limited
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

#include "SocketStats.h"

#if MBED_CONF_NSAPI_SOCKET_STATS_ENABLED
int SocketStats::get_entry_position(const Socket *const reference_id)
{
    return 0;
}
#endif

size_t SocketStats::mbed_stats_socket_get_each(mbed_stats_socket_t *stats, size_t count)
{
    return 0;
}

SocketStats::SocketStats()
{
}

void SocketStats::stats_new_socket_entry(const Socket *const reference_id)
{
    return;
}

void SocketStats::stats_update_socket_state(const Socket *const reference_id, socket_state state)
{
    return;
}

void SocketStats::stats_update_peer(const Socket *const reference_id, const SocketAddress &peer)
{
    return;
}

void SocketStats::stats_update_proto(const Socket *const reference_id, nsapi_protocol_t proto)
{
    return;
}

void SocketStats::stats_update_sent_bytes(const Socket *const reference_id, size_t sent_bytes)
{
    return;
}

void SocketStats::stats_update_recv_bytes(const Socket *const reference_id, size_t recv_bytes)
{
    return;
}
