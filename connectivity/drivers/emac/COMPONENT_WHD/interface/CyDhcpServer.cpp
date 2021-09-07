/*
 * Copyright (c) 2018-2019 ARM Limited
 * SPDX-License-Identifier: Apache-2.0
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

#include "CyDhcpServer.h"
#include "cy_utils.h"
#include "Callback.h"
#include "def.h"
#include "whd_types.h"

#ifdef DHCP_EXTENSIVE_DEBUG
extern "C" void dhcp_server_print_header_info(dhcp_packet_t *header, uint32_t datalen, const char *title);
#endif

/* UDP port numbers for DHCP server and client */
#define IP_PORT_DHCP_SERVER                         (67)
#define IP_PORT_DHCP_CLIENT                         (68)

/* BOOTP operations */
#define BOOTP_OP_REQUEST                            (1)
#define BOOTP_OP_REPLY                              (2)

/* DCHP message types */
#define DHCP_MSG_TYPE_DISCOVER                      (1)
#define DHCP_MSG_TYPE_OFFER                         (2)
#define DHCP_MSG_TYPE_REQUEST                       (3)
#define DHCP_MSG_TYPE_DECLINE                       (4)
#define DHCP_MSG_TYPE_ACK                           (5)
#define DHCP_MSG_TYPE_NACK                          (6)
#define DHCP_MSG_TYPE_RELEASE                       (7)
#define DHCP_MSG_TYPE_INFORM                        (8)
#define DHCP_MSG_TYPE_INVALID                       (255)

#define DHCP_MSG_MAGIC_COOKIE                       (0x63825363)

#define DHCP_STACK_SIZE                             (8*1024)

/********************* Options manipulation functions ***********************************/
static void addOption(dhcp_packet_t *dhcp, uint32_t &index, uint8_t optype)
{
    if (index + sizeof(dhcp_packet_t) - 1 + 1 >= DHCP_PACKET_SIZE) {
        printf("DHCP ERROR: Option index %d (Optype: %d) written to exceeds size of the packet", (int)index, (int)optype);
        return;
    }

    dhcp->Options[index++] = optype;

    return;
}

static void addOption(dhcp_packet_t *dhcp, uint32_t &index, uint8_t optype, uint8_t value)
{
    if (index + sizeof(dhcp_packet_t) - 1 + 3 >= DHCP_PACKET_SIZE) {
        printf("DHCP ERROR: Option index %d (Optype: %d) written to exceeds size of the packet", (int)index, (int)optype);
        return;
    }

    dhcp->Options[index++] = optype;
    dhcp->Options[index++] = 0x01;
    dhcp->Options[index++] = value;

    return;
}

static void addOption(dhcp_packet_t *dhcp, uint32_t &index, uint8_t optype, uint16_t value)
{
    if (index + sizeof(dhcp_packet_t) - 1 + 4 >= DHCP_PACKET_SIZE) {
        printf("DHCP ERROR: Option index %d (Optype: %d) written to exceeds size of the packet", (int)index, (int)optype);
        return;
    }

    dhcp->Options[index++] = optype;
    dhcp->Options[index++] = 0x02;
    dhcp->Options[index++] = static_cast<uint8_t>((value >> 0) & 0xFF);
    dhcp->Options[index++] = static_cast<uint8_t>((value >> 8) & 0xFF);

    return;
}

static void addOption(dhcp_packet_t *dhcp, uint32_t &index, uint8_t optype, uint32_t value)
{
    if (index + sizeof(dhcp_packet_t) - 1 + 6 >= DHCP_PACKET_SIZE) {
        printf("DHCP ERROR: Option index %d (Optype: %d) written to exceeds size of the packet", (int)index, (int)optype);
        return;
    }

    dhcp->Options[index++] = optype;
    dhcp->Options[index++] = 0x04;
    dhcp->Options[index++] = static_cast<uint8_t>((value >> 0) & 0xFF);
    dhcp->Options[index++] = static_cast<uint8_t>((value >> 8) & 0xFF);
    dhcp->Options[index++] = static_cast<uint8_t>((value >> 16) & 0xFF);
    dhcp->Options[index++] = static_cast<uint8_t>((value >> 24) & 0xFF);

    return;
}

