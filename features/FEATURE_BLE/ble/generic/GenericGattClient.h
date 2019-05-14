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
#include "ble/pal/SigningEventMonitor.h"

// IMPORTANT: private header. Not part of the public interface.

namespace ble {
namespace generic {

/**
 * Generic implementation of the GattClient.
 * It requires a pal::GattClient injected at construction site.
 * @attention: Not part of the public interface of BLE API.
 */
template<template<class> class TPalGattClient, class SigningMonitorEventHandler>
class GenericGattClient :
    public interface::GattClient<GenericGattClient<TPalGattClient, SigningMonitorEventHandler> >,
    public pal::SigningEventMonitor<GenericGattClient<TPalGattClient, SigningMonitorEventHandler>, SigningMonitorEventHandler>,
    public pal::GattClientEventHandler<GenericGattClient<TPalGattClient, SigningMonitorEventHandler> > {

    using interface::GattClient<GenericGattClient<TPalGattClient, SigningMonitorEventHandler> >::eventHandler;

public:

    typedef interface::GattClient<GenericGattClient<TPalGattClient, SigningMonitorEventHandler> > Base;
    typedef TPalGattClient<GenericGattClient> PalGattClient;
    typedef typename Base::WriteOp_t WriteOp_t;

    /**
     * Create a GenericGattClient from a pal::GattClient
     */
    GenericGattClient(PalGattClient* pal_client);

    /**
     * @see GattClient::launchServiceDiscovery
     */
    ble_error_t launchServiceDiscovery_(
        connection_handle_t connection_handle,
        ServiceDiscovery::ServiceCallback_t service_callback,
        ServiceDiscovery::CharacteristicCallback_t characteristic_callback,
        const UUID& matching_service_uuid,
        const UUID& matching_characteristic_uuid
    );

	/**
	 * @see GattClient::isServiceDiscoveryActive
	 */
    bool isServiceDiscoveryActive_() const;

	/**
	 * @see GattClient::terminateServiceDiscovery
	 */
    void terminateServiceDiscovery_();

	/**
	 * @see GattClient::read
	 */
    ble_error_t read_(
        connection_handle_t connection_handle,
        GattAttribute::Handle_t attribute_handle,
        uint16_t offset
    ) const;

	/**
	 * @see GattClient::write
	 */
    ble_error_t write_(
        WriteOp_t cmd,
        connection_handle_t connection_handle,
        GattAttribute::Handle_t attribute_handle,
        size_t length,
        const uint8_t* value
    ) const;

	/**
	 * @see GattClient::onServiceDiscoveryTermination
	 */
    void onServiceDiscoveryTermination_(
        ServiceDiscovery::TerminationCallback_t callback
    );

	/**
	 * @see GattClient::discoverCharacteristicDescriptors
	 */
    ble_error_t discoverCharacteristicDescriptors_(
        const DiscoveredCharacteristic& characteristic,
        const CharacteristicDescriptorDiscovery::DiscoveryCallback_t& discoveryCallback,
        const CharacteristicDescriptorDiscovery::TerminationCallback_t& terminationCallback
    );

	/**
	 * @see GattClient::isCharacteristicDescriptorDiscoveryActive
	 */
    bool isCharacteristicDescriptorDiscoveryActive_(
        const DiscoveredCharacteristic& characteristic
    ) const;

	/**
	 * @see GattClient::terminateCharacteristicDescriptorDiscovery
	 */
    void terminateCharacteristicDescriptorDiscovery_(
        const DiscoveredCharacteristic& characteristic
    );

    /**
     * @see GattClient::negotiateAttMtu
     */
    ble_error_t negotiateAttMtu_(
        connection_handle_t connection
    );

	/**
	 * @see GattClient::reset
	 */
    ble_error_t reset_(void);

    /**
     * @see ble::pal::SigningEventMonitor::set_signing_event_handler
     */
    void set_signing_event_handler_(SigningMonitorEventHandler *signing_event_handler);

    /**
     * @see pal::GattClient::EventHandler::on_att_mtu_change
     */
    void on_att_mtu_change_(
        ble::connection_handle_t connection_handle,
        uint16_t att_mtu_size
    );

    /**
     * @see pal::GattClient::EventHandler::on_write_command_sent
     */
    void on_write_command_sent_(
        ble::connection_handle_t connection_handle,
        ble::attribute_handle_t attribute_handle,
        uint8_t status
    );

private:
    struct ProcedureControlBlock;
    struct DiscoveryControlBlock;
    struct ReadControlBlock;
    struct WriteControlBlock;
    struct DescriptorDiscoveryControlBlock;

    ProcedureControlBlock* get_control_block(connection_handle_t connection);
    const ProcedureControlBlock* get_control_block(connection_handle_t connection) const;
    void insert_control_block(ProcedureControlBlock* cb) const;
    void remove_control_block(ProcedureControlBlock* cb) const;

    void on_termination(connection_handle_t connection_handle);
    void on_server_message_received(connection_handle_t, const pal::AttServerMessage&);
    void on_server_response(connection_handle_t, const pal::AttServerMessage&);
    void on_server_event(connection_handle_t, const pal::AttServerMessage&);
    void on_transaction_timeout(connection_handle_t);

    uint16_t get_mtu(connection_handle_t connection) const;

    PalGattClient* const _pal_client;
    ServiceDiscovery::TerminationCallback_t _termination_callback;
    SigningMonitorEventHandler* _signing_event_handler;
    mutable ProcedureControlBlock* control_blocks;
    bool _is_reseting;
};

} // generic
} // ble

#endif /* MBED_BLE_GENERIC_GATT_CLIENT */
