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

#ifndef __GATT_SERVER_H__
#define __GATT_SERVER_H__

#include "Gap.h"
#include "GattService.h"
#include "GattAttribute.h"
#include "GattServerEvents.h"
#include "GattCallbackParamTypes.h"
#include "CallChainOfFunctionPointersWithContext.h"

class GattServer {
public:
    /**
     * Type for the registered callbacks added to the data sent callchain.
     * Refer to GattServer::onDataSent().
     */
    typedef FunctionPointerWithContext<unsigned> DataSentCallback_t;
    /**
     * Type for the data sent event callchain. Refer to GattServer::onDataSent().
     */
    typedef CallChainOfFunctionPointersWithContext<unsigned> DataSentCallbackChain_t;

    /**
     * Type for the registered callbacks added to the data written callchain.
     * Refer to GattServer::onDataWritten().
     */
    typedef FunctionPointerWithContext<const GattWriteCallbackParams*> DataWrittenCallback_t;
    /**
     * Type for the data written event callchain. Refer to GattServer::onDataWritten().
     */
    typedef CallChainOfFunctionPointersWithContext<const GattWriteCallbackParams*> DataWrittenCallbackChain_t;

    /**
     * Type for the registered callbacks added to the data read callchain.
     * Refer to GattServer::onDataRead().
     */
    typedef FunctionPointerWithContext<const GattReadCallbackParams*> DataReadCallback_t;
    /**
     * Type for the data read event callchain. Refer to GattServer::onDataRead().
     */
    typedef CallChainOfFunctionPointersWithContext<const GattReadCallbackParams *> DataReadCallbackChain_t;

    /**
     * Type for the registered callbacks added to the shutdown callchain.
     * Refer to GattServer::onShutdown().
     */
    typedef FunctionPointerWithContext<const GattServer *> GattServerShutdownCallback_t;
    /**
     * Type for the shutdown event callchain. Refer to GattServer::onShutdown().
     */
    typedef CallChainOfFunctionPointersWithContext<const GattServer *> GattServerShutdownCallbackChain_t;

    /**
     * Type for the registered callback for various events. Refer to
     * GattServer::onUpdatesEnabled(), GattServer::onUpdateDisabled() and
     * GattServer::onConfirmationReceived().
     */
    typedef FunctionPointerWithContext<GattAttribute::Handle_t> EventCallback_t;

protected:
    /**
     * Construct a GattServer instance.
     */
    GattServer() :
        serviceCount(0),
        characteristicCount(0),
        dataSentCallChain(),
        dataWrittenCallChain(),
        dataReadCallChain(),
        updatesEnabledCallback(NULL),
        updatesDisabledCallback(NULL),
        confirmationReceivedCallback(NULL) {
        /* empty */
    }

    /*
     * The following functions are meant to be overridden in the platform-specific sub-class.
     */
public:

    /**
     * Add a service declaration to the local server ATT table. Also add the
     * characteristics contained within.
     *
     * @param[in] service
     *              The service to be added.
     *
     * @return BLE_ERROR_NONE if the service was successfully added.
     */
    virtual ble_error_t addService(GattService &service) {
        /* Avoid compiler warnings about unused variables. */
        (void)service;

        return BLE_ERROR_NOT_IMPLEMENTED; /* Requesting action from porters: override this API if this capability is supported. */
    }

    /**
     * Read the value of a characteristic from the local GATT server.
     *
     * @param[in]     attributeHandle
     *                  Attribute handle for the value attribute of the characteristic.
     * @param[out]    buffer
     *                  A buffer to hold the value being read.
     * @param[in,out] lengthP
     *                  Length of the buffer being supplied. If the attribute
     *                  value is longer than the size of the supplied buffer,
     *                  this variable will hold upon return the total attribute value length
     *                  (excluding offset). The application may use this
     *                  information to allocate a suitable buffer size.
     *
     * @return BLE_ERROR_NONE if a value was read successfully into the buffer.
     */
    virtual ble_error_t read(GattAttribute::Handle_t attributeHandle, uint8_t buffer[], uint16_t *lengthP) {
        /* Avoid compiler warnings about unused variables. */
        (void)attributeHandle;
        (void)buffer;
        (void)lengthP;

        return BLE_ERROR_NOT_IMPLEMENTED; /* Requesting action from porters: override this API if this capability is supported. */
    }

