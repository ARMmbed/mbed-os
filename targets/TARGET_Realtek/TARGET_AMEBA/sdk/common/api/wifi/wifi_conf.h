/******************************************************************************
 * Copyright (c) 2013-2016 Realtek Semiconductor Corp.
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
 *
 ******************************************************************************
 * @file    wifi_conf.h
 * @author
 * @version
 * @brief   This file provides user interface for Wi-Fi station and AP mode configuration 
 *             base on the functionalities provided by Realtek Wi-Fi driver.
 ******************************************************************************
 */
#ifndef __WIFI_API_H
#define __WIFI_API_H

#include "osdep_service.h"
#include "wifi_constants.h"   
#include "wifi_structures.h"  
#include "wifi_util.h"    
#include "wifi_ind.h"    
#ifndef CONFIG_MBED_ENABLED  
#include <platform/platform_stdlib.h>
#endif

#ifdef __cplusplus
  extern "C" {
#endif

/******************************************************
 *                    Macros
 ******************************************************/

#define RTW_ENABLE_API_INFO

#ifdef RTW_ENABLE_API_INFO
    #define RTW_API_INFO(args) do {printf args;} while(0)
#else
    #define RTW_API_INFO(args)
#endif

#define MAC_ARG(x) ((u8*)(x))[0],((u8*)(x))[1],((u8*)(x))[2],((u8*)(x))[3],((u8*)(x))[4],((u8*)(x))[5]
#define CMP_MAC( a, b )  (((a[0])==(b[0]))&& \
                          ((a[1])==(b[1]))&& \
                          ((a[2])==(b[2]))&& \
                          ((a[3])==(b[3]))&& \
                          ((a[4])==(b[4]))&& \
                          ((a[5])==(b[5])))

/******************************************************
 *                    Constants
 ******************************************************/
#define SCAN_LONGEST_WAIT_TIME  (4500)


#define MAC_FMT "%02x:%02x:%02x:%02x:%02x:%02x"

#define PSCAN_ENABLE 0x01      //enable for partial channel scan
#define PSCAN_FAST_SURVEY 0x02 //set to select scan time to FAST_SURVEY_TO, otherwise SURVEY_TO
#define PSCAN_SIMPLE_CONFIG   0x04 //set to select scan time to FAST_SURVEY_TO and resend probe request

/******************************************************
 *                 Type Definitions
 ******************************************************/

/** Scan result callback function pointer type
 *
 * @param result_ptr  : A pointer to the pointer that indicates where to put the next scan result
 * @param user_data   : User provided data
 */
typedef void (*rtw_scan_result_callback_t)( rtw_scan_result_t** result_ptr, void* user_data );
typedef rtw_result_t (*rtw_scan_result_handler_t)( rtw_scan_handler_result_t* malloced_scan_result );

/******************************************************
 *                    Structures
 ******************************************************/
typedef struct {
	char *buf;
	int buf_len;
} scan_buf_arg;

/******************************************************
 *                    Structures
 ******************************************************/
typedef struct internal_scan_handler{
	rtw_scan_result_t** pap_details;
	rtw_scan_result_t * ap_details;
	int	scan_cnt;
	rtw_bool_t	scan_complete;
	unsigned char	max_ap_size;
	rtw_scan_result_handler_t gscan_result_handler;
#if SCAN_USE_SEMAPHORE
	void *scan_semaphore;
#else
	int 	scan_running;
#endif
	void*	user_data;
	unsigned int	scan_start_time;
} internal_scan_handler_t;

typedef struct {
    rtw_network_info_t	network_info;
    void *join_sema;
} internal_join_result_t;

/******************************************************
 *               Function Declarations
 ******************************************************/
/**
 * @brief  Initialize Realtek WiFi API System.
 * 			- Initialize the required parts of the software platform.
 *   			i.e. worker, event registering, semaphore, etc.
 * 			- Initialize the RTW API thread which handles the asynchronous event.
 * @return RTW_SUCCESS if initialization is successful, RTW_ERROR otherwise
 */
int wifi_manager_init(void);

