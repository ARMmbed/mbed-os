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

#ifndef __DISCOVERED_CHARACTERISTIC_DESCRIPTOR_H__
#define __DISCOVERED_CHARACTERISTIC_DESCRIPTOR_H__

#include "UUID.h"
#include "Gap.h"
#include "GattAttribute.h"
#include "GattClient.h"
#include "CharacteristicDescriptorDiscovery.h"

/**
 * @brief Representation of a descriptor discovered during a GattClient
 * discovery procedure (see GattClient::discoverCharacteristicDescriptors or
 * DiscoveredCharacteristic::discoverDescriptors ).
 *
 * @details Provide detailed informations about a discovered characteristic descriptor
 * like:
 *     - Its UUID (see #getUUID).
 *     - Its handle (see #getAttributeHandle)
 * Basic read (see GattClient::read) and write (see GattClient::write) procedure from
 * GattClient can be used access the value of the descriptor.
 *
 * @todo read member function
 * @todo write member function
 * @todo enumeration of standard descriptors
 */
class DiscoveredCharacteristicDescriptor {

public:

    /**
     * @brief construct a new instance of a DiscoveredCharacteristicDescriptor
     *
     * @param client The client from where the descriptor has been discovered
     * @param connectionHandle The connection handle on which the descriptor has
     * been discovered
     * @param attributeHandle The handle of the attribute containing this descriptor
     * @param uuid The UUID of the descriptor
     */
    DiscoveredCharacteristicDescriptor(
        GattClient* client, Gap::Handle_t connectionHandle,  GattAttribute::Handle_t attributeHandle, const UUID& uuid) :
        _client(client), _connectionHandle(connectionHandle), _uuid(uuid), _gattHandle(attributeHandle) {

    }

    /**
     * @brief Return the GattClient which can operate on this descriptor.
     * @return The GattClient which can operate on this descriptor.
     */
    GattClient* getGattClient() {
        return _client;
    }

    /**
     * @brief Return the GattClient which can operate on this descriptor.
     * @return The GattClient which can operate on this descriptor.
     */
    const GattClient* getGattClient() const {
        return _client;
    }

    /**
     * @brief Return the connection handle to the GattServer which contain
     * this descriptor.
     * @return the connection handle to the GattServer which contain
     * this descriptor.
     */
    Gap::Handle_t getConnectionHandle() const {
        return _connectionHandle;
    }

    /**
     * @brief Return the UUID of this descriptor
     * @return the UUID of this descriptor
     */
    const UUID& getUUID(void) const {
        return _uuid;
    }

    /**
     * @brief Return the attribute handle to use to access to this descriptor
     * on the gatt server.
     * @return The attribute handle of the descriptor
     */
    GattAttribute::Handle_t getAttributeHandle() const {
        return _gattHandle;
    }

private:
    GattClient  *_client;
    Gap::Handle_t _connectionHandle;
    UUID _uuid;
    GattAttribute::Handle_t _gattHandle;
};

#endif /*__DISCOVERED_CHARACTERISTIC_DESCRIPTOR_H__*/
