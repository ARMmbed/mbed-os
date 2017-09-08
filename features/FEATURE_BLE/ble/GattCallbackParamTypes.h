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

#ifndef __GATT_CALLBACK_PARAM_TYPES_H__
#define __GATT_CALLBACK_PARAM_TYPES_H__

/**
 * Parameter of the callback invoked on a write operation.
 * This parameter is used whether a GattServer as received a write operation or
 * if the GattClient has completed a write operation.
 *
 * @important The fields connHandle, handle and writeOp are used in both
 * callbacks while:
 *   - offset, len and data are reserved for GattServer write callbacks.
 *   - status and error_code are reserved for GattClient write callbacks.
 */
struct GattWriteCallbackParams {
    /**
     * Enumeration for write operations.
     */
    enum WriteOp_t {
        OP_INVALID               = 0x00,  /**< Invalid operation. */
        OP_WRITE_REQ             = 0x01,  /**< Write request. */
        OP_WRITE_CMD             = 0x02,  /**< Write command. */
        OP_SIGN_WRITE_CMD        = 0x03,  /**< Signed write command. */
        OP_PREP_WRITE_REQ        = 0x04,  /**< Prepare write request. */
        OP_EXEC_WRITE_REQ_CANCEL = 0x05,  /**< Execute write request: cancel all prepared writes. */
        OP_EXEC_WRITE_REQ_NOW    = 0x06,  /**< Execute write request: immediately execute all prepared writes. */
    };

    Gap::Handle_t            connHandle; /**< The handle of the connection that triggered the event. */
    GattAttribute::Handle_t  handle;     /**< Attribute Handle to which the write operation applies. */
    WriteOp_t                writeOp;    /**< Type of write operation. */

    // Note: offset is used in GattServer while status is used in GattClient
    union {
        uint16_t                offset;     /**< Offset for the GattServer write operation. */
        ble_error_t             status;     /**< Status of the GattClient Write operation */
    };

    // Note: len is used in GattServer while error_code is used in GattClient
    union {
        uint16_t                 len;        /**< Length (in bytes) of the data to write (GattServer). */
        uint8_t                  error_code; /**< Error code of the GattClient Write operation */
    };

    /**
     * Pointer to the data to write.
     *
     * @note Data might not persist beyond the callback; make a local copy if
     *       needed.
     * @note This field is not used by callbacks invoked by the GattClient module.
     */
    const uint8_t           *data;
};

/**
 * Parameter of the callback invoked on a read operation.
 * This parameter is used whether a GattServer as received a read operation or
 * if the GattClient has completed a read operation.
 *
 * @important The fields connHandle, handle and offset are used in both
 * callbacks while:
 *   - len and data are reserved for GattServer read callbacks.
 *   - status and error_code are reserved for GattClient read callbacks.
 */
struct GattReadCallbackParams {
    Gap::Handle_t            connHandle; /**< The handle of the connection that triggered the event. */
    GattAttribute::Handle_t  handle;     /**< Attribute Handle to which the read operation applies. */
    uint16_t                 offset;     /**< Offset for the read operation. */
    union {
        uint16_t             len;        /**< Length (in bytes) of the data to read. */
        uint8_t              error_code; /**< Error code if any (GattClient) */
    };

    /**
     * Pointer to the data read.
     *
     * @note Data might not persist beyond the callback; make a local copy if
     *       needed.
     */
    const uint8_t           *data;
    ble_error_t              status;     /**< Status of the operation BLE_ERROR_NONE in case of success or the error in case of error */
};

enum GattAuthCallbackReply_t {
    AUTH_CALLBACK_REPLY_SUCCESS                       = 0x00,    /**< Success. */
    AUTH_CALLBACK_REPLY_ATTERR_INVALID_HANDLE         = 0x0101,  /**< ATT Error: Invalid attribute handle. */
    AUTH_CALLBACK_REPLY_ATTERR_READ_NOT_PERMITTED     = 0x0102,  /**< ATT Error: Read not permitted. */
    AUTH_CALLBACK_REPLY_ATTERR_WRITE_NOT_PERMITTED    = 0x0103,  /**< ATT Error: Write not permitted. */
    AUTH_CALLBACK_REPLY_ATTERR_INSUF_AUTHENTICATION   = 0x0105,  /**< ATT Error: Authenticated link required. */
    AUTH_CALLBACK_REPLY_ATTERR_INVALID_OFFSET         = 0x0107,  /**< ATT Error: The specified offset was past the end of the attribute. */
    AUTH_CALLBACK_REPLY_ATTERR_INSUF_AUTHORIZATION    = 0x0108,  /**< ATT Error: Used in ATT as "insufficient authorization". */
    AUTH_CALLBACK_REPLY_ATTERR_PREPARE_QUEUE_FULL     = 0x0109,  /**< ATT Error: Used in ATT as "prepare queue full". */
    AUTH_CALLBACK_REPLY_ATTERR_ATTRIBUTE_NOT_FOUND    = 0x010A,  /**< ATT Error: Used in ATT as "attribute not found". */
    AUTH_CALLBACK_REPLY_ATTERR_ATTRIBUTE_NOT_LONG     = 0x010B,  /**< ATT Error: Attribute cannot be read or written using read/write blob requests. */
    AUTH_CALLBACK_REPLY_ATTERR_INVALID_ATT_VAL_LENGTH = 0x010D,  /**< ATT Error: Invalid value size. */
    AUTH_CALLBACK_REPLY_ATTERR_INSUF_RESOURCES        = 0x0111,  /**< ATT Error: Encrypted link required. */
};

struct GattWriteAuthCallbackParams {
    Gap::Handle_t            connHandle; /**< The handle of the connection that triggered the event. */
    GattAttribute::Handle_t  handle;     /**< Attribute Handle to which the write operation applies. */
    uint16_t                 offset;     /**< Offset for the write operation. */
    uint16_t                 len;        /**< Length of the incoming data. */
    const uint8_t           *data;       /**< Incoming data, variable length. */
    /**
     * This is the out parameter that the callback needs to set to
     * AUTH_CALLBACK_REPLY_SUCCESS for the request to proceed.
     */
    GattAuthCallbackReply_t  authorizationReply;
};

struct GattReadAuthCallbackParams {
    Gap::Handle_t            connHandle; /**< The handle of the connection that triggered the event. */
    GattAttribute::Handle_t  handle;     /**< Attribute Handle to which the read operation applies. */
    uint16_t                 offset;     /**< Offset for the read operation. */
    uint16_t                 len;        /**< Optional: new length of the outgoing data. */
    uint8_t                 *data;       /**< Optional: new outgoing data. Leave at NULL if data is unchanged. */
    /**
     * This is the out parameter that the callback needs to set to
     * AUTH_CALLBACK_REPLY_SUCCESS for the request to proceed.
     */
    GattAuthCallbackReply_t  authorizationReply;
};

/**
 * For encapsulating handle-value update events (notifications or indications)
 * generated at the remote server.
 */
struct GattHVXCallbackParams {
  Gap::Handle_t            connHandle; /**< The handle of the connection that triggered the event. */
  GattAttribute::Handle_t  handle;     /**< Attribute Handle to which the HVx operation applies. */
  HVXType_t                type;       /**< Indication or Notification, see HVXType_t. */
  uint16_t                 len;        /**< Attribute data length. */
  const uint8_t           *data;       /**< Attribute data, variable length. */
};

#endif /*__GATT_CALLBACK_PARAM_TYPES_H__*/
