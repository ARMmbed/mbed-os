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
#include "TLSSocket.h"
#include "NetworkInterface.h"
#include "SocketAddress.h"
#include "Queue.h"

#include <vector>
#include <cctype>
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
#define CERT_BUFFER_SIZE 1648


#define MAN_SOCKET          "\r\nSOCKET API\r\n"\
                            "\r\n"\
                            "socket <operation> [options]\r\n\r\n"\
                            " new <type>\r\n" \
                            "   type: UDPSocket|TCPSocket|TCPServer|TLSSocket [--cert_file <file>|--cert_default]\r\n"\
                            "   return socket id\r\n"\
                            " <id> delete\r\n"\
                            "   remote the space allocated for Socket\r\n"\
                            " <id> open [--if <interface>] \r\n"\
                            "   interface (or use default interface) \r\n"\
                            " <id> close\r\n"\
                            " <id> bind [port] <port> [addr <addr>]\r\n"\
                            " <id> set_blocking <bool>\r\n"\
                            " <id> set_timeout <ms>\r\n"\
                            " <id> register_sigio_cb\r\n"\
                            " <id> set_RFC_864_pattern_check <true|false>\r\n"\
                            " <id> set_root_ca_cert --cert_url <url>|--cert_file <file>|--cert_default\r\n"\
                            " <id> sendto <addr> <port> (\"msg\" | --data_len <len>)\r\n"\
                            "   \"msg\" Send packet with defined string content\r\n"\
                            "    --data_len Send packet with random content with size <len>\r\n"\
                            " <id> recvfrom <len>\r\n"\
                            " <id> start_udp_receiver_thread --max_data_len <len> [--packets <N>]\r\n"\
                            "    --max_data_len Size of input buffer to fill up\r\n"\
                            "    --packets Receive N number of packets, default 1\r\n"\
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
                            " <id> listen [backlog]\r\n"\
                            " <id> accept\r\n" \
                            "   accept new connection and returns new socket ID\r\n" \
                            "\r\nFor TCPServer\r\n"\
                            " <id> accept <new_id>\r\n"\
                            "   accept new connection into <new_id> socket. Requires <new_id> to be pre-allocated.\r\n"\
                            "\r\nOther options\r\n"\
                            " print-mode [--string|--hex|--disabled] [--col-width <width>]"



