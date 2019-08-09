/*
 * Copyright (c) 2013 - 2014, Freescale Semiconductor, Inc.
 * Copyright 2016-2017 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

// SDK Included Files
#include <stdio.h>
#include <stdlib.h>
#include "qcom_api.h"
#include "wlan_qcom.h"

// devId used for first argument of all the qcom API functions
static int devId = 0;

// Values used by qcom_ipconfig()
struct network_ipv4_s
{
    uint32_t addr;
    uint32_t mask;
    uint32_t gateway;
} ipv4;

static int _traceQcomApi = 0;

// ============================================================================
// Defines
// ============================================================================

// Convert IP address in uint32_t to comma separated bytes
#define UINT32_IPADDR_TO_CSV_BYTES(a) ((uint8_t)((a) >> 24) & 0xFF), (uint8_t)(((a) >> 16) & 0xFF), (uint8_t)(((a) >> 8) & 0xFF), (uint8_t)((a)&0xFF)
// Convert comma separated bytes to a uint32_t IP address
#define CSV_BYTES_TO_UINT32_IPADDR(a0, a1, a2, a3) \
    (((uint32_t)(a0)&0xFF) << 24) | (((uint32_t)(a1)&0xFF) << 16) | (((uint32_t)(a2)&0xFF) << 8) | ((uint32_t)(a3)&0xFF)
#define CONSTSTR_LEN(variable) (sizeof(variable) - 1)

// ============================================================================
// Utility Functions
// ============================================================================

static void printError(uint32_t value, const char *funcName)
{
    printf("ERROR: %s() returned %d\r\n", funcName, value);
}

static int isQcomError(A_STATUS status, const char *funcName)
{
    if (status != A_OK)
    {
        printError(status, funcName);
    }
    else if (_traceQcomApi)
    {
        printf("%s() OK\r\n", funcName);
    }
    return (status != A_OK);
}

static int isValueFailed(int32_t value, int32_t failValue, const char *funcName)
{
    if (value == failValue)
    {
        printError(value, funcName);
    }
    else if (_traceQcomApi)
    {
        printf("%s() OK\r\n", funcName);
    }
    return (value == failValue);
}

/** Enable printing/tracing of QCOM functions calls
 *  @param enable   0 = disable
 *                  1 = enable
 *                 -1 = toggle
 *  @return         previous value
 */
int traceQcomApi(int enable)
{
    int oldValue = _traceQcomApi;
    if (enable == -1)
        _traceQcomApi = !_traceQcomApi;
    else
        _traceQcomApi = enable;
    return oldValue;
}

// ============================================================================
// IP config and DHCP
// ============================================================================

// Although indicated in the QCA 80-Y9106-1 doc, this enum is unfortunately not defined
enum ipconfigActions
{
    IP_CONFIG_QUERY = 0,
    IP_CONFIG_SET = 1,
    IP_CONFIG_DHCP = 2,
};

int getDhcp(void)
{
    A_STATUS status;

    printf("Getting DHCP address...\r\n");

    for (int32_t i = 20; i > 0; i--)
    {
        status = qcom_ipconfig(devId, QCOM_IPCONFIG_DHCP, &ipv4.addr, &ipv4.mask, &ipv4.gateway);
        A_MDELAY(50);
        status = qcom_ipconfig(devId, QCOM_IPCONFIG_QUERY, &ipv4.addr, &ipv4.mask, &ipv4.gateway);
        if (ipv4.addr != 0)
        {
            break;
        }
    }

    if (isQcomError(status, "qcom_ipconfig"))
        return A_ERROR;

    // Print the DNS servers we got from the DHCP server
    uint32_t dnsServers[3];
    uint32_t num;
    status = qcom_dns_server_address_get(dnsServers, &num);
    if (isQcomError(status, "qcom_dns_server_address_get"))
    {
        printf("WARNING: No DNS servers returned by DHCP\r\n");
        printf("         You will NOT be able to resolve domain names\r\n");
        return A_ERROR;
    }

    for (uint32_t i = 0; i < num; i++)
    {
        printf("DNS %d: %d.%d.%d.%d\r\n", i, UINT32_IPADDR_TO_CSV_BYTES(dnsServers[i]));
    }

    printIpConfig();

    return A_OK;
}

