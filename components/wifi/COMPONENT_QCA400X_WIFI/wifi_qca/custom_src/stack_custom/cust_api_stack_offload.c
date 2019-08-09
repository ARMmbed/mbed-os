//------------------------------------------------------------------------------
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
//------------------------------------------------------------------------------
#include <a_config.h>
#include <a_types.h>
#include <a_osapi.h>
#include <driver_cxt.h>
#include <common_api.h>
#include <custom_wlan_api.h>
#include <wmi_api.h>
#include <htc.h>
#include "atheros_wifi.h"
#include "atheros_wifi_api.h"
#include "atheros_wifi_internal.h"
#include "cust_netbuf.h"

#if ENABLE_STACK_OFFLOAD
#include "atheros_stack_offload.h"
#include "common_stack_offload.h"
#include "custom_stack_offload.h"

#if ZERO_COPY
A_NETBUF_QUEUE_T zero_copy_free_queue;
#endif

#if ENABLE_SSL
extern void *sslhandle;
int32_t find_socket_context_from_ssl(SSL *ssl);
#endif

#define DEFAULT_DNS_TIMEOUT 25000 /* 25 seconds */
uint32_t dns_block_time = DEFAULT_DNS_TIMEOUT;
uint32_t (*dhcps_success_callback)(uint8_t *mac, uint32_t ip) = NULL;
uint32_t (*dhcpc_success_callback)(uint32_t ip, uint32_t mask, uint32_t gw) = NULL;

/*****************************************************************************/
/*  t_socket - Custom version of socket API- please see Api_socket for details
 * RETURNS: socket handle or A_ERROR
 *****************************************************************************/
int32_t t_socket(QCA_CONTEXT_STRUCT_PTR qca_ptr, uint32_t domain, uint32_t type, uint32_t protocol)
{
    void *pCxt;
    int32_t result = -1;
    uint32_t delay_ms = 0;
    volatile uint32_t timeout_idx = 0;

    if ((pCxt = qca_ptr->MAC_CONTEXT_PTR) == NULL)
    {
        return A_ERROR;
    }
    /*Wait for chip to be up*/
    if (A_OK != Api_DriverAccessCheck(pCxt, 1, ACCESS_REQUEST_IOCTL))
    {
        return A_ERROR;
    }

    for (timeout_idx = 0; timeout_idx < 8; timeout_idx++)
    {
        result = Api_socket(pCxt, domain, type, protocol);
        /* WiFi cannot allocate more resorces, let's try it again in 100ms */
        if (A_RESOURCES == result || A_ERROR == result)
        {
            delay_ms = 100 << (timeout_idx);
            A_MDELAY(delay_ms);
        }
        else
        {
            break;
        }
    }

    /* Report last A_RESOURCES error as generic error to keep compatible API */
    result = result == A_RESOURCES ? A_ERROR : result;

    return (result);
}

/*****************************************************************************/
/*  t_shutdown - Custom version of socket shutdown API- please see Api_shutdown for details
 * RETURNS: A_OK or A_ERROR
 *****************************************************************************/
int32_t t_shutdown(QCA_CONTEXT_STRUCT_PTR qca_ptr, uint32_t handle)
{
    void *pCxt;

    if ((pCxt = qca_ptr->MAC_CONTEXT_PTR) == NULL)
    {
        return A_ERROR;
    }
    /*Wait for chip to be up*/
    if (A_OK != Api_DriverAccessCheck(pCxt, 1, ACCESS_REQUEST_IOCTL))
    {
        return A_ERROR;
    }

    return (Api_shutdown(pCxt, handle));
}

/*****************************************************************************/
/*  t_connect - Custom version of socket connect API- please see Api_connect for details
 * RETURNS: A_OK or A_ERROR
 *****************************************************************************/
int32_t t_connect(QCA_CONTEXT_STRUCT_PTR qca_ptr, uint32_t handle, void *name, uint16_t length)
{
    void *pCxt;

    if ((pCxt = qca_ptr->MAC_CONTEXT_PTR) == NULL)
    {
        return A_ERROR;
    }
    /*Wait for chip to be up*/
    if (A_OK != Api_DriverAccessCheck(pCxt, 1, ACCESS_REQUEST_IOCTL))
    {
        return A_ERROR;
    }

    return (Api_connect(pCxt, handle, name, length));
}

/*****************************************************************************/
/*  t_bind - Custom version of socket bind API- please see Api_bind for details
 * RETURNS: A_OK or A_ERROR
 *****************************************************************************/
int32_t t_bind(QCA_CONTEXT_STRUCT_PTR qca_ptr, uint32_t handle, void *name, uint16_t length)
{
    void *pCxt;

    if ((pCxt = qca_ptr->MAC_CONTEXT_PTR) == NULL)
    {
        return A_ERROR;
    }
    /*Wait for chip to be up*/
    if (A_OK != Api_DriverAccessCheck(pCxt, 1, ACCESS_REQUEST_IOCTL))
    {
        return A_ERROR;
    }

    return (Api_bind(pCxt, handle, name, length));
}

/*****************************************************************************/
/*  t_listen - Custom version of socket listen API- please see Api_listen for details
 * RETURNS: A_OK or A_ERROR
 *****************************************************************************/
int32_t t_listen(QCA_CONTEXT_STRUCT_PTR qca_ptr, uint32_t handle, uint32_t backlog)
{
    void *pCxt;

    if ((pCxt = qca_ptr->MAC_CONTEXT_PTR) == NULL)
    {
        return A_ERROR;
    }
    /*Wait for chip to be up*/
    if (A_OK != Api_DriverAccessCheck(pCxt, 1, ACCESS_REQUEST_IOCTL))
    {
        return A_ERROR;
    }

    return (Api_listen(pCxt, handle, backlog));
}

/*****************************************************************************/
/*  t_accept - Custom version of socket accept API- please see Api_accept for details
 * RETURNS: A_OK or A_ERROR
 *****************************************************************************/
int32_t t_accept(QCA_CONTEXT_STRUCT_PTR qca_ptr, uint32_t handle, void *name, socklen_t length)
{
    void *pCxt;

    if ((pCxt = qca_ptr->MAC_CONTEXT_PTR) == NULL)
    {
        return A_ERROR;
    }
    /*Wait for chip to be up*/
    if (A_OK != Api_DriverAccessCheck(pCxt, 1, ACCESS_REQUEST_IOCTL))
    {
        return A_ERROR;
    }
    return (Api_accept(pCxt, handle, name, length));
}

#if T_SELECT_VER1
int32_t t_accept_nb(QCA_CONTEXT_STRUCT_PTR qca_ptr, uint32_t handle, void *name, socklen_t length)
{
    void *pCxt;

    if ((pCxt = qca_ptr->MAC_CONTEXT_PTR) == NULL)
    {
        return A_ERROR;
    }
    /*Wait for chip to be up*/
    if (A_OK != Api_DriverAccessCheck(pCxt, 1, ACCESS_REQUEST_IOCTL))
    {
        return A_ERROR;
    }

    return (Api_accept_ver1(pCxt, handle, name, length));
}

int32_t t_select_ver1(
    QCA_CONTEXT_STRUCT_PTR qca_ptr, int32_t num, uint32_t *r_fd, uint32_t *w_fd, uint32_t *e_fd, uint32_t tv)
{
    void *pCxt;

    if ((pCxt = qca_ptr->MAC_CONTEXT_PTR) == NULL)
    {
        return A_ERROR;
    }

    /*Wait for chip to be up*/
    if (A_OK != Api_DriverAccessCheck(pCxt, 1, ACCESS_REQUEST_RX))
    {
        return A_ERROR;
    }

    return (Api_select_ver1(pCxt, num, r_fd, w_fd, e_fd, tv));
}

#endif

/*****************************************************************************/
/*  t_select - Custom version of socket select API- please see Api_select for details
 * RETURNS: A_OK or A_ERROR
 *****************************************************************************/
int32_t t_select(QCA_CONTEXT_STRUCT_PTR qca_ptr, uint32_t handle, uint32_t tv)
{
    void *pCxt;

    if ((pCxt = qca_ptr->MAC_CONTEXT_PTR) == NULL)
    {
        return A_ERROR;
    }
    /*Wait for chip to be up*/
    if (A_OK != Api_DriverAccessCheck(pCxt, 1, ACCESS_REQUEST_RX))
    {
        return A_ERROR;
    }

    return (Api_select(pCxt, handle, tv));
}

/*****************************************************************************/
/*  t_errno - Custom version of socket errno API- please see Api_errno for details
 * RETURNS: A_OK or A_ERROR
 *****************************************************************************/
int32_t t_errno(QCA_CONTEXT_STRUCT_PTR qca_ptr, uint32_t handle)
{
    void *pCxt;

    if ((pCxt = qca_ptr->MAC_CONTEXT_PTR) == NULL)
    {
        return A_ERROR;
    }
    /*Wait for chip to be up*/
    if (A_OK != Api_DriverAccessCheck(pCxt, 1, ACCESS_REQUEST_IOCTL))
    {
        return A_ERROR;
    }

    return (Api_errno(pCxt, handle));
}

/*****************************************************************************/
/*  t_setsockopt - Custom version of socket setsockopt API- please see Api_setsockopt for details
 * RETURNS: A_OK or A_ERROR
 *****************************************************************************/
int32_t t_setsockopt(
    QCA_CONTEXT_STRUCT_PTR qca_ptr, uint32_t handle, uint32_t level, uint32_t optname, uint8_t *optval, uint32_t optlen)
{
    void *pCxt;

    if ((pCxt = qca_ptr->MAC_CONTEXT_PTR) == NULL)
    {
        return A_ERROR;
    }
    /*Wait for chip to be up*/
    if (A_OK != Api_DriverAccessCheck(pCxt, 1, ACCESS_REQUEST_IOCTL))
    {
        return A_ERROR;
    }

    return (Api_setsockopt(pCxt, handle, level, optname, optval, optlen));
}

/*****************************************************************************/
/*  t_getsockopt - Custom version of socket getsockopt API- please see
 *                 Api_getsockopt for details.
 * RETURNS: A_OK or A_ERROR
 *****************************************************************************/
