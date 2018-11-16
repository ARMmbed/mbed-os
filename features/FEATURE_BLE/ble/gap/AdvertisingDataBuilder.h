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

#include "platform/Span.h"
#include "platform/NonCopyable.h"

#include "ble/blecommon.h"
#include "ble/SafeEnum.h"
#include "UUID.h"

/**
 * @addtogroup ble
 * @{
 * @addtogroup gap
 * @{
 */

namespace ble {

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
struct adv_data_type_t : SafeEnum<adv_data_type_t, uint8_t> {
    /** struct scoped enum wrapped by the class */
    enum type {
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
     * Construct a new instance of adv_data_type_t.
     */
    adv_data_type_t(type value) :
        SafeEnum<adv_data_type_t, uint8_t>(value) { }
};


/**
 *  Set of advertising flags.
 *
 *  @note LE_LIMITED_DISCOVERABLE and LE_GENERAL_DISCOVERABLE are mutually
 *  exclusive
 *
 *  @li @c Bluetooth Core Specification 4.0 (Vol. 3), Part C, Section 18.1.
 */
struct adv_data_flags_t {
    enum {
        LE_LIMITED_DISCOVERABLE = 0x01, /**< Discoverable for a limited period of time.*/
        LE_GENERAL_DISCOVERABLE = 0x02, /**< Discoverable at any moment. */
        BREDR_NOT_SUPPORTED     = 0x04, /**< LE only and does not support Bluetooth Enhanced DataRate. */
        SIMULTANEOUS_LE_BREDR_C = 0x08, /**< Not relevant - dual mode only. */
        SIMULTANEOUS_LE_BREDR_H = 0x10  /**< Not relevant - dual mode only. */
    };

    static const uint8_t default_flags = BREDR_NOT_SUPPORTED | LE_GENERAL_DISCOVERABLE;

    /** Create from raw value */
    adv_data_flags_t(uint8_t value = 0) : _value(value) {};

    adv_data_flags_t& setGeneralDiscoverable(bool enable = true) {
        _value &= ~0x03;
        if (enable) {
            _value |= LE_GENERAL_DISCOVERABLE;
        }
        return *this;
    }

    adv_data_flags_t& setLimitedDiscoverable(bool enable = true) {
        _value &= ~0x03;
        if (enable) {
            _value |= LE_LIMITED_DISCOVERABLE;
        }
        return *this;
    }

    adv_data_flags_t& setBredrNotSupported(bool enable = true) {
        _value &= ~BREDR_NOT_SUPPORTED;
        if (enable) {
            _value |= BREDR_NOT_SUPPORTED;
        }
        return *this;
    }

    adv_data_flags_t& setSimultaneousLeBredrC(bool enable = true) {
        _value &= ~SIMULTANEOUS_LE_BREDR_C;
        if (enable) {
            _value |= SIMULTANEOUS_LE_BREDR_C;
        }
        return *this;
    }

    adv_data_flags_t& setSimultaneousLeBredrH(bool enable = true) {
        _value &= ~SIMULTANEOUS_LE_BREDR_H;
        if (enable) {
            _value |= SIMULTANEOUS_LE_BREDR_H;
        }
        return *this;
    }

    bool getGeneralDiscoverable() {
        return _value& LE_GENERAL_DISCOVERABLE;
    }

    bool getlimitedDiscoverable() {
        return _value& LE_LIMITED_DISCOVERABLE;
    }

    bool getBrEdrNotSupported() {
        return _value& BREDR_NOT_SUPPORTED;
    }

    bool getSimultaneousLeBredrC() {
        return _value& SIMULTANEOUS_LE_BREDR_C;
    }

    bool getSimultaneousLeBredrH() {
        return _value& SIMULTANEOUS_LE_BREDR_H;
    }

    void clear() {
        _value = 0;
    }

    uint8_t value() {
        return _value;
    }

private:
    uint8_t _value;
};


/**
 *  Enumeration of values for the adv_data_type_t::APPEARANCE.
 *
 *  These values describe the physical shape or appearance of the device.
 *
 *  @par Source
 *
 *  @li @c Bluetooth Core Specification Supplement, Part A, Section 1.12.
 *  @li @c Bluetooth Core Specification 4.0 (Vol. 3), Part C, Section 12.2.
 *  @li @c https://developer.bluetooth.org/gatt/characteristics/Pages/CharacteristicViewer.aspx?u=org.bluetooth.characteristic.gap.appearance.xml.
 */
struct adv_data_appearance_t : SafeEnum<adv_data_appearance_t, uint16_t> {
    /** struct scoped enum wrapped by the class */
    enum type {
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

