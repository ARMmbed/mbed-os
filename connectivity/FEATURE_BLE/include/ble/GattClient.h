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


#ifndef MBED_GATT_CLIENT_H__

#define MBED_GATT_CLIENT_H__

#include "ble/common/CallChainOfFunctionPointersWithContext.h"
#include "ble/common/blecommon.h"

#include "ble/gatt/GattAttribute.h"
#include "ble/gatt/ServiceDiscovery.h"
#include "ble/gatt/CharacteristicDescriptorDiscovery.h"
#include "ble/gatt/GattCallbackParamTypes.h"
#include "ble/gatt/DiscoveredService.h"
#include "ble/gatt/DiscoveredCharacteristic.h"

namespace ble {

#if !defined(DOXYGEN_ONLY)
namespace impl {
class GattClient;
}
#endif // !defined(DOXYGEN_ONLY)

/**
 * @addtogroup ble
 * @{
 * @addtogroup gatt
 * @{
 * @addtogroup client
 * @{
 */

/**
 * Define procedures required for interacting with a distant GATT server.
 *
 * @par Discovery procedures
 *
 * A GATT server hosts a fixed set of services. These services are a logical
 * composition of characteristics that may be discovered, read, written or also
 * broadcast their state to a connected client. These characteristics may also
 * contain metainformation named characteristic descriptors. A characteristic
 * descriptor may be used to indicate the unit used for a characteristic value,
 * describe in a textual form the characterisic purpose or allow a client to
 * register for notification of updates of the characteristic value.
 *
 * Prior to any interaction with server characteristic, a GATT client
 * discovers the layout of the services and characteristics present on the
 * server.
 *
 * The layout of the descriptors of a characteristic may also be issued to
 * as an extra discovery step.
 *
 * @par Attribute manipulation
 *
 * As a result of the discovery process, the client can start interacting with
 * the characteristic discovered. Depending on the characteristic properties
 * (acquired during discovery), a client can read or write the value of a given
 * characteristic.
 *
 * Mbed BLE abstracts most read and write operations to offer a single API that
 * can be used to read or write characteristics values. Application code does not
 * have to handle the fragmentation/reassembly process necessary if the attribute
 * value to transported cannot fit in a single data packet.
 *
 * @par Server Initiated events
 *
 * If a characteristic has to notify or indicate a property set; then, a client may
 * register to a notification or indication from the characteristic. When the
 * server updates the characteristic value, the server can forward the
 * new value to the registered clients. The notification/indication mechanism
 * prevents polling from the client and therefore minimize the transactions
 * involved between a client and a server.
 *
 * Registration is made by writing the Client Characteristic Configuration
 * Descriptor, which is present in the characteristic if the notify or
 * indicate properties are set. The client discovers that descriptor
 * if it intends to register to server initiated events.
 */
class GattClient {
public:
    /**
     * Definition of the general handler of GattClient related events.
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
    protected:
        /**
         * Prevent polymorphic deletion and avoid unnecessary virtual destructor
         * as the GattClient class will never delete the instance it contains.
         */
        ~EventHandler() = default;
    };

    /**
     * Assign the event handler implementation that will be used by the
     * module to signal events back to the application.
     *
     * @param handler Application implementation of an EventHandler.
     */
    void setEventHandler(EventHandler *handler);

    /**
     * Attribute read event handler.
     *
     * @see GattClient::onDataRead().
     * @deprecated Use the version in global ble namespace.
     */
    typedef FunctionPointerWithContext<const GattReadCallbackParams*>
        ReadCallback_t;

    /**
     * Callchain of attribute read event handlers.
     * @deprecated Use the version in global ble namespace.
     */
    typedef CallChainOfFunctionPointersWithContext<const GattReadCallbackParams*>
        ReadCallbackChain_t;

