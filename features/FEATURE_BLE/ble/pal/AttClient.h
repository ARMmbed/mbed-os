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

#ifndef BLE_PAL_ATTCLIENT_H_
#define BLE_PAL_ATTCLIENT_H_

#include "ble/common/StaticInterface.h"
#include "ble/UUID.h"
#include "ble/BLETypes.h"
#include "ble/blecommon.h"
#include "platform/Callback.h"
#include "AttServerMessage.h"

namespace ble {
namespace pal {

/**
 * Send attribute protocol requests to an ATT server. It also handle reception
 * of ATT response and server indication/notification.
 *
 * Every request send and response or response event received is for a specified
 * connection.
 *
 * @warning This class should not be used outside mbed BLE, availability is not
 * guaranteed for all ports.
 *
 * @note see BLUETOOTH SPECIFICATION Version 5.0 | Vol 3, Part F
 */
template<class Impl>
struct AttClient : public StaticInterface<Impl, AttClient> {
private:

    using StaticInterface<Impl, ble::pal::AttClient>::impl;

public:


    /**
     * Initialization of the instance. An implementation can use this function
     * to initialise the subsystems needed to realize the ATT operations of this
     * interface.
     *
     * This function has to be called before any other operations.
     *
     * @return BLE_ERROR_NONE if the request has been successfully sent or the
     * appropriate error otherwise.
     */
    ble_error_t initialize() {
        return impl()->initialize_();
    }

    /**
     * Termination of the instance. An implementation can use this function
     * to release the subsystems initialised  to realise the ATT operations of
     * this interface.
     *
     * After a call to this function, initialise should be called again to
     * allow usage of the interface.
     *
     * @return BLE_ERROR_NONE if the request has been successfully sent or the
     * appropriate error otherwise.
     */
    ble_error_t terminate() {
        return impl()->terminate_();
    }

    /**
     * Send an exchange MTU request which negotiate the size of the MTU used by
     * the connection.
     *
     * First the client send to the server the maximum rx mtu that it can receive
     * then the client reply with the maximum rx mtu it can receive.
     *
     * The mtu choosen for the connection is the minimum of the client Rx mtu
     * and server Rx mtu values.
     *
     * If an error occured then the mtu used remains the default value.
     *
     * @param connection The handle of the connection to send this request to.
     *
     * @return BLE_ERROR_NONE if the request has been succesfully sent or the
     * appropriate error otherwise.
     *
     * @see ble::pal::AttExchangeMTUResponse The type of response received from
     * the  server
     * @see ble::pal::AttErrorResponse::REQUEST_NOT_SUPPORTED The error code
     * returned by the server in case of error.
     *
     * @note see BLUETOOTH SPECIFICATION Version 5.0 | Vol 3, Part F Section 3.4.2.1
     */
    ble_error_t exchange_mtu_request(connection_handle_t connection) {
        return impl()->exchange_mtu_request_(connection);
    }

    /**
     * Acquire the size of the mtu for a given connection.
     *
     * @param connection The handle of the connection for which the the MTU size
     * should be acquired.
     *
     * @param mtu_size Output parameter which will contain the MTU size.
     *
     * @return BLE_ERROR_NONE if the MTU size has been acquired or the
     * appropriate error otherwise.
     */
    ble_error_t get_mtu_size(
        connection_handle_t connection_handle,
        uint16_t& mtu_size
    ) {
        return impl()->get_mtu_size_(connection_handle, mtu_size);
    }