void printIpConfig(void)
{
    A_STATUS status = qcom_ipconfig(devId, QCOM_IPCONFIG_QUERY, &ipv4.addr, &ipv4.mask, &ipv4.gateway);
    isQcomError(status, "qcom_ipconfig");

    printf("addr: %d.%d.%d.%d mask: %d.%d.%d.%d gw: %d.%d.%d.%d\r\n", UINT32_IPADDR_TO_CSV_BYTES(ipv4.addr),
           UINT32_IPADDR_TO_CSV_BYTES(ipv4.mask), UINT32_IPADDR_TO_CSV_BYTES(ipv4.gateway));
}

// ============================================================================
// Scanning and Access Point Connect/Disconnect
// ============================================================================

char _isConnected = 0;

static void onConnect(uint8_t event, uint8_t devId, char *bssid, uint8_t bssConn)
{
    printf("EVENT: ");

    switch (event)
    {
        case 1:
            if (devId == 0)
            {
                printf("%s connected\r\n", bssConn ? "AP" : "CLIENT");
            }
            else
            {
                printf("Connected\r\n");
            }
            // NOTE that station is not fully connected until PEER_FIRST_NODE_JOIN_EVENT
            break;
        case INVALID_PROFILE:
            // this event is used to indicate RSNA failure
            _isConnected = 0;
            printf("4 way handshake failure for device=%d n", devId);
            break;
        case PEER_FIRST_NODE_JOIN_EVENT:
            // this event is used to RSNA success
            _isConnected = 1;
            printf("4 way handshake success for device=%d\r\n", devId);
            break;
        case 0:
            _isConnected = 0;
            if (devId == 0)
            {
                printf("%s disconnect\r\n", bssConn ? "AP" : "CLIENT");
            }
            break;

        default:
            printf("code %d\r\n", event);
    }
}

int isConnected(void)
{
    return _isConnected;
}

static void apConnectParamsPrint(void)
{
    A_STATUS status;

    printf("Reading connection params\r\n");

    // Get current operational mode
    QCOM_WLAN_DEV_MODE opMode = QCOM_WLAN_DEV_MODE_INVALID;
    status = qcom_op_get_mode(devId, &opMode);
    isQcomError(status, "qcom_op_get_mode");
    printf("  opMode=%d (%s)\r\n", opMode, (opMode == 0 ? "Station" : "AP"));

    char *phyMode = NULL;
    status = qcom_get_phy_mode(devId, &phyMode);
    isQcomError(status, "qcom_get_phy_mode");
    if (NULL != phyMode)
    {
        printf("  phyMode=%s\r\n", phyMode);
    }

    // NOTE: Listed in QCA 80-Y9106-1 although not implemented by driver
    // uint32_t authMode
    // status = qcom_sec_get_auth_mode(devId, &authMode);
    // checkForError(status, "qcom_sec_get_auth_mode");

    QCOM_SSID ssidRead = {0};
    status = qcom_get_ssid(devId, &ssidRead);
    isQcomError(status, "qcom_get_ssid");
    printf("  ssid=%s\r\n", ssidRead.ssid);
}

static char *getSecurityStr(uint8_t auth, uint8_t cipher)
{
    static char s[100];
    char *p = s;
    s[0] = 0;

    if (auth & SECURITY_AUTH_1X)
        p += sprintf(p, "802.1X ");
    if (auth & SECURITY_AUTH_PSK)
        p += sprintf(p, "PSK ");

    p += sprintf(p, "/ ");

    if (cipher & ATH_CIPHER_TYPE_WEP)
        p += sprintf(p, "WEP ");
    if (cipher & ATH_CIPHER_TYPE_TKIP)
        p += sprintf(p, "TKIP ");
    if (cipher & ATH_CIPHER_TYPE_CCMP)
        p += sprintf(p, "CCMP ");

    return s;
}

