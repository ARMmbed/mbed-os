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

#ifndef __NRF_SERVICE_DISCOVERY_H__
#define __NRF_SERVICE_DISCOVERY_H__

#include "ble/ServiceDiscovery.h"
#include "ble/DiscoveredService.h"
#include "nRF5xDiscoveredCharacteristic.h"

#include "headers/nrf_ble.h"
#include "headers/nrf_ble_gattc.h"

class nRF5xGattClient; /* forward declaration */

class nRF5xServiceDiscovery : public ServiceDiscovery
{
public:
    static const uint16_t SRV_DISC_START_HANDLE             = 0x0001; /**< The start handle value used during service discovery. */
    static const uint16_t SRV_DISC_END_HANDLE               = 0xFFFF; /**< The end handle value used during service discovery. */

public:
    static const unsigned BLE_DB_DISCOVERY_MAX_SRV          = 4;      /**< Maximum number of services we can retain information for after a single discovery. */
    static const unsigned BLE_DB_DISCOVERY_MAX_CHAR_PER_SRV = 4;      /**< Maximum number of characteristics per service we can retain information for. */

public:
    nRF5xServiceDiscovery(nRF5xGattClient *gattcIn) :
        gattc(gattcIn),
        serviceIndex(0),
        numServices(0),
        numCharacteristics(0),
        state(INACTIVE),
        services(),
        characteristics(),
        serviceUUIDDiscoveryQueue(this),
        charUUIDDiscoveryQueue(this),
        onTerminationCallback(NULL) {
        /* empty */
    }

    virtual ble_error_t launch(Gap::Handle_t                               connectionHandle,
                               ServiceDiscovery::ServiceCallback_t         sc,
                               ServiceDiscovery::CharacteristicCallback_t  cc,
                               const UUID                                 &matchingServiceUUIDIn,
                               const UUID                                 &matchingCharacteristicUUIDIn)
    {
        if (isActive()) {
            return BLE_ERROR_INVALID_STATE;
        }

        serviceCallback            = sc;
        characteristicCallback     = cc;
        matchingServiceUUID        = matchingServiceUUIDIn;
        matchingCharacteristicUUID = matchingCharacteristicUUIDIn;

        serviceDiscoveryStarted(connectionHandle);

        uint32_t rc;
        if ((rc = sd_ble_gattc_primary_services_discover(connectionHandle, SRV_DISC_START_HANDLE, NULL)) != NRF_SUCCESS) {
            terminate();
            switch (rc) {
                case NRF_ERROR_INVALID_PARAM:
                case BLE_ERROR_INVALID_CONN_HANDLE:
                    return BLE_ERROR_INVALID_PARAM;
                case NRF_ERROR_BUSY:
                    return BLE_STACK_BUSY;
                default:
                case NRF_ERROR_INVALID_STATE:
                    return BLE_ERROR_INVALID_STATE;
            }
        }

        return BLE_ERROR_NONE;
    }

    virtual bool isActive(void) const {
        return state != INACTIVE;
    }

    virtual void terminate(void) {
        terminateServiceDiscovery();
    }

    void terminate(Gap::Handle_t connectionHandle) {
        if(connHandle == connectionHandle) {
            terminate();
        }
    }

    virtual void onTermination(ServiceDiscovery::TerminationCallback_t callback) {
        onTerminationCallback = callback;
    }

    /**
     * @brief  Clear nRF5xServiceDiscovery's state.
     *
     * @return
     *           BLE_ERROR_NONE if successful.
     */
    virtual ble_error_t reset(void) {
        /* Clear all state that is from the parent, including private members */
        if (ServiceDiscovery::reset() != BLE_ERROR_NONE) {
            return BLE_ERROR_INVALID_STATE;
        }

        /* Clear derived class members */
        serviceIndex = 0;
        numServices = 0;
        numCharacteristics = 0;

        state = INACTIVE;

        serviceUUIDDiscoveryQueue.reset();
        charUUIDDiscoveryQueue.reset();

        onTerminationCallback = NULL;

        return BLE_ERROR_NONE;
    }

private:
    ble_error_t launchCharacteristicDiscovery(Gap::Handle_t connectionHandle, Gap::Handle_t startHandle, Gap::Handle_t endHandle);

private:
    void setupDiscoveredServices(const ble_gattc_evt_prim_srvc_disc_rsp_t *response);
    void setupDiscoveredCharacteristics(const ble_gattc_evt_char_disc_rsp_t *response);

