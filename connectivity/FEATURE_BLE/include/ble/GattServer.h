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


#ifndef MBED_GATT_SERVER_H__
#define MBED_GATT_SERVER_H__

#include "platform/mbed_toolchain.h"

#include "ble/common/CallChainOfFunctionPointersWithContext.h"
#include "ble/common/blecommon.h"

#include "ble/gatt/GattService.h"
#include "ble/gatt/GattAttribute.h"
#include "ble/gatt/GattCallbackParamTypes.h"

namespace ble {

#if !defined(DOXYGEN_ONLY)
namespace impl {
class GattServer;
}
#endif // !defined(DOXYGEN_ONLY)


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
     * Definition of the general handler of GattServer related events.
     */
    struct EventHandler {
        /**
         * Function invoked when the connections changes the ATT_MTU which controls
         * the maximum size of an attribute that can be read in a single L2CAP packet
         * which might be fragmented across multiple packets.
         *
         * @param connectionHandle The handle of the connection that changed the size.
         * @param attMtuSize
         *
         * @see negotiateAttMtu()
         */
        virtual void onAttMtuChange(
            ble::connection_handle_t connectionHandle,
            uint16_t attMtuSize
        ) {
            (void)connectionHandle;
            (void)attMtuSize;
        }

        /**
         * Function invoked when the server has sent data to a client. For
         * notifications this is triggered when data is sent, for indications
         * it's only triggered when the confirmation has been received.
         *
         * @note params has a temporary scope and should be copied by the
         * application if needed later
         */
        virtual void onDataSent(const GattDataSentCallbackParams &params) {
            (void)params;
        }

        /**
         * Function invoked when a client writes an attribute
         *
         * @note params has a temporary scope and should be copied by the
         * application if needed later
         */
        virtual void onDataWritten(const GattWriteCallbackParams &params) {
            (void)params;
        }

        /**
         * Function invoked when a client reads an attribute
         *
         * @note  This functionality may not be available on all underlying stacks.
         * Application code may work around that limitation by monitoring read
         * requests instead of read events.
         *
         * @note params has a temporary scope and should be copied by the
         * application if needed later
         *
         * @see GattCharacteristic::setReadAuthorizationCallback()
         * @see isOnDataReadAvailable().
         */
        virtual void onDataRead(const GattReadCallbackParams &params) {
            (void)params;
        }

        /**
         * Function invoked when the GattServer instance is about
         * to be shut down. This can be the result of a call to reset() or BLE::reset().
         */
        virtual void onShutdown(const GattServer &server) {
            (void)server;
        }

        /**
         * Function invoked when the client has subscribed to characteristic updates.
         *
         * @note params has a temporary scope and should be copied by the
         * application if needed later
         */
        virtual void onUpdatesEnabled(const GattUpdatesEnabledCallbackParams &params) {
            (void)params;
        }

        /**
         * Function invoked when the client has unsubscribed from characteristic updates.
         *
         * @note params has a temporary scope and should be copied by the
         * application if needed later
         */
        virtual void onUpdatesDisabled(const GattUpdatesDisabledCallbackParams &params) {
            (void)params;
        }

        /**
         * Event not used.
         *
         * @note params has a temporary scope and should be copied by the
         * application if needed later
         */
        MBED_DEPRECATED_SINCE("mbed-os-6.11.0", "This event is never triggered. Indication triggers onDataSent"
                                                "when confirmation is received.")
        virtual void onConfirmationReceived(const GattConfirmationReceivedCallbackParams &params) {
            (void)params;
        }

    protected:
        /**
         * Prevent polymorphic deletion and avoid unnecessary virtual destructor
         * as the GattServer class will never delete the instance it contains.
         */
        ~EventHandler() = default;
    };

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
     * @see onUpdatesEnabled() onUpdateDisabled()
     */
    typedef FunctionPointerWithContext<GattAttribute::Handle_t> EventCallback_t;

public:
    /**
     * Assign the event handler implementation that will be used by the
     * module to signal events back to the application.
     *
     * @param handler Application implementation of an EventHandler.
     *
     * @note Multiple discrete EventHandler instances may be used by adding them
     * to a ChainableGattServerEventHandler and then setting the chain as the primary
     * GattServer EventHandler using this function.
     *
     * @see ChainableGattServerEventHandler
     */
    void setEventHandler(EventHandler *handler);

    /**
     * Shut down the GattServer instance.
     *
     * This function notifies all event handlers listening for shutdown events
     * that the GattServer is about to be shut down; then it clears all
     * GattServer state.
     *
     * @note This function is meant to be overridden in the platform-specific
     * subclass. Overrides must call the parent function before any cleanup.
     *
     * @return BLE_ERROR_NONE on success.
     *
     * @see EventHandler::onShutdown()
     */
    ble_error_t reset();

