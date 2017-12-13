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

#ifndef MBED_GATT_SERVER_H__
#define MBED_GATT_SERVER_H__

#include "Gap.h"
#include "GattService.h"
#include "GattAttribute.h"
#include "GattServerEvents.h"
#include "GattCallbackParamTypes.h"
#include "CallChainOfFunctionPointersWithContext.h"

/**
 * @addtogroup ble
 * @{
 * @addtogroup gatt
 * @{
 * @addtogroup server
 * @{
 */

/**
 * Construct and operates a GATT server.
 *
 * A Gatt server is a collection of GattService; these services contain
 * characteristics that a peer connected to the device may read or write.
 * These characteristics may also emit updates to subscribed clients when their
 * values change.
 *
 * @p Server Layout
 *
 * Application code can add a GattService object to the server with the help of
 * the function addService(). That function registers all the GattCharacteristic
 * enclosed in the service, as well as all the characteristics descriptors (see
 * GattAttribute) these characteristics contain. Service registration assigns
 * a unique handle to the various attributes being part of the service; this
 * handle should be used for subsequent read or write of these components.
 *
 * There are no primitives defined to remove a single service; however, a call to
 * the function reset() removes all services previously registered in the
 * GattServer.
 *
 * @p Characteristic and attributes access
 *
 * Values of the characteristic and the characteristic descriptor present in the
 * GattServer must be accessed through the handle assigned to them when the service
 * has been registered; the GattServer class offers several flavors of read()
 * and write() functions that retrieve or mutate an attribute value.
 *
 * Application code can query if a client has subscribed to a given
 * characteristic's value update by invoking the function areUpdatesEnabled().
 *
 * @p Events
 *
 * The GattServer allows application code to register several event handlers that
 * can be used to monitor client and server activities:
 *   - onDataSent(): Register an event handler that is called when a
 *     characteristic value update has been sent to a client.
 *   - onDataWriten(): Register an event handler that is called when a
 *     client has written an attribute of the server.
 *   - onDataRead(): Register an event handler that is called when a
 *     client has read an attribute of the server.
 *   - onUpdatesEnabled: Register an event handler that is called when a
 *     client subscribes to updates of a characteristic.
 *   - onUpdatesDisabled: Register an event handler that is called when a
 *     client unsubscribes from updates of a characteristic.
 *   - onConfimationReceived: Register an event handler that is called
 *     when a client acknowledges a characteristic value notification.
 *
 * @note The term characteristic value update is used to represent
 * Characteristic Value Notification and Characteristic Value Indication when
 * the nature of the server initiated is not relevant.
 */
class GattServer {
public:
    /**
     * Event handler invoked when the server has sent data to a client.
     *
     * @see onDataSent().
     */
    typedef FunctionPointerWithContext<unsigned> DataSentCallback_t;

    /**
     * Callchain of DataSentCallback_t objects.
     *
     * @see onDataSent().
     */
    typedef CallChainOfFunctionPointersWithContext<unsigned>
        DataSentCallbackChain_t;

    /**
     * Event handler invoked when the client has written an attribute of the
     * server.
     *
     * @see onDataWritten().
     */
    typedef FunctionPointerWithContext<const GattWriteCallbackParams*>
        DataWrittenCallback_t;

    /**
     * Callchain of DataWrittenCallback_t objects.
     *
     * @see onDataWritten().
     */
    typedef CallChainOfFunctionPointersWithContext<const GattWriteCallbackParams*>
        DataWrittenCallbackChain_t;

    /**
     * Event handler invoked when the client has read an attribute of the server.
     *
     * @see onDataRead().
     */
    typedef FunctionPointerWithContext<const GattReadCallbackParams*>
        DataReadCallback_t;

    /**
     * Callchain of DataReadCallback_t.
     *
     * @see onDataRead().
     */
    typedef CallChainOfFunctionPointersWithContext<const GattReadCallbackParams*>
        DataReadCallbackChain_t;

    /**
     * Event handler invoked when the GattServer is reset.
     *
     * @see onShutdown() reset()
     */
    typedef FunctionPointerWithContext<const GattServer *>
        GattServerShutdownCallback_t;

    /**
     * Callchain of GattServerShutdownCallback_t.
     *
     * @see onShutdown() reset()
     */
    typedef CallChainOfFunctionPointersWithContext<const GattServer*>
        GattServerShutdownCallbackChain_t;

    /**
     * Event handler that handles subscription to characteristic updates,
     * unsubscription from characteristic updates and notification confirmation.
     *
     * @see onUpdatesEnabled() onUpdateDisabled() onConfirmationReceived()
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
    }

    /*
     * The following functions are meant to be overridden in the platform
     * specific subclass.
     */
public:

