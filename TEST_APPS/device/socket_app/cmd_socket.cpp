/*
 * Copyright (c) 2018 ARM Limited. All rights reserved.
 * SPDX-License-Identifier: Apache-2.0
 * Licensed under the Apache License, Version 2.0 (the License); you may
 * not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an AS IS BASIS, WITHOUT
 * WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
#include "mbed.h"
#include "NetworkStack.h"
#include "UDPSocket.h"
#include "TCPSocket.h"
#include "TCPServer.h"
#include "NetworkInterface.h"
#include "SocketAddress.h"
#include "Queue.h"

#include <vector>
#include <cctype>
#include <cassert>
#include <cstring>
#ifndef __STDC_FORMAT_MACROS
#define __STDC_FORMAT_MACROS
#endif
#include <inttypes.h>

#include "mbed-client-cli/ns_cmdline.h"
#include "mbed-trace/mbed_trace.h"
#include "strconv.h"

#define TRACE_GROUP "Asck"

#include "cmd_ifconfig.h"
#include "cmd_socket.h"

#define SIGNAL_SIGIO 0x1
#define PACKET_SIZE_ARRAY_LEN 5

#define MAN_SOCKET          "\r\nSOCKET API\r\n"\
                            "\r\n"\
                            "socket <operation> [options]\r\n\r\n"\
                            " new <type>\r\n" \
                            "   type: UDPSocket|TCPSocket|TCPServer\r\n"\
                            "   return socket id\r\n"\
                            " <id> delete\r\n"\
                            "   remote the space allocated for Socket\r\n"\
                            " <id> open\r\n"\
                            " <id> close\r\n"\
                            " <id> bind [port] <port> [addr <addr>]\r\n"\
                            " <id> set_blocking <bool>\r\n"\
                            " <id> set_timeout <ms>\r\n"\
                            " <id> register_sigio_cb\r\n"\
                            " <id> set_RFC_864_pattern_check <true|false>\r\n"\
                            "\r\nFor UDPSocket\r\n"\
                            " <id> sendto <addr> <port> (\"msg\" | --data_len <len>)\r\n"\
                            "   \"msg\" Send packet with defined string content\r\n"\
                            "    --data_len Send packet with random content with size <len>\r\n"\
                            " <id> recvfrom <len>\r\n"\
                            " <id> start_udp_receiver_thread --max_data_len <len> [--packets <N>]\r\n"\
                            "    --max_data_len Size of input buffer to fill up\r\n"\
                            "    --packets Receive N number of packets, default 1\r\n"\
                            "\r\nFor TCPSocket\r\n"\
                            " <id> connect <host> <port>\r\n"\
                            " <id> send (\"msg\" | --data_len <len>)\r\n"\
                            " <id> recv <len>\r\n"\
                            " <id> start_tcp_receiver_thread --data_len <len> [--max_recv_len <L>] [--repeat <N>]\r\n"\
                            "    --data_len Size of input buffer to fill up\r\n"\
                            "    --max_recv_len Read maximum of L bytes in at a time. Default full buffer <len>\r\n"\
                            "    --repeat Repeat buffer filling N times, default 1\r\n"\
                            " <id> join_tcp_receiver_thread\r\n"\
                            " <id> start_bg_traffic_thread\r\n"\
                            " <id> join_bg_traffic_thread\r\n"\
                            " <id> setsockopt_keepalive <seconds[0-7200]>\r\n"\
                            " <id> getsockopt_keepalive\r\n"\
                            "\r\nFor TCPServer\r\n"\
                            " <id> listen [backlog]\r\n"\
                            " <id> accept <new_id>\r\n"\
                            "   accept new connection into <new_id> socket. Requires <new_id> to be pre-allocated.\r\n"\
                            "\r\nOther options\r\n"\
                            " print-mode [--string|--hex|--disabled] [--col-width <width>]"

class SInfo;
static Queue<SInfo, 10> event_queue;
static int id_count = 0;

class SInfo {
public:
    enum SocketType {
        TCP_CLIENT,
        TCP_SERVER,
        UDP,
        OTHER
    };
    SInfo(TCPSocket *sock):
        _id(id_count++),
        _sock(sock),
        _type(SInfo::TCP_CLIENT),
        _blocking(true),
        _dataLen(0),
        _maxRecvLen(0),
        _repeatBufferFill(1),
        _receivedTotal(0),
        _receiverThread(NULL),
        _receiveBuffer(NULL),
        _senderThreadId(NULL),
        _receiverThreadId(NULL),
        _packetSizes(NULL),
        _check_pattern(false)
    {
        assert(sock);
    }
    SInfo(TCPServer *sock):
        _id(id_count++),
        _sock(sock),
        _type(SInfo::TCP_SERVER),
        _blocking(true),
        _dataLen(0),
        _maxRecvLen(0),
        _repeatBufferFill(1),
        _receivedTotal(0),
        _receiverThread(NULL),
        _receiveBuffer(NULL),
        _senderThreadId(NULL),
        _receiverThreadId(NULL),
        _packetSizes(NULL),
        _check_pattern(false)
    {
        assert(sock);
    }
    SInfo(UDPSocket *sock):
        _id(id_count++),
        _sock(sock),
        _type(SInfo::UDP),
        _blocking(true),
        _dataLen(0),
        _maxRecvLen(0),
        _repeatBufferFill(1),
        _receivedTotal(0),
        _receiverThread(NULL),
        _receiveBuffer(NULL),
        _senderThreadId(NULL),
        _receiverThreadId(NULL),
        _packetSizes(NULL),
        _check_pattern(false)
    {
        assert(sock);
    }
    SInfo(Socket* sock, bool delete_on_exit=true):
        _id(id_count++),
        _sock(sock),
        _type(SInfo::OTHER),
        _blocking(true),
        _dataLen(0),
        _maxRecvLen(0),
        _repeatBufferFill(1),
        _receivedTotal(0),
        _receiverThread(NULL),
        _receiveBuffer(NULL),
        _senderThreadId(NULL),
        _receiverThreadId(NULL),
        _packetSizes(NULL),
        _check_pattern(false)
    {
        MBED_ASSERT(sock);
    }
    ~SInfo()
    {
        this->_sock->sigio(Callback<void()>());
        if (this->_receiverThread) {
            this->_receiverThread->terminate();
            delete this->_receiverThread;
        }
        if (this->_receiveBuffer) {
            delete this->_receiveBuffer;
        }
        delete this->_sock;
    }
    int id() const
    {
        return this->_id;
    }
    Socket &socket()
    {
        return *(this->_sock);
    }
    Socket &socket() const
    {
        return *(this->_sock);
    }
    TCPSocket *tcp_socket()
    {
        return this->_type == SInfo::TCP_CLIENT ? static_cast<TCPSocket *>(this->_sock) : NULL;
    }
    TCPServer *tcp_server()
    {
        return this->_type == SInfo::TCP_SERVER ? static_cast<TCPServer *>(this->_sock) : NULL;
    }
    UDPSocket *udp_socket()
    {
        return this->_type == SInfo::UDP        ? static_cast<UDPSocket *>(this->_sock) : NULL;
    }
    SInfo::SocketType type() const
    {
        return this->_type;
    }
    void setDataCount(int dataCount)
    {
        this->_dataLen = dataCount;
    }
    int getDataCount()
    {
        return this->_dataLen;
    }
    void setReceiverThread(Thread *receiverThread)
    {
        this->_receiverThread = receiverThread;
    }
    Thread *getReceiverThread()
    {
        return this->_receiverThread;
    }
    void setReceiveBuffer(uint8_t *receiveBuffer)
    {
        this->_receiveBuffer = receiveBuffer;
    }
    uint8_t *getReceiveBuffer()
    {
        return this->_receiveBuffer;
    }
    void setMaxRecvLen(int recvLen)
    {
        this->_maxRecvLen = recvLen;
    }
    int getMaxRecvLen()
    {
        return this->_maxRecvLen;
    }
    void setRepeatBufferFill(int n)
    {
        this->_repeatBufferFill = n;
    }
    int getRepeatBufferFill()
    {
        return this->_repeatBufferFill;
    }
    void setRecvTotal(int n)
    {
        this->_receivedTotal = n;
    }
    int getRecvTotal()
    {
        return this->_receivedTotal;
    }
    void setSenderThreadId(osThreadId threadID)
    {
        this->_senderThreadId = threadID;
    }
    void setReceiverThreadId(osThreadId threadID)
    {
        this->_receiverThreadId = threadID;
    }
    osThreadId getSenderThreadId()
    {
        return this->_senderThreadId;
    }
    osThreadId getReceiverThreadId()
    {
        return this->_receiverThreadId;
    }
    void setPacketSizeArray(int *ptr)
    {
        this->_packetSizes = ptr;
    }
    int *getPacketSizeArray()
    {
        return this->_packetSizes;
    }
    void setUnavailable()
    {
        this->_available = false;
    }
    void setAvailable()
    {
        this->_available = true;
    }
    bool available()
    {
        return this->_available;
    }
    void set_pattern_check(bool enabled)
    {
        _check_pattern = enabled;
    };
    bool check_pattern(void *buffer, size_t len);

    const char *type_str() const
    {
        const char *str;
        switch (this->_type) {
            case SInfo::TCP_CLIENT:
                str = "TCPSocket";
                break;
            case SInfo::TCP_SERVER:
                str = "TCPServer";
                break;
            case SInfo::UDP:
                str = "UDPSocket";
                break;
            default:
                assert(0);
                break;
        }
        return str;
    }
    bool blocking() const
    {
        return this->_blocking;
    }
    void set_blocking(bool blocking)
    {
        socket().set_blocking(blocking);
        this->_blocking = blocking;
    }
    bool can_connect()
    {
        return (this->type() == SInfo::TCP_CLIENT);
    }
    bool can_bind()
    {
        return (this->type() == SInfo::UDP || this->type() == SInfo::TCP_SERVER);
    }
    bool can_send()
    {
        return (this->type() == SInfo::TCP_CLIENT);
    }
    bool can_recv()
    {
        return (this->type() == SInfo::TCP_CLIENT);
    }
    bool can_sendto()
    {
        return (this->type() == SInfo::UDP);
    }
    bool can_recvfrom()
    {
        return (this->type() == SInfo::UDP);
    }
    bool can_listen()
    {
        return (this->type() == SInfo::TCP_SERVER);
    }
    bool can_accept()
    {
        return (this->type() == SInfo::TCP_SERVER);
    }
private:
    const int _id;
    Socket *_sock;
    const SInfo::SocketType _type;
    bool _blocking;
    int _dataLen;
    int _maxRecvLen;
    int _repeatBufferFill;
    int _receivedTotal;
    Thread *_receiverThread;
    uint8_t *_receiveBuffer;
    osThreadId _senderThreadId;
    osThreadId _receiverThreadId;
    int *_packetSizes;
    bool _available;
    bool _check_pattern;

    SInfo();
};

static std::vector<SInfo *> m_sockets;

static enum {
    PRINT_DISABLED,
    PRINT_STRING,
    PRINT_HEX
} printing_mode = PRINT_STRING;
static int printing_col_width = 20;

static int cmd_socket(int argc, char *argv[]);
static void print_data(const uint8_t *buf, int len);
static void print_data_as_string(const uint8_t *buf, int len, int col_width);
static void print_data_as_hex(const uint8_t *buf, int len, int col_width);

/** Generate RFC 864 example pattern.
 *
 * Pattern is 72 chraracter lines of the ASCII printing characters ending with "\r\n".
 * There are 95 printing characters in the ASCII character set.
 * Example: `nc echo.mbedcloudtesting.com 19 | dd bs=1 count=222`
 *  !"#$%&'()*+,-./0123456789:;<=>?@ABCDEFGHIJKLMNOPQRSTUVWXYZ[\]^_`abcdefg
 * !"#$%&'()*+,-./0123456789:;<=>?@ABCDEFGHIJKLMNOPQRSTUVWXYZ[\]^_`abcdefgh
 * "#$%&'()*+,-./0123456789:;<=>?@ABCDEFGHIJKLMNOPQRSTUVWXYZ[\]^_`abcdefghi
 *
 * NOTE: Pattern starts with space, not !
 *
 * \param offset Start pattern from offset
 * \param len Length of pattern to generate.
 */
