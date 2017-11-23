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

#include "BLETypes.h"
#include "BLEProtocol.h"
#include "GapAdvertisingData.h"
#include "GapAdvertisingParams.h"
#include "GapScanningParams.h"
#include "GapEvents.h"
#include "CallChainOfFunctionPointersWithContext.h"
#include "FunctionPointerWithContext.h"
#include "platform/mbed_toolchain.h"

/* Forward declarations for classes that are only used for pointers or
   references. */
class GapAdvertisingParams;
class GapScanningParams;
class GapAdvertisingData;

/**
 * @addtogroup ble
 * @{
 * @addtogroup gap
 * @{
 */

/**
 * Define device discovery, connection and link management procedures.
 *
 * - Device discovery: A device can advertise nearby peers of its existence,
 * identity and capabilities. Similarly, a device can scan its environment to
 * find advertising peers. The information acquired during the scan helps to
 * identify peers and understand their use. A scanner may acquire more information
 * about an advertising peer by sending a scan request. If the peer accepts scan
 * requests, it may reply with additional information about its state.
 *
 * - Connection: A bluetooth device can establish a connection to a connectable
 * advertising peer. Once the connection is established, both devices can
 * communicate using the GATT protocol. The GATT protocol allows connected
 * devices to expose a set of states that the other peer can discover, read and write.
 *
 * - Link Management: Connected devices may drop the connection and may adjust
 * connection parameters according to the power envelop needed for their
 * application.
 *
 * @par Accessing gap
 *
 * Instance of a Gap class for a given BLE device should be accessed using
 * BLE::gap(). The reference returned remains valid until the BLE instance
 * shut down (see BLE::shutdown()).
 *
 * @code
 * // assuming ble_device has been initialized
 * BLE& ble_device;
 *
 * Gap& gap = ble_device.gap();
 * @endcode
 *
 * @par Advertising
 *
 * Advertising consists of broadcasting at a regular interval a small amount of
 * data containing valuable informations about the device. These packets may be
 * scanned by peer devices listening on BLE advertising channels.
 *
 * Scanners may also request additional information from a device advertising by
 * sending a scan request. If the broadcaster accepts scan requests, it can reply
 * with a scan response packet containing additional information.
 *
 * @code
 * // assuming gap has been initialized
 * Gap& gap;
 *
 * // construct the packet to advertise
 * GapAdvertisingData advertising_data;
 *
 * // Add advertiser flags
 * advertising_data.addFlags(
 *    GapAdvertisingData::LE_GENERAL_DISCOVERABLE |
 *    GapAdvertisingData::BREDR_NOT_SUPPORTED
 * );
 *
 * // Add the name of the device to the advertising data
 * static const uint8_t device_name[] = "HRM";
 * advertising_data.addData(
 *     GapAdvertisingData::COMPLETE_LOCAL_NAME,
 *     device_name,
 *     sizeof(device_name)
 * );
 *
 * // set the advertising data in the gap instance, they will be used when
 * // advertising starts.
 * gap.setAdvertisingPayload(advertising_data);
 *
 * // Configure the advertising procedure
 * GapAdvertisingParams advertising_params(
 *     GapAdvertisingParams::ADV_CONNECTABLE_UNDIRECTED, // type of advertising
 *     GapAdvertisingParams::MSEC_TO_ADVERTISEMENT_DURATION_UNITS(1000), // interval
 *     0 // The advertising procedure will not timeout
 * );
 *
 * gap.setAdvertisingParams(advertising_params);
 *
 * // start the advertising procedure, the device will advertise its flag and the
 * // name "HRM". Other peers will also be allowed to connect to it.
 * gap.startAdvertising();
 * @endcode
 *
 * @par Scanning
 *
 * Scanning consist of listening for peer advertising packets. From a scan, a
 * device can identify devices available in its environment.
 *
 * If the device scans actively, then it will send scan request to scannable
 * advertisers and collect their scan response.
 *
 * @code
 * // assuming gap has been initialized
 * Gap& gap;
 *
 * // Handle advertising packet by dumping their content
 * void handle_advertising_packet(const AdvertisementCallbackParams_t* packet)
 * {
 *    printf("Packet received: \r\n");
 *    printf("  - peer address: %02X:%02X:%02X:%02X:%02X:%02X\r\n",
 *           packet->peerAddr[5], packet->peerAddr[4], packet->peerAddr[3],
 *           packet->peerAddr[2], packet->peerAddr[1], packet->peerAddr[0]);
 *    printf("  - rssi: %d", packet->rssi);
 *    printf("  - scan response: %s\r\n", packet->isScanresponse ? "true" : "false");
 *    printf("  - advertising type: %d\r\n", packet->type);
 *    printf("  - advertising type: %d\r\n", packet->type);
 *    printf("  - Advertising data: \r\n");
 *
 *    // parse advertising data, it is a succession of AD structures where
 *    // the first byte is the size of the AD structure, the second byte the
 *    // type of the data and remaining bytes are the value.
 *
 *    for (size_t i = 0; i < packet->advertisingDataLen; i += packet->advertisingData[i]) {
 *        printf("    - type: 0X%02X, data: ", packet->advertisingData[i + 1]);
 *        for (size_t j = 0; j < packet->advertisingData[i] - 2; ++j) {
 *            printf("0X%02X ", packet->advertisingData[i + 2 + j]);
 *        }
 *        printf("\r\n");
 *    }
 * }
 *
 * // set the scan parameters
 * gap.setScanParams(
 *      100, // interval between two scan window in ms
 *      50,  // scan window: period during which the device listen for advertising packets.
 *      0,   // the scan process never ends
 *      true // the device sends scan request to scannable peers.
 * );
 *
 * // start the scan procedure
 * gap.startScan(handle_advertising_packet);
 * @endcode
 *
 * @par Connection event handling
 *
 * A peer may connect device advertising connectable packets. The
 * advertising procedure ends as soon as the device is connected.
 *
 * A device accepting a connection request from a peer is named a peripheral,
 * and the device initiating the connection is named a central.
 *
 * Peripheral and central receive a connection event when the connection is
 * effective.
 *
 * @code
 * Gap& gap;
 *
 * // handle connection event
 * void when_connected(const ConnectionCallbackParams_t *connection_event) {
 *    // If this callback is entered, then the connection to a peer is effective.
 * }
 *
 * // register connection event handler, which will be invoked whether the device
 * // acts as a central or a peripheral
 * gap.onConnection(when_connected);
 * @endcode
 *
 * @par Connection initiation
 *
 * Connection is initiated central devices.
 *
 * @code
 * // assuming gap has been initialized
 * Gap& gap;
 *
 * // Handle the connection event
 * void handle_connection(const ConnectionCallbackParams_t* connection_event)
 * {
 *    // event handling
 * }
 *
 * // Handle advertising packet: connect to the first connectable device
 * void handle_advertising_packet(const AdvertisementCallbackParams_t* packet)
 * {
 *    if (packet->type != GapAdvertisingParams::ADV_CONNECTABLE_UNDIRECTED) {
 *       return;
 *    }
 *
 *    // register connection event handler
 *    gap.onConnection(handle_connection);
 *
 *    Gap::ConnectionParams_t connection_parameters = {
 *       50, // min connection interval
 *       100, // max connection interval
 *       0, // slave latency
 *       600 // connection supervision timeout
 *    };
 *
 *    // scan parameter used to find the device to connect to
 *    GapScanningParams scanning_params(
 *      100, // interval
 *      100, // window
 *      0, // timeout
 *      false // active
 *    );
 *
 *    // Initiate the connection procedure
 *    gap.connect(
 *       packet->peerAddr,
 *       BLEProtocol::RANDOM_STATIC,
 *       &connection_parameters,
 *       &scanning_params
 *    );
 * }
 *
 * // set the scan parameters
 * gap.setScanParams(
 *      100, // interval between two scan window in ms
 *      50,  // scan window: period during which the device listen for advertising packets.
 *      0,   // the scan process never ends
 *      true // the device sends scan request to scannable peers.
 * );
 *
 * // start the scan procedure
 * gap.startScan(handle_advertising_packet);
 * @endcode
 *
 * @par disconnection
 *
 * The application code initiates a disconnection when it calls the
 * disconnect(Handle_t, DisconnectionReason_t) function.
 *
 * Disconnection may also be initiated by the remote peer or the local
 * controller/stack. To catch all disconnection events, application code may
 * set up an handler taking care of disconnection events by calling
 * onDisconnection().
 */
