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
#include "EMAC.h"
#include "wifi_constants.h"
#include "wifi_conf.h"

#include "OnboardNetworkStack.h"
#include "EMACMemoryManager.h"
#include "osdep_service.h"

typedef struct _wifi_scan_hdl {
    _sema scan_sema;
    nsapi_size_t ap_num;
    nsapi_size_t scan_num;
    WiFiAccessPoint *ap_details;
} wifi_scan_hdl;

#define MAX_SCAN_TIMEOUT (15000)

static bool _inited = false;

static rtw_result_t scan_result_handler( rtw_scan_handler_result_t* malloced_scan_result )
{
    wifi_scan_hdl *scan_handler = (wifi_scan_hdl *)malloced_scan_result->user_data;
    if (malloced_scan_result->scan_complete != RTW_TRUE) {
        if (scan_handler->ap_details && scan_handler->scan_num > scan_handler->ap_num) {
            nsapi_wifi_ap_t ap;
            rtw_scan_result_t* record = &malloced_scan_result->ap_details;
            record->SSID.val[record->SSID.len] = 0; /* Ensure the SSID is null terminated */
            memset((void*)&ap, 0x00, sizeof(nsapi_wifi_ap_t));
            memcpy(ap.ssid, record->SSID.val, record->SSID.len);
            memcpy(ap.bssid, record->BSSID.octet, 6);
            switch (record->security) {
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
    } else {
        // scan done
        rtw_up_sema(&scan_handler->scan_sema);
    }
    return RTW_SUCCESS;
}

RTWInterface::RTWInterface(RTW_EMAC &get_rtw_emac, OnboardNetworkStack &get_rtw_obn_stack) :
        rtw_emac(get_rtw_emac),
        rtw_obn_stack(get_rtw_obn_stack),
        rtw_interface(NULL),
        _dhcp(true), 
        _ip_address(), 
        _netmask(), 
        _gateway(),
        _mac_address()
{
    rtw_emac.power_up();
}

RTWInterface::~RTWInterface()
{
    rtw_emac.wlan_emac_link_change(false);
    rtw_interface->bringdown();
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
    if (!ssid) {
        return NSAPI_ERROR_PARAMETER;
    }

    switch (security) {
        case NSAPI_SECURITY_WPA:
        case NSAPI_SECURITY_WPA2:
        case NSAPI_SECURITY_WPA_WPA2:
        case NSAPI_SECURITY_WEP:
            if ((strlen(pass) < 8) || (strlen(pass) > 63)) { // 802.11 password 8-63 characters
                return NSAPI_ERROR_PARAMETER;
            }
            break;
        case NSAPI_SECURITY_NONE:
            break;
        default:
            return NSAPI_ERROR_PARAMETER;
    }

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

    if (_channel > 0 && _channel < 14) {
        uint8_t pscan_config = PSCAN_ENABLE;
        wifi_set_pscan_chan(&_channel, &pscan_config, 1);
    }

    ret = wifi_connect(_ssid, sec, _pass, strlen(_ssid), strlen(_pass), 0, (void *)NULL);
    if (ret != RTW_SUCCESS) {
        printf("failed: %d\r\n", ret);
        return NSAPI_ERROR_NO_CONNECTION;
    }

    rtw_emac.wlan_emac_link_change(true);
    if (!rtw_interface) {
        nsapi_error_t err = rtw_obn_stack.add_ethernet_interface(rtw_emac, true, &rtw_interface);
        if (err != NSAPI_ERROR_OK) {
            rtw_interface = NULL;
            return err;
        }
    }

    int rtw_if_bringup = rtw_interface->bringup(_dhcp,
                           _ip_address[0] ? _ip_address : 0,
                           _netmask[0] ? _netmask : 0,
                           _gateway[0] ? _gateway : 0,
                           DEFAULT_STACK);
    return rtw_if_bringup;
}

nsapi_error_t RTWInterface::scan(WiFiAccessPoint *res, unsigned count)
{
    static wifi_scan_hdl scan_handler;
    scan_handler.ap_num = 0;
    if (!scan_handler.scan_sema) {
        rtw_init_sema(&scan_handler.scan_sema, 0);
    }
    scan_handler.scan_num = count;
    scan_handler.ap_details = res;
    if (wifi_scan_networks(scan_result_handler, (void *)&scan_handler) != RTW_SUCCESS) {
        printf("wifi scan failed\n\r");
        return NSAPI_ERROR_DEVICE_ERROR;
    }
    if (rtw_down_timeout_sema( &scan_handler.scan_sema, MAX_SCAN_TIMEOUT ) == RTW_FALSE) {
        printf("wifi scan timeout\r\n");
        return NSAPI_ERROR_DEVICE_ERROR;
    }
    if (count <= 0 || count > scan_handler.ap_num) {
        count = scan_handler.ap_num;
    }
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
    if (wifi_get_rssi(&rssi) == 0) {
        return (int8_t)rssi;
    }  
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

    rtw_emac.wlan_emac_link_change(false);
    rtw_interface->bringdown();
    if (wifi_is_connected_to_ap() != RTW_SUCCESS) {
        return NSAPI_ERROR_NO_CONNECTION;
    }
    if (wifi_disconnect()<0) {
        return NSAPI_ERROR_DEVICE_ERROR;
    }
    while(1) {
        if (wext_get_ssid(WLAN0_NAME, (unsigned char *) essid) < 0) {
            break;
        }
    }
    return NSAPI_ERROR_OK;
}

int RTWInterface::is_connected()
{
    return !wifi_is_connected_to_ap();
}

const char *RTWInterface::get_mac_address()
{
    if (rtw_interface->get_mac_address(_mac_address, sizeof _mac_address)) {
        return _mac_address;
    }
    return 0;
}

const char *RTWInterface::get_ip_address()
{
    if (rtw_interface->get_ip_address(_ip_address, sizeof _ip_address)) {
        return _ip_address;
    }
    return 0;
}

const char *RTWInterface::get_netmask()
{
    if (rtw_interface->get_netmask(_netmask, sizeof _netmask)) {
        return _netmask;
    }
    return 0;
}

const char *RTWInterface::get_gateway()
{
    if (rtw_interface->get_gateway(_gateway, sizeof _gateway)) {
        return _gateway;
    }
    return 0;
}

NetworkStack *RTWInterface::get_stack()
{
    return &rtw_obn_stack;
}
