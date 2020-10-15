/* mbed Microcontroller Library
 * Copyright (c) 2020 ARM Limited
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

#ifndef BLE_PAL_PRIVATE_ADDRESS_CONTROLLER_H
#define BLE_PAL_PRIVATE_ADDRESS_CONTROLLER_H

#if BLE_FEATURE_PRIVACY

#include "ble/common/BLETypes.h"
#include "ble/common/blecommon.h"
#include "GapTypes.h"

namespace ble {

/**
 * Control the controller resolution list, resolve private addresses and generate
 * new ones.
 */
struct PalPrivateAddressController {
    struct EventHandler {
#if BLE_GAP_HOST_BASED_PRIVATE_ADDRESS_RESOLUTION
        /** Called when the address resolution request has completed.
         *
         * @param success true if the address has been resolved and false otherwise.
         */
        virtual void on_private_address_resolved(bool success) = 0;
#endif // BLE_GAP_HOST_BASED_PRIVATE_ADDRESS_RESOLUTION

        /** Called when the resolvable private address request has completed.
         *
         * @param rpa The address generated. In case of failure, the address is
         * equal to an invalid address (default value from address_t).
         */
        virtual void on_resolvable_private_address_generated(const address_t &rpa) = 0;

        /**
         * Called when a new item has been added or removed to the resolving list
         * or if the resolving list has been cleared.
         */
        virtual void on_resolving_list_action_complete() = 0;
    };

    /**
     * Initialize the address registry module
     * @return BLE_ERROR_NONE in case of success.
     */
    virtual ble_error_t initialize() = 0;

    /**
     * Terminate the address registry module
     * @return BLE_ERROR_NONE in case of success.
     */
    virtual ble_error_t terminate() = 0;

    /** Generate a new resolvable private address from an IRK
     *
     * The completion of the request is signaled by the stack with a call to
     * PalPrivateAddressController::on_resolvable_private_address_generated.
     *
     * @param local_irk The IRK to use to generate the address.
     *
     * @return BLE_ERROR_NONE if the request has been successfully sent or the
     * appropriate error otherwise.
     *
     * @note Addresses are generated one at a time, this function should not be
     * called while an address is being generated.
     *
     * @note: See Bluetooth 5 Vol 6 PartB: 1.3.2.2 Private Device Address Generation
     */
    virtual ble_error_t generate_resolvable_private_address(const irk_t& local_irk) = 0;

    /** Generate a new non-resolvable private address
     *
     * @return The address generated.
     *
     * @note The address generated might be invalid (all zeros).
     *
     * @note: See Bluetooth 5 Vol 6 PartB: 1.3.2.2 Private Device Address Generation
     */
    virtual address_t generate_non_resolvable_private_address() = 0;

    /** Attempt to resolve a resolvable private address received with an IRK.
     *
     * The completion of the request is signaled by the stack with a call to
     * PalGapEventHandler::on_private_address_resolved.
     *
     * @param address The address to resole
     * @param irk The IRK used for the resolution of the address.
     *
     * @return BLE_ERROR_NONE if the request has been successfully sent or the
     * appropriate error otherwise.
     *
     * @note Addresses are resolved one at a time, this function should not be
     * called while an address is being resolved.
     *
     * @note: See Bluetooth 5 Vol 6 PartB: 1.3.2.3 Private Device Address Resolution
     */
    virtual ble_error_t resolve_private_address(
        const address_t &address,
        const irk_t& irk
    ) = 0;

    ////////////////////////////////////////////////////////////////////////////
    // LL resolution and generation management
    //

    /** Check if privacy feature is supported by LL implementation
     *
     * @return true if privacy is supported, false otherwise.
     *
     * @note: See Bluetooth 5 Vol 3 Part C: 10.7 Privacy feature.
     */
    virtual bool is_ll_privacy_supported() = 0;

    /** Enable or disable private addresses resolution
     *
     * @param enable whether to enable private addresses resolution
     *
     * @return BLE_ERROR_NONE if the request has been successfully sent or the
     * appropriate error otherwise.
     *
     * @note: See Bluetooth 5 Vol 2 PartE: 7.8.44 LE Set Address Resolution Enable command.
     */
    virtual ble_error_t set_ll_address_resolution(
        bool enable
    ) = 0;

