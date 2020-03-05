/*
 * (c) (2019-2020), Cypress Semiconductor Corporation. All rights reserved.
 *
 * This software, including source code, documentation and related materials
 * ("Software"), is owned by Cypress Semiconductor Corporation or one of its
 * subsidiaries ("Cypress") and is protected by and subject to worldwide patent
 * protection (United States and foreign), United States copyright laws and
 * international treaty provisions. Therefore, you may use this Software only
 * as provided in the license agreement accompanying the software package from
 * which you obtained this Software ("EULA").
 *
 * If no EULA applies, Cypress hereby grants you a personal, non-exclusive,
 * non-transferable license to copy, modify, and compile the Software source
 * code solely for use in connection with Cypress' integrated circuit products.
 * Any reproduction, modification, translation, compilation, or representation
 * of this Software except as specified above is prohibited without the express
 * written permission of Cypress.
 *
 * Disclaimer: THIS SOFTWARE IS PROVIDED AS-IS, WITH NO WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING, BUT NOT LIMITED TO, NONINFRINGEMENT, IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE. Cypress
 * reserves the right to make changes to the Software without notice. Cypress
 * does not assume any liability arising out of the application or use of the
 * Software or any product or circuit described in the Software. Cypress does
 * not authorize its products for use in any products where a malfunction or
 * failure of the Cypress product may reasonably be expected to result in
 * significant property damage, injury or death ("High Risk Product"). By
 * including Cypress' product in a High Risk Product, the manufacturer of such
 * system or application assumes all risk of such use and in doing so agrees to
 * indemnify Cypress against all liability.
 */

#include <cstring>
#include <algorithm>
#include <vector>
#include "SclSTAInterface.h"
#include "nsapi.h"
#include "lwipopts.h"
#include "lwip/etharp.h"
#include "lwip/ethip6.h"
#include "rtos.h"
#include "scl_emac.h"
#include "scl_ipc.h"
#include "mbed_wait_api.h"

#define PARAM_LEN                             (20)

struct network_params_t {
    char ip_address[PARAM_LEN];
    char netmask[PARAM_LEN];
    char gateway[PARAM_LEN];
    int  connection_status;
};

struct scl_tx_nw_credentials {
    nsapi_security_t network_security_type;
    int ssid_len;
    int pass_len;
    const char* network_ssid;
    const char* network_passphrase;
} scl_tx_nw_credentials_t;
struct network_params_t network_parameter;

int scl_toerror(scl_result_t res)
{
    switch (res) {
        case SCL_SUCCESS:
            return NSAPI_ERROR_OK;
        case SCL_UNSUPPORTED:
            return NSAPI_ERROR_UNSUPPORTED;
        case SCL_BADARG:
            return NSAPI_ERROR_PARAMETER;
        case SCL_INVALID_JOIN_STATUS:
            return NSAPI_ERROR_NO_CONNECTION;
        case SCL_BUFFER_UNAVAILABLE_PERMANENT:
        case SCL_BUFFER_UNAVAILABLE_TEMPORARY:
        case SCL_RX_BUFFER_ALLOC_FAIL:
        case SCL_BUFFER_ALLOC_FAIL:
        case SCL_MALLOC_FAILURE:
            return NSAPI_ERROR_NO_MEMORY;
        case SCL_ACCESS_POINT_NOT_FOUND:
        case SCL_NETWORK_NOT_FOUND:
            return NSAPI_ERROR_NO_SSID;
        case SCL_NOT_AUTHENTICATED:
        case SCL_INVALID_KEY:
        case SCL_NOT_KEYED:
            return NSAPI_ERROR_AUTH_FAILURE;
        case SCL_PENDING:
        case SCL_JOIN_IN_PROGRESS:
            return NSAPI_ERROR_IN_PROGRESS;
        case SCL_CONNECTION_LOST:
            return NSAPI_ERROR_CONNECTION_LOST;
        case SCL_TIMEOUT:
        case SCL_EAPOL_KEY_PACKET_M1_TIMEOUT:
        case SCL_EAPOL_KEY_PACKET_M3_TIMEOUT:
        case SCL_EAPOL_KEY_PACKET_G1_TIMEOUT:
            return NSAPI_ERROR_CONNECTION_TIMEOUT;
        default:
            return -res;
    }
}

