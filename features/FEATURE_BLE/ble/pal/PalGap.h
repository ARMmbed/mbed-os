/* mbed Microcontroller Library
 * Copyright (c) 2017-2017 ARM Limited
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

#ifndef BLE_PAL_GAP_H_
#define BLE_PAL_GAP_H_

#include "platform/Callback.h"
#include "GapTypes.h"
#include "GapEvents.h"

namespace ble {
namespace pal {

/**
 * Adaptation interface for the GAP layer.
 *
 * Define the primitive necessary to realize GAP operations. the API and event
 * follow closely the definition of the HCI commands and events used
 * by that layer.
 */
struct Gap {
    /**
     * Initialisation of the instance. An implementation can use this function
     * to initialise the subsystems needed to realize the operations of this
     * interface.
     *
     * This function has to be called before any other operations.
     *
     * @return BLE_ERROR_NONE if the request has been successfully sent or the
     * appropriate error otherwise.
     */
    virtual ble_error_t initialize() = 0;

    /**
     * Termination of the instance. An implementation can use this function
     * to release the subsystems initialised to realise the operations of
     * this interface.
     *
     * After a call to this function, initialise should be called again to
     * allow use of the interface.
     *
     * @return BLE_ERROR_NONE if the request has been successfully sent or the
     * appropriate error otherwise.
     */
    virtual ble_error_t terminate() = 0;

    /**
     * Return the public device address.
     *
     * @note The public device address is usually acquired at initialization and
     * stored in the instance.
     *
     * @return the public device address.
     */
    virtual address_t get_device_address() = 0;

    /**
     * Return the current random address.
     *
     * @note The random address is usually acquired at initialization and stored
     * in the instance.
     *
     * @return the random device address.
     */
    virtual address_t get_random_address() = 0;

    /**
     * Set the random address which will used be during scan, connection or
     * advertising process if the own address type selected is random.
     *
     * Changing the address during scan, connection or advertising process is
     * forbiden.
     *
     * @note: See Bluetooth 5 Vol 2 PartE: 7.8.4 LE set random address command.
     *
     * @return BLE_ERROR_NONE if the request has been successfully sent or the
     * appropriate error otherwise.
     */
    virtual ble_error_t set_random_address(const address_t& address) = 0;

    /**
     * Set the advertising parameters which will be used during the advertising
     * process.
     *
     * @param advertising_interval_min: Minimum advertising interval which can
     * be used during undirected and low duty cycle directed advertising. This
     * parameter shall be less than or equal to advertising_interval_max. This
     * parameter shall be in the range [0x20 : 0x4000] where each unit is equal
     * to 0.625ms.
     * This parameter is not used by directed high duty cycle advertising.
     *
     * @param advertising_interval_max: Maximum advertising interval which can
     * be used during undirected and low duty cycle directed advertising. This
     * parameter shall be more than or equal to advertising_interval_min. This
     * parameter shall be in the range [0x20 : 0x4000] where each unit is equal
     * to 0.625ms.
     * This parameter is not used by directed high duty cycle advertising.
     *
     * @param advertising_type Packet type that is used during the
     * advertising process. Direct advertising require valid peer addresses
     * parameters and ignore the filter policy set.
     * If the High duty cycle advertising is used then the advertising parameter
     * intervals are ignored.
     *
     * @param own_address_type Own address type used during advertising.
     * If own address type is equal to RESOLVABLE_PRIVATE_ADDRESS_PUBLIC_FALLBACK
     * or RESOLVABLE_PRIVATE_ADDRESS_RANDOM_FALLBACK then the peer address
     * parameters (type and address) will be used to find the local IRK.
     *
     * @param peer_address_type Address type of the peer.
     * This parameter shall be valid if directed advertising is used (
     * ADV_DIRECT_IND or ADV_DIRECT_IND_LOW_DUTY_CYCLE). This parameter should
     * be valid if the own address type is equal to 0x02 or 0x03.
     * In other cases, this parameter is ignored.
     *
     * @param peer_address Public device address, Random device addres, Public
     * identity address or Random static identity address of the device targeted
     * by the advertising.
     * This parameter shall be valid if directed advertising is used (
     * ADV_DIRECT_IND or ADV_DIRECT_IND_LOW_DUTY_CYCLE). This parameter should
     * be valid if the own address type is equal to 0x02 or 0x03.
     * In other cases, this parameter is ignored.
     *
     * @param advertising_channel_map Map of the channel used to send
     * advertising data.
     *
     * @param advertising_filter_policy Filter policy applied during the
     * advertising process. The subsystem should use the whitelist to apply the
     * policy. This parameter is ignored if the advertising type is directed (
     * ADV_DIRECT_IND or ADV_DIRECT_IND_LOW_DUTY_CYCLE).
     *
     * @return BLE_ERROR_NONE if the request has been successfully sent or the
     * appropriate error otherwise.
     *
     * @note: See Bluetooth 5 Vol 2 PartE: 7.8.5 LE set advertising parameters
     * command.
     */
    virtual ble_error_t set_advertising_parameters(
        uint16_t advertising_interval_min,
        uint16_t advertising_interval_max,
        advertising_type_t advertising_type,
        own_address_type_t own_address_type,
        advertising_peer_address_type_t peer_address_type,
        const address_t& peer_address,
        advertising_channel_map_t advertising_channel_map,
        advertising_filter_policy_t advertising_filter_policy
    ) = 0;

