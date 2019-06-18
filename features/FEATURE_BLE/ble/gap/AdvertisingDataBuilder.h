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

#include "ble/BLETypes.h"
#include "platform/NonCopyable.h"

#include "ble/blecommon.h"
#include "UUID.h"
#include "ble/gap/AdvertisingDataTypes.h"
#include "ble/gap/Types.h"

namespace ble {

/**
 * @addtogroup ble
 * @{
 * @addtogroup gap
 * @{
 */

/**
 * Build advertising data.
 *
 * The builder accepts an array of bytes in input and returns the result of the
 * construction with getAdvertisingData().
 */
class AdvertisingDataBuilder {
public:
    /** Advertising data needs a user-provided buffer to store the data.
     *
     * @param buffer Buffer used to store the data.
     * @note Use Gap::getMaxAdvertisingDataLength() to find out how much can be accepted.
     */
    AdvertisingDataBuilder(mbed::Span<uint8_t> buffer);

    /** Advertising data needs a user provided buffer to store the data.
     *
     * @param buffer Pointer to buffer to be used for storing advertising data.
     * @param buffer_size Size of the buffer.
     * @note Use Gap::getMaxAdvertisingDataLength() to find out how much can be accepted.
     */
    AdvertisingDataBuilder(uint8_t *buffer, size_t buffer_size);

    /**
     * Get the subspan of the buffer containing valid data.
     *
     * @return A Span containing the payload.
     */
    mbed::Span<const uint8_t> getAdvertisingData() const;

    /**
     * Add a new field into the payload. Returns an error if type is already present.
     *
     * @note Data size for individual types cannot exceed 255 bytes.
     *
     * @param[in] advDataType The type of the field to add.
     * @param[in] fieldData Span of data to add.
     *
     * @retval BLE_ERROR_NONE on success.
     * @retval BLE_ERROR_BUFFER_OVERFLOW if buffer is too small to contain the new data.
     * @retval BLE_ERROR_OPERATION_NOT_PERMITTED if data type already present.
     * @retval BLE_ERROR_INVALID_PARAM if size of data is too big too fit in an individual data field.
     */
    ble_error_t addData(
        adv_data_type_t advDataType,
        mbed::Span<const uint8_t> fieldData
    );

    /**
     * Replace a new field into the payload. Will fail if type is not already present.
     *
     * @note Data size for individual types cannot exceed 255 bytes.
     *
     * @param[in] advDataType The type of the field to add.
     * @param[in] fieldData Span of data to add.
     *
     * @retval BLE_ERROR_NONE on success.
     * @retval BLE_ERROR_BUFFER_OVERFLOW if buffer is too small to contain the new data.
     * @retval BLE_ERROR_NOT_FOUND if data type not present.
     * @retval BLE_ERROR_INVALID_PARAM if size of data is too big too fit in an individual data field.
     */
    ble_error_t replaceData(
        adv_data_type_t advDataType,
        mbed::Span<const uint8_t> fieldData
    );

    /**
     * Append data to an existing field in the payload. Will fail if type is not already
     * present.
     *
     * @note Data size for individual types cannot exceed 255 bytes.
     *
     * @param[in] advDataType The type of the field to add.
     * @param[in] fieldData Span of data to add.
     *
     * @retval BLE_ERROR_NONE on success.
     * @retval BLE_ERROR_BUFFER_OVERFLOW if buffer is too small to contain the new data.
     * @retval BLE_ERROR_NOT_FOUND if data type not present.
     * @retval BLE_ERROR_INVALID_PARAM if size of data is too big too fit in an individual data field.
     */
    ble_error_t appendData(
        adv_data_type_t advDataType,
        mbed::Span<const uint8_t> fieldData
    );

    /**
     * Remove existing date of given type. Will return an error if type is not present.
     *
     * @param[in] advDataType The type of the field to remove.
     *
     * @return BLE_ERROR_NONE returned on success, BLE_ERROR_INVALID_PARAM if field doesn't exist
     */
    ble_error_t removeData(adv_data_type_t advDataType);

    /**
     * Adds a new field into the payload. If the supplied advertising data type is
     * already present in the advertising payload, then the value is replaced.
     *
     * @note Data size for individual types cannot exceed 255 bytes.
     *
     * @param[in] advDataType The type of the field to add.
     * @param[in] fieldData Span of data to add.
     *
     * @retval BLE_ERROR_NONE on success.
     * @retval BLE_ERROR_BUFFER_OVERFLOW if buffer is too small to contain the new data.
     * @retval BLE_ERROR_INVALID_PARAM if size of data is too big too fit in an individual data field.
     */
    ble_error_t addOrReplaceData(
        adv_data_type_t advDataType,
        mbed::Span<const uint8_t> fieldData
    );