    /**
     * Read the value of a characteristic from the local GATT server.
     *
     * @param[in]     connectionHandle
     *                  Connection handle.
     * @param[in]     attributeHandle
     *                  Attribute handle for the value attribute of the characteristic.
     * @param[out]    buffer
     *                  A buffer to hold the value being read.
     * @param[in,out] lengthP
     *                  Length of the buffer being supplied. If the attribute
     *                  value is longer than the size of the supplied buffer,
     *                  this variable will hold upon return the total attribute value length
     *                  (excluding offset). The application may use this
     *                  information to allocate a suitable buffer size.
     *
     * @return BLE_ERROR_NONE if a value was read successfully into the buffer.
     *
     * @note This API is a version of the above, with an additional connection handle
     *     parameter to allow fetches for connection-specific multivalued
     *     attributes (such as the CCCDs).
     */
    virtual ble_error_t read(Gap::Handle_t connectionHandle, GattAttribute::Handle_t attributeHandle, uint8_t *buffer, uint16_t *lengthP) {
        /* Avoid compiler warnings about unused variables. */
        (void)connectionHandle;
        (void)attributeHandle;
        (void)buffer;
        (void)lengthP;

        return BLE_ERROR_NOT_IMPLEMENTED; /* Requesting action from porters: override this API if this capability is supported. */
    }

    /**
     * Update the value of a characteristic on the local GATT server.
     *
     * @param[in] attributeHandle
     *              Handle for the value attribute of the characteristic.
     * @param[in] value
     *              A pointer to a buffer holding the new value.
     * @param[in] size
     *              Size of the new value (in bytes).
     * @param[in] localOnly
     *              Should this update be kept on the local
     *              GATT server regardless of the state of the
     *              notify/indicate flag in the CCCD for this
     *              Characteristic? If set to true, no notification
     *              or indication is generated.
     *
     * @return BLE_ERROR_NONE if we have successfully set the value of the attribute.
     */
    virtual ble_error_t write(GattAttribute::Handle_t attributeHandle, const uint8_t *value, uint16_t size, bool localOnly = false) {
        /* Avoid compiler warnings about unused variables. */
        (void)attributeHandle;
        (void)value;
        (void)size;
        (void)localOnly;

        return BLE_ERROR_NOT_IMPLEMENTED; /* Requesting action from porters: override this API if this capability is supported. */
    }

    /**
     * Update the value of a characteristic on the local GATT server. A version
     * of the same as the above, with a connection handle parameter to allow updates
     * for connection-specific multivalued attributes (such as the CCCDs).
     *
     * @param[in] connectionHandle
     *              Connection handle.
     * @param[in] attributeHandle
     *              Handle for the value attribute of the characteristic.
     * @param[in] value
     *              A pointer to a buffer holding the new value.
     * @param[in] size
     *              Size of the new value (in bytes).
     * @param[in] localOnly
     *              Should this update be kept on the local
     *              GattServer regardless of the state of the
     *              notify/indicate flag in the CCCD for this
     *              Characteristic? If set to true, no notification
     *              or indication is generated.
     *
     * @return BLE_ERROR_NONE if we have successfully set the value of the attribute.
     */
    virtual ble_error_t write(Gap::Handle_t connectionHandle, GattAttribute::Handle_t attributeHandle, const uint8_t *value, uint16_t size, bool localOnly = false) {
        /* Avoid compiler warnings about unused variables. */
        (void)connectionHandle;
        (void)attributeHandle;
        (void)value;
        (void)size;
        (void)localOnly;

        return BLE_ERROR_NOT_IMPLEMENTED; /* Requesting action from porters: override this API if this capability is supported. */
    }

    /**
     * Determine the updates-enabled status (notification or indication) for the current connection from a characteristic's CCCD.
     *
     * @param[in]   characteristic
     *                The characteristic.
     * @param[out]  enabledP
     *                Upon return, *enabledP is true if updates are enabled, else false.
     *
     * @return BLE_ERROR_NONE if the connection and handle are found. False otherwise.
     */
    virtual ble_error_t areUpdatesEnabled(const GattCharacteristic &characteristic, bool *enabledP) {
        /* Avoid compiler warnings about unused variables. */
        (void)characteristic;
        (void)enabledP;

        return BLE_ERROR_NOT_IMPLEMENTED; /* Requesting action from porters: override this API if this capability is supported. */
    }

