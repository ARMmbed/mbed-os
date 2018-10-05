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

#ifndef MBED_BLE_PROTOCOL_H__
#define MBED_BLE_PROTOCOL_H__

#include <stddef.h>
#include <stdint.h>
#include <algorithm>

/**
 * @addtogroup ble
 * @{
 * @addtogroup common
 * @{
 */

/**
 * Common namespace for types and constants used everywhere in BLE API.
 */
namespace BLEProtocol {

    /**
     * Container for the enumeration of BLE address types.
     *
     * @note Adding a struct to encapsulate the contained enumeration prevents
     * polluting the BLEProtocol namespace with the enumerated values. It also
     * allows type-aliases for the enumeration while retaining the enumerated
     * values. i.e. doing:
     *
     * @code
     *       typedef AddressType AliasedType;
     * @endcode
     *
     * would allow the use of AliasedType::PUBLIC in code.
     *
     * @note see Bluetooth Standard version 4.2 [Vol 6, Part B] section 1.3 .
     */
    struct AddressType {
        /**
         * Address-types for Protocol addresses.
         */
        enum Type {
            /**
             * Public device address.
             */
            PUBLIC = 0,

            /**
             * Random static device address.
             *
             * @deprecated This enumeration value is not relevant anymore.
             * Advertising reporting and the connection procedure should rely
             * on RANDOM instead. Use Gap::getRandomAddressType to retrieve the
             * type of the random address.
             */
            RANDOM_STATIC,

            /**
             * Private resolvable device address.
             *
             * @deprecated This enumeration value is not relevant anymore.
             * Advertising reporting and the connection procedure should rely
             * on RANDOM instead. Use Gap::getRandomAddressType to retrieve the
             * type of the random address.
             */
            RANDOM_PRIVATE_RESOLVABLE,

            /**
             * Private non-resolvable device address.
             *
             * @deprecated This enumeration value is not relevant anymore.
             * Advertising reporting and the connection procedure should rely
             * on RANDOM instead. Use Gap::getRandomAddressType to retrieve the
             * type of the random address.
             */
            RANDOM_PRIVATE_NON_RESOLVABLE
        };
    };

    /**
     * Alias for AddressType::Type
     */
    typedef AddressType::Type AddressType_t;

    /**
     * Length (in octets) of the BLE MAC address.
     */
    static const size_t ADDR_LEN = 6;

    /**
     * 48-bit address, in LSB format.
     */
    typedef uint8_t AddressBytes_t[ADDR_LEN];

    /**
     * BLE address representation.
     *
     * It contains an address-type (::AddressType_t) and the address value
     * (::AddressBytes_t).
     */
    struct Address_t {
        /**
         * Construct an Address_t object with the supplied type and address.
         *
         * @param[in] typeIn The BLE address type.
         * @param[in] addressIn The BLE address.
         *
         * @post type is equal to typeIn and address is equal to the content
         * present in addressIn.
         */
        Address_t(AddressType_t typeIn, const AddressBytes_t &addressIn) :
            type(typeIn) {
            std::copy(addressIn, addressIn + ADDR_LEN, address);
        }

        /**
         * Empty constructor.
         *
         * @note The address constructed with the empty constructor is not
         * valid.
         *
         * @post type is equal to PUBLIC and the address value is equal to
         * 00:00:00:00:00:00
         */
        Address_t(void) : type(), address() { }

        /**
         * Type of the BLE device address.
         */
        AddressType_t  type;

        /**
         * Value of the device address.
         */
        AddressBytes_t address;
    };
};

/**
 * @}
 * @}
 */

#endif /* MBED_BLE_PROTOCOL_H__ */