    /**
     * Adds a new field into the payload. If the supplied advertising data type is
     * already present in the advertising payload, then the value is replaced.
     *
     * @note Data size for individual types cannot exceed 255 bytes.
     *
     * @param[in] advDataType The type of the field to add.
     * @param[in] fieldData Span of data to add.
     *
     * @retval BLE_ERROR_NONE on success.
     * @retval BLE_ERROR_BUFFER_OVERFLOW if buffer is too small to contain the new data.
     * @retval BLE_ERROR_INVALID_PARAM if size of data is too big too fit in an individual data field.
     */
    ble_error_t addOrAppendData(
        adv_data_type_t advDataType,
        mbed::Span<const uint8_t> fieldData
    );

    /**
     * Clears the advertising data payload.
     *
     * @post getPayloadLen() returns 0.
     */
    void clear();

    /**
     * Add device appearance in the advertising payload.
     *
     * @param[in] appearance The appearance to advertise.
     *
     * @retval BLE_ERROR_NONE on success.
     * @retval BLE_ERROR_BUFFER_OVERFLOW if buffer is too small to contain the new data.
     *
     * @note This call is equivalent to calling addOrReplaceData() with
     * adv_data_type_t::APPEARANCE as the field type.
     */
    ble_error_t setAppearance(adv_data_appearance_t appearance);

    /**
     * Add BLE flags in the advertising payload.
     *
     * @param[in] flags Bitfield describing the capability of the device. See
     * allowed flags in Flags_t.
     *
     * @retval BLE_ERROR_NONE on success.
     * @retval BLE_ERROR_BUFFER_OVERFLOW if buffer is too small to contain the new data.
     *
     * @note This call is equivalent to calling addOrReplaceData() with
     * adv_data_type_t::FLAGS as the field type.
     */
    ble_error_t setFlags(
        adv_data_flags_t flags = adv_data_flags_t::default_flags
    );

    /**
     * Add the advertising TX in the advertising payload.
     *
     * @param[in] txPower Transmission power level in dB.
     *
     * @retval BLE_ERROR_NONE on success.
     * @retval BLE_ERROR_BUFFER_OVERFLOW if buffer is too small to contain the new data.
     *
     * @note This call is equivalent to calling addOrReplaceData() with
     * adv_data_type_t::TX_POWER_LEVEL as the field type.
     */
    ble_error_t setTxPowerAdvertised(advertising_power_t txPower);

    /**
     * Add device name to the advertising payload.
     *
     * @note Data size for individual types cannot exceed 255 bytes.
     *
     * @param[in] name Null terminated string containing the name.
     * @param[in] complete Complete local name if true, otherwise
     *
     * @retval BLE_ERROR_NONE on success.
     * @retval BLE_ERROR_BUFFER_OVERFLOW if buffer is too small to contain the new data.
     * @retval BLE_ERROR_INVALID_PARAM if size of data is too big too fit in an individual data field.
     */
    ble_error_t setName(const char *name, bool complete = true);

    /**
     * Add manufacturer-specific data to the advertising payload.
     *
     * @note Data size for individual types cannot exceed 255 bytes.
     *
     * @param[in] data New data to be added.
     *
     * @retval BLE_ERROR_NONE on success.
     * @retval BLE_ERROR_BUFFER_OVERFLOW if buffer is too small to contain the new data.
     * @retval BLE_ERROR_INVALID_PARAM if size of data is too big too fit in an individual
     *                                 data field or the data is too small (must contain
     *                                 2 bytes of manufacturer ID)
     */
    ble_error_t setManufacturerSpecificData(mbed::Span<const uint8_t> data);

    /**
     * Add advertising interval to the payload. This field can only carry 2 bytes.
     *
     * @param interval Interval to advertise. Cannot be larger than 0xFFFF.

     * @retval BLE_ERROR_NONE on success.
     * @retval BLE_ERROR_BUFFER_OVERFLOW if buffer is too small to contain the new data.
     * @retval BLE_ERROR_INVALID_PARAM if interval value outside of valid range.
     */
    ble_error_t setAdvertisingInterval(adv_interval_t interval);

