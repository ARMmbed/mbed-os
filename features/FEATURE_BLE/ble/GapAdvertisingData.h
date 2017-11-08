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

#include "blecommon.h"

/**
 * @addtogroup ble
 * @{
 * @addtogroup gap
 * @{
 */

#define GAP_ADVERTISING_DATA_MAX_PAYLOAD        (31)

/**
 * GAP advertising data builder.
 *
 * Advertising data are used by broadcaster or peripheral to advertise state
 * about the device. This class offers the function to add and update states present
 * in an advertisement payload.
 *
 * After construction, the advertising payload contained in the instance of
 * GapAdvertisingData is empty. Adding new states and named fields can be
 * achieved by invoking the function addData(), and updating existing state
 * involves calling the function updateData().
 *
 * Fields present in the payload can be retrieved by a call to the function
 * findField.
 *
 * This class includes shorthand for the most common fields:
 *   - FLAGS: addFlags().
 *   - APPEARANCE: addAppearance().
 *   - TX_POWER_LEVEL: addTxPower().
 *
 * @code
 *
 * Gap &gap;
 *
 * static const uint8_t device_name[] = "HRM";
 *
 * // construct an empty advertising payload
 * GapAdvertisingData advertising_data;
 *
 * // set the flags of the advertising device
 * advertising_data.addFlags(
 *    GapAdvertisingData::LE_GENERAL_DISCOVERABLE |
 *    GapAdvertisingData::BREDR_NOT_SUPPORTED
 * );
 *
 * // set the advertised name of the device
 * advertising_data.addData(
 *     GapAdvertisingData::COMPLETE_LOCAL_NAME,
 *     device_name,
 *     sizeof(device_name)
 * );
 *
 * // update the advertising data of the gap payload
 * gap.setAdvertisingPayload(advertising_data);
 *
 * @endcode
 *
 * @note See Bluetooth Specification 4.0 (Vol. 3), Part C, Sections 11 and 18
 * for further information on advertising and scan response data.
 *
 * @par Advertising and Scan Response Payloads
 * Advertising data and scan response data are organized around a set of
 * data types called 'AD types' in Bluetooth 4.0 (see the Bluetooth Core
 * Specification v4.0, Vol. 3, Part C, Sections 11 and 18).
 *
 * @par
 * Each AD type has its own standardized assigned number, as
 * the Bluetooth SIG defines:
 * https://www.bluetooth.org/en-us/specification/assigned-numbers/generic-access-profile.
 *
 * @par
 * For convenience, all appropriate AD types are encapsulated in
 * GapAdvertisingData::DataType.
 *
 * @par
 * Before the AD Types and their payload (if any) can be inserted into
 * the advertising or scan response frames, they need to be formatted as
 * follows:
 *
 * @li @c Record length (1 byte).
 * @li @c AD Type (1 byte).
 * @li @c AD payload (optional; only present if record length > 1).
 *
 * @par
 * This class takes care of properly formatting the payload, performs
 * some basic checks on the payload length and tries to avoid common
 * errors such as adding an exclusive AD field twice in the advertising
 * or scan response payload.
 */
class GapAdvertisingData
{
public:
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
    enum DataType_t {
        /**
         * Flags, refer to GapAdvertisingData::Flags_t.
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
         * Appearance, refer to GapAdvertisingData::Appearance_t.
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
     * Alias for GapAdvertisingData::DataType_t.
     *
     * @deprecated Future releases will drop this type alias.
     */
    typedef enum DataType_t DataType;

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
    enum Flags_t {
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
     * Alias for GapAdvertisingData::Flags_t.
     *
     * @deprecated  Future releases will drop this type alias.
     */
    typedef enum Flags_t Flags;

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
    enum Appearance_t {
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
     * Alias for GapAdvertisingData::Appearance_t.
     *
     * @deprecated Future releases will drop this type alias.
     */
    typedef enum Appearance_t Appearance;

