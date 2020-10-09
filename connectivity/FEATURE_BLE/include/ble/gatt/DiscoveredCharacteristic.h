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

#ifndef MBED_DISCOVERED_CHARACTERISTIC_H__
#define MBED_DISCOVERED_CHARACTERISTIC_H__

#include "ble/common/UUID.h"
#include "ble/gatt/GattAttribute.h"
#include "ble/gatt/GattCallbackParamTypes.h"
#include "ble/gatt/CharacteristicDescriptorDiscovery.h"
#include "ble/gatt/DiscoveredCharacteristicDescriptor.h"

namespace ble {
class GattClient;
}

/**
 * @addtogroup ble
 * @{
 * @addtogroup gatt
 * @{
 * @addtogroup client
 * @{
 */

/**
 * Representation of a characteristic discovered.
 *
 * The ble::GattClient discovery procedure initiated with
 * ble::GattClient::launchServiceDiscovery() generates instances of this class.
 *
 * It exposes the main attributes of the discovered characteristic:
 *   - The UUID of the characteristic, it can be retrieved by a call to the
 *     function getUUID(). This UUID is the type of the characteristic.
 *   - Attribute Handles of the characteristic are present as the triplet
 *     declaration handle, value handle and last handle. The value handle is
 *     used to read or write the content of the characteristic.
 *   - The properties contain the set of operations the characteristic can
 *     handle, for instance being read or written.
 *
 * It important to note that the value of the characteristic - if it is
 * accessible - is not fetched at discovery time.
 *
 * The main operations the class offers are reading, writing and discovering
 * the descriptors of the characteristic discovered.
 *
 * Reading a discovered characteristic can be accomplished in two different
 * fashions:
 *
 * If the user has a callback registered for the data read operation in the
 * ble::GattClient, then a call to the read(uint16_t) function will initiate a read of
 * the characteristic. Results of the operation will be pass on the callback
 * registered by ble::GattClient::onDataRead(), which processes all the responses to
 * read requests. The read request for a given characteristic can be identified
 * by the connection handle and the attribute handle, which are present in
 * GattReadCallbackParams.
 *
 * Another overload (read(uint16_t, const ble::ReadCallback_t&)) of the
 * read function accepts a completion callback as a last parameter. That
 * completion callback will be invoked automatically once the response to the
 * read request for that given characteristic has been received. However,
 * convenience came at the expense of dynamic memory usage for the time of the
 * transaction.
 *
 * Similarly, two versions of the write() API are exposed. One where the user
 * has to register a callback handling write response through the function
 * ble::GattClient::onDataWritten() and another one that accepts a completion
 * callback in input.
 *
 * It is also possible to send a write command, which is not acknowledged by the
 * peer server by using the function writeWoResponse().
 *
 * Finally, descriptors of the characteristic can be discovered by invoking the
 * function discoverDescriptors, which is shorthand for calling
 * ble::GattClient::discoverCharacteristicDescriptors. That discovery is necessary to
 * enable or disable characteristic notification or indication that is achieved
 * by writing on the Client Characteristic Configuration Descriptor (CCCD).
 */
class DiscoveredCharacteristic {
public:
    /**
     * Properties of a discovered characteristic.
     */
    struct Properties_t {
        /**
         * Permits broadcasts of the characteristic value using the character
         * the Server Characteristic Configuration Descriptor.
         *
         * @note If set, descriptors of the characteristic contain a Server
         * Characteristic Configuration Descriptor.
         */
        uint8_t _broadcast :1;

        /**
         * If set, the value of the characteristic can be read.
         */
        uint8_t _read :1;

        /**
         * If set, a write command can write the characteristic value
         * (write without response).
         */
        uint8_t _writeWoResp :1;

        /**
         * If set, clients can issue requests to write the characteristic.
         */
        uint8_t _write :1;

        /**
         * If set, the server can emit notifications of the Characteristic Value
         * (without client acknowledgment).
         *
         * @note If set, descriptors of the characteristic contain a Client
         * Characteristic Configuration Descriptor.
         */
        uint8_t _notify :1;

        /**
         * If set, the server can emit indication of the Characteristic Value
         * (with client acknowledgement).
         *
         * @note If set, descriptors of the characteristic contain a Client
         * Characteristic Configuration Descriptor.
         */
        uint8_t _indicate :1;

        /**
         * If set, signed write of the Characteristic Value is supported.
         */
        uint8_t _authSignedWrite :1;

    public:
        /**
         * Return the value of the broadcast propertie.
         *
         * @return true if the Server Characteristic Configuration Descriptor
         * of the characteristic can be configured to broadcast the
         * characteristic value during broadcast procedure.
         *
         * @see _broadcast
         */
        bool broadcast() const
        {
            return _broadcast;
        }

