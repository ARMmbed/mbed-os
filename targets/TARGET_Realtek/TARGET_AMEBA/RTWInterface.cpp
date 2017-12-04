/* Ameba implementation of NetworkInterfaceAPI
 * Copyright (c) 2015 ARM Limited
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

#include "mbed.h"
#include "rtos.h"

#include "RTWInterface.h"
#include "mbed_interface.h"
#include "rtw_emac.h"

#include "wifi_constants.h"
#include "wifi_conf.h"
#include "lwip_stack.h"

#include "osdep_service.h"

typedef struct _wifi_scan_hdl {
    _sema scan_sema;
    nsapi_size_t ap_num;
    nsapi_size_t scan_num;
    WiFiAccessPoint *ap_details;
} wifi_scan_hdl;

#define MAX_SCAN_TIMEOUT (15000)

static rtw_result_t scan_result_handler( rtw_scan_handler_result_t* malloced_scan_result )
{
    wifi_scan_hdl *scan_handler = (wifi_scan_hdl *)malloced_scan_result->user_data;
    if (malloced_scan_result->scan_complete != RTW_TRUE) {
        if(scan_handler->ap_details && scan_handler->scan_num > scan_handler->ap_num){
            nsapi_wifi_ap_t ap;    
            rtw_scan_result_t* record = &malloced_scan_result->ap_details;
            record->SSID.val[record->SSID.len] = 0; /* Ensure the SSID is null terminated */    
            memset((void*)&ap, 0x00, sizeof(nsapi_wifi_ap_t));
            memcpy(ap.ssid, record->SSID.val, record->SSID.len);
            memcpy(ap.bssid, record->BSSID.octet, 6);
            switch (record->security){
                case RTW_SECURITY_OPEN:
                    ap.security = NSAPI_SECURITY_NONE;
                    break;
                case RTW_SECURITY_WEP_PSK:
                case RTW_SECURITY_WEP_SHARED:
                    ap.security = NSAPI_SECURITY_WEP;
                    break;
                case RTW_SECURITY_WPA_TKIP_PSK:
                case RTW_SECURITY_WPA_AES_PSK:
                    ap.security = NSAPI_SECURITY_WPA;
                    break;
                case RTW_SECURITY_WPA2_AES_PSK:
                case RTW_SECURITY_WPA2_TKIP_PSK:
                case RTW_SECURITY_WPA2_MIXED_PSK:
                    ap.security = NSAPI_SECURITY_WPA2;
                    break;
                case RTW_SECURITY_WPA_WPA2_MIXED:
                    ap.security = NSAPI_SECURITY_WPA_WPA2;
                    break;
                default:
                    ap.security = NSAPI_SECURITY_UNKNOWN;
                    break;
            }
            ap.rssi = record->signal_strength;
            ap.channel = record->channel;
            scan_handler->ap_details[scan_handler->ap_num] = WiFiAccessPoint(ap);
        }
        scan_handler->ap_num++;
    } else{
        // scan done
        rtw_up_sema(&scan_handler->scan_sema);
    }
    return RTW_SUCCESS;
}

RTWInterface::RTWInterface(bool debug)
    : _dhcp(true), _ip_address(), _netmask(), _gateway()
{
    emac_interface_t *emac;
    int ret;
    extern u32 GlobalDebugEnable; 

    GlobalDebugEnable = debug?1:0;
    emac = wlan_emac_init_interface();
    if (!emac) {
        printf("Error init RTWInterface!\r\n");
        return;
    }
    emac->ops.power_up(emac);
    ret = mbed_lwip_init(emac);
    if (ret != 0) {
        printf("Error init RTWInterface!(%d)\r\n", ret);
        return;
    }
}

RTWInterface::~RTWInterface()
{
    wlan_emac_link_change(false);
    mbed_lwip_bringdown();
}

nsapi_error_t RTWInterface::set_network(const char *ip_address, const char *netmask, const char *gateway)
{
    _dhcp = false;
    strncpy(_ip_address, ip_address ? ip_address : "", sizeof(_ip_address));
    strncpy(_netmask, netmask ? netmask : "", sizeof(_netmask));
    strncpy(_gateway, gateway ? gateway : "", sizeof(_gateway));
    return NSAPI_ERROR_OK;
}

nsapi_error_t RTWInterface::set_dhcp(bool dhcp)
{
    _dhcp = dhcp;
    return NSAPI_ERROR_OK;
}

/*
 * we may call connect multiple times
 */
nsapi_error_t RTWInterface::set_credentials(const char *ssid, const char *pass, nsapi_security_t security)
{
    strncpy(_ssid, ssid, 255);
    strncpy(_pass, pass, 255);
    _security = security;

    return NSAPI_ERROR_OK;
}

