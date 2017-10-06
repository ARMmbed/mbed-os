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

#ifndef MBED_BLE_GENERIC_GATT_CLIENT
#define MBED_BLE_GENERIC_GATT_CLIENT

#include <algorithm>
#include "ble/GattClient.h"
#include "ble/pal/PalGattClient.h"

// IMPORTANT: private header. Not part of the public interface.

namespace ble {
namespace generic {

// forward declarations
struct procedure_control_block_t;
struct discovery_control_block_t;
struct read_control_block_t;
struct write_control_block_t;
struct descriptor_discovery_control_block_t;

/**
 * Generic implementation of the GattClient.
 * It requires a pal::GattClient injected at construction site.
 * @important: Not part of the public interface of BLE API.
 */
class GenericGattClient : public GattClient {

    // give access to control block classes
    friend struct procedure_control_block_t;
    friend struct discovery_control_block_t;
    friend struct read_control_block_t;
    friend struct write_control_block_t;
    friend struct descriptor_discovery_control_block_t;

public:
    /**
     * Create a GenericGattClient from a pal::GattClient
     */
    GenericGattClient(pal::GattClient* pal_client);

    /**
     * @see GattClient::launchServiceDiscovery
     */
    virtual ble_error_t launchServiceDiscovery(
        Gap::Handle_t connection_handle,
        ServiceDiscovery::ServiceCallback_t service_callback,
        ServiceDiscovery::CharacteristicCallback_t characteristic_callback,
        const UUID& matching_service_uuid,
        const UUID& matching_characteristic_uuid
    );

	/**
	 * @see GattClient::isServiceDiscoveryActive
	 */
    virtual bool isServiceDiscoveryActive() const;

	/**
	 * @see GattClient::terminateServiceDiscovery
	 */
    virtual void terminateServiceDiscovery();

	/**
	 * @see GattClient::read
	 */
    virtual ble_error_t read(
        Gap::Handle_t connection_handle,
        GattAttribute::Handle_t attribute_handle,
        uint16_t offset
    ) const;

	/**
	 * @see GattClient::write
	 */
    virtual ble_error_t write(
        GattClient::WriteOp_t cmd,
        Gap::Handle_t connection_handle,
        GattAttribute::Handle_t attribute_handle,
        size_t length,
        const uint8_t* value
    ) const;

	/**
	 * @see GattClient::onServiceDiscoveryTermination
	 */
    virtual void onServiceDiscoveryTermination(
        ServiceDiscovery::TerminationCallback_t callback
    );

	/**
	 * @see GattClient::discoverCharacteristicDescriptors
	 */
    virtual ble_error_t discoverCharacteristicDescriptors(
        const DiscoveredCharacteristic& characteristic,
        const CharacteristicDescriptorDiscovery::DiscoveryCallback_t& discoveryCallback,
        const CharacteristicDescriptorDiscovery::TerminationCallback_t& terminationCallback
    );

	/**
	 * @see GattClient::isCharacteristicDescriptorDiscoveryActive
	 */
    virtual bool isCharacteristicDescriptorDiscoveryActive(
        const DiscoveredCharacteristic& characteristic
    ) const;

	/**
	 * @see GattClient::terminateCharacteristicDescriptorDiscovery
	 */
    virtual void terminateCharacteristicDescriptorDiscovery(
        const DiscoveredCharacteristic& characteristic
    );

	/**
	 * @see GattClient::reset
	 */
    virtual ble_error_t reset(void);

private:
    procedure_control_block_t* get_control_block(Gap::Handle_t connection);
    const procedure_control_block_t* get_control_block(Gap::Handle_t connection) const;
    void insert_control_block(procedure_control_block_t* cb) const;
    void remove_control_block(procedure_control_block_t* cb) const;

    void on_termination(Gap::Handle_t connection_handle);
    void on_server_message_received(connection_handle_t, const pal::AttServerMessage&);
    void on_server_response(connection_handle_t, const pal::AttServerMessage&);
    void on_server_event(connection_handle_t, const pal::AttServerMessage&);
    void on_transaction_timeout(connection_handle_t);

    uint16_t get_mtu(Gap::Handle_t connection) const;

    pal::GattClient* const _pal_client;
    ServiceDiscovery::TerminationCallback_t _termination_callback;
    mutable procedure_control_block_t* control_blocks;
};

}
}

#endif /* MBED_BLE_GENERIC_GATT_CLIENT */
