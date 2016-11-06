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

#ifndef __SERVICE_DISOVERY_H__
#define __SERVICE_DISOVERY_H__

#include "UUID.h"
#include "Gap.h"
#include "GattAttribute.h"

class DiscoveredService;
class DiscoveredCharacteristic;

class ServiceDiscovery {
public:
    /*
     * Exposed application callback types.
     */

    /**
     * Callback type for when a matching service is found during service-
     * discovery. The receiving function is passed in a pointer to a
     * DiscoveredService object, which will remain valid for the lifetime of the
     * callback. Memory for this object is owned by the BLE_API eventing
     * framework. The application can safely make a persistent shallow-copy of
     * this object to work with the service beyond the callback.
     */
    typedef FunctionPointerWithContext<const DiscoveredService *> ServiceCallback_t;

    /**
     * Callback type for when a matching characteristic is found during service-
     * discovery. The receiving function is passed in a pointer to a
     * DiscoveredCharacteristic object, which will remain valid for the lifetime
     * of the callback. Memory for this object is owned by the BLE_API eventing
     * framework. The application can safely make a persistent shallow-copy of
     * this object to work with the characteristic beyond the callback.
     */
    typedef FunctionPointerWithContext<const DiscoveredCharacteristic *> CharacteristicCallback_t;

    /**
     * Callback type for when serviceDiscovery terminates.
     */
    typedef FunctionPointerWithContext<Gap::Handle_t> TerminationCallback_t;

public:
    /**
     * Launch service discovery. Once launched, service discovery will remain
     * active with callbacks being issued back into the application for matching
     * services or characteristics. isActive() can be used to determine status, and
     * a termination callback (if set up) will be invoked at the end. Service
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
     *           interested. By default it is set as the wildcard UUID_UNKNOWN,
     *           in which case it matches all services. If characteristic-UUID
     *           filter (below) is set to the wildcard value, then a service
     *           callback will be invoked for the matching service (or for every
     *           service if the service filter is a wildcard).
     * @param  matchingCharacteristicUUIDIn
     *           UUID-based filter for specifying a characteristic in which the application
     *           is interested. By default it is set as the wildcard UUID_UKNOWN
     *           to match against any characteristic. If both service-UUID
     *           filter and characteristic-UUID filter are used with non-wildcard
     *           values, then only a single characteristic callback is
     *           invoked for the matching characteristic.
     *
     * @note     Using wildcard values for both service-UUID and characteristic-
     *           UUID will result in complete service discovery: callbacks being
     *           called for every service and characteristic.
     *
     * @note     Providing NULL for the characteristic callback will result in
     *           characteristic discovery being skipped for each matching
     *           service. This allows for an inexpensive method to discover only
     *           services.
     *
     * @return
     *           BLE_ERROR_NONE if service discovery is launched successfully; else an appropriate error.
     */
    virtual ble_error_t launch(Gap::Handle_t             connectionHandle,
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
     * sub-class. Nevertheless, the sub-class is only expected to reset its
     * state and not the data held in ServiceDiscovery members. This shall be
     * achieved by a call to ServiceDiscovery::reset() from the sub-class'
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
    Gap::Handle_t            connHandle;
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

#endif /* ifndef __SERVICE_DISOVERY_H__ */
