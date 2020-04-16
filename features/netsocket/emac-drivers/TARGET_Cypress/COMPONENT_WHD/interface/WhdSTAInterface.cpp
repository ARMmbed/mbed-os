/* WHD STAION implementation of NetworkInterfaceAPI
 * Copyright (c) 2017-2019 ARM Limited
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

#include <cstring>
#include <algorithm>
#include <vector>
#include "WhdSTAInterface.h"
#include "nsapi.h"
#include "lwipopts.h"
#include "lwip/etharp.h"
#include "lwip/ethip6.h"
#include "rtos.h"
#include "whd_emac.h"
#include "whd_wifi_api.h"
#include "whd_wlioctl.h"

#define CMP_MAC( a, b )  (((((unsigned char*)a)[0])==(((unsigned char*)b)[0]))&& \
                          ((((unsigned char*)a)[1])==(((unsigned char*)b)[1]))&& \
                          ((((unsigned char*)a)[2])==(((unsigned char*)b)[2]))&& \
                          ((((unsigned char*)a)[3])==(((unsigned char*)b)[3]))&& \
                          ((((unsigned char*)a)[4])==(((unsigned char*)b)[4]))&& \
                          ((((unsigned char*)a)[5])==(((unsigned char*)b)[5])))

struct whd_scan_userdata {
    rtos::Semaphore *sema;
    scan_result_type sres_type;
    WiFiAccessPoint *aps;
    std::vector<whd_scan_result_t> *result_buff;
    unsigned count;
    unsigned offset;
    bool scan_in_progress;
};

static whd_scan_userdata interal_scan_data;
static whd_scan_result_t internal_scan_result;
static uint16_t sta_link_update_entry = 0xFF;
static const whd_event_num_t sta_link_change_events[] = {
    WLC_E_SET_SSID, WLC_E_LINK, WLC_E_AUTH,  WLC_E_ASSOC, WLC_E_DEAUTH_IND, WLC_E_DISASSOC_IND, WLC_E_DISASSOC,
    WLC_E_REASSOC, WLC_E_PSK_SUP, WLC_E_ACTION_FRAME_COMPLETE, WLC_E_NONE
};


extern "C" void whd_emac_wifi_link_state_changed(whd_interface_t ifp, whd_bool_t state_up);


int whd_toerror(whd_result_t res)
{
    switch (res) {
        case WHD_SUCCESS:
            return NSAPI_ERROR_OK;
        case WHD_UNSUPPORTED:
        case WHD_WLAN_UNSUPPORTED:
        case WHD_WLAN_ACM_NOTSUPPORTED:
            return NSAPI_ERROR_UNSUPPORTED;
        case WHD_BADARG:
        case WHD_WLAN_BADARG:
            return NSAPI_ERROR_PARAMETER;
        case WHD_WLAN_NOTASSOCIATED:
        case WHD_INVALID_JOIN_STATUS:
            return NSAPI_ERROR_NO_CONNECTION;
        case WHD_BUFFER_UNAVAILABLE_PERMANENT:
        case WHD_BUFFER_UNAVAILABLE_TEMPORARY:
        case WHD_RX_BUFFER_ALLOC_FAIL:
        case WHD_BUFFER_ALLOC_FAIL:
        case WHD_WLAN_NOMEM:
        case WHD_MALLOC_FAILURE:
            return NSAPI_ERROR_NO_MEMORY;
        case WHD_ACCESS_POINT_NOT_FOUND:
        case WHD_NETWORK_NOT_FOUND:
            return NSAPI_ERROR_NO_SSID;
        case WHD_NOT_AUTHENTICATED:
        case WHD_INVALID_KEY:
        case WHD_NOT_KEYED:
            return NSAPI_ERROR_AUTH_FAILURE;
        case WHD_PENDING:
        case WHD_JOIN_IN_PROGRESS:
            return NSAPI_ERROR_IN_PROGRESS;
        case WHD_CONNECTION_LOST:
            return NSAPI_ERROR_CONNECTION_LOST;
        case WHD_TIMEOUT:
        case WHD_EAPOL_KEY_PACKET_M1_TIMEOUT:
        case WHD_EAPOL_KEY_PACKET_M3_TIMEOUT:
        case WHD_EAPOL_KEY_PACKET_G1_TIMEOUT:
            return NSAPI_ERROR_CONNECTION_TIMEOUT;
        case WHD_WLAN_BUSY:
            return NSAPI_ERROR_BUSY;
        case WHD_WLAN_NODEVICE:
            return NSAPI_ERROR_DEVICE_ERROR;
        default:
            return -res;
    }
}

static nsapi_security_t whd_tosecurity(whd_security_t sec)
{
    switch (sec) {
        case WHD_SECURITY_OPEN:
            return NSAPI_SECURITY_NONE;
        case WHD_SECURITY_WEP_PSK:
        case WHD_SECURITY_WEP_SHARED:
            return NSAPI_SECURITY_WEP;
        case WHD_SECURITY_WPA_TKIP_PSK:
        case WHD_SECURITY_WPA_TKIP_ENT:
            return NSAPI_SECURITY_WPA;
        case WHD_SECURITY_WPA2_MIXED_PSK:
            return NSAPI_SECURITY_WPA_WPA2;
        case WHD_SECURITY_WPA2_MIXED_ENT:
            return NSAPI_SECURITY_WPA2_ENT;
        case WHD_SECURITY_WPA2_AES_PSK:
        case WHD_SECURITY_WPA2_AES_ENT:
        case WHD_SECURITY_WPA2_FBT_PSK:
        case WHD_SECURITY_WPA2_FBT_ENT:
            return NSAPI_SECURITY_WPA2;
        case WHD_SECURITY_WPA3_SAE:
            return NSAPI_SECURITY_WPA3;
        case WHD_SECURITY_WPA3_WPA2_PSK:
            return NSAPI_SECURITY_WPA3_WPA2;
        default:
            return NSAPI_SECURITY_UNKNOWN;
    }
}

whd_security_t whd_fromsecurity(nsapi_security_t sec)
{
    switch (sec) {
        case NSAPI_SECURITY_NONE:
            return WHD_SECURITY_OPEN;
        case NSAPI_SECURITY_WEP:
            return WHD_SECURITY_WEP_PSK;
        case NSAPI_SECURITY_WPA:
            return WHD_SECURITY_WPA_MIXED_PSK;
        case NSAPI_SECURITY_WPA2:
            return WHD_SECURITY_WPA2_AES_PSK;
        case NSAPI_SECURITY_WPA_WPA2:
            return WHD_SECURITY_WPA2_MIXED_PSK;
        case NSAPI_SECURITY_WPA2_ENT:
            return WHD_SECURITY_WPA2_MIXED_ENT;
        case NSAPI_SECURITY_WPA3:
            return WHD_SECURITY_WPA3_SAE;
        case NSAPI_SECURITY_WPA3_WPA2:
            return WHD_SECURITY_WPA3_WPA2_PSK;
        default:
            return WHD_SECURITY_UNKNOWN;
    }
}

static void *whd_wifi_link_state_change_handler(whd_interface_t ifp,
                                                const whd_event_header_t *event_header,
                                                const uint8_t *event_data,
                                                void *handler_user_data)
{
    UNUSED_PARAMETER(event_data);

    if (event_header->bsscfgidx >= WHD_INTERFACE_MAX) {
        WPRINT_WHD_DEBUG(("%s: event_header: Bad interface\n", __FUNCTION__));
        return NULL;
    }

    if ((event_header->event_type == WLC_E_DEAUTH_IND) ||
            (event_header->event_type == WLC_E_DISASSOC_IND) ||
            ((event_header->event_type == WLC_E_PSK_SUP) &&
            (event_header->status == WLC_SUP_KEYED) &&
            (event_header->reason == WLC_E_SUP_DEAUTH))) {
        whd_emac_wifi_link_state_changed(ifp, WHD_FALSE);
        return handler_user_data;
    }

    if (((event_header->event_type == WLC_E_PSK_SUP) &&
            (event_header->status == WLC_SUP_KEYED) &&
            (event_header->reason == WLC_E_SUP_OTHER)) ||
            (whd_wifi_is_ready_to_transceive(ifp) == WHD_SUCCESS)) {
        whd_emac_wifi_link_state_changed(ifp, WHD_TRUE);
        return handler_user_data;
    }

    return handler_user_data;
}


MBED_WEAK WhdSTAInterface::OlmInterface &WhdSTAInterface::OlmInterface::get_default_instance()
{
    static OlmInterface olm;
    return olm;
}

WhdSTAInterface::WhdSTAInterface(WHD_EMAC &emac, OnboardNetworkStack &stack, OlmInterface &olm, whd_interface_shared_info_t &shared)
    : EMACInterface(emac, stack),
      _ssid("\0"),
      _pass("\0"),
      _security(NSAPI_SECURITY_NONE),
      _whd_emac(emac),
      _olm(&olm),
      _iface_shared(shared)
{
}

nsapi_error_t WhdSTAInterface::connect(
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

nsapi_error_t WhdSTAInterface::set_credentials(const char *ssid, const char *pass, nsapi_security_t security)
{
    if ((ssid == NULL) ||
            (strlen(ssid) == 0) ||
            (pass == NULL && (security != NSAPI_SECURITY_NONE && security != NSAPI_SECURITY_WPA2_ENT)) ||
            (strlen(pass) == 0 && (security != NSAPI_SECURITY_NONE && security != NSAPI_SECURITY_WPA2_ENT)) ||
            (strlen(pass) > 63 && (security == NSAPI_SECURITY_WPA2 || security == NSAPI_SECURITY_WPA ||
            security == NSAPI_SECURITY_WPA_WPA2 || security == NSAPI_SECURITY_WPA3 || security == NSAPI_SECURITY_WPA3_WPA2))
       ) {
        return NSAPI_ERROR_PARAMETER;
    }

    memset(_ssid, 0, sizeof(_ssid));
    strncpy(_ssid, ssid, sizeof(_ssid));

    memset(_pass, 0, sizeof(_pass));
    strncpy(_pass, pass, sizeof(_pass));

    _security = security;

    return NSAPI_ERROR_OK;
}

nsapi_error_t WhdSTAInterface::connect()
{
    ScopedMutexLock lock(_iface_shared.mutex);

#define MAX_RETRY_COUNT    ( 5 )
    int i;
    whd_result_t res;

    // initialize wiced, this is noop if already init
    if (!_whd_emac.powered_up) {
        if(!_whd_emac.power_up()) {
            return NSAPI_ERROR_DEVICE_ERROR;
        }
    }

    res = whd_management_set_event_handler(_whd_emac.ifp, sta_link_change_events,
                                           whd_wifi_link_state_change_handler, NULL, &sta_link_update_entry);
    if (res != WHD_SUCCESS) {
        return whd_toerror(res);
    }

    _iface_shared.if_status_flags |= IF_STATUS_STA_UP;
    _iface_shared.default_if_cfg = DEFAULT_IF_STA;
    if (!_interface) {
        nsapi_error_t err = _stack.add_ethernet_interface(_emac, true, &_interface);
        if (err != NSAPI_ERROR_OK) {
            _interface = NULL;
            return err;
        }
        _interface->attach(_connection_status_cb);
        _iface_shared.iface_sta = _interface;
    } else {
        _stack.set_default_interface(_interface);
    }

    // Initialize the Offload Manager
    if (_olm != NULL) {
        _olm->init_ols(_whd_emac.ifp, this);
    }

    if ((_ssid == NULL) ||
            (strlen(_ssid) == 0)) {
        return NSAPI_ERROR_PARAMETER;
    }

    // setup ssid
    whd_ssid_t ssid;
    strncpy((char *)ssid.value, _ssid, SSID_NAME_SIZE);
    ssid.value[SSID_NAME_SIZE - 1] = '\0';
    ssid.length = strlen((char *)ssid.value);

    // choose network security
    whd_security_t security = whd_fromsecurity(_security);

#if defined MBED_CONF_APP_WIFI_PASSWORD_WPA2PSK
    /* Set PSK password for WPA3_WPA2 */
    if (security == WHD_SECURITY_WPA3_WPA2_PSK) {
        res = (whd_result_t)whd_wifi_enable_sup_set_passphrase( _whd_emac.ifp, (const uint8_t *)MBED_CONF_APP_WIFI_PASSWORD_WPA2PSK,
                  strlen(MBED_CONF_APP_WIFI_PASSWORD_WPA2PSK), WHD_SECURITY_WPA3_WPA2_PSK );
    }