class Gap {
    /*
     * DEPRECATION ALERT: all of the APIs in this `public` block are deprecated.
     * They have been relocated to the class BLEProtocol.
     */
public:
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
     * @important There might be a mismatch between the disconnection reason
     * passed to disconnect() and the disconnection event generated locally
     * because the disconnection reason passed to disconnect() is the
     * disconnection reason to be transmitted to the peer.
     */
    enum DisconnectionReason_t {
        /**
         * The connection timed out.
         *
         * @important shall not be used as a reason in disconnect().
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
         * @important shall not be used as a reason in disconnect().
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
     * @important A device can fulfill different roles concurrently.
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
         */
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
         */
        BLEProtocol::AddressBytes_t ownAddr;

        /**
         * Connection parameters.
         */
        const ConnectionParams_t   *connectionParams;

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
         *
         * @note Constructor is not meant to be called by user code.
         * The BLE API vendor code generates ConnectionCallbackParams_t.
         */
        ConnectionCallbackParams_t(
            Handle_t handleIn,
            Role_t roleIn,
            BLEProtocol::AddressType_t peerAddrTypeIn,
            const uint8_t *peerAddrIn,
            BLEProtocol::AddressType_t ownAddrTypeIn,
            const uint8_t *ownAddrIn,
            const ConnectionParams_t *connectionParamsIn
        ) : handle(handleIn),
            role(roleIn),
            peerAddrType(peerAddrTypeIn),
            peerAddr(),
            ownAddrType(ownAddrTypeIn),
            ownAddr(),
            connectionParams(connectionParamsIn)
        {
            memcpy(peerAddr, peerAddrIn, ADDR_LEN);
            memcpy(ownAddr, ownAddrIn, ADDR_LEN);
        }
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
        {}
    };

    /**
     * Number of microseconds in 1.25 milliseconds.
     */
    static const uint16_t UNIT_1_25_MS  = 1250;

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
    typedef FunctionPointerWithContext<const DisconnectionCallbackParams_t*>
        DisconnectionEventCallback_t;

    /**
     * Callchain of disconnection event handlers.
     *
     * @see Gap::onDisconnection().
     */
    typedef CallChainOfFunctionPointersWithContext<const DisconnectionCallbackParams_t*>
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
    typedef FunctionPointerWithContext<const Gap *> GapShutdownCallback_t;

    /**
     * Callchain of gap shutdown event handler.
     *
     * @see Gap::onShutdown().
     */
    typedef CallChainOfFunctionPointersWithContext<const Gap *>
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
     * @return BLE_ERROR_NONE on success.
     */
    virtual ble_error_t setAddress(
        BLEProtocol::AddressType_t type,
        const BLEProtocol::AddressBytes_t address
    ) {
        /* avoid compiler warnings about unused variables */
        (void)type;
        (void)address;

        /* Requesting action from porter(s): override this API if this capability
           is supported. */
        return BLE_ERROR_NOT_IMPLEMENTED;
    }

