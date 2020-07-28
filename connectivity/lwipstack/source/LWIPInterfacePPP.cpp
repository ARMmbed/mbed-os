/*
 * Copyright (c) 2019 ARM Limited
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

#include "lwip/tcpip.h"
#include "lwip/tcp.h"
#include "lwip/ip.h"
#include "lwip/ip_addr.h"
#include "lwip/dns.h"
#include "netif/etharp.h"
#include "lwip/ethip6.h"
#include "netsocket/nsapi_types.h"
#include "netsocket/PPP.h"
#include "LWIPStack.h"
#include "lwip_tools.h"

#if PPP_SUPPORT

#if PPP_IPV4_SUPPORT && LWIP_IPV4
err_t LWIP::Interface::ppp4_output(struct netif *netif, struct pbuf *p, const ip4_addr_t *ipaddr)
{
    /* Increase reference counter since lwip stores handle to pbuf and frees
       it after output */
    pbuf_ref(p);

    LWIP::Interface *mbed_if = static_cast<LWIP::Interface *>(netif->state);
    bool ret = mbed_if->ppp->link_out(p, IPV4_STACK);
    return ret ? ERR_OK : ERR_IF;
}
#endif
#if PPP_IPV6_SUPPORT && LWIP_IPV6
err_t LWIP::Interface::ppp6_output(struct netif *netif, struct pbuf *p, const ip6_addr_t *ipaddr)
{
    /* Increase reference counter since lwip stores handle to pbuf and frees
       it after output */
    pbuf_ref(p);

    LWIP::Interface *mbed_if = static_cast<LWIP::Interface *>(netif->state);
    bool ret = mbed_if->ppp->link_out(p, IPV6_STACK);
    return ret ? ERR_OK : ERR_IF;
}
#endif
void LWIP::Interface::ppp_input(net_stack_mem_buf_t *buf)
{
    struct pbuf *p = static_cast<struct pbuf *>(buf);

    /* pass all packets to IP stack input */
    if (netif.input(p, &netif) != ERR_OK) {
        LWIP_DEBUGF(NETIF_DEBUG, ("Emac LWIP: IP input error\n"));

        pbuf_free(p);
    }
}

void LWIP::Interface::ppp_state_change(bool up)
{
    if (up) {
#if PPP_IPV6_SUPPORT && LWIP_IPV6
        const nsapi_addr_t *ipv6_addr = LWIP::Interface::ppp->get_ip_address(NSAPI_IPv6);

        ip_addr_t ip_addr;
        if (ipv6_addr && convert_mbed_addr_to_lwip(&ip_addr, ipv6_addr)) {
            netif_ip6_addr_set(&netif, 0, ip_2_ip6(&ip_addr));
            netif_ip6_addr_set_state(&netif, 0, IP6_ADDR_PREFERRED);
        }
#endif

#if PPP_IPV4_SUPPORT && LWIP_IPV4
        const nsapi_addr_t *ipv4_addr = LWIP::Interface::ppp->get_ip_address(NSAPI_IPv4);
        if (ipv4_addr) {
            ip_addr_t ip_addr;
            ip_addr_t netmask;
            ip_addr_t gateway;

            int conv_ip = 0;
            if (convert_mbed_addr_to_lwip(&ip_addr, ipv4_addr)) {
                conv_ip++;
            }

            const nsapi_addr_t *ipv4_netmask = LWIP::Interface::ppp->get_netmask();
            if (ipv4_netmask && convert_mbed_addr_to_lwip(&netmask, ipv4_netmask)) {
                conv_ip++;
            }

            const nsapi_addr_t *ipv4_gateway = LWIP::Interface::ppp->get_gateway();
            if (ipv4_gateway && convert_mbed_addr_to_lwip(&gateway, ipv4_gateway)) {
                conv_ip++;
            }

            if (conv_ip == 3) {
                netif_set_addr(&netif, ip_2_ip4(&ip_addr), ip_2_ip4(&netmask), ip_2_ip4(&gateway));
            }

            unsigned char dns_index = 0;
            // If default interface set default DNS addresses, otherwise interface specific
            struct netif *dns_netif = &netif;
            if (netif_check_default(&netif)) {
                dns_netif = NULL;
            }
            for (unsigned char index = 0; index < 2; index++) {
                ip_addr_t dns_server;
                const nsapi_addr_t *ipv4_dns_server = LWIP::Interface::ppp->get_dns_server(index);
                if (ipv4_dns_server && convert_mbed_addr_to_lwip(&dns_server, ipv4_dns_server)) {
                    dns_setserver(dns_index++, &dns_server, dns_netif);
                }
            }
        }
#endif
        // Read negotiated MTU
        uint32_t mtu = LWIP::Interface::ppp->get_mtu_size();
        netif.mtu = mtu;
#if PPP_IPV6_SUPPORT && LWIP_IPV6 && LWIP_ND6_ALLOW_RA_UPDATES
        netif.mtu6 = mtu;
#endif
        tcpip_callback_with_block((tcpip_callback_fn)netif_set_link_up, &netif, 1);
    } else {
        tcpip_callback_with_block((tcpip_callback_fn)netif_set_link_down, &netif, 1);
    }
}

err_t LWIP::Interface::ppp_if_init(struct netif *netif)
{
    int err = ERR_OK;
    LWIP::Interface *mbed_if = static_cast<LWIP::Interface *>(netif->state);

    mbed_if->ppp->set_memory_manager(*mbed_if->memory_manager);
    mbed_if->ppp->set_link_input_cb(mbed::callback(mbed_if, &LWIP::Interface::ppp_input));
    mbed_if->ppp->set_link_state_cb(mbed::callback(mbed_if, &LWIP::Interface::ppp_state_change));

    /* Interface capabilities */
    netif->flags = NETIF_FLAG_BROADCAST;

    if (!mbed_if->ppp->power_up()) {
        err = ERR_IF;
    }

    netif->mtu = mbed_if->ppp->get_mtu_size();
    mbed_if->ppp->get_ifname(netif->name, NSAPI_INTERFACE_PREFIX_SIZE);

#if PPP_IPV4_SUPPORT && LWIP_IPV4
    netif->output = &LWIP::Interface::ppp4_output;
#endif /* PPP_IPV4_SUPPORT */
#if PPP_IPV6_SUPPORT && LWIP_IPV6
    netif->output_ip6 = &LWIP::Interface::ppp6_output;
#endif
    netif->linkoutput = NULL;

    return err;
}

#endif