    /** Set the timeout for LL resolvable private address rotation.
     *
     * @param timeout The timeout after which the resolvable private address in
     * the controller is changed.
     *
     * @return BLE_ERROR_NONE if the request has been successfully sent or the
     * appropriate error otherwise.
     *
     * @note: See Bluetooth 5 Vol 2 PartE: 7.8.45 LE Set Resolvable Private Address Timeout Command
     */
    virtual ble_error_t set_ll_resolvable_private_address_timeout(resolvable_address_timeout_t timeout) = 0;

    ////////////////////////////////////////////////////////////////////////////
    // Resolving list management
    //

    /** Return the resolving list capacity
     *
     * @return A positive number if LL privacy is supported, 0 if not.
     *
     * @note: See Bluetooth 5 Vol 2 PartE: 7.8.41 LE Read Resolving List Size Command.
     */
    virtual uint8_t read_resolving_list_capacity() = 0;

    /** Add a device definition into the resolving list.
     *
     * @param peer_address_type The type of the identity address received from the
     * peer during bonding.
     * @param peer_identity_address The identity address of the peer received
     * during bonding.
     * @param peer_irk The irk of the peer.
     * @param local_irk The irk sent to the peer during bonding.
     *
     * @return BLE_ERROR_NONE if the request has been successfully sent or the
     * appropriate error otherwise.
     *
     * @note: Cannot be used if address translation is enabled and the device is
     * advertising/scanning or initiating.
     *
     * @note: See Bluetooth 5 Vol 2 PartE: 7.8.38 LE Add Device To Resolving List Command
     */
    virtual ble_error_t add_device_to_resolving_list(
        target_peer_address_type_t peer_address_type,
        const address_t &peer_identity_address,
        const irk_t& peer_irk,
        const irk_t& local_irk
    ) = 0;

    /** Remove a device definition from the resolving list.
     *
     * @param peer_address_type The type of the identity address received from the
     * peer during bonding.
     * @param peer_identity_address The identity address of the peer received
     * during bonding.
     *
     * @return BLE_ERROR_NONE if the request has been successfully sent or the
     * appropriate error otherwise.
     *
     * @note: Cannot be used if address translation is enabled and the device is
     * advertising/scanning or initiating.
     *
     * @note: See Bluetooth 5 Vol 2 PartE: 7.8.39 LE Remove Device From Resolving List Command
     */
    virtual ble_error_t remove_device_from_resolving_list(
        target_peer_address_type_t peer_address_type,
        const address_t &peer_identity_address
    ) = 0;

    /** Remove all entries from the resolving list.
     *
     * @return BLE_ERROR_NONE if the request has been successfully sent or the
     * appropriate error otherwise.
     *
     * @note: Cannot be used if address translation is enabled and the device is
     * advertising/scanning or initiating.
     *
     * @note: See Bluetooth 5 Vol 2 PartE: 7.8.40 LE Clear Resolving List Command
     */
    virtual ble_error_t clear_resolving_list() = 0;

    /** Set the privacy mode (for LL controllers)
     *
     * @param peer_address_type The identity address type of the peer
     * @param peer_address The identity address of the peer
     * @param privacy_mode whether network or device privacy should be used for
     * the peer.
     *
     * @return BLE_ERROR_NONE if the request has been successfully sent or the
     * appropriate error otherwise.
     *
     * @note: See Bluetooth 5 Vol 2 PartE: 7.8.77 LE Set Privacy Mode Command
     */
    virtual ble_error_t set_peer_privacy_mode(
        target_peer_address_type_t peer_address_type,
        const address_t &peer_address,
        privacy_mode_t privacy_mode
    ) = 0;

    /**
     * Set the event handler receiving events from this instance.
     * @param handler The handler to use when an event happen.
     */
    virtual void set_event_handler(EventHandler *handler) = 0;
};

} // namespace ble

#endif // BLE_FEATURE_PRIVACY

#endif //BLE_PAL_PRIVATE_ADDRESS_CONTROLLER_H
