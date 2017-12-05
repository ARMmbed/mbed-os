/* Copyright (c) 2017 ARM Limited
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

#ifndef PPP_CELLULAR_INTERFACE_
#define PPP_CELLULAR_INTERFACE_

#include "CellularBase.h"
#include "platform/ATCmdParser.h"
#include "mbed.h"

#if NSAPI_PPP_AVAILABLE

// Forward declaration
class NetworkStack;

/**
 * Network registration status
 * UBX-13001820 - AT Commands Example Application Note (Section 4.1.4.5)
 */
typedef enum {
   GSM=0,
   COMPACT_GSM=1,
   UTRAN=2,
   EDGE=3,
   HSDPA=4,
   HSUPA=5,
   HSDPA_HSUPA=6,
   LTE=7
} radio_access_nwk_type;

/**
 * Used in registration process to tell which type of network
 * to connect.
 */
typedef enum {
    CIRCUIT_SWITCHED=0,
    PACKET_SWITCHED
} nwk_type;

/**
 * Circuit Switched network registration status (CREG Usage)
 * UBX-13001820 - AT Commands Example Application Note (Section 7.10.3)
 */
typedef enum {
    CSD_NOT_REGISTERED_NOT_SEARCHING=0,
    CSD_REGISTERED=1,
    CSD_NOT_REGISTERED_SEARCHING=2,
    CSD_REGISTRATION_DENIED=3,
    CSD_UNKNOWN_COVERAGE=4,
    CSD_REGISTERED_ROAMING=5,
    CSD_SMS_ONLY=6,
    CSD_SMS_ONLY_ROAMING=7,
    CSD_CSFB_NOT_PREFERRED=9
} nwk_registration_status_csd;

/**
 * Packet Switched network registration status (CGREG Usage)
 * UBX-13001820 - AT Commands Example Application Note (Section 18.27.3)
 */
typedef enum {
    PSD_NOT_REGISTERED_NOT_SEARCHING=0,
    PSD_REGISTERED=1,
    PSD_NOT_REGISTERED_SEARCHING=2,
    PSD_REGISTRATION_DENIED=3,
    PSD_UNKNOWN_COVERAGE=4,
    PSD_REGISTERED_ROAMING=5,
    PSD_EMERGENCY_SERVICES_ONLY=8
} nwk_registration_status_psd;

typedef struct {
    char ccid[20+1];    //!< Integrated Circuit Card ID
    char imsi[15+1];    //!< International Mobile Station Identity
    char imei[15+1];    //!< International Mobile Equipment Identity
    char meid[18+1];    //!< Mobile Equipment IDentifier
    int flags;
    radio_access_nwk_type rat;
    nwk_registration_status_csd reg_status_csd;
    nwk_registration_status_psd reg_status_psd;
} device_info;

/** PPPCellularInterface class
 *
 *  @deprecated This API will be deprecated in mbed-os-5.9. Use mbed-os/features/cellular/easy_cellular/EasyCellularConnection.h instead.
 *
 *  This interface serves as the controller/driver for the Cellular
 *  modems (tested with UBLOX_C027 and MTS_DRAGONFLY_F411RE).
 *
 *  The driver will work with any generic FileHandle, and can be
 *  derived from in order to provide forms for specific interfaces, as well as
 *  adding extra power and reset controls alongside the FileHandle.
 */

class PPPCellularInterface : public CellularBase {

public:

    /** Constructor for a generic modem, using a single FileHandle for commands and PPP data.
     *
     *  @deprecated This API will be deprecated in mbed-os-5.9. Use mbed-os/features/cellular/easy_cellular/EasyCellularConnection.h instead.
     *
     * The file handle pointer is not accessed within the constructor, only recorded for later
     * use - this permits a derived class to pass a pointer to a not-yet-constructed member object.
     */
    MBED_DEPRECATED_SINCE("mbed-os-5.9", "This API will be deprecated, use mbed-os/features/cellular/easy_cellular/EasyCellularConnection.h instead.")
    PPPCellularInterface(FileHandle *fh, bool debug = false);

    /** Destructor
     *
     *  @deprecated This API will be deprecated in mbed-os-5.9. Use mbed-os/features/cellular/easy_cellular/EasyCellularConnection.h instead.
     */
    MBED_DEPRECATED_SINCE("mbed-os-5.9", "This API will be deprecated, use mbed-os/features/cellular/easy_cellular/EasyCellularConnection.h instead.")
    virtual ~PPPCellularInterface();