int32_t t_getsockopt(
    QCA_CONTEXT_STRUCT_PTR qca_ptr, uint32_t handle, uint32_t level, uint32_t optname, uint8_t *optval, uint32_t optlen)
{
    void *pCxt;

    if ((pCxt = qca_ptr->MAC_CONTEXT_PTR) == NULL)
    {
        return A_ERROR;
    }
    /*Wait for chip to be up*/
    if (A_OK != Api_DriverAccessCheck(pCxt, 1, ACCESS_REQUEST_IOCTL))
    {
        return A_ERROR;
    }

    return (Api_getsockopt(pCxt, handle, level, optname, optval, optlen));
}

/*****************************************************************************/
/*  t_ipconfig - Custom version of ipconfig API- please see
 *                 Api_ipconfig for details.
 * RETURNS: A_OK or A_ERROR
 *****************************************************************************/
// int32_t t_ipconfig(QCA_CONTEXT_STRUCT_PTR qca_ptr, uint32_t mode,uint32_t* ipv4_addr, uint32_t* subnetMask, uint32_t*
// gateway4)
A_STATUS t_ipconfig(void *handle,
                   uint32_t mode,
                   uint32_t *ipv4_addr,
                   uint32_t *subnetMask,
                   uint32_t *gateway4,
                   IP46ADDR *dnsaddr,
                   char *hostname)
{
    void *pCxt = ((QCA_CONTEXT_STRUCT_PTR)handle)->MAC_CONTEXT_PTR;
    int32_t result;

    if (pCxt == NULL)
    {
        return A_ERROR;
    }

    /*Wait for chip to be up*/
    if (A_OK != Api_DriverAccessCheck(pCxt, 1, ACCESS_REQUEST_IOCTL))
    {
        return A_ERROR;
    }

    result = Api_ipconfig(pCxt, mode, ipv4_addr, subnetMask, gateway4, dnsaddr, hostname);
    return result == 0 ? A_OK : A_ERROR;
}

/*****************************************************************************/
/*  t_ip6config - Custom version of ipconfig API- please see
 *                 Api_ip6config for details.
 * RETURNS: A_OK or A_ERROR
 *****************************************************************************/
A_STATUS t_ip6config(QCA_CONTEXT_STRUCT_PTR qca_ptr,
                    uint32_t mode,
                    IP6_ADDR_T *v6Global,
                    IP6_ADDR_T *v6Local,
                    IP6_ADDR_T *v6DefGw,
                    IP6_ADDR_T *v6GlobalExtd,
                    int32_t *LinkPrefix,
                    int32_t *GlbPrefix,
                    int32_t *DefGwPrefix,
                    int32_t *GlbPrefixExtd)
{
    void *pCxt;
    int32_t result;

    if ((pCxt = qca_ptr->MAC_CONTEXT_PTR) == NULL)
    {
        return A_ERROR;
    }
    /*Wait for chip to be up*/
    if (A_OK != Api_DriverAccessCheck(pCxt, 1, ACCESS_REQUEST_IOCTL))
    {
        return A_ERROR;
    }

    result = Api_ip6config(pCxt, mode, v6Global, v6Local, v6DefGw, v6GlobalExtd, LinkPrefix, GlbPrefix, DefGwPrefix,
                          GlbPrefixExtd);
    return 0 == result ? A_OK : A_ERROR;
}

/*****************************************************************************/
/*  t_ipconfig_dhcp_pool -  Function to create DHCP Pool
 * RETURNS: A_OK or A_ERROR
 *****************************************************************************/
int32_t t_ipconfig_dhcp_pool(QCA_CONTEXT_STRUCT_PTR qca_ptr,
                             uint32_t *start_ipv4_addr,
                             uint32_t *end_ipv4_addr,
                             int32_t leasetime)
{
    void *pCxt;

    if ((pCxt = qca_ptr->MAC_CONTEXT_PTR) == NULL)
    {
        return A_ERROR;
    }
    /*Wait for chip to be up*/
    if (A_OK != Api_DriverAccessCheck(pCxt, 1, ACCESS_REQUEST_IOCTL))
    {
        return A_ERROR;
    }

    return (Api_ipconfig_dhcp_pool(pCxt, start_ipv4_addr, end_ipv4_addr, leasetime));
}

/*****************************************************************************/
/*  t_ipconfig_dhcps_cb_enable -  Function to register DHCP Server Callback
          Callback is invoked with the client MAC Addr
           and the IP address assigned
 * RETURNS: A_OK or A_ERROR
 *****************************************************************************/
int32_t t_ipconfig_dhcps_cb_enable(QCA_CONTEXT_STRUCT_PTR qca_ptr, void *callback)
{
    if (qca_ptr->MAC_CONTEXT_PTR == NULL)
    {
        return A_ERROR;
    }
    /*Register the dhcp callback*/
    dhcps_success_callback = (uint32_t (*)(uint8_t *, uint32_t))callback;
    return A_OK;
}

/*****************************************************************************/
/*  t_ipconfig_dhcpc_cb_enable -  Function to register DHCP Client Callback
          Callback is invoked with the client's IP,
           Mask and the Gateway Address
 * RETURNS: A_OK or A_ERROR
 *****************************************************************************/
int32_t t_ipconfig_dhcpc_cb_enable(QCA_CONTEXT_STRUCT_PTR qca_ptr, void *callback)
{
    if (qca_ptr->MAC_CONTEXT_PTR == NULL)
    {
        return A_ERROR;
    }
    /*Register the dhcp callback*/
    dhcpc_success_callback = (uint32_t (*)(uint32_t, uint32_t, uint32_t))callback;
    return A_OK;
}

A_STATUS t_ip6config_router_prefix(
    QCA_CONTEXT_STRUCT_PTR qca_ptr, IP6_ADDR_T *v6addr, int32_t prefixlen, int32_t prefix_lifetime, int32_t valid_lifetime)
{
    void *pCxt;
    int32_t result;

    if ((pCxt = qca_ptr->MAC_CONTEXT_PTR) == NULL)
    {
        return A_ERROR;
    }
    /*Wait for chip to be up*/
    if (A_OK != Api_DriverAccessCheck(pCxt, 1, ACCESS_REQUEST_IOCTL))
    {
        return A_ERROR;
    }

    result = Api_ip6config_router_prefix(pCxt, v6addr, prefixlen, prefix_lifetime, valid_lifetime);
    return 0 == result ? A_OK : A_ERROR;
}

/*****************************************************************************/
/*  custom_ipbridgemode - Custom version of bridgemode API- please see
 *                 Api_ipbridgemode for details.
 * RETURNS: A_OK or A_ERROR
 *****************************************************************************/
A_STATUS custom_ipbridgemode(void *handle, uint16_t status)
{
    void *pCxt = ((QCA_CONTEXT_STRUCT_PTR)handle)->MAC_CONTEXT_PTR;
    int32_t result;

    if (pCxt == NULL)
    {
        return A_ERROR;
    }

    /*Wait for chip to be up*/
    if (A_OK != Api_DriverAccessCheck(pCxt, 1, ACCESS_REQUEST_IOCTL))
    {
        return A_ERROR;
    }

    result = Api_ipbridgemode(pCxt, status);
    return 0 == result ? A_OK : A_ERROR;
}

A_STATUS custom_ipconfig_set_tcp_exponential_backoff_retry(QCA_CONTEXT_STRUCT_PTR qca_ptr, int32_t retry)
{
    void *pCxt;
    int32_t result;

    if ((pCxt = qca_ptr->MAC_CONTEXT_PTR) == NULL)
    {
        return A_ERROR;
    }
    /*Wait for chip to be up*/
    if (A_OK != Api_DriverAccessCheck(pCxt, 1, ACCESS_REQUEST_IOCTL))
    {
        return A_ERROR;
    }

    result = Api_ipconfig_set_tcp_exponential_backoff_retry(pCxt, retry);
    return 0 == result ? A_OK : A_ERROR;
}

int32_t custom_ipconfig_set_ip6_status(QCA_CONTEXT_STRUCT_PTR qca_ptr, uint16_t status)
{
    void *pCxt;

    if ((pCxt = qca_ptr->MAC_CONTEXT_PTR) == NULL)
    {
        return A_ERROR;
    }
    /*Wait for chip to be up*/
    if (A_OK != Api_DriverAccessCheck(pCxt, 1, ACCESS_REQUEST_IOCTL))
    {
        return A_ERROR;
    }
    return (Api_ipconfig_set_ip6_status(pCxt, status));
}

A_STATUS custom_ipconfig_dhcp_release(QCA_CONTEXT_STRUCT_PTR qca_ptr)
{
    void *pCxt;
    int32_t result;

    if ((pCxt = qca_ptr->MAC_CONTEXT_PTR) == NULL)
    {
        return A_ERROR;
    }
    /*Wait for chip to be up*/
    if (A_OK != Api_DriverAccessCheck(pCxt, 1, ACCESS_REQUEST_IOCTL))
    {
        return A_ERROR;
    }

    result = Api_ipconfig_dhcp_release(pCxt);
    return 0 == result ? A_OK : A_ERROR;
}
int32_t Custom_Api_Dhcps_Success_Callback_Event(void *pCxt, uint8_t *datap)
{
    SOCK_DHCPS_SUCCESS_CALLBACK_T *dhcps_cb_info = (SOCK_DHCPS_SUCCESS_CALLBACK_T *)datap;
    if (dhcps_success_callback)
    {
        dhcps_success_callback(dhcps_cb_info->mac, dhcps_cb_info->ip);
    }
    return A_OK;
}

int32_t Custom_Api_Dhcpc_Success_Callback_Event(void *pCxt, uint8_t *datap)
{
    SOCK_DHCPC_SUCCESS_CALLBACK_T *dhcpc_cb_info = (SOCK_DHCPC_SUCCESS_CALLBACK_T *)datap;
    if (dhcpc_success_callback)
    {
        dhcpc_success_callback(dhcpc_cb_info->ip, dhcpc_cb_info->mask, dhcpc_cb_info->gw);
    }
    return A_OK;
}

A_STATUS custom_ipconfig_set_tcp_rx_buffer(QCA_CONTEXT_STRUCT_PTR qca_ptr, int32_t rxbuf)
{
    void *pCxt;
    int32_t result;

    if ((pCxt = qca_ptr->MAC_CONTEXT_PTR) == NULL)
    {
        return A_ERROR;
    }
    /*Wait for chip to be up*/
    if (A_OK != Api_DriverAccessCheck(pCxt, 1, ACCESS_REQUEST_IOCTL))
    {
        return A_ERROR;
    }

    result = Api_ipconfig_set_tcp_rx_buffer(pCxt, rxbuf);
    return 0 == result ? A_OK : A_ERROR;
}

