// Copyright (c) Qualcomm Atheros, Inc.
// All rights reserved.
// Redistribution and use in source and binary forms, with or without modification, are permitted (subject to
// the limitations in the disclaimer below) provided that the following conditions are met:
//
// · Redistributions of source code must retain the above copyright notice, this list of conditions and the
//   following disclaimer.
// · Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the
//   following disclaimer in the documentation and/or other materials provided with the distribution.
// · Neither the name of nor the names of its contributors may be used to endorse or promote products derived
//   from this software without specific prior written permission.
//
// NO EXPRESS OR IMPLIED LICENSES TO ANY PARTY'S PATENT RIGHTS ARE GRANTED BY THIS LICENSE. THIS SOFTWARE IS
// PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING,
// BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
// DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
// INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
// SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
// ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
// ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
//==============================================================================
// Author(s): ="Atheros"
//==============================================================================

#include <a_config.h>
#include <a_types.h>
#include <a_osapi.h>
#include <driver_cxt.h>
#include <common_api.h>
#include <custom_wlan_api.h>
#include <wmi_api.h>
#include <bmi.h>
#include <htc.h>
#include <wmi_host.h>
#if ENABLE_P2P_MODE
#include <wmi.h>
#include "p2p.h"
#endif
#include "atheros_wifi.h"
#include "atheros_wifi_api.h"
#include "atheros_wifi_internal.h"
#include "atheros_stack_offload.h"
#include "dset_api.h"
#include "common_stack_offload.h"
#include "qcom_api.h"
//------------------------------------------------------------------------------------------------------------
qcom_wps_credentials_t gWpsCredentials;

//------------------------------------------------------------------------------------------------------------

A_STATUS custom_qapi(uint8_t device_id, uint32_t cmd, void *param);
void *Custom_Api_GetDriverCxt(uint8_t device_id);

/*FUNCTION*-------------------------------------------------------------
*
* Function Name   : qcom_set_deviceid()
* Returned Value  : A_ERROR is command failed, else A_OK
* Comments	: Sets device ID in driver context
*
*END*-----------------------------------------------------------------*/
A_STATUS qcom_set_deviceid(uint16_t id)
{
    ATH_IOCTL_PARAM_STRUCT inout_param;

    inout_param.cmd_id = ATH_SET_DEVICE_ID;
    inout_param.data = (void *)&id;
    inout_param.length = sizeof(uint16_t);

    return (custom_qapi(id, ATH_SET_DEVICE_ID, &inout_param));
}

#if 0
static enum p2p_wps_method qcom_p2p_get_wps(P2P_WPS_METHOD wps)
{
    switch (wps) {
        case P2P_WPS_NOT_READY:
            return WPS_NOT_READY;
        case P2P_WPS_PIN_LABEL:
            return WPS_PIN_LABEL;
        case P2P_WPS_PIN_DISPLAY:
            return WPS_PIN_DISPLAY;
        case P2P_WPS_PIN_KEYPAD:
            return WPS_PIN_KEYPAD;
        case P2P_WPS_PBC:
            return WPS_PBC;
        default:
            return WPS_NOT_READY;
    }
}
#endif

//------------------------------------------------------------------------------------------------------------

#if ENABLE_STACK_OFFLOAD

int qcom_socket(int family, int type, int protocol)
{
    return (t_socket(Custom_Api_GetDriverCxt(0), family, type, protocol));
}

int qcom_connect(int s, struct sockaddr *addr, int addrlen)
{
    return (t_connect(Custom_Api_GetDriverCxt(0), s, addr, addrlen));
}

int qcom_bind(int s, struct sockaddr *addr, int addrlen)
{
    return (t_bind(Custom_Api_GetDriverCxt(0), s, addr, addrlen));
}

int qcom_listen(int s, int backlog)
{
    return (t_listen(Custom_Api_GetDriverCxt(0), s, backlog));
}

int qcom_accept(int s, struct sockaddr *addr, socklen_t *addrlen)
{
    return (t_accept(Custom_Api_GetDriverCxt(0), s, addr, *addrlen));
}

int qcom_setsockopt(int s, int level, int name, void *arg, int arglen)
{
    return (t_setsockopt(Custom_Api_GetDriverCxt(0), s, level, name, arg, arglen));
}

int qcom_getsockopt(int s, int level, int name, void *arg, int arglen)
{
    return (t_getsockopt(Custom_Api_GetDriverCxt(0), s, level, name, arg, arglen));
}

#if ZERO_COPY
int qcom_recv(int s, char **buf, int len, int flags)
#else
int qcom_recv(int s, char *buf, int len, int flags)
#endif
{
    return (t_recv(Custom_Api_GetDriverCxt(0), s, (void *)buf, len, flags));
}

#if ZERO_COPY
int qcom_recvfrom(int s, char **buf, int len, int flags, struct sockaddr *from, socklen_t *fromlen)
#else
int qcom_recvfrom(int s, char *buf, int len, int flags, struct sockaddr *from, socklen_t *fromlen)
#endif
{
    return (t_recvfrom(Custom_Api_GetDriverCxt(0), s, (void *)buf, len, flags, from, (socklen_t*)fromlen));
}

#if ENABLE_DNS_CLIENT
A_STATUS qcom_dnsc_enable(boolean enable)
{
    return ((A_STATUS)custom_ip_dns_client(Custom_Api_GetDriverCxt(0), enable));
}

A_STATUS qcom_dnsc_add_server_address(uint8_t *ipaddress, uint8_t type)
{
    IP46ADDR dnsaddr;

    memset(&dnsaddr, 0, sizeof(IP46ADDR));
    if (type == ATH_AF_INET)
    {
        dnsaddr.type = ATH_AF_INET;
        dnsaddr.addr4 = (uint32_t) * (uint32_t *)ipaddress;
    }
    else
    {
        dnsaddr.type = ATH_AF_INET6;
        memcpy(&dnsaddr.addr6, ipaddress, sizeof(IP6_ADDR_T));
    }

    return ((A_STATUS)custom_ip_dns_server_addr(Custom_Api_GetDriverCxt(0), &dnsaddr));
}

A_STATUS qcom_dnsc_del_server_address(uint8_t *ipaddress, uint8_t type)
{
    IP46ADDR dnsaddr;
#define DELETE_DNS_SERVER_ADDR 1

    memset(&dnsaddr, 0, sizeof(IP46ADDR));
    dnsaddr.au1Rsvd[0] = DELETE_DNS_SERVER_ADDR;
    if (type == ATH_AF_INET)
    {
        dnsaddr.type = ATH_AF_INET;
        dnsaddr.addr4 = (uint32_t) * (uint32_t *)ipaddress;
    }
    else
    {
        dnsaddr.type = ATH_AF_INET6;
        memcpy(&dnsaddr.addr6, ipaddress, sizeof(IP6_ADDR_T));
    }

    return ((A_STATUS)custom_ip_dns_server_addr(Custom_Api_GetDriverCxt(0), &dnsaddr));
}

A_STATUS qcom_dnsc_get_host_by_name(char *pname, uint32_t *pipaddress)
{
    DNC_CFG_CMD DnsCfg;
    DNC_RESP_INFO DnsRespInfo;

    memset(&DnsRespInfo, 0, sizeof(DnsRespInfo));
    strcpy((char *)DnsCfg.ahostname, pname);
    DnsCfg.domain = 2;
    DnsCfg.mode = GETHOSTBYNAME;
    if (A_OK != custom_ip_resolve_hostname(Custom_Api_GetDriverCxt(0), &DnsCfg, &DnsRespInfo))
    {
        return A_ERROR;
    }
    *pipaddress = A_CPU2BE32(DnsRespInfo.ipaddrs_list[0]);
    return A_OK;
}

A_STATUS qcom_dnsc_get_host_by_name2(char *pname, uint32_t *pipaddress, int32_t domain, uint32_t mode)
{
    DNC_CFG_CMD DnsCfg;
    DNC_RESP_INFO DnsRespInfo;

    memset(&DnsRespInfo, 0, sizeof(DnsRespInfo));
    strcpy((char *)DnsCfg.ahostname, pname);
    DnsCfg.domain = domain;
    DnsCfg.mode = mode;
    if (A_OK != custom_ip_resolve_hostname(Custom_Api_GetDriverCxt(0), &DnsCfg, &DnsRespInfo))
    {
        return A_ERROR;
    }

    if (domain == 3)
        memcpy(pipaddress, (void*)&DnsRespInfo.ip6addrs_list[0], 16);
    else
        memcpy(pipaddress, (void*)&DnsRespInfo.ipaddrs_list[0], 4);
    return A_OK;
}

#endif
#if ENABLE_DNS_SERVER
A_STATUS qcom_dns_server_address_get(uint32_t pdns[], uint32_t *number)
{
    int i;
    A_STATUS error;
    IP46ADDR dnsaddr[MAX_DNSADDRS];

    memset(&dnsaddr[0], 0, sizeof(dnsaddr));
    error = t_ipconfig(Custom_Api_GetDriverCxt(0), IPCFG_QUERY, NULL, NULL, NULL, dnsaddr, NULL);
    *number = 0;

    if (error == A_OK)
    {
        for (i = 0; i < MAX_DNSADDRS; i++)
        {
            if (dnsaddr[i].addr4 != 0)
            {
                pdns[i] = A_CPU2BE32(dnsaddr[i].addr4);
                *number = *number + 1;
            }
            else
            {
                break;
            }
        }
    }

    return error;
}

A_STATUS qcom_dnss_enable(boolean enable)
{
    return ((A_STATUS)custom_ip_dns_server(Custom_Api_GetDriverCxt(0), (uint32_t)enable));
}

A_STATUS qcom_dns_local_domain(char *local_domain)
{
    return ((A_STATUS)custom_ip_dns_local_domain(Custom_Api_GetDriverCxt(0), local_domain));
}

A_STATUS qcom_dns_entry_create(char *hostname, uint32_t ipaddress)
{
    IP46ADDR dnsaddr = {0};
    dnsaddr.type = ATH_AF_INET;
    dnsaddr.addr4 = ipaddress;
    return ((A_STATUS)custom_ipdns(Custom_Api_GetDriverCxt(0), 1, hostname, &dnsaddr));
}

A_STATUS qcom_dns_entry_delete(char *hostname, uint32_t ipaddress)
{
    IP46ADDR dnsaddr = {0};
    dnsaddr.type = ATH_AF_INET;
    dnsaddr.addr4 = ipaddress;
    return ((A_STATUS)custom_ipdns(Custom_Api_GetDriverCxt(0), 2, hostname, &dnsaddr));
}

A_STATUS qcom_dns_6entry_create(char *hostname, IP6_ADDR_T *ip6addr)
{
    IP46ADDR dnsaddr = {0};
    dnsaddr.type = ATH_AF_INET6;
    memcpy(&dnsaddr.addr6, ip6addr, sizeof(dnsaddr.addr6));
    return ((A_STATUS)custom_ipdns(Custom_Api_GetDriverCxt(0), 1, hostname, &dnsaddr));
}

A_STATUS qcom_dns_6entry_delete(char *hostname, IP6_ADDR_T *ip6addr)
{
    IP46ADDR dnsaddr = {0};
    dnsaddr.type = ATH_AF_INET6;
    memcpy(&dnsaddr.addr6, ip6addr, sizeof(dnsaddr.addr6));
    return ((A_STATUS)custom_ipdns(Custom_Api_GetDriverCxt(0), 2, hostname, &dnsaddr));
}

#endif

#if ENABLE_SNTP_CLIENT
void qcom_sntp_srvr_addr(int flag, char *srv_addr)
{
    custom_ip_sntp_srvr_addr(Custom_Api_GetDriverCxt(0), flag, srv_addr);
}

void qcom_sntp_get_time(uint8_t device_id, tSntpTime *time)
{
    if (A_OK != qcom_set_deviceid(device_id))
    {
        return;
    }
    custom_ip_sntp_get_time(Custom_Api_GetDriverCxt(device_id), time);
}

void qcom_sntp_get_time_of_day(uint8_t device_id, tSntpTM *time)
{
    if (A_OK != qcom_set_deviceid(device_id))
    {
        return;
    }
    custom_ip_sntp_get_time_of_day(Custom_Api_GetDriverCxt(device_id), time);
}
void qcom_sntp_zone(int hour, int min, int add_sub, int enable)
{
    custom_ip_sntp_modify_zone_dse(Custom_Api_GetDriverCxt(0), hour, min, add_sub, enable);
}

void qcom_sntp_query_srvr_address(uint8_t device_id, tSntpDnsAddr *addr)
{
    if (A_OK != qcom_set_deviceid(device_id))
    {
        return;
    }
    custom_ip_sntp_query_srvr_address(Custom_Api_GetDriverCxt(device_id), addr);
}

