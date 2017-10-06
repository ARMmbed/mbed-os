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

#ifndef BLE_TYPES_H_
#define BLE_TYPES_H_

#include <stddef.h>
#include <stdint.h>

namespace ble {

/**
 * A connection handle is an unsigned integer capable of holding a pointer.
 * The real type (either a pointer to an object or an integer) is opaque and
 * platform dependent.
 */
typedef uintptr_t connection_handle_t;

/**
 * Model an attribute handle in a GATT database.
 */
typedef uint16_t attribute_handle_t;


 /**
  * Model an inclusive range of GATT attributes handles.
  */
struct attribute_handle_range_t {
    attribute_handle_t begin;
    attribute_handle_t end;

    friend bool operator==(
        const attribute_handle_range_t& lhs, const attribute_handle_range_t& rhs
    ) {
        return (lhs.begin == rhs.begin) && (lhs.end == rhs.end);
    }

    friend bool operator!=(
        const attribute_handle_range_t& lhs, const attribute_handle_range_t& rhs
    ) {
        return !(lhs == rhs);
    }
};


/**
 * Construct an attribute_handle_range_t from its start and end handle.
 * @note This function is defined instead of a constructor to keep "POD-ness"
 * of attribute_handle_range_t.
 */
static inline attribute_handle_range_t attribute_handle_range(
    attribute_handle_t begin,
    attribute_handle_t end
) {
    attribute_handle_range_t result = {
        begin,
        end
    };
    return result;
}

} // namespace ble

#endif /* BLE_TYPES_H_ */
