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

#ifndef SOCKET_STATS_H
#define SOCKET_STATS_H

#include "platform/mbed_stats.h"
#include "platform/SingletonPtr.h"
#include "platform/PlatformMutex.h"
#include "netsocket/Socket.h"
#include "SocketAddress.h"
#include "hal/ticker_api.h"

#ifndef MBED_CONF_NSAPI_SOCKET_STATS_MAX_COUNT
#define MBED_CONF_NSAPI_SOCKET_STATS_MAX_COUNT      10
#endif

typedef enum {
    SOCK_CLOSED,                    /**< Socket is closed and does not exist anymore in the system */
    SOCK_OPEN,                      /**< Socket is open, but not associated to any peer address */
    SOCK_CONNECTED,                 /**< Socket is associated to peer address, either by connect() or sendto()/recvfrom() calls */
    SOCK_LISTEN,                    /**< Socket is listening for incoming connections */
}socket_state;

typedef struct {
    Socket *reference_id;           /**< Used for identifying socket */
    SocketAddress peer;             /**< Last associated peername of this socket (Destination address) */
    socket_state state;             /**< State of this socket */
    nsapi_protocol_t proto;         /**< Specifies a protocol used with socket */
    size_t sent_bytes;              /**< Data sent through this socket */
    size_t recv_bytes;              /**< Data received through this socket */
    us_timestamp_t last_change_tick;/**< osKernelGetTick() when state last changed */
} mbed_stats_socket_t;

/**  SocketStats class
 *
 *  
 */
class SocketStats
{
public:    
    SocketStats();
    virtual ~SocketStats()
    {
    }

    /**
     *  Fill the passed array of structures with the socket statistics for each created socket.
     *
     *  @param stats    A pointer to an array of mbed_stats_socket_t structures to fill
     *  @param count    The number of mbed_stats_socket_t structures in the provided array
     *  @return         The number of mbed_stats_socket_t structures that have been filled.
     *                  If the number of sockets on the system is less than or equal to count, 
     *                  it will equal the number of sockets created (active / closed).
     *                  If the number of sockets on the system is greater than count, 
     *                  it will equal count.
     */
    static size_t mbed_stats_socket_get_each(mbed_stats_socket_t *stats, size_t count);

    /** Add entry of newly created socket in statistics array.
        @Note: Entry in the array will be maintained even after socket is closed. 
               It will be over-written for closed sockets when socket entries in 
               `MBED_CONF_NSAPI_SOCKET_STATS_MAX_COUNT` exceed.
      */
    void stats_new_socket_entry(const Socket *const reference_id);
    
    /** Updates the state of socket and along with that records tick_last_change */
    void stats_update_socket_state(const Socket *const reference_id, socket_state state);
    
    /** Update the peer information of the socket */
    void stats_update_peer(const Socket *const reference_id, const SocketAddress &peer);
    
    /** Update socket protocol */
    void stats_update_proto(const Socket *const reference_id, nsapi_protocol_t proto);
    
    /** Update bytes sent on socket, which is cumulative count per socket */
    void stats_update_sent_bytes(const Socket *const reference_id, size_t sent_bytes);

    /** Update bytes received on socket, which is cumulative count per socket */
    void stats_update_recv_bytes(const Socket *const reference_id, size_t recv_bytes);

#if defined(MBED_NW_STATS_ENABLED)
private:
    static mbed_stats_socket_t _stats[MBED_CONF_NSAPI_SOCKET_STATS_MAX_COUNT];
    static SingletonPtr<PlatformMutex> _mutex;
    static uint32_t _size;

    /** Internal function to scan the array and get position of element in the list.
        This API locks the mutex and next API updating the entry in array 
        should release the lock */
    int get_entry_position(const Socket *const reference_id);
#endif
};

#endif