static void addOption(dhcp_packet_t *dhcp, uint32_t &index, uint8_t optype, uint8_t *value, uint32_t size)
{
    if (index + sizeof(dhcp_packet_t) - 1 + 2 + size >= DHCP_PACKET_SIZE) {
        printf("DHCP ERROR: Option index %d (Optype: %d) written to exceeds size of the packet", (int)index, (int)optype);
        return;
    }

    dhcp->Options[index++] = optype;
    dhcp->Options[index++] = size;
    memcpy(&dhcp->Options[index], value, size);
    index += size;

    return;
}

static const uint8_t *findOption(const dhcp_packet_t *request, uint8_t option_num)
{
    const uint8_t *option_ptr = request->Options;
    while ((option_ptr[0] != DHCP_END_OPTION_CODE) &&
            (option_ptr[0] != option_num) &&
            (option_ptr < ((const uint8_t *)request) + DHCP_PACKET_SIZE)) {
        option_ptr += option_ptr[1] + 2;
    }

    /* Was the option found? */
    if (option_ptr[0] == option_num) {
        return &option_ptr[2];
    }
    return NULL;
}

static void addCommonOptions(dhcp_packet_t *dhcp, uint32_t &index, const uint32_t server_addr, const uint32_t netmask)
{
    /* Prepare the Web proxy auto discovery URL */
    char wpad_sample_url[] = "http://xxx.xxx.xxx.xxx/wpad.dat";
    char ip_str[16];
    ipv4_to_string(ip_str, htonl(server_addr));
    memcpy(&wpad_sample_url[7], &ip_str[0], 15);

    /* Server identifier */
    addOption(dhcp, index, DHCP_SERVER_IDENTIFIER_OPTION_CODE, server_addr);
    /* Lease Time */
    addOption(dhcp, index, DHCP_LEASETIME_OPTION_CODE, static_cast<uint32_t>(0x00015180));
    /* Subnet Mask */
    addOption(dhcp, index, DHCP_SUBNETMASK_OPTION_CODE, htonl(netmask));
    /* Web proxy auto discovery URL */
    addOption(dhcp, index, DHCP_WPAD_OPTION_CODE, (uint8_t *)&wpad_sample_url[0], strlen(wpad_sample_url));
    /* Router (gateway) */
    addOption(dhcp, index, DHCP_ROUTER_OPTION_CODE, htonl(server_addr));
    /* DNS server */
    addOption(dhcp, index, DHCP_DNS_SERVER_OPTION_CODE, htonl(server_addr));
    /* Interface MTU */
    addOption(dhcp, index, DHCP_MTU_OPTION_CODE, static_cast<uint16_t>(WHD_PAYLOAD_MTU));
}

static void sendPacket(UDPSocket *socket, dhcp_packet_t *dhcp, uint32_t size)
{
    nsapi_size_or_error_t err;
    uint32_t broadcast_ip = 0xFFFFFFFF;
    char string_addr[16];
    ipv4_to_string(string_addr, htonl(broadcast_ip));
    SocketAddress sock_addr(string_addr, IP_PORT_DHCP_CLIENT);

    err = socket->sendto(sock_addr, reinterpret_cast<uint8_t *>(dhcp), size);
    if (err < 0) {
        printf("DHCP ERROR: Packet send failure with error %d.", err);
    } else if (err != (int)size) {
        printf("DHCP ERROR: Could not send entire packet. Only %d bytes were sent.", err);
    }
}

