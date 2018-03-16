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

#ifndef MBED_GATT_SERVICE_H__
#define MBED_GATT_SERVICE_H__

#include "UUID.h"
#include "GattCharacteristic.h"

/**
 * @addtogroup ble
 * @{
 * @addtogroup gatt
 * @{
 * @addtogroup server
 * @{
 */

/**
 * Representation of a GattServer service.
 *
 * A service is a cohesive collection of characteristics. It is identified by a
 * UUID and starts at a specific handle of its GattServer.
 */
class GattService {
public:
    enum {
        /**
         * UUID of the Alert Notification service.
         */
        UUID_ALERT_NOTIFICATION_SERVICE = 0x1811,

        /**
         * UUID of the Battery service.
         */
        UUID_BATTERY_SERVICE = 0x180F,

        /**
         * UUID of the Blood Pressure service.
         */
        UUID_BLOOD_PRESSURE_SERVICE = 0x1810,

        /**
         * UUID of the Current Time service.
         */
        UUID_CURRENT_TIME_SERVICE = 0x1805,

        /**
         * UUID of the Cycling Speed and Cadence (CSC) service.
         */
        UUID_CYCLING_SPEED_AND_CADENCE = 0x1816,

        /**
         * UUID of the Device Information Service (DIS).
         */
        UUID_DEVICE_INFORMATION_SERVICE = 0x180A,

        /**
         * UUID of the environmental service.
         */
        UUID_ENVIRONMENTAL_SERVICE = 0x181A,

        /**
         * UUID of the Glucose service.
         */
        UUID_GLUCOSE_SERVICE = 0x1808,

        /**
         * UUID of the health thermometer.
         */
        UUID_HEALTH_THERMOMETER_SERVICE = 0x1809,

        /**
         * UUID of the Heart Rate service.
         */
        UUID_HEART_RATE_SERVICE = 0x180D,

        /**
         * UUID of the Human Interface Device (HID) service.
         */
        UUID_HUMAN_INTERFACE_DEVICE_SERVICE = 0x1812,

        /**
         * UUID of the Immediate Alert service.
         */
        UUID_IMMEDIATE_ALERT_SERVICE = 0x1802,

        /**
         * UUID of the Link Loss service.
         */
        UUID_LINK_LOSS_SERVICE = 0x1803,

        /**
         * UUID of the Next DST change service.
         */
        UUID_NEXT_DST_CHANGE_SERVICE = 0x1807,

        /**
         * UUID of the Phone Alert Status service.
         */
        UUID_PHONE_ALERT_STATUS_SERVICE = 0x180E,

        /**
         * UUID of the Reference Time Update service.
         */
        UUID_REFERENCE_TIME_UPDATE_SERVICE = 0x1806,

        /**
         * UUID of the Running Speed and Cadence (RSC) service.
         */
        UUID_RUNNING_SPEED_AND_CADENCE = 0x1814,

        /**
         * UUID of the Scan Parameter service.
         */
        UUID_SCAN_PARAMETERS_SERVICE = 0x1813,

        /**
         * UUID of the TX power service.
         */
        UUID_TX_POWER_SERVICE = 0x1804
    };

public:
    /**
     * Construct a GattService.
     *
     * @param[in] uuid The UUID assigned to this service.
     * @param[in] characteristics A pointer to the array of characteristics that
     * belongs to the service.
     * @param[in] numCharacteristics The number of characteristics.
     *
     * @attention The characteristics of the service must remain valid while the
     * GattServer uses the service.
     */
    GattService(
        const UUID &uuid,
        GattCharacteristic *characteristics[],
        unsigned numCharacteristics
    ) :
        _primaryServiceID(uuid),
        _characteristicCount(numCharacteristics),
        _characteristics(characteristics),
        _handle(0) {
    }

    /**
     * Get this service's UUID.
     *
     * @return A reference to the service's UUID.
     */
    const UUID &getUUID(void) const
    {
        return _primaryServiceID;
    }

    /**
     * Get the handle of the service declaration attribute in the ATT table.
     *
     * @return The service's handle.
     */
    uint16_t getHandle(void) const
    {
        return _handle;
    }

    /**
     * Get the total number of characteristics within this service.
     *
     * @return The total number of characteristics within this service.
     */
    uint8_t getCharacteristicCount(void) const
    {
        return _characteristicCount;
    }

    /**
     * Set the handle of the service declaration attribute in the ATT table.
     *
     * @attention Application code must not use this API.
     *
     * @param[in] handle The service's handle.
     */
    void setHandle(uint16_t handle)
    {
        _handle = handle;
    }

    /**
     * Get this service's characteristic at a specific index.
     *
     * @param[in] index The index of the characteristic.
     *
     * @return A pointer to the characteristic at index @p index.
     */
    GattCharacteristic *getCharacteristic(uint8_t index)
    {
        if (index >= _characteristicCount) {
            return NULL;
        }

        return _characteristics[index];
    }

private:
    /**
     * This service's UUID.
     */
    UUID _primaryServiceID;

    /**
     * Total number of characteristics within this service.
     */
    uint8_t _characteristicCount;

    /**
     * An array with pointers to the characteristics added to this service.
     */
    GattCharacteristic **_characteristics;

    /**
     * Handle of the service declaration attribute in the ATT table.
     *
     * @note The underlying BLE stack generally assigns this handle when the
     * service is added to the ATT table.
     */
    uint16_t _handle;
};

/**
 * @}
 * @}
 * @}
 */

#endif /* ifndef MBED_GATT_SERVICE_H__ */
