/*
 * Copyright (c) STMicroelectronics 2021
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

#include "EMW3080B_EMAC.h"
#include "mx_wifi_ipc.h"

#include "lwip/etharp.h"
#include "lwip/ethip6.h"

#define DEBUG_SILENT  0
#define DEBUG_WARNING 1
#define DEBUG_INFO    2
#define DEBUG_LOG     3
#define DEFAULT_DEBUG DEBUG_SILENT


EMW3080B_EMAC::EMW3080B_EMAC()
{
    _debug_level = DEFAULT_DEBUG;
}


uint32_t EMW3080B_EMAC::get_mtu_size() const
{
    debug_if(_debug_level >= DEBUG_INFO, "EMW3080B_EMAC : get_mtu_size %d\n", MX_WIFI_MTU_SIZE);
    return MX_WIFI_MTU_SIZE;
}

uint32_t EMW3080B_EMAC::get_align_preference() const
{
    debug_if(_debug_level >= DEBUG_INFO, "EMW3080B_EMAC : get_align_preference 0\n");
    return 0;
}

void EMW3080B_EMAC::add_multicast_group(const uint8_t *addr)
{
    debug_if(_debug_level >= DEBUG_WARNING, "EMW3080B_EMAC : add_multicast_group is not supported\n");
}

void EMW3080B_EMAC::remove_multicast_group(const uint8_t *addr)
{
    debug_if(_debug_level >= DEBUG_WARNING, "EMW3080B_EMAC : remove_multicast_group is not supported\n");
}

void EMW3080B_EMAC::set_all_multicast(bool all)
{
    debug_if(_debug_level >= DEBUG_WARNING, "EMW3080B_EMAC : set_all_multicast not supported\n");
    /* No-op at this stage */
}

void EMW3080B_EMAC::power_down()
{
    debug_if(_debug_level >= DEBUG_INFO, "EMW3080B_EMAC : power_down\n");

}

bool EMW3080B_EMAC::power_up()
{
    debug_if(_debug_level >= DEBUG_INFO, "EMW3080B_EMAC : power_up\n");

    return true;
}


bool EMW3080B_EMAC::get_hwaddr(uint8_t *addr) const
{
    bool ret;
    debug_if(_debug_level >= DEBUG_INFO, "EMW3080B_EMAC : get_hwaddr\n");

    MX_WIFIObject_t *pMxWifiObj = wifi_obj_get();
    if (pMxWifiObj) {
        addr[0] = pMxWifiObj->SysInfo.MAC[0];
        addr[1] = pMxWifiObj->SysInfo.MAC[1];
        addr[2] = pMxWifiObj->SysInfo.MAC[2];
        addr[3] = pMxWifiObj->SysInfo.MAC[3];
        addr[4] = pMxWifiObj->SysInfo.MAC[4];
        addr[5] = pMxWifiObj->SysInfo.MAC[5];
        ret = true;
    } else {
        ret = false;
    }
    return ret;
}


void EMW3080B_EMAC::set_hwaddr(const uint8_t *addr)
{
    debug_if(_debug_level >= DEBUG_WARNING, "EMW3080B_EMAC : set_hwaddr ignoring this command ,not possible to set MAC address for MXCHIP device\n");
}

uint8_t EMW3080B_EMAC::get_hwaddr_size() const
{
    debug_if(_debug_level >= DEBUG_INFO, "EMW3080B_EMAC : get_hwaddr_size %u\n", EMW3080B_HWADDR_SIZE);
    return EMW3080B_HWADDR_SIZE;
}


static void emac_data_callback(mx_buf_t *buffer, void *user_args)
{
    /* to retrieve the C++ class */
    EMW3080B_EMAC  emac = EMW3080B_EMAC::get_instance();
    emac.mx_wifi_netlink_input_callback(buffer);
}

void EMW3080B_EMAC::set_link_input_cb(emac_link_input_cb_t input_cb)
{
    debug_if(_debug_level >= DEBUG_INFO, "EMW3080B_EMAC : set_link_input_cb\n");
    if (MX_WIFI_STATUS_OK != MX_WIFI_Network_bypass_mode_set(wifi_obj_get(), 1, emac_data_callback, NULL)) {
        error("EMW3080B_EMAC : can not set set_link_input_cb\n");
    } else {
        _emac_input_data_cb = input_cb;
    }
}

