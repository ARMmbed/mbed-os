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

#ifndef MBED_UUID_H__
#define MBED_UUID_H__

#include <stddef.h>
#include <stdint.h>
#include <string.h>
#include <algorithm>

#include "blecommon.h"

/**
 * @file
 * @addtogroup ble
 * @{
 * @addtogroup common
 * @{
 */

/**
 * Convert a character containing an hexadecimal digit into an unsigned integer.
 *
 * @param[in] c Hexadecimal digit in a character representation.
 *
 * @return The corresponding value as unsigned integer.
 */
static uint8_t char2int(char c)
{
    if ((c >= '0') && (c <= '9')) {
        return c - '0';
    } else if ((c >= 'a') && (c <= 'f')) {
        return c - 'a' + 10;
    } else if ((c >= 'A') && (c <= 'F')) {
        return c - 'A' + 10;
    } else {
        return 0;
    }
}

/**
 * Representation of a Universally Unique Identifier (UUID).
 *
 * UUIDs are 128-bit wide numbers used to identify data type and elements in
 * many layers of the Bluetooth specification.
 *
 * Two representations of UUIDS exist:
 *   - 16-bit UUIDs: Shortened representation of the 128 bit UUID
 *     0000xxxx-0000-1000-8000-00805F9B34FB where xxxx is the 16 bit UUID.
 *     Values of those UUIDs are defined by the Bluetooth body. The short
 *     representation saves bandwidth during protocol transactions.
 *   - 128-bit UUIDs: Complete representation of a UUID. They are commonly
 *     used for user defined UUID.
 *
 * This class acts as an adapter over these two kinds of UUIDs to allow
 * indiscriminate use of both forms in Mbed BLE APIs.
 *
 * @note 32-bit UUID representation is not supported currently.
 */
class UUID {
public:

    /**
     * Enumeration of the types of UUIDs.
     */
    enum UUID_Type_t {
        /**
         * 16-bit wide UUID representation.
         */
        UUID_TYPE_SHORT = 0,

        /**
         * 128-bit wide UUID representation.
         */
        UUID_TYPE_LONG  = 1
    };

    /**
     * Enumeration of byte ordering.
     *
     * It is used to construct 128-byte UUIDs.
     */
    typedef enum {
        /**
         * Most significant byte first (at the smallest address).
         */
        MSB,

        /**
         * Least significant byte first (at the smallest address).
         */
        LSB
    } ByteOrder_t;

    /**
     * Type for a 16-bit UUID.
     */
    typedef uint16_t ShortUUIDBytes_t;

    /**
     * Length in bytes of a long UUID.
     */
    static const unsigned LENGTH_OF_LONG_UUID = 16;

    /**
     * Type for a 128-bit UUID.
     */
    typedef uint8_t LongUUIDBytes_t[LENGTH_OF_LONG_UUID];

    /**
     * Maximum length for the string representation of a UUID excluding the null
     * terminator.
     *
     * The string is composed of two characters per byte plus four '-'
     * characters.
     */
    static const unsigned MAX_UUID_STRING_LENGTH = LENGTH_OF_LONG_UUID * 2 + 4;

public:

    /**
     * Construct a 128-bit UUID from a string.
     *
     * @param[in] stringUUID Human readable representation of the UUID following
     * the format XXXXXXXX-XXXX-XXXX-XXXX-XXXXXXXXXXXX.
     *
     * @note Upper and lower case are supported.
     * @note Hyphens are optional. The string must include at most four hyphens.
     *
     * @note Internally, the UUID is stored in the little endian order as a
     * 16-byte array.
     */
    UUID(const char* stringUUID) :
        type(UUID_TYPE_LONG),
        baseUUID(),
        shortUUID(0)
    {
        bool nibble = false;
        uint8_t byte = 0;
        size_t baseIndex = 0;
        uint8_t tempUUID[LENGTH_OF_LONG_UUID];

        /*
         * Iterate through string; abort if NULL is encountered prematurely.
         * Ignore up to four hyphens.
         */
        for (size_t index = 0; (index < MAX_UUID_STRING_LENGTH) && (baseIndex < LENGTH_OF_LONG_UUID); index++) {
            if (stringUUID[index] == '\0') {
                /* Error abort */
                break;
            } else if (stringUUID[index] == '-') {
                /* Ignore hyphen */
                continue;
            } else if (nibble) {
                /* Got second nibble */
                byte |= char2int(stringUUID[index]);
                nibble = false;

                /* Store copy */
                tempUUID[baseIndex++] = byte;
            } else {
                /* Got first nibble */
                byte = char2int(stringUUID[index]) << 4;
                nibble = true;
            }
        }

        /* Populate internal variables if string was successfully parsed */
        if (baseIndex == LENGTH_OF_LONG_UUID) {
            setupLong(tempUUID, UUID::MSB);
        } else {
            const uint8_t sig[] = { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x10, 0x00,
                                    0x80, 0x00, 0x00, 0x80, 0x5F, 0x9B, 0x34, 0xFB };
            setupLong(sig, UUID::MSB);
        }
    }

    /**
     * Construct a new UUID from a 128-bit representation.
     *
     * @param[in] longUUID The 128-bit (16-byte) of the UUID value.
     * @param[in] order Bytes order of @p longUUID.
     */
    UUID(const LongUUIDBytes_t longUUID, ByteOrder_t order = UUID::MSB) : type(UUID_TYPE_LONG), baseUUID(), shortUUID(0) {
        setupLong(longUUID, order);
    }

