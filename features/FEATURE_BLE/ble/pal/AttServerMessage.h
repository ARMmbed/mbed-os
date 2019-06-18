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

#ifndef BLE_PAL_ATT_SERVER_MESSAGE_H_
#define BLE_PAL_ATT_SERVER_MESSAGE_H_

#include "ble/BLETypes.h"

namespace ble {
namespace pal {

/**
 * Operation code defined for attribute operations
 * @note see: BLUETOOTH SPECIFICATION Version 5.0 | Vol 3, Part F Section 3.4.8
 */
struct AttributeOpcode {
    enum Code {
        ERROR_RESPONSE = 0x01,               /// Opcode of an AttErrorResponse
        EXCHANGE_MTU_REQUEST = 0x02,
        EXCHANGE_MTU_RESPONSE = 0x03,       /// OpCode of an AttExchangeMTUResponse
        FIND_INFORMATION_REQUEST = 0x04,
        FIND_INFORMATION_RESPONSE = 0x05,   /// OpCode of an AttFindInformationResponse
        FIND_BY_TYPE_VALUE_REQUEST = 0x06,
        FIND_BY_VALUE_TYPE_RESPONSE = 0x07, /// OpCode of an AttFindByTypeValueResponse
        READ_BY_TYPE_REQUEST = 0x08,
        READ_BY_TYPE_RESPONSE = 0x09,       /// Opcode of an AttReadByTypeResponse
        READ_REQUEST = 0x0A,
        READ_RESPONSE = 0x0B,               /// Opcode of an AttReadResponse
        READ_BLOB_REQUEST = 0x0C,
        READ_BLOB_RESPONSE = 0x0D,          /// Opcode of an AttReadBlobResponse
        READ_MULTIPLE_REQUEST = 0x0E,
        READ_MULTIPLE_RESPONSE = 0x0F,      /// Opcode of an AttReadMultipleResponse
        READ_BY_GROUP_TYPE_REQUEST = 0x10,
        READ_BY_GROUP_TYPE_RESPONSE = 0x11, /// Opcode of an AttReadByGroupTypeResponse
        WRITE_REQUEST = 0x12,
        WRITE_RESPONSE = 0x13,              /// Opcode of an AttWriteResponse
        WRITE_COMMAND = 0x52,
        SIGNED_WRITE_COMMAND = 0xD2,
        PREPARE_WRITE_REQUEST = 0x16,
        PREPARE_WRITE_RESPONSE = 0x17,      /// Opcode of an AttPrepareWriteResponse
        EXECUTE_WRITE_REQUEST = 0x18,
        EXECUTE_WRITE_RESPONSE = 0x19,      /// Opcode of an AttExecuteWriteResponse
        HANDLE_VALUE_NOTIFICATION = 0x1B,
        HANDLE_VALUE_INDICATION = 0x1D
    };

    /**
     * Construct an AttributeOpcode from a Code.
     */
    AttributeOpcode(Code value) : _value(value) { }

    /**
     * Equality comparison operator between two AttributeOpcode
     */
    friend bool operator==(AttributeOpcode lhs, AttributeOpcode rhs) {
        return lhs._value == rhs._value;
    }

    /**
     * Non equality comparison operator between two AttributeOpcode
     */
    friend bool operator!=(AttributeOpcode lhs, AttributeOpcode rhs) {
        return lhs._value != rhs._value;
    }

