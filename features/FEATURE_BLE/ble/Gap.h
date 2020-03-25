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

#ifndef MBED_BLE_GAP_H__
#define MBED_BLE_GAP_H__

#include "BLERoles.h"
#include "ble/common/StaticInterface.h"
#include "BLETypes.h"
#include "BLEProtocol.h"
#include "ble/GapAdvertisingData.h"
#include "ble/GapAdvertisingParams.h"
#include "ble/GapScanningParams.h"
#include "ble/GapEvents.h"

#include "CallChainOfFunctionPointersWithContext.h"
#include "FunctionPointerWithContext.h"
#include "platform/mbed_toolchain.h"

#include "ble/gap/Gap.h"
#include "BleImplementationForward.h"

/**
 * @addtogroup ble
 * @{
 * @addtogroup gap
 * @{
 */

#if !defined(DOXYGEN_ONLY)
namespace ble {
namespace interface {
#endif

/**
 * @copydoc ble::Gap
 */
#if defined(DOXYGEN_ONLY)
class Gap : public ble::Gap {
#else
template<class Impl>
class LegacyGap :
    public ble::StaticInterface<Impl, LegacyGap>,
    public ble::interface::Gap<Impl>
{
#endif
    using ble::StaticInterface<Impl, ::ble::interface::LegacyGap>::impl;

public:
#if BLE_ROLE_BROADCASTER
    using ble::interface::Gap<Impl>::setAdvertisingParameters;
    using ble::interface::Gap<Impl>::setAdvertisingPayload;
    using ble::interface::Gap<Impl>::setAdvertisingScanResponse;
    using ble::interface::Gap<Impl>::startAdvertising;
    using ble::interface::Gap<Impl>::stopAdvertising;
#endif // BLE_ROLE_BROADCASTER
#if BLE_ROLE_CENTRAL
    using ble::interface::Gap<Impl>::connect;
#endif
#if BLE_FEATURE_CONNECTABLE
    using ble::interface::Gap<Impl>::disconnect;
#endif
#if BLE_ROLE_OBSERVER
    using ble::interface::Gap<Impl>::startScan;
#endif

    /**
     * Length (in octets) of the BLE MAC address.
     */
    static const unsigned ADDR_LEN = BLEProtocol::ADDR_LEN;

public:
    /**
     * Enumeration of possible timeout sources.
     */
    enum TimeoutSource_t {
        /**
         * Advertising timeout.
         */
        TIMEOUT_SRC_ADVERTISING = 0x00,

        /**
         * Security request timeout.
         */
        TIMEOUT_SRC_SECURITY_REQUEST = 0x01,

        /**
         * Scanning timeout.
         */
        TIMEOUT_SRC_SCAN = 0x02,

        /**
         * Connection timeout.
         */
        TIMEOUT_SRC_CONN = 0x03,
    };

    /**
     * Enumeration of disconnection reasons.
     *
     * @attention There might be a mismatch between the disconnection reason
     * passed to disconnect() and the disconnection event generated locally
     * because the disconnection reason passed to disconnect() is the
     * disconnection reason to be transmitted to the peer.
     */
    enum DisconnectionReason_t {

        /**
         * GAP or GATT failed to authenticate the peer.
         */
        AUTHENTICATION_FAILURE = 0x05,

        /**
         * The connection timed out.
         *
         * @attention shall not be used as a reason in disconnect().
         */
        CONNECTION_TIMEOUT = 0x08,

        /**
         * Connection terminated by the user.
         */
        REMOTE_USER_TERMINATED_CONNECTION = 0x13,

        /**
         * Remote device terminated connection due to low resources.
         */
        REMOTE_DEV_TERMINATION_DUE_TO_LOW_RESOURCES = 0x14,

        /**
         * Remote device terminated connection due to power off.
         */
        REMOTE_DEV_TERMINATION_DUE_TO_POWER_OFF = 0x15,

        /**
         * Indicate that the local user or the internal
         * Bluetooth subsystem terminated the connection.
         *
         * @attention shall not be used as a reason in disconnect().
         */
        LOCAL_HOST_TERMINATED_CONNECTION = 0x16,

        /**
         * Connection parameters were unacceptable.
         */
        CONN_INTERVAL_UNACCEPTABLE = 0x3B,
    };

    /**
     * Advertising policy filter modes.
     *
     * @see Bluetooth Core Specification 4.2 (Vol. 6), Part B, Section 4.3.2.
     */
    enum AdvertisingPolicyMode_t {
        /**
         * The whitelist is not used to filter peer request during advertising.
         */
        ADV_POLICY_IGNORE_WHITELIST = 0,

        /**
         * The whitelist is used to filter peer scan requests.
         */
        ADV_POLICY_FILTER_SCAN_REQS = 1,

        /**
         * The whitelist is used to filter peer connection requests.
         */
        ADV_POLICY_FILTER_CONN_REQS = 2,

        /**
         * The whitelist is used to filter peer scan and connection requests.
         */
        ADV_POLICY_FILTER_ALL_REQS  = 3,
    };

    /**
     * Scanning policy filter mode.
     *
     * @see Bluetooth Core Specification 4.2 (Vol. 6), Part B, Section 4.3.3.
     */
    enum ScanningPolicyMode_t {
        /**
         * The whitelist is not used for scanning operations.
         */
        SCAN_POLICY_IGNORE_WHITELIST = 0,

        /**
        * The whitelist is used to filter incoming advertising.
        */
        SCAN_POLICY_FILTER_ALL_ADV = 1,
    };

    /**
     * Connection initiation policy filter mode.
     *
     * @see Bluetooth Core Specification 4.2 (vol. 6), Part B, Section 4.4.4.
     */
    enum InitiatorPolicyMode_t {
        /**
         * Connection can be initiated to any device.
         */
        INIT_POLICY_IGNORE_WHITELIST = 0,

        /**
         * Connection initiation is restricted to the devices present in the
         * whitelist.
         */
        INIT_POLICY_FILTER_ALL_ADV = 1,
    };

    /**
     * Description of the states of the device.
     */
    struct GapState_t {
        /**
         * If set, the device is currently advertising.
         */
        unsigned advertising : 1;

        /**
         * If set, the device is connected to at least one other peer.
         */
        unsigned connected : 1;
    };

    /**
     * Opaque value type representing a connection handle.
     *
     * It is used to identify to refer to a specific connection across Gap,
     * GattClient and GattEvent API.
     *
     * @note instances are generated by in the connection callback.
     */
    typedef ble::connection_handle_t Handle_t;

    /**
     * Enumeration of random address types.
     */
    typedef ble::random_address_type_t RandomAddressType_t;

    /**
     * Enumeration of peer address types
     */
    typedef ble::peer_address_type_t PeerAddressType_t;

    /**
     * Enumeration of BLE PHY
     */
    typedef ble::phy_t Phy_t;

    /**
     * Set of BLE PHYs
     */
    typedef ble::phy_set_t PhySet_t;

    /**
     * Enumeration of type of symbols that can be used with LE coded PHY.
     */
    typedef ble::coded_symbol_per_bit_t CodedSymbolPerBit_t;

    /**
     * Parameters of a BLE connection.
     */
    typedef struct {
        /**
         * Minimum interval between two connection events allowed for a
         * connection.
         *
         * It shall be less than or equal to maxConnectionInterval. This value,
         * in units of 1.25ms, is included in the range [0x0006 : 0x0C80].
         */
        uint16_t minConnectionInterval;

        /**
         * Maximum interval between two connection events allowed for a
         * connection.
         *
         * It shall be greater than or equal to minConnectionInterval. This
         * value is in unit of 1.25ms and is in the range [0x0006 : 0x0C80].
         */
        uint16_t maxConnectionInterval;

        /**
         * Number of connection events the slave can drop if it has nothing to
         * communicate to the master.
         *
         * This value shall be in the range [0x0000 : 0x01F3].
         */
        uint16_t slaveLatency;

        /**
         * Link supervision timeout for the connection.
         *
         * Time after which the connection is considered lost if the device
         * didn't receive a packet from its peer.
         *
         * It is larger than:
         *        (1 + slaveLatency) * maxConnectionInterval * 2
         *
         * This value is in the range [0x000A : 0x0C80] and is in unit of
         * 10 ms.
         *
         * @note maxConnectionInterval is in ms in the formulae above.
         */
        uint16_t connectionSupervisionTimeout;
    } ConnectionParams_t;

    /**
     * Enumeration of GAP roles.
     *
     * @note The BLE API does not express the broadcaster and scanner roles.
     *
     * @attention A device can fulfill different roles concurrently.
     */
    enum Role_t {
        /**
         * Peripheral Role.
         *
         * The device can advertise and it can be connected by a central. It
         * acts as a slave when connected.
         *
         * @note A peripheral is a broadcaster.
         */
        PERIPHERAL = 0x1,

        /**
         * Central Role.
         *
         * The device can scan and initiate connection to peripherals. It
         * acts as the master when a connection is established.
         *
         * @note A central is a scanner.
         */
        CENTRAL = 0x2,
    };

    /**
     * Representation of a scanned advertising packet.
     *
     * Instances of this type are passed to the callback registered in
     * startScan().
     */
    struct AdvertisementCallbackParams_t {
        /**
         * Default constructor.
         */
        AdvertisementCallbackParams_t();

        /**
         * BLE address of the device that has advertised the packet.
         */
        BLEProtocol::AddressBytes_t peerAddr;

        /**
         * RSSI value of the packet.
         */
        int8_t rssi;

        /**
         * Flag indicating if the packet is a response to a scan request.
         */
        bool isScanResponse;

        /**
         * Type of advertisement.
         */
        GapAdvertisingParams::AdvertisingType_t type;

        /**
         * Length of the advertisement data.
         */
        uint8_t advertisingDataLen;

        /**
         * Pointer to the advertisement packet's data.
         */
        const uint8_t *advertisingData;

        /**
         * Type of the address received.
         *
         * @note This value should be used in the connect function to establish
         * a connection with the peer that has sent this advertisement packet.
         */
        PeerAddressType_t peerAddrType;
    };

    /**
     * Type of the callback handling scanned advertisement packets.
     *
     * @see Gap::startScan().
     */
    typedef FunctionPointerWithContext<const AdvertisementCallbackParams_t *>
        AdvertisementReportCallback_t;

    /**
     * Connection events.
     *
     * It contains all the information related to a newly established connection.
     *
     * Instances of this structure are passed to handlers that
     * Gap::onConnection() registers when a connection is established.
     */
    struct ConnectionCallbackParams_t {
        /**
         * Connection handle.
         */
        Handle_t handle;

        /**
         * Connection Role of the local device.
         */
        Role_t role;

        /**
         * Address of the peer.
         */
        BLEProtocol::AddressBytes_t peerAddr;

        /**
         * Address type of the local device.
         */
        BLEProtocol::AddressType_t  ownAddrType;

        /**
         * Connection parameters.
         */
        const ConnectionParams_t *connectionParams;

        /**
         * Resolvable address used by the peer.
         *
         * @note All bytes of the address are set to 0 if not applicable
         */
        BLEProtocol::AddressBytes_t peerResolvableAddr;

        /**
         * resolvable address of the local device.
         *
         * @note All bytes of the address are set to 0 if not applicable
         */
        BLEProtocol::AddressBytes_t localResolvableAddr;

        /**
         * Type of the address the peer uses.
         */
        PeerAddressType_t peerAddressType;

        /**
         * Construct an instance of ConnectionCallbackParams_t.
         *
         * @param[in] handleIn Value to assign to handle.
         * @param[in] roleIn Value to assign to role.
         * @param[in] peerAddrTypeIn Value to assign to peerAddrType.
         * @param[in] peerAddrIn Value to assign to peerAddr.
         * @param[in] ownAddrTypeIn Value to assign to ownAddrType.
         * @param[in] ownAddrIn Value to assign to ownAddr. This may be NULL.
         * @param[in] connectionParamsIn Value to assign to connectionParams.
         * @param[in] peerResolvableAddrIn Value to assign to peerResolvableAddr.
         * @param[in] localResolvableAddrIn Value to assign to localResolvableAddr.
         *
         * @note Constructor is not meant to be called by user code.
         * The BLE API vendor code generates ConnectionCallbackParams_t.
         */
        ConnectionCallbackParams_t(
            Handle_t handleIn,
            Role_t roleIn,
            PeerAddressType_t peerAddrTypeIn,
            const uint8_t *peerAddrIn,
            BLEProtocol::AddressType_t ownAddrTypeIn,
            const uint8_t *ownAddrIn,
            const ConnectionParams_t *connectionParamsIn,
            const uint8_t *peerResolvableAddrIn = NULL,
            const uint8_t *localResolvableAddrIn = NULL
        );

    private:
        void constructor_helper(
            const uint8_t *peerAddrIn,
            const uint8_t *ownAddrIn,
            const uint8_t *peerResolvableAddrIn,
            const uint8_t *localResolvableAddrIn
        );
    };

    /**
     * Disconnection event.
     *
     * Instances of this event are passed to callbacks registered with
     * Gap::onDisconnection() when a connection ends.
     *
     * @note Constructor is not meant to be called by user code.
     * The BLE API vendor code generates ConnectionCallbackParams_t.
     */
    struct DisconnectionCallbackParams_t {
        /**
         * ID of the connection that has ended.
         */
        Handle_t handle;

        /**
         * Reason of the disconnection.
         */
        DisconnectionReason_t reason;

        /**
         * Construct a DisconnectionCallbackParams_t.
         *
         * @param[in] handleIn Value assigned to handle.
         * @param[in] reasonIn Value assigned to reason.
         */
        DisconnectionCallbackParams_t(
            Handle_t handleIn,
            DisconnectionReason_t reasonIn
        ) : handle(handleIn),
            reason(reasonIn)
        {
        }
    };

    /**
     * @copydoc ble::peripheral_privacy_configuration_t
     */
    typedef ble::peripheral_privacy_configuration_t PeripheralPrivacyConfiguration_t;

    /**
     * @copydoc ble::central_privay_configuration_t
     */
    typedef ble::central_privay_configuration_t CentralPrivacyConfiguration_t;

    /**
     * Number of microseconds in 1.25 milliseconds.
     */
    static const uint16_t UNIT_1_25_MS = 1250;

    /**
     * Convert milliseconds into 1.25ms units.
     *
     * This function may be used to convert ms time of connection intervals into
     * the format expected for connection parameters.
     *
     * @param[in] durationInMillis The duration in milliseconds.
     *
     * @return The duration in unit of 1.25ms.
     */
    static uint16_t MSEC_TO_GAP_DURATION_UNITS(uint32_t durationInMillis)
    {
        return (durationInMillis * 1000) / UNIT_1_25_MS;
    }

    /**
     * Timeout event handler.
     *
     * @see Gap::onTimeout().
     */
    typedef FunctionPointerWithContext<TimeoutSource_t> TimeoutEventCallback_t;

    /**
     * Callchain of timeout event handlers.
     *
     * @see Gap::onTimeout().
     */
    typedef CallChainOfFunctionPointersWithContext<TimeoutSource_t>
        TimeoutEventCallbackChain_t;

    /**
     * Connection event handler.
     *
     * @see Gap::onConnection().
     */
    typedef FunctionPointerWithContext<const ConnectionCallbackParams_t *>
        ConnectionEventCallback_t;

    /**
     * Callchain of connection event handlers.
     *
     * @see Gap::onConnection().
     */
    typedef CallChainOfFunctionPointersWithContext<const ConnectionCallbackParams_t *>
        ConnectionEventCallbackChain_t;

    /**
     * Disconnection event handler.
     *
     * @see Gap::onDisconnection().
     */
    typedef FunctionPointerWithContext<const DisconnectionCallbackParams_t *>
        DisconnectionEventCallback_t;

    /**
     * Callchain of disconnection event handlers.
     *
     * @see Gap::onDisconnection().
     */
    typedef CallChainOfFunctionPointersWithContext<const DisconnectionCallbackParams_t *>
        DisconnectionEventCallbackChain_t;

    /**
     * Radio notification event handler.
     *
     * @see Gap::onRadioNotification().
     */
    typedef FunctionPointerWithContext<bool> RadioNotificationEventCallback_t;

    /**
     * Gap shutdown event handler.
     *
     * @see Gap::onShutdown().
     */
    typedef FunctionPointerWithContext<const LegacyGap *> GapShutdownCallback_t;

    /**
     * Callchain of gap shutdown event handler.
     *
     * @see Gap::onShutdown().
     */
    typedef CallChainOfFunctionPointersWithContext<const LegacyGap *>
        GapShutdownCallbackChain_t;

    /*
     * The following functions are meant to be overridden in the platform-specific subclass.
     */
public:

    /**
     * Fetch the current address and its type.
     *
     * @param[out] typeP Type of the current address set.
     * @param[out] address Value of the current address.
     *
     * @note If privacy is enabled the device address may be unavailable to
     * application code.
     *
     * @return BLE_ERROR_NONE on success.
     */
    ble_error_t getAddress(
        BLEProtocol::AddressType_t *typeP,
        BLEProtocol::AddressBytes_t address
    );

    /**
     * Return the type of a random address.
     *
     * @param[in] address The random address to retrieve the type from. The
     * address must be ordered in little endian.
     *
     * @param[out] addressType Type of the address to fill.
     *
     * @return BLE_ERROR_NONE in case of success or BLE_ERROR_INVALID_PARAM if
     * the address in input was not identifiable as a random address.
     */
    static ble_error_t getRandomAddressType(
        const BLEProtocol::AddressBytes_t address,
        RandomAddressType_t *addressType
    );

#if BLE_ROLE_BROADCASTER
    /**
     * Get the minimum advertising interval in milliseconds, which can be used
     * for connectable advertising types.
     *
     * @return Minimum Advertising interval in milliseconds for connectable
     * undirected and connectable directed advertising types.
     */
    uint16_t getMinAdvertisingInterval(void) const;

    /**
     * Get the minimum advertising interval in milliseconds, which can be
     * used for nonconnectable advertising type.
     *
     * @return Minimum Advertising interval in milliseconds for scannable
     * undirected and nonconnectable undirected event types.
     */
    uint16_t getMinNonConnectableAdvertisingInterval(void) const;

    /**
     * Get the maximum advertising interval in milliseconds.
     *
     * @return Maximum Advertising interval in milliseconds.
     */
    uint16_t getMaxAdvertisingInterval(void) const;

#endif //BLE_ROLE_BROADCASTER

#if BLE_FEATURE_CONNECTABLE

public:

    /**
     * Returned the preferred connection parameters exposed in the GATT Generic
     * Access Service.
     *
     * @param[out] params Structure where the parameters are stored.
     *
     * @return BLE_ERROR_NONE if the parameters were successfully filled into
     * @p params.
     */
    ble_error_t getPreferredConnectionParams(ConnectionParams_t *params);

    /**
     * Set the value of the preferred connection parameters exposed in the GATT
     * Generic Access Service.
     *
     * A connected peer may read the characteristic exposing these parameters
     * and request an update of the connection parameters to accommodate the
     * local device.
     *
     * @param[in] params Value of the preferred connection parameters.
     *
     * @return BLE_ERROR_NONE if the preferred connection params were set
     * correctly.
     */
    ble_error_t setPreferredConnectionParams(
        const ConnectionParams_t *params
    );

#endif // BLE_FEATURE_CONNECTABLE
#if BLE_FEATURE_GATT_SERVER
    /**
     * Set the value of the device name characteristic in the Generic Access
     * Service.
     *
     * @param[in] deviceName The new value for the device-name. This is a
     * UTF-8 encoded, <b>NULL-terminated</b> string.
     *
     * @return BLE_ERROR_NONE if the device name was set correctly.
     */
    ble_error_t setDeviceName(const uint8_t *deviceName);

    /**
     * Get the value of the device name characteristic in the Generic Access
     * Service.
     *
     * To obtain the length of the deviceName value, this function is
     * invoked with the @p deviceName parameter set to NULL.
     *
     * @param[out] deviceName Pointer to an empty buffer where the UTF-8
     * <b>non NULL-terminated</b> string is placed.
     *
     * @param[in,out] lengthP Length of the @p deviceName buffer. If the device
     * name is successfully copied, then the length of the device name
     * string (excluding the null terminator) replaces this value.
     *
     * @return BLE_ERROR_NONE if the device name was fetched correctly from the
     * underlying BLE stack.
     *
     * @note If the device name is longer than the size of the supplied buffer,
     * length returns the complete device name length and not the number of
     * bytes actually returned in deviceName. The application may use this
     * information to retry with a suitable buffer size.
     */
    ble_error_t getDeviceName(uint8_t *deviceName, unsigned *lengthP);

    /**
     * Set the value of the appearance characteristic in the GAP service.
     *
     * @param[in] appearance The new value for the device-appearance.
     *
     * @return BLE_ERROR_NONE if the new appearance was set correctly.
     */
    ble_error_t setAppearance(GapAdvertisingData::Appearance appearance);

    /**
     * Get the value of the appearance characteristic in the GAP service.
     *
     * @param[out] appearanceP The current device-appearance value.
     *
     * @return BLE_ERROR_NONE if the device-appearance was fetched correctly
     * from the underlying BLE stack.
     */
    ble_error_t getAppearance(GapAdvertisingData::Appearance *appearanceP);
#endif // BLE_FEATURE_GATT_SERVER

    /*
     * APIs with nonvirtual implementations.
     */

//     /* Event handlers. */
public:

    /**
     * Register a Gap shutdown event handler.
     *
     * The handler is called when the Gap instance is about to shut down.
     * It is usually issued after a call to BLE::shutdown().
     *
     * @param[in] callback Shutdown event handler to register.
     *
     * @note To unregister a shutdown event handler, use
     * onShutdown().detach(callback).
     */
    void onShutdown(const GapShutdownCallback_t &callback);

    /**
     * Register a Gap shutdown event handler.
     *
     * @param[in] objPtr Instance used to invoke @p memberPtr.
     * @param[in] memberPtr Shutdown event handler to register.
     */
    template<typename T>
    void onShutdown(T *objPtr, void (T::*memberPtr)(const LegacyGap *))
    {
        shutdownCallChain.add(objPtr, memberPtr);
    }

    /**
     * Access the callchain of shutdown event handler.
     *
     * @note To register callbacks, use onShutdown().add(callback).
     *
     * @note To unregister callbacks, use onShutdown().detach(callback).
     *
     * @return A reference to the shutdown event callback chain.
     */
    GapShutdownCallbackChain_t &onShutdown();

public:
    /**
     * Reset the Gap instance.
     *
     * Reset process starts by notifying all registered shutdown event handlers
     * that the Gap instance is about to be shut down. Then, it clears all Gap state
     * of the associated object and then cleans the state present in the vendor
     * implementation.
     *
     * This function is meant to be overridden in the platform-specific
     * subclass. Nevertheless, the subclass only resets its
     * state and not the data held in Gap members. This is achieved by a
     * call to Gap::reset() from the subclass' reset() implementation.
     *
     * @return BLE_ERROR_NONE on success.
     *
     * @note Currently, a call to reset() does not reset the advertising and
     * scan parameters to default values.
     */
    ble_error_t reset(void);

protected:
    /**
     * Construct a Gap instance.
     */
    LegacyGap();

    /* Entry points for the underlying stack to report events back to the user. */
public:
    /**
     * Notify all registered connection event handlers of a connection event.
     *
     * @attention This function is meant to be called from the BLE stack specific
     * implementation when a connection event occurs.
     *
     * @param[in] handle Handle of the new connection.
     * @param[in] role Role of this BLE device in the connection.
     * @param[in] peerAddrType Address type of the connected peer.
     * @param[in] peerAddr Address of the connected peer.
     * @param[in] ownAddrType Address type this device uses for this
     * connection.
     * @param[in] ownAddr Address this device uses for this connection. This
     * parameter may be NULL if the local address is not available.
     * @param[in] connectionParams Parameters of the connection.
     * @param[in] peerResolvableAddr Resolvable address used by the peer.
     * @param[in] localResolvableAddr resolvable address used by the local device.
     *
     * @deprecated Deprecated since addition of extended advertising support.
     * Use EventHandler::onConnectionComplete() instead.
     */
    MBED_DEPRECATED_SINCE(
        "mbed-os-5.11.0",
        "Deprecated since addition of extended advertising support. "
        "Use EventHandler::onConnectionComplete() instead"
    )
    void processConnectionEvent(
        Handle_t handle,
        Role_t role,
        PeerAddressType_t peerAddrType,
        const BLEProtocol::AddressBytes_t peerAddr,
        BLEProtocol::AddressType_t ownAddrType,
        const BLEProtocol::AddressBytes_t ownAddr,
        const ConnectionParams_t *connectionParams,
        const uint8_t *peerResolvableAddr = NULL,
        const uint8_t *localResolvableAddr = NULL
    );

    /**
     * Notify all registered disconnection event handlers of a disconnection event.
     *
     * @attention This function is meant to be called from the BLE stack specific
     * implementation when a disconnection event occurs.
     *
     * @param[in] handle Handle of the terminated connection.
     * @param[in] reason Reason of the disconnection.
     *
     * @deprecated Deprecated since addition of extended advertising support.
     * Use EventHandler::onDisconnection() instead.
     */
    MBED_DEPRECATED_SINCE(
        "mbed-os-5.11.0",
        "Deprecated since addition of extended advertising support. "
        "Use EventHandler::onDisconnectionComplete() instead"
    )
    void processDisconnectionEvent(Handle_t handle, DisconnectionReason_t reason);

    /**
     * Forward a received advertising packet to all registered event handlers
     * listening for scanned packet events.
     *
     * @attention This function is meant to be called from the BLE stack specific
     * implementation when a disconnection event occurs.
     *
     * @param[in] peerAddr Address of the peer that has emitted the packet.
     * @param[in] rssi Value of the RSSI measured for the received packet.
     * @param[in] isScanResponse If true, then the packet is a response to a scan
     * request.
     * @param[in] type Advertising type of the packet.
     * @param[in] advertisingDataLen Length of the advertisement data received.
     * @param[in] advertisingData Pointer to the advertisement packet's data.
     * @param[in] addressType Type of the address of the peer that has emitted
     * the packet.
     *
     * @deprecated Deprecated since addition of extended advertising support.
     * Use EventHandler::onAdvertisingReport() instead.
     */
    MBED_DEPRECATED_SINCE(
        "mbed-os-5.11.0",
        "Deprecated since addition of extended advertising support. "
        "Use EventHandler::onAdvertisingReport() instead"
    )
    void processAdvertisementReport(
        const BLEProtocol::AddressBytes_t peerAddr,
        int8_t rssi,
        bool isScanResponse,
        GapAdvertisingParams::AdvertisingType_t type,
        uint8_t advertisingDataLen,
        const uint8_t *advertisingData,
        PeerAddressType_t addressType
    );

    /**
     * Notify the occurrence of a timeout event to all registered timeout events
     * handler.
     *
     * @attention This function is meant to be called from the BLE stack specific
     * implementation when a disconnection event occurs.
     *
     * @param[in] source Source of the timout event.
     *
     * @deprecated Deprecated since addition of extended advertising support.
     * Use EventHandler instead.
     */
    MBED_DEPRECATED_SINCE(
        "mbed-os-5.11.0",
        "Deprecated since addition of extended advertising support. "
        "Use EventHandler instead"
    )
    void processTimeoutEvent(TimeoutSource_t source);

protected:
    /**
     * Current advertising parameters.
     */
    GapAdvertisingParams _advParams;

    /**
     * Current advertising data.
     */
    GapAdvertisingData _advPayload;

    /**
     * Current scanning parameters.
     */
    GapScanningParams _scanningParams;

    /**
     * Current scan response.
     */
    GapAdvertisingData _scanResponse;

    /**
     * Number of open connections.
     */
    uint8_t connectionCount;

    /**
     * Current GAP state.
     */
    GapState_t state;

    /**
     * Active scanning flag.
     */
    bool scanningActive;

protected:
    /**
     * Callchain containing all registered callback handlers for timeout
     * events.
     */
    TimeoutEventCallbackChain_t timeoutCallbackChain;

    /**
     * The registered callback handler for radio notification events.
     */
    RadioNotificationEventCallback_t radioNotificationCallback;

    /**
     * The registered callback handler for scanned advertisement packet
     * notifications.
     */
    AdvertisementReportCallback_t onAdvertisementReport;

    /**
     * Callchain containing all registered callback handlers for connection
     * events.
     */
    ConnectionEventCallbackChain_t connectionCallChain;

    /**
     * Callchain containing all registered callback handlers for disconnection
     * events.
     */
    DisconnectionEventCallbackChain_t disconnectionCallChain;

private:
    /**
     * Callchain containing all registered callback handlers for shutdown
     * events.
     */
    GapShutdownCallbackChain_t shutdownCallChain;

private:
    /* Disallow copy and assignment. */
    LegacyGap(const LegacyGap &);

    LegacyGap &operator=(const LegacyGap &);


protected:
    using ble::interface::Gap<Impl>::startAdvertising_;
    using ble::interface::Gap<Impl>::stopAdvertising_;
    using ble::interface::Gap<Impl>::connect_;
    using ble::interface::Gap<Impl>::disconnect_;

    /* --- Abstract calls with default implementation --- */
    uint16_t getMinAdvertisingInterval_(void) const;

    uint16_t getMinNonConnectableAdvertisingInterval_(void) const;

    uint16_t getMaxAdvertisingInterval_(void) const;

    /* Note: Implementation must call the base class reset_ */
    ble_error_t reset_(void);

    /* --- Abstract calls to override --- */

    ble_error_t getAddress_(
        BLEProtocol::AddressType_t *typeP,
        BLEProtocol::AddressBytes_t address
    );

    ble_error_t getPreferredConnectionParams_(ConnectionParams_t *params);

    ble_error_t setPreferredConnectionParams_(
        const ConnectionParams_t *params
    );

    ble_error_t setDeviceName_(const uint8_t *deviceName);

    ble_error_t getDeviceName_(uint8_t *deviceName, unsigned *lengthP);

    ble_error_t setAppearance_(GapAdvertisingData::Appearance appearance);

    ble_error_t getAppearance_(GapAdvertisingData::Appearance *appearanceP);
};

/**
 * @}
 * @}
 */

} // interface
} // ble

// import LegacyGap implementation into global namespace
typedef ble::impl::LegacyGap Gap;

// import Gap implementation into ble namespace
namespace ble {
typedef impl::Gap Gap;
}


#endif // ifndef MBED_BLE_GAP_H__