/**
 * @brief  Join a Wi-Fi network.
 * 		Scan for, associate and authenticate with a Wi-Fi network.
 *		On successful return, the system is ready to send data packets.
 *
 * @param[in]  ssid: A null terminated string containing the SSID name of the network to join.
 * @param[in]  security_type: Authentication type:
 *                         - RTW_SECURITY_OPEN           - Open Security
 *                         - RTW_SECURITY_WEP_PSK        - WEP Security with open authentication
 *                         - RTW_SECURITY_WEP_SHARED     - WEP Security with shared authentication
 *                         - RTW_SECURITY_WPA_TKIP_PSK   - WPA Security
 *                         - RTW_SECURITY_WPA2_AES_PSK   - WPA2 Security using AES cipher
 *                         - RTW_SECURITY_WPA2_TKIP_PSK  - WPA2 Security using TKIP cipher
 *                         - RTW_SECURITY_WPA2_MIXED_PSK - WPA2 Security using AES and/or TKIP ciphers
 * @param[in]  password: A byte array containing either the cleartext security key for WPA/WPA2
 *  						 secured networks, or a pointer to an array of rtw_wep_key_t
 *  						 structures for WEP secured networks.
 * @param[in]  ssid_len: The length of the SSID in bytes.
 * @param[in]  password_len: The length of the security_key in bytes.
 * @param[in]  key_id: The index of the wep key (0, 1, 2, or 3). If not using it, leave it with value -1.
 * @param[in]  semaphore: A user provided semaphore that is flagged when the join is complete. If not using it, leave it with NULL value.
 * @return  RTW_SUCCESS: when the system is joined and ready to send data packets.
 * @return  RTW_ERROR: if an error occurred.
 * @note  Please make sure the Wi-Fi is enabled before invoking this function. (@ref wifi_on())
 */
int wifi_connect(
	char 				*ssid,
	rtw_security_t	security_type,
	char 				*password,
	int 				ssid_len,
	int 				password_len,
	int 				key_id,
	void 				*semaphore);

/**
 * @brief  Join a Wi-Fi network with specified BSSID.
 * 		Scan for, associate and authenticate with a Wi-Fi network.
 *		On successful return, the system is ready to send data packets.
 * @param[in]  bssid: The specified BSSID to connect.
 * @param[in]  ssid: A null terminated string containing the SSID name of the network to join.
 * @param[in]  security_type: Authentication type:
 *                         - RTW_SECURITY_OPEN           - Open Security
 *                         - RTW_SECURITY_WEP_PSK        - WEP Security with open authentication
 *                         - RTW_SECURITY_WEP_SHARED     - WEP Security with shared authentication
 *                         - RTW_SECURITY_WPA_TKIP_PSK   - WPA Security
 *                         - RTW_SECURITY_WPA2_AES_PSK   - WPA2 Security using AES cipher
 *                         - RTW_SECURITY_WPA2_TKIP_PSK  - WPA2 Security using TKIP cipher
 *                         - RTW_SECURITY_WPA2_MIXED_PSK - WPA2 Security using AES and/or TKIP ciphers
 * @param[in]  password: A byte array containing either the cleartext security key for WPA/WPA2
 *  						 secured networks, or a pointer to an array of rtw_wep_key_t
 *  						 structures for WEP secured networks.
 * @param[in]  ssid_len: The length of the SSID in bytes.
 * @param[in]  password_len: The length of the security_key in bytes.
 * @param[in]  key_id: The index of the wep key.
 * @param[in]  semaphore: A user provided semaphore that is flagged when the join is complete.
 * @return  RTW_SUCCESS: when the system is joined and ready to send data packets.
 * @return  RTW_ERROR: if an error occurred.
 * @note  Please make sure the Wi-Fi is enabled before invoking this function. (@ref wifi_on())
 * @note  The difference between @ref wifi_connect_bssid() and @ref wifi_connect() is that BSSID has higher priority as the basis of connection in @ref wifi_connect_bssid.
 */
int wifi_connect_bssid(
	unsigned char 		bssid[ETH_ALEN],
	char 				*ssid,
	rtw_security_t	security_type,
	char 				*password,
	int 				bssid_len,
	int 				ssid_len,
	int 				password_len,
	int 				key_id,
	void 				*semaphore);

/**
  * @brief  Disassociates from current Wi-Fi network.
  * @param  None
  * @return  RTW_SUCCESS: On successful disassociation from the AP.
  * @return  RTW_ERROR: If an error occurred.
  */
int wifi_disconnect(void);

/**
* @brief  Check if Wi-Fi has connected to AP before dhcp.
* @param  None
* @return  RTW_SUCCESS: If conneced.
* @return  RTW_ERROR: If not connect.
*/
int wifi_is_connected_to_ap(void);

/**
  * @brief  Check if the specified interface is up.
  * @param[in]  interface: The interface can be set as RTW_STA_INTERFACE or RTW_AP_INTERFACE. (@ref rtw_interface_t)
  * @return  If the function succeeds, the return value is 1. Otherwise, return 0.
  */
int wifi_is_up(rtw_interface_t interface);

