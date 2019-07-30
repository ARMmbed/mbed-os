/* CC3220SF implementation of NetworkInterfaceAPI
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

#include "cc3200_simplelink.h"
#include "CC3220SF_WiFiInterface.h"

#define CONNECT_TIMEOUT_MS     (4000)
#define IP_SET_TIMEOUT_MS      (2000)
#define READ_THREAD_SLEEP_MS   (50)
#define SOCKET_TASK_STACK_SIZE (2048)

CC3220SFInterface * CC3220SFInterface::cc3200sf_wifi_instance;

CC3220SFInterface::CC3220SFInterface():
    _thread_read_socket(osPriorityNormal, SOCKET_TASK_STACK_SIZE, NULL, "read_socket_thread"),
    _initialized(false),
    _started(false),
    _channel(6),
    _connect_sem(0, 1),
    _ip_set_sem(0, 1)
{
    memset(_id_in_use, 0, sizeof(_id_in_use));
    memset(_socket_obj, 0, sizeof(_socket_obj));
    memset(_cbs, 0, sizeof(_cbs));
    memset(_ssid, 0, sizeof(_ssid));
    memset(_pass, 0, sizeof(_pass));
    _security = NSAPI_SECURITY_UNKNOWN;
    cc3200sf_wifi_instance = this;
    _cc3200_simplelink.initialize();
    _thread_read_socket.start(callback(this, &CC3220SFInterface::_socket_background_thread));
}

nsapi_error_t CC3220SFInterface::connect(const char *ssid, const char *pass, nsapi_security_t security,
        uint8_t channel)
{
    nsapi_error_t ret_code;

    ret_code = set_credentials(ssid, pass, security);
    if(ret_code)
    {
        printf ("set_credentials failed with 0x%x\n", ret_code);
        return ret_code;
    }

    return connect();
}

nsapi_error_t CC3220SFInterface::connect()
{
    nsapi_error_t status;

    if (strlen(_ssid) == 0)
    {
        return NSAPI_ERROR_NO_SSID;
    }

    if(get_ip_address())
    {
        return NSAPI_ERROR_IS_CONNECTED;
    }

    _cc3200_simplelink.set_connection_status(NSAPI_STATUS_CONNECTING);

    status = _startup(CC3200_SIMPLELINK::WIFI_ROLE_STATION);
    if(status != NSAPI_ERROR_OK)
    {
        return status;
    }
    _started = true;

    if (!_cc3200_simplelink.dhcp(true, CC3200_SIMPLELINK::WIFI_ROLE_STATION))
    {
        return NSAPI_ERROR_DHCP_FAILURE;
    }
    int connect_error = _cc3200_simplelink.connect(_ssid, _pass, _security);
    if (connect_error)
    {
        return connect_error;
    }

    // Connect is async call. Wait till connection completes
    if (_connect_sem.try_acquire_for(CONNECT_TIMEOUT_MS) == false)
    {
        printf("Connection timed out\n");
        return NSAPI_ERROR_CONNECTION_TIMEOUT;
    }
    if (_ip_set_sem.try_acquire_for(IP_SET_TIMEOUT_MS) == false)
    {
        printf("IP address set timed out\n");
    }
    else
    {
        _cc3200_simplelink.set_connection_status(NSAPI_STATUS_GLOBAL_UP);
    }

    if (!get_ip_address())
    {
        return NSAPI_ERROR_DHCP_FAILURE;
    }

    return NSAPI_ERROR_OK;
}

nsapi_error_t CC3220SFInterface::set_credentials(const char *ssid, const char *pass, nsapi_security_t security)
{
    if (!ssid) {
        return NSAPI_ERROR_PARAMETER;
    }

    if ((strlen(ssid) == 0) || (strlen(ssid) > 32)) {
        return NSAPI_ERROR_PARAMETER;
    }

    if ((security != NSAPI_SECURITY_NONE) && (strcmp(pass, "") == 0)) {
        return NSAPI_ERROR_PARAMETER;
    }

    switch (security)
    {
        case NSAPI_SECURITY_WPA:
        case NSAPI_SECURITY_WPA2:
        case NSAPI_SECURITY_WPA_WPA2:
        case NSAPI_SECURITY_WEP:
            if ((strlen(pass) < 8) || (strlen(pass) > 63)) { // 802.11 password 8-63 characters
                return NSAPI_ERROR_PARAMETER;
            }
            break;
        case NSAPI_SECURITY_NONE:
            break;
        default:
            return NSAPI_ERROR_PARAMETER;
    }

    if (security != NSAPI_SECURITY_NONE)
    {
        if(!pass)
        {
            return NSAPI_ERROR_PARAMETER;
        }
    }

    strncpy(_ssid, ssid, sizeof(_ssid));
    strncpy(_pass, pass, sizeof(_pass));
    _security = security;

    return NSAPI_ERROR_OK;
}

nsapi_error_t CC3220SFInterface::set_channel(uint8_t channel)
{
    return NSAPI_ERROR_UNSUPPORTED;
}

nsapi_error_t CC3220SFInterface::disconnect()
{
    nsapi_error_t status;

    _started = false;
    _initialized = false;

    if (_cc3200_simplelink.disconnect())
    {
        status = NSAPI_ERROR_OK;
        _cc3200_simplelink.set_connection_status(NSAPI_STATUS_DISCONNECTED);
    }
    else
    {
        status = NSAPI_ERROR_DEVICE_ERROR;
    }
    return status;
}

const char *CC3220SFInterface::get_ip_address()
{
    if(!_started) {
        return NULL;
    }

    const char *ip_buff = _cc3200_simplelink.getIPAddress();
    if(!ip_buff || std::strcmp(ip_buff, "0.0.0.0") == 0) {
        return NULL;
    }

    return ip_buff;
}

const char *CC3220SFInterface::get_mac_address()
{
    return _cc3200_simplelink.getMACAddress();
}

const char *CC3220SFInterface::get_gateway()
{
    return _started ? _cc3200_simplelink.getGateway() : NULL;
}

const char *CC3220SFInterface::get_netmask()
{
    return _started ? _cc3200_simplelink.getNetmask() : NULL;
}

int8_t CC3220SFInterface::get_rssi()
{
    return _started ? _cc3200_simplelink.getRSSI() : 0;
}

int CC3220SFInterface::scan(WiFiAccessPoint *res, unsigned count)
{
    nsapi_error_t status;

    status = _startup(CC3200_SIMPLELINK::WIFI_ROLE_STATION);
    if(status != NSAPI_ERROR_OK) {
        return status;
    }

    return _cc3200_simplelink.scan(res, count);
}

nsapi_error_t CC3220SFInterface::_startup(const int8_t wifi_mode)
{
    if (!_started) {
        if (!_cc3200_simplelink.startup(wifi_mode)) {
            return NSAPI_ERROR_DEVICE_ERROR;
        }
    }
    return NSAPI_ERROR_OK;
}

void CC3220SFInterface::attach(mbed::Callback<void(nsapi_event_t, intptr_t)> status_cb)
{
    _cc3200_simplelink.attach(status_cb);
}

nsapi_connection_status_t CC3220SFInterface::get_connection_status() const
{
    return _cc3200_simplelink.get_connection_status();
}

nsapi_error_t CC3220SFInterface::socket_open(void **handle, nsapi_protocol_t proto)
{
    int32_t sd, i, ret = NSAPI_ERROR_PARAMETER;

    if (!handle)
    {
        return ret;
    }

    _mutex.lock();
    for (i = 0; i < CC3220SF_SOCKET_COUNT; i++) {
        if (_id_in_use[i] == false)
        {
            _id_in_use[i] = true;
            break;
        }
    }

    if (i == CC3220SF_SOCKET_COUNT) {
        _mutex.unlock();
        return NSAPI_ERROR_NO_SOCKET;
    }

    struct cc3200_socket *socket = new struct cc3200_socket;
    if (!socket)
    {
        _mutex.unlock();
        return NSAPI_ERROR_NO_SOCKET;
    }

    sd = _cc3200_simplelink.open_socket(proto);
    if (sd >= 0) // socket open succeeded
    {
        socket->id = i;
        socket->sd = sd;
        socket->proto = proto;
        socket->connected = false;
        socket->read_data_available = 0;
        socket->data_to_read = 0;
        socket->callback_pending = false;
        *handle = socket;
        _socket_obj[i] = (void *)socket;
        ret = NSAPI_ERROR_OK;
    }
    else
    {
        delete socket;
        *handle = NULL;
        ret = NSAPI_ERROR_NO_SOCKET;
    }
    _mutex.unlock();
    return ret;
}

nsapi_error_t CC3220SFInterface::socket_close(void *handle)
{
    struct cc3200_socket *socket = (struct cc3200_socket *)handle;
    int err = NSAPI_ERROR_OK;

    if (!socket) {
        return NSAPI_ERROR_NO_SOCKET;
    }

    _mutex.lock();
    if (!_cc3200_simplelink.close_socket(socket->sd))
    {
        err = NSAPI_ERROR_DEVICE_ERROR;
    }
    socket->connected = false;
    socket->callback_pending = false;
    _id_in_use[socket->id] = false;
    _socket_obj[socket->id] = 0;
    _mutex.unlock();
    delete socket;
    return err;
}

#if 0
nsapi_error_t CC3220SFInterface::gethostbyname(const char *name, SocketAddress *address, nsapi_version_t version)
{
    nsapi_addr_t ip_address;
    ip_address.version = NSAPI_IPv4;
    if (_cc3200_simplelink.dns_lookup(name, (char*)ip_address.bytes, sizeof(ip_address.bytes), version) == NSAPI_ERROR_OK)
    {
         address->set_addr(ip_address);
         return NSAPI_ERROR_OK;
    }
    else
    {
        return NSAPI_ERROR_DNS_FAILURE;
    }
}
#endif

nsapi_error_t CC3220SFInterface::get_dns_server(int index, SocketAddress *address, const char *interface_name)
{
    nsapi_addr_t dns_address;
    dns_address.version = NSAPI_IPv4;
    if (_cc3200_simplelink.getDNS(dns_address.bytes, sizeof(dns_address.bytes)) == NSAPI_ERROR_OK)
    {
        address->set_addr(dns_address);
        return NSAPI_ERROR_OK;
    }
    else
    {
        return NSAPI_ERROR_DNS_FAILURE;
    }
}

nsapi_error_t CC3220SFInterface::setsockopt(nsapi_socket_t handle, int level,
        int optname, const void *optval, unsigned optlen)
{
    struct cc3200_socket *socket = (struct cc3200_socket *)handle;
    if (!socket)
    {
        return NSAPI_ERROR_NO_SOCKET;
    }
    _mutex.lock();
    nsapi_error_t retcode = _cc3200_simplelink.setsockopt(socket->sd, level, optname, optval, optlen);
    _mutex.unlock();
    return retcode;
}

nsapi_error_t CC3220SFInterface::getsockopt(nsapi_socket_t handle, int level, int optname,
        void *optval, unsigned *optlen)
{
    struct cc3200_socket *socket = (struct cc3200_socket *)handle;
    if (!socket)
    {
        return NSAPI_ERROR_NO_SOCKET;
    }
    _mutex.lock();
    nsapi_error_t retcode = _cc3200_simplelink.getsockopt(socket->sd, level, optname, optval, optlen);
    _mutex.unlock();

    return retcode;
}

int CC3220SFInterface::socket_bind(void *handle, const SocketAddress &address)
{
    struct cc3200_socket *socket = (struct cc3200_socket *)handle;
    if (!socket)
    {
        return NSAPI_ERROR_NO_SOCKET;
    }

    _mutex.lock();

    int ret = _cc3200_simplelink.bind_socket(socket->sd, address);
    _mutex.unlock();
    return ret;
}

int CC3220SFInterface::socket_listen(void *handle, int backlog)
{
    return NSAPI_ERROR_UNSUPPORTED;
}

int CC3220SFInterface::socket_connect(void *handle, const SocketAddress &addr)
{
    struct cc3200_socket *socket = (struct cc3200_socket *)handle;
    if (!socket)
    {
        return NSAPI_ERROR_NO_SOCKET;
    }
    _mutex.lock();

    int ret;
    if (_cc3200_simplelink.connect_socket(socket->sd, addr) == NSAPI_ERROR_OK)
    {
        socket->connected= true;
        ret = NSAPI_ERROR_OK;
    }
    else
    {
        printf("socket_connect failed\n");
        ret = NSAPI_ERROR_DEVICE_ERROR;
    }
    _mutex.unlock();
    return ret;
}

int CC3220SFInterface::socket_accept(void *handle, void **socket, SocketAddress *address)
{
    // TODO
    return NSAPI_ERROR_UNSUPPORTED;
}

int CC3220SFInterface::socket_send(void *handle, const void *data, unsigned size)
{
    struct cc3200_socket *socket = (struct cc3200_socket *)handle;
    if (!socket)
    {
        return NSAPI_ERROR_NO_SOCKET;
    }
    _mutex.lock();
    int ret = _cc3200_simplelink.send(socket->sd, data, size);
    if (ret > 0)
    {
        socket->callback_pending = true;
    }
    _mutex.unlock();

    return ret;
}

int CC3220SFInterface::socket_recv(void *handle, void *data, unsigned size)
{
    struct cc3200_socket *socket = (struct cc3200_socket *)handle;
    if (!socket)
    {
        return NSAPI_ERROR_NO_SOCKET;
    }
    _mutex.lock();
    socket->data_to_read = size;
    int ret = _socket_receive_data(socket, data);
    _mutex.unlock();

    return ret;
}

int CC3220SFInterface::socket_sendto(void *handle, const SocketAddress &address, const void *data, unsigned size)
{
    struct cc3200_socket *socket = (struct cc3200_socket *)handle;
    if (!socket)
    {
        return NSAPI_ERROR_NO_SOCKET;
    }

    _mutex.lock();
    // Sending a datagram of length 0 is OK
    int ret = _cc3200_simplelink.sendto_socket(socket->sd, data, size, address);

    if (ret >= 0)
    {
        socket->callback_pending = true;
    }
    _mutex.unlock();

    return ret;
}

int CC3220SFInterface::socket_recvfrom(void *handle, SocketAddress *address, void *buffer, unsigned size)
{
    struct cc3200_socket *socket = (struct cc3200_socket *)handle;
    if (!socket)
    {
        return NSAPI_ERROR_NO_SOCKET;
    }

    _mutex.lock();
    // It's okay to receive 0 for a datagram protocol. Unlike TCP, it doesn't mean the peer has closed the connection.
    socket->data_to_read = size;
    int ret = _socket_receive_data(socket, buffer);
    if (ret >= 0 && address)
    {
        *address = socket->addr;
    }
    _mutex.unlock();

    return ret;
}

void CC3220SFInterface::socket_attach(void *handle, void (*callback)(void *), void *data)
{
    struct cc3200_socket *socket = (struct cc3200_socket *)handle;
    if (socket)
    {
        _mutex.lock();
        _cbs[socket->id].callback = callback;
        _cbs[socket->id].data = data;
        _mutex.unlock();
    }
}

void CC3220SFInterface::_socket_background_thread()
{
    while (1)
    {
        for (int i = 0; i < CC3220SF_SOCKET_COUNT; i++)
        {
            _mutex.lock();
            if (_socket_obj[i])
            {
                struct cc3200_socket *socket = (struct cc3200_socket *)_socket_obj[i];
                /* Check if an async event needs to be sent*/
                if ((socket->callback_pending) && _cbs[socket->id].callback)
                {
                    _cbs[socket->id].callback(_cbs[socket->id].data);
                    socket->callback_pending = false;
                }
                /* Check if there is something to read for this socket. But if it */
                /* has already been read : don't read again */
                if ((socket->read_data_available == 0) && (socket->data_to_read) && _cbs[socket->id].callback)
                {
                    int read_amount, bytes_to_receive = socket->data_to_read;
                    if (bytes_to_receive > MAX_RECV_PACKET_SIZE)
                    {
                        bytes_to_receive = MAX_RECV_PACKET_SIZE;
                    }
                    if (socket->proto == NSAPI_UDP)
                    {
                        read_amount = _cc3200_simplelink.recvfrom(socket->sd, socket->read_data_internal_buffer, bytes_to_receive, socket->addr);
                    }
                    else
                    {
                        read_amount= _cc3200_simplelink.recv(socket->sd, socket->read_data_internal_buffer, bytes_to_receive);
                    }
                    //printf("background receive %d %d\n", bytes_to_receive, read_amount);
                    if (read_amount > 0)
                    {
                        socket->read_data_available = read_amount;
                        // For UDP packet, no packet fragmentation, so none or all
                        if (socket->proto == NSAPI_UDP)
                        {
                            socket->data_to_read = 0;
                        }
                        else
                        {
                            socket->data_to_read -= read_amount;
                        }
                    }
                    else if ((read_amount < 0 && read_amount != NSAPI_ERROR_WOULD_BLOCK) || (read_amount == 0))
                    {
                        /* Mark connection has been lost or closed */
                        printf("Connection lost\n");
                        socket->connected = false;
                        socket->data_to_read = 0;
                    }

                    if (read_amount >= 0)
                    {
                        /* There is something to read in this socket*/
                        _cbs[socket->id].callback(_cbs[socket->id].data);
                    }
                }
            }
            _mutex.unlock();
        }
        ThisThread::sleep_for(READ_THREAD_SLEEP_MS);
    }
}