    /**
     * Send a find information request to a server in order to obtain the
     * mapping of attribute handles with their associated types.
     *
     * The server will reply with a ble::pal::AttFindInformationResponse
     * containing at least one [attribute handle, attribute type] pair. If the
     * last handle in the response is not equal to the end handle of the finding
     * range then this request can be issued again with an updated range (begin
     * equal to last handle received + 1) to discover the remaining attributes.
     *
     * To discover the whole ATT server, the first find information request
     * should have a discovery range of [0x0001 - 0xFFFF].
     *
     * The server can send a ble::pal::AttErrorResponse with the code
     * ble::pal::AttErrorResponse::ATTRIBUTE_NOT_FOUND if no attributes have
     * been found in the range specified. The attribute handle in the response
     * is then equal to the first handle of the discovery range.
     *
     * If the range is malformed the server will reply a
     * ble::pal::AttErrorResponse with the error code ble::pal::INVALID_HANDLE.
     *
     * @param connection_handle The handle of the connection to send this
     * request to.
     * @param discovery_range The attribute range where handle-type informations
     * should be discovered.
     *
     * @return BLE_ERROR_NONE if the request has been successfully sent or the
     * appropriate error otherwise.
     *
     * @note see BLUETOOTH SPECIFICATION Version 5.0 | Vol 3, Part F Section 3.4.3.1
     */
    ble_error_t find_information_request(
        connection_handle_t connection_handle,
        attribute_handle_range_t discovery_range
    ) {
        return impl()->find_information_request_(connection_handle, discovery_range);
    }

    /**
     * Send a Find By Type Value Request which retrieve the handles of attributes
     * that have known 16-bit UUID attribute type and known attribute value.
     *
     * The server should reply with a ble::pal::AttFindByTypeValueResponse
     * containing the handle (or handle range in case of grouping attributes) of
     * the attribute found.
     *
     * If not all attributes can be contained in the response it is necessary to
     * send again this request with an updated range to continue the discovery.
     *
     * The server can send a ble::pal::AttErrorResponse with the code
     * ble::pal::AttErrorResponse::ATTRIBUTE_NOT_FOUND if no attributes have
     * been found in the range specified. The attribute handle in the response
     * is then equal to the first handle of the discovery range.
     *
     * If the range is malformed the server will reply a
     * ble::pal::AttErrorResponse with the error code ble::pal::INVALID_HANDLE.
     *
     * @param connection_handle The handle of the connection to send this
     * request to.
     * @param discovery_range The handle range where attributes with type and
     * value are searched.
     * @param type The type of attribute to find (it is a 16 bit UUID).
     * @param value The value of the attributes to found.
     *
     * @return BLE_ERROR_NONE if the request has been successfully sent or the
     * appropriate error otherwise.
     *
     * @note see BLUETOOTH SPECIFICATION Version 5.0 | Vol 3, Part F Section 3.4.3.3
     */
    ble_error_t find_by_type_value_request(
        connection_handle_t connection_handle,
        attribute_handle_range_t discovery_range,
        uint16_t type,
        const Span<const uint8_t>& value
    ) {
        return impl()->find_by_type_value_request_(
            connection_handle,
            discovery_range,
            type,
            value
        );
    }

    /**
     * Send a Read By Type Request used to obtain the values of attributes where
     * the attribute type is known but the handle is not known.
     *
     * If attributes with the type requested are present in the range, the server
     * should reply with a ble::pal::AttReadByTypeResponse. If the response does
     * not cover the full range, the request should be sent again with an updated
     * range.
     *
     * In case of error, the server will send a ble::pal::AttErrorResponse. The
     * error code depends on the situation:
     *   - ble::pal::AttErrorResponse::ATTRIBUTE_NOT_FOUND: If there is no
     *     attributes matching type in the range.
     *   - ble::pal::AttErrorResponse::INVALID_HANDLE: If the range is
     *     invalid.
     *   - ble::pal::AttErrorResponse::INSUFFICIENT_AUTHENTICATION: If the client
     *     security is not sufficient.
     *   - ble::pal::AttErrorResponse::INSUFFICIENT_AUTHORIZATION: If the client
     *     authorization is not sufficient.
     *   - ble::pal::AttErrorResponse::INSUFFICIENT_ENCRYPTION_KEY_SIZE: If the
     *     client has an insufficient encryption key size.
     *   - ble::pal::AttErrorResponse::INSUFFICIENT_ENCRYPTION: If the client
     *     has not enabled encryption.
     *   - ble::pal::AttErrorResponse::READ_NOT_PERMITTED: If the attribute
     *     value cannot be read.
     *
     * @param connection_handle The handle of the connection to send this
     * request to.
     * @param read_range The handle range where attributes with the given type
     * should be read.
     * @param type The type of attributes to read.
     *
     * @return BLE_ERROR_NONE if the request has been successfully sent or the
     * appropriate error otherwise.
     *
     * @note see BLUETOOTH SPECIFICATION Version 5.0 | Vol 3, Part F Section 3.4.4.1
     */
    ble_error_t read_by_type_request(
        connection_handle_t connection_handle,
        attribute_handle_range_t read_range,
        const UUID& type
    ) {
        return impl()->read_by_type_request_(connection_handle, read_range, type);
    }

