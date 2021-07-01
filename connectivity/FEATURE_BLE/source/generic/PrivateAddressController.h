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

#ifndef BLE_GENERIC_PRIVATE_ADDRESS_CONTROLLER_H
#define BLE_GENERIC_PRIVATE_ADDRESS_CONTROLLER_H

#if BLE_FEATURE_PRIVACY

#include <cstdio>

#include "ble/common/BLETypes.h"
#include "ble/common/blecommon.h"
#include "source/pal/PalEventQueue.h"
#include "source/pal/PalPrivateAddressController.h"

#ifdef DEVICE_LPTICKER
#include "drivers/LowPowerTicker.h"
#else
#include "drivers/Ticker.h"
#endif

namespace ble {

/**
 * This module generate resolvable and non resolvable private addresses usable
 * by Gap when it advertise, scan or connect to a peer.
 * It also manages the LL resolvable private address resolution and offer software
 * workaround to resolve addresses on the host.
 */
class PrivateAddressController : private PalPrivateAddressController::EventHandler {
public:
#ifdef DEVICE_LPTICKER
    using Ticker  = mbed::LowPowerTicker;
#else
    using Ticker  = mbed::Ticker;
#endif

    struct EventHandler {
        /**
         * Called when a new resolvable private address has been generated.
         * @param address The resolvable private address generated.
         */
        virtual void on_resolvable_private_addresses_generated(
            const address_t &address
        ) = 0;

        /**
         * Called when a new non resolvable private address has been generated.
         * @param address The non resolvable private address generated.
         */
        virtual void on_non_resolvable_private_addresses_generated(
            const address_t &address
        ) = 0;
#if BLE_GAP_HOST_BASED_PRIVATE_ADDRESS_RESOLUTION
        /**
         * Called when address resolution has been completed by the host.
         *
         * @note Address resolution process is completly transparent when done
         * in LL.
         *
         * @param peer_resolvable_address The resolvable address that was being
         * resolved.
         * @param resolved Indicate if the address has been resolved or not with
         * local irks. If false, the params identity_address_type and
         * identity_address are not valid.
         * @param identity_address_type The type of the identity address resolved.
         * @param identity_address The identity address resolved.
         */
        virtual void on_address_resolution_completed(
            const address_t &peer_resolvable_address,
            bool resolved,
            target_peer_address_type_t identity_address_type,
            const address_t &identity_address
        ) = 0;
#endif // BLE_GAP_HOST_BASED_PRIVATE_ADDRESS_RESOLUTION
    };

    /**
     * Create a new PrivateAddressController.
     * @param pal_address_controller Reference to the underlying pal.
     * @param event_queue Reference to the event queue running in the stack
     * context.
     * @param rotation_timeout Time after which private addresses must be renew.
     */
    PrivateAddressController(
        PalPrivateAddressController &pal_address_controller,
        PalEventQueue &event_queue,
        ble::resolvable_address_timeout_t rotation_timeout
    );

    ~PrivateAddressController();

    PrivateAddressController(const PrivateAddressController&) = delete;
    PrivateAddressController& operator=(const PrivateAddressController&) = delete;

    /**
     * Set the local irk used for address generation. Generation doesn't start
     * until a valid irk has been set.
     * @param local_irk
     */
    void set_local_irk(const irk_t &local_irk);

    /**
     * Change address renewal timeout.
     * @param rotation_timeout address generation timeout.
     */
    void set_timeout(ble::resolvable_address_timeout_t rotation_timeout);

    /**
     * Get the current resolvable private address.
     */
    const address_t& get_resolvable_private_address();

    /**
     * Get the current non resolvable private address.
     */
    const address_t& get_non_resolvable_private_address();

    /**
     * Set the event handler catching events from this instance.
     */
    void set_event_handler(EventHandler *handler);

    /**
     * Start private address generation. A new address is generated when the
     * timeout expired.
     * it ends when stop_private_address_generation is called.
     */
    void start_private_address_generation();

