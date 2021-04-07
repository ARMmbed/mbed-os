/* mbed Microcontroller Library
 * Copyright (c) 2006-2020 ARM Limited
 *
 * SPDX-License-Identifier: Apache-2.0
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
#include "ble/common/CallChainOfFunctionPointersWithContext.h"

#ifndef MBED_BLE_GATT_CALLBACK_PARAM_TYPES_H__
#define MBED_BLE_GATT_CALLBACK_PARAM_TYPES_H__

/**
 * @addtogroup ble
 * @{
 * @addtogroup gatt
 * @{
 */

/**
 * GATT Write event definition.
 *
 * Instances of this type are created and passed to user registered callbacks
 * whether the GattServer has received a write request or a GattClient has
 * received a write response.
 *
 * @attention The GattServer only populates the fields offset, len and data
 * when it has received a write request. Callbacks attached to the GattClient
 * do not use those fields.
 *
 * @attention The GattClient only populates the fields status and error_code
 * when it has received a write response. Callbacks attached to the GattServer
 * do not use those fields.
 */
struct GattWriteCallbackParams {
    /**
     * Enumeration of allowed write operations.
     */
    enum WriteOp_t {
        /**
         * Invalid operation.
         */
        OP_INVALID = 0x00,

        /**
         * Write request.
         */
        OP_WRITE_REQ = 0x01,

        /**
         * Write command.
         */
        OP_WRITE_CMD = 0x02,

        /**
         * Signed write command.
         */
        OP_SIGN_WRITE_CMD = 0x03,

        /**
         * Prepare write request.
         */
        OP_PREP_WRITE_REQ = 0x04,

        /**
         * Execute write request: cancel all prepared writes.
         */
        OP_EXEC_WRITE_REQ_CANCEL = 0x05,

        /**
         * Execute write request: immediately execute all prepared writes.
         */
        OP_EXEC_WRITE_REQ_NOW = 0x06,
    };

    /**
     * Handle of the connection that triggered the event.
     */
    ble::connection_handle_t connHandle;

    /**
     * Handle of the attribute to which the write operation applies.
     */
    GattAttribute::Handle_t handle;

    /**
     * Type of the write operation.
     */
    WriteOp_t writeOp;

    union {
        /**
         * Offset within the attribute value to be written.
         *
         * @attention Reserved for GattServer registered callbacks.
         */
        uint16_t offset;

        /**
         * Status of the GattClient Write operation.
         *
         * @attention Reserved for GattClient registered callbacks.
         */
        ble_error_t status;
    };

    union {
        /**
         * Length (in bytes) of the data to write.
         *
         * @attention Reserved for GattServer registered callbacks.
         */
        uint16_t len;

        /**
         * Error code of the GattClient Write operation.
         *
         * @attention Reserved for GattClient registered callbacks.
         */
        uint8_t error_code;
    };

    /**
     * Pointer to the data to write.
     *
     * @attention Data may not persist beyond the callback scope.
     *
     * @attention Reserved for GattServer registered callbacks.
     */
    const uint8_t *data;
};

/**
 * GATT Read event definition.
 *
 * Instances of this type are created and passed to user registered callbacks
 * whether the GattServer has received a read request or a GattClient has
 * received a read response.
 *
 * @attention The GattClient only populates the fields status and error_code
 * when it has received a read response. Callbacks attached to the GattServer
 * do not use those fields.
 */
struct GattReadCallbackParams {
    /**
     * Handle of the connection that triggered the event.
     */
    ble::connection_handle_t connHandle;

    /**
     * Attribute Handle to which the read operation applies.
     */
    GattAttribute::Handle_t handle;

    /**
     * Offset within the attribute value read.
     */
    uint16_t offset;

    union {
        /**
         * Length in bytes of the data read.
         */
        uint16_t len;

        /**
         * Error code of the GattClient read operation.
         *
         * @attention Reserved for GattClient registered callbacks.
         *
         * @attention set if status is not equal to BLE_ERROR_NONE; otherwise,
         * this field is interpreted as len.
         */
        uint8_t error_code;
    };

    /**
     * Pointer to the data read.
     *
     * @attention Data may not persist beyond the callback scope.
     */
    const uint8_t *data;

    /**
     * Status of the GattClient Read operation.
     *
     * @attention Reserved for GattClient registered callbacks.
     */
    ble_error_t status;
};

/**
 * @addtogroup server
 * @{
 */

/**
 * Enumeration of allowed values returned by read or write authorization process.
 */
enum GattAuthCallbackReply_t {
    /**
     * Success.
     */
    AUTH_CALLBACK_REPLY_SUCCESS = 0x00,

