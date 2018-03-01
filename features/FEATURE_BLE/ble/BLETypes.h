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
#include <string.h>
#include "ble/SafeEnum.h"

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
     * Beginning of the range.
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
 * @param[in] begin Handle at the beginning of the range.
 * @param[in] end Handle at the end of the range.
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

/**
 * Type that describes link's encryption state.
 */
struct link_encryption_t : SafeEnum<link_encryption_t, uint8_t> {
    /** struct scoped enum wrapped by the class */
    enum type {
        NOT_ENCRYPTED,          /**< The link is not secured. */
        ENCRYPTION_IN_PROGRESS, /**< Link security is being established. */
        ENCRYPTED,              /**< The link is secure. */
        ENCRYPTED_WITH_MITM     /**< The link is secure and authenticated. */
    };

    /**
     * Construct a new instance of link_encryption_t.
     */
    link_encryption_t(type value) : SafeEnum<link_encryption_t, uint8_t>(value) { }
};

/**
 * Type that describe a pairing failure.
 */
struct pairing_failure_t : SafeEnum<pairing_failure_t, uint8_t> {
    /** struct scoped enum wrapped by the class */
    enum type {
        PASSKEY_ENTRY_FAILED = 0x01,
        OOB_NOT_AVAILABLE = 0x02,
        AUTHENTICATION_REQUIREMENTS = 0x03,
        CONFIRM_VALUE_FAILED = 0x04,
        PAIRING_NOT_SUPPORTED = 0x05,
        ENCRYPTION_KEY_SIZE = 0x06,
        COMMAND_NOT_SUPPORTED = 0x07,
        UNSPECIFIED_REASON = 0x08,
        REPEATED_ATTEMPTS = 0x09,
        INVALID_PARAMETERS = 0x0A,
        DHKEY_CHECK_FAILED = 0x0B,
        NUMERIC_COMPARISON_FAILED = 0x0c,
        BR_EDR_PAIRING_IN_PROGRESS = 0x0D,
        CROSS_TRANSPORT_KEY_DERIVATION_OR_GENERATION_NOT_ALLOWED = 0x0E
    };

    /**
     * Construct a new instance of pairing_failure_t.
     */
    pairing_failure_t(type value) : SafeEnum<pairing_failure_t, uint8_t>(value) { }
};


/**
 * Type that describe the IO capability of a device; it is used during Pairing
 * Feature exchange.
 */
struct io_capability_t : SafeEnum<io_capability_t, uint8_t> {
    enum type {
        DISPLAY_ONLY = 0x00,
        DISPLAY_YES_NO = 0x01,
        KEYBOARD_ONLY = 0x02,
        NO_INPUT_NO_OUTPUT = 0x03,
        KEYBOARD_DISPLAY = 0x04
    };

    /**
     * Construct a new instance of io_capability_t.
     */
    io_capability_t(type value) : SafeEnum<io_capability_t, uint8_t>(value) { }
};

/**
 * Passkey stored as a number.
 */
typedef uint32_t passkey_num_t;

/**
 * Passkey stored as a string of digits.
 */
class PasskeyAscii {
public:
    static const uint8_t PASSKEY_LEN = 6;
    static const uint8_t NUMBER_OFFSET = '0';

    /**
     * Default to all zeroes
     */
    PasskeyAscii() {
        memset(ascii, NUMBER_OFFSET, PASSKEY_LEN);
    }

    /**
     * Initialize a data from a string.
     *
     * @param[in] passkey value of the data.
     */
    PasskeyAscii(const uint8_t* passkey) {
        if (passkey) {
            memcpy(ascii, passkey, PASSKEY_LEN);
        } else {
            memset(ascii, NUMBER_OFFSET, PASSKEY_LEN);
        }
    }

    /**
     * Initialize a data from a number.
     *
     * @param[in] passkey value of the data.
     */
    PasskeyAscii(passkey_num_t passkey) {
        for (int i = 5, m = 100000; i >= 0; --i, m /= 10) {
            uint32_t result = passkey / m;
            ascii[i] = NUMBER_OFFSET + result;
            passkey -= result * m;
        }
    }