void qcom_enable_sntp_client(int enable)
{
    custom_ip_sntp_client(Custom_Api_GetDriverCxt(0), enable);
}

#endif

/****************QCOM_OTA_APIs************************/

A_STATUS qcom_ota_upgrade(uint8_t device_id,
                          uint32_t addr,
                          char *filename,
                          uint8_t mode,
                          uint8_t preserve_last,
                          uint8_t protocol,
                          uint32_t *resp_code,
                          uint32_t *length)
{
    return ((A_STATUS)custom_ota_upgrade(Custom_Api_GetDriverCxt(device_id), addr, filename, mode, preserve_last,
                                         protocol, resp_code, length));
}

A_STATUS qcom_ota_read_area(uint32_t offset, uint32_t size, uint8_t *buffer, uint32_t *retlen)
{
    return ((A_STATUS)custom_ota_read_area(Custom_Api_GetDriverCxt(0), offset, size, buffer, retlen));
}

A_STATUS qcom_ota_done(boolean good_image)
{
    return ((A_STATUS)custom_ota_done(Custom_Api_GetDriverCxt(0), good_image));
}

QCOM_OTA_STATUS_CODE_t qcom_ota_session_start(uint32_t flags, uint32_t partition_index)
{
    return ((QCOM_OTA_STATUS_CODE_t)custom_ota_session_start(Custom_Api_GetDriverCxt(0), flags, partition_index));
}

uint32_t qcom_ota_partition_get_size(void)
{
    return (custom_ota_partition_get_size(Custom_Api_GetDriverCxt(0)));
}

QCOM_OTA_STATUS_CODE_t qcom_ota_partition_erase(void)
{
    return ((QCOM_OTA_STATUS_CODE_t)custom_ota_partition_erase(Custom_Api_GetDriverCxt(0)));
}

QCOM_OTA_STATUS_CODE_t qcom_ota_partition_verify_checksum(void)
{
    return ((QCOM_OTA_STATUS_CODE_t)custom_ota_partition_verify_checksum(Custom_Api_GetDriverCxt(0)));
}

QCOM_OTA_STATUS_CODE_t qcom_ota_parse_image_hdr(uint8_t *header, uint32_t *offset)
{
    return ((QCOM_OTA_STATUS_CODE_t)custom_ota_parse_image_hdr(Custom_Api_GetDriverCxt(0), header, offset));
}

uint32_t qcom_ota_partition_read_data(uint32_t offset, uint8_t *buf, uint32_t size)
{
    uint32_t rtn_len = 0;
    if (qcom_ota_read_area(offset, size, buf, &rtn_len) != A_OK)
    {
        if (rtn_len > 0)
        {
            rtn_len = rtn_len;
        }
    }
    return (rtn_len);
}

QCOM_OTA_STATUS_CODE_t qcom_ota_partition_write_data(uint32_t offset, uint8_t *buf, uint32_t size, uint32_t *ret_size)
{
    return ((QCOM_OTA_STATUS_CODE_t)custom_ota_partition_write_data(Custom_Api_GetDriverCxt(0), offset, buf, size,
                                                                    ret_size));
}

QCOM_OTA_STATUS_CODE_t qcom_ota_session_end(uint32_t good_image)
{
    return (QCOM_OTA_STATUS_CODE_t)qcom_ota_done(good_image);
}

QCOM_OTA_STATUS_CODE_t qcom_ota_partition_format(uint32_t partition_index)
{
    QCOM_OTA_STATUS_CODE_t rtn;

    if (partition_index == 0)
        return QCOM_OTA_ERR_INVALID_PARTITION_INDEX;

    if ((rtn = qcom_ota_session_start(QCOM_OTA_ERASING_RW_DSET, partition_index)) == QCOM_OTA_OK)
    {
        rtn = (QCOM_OTA_STATUS_CODE_t)qcom_ota_partition_erase();
        qcom_ota_session_end(0);
    }
    return rtn;
}

A_STATUS qcom_ota_set_callback(void *callback)
{
    return ((A_STATUS)custom_ota_set_response_cb(Custom_Api_GetDriverCxt(0), callback));
}

int qcom_sendto(int s, char *buffer, int length, int flags, struct sockaddr *to, int tolen)
{
    return (t_sendto(Custom_Api_GetDriverCxt(0), s, (uint8_t *)buffer, length, flags, to, tolen));
}

int qcom_send(int s, char *buffer, int length, int flags)
{
    return (t_send(Custom_Api_GetDriverCxt(0), s, (uint8_t *)buffer, length, flags));
}

int qcom_socket_close(int s)
{
    return (t_shutdown(Custom_Api_GetDriverCxt(0), s));
}

A_STATUS qcom_ipconfig(uint8_t device_id, QCOM_IPCONFIG_MODE mode, uint32_t *address, uint32_t *submask, uint32_t *gateway)
{
    if (A_OK != qcom_set_deviceid(device_id))
    {
        return A_ERROR;
    }
    return (t_ipconfig(Custom_Api_GetDriverCxt(device_id), mode, address, submask, gateway, NULL, NULL));
}

A_STATUS qcom_ip6_address_get(uint8_t device_id,
                              IP6_ADDR_T *v6Global,
                              IP6_ADDR_T *v6Link,
                              IP6_ADDR_T *v6DefGw,
                              IP6_ADDR_T *v6GlobalExtd,
                              int32_t *LinkPrefix,
                              int32_t *GlbPrefix,
                              int32_t *DefgwPrefix,
                              int32_t *GlbPrefixExtd)
{
    if (A_OK != qcom_set_deviceid(device_id))
    {
        return A_ERROR;
    }
    return (t_ip6config(Custom_Api_GetDriverCxt(device_id), IPCFG_QUERY, (IP6_ADDR_T *)v6Global, (IP6_ADDR_T *)v6Link,
                        (IP6_ADDR_T *)v6DefGw, (IP6_ADDR_T *)v6GlobalExtd, LinkPrefix, GlbPrefix, DefgwPrefix,
                        GlbPrefixExtd));
}

A_STATUS qcom_ping(uint32_t host, uint32_t size)
{
    return (t_ping(Custom_Api_GetDriverCxt(0), host, size, COMMAND_BLOCK_TIMEOUT));
}

A_STATUS qcom_ping_ms(uint32_t host, uint32_t size, uint32_t ms_interval)
{
    return (t_ping(Custom_Api_GetDriverCxt(0), host, size, ms_interval));
}

A_STATUS qcom_ping6(uint8_t *host, uint32_t size)
{
    return (t_ping6(Custom_Api_GetDriverCxt(0), host, size, COMMAND_BLOCK_TIMEOUT));
}

A_STATUS qcom_ping6_ms(uint8_t *host, uint32_t size, uint32_t ms_interval)
{
    return (t_ping6(Custom_Api_GetDriverCxt(0), host, size, ms_interval));
}

A_STATUS qcom_ip6config_router_prefix(
    uint8_t device_id, IP6_ADDR_T *addr, int32_t prefixlen, int32_t prefix_lifetime, int32_t valid_lifetime)
{
    if (A_OK != qcom_set_deviceid(device_id))
    {
        return A_ERROR;
    }
    return (t_ip6config_router_prefix(Custom_Api_GetDriverCxt(0), addr, prefixlen, prefix_lifetime, valid_lifetime));
}

A_STATUS qcom_bridge_mode_enable(uint16_t bridgemode)
{
    return (custom_ipbridgemode(Custom_Api_GetDriverCxt(0), bridgemode));
}

A_STATUS qcom_tcp_set_exp_backoff(uint8_t device_id, int32_t retry)
{
    if (A_OK != qcom_set_deviceid(device_id))
    {
        return A_ERROR;
    }
    return (custom_ipconfig_set_tcp_exponential_backoff_retry(Custom_Api_GetDriverCxt(device_id), retry));
}

#if ENABLE_ROUTING_CMDS
A_STATUS qcom_ip4_route(uint8_t device_id,
                        uint32_t cmd,
                        IP_ADDR_T *addr,
                        IP_ADDR_T *subnet,
                        IP_ADDR_T *gw,
                        uint32_t *ifindex,
                        IPV4_ROUTE_LIST_T *rtlist)
{
    if (A_OK != qcom_set_deviceid(device_id))
    {
        return A_ERROR;
    }
    return (custom_ipv4_route(Custom_Api_GetDriverCxt(device_id), cmd, addr, subnet, gw, ifindex, rtlist));
}

A_STATUS qcom_ip6_route(uint8_t device_id,
                        uint32_t cmd,
                        IP6_ADDR_T *ip6addr,
                        uint32_t *prefixLen,
                        IP6_ADDR_T *gw,
                        uint32_t *ifindex,
                        IPV6_ROUTE_LIST_T *rtlist)
{
    if (A_OK != qcom_set_deviceid(device_id))
    {
        return A_ERROR;
    }
    return (custom_ipv6_route(Custom_Api_GetDriverCxt(device_id), cmd, ip6addr, prefixLen, gw, ifindex, rtlist));
}

#endif

A_STATUS qcom_tcp_conn_timeout(uint32_t timeout_val)
{
    return (custom_tcp_connection_timeout(Custom_Api_GetDriverCxt(0), timeout_val));
}

int32_t qcom_dhcps_set_pool(uint8_t device_id, uint32_t startip, uint32_t endip, int32_t leasetime)
{
    if (A_OK != qcom_set_deviceid(device_id))
    {
        return A_ERROR;
    }
    return (t_ipconfig_dhcp_pool(Custom_Api_GetDriverCxt(device_id), &startip, &endip, leasetime));
}

int32_t qcom_dhcps_register_cb(uint8_t device_id, void *callback)
{
    if (A_OK != qcom_set_deviceid(device_id))
    {
        return A_ERROR;
    }
    return (t_ipconfig_dhcps_cb_enable(Custom_Api_GetDriverCxt(device_id), callback));
}

int32_t qcom_dhcpc_register_cb(uint8_t device_id, void *callback)
{
    if (A_OK != qcom_set_deviceid(device_id))
    {
        return A_ERROR;
    }
    return (t_ipconfig_dhcpc_cb_enable(Custom_Api_GetDriverCxt(device_id), callback));
}

A_STATUS qcom_dhcps_release_pool(uint8_t device_id)
{
    if (A_OK != qcom_set_deviceid(device_id))
    {
        return A_ERROR;
    }
    return (custom_ipconfig_dhcp_release(Custom_Api_GetDriverCxt(device_id)));
}

A_STATUS qcom_http_server(int32_t enable)
{
    return (custom_ip_http_server(Custom_Api_GetDriverCxt(0), enable));
}

A_STATUS qcom_http_set_post_cb(void *cxt, void *callback)
{
    if (callback == NULL)
    {
        return A_ERROR;
    }
    return ((A_STATUS)custom_http_set_post_cb(Custom_Api_GetDriverCxt(0), cxt, callback));
}

A_STATUS qcom_ip_http_server_method(
    int32_t cmd, uint8_t *pagename, uint8_t *objname, int32_t objtype, int32_t objlen, uint8_t *value)
{
    return (custom_ip_http_server_method(Custom_Api_GetDriverCxt(0), cmd, pagename, objname, objtype, objlen, value));
}

#if ENABLE_HTTP_CLIENT
A_STATUS qcom_http_client_method(uint32_t cmd, uint8_t *url, uint8_t *data, uint8_t *value)
{
    HTTPC_RESPONSE *output = NULL;
    A_STATUS result = A_OK;

    if (NULL == value)
    {
        // for HTTPC_CONNECT or HTTPC_DISC
        result = custom_httpc_method(Custom_Api_GetDriverCxt(0), cmd, url, data, NULL);
    }
    else
    {
        result = custom_httpc_method(Custom_Api_GetDriverCxt(0), cmd, url, data, (uint8_t **)&output);
    }

    if (result == A_OK)
    {
        if (output && value)
        {
            A_MEMCPY(value, output, sizeof(HTTPC_RESPONSE) + output->length - 1);
            zero_copy_http_free((void *)output);
        }
    }

    return result;
}
#endif

#if ENABLE_SSL
SSL_CTX *qcom_SSL_ctx_new(SSL_ROLE_T role, int32_t inbufSize, int32_t outbufSize, int32_t reserved)
{
    return ((SSL_CTX *)SSL_ctx_new(role, inbufSize, outbufSize, reserved));
}

int32_t qcom_SSL_ctx_free(SSL_CTX *ctx)
{
    return (SSL_ctx_free(ctx));
}

SSL *qcom_SSL_new(SSL_CTX *ctx)
{
    return ((SSL *)SSL_new(ctx));
}

