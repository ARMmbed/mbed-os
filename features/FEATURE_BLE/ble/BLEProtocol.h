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

#ifndef __BLE_PROTOCOL_H__
#define __BLE_PROTOCOL_H__

#include <stddef.h>
#include <stdint.h>
#include <algorithm>

/**
 * A common namespace for types and constants used everywhere in BLE API.
 */
namespace BLEProtocol {
    /**<
     * A simple container for the enumeration of address-types for Protocol addresses.
     *
     * Adding a struct to encapsulate the contained enumeration prevents
     * polluting the BLEProtocol namespace with the enumerated values. It also
     * allows type-aliases for the enumeration while retaining the enumerated
     * values. i.e. doing:
     *       typedef AddressType AliasedType;
     *
     * would allow the use of AliasedType::PUBLIC in code.
     */
    struct AddressType {
        /**< Address-types for Protocol addresses. */
        enum Type {
            PUBLIC = 0,
            RANDOM_STATIC,
            RANDOM_PRIVATE_RESOLVABLE,
            RANDOM_PRIVATE_NON_RESOLVABLE
        };
    };
    typedef AddressType::Type AddressType_t;  /**< Alias for AddressType::Type */

    static const size_t ADDR_LEN = 6;         /**< Length (in octets) of the BLE MAC address. */
    typedef uint8_t AddressBytes_t[ADDR_LEN]; /**< 48-bit address, in LSB format. */

    /**
     * BLE address. It contains an address-type (AddressType_t) and bytes (AddressBytes_t).
     */
    struct Address_t {
        AddressType_t  type;    /**< The type of the BLE address. */
        AddressBytes_t address; /**< The BLE address. */

        /**
         * Construct an Address_t object with the supplied type and address.
         *
         * @param[in] typeIn
         *              The BLE address type.
         * @param[in] addressIn
         *              The BLE address.
         */
        Address_t(AddressType_t typeIn, const AddressBytes_t& addressIn) : type(typeIn) {
            std::copy(addressIn, addressIn + ADDR_LEN, address);
        }

        /**
         * Empty constructor.
         */
        Address_t() : type(), address() {
        }
    };
};

#endif /* __BLE_PROTOCOL_H__ */