const char *cert = \
                   "-----BEGIN CERTIFICATE-----\n" \
                   "MIIEkjCCA3qgAwIBAgIQCgFBQgAAAVOFc2oLheynCDANBgkqhkiG9w0BAQsFADA/\n" \
                   "MSQwIgYDVQQKExtEaWdpdGFsIFNpZ25hdHVyZSBUcnVzdCBDby4xFzAVBgNVBAMT\n" \
                   "DkRTVCBSb290IENBIFgzMB4XDTE2MDMxNzE2NDA0NloXDTIxMDMxNzE2NDA0Nlow\n" \
                   "SjELMAkGA1UEBhMCVVMxFjAUBgNVBAoTDUxldCdzIEVuY3J5cHQxIzAhBgNVBAMT\n" \
                   "GkxldCdzIEVuY3J5cHQgQXV0aG9yaXR5IFgzMIIBIjANBgkqhkiG9w0BAQEFAAOC\n" \
                   "AQ8AMIIBCgKCAQEAnNMM8FrlLke3cl03g7NoYzDq1zUmGSXhvb418XCSL7e4S0EF\n" \
                   "q6meNQhY7LEqxGiHC6PjdeTm86dicbp5gWAf15Gan/PQeGdxyGkOlZHP/uaZ6WA8\n" \
                   "SMx+yk13EiSdRxta67nsHjcAHJyse6cF6s5K671B5TaYucv9bTyWaN8jKkKQDIZ0\n" \
                   "Z8h/pZq4UmEUEz9l6YKHy9v6Dlb2honzhT+Xhq+w3Brvaw2VFn3EK6BlspkENnWA\n" \
                   "a6xK8xuQSXgvopZPKiAlKQTGdMDQMc2PMTiVFrqoM7hD8bEfwzB/onkxEz0tNvjj\n" \
                   "/PIzark5McWvxI0NHWQWM6r6hCm21AvA2H3DkwIDAQABo4IBfTCCAXkwEgYDVR0T\n" \
                   "AQH/BAgwBgEB/wIBADAOBgNVHQ8BAf8EBAMCAYYwfwYIKwYBBQUHAQEEczBxMDIG\n" \
                   "CCsGAQUFBzABhiZodHRwOi8vaXNyZy50cnVzdGlkLm9jc3AuaWRlbnRydXN0LmNv\n" \
                   "bTA7BggrBgEFBQcwAoYvaHR0cDovL2FwcHMuaWRlbnRydXN0LmNvbS9yb290cy9k\n" \
                   "c3Ryb290Y2F4My5wN2MwHwYDVR0jBBgwFoAUxKexpHsscfrb4UuQdf/EFWCFiRAw\n" \
                   "VAYDVR0gBE0wSzAIBgZngQwBAgEwPwYLKwYBBAGC3xMBAQEwMDAuBggrBgEFBQcC\n" \
                   "ARYiaHR0cDovL2Nwcy5yb290LXgxLmxldHNlbmNyeXB0Lm9yZzA8BgNVHR8ENTAz\n" \
                   "MDGgL6AthitodHRwOi8vY3JsLmlkZW50cnVzdC5jb20vRFNUUk9PVENBWDNDUkwu\n" \
                   "Y3JsMB0GA1UdDgQWBBSoSmpjBH3duubRObemRWXv86jsoTANBgkqhkiG9w0BAQsF\n" \
                   "AAOCAQEA3TPXEfNjWDjdGBX7CVW+dla5cEilaUcne8IkCJLxWh9KEik3JHRRHGJo\n" \
                   "uM2VcGfl96S8TihRzZvoroed6ti6WqEBmtzw3Wodatg+VyOeph4EYpr/1wXKtx8/\n" \
                   "wApIvJSwtmVi4MFU5aMqrSDE6ea73Mj2tcMyo5jMd6jmeWUHK8so/joWUoHOUgwu\n" \
                   "X4Po1QYz+3dszkDqMp4fklxBwXRsW10KXzPMTZ+sOPAveyxindmjkW8lGy+QsRlG\n" \
                   "PfZ+G6Z6h7mjem0Y+iWlkYcV4PIWL1iwBi8saCbGS5jN2p8M+X+Q7UNKEkROb3N6\n" \
                   "KOqkqm57TH2H3eDJAkSnh6/DNFu0Qg==\n" \
                   "-----END CERTIFICATE-----\n";

class SInfo {
public:
    enum SocketType {
        IP,
        TCP_SERVER,
        OTHER,
#if defined(MBEDTLS_SSL_CLI_C)
        TLS
#endif
    };
    SInfo(InternetSocket *sock, bool delete_on_exit = true):
        _id(id_count++),
        _sock(sock),
        _type(SInfo::IP),
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
        _check_pattern(false),
        _delete_on_exit(delete_on_exit)
    {
        MBED_ASSERT(sock);
    }
    SInfo(TCPServer *sock, bool delete_on_exit = true):
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
        _check_pattern(false),
        _delete_on_exit(delete_on_exit)
    {
        MBED_ASSERT(sock);
    }
    SInfo(Socket *sock, bool delete_on_exit = true):
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
        _check_pattern(false),
        _delete_on_exit(delete_on_exit)
    {
        MBED_ASSERT(sock);
    }
#if defined(MBEDTLS_SSL_CLI_C)
    SInfo(TLSSocket *sock, bool delete_on_exit = true):
        _id(id_count++),
        _sock(sock),
        _type(SInfo::TLS),
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
        _check_pattern(false),
        _delete_on_exit(delete_on_exit)
    {
        MBED_ASSERT(sock);
    }
#endif
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
        if (_delete_on_exit) {
            delete this->_sock;
        }
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
    InternetSocket *internetsocket()
    {
        return this->_type == SInfo::IP ? static_cast<InternetSocket *>(this->_sock) : NULL;
    }
    TCPServer *tcp_server()
    {
        return this->_type == SInfo::TCP_SERVER ? static_cast<TCPServer *>(this->_sock) : NULL;
    }
#if defined(MBEDTLS_SSL_CLI_C)
    TLSSocket *tls_socket()
    {
        return this->_type == SInfo::TLS        ? static_cast<TLSSocket *>(this->_sock) : NULL;
    }
#endif
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
            case SInfo::IP:
                str = "InternetSocket";
                break;
            case SInfo::TCP_SERVER:
                str = "TCPServer";
                break;
            case SInfo::OTHER:
                str = "Socket";
                break;
#if defined(MBEDTLS_SSL_CLI_C)
            case SInfo::TLS:
                str = "TLSSocket";
                break;
#endif
            default:
                MBED_ASSERT(0);
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
private:
    static int id_count;
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
    bool _delete_on_exit;