/********************* Cache utility functions ***********************************/
void CyDhcpServer::setAddress(const cy_mac_addr_t &mac_id, const cy_ip_addr_t &addr)
{
    uint32_t a;
    uint32_t first_empty_slot;
    uint32_t cached_slot;
    char empty_cache[NSAPI_IPv6_SIZE] = "";

    /* Search for empty slot in cache */
    for (a = 0, first_empty_slot = DHCP_IP_ADDRESS_CACHE_MAX, cached_slot = DHCP_IP_ADDRESS_CACHE_MAX; a < DHCP_IP_ADDRESS_CACHE_MAX; a++) {
        /* Check for matching MAC address */
        if (memcmp(&_mac_addr_cache[a], &mac_id, sizeof(mac_id)) == 0) {
            /* Cached device found */
            cached_slot = a;
            break;
        } else if (first_empty_slot == DHCP_IP_ADDRESS_CACHE_MAX && memcmp(&_mac_addr_cache[a], &empty_cache, sizeof(cy_mac_addr_t)) == 0) {
            /* Device not found in cache. Return the first empty slot */
            first_empty_slot = a;
        }
    }

    if (cached_slot != DHCP_IP_ADDRESS_CACHE_MAX) {
        /* Update IP address of cached device */
        _ip_addr_cache[cached_slot] = addr;
    } else if (first_empty_slot != DHCP_IP_ADDRESS_CACHE_MAX) {
        /* Add device to the first empty slot */
        _mac_addr_cache[first_empty_slot] = mac_id;
        _ip_addr_cache[first_empty_slot] = addr;
    } else {
        /* Cache is full. Add device to slot 0 */
        _mac_addr_cache[0] = mac_id;
        _ip_addr_cache [0] = addr;
    }
}

bool CyDhcpServer::lookupAddress(const cy_mac_addr_t &mac_id, cy_ip_addr_t &addr)
{
    /* Check whether device is already cached */
    for (uint32_t a = 0; a < DHCP_IP_ADDRESS_CACHE_MAX; a++) {
        if (memcmp(&_mac_addr_cache[a], &mac_id, sizeof(mac_id)) == 0) {
            addr = _ip_addr_cache[a];
            return true;
        }
    }
    return false;
}

void CyDhcpServer::freeAddress(const cy_mac_addr_t &mac_id)
{
    /* Check whether device is already cached */
    for (uint32_t a = 0; a < DHCP_IP_ADDRESS_CACHE_MAX; a++) {
        if (memcmp(&_mac_addr_cache[a], &mac_id, sizeof(mac_id)) == 0) {
            memset(&_mac_addr_cache[a], 0, sizeof(_mac_addr_cache[a]));
            memset(&_ip_addr_cache[a], 0, sizeof(_ip_addr_cache[a]));
        }
    }
}

void CyDhcpServer::handleDiscover(dhcp_packet_t *dhcp)
{
#ifdef DHCP_EXTENSIVE_DEBUG
    dhcp_server_print_header_info(dhcp, DHCP_PACKET_SIZE, "\n\nDHCP DISCOVER RECEIVED");
#endif

    uint32_t index;
    cy_mac_addr_t client_mac;
    cy_ip_addr_t client_ip;

    memcpy(&client_mac, dhcp->ClientHwAddr, sizeof(client_mac));
    if (!lookupAddress(client_mac, client_ip)) {
        client_ip = _available_addr;
    }

    memset(&dhcp->Legacy, 0, sizeof(dhcp->Legacy));
    memset(&dhcp->Options[0], 0, DHCP_PACKET_SIZE - sizeof(dhcp_packet_t) + 3);

    dhcp->Opcode = BOOTP_OP_REPLY;
    dhcp->YourIpAddr = htonl(client_ip.addrv4.addr);
    dhcp->MagicCookie = htonl(static_cast<uint32_t>(DHCP_MSG_MAGIC_COOKIE));

    /* Add options */
    index = 0;
    addOption(dhcp, index, DHCP_MESSAGETYPE_OPTION_CODE, static_cast<uint8_t>(DHCP_MSG_TYPE_OFFER));
    addCommonOptions(dhcp, index, _server_addr.addrv4.addr, _netmask.addrv4.addr);
    addOption(dhcp, index, static_cast<uint8_t>(DHCP_END_OPTION_CODE));

    uint32_t size = sizeof(dhcp_packet_t) + index - 1;
    CY_ASSERT(size <= DHCP_PACKET_SIZE);

#ifdef DHCP_EXTENSIVE_DEBUG
    dhcp_server_print_header_info(dhcp, size, "\n\nDHCP OFFER SENT");
#endif
    sendPacket(&_socket, dhcp, size);
}