#if ENABLE_HTTP_SERVER
A_STATUS custom_ip_http_server(void *handle, int32_t command)
{
    int32_t result;
#if 0 /* keviny */
    void *pCxt = handle;
#else
    void *pCxt = ((QCA_CONTEXT_STRUCT_PTR)handle)->MAC_CONTEXT_PTR;
#endif

    if (pCxt == NULL)
    {
        return A_ERROR;
    }

    /*Wait for chip to be up*/
    if (A_OK != Api_DriverAccessCheck(pCxt, 1, ACCESS_REQUEST_IOCTL))
    {
        return A_ERROR;
    }

    result = Api_ip_http_server(pCxt, command);
    return 0 == result ? A_OK : A_ERROR;
}

A_STATUS custom_ip_http_server_method(
    void *handle, int32_t command, uint8_t *pagename, uint8_t *objname, int32_t objtype, int32_t objlen, uint8_t *value)
{
    int32_t result;
#if 0 /* keviny */
		void *pCxt = handle;
#else
    void *pCxt = ((QCA_CONTEXT_STRUCT_PTR)handle)->MAC_CONTEXT_PTR;
#endif

    if (pCxt == NULL)
    {
        return A_ERROR;
    }

    /*Wait for chip to be up*/
    if (A_OK != Api_DriverAccessCheck(pCxt, 1, ACCESS_REQUEST_IOCTL))
    {
        return A_ERROR;
    }

    result = Api_ip_http_server_method(pCxt, command, pagename, objname, objtype, objlen, value);
    return 0 == result ? A_OK : A_ERROR;
}

void Custom_Api_Http_Post_Event(void *pCxt, uint8_t *datap)
{
    WMI_HTTP_POST_EVENT_CB cb = NULL;

    DRIVER_SHARED_RESOURCE_ACCESS_ACQUIRE(pCxt);

    cb = (WMI_HTTP_POST_EVENT_CB)(GET_DRIVER_CXT(pCxt)->httpPostCb);

    DRIVER_SHARED_RESOURCE_ACCESS_RELEASE(pCxt);

    if (cb)
    {
        cb(GET_DRIVER_CXT(pCxt)->httpPostCbCxt, (void *)datap);
    }
}

int32_t custom_http_set_post_cb(void *handle, void *cxt, void *callback)
{
    void *pCxt = ((QCA_CONTEXT_STRUCT_PTR)handle)->MAC_CONTEXT_PTR;

    GET_DRIVER_CXT(pCxt)->httpPostCb = callback;
    GET_DRIVER_CXT(pCxt)->httpPostCbCxt = cxt;
    return A_OK;
}
#endif

#if ENABLE_HTTP_CLIENT

/*****************************************************************************/
/*  cust_httpc_method - Custom version of httpc API- please see Api_httpc_connect for details
* RETURNS: A_OK or A_ERROR
*****************************************************************************/
A_STATUS custom_httpc_method(void *handle, uint32_t command, uint8_t *url, uint8_t *data, uint8_t **output)
{
    void *pCxt = ((QCA_CONTEXT_STRUCT_PTR)handle)->MAC_CONTEXT_PTR;
    int32_t result;

    if (pCxt == NULL)
    {
        return A_ERROR;
    }
    /*Wait for chip to be up*/
    if (A_OK != Api_DriverAccessCheck(pCxt, 1, ACCESS_REQUEST_IOCTL))
    {
        return A_ERROR;
    }

    result = Api_httpc_method(pCxt, command, url, data, output);
    return 0 == result ? A_OK : A_ERROR;
}

/*****************************************************************************/
/*  zero_copy_http_free - ZERO_COPY free API.It will find and remove rx buffer
 *                  for HTTP client.
 * RETURNS: Nothing
 *****************************************************************************/
void zero_copy_http_free(void *buffer)
{
    uint32_t *length;

    length = (uint32_t *)buffer;
    buffer = (void *)(length - 3);
    // printf("Deleting from Q %p\n", buffer);
    zero_copy_free(buffer);
}
#endif /* ENABLE_HTTP_CLIENT */

int32_t custom_ip_hostname(void *handle, char *domain_name)
{
    void *pCxt = ((QCA_CONTEXT_STRUCT_PTR)handle)->MAC_CONTEXT_PTR;

    if (pCxt == NULL)
    {
        return A_ERROR;
    }
    /*Wait for chip to be up*/
    if (A_OK != Api_DriverAccessCheck(pCxt, 1, ACCESS_REQUEST_IOCTL))
    {
        return A_ERROR;
    }
    return (Api_ip_hostname(pCxt, domain_name));
}

#if ENABLE_DNS_SERVER
int32_t custom_ip_dns_local_domain(void *handle, char *domain_name)
{
    void *pCxt = ((QCA_CONTEXT_STRUCT_PTR)handle)->MAC_CONTEXT_PTR;
    if (pCxt == NULL)
    {
        return A_ERROR;
    }
    /*Wait for chip to be up*/
    if (A_OK != Api_DriverAccessCheck(pCxt, 1, ACCESS_REQUEST_IOCTL))
    {
        return A_ERROR;
    }
    return (Api_ip_dns_local_domain(pCxt, domain_name));
}

int32_t custom_ipdns(void *handle, int32_t command, char *domain_name, IP46ADDR *dnsaddr)
{
    void *pCxt = ((QCA_CONTEXT_STRUCT_PTR)handle)->MAC_CONTEXT_PTR;

    if (pCxt == NULL)
    {
        return A_ERROR;
    }
    /*Wait for chip to be up*/
    if (A_OK != Api_DriverAccessCheck(pCxt, 1, ACCESS_REQUEST_IOCTL))
    {
        return A_ERROR;
    }
    return (Api_ipdns(pCxt, command, domain_name, dnsaddr));
}

int32_t custom_ip_dns_server(void *handle, uint32_t command)
{
    void *pCxt = ((QCA_CONTEXT_STRUCT_PTR)handle)->MAC_CONTEXT_PTR;
    if (pCxt == NULL)
    {
        return A_ERROR;
    }
    /*Wait for chip to be up*/
    if (A_OK != Api_DriverAccessCheck(pCxt, 1, ACCESS_REQUEST_IOCTL))
    {
        return A_ERROR;
    }

    return (Api_ip_dns_server(pCxt, command));
}

#endif /* ENABLE_DNS_SERVER */

#if ENABLE_DNS_CLIENT
int32_t custom_ip_set_dns_block_time(void *handle, int32_t blockSec)
{
    if (blockSec <= 0)
    {
        dns_block_time = DEFAULT_DNS_TIMEOUT;
    }
    else
    {
        dns_block_time = (blockSec > 0xFF ? 0xFF : blockSec) * 1000;
    }
    return A_OK;
}

int32_t custom_ip_resolve_hostname(void *handle, DNC_CFG_CMD *DncCfg, DNC_RESP_INFO *DncRespInfo)
{
    void *pCxt = ((QCA_CONTEXT_STRUCT_PTR)handle)->MAC_CONTEXT_PTR;

    if (pCxt == NULL)
    {
        return A_ERROR;
    }

    /*Wait for chip to be up*/
    if (A_OK != Api_DriverAccessCheck(pCxt, 1, ACCESS_REQUEST_IOCTL))
    {
        return A_ERROR;
    }
    return (Api_ip_resolve_host_name(pCxt, DncCfg, DncRespInfo));
}

int32_t custom_ip_dns_server_addr(void *handle, IP46ADDR *addr)
{
    void *pCxt = ((QCA_CONTEXT_STRUCT_PTR)handle)->MAC_CONTEXT_PTR;
    if (pCxt == NULL)
    {
        return A_ERROR;
    }
    /*Wait for chip to be up*/
    if (A_OK != Api_DriverAccessCheck(pCxt, 1, ACCESS_REQUEST_IOCTL))
    {
        return A_ERROR;
    }
    return (Api_ip_dns_server_addr(pCxt, addr));
}

int32_t custom_ip_dns_client(void *handle, int32_t command)
{
    void *pCxt = ((QCA_CONTEXT_STRUCT_PTR)handle)->MAC_CONTEXT_PTR;

    if (pCxt == NULL)
    {
        return A_ERROR;
    }

    /*Wait for chip to be up*/
    if (A_OK != Api_DriverAccessCheck(pCxt, 1, ACCESS_REQUEST_IOCTL))
    {
        return A_ERROR;
    }
    return (Api_ip_dns_client(pCxt, command));
}
#endif /* ENABLE_DNS_CLIENT */

#if ENABLE_SNTP_CLIENT
int32_t custom_ip_sntp_srvr_addr(void *handle, int32_t command, char *sntp_srvr_addr)
{
    void *pCxt = ((QCA_CONTEXT_STRUCT_PTR)handle)->MAC_CONTEXT_PTR;

    if (pCxt == NULL)
    {
        return A_ERROR;
    }
    /*Wait for chip to be up*/
    if (A_OK != Api_DriverAccessCheck(pCxt, 1, ACCESS_REQUEST_IOCTL))
    {
        return A_ERROR;
    }
    return (Api_ip_sntp_srvr_addr(pCxt, command, sntp_srvr_addr));
}

int32_t custom_ip_sntp_get_time(void *handle, tSntpTime *SntpTime)
{
    void *pCxt = ((QCA_CONTEXT_STRUCT_PTR)handle)->MAC_CONTEXT_PTR;

    if (pCxt == NULL)
    {
        return A_ERROR;
    }
    /*Wait for chip to be up*/
    if (A_OK != Api_DriverAccessCheck(pCxt, 1, ACCESS_REQUEST_IOCTL))
    {
        return A_ERROR;
    }
    return (Api_ip_sntp_get_time(pCxt, SntpTime));
}
int32_t custom_ip_sntp_get_time_of_day(void *handle, tSntpTM *SntpTm)
{
    void *pCxt = ((QCA_CONTEXT_STRUCT_PTR)handle)->MAC_CONTEXT_PTR;

    if (pCxt == NULL)
    {
        return A_ERROR;
    }
    /*Wait for chip to be up*/
    if (A_OK != Api_DriverAccessCheck(pCxt, 1, ACCESS_REQUEST_IOCTL))
    {
        return A_ERROR;
    }
    return (Api_ip_sntp_get_time_of_day(pCxt, SntpTm));
}
int32_t custom_ip_sntp_modify_zone_dse(void *handle, uint8_t hour, uint8_t min, uint8_t add_sub, uint8_t dse)
{
    void *pCxt = ((QCA_CONTEXT_STRUCT_PTR)handle)->MAC_CONTEXT_PTR;

    if (pCxt == NULL)
    {
        return A_ERROR;
    }
    /*Wait for chip to be up*/
    if (A_OK != Api_DriverAccessCheck(pCxt, 1, ACCESS_REQUEST_IOCTL))
    {
        return A_ERROR;
    }
    return (Api_ip_sntp_modify_zone_dse(pCxt, hour, min, add_sub, dse));
}