#else
    /* Set PSK password for WPA3_WPA2 */
    if (security == WHD_SECURITY_WPA3_WPA2_PSK) {
        res = (whd_result_t)whd_wifi_enable_sup_set_passphrase( _whd_emac.ifp, (const uint8_t *)_pass,
                  strlen(_pass), WHD_SECURITY_WPA3_WPA2_PSK );
    }
#endif
    // join the network
    for (i = 0; i < MAX_RETRY_COUNT; i++) {
        res = (whd_result_t)whd_wifi_join(_whd_emac.ifp,
                                          &ssid,
                                          security,
                                          (const uint8_t *)_pass, strlen(_pass));
        if (res == WHD_SUCCESS) {
            break;
        }
    }

    if (res != WHD_SUCCESS) {
        return whd_toerror(res);
    }

    if (whd_wifi_is_ready_to_transceive(_whd_emac.ifp) == WHD_SUCCESS) {
        whd_emac_wifi_link_state_changed(_whd_emac.ifp, WHD_TRUE);
    }

    // bring up
    return _interface->bringup(_dhcp,
                               _ip_address[0] ? _ip_address : 0,
                               _netmask[0] ? _netmask : 0,
                               _gateway[0] ? _gateway : 0,
                               DEFAULT_STACK);
}

void WhdSTAInterface::wifi_on()
{
    if (!_whd_emac.powered_up) {
        if(!_whd_emac.power_up()) {
            CY_ASSERT(false);
        }
    }
}

