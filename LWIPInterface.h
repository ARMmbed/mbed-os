/* LWIP implementation of NetworkInterfaceAPI
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

#ifndef LWIP_INTERFACE_H
#define LWIP_INTERFACE_H

#include "EthernetInterface.h"
#include "rtos.h"
#include "lwip/netif.h"


/** LWIPInterface class
 *  Implementation of the NetworkInterface for LWIP
 */
class LWIPInterface : public EthernetInterface
{
public:

    // Implementation of EthernetInterface
    virtual int32_t connect();
    virtual int32_t disconnect();

    // Implementation of NetworkInterface
    virtual const char *getIPAddress();
    virtual const char *getMACAddress();

    virtual SocketInterface *createSocket(ns_protocol_t proto);
    virtual void destroySocket(SocketInterface *socket);

private:

    // Implementation of the TCP SocketInterface for LWIP
    class LWIPSocket : public SocketInterface
    {
    public:

        LWIPSocket(int fd) : fd(fd) {}
        int fd;

        // Implementation of SocketInterface
        virtual int32_t open(const char *ip, uint16_t port);
        virtual int32_t close();

        virtual int32_t send(const void *data, uint32_t size);
        virtual int32_t recv(void *data, uint32_t size);
    };
};


#endif