static void generate_RFC_864_pattern(size_t offset, uint8_t *buf,  size_t len)
{
    while (len--) {
        if (offset % 74 == 72) {
            *buf++ = '\r';
        } else if (offset % 74 == 73) {
            *buf++ = '\n';
        } else {
            *buf++ = ' ' + (offset % 74 + offset / 74) % 95 ;
        }
        offset++;
    }
}

bool SInfo::check_pattern(void *buffer, size_t len)
{
    if (!_check_pattern) {
        return true;
    }
    void *buf = malloc(len);
    if (!buf) {
        return false;
    }
    size_t offset = _receivedTotal;
    generate_RFC_864_pattern(offset, (uint8_t *)buf, len);
    bool match = memcmp(buf, buffer, len) == 0;
    if (!match) {
        cmd_printf("Pattern check failed\r\nWAS:%.*s\r\nREF:%.*s\r\n", len, (char *)buffer, len, (char *)buf);
    }
    free(buf);
    return match;
}

static void sigio_handler(SInfo *info)
{
    if (info->getReceiverThreadId()) {
        osSignalSet(info->getReceiverThreadId(), SIGNAL_SIGIO);
    }
    if (info->getSenderThreadId()) {
        osSignalSet(info->getSenderThreadId(), SIGNAL_SIGIO);
    }
}