    /**
     * Creates a new 16-bit UUID.
     *
     * The Bluetooth standard body defines 16-bit wide UUIDs. They are the
     * shortened version of the UUID 0000xxxx-0000-1000-8000-00805F9B34FB, where
     * xxxx is the value of the 16-bit UUID.
     *
     * @attention 16-bit UUIDs are not used in user defined data type or
     * user defined element ID.
     *
     * @param[in] _shortUUID 16-bit part of the standard UUID.
     *              The short UUID value.
     *
     * @note User defined UUIDs are commonly named vendor-specific UUIDs across
     * the Bluetooth literature.
     */
    UUID(ShortUUIDBytes_t _shortUUID) :
        type(UUID_TYPE_SHORT),
        baseUUID(),
        shortUUID(_shortUUID) {
    }

    /**
     * UUID copy constructor.
     *
     * @param[in] source The UUID to copy.
     */
    UUID(const UUID &source)
    {
        type      = source.type;
        shortUUID = source.shortUUID;
        memcpy(baseUUID, source.baseUUID, LENGTH_OF_LONG_UUID);
    }

    /**
     * Default constructor.
     *
     * Construct an invalid UUID.
     *
     * @post shortOrLong() returns the value UUID_TYPE_SHORT.
     * @post getShortUUID() returns the value BLE_UUID_UNKNOWN.
     */
    UUID(void) :
        type(UUID_TYPE_SHORT),
        shortUUID(BLE_UUID_UNKNOWN) {
    }

    /**
     * Replace existing value with a 128-bit UUID.
     *
     * @param[in] longUUID New 16-byte wide UUID value.
     * @param[in] order Byte ordering of @p longUUID.
     */
    void setupLong(const LongUUIDBytes_t longUUID, ByteOrder_t order = UUID::MSB)
    {
        type = UUID_TYPE_LONG;
        if (order == UUID::MSB) {
            /*
             * Switch endian. Input is big-endian, internal representation
             * is little endian.
             */
            std::reverse_copy(longUUID, longUUID + LENGTH_OF_LONG_UUID, baseUUID);
        } else {
            std::copy(longUUID, longUUID + LENGTH_OF_LONG_UUID, baseUUID);
        }
        shortUUID = (uint16_t)((baseUUID[13] << 8) | (baseUUID[12]));
    }

public:
    /**
     * Return the internal type of the UUID.
     *
     * @return UUID_TYPE_SHORT if the UUID is 16-bit wide.
     * @return UUID_TYPE_LONG if the UUID is 128-bit wide.
     */
    UUID_Type_t shortOrLong(void) const
    {
        return type;
    }

    /**
     * Get a pointer to the UUID value based on the current UUID type.
     *
     * @return A pointer to an uint16_t object if the UUID is 16 bits long.
     * @return A pointer to an array of 16 bytes if the UUID is 128 bits long.
     */
    const uint8_t *getBaseUUID(void) const
    {
        if (type == UUID_TYPE_SHORT) {
            return (const uint8_t*)&shortUUID;
        } else {
            return baseUUID;
        }
    }

    /**
     * Get the uint16_t value of the UUID.
     *
     * @attention This function is not used on long UUIDs.
     *
     * @return The value of the shortened UUID.
     */
    ShortUUIDBytes_t getShortUUID(void) const
    {
        return shortUUID;
    }

    /**
     * Get the length (in bytes) of the internal UUID representation.
     *
     * @return sizeof(ShortUUIDBytes_t) if the UUID type is UUID_TYPE_SHORT.
     * @return LENGTH_OF_LONG_UUID if the UUID type is UUID_TYPE_LONG.
     */
    uint8_t getLen(void) const
    {
        return ((type == UUID_TYPE_SHORT) ?
            sizeof(ShortUUIDBytes_t) :
            LENGTH_OF_LONG_UUID);
    }

    /**
     * Equal to operator between UUIDs.
     *
     * @param[in] other The UUID to compare to this.
     *
     * @return true if both UUIDs are equal and false otherwise.
     */
    bool operator== (const UUID &other) const
    {
        if ((this->type == UUID_TYPE_SHORT) && (other.type == UUID_TYPE_SHORT) &&
            (this->shortUUID == other.shortUUID)) {
            return true;
        }

        if ((this->type == UUID_TYPE_LONG) && (other.type == UUID_TYPE_LONG) &&
            (memcmp(this->baseUUID, other.baseUUID, LENGTH_OF_LONG_UUID) == 0)) {
            return true;
        }

        return false;
    }

    /**
     * Not equal to operator.
     *
     * @param[in] other The UUID compared to this.
     *
     * @return true if both UUIDs are not equal and false otherwise.
     */
    bool operator!= (const UUID &other) const
    {
        return !(*this == other);
    }

private:
    /**
     * Representation type of the UUID.
     */
    UUID_Type_t type;

    /**
     * Container of UUID value if the UUID type is equal to UUID_TYPE_LONG.
     */
    LongUUIDBytes_t  baseUUID;

    /**
     * Container of UUID value if the UUID type is equal to UUID_TYPE_SHORT.
     */
    ShortUUIDBytes_t shortUUID;
};

/**
 * @}
 * @}
 */

#endif // ifndef MBED_UUID_H__
