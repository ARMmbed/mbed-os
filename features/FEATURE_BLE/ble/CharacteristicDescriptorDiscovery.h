/* mbed Microcontroller Library
 * Copyright (c) 2006-2015 ARM Limited
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

#ifndef __CHARACTERISTIC_DESCRIPTOR_DISCOVERY_H__
#define __CHARACTERISTIC_DESCRIPTOR_DISCOVERY_H__

#include "FunctionPointerWithContext.h"

class DiscoveredCharacteristic;                         // forward declaration
class DiscoveredCharacteristicDescriptor;               // forward declaration

/**
 * @brief Contain all definitions of callbacks and callbacks parameters types
 * related to characteristic descriptor discovery.
 *
 * @details This class act like a namespace for characteristic descriptor discovery
 * types. It act like ServiceDiscovery by providing callbacks and callbacks
 * parameters types related to the characteristic descriptor discovery process but
 * contrary to ServiceDiscovery class, it does not force the porter to use a
 * specific interface for the characteristic descriptor discovery process.
 */
class CharacteristicDescriptorDiscovery {
public:
    /**
     * @brief Parameter type of CharacteristicDescriptorDiscovery::DiscoveryCallback_t.
     * @details Every time a characteristic descriptor has been discovered, the callback
     * registered for the discovery operation through GattClient::discoverCharacteristicDescriptors
     * or DiscoveredCharacteristic::discoverDescriptors will be called with this parameter.
     *
     */
    struct DiscoveryCallbackParams_t {
        /**
         * The characteristic owning the DiscoveredCharacteristicDescriptor
         */
        const DiscoveredCharacteristic& characteristic;

        /**
         * The characteristic descriptor discovered
         */
        const DiscoveredCharacteristicDescriptor& descriptor;
    };

    /**
     * @brief Parameter type of CharacteristicDescriptorDiscovery::TerminationCallback_t.
     * @details Once a characteristic descriptor discovery process terminate, the termination
     * callback registered for the discovery operation through
     * GattClient::discoverCharacteristicDescriptors or DiscoveredCharacteristic::discoverDescriptors
     * will be called with this parameter.
     */
    struct TerminationCallbackParams_t {
        /**
         * The characteristic for which the descriptors has been discovered
         */
        const DiscoveredCharacteristic& characteristic;

        /**
         * status of the discovery operation
         */
        ble_error_t status;
    };

    /**
     * @brief Callback type for when a matching characteristic descriptor is found during
     * characteristic descriptor discovery.
     *
     * @param param A pointer to a DiscoveryCallbackParams_t object which will remain
     * valid for the lifetime of the callback. Memory for this object is owned by
     * the BLE_API eventing framework. The application can safely make a persistent
     * shallow-copy of this object in order to work with the service beyond the
     * callback.
     */
    typedef FunctionPointerWithContext<const DiscoveryCallbackParams_t*> DiscoveryCallback_t;

    /**
     * @brief Callback type for when characteristic descriptor discovery terminates.
     *
     * @param param A pointer to a TerminationCallbackParams_t object which will remain
     * valid for the lifetime of the callback. Memory for this object is owned by
     * the BLE_API eventing framework. The application can safely make a persistent
     * shallow-copy of this object in order to work with the service beyond the
     * callback.
     */
    typedef FunctionPointerWithContext<const TerminationCallbackParams_t*> TerminationCallback_t;
};

#endif // ifndef __CHARACTERISTIC_DESCRIPTOR_DISCOVERY_H__