    /**
     * Stop private address generation.
     */
    void stop_private_address_generation();

    /**
     * Return true if privacy is supported by the underlying controller and false
     * otherwise.
     */
    bool is_controller_privacy_supported();

#if !BLE_GAP_HOST_BASED_PRIVATE_ADDRESS_RESOLUTION
    /**
     * Enable address resolution by the controller.
     */
    ble_error_t enable_controller_address_resolution(bool enable);
#endif //!BLE_GAP_HOST_BASED_PRIVATE_ADDRESS_RESOLUTION

    /**
     * Read the number of entries that can be put in the resolving list.
     */
    uint8_t read_resolving_list_capacity();

    /**
     * Read the number of entries that are in the resolving list.
     */
    uint8_t read_resolving_list_size();

    /**
     * Add a new peer to the resolving list.
     * @param peer_address_type The type of the peer's identity address.
     * @param peer_identity_address The identity address of the peer.
     * @param peer_irk The IRK of the peer.
     * @return BLE_ERROR_NONE in case of success and false otherwise.
     */
    ble_error_t add_device_to_resolving_list(
        target_peer_address_type_t peer_address_type,
        const address_t &peer_identity_address,
        const irk_t& peer_irk
    );

    /**
     * Remove a device from the resolving list.
     * @param peer_address_type The type of the peer's identity address.
     * @param peer_identity_address The identity address of the peer.
     * @return
     */
    ble_error_t remove_device_from_resolving_list(
        target_peer_address_type_t peer_address_type,
        const address_t &peer_identity_address
    );

    /**
     * Empty the resolving list.
     */
    ble_error_t clear_resolving_list();

#if BLE_GAP_HOST_BASED_PRIVATE_ADDRESS_RESOLUTION
    /**
     * Resolve a private resolvable address on the host.
     *
     * To speedup operations, this module uses a cache to avoid too many
     * resolutions. If the entry is available in the cache, it is returned
     * immediately otherwise, the request will be completed by a report through
     * EventHandler::on_address_resolution_completed
     *
     * @param peer_address the address to resolve.
     * @param resolution_complete Set to true if the resolution process has been
     * completed. If false, result will be made available by
     * EventHandler::on_address_resolution_completed.
     * @param retrieved_address_type The type of the address resolved. Valid if
     * resolution_complete is equals to true AND retrieved_address is not null.
     * @param retrieved_address If an entry has been found for the address in
     * input this will be set to null if the address has not been resolved with
     * the local irks or a pointer to the identity address if it has been resolved.
     */
    ble_error_t resolve_address_on_host(
        const address_t &peer_address,
        bool *resolution_complete,
        target_peer_address_type_t *retrieved_address_type,
        const address_t **retrieved_address
    );
    /**
     * Resolve a private address by looking in the cache.
     *
     * @param peer_address the address to resolve.
     * @param retrieved_address_type The type of the address resolved. Valid if
     * resolution_complete is equals to true AND retrieved_address is not null.
     * @param retrieved_address If an entry has been found for the address in
     * input this will be set to null if the address has not been resolved with
     * the local irks or a pointer to the identity address if it has been resolved.
     *
     * @return True if the address has been found in cache.
    */
    bool resolve_address_in_host_cache(
        const address_t &peer_address,
        target_peer_address_type_t *retrieved_address_type,
        const address_t **retrieved_address
    );

    /**
     * Resolve a private resolvable address asynchronously. The request will be completed by a report through
     * EventHandler::on_address_resolution_completed.
     *
     * @param peer_address the address to resolve.
     */
    ble_error_t queue_resolve_address_on_host(const address_t &peer_address);
#endif // BLE_GAP_HOST_BASED_PRIVATE_ADDRESS_RESOLUTION

private:

    void generate_resolvable_address();

    void generate_non_resolvable_address();