void apScan(void)
{
    A_STATUS status;

    // NOTE: If scan is initiated when SSID is set to a value then only that SSID is scanned
    status = qcom_set_scan(devId, NULL);
    isQcomError(status, "qcom_set_scan");

    // NOTE: calling qcom_get_scan() before qcom_set_scan() is the same as suspending for 5s,
    // i.e. nothing happens for 5s where after 0 is returned
    // NOTE: A maximum of 12 results are returned
    QCOM_BSS_SCAN_INFO *scanResult;
    int16_t scanNumResults = 0;
    status = qcom_get_scan(devId, &scanResult, &scanNumResults);
    isQcomError(status, "qcom_get_scan");
    // printf("qcom_get_scan numResults=%d\r\n", scanNumResults);

    printf(" i Ch RSSI  MAC                SSID\r\n");
    for (int i = 0; i < scanNumResults; i++)
    {
        QCOM_BSS_SCAN_INFO *scr = &scanResult[i];
        int ssidLen = scr->ssid_len;
        scr->ssid[ssidLen] = 0;

        printf("%2d %2d %4d  %02x:%02x:%02x:%02x:%02x:%02x  %s\r\n", i, scr->channel, scr->rssi, scr->bssid[0],
               scr->bssid[1], scr->bssid[2], scr->bssid[3], scr->bssid[4], scr->bssid[5], scr->ssid);

        if (scr->security_enabled)
        {
            if (scr->rsn_cipher == 0 && scr->wpa_cipher == 0)
            {
                printf("            WEP\r\n");
            }
            else
            {
                char *secStr;
                secStr = getSecurityStr(scr->rsn_auth, scr->rsn_cipher);
                printf("            RSN: %s\r\n", secStr);
                secStr = getSecurityStr(scr->wpa_auth, scr->wpa_cipher);
                printf("            WPA: %s\r\n", secStr);
            }
        }
        else
        {
            printf("            open AP\r\n");
        }
    }

    printf("--------------------\r\n");
}

int apConnect(QCOM_SSID *ssid, QCOM_PASSPHRASE *passphrase, WLAN_AUTH_MODE auth, WLAN_CRYPT_TYPE cipher)
{
    A_STATUS status;

    // Set station mode to station (0) or AP (1)
    status = qcom_op_set_mode(devId, QCOM_WLAN_DEV_MODE_STATION);
    if (isQcomError(status, "qcom_op_set_mode")) {
        return A_ERROR;
    }

    // Optionally set the PHY mode
    // status = qcom_set_phy_mode(devId, QCOM_11G_MODE);
    // checkForError(status, "qcom_set_phy_mode");

    // NOTE: qcom API requires to first set a valid SSID (before auth, cipher and passphrase)
    status = qcom_set_ssid(devId, ssid);
    if (isQcomError(status, "qcom_set_ssid")) {
        return A_ERROR;
    }

    status = qcom_sec_set_encrypt_mode(devId, cipher);
    if (isQcomError(status, "qcom_sec_set_encrypt_mode")) {
        return A_ERROR;
    }

    // Note that only 4 of all modes listed in QCA 80-Y9106-1 are supported!
    // The modes are: WLAN_AUTH_NONE, WLAN_AUTH_WPA_PSK, WLAN_AUTH_WPA2_PSK, WLAN_AUTH_WEP
    status = qcom_sec_set_auth_mode(devId, auth);
    if (isQcomError(status, "qcom_sec_set_auth_mode")) {
        return A_ERROR;
    }

    // NOTE: The driver insists that the SSID is configured *before* the passphrase
    status = qcom_sec_set_passphrase(devId, passphrase);
    if (isQcomError(status, "qcom_sec_set_passphrase")) {
        return A_ERROR;
    }


    // The connect callback is actually used for four different callbacks:
    //     onConnect(1 (TRUE), uint8_t devId, uint8_t *ssid, uint8_t bssMac[6]);
    //     onDisconnect(status, uint8_t devId, uint8_t *ssid, uint8_t bssMac[6]);
    //         where status = 0x10 (INVALID_PROFILE) on ?
    //                      =    0 on normal disconnect
    //     onRSNASuccessEvent(uint8_t code, uint8_t devId, NULL, 0)
    //     onBitRateEvent_tx(wmi_rateTable[rateIndex][0], devId, NULL, 0);
    // It is not possible to discern the onBitRateEvent_tx event from the others
    status = qcom_set_connect_callback(devId, (void *)onConnect);
    if (isQcomError(status, "qcom_set_connect_callback")) {
        return A_ERROR;
    }

    apConnectParamsPrint();

    status = qcom_commit(devId);
    if (isQcomError(status, "qcom_commit")) {
        return A_ERROR;
    }

    return A_OK;
}

