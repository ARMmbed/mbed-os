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
 * code solely for use in connection with Cypress's integrated circuit products.
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
 * including Cypress's product in a High Risk Product, the manufacturer of such
 * system or application assumes all risk of such use and in doing so agrees to
 * indemnify Cypress against all liability.
 */

#ifndef SCL_STA_INTERFACE_H
#define SCL_STA_INTERFACE_H


/** @file
 *  Provides SCL interface functions to be used with WiFiInterface or NetworkInterface Objects
 */

#include "netsocket/WiFiInterface.h"
#include "netsocket/EMACInterface.h"
#include "netsocket/OnboardNetworkStack.h"
#include "scl_emac.h"
#include "scl_wifi_api.h"
#include "scl_types.h"
#define MAX_SSID_LENGTH                       (33)
#define MAX_PASSWORD_LENGTH                   (64)

/** SclSTAInterface class
 *  Implementation of the NetworkStack for the SCL
 */
class SclSTAInterface : public WiFiInterface, public EMACInterface {
public:

    SclSTAInterface(
        SCL_EMAC &emac = SCL_EMAC::get_instance(),
        OnboardNetworkStack &stack = OnboardNetworkStack::get_default_instance());
    
    /** Get current instance of the SclSTAInterface
     *
     *  @return         pointer to the object of class SclSTAInterface
     */
	static SclSTAInterface *get_default_instance();
    
	/** Turn on the WiFi device
    *
    *  @return         void
    */
    /* Turn on the wifi device*/
    void wifi_on();

    /** Start the interface
     *
     *  Attempts to connect to a WiFi network. Requires ssid and passphrase to be set.
     *  If passphrase is invalid, NSAPI_ERROR_AUTH_ERROR is returned.
     *
     *  @return         0 on success, negative error code on failure
     */
    nsapi_error_t connect();

    /** Start the interface
     *
     *  Attempts to connect to a WiFi network.
     *
     *  @param ssid      Name of the network to connect to
     *  @param pass      Security passphrase to connect to the network
     *  @param security  Type of encryption for connection (Default: NSAPI_SECURITY_NONE)
     *  @param channel   This parameter is not supported, setting it to anything else than 0 will result in NSAPI_ERROR_UNSUPPORTED
     *  @return          0 on success, or error code on failure
     */
    nsapi_error_t connect(const char *ssid, const char *pass, nsapi_security_t security = NSAPI_SECURITY_NONE, uint8_t channel = 0);

    /** Stop the interface
     *  @return             0 on success, negative on failure
     */
    nsapi_error_t disconnect();

    /** Set the WiFi network credentials
     *
     *  @param ssid      Name of the network to connect to
     *  @param pass      Security passphrase to connect to the network
     *  @param security  Type of encryption for connection
     *                   (defaults to NSAPI_SECURITY_NONE)
     *  @return          0 on success, or error code on failure
     */
    nsapi_error_t set_credentials(const char *ssid, const char *pass, nsapi_security_t security = NSAPI_SECURITY_NONE);

    /** Set the WiFi network channel - NOT SUPPORTED
     *
     * This function is not supported and will return NSAPI_ERROR_UNSUPPORTED
     *
     *  @param channel   Channel on which the connection is to be made, or 0 for any (Default: 0)
     *  @return          Not supported, returns NSAPI_ERROR_UNSUPPORTED
     */
    nsapi_error_t set_channel(uint8_t channel)
    {
        if (channel != 0) {
            return NSAPI_ERROR_UNSUPPORTED;
        }
        return 0;
    }

    /** Set blocking status of interface. 
     *  Nonblocking mode unsupported.
     *
     *  @param blocking true if connect is blocking
     *  @return         0 on success, negative error code on failure
     */
    nsapi_error_t set_blocking(bool blocking)
    {
        if (blocking) {
            _blocking = blocking;
            return NSAPI_ERROR_OK;
        } else {
            return NSAPI_ERROR_UNSUPPORTED;
        }
    }
    /** Gets the current radio signal strength for active connection
     *
     * @return          Connection strength in dBm (negative value)
     */
    int8_t get_rssi();

    /** Scan for available networks
     *
     * This function will block.
     *
     * @param  ap       Pointer to allocated array to store discovered AP
     * @param  count    Size of allocated @a res array, or 0 to only count available AP
     * @param  timeout  Timeout in milliseconds; 0 for no timeout (Default: 0)
     * @return          Number of entries in @a, or if @a count was 0 number of available networks, negative on error
     *                  see @a nsapi_error
     */
    int scan(WiFiAccessPoint *res, unsigned count);

    /** This function is used to know if the device is connected to the network
     * @return          SCL_SUCCESS if device is connected
     */
    int is_interface_connected();

    /** Get the BSSID (MAC address of device connected to)
     *
     *  @param bssid   pointer to the BSSID value
     *  @return        SCL_SUCCESS if BSSID is obtained successfully
     *  @return        SCL_BADARG if input parameter is NULL
     *  @return        SCL_ERROR if unable to fetch BSSID
     */
    int get_bssid(uint8_t *bssid);

    /** This function is used to set up the wifi interface after wifi on
     * @return          SCL_SUCCESS if wifi interface is up successfully
     */
    int wifi_set_up(void);

private:

    char _ssid[MAX_SSID_LENGTH]; /* The longest possible name (defined in 802.11) +1 for the \0 */
    char _pass[MAX_PASSWORD_LENGTH]; /* The longest allowed passphrase + 1 */
    nsapi_security_t _security;
    SCL_EMAC &_scl_emac;
};
#endif