void cmd_socket_init(void)
{
    cmd_add("socket", cmd_socket, "socket", MAN_SOCKET);
}

int handle_nsapi_error(const char *function, nsapi_error_t ret)
{
    if (ret != NSAPI_ERROR_OK) {
        cmd_printf("%s returned: %d -> %s\r\n", function, ret, networkstack_error_to_str(ret));
        return CMDLINE_RETCODE_FAIL;
    }
    return CMDLINE_RETCODE_SUCCESS;
}

int handle_nsapi_size_or_error(const char *function, nsapi_size_or_error_t ret)
{
    if (ret < 0) {
        return handle_nsapi_error(function, ret);
    } else {
        cmd_printf("%s returned: %d\r\n", function, ret);
    }
    return CMDLINE_RETCODE_SUCCESS;
}

static SInfo *get_sinfo(int id)
{

    for (std::vector<SInfo *>::iterator it = m_sockets.begin(); it != m_sockets.end(); it++) {
        if ((*it)->id() == id) {
            return *it;
        }
    }
    return NULL;
}

static int del_sinfo(SInfo *info)
{
    for (std::vector<SInfo *>::iterator it = m_sockets.begin(); it != m_sockets.end(); it++) {
        if ((*it) == info) {
            delete info;
            m_sockets.erase(it);
            return CMDLINE_RETCODE_SUCCESS;
        }
    }
    return CMDLINE_RETCODE_FAIL;
}

static int cmd_socket_new(int argc, char *argv[])
{
    const char *s;
    SInfo *info;

    if (cmd_parameter_last(argc, argv)) {
        s = cmd_parameter_last(argc, argv);
        if (strcmp(s, "UDPSocket") == 0) {
            tr_debug("Creating a new UDPSocket");
            info = new SInfo(new UDPSocket);
        } else if (strcmp(s, "TCPSocket") == 0) {
            tr_debug("Creating a new TCPSocket");
            info = new SInfo(new TCPSocket);
        } else if (strcmp(s, "TCPServer") == 0) {
            tr_debug("Creating a new TCPServer");
            info = new SInfo(new TCPServer);
        } else {
            cmd_printf("unsupported protocol: %s\r\n", s);
            return CMDLINE_RETCODE_INVALID_PARAMETERS;
        }
    } else {
        cmd_printf("Must specify socket type\r\n");
        return CMDLINE_RETCODE_INVALID_PARAMETERS;
    }
    // Note, this cannot fail. We either succeed or "new" woud call exit(1) in failure.
    // We did not ask no_throw version of it.
    cmd_printf("new socket. sid: %d\r\n", info->id());
    m_sockets.push_back(info);
    return CMDLINE_RETCODE_SUCCESS;
}