/** Determines if a particular interface is ready to transceive ethernet packets
 *
 * @param     Radio interface to check, options are 
 *  				RTW_STA_INTERFACE, RTW_AP_INTERFACE
 * @return    RTW_SUCCESS  : if the interface is ready to 
 *  		  transceive ethernet packets
 * @return    RTW_NOTFOUND : no AP with a matching SSID was 
 *  		  found
 * @return    RTW_NOT_AUTHENTICATED: a matching AP was found but
 *  								   it won't let you
 *  								   authenticate. This can
 *  								   occur if this device is
 *  								   in the block list on the
 *  								   AP.
 * @return    RTW_NOT_KEYED: the device has authenticated and 
 *  						   associated but has not completed
 *  						   the key exchange. This can occur
 *  						   if the passphrase is incorrect.
 * @return    RTW_ERROR    : if the interface is not ready to 
 *  		  transceive ethernet packets
 */
int wifi_is_ready_to_transceive(rtw_interface_t interface);

/**
 * @brief  This function sets the current Media Access Control (MAC) address of the 802.11 device.
 * @param[in] mac: Wi-Fi MAC address.
 * @return    RTW_SUCCESS or RTW_ERROR
 */
int wifi_set_mac_address(char * mac);

/**
 * @brief  Retrieves the current Media Access Control (MAC) address
 *			(or Ethernet hardware address) of the 802.11 device.
 * @param[in]  mac: Point to the result of the mac address will be get.
 * @return    RTW_SUCCESS or RTW_ERROR
 */
int wifi_get_mac_address(char * mac);

/**
 * @brief Enable Wi-Fi powersave mode.
 * @param  None
 * @return RTW_SUCCESS or RTW_ERROR.
 */
int wifi_enable_powersave(void);

/**
 * @brief Disable Wi-Fi powersave mode.
 * @param  None
 * @return RTW_SUCCESS or RTW_ERROR.
 */
int wifi_disable_powersave(void);

/** Gets the tx power in index units
 *
 * @param dbm : The variable to receive the tx power in index.
 *
 * @return  RTW_SUCCESS : if successful
 *          RTW_ERROR   : if not successful
 */
int wifi_get_txpower(int *poweridx);

/**
 * @brief  Set the tx power in index units.
 * @param[in] poweridx: The desired tx power in index.
 * @return  RTW_SUCCESS: if tx power is successfully set
 * @return  RTW_ERROR: if tx power is not successfully set
 */
int wifi_set_txpower(int poweridx);

/**
 * @brief  Get the associated clients with SoftAP.
 * @param[out]  client_list_buffer: The location where the client list will be stored.
 * @param[in]  buffer_length: The buffer length.
 * @return  RTW_SUCCESS: The result is successfully got.
 * @return  RTW_ERROR: The result is not successfully got.
 */
int wifi_get_associated_client_list(void * client_list_buffer, unsigned short buffer_length);

/**
 * @brief  Get the SoftAP information.
 * @param[out]  ap_info: The location where the AP info will be stored.
 * @param[out]  security: The security type.
 * @return  RTW_SUCCESS: The result is successfully got.
 * @return  RTW_ERROR: The result is not successfully got.
 */
int wifi_get_ap_info(rtw_bss_info_t * ap_info, rtw_security_t* security);

/**
 * @brief  Set the country code to driver to determine the channel set.
 * @param[in]  country_code: Specify the country code.
 * @return  RTW_SUCCESS: If result is successfully set.
 * @return  RTW_ERROR: If result is not successfully set.
 */
int wifi_set_country(rtw_country_code_t country_code);

/**
 * @brief  Retrieve the latest RSSI value.
 * @param[out]  pRSSI: Points to the integer to store the RSSI value gotten from driver.
 * @return  RTW_SUCCESS: If the RSSI is succesfully retrieved.
 * @return  RTW_ERROR: If the RSSI is not retrieved.
 */
int wifi_get_rssi(int *pRSSI);

/**
 * @brief  Set the listening channel for promiscuous mode.
 * @param[in]  channel: The desired channel.
 * @return  RTW_SUCCESS: If the channel is successfully set.
 * @return  RTW_ERROR: If the channel is not successfully set.
 * @note  Do NOT need to call this function for STA mode wifi driver, since it will determine the channel from received beacon.
 */
int wifi_set_channel(int channel);

/**
 * @brief  Get the current channel on STA interface.
 * @param[out] channel: A pointer to the variable where the 
 *  				channel value will be written
 * @return  RTW_SUCCESS: If the channel is successfully read.
 * @return  RTW_ERROR: If the channel is not successfully read.
 */