int apDisconnect(void)
{
    A_STATUS status = qcom_disconnect(devId);
    if (isQcomError(status, "qcom_disconnect")) {
        return A_ERROR;
    }

    return A_OK;
}

// ============================================================================
// Resolving and Pinging
// ============================================================================

uint32_t resolveHostname(const char *hostname)
{
    uint32_t addr = 0;

    // NOTE: This function returns the address in reverse byte order
    A_STATUS status = qcom_dnsc_get_host_by_name((char *)hostname, &addr);
    isQcomError(status, "qcom_dnsc_get_host_by_name");
    if (status == 0)
    {
        printf("Looked up %s as %d.%d.%d.%d\r\n", hostname, UINT32_IPADDR_TO_CSV_BYTES(addr));
    }
    return addr;
}

/** Ping given hostname
 *  If hostname is NULL then the gateway is used
 */
uint32_t pingHost(const char *hostname)
{
    // Default is to ping the gateway
    uint32_t addr = ipv4.gateway;

    if (hostname != NULL)
    {
        addr = resolveHostname(hostname);
    }

    // NOTE: qcom_ping() is a blocking function and has no timeout so it will
    printf("Pinging %d.%d.%d.%d... ", UINT32_IPADDR_TO_CSV_BYTES(addr));
    uint32_t t = A_TIME_GET_MSEC();
    A_STATUS status = qcom_ping(addr, 10);
    uint32_t elapsed = status;
    if (status == 0)
    {
        elapsed = A_TIME_GET_MSEC() - t;
        printf("OK (%d ms)\r\n", elapsed);
    }
    isQcomError(status, "qcom_ping");
    return elapsed;
}

void resolveManyHosts(void)
{
    const char *hostnames[] = {
        "google.com", "cr.yp.to", "kernel.org", "nxp.com",
    };
    int numHosts = sizeof(hostnames) / sizeof(hostnames[0]);
    for (int i = 0; i < numHosts; i++)
    {
        resolveHostname(hostnames[i]);
    }
}

// ============================================================================
// UDP example
// ============================================================================

static int32_t udpSock = 0;
static char *udpRecvBuf = NULL;
static int udpRecvBufLen = 0;

// IPv4 address of remote peer
static SOCKADDR_T remAddr;
static socklen_t remAddrLen;

static void udpHelp(uint16_t port)
{
    printf("\r\n");
    printf("Test UDP echo server from your Unix workstation with the netcat command:\r\n");
    printf("  echo 'hello' | netcat -u -q1 %d.%d.%d.%d %d\r\n", UINT32_IPADDR_TO_CSV_BYTES(ipv4.addr), port);
}