    /**
     * GATT write operations.
     */
    enum WriteOp_t {
        /**
         * Write request.
         *
         * It is used to request the server to write the value of an attribute
         * and acknowledge that this has been achieved in a Write Response.
         */
        GATT_OP_WRITE_REQ = 0x01,

        /**
         * Write command.
         *
         * It is used to request the server to write the value of an attribute.
         * The server does not acknowledge the status of the operation.
         */
        GATT_OP_WRITE_CMD = 0x02,

        /**
         * Signed Write command.
         *
         * It is used to request the server to write the value of an attribute
         * using a signed packet. The server does not acknowledge the status
         * of the operation.
         */
        GATT_OP_SIGNED_WRITE_CMD = 0x03
    };

    /**
     * Attribute write event handler.ble::WriteCallback_t
     *
     * @see GattClient::onDataWrite().
     * @deprecated Use the version in global ble namespace.
     */
    typedef FunctionPointerWithContext<const GattWriteCallbackParams*>
        WriteCallback_t;

    /**
     * Callchain of attribute write event handlers.
     *
     * @see GattClient::onDataWrite().
     * @deprecated Use the version in global ble namespace.
     */
    typedef CallChainOfFunctionPointersWithContext<const GattWriteCallbackParams*>
        WriteCallbackChain_t;

    /**
     * Handle value notification/indication event handler.
     *
     * @see to GattClient::onHVX().
     */
    typedef FunctionPointerWithContext<const GattHVXCallbackParams*>
        HVXCallback_t;

    /**
     * Callchain of handle value notification/indication event handlers.
     *
     * @see GattClient::onHVX().
     */
    typedef CallChainOfFunctionPointersWithContext<const GattHVXCallbackParams*>
        HVXCallbackChain_t;

    /**
     * Shutdown event handler.
     *
     * @see GattClient::onShutdown().
     */
    typedef FunctionPointerWithContext<const GattClient *>
        GattClientShutdownCallback_t;


    /**
     * Callchain of shutdown event handlers.
     *
     * @see to GattClient::onShutown().
     */
    typedef CallChainOfFunctionPointersWithContext<const GattClient *>
        GattClientShutdownCallbackChain_t;

    /*
     * The following functions are meant to be overridden in the platform
     * specific subclass.
     */

    ~GattClient() = default;

    /**
     * Launch the service and characteristic discovery procedure of a GATT server
     * peer.
     *
     * The procedure invokes application callbacks for matching services or
     * characteristics. The process ends after all the services and
     * characteristics present on the distant GATT server have been discovered.
     * Termination callbacks registered with onServiceDiscoveryTermination() are
     * invoked to notify the application of the termination of the procedure.
     *
     * Application code can track the status of the procedure by invoking the
     * function isServiceDiscoveryActive(), which returns true if the
     * procedure is ongoing.
     *
     * At any point, application code can prematurely terminate the discovery
     * procedure by calling terminateServiceDiscovery().
     *
     * @param[in] connectionHandle Handle of the connection with the peer GATT
     * server.
     * @param[in] sc Service discovered event handler invoked when a matching
     * service has been discovered. This parameter may be NULL.
     * @param[in] cc Characteristic discovered event handler invoked when a
     * matching characteristic has been found. This parameter may be NULL.
     * @param[in] matchingServiceUUID UUID of the service the caller is
     * interested in. If a service discovered matches this filter, then @p sc is
     * invoked with it. The special value BLE_UUID_UNKNOWN acts as a wildcard,
     * which can be used to discover all services present on the peer GATT
     * server.
     * @param[in] matchingCharacteristicUUIDIn UUID of the characteristic the
     * caller is interested in. If a characteristic discovered matches this
     * filter, then @p cc is  invoked with it. The special value BLE_UUID_UNKNOWN
     * acts as a wildcard, which can be used to discover all services present on
     * the peer GATT server.
     *
     * @par Discovery procedure implementation detail
     *
     * It is recommended to implement several strategies based on the
     * combination of callbacks and filters passed in input to efficiently
     * realize the discovery procedure:
     * - If @p sc and @p cc are NULL, then it is not necessay to initiate any
     * discovery, and the termination handlers can be invoked immediately.
     * - If @p matchingServiceUUID is set, then the GATT discover services by
     * service UUID procedure should be used; otherwise, the GATT discover primary
     * services procedure should be used.
     * - If @p cc is NULL, then the discovery process should end after the discovery
     * of the services.
     *
     * @return BLE_ERROR_NONE if the discovery procedure has been successfully
     * started and an appropriate error otherwise.
     */
    ble_error_t launchServiceDiscovery(
        ble::connection_handle_t connectionHandle,
        ServiceDiscovery::ServiceCallback_t sc = nullptr,
        ServiceDiscovery::CharacteristicCallback_t  cc = nullptr,
        const UUID &matchingServiceUUID = UUID::ShortUUIDBytes_t(BLE_UUID_UNKNOWN),
        const UUID &matchingCharacteristicUUIDIn = UUID::ShortUUIDBytes_t(BLE_UUID_UNKNOWN)
    );

