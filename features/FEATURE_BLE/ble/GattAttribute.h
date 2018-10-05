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

#ifndef MBED_GATT_ATTRIBUTE_H__
#define MBED_GATT_ATTRIBUTE_H__

#include "UUID.h"
#include "BLETypes.h"

/**
 * @addtogroup ble
 * @{
 * @addtogroup gatt
 * @{
 * @addtogroup server
 * @{
 */

/**
 * Representation of a GattServer attribute.
 *
 * Attributes are the building block of GATT servers: services are attributes,
 * characteristics are groups of attributes and characteristic descriptors are
 * attributes, too.
 *
 * @par Typed values
 *
 * Attributes are typed values composed of a type and its associated value. The
 * attribute type identifies the attribute purpose. A UUID read by the client
 * during the discovery of the GATT server models the attribute type. The value of the
 * attribute is an array of bytes; its length may be fixed or variable.
 *
 * As an example, a primary service is declared by an attribute with the type
 * 0x2800, and the value of the attribute is the UUID of the service.
 *
 * @par Attribute Access
 *
 * The GATT server is an array of attributes in which a unique index identifies
 * each of the attributes within the array. That index is called the attribute
 * handle, and clients use it to access to attributes within the server.
 *
 * @note Attributes do not contain information related to their permissions,
 * grouping or semantic. Higher level specifications define these concepts.
 */
class GattAttribute {
public:
    /**
     * Representation of an attribute handle.
     *
     * Each attribute in a GattServer has a unique handle that clients can use
     * to identify the attribute. The underlying BLE stack usually
     * generates and assigns handles to attributes.
     */
    typedef ble::attribute_handle_t Handle_t;

    /**
     * Invalid attribute handle.
     */
    static const Handle_t INVALID_HANDLE = 0x0000;

public:

    typedef ble::att_security_requirement_t Security_t;

    /**
     * Construct an attribute.
     *
     * Application code uses attributes to model characteristic descriptors and
     * characteristics values.
     *
     * @param[in] uuid The type of the attribute.
     * @param[in] valuePtr Pointer to the memory buffer, which contains the
     * initial value of the attribute. The constructor does not make a copy of
     * the attribute buffer; as a consequence, the memory buffer must remain
     * valid during the lifetime of the attribute.
     * @param[in] len The length in bytes of this attribute's value.
     * @param[in] maxLen The length in bytes of the memory buffer containing the
     * attribute value. It must be greater than or equal to @p len.
     * @param[in] hasVariableLen Flag that indicates whether the attribute's value
     * length can change throughout time.
     *
     * @par Example
     *
     * @code
     * // declare a value of 2 bytes within a 10 bytes buffer
     * const uint8_t attribute_value[10] = { 10, 50 };
     * GattAttribute attr = GattAttribute(
     *    0x2A19, // attribute type
     *    attribute_value,
     *    2, // length of the current value
     *    sizeof(attribute_value), // length of the buffer containing the value
     *    true // variable length
     * );
     * @endcode
     *
     * @note By default, read and write operations are allowed and does not
     * require any security.
     */
    GattAttribute(
        const UUID &uuid,
        uint8_t *valuePtr = NULL,
        uint16_t len = 0,
        uint16_t maxLen = 0,
        bool hasVariableLen = true
    ) : _uuid(uuid),
        _valuePtr(valuePtr),
        _lenMax(maxLen),
        _len(len),
        _handle(),
        _hasVariableLen(hasVariableLen),
        _read_allowed(true),
        _read_security(Security_t::NONE),
        _write_allowed(true),
        _write_security(Security_t::NONE) {
    }

public:
    /**
     * Get the attribute's handle in the ATT table.
     *
     * @note The GattServer sets the attribute's handle when services are
     * inserted.
     *
     * @return The attribute's handle.
     */
    Handle_t getHandle(void) const
    {
        return _handle;
    }

    /**
     * Get the UUID of the attribute.
     *
     * The UUID identifies the type of the attribute.
     *
     * @return The attribute.
     */
    const UUID &getUUID(void) const
    {
        return _uuid;
    }