    /**
     * Construct a new instance of adv_data_appearance_t.
     */
    adv_data_appearance_t(type value) :
        SafeEnum<adv_data_appearance_t, uint16_t>(value) { }
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
        _payload_length(0) {
    }

    /** Advertising data needs a user provided buffer to store the data.
     *
     * @param buffer Pointer to buffer to be used for storing advertising data.
     * @param buffer_size Size of the buffer.
     * @note Use Gap::getMaxAdvertisingDataLength() to find out how much can be accepted.
     */
    AdvertisingDataBuilder(uint8_t* buffer, size_t buffer_size) :
        _buffer(buffer, buffer_size),
        _payload_length(0) {
    }

    /**
     * Get the subspan of the buffer containing valid data.
     *
     * @return A Span containing the payload.
     */
    mbed::Span<uint8_t> getAdvertisingData() const {
        return _buffer.first(_payload_length);
    }

    /**
     * Adds a new field into the payload.
     *
     * If the supplied advertising data type is already present in the
     * advertising payload, then the value is updated.
     *
     * @param[in] advDataType The type of the field to add.
     * @param[in] fieldData Span of data to add.
     *
     * @retval BLE_ERROR_NONE on success.
     * @retval BLE_ERROR_BUFFER_OVERFLOW if buffer is too small to contain the new data.
     *
     * @note When the specified data type is INCOMPLETE_LIST_16BIT_SERVICE_IDS,
     * COMPLETE_LIST_16BIT_SERVICE_IDS, INCOMPLETE_LIST_32BIT_SERVICE_IDS,
     * COMPLETE_LIST_32BIT_SERVICE_IDS, INCOMPLETE_LIST_128BIT_SERVICE_IDS,
     * COMPLETE_LIST_128BIT_SERVICE_IDS or LIST_128BIT_SOLICITATION_IDS, the
     * supplied value is appended to the values present in the payload.
     */
    ble_error_t addData(
        adv_data_type_t advDataType,
        mbed::Span<const uint8_t> fieldData
    ) {
        uint8_t* field = findField(advDataType);

        if (field) {
            switch(advDataType.value()) {
                /* These types are appended to the existing field */
                case adv_data_type_t::INCOMPLETE_LIST_16BIT_SERVICE_IDS:
                case adv_data_type_t::COMPLETE_LIST_16BIT_SERVICE_IDS:
                case adv_data_type_t::INCOMPLETE_LIST_32BIT_SERVICE_IDS:
                case adv_data_type_t::COMPLETE_LIST_32BIT_SERVICE_IDS:
                case adv_data_type_t::INCOMPLETE_LIST_128BIT_SERVICE_IDS:
                case adv_data_type_t::COMPLETE_LIST_128BIT_SERVICE_IDS:
                case adv_data_type_t::LIST_128BIT_SOLICITATION_IDS:
                    return appendToField(fieldData, field);
                default:
                    /* All other types have their field contents replaced */
                    return replaceField(advDataType, fieldData, field);
            }
        } else {
            /* field doesn't exist, add it */
            return addField(advDataType, fieldData);
        }
    }

    /**
     * Remove existing date of given type
     *
     * @param[in] advDataType The type of the field to remove.
     *
     * @return BLE_ERROR_NONE returned on success, BLE_ERROR_INVALID_PARAM if field doesn't exist
     */
    ble_error_t removeData(
        adv_data_type_t advDataType
    ) {
        uint8_t* field = findField(advDataType);

        if (field) {
            return removeField(field);
        } else {
            return BLE_ERROR_NOT_FOUND;
        }
    }

    /**
     * Clears the advertising data payload.
     *
     * @post getPayloadLen() returns 0.
     */
    void clear() {
        memset(_buffer.data(), 0, _buffer.size());
        _payload_length = 0;
    }

    /**
     * Add device appearance in the advertising payload.
     *
     * @param[in] appearance The appearance to advertise.
     *
     * @retval BLE_ERROR_NONE on success.
     * @retval BLE_ERROR_BUFFER_OVERFLOW if buffer is too small to contain the new data.
     *
     * @note This call is equivalent to calling addData() with
     * adv_data_type_t::APPEARANCE as the field type.
     */
    ble_error_t setAppearance(
        adv_data_appearance_t appearance
    ) {
        uint8_t appearence_byte = appearance.value();
        mbed::Span<const uint8_t> appearance_span((const uint8_t*) &appearence_byte, 2);
        return addData(adv_data_type_t::APPEARANCE, appearance_span);
    }

