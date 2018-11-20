/* mbed Microcontroller Library
 * Copyright (c) 2018 ARM Limited
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

#include "ble/gap/AdvertisingDataBuilder.h"

namespace ble {

AdvertisingDataBuilder::AdvertisingDataBuilder(mbed::Span<uint8_t> buffer) :
    _buffer(buffer),
    _payload_length(0) {
}

AdvertisingDataBuilder::AdvertisingDataBuilder(uint8_t* buffer, size_t buffer_size) :
    _buffer(buffer, buffer_size),
    _payload_length(0) {
}

mbed::Span<uint8_t> AdvertisingDataBuilder::getAdvertisingData() const {
    return _buffer.first(_payload_length);
}

ble_error_t AdvertisingDataBuilder::addData(
    adv_data_type_t advDataType,
    mbed::Span<const uint8_t> fieldData
) {
    if (findField(advDataType)) {
        return BLE_ERROR_OPERATION_NOT_PERMITTED;
    } else {
        return addField(advDataType, fieldData);
    }
}

ble_error_t AdvertisingDataBuilder::replaceData(
    adv_data_type_t advDataType,
    mbed::Span<const uint8_t> fieldData
) {
    uint8_t* field = findField(advDataType);

    if (field) {
        return replaceField(advDataType, fieldData, field);
    } else {
        return BLE_ERROR_NOT_FOUND;
    }
}

ble_error_t AdvertisingDataBuilder::appendData(
    adv_data_type_t advDataType,
    mbed::Span<const uint8_t> fieldData
) {
    uint8_t* field = findField(advDataType);

    if (field) {
        return appendToField(fieldData, field);
    } else {
        return BLE_ERROR_NOT_FOUND;
    }
}

ble_error_t AdvertisingDataBuilder::removeData(
    adv_data_type_t advDataType
) {
    uint8_t* field = findField(advDataType);

    if (field) {
        return removeField(field);
    } else {
        return BLE_ERROR_NOT_FOUND;
    }
}

ble_error_t AdvertisingDataBuilder::addOrReplaceData(
    adv_data_type_t advDataType,
    mbed::Span<const uint8_t> fieldData
) {
    uint8_t* field = findField(advDataType);

    if (field) {
        return replaceField(advDataType, fieldData, field);
    } else {
        return addField(advDataType, fieldData);
    }
}

ble_error_t AdvertisingDataBuilder::addOrAppendData(
    adv_data_type_t advDataType,
    mbed::Span<const uint8_t> fieldData
) {
    uint8_t* field = findField(advDataType);

    if (field) {
        return appendToField(fieldData, field);
    } else {
        return addField(advDataType, fieldData);
    }
}

void AdvertisingDataBuilder::clear() {
    memset(_buffer.data(), 0, _buffer.size());
    _payload_length = 0;
}

ble_error_t AdvertisingDataBuilder::setAppearance(
    adv_data_appearance_t appearance
) {
    uint8_t appearence_byte = appearance.value();
    mbed::Span<const uint8_t> appearance_span((const uint8_t*) &appearence_byte, 2);
    return addOrReplaceData(adv_data_type_t::APPEARANCE, appearance_span);
}

ble_error_t AdvertisingDataBuilder::setFlags(
    adv_data_flags_t flags
) {
    uint8_t flags_byte = flags.value();
    mbed::Span<const uint8_t> flags_span((const uint8_t*) &flags_byte, 1);
    return addOrReplaceData(adv_data_type_t::FLAGS, flags_span);
}

ble_error_t AdvertisingDataBuilder::setTxPowerAdvertised(
    advertising_power_t txPower
) {
    mbed::Span<const uint8_t> power_span((const uint8_t*) &txPower, 1);
    return addOrReplaceData(adv_data_type_t::TX_POWER_LEVEL, power_span);
}

ble_error_t AdvertisingDataBuilder::setName(
    const char* name,
    bool complete
) {
    mbed::Span<const uint8_t> power_span((const uint8_t*)name, strlen(name));

    if (complete) {
        return addOrReplaceData(adv_data_type_t::COMPLETE_LOCAL_NAME, power_span);
    } else {
        return addOrReplaceData(adv_data_type_t::SHORTENED_LOCAL_NAME, power_span);
    }
}

ble_error_t AdvertisingDataBuilder::setManufacturerSpecificData(
    mbed::Span<const uint8_t> data
) {
    if (data.size() < 2) {
        return BLE_ERROR_INVALID_PARAM;
    }

    return addOrReplaceData(adv_data_type_t::MANUFACTURER_SPECIFIC_DATA, data);
}

ble_error_t AdvertisingDataBuilder::setAdvertisingInterval(
    adv_interval_t interval
) {
    if (interval.value() > 0xFFFF) {
        return BLE_ERROR_INVALID_PARAM;
    }

    return addOrReplaceData(
        adv_data_type_t::ADVERTISING_INTERVAL,
        mbed::make_Span((const uint8_t*)interval.storage(), 2)
    );
}

ble_error_t AdvertisingDataBuilder::setConnectionIntervalPreference(
    conn_interval_t min,
    conn_interval_t max
) {
    uint32_t interval =  max.value();
    interval = interval << 16;
    interval |= min.value();
    return addOrReplaceData(
        adv_data_type_t::SLAVE_CONNECTION_INTERVAL_RANGE,
        mbed::make_Span((const uint8_t*)&interval, 4)
    );
}

ble_error_t AdvertisingDataBuilder::setServiceData(
    UUID service,
    mbed::Span<const uint8_t> data
) {
    if (service.getLen() + data.size() > 0xFE) {
        return BLE_ERROR_INVALID_PARAM;
    }

    adv_data_type_t short_type = adv_data_type_t::SERVICE_DATA_16BIT_ID;
    adv_data_type_t long_type = adv_data_type_t::SERVICE_DATA_128BIT_ID;

    size_t total_size = data.size() + service.getLen() + 2;
    size_t old_size = getFieldSize(
        (service.shortOrLong() == UUID::UUID_TYPE_SHORT) ? short_type : long_type
    );

    /* if we can't fit the new data do not proceed */
    if (total_size > data.size() - (_payload_length - old_size)) {
        return BLE_ERROR_BUFFER_OVERFLOW;
    }

    /* this will insert only the UUID (and remove old data) */
    ble_error_t status = setUUIDData(
        mbed::make_Span(&service, 1),
        short_type,
        long_type
    );

    if (status != BLE_ERROR_NONE) {
        /* we already checked for size so this must not happen */
        return BLE_ERROR_INTERNAL_STACK_FAILURE;
    }

    status = appendData(
        (service.shortOrLong() == UUID::UUID_TYPE_SHORT) ? short_type : long_type,
        data
    );

    if (status != BLE_ERROR_NONE) {
        return BLE_ERROR_INTERNAL_STACK_FAILURE;
    }

    return BLE_ERROR_NONE;
}