    /**
     * Add a service declaration to the local attribute server table.
     *
     * This functions inserts a service declaration in the attribute table
     * followed by the characteristic declarations (including characteristic
     * descriptors) present in @p service.
     *
     * The process assigns a unique attribute handle to all the elements added
     * into the attribute table. This handle is an ID that must be used for
     * subsequent interractions with the elements.
     *
     * @note There is no mirror function that removes a single service.
     * Application code can remove all the registered services by calling
     * reset().
     *
     * @attention Service, characteristics and descriptors objects registered
     * within the GattServer must remain reachable until reset() is called.
     *
     * @param[in] service The service to be added; attribute handle of services,
     * characteristic and characteristic descriptors are updated by the
     * process.
     *
     * @return BLE_ERROR_NONE if the service was successfully added.
     */
    virtual ble_error_t addService(GattService &service)
    {
        /* Avoid compiler warnings about unused variables. */
        (void)service;

        /* Requesting action from porters: override this API if this capability
           is supported. */
        return BLE_ERROR_NOT_IMPLEMENTED;
    }

    /**
     * Read the value of an attribute present in the local GATT server.
     *
     * @param[in] attributeHandle Handle of the attribute to read.
     * @param[out] buffer A buffer to hold the value being read.
     * @param[in,out] lengthP Length of the buffer being supplied. If the
     * attribute value is longer than the size of the supplied buffer, this
     * variable holds upon return the total attribute value length (excluding
     * offset). The application may use this information to allocate a suitable
     * buffer size.
     *
     * @return BLE_ERROR_NONE if a value was read successfully into the buffer.
     *
     * @attention read(Gap::Handle_t, GattAttribute::Handle_t, uint8_t *, uint16_t *)
     * must be used to read Client Characteristic Configuration Descriptor (CCCD)
     * because the value of this type of attribute depends on the connection.
     */
    virtual ble_error_t read(
        GattAttribute::Handle_t attributeHandle,
        uint8_t buffer[],
        uint16_t *lengthP
    ) {
        /* Avoid compiler warnings about unused variables. */
        (void)attributeHandle;
        (void)buffer;
        (void)lengthP;

        /* Requesting action from porters: override this API if this capability
           is supported. */
        return BLE_ERROR_NOT_IMPLEMENTED;
    }

    /**
     * Read the value of an attribute present in the local GATT server.
     *
     * The connection handle allows application code to read the value of a
     * Client Characteristic Configuration Descriptor for a given connection.
     *
     * @param[in] connectionHandle Connection handle.
     * @param[in] attributeHandle Attribute handle for the value attribute of
     * the characteristic.
     * @param[out] buffer A buffer to hold the value being read.
     * @param[in,out] lengthP Length of the buffer being supplied. If the
     * attribute value is longer than the size of the supplied buffer, this
     * variable holds upon return the total attribute value length (excluding
     * offset). The application may use this information to allocate a suitable
     * buffer size.
     *
     * @return BLE_ERROR_NONE if a value was read successfully into the buffer.
     */
    virtual ble_error_t read(
        Gap::Handle_t connectionHandle,
        GattAttribute::Handle_t attributeHandle,
        uint8_t *buffer,
        uint16_t *lengthP
    ) {
        /* Avoid compiler warnings about unused variables. */
        (void)connectionHandle;
        (void)attributeHandle;
        (void)buffer;
        (void)lengthP;

        /* Requesting action from porters: override this API if this capability
           is supported. */
        return BLE_ERROR_NOT_IMPLEMENTED;
    }

    /**
     * Update the value of an attribute present in the local GATT server.
     *
     * @param[in] attributeHandle Handle of the attribute to write.
     * @param[in] value A pointer to a buffer holding the new value.
     * @param[in] size Size in bytes of the new value (in bytes).
     * @param[in] localOnly If this flag is false and the attribute handle
     * written is a characteristic value, then the server sends an update
     * containing the new value to all clients that have subscribed to the
     * characteristic's notifications or indications. Otherwise, the update does
     * not generate a single server initiated event.
     *
     * @return BLE_ERROR_NONE if the attribute value has been successfully
     * updated.
     */
    virtual ble_error_t write(
        GattAttribute::Handle_t attributeHandle,
        const uint8_t *value,
        uint16_t size,
        bool localOnly = false
    ) {
        /* Avoid compiler warnings about unused variables. */
        (void)attributeHandle;
        (void)value;
        (void)size;
        (void)localOnly;

        /* Requesting action from porters: override this API if this capability
           is supported. */
        return BLE_ERROR_NOT_IMPLEMENTED;
    }

