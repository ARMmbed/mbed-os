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

/**
 * @addtogroup ble
 * @{
 * @addtogroup common
 * @{
 */

namespace ble {

/**
 * Opaque reference to a connection.
 *
 * Internally a connection handle is an unsigned integer capable of holding a
 * pointer.
 *
 * The real type (either a pointer to an object or an integer) is opaque for
 * users and platform dependent.
 */
typedef uintptr_t connection_handle_t;

/**
 * Reference to an attribute in a GATT database.
 */
typedef uint16_t attribute_handle_t;


 /**
  * Inclusive range of GATT attributes handles.
  *
  * @note Instances can be constructed with the help of the factory function
  * attribute_handle_range().
  */
struct attribute_handle_range_t {
    /**
     * Begining of the range.
     */
    attribute_handle_t begin;

    /**
     * End of the range.
     */
    attribute_handle_t end;

    /**
     * Equal operator for attribute_handle_range_t.
     *
     * @param[in] lhs Left hand side of the expression.
     * @param[in] rhs Right hand side of the expression.
     *
     * @return true if lhs is equal to rhs and false otherwise.
     */
    friend bool operator==(
        const attribute_handle_range_t &lhs, const attribute_handle_range_t &rhs
    ) {
        return (lhs.begin == rhs.begin) && (lhs.end == rhs.end);
    }

    /**
     * Not equal operator for attribute_handle_range_t.
     *
     * @param[in] lhs Left hand side of the expression.
     * @param[in] rhs Right hand side of the expression.
     *
     * @return true if lhs is not equal to rhs and false otherwise.
     */
    friend bool operator!=(
        const attribute_handle_range_t &lhs, const attribute_handle_range_t &rhs
    ) {
        return !(lhs == rhs);
    }
};


/**
 * Construct an attribute_handle_range_t from its first and last attribute handle.
 *
 * @param begin Handle at the begining of the range.
 * @param end Handle at the end of the range.
 *
 * @return An instance of attribute_handle_range_t where
 * attribute_handle_range_t::begin is equal to begin and
 * attribute_handle_range_t::end is equal to end.
 *
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

/**
 * @}
 * @}
 */

#endif /* BLE_TYPES_H_ */
