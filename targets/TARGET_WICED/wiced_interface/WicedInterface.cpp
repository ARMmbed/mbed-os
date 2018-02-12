/* Wiced implementation of NetworkInterfaceAPI
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

#include "WicedInterface.h"

#include "EthernetInterface.h"
#include "nsapi.h"
#include "lwipopts.h"

#include "wiced_wifi.h"
#include "wiced_management.h"
#include "wwd_network_constants.h"
#include "wwd_buffer_interface.h"

#include "lwip/etharp.h"
#include "lwip/ethip6.h"


static int wiced_toerror(wiced_result_t res) {
    return -res;
}

static nsapi_security_t wiced_tosecurity(wiced_security_t sec) {
    switch (sec & (WEP_ENABLED | WPA_SECURITY | WPA2_SECURITY)) {
        case WEP_ENABLED:   return NSAPI_SECURITY_WEP;
        case WPA_SECURITY:  return NSAPI_SECURITY_WPA;
        case WPA2_SECURITY: return NSAPI_SECURITY_WPA2;
        default:
            if (sec == WICED_SECURITY_OPEN) return NSAPI_SECURITY_NONE;
            else                            return NSAPI_SECURITY_UNKNOWN;
    }
}

static wiced_security_t wiced_fromsecurity(nsapi_security_t sec) {
    switch (sec) {
        case NSAPI_SECURITY_NONE:       return WICED_SECURITY_OPEN;
        case NSAPI_SECURITY_WEP:        return WICED_SECURITY_WEP_PSK;
        case NSAPI_SECURITY_WPA:        return WICED_SECURITY_WPA_MIXED_PSK;
        case NSAPI_SECURITY_WPA2:       return WICED_SECURITY_WPA2_MIXED_PSK;
        case NSAPI_SECURITY_WPA_WPA2:   return WICED_SECURITY_WPA2_MIXED_PSK;
        default:                        return WICED_SECURITY_UNKNOWN;
    }
}

int WicedInterface::connect(
        const char *ssid, const char *pass,
        nsapi_security_t security,
        uint8_t channel)
{
    int err = set_channel(channel);
    if (err) {
        return err;
    }

    err = set_credentials(ssid, pass, security);
    if (err) {
        return err;
    }

    return connect();
}

int WicedInterface::set_credentials(const char *ssid, const char *pass, nsapi_security_t security)
{
    if ((ssid == NULL) ||
        (pass == NULL && security != NSAPI_SECURITY_NONE) ||
        (strlen(pass) > 63 && (security == NSAPI_SECURITY_WPA2 || security == NSAPI_SECURITY_WPA || security == NSAPI_SECURITY_WPA_WPA2)))
    {
        return NSAPI_ERROR_PARAMETER;
    }
    _ssid = ssid;
    _pass = pass;
    _security = security;
    return NSAPI_ERROR_OK;
}

int WicedInterface::set_network(const char *ip_address, const char *netmask, const char *gateway)
{
    return _iface.set_network(ip_address, netmask, gateway);
}

int WicedInterface::set_dhcp(bool dhcp)
{
    return _iface.set_dhcp(dhcp);
}

struct wiced_scan_security_userdata {
    Semaphore sema;
    const char *ssid;
    int ssidlen;
    wiced_security_t security;
};

static wiced_result_t wiced_scan_security_handler(
        wiced_scan_handler_result_t *result)
{
    wiced_scan_security_userdata *data =
            (wiced_scan_security_userdata*)result->user_data;
    malloc_transfer_to_curr_thread(result);

    // finished scan, either succesfully or through an abort
    if (result->status != WICED_SCAN_INCOMPLETE) {
        data->sema.release();
        free(result);
        return WICED_SUCCESS;
    }

    if (data->ssidlen == result->ap_details.SSID.length &&
        memcmp(data->ssid, result->ap_details.SSID.value, data->ssidlen) == 0) {
        // found a match
        data->security = result->ap_details.security;
        wwd_wifi_abort_scan();
    }

    // release result
    free(result);
    return WICED_SUCCESS;
}

int WicedInterface::connect()
{
    // initialize wiced, this is noop if already init
    wiced_result_t res = wiced_init();
    if (res != WICED_SUCCESS) {
        return wiced_toerror(res);
    }

    // setup ssid
    wiced_ssid_t ssid;
    strncpy((char*)ssid.value, _ssid, SSID_NAME_SIZE);
    ssid.value[SSID_NAME_SIZE-1] = '\0';
    ssid.length = strlen((char*)ssid.value);

    // choose network security
    wiced_security_t security = wiced_fromsecurity(_security);
    if (security == WICED_SECURITY_OPEN) {
        // none actually indicates we need to find out the security ourselves
        // if ssid isn't being broadcasted we just continue with security none
        wiced_scan_security_userdata data;
        data.ssid = _ssid;
        data.ssidlen = strlen(_ssid);
        data.security = WICED_SECURITY_OPEN;

        res = wiced_wifi_scan_networks(wiced_scan_security_handler, &data);
        if (res != WICED_SUCCESS) {
            return wiced_toerror(res);
        }

        int tok = data.sema.wait();
        if (tok < 1) {
            return NSAPI_ERROR_WOULD_BLOCK;
        }

        security = data.security;
    }

    // join the network
    res = (wiced_result_t)wwd_wifi_join(
            &ssid,
            security,
            (const uint8_t *)_pass, strlen(_pass),
            NULL,
            WWD_STA_INTERFACE);
    if (res != WICED_SUCCESS) {
        return wiced_toerror(res);
    }

    // bring up lwip
    return _iface.connect();
}

int WicedInterface::disconnect()
{
    // bring down lwip
    int err = _iface.disconnect();
    if (err) {
        return err;
    }

    // leave network
    wiced_result_t res = (wiced_result_t)wwd_wifi_leave(WWD_STA_INTERFACE);
    if (res != WICED_SUCCESS) {
        return wiced_toerror(res);
    }

    return 0;
}

const char *WicedInterface::get_mac_address()
{
    return _iface.get_mac_address();
}

const char *WicedInterface::get_ip_address()
{
    return _iface.get_ip_address();
}

const char *WicedInterface::get_gateway()
{
    return _iface.get_gateway();
}

const char *WicedInterface::get_netmask()
{
    return _iface.get_netmask();
}

int8_t WicedInterface::get_rssi()
{
    int32_t rssi;
    wiced_result_t res = (wiced_result_t)wwd_wifi_get_rssi(&rssi);
    if (res != 0) {
        return 0;
    }

    return (int8_t)rssi;
}

struct wiced_scan_userdata {
    Semaphore sema;
    WiFiAccessPoint *aps;
    unsigned count;
    unsigned offset;
};

static wiced_result_t wiced_scan_count_handler(
        wiced_scan_handler_result_t *result)
{
    wiced_scan_userdata *data = (wiced_scan_userdata*)result->user_data;
    malloc_transfer_to_curr_thread(result);

    // finished scan, either succesfully or through an abort
    if (result->status != WICED_SCAN_INCOMPLETE) {
        data->sema.release();
        free(result);
        return WICED_SUCCESS;
    }

    // just count the available networks
    data->offset += 1;

    // release result
    free(result);
    return WICED_SUCCESS;
}

static wiced_result_t wiced_scan_handler(
        wiced_scan_handler_result_t *result)
{
    wiced_scan_userdata *data = (wiced_scan_userdata*)result->user_data;
    malloc_transfer_to_curr_thread(result);

    // finished scan, either succesfully or through an abort
    if (result->status != WICED_SCAN_INCOMPLETE) {
        data->sema.release();
        free(result);
        return WICED_SUCCESS;
    }

    // can't really keep anymore scan results
    if (data->offset == data->count) {
        wwd_wifi_abort_scan();
        free(result);
        return WICED_SUCCESS;
    }

    // get ap stats
    nsapi_wifi_ap ap;

    uint8_t length = result->ap_details.SSID.length;
    if (length < sizeof(ap.ssid)-1) {
        length = sizeof(ap.ssid)-1;
    }
    memcpy(ap.ssid, result->ap_details.SSID.value, length);
    ap.ssid[length] = '\0';

    memcpy(ap.bssid, result->ap_details.BSSID.octet, sizeof(ap.bssid));

    ap.security = wiced_tosecurity(result->ap_details.security);
    ap.rssi = result->ap_details.signal_strength;
    ap.channel = result->ap_details.channel;

    // store as ap object
    data->aps[data->offset] = WiFiAccessPoint(ap);
    data->offset += 1;

    // release result
    free(result);
    return WICED_SUCCESS;
}

int WicedInterface::scan(WiFiAccessPoint *aps, unsigned count)
{
    // initialize wiced, this is noop if already init
    wiced_result_t res = wiced_init();
    if (res != WICED_SUCCESS) {
        return wiced_toerror(res);
    }

    wiced_scan_userdata data;
    data.aps = aps;
    data.count = count;
    data.offset = 0;

    // either count available ap or actually scan based on count argument
    wiced_scan_result_handler_t handler = (count == 0)
        ? wiced_scan_count_handler : wiced_scan_handler;

    res = wiced_wifi_scan_networks(handler, &data);
    if (res != WICED_SUCCESS) {
        return wiced_toerror(res);
    }

    int tok = data.sema.wait();
    if (tok < 1) {
        return NSAPI_ERROR_WOULD_BLOCK;
    }

    return data.offset;
}

NetworkStack *WicedInterface::get_stack()
{
    return _iface.get_stack();
}