    /**
     * Update the value of an attribute present in the local GATT server.
     *
     * The connection handle parameter allows application code to direct
     * notification or indication resulting from the update to a specific client.
     *
     * @param[in] connectionHandle Connection handle.
     * @param[in] attributeHandle Handle for the value attribute of the
     * characteristic.
     * @param[in] value A pointer to a buffer holding the new value.
     * @param[in] size Size of the new value (in bytes).
     * @param[in] localOnly If this flag is false and the attribute handle
     * written is a characteristic value, then the server sends an update
     * containing the new value to the client identified by the parameter
     * @p connectionHandle if it is subscribed to the characteristic's
     * notifications or indications. Otherwise, the update does not generate a
     * single server initiated event.
     *
     * @return BLE_ERROR_NONE if the attribute value has been successfully
     * updated.
     */
    virtual ble_error_t write(
        Gap::Handle_t connectionHandle,
        GattAttribute::Handle_t attributeHandle,
        const uint8_t *value,
        uint16_t size,
        bool localOnly = false
    ) {
        /* Avoid compiler warnings about unused variables. */
        (void)connectionHandle;
        (void)attributeHandle;
        (void)value;
        (void)size;
        (void)localOnly;

        /* Requesting action from porters: override this API if this capability
           is supported. */
        return BLE_ERROR_NOT_IMPLEMENTED;
    }

    /**
     * Determine if one of the connected clients has subscribed to notifications
     * or indications of the characteristic in input.
     *
     * @param[in] characteristic The characteristic.
     * @param[out] enabledP Upon return, *enabledP is true if updates are
     * enabled for a connected client; otherwise, *enabledP is false.
     *
     * @return BLE_ERROR_NONE if the connection and handle are found. False
     * otherwise.
     */
    virtual ble_error_t areUpdatesEnabled(
        const GattCharacteristic &characteristic,
        bool *enabledP
    ) {
        /* Avoid compiler warnings about unused variables. */
        (void)characteristic;
        (void)enabledP;

        /* Requesting action from porters: override this API if this capability
           is supported. */
        return BLE_ERROR_NOT_IMPLEMENTED;
    }

    /**
     * Determine if an identified client has subscribed to notifications or
     * indications of a given characteristic.
     *
     * @param[in] connectionHandle The connection handle.
     * @param[in] characteristic The characteristic.
     * @param[out] enabledP Upon return, *enabledP is true if the client
     * identified by @p connectionHandle has subscribed to notifications or
     * indications of @p characteristic; otherwise, *enabledP is false.
     *
     * @return BLE_ERROR_NONE if the connection and handle are found. False
     * otherwise.
     */
    virtual ble_error_t areUpdatesEnabled(
        Gap::Handle_t connectionHandle,
        const GattCharacteristic &characteristic,
        bool *enabledP
    ) {
        /* Avoid compiler warnings about unused variables. */
        (void)connectionHandle;
        (void)characteristic;
        (void)enabledP;

        /* Requesting action from porters: override this API if this capability
           is supported. */
        return BLE_ERROR_NOT_IMPLEMENTED;
    }

    /**
     * Indicate if the underlying stack emit events when an attribute is read by
     * a client.
     *
     * @attention This function should be overridden to return true if
     * applicable.
     *
     * @return true if onDataRead is supported; false otherwise.
     */
    virtual bool isOnDataReadAvailable() const
    {
        /* Requesting action from porters: override this API if this capability
           is supported. */
        return false;
    }

    /*
     * APIs with nonvirtual implementations.
     */
public:
    /**
     * Add an event handler that monitors emission of characteristic value
     * updates.
     *
     * @param[in] callback Event handler being registered.
     *
     * @note It is possible to chain together multiple onDataSent callbacks
     * (potentially from different modules of an application).
     */
    void onDataSent(const DataSentCallback_t &callback)
    {
        dataSentCallChain.add(callback);
    }

    /**
     * Add an event handler that monitors emission of characteristic value
     * updates.
     *
     * @param[in] objPtr Pointer to the instance that is used to invoke the
     * event handler.
     * @param[in] memberPtr Event handler being registered. It is a member
     * function.
     */
    template <typename T>
    void onDataSent(T *objPtr, void (T::*memberPtr)(unsigned count))
    {
        dataSentCallChain.add(objPtr, memberPtr);
    }

