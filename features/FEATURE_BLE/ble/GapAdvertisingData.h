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

#ifndef __GAP_ADVERTISING_DATA_H__
#define __GAP_ADVERTISING_DATA_H__

#include <stdint.h>
#include <string.h>

#include "blecommon.h"

#define GAP_ADVERTISING_DATA_MAX_PAYLOAD        (31)

/**
 * @brief This class provides several helper functions to generate properly
 *        formatted GAP Advertising and Scan Response data payloads.
 *
 * @note See Bluetooth Specification 4.0 (Vol. 3), Part C, Sections 11 and 18
 *       for further information on Advertising and Scan Response data.
 *
 * @par Advertising and Scan Response Payloads
 *      Advertising data and Scan Response data are organized around a set of
 *      data types called 'AD types' in Bluetooth 4.0 (see the Bluetooth Core
 *      Specification v4.0, Vol. 3, Part C, Sections 11 and 18).
 *
 * @par
 *      Each AD type has its own standardized assigned number, as defined
 *      by the Bluetooth SIG:
 *      https://www.bluetooth.org/en-us/specification/assigned-numbers/generic-access-profile.
 *
 * @par
 *      For convenience, all appropriate AD types are encapsulated
 *      in GapAdvertisingData::DataType.
 *
 * @par
 *      Before the AD Types and their payload (if any) can be inserted into
 *      the Advertising or Scan Response frames, they need to be formatted as
 *      follows:
 *
 * @li @c Record length (1 byte).
 * @li @c AD Type (1 byte).
 * @li @c AD payload (optional; only present if record length > 1).
 *
 * @par
 *      This class takes care of properly formatting the payload, performs
 *      some basic checks on the payload length, and tries to avoid common
 *      errors like adding an exclusive AD field twice in the Advertising
 *      or Scan Response payload.
 *
 * @par EXAMPLE
 *
 * @code
 *
 * // ToDo
 *
 * @endcode
 */
class GapAdvertisingData
{
public:
    /*!
     * @brief A list of Advertising Data types commonly used by peripherals.
     *        These AD types are used to describe the capabilities of the
     *        peripheral, and are inserted inside the advertising or scan
     *        response payloads.
     *
     * @par Source
     *
     * @li @c Bluetooth Core Specification 4.0 (Vol. 3), Part C, Section 11, 18.
     * @li @c https://www.bluetooth.org/en-us/specification/assigned-numbers/generic-access-profile.
     */
    enum DataType_t {
        FLAGS                              = 0x01, /**< Flags, refer to GapAdvertisingData::Flags_t. */
        INCOMPLETE_LIST_16BIT_SERVICE_IDS  = 0x02, /**< Incomplete list of 16-bit Service IDs. */
        COMPLETE_LIST_16BIT_SERVICE_IDS    = 0x03, /**< Complete list of 16-bit Service IDs. */
        INCOMPLETE_LIST_32BIT_SERVICE_IDS  = 0x04, /**< Incomplete list of 32-bit Service IDs (not relevant for Bluetooth 4.0). */
        COMPLETE_LIST_32BIT_SERVICE_IDS    = 0x05, /**< Complete list of 32-bit Service IDs (not relevant for Bluetooth 4.0). */
        INCOMPLETE_LIST_128BIT_SERVICE_IDS = 0x06, /**< Incomplete list of 128-bit Service IDs. */
        COMPLETE_LIST_128BIT_SERVICE_IDS   = 0x07, /**< Complete list of 128-bit Service IDs. */
        SHORTENED_LOCAL_NAME               = 0x08, /**< Shortened Local Name. */
        COMPLETE_LOCAL_NAME                = 0x09, /**< Complete Local Name. */
        TX_POWER_LEVEL                     = 0x0A, /**< TX Power Level (in dBm). */
        DEVICE_ID                          = 0x10, /**< Device ID. */
        SLAVE_CONNECTION_INTERVAL_RANGE    = 0x12, /**< Slave Connection Interval Range. */
        LIST_128BIT_SOLICITATION_IDS       = 0x15, /**< List of 128 bit service UUIDs the device is looking for. */
        SERVICE_DATA                       = 0x16, /**< Service Data. */
        APPEARANCE                         = 0x19, /**< Appearance, refer to GapAdvertisingData::Appearance_t. */
        ADVERTISING_INTERVAL               = 0x1A, /**< Advertising Interval. */
        MANUFACTURER_SPECIFIC_DATA         = 0xFF  /**< Manufacturer Specific Data. */
    };
    /**
     * Type alias for GapAdvertisingData::DataType_t.
     *
     * @deprecated  This type alias will be dropped in future releases.
     */
    typedef enum DataType_t DataType;

