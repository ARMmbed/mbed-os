/* mbed Microcontroller Library
 * Copyright (c) 2017-2017 ARM Limited
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

#ifndef MBED_BLE_GENERIC_GAP
#define MBED_BLE_GENERIC_GAP

#include <algorithm>

#include "ble/BLE.h"
#include "ble/BLEProtocol.h"
#include "ble/Gap.h"
#include "ble/pal/PalGap.h"
#include "ble/pal/PalSecurityManager.h"
#include "ble/pal/GapEvents.h"
#include "ble/pal/GapTypes.h"
#include "ble/BLETypes.h"
#include "ble/pal/GenericAccessService.h"
#include "ble/pal/EventQueue.h"
#include "ble/pal/ConnectionEventMonitor.h"

#include "drivers/Timeout.h"

namespace ble {
namespace generic {
/**
 * Generic implementation of the Gap class.
 * It requires a pal::Gap and a pal::GenericAccessService injected at
 * construction site.
 *
 * @attention: Not part of the public interface of BLE API.
 */
class GenericGap : public ::Gap,
                   public pal::ConnectionEventMonitor {

public:
    /**
     * Construct a GenericGap instance for a given BLE instance ID.
     *
     * @param ble_instance_id Id of the BLE instance using this instance.
     *
     * @param pal_gap GAP Platform abstraction instance containing the base GAP
     * primitives.
     *
     * @param generic_access_service Platform abstraction instance managing
     * the GATT generic access service.
     * 
     * @param pal_sm Security Manager Platform abstraction instance containing the base
     * Security Manager primitives.
     */
    GenericGap(
        pal::EventQueue &event_queue,
        pal::Gap &pal_gap,
        pal::GenericAccessService &generic_access_service,
        pal::SecurityManager &pal_sm
    );

    /**
     * @see Gap::~Gap
     */
    virtual ~GenericGap();

    /**
     * @see Gap::setAddress
     */
    virtual ble_error_t setAddress(
        BLEProtocol::AddressType_t type,
        const BLEProtocol::AddressBytes_t address
    );

    /**
     * @see Gap::getAddress
     */
    virtual ble_error_t getAddress(
        BLEProtocol::AddressType_t *type,
        BLEProtocol::AddressBytes_t address
    );

    /**
     * @see Gap::getMinAdvertisingInterval
     */
    virtual uint16_t getMinAdvertisingInterval() const;

    /**
     * @see Gap::getMinNonConnectableAdvertisingInterval
     */
    virtual uint16_t getMinNonConnectableAdvertisingInterval() const;

    /**
     * @see Gap::getMaxAdvertisingInterval
     */
    virtual uint16_t getMaxAdvertisingInterval() const;

    /**
     * @see Gap::stopAdvertising
     */
    virtual ble_error_t stopAdvertising();

    /**
     * @see Gap::stopScan
     */
    virtual ble_error_t stopScan();

    /**
     * @see Gap::connect
     */
    virtual ble_error_t connect(
        const BLEProtocol::AddressBytes_t peerAddr,
        PeerAddressType_t peerAddrType,
        const ConnectionParams_t *connectionParams,
        const GapScanningParams *scanParams
    );

    /**
     * @see Gap::connect
     */
    virtual ble_error_t connect(
        const BLEProtocol::AddressBytes_t peerAddr,
        BLEProtocol::AddressType_t peerAddrType,
        const ConnectionParams_t *connectionParams,
        const GapScanningParams *scanParams
    );

    /**
     * @see Gap::disconnect
     */
    virtual ble_error_t disconnect(
        Handle_t connectionHandle,
        DisconnectionReason_t reason
    );

    /**
     * @see Gap::updateConnectionParams
     */
    virtual ble_error_t updateConnectionParams(
        Handle_t handle,
        const ConnectionParams_t *params
    );

    /**
     * @see Gap::getPreferredConnectionParams
     */
    virtual ble_error_t getPreferredConnectionParams(
        ConnectionParams_t *params
    );

    /**
     * @see Gap::setPreferredConnectionParams
     */
    virtual ble_error_t setPreferredConnectionParams(
        const ConnectionParams_t *params
    );

    /**
     * @see Gap::setDeviceName
     */
    virtual ble_error_t setDeviceName(const uint8_t *deviceName);

    /**
     * @see Gap::getDeviceName
     */
    virtual ble_error_t getDeviceName(uint8_t *deviceName, unsigned *lengthP);

    /**
     * @see Gap::setAppearance
     */
    virtual ble_error_t setAppearance(GapAdvertisingData::Appearance appearance);

    /**
     * @see Gap::getAppearance
     */
    virtual ble_error_t getAppearance(GapAdvertisingData::Appearance *appearanceP);

    /**
     * @see Gap::setTxPower
     */
    virtual ble_error_t setTxPower(int8_t txPower);

    /**
     * @see Gap::getPermittedTxPowerValues
     */
    virtual void getPermittedTxPowerValues(const int8_t **valueArrayPP, size_t *countP);

    /**
     * @see Gap::getMaxWhitelistSize
     */
    virtual uint8_t getMaxWhitelistSize(void) const;

    /**
     * @see Gap::getWhitelist
     */
    virtual ble_error_t getWhitelist(Whitelist_t &whitelist) const;

    /**
     * @see Gap::setWhitelist
     */
    virtual ble_error_t setWhitelist(const Whitelist_t &whitelist);

    /**
     * @see Gap::setAdvertisingPolicyMode
     */
    virtual ble_error_t setAdvertisingPolicyMode(AdvertisingPolicyMode_t mode);

    /**
     * @see Gap::setScanningPolicyMode
     */
    virtual ble_error_t setScanningPolicyMode(ScanningPolicyMode_t mode);

    /**
     * @see Gap::setInitiatorPolicyMode
     */
    virtual ble_error_t setInitiatorPolicyMode(InitiatorPolicyMode_t mode);

    /**
     * @see Gap::getAdvertisingPolicyMode
     */
    virtual AdvertisingPolicyMode_t getAdvertisingPolicyMode(void) const;

    /**
     * @see Gap::getScanningPolicyMode
     */
    virtual ScanningPolicyMode_t getScanningPolicyMode(void) const;

    /**
     * @see Gap::getInitiatorPolicyMode
     */
    virtual InitiatorPolicyMode_t getInitiatorPolicyMode(void) const;

    /**
     * @see Gap::startRadioScan
     */
    virtual ble_error_t startRadioScan(const GapScanningParams &scanningParams);

    /**
     * @see Gap::initRadioNotification
     */
    virtual ble_error_t initRadioNotification(void);

    /**
     * @see Gap::enablePrivacy
     */
    virtual ble_error_t enablePrivacy(bool enable);

    /**
     * @see Gap::setPeripheralPrivacyConfiguration
     */
    virtual ble_error_t setPeripheralPrivacyConfiguration(
        const PeripheralPrivacyConfiguration_t *configuration
    );

    /**
     * @see Gap::getPeripheralPrivacyConfiguration
     */
    virtual ble_error_t getPeripheralPrivacyConfiguration(
        PeripheralPrivacyConfiguration_t *configuration
    );

    /**
     * @see Gap::setCentralPrivacyConfiguration
     */
    virtual ble_error_t setCentralPrivacyConfiguration(
        const CentralPrivacyConfiguration_t *configuration
    );

    /**
     * @see Gap::getCentralPrivacyConfiguration
     */
    virtual ble_error_t getCentralPrivacyConfiguration(
        CentralPrivacyConfiguration_t *configuration
    );

    /**
     * @see Gap::setAdvertisingData
     */
    virtual ble_error_t setAdvertisingData(
        const GapAdvertisingData &advData,
        const GapAdvertisingData &scanResponse
    );

    /**
     * @see Gap::startAdvertising
     */
    virtual ble_error_t startAdvertising(const GapAdvertisingParams &params);

    /**
     * @see Gap::reset
     */
    virtual ble_error_t reset(void);

    /**
     * @copydoc ::Gap::processConnectionEvent
     */
    void processConnectionEvent(
        Handle_t handle,
        Role_t role,
        peer_address_type_t peerAddrType,
        const BLEProtocol::AddressBytes_t peerAddr,
        BLEProtocol::AddressType_t ownAddrType,
        const BLEProtocol::AddressBytes_t ownAddr,
        const ConnectionParams_t *connectionParams,
        const uint8_t *peerResolvableAddr,
        const uint8_t *localResolvableAddr
    );

    /**
     * @copydoc ::Gap::processDisconnectionEvent
     */
    void processDisconnectionEvent(
        Handle_t handle,
        DisconnectionReason_t reason
    );

private:
    /** @note Implements ConnectionEventMonitor.
     *  @copydoc ConnectionEventMonitor::set_connection_event_handler
     */
    void set_connection_event_handler(pal::ConnectionEventMonitor::EventHandler *_connection_event_handler);

    void on_scan_timeout();

    void process_scan_timeout();

    void on_advertising_timeout();

    void process_advertising_timeout();

    void on_gap_event_received(const pal::GapEvent &e);

    void on_advertising_report(const pal::GapAdvertisingReportEvent &e);

    void on_connection_complete(const pal::GapConnectionCompleteEvent &e);

    void on_disconnection_complete(const pal::GapDisconnectionCompleteEvent &e);

    void on_connection_parameter_request(
        const pal::GapRemoteConnectionParameterRequestEvent &e
    );

    void on_connection_update(const pal::GapConnectionUpdateEvent &e);

    void on_unexpected_error(const pal::GapUnexpectedErrorEvent &e);

    enum AddressUseType_t {
        CENTRAL_CONNECTION,
        CENTRAL_SCAN,
        PERIPHERAL_CONNECTABLE,
        PERIPHERAL_NON_CONNECTABLE
    };

    pal::own_address_type_t get_own_address_type(AddressUseType_t address_use_type);

    bool initialize_whitelist() const;

    ble_error_t update_address_resolution_setting();

    void set_random_address_rotation(bool enable);

    void update_random_address();

    void on_address_rotation_timeout();

    pal::EventQueue& _event_queue;
    pal::Gap &_pal_gap;
    pal::GenericAccessService &_gap_service;
    pal::SecurityManager &_pal_sm;
    BLEProtocol::AddressType_t _address_type;
    ble::address_t _address;
    pal::initiator_policy_t _initiator_policy_mode;
    pal::scanning_filter_policy_t _scanning_filter_policy;
    pal::advertising_filter_policy_t _advertising_filter_policy;
    mutable Whitelist_t _whitelist;
    
    bool _privacy_enabled;
    PeripheralPrivacyConfiguration_t _peripheral_privacy_configuration;
    CentralPrivacyConfiguration_t _central_privacy_configuration;
    ble::address_t _random_static_identity_address;
    bool _random_address_rotating;
    
    mbed::Timeout _advertising_timeout;
    mbed::Timeout _scan_timeout;
    mbed::Ticker _address_rotation_ticker;
    pal::ConnectionEventMonitor::EventHandler *_connection_event_handler;
};

}
}

#endif /* MBED_BLE_GENERIC_GAP */