static void udp_receiver_thread(SInfo *info)
{
    SocketAddress addr;
    int i = 0, received = 0;
    int n = info->getRepeatBufferFill();
    int *packetSizes = info->getPacketSizeArray();
    nsapi_size_or_error_t ret = 0;

    info->setReceiverThreadId(Thread::gettid());

    while (i < n) {
        ret = static_cast<UDPSocket &>(info->socket()).recvfrom(&addr, info->getReceiveBuffer() + received, info->getDataCount() - received);
        if (ret > 0) {
            if (!info->check_pattern(info->getReceiveBuffer() + received, ret)) {
                return;
            }
            received += ret;
            packetSizes[i % PACKET_SIZE_ARRAY_LEN] = ret;
            i++;
            info->setRecvTotal(info->getRecvTotal() + ret);
        } else if (ret == NSAPI_ERROR_WOULD_BLOCK) {
            Thread::signal_wait(SIGNAL_SIGIO);
        } else {
            handle_nsapi_size_or_error("Thread: UDPSocket::recvfrom()", ret);
            return;
        }
    }
}

static nsapi_size_or_error_t start_udp_receiver_thread(SInfo *info, int argc, char *argv[])
{
    int32_t max_size;
    int32_t n = 1;

    if (!cmd_parameter_int(argc, argv, "--max_data_len", &max_size)) {
        cmd_printf("Need data max data size\r\n");
        return CMDLINE_RETCODE_INVALID_PARAMETERS;
    }
    if (cmd_parameter_index(argc, argv, "--packets") > 0) {
        if (!cmd_parameter_int(argc, argv, "--packets", &n)) {
            cmd_printf("Need number of packets\r\n");
            return CMDLINE_RETCODE_INVALID_PARAMETERS;
        }
    }
    uint8_t *dataIn = (uint8_t *)malloc(max_size + 1);
    if (!dataIn) {
        cmd_printf("malloc() failed\r\n");
        return CMDLINE_RETCODE_FAIL;
    }
    int *packetSizes = new (nothrow) int[PACKET_SIZE_ARRAY_LEN];
    if (!packetSizes) {
        cmd_printf("Allocation failed\r\n");
        return CMDLINE_RETCODE_FAIL;
    }
    for (int i = 0; i < PACKET_SIZE_ARRAY_LEN; i++) {
        packetSizes[i] = 0;
    }
    memset(dataIn, 0x00, max_size + 1);
    info->setReceiveBuffer(dataIn);
    info->setDataCount(max_size);
    info->setRepeatBufferFill(n);
    info->setPacketSizeArray(packetSizes);
    info->setReceiverThread(new Thread());
    info->getReceiverThread()->start(callback(udp_receiver_thread, info));
    return CMDLINE_RETCODE_SUCCESS;
}

static nsapi_size_or_error_t udp_sendto_command_handler(SInfo *info, int argc, char *argv[])
{
    char *host;
    int32_t port;

    if (!cmd_parameter_val(argc, argv, "sendto", &host)) {
        cmd_printf("Need host name\r\n");
        return CMDLINE_RETCODE_INVALID_PARAMETERS;
    }
    if (!cmd_parameter_int(argc, argv, host, &port)) {
        cmd_printf("Need port number\r\n");
        return CMDLINE_RETCODE_INVALID_PARAMETERS;
    }
    // Replace NULL-strings with NULL
    host = strcmp(host, "NULL") ? host : NULL;

    int32_t len;
    void *data;
    if (cmd_parameter_int(argc, argv, "--data_len", &len)) {
        data = malloc(len);
        if (!data) {
            cmd_printf("Failed to allocate memory\r\n");
            return CMDLINE_RETCODE_FAIL;
        }
    } else {
        // Replace NULL-strings with NULL
        if (strcmp(argv[5], "NULL") == 0) {
            data = NULL;
            len = 0;
        } else {
            data = argv[5];
            len = strlen(argv[5]);
        }
    }

    nsapi_size_or_error_t ret = static_cast<UDPSocket &>(info->socket()).sendto(host, port, data, len);
    if (ret > 0) {
        cmd_printf("sent: %d bytes\r\n", ret);
    }
    if (data != argv[5]) {
        free(data);
    }

    return handle_nsapi_size_or_error("UDPSocket::sendto()", ret);
}

static nsapi_size_or_error_t udp_recvfrom_command_handler(SInfo *info, int argc, char *argv[])
{
    SocketAddress addr;
    int32_t len;

    if (!cmd_parameter_int(argc, argv, "recvfrom", &len)) {
        cmd_printf("Need len\r\n");
        return CMDLINE_RETCODE_INVALID_PARAMETERS;
    }

    void *data = malloc(len);
    if (!data) {
        cmd_printf("malloc() failed\r\n");
        return CMDLINE_RETCODE_FAIL;
    }
    nsapi_size_or_error_t ret = static_cast<UDPSocket &>(info->socket()).recvfrom(&addr, data, len);
    if (ret > 0) {
        cmd_printf("UDPSocket::recvfrom, addr=%s port=%d\r\n", addr.get_ip_address(), addr.get_port());
        cmd_printf("received: %d bytes\r\n", ret);
        print_data((const uint8_t *)data, len);
        if (!info->check_pattern(data, len)) {
            ret = -1;
        }
        info->setRecvTotal(info->getRecvTotal() + ret);
    }
    free(data);
    return handle_nsapi_size_or_error("UDPSocket::recvfrom()", ret);
}