    /** Set the Cellular network credentials
     *
     *  @deprecated This API will be deprecated in mbed-os-5.9. Use mbed-os/features/cellular/easy_cellular/EasyCellularConnection.h instead.
     *
     *  Please check documentation of connect() for default behaviour of APN settings.
     *
     *  @param apn      Access point name
     *  @param uname    optionally, Username
     *  @param pwd      optionally, password
     */
    MBED_DEPRECATED_SINCE("mbed-os-5.9", "This API will be deprecated, use mbed-os/features/cellular/easy_cellular/EasyCellularConnection.h instead.")
    virtual void set_credentials(const char *apn, const char *uname = 0,
                                                  const char *pwd = 0);

    /** Set the pin code for SIM card
     *
     *  @deprecated This API will be deprecated in mbed-os-5.9. Use mbed-os/features/cellular/easy_cellular/EasyCellularConnection.h instead.
     *
     *  @param sim_pin      PIN for the SIM card
     */
    MBED_DEPRECATED_SINCE("mbed-os-5.9", "This API will be deprecated, use mbed-os/features/cellular/easy_cellular/EasyCellularConnection.h instead.")
    virtual void set_sim_pin(const char *sim_pin);

    /** Start the interface
     *
     *  @deprecated This API will be deprecated in mbed-os-5.9. Use mbed-os/features/cellular/easy_cellular/EasyCellularConnection.h instead.
     *
     *  Attempts to connect to a Cellular network.
     *  This driver is written mainly for data network connections as CellularBase
     *  is NetworkInterface. That's why connect() call internally calls nwk_registration()
     *  method with parameter PACKET_SWITCHED network. Circuit switched hook and registration
     *  process is implemented and left in the driver for future extension/subclass support,e.g.,
     *  an SMS or GPS extension.
     *
     *  @param sim_pin     PIN for the SIM card
     *  @param apn         optionally, access point name
     *  @param uname       optionally, Username
     *  @param pwd         optionally, password
     *  @return            NSAPI_ERROR_OK on success, or negative error code on failure
     */
    MBED_DEPRECATED_SINCE("mbed-os-5.9", "This API will be deprecated, use mbed-os/features/cellular/easy_cellular/EasyCellularConnection.h instead.")
    virtual nsapi_error_t connect(const char *sim_pin, const char *apn = 0,
                                  const char *uname = 0, const char *pwd = 0);

    /** Attempt to connect to the Cellular network
     *
     *  @deprecated This API will be deprecated in mbed-os-5.9. Use mbed-os/features/cellular/easy_cellular/EasyCellularConnection.h instead.
     *
     *  Brings up the network interface. Connects to the Cellular Radio
     *  network and then brings up the underlying network stack to be used
     *  by the cellular modem over PPP interface.
     *
     *  If the SIM requires a PIN, and it is not set/invalid, NSAPI_ERROR_AUTH_ERROR is returned.
     *  For APN setup, default behaviour is to use 'internet' as APN string and assuming no authentication
     *  is required, i.e., username and password are not set. Optionally, a database lookup can be requested
     *  by turning on the APN database lookup feature. In order to do so, add 'MBED_CONF_PPP_CELL_IFACE_APN_LOOKUP'
     *  in your mbed_app.json. APN database is by no means exhaustive. It contains a short list of some public
     *  APNs with publicly available usernames and passwords (if required) in some particular countries only.
     *  Lookup is done using IMSI (International mobile subscriber identifier).
     *  Please note that even if 'MBED_CONF_PPP_CELL_IFACE_APN_LOOKUP' config option is set, 'set_credentials()' api still
     *  gets the precedence. If the aforementioned API is not used and the config option is set but no match is found in
     *  the lookup table then the driver tries to resort to default APN settings.
     *
     *  Preferred method is to setup APN using 'set_credentials()' API.
     *  @return         0 on success, negative error code on failure
     */
    MBED_DEPRECATED_SINCE("mbed-os-5.9", "This API will be deprecated, use mbed-os/features/cellular/easy_cellular/EasyCellularConnection.h instead.")
    virtual nsapi_error_t connect();

    /** Attempt to disconnect from the network
     *
     *  Brings down the network interface. Shuts down the PPP interface
     *  of the underlying network stack. Does not bring down the Radio network
     *
     *  @return         0 on success, negative error code on failure
     */
    MBED_DEPRECATED_SINCE("mbed-os-5.9", "This API will be deprecated, use mbed-os/features/cellular/easy_cellular/EasyCellularConnection.h instead.")
    virtual nsapi_error_t disconnect();

