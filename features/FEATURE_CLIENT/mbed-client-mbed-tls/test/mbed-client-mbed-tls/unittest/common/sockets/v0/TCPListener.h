/*
 * PackageLicenseDeclared: Apache-2.0
 * Copyright (c) 2015 ARM Limited
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
#ifndef __MBED_NET_SOCKETS_V0_TCPLISTENER_H__
#define __MBED_NET_SOCKETS_V0_TCPLISTENER_H__

#include <stddef.h>
#include <stdint.h>
#include "mbed/FunctionPointer.h"
#include "TCPAsynch.h"
#include "TCPStream.h"

namespace mbed {
namespace Sockets {
namespace v0 {
/** \brief TCPListener: a TCP server socket
 *  The TCPListener listens for incoming connections.  Prior to listening, the
 *  TCPListener must be bound to a port and, optionally, an address.
 */
class TCPListener: public TCPAsynch {
public:
    typedef FunctionPointer2<void, TCPListener *, void *> IncomingHandler_t;
    /**
     *  The TCP Listener constructor.
     * @param[in] stack the network stack to use
     */
    TCPListener(const socket_stack_t stack);

    /**
     *  The TCP Listener destructor
     *  This performs teardown of the listener
     */
    ~TCPListener();

    /**
     *  Begin listening for incoming connections.
     *  The supplied listenHandler will be called whenever an incoming connection arrives
     *  To reject a connection, the event handler needs to set the reject flag in the event.
     * @param[in] listenHandler The event handler to call when an incoming connection arrives
     * @param[in] backlog The number of connection requests to keep in the backlog
     * @return SOCKET_ERROR_NONE on success, or an error code on failure
     */
    socket_error_t start_listening(IncomingHandler_t listenHandler, uint32_t backlog = 0);
    /**
     * Stop listening for incoming connections
     * After this call, the server will reject incoming connections until start_listening is called again
     * @return SOCKET_ERROR_NONE on success, or an error code on failure
     */
    socket_error_t stop_listening();

    /**
     * accept is a utility function for constructing a new TCPStream from an incoming connection
     * The event passed to the listenHandler contains a pointer to a low level implementation of an
     * already connected socket.  This function
     * @param new_impl
     * @return
     */
    virtual TCPStream * accept(void *new_impl);
    virtual void reject(void *new_impl);

protected:
    /**
     * Internal event handler
     * @param[in] ev the event to handle
     */
    void _eventHandler(struct socket_event *ev);

    IncomingHandler_t _onIncoming;
};
} // namespace v0
} // namespace Sockets
} // namespace mbed
#endif // __MBED_NET_SOCKETS_V0_TCPLISTENER_H__
