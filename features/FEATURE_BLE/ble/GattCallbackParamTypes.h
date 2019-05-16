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

    /**
     * ATT Error: Invalid attribute handle.
     */
    AUTH_CALLBACK_REPLY_ATTERR_INVALID_HANDLE = 0x0101,

    /**
     * ATT Error: Read not permitted.
     */
    AUTH_CALLBACK_REPLY_ATTERR_READ_NOT_PERMITTED = 0x0102,

    /**
     * ATT Error: Write not permitted.
     */
    AUTH_CALLBACK_REPLY_ATTERR_WRITE_NOT_PERMITTED = 0x0103,

    /**
     * ATT Error: Authenticated link required.
     */
    AUTH_CALLBACK_REPLY_ATTERR_INSUF_AUTHENTICATION = 0x0105,

    /**
     * ATT Error: The specified offset was past the end of the attribute.
     */
    AUTH_CALLBACK_REPLY_ATTERR_INVALID_OFFSET = 0x0107,

    /**
     * ATT Error: Used in ATT as "insufficient authorization".
     */
    AUTH_CALLBACK_REPLY_ATTERR_INSUF_AUTHORIZATION = 0x0108,

    /**
     * ATT Error: Used in ATT as "prepare queue full".
     */
    AUTH_CALLBACK_REPLY_ATTERR_PREPARE_QUEUE_FULL = 0x0109,

    /**
     * ATT Error: Used in ATT as "attribute not found".
     */
    AUTH_CALLBACK_REPLY_ATTERR_ATTRIBUTE_NOT_FOUND = 0x010A,

    /**
     * ATT Error: Attribute cannot be read or written using read/write blob
     * requests.
     */
    AUTH_CALLBACK_REPLY_ATTERR_ATTRIBUTE_NOT_LONG = 0x010B,

    /**
     * ATT Error: Invalid value size.
     */
    AUTH_CALLBACK_REPLY_ATTERR_INVALID_ATT_VAL_LENGTH = 0x010D,

    /**
     * ATT Error: Encrypted link required.
     */
    AUTH_CALLBACK_REPLY_ATTERR_INSUF_RESOURCES = 0x0111,
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
 * @}
 * @}
 * @}
 */

#endif /*MBED_BLE_GATT_CALLBACK_PARAM_TYPES_H__*/