static void tcp_receiver_thread(SInfo *info)
{
    int i, received;
    int n = info->getRepeatBufferFill();
    int recv_len = info->getMaxRecvLen();
    int bufferSize = info->getDataCount();
    nsapi_size_or_error_t ret = 0;

    info->setReceiverThreadId(Thread::gettid());

    for (i = 0; i < n; i++) {
        received = 0;
        while (received < bufferSize) {
            ret = static_cast<TCPSocket &>(info->socket()).recv(info->getReceiveBuffer() + received, recv_len - received);
            if (ret > 0) {
                if (!info->check_pattern(info->getReceiveBuffer() + received, ret)) {
                    return;
                }
                received += ret;
                info->setRecvTotal(info->getRecvTotal() + ret);
            } else if (ret == NSAPI_ERROR_WOULD_BLOCK) {
                Thread::signal_wait(SIGNAL_SIGIO);
            } else {
                handle_nsapi_size_or_error("Thread: TCPSocket::recv()", ret);
                return;
            }
        }
    }
}

static nsapi_size_or_error_t start_tcp_receiver_thread(SInfo *info, int argc, char *argv[])
{
    int32_t len;
    int32_t recv_len;
    int32_t n = 1;

    if (!cmd_parameter_int(argc, argv, "--data_len", &len)) {
        cmd_printf("Need data len\r\n");
        return CMDLINE_RETCODE_INVALID_PARAMETERS;
    }
    recv_len = len;

    if (cmd_parameter_index(argc, argv, "--max_recv_len") > 0) {
        if (!cmd_parameter_int(argc, argv, "--max_recv_len", &recv_len)) {
            cmd_printf("Need max recv len\r\n");
            return CMDLINE_RETCODE_INVALID_PARAMETERS;
        }
    }

    if (cmd_parameter_index(argc, argv, "--repeat") > 0) {
        if (!cmd_parameter_int(argc, argv, "--repeat", &n)) {
            cmd_printf("Need repeat number\r\n");
            return CMDLINE_RETCODE_INVALID_PARAMETERS;
        }
    }

    uint8_t *dataIn = (uint8_t *)malloc(len + 1);
    if (!dataIn) {
        cmd_printf("malloc() failed\r\n");
        return CMDLINE_RETCODE_FAIL;
    }

    info->setReceiveBuffer(dataIn);
    info->setDataCount(len);
    info->setMaxRecvLen(recv_len);
    info->setRepeatBufferFill(n);
    info->setReceiverThread(new Thread());
    info->getReceiverThread()->start(callback(tcp_receiver_thread, info));
    return CMDLINE_RETCODE_SUCCESS;
}

static nsapi_size_or_error_t tcp_send_command_handler(SInfo *info, int argc, char *argv[])
{
    int32_t len;
    void *data;
    nsapi_size_or_error_t ret = 0;

    info->setSenderThreadId(Thread::gettid());

    if (cmd_parameter_int(argc, argv, "--data_len", &len)) {
        data = malloc(len);
        if (!data) {
            cmd_printf("Failed to allocate memory\r\n");
            return CMDLINE_RETCODE_FAIL;
        }
    } else {
        data = argv[3];
        len = strlen(argv[3]);
    }

    ret = static_cast<TCPSocket &>(info->socket()).send(data, len);

    if (ret > 0) {
        cmd_printf("sent: %d bytes\r\n", ret);
    }
    if (data != argv[3]) {
        free(data);
    }
    return handle_nsapi_size_or_error("TCPSocket::send()", ret);
}

static nsapi_size_or_error_t tcp_recv_command_handler(SInfo *info, int argc, char *argv[])
{
    int32_t len;

    if (!cmd_parameter_int(argc, argv, "recv", &len)) {
        cmd_printf("Need length\r\n");
        return CMDLINE_RETCODE_INVALID_PARAMETERS;
    }

    void *data = malloc(len);
    if (!data) {
        cmd_printf("malloc() failed\r\n");
        return CMDLINE_RETCODE_FAIL;
    }

    nsapi_size_or_error_t ret = static_cast<TCPSocket &>(info->socket()).recv(data, len);
    if (ret > 0) {
        cmd_printf("received: %d bytes\r\n", ret);
        print_data((const uint8_t *)data, ret);
        if (!info->check_pattern(data, ret)) {
            ret = -1;
        }
        info->setRecvTotal(info->getRecvTotal() + ret);
    }
    free(data);
    return handle_nsapi_size_or_error("TCPSocket::recv()", ret);
}

static nsapi_size_or_error_t recv_all(char *const rbuffer, const int expt_len, SInfo *const info)
{
    int rtotal, rbytes;
    char *rhead;

    rtotal = 0;
    rhead = rbuffer;

    while (rtotal < expt_len) {
        rbytes = info->tcp_socket()->recv(rhead, expt_len);
        if (rbytes <= 0) { // Connection closed abruptly
            rbuffer[rtotal] = '\0';
            return rbytes;
        }
        rtotal += rbytes;
        rhead = rbuffer + rtotal;
    }
    rbuffer[rtotal] = '\0';
    info->setRecvTotal(info->getRecvTotal() + rtotal);
    return rtotal;
}

