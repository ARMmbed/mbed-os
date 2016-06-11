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

#ifndef __DISCOVERED_CHARACTERISTIC_H__
#define __DISCOVERED_CHARACTERISTIC_H__

#include "UUID.h"
#include "Gap.h"
#include "GattAttribute.h"
#include "GattClient.h"
#include "CharacteristicDescriptorDiscovery.h"
#include "ble/DiscoveredCharacteristicDescriptor.h"

/**
 * @brief Representation of a characteristic discovered during a GattClient
 * discovery procedure (see GattClient::launchServiceDiscovery ).
 *
 * @details Provide detailed informations about a discovered characteristic like:
 *     - Its UUID (see getUUID()).
 *     - The most important handles of the characteristic definition
 *       (see getDeclHandle(), getValueHandle(), getLastHandle())
 *     - Its properties (see getProperties()).
 * This class also provide functions to operate on the characteristic:
 *     - Read the characteristic value (see read())
 *     - Writing a characteristic value (see write() or writeWoResponse())
 *     - Discover descriptors inside the characteristic definition. These descriptors
 *       extends the characteristic. More information about descriptor usage is
 *       available in DiscoveredCharacteristicDescriptor class.
 */
class DiscoveredCharacteristic {
public:
    /**
     * Structure that encapsulates the properties of a discovered
     * characteristic.
     */
    struct Properties_t {
        uint8_t _broadcast       :1; /**< Broadcasting the value permitted. */
        uint8_t _read            :1; /**< Reading the value permitted. */
        uint8_t _writeWoResp     :1; /**< Writing the value with Write Command permitted. */
        uint8_t _write           :1; /**< Writing the value with Write Request permitted. */
        uint8_t _notify          :1; /**< Notifications of the value permitted. */
        uint8_t _indicate        :1; /**< Indications of the value permitted. */
        uint8_t _authSignedWrite :1; /**< Writing the value with Signed Write Command permitted. */

    public:
        /**
         * @brief   Check if broadcasting is permitted.
         *
         * @return  true if broadcasting the value is permitted, and false
         *          otherwise.
         */
        bool broadcast(void) const {
            return _broadcast;
        }

        /**
         * @brief   Check reading is permitted.
         *
         * @return  true if reading the value is permitted, and false
         *          otherwise.
         */
        bool read(void) const {
            return _read;
        }

        /**
         * @brief   Check if writing with Write Command is permitted.
         *
         * @return  true if writing the value with Write Command is permitted,
         *          false otherwise.
         */
        bool writeWoResp(void) const {
            return _writeWoResp;
        }

        /**
         * @brief   Check if writing with Write Request is permitted.
         *
         * @return  true if writing the value with Write Request is permitted,
         *          false otherwise.
         */
        bool write(void) const {
            return _write;
        }

        /**
         * @brief   Check notifications are permitted.
         *
         * @return  true if notifications of the value are permitted, false
         *          otherwise.
         */
        bool notify(void) const {
            return _notify;
        }

        /**
         * @brief   Check if indications are permitted.
         *
         * @return  true if indications of the value are permitted, false
         *          otherwise.
         */
        bool indicate(void) const {
            return _indicate;
        }

        /**
         * @brief   Check if writing with Signed Write Command is permitted.
         *
         * @return  true if writing the value with Signed Write Command is
         *          permitted, false otherwise.
         */
        bool authSignedWrite(void) const {
            return _authSignedWrite;
        }

        /**
         * @brief "Equal to" operator for DiscoveredCharacteristic::Properties_t
         *
         * @param[in] lhs The left hand side of the equality expression
         * @param[in] rhs The right hand side of the equality expression
         *
         * @return true if operands are equals, false otherwise.
         */
        friend bool operator==(Properties_t lhs, Properties_t rhs) {
            return lhs._broadcast == rhs._broadcast &&
                   lhs._read == rhs._read &&
                   lhs._writeWoResp == rhs._writeWoResp &&
                   lhs._write == rhs._write &&
                   lhs._notify == rhs._notify &&
                   lhs._indicate == rhs._indicate &&
                   lhs._authSignedWrite == rhs._authSignedWrite;
        }

        /**
         * @brief "Not equal to" operator for DiscoveredCharacteristic::Properties_t
         *
         * @param lhs The right hand side of the expression
         * @param rhs The left hand side of the expression
         *
         * @return true if operands are not equals, false otherwise.
         */
        friend bool operator!=(Properties_t lhs, Properties_t rhs) {
            return !(lhs == rhs);
        }

    private:
        operator uint8_t()  const; /* Disallow implicit conversion into an integer. */
        operator unsigned() const; /* Disallow implicit conversion into an integer. */
    };