    /**
     * Launch the service discovery procedure of a GATT server peer.
     *
     * The procedure invokes the application callback for matching services.
     * The process ends after all the services present on the distant GATT
     * server have been discovered.
     * Termination callbacks registered with onServiceDiscoveryTermination() are
     * invoked to notify the application of the termination of the procedure.
     *
     * Application code can track the status of the procedure by invoking the
     * function isServiceDiscoveryActive(), which returns true if the
     * procedure is ongoing.
     *
     * At any point, application code can prematurely terminate the discovery
     * procedure by calling terminateServiceDiscovery().
     *
     * @param[in] connectionHandle Handle of the connection with the peer GATT
     * server.
     * @param[in] callback Service discovered event handler invoked when a
     * matching service has been discovered. This parameter may be NULL.
     * @param[in] matchingServiceUUID UUID of the service the caller is
     * interested in. If a service discovered matches this filter, then @p sc is
     * invoked with it. The special value BLE_UUID_UNKNOWN act is a wildcard,
     * which can be used to discover all services present on the peer GATT
     * server.
     *
     * @return BLE_ERROR_NONE if the discovery procedure has been successfully
     * started and an appropriate error otherwise.
     */
    ble_error_t discoverServices(
        ble::connection_handle_t connectionHandle,
        ServiceDiscovery::ServiceCallback_t callback,
        const UUID &matchingServiceUUID = UUID::ShortUUIDBytes_t(BLE_UUID_UNKNOWN)
    );

    /**
     * Launch the service discovery procedure of a GATT server peer.
     *
     * The process ends after all the services present in the attribute range @p
     * startHandle to @p endHandle have been discovered.
     *
     * Termination callbacks registered with onServiceDiscoveryTermination() are
     * invoked to notify the application of the termination of the procedure.
     *
     * Application code can track the status of the procedure by invoking the
     * function isServiceDiscoveryActive(), which returns true if the
     * procedure is ongoing.
     *
     * At any point, application code can prematurely terminate the discovery
     * procedure by calling terminateServiceDiscovery().
     *
     * @param[in] connectionHandle Handle of the connection with the peer GATT
     * server.
     * @param[in] callback Service discovered event handler invoked when a
     * matching service has been discovered. This parameter may be NULL.
     * @param[in] startHandle First attribute handle of the discovery range.
     * @param[in] endHandle end Lasr attribute handle of the discovery range.
     *
     * @return BLE_ERROR_NONE if the discovery procedure has been successfully
     * started and an appropriate error otherwise.
     */
    ble_error_t discoverServices(
        ble::connection_handle_t connectionHandle,
        ServiceDiscovery::ServiceCallback_t callback,
        GattAttribute::Handle_t startHandle,
        GattAttribute::Handle_t endHandle
    );