int32_t custom_ip_sntp_query_srvr_address(void *handle, tSntpDnsAddr SntpDnsAddr[MAX_SNTP_SERVERS])
{
    void *pCxt = ((QCA_CONTEXT_STRUCT_PTR)handle)->MAC_CONTEXT_PTR;

    if (pCxt == NULL)
    {
        return A_ERROR;
    }
    /*Wait for chip to be up*/
    if (A_OK != Api_DriverAccessCheck(pCxt, 1, ACCESS_REQUEST_IOCTL))
    {
        return A_ERROR;
    }
    return (Api_ip_sntp_query_srvr_address(pCxt, SntpDnsAddr));
}

int32_t custom_ip_sntp_client(void *handle, int32_t command)
{
    void *pCxt = ((QCA_CONTEXT_STRUCT_PTR)handle)->MAC_CONTEXT_PTR;

    if (pCxt == NULL)
    {
        return A_ERROR;
    }

    /*Wait for chip to be up*/
    if (A_OK != Api_DriverAccessCheck(pCxt, 1, ACCESS_REQUEST_IOCTL))
    {
        return A_ERROR;
    }
    return (Api_ip_sntp_client(pCxt, command));
}
#endif /* ENABLE_SNTP_CLIENT */

#if ENABLE_ROUTING_CMDS
A_STATUS custom_ipv4_route(void *handle,
                          uint32_t command,
                          IP_ADDR_T *ipv4_addr,
                          IP_ADDR_T *subnetMask,
                          IP_ADDR_T *gateway,
                          uint32_t *ifIndex,
                          IPV4_ROUTE_LIST_T *routelist)
{
    void *pCxt = ((QCA_CONTEXT_STRUCT_PTR)handle)->MAC_CONTEXT_PTR;
    int32_t result;

    if (pCxt == NULL)
    {
        return A_ERROR;
    }

    /*Wait for chip to be up*/
    if (A_OK != Api_DriverAccessCheck(pCxt, 1, ACCESS_REQUEST_IOCTL))
    {
        return A_ERROR;
    }

    result = Api_ipv4_route(pCxt, command, ipv4_addr, subnetMask, gateway, ifIndex, routelist);
    return 0 == result ? A_OK : A_ERROR;
}

A_STATUS custom_ipv6_route(void *handle,
                          uint32_t command,
                          IP6_ADDR_T *ipv6_addr,
                          uint32_t *prefixLen,
                          IP6_ADDR_T *gateway,
                          uint32_t *ifIndex,
                          IPV6_ROUTE_LIST_T *routelist)
{
    void *pCxt = ((QCA_CONTEXT_STRUCT_PTR)handle)->MAC_CONTEXT_PTR;
    int32_t result;

    if (pCxt == NULL)
    {
        return A_ERROR;
    }

    /*Wait for chip to be up*/
    if (A_OK != Api_DriverAccessCheck(pCxt, 1, ACCESS_REQUEST_IOCTL))
    {
        return A_ERROR;
    }

    result = Api_ipv6_route(pCxt, command, ipv6_addr, prefixLen, gateway, ifIndex, routelist);
    return 0 == result ? A_OK : A_ERROR;
}
#endif /* ENABLE_ROUTING_CMDS */
A_STATUS custom_tcp_connection_timeout(void *handle, uint32_t timeout_val)
{
    void *pCxt = ((QCA_CONTEXT_STRUCT_PTR)handle)->MAC_CONTEXT_PTR;
    int32_t result;

    if (pCxt == NULL)
    {
        return A_ERROR;
    }
    /*Wait for chip to be up*/
    if (A_OK != Api_DriverAccessCheck(pCxt, 1, ACCESS_REQUEST_IOCTL))
    {
        return A_ERROR;
    }

    result = Api_tcp_connection_timeout(pCxt, timeout_val);
    return 0 == result ? A_OK : A_ERROR;
}
int32_t custom_ota_upgrade(void *handle,
                           uint32_t addr,
                           char *filename,
                           uint8_t mode,
                           uint8_t preserve_last,
                           uint8_t protocol,
                           uint32_t *resp_code,
                           uint32_t *ret_len)
{
    void *pCxt = ((QCA_CONTEXT_STRUCT_PTR)handle)->MAC_CONTEXT_PTR;

    if (pCxt == NULL)
    {
        return A_ERROR;
    }
    /*Wait for chip to be up*/
    if (A_OK != Api_DriverAccessCheck(pCxt, 1, ACCESS_REQUEST_IOCTL))
    {
        return A_ERROR;
    }

    return (Api_ota_upgrade(pCxt, addr, filename, mode, preserve_last, protocol, resp_code, ret_len));
}
int32_t custom_ota_read_area(void *handle, uint32_t offset, uint32_t size, uint8_t *buffer, uint32_t *ret_len)
{
    void *pCxt = ((QCA_CONTEXT_STRUCT_PTR)handle)->MAC_CONTEXT_PTR;

    if (pCxt == NULL)
    {
        return A_ERROR;
    }
    /*Wait for chip to be up*/
    if (A_OK != Api_DriverAccessCheck(pCxt, 1, ACCESS_REQUEST_IOCTL))
    {
        return A_ERROR;
    }

    return (Api_ota_read_area(pCxt, offset, size, buffer, ret_len));
}

int32_t custom_ota_done(void *handle, boolean good_image)
{
    void *pCxt = ((QCA_CONTEXT_STRUCT_PTR)handle)->MAC_CONTEXT_PTR;

    if (pCxt == NULL)
    {
        return A_ERROR;
    }
    /*Wait for chip to be up*/
    if (A_OK != Api_DriverAccessCheck(pCxt, 1, ACCESS_REQUEST_IOCTL))
    {
        return A_ERROR;
    }

    return (Api_ota_done(pCxt, good_image));
}

int32_t custom_ota_session_start(void *handle, uint32_t flags, uint32_t partition_index)
{
    void *pCxt = ((QCA_CONTEXT_STRUCT_PTR)handle)->MAC_CONTEXT_PTR;

    if (pCxt == NULL)
    {
        return A_ERROR;
    }
    /*Wait for chip to be up*/
    if (A_OK != Api_DriverAccessCheck(pCxt, 1, ACCESS_REQUEST_IOCTL))
    {
        return A_ERROR;
    }

    return (Api_ota_session_start(pCxt, flags, partition_index));
}

uint32_t custom_ota_partition_get_size(void *handle)
{
    void *pCxt = ((QCA_CONTEXT_STRUCT_PTR)handle)->MAC_CONTEXT_PTR;

    if (pCxt == NULL)
    {
        return 0;
    }
    /*Wait for chip to be up*/
    if (A_OK != Api_DriverAccessCheck(pCxt, 1, ACCESS_REQUEST_IOCTL))
    {
        return 0;
    }

    return (Api_ota_partition_get_size(pCxt));
}

int32_t custom_ota_partition_erase(void *handle)
{
    void *pCxt = ((QCA_CONTEXT_STRUCT_PTR)handle)->MAC_CONTEXT_PTR;

    if (pCxt == NULL)
    {
        return A_ERROR;
    }
    /*Wait for chip to be up*/
    if (A_OK != Api_DriverAccessCheck(pCxt, 1, ACCESS_REQUEST_IOCTL))
    {
        return A_ERROR;
    }

    return (Api_ota_partition_erase(pCxt));
}

int32_t custom_ota_partition_verify_checksum(void *handle)
{
    void *pCxt = ((QCA_CONTEXT_STRUCT_PTR)handle)->MAC_CONTEXT_PTR;

    if (pCxt == NULL)
    {
        return A_ERROR;
    }
    /*Wait for chip to be up*/
    if (A_OK != Api_DriverAccessCheck(pCxt, 1, ACCESS_REQUEST_IOCTL))
    {
        return A_ERROR;
    }

    return (Api_ota_partition_verify_checksum(pCxt));
}

int32_t custom_ota_parse_image_hdr(void *handle, uint8_t *header, uint32_t *offset)
{
    void *pCxt = ((QCA_CONTEXT_STRUCT_PTR)handle)->MAC_CONTEXT_PTR;

    if (pCxt == NULL)
    {
        return A_ERROR;
    }
    /*Wait for chip to be up*/
    if (A_OK != Api_DriverAccessCheck(pCxt, 1, ACCESS_REQUEST_IOCTL))
    {
        return A_ERROR;
    }

    return (Api_ota_parse_image_hdr(pCxt, header, offset));
}

int32_t custom_ota_partition_write_data(void *handle, uint32_t offset, uint8_t *buf, uint32_t size, uint32_t *ret_size)
{
    void *pCxt = ((QCA_CONTEXT_STRUCT_PTR)handle)->MAC_CONTEXT_PTR;

    if (pCxt == NULL)
    {
        return 0;
    }
    /*Wait for chip to be up*/
    if (A_OK != Api_DriverAccessCheck(pCxt, 1, ACCESS_REQUEST_IOCTL))
    {
        return 0;
    }

    return (Api_ota_partition_write_data(pCxt, offset, buf, size, ret_size));
}

int32_t custom_ota_set_response_cb(void *handle, void *callback)
{
    void *pCxt = ((QCA_CONTEXT_STRUCT_PTR)handle)->MAC_CONTEXT_PTR;

    GET_DRIVER_CXT(pCxt)->otaCB = callback;
    return A_OK;
}

void Custom_Api_Ota_Resp_Result(void *pCxt, uint32_t cmd, uint32_t resp_code, uint32_t result)
{
    ATH_OTA_CB cb = NULL;
    DRIVER_SHARED_RESOURCE_ACCESS_ACQUIRE(pCxt);
    cb = (ATH_OTA_CB)(GET_DRIVER_CXT(pCxt)->otaCB);
    DRIVER_SHARED_RESOURCE_ACCESS_RELEASE(pCxt);

    if (cb)
    {
        cb(cmd, resp_code, result);
    }
}