int wifi_get_channel(int *channel);

/**
 * @brief  Register interest in a multicast address.\n
 * 		Once a multicast address has been registered, all packets detected on the
 * 		medium destined for that address are forwarded to the host.
 * 		Otherwise they are ignored.
 * @param[in] mac: Ethernet MAC address
 * @return  RTW_SUCCESS: If the address is registered successfully.
 * @return  RTW_ERROR: If the address is not registered.
 */
int wifi_register_multicast_address(rtw_mac_t *mac);

/** 
 * @brief  Unregister interest in a multicast address.\n
 * 		Once a multicast address has been unregistered, all packets detected on the
 * 		medium destined for that address are ignored.
 * @param[in] mac: Ethernet MAC address
 * @return  RTW_SUCCESS: If the address is unregistered successfully.
 * @return  RTW_ERROR: If the address is not unregistered.
 */
int wifi_unregister_multicast_address(rtw_mac_t *mac);

/**
 * @brief  Disable the adaptivity mode.
 * @param  None
 * @return  If the function succeeds, the return value is 0.
 */
void wifi_set_mib(void);

/**
 * @brief  Enable Wi-Fi RF.
 * @param  None
 * @return  If the function succeeds, the return value is 0.
 * @note  The difference between @ref wifi_rf_on() and @ref wifi_on() is that @ref wifi_rf_on() simply enable RF HAL, it does not enable the driver or allocate memory.
 */
int wifi_rf_on(void);

/**
 * @brief  Disable Wi-Fi RF.
 * @param  None
 * @return  If the function succeeds, the return value is 0.
 * @note  The difference between @ref wifi_rf_off() and @ref wifi_off() is that @ref wifi_rf_off() simply disable RF HAL, the driver and used heap memory will NOT be released.
 */
int wifi_rf_off(void);

/**
 * @brief  Enable Wi-Fi.
 * - Bring the Wireless interface "Up"
 * - Initialize the driver thread which arbitrates access
 *   to the SDIO/SPI bus
 *
 * @param[in]  mode: Decide to enable WiFi in which mode. The optional modes are enumerated in @ref rtw_mode_t.
 * @return  RTW_SUCCESS: if the WiFi chip was initialized successfully.
 * @return  RTW_ERROR: if the WiFi chip was not initialized successfully.
 */
int wifi_on(rtw_mode_t mode);

/**
 * @brief  Disable Wi-Fi.
 *  
 * @param  None
 * @return  RTW_SUCCESS: if deinitialization is successful.
 * @return  RTW_ERROR: otherwise.
 */
int wifi_off(void);

/**
 * Turn off the Wi-Fi device
 *
 * - Bring the Wireless interface "Down"
 * - De-Initialises the driver thread which arbitrates access
 *   to the SDIO/SPI bus
 *
 * @return RTW_SUCCESS if deinitialization is successful, 
 *  	   RTW_ERROR otherwise
 */
int wifi_off_fastly(void);

/**
 * @brief  Set IPS/LPS mode.
 * @param[in] ips_mode: The desired IPS mode. It becomes effective when wlan enter ips.\n
 *		@ref ips_mode is inactive power save mode. Wi-Fi automatically turns RF off if it is not associated to AP. Set 1 to enable inactive power save mode.
 * @param[in] lps_mode: The desired LPS mode. It becomes effective when wlan enter lps.\n
 *		@ref lps_mode is leisure power save mode. Wi-Fi automatically turns RF off during the association to AP is traffic is not busy while it also automatically turns RF on to listen to beacon. Set 1 to enable leisure power save mode.
 * @return  RTW_SUCCESS if setting LPS mode successful.
 * @return  RTW_ERROR otherwise.
 */
 
int wifi_set_power_mode(unsigned char ips_mode, unsigned char lps_mode);

/**
 * Set TDMA parameters
 *
 * @param[in] slot_period  : We separate TBTT into 2 or 3 slots.
 *                           If we separate TBTT into 2 slots, then slot_period should be larger or equal to 50ms.
 *                           It means 2 slot period is
 *                               slot_period, 100-slot_period
 *                           If we separate TBTT into 3 slots, then slot_period should be less or equal to 33ms.
 *                           It means 3 slot period is
 *                               100 - 2 * slot_period, slot_period, slot_period
 * @param[in] rfon_period_len_1: rf on period of slot 1
 * @param[in] rfon_period_len_2: rf on period of slot 2
 * @param[in] rfon_period_len_3: rf on period of slot 3
 *
 * @return RTW_SUCCESS if setting TDMA parameters successful
 *         RTW_ERROR otherwise
 */
