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

#ifndef WHD_DHCP_SERVER_H
#define WHD_DHCP_SERVER_H

#include "cy_result.h"
#include "cy_syslib.h"
#include "cynetwork_utils.h"
#include "UDPSocket.h"
#include "netsocket/NetworkInterface.h"
#include "netsocket/NetworkStack.h"
#include "rtos.h"

/* DHCP data structure */
typedef struct {
    uint8_t  Opcode;            /* packet opcode type */
    uint8_t  HwType;            /* hardware addr type */
    uint8_t  HwLen;             /* hardware addr length */
    uint8_t  Hops;              /* gateway hops */
    uint32_t TransactionId;     /* transaction ID */
    uint16_t SecsElapsed;       /* seconds since boot began */
    uint16_t Flags;
    uint32_t ClientIpAddr;      /* client IP address */
    uint32_t YourIpAddr;        /* 'your' IP address */
    uint32_t ServerIpAddr;      /* server IP address */
    uint32_t GatewayIpAddr;     /* gateway IP address */
    uint8_t  ClientHwAddr[16];  /* client hardware address */
    uint8_t  Legacy[192];       /* SName, File */
    uint32_t MagicCookie;
    uint8_t  Options[3];        /* options area */
    /* as of RFC2131 it is variable length */
} dhcp_packet_t;

#define DHCP_SUBNETMASK_OPTION_CODE                 (1)
#define DHCP_ROUTER_OPTION_CODE                     (3)
#define DHCP_DNS_SERVER_OPTION_CODE                 (6)
#define DHCP_HOST_NAME_OPTION_CODE                  (12)
#define DHCP_MTU_OPTION_CODE                        (26)
#define DHCP_REQUESTED_IP_ADDRESS_OPTION_CODE       (50)
#define DHCP_LEASETIME_OPTION_CODE                  (51)
#define DHCP_MESSAGETYPE_OPTION_CODE                (53)
#define DHCP_SERVER_IDENTIFIER_OPTION_CODE          (54)
#define DHCP_PARAM_REQUEST_LIST_OPTION_CODE         (55)
#define DHCP_WPAD_OPTION_CODE                       (252)
#define DHCP_END_OPTION_CODE                        (255)

#define DHCP_IP_ADDRESS_CACHE_MAX                   (5)
#define ADDITIONAL_OPTION_BYTES                     (272)
#define DHCP_PACKET_SIZE                            (sizeof(dhcp_packet_t) + ADDITIONAL_OPTION_BYTES)

/** DHCP thread could not be started */
#define CY_DHCP_THREAD_CREATION_FAILED  CY_RSLT_CREATE(CY_RSLT_TYPE_ERROR, CY_RSLT_MODULE_MIDDLEWARE_BASE, 0)

/** Error while trying to stop the DHCP server */
#define CY_DHCP_STOP_FAILED  CY_RSLT_CREATE(CY_RSLT_TYPE_ERROR, CY_RSLT_MODULE_MIDDLEWARE_BASE, 1)

/**
 *  Implementation of a DHCP sever
 */
class CyDhcpServer {
public:
    /**
     * Create a DHCP server.
     */
    CyDhcpServer(NetworkStack *nstack, NetworkInterface *niface);

    /**
     * Delete the DHCP server.
     */
    virtual ~CyDhcpServer();

    /**
     *  Start a DHCP server instance.
     * @return CY_RSLT_SUCCESS on success otherwise error.
     */
    cy_rslt_t start(void);

    /**
     *  Stop a DHCP server instance.
     * @return CY_RSLT_SUCCESS on success otherwise error.
     */
    cy_rslt_t stop(void);

private:
    NetworkStack *_nstack = NULL;
    NetworkInterface *_niface = NULL;
    UDPSocket _socket;
    rtos::Thread _thread;
    bool _running = false;

    cy_ip_addr_t _available_addr;
    cy_ip_addr_t _server_addr;
    cy_ip_addr_t _netmask;

    cy_mac_addr_t _mac_addr_cache[DHCP_IP_ADDRESS_CACHE_MAX];
    cy_ip_addr_t _ip_addr_cache[DHCP_IP_ADDRESS_CACHE_MAX];
    uint8_t _buff[DHCP_PACKET_SIZE];

    static void threadWrapper(CyDhcpServer *obj);
    void runServer(void);

    void setAddress(const cy_mac_addr_t &mac_id, const cy_ip_addr_t &addr);
    bool lookupAddress(const cy_mac_addr_t &mac_id, cy_ip_addr_t &addr);
    void freeAddress(const cy_mac_addr_t &mac_id);

    void handleDiscover(dhcp_packet_t *dhcp);
    void handleRequest(dhcp_packet_t *dhcp);
};

#endif /* WHD_DHCP_SERVER_H */
