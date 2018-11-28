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

#include "nRF5xServiceDiscovery.h"

ble_error_t
nRF5xServiceDiscovery::launchCharacteristicDiscovery(Gap::Handle_t connectionHandle,
                                                     Gap::Handle_t startHandle,
                                                     Gap::Handle_t endHandle)
{
    characteristicDiscoveryStarted(connectionHandle);

    ble_gattc_handle_range_t handleRange = {
        (uint16_t) startHandle,
        (uint16_t) endHandle
    };
    uint32_t rc = sd_ble_gattc_characteristics_discover(connectionHandle, &handleRange);
    ble_error_t err = BLE_ERROR_NONE;

    switch (rc) {
        case NRF_SUCCESS:
            err = BLE_ERROR_NONE;
            break;
        case BLE_ERROR_INVALID_CONN_HANDLE:
        case NRF_ERROR_INVALID_ADDR:
            err = BLE_ERROR_INVALID_PARAM;
            break;
        case NRF_ERROR_BUSY:
            err = BLE_STACK_BUSY;
            break;
        case NRF_ERROR_INVALID_STATE:
            err = BLE_ERROR_INVALID_STATE;
            break;
        default:
            err = BLE_ERROR_UNSPECIFIED;
            break;
    }

    if (err) {
        terminateCharacteristicDiscovery(err);
    }
    return err;
}

void
nRF5xServiceDiscovery::setupDiscoveredServices(const ble_gattc_evt_prim_srvc_disc_rsp_t *response)
{
    serviceIndex = 0;
    numServices  = response->count;

    /* Account for the limitation on the number of discovered services we can handle at a time. */
    if (numServices > BLE_DB_DISCOVERY_MAX_SRV) {
        numServices = BLE_DB_DISCOVERY_MAX_SRV;
    }

    serviceUUIDDiscoveryQueue.reset();
    for (unsigned serviceIndex = 0; serviceIndex < numServices; serviceIndex++) {
        if (response->services[serviceIndex].uuid.type == BLE_UUID_TYPE_UNKNOWN) {
            serviceUUIDDiscoveryQueue.enqueue(serviceIndex);
            services[serviceIndex].setup(response->services[serviceIndex].handle_range.start_handle,
                                         response->services[serviceIndex].handle_range.end_handle);
        } else {
            services[serviceIndex].setup(response->services[serviceIndex].uuid.uuid,
                                         response->services[serviceIndex].handle_range.start_handle,
                                         response->services[serviceIndex].handle_range.end_handle);
        }
    }

    /* Trigger discovery of service UUID if necessary. */
    if (serviceUUIDDiscoveryQueue.getCount()) {
        serviceUUIDDiscoveryQueue.triggerFirst();
    }
}

void
nRF5xServiceDiscovery::setupDiscoveredCharacteristics(const ble_gattc_evt_char_disc_rsp_t *response)
{
    numCharacteristics  = response->count;

    /* Account for the limitation on the number of discovered characteristics we can handle at a time. */
    if (numCharacteristics > BLE_DB_DISCOVERY_MAX_CHAR_PER_SRV) {
        numCharacteristics = BLE_DB_DISCOVERY_MAX_CHAR_PER_SRV;
    }

    charUUIDDiscoveryQueue.reset();
    for (unsigned charIndex = 0; charIndex < numCharacteristics; charIndex++) {
        if (response->chars[charIndex].uuid.type == BLE_UUID_TYPE_UNKNOWN) {
            charUUIDDiscoveryQueue.enqueue(charIndex);
            characteristics[charIndex].setup(gattc,
                                             connHandle,
                                             response->chars[charIndex].char_props,
                                             response->chars[charIndex].handle_decl,
                                             response->chars[charIndex].handle_value);
        } else {
            characteristics[charIndex].setup(gattc,
                                             connHandle,
                                             response->chars[charIndex].uuid.uuid,
                                             response->chars[charIndex].char_props,
                                             response->chars[charIndex].handle_decl,
                                             response->chars[charIndex].handle_value);
        }
    }

    /* Trigger discovery of char UUID if necessary. */
    if (charUUIDDiscoveryQueue.getCount()) {
        charUUIDDiscoveryQueue.triggerFirst();
    }
}