    /**
     * Set the data sends in advertising packet. If the advertising is
     * currently enabled, the data shall be used when a new advertising packet
     * is issued.
     *
     * @param advertising_data_length Number of significant bytes in the
     * advertising data.
     *
     * @param advertising_data The data sends in advertising packets. Non
     * significant bytes shall be equal to 0.
     *
     * @return BLE_ERROR_NONE if the request has been successfully sent or the
     * appropriate error otherwise.
     *
     * @note: See Bluetooth 5 Vol 2 PartE: 7.8.5 LE set advertising data
     * command.
     */
    virtual ble_error_t set_advertising_data(
        uint8_t advertising_data_length,
        const advertising_data_t& advertising_data
    ) = 0;

    /**
     * Set the data sends in scan response packets. If the advertising is
     * currently enabled, the data shall be used when a new scan response is
     * issued.
     *
     * @param scan_response_data_length Number of significant bytes in the
     * scan response data.
     *
     * @param scan_response_data The data sends in scan response packets. Non
     * significant bytes shall be equal to 0.
     *
     * @return BLE_ERROR_NONE if the request has been successfully sent or the
     * appropriate error otherwise.
     *
     * @note: See Bluetooth 5 Vol 2 PartE: 7.8.8 LE set scan response data
     * command.
     */
    virtual ble_error_t set_scan_response_data(
        uint8_t scan_response_data_length,
        const advertising_data_t& scan_response_data
    ) = 0;

    /**
     * Start or stop advertising.
     *
     * The process use advertising and scan response data set with
     * set_advertising_data and set_scan_response_data while the parameters used
     * are the one set by set_advertising_parameters.
     *
     * The advertising shall continue until:
     *     - The advertising is manually disabled (advertising_enable(false)).
     *     - A connection is created.
     *     - Time out in case of high duty cycle directed advertising.
     *
     * If the random address has not been set and the advertising parameter
     * own_address_type is set to 0x01 then the procedure shall fail.
     *
     * If the random address has not been set and the advertising parameter
     * own_address_type is set to RESOLVABLE_PRIVATE_ADDRESS_RANDOM_FALLBACK and
     * the peer address is not in the resolving list then the procedure shall
     * fail.
     *
     * @param enable If true start the advertising process, if the process was
     * already runing and own_address_type is equal to 0x02 or 0x03, the
     * subsystem can change the random address.
     * If false and the advertising is running then the process should be
     * stoped.
     *
     * @return BLE_ERROR_NONE if the request has been successfully sent or the
     * appropriate error otherwise.
     *
     * @note: See Bluetooth 5 Vol 2 PartE: 7.8.9 LE set advertising enable
     * command.
     *
     * @note If the advertising type is ADV_DIRECT_IND and the connection is not
     * created during the time allowed to the procedure then a
     * ConnectionComplete event shall be emmited with its error code set to
     * ADVERTISING_TIMEOUT.
     *
     * @note Successfull connection shall emit a ConnectionComplete event. It
     * also means advertising is disabled.
     */
    virtual ble_error_t advertising_enable(bool enable) = 0;