        /**
         * Return the value of the read property
         *
         * @return true if the characteristic value can be read and false
         * otherwise.
         *
         * @see _read
         */
        bool read() const
        {
            return _read;
        }

        /**
         * Return the value of the write without response property.
         *
         * @return true if the characteristic accepts write without response
         * commands and false otherwise.
         *
         * @see _writeWoResp
         */
        bool writeWoResp() const
        {
            return _writeWoResp;
        }

        /**
         * Return the value of the write property.
         *
         * @return true if writing the characteristic accepts write requests and
         * false otherwise.
         *
         * @see _write
         */
        bool write() const
        {
            return _write;
        }

        /**
         * Return the value of the notification property.
         *
         * @return true if the Client Characteristic Configuration Descriptor
         * can be configured to notify the characteristic value to a given
         * client and false otherwise.
         *
         * @note unlike indication, the notification procedure does not require
         * acknowledgement from the client.
         *
         * @see _notify
         */
        bool notify() const
        {
            return _notify;
        }

        /**
         * Return the value of the indicate property.
         *
         * @return true if the Client Characteristic Configuration Descriptor
         * can be configured to indicate the characteristic value to a given
         * client and false otherwise.
         *
         * @note unlike notification the indication procedure does require
         * acknowledgment from the client.
         *
         * @see _indicate
         */
        bool indicate() const
        {
            return _indicate;
        }

        /**
         * Return the value of the authenticated signed writes property.
         *
         * @return true if the characteristic accepts authenticated signed write
         * and false otherwise.
         */
        bool authSignedWrite() const
        {
            return _authSignedWrite;
        }

        /**
         * Equal to operator for DiscoveredCharacteristic::Properties_t.
         *
         * @param[in] lhs The left hand side of the equality expression.
         * @param[in] rhs The right hand side of the equality expression.
         *
         * @return true if operands are equals and false otherwise.
         */
        friend bool operator==(Properties_t lhs, Properties_t rhs)
        {
            return lhs._broadcast == rhs._broadcast &&
                   lhs._read == rhs._read &&
                   lhs._writeWoResp == rhs._writeWoResp &&
                   lhs._write == rhs._write &&
                   lhs._notify == rhs._notify &&
                   lhs._indicate == rhs._indicate &&
                   lhs._authSignedWrite == rhs._authSignedWrite;
        }

        /**
         * Not equal to operator for DiscoveredCharacteristic::Properties_t.
         *
         * @param lhs The left hand side of the expression.
         * @param rhs The right hand side of the expression.
         *
         * @return true if operands are not equals, false otherwise.
         */
        friend bool operator!=(Properties_t lhs, Properties_t rhs)
        {
            return !(lhs == rhs);
        }

    private:
        /* Disallow implicit conversion to integer types. */
        operator uint8_t() const;
        operator unsigned() const;
    };

    /**
     * Initiate a read of the characteristic value.
     *
     * The characteristic value is read in its entirety from the value attribute
     * of the characteristic.
     *
     * Read responses will be passed to the callback registered in
     * ble::GattClient::onDataRead(). Read responses to read requests that this function
     * call initiates will have their GattReadCallbackParams::connHandle
     * field equal to the value returned by getConnectionHandle() and their
     * GattReadCallbackParams::handle field equal to the value returned by
     * getValueHandle().
     *
     * @param[in] offset The position - in the characteristic value bytes stream
     * - where the read operation begin. This parameter is optional.
     *
     * @return BLE_ERROR_NONE if a read has been initiated.
     * @return BLE_ERROR_INVALID_STATE if some internal state about the
     * connection is invalid.
     * @return BLE_STACK_BUSY if some client procedure is already in progress.
     * @return BLE_ERROR_OPERATION_NOT_PERMITTED due to the characteristic's
     * properties.
     */
    ble_error_t read(uint16_t offset = 0) const;

    /**
     * Initiate a read of the characteristic value and pass the response to its
     * completion callback.
     *
     * @param[in] offset The position - in the characteristic value bytes stream
     * - where the read operation begin.
     *
     * @param[in] onRead Completion callback which will accept the response of
     * the read request. The callback is copied; it is unnecessary to keep it
     * in memory after the call.
     *
     * @return BLE_ERROR_NONE if a read has been initiated.
     * @return BLE_ERROR_INVALID_STATE if some internal state about the
     * connection is invalid.
     * @return BLE_STACK_BUSY if some client procedure is already in progress.
     * @return BLE_ERROR_OPERATION_NOT_PERMITTED due to the characteristic's
     * properties.
     *
     * @note This function is similar to read(uint16_t) const; however, it uses
     * dynamic memory to store the use completion callback.
     */
    ble_error_t read(
        uint16_t offset,
        const ble::ReadCallback_t &onRead
    ) const;