    /**
     * Add connection interval preferences to the payload
     *
     * @param min Minimum connection interval to advertise.
     * @param max Maximum connection interval to advertise.

     * @retval BLE_ERROR_NONE on success.
     * @retval BLE_ERROR_BUFFER_OVERFLOW if buffer is too small to contain the new data.
     */
    ble_error_t setConnectionIntervalPreference(
        conn_interval_t min,
        conn_interval_t max
    );

    /**
     * Add service data data to the advertising payload.
     *
     * @note Data size for individual types cannot exceed 255 bytes.
     *
     * @param[in] service UUID of the service.
     * @param[in] data New data to be added.
     *
     * @retval BLE_ERROR_NONE on success.
     * @retval BLE_ERROR_BUFFER_OVERFLOW if buffer is too small to contain the new data.
     * @retval BLE_ERROR_INVALID_PARAM if size of data is too big too fit in an individual data field.
     */
    ble_error_t setServiceData(UUID service, mbed::Span<const uint8_t> data);

    /**
     * Add local service IDs to the advertising payload. If the data can't fit,
     * no modification will take place.
     *
     * @note Data size for individual types cannot exceed 255 bytes.
     *
     * @param[in] data New data to be added.
     * @param[in] complete True if this is a complete list.
     *
     * @retval BLE_ERROR_NONE on success.
     * @retval BLE_ERROR_BUFFER_OVERFLOW if buffer is too small to contain the new data.
     * @retval BLE_ERROR_INVALID_PARAM if number of UUIDs of any one type is too high.
     */
    ble_error_t setLocalServiceList(
        mbed::Span<const UUID> data,
        bool complete = true
    );

    /**
     * Add a list of UUIDs of solicited services.
     *
     * @note Data size for individual types cannot exceed 255 bytes.
     *
     * @param[in] data List of 128 or 16 bit service UUIDs.
     *
     * @retval BLE_ERROR_NONE on success.
     * @retval BLE_ERROR_BUFFER_OVERFLOW if buffer is too small to contain the new data.
     * @retval BLE_ERROR_INVALID_PARAM if number of UUIDs of any one type is too high.
     */
    ble_error_t setRequestedServiceList(mbed::Span<const UUID> data);

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
    );

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
    uint8_t *findField(adv_data_type_t type);

    /**
     * Get field size (includes type and size bytes)
     *
     * @param type The field type.
     *
     * @return Size of the whole field including type and size bytes.
     */
    uint8_t getFieldSize(adv_data_type_t type);

    /**
     * Append advertising data based on the specified type.
     *
     * @note Data size for individual types cannot exceed 255 bytes.
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
    );

    /**
     * Append data to a field in the advertising payload.
     *
     * @note Data size for individual types cannot exceed 255 bytes.
     *
     * @param[in] fieldData Span of data to add.
     * @param[in] field Pointer to the field in the advertising buffer.
     *
     * @return BLE_ERROR_NONE on success.
     */
    ble_error_t appendToField(
        mbed::Span<const uint8_t> fieldData,
        uint8_t *field
    );

    /**
     * Update in place the value of a field in the advertising payload.
     *
     * @note Data size for individual types cannot exceed 255 bytes.
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
        uint8_t *field
    );

    /**
     * Remove the field.
     *
     * @param[in] field Pointer to the field in the advertising buffer.
     *
     * @return BLE_ERROR_NONE on success.
     */
    ble_error_t removeField(uint8_t *field);

    /**
     * Add a list of UUIDs to given types.
     *
     * @note Data size for individual types cannot exceed 255 bytes.
     *
     * @param[in] data List of 128 or 16 bit service UUIDs.
     * @param[in] shortType Type of field to add the short UUIDs to.
     * @param[in] longType Type of field to add the long UUIDs to.
     *
     * @retval BLE_ERROR_NONE on success.
     * @retval BLE_ERROR_BUFFER_OVERFLOW if buffer is too small to contain the new data.
     * @retval BLE_ERROR_INVALID_PARAM if number of UUIDs of any one type is too high.
     */
    ble_error_t setUUIDData(
        mbed::Span<const UUID> data,
        adv_data_type_t shortType,
        adv_data_type_t longType
    );

private:
    /** The memory backing the the data provided by the user. */
    mbed::Span<uint8_t> _buffer;

    /** Length of the data added to the advertising buffer. */
    uint8_t _payload_length;
};

/**
 * @}
 * @}
 */

} // namespace ble

#endif /* ifndef MBED_GAP_ADVERTISING_DATA_H__ */