static void bg_traffic_thread(SInfo *info)
{
    static const int data_len = 10;
    char sbuffer[data_len + 1] = "dummydata_";
    char rbuffer[data_len + 1];
    int scount, rtotal = 0;
    info->setSenderThreadId(Thread::gettid());

    for (;;) {
        if (!info->available()) {
            (void)handle_nsapi_size_or_error(__func__, rtotal);
            break;
        }
        sbuffer[data_len - 1] = 'A' + (rand() % 26);
        scount = info->tcp_socket()->send(sbuffer, data_len);
        rtotal = recv_all(rbuffer, data_len, info);

        if (scount != rtotal || (strcmp(sbuffer, rbuffer) != 0)) {
            info->setUnavailable();

            tr_err("Background received data does not match to sent data");
            tr_err("Background sent: \"%s\"", sbuffer);
            tr_err("Background received: \"%s\"", rbuffer);
        }
        wait_ms(10);
    }
}

static nsapi_size_or_error_t start_bg_traffic_thread(SInfo *info, int argc, char *argv[])
{
    info->setReceiverThread(new Thread());
    info->setAvailable();
    info->getReceiverThread()->start(callback(bg_traffic_thread, info));
    return CMDLINE_RETCODE_SUCCESS;
}

static void thread_clean_up(SInfo *info)
{
    if (info->getReceiverThread()) {
        delete info->getReceiverThread();
        info->setReceiverThread(NULL);
    }
    if (info->getReceiveBuffer()) {
        delete info->getReceiveBuffer();
        info->setReceiveBuffer(NULL);
    }
    if (info->getPacketSizeArray()) {
        delete[] info->getPacketSizeArray();
        info->setPacketSizeArray(NULL);
    }
}