    /**
     * Access the callchain of data sent event handlers.
     *
     * @return A reference to the DATA_SENT event callback chain.
     */
    DataSentCallbackChain_t &onDataSent()
    {
        return dataSentCallChain;
    }

    /**
     * Set an event handler that is called after
     * a connected peer has written an attribute.
     *
     * @param[in] callback The event handler being registered.
     *
     * @attention It is possible to set multiple event handlers. Registered
     * handlers may be removed with onDataWritten().detach(callback).
     */
    void onDataWritten(const DataWrittenCallback_t &callback)
    {
        dataWrittenCallChain.add(callback);
    }

    /**
     * Set an event handler that is called after
     * a connected peer has written an attribute.
     *
     * @param[in] objPtr Pointer to the instance that is used to invoke the
     * event handler (@p memberPtr).
     * @param[in] memberPtr Event handler being registered. It is a member
     * function.
     */
    template <typename T>
    void onDataWritten(
        T *objPtr,
        void (T::*memberPtr)(const GattWriteCallbackParams *context)
    ) {
        dataWrittenCallChain.add(objPtr, memberPtr);
    }

    /**
     * Access the callchain of data written event handlers.
     *
     * @return A reference to the data written event callbacks chain.
     *
     * @note It is possible to register callbacks using
     * onDataWritten().add(callback).
     *
     * @note It is possible to unregister callbacks using
     * onDataWritten().detach(callback).
     */
    DataWrittenCallbackChain_t &onDataWritten()
    {
        return dataWrittenCallChain;
    }

    /**
     * Set an event handler that monitors attribute reads from connected clients.
     *
     * @param[in] callback Event handler being registered.
     *
     * @return BLE_ERROR_NOT_IMPLEMENTED if this functionality isn't available;
     * else BLE_ERROR_NONE.
     *
     * @note  This functionality may not be available on all underlying stacks.
     * Application code may work around that limitation by monitoring read
     * requests instead of read events.
     *
     * @see GattCharacteristic::setReadAuthorizationCallback()
     * @see isOnDataReadAvailable().
     *
     * @attention It is possible to set multiple event handlers. Registered
     * handlers may be removed with onDataRead().detach(callback).
     */
    ble_error_t onDataRead(const DataReadCallback_t &callback)
    {
        if (!isOnDataReadAvailable()) {
            return BLE_ERROR_NOT_IMPLEMENTED;
        }

        dataReadCallChain.add(callback);
        return BLE_ERROR_NONE;
    }

    /**
     * Set an event handler that monitors attribute reads from connected clients.
     *
     * @param[in] objPtr Pointer to the instance that is used to invoke the
     * event handler (@p memberPtr).
     * @param[in] memberPtr Event handler being registered. It is a member
     * function.
     */
    template <typename T>
    ble_error_t onDataRead(
        T *objPtr,
        void (T::*memberPtr)(const GattReadCallbackParams *context)
    ) {
        if (!isOnDataReadAvailable()) {
            return BLE_ERROR_NOT_IMPLEMENTED;
        }

        dataReadCallChain.add(objPtr, memberPtr);
        return BLE_ERROR_NONE;
    }

    /**
     * Access the callchain of data read event handlers.
     *
     * @return A reference to the data read event callbacks chain.
     *
     * @note It is possible to register callbacks using
     * onDataRead().add(callback).
     *
     * @note It is possible to unregister callbacks using
     * onDataRead().detach(callback).
     */
    DataReadCallbackChain_t &onDataRead()
    {
        return dataReadCallChain;
    }

    /**
     * Set an event handler that monitors shutdown or reset of the GattServer.
     *
     * The event handler is invoked when the GattServer instance is about
     * to be shut down. This can result in a call to reset() or BLE::reset().
     *
     * @param[in] callback Event handler being registered.
     *
     * @note It is possible to set up multiple shutdown event handlers.
     *
     * @note It is possible to unregister a callback using
     * onShutdown().detach(callback)
     */
    void onShutdown(const GattServerShutdownCallback_t &callback)
    {
        shutdownCallChain.add(callback);
    }

    /**
     * Set an event handler that monitors shutdown or reset of the GattServer.
     *
     * The event handler is invoked when the GattServer instance is about
     * to be shut down. This can result of a call to reset() or BLE::reset().
     *
     * @param[in] objPtr Pointer to the instance that is used to invoke the
     * event handler (@p memberPtr).
     * @param[in] memberPtr Event handler being registered. It is a member
     * function.
     */
    template <typename T>
    void onShutdown(T *objPtr, void (T::*memberPtr)(const GattServer *))
    {
        shutdownCallChain.add(objPtr, memberPtr);
    }

