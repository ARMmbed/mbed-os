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

#ifndef __BLE_HEALTH_THERMOMETER_SERVICE_H__
#define __BLE_HEALTH_THERMOMETER_SERVICE_H__

#include "ble/BLE.h"

/**
* @class HealthThermometerService
* @brief BLE Health Thermometer Service. This service provides the location of the thermometer and the temperature.
* Service:  https://developer.bluetooth.org/gatt/profiles/Pages/ProfileViewer.aspx?u=org.bluetooth.profile.health_thermometer.xml
* Temperature Measurement: https://developer.bluetooth.org/gatt/characteristics/Pages/CharacteristicViewer.aspx?u=org.bluetooth.characteristic.temperature_measurement.xml
* Temperature Type: https://developer.bluetooth.org/gatt/characteristics/Pages/CharacteristicViewer.aspx?u=org.bluetooth.characteristic.temperature_type.xml
*/
class HealthThermometerService {
public:
    /**
    * @brief Location of sensor on the body.
    */
    enum SensorLocation_t {
        LOCATION_ARMPIT = 1,    /*!< Armpit. */
        LOCATION_BODY,          /*!< Body. */
        LOCATION_EAR,           /*!< Ear. */
        LOCATION_FINGER,        /*!< Finger. */
        LOCATION_GI_TRACT,      /*!< GI tract */
        LOCATION_MOUTH,         /*!< Mouth. */
        LOCATION_RECTUM,        /*!< Rectum. */
        LOCATION_TOE,           /*!< Toe. */
        LOCATION_EAR_DRUM,      /*!< Eardrum. */
    };

public:
    /**
     * @brief Add the Health Thermometer Service to an existing BLE object, initialize with temperature and location.
     * @param[in] _ble         Reference to the BLE device.
     * @param[in] initialTemp  Initial value in celsius.
     * @param[in] _location
     */
    HealthThermometerService(BLE &_ble, float initialTemp, uint8_t _location) :
        ble(_ble),
        valueBytes(initialTemp),
        tempMeasurement(GattCharacteristic::UUID_TEMPERATURE_MEASUREMENT_CHAR, (TemperatureValueBytes *)valueBytes.getPointer(), GattCharacteristic::BLE_GATT_CHAR_PROPERTIES_NOTIFY),
        tempLocation(GattCharacteristic::UUID_TEMPERATURE_TYPE_CHAR, &_location) {

        GattCharacteristic *hrmChars[] = {&tempMeasurement, &tempLocation, };
        GattService         hrmService(GattService::UUID_HEALTH_THERMOMETER_SERVICE, hrmChars, sizeof(hrmChars) / sizeof(GattCharacteristic *));

        ble.addService(hrmService);
    }

    /**
    * @brief Update the temperature being broadcast.
    *
    * @param[in] temperature
    *                   Floating point value of the temperature.
    *
    */
    void updateTemperature(float temperature) {
        if (ble.getGapState().connected) {
            valueBytes.updateTemperature(temperature);
            ble.gattServer().write(tempMeasurement.getValueHandle(), valueBytes.getPointer(), sizeof(TemperatureValueBytes));
        }
    }

    /**
     * @brief Update the location.
     * @param loc
     *        New location value.
     */
    void updateLocation(SensorLocation_t loc) {
        ble.gattServer().write(tempLocation.getValueHandle(), reinterpret_cast<uint8_t *>(&loc), sizeof(uint8_t));
    }

private:
    /* Private internal representation for the bytes used to work with the vaulue of the temperature characteristic. */
    struct TemperatureValueBytes {
        static const unsigned OFFSET_OF_FLAGS    = 0;
        static const unsigned OFFSET_OF_VALUE    = OFFSET_OF_FLAGS + sizeof(uint8_t);
        static const unsigned SIZEOF_VALUE_BYTES = sizeof(uint8_t) + sizeof(float);

        static const unsigned TEMPERATURE_UNITS_FLAG_POS = 0;
        static const unsigned TIMESTAMP_FLAG_POS         = 1;
        static const unsigned TEMPERATURE_TYPE_FLAG_POS  = 2;

        static const uint8_t  TEMPERATURE_UNITS_CELSIUS    = 0;
        static const uint8_t  TEMPERATURE_UNITS_FAHRENHEIT = 1;

        TemperatureValueBytes(float initialTemperature) : bytes() {
            /* Assumption: temperature values are expressed in celsius */
            bytes[OFFSET_OF_FLAGS] =  (TEMPERATURE_UNITS_CELSIUS << TEMPERATURE_UNITS_FLAG_POS) |
                                      (false << TIMESTAMP_FLAG_POS) |
                                      (false << TEMPERATURE_TYPE_FLAG_POS);
            updateTemperature(initialTemperature);
        }

        void updateTemperature(float temp) {
            uint32_t temp_ieee11073 = quick_ieee11073_from_float(temp);
            memcpy(&bytes[OFFSET_OF_VALUE], &temp_ieee11073, sizeof(float));
        }

        uint8_t       *getPointer(void) {
            return bytes;
        }

        const uint8_t *getPointer(void) const {
            return bytes;
        }

private:
        /**
         * @brief A very quick conversion between a float temperature and 11073-20601 FLOAT-Type.
         * @param temperature The temperature as a float.
         * @return The temperature in 11073-20601 FLOAT-Type format.
         */
        uint32_t quick_ieee11073_from_float(float temperature) {
            uint8_t  exponent = 0xFE; //Exponent is -2
            uint32_t mantissa = (uint32_t)(temperature * 100);

            return (((uint32_t)exponent) << 24) | mantissa;
        }

private:
        /* First byte: 8-bit flags. Second field is a float holding the temperature value. */
        /* See https://developer.bluetooth.org/gatt/characteristics/Pages/CharacteristicViewer.aspx?u=org.bluetooth.characteristic.temperature_measurement.xml */
        uint8_t bytes[SIZEOF_VALUE_BYTES];
    };

protected:
    BLE                                               &ble;
    TemperatureValueBytes                              valueBytes;
    ReadOnlyGattCharacteristic<TemperatureValueBytes>  tempMeasurement;
    ReadOnlyGattCharacteristic<uint8_t>                tempLocation;
};

#endif /* #ifndef __BLE_HEALTH_THERMOMETER_SERVICE_H__*/
