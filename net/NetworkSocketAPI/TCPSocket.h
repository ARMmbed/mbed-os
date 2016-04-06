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

#ifndef TCPSOCKET_H
#define TCPSOCKET_H

#include "Socket.h"
#include "NetworkInterface.h"

/**
TCP socket connection
*/
class TCPSocket : public Socket {
public:
    /** TCP socket lifetime
    */
    TCPSocket(NetworkInterface *iface);
    virtual ~TCPSocket();
    
    /** Connects this TCP socket to the server
    \param host     The host to connect to. It can either be an IP Address
                    or a hostname that will be resolved with DNS
    \param port     The host's port to connect to
    \return         0 on success, negative on failure
    */
    int connect(const char *host, uint16_t port);

    /** Connects this TCP socket to the server
    \param address  SocketAddress to connect to
    \return         0 on success, negative on failure
    */
    int connect(const SocketAddress &address);
    
    /** Check if the socket is connected
    \return         true if connected, false otherwise
    */
    bool is_connected();
    
    /** Send data to the remote host
    \param data     The buffer to send to the host
    \param size     The length of the buffer to send
    \return         Number of written bytes on success, negative on failure
    */
    int send(const void *data, unsigned size);
    
    /** Receive data from the remote host
    \param data     The buffer in which to store the data received from the host
    \param size     The maximum length of the buffer
    \return         Number of received bytes on success, negative on failure
    */
    int recv(void *data, unsigned size);

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
    friend class TCPServer;

    FunctionPointer _send_cb;
    FunctionPointer _recv_cb;
};

#endif