    /** Adds or removes a SIM facility lock
     *
     *  @deprecated This API will be deprecated in mbed-os-5.9. Use mbed-os/features/cellular/easy_cellular/EasyCellularConnection.h instead.
     *
     * Can be used to enable or disable SIM pin check at device startup.
     * This API sets up flags for the driver which would either enable or disable
     * SIM pin checking depending upon the user provided argument while establishing
     * connection. It doesn't do anything immediately other than setting up flags.
     *
     * @param set        can be set to true if the SIM pin check is supposed to be enabled
     *                   and vice versa.
     */
    MBED_DEPRECATED_SINCE("mbed-os-5.9", "This API will be deprecated, use mbed-os/features/cellular/easy_cellular/EasyCellularConnection.h instead.")
    void set_sim_pin_check(bool set);

    /** Change the pin for the SIM card
     *
     *  @deprecated This API will be deprecated in mbed-os-5.9. Use mbed-os/features/cellular/easy_cellular/EasyCellularConnection.h instead.
     *
     * Provide the new pin for your SIM card with this API. Old pin code will be assumed to
     * be set using set_SIM_pin() API. This API have no immediate effect. While establishing
     * connection, driver will change the SIM pin for the next boot.
     *
     * @param new_pin     new pin to be used in string format
     */
    MBED_DEPRECATED_SINCE("mbed-os-5.9", "This API will be deprecated, use mbed-os/features/cellular/easy_cellular/EasyCellularConnection.h instead.")
    void set_new_sim_pin(const char *new_pin);

    /** Check if the connection is currently established or not
     *
     *  @deprecated This API will be deprecated in mbed-os-5.9. Use mbed-os/features/cellular/easy_cellular/EasyCellularConnection.h instead.
     *
     * @return true/false   If the cellular module have successfully acquired a carrier and is
     *                      connected to an external packet data network using PPP, isConnected()
     *                      API returns true and false otherwise.
     */
    MBED_DEPRECATED_SINCE("mbed-os-5.9", "This API will be deprecated, use mbed-os/features/cellular/easy_cellular/EasyCellularConnection.h instead.")
    virtual bool is_connected();

    /** Get the local IP address
     *
     *  @deprecated This API will be deprecated in mbed-os-5.9. Use mbed-os/features/cellular/easy_cellular/EasyCellularConnection.h instead.
     *
     *  @return         Null-terminated representation of the local IP address
     *                  or null if no IP address has been received
     */
    MBED_DEPRECATED_SINCE("mbed-os-5.9", "This API will be deprecated, use mbed-os/features/cellular/easy_cellular/EasyCellularConnection.h instead.")
    virtual const char *get_ip_address();

    /** Get the local network mask
     *
     *  @deprecated This API will be deprecated in mbed-os-5.9. Use mbed-os/features/cellular/easy_cellular/EasyCellularConnection.h instead.
     *
     *  @return         Null-terminated representation of the local network mask
     *                  or null if no network mask has been received
     */
    MBED_DEPRECATED_SINCE("mbed-os-5.9", "This API will be deprecated, use mbed-os/features/cellular/easy_cellular/EasyCellularConnection.h instead.")
    virtual const char *get_netmask();

    /** Get the local gateways
     *
     *  @deprecated This API will be deprecated in mbed-os-5.9. Use mbed-os/features/cellular/easy_cellular/EasyCellularConnection.h instead.
     *
     *  @return         Null-terminated representation of the local gateway
     *                  or null if no network mask has been received
     */
    MBED_DEPRECATED_SINCE("mbed-os-5.9", "This API will be deprecated, use mbed-os/features/cellular/easy_cellular/EasyCellularConnection.h instead.")
    virtual const char *get_gateway();


    /** Turn modem debug traces on
     *
     *  @param on         set true to enable debug traces
     */
    MBED_DEPRECATED_SINCE("mbed-os-5.9", "This API will be deprecated, use mbed-os/features/cellular/easy_cellular/EasyCellularConnection.h instead.")
    void modem_debug_on(bool on);

    /** Register callback for status reporting
     *
     *  @deprecated This API will be deprecated in mbed-os-5.9. Use mbed-os/features/cellular/easy_cellular/EasyCellularConnection.h instead.
     *
     *  @param status_cb The callback for status changes
     */
    MBED_DEPRECATED_SINCE("mbed-os-5.9", "This API will be deprecated, use mbed-os/features/cellular/easy_cellular/EasyCellularConnection.h instead.")
    virtual void attach(Callback<void(nsapi_event_t, intptr_t)> status_cb);

