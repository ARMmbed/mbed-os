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

#include "nRF5xGattClient.h"

#if !defined(TARGET_MCU_NRF51_16K_S110) && !defined(TARGET_MCU_NRF51_32K_S110)
ble_error_t
nRF5xGattClient::launchServiceDiscovery(Gap::Handle_t                               connectionHandle,
                                        ServiceDiscovery::ServiceCallback_t         sc,
                                        ServiceDiscovery::CharacteristicCallback_t  cc,
                                        const UUID                                 &matchingServiceUUIDIn,
                                        const UUID                                 &matchingCharacteristicUUIDIn)
{
    return _discovery.launch(connectionHandle, sc, cc, matchingServiceUUIDIn, matchingCharacteristicUUIDIn);
}

ble_error_t nRF5xGattClient::discoverCharacteristicDescriptors(
    const DiscoveredCharacteristic& characteristic,
    const CharacteristicDescriptorDiscovery::DiscoveryCallback_t& discoveryCallback,
    const CharacteristicDescriptorDiscovery::TerminationCallback_t& terminationCallback)
{
    return _characteristicDescriptorDiscoverer.launch(
        characteristic, 
        discoveryCallback, 
        terminationCallback
    );
}

bool nRF5xGattClient::isCharacteristicDescriptorsDiscoveryActive(const DiscoveredCharacteristic& characteristic) const {
    return _characteristicDescriptorDiscoverer.isActive(characteristic);   
}

void nRF5xGattClient::terminateCharacteristicDescriptorsDiscovery(const DiscoveredCharacteristic& characteristic) { 
    return _characteristicDescriptorDiscoverer.requestTerminate(characteristic);
}

#endif