int32_t qcom_SSL_setCaList(SSL_CTX *ctx, SslCAList caList, uint32_t size)
{
    return (SSL_setCaList(ctx, caList, size));
}

int32_t qcom_SSL_addCert(SSL_CTX *ctx, SslCert cert, uint32_t size)
{
    return (SSL_addCert(ctx, cert, size));
}

int32_t qcom_SSL_storeCert(char *name, SslCert cert, uint32_t size)
{
    return (SSL_storeCert(name, cert, size));
}
int32_t qcom_SSL_loadCert(SSL_CTX *ctx, SSL_CERT_TYPE_T type, char *name)
{
    return (SSL_loadCert(ctx, type, name));
}

int32_t qcom_SSL_listCert(SSL_FILE_NAME_LIST *fileNames)
{
    return (SSL_listCert(fileNames));
}

int32_t qcom_SSL_set_fd(SSL *ssl, uint32_t fd)
{
    return (SSL_set_fd(ssl, fd));
}

int32_t qcom_SSL_accept(SSL *ssl)
{
    return (SSL_accept(ssl));
}

int32_t qcom_SSL_connect(SSL *ssl)
{
    return (SSL_connect(ssl));
}

int32_t qcom_SSL_shutdown(SSL *ssl)
{
    return (SSL_shutdown(ssl));
}

int32_t qcom_SSL_configure(SSL *ssl, SSL_CONFIG *cfg)
{
    return (SSL_configure(ssl, cfg));
}

#if ZERO_COPY
int32_t qcom_SSL_read(SSL *ssl, void **buf, int32_t num)
#else
int32_t qcom_SSL_read(SSL *ssl, void *buf, int32_t num)
#endif
{
    return (SSL_read(ssl, buf, num));
}
int32_t qcom_SSL_write(SSL *ssl, const void *buf, int32_t num)
{
    return (SSL_write(ssl, buf, num));
}

#endif // ENABLE_SSL
#endif

/****************************************************************************************/
A_STATUS _qcom_set_scan(uint8_t device_id, qcom_start_scan_params_t *pScanParams)
{
    uint32_t size;
    WMI_START_SCAN_CMD *pStartScan;
    A_STATUS error = A_OK;
    uint8_t *buffer = NULL;

    if (A_OK != qcom_set_deviceid(device_id))
    {
        return A_ERROR;
    }

    if (pScanParams == NULL)
    {
        pStartScan = NULL;
    }
    else
    {
        size = sizeof(WMI_START_SCAN_CMD);
        if (pScanParams->numChannels > 0)
        {
            size = size + (pScanParams->numChannels - 1) * sizeof(uint16_t);
        }

        if (NULL == (buffer = A_MALLOC(size, MALLOC_ID_CONTEXT)))
        {
            return A_ERROR;
        }

        pStartScan = (WMI_START_SCAN_CMD *)buffer;
        A_MEMZERO(pStartScan, size);
        pStartScan->forceFgScan = pScanParams->forceFgScan;
        pStartScan->isLegacy = false;
        pStartScan->homeDwellTime = pScanParams->homeDwellTimeInMs;
        pStartScan->forceScanInterval = pScanParams->forceScanIntervalInMs;
        pStartScan->scanType = pScanParams->scanType;
        pStartScan->numChannels = pScanParams->numChannels;
        A_MEMCPY((uint8_t *)pStartScan->channelList, (uint8_t *)pScanParams->channelList,
                 pScanParams->numChannels * sizeof(uint16_t));
    }
    if (custom_qapi(device_id, ATH_SET_SCAN, (void *)pStartScan) != A_OK)
        error = A_ERROR;

    if (buffer)
        A_FREE(buffer, MALLOC_ID_CONTEXT);
    return error;
}

A_STATUS qcom_get_scan(uint8_t device_id, QCOM_BSS_SCAN_INFO **buf, int16_t *numResults)
{
    ATH_SCAN_LIST param = {0};

    if (A_OK != qcom_set_deviceid(device_id))
    {
        return A_ERROR;
    }

    if (custom_qapi(device_id, ATH_GET_SCAN_RESULTS, &param) != A_OK)
    {
        return A_ERROR;
    }
    *buf = (QCOM_BSS_SCAN_INFO *)param.scan_list;
    *numResults = param.num_scan_entries;

    return A_OK;
}

/* NOTE: Passing NULL to pssid is not longer supported. Use empty
 * string "" instead */
A_STATUS qcom_set_ssid(uint8_t device_id, QCOM_SSID *pssid)
{
    ATH_IOCTL_PARAM_STRUCT param = {0};
    if (NULL == pssid)
    {
        return A_ERROR;
    }
    if (A_OK != qcom_set_deviceid(device_id))
    {
        return A_ERROR;
    }
    //TODO: check sizeof vs strlen ?
    param.data = pssid;
    if (A_OK != custom_qapi(device_id, ATH_SET_SSID, &param))
    {
        return A_ERROR;
    }
    return A_OK;
}

A_STATUS qcom_get_ssid(uint8_t device_id, QCOM_SSID *pssid)
{
    ATH_IOCTL_PARAM_STRUCT param = {0};
    uint32_t null_position;
    if (NULL == pssid)
    {
        return A_ERROR;
    }
    if (A_OK != qcom_set_deviceid(device_id))
    {
        return A_ERROR;
    }
    param.data = (void *)pssid;
    if (A_OK != custom_qapi(device_id, ATH_GET_ESSID, &param))
    {
        return A_ERROR;
    }
    /* terminate SSID string */
    null_position = param.length < sizeof(QCOM_SSID) ? param.length : sizeof(QCOM_SSID) - 1;
    pssid->ssid[null_position] = '\0';
    return A_OK;
}

A_STATUS qcom_set_connect_callback(uint8_t device_id, void *callback)
{
    ATH_IOCTL_PARAM_STRUCT inout_param;
    A_STATUS error = A_OK;

    if (A_OK != qcom_set_deviceid(device_id))
    {
        return A_ERROR;
    }
    inout_param.cmd_id = ATH_SET_CONNECT_STATE_CALLBACK;
    inout_param.data = (void *)callback;
    inout_param.length = sizeof(void *);

    if (custom_qapi(device_id, ATH_SET_CONNECT_STATE_CALLBACK, &inout_param) != A_OK)
    {
        error = A_ERROR;
    }
    return error;
}

A_STATUS qcom_set_probe_req_callback(uint8_t device_id, void *callback)
{
    ATH_IOCTL_PARAM_STRUCT inout_param;
    A_STATUS error = A_OK;
    if (A_OK != qcom_set_deviceid(device_id))
    {
        return A_ERROR;
    }
    inout_param.cmd_id = ATH_SET_PROBEREQ_CB;
    inout_param.data = (void *)callback;
    inout_param.length = sizeof(void *);

    if (custom_qapi(device_id, ATH_SET_PROBEREQ_CB, &inout_param) != A_OK)
    {
        error = A_ERROR;
    }
    return error;
}

A_STATUS qcom_enable_probe_req_event(uint8_t device_id, uint8_t enable)
{
    ATH_IOCTL_PARAM_STRUCT inout_param;
    A_STATUS error = A_OK;
    if (A_OK != qcom_set_deviceid(device_id))
    {
        return A_ERROR;
    }
    inout_param.cmd_id = ATH_SET_PROBEREQ_EV_ENABLE;
    inout_param.data = (void *)&enable;
    inout_param.length = sizeof(void *);

    if (custom_qapi(device_id, ATH_SET_PROBEREQ_EV_ENABLE, &inout_param) != A_OK)
    {
        error = A_ERROR;
    }
    return error;
}

#if ENABLE_SCC_MODE
static A_STATUS qcom_get_concurrent_channel(uint8_t device_id, uint16_t *conc_channel)
{
    ATH_IOCTL_PARAM_STRUCT ath_param = {0};
    A_STATUS error = A_OK;

    if (A_OK != qcom_set_deviceid(device_id))
    {
        return A_ERROR;
    }
    ath_param.cmd_id = ATH_GET_CONC_DEV_CHANNEL;
    ath_param.data = conc_channel;

    if (custom_qapi(device_id, ATH_GET_CONC_DEV_CHANNEL, &ath_param) != A_OK)
    {
        error = A_ERROR;
    }
    return error;
}

static A_STATUS qcom_get_channelhint(uint8_t device_id, uint16_t *pchannel)
{
    ATH_IOCTL_PARAM_STRUCT inout_param = {0};
    A_STATUS error = A_OK;

    if (A_OK != qcom_set_deviceid(device_id))
    {
        return A_ERROR;
    }
    inout_param.cmd_id = ATH_GET_CHANNELHINT;
    inout_param.data = pchannel;

    if (custom_qapi(device_id, ATH_GET_CHANNELHINT, &inout_param) != A_OK)
    {
        error = A_ERROR;
    }
    return error;
}

#endif

A_STATUS qcom_commit(uint8_t device_id)
{
#if ENABLE_SCC_MODE
    uint16_t conc_channel, cur_channel;
    QCOM_SSID ssid;
    int num_dev = WLAN_NUM_OF_DEVICES;

    if (qcom_get_ssid(device_id, &ssid) != A_OK)
        return A_ERROR;

    /*this is connect request */
    if ((strlen(ssid.ssid) != 0) && (num_dev > 1))
    {
        qcom_get_channelhint(device_id, &cur_channel);
        qcom_get_concurrent_channel(device_id, &conc_channel);
        /* if device 1 is connected and this connect request is from device 0,
           check both devices whether stay at same channel, if not, return fail due to device 1 is primary
        */
        if ((conc_channel != 0) && (device_id == 0) && (conc_channel != cur_channel))
        {
            return A_ERROR;
        }
        /* if device 0 is connected and this connect request is from device 1,
           check both devices whether stay at same channel, if not, p2p_cancel is issue to device 0 because device 1 is
           promary
        */
        if ((conc_channel != 0) && (device_id == 1) && (conc_channel != cur_channel))
        {
            qcom_set_deviceid(0);
            qcom_set_channel(0, conc_channel);
            A_MDELAY(50);
            qcom_p2p_func_cancel(0);
            A_MDELAY(500);
            qcom_set_deviceid(1);
            qcom_set_channel(1, cur_channel);
            A_MDELAY(50);
        }
    }

    if (A_OK != qcom_set_deviceid(device_id))
    {
        return A_ERROR;
    }

    return (custom_qapi(device_id, ATH_SET_COMMIT, NULL));

#else
    if (A_OK != qcom_set_deviceid(device_id))
    {
        return A_ERROR;
    }

    return (custom_qapi(device_id, ATH_SET_COMMIT, NULL));
#endif // ENABLE_SCC_MODE
}

A_STATUS qcom_sta_get_rssi(uint8_t device_id, uint8_t *prssi)
{
    A_STATUS error = A_OK;
    ATH_IOCTL_PARAM_STRUCT inout_param;

    if (A_OK != qcom_set_deviceid(device_id))
    {
        return A_ERROR;
    }
    inout_param.cmd_id = ATH_GET_RX_RSSI;
    inout_param.data = prssi;

    if (custom_qapi(device_id, ATH_GET_RX_RSSI, &inout_param) != A_OK)
    {
        error = A_ERROR;
    }
    return error;
}

/* for hostless , the listentime is 1Tus, 1Tus = 1024 us. need to confirm the UNIT */
A_STATUS qcom_sta_set_listen_time(uint8_t device_id, uint32_t listentime)
{
    ATH_IOCTL_PARAM_STRUCT inout_param;
    A_STATUS error = A_OK;

    if (A_OK != qcom_set_deviceid(device_id))
    {
        return A_ERROR;
    }

    inout_param.cmd_id = ATH_SET_LISTEN_INT;
    inout_param.data = &listentime;
    inout_param.length = 4;

    if (custom_qapi(device_id, ATH_SET_LISTEN_INT, &inout_param) != A_OK)
    {
        error = A_ERROR;
    }
    return error;
}

A_STATUS qcom_ap_set_beacon_interval(uint8_t device_id, uint16_t beacon_interval)
{
    ATH_IOCTL_PARAM_STRUCT inout_param = {0};
    ATH_AP_PARAM_STRUCT ap_param;
    A_STATUS error = A_OK;

    if (A_OK != qcom_set_deviceid(device_id))
    {
        return A_ERROR;
    }
    inout_param.cmd_id = ATH_CONFIG_AP;
    ap_param.cmd_subset = AP_SUB_CMD_BCON_INT;
    ap_param.data = &beacon_interval;
    inout_param.data = &ap_param;

    if (custom_qapi(device_id, ATH_CONFIG_AP, &inout_param) != A_OK)
    {
        error = A_ERROR;
    }
    return error;
}

