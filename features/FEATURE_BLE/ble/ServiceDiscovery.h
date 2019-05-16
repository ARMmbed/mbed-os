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

#ifndef MBED_BLE_SERVICE_DISOVERY_H__
#define MBED_BLE_SERVICE_DISOVERY_H__

#include "UUID.h"
#include "ble/Gap.h"
#include "GattAttribute.h"

class DiscoveredService;
class DiscoveredCharacteristic;

/**
 * @addtogroup ble
 * @{
 * @addtogroup gatt
 * @{
 * @addtogroup client
 * @{
 */

/**
 * Host callback types needed by the service discovery procedure.
 *
 * This class is also an interface that may be used in vendor port to model
 * the service discovery process. This interface is not used in user code.
 *
 * @attention Implementing this interface is not a requirement for the
 * implementation of the service discover process.
 */
class ServiceDiscovery {
public:
    /**
     * Service discovered event handler.
     *
     * The callback accepts a pointer to a DiscoveredService as parameter.
     *
     * @attention The argument passed to the callback may not persist after the
     * callback invocation; therefore, the callbacks must make a shallow copy
     * of the DiscoveredService passed as parameter to access its value beyond
     * the callback scope.
     */
    typedef FunctionPointerWithContext<const DiscoveredService *>
        ServiceCallback_t;

    /**
     * Characteristic discovered event handler.
     *
     * The callback accepts a pointer to a DiscoveredCharacteristic as
     * parameter.
     *
     * @attention The argument passed to the callback may not persist after the
     * callback invocation; therefore, the callbacks must make a shallow copy
     * of the DiscoveredCharacteristic passed as parameter to access its value
     * beyond the callback scope.
     */
    typedef FunctionPointerWithContext<const DiscoveredCharacteristic *>
        CharacteristicCallback_t;

    /**
     * Service discovery ended event.
     *
     * The callback accepts a connection handle as parameter. This
     * parameter is used to identify on which connection the service discovery
     * process ended.
     */
    typedef FunctionPointerWithContext<ble::connection_handle_t> TerminationCallback_t;

public:
    /**
     * Launch service discovery. Once launched, service discovery remains
     * active with callbacks being issued back into the application for matching
     * services or characteristics. isActive() can be used to determine status, and
     * a termination callback (if set up) is invoked at the end. Service
     * discovery can be terminated prematurely, if needed, using terminate().
     *
     * @param  connectionHandle
     *           Handle for the connection with the peer.
     * @param  sc
     *           This is the application callback for a matching service. Taken as
     *           NULL by default. Note: service discovery may still be active
     *           when this callback is issued; calling asynchronous BLE-stack
     *           APIs from within this application callback might cause the
     *           stack to abort service discovery. If this becomes an issue, it
     *           may be better to make a local copy of the discoveredService and
     *           wait for service discovery to terminate before operating on the
     *           service.
     * @param  cc
     *           This is the application callback for a matching characteristic.
     *           Taken as NULL by default. Note: service discovery may still be
     *           active when this callback is issued; calling asynchronous
     *           BLE-stack APIs from within this application callback might cause
     *           the stack to abort service discovery. If this becomes an issue,
     *           it may be better to make a local copy of the discoveredCharacteristic
     *           and wait for service discovery to terminate before operating on the
     *           characteristic.
     * @param  matchingServiceUUID
     *           UUID-based filter for specifying a service in which the application is
     *           interested. By default, it is set as the wildcard UUID_UNKNOWN,
     *           in which case it matches all services. If characteristic-UUID
     *           filter (below) is set to the wildcard value, then a service
     *           callback is invoked for the matching service (or for every
     *           service if the service filter is a wildcard).
     * @param  matchingCharacteristicUUIDIn
     *           UUID-based filter for specifying a characteristic in which the application
     *           is interested. By default, it is set as the wildcard UUID_UKNOWN
     *           to match against any characteristic. If both service-UUID
     *           filter and characteristic-UUID filter are used with nonwildcard
     *           values, then only a single characteristic callback is
     *           invoked for the matching characteristic.
     *
     * @note     Using wildcard values for both service-UUID and characteristic-
     *           UUID result in complete service discovery: callbacks being
     *           called for every service and characteristic.
     *
     * @note     Providing NULL for the characteristic callback results in
     *           characteristic discovery being skipped for each matching
     *           service. This allows for an inexpensive method to discover only
     *           services.
     *
     * @return
     *           BLE_ERROR_NONE if service discovery is launched successfully; else an appropriate error.
     */
    virtual ble_error_t launch(ble::connection_handle_t             connectionHandle,
                               ServiceCallback_t         sc = NULL,
                               CharacteristicCallback_t  cc = NULL,
                               const UUID               &matchingServiceUUID = UUID::ShortUUIDBytes_t(BLE_UUID_UNKNOWN),
                               const UUID               &matchingCharacteristicUUIDIn = UUID::ShortUUIDBytes_t(BLE_UUID_UNKNOWN)) = 0;

    /**
     * Check whether service-discovery is currently active.
     */
    virtual bool        isActive(void) const = 0;

    /**
     * Terminate an ongoing service discovery. This should result in an
     * invocation of the TerminationCallback if service discovery is active.
     */
    virtual void        terminate(void) = 0;

    /**
     * Set up a callback to be invoked when service discovery is terminated.
     */
    virtual void        onTermination(TerminationCallback_t callback) = 0;

    /**
     * Clear all ServiceDiscovery state of the associated object.
     *
     * This function is meant to be overridden in the platform-specific
     * subclass. Nevertheless, the subclass is only expected to reset its
     * state and not the data held in ServiceDiscovery members. This is
     * achieved by a call to ServiceDiscovery::reset() from the subclass'
     * reset() implementation.
     *
     * @return BLE_ERROR_NONE on success.
     */
    virtual ble_error_t reset(void) {
        connHandle                 = 0;
        matchingServiceUUID        = UUID::ShortUUIDBytes_t(BLE_UUID_UNKNOWN);
        serviceCallback            = NULL;
        matchingCharacteristicUUID = UUID::ShortUUIDBytes_t(BLE_UUID_UNKNOWN);
        characteristicCallback     = NULL;

        return BLE_ERROR_NONE;
    }

protected:
    /**
     * Connection handle as provided by the SoftDevice.
     */
    ble::connection_handle_t            connHandle;
    /**
     * UUID-based filter that specifies the service that the application is
     * interested in.
     */
    UUID                     matchingServiceUUID;
    /**
     * The registered callback handle for when a matching service is found
     * during service-discovery.
     */
    ServiceCallback_t        serviceCallback;
    /**
     * UUID-based filter that specifies the characteristic that the
     * application is interested in.
     */
    UUID                     matchingCharacteristicUUID;
    /**
     * The registered callback handler for when a matching characteristic is
     * found during service-discovery.
     */
    CharacteristicCallback_t characteristicCallback;
};

/**
 * @}
 * @}
 * @}
 */

#endif /* ifndef MBED_BLE_SERVICE_DISOVERY_H__ */