    /**
     * Set the parameter of the scan process.
     *
     * This command shall not be effective when the scanning process is running.
     *
     * @param active_scanning If true the subsystem does active scanning and
     * the bluetooth subsystem shall send scanning PDUs. It shall also listen
     * to scan responses. If false no scanning PDUs are sent during the scan
     * process.
     *
     * @param scan_interval The time interval between two subsequent LE scans in
     * unit of 0.625ms. This parameter shall be in the range [0x0004 : 0x4000].
     *
     * @param scan_window Duration of the LE scan. It shall be less than or
     * equal to scan_interval value. This parameter shall be in the range
     * [0x0004 : 0x4000] and is in unit of 0.625ms.
     *
     * @param own_address_type Own address type used in scan request packets.
     *
     * @param filter_policy Filter policy applied when scanning.
     *
     * @return BLE_ERROR_NONE if the request has been successfully sent or the
     * appropriate error otherwise.
     *
     * @note: See Bluetooth 5 Vol 2 PartE: 7.8.10 LE set scan parameters
     * command.
     */
    virtual ble_error_t set_scan_parameters(
        bool active_scanning,
        uint16_t scan_interval,
        uint16_t scan_window,
        own_address_type_t own_address_type,
        scanning_filter_policy_t filter_policy
    ) = 0;

    /**
     * Start/stop scanning process.
     *
     * Parameters of the scanning process shall be set before the scan launch
     * by using the function set_scan_parameters.
     *
     * @parameter enable Start the scanning process if true and stop it if
     * false. If the scan process is already started, enabling it again will
     * only update the duplicate filtering; based on the new parameter.
     *
     * @parameter filter_duplicates Enable duplicate filtering if true,
     * otherwise disable it.
     *
     * @attention advertising data or scan response data is not considered
     * significant when determining duplicate advertising reports.
     *
     * @return BLE_ERROR_NONE if the request has been successfully sent or the
     * appropriate error otherwise.
     *
     * @note: See Bluetooth 5 Vol 2 PartE: 7.8.11 LE set scan enable command.
     */
    virtual ble_error_t scan_enable(
        bool enable,
        bool filter_duplicates
    ) = 0;

    /**
     * Create a new le connection to a connectable advertiser.
     *
     * @param scan_interval Hint to the le subsystem indicating how
     * frequently it should scan for the peer address. It represent the time
     * interval between two subsequent scan for the peer. It shall be in the
     * range [0x0004 : 0x4000] and the time is in unit of 0.625ms.
     *
     * @param scan_window Hint to the le subsystem indicating for how long it
     * should scan during a scan interval. The value shall be smaller or equal
     * to scan_interval. If it is equal to scan_interval then scanning should
     * run continuously. It shall be in the range [0x0004 : 0x4000] and the time
     * is in unit of 0.625ms.
     *
     * @param initiator_policy Used to determine if the whitelist is used to
     * determine the address to connect to. If the whitelist is not used, the
     * connection shall be made against an advertiser matching the peer_address
     * and the peer_address_type parameters. Otherwise those parameters are
     * ignored.
     *
     * @param peer_address_type Type of address used by the advertiser. Not used
     * if initiator_policy use the whitelist.
     *
     * @param Address used by the advertiser in its advertising packets. Not
     * used if initiator_policy use the whitelist.
     *
     * @param own_address_type Type of address used in the connection request
     * packet.
     *
     * @param connection_interval_min Minimum interval between two connection
     * events allowed for the connection. It shall be less than or equal to
     * connection_interval_max. This value shall be in range [0x0006 : 0x0C80]
     * and is in unit of 1.25ms.
     *
     * @param connection_interval_max Maximum interval between two connection
     * events allowed for the connection. It shall be greater than or equal to
     * connection_interval_min. This value shall be in range [0x0006 : 0x0C80]
     * and is in unit of 1.25ms.
     *
     * @param connection_latency Number of connection events the slave can drop
     * if it has nothing to communicate to the master. This value shall be in
     * the range [0x0000 : 0x01F3].
     *
     * @param supervision_timeout Link supervision timeout for the connection.
     * It shall be larger than:
     *        (1 + connection_latency) * connection_interval_max * 2
     * Note: connection_interval_max is in ms in this formulae.
     * Everytime the master or the slave receive a valid packet from the peer,
     * the supervision timer is reset. If the supervision timer reaches
     * supervision_timeout then the connection is considered lost and a
     * disconnect event shall be emmited.
     * This value shall be in the range [0x000A : 0x0C80] and is in unit of 10
     * ms.
     *
     * @param minimum_connection_event_length Informative parameter of the
     * minimum length a connection event. It shall be less than or equal to
     * maximum_connection_event_length. It shall be in the range
     * [0x0000 : 0xFFFF]. It should also be less than the expected connection
     * interval. The unit is 0.625ms.
     *
     * @param maximum_connection_event_length Informative parameter of the
     * maximum length a connection event. It shall be more than or equal to
     * minimum_connection_event_length. It shall be in the range
     * [0x0000 : 0xFFFF]. It should also be less than the expected connection
     * interval. The unit is 0.625ms.
     *
     * @return BLE_ERROR_NONE if the request has been successfully sent or the
     * appropriate error otherwise.
     *
     * @note: See Bluetooth 5 Vol 2 PartE: 7.8.12 LE create connection command.
     */
    virtual ble_error_t create_connection(
        uint16_t scan_interval,
        uint16_t scan_window,
        initiator_policy_t initiator_policy,
        connection_peer_address_type_t peer_address_type,
        const address_t& peer_address,
        own_address_type_t own_address_type,
        uint16_t connection_interval_min,
        uint16_t connection_interval_max,
        uint16_t connection_latency,
        uint16_t supervision_timeout,
        uint16_t minimum_connection_event_length,
        uint16_t maximum_connection_event_length
    ) = 0;