void CyDhcpServer::handleRequest(dhcp_packet_t *dhcp)
{
#ifdef DHCP_EXTENSIVE_DEBUG
    dhcp_server_print_header_info(dhcp, DHCP_PACKET_SIZE, "\n\nDHCP REQUEST RECEIVED");
#endif

    cy_mac_addr_t client_mac;
    cy_ip_addr_t client_ip;
    cy_ip_addr_t req_ip;
    bool increment = false;
    uint32_t index;

    /* Check that the REQUEST is for this server */
    uint32_t *server_id_req = (uint32_t *)findOption(dhcp, DHCP_SERVER_IDENTIFIER_OPTION_CODE);
    if ((server_id_req == NULL) || ((server_id_req != NULL) && (_server_addr.addrv4.addr != *server_id_req))) {
        return; /* Server ID was not found or does not match local IP address */
    }

    /* Locate the requested address in the options and keep requested address */
    req_ip.addrv4.addr = ntohl(*(uint32_t *)findOption(dhcp, DHCP_REQUESTED_IP_ADDRESS_OPTION_CODE));

    memcpy(&client_mac, dhcp->ClientHwAddr, sizeof(client_mac));
    if (!lookupAddress(client_mac, client_ip)) {
        client_ip = _available_addr;
        increment = true;
    }

    memset(&dhcp->Legacy, 0, sizeof(dhcp->Legacy));
    memset(&dhcp->Options[0], 0, DHCP_PACKET_SIZE - sizeof(dhcp_packet_t) + 3);

    dhcp->Opcode = BOOTP_OP_REPLY;
    dhcp->MagicCookie = htonl(static_cast<uint32_t>(DHCP_MSG_MAGIC_COOKIE));

    index = 0;
    /* Check if the requested IP address matches one we have assigned */
    if (req_ip.addrv4.addr != client_ip.addrv4.addr) {
        /* Request is not for the assigned IP - force client to take next available IP by sending NAK */
        addOption(dhcp, index, DHCP_MESSAGETYPE_OPTION_CODE, static_cast<uint8_t>(DHCP_MSG_TYPE_NACK));
        addOption(dhcp, index, DHCP_SERVER_IDENTIFIER_OPTION_CODE, _server_addr.addrv4.addr);
        printf("\n\nDHCP_THREAD: %d REQUEST NAK\n", __LINE__);
    } else {
        dhcp->YourIpAddr = htonl(client_ip.addrv4.addr);

        addOption(dhcp, index, DHCP_MESSAGETYPE_OPTION_CODE, static_cast<uint8_t>(DHCP_MSG_TYPE_ACK));
        addCommonOptions(dhcp, index, _server_addr.addrv4.addr, _netmask.addrv4.addr);

        if (increment) {
            uint32_t ip_mask = ~(_netmask.addrv4.addr);
            uint32_t subnet = _server_addr.addrv4.addr & _netmask.addrv4.addr;
            do {
                _available_addr.addrv4.addr = subnet | ((_available_addr.addrv4.addr + 1) & ip_mask);
            } while (_available_addr.addrv4.addr == _server_addr.addrv4.addr);
        }
        setAddress(client_mac, client_ip);
    }
    addOption(dhcp, index, static_cast<uint8_t>(DHCP_END_OPTION_CODE));

    uint32_t size = sizeof(dhcp_packet_t) + index - 1;
    CY_ASSERT(size <= DHCP_PACKET_SIZE);

#ifdef DHCP_EXTENSIVE_DEBUG
    dhcp_server_print_header_info(dhcp, DHCP_PACKET_SIZE, "\n\nDHCP REQUEST REPLY SENT");
#endif
    sendPacket(&_socket, dhcp, size);
}

