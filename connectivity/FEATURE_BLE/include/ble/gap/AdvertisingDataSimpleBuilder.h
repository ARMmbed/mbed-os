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

#ifndef BLE_GAP_SIMPLEADVERTISINGDATABUILDER_H
#define BLE_GAP_SIMPLEADVERTISINGDATABUILDER_H

#include "platform/Span.h"

#include "ble/common/UUID.h"
#include "ble/gap/AdvertisingDataBuilder.h"

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
 * It is a simplified version of AdvertisingDataBuilder that can generate
 * advertising data "inline".
 *
 * It differs from AdvertisingDataBuilder on the following points:
 *   - The buffer used to build the advertising data is embedded in the object.
 *   - If insertion fails, an assertion is raised. Outside of debug mode, if an
 *   insertion fails, the buffer is not modified.
 *   - The API is fluent.
 *   - It hides advanced functions.
 *
 * @code
    void setupAdvertisingData(ble::Gap& gap)
    {
        using namespace ble;
        gap.setAdvertisingPayload(
            LEGACY_ADVERTISING_HANDLE,
            AdvertisingDataSimpleBuilder<LEGACY_ADVERTISING_MAX_SIZE>()
                .setFlags()
                .setName("My device", true)
                .setAppearance(adv_data_appearance_t::GENERIC_HEART_RATE_SENSOR)
                .setLocalService(ATT_UUID_HEART_RATE_SERVICE)
                .getAdvertisingData()
        );
    }
 * @endcode
 */
template<size_t DataSize>
class AdvertisingDataSimpleBuilder {
public:
    /**
     * Construct a AdvertisingDataSimpleBuilder
     */
    AdvertisingDataSimpleBuilder() : _buffer(), _builder(_buffer)
    {
    }

    /**
     * Add device appearance in the advertising payload.
     *
     * @param[in] appearance The appearance to advertise.
     *
     * @return A reference to this object.
     *
     * @note If the field is already present in the payload, it is replaced.
     */
    AdvertisingDataSimpleBuilder &setAppearance(adv_data_appearance_t appearance)
    {
        ble_error_t res = _builder.setAppearance(appearance);
        MBED_ASSERT(res == BLE_ERROR_NONE);
        return *this;
    }

    /**
     * Add BLE flags in the advertising payload.
     *
     * @param[in] flags Bitfield describing the capability of the device. See
     * allowed flags in Flags_t.
     *
     * @return A reference to this object.
     *
     * @note If the field is already present in the payload, it is replaced.
     */
    AdvertisingDataSimpleBuilder &setFlags(
        adv_data_flags_t flags = adv_data_flags_t::default_flags
    )
    {
        ble_error_t res = _builder.setFlags(flags);
        MBED_ASSERT(res == BLE_ERROR_NONE);
        return *this;
    }

    /**
     * Add the advertising TX in the advertising payload.
     *
     * @param[in] txPower Transmission power level in dB.
     *
     * @return A reference to this object.
     *
     * @note If the field is already present in the payload, it is replaced.
     */
    AdvertisingDataSimpleBuilder &setTxPowerAdvertised(advertising_power_t txPower)
    {
        ble_error_t res = _builder.setTxPowerAdvertised(txPower);
        MBED_ASSERT(res == BLE_ERROR_NONE);
        return *this;
    }

    /**
     * Add device name to the advertising payload.
     *
     * @note Data size for individual types cannot exceed 255 bytes.
     *
     * @param[in] name Null terminated string containing the name.
     * @param[in] complete Complete local name if true, otherwise
     *
     * @return A reference to this object.
     *
     * @note If the field is already present in the payload, it is replaced.
     */
    AdvertisingDataSimpleBuilder &setName(const char *name, bool complete = true)
    {
        ble_error_t res = _builder.setName(name, complete);
        MBED_ASSERT(res == BLE_ERROR_NONE);
        return *this;
    }

    /**
     * Add manufacturer specific data to the advertising payload.
     *
     * @note Data size for individual types cannot exceed 255 bytes.
     *
     * @param[in] data New data to be added.
     *
     * @return a reference to this object.
     */
    AdvertisingDataSimpleBuilder &setManufacturerSpecificData(mbed::Span<const uint8_t> data)
    {
        ble_error_t res = _builder.setManufacturerSpecificData(data);
        MBED_ASSERT(res == BLE_ERROR_NONE);
        return *this;
    }

    /**
     * Add advertising interval to the payload. This field can only carry 2 bytes.
     *
     * @param interval Interval to advertise. Cannot be larger than 0xFFFF.
     *
     * @return a reference to this object.
     */
    AdvertisingDataSimpleBuilder &setAdvertisingInterval(adv_interval_t interval)
    {
        ble_error_t res = _builder.setAdvertisingInterval(interval);
        MBED_ASSERT(res == BLE_ERROR_NONE);
        return *this;
    }