    /**
     *  @brief A list of values for the FLAGS AD Type.
     *
     *  @note You can use more than one value in the FLAGS AD Type (ex.
     *        LE_GENERAL_DISCOVERABLE and BREDR_NOT_SUPPORTED).
     *
     *  @par Source
     *
     *  @li @c Bluetooth Core Specification 4.0 (Vol. 3), Part C, Section 18.1.
     */
    enum Flags_t {
        LE_LIMITED_DISCOVERABLE = 0x01, /**< Peripheral device is discoverable for a limited period of time. */
        LE_GENERAL_DISCOVERABLE = 0x02, /**< Peripheral device is discoverable at any moment. */
        BREDR_NOT_SUPPORTED     = 0x04, /**< Peripheral device is LE only. */
        SIMULTANEOUS_LE_BREDR_C = 0x08, /**< Not relevant - central mode only. */
        SIMULTANEOUS_LE_BREDR_H = 0x10  /**< Not relevant - central mode only. */
    };
    /**
     * Type alias for GapAdvertisingData::Flags_t.
     *
     * @deprecated  This type alias will be dropped in future releases.
     */
    typedef enum Flags_t Flags;

    /**
     *  @brief
     *  A list of values for the APPEARANCE AD Type, which describes the
     *  physical shape or appearance of the device.
     *
     *  @par Source
     *
     *  @li @c Bluetooth Core Specification Supplement, Part A, Section 1.12.
     *  @li @c Bluetooth Core Specification 4.0 (Vol. 3), Part C, Section 12.2.
     *  @li @c https://developer.bluetooth.org/gatt/characteristics/Pages/CharacteristicViewer.aspx?u=org.bluetooth.characteristic.gap.appearance.xml.
     */
    enum Appearance_t {
        UNKNOWN                                        = 0,     /**< Unknown or unspecified appearance type. */
        GENERIC_PHONE                                  = 64,    /**< Generic Phone. */
        GENERIC_COMPUTER                               = 128,   /**< Generic Computer. */
        GENERIC_WATCH                                  = 192,   /**< Generic Watch. */
        WATCH_SPORTS_WATCH                             = 193,   /**< Sports Watch. */
        GENERIC_CLOCK                                  = 256,   /**< Generic Clock. */
        GENERIC_DISPLAY                                = 320,   /**< Generic Display. */
        GENERIC_REMOTE_CONTROL                         = 384,   /**< Generic Remote Control. */
        GENERIC_EYE_GLASSES                            = 448,   /**< Generic Eye Glasses. */
        GENERIC_TAG                                    = 512,   /**< Generic Tag. */
        GENERIC_KEYRING                                = 576,   /**< Generic Keyring. */
        GENERIC_MEDIA_PLAYER                           = 640,   /**< Generic Media Player. */
        GENERIC_BARCODE_SCANNER                        = 704,   /**< Generic Barcode Scanner. */
        GENERIC_THERMOMETER                            = 768,   /**< Generic Thermometer. */
        THERMOMETER_EAR                                = 769,   /**< Ear Thermometer. */
        GENERIC_HEART_RATE_SENSOR                      = 832,   /**< Generic Heart Rate Sensor. */
        HEART_RATE_SENSOR_HEART_RATE_BELT              = 833,   /**< Belt Heart Rate Sensor. */
        GENERIC_BLOOD_PRESSURE                         = 896,   /**< Generic Blood Pressure. */
        BLOOD_PRESSURE_ARM                             = 897,   /**< Arm Blood Pressure. */
        BLOOD_PRESSURE_WRIST                           = 898,   /**< Wrist Blood Pressure. */
        HUMAN_INTERFACE_DEVICE_HID                     = 960,   /**< Human Interface Device (HID). */
        KEYBOARD                                       = 961,   /**< Keyboard. */
        MOUSE                                          = 962,   /**< Mouse. */
        JOYSTICK                                       = 963,   /**< Joystick. */
        GAMEPAD                                        = 964,   /**< Gamepad. */
        DIGITIZER_TABLET                               = 965,   /**< Digitizer Tablet. */
        CARD_READER                                    = 966,   /**< Card Reader. */
        DIGITAL_PEN                                    = 967,   /**< Digital Pen. */
        BARCODE_SCANNER                                = 968,   /**< Barcode Scanner. */
        GENERIC_GLUCOSE_METER                          = 1024,  /**< Generic Glucose Meter. */
        GENERIC_RUNNING_WALKING_SENSOR                 = 1088,  /**< Generic Running/Walking Sensor. */
        RUNNING_WALKING_SENSOR_IN_SHOE                 = 1089,  /**< In Shoe Running/Walking Sensor. */
        RUNNING_WALKING_SENSOR_ON_SHOE                 = 1090,  /**< On Shoe Running/Walking Sensor. */
        RUNNING_WALKING_SENSOR_ON_HIP                  = 1091,  /**< On Hip Running/Walking Sensor. */
        GENERIC_CYCLING                                = 1152,  /**< Generic Cycling. */
        CYCLING_CYCLING_COMPUTER                       = 1153,  /**< Cycling Computer. */
        CYCLING_SPEED_SENSOR                           = 1154,  /**< Cycling Speed Sensor. */
        CYCLING_CADENCE_SENSOR                         = 1155,  /**< Cycling Cadence Sensor. */
        CYCLING_POWER_SENSOR                           = 1156,  /**< Cycling Power Sensor. */
        CYCLING_SPEED_AND_CADENCE_SENSOR               = 1157,  /**< Cycling Speed and Cadence Sensor. */
        PULSE_OXIMETER_GENERIC                         = 3136,  /**< Generic Pulse Oximeter. */
        PULSE_OXIMETER_FINGERTIP                       = 3137,  /**< Fingertip Pulse Oximeter. */
        PULSE_OXIMETER_WRIST_WORN                      = 3138,  /**< Wrist Worn Pulse Oximeter. */
        GENERIC_WEIGHT_SCALE                           = 3200,  /**< Generic Weight Scale. */
        OUTDOOR_GENERIC                                = 5184,  /**< Generic Outdoor. */
        OUTDOOR_LOCATION_DISPLAY_DEVICE                = 5185,  /**< Outdoor Location Display Device. */
        OUTDOOR_LOCATION_AND_NAVIGATION_DISPLAY_DEVICE = 5186,  /**< Outdoor Location and Navigation Display Device. */
        OUTDOOR_LOCATION_POD                           = 5187,  /**< Outdoor Location Pod. */
        OUTDOOR_LOCATION_AND_NAVIGATION_POD            = 5188   /**< Outdoor Location and Navigation Pod. */
    };
    /**
     * Type alias for GapAdvertisingData::Appearance_t.
     *
     * @deprecated  This type alias will be dropped in future releases.
     */
    typedef enum Appearance_t Appearance;