int wifi_set_tdma_param(unsigned char slot_period, unsigned char rfon_period_len_1, unsigned char rfon_period_len_2, unsigned char rfon_period_len_3);

/**
 * @brief Set LPS DTIM.
 * @param[in] dtim: In LPS, the package can be buffered at AP side.
 *                    STA leave LPS until dtim count of packages buffered at AP side.
 * @return  RTW_SUCCESS if setting LPS dtim successful.
 * @return  RTW_ERROR otherwise
 */
int wifi_set_lps_dtim(unsigned char dtim);

/**
 * @brief  Get LPS DTIM.
 * @param[out] dtim: In LPS, the package can be buffered at AP side.
 *                    STA leave LPS until dtim count of packages buffered at AP side.
 * @return  RTW_SUCCESS if getting LPS dtim successful.
 * @return  RTW_ERROR otherwise.
 */
int wifi_get_lps_dtim(unsigned char *dtim);

/**
 * @brief  Trigger Wi-Fi driver to start an infrastructure Wi-Fi network.
 * @warning If a STA interface is active when this function is called, the softAP will
 *          start on the same channel as the STA. It will NOT use the channel provided!
 * @param[in]  ssid: A null terminated string containing the SSID name of the network.
 * @param[in]  security_type: 
 *                         - RTW_SECURITY_OPEN           - Open Security
 *                         - RTW_SECURITY_WPA_TKIP_PSK   - WPA Security
 *                         - RTW_SECURITY_WPA2_AES_PSK   - WPA2 Security using AES cipher
 *                         - RTW_SECURITY_WPA2_MIXED_PSK - WPA2 Security using AES and/or TKIP ciphers
 *                         - WEP security is NOT IMPLEMENTED. It is NOT SECURE!
 * @param[in]  password: A byte array containing the cleartext security key for the network.
 * @param[in]  ssid_len: The length of the SSID in bytes.
 * @param[in]  password_len: The length of the security_key in bytes.
 * @param[in]  channel: 802.11 channel number.
 * @return  RTW_SUCCESS: If successfully creates an AP.
 * @return  RTW_ERROR: If an error occurred.
 * @note  Please make sure the Wi-Fi is enabled before invoking this function. (@ref wifi_on())
 */
int wifi_start_ap(
	char 				*ssid,
	rtw_security_t		security_type,
	char 				*password,
	int 				ssid_len,
	int 				password_len,
	int					channel);

/**
 * @brief  Start an infrastructure Wi-Fi network with hidden SSID.
 * @warning If a STA interface is active when this function is called, the softAP will
 *          start on the same channel as the STA. It will NOT use the channel provided!
 *
 * @param[in]  ssid: A null terminated string containing
 *  	 the SSID name of the network to join.
 * @param[in]  security_type: Authentication type: \n
 *                         - RTW_SECURITY_OPEN           - Open Security
 *                         - RTW_SECURITY_WPA_TKIP_PSK   - WPA Security
 *                         - RTW_SECURITY_WPA2_AES_PSK   - WPA2 Security using AES cipher
 *                         - RTW_SECURITY_WPA2_MIXED_PSK - WPA2 Security using AES and/or TKIP ciphers
 *                         - WEP security is NOT IMPLEMENTED. It is NOT SECURE!
 * @param[in]  password: A byte array containing the cleartext
 *  	 security key for the network.
 * @param[in]  ssid_len: The length of the SSID in bytes.
 * @param[in]  password_len: The length of the security_key in bytes.
 * @param[in]  channel: 802.11 channel number
 *
 * @return  RTW_SUCCESS: If successfully creates an AP.
 * @return  RTW_ERROR: If an error occurred.
 */
int wifi_start_ap_with_hidden_ssid(
	char 				*ssid,
	rtw_security_t		security_type,
	char 				*password,
	int 				ssid_len,
	int 				password_len,
	int					channel);

/**
 * @brief  Initiate a scan to search for 802.11 networks.
 *
 * @param[in]  scan_type: Specifies whether the scan should 
 *                             be Active, Passive or scan 
 *                             Prohibited channels
 * @param[in]  bss_type: Specifies whether the scan should 
 *                             search for Infrastructure
 *                             networks (those using an Access
 *                             Point), Ad-hoc networks, or both
 *                             types.
 * @param[in]  result_ptr: Scan specific ssid. The first 4 
 *                             bytes is ssid lenth, and ssid name
 *                             append after it.
 *                             If no specific ssid need to scan,
 *                             PLEASE CLEAN result_ptr before pass
 *                             it into parameter.
 * @param[out]  result_ptr: a pointer to a pointer to a result 
 *                             storage structure.
 * @return  RTW_SUCCESS or RTW_ERROR
 * @note  The scan progressively accumulates results over time, and
 *  			may take between 1 and 3 seconds to complete. The results of
 *  			the scan will be individually provided to the callback
 *  			function. Note: The callback function will be executed in
 *  			the context of the RTW thread.
 * @note  When scanning specific channels, devices with a 
 *  	 strong signal strength on nearby channels may be
 *  	 detected
 */
