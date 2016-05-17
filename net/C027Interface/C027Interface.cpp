/* C027 implementation of NetworkInterfaceAPI
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

#include "C027Interface.h"
#include "mbed.h"


// C027Interface implementation
C027Interface::C027Interface(const char *simpin, bool debug)
    : _debug(debug)
{
    strcpy(_pin, simpin);
}

int C027Interface::connect(const char *apn, const char *username, const char *password)
{
    // create the modem
    _mdm = new MDMSerial;
    if (_debug) {
        _mdm->setDebug(4);
    } else {
        _mdm->setDebug(0);
    }
    
    // initialize the modem 
    MDMParser::DevStatus devStatus = {};
    MDMParser::NetStatus netStatus = {};
    bool mdmOk = _mdm->init(_pin, &devStatus);
    if (_debug) {
        _mdm->dumpDevStatus(&devStatus);
    }
    
    if (mdmOk) {
        // wait until we are connected
        mdmOk = _mdm->registerNet(&netStatus);
        if (_debug) {
            _mdm->dumpNetStatus(&netStatus);
        }
    }
    
    if (mdmOk) {
        // join the internet connection 
        MDMParser::IP ip = _mdm->join(apn, username, password);
        _ip_address.set_ip_bytes(&ip, NSAPI_IPv4);
        mdmOk = (ip != NOIP);
    }
    
    return mdmOk ? 0 : NSAPI_ERROR_DEVICE_ERROR;
}

int C027Interface::disconnect()
{
    if (!_mdm->disconnect()) {
        return NSAPI_ERROR_DEVICE_ERROR;
    }
    
    return 0;
}

const char *C027Interface::get_ip_address()
{
    return _ip_address.get_ip_address();
}

const char *C027Interface::get_mac_address()
{
    return 0;
}

struct c027_socket {
    int socket;
    MDMParser::IpProtocol proto;
    MDMParser::IP ip;
    int port;
};

int C027Interface::socket_open(void **handle, nsapi_protocol_t proto)
{
    MDMParser::IpProtocol mdmproto = (proto == NSAPI_UDP) ? MDMParser::IPPROTO_UDP : MDMParser::IPPROTO_TCP;
    int fd = _mdm->socketSocket(mdmproto);
    if (fd < 0) {
        return NSAPI_ERROR_NO_SOCKET;
    }
    
    _mdm->socketSetBlocking(fd, 10000);
    struct c027_socket *socket = new struct c027_socket;
    if (!socket) {
        return NSAPI_ERROR_NO_SOCKET;
    }

    socket->socket = fd;
    socket->proto = mdmproto;
    *handle = socket;
    return 0;
}

int C027Interface::socket_close(void *handle)
{
    struct c027_socket *socket = (struct c027_socket *)handle;
    _mdm->socketFree(socket->socket);

    delete socket;
    return 0;
}

int C027Interface::socket_bind(void *handle, const SocketAddress &address)
{
    return NSAPI_ERROR_UNSUPPORTED;
}

int C027Interface::socket_listen(void *handle, int backlog)
{
    return NSAPI_ERROR_UNSUPPORTED;
}

int C027Interface::socket_connect(void *handle, const SocketAddress &addr)
{
    struct c027_socket *socket = (struct c027_socket *)handle;
    
    if (!_mdm->socketConnect(socket->socket, addr.get_ip_address(), addr.get_port())) {
        return NSAPI_ERROR_DEVICE_ERROR;
    }
    
    return 0;
}

int C027Interface::socket_accept(void **handle, void *server)
{
    return NSAPI_ERROR_UNSUPPORTED;
}

int C027Interface::socket_send(void *handle, const void *data, unsigned size)
{
    struct c027_socket *socket = (struct c027_socket *)handle;

    int sent = _mdm->socketSend(socket->socket, (const char *)data, size);
    if (sent == SOCKET_ERROR) {
        return NSAPI_ERROR_DEVICE_ERROR;
    }
    
    return sent;
}

int C027Interface::socket_recv(void *handle, void *data, unsigned size)
{
    struct c027_socket *socket = (struct c027_socket *)handle;
    if (!_mdm->socketReadable(socket->socket)) {
        return NSAPI_ERROR_WOULD_BLOCK;
    }
    
    int recv = _mdm->socketRecv(socket->socket, (char *)data, size);
    if (recv == SOCKET_ERROR) {
        return NSAPI_ERROR_DEVICE_ERROR;
    }
    
    return recv;
}

int C027Interface::socket_sendto(void *handle, const SocketAddress &addr, const void *data, unsigned size)
{
    struct c027_socket *socket = (struct c027_socket *)handle;

    int sent = _mdm->socketSendTo(socket->socket,
            *(MDMParser::IP *)addr.get_ip_bytes(), addr.get_port(),
            (const char *)data, size);
            
    if (sent == SOCKET_ERROR) {
        return NSAPI_ERROR_DEVICE_ERROR;
    }
    
    return sent;
}

int C027Interface::socket_recvfrom(void *handle, SocketAddress *addr, void *data, unsigned size)
{
    struct c027_socket *socket = (struct c027_socket *)handle;
    if (!_mdm->socketReadable(socket->socket)) {
        return NSAPI_ERROR_WOULD_BLOCK;
    }
    
    MDMParser::IP ip;
    int port;

    int recv = _mdm->socketRecvFrom(socket->socket, &ip, &port, (char *)data, size);
    if (recv == SOCKET_ERROR) {
        return NSAPI_ERROR_DEVICE_ERROR;
    }

    if (addr) {
        addr->set_ip_bytes(&ip, NSAPI_IPv4);
        addr->set_port(port);
    }
    
    return recv;
}

void C027Interface::socket_attach(void *handle, void (*callback)(void *), void *data)
{
}