static int cmd_socket(int argc, char *argv[])
{
    if (cmd_parameter_index(argc, argv, "new") == 1) {
        return cmd_socket_new(argc, argv);
    } else if (cmd_parameter_index(argc, argv, "print-mode") > 0) {
        if (cmd_parameter_index(argc, argv, "--string") > 0) {
            printing_mode = PRINT_STRING;
        } else if (cmd_parameter_index(argc, argv, "--hex") > 0) {
            printing_mode = PRINT_HEX;
        } else if (cmd_parameter_index(argc, argv, "--disabled") > 0) {
            printing_mode = PRINT_DISABLED;
        }
        int32_t parsed_col_width = 0;
        if (cmd_parameter_int(argc, argv, "--col-width", &parsed_col_width)) {
            if (parsed_col_width <= 0) {
                cmd_printf("Printing column width must be > 0");
                return CMDLINE_RETCODE_FAIL;
            }
            if (printing_mode == PRINT_HEX && parsed_col_width > 42) {
                cmd_printf("Maximum column width for hex data is 42 bytes");
                return CMDLINE_RETCODE_FAIL;
            }
            printing_col_width = (int)parsed_col_width;
        }
        // Allow print-mode to be used as a parameter to other commands
        if (cmd_parameter_index(argc, argv, "print-mode") == 1) {
            return CMDLINE_RETCODE_SUCCESS;
        }
    }

    // Rest of the commands require Socket
    SInfo *info = get_sinfo(strtol(argv[1], NULL, 10));
    if (!info) {
        cmd_printf("Invalid socket id %s\r\n", argv[1]);
        return CMDLINE_RETCODE_FAIL;
    }

    bool enable_pattern_check;
    if (cmd_parameter_bool(argc, argv, "set_RFC_864_pattern_check", &enable_pattern_check)) {
        info->set_pattern_check(enable_pattern_check);
        return CMDLINE_RETCODE_SUCCESS;
    }

    // Helper macro for checking the which command was given
#define COMMAND_IS(cmd) (cmd_parameter_index(argc, argv, cmd) == 2)

    /*
     * Generic Socket commands:
     * delete, open, close, bind, set_blocking, set_timeout
     */

    if (COMMAND_IS("delete")) {
        return del_sinfo(info);

    } else if (COMMAND_IS("open")) {
        NetworkInterface *interface;

        interface = get_interface(); // get default interface

        if (!interface) {
            cmd_printf("Invalid interface\r\n");
            return CMDLINE_RETCODE_FAIL;
        }

        switch (info->type()) {
            case SInfo::TCP_CLIENT:
                return handle_nsapi_error("Socket::open()", info->tcp_socket()->open(interface));
            case SInfo::UDP:
                return handle_nsapi_error("Socket::open()", info->udp_socket()->open(interface));
            case SInfo::TCP_SERVER:
                return handle_nsapi_error("Socket::open()", info->tcp_server()->open(interface));
        }

    } else if (COMMAND_IS("close")) {
        return handle_nsapi_error("Socket::close()", info->socket().close());

    } else if (COMMAND_IS("bind")) {
        int32_t port = 0;
        char *addr;

        if (!cmd_parameter_int(argc, argv, "port", &port) && !cmd_parameter_int(argc, argv, "bind", &port) && port <= 0 && port > 65535) {
            printf("Missing or invalid port number\n");
            return CMDLINE_RETCODE_INVALID_PARAMETERS;
        }

        if (cmd_parameter_val(argc, argv, "addr", &addr)) {
            // Replace NULL-strings with NULL
            addr = strcmp(addr, "NULL") ? addr : NULL;
            cmd_printf("Socket::bind(%s, %" PRId32 ")\r\n", addr, port);
            SocketAddress tmp(addr, port);
            return handle_nsapi_error("Socket::bind(addr, port)", info->socket().bind(tmp));
        } else {
            cmd_printf("Socket::bind(%" PRId32 ")\r\n", port);
            SocketAddress tmp(NULL, port);
            return handle_nsapi_error("Socket::bind(port)", info->socket().bind(tmp));
        }

    } else if (COMMAND_IS("set_blocking")) {
        bool val;
        if (!cmd_parameter_bool(argc, argv, "set_blocking", &val)) {
            cmd_printf("Need boolean value");
            return CMDLINE_RETCODE_INVALID_PARAMETERS;
        }
        info->set_blocking(val);
        return CMDLINE_RETCODE_SUCCESS;

    } else if (COMMAND_IS("set_timeout")) {
        int32_t ms;
        if (!cmd_parameter_int(argc, argv, "set_timeout", &ms)) {
            cmd_printf("Need timeout value");
            return CMDLINE_RETCODE_INVALID_PARAMETERS;
        }
        if (ms == -1) {
            info->set_blocking(true);
        } else {
            info->set_blocking(false);
        }

        info->socket().set_timeout(ms);
        return CMDLINE_RETCODE_SUCCESS;

    } else if (COMMAND_IS("register_sigio_cb")) {
        info->socket().sigio(callback(sigio_handler, info));
        return CMDLINE_RETCODE_SUCCESS;
    }


    /*
     * Commands related to UDPSocket:
     * sendto, recvfrom
     */
    if ((COMMAND_IS("sendto") || COMMAND_IS("recvfrom") || COMMAND_IS("start_udp_receiver_thread")
            || COMMAND_IS("last_data_received")) && info->type() != SInfo::UDP) {
        cmd_printf("Not UDPSocket\r\n");
        return CMDLINE_RETCODE_FAIL;
    }

    if (COMMAND_IS("sendto")) {
        return udp_sendto_command_handler(info, argc, argv);
    } else if (COMMAND_IS("recvfrom")) {
        return udp_recvfrom_command_handler(info, argc, argv);
    } else if (COMMAND_IS("start_udp_receiver_thread")) {
        return start_udp_receiver_thread(info, argc, argv);
    } else if (COMMAND_IS("last_data_received")) {
        print_data((const uint8_t *)info->getReceiveBuffer(), info->getDataCount());
        if (info->getPacketSizeArray()) {
            int *packetSizes = info->getPacketSizeArray();
            cmd_printf("packet_sizes: ");
            for (int i = 0; i < PACKET_SIZE_ARRAY_LEN; i++) {
                cmd_printf("%d ", packetSizes[i]);
            }
            cmd_printf("\r\n");
        }
        if (info->getReceiverThread()) {
            info->getReceiverThread()->terminate();
        }
        thread_clean_up(info);

        return handle_nsapi_error("UDPSocket::last_data_received()", NSAPI_ERROR_OK);
    }

    /*
     * Commands related to TCPSocket
     * connect, send, recv
     */
    if ((COMMAND_IS("connect") || COMMAND_IS("recv")
            || COMMAND_IS("start_tcp_receiver_thread") || COMMAND_IS("join_tcp_receiver_thread")
            || COMMAND_IS("start_bg_traffic_thread") || COMMAND_IS("join_bg_traffic_thread")
            || COMMAND_IS("setsockopt_keepalive") || COMMAND_IS("getsockopt_keepalive"))
            && info->type() != SInfo::TCP_CLIENT) {
        cmd_printf("Not TCPSocket\r\n");
        return CMDLINE_RETCODE_FAIL;
    }

    if (COMMAND_IS("connect")) {
        char *host;
        int32_t port;

        if (!cmd_parameter_val(argc, argv, "connect", &host)) {
            cmd_printf("Need host name\r\n");
            return CMDLINE_RETCODE_INVALID_PARAMETERS;
        }
        if (!cmd_parameter_int(argc, argv, host, &port)) {
            cmd_printf("Need port number\r\n");
            return CMDLINE_RETCODE_INVALID_PARAMETERS;
        }
        if (strcmp(host, "NULL") == 0) {
            host = NULL;
        }

        cmd_printf("Host name: %s port: %" PRId32 "\r\n", host, port);
        return handle_nsapi_error("TCPSocket::connect()", static_cast<TCPSocket &>(info->socket()).connect(host, port));

    } else if (COMMAND_IS("send")) {
        return tcp_send_command_handler(info, argc, argv);

    } else if (COMMAND_IS("recv")) {
        return tcp_recv_command_handler(info, argc, argv);

    } else if (COMMAND_IS("start_tcp_receiver_thread")) {
        return start_tcp_receiver_thread(info, argc, argv);

    } else if (COMMAND_IS("join_tcp_receiver_thread")) {
        info->getReceiverThread()->join();
        print_data((const uint8_t *)info->getReceiveBuffer(), info->getDataCount());
        cmd_printf("received: %d bytes\r\n", info->getRecvTotal());

        thread_clean_up(info);

        return CMDLINE_RETCODE_SUCCESS;

    } else if (COMMAND_IS("start_bg_traffic_thread")) {
        return start_bg_traffic_thread(info, argc, argv);

    } else if (COMMAND_IS("join_bg_traffic_thread")) {
        int bg_thread_success = CMDLINE_RETCODE_SUCCESS;

        if (!info->available()) { // Tells that background thread stumbled to an issue and stopped prematurely
            bg_thread_success = CMDLINE_RETCODE_FAIL;
        }

        info->setUnavailable();
        info->getReceiverThread()->join();
        thread_clean_up(info);

        return bg_thread_success;
    } else if (COMMAND_IS("setsockopt_keepalive")) {
        int32_t seconds;
        nsapi_error_t ret;
        if (!cmd_parameter_int(argc, argv, "setsockopt_keepalive", &seconds)) {
            cmd_printf("Need keep-alive value(0-7200seconds)");
            return CMDLINE_RETCODE_INVALID_PARAMETERS;
        }

        ret = info->socket().setsockopt(NSAPI_SOCKET, NSAPI_KEEPALIVE, &seconds, sizeof(seconds));

        return handle_nsapi_error("TCPSocket::setsockopt()", ret);
    }  else if (COMMAND_IS("getsockopt_keepalive")) {
        int32_t optval;
        unsigned optlen = sizeof(optval);
        nsapi_error_t ret;

        ret = info->socket().getsockopt(NSAPI_SOCKET, NSAPI_KEEPALIVE, &optval, &optlen);

        if (optlen != sizeof(int)) {
            return CMDLINE_RETCODE_FAIL;
        }
        if (ret < 0) {
            return handle_nsapi_error("TCPSocket::getsockopt()", ret);
        }
        return handle_nsapi_size_or_error("TCPSocket::getsockopt()", optval);
    }

    /*
     * Commands for TCPServer and TCPSocket
     * listen, accept
     */
    if (COMMAND_IS("listen")) {
        int32_t backlog;
        if (cmd_parameter_int(argc, argv, "listen", &backlog)) {
            return handle_nsapi_error("Socket::listen()", info->socket().listen(backlog));
        } else {
            return handle_nsapi_error("Socket::listen()", info->socket().listen());
        }

    } else if (COMMAND_IS("accept")) {
        nsapi_error_t ret;
        if (info->type() != SInfo::TCP_SERVER) {
            Socket *new_sock = info->socket().accept(&ret);
            if (ret == NSAPI_ERROR_OK) {
                SInfo *new_info = new SInfo(new_sock);
                m_sockets.push_back(new_info);
                cmd_printf("Socket::accept() new socket sid: %d\r\n", new_info->id());
            }
            return handle_nsapi_error("Socket::accept()", ret);
        } else { // Old TCPServer API
            int32_t id;
            SocketAddress addr;

            if (!cmd_parameter_int(argc, argv, "accept", &id)) {
                cmd_printf("Need new socket id\r\n");
                return CMDLINE_RETCODE_INVALID_PARAMETERS;
            }
            SInfo *new_info = get_sinfo(id);
            if (!new_info) {
                cmd_printf("Invalid socket id\r\n");
                return CMDLINE_RETCODE_FAIL;
            }
            TCPSocket *new_sock = static_cast<TCPSocket*>(&new_info->socket());
            nsapi_error_t ret = static_cast<TCPServer&>(info->socket()).accept(new_sock, &addr);
            if (ret == NSAPI_ERROR_OK) {
                cmd_printf("TCPServer::accept() new socket sid: %d connection from %s port %d\r\n",
                        new_info->id(), addr.get_ip_address(), addr.get_port());
            }
            return handle_nsapi_error("TCPServer::accept()", ret);
        }
    }
    return CMDLINE_RETCODE_INVALID_PARAMETERS;
}

