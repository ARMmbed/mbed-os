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

#ifndef BLE_GAP_ADVERTISINGDATAPARSER_H
#define BLE_GAP_ADVERTISINGDATAPARSER_H

#include <stdint.h>
#include "ble/gap/AdvertisingDataTypes.h"

namespace ble {

/**
 * @addtogroup ble
 * @{
 * @addtogroup gap
 * @{
 */

/**
 * Parse and iterate over advertising data
 */
class AdvertisingDataParser {

    enum {
        DATA_SIZE_INDEX = 0,
        TYPE_INDEX = 1,
        VALUE_INDEX = 2,
        TYPE_SIZE = 1,
        DATA_SIZE_SIZE = 1
    };

public:
    /**
     * Representation of an Advertising Data element.
     */
    struct element_t {
        adv_data_type_t type;
        mbed::Span<const uint8_t> value;
    };

    /**
     * Build a parser from an array of bytes.
     * @param data The data to parse.
     */
    AdvertisingDataParser(mbed::Span<const uint8_t> data) :
        data(data),
        position(0)
    {
    }

    /**
     * Return if there is advertising data element left to parse.
     */
    bool hasNext() const
    {
        if (position >= data.size()) {
            return false;
        }
        
        /* early termination of packet, no more meaningful octets */
        if (current_length() == 0) {
            return false;
        }

        if (position + current_length() >= data.size()) {
            return false;
        }

        return true;
    }

    /**
     * Return the next advertising data element.
     *
     * @note Calling this function if there is no next element is undefined
     * behavior.
     */
    element_t next()
    {
        element_t element = {
            (ble::adv_data_type_t::type) data[position + TYPE_INDEX],
            data.subspan(position + VALUE_INDEX, current_length() - (TYPE_SIZE))
        };

        position += (DATA_SIZE_SIZE + current_length());
        return element;
    }

    /**
     * Reset the parser.
     */
    void reset()
    {
        position = 0;
    }

private:
    uint8_t current_length() const {
        return data[position + DATA_SIZE_INDEX];
    }

    mbed::Span<const uint8_t> data;
    size_t position;
};

/**
 * @}
 * @}
 */

} // namespace ble

#endif //BLE_GAP_ADVERTISINGDATAPARSER_H