A_STATUS qcom_ap_hidden_mode_enable(uint8_t device_id, boolean enable)
{
    ATH_IOCTL_PARAM_STRUCT inout_param = {0};
    ATH_AP_PARAM_STRUCT ap_param;
    A_STATUS error = A_OK;

    if (A_OK != qcom_set_deviceid(device_id))
    {
        return A_ERROR;
    }

    inout_param.cmd_id = ATH_CONFIG_AP;
    ap_param.cmd_subset = AP_SUB_CMD_HIDDEN_FLAG;
    ap_param.data = &enable;
    inout_param.data = &ap_param;

    if (custom_qapi(device_id, ATH_CONFIG_AP, &inout_param) != A_OK)
    {
        error = A_ERROR;
    }
    return error;
}

//TODO: always check return value in place of call
A_STATUS qcom_op_get_mode(uint8_t device_id, QCOM_WLAN_DEV_MODE *pmode)
{
    uint32_t inout_param = 0;

    if (A_OK != qcom_set_deviceid(device_id))
    {
        return A_ERROR;
    }

    if (custom_qapi(device_id, ATH_GET_OP_MODE, &inout_param) != A_OK)
    {
        return A_ERROR;
    }
    *pmode = (QCOM_WLAN_DEV_MODE)inout_param;

    return A_OK;
}

A_STATUS qcom_op_set_mode(uint8_t device_id, QCOM_WLAN_DEV_MODE mode)
{
    uint32_t inout_param = 0;

    if (A_OK != qcom_set_deviceid(device_id))
    {
        return A_ERROR;
    }
    inout_param = mode;

    if (A_OK != custom_qapi(device_id, ATH_SET_OP_MODE, &inout_param))
    {
        return A_ERROR;
    }

    return A_OK;
}

A_STATUS qcom_disconnect(uint8_t device_id)
{
    A_STATUS error = A_OK;
    QCOM_SSID tmp_ssid = QCOM_SSID_FROM_STR("");

    if (A_OK != qcom_set_deviceid(device_id))
    {
        return A_ERROR;
    }
    /* set ssid to be blank string , ssid length is 0 , the fist step to disconnectl*/
    if (qcom_set_ssid(device_id, &tmp_ssid) != A_OK)
    {
        return A_ERROR;
    }
    /* a zero length ssid + a COMMIT command is interpreted as a
     request from the caller to disconnect.*/
    if (qcom_commit(device_id) != A_OK)
    {
        return A_ERROR;
    }
    return error;
}

static const char qcom_phy_mode_a[] = "a";
static const char qcom_phy_mode_g[] = "g";
static const char qcom_phy_mode_b[] = "b";
static const char qcom_phy_mode_mixed[] = "mixed";

A_STATUS qcom_get_phy_mode(uint8_t device_id, char **pphymode)
{
    ATH_IOCTL_PARAM_STRUCT inout_param;
    uint32_t value = 0;

    if (A_OK != qcom_set_deviceid(device_id))
    {
        return A_ERROR;
    }
    inout_param.cmd_id = ATH_GET_PHY_MODE;
    inout_param.data = &value;

    if (custom_qapi(device_id, ATH_GET_PHY_MODE, &inout_param) != A_OK)
    {
        return A_ERROR;
    }

    if (*(uint32_t *)(inout_param.data) == 0x01)
    {
        *pphymode = (char*)qcom_phy_mode_a;
    }
    else if (*(uint32_t *)(inout_param.data) == 0x02)
    {
        *pphymode = (char*)qcom_phy_mode_g;
    }
    else if (*(uint32_t *)(inout_param.data) == 0x04)
    {
        *pphymode = (char*)qcom_phy_mode_b;
    }
    else
    {
        *pphymode = (char*)qcom_phy_mode_mixed;
    }
    return A_OK;
}

A_STATUS qcom_get_country_code(uint8_t device_id, uint8_t *countryCode)
{
    ATH_IOCTL_PARAM_STRUCT inout_param;
    WMI_GET_COUNTRY_CODE_REPLY para_countryCode;

    if (A_OK != qcom_set_deviceid(device_id))
    {
        return A_ERROR;
    }

    inout_param.cmd_id = ATH_GET_COUNTRY_CODE;
    inout_param.data = (uint8_t *)&para_countryCode;
    inout_param.length = sizeof(para_countryCode);

    if (custom_qapi(device_id, ATH_GET_COUNTRY_CODE, &inout_param) != A_OK)
    {
        return A_ERROR;
    }
    A_MEMCPY(countryCode, para_countryCode.country_code, 3);

    return A_OK;
}

A_STATUS qcom_set_country_code(uint8_t device_id, uint8_t *countryCode)
{
    ATH_IOCTL_PARAM_STRUCT inout_param;
    ATH_PROGRAM_COUNTRY_CODE_PARAM data;

    if (A_OK != qcom_set_deviceid(device_id))
    {
        return A_ERROR;
    }

    A_MEMZERO(data.countryCode, 3);
    A_MEMCPY(data.countryCode, countryCode, 3);

    inout_param.cmd_id = ATH_PROGRAM_COUNTRY_CODE;
    inout_param.length = sizeof(data);
    inout_param.data = (uint8_t *)&data;

    if (custom_qapi(device_id, ATH_PROGRAM_COUNTRY_CODE, &inout_param) != A_OK)
    {
        QCADRV_PRINTF("countryCode PROGRAM ERROR: unknown driver error\n");
        return A_ERROR;
    }
    else
    {
        switch (data.result)
        {
            case ATH_PROGRAM_COUNTRY_CODE_RESULT_DRIVER_FAILED:
                QCADRV_PRINTF("COUNTRY_CODE PROGRAM ERROR: the driver was unable to complete the request.\n");
                break;
            case ATH_PROGRAM_COUNTRY_CODE_RESULT_SUCCESS:
                QCADRV_PRINTF("COUNTRY_CODE PROGRAM SUCCESS.\n");
                break;
            case ATH_PROGRAM_COUNTRY_CODE_RESULT_DEV_DENIED:
                QCADRV_PRINTF("COUNTRY_CODE PROGRAM ERROR: the firmware failed to program the country_code.\n");
                QCADRV_PRINTF(" possibly the same COUNTRY_CODEis already programmed.\n");
                break;
            case ATH_PROGRAM_COUNTRY_CODE_RESULT_DEV_FAILED:
            default:
                QCADRV_PRINTF("COUNTRY_CODE PROGRAM ERROR: Device unknown failure.\n");
                break;
        }
    }

    return A_OK;
}
A_STATUS qcom_set_phy_mode(uint8_t device_id, uint8_t phymode)
{
#define BUF_LEN 20
    ATH_IOCTL_PARAM_STRUCT inout_param;
    A_STATUS error = A_OK;
    char p[BUF_LEN];

    if (A_OK != qcom_set_deviceid(device_id))
    {
        return A_ERROR;
    }
    if (phymode == QCOM_11A_MODE)
        strcpy(p, "a");
    else if (phymode == QCOM_11B_MODE)
        strcpy(p, "b");
    else if (phymode == QCOM_11G_MODE)
        strcpy(p, "g");
    else if (phymode == QCOM_11N_MODE)
        strcpy(p, "n");
    else if (phymode == QCOM_HT40_MODE)
        strcpy(p, "ht40");

    inout_param.cmd_id = ATH_SET_PHY_MODE;
    inout_param.data = p;
    inout_param.length = strlen(p);

    if (custom_qapi(device_id, ATH_SET_PHY_MODE, &inout_param) != A_OK)
    {
        error = A_ERROR;
    }
    return error;
}

A_STATUS qcom_set_channel(uint8_t device_id, uint16_t channel)
{
    ATH_IOCTL_PARAM_STRUCT inout_param;
    A_STATUS error = A_OK;

    if (A_OK != qcom_set_deviceid(device_id))
    {
        return A_ERROR;
    }

    if (channel < 1 || channel > 165)
    {
        return A_ERROR;
    }
    if (channel < 27)
    {
        channel = ATH_IOCTL_FREQ_1 + (channel - 1) * 5;
    }
    else
    {
        channel = (5000 + (channel * 5));
    }

    inout_param.cmd_id = ATH_SET_CHANNEL;
    inout_param.data = &channel;
    inout_param.length = 4;

    if (custom_qapi(device_id, ATH_SET_CHANNEL, &inout_param) != A_OK)
    {
        error = A_ERROR;
    }
    return error;
}

A_STATUS qcom_get_channel(uint8_t device_id, uint16_t *pchannel)
{
    ATH_IOCTL_PARAM_STRUCT inout_param = {0};
    A_STATUS error = A_OK;

    if (A_OK != qcom_set_deviceid(device_id))
    {
        return A_ERROR;
    }
    inout_param.cmd_id = ATH_GET_CHANNEL;
    inout_param.data = pchannel;

    if (custom_qapi(device_id, ATH_GET_CHANNEL, &inout_param) != A_OK)
    {
        error = A_ERROR;
    }
    return error;
}

A_STATUS qcom_get_tx_power(uint8_t device_id, uint32_t *pdbm)
{
    /* No command id for get tx_power */
    return A_OK;
}

A_STATUS qcom_set_tx_power(uint8_t device_id, uint32_t dbm)
{
    ATH_IOCTL_PARAM_STRUCT inout_param;
    A_STATUS error = A_OK;
    uint8_t pdBm;

    if (A_OK != qcom_set_deviceid(device_id))
    {
        return A_ERROR;
    }
    pdBm = (uint8_t)dbm;
    inout_param.cmd_id = ATH_SET_TXPWR;
    inout_param.data = &pdBm;
    inout_param.length = 1; // strlen(dBm);

    if (custom_qapi(device_id, ATH_SET_TXPWR, &inout_param) != A_OK)
    {
        error = A_ERROR;
    }
    return error;
}

A_STATUS qcom_allow_aggr_set_tid(uint8_t device_id, uint16_t tx_allow_aggr, uint16_t rx_allow_aggr)
{
    A_STATUS error = A_OK;
    ATH_SET_AGGREGATION_PARAM param;
    ATH_IOCTL_PARAM_STRUCT inout_param;

    if (A_OK != qcom_set_deviceid(device_id))
    {
        return A_ERROR;
    }

    param.txTIDMask = tx_allow_aggr;
    param.rxTIDMask = rx_allow_aggr;

    inout_param.cmd_id = ATH_SET_AGGREGATION;
    inout_param.data = &param;
    inout_param.length = sizeof(ATH_SET_AGGREGATION_PARAM);
    if (custom_qapi(device_id, ATH_SET_AGGREGATION, &inout_param) != A_OK)
    {
        error = A_ERROR;
    }
    return error;
}

A_STATUS qcom_scan_set_mode(uint8_t device_id, uint32_t mode)
{
    A_STATUS error = A_OK;
    ATH_IOCTL_PARAM_STRUCT inout_param;

    if (A_OK != qcom_set_deviceid(device_id))
    {
        return A_ERROR;
    }
    inout_param.cmd_id = ATH_SCAN_CTRL;
    inout_param.data = &mode;
    inout_param.length = 4;

    if (custom_qapi(device_id, ATH_SCAN_CTRL, &inout_param) != A_OK)
    {
        error = A_ERROR;
    }
    return error;
}

A_STATUS qcom_scan_params_set(uint8_t device_id, qcom_scan_params_t *pParam)
{
    A_STATUS error = A_OK;
    ATH_IOCTL_PARAM_STRUCT inout_param;
    WMI_SCAN_PARAMS_CMD scan_param;

    if (A_OK != qcom_set_deviceid(device_id))
    {
        return A_ERROR;
    }

    scan_param.fg_start_period = pParam->fgStartPeriod;
    scan_param.fg_end_period = pParam->fgEndPeriod;
    scan_param.bg_period = pParam->bgPeriod;
    scan_param.maxact_chdwell_time = pParam->maxActChDwellTimeInMs;
    scan_param.pas_chdwell_time = pParam->pasChDwellTimeInMs;
    scan_param.shortScanRatio = pParam->shortScanRatio;
    scan_param.scanCtrlFlags = pParam->scanCtrlFlags;
    scan_param.minact_chdwell_time = pParam->minActChDwellTimeInMs;
    scan_param.maxact_scan_per_ssid = pParam->maxActScanPerSsid;
    scan_param.max_dfsch_act_time = pParam->maxDfsChActTimeInMs;

    inout_param.cmd_id = ATH_SET_SCAN_PARAM;
    inout_param.data = &scan_param;
    inout_param.length = sizeof(WMI_SCAN_PARAMS_CMD);

    if (custom_qapi(device_id, ATH_SET_SCAN_PARAM, &inout_param) != A_OK)
    {
        error = A_ERROR;
    }
    return error;
}

