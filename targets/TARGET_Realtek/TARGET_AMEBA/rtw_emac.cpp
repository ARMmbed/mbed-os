/* mbed Microcontroller Library
 * Copyright (c) 2016 Realtek Semiconductor Corp.
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

#if DEVICE_EMAC

#include <stdio.h>
#include "mbed_assert.h"
#include "mbed_events.h"

#include "emac_api.h"
#include "rtos.h"

#include "lwip/pbuf.h"
#include "netif/etharp.h"

#include "lwip_intf.h"
#include "wifi_constants.h"
#include "wifi_conf.h"

#define RTW_EMAC_MTU_SIZE  (1500U)

static emac_interface_t *_emac;
static emac_link_input_fn link_input_cb;
static emac_link_state_change_fn link_state_cb;
static void *link_input_data;
static void *link_state_data;

static uint32_t wlan_get_mtu_size(emac_interface_t *emac)
{
    return RTW_EMAC_MTU_SIZE;
}

static void wlan_get_ifname(emac_interface_t *emac, char *name, uint8_t size)
{
    MBED_ASSERT(name != NULL);
    strncpy(name, "r0", size);
}

static uint8_t wlan_get_hwaddr_size(emac_interface_t *emac)
{
    return ETHARP_HWADDR_LEN;
}

static void wlan_get_hwaddr(emac_interface_t *emac, uint8_t *addr)
{
    char mac[20];    
    if(RTW_SUCCESS == wifi_get_mac_address(mac))
    {
        if (sscanf(mac, "%x:%x:%x:%x:%x:%x", &addr[0], &addr[1], &addr[2], &addr[3], &addr[4], &addr[5]) != 6)
            printf("Get HW address failed\r\n");
    }else{
            printf("Get HW address failed\r\n");
    }
}

static void wlan_set_hwaddr(emac_interface_t *emac, uint8_t *addr)
{
    
}


static bool wlan_link_out(emac_interface_t *emac, emac_stack_mem_t *buf)
{
    struct eth_drv_sg * sg_list=0;
    int sg_len = 0;
    int tot_len;
    struct pbuf *p;
    bool ret = true;

    if (!rltk_wlan_running(0)) {
        return false;
    }

    sg_list = (struct eth_drv_sg *)malloc(sizeof(struct eth_drv_sg)*MAX_ETH_DRV_SG);
    if(sg_list == 0){//malloc fail
        return false;
    }
    emac_stack_mem_ref(emac, buf);

    p = (struct pbuf *)buf;
    tot_len = p->tot_len;
    for (; p != NULL && sg_len < MAX_ETH_DRV_SG; p = p->next) {
        sg_list[sg_len].buf = (uint32_t) p->payload;
        sg_list[sg_len].len = p->len;
    sg_len++;
    }

    if (sg_len) {
        if (rltk_wlan_send(0, sg_list, sg_len, tot_len) != 0) {
            ret = false;
        }
    }

    emac_stack_mem_free(emac, buf);
    free(sg_list);
    return ret;
}

static bool wlan_power_up(emac_interface_t *emac)
{
    wifi_on(RTW_MODE_STA);
    wait_ms(1000);
    return true;
}

static void wlan_power_down(emac_interface_t *emac)
{
    wifi_off();
}

static void wlan_set_link_input_cb(emac_interface_t *emac, emac_link_input_fn cb, void *data)
{
    link_input_cb = cb;
    link_input_data = data;
}

static void wlan_set_link_state_cb(emac_interface_t *emac, emac_link_state_change_fn cb, void *data)
{
    link_state_cb = cb;
    link_state_data = data;
}

void wlan_emac_recv(struct netif *netif, int len)
{
    struct eth_drv_sg sg_list[MAX_ETH_DRV_SG];
    emac_stack_mem_t *buf;
    struct pbuf *p;
    int sg_len = 0;

    if (!rltk_wlan_running(0)) {
        return;
    }

    if (len > MAX_ETH_MSG || len < 0) {
        len = MAX_ETH_MSG;
    }

    buf = emac_stack_mem_alloc(NULL, len, 0);
    if (buf == NULL) {
        return;
    }   

    p = (struct pbuf *)buf;
    for (; p != NULL && sg_len < MAX_ETH_DRV_SG; p = p->next) {
        sg_list[sg_len].buf = (uint32_t) p->payload;
        sg_list[sg_len].len = p->len;
        sg_len++;
    }
    rltk_wlan_recv(0, sg_list, sg_len);

    if (link_input_cb) {
        link_input_cb(link_input_data, buf);
    }
    return;
}

const emac_interface_ops_t wlan_emac_interface = {
    .get_mtu_size = wlan_get_mtu_size,
    .get_ifname = wlan_get_ifname,
    .get_hwaddr_size = wlan_get_hwaddr_size,
    .get_hwaddr = wlan_get_hwaddr,
    .set_hwaddr = wlan_set_hwaddr,
    .link_out = wlan_link_out,
    .power_up = wlan_power_up,
    .power_down = wlan_power_down,
    .set_link_input_cb = wlan_set_link_input_cb,
    .set_link_state_cb = wlan_set_link_state_cb
};

void mbed_default_mac_address(char *mac) {
    unsigned char RTK_mac_addr[3] = {0x00, 0xE0, 0x4C}; // default Realtek mac address

    mac[0] = RTK_mac_addr[0];
    mac[1] = RTK_mac_addr[1];
    mac[2] = RTK_mac_addr[2];
    mac[3] = 0x87;
    mac[4] = 0x00;
    mac[5] = 0x01;

    return;
}

void mbed_mac_address(char *mac)
{
    char hwaddr[20];    
    if(RTW_SUCCESS == wifi_get_mac_address(hwaddr))
    {
        if (sscanf(hwaddr, "%x:%x:%x:%x:%x:%x", &mac[0], &mac[1], &mac[2], &mac[3], &mac[4], &mac[5]) != 6)
            printf("Get HW address failed\r\n");
    }else{
        printf("Get HW address failed\r\n");
        mbed_default_mac_address(mac);
    }
}

void wlan_emac_link_change(bool up)
{
    if (link_state_cb) {
        link_state_cb(link_state_data, up);
    }
}

emac_interface_t *wlan_emac_init_interface()
{

    if (_emac == NULL) {
        _emac = (emac_interface_t*) malloc(sizeof(emac_interface_t));
        MBED_ASSERT(_emac);
        _emac->hw = NULL;
        memcpy((void*)&_emac->ops, &wlan_emac_interface, sizeof(wlan_emac_interface));
    }
    return _emac;
}
#endif