    /**
     * Perform a write without response procedure.
     *
     * @note The server does not acknowledge write without responses.
     * Therefore, they won't generate any event on the client side.
     *
     * @param[in] length The amount of data being written.
     * @param[in] value The bytes being written.
     *
     * @return BLE_ERROR_NONE Successfully started the Write procedure.
     * @return BLE_ERROR_INVALID_STATE if some internal state about the
     * connection is invalid.
     * @return BLE_STACK_BUSY if some client procedure is already in progress.
     * @return BLE_ERROR_NO_MEM if there are no available buffers left to
     * process the request.
     * @return BLE_ERROR_OPERATION_NOT_PERMITTED due to the characteristic's
     * properties.
     */
    ble_error_t writeWoResponse(uint16_t length, const uint8_t *value) const;

    /**
     * Initiate a discovery of the characteristic descriptors.
     *
     * When a descriptor is discovered, the callback onDescriptorDiscovered is
     * invoked with the descriptor discovered as parameter. When the process
     * ends, the callback onTermination is invoked.
     *
     * @param[in] onDescriptorDiscovered Callback is invoked when a descriptor is
     * discovered.
     *
     * @param[in] onTermination Callback is invoked when the discovery process ends.
     *
     * @return BLE_ERROR_NONE if descriptor discovery is launched successfully;
     * else an appropriate error.
     *
     * @note This function is shorthand for
     * ble::GattClient::discoverCharacteristicDescriptors; therefore,
     * ble::GattClient::isCharacteristicDescriptorDiscoveryActive can be used to
     * determine the descriptor discovery and
     * ble::GattClient::terminateCharacteristicDescriptorDiscovery can be used to
     * end the discovery process.
     */
    ble_error_t discoverDescriptors(
        const CharacteristicDescriptorDiscovery::DiscoveryCallback_t &onDescriptorDiscovered,
        const CharacteristicDescriptorDiscovery::TerminationCallback_t &onTermination
    ) const;

    /**
     * Initiate a write procedure of the characteristic value.
     *
     * Unlike write without responses (see writeWoResponse()), an acknowledgment
     * is expected for this procedure. The response of the peer GATT server to
     * the write request is passed to callbacks registered in
     * ble::GattClient::onDataWritten().
     *
     * Similarly to read responses, responses to write request of this
     * characteristic can be identified by their connection handle (
     * GattWriteCallbackParams::connHandle), which is equal to the value
     * returned by getConnectionHandle() and their attribute handle (
     * GattWriteCallbackParams::handle), which is equal to the value
     * returned by getValueHandle().
     *
     * @param[in] length The amount of data being written.
     * @param[in] value The bytes being written.
     *
     * @return BLE_ERROR_NONE Successfully started the Write procedure.
     * @return BLE_ERROR_INVALID_STATE If some internal state about the
     * connection is invalid.
     * @return BLE_STACK_BUSY If some client procedure is already in progress.
     * @return BLE_ERROR_NO_MEM If there are no available buffers left to
     * process the request.
     * @return BLE_ERROR_OPERATION_NOT_PERMITTED due to the characteristic's
     * properties.
     *
     * @note Internally, the API uses the write or long write procedure, depending
     * on the number of bytes to write and the MTU size.
     */
    ble_error_t write(uint16_t length, const uint8_t *value) const;

    /**
     * Initiate a write procedure of the characteristic value.
     *
     * Same as write(uint16_t, const uint8_t *) const but accepts a completion
     * callback, which is invoked when the server response is received.
     *
     * @param[in] length The amount of bytes to write.
     * @param[in] value The bytes to write.
     * @param[in] onWrite Continuation callback of the write procedure.
     *
     * @return BLE_ERROR_NONE Successfully started the Write procedure.
     * @return BLE_ERROR_INVALID_STATE if some internal state about the
     * connection is invalid.
     * @return BLE_STACK_BUSY if some client procedure is already in progress.
     * @return BLE_ERROR_NO_MEM if there are no available buffers left to
     * process the request.
     * @return BLE_ERROR_OPERATION_NOT_PERMITTED due to the characteristic's
     * properties.
     */
    ble_error_t write(
        uint16_t length,
        const uint8_t *value,
        const ble::WriteCallback_t &onWrite
    ) const;

    void setupLongUUID(UUID::LongUUIDBytes_t longUUID, UUID::ByteOrder_t order = UUID::MSB) {
        uuid.setupLong(longUUID, order);
    }

public:
    /**
     * Get the UUID of the discovered characteristic.
     *
     * @return The UUID of this characteristic.
     */
    const UUID &getUUID() const
    {
        return uuid;
    }

    /**
     * Get the properties of this characteristic.
     *
     * @return The set of properties of this characteristic.
     */
    const Properties_t &getProperties() const
    {
        return props;
    }

