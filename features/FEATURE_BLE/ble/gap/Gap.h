/* mbed Microcontroller Library
 * Copyright (c) 2018-2018 ARM Limited
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

#ifndef BLE_GAP_GAP_H
#define BLE_GAP_GAP_H


#include "ble/BLETypes.h"
#include "ble/BLEProtocol.h"
#include "ble/gap/AdvertisingDataBuilder.h"
#include "ble/gap/ConnectionParameters.h"
#include "ble/gap/ScanParameters.h"
#include "ble/gap/AdvertisingParameters.h"
#include "ble/gap/Events.h"

namespace ble {

class Gap {
public:

    /**
     * Definition of the general handler of Gap related events.
     */
    struct EventHandler {
        /** Called when a scanning device request a scan response.
         *
         * @param event Scan request event: @see ScanRequestEvent_t for details.
         */
        virtual void onScanRequestReceived(const ScanRequestEvent_t &event) { }

        /** Called when advertising ends.
         *
         *  @param event Advertising end event: @see AdvertisingEndEvent_t for details.
         */
        virtual void onAdvertisingEnd(const AdvertisingEndEvent_t &event) { }

        /** Called when scanning reads an advertising packet during passive scan or receives
         *  a scan response during an active scan.
         *
         * @param event Advertising report @see AdvertisingReportEvent_t for details.
         */
        virtual void onAdvertisingReport(const AdvertisingReportEvent &event) { }

        /** Called when scan times out.
         */
        virtual void onScanTimeout(const ScanTimeoutEvent &) { }

        /** Called when first advertising packet in periodic advertising is received.
         *
         * @param event Periodic advertising sync event @see PeriodicAdvertisingSyncEstablishedEvent.
         */
        virtual void onPeriodicAdvertisingSyncEstablished(
            const PeriodicAdvertisingSyncEstablishedEvent &event
        ) { }

        /** Called when a periodic advertising packet is received.
         *
         * @param event Periodic advertisement event.
         */
        virtual void onPeriodicAdvertisingReport(
            const PeriodicAdvertisingReportEvent &event
        ) { }

        virtual void onPeriodicAdvertisingSyncLoss(
            const PeriodicAdvertisingSyncLoss &event
        ) { }

        /** Called when connection attempt ends.
         *
         * @param event Connection event @see ConnectionCompleteEvent_t for details.
         */
        virtual void onConnectionComplete(const ConnectionCompleteEvent &event) { }

        virtual void onUpdateConnectionParametersRequest(
            const UpdateConnectionParametersRequestEvent &event
        ) { }

        virtual void onConnectionParametersUpdateComplete(
            const ConnectionParametersUpdateCompleteEvent &event
        ) { }

        virtual void onDisconnectionComplete(const DisconnectionEvent &event) { }

        /**
         * Function invoked when the current transmitter and receiver PHY have
         * been read for a given connection.
         *
         * @param status Status of the operation: BLE_ERROR_NONE in case of
         * success or an appropriate error code.
         *
         * @param connectionHandle: The handle of the connection for which the
         * PHYs have been read.
         *
         * @param txPhy PHY used by the transmitter.
         *
         * @param rxPhy PHY used by the receiver.
         */
        virtual void onReadPhy(
            ble_error_t status,
            connection_handle_t connectionHandle,
            phy_t txPhy,
            phy_t rxPhy
        ) { }

        /**
         * Function invoked when the update process of the PHY has been completed.
         *
         * The process can be initiated by a call to the function setPhy, the
         * local bluetooth subsystem or the peer.
         *
         * @param status Status of the operation: BLE_ERROR_NONE in case of
         * success or an appropriate error code.
         *
         * @param connectionHandle: The handle of the connection on which the
         * operation was made.
         *
         * @param txPhy PHY used by the transmitter.
         *
         * @param rxPhy PHY used by the receiver.
         *
         * @note Success doesn't mean the PHY has been updated it means both
         * ends have negotiated the best PHY according to their configuration and
         * capabilities. The PHY currently used are present in the txPhy and
         * rxPhy parameters.
         */
        virtual void onPhyUpdateComplete(
            ble_error_t status,
            connection_handle_t connectionHandle,
            phy_t txPhy,
            phy_t rxPhy
        ) { }

    protected:
        /**
         * Prevent polymorphic deletion and avoid unnecessary virtual destructor
         * as the Gap class will never delete the instance it contains.
         */
        ~EventHandler() { }
    };

    /**
     * Assign the event handler implementation that will be used by the gap
     * module to signal events back to the application.
     *
     * @param handler Application implementation of an Eventhandler.
     */
    void setEventHandler(EventHandler *handler)
    {
        _eventHandler = handler;
    }

    /** Check controller support for a specific feature.
     *
     * @param feature Feature to check.
     * @return True if feature is supported.
     */
    virtual bool isFeatureSupported(controller_supported_features_t feature);

    /*                                     advertising                                           */

    /** Return currently available number of supported advertising sets.
     *  This may change at runtime.
     *
     * @return Number of advertising sets that may be created at the same time.
     */
    virtual uint8_t getMaxAdvertisingSetNumber();

    /** Return maximum advertising data length supported.
     *
     * @return Maximum advertising data length supported.
     */
    virtual uint8_t getMaxAdvertisingDataLength();

    /** Create an advertising set and apply the passed in parameters. The handle returned
     *  by this function must be used for all other calls that accept an advertising handle.
     *  When done with advertising, remove from the system using destroyAdvertisingSet().
     *
     * @note The exception is the LEGACY_ADVERTISING_HANDLE which may be used at any time.
     *
     * @param[out] handle Advertising handle returned, valid only if function returned success.
     * @param parameters Advertising parameters for the newly created set.
     * @return BLE_ERROR_NONE on success.
     */
    virtual ble_error_t createAdvertisingSet(
        advertising_handle_t *handle,
        const AdvertisingParameters &parameters
    );

    /** Remove the advertising set (resets its set parameters). The advertising set must not
     *  be active.
     *
     * @note LEGACY_ADVERTISING_HANDLE may not be destroyed.
     *
     * @param handle Advertising set handle.
     * @return BLE_ERROR_NONE on success.
     */
    virtual ble_error_t destroyAdvertisingSet(advertising_handle_t handle);

    /** Set advertising parameters of an existing set.
     *
     * @param handle Advertising set handle.
     * @param params New advertising parameters.
     * @return BLE_ERROR_NONE on success.
     */
    virtual ble_error_t setAdvertisingParameters(
        advertising_handle_t handle,
        const AdvertisingParameters &params
    );

    /** Set new advertising payload for a given advertising set.
     *
     * @param handle Advertising set handle.
     * @param payload Advertising payload.
     * @param minimiseFragmentation Preference for fragmentation.
     * @return BLE_ERROR_NONE on success.
     */
    virtual ble_error_t setAdvertisingPayload(
        advertising_handle_t handle,
        mbed::Span<const uint8_t> payload,
        bool minimiseFragmentation = false
    );

    /** Set new advertising scan response for a given advertising set. This will be sent to
     *  device who perform active scanning.
     *
     * @param handle Advertising set handle.
     * @param response Advertising scan response.
     * @param minimiseFragmentation Preference for fragmentation.
     * @return BLE_ERROR_NONE on success.
     */
    virtual ble_error_t setAdvertisingScanResponse(
        advertising_handle_t handle,
        mbed::Span<const uint8_t> response,
        bool minimiseFragmentation = false
    );

    /** Start advertising using the given advertising set.
     *
     * @param handle Advertising set handle.
     * @param maxDuration Max duration for advertising (in units of 10ms) - 0 means no limit.
     * @param maxEvents Max number of events produced during advertising - 0 means no limit.
     * @return BLE_ERROR_NONE on success.
     */
    virtual ble_error_t startAdvertising(
        advertising_handle_t handle,
        adv_duration_t maxDuration = adv_duration_t::forever(),
        uint8_t maxEvents = 0
    );

    /** Stop advertising given advertising set. This is separate from periodic advertising
     *  which will not be affected.
     *
     * @param handle Advertising set handle.
     * @return BLE_ERROR_NONE on success.
     */
    virtual ble_error_t stopAdvertising(advertising_handle_t handle);

    /** Check if advertising is active for a given advertising set.
     *
     * @param handle Advertising set handle.
     * @return True if advertising is active on this set.
     */
    virtual bool isAdvertisingActive(advertising_handle_t handle);

    /** Set periodic advertising parameters for a given advertising set.
     *
     * @param handle Advertising set handle.
     * @param periodicAdvertisingIntervalMin Minimum interval for periodic advertising.
     * @param periodicAdvertisingIntervalMax Maximum interval for periodic advertising.
     * @param advertiseTxPower Include transmission power in the advertisements.
     * @return BLE_ERROR_NONE on success.
     */
    virtual ble_error_t setPeriodicAdvertisingParameters(
        advertising_handle_t handle,
        periodic_interval_t periodicAdvertisingIntervalMin,
        periodic_interval_t periodicAdvertisingIntervalMax,
        bool advertiseTxPower = true
    );

    /** Set new periodic advertising payload for a given advertising set.
     *
     * @param handle Advertising set handle.
     * @param payload Advertising payload.
     * @return BLE_ERROR_NONE on success.
     */
    virtual ble_error_t setPeriodicAdvertisingPayload(
        advertising_handle_t handle,
        mbed::Span<const uint8_t> payload
    );

    /** Start periodic advertising for a given set. Periodic advertising will not start until
     *  normal advertising is running but will continue to run after normal advertising has stopped.
     *
     * @param handle Advertising set handle.
     * @return BLE_ERROR_NONE on success.
     */
    virtual ble_error_t startPeriodicAdvertising(advertising_handle_t handle);

    /** Stop periodic advertising for a given set.
     *
     * @param handle Advertising set handle.
     * @return BLE_ERROR_NONE on success.
     */
    virtual ble_error_t stopPeriodicAdvertising(advertising_handle_t handle);

    /** Check if periodic advertising is active for a given advertising set.
     *
     * @param handle Advertising set handle.
     * @return True if periodic advertising is active on this set.
     */
    virtual bool isPeriodicAdvertisingActive(advertising_handle_t handle);

    /*                                     scanning                                              */

    /** Set new scan parameters.
     *
     * @param params Scan parameters, @see GapScanParameters for details.
     * @return BLE_ERROR_NONE on success.
     */
    virtual ble_error_t setScanParameters(const ScanParameters &params);

    /** Start scanning.
     *
     * @param filtering Filtering policy.
     * @param duration How long to scan for. Special value 0 means scan forever.
     * @param period How long to scan for in single period. If the period is 0 and duration
     *               is nonzero the scan will last for single duration.
     *
     * @note When the duration and period parameters are non-zero scanning will last for
     * the duration within the period. After the scan period has expired a new scan period
     * will begin and scanning. This will repeat until stopScan() is called.
     *
     * @return BLE_ERROR_NONE on success.
     */
    virtual ble_error_t startScan(
        duplicates_filter_t filtering = duplicates_filter_t::DISABLE,
        scan_duration_t duration = scan_duration_t::forever(),
        scan_period_t period = scan_period_t(0)
    );

    /**
     * Stop the ongoing scanning procedure.
     *
     * The current scanning parameters remain in effect.
     *
     * @retval BLE_ERROR_NONE if successfully stopped scanning procedure.
     */
    virtual ble_error_t stopScan();

    /** Synchronise with periodic advertising from an advertiser and begin receiving periodic
     *  advertising packets.
     *
     * @param peerAddressType Peer address type.
     * @param peerAddress Peer address.
     * @param sid Advertiser set identifier.
     * @param maxPacketSkip Number of consecutive periodic advertising packets that the receiver
     *                      may skip after successfully receiving a periodic advertising packet.
     * @param timeout Maximum permitted time between successful receptions. If this time is
     *                exceeded, synchronisation is lost.
     * @return BLE_ERROR_NONE on success.
     */
    virtual ble_error_t createSync(
        peer_address_type_t peerAddressType,
        const address_t &peerAddress,
        uint8_t sid,
        slave_latency_t maxPacketSkip,
        sync_timeout_t timeout
    );

    /** Synchronise with periodic advertising from an advertiser and begin receiving periodic
     *  advertising packets. Use periodic advertising sync list to determine who to sync with.
     *
     * @param maxPacketSkip Number of consecutive periodic advertising packets that the receiver
     *                      may skip after successfully receiving a periodic advertising packet.
     * @param timeout Maximum permitted time between successful receives.
     *                If this time is exceeded, synchronisation is lost.
     * @return BLE_ERROR_NONE on success.
     */
    virtual ble_error_t createSync(
        slave_latency_t maxPacketSkip,
        sync_timeout_t timeout
    );

    /** Cancel sync attempt.
     *
     * @return BLE_ERROR_NONE on success.
     */
    virtual ble_error_t cancelCreateSync();

    /** Stop reception of the periodic advertising identified by the handle.
     *
     * @param handle Periodic advertising synchronisation handle.
     * @return BLE_ERROR_NONE on success.
     */
    virtual ble_error_t terminateSync(periodic_sync_handle_t handle);

    /** Add device to the periodic advertiser list. Cannot be called when sync is ongoing.
     *
     * @param peerAddressType Peer address type.
     * @param peerAddress Peer address.
     * @param sid Advertiser set identifier.
     * @return BLE_ERROR_NONE on success.
     */
    virtual ble_error_t addDeviceToPeriodicAdvertiserList(
        peer_address_type_t peerAddressType,
        const address_t &peerAddress,
        advertising_sid_t sid
    );

    /** Remove device from the periodic advertiser list. Cannot be called when sync is ongoing.
     *
     * @param peerAddressType Peer address type.
     * @param peerAddress Peer address.
     * @param sid Advertiser set identifier.
     * @return BLE_ERROR_NONE on success.
     */
    virtual ble_error_t removeDeviceFromPeriodicAdvertiserList(
        peer_address_type_t peerAddressType,
        const address_t &peerAddress,
        advertising_sid_t sid
    );

    /** Remove all devices from periodic advertiser list.
     *
     * @return BLE_ERROR_NONE on success.
     */
    virtual ble_error_t clearPeriodicAdvertiserList();

    /** Get number of devices that can be added to the periodic advertiser list.
     * @return Number of devices that can be added to the periodic advertiser list.
     */
    virtual uint8_t getMaxPeriodicAdvertiserListSize();

    /**
     * Initiate a connection to a peer.
     *
     * Once the connection is established an onConnectionComplete in the event handler
     * will be called.
     *
     * @param peerAddressType
     * @param peerAddress
     * @param connectionParams
     *
     * @return BLE_ERROR_NONE if connection establishment procedure is started
     * successfully. The connectionCallChain (if set) is invoked upon
     * a connection event.
     */
    virtual ble_error_t connect(
        target_peer_address_type_t peerAddressType,
        const address_t &peerAddress,
        const ConnectionParameters &connectionParams
    );

    /** Cancel the connection attempt. This is not guaranteed to succeed. As a result
     *  onConnectionComplete in the event handler will be called. Check the success parameter
     *  to see if the connection was created.
     *
     * @return BLE_ERROR_NONE if the connection attempt has been requested to be cancelled.
     */
    virtual ble_error_t cancelConnect();

    virtual ble_error_t updateConnectionParameters(
        connection_handle_t connectionHandle,
        conn_interval_t minConnectionInterval,
        conn_interval_t maxConnectionInterval,
        slave_latency_t slaveLatency,
        supervision_timeout_t supervision_timeout,
        conn_event_length_t minConnectionEventLength = conn_event_length_t(0),
        conn_event_length_t maxConnectionEventLength = conn_event_length_t(0)
    );

    virtual ble_error_t manageConnectionParametersUpdateRequest(
        bool userManageConnectionUpdateRequest
    );

    virtual ble_error_t acceptConnectionParametersUpdate(
        connection_handle_t connectionHandle,
        conn_interval_t minConnectionInterval,
        conn_interval_t maxConnectionInterval,
        slave_latency_t slaveLatency,
        supervision_timeout_t supervision_timeout,
        conn_event_length_t minConnectionEventLength = conn_event_length_t(0),
        conn_event_length_t maxConnectionEventLength = conn_event_length_t(0)
    );

    virtual ble_error_t rejectConnectionParametersUpdate(
        connection_handle_t connectionHandle
    );

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
        connection_handle_t connectionHandle,
        local_disconnection_reason_t reason
    );

    /**
     * Read the PHY used by the transmitter and the receiver on a connection.
     *
     * Once the PHY has been read, it is reported back via the function onPhyRead
     * of the event handler registered by the application.
     *
     * @param connection Handle of the connection for which the PHY being used is
     * queried.
     *
     * @return BLE_ERROR_NONE if the read PHY procedure has been started or an
     * appropriate error code.
     */
    virtual ble_error_t readPhy(connection_handle_t connection);

    /**
     * Set the preferred PHYs to use in a connection.
     *
     * @param txPhys: Set of PHYs preferred for tx operations. If NULL then no
     * preferred PHYs are set and the default value of the subsystem is used.
     *
     * @param rxPhys: Set of PHYs preferred for rx operations. If NULL then no
     * preferred PHYs are set and the default value of the subsystem is used.
     *
     * @return BLE_ERROR_NONE if the preferences have been set or an appropriate
     * error code.
     */
    virtual ble_error_t setPreferredPhys(
        const phy_set_t *txPhys,
        const phy_set_t *rxPhys
    );

    /**
     * Update the PHY used by a connection.
     *
     * Once the update process has been completed, it is reported back to the
     * application via the function onPhyUpdateComplete of the event handler
     * registered by the application.
     *
     * @param connection Handle of the connection to update.
     *
     * @param txPhys Set of PHYs preferred for tx operations. If NULL then the
     * choice is up to the Bluetooth subsystem.
     *
     * @param rxPhys Set of PHYs preferred for rx operations. If NULL then the
     * choice is up to the Bluetooth subsystem.
     *
     * @param codedSymbol Number of symbols used to code a bit when le coded is
     * used. If the value is UNDEFINED then the choice is up to the Bluetooth
     * subsystem.
     *
     * @return BLE_ERROR_NONE if the update PHY procedure has been successfully
     * started or an error code.
     */
    virtual ble_error_t setPhy(
        connection_handle_t connection,
        const phy_set_t *txPhys,
        const phy_set_t *rxPhys,
        coded_symbol_per_bit_t codedSymbol
    );