    /**
     * Add a service declaration to the local attribute server table.
     *
     * This functions inserts a service declaration in the attribute table
     * followed by the characteristic declarations (including characteristic
     * descriptors) present in @p service.
     *
     * The process assigns a unique attribute handle to all the elements added
     * into the attribute table. This handle is an ID that must be used for
     * subsequent interactions with the elements.
     *
     * @note There is no mirror function that removes a single service.
     * Application code can remove all the registered services by calling
     * reset().
     *
     * @attention GattServer allocates its own memory for all the attributes.
     * The GattServer will set the handles on the service passed in and the
     * characteristics it contains. You may record the handles you want to
     * interact with in the future. After that the service and characteristics
     * you passed in as the parameter may be freed. To write to the GattServer
     * instances of the characteristics you have to use the saved handles.
     *
     * @param[in] service The service to be added; attribute handle of services,
     * characteristic and characteristic descriptors are updated by the
     * process.
     *
     * @return BLE_ERROR_NONE if the service was successfully added.
     */
    ble_error_t addService(GattService &service);

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
     * @attention read(ble::connection_handle_t, GattAttribute::Handle_t, uint8_t *, uint16_t *)
     * must be used to read Client Characteristic Configuration Descriptor (CCCD)
     * because the value of this type of attribute depends on the connection.
     */
    ble_error_t read(
        GattAttribute::Handle_t attributeHandle,
        uint8_t buffer[],
        uint16_t *lengthP
    );

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
    ble_error_t read(
        ble::connection_handle_t connectionHandle,
        GattAttribute::Handle_t attributeHandle,
        uint8_t *buffer,
        uint16_t *lengthP
    );

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
     *
     * @see EventHandler::onDataSent(), this will only be triggered if there are
     * client subscribed and the localOnly parameter is set to false.
     */
    ble_error_t write(
        GattAttribute::Handle_t attributeHandle,
        const uint8_t *value,
        uint16_t size,
        bool localOnly = false
    );

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
     *
     * @see EventHandler::onDataSent(), this will only be triggered if there are
     * client subscribed and the localOnly parameter is set to false.
     */
    ble_error_t write(
        ble::connection_handle_t connectionHandle,
        GattAttribute::Handle_t attributeHandle,
        const uint8_t *value,
        uint16_t size,
        bool localOnly = false
    );

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
     *
     * @see EventHandler::onDataSent()
     */
    ble_error_t areUpdatesEnabled(
        const GattCharacteristic &characteristic,
        bool *enabledP
    );

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
     *
     * @see EventHandler::onDataSent()
     */
    ble_error_t areUpdatesEnabled(
        ble::connection_handle_t connectionHandle,
        const GattCharacteristic &characteristic,
        bool *enabledP
    );

    /**
     * Indicate if the underlying stack emit events when an attribute is read by
     * a client.
     *
     * @attention This function should be overridden to return true if
     * applicable.
     *
     * @return true if onDataRead is supported; false otherwise.
     */
    bool isOnDataReadAvailable() const;

    /**
     * Add an event handler that monitors emission of characteristic value
     * updates.
     *
     * @param[in] callback Event handler being registered.
     *
     * @note It is possible to chain together multiple onDataSent callbacks
     * (potentially from different modules of an application).
     */
    MBED_DEPRECATED_SINCE("mbed-os-6.3.0", "Individual callback-registering functions have"
                          "been replaced by GattServer::setEventHandler. Use that function instead.")
    void onDataSent(const DataSentCallback_t &callback);

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
    MBED_DEPRECATED_SINCE("mbed-os-6.3.0", "Individual callback-registering functions have"
                          "been replaced by GattServer::setEventHandler. Use that function instead.")
    void onDataSent(T *objPtr, void (T::*memberPtr)(unsigned count))
    {
        onDataSent({objPtr, memberPtr});
    }

    /**
     * Access the callchain of data sent event handlers.
     *
     * @return A reference to the DATA_SENT event callback chain.
     */
    MBED_DEPRECATED_SINCE("mbed-os-6.3.0", "Individual callback-registering functions have"
                          "been replaced by GattServer::setEventHandler. Use that function instead.")
    DataSentCallbackChain_t &onDataSent();

