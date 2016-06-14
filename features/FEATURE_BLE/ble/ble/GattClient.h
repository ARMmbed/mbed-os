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

#ifndef __GATT_CLIENT_H__
#define __GATT_CLIENT_H__

#include "Gap.h"
#include "GattAttribute.h"
#include "ServiceDiscovery.h"
#include "CharacteristicDescriptorDiscovery.h"

#include "GattCallbackParamTypes.h"

#include "CallChainOfFunctionPointersWithContext.h"

class GattClient {
public:
    /**
     * Type for the registered callbacks added to the data read callchain.
     * Refer to GattClient::onDataRead().
     */
    typedef FunctionPointerWithContext<const GattReadCallbackParams*> ReadCallback_t;
    /**
     * Type for the data read event callchain. Refer to GattClient::onDataRead().
     */
    typedef CallChainOfFunctionPointersWithContext<const GattReadCallbackParams*> ReadCallbackChain_t;

    /**
     * Enumerator for write operations.
     */
    enum WriteOp_t {
        GATT_OP_WRITE_REQ = 0x01,  /**< Write request. */
        GATT_OP_WRITE_CMD = 0x02,  /**< Write command. */
    };

    /**
     * Type for the registered callbacks added to the data write callchain.
     * Refer to GattClient::onDataWrite().
     */
    typedef FunctionPointerWithContext<const GattWriteCallbackParams*> WriteCallback_t;
    /**
     * Type for the data write event callchain. Refer to GattClient::onDataWrite().
     */
    typedef CallChainOfFunctionPointersWithContext<const GattWriteCallbackParams*> WriteCallbackChain_t;

    /**
     * Type for the registered callbacks added to the update event callchain.
     * Refer to GattClient::onHVX().
     */
    typedef FunctionPointerWithContext<const GattHVXCallbackParams*> HVXCallback_t;
    /**
     * Type for the update event callchain. Refer to GattClient::onHVX().
     */
    typedef CallChainOfFunctionPointersWithContext<const GattHVXCallbackParams*> HVXCallbackChain_t;

    /**
     * Type for the registered callbacks added to the shutdown callchain.
     * Refer to GattClient::onShutdown().
     */
    typedef FunctionPointerWithContext<const GattClient *> GattClientShutdownCallback_t;
    /**
     * Type for the shutdown event callchain. Refer to GattClient::onShutown().
     */
    typedef CallChainOfFunctionPointersWithContext<const GattClient *> GattClientShutdownCallbackChain_t;

    /*
     * The following functions are meant to be overridden in the platform-specific sub-class.
     */
public:
    /**
     * Launch service discovery. Once launched, application callbacks will be
     * invoked for matching services or characteristics. isServiceDiscoveryActive()
     * can be used to determine status, and a termination callback (if one was set up)
     * will be invoked at the end. Service discovery can be terminated prematurely,
     * if needed, using terminateServiceDiscovery().
     *
     * @param[in]  connectionHandle
     *              Handle for the connection with the peer.
     * @param[in]  sc
     *              This is the application callback for a matching service. Taken as
     *              NULL by default. Note: service discovery may still be active
     *              when this callback is issued; calling asynchronous BLE-stack
     *              APIs from within this application callback might cause the
     *              stack to abort service discovery. If this becomes an issue, it
     *              may be better to make a local copy of the discoveredService and
     *              wait for service discovery to terminate before operating on the
     *              service.
     * @param[in]  cc
     *              This is the application callback for a matching characteristic.
     *              Taken as NULL by default. Note: service discovery may still be
     *              active when this callback is issued; calling asynchronous
     *              BLE-stack APIs from within this application callback might cause
     *              the stack to abort service discovery. If this becomes an issue,
     *              it may be better to make a local copy of the discoveredCharacteristic
     *              and wait for service discovery to terminate before operating on the
     *              characteristic.
     * @param[in]  matchingServiceUUID
     *              UUID-based filter for specifying a service in which the application is
     *              interested. By default it is set as the wildcard UUID_UNKNOWN,
     *              in which case it matches all services. If characteristic-UUID
     *              filter (below) is set to the wildcard value, then a service
     *              callback will be invoked for the matching service (or for every
     *              service if the service filter is a wildcard).
     * @param[in]  matchingCharacteristicUUIDIn
     *              UUID-based filter for specifying characteristic in which the application
     *              is interested. By default it is set as the wildcard UUID_UKNOWN
     *              to match against any characteristic. If both service-UUID
     *              filter and characteristic-UUID filter are used with non-wildcard
     *              values, then only a single characteristic callback is
     *              invoked for the matching characteristic.
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
    virtual ble_error_t launchServiceDiscovery(Gap::Handle_t                               connectionHandle,
                                               ServiceDiscovery::ServiceCallback_t         sc                           = NULL,
                                               ServiceDiscovery::CharacteristicCallback_t  cc                           = NULL,
                                               const UUID                                 &matchingServiceUUID          = UUID::ShortUUIDBytes_t(BLE_UUID_UNKNOWN),
                                               const UUID                                 &matchingCharacteristicUUIDIn = UUID::ShortUUIDBytes_t(BLE_UUID_UNKNOWN)) {
        /* Avoid compiler warnings about unused variables. */
        (void)connectionHandle;
        (void)sc;
        (void)cc;
        (void)matchingServiceUUID;
        (void)matchingCharacteristicUUIDIn;

