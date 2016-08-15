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
#include "nRF5xCharacteristicDescriptorDiscoverer.h"
#include "ble_err.h"
#include "ble/DiscoveredCharacteristicDescriptor.h"

nRF5xCharacteristicDescriptorDiscoverer::nRF5xCharacteristicDescriptorDiscoverer() :
    discoveryRunning() {
    // nothing to do
}

nRF5xCharacteristicDescriptorDiscoverer::~nRF5xCharacteristicDescriptorDiscoverer() {
    // nothing to do
}

ble_error_t nRF5xCharacteristicDescriptorDiscoverer::launch(
    const DiscoveredCharacteristic& characteristic,
    const CharacteristicDescriptorDiscovery::DiscoveryCallback_t& discoveryCallback,
    const CharacteristicDescriptorDiscovery::TerminationCallback_t& terminationCallback
) {
    Gap::Handle_t connHandle = characteristic.getConnectionHandle();
    // it is ok to deduce that the start handle for descriptors is after
    // the characteristic declaration and the characteristic value declaration
    // see BLUETOOTH SPECIFICATION Version 4.2 [Vol 3, Part G] (3.3)
    Gap::Handle_t descriptorStartHandle = characteristic.getDeclHandle() + 2;
    Gap::Handle_t descriptorEndHandle = characteristic.getLastHandle();

    // check if there is any descriptor to discover
    if (descriptorEndHandle < descriptorStartHandle) {
        CharacteristicDescriptorDiscovery::TerminationCallbackParams_t termParams = {
            characteristic,
            BLE_ERROR_NONE
        };
        terminationCallback.call(&termParams);
        return BLE_ERROR_NONE;
    }

    // check if we can run this discovery
    if (isConnectionInUse(connHandle)) {
        return BLE_STACK_BUSY;
    }

    // get a new discovery slot, if none are available, just return
    Discovery* discovery = getAvailableDiscoverySlot();
    if(discovery == NULL) {
        return BLE_STACK_BUSY;
    }

    // try to launch the discovery
    ble_error_t err = gattc_descriptors_discover(connHandle, descriptorStartHandle, descriptorEndHandle);
    if(!err) {
        // commit the new discovery to its slot
        *discovery = Discovery(characteristic, discoveryCallback, terminationCallback);
    }

    return err;
}

bool nRF5xCharacteristicDescriptorDiscoverer::isActive(const DiscoveredCharacteristic& characteristic) const {
    for(size_t i = 0; i < MAXIMUM_CONCURRENT_CONNECTIONS_COUNT; ++i) {
        if(discoveryRunning[i].getCharacteristic() == characteristic) {
            return true;
        }
    }
    return false;
}

void nRF5xCharacteristicDescriptorDiscoverer::requestTerminate(const DiscoveredCharacteristic& characteristic) {
    Discovery* discovery = findRunningDiscovery(characteristic);
    if(discovery) {
        // call terminate anyway
        terminate(discovery, BLE_ERROR_NONE);
    }
}

void nRF5xCharacteristicDescriptorDiscoverer::process(uint16_t connectionHandle, const ble_gattc_evt_desc_disc_rsp_t& descriptors) {
    Discovery* discovery = findRunningDiscovery(connectionHandle);
    // the discovery has been removed
    if(!discovery) {
        return;
    }

    for (uint16_t i = 0; i < descriptors.count; ++i) {
        discovery->process(
            descriptors.descs[i].handle, UUID(descriptors.descs[i].uuid.uuid)
        );
    }

    // prepare the next discovery request (if needed)
    uint16_t startHandle = descriptors.descs[descriptors.count - 1].handle + 1;
    uint16_t endHandle = discovery->getCharacteristic().getLastHandle();

    if(startHandle > endHandle) {
        terminate(discovery, BLE_ERROR_NONE);
        return;
    }

    ble_error_t err = gattc_descriptors_discover(connectionHandle, startHandle, endHandle);
    if(err) {
        terminate(discovery, err);
        return;
    }
}

void nRF5xCharacteristicDescriptorDiscoverer::terminate(uint16_t handle, ble_error_t err) {
    Discovery* discovery = findRunningDiscovery(handle);
    // the discovery has already been terminated
    if(!discovery) {
        return;
    }

    terminate(discovery, err);
}