    void triggerServiceUUIDDiscovery(void);
    void processDiscoverUUIDResponse(const ble_gattc_evt_t *p_gattc_evt);
    void removeFirstServiceNeedingUUIDDiscovery(void);

    void terminateServiceDiscovery(void) {
        discoveredCharacteristic = nRF5xDiscoveredCharacteristic();

        bool wasActive = isActive();
        state = INACTIVE;

        if (wasActive && onTerminationCallback) {
            onTerminationCallback(connHandle);
        }
    }

    void terminateCharacteristicDiscovery(ble_error_t err) {
        if (state == CHARACTERISTIC_DISCOVERY_ACTIVE) {
            if(discoveredCharacteristic != nRF5xDiscoveredCharacteristic()) {
               if(err == BLE_ERROR_NONE) {
                    // fullfill the last characteristic
                    discoveredCharacteristic.setLastHandle(services[serviceIndex].getEndHandle());

                    if ((matchingCharacteristicUUID == UUID::ShortUUIDBytes_t(BLE_UUID_UNKNOWN)) ||
                        ((matchingCharacteristicUUID == discoveredCharacteristic.getUUID()) &&
                         (matchingServiceUUID != UUID::ShortUUIDBytes_t(BLE_UUID_UNKNOWN)))) {
                        if (characteristicCallback) {
                            characteristicCallback(&discoveredCharacteristic);
                        }
                    }
               }
               discoveredCharacteristic = nRF5xDiscoveredCharacteristic();
            }

            state = SERVICE_DISCOVERY_ACTIVE;
        }
        serviceIndex++; /* Progress service index to keep discovery alive. */
    }

private:
    void resetDiscoveredServices(void) {
        numServices  = 0;
        serviceIndex = 0;
    }

    void resetDiscoveredCharacteristics(void) {
        numCharacteristics  = 0;
    }

private:
    void serviceDiscoveryStarted(Gap::Handle_t connectionHandle) {
        connHandle = connectionHandle;
        resetDiscoveredServices();
        state = SERVICE_DISCOVERY_ACTIVE;
    }

private:
    void characteristicDiscoveryStarted(Gap::Handle_t connectionHandle) {
        connHandle = connectionHandle;
        resetDiscoveredCharacteristics();
        state = CHARACTERISTIC_DISCOVERY_ACTIVE;
    }

private:
    /**
     * A datatype to contain service-indices for which long UUIDs need to be
     * discovered using read_val_by_uuid().
     */
    class ServiceUUIDDiscoveryQueue {
    public:
        ServiceUUIDDiscoveryQueue(nRF5xServiceDiscovery *parent) :
            numIndices(0),
            serviceIndices(),
            parentDiscoveryObject(parent) {
            /* empty */
        }

    public:
        void reset(void) {
            numIndices = 0;
            for (unsigned i = 0; i < BLE_DB_DISCOVERY_MAX_SRV; i++) {
                serviceIndices[i] = INVALID_INDEX;
            }
        }
        void enqueue(int serviceIndex) {
            serviceIndices[numIndices++] = serviceIndex;
        }
        int dequeue(void) {
            if (numIndices == 0) {
                return INVALID_INDEX;
            }

            unsigned valueToReturn = serviceIndices[0];
            numIndices--;
            for (unsigned i = 0; i < numIndices; i++) {
                serviceIndices[i] = serviceIndices[i + 1];
            }

            return valueToReturn;
        }
        unsigned getFirst(void) const {
            return serviceIndices[0];
        }
        size_t getCount(void) const {
            return numIndices;
        }