    /**
     * Send a Read Request to read the value of an attribute in the server.
     *
     * In case of success, the server will reply with a ble::pal::AttReadResponse.
     * containing the value of the attribute. If the length of the value in the
     * response is equal to (mtu - 1) then the remaining part of the value can
     * be obtained by a read_blob_request.
     *
     * In case of error, the server will send a ble::pal::AttErrorResponse. The
     * error code depends on the situation:
     *   - ble::pal::AttErrorResponse::INVALID_HANDLE: If the attribute handle
     *     is invalid.
     *   - ble::pal::AttErrorResponse::INSUFFICIENT_AUTHENTICATION: If the client
     *     security is not sufficient.
     *   - ble::pal::AttErrorResponse::INSUFFICIENT_AUTHORIZATION: If the client
     *     authorization is not sufficient.
     *   - ble::pal::AttErrorResponse::INSUFFICIENT_ENCRYPTION_KEY_SIZE: If the
     *     client has an insufficient encryption key size.
     *   - ble::pal::AttErrorResponse::INSUFFICIENT_ENCRYPTION: If the client
     *     has not enabled encryption.
     *   - ble::pal::AttErrorResponse::READ_NOT_PERMITTED: If the attribute
     *     value cannot be read.
     * Higher layer can also set an application error code (0x80 - 0x9F).
     *
     * @param connection_handle The handle of the connection to send this
     * request to.
     * @param attribute_handle The handle of the attribute to read.
     *
     * @return BLE_ERROR_NONE if the request has been successfully sent or the
     * appropriate error otherwise.
     *
     * @note see BLUETOOTH SPECIFICATION Version 5.0 | Vol 3, Part F Section 3.4.4.3
     */
    ble_error_t read_request(
        connection_handle_t connection_handle,
        attribute_handle_t attribute_handle
    ) {
        return impl()->read_request_(connection_handle, attribute_handle);
    }

    /**
     * Send a read blob request to a server to read a part of the value of an
     * attribute at a given offset.
     *
     * In case of success, the server will reply with a ble::pal::AttReadBlobResponse
     * containing the value read. If the value of the attribute starting at the
     * offset requested is longer than (mtu - 1) octets then only the first
     * (mtu - 1) octets will be present in the response.
     * The remaining octets can be acquired by another Read Blob Request with an
     * updated index.
     *
     * In case of error, the server will send a ble::pal::AttErrorResponse. The
     * error code depends on the situation:
     *   - ble::pal::AttErrorResponse::INVALID_HANDLE: If the attribute handle
     *     is invalid.
     *   - ble::pal::AttErrorResponse::INSUFFICIENT_AUTHENTICATION: If the client
     *     security is not sufficient.
     *   - ble::pal::AttErrorResponse::INSUFFICIENT_AUTHORIZATION: If the client
     *     authorization is not sufficient.
     *   - ble::pal::AttErrorResponse::INSUFFICIENT_ENCRYPTION_KEY_SIZE: If the
     *     client has an insufficient encryption key size.
     *   - ble::pal::AttErrorResponse::INSUFFICIENT_ENCRYPTION: If the client
     *     has not enabled encryption.
     *   - ble::pal::AttErrorResponse::READ_NOT_PERMITTED: If the attribute
     *     value cannot be read.
     *   - ble::pal::AttErrorResponse::INVALID_OFFSET: If the offset is greater
     *     than the attribute length.
     *   - ble::pal::AttErrorResponse::ATTRIBUTE_NOT_LONG: If the attribute
     *     value has a length that is less than or equal to (mtu - 1).
     * Higher layer can also set an application error code (0x80 - 0x9F).
     *
     * @param connection_handle The handle of the connection to send this
     * request to.
     * @param attribute_handle The handle of the attribute to read.
     * @param offset The offset of the first octet to read.
     *
     * @return BLE_ERROR_NONE if the request has been successfully sent or an
     * appropriate error otherwise.
     *
     * @note see BLUETOOTH SPECIFICATION Version 5.0 | Vol 3, Part F Section 3.4.4.5
     */
    ble_error_t read_blob_request(
        connection_handle_t connection_handle,
        attribute_handle_t attribute_handle,
        uint16_t offset
    ) {
        return impl()->read_blob_request_(connection_handle, attribute_handle, offset);
    }