    /**
     * Get the current length of the attribute value.
     *
     * @return The current length of the attribute value.
     */
    uint16_t getLength(void) const
    {
        return _len;
    }

    /**
     * Get the maximum length of the attribute value.
     *
     * The maximum length of the attribute value.
     */
    uint16_t getMaxLength(void) const
    {
        return _lenMax;
    }

    /**
     * Get a pointer to the current length of the attribute value.
     *
     * @attention note Do not use this function.
     *
     * @return A pointer to the current length of the attribute value.
     */
    uint16_t *getLengthPtr(void)
    {
        return &_len;
    }

    /**
     * Set the attribute handle.
     *
     * @attention The GattServer uses this function internally.
     * Application code must not use it.
     *
     * @param[in] id The new attribute handle.
     */
    void setHandle(Handle_t id)
    {
        _handle = id;
    }

    /**
     * Get a pointer to the attribute value.
     *
     * @return A pointer to the attribute value.
     */
    uint8_t *getValuePtr(void)
    {
        return _valuePtr;
    }

    /**
     * Check whether the length of the attribute's value can change throughout time.
     *
     * @return true if the attribute value has a variable length and false
     * otherwise.
     */
    bool hasVariableLength(void) const
    {
        return _hasVariableLen;
    }

    /**
     * Allow or disallow read operation from a client.
     * @param allow_read Read is allowed if true.
     */
    void allowRead(bool allow_read)
    {
        _read_allowed = allow_read;
    }

    /**
     * Indicate if a client is allowed to read the attribute.
     * @return true if a client is allowed to read the attribute.
     */
    bool isReadAllowed(void) const
    {
        return _read_allowed;
    }

    /**
     * Set the security requirements of the read operations.
     * @param requirement The security level required by the read operations.
     */
    void setReadSecurityRequirement(Security_t requirement)
    {
        _read_security = requirement.value();
    }

    /**
     * Return the security level required by read operations.
     * @return The security level of the read operations.
     */
    Security_t getReadSecurityRequirement() const
    {
        return static_cast<Security_t::type>(_read_security);
    }

    /**
     * Allow or disallow write operation from a client.
     * @param allow_write Write is allowed if true.
     */
    void allowWrite(bool allow_write)
    {
        _write_allowed = allow_write;
    }

    /**
     * Indicate if a client is allowed to write the attribute.
     * @return true if a client is allowed to write the attribute.
     */
    bool isWriteAllowed(void) const
    {
        return _write_allowed;
    }

    /**
     * Set the security requirements of the write operations.
     * @param requirement The security level required by the write operations.
     */
    void setWriteSecurityRequirement(Security_t requirement)
    {
        _write_security = requirement.value();
    }

    /**
     * Return the security level required by write operations.
     * @return The security level of the write operations.
     */
    Security_t getWriteSecurityRequirement() const
    {
        return static_cast<Security_t::type>(_write_security);
    }

private:
    /**
     * Characteristic's UUID.
     */
    UUID _uuid;

    /**
     * Pointer to the attribute's value.
     */
    uint8_t *_valuePtr;

    /**
     * Length in byte of the buffer containing the attribute value.
     */
    uint16_t _lenMax;

    /**
     * Current length of the value pointed to by GattAttribute::_valuePtr.
     */
    uint16_t _len;

    /**
     * The attribute's handle in the ATT table.
     */
    Handle_t _handle;

    /**
     * Whether the length of the value can change throughout time.
     */
    bool _hasVariableLen;

    /**
     * Whether read is allowed or not.
     */
    uint8_t _read_allowed:1;

    /**
     * Security applied to the read operation.
     */
    uint8_t _read_security: Security_t::size;

    /**
     * Whether write is allowed or not.
     */
    uint8_t _write_allowed:1;

    /**
     * Security applied to the write operation.
     */
    uint8_t _write_security: Security_t::size;

private:
    /* Disallow copy and assignment. */
    GattAttribute(const GattAttribute &);
    GattAttribute& operator=(const GattAttribute &);
};

/**
 * @}
 * @}
 * @}
 */

#endif /* ifndef MBED_GATT_ATTRIBUTE_H__ */
