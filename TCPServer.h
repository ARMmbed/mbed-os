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

#ifndef TCPSERVER_H
#define TCPSERVER_H

#include "Socket.h"
#include "TCPSocket.h"
#include "NetworkInterface.h"

/** TCP Server.
  */
class TCPServer : public Socket {
public:
    /** TCP Server lifetime
     */
    TCPServer();
    TCPServer(NetworkInterface *iface);
    virtual ~TCPServer();

    /** Open the socket
     *  @param iface    Interface to open socket on
     */
    virtual int open(NetworkInterface *iface);
    
    /** Bind a socket to a specific port
     * @param port      The port to listen for incoming connections on
     * @return          0 on success, negative on failure
     */
    int bind(uint16_t port);
    
    /** Start listening for incoming connections
     * @param backlog   Number of pending connections that can be queued up at any
     *                  one time [Default: 1]
     * @return          0 on success, negative on failure
     */
    int listen(int backlog=1);
    
    /** Accept a new connection.
     * @param socket    A TCPSocket instance that will handle the incoming connection.
     * @return          0 on success, negative on failure.
     */
    int accept(TCPSocket *connection);
};

#endif