nsapi_error_t WhdSTAInterface::disconnect()
{
    ScopedMutexLock lock(_iface_shared.mutex);

    if (!_interface) {
        return NSAPI_STATUS_DISCONNECTED;
    }

    // bring down
    int err = _interface->bringdown();
    if (err) {
        return err;
    }

    _iface_shared.if_status_flags &= ~IF_STATUS_STA_UP;
    if (_iface_shared.if_status_flags & IF_STATUS_SOFT_AP_UP) {
        _iface_shared.default_if_cfg = DEFAULT_IF_SOFT_AP;
        _stack.set_default_interface(_iface_shared.iface_softap);
    } else {
        _iface_shared.default_if_cfg = DEFAULT_IF_NOT_SET;
    }

    // leave network
    whd_result_t res = whd_wifi_leave(_whd_emac.ifp);
    if (res != WHD_SUCCESS) {
        return whd_toerror(res);
    }
    whd_emac_wifi_link_state_changed(_whd_emac.ifp, WHD_FALSE);

    // remove the interface added in connect
    if (_interface) {
        nsapi_error_t err = _stack.remove_ethernet_interface(&_interface);
        if (err != NSAPI_ERROR_OK) {
            return err;
        }
        _iface_shared.iface_sta = NULL;
    }

    res = whd_wifi_deregister_event_handler(_whd_emac.ifp, sta_link_update_entry);
    if (res != WHD_SUCCESS) {
        return whd_toerror(res);
    }

    // de-init Offload Manager
    if (_olm != NULL) {
        _olm->deinit_ols();
    }

    return NSAPI_ERROR_OK;
}

