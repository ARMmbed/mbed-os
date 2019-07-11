/* Wiced implementation of NetworkInterfaceAPI
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
extern "C" void whd_emac_wifi_link_state_changed(bool state_up, whd_interface_t ifp);


static const whd_event_num_t ap_client_events[] = { WLC_E_DEAUTH, WLC_E_DEAUTH_IND, WLC_E_DISASSOC, WLC_E_DISASSOC_IND, WLC_E_ASSOC_IND, WLC_E_REASSOC_IND, WLC_E_NONE };
static uint16_t ap_event_entry = 2;

WhdSoftAPInterface::WhdSoftAPInterface(WHD_EMAC &emac, OnboardNetworkStack &stack)
    : EMACInterface(emac, stack),
      _whd_emac(emac)
{

}


int WhdSoftAPInterface::start(const char *ssid, const char *pass, nsapi_security_t security, uint8_t channel,
                              bool start_dhcp_server, const whd_custom_ie_info_t *ie_info)
{
    nsapi_error_t        err;

    /* set up our interface */
    if (!_interface) {
        nsapi_error_t err = _stack.add_ethernet_interface(_emac, true, &_interface);
        if (err != NSAPI_ERROR_OK) {
            _interface = NULL;
            return err;
        }
        _interface->attach(_connection_status_cb);
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
        whd_emac_wifi_link_state_changed(true, _whd_emac.ifp);
    }

    // bring up
    err = _interface->bringup(_dhcp,
                              _ip_address[0] ? _ip_address : 0,
                              _netmask[0] ? _netmask : 0,
                              _gateway[0] ? _gateway : 0,
                              DEFAULT_STACK);
    if (err != NSAPI_ERROR_OK) {
        printf("bringup() ERROR: %d\n", err);
    }
    return err;
}



int WhdSoftAPInterface::stop(void)
{
    return whd_wifi_stop_ap(_whd_emac.ifp);
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
    return whd_management_set_event_handler(_whd_emac.ifp, ap_client_events, softap_event_handler, NULL, &ap_event_entry);
}

int WhdSoftAPInterface::unregister_event_handler(void)
{
    return whd_wifi_deregister_event_handler(_whd_emac.ifp, ap_event_entry);
}