    /**
     * Send a read multiple request to the server. It is used to read two or more
     * attributes values at once.
     *
     * In case of success, the server will reply with a
     * ble::pal::AttReadMultipleResponse containing the concatenation of the
     * values read. Given that values are concatained, all attributes values
     * should be of fixed size except for the last one. The concatained value
     * is also truncated to (mtu - 1) if it doesn't fit in the response.
     *
     * In case of error, the server will send a ble::pal::AttErrorResponse. The
     * error code depends on the situation:
     *   - ble::pal::AttErrorResponse::INVALID_HANDLE: If any of the attribute
     *     handle is invalid.
     *   - ble::pal::AttErrorResponse::INSUFFICIENT_AUTHENTICATION: If the client
     *     security is not sufficient to read any of the attribute.
     *   - ble::pal::AttErrorResponse::INSUFFICIENT_AUTHORIZATION: If the client
     *     authorization is not sufficient to read any of the attribute.
     *   - ble::pal::AttErrorResponse::INSUFFICIENT_ENCRYPTION_KEY_SIZE: If the
     *     client has an insufficient encryption key size to read any of the
     *     attributes.
     *   - ble::pal::AttErrorResponse::INSUFFICIENT_ENCRYPTION: If the client
     *     has not enabled encryption required to read any of the attributes.
     *   - ble::pal::AttErrorResponse::READ_NOT_PERMITTED: If any of the
     *     attributes value cannot be read.
     * The first attribute causing the error is reporter in the handle_in_error
     * field in the error response.
     * Higher layer can also set an application error code (0x80 - 0x9F).
     *
     * @param connection_handle The handle of the connection to send this
     * request to.
     * @param attribute_handles Set of attribute handles to read.
     *
     * @return BLE_ERROR_NONE if the request has been successfully sent or an
     * appropriate error otherwise.
     *
     * @note see BLUETOOTH SPECIFICATION Version 5.0 | Vol 3, Part F Section 3.4.4.7
     */
    ble_error_t read_multiple_request(
        connection_handle_t connection_handle,
        const Span<const attribute_handle_t>& attribute_handles
    ) {
        return impl()->read_multiple_request_(connection_handle, attribute_handles);
    }

    /**
     * Send a read by group type request to the server. It is used to get
     * informations about grouping attribute of a given type on a server.
     *
     * The server will reply with a ble::pal::ReadByGroupTypeResponse containing
     * informations about the grouping attribute found. Informations are:
     *    - handle of the grouping attribute.
     *    - last handle of the group .
     *    - attribute value.
     *
     * If the last handle received is not the last handle of the discovery range
     * then it is necessary to send another request with a discovery range
     * updated to: [last handle + 1 : end].
     *
     * In case of error, the server will send a ble::pal::AttErrorResponse. The
     * error code depends on the situation:
     *   - ble::pal::AttErrorResponse::INVALID_HANDLE: If the range of handle
     *     provided is invalid.
     *   - ble::pal::AttErrorResponse::UNSUPPORTED_GROUP_TYPE: if the group type
     *     is not a supported grouping attribute.
     *   - ble::pal::AttErrorResponse::ATTRIBUTE_NOT_FOUND: If no attribute with
     *     the given type exists within the range provided.
     *   - ble::pal::AttErrorResponse::INSUFFICIENT_AUTHENTICATION: If the client
     *     security is not sufficient to read the requested attribute.
     *   - ble::pal::AttErrorResponse::INSUFFICIENT_AUTHORIZATION: If the client
     *     authorization is not sufficient to read the requested attribute.
     *   - ble::pal::AttErrorResponse::INSUFFICIENT_ENCRYPTION_KEY_SIZE: If the
     *     client has an insufficient encryption key size to read the requested
     *     attributes.
     *   - ble::pal::AttErrorResponse::INSUFFICIENT_ENCRYPTION: If the client
     *     has not enabled encryption required to read the requested attributes.
     *   - ble::pal::AttErrorResponse::READ_NOT_PERMITTED: If any of the
     *     attributes value cannot be read.
     * Higher layer can also set an application error code (0x80 - 0x9F).
     *
     * @param connection_handle The handle of the connection to send this
     * request to.
     * @param read_range Range where this request apply.
     * @param group_type Type of the grouping attribute to find and read.
     *
     * @return BLE_ERROR_NONE if the request has been successfully sent or an
     * appropriate error otherwise.
     *
     * @note see BLUETOOTH SPECIFICATION Version 5.0 | Vol 3, Part F Section 3.4.4.9
     */
    ble_error_t read_by_group_type_request(
        connection_handle_t connection_handle,
        attribute_handle_range_t read_range,
        const UUID& group_type
    ) {
        return impl()->read_by_group_type_request_(connection_handle, read_range, group_type);
    }