protected:
    /* Override the following in the underlying adaptation layer to provide the
     * functionality of scanning. */

    /** Can only be called if use_non_deprecated_scan_api() hasn't been called.
     *  This guards against mixed use of deprecated and nondeprecated API.
     */
    virtual void useVersionOneAPI() const { }

    /** Can only be called if use_deprecated_scan_api() hasn't been called.
     *  This guards against mixed use of deprecated and nondeprecated API.
     */
    virtual void useVersionTwoAPI() const { }


    /* -------- soon to be deprecated API -------- */

public:

    /**
 * Privacy Configuration of the peripheral role.
 *
 * @note This configuration also applies to the broadcaster role configuration.
 */
    struct PeripheralPrivacyConfiguration_t {
        /**
         * Indicates if non resolvable random address should be used when the
         * peripheral advertises non connectable packets.
         *
         * Resolvable random address continues to be used for connectable packets.
         */
        bool use_non_resolvable_random_address;

        /**
         * Resolution strategy for initiator resolvable addresses when a
         * connection request is received.
         */
        enum ResolutionStrategy {
            /**
             * Do not resolve the address of the initiator and accept the
             * connection request.
             */
                DO_NOT_RESOLVE,

            /**
             * If a bond is present in the secure database and the address
             * resolution fail then reject the connection request with the error
             * code AUTHENTICATION_FAILLURE.
             */
                REJECT_NON_RESOLVED_ADDRESS,

            /**
             * Perform the pairing procedure if the initiator resolvable
             * address failed the resolution process.
             */
                PERFORM_PAIRING_PROCEDURE,

            /**
             * Perform the authentication procedure if the initiator resolvable
             * address failed the resolution process.
             */
                PERFORM_AUTHENTICATION_PROCEDURE
        };

        /**
         * Connection strategy to use when a connection request contains a
         * private resolvable address.
         */
        ResolutionStrategy resolution_strategy;
    };

    /**
     * Privacy Configuration of the central role.
     *
     * @note This configuration is also used when the local device operates as
     * an observer.
     */
    struct CentralPrivacyConfiguration_t {
        /**
         * Indicates if non resolvable random address should be used when the
         * central or observer sends scan request packets.
         *
         * Resolvable random address continue to be used for connection requests.
         */
        bool use_non_resolvable_random_address;


        /**
         * Resolution strategy of resolvable addresses received in advertising
         * packets.
         */
        enum ResolutionStrategy {
            /**
             * Do not resolve the address received in advertising packets.
             */
                DO_NOT_RESOLVE,

            /**
             * Resolve the resolvable addresses in the advertising packet and
             * forward advertising packet to the application independently of
             * the address resolution procedure result.
             */
                RESOLVE_AND_FORWARD,

            /**
             * Filter out packets containing a resolvable that cannot be resolved
             * by this device.
             *
             * @note Filtering is applied if the local device contains at least
             * one bond.
             */
                RESOLVE_AND_FILTER
        };

        /**
         * Resolution strategy applied to advertising packets received by the
         * local device.
         */
        ResolutionStrategy resolution_strategy;
    };
    
    static const PeripheralPrivacyConfiguration_t
        default_peripheral_privacy_configuration;

    static const CentralPrivacyConfiguration_t
        default_central_privacy_configuration;

    /**
     * Enable or disable privacy mode of the local device.
     *
     * When privacy is enabled, the system use private addresses while it scans,
     * advertises or initiate a connection. The device private address is
     * renewed every 15 minutes.
     *
     * @par Configuration
     *
     * The privacy feature can be configured with the help of the functions
     * setPeripheralPrivacyConfiguration and setCentralPrivacyConfiguration
     * which respectively set the privacy configuration of the peripheral and
     * central role.
     *
     * @par Default configuration of peripheral role
     *
     * By default private resolvable addresses are used for all procedures;
     * including advertisement of non connectable packets. Connection request
     * from an unknown initiator with a private resolvable address triggers the
     * pairing procedure.
     *
     * @par Default configuration of central role
     *
     * By default private resolvable addresses are used for all procedures;
     * including active scanning. Addresses present in advertisement packet are
     * resolved and advertisement packets are forwarded to the application
     * even if the advertiser private address is unknown.
     *
     * @param[in] enable Should be set to true to enable the privacy mode and
     * false to disable it.
     *
     * @return BLE_ERROR_NONE in case of success or an appropriate error code.
     */
    virtual ble_error_t enablePrivacy(bool enable);

    /**
     * Set the privacy configuration used by the peripheral role.
     *
     * @param[in] configuration The configuration to set.
     *
     * @return BLE_ERROR_NONE in case of success or an appropriate error code.
     */
    virtual ble_error_t setPeripheralPrivacyConfiguration(
        const PeripheralPrivacyConfiguration_t *configuration
    );

    /**
     * Get the privacy configuration used by the peripheral role.
     *
     * @param[out] configuration The variable filled with the current
     * configuration.
     *
     * @return BLE_ERROR_NONE in case of success or an appropriate error code.
     */
    virtual ble_error_t getPeripheralPrivacyConfiguration(
        PeripheralPrivacyConfiguration_t *configuration
    );

    /**
     * Set the privacy configuration used by the central role.
     *
     * @param[in] configuration The configuration to set.
     *
     * @return BLE_ERROR_NONE in case of success or an appropriate error code.
     */
    virtual ble_error_t setCentralPrivacyConfiguration(
        const CentralPrivacyConfiguration_t *configuration
    );

    /**
     * Get the privacy configuration used by the central role.
     *
     * @param[out] configuration The variable filled with the current
     * configuration.
     *
     * @return BLE_ERROR_NONE in case of success or an appropriate error code.
     */
    virtual ble_error_t getCentralPrivacyConfiguration(
        CentralPrivacyConfiguration_t *configuration
    );

protected:
    /**
     * Construct a Gap instance.
     */
    Gap() : _eventHandler(NULL) { }

    /**
     * Event handler provided by the application.
     */
    EventHandler *_eventHandler;
};

} // namespace ble

#endif //BLE_GAP_GAP_H