void
nRF5xServiceDiscovery::progressCharacteristicDiscovery(void)
{
    if (state != CHARACTERISTIC_DISCOVERY_ACTIVE) {
        return;
    }

    if ((discoveredCharacteristic != nRF5xDiscoveredCharacteristic()) && (numCharacteristics > 0)) {
        discoveredCharacteristic.setLastHandle(characteristics[0].getDeclHandle() - 1);

        if ((matchingCharacteristicUUID == UUID::ShortUUIDBytes_t(BLE_UUID_UNKNOWN)) ||
            ((matchingCharacteristicUUID == discoveredCharacteristic.getUUID()) &&
             (matchingServiceUUID != UUID::ShortUUIDBytes_t(BLE_UUID_UNKNOWN)))) {
            if (characteristicCallback) {
                characteristicCallback(&discoveredCharacteristic);
            }
        }
    }

    for (uint8_t i = 0; i < numCharacteristics; ++i) {
        if (state != CHARACTERISTIC_DISCOVERY_ACTIVE) {
            return;
        }

        if (i == (numCharacteristics - 1)) {
            discoveredCharacteristic = characteristics[i];
            break;
        } else {
            characteristics[i].setLastHandle(characteristics[i + 1].getDeclHandle() - 1);
        }

        if ((matchingCharacteristicUUID == UUID::ShortUUIDBytes_t(BLE_UUID_UNKNOWN)) ||
            ((matchingCharacteristicUUID == characteristics[i].getUUID()) &&
             (matchingServiceUUID != UUID::ShortUUIDBytes_t(BLE_UUID_UNKNOWN)))) {
            if (characteristicCallback) {
                characteristicCallback(&characteristics[i]);
            }
        }
    }

    if (state != CHARACTERISTIC_DISCOVERY_ACTIVE) {
        return;
    }

    Gap::Handle_t startHandle = (numCharacteristics > 0) ? characteristics[numCharacteristics - 1].getValueHandle() + 1 : SRV_DISC_END_HANDLE;
    Gap::Handle_t endHandle   = services[serviceIndex].getEndHandle();
    resetDiscoveredCharacteristics(); /* Note: resetDiscoveredCharacteristics() must come after fetching start and end Handles. */

    if (startHandle < endHandle) {
        ble_gattc_handle_range_t handleRange = {
            (uint16_t) startHandle,
            (uint16_t) endHandle
        };
        if (sd_ble_gattc_characteristics_discover(connHandle, &handleRange) != NRF_SUCCESS) {
            terminateCharacteristicDiscovery(BLE_ERROR_UNSPECIFIED);
        }
    } else {
        terminateCharacteristicDiscovery(BLE_ERROR_NONE);
    }
}

void
nRF5xServiceDiscovery::progressServiceDiscovery(void)
{
    /* Iterate through the previously discovered services cached in services[]. */
    while ((state == SERVICE_DISCOVERY_ACTIVE) && (serviceIndex < numServices)) {
        if ((matchingServiceUUID == UUID::ShortUUIDBytes_t(BLE_UUID_UNKNOWN)) ||
            (matchingServiceUUID == services[serviceIndex].getUUID())) {

            if (serviceCallback && (matchingCharacteristicUUID == UUID::ShortUUIDBytes_t(BLE_UUID_UNKNOWN))) {
                serviceCallback(&services[serviceIndex]);
            }

            if ((state == SERVICE_DISCOVERY_ACTIVE) && characteristicCallback) {
                launchCharacteristicDiscovery(connHandle, services[serviceIndex].getStartHandle(), services[serviceIndex].getEndHandle());
            } else {
                serviceIndex++;
            }
        } else {
            serviceIndex++;
        }
    }

    /* Relaunch discovery of new services beyond the last entry cached in services[]. */
    if ((state == SERVICE_DISCOVERY_ACTIVE) && (numServices > 0) && (serviceIndex > 0)) {
        /* Determine the ending handle of the last cached service. */
        Gap::Handle_t endHandle = services[serviceIndex - 1].getEndHandle();
        resetDiscoveredServices(); /* Note: resetDiscoveredServices() must come after fetching endHandle. */

        if (endHandle == SRV_DISC_END_HANDLE) {
            terminateServiceDiscovery();
        } else {
            if (sd_ble_gattc_primary_services_discover(connHandle, endHandle, NULL) != NRF_SUCCESS) {
                terminateServiceDiscovery();
            }
        }
    }
}