    /**
     * Add connection interval preferences to the payload
     *
     * @param min Minimum connection interval to advertise.
     * @param max Maximum connection interval to advertise.
     *
     * @return a reference to this object.
     */
    AdvertisingDataSimpleBuilder &setConnectionIntervalPreference(
        conn_interval_t min,
        conn_interval_t max
    )
    {
        ble_error_t res = _builder.setConnectionIntervalPreference(min, max);
        MBED_ASSERT(res == BLE_ERROR_NONE);
        return *this;
    }

    /**
     * Add service data to the advertising payload.
     *
     * @note Data size for individual types cannot exceed 255 bytes.
     *
     * @param[in] service UUID of the service.
     * @param[in] data New data to be added.
     *
     * @return A reference to this object.
     */
    AdvertisingDataSimpleBuilder &setServiceData(const UUID& service, mbed::Span<const uint8_t> data)
    {
        ble_error_t res = _builder.setServiceData(service, data);
        MBED_ASSERT(res == BLE_ERROR_NONE);
        return *this;
    }

    /**
     * Add local service ID to the advertising payload. If the data can't fit,
     * no modification will take place.
     *
     * @note Data size for individual types cannot exceed 255 bytes.
     *
     * @param[in] data New data to be added.
     * @param[in] complete True if this is a complete list.
     *
     * @return A reference to this object.
     */
    AdvertisingDataSimpleBuilder &setLocalService(
        const UUID& data,
        bool complete = true
    )
    {
        ble_error_t res = _builder.setLocalServiceList(
                mbed::make_Span(&data, 1), complete
            );
        MBED_ASSERT(res == BLE_ERROR_NONE);
        return *this;
    }


    /**
     * Add local service IDs to the advertising payload. If the data can't fit,
     * no modification will take place.
     *
     * @note Data size for individual types cannot exceed 255 bytes.
     *
     * @param[in] data New data to be added.
     * @param[in] complete True if this is a complete list.
     *
     * @return A reference to this object.
     */
    AdvertisingDataSimpleBuilder &setLocalServiceList(
        mbed::Span<const UUID> data,
        bool complete = true
    )
    {
        ble_error_t res = _builder.setLocalServiceList(data, complete);
        MBED_ASSERT(res == BLE_ERROR_NONE);
        return *this;
    }

    /**
     * Add a UUID of a solicited service.
     *
     * @note Data size for individual types cannot exceed 255 bytes.
     *
     * @param[in] data List of 128 or 16 bit service UUIDs.
     *
     * @return A reference to this object.
     */
    AdvertisingDataSimpleBuilder &setRequestedService(const UUID& data)
    {
        ble_error_t res = _builder.setRequestedServiceList(mbed::make_Span(&data, 1));
        MBED_ASSERT(res == BLE_ERROR_NONE);
        return *this;
    }

    /**
     * Add a list of UUIDs of solicited services.
     *
     * @note Data size for individual types cannot exceed 255 bytes.
     *
     * @param[in] data List of 128 or 16 bit service UUIDs.
     *
     * @return A reference to this object.
     */
    AdvertisingDataSimpleBuilder &setRequestedServiceList(mbed::Span<const UUID> data)
    {
        ble_error_t res = _builder.setRequestedServiceList(data);
        MBED_ASSERT(res == BLE_ERROR_NONE);
        return *this;
    }

    /**
     * Add a new field into the payload. The operation fails if type is already present.
     *
     * @note Data size for individual types cannot exceed 255 bytes.
     *
     * @param[in] advDataType The type of the field to add.
     * @param[in] fieldData Span of data to add.
     *
     * @return A reference to this object.
     */
    AdvertisingDataSimpleBuilder& addData(
        adv_data_type_t advDataType,
        mbed::Span<const uint8_t> fieldData
    )
    {
        ble_error_t res = _builder.addData(advDataType, fieldData);
        MBED_ASSERT(res == BLE_ERROR_NONE);
        return *this;
    }

    /**
     * Get the subspan of the buffer containing valid data.
     *
     * @return A Span containing the payload.
     */
    mbed::Span<const uint8_t> getAdvertisingData() const
    {
        return _builder.getAdvertisingData();
    }

private:
    uint8_t _buffer[DataSize];
    AdvertisingDataBuilder _builder;
};

/**
 * @}
 * @}
 */

} // namespace ble


#endif //BLE_GAP_SIMPLEADVERTISINGDATABUILDER_H