    /**
     * Check if the service discovery procedure is currently active.
     *
     * @return true if service discovery procedure is active and false otherwise.
     */
    bool isServiceDiscoveryActive() const;

    /**
     * Terminate all ongoing service discovery procedures.
     *
     * It results in an invocation of the service discovery termination handler
     * registered with onServiceDiscoveryTermination().
     */
    void terminateServiceDiscovery();

    /**
     * Initiate the read procedure of an attribute handle.
     *
     * Once the attribute value has been read in its entirety, the process issues
     * an attribute read event and passes it to all events handlers registered
     * by onDataRead.
     *
     * @param[in] connHandle Handle of the connection used to send the read
     * request.
     * @param[in] attributeHandle Handle of the attribute to read data from.
     * @param[in] offset The offset from the start of the attribute value to be
     * read.
     *
     * @return BLE_ERROR_NONE if read procedure successfully started.
     *
     * @par Implementation notes:
     *
     * Reading the attribute value in its entirety may involve sending several
     * GATT requests to the peer. The following algorithm may be used to
     * implement the process:
     *
     * If the offset is equal to 0, then send a read request; otherwise, send a
     * read blob request at the specified offset.
     *
     * While the attribute data in the response are MTU - 1 long:
     *   - Concat the response to the value containing the previous responses.
     *   - Increment the value of the offset by MTU - 1.
     *   - Send a read blob request with the updated offset.
     *
     * Finally, concat the last response with the value containing all the
     * previous responses and forward that value to the event handlers.
     */
    ble_error_t read(
        ble::connection_handle_t connHandle,
        GattAttribute::Handle_t attributeHandle,
        uint16_t offset
    ) const;

    /**
     * Initiate a write procedure on an attribute value.
     *
     * If @p cmd is equal to GATT_OP_WRITE_REQ, then the status of the operation
     * is reported to the event handlers registered through onDataWritten().
     *
     * @param[in] cmd Type of the write procedure used. If GATT_OP_WRITE_CMD
     * is set, then value length is not greater than the size of the mtu
     * of connHandle minus three.
     * @param[in] connHandle Handle of the connection used to send the write
     * request or command.
     * @param[in] attributeHandle Handle of the attribute value to write.
     * @param[in] length Number of bytes present in @p value.
     * @param[in] value Data buffer to write to attributeHandle.
     *
     * @return BLE_ERROR_NONE if the write procedure successfully started.
     *
     * @par Implementation notes:
     *
     * If the operation is a write command, then an implementation uses the
     * GATT write without response procedure and an error is returned if
     * the data buffer to write is larger than the size of the MTU - 3.
     *
     * If the operation is a write command and the size of the data buffer to
     * write is less than than the size of the MTU - 3, then the ATT write request
     * procedure is used, and the response is reported to the handlers
     * listening for write response.
     *
     * Otherwise, the data buffer to write is divided in chunks with a
     * maximum size of MTU - 5. Those chunks are sent sequentially to the
     * peer in ATT prepare write requests. If an error response is received
     * during the process, the procedure ends immediately, the prepared
     * write is discarded and an error is reported to the application handlers.
     * Once all the chunks have been sent, the transaction is completed
     * by sending an execute write request to the peer. The peer response is
     * forwarded to the application handlers.
     */
    ble_error_t write(
        GattClient::WriteOp_t cmd,
        ble::connection_handle_t connHandle,
        GattAttribute::Handle_t attributeHandle,
        size_t length,
        const uint8_t *value
    ) const;

    /* Event callback handlers. */

    /**
     * Register an attribute read event handler.
     *
     * @note It is possible to unregister a callback using
     * onDataRead().detach(callbackToRemove).
     *
     * @param[in] callback Event handler being registered.
     */
    void onDataRead(ble::ReadCallback_t callback);