/*****************************************************************************/
/*  t_ping - Custom version of ping API- please see
 *                 Api_ping for details.
 * RETURNS: A_OK or A_ERROR
 *****************************************************************************/
A_STATUS t_ping(QCA_CONTEXT_STRUCT_PTR qca_ptr, uint32_t ipv4_addr, uint32_t size, uint32_t ms_interval)
{
    void *pCxt;
    int32_t result;

    if ((pCxt = qca_ptr->MAC_CONTEXT_PTR) == NULL)
    {
        return A_ERROR;
    }
    /*Wait for chip to be up*/
    if (A_OK != Api_DriverAccessCheck(pCxt, 1, ACCESS_REQUEST_IOCTL))
    {
        return A_ERROR;
    }

    result = Api_ping(pCxt, ipv4_addr, size, ms_interval);
    return result == 0 ? A_OK : A_ERROR;
}

/*****************************************************************************/
/*  t_ping6 - Custom version of ping API- please see
 *                 Api_ping6 for details.
 * RETURNS: A_OK or A_ERROR
 *****************************************************************************/
A_STATUS t_ping6(QCA_CONTEXT_STRUCT_PTR qca_ptr, uint8_t *ip6addr, uint32_t size, uint32_t ms_interval)
{
    void *pCxt;
    int32_t result;

    if ((pCxt = qca_ptr->MAC_CONTEXT_PTR) == NULL)
    {
        return A_ERROR;
    }

    /*Wait for chip to be up*/
    if (A_OK != Api_DriverAccessCheck(pCxt, 1, ACCESS_REQUEST_IOCTL))
    {
        return A_ERROR;
    }

    result = Api_ping6(pCxt, ip6addr, size, ms_interval);
    return 0 == result ? A_OK : A_ERROR;
}

/*****************************************************************************/
/*  t_send - Custom version of socket send API. Used for stream based sockets.
 * 			 Sends provided buffer to target.
 * RETURNS: Number of bytes sent to target
 *****************************************************************************/
int32_t t_send(QCA_CONTEXT_STRUCT_PTR qca_ptr, uint32_t handle, uint8_t *buffer, uint32_t length, uint32_t flags)
{
    int32_t index;
    int32_t result = 0;

    /* This function is dedicated to TCP sockets, so transfer *should* pass without an error.
     * Perform several attempts before give up on DEADLOCK state. */
    for (int deadlock_attempts = 0; deadlock_attempts < 50; deadlock_attempts++)
    {
        /* Find socket context */
        if((index = find_socket_context(handle, true)) == SOCKET_NOT_FOUND)
        {
            last_driver_error = A_SOCKCXT_NOT_FOUND;
            return A_SOCK_INVALID;
        }

        if( ath_sock_context[index]->TCPCtrFlag == TCP_FIN )
        {
            last_driver_error = A_SOCKCXT_NOT_FOUND;
            return A_SOCK_INVALID;
        }

        /* Send data to 'driver_task' */
        result = t_sendto(qca_ptr, handle, buffer, length, flags, NULL, 0);

        /* If 'driver_task' refuses to send data in case of 'deadlock' state,
         * then put API task to sleep and try to send data later */
        if (A_DEADLOCK == result)
        {
            A_MDELAY(70);
            continue;
        }
        else
        {
            break;
        }
    }

    return result;
}

/*****************************************************************************/
/*  t_sendto - Custom version of socket send API. Used for datagram sockets.
 * 		Sends provided buffer to target. Creates space for headroom at
 *              the begining of the packet. The headroom is used by the stack to
 *              fill in the TCP-IP header.
 * RETURNS: Number of bytes sent to target
 *****************************************************************************/
int32_t t_sendto(QCA_CONTEXT_STRUCT_PTR qca_ptr,
                 uint32_t handle,
                 uint8_t *buffer,
                 uint32_t length,
                 uint32_t flags,
                 void *name,
                 uint32_t socklength)
{
    uint8_t custom_hdr[TCP6_HEADROOM];
    int32_t index = 0;
    uint16_t custom_hdr_length = 0;
    DRV_BUFFER_PTR db_ptr;
    uint16_t hdr_length = sizeof(ATH_MAC_HDR);
    uint8_t *hdr = NULL, *bufPtr = buffer;
    void *pCxt;
#if NON_BLOCKING_TX
    SOCKET_CONTEXT_PTR pcustctxt;
#endif
    int32_t result = 0, block_result = 0;
    A_STATUS status = A_OK;
    uint16_t rem_len = length;
    uint16_t offset = 0;

    if ((pCxt = qca_ptr->MAC_CONTEXT_PTR) == NULL)
    {
        return A_ERROR;
    }

    /*Find socket context*/
    if ((index = find_socket_context(handle, true)) == SOCKET_NOT_FOUND)
    {
        last_driver_error = A_SOCKCXT_NOT_FOUND;
        return A_SOCK_INVALID;
    }

    /* If socket is TCP and not connected, report error */
    if (
        ((ath_sock_context[index]->domain == ATH_AF_INET) && 
         (ath_sock_context[index]->type == SOCK_STREAM_TYPE) && 
         (ath_sock_context[index]->TCPCtrFlag != TCP_CONNECTED)) || 
        ((ath_sock_context[index]->domain == ATH_AF_INET6) && 
         (ath_sock_context[index]->type == SOCK_STREAM_TYPE) && 
         (ath_sock_context[index]->TCPCtrFlag != TCP_CONNECTED))
     )
        return A_ERROR;

    /* Get expected size of header */
    /* TODO: fix for raw socket domain */
    if (ath_sock_context[index]->domain == ATH_AF_INET)
    {
        if (ath_sock_context[index]->type == SOCK_STREAM_TYPE)
            hdr_length = TCP_HEADROOM;
        else
            hdr_length = UDP_HEADROOM;
    }
    else
    {
        if (ath_sock_context[index]->type == SOCK_STREAM_TYPE)
            hdr_length = TCP6_HEADROOM_WITH_NO_OPTION;
        else
            hdr_length = UDP6_HEADROOM;
    }

    while (rem_len != 0)
    {
        length = rem_len;
        A_MEMZERO(custom_hdr, TCP6_HEADROOM);

        /*Calculate fragmentation threshold. We cannot send packets greater that HTC credit size
          over HTC, Bigger packets need to be fragmented. Thresholds are different for IP v4 vs v6*/
        if (ath_sock_context[index]->domain == ATH_AF_INET6)
        {
            custom_hdr_length = sizeof(SOCK_SEND6_T);
            SOCK_SEND6_T *tmp_ipv6 = (SOCK_SEND6_T*)custom_hdr;
            if (name != NULL)
            {
                tmp_ipv6->name6.sin6_port = A_CPU2LE16(((SOCKADDR_6_T *)name)->sin6_port);
                tmp_ipv6->name6.sin6_family = A_CPU2LE16(((SOCKADDR_6_T *)name)->sin6_family);
                tmp_ipv6->name6.sin6_flowinfo = A_CPU2LE32(((SOCKADDR_6_T *)name)->sin6_flowinfo);
                A_MEMCPY((uint8_t *)&(tmp_ipv6->name6.sin6_addr),
                         (uint8_t *)&((SOCKADDR_6_T *)name)->sin6_addr, sizeof(IP6_ADDR_T));
                tmp_ipv6->socklength = A_CPU2LE32(socklength);
            }
            else
            {
                memset((uint8_t *)(&tmp_ipv6->name6), 0, sizeof(name));
            }
        }
        else
        {
            custom_hdr_length = sizeof(SOCK_SEND_T);
            SOCK_SEND_T *tmp_ipv4 = (SOCK_SEND_T*)custom_hdr;
            if (name != NULL)
            {
                tmp_ipv4->name.sin_port = A_CPU2LE16(((SOCKADDR_T *)name)->sin_port);
                tmp_ipv4->name.sin_family = A_CPU2LE16(((SOCKADDR_T *)name)->sin_family);
                tmp_ipv4->name.sin_addr = A_CPU2LE32(((SOCKADDR_T *)name)->sin_addr);
                tmp_ipv4->socklength = A_CPU2LE32(socklength);
            }
            else
            {
                memset((uint8_t *)(&tmp_ipv4->name), 0, sizeof(name));
            }
        }

        /*Populate common fields of custom header, these are the same for IP v4/v6*/
        SOCK_SEND_T *tmp_sock = (SOCK_SEND_T*)custom_hdr;
        tmp_sock->handle = A_CPU2LE32(handle);
        tmp_sock->flags = A_CPU2LE32(flags);

        if (offset != 0)
        {
            memmove(bufPtr, (bufPtr + offset), rem_len);
        }

        db_ptr = &((TX_PACKET_PTR)(bufPtr - TX_PKT_OVERHEAD))->db;
        hdr = ((TX_PACKET_PTR)(bufPtr - TX_PKT_OVERHEAD))->hdr;

        A_MEMZERO(hdr, hdr_length);

#if NON_BLOCKING_TX
        pcustctxt = GET_SOCKET_CONTEXT(ath_sock_context[index]);
        /*In non blocking TX case, Enqueue the packet so that it can be freed later when
                 TX over SPI is successful. Application should not free this buffer*/

        A_MUTEX_ACQUIRE(&pcustctxt->nb_tx_mutex);
        A_NETBUF_ENQUEUE(&(pcustctxt->non_block_queue), bufPtr);
        A_MUTEX_RELEASE(&pcustctxt->nb_tx_mutex);
#endif

        if ((length == 0) || (length > IPV4_FRAGMENTATION_THRESHOLD))
        {
            /*Host fragmentation is not allowed, application must send payload
             below IPV4_FRAGMENTATION_THRESHOLD size*/
            A_ASSERT(0);
        }

        tmp_sock->length = A_CPU2LE32(length);

        A_MEMCPY(hdr, custom_hdr, custom_hdr_length);

        db_ptr->context = (void *)ath_sock_context[index];
        if ((ath_sock_context[index]->type == SOCK_STREAM_TYPE) && (length > TCP_MSS))
        {
            offset = TCP_MSS;
            length = TCP_MSS;
        }

        rem_len -= length;

        db_ptr->bufFragment[0].payload = bufPtr;
        db_ptr->bufFragment[0].length = length;
        result += length;
        status = custom_send_tcpip(pCxt, db_ptr, length, 1, hdr, hdr_length);
        if (status != A_OK)
        {
            goto END_TX;
        }

#if !NON_BLOCKING_TX
        /*Wait till packet is sent to target*/
        if ((block_result = BLOCK(pCxt, ath_sock_context[index], TRANSMIT_BLOCK_TIMEOUT, TX_DIRECTION)) != A_OK)
        {
            result = block_result;
            goto END_TX;
        }
#endif
    }
END_TX:

    if (status != A_OK)
    {
        result = A_SOCK_INVALID;
    }

    return result;
}