    /**
     * Empty constructor.
     */
    GapAdvertisingData(void) : _payload(), _payloadLen(0), _appearance(GENERIC_TAG) {
        /* empty */
    }

    /**
     * Adds advertising data based on the specified AD type (see GapAdvertisingData::DataType_t).
     * If the supplied AD type is already present in the advertising
     * payload, then the value is updated.
     *
     * @param[in] advDataType  The Advertising 'DataType' to add.
     * @param[in] payload      Pointer to the payload contents.
     * @param[in] len          Size of the payload in bytes.
     *
     * @return BLE_ERROR_BUFFER_OVERFLOW if the new value causes the
     *         advertising buffer to overflow. BLE_ERROR_NONE is returned
     *         on success.
     *
     * @note When the specified AD type is INCOMPLETE_LIST_16BIT_SERVICE_IDS,
     *       COMPLETE_LIST_16BIT_SERVICE_IDS, INCOMPLETE_LIST_32BIT_SERVICE_IDS,
     *       COMPLETE_LIST_32BIT_SERVICE_IDS, INCOMPLETE_LIST_128BIT_SERVICE_IDS,
     *       COMPLETE_LIST_128BIT_SERVICE_IDS or LIST_128BIT_SOLICITATION_IDS the
     *       supplied value is appended to the values previously added to the
     *       payload.
     */
    ble_error_t addData(DataType_t advDataType, const uint8_t *payload, uint8_t len)
    {
        /* Find field */
        uint8_t* field = findField(advDataType);

        if (field) {
            /* Field type already exist, either add to field or replace */
            return addField(advDataType, payload, len, field);
        } else {
            /* Field doesn't exists, insert new */
            return appendField(advDataType, payload, len);
        }
    }

