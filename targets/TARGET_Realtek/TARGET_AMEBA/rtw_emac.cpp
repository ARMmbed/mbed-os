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

#include "rtw_emac.h"
#include "EMACMemoryManager.h"

#include "rtos.h"
#include "lwip/pbuf.h"
#include "netif/etharp.h"

#include "lwip_intf.h"
#include "wifi_constants.h"
#include "wifi_conf.h"

#define RTW_EMAC_MTU_SIZE  (1500U)

RTW_EMAC::RTW_EMAC() 
{
    set_callback_func((emac_callback)(&RTW_EMAC::wlan_emac_recv), this);
}

uint32_t RTW_EMAC::get_mtu_size() const
{
    return RTW_EMAC_MTU_SIZE;
}

uint32_t RTW_EMAC::get_align_preference() const
{
    return true;
}

void RTW_EMAC::get_ifname(char *name, uint8_t size) const
{
    MBED_ASSERT(name != NULL);
    strncpy(name, "r0", size);
}

uint8_t RTW_EMAC::get_hwaddr_size() const
{
    return ETHARP_HWADDR_LEN;
}

bool RTW_EMAC::get_hwaddr(uint8_t *addr) const
{
    char mac[20];
    int val[6];
    int i;

    if (RTW_SUCCESS == wifi_get_mac_address(mac)) {
        if (sscanf(mac, "%x:%x:%x:%x:%x:%x", 
            &val[0], &val[1], &val[2], &val[3], &val[4], &val[5]) != 6) {
            printf("Get HW address failed\r\n");
        }
        for (i = 0; i < 6; i++) {
            addr[i] = (unsigned char) val[i];
        }
    } else {
        printf("Get HW address failed\r\n");
    }
}

void RTW_EMAC::set_hwaddr(const uint8_t *addr)
{
}

bool RTW_EMAC::link_out(emac_mem_buf_t *buf)
{
    struct eth_drv_sg *sg_list;
    int sg_len = 0;
    int tot_len;
    emac_mem_buf_t *p;
    bool ret = true;
    if (!rltk_wlan_running(0)) {
        memory_manager->free(buf);
        return false;
    }

    sg_list = (struct eth_drv_sg *)malloc(sizeof(struct eth_drv_sg)*MAX_ETH_DRV_SG);
    if (sg_list == 0) {
        memory_manager->free(buf);
        return false;
    }

    p = buf;
    tot_len = memory_manager->get_total_len(p);
    for (; p != NULL && sg_len < MAX_ETH_DRV_SG; p = memory_manager->get_next(p)) {
        sg_list[sg_len].buf = (unsigned int)(static_cast<uint8_t *>(memory_manager->get_ptr(p)));
        sg_list[sg_len].len = memory_manager->get_len(p);
        sg_len++;
    }
    if (sg_len) {
        if (rltk_wlan_send(0, sg_list, sg_len, tot_len) != 0) {
            ret = false;
        }
    }

    memory_manager->free(buf);
    free(sg_list);
    return ret;
}

bool RTW_EMAC::power_up()
{
    wifi_on(RTW_MODE_STA);
    wait_ms(1000);
    wlan_emac_link_change(true);
    return true;
}

void RTW_EMAC::power_down()
{
    wifi_off();
}

void RTW_EMAC::set_link_input_cb(emac_link_input_cb_t input_cb)
{
    emac_link_input_cb = input_cb;
}

void RTW_EMAC::set_link_state_cb(emac_link_state_change_cb_t state_cb)
{
    emac_link_state_cb = state_cb;
}

void RTW_EMAC::add_multicast_group(const uint8_t *addr)
{
}

void RTW_EMAC::remove_multicast_group(const uint8_t *addr)
{
}

void RTW_EMAC::set_all_multicast(bool all)
{
}

void RTW_EMAC::set_memory_manager(EMACMemoryManager &mem_mngr)
{
    memory_manager = &mem_mngr;
}

void RTW_EMAC::wlan_emac_recv(void *param, struct netif *netif, uint32_t len)
{
    struct eth_drv_sg sg_list[MAX_ETH_DRV_SG] = {0};
    emac_mem_buf_t *buf;
    RTW_EMAC *enet = static_cast<RTW_EMAC *>(param);
    emac_mem_buf_t *p;
    int sg_len = 0;
    if (!rltk_wlan_running(0)) {
        return;
    }

    if (len > MAX_ETH_MSG || len < 0) {
        len = MAX_ETH_MSG;
    }

    buf = enet->memory_manager->alloc_heap(len, 0);
    if (buf == NULL) {
        return;
    } 

    enet->memory_manager->set_len(buf, len);
    p = buf;
    for (; p != NULL && sg_len < MAX_ETH_DRV_SG; p = enet->memory_manager->get_next(p)) {
        sg_list[sg_len].buf = (unsigned int)(static_cast<uint8_t *>(enet->memory_manager->get_ptr(p)));
        sg_list[sg_len].len = enet->memory_manager->get_len(p);
        sg_len++;
    }

    rltk_wlan_recv(0, sg_list, sg_len);
    if (enet->emac_link_input_cb) {
        enet->emac_link_input_cb(buf);
    }
}

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
    int val[6];
    int i;
    if (RTW_SUCCESS == wifi_get_mac_address(hwaddr)) {
        if (sscanf(hwaddr, "%x:%x:%x:%x:%x:%x",
            &val[0], &val[1], &val[2], &val[3], &val[4], &val[5]) != 6) {
            printf("Get HW address failed\r\n");
        }
        for (i = 0; i < 6; i++) {
            mac[i] = (unsigned char) val[i];
        }
    } else {
        printf("Get HW address failed\r\n");
        mbed_default_mac_address(mac);
    }
}

void RTW_EMAC::wlan_emac_link_change(bool up)
{
    if (emac_link_state_cb) {
        emac_link_state_cb(up);
    }
}

RTW_EMAC &RTW_EMAC::get_instance() {
    static RTW_EMAC rtw_emac;
    return rtw_emac;
}
// Weak so a module can override
MBED_WEAK EMAC &EMAC::get_default_instance() {
    return RTW_EMAC::get_instance();
}
#endif
