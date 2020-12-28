/*
 * Copyright 2018-2020 Cypress Semiconductor Corporation
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

#include "scl_wifi_api.h"
#include "scl_ipc.h"
#include "scl_types.h"

/******************************************************
 *        Variables Definitions
 *****************************************************/

typedef struct {
    scl_mac_t *mac;
    uint32_t retval;
} scl_mac;

typedef struct {
    scl_scan_type_t scan_type;
    scl_bss_type_t bss_type;
    const scl_ssid_t *optional_ssid;
    const scl_mac_t *optional_mac;
    const uint16_t *optional_channel_list;
    const scl_scan_extended_params_t *optional_extended_params;
    //scl_scan_result_callback_t callback;
    scl_scan_result_t *result_ptr;
    void *user_data;
} scl_scan_parameters_for_np_t;

scl_scan_result_callback_t scan_callback;

/******************************************************
 *               Function Definitions
 ******************************************************/

scl_result_t scl_wifi_is_ready_to_transceive(void)
{
    scl_result_t result = SCL_SUCCESS;
    scl_result_t retval = SCL_SUCCESS;

    result = scl_send_data(SCL_TX_TRANSCEIVE_READY, (char *)&retval, TIMER_DEFAULT_VALUE);
    if (result == SCL_ERROR) {
        SCL_LOG(("Ready to tranceive error\r\n"));
        return SCL_ERROR;
    } else {
        return retval;
    }
}

bool scl_wifi_on(void)
{
    bool retval = false;
    scl_result_t result = SCL_SUCCESS;
    result = scl_send_data(SCL_TX_WIFI_ON, (char *)&retval, WIFI_ON_TIMEOUT);
    if (result == SCL_ERROR) {
        SCL_LOG(("wifi_on Error\r\n"));
        return false;
    } else {
        return retval;
    }
}

scl_result_t scl_wifi_set_up(void)
{
    scl_result_t retval = SCL_SUCCESS;
    scl_result_t result = SCL_SUCCESS;
    result = scl_send_data(SCL_TX_WIFI_SET_UP, (char *)&retval, TIMER_DEFAULT_VALUE);
    if (result == SCL_SUCCESS) {
        return retval;
    } else {
        SCL_LOG(("Wifi set up error\r\n"));
        return SCL_ERROR;
    }
}

scl_result_t scl_wifi_get_mac_address(scl_mac_t *mac)
{
    scl_mac scl_mac_data;
    scl_result_t scl_retval = SCL_SUCCESS;
    scl_mac_data.mac = mac;
    scl_mac_data.retval = SCL_SUCCESS;

    if (mac == NULL) {
        return SCL_BADARG;
    }
    scl_retval = scl_send_data(SCL_TX_GET_MAC, (char *)&scl_mac_data, TIMER_DEFAULT_VALUE);
    if (scl_retval == SCL_SUCCESS) {
        return scl_mac_data.retval;
    } else {
        SCL_LOG(("Get MAC address error\r\n"));
        return SCL_ERROR;
    }
}

scl_result_t scl_wifi_get_bssid(scl_mac_t *bssid)
{
    struct scl_bssid {
        scl_mac_t *bssid;
        uint32_t retval;
    } scl_bssid_t;
    scl_result_t scl_retval = SCL_SUCCESS;
    scl_bssid_t.bssid = bssid;
    scl_bssid_t.retval = SCL_SUCCESS;
    if (bssid == NULL) {
        return SCL_BADARG;
    }
    scl_retval = scl_send_data(SCL_TX_WIFI_GET_BSSID, (char *)&scl_bssid_t, TIMER_DEFAULT_VALUE);
    if (scl_retval == SCL_SUCCESS) {
        return scl_bssid_t.retval;
    } else {
        SCL_LOG(("get bssid error\r\n"));
        return SCL_ERROR;
    }
}

scl_result_t scl_wifi_register_multicast_address(scl_mac_t *mac)
{
    scl_mac scl_mac_t;
    scl_mac_t.mac = mac;
    scl_mac_t.retval = SCL_SUCCESS;
    scl_result_t scl_retval = SCL_SUCCESS;
    if (mac == NULL) {
        return SCL_BADARG;
    }
    scl_retval = scl_send_data(SCL_TX_REGISTER_MULTICAST_ADDRESS, (char *)&scl_mac_t, TIMER_DEFAULT_VALUE);
    if (scl_retval != SCL_SUCCESS) {
        SCL_LOG(("Register Multicast Address IPC Error\r\n"));
        return SCL_ERROR;
    }
    return (scl_mac_t.retval);
}

scl_result_t scl_network_send_ethernet_data(scl_tx_buf_t scl_buffer)
{
    scl_result_t retval = SCL_SUCCESS;
    if (scl_buffer.buffer == NULL) {
        return SCL_BADARG;
    }
    retval = scl_send_data(SCL_TX_SEND_OUT, (char *)&scl_buffer, TIMER_DEFAULT_VALUE);
    return retval;
}

scl_result_t scl_wifi_get_rssi(int32_t *rssi)
{
    struct tx_param {
        uint32_t retval;
        int32_t *get_rssi;
    } tx_param_t;
    scl_result_t scl_retval = SCL_SUCCESS;

    if (rssi == NULL) {
        return SCL_BADARG;
    }
    tx_param_t.get_rssi = rssi;
    scl_retval = scl_send_data(SCL_TX_WIFI_GET_RSSI, (char *) &tx_param_t, TIMER_DEFAULT_VALUE);
    if (scl_retval == SCL_SUCCESS) {
        return tx_param_t.retval;
    } else {
        SCL_LOG(("get rssi error\r\n"));
        return SCL_ERROR;
    }
}

/*
 * NOTE: search references of function wlu_get in wl/exe/wlu.c to find what format the returned IOCTL data is.
 */
uint32_t scl_wifi_scan(scl_scan_type_t scan_type,
                       scl_bss_type_t bss_type,
                       const scl_ssid_t *optional_ssid,
                       const scl_mac_t *optional_mac,
                       const uint16_t *optional_channel_list,
                       const scl_scan_extended_params_t *optional_extended_params,
                       scl_scan_result_callback_t callback,
                       scl_scan_result_t *result_ptr,
                       void *user_data
                       )
{
    scl_scan_parameters_for_np_t scl_scan_parameters_for_np;
    scl_result_t retval = SCL_SUCCESS;
    /* fill the scan parameters to a structure and send it to NP */
    scl_scan_parameters_for_np.scan_type = scan_type;
    scl_scan_parameters_for_np.bss_type = bss_type;
    scl_scan_parameters_for_np.optional_ssid = optional_ssid;
    scl_scan_parameters_for_np.optional_mac = optional_mac;
    scl_scan_parameters_for_np.optional_channel_list = optional_channel_list;
    scl_scan_parameters_for_np.optional_extended_params = optional_extended_params;
    scl_scan_parameters_for_np.result_ptr = result_ptr;
    scl_scan_parameters_for_np.user_data = user_data;
    /* callback to be used when there is a scan result from CP */
    scan_callback = callback;
    /* send scan parameters to NP*/
    retval = scl_send_data(SCL_TX_SCAN, (char *)&scl_scan_parameters_for_np, TIMER_DEFAULT_VALUE);
    return retval;
}