    /**
     * Update a particular ADV field in the advertising payload (based on
     * matching type).
     *
     * @param[in] advDataType  The Advertising 'DataType' to add.
     * @param[in] payload      Pointer to the payload contents.
     * @param[in] len          Size of the payload in bytes.
     *
     * @return BLE_ERROR_UNSPECIFIED if the specified field is not found,
     *         BLE_ERROR_BUFFER_OVERFLOW if the new value causes the
     *         advertising buffer to overflow. BLE_ERROR_NONE is returned
     *         on success.
     */
    ble_error_t updateData(DataType_t advDataType, const uint8_t *payload, uint8_t len)
    {
        /* Find field */
        uint8_t* field = findField(advDataType);

        if (field) {
            /* Field type already exist, replace field contents */
            return updateField(advDataType, payload, len, field);
        } else {
            /* field doesn't exists, return an error */
            return BLE_ERROR_UNSPECIFIED;
        }
    }

    /**
     * Helper function to add APPEARANCE data to the advertising payload.
     *
     * @param  appearance
     *           The APPEARANCE value to add.
     *
     * @return BLE_ERROR_BUFFER_OVERFLOW if the specified data would cause the
     *         advertising buffer to overflow, else BLE_ERROR_NONE.
     */
    ble_error_t addAppearance(Appearance appearance = GENERIC_TAG) {
        _appearance = appearance;
        return addData(GapAdvertisingData::APPEARANCE, (uint8_t *)&appearance, 2);
    }

    /**
     * Helper function to add FLAGS data to the advertising payload.
     *
     * @param[in]  flags
     *               LE_LIMITED_DISCOVERABLE
     *                 The peripheral is discoverable for a limited period of time.
     *               LE_GENERAL_DISCOVERABLE
     *                 The peripheral is permanently discoverable.
     *               BREDR_NOT_SUPPORTED
     *                 This peripheral is a Bluetooth Low Energy only device (no EDR support).
     *
     * @return BLE_ERROR_BUFFER_OVERFLOW if the specified data would cause the
     *         advertising buffer to overflow, else BLE_ERROR_NONE.
     */
    ble_error_t addFlags(uint8_t flags = LE_GENERAL_DISCOVERABLE) {
        return addData(GapAdvertisingData::FLAGS, &flags, 1);
    }

    /**
     * Helper function to add TX_POWER_LEVEL data to the advertising payload.
     *
     * @return BLE_ERROR_BUFFER_OVERFLOW if the specified data would cause the
     *         advertising buffer to overflow, else BLE_ERROR_NONE.
     */
    ble_error_t addTxPower(int8_t txPower) {
        /* To Do: Basic error checking to make sure txPower is in range. */
        return addData(GapAdvertisingData::TX_POWER_LEVEL, (uint8_t *)&txPower, 1);
    }

    /**
     * Clears the payload and resets the payload length counter.
     */
    void        clear(void) {
        memset(&_payload, 0, GAP_ADVERTISING_DATA_MAX_PAYLOAD);
        _payloadLen = 0;
    }

    /**
     * Access the current payload.
     *
     * @return A pointer to the current payload.
     */
    const uint8_t *getPayload(void) const {
        return _payload;
    }

    /**
     * Get the current payload length.
     *
     * @return The current payload length (0..31 bytes).
     */
    uint8_t     getPayloadLen(void) const {
        return _payloadLen;
    }

