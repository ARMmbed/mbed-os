/* mbed Microcontroller Library
 * Copyright (c) 2017-2017 ARM Limited
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

#ifndef BLE_PAL_SIMPLEATTSERVERMESSAGE_H_
#define BLE_PAL_SIMPLEATTSERVERMESSAGE_H_

#include "AttServerMessage.h"

namespace ble {
namespace pal {

/**
 * Simple implementation of ble::pal::AttFindInformationResponse.
 * It should fit for any vendor stack exposing a proper ATT interface.
 */
struct SimpleAttFindInformationResponse : public AttFindInformationResponse {
    /**
     * Format of the UUID in the response.
     */
    enum Format {
        FORMAT_16_BIT_UUID = 0x01,//!< FORMAT_16_BIT_UUID
        FORMAT_128_BIT_UUID = 0x02//!< FORMAT_128_BIT_UUID
    };

    /**
     * Construct an AttFindInformationResponse from format and an array of bytes
     * containing the information data.
     * @param format The format of the information data.
     * @param information_data The information data whose format is determined
     * by the Format field
     */
    SimpleAttFindInformationResponse(
        Format format, ArrayView<const uint8_t> information_data
    ) : AttFindInformationResponse(),
        _format(format), _information_data(information_data),
        _item_size(information_data_item_size()) {
    }

    /**
     * @see ble::pal::AttFindInformationResponse::size
     */
    virtual size_t size() const {
        return _information_data.size() / _item_size;
    }

    /**
     * @see ble::pal::AttFindInformationResponse::operator[]
     */
    virtual information_data_t operator[](size_t index) const  {
        const uint8_t* item = &_information_data[index * _item_size];

        information_data_t result;
        memcpy(&result.handle, item, sizeof(result.handle));

        if (_format == FORMAT_16_BIT_UUID) {
            uint16_t short_uuid = 0;
            memcpy(&short_uuid, item + 2, sizeof(short_uuid));
            result.uuid = UUID(short_uuid);
        } else {
            result.uuid = UUID(item + 2, UUID::LSB);
        }

        return result;
    }

private:
    size_t information_data_item_size() const {
        return sizeof(attribute_handle_t) + ((_format == FORMAT_16_BIT_UUID) ? 2 : 16);
    }

    const Format _format;
    const ArrayView<const uint8_t> _information_data;
    const size_t _item_size;
};


/**
 * Simple implementation of ble::pal::AttFindByTypeValueResponse.
 * It should fit for any vendor stack exposing a proper ATT interface.
 */
struct SimpleAttFindByTypeValueResponse : public AttFindByTypeValueResponse {
    /**
     * Construct a AttFindByTypeValueResponse from a raw array containing the
     * Handle Informations.
     * @param handles raw array containing one or more Handle Informations.
     */
    SimpleAttFindByTypeValueResponse(ArrayView<const uint8_t> handles) :
        AttFindByTypeValueResponse(), _handles(handles) {
    }

    /**
     * @see ble::pal::AttFindByTypeValueResponse::size
     */
    virtual std::size_t size() const {
        return _handles.size() / item_size;
    }

    /**
     * @see ble::pal::AttFindByTypeValueResponse::operator[]
     */
    virtual attribute_handle_range_t operator[](size_t index) const {
        attribute_handle_range_t result;
        const uint8_t* item = &_handles[index * item_size];
        memcpy(&result.begin, item, sizeof(result.begin));
        memcpy(&result.end, item + sizeof(result.begin), sizeof(result.end));
        return result;
    }

private:
    static const size_t item_size = 4;
    const ArrayView<const uint8_t> _handles;
};


/**
 * Simple implementation of ble::pal::AttReadByTypeResponse.
 * It should fit for any vendor stack exposing a proper ATT interface.
 */
struct SimpleAttReadByTypeResponse : public AttReadByTypeResponse {
    /**
     * Construct an AttReadByTypeResponse from the size of each attribute
     * handle-value pair and a list of attribute data.
     * @param element_size The size of each attribute-handle pair.
     * @param attribute_data Raw bytes array containing the list of attribute
     * data.
     */
    SimpleAttReadByTypeResponse(
        uint8_t element_size, ArrayView<const uint8_t> attribute_data
    ) : AttReadByTypeResponse(),
        _attribute_data(attribute_data), _element_size(element_size) {
    }

    /**
     * @see ble::pal::AttReadByTypeResponse::size
     */
    virtual size_t size() const {
        return _attribute_data.size() / _element_size;
    }

    /**
     * @see ble::pal::AttReadByTypeResponse::operator[]
     */
    virtual attribute_data_t operator[](size_t index) const {
        const uint8_t* item = &_attribute_data[index * _element_size];
        uint16_t handle;
        memcpy(&handle, item, sizeof(handle));

        attribute_data_t result = {
            handle,
            ArrayView<const uint8_t>(
                item + sizeof(handle),
                _element_size - sizeof(handle)
            )
        };

        return result;
    }

private:
    ArrayView<const uint8_t> _attribute_data;
    uint8_t _element_size;
};


/**
 * Simple implementation of ble::pal::AttReadByGroupTypeResponse.
 * It should fit for any vendor stack exposing a proper ATT interface.
 */
struct SimpleAttReadByGroupTypeResponse : public AttReadByGroupTypeResponse {
    /**
     * Construct an instance of AttReadByGroupTypeResponse from the size of each
     * attribute data and a byte array containing the list of attribute data.
     * @param element_size The size of each Attribute Data
     * @param attribute_data Byte array containing the list of Attribute Data.
     */
    SimpleAttReadByGroupTypeResponse(
        uint8_t element_size, ArrayView<const uint8_t> attribute_data
    ) : AttReadByGroupTypeResponse(),
        _attribute_data(attribute_data), _element_size(element_size) {
    }

    /**
     * @see ble::pal::AttReadByGroupTypeResponse::size
     */
    virtual size_t size() const {
        return _attribute_data.size() / _element_size;
    }

    /**
     * @see ble::pal::AttReadByGroupTypeResponse::operator[]
     */
    virtual attribute_data_t operator[](size_t index) const {
        const uint8_t* item = &_attribute_data[index * _element_size];
        uint16_t begin;
        uint16_t end;

        memcpy(&begin, item, sizeof(begin));
        memcpy(&end, item + sizeof(begin), sizeof(end));

        attribute_data_t result = {
            { begin, end },
            ArrayView<const uint8_t>(
                item + sizeof(begin) + sizeof(end),
                _element_size - (sizeof(begin) + sizeof(end))
            )
        };

        return result;
    }

private:
    ArrayView<const uint8_t> _attribute_data;
    uint8_t _element_size;
};

}  // namespace pal
}  // namespace ble

#endif /* BLE_PAL_SIMPLEATTSERVERMESSAGE_H_ */