int8_t WhdSTAInterface::get_rssi()
{
    int32_t rssi;
    whd_result_t res;

    if (!_whd_emac.powered_up) {
        if(!_whd_emac.power_up()) {
            CY_ASSERT(false);
        }
    }

    res = (whd_result_t)whd_wifi_get_rssi(_whd_emac.ifp, &rssi);
    if (res != 0) {
        /* The network GT tests expect that this function should return 0 in case of an error and not assert */
        return 0;
    }

    return (int8_t)rssi;
}

static void whd_scan_handler(whd_scan_result_t **result_ptr,
                             void *user_data, whd_scan_status_t status)
{

    whd_scan_userdata *data = (whd_scan_userdata *)user_data;

    /* Even after stopping scan, some results will still come as results are already present in the queue */
    if (data->scan_in_progress == false) {
        return;
    }

    // finished scan, either succesfully or through an abort
    if (status != WHD_SCAN_INCOMPLETE) {
        data->scan_in_progress = false;
        data->sema->release();
        return;
    }

    // can't really keep anymore scan results
    if (data->count > 0 && data->offset >= data->count) {
        /* We can not abort the scan as this function is getting executed in WHD context,
           Note that to call any WHD API, caller function should not in WHD context */
        return;
    }

    whd_scan_result_t *record = *result_ptr;

    for (unsigned int i = 0; i < data->result_buff->size(); i++) {
        if (CMP_MAC((*data->result_buff)[i].BSSID.octet, record->BSSID.octet)) {
            return;
        }
    }

    if (data->count > 0 && data->aps != NULL) {
        // get ap stats
        nsapi_wifi_ap ap;
        uint8_t length = record->SSID.length;
        if (length < sizeof(ap.ssid) - 1) {
            length = sizeof(ap.ssid) - 1;
        }
        memcpy(ap.ssid, record->SSID.value, length);
        ap.ssid[length] = '\0';

        memcpy(ap.bssid, record->BSSID.octet, sizeof(ap.bssid));

        ap.security = whd_tosecurity(record->security);
        ap.rssi = record->signal_strength;
        ap.channel = record->channel;
        if (data->sres_type == SRES_TYPE_WIFI_ACCESS_POINT) {
            data->aps[data->offset] = WiFiAccessPoint(ap);
        } else if (data->sres_type == SRES_TYPE_WHD_ACCESS_POINT) {
            WhdAccessPoint *aps_sres = static_cast<WhdAccessPoint *>(data->aps);
            aps_sres[data->offset] = std::move(WhdAccessPoint(ap, record->bss_type,
                                                              record->ie_ptr, record->ie_len));
        }
    }

    // store to result_buff for future duplication removal
    data->result_buff->push_back(*record);
    data->offset = data->result_buff->size();
}

