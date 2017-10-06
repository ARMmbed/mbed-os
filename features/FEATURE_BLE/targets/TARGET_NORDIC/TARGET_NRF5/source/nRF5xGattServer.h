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

#ifndef __NRF51822_GATT_SERVER_H__
#define __NRF51822_GATT_SERVER_H__

#include <stddef.h>

#include "ble/blecommon.h"
#include "headers/nrf_ble.h" /* nordic ble */
#include "ble/Gap.h"
#include "ble/GattServer.h"

class nRF5xGattServer : public GattServer
{
public:
    /* Functions that must be implemented from GattServer */
    virtual ble_error_t addService(GattService &);
    virtual ble_error_t read(GattAttribute::Handle_t attributeHandle, uint8_t buffer[], uint16_t *lengthP);
    virtual ble_error_t read(Gap::Handle_t connectionHandle, GattAttribute::Handle_t attributeHandle, uint8_t buffer[], uint16_t *lengthP);
    virtual ble_error_t write(GattAttribute::Handle_t, const uint8_t[], uint16_t, bool localOnly = false);
    virtual ble_error_t write(Gap::Handle_t connectionHandle, GattAttribute::Handle_t, const uint8_t[], uint16_t, bool localOnly = false);
    virtual ble_error_t areUpdatesEnabled(const GattCharacteristic &characteristic, bool *enabledP);
    virtual ble_error_t areUpdatesEnabled(Gap::Handle_t connectionHandle, const GattCharacteristic &characteristic, bool *enabledP);
    virtual ble_error_t reset(void);

    /* nRF51 Functions */
    void eventCallback(void);
    void hwCallback(ble_evt_t *p_ble_evt);


private:
    const static unsigned BLE_TOTAL_CHARACTERISTICS = 20;
    const static unsigned BLE_TOTAL_DESCRIPTORS     = 8;
    const static unsigned TOTAL_CONCURRENT_LONG_WRITE_REQUESTS = 3;

private:
    struct long_write_request_t {
        // the connection handle for a long write request
        uint16_t conn_handle;

        // the attribute handle for the long write request
        // This implementation folow the bluetooth route
        // where a write request target a single characteristic
        // (see BLUETOOTH SPECIFICATION Version 4.2 [Vol 3, Part G] - 4.9.4)
        uint16_t attr_handle;

        // offset of the transaction
        uint16_t offset;

        // length of the data
        uint16_t length;

        // current data
        uint8_t* data;
    };


private:
    /**
     * resolve a value attribute to its owning characteristic.
     * @param  valueHandle the value handle to be resolved.
     * @return             characteristic index if a resolution is found, else -1.
     */
    int resolveValueHandleToCharIndex(GattAttribute::Handle_t valueHandle) const {
        unsigned charIndex;
        for (charIndex = 0; charIndex < characteristicCount; charIndex++) {
            if (nrfCharacteristicHandles[charIndex].value_handle == valueHandle) {
                return charIndex;
            }
        }

        return -1;
    }

    /**
     * resolve a CCCD attribute handle to its owning characteristic.
     * @param  cccdHandle the CCCD handle to be resolved.
     * @return             characteristic index if a resolution is found, else -1.
     */
    int resolveCCCDHandleToCharIndex(GattAttribute::Handle_t cccdHandle) const {
        unsigned charIndex;
        for (charIndex = 0; charIndex < characteristicCount; charIndex++) {
            if (nrfCharacteristicHandles[charIndex].cccd_handle == cccdHandle) {
                return charIndex;
            }
        }

        return -1;
    }

    /**
     * Return the biggest size used by a characteristic in the server
     */
    uint16_t getBiggestCharacteristicSize() const;

    /**
     * Allocate a new write long request. return null if no requests are available.
     * @param  connection_handle The connection handle to be associated with the request.
     * @return the allocated request or NULL if no requests are available.
     */
    long_write_request_t* allocateLongWriteRequest(uint16_t connection_handle);

    /**
     * Release a long write request and free a slot for subsequent write long requests.
     * @param  connection_handle The connection handle associated with the request
     * @return true if the request where allocated and was release, false otherwise.
     */
    bool releaseLongWriteRequest(uint16_t connection_handle);

    /**
     * Find a long write request from a characteristic handle
     * @param  connection_handle The connection handle associated with the request.
     * @return a pointer to the request if found otherwise NULL.
     */
    long_write_request_t* findLongWriteRequest(uint16_t connection_handle);

    /**
     * Release all pending write requests.
     */
    void releaseAllWriteRequests();

private:
    GattCharacteristic       *p_characteristics[BLE_TOTAL_CHARACTERISTICS];
    ble_gatts_char_handles_t  nrfCharacteristicHandles[BLE_TOTAL_CHARACTERISTICS];
    GattAttribute            *p_descriptors[BLE_TOTAL_DESCRIPTORS];
    uint8_t                   descriptorCount;
    uint16_t                  nrfDescriptorHandles[BLE_TOTAL_DESCRIPTORS];
    long_write_request_t      long_write_requests[TOTAL_CONCURRENT_LONG_WRITE_REQUESTS];

    /*
     * Allow instantiation from nRF5xn when required.
     */
    friend class nRF5xn;

    nRF5xGattServer() : GattServer(), p_characteristics(), nrfCharacteristicHandles(), p_descriptors(), descriptorCount(0), nrfDescriptorHandles(), long_write_requests() {
        /* empty */
    }

private:
    nRF5xGattServer(const nRF5xGattServer &);
    const nRF5xGattServer& operator=(const nRF5xGattServer &);
};

#endif // ifndef __NRF51822_GATT_SERVER_H__
