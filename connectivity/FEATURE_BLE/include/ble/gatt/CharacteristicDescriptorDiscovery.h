/* mbed Microcontroller Library
 * Copyright (c) 2006-2020 ARM Limited
 *
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

#ifndef MBED_CHARACTERISTIC_DESCRIPTOR_DISCOVERY_H__
#define MBED_CHARACTERISTIC_DESCRIPTOR_DISCOVERY_H__

#include "ble/common/FunctionPointerWithContext.h"

class DiscoveredCharacteristic;                         // forward declaration
class DiscoveredCharacteristicDescriptor;               // forward declaration

/**
 * @addtogroup ble
 * @{
 * @addtogroup gatt
 * @{
 */

/**
 * Definitions of events and event handlers that the characteristic descriptor
 * discovery procedure uses.
 *
 * This class acts like a namespace for characteristic descriptor discovery
 * types. Like ServiceDiscovery, it provides callbacks and callbacks parameters
 * types related to the characteristic descriptor discovery process, but contrary
 * to the ServiceDiscovery class, it does not force the porter to use a specific
 * interface for the characteristic descriptor discovery process.
 */
class CharacteristicDescriptorDiscovery {
public:
    /**
     * Characteristic descriptor discovered event.
     *
     * When a characteristic descriptor has been discovered, the callback
     * registered for the discovery operation through
     * GattClient::discoverCharacteristicDescriptors or
     * DiscoveredCharacteristic::discoverDescriptors is called with an instance
     * of this type.
     *
     * The values reported to the user are the descriptor discovered and the
     * characteristic owning that descriptor.
     *
     * @see GattClient::discoverCharacteristicDescriptors
     * DiscoveredCharacteristic::discoverDescriptors
     */
    struct DiscoveryCallbackParams_t {
        /**
         * Characteristic owning the descriptor discovered.
         */
        const DiscoveredCharacteristic &characteristic;

        /**
         * Characteristic descriptor discovered.
         */
        const DiscoveredCharacteristicDescriptor &descriptor;
    };

    /**
     * Characteristic descriptor discovery ended event.
     *
     * When the characteristic descriptor discovery process ends, the
     * termination callback registered for the discovery operation through
     * GattClient::discoverCharacteristicDescriptors or
     * DiscoveredCharacteristic::discoverDescriptors will be called with an
     * instance of this type.
     *
     * @see GattClient::discoverCharacteristicDescriptors
     * DiscoveredCharacteristic::discoverDescriptors
     */
    struct TerminationCallbackParams_t {
        /**
         * Characteristic for which descriptors has been discovered.
         */
        const DiscoveredCharacteristic& characteristic;

        /**
         * Status of the discovery operation.
         */
        ble_error_t status;

        /**
         * Error code associated with the status if any.
         */
        uint8_t error_code;
    };

    /**
     * Characteristic descriptor discovered event handler.
     *
     * As a parameter, it expects a pointer to a DiscoveryCallbackParams_t instance.
     *
     * @note The object passed in parameter will remain valid for the lifetime
     * of the callback. The BLE_API eventing framework owns memory for this
     * object. The application can safely make a persistent shallow-copy of
     * this object to work with the service beyond the callback.
     *
     * @see DiscoveryCallbackParams_t
     * GattClient::discoverCharacteristicDescriptors
     * DiscoveredCharacteristic::discoverDescriptors
     */
    typedef FunctionPointerWithContext<const DiscoveryCallbackParams_t*>
        DiscoveryCallback_t;

    /**
     * Handler of Characteristic descriptor discovery ended event.
     *
     * As a parameter, it expects a pointer to a TerminationCallbackParams_t instance.
     *
     * @note The object passed in parameter will remain valid for the lifetime
     * of the callback. The BLE_API eventing framework owns the memory for this
     * object. The application can safely make a persistent shallow-copy of
     * this object to work with the service beyond the callback.
     *
     * @see TerminationCallbackParams_t
     * GattClient::discoverCharacteristicDescriptors
     * DiscoveredCharacteristic::discoverDescriptors
     */
    typedef FunctionPointerWithContext<const TerminationCallbackParams_t*>
        TerminationCallback_t;
};

/**
 * @}
 * @}
 */

#endif // ifndef MBED_CHARACTERISTIC_DESCRIPTOR_DISCOVERY_H__
