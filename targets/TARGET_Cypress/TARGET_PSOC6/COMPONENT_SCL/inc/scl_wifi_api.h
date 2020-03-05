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

/** @file
 *  Prototypes of functions for controlling the WiFi system
 *
 *  This file provides prototypes for end-user functions, which allow
 *  actions such as scanning for Wi-Fi networks, joining WiFi
 *  networks, getting the MAC address, and so on.
 *
 */

#include <stdbool.h>
#include "scl_common.h"
#ifndef INCLUDED_SCL_WIFI_API_H
#define INCLUDED_SCL_WIFI_API_H

#ifdef __cplusplus
extern "C"
{
#endif

typedef struct scl_tx_buf{
    scl_buffer_t buffer;
    uint32_t size;
}scl_tx_buf_t;

/******************************************************
*             Function Declarations
******************************************************/

/** @addtogroup wifi SCL WiFi API
 *  APIs for controlling the Wi-Fi system
 *  @{
 */

/** @addtogroup wifimanagement SCL WiFi Management API
 *  @ingroup wifi
 *  Initialization and other management functions for SCL system
 *  @{
 */

/**
 * Turn ON the WiFi device
 *
 *  Initialize WiFi platform
 *  Program various WiFi parameters and modes
 *  This API should be called before using any scl_wifi_api
 *  @return true if initialization is successful, false otherwise
 */
extern bool scl_wifi_on(void);

/** Brings up the WiFi core
 *  
 *  @return SCL_SUCCESS or Error code
 */
extern scl_result_t scl_wifi_set_up(void);

/** Retrieves the current Media Access Control (MAC) address
 *  (or Ethernet hardware address) of the 802.11 device
 *
 *  @param   mac           Pointer to a variable to which the current MAC address will be written.
 *
 *  @return  SCL_SUCCESS or Error code
 */
extern scl_result_t scl_wifi_get_mac_address(scl_mac_t *mac);

/** Gets the BSSID of the interface
 *  This API should be used after the device is connected to a network.
 *  @param  bssid         Returns the BSSID address (mac address), if associated.
 *
 *  @return SCL_SUCCESS or Error code
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
 *  @return SCL_SUCCESS      If the address was registered successfully.
 *          Error code       If the address was not registered.
 */
extern scl_result_t scl_wifi_register_multicast_address(scl_mac_t *mac);

/** Determines if a particular interface is ready to transceive ethernet packets.
 *  This function has to be called after the connection is established; otherwise, it returns error code.
 *
 *  @return    SCL_SUCCESS            If the interface is ready to transceive ethernet packets.
 *             SCL_NOTFOUND           If no AP with a matching SSID was found.
 *             SCL_NOT_AUTHENTICATED  If matching AP was found, but it does not let you authenticate.
 *                                    This can occur if the device is in the blocklist of the AP.
 *             SCL_NOT_KEYED          If the device has authenticated and associated, but has not completed the key exchange.
 *                                    This can occur if the passphrase is incorrect.
 *             Error code             If the interface is not ready to transceive ethernet packets.
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
 *  @return SCL_SUCCESS or Error code
 *
 */
extern scl_result_t scl_network_send_ethernet_data(scl_tx_buf_t buffer);

/** Retrieves the latest RSSI value
 *  This API has to be called after the device is connected to a network.
 *  @param   rssi          Location where the RSSI value will be stored.
 *
 *  @return  SCL_SUCCESS   If the RSSI was successfully retrieved.
 *           Error code    If the RSSI was not retrieved.
 */
extern scl_result_t scl_wifi_get_rssi(int32_t *rssi);

/** Retrieves the RX data packet
 *
 *  @param   Pointer to RX buffer.
 *
 *  @return  void  
 *           
 */
extern void scl_network_process_ethernet_data(scl_buffer_t buffer);
extern void scl_emac_wifi_link_state_changed(bool state_up);

#ifdef __cplusplus
} /* extern "C" */
#endif
#endif /* ifndef INCLUDED_SCL_WIFI_API_H */