    /**
     * Initiate (or continue) a read for the value attribute, optionally at a
     * given offset. If the characteristic or descriptor to be read is longer
     * than ATT_MTU - 1, this function must be called multiple times with
     * appropriate offset to read the complete value.
     *
     * @param[in] offset
     *              The position - in the characteristic value bytes stream - where
     *              the read operation begin.
     *
     * @return BLE_ERROR_NONE if a read has been initiated, or
     *         BLE_ERROR_INVALID_STATE if some internal state about the connection is invalid, or
     *         BLE_STACK_BUSY if some client procedure is already in progress, or
     *         BLE_ERROR_OPERATION_NOT_PERMITTED due to the characteristic's properties.
     */
    ble_error_t read(uint16_t offset = 0) const;

    /**
     * @brief Same as #read(uint16_t) const but allow the user to register a callback
     * which will be fired once the read is done.
     *
     * @param[in] offset
     *              The position - in the characteristic value bytes stream - where
     *              the read operation begin.
     * @param[in] onRead
     *              Continuation of the read operation
     */
    ble_error_t read(uint16_t offset, const GattClient::ReadCallback_t& onRead) const;

    /**
     * Perform a write without response procedure.
     *
     * @param[in]  length
     *           The amount of data being written.
     * @param[in]  value
     *           The bytes being written.
     *
     * @note   It is important to note that a write without response will generate
     *         an onDataSent() callback when the packet has been transmitted. There
     *         will be a BLE-stack specific limit to the number of pending
     *         writeWoResponse operations; the user may want to use the onDataSent()
     *         callback for flow-control.
     *
     * @retval BLE_ERROR_NONE Successfully started the Write procedure, or
     *         BLE_ERROR_INVALID_STATE if some internal state about the connection is invalid, or
     *         BLE_STACK_BUSY if some client procedure is already in progress, or
     *         BLE_ERROR_NO_MEM if there are no available buffers left to process the request, or
     *         BLE_ERROR_OPERATION_NOT_PERMITTED due to the characteristic's properties.
     */
    ble_error_t writeWoResponse(uint16_t length, const uint8_t *value) const;

    /**
     * Initiate a GATT Characteristic Descriptor Discovery procedure for descriptors within this characteristic.
     *
     * @param[in] onDescriptorDiscovered This callback will be called every time a descriptor is discovered
     * @param[in] onTermination This callback will be called when the discovery process is over.
     *
     * @return BLE_ERROR_NONE if descriptor discovery is launched successfully; else an appropriate error.
     */
    ble_error_t discoverDescriptors(const CharacteristicDescriptorDiscovery::DiscoveryCallback_t& onDescriptorDiscovered,
                                    const CharacteristicDescriptorDiscovery::TerminationCallback_t& onTermination) const;

    /**
     * Perform a write procedure.
     *
     * @param[in]  length
     *           The amount of data being written.
     * @param[in]  value
     *           The bytes being written.
     *
     * @note   It is important to note that a write will generate
     *         an onDataWritten() callback when the peer acknowledges the request.
     *
     * @retval BLE_ERROR_NONE Successfully started the Write procedure, or
     *         BLE_ERROR_INVALID_STATE if some internal state about the connection is invalid, or
     *         BLE_STACK_BUSY if some client procedure is already in progress, or
     *         BLE_ERROR_NO_MEM if there are no available buffers left to process the request, or
     *         BLE_ERROR_OPERATION_NOT_PERMITTED due to the characteristic's properties.
     */
    ble_error_t write(uint16_t length, const uint8_t *value) const;

    /**
     * Same as write(uint16_t, const uint8_t *) const but register a callback
     * which will be called once the data has been written.
     *
     * @param[in] length
     *              The amount of bytes to write.
     * @param[in] value
     *              The bytes to write.
     * @param[in] onWrite
     *              Continuation callback for the write operation
     *
     * @retval BLE_ERROR_NONE Successfully started the Write procedure, or
     *         BLE_ERROR_INVALID_STATE if some internal state about the connection is invalid, or
     *         BLE_STACK_BUSY if some client procedure is already in progress, or
     *         BLE_ERROR_NO_MEM if there are no available buffers left to process the request, or
     *         BLE_ERROR_OPERATION_NOT_PERMITTED due to the characteristic's properties.
     */
    ble_error_t write(uint16_t length, const uint8_t *value, const GattClient::WriteCallback_t& onWrite) const;

    void setupLongUUID(UUID::LongUUIDBytes_t longUUID, UUID::ByteOrder_t order = UUID::MSB) {
        uuid.setupLong(longUUID, order);
    }

public:
    /**
     * @brief Get the UUID of the discovered characteristic
     * @return the UUID of this characteristic
     */
    const UUID& getUUID(void) const {
        return uuid;
    }

    /**
     * @brief Get the properties of this characteristic
     * @return the set of properties of this characteristic
     */
    const Properties_t& getProperties(void) const {
        return props;
    }