    /** Get the connection status
     *
     *  @deprecated This API will be deprecated in mbed-os-5.9. Use mbed-os/features/cellular/easy_cellular/EasyCellularConnection.h instead.
     *
     *  @return         The connection status according to nsapi_connection_status_t
     */
    MBED_DEPRECATED_SINCE("mbed-os-5.9", "This API will be deprecated, use mbed-os/features/cellular/easy_cellular/EasyCellularConnection.h instead.")
    virtual nsapi_connection_status_t get_connection_status() const;

    /** Set blocking status of connect() which by default should be blocking
     *
     *  @deprecated This API will be deprecated in mbed-os-5.9. Use mbed-os/features/cellular/easy_cellular/EasyCellularConnection.h instead.
     *
     *  @param blocking true if connect is blocking
     *  @return         0 on success, negative error code on failure
     */
    MBED_DEPRECATED_SINCE("mbed-os-5.9", "This API will be deprecated, use mbed-os/features/cellular/easy_cellular/EasyCellularConnection.h instead.")
    virtual nsapi_error_t set_blocking(bool blocking);

private:
    FileHandle *_fh;
    ATCmdParser *_at;
    const char *_new_pin;
    const char *_pin;
    const char *_apn;
    const char *_uname;
    const char *_pwd;
    bool _debug_trace_on;
    nsapi_ip_stack_t _stack;
    Callback<void(nsapi_event_t, intptr_t)> _connection_status_cb;
    nsapi_connection_status_t _connect_status;
    bool _connect_is_blocking;
    void base_initialization();
    void setup_at_parser();
    void shutdown_at_parser();
    nsapi_error_t initialize_sim_card();
    nsapi_error_t setup_context_and_credentials();
    bool power_up();
    void power_down();
    void ppp_status_cb(nsapi_event_t, intptr_t);

protected:
    /** Enable or disable hang-up detection
     *
     * When in PPP data pump mode, it is helpful if the FileHandle will signal hang-up via
     * POLLHUP, e.g., if the DCD line is deasserted on a UART. During command mode, this
     * signaling is not desired. enable_hup() controls whether this function should be
     * active.
     *
     * Meant to be overridden.
     */
    virtual void enable_hup(bool enable);

    /** Sets the modem up for powering on
     *
     *  modem_init() is equivalent to plugging in the device, e.g., attaching power and serial port.
     *  It is meant to be overridden.
     *  If the modem is on-board, an implementation of onboard_modem_api.h
     *  will override this.
     *  If the the modem is a plugged-in device (i.e., a shield type component), the driver deriving from this
     *  class will override.
     */
    virtual void modem_init();

    /** Sets the modem in unplugged state
     *
     *  modem_deinit() will be equivalent to pulling the plug off of the device, i.e., detaching power
     *  and serial port. This puts the modem in lowest power state.
     *  It is meant to be overridden.
     *  If the modem is on-board, an implementation of onboard_modem_api.h
     *  will override this.
     *  If the the modem is a plugged-in device (i.e., a shield type component), the driver deriving from this
     *  class will override.
     */
    virtual void modem_deinit();

    /** Powers up the modem
     *
     *  modem_power_up() is equivalent to pressing the soft power button.
     *  The driver may repeat this if the modem is not responsive to AT commands.
     *  It is meant to be overridden.
     *  If the modem is on-board, an implementation of onboard_modem_api.h
     *  will override this.
     *  If the the modem is a plugged-in device (i.e., a shield type component), the driver deriving from this
     *  class will override.
     */
    virtual void modem_power_up();

    /** Powers down the modem
     *
     *  modem_power_down() is equivalent to turning off the modem by button press.
     *  It is meant to be overridden.
     *  If the modem is on-board, an implementation of onboard_modem_api.h
     *  will override this.
     *  If the the modem is a plugged-in device (i.e., a shield type component), the driver deriving from this
     *  class will override.
     */
    virtual void modem_power_down();

    /** Provide access to the underlying stack
     *
     *  @return The underlying network stack
     */
    virtual NetworkStack *get_stack();

    /** Starts network registration process.
     *
     * Potential users could be subclasses who are not network interface
     * but would like to use CellularBase infrastructure to register
     * with a cellular network, e.g., an SMS extension to CellularBase.
     *
     * @param nwk_type type of network to connect, defaults to packet switched network
     *
     * @return true if registration is successful
     */
    bool nwk_registration(uint8_t nwk_type=PACKET_SWITCHED);

};

#endif //NSAPI_PPP_AVAILABLE

#endif //PPP_CELLULAR_INTERFACE_
