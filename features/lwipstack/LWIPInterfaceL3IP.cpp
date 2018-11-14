/*
 * Copyright (c) 2018 ARM Limited
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
#include "netif/etharp.h"
#include "lwip/ethip6.h"
#include "netsocket/nsapi_types.h"
#include "netsocket/L3IP.h"

#include "LWIPStack.h"

#if LWIP_L3IP
#if LWIP_IPV4
err_t LWIP::Interface::l3ip4_output(struct netif *netif, struct pbuf *p, const ip4_addr_t *ipaddr)
{
    /* Increase reference counter since lwip stores handle to pbuf and frees
       it after output */
    pbuf_ref(p);

    LWIP::Interface *mbed_if = static_cast<LWIP::Interface *>(netif->state);
    bool ret = mbed_if->l3ip->link_out(p);
    return ret ? ERR_OK : ERR_IF;
}
#endif
#if LWIP_IPV6
err_t LWIP::Interface::l3ip6_output(struct netif *netif, struct pbuf *p, const ip6_addr_t *ipaddr)
{
    /* Increase reference counter since lwip stores handle to pbuf and frees
       it after output */
    pbuf_ref(p);

    LWIP::Interface *mbed_if = static_cast<LWIP::Interface *>(netif->state);
    bool ret = mbed_if->l3ip->link_out(p);
    return ret ? ERR_OK : ERR_IF;
}
#endif
void LWIP::Interface::l3ip_input(net_stack_mem_buf_t *buf)
{
    struct pbuf *p = static_cast<struct pbuf *>(buf);

    /* pass all packets to IP stack input */
    if (netif.input(p, &netif) != ERR_OK) {
        LWIP_DEBUGF(NETIF_DEBUG, ("Emac LWIP: IP input error\n"));

        pbuf_free(p);
    }
}

void LWIP::Interface::l3ip_state_change(bool up)
{
    if (up) {
        tcpip_callback_with_block((tcpip_callback_fn)netif_set_link_up, &netif, 1);
    } else {
        tcpip_callback_with_block((tcpip_callback_fn)netif_set_link_down, &netif, 1);
    }
}

#if LWIP_IGMP

#include "lwip/igmp.h"
/**
 * IPv4 address filtering setup.
 *
 * \param[in] netif the lwip network interface structure
 * \param[in] group IPv4 group to modify
 * \param[in] action
 * \return ERR_OK or error code
 */
err_t LWIP::Interface::l3ip_multicast_ipv4_filter(struct netif *netif, const ip4_addr_t *group, enum netif_mac_filter_action action)
{
    LWIP::Interface *mbed_if = static_cast<LWIP::Interface *>(netif->state);
    SocketAddress addr(&group, NSAPI_IPv6);
    switch (action) {
        case NETIF_ADD_MAC_FILTER: {
            mbed_if->l3ip->add_ipv4_multicast_group(addr.get_ip_address());
            return ERR_OK;
        }
        case NETIF_DEL_MAC_FILTER:
            mbed_if->l3ip->remove_ipv4_multicast_group(addr.get_ip_address());
            return ERR_OK;
        default:
            return ERR_ARG;
    }

}
#endif

#if LWIP_IPV6_MLD

#include "lwip/mld6.h"
/**
 * IPv6 address filtering setup.
 *
 * \param[in] netif the lwip network interface structure
 * \param[in] group IPv6 group to modify
 * \param[in] action
 * \return ERR_OK or error code
 */
err_t LWIP::Interface::l3ip_multicast_ipv6_filter(struct netif *netif, const ip6_addr_t *group, enum netif_mac_filter_action action)
{
    LWIP::Interface *mbed_if = static_cast<LWIP::Interface *>(netif->state);
    SocketAddress addr(&group, NSAPI_IPv6);
    switch (action) {
        case NETIF_ADD_MAC_FILTER: {

            mbed_if->l3ip->add_ipv6_multicast_group(addr.get_ip_address());
            return ERR_OK;
        }
        case NETIF_DEL_MAC_FILTER:
            mbed_if->l3ip->remove_ipv6_multicast_group(addr.get_ip_address());
            return ERR_OK;
        default:
            return ERR_ARG;
    }

}
#endif

err_t LWIP::Interface::l3ip_if_init(struct netif *netif)
{
    int err = ERR_OK;
    LWIP::Interface *mbed_if = static_cast<LWIP::Interface *>(netif->state);

    mbed_if->l3ip->set_memory_manager(*mbed_if->memory_manager);
    mbed_if->l3ip->set_link_input_cb(mbed::callback(mbed_if, &LWIP::Interface::l3ip_input));
    mbed_if->l3ip->set_link_state_cb(mbed::callback(mbed_if, &LWIP::Interface::l3ip_state_change));

    /* Interface capabilities */
    netif->flags = NETIF_FLAG_BROADCAST;

    if (!mbed_if->l3ip->power_up()) {
        err = ERR_IF;
    }

    netif->mtu = mbed_if->l3ip->get_mtu_size();
    mbed_if->l3ip->get_ifname(netif->name, NSAPI_INTERFACE_PREFIX_SIZE);

#if LWIP_IPV4
    netif->output = &LWIP::Interface::l3ip4_output;
#if LWIP_IGMP
    netif->igmp_mac_filter = &LWIP::Interface::l3ip_multicast_ipv4_filter;
    netif->flags |= NETIF_FLAG_IGMP;
#endif /* LWIP_IGMP */
#endif /* LWIP_IPV4 */
#if LWIP_IPV6
    netif->output_ip6 = &LWIP::Interface::l3ip6_output;
#if LWIP_IPV6_MLD
    netif->mld_mac_filter = &LWIP::Interface::l3ip_multicast_ipv6_filter;
    netif->flags |= NETIF_FLAG_MLD6;
#else
// Would need to enable all multicasts here - no API in fsl_enet to do that
#error "IPv6 multicasts won't be received if LWIP_IPV6_MLD is disabled, breaking the system"
#endif
#endif

    netif->linkoutput = NULL;

    return err;
}

#endif