    /**
     * Fetch the current address and its type.
     *
     * @param[out] typeP Type of the current address set.
     * @param[out] address Value of the current address.
     *
     * @return BLE_ERROR_NONE on success.
     */
    virtual ble_error_t getAddress(
        BLEProtocol::AddressType_t *typeP,
        BLEProtocol::AddressBytes_t address
    ) {
        /* Avoid compiler warnings about unused variables. */
        (void)typeP;
        (void)address;

        /* Requesting action from porter(s): override this API if this capability
           is supported. */
        return BLE_ERROR_NOT_IMPLEMENTED;
    }

    /**
     * Get the minimum advertising interval in milliseconds, which can be used
     * for connectable advertising types.
     *
     * @return Minimum Advertising interval in milliseconds for connectable
     * undirected and connectable directed advertising types.
     */
    virtual uint16_t getMinAdvertisingInterval(void) const
    {
        /* Requesting action from porter(s): override this API if this capability
           is supported. */
        return 0;
    }

    /**
     * Get the minimum advertising interval in milliseconds, which can be
     * used for nonconnectable advertising type.
     *
     * @return Minimum Advertising interval in milliseconds for scannable
     * undirected and nonconnectable undirected event types.
     */
    virtual uint16_t getMinNonConnectableAdvertisingInterval(void) const
    {
        /* Requesting action from porter(s): override this API if this capability
           is supported. */
        return 0;
    }

    /**
     * Get the maximum advertising interval in milliseconds.
     *
     * @return Maximum Advertising interval in milliseconds.
     */
    virtual uint16_t getMaxAdvertisingInterval(void) const
    {
        /* Requesting action from porter(s): override this API if this capability
           is supported. */
        return 0xFFFF;
    }

    /**
     * Stop the ongoing advertising procedure.
     *
     * @note The current advertising parameters remain in effect.
     *
     * @retval BLE_ERROR_NONE if the advertising procedure has been successfully
     * stopped.
     */
    virtual ble_error_t stopAdvertising(void)
    {
        /* Requesting action from porter(s): override this API if this capability
           is supported. */
        return BLE_ERROR_NOT_IMPLEMENTED;
    }

