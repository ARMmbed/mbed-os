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

/* MBED_DEPRECATED */
#warning "These services are deprecated and will be removed. Please see services.md for details about replacement services."

#ifndef __BLE_DEVICE_INFORMATION_SERVICE_H__
#define __BLE_DEVICE_INFORMATION_SERVICE_H__

#include "ble/BLE.h"
#include "ble/Gap.h"
#include "ble/GattServer.h"

#if BLE_FEATURE_GATT_SERVER

/**
* @class DeviceInformationService
* @brief BLE Device Information Service
* Service: https://developer.bluetooth.org/gatt/services/Pages/ServiceViewer.aspx?u=org.bluetooth.service.device_information.xml
* Manufacturer Name String Char: https://developer.bluetooth.org/gatt/characteristics/Pages/CharacteristicViewer.aspx?u=org.bluetooth.characteristic.manufacturer_name_string.xml
*/
class DeviceInformationService {
public:
    /**
     * @brief Device Information Service Constructor: copies device-specific information
     * into the BLE stack.
     *
     * @param[in] _ble
     *                A reference to a BLE object for the underlying controller.
     * @param[in] manufacturersName
     *                The name of the manufacturer of the device.
     * @param[in] modelNumber
     *                The model number that is assigned by the device vendor.
     * @param[in] serialNumber
     *                The serial number for a particular instance of the device.
     * @param[in] hardwareRevision
     *                The hardware revision for the hardware within the device.
     * @param[in] firmwareRevision
     *                The device's firmware version.
     * @param[in] softwareRevision
     *                The device's software version.
     */
    DeviceInformationService(BLE        &_ble,
                             const char *manufacturersName = nullptr,
                             const char *modelNumber       = nullptr,
                             const char *serialNumber      = nullptr,
                             const char *hardwareRevision  = nullptr,
                             const char *firmwareRevision  = nullptr,
                             const char *softwareRevision  = nullptr) :
        ble(_ble),
        manufacturersNameStringCharacteristic(GattCharacteristic::UUID_MANUFACTURER_NAME_STRING_CHAR,
                                              (uint8_t *)manufacturersName,
                                              (manufacturersName != nullptr) ? strlen(manufacturersName) : 0, /* Min length */
                                              (manufacturersName != nullptr) ? strlen(manufacturersName) : 0, /* Max length */
                                              GattCharacteristic::BLE_GATT_CHAR_PROPERTIES_READ),
        modelNumberStringCharacteristic(GattCharacteristic::UUID_MODEL_NUMBER_STRING_CHAR,
                                        (uint8_t *)modelNumber,
                                        (modelNumber != nullptr) ? strlen(modelNumber) : 0, /* Min length */
                                        (modelNumber != nullptr) ? strlen(modelNumber) : 0, /* Max length */
                                        GattCharacteristic::BLE_GATT_CHAR_PROPERTIES_READ),
        serialNumberStringCharacteristic(GattCharacteristic::UUID_SERIAL_NUMBER_STRING_CHAR,
                                         (uint8_t *)serialNumber,
                                         (serialNumber != nullptr) ? strlen(serialNumber) : 0, /* Min length */
                                         (serialNumber != nullptr) ? strlen(serialNumber) : 0, /* Max length */
                                         GattCharacteristic::BLE_GATT_CHAR_PROPERTIES_READ),
        hardwareRevisionStringCharacteristic(GattCharacteristic::UUID_HARDWARE_REVISION_STRING_CHAR,
                                             (uint8_t *)hardwareRevision,
                                             (hardwareRevision != nullptr) ? strlen(hardwareRevision) : 0, /* Min length */
                                             (hardwareRevision != nullptr) ? strlen(hardwareRevision) : 0, /* Max length */
                                             GattCharacteristic::BLE_GATT_CHAR_PROPERTIES_READ),
        firmwareRevisionStringCharacteristic(GattCharacteristic::UUID_FIRMWARE_REVISION_STRING_CHAR,
                                             (uint8_t *)firmwareRevision,
                                             (firmwareRevision != nullptr) ? strlen(firmwareRevision) : 0, /* Min length */
                                             (firmwareRevision != nullptr) ? strlen(firmwareRevision) : 0, /* Max length */
                                             GattCharacteristic::BLE_GATT_CHAR_PROPERTIES_READ),
        softwareRevisionStringCharacteristic(GattCharacteristic::UUID_SOFTWARE_REVISION_STRING_CHAR,
                                             (uint8_t *)softwareRevision,
                                             (softwareRevision != nullptr) ? strlen(softwareRevision) : 0, /* Min length */
                                             (softwareRevision != nullptr) ? strlen(softwareRevision) : 0, /* Max length */
                                             GattCharacteristic::BLE_GATT_CHAR_PROPERTIES_READ)
    {
        static bool serviceAdded = false; /* We only add the information service once. */
        if (serviceAdded) {
            return;
        }

        GattCharacteristic *charTable[] = {&manufacturersNameStringCharacteristic,
                                           &modelNumberStringCharacteristic,
                                           &serialNumberStringCharacteristic,
                                           &hardwareRevisionStringCharacteristic,
                                           &firmwareRevisionStringCharacteristic,
                                           &softwareRevisionStringCharacteristic};
        GattService         deviceInformationService(GattService::UUID_DEVICE_INFORMATION_SERVICE, charTable,
                                                     sizeof(charTable) / sizeof(charTable[0]));

        ble.gattServer().addService(deviceInformationService);
        serviceAdded = true;
    }

protected:
    /**
     * A reference to the BLE instance object to which the services and
     * characteristics will be added.
     */
    BLE                &ble;
    /**
     * BLE characterising to allow BLE peers access to the manufacturer's name.
     */
    GattCharacteristic  manufacturersNameStringCharacteristic;
    /**
     * BLE characterising to allow BLE peers access to the model number.
     */
    GattCharacteristic  modelNumberStringCharacteristic;
    /**
     * BLE characterising to allow BLE peers access to the serial number.
     */
    GattCharacteristic  serialNumberStringCharacteristic;
    /**
     * BLE characterising to allow BLE peers access to the hardware revision string.
     */
    GattCharacteristic  hardwareRevisionStringCharacteristic;
    /**
     * BLE characterising to allow BLE peers access to the firmware revision string.
     */
    GattCharacteristic  firmwareRevisionStringCharacteristic;
    /**
     * BLE characterising to allow BLE peers access to the software revision string.
     */
    GattCharacteristic  softwareRevisionStringCharacteristic;
};

#endif // BLE_FEATURE_GATT_SERVER

#endif /* #ifndef __BLE_DEVICE_INFORMATION_SERVICE_H__*/