int wifi_scan(rtw_scan_type_t                    scan_type,
				  rtw_bss_type_t                     bss_type,
				  void*                result_ptr);

/**
 * @brief  Initiate a scan to search for 802.11 networks, a higher level API based on wifi_scan
 *			to simplify the scan operation.
 * @param[in]  results_handler: The callback function which will receive and process the result data.
 * @param[in]  user_data: User specified data that will be passed directly to the callback function.
 * @return  RTW_SUCCESS or RTW_ERROR
 * @note  Callback must not use blocking functions, since it is called from the context of the RTW thread. 
 *			The callback, user_data variables will be referenced after the function returns. 
 *			Those variables must remain valid until the scan is completed.
 *			The usage of this api can reference ATWS in atcmd_wifi.c.
 */
int wifi_scan_networks(rtw_scan_result_handler_t results_handler, void* user_data);

/**
 * @brief  Initiate a scan to search for 802.11 networks with specified SSID.
 * @param[in]  results_handler: The callback function which will receive and process the result data.
 * @param[in]  user_data: User specified data that will be passed directly to the callback function.
 * @param[in]  scan_buflen: The length of the result storage structure.
 * @param[in]  ssid: The SSID of target network.
 * @param[in]  ssid_len: The length of the target network SSID.
 * @return  RTW_SUCCESS or RTW_ERROR
 * @note  Callback must not use blocking functions, since it is called from the context of the RTW thread. 
 *			The callback, user_data variables will be referenced after the function returns. 
 *			Those variables must remain valid until the scan is completed.
 */
int wifi_scan_networks_with_ssid(int (results_handler)(char*, int, char *, void *), void* user_data, int scan_buflen, char* ssid, int ssid_len);

/**
* @brief  Set the channel used to be partial scanned.
* @param[in]  channel_list: An array stores the channel list.
* @param[in]  pscan_config: the pscan_config of the channel set.
* @param[in]  length: The length of the channel_list.
* @return  RTW_SUCCESS or RTW_ERROR.
* @note  This function should be used with wifi_scan function. First, use @ref wifi_set_pscan_chan to
*			indicate which channel will be scanned, and then use @ref wifi_scan to get scanned results.
*/
int wifi_set_pscan_chan(__u8 * channel_list,__u8 * pscan_config, __u8 length);

/**
 * @brief  Get current Wi-Fi setting from driver.
 * @param[in]  ifname: the wlan interface name, can be WLAN0_NAME or WLAN1_NAME.
 * @param[out]  pSetting: Points to the rtw_wifi_setting_t structure to store the WIFI setting gotten from driver.
 * @return  RTW_SUCCESS or RTW_ERROR.
 */
int wifi_get_setting(const char *ifname,rtw_wifi_setting_t *pSetting);

/**
 * @brief  Show the network information stored in a rtw_wifi_setting_t structure.
 * @param[in]  ifname: the wlan interface name, can be WLAN0_NAME or WLAN1_NAME.
 * @param[in]  pSetting: Points to the rtw_wifi_setting_t structure which information is gotten by @ref wifi_get_setting().
 * @return  RTW_SUCCESS or RTW_ERROR.
 */
int wifi_show_setting(const char *ifname,rtw_wifi_setting_t *pSetting);

/**
 * @brief  
Set the network mode according to the data rate its supported. 
 *			Driver works in BGN mode in default after driver initialization. This function is used to
 *			change wireless network mode for station mode before connecting to AP.
 * @param[in]  mode: Network mode to set. The value can be RTW_NETWORK_B/RTW_NETWORK_BG/RTW_NETWORK_BGN.
 * @return  RTW_SUCCESS or RTW_ERROR.
 */
int wifi_set_network_mode(rtw_network_mode_t mode);

