/* Copyright (c) 2019 Unisoc Communications Inc.
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

#include "WiFiInterface.h"
#include "RdaWiFiInterface.h"
#include "rda5991h_wland.h"
#include "nsapi_types.h"
#include "wland_types.h"
#include "rda_sys_wrapper.h"

typedef enum {
    WIFI_CONNECTED,
    WIFI_DISCONNECTED,	
}WIFI_STATE;

static WIFI_STATE wifi_state = WIFI_DISCONNECTED;

void daemon(void *para)
{
    void *main_msgQ = NULL;
    rda_msg msg;
    int ret;
    RDAWiFiInterface *wifi = (RDAWiFiInterface *)para;
    main_msgQ = rda_mail_create(10, sizeof(unsigned int)*4);
    wifi->set_msg_queue(main_msgQ);
    while(1){
        rda_mail_get(main_msgQ, (void*)&msg, osWaitForever);
        switch(msg.type)
        {
            case MAIN_RECONNECT:
                printf("wifi disconnect!\r\n");
                ret = wifi->disconnect();
                if(ret != 0){
                    printf("disconnect failed!\r\n");
                    break;
                }
                ret = wifi->reconnect();
                while(ret != 0){
                    osDelay(5*1000);
                    ret = wifi->reconnect();
                };
                break;
            default:
                printf("unknown msg\r\n");
                break;
        }
    }
}

nsapi_error_t RDAWiFiInterface::set_channel(uint8_t channel)
{
    int ret= 0;
    init();

    if (channel > 13)
        return NSAPI_ERROR_PARAMETER;

    if (channel == 0) {
        _channel = 0;
        return NSAPI_ERROR_OK;
    }

    ret = rda5981_set_channel(channel);
    if (ret == 0) {
        _channel = channel;
        return NSAPI_ERROR_OK;
    } else {
        return NSAPI_ERROR_TIMEOUT;
    }
}

int8_t RDAWiFiInterface::get_rssi()
{
    return rda5981_get_rssi();
}

nsapi_error_t RDAWiFiInterface::init()
{
    if (!_interface) {
        if (!_emac.power_up()) {
            LWIP_DEBUGF(NETIF_DEBUG,("power up failed!\n"));
        }
        nsapi_error_t err = _stack.add_ethernet_interface(_emac, true, &_interface);
        if (err != NSAPI_ERROR_OK) {
            _interface = NULL;
            return err;
        }
        _interface->attach(_connection_status_cb);
        //rda_thread_new("daemon", daemon, this, DEFAULT_THREAD_STACKSIZE*4, osPriorityNormal);
    }
    return NSAPI_ERROR_OK;
}

nsapi_error_t RDAWiFiInterface::set_credentials(const char *ssid, const char *pass,
                nsapi_security_t security)
{
    if (ssid == 0 || strlen(ssid) == 0) {
        return NSAPI_ERROR_PARAMETER;
    }
    if (security != NSAPI_SECURITY_NONE && (pass == 0 || strlen(pass) == 0)) {
        return NSAPI_ERROR_PARAMETER;
    }
    if (strlen(ssid) > 32 || strlen(pass) > 63) {
        return NSAPI_ERROR_PARAMETER;
    }
    memcpy((void*)_ssid, (void*)ssid, strlen(ssid));
    _ssid[strlen(ssid)] = '\0';
    memcpy((void*)_pass, (void*)pass, strlen(pass));
    _pass[strlen(pass)] = '\0';
    _security = security;
    return NSAPI_ERROR_OK;
}

nsapi_error_t RDAWiFiInterface::connect(const char *ssid, const char *pass,
                nsapi_security_t security, uint8_t channel)
{
    rda_msg msg;
    bool find = false;
    int i = 0;
    rda5981_scan_result bss;
    int ret = 0;

    if(wifi_state == WIFI_CONNECTED) {
        return NSAPI_ERROR_IS_CONNECTED;
    }
	
    if (ssid == NULL || ssid[0] == 0) {
        return NSAPI_ERROR_PARAMETER;
    }
	
    set_credentials(ssid, pass, security);
    set_channel(channel);
	
    init();

    //reset all scan result to avoid any previous stored SSID/PW/CHANNEL
    rda5981_del_scan_all_result();
    rda5981_scan(NULL,0,0);
    if(rda5981_check_scan_result_name(ssid) != 0) {
        for (i = 0; i< 5; i++) {
            rda5981_scan(NULL, 0, 0);
            if(rda5981_check_scan_result_name(ssid) == 0) {
                find = true;
                break;
            }
        }
    } else {
        find = true;
    }

    if (find == false) {
        LWIP_DEBUGF(NETIF_DEBUG,("can not find the ap.\r\n"));
        return NSAPI_ERROR_NO_SSID;
    }
    bss.channel = 15;
    rda5981_get_scan_result_name(&bss, ssid);
    if ((channel !=0) && (bss.channel != channel)) {
        LWIP_DEBUGF(NETIF_DEBUG, ("invalid channel\r\n"));
        return NSAPI_ERROR_CONNECTION_TIMEOUT;
    }

    memcpy(gssid, ssid, strlen(ssid));
    if (pass[0] != 0) {
        memcpy(gpass, pass, strlen(pass));
    }
    memset(gbssid, 0, NSAPI_MAC_BYTES);
    gssid[strlen(ssid)] = gpass[strlen(pass)] = '\0';

    msg.type = WLAND_CONNECT;
    rda_mail_put(wland_msgQ, (void*)&msg, osWaitForever);
    ret = rda_sem_wait(wifi_auth_sem, 10000);
    if (ret) {
        return NSAPI_ERROR_CONNECTION_TIMEOUT;
    }
	
	wifi_state = WIFI_CONNECTED;

    ret = _interface->bringup(_dhcp,
          _ip_address[0] ? _ip_address : 0,
          _netmask[0] ? _netmask : 0,
          _gateway[0] ? _gateway : 0,
          DEFAULT_STACK,
          _blocking);
    LWIP_DEBUGF(NETIF_DEBUG,("Interface bringup up status:%d\r\n",ret));

    if( ret == NSAPI_ERROR_OK || ret == NSAPI_ERROR_IS_CONNECTED ) {
        ret = NSAPI_ERROR_OK;
    }
    else if( ret == NSAPI_ERROR_DHCP_FAILURE) {
        disconnect();
    }

    return ret;
}


nsapi_error_t RDAWiFiInterface::connect()
{
    return connect(_ssid, _pass, _security, _channel);
}

nsapi_error_t RDAWiFiInterface::disconnect()
{
    rda_msg msg;

    if(wifi_state == WIFI_DISCONNECTED) {
        return NSAPI_ERROR_NO_CONNECTION;
    }
    wifi_state = WIFI_DISCONNECTED;
    void* wifi_disconnect_sem = rda_sem_create(0);
    msg.type = WLAND_DISCONNECT;
    msg.arg1 = (unsigned int)wifi_disconnect_sem;
    rda_mail_put(wland_msgQ, (void*)&msg, osWaitForever);
    rda_sem_wait(wifi_disconnect_sem, osWaitForever);
    rda_sem_delete(wifi_disconnect_sem);
    if (_interface) {
        return _interface->bringdown();
    }

    return NSAPI_ERROR_NO_CONNECTION;
}

nsapi_error_t RDAWiFiInterface::reconnect()
{
    rda_msg msg;
    bool find = false;
    int i = 0;
    rda5981_scan_result bss;
    int ret = 0;

    if (_ssid == NULL || _ssid[0] == 0) {
        return NSAPI_ERROR_PARAMETER;
    }

    rda5981_del_scan_all_result();
    if(rda5981_check_scan_result_name(_ssid) != 0) {
        for (i = 0; i< 5; i++) {
            rda5981_scan(NULL, 0, 0);
            if(rda5981_check_scan_result_name(_ssid) == 0) {
                find = true;
                break;
            }
        }
    } else {
        find = true;
    }

    if (find == false) {
        LWIP_DEBUGF(NETIF_DEBUG,"can not find the ap.\r\n");
        return NSAPI_ERROR_CONNECTION_TIMEOUT;
    }
    bss.channel = 15;
    rda5981_get_scan_result_name(&bss, _ssid);
    if ((_channel !=0) && (bss.channel != _channel)) {
        LWIP_DEBUGF(NETIF_DEBUG, "invalid channel\r\n");
        return NSAPI_ERROR_CONNECTION_TIMEOUT;
    }

    memcpy(gssid, _ssid, strlen(_ssid));
    if (_pass[0] != 0) {
        memcpy(gpass, _pass, strlen(_pass));
    }
    memset(gbssid, 0, NSAPI_MAC_BYTES);
    gssid[strlen(_ssid)] = gpass[strlen(_pass)] = '\0';

    msg.type = WLAND_CONNECT;
    rda_mail_put(wland_msgQ, (void*)&msg, osWaitForever);
    ret = rda_sem_wait(wifi_auth_sem, 10000);
    if (ret) {
        return NSAPI_ERROR_CONNECTION_TIMEOUT;
    }

    if(_dhcp) {
        memset(_ip_address, 0, sizeof(_ip_address));
        memset(_netmask, 0, sizeof(_netmask));
        memset(_gateway, 0, sizeof(_gateway));
    }

    ret = _interface->bringup(_dhcp,
          _ip_address[0] ? _ip_address : 0,
          _netmask[0] ? _netmask : 0,
          _gateway[0] ? _gateway : 0,
          DEFAULT_STACK,
          _blocking);
    LWIP_DEBUGF(NETIF_DEBUG,("Interface bringup up status:%d\r\n",ret));

    if( ret == NSAPI_ERROR_OK || ret == NSAPI_ERROR_IS_CONNECTED ) {
        ret = NSAPI_ERROR_OK;
		wifi_state = WIFI_CONNECTED;
    }
    else if( ret == NSAPI_ERROR_DHCP_FAILURE) {
        disconnect();
    }
    return ret;
}


nsapi_size_or_error_t RDAWiFiInterface::scan(WiFiAccessPoint *res, nsapi_size_t count)
{
    int bss_num = 0, i;
    rda5981_scan_result *bss;
    nsapi_wifi_ap_t ap;

    init();

    rda5981_scan(NULL, 0, 0);
    bss_num = rda5981_get_scan_num();
    if (count != 0) {
        bss_num = (bss_num < count) ? bss_num : count;
    }
    if (res) {
        bss = (rda5981_scan_result *)malloc(bss_num * sizeof(rda5981_scan_result));
        rda5981_get_scan_result(bss, bss_num);
        for (i=0; i<bss_num; i++) {
            memset(&ap, 0, sizeof(nsapi_wifi_ap_t));
            memcpy(ap.bssid, bss[i].BSSID, 6);
            memcpy(ap.ssid, bss[i].SSID, bss[i].SSID_len);
            ap.channel = bss[i].channel;
            ap.rssi = bss[i].RSSI;
            if (bss[i].secure_type == ENCRYPT_NONE) {
                ap.security = NSAPI_SECURITY_NONE;
            } else if(bss[i].secure_type & ENCRYPT_WEP) {
                ap.security = NSAPI_SECURITY_WEP;
            } else if((bss[i].secure_type & (ENCRYPT_WPA_TKIP | ENCRYPT_WPA_CCMP)) && \
                        (bss[i].secure_type & (ENCRYPT_WPA2_TKIP | ENCRYPT_WPA2_CCMP))) {
                ap.security = NSAPI_SECURITY_WPA_WPA2;
            } else if((bss[i].secure_type & (ENCRYPT_WPA_TKIP | ENCRYPT_WPA_CCMP))) {
                ap.security = NSAPI_SECURITY_WPA;
            } else {
                ap.security = NSAPI_SECURITY_WPA2;
            }
            WiFiAccessPoint ap_temp(ap);
            memcpy(&res[i], &ap_temp, sizeof(WiFiAccessPoint));
        }
        free(bss);
    }
    return bss_num;
 
}

WiFiInterface *WiFiInterface::get_default_instance() {
    static RDAWiFiInterface wifinet;
    return &wifinet;
}

nsapi_size_or_error_t RDAWiFiInterface::set_msg_queue(void *queue)
{
    //TO_DO: No need for 1st stage since application already control the logic.
    //rda5981_set_main_queue(queue);
    return 0;
}