    /**
     * Determine the connection-specific updates-enabled status (notification or indication) from a characteristic's CCCD.
     *
     * @param[in]   connectionHandle
     *                The connection handle.
     * @param[in]  characteristic
     *                The characteristic.
     * @param[out]  enabledP
     *                Upon return, *enabledP is true if updates are enabled, else false.
     *
     * @return BLE_ERROR_NONE if the connection and handle are found. False otherwise.
     */
    virtual ble_error_t areUpdatesEnabled(Gap::Handle_t connectionHandle, const GattCharacteristic &characteristic, bool *enabledP) {
        /* Avoid compiler warnings about unused variables. */
        (void)connectionHandle;
        (void)characteristic;
        (void)enabledP;

        return BLE_ERROR_NOT_IMPLEMENTED; /* Requesting action from porters: override this API if this capability is supported. */
    }

    /**
     * A virtual function to allow underlying stacks to indicate if they support
     * onDataRead(). It should be overridden to return true as applicable.
     *
     * @return true if onDataRead is supported, false otherwise.
     */
    virtual bool isOnDataReadAvailable() const {
        return false; /* Requesting action from porters: override this API if this capability is supported. */
    }

    /*
     * APIs with non-virtual implementations.
     */
public:
    /**
     * Add a callback for the GATT event DATA_SENT (which is triggered when
     * updates are sent out by GATT in the form of notifications).
     *
     * @param[in] callback
     *              Event handler being registered.
     *
     * @note It is possible to chain together multiple onDataSent callbacks
     *       (potentially from different modules of an application) to receive updates
     *       to characteristics.
     *
     * @note It is also possible to set up a callback into a member function of
     *       some object.
     */
    void onDataSent(const DataSentCallback_t& callback) {
        dataSentCallChain.add(callback);
    }

    /**
     * Same as GattServer::onDataSent(), but allows the possibility to add an object
     * reference and member function as handler for DATA_SENT event
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
    void onDataSent(T *objPtr, void (T::*memberPtr)(unsigned count)) {
        dataSentCallChain.add(objPtr, memberPtr);
    }

    /**
     * @brief Provide access to the callchain of DATA_SENT event callbacks.
     *
     * @return A reference to the DATA_SENT event callback chain.
     */
    DataSentCallbackChain_t& onDataSent() {
        return dataSentCallChain;
    }

    /**
     * Set up a callback for when an attribute has its value updated by or at the
     * connected peer. For a peripheral, this callback is triggered when the local
     * GATT server has an attribute updated by a write command from the peer.
     * For a central, this callback is triggered when a response is received for
     * a write request.
     *
     * @param[in] callback
     *              Event handler being registered.
     *
     * @note  It is possible to chain together multiple onDataWritten callbacks
     * (potentially from different modules of an application) to receive updates
     * to characteristics. Many services, such as DFU and UART, add their own
     * onDataWritten callbacks behind the scenes to trap interesting events.
     *
     * @note  It is also possible to set up a callback into a member function of
     * some object.
     *
     * @note It is possible to unregister a callback using onDataWritten().detach(callback)
     */
    void onDataWritten(const DataWrittenCallback_t& callback) {
        dataWrittenCallChain.add(callback);
    }

    /**
     * Same as GattServer::onDataWritten(), but allows the possibility to add an object
     * reference and member function as handler for data written event
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
    void onDataWritten(T *objPtr, void (T::*memberPtr)(const GattWriteCallbackParams *context)) {
        dataWrittenCallChain.add(objPtr, memberPtr);
    }

    /**
     * @brief Provide access to the callchain of data written event callbacks.
     *
     * @return A reference to the data written event callbacks chain.
     *
     * @note It is possible to register callbacks using onDataWritten().add(callback).
     *
     * @note It is possible to unregister callbacks using onDataWritten().detach(callback).
     */
    DataWrittenCallbackChain_t& onDataWritten() {
        return dataWrittenCallChain;
    }