nsapi_error_t RTWInterface::connect()
{
    int ret;
    rtw_security_t sec;

    if (!_ssid || (!_pass && _security != NSAPI_SECURITY_NONE)) {
        printf("Invalid credentials\r\n");
        return NSAPI_ERROR_PARAMETER;
    }

    switch (_security) {
        case NSAPI_SECURITY_WPA:
        case NSAPI_SECURITY_WPA2:
        case NSAPI_SECURITY_WPA_WPA2:
            sec = RTW_SECURITY_WPA_WPA2_MIXED;
            break;
        case NSAPI_SECURITY_WEP:
            sec = RTW_SECURITY_WEP_PSK;
            break;
        case NSAPI_SECURITY_NONE:
            sec = RTW_SECURITY_OPEN;
            break;            
        default:
            return NSAPI_ERROR_PARAMETER;
    }

    if(_channel > 0 && _channel < 14){
        uint8_t pscan_config = PSCAN_ENABLE;
        wifi_set_pscan_chan(&_channel, &pscan_config, 1);
    }
    
    ret = wifi_connect(_ssid, sec, _pass, strlen(_ssid), strlen(_pass), 0, (void *)NULL);
    if (ret != RTW_SUCCESS) {
        printf("failed: %d\r\n", ret);
        return NSAPI_ERROR_NO_CONNECTION;
    }

    wlan_emac_link_change(true);
    return mbed_lwip_bringup(_dhcp,
                             _ip_address[0] ? _ip_address : 0,
                             _netmask[0] ? _netmask : 0,
                             _gateway[0] ? _gateway : 0);
}

nsapi_error_t RTWInterface::scan(WiFiAccessPoint *res, unsigned count)
{
    static wifi_scan_hdl scan_handler;
    scan_handler.ap_num = 0;
    if(!scan_handler.scan_sema)
        rtw_init_sema(&scan_handler.scan_sema, 0);
    scan_handler.scan_num = count;
    scan_handler.ap_details = res;
    if(wifi_scan_networks(scan_result_handler, (void *)&scan_handler) != RTW_SUCCESS){
        printf("wifi scan failed\n\r");
        return NSAPI_ERROR_DEVICE_ERROR;
    }
    if(rtw_down_timeout_sema( &scan_handler.scan_sema, MAX_SCAN_TIMEOUT ) == RTW_FALSE) {
        printf("wifi scan timeout\r\n");
        return NSAPI_ERROR_DEVICE_ERROR;
    }
    if(count <= 0 || count > scan_handler.ap_num)
        count = scan_handler.ap_num;

    return count;
}

nsapi_error_t RTWInterface::set_channel(uint8_t channel)
{
    _channel = channel;
    return NSAPI_ERROR_OK;
}

int8_t RTWInterface::get_rssi()
{
    int rssi = 0;
    if(wifi_get_rssi(&rssi) == 0)
        return (int8_t)rssi;
    return NSAPI_ERROR_OK;
}

nsapi_error_t RTWInterface::connect(const char *ssid, const char *pass,
                           nsapi_security_t security, uint8_t channel)
{
    set_credentials(ssid, pass, security);
    set_channel(channel);
    return connect();
}

nsapi_error_t RTWInterface::disconnect()
{
    char essid[33];

    wlan_emac_link_change(false);
    if(wifi_is_connected_to_ap() != RTW_SUCCESS)
        return NSAPI_ERROR_NO_CONNECTION;
    if(wifi_disconnect()<0){        
        return NSAPI_ERROR_DEVICE_ERROR;
    }
    while(1){
        if(wext_get_ssid(WLAN0_NAME, (unsigned char *) essid) < 0) {
            break;
        }
    }
    return NSAPI_ERROR_OK;
}

int RTWInterface::is_connected()
{
    // wifi_is_connected_to_ap return 0 on connected
    return !wifi_is_connected_to_ap();
}

const char *RTWInterface::get_mac_address()
{
    return mbed_lwip_get_mac_address();
}

const char *RTWInterface::get_ip_address()
{
    if (mbed_lwip_get_ip_address(_ip_address, sizeof _ip_address)) {
        return _ip_address;
    }
    return 0;
}

const char *RTWInterface::get_netmask()
{
    if (mbed_lwip_get_netmask(_netmask, sizeof _netmask)) {
        return _netmask;
    }
    return 0;
}

const char *RTWInterface::get_gateway()
{
    if (mbed_lwip_get_gateway(_gateway, sizeof _gateway)) {
        return _gateway;
    }
    return 0;
}

NetworkStack *RTWInterface::get_stack()
{
    return nsapi_create_stack(&lwip_stack);
}