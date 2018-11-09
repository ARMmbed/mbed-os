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

#ifndef MBED_GAP_ADVERTISING_DATA_H__
#define MBED_GAP_ADVERTISING_DATA_H__

#include <stdint.h>
#include <string.h>
#include <stdlib.h>

#include "blecommon.h"
#include "platform/Span.h"
#include "NonCopyable.h"

/**
 * @addtogroup ble
 * @{
 * @addtogroup gap
 * @{
 */

struct GapAdvertisingConstant {
    /*!
     * List of standard Advertising Data types.
     *
     * These AD types are used to describe the capabilities of the peripheral
     * and are inserted inside the advertising or scan response payloads.
     *
     * @par Source
     *
     * @li @c Bluetooth Core Specification 4.0 (Vol. 3), Part C, Section 11, 18.
     * @li @c https://www.bluetooth.org/en-us/specification/assigned-numbers/generic-access-profile.
     */
    enum DataType {
        /**
         * Flags, refer to AdvertisingData::Flags_t.
         */
        FLAGS = 0x01,

        /**
         * Incomplete list of 16-bit Service IDs.
         */
        INCOMPLETE_LIST_16BIT_SERVICE_IDS = 0x02,

        /**
         * Complete list of 16-bit Service IDs.
         */
        COMPLETE_LIST_16BIT_SERVICE_IDS = 0x03,

        /**
         * Incomplete list of 32-bit Service IDs (not relevant for Bluetooth 4.0).
         */
        INCOMPLETE_LIST_32BIT_SERVICE_IDS = 0x04,

        /**
         * Complete list of 32-bit Service IDs (not relevant for Bluetooth 4.0).
         */
        COMPLETE_LIST_32BIT_SERVICE_IDS = 0x05,

        /**
         * Incomplete list of 128-bit Service IDs.
         */
        INCOMPLETE_LIST_128BIT_SERVICE_IDS = 0x06,

        /**
         * Complete list of 128-bit Service IDs.
         */
        COMPLETE_LIST_128BIT_SERVICE_IDS = 0x07,

        /**
         * Shortened Local Name.
         */
        SHORTENED_LOCAL_NAME = 0x08,

        /**
         * Complete Local Name.
         */
        COMPLETE_LOCAL_NAME = 0x09,

        /**
         * TX Power Level (in dBm).
         */
        TX_POWER_LEVEL = 0x0A,

        /**
         * Device ID.
         */
        DEVICE_ID = 0x10,

        /**
         * Slave Connection Interval Range.
         */
        SLAVE_CONNECTION_INTERVAL_RANGE = 0x12,

        /**
         * List of 128-bit service UUIDs the device is looking for.
         */
        LIST_128BIT_SOLICITATION_IDS = 0x15,

        /**
         * Service Data.
         */
        SERVICE_DATA = 0x16,

        /**
         * Appearance, refer to AdvertisingData::Appearance_t.
         */
        APPEARANCE = 0x19,

        /**
         * Advertising Interval.
         */
        ADVERTISING_INTERVAL = 0x1A,

        /**
         * Manufacturer Specific Data.
         */
        MANUFACTURER_SPECIFIC_DATA = 0xFF
    };

    /**
     *  Enumeration of allowed flags for DataType_t::FLAGS.
     *
     *  @note DataType_t::FLAGS may contain several flags that the bitwise
     * and operator (ex.LE_GENERAL_DISCOVERABLE & BREDR_NOT_SUPPORTED) assembled.
     *
     *  @par Source
     *
     *  @li @c Bluetooth Core Specification 4.0 (Vol. 3), Part C, Section 18.1.
     */
    enum Flags {
        /**
         * Peripheral device is discoverable for a limited period of time.
         */
        LE_LIMITED_DISCOVERABLE = 0x01,

        /**
         * Peripheral device is discoverable at any moment.
         */
        LE_GENERAL_DISCOVERABLE = 0x02,

        /**
         * Peripheral device is LE only and does not support Bluetooth Enhanced
         * DataRate.
         */
        BREDR_NOT_SUPPORTED = 0x04,

        /**
         * Not relevant - dual mode only.
         */
        SIMULTANEOUS_LE_BREDR_C = 0x08,

        /**
         * Not relevant - dual mode only.
         */
        SIMULTANEOUS_LE_BREDR_H = 0x10

    };

    /**
     *  Enumeration of values for the DataType_t::APPEARANCE.
     *
     *  These values describe the physical shape or appearance of the device.
     *
     *  @par Source
     *
     *  @li @c Bluetooth Core Specification Supplement, Part A, Section 1.12.
     *  @li @c Bluetooth Core Specification 4.0 (Vol. 3), Part C, Section 12.2.
     *  @li @c https://developer.bluetooth.org/gatt/characteristics/Pages/CharacteristicViewer.aspx?u=org.bluetooth.characteristic.gap.appearance.xml.
     */
    enum Appearance {
        /**
         * Unknown or unspecified appearance type.
         */
        UNKNOWN = 0,

        /**
         * Generic Phone.
         */
        GENERIC_PHONE = 64,

        /**
         * Generic Computer.
         */
        GENERIC_COMPUTER = 128,

        /**
         * Generic Watch.
         */
        GENERIC_WATCH = 192,

        /**
         * Sports Watch.
         */
        WATCH_SPORTS_WATCH = 193,

        /**
         * Generic Clock.
         */
        GENERIC_CLOCK = 256,

        /**
         * Generic Display.
         */
        GENERIC_DISPLAY = 320,

        /**
         * Generic Remote Control.
         */
        GENERIC_REMOTE_CONTROL = 384,

        /**
         * Generic Eye Glasses.
         */
        GENERIC_EYE_GLASSES = 448,

        /**
         * Generic Tag.
         */
        GENERIC_TAG = 512,

        /**
         * Generic Keyring.
         */
        GENERIC_KEYRING = 576,

        /**
         * Generic Media Player.
         */
        GENERIC_MEDIA_PLAYER = 640,

        /**
         * Generic Bar Code Scanner.
         */
        GENERIC_BARCODE_SCANNER = 704,

        /**
         * Generic Thermometer.
         */
        GENERIC_THERMOMETER = 768,

        /**
         * Ear Thermometer.
         */
        THERMOMETER_EAR = 769,

        /**
         * Generic Heart Rate Sensor.
         */
        GENERIC_HEART_RATE_SENSOR = 832,

        /**
         * Belt Heart Rate Sensor.
         */
        HEART_RATE_SENSOR_HEART_RATE_BELT = 833,

        /**
         * Generic Blood Pressure.
         */
        GENERIC_BLOOD_PRESSURE = 896,

        /**
         * Arm Blood Pressure.
         */
        BLOOD_PRESSURE_ARM = 897,

        /**
         * Wrist Blood Pressure.
         */
        BLOOD_PRESSURE_WRIST = 898,

        /**
         * Human Interface Device (HID).
         */
        HUMAN_INTERFACE_DEVICE_HID = 960,

        /**
         * Keyboard.
         */
        KEYBOARD = 961,

        /**
         * Mouse.
         */
        MOUSE = 962,

        /**
         * Joystick.
         */
        JOYSTICK = 963,

        /**
         * Gamepad.
         */
        GAMEPAD = 964,

        /**
         * Digitizer Tablet.
         */
        DIGITIZER_TABLET = 965,

        /**
         * Card Reader.
         */
        CARD_READER = 966,

        /**
         * Digital Pen.
         */
        DIGITAL_PEN = 967,

        /**
         * Bar Code Scanner.
         */
        BARCODE_SCANNER = 968,

        /**
         * Generic Glucose Meter.
         */
        GENERIC_GLUCOSE_METER = 1024,

        /**
         * Generic Running/Walking Sensor.
         */
        GENERIC_RUNNING_WALKING_SENSOR = 1088,

        /**
         * In Shoe Running/Walking Sensor.
         */
        RUNNING_WALKING_SENSOR_IN_SHOE = 1089,

        /**
         * On Shoe Running/Walking Sensor.
         */
        RUNNING_WALKING_SENSOR_ON_SHOE = 1090,

        /**
         * On Hip Running/Walking Sensor.
         */
        RUNNING_WALKING_SENSOR_ON_HIP = 1091,

        /**
         * Generic Cycling.
         */
        GENERIC_CYCLING = 1152,

        /**
         * Cycling Computer.
         */
        CYCLING_CYCLING_COMPUTER = 1153,

        /**
         * Cycling Speed Sensor.
         */
        CYCLING_SPEED_SENSOR = 1154,

        /**
         * Cycling Cadence Sensor.
         */
        CYCLING_CADENCE_SENSOR = 1155,

        /**
         * Cycling Power Sensor.
         */
        CYCLING_POWER_SENSOR = 1156,

        /**
         * Cycling Speed and Cadence Sensor.
         */
        CYCLING_SPEED_AND_CADENCE_SENSOR = 1157,

        /**
         * Generic Pulse Oximeter.
         */
        PULSE_OXIMETER_GENERIC = 3136,

        /**
         * Fingertip Pulse Oximeter.
         */
        PULSE_OXIMETER_FINGERTIP = 3137,

        /**
         * Wrist Worn Pulse Oximeter.
         */
        PULSE_OXIMETER_WRIST_WORN = 3138,

        /**
         * Generic Weight Scale.
         */
        GENERIC_WEIGHT_SCALE = 3200,