    /**
     * Add BLE flags in the advertising payload.
     *
     * @param[in] flags Bitfield describing the capability of the device. See
     * allowed flags in Flags_t.
     *
     * @retval BLE_ERROR_NONE on success.
     * @retval BLE_ERROR_BUFFER_OVERFLOW if buffer is too small to contain the new data.
     *
     * @note This call is equivalent to calling addData() with
     * adv_data_type_t::FLAGS as the field type.
     */
    ble_error_t setFlags(
        adv_data_flags_t flags = adv_data_flags_t::default_flags
    ) {
        uint8_t flags_byte = flags.value();
        mbed::Span<const uint8_t> flags_span((const uint8_t*) &flags_byte, 1);
        return addData(adv_data_type_t::FLAGS, flags_span);
    }

    /**
     * Add the advertising TX in the advertising payload.
     *
     * @param[in] txPower Transmission power level in dB.
     *
     * @retval BLE_ERROR_NONE on success.
     * @retval BLE_ERROR_BUFFER_OVERFLOW if buffer is too small to contain the new data.
     *
     * @note This call is equivalent to calling addData() with
     * adv_data_type_t::TX_POWER_LEVEL as the field type.
     */
    ble_error_t setTxPowerAdvertised(
        advertising_power_t txPower
    ) {
        mbed::Span<const uint8_t> power_span((const uint8_t*) &txPower, 1);
        return addData(adv_data_type_t::TX_POWER_LEVEL, power_span);
    }

    /**
     * Add device name to the advertising payload.
     *
     * @param[in] name Null terminated string containing the name.
     * @param[in] complete Complete local name if true, otherwise
     *
     * @retval BLE_ERROR_NONE on success.
     * @retval BLE_ERROR_BUFFER_OVERFLOW if buffer is too small to contain the new data.
     */
    ble_error_t setName(
        const char* name,
        bool complete = true
    ) {
        mbed::Span<const uint8_t> power_span((const uint8_t*)name, strlen(name));

        if (complete) {
            return addData(adv_data_type_t::COMPLETE_LOCAL_NAME, power_span);
        } else {
            return addData(adv_data_type_t::SHORTENED_LOCAL_NAME, power_span);
        }
    }

    /**
     * Add manufacturer specific data to the advertising payload.
     *
     * @param[in] data New data to be added.
     *
     * @retval BLE_ERROR_NONE on success.
     * @retval BLE_ERROR_BUFFER_OVERFLOW if buffer is too small to contain the new data.
     */
    ble_error_t setManufacturerSpecificData(
        mbed::Span<const uint8_t> data
    ) {
        return addData(adv_data_type_t::MANUFACTURER_SPECIFIC_DATA, data);
    }