int WhdSTAInterface::internal_scan(WiFiAccessPoint *aps, unsigned count, scan_result_type sres_type)
{
    ScopedMutexLock lock(_iface_shared.mutex);

    // initialize wiced, this is noop if already init
    if (!_whd_emac.powered_up) {
        if(!_whd_emac.power_up()) {
            return NSAPI_ERROR_DEVICE_ERROR;
        }
    }

    interal_scan_data.sema = new Semaphore();
    interal_scan_data.sres_type = sres_type;
    interal_scan_data.aps = aps;
    interal_scan_data.count = count;
    interal_scan_data.offset = 0;
    interal_scan_data.scan_in_progress = true;
    interal_scan_data.result_buff = new std::vector<whd_scan_result_t>();
    whd_result_t whd_res;
    int res;

    whd_res = (whd_result_t)whd_wifi_scan(_whd_emac.ifp, WHD_SCAN_TYPE_ACTIVE, WHD_BSS_TYPE_ANY,
                                          NULL, NULL, NULL, NULL, whd_scan_handler, &internal_scan_result, &interal_scan_data);
    if (whd_res != WHD_SUCCESS) {
        res = whd_toerror(whd_res);
    } else {
        interal_scan_data.sema->acquire();
        res = interal_scan_data.offset;
    }

    delete interal_scan_data.sema;
    delete interal_scan_data.result_buff;
    return res;
}

int WhdSTAInterface::scan(WiFiAccessPoint *aps, unsigned count)
{
    return internal_scan(aps, count, SRES_TYPE_WIFI_ACCESS_POINT);
}

int WhdSTAInterface::scan_whd(WhdAccessPoint *aps, unsigned count)
{
    return internal_scan(aps, count, SRES_TYPE_WHD_ACCESS_POINT);
}

int WhdSTAInterface::is_interface_connected(void)
{
    if (!_whd_emac.ifp) {
        return WHD_INTERFACE_NOT_UP;
    }
    _whd_emac.ifp->role = WHD_STA_ROLE;
    if ((whd_wifi_is_ready_to_transceive(_whd_emac.ifp) == WHD_SUCCESS)) {
        return WHD_SUCCESS;
    } else {
        return WHD_CONNECTION_LOST;
    }
}

