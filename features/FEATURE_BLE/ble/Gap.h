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
     * Address-type for BLEProtocol addresses.
     *
     * @deprecated Use BLEProtocol::AddressType_t instead.
     */
    typedef BLEProtocol::AddressType_t AddressType_t;

    /**
     * Address-type for BLEProtocol addresses.
     *
     * @deprecated Use BLEProtocol::AddressType_t instead.
     */
    typedef BLEProtocol::AddressType_t addr_type_t;

    /**
     * Address-type for BLEProtocol addresses.
     *
     * @deprecated Use BLEProtocol::AddressType_t instead. The following
     * constants have been left in their deprecated state to transparently
     * support existing applications that may have used Gap::ADDR_TYPE_*.
     */
    enum DeprecatedAddressType_t {
        ADDR_TYPE_PUBLIC = BLEProtocol::AddressType::PUBLIC,
        ADDR_TYPE_RANDOM_STATIC = BLEProtocol::AddressType::RANDOM_STATIC,
        ADDR_TYPE_RANDOM_PRIVATE_RESOLVABLE = BLEProtocol::AddressType::RANDOM_PRIVATE_RESOLVABLE,
        ADDR_TYPE_RANDOM_PRIVATE_NON_RESOLVABLE = BLEProtocol::AddressType::RANDOM_PRIVATE_NON_RESOLVABLE
    };

    /**
     * Length (in octets) of the BLE MAC address.
     */
    static const unsigned ADDR_LEN = BLEProtocol::ADDR_LEN;

    /**
     * 48-bit address, LSB format.
     *
     * @deprecated Use BLEProtocol::AddressBytes_t instead.
     */
    typedef BLEProtocol::AddressBytes_t Address_t;

    /**
     * 48-bit address, LSB format.
     *
     * @deprecated Use BLEProtocol::AddressBytes_t instead.
     */
    typedef BLEProtocol::AddressBytes_t address_t;

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
     * Representation of a whitelist of addresses.
     */
    struct Whitelist_t {
        /**
         * Pointer to the array of the addresses composing the whitelist.
         */
        BLEProtocol::Address_t *addresses;

        /**
         * Number addresses in this whitelist.
         */
        uint8_t size;

        /**
         * Capacity of the array holding the addresses.
         */
        uint8_t capacity;
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
         * @deprecated AddressType_t do not carry enough information to be used
         * when privacy is enabled. Use peerAddressType instead.
         *
         * @note This value should be used in the connect function to establish
         * a connection with the peer that has sent this advertisement packet.
         */
        MBED_DEPRECATED_SINCE(
            "mbed-os-5.9.0",
            "addressType won't work in connect when privacy is enabled; please"
            "use peerAddrType"
        )
        AddressType_t addressType;

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
         * Type of the address the peer uses.
         *
         * @deprecated The type BLEProtocol::AddressType_t is not suitable when
         * privacy is enabled. Use peerAddressType instead.
         */
        MBED_DEPRECATED_SINCE(
            "mbed-os-5.9",
            "The type BLEProtocol::AddressType_t is not suitable when privacy is "
            "enabled. Use peerAddressType instead."
        )
        BLEProtocol::AddressType_t peerAddrType;

        /**
         * Address of the peer.
         */
        BLEProtocol::AddressBytes_t peerAddr;

        /**
         * Address type of the local device.
         */
        BLEProtocol::AddressType_t  ownAddrType;

        /**
         * Address of the local device.
         *
         * @deprecated The local address used for the connection may not be known,
         * Therefore this field is not reliable.
         *
         * @note All bytes of the address are set to 0 if not applicable
         */
        MBED_DEPRECATED_SINCE(
            "mbed-os-5.9",
            "A Bluetooth controller is not supposed to return the address it used"
            "to connect. With privacy enabled the controller address may be unknown"
            "to the host. There is no replacement for this deprecation."
        )
        BLEProtocol::AddressBytes_t ownAddr;

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

        /**
         * Construct an instance of ConnectionCallbackParams_t.
         *
         * @param[in] handleIn Value to assign to handle.
         * @param[in] roleIn Value to assign to role.
         * @param[in] peerAddrTypeIn Value to assign to peerAddrType.
         * @param[in] peerAddrIn Value to assign to peerAddr.
         * @param[in] ownAddrTypeIn Value to assign to ownAddrType.
         * @param[in] ownAddrIn Value to assign to ownAddr.
         * @param[in] connectionParamsIn Value to assign to connectionParams.
         * @param[in] peerResolvableAddrIn Value to assign to peerResolvableAddr.
         * @param[in] localResolvableAddrIn Value to assign to localResolvableAddr.
         *
         * @note Constructor is not meant to be called by user code.
         * The BLE API vendor code generates ConnectionCallbackParams_t.
         *
         * @deprecated The type BLEProtocol::AddressType_t is not suitable when
         * privacy is enabled. Use the constructor that accepts a
         * PeerAddressType_t instead.
         */
        MBED_DEPRECATED_SINCE(
            "mbed-os-5.9.0",
            "The type BLEProtocol::AddressType_t is not suitable when privacy is "
            "enabled. Use the constructor that accepts a PeerAddressType_t instead."
        )
        ConnectionCallbackParams_t(
            Handle_t handleIn,
            Role_t roleIn,
            BLEProtocol::AddressType_t peerAddrTypeIn,
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
     * Set the device MAC address and type.
     *
     * The address set is used in subsequent GAP operations: scanning,
     * advertising and connection initiation.
     *
     * @param[in] type Type of the address to set.
     * @param[in] address Value of the address to set. It is ordered in
     * little endian. This parameter is not considered if the address type
     * is RANDOM_PRIVATE_RESOLVABLE or RANDOM_PRIVATE_NON_RESOLVABLE. For those
     * types of address, the BLE API itself generates the address.
     *
     * @note Some implementation may refuse to set a new PUBLIC address.
     * @note Random static address set does not change.
     *
     * @deprecated Starting with mbed-os-5.9.0 this function is deprecated and
     * address management is delegated to implementation. Implementations may or
     * may not continue to support this function. Compliance with the Bluetooth
     * specification and unification of behaviour between implementations are
     * the key reasons behind this change:
     *   - Many implementations do not allow changing of the public address.
     *   Therefore programs relying on this function are not portable across BLE
     *   implementations.
     *   - The Bluetooth specification forbid replacement of the random static
     *   address; this address can be set once and only once: at startup.
     *   Depending on the underlying implementation the random address may or
     *   may not have been set automatically at startup; therefore update of the
     *   Random Static address after ble initialisation may be a fault. As a
     *   result calls to this function were not portable.
     *   Furthermore replacement of the random static address silently
     *   invalidates the bond stored in the secure database.

     * @return BLE_ERROR_NONE on success.
     */
    MBED_DEPRECATED_SINCE(
        "mbed-os-5.9.0",
        "Non portable API, use enablePrivacy to enable use of private addresses"
    )
    ble_error_t setAddress(
        BLEProtocol::AddressType_t type,
        const BLEProtocol::AddressBytes_t address
    );

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

    /**
     * Stop the ongoing advertising procedure.
     *
     * @note The current advertising parameters remain in effect.
     *
     * @retval BLE_ERROR_NONE if the advertising procedure has been successfully
     * stopped.
     *
     * @deprecated Deprecated since addition of extended advertising support.
     * Use stopAdvertising(advertising_handle_t).
     */
    MBED_DEPRECATED_SINCE(
        "mbed-os-5.11.0",
        "Deprecated since addition of extended advertising support."
        "Use stopAdvertising(advertising_handle_t) instead."
    )
    ble_error_t stopAdvertising(void);
#endif //BLE_ROLE_BROADCASTER
#if BLE_ROLE_CENTRAL
    /**
     * Initiate a connection to a peer.
     *
     * Once the connection is established, a ConnectionCallbackParams_t event is
     * emitted to handlers that have been registered with onConnection().
     *
     * @param[in] peerAddr MAC address of the peer. It must be in LSB format.
     * @param[in] peerAddrType Address type of the peer. It is usually obtained
     * from advertising frames.
     * @param[in] connectionParams Connection parameters to use.
     * @param[in] scanParams Scan parameters used to find the peer.
     *
     * @return BLE_ERROR_NONE if connection establishment procedure is started
     * successfully. The connectionCallChain (if set) is invoked upon
     * a connection event.
     *
     * @deprecated Deprecated since addition of extended advertising support.
     * Use connect(target_peer_address_type_t, address_t, ConnectionParameters).
     */
    MBED_DEPRECATED_SINCE(
        "mbed-os-5.11.0",
        "Deprecated since addition of extended advertising support."
        "Use connect(target_peer_address_type_t, address_t, ConnectionParameters) instead."
    )
    ble_error_t connect(
        const BLEProtocol::AddressBytes_t peerAddr,
        PeerAddressType_t peerAddrType,
        const ConnectionParams_t *connectionParams,
        const GapScanningParams *scanParams
    );

    /**
     * Initiate a connection to a peer.
     *
     * Once the connection is established, a ConnectionCallbackParams_t event is
     * emitted to handlers that have been registered with onConnection().
     *
     * @param[in] peerAddr MAC address of the peer. It must be in LSB format.
     * @param[in] peerAddrType Address type of the peer.
     * @param[in] connectionParams Connection parameters to use.
     * @param[in] scanParams Scan parameters used to find the peer.
     *
     * @deprecated BLEProtocol::AddressType_t is not able to to carry accurate
     * meaning when privacy is in use. Please Uses the connect overload that
     * accept a PeerAddressType_t as the peer address type.
     *
     * @return BLE_ERROR_NONE if connection establishment procedure is started
     * successfully. The connectionCallChain (if set) is invoked upon
     * a connection event.
     */
    MBED_DEPRECATED_SINCE(
        "mbed-os-5.9.0",
        "This function won't work if privacy is enabled; You must use the overload "
        "accepting PeerAddressType_t."
    )
    ble_error_t connect(
        const BLEProtocol::AddressBytes_t peerAddr,
        BLEProtocol::AddressType_t peerAddrType,
        const ConnectionParams_t *connectionParams,
        const GapScanningParams *scanParams
    );

    /**
     * Initiate a connection to a peer.
     *
     * @see connect()
     *
     * @deprecated  This funtion overloads Gap::connect(
     *      const BLEProtocol::Address_t peerAddr,
     *      BLEProtocol::AddressType_t peerAddrType,
     *      const ConnectionParams_t *connectionParams,
     *      const GapScanningParams *scanParams
     * )
     * to maintain backward compatibility for changes from Gap::AddressType_t to
     * BLEProtocol::AddressType_t.
     */
    MBED_DEPRECATED("Gap::DeprecatedAddressType_t is deprecated, use BLEProtocol::AddressType_t instead")
    ble_error_t connect(
        const BLEProtocol::AddressBytes_t peerAddr,
        DeprecatedAddressType_t peerAddrType,
        const ConnectionParams_t *connectionParams,
        const GapScanningParams *scanParams
    );
#endif // BLE_ROLE_CENTRAL

#if BLE_FEATURE_CONNECTABLE
    /**
     * Initiate a disconnection procedure.
     *
     * Once the disconnection procedure has completed a
     * DisconnectionCallbackParams_t, the event is emitted to handlers that
     * have been registered with onDisconnection().
     *
     * @param[in] reason Reason of the disconnection transmitted to the peer.
     * @param[in] connectionHandle Handle of the connection to end.
     *
     * @return  BLE_ERROR_NONE if the disconnection procedure successfully
     * started.
     *
     * @deprecated Deprecated since addition of extended advertising support.
     * Use disconnect(connection_handle_t, local_disconnection_reason_t) instead.
     */
    MBED_DEPRECATED_SINCE(
        "mbed-os-5.11.0",
        "Deprecated since addition of extended advertising support."
        "Use disconnect(connection_handle_t, local_disconnection_reason_t) instead."
    )
    ble_error_t disconnect(
        Handle_t connectionHandle, DisconnectionReason_t reason
    );

    /**
     * Initiate a disconnection procedure.
     *
     * @deprecated This version of disconnect() doesn't take a connection handle.
     * It works reliably only for stacks that are limited to a single connection.
     * Use Gap::disconnect(Handle_t connectionHandle, DisconnectionReason_t reason)
     * instead.
     *
     * @param[in] reason The reason for disconnection; to be sent back to the peer.
     *
     * @return BLE_ERROR_NONE if disconnection was successful.
     */
    MBED_DEPRECATED("Use disconnect(Handle_t, DisconnectionReason_t) instead.")
    ble_error_t disconnect(DisconnectionReason_t reason);

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

    /**
     * Update connection parameters of an existing connection.
     *
     * In the central role, this initiates a Link Layer connection parameter
     * update procedure. In the peripheral role, this sends the corresponding
     * L2CAP request and waits for the central to perform the procedure.
     *
     * @param[in] handle Connection Handle.
     * @param[in] params Pointer to desired connection parameters.
     *
     * @return BLE_ERROR_NONE if the connection parameters were updated correctly.
     *
     * @deprecated Deprecated since addition of extended advertising support.
     * Use updateConnectionParameters(connection_handle_t, conn_interval_t,
     * conn_interval_t, slave_latency_t, supervision_timeout_t,
     * conn_event_length_t, conn_event_length_t) instead.
     */
    MBED_DEPRECATED_SINCE(
        "mbed-os-5.11.0",
        "Deprecated since addition of extended advertising support."
        "Use updateConnectionParameters(connection_handle_t, conn_interval_t, "
        "conn_interval_t, slave_latency_t, supervision_timeout_t, "
        "conn_event_length_t, conn_event_length_t) instead."
    )
    ble_error_t updateConnectionParams(
        Handle_t handle,
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

    /**
     * Set the radio's transmit power.
     *
     * @param[in] txPower Radio's transmit power in dBm.
     *
     * @return BLE_ERROR_NONE if the new radio's transmit power was set
     * correctly.
     *
     * @deprecated Deprecated since addition of extended advertising support.
     * See ble::AdvertisingParameters and setAdvertisingParameters.
     */
    MBED_DEPRECATED_SINCE(
        "mbed-os-5.11.0",
        "Deprecated since addition of extended advertising support."
        "See ble::AdvertisingParameters and setAdvertisingParameters."
    )
    ble_error_t setTxPower(int8_t txPower);

    /**
     * Query the underlying stack for allowed Tx power values.
     *
     * @param[out] valueArrayPP Receive the immutable array of Tx values.
     * @param[out] countP Receive the array's size.
     *
     * @deprecated Deprecated since addition of extended advertising support.
     */
    MBED_DEPRECATED_SINCE(
        "mbed-os-5.11.0",
        "Deprecated since addition of extended advertising support."
    )
    void getPermittedTxPowerValues(
        const int8_t **valueArrayPP, size_t *countP
    );

#if BLE_FEATURE_WHITELIST
    /**
     * Get the maximum size of the whitelist.
     *
     * @return Maximum size of the whitelist.
     *
     * @note If using Mbed OS, you can configure the size of the whitelist by
     * setting the YOTTA_CFG_WHITELIST_MAX_SIZE macro in your yotta config file.
     */
    uint8_t getMaxWhitelistSize(void) const;

    /**
     * Get the Link Layer to use the internal whitelist when scanning,
     * advertising or initiating a connection depending on the filter policies.
     *
     * @param[in,out] whitelist Define the whitelist instance which is used
     * to store the whitelist requested. In input, the caller provisions memory.
     *
     * @return BLE_ERROR_NONE if the implementation's whitelist was successfully
     * copied into the supplied reference.
     */
    ble_error_t getWhitelist(Whitelist_t &whitelist) const;

    /**
     * Set the value of the whitelist to be used during GAP procedures.
     *
     * @param[in] whitelist A reference to a whitelist containing the addresses
     * to be copied to the internal whitelist.
     *
     * @return BLE_ERROR_NONE if the implementation's whitelist was successfully
     * populated with the addresses in the given whitelist.
     *
     * @note The whitelist must not contain addresses of type @ref
     * BLEProtocol::AddressType::RANDOM_PRIVATE_NON_RESOLVABLE. This
     * results in a @ref BLE_ERROR_INVALID_PARAM because the remote peer might
     * change its private address at any time, and it is not possible to resolve
     * it.
     *
     * @note If the input whitelist is larger than @ref getMaxWhitelistSize(),
     * then @ref BLE_ERROR_PARAM_OUT_OF_RANGE is returned.
     */
    ble_error_t setWhitelist(const Whitelist_t &whitelist);

    /**
     * Set the advertising policy filter mode to be used during the next
     * advertising procedure.
     *
     * @param[in] mode New advertising policy filter mode.
     *
     * @return BLE_ERROR_NONE if the specified policy filter mode was set
     * successfully.
     *
     * @deprecated Deprecated since addition of extended advertising support.
     * This setting is now part of ble::AdvertisingParameters.
     */
    MBED_DEPRECATED_SINCE(
        "mbed-os-5.11.0",
        "Deprecated since addition of extended advertising support."
        "This setting is now part of ble::AdvertisingParameters."
    )
    ble_error_t setAdvertisingPolicyMode(AdvertisingPolicyMode_t mode);

    /**
     * Set the scan policy filter mode to be used during the next scan procedure.
     *
     * @param[in] mode New scan policy filter mode.
     *
     * @return BLE_ERROR_NONE if the specified policy filter mode was set
     * successfully.
     *
     * @deprecated Deprecated since addition of extended advertising support.
     * This setting is now part of ble::ScanParameters.
     */
    MBED_DEPRECATED_SINCE(
        "mbed-os-5.11.0",
        "Deprecated since addition of extended advertising support."
        "This setting is now part of ble::ScanParameters."
    )
    ble_error_t setScanningPolicyMode(ScanningPolicyMode_t mode);

    /**
     * Set the initiator policy filter mode to be used during the next connection
     * initiation.
     *
     * @param[in] mode New initiator policy filter mode.
     *
     * @return BLE_ERROR_NONE if the specified policy filter mode was set
     * successfully.
     *
     * @deprecated Deprecated since addition of extended advertising support.
     * This setting is now part of ble::ConnectionParameters.
     */
    MBED_DEPRECATED_SINCE(
        "mbed-os-5.11.0",
        "Deprecated since addition of extended advertising support."
        "This setting is now part of ble::ConnectionParameters."
    )
    ble_error_t setInitiatorPolicyMode(InitiatorPolicyMode_t mode);

    /**
     * Get the current advertising policy filter mode.
     *
     * @return The current advertising policy filter mode.
     *
     * @deprecated Deprecated since addition of extended advertising support.
     */
    MBED_DEPRECATED_SINCE(
        "mbed-os-5.11.0",
        "Deprecated since addition of extended advertising support."
    )
    AdvertisingPolicyMode_t getAdvertisingPolicyMode(void) const;

    /**
     * Get the current scan policy filter mode.
     *
     * @return The current scan policy filter mode.
     *
     * @deprecated Deprecated since addition of extended advertising support.
     */
    MBED_DEPRECATED_SINCE(
        "mbed-os-5.11.0",
        "Deprecated since addition of extended advertising support."
    )
    ScanningPolicyMode_t getScanningPolicyMode(void) const;

    /**
     * Get the current initiator policy filter mode.
     *
     * @return The current scan policy filter mode.
     *
     * @deprecated Deprecated since addition of extended advertising support.
     */
    MBED_DEPRECATED_SINCE(
        "mbed-os-5.11.0",
        "Deprecated since addition of extended advertising support."
    )
    InitiatorPolicyMode_t getInitiatorPolicyMode(void) const;
#endif // BLE_FEATURE_WHITELIST

protected:
#if BLE_ROLE_OBSERVER
    /* Override the following in the underlying adaptation layer to provide the
      functionality of scanning. */

    /**
     * Start scanning procedure in the underlying BLE stack.
     *
     * @param[in] scanningParams Parameters of the scan procedure.
     *
     * @return BLE_ERROR_NONE if the scan procedure was successfully started.
     *
     * @deprecated Deprecated since addition of extended advertising support.
     * Vendors should use the Cordio HCI interface or the ble::pal or implement
     * startScan(duplicates_filter_t, scan_duration_t, period)
     */
    MBED_DEPRECATED_SINCE(
        "mbed-os-5.11.0",
        "Deprecated since addition of extended advertising support. "
        "Vendors should use the cordio hci interface or the ble::pal or "
        "implement startScan(duplicates_filter_t, scan_duration_t, period)"
    )
    ble_error_t startRadioScan(const GapScanningParams &scanningParams);
#endif // BLE_ROLE_OBSERVER

    /*
     * APIs with nonvirtual implementations.
     */
public:
    /**
     * Get the current advertising and connection states of the device.
     *
     * @return The current GAP state of the device.
     *
     * @deprecated Deprecated since addition of extended advertising support.
     * This is not meaningful when you use extended advertising; Please replace
     * getState().advertising with isAdvertisingActive(), and replace
     * getState().connected with your own record and update during callbacks.
     */
    MBED_DEPRECATED_SINCE(
        "mbed-os-5.11.0",
        "Deprecated since addition of extended advertising support. "
        "Replace getState().advertising with isAdvertisingActive()."
        "Replace getState().connected with your own record and update during callbacks."
    )
    GapState_t getState(void) const;

#if BLE_ROLE_BROADCASTER
    /**
     * Set the advertising type to use during the advertising procedure.
     *
     * @param[in] advType New type of advertising to use.
     *
     * @deprecated Deprecated since addition of extended advertising support.
     * This option is now part of ble::AdvertisingParameters.
     */
    MBED_DEPRECATED_SINCE(
        "mbed-os-5.11.0",
        "Deprecated since addition of extended advertising support. "
        "This option is now part of ble::AdvertisingParameters"
    )
    void setAdvertisingType(GapAdvertisingParams::AdvertisingType_t advType);

    /**
     * Set the advertising interval.
     *
     * @param[in] interval Advertising interval in units of milliseconds.
     * Advertising is disabled if interval is 0. If interval is smaller than
     * the minimum supported value, then the minimum supported value is used
     * instead. This minimum value can be discovered using
     * getMinAdvertisingInterval().
     *
     * This field must be set to 0 if connectionMode is equal
     * to ADV_CONNECTABLE_DIRECTED.
     *
     * @note  Decreasing this value allows central devices to detect a
     * peripheral faster, at the expense of the radio using more power
     * due to the higher data transmit rate.
     *
     * @deprecated Deprecated since addition of extended advertising support.
     * This option is now part of ble::AdvertisingParameters.
     */
    MBED_DEPRECATED_SINCE(
        "mbed-os-5.11.0",
        "Deprecated since addition of extended advertising support. "
        "This option is now part of ble::AdvertisingParameters"
    )
    void setAdvertisingInterval(uint16_t interval);

    /**
     * Set the advertising duration.
     *
     * A timeout event is genenerated once the advertising period expired.
     *
     * @param[in] timeout Advertising timeout (in seconds) between 0x1 and 0x3FFF.
     * The special value 0 may be used to disable the advertising timeout.
     *
     * @deprecated Deprecated since addition of extended advertising support.
     * This option is now part of ble::AdvertisingParameters.
     */
    MBED_DEPRECATED_SINCE(
        "mbed-os-5.11.0",
        "Deprecated since addition of extended advertising support. "
        "This option is now part of ble::AdvertisingParameters"
    )
    void setAdvertisingTimeout(uint16_t timeout);

    /**
     * Start the advertising procedure.
     *
     * @return BLE_ERROR_NONE if the device started advertising successfully.
     *
     * @deprecated Deprecated since addition of extended advertising support.
     * Use startAdvertising(advertising_handle_t, adv_duration_t, uint8_t) instead.
     */
    MBED_DEPRECATED_SINCE(
        "mbed-os-5.11.0",
        "Deprecated since addition of extended advertising support. "
        "Use startAdvertising(advertising_handle_t, adv_duration_t, uint8_t) instead."
    )
    ble_error_t startAdvertising(void);

    /**
     * Reset the value of the advertising payload advertised.
     *
     * @deprecated Deprecated since addition of extended advertising support.
     * Use setAdvertisingPayload(ble::advertising_handle_t, Span<uint8_t>,
     * bool).
     */
    MBED_DEPRECATED_SINCE(
        "mbed-os-5.11.0",
        "Deprecated since addition of extended advertising support. "
        "Use setAdvertisingPayload(ble::advertising_handle_t, Span<uint8_t>,"
        "bool)."
    )
    void clearAdvertisingPayload(void);

    /**
     * Set gap flags in the advertising payload.
     *
     * A call to this function is equivalent to:
     *
     * @code
     * Gap &gap;
     *
     * GapAdvertisingData payload = gap.getAdvertisingPayload();
     * payload.addFlags(flags);
     * gap.setAdvertisingPayload(payload);
     * @endcode
     *
     * @param[in] flags The flags to be added.
     *
     * @return BLE_ERROR_NONE if the data was successfully added to the
     * advertising payload.
     *
     * @deprecated Deprecated since addition of extended advertising support.
     * Use ble::AdvertisingDataBuilder.
     */
    MBED_DEPRECATED_SINCE(
        "mbed-os-5.11.0",
        "Deprecated since addition of extended advertising support."
        "Use ble::AdvertisingDataBuilder instead."
    )
    ble_error_t accumulateAdvertisingPayload(uint8_t flags);

    /**
     * Set the appearance field in the advertising payload.
     *
     * A call to this function is equivalent to:
     *
     * @code
     * Gap &gap;
     *
     * GapAdvertisingData payload = gap.getAdvertisingPayload();
     * payload.addAppearance(app);
     * gap.setAdvertisingPayload(payload);
     * @endcode
     *
     * @param[in] app The appearance to advertise.
     *
     * @return BLE_ERROR_NONE if the data was successfully added to the
     * advertising payload.
     *
     * @deprecated Deprecated since addition of extended advertising support.
     * Use ble::AdvertisingDataBuilder instead.
     */
    MBED_DEPRECATED_SINCE(
        "mbed-os-5.11.0",
        "Deprecated since addition of extended advertising support. "
        "Use ble::AdvertisingDataBuilder instead."
    )
    ble_error_t accumulateAdvertisingPayload(GapAdvertisingData::Appearance app);

    /**
     * Set the Tx Power field in the advertising payload.
     *
     * A call to this function is equivalent to:
     *
     * @code
     * Gap &gap;
     *
     * GapAdvertisingData payload = gap.getAdvertisingPayload();
     * payload.addTxPower(power);
     * gap.setAdvertisingPayload(payload);
     * @endcode
     *
     * @param[in] power Transmit power in dBm used by the controller to advertise.
     *
     * @return BLE_ERROR_NONE if the data was successfully added to the
     * advertising payload.
     *
     * @deprecated Deprecated since addition of extended advertising support.
     * Use ble::AdvertisingDataBuilder instead.
     */
    MBED_DEPRECATED_SINCE(
        "mbed-os-5.11.0",
        "Deprecated since addition of extended advertising support. "
        "Use ble::AdvertisingDataBuilder instead."
    )
    ble_error_t accumulateAdvertisingPayloadTxPower(int8_t power);

    /**
     * Add a new field in the advertising payload.
     *
     * A call to this function is equivalent to:
     *
     * @code
     * Gap &gap;
     *
     * GapAdvertisingData payload = gap.getAdvertisingPayload();
     * payload.addData(type, data, len);
     * gap.setAdvertisingPayload(payload);
     * @endcode
     *
     * @param[in] type Identity of the field being added.
     * @param[in] data Buffer containing the value of the field.
     * @param[in] len Length of the data buffer.
     *
     * @return BLE_ERROR_NONE if the advertisement payload was updated based on
     * matching AD type; otherwise, an appropriate error.
     *
     * @note When the specified AD type is INCOMPLETE_LIST_16BIT_SERVICE_IDS,
     * COMPLETE_LIST_16BIT_SERVICE_IDS, INCOMPLETE_LIST_32BIT_SERVICE_IDS,
     * COMPLETE_LIST_32BIT_SERVICE_IDS, INCOMPLETE_LIST_128BIT_SERVICE_IDS,
     * COMPLETE_LIST_128BIT_SERVICE_IDS or LIST_128BIT_SOLICITATION_IDS the
     * supplied value is appended to the values previously added to the payload.
     *
     * @deprecated Deprecated since addition of extended advertising support.
     * Use ble::AdvertisingDataBuilder instead.
     */
    MBED_DEPRECATED_SINCE(
        "mbed-os-5.11.0",
        "Deprecated since addition of extended advertising support. "
        "Use ble::AdvertisingDataBuilder instead."
    )
    ble_error_t accumulateAdvertisingPayload(
        GapAdvertisingData::DataType type, const uint8_t *data, uint8_t len
    );

    /**
     * Update a particular field in the advertising payload.
     *
     * A call to this function is equivalent to:
     *
     * @code
     * Gap &gap;
     *
     * GapAdvertisingData payload = gap.getAdvertisingPayload();
     * payload.updateData(type, data, len);
     * gap.setAdvertisingPayload(payload);
     * @endcode
     *
     *
     * @param[in] type Id of the field to update.
     * @param[in] data data buffer containing the new value of the field.
     * @param[in] len Length of the data buffer.
     *
     * @note If advertisements are enabled, then the update takes effect
     * immediately.
     *
     * @return BLE_ERROR_NONE if the advertisement payload was updated based on
     * matching AD type; otherwise, an appropriate error.
     *
     * @deprecated Deprecated since addition of extended advertising support.
     * Use ble::AdvertisingDataBuilder instead.
     */
    MBED_DEPRECATED_SINCE(
        "mbed-os-5.11.0",
        "Deprecated since addition of extended advertising support. "
        " Use ble::AdvertisingDataBuilder instead."
    )
    ble_error_t updateAdvertisingPayload(
        GapAdvertisingData::DataType type, const uint8_t *data, uint8_t len
    );

    /**
     * Set the value of the payload advertised.
     *
     * @param[in] payload A reference to a user constructed advertisement
     * payload to set.
     *
     * @return BLE_ERROR_NONE if the advertisement payload was successfully
     * set.
     *
     * @deprecated Deprecated since addition of extended advertising support.
     * Use ble::AdvertisingDataBuilder instead.
     */
    MBED_DEPRECATED_SINCE(
        "mbed-os-5.11.0",
        "Deprecated since addition of extended advertising support. "
        "Use ble::AdvertisingDataBuilder instead."
    )
    ble_error_t setAdvertisingPayload(const GapAdvertisingData &payload);

    /**
     * Get a reference to the current advertising payload.
     *
     * @return A reference to the current advertising payload.
     *
     * @deprecated Deprecated since addition of extended advertising support.
     */
    MBED_DEPRECATED_SINCE(
        "mbed-os-5.11.0",
        "Deprecated since addition of extended advertising support."
    )
    const GapAdvertisingData &getAdvertisingPayload(void) const;

    /**
     * Add a new field in the advertising payload.
     *
     * @param[in] type AD type identifier.
     * @param[in] data buffer containing AD data.
     * @param[in] len Length of the data buffer.
     *
     * @return BLE_ERROR_NONE if the data was successfully added to the scan
     * response payload.
     *
     * @deprecated Deprecated since addition of extended advertising support.
     * Use createAdvertisingSet().
     */
    MBED_DEPRECATED_SINCE(
        "mbed-os-5.11.0",
        "Deprecated since addition of extended advertising support."
        "Use ble::AdvertisingDataBuilder instead."
    )
    ble_error_t accumulateScanResponse(
        GapAdvertisingData::DataType type, const uint8_t *data, uint8_t len
    );

    /**
     * Reset the content of the scan response.
     *
     * @note This should be followed by a call to Gap::setAdvertisingPayload()
     * or Gap::startAdvertising() before the update takes effect.
     *
     * @deprecated Deprecated since addition of extended advertising support.
     * Use setAdvertisingScanResponse().
     */
    MBED_DEPRECATED_SINCE(
        "mbed-os-5.11.0",
        "Deprecated since addition of extended advertising support. "
        "Use setAdvertisingScanResponse() instead."
    )
    void clearScanResponse(void);
#endif // BLE_ROLE_BROADCASTER
#if BLE_ROLE_OBSERVER
    /**
     * Set the parameters used during a scan procedure.
     *
     * @param[in] interval in ms between the start of two consecutive scan windows.
     * That value is greater or equal to the scan window value. The
     * maximum allowed value is 10.24ms.
     *
     * @param[in] window Period in ms during which the scanner listens to
     * advertising channels. That value is in the range 2.5ms to 10.24s.
     *
     * @param[in] timeout Duration in seconds of the scan procedure if any. The
     * special value 0 disable specific duration of the scan procedure.
     *
     * @param[in] activeScanning If set to true, then the scanner sends scan
     * requests to a scannable or connectable advertiser. If set to false, then the
     * scanner does not send any request during the scan procedure.
     *
     * @return BLE_ERROR_NONE if the scan parameters were correctly set.
     *
     * @note The scanning window divided by the interval determines the duty
     * cycle for scanning. For example, if the interval is 100ms and the window
     * is 10ms, then the controller scans for 10 percent of the time.
     *
     * @note If the interval and the window are set to the same value, then the
     * device scans continuously during the scan procedure. The scanning
     * frequency changes at every interval.
     *
     * @note Once the scanning parameters have been configured, scanning can be
     * enabled by using startScan().
     *
     * @note The scan interval and window are recommendations to the BLE stack.
     *
     * @deprecated Deprecated since addition of extended advertising support.
     * Use setScanParameters(const ScanParameters &) instead.
     */
    MBED_DEPRECATED_SINCE(
        "mbed-os-5.11.0",
        "Deprecated since addition of extended advertising support. "
        "Use setScanParameters(const ScanParameters &) instead."
    )
    ble_error_t setScanParams(
        uint16_t interval = GapScanningParams::SCAN_INTERVAL_MAX,
        uint16_t window = GapScanningParams::SCAN_WINDOW_MAX,
        uint16_t timeout = 0,
        bool activeScanning = false
    );

    /**
     * Set the parameters used during a scan procedure.
     *
     * @param[in] scanningParams Parameter struct containing the interval, period,
     * timeout and active scanning toggle.
     *
     * @return BLE_ERROR_NONE if the scan parameters were correctly set.
     *
     * @note All restrictions from setScanParams(uint16_t, uint16_t, uint16_t, bool) apply.
     *
     * @deprecated Deprecated since addition of extended advertising support.
     * Use setScanParameters(const ScanParameters &) instead.
     */
    MBED_DEPRECATED_SINCE(
        "mbed-os-5.11.0",
        "Deprecated since addition of extended advertising support. "
        "Use setScanParameters(const ScanParameters &) instead."
    )
    ble_error_t setScanParams(const GapScanningParams &scanningParams);

    /**
     * Set the interval parameter used during scanning procedures.
     *
     * @param[in] interval Interval in ms between the start of two consecutive
     * scan windows. That value is greater or equal to the scan window value.
     * The maximum allowed value is 10.24ms.
     *
     * @return BLE_ERROR_NONE if the scan interval was correctly set.
     *
     * @deprecated Deprecated since addition of extended advertising support.
     * Use setScanParameters(const ScanParameters &) instead.
     */
    MBED_DEPRECATED_SINCE(
        "mbed-os-5.11.0",
        "Deprecated since addition of extended advertising support. "
        "Use setScanParameters(const ScanParameters &) instead."
    )
    ble_error_t setScanInterval(uint16_t interval);

    /**
     * Set the window parameter used during scanning procedures.
     *
     * @param[in] window Period in ms during which the scanner listens to
     * advertising channels. That value is in the range 2.5ms to 10.24s.
     *
     * @return BLE_ERROR_NONE if the scan window was correctly set.
     *
     * @note If scanning is already active, the updated value of scanWindow
     * is propagated to the underlying BLE stack.
     *
     * @deprecated Deprecated since addition of extended advertising support.
     * Use setScanParameters(const ScanParameters &) instead.
     */
    MBED_DEPRECATED_SINCE(
        "mbed-os-5.11.0",
        "Deprecated since addition of extended advertising support. "
        "Use setScanParameters(const ScanParameters &) instead."
    )
    ble_error_t setScanWindow(uint16_t window);

    /**
     * Set the timeout parameter used during scanning procedures.
     *
     * @param[in] timeout Duration in seconds of the scan procedure if any. The
     * special value 0 disables specific duration of the scan procedure.
     *
     * @return BLE_ERROR_NONE if the scan timeout was correctly set.
     *
     * @note If scanning is already active, the updated value of scanTimeout
     * is propagated to the underlying BLE stack.
     *
     * @deprecated Deprecated since addition of extended advertising support.
     * Use setScanParameters(const ScanParameters &) instead.
     */
    MBED_DEPRECATED_SINCE(
        "mbed-os-5.11.0",
        "Deprecated since addition of extended advertising support. "
        "Use setScanParameters(const ScanParameters &) instead."
    )
    ble_error_t setScanTimeout(uint16_t timeout);

    /**
     * Enable or disable active scanning.
     *
     * @param[in] activeScanning If set to true, then the scanner sends scan
     * requests to a scannable or connectable advertiser. If set to false then the
     * scanner does not send any request during the scan procedure.
     *
     * @return BLE_ERROR_NONE if active scanning was successfully set.
     *
     * @note If scanning is already in progress, then active scanning is
     * enabled for the underlying BLE stack.
     *
     * @deprecated Deprecated since addition of extended advertising support.
     * Use setScanParameters(const ScanParameters &) instead.
     */
    MBED_DEPRECATED_SINCE(
        "mbed-os-5.11.0",
        "Deprecated since addition of extended advertising support. "
        "Use setScanParameters(const ScanParameters &) instead."
    )
    ble_error_t setActiveScanning(bool activeScanning);

    /**
     * Start the scanning procedure.
     *
     * Packets received during the scan procedure are forwarded to the
     * scan packet handler passed as argument to this function.
     *
     * @param[in] callback Advertisement packet event handler. Upon reception
     * of an advertising packet, the packet is forwarded to @p callback.
     *
     * @return BLE_ERROR_NONE if the device successfully started the scan
     *         procedure.
     *
     * @note The parameters used by the procedure are defined by setScanParams().
     *
     * @deprecated Deprecated since addition of extended advertising support.
     * Use startScan(duplicates_filter_t, scan_duration_t, scan_period_t) instead.
     */
    MBED_DEPRECATED_SINCE(
        "mbed-os-5.11.0",
        "Deprecated since addition of extended advertising support. "
        "Use startScan(duplicates_filter_t, scan_duration_t, scan_period_t) instead."
    )
    ble_error_t startScan(
        void (*callback)(const AdvertisementCallbackParams_t *params)
    );

    /**
     * Start the scanning procedure.
     *
     * Packets received during the scan procedure are forwarded to the
     * scan packet handler passed as argument to this function.
     *
     * @param[in] object Instance used to invoke @p callbackMember.
     *
     * @param[in] callbackMember Advertisement packet event handler. Upon
     * reception of an advertising packet, the packet is forwarded to @p
     * callback invoked from @p object.
     *
     * @return BLE_ERROR_NONE if the device successfully started the scan
     * procedure.
     *
     * @note The parameters used by the procedure are defined by setScanParams().
     *
     * @deprecated Deprecated since addition of extended advertising support.
     * Use createAdvertisingSet().
     *
     * @deprecated Deprecated since addition of extended advertising support.
     * Use startScan(duplicates_filter_t, scan_duration_t, scan_period_t) instead.
     */
    template<typename T>
    MBED_DEPRECATED_SINCE(
        "mbed-os-5.11.0",
        "Deprecated since addition of extended advertising support. "
        "Use startScan(duplicates_filter_t, scan_duration_t, scan_period_t) instead."
    )
    ble_error_t startScan(
        T *object,
        void (T::*callbackMember)(const AdvertisementCallbackParams_t *params)
    );
#endif // BLE_ROLE_OBSERVER

    /**
     * Enable radio-notification events.
     *
     * Radio Notification is a feature that notifies the application when the
     * radio is in use.
     *
     * The ACTIVE signal is sent before the radio event starts. The nACTIVE
     * signal is sent at the end of the radio event. The application programmer can
     * use these signals to synchronize application logic with radio
     * activity. For example, the ACTIVE signal can be used to shut off external
     * devices, to manage peak current drawn during periods when the radio is on
     * or to trigger sensor data collection for transmission in the Radio Event.
     *
     * @return BLE_ERROR_NONE on successful initialization, otherwise an error code.
     *
     * @deprecated Deprecated since addition of extended advertising support.
     */
    MBED_DEPRECATED_SINCE(
        "mbed-os-5.11.0",
        "Deprecated since addition of extended advertising support. "
    )
    ble_error_t initRadioNotification(void);

private:
#if BLE_ROLE_BROADCASTER
    /**
     * Set the advertising data and scan response in the vendor subsytem.
     *
     * @param[in] advData Advertising data to set.
     * @param[in] scanResponse Scan response to set.
     *
     * @return BLE_ERROR_NONE if the advertising data was set successfully.
     *
     * @note Must be implemented in vendor port.
     *
     * @deprecated Deprecated since addition of extended advertising support.
     * Implement setAdvertisingPayload() and setAdvertisingScanResponse() instead.
     */
    MBED_DEPRECATED_SINCE(
        "mbed-os-5.11.0",
        "Deprecated since addition of extended advertising support. "
        "Implement setAdvertisingPayload() and setAdvertisingScanResponse() instead."
    )
    ble_error_t setAdvertisingData(
        const GapAdvertisingData &advData,
        const GapAdvertisingData &scanResponse
    );

    /**
     * Start the advertising procedure.
     *
     * @param[in] params Advertising parameters to use.
     *
     * @return BLE_ERROR_NONE if the advertising procedure successfully
     * started.
     *
     * @note Must be implemented in vendor port.
     *
     * @deprecated Deprecated since addition of extended advertising support.
     * Implement startAdvertising(advertising_handle_t, adv_duration_t, uint8_t)
     * instead.
     */
    MBED_DEPRECATED_SINCE(
        "mbed-os-5.11.0",
        "Deprecated since addition of extended advertising support. "
        "Implement startAdvertising(advertising_handle_t, adv_duration_t, uint8_t)"
        "instead."
    )
    ble_error_t startAdvertising(const GapAdvertisingParams &params);

public:
    /**
     * Get the current advertising parameters.
     *
     * @return A reference to the current advertising parameters.
     *
     * @deprecated Deprecated since addition of extended advertising support.
     */
    MBED_DEPRECATED_SINCE(
        "mbed-os-5.11.0",
        "Deprecated since addition of extended advertising support."
    )
    GapAdvertisingParams &getAdvertisingParams(void);

    /**
     * Const alternative to Gap::getAdvertisingParams().
     *
     * @return A const reference to the current advertising parameters.
     *
     * @deprecated Deprecated since addition of extended advertising support.
     */
    MBED_DEPRECATED_SINCE(
        "mbed-os-5.11.0",
        "Deprecated since addition of extended advertising support."
    )
    const GapAdvertisingParams &getAdvertisingParams(void) const;

    /**
     * Set the advertising parameters.
     *
     * @param[in] newParams The new advertising parameters.
     *
     * @deprecated Deprecated since addition of extended advertising support.
     * Use setAdvertisingParameters() instead.
     */
    MBED_DEPRECATED_SINCE(
        "mbed-os-5.11.0",
        "Deprecated since addition of extended advertising support."
        "Use setAdvertisingParameters() instead."
    )
    void setAdvertisingParams(const GapAdvertisingParams &newParams);
#endif // BLE_ROLE_BROADCASTER
    /* Event handlers. */
public:

    /**
     * Register a callback handling timeout events.
     *
     * @param[in] callback Event handler being registered.
     *
     * @note A callback may be unregistered using onTimeout().detach(callback).
     *
     * @see TimeoutSource_t
     *
     * @deprecated Deprecated since addition of extended advertising support.
     * Use setEventHandler() instead.
     */
    MBED_DEPRECATED_SINCE(
        "mbed-os-5.11.0",
        "Deprecated since addition of extended advertising support. "
        "Use setEventHandler() instead."
    )
    void onTimeout(TimeoutEventCallback_t callback);

    /**
     * Get the callchain of registered timeout event handlers.
     *
     * @note To register callbacks, use onTimeout().add(callback).
     *
     * @note To unregister callbacks, use onTimeout().detach(callback).
     *
     * @return A reference to the timeout event callbacks chain.
     *
     * @deprecated Deprecated since addition of extended advertising support.
     * Use setEventHandler() instead.
     */
    MBED_DEPRECATED_SINCE(
        "mbed-os-5.11.0",
        "Deprecated since addition of extended advertising support. "
        "Use setEventHandler() instead."
    )
    TimeoutEventCallbackChain_t &onTimeout();
#if BLE_FEATURE_CONNECTABLE
    /**
     * Register a callback handling connection events.
     *
     * @param[in] callback Event handler being registered.
     *
     * @note A callback may be unregistered using onConnection().detach(callback).
     *
     * @deprecated Deprecated since addition of extended advertising support.
     * Use setEventHandler() instead.
     */
    MBED_DEPRECATED_SINCE(
        "mbed-os-5.11.0",
        "Deprecated since addition of extended advertising support. "
        "Use setEventHandler() instead."
    )
    void onConnection(ConnectionEventCallback_t callback);

    /**
     * Register a callback handling connection events.
     *
     * @param[in] tptr Instance used to invoke @p mptr.
     * @param[in] mptr Event handler being registered.
     *
     * @note A callback may be unregistered using onConnection().detach(callback).
     *
     * @deprecated Deprecated since addition of extended advertising support.
     * Use setEventHandler() instead.
     */
    template<typename T>
    MBED_DEPRECATED_SINCE(
        "mbed-os-5.11.0",
        "Deprecated since addition of extended advertising support. "
        "Use setEventHandler() instead."
    )
    void onConnection(T *tptr, void (T::*mptr)(const ConnectionCallbackParams_t *));

    /**
     * Get the callchain of registered connection event handlers.
     *
     * @note To register callbacks, use onConnection().add(callback).
     *
     * @note To unregister callbacks, use onConnection().detach(callback).
     *
     * @return A reference to the connection event callbacks chain.
     *
     * @deprecated Deprecated since addition of extended advertising support.
     * Use setEventHandler() instead.
     */
    MBED_DEPRECATED_SINCE(
        "mbed-os-5.11.0",
        "Deprecated since addition of extended advertising support. "
        "Use setEventHandler() instead."
    )
    ConnectionEventCallbackChain_t &onConnection();

    /**
     * Register a callback handling disconnection events.
     *
     * @param[in] callback Event handler being registered.
     *
     * @note A callback may be unregistered using onDisconnection().detach(callback).
     *
     * @deprecated Deprecated since addition of extended advertising support.
     * Use setEventHandler() instead.
     */
    MBED_DEPRECATED_SINCE(
        "mbed-os-5.11.0",
        "Deprecated since addition of extended advertising support. "
        "Use setEventHandler() instead."
    )
    void onDisconnection(DisconnectionEventCallback_t callback);

    /**
     * Register a callback handling disconnection events.
     *
     * @param[in] tptr Instance used to invoke mptr.
     * @param[in] mptr Event handler being registered.
     *
     * @note A callback may be unregistered using onDisconnection().detach(callback).
     *
     * @deprecated Deprecated since addition of extended advertising support.
     * Use setEventHandler() instead.
     */
    template<typename T>
    MBED_DEPRECATED_SINCE(
        "mbed-os-5.11.0",
        "Deprecated since addition of extended advertising support. "
        "Use setEventHandler() instead."
    )
    void onDisconnection(T *tptr, void (T::*mptr)(const DisconnectionCallbackParams_t *));

    /**
     * Get the callchain of registered disconnection event handlers.
     *
     * @note To register callbacks use onDisconnection().add(callback).
     *
     * @note To unregister callbacks use onDisconnection().detach(callback).
     *
     * @return A reference to the disconnection event callbacks chain.
     *
     * @deprecated Deprecated since addition of extended advertising support.
     * Use setEventHandler() instead.
     */
    MBED_DEPRECATED_SINCE(
        "mbed-os-5.11.0",
        "Deprecated since addition of extended advertising support. "
        "Use setEventHandler() instead."
    )
    DisconnectionEventCallbackChain_t &onDisconnection();
#endif //BLE_FEATURE_CONNECTABLE
    /**
     * Set the radio-notification events handler.
     *
     * Radio Notification is a feature that enables ACTIVE and INACTIVE
     * (nACTIVE) signals from the stack that notify the application when the
     * radio is in use.
     *
     * The ACTIVE signal is sent before the radio event starts. The nACTIVE
     * signal is sent at the end of the radio event. The application programmer can
     * use these signals to synchronize application logic with radio
     * activity. For example, the ACTIVE signal can be used to shut off external
     * devices, to manage peak current drawn during periods when the radio is on
     * or to trigger sensor data collection for transmission in the Radio Event.
     *
     * @param[in] callback Application handler to be invoked in response to a
     * radio ACTIVE/INACTIVE event.
     *
     * @deprecated Deprecated since addition of extended advertising support.
     */
    MBED_DEPRECATED_SINCE(
        "mbed-os-5.11.0",
        "Deprecated since addition of extended advertising support. "
    )
    void onRadioNotification(void (*callback)(bool param));

    /**
     * Set the radio-notification events handler.
     *
     * @param[in] tptr Instance to be used to invoke mptr.
     * @param[in] mptr Application handler to be invoked in response to a
     * radio ACTIVE/INACTIVE event.
     *
     * @deprecated Deprecated since addition of extended advertising support.
     * Use setEventHandler() instead.
     */
    template<typename T>
    MBED_DEPRECATED_SINCE(
        "mbed-os-5.11.0",
        "Deprecated since addition of extended advertising support. "
        "Use setEventHandler() instead."
    )
    void onRadioNotification(T *tptr, void (T::*mptr)(bool));

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
     * @param[in] ownAddr Address this device uses for this connection.
     * @param[in] connectionParams Parameters of the connection.
     * @param[in] peerResolvableAddr Resolvable address used by the peer.
     * @param[in] localResolvableAddr resolvable address used by the local device.
     *
     * @deprecated The type BLEProtocol::AddressType_t is not suitable when
     * privacy is enabled. Use the overload that accepts a PeerAddressType_t
     * instead.
     */
    MBED_DEPRECATED_SINCE(
        "mbed-os-5.9.0",
        "The type BLEProtocol::AddressType_t is not suitable when privacy is "
        "enabled. Use the overload that accepts a PeerAddressType_t instead."
    )
    void processConnectionEvent(
        Handle_t handle,
        Role_t role,
        BLEProtocol::AddressType_t peerAddrType,
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
     * @param[in] addressType Type of the address of the peer that has emitted the packet.
     *
     * @deprecated The type BLEProtocol::AddressType_t is not suitable when
     * privacy is enabled. Use the overload that accepts a PeerAddressType_t
     * instead.
     */
    MBED_DEPRECATED_SINCE(
        "mbed-os-5.9.0",
        "The type BLEProtocol::AddressType_t is not suitable when privacy is "
        "enabled. Use the overload that accepts a PeerAddressType_t instead."
    )
    void processAdvertisementReport(
        const BLEProtocol::AddressBytes_t peerAddr,
        int8_t rssi,
        bool isScanResponse,
        GapAdvertisingParams::AdvertisingType_t type,
        uint8_t advertisingDataLen,
        const uint8_t *advertisingData,
        BLEProtocol::AddressType_t addressType = BLEProtocol::AddressType::RANDOM_STATIC
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

    uint8_t getMaxWhitelistSize_(void) const;

    ble_error_t getWhitelist_(Whitelist_t &whitelist) const;

    ble_error_t setWhitelist_(const Whitelist_t &whitelist);

    ble_error_t setAddress_(
        BLEProtocol::AddressType_t type,
        const BLEProtocol::AddressBytes_t address
    );

    ble_error_t getAddress_(
        BLEProtocol::AddressType_t *typeP,
        BLEProtocol::AddressBytes_t address
    );

    ble_error_t stopAdvertising_(void);

    ble_error_t connect_(
        const BLEProtocol::AddressBytes_t peerAddr,
        PeerAddressType_t peerAddrType,
        const ConnectionParams_t *connectionParams,
        const GapScanningParams *scanParams
    );

    ble_error_t connect_(
        const BLEProtocol::AddressBytes_t peerAddr,
        BLEProtocol::AddressType_t peerAddrType,
        const ConnectionParams_t *connectionParams,
        const GapScanningParams *scanParams
    );

    ble_error_t disconnect_(
        Handle_t connectionHandle, DisconnectionReason_t reason
    );

    ble_error_t disconnect_(DisconnectionReason_t reason);

    ble_error_t updateConnectionParams_(
        Handle_t handle,
        const ConnectionParams_t *params
    );

    ble_error_t setTxPower_(int8_t txPower);

    void getPermittedTxPowerValues_(
        const int8_t **valueArrayPP, size_t *countP
    );

    ble_error_t setAdvertisingPolicyMode_(AdvertisingPolicyMode_t mode);

    ble_error_t setScanningPolicyMode_(ScanningPolicyMode_t mode);

    ble_error_t setInitiatorPolicyMode_(InitiatorPolicyMode_t mode);

    AdvertisingPolicyMode_t getAdvertisingPolicyMode_(void) const;

    ScanningPolicyMode_t getScanningPolicyMode_(void) const;

    InitiatorPolicyMode_t getInitiatorPolicyMode_(void) const;

    ble_error_t startRadioScan_(const GapScanningParams &scanningParams);

    ble_error_t initRadioNotification_(void);

    ble_error_t getPreferredConnectionParams_(ConnectionParams_t *params);

    ble_error_t setPreferredConnectionParams_(
        const ConnectionParams_t *params
    );

    ble_error_t setDeviceName_(const uint8_t *deviceName);

    ble_error_t getDeviceName_(uint8_t *deviceName, unsigned *lengthP);

    ble_error_t setAppearance_(GapAdvertisingData::Appearance appearance);

    ble_error_t getAppearance_(GapAdvertisingData::Appearance *appearanceP);

    ble_error_t setAdvertisingData_(
        const GapAdvertisingData &advData,
        const GapAdvertisingData &scanResponse
    );

    ble_error_t startAdvertising_(const GapAdvertisingParams &params);
};

/**
 * @}
 * @}
 */

/* -------- deprecated template implementation -------- */

#if defined(__GNUC__)
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wdeprecated-declarations"
#endif

#if BLE_ROLE_OBSERVER
template<class Impl>
template<typename T>
ble_error_t LegacyGap<Impl>::startScan(
    T *object,
    void (T::*callbackMember)(const AdvertisementCallbackParams_t *params)
)
{
    ble_error_t err = BLE_ERROR_NONE;
    if (object && callbackMember) {
        if ((err = startRadioScan(_scanningParams)) == BLE_ERROR_NONE) {
            scanningActive = true;
            onAdvertisementReport.attach(object, callbackMember);
        }
    }

    return err;
}
#endif // BLE_ROLE_OBSERVER

#if BLE_FEATURE_CONNECTABLE
template<class Impl>
template<typename T>
void LegacyGap<Impl>::onConnection(T *tptr, void (T::*mptr)(const ConnectionCallbackParams_t *))
{
    connectionCallChain.add(tptr, mptr);
}

template<class Impl>
template<typename T>
void LegacyGap<Impl>::onDisconnection(T *tptr, void (T::*mptr)(const DisconnectionCallbackParams_t *))
{
    disconnectionCallChain.add(tptr, mptr);
}
#endif //BLE_FEATURE_CONNECTABLE

template<class Impl>
template<typename T>
void LegacyGap<Impl>::onRadioNotification(T *tptr, void (T::*mptr)(bool))
{
    radioNotificationCallback.attach(tptr, mptr);
}

#if defined(__GNUC__)
#pragma GCC diagnostic pop
#endif

} // interface
} // ble

// import LegacyGap implementation into global namespace
typedef ble::impl::LegacyGap Gap;

// import Gap implementation into ble namespace
namespace ble {
typedef impl::Gap Gap;
}


#endif // ifndef MBED_BLE_GAP_H__