    /**
     * Send a write request to the server to write the value of an attribute.
     *
     * In case of success, the server will reply with a
     * ble::pal::AttWriteResponse to acknowledge that the write operation went
     * well.
     *
     * If the attribute value has a variable length, then the attribute value
     * shall be truncated or lengthened to match the length of the value in the
     * request.
     *
     * If the attribute value has a fixed length and the Attribute Value parameter length
     * is less than or equal to the length of the attribute value, the octets of the
     * attribute value parameter length shall be written; all other octets in this attribute
     * value shall be unchanged.
     *
     * In case of error, the server will send a ble::pal::AttErrorResponse. The
     * error code depends on the situation:
     *   - ble::pal::AttErrorResponse::INVALID_HANDLE: If the handle to write is
     *     invalid.
     *   - ble::pal::AttErrorResponse::INSUFFICIENT_AUTHENTICATION: If the client
     *     security is not sufficient to write the requested attribute.
     *   - ble::pal::AttErrorResponse::INSUFFICIENT_AUTHORIZATION: If the client
     *     authorization is not sufficient to write the requested attribute.
     *   - ble::pal::AttErrorResponse::INSUFFICIENT_ENCRYPTION_KEY_SIZE: If the
     *     client has an insufficient encryption key size to write the requested
     *     attributes.
     *   - ble::pal::AttErrorResponse::INSUFFICIENT_ENCRYPTION: If the client
     *     has not enabled encryption required to write the requested attributes.
     *   - ble::pal::AttErrorResponse::WRITE_NOT_PERMITTED: If the attribute
     *     value cannot be written due to permission.
     *   - ble::pal::AttErrorResponse::INVALID_ATTRIBUTE_VALUE_LENGTH: If the
     *     value to write exceeds the maximum valid length or of the attribute
     *     value; whether the attribute has a variable length value or a fixed
     *     length value.
     * Higher layer can also set an application error code (0x80 - 0x9F).
     *
     * @param connection_handle The handle of the connection to send this
     * request to.
     * @param attribute_handle Handle of the attribute to write.
     * @param value Value to write. It can't be longer than (mtu - 3).
     *
     * @return BLE_ERROR_NONE if the request has been successfully sent or an
     * appropriate error otherwise.
     *
     * @note see BLUETOOTH SPECIFICATION Version 5.0 | Vol 3, Part F Section 3.4.5.1
     */
    ble_error_t write_request(
        connection_handle_t connection_handle,
        attribute_handle_t attribute_handle,
        const Span<const uint8_t>& value
    ) {
        return impl()->write_request_(connection_handle, attribute_handle, value);
    }