    /**
     * Add manufacturer specific data to the advertising payload.
     *
     * @param[in] data New data to be added.
     * @param[in] complete True if this is a complete list.
     *
     * @retval BLE_ERROR_NONE on success.
     * @retval BLE_ERROR_BUFFER_OVERFLOW if buffer is too small to contain the new data.
     */
    ble_error_t setLocalServiceList(
        mbed::Span<const UUID> data,
        bool complete = true
    ) {
        ble_error_t status = BLE_ERROR_NONE;

        /* first count all the bytes we need to store all the UUIDs */
        size_t size_incomplete_16 = 0;
        size_t size_complete_16 = 0;
        size_t size_incomplete_128 = 0;
        size_t size_complete_128 = 0;
        for (size_t i = 0; i < data.size(); ++i) {
            if (data[i].shortOrLong() == UUID::UUID_TYPE_SHORT) {
                if (complete) {
                    size_incomplete_16++;
                } else {
                    size_complete_16++;
                }
            } else {
                if (complete) {
                    size_incomplete_128++;
                } else {
                    size_complete_128++;
                }
            }
        }

        /* calculate total size including headers for types */
        size_t total_size = size_incomplete_16 + (!!size_incomplete_16) * 2 +
                            size_complete_16 + (!!size_complete_16) * 2 +
                            size_incomplete_128 + (!!size_incomplete_128) * 2 +
                            size_complete_128 + (!!size_complete_128) * 2;

        /* count all the bytes of existing data */
        size_t old_size = 0;
        mbed::Span<const uint8_t> existing_data;
        if (getData(existing_data, adv_data_type_t::INCOMPLETE_LIST_16BIT_SERVICE_IDS) == BLE_ERROR_NONE) {
            old_size += existing_data.size() + 2;
        }
        if (getData(existing_data, adv_data_type_t::COMPLETE_LIST_16BIT_SERVICE_IDS) == BLE_ERROR_NONE) {
            old_size += existing_data.size() + 2;
        }
        if (getData(existing_data, adv_data_type_t::INCOMPLETE_LIST_128BIT_SERVICE_IDS) == BLE_ERROR_NONE) {
            old_size += existing_data.size() + 2;
        }
        if (getData(existing_data, adv_data_type_t::COMPLETE_LIST_128BIT_SERVICE_IDS) == BLE_ERROR_NONE) {
            old_size += existing_data.size() + 2;
        }

        /* if we can't fit the new data do not proceed */
        if (total_size - old_size > data.size() - _payload_length) {
            return BLE_ERROR_BUFFER_OVERFLOW;
        }

        /* otherwise wipe old data */
        removeData(adv_data_type_t::INCOMPLETE_LIST_16BIT_SERVICE_IDS);
        removeData(adv_data_type_t::COMPLETE_LIST_16BIT_SERVICE_IDS);
        removeData(adv_data_type_t::INCOMPLETE_LIST_128BIT_SERVICE_IDS);
        removeData(adv_data_type_t::COMPLETE_LIST_128BIT_SERVICE_IDS);

        /* and insert individual UUIDs into appropriate fields */
        for (size_t i = 0; i < data.size(); ++i) {
            adv_data_type_t field_type(adv_data_type_t::FLAGS);
            if (data[i].shortOrLong() == UUID::UUID_TYPE_SHORT) {
                if (complete) {
                    field_type = adv_data_type_t::INCOMPLETE_LIST_16BIT_SERVICE_IDS;
                } else {
                    field_type = adv_data_type_t::COMPLETE_LIST_16BIT_SERVICE_IDS;
                }
            } else {
                if (complete) {
                    field_type = adv_data_type_t::INCOMPLETE_LIST_128BIT_SERVICE_IDS;
                } else {
                    field_type = adv_data_type_t::COMPLETE_LIST_128BIT_SERVICE_IDS;
                }
            }

            mbed::Span<const uint8_t> span(data[i].getBaseUUID(), data[i].getLen());

            uint8_t *field = findField(field_type);

            if (field) {
                status = appendToField(span, field);
                if (status != BLE_ERROR_NONE) {
                    /* we already checked for size so this must not happen */
                    return BLE_ERROR_INTERNAL_STACK_FAILURE;
                }
            } else {
                status = addField(field_type, span);
                if (status != BLE_ERROR_NONE) {
                    /* we already checked for size so this must not happen */
                    return BLE_ERROR_INTERNAL_STACK_FAILURE;
                }
            }
        }

        return status;
    }

    /**
     * Return a span of data containing the the type of data requested.
     *
     * @param[out] data Span used to return the requested data.
     * @param[in] advDataType Data type to return.
     *
     * @return BLE_ERROR_NONE if data was found and BLE_ERROR_NOT_FOUND if not.
     */
    ble_error_t getData(
        mbed::Span<const uint8_t> &data,
        adv_data_type_t advDataType
    ) {
        uint8_t *field = findField(advDataType);
        if (field) {
            uint8_t data_length = field[0] - 1 /* skip type */;
            data = mbed::make_Span((const uint8_t*)(field + 2 /* skip type and length */), data_length);
            return BLE_ERROR_NONE;
        } else {
            return BLE_ERROR_NOT_FOUND;
        }
    }

private:
    /**
    * Search advertisement data for a specific field.
    *
    * @param[in] type The type of the field to find.
    *
    * @return A pointer to the first element in the field if found. The first
    * element being the length of the field followed by the value of the field.
    * NULL if the field is not present in the payload.
    */
    uint8_t* findField(adv_data_type_t type)
    {
        /* Scan through advertisement data */
        for (uint8_t idx = 0; idx < _payload_length; ) {
            uint8_t fieldType = _buffer[idx + 1];

            if (fieldType == type) {
                return _buffer.data() + idx;
            }

            /* Advance to next field */
            idx += _buffer[idx] + 1;
        }

        return NULL;
    }