    /**
     * implicit cast to uint8_t.
     * Allows AttributeOpcode to be used in switch statements.
     */
    operator uint8_t() const {
        return _value;
    }

private:
    uint8_t _value;
};


/**
 * Base class for Attribute Server Message.
 * The correct type of the instance can be determined with the attribute opcode.
 * @note see BLUETOOTH SPECIFICATION Version 5.0 | Vol 3, Part F Section 3.3.1
 */
struct AttServerMessage {
    /**
     * Op code used to identify the type of the attribute response.
     */
    const AttributeOpcode opcode;

protected:
    /**
     * Construction of an AttResponse is reserved for descendent of the class
     */
    AttServerMessage(AttributeOpcode opcode_) : opcode(opcode_) { }
};


/**
 * Response to a request which can't be performed
 * @note see BLUETOOTH SPECIFICATION Version 5.0 | Vol 3, Part F Section 3.4.1.1
 * for details about error response.
 * @note see BLUETOOTH SPECIFICATION Version 5.0 | Vol 3, Part F Section 3.4.9
 * which details possible error response by requests.
 */
struct AttErrorResponse : public AttServerMessage {
    /**
     * Construct an attribute error response.
     *
     * @param request_opcode_ The Attribute opcode of the request that generated
     * the error.
     * @param handle_in_error_ The attribute handle that generated this error
     * response.
     * @param error_code The reason why the request has generated an error.
     */
    AttErrorResponse(
        AttributeOpcode request_opcode_,
        attribute_handle_t handle_in_error_,
        uint8_t error_code_
    ) : AttServerMessage(AttributeOpcode::ERROR_RESPONSE),
        request_opcode(request_opcode_),
        handle_in_error(handle_in_error_), error_code(error_code_) {
    }

    /**
     * Construct an attribute error response in the case where there was no
     * attribute handle in the original response or if the request is not
     * supported.
     *
     * @param request_opcode_ The Attribute opcode of the request that generated
     * the error.
     * @param error_code The reason why the request has generated an error.
     */
    AttErrorResponse(
        AttributeOpcode request_opcode_,
        uint8_t error_code_
    ) : AttServerMessage(AttributeOpcode::ERROR_RESPONSE),
        request_opcode(request_opcode_),
        handle_in_error(0x0000), error_code(error_code_) {
    }

    /**
     * The opcode of the request that generated this error response.
     */
    const AttributeOpcode request_opcode;

    /**
     * The attribute handle that generated this error response.
     * If there was no attribute handle in the original request or if the
     * request is not supported, then this field is equal to 0x0000.
     */
    const attribute_handle_t handle_in_error;

    /**
     * The reason why the request has generated an error response
     */
    const uint8_t error_code;

    /**
     * List of Error codes for the ATT protocol
     */
    enum AttributeErrorCode {
        /** The attribute handle given was not valid on this server. */
        INVALID_HANDLE = 0x01,

        /** The attribute cannot be read. */
        READ_NOT_PERMITTED = 0x02,

        /** The attribute cannot be written. */
        WRITE_NOT_PERMITTED = 0x03,

        /** The attribute PDU was invalid. */
        INVALID_PDU = 0x04,

        /** The attribute requires authentication before it can be read or
         * written.
         */
        INSUFFICIENT_AUTHENTICATION = 0x05,

        /** Attribute server does not support the request received from the
         * client.
         */
        REQUEST_NOT_SUPPORTED = 0x06,

        /** Offset specified was past the end of the attribute. */
        INVALID_OFFSET = 0x07,

        /** The attribute requires authorization before it can be read or written. */
        INSUFFICIENT_AUTHORIZATION = 0x08,

        /** Too many prepare writes have been queued. */
        PREPARE_QUEUE_FULL = 0x09,

        /** No attribute found within the given attribute handle range. */
        ATTRIBUTE_NOT_FOUND = 0x0A,

        /** The attribute cannot be read using the Read Blob Request. */
        ATTRIBUTE_NOT_LONG = 0x0B,

        /** The Encryption Key Size used for encrypting this link is
         * insufficient.
         */
        INSUFFICIENT_ENCRYPTION_KEY_SIZE = 0x0C,

        /** The attribute value length is invalid for the operation. */
        INVALID_ATTRIBUTE_VALUE_LENGTH = 0x0D,

        /** The attribute request that was requested has encountered an error
         * that was unlikely, and therefore could not be completed as requested.
         */
        UNLIKELY_ERROR = 0x0E,

        /** The attribute requires encryption before it can be read or written. */
        INSUFFICIENT_ENCRYPTION = 0x0F,

        /** The attribute type is not a supported grouping attribute as defined
         * by a higher layer specification.
         */
        UNSUPPORTED_GROUP_TYPE = 0x10,