#if ZERO_COPY
int32_t t_recvfrom(QCA_CONTEXT_STRUCT_PTR qca_ptr,
                   uint32_t handle,
                   void **buffer,
                   uint32_t length,
                   uint32_t flags,
                   void *name,
                   socklen_t *socklength)
#else
int32_t t_recvfrom(QCA_CONTEXT_STRUCT_PTR qca_ptr,
                   uint32_t handle,
                   void *buffer,
                   uint32_t length,
                   uint32_t flags,
                   void *name,
                   socklen_t *socklength)
#endif
{
    void *pCxt;

    if ((pCxt = qca_ptr->MAC_CONTEXT_PTR) == NULL)
    {
        return A_ERROR;
    }
    /*Wait for chip to be up*/
    if (A_OK != Api_DriverAccessCheck(pCxt, 1, ACCESS_REQUEST_RX))
    {
        return A_ERROR;
    }

    return (Api_recvfrom(pCxt, handle, buffer, length, flags, name, socklength));
}

#if ZERO_COPY
int32_t t_recv(QCA_CONTEXT_STRUCT_PTR qca_ptr, uint32_t handle, void **buffer, uint32_t length, uint32_t flags)
#else
int32_t t_recv(QCA_CONTEXT_STRUCT_PTR qca_ptr, uint32_t handle, void *buffer, uint32_t length, uint32_t flags)
#endif
{
    return (t_recvfrom(qca_ptr, handle, buffer, length, flags, NULL, NULL));
}

#if ZERO_COPY

/*****************************************************************************/
/*  zero_copy_free - ZERO_COPY free API.It will find and remove rx buffer from
 *                  a common queue.
 * RETURNS: Number of bytes received or A_ERROR in case of failure
 *****************************************************************************/
void zero_copy_free(void *buffer)
{
    A_NETBUF *a_netbuf_ptr = NULL;

    /*find buffer in zero copy queue*/
    a_netbuf_ptr = A_NETBUF_DEQUEUE_ADV(&zero_copy_free_queue, buffer);

    if (a_netbuf_ptr != NULL)
        A_NETBUF_FREE(a_netbuf_ptr);
    // else
    // printf("Error: buffer not found\n");
}

/*****************************************************************************/
/*  Api_recvfrom - ZERO_COPY version of receive API.It will check socket's
 *				receive quese for pending packets. If a packet is available,
 *				it will be passed to the application without a memcopy. The
 *				application must call a zero_copy_free API to free this buffer.
 * RETURNS: Number of bytes received or A_ERROR in case of failure
 *****************************************************************************/
int32_t Api_recvfrom(
    void *pCxt, uint32_t handle, void **buffer, uint32_t length, uint32_t flags, void *name, socklen_t *socklength)
{
    int32_t index;
    uint32_t len = 0, hdrlen = 0;
    A_NETBUF *a_netbuf_ptr = NULL;
    uint8_t *data = NULL;
    SOCKET_CONTEXT_PTR pcustctxt;

    /*Find context*/
    if ((index = find_socket_context(handle, true)) == SOCKET_NOT_FOUND)
    {
        return A_SOCK_INVALID;
    }

    if (ath_sock_context[index]->TCPCtrFlag == TCP_FIN)
    {
        QCADRV_PRINTF("fin in recv \r\n");
        return A_SOCK_INVALID;
    }

    /* If socket is TCP and not connected, report error */
    if (
        ((ath_sock_context[index]->domain == ATH_AF_INET) && 
         (ath_sock_context[index]->type == SOCK_STREAM_TYPE) && 
         (ath_sock_context[index]->TCPCtrFlag != TCP_CONNECTED)) || 
        ((ath_sock_context[index]->domain == ATH_AF_INET6) && 
         (ath_sock_context[index]->type == SOCK_STREAM_TYPE) && 
         (ath_sock_context[index]->TCPCtrFlag != TCP_CONNECTED))
     )
        return A_ERROR;

    pcustctxt = GET_SOCKET_CONTEXT(ath_sock_context[index]);
    /*Check if a packet is available*/
    /*Check if a packet is available*/
    if ((ath_sock_context[index]->type == SOCK_STREAM_TYPE) && (ath_sock_context[index]->remaining_bytes != 0) &&
        (ath_sock_context[index]->old_netbuf != NULL))
    {
        a_netbuf_ptr = ath_sock_context[index]->old_netbuf;
        data = (uint8_t *)A_NETBUF_DATA(a_netbuf_ptr);
    }
    else
    {
        while ((a_netbuf_ptr = A_NETBUF_DEQUEUE(&(pcustctxt->rxqueue))) == NULL)
        {
#if NON_BLOCKING_RX
            /*No packet is available, return -1*/
            return -1;
#else
            if (ath_sock_context[index]->TCPCtrFlag == TCP_FIN)
            {
                if (queue_empty(index))
                {
                    QCADRV_PRINTF("fin in recv \r\n");
                    clear_socket_context(index);
                    return A_SOCK_INVALID;
                }
            }
            /*No packet available, block*/
            if (BLOCK(pCxt, ath_sock_context[index], 0, RX_DIRECTION) != A_OK)
            {
                /*Check if Peer closed socket while we were waiting*/
                if (ath_sock_context[index]->handle == 0)
                {
                    return A_SOCK_INVALID;
                }
                if (ath_sock_context[index]->TCPCtrFlag == TCP_FIN)
                {
                    QCADRV_PRINTF("clr sock in recv \r\n");
                    clear_socket_context(index);
                    return A_SOCK_INVALID;
                }
                return A_ERROR;
            }
#endif
        }
        data = (uint8_t *)A_NETBUF_DATA(a_netbuf_ptr);
    }

    if (ath_sock_context[index]->domain == ATH_AF_INET)
    {
        hdrlen = sizeof(SOCK_RECV_T);
        /*If we are receiving data for a UDP socket, extract sockaddr info*/
        if ((ath_sock_context[index]->type == SOCK_DGRAM_TYPE) || (ath_sock_context[index]->type == SOCK_RAW_TYPE))
        {
            A_MEMCPY(name, &((SOCK_RECV_T *)data)->name, sizeof(SOCKADDR_T));
            ((SOCKADDR_T *)name)->sin_port = A_CPU2LE16(((SOCKADDR_T *)name)->sin_port);
            ((SOCKADDR_T *)name)->sin_family = A_CPU2LE16(((SOCKADDR_T *)name)->sin_family);
            ((SOCKADDR_T *)name)->sin_addr = A_CPU2LE32(((SOCKADDR_T *)name)->sin_addr);
            *socklength = sizeof(SOCKADDR_T);
        }
    }
    else if (ath_sock_context[index]->domain == ATH_AF_INET6)
    {
        hdrlen = sizeof(SOCK_RECV6_T);
        /*If we are receiving data for a UDP socket, extract sockaddr info*/
        if ((ath_sock_context[index]->type == SOCK_DGRAM_TYPE) || (ath_sock_context[index]->type == SOCK_RAW_TYPE))
        {
            A_MEMCPY(name, &((SOCK_RECV6_T *)data)->name6, sizeof(SOCKADDR_6_T));
            *socklength = sizeof(SOCKADDR_6_T);
        }
    }
    else
    {
        A_ASSERT(0);
    }

    if (!((ath_sock_context[index]->type == SOCK_STREAM_TYPE) && (ath_sock_context[index]->remaining_bytes != 0) &&
          (ath_sock_context[index]->old_netbuf != NULL)))
    {
        /*Remove custom header from payload*/
        A_NETBUF_PULL(a_netbuf_ptr, hdrlen);
    }

    len = A_NETBUF_LEN(a_netbuf_ptr);
    if (ath_sock_context[index]->type == SOCK_STREAM_TYPE)
    {
        if (len > length)
        {
            ath_sock_context[index]->remaining_bytes = (len - length);
            len = length;
        }
        else
        {
            ath_sock_context[index]->remaining_bytes = 0;
            ath_sock_context[index]->old_netbuf = NULL;
        }
    }
    else
    {
        if (len > length)
        {
            len = length; // Discard excess bytes
        }
    }

    *buffer = (uint8_t *)A_NETBUF_DATA(a_netbuf_ptr);
    if (ath_sock_context[index]->type == SOCK_STREAM_TYPE)
    {
        if (ath_sock_context[index]->remaining_bytes == 0)
        {
            A_NETBUF_ENQUEUE(&zero_copy_free_queue, a_netbuf_ptr);
        }
        else
        {
            A_NETBUF_PULL(a_netbuf_ptr, len);
            ath_sock_context[index]->old_netbuf = a_netbuf_ptr;
        }
    }
    else
    {
        A_NETBUF_ENQUEUE(&zero_copy_free_queue, a_netbuf_ptr);
    }
    return len;
}
#else

/*****************************************************************************/
/*  Api_recvfrom - Non ZERO_COPY version of receive API.It will check socket's
 *				receive quese for pending packets. If a packet is available,
 *				it will be copied into user provided buffer.
 * RETURNS: Number of bytes received or A_ERROR in case of failure
 *****************************************************************************/