void
nRF5xServiceDiscovery::ServiceUUIDDiscoveryQueue::triggerFirst(void)
{
    while (numIndices) { /* loop until a call to char_value_by_uuid_read() succeeds or we run out of pending indices. */
        parentDiscoveryObject->state = DISCOVER_SERVICE_UUIDS;

        unsigned serviceIndex = getFirst();
        ble_uuid_t uuid = {
            .uuid = BLE_UUID_SERVICE_PRIMARY,
            .type = BLE_UUID_TYPE_BLE,
        };
        ble_gattc_handle_range_t handleRange = {
            .start_handle = parentDiscoveryObject->services[serviceIndex].getStartHandle(),
            .end_handle   = parentDiscoveryObject->services[serviceIndex].getEndHandle(),
        };
        if (sd_ble_gattc_char_value_by_uuid_read(parentDiscoveryObject->connHandle, &uuid, &handleRange) == NRF_SUCCESS) {
            return;
        }

        /* Skip this service if we fail to launch a read for its service-declaration
         * attribute. Its UUID will remain INVALID, and it may not match any filters. */
        dequeue();
    }

    /* Switch back to service discovery upon exhausting the service-indices pending UUID discovery. */
    if (parentDiscoveryObject->state == DISCOVER_SERVICE_UUIDS) {
        parentDiscoveryObject->state = SERVICE_DISCOVERY_ACTIVE;
    }
}

void
nRF5xServiceDiscovery::CharUUIDDiscoveryQueue::triggerFirst(void)
{
    while (numIndices) { /* loop until a call to char_value_by_uuid_read() succeeds or we run out of pending indices. */
        parentDiscoveryObject->state = DISCOVER_CHARACTERISTIC_UUIDS;

        unsigned charIndex = getFirst();
        ble_uuid_t uuid = {
            .uuid = BLE_UUID_CHARACTERISTIC,
            .type = BLE_UUID_TYPE_BLE,
        };
        ble_gattc_handle_range_t handleRange = { };
        handleRange.start_handle = parentDiscoveryObject->characteristics[charIndex].getDeclHandle();
        handleRange.end_handle   = parentDiscoveryObject->characteristics[charIndex].getDeclHandle() + 1;
        if (sd_ble_gattc_char_value_by_uuid_read(parentDiscoveryObject->connHandle, &uuid, &handleRange) == NRF_SUCCESS) {
            return;
        }

        /* Skip this service if we fail to launch a read for its service-declaration
         * attribute. Its UUID will remain INVALID, and it may not match any filters. */
        dequeue();
    }

    /* Switch back to service discovery upon exhausting the service-indices pending UUID discovery. */
    if (parentDiscoveryObject->state == DISCOVER_CHARACTERISTIC_UUIDS) {
        parentDiscoveryObject->state = CHARACTERISTIC_DISCOVERY_ACTIVE;
    }
}

void
nRF5xServiceDiscovery::processDiscoverUUIDResponse(const ble_gattc_evt_char_val_by_uuid_read_rsp_t *response)
{
    if (state == DISCOVER_SERVICE_UUIDS) {
        if ((response->count == 1) && (response->value_len == UUID::LENGTH_OF_LONG_UUID)) {
            UUID::LongUUIDBytes_t uuid;
            memcpy(uuid, response->handle_value[0].p_value, UUID::LENGTH_OF_LONG_UUID);

            unsigned serviceIndex = serviceUUIDDiscoveryQueue.dequeue();
            services[serviceIndex].setupLongUUID(uuid, UUID::LSB);

            serviceUUIDDiscoveryQueue.triggerFirst();
        } else {
            serviceUUIDDiscoveryQueue.dequeue();
        }
    } else if (state == DISCOVER_CHARACTERISTIC_UUIDS) {
        if ((response->count == 1) && (response->value_len == UUID::LENGTH_OF_LONG_UUID + 1 /* props */ + 2 /* value handle */)) {
            UUID::LongUUIDBytes_t uuid;

            memcpy(uuid, &(response->handle_value[0].p_value[3]), UUID::LENGTH_OF_LONG_UUID);

            unsigned charIndex = charUUIDDiscoveryQueue.dequeue();
            characteristics[charIndex].setupLongUUID(uuid, UUID::LSB);

            charUUIDDiscoveryQueue.triggerFirst();
        } else {
            charUUIDDiscoveryQueue.dequeue();
        }
    }
}