/**
 * @brief  Set the chip to start or stop the promiscuous mode.
 * @param[in]  enabled: enabled can be set 0, 1 and 2. if enabled is zero, disable the promisc, else enable the promisc.
 *                    - 0 means disable the promisc.
 *                    - 1 means enable the promisc.
 *                    - 2 means enable the promisc special for length is used.
 * @param[in]  callback: the callback function which will 
 *  			  receive and process the netowork data.
 * @param[in]  len_used: specify if the the promisc length is used.
 *				If len_used set to 1, packet length will be saved and transferred to callback function.
 *
 * @return  RTW_SUCCESS or RTW_ERROR
 * @note  This function can be used to implement vendor specified simple configure.
 */
int wifi_set_promisc(rtw_rcr_level_t enabled, void (*callback)(unsigned char*, unsigned int, void*), unsigned char len_used);

/**
 * @brief  Let Wi-Fi enter promiscuous mode.
 * @param[in]  None
 * @return  None
 */
void wifi_enter_promisc_mode(void);

/** Set the wps phase
 *  
 * @param is_trigger_wps[in]   : to trigger wps function or not
 *
 * @return    RTW_SUCCESS or RTW_ERROR
 */
int wifi_set_wps_phase(unsigned char is_trigger_wps);

/**
 * @brief  Trigger Wi-Fi driver to restart an infrastructure Wi-Fi network.
 * @warning If a STA interface is active when this function is called, the softAP will
 *          start on the same channel as the STA. It will NOT use the channel provided!
 * @param[in]  ssid: A null terminated string containing the SSID name of the network.
 * @param[in]  security_type: 
 *                         - RTW_SECURITY_OPEN           - Open Security
 *                         - RTW_SECURITY_WPA_TKIP_PSK   - WPA Security
 *                         - RTW_SECURITY_WPA2_AES_PSK   - WPA2 Security using AES cipher
 *                         - RTW_SECURITY_WPA2_MIXED_PSK - WPA2 Security using AES and/or TKIP ciphers
 *                         - WEP security is NOT IMPLEMENTED. It is NOT SECURE!
 * @param[in]  password: A byte array containing the cleartext security key for the network.
 * @param[in]  ssid_len: The length of the SSID in bytes.
 * @param[in]  password_len: The length of the security_key in bytes.
 * @param[in]  channel: 802.11 channel number.
 * @return  RTW_SUCCESS: If successfully creates an AP.
 * @return  RTW_ERROR: If an error occurred.
 * @note  Please make sure the Wi-Fi is enabled before invoking this function. (@ref wifi_on())
 */
int wifi_restart_ap(
	unsigned char 		*ssid,
	rtw_security_t		security_type,
	unsigned char 		*password,
	int 				ssid_len,
	int 				password_len,
	int					channel);

/**
 * @brief  Set reconnection mode with configuration.
 * @param[in]  mode: Set 1/0 to enalbe/disable the reconnection mode.
 * @param[in]  retry_times: The number of retry limit.
 * @param[in]  timeout: The timeout value (in seconds).
 * @return  0 if success, otherwise return -1.
 * @note  Defining CONFIG_AUTO_RECONNECT in "autoconf.h" needs to be done before compiling,
 *			or this API won't be effective.
 * @note  The difference between @ref wifi_config_autoreconnect() and @ref wifi_set_autoreconnect() is that 
 *			user can specify the retry times and timeout value in @ref wifi_config_autoreconnect().
 *			But in @ref wifi_set_autoreconnect() these values are set with 3 retry limit and 5 seconds timeout as default.
 */
int wifi_config_autoreconnect(__u8 mode, __u8 retry_times, __u16 timeout);

/**
  * @brief  Set reconnection mode with 3 retry limit and 5 seconds timeout as default.
  * @param[in]  mode: Set 1/0 to enalbe/disable the reconnection mode.
  * @return  0 if success, otherwise return -1.
  * @note  Defining CONFIG_AUTO_RECONNECT in "autoconf.h" needs to be done before compiling,
  *			or this API won't be effective.
  * @note  The difference between @ref wifi_config_autoreconnect() and @ref wifi_set_autoreconnect() is that 
  *			user can specify the retry times and timeout value in @ref wifi_config_autoreconnect().
  *			But in @ref wifi_set_autoreconnect() these values are set with 3 retry limit and 5 seconds timeout as default.
  */
int wifi_set_autoreconnect(__u8 mode);

/**
  * @brief  Get the result of setting reconnection mode.
  * @param[out]  mode: Point to the result of setting reconnection mode.
  * @return  0 if success, otherwise return -1.
  * @note  Defining CONFIG_AUTO_RECONNECT in "autoconf.h" needs to be done before compiling,
  *			or this API won't be effective.
  */
int wifi_get_autoreconnect(__u8 *mode);