    /**
     * Get the current appearance value.
     *
     * @return The 16-bit appearance value for this device.
     */
    uint16_t    getAppearance(void) const {
        return (uint16_t)_appearance;
    }

    /**
     * Search advertisement data for a specific field.
     *
     * @param[in] type
     *              The type of the field to find.
     *
     * @return A pointer to the first element in the field if found, NULL otherwise.
     *         Where the first element is the length of the field.
     */
    const uint8_t* findField(DataType_t type) const {
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
     * Append advertising data based on the specified AD type (see
     * GapAdvertisingData::DataType_t).
     *
     * @param[in] advDataType
     *              The type of the new data.
     * @param[in] payload
     *              A pointer to the data to be appended to the advertising
     *              payload.
     * @param[in] len
     *              The length of th data pointed to by @p payload.
     *
     * @return BLE_ERROR_BUFFER_OVERFLOW if the specified data would cause the
     *         advertising buffer to overflow, else BLE_ERROR_NONE.
     */
    ble_error_t appendField(DataType advDataType, const uint8_t *payload, uint8_t len)
    {
        /* Make sure we don't exceed the 31 byte payload limit */
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
     * Search advertisement data for field.
     *
     * @param[in] type
     *              The type of the data to find.
     *
     * @return A pointer to the first element in the field if found, NULL
     *         otherwise. Where the first element is the length of the field.
     */
    uint8_t* findField(DataType_t type) {
        return const_cast<uint8_t*>(static_cast<const GapAdvertisingData*>(this)->findField(type));
    }

    /**
     * Given the a pointer to a field in the advertising payload it replaces
     * the existing data in the field with the supplied data.
     *
     * @param[in] advDataType
     *              The type of the new data.
     * @param[in] payload
     *              A pointer to the data to be added to the advertising
     *              payload.
     * @param[in] len
     *              The length of th data pointed to by @p payload.
     * @param[in] field
     *              A pointer to the field of type @p advDataType in the
     *              advertising buffer.
     *
     * When the specified AD type is INCOMPLETE_LIST_16BIT_SERVICE_IDS,
     * COMPLETE_LIST_16BIT_SERVICE_IDS, INCOMPLETE_LIST_32BIT_SERVICE_IDS,
     * COMPLETE_LIST_32BIT_SERVICE_IDS, INCOMPLETE_LIST_128BIT_SERVICE_IDS,
     * COMPLETE_LIST_128BIT_SERVICE_IDS or LIST_128BIT_SOLICITATION_IDS the
     * supplied value is appended to the values previously added to the
     * payload.
     *
     * @return BLE_ERROR_NONE on success.
     */
    ble_error_t addField(DataType_t advDataType, const uint8_t *payload, uint8_t len, uint8_t* field)
    {
        ble_error_t result = BLE_ERROR_BUFFER_OVERFLOW;

        switch(advDataType) {
            /* These fields will have the new data appended if there is sufficient space */
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
            /* These fields will be overwritten with the new value */
            default: {
                result = updateField(advDataType, payload, len, field);

                break;
            }
        }

        return result;
    }

    /**
     * Given the a pointer to a field in the advertising payload it replaces
     * the existing data in the field with the supplied data.
     *
     * @param[in] advDataType
     *              The type of the data to be updated.
     * @param[in] payload
     *              A pointer to the data to be updated to the advertising
     *              payload.
     * @param[in] len
     *              The length of th data pointed to by @p payload.
     * @param[in] field
     *              A pointer to the field of type @p advDataType in the
     *              advertising buffer.
     *
     * @return BLE_ERROR_NONE on success.
     */
    ble_error_t updateField(DataType_t advDataType, const uint8_t *payload, uint8_t len, uint8_t* field)
    {
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
     * The advertising data buffer
     */
    uint8_t  _payload[GAP_ADVERTISING_DATA_MAX_PAYLOAD];
    /**
     * The length of the data added to the advertising buffer.
     */
    uint8_t  _payloadLen;
    /**
     * Appearance value.
     */
    uint16_t _appearance;
};

#endif /* ifndef __GAP_ADVERTISING_DATA_H__ */