    /**
     * Send a write command to the server. A write command is similar to a write
     * request except that it won't receive any response from the server
     *
     * @param connection_handle The handle of the connection to send this
     * request to.
     * @param attribute_handle Handle of the attribute to write.
     * @param value Value to write. It can't be longer than (mtu - 3).
     *
     * @return BLE_ERROR_NONE if the request has been successfully sent or an
     * appropriate error otherwise.
     *
     * @note see BLUETOOTH SPECIFICATION Version 5.0 | Vol 3, Part F Section 3.4.5.3
     */
    ble_error_t write_command(
        connection_handle_t connection_handle,
        attribute_handle_t attribute_handle,
        const Span<const uint8_t>& value
    ) {
        return impl()->write_command_(connection_handle, attribute_handle, value);
    }

    /**
     * Send a signed write command to the server. Behaviour is similar to a write
     * command except that 12 bytes of the mtu are reserved for the authentication
     * signature.
     *
     * @param connection_handle The handle of the connection to send this
     * request to.
     * @param attribute_handle Handle of the attribute to write.
     * @param value Value to write. It can't be longer than (mtu - 15).
     *
     * @note the authentication signature to send with this request is
     * computed by the implementation following the rules defined in BLUETOOTH
     * SPECIFICATION Version 5.0 | Vol 3, Part F Section 3.3.1.
     *
     * @return BLE_ERROR_NONE if the request has been successfully sent or an
     * appropriate error otherwise.
     *
     * @note see BLUETOOTH SPECIFICATION Version 5.0 | Vol 3, Part F Section 3.4.5.4
     */
    ble_error_t signed_write_command(
        connection_handle_t connection_handle,
        attribute_handle_t attribute_handle,
        const Span<const uint8_t>& value
    ) {
        return impl()->signed_write_command_(connection_handle, attribute_handle, value);
    }

    /**
     * The Prepare Write Request is used to request the server to prepare to
     * write the value of an attribute. The client can send multiple prepare
     * write request which will be put in a queue until the client send an
     * Execute Write Request which will execute sequentially the write request
     * in the queue.
     *
     * In case of success the server will respond with a
     * ble::pal::AttPrepareWriteResponse containing the values (attribute handle,
     * offset and value) present in the write request.
     *
     * If a prepare write request is rejected by the server, the state queue of
     * the prepare write request queue remains unaltered.
     *
     * In case of error, the server will send a ble::pal::AttErrorResponse. The
     * error code depends on the situation:
     *   - ble::pal::AttErrorResponse::INVALID_HANDLE: If the handle to write is
     *     invalid.
     *   - ble::pal::AttErrorResponse::INSUFFICIENT_AUTHENTICATION: If the client
     *     security is not sufficient to write the requested attribute.
     *   - ble::pal::AttErrorResponse::INSUFFICIENT_AUTHORIZATION: If the client
     *     authorization is not sufficient to write the requested attribute.
     *   - ble::pal::AttErrorResponse::INSUFFICIENT_ENCRYPTION_KEY_SIZE: If the
     *     client has an insufficient encryption key size to write the requested
     *     attributes.
     *   - ble::pal::AttErrorResponse::INSUFFICIENT_ENCRYPTION: If the client
     *     has not enabled encryption required to write the requested attributes.
     *   - ble::pal::AttErrorResponse::WRITE_NOT_PERMITTED: If the attribute
     *     value cannot be written due to permission.
     *   - ble::pal::PREPARE_QUEUE_FULL: If the queue of prepare write request
     *     is full.
     * Higher layer can also set an application error code (0x80 - 0x9F).
     *
     * @param connection_handle The handle of the connection to send this
     * request to.
     * @param attribute_handle The handle of the attribute to be written.
     * @param offset The offset of the first octet to be written.
     * @param value The value of the attribute to be written. It can't be longer
     * than (mtu - 5).
     *
     * @return BLE_ERROR_NONE if the request has been successfully sent or an
     * appropriate error otherwise.
     *
     * @note see BLUETOOTH SPECIFICATION Version 5.0 | Vol 3, Part F Section 3.4.6.1
     *
     */
    ble_error_t prepare_write_request(
        connection_handle_t connection_handle,
        attribute_handle_t attribute_handle,
        uint16_t offset,
        const Span<const uint8_t>& value
    ) {
        return impl()->prepare_write_request_(
            connection_handle,
            attribute_handle,
            offset,
            value
        );
    }