extern "C" {
    void emac_status_changed(uint8_t cate, uint8_t status, void *arg)
    {
        /* to retrieve the C++ class */
        EMW3080B_EMAC  emac = EMW3080B_EMAC::get_instance();
        emac.mx_wifi_status_changed(cate, status, arg);
    }
}

void EMW3080B_EMAC::mx_wifi_status_changed(uint8_t cate, uint8_t status, void *arg)
{
    bool mx_wifi_driver_up;
    if ((uint8_t)MC_STATION == cate) {
        switch (status) {
            case MWIFI_EVENT_STA_DOWN:
                mx_wifi_driver_up = false;
                break;

            case MWIFI_EVENT_STA_UP:
                mx_wifi_driver_up = true;
                break;

            default:
                debug_if(_debug_level >= DEBUG_WARNING, "EMW3080B_EMAC : mx_wifi_status_changed get unknown Event from EMW3080B\n");
                mx_wifi_driver_up = false;
                break;
        }

        debug_if(_debug_level >= DEBUG_INFO, "EMW3080B_EMAC : Interface is %s (calling)\n", mx_wifi_driver_up ? "UP" : "DOWN");
        if (_emac_link_state_cb) {
            _emac_link_state_cb(mx_wifi_driver_up);
        }

    } else if ((uint8_t)MC_SOFTAP == cate) {
        switch (status) {
            case MWIFI_EVENT_AP_DOWN:
                break;

            case MWIFI_EVENT_AP_UP:
                break;

            default:
                break;
        }
    } else {
        /* nothing */
    }
}



void EMW3080B_EMAC::set_link_state_cb(emac_link_state_change_cb_t state_cb)
{
    _emac_link_state_cb = state_cb;


    if (MX_WIFI_RegisterStatusCallback_if(
                wifi_obj_get(),
                emac_status_changed,
                NULL /* void * arg */,
                (mwifi_if_t)MC_STATION)) {
        debug_if(_debug_level >= DEBUG_WARNING, "EMW3080B_EMAC : Fail to register callback\n");
        return ;
    }


}

EMACMemoryManager  *emac3080b_global_memory_manager = NULL;

void EMW3080B_EMAC::set_memory_manager(EMACMemoryManager &mem_mngr)
{
    debug_if(_debug_level >= DEBUG_INFO, "EMW3080B_EMAC : set_memory_manager\n");
    _memory_manager = &mem_mngr;
    emac3080b_global_memory_manager = &mem_mngr;

}
#define EMW_PROTOCOL_HEADROOM  sizeof(wifi_bypass_out_cparams_t)