    /**
     * Stop the ongoing scanning procedure.
     *
     * The current scanning parameters remain in effect.
     *
     * @retval BLE_ERROR_NONE if successfully stopped scanning procedure.
     */
    virtual ble_error_t stopScan()
    {
        /* Requesting action from porter(s): override this API if this capability
           is supported. */
        return BLE_ERROR_NOT_IMPLEMENTED;
    }

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
     * @return BLE_ERROR_NONE if connection establishment procedure is started
     * successfully. The connectionCallChain (if set) is invoked upon
     * a connection event.
     */
    virtual ble_error_t connect(
        const BLEProtocol::AddressBytes_t peerAddr,
        BLEProtocol::AddressType_t peerAddrType,
        const ConnectionParams_t *connectionParams,
        const GapScanningParams *scanParams
    ) {
        /* Avoid compiler warnings about unused variables. */
        (void)peerAddr;
        (void)peerAddrType;
        (void)connectionParams;
        (void)scanParams;

        /* Requesting action from porter(s): override this API if this capability
           is supported. */
        return BLE_ERROR_NOT_IMPLEMENTED;
    }

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
    ) {
        return connect(
            peerAddr,
            (BLEProtocol::AddressType_t)
            peerAddrType,
            connectionParams,
            scanParams
        );
    }

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
     */
    virtual ble_error_t disconnect(
        Handle_t connectionHandle, DisconnectionReason_t reason
    ) {
        /* avoid compiler warnings about unused variables */
        (void)connectionHandle;
        (void)reason;

        /* Requesting action from porter(s): override this API if this capability
           is supported. */
        return BLE_ERROR_NOT_IMPLEMENTED;
    }

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
    virtual ble_error_t disconnect(DisconnectionReason_t reason) {
        /* Avoid compiler warnings about unused variables. */
        (void)reason;

        /* Requesting action from porter(s): override this API if this capability
           is supported. */
        return BLE_ERROR_NOT_IMPLEMENTED;
    }

    /**
     * Returned the preferred connection parameters exposed in the GATT Generic
     * Access Service.
     *
     * @param[out] params Structure where the parameters are stored.
     *
     * @return BLE_ERROR_NONE if the parameters were successfully filled into
     * @p params.
     */
    virtual ble_error_t getPreferredConnectionParams(ConnectionParams_t *params)
    {
        /* Avoid compiler warnings about unused variables. */
        (void)params;

        /* Requesting action from porter(s): override this API if this capability
           is supported. */
        return BLE_ERROR_NOT_IMPLEMENTED;
    }

    /**
     * Set the value of the preferred connection parameters exposed in the GATT
     * Generic Access Service.
     *
     * A connected peer may read the characteristic exposing these parameters
     * and request an update of the connection parameters to accomodate the
     * local device.
     *
     * @param[in] params Value of the preferred connection parameters.
     *
     * @return BLE_ERROR_NONE if the preferred connection params were set
     * correctly.
     */
    virtual ble_error_t setPreferredConnectionParams(
        const ConnectionParams_t *params
    ) {
        /* Avoid compiler warnings about unused variables. */
        (void)params;

        /* Requesting action from porter(s): override this API if this capability
           is supported. */
        return BLE_ERROR_NOT_IMPLEMENTED;
    }

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
     */
    virtual ble_error_t updateConnectionParams(
        Handle_t handle,
        const ConnectionParams_t *params
    ) {
        /* avoid compiler warnings about unused variables */
        (void)handle;
        (void)params;

        /* Requesting action from porter(s): override this API if this capability
           is supported. */
        return BLE_ERROR_NOT_IMPLEMENTED;
    }

    /**
     * Set the value of the device name characteristic in the Generic Access
     * Service.
     *
     * @param[in] deviceName The new value for the device-name. This is a
     * UTF-8 encoded, <b>NULL-terminated</b> string.
     *
     * @return BLE_ERROR_NONE if the device name was set correctly.
     */
    virtual ble_error_t setDeviceName(const uint8_t *deviceName) {
        /* Avoid compiler warnings about unused variables. */
        (void)deviceName;

        /* Requesting action from porter(s): override this API if this capability
           is supported. */
        return BLE_ERROR_NOT_IMPLEMENTED;
    }

    /**
     * Get the value of the device name characteristic in the Generic Access
     * Service.
     *
     * To obtain the length of the deviceName value, this function is
     * invoked with the @p deviceName parameter set to NULL.
     *
     * @param[out] deviceName Pointer to an empty buffer where the UTF-8
     * <b>non NULL-terminated<b> string is placed.
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
    virtual ble_error_t getDeviceName(uint8_t *deviceName, unsigned *lengthP)
    {
        /* avoid compiler warnings about unused variables */
        (void)deviceName;
        (void)lengthP;

        /* Requesting action from porter(s): override this API if this capability
           is supported. */
        return BLE_ERROR_NOT_IMPLEMENTED;
    }

    /**
     * Set the value of the appearance characteristic in the GAP service.
     *
     * @param[in] appearance The new value for the device-appearance.
     *
     * @return BLE_ERROR_NONE if the new appearance was set correctly.
     */
    virtual ble_error_t setAppearance(GapAdvertisingData::Appearance appearance)
    {
        /* Avoid compiler warnings about unused variables. */
        (void)appearance;

        /* Requesting action from porter(s): override this API if this capability
           is supported. */
        return BLE_ERROR_NOT_IMPLEMENTED;
    }

    /**
     * Get the value of the appearance characteristic in the GAP service.
     *
     * @param[out] appearance The current device-appearance value.
     *
     * @return BLE_ERROR_NONE if the device-appearance was fetched correctly
     * from the underlying BLE stack.
     */
    virtual ble_error_t getAppearance(GapAdvertisingData::Appearance *appearanceP)
    {
        /* Avoid compiler warnings about unused variables. */
        (void)appearanceP;

        /* Requesting action from porter(s): override this API if this capability
           is supported. */
        return BLE_ERROR_NOT_IMPLEMENTED;
    }

    /**
     * Set the radio's transmit power.
     *
     * @param[in] txPower Radio's transmit power in dBm.
     *
     * @return BLE_ERROR_NONE if the new radio's transmit power was set
     * correctly.
     */
    virtual ble_error_t setTxPower(int8_t txPower)
    {
        /* Avoid compiler warnings about unused variables. */
        (void)txPower;

        /* Requesting action from porter(s): override this API if this capability
           is supported. */
        return BLE_ERROR_NOT_IMPLEMENTED;
    }

    /**
     * Query the underlying stack for allowed Tx power values.
     *
     * @param[out] valueArrayPP Receive the immutable array of Tx values.
     * @param[out] countP Receive the array's size.
     */
    virtual void getPermittedTxPowerValues(
        const int8_t **valueArrayPP, size_t *countP
    ) {
        /* Avoid compiler warnings about unused variables. */
        (void)valueArrayPP;
        (void)countP;

        /* Requesting action from porter(s): override this API if this capability
           is supported. */
        *countP = 0;
    }

    /**
     * Get the maximum size of the whitelist.
     *
     * @return Maximum size of the whitelist.
     *
     * @note If using Mbed OS, you can configure the size of the whitelist by
     * setting the YOTTA_CFG_WHITELIST_MAX_SIZE macro in your yotta config file.
     */
    virtual uint8_t getMaxWhitelistSize(void) const
    {
        return 0;
    }

    /**
     * Get the Link Layer to use the internal whitelist when scanning,
     * advertising or initiating a connection depending on the filter policies.
     *
     * @param[in,out] whitelist Define the whitelist instance which is used
     * to store the whitelist requested. In input, the caller provisions memory.
     *
     * @return BLE_ERROR_NONE if the implementation's whitelist was successfully
     * copied into the supplied reference.
     *
     * @experimental
     */
    virtual ble_error_t getWhitelist(Whitelist_t &whitelist) const
    {
        (void) whitelist;
        return BLE_ERROR_NOT_IMPLEMENTED;
    }

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
     * BLEProtocol::AddressType_t::RANDOM_PRIVATE_NON_RESOLVABLE. This
     * results in a @ref BLE_ERROR_INVALID_PARAM because the remote peer might
     * change its private address at any time, and it is not possible to resolve
     * it.
     *
     * @note If the input whitelist is larger than @ref getMaxWhitelistSize(),
     * then @ref BLE_ERROR_PARAM_OUT_OF_RANGE is returned.
     */
    virtual ble_error_t setWhitelist(const Whitelist_t &whitelist)
    {
        (void) whitelist;
        return BLE_ERROR_NOT_IMPLEMENTED;
    }

    /**
     * Set the advertising policy filter mode to be used during the next
     * advertising procedure.
     *
     * @param[in] mode New advertising policy filter mode.
     *
     * @return BLE_ERROR_NONE if the specified policy filter mode was set
     * successfully.
     */
    virtual ble_error_t setAdvertisingPolicyMode(AdvertisingPolicyMode_t mode)
    {
        (void) mode;
        return BLE_ERROR_NOT_IMPLEMENTED;
    }

    /**
     * Set the scan policy filter mode to be used during the next scan procedure.
     *
     * @param[in] mode New scan policy filter mode.
     *
     * @return BLE_ERROR_NONE if the specified policy filter mode was set
     * successfully.
     */
    virtual ble_error_t setScanningPolicyMode(ScanningPolicyMode_t mode)
    {
        (void) mode;
        return BLE_ERROR_NOT_IMPLEMENTED;
    }

    /**
     * Set the initiator policy filter mode to be used during the next connection
     * initiation.
     *
     * @param[in] mode New initiator policy filter mode.
     *
     * @return BLE_ERROR_NONE if the specified policy filter mode was set
     * successfully.
     */
    virtual ble_error_t setInitiatorPolicyMode(InitiatorPolicyMode_t mode)
    {
        (void) mode;
        return BLE_ERROR_NOT_IMPLEMENTED;
    }

    /**
     * Get the current advertising policy filter mode.
     *
     * @return The current advertising policy filter mode.
     */
    virtual AdvertisingPolicyMode_t getAdvertisingPolicyMode(void) const
    {
        return ADV_POLICY_IGNORE_WHITELIST;
    }

    /**
     * Get the current scan policy filter mode.
     *
     * @return The current scan policy filter mode.
     */
    virtual ScanningPolicyMode_t getScanningPolicyMode(void) const
    {
        return SCAN_POLICY_IGNORE_WHITELIST;
    }

    /**
     * Get the current initiator policy filter mode.
     *
     * @return The current scan policy filter mode.
     */
    virtual InitiatorPolicyMode_t getInitiatorPolicyMode(void) const
    {
        return INIT_POLICY_IGNORE_WHITELIST;
    }