int WhdSTAInterface::get_bssid(uint8_t *bssid)
{
    whd_mac_t ap_mac;
    whd_result_t res = WHD_SUCCESS;

    memset(&ap_mac, 0, sizeof(ap_mac));
    _whd_emac.ifp->role = WHD_STA_ROLE;
    if ((whd_wifi_is_ready_to_transceive(_whd_emac.ifp) == WHD_SUCCESS)) {
        res = whd_wifi_get_bssid(_whd_emac.ifp, &ap_mac);

        if (res == WHD_SUCCESS) {
            memcpy(bssid, ap_mac.octet, sizeof(ap_mac.octet));
        }
    }
    return res;
}

int WhdSTAInterface::whd_log_print(void)
{
    return whd_wifi_print_whd_log(_whd_emac.drvp);
}

int WhdSTAInterface::whd_log_read(char *buffer, int buffer_size)
{
    whd_result_t res = WHD_SUCCESS;
    if (buffer != NULL) {
        res = whd_wifi_read_wlan_log(_whd_emac.drvp, buffer, buffer_size);
    }
    return res;
}

nsapi_error_t WhdSTAInterface::wifi_get_ac_params_sta(void *acp)
{
    whd_result_t res = WHD_SUCCESS;
    edcf_acparam_t *ac_param = (edcf_acparam_t *)acp;

    res =  whd_wifi_get_acparams(_whd_emac.ifp, ac_param);
    if (res != WHD_SUCCESS) {
        return res;
    }
    return res;
}

int WhdSTAInterface::wifi_set_iovar_value(const char *iovar, uint32_t value)
{
    whd_result_t res = WHD_SUCCESS;
    _whd_emac.ifp->role = WHD_STA_ROLE;
    res = whd_wifi_set_iovar_value(_whd_emac.ifp, iovar, value);
    return res;
}

int WhdSTAInterface::wifi_get_iovar_value(const char *iovar, uint32_t *value)
{
    int res = WHD_SUCCESS;
    _whd_emac.ifp->role = WHD_STA_ROLE;
    res = whd_wifi_get_iovar_value(_whd_emac.ifp, iovar, value);
    return res;
}
int WhdSTAInterface::wifi_set_ioctl_value(uint32_t ioctl, uint32_t value)
{
    int res = WHD_SUCCESS;
    _whd_emac.ifp->role = WHD_STA_ROLE;
    res = whd_wifi_set_ioctl_value(_whd_emac.ifp, ioctl, value);
    return res;
}

int WhdSTAInterface::wifi_get_ioctl_value(uint32_t ioctl, uint32_t *value )
{
    int res = WHD_SUCCESS;
    _whd_emac.ifp->role = WHD_STA_ROLE;
    res = whd_wifi_get_ioctl_value(_whd_emac.ifp, ioctl, value);
    return res;
}

int WhdSTAInterface::wifi_get_ioctl_buffer(uint32_t ioctl, uint8_t *buffer, uint16_t len)
{
    int res = WHD_SUCCESS;
    _whd_emac.ifp->role = WHD_STA_ROLE;
    res = whd_wifi_get_ioctl_buffer(_whd_emac.ifp, ioctl, buffer, len);
    return res;
}

int WhdSTAInterface::wifi_set_ioctl_buffer(uint32_t ioctl, uint8_t *buffer, uint16_t len)
{
    int res = WHD_SUCCESS;
    _whd_emac.ifp->role = WHD_STA_ROLE;
    res = whd_wifi_set_ioctl_buffer(_whd_emac.ifp, ioctl, buffer, len);
    return res;
}

int WhdSTAInterface::wifi_get_ifp(whd_interface_t *ifp)
{
    int res = WHD_SUCCESS;
    *ifp = _whd_emac.ifp;
    return res;
}

int WhdSTAInterface::wifi_set_up(void)
{
    int res = WHD_SUCCESS;
    res = whd_wifi_set_up(_whd_emac.ifp);
    return res;
}

int WhdSTAInterface::wifi_set_down(void)
{
    int res = WHD_SUCCESS;
    res = whd_wifi_set_down(_whd_emac.ifp);
    return res;
}

int WhdSTAInterface::wifi_set_coex_config(whd_coex_config_t *coex_config)
{
    int res = WHD_SUCCESS;
    res = whd_wifi_set_coex_config(_whd_emac.ifp, coex_config);
    return res;
}