    /**
     * Get the declaration handle of this characteristic.
     *
     * The declaration handle is the first handle of a characteristic
     * definition. The value accessible at this handle contains the following
     * informations:
     *    - The characteristics properties (see Properties_t). This value can
     *      be accessed by using #getProperties .
     *    - The characteristic value attribute handle. This field can be accessed
     *      by using #getValueHandle .
     *    - The characteristic UUID, this value can be accessed by using the
     *      function #getUUID .
     *
     * @return the declaration handle of this characteristic.
     */
    GattAttribute::Handle_t getDeclHandle() const
    {
        return declHandle;
    }

    /**
     * Get the attribute handle of the characteristic value.
     *
     * This handle is used to read or write the value of the characteristic.
     *
     * @return The handle to access the value of this characteristic.
     */
    GattAttribute::Handle_t getValueHandle() const
    {
        return valueHandle;
    }

    /**
     * Return the last attribute handle of the characteristic definition.
     *
     * The attribute layout of a characteristic definition is:
     *   - Declaration attribute (see #getDeclHandle).
     *   - Value attribute (see #getValueHandle).
     *   - Zero or more characteristic descriptors attribute.
     *
     * The last attribute handle is used internally to discover characteristic
     * descriptors. The discovery operates on the range [ValueHandle + 1 :
     * LastHandle].
     *
     * @return The last handle of this characteristic definition.
     *
     * @note This function is public for informative purposes.
     */
    GattAttribute::Handle_t getLastHandle() const
    {
        return lastHandle;
    }

    /**
     * Get the ble::GattClient, which can operate on this characteristic.
     *
     * @return The ble::GattClient, which can operate on this characteristic.
     */
    ble::GattClient* getGattClient()
    {
        return gattc;
    }

    /**
     * Get the ble::GattClient, which can operate on this characteristic.
     *
     * @return The ble::GattClient, which can operate on this characteristic.
     */
    const ble::GattClient* getGattClient() const
    {
        return gattc;
    }

    /**
     * @brief Get the connection handle to the GattServer containing this
     * characteristic.
     *
     * @return Connection handle to the GattServer, which contains this
     * characteristic.
     */
    ble::connection_handle_t getConnectionHandle() const
    {
        return connHandle;
    }

    /**
     * "Equal to" operator for DiscoveredCharacteristic.
     *
     * @param[in] lhs The left hand side of the equality expression.
     * @param[in] rhs The right hand side of the equality expression.
     *
     * @return true if operands are equals and false otherwise.
     */
    friend bool operator==(
        const DiscoveredCharacteristic& lhs, const DiscoveredCharacteristic& rhs
    ) {
        return lhs.gattc == rhs.gattc &&
               lhs.uuid == rhs.uuid &&
               lhs.props == rhs.props &&
               lhs.declHandle == rhs.declHandle &&
               lhs.valueHandle == rhs.valueHandle &&
               lhs.lastHandle == rhs.lastHandle &&
               lhs.connHandle == rhs.connHandle;
    }

    /**
     * "Not equal to" operator for DiscoveredCharacteristic.
     *
     * @param[in] lhs The right hand side of the expression.
     * @param[in] rhs The left hand side of the expression.
     *
     * @return true if operands are not equal and false otherwise.
     */
    friend bool operator !=(
        const DiscoveredCharacteristic& lhs, const DiscoveredCharacteristic& rhs
    ) {
        return !(lhs == rhs);
    }

public:
    DiscoveredCharacteristic() :
        gattc(nullptr),
        uuid(UUID::ShortUUIDBytes_t(0)),
        props(),
        declHandle(GattAttribute::INVALID_HANDLE),
        valueHandle(GattAttribute::INVALID_HANDLE),
        lastHandle(GattAttribute::INVALID_HANDLE),
        connHandle() {
    }

protected:
    /**
     * Pointer to the underlying ble::GattClient for this DiscoveredCharacteristic
     * object.
     */
    ble::GattClient *gattc;

protected:
    /**
     * Discovered characteristic's UUID.
     */
    UUID uuid;

    /**
     * Hold the configured properties of the discovered characteristic.
     *
     * @see Properties_t.
     */
    Properties_t props;

    /**
     * Value handle of the discovered characteristic's declaration attribute.
     */
    GattAttribute::Handle_t declHandle;

    /**
     * Value handle of the discovered characteristic's value attribute.
     */
    GattAttribute::Handle_t valueHandle;

    /**
     * Value handle of the discovered characteristic's last attribute.
     */
    GattAttribute::Handle_t lastHandle;

    /**
     * Handle of the connection where the characteristic was discovered.
     */
    ble::connection_handle_t connHandle;
};

/**
 * @}
 * @}
 * @}
 */

#endif /*MBED_DISCOVERED_CHARACTERISTIC_H__*/
