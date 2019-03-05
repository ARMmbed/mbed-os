/* mbed Microcontroller Library
 * Copyright (c) 2006-2013 ARM Limited
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

#ifndef MBED_DISCOVERED_CHARACTERISTIC_DESCRIPTOR_H__
#define MBED_DISCOVERED_CHARACTERISTIC_DESCRIPTOR_H__

#include "UUID.h"
#include "ble/Gap.h"
#include "GattAttribute.h"
#include "GattClient.h"
#include "CharacteristicDescriptorDiscovery.h"

/**
 * @addtogroup ble
 * @{
 * @addtogroup gatt
 * @{
 * @addtogroup client
 * @{
 */

/**
 * Representation of a characteristic descriptor discovered.
 *
 * Characteristic descriptors can be seen as the metadata of the characteristic.
 * They can contain things such as the unit of the characteristic value, extra
 * permission informations or the Client Configuration state in regard to
 * notification or indication.
 *
 * The descriptors of a characterstic are discovered by a Characteristic
 * Descriptor Discovery Procedure, which can be initiated by either
 * GattClient::discoverCharacteristicDescriptors() or
 * DiscoveredCharacteristic::discoverDescriptors().
 *
 * The discovery procedure returns the UUID of the descriptor (its type) and its
 * handle.
 *
 * Read and write of the descriptor value can be initiated by
 * GattClient::read and GattClient::write.
 *
 * @todo read member function
 * @todo write member function
 * @todo enumeration of standard descriptors
 */
class DiscoveredCharacteristicDescriptor {

public:

    /**
     * Construct a new instance of a DiscoveredCharacteristicDescriptor.
     *
     * @param[in] client The client that has discovered the descriptor.
     * @param[in] connectionHandle Handle of the connection to the GATT server
     * containing the descriptor.
     * @param[in] attributeHandle GATT attribute handle of the descriptor.
     * @param[in] uuid UUID of the descriptor.
     *
     * @note This constructor is not meant to be called directly by application
     * code. The Gattclient class generates descriptors discovered.
     */
    DiscoveredCharacteristicDescriptor(
        GattClient *client,
        ble::connection_handle_t connectionHandle,
        GattAttribute::Handle_t attributeHandle,
        const UUID &uuid
    ) : _client(client),
        _connectionHandle(connectionHandle),
        _uuid(uuid),
        _gattHandle(attributeHandle) {
    }

    /**
     * Return the GattClient, which can operate on this descriptor.
     *
     * @return GattClient, which can operate on this descriptor.
     */
    GattClient* getGattClient()
    {
        return _client;
    }

    /**
     * Return the GattClient, which can operate on this descriptor.
     *
     * @return GattClient, which can operate on this descriptor.
     */
    const GattClient* getGattClient() const
    {
        return _client;
    }

    /**
     * Return the connection handle to the GattServer containing this
     * descriptor.
     *
     * @return the connection handle to the GattServer containing this
     * descriptor.
     */
    ble::connection_handle_t getConnectionHandle() const
    {
        return _connectionHandle;
    }

    /**
     * Return the UUID of this descriptor.
     *
     * @return UUID of this descriptor.
     */
    const UUID& getUUID(void) const
    {
        return _uuid;
    }

    /**
     * Return the attribute handle of this descriptor.
     *
     * This attribute handle can be used to interact with the descriptor on its
     * gatt server.
     *
     * @return Attribute handle of the descriptor
     */
    GattAttribute::Handle_t getAttributeHandle() const
    {
        return _gattHandle;
    }

private:
    GattClient  *_client;
    ble::connection_handle_t _connectionHandle;
    UUID _uuid;
    GattAttribute::Handle_t _gattHandle;
};

/**
 * @}
 * @}
 * @}
 */

#endif /* MBED_DISCOVERED_CHARACTERISTIC_DESCRIPTOR_H__ */