    /**
     * Get the callchain of attribute read event handlers.
     *
     * @return A reference to the read event callback chain.
     *
     * @note It is possible to register new handlers using
     * onDataRead().add(callback).
     *
     * @note It is possible to unregister an handler by using
     * onDataRead().detach(callback).
     */
    ble::ReadCallbackChain_t& onDataRead();

    /**
     * Register an attribute write event handler.
     *
     * @param[in] callback Event handler being registered.
     *
     * @note It is possible to remove registered handlers using
     * onDataWritten().detach(callbackToRemove).
     *
     * @note Write commands (issued using writeWoResponse) don't generate a
     * response.
     */
    void onDataWritten(ble::WriteCallback_t callback);

    /**
     * Get the callchain of attribute write event handlers.
     *
     * @return A reference to the data written callbacks chain.
     *
     * @note It is possible to register new handlers by using
     * onDataWritten().add(callback).
     *
     * @note It is possible to unregister an handler by using
     * onDataWritten().detach(callback).
     */
    ble::WriteCallbackChain_t& onDataWritten();

    /**
     * Register a service discovery termination event handler.
     *
     * @param[in] callback Event handler being registered.
     */
    void onServiceDiscoveryTermination(
        ServiceDiscovery::TerminationCallback_t callback
    );

    /**
     * Initiate the descriptor discovery procedure for a given characteristic.
     *
     * When a descriptor is discovered the discovered descriptor is forwarded
     * to @p discoveryCallback. After the discovery of all the descriptors, the
     * procedure ends and send a descriptor discovery termination event to @p
     * termination callback.
     *
     * Application code may monitor the discovery process by querying its status
     * with isCharacteristicDescriptorDiscoveryActive(). It can also end the
     * discovery process by calling terminateCharacteristicDescriptorDiscovery().
     *
     * @param[in] characteristic The characteristic owning the descriptors to
     * discover.
     * @param[in] discoveryCallback Handle descriptor discovered events for the
     * duration of the procedure.
     * @param[in] terminationCallback Handle descriptor discovery termination
     * event of the procedure.
     *
     * @return BLE_ERROR_NONE if the characteristic descriptor discovery
     * procedure has been launched successfully otherwise an appropriate error.
     */
    ble_error_t discoverCharacteristicDescriptors(
        const DiscoveredCharacteristic& characteristic,
        const CharacteristicDescriptorDiscovery::DiscoveryCallback_t& discoveryCallback,
        const CharacteristicDescriptorDiscovery::TerminationCallback_t& terminationCallback
    );

    /**
     * Query status of the descriptor discovery procedure for a given
     * characteristic.
     *
     * @param[in] characteristic The characteristic concerned by the descriptors
     * discovery.
     *
     * @return true if a descriptors discovery is active for the characteristic
     * in input otherwise false.
     */
    bool isCharacteristicDescriptorDiscoveryActive(
        const DiscoveredCharacteristic& characteristic
    ) const;

    /**
     * @brief Terminate an ongoing characteristic descriptor discovery procedure.
     *
     * If the procedure is active, then it ends, and the termination handler
     * associated with the procedure is called.
     *
     * @param[in] characteristic The characteristic containing the descriptors
     * being discovered.
     */
    void terminateCharacteristicDescriptorDiscovery(
        const DiscoveredCharacteristic& characteristic
    );

    /**
     * Trigger MTU negotiation. This might result in a Gap event onAttMtuChange
     * being called if MTU changes.
     *
     * @note This does not guarantee a change in MTU size. If size remains
     * unchanged no event will be generated.
     *
     * @param connection Connection on which the MTU is to be negotiated.
     *
     * @return BLE_ERROR_NONE if the procedure has been launched successfully
     * otherwise an appropriate error.
     */
    ble_error_t negotiateAttMtu(ble::connection_handle_t connection);

    /**
     * Register an handler for Handle Value Notification/Indication events.
     *
     * @param callback Event handler to register.
     *
     * @note It is possible to unregister a callback by using
     * onHVX().detach(callbackToRemove).
     */
    void onHVX(HVXCallback_t callback);