    /**
     * Cancel the ongoing connection creation process.
     *
     * @return BLE_ERROR_NONE if the request has been successfully sent or the
     * appropriate error otherwise.
     *
     * @note: See Bluetooth 5 Vol 2 PartE: 7.8.13 LE create connection cancel
     * command.
     */
    virtual ble_error_t cancel_connection_creation() = 0;

    /**
     * Return the number of total whitelist entries that can be stored in the
     * le subsystem.
     *
     * @note The value returned can change over time.
     *
     * @return The number of entries that can be stored in the LE subsystem. It
     * range from 0x01 to 0xFF.
     *
     * @note: See Bluetooth 5 Vol 2 PartE: 7.8.14 LE read white list size
     * command.
     */
    virtual uint8_t read_white_list_capacity() = 0;

    /**
     * Clear the whitelist stored in the LE subsystem.
     *
     * @attention This command shall not be issued if the whitelist is being
     * used by the advertising, scanning or connection creation procedure.
     *
     * @return BLE_ERROR_NONE if the request has been successfully sent or the
     * appropriate error otherwise.
     *
     * @note: See Bluetooth 5 Vol 2 PartE: 7.8.15 LE clear white list command.
     */
    virtual ble_error_t clear_whitelist() = 0;

    /**
     * Add a device to the LE subsystem Whitelist.
     *
     * @param address_type address_type Type of the address to add in the
     * whitelist.
     *
     * @param address Address of the device.
     *
     * @return BLE_ERROR_NONE if the request has been successfully sent or the
     * appropriate error otherwise.
     *
     * @attention This command shall not be issued if the whitelist is being
     * used by the advertising, scanning or connection creation procedure.
     *
     * @note: See Bluetooth 5 Vol 2 PartE: 7.8.16 LE add device to white list
     * command.
     */
    virtual ble_error_t add_device_to_whitelist(
        whitelist_address_type_t address_type,
        address_t address
    ) = 0;

    /**
     * Remove a device from the LE subsystem Whitelist.
     *
     * @param address_type address_type Type of the address of the device to
     * remove from the whitelist.
     *
     * @param address Address of the device to remove from the whitelist
     *
     * @return BLE_ERROR_NONE if the request has been successfully sent or the
     * appropriate error otherwise.
     *
     * @attention This command shall not be issued if the whitelist is being
     * used by the advertising, scanning or connection creation procedure.
     *
     * @note: See Bluetooth 5 Vol 2 PartE: 7.8.17 LE remove device from white
     * list command.
     */
    virtual ble_error_t remove_device_from_whitelist(
        whitelist_address_type_t address_type,
        address_t address
    ) = 0;