void print_data(const uint8_t *buf, int len)
{
    switch (printing_mode) {
        case PRINT_STRING:
            print_data_as_string(buf, len, printing_col_width);
            break;
        case PRINT_HEX:
            print_data_as_hex(buf, len, printing_col_width);
            break;
        case PRINT_DISABLED:
            break;
        default:
            assert(0);
    }
}

void print_data_as_string(const uint8_t *buf, int len, int col_width)
{
    int printable_bytes;
    for (printable_bytes = 0; printable_bytes < len; printable_bytes++) {
        if (!isprint(buf[printable_bytes])) {
            break;
        }
    }

    cmd_mutex_lock();
    cmd_printf("string data, printing %d bytes:\r\n", len);
    for (int i = 0; i < printable_bytes; i += col_width) {
        if (printable_bytes - i > col_width) {
            cmd_printf("%04d:  %.*s\r\n", i, col_width, buf + i);
        } else {
            cmd_printf("%04d:  %.*s\r\n", i, printable_bytes - i, buf + i);
        }
    }
    cmd_printf("Printed %d bytes\r\n", printable_bytes);

    if (len != printable_bytes) {
        cmd_printf("Error! Couldn't print all data. "
                   "Unprintable character: 0x%02x found at index: %d\r\n",
                   buf[printable_bytes], printable_bytes);
    }
    cmd_mutex_unlock();
}

void print_data_as_hex(const uint8_t *buf, int len, int col_width)
{
    cmd_mutex_lock();
    cmd_printf("hex data, printing %d bytes:\r\n", len);
    for (int i = 0; i < len; i += col_width) {
        if (len - i > col_width) {
            cmd_printf("%04d:  %s:\r\n", i, print_array(buf + i, col_width));
        } else {
            cmd_printf("%04d:  %s\r\n", i, print_array(buf + i, len - i));
        }
    }
    cmd_printf("Printed %d bytes\r\n", len);
    cmd_mutex_unlock();
}
