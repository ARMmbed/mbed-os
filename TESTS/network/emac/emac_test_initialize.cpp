/*
 * Copyright (c) 2017, ARM Limited, All Rights Reserved
 * SPDX-License-Identifier: Apache-2.0
 *
 * Licensed under the Apache License, Version 2.0 (the "License"); you may
 * not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS, WITHOUT
 * WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include <inttypes.h>
#include "mbed.h"
#include "mbed_stats.h"
#include "greentea-client/test_env.h"
#include "unity.h"
#include "utest.h"

#if MBED_CONF_APP_TEST_WIFI || MBED_CONF_APP_TEST_ETHERNET

#if MBED_CONF_APP_TEST_EMAC

#include "EthernetInterface.h"
#include "EMAC.h"
#include "EMACMemoryManager.h"
#include "emac_TestMemoryManager.h"
#include "emac_TestNetworkStack.h"

#else

#include "inttypes.h"

#if MBED_CONF_APP_TEST_WIFI
#ifdef TARGET_UBLOX_EVK_ODIN_W2
#include "wifi_emac_api.h"
#include "OdinWiFiInterface.h"
#endif
#ifdef TARGET_REALTEK_RTL8195AM
#include "rtw_emac.h"
#include "RTWInterface.h"
#endif
#endif

#include "emac_api.h"

#endif

#include "emac_initialize.h"
#include "emac_tests.h"
#include "emac_util.h"

using namespace utest::v1;

static unsigned char eth_mac_addr[ETH_MAC_ADDR_LEN];

#if !MBED_CONF_APP_TEST_EMAC
static char emac_if_link_state_change_cb_data[] = "link_state_change_cb_data";
static char emac_if_link_input_cb_data[] = "link_input_cb_data";
#endif

void test_emac_initialize()
{
    worker_loop_init();

#if MBED_CONF_APP_TEST_EMAC

#if MBED_CONF_APP_TEST_ETHERNET

    static EthernetInterface *network_interface = new EthernetInterface;

#elif MBED_CONF_APP_TEST_WIFI

    // Add wifi classes here
#ifdef TARGET_UBLOX_EVK_ODIN_W2
    static WiFiInterface *network_interface = new OdinWiFiInterface;
#elif TARGET_REALTEK_RTL8195AM
    static WiFiInterface *network_interface = new RTWInterface;
#else
    static WiFiInterface *network_interface = new WiFiInterface;
#endif

#if MBED_CONF_APP_WIFI_SCAN
    WiFiAccessPoint ap[30];

    int size = network_interface->scan(ap, 30);

    for (int i=0; i<size; i++) {
        const char *ssid = ap[i].get_ssid();
        nsapi_security_t security = ap[i].get_security();
        int8_t rssi = ap[i].get_rssi();
        char ch = ap[i].get_channel();

        printf("BS %i\r\n", i);
        printf("ssid %s\r\n", ssid);
        printf("security %i\r\n", security);
        printf("rssi %i\r\n", rssi);
        printf("ch %i\r\n\r\n", ch);
    }
#endif

    network_interface->set_credentials(MBED_CONF_APP_WIFI_SSID, MBED_CONF_APP_WIFI_PASSWORD, MBED_CONF_APP_WIFI_SECURITY);

#endif

    // Power up the interface and emac driver
    network_interface->connect();

    worker_loop_link_up_wait();
#else

#if MBED_CONF_APP_TEST_WIFI
    static WiFiInterface *wifi;

#ifdef TARGET_UBLOX_EVK_ODIN_W2
    wifi = new OdinWiFiInterface;
#endif
#ifdef TARGET_REALTEK_RTL8195AM
    wifi = new RTWInterface;
#endif

#if MBED_CONF_APP_WIFI_SCAN
    WiFiAccessPoint ap[30];

    int size = wifi->scan(ap, 30);

    for (int i=0; i<size; i++) {
        const char *ssid = ap[i].get_ssid();
        nsapi_security_t security = ap[i].get_security();
        int8_t rssi = ap[i].get_rssi();
        char ch = ap[i].get_channel();

        printf("BS %i\r\n", i);
        printf("ssid %s\r\n", ssid);
        printf("security %i\r\n", security);
        printf("rssi %i\r\n", rssi);
        printf("ch %i\r\n\r\n", ch);
    }
#endif

    wifi->set_credentials(MBED_CONF_APP_WIFI_SSID, MBED_CONF_APP_WIFI_PASSWORD, MBED_CONF_APP_WIFI_SECURITY);
    wifi->connect();

    const char *ip_addr = wifi->get_ip_address();
    printf("connected IP %s\r\n\r\n", ip_addr);

    TEST_ASSERT(emac_if_init());
#endif
#endif
}

unsigned char *emac_if_get_hw_addr(void)
{
    return &eth_mac_addr[0];
}

#if MBED_CONF_APP_TEST_EMAC
EMAC *emac_if_get(void)
{
    return &EMAC::get_default_instance();
}

EmacTestMemoryManager *emac_m_mngr_get(void)
{
    return &EmacTestMemoryManager::get_instance();
}

#else

emac_interface_t *emac_if_get(void)
{
#if MBED_CONF_APP_TEST_WIFI
#ifdef TARGET_UBLOX_EVK_ODIN_W2
    return wifi_emac_get_interface();
#endif
#ifdef TARGET_REALTEK_RTL8195AM
    return wlan_emac_init_interface();
#endif
#else
    return 0;
#endif
}
#endif

bool emac_if_init(void)
{
#if MBED_CONF_APP_TEST_EMAC
    static EMAC *emac = &EMAC::get_default_instance();
    static EmacTestMemoryManager *memory_manager = &EmacTestMemoryManager::get_instance();
    emac->set_memory_manager(*memory_manager);

    emac->set_link_input_cb(emac_if_link_input_cb);
    emac->set_link_state_cb(emac_if_link_state_change_cb);

    if (!emac->power_up()) {
        TEST_ASSERT_MESSAGE(0, "emac power up failed!");
    }

    int hwaddr_len = emac->get_hwaddr_size();
    printf("emac hwaddr length %i\r\n\r\n", hwaddr_len);

    TEST_ASSERT_MESSAGE(hwaddr_len == 6, "invalid emac hwaddr length!");

    // If driver updates this, write it back, otherwise write default from mbed_mac_address
    mbed_mac_address(reinterpret_cast<char *>(&eth_mac_addr[0]));
    emac->get_hwaddr(eth_mac_addr);
    emac->set_hwaddr(eth_mac_addr);
    printf("emac hwaddr %x:%x:%x:%x:%x:%x\r\n\r\n", eth_mac_addr[0],eth_mac_addr[1],eth_mac_addr[2],eth_mac_addr[3],eth_mac_addr[4],eth_mac_addr[5]);

    int mtu_size = emac->get_mtu_size();
    printf("emac mtu %i\r\n\r\n", mtu_size);
    emac_if_set_mtu_size(mtu_size);

    char hw_name[11];
    emac->get_ifname(hw_name, 10);
    printf("emac if name %s\r\n\r\n", hw_name);

    return true;
#else
    emac_interface_t *emac_if = emac_if_get();

    emac_if->ops.set_link_input_cb(emac_if, emac_if_link_input_cb, emac_if_link_input_cb_data);
    emac_if->ops.set_link_state_cb(emac_if, emac_if_link_state_change_cb, emac_if_link_state_change_cb_data);

    int hwaddr_len = emac_if->ops.get_hwaddr_size(emac_if);
    printf("emac hwaddr length %i\r\n\r\n", hwaddr_len);

    if (hwaddr_len == 6) {
        emac_if->ops.get_hwaddr(emac_if, eth_mac_addr);
        printf("emac hwaddr %x:%x:%x:%x:%x:%x\r\n\r\n", eth_mac_addr[0],eth_mac_addr[1],eth_mac_addr[2],eth_mac_addr[3],eth_mac_addr[4],eth_mac_addr[5]);
    }

    int  mtu_size = emac_if->ops.get_mtu_size(emac_if);
    printf("emac mtu %i\r\n\r\n", mtu_size);
    emac_if_set_mtu_size(mtu_size);

    char hw_name[11];
    emac_if->ops.get_ifname(emac_if, hw_name, 10);
    printf("emac if name %s\r\n\r\n", hw_name);

    if (!emac_if->ops.power_up(emac_if)) {
        return false;
    }

    return true;
#endif
}

#endif