protected:
    /* Override the following in the underlying adaptation layer to provide the
      functionality of scanning. */

    /**
     * Start scanning procedure in the underlying BLE stack.
     *
     * @param[in] scanningParams Parameters of the scan procedure.
     *
     * @return BLE_ERROR_NONE if the scan procedure was successfully started.
     */
    virtual ble_error_t startRadioScan(const GapScanningParams &scanningParams)
    {
        (void)scanningParams;
        /* Requesting action from porter(s): override this API if this capability
           is supported. */
        return BLE_ERROR_NOT_IMPLEMENTED;
    }

    /*
     * APIs with nonvirtual implementations.
     */
public:
    /**
     * Get the current advertising and connection states of the device.
     *
     * @return The current GAP state of the device.
     */
    GapState_t getState(void) const
    {
        return state;
    }

    /**
     * Set the advertising type to use during the advertising procedure.
     *
     * @param[in] advType New type of advertising to use.
     */
    void setAdvertisingType(GapAdvertisingParams::AdvertisingType_t advType)
    {
        _advParams.setAdvertisingType(advType);
    }

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
     */
    void setAdvertisingInterval(uint16_t interval)
    {
        if (interval == 0) {
            stopAdvertising();
        } else if (interval < getMinAdvertisingInterval()) {
            interval = getMinAdvertisingInterval();
        }
        _advParams.setInterval(interval);
    }

    /**
     * Set the advertising duration.
     *
     * A timeout event is genenerated once the advertising period expired.
     *
     * @param[in] timeout Advertising timeout (in seconds) between 0x1 and 0x3FFF.
     * The special value 0 may be used to disable the advertising timeout.
     */
    void setAdvertisingTimeout(uint16_t timeout)
    {
        _advParams.setTimeout(timeout);
    }

    /**
     * Start the advertising procedure.
     *
     * @return BLE_ERROR_NONE if the device started advertising successfully.
     */
    ble_error_t startAdvertising(void)
    {
        ble_error_t rc;
        if ((rc = startAdvertising(_advParams)) == BLE_ERROR_NONE) {
            state.advertising = 1;
        }
        return rc;
    }

    /**
     * Reset the value of the advertising payload advertised.
     */
    void clearAdvertisingPayload(void)
    {
        _advPayload.clear();
        setAdvertisingData(_advPayload, _scanResponse);
    }

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
     */
    ble_error_t accumulateAdvertisingPayload(uint8_t flags)
    {
        GapAdvertisingData advPayloadCopy = _advPayload;
        ble_error_t rc;
        if ((rc = advPayloadCopy.addFlags(flags)) != BLE_ERROR_NONE) {
            return rc;
        }

        rc = setAdvertisingData(advPayloadCopy, _scanResponse);
        if (rc == BLE_ERROR_NONE) {
            _advPayload = advPayloadCopy;
        }

        return rc;
    }

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
     */
    ble_error_t accumulateAdvertisingPayload(GapAdvertisingData::Appearance app)
    {
        GapAdvertisingData advPayloadCopy = _advPayload;
        ble_error_t rc;
        if ((rc = advPayloadCopy.addAppearance(app)) != BLE_ERROR_NONE) {
            return rc;
        }

        rc = setAdvertisingData(advPayloadCopy, _scanResponse);
        if (rc == BLE_ERROR_NONE) {
            _advPayload = advPayloadCopy;
        }

        return rc;
    }

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
     */
    ble_error_t accumulateAdvertisingPayloadTxPower(int8_t power)
    {
        GapAdvertisingData advPayloadCopy = _advPayload;
        ble_error_t rc;
        if ((rc = advPayloadCopy.addTxPower(power)) != BLE_ERROR_NONE) {
            return rc;
        }

        rc = setAdvertisingData(advPayloadCopy, _scanResponse);
        if (rc == BLE_ERROR_NONE) {
            _advPayload = advPayloadCopy;
        }

        return rc;
    }

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
     */
    ble_error_t accumulateAdvertisingPayload(
        GapAdvertisingData::DataType type, const uint8_t *data, uint8_t len
    ) {
        GapAdvertisingData advPayloadCopy = _advPayload;
        ble_error_t rc;
        if ((rc = advPayloadCopy.addData(type, data, len)) != BLE_ERROR_NONE) {
            return rc;
        }

        rc = setAdvertisingData(advPayloadCopy, _scanResponse);
        if (rc == BLE_ERROR_NONE) {
            _advPayload = advPayloadCopy;
        }

        return rc;
    }

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
     */
    ble_error_t updateAdvertisingPayload(
        GapAdvertisingData::DataType type, const uint8_t *data, uint8_t len
    ) {
        GapAdvertisingData advPayloadCopy = _advPayload;
        ble_error_t rc;
        if ((rc = advPayloadCopy.updateData(type, data, len)) != BLE_ERROR_NONE) {
            return rc;
        }

        rc = setAdvertisingData(advPayloadCopy, _scanResponse);
        if (rc == BLE_ERROR_NONE) {
            _advPayload = advPayloadCopy;
        }

        return rc;
    }

    /**
     * Set the value of the payload advertised.
     *
     * @param[in] payload A reference to a user constructed advertisement
     * payload to set.
     *
     * @return BLE_ERROR_NONE if the advertisement payload was successfully
     * set.
     */
    ble_error_t setAdvertisingPayload(const GapAdvertisingData &payload)
    {
        ble_error_t rc = setAdvertisingData(payload, _scanResponse);
        if (rc == BLE_ERROR_NONE) {
            _advPayload = payload;
        }

        return rc;
    }

    /**
     * Get a reference to the current advertising payload.
     *
     * @return A reference to the current advertising payload.
     */
    const GapAdvertisingData &getAdvertisingPayload(void) const
    {
        return _advPayload;
    }

    /**
     * Add a new field in the advertising payload.
     *
     * @param[in] type AD type identifier.
     * @param[in] data buffer containing AD data.
     * @param[in] len Length of the data buffer.
     *
     * @return BLE_ERROR_NONE if the data was successfully added to the scan
     * response payload.
     */
    ble_error_t accumulateScanResponse(
        GapAdvertisingData::DataType type, const uint8_t *data, uint8_t len
    ) {
        GapAdvertisingData scanResponseCopy = _scanResponse;
        ble_error_t rc;
        if ((rc = scanResponseCopy.addData(type, data, len)) != BLE_ERROR_NONE) {
            return rc;
        }

        rc = setAdvertisingData(_advPayload, scanResponseCopy);
        if (rc == BLE_ERROR_NONE) {
            _scanResponse = scanResponseCopy;
        }

        return rc;
    }

    /**
     * Reset the content of the scan response.
     *
     * @note This should be followed by a call to Gap::setAdvertisingPayload()
     * or Gap::startAdvertising() before the update takes effect.
     */
    void clearScanResponse(void) {
        _scanResponse.clear();
        setAdvertisingData(_advPayload, _scanResponse);
    }

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
     */
    ble_error_t setScanParams(
        uint16_t interval = GapScanningParams::SCAN_INTERVAL_MAX,
        uint16_t window = GapScanningParams::SCAN_WINDOW_MAX,
        uint16_t timeout = 0,
        bool activeScanning = false
    ) {
        ble_error_t rc;
        if (((rc = _scanningParams.setInterval(interval)) == BLE_ERROR_NONE) &&
            ((rc = _scanningParams.setWindow(window))     == BLE_ERROR_NONE) &&
            ((rc = _scanningParams.setTimeout(timeout))   == BLE_ERROR_NONE)) {
            _scanningParams.setActiveScanning(activeScanning);
            return BLE_ERROR_NONE;
        }

        return rc;
    }

    /**
     * Set the interval parameter used during scanning procedures.
     *
     * @param[in] interval Interval in ms between the start of two consecutive
     * scan windows. That value is greater or equal to the scan window value.
     * The maximum allowed value is 10.24ms.
     *
     * @return BLE_ERROR_NONE if the scan interval was correctly set.
     */
    ble_error_t setScanInterval(uint16_t interval)
    {
        return _scanningParams.setInterval(interval);
    }

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
     */
    ble_error_t setScanWindow(uint16_t window)
    {
        ble_error_t rc;
        if ((rc = _scanningParams.setWindow(window)) != BLE_ERROR_NONE) {
            return rc;
        }

        /* If scanning is already active, propagate the new setting to the stack. */
        if (scanningActive) {
            return startRadioScan(_scanningParams);
        }

        return BLE_ERROR_NONE;
    }

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
     */
    ble_error_t setScanTimeout(uint16_t timeout)
    {
        ble_error_t rc;
        if ((rc = _scanningParams.setTimeout(timeout)) != BLE_ERROR_NONE) {
            return rc;
        }

        /* If scanning is already active, propagate the new settings to the stack. */
        if (scanningActive) {
            return startRadioScan(_scanningParams);
        }

        return BLE_ERROR_NONE;
    }

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
     */
    ble_error_t setActiveScanning(bool activeScanning)
    {
        _scanningParams.setActiveScanning(activeScanning);

        /* If scanning is already active, propagate the new settings to the stack. */
        if (scanningActive) {
            return startRadioScan(_scanningParams);
        }

        return BLE_ERROR_NONE;
    }

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
     */
    ble_error_t startScan(
        void (*callback)(const AdvertisementCallbackParams_t *params)
    ) {
        ble_error_t err = BLE_ERROR_NONE;
        if (callback) {
            if ((err = startRadioScan(_scanningParams)) == BLE_ERROR_NONE) {
                scanningActive = true;
                onAdvertisementReport.attach(callback);
            }
        }

        return err;
    }

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
     */
    template<typename T>
    ble_error_t startScan(
        T *object,
        void (T::*callbackMember)(const AdvertisementCallbackParams_t *params)
    ) {
        ble_error_t err = BLE_ERROR_NONE;
        if (object && callbackMember) {
            if ((err = startRadioScan(_scanningParams)) == BLE_ERROR_NONE) {
                scanningActive = true;
                onAdvertisementReport.attach(object, callbackMember);
            }
        }

        return err;
    }

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
     */
    virtual ble_error_t initRadioNotification(void)
    {
        /* Requesting action from porter(s): override this API if this capability
           is supported. */
        return BLE_ERROR_NOT_IMPLEMENTED;
    }