void udpBind(uint16_t port)
{
    A_STATUS status;
    SOCKADDR_T localAddr;

    if (udpSock)
    {
        printf("UDP echo server socket already created\r\n");
        udpHelp(port);
        return;
    }

    udpSock = qcom_socket(ATH_AF_INET, SOCK_DGRAM_TYPE, 0);
    if (isValueFailed(udpSock, -1, "qcom_socket"))
        return;

    memset(&localAddr, 0, sizeof(localAddr));
    localAddr.sin_port = port;
    localAddr.sin_family = ATH_AF_INET;
    localAddr.sin_addr.s_addr = CSV_BYTES_TO_UINT32_IPADDR(0, 0, 0, 0);

    status = 0 == qcom_bind(udpSock, (struct sockaddr *)(&localAddr), sizeof(localAddr)) ? A_OK : A_ERROR;
    if (isQcomError(status, "qcom_bind"))
        return;

    printf("UDP echo server listening on port %d\r\n", port);
    udpHelp(port);
}

/** Poll for UDP packet for [timeout] ms
 *  Return  -1 on timeout
 *          -9 if UDP socket hasn't been opened
 *          Otherwise, number of bytes received
 */
int udpPollAndRecv(int timeout)
{
    // Return immediately if UDP listen socket hasn't been created yet
    if (udpSock == 0)
        return -9;

    // NOTE: There is no qcom_select() function, only t_select() that polls only one socket
    // NOTE: t_select() does NOT take a devId like all the other functions
    QCA_CONTEXT_STRUCT *enetCtx = wlan_get_context();
    int32_t status = t_select(enetCtx, udpSock, timeout);
    if (status == -1)
        return A_ERROR;

    // Free the ZERO_COPY receive buffer (from previous call of this function)
    if (udpRecvBuf != NULL)
    {
        // printf("Freeing old buffer\r\n");
        zero_copy_free(udpRecvBuf);
        udpRecvBuf = NULL;
        udpRecvBufLen = 0;
    }

    // NOTE: when QCA400x library has been compiled with ZERO_COPY, the recvBuf
    // is allocated by the qcom_recv() function and the user must free it afterwards!
    // NOTE: qcom_recvfrom() buffer argument is of type char although it should be void
    int received = qcom_recvfrom(udpSock, &udpRecvBuf, 500, 0, (struct sockaddr *)&remAddr, &remAddrLen);
    if (received <= 0)
    {
        printf("qcom_recvfrom() ERROR: %d\r\n", received);
    }
    printf("UDP received %d bytes from %d.%d.%d.%d:%d\r\n", received,
           UINT32_IPADDR_TO_CSV_BYTES(remAddr.sin_addr.s_addr), remAddr.sin_port);

    udpRecvBufLen = received;
    return received;
}

int udpSendEcho(void)
{
    // This is how you allocate a send buffer
    char *sendBuf = custom_alloc(udpRecvBufLen);
    if (sendBuf == NULL)
    {
        printError(0, "custom_alloc");
        return -1;
    }

    // Copy received data into send buffer
    memcpy(sendBuf, udpRecvBuf, udpRecvBufLen);

    int sent = qcom_sendto(udpSock, sendBuf, udpRecvBufLen, 0, (struct sockaddr *)&remAddr, remAddrLen);
    printf("UDP sent/echoed %d bytes\r\n", sent);

    // After sending the buffer, remember to free it!
    custom_free(sendBuf);

    return sent;
}

// ============================================================================
// TCP example
// ============================================================================
const static char header_start[] =
    "GET / HTTP/1.0\r\n"
    "User-Agent: Mozilla/5.0 (Windows NT 6.1; WOW64; rv:50.0) Gecko/20100101 Firefox/50.0\r\n"
    "Accept-Language: en-us\r\n"
    "Host: ";
const static char header_end[] = "\r\n\r\n";
static char gs_hostname[16] = {0};