        /** Insufficient Resources to complete the request. */
        INSUFFICIENT_RESOURCES = 0x11,

        /* 0x12 - 0x7F => reserved for future use */

        /* 0x80 - 0x9F => Application Error */

        /* 0xA0 0xDF => Reserved for future use */

        /* 0xE0 - 0xFF Common Profile and service Error Codes */

        /** The Write Request Rejected error code is used when a requested write
         * operation cannot be fulfilled for reasons other than permissions.
         */
        WRITE_REQUEST_REJECTED = 0xFC,

        /** The Client Characteristic Configuration Descriptor Improperly
         * Configured error code is used when a Client Characteristic
         * Configuration descriptor is not configured according to the
         * requirements of the profile or service.
         */
        CLIENT_CHARACTERISTIC_CONFIGURATION_DESCRIPTOR_IMPROPERLY_CONFIGURED = 0xFD,

        /** The Procedure Already in Progress error code is used when a profile
         * or service request cannot be serviced because an operation that has
         * been previously triggered is still in progress
         */
        PROCEDURE_ALREADY_IN_PROGRESS = 0xFE,

        /** The Out of Range error code is used when an attribute value is out
         * of range as defined by a profile or service specification.
         */
        OUT_OF_RANGE = 0xFF
    };
};


/**
 * The Exchange MTU Request is used by the client to inform the server of the
 * client’s maximum receive MTU size and request the server to respond with its
 * maximum rx MTU size.
 * @note see BLUETOOTH SPECIFICATION Version 5.0 | Vol 3, Part F Section 3.4.2.2
 */
struct AttExchangeMTUResponse : public AttServerMessage {
    /**
     * Construct an exchange mtu response containing the max rx mtu of the
     * server.
     *
     * @param server_rx_mtu_ The max rx mtu the server can handle.
     */
    AttExchangeMTUResponse(uint16_t server_rx_mtu_) :
        AttServerMessage(AttributeOpcode::EXCHANGE_MTU_RESPONSE),
        server_rx_mtu(server_rx_mtu_) {
    }

    /**
     * The max rx mtu the server can handle.
     */
    const uint16_t server_rx_mtu;
};


/**
 * The Find Information Response is sent in reply to a received Find Information
 * Request and contains information about this server.
 *
 * The Find Information Response contains a sequence of handle-uuid pairs in
 * ascending order if attribute handles.
 *
 * This class has to be subclassed by an implementation specific class defining
 * the member function size and the subscript operator.
 * @note see BLUETOOTH SPECIFICATION Version 5.0 | Vol 3, Part F Section 3.4.3.2
 */
struct AttFindInformationResponse : public AttServerMessage {

    /** handle-uuid pair */
    struct information_data_t {
        attribute_handle_t handle;
        UUID uuid;
    };

    /**
     * Base constructor, setup the OpCode of the response.
     */
    AttFindInformationResponse() :
        AttServerMessage(AttributeOpcode::FIND_INFORMATION_RESPONSE) {
    }

    /**
     * virtual destructor to overide if the sub class needs it.
     */
    virtual ~AttFindInformationResponse() { }

    /**
     * Returns the number of information_data_t present in the response.
     */
    virtual size_t size() const = 0;

    /**
     * Access to information_data_t elements present in the response.
     * @note Out of range access is undefined.
     */
    virtual information_data_t operator[](size_t index) const = 0;
};


/**
 * Find by type value responses are sent in response to find by type value
 * request.
 *
 * The response contains a sequence of Found Attribute Handle, Group End Handle
 * pair where:
 *   - Found Attribute Handle is the handle of an attribute matching the type
 *     and the value requested.
 *   - Group End Handle is the end of the attribute group if the attribute found
 *     is a grouping attribute or the same value as Found Attribute Handle if
 *     the attribute is not a grouping attribute.
 *
 * This class should be subclassed by an implementation specific class defining
 * the member function size and the subscript operator.
 *
 * @note see BLUETOOTH SPECIFICATION Version 5.0 | Vol 3, Part F Section 3.4.3.4
 */
struct AttFindByTypeValueResponse : public AttServerMessage {
    /**
     * Base constructor, setup the OpCode of the response.
     */
    AttFindByTypeValueResponse() :
        AttServerMessage(AttributeOpcode::FIND_BY_VALUE_TYPE_RESPONSE) {
    }

