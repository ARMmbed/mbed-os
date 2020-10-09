/*
 * Copyright (c) 2018-2019 ARM Limited
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

#include "nsapi.h"
#include "lwipopts.h"
#include "WhdSoftAPInterface.h"
#include "nsapi.h"
#include "lwipopts.h"
#include "lwip/etharp.h"
#include "lwip/ethip6.h"
#include "rtos.h"
#include "whd_emac.h"
#include "whd_wifi_api.h"


extern int whd_toerror(whd_result_t res);
extern nsapi_security_t whd_tosecurity(whd_security_t sec);
extern whd_security_t whd_fromsecurity(nsapi_security_t sec);
extern "C" void whd_emac_wifi_link_state_changed(whd_interface_t ifp, whd_bool_t state_up);

static const whd_event_num_t ap_events[] = { WLC_E_LINK, WLC_E_IF, WLC_E_DEAUTH, WLC_E_DEAUTH_IND, WLC_E_DISASSOC, WLC_E_DISASSOC_IND, WLC_E_ASSOC_IND, WLC_E_REASSOC_IND, WLC_E_NONE };

static void *whd_default_handle_softap_events(whd_interface_t ifp, const whd_event_header_t *event_header,
                                              const uint8_t *event_data, void *handler_user_data)
{
    whd_driver_t whd_driver = ifp->whd_driver;

    UNUSED_PARAMETER(event_header);
    UNUSED_PARAMETER(event_data);
    UNUSED_PARAMETER(handler_user_data);

    WHD_IOCTL_LOG_ADD_EVENT(whd_driver, event_header->event_type, event_header->flags, event_header->reason);

    if ((event_header->event_type == (whd_event_num_t)WLC_E_LINK) ||
            (event_header->event_type == WLC_E_IF)) {
        if (osSemaphoreGetCount(whd_driver->ap_info.whd_wifi_sleep_flag) < 1) {
            osStatus_t result = osSemaphoreRelease(whd_driver->ap_info.whd_wifi_sleep_flag);
            if (result != osOK) {
                printf("Release whd_wifi_sleep_flag ERROR: %d", result);
            }
        }
    }
    return handler_user_data;
}


WhdSoftAPInterface::WhdSoftAPInterface(WHD_EMAC &emac, OnboardNetworkStack &stack, whd_interface_shared_info_t &shared)
    : EMACInterface(emac, stack),
      _whd_emac(emac),
      _iface_shared(shared)
{

}


int WhdSoftAPInterface::start(const char *ssid, const char *pass, nsapi_security_t security, uint8_t channel,
                              bool start_dhcp_server, const whd_custom_ie_info_t *ie_info, bool ap_sta_concur)
{
    ScopedMutexLock lock(_iface_shared.mutex);

    nsapi_error_t        err;
    // power up primary emac interface first
    if (ap_sta_concur) {
        WHD_EMAC &emac_prime = WHD_EMAC::get_instance(WHD_STA_ROLE);
        if (!emac_prime.power_up()) {
            printf("Primary interface power up ERROR!\n");
            return NSAPI_ERROR_DEVICE_ERROR;
        }
    }

    // set concurrency mode and power up secondary, the bsp init is done by primary emac
    _whd_emac.ap_sta_concur = ap_sta_concur;
    if (!_whd_emac.power_up()) {
        printf("Secondary interface power up ERROR!\n");
        return NSAPI_ERROR_DEVICE_ERROR;
    }

    // setup ssid
    whd_ssid_t whd_ssid;
    strncpy((char *)whd_ssid.value, ssid, SSID_NAME_SIZE);
    whd_ssid.value[SSID_NAME_SIZE - 1] = '\0';
    whd_ssid.length = strlen((char *)whd_ssid.value);

    // choose network security
    whd_security_t whd_security = whd_fromsecurity(security);

    /* set up the AP info */
    err = whd_wifi_init_ap(_whd_emac.ifp, &whd_ssid, whd_security, (const uint8_t *)pass,
                           strlen(pass), channel);
    if (err != NSAPI_ERROR_OK) {
        printf("whd_wifi_init_ap() ERROR: %d\n", err);
        return err;
    }

    // update default softap interface event handler
    err = unregister_event_handler();
    if (err != NSAPI_ERROR_OK) {
        printf("unregister_event_handler() ERROR: %d\n", err);
        return err;
    }
    err = register_event_handler(whd_default_handle_softap_events);
    if (err != NSAPI_ERROR_OK) {
        printf("register_event_handler() ERROR: %d\n", err);
        return err;
    }

    _iface_shared.if_status_flags |= IF_STATUS_SOFT_AP_UP;
    if (!ap_sta_concur || (_iface_shared.default_if_cfg == DEFAULT_IF_NOT_SET)) {
        _iface_shared.default_if_cfg = DEFAULT_IF_SOFT_AP;
    }
    if (!_interface) {
        nsapi_error_t err = _stack.add_ethernet_interface(_whd_emac, _iface_shared.default_if_cfg == DEFAULT_IF_SOFT_AP, &_interface);
        if (err != NSAPI_ERROR_OK) {
            _interface = NULL;
            return err;
        }
        _interface->attach(_connection_status_cb);
        _iface_shared.iface_softap = _interface;
    } else if (_iface_shared.default_if_cfg == DEFAULT_IF_SOFT_AP) {
        _stack.set_default_interface(_interface);
    }

    if (ie_info) {
        err = whd_wifi_manage_custom_ie(_whd_emac.ifp, WHD_ADD_CUSTOM_IE, (const uint8_t *)ie_info->oui,
                                        ie_info->subtype, (const void *)ie_info->data, ie_info->length, ie_info->which_packets);
        if (err != NSAPI_ERROR_OK) {
            printf("whd_wifi_manage_custom_ie() ERROR: %d\n", err);
            return err;
        }
    }

    err = whd_wifi_start_ap(_whd_emac.ifp);
    if (err != NSAPI_ERROR_OK) {
        printf("whd_wifi_start_ap() ERROR: %d\n", err);
        return err;
    }

    // Set static IP address for SoftAP and bring up
    set_dhcp(false);

    if (whd_wifi_is_ready_to_transceive(_whd_emac.ifp) == WHD_SUCCESS) {
        whd_emac_wifi_link_state_changed(_whd_emac.ifp, WHD_TRUE);
    }

    err = _interface->bringup(_dhcp,
                              _ip_address[0] ? _ip_address : 0,
                              _netmask[0] ? _netmask : 0,
                              _gateway[0] ? _gateway : 0,
                              DEFAULT_STACK);
    if (err != NSAPI_ERROR_OK) {
        printf("bringup() ERROR: %d\n", err);
    }

    if (start_dhcp_server) {
        _dhcp_server = std::make_unique<CyDhcpServer>(get_stack(), this);
        if (CY_RSLT_SUCCESS != _dhcp_server->start()) {
            err = NSAPI_ERROR_DHCP_FAILURE;
        }
    }
    return err;
}