void CyDhcpServer::runServer(void)
{
    nsapi_size_or_error_t err_or_size;

    _running = true;

    /* Create receive DHCP socket */
    _socket.open(_nstack);
    char iface_name[8] = {0};
    _niface->get_interface_name(iface_name);
    _socket.setsockopt(NSAPI_SOCKET, NSAPI_BIND_TO_DEVICE, iface_name, strlen(iface_name));
    _socket.bind((uint16_t)IP_PORT_DHCP_SERVER);

    /* Save the current netmask to be sent in DHCP packets as the 'subnet mask option' */
    SocketAddress sock_addr;
    _niface->get_ip_address(&sock_addr);
    _server_addr.addrv4.addr = string_to_ipv4(sock_addr.get_ip_address());
    _niface->get_netmask(&sock_addr);
    _netmask.addrv4.addr = string_to_ipv4(sock_addr.get_ip_address());

#ifdef DHCP_EXTENSIVE_DEBUG
    printf("DHCP Server started.\n");
    printf("DHCP Server: IP     : %s\n", _niface->get_ip_address());
    printf("DHCP Server: Netmask: %s\n", _niface->get_netmask());
    printf("DHCP Server: Gateway: %s\n", _niface->get_gateway());
    printf("DHCP Server: MAC    : %s\n\n", _niface->get_mac_address());
#endif

    /* Calculate the first available IP address which will be served - based on the netmask and the local IP */
    uint32_t ip_mask = ~(_netmask.addrv4.addr);
    uint32_t subnet = _server_addr.addrv4.addr & _netmask.addrv4.addr;
    _available_addr.addrv4.addr = subnet | ((_server_addr.addrv4.addr + 1) & ip_mask);

    while (_running) {
        /* Sleep until data is received from socket. */
        err_or_size = _socket.recv(_buff, DHCP_PACKET_SIZE);
        /* Options field in DHCP header is variable length. We are looking for option "DHCP Message Type" that is 3 octets in size (code, length and type) */
        /* If the return value is <0, it is an error; if it is >=0, it is the received length */
        if (err_or_size < 0 || err_or_size < (int32_t)sizeof(dhcp_packet_t)) {
            continue;
        }

        dhcp_packet_t *dhcp = reinterpret_cast<dhcp_packet_t *>(_buff);
        /* Check if the option in the dhcp header is "DHCP Message Type", code value for option "DHCP Message Type" is 53 as per rfc2132 */
        if (dhcp->Options[0] != DHCP_MESSAGETYPE_OPTION_CODE) {
            printf("%d: %s received option code wrong: %d != %d\n", __LINE__, __func__, dhcp->Options[0], DHCP_MESSAGETYPE_OPTION_CODE);
            continue;
        }

        uint8_t msg_type = dhcp->Options[2];
        switch (msg_type) {
            case DHCP_MSG_TYPE_DISCOVER:
                handleDiscover(dhcp);
                break;
            case DHCP_MSG_TYPE_REQUEST:
                handleRequest(dhcp);
                break;
            default:
                printf("DHCP ERROR: Unhandled dhcp packet type, %d", msg_type);
                break;
        }
    }
}

void CyDhcpServer::threadWrapper(CyDhcpServer *obj)
{
    obj->runServer();
}

CyDhcpServer::CyDhcpServer(NetworkStack *nstack, NetworkInterface *niface)
    : _nstack(nstack),
      _niface(niface),
      _thread(osPriorityNormal, DHCP_STACK_SIZE, NULL, "DHCPserver") {}

CyDhcpServer::~CyDhcpServer()
{
    stop();
}

cy_rslt_t CyDhcpServer::start(void)
{
    cy_rslt_t result = CY_RSLT_SUCCESS;
    if (!_running) {
        CY_ASSERT(_nstack != NULL);

        /* Clear cache */
        memset(_mac_addr_cache, 0, sizeof(_mac_addr_cache));
        memset(_ip_addr_cache, 0, sizeof(_ip_addr_cache));

        /* Start DHCP server */
        if (osOK != _thread.start(mbed::callback(threadWrapper, this))) {
            result = CY_DHCP_THREAD_CREATION_FAILED;
        }
    }
    return result;
}

cy_rslt_t CyDhcpServer::stop(void)
{
    cy_rslt_t result = CY_RSLT_SUCCESS;
    if (_running) {
        _running = false;
        if (NSAPI_ERROR_OK != _socket.close()) {
            printf("DHCP ERROR: DHCP socket closure failed.\n");
            result = CY_DHCP_STOP_FAILED;
        }
        if (osOK != _thread.join()) {
            printf("DHCP ERROR: DHCP thread join failed.\n");
            result = CY_DHCP_STOP_FAILED;
        }
    }
    return result;
}