ble_error_t AdvertisingDataBuilder::setLocalServiceList(
    mbed::Span<const UUID> data,
    bool complete
) {
    adv_data_type_t short_type = complete ?
        adv_data_type_t::COMPLETE_LIST_16BIT_SERVICE_IDS :
        adv_data_type_t::INCOMPLETE_LIST_16BIT_SERVICE_IDS;

    adv_data_type_t long_type = complete ?
        adv_data_type_t::COMPLETE_LIST_128BIT_SERVICE_IDS :
        adv_data_type_t::INCOMPLETE_LIST_128BIT_SERVICE_IDS;

    return setUUIDData(data, short_type, long_type);
}

ble_error_t AdvertisingDataBuilder::setRequestedServiceList(
    mbed::Span<const UUID> data
) {
    adv_data_type_t short_type = adv_data_type_t::LIST_16BIT_SOLICITATION_IDS;
    adv_data_type_t long_type = adv_data_type_t::LIST_128BIT_SOLICITATION_IDS;

    return setUUIDData(data, short_type, long_type);
}

ble_error_t AdvertisingDataBuilder::getData(
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

uint8_t* AdvertisingDataBuilder::findField(adv_data_type_t type)
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

uint8_t AdvertisingDataBuilder::getFieldSize(adv_data_type_t type) {
    uint8_t *field = findField(type);
    if (field) {
        return field[0] + 1;
    } else {
        return 0;
    }
}

ble_error_t AdvertisingDataBuilder::addField(
    adv_data_type_t advDataType,
    mbed::Span<const uint8_t> fieldData
) {
    if (fieldData.size() > 0xFE) {
        return BLE_ERROR_INVALID_PARAM;
    }

    /* Make sure we don't exceed the buffer size */
    if (_payload_length + fieldData.size() + 2 > _buffer.size()) {
        return BLE_ERROR_BUFFER_OVERFLOW;
    }

    /* Field length (includes field ID byte) */
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

ble_error_t AdvertisingDataBuilder::appendToField(
    mbed::Span<const uint8_t> fieldData,
    uint8_t* field
) {
    if (fieldData.size() + field[0] > 0xFF /* field[0] already includes the type byte */) {
        return BLE_ERROR_INVALID_PARAM;
    }

    /* Check if data fits */
    if ((_payload_length + fieldData.size()) <= _buffer.size()) {
        uint8_t old_data_length = field[0];

        /* get the size of bytes in the payload after the field */
        size_t remainder_size = _payload_length -
                                (field - _buffer.data()) - /* length of all data before the field */
                                (old_data_length + 1) /* length of the old field */;

        /* move data after the field to fit new data */
        if (remainder_size) {
            memmove(
                field + old_data_length + 1 + fieldData.size(),
                field + old_data_length + 1,
                remainder_size
            );
        }

        /* append new data */
        memcpy(field + old_data_length + 1, fieldData.data(), fieldData.size());

        /* Increment lengths */
        field[0] += fieldData.size();
        _payload_length += fieldData.size();

        return BLE_ERROR_NONE;
    } else {
        return BLE_ERROR_BUFFER_OVERFLOW;
    }
}

ble_error_t AdvertisingDataBuilder::replaceField(
    adv_data_type_t advDataType,
    mbed::Span<const uint8_t> fieldData,
    uint8_t* field
) {
    if (fieldData.size() > 0xFE) {
        return BLE_ERROR_INVALID_PARAM;
    }

    uint8_t old_data_length = field[0] - 1;

    /* New data has same length, do in-order replacement */
    if (fieldData.size() == old_data_length) {
        memcpy(field + 2, fieldData.data(), old_data_length);

        return BLE_ERROR_NONE;
    } else {
        /* Check if data fits */
        if ((_payload_length - old_data_length + fieldData.size()) <= _buffer.size()) {
            removeField(field);

            /* Add new field */
            return addField(advDataType, fieldData);
        } else {
            return BLE_ERROR_BUFFER_OVERFLOW;
        }
    }
}

ble_error_t AdvertisingDataBuilder::removeField(uint8_t* field)
{
    /* stored length + the byte containing length */
    uint8_t old_field_length = field[0] + 1;

    memmove(field, field + old_field_length, old_field_length);

    _payload_length -= old_field_length;

    return BLE_ERROR_NONE;
}

ble_error_t AdvertisingDataBuilder::setUUIDData(
    mbed::Span<const UUID> data,
    adv_data_type_t shortType,
    adv_data_type_t longType
) {
    ble_error_t status = BLE_ERROR_NONE;

    /* first count all the bytes we need to store all the UUIDs */
    size_t size_long = 0;
    size_t size_short = 0;

    for (size_t i = 0, end = data.size(); i < end; ++i) {
        if (data[i].shortOrLong() == UUID::UUID_TYPE_SHORT) {
            size_short++;
        } else {
            size_long++;
        }
    }

    if ((size_long * 8 > 0xFE) || (size_short * 2 > 0xFE)) {
        return BLE_ERROR_INVALID_PARAM;
    }

    /* calculate total size including headers for types */
    size_t total_size = size_long + (!!size_long) * 2 +
                        size_short + (!!size_short) * 2;

    /* count all the bytes of existing data */
    size_t old_size = getFieldSize(shortType) + getFieldSize(longType);

    /* if we can't fit the new data do not proceed */
    if (total_size > data.size() - (_payload_length - old_size)) {
        return BLE_ERROR_BUFFER_OVERFLOW;
    }

    /* otherwise wipe old data */
    removeData(shortType);
    removeData(longType);

    /* and insert individual UUIDs into appropriate fields */
    for (size_t i = 0, end = data.size(); i < end; ++i) {
        adv_data_type_t field_type = (data[i].shortOrLong() == UUID::UUID_TYPE_SHORT) ? shortType : longType;

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

} // end of namespace ble