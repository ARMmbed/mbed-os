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

#ifndef __NRF51822_GATT_CLIENT_H__
#define __NRF51822_GATT_CLIENT_H__

#include "ble/GattClient.h"
#include "nRF5xServiceDiscovery.h"
#include "nRF5xCharacteristicDescriptorDiscoverer.h"

class nRF5xGattClient : public GattClient
{
public:
    /**
     * When using S110, all Gatt client features will return
     * BLE_ERROR_NOT_IMPLEMENTED
     */
#if !defined(TARGET_MCU_NRF51_16K_S110) && !defined(TARGET_MCU_NRF51_32K_S110)

    /**
     * Launch service discovery. Once launched, service discovery will remain
     * active with callbacks being issued back into the application for matching
     * services/characteristics. isActive() can be used to determine status; and
     * a termination callback (if setup) will be invoked at the end. Service
     * discovery can be terminated prematurely if needed using terminate().
     *
     * @param  connectionHandle
     *           Handle for the connection with the peer.
     * @param  sc
     *           This is the application callback for matching service. Taken as
     *           NULL by default. Note: service discovery may still be active
     *           when this callback is issued; calling asynchronous BLE-stack
     *           APIs from within this application callback might cause the
     *           stack to abort service discovery. If this becomes an issue, it
     *           may be better to make local copy of the discoveredService and
     *           wait for service discovery to terminate before operating on the
     *           service.
     * @param  cc
     *           This is the application callback for matching characteristic.
     *           Taken as NULL by default. Note: service discovery may still be
     *           active when this callback is issued; calling asynchronous
     *           BLE-stack APIs from within this application callback might cause
     *           the stack to abort service discovery. If this becomes an issue,
     *           it may be better to make local copy of the discoveredCharacteristic
     *           and wait for service discovery to terminate before operating on the
     *           characteristic.
     * @param  matchingServiceUUID
     *           UUID based filter for specifying a service in which the application is
     *           interested. By default it is set as the wildcard UUID_UNKNOWN,
     *           in which case it matches all services. If characteristic-UUID
     *           filter (below) is set to the wildcard value, then a service
     *           callback will be invoked for the matching service (or for every
     *           service if the service filter is a wildcard).
     * @param  matchingCharacteristicUUIDIn
     *           UUID based filter for specifying characteristic in which the application
     *           is interested. By default it is set as the wildcard UUID_UKNOWN
     *           to match against any characteristic. If both service-UUID
     *           filter and characteristic-UUID filter are used with non- wildcard
     *           values, then only a single characteristic callback is
     *           invoked for the matching characteristic.
     *
     * @Note     Using wildcard values for both service-UUID and characteristic-
     *           UUID will result in complete service discovery--callbacks being
     *           called for every service and characteristic.
     *
     * @return
     *           BLE_ERROR_NONE if service discovery is launched successfully; else an appropriate error.
     */
    virtual ble_error_t launchServiceDiscovery(Gap::Handle_t                               connectionHandle,
                                               ServiceDiscovery::ServiceCallback_t         sc = NULL,
                                               ServiceDiscovery::CharacteristicCallback_t  cc = NULL,
                                               const UUID                                 &matchingServiceUUID = UUID::ShortUUIDBytes_t(BLE_UUID_UNKNOWN),
                                               const UUID                                 &matchingCharacteristicUUIDIn = UUID::ShortUUIDBytes_t(BLE_UUID_UNKNOWN));

    virtual void onServiceDiscoveryTermination(ServiceDiscovery::TerminationCallback_t callback) {
        _discovery.onTermination(callback);
    }

    /**
     * Is service-discovery currently active?
     */
    virtual bool isServiceDiscoveryActive(void) const {
        return _discovery.isActive();
    }

    /**
     * Terminate an ongoing service-discovery. This should result in an
     * invocation of the TerminationCallback if service-discovery is active.
     */
    virtual void terminateServiceDiscovery(void) {
        _discovery.terminate();
    }