int32_t Api_recvfrom(
    void *pCxt, uint32_t handle, void *buffer, uint32_t length, uint32_t flags, void *name, socklen_t *socklength)
{
    int32_t index;
    uint32_t len = 0, hdrlen = 0;
    A_NETBUF *a_netbuf_ptr = NULL;
    uint8_t *data = NULL;
    SOCKET_CONTEXT_PTR pcustctxt;

    /*Find context*/
    if ((index = find_socket_context(handle, true)) == SOCKET_NOT_FOUND)
    {
        return A_SOCK_INVALID;
    }
    pcustctxt = GET_SOCKET_CONTEXT(ath_sock_context[index]);

    while ((a_netbuf_ptr = A_NETBUF_DEQUEUE(&(pcustctxt->rxqueue))) == NULL)
    {
#if NON_BLOCKING_RX
        /*No packet is available, return -1*/
        return A_ERROR;
#else
        /*No packet available, block*/
        if (CUSTOM_BLOCK(pCxt, ath_sock_context[index], 0, RX_DIRECTION) != A_OK)
        {
            /*Check if Peer closed socket while we were waiting*/
            if (ath_sock_context[index]->handle == 0)
            {
                return A_SOCK_INVALID;
            }
            return A_ERROR;
        }
#endif
    }

    /*Extract custom header*/
    data = (uint8_t *)A_NETBUF_DATA(a_netbuf_ptr);
    if (ath_sock_context[index]->domain == ATH_AF_INET)
    {
        hdrlen = sizeof(SOCK_RECV_T);
        /*If we are receiving data for a UDP socket, extract sockaddr info*/
        if (ath_sock_context[index]->type == SOCK_DGRAM_TYPE)
        {
            A_MEMCPY(name, &((SOCK_RECV_T *)data)->name, sizeof(SOCKADDR_T));
            ((SOCKADDR_T *)name)->sin_port = A_CPU2LE16(((SOCKADDR_T *)name)->sin_port);
            ((SOCKADDR_T *)name)->sin_family = A_CPU2LE16(((SOCKADDR_T *)name)->sin_family);
            ((SOCKADDR_T *)name)->sin_addr = A_CPU2LE32(((SOCKADDR_T *)name)->sin_addr);
            *socklength = sizeof(SOCKADDR_T);
        }
    }
    else if (ath_sock_context[index]->domain == ATH_AF_INET6)
    {
        hdrlen = sizeof(SOCK_RECV6_T);
        /*If we are receiving data for a UDP socket, extract sockaddr info*/
        if (ath_sock_context[index]->type == SOCK_DGRAM_TYPE)
        {
            A_MEMCPY(name, &((SOCK_RECV6_T *)data)->name6, sizeof(SOCKADDR_6_T));
            *socklength = sizeof(SOCKADDR_6_T);
        }
    }
    else
    {
        A_ASSERT(0);
    }

    /*Remove custom header from payload*/
    A_NETBUF_PULL(a_netbuf_ptr, hdrlen);

    len = A_NETBUF_LEN(a_netbuf_ptr);

    if (len > length)
        len = length; // Discard excess bytes

    A_MEMCPY((uint8_t *)buffer, (uint8_t *)A_NETBUF_DATA(a_netbuf_ptr), len);
    A_NETBUF_FREE(a_netbuf_ptr);

    return len;
}
#endif

/*****************************************************************************/
/*  txpkt_free - function to free driver buffers that are allocated during send
 *          operations. The function is called from a_netbuf_free, after tx
 *          has successfully completed.
 *          If NON_BLOCKING_TX is defined, the user buffer is freed here
 *          as well.
 *    Note 1- called from driver thread.
 *    Note 2- should not be called from application
 * RETURNS: none
 *****************************************************************************/
void txpkt_free(void *buffPtr)
{
    A_NETBUF_PTR a_netbuf_ptr = (A_NETBUF *)buffPtr;
    DRV_BUFFER_PTR db_ptr = a_netbuf_ptr->native_orig;

#if NON_BLOCKING_TX
    SOCKET_CONTEXT_PTR pcustctxt = GET_SOCKET_CONTEXT((ATH_SOCKET_CONTEXT_PTR)db_ptr->context);
    void *bufPtr = NULL;
    /*In case of non blocking TX, driver should free the payload buffer*/
    A_MUTEX_ACQUIRE(&pcustctxt->nb_tx_mutex);
    bufPtr = A_NETBUF_DEQUEUE(&(pcustctxt->non_block_queue));
    A_MUTEX_RELEASE(&pcustctxt->nb_tx_mutex);

    if (bufPtr != NULL)
    {
        A_FREE(((uint8_t *)bufPtr - TX_PKT_OVERHEAD), MALLOC_ID_CONTEXT);
        bufPtr = NULL;
    }

    /*We are done with netbuf, free Netbuf structure here*/
    A_FREE(a_netbuf_ptr->head, MALLOC_ID_NETBUFF);
    A_FREE(a_netbuf_ptr, MALLOC_ID_NETBUFF_OBJ);
#else
    /*Tail, data pointers within netbuf may have moved during previous TX,
     reinit the netbuf so it can be used again*/
    a_netbuf_ptr = a_netbuf_reinit((void *)a_netbuf_ptr, TCP6_HEADROOM);

    /*Unblock the application thread*/
    UNBLOCK(db_ptr->context, TX_DIRECTION);
#endif
}

/*****************************************************************************/
/*  custom_alloc - API for application to allocate a TX buffer. Here we ensure that
              enough resources are available for a packet TX. All allocations
             for a TX operation are made here. This includes Headroom, DB and netbuf.
             If any allocation fails, this API returns NULL, and the host must
             wait for some time to allow memory to be freed.
      Note 1-  Allocation may fail if too many packets are queued up for Transmission,
               e.g. in the the non-blocking TX case.
      Note 2- This API should ONLY be used for TX packets.

 * RETURNS: pointer to payload buffer for success and NULL for failure
 *****************************************************************************/
void *custom_alloc(uint32_t size)
{
    uint32_t total_size = 0;
    uint8_t *buffer, *payload;

    /*Allocate TX buffer that will include- payload + headroom + Driver buffer + pointer to netbuf*/
    total_size = size + TX_PKT_OVERHEAD;

    /*Round off to 4 byte boundary*/
    if ((total_size % 4) != 0)
    {
        total_size += 4 - total_size % 4;
    }
    if ((buffer = A_MALLOC(total_size, MALLOC_ID_CONTEXT)) == NULL)
    {
        return NULL;
    }

    /*Allocate netbuf with max possible headroom here so that there is no need to do it during TX*/
    if ((((TX_PACKET_PTR)buffer)->a_netbuf_ptr = A_NETBUF_ALLOC(TCP6_HEADROOM)) == NULL)
    {
        A_FREE(buffer, MALLOC_ID_CONTEXT);
        return NULL;
    }
    /*Obtain pointer to start of payload*/
    payload = buffer + TX_PKT_OVERHEAD;
    return payload;
}

/*****************************************************************************/
/*  custom_free - API for application to free TX buffer. It should ONLY be called
 *              by the app if Blocking TX mode is enabled. It will free all allocations
 *              made during the custom_alloc
 * RETURNS: none
 *****************************************************************************/
void custom_free(void *buf)
{
#if (!NON_BLOCKING_TX)
    uint8_t *buffer;
    A_NETBUF_PTR a_netbuf_ptr;

    /*Move the the begining of TX buffer*/
    buffer = (uint8_t *)buf - TX_PKT_OVERHEAD;

    /*Get pointer to netbuf from TX buffer*/
    a_netbuf_ptr = ((TX_PACKET_PTR)buffer)->a_netbuf_ptr;

    if (a_netbuf_ptr)
    {
        /*We are done with netbuf, free Netbuf structure here*/
        A_FREE(a_netbuf_ptr->head, MALLOC_ID_NETBUFF);
        A_FREE(a_netbuf_ptr, MALLOC_ID_NETBUFF_OBJ);
    }
    A_FREE(buffer, MALLOC_ID_CONTEXT);
#endif
}

/*****************************************************************************/
/*  get_total_pkts_buffered - Returns number of packets buffered across all
 *          socket queues.
 * RETURNS: number of packets
 *****************************************************************************/
uint32_t get_total_pkts_buffered(void)
{
    uint32_t index;
    uint32_t totalPkts = 0;

    SOCKET_CONTEXT_PTR pcustctxt;

    for (index = 0; index < MAX_SOCKETS_SUPPORTED; index++)
    {
        pcustctxt = GET_SOCKET_CONTEXT(ath_sock_context[index]);
        totalPkts += A_NETBUF_QUEUE_SIZE(&(pcustctxt->rxqueue));
    }
    return totalPkts;
}

#if ENABLE_SSL
/*****************************************************************************/
/* SSL_ctx_new - Create new SSL context. This function must be called before
 *               using any other SSL functions. It needs to be called as either
 *               server or client
 * Sslrole role - 1 = server, 2 = client
 * int32_t inbufSize - initial inBuf size: Can grow
 * int32_t outbufSize - outBuf size: Fixed
 * int32_t reserved - currently not used (must be zero)
 * Returns - SSL context handle on success or NULL on error (out of memory)
 *****************************************************************************/
SSL_CTX *SSL_ctx_new(SSL_ROLE_T role, int32_t inbufSize, int32_t outbufSize, int32_t reserved)
{
    void *pCxt;

    if ((pCxt = ((QCA_CONTEXT_STRUCT_PTR)sslhandle)->MAC_CONTEXT_PTR) == NULL)
    {
        return NULL;
    }

    /*Wait for chip to be up*/
    if (A_OK != Api_DriverAccessCheck(pCxt, 1, ACCESS_REQUEST_IOCTL))
    {
        return NULL;
    }
    return ((SSL_CTX *)Api_SSL_ctx_new(pCxt, role, inbufSize, outbufSize, reserved));
}

/*****************************************************************************/
/* SSL_ctx_free - Free the SSL context
 * SSL_CTX *ctx - sslContext
 *****************************************************************************/
int32_t SSL_ctx_free(SSL_CTX *ctx)
{
    void *pCxt;

    if ((pCxt = ((QCA_CONTEXT_STRUCT_PTR)sslhandle)->MAC_CONTEXT_PTR) == NULL)
    {
        return A_ERROR;
    }

    /*Wait for chip to be up*/
    if (A_OK != Api_DriverAccessCheck(pCxt, 1, ACCESS_REQUEST_IOCTL))
    {
        return A_ERROR;
    }
    return (Api_SSL_ctx_free(pCxt, ctx));
}

/*****************************************************************************/
/* SSL_new - Create SSL connection object. When SSL transaction is done, close
 *           it with ssl_shutdown().
 *           Note that the socket should subsequently also be closed.
 * SSL_CTX *ctx - sslContext
 * Return - SSL object handle on success or NULL on error (out of memory)
 *****************************************************************************/
SSL *SSL_new(SSL_CTX *ctx)
{
    void *pCxt;

    if ((pCxt = ((QCA_CONTEXT_STRUCT_PTR)sslhandle)->MAC_CONTEXT_PTR) == NULL)
    {
        return NULL;
    }

    /*Wait for chip to be up*/
    if (A_OK != Api_DriverAccessCheck(pCxt, 1, ACCESS_REQUEST_IOCTL))
    {
        return NULL;
    }
    return (Api_SSL_new(pCxt, ctx));
}