/***************************************************************************************
* QCOM SECURITY APIs
***************************************************************************************/
/*FUNCTION*-----------------------------------------------------------------
*
* Function Name  : set_wep_key
* Returned Value : A_ERROR on error else A_OK
* Comments       : Store WEP key for later use. Size of Key must be 10 or 26
*                  Hex characters
*
*END------------------------------------------------------------------*/
A_STATUS qcom_sec_set_wepkey(uint8_t device_id, uint32_t keyindex, char *pkey)
{
    A_STATUS error = A_OK;
    uint32_t len = strlen(pkey);
    ATH_IOCTL_PARAM_STRUCT inout_param;
    ATH_WEPKEYS param;

    param.defKeyIndex = keyindex;
    param.keyLength = len;
    param.numKeys = 1;
    param.keys[0] = pkey;

    inout_param.cmd_id = ATH_SET_WEPKEY;
    inout_param.data = &param;
    inout_param.length = sizeof(ATH_WEPKEYS);
    if (custom_qapi(device_id, ATH_SET_WEPKEY, &inout_param) != A_OK)
    {
        error = A_ERROR;
    }

    return error;
}

A_STATUS qcom_sec_get_wepkey(uint8_t device_id, uint32_t keyindex, char *pkey)
{
    A_STATUS error = A_OK;
    ATH_IOCTL_PARAM_STRUCT inout_param;
    ATH_WEPKEYS param;

    param.defKeyIndex = keyindex;
    param.numKeys = 1;
    param.keys[0] = pkey;

    inout_param.cmd_id = ATH_GET_WEPKEY;
    inout_param.data = &param;
    inout_param.length = sizeof(ATH_WEPKEYS);
    if (custom_qapi(device_id, ATH_GET_WEPKEY, &inout_param) != A_OK)
    {
        error = A_ERROR;
    }

    return error;
}

A_STATUS qcom_sec_set_wepkey_index(uint8_t device_id, uint32_t keyindex)
{
    ATH_IOCTL_PARAM_STRUCT inout_param;
    uint32_t param;

    param = keyindex;

    inout_param.cmd_id = ATH_SET_WEPINDEX;
    inout_param.data = &param;
    inout_param.length = sizeof(uint32_t);
    if (custom_qapi(device_id, ATH_SET_WEPINDEX, &inout_param) != A_OK)
    {
        return A_ERROR;
    }

    param = WLAN_AUTH_WEP;
    return (custom_qapi(device_id, ATH_SET_SEC_TYPE, &param));
}

A_STATUS qcom_sec_get_wepkey_index(uint8_t device_id, uint32_t *pkeyindex)
{
    ATH_IOCTL_PARAM_STRUCT inout_param;
    uint32_t param;

    inout_param.cmd_id = ATH_GET_WEPINDEX;
    inout_param.data = &param;
    inout_param.length = sizeof(uint32_t);
    if (custom_qapi(device_id, ATH_GET_WEPINDEX, &inout_param) != A_OK)
    {
        return A_ERROR;
    }

    *pkeyindex = param;
    return A_OK;
}

A_STATUS qcom_sec_set_auth_mode(uint8_t device_id, WLAN_AUTH_MODE mode)
{
    uint32_t inout_param = {0};
    if ((uint32_t)mode >= (uint32_t)WLAN_AUTH_INVALID)
    {
        return A_ERROR;
    }
    if (A_OK != qcom_set_deviceid(device_id))
    {
        return A_ERROR;
    }
    inout_param = mode;
    return (custom_qapi(device_id, ATH_SET_SEC_TYPE, &inout_param));
}

A_STATUS qcom_sec_set_encrypt_mode(uint8_t device_id, WLAN_CRYPT_TYPE mode)
{
    cipher_t cipher_mode;
    ATH_IOCTL_PARAM_STRUCT inout_param = {0};

    /* NOTE: lower layer does not perform *any* range check !! */
    if ((uint32_t)mode >= (uint32_t)WLAN_CRYPT_INVALID)
    {
        return A_ERROR;
    }
    if (A_OK != qcom_set_deviceid(device_id))
    {
        return A_ERROR;
    }

    cipher_mode.ucipher = 0x1 << mode;
    cipher_mode.mcipher = cipher_mode.ucipher;
    inout_param.cmd_id = ATH_SET_CIPHER;
    inout_param.data = &cipher_mode;
    inout_param.length = sizeof(cipher_t);

    if (A_OK != custom_qapi(device_id, ATH_SET_CIPHER, &inout_param))
    {
        return A_ERROR;
    }

    return A_OK;
}

A_STATUS qcom_sec_set_passphrase(uint8_t device_id, QCOM_PASSPHRASE *passphrase)
{
    ATH_IOCTL_PARAM_STRUCT param = {0};
    if (NULL == passphrase)
    {
        return A_ERROR;
    }
    if (A_OK != qcom_set_deviceid(device_id))
    {
        return A_ERROR;
    }

    param.data = passphrase;
    param.length = strlen(passphrase->passphrase);

    if (param.length >= sizeof(QCOM_PASSPHRASE))
    {
        return A_ERROR;
    }
    if (A_OK != custom_qapi(device_id, ATH_SET_PASSPHRASE, &param))
    {
        return A_ERROR;
    }
    return A_OK;
}

A_STATUS qcom_sec_set_pmk(uint8_t device_id, char *pmk)
{
    int j;
    uint8_t val = 0;
    WMI_SET_PMK_CMD cmd;
    ATH_IOCTL_PARAM_STRUCT inout_param;
    A_STATUS error = A_OK;

    if (A_OK != qcom_set_deviceid(device_id))
    {
        return A_ERROR;
    }

    A_MEMZERO(cmd.pmk, WMI_PMK_LEN);
    cmd.pmk_len = WMI_PMK_LEN;

    for (j = 0; j < 64; j++)
    {
        val = Util_Ascii2Hex(pmk[j]);
        if (val == 0xff)
        {
            QCADRV_PRINTF("Invalid character\n");
            return A_ERROR;
        }
        else
        {
            if ((j & 1) == 0)
                val <<= 4;

            cmd.pmk[j >> 1] |= val;
        }
    }

    inout_param.cmd_id = ATH_SET_PMK;
    inout_param.data = &cmd;
    inout_param.length = sizeof(cmd);
    if (custom_qapi(device_id, ATH_SET_PMK, &inout_param) != A_OK)
    {
        error = A_ERROR;
    }
    return error;
}

A_STATUS qcom_power_set_mode(uint8_t device_id, uint32_t powerMode, uint8_t powerModule)
{
    ATH_IOCTL_PARAM_STRUCT param;
    POWER_MODE power;
    A_STATUS error = A_OK;

    if (A_OK != qcom_set_deviceid(device_id))
    {
        return A_ERROR;
    }

    power.pwr_module = powerModule;
    power.pwr_mode = powerMode;

    param.cmd_id = ATH_SET_POWER;
    param.data = (void *)&power;
    param.length = sizeof(POWER_MODE);

    if (custom_qapi(device_id, ATH_SET_POWER, &param) != A_OK)
    {
        error = A_ERROR;
    }

    return error;
}

A_STATUS qcom_power_get_mode(uint8_t device_id, uint32_t *powerMode)
{
    uint32_t inout_param = 0;
    A_STATUS error;

    error = custom_qapi(device_id, ATH_GET_POWER, &inout_param);
    if (A_OK != error)
    {
        return A_ERROR;
    }

    if (inout_param == 1)
    {
        *powerMode = REC_POWER;
    }
    else if (inout_param == 0)
    {
        *powerMode = MAX_PERF_POWER;
    }
    else
    {
        *powerMode = 0;
    }
    return error;
}

A_STATUS qcom_suspend_enable(boolean enable)
{
    ATH_IOCTL_PARAM_STRUCT inout_param;
    A_STATUS error = A_OK;

    inout_param.cmd_id = ATH_DEVICE_SUSPEND_ENABLE;
    inout_param.data = NULL;
    inout_param.length = 0;

    if (custom_qapi(0, ATH_DEVICE_SUSPEND_ENABLE, &inout_param) != A_OK)
    {
        error = A_ERROR;
    }

    return error;
}

A_STATUS qcom_suspend_start(uint32_t susp_time)
{
    uint32_t suspend_time;
    ATH_IOCTL_PARAM_STRUCT inout_param;
    A_STATUS error = A_OK;

    suspend_time = susp_time;
    inout_param.cmd_id = ATH_DEVICE_SUSPEND_START;
    inout_param.data = &suspend_time;
    inout_param.length = sizeof(uint32_t);

    if (custom_qapi(0, ATH_DEVICE_SUSPEND_START, &inout_param) != A_OK)
    {
        error = A_ERROR;
    }

    return error;
}

A_STATUS qcom_power_set_parameters(uint8_t device_id,
                                   uint16_t idlePeriod,
                                   uint16_t psPollNum,
                                   uint16_t dtimPolicy,
                                   uint16_t tx_wakeup_policy,
                                   uint16_t num_tx_to_wakeup,
                                   uint16_t ps_fail_event_policy)
{
    ATH_WMI_POWER_PARAMS_CMD pm;
    ATH_IOCTL_PARAM_STRUCT inout_param;
    A_STATUS error = A_OK;

    if (A_OK != qcom_set_deviceid(device_id))
    {
        return A_ERROR;
    }
    pm.idle_period = idlePeriod;
    pm.pspoll_number = psPollNum;
    pm.dtim_policy = dtimPolicy;
    pm.tx_wakeup_policy = tx_wakeup_policy;
    pm.num_tx_to_wakeup = num_tx_to_wakeup;
    pm.ps_fail_event_policy = ps_fail_event_policy;

    inout_param.cmd_id = ATH_SET_PMPARAMS;
    inout_param.data = &pm;
    inout_param.length = sizeof(ATH_WMI_POWER_PARAMS_CMD);

    if (custom_qapi(device_id, ATH_SET_PMPARAMS, &inout_param) != A_OK)
    {
        error = A_ERROR;
    }

    return error;
}

A_STATUS qcom_get_bssid(uint8_t device_id, uint8_t mac_addr[ATH_MAC_LEN])
{
    ATH_IOCTL_PARAM_STRUCT inout_param;
    A_STATUS error = A_OK;

    inout_param.cmd_id = ATH_GET_MACADDR;
    inout_param.data = mac_addr;
    inout_param.length = ATH_MAC_LEN;

    if (custom_qapi(device_id, ATH_GET_MACADDR, &inout_param) != A_OK)
    {
        error = A_ERROR;
    }
    return error;
}

/***************************************************************************************************************
* QCOM WPS APIs
****************************************************************************************************************/

A_STATUS qcom_wps_start(uint8_t device_id, int connect, int use_pinmode, char *pin)
{
#define WPS_STANDARD_TIMEOUT (30)
    ATH_IOCTL_PARAM_STRUCT param;
    ATH_WPS_START wps_start;
    A_STATUS error = A_OK;

    if (A_OK != qcom_set_deviceid(device_id))
    {
        return A_ERROR;
    }

    do
    {
        A_MEMZERO(&wps_start, sizeof(ATH_WPS_START));
        param.cmd_id = ATH_START_WPS;
        param.data = &wps_start;
        param.length = sizeof(wps_start);

        wps_start.connect_flag = connect;
        if (use_pinmode == 0)
        {
            wps_start.wps_mode = ATH_WPS_MODE_PIN;
            wps_start.pin_length = strlen(pin);
            // FIXME: This hardcoded pin value needs to be changed
            // for production to reflect what is on a sticker/label
            // TODO: what is this ????
            memcpy(wps_start.pin, pin, wps_start.pin_length + 1);
        }
        else
        {
            wps_start.wps_mode = ATH_WPS_MODE_PUSHBUTTON;
        }
        wps_start.timeout_seconds = WPS_STANDARD_TIMEOUT;

        if (gWpsCredentials.ssid_len == 0)
        {
            wps_start.ssid_info.ssid_len = 0;
        }
        else
        {
            memcpy(wps_start.ssid_info.ssid, &gWpsCredentials.ssid, sizeof(wps_start.ssid_info.ssid));
            memcpy(wps_start.ssid_info.macaddress, gWpsCredentials.mac_addr, ATH_MAC_LEN);
            wps_start.ssid_info.channel = gWpsCredentials.ap_channel;
            wps_start.ssid_info.ssid_len = gWpsCredentials.ssid_len;
        }

        /* this starts WPS on the Aheros wifi */
        if (custom_qapi(device_id, ATH_START_WPS, &param) != A_OK)
        {
            error = A_ERROR;
        }
    } while (0);

    return error;
}