nsapi_security_t scl_tosecurity(scl_security_t sec)
{
    switch (sec) {
        case SCL_SECURITY_OPEN:
            return NSAPI_SECURITY_NONE;
        case SCL_SECURITY_WEP_PSK:
        case SCL_SECURITY_WEP_SHARED:
            return NSAPI_SECURITY_WEP;
        case SCL_SECURITY_WPA_TKIP_PSK:
        case SCL_SECURITY_WPA_TKIP_ENT:
            return NSAPI_SECURITY_WPA;
        case SCL_SECURITY_WPA2_MIXED_PSK:
            return NSAPI_SECURITY_WPA_WPA2;
        case SCL_SECURITY_WPA2_AES_PSK:
        case SCL_SECURITY_WPA2_AES_ENT:
        case SCL_SECURITY_WPA2_FBT_PSK:
        case SCL_SECURITY_WPA2_FBT_ENT:
            return NSAPI_SECURITY_WPA2;
        default:
            return NSAPI_SECURITY_UNKNOWN;
    }
}

scl_security_t scl_fromsecurity(nsapi_security_t sec)
{
    switch (sec) {
        case NSAPI_SECURITY_NONE:
            return SCL_SECURITY_OPEN;
        case NSAPI_SECURITY_WEP:
            return SCL_SECURITY_WEP_PSK;
        case NSAPI_SECURITY_WPA:
            return SCL_SECURITY_WPA_MIXED_PSK;
        case NSAPI_SECURITY_WPA2:
            return SCL_SECURITY_WPA2_AES_PSK;
        case NSAPI_SECURITY_WPA_WPA2:
            return SCL_SECURITY_WPA2_MIXED_PSK;
        default:
            return SCL_SECURITY_UNKNOWN;
    }
}

SclSTAInterface::SclSTAInterface(SCL_EMAC &emac, OnboardNetworkStack &stack)
    : EMACInterface(emac, stack),
      _ssid("\0"),
      _pass("\0"),
      _security(NSAPI_SECURITY_NONE),
      _scl_emac(emac)
{
}

nsapi_error_t SclSTAInterface::connect(const char *ssid, const char *pass, nsapi_security_t security, uint8_t channel)
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