/*****************************************************************************/
/* SSL_set_fd - Attach given socket descriptor to the SSL connection
 * SSL *ssl - SSL connection
 * uint32_t fd - Socket descriptor
 * Return - 1 on success or negative error code on error (see SslErrors)
 *****************************************************************************/
int32_t SSL_set_fd(SSL *ssl, uint32_t fd)
{
    void *pCxt;

    if ((pCxt = ((QCA_CONTEXT_STRUCT_PTR)sslhandle)->MAC_CONTEXT_PTR) == NULL)
    {
        return A_ERROR;
    }

    /*Wait for chip to be up*/
    if (A_OK != Api_DriverAccessCheck(pCxt, 1, ACCESS_REQUEST_IOCTL))
    {
        return A_ERROR;
    }
    return (Api_SSL_set_fd(pCxt, ssl, fd));
}

/*****************************************************************************/
/* SSL_accept - Initiate SSL handshake.
 * SSL *ssl - SSL connection
 * Returns - 1 on success, ESSL_HSDONE if handshake has already been performed.
 *           Negative error code otherwise.
 *****************************************************************************/
int32_t SSL_accept(SSL *ssl)
{
    void *pCxt;

    if ((pCxt = ((QCA_CONTEXT_STRUCT_PTR)sslhandle)->MAC_CONTEXT_PTR) == NULL)
    {
        return A_ERROR;
    }

    /*Wait for chip to be up*/
    if (A_OK != Api_DriverAccessCheck(pCxt, 1, ACCESS_REQUEST_IOCTL))
    {
        return A_ERROR;
    }
    return (Api_SSL_accept(pCxt, ssl));
}

/*****************************************************************************/
/* SSL_connect - Initiate SSL handshake.
 * SSL *ssl - SSL connection
 * Returns - 1 on success, ESSL_HSDONE if handshake has already been performed.
 *           Negative error code otherwise.
 *****************************************************************************/
int32_t SSL_connect(SSL *ssl)
{
    void *pCxt;

    if ((pCxt = ((QCA_CONTEXT_STRUCT_PTR)sslhandle)->MAC_CONTEXT_PTR) == NULL)
    {
        return A_ERROR;
    }

    /*Wait for chip to be up*/
    if (A_OK != Api_DriverAccessCheck(pCxt, 1, ACCESS_REQUEST_IOCTL))
    {
        return A_ERROR;
    }
    return (Api_SSL_connect(pCxt, ssl));
}

/*****************************************************************************/
/* SSL_shutdown - Close SSL connection.
 *                The socket must be closed by other means.
 * SSL *ssl - SSL connection
 * Returns - 1 on success or negative error code on error (see SslErrors)
 *****************************************************************************/
int32_t SSL_shutdown(SSL *ssl)
{
    void *pCxt;

    if ((pCxt = ((QCA_CONTEXT_STRUCT_PTR)sslhandle)->MAC_CONTEXT_PTR) == NULL)
    {
        return A_ERROR;
    }

    /*Wait for chip to be up*/
    if (A_OK != Api_DriverAccessCheck(pCxt, 1, ACCESS_REQUEST_IOCTL))
    {
        return A_ERROR;
    }
    return (Api_SSL_shutdown(pCxt, ssl));
}

/*****************************************************************************/
/* SSL_configure - Configure a SSL connection.
 * SSL *ssl - SSL connection
 * SSL_CONFIG *cfg - pointer to struct holding the SSL configuration.
 * Returns - 1 on success or negative error code on error (see SslErrors)
 *****************************************************************************/
int32_t SSL_configure(SSL *ssl, SSL_CONFIG *cfg)
{
    void *pCxt;

    if ((pCxt = ((QCA_CONTEXT_STRUCT_PTR)sslhandle)->MAC_CONTEXT_PTR) == NULL)
    {
        return A_ERROR;
    }

    /*Wait for chip to be up*/
    if (A_OK != Api_DriverAccessCheck(pCxt, 1, ACCESS_REQUEST_IOCTL))
    {
        return A_ERROR;
    }
    return (Api_SSL_configure(pCxt, ssl, cfg));
}

/*****************************************************************************/
/* SSL_setCaList - Set a Certificate Authority (CA) list so SSL can perform
 *                 certificate validation on the peer's certificate.
 *                 You can only set one CA list, thus the CA list must include
 *                 all root certificates required for the session.
 * SSL *ssl - SSL connection
 * SslCert cert -address of array of binary data
 * uint32_t size - size of array
 * Returns - 1 on success or negative error code on error (see SslErrors)
 *****************************************************************************/
int32_t SSL_setCaList(SSL_CTX *ctx, SslCAList caList, uint32_t size)
{
    void *pCxt;

    if ((pCxt = ((QCA_CONTEXT_STRUCT_PTR)sslhandle)->MAC_CONTEXT_PTR) == NULL)
    {
        return A_ERROR;
    }

    /*Wait for chip to be up*/
    if (A_OK != Api_DriverAccessCheck(pCxt, 1, ACCESS_REQUEST_IOCTL))
    {
        return A_ERROR;
    }
    return (Api_SSL_addCert(pCxt, ctx, SSL_CA_LIST, (uint8_t *)caList, size));
}

/*****************************************************************************/
/* SSL_addCert - Add a certificate to the SharkSsl object. A SharkSsl object
 *               in server mode is required to have at least one certificate.
 * SSL *ssl - SSL connection
 * SslCert cert -address of array of binary data
 * uint32_t size - size of array
 * Returns - 1 on success or negative error code on error (see SslErrors)
 *****************************************************************************/
int32_t SSL_addCert(SSL_CTX *ctx, SslCert cert, uint32_t size)
{
    void *pCxt;

    if ((pCxt = ((QCA_CONTEXT_STRUCT_PTR)sslhandle)->MAC_CONTEXT_PTR) == NULL)
    {
        return A_ERROR;
    }

    /*Wait for chip to be up*/
    if (A_OK != Api_DriverAccessCheck(pCxt, 1, ACCESS_REQUEST_IOCTL))
    {
        return A_ERROR;
    }
    return (Api_SSL_addCert(pCxt, ctx, SSL_CERTIFICATE, (uint8_t *)cert, size));
}

/*****************************************************************************/
/* SSL_storeCert - Store a certificate or CA list in FLASH.
 * char *name - the name
 * SslCert cert -address of array of binary data
 * uint32_t size - size of array
 * Returns - 1 on success or negative error code on error (see SslErrors)
 *****************************************************************************/
int32_t SSL_storeCert(char *name, SslCert cert, uint32_t size)
{
    void *pCxt;

    if ((pCxt = ((QCA_CONTEXT_STRUCT_PTR)sslhandle)->MAC_CONTEXT_PTR) == NULL)
    {
        return A_ERROR;
    }

    /*Wait for chip to be up*/
    if (A_OK != Api_DriverAccessCheck(pCxt, 1, ACCESS_REQUEST_IOCTL))
    {
        return A_ERROR;
    }
    return (Api_SSL_storeCert(pCxt, name, (uint8_t *)cert, size));
}

/*****************************************************************************/
/* SSL_loadCert - Load a certificate or CA list from FLASH and store it in the
 *                sharkSs object.
 * SSL_CERT_TYPE_T type - certificate or CA list
 * char *name - the name
 * Returns - 1 on success or negative error code on error (see SslErrors)
 *****************************************************************************/
int32_t SSL_loadCert(SSL_CTX *ctx, SSL_CERT_TYPE_T type, char *name)
{
    void *pCxt;

    if ((pCxt = ((QCA_CONTEXT_STRUCT_PTR)sslhandle)->MAC_CONTEXT_PTR) == NULL)
    {
        return A_ERROR;
    }

    /*Wait for chip to be up*/
    if (A_OK != Api_DriverAccessCheck(pCxt, 1, ACCESS_REQUEST_IOCTL))
    {
        return A_ERROR;
    }
    return (Api_SSL_loadCert(pCxt, ctx, type, name));
}

int32_t SSL_listCert(SSL_FILE_NAME_LIST *fileNames)
{
    void *pCxt;

    if ((pCxt = ((QCA_CONTEXT_STRUCT_PTR)sslhandle)->MAC_CONTEXT_PTR) == NULL)
    {
        return A_ERROR;
    }

    /*Wait for chip to be up*/
    if (A_OK != Api_DriverAccessCheck(pCxt, 1, ACCESS_REQUEST_IOCTL))
    {
        return A_ERROR;
    }
    return (Api_SSL_listCert(pCxt, fileNames));
}

#if ZERO_COPY
/*****************************************************************************/
/*  SSL_read - ZERO_COPY version of SSL read function. It uses the ssl pointer
 *             to find the socket to read from.It will check socket's receive
 *             queues for pending packets.  If a packet is available, it will be
 *             passed to the application without a memcopy. The application must
 *             call a zero_copy_free API to free this buffer.
 * SSL *ssl - pointer to SSL connection object.
 * void **buf - pointer to pointer holding the address of the receive buffer.
 * int32_t num - The max number of bytes to read.
 * Returns - Number of bytes received or A_ERROR in case of failure
 *****************************************************************************/
int32_t SSL_read(SSL *ssl, void **buf, int32_t num)
#else
int32_t SSL_read(SSL *ssl, void *buf, int32_t num)
#endif
{
    /* Find the socket used with this SSL connection */
    int32_t index = find_socket_context_from_ssl(ssl);
    if (index == SOCKET_NOT_FOUND)
    {
        return A_ERROR;
    }

    /* Read data from the socket */
    return t_recv(sslhandle, ath_sock_context[index]->handle, buf, num, 0);
}

/*****************************************************************************/
/* SSL_write - Encrypt and send data in buf and send them on the associated
 *             socket. The ssl pointer is sued to find the socket to send on.
 * SSL *ssl - pointer to SSL connection object.
 * void *buf - pointer to buffer holding the data to send.
 * int32_t num  - The number of bytes to send.
 * Returns - Number of bytes sent on success or negative error code on error
 *****************************************************************************/
int32_t SSL_write(SSL *ssl, const void *buf, int32_t num)
{
    /* Find the socket used with this SSL connection */
    int32_t index = find_socket_context_from_ssl(ssl);
    if (index == SOCKET_NOT_FOUND)
    {
        return A_ERROR;
    }

    /* Send the data */
    return t_send(sslhandle, ath_sock_context[index]->handle, (uint8_t *)buf, num, 0);
}

#endif // ENABLE_SSL
#endif