    /** The attribute handle given was not valid on this server. */
    AUTH_CALLBACK_REPLY_ATTERR_INVALID_HANDLE = 0x01,

    /** The attribute cannot be read. */
    AUTH_CALLBACK_REPLY_ATTERR_READ_NOT_PERMITTED = 0x02,

    /** The attribute cannot be written. */
    AUTH_CALLBACK_REPLY_ATTERR_WRITE_NOT_PERMITTED = 0x03,

    /** The attribute PDU was invalid. */
    AUTH_CALLBACK_REPLY_ATTERR_INVALID_PDU = 0x04,

    /** The attribute requires authentication before it can be read or
     * written.
     */
    AUTH_CALLBACK_REPLY_ATTERR_INSUFFICIENT_AUTHENTICATION = 0x05,
    AUTH_CALLBACK_REPLY_ATTERR_INSUF_AUTHENTICATION = 0x05,

    /** Attribute server does not support the request received from the
     * client.
     */
    AUTH_CALLBACK_REPLY_ATTERR_REQUEST_NOT_SUPPORTED = 0x06,

    /** Offset specified was past the end of the attribute. */
    AUTH_CALLBACK_REPLY_ATTERR_INVALID_OFFSET = 0x07,

    /** The attribute requires authorization before it can be read or written. */
    AUTH_CALLBACK_REPLY_ATTERR_INSUFFICIENT_AUTHORIZATION = 0x08,
    AUTH_CALLBACK_REPLY_ATTERR_INSUF_AUTHORIZATION = 0x08,

    /** Too many prepare writes have been queued. */
    AUTH_CALLBACK_REPLY_ATTERR_PREPARE_QUEUE_FULL = 0x09,

    /** No attribute found within the given attribute handle range. */
    AUTH_CALLBACK_REPLY_ATTERR_ATTRIBUTE_NOT_FOUND = 0x0A,

    /** The attribute cannot be read using the Read Blob Request. */
    AUTH_CALLBACK_REPLY_ATTERR_ATTRIBUTE_NOT_LONG = 0x0B,

    /** The Encryption Key Size used for encrypting this link is
     * insufficient.
     */
    AUTH_CALLBACK_REPLY_ATTERR_INSUFFICIENT_ENCRYPTION_KEY_SIZE = 0x0C,

    /** The attribute value length is invalid for the operation. */
    AUTH_CALLBACK_REPLY_ATTERR_INVALID_ATTRIBUTE_VALUE_LENGTH = 0x0D,
    AUTH_CALLBACK_REPLY_ATTERR_INVALID_ATT_VAL_LENGTH = 0x0D,

    /** The attribute request that was requested has encountered an error
     * that was unlikely, and therefore could not be completed as requested.
     */
    AUTH_CALLBACK_REPLY_ATTERR_UNLIKELY_ERROR = 0x0E,

    /** The attribute requires encryption before it can be read or written. */
    AUTH_CALLBACK_REPLY_ATTERR_INSUFFICIENT_ENCRYPTION = 0x0F,

    /** The attribute type is not a supported grouping attribute as defined
     * by a higher layer specification.
     */
    AUTH_CALLBACK_REPLY_ATTERR_UNSUPPORTED_GROUP_TYPE = 0x10,

    /** Insufficient Resources to complete the request. */
    AUTH_CALLBACK_REPLY_ATTERR_INSUFFICIENT_RESOURCES = 0x11,
    AUTH_CALLBACK_REPLY_ATTERR_INSUF_RESOURCES = 0x11,

    /* 0x12 - 0x7F => reserved for future use */

    /* 0x80 - 0x9F => Application Error */

    /* 0xA0 0xDF => Reserved for future use */

    /* 0xE0 - 0xFF Common Profile and service Error Codes */

    /** The Write Request Rejected error code is used when a requested write
     * operation cannot be fulfilled for reasons other than permissions.
     */
    AUTH_CALLBACK_REPLY_ATTERR_WRITE_REQUEST_REJECTED = 0xFC,

    /** The Client Characteristic Configuration Descriptor Improperly
     * Configured error code is used when a Client Characteristic
     * Configuration descriptor is not configured according to the
     * requirements of the profile or service.
     */
    AUTH_CALLBACK_REPLY_ATTERR_CLIENT_CHARACTERISTIC_CONFIGURATION_DESCRIPTOR_IMPROPERLY_CONFIGURED = 0xFD,

    /** The Procedure Already in Progress error code is used when a profile
     * or service request cannot be serviced because an operation that has
     * been previously triggered is still in progress
     */
    AUTH_CALLBACK_REPLY_ATTERR_PROCEDURE_ALREADY_IN_PROGRESS = 0xFE,