        /**
         * Generic Outdoor.
         */
        OUTDOOR_GENERIC = 5184,

        /**
         * Outdoor Location Display Device.
         */
        OUTDOOR_LOCATION_DISPLAY_DEVICE = 5185,

        /**
         * Outdoor Location and Navigation Display Device.
         */
        OUTDOOR_LOCATION_AND_NAVIGATION_DISPLAY_DEVICE = 5186,

        /**
         * Outdoor Location Pod.
         */
        OUTDOOR_LOCATION_POD = 5187,

        /**
         * Outdoor Location and Navigation Pod.
         */
        OUTDOOR_LOCATION_AND_NAVIGATION_POD = 5188

    };
};


class AdvertisingDataBuilder
{
public:
    /** Advertising data needs a user provided buffer to store the data.
     *
     * @param buffer Buffer used to store the data.
     * @note Use Gap::getMaxAdvertisingDataLength() to find out how much can be accepted.
     */
    AdvertisingDataBuilder(mbed::Span<uint8_t> buffer) :
        _buffer(buffer),
        _payloadLen(0) {
    }

    /** Advertising data needs a user provided buffer to store the data.
     *
     * @param buffer Pointer to buffer to be used for storing advertising data.
     * @param buffer_size Size of the buffer.
     * @note Use Gap::getMaxAdvertisingDataLength() to find out how much can be accepted.
     */
    AdvertisingDataBuilder(uint8_t* buffer, size_t buffer_size) :
        _buffer(buffer, buffer_size),
        _payloadLen(0) {
    }

    /**
     * Get the subspan of the buffer containing valid data.
     *
     * @return A Span containing the payload.
     */
    mbed::Span<uint8_t> getAdvertisingData() const {
        return _buffer.first(_payloadLen);
    }

    /**
     * Adds a new field into the payload.
     *
     * If the supplied advertising data type is already present in the
     * advertising payload, then the value is updated.
     *
     * @param[in] advDataType The type of the field to add.
     * @param[in] payload Pointer to the value of the field to add.
     * @param[in] len Size in bytes of the value to add.
     *
     * @return BLE_ERROR_NONE on success.
     * @return BLE_ERROR_BUFFER_OVERFLOW if the new value causes the advertising
     * buffer to overflow.
     *
     * @note When the specified data type is INCOMPLETE_LIST_16BIT_SERVICE_IDS,
     * COMPLETE_LIST_16BIT_SERVICE_IDS, INCOMPLETE_LIST_32BIT_SERVICE_IDS,
     * COMPLETE_LIST_32BIT_SERVICE_IDS, INCOMPLETE_LIST_128BIT_SERVICE_IDS,
     * COMPLETE_LIST_128BIT_SERVICE_IDS or LIST_128BIT_SOLICITATION_IDS, the
     * supplied value is appended to the values present in the payload.
     */
    ble_error_t addData(GapAdvertisingConstant::DataType advDataType, const uint8_t *payload, uint8_t len)
    {
        return appendField(advDataType, payload, len);
    }

    /**
     * Clears the advertising data payload.
     *
     * @post getPayloadLen() returns 0.
     */
    void clear(void)
    {
        memset(_buffer.data(), 0, _buffer.size());
        _payloadLen = 0;
    }

private:
    /**
     * Append advertising data based on the specified type.
     *
     * @param[in] advDataType Type of the new data.
     * @param[in] payload Pointer to the data to be appended to the advertising
     * payload.
     * @param[in] len Length of the data pointed to by @p payload.
     *
     * @return BLE_ERROR_NONE on success.
     * @return BLE_ERROR_BUFFER_OVERFLOW if the specified data would cause the
     * advertising buffer to overflow.
     */
    ble_error_t appendField(GapAdvertisingConstant::DataType advDataType, const uint8_t *payload, uint8_t len)
    {
        /* Make sure we don't exceed the buffer size */
        if (_payloadLen + len + 2 > _buffer.size()) {
            return BLE_ERROR_BUFFER_OVERFLOW;
        }

        /* Field length. */
        memset(_buffer.data() + _payloadLen, len + 1, 1);
        _payloadLen++;

        /* Field ID. */
        memset(_buffer.data() + _payloadLen, (uint8_t)advDataType, 1);
        _payloadLen++;

        /* Payload. */
        memcpy(_buffer.data() + _payloadLen, payload, len);
        _payloadLen += len;

        return BLE_ERROR_NONE;
    }

protected:
    /** The memory backing the the data provided by the user. */
    mbed::Span<uint8_t> _buffer;

    /** Length of the data added to the advertising buffer. */
    uint8_t _payloadLen;
};

/**
 * @}
 * @}
 */


#endif /* ifndef MBED_GAP_ADVERTISING_DATA_H__ */