        /**
         * Trigger UUID discovery for the first of the enqueued ServiceIndices.
         */
        void triggerFirst(void);

    private:
        static const int INVALID_INDEX = -1;

    private:
        size_t numIndices;
        int    serviceIndices[BLE_DB_DISCOVERY_MAX_SRV];

        nRF5xServiceDiscovery *parentDiscoveryObject;
    };
    friend class ServiceUUIDDiscoveryQueue;

    /**
     * A datatype to contain characteristic-indices for which long UUIDs need to
     * be discovered using read_val_by_uuid().
     */
    class CharUUIDDiscoveryQueue {
    public:
        CharUUIDDiscoveryQueue(nRF5xServiceDiscovery *parent) :
            numIndices(0),
            charIndices(),
            parentDiscoveryObject(parent) {
            /* empty */
        }

    public:
        void reset(void) {
            numIndices = 0;
            for (unsigned i = 0; i < BLE_DB_DISCOVERY_MAX_SRV; i++) {
                charIndices[i] = INVALID_INDEX;
            }
        }
        void enqueue(int serviceIndex) {
            charIndices[numIndices++] = serviceIndex;
        }
        int dequeue(void) {
            if (numIndices == 0) {
                return INVALID_INDEX;
            }

            unsigned valueToReturn = charIndices[0];
            numIndices--;
            for (unsigned i = 0; i < numIndices; i++) {
                charIndices[i] = charIndices[i + 1];
            }

            return valueToReturn;
        }
        unsigned getFirst(void) const {
            return charIndices[0];
        }
        size_t getCount(void) const {
            return numIndices;
        }

        /**
         * Trigger UUID discovery for the first of the enqueued charIndices.
         */
        void triggerFirst(void);

    private:
        static const int INVALID_INDEX = -1;

    private:
        size_t numIndices;
        int    charIndices[BLE_DB_DISCOVERY_MAX_CHAR_PER_SRV];

        nRF5xServiceDiscovery *parentDiscoveryObject;
    };
    friend class CharUUIDDiscoveryQueue;

private:
    friend void bleGattcEventHandler(const ble_evt_t *p_ble_evt);
    void progressCharacteristicDiscovery(void);
    void progressServiceDiscovery(void);

private:
    nRF5xGattClient *gattc;

private:
    uint8_t  serviceIndex;        /**< Index of the current service being discovered. This is intended for internal use during service discovery.*/
    uint8_t  numServices;         /**< Number of services at the peers GATT database.*/
    uint8_t  numCharacteristics;  /**< Number of characteristics within the service.*/

    enum State_t {
        INACTIVE,
        SERVICE_DISCOVERY_ACTIVE,
        CHARACTERISTIC_DISCOVERY_ACTIVE,
        DISCOVER_SERVICE_UUIDS,
        DISCOVER_CHARACTERISTIC_UUIDS,
    } state;

    DiscoveredService           services[BLE_DB_DISCOVERY_MAX_SRV];  /**< Information related to the current service being discovered.
                                                                      *  This is intended for internal use during service discovery. */
    nRF5xDiscoveredCharacteristic characteristics[BLE_DB_DISCOVERY_MAX_CHAR_PER_SRV];

    ServiceUUIDDiscoveryQueue   serviceUUIDDiscoveryQueue;
    CharUUIDDiscoveryQueue      charUUIDDiscoveryQueue;

    TerminationCallback_t       onTerminationCallback;

    /*
     * The currently discovered characteristic. Discovery of a characteristic
     * is a two phase process.
     * First, declaration handle is fetched, it provide the UUID, the value handle and
     * the properties of a characteristic.
     * Second, the next declaration handle is fetched, with its declaration handle, it is
     * possible to compute the last handle of the discovered characteristic and fill the
     * missing part of the object.
     * If there is no remaining characteristic to discover, the last handle of the
     * discovered characteristic will be set to the last handle of its enclosing service.
     */
    nRF5xDiscoveredCharacteristic discoveredCharacteristic;
};

#endif /*__NRF_SERVICE_DISCOVERY_H__*/
