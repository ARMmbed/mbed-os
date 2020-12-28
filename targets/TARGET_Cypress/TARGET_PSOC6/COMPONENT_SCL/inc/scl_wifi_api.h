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

/** @file
 *  Prototypes of functions for controlling the Wi-Fi system
 *
 *  This file provides prototypes for end-user functions, which allow
 *  actions such as scanning for Wi-Fi networks, joining Wi-Fi
 *  networks, getting the MAC address, and so on.
 *
 */

#include <stdbool.h>
#include "scl_common.h"
#include "scl_types.h"
#ifndef INCLUDED_SCL_WIFI_API_H
#define INCLUDED_SCL_WIFI_API_H

#ifdef __cplusplus
extern "C"
{
#endif

/**
 * SCL transmit buffer structure
 */
typedef struct scl_tx_buf {
    scl_buffer_t buffer; /**< pointer to the buffer */
    uint32_t size;       /**< size of the buffer */
} scl_tx_buf_t;

/******************************************************
*             Function Declarations
******************************************************/

/** @addtogroup wifi SCL Wi-Fi API
 *  APIs for controlling the Wi-Fi system
 *  @{
 */

/** Turn on the Wi-Fi device
 *
 *  @note This API should be called before using any SCL Wi-Fi API.
 *
 *  @return True if initialization is successful, False otherwise.
 */
extern bool scl_wifi_on(void);

/** Brings up the Wi-Fi core
 *
 *  @return SCL_SUCCESS or Error code.
 */
extern scl_result_t scl_wifi_set_up(void);

/** Retrieves the current Media Access Control (MAC) address
 *  (or Ethernet hardware address) of the 802.11 device
 *
 *  @param   mac           Pointer to a variable to which the current MAC address will be written.
 *
 *  @return  SCL_SUCCESS or Error code.
 */
extern scl_result_t scl_wifi_get_mac_address(scl_mac_t *mac);

/** Gets the BSSID of the interface
 *
 *  @note This API should be used after the device is connected to a network.
 *
 *  @param  bssid         Returns the BSSID address (mac address), if associated.
 *
 *  @return SCL_SUCCESS or Error code.
 */
extern scl_result_t scl_wifi_get_bssid(scl_mac_t *bssid);

/** Registers interest in a multicast address
 *
 *  Once a multicast address has been registered, all packets detected on the
 *  medium destined for that address are forwarded to the host.
 *  Otherwise, the packets are ignored.
 *
 *  @param  mac              Ethernet MAC address.
 *
 *  @return SCL_SUCCESS      If the address was registered successfully or Error code.
 */
extern scl_result_t scl_wifi_register_multicast_address(scl_mac_t *mac);

/** Determines if an interface is ready to transmit/receive ethernet packets.
 *
 *  @note This function must be called after the connection is established; otherwise, it returns Error code.
 *
 *  @return    SCL_SUCCESS            If the interface is ready to transmit/receive ethernet packets.
 *             SCL_NOTFOUND           If no AP with a matching SSID was found.
 *             SCL_NOT_AUTHENTICATED  If matching AP was found, but it does not let you authenticate.
 *                                    @note This can occur if the device is in the blocklist of the AP.
 *             SCL_NOT_KEYED          If the device has authenticated and associated but has not completed the key exchange.
 *                                    @note This can occur if the passphrase is incorrect.
 *             Error code             If the interface is not ready to transmit/receive ethernet packets.
 */
extern scl_result_t scl_wifi_is_ready_to_transceive(void);

/** Sends an ethernet frame to SCL (called by the Network Stack)
 *
 *  This function takes ethernet data from the network stack and transmits over the wireless network.
 *  This function returns immediately after the packet has been queued for transmission,
 *  NOT after it has been transmitted. Packet buffers passed to the SCL
 *  are released inside the SCL once they have been transmitted.
 *
 *  @param buffer        Handle of the packet buffer to be sent.
 *
 *  @return SCL_SUCCESS or Error code.
 */
extern scl_result_t scl_network_send_ethernet_data(scl_tx_buf_t buffer);

/** Retrieves the latest RSSI value
 *
 *  @note This API must be called after the device is connected to a network.
 *
 *  @param   rssi          Location where the RSSI value will be stored.
 *
 *  @return  SCL_SUCCESS   If the RSSI was successfully retrieved or Error code.
 */
extern scl_result_t scl_wifi_get_rssi(int32_t *rssi);

/** Retrieves the RX data packet
 *
 *  @param   buffer        Pointer to RX buffer.
 */
extern void scl_network_process_ethernet_data(scl_buffer_t buffer);

/** Notifies network stack about the change in network connection state
 *
 *  @param   state_up      Connection state.
 */
extern void scl_emac_wifi_link_state_changed(bool state_up);

/** Scan result callback function pointer type
 *
 * @param result_ptr   A pointer to the pointer that indicates where to put the next scan result
 * @param user_data    User provided data
 * @param status       Status of scan process
 */
typedef void (*scl_scan_result_callback_t)(scl_scan_result_t *result_ptr, void *user_data, scl_scan_status_t status);

/** Initiates a scan to search for 802.11 networks.
 *
 *  The scan progressively accumulates results over time, and may take between 1 and 10 seconds to complete.
 *  The results of the scan will be individually provided to the callback function.
 *  Note: The callback function will be executed in the context of the SCL thread and so must not perform any
 *  actions that may cause a bus transaction.
 *
 *  @param   scan_type                 Specifies whether the scan should be Active, Passive or scan Prohibited channels
 *  @param   bss_type                  Specifies whether the scan should search for Infrastructure networks (those using
 *                                     an Access Point), Ad-hoc networks, or both types.
 *  @param   optional_ssid             If this is non-Null, then the scan will only search for networks using the specified SSID.
 *  @param   optional_mac              If this is non-Null, then the scan will only search for networks where
 *                                     the BSSID (MAC address of the Access Point) matches the specified MAC address.
 *  @param   optional_channel_list     If this is non-Null, then the scan will only search for networks on the
 *                                     specified channels - array of channel numbers to search, terminated with a zero
 *  @param   optional_extended_params  If this is non-Null, then the scan will obey the specifications about
 *                                     dwell times and number of probes.
 *  @param   callback                  The callback function which will receive and process the result data.
 *  @param   result_ptr                Pointer to a pointer to a result storage structure.
 *  @param   user_data                 user specific data that will be passed directly to the callback function
 *
 *  @note - When scanning specific channels, devices with a strong signal strength on nearby channels may be detected
 *        - Callback must not use blocking functions, nor use SCL functions, since it is called from the context of the
 *          SCL thread.
 *        - The callback, result_ptr and user_data variables will be referenced after the function returns.
 *          Those variables must remain valid until the scan is complete.
 *
 *  @return SCL_SUCCESS or Error code
 */
extern uint32_t scl_wifi_scan(scl_scan_type_t scan_type,
                              scl_bss_type_t bss_type,
                              const scl_ssid_t *optional_ssid,
                              const scl_mac_t *optional_mac,
                              const uint16_t *optional_channel_list,
                              const scl_scan_extended_params_t *optional_extended_params,
                              scl_scan_result_callback_t callback,
                              scl_scan_result_t *result_ptr,
                              void *user_data);

extern scl_scan_result_callback_t scan_callback;
#ifdef __cplusplus
} /* extern "C" */
#endif
#endif /* ifndef INCLUDED_SCL_WIFI_API_H */
