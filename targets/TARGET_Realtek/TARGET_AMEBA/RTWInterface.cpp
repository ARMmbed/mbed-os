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

struct netif *xnetif[2];
static struct netif lwap_netif;
extern struct netif *netif_default;

RTWInterface::RTWInterface()
    : _dhcp(true), _ip_address(), _netmask(), _gateway()
{
	nsapi_error_t ret;
	ret = init();
	if (ret != NSAPI_ERROR_OK){
		printf("Error init RTWInterface!(%d)\r\n", ret);
	}
}

static void *scan_sema;
static const signed int maxApNum = 4;
static signed int ApNum;
static WiFiAccessPoint *SCANED_AP[maxApNum]; /*maximum store 15 APs*/

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

nsapi_error_t RTWInterface::init()
{
    emac_interface_t *emac;
    int ret;

    //printf("\r\nInitializing emac ...\r\n");
    emac = wlan_emac_init_interface();
    if (!emac) {
        return NSAPI_ERROR_DEVICE_ERROR;
    }
    emac->ops.power_up(emac);
    //printf("Initializing lwip ...\r\n");
    ret = mbed_lwip_init(emac);
    if (ret != 0) {
        return ret;
    }
    xnetif[0] = netif_default;
    return NSAPI_ERROR_OK;
}

nsapi_error_t RTWInterface::deinit()
{
    return mbed_lwip_bringdown();
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
	    case NSAPI_SECURITY_WPA2:
	        sec = RTW_SECURITY_WPA2_MIXED_PSK;
	        break;
	    case NSAPI_SECURITY_NONE:
	    default:
	        sec = RTW_SECURITY_OPEN;
	        break;
    }

    printf("Connecting to: %s ... \r\n", _ssid);
    ret = wifi_connect(_ssid, sec, _pass, strlen(_ssid), strlen(_pass), 0, (void *)NULL);
    if (ret != RTW_SUCCESS) {
        printf("failed: %d\r\n", ret);
        return NSAPI_ERROR_NO_CONNECTION;
    }
    //printf("connected\r\n");

    wlan_emac_link_up(0);
    return mbed_lwip_bringup(_dhcp,
                             _ip_address[0] ? _ip_address : 0,
                             _netmask[0] ? _netmask : 0,
                             _gateway[0] ? _gateway : 0);
}

static rtw_result_t scan_result_handler( rtw_scan_handler_result_t* malloced_scan_result )
{
	if (malloced_scan_result->scan_complete != RTW_TRUE) {
		rtw_scan_result_t* record = &malloced_scan_result->ap_details;
		record->SSID.val[record->SSID.len] = 0; /* Ensure the SSID is null terminated */
		if(ApNum>maxApNum)
			return RTW_SUCCESS; 
		nsapi_wifi_ap_t ap;
		
		memset((void*)&ap, 0x00, sizeof(nsapi_wifi_ap_t));
		memcpy(ap.ssid, record->SSID.val, record->SSID.len);
		memcpy(ap.bssid, record->BSSID.octet, 6);
		ap.security = (record->security == RTW_SECURITY_OPEN)?NSAPI_SECURITY_NONE :
						  (record->security == RTW_SECURITY_WEP_PSK)?NSAPI_SECURITY_WEP:
						  (record->security == RTW_SECURITY_WPA_TKIP_PSK || record->security == RTW_SECURITY_WPA_AES_PSK)? NSAPI_SECURITY_WPA:
						  (record->security == RTW_SECURITY_WPA2_AES_PSK || \
						   record->security == RTW_SECURITY_WPA2_TKIP_PSK || \
						   record->security == RTW_SECURITY_WPA2_MIXED_PSK)?NSAPI_SECURITY_WPA2:
						  (record->security == RTW_SECURITY_WPA_WPA2_MIXED)?NSAPI_SECURITY_WPA_WPA2:NSAPI_SECURITY_UNKNOWN;
		ap.rssi = record->signal_strength;
		ap.channel = record->channel;


		SCANED_AP[ApNum++] = new WiFiAccessPoint(ap);
		
	} else{
		// scan done

		rtw_up_sema(&scan_sema);
	}
	return RTW_SUCCESS;
}

nsapi_error_t RTWInterface::scan(WiFiAccessPoint *res, unsigned count)
{
	// blocked
	if(count == 0){
		ApNum = 0;
		
		rtw_init_sema(&scan_sema, 0);
		if(wifi_scan_networks(scan_result_handler, NULL) != RTW_SUCCESS){
			printf("wifi scan failed\n\r");
			//return NSAPI_ERROR_DEVICE_ERROR;
			goto error;
		}
		
		if(rtw_down_timeout_sema( &scan_sema, 15000 ) == RTW_FALSE) {
			printf("wifi scan timeout\r\n");
			//return NSAPI_ERROR_DEVICE_ERROR;
			goto error;
		} 
		rtw_free_sema(&scan_sema);
		return ApNum;

	}else if(count > 0 && res != NULL){
		count = count < maxApNum ? count : maxApNum;
		for(int i = 0; i < count; i++){
			memcpy(&res[i], SCANED_AP[i], sizeof(WiFiAccessPoint));
			delete[] SCANED_AP[i];
 		}
		return (signed int)count;
	}
    return NSAPI_ERROR_OK;
error:
	rtw_free_sema(&scan_sema);
	return NSAPI_ERROR_DEVICE_ERROR;
}

nsapi_error_t RTWInterface::set_channel(uint8_t channel)
{
	if(wifi_set_channel(channel) == 0)
		return NSAPI_ERROR_OK;
    return NSAPI_ERROR_DEVICE_ERROR;
}

int8_t RTWInterface::get_rssi()
{
    return NSAPI_ERROR_UNSUPPORTED;
}

nsapi_error_t RTWInterface::connect(const char *ssid, const char *pass,
                           nsapi_security_t security, uint8_t channel)
{
    set_credentials(ssid, pass, security);
    return connect();
}

nsapi_error_t RTWInterface::disconnect()
{
	char essid[33];

    if(wifi_is_connected_to_ap() != RTW_SUCCESS)
		return NSAPI_ERROR_NO_CONNECTION;
	printf("Deassociating AP ...\r\n");
	if(wifi_disconnect()<0){		
		return NSAPI_ERROR_DEVICE_ERROR;
	}

	while(1){
		if(wext_get_ssid(WLAN0_NAME, (unsigned char *) essid) < 0) {
			printf("WIFI disconnected\n\r");
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