        return BLE_ERROR_NOT_IMPLEMENTED; /* Requesting action from porters: override this API if this capability is supported. */
    }

    /**
     * Launch service discovery for services. Once launched, service discovery will remain
     * active with service-callbacks being issued back into the application for matching
     * services. isServiceDiscoveryActive() can be used to
     * determine status, and a termination callback (if set up) will be invoked
     * at the end. Service discovery can be terminated prematurely, if needed,
     * using terminateServiceDiscovery().
     *
     * @param[in]  connectionHandle
     *              Handle for the connection with the peer.
     * @param[in]  callback
     *              This is the application callback for a matching service.
     *              Note: service discovery may still be active
     *              when this callback is issued; calling asynchronous BLE-stack
     *              APIs from within this application callback might cause the
     *              stack to abort service discovery. If this becomes an issue, it
     *              may be better to make a local copy of the discoveredService and
     *              wait for service discovery to terminate before operating on the
     *              service.
     * @param[in]  matchingServiceUUID
     *              UUID-based filter for specifying a service in which the application is
     *              interested. By default it is set as the wildcard UUID_UNKNOWN,
     *              in which case it matches all services.
     *
     * @return
     *           BLE_ERROR_NONE if service discovery is launched successfully; else an appropriate error.
     */
    virtual ble_error_t discoverServices(Gap::Handle_t                        connectionHandle,
                                         ServiceDiscovery::ServiceCallback_t  callback,
                                         const UUID                          &matchingServiceUUID = UUID::ShortUUIDBytes_t(BLE_UUID_UNKNOWN)) {
        return launchServiceDiscovery(connectionHandle, callback, NULL, matchingServiceUUID); /* We take advantage of the property
                                                                * that providing NULL for the characteristic callback will result in
                                                                * characteristic discovery being skipped for each matching
                                                                * service. This allows for an inexpensive method to discover only
                                                                * services. Porters are free to override this. */
    }

    /**
     * Launch service discovery for services. Once launched, service discovery will remain
     * active with service-callbacks being issued back into the application for matching
     * services. isServiceDiscoveryActive() can be used to
     * determine status, and a termination callback (if set up) will be invoked
     * at the end. Service discovery can be terminated prematurely, if needed,
     * using terminateServiceDiscovery().
     *
     * @param[in]  connectionHandle
     *              Handle for the connection with the peer.
     * @param[in]  callback
     *              This is the application callback for a matching service.
     *              Note: service discovery may still be active
     *              when this callback is issued; calling asynchronous BLE-stack
     *              APIs from within this application callback might cause the
     *              stack to abort service discovery. If this becomes an issue, it
     *              may be better to make a local copy of the discoveredService and
     *              wait for service discovery to terminate before operating on the
     *              service.
     * @param[in]  startHandle, endHandle
     *              Handle range within which to limit the search.
     *
     * @return
     *           BLE_ERROR_NONE if service discovery is launched successfully; else an appropriate error.
     */
    virtual ble_error_t discoverServices(Gap::Handle_t                        connectionHandle,
                                         ServiceDiscovery::ServiceCallback_t  callback,
                                         GattAttribute::Handle_t              startHandle,
                                         GattAttribute::Handle_t              endHandle) {
        /* Avoid compiler warnings about unused variables. */
        (void)connectionHandle;
        (void)callback;
        (void)startHandle;
        (void)endHandle;

        return BLE_ERROR_NOT_IMPLEMENTED; /* Requesting action from porters: override this API if this capability is supported. */
    }

    /**
     * Check if service-discovery is currently active.
     *
     * @return true if service-discovery is active, false otherwise.
     */
    virtual bool isServiceDiscoveryActive(void) const {
        return false; /* Requesting action from porters: override this API if this capability is supported. */
    }

    /**
     * Terminate an ongoing service discovery. This should result in an
     * invocation of TerminationCallback if service-discovery is active.
     */
    virtual void terminateServiceDiscovery(void) {
        /* Requesting action from porters: override this API if this capability is supported. */
    }

    /**
     * Initiate a GATT Client read procedure by attribute-handle.
     *
     * @param[in] connHandle
     *              Handle for the connection with the peer.
     * @param[in] attributeHandle
     *              Handle of the attribute to read data from.
     * @param[in] offset
     *              The offset from the start of the attribute value to be read.
     *
     * @return
     *          BLE_ERROR_NONE if read procedure was successfully started.
     */
    virtual ble_error_t read(Gap::Handle_t connHandle, GattAttribute::Handle_t attributeHandle, uint16_t offset) const {
        /* Avoid compiler warnings about unused variables. */
        (void)connHandle;
        (void)attributeHandle;
        (void)offset;

        return BLE_ERROR_NOT_IMPLEMENTED; /* Requesting action from porters: override this API if this capability is supported. */
    }

    /**
     * Initiate a GATT Client write procedure.
     *
     * @param[in] cmd
     *              Command can be either a write-request (which generates a
     *              matching response from the peripheral), or a write-command
     *              (which doesn't require the connected peer to respond).
     * @param[in] connHandle
     *              Connection handle.
     * @param[in] attributeHandle
     *              Handle for the target attribtue on the remote GATT server.
     * @param[in] length
     *              Length of the new value.
     * @param[in] value
     *              New value being written.
     *
     * @return
     *          BLE_ERROR_NONE if write procedure was successfully started.
     */
    virtual ble_error_t write(GattClient::WriteOp_t    cmd,
                              Gap::Handle_t            connHandle,
                              GattAttribute::Handle_t  attributeHandle,
                              size_t                   length,
                              const uint8_t           *value) const {
        /* Avoid compiler warnings about unused variables. */
        (void)cmd;
        (void)connHandle;
        (void)attributeHandle;
        (void)length;
        (void)value;

        return BLE_ERROR_NOT_IMPLEMENTED; /* Requesting action from porters: override this API if this capability is supported. */
    }

    /* Event callback handlers. */