// This function is expected to run with mutex protection
int CC3220SFInterface::_socket_receive_data(void *obj, void *data)
{
    int   recv = 0;
    char *ptr  = (char *)data;
    int   bytes_to_receive = 0;

    cc3200_socket *socket = (cc3200_socket *)obj;

    if (socket->read_data_available == 0)
    {
        // Need to receive
        int read_amount;

        // Simplelink can receive up to MAX_RECV_PACKET_SIZE
        bytes_to_receive = socket->data_to_read;
        if (bytes_to_receive > MAX_RECV_PACKET_SIZE)
        {
            bytes_to_receive = MAX_RECV_PACKET_SIZE;
        }
        if (socket->proto == NSAPI_UDP) // TODO: is this good enough to distinguish recvfrom and recv?
        {
            read_amount = _cc3200_simplelink.recvfrom(socket->sd, socket->read_data_internal_buffer, bytes_to_receive, socket->addr);
        }
        else
        {
            read_amount= _cc3200_simplelink.recv(socket->sd, socket->read_data_internal_buffer, bytes_to_receive);
        }
        //printf("foreground receive %d %d\n", bytes_to_receive, read_amount);
        if (read_amount == 0)
        {
            // No data to read
            socket->data_to_read = 0;
            return 0;
        }
        if (read_amount > 0)
        {
            socket->read_data_available = read_amount;
        }
        else // read_amount < 0
        {
            if (read_amount == NSAPI_ERROR_WOULD_BLOCK)
            {
                return NSAPI_ERROR_WOULD_BLOCK;
            }
            else
            {
                socket->connected = false;
                printf("connection lost\n");
                socket->data_to_read = 0;
                return NSAPI_ERROR_CONNECTION_LOST;
            }
        }
    }
    if (socket->read_data_available > 0)
    {
        // Data has been received or there's already data waiting to be copied into the buffer provided
        uint32_t i = 0;
        while (i < socket->read_data_available)
        {
            *ptr++ = socket->read_data_internal_buffer[i];
            i++;
        }
        recv += i;

        /* All the stored data has been read, reset buffer */
        memset(socket->read_data_internal_buffer, 0, sizeof(socket->read_data_internal_buffer));
        // For UDP packet, no packet fragmentation, so none or all
        if (socket->proto == NSAPI_UDP)
        {
            socket->data_to_read = 0;
        }
        else
        {
            socket->data_to_read -= socket->read_data_available;
        }
        socket->read_data_available = 0;
    }
    return recv;
}