    SInfo();
};
int SInfo::id_count = 0;

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
static void generate_RFC_864_pattern(size_t offset, uint8_t *buf,  size_t len, bool is_xinetd)
{
    const int row_size = 74; // Number of chars in single row
    const int row_count = 95; // Number of rows in pattern after which pattern start from beginning
    const int chars_scope = is_xinetd ? 93 : 95; // Number of chars from ASCII table used in pattern
    const char first_char = is_xinetd ? '!' : ' '; // First char from ASCII table used in pattern
    while (len--) {
        if (offset % row_size == (row_size - 2)) {
            *buf++ = '\r';
        } else if (offset % row_size == (row_size - 1)) {
            *buf++ = '\n';
        } else {
            *buf++ = first_char + (offset % row_size + ((offset / row_size) % row_count)) % chars_scope;
        }
        offset++;
    }
}

static int get_cert_from_file(const char *filename, char **cert)
{
    int filedesc = open(filename, O_RDONLY);
    if (filedesc < 0) {
        cmd_printf("Cannot open file: %s\r\n", filename);
        return CMDLINE_RETCODE_FAIL;
    }

    if (read(filedesc, *cert, CERT_BUFFER_SIZE) != CERT_BUFFER_SIZE) {
        cmd_printf("Cannot read from file %s\r\n", filename);
        return CMDLINE_RETCODE_FAIL;
    }

    return CMDLINE_RETCODE_SUCCESS;
}