    /**
     * Setup a callback to be invoked on the peripheral when an attribute is
     * being read by a remote client.
     *
     * @param[in] callback
     *              Event handler being registered.
     *
     * @return BLE_ERROR_NOT_IMPLEMENTED if this functionality isn't available;
     *         else BLE_ERROR_NONE.
     *
     * @note  This functionality may not be available on all underlying stacks.
     * You could use GattCharacteristic::setReadAuthorizationCallback() as an
     * alternative. Refer to isOnDataReadAvailable().
     *
     * @note  It is possible to chain together multiple onDataRead callbacks
     * (potentially from different modules of an application) to receive updates
     * to characteristics. Services may add their own onDataRead callbacks
     * behind the scenes to trap interesting events.
     *
     * @note  It is also possible to set up a callback into a member function of
     * some object.
     *
     * @note It is possible to unregister a callback using onDataRead().detach(callback).
     */
    ble_error_t onDataRead(const DataReadCallback_t& callback) {
        if (!isOnDataReadAvailable()) {
            return BLE_ERROR_NOT_IMPLEMENTED;
        }

        dataReadCallChain.add(callback);
        return BLE_ERROR_NONE;
    }

    /**
     * Same as GattServer::onDataRead(), but allows the possibility to add an object
     * reference and member function as handler for data read event
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
    ble_error_t onDataRead(T *objPtr, void (T::*memberPtr)(const GattReadCallbackParams *context)) {
        if (!isOnDataReadAvailable()) {
            return BLE_ERROR_NOT_IMPLEMENTED;
        }

        dataReadCallChain.add(objPtr, memberPtr);
        return BLE_ERROR_NONE;
    }

    /**
     * @brief Provide access to the callchain of data read event callbacks.
     *
     * @return A reference to the data read event callbacks chain.
     *
     * @note It is possible to register callbacks using onDataRead().add(callback).
     *
     * @note It is possible to unregister callbacks using onDataRead().detach(callback).
     */
    DataReadCallbackChain_t& onDataRead() {
        return dataReadCallChain;
    }

    /**
     * Setup a callback to be invoked to notify the user application that the
     * GattServer instance is about to shutdown (possibly as a result of a call
     * to BLE::shutdown()).
     *
     * @param[in] callback
     *              Event handler being registered.
     *
     * @note  It is possible to chain together multiple onShutdown callbacks
     * (potentially from different modules of an application) to be notified
     * before the GattServer is shutdown.
     *
     * @note  It is also possible to set up a callback into a member function of
     * some object.
     *
     * @note It is possible to unregister a callback using onShutdown().detach(callback)
     */
    void onShutdown(const GattServerShutdownCallback_t& callback) {
        shutdownCallChain.add(callback);
    }

    /**
     * Same as GattServer::onShutdown(), but allows the possibility to add an object
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
    void onShutdown(T *objPtr, void (T::*memberPtr)(const GattServer *)) {
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
    GattServerShutdownCallbackChain_t& onShutdown() {
        return shutdownCallChain;
    }

    /**
     * Set up a callback for when notifications or indications are enabled for a
     * characteristic on the local GATT server.
     *
     * @param[in] callback
     *              Event handler being registered.
     */
    void onUpdatesEnabled(EventCallback_t callback) {
        updatesEnabledCallback = callback;
    }

    /**
     * Set up a callback for when notifications or indications are disabled for a
     * characteristic on the local GATT server.
     *
     * @param[in] callback
     *              Event handler being registered.
     */
    void onUpdatesDisabled(EventCallback_t callback) {
        updatesDisabledCallback = callback;
    }

    /**
     * Set up a callback for when the GATT server receives a response for an
     * indication event sent previously.
     *
     * @param[in] callback
     *              Event handler being registered.
     */
    void onConfirmationReceived(EventCallback_t callback) {
        confirmationReceivedCallback = callback;
    }

    /* Entry points for the underlying stack to report events back to the user. */
protected:
    /**
     * Helper function that notifies all registered handlers of an occurrence
     * of a data written event. This function is meant to be called from the
     * BLE stack specific implementation when a data written event occurs.
     *
     * @param[in] params
     *              The data written parameters passed to the registered
     *              handlers.
     */
    void handleDataWrittenEvent(const GattWriteCallbackParams *params) {
        dataWrittenCallChain.call(params);
    }

