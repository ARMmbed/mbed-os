/* Socket
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

#ifndef UDPSOCKET_H
#define UDPSOCKET_H

#include "Socket.h"
#include "NetworkInterface.h"

/**
UDP Socket
*/
class UDPSocket : public Socket {
public:
    /** UDPSocket lifetime
    */
    UDPSocket(NetworkInterface *iface);
    virtual ~UDPSocket();
    
    /** Bind a UDP Server Socket to a specific port
    \param port The port to listen for incoming connections on
    \return 0 on success, negative on failure.
    */
    int bind(uint16_t port);

    /** Send a packet to a remote endpoint
    \param address  The remote SocketAddress
    \param data     The packet to be sent
    \param size     The length of the packet to be sent
    \return the number of written bytes on success, negative on failure
    */
    int sendto(const SocketAddress &address, const void *data, unsigned size);

    /** Receive a packet from a remote endpoint
    \param address  Destination for the remote SocketAddress or null
    \param buffer   The buffer for storing the incoming packet data
                    If a packet is too long to fit in the supplied buffer,
                    excess bytes are discarded
    \param size     The length of the buffer
    \return the number of received bytes on success, negative on failure
    */
    int recvfrom(SocketAddress *address, void *buffer, unsigned size);

    /** Register a callback on when send is ready
    \param callback Function to call when send will succeed, may be called in
                    interrupt context.
    */
    void attach_send(FunctionPointer callback);

    template <typename T, typename M>
    void attach_send(T *tptr, M mptr) {
        attach_send(FunctionPointer(tptr, mptr));
    }

    /** Register a callback on when recv is ready
    \param callback Function to call when recv will succeed, may be called in
                    interrupt context.
    */
    void attach_recv(FunctionPointer callback);

    template <typename T, typename M>
    void attach_recv(T *tptr, M mptr) {
        attach_recv(FunctionPointer(tptr, mptr));
    }

private:
    FunctionPointer _send_cb;
    FunctionPointer _recv_cb;
};

#endif