int WhdSoftAPInterface::stop(void)
{
    ScopedMutexLock lock(_iface_shared.mutex);

    if (_dhcp_server && CY_RSLT_SUCCESS != _dhcp_server->stop()) {
        return NSAPI_ERROR_DHCP_FAILURE;
    }
    _dhcp_server.reset();

    // bring down
    int err = _interface->bringdown();
    if (err) {
        return err;
    }

    _iface_shared.if_status_flags &= ~IF_STATUS_SOFT_AP_UP;
    if ((_iface_shared.if_status_flags & IF_STATUS_STA_UP) == 0) {
        _iface_shared.default_if_cfg = DEFAULT_IF_NOT_SET;
    }

    // stop softap
    whd_result_t res = whd_wifi_stop_ap(_whd_emac.ifp);
    if (res != WHD_SUCCESS) {
        return whd_toerror(res);
    }

    // remove the interface added in start
    if (_interface) {
        nsapi_error_t err = _stack.remove_ethernet_interface(&_interface);
        if (err != NSAPI_ERROR_OK) {
            return err;
        }
        _iface_shared.iface_softap = NULL;
    }
    return NSAPI_ERROR_OK;
}

int WhdSoftAPInterface::remove_custom_ie(const whd_custom_ie_info_t *ie_info)
{
    return whd_wifi_manage_custom_ie(_whd_emac.ifp, WHD_REMOVE_CUSTOM_IE, (const uint8_t *)ie_info->oui,
                                     ie_info->subtype, (const void *)ie_info->data, ie_info->length, ie_info->which_packets);
}

int WhdSoftAPInterface::get_associated_client_list(void *client_list_buffer, uint16_t buffer_length)
{

    return whd_wifi_get_associated_client_list(_whd_emac.ifp, client_list_buffer, buffer_length);
}

int WhdSoftAPInterface::register_event_handler(whd_event_handler_t softap_event_handler)
{
    uint16_t ap_events_entry = _whd_emac.ifp->event_reg_list[WHD_AP_EVENT_ENTRY];
    return whd_management_set_event_handler(_whd_emac.ifp, ap_events, softap_event_handler, NULL, &ap_events_entry);
}

int WhdSoftAPInterface::unregister_event_handler(void)
{
    return whd_wifi_deregister_event_handler(_whd_emac.ifp, _whd_emac.ifp->event_reg_list[WHD_AP_EVENT_ENTRY]);
}