private:
    /**
     * Set the advertising data and scan response in the vendor subsytem.
     *
     * @param[in] advData Advertising data to set.
     * @param[in] scanResponse Scan response to set.
     *
     * @return BLE_ERROR_NONE if the advertising data was set successfully.
     *
     * @note Must be implemented in vendor port.
     */
    virtual ble_error_t setAdvertisingData(
        const GapAdvertisingData &advData,
        const GapAdvertisingData &scanResponse
    ) = 0;

    /**
     * Start the advertising procedure.
     *
     * @param[in] Advertising parameters to use.
     *
     * @return BLE_ERROR_NONE if the advertising procedure successfully
     * started.
     *
     * @note Must be implemented in vendor port.
     */
    virtual ble_error_t startAdvertising(const GapAdvertisingParams &) = 0;

public:
    /**
     * Get the current advertising parameters.
     *
     * @return A reference to the current advertising parameters.
     */
    GapAdvertisingParams &getAdvertisingParams(void)
    {
        return _advParams;
    }

    /**
     * Const alternative to Gap::getAdvertisingParams().
     *
     * @return A const reference to the current advertising parameters.
     */
    const GapAdvertisingParams &getAdvertisingParams(void) const
    {
        return _advParams;
    }

    /**
     * Set the advertising parameters.
     *
     * @param[in] newParams The new advertising parameters.
     */
    void setAdvertisingParams(const GapAdvertisingParams &newParams)
    {
        _advParams = newParams;
    }

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
     */
    void onTimeout(TimeoutEventCallback_t callback)
    {
        timeoutCallbackChain.add(callback);
    }

    /**
     * Get the callchain of registered timeout event handlers.
     *
     * @note To register callbacks, use onTimeout().add(callback).
     *
     * @note To unregister callbacks, use onTimeout().detach(callback).
     *
     * @return A reference to the timeout event callbacks chain.
     */
    TimeoutEventCallbackChain_t& onTimeout()
    {
        return timeoutCallbackChain;
    }

    /**
     * Register a callback handling connection events.
     *
     * @param[in] callback Event handler being registered.
     *
     * @note A callback may be unregistered using onConnection().detach(callback).
     */
    void onConnection(ConnectionEventCallback_t callback)
    {
        connectionCallChain.add(callback);
    }

    /**
     * Register a callback handling connection events.
     *
     * @param[in] tptr Instance used to invoke @p mptr.
     * @param[in] mptr Event handler being registered.
     *
     * @note A callback may be unregistered using onConnection().detach(callback).
     */
    template<typename T>
    void onConnection(T *tptr, void (T::*mptr)(const ConnectionCallbackParams_t*))
    {
        connectionCallChain.add(tptr, mptr);
    }

    /**
     * Get the callchain of registered connection event handlers.
     *
     * @note To register callbacks, use onConnection().add(callback).
     *
     * @note To unregister callbacks, use onConnection().detach(callback).
     *
     * @return A reference to the connection event callbacks chain.
     */
    ConnectionEventCallbackChain_t& onConnection()
    {
        return connectionCallChain;
    }

    /**
     * Register a callback handling disconnection events.
     *
     * @param[in] callback Event handler being registered.
     *
     * @note A callback may be unregistered using onDisconnection().detach(callback).
     */
    void onDisconnection(DisconnectionEventCallback_t callback)
    {
        disconnectionCallChain.add(callback);
    }

    /**
     * Register a callback handling disconnection events.
     *
     * @param[in] tptr Instance used to invoke mptr.
     * @param[in] mptr Event handler being registered.
     *
     * @note A callback may be unregistered using onDisconnection().detach(callback).
     */
    template<typename T>
    void onDisconnection(T *tptr, void (T::*mptr)(const DisconnectionCallbackParams_t*))
    {
        disconnectionCallChain.add(tptr, mptr);
    }

    /**
     * Get the callchain of registered disconnection event handlers.
     *
     * @note To register callbacks use onDisconnection().add(callback).
     *
     * @note To unregister callbacks use onDisconnection().detach(callback).
     *
     * @return A reference to the disconnection event callbacks chain.
     */
    DisconnectionEventCallbackChain_t& onDisconnection()
    {
        return disconnectionCallChain;
    }

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
     */
    void onRadioNotification(void (*callback)(bool param))
    {
        radioNotificationCallback.attach(callback);
    }

    /**
     * Set the radio-notification events handler.
     *
     * @param[in] tptr Instance to be used to invoke mptr.
     * @param[in] mptr Application handler to be invoked in response to a
     * radio ACTIVE/INACTIVE event.
     */
    template <typename T>
    void onRadioNotification(T *tptr, void (T::*mptr)(bool))
    {
        radioNotificationCallback.attach(tptr, mptr);
    }

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
    void onShutdown(const GapShutdownCallback_t& callback)
    {
        shutdownCallChain.add(callback);
    }

    /**
     * Register a Gap shutdown event handler.
     *
     * @param[in] objPtr Instance used to invoke @p memberPtr.
     * @param[in] memberPtr Shutdown event handler to register.
     */
    template <typename T>
    void onShutdown(T *objPtr, void (T::*memberPtr)(const Gap *))
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
    GapShutdownCallbackChain_t& onShutdown()
    {
        return shutdownCallChain;
    }

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
    virtual ble_error_t reset(void)
    {
        /* Notify that the instance is about to shut down */
        shutdownCallChain.call(this);
        shutdownCallChain.clear();

        /* Clear Gap state */
        state.advertising = 0;
        state.connected   = 0;
        connectionCount   = 0;

        /* Clear scanning state */
        scanningActive = false;

        /* Clear advertising and scanning data */
        _advPayload.clear();
        _scanResponse.clear();

        /* Clear callbacks */
        timeoutCallbackChain.clear();
        connectionCallChain.clear();
        disconnectionCallChain.clear();
        radioNotificationCallback = NULL;
        onAdvertisementReport     = NULL;

        return BLE_ERROR_NONE;
    }