    /**
     * Start a connection update procedure.
     *
     * This procedure change the parameter used for a connection it can be
     * master or slave initiated.
     *
     * The peer will received a connection parameters request and will either
     * accept or reject the new parameters for the connection.
     *
     * Once the peer response has been received, the procedure ends and a
     * Connection update complete event is emmited.
     *
     * @param connection Handle of the connection.
     *
     * @param connection_interval_min Minimum interval between two connection
     * events allowed for the connection. It shall be less than or equal to
     * connection_interval_max. This value shall be in range [0x0006 : 0x0C80]
     * and is in unit of 1.25ms.
     *
     * @param connection_interval_max Maximum interval between two connection
     * events allowed for the connection. It shall be greater than or equal to
     * connection_interval_min. This value shall be in range [0x0006 : 0x0C80]
     * and is in unit of 1.25ms.
     *
     * @param connection_latency Number of connection events the slave can drop
     * if it has nothing to communicate to the master. This value shall be in
     * the range [0x0000 : 0x01F3].
     *
     * @param supervision_timeout Link supervision timeout for the connection.
     * It shall be larger than:
     *        (1 + connection_latency) * connection_interval_max * 2
     * Note: connection_interval_max is in ms in this formulae.
     * Everytime the master or the slave receive a valid packet from the peer,
     * the supervision timer is reset. If the supervision timer reaches
     * supervision_timeout then the connection is considered lost and a
     * disconnect event shall be emmited.
     * This value shall be in the range [0x000A : 0x0C80] and is in unit of 10
     * ms.
     *
     * @param minimum_connection_event_length Informative parameter of the
     * minimum length a connection event. It shall be less than or equal to
     * maximum_connection_event_length. It shall be in the range
     * [0x0000 : 0xFFFF]. It should also be less than the expected connection
     * interval. The unit is 0.625ms.
     *
     * @param maximum_connection_event_length Informative parameter of the
     * maximum length a connection event. It shall be more than or equal to
     * minimum_connection_event_length. It shall be in the range
     * [0x0000 : 0xFFFF]. It should also be less than the expected connection
     * interval. The unit is 0.625ms.
     *
     * @return BLE_ERROR_NONE if the request has been successfully sent or the
     * appropriate error otherwise.
     *
     * @note: See Bluetooth 5 Vol 2 PartE: 7.8.18 LE Connection update command.
     *
     */
    virtual ble_error_t connection_parameters_update(
        connection_handle_t connection,
        uint16_t connection_interval_min,
        uint16_t connection_interval_max,
        uint16_t connection_latency,
        uint16_t supervision_timeout,
        uint16_t minimum_connection_event_length,
        uint16_t maximum_connection_event_length
    ) = 0;

    /**
     * Accept connection parameter request.
     *
     * This command sends a positive response to a connection parameter request
     * from a peer.
     *
     * @param connection Handle of the connection.
     *
     * @param connection_interval_min Minimum interval between two connection
     * events allowed for the connection. It shall be less than or equal to
     * connection_interval_max. This value shall be in range [0x0006 : 0x0C80]
     * and is in unit of 1.25ms.
     *
     * @param connection_interval_max Maximum interval between two connection
     * events allowed for the connection. It shall be greater than or equal to
     * connection_interval_min. This value shall be in range [0x0006 : 0x0C80]
     * and is in unit of 1.25ms.
     *
     * @param connection_latency Number of connection events the slave can drop
     * if it has nothing to communicate to the master. This value shall be in
     * the range [0x0000 : 0x01F3].
     *
     * @param supervision_timeout Link supervision timeout for the connection.
     * It shall be larger than:
     *        (1 + connection_latency) * connection_interval_max * 2
     * Note: connection_interval_max is in ms in this formulae.
     * Everytime the master or the slave receive a valid packet from the peer,
     * the supervision timer is reset. If the supervision timer reaches
     * supervision_timeout then the connection is considered lost and a
     * disconnect event shall be emmited.
     * This value shall be in the range [0x000A : 0x0C80] and is in unit of 10
     * ms.
     *
     * @param minimum_connection_event_length Informative parameter of the
     * minimum length a connection event. It shall be less than or equal to
     * maximum_connection_event_length. It shall be in the range
     * [0x0000 : 0xFFFF]. It should also be less than the expected connection
     * interval. The unit is 0.625ms.
     *
     * @param maximum_connection_event_length Informative parameter of the
     * maximum length a connection event. It shall be more than or equal to
     * minimum_connection_event_length. It shall be in the range
     * [0x0000 : 0xFFFF]. It should also be less than the expected connection
     * interval. The unit is 0.625ms.
     *
     * @note Usually parameters of this function match the connection parameters
     * received in the connection parameter request event.
     *
     * @attention: Once the new connection parameters are in used a Connection
     * Update Complete event shall be emmited.
     *
     * @return BLE_ERROR_NONE if the request has been successfully sent or the
     * appropriate error otherwise.
     *
     * @note: See Bluetooth 5 Vol 2 PartE: 7.8.31 LE remote connection parameter
     * request reply command.
     */
    virtual ble_error_t accept_connection_parameter_request(
        connection_handle_t connection_handle,
        uint16_t interval_min,
        uint16_t interval_max,
        uint16_t latency,
        uint16_t supervision_timeout,
        uint16_t minimum_connection_event_length,
        uint16_t maximum_connection_event_length
    ) = 0;

