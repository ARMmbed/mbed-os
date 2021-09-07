/*
 * Copyright 2021, Cypress Semiconductor Corporation (an Infineon company)
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
 *  This file provides prototypes for end-user functions which allow
 *  actions such as scanning for Wi-Fi networks, joining Wi-Fi
 *  networks, getting the MAC address, etc
 *
 */

#include "whd.h"
#include "whd_types.h"

#ifndef INCLUDED_WHD_WIFI_API_H
#define INCLUDED_WHD_WIFI_API_H

#ifdef __cplusplus
extern "C"
{
#endif

/******************************************************
*             Function declarations
******************************************************/

/** @addtogroup wifi WHD Wi-Fi API
 *  APIs for controlling the Wi-Fi system
 *  @{
 */

/** @addtogroup wifimanagement WHD Wi-Fi Management API
 *  @ingroup wifi
 *  Initialisation and other management functions for WHD system
 *  @{
 */

/** Initialize an instance of the WHD driver
 *
 *  @param whd_driver_ptr       Pointer to Pointer to handle instance of the driver
 *  @param whd_init_config      Pointer to configuration data that controls how the driver is initialized
 *  @param resource_ops         Pointer to resource interface to provide resources to the driver initialization process
 *  @param buffer_ops           Pointer to a buffer interface to provide buffer related services to the driver instance
 *  @param network_ops          Pointer to a whd_netif_funcs_t to provide network stack services to the driver instance
 *
 *  @return WHD_SUCCESS or Error code
 */
extern uint32_t whd_init(whd_driver_t *whd_driver_ptr, whd_init_config_t *whd_init_config,
                         whd_resource_source_t *resource_ops, whd_buffer_funcs_t *buffer_ops,
                         whd_netif_funcs_t *network_ops);
/* @} */
/* @} */

/** @addtogroup busapi WHD Bus API
 * Allows WHD to operate with specific SDIO/SPI bus
 *  @{
 */

/** Attach the WLAN Device to a specific SDIO bus
 *
 *  @param  whd_driver         Pointer to handle instance of the driver
 *  @param  whd_config         Configuration for SDIO bus
 *  @param  sdio_obj           The SDHC hardware interface, from the Level 3 CY HW APIs
 *
 *  @return WHD_SUCCESS or Error code
 */
extern uint32_t whd_bus_sdio_attach(whd_driver_t whd_driver, whd_sdio_config_t *whd_config, cyhal_sdio_t *sdio_obj);

/** Detach the WLAN Device to a specific SDIO bus
 *
 *  @param  whd_driver         Pointer to handle instance of the driver
 */
extern void whd_bus_sdio_detach(whd_driver_t whd_driver);

/** Attach the WLAN Device to a specific SPI bus
 *
 *  @param  whd_driver        Pointer to handle instance of the driver
 *  @param  whd_config        Configuration for SPI bus
 *  @param  spi_obj           The SPI hardware interface, from the Level 3 CY HW APIs
 *
 *  @return WHD_SUCCESS or Error code
 */
extern uint32_t whd_bus_spi_attach(whd_driver_t whd_driver, whd_spi_config_t *whd_config, cyhal_spi_t *spi_obj);

/** Detach the WLAN Device to a specific SPI bus
 *
 *  @param  whd_driver         Pointer to handle instance of the driver
 */
extern void whd_bus_spi_detach(whd_driver_t whd_driver);

/*  @} */

/** @addtogroup wifi WHD Wi-Fi API
 *  APIs for controlling the Wi-Fi system
 *  @{
 */

/** @addtogroup wifimanagement WHD Wi-Fi Management API
 *  @ingroup wifi
 *  Initialisation and other management functions for WHD system
 *  @{
 */

/**
 * Turn on the Wi-Fi device
 *
 *  Initialise Wi-Fi platform
 *  Program various WiFi parameters and modes
 *
 *  @param  whd_driver        Pointer to handle instance of the driver
 *  @param   ifpp             Pointer to Pointer to handle instance of whd interface
 *
 *  @return WHD_SUCCESS if initialization is successful, error code otherwise
 */
extern uint32_t whd_wifi_on(whd_driver_t whd_driver, whd_interface_t *ifpp);

/**
 * Turn off the Wi-Fi device
 *
 *  - De-Initialises the required parts of the hardware platform
 *    i.e. pins for SDIO/SPI, interrupt, reset, power etc.
 *
 *  - De-Initialises the whd thread which arbitrates access
 *    to the SDIO/SPI bus
 *
 *  @param   ifp              Pointer to handle instance of whd interface
 *
 *  @return WHD_SUCCESS if deinitialization is successful, Error code otherwise
 */
extern uint32_t whd_wifi_off(whd_interface_t ifp);

/** Shutdown this instance of the wifi driver, freeing all used resources
 *
 *  @param   ifp              Pointer to handle instance of whd interface
 *
 *  @return WHD_SUCCESS or Error code
 */
extern uint32_t whd_deinit(whd_interface_t ifp);

/** Brings up the Wi-Fi core
 *
 *  @param   ifp              Pointer to handle instance of whd interface
 *
 *  @return WHD_SUCCESS or Error code
 */
extern uint32_t whd_wifi_set_up(whd_interface_t ifp);

/** Bring down the Wi-Fi core
 *
 *  WARNING / NOTE:
 *     This brings down the Wi-Fi core and existing network connections will be lost.
 *
 *  @param   ifp               Pointer to handle instance of whd interface
 *
 *  @return WHD_SUCCESS or Error code
 */
extern uint32_t whd_wifi_set_down(whd_interface_t ifp);

/** Creates a secondary interface
 *
 *  @param  whd_drv              Pointer to handle instance of the driver
 *  @param  mac_addr             MAC address for the interface
 *  @param  ifpp                 Pointer to the whd interface pointer
 *
 *  @return WHD_SUCCESS or Error code
 */
extern uint32_t whd_add_secondary_interface(whd_driver_t whd_drv, whd_mac_t *mac_addr, whd_interface_t *ifpp);
/*  @} */

/** @addtogroup wifijoin   WHD Wi-Fi Join, Scan and Halt API
 *  @ingroup wifi
 *  Wi-Fi APIs for join, scan & leave
 *  @{
 */

/** Scan result callback function pointer type
 *
 * @param result_ptr   A pointer to the pointer that indicates where to put the next scan result
 * @param user_data    User provided data
 * @param status       Status of scan process
 */
typedef void (*whd_scan_result_callback_t)(whd_scan_result_t **result_ptr, void *user_data, whd_scan_status_t status);

/** Initiates a scan to search for 802.11 networks.
 *
 *  This functions returns the scan results with limited sets of parameter in a buffer provided by the caller.
 *  It is also a blocking call. It is an simplified version of the whd_wifi_scan().
 *
 *  @param   ifp                       Pointer to handle instance of whd interface
 *  @param   scan_result               pointer to user requested records buffer.
 *  @param   count                     No of records user is interested in.
 *                                     If 0 return the total record count.
 *
 *  @note    When scanning specific channels, devices with a strong signal strength on nearby channels may be detected
 *
 *  @return record count or Error code
 */
extern uint32_t whd_wifi_scan_synch(whd_interface_t ifp,
                                    whd_sync_scan_result_t *scan_result,
                                    uint32_t count
                                    );

/** Initiates a scan to search for 802.11 networks.
 *
 *  The scan progressively accumulates results over time, and may take between 1 and 10 seconds to complete.
 *  The results of the scan will be individually provided to the callback function.
 *  Note: The callback function will be executed in the context of the WHD thread and so must not perform any
 *  actions that may cause a bus transaction.
 *
 *  @param   ifp                       Pointer to handle instance of whd interface
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
 *        - Callback must not use blocking functions, nor use WHD functions, since it is called from the context of the
 *          WHD thread.
 *        - The callback, result_ptr and user_data variables will be referenced after the function returns.
 *          Those variables must remain valid until the scan is complete.
 *
 *  @return WHD_SUCCESS or Error code
 */
extern uint32_t whd_wifi_scan(whd_interface_t ifp,
                              whd_scan_type_t scan_type,
                              whd_bss_type_t bss_type,
                              const whd_ssid_t *optional_ssid,
                              const whd_mac_t *optional_mac,
                              const uint16_t *optional_channel_list,
                              const whd_scan_extended_params_t *optional_extended_params,
                              whd_scan_result_callback_t callback,
                              whd_scan_result_t *result_ptr,
                              void *user_data);

/** Abort a previously issued scan
 *
 *  @param   ifp           Pointer to handle instance of whd interface
 *
 *  @return WHD_SUCCESS or Error code
 */
extern uint32_t whd_wifi_stop_scan(whd_interface_t ifp);

/** Joins a Wi-Fi network
 *
 *  Scans for, associates and authenticates with a Wi-Fi network.
 *  On successful return, the system is ready to send data packets.
 *
 *  @param   ifp           Pointer to handle instance of whd interface
 *  @param   ssid          A null terminated string containing the SSID name of the network to join
 *  @param   auth_type     Authentication type
 *  @param   security_key  A byte array containing either the cleartext security key for WPA/WPA2/WPA3 secured networks
 *  @param   key_length    The length of the security_key in bytes.
 *
 *  @note    In case of WPA3/WPA2 transition mode, the security_key value is WPA3 password.
 *
 *  @return  WHD_SUCCESS   when the system is joined and ready to send data packets
 *           Error code    if an error occurred
 */
extern uint32_t whd_wifi_join(whd_interface_t ifp, const whd_ssid_t *ssid, whd_security_t auth_type,
                              const uint8_t *security_key, uint8_t key_length);

/** Joins a specific Wi-Fi network
 *
 *  Associates and authenticates with a specific Wi-Fi access point.
 *  On successful return, the system is ready to send data packets.
 *
 *  @param   ifp           Pointer to handle instance of whd interface
 *  @param   ap            A pointer to a whd_scan_result_t structure containing AP details and
 *                         set ap.channel to 0 for unspecificed channel
 *  @param   security_key  A byte array containing either the cleartext security key for WPA/WPA2
 *                         secured networks
 *  @param   key_length    The length of the security_key in bytes.
 *
 *  @return  WHD_SUCCESS   when the system is joined and ready to send data packets
 *           Error code    if an error occurred
 */
extern uint32_t whd_wifi_join_specific(whd_interface_t ifp, const whd_scan_result_t *ap, const uint8_t *security_key,
                                       uint8_t key_length);

/*  @} */

/** @addtogroup wifiutilities   WHD Wi-Fi Utility API
 *  @ingroup wifi
 *  Allows WHD to perform utility operations
 *  @{
 */

/** Set the current channel on the WLAN radio
 *
 *  @note  On most WLAN devices this will set the channel for both AP *AND* STA
 *        (since there is only one radio - it cannot be on two channels simulaneously)
 *
 *  @param   ifp           Pointer to handle instance of whd interface
 *  @param   channel       The desired channel
 *
 *  @return  WHD_SUCCESS   if the channel was successfully set
 *           Error code    if the channel was not successfully set
 */
extern uint32_t whd_wifi_set_channel(whd_interface_t ifp, uint32_t channel);

/** Get the current channel on the WLAN radio
 *
 *  @note On most WLAN devices this will get the channel for both AP *AND* STA
 *       (since there is only one radio - it cannot be on two channels simulaneously)
 *
 *  @param   ifp            Pointer to handle instance of whd interface
 *  @param   channel        Pointer to receive the current channel
 *
 *  @return  WHD_SUCCESS    if the channel was successfully retrieved
 *           Error code     if the channel was not successfully retrieved
 */
extern uint32_t whd_wifi_get_channel(whd_interface_t ifp, uint32_t *channel);

/** Set the passphrase
 *
 *  @param   ifp            Pointer to handle instance of whd interface
 *  @param   security_key   The security key (passphrase) which is to be set
 *  @param   key_length     length of the key
 *
 *  @return  WHD_SUCCESS    when the key is set
 *           Error code     if an error occurred
 */
extern uint32_t whd_wifi_set_passphrase(whd_interface_t ifp, const uint8_t *security_key, uint8_t key_length);

/** Set the SAE password
 *
 *  @param   ifp            Pointer to handle instance of whd interface
 *  @param   security_key   The security key (password) which is to be set
 *  @param   key_length     length of the key
 *
 *  @return  WHD_SUCCESS    when the key is set
 *           Error code     if an error occurred
 */
extern uint32_t whd_wifi_sae_password(whd_interface_t ifp, const uint8_t *security_key, uint8_t key_length);

/** Enable WHD internal supplicant and set WPA2 passphrase in case of WPA3/WPA2 transition mode
 *
 *  @param   ifp                Pointer to handle instance of whd interface
 *  @param   security_key_psk   The security key (passphrase) which is to be set
 *  @param   psk_length         length of the key
 *  @param   auth_type          Authentication type: @ref WHD_SECURITY_WPA3_WPA2_PSK
 *
 *  @return  WHD_SUCCESS        when the supplicant variable and wpa2 passphrase is set
 *           Error code         if an error occurred
 */
extern uint32_t whd_wifi_enable_sup_set_passphrase(whd_interface_t ifp, const uint8_t *security_key_psk,
                                                   uint8_t psk_length, whd_security_t auth_type);


/** Enable WHD internal supplicant
 *
 *  @param   ifp            Pointer to handle instance of whd interface
 *  @param   auth_type      Authentication type
 *
 *  @return  WHD_SUCCESS    when the supplicant variable is set
 *           Error code     if an error occurred
 */
extern uint32_t whd_wifi_enable_supplicant(whd_interface_t ifp, whd_security_t auth_type);

/** Retrieve the latest RSSI value
 *
 *  @param   ifp           Pointer to handle instance of whd interface
 *  @param   rssi          The location where the RSSI value will be stored
 *
 *  @return  WHD_SUCCESS   if the RSSI was successfully retrieved
 *           Error code    if the RSSI was not retrieved
 */
extern uint32_t whd_wifi_get_rssi(whd_interface_t ifp, int32_t *rssi);

/** Retrieve the associated STA's RSSI value
 *
 *  @param   ifp          : Pointer to handle instance of whd interface
 *  @param   rssi         : The location where the RSSI value will be stored
 *  @param   client_mac   : Pointer to associated client's MAC address
 *
 *  @return  WHD_SUCCESS  : if the RSSI was successfully retrieved
 *           Error code   : if the RSSI was not retrieved
 */
extern uint32_t whd_wifi_get_ap_client_rssi(whd_interface_t ifp, int32_t *rssi, const whd_mac_t *client_mac);


/* @} */

/** @addtogroup wifijoin   WHD Wi-Fi Join, Scan and Halt API
 *  @ingroup wifi
 *  Wi-Fi APIs for join, scan & leave
 *  @{
 */
/** Disassociates from a Wi-Fi network.
 *  Applicable only for STA role
 *
 *  @param   ifp           Pointer to handle instance of whd interface.
 *
 *  @return  WHD_SUCCESS   On successful disassociation from the AP
 *           Error code    If an error occurred
 */
extern uint32_t whd_wifi_leave(whd_interface_t ifp);
/* @} */

/** @addtogroup wifiutilities   WHD Wi-Fi Utility API
 *  @ingroup wifi
 *  Allows WHD to perform utility operations
 *  @{
 */

/** Retrieves the current Media Access Control (MAC) address
 *  (or Ethernet hardware address) of the 802.11 device
 *
 *  @param   ifp           Pointer to handle instance of whd interface
 *  @param   mac           Pointer to a variable that the current MAC address will be written to
 *
 *  @return  WHD_SUCCESS or Error code
 */
extern uint32_t whd_wifi_get_mac_address(whd_interface_t ifp, whd_mac_t *mac);

/** Get the BSSID of the interface
 *
 *  @param  ifp           Pointer to the whd_interface_t
 *  @param  bssid         Returns the BSSID address (mac address) if associated
 *
 *  @return WHD_SUCCESS or Error code
 */
extern uint32_t whd_wifi_get_bssid(whd_interface_t ifp, whd_mac_t *bssid);
/* @} */

/** @addtogroup wifisoftap     WHD Wi-Fi SoftAP API
 *  @ingroup wifi
 *  Wi-Fi APIs to perform SoftAP related functionalities
 *  @{
 */

/** Initialises an infrastructure WiFi network (SoftAP)
 *
 *  @param   ifp           Pointer to handle instance of whd interface
 *  @param   ssid          A null terminated string containing the SSID name of the network to join
 *  @param   auth_type     Authentication type
 *  @param   security_key  A byte array containing the cleartext security key for the network
 *  @param   key_length    The length of the security_key in bytes.
 *  @param   channel       802.11 channel number
 *
 *  @return  WHD_SUCCESS   if successfully initialises an AP
 *           Error code    if an error occurred
 */
extern uint32_t whd_wifi_init_ap(whd_interface_t ifp, whd_ssid_t *ssid, whd_security_t auth_type,
                                 const uint8_t *security_key, uint8_t key_length, uint8_t channel);

/** Start the infrastructure WiFi network (SoftAP)
 *  using the parameter set by whd_wifi_init_ap() and optionaly by whd_wifi_manage_custom_ie()
 *
 *  @return  WHD_SUCCESS   if successfully creates an AP
 *           Error code    if an error occurred
 */
extern uint32_t whd_wifi_start_ap(whd_interface_t ifp);

/** Stops an existing infrastructure WiFi network
 *
 *  @param   ifp           Pointer to handle instance of whd interface
 *
 *  @return  WHD_SUCCESS   if the AP is successfully stopped or if the AP has not yet been brought up
 *           Error code    if an error occurred
 */
extern uint32_t whd_wifi_stop_ap(whd_interface_t ifp);


/** Get the maximum number of associations supported by AP interfaces
 *
 *  @param   ifp           Pointer to handle instance of whd interface
 *  @param   max_assoc     The maximum number of associations supported by Soft AP interfaces.
 *
 *  @return  WHD_SUCCESS   if the maximum number of associated clients was successfully read
 *           WHD_ERROR     if the maximum number of associated clients was not successfully read
 */
extern uint32_t whd_wifi_ap_get_max_assoc(whd_interface_t ifp, uint32_t *max_assoc);

/** Gets the current number of active connections
 *
 *  @param   ifp                 Pointer to handle instance of whd interface
 *  @param   client_list_buffer  Buffer to store list of associated clients
 *  @param   buffer_length       Length of client list buffer
 *
 *  @return  WHD_SUCCESS         if the active connections was successfully read
 *           WHD_ERROR           if the active connections was not successfully read
 */
extern uint32_t whd_wifi_get_associated_client_list(whd_interface_t ifp, void *client_list_buffer,
                                                    uint16_t buffer_length);

/** Deauthenticates a STA which may or may not be associated to SoftAP
 *
 * @param   ifp             Pointer to handle instance of whd interface
 * @param   mac             Pointer to a variable containing the MAC address to which the deauthentication will be sent
 *                          NULL mac address will deauthenticate all the associated STAs
 *
 * @param   reason          Deauthentication reason code
 *
 * @return  WHD_SUCCESS     On successful deauthentication of the other STA
 *          WHD_ERROR       If an error occurred
 */
extern uint32_t whd_wifi_deauth_sta(whd_interface_t ifp, whd_mac_t *mac, whd_dot11_reason_code_t reason);

/** Retrieves AP information
 *
 *  @param  ifp            Pointer to handle instance of whd interface
 *  @param  ap_info        Returns a whd_bss_info_t structure containing AP details
 *  @param  security       Authentication type
 *
 *  @return WHD_SUCCESS    if the AP info was successfully retrieved
 *          Error code     if the AP info was not successfully retrieved
 */
extern uint32_t whd_wifi_get_ap_info(whd_interface_t ifp, whd_bss_info_t *ap_info, whd_security_t *security);

/** Set the beacon interval.
 *
 *  Note that the value needs to be set before ap_start in order to beacon interval to take effect.
 *
 *  @param  ifp            Pointer to handle instance of whd interface
 *  @param  interval       Beacon interval in time units (Default: 100 time units = 102.4 ms)
 *
 *  @return WHD_SUCCESS or Error code
 */
extern uint32_t whd_wifi_ap_set_beacon_interval(whd_interface_t ifp, uint16_t interval);

/** Set the DTIM interval.
 *
 *  Note that the value needs to be set before ap_start in order to DTIM interval to take effect.
 *
 *  @param  ifp            Pointer to handle instance of whd interface
 *  @param  interval       DTIM interval, in unit of beacon interval
 *
 *  @return WHD_SUCCESS or Error code
 */
extern uint32_t whd_wifi_ap_set_dtim_interval(whd_interface_t ifp, uint16_t interval);
/*  @} */


/** @addtogroup wifipowersave   WHD Wi-Fi Power Save API
 *  @ingroup wifi
 *  Wi-Fi functions for WLAN low power modes
 *  @{
 */

/** Enables powersave mode on specified interface without regard for throughput reduction
 *
 *  This function enables (legacy) 802.11 PS-Poll mode and should be used
 *  to achieve the lowest power consumption possible when the Wi-Fi device
 *  is primarily passively listening to the network
 *
 *  @param  ifp            Pointer to handle instance of whd interface
 *
 *  @return WHD_SUCCESS or Error code
 */
extern uint32_t whd_wifi_enable_powersave(whd_interface_t ifp);

/** Enables powersave mode on specified interface while attempting to maximise throughput
 *
 *
 *  Network traffic is typically bursty. Reception of a packet often means that another
 *  packet will be received shortly afterwards (and vice versa for transmit).
 *
 *  In high throughput powersave mode, rather then entering powersave mode immediately
 *  after receiving or sending a packet, the WLAN chip waits for a timeout period before
 *  returning to sleep.
 *
 *  @param   ifp                    Pointer to handle instance of whd interface
 *  @param   return_to_sleep_delay  The variable to set return to sleep delay.
 *                                 return to sleep delay must be set to a multiple of 10 and not equal to zero.
 *
 *  @return  WHD_SUCCESS            if power save mode was successfully enabled
 *           Error code             if power save mode was not successfully enabled
 *
 */
extern uint32_t whd_wifi_enable_powersave_with_throughput(whd_interface_t ifp, uint16_t return_to_sleep_delay);

/** Get powersave mode on specified interface
 *
 *  @param  ifp            Pointer to handle instance of whd interface
 *  @param  value          Value of the current powersave state
 *                          PM1_POWERSAVE_MODE, PM2_POWERSAVE_MODE, NO_POWERSAVE_MODE
 *
 *  @return WHD_SUCCESS or Error code
 */
extern uint32_t whd_wifi_get_powersave_mode(whd_interface_t ifp, uint32_t *value);

/** Disables 802.11 power save mode on specified interface
 *
 *  @param   ifp               Pointer to handle instance of whd interface
 *
 *  @return  WHD_SUCCESS       if power save mode was successfully disabled
 *           Error code        if power save mode was not successfully disabled
 *
 */
extern uint32_t whd_wifi_disable_powersave(whd_interface_t ifp);
/* @} */

/** @addtogroup wifiutilities   WHD Wi-Fi Utility API
 *  @ingroup wifi
 *  Allows WHD to perform utility operations
 *  @{
 */
/** Registers interest in a multicast address
 *
 *  Once a multicast address has been registered, all packets detected on the
 *  medium destined for that address are forwarded to the host.
 *  Otherwise they are ignored.
 *
 *  @param  ifp              Pointer to handle instance of whd interface
 *  @param  mac              Ethernet MAC address
 *
 *  @return WHD_SUCCESS      if the address was registered successfully
 *          Error code       if the address was not registered
 */
extern uint32_t whd_wifi_register_multicast_address(whd_interface_t ifp, const whd_mac_t *mac);

/** Unregisters interest in a multicast address
 *
 *  Once a multicast address has been unregistered, all packets detected on the
 *  medium destined for that address are ignored.
 *
 *  @param  ifp              Pointer to handle instance of whd interface
 *  @param  mac              Ethernet MAC address
 *
 *  @return WHD_SUCCESS      if the address was unregistered successfully
 *          Error code       if the address was not unregistered
 */
extern uint32_t whd_wifi_unregister_multicast_address(whd_interface_t ifp, const whd_mac_t *mac);

/** Sets the 802.11 powersave listen interval for a Wi-Fi client, and communicates
 *  the listen interval to the Access Point. The listen interval will be set to
 *  (listen_interval x time_unit) seconds.
 *
 *  The default value for the listen interval is 0. With the default value of 0 set,
 *  the Wi-Fi device wakes to listen for AP beacons every DTIM period.
 *
 *  If the DTIM listen interval is non-zero, the DTIM listen interval will over ride
 *  the beacon listen interval value.
 *
 *  @param  ifp              Pointer to handle instance of whd interface
 *  @param  listen_interval  The desired beacon listen interval
 *  @param  time_unit        The listen interval time unit; options are beacon period or DTIM period.
 *
 *  @return WHD_SUCCESS      If the listen interval was successfully set.
 *          Error code       If the listen interval was not successfully set.
 */
extern uint32_t whd_wifi_set_listen_interval(whd_interface_t ifp, uint8_t listen_interval,
                                             whd_listen_interval_time_unit_t time_unit);

/** Gets the current value of all beacon listen interval variables
 *
 *  @param  ifp                     Pointer to handle instance of whd interface
 *  @param  li                      Powersave listen interval values
 *                                     - listen_interval_beacon : The current value of the listen interval set as a multiple of the beacon period
 *                                     - listen_interval_dtim   : The current value of the listen interval set as a multiple of the DTIM period
 *                                     - listen_interval_assoc  : The current value of the listen interval sent to access points in an association request frame
 *
 *  @return WHD_SUCCESS             If all listen interval values are read successfully
 *          Error code              If at least one of the listen interval values are NOT read successfully
 */
extern uint32_t whd_wifi_get_listen_interval(whd_interface_t ifp, whd_listen_interval_t *li);

/** Determines if a particular interface is ready to transceive ethernet packets
 *
 *  @param     ifp                    Pointer to handle instance of whd interface
 *
 *  @return    WHD_SUCCESS            if the interface is ready to transceive ethernet packets
 *             WHD_NOTFOUND           no AP with a matching SSID was found
 *             WHD_NOT_AUTHENTICATED  Matching AP was found but it won't let you authenticate.
 *                                    This can occur if this device is in the block list on the AP.
 *             WHD_NOT_KEYED          Device has authenticated and associated but has not completed the key exchange.
 *                                    This can occur if the passphrase is incorrect.
 *             Error code             if the interface is not ready to transceive ethernet packets
 */
extern uint32_t whd_wifi_is_ready_to_transceive(whd_interface_t ifp);

/* Certification APIs */

/** Retrieve the latest STA EDCF AC parameters
 *
 *  Retrieve the latest Station (STA) interface EDCF (Enhanced Distributed
 *  Coordination Function) Access Category parameters
 *
 *  @param  ifp            Pointer to handle instance of whd interface
 *  @param  acp            The location where the array of AC parameters will be stored
 *
 *  @return  WHD_SUCCESS   if the AC Parameters were successfully retrieved
 *           Error code    if the AC Parameters were not retrieved
 */
extern uint32_t whd_wifi_get_acparams(whd_interface_t ifp, whd_edcf_ac_param_t *acp);

/* Action Frames */

/** Manage the addition and removal of custom IEs
 *
 *  @param  ifp            Pointer to handle instance of whd interface
 *  @param  action         the action to take (add or remove IE)
 *  @param  oui            the oui of the custom IE
 *  @param  subtype        the IE sub-type
 *  @param  data           a pointer to the buffer that hold the custom IE
 *  @param  length         the length of the buffer pointed to by 'data'
 *  @param  which_packets  A mask to indicate in which all packets this IE should be included. See whd_ie_packet_flag_t.
 *
 *  @return WHD_SUCCESS    if the custom IE action was successful
 *          Error code     if the custom IE action failed
 */
extern uint32_t whd_wifi_manage_custom_ie(whd_interface_t ifp, whd_custom_ie_action_t action,
                                          const uint8_t *oui, uint8_t subtype, const void *data,
                                          uint16_t length, uint16_t which_packets);

/** Send a pre-prepared action frame
 *
 *  @param  ifp            Pointer to handle instance of whd interface
 *  @param  af_params      A pointer to a pre-prepared action frame structure
 *
 *  @return WHD_SUCCESS or Error code
 */
extern uint32_t whd_wifi_send_action_frame(whd_interface_t ifp, whd_af_params_t *af_params);

/** Set coex configuration
 *
 *  @param  ifp                  Pointer to handle instance of whd interface
 *  @param  coex_config          Pointer to the structure whd_coex_config_t
 *
 *  @return WHD_SUCCESS or Error code
 */
extern uint32_t whd_wifi_set_coex_config(whd_interface_t ifp, whd_coex_config_t *coex_config);

/** Get version of Device (WLAN) Firmware
 *
 * @param[in]    ifp            : pointer to handle instance of whd interface
 * @param[out]   version        : pointer to store version #
 *
 * @return whd_result_t
 */
whd_result_t whd_arp_version(whd_interface_t ifp, uint32_t *version);

/** Get ARP Offload Peer Age from Device (WLAN)
 *    Length of time in seconds before aging out an entry in the WLAN processor ARP table.
 *
 * @param[in]    ifp            : pointer to handle instance of whd interface
 * @param[out]   seconds        : pointer to store value
 *
 * @return whd_result_t
 */
whd_result_t whd_arp_peerage_get(whd_interface_t ifp, uint32_t *seconds);

/** Set ARP Offload Peer Age in Device (WLAN)
 *    Length of time in seconds before aging out an entry in the WLAN processor ARP table.
 *
 * @param[in]    ifp            : pointer to handle instance of whd interface
 * @param[in]    seconds        : Seconds to age out IP
 *
 * @return whd_result_t
 */
whd_result_t whd_arp_peerage_set(whd_interface_t ifp, uint32_t seconds);

/** Get ARP Offload Agent Enable from Device (WLAN)
 *
 * @param[in]    ifp            : pointer to handle instance of whd interface
 * @param[out]   agent_enable   : pointer to store value
 *
 * @return whd_result_t
 */
whd_result_t whd_arp_arpoe_get(whd_interface_t ifp, uint32_t *agent_enable);

/** Set ARP Offload Agent Enable in Device (WLAN)
 *    Set Enable/Disable of ARP Offload Agent
 *
 * @param[in]    ifp            : pointer to handle instance of whd interface
 * @param[in]    agent_enable   : Enable=1 / Disable=0
 *
 * @return whd_result_t
 */
whd_result_t whd_arp_arpoe_set(whd_interface_t ifp, uint32_t agent_enable);

/** Clear ARP Offload cache in Device (WLAN)
 *
 * @param[in]    ifp            : pointer to handle instance of whd interface
 *
 * @return whd_result_t
 */
whd_result_t whd_arp_cache_clear(whd_interface_t ifp);

/** Get ARP Offload Feature Flags from Device (WLAN)
 *
 * @param[in]    ifp        : pointer to handle instance of whd interface
 * @param[out]   features   : ptr to store currently set features - bit flags CY_ARP_OL_AGENT_ENABLE, etc.
 *                            ARL_OL_AGENT | ARL_OL_SNOOP | ARP_OL_HOST_AUTO_REPLY | ARP_OL_PEER_AUTO_REPLY
 *
 * @return whd_result_t
 */
whd_result_t whd_arp_features_get(whd_interface_t ifp, uint32_t *features);

/** Set ARP Offload Feature Flags in Device (WLAN)
 *
 * @param[in]    ifp        : pointer to handle instance of whd interface
 * @param[in]    features   : features to set value (you can OR ('|') multiple flags) CY_ARP_OL_AGENT_ENABLE, etc.
 *                           ARL_OL_AGENT | ARL_OL_SNOOP | ARP_OL_HOST_AUTO_REPLY | ARP_OL_PEER_AUTO_REPLY
 *
 * @return whd_result_t
 */
whd_result_t whd_arp_features_set(whd_interface_t ifp, uint32_t features);

/** Print ARP Offload Feature Flags in Human readable form to console
 *
 * @param[in]    features   : feature flags to set (you can OR '|' multiple flags) CY_ARP_OL_AGENT_ENABLE, etc.
 *                            ARL_OL_AGENT | ARL_OL_SNOOP | ARP_OL_HOST_AUTO_REPLY | ARP_OL_PEER_AUTO_REPLY
 * @param[in]    title      : Optional: Title for output (NULL == no title)
 *
 * @return whd_result_t
 */
whd_result_t whd_arp_features_print(uint32_t features, const char *title);

/** Add ARP Offload Host IP Identifier(s) to HostIP List to Device (WLAN)
 *
 * @param[in]    ifp            : pointer to handle instance of whd interface
 * @param[in]    host_ipv4_list : pointer to host_ip data (IPv4, 1 uint32_t per ip addr)
 * @param[in]    count          : Number of array elements in host_ip
 *
 * @return whd_result_t
 */
whd_result_t whd_arp_hostip_list_add(whd_interface_t ifp, uint32_t *host_ipv4_list, uint32_t count);

/** Add One ARP Offload Host IP Identifier to HostIP List (mbed-style IP string) to Device (WLAN)
 *
 * @param[in]    ifp        : pointer to handle instance of whd interface
 * @param[in]    ip_addr    : pointer to ip string (as returned from mbedos calls)
 *
 * @return whd_result_t
 */
whd_result_t whd_arp_hostip_list_add_string(whd_interface_t ifp, const char *ip_addr);

/** Clear One ARP Offload Host IP Identifier from Host IP List in Device (WLAN)
 *
 * @param[in]    ifp        : pointer to handle instance of whd interface
 * @param[in]    ipv4_addr  : ip addr expressed as a uint32_t
 *
 * @return whd_result_t
 */
whd_result_t whd_arp_hostip_list_clear_id(whd_interface_t ifp, uint32_t ipv4_addr);

/** Clear One ARP Offload Host IP Identifier from Host IP List (mbed-style IP string) in Device (WLAN)
 *
 * @param[in]    ifp        : pointer to handle instance of whd interface
 * @param[in]    ip_addr    : pointer to ip string (as returned from mbedos calls)
 *
 * @return whd_result_t
 */
whd_result_t whd_arp_hostip_list_clear_id_string(whd_interface_t ifp, const char *ip_addr);

/** Clear all ARP Offload Host IP Identifier List
 *
 * @param[in]    ifp        : pointer to handle instance of whd interface
 *
 * @return whd_result_t
 */
whd_result_t whd_arp_hostip_list_clear(whd_interface_t ifp);

/** Get ARP Offload Host IP Identifiers from Device (WLAN)
 *
 * @param[in]    ifp        : pointer to handle instance of whd interface
 * @param[in]    count          : Number of array elements in host_ip
 * @param[out]   host_ipv4_list : Pointer to structure array to store host_ip data
 * @param[out]   filled         : Number of array elements filled by this routine
 *
 * @return whd_result_t
 */
whd_result_t whd_arp_hostip_list_get(whd_interface_t ifp, uint32_t count, uint32_t *host_ipv4_list, uint32_t *filled);

/** Clear ARP Offload statistics in Device (WLAN)
 *
 * @param[in]    ifp        : pointer to handle instance of whd interface
 *
 * @return whd_result_t
 */
whd_result_t whd_arp_stats_clear(whd_interface_t ifp);

/** Get ARP Offload statistics from Device (WLAN)
 *
 * @param[in]    ifp        : pointer to handle instance of whd interface
 * @param[out]   stats      : Ptr to store statistics whd_arp_stats_t
 *
 * @return whd_result_t
 */
whd_result_t whd_arp_stats_get(whd_interface_t ifp, whd_arp_stats_t *stats);

/** Print ARP Offload statistics
 *  NOTE: call whd_arp_stats_get(), then print them using this function.
 *
 * @param[in]    arp_stats  : Ptr to ARP statistics structure
 * @param[in]    title      : Optional: Title for output (NULL == no title)
 *
 * @return whd_result_t
 */
whd_result_t whd_arp_stats_print(whd_arp_stats_t *arp_stats, const char *title);

/** A filter must be added (e.g. created) before it can be enabled.
 * @param[in]    ifp        : pointer to handle instance of whd interface
 * @param[in]    settings   : Ptr to filter settings @ref whd_packet_filter_t
 * @return whd_result_t
 */
whd_result_t whd_pf_add_packet_filter(whd_interface_t ifp, const whd_packet_filter_t *settings);

/** Remove a previously added filter.
 * @param[in]    ifp        : pointer to handle instance of whd interface
 * @param[in]    filter_id  : filter to remove
 * @return whd_result_t
 */
whd_result_t whd_pf_remove_packet_filter(whd_interface_t ifp, uint8_t filter_id);

/** After a filter has been added it can be enabled or disabled as needed.
 * @param[in]    ifp        : pointer to handle instance of whd interface
 * @param[in]    filter_id  : filter to enable
 * @return whd_result_t
 */
whd_result_t whd_pf_enable_packet_filter(whd_interface_t ifp, uint8_t filter_id);

/** After a filter has been added it can be enabled or disabled as needed.
 * @param[in]    ifp        : pointer to handle instance of whd interface
 * @param[in]    filter_id  : filter to disable
 * @return whd_result_t
 */
whd_result_t whd_pf_disable_packet_filter(whd_interface_t ifp, uint8_t filter_id);

/** After a filter has been added it can be enabled or disabled as needed.
 * @param[in]    ifp        : pointer to handle instance of whd interface
 * @param[in]    filter_id  : filter to disable/enable
 * @param[in]    enable     : Enable/Disable Flag
 * @return whd_result_t
 */
whd_result_t whd_wifi_toggle_packet_filter(whd_interface_t ifp, uint8_t filter_id, whd_bool_t enable);

/** Filters are implemented in WLAN subsystem as a bit pattern and matching bit mask that
 *  are applied to incoming packets.  This API retrieves the pattern and mask.
 * @param[in]    ifp        : pointer to handle instance of whd interface
 * @param[in]    filter_id  : which filter to retrieve
 * @param[in]    max_size   : size of both mask and pattern buffers
 * @param[out]   mask       : mask for this filter
 * @param[out]   pattern    : pattern for this filter
 * @param[out]   size_out   : length of both mask and pattern
 * @return whd_result_t
 */
whd_result_t whd_pf_get_packet_filter_mask_and_pattern(whd_interface_t ifp, uint8_t filter_id, uint32_t max_size,
                                                       uint8_t *mask,
                                                       uint8_t *pattern, uint32_t *size_out);

/** Clear the packet filter stats associated with a filter id
 * @param[in]    ifp        : pointer to handle instance of whd interface
 * @param[in]    filter_id  : which filter
 * @return whd_result_t
 */
whd_result_t whd_wifi_clear_packet_filter_stats(whd_interface_t ifp, uint32_t filter_id);

/** Return the stats associated with a filter
 * @param[in]    ifp        : pointer to handle instance of whd interface
 * @param[in]    filter_id  : which filter
 * @param[out]   stats      : Ptr to store statistics wl_pkt_filter_stats_t
 * @return whd_result_t
 */
whd_result_t whd_pf_get_packet_filter_stats(whd_interface_t ifp, uint8_t filter_id, whd_pkt_filter_stats_t *stats);

/** Set/Get TKO retry & interval parameters
 * @param[in]    ifp            : Pointer to handle instance of whd interface
 * @param[in]    whd_tko_retry  : whd retry & interval parameters structure
 * @param[in]    set            : Set/Get Flag
 * @return whd_result_t
 */
whd_result_t whd_tko_param(whd_interface_t ifp, whd_tko_retry_t *whd_tko_retry, int set);

/** Return the tko status for all indexes
 * @param[in]    ifp        : Pointer to handle instance of whd interface
 * @param[out]   tko_status : Ptr to store tko_status
 * @return whd_result_t
 */
whd_result_t whd_tko_get_status(whd_interface_t ifp, whd_tko_status_t *tko_status);

/** Return the stats associated with a filter
 * @param[in]    ifp        : Pointer to handle instance of whd interface
 * @param[out]   max        : returns Max TCP connections supported by WLAN Firmware
 * @return whd_result_t
 */
whd_result_t whd_tko_max_assoc(whd_interface_t ifp, uint8_t *max);

/** Return the stats associated with a filter
 * @param[in]    ifp          : Pointer to handle instance of whd interface
 * @param[in]    index        : index for TCP offload connection
 * @param[out]   whd_connect  : tko_connect structure buffer from Firmware
 * @param[in]    buflen       : Buffer given for tko_connect
 * @return whd_result_t
 */
whd_result_t whd_tko_get_FW_connect(whd_interface_t ifp, uint8_t index, whd_tko_connect_t *whd_connect,
                                    uint16_t buflen);

/** Return the stats associated with a filter
 * @param[in]    ifp        : Pointer to handle instance of whd interface
 * @param[in]    enable     : Enable/Disable TCP Keepalive offload
 * @return whd_result_t
 */
whd_result_t whd_tko_toggle(whd_interface_t ifp, whd_bool_t enable);


/* @} */

/** @addtogroup wifiioctl   WHD Wi-Fi IOCTL Set/Get API
 *  @ingroup wifi
 *  Set and get IOCTL values
 *  @{
 */
/** Sends an IOCTL command - CDC_SET IOCTL value
 *
 *  @param  ifp            Pointer to handle instance of whd interface
 *  @param  ioctl          CDC_SET - To set the I/O control
 *  @param  value          Data value to be sent
 *
 *  @return WHD_SUCCESS or Error code
 */
extern uint32_t whd_wifi_set_ioctl_value(whd_interface_t ifp, uint32_t ioctl, uint32_t value);

/** Sends an IOCTL command - CDC_GET IOCTL value
 *
 *  @param  ifp            Pointer to handle instance of whd interface
 *  @param  ioctl          CDC_GET - To get the I/O control
 *  @param  value          Pointer to receive the data value
 *
 *  @return WHD_SUCCESS or Error code
 */
extern uint32_t whd_wifi_get_ioctl_value(whd_interface_t ifp, uint32_t ioctl, uint32_t *value);

/** Sends an IOCTL command - CDC_SET IOCTL buffer
 *
 *  @param  ifp            Pointer to handle instance of whd interface
 *  @param  ioctl          CDC_SET - To set the I/O control
 *  @param  buffer         Handle for a packet buffer containing the data value to be sent.
 *  @param  buffer_length  Length of buffer
 *
 *  @return WHD_SUCCESS or Error code
 */
extern uint32_t whd_wifi_set_ioctl_buffer(whd_interface_t ifp, uint32_t ioctl, void *buffer, uint16_t buffer_length);

/** Sends an IOCTL command - CDC_GET IOCTL buffer
 *
 *  @param  ifp           Pointer to handle instance of whd interface
 *  @param  ioctl         CDC_GET - To get the I/O control
 *  @param  out_buffer    Pointer to receive the handle for the packet buffer containing the response data value received
 *  @param  out_length    Length of out_buffer
 *
 *  @return WHD_SUCCESS or Error code
 */
extern uint32_t whd_wifi_get_ioctl_buffer(whd_interface_t ifp, uint32_t ioctl, uint8_t *out_buffer,
                                          uint16_t out_length);

/** Sends an IOVAR command
 *
 *  @param  ifp           Pointer to handle instance of whd interface
 *  @param  iovar_name    SDPCM_GET - To get the I/O Variable
 *  @param  param         Paramater to be passed for the IOVAR
 *  @param  paramlen      Paramter length
 *  @param  out_buffer    Pointer to receive the handle for the packet buffer containing the response data value received
 *  @param  out_length    Length of out_buffer
 *
 *  @return WHD_SUCCESS or Error code
 */
extern uint32_t whd_wifi_get_iovar_buffer_with_param(whd_interface_t ifp, const char *iovar_name, void *param,
                                                     uint32_t paramlen, uint8_t *out_buffer, uint32_t out_length);

/* @} */

/** @addtogroup dbg  WHD Wi-Fi Debug API
 *  @ingroup wifi
 *  WHD APIs which allows debugging like, printing whd log information, getting whd stats, etc.
 *  @{
 */
/** Retrieves the WLAN firmware version
 *
 *  @param  ifp            Pointer to handle instance of whd interface
 *  @param  version        Pointer to a buffer that version information will be written to
 *  @param  length         Length of the buffer
 *
 *  @return WHD_SUCCESS or Error code
 */
extern uint32_t whd_wifi_get_wifi_version(whd_interface_t ifp, char *version, uint8_t length);

/** Retrieves the WLAN CLM version
 *
 *  @param  ifp            Pointer to handle instance of whd interface
 *  @param  version        Pointer to a buffer that version information will be written to
 *  @param  length         Length of the buffer
 *
 *  @return WHD_SUCCESS or Error code
 */
extern uint32_t whd_wifi_get_clm_version(whd_interface_t ifp, char *version, uint8_t length);

/** To print whd log information
 *
 *  @param  whd_drv        Pointer to handle instance of the driver
 *  @param  buffer         Buffer to store read log results
 *  @param  buffer_size    Variable to store size of the buffer
 *
 *  @return WHD_SUCCESS or Error code
 */
extern uint32_t whd_wifi_read_wlan_log(whd_driver_t whd_drv, char *buffer, uint32_t buffer_size);

/** To print whd log information
 *
 *  @param  whd_drv        Pointer to handle instance of the driver
 *
 *  @return WHD_SUCCESS or Error code
 */
extern uint32_t whd_wifi_print_whd_log(whd_driver_t whd_drv);

/** Retrieves the ifidx from interface pointer.
 *  ifidx is a unique value and be used to identify a instance of tcp/ip stack
 *
 *  @param  ifp           Pointer to the whd_interface_t
 *  @param  ifidx         Pointer to ifidx
 *
 *  @return WHD_SUCCESS or Error code
 */
extern uint32_t whd_network_get_ifidx_from_ifp(whd_interface_t ifp, uint8_t *ifidx);

/** Retrieves the bsscfgidx from interface pointer.
 *
 *  Can be used to send IOCTL with requires bsscfgidx
 *
 *  @param  ifp           Pointer to handle instance of whd interface
 *  @param  bsscfgidx     Pointer to bsscfgidx
 *
 *  @return WHD_SUCCESS or Error code
 */
extern uint32_t whd_network_get_bsscfgidx_from_ifp(whd_interface_t ifp, uint8_t *bsscfgidx);


/** Retrives the bss info
 *
 *  @param  ifp                  Pointer to handle instance of whd interface
 *  @param  bi                   A pointer to the structure wl_bss_info_t
 *
 *  @return WHD_SUCCESS or Error code
 */
extern uint32_t whd_wifi_get_bss_info(whd_interface_t ifp, wl_bss_info_t *bi);

/** Prints WHD stats
 *
 *  @param  whd_drv              Pointer to handle instance of the driver
 *  @param  reset_after_print    Bool variable to decide if whd_stats to be reset
 *
 *  @return WHD_SUCCESS or Error code
 */
extern uint32_t whd_print_stats(whd_driver_t whd_drv, whd_bool_t reset_after_print);

/* @} */
/* @} */

#ifdef __cplusplus
} /* extern "C" */
#endif
#endif /* ifndef INCLUDED_WHD_WIFI_H */