A_STATUS qcom_wps_connect(uint8_t device_id)
{
    qcom_set_ssid(device_id, &gWpsCredentials.ssid);
    if (gWpsCredentials.key_idx != 0)
    {
        qcom_sec_set_wepkey(device_id, gWpsCredentials.key_idx, (char *)gWpsCredentials.key);
        qcom_sec_set_wepkey_index(device_id, gWpsCredentials.key_idx);
    }
    else if (gWpsCredentials.auth_type != WLAN_AUTH_NONE)
    {
        qcom_sec_set_auth_mode(device_id, gWpsCredentials.auth_type);
        qcom_sec_set_encrypt_mode(device_id, gWpsCredentials.encr_type);
    }

    return qcom_commit(device_id);
}

A_STATUS qcom_wps_set_credentials(uint8_t device_id, qcom_wps_credentials_t *pwps_prof)
{
    /* save wps credentials */
    A_MEMZERO(&gWpsCredentials, sizeof(qcom_wps_credentials_t));
    if (pwps_prof != NULL)
        A_MEMCPY(&gWpsCredentials, pwps_prof, sizeof(qcom_wps_credentials_t));
    return A_OK;
}

A_STATUS qcom_param_set(
    uint8_t device_id, uint16_t grp_id, uint16_t param_id, void *data, uint32_t data_length, boolean wait_for_status)
{
    A_STATUS error = A_OK;
    ATH_IOCTL_PARAM_STRUCT inout_param;
    uint32_t which_param = QCOM_PARAM_MAKE_ID(grp_id, param_id);
    uint32_t cmd_length = sizeof(WMI_PARAM_SET_CMD) + data_length - 1;

    if (A_OK != qcom_set_deviceid(device_id))
    {
        return A_ERROR;
    }

    WMI_PARAM_SET_CMD *cmd = (WMI_PARAM_SET_CMD *)A_MALLOC(cmd_length + sizeof(boolean), MALLOC_ID_CONTEXT);

    if (!cmd)
    {
        QCADRV_PRINTF("malloc fail\n");
        return A_ERROR;
    }
    cmd->length = cmd_length;
    cmd->which_param = which_param;
    memcpy(cmd->data_buffer, data, data_length);

    inout_param.cmd_id = ATH_SET_PARAM;
    inout_param.data = cmd;
    *((uint32_t *)((uint8_t *)(inout_param.data) + cmd_length)) = wait_for_status;
    inout_param.length = cmd_length;

    if (custom_qapi(device_id, ATH_SET_PARAM, &inout_param) != A_OK)
    {
        error = A_ERROR;
    }

    A_FREE(cmd, MALLOC_ID_CONTEXT);

    return error;
}

#if ENABLE_P2P_MODE

A_STATUS qcom_p2p_enable(uint8_t device_id, P2P_DEV_CTXT *dev, int32_t enable)
{
    ATH_IOCTL_PARAM_STRUCT param;
    A_STATUS error = A_OK;
    WMI_P2P_SET_PROFILE_CMD p2p;

    if (A_OK != qcom_set_deviceid(device_id))
    {
        return A_ERROR;
    }
    A_MEMZERO(&p2p, sizeof(WMI_P2P_SET_PROFILE_CMD));
    p2p.enable = enable;

    param.cmd_id = ATH_P2P_SWITCH;
    param.data = &p2p;
    param.length = sizeof(p2p);
    if (custom_qapi(device_id, ATH_P2P_SWITCH, &param) != A_OK)
    {
        error = A_ERROR;
    }
    return error;
}

static A_STATUS qcom_p2p_group_init(uint8_t device_id, uint8_t persistent_group, uint8_t group_formation)
{
    ATH_IOCTL_PARAM_STRUCT param;
    A_STATUS error = A_OK;
    WMI_P2P_GRP_INIT_CMD grpInit;

    if (A_OK != qcom_set_deviceid(device_id))
    {
        return A_ERROR;
    }
    A_MEMZERO(&grpInit, sizeof(WMI_P2P_GRP_INIT_CMD));
    grpInit.group_formation = group_formation;
    grpInit.persistent_group = persistent_group;

    param.cmd_id = ATH_P2P_APMODE;
    param.data = &grpInit;
    param.length = sizeof(grpInit);
    if (custom_qapi(device_id, ATH_P2P_APMODE, &param) != A_OK)
    {
        error = A_ERROR;
    }
    return error;
}

static A_STATUS qcom_set_ap_params(uint8_t device_id, uint16_t param_cmd, uint8_t *data_val)
{
    ATH_IOCTL_PARAM_STRUCT inout_param = {0};
    ATH_AP_PARAM_STRUCT ap_param;
    A_STATUS error = A_OK;

    if (A_OK != qcom_set_deviceid(device_id))
    {
        return A_ERROR;
    }

    inout_param.cmd_id = ATH_CONFIG_AP;
    ap_param.cmd_subset = param_cmd;
    ap_param.data = data_val;
    inout_param.data = &ap_param;

    if (custom_qapi(device_id, ATH_CONFIG_AP, &inout_param) != A_OK)
    {
        error = A_ERROR;
    }
    return error;
}

A_STATUS qcom_p2p_func_start_go(
    uint8_t device_id, QCOM_SSID *pssid, QCOM_PASSPHRASE *ppass, int32_t channel, boolean ucpersistent)
{
    A_STATUS error = A_OK;
    uint8_t wps_flag;

    qcom_sec_set_auth_mode(device_id, WLAN_AUTH_WPA2_PSK);
    qcom_sec_set_encrypt_mode(device_id, WLAN_CRYPT_AES_CRYPT);
    if (pssid != NULL)
    {
        qcom_p2p_func_set_pass_ssid(device_id, ppass, pssid);
    }

    qcom_op_set_mode(device_id, QCOM_WLAN_DEV_MODE_AP);

    if (channel != P2P_AUTO_CHANNEL)
    {
        qcom_set_channel(device_id, channel);
    }

    wps_flag = 0x01;
    if (qcom_set_ap_params(device_id, AP_SUB_CMD_WPS_FLAG, &wps_flag) != A_OK)
    {
        return A_ERROR;
    }

    qcom_op_set_mode(device_id, QCOM_WLAN_DEV_MODE_AP);

    if (qcom_p2p_group_init(device_id, ucpersistent, 1) != A_OK)
    {
        return A_ERROR;
    }

    /* Set MAX PERF */
    qcom_power_set_mode(device_id, MAX_PERF_POWER, P2P);
    return error;
}

A_STATUS qcom_p2p_func_init(uint8_t device_id, int32_t enable)
{
    static int32_t p2p_enable_flag = 0xFEFE;

    if (enable == p2p_enable_flag)
    {
        return A_OK;
    }
    p2p_enable_flag = enable;

    qcom_p2p_enable(device_id, NULL, enable);
    qcom_p2p_func_set_config(device_id, 0, 1, 1, 3000, 81, 81, 5);

    return A_OK;
}

A_STATUS qcom_p2p_func_find(uint8_t device_id, void *dev, uint8_t type, uint32_t timeout)
{
    WMI_P2P_FIND_CMD find_params;
    ATH_IOCTL_PARAM_STRUCT param;
    A_STATUS error = A_OK;

    if (A_OK != qcom_set_deviceid(device_id))
    {
        return A_ERROR;
    }
    find_params.type = type;
    find_params.timeout = A_CPU2LE32(timeout);
    param.cmd_id = ATH_P2P_FIND;
    param.data = &find_params;
    param.length = sizeof(WMI_P2P_FIND_CMD);
    if (custom_qapi(device_id, ATH_P2P_FIND, &param) != A_OK)
    {
        error = A_ERROR;
    }
    return error;
}

A_STATUS qcom_p2p_func_set_pass_ssid(uint8_t device_id, QCOM_PASSPHRASE *ppass, QCOM_SSID *pssid)
{
    ATH_IOCTL_PARAM_STRUCT param;
    A_STATUS error = A_OK;
    WMI_SET_PASSPHRASE_CMD setPassPhrase;

    if (qcom_set_ssid(device_id, pssid) != A_OK)
        return A_ERROR;

    strcpy((char *)setPassPhrase.passphrase, ppass->passphrase);
    setPassPhrase.passphrase_len = strlen(ppass->passphrase);
    strcpy((char *)setPassPhrase.ssid, pssid->ssid);
    setPassPhrase.ssid_len = strlen(pssid->ssid);

    param.cmd_id = ATH_P2P_APMODE_PP;
    param.data = &setPassPhrase;
    param.length = sizeof(WMI_SET_PASSPHRASE_CMD);
    if (custom_qapi(device_id, ATH_P2P_APMODE_PP, &param) != A_OK)
    {
        error = A_ERROR;
    }

    return error;
}

A_STATUS qcom_p2p_func_listen(uint8_t device_id, uint32_t timeout)
{
    ATH_IOCTL_PARAM_STRUCT param;
    A_STATUS error = A_OK;
    uint32_t tout;

    if (A_OK != qcom_set_deviceid(device_id))
    {
        return A_ERROR;
    }

    tout = timeout;
    param.cmd_id = ATH_P2P_LISTEN;
    param.data = &tout;
    param.length = sizeof(tout);
    if (custom_qapi(device_id, ATH_P2P_LISTEN, &param) != A_OK)
    {
        error = A_ERROR;
    }
    return error;
}

A_STATUS qcom_p2p_func_connect(uint8_t device_id, P2P_WPS_METHOD wps_method, uint8_t *peer_mac, boolean persistent)
{
    ATH_IOCTL_PARAM_STRUCT param;
    A_STATUS error = A_OK;
    WMI_P2P_FW_CONNECT_CMD_STRUCT p2p_connect;

    if (A_OK != qcom_set_deviceid(device_id))
    {
        return A_ERROR;
    }

    A_MEMZERO(&p2p_connect, sizeof(WMI_P2P_FW_CONNECT_CMD_STRUCT));
    p2p_connect.wps_method = wps_method;
    A_MEMCPY(p2p_connect.peer_addr, peer_mac, ATH_MAC_LEN);

    /* Setting Default Value for now !!! */
    p2p_connect.dialog_token = 1;
    p2p_connect.go_intent = 0;
    p2p_connect.go_dev_dialog_token = 0;
    p2p_connect.dev_capab = 0x23;
    if (persistent)
    {
        p2p_connect.dev_capab |= P2P_PERSISTENT_FLAG;
    }

    param.cmd_id = ATH_P2P_CONNECT_CLIENT;
    param.data = &p2p_connect;
    param.length = sizeof(WMI_P2P_FW_CONNECT_CMD_STRUCT);
    if (custom_qapi(device_id, ATH_P2P_CONNECT_CLIENT, &param) != A_OK)
    {
        error = A_ERROR;
    }
    return error;
}

A_STATUS qcom_p2p_func_auth(
    uint8_t device_id, int32_t dev_auth, P2P_WPS_METHOD wps_method, uint8_t *peer_mac, boolean persistent)
{
    ATH_IOCTL_PARAM_STRUCT param;
    A_STATUS error = A_OK;
    WMI_P2P_FW_CONNECT_CMD_STRUCT p2p_connect;

    if (A_OK != qcom_set_deviceid(device_id))
    {
        return A_ERROR;
    }
    A_MEMZERO(&p2p_connect, sizeof(WMI_P2P_FW_CONNECT_CMD_STRUCT));

    if (persistent)
        p2p_connect.dev_capab |= P2P_PERSISTENT_FLAG;
    p2p_connect.dev_auth = dev_auth;
    p2p_connect.wps_method = wps_method;
    A_MEMCPY(p2p_connect.peer_addr, peer_mac, ATH_MAC_LEN);
    /* If go_intent <= 0, wlan firmware will use the intent value configured via
     * qcom_p2p_set
     */
    p2p_connect.go_intent = 0;

    param.cmd_id = ATH_P2P_AUTH;
    param.data = &p2p_connect;
    param.length = sizeof(WMI_P2P_FW_CONNECT_CMD_STRUCT);
    if (custom_qapi(device_id, ATH_P2P_AUTH, &param) != A_OK)
    {
        error = A_ERROR;
    }
    return error;
}

A_STATUS qcom_p2p_func_cancel(uint8_t device_id)
{
    ATH_IOCTL_PARAM_STRUCT param;
    A_STATUS error = A_OK;

    if (A_OK != qcom_set_deviceid(device_id))
    {
        return A_ERROR;
    }

    param.cmd_id = ATH_P2P_CANCEL;
    param.data = NULL;
    param.length = 0;
    if (custom_qapi(device_id, ATH_P2P_CANCEL, &param) != A_OK)
    {
        error = A_ERROR;
    }
    return error;
}