protected:
    /**
     * Construct a Gap instance.
     */
    Gap() :
        _advParams(),
        _advPayload(),
        _scanningParams(),
        _scanResponse(),
        connectionCount(0),
        state(),
        scanningActive(false),
        timeoutCallbackChain(),
        radioNotificationCallback(),
        onAdvertisementReport(),
        connectionCallChain(),
        disconnectionCallChain() {
        _advPayload.clear();
        _scanResponse.clear();
    }

    /* Entry points for the underlying stack to report events back to the user. */
public:
    /**
     * Notify all registered connection event handlers of a connection event.
     *
     * @important This function is meant to be called from the BLE stack specific
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
     */
    void processConnectionEvent(
        Handle_t handle,
        Role_t role,
        BLEProtocol::AddressType_t peerAddrType,
        const BLEProtocol::AddressBytes_t peerAddr,
        BLEProtocol::AddressType_t ownAddrType,
        const BLEProtocol::AddressBytes_t ownAddr,
        const ConnectionParams_t *connectionParams
    ) {
        /* Update Gap state */
        state.advertising = 0;
        state.connected   = 1;
        ++connectionCount;

        ConnectionCallbackParams_t callbackParams(
            handle,
            role,
            peerAddrType,
            peerAddr,
            ownAddrType,
            ownAddr,
            connectionParams
        );
        connectionCallChain.call(&callbackParams);
    }

    /**
     * Notify all registered disconnection event handlers of a disconnection event.
     *
     * @important This function is meant to be called from the BLE stack specific
     * implementation when a disconnection event occurs.
     *
     * @param[in] handle Handle of the terminated connection.
     * @param[in] reason Reason of the disconnection.
     */
    void processDisconnectionEvent(Handle_t handle, DisconnectionReason_t reason)
    {
        /* Update Gap state */
        --connectionCount;
        if (!connectionCount) {
            state.connected = 0;
        }

        DisconnectionCallbackParams_t callbackParams(handle, reason);
        disconnectionCallChain.call(&callbackParams);
    }

    /**
     * Forward a received advertising packet to all registered event handlers
     * listening for scanned packet events.
     *
     * @important This function is meant to be called from the BLE stack specific
     * implementation when a disconnection event occurs.
     *
     * @param[in] peerAddr Address of the peer that has emitted the packet.
     * @param[in] rssi Value of the RSSI measured for the received packet.
     * @param[in] isScanReponse If true, then the packet is a response to a scan
     * request.
     * @param[in] type Advertising type of the packet.
     * @param[in] advertisingDataLen Length of the advertisement data received.
     * @param[in] advertisingData Pointer to the advertisement packet's data.
     */
    void processAdvertisementReport(
        const BLEProtocol::AddressBytes_t peerAddr,
        int8_t rssi,
        bool isScanResponse,
        GapAdvertisingParams::AdvertisingType_t type,
        uint8_t advertisingDataLen,
        const uint8_t *advertisingData
    ) {
        AdvertisementCallbackParams_t params;
        memcpy(params.peerAddr, peerAddr, ADDR_LEN);
        params.rssi = rssi;
        params.isScanResponse = isScanResponse;
        params.type = type;
        params.advertisingDataLen = advertisingDataLen;
        params.advertisingData = advertisingData;
        onAdvertisementReport.call(&params);
    }

    /**
     * Notify the occurrence of a timeout event to all registered timeout events
     * handler.
     *
     * @important This function is meant to be called from the BLE stack specific
     * implementation when a disconnection event occurs.
     *
     * @param[in] source Source of the timout event.
     */
    void processTimeoutEvent(TimeoutSource_t source)
    {
        if (source == TIMEOUT_SRC_ADVERTISING) {
            /* Update gap state if the source is an advertising timeout */
            state.advertising = 0;
        }
        if (timeoutCallbackChain) {
            timeoutCallbackChain(source);
        }
    }

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
    Gap(const Gap &);
    Gap& operator=(const Gap &);
};

/**
 * @}
 * @}
 */

#endif // ifndef MBED_BLE_GAP_H__