    /**
     * Append advertising data based on the specified type.
     *
     * @param[in] advDataType Type of the new data.
     * @param[in] fieldData Span of data to add.
     *
     * @retval BLE_ERROR_NONE on success.
     * @retval BLE_ERROR_BUFFER_OVERFLOW if buffer is too small to contain the new data.
     */
    ble_error_t addField(
        adv_data_type_t advDataType,
        mbed::Span<const uint8_t> fieldData
    ) {

        /* Make sure we don't exceed the buffer size */
        if (_payload_length + fieldData.size() + 2 > _buffer.size()) {
            return BLE_ERROR_BUFFER_OVERFLOW;
        }

        /* Field length. */
        _buffer[_payload_length] = fieldData.size() + 1;
        ++_payload_length;

        /* Field ID. */
        _buffer[_payload_length] = advDataType.value();
        ++_payload_length;

        /* Payload. */
        memcpy(&_buffer[_payload_length], fieldData.data(), fieldData.size());
        _payload_length += fieldData.size();

        return BLE_ERROR_NONE;
    }

    /**
     * Append data to a field in the advertising payload.
     *
     * @param[in] fieldData Span of data to add.
     * @param[in] field Pointer to the field in the advertising buffer.
     *
     * @return BLE_ERROR_NONE on success.
     */
    ble_error_t appendToField(
        mbed::Span<const uint8_t> fieldData,
        uint8_t* field
    ) {
        /* Check if data fits */
        if ((_payload_length + fieldData.size()) <= _buffer.size()) {
            /*
             * Make room for new field by moving the remainder of the
             * advertisement payload "to the right" starting after the
             * TYPE field.
             */
            uint8_t* end = _buffer.data() + _payload_length;

            while (&field[1] < end) {
                end[fieldData.size()] = *end;
                end--;
            }

            /* Insert new data */
            for (uint8_t idx = 0; idx < fieldData.size(); idx++) {
                field[2 + idx] = fieldData[idx];
            }

            /* Increment lengths */
            field[0] += fieldData.size();
            _payload_length += fieldData.size();

            return BLE_ERROR_NONE;
        } else {
            return BLE_ERROR_BUFFER_OVERFLOW;
        }
    }

    /**
     * Update in place the value of a field in the advertising payload.
     *
     * @param[in] advDataType Type of the new data.
     * @param[in] fieldData Span of data to add.
     * @param[in] field Pointer to the field of type @p advDataType in the
     * advertising buffer.
     *
     * @retval BLE_ERROR_NONE on success.
     * @retval BLE_ERROR_BUFFER_OVERFLOW if buffer is too small to contain the new data.
     */
    ble_error_t replaceField(
        adv_data_type_t advDataType,
        mbed::Span<const uint8_t> fieldData,
        uint8_t* field
    ) {
        ble_error_t result = BLE_ERROR_BUFFER_OVERFLOW;
        uint8_t old_data_length = field[0] - 1;

        /* New data has same length, do in-order replacement */
        if (fieldData.size() == old_data_length) {
            for (uint8_t idx = 0; idx < old_data_length; idx++) {
                field[2 + idx] = fieldData[idx];
            }

            result = BLE_ERROR_NONE;
        } else {
            /* Check if data fits */
            if ((_payload_length - old_data_length + fieldData.size()) <= _buffer.size()) {
                removeField(field);

                /* Add new field */
                result = addField(advDataType, fieldData);
            }
        }

        return result;
    }

    /**
     * Remove the field.
     *
     * @param[in] field Pointer to the field in the advertising buffer.
     *
     * @return BLE_ERROR_NONE on success.
     */
    ble_error_t removeField(
        uint8_t* field
    ) {
        uint8_t old_data_length = field[0] - 1;

        /* Remove old field */
        while ((field + old_data_length + 2) < _buffer.data() + _payload_length) {
            *field = field[old_data_length + 2];
            field++;
        }

        /* Reduce length */
        _payload_length -= old_data_length + 2;

        return BLE_ERROR_NONE;
    }

protected:
    /** The memory backing the the data provided by the user. */
    mbed::Span<uint8_t> _buffer;

    /** Length of the data added to the advertising buffer. */
    uint8_t _payload_length;
};

} // namespace ble

/**
 * @}
 * @}
 */


#endif /* ifndef MBED_GAP_ADVERTISING_DATA_H__ */
