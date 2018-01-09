/* mbed Microcontroller Library
 * Copyright (c) 2017 ARM Limited
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
#include "mbed_assert.h"
#include "mbed_wait_api.h"
#include "nsapi.h"
#include "lwipopts.h"
#include <string.h>

#include "wiced_wifi.h"
#include "wiced_management.h"
#include "wwd_network_constants.h"
#include "wwd_buffer_interface.h"

#include "lwip/etharp.h"
#include "lwip/ethip6.h"


/* globally stored netif for for wiced
 */
static struct netif *host_network_netif = 0;

/* called by lwip layer, nops
 */
void eth_arch_enable_interrupts(void) {}
void eth_arch_disable_interrupts(void) {}

/* notify link is up or down
 */
wiced_result_t wiced_network_notify_link_up(void)
{
    if (host_network_netif) {
        netif_set_link_up(host_network_netif);
    }
    return WICED_SUCCESS;
}

wiced_result_t wiced_network_notify_link_down(void)
{
    if (host_network_netif) {
        netif_set_link_down(host_network_netif);
    }
    return WICED_SUCCESS;
}

/*
 * Called by WICED to pass received data to the network stack
 *
 * Implemented in 'network adapter driver' which is specific to the
 * network stack in use.
 * Packets received from the Wi-Fi network by WICED are forwarded to this function which
 * must be implemented in the network interface. Ethernet headers
 * are present at the start of these packet buffers.
 *
 * This function is called asynchronously in the context of the
 * WICED thread whenever new data has arrived.
 * Packet buffers are allocated within WICED, and ownership is transferred
 * to the network stack. The network stack or application is thus
 * responsible for releasing the packet buffers.
 * Most packet buffering systems have a pointer to the 'current point' within
 * the packet buffer. When this function is called, the pointer points
 * to the start of the Ethernet header. There is other inconsequential data
 * before the Ethernet header.
 *
 * It is preferable that the @ref host_network_process_ethernet_data function simply puts
 * the received packet on a queue for processing by another thread. This avoids the
 * WICED thread being unnecessarily tied up which would delay other packets
 * being transmitted or received.
 *
 * @param buffer : Handle of the packet which has just been received. Responsibility for
 *                 releasing this buffer is transferred from WICED at this point.
 * @param interface : The interface (AP or STA) on which the packet was received.
 *
 */
void host_network_process_ethernet_data(wiced_buffer_t buffer, wwd_interface_t interface)
{
    MBED_ASSERT(interface == WWD_STA_INTERFACE);

    if (!host_network_netif) {
        // ignore any trailing packets
        host_buffer_release(buffer, WWD_NETWORK_RX);
        return;
    }

    void *data = host_buffer_get_current_piece_data_pointer(buffer);
    uint16_t size = host_buffer_get_current_piece_size(buffer);

    struct pbuf *p = pbuf_alloc(PBUF_RAW, size, PBUF_RAM);
    if (!p) {
        // ignore oom, may be caused by excessive packets from outside source
        return;
    }

    memcpy(p->payload, data, size);
    host_buffer_release(buffer, WWD_NETWORK_RX);

    err_t err = host_network_netif->input(p, host_network_netif);
    MBED_ASSERT(err == ERR_OK);
}

/*
 * Called by the Network Stack to send an ethernet frame
 *
 * Implemented in 'network adapter driver' which is specific to the
 * network stack in use.
 * This function takes Ethernet data from the network stack and queues it for transmission over the wireless network.
 * The function can be called from any thread context as it is thread safe, however
 * it must not be called from interrupt context since it can block while waiting
 * for a lock on the transmit queue.
 *
 * This function returns immediately after the packet has been queued for transmit,
 * NOT after it has been transmitted.  Packet buffers passed to the WICED core
 * are released inside the WICED core once they have been transmitted.
 *
 * Some network stacks assume the driver send function blocks until the packet has been physically sent. This
 * type of stack typically releases the packet buffer immediately after the driver send function returns.
 * In this case, and assuming the buffering system can count references to packet buffers, the driver send function
 * can take an additional reference to the packet buffer. This enables the network stack and the WICED core driver
 * to independently release their own packet buffer references.
 *
 * @param buffer : Handle of the packet buffer to be sent.
 * @param interface : the interface over which to send the packet (AP or STA)
 *
 */
extern void wwd_network_send_ethernet_data(wiced_buffer_t buffer, wwd_interface_t interface); /* Returns immediately - Wiced_buffer_tx_completed will be called once the transmission has finished */

/* Called by LWIP to send an ethernet frame
 *
 * Must call wwd_network_send_ethernet_data, see above
 */
err_t host_network_send_ethernet_data(struct netif *netif, struct pbuf *p)
{
    void *data = p->payload;
    uint16_t size = p->tot_len;

    wiced_buffer_t buffer;
    wwd_result_t res = host_buffer_get(&buffer, WWD_NETWORK_TX, size+64, WICED_TRUE);
    MBED_ASSERT(res == WWD_SUCCESS);

    host_buffer_add_remove_at_front(&buffer, 64);

    void *dest = host_buffer_get_current_piece_data_pointer(buffer);
    (void)data;
    pbuf_copy_partial(p, dest, size, 0);

    wwd_network_send_ethernet_data(buffer, WWD_STA_INTERFACE);

    return ERR_OK;
}

/*
 * Should be called at the beginning of the program to set up the
 * network interface.
 *
 * This function should be passed as a parameter to netif_add().
 *
 * @param[in] netif the lwip network interface structure for this netif
 * @return ERR_OK if the loopif is initialized
 *         ERR_MEM if private data couldn't be allocated
 *         any other err_t on error
 */
err_t eth_arch_enetif_init(struct netif *netif)
{
    // Make sure WICED is up, noop if already init
    wiced_result_t res = wiced_init();
    MBED_ASSERT(res == WICED_SUCCESS);

    // get mac address
    wiced_mac_t mac;
    res = wiced_wifi_get_mac_address(&mac);
    MBED_ASSERT(res == WICED_SUCCESS);

    MBED_STATIC_ASSERT(sizeof(netif->hwaddr) >= sizeof(mac.octet),
            "Must have enouch space for mac address");
    memcpy(netif->hwaddr, mac.octet, sizeof(mac.octet));
    netif->hwaddr_len = sizeof(mac.octet);

    // get mtu
    netif->mtu = WICED_PAYLOAD_MTU;

    // get flags, note link starts already up
    netif->flags
        = NETIF_FLAG_BROADCAST
        | NETIF_FLAG_ETHARP
        | NETIF_FLAG_ETHERNET
        | NETIF_FLAG_LINK_UP;

    // get interface name
    netif->hostname = "wiced";
    memcpy(netif->name, "wl", 2);

    // setup output functions
#if LWIP_IPV4
    netif->output = etharp_output;
#endif
#if LWIP_IPV6
    netif->output_ip6 = ethip6_output;
#endif

    // setup link output function
    netif->linkoutput = host_network_send_ethernet_data;

    // keep track of netif for wiced
    host_network_netif = netif;

    return ERR_OK;
}

/* Other stubs
 */
wiced_result_t wiced_network_deinit      (void) { return WICED_SUCCESS; }
wiced_result_t wiced_network_init        (void) { return WICED_SUCCESS; }
void wiced_wireless_link_down_handler    (void) {}
void wiced_wireless_link_renew_handler   (void) {}
void wiced_wireless_link_up_handler      (void) {}