    /**
     * virtual destructor to overide if the sub class needs it.
     */
    virtual ~AttFindByTypeValueResponse() { }

    /**
     * Returns the number of attribute_handle_range_t present in the response.
     */
    virtual std::size_t size() const = 0;

    /**
     * Access to the attribute range present in the response.
     * @note Out of range access is undefined.
     */
    virtual attribute_handle_range_t operator[](size_t index) const = 0;
};


/**
 * Response to a Read By Type request.
 *
 * It contains a list of handle-value pairs where:
 *   - handle is the handle of the attribute matching the rype requested.
 *   - value is the value of the attribute found. If the value is longer than
 *     (mtu - 4) then it can be truncated and read blob request should be used
 *     to read the remaining octet of the attribute.
 *
 * This class has to be subclassed by an implementation specific class defining
 * the member function size and the subscript operator.
 *
 * @note see BLUETOOTH SPECIFICATION Version 5.0 | Vol 3, Part F Section 3.4.4.2
 */
struct AttReadByTypeResponse : public AttServerMessage {
    /**
     * handle-value pair
     */
    struct attribute_data_t {
        attribute_handle_t handle;
        Span<const uint8_t> value;
    };

    /**
     * Base constructor, setup the OpCode of the response.
     */
    AttReadByTypeResponse() :
        AttServerMessage(AttributeOpcode::READ_BY_TYPE_RESPONSE) {
    }

    /**
     * virtual destructor to overide if the sub class needs it.
     */
    virtual ~AttReadByTypeResponse() { }

    /**
     * Return the number of attribute_data_t presents in the response.
     */
    virtual size_t size() const = 0;

    /**
     * Return the attribute data at index.
     * @note Out of range access is undefined.
     */
    virtual attribute_data_t operator[](size_t index) const = 0;
};


/**
 * The read response is sent in reply to a received Read Request and contains
 * the value of the attribute that has been read.
 *
 * The attribute value shall be set to the value of the attribute identified by
 * the attribute handle in the request. If the attribute value is longer than
 * (ATT_MTU-1) then the first (ATT_MTU-1) octets shall be included in this
 * response.
 *
 * @note The Read Blob Request would be used to read the remaining octets of a
 * long attribute value.
 *
 * @note see BLUETOOTH SPECIFICATION Version 5.0 | Vol 3, Part F Section 3.4.4.4
 */
struct AttReadResponse : public AttServerMessage {
    /**
     * Construct a Read Response from an array of bytes.
     */
    AttReadResponse(Span<const uint8_t> data_) :
        AttServerMessage(AttributeOpcode::READ_RESPONSE), _data(data_) {
    }

    /**
     * Return the number of octets presents in the response.
     */
    size_t size() const {
        return _data.size();
    }

    /**
     * Return the octet at the specified index.
     * @note Out of range access is undefined.
     */
    uint8_t operator[](size_t index) const {
        return _data[index];
    }

    /**
     * Return the pointer to the actual data
     */
    const uint8_t* data() const {
    	return _data.data();
    }

private:
    const Span<const uint8_t> _data;
};


/**
 * The Read Blob Response is sent in reply to a received Read Blob Request and
 * contains part of the value of the attribute that has been read.
 *
 * If the offset requested is equal to the length of the attribute then the
 * response contains no data and the size of the data returned is equal to 0.
 *
 * If the value of the attribute starting at the offset requested is longer than
 * (mtu - 1) octets then the first (mtu - 1) will be present in the response.
 * The remaining octets will be acquired by another Read Blob Request with an
 * updated index.
 *
 * @note see BLUETOOTH SPECIFICATION Version 5.0 | Vol 3, Part F Section 3.4.4.6
 */
struct AttReadBlobResponse : public AttServerMessage {
    /**
     * Construct a read blob response from the value responded.
     */
    AttReadBlobResponse(Span<const uint8_t> data_) :
        AttServerMessage(AttributeOpcode::READ_BLOB_RESPONSE), _data(data_) {
    }