/**
  * @brief  Present the device disconnect reason while connecting.
  * @param  None
  * @return  @ref rtw_connect_error_flag_t
  *			- 0: RTW_NO_ERROR
  *			- 1: RTW_NONE_NETWORK
  *			- 2: RTW_CONNECT_FAIL
  *			- 3: RTW_WRONG_PASSWORD
  *			- 4: RTW_DHCP_FAIL
  *			- 5: RTW_UNKNOWN (initial status)
  */
int wifi_get_last_error(void);


#ifdef CONFIG_CUSTOM_IE
#ifndef BIT
#define BIT(x)	((__u32)1 << (x))
#endif

#ifndef _CUSTOM_IE_TYPE_
#define _CUSTOM_IE_TYPE_
/**
  * @brief  The enumeration is transmission type for wifi custom ie.
  */
enum CUSTOM_IE_TYPE{
	PROBE_REQ = BIT(0),
	PROBE_RSP = BIT(1),
	BEACON	  = BIT(2),
};
typedef uint32_t rtw_custom_ie_type_t;
#endif /* _CUSTOM_IE_TYPE_ */

/* ie format
 * +-----------+--------+-----------------------+
 * |element ID | length | content in length byte|
 * +-----------+--------+-----------------------+
 *
 * type: refer to CUSTOM_IE_TYPE
 */
#ifndef _CUS_IE_
#define _CUS_IE_
/**
  * @brief  The structure is used to set WIFI custom ie list, and type match CUSTOM_IE_TYPE.\n
  *			The ie will be transmitted according to the type.
  */
typedef struct _cus_ie{
	__u8 *ie;
	__u8 type;
}rtw_custom_ie_t, *p_rtw_custom_ie_t;
#endif /* _CUS_IE_ */

/**
  * @brief  Setup custom ie list.
  * @warning  This API can't be executed twice before deleting the previous custom ie list.
  * @param[in]  cus_ie: Pointer to WIFI CUSTOM IE list.
  * @param[in]  ie_num: The number of WIFI CUSTOM IE list.
  * @return  0 if success, otherwise return -1.
  * @note  Defininig CONFIG_CUSTOM_IE in "autoconf.h" needs to be done before compiling,
  *			or this API won't be effective.
  */
int wifi_add_custom_ie(void *cus_ie, int ie_num);

/**
  * @brief  Update the item in WIFI CUSTOM IE list.
  * @param[in]  cus_ie: Pointer to WIFI CUSTOM IE address.
  * @param[in]  ie_index: Index of WIFI CUSTOM IE list.
  * @return  0 if success, otherwise return -1.
  * @note  Defininig CONFIG_CUSTOM_IE in "autoconf.h" needs to be done before compiling,
  *			or this API won't be effective.
  */
int wifi_update_custom_ie(void *cus_ie, int ie_index);

/**
  * @brief  Delete WIFI CUSTOM IE list.
  * @param  None
  * @return  0 if success, otherwise return -1.
  * @note  Defininig CONFIG_CUSTOM_IE in "autoconf.h" needs to be done before compiling,
  *			or this API won't be effective.
  */
int wifi_del_custom_ie(void);
#endif

#ifdef CONFIG_PROMISC

/**
  * @brief  Initialize packet filter related data.
  * @param  None
  * @return  None
  */
void wifi_init_packet_filter(void);

/**
  * @brief  Add packet filter.
  * @param[in]  filter_id: The filter id.
  * @param[in]  patt: Point to the filter pattern.
  * @param[in]  rule: Point to the filter rule.
  * @return  0 if success, otherwise return -1.
  * @note  For now, the maximum number of filters is 5.
  */
int wifi_add_packet_filter(unsigned char filter_id, rtw_packet_filter_pattern_t *patt, rtw_packet_filter_rule_t rule);

/**
  * @brief  Enable the packet filter.
  * @param[in]  filter_id: The filter id.
  * @return  0 if success, otherwise return -1.
  * @note  The filter can be used only if it has been enabled.
  */
int wifi_enable_packet_filter(unsigned char filter_id);

/**
  * @brief  Disable the packet filter.
  * @param[in]  filter_id: The filter id.
  * @return  0 if success, otherwise return -1.
  */
int wifi_disable_packet_filter(unsigned char filter_id);

/**
  * @brief  Remove the packet filter.
  * @param[in]  filter_id: The filter id.
  * @return  0 if success, otherwise return -1.
  */
int wifi_remove_packet_filter(unsigned char filter_id);
#endif

void wifi_set_indicate_mgnt(int enable);

#ifdef __cplusplus
  }
#endif

#endif // __WIFI_API_H

//----------------------------------------------------------------------------//