    /**
     * Access the callchain of shutdown event handlers.
     *
     * @return A reference to the shutdown event callbacks chain.
     *
     * @note It is possible to register callbacks using
     * onShutdown().add(callback).
     *
     * @note It is possible to unregister callbacks using
     * onShutdown().detach(callback).
     */
    GattServerShutdownCallbackChain_t& onShutdown()
    {
        return shutdownCallChain;
    }

    /**
     * Set up an event handler that monitors subscription to characteristic
     * updates.
     *
     * @param[in] callback Event handler being registered.
     */
    void onUpdatesEnabled(EventCallback_t callback)
    {
        updatesEnabledCallback = callback;
    }

    /**
     * Set up an event handler that monitors unsubscription from characteristic
     * updates.
     *
     * @param[in] callback Event handler being registered.
     */
    void onUpdatesDisabled(EventCallback_t callback)
    {
        updatesDisabledCallback = callback;
    }

    /**
     * Set up an event handler that monitors notification acknowledgment.
     *
     * The event handler is called when a client sends a confirmation that it has
     * correctly received a notification from the server.
     *
     * @param[in] callback Event handler being registered.
     */
    void onConfirmationReceived(EventCallback_t callback)
    {
        confirmationReceivedCallback = callback;
    }

    /* Entry points for the underlying stack to report events back to the user. */
protected:
    /**
     * Helper function that notifies all registered handlers of an occurrence
     * of a data written event.
     *
     * @attention Vendor implementation must invoke this function after one of
     * the GattServer attributes has been written.
     *
     * @param[in] params The data written parameters passed to the registered
     * handlers.
     */
    void handleDataWrittenEvent(const GattWriteCallbackParams *params)
    {
        dataWrittenCallChain.call(params);
    }

    /**
     * Helper function that notifies all registered handlers of an occurrence
     * of a data read event.
     *
     * @attention Vendor implementation must invoke this function after one of
     * the GattServer attributes has been read.
     *
     * @param[in] params The data read parameters passed to the registered
     * handlers.
     */
    void handleDataReadEvent(const GattReadCallbackParams *params)
    {
        dataReadCallChain.call(params);
    }

    /**
     * Helper function that notifies the registered handler of an occurrence
     * of updates enabled, updates disabled or confirmation received events.
     *
     * @attention Vendor implementation must invoke this function when a client
     * subscribes to characteristic updates, unsubscribes from characteristic
     * updates or a notification confirmation has been received.
     *
     * @param[in] type The type of event that occurred.
     * @param[in] attributeHandle The handle of the attribute concerned by the
     * event.
     */
    void handleEvent(
        GattServerEvents::gattEvent_e type,
        GattAttribute::Handle_t attributeHandle
    ) {
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
     * of a data sent event.
     *
     * @attention Vendor implementation must invoke this function after the
     * emission of a notification or an indication.
     *
     * @param[in] count Number of packets sent.
     */
    void handleDataSentEvent(unsigned count)
    {
        dataSentCallChain.call(count);
    }

public:
    /**
     * Shut down the GattServer instance.
     *
     * This function notifies all event handlers listening for shutdown events
     * that the GattServer is about to be shut down; then it clears all
     * GattServer state.
     *
     * @note This function is meant to be overridden in the platform-specific
     * subclass. Overides must call the parent function before any cleanup.
     *
     * @return BLE_ERROR_NONE on success.
     */
    virtual ble_error_t reset(void)
    {
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
    DataSentCallbackChain_t dataSentCallChain;

    /**
     * Callchain containing all registered callback handlers for data written
     * events.
     */
    DataWrittenCallbackChain_t dataWrittenCallChain;

    /**
     * Callchain containing all registered callback handlers for data read
     * events.
     */
    DataReadCallbackChain_t dataReadCallChain;

    /**
     * Callchain containing all registered callback handlers for shutdown
     * events.
     */
    GattServerShutdownCallbackChain_t shutdownCallChain;

    /**
     * The registered callback handler for updates enabled events.
     */
    EventCallback_t updatesEnabledCallback;

    /**
     * The registered callback handler for updates disabled events.
     */
    EventCallback_t updatesDisabledCallback;

    /**
     * The registered callback handler for confirmation received events.
     */
    EventCallback_t confirmationReceivedCallback;

private:
    /* Disallow copy and assignment. */
    GattServer(const GattServer &);
    GattServer& operator=(const GattServer &);
};

/**
 * @}
 * @}
 * @}
 */

#endif /* ifndef MBED_GATT_SERVER_H__ */