    /**
     * Return the number of octets presents in the response value.
     */
    size_t size() const {
        return _data.size();
    }

    /**
     * Return the octet of the value read at the specified index.
     * @note Out of range access is undefined.
     */
    uint8_t operator[](size_t index) const {
        return _data[index];
    }

    /**
     * Return the pointer to the actual data
     */
    const uint8_t* data() const {
    	return _data.data();
    }

private:
    const Span<const uint8_t> _data;
};


/**
 * Response to a Read Multiple Request. It contains the values of the attributes
 * that have been read.
 *
 * If the set of values that has been read is longer than (mtu - 1) then only
 * the first (mtu - 1) octets are included in the response.
 *
 * @note see BLUETOOTH SPECIFICATION Version 5.0 | Vol 3, Part F Section 3.4.4.8
 */
struct AttReadMultipleResponse : public AttServerMessage {
    /**
     * Construct a Resd Multiple Response from the set of value received.
     */
    AttReadMultipleResponse(Span<const uint8_t> data_) :
        AttServerMessage(AttributeOpcode::READ_MULTIPLE_RESPONSE), _data(data_) {
    }

    /**
     * Return the number of octets presents in the response set of value.
     */
    size_t size() const {
        return _data.size();
    }

    /**
     * Return the octet of the set of value read at the specified index.
     * @note Out of range access is undefined.
     */
    uint8_t operator[](size_t index) const {
        return _data[index];
    }

private:
    const Span<const uint8_t> _data;
};


/**
 * The Read By Group Type Response is sent in reply to a received Read By
 * Group Type Request and contains the handles and values of the attributes that
 * have been read.
 *
 * The response is a list of group range-value pair where:
 *   - group range: The range of the group found where begin is the grouping
 *     attribute handle and end is the handle of the end of the group.
 *   - value: The value of the grouping attribute.
 *
 * This class has to be subclassed by an implementation specific class defining
 * the member function size and the subscript operator.
 *
 * @note The value responded can be trucated if it doesn't fit in the response,
 * in that case a Read Blob Request could be used to read the remaining octets.
 *
 * @note see BLUETOOTH SPECIFICATION Version 5.0 | Vol 3, Part F Section 3.4.4.10
 */
struct AttReadByGroupTypeResponse : public AttServerMessage {
    /**
     * Data read from the grouping attribute.
     * It includes the range of the group and the value of the attribute.
     */
    struct attribute_data_t {
        attribute_handle_range_t group_range;
        Span<const uint8_t> value;
    };

    /**
     * Base constructor, setup the OpCode of the response.
     */
    AttReadByGroupTypeResponse() :
        AttServerMessage(AttributeOpcode::READ_BY_GROUP_TYPE_RESPONSE) {
    }

    /**
     * virtual destructor to overide if the sub class needs it.
     */
    virtual ~AttReadByGroupTypeResponse() { }

    /**
     * Return the number of attribute_data_t present in the response.
     */
    virtual size_t size() const = 0;