    /**
     * Set an event handler that is called after
     * a connected peer has written an attribute.
     *
     * @param[in] callback The event handler being registered.
     *
     * @attention It is possible to set multiple event handlers. Registered
     * handlers may be removed with onDataWritten().detach(callback).
     */
    MBED_DEPRECATED_SINCE("mbed-os-6.3.0", "Individual callback-registering functions have"
                          "been replaced by GattServer::setEventHandler. Use that function instead.")
    void onDataWritten(const DataWrittenCallback_t &callback);

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
    MBED_DEPRECATED_SINCE("mbed-os-6.3.0", "Individual callback-registering functions have"
                          "been replaced by GattServer::setEventHandler. Use that function instead.")
    void onDataWritten(
        T *objPtr,
        void (T::*memberPtr)(const GattWriteCallbackParams *context)
    )
    {
        onDataWritten({objPtr, memberPtr});
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
    MBED_DEPRECATED_SINCE("mbed-os-6.3.0", "Individual callback-registering functions have"
                          "been replaced by GattServer::setEventHandler. Use that function instead.")
    DataWrittenCallbackChain_t &onDataWritten();

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
    MBED_DEPRECATED_SINCE("mbed-os-6.3.0", "Individual callback-registering functions have"
                          "been replaced by GattServer::setEventHandler. Use that function instead.")
    ble_error_t onDataRead(const DataReadCallback_t &callback);

    /**
     * Set an event handler that monitors attribute reads from connected clients.
     *
     * @param[in] objPtr Pointer to the instance that is used to invoke the
     * event handler (@p memberPtr).
     * @param[in] memberPtr Event handler being registered. It is a member
     * function.
     */
    template <typename T>
    MBED_DEPRECATED_SINCE("mbed-os-6.3.0", "Individual callback-registering functions have"
                          "been replaced by GattServer::setEventHandler. Use that function instead.")
    ble_error_t onDataRead(
        T *objPtr,
        void (T::*memberPtr)(const GattReadCallbackParams *context)
    )
    {
        return onDataRead({objPtr, memberPtr});
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
    MBED_DEPRECATED_SINCE("mbed-os-6.3.0", "Individual callback-registering functions have"
                          "been replaced by GattServer::setEventHandler. Use that function instead.")
    DataReadCallbackChain_t &onDataRead();

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
    MBED_DEPRECATED_SINCE("mbed-os-6.3.0", "Individual callback-registering functions have"
                          "been replaced by GattServer::setEventHandler. Use that function instead.")
    void onShutdown(const GattServerShutdownCallback_t &callback);

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
    MBED_DEPRECATED_SINCE("mbed-os-6.3.0", "Individual callback-registering functions have"
                          "been replaced by GattServer::setEventHandler. Use that function instead.")
    void onShutdown(T *objPtr, void (T::*memberPtr)(const GattServer *))
    {
        onShutdown({objPtr, memberPtr});
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
    MBED_DEPRECATED_SINCE("mbed-os-6.3.0", "Individual callback-registering functions have"
                          "been replaced by GattServer::setEventHandler. Use that function instead.")
    GattServerShutdownCallbackChain_t& onShutdown();

    /**
     * Set up an event handler that monitors subscription to characteristic
     * updates.
     *
     * @param[in] callback Event handler being registered.
     */
    MBED_DEPRECATED_SINCE("mbed-os-6.3.0", "Individual callback-registering functions have"
                          "been replaced by GattServer::setEventHandler. Use that function instead.")
    void onUpdatesEnabled(EventCallback_t callback);

    /**
     * Set up an event handler that monitors unsubscription from characteristic
     * updates.
     *
     * @param[in] callback Event handler being registered.
     */
    MBED_DEPRECATED_SINCE("mbed-os-6.3.0", "Individual callback-registering functions have"
                          "been replaced by GattServer::setEventHandler. Use that function instead.")
    void onUpdatesDisabled(EventCallback_t callback);

    /**
     * Set up an event handler that monitors notification acknowledgment.
     *
     * The event handler is called when a client sends a confirmation that it has
     * correctly received a notification from the server.
     *
     * @param[in] callback Event handler being registered.
     */
    MBED_DEPRECATED_SINCE("mbed-os-6.3.0", "Individual callback-registering functions have"
                          "been replaced by an event handler. Indication confirmation triggers"
                          "GattServer::onDataSent event instead.")
    void onConfirmationReceived(EventCallback_t callback);

#if !defined(DOXYGEN_ONLY)
    GattServer(impl::GattServer* impl) : impl(impl) {}
    GattServer(const GattServer&) = delete;
    GattServer& operator=(const GattServer&) = delete;
#endif // !defined(DOXYGEN_ONLY)

private:
    impl::GattServer *impl;
};

/**
 * @}
 * @}
 * @}
 */

} // ble

/** @deprecated Use the namespaced ble::GattServer instead of the global GattServer. */
using ble::GattServer;

#endif /* ifndef MBED_GATT_SERVER_H__ */