public:
    /**
     * Set up a callback for read response events.
     *
     * @param[in] callback
     *              Event handler being registered.
     *
     * @note It is possible to chain together multiple onDataRead callbacks
     * (potentially from different modules of an application).
     *
     * @note It is possible to unregister a callback using
     * onDataRead().detach(callbackToRemove).
     */
    void onDataRead(ReadCallback_t callback) {
        onDataReadCallbackChain.add(callback);
    }

    /**
     * @brief Provide access to the callchain of read event callbacks.
     *
     * @return A reference to the read event callback chain.
     *
     * @note It is possible to register callbacks using onDataRead().add(callback).
     *
     * @note It is possible to unregister callbacks using onDataRead().detach(callback).
     */
    ReadCallbackChain_t& onDataRead() {
        return onDataReadCallbackChain;
    }

    /**
     * Set up a callback for write response events.
     *
     * @param[in] callback
     *              Event handler being registered.
     *
     * @note It is possible to remove registered callbacks using
     * onDataWritten().detach(callbackToRemove).
     *
     * @note  Write commands (issued using writeWoResponse) don't generate a response.
     */
    void onDataWritten(WriteCallback_t callback) {
        onDataWriteCallbackChain.add(callback);
    }

    /**
     * @brief Provide access to the callchain of data written callbacks.
     *
     * @return A reference to the data written callbacks chain.
     *
     * @note It is possible to register callbacks using onDataWritten().add(callback).
     *
     * @note It is possible to unregister callbacks using onDataWritten().detach(callback).
     */
    WriteCallbackChain_t& onDataWritten() {
        return onDataWriteCallbackChain;
    }

    /**
     * Set up a callback for write response events.
     *
     * @param[in] callback
     *              Event handler being registered.
     *
     * @note  Write commands (issued using writeWoResponse) don't generate a response.
     *
     * @deprecated Please use GattServer::onDataWritten() instead.
     */
    void onDataWrite(WriteCallback_t callback) {
        onDataWritten(callback);
    }

    /**
     * Set up a callback for when serviceDiscovery terminates.
     *
     * @param[in] callback
     *              Event handler being registered.
     */
    virtual void onServiceDiscoveryTermination(ServiceDiscovery::TerminationCallback_t callback) {
        (void)callback; /* Avoid compiler warnings about ununsed variables. */

        /* Requesting action from porters: override this API if this capability is supported. */
    }

    /**
     * @brief Launch discovery of descriptors for a given characteristic.
     *
     * @details This function will discover all descriptors available for a
     *          specific characteristic.
     *
     * @param[in] characteristic
     *              The characteristic targeted by this discovery procedure.
     * @param[in] discoveryCallback
     *              User function called each time a descriptor is found during
     *              the procedure.
     * @param[in] terminationCallback
     *              User provided function which will be called once the
     *              discovery procedure is terminating. This will get called
     *              when all the descriptors have been discovered or if an
     *              error occur during the discovery procedure.
     *
     * @return
     *   BLE_ERROR_NONE if characteristic descriptor discovery is launched
     *   successfully; else an appropriate error.
     */
    virtual ble_error_t discoverCharacteristicDescriptors(
        const DiscoveredCharacteristic& characteristic,
        const CharacteristicDescriptorDiscovery::DiscoveryCallback_t& discoveryCallback,
        const CharacteristicDescriptorDiscovery::TerminationCallback_t& terminationCallback) {
        (void) characteristic;
        (void) discoveryCallback;
        (void) terminationCallback;
        /* Requesting action from porter(s): override this API if this capability is supported. */
        return BLE_ERROR_NOT_IMPLEMENTED;
    }

    /**
     * @brief Indicate if the discovery of characteristic descriptors is active
     *        for a given characteristic or not.
     *
     * @param[in] characteristic
     *              The characteristic concerned by the descriptors discovery.
     *
     * @return true if a descriptors discovery is active for the characteristic
     *         in input; otherwise false.
     */
    virtual bool isCharacteristicDescriptorDiscoveryActive(const DiscoveredCharacteristic& characteristic) const
     {
        (void) characteristic;
        return false; /* Requesting action from porter(s): override this API if this capability is supported. */
    }

    /**
     * @brief Terminate an ongoing characteristic descriptor discovery.
     *
     * @details This should result in an invocation of the TerminationCallback if
     *          the characteristic descriptor discovery is active.
     *
     * @param[in] characteristic
     *              The characteristic on which the running descriptors
     *              discovery should be stopped.
     */
    virtual void terminateCharacteristicDescriptorDiscovery(const DiscoveredCharacteristic& characteristic) {
        /* Requesting action from porter(s): override this API if this capability is supported. */
        (void) characteristic;
    }

    /**
     * Set up a callback for when the GATT Client receives an update event
     * corresponding to a change in the value of a characteristic on the remote
     * GATT Server.
     *
     * @note It is possible to unregister callbacks using
     *       onHVX().detach(callbackToRemove).
     */
    void onHVX(HVXCallback_t callback) {
        onHVXCallbackChain.add(callback);
    }

    /**
     * Setup a callback to be invoked to notify the user application that the
     * GattClient instance is about to shutdown (possibly as a result of a call
     * to BLE::shutdown()).
     *
     * @param[in] callback
     *              Event handler being registered.
     *
     * @note  It is possible to chain together multiple onShutdown callbacks
     *        (potentially from different modules of an application) to be notified
     *        before the GattClient is shutdown.
     *
     * @note  It is also possible to set up a callback into a member function of
     *        some object.
     *
     * @note It is possible to unregister a callback using onShutdown().detach(callback).
     */
    void onShutdown(const GattClientShutdownCallback_t& callback) {
        shutdownCallChain.add(callback);
    }

    /**
     * Same as GattClient::onShutdown(), but allows the possibility to add an object
     * reference and member function as handler for shutdown event
     * callbacks.
     *
     * @param[in] objPtr
     *              Pointer to the object of a class defining the member callback
     *              function (@p memberPtr).
     * @param[in] memberPtr
     *              The member callback (within the context of an object) to be
     *              invoked.
     */
    template <typename T>
    void onShutdown(T *objPtr, void (T::*memberPtr)(const GattClient *)) {
        shutdownCallChain.add(objPtr, memberPtr);
    }

    /**
     * @brief Provide access to the callchain of shutdown event callbacks.
     *
     * @return A reference to the shutdown event callbacks chain.
     *
     * @note It is possible to register callbacks using onShutdown().add(callback).
     *
     * @note It is possible to unregister callbacks using onShutdown().detach(callback).
     */
    GattClientShutdownCallbackChain_t& onShutdown() {
        return shutdownCallChain;
    }

    /**
     * @brief provide access to the callchain of HVX callbacks.
     *
     * @return A reference to the HVX callbacks chain.
     *
     * @note It is possible to register callbacks using onHVX().add(callback).
     *
     * @note It is possible to unregister callbacks using onHVX().detach(callback).
     */
    HVXCallbackChain_t& onHVX() {
        return onHVXCallbackChain;
    }