void httpGet(const char *hostname, int timeout)
{
    int32_t sock = 0;
    SOCKADDR_T addr;
    A_STATUS status;
    char *request_data = NULL;
    char *response_data = NULL;

    printf("****************************************\r\n");

    memset(&addr, 0, sizeof(addr));
    addr.sin_family = ATH_AF_INET;
    if (hostname == NULL)
    {
        addr.sin_addr.s_addr = ipv4.gateway;
    }
    else
    {
        addr.sin_addr.s_addr = resolveHostname(hostname);
        if (addr.sin_addr.s_addr == 0)
        {
            printf("ERROR: Failed to resolve %s\r\n", hostname);
            return;
        }
    }
    addr.sin_port = 80;

    do
    {
        /* Create TCP socket */
        sock = qcom_socket(ATH_AF_INET, SOCK_STREAM_TYPE, 0);
        if (isValueFailed(sock, -1, "qcom_socket"))
            break;

        /* Connect to remote */
        status = (A_STATUS)qcom_connect(sock, (struct sockaddr *)&addr, sizeof(addr));
        if (isQcomError(status, "qcom_connect"))
            break;

        /* No explicit hostname, use gateway addr */
        if (hostname == NULL)
        {
            int32_t result = snprintf(gs_hostname, CONSTSTR_LEN(gs_hostname), "%u.%u.%u.%u", UINT32_IPADDR_TO_CSV_BYTES(ipv4.gateway));
            (void)result;
            assert(result > 0);
            hostname = gs_hostname;
        }

        /* Allocate request_data of size that includes extra '\0' for string functions */
        uint32_t hostname_len = strlen(hostname);
        uint32_t request_data_size = CONSTSTR_LEN(header_start) + hostname_len + CONSTSTR_LEN(header_end) + 1;
        request_data = custom_alloc(request_data_size);
        assert(!(NULL == request_data));
        if (NULL == request_data)
            break;
        memset(request_data, 0, request_data_size);

        /* Assemble HTTP header*/
        uint32_t request_data_i = 0, request_piece_size = 0;
        /* Copy 'header_start' to request_data */
        request_piece_size = CONSTSTR_LEN(header_start);
        memcpy(&request_data[request_data_i], header_start, request_piece_size);
        request_data_i += request_piece_size;
        /* Copy 'hostname' to 'request_data' */
        request_piece_size = hostname_len;
        memcpy(&request_data[request_data_i], hostname, request_piece_size);
        request_data_i += request_piece_size;
        /* Copy 'header_end' to 'request_data' */
        request_piece_size = CONSTSTR_LEN(header_end);
        memcpy(&request_data[request_data_i], header_end, request_piece_size);
        request_data_i += request_piece_size;

        /* Send HTTP header in TCP request */
        printf("HTTP GET from %u.%u.%u.%u:%u\r\n", UINT32_IPADDR_TO_CSV_BYTES(addr.sin_addr.s_addr), addr.sin_port);
        printf("%s\r\n", request_data);
        int sent = qcom_send(sock, request_data, request_data_size, 0);
        printf("TCP sent %d bytes\r\n", sent);

        /* Free request data */
        custom_free(request_data);
        request_data = NULL;
        if (sent < 0)
            break;

        /* Block wait for response */
        printf("Waiting for response (with t_select)\r\n");
        if (timeout == 0)
            timeout = 5000;
        QCA_CONTEXT_STRUCT *enetCtx = wlan_get_context();
        status = (A_STATUS)t_select(enetCtx, sock, timeout);
        if (status == A_ERROR)
            break;

        /* Receive response */
        printf("qcom_recv() receiving response\r\n");
        int recvLen = qcom_recv(sock, &response_data, 1400, 0);
        printf("TCP received %d bytes\r\n", recvLen);
        if (recvLen >= 0)
        {
            response_data[recvLen] = 0;
            printf("%s\r\n", response_data);
        }

        /* Free 'receive_data' */
        if (response_data != NULL)
        {
            zero_copy_free(response_data);
        }
    } while (0);

    status = (A_STATUS)qcom_socket_close(sock);
    isQcomError(status, "qcom_socket_close");
}