    /**
     * Send an Execute Write Request to the server. This request will instruct
     * the server to execute or cancel the prepare write requests currently held
     * in the prepare queue from this client.
     *
     * If the execute parameter is set to true, the server should execute the
     * request held in the queue. If the parameter is equal to false then the
     * server should cancel the requests in the queue.
     *
     * In case of success, the server will respond with a
     * ble::pal::AttExecuteWriteResponse indicating that the request was correctly
     * handled.
     *
     * In case of error, the server will send a ble::pal::AttErrorResponse. The
     * error code depends on the situation:
     *   - ble::pal::AttErrorResponse::INVALID_OFFSET: If the value offset is
     *     greater than the current length of the attribute to write.
     *   - ble::pal::AttErrorResponse::INVALID_ATTRIBUTE_VALUE_LENGTH: If the
     *     length of the value write exceeds the length of the attribute value
     *     about to be written.
     * Higher layer can also set an application error code (0x80 - 0x9F).
     *
     * The error response will contains the attribute handle which as caused the
     * error and the remaining of the prepare queue is discarded. The state of
     * the attributes that were to be written from the prepare queue is not
     * defined in this case.
     *
     * @param connection_handle The handle of the connection to send this
     * request to.
     * @param execute Boolean indicating if the prepare queue should be executed
     * or cleared.
     *
     * @return BLE_ERROR_NONE if the request has been successfully sent or an
     * appropriate error otherwise.
     *
     * @note see BLUETOOTH SPECIFICATION Version 5.0 | Vol 3, Part F Section 3.4.6.3
     */
    ble_error_t execute_write_request(
        connection_handle_t connection_handle,
        bool execute
    ) {
        return impl()->execute_write_request_(connection_handle, execute);
    }

    /**
     * Register a callback which will handle messages from the server.
     *
     * @param cb The callback object which will handle messages from the server.
     * It accept two parameters in input: The handle of the connection where the
     * message was received and the message received. Real type of the message
     * can be obtained from its opcode.
     */
    void when_server_message_received(
        mbed::Callback<void(connection_handle_t, const AttServerMessage&)> cb
    ) {
        _server_message_cb = cb;
    }

    /**
     * Register a callback handling transaction timeout.
     *
     * @param cb The callback handling timeout of a transaction. It accepts as
     * a parameter the connection handle involved in the timeout.
     *
     * @note No more attribute protocol requests, commands, indication or
     * notification shall be sent over a connection implied in a transaction
     * timeout. To send a new ATT message, the conenction should be
     * reestablished.
     */
     void when_transaction_timeout(
         mbed::Callback<void(connection_handle_t)> cb
     ) {
         _transaction_timeout_cb = cb;
     }

protected:
    AttClient() { }

    ~AttClient() { }

    /**
     * Upon server message reception an implementation shall call this function.
     *
     * @param connection_handle The handle of the connection which has received
     * the server message.
     * @param server_message The message received from the server.
     */
    void on_server_event(
        connection_handle_t connection_handle,
        const AttServerMessage& server_message
    ) {
        if (_server_message_cb) {
            _server_message_cb(connection_handle, server_message);
        }
    }

    /**
     * Upon transaction timeout an implementation shall call this function.
     *
     * @param connection_handle The handle of the connection of the transaction
     * which has times out.
     *
     * @note see BLUETOOTH SPECIFICATION Version 5.0 | Vol 3, Part F Section 3.3.3
     */
    void on_transaction_timeout(
        connection_handle_t connection_handle
    ) {
        if (_transaction_timeout_cb) {
            _transaction_timeout_cb(connection_handle);
        }
    }

private:
    /**
     * Callback called when the client receive a message from the server.
     */
    mbed::Callback<void(connection_handle_t, const AttServerMessage&)> _server_message_cb;

    /**
     * Callback called when a transaction times out.
     */
    mbed::Callback<void(connection_handle_t)> _transaction_timeout_cb;

    // Disallow copy construction and copy assignment.
    AttClient(const AttClient&);
    AttClient& operator=(const AttClient&);
};


} // namespace pal
} // namespace ble

#endif /* BLE_PAL_ATTCLIENT_H_ */