public:
    /**
     * Notify all registered onShutdown callbacks that the GattClient is
     * about to be shutdown and clear all GattClient state of the
     * associated object.
     *
     * This function is meant to be overridden in the platform-specific
     * sub-class. Nevertheless, the sub-class is only expected to reset its
     * state and not the data held in GattClient members. This shall be achieved
     * by a call to GattClient::reset() from the sub-class' reset()
     * implementation.
     *
     * @return BLE_ERROR_NONE on success.
     */
    virtual ble_error_t reset(void) {
        /* Notify that the instance is about to shutdown */
        shutdownCallChain.call(this);
        shutdownCallChain.clear();

        onDataReadCallbackChain.clear();
        onDataWriteCallbackChain.clear();
        onHVXCallbackChain.clear();

        return BLE_ERROR_NONE;
    }

protected:
    GattClient() {
        /* Empty */
    }

    /* Entry points for the underlying stack to report events back to the user. */
public:
    /**
     * Helper function that notifies all registered handlers of an occurrence
     * of a data read event. This function is meant to be called from the
     * BLE stack specific implementation when a data read event occurs.
     *
     * @param[in] params
     *              The data read parameters passed to the registered
     *              handlers.
     */
    void processReadResponse(const GattReadCallbackParams *params) {
        onDataReadCallbackChain(params);
    }

    /**
     * Helper function that notifies all registered handlers of an occurrence
     * of a data written event. This function is meant to be called from the
     * BLE stack specific implementation when a data written event occurs.
     *
     * @param[in] params
     *              The data written parameters passed to the registered
     *              handlers.
     */
    void processWriteResponse(const GattWriteCallbackParams *params) {
        onDataWriteCallbackChain(params);
    }

    /**
     * Helper function that notifies all registered handlers of an occurrence
     * of an update event. This function is meant to be called from the
     * BLE stack specific implementation when an update event occurs.
     *
     * @param[in] params
     *              The update event parameters passed to the registered
     *              handlers.
     */
    void processHVXEvent(const GattHVXCallbackParams *params) {
        if (onHVXCallbackChain) {
            onHVXCallbackChain(params);
        }
    }

protected:
    /**
     * Callchain containing all registered callback handlers for data read
     * events.
     */
    ReadCallbackChain_t               onDataReadCallbackChain;
    /**
     * Callchain containing all registered callback handlers for data write
     * events.
     */
    WriteCallbackChain_t              onDataWriteCallbackChain;
    /**
     * Callchain containing all registered callback handlers for update
     * events.
     */
    HVXCallbackChain_t                onHVXCallbackChain;
    /**
     * Callchain containing all registered callback handlers for shutdown
     * events.
     */
    GattClientShutdownCallbackChain_t shutdownCallChain;

private:
    /* Disallow copy and assignment. */
    GattClient(const GattClient &);
    GattClient& operator=(const GattClient &);
};

#endif /* ifndef __GATT_CLIENT_H__ */