    /**
     * @brief Implementation of GattClient::discoverCharacteristicDescriptors
     * @see GattClient::discoverCharacteristicDescriptors
     */
    virtual ble_error_t discoverCharacteristicDescriptors(
        const DiscoveredCharacteristic& characteristic,
        const CharacteristicDescriptorDiscovery::DiscoveryCallback_t& discoveryCallback,
        const CharacteristicDescriptorDiscovery::TerminationCallback_t& terminationCallback
    );

    /**
     * @brief Implementation of GattClient::isCharacteristicDiscoveryActive
     * @see GattClient::isCharacteristicDiscoveryActive
     */
    virtual bool isCharacteristicDescriptorsDiscoveryActive(const DiscoveredCharacteristic& characteristic) const;

    /**
     * @brief Implementation of GattClient::terminateCharacteristicDiscovery
     * @see GattClient::terminateCharacteristicDiscovery
     */
    virtual void terminateCharacteristicDescriptorsDiscovery(const DiscoveredCharacteristic& characteristic);

    virtual ble_error_t read(Gap::Handle_t connHandle, GattAttribute::Handle_t attributeHandle, uint16_t offset) const {
        uint32_t rc = sd_ble_gattc_read(connHandle, attributeHandle, offset);
        if (rc == NRF_SUCCESS) {
            return BLE_ERROR_NONE;
        }
        switch (rc) {
            case NRF_ERROR_BUSY:
                return BLE_STACK_BUSY;
            case BLE_ERROR_INVALID_CONN_HANDLE:
            case NRF_ERROR_INVALID_STATE:
            case NRF_ERROR_INVALID_ADDR:
            default:
                return BLE_ERROR_INVALID_STATE;
        }
    }

    virtual ble_error_t write(GattClient::WriteOp_t cmd, Gap::Handle_t connHandle, GattAttribute::Handle_t attributeHandle, size_t length, const uint8_t *value) const {
        ble_gattc_write_params_t writeParams;
        writeParams.write_op = cmd;
        writeParams.flags    = 0; /* this is inconsequential */
        writeParams.handle   = attributeHandle;
        writeParams.offset   = 0;
        writeParams.len      = length;
        writeParams.p_value  = const_cast<uint8_t *>(value);

        uint32_t rc = sd_ble_gattc_write(connHandle, &writeParams);
        if (rc == NRF_SUCCESS) {
            return BLE_ERROR_NONE;
        }
        switch (rc) {
            case NRF_ERROR_BUSY:
                return BLE_STACK_BUSY;
            case BLE_ERROR_NO_TX_PACKETS:
                return BLE_ERROR_NO_MEM;
            case BLE_ERROR_INVALID_CONN_HANDLE:
            case NRF_ERROR_INVALID_STATE:
            case NRF_ERROR_INVALID_ADDR:
            default:
                return BLE_ERROR_INVALID_STATE;
        }
    }

    /**
     * @brief  Clear nRF5xGattClient's state.
     *
     * @return
     *           BLE_ERROR_NONE if successful.
     */
    virtual ble_error_t reset(void) {
        /* Clear all state that is from the parent, including private members */
        if (GattClient::reset() != BLE_ERROR_NONE) {
            return BLE_ERROR_INVALID_STATE;
        }

        /* Clear derived class members */
        _discovery.reset();

        return BLE_ERROR_NONE;
    }

public:
    /*
     * Allow instantiation from nRF5xn when required.
     */
    friend class nRF5xn;

    nRF5xGattClient() : _discovery(this) {
        /* empty */
    }

    nRF5xServiceDiscovery& discovery() {
        return _discovery;
    }

    nRF5xCharacteristicDescriptorDiscoverer& characteristicDescriptorDiscoverer() {
        return _characteristicDescriptorDiscoverer;
    }

private:
    nRF5xGattClient(const nRF5xGattClient &);
    const nRF5xGattClient& operator=(const nRF5xGattClient &);

private:
    nRF5xServiceDiscovery _discovery;
    nRF5xCharacteristicDescriptorDiscoverer _characteristicDescriptorDiscoverer;

#endif // if !S110
};

#endif // ifndef __NRF51822_GATT_CLIENT_H__