    /**
     * Reject a connection parameter update request.
     *
     * @param connection_handle handle to the peer which has issued the
     * connection parameter request.
     *
     * @param rejection_reason Indicate to the peer why the proposed connection
     * parameters were rejected.
     *
     * @return BLE_ERROR_NONE if the request has been successfully sent or the
     * appropriate error otherwise.
     *
     * @note: See Bluetooth 5 Vol 2 PartE: 7.8.32 LE Remote Connection Parameter
     * Request Negative Reply Command.
     */
    virtual ble_error_t reject_connection_parameter_request(
        connection_handle_t connection_handle,
        hci_error_code_t rejection_reason
    ) = 0;

    /**
     * Start a disconnection procedure.
     *
     * Once the procedure is complete it should emit a disconnection complete
     * event.
     *
     * @param connection Handle of the connection to terminate.
     *
     * @param disconnection_reason Indicates the reason for ending the
     * connection.
     *
     * @return BLE_ERROR_NONE if the request has been successfully sent or the
     * appropriate error otherwise.
     *
     * @note: See Bluetooth 5 Vol 2 PartE: 7.1.6 disconenct command.
     */
    virtual ble_error_t disconnect(
        connection_handle_t connection,
        disconnection_reason_t disconnection_reason
    ) = 0;

    /** Check if privacy feature is supported by implementation
     *  
     * @return true if privacy is supported, false otherwise.
     * 
     * @note: See Bluetooth 5 Vol 3 Part C: 10.7 Privacy feature.
     */ 
    virtual bool is_privacy_supported() = 0;

    /** Enable or disable private addresses resolution
     *
     * @param enable whether to enable private addresses resolution
     *  
     * @return BLE_ERROR_NONE if the request has been successfully sent or the
     * appropriate error otherwise.
     * 
     * @note: See Bluetooth 5 Vol 2 PartE: 7.8.44 LE Set Address Resolution Enable command.
     */
    virtual ble_error_t set_address_resolution(
        bool enable
    ) = 0;

    /**
     * Register a callback which will handle Gap events.
     *
     * @param cb The callback object which will handle Gap events from the
     * LE subsystem.
     * It accept a single parameter in input: The event received.
     */
    void when_gap_event_received(mbed::Callback<void(const GapEvent&)> cb)
    {
        _gap_event_cb = cb;
    }

protected:
    Gap() { }

    virtual ~Gap() { }

    /**
     * Implementation shall call this function whenever the LE subsystem
     * generate a Gap event.
     *
     * @param gap_event The event to emit to higher layer.
     */
    void emit_gap_event(const GapEvent& gap_event)
    {
        if (_gap_event_cb) {
            _gap_event_cb(gap_event);
        }
    }

private:
    /**
     * Callback called when an event is emitted by the LE subsystem.
     */
    mbed::Callback<void(const GapEvent&)> _gap_event_cb;

    // Disallow copy construction and copy assignment.
    Gap(const Gap&);
    Gap& operator=(const Gap&);
};

} // namespace pal
} // namespace ble

#endif /* BLE_PAL_GAP_H_ */
