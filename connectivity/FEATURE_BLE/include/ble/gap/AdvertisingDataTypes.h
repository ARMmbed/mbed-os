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

#ifndef BLE_GAP_ADVERTISINGDATATYPES_H
#define BLE_GAP_ADVERTISINGDATATYPES_H

#include "ble/common/SafeEnum.h"

namespace ble {

/**
 * @addtogroup ble
 * @{
 * @addtogroup gap
 * @{
 */

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
        LIST_16BIT_SOLICITATION_IDS = 0x14,

        /**
         * List of 128-bit service UUIDs the device is looking for.
         */
        LIST_128BIT_SOLICITATION_IDS = 0x15,

        /**
         * Service Data.
         */
        SERVICE_DATA = 0x16,

        /**
         * Service Data.
         */
        SERVICE_DATA_16BIT_ID = 0x16,

        /**
         * Service Data.
         */
        SERVICE_DATA_128BIT_ID = 0x21,

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
    adv_data_type_t(type value) : SafeEnum(value)
    {
    }
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
        BREDR_NOT_SUPPORTED = 0x04, /**< LE only and does not support Bluetooth Enhanced DataRate. */
        SIMULTANEOUS_LE_BREDR_C = 0x08, /**< Not relevant - dual mode only. */
        SIMULTANEOUS_LE_BREDR_H = 0x10  /**< Not relevant - dual mode only. */
    };

    static const uint8_t default_flags = BREDR_NOT_SUPPORTED | LE_GENERAL_DISCOVERABLE;

    /** Create from raw value */
    adv_data_flags_t(uint8_t value = 0) : _value(value)
    {
    }

    adv_data_flags_t &setGeneralDiscoverable(bool enable = true)
    {
        _value &= ~0x03U;
        if (enable) {
            _value |= LE_GENERAL_DISCOVERABLE;
        }
        return *this;
    }

    adv_data_flags_t &setLimitedDiscoverable(bool enable = true)
    {
        _value &= ~0x03U;
        if (enable) {
            _value |= LE_LIMITED_DISCOVERABLE;
        }
        return *this;
    }

    adv_data_flags_t &setBredrNotSupported(bool enable = true)
    {
        _value &= ~BREDR_NOT_SUPPORTED;
        if (enable) {
            _value |= BREDR_NOT_SUPPORTED;
        }
        return *this;
    }

    adv_data_flags_t &setSimultaneousLeBredrC(bool enable = true)
    {
        _value &= ~SIMULTANEOUS_LE_BREDR_C;
        if (enable) {
            _value |= SIMULTANEOUS_LE_BREDR_C;
        }
        return *this;
    }

    adv_data_flags_t &setSimultaneousLeBredrH(bool enable = true)
    {
        _value &= ~SIMULTANEOUS_LE_BREDR_H;
        if (enable) {
            _value |= SIMULTANEOUS_LE_BREDR_H;
        }
        return *this;
    }

    bool getGeneralDiscoverable() const
    {
        return _value & LE_GENERAL_DISCOVERABLE;
    }

    bool getlimitedDiscoverable() const
    {
        return _value & LE_LIMITED_DISCOVERABLE;
    }

    bool getBrEdrNotSupported() const
    {
        return _value & BREDR_NOT_SUPPORTED;
    }

    bool getSimultaneousLeBredrC() const
    {
        return _value & SIMULTANEOUS_LE_BREDR_C;
    }

    bool getSimultaneousLeBredrH() const
    {
        return _value & SIMULTANEOUS_LE_BREDR_H;
    }

    void clear()
    {
        _value = 0;
    }

    uint8_t value() const
    {
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
    adv_data_appearance_t(type value) : SafeEnum(value)
    {
    }
};

/**
 * @}
 * @}
 */

} // namespace ble

#endif //BLE_GAP_ADVERTISINGDATATYPES_H