void nRF5xCharacteristicDescriptorDiscoverer::terminate(Discovery* discovery, ble_error_t err) {
    // temporary copy, user code can try to launch a new discovery in the onTerminate
    // callback. So, this discovery should not appear in such case.
    Discovery tmp = *discovery;
    *discovery = Discovery();
    tmp.terminate(err);
}

nRF5xCharacteristicDescriptorDiscoverer::Discovery*
nRF5xCharacteristicDescriptorDiscoverer::findRunningDiscovery(const DiscoveredCharacteristic& characteristic) {
    for(size_t i = 0; i < MAXIMUM_CONCURRENT_CONNECTIONS_COUNT; ++i) {
        if((discoveryRunning[i].getCharacteristic() == characteristic) &&
           (discoveryRunning[i].isEmpty() == false)) {
            return &discoveryRunning[i];
        }
    }
    return NULL;
}

nRF5xCharacteristicDescriptorDiscoverer::Discovery*
nRF5xCharacteristicDescriptorDiscoverer::findRunningDiscovery(uint16_t handle) {
    for(size_t i = 0; i < MAXIMUM_CONCURRENT_CONNECTIONS_COUNT; ++i) {
        if((discoveryRunning[i].getCharacteristic().getConnectionHandle() == handle) &&
           (discoveryRunning[i].isEmpty() == false)) {
            return &discoveryRunning[i];
        }
    }
    return NULL;
}

nRF5xCharacteristicDescriptorDiscoverer::Discovery*
nRF5xCharacteristicDescriptorDiscoverer::getAvailableDiscoverySlot() {
    for(size_t i = 0; i < MAXIMUM_CONCURRENT_CONNECTIONS_COUNT; ++i) {
        if(discoveryRunning[i].isEmpty()) {
            return &discoveryRunning[i];
        }
    }
    return NULL;
}

bool nRF5xCharacteristicDescriptorDiscoverer::isConnectionInUse(uint16_t connHandle) {
     return findRunningDiscovery(connHandle) != NULL;
}

ble_error_t nRF5xCharacteristicDescriptorDiscoverer::gattc_descriptors_discover(
    uint16_t connection_handle, uint16_t start_handle, uint16_t end_handle) {

    ble_gattc_handle_range_t discoveryRange = {
        start_handle,
        end_handle
    };
    uint32_t err = sd_ble_gattc_descriptors_discover(connection_handle, &discoveryRange);

    switch(err) {
        case NRF_SUCCESS:
            return BLE_ERROR_NONE;
        case BLE_ERROR_INVALID_CONN_HANDLE:
            return BLE_ERROR_INVALID_PARAM;
        case NRF_ERROR_INVALID_ADDR:
            return BLE_ERROR_PARAM_OUT_OF_RANGE;
        case NRF_ERROR_BUSY:
            return BLE_STACK_BUSY;
        default:
            return BLE_ERROR_UNSPECIFIED;
    }
}

// implementation of nRF5xCharacteristicDescriptorDiscoverer::Discovery

nRF5xCharacteristicDescriptorDiscoverer::Discovery::Discovery() :
    characteristic(), onDiscovery(), onTerminate() {
}

nRF5xCharacteristicDescriptorDiscoverer::Discovery::Discovery(
    const DiscoveredCharacteristic& c, const DiscoveryCallback_t& dCb, const TerminationCallback_t& tCb) :
    characteristic(c), onDiscovery(dCb), onTerminate(tCb) {
}

void nRF5xCharacteristicDescriptorDiscoverer::Discovery::process(
    GattAttribute::Handle_t handle, const UUID& uuid) {
    CharacteristicDescriptorDiscovery::DiscoveryCallbackParams_t params = {
        characteristic,
        DiscoveredCharacteristicDescriptor(
            characteristic.getGattClient(),
            characteristic.getConnectionHandle(),
            handle,
            uuid
        )
    };
    onDiscovery.call(&params);
}

void nRF5xCharacteristicDescriptorDiscoverer::Discovery::terminate(ble_error_t err) {
    CharacteristicDescriptorDiscovery::TerminationCallbackParams_t params = {
        characteristic,
        err
    };

    onTerminate.call(&params);
}

bool nRF5xCharacteristicDescriptorDiscoverer::Discovery::isEmpty() const {
    return *this == Discovery();
}

const DiscoveredCharacteristic& nRF5xCharacteristicDescriptorDiscoverer::Discovery::getCharacteristic() const {
    return characteristic;
}
