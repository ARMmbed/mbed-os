/* mbed Microcontroller Library
 * Copyright (c) 2016 ARM Limited
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
#include "netsocket/EMAC.h"

#include "LWIPStack.h"

#if MBED_EMAC_LWIP_L2_BRIDGE
#include "lwip_l2_bridge.h"
#endif

#if LWIP_ETHERNET

err_t LWIP::Interface::emac_low_level_output(struct netif *netif, struct pbuf *p)
{
    /* Increase reference counter since lwip stores handle to pbuf and frees
       it after output */
    pbuf_ref(p);

#if MBED_EMAC_LWIP_L2_BRIDGE
    return emac_lwip_l2b_output(netif, (emac_mem_buf_t*) p);
#else

    LWIP::Interface *mbed_if = static_cast<LWIP::Interface *>(netif->state);
    bool ret = mbed_if->emac->link_out(p);
    return ret ? ERR_OK : ERR_IF;
#endif
}

void LWIP::Interface::emac_input(emac_mem_buf_t *buf)
{
#if MBED_EMAC_LWIP_L2_BRIDGE
    emac_lwip_l2b_input(&netif, buf);
#else
    struct pbuf *p = static_cast<struct pbuf *>(buf);

    /* pass all packets to ethernet_input, which decides what packets it supports */
    if (netif.input(p, &netif) != ERR_OK) {
        LWIP_DEBUGF(NETIF_DEBUG, ("Emac LWIP: IP input error\n"));

        pbuf_free(p);
    }
#endif
}

void LWIP::Interface::emac_state_change(bool up)
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
err_t LWIP::Interface::emac_igmp_mac_filter(struct netif *netif, const ip4_addr_t *group, enum netif_mac_filter_action action)
{
    LWIP::Interface *mbed_if = static_cast<LWIP::Interface *>(netif->state);

    switch (action) {
        case NETIF_ADD_MAC_FILTER: {
            uint32_t group23 = ntohl(group->addr) & 0x007FFFFF;
            uint8_t addr[6];
            addr[0] = LL_IP4_MULTICAST_ADDR_0;
            addr[1] = LL_IP4_MULTICAST_ADDR_1;
            addr[2] = LL_IP4_MULTICAST_ADDR_2;
            addr[3] = group23 >> 16;
            addr[4] = group23 >> 8;
            addr[5] = group23;
            mbed_if->emac->add_multicast_group(addr);
            return ERR_OK;
        }
        case NETIF_DEL_MAC_FILTER:
            /* As we don't reference count, silently ignore delete requests */
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
err_t LWIP::Interface::emac_mld_mac_filter(struct netif *netif, const ip6_addr_t *group, enum netif_mac_filter_action action)
{
    LWIP::Interface *mbed_if = static_cast<LWIP::Interface *>(netif->state);

    switch (action) {
        case NETIF_ADD_MAC_FILTER: {
            uint32_t group32 = ntohl(group->addr[3]);
            uint8_t addr[6];
            addr[0] = LL_IP6_MULTICAST_ADDR_0;
            addr[1] = LL_IP6_MULTICAST_ADDR_1;
            addr[2] = group32 >> 24;
            addr[3] = group32 >> 16;
            addr[4] = group32 >> 8;
            addr[5] = group32;
            mbed_if->emac->add_multicast_group(addr);
            return ERR_OK;
        }
        case NETIF_DEL_MAC_FILTER:
            /* As we don't reference count, silently ignore delete requests */
            return ERR_OK;
        default:
            return ERR_ARG;
    }
}
#endif

err_t LWIP::Interface::emac_if_init(struct netif *netif)
{
    int err = ERR_OK;
    LWIP::Interface *mbed_if = static_cast<LWIP::Interface *>(netif->state);

    mbed_if->emac->set_memory_manager(*mbed_if->memory_manager);
    mbed_if->emac->set_link_input_cb(mbed::callback(mbed_if, &LWIP::Interface::emac_input));
    mbed_if->emac->set_link_state_cb(mbed::callback(mbed_if, &LWIP::Interface::emac_state_change));

    /* Interface capabilities */
    netif->flags = NETIF_FLAG_BROADCAST | NETIF_FLAG_ETHARP | NETIF_FLAG_ETHERNET;

    if (!mbed_if->emac->power_up()) {
        err = ERR_IF;
    }

    netif->mtu = mbed_if->emac->get_mtu_size();
    /* We have a default MAC address, so do don't force them to supply one */
    netif->hwaddr_len = mbed_if->emac->get_hwaddr_size();
    /* They may or may not update hwaddr with their address */
    mbed_if->emac->get_hwaddr(netif->hwaddr);
    /* Then we write back either what they gave us, or our default */
    mbed_if->emac->set_hwaddr(netif->hwaddr);

    mbed_if->emac->get_ifname(netif->name, 2);

#if LWIP_IPV4
    netif->output = etharp_output;
#if LWIP_IGMP
    netif->igmp_mac_filter = &LWIP::Interface::emac_igmp_mac_filter;
    netif->flags |= NETIF_FLAG_IGMP;
#endif /* LWIP_IGMP */
#endif /* LWIP_IPV4 */
#if LWIP_IPV6
    netif->output_ip6 = ethip6_output;
#if LWIP_IPV6_MLD
    netif->mld_mac_filter = &LWIP::Interface::emac_mld_mac_filter;
    netif->flags |= NETIF_FLAG_MLD6;
#else
// Would need to enable all multicasts here - no API in fsl_enet to do that
#error "IPv6 multicasts won't be received if LWIP_IPV6_MLD is disabled, breaking the system"
#endif
#endif

    netif->linkoutput = &LWIP::Interface::emac_low_level_output;

#if MBED_EMAC_LWIP_L2_BRIDGE
    if(err == ERR_OK) {
        emac_lwip_l2b_register_interface(netif);
    }
#endif

    return err;
}

#endif