bool EMW3080B_EMAC::link_out(emac_mem_buf_t *buf)
{
    bool ret = true;

    emac_mem_buf_t *q;
    uint32_t  m = 0;
    uint32_t  len = 0;
    int32_t  mx_ret;

#if MX_WIFI_TX_BUFFER_NO_COPY
    for (q = buf; q != NULL; q = _memory_manager->get_next(q)) {
        len += _memory_manager->get_len(q);
    }

    mx_buf_t *net = mx_buf_alloc(len + EMW_PROTOCOL_HEADROOM);
    if (net != NULL) {
        MX_NET_BUFFER_HIDE_HEADER(net, EMW_PROTOCOL_HEADROOM);
        MX_NET_BUFFER_SET_PAYLOAD_SIZE(net, len);

        uint8_t *payload = MX_NET_BUFFER_PAYLOAD(net);
        for (q = buf; q != NULL; q = _memory_manager->get_next(q)) {
            memcpy(payload, _memory_manager->get_ptr(q), _memory_manager->get_len(q));
            payload += _memory_manager->get_len(q);
            debug_if(_debug_level >= DEBUG_LOG, "EMW3080B_EMAC : TX seg %" PRIu32 " %" PRIu32 " bytes\n", m, _memory_manager->get_len(q));
            m++;
        }

        debug_if(_debug_level >= DEBUG_LOG, "EMW3080B_EMAC : %" PRIu32 " TX %" PRIu32 "\n", m, MX_NET_BUFFER_GET_PAYLOAD_SIZE(net));
        mx_ret = MX_WIFI_Network_bypass_netlink_output(
                     wifi_obj_get(),
                     MX_NET_BUFFER_PAYLOAD(net),
                     MX_NET_BUFFER_GET_PAYLOAD_SIZE(net),
                     STATION_IDX
                 );
        debug_if(_debug_level >= DEBUG_LOG, "EMW3080B_EMAC : TX done lust e zero %" PRIu32 "\n", mx_ret);
        MX_NET_BUFFER_FREE(net);
        if (mx_ret != MX_WIFI_STATUS_OK) {
            debug_if(_debug_level >= DEBUG_WARNING, "EMW3080B_EMAC : link_out : failed to send buffer\n");
            ret = false;
        }
#else
    /* copy is managed internally */
    ret = MX_WIFI_Network_bypass_netlink_output(
              wifi_obj_get(),
              _memory_manager->get_ptr(q),
              _memory_manager->get_len(q),
              STATION_IDX
          );
#endif /* MX_WIFI_TX_BUFFER_NO_COPY */
    } else
    {
        error("EMW3080B_EMAC : unable to allocate %" PRIu32 " bytes\n", len);
    }

    _memory_manager->free(buf);
    return ret;
}

void EMW3080B_EMAC::get_ifname(char *name, uint8_t size) const
{
    debug_if(_debug_level >= DEBUG_INFO, "EMW3080B_EMAC : get_ifname\n");
    if (size > 0) {
        strncpy(name, "st", size - 1);
        name[size - 1] = '\0';
    }
}

EMW3080B_EMAC &EMW3080B_EMAC::get_instance()
{
    static EMW3080B_EMAC emac;
    return emac;
}

void EMW3080B_EMAC::mx_wifi_netlink_input_callback(mx_buf_t *buffer)
{
    struct eth_hdr *ethernet_header;
    uint16_t        ethertype;
    uint32_t        len = 0U;

    if (buffer != NULL) {
        len = MX_NET_BUFFER_GET_PAYLOAD_SIZE(buffer);

        if ((len > 0U) && (len <= (uint32_t) MX_WIFI_MTU_SIZE)) {
            /* points to packet payload, which starts with an Ethernet header */
            ethernet_header = (struct eth_hdr *) MX_NET_BUFFER_PAYLOAD(buffer);

            ethertype = lwip_htons(ethernet_header->type);
            switch (ethertype) {
                case ETHTYPE_IP:
                /* case ETHTYPE_IPV6: */
                case ETHTYPE_ARP:
                /* PPPoE packet */
                case ETHTYPE_PPPOEDISC:
                case ETHTYPE_PPPOE: {
                    debug_if(_debug_level >= DEBUG_LOG, "EMW3080B_EMAC : process input packet 0x%02x, len=%" PRIu32 "\n", ethertype, len);
                    emac_mem_buf_t *p, *q;
                    uint32_t  index = 0;
                    p = _memory_manager->alloc_pool(len, 0);
                    if (p != NULL) {
                        uint8_t *src = (uint8_t *) ethernet_header;
                        uint32_t  m = 0;
                        for (q = p; q != NULL; q = _memory_manager->get_next(q)) {
                            uint8_t *dest = (uint8_t *) _memory_manager->get_ptr(q);
                            memcpy(dest, &src[index], _memory_manager->get_len(q));
                            index += _memory_manager->get_len(q);

                            if (index >= len) {
                                break;
                            }
                            m++;
                        }


                        if (_emac_input_data_cb) {
                            _emac_input_data_cb(p);
                        }
                    } else {
                        debug_if(_debug_level >= DEBUG_WARNING, "EMW3080B_EMAC : fail to allocate emac_mem_buf_t %" PRIu32 " bytes\n", len);
                    }
                }
                break;

                default:
                    break;
            }
        }
        MX_NET_BUFFER_FREE(buffer);
    }
}