    /**
     * Helper function that notifies all registered handlers of an occurrence
     * of a data read event. This function is meant to be called from the
     * BLE stack specific implementation when a data read event occurs.
     *
     * @param[in] params
     *              The data read parameters passed to the registered
     *              handlers.
     */
    void handleDataReadEvent(const GattReadCallbackParams *params) {
        dataReadCallChain.call(params);
    }

    /**
     * Helper function that notifies the registered handler of an occurrence
     * of updates enabled, updates disabled and confirmation received events.
     * This function is meant to be called from the BLE stack specific
     * implementation when any of these events occurs.
     *
     * @param[in] type
     *              The type of event that occurred.
     * @param[in] attributeHandle
     *              The handle of the attribute that was modified.
     */
    void handleEvent(GattServerEvents::gattEvent_e type, GattAttribute::Handle_t attributeHandle) {
        switch (type) {
            case GattServerEvents::GATT_EVENT_UPDATES_ENABLED:
                if (updatesEnabledCallback) {
                    updatesEnabledCallback(attributeHandle);
                }
                break;
            case GattServerEvents::GATT_EVENT_UPDATES_DISABLED:
                if (updatesDisabledCallback) {
                    updatesDisabledCallback(attributeHandle);
                }
                break;
            case GattServerEvents::GATT_EVENT_CONFIRMATION_RECEIVED:
                if (confirmationReceivedCallback) {
                    confirmationReceivedCallback(attributeHandle);
                }
                break;
            default:
                break;
        }
    }

    /**
     * Helper function that notifies all registered handlers of an occurrence
     * of a data sent event. This function is meant to be called from the
     * BLE stack specific implementation when a data sent event occurs.
     *
     * @param[in] count
     *              Number of packets sent.
     */
    void handleDataSentEvent(unsigned count) {
        dataSentCallChain.call(count);
    }

public:
    /**
     * Notify all registered onShutdown callbacks that the GattServer is
     * about to be shutdown and clear all GattServer state of the
     * associated object.
     *
     * This function is meant to be overridden in the platform-specific
     * sub-class. Nevertheless, the sub-class is only expected to reset its
     * state and not the data held in GattServer members. This shall be achieved
     * by a call to GattServer::reset() from the sub-class' reset()
     * implementation.
     *
     * @return BLE_ERROR_NONE on success.
     */
    virtual ble_error_t reset(void) {
        /* Notify that the instance is about to shutdown */
        shutdownCallChain.call(this);
        shutdownCallChain.clear();

        serviceCount = 0;
        characteristicCount = 0;

        dataSentCallChain.clear();
        dataWrittenCallChain.clear();
        dataReadCallChain.clear();
        updatesEnabledCallback       = NULL;
        updatesDisabledCallback      = NULL;
        confirmationReceivedCallback = NULL;

        return BLE_ERROR_NONE;
    }

protected:
    /**
     * The total number of services added to the ATT table.
     */
    uint8_t serviceCount;
    /**
     * The total number of characteristics added to the ATT table.
     */
    uint8_t characteristicCount;

private:
    /**
     * Callchain containing all registered callback handlers for data sent
     * events.
     */
    DataSentCallbackChain_t           dataSentCallChain;
    /**
     * Callchain containing all registered callback handlers for data written
     * events.
     */
    DataWrittenCallbackChain_t        dataWrittenCallChain;
    /**
     * Callchain containing all registered callback handlers for data read
     * events.
     */
    DataReadCallbackChain_t           dataReadCallChain;
    /**
     * Callchain containing all registered callback handlers for shutdown
     * events.
     */
    GattServerShutdownCallbackChain_t shutdownCallChain;
    /**
     * The registered callback handler for updates enabled events.
     */
    EventCallback_t                   updatesEnabledCallback;
    /**
     * The registered callback handler for updates disabled events.
     */
    EventCallback_t                   updatesDisabledCallback;
    /**
     * The registered callback handler for confirmation received events.
     */
    EventCallback_t                   confirmationReceivedCallback;

private:
    /* Disallow copy and assignment. */
    GattServer(const GattServer &);
    GattServer& operator=(const GattServer &);
};

#endif /* ifndef __GATT_SERVER_H__ */