    /**
     * Construct a GapAdvertising instance with an empty payload.
     */
    GapAdvertisingData(void) :
        _payload(),
        _payloadLen(0),
        _appearance(GENERIC_TAG) {
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
    ble_error_t addData(DataType_t advDataType, const uint8_t *payload, uint8_t len)
    {
        /* Find field */
        uint8_t* field = findField(advDataType);

        if (field) {
            /* Field type already exists, either add to field or replace */
            return addField(advDataType, payload, len, field);
        } else {
            /* Field doesn't exist, insert new */
            return appendField(advDataType, payload, len);
        }
    }

    /**
     * Update a specific field in the advertising payload.
     *
     * @param[in] advDataType The type of the field to update.
     * @param[in] payload Pointer to the updated value of the field.
     * @param[in] len Size of the new value in bytes.
     *
     * @return BLE_ERROR_NONE returned on success.
     * @return BLE_ERROR_UNSPECIFIED if the specified field is not found,
     * @return BLE_ERROR_BUFFER_OVERFLOW if the new value causes the
     * advertising buffer to overflow.
     */
    ble_error_t updateData(DataType_t advDataType, const uint8_t *payload, uint8_t len)
    {
        /* Find field */
        uint8_t* field = findField(advDataType);

        if (field) {
            /* Field type already exists, replace field contents */
            return updateField(advDataType, payload, len, field);
        } else {
            /* field doesn't exist, return an error */
            return BLE_ERROR_UNSPECIFIED;
        }
    }

    /**
     * Add device appearance in the advertising payload.
     *
     * @param[in] appearance The appearance to advertise.
     *
     * @return BLE_ERROR_NONE on success.
     * @return BLE_ERROR_BUFFER_OVERFLOW if the specified data would cause the
     * advertising buffer to overflow.
     *
     * @note This call is equivalent to calling addData() with
     * GapAdvertisingData::APPEARANCE as the field type.
     */
    ble_error_t addAppearance(Appearance appearance = GENERIC_TAG)
    {
        _appearance = appearance;
        return addData(GapAdvertisingData::APPEARANCE, (uint8_t *)&appearance, 2);
    }

    /**
     * Add BLE flags in the advertising payload.
     *
     * @param[in] flags Bitfield describing the capability of the device. See
     * allowed flags in Flags_t.
     *
     * @return BLE_ERROR_NONE on success.
     * @return BLE_ERROR_BUFFER_OVERFLOW if the specified data would cause the
     * advertising buffer to overflow.
     *
     * @note This call is equivalent to calling addData() with
     * GapAdvertisingData::FLAGS as the field type.
     */
    ble_error_t addFlags(uint8_t flags = LE_GENERAL_DISCOVERABLE)
    {
        return addData(GapAdvertisingData::FLAGS, &flags, 1);
    }

    /**
     * Add the advertising TX in the advertising payload.
     *
     * @param[in] txPower Transmission power level in dB.
     *
     * @return BLE_ERROR_NONE on success.
     * @return BLE_ERROR_BUFFER_OVERFLOW if the specified data would cause the
     * advertising buffer to overflow.
     *
     * @note This call is equivalent to calling addData() with
     * GapAdvertisingData::TX_POWER_LEVEL as the field type.
     */
    ble_error_t addTxPower(int8_t txPower)
    {
        /* To Do: Basic error checking to make sure txPower is in range. */
        return addData(GapAdvertisingData::TX_POWER_LEVEL, (uint8_t *)&txPower, 1);
    }

    /**
     * Clears the advertising data payload.
     *
     * @post getPayloadLen() returns 0.
     */
    void clear(void)
    {
        memset(&_payload, 0, GAP_ADVERTISING_DATA_MAX_PAYLOAD);
        _payloadLen = 0;
    }

    /**
     * Get the pointer to the advertising payload bytes.
     *
     * @return A pointer to the payload.
     */
    const uint8_t *getPayload(void) const
    {
        return _payload;
    }

    /**
     * Get the payload length.
     *
     * @return The payload length in bytes.
     */
    uint8_t getPayloadLen(void) const
    {
        return _payloadLen;
    }

    /**
     * Get the appearance set.
     *
     * If no value has been set, this function returns GENERIC_TAG.
     *
     * @return The appearance value set for this device.
     */
    uint16_t getAppearance(void) const
    {
        return (uint16_t)_appearance;
    }

    /**
     * Search advertisement data for a specific field.
     *
     * @param[in] type The type of the field to find.
     *
     * @return A pointer to the first element in the field if found. The first
     * element being the length of the field followed by the value of the field.
     * @return NULL if the field is not present in the payload.
     */
    const uint8_t* findField(DataType_t type) const
    {
        /* Scan through advertisement data */
        for (uint8_t idx = 0; idx < _payloadLen; ) {
            uint8_t fieldType = _payload[idx + 1];

            if (fieldType == type) {
                return &_payload[idx];
            }

            /* Advance to next field */
            idx += _payload[idx] + 1;
        }

        /* Field not found */
        return NULL;
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
    ble_error_t appendField(DataType advDataType, const uint8_t *payload, uint8_t len)
    {
        /* Make sure we don't exceed the 31-byte payload limit */
        if (_payloadLen + len + 2 > GAP_ADVERTISING_DATA_MAX_PAYLOAD) {
            return BLE_ERROR_BUFFER_OVERFLOW;
        }

        /* Field length. */
        memset(&_payload[_payloadLen], len + 1, 1);
        _payloadLen++;

        /* Field ID. */
        memset(&_payload[_payloadLen], (uint8_t)advDataType, 1);
        _payloadLen++;

        /* Payload. */
        memcpy(&_payload[_payloadLen], payload, len);
        _payloadLen += len;

        return BLE_ERROR_NONE;
    }

    /**
     * Search advertisement data for a specific field.
     *
     * @param[in] type The type of the field to find.
     *
     * @return A pointer to the first element in the field if found. The first
     * element being the length of the field followed by the value of the field.
     * @return NULL if the field is not present in the payload.
     */
    uint8_t* findField(DataType_t type)
    {
        return const_cast<uint8_t*>(
            static_cast<const GapAdvertisingData*>(this)->findField(type)
        );
    }

    /**
     * Update in place the value of a field in the advertising payload.
     *
     * @param[in] advDataType Type of the new data.
     * @param[in] payload Pointer to the data to be added to the advertising
     * payload.
     * @param[in] len Length of the data pointed to by @p payload.
     * @param[in] field Pointer to the field of type @p advDataType in the
     * advertising buffer.
     *
     * @note When the specified AD type is INCOMPLETE_LIST_16BIT_SERVICE_IDS,
     * COMPLETE_LIST_16BIT_SERVICE_IDS, INCOMPLETE_LIST_32BIT_SERVICE_IDS,
     * COMPLETE_LIST_32BIT_SERVICE_IDS, INCOMPLETE_LIST_128BIT_SERVICE_IDS,
     * COMPLETE_LIST_128BIT_SERVICE_IDS or LIST_128BIT_SOLICITATION_IDS, the
     * supplied value is appended to the values previously added to the
     * payload.
     *
     * @return BLE_ERROR_NONE on success.
     */
    ble_error_t addField(
        DataType_t advDataType,
        const uint8_t *payload,
        uint8_t len,
        uint8_t* field
    ) {
        ble_error_t result = BLE_ERROR_BUFFER_OVERFLOW;

        switch(advDataType) {
            /* These fields have the new data appended if there is sufficient space. */
            case INCOMPLETE_LIST_16BIT_SERVICE_IDS:
            case COMPLETE_LIST_16BIT_SERVICE_IDS:
            case INCOMPLETE_LIST_32BIT_SERVICE_IDS:
            case COMPLETE_LIST_32BIT_SERVICE_IDS:
            case INCOMPLETE_LIST_128BIT_SERVICE_IDS:
            case COMPLETE_LIST_128BIT_SERVICE_IDS:
            case LIST_128BIT_SOLICITATION_IDS: {
                /* Check if data fits */
                if ((_payloadLen + len) <= GAP_ADVERTISING_DATA_MAX_PAYLOAD) {
                    /*
                     * Make room for new field by moving the remainder of the
                     * advertisement payload "to the right" starting after the
                     * TYPE field.
                     */
                    uint8_t* end = &_payload[_payloadLen];

                    while (&field[1] < end) {
                        end[len] = *end;
                        end--;
                    }

                    /* Insert new data */
                    for (uint8_t idx = 0; idx < len; idx++) {
                        field[2 + idx] = payload[idx];
                    }

                    /* Increment lengths */
                    field[0] += len;
                    _payloadLen += len;

                    result = BLE_ERROR_NONE;
                }

                break;
            }
            /* These fields are overwritten with the new value */
            default: {
                result = updateField(advDataType, payload, len, field);

                break;
            }
        }

        return result;
    }

    /**
     * Update in place the value of a field in the advertising payload.
     *
     * @param[in] advDataType Type of the new data.
     * @param[in] payload Pointer to the data to be added to the advertising
     * payload.
     * @param[in] len Length of the data pointed to by @p payload.
     * @param[in] field Pointer to the field of type @p advDataType in the
     * advertising buffer.
     *
     * @return BLE_ERROR_NONE on success.
     */
    ble_error_t updateField(
        DataType_t advDataType,
        const uint8_t *payload,
        uint8_t len,
        uint8_t* field
    ) {
        ble_error_t result = BLE_ERROR_BUFFER_OVERFLOW;
        uint8_t dataLength = field[0] - 1;

        /* New data has same length, do in-order replacement */
        if (len == dataLength) {
            for (uint8_t idx = 0; idx < dataLength; idx++) {
                field[2 + idx] = payload[idx];
            }

            result = BLE_ERROR_NONE;
        } else {
            /* Check if data fits */
            if ((_payloadLen - dataLength + len) <= GAP_ADVERTISING_DATA_MAX_PAYLOAD) {

                /* Remove old field */
                while ((field + dataLength + 2) < &_payload[_payloadLen]) {
                    *field = field[dataLength + 2];
                    field++;
                }

                /* Reduce length */
                _payloadLen -= dataLength + 2;

                /* Add new field */
                result = appendField(advDataType, payload, len);
            }
        }

        return result;
    }

    /**
     * Advertising data buffer.
     */
    uint8_t _payload[GAP_ADVERTISING_DATA_MAX_PAYLOAD];

    /**
     * Length of the data added to the advertising buffer.
     */
    uint8_t _payloadLen;

    /**
     * Appearance value.
     */
    uint16_t _appearance;
};

/**
 * @}
 * @}
 */


#endif /* ifndef MBED_GAP_ADVERTISING_DATA_H__ */
