/* mbed Microcontroller Library
 * Copyright (c) 2018 ARM Limited
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
#include "platform/mbed_error.h"
#include "platform/mbed_assert.h"
#ifdef MBED_CONF_RTOS_PRESENT
#include "rtos/Kernel.h"
#endif

#include <string.h>
#include <stdlib.h>

#if MBED_CONF_NSAPI_SOCKET_STATS_ENABLED
SingletonPtr<PlatformMutex> SocketStats::_mutex;
mbed_stats_socket_t SocketStats::_stats[MBED_CONF_NSAPI_SOCKET_STATS_MAX_COUNT];
uint32_t SocketStats::_size = 0;

int SocketStats::get_entry_position(const Socket *const reference_id)
{
    for (uint32_t j = 0; j < _size; j++) {
        if (_stats[j].reference_id == reference_id) {
            return j;
        }
    }
    return -1;
}
#endif

size_t SocketStats::mbed_stats_socket_get_each(mbed_stats_socket_t *stats, size_t count)
{
    MBED_ASSERT(stats != NULL);
    size_t i = 0;
#if MBED_CONF_NSAPI_SOCKET_STATS_ENABLED
    memset(stats, 0, count * sizeof(mbed_stats_socket_t));
    _mutex->lock();
    for (uint32_t j = 0; j < count; j++) {
        if (_stats[j].reference_id) {
            memcpy(&stats[i], &_stats[j], sizeof(mbed_stats_socket_t));
            i++;
        }
    }
    _mutex->unlock();
#endif
    return i;
}

SocketStats::SocketStats()
{
}

void SocketStats::stats_new_socket_entry(const Socket *const reference_id)
{
#if MBED_CONF_NSAPI_SOCKET_STATS_ENABLED
    _mutex->lock();
    if (get_entry_position(reference_id) >= 0) {
        // Duplicate entry
        MBED_WARNING1(MBED_MAKE_ERROR(MBED_MODULE_NETWORK_STATS, MBED_ERROR_CODE_INVALID_INDEX), "Duplicate socket Reference ID ", reference_id);
    } else if (_size < MBED_CONF_NSAPI_SOCKET_STATS_MAX_COUNT) {
        // Add new entry
        _stats[_size].reference_id = (Socket *)reference_id;
        _size++;
    } else {
        int position = -1;
        uint64_t oldest_time = 0;
        // Determine which entry in the list shall be over-written
        for (uint32_t j = 0; j < MBED_CONF_NSAPI_SOCKET_STATS_MAX_COUNT; j++) {
            if (SOCK_CLOSED == _stats[j].state) {
                if ((0 == oldest_time) || (oldest_time < _stats[j].last_change_tick)) {
                    oldest_time = _stats[j].last_change_tick;
                    position = j;
                }
            }
        }
        if (-1 == position) {
            MBED_ERROR(MBED_MAKE_ERROR(MBED_MODULE_NETWORK_STATS, MBED_ERROR_CODE_OUT_OF_RESOURCES), "List full with all open sockets");
        }
        memset(&_stats[position], 0, sizeof(mbed_stats_socket_t));
        _stats[position].reference_id = (Socket *)reference_id;
    }
    _mutex->unlock();
#endif
    return;
}

void SocketStats::stats_update_socket_state(const Socket *const reference_id, socket_state state)
{
#if MBED_CONF_NSAPI_SOCKET_STATS_ENABLED
    _mutex->lock();
    int position = get_entry_position(reference_id);
    if (position >= 0) {
        _stats[position].state = state;
#ifdef MBED_CONF_RTOS_PRESENT
        _stats[position].last_change_tick = rtos::Kernel::get_ms_count();
#endif
    }
    _mutex->unlock();
#endif
}

void SocketStats::stats_update_peer(const Socket *const reference_id, const SocketAddress &peer)
{
#if MBED_CONF_NSAPI_SOCKET_STATS_ENABLED
    _mutex->lock();
    int position = get_entry_position(reference_id);
    if ((position >= 0) && (!_stats[position].peer)) {
        _stats[position].peer = peer;
    }
    _mutex->unlock();
#endif
}

void SocketStats::stats_update_proto(const Socket *const reference_id, nsapi_protocol_t proto)
{
#if MBED_CONF_NSAPI_SOCKET_STATS_ENABLED
    _mutex->lock();
    int position = get_entry_position(reference_id);
    if (position >= 0) {
        _stats[position].proto = proto;
    }
    _mutex->unlock();
#endif
}

void SocketStats::stats_update_sent_bytes(const Socket *const reference_id, size_t sent_bytes)
{
#if MBED_CONF_NSAPI_SOCKET_STATS_ENABLED
    _mutex->lock();
    int position = get_entry_position(reference_id);
    if ((position >= 0) && ((int32_t)sent_bytes > 0)) {
        _stats[position].sent_bytes += sent_bytes;
    }
    _mutex->unlock();
#endif
}

void SocketStats::stats_update_recv_bytes(const Socket *const reference_id, size_t recv_bytes)
{
#if MBED_CONF_NSAPI_SOCKET_STATS_ENABLED
    _mutex->lock();
    int position = get_entry_position(reference_id);
    if ((position >= 0) && ((int32_t)recv_bytes > 0)) {
        _stats[position].recv_bytes += recv_bytes;
    }
    _mutex->unlock();
#endif
}