A_STATUS qcom_p2p_func_stop_find(uint8_t device_id)
{
    ATH_IOCTL_PARAM_STRUCT param;
    A_STATUS error = A_OK;

    if (A_OK != qcom_set_deviceid(device_id))
    {
        return A_ERROR;
    }
    param.cmd_id = ATH_P2P_STOP;
    param.data = NULL;
    param.length = 0;
    if (custom_qapi(device_id, ATH_P2P_STOP, &param) != A_OK)
    {
        error = A_ERROR;
    }
    return error;
}

/* NODE_LIST */
A_STATUS qcom_p2p_func_get_node_list(uint8_t device_id, void *app_buf, uint32_t buf_size)
{
    ATH_IOCTL_PARAM_STRUCT param;
    A_STATUS error = A_OK;

    if (A_OK != qcom_set_deviceid(device_id))
    {
        return A_ERROR;
    }
    param.cmd_id = ATH_P2P_NODE_LIST;
    param.data = app_buf;
    param.length = buf_size;
    if (custom_qapi(device_id, ATH_P2P_NODE_LIST, &param) != A_OK)
    {
        error = A_ERROR;
    }
    return error;
}

A_STATUS qcom_p2p_func_set_config(uint8_t device_id,
                                  uint32_t uigo_intent,
                                  uint32_t uiclisten_ch,
                                  uint32_t uiop_ch,
                                  uint32_t uiage,
                                  uint32_t reg_class,
                                  uint32_t op_reg_class,
                                  uint32_t max_node_count)
{
    ATH_IOCTL_PARAM_STRUCT param;
    A_STATUS error = A_OK;
    WMI_P2P_FW_SET_CONFIG_CMD stp2p_cfg_cmd;

    if (A_OK != qcom_set_deviceid(device_id))
    {
        return A_ERROR;
    }
    A_MEMZERO(&stp2p_cfg_cmd, sizeof(stp2p_cfg_cmd));
    stp2p_cfg_cmd.go_intent = uigo_intent;
    stp2p_cfg_cmd.reg_class = reg_class;
    stp2p_cfg_cmd.op_reg_class = op_reg_class;
    stp2p_cfg_cmd.op_channel = uiop_ch;
    stp2p_cfg_cmd.listen_channel = uiclisten_ch;
    stp2p_cfg_cmd.node_age_to = uiage;
    stp2p_cfg_cmd.max_node_count = max_node_count;

    param.cmd_id = ATH_P2P_SET_CONFIG;
    param.data = &stp2p_cfg_cmd;
    param.length = sizeof(WMI_P2P_FW_SET_CONFIG_CMD);
    if (custom_qapi(device_id, ATH_P2P_SET_CONFIG, &param) != A_OK)
    {
        error = A_ERROR;
    }
    return error;
}

A_STATUS qcom_p2p_func_wps_config(uint8_t device_id, uint8_t *p2p_ptr)
{
    ATH_IOCTL_PARAM_STRUCT param;
    A_STATUS error = A_OK;

    if (A_OK != qcom_set_deviceid(device_id))
    {
        return A_ERROR;
    }

    param.cmd_id = ATH_P2P_WPS_CONFIG;
    param.data = p2p_ptr;
    param.length = sizeof(WMI_WPS_SET_CONFIG_CMD);
    if (custom_qapi(device_id, ATH_P2P_WPS_CONFIG, &param) != A_OK)
    {
        error = A_ERROR;
    }
    return error;
}

A_STATUS qcom_p2p_func_disc_req(uint8_t device_id, uint8_t *p2p_ptr)
{
    ATH_IOCTL_PARAM_STRUCT param;
    A_STATUS error = A_OK;

    param.cmd_id = ATH_P2P_DISC_REQ;
    param.data = p2p_ptr;
    param.length = sizeof(WMI_P2P_FW_PROV_DISC_REQ_CMD);
    if (custom_qapi(device_id, ATH_P2P_DISC_REQ, &param) != A_OK)
    {
        error = A_ERROR;
    }
    return error;
}

A_STATUS qcom_p2p_set(uint8_t device_id, P2P_CONF_ID config_id, void *data, uint32_t data_length)
{
    ATH_IOCTL_PARAM_STRUCT param;
    A_STATUS error = A_OK;
    WMI_P2P_SET_CMD p2p_set_params;
    WMI_P2P_CONF_ID wmi_config_id;

    if (A_OK != qcom_set_deviceid(device_id))
    {
        return A_ERROR;
    }
    A_MEMZERO(&p2p_set_params, sizeof(p2p_set_params));

    /*TODO: aren't WMI_P2P_CONF_ID / P2P_CONF_ID the same ?*/
    switch (config_id)
    {
        case P2P_CONFIG_LISTEN_CHANNEL:
            wmi_config_id = WMI_P2P_CONFID_LISTEN_CHANNEL;
            break;
        case P2P_CONFIG_CROSS_CONNECT:
            wmi_config_id = WMI_P2P_CONFID_CROSS_CONNECT;
            break;
        case P2P_CONFIG_SSID_POSTFIX:
            wmi_config_id = WMI_P2P_CONFID_SSID_POSTFIX;
            break;
        case P2P_CONFIG_INTRA_BSS:
            wmi_config_id = WMI_P2P_CONFID_INTRA_BSS;
            break;
        case P2P_CONFIG_CONCURRENT_MODE:
            wmi_config_id = WMI_P2P_CONFID_CONCURRENT_MODE;
            break;
        case P2P_CONFIG_GO_INTENT:
            wmi_config_id = WMI_P2P_CONFID_GO_INTENT;
            break;
        case P2P_CONFIG_DEV_NAME:
            wmi_config_id = WMI_P2P_CONFID_DEV_NAME;
            break;
        case P2P_CONFIG_P2P_OPMODE:
            wmi_config_id = WMI_P2P_CONFID_P2P_OPMODE;
            break;
        case P2P_CONFIG_CCK_RATES:
            wmi_config_id = WMI_P2P_CONFID_CCK_RATES;
            break;
        default:
            /* Unknown parameter */
            return A_ERROR;
    }

    p2p_set_params.config_id = wmi_config_id;
    A_MEMCPY(&p2p_set_params.val, data, data_length);

    param.cmd_id = ATH_P2P_SET;
    param.data = &p2p_set_params;
    param.length = sizeof(WMI_P2P_SET_CMD);
    if (custom_qapi(device_id, ATH_P2P_SET, &param) != A_OK)
    {
        error = A_ERROR;
    }
    return error;
}

A_STATUS qcom_p2p_func_join(uint8_t device_id, P2P_WPS_METHOD wps_method, uint8_t *pmac, char *ppin, uint16_t channel)
{
    uint16_t conc_channel;
    WMI_P2P_FW_CONNECT_CMD_STRUCT p2p_join_profile;
    WMI_P2P_PROV_INFO p2p_info;
    ATH_IOCTL_PARAM_STRUCT param;
    A_STATUS error = A_OK;

#if ENABLE_SCC_MODE
    int num_dev = WLAN_NUM_OF_DEVICES;
    if ((num_dev > 1) && (device_id == 0))
    {
        qcom_get_concurrent_channel(device_id, &conc_channel);
        if ((conc_channel != 0) && (channel == 0))
        {
            return A_ERROR;
        }
        if ((conc_channel != 0) && (channel != conc_channel))
        {
            return A_ERROR;
        }
    }
#endif /* ENABLE_SCC_MODE */

    if (A_OK != qcom_set_deviceid(device_id))
    {
        return A_ERROR;
    }

    A_MEMZERO(&p2p_join_profile, sizeof(WMI_P2P_FW_CONNECT_CMD_STRUCT));
    A_MEMCPY(p2p_join_profile.peer_addr, pmac, ATH_MAC_LEN);
    p2p_join_profile.go_oper_freq = channel;
    p2p_join_profile.wps_method = wps_method;

    if (wps_method != (uint8_t)WPS_PBC)
    {
        strcpy(p2p_info.wps_pin, ppin);
        wmi_save_key_info(&p2p_info);
    }

    param.cmd_id = ATH_P2P_JOIN;
    param.data = &p2p_join_profile;
    param.length = sizeof(WMI_P2P_FW_CONNECT_CMD_STRUCT);
    if (custom_qapi(device_id, ATH_P2P_JOIN, &param) != A_OK)
    {
        error = A_ERROR;
    }
    return error;
}

A_STATUS qcom_p2p_func_invite_auth(uint8_t device_id, uint8_t *inv)
{
    ATH_IOCTL_PARAM_STRUCT param;
    A_STATUS error = A_OK;

    if (A_OK != qcom_set_deviceid(device_id))
    {
        return A_ERROR;
    }

    param.cmd_id = ATH_P2P_INVITE_AUTH;
    param.data = inv;
    param.length = sizeof(WMI_P2P_FW_INVITE_REQ_RSP_CMD);
    if (custom_qapi(device_id, ATH_P2P_INVITE_AUTH, &param) != A_OK)
    {
        error = A_ERROR;
    }
    return error;
}

/* LIST_NETWORK */
A_STATUS qcom_p2p_func_get_network_list(uint8_t device_id, void *app_buf, uint32_t buf_size)
{
    ATH_IOCTL_PARAM_STRUCT param;
    A_STATUS error = A_OK;

    if (A_OK != qcom_set_deviceid(device_id))
    {
        return A_ERROR;
    }
    param.cmd_id = ATH_P2P_PERSISTENT_LIST;
    param.data = app_buf;
    param.length = buf_size;
    if (custom_qapi(device_id, ATH_P2P_PERSISTENT_LIST, &param) != A_OK)
    {
        error = A_ERROR;
    }
    return error;
}

A_STATUS qcom_p2p_func_invite(
    uint8_t device_id, char *pssid, P2P_WPS_METHOD wps_method, uint8_t *pmac, boolean persistent, P2P_INV_ROLE role)
{
    ATH_IOCTL_PARAM_STRUCT param;
    A_STATUS error = A_OK;
    WMI_P2P_INVITE_CMD p2pInvite;

    if (A_OK != qcom_set_deviceid(device_id))
    {
        return A_ERROR;
    }

    A_MEMZERO(&p2pInvite, sizeof(p2pInvite));

    A_MEMCPY(p2pInvite.ssid.ssid, pssid, strlen(pssid));
    p2pInvite.ssid.ssidLength = strlen(pssid);
    A_MEMCPY(p2pInvite.peer_addr, pmac, ATH_MAC_LEN);
    p2pInvite.wps_method = wps_method;
    p2pInvite.is_persistent = persistent;
    p2pInvite.role = role;

    param.cmd_id = ATH_P2P_INVITE;
    param.data = &p2pInvite;
    param.length = sizeof(WMI_P2P_INVITE_CMD);

    if (custom_qapi(device_id, ATH_P2P_INVITE, &param) != A_OK)
    {
        error = A_ERROR;
    }
    return error;
}

A_STATUS qcom_p2p_func_join_profile(uint8_t device_id, uint8_t *pmac)
{
    ATH_IOCTL_PARAM_STRUCT param;
    A_STATUS error = A_OK;
    WMI_P2P_FW_CONNECT_CMD_STRUCT p2p_connect;

    if (A_OK != qcom_set_deviceid(device_id))
    {
        return A_ERROR;
    }
    A_MEMZERO(&p2p_connect, sizeof(p2p_connect));
    A_MEMCPY(p2p_connect.peer_addr, pmac, ATH_MAC_LEN);

    param.cmd_id = ATH_P2P_JOIN_PROFILE;
    param.data = &p2p_connect;
    param.length = sizeof(WMI_P2P_FW_CONNECT_CMD_STRUCT);
    if (custom_qapi(device_id, ATH_P2P_JOIN_PROFILE, &param) != A_OK)
    {
        error = A_ERROR;
    }
    return error;
}

A_STATUS qcom_p2p_ap_mode(uint8_t device_id, uint8_t *p2p_ptr)
{
    ATH_IOCTL_PARAM_STRUCT param;
    A_STATUS error = A_OK;

    if (A_OK != qcom_set_deviceid(device_id))
    {
        return A_ERROR;
    }
    param.cmd_id = ATH_P2P_APMODE;
    param.data = p2p_ptr;
    param.length = sizeof(WMI_P2P_GRP_INIT_CMD);
    if (custom_qapi(device_id, ATH_P2P_APMODE, &param) != A_OK)
    {
        error = A_ERROR;
    }
    return error;
}