bool SInfo::check_pattern(void *buffer, size_t len)
{
    static bool is_xinetd = false;
    if (!_check_pattern) {
        return true;
    }
    void *buf = malloc(len);
    if (!buf) {
        return false;
    }

    size_t offset = _receivedTotal;

    if (offset == 0) {
        is_xinetd = ((uint8_t *)buffer)[0] == '!';
    }

    generate_RFC_864_pattern(offset, (uint8_t *)buf, len, is_xinetd);
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
    cmd_alias_add("socket help", "socket -h");
    cmd_alias_add("socket --help", "socket -h");
    cmd_alias_add("ping server start", "socket echo-server new start");
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

#if defined(MBEDTLS_SSL_CLI_C)
static int tls_set_cert(int argc, char *argv[], SInfo *info)
{
    static char read_cert[CERT_BUFFER_SIZE];
    char *ptr_cert = NULL;
    char *src = NULL;
    if (cmd_parameter_val(argc, argv, "--cert_file", &src)) {
        tr_debug("Root ca certificate read from file: %s", src);
        ptr_cert = read_cert;
        if (get_cert_from_file(src, &ptr_cert) == CMDLINE_RETCODE_FAIL) {
            cmd_printf("Cannot read from url: %s\r\n", src);
            return CMDLINE_RETCODE_INVALID_PARAMETERS;
        }
    } else if (cmd_parameter_index(argc, argv, "--cert_default") != -1) {
        cmd_printf("Using default certificate\r\n");
        ptr_cert = (char *)cert;
    } else  {
        cmd_printf("No cert specified. Use set_root_ca_cert to set it.\r\n");
        // Do not return error, allow the certificate not to be set.
        return CMDLINE_RETCODE_SUCCESS;
    }

    int ret = info->tls_socket()->set_root_ca_cert(ptr_cert);
    if (ret != NSAPI_ERROR_OK) {
        cmd_printf("Invalid root certificate\r\n");
        return CMDLINE_RETCODE_FAIL;
    }

    return CMDLINE_RETCODE_SUCCESS;
}
#endif

static int cmd_socket_new(int argc, char *argv[])
{
    const char *s;
    SInfo *info;
    nsapi_error_t ret;

    if (argc > 2) {
        s = argv[2];
        if (strcmp(s, "UDPSocket") == 0) {
            tr_debug("Creating a new UDPSocket");
            info = new SInfo(new UDPSocket);
        } else if (strcmp(s, "TCPSocket") == 0) {
            tr_debug("Creating a new TCPSocket");
            info = new SInfo(new TCPSocket);
        } else if (strcmp(s, "TCPServer") == 0) {
            tr_debug("Creating a new TCPServer");
            info = new SInfo(new TCPServer);
#if defined(MBEDTLS_SSL_CLI_C)
        } else if (strcmp(s, "TLSSocket") == 0) {
            tr_debug("Creating a new TLSSocket");
            info = new SInfo(new TLSSocket);
            ret = tls_set_cert(argc, argv, info);
            if (ret) {
                delete info;
                return ret;
            }
#endif
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

    info->setReceiverThreadId(ThisThread::get_id());

    while (i < n) {
        ret = info->socket().recvfrom(&addr, info->getReceiveBuffer() + received, info->getDataCount() - received);
        if (ret > 0) {
            if (!info->check_pattern(info->getReceiveBuffer() + received, ret)) {
                return;
            }
            received += ret;
            packetSizes[i % PACKET_SIZE_ARRAY_LEN] = ret;
            i++;
            info->setRecvTotal(info->getRecvTotal() + ret);
        } else if (ret == NSAPI_ERROR_WOULD_BLOCK) {
            ThisThread::flags_wait_any(SIGNAL_SIGIO);
        } else {
            handle_nsapi_size_or_error("Thread: Socket::recvfrom()", ret);
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
        free(dataIn);
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

    SocketAddress addr(NULL, port);
    nsapi_size_or_error_t ret = get_interface()->gethostbyname(host, &addr);
    if (ret) {
        return handle_nsapi_size_or_error("NetworkInterface::gethostbyname()", ret);
    }
    ret = info->socket().sendto(addr, data, len);
    if (ret > 0) {
        cmd_printf("sent: %d bytes\r\n", ret);
    }
    if (data != argv[5]) {
        free(data);
    }

    return handle_nsapi_size_or_error("Socket::sendto()", ret);
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
    nsapi_size_or_error_t ret = info->socket().recvfrom(&addr, data, len);
    if (ret > 0) {
        cmd_printf("Socket::recvfrom, addr=%s port=%d\r\n", addr.get_ip_address(), addr.get_port());
        cmd_printf("received: %d bytes\r\n", ret);
        print_data((const uint8_t *)data, len);
        if (!info->check_pattern(data, len)) {
            ret = -1;
        }
        info->setRecvTotal(info->getRecvTotal() + ret);
    }
    free(data);
    return handle_nsapi_size_or_error("Socket::recvfrom()", ret);
}

static void tcp_receiver_thread(SInfo *info)
{
    int i, received;
    int n = info->getRepeatBufferFill();
    int recv_len = info->getMaxRecvLen();
    int bufferSize = info->getDataCount();
    nsapi_size_or_error_t ret = 0;

    info->setReceiverThreadId(ThisThread::get_id());

    for (i = 0; i < n; i++) {
        received = 0;
        while (received < bufferSize) {
            ret = info->socket().recv(info->getReceiveBuffer() + received, recv_len - received);
            if (ret > 0) {
                if (!info->check_pattern(info->getReceiveBuffer() + received, ret)) {
                    return;
                }
                received += ret;
                info->setRecvTotal(info->getRecvTotal() + ret);
            } else if (ret == NSAPI_ERROR_WOULD_BLOCK) {
                ThisThread::flags_wait_all(SIGNAL_SIGIO);
            } else {
                handle_nsapi_size_or_error("Thread: Socket::recv()", ret);
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

    info->setSenderThreadId(ThisThread::get_id());

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

    ret = info->socket().send(data, len);

    if (ret > 0) {
        cmd_printf("sent: %d bytes\r\n", ret);
    }
    if (data != argv[3]) {
        free(data);
    }
    return handle_nsapi_size_or_error("Socket::send()", ret);
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

    nsapi_size_or_error_t ret = info->socket().recv(data, len);
    if (ret > 0) {
        cmd_printf("received: %d bytes\r\n", ret);
        print_data((const uint8_t *)data, ret);
        if (!info->check_pattern(data, ret)) {
            ret = -1;
        }
        info->setRecvTotal(info->getRecvTotal() + ret);
    }
    free(data);
    return handle_nsapi_size_or_error("Socket::recv()", ret);
}

static nsapi_size_or_error_t recv_all(char *const rbuffer, const int expt_len, SInfo *const info)
{
    int rtotal, rbytes;
    char *rhead;

    rtotal = 0;
    rhead = rbuffer;

    while (rtotal < expt_len) {
        rbytes = info->socket().recv(rhead, expt_len);
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
    info->setSenderThreadId(ThisThread::get_id());

    for (;;) {
        if (!info->available()) {
            (void)handle_nsapi_size_or_error(__func__, rtotal);
            break;
        }
        sbuffer[data_len - 1] = 'A' + (rand() % 26);
        scount = info->socket().send(sbuffer, data_len);
        rtotal = recv_all(rbuffer, data_len, info);

        if (scount != rtotal || (strcmp(sbuffer, rbuffer) != 0)) {
            info->setUnavailable();

            tr_err("Background received data does not match to sent data");
            tr_err("Background sent: \"%s\"", sbuffer);
            tr_err("Background received: \"%s\"", rbuffer);
        }
        ThisThread::sleep_for(10);
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
            case SInfo::IP:
                return handle_nsapi_error("Socket::open()", info->internetsocket()->open(interface));
            case SInfo::TCP_SERVER:
                return handle_nsapi_error("TCPServer::open()", info->tcp_server()->open(interface));
#if defined(MBEDTLS_SSL_CLI_C)
            case SInfo::TLS:
                return handle_nsapi_error("Socket::open()", info->tls_socket()->open(interface));
#endif
            default:
                cmd_printf("Not a IP socket\r\n");
                return CMDLINE_RETCODE_FAIL;
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

        return handle_nsapi_error("Socket::last_data_received()", NSAPI_ERROR_OK);
    }

    /*
     * Commands related to TCPSocket, TLSSocket
     * connect, send, recv
     */
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
        if (info->type() == SInfo::IP) {
            SocketAddress addr(NULL, port);
            nsapi_error_t ret = get_interface()->gethostbyname(host, &addr);
            if (ret) {
                return handle_nsapi_error("NetworkInterface::gethostbyname()", ret);
            }
            return handle_nsapi_error("Socket::connect()", info->socket().connect(addr));
#if defined(MBEDTLS_SSL_CLI_C)
        } else if (info->type() == SInfo::TLS) {
            return handle_nsapi_error("TLSSocket::connect()", static_cast<TLSSocket &>(info->socket()).connect(host, port));
#endif
        }

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

        return handle_nsapi_error("Socket::setsockopt()", ret);
    }  else if (COMMAND_IS("getsockopt_keepalive")) {
        int32_t optval;
        unsigned optlen = sizeof(optval);
        nsapi_error_t ret;

        ret = info->socket().getsockopt(NSAPI_SOCKET, NSAPI_KEEPALIVE, &optval, &optlen);

        if (optlen != sizeof(int)) {
            return CMDLINE_RETCODE_FAIL;
        }
        if (ret < 0) {
            return handle_nsapi_error("Socket::getsockopt()", ret);
        }
        return handle_nsapi_size_or_error("Socket::getsockopt()", optval);
    }

    /*
     * Commands for TCPServer
     * listen, accept
     */
    if (COMMAND_IS("listen")) {
        int32_t backlog;
        if (cmd_parameter_int(argc, argv, "listen", &backlog)) {
            return handle_nsapi_error("TCPServer::listen()", info->socket().listen(backlog));
        } else {
            return handle_nsapi_error("TCPServer::listen()", info->socket().listen());
        }

    } else if (COMMAND_IS("accept")) {
        nsapi_error_t ret;

        if (info->type() != SInfo::TCP_SERVER) {
            Socket *new_sock = info->socket().accept(&ret);
            if (ret == NSAPI_ERROR_OK) {
                SInfo *new_info = new SInfo(new_sock, false);
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
            TCPSocket *new_sock = static_cast<TCPSocket *>(&new_info->socket());
            nsapi_error_t ret = static_cast<TCPServer &>(info->socket()).accept(new_sock, &addr);
            if (ret == NSAPI_ERROR_OK) {
                cmd_printf("TCPServer::accept() new socket sid: %d connection from %s port %d\r\n",
                           new_info->id(), addr.get_ip_address(), addr.get_port());
            }
            return handle_nsapi_error("TCPServer::accept()", ret);
        }
    }


    /*
     * Commands for TLSSocket
     * set_root_ca_cert
     */
#if defined(MBEDTLS_SSL_CLI_C)
    if (COMMAND_IS("set_root_ca_cert")) {
        if (info->type() != SInfo::TLS) {
            cmd_printf("Not a TLS socket.\r\n");
            return CMDLINE_RETCODE_FAIL;
        }
        return handle_nsapi_error("TLSSocket::tls_set_cert", tls_set_cert(argc, argv, info));
    }
#endif

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
            MBED_ASSERT(0);
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