    /** The Out of Range error code is used when an attribute value is out
     * of range as defined by a profile or service specification.
     */
    AUTH_CALLBACK_REPLY_ATTERR_OUT_OF_RANGE = 0xFF,
};

/**
 * GATT write authorization request event.
 */
struct GattWriteAuthCallbackParams {
    /**
     * Handle of the connection that triggered the event.
     */
    ble::connection_handle_t connHandle;

    /**
     * Attribute Handle to which the write operation applies.
     */
    GattAttribute::Handle_t handle;

    /**
     * Offset for the write operation.
     */
    uint16_t offset;

    /**
     * Length of the incoming data.
     */
    uint16_t len;

    /**
     * Incoming data.
     */
    const uint8_t *data;

    /**
     * Authorization result.
     *
     * The callback sets this parameter. If the value is set to
     * AUTH_CALLBACK_REPLY_SUCCESS, then the write request is accepted;
     * otherwise, an error code is returned to the peer client.
     */
    GattAuthCallbackReply_t authorizationReply;
};

/**
 * GATT read authorization request event.
 */
struct GattReadAuthCallbackParams {
    /**
     * The handle of the connection that triggered the event.
     */
    ble::connection_handle_t connHandle;

    /**
     * Attribute Handle to which the read operation applies.
     */
    GattAttribute::Handle_t handle;

    /**
     * Offset for the read operation.
     */
    uint16_t offset;

    /**
     * Optional: new length of the outgoing data.
     */
    uint16_t len;

    /**
     * Optional: new outgoing data. Leave at NULL if data is unchanged.
     */
    uint8_t *data;

    /**
     * Authorization result.
     *
     * The callback sets this parameter. If the value is set to
     * AUTH_CALLBACK_REPLY_SUCCESS, then the read request is accepted;
     * otherwise, an error code is returned to the peer client.
     */
    GattAuthCallbackReply_t authorizationReply;
};

/**
 * Handle Value Notification/Indication event.
 *
 * The GattClient generates this type of event upon the reception of a
 * Handle Value Notification or Indication.
 *
 * The event is passed to callbacks registered by GattClient::onHVX().
 */
struct GattHVXCallbackParams {
    /**
     * The handle of the connection that triggered the event.
     */
    ble::connection_handle_t connHandle;

    /**
     * Attribute Handle to which the HVx operation applies.
     */
    GattAttribute::Handle_t handle;

    /**
     * Indication or Notification, see HVXType_t.
     */
    HVXType_t type;

    /**
     * Attribute value length.
     */
    uint16_t len;

    /**
     * Attribute value.
     */
    const uint8_t *data;

};

/**
 * Gatt Data Sent Attribute related events
 *
 * Used by `onDataSent`
 */
struct GattDataSentCallbackParams {

    /**
     * The handle of the connection that triggered the event.
     */
    ble::connection_handle_t connHandle;

    /**
     * Attribute Handle to which the event applies
     */
    GattAttribute::Handle_t attHandle;

};

/**
 * Gatt Updates Changed Attribute related events
 *
 * Used by `onUpdatesEnabled` and 'onUpdatesDisabled'
 */
struct GattUpdatesChangedCallbackParams {

    /**
     * The handle of the connection that triggered the event.
     */
    ble::connection_handle_t connHandle;

    /**
     * The handle of the CCCD producing the event
     */
    GattAttribute::Handle_t attHandle;

    /**
     * The value handle of the characteristic containing the CCCD  
     */
    GattAttribute::Handle_t charHandle;
    
};

using GattUpdatesEnabledCallbackParams       = GattUpdatesChangedCallbackParams;
using GattUpdatesDisabledCallbackParams      = GattUpdatesChangedCallbackParams;
using GattConfirmationReceivedCallbackParams = GattDataSentCallbackParams;

namespace ble {

/**
 * Attribute read event handler.
 *
 * @see GattClient::onDataRead().
 */
typedef FunctionPointerWithContext<const GattReadCallbackParams *> ReadCallback_t;

/**
 * Callchain of attribute read event handlers.
 */
typedef CallChainOfFunctionPointersWithContext<const GattReadCallbackParams *> ReadCallbackChain_t;

/**
 * Attribute write event handler.
 *
 * @see GattClient::onDataWrite().
 */
typedef FunctionPointerWithContext<const GattWriteCallbackParams *> WriteCallback_t;

/**
 * Callchain of attribute write event handlers.
 *
 * @see GattClient::onDataWrite().
 */
typedef CallChainOfFunctionPointersWithContext<const GattWriteCallbackParams *> WriteCallbackChain_t;

}

/**
 * @}
 * @}
 * @}
 */

#endif /*MBED_BLE_GATT_CALLBACK_PARAM_TYPES_H__*/