    // EventHandler implementation
    void on_resolvable_private_address_generated(const address_t &rpa) final;

#if BLE_GAP_HOST_BASED_PRIVATE_ADDRESS_RESOLUTION
    void on_private_address_resolved(bool success) final;
#endif // BLE_GAP_HOST_BASED_PRIVATE_ADDRESS_RESOLUTION

    void on_resolving_list_action_complete() final;

    // --- resolving list and resolution management
    // The same list is shared between the two processes given they will never
    // be active at the same time. Either the controller supports LL resolution
    // and the list will only contain resolving list update operations or the
    // controller doesn't support LL and address resolution is the only operation
    // present in the operation list.
    struct PrivacyControlBlock;
    struct PrivacyClearResListControlBlock;
    struct PrivacyAddDevToResListControlBlock;
    struct PrivacyRemoveDevFromResListControlBlock;

    // Queue control block to add device to resolving list
    ble_error_t queue_add_device_to_resolving_list(
        advertising_peer_address_type_t peer_identity_address_type,
        const address_t &peer_identity_address,
        const irk_t &peer_irk
    );

    // Queue control block to remove device from resolving list
    ble_error_t queue_remove_device_from_resolving_list(
        advertising_peer_address_type_t peer_identity_address_type,
        const address_t &peer_identity_address
    );

    // Queue control block to clear resolving list
    ble_error_t queue_clear_resolving_list();

#if BLE_GAP_HOST_BASED_PRIVATE_ADDRESS_RESOLUTION
    struct PrivacyResolveAddressOnHost;

    void restart_resolution_process_on_host();
#endif // BLE_GAP_HOST_BASED_PRIVATE_ADDRESS_RESOLUTION

    // Clear all control blocks
    void clear_privacy_control_blocks();

    // Queue a control block
    void queue_privacy_control_block(PrivacyControlBlock *block);

    // Try to dequeue and process the next control block.
    // cb_completed is set when the previous block has completed
    void process_privacy_control_blocks(bool cb_completed);

#if BLE_GAP_HOST_BASED_PRIVATE_ADDRESS_RESOLUTION
    struct resolving_list_entry_t;

    template<typename Pred>
    void remove_resolution_entry_from_cache(const Pred& predicate);

    void add_resolution_entry_to_cache(const address_t& address, resolving_list_entry_t* identity);
#endif // BLE_GAP_HOST_BASED_PRIVATE_ADDRESS_RESOLUTION

    PalPrivateAddressController &_pal;
    PalEventQueue &_event_queue;
    ble::resolvable_address_timeout_t _rotation_timeout;
    bool _generation_started;
    irk_t _local_irk = {};
    EventHandler *_event_handler = nullptr;
    Ticker _address_rotation_ticker;
    address_t _resolvable_address = {};
    address_t _non_resolvable_address = {};

    PrivacyControlBlock *_pending_privacy_control_blocks = nullptr;
    bool _processing_privacy_control_block = false;

    uint8_t _resolving_list_size = 0;

#if BLE_GAP_HOST_BASED_PRIVATE_ADDRESS_RESOLUTION
    struct resolving_list_entry_t {
        address_t peer_address = {};
        irk_t  peer_irk = {};
        target_peer_address_type_t peer_address_type = target_peer_address_type_t::PUBLIC;
        bool populated = false;
    };

    resolving_list_entry_t _resolving_list[BLE_SECURITY_DATABASE_MAX_ENTRIES];

    struct resolution_entry_t {
        address_t address = address_t {};
        resolution_entry_t *next = nullptr;
        resolving_list_entry_t *identity = nullptr;
    };

    resolution_entry_t _resolution_list[BLE_GAP_HOST_PRIVACY_RESOLVED_CACHE_SIZE];
    resolution_entry_t *_resolved_list = nullptr;
    resolution_entry_t *_free_resolution_entries = nullptr;
#endif // BLE_GAP_HOST_BASED_PRIVATE_ADDRESS_RESOLUTION
};

}

#endif // BLE_FEATURE_PRIVACY

#endif //BLE_GENERIC_PRIVATE_ADDRESS_CONTROLLER_H
