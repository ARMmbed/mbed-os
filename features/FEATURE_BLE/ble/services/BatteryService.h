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

#ifndef MBED_BLE_BATTERY_SERVICE_H__
#define MBED_BLE_BATTERY_SERVICE_H__

#include "platform/mbed_assert.h"
#include "ble/BLE.h"

/**
 * BLE Battery service.
 *
 * @par purpose
 *
 * The battery service exposes the charge level of the battery of the device.
 * This information is exposed as a percentage from 0% to 100%; a value of 0%
 * represents a fully discharged battery, and a value of 100% represents a
 * fully charged battery.
 *
 * Clients can read the current charge level and subscribe to server initiated
 * updates of the charge level. The server delivers these updates to the subscribed
 * client in a notification packet.
 *
 * The subscription mechanism is useful to save power; it avoids unecessary data
 * traffic between the client and the server, which may be induced by polling the
 * battery level characteristic value.
 *
 * @par usage
 *
 * When this class is instantiated, it adds a battery service in the GattServer.
 *
 * The application code can use the function updateBatteryLevel() to update the
 * charge level that the service exposes and to notify the subscribed client that the
 * value changed.
 *
 * @note You can find specification of the battery service here:
 * https://www.bluetooth.com/specifications/gatt
 *
 * @attention Multiple instances of this battery service are not supported.
 */
class BatteryService {
public:
    /**
     * Instantiate a battery service.
     *
     * The construction of a BatteryService adds a GATT battery service in @p
     * _ble GattServer and sets the initial charge level of the battery to @p
     * level.
     *
     * @param[in] _ble BLE device which will host the battery service.
     * @param[in] level Initial charge level of the battery. It is a percentage
     * where 0% means that the battery is fully discharged and 100% means that
     * the battery is fully charged.
     */
    BatteryService(BLE &_ble, uint8_t level = 100) :
        ble(_ble),
        batteryLevel(level),
        batteryLevelCharacteristic(
            GattCharacteristic::UUID_BATTERY_LEVEL_CHAR,
            &batteryLevel,
            GattCharacteristic::BLE_GATT_CHAR_PROPERTIES_NOTIFY
        )
    {
        MBED_ASSERT(level <= 100);
        GattCharacteristic *charTable[] = { &batteryLevelCharacteristic };
        GattService batteryService(
            GattService::UUID_BATTERY_SERVICE,
            charTable,
            sizeof(charTable) / sizeof(GattCharacteristic *)
        );

        ble.addService(batteryService);
    }

    /**
     * Update the battery charge level that the service exposes.
     *
     * The server sends a notification of the new value to clients that have
     * subscribed to the battery level characteristic updates, and clients
     * reading the charge level after the update obtain the updated value.
     *
     * @param newLevel Charge level of the battery. It is a percentage of the
     * remaining charge between 0% and 100%.
     *
     * @attention This function must be called in the execution context of the
     * BLE stack.
     */
    void updateBatteryLevel(uint8_t newLevel)
    {
        MBED_ASSERT(newLevel <= 100);
        batteryLevel = newLevel;
        ble.gattServer().write(
            batteryLevelCharacteristic.getValueHandle(),
            &batteryLevel,
            1
        );
    }

protected:
    /**
     * Reference to the underlying BLE instance that this object is attached to.
     *
     * The services and characteristics are registered in the GattServer of
     * this BLE instance.
     */
    BLE &ble;

    /**
     * The current battery level represented as an integer from 0% to 100%.
     */
    uint8_t batteryLevel;

    /**
     * The GATT characteristic, which exposes the charge level.
     */
    ReadOnlyGattCharacteristic<uint8_t> batteryLevelCharacteristic;
};

#endif /* #ifndef MBED_BLE_BATTERY_SERVICE_H__*/