A_STATUS qcom_p2p_func_on_off(uint8_t device_id, uint8_t *p2p_ptr)
{
    ATH_IOCTL_PARAM_STRUCT param;
    A_STATUS error = A_OK;

    if (A_OK != qcom_set_deviceid(device_id))
    {
        return A_ERROR;
    }
    param.cmd_id = ATH_P2P_SWITCH;
    param.data = p2p_ptr;
    param.length = sizeof(WMI_P2P_SET_PROFILE_CMD);
    if (custom_qapi(device_id, ATH_P2P_SWITCH, &param) != A_OK)
    {
        error = A_ERROR;
    }
    return error;
}

A_STATUS qcom_p2p_func_set_noa(
    uint8_t device_id, uint8_t uccount, uint32_t uistart, uint32_t uiduration, uint32_t uiinterval)
{
    ATH_IOCTL_PARAM_STRUCT param;
    A_STATUS error = A_OK;
    P2P_NOA_INFO p2p_noa_desc;

    if (A_OK != qcom_set_deviceid(device_id))
    {
        return A_ERROR;
    }
    if (uccount >= P2P_MAX_NOA_DESCRIPTORS)
    {
        return A_ERROR;
    }

    A_MEMZERO(&p2p_noa_desc, sizeof(p2p_noa_desc));
    if (uccount > 0)
    {
        uint8_t i;
        p2p_noa_desc.enable = 1;
        p2p_noa_desc.count = uccount;

        for (i = 0; i < uccount; i++)
        {
            p2p_noa_desc.noas[i].count_or_type = uccount;
            p2p_noa_desc.noas[i].duration = uiduration;
            p2p_noa_desc.noas[i].interval = uiinterval;
            p2p_noa_desc.noas[i].start_or_offset = uistart;
        }
    }

    param.cmd_id = ATH_P2P_SET_NOA;
    param.data = &p2p_noa_desc;
    param.length = sizeof(p2p_noa_desc);
    if (custom_qapi(device_id, ATH_P2P_SET_NOA, &param) != A_OK)
    {
        error = A_ERROR;
    }
    return error;
}

A_STATUS qcom_p2p_func_set_oppps(uint8_t device_id, uint8_t enable, uint8_t ctwin)
{
    ATH_IOCTL_PARAM_STRUCT param;
    A_STATUS error = A_OK;
    WMI_OPPPS_INFO_STRUCT p2p_oppps;

    if (A_OK != qcom_set_deviceid(device_id))
    {
        return A_ERROR;
    }
    A_MEMZERO(&p2p_oppps, sizeof(WMI_OPPPS_INFO_STRUCT));
    p2p_oppps.enable = enable;
    p2p_oppps.ctwin = ctwin;

    param.cmd_id = ATH_P2P_SET_OPPPS;
    param.data = &p2p_oppps;
    param.length = sizeof(WMI_OPPPS_INFO_STRUCT);
    if (custom_qapi(device_id, ATH_P2P_SET_OPPPS, &param) != A_OK)
    {
        error = A_ERROR;
    }
    return error;
}

A_STATUS qcom_p2p_func_prov(uint8_t device_id, P2P_WPS_METHOD wps_method, uint8_t *pmac)
{
    ATH_IOCTL_PARAM_STRUCT param;
    A_STATUS error = A_OK;
    WMI_P2P_FW_PROV_DISC_REQ_CMD p2p_prov_disc;

    if (A_OK != qcom_set_deviceid(device_id))
    {
        return A_ERROR;
    }
    A_MEMZERO(&p2p_prov_disc, sizeof(p2p_prov_disc));
    p2p_prov_disc.dialog_token = 1;
    p2p_prov_disc.wps_method = wps_method;
    A_MEMCPY(p2p_prov_disc.peer, pmac, ATH_MAC_LEN);

    param.cmd_id = ATH_P2P_DISC_REQ;
    param.data = &p2p_prov_disc;
    param.length = sizeof(p2p_prov_disc);
    if (custom_qapi(device_id, ATH_P2P_DISC_REQ, &param) != A_OK)
    {
        error = A_ERROR;
    }
    return error;
}
#endif

A_STATUS qcom_raw_mode_send_pkt(QCOM_RAW_MODE_PARAM_t *ppara)
{
    uint8_t access_cat;
    int32_t i;
    ATH_IOCTL_PARAM_STRUCT param;
    ATH_MAC_TX_RAW_S details;
    uint8_t *buffer = NULL;
    typedef struct
    {
        uint8_t fc[2];
        uint8_t duration[2];
        uint8_t addr[3][6];
        uint8_t seq[2];
    } WIFI_HDR;

    typedef struct
    {
        uint8_t fc[2];
        uint8_t duration[2];
        uint8_t addr[3][6];
        uint8_t seq[2];
        uint8_t addr_3[6];
        uint8_t pad[2]; // pad required by firmware
    } WIFI_4_HDR;

    typedef struct
    {
        uint8_t fc[2];
        uint8_t duration[2];
        uint8_t addr[3][6];
        uint8_t seq[2];
        uint8_t qos[2];
        uint8_t pad[2]; // pad required by firmware
    } WIFI_QOS_HDR;
#define WIFI_HDR_SZ (24)
#define WIFI_4_HDR_SZ (32)
#define WIFI_QOS_HDR_SZ (28)
    uint8_t header_sz[3] = {WIFI_HDR_SZ, WIFI_QOS_HDR_SZ, WIFI_4_HDR_SZ};
    WIFI_4_HDR *p4Hdr;
    WIFI_HDR *pHdr;
    WIFI_QOS_HDR *pQosHdr;
    A_STATUS error = A_OK;

    uint8_t rate_index = ppara->rate_index;
    uint8_t tries = ppara->tries;
    uint32_t size = ppara->size;
    uint32_t chan = ppara->chan;
    uint8_t header_type = ppara->header_type;
    uint16_t bcn_seq_num = ppara->seq;
    uint8_t *paddr1 = &ppara->addr1.addr[0];
    uint8_t *paddr2 = &ppara->addr2.addr[0];
    uint8_t *paddr3 = &ppara->addr3.addr[0];
    uint8_t *paddr4 = &ppara->addr4.addr[0];
    uint8_t *pdatabuf = ppara->pdatabuf;
    uint32_t buflen = ppara->buflen;

    do
    {
        if (rate_index > 19 || tries > 14 || size > 1400 || chan > 14 || header_type > 2)
        {
            error = A_ERROR;
            break;
        }

        if ((pdatabuf != NULL) && (buflen >= size))
        {
            error = A_ERROR;
            break;
        }

        if (chan != 0)
        {
            error = qcom_set_channel(0, chan);
            if (A_OK != error)
            {
                break;
            }
        }

        size += header_sz[header_type];
        if (NULL == (buffer = A_MALLOC(size, MALLOC_ID_CONTEXT)))
        {
            error = A_ERROR;
            break;
        }

        pHdr = (WIFI_HDR *)&buffer[0];

        A_MEMZERO(pHdr, WIFI_HDR_SZ);

        pHdr->fc[0] = 0x80; // beacon
        pHdr->seq[0] = ((bcn_seq_num & 0xf) << 4);
        pHdr->seq[1] = ((bcn_seq_num & 0xff0) >> 4);

        A_MEMCPY(&pHdr->addr[0][0], paddr1, ATH_MAC_LEN);
        A_MEMCPY(&pHdr->addr[1][0], paddr2, ATH_MAC_LEN);
        A_MEMCPY(&pHdr->addr[2][0], paddr3, ATH_MAC_LEN);

        access_cat = ATH_ACCESS_CAT_BE;

        if (header_type == 1)
        {
            // change frame control to qos data
            pHdr->fc[0] = 0x88;
            pHdr->fc[1] = 0x01; // to-ds
            pQosHdr = (WIFI_QOS_HDR *)pHdr;
            pQosHdr->qos[0] = 0x03;
            pQosHdr->qos[1] = 0x00;
            // bssid == addr[0]
            // memcpy(&(pHdr->addr[0][0]), &(pHdr->addr[1][0]), 6);
            // change destination address
            // pHdr->addr[2][3] = 0xaa;
            // pHdr->addr[2][4] = 0xaa;
            // pHdr->addr[2][5] = 0xaa;
            access_cat = ATH_ACCESS_CAT_VI;
        }
        else if (header_type == 2)
        {
            pHdr->fc[0] = 0x08;
            pHdr->fc[1] = 0x03; // to-ds | from-ds
            p4Hdr = (WIFI_4_HDR *)pHdr;
            p4Hdr->addr_3[0] = paddr4[0];
            p4Hdr->addr_3[1] = paddr4[1];
            p4Hdr->addr_3[2] = paddr4[2];
            p4Hdr->addr_3[3] = paddr4[3];
            p4Hdr->addr_3[4] = paddr4[4];
            p4Hdr->addr_3[5] = paddr4[5];
        }

        /* now setup the frame for transmission */
        if (pdatabuf == NULL)
        {
            for (i = header_sz[header_type]; i < (int32_t)size; i++)
                buffer[i] = (i - header_sz[header_type]) & 0xff;
        }
        else
        {
            i = header_sz[header_type];
            A_MEMCPY(&(buffer[i]), pdatabuf, buflen);
        }

        A_MEMZERO(&details, sizeof(details));

        details.buffer = buffer;
        details.length = size;
        details.params.pktID = 1;
        details.params.rateSched.accessCategory = access_cat;
        /* we set the ACK bit simply to force the desired number of retries */
        details.params.rateSched.flags = ATH_MAC_TX_FLAG_ACK;
        details.params.rateSched.trySeries[0] = tries;
        details.params.rateSched.rateSeries[0] = rate_index;
        details.params.rateSched.rateSeries[1] = 0xff; // terminate series

        param.cmd_id = ATH_MAC_TX_RAW;
        param.data = &details;
        param.length = sizeof(details);

        if (custom_qapi(0, ATH_MAC_TX_RAW, &param) != A_OK)
        {
            error = A_ERROR;
        }
    } while (0);

    if (buffer)
        A_FREE(buffer, MALLOC_ID_CONTEXT);

    return error;
}

A_STATUS qcom_get_versionstr(ATH_VERSION_STR *versionstr)
{
    ATH_IOCTL_PARAM_STRUCT param;

    param.cmd_id = ATH_GET_VERSION_STR;
    param.data = versionstr;
    param.length = sizeof(ATH_VERSION_STR);

    return custom_qapi(0, ATH_GET_VERSION_STR, &param);
}

A_STATUS qcom_set_appie(uint8_t device_id, uint8_t mgmtType, uint8_t *appie, uint8_t ieLen)
{
    A_STATUS error = A_OK;
    ATH_IOCTL_PARAM_STRUCT inout_param;
    ATH_APPIE_PARAM param;

    if(A_OK != qcom_set_deviceid(device_id))
    {
        return A_ERROR;
    }

    /* appie is a hex string beginning with DD */
    param.mgmtFrmType = mgmtType;
    param.ieLen = ieLen;
    param.ieInfo = appie;

    inout_param.cmd_id = ATH_SET_APPIE;
    inout_param.data = &param;
    inout_param.length = sizeof(ATH_APPIE_PARAM);

    if(param.ieLen > ATH_MAX_IE_LEN)
    {
        return A_ERROR;
    }

    if(custom_qapi(device_id, ATH_SET_APPIE, &inout_param) != A_OK)
    {
        error = A_ERROR;
    }

    return error;
}

A_STATUS qcom_wps_init_key (uint8_t device_id)
{
    ATH_IOCTL_PARAM_STRUCT  inout_param;
    A_STATUS error = A_OK;

    inout_param.cmd_id = ATH_WLAN_WPS_INIT_KEY;
    inout_param.data = NULL;
    inout_param.length = 0;

    if(custom_qapi(device_id, ATH_WLAN_WPS_INIT_KEY, &inout_param) != A_OK)
    {
        error = A_ERROR;
    }
    return error;
}

A_STATUS qcom_heartbeat_challenge (uint8_t device_id)
{
    ATH_IOCTL_PARAM_STRUCT  inout_param;
    A_STATUS error = A_OK;

#define HB_MAGIC        0x63825363L

    struct WMIX_GPIO_REGISTER_GET {
        WMIX_CMD_HDR cmd;
        WMIX_HB_CHALLENGE_RESP_CMD hb;
    } hb_challenge_cmd;


    hb_challenge_cmd.cmd.commandId =WMIX_HB_CHALLENGE_RESP_CMDID;
    hb_challenge_cmd.hb.cookie = HB_MAGIC;
    hb_challenge_cmd.hb.source = 0; //TODO

    inout_param.cmd_id = ATH_HEARTBEAT_CHALLEANGE;
    inout_param.data = (void *)&hb_challenge_cmd;
    inout_param.length = sizeof(hb_challenge_cmd);

    if(custom_qapi(device_id, ATH_HEARTBEAT_CHALLEANGE, &inout_param) != A_OK)
    {
        error = A_ERROR;
    }
    return error;
}