    /**
     * Cast to number.
     */
    operator passkey_num_t() {
        return to_num(ascii);
    }

    /**
     * Convert ASCII string of digits into a number.
     * @param[in] ascii ASCII string of 6 digits stored as ASCII characters
     * @return Passkey as a number.
     */
    static uint32_t to_num(const uint8_t *ascii) {
        uint32_t passkey = 0;
        for (size_t i = 0, m = 1; i < PASSKEY_LEN; ++i, m *= 10) {
            passkey += (ascii[i] - NUMBER_OFFSET) * m;
        }
        return passkey;
    }

    /**
     * Return the pointer to the buffer holding the string.
     */
    uint8_t* value() {
        return ascii;
    }
private:
    uint8_t ascii[PASSKEY_LEN];
};

template <size_t array_size>
struct byte_array_t {
    /**
     * Default to all zeroes
     */
    byte_array_t() {
        memset(_value, 0x00, sizeof(_value));
    }

    /**
     * Initialize a data from an array of bytes.
     *
     * @param[in] input_value value of the data.
     */
    byte_array_t(const uint8_t *input_value) {
        memcpy(_value, input_value, sizeof(_value));
    }

    /**
     * Initialize a data from an buffer of bytes.
     *
     * @param[in] input_value pointer to buffer.
     * @param[in] size buffer size
     */
    byte_array_t(const uint8_t* input_value, size_t size) {
        memcpy(_value, input_value, size);
    }

    /**
     * Equal operator between two octet types.
     */
    friend bool operator==(const byte_array_t& lhs, const byte_array_t& rhs) {
        return memcmp(lhs._value, rhs._value, sizeof(lhs._value)) == 0;
    }

    /**
     * Non equal operator between two octet types.
     */
    friend bool operator!=(const byte_array_t& lhs, const byte_array_t& rhs) {
        return !(lhs == rhs);
    }

    /**
     * Subscript operator to access data content
     */
    uint8_t& operator[](uint8_t i) {
        return _value[i];
    }

    /**
     * Return the pointer to the buffer holding data.
     */
    const uint8_t* data() const {
        return _value;
    }

    /**
     * Return the pointer to the buffer holding data.
     */
    uint8_t* buffer() {
        return _value;
    }

    /**
     * Size in byte of a data.
     */
    static size_t size() {
        return array_size;
    }

protected:
    uint8_t _value[array_size];
};

/** 128 bit keys used by paired devices */
typedef byte_array_t<16> irk_t;
typedef byte_array_t<16> csrk_t;
typedef byte_array_t<16> ltk_t;

/** Used to identify LTK for legacy pairing connections */
typedef byte_array_t<2> ediv_t;
typedef byte_array_t<8> rand_t;

/** Out of band data exchanged during pairing */
typedef byte_array_t<16> oob_tk_t; /**< legacy pairing TK */
typedef byte_array_t<16> oob_lesc_value_t; /**< secure connections oob random 128 value */
typedef byte_array_t<16> oob_confirm_t; /**< secure connections oob confirmation value */

/** data to be encrypted */
typedef byte_array_t<16> encryption_block_t;

/** public key coordinate, two of which define the public key */
typedef byte_array_t<32> public_key_coord_t;

/** Diffie-Hellman key */
typedef byte_array_t<32> dhkey_t;

/**
 * MAC address data type.
 */
struct address_t : public byte_array_t<6> {
    /**
     * Create an invalid mac address, equal to FF:FF:FF:FF:FF:FF
     */
    address_t() {
        memset(_value, 0xFF, sizeof(_value));
    }

    /**
     * Initialize a data from an array of bytes.
     *
     * @param[in] input_value value of the data.
     */
    address_t(const uint8_t *input_value) {
        memcpy(_value, input_value, sizeof(_value));
    }
};

} // namespace ble

/**
 * @}
 * @}
 */

#endif /* BLE_TYPES_H_ */