    /**
     * Register a shutdown event handler.
     *
     * The registered handler is invoked when the GattClient instance is
     * about to be shut down.
     *
     * @param[in] callback Event handler to invoke when a shutdown event is
     * available.
     *
     * @note onShutdown().detach(callback) may be used to unregister a given
     * callback.
     *
     * @see BLE::shutdown()
     */
    void onShutdown(const GattClientShutdownCallback_t& callback);

    /**
     * Register a shutdown event handler.
     *
     * The registered handler is invoked when the GattClient instance is
     * about to be shut down.
     *
     * @param[in] objPtr Instance that will be used to invoke @p memberPtr.
     * @param[in] memberPtr Event handler to invoke when a shutdown event is
     * available.
     */
    template <typename T>
    void onShutdown(T *objPtr, void (T::*memberPtr)(const GattClient *))
    {
        onShutdown({objPtr, memberPtr});
    }

    /**
     * Get the callchain of shutdown event handlers.
     *
     * @return A reference to the shutdown event callbacks chain.
     *
     * @note onShutdown().add(callback) may be used to register new handlers.
     *
     * @note onShutdown().detach(callback) may be used to unregister an handler.
     */
    GattClientShutdownCallbackChain_t& onShutdown();

    /**
     * @brief provide access to the callchain of HVX callbacks.
     *
     * @return A reference to the HVX callbacks chain.
     *
     * @note It is possible to register callbacks using onHVX().add(callback).
     *
     * @note It is possible to unregister callbacks using onHVX().detach(callback).
     */
    HVXCallbackChain_t& onHVX();

    /**
     * Reset the state of the GattClient instance.
     *
     * Prior to any state modification, shutdown event handlers are notified
     * that the GattClient instance is about to be shut down. Then, running
     * procedures end. Finally, the state of the instance is reset.
     *
     * @par implementation note
     *
     * This function is meant to be overridden in the platform-specific
     * subclass. Nevertheless, the subclass only resets its
     * state and not the data held in GattClient members. This is achieved
     * by a call to GattClient::reset() from the subclass' reset()
     * implementation.
     *
     * @return BLE_ERROR_NONE on success.
     */
    ble_error_t reset();

    /* Entry points for the underlying stack to report events back to the user. */

    /**
     * Forward an attribute read event to all registered handlers.
     *
     * @attention This function is meant to be called from the vendor
     * implementation when an attribute read event occurs.
     *
     * @param[in] params Attribute read event to pass to the registered handlers.
     */
    void processReadResponse(const GattReadCallbackParams *params);

    /**
     * Forward an attribute written event to all registered handlers.
     *
     * @attention This function is meant to be called from the vendor
     * implementation when an attribute written event occurs.
     *
     * @param[in] params Attribute written event to pass to the registered
     * handlers.
     */
    void processWriteResponse(const GattWriteCallbackParams *params);

    /**
     * Forward a handle value notification or indication event to all registered
     * handlers.
     *
     * @attention This function is meant to be called from the vendor
     * implementation when a notification or indication event is available.
     *
     * @param[in] params Notification or Indication event to pass to the
     * registered handlers.
     */
    void processHVXEvent(const GattHVXCallbackParams *params);

#if !defined(DOXYGEN_ONLY)
    GattClient(impl::GattClient* impl) : impl(impl) { }
    GattClient(const GattClient&) = delete;
    GattClient& operator=(const GattClient&) = delete;
#endif // !defined(DOXYGEN_ONLY)

private:
    impl::GattClient *impl;
};

/**
 * @}
 * @}
 * @}
 */

} // namespace ble

/** @deprecated Use the namespaced ble::GattClient instead of the global GattClient. */
using ble::GattClient;

#endif /* ifndef MBED_GATT_CLIENT_H__ */