    /**
     * Return the attribute data read at the index specified.
     * @note Out of range access is undefined.
     */
    virtual attribute_data_t operator[](size_t index) const = 0;
};


/**
 * The Write Response is sent in reply to a valid Write Request and
 * acknowledges that the attribute has been successfully written.
 * It is just a placeholder which indicates the client that the write request
 * was successful.
 *
 * @note see BLUETOOTH SPECIFICATION Version 5.0 | Vol 3, Part F Section 3.4.5.2
 */
struct AttWriteResponse : public AttServerMessage {
    /**
     * Construct a write response.
     */
    AttWriteResponse() : AttServerMessage(AttributeOpcode::WRITE_RESPONSE) { }
};


/**
 * Response to a Prepare Write Request. It acknowledges the client that the
 * value has been successfully received and placed in the write queue.
 *
 * The response contains the same values as the one present in the request.
 *
 * @note see BLUETOOTH SPECIFICATION Version 5.0 | Vol 3, Part F Section 3.4.6.2
 */
struct AttPrepareWriteResponse : public AttServerMessage {
    /**
     * Construct a prepare write response.
     * @param handle_ The handle of the attribute to be written.
     * @param offset_: The offset of the first octet to be writen.
     * @param value_: The value of the attribute to be written at the offset
     * indicated.
     */
    AttPrepareWriteResponse(
        attribute_handle_t handle_,
        uint16_t offset_,
        Span<const uint8_t> value_
    ) : AttServerMessage(AttributeOpcode::PREPARE_WRITE_RESPONSE),
        attribute_handle(handle_),
        offset(offset_),
        partial_value(value_) {
    }

    /**
     * The handle of the attribute to be written.
     */
    const attribute_handle_t attribute_handle;

    /**
     * The offset of the first octet to be writen.
     */
    const uint16_t offset;

    /**
     * The value of the attribute to be written at the offset indicated.
     */
    const Span<const uint8_t> partial_value;
};


/**
 * The Execute Write Response is sent in response to a received Execute Write
 * Request.
 *
 * It is just a placeholder which indicates the client that the execution of the
 * write request has been successfull.
 *
 * @note see BLUETOOTH SPECIFICATION Version 5.0 | Vol 3, Part F Section 3.4.6.4
 */
struct AttExecuteWriteResponse : public AttServerMessage {
    /**
     * Construct an execute write response object.
     */
    AttExecuteWriteResponse() :
        AttServerMessage(AttributeOpcode::EXECUTE_WRITE_RESPONSE) {
    }
};


/**
 * Notification of an attribute's value sent by the server.
 *
 * It contains the handle of the attribute and its value.
 *
 * If the attribute value is longer than (mtu - 3) then the value is truncated
 * to (mtu - 3) octets to fit in the response and the client will have to use
 * a read blob request to read the remaining octets of the attribute.
 *
 * @note see BLUETOOTH SPECIFICATION Version 5.0 | Vol 3, Part F Section 3.4.7.1
 */
struct AttHandleValueNotification : public AttServerMessage {
    /**
     * Construct an Handle Value Notification from the attribute handle and its
     * value notified.
     */
    AttHandleValueNotification(
        attribute_handle_t attribute_handle,
        Span<const uint8_t> attribute_value
    ) : AttServerMessage(AttributeOpcode::HANDLE_VALUE_NOTIFICATION),
        attribute_handle(attribute_handle),
        attribute_value(attribute_value) {
    }

    /**
     * Handle of the attribute
     */
    const attribute_handle_t attribute_handle;

    /**
     * The current value of the attribute.
     */
    const Span<const uint8_t> attribute_value;
};


/**
 * Indication of an attribute's value sent by the server.
 *
 * It contains the handle of the attribute and its value. The client should
 * respond with and handle value confirmation.
 *
 * If the attribute value is longer than (mtu - 3) then the value is truncated
 * to (mtu - 3) octets to fit in the response and the client will have to use
 * a read blob request to read the remaining octets of the attribute.
 *
 * @note see BLUETOOTH SPECIFICATION Version 5.0 | Vol 3, Part F Section 3.4.7.2
 */
struct AttHandleValueIndication : public AttServerMessage {
    /**
     * Construct an Handle Value Indication from the attribute handle and its
     * value indicated.
     */
    AttHandleValueIndication(
        attribute_handle_t handle, Span<const uint8_t> value
    ) : AttServerMessage(AttributeOpcode::HANDLE_VALUE_INDICATION),
        attribute_handle(handle), attribute_value(value) {
    }

    /**
     * Handle of the attribute
     */
    const attribute_handle_t attribute_handle;

    /**
     * The current value of the attribute.
     */
    const Span<const uint8_t> attribute_value;
};


} // namespace pal
} // namespace ble

#endif /* BLE_PAL_ATT_SERVER_MESSAGE_H_ */