    /**
     * @brief Get the declaration handle of this characteristic.
     * @details The declaration handle is the first handle of a characteristic
     * definition. The value accessible at this handle contains the following
     * informations:
     *    - The characteristics properties (see Properties_t). This value can
     *      be accessed by using #getProperties .
     *    - The characteristic value attribute handle. This field can be accessed
     *      by using #getValueHandle .
     *    - The characteristic UUID, this value can be accessed by using the
     *      function #getUUID .
     * @return the declaration handle of this characteristic.
     */
    GattAttribute::Handle_t getDeclHandle(void) const {
        return declHandle;
    }

    /**
     * @brief Return the handle used to access the value of this characteristic.
     * @details This handle is the one provided in the characteristic declaration
     * value. Usually, it is equal to #getDeclHandle() + 1. But it is not always
     * the case. Anyway, users are allowed to use #getDeclHandle() + 1 to access
     * the value of a characteristic.
     * @return The handle to access the value of this characteristic.
     */
    GattAttribute::Handle_t getValueHandle(void) const {
        return valueHandle;
    }

    /**
     * @brief Return the last handle of the characteristic definition.
     * @details A Characteristic definition can contain a lot of handles:
     *     - one for the declaration (see #getDeclHandle)
     *     - one for the value (see #getValueHandle)
     *     - zero of more for the characteristic descriptors.
     * This handle is the last handle of the characteristic definition.
     * @return The last handle of this characteristic definition.
     */
    GattAttribute::Handle_t getLastHandle(void) const {
        return lastHandle;
    }

    /**
     * @brief Return the GattClient which can operate on this characteristic.
     * @return The GattClient which can operate on this characteristic.
     */
    GattClient* getGattClient() {
        return gattc;
    }

    /**
     * @brief Return the GattClient which can operate on this characteristic.
     * @return The GattClient which can operate on this characteristic.
     */
    const GattClient* getGattClient() const {
        return gattc;
    }

    /**
     * @brief Return the connection handle to the GattServer which contain
     * this characteristic.
     * @return the connection handle to the GattServer which contain
     * this characteristic.
     */
    Gap::Handle_t getConnectionHandle() const {
        return connHandle;
    }

    /**
     * @brief "Equal to" operator for DiscoveredCharacteristic
     *
     * @param[in] lhs
     *              The left hand side of the equality expression
     * @param[in] rhs
     *              The right hand side of the equality expression
     *
     * @return true if operands are equals, false otherwise.
     */
    friend bool operator==(const DiscoveredCharacteristic& lhs, const DiscoveredCharacteristic& rhs) {
        return lhs.gattc == rhs.gattc &&
               lhs.uuid == rhs.uuid &&
               lhs.props == rhs.props &&
               lhs.declHandle == rhs.declHandle &&
               lhs.valueHandle == rhs.valueHandle &&
               lhs.lastHandle == rhs.lastHandle &&
               lhs.connHandle == rhs.connHandle;
    }

    /**
     * @brief "Not equal to" operator for DiscoveredCharacteristic
     *
     * @param[in] lhs
     *              The right hand side of the expression
     * @param[in] rhs
     *              The left hand side of the expression
     *
     * @return true if operands are not equal, false otherwise.
     */
    friend bool operator !=(const DiscoveredCharacteristic& lhs, const DiscoveredCharacteristic& rhs) {
        return !(lhs == rhs);
    }

public:
    DiscoveredCharacteristic() : gattc(NULL),
                                 uuid(UUID::ShortUUIDBytes_t(0)),
                                 props(),
                                 declHandle(GattAttribute::INVALID_HANDLE),
                                 valueHandle(GattAttribute::INVALID_HANDLE),
                                 lastHandle(GattAttribute::INVALID_HANDLE),
                                 connHandle() {
        /* empty */
    }

protected:
    /**
     * Pointer to the underlying GattClient for this DiscoveredCharacteristic object.
     */
    GattClient              *gattc;

protected:
    /**
     * Discovered characteristic's UUID.
     */
    UUID                     uuid;
    /**
     * Hold the configured properties of the discovered characteristic.
     * For more information refer to Properties_t.
     */
    Properties_t             props;
    /**
     * Value handle of the discovered characteristic's declaration attribute.
     */
    GattAttribute::Handle_t  declHandle;
    /**
     * Value handle of the discovered characteristic's value attribute.
     */
    GattAttribute::Handle_t  valueHandle;
    /**
     * Value handle of the discovered characteristic's last attribute.
     */
    GattAttribute::Handle_t  lastHandle;

    /**
     * Handle for the connection where the characteristic was discovered.
     */
    Gap::Handle_t            connHandle;
};

#endif /*__DISCOVERED_CHARACTERISTIC_H__*/