nsapi_error_t SclSTAInterface::set_credentials(const char *ssid, const char *pass, nsapi_security_t security)
{
    if ((ssid == NULL) ||
            (strlen(ssid) == 0) ||
            (pass == NULL && (security != NSAPI_SECURITY_NONE)) ||
            (strlen(pass) == 0 && (security != NSAPI_SECURITY_NONE)) ||
            (strlen(pass) > 63 && (security == NSAPI_SECURITY_WPA2 || security == NSAPI_SECURITY_WPA || security == NSAPI_SECURITY_WPA_WPA2))
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

nsapi_error_t SclSTAInterface::connect()
{

    uint32_t delay_timeout = 0;
    scl_result_t ret_val;
    nsapi_error_t interface_status;
    uint32_t connection_status = 0;

    scl_tx_nw_credentials_t.network_ssid = _ssid;
    if (strlen(_ssid) < MAX_SSID_LENGTH) {
        scl_tx_nw_credentials_t.ssid_len = strlen(_ssid);
    }
    scl_tx_nw_credentials_t.network_passphrase = _pass;
    if (strlen(_pass) < MAX_PASSWORD_LENGTH) {
        scl_tx_nw_credentials_t.pass_len = strlen(_pass);
    }
    scl_tx_nw_credentials_t.network_security_type = _security;
    
    ret_val = scl_send_data(SCL_TX_CONNECT, (char*)&scl_tx_nw_credentials_t, TIMER_DEFAULT_VALUE);
    
    if (ret_val == SCL_SUCCESS) {
        SCL_LOG(("wifi provisioning in progress"));
    }

    network_parameter.connection_status = NSAPI_STATUS_DISCONNECTED;


    //Get the network parameter from NP
    while ((network_parameter.connection_status != NSAPI_STATUS_GLOBAL_UP) && delay_timeout < NW_CONNECT_TIMEOUT) {
        ret_val = scl_get_nw_parameters(&network_parameter);
        wait_us(NW_DELAY_TIME);
        delay_timeout++;
    }
    
    if (delay_timeout >= NW_CONNECT_TIMEOUT || ret_val != SCL_SUCCESS) {
        return NSAPI_ERROR_NO_CONNECTION;
    }
    
    if (!_scl_emac.powered_up) {
        _scl_emac.power_up();
    }
    
    if (!_interface) {
        nsapi_error_t err = _stack.add_ethernet_interface(_emac, true, &_interface);
        if (err != NSAPI_ERROR_OK) {
            _interface = NULL;
            return err;
        }
        _interface->attach(_connection_status_cb);
    }

    if (!scl_wifi_is_ready_to_transceive()) {
        scl_emac_wifi_link_state_changed(true);
    }

    interface_status = _interface->bringup(false,
                               network_parameter.ip_address,
                               network_parameter.netmask,
                               network_parameter.gateway,
                               DEFAULT_STACK);

    scl_send_data(SCL_TX_CONNECTION_STATUS, (char*)&connection_status, TIMER_DEFAULT_VALUE);

    return interface_status;
}

void SclSTAInterface::wifi_on()
{
    if (!_scl_emac.powered_up) {
        _scl_emac.power_up();
    }
}

nsapi_error_t SclSTAInterface::disconnect()
{
    scl_result_t ret_val;
    nsapi_error_t disconnect_status;
    ret_val = scl_send_data(SCL_TX_DISCONNECT, (char*)&disconnect_status, TIMER_DEFAULT_VALUE);
    
    if (ret_val == SCL_ERROR) {
        return NSAPI_ERROR_TIMEOUT;
    }
    
    if (!_interface) {
        return NSAPI_STATUS_DISCONNECTED;
    }

    // bring down
    int err = _interface->bringdown();
    if (err) {
        return err;
    }

    scl_emac_wifi_link_state_changed(false);

    return NSAPI_ERROR_OK;
}

int SclSTAInterface::scan(WiFiAccessPoint *res, unsigned count)
{
    /* To Do */
    return NSAPI_ERROR_UNSUPPORTED;
}

int8_t SclSTAInterface::get_rssi()
{
    int32_t rssi;
    scl_result_t res;

    if (!_scl_emac.powered_up) {
        _scl_emac.power_up();
    }

    res = (scl_result_t) scl_wifi_get_rssi(&rssi);
    if (res == SCL_ERROR) {
        return SCL_ERROR;
    }

    return (int8_t)rssi;
}

int SclSTAInterface::is_interface_connected(void)
{
    if (scl_wifi_is_ready_to_transceive() == SCL_SUCCESS) {
        return SCL_SUCCESS;
    } else {
        return SCL_CONNECTION_LOST;
    }
}

int SclSTAInterface::get_bssid(uint8_t *bssid)
{
    scl_mac_t ap_mac;
    scl_result_t res = SCL_SUCCESS;
    
    if (bssid == NULL) {
        return SCL_BADARG;
    }
    
    memset(&ap_mac, 0, sizeof(ap_mac));
    if (scl_wifi_is_ready_to_transceive() == SCL_SUCCESS) {
        res = (scl_result_t) scl_wifi_get_bssid(&ap_mac);
        if (res == SCL_SUCCESS) {
            memcpy(bssid, ap_mac.octet, sizeof(ap_mac.octet));
        }
    }
    else {
        return SCL_CONNECTION_LOST;
    }
    return res;
}

int SclSTAInterface::wifi_set_up(void)
{
    int res = SCL_SUCCESS;
    res = scl_wifi_set_up();
    return res;
}
