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

#include <algorithm>
#include <stdint.h>

#include "ble/BLEInstanceBase.h"
#include "ble/BLEProtocol.h"
#include "ble/Gap.h"
#include "ble/pal/PalGap.h"
#include "ble/pal/GapEvents.h"
#include "ble/pal/GapTypes.h"
#include "ble/pal/GenericAccessService.h"
#include "ble/generic/GenericGap.h"

#include "drivers/Timeout.h"
#include "platform/Span.h"

#include "ble/pal/Deprecated.h"

namespace ble {
namespace generic {

using pal::connection_peer_address_type_t;
typedef BLEProtocol::AddressType_t LegacyAddressType_t;
typedef BLEProtocol::AddressType LegacyAddressType;

namespace {

// Constants
static const uint16_t scan_interval_min = 0x0004;
static const uint16_t scan_interval_max = 0x4000;
static const uint16_t connection_interval_min = 0x0006;
static const uint16_t connection_interval_max = 0x0C80;
static const uint16_t slave_latency_min = 0x0000;
static const uint16_t slave_latency_max = 0x01F3;
static const uint16_t advertising_interval_min = 0x0020;
static const uint16_t advertising_interval_max = 0x4000;
static const uint16_t supervision_timeout_min = 0x000A;
static const uint16_t supervision_timeout_max = 0x0C80;

static const ::Gap::ConnectionParams_t default_connection_params = {
    /* min conn interval */ 50,
    /* max  conn interval */ 100,
    /* slave latency */ 0,
    /* supervision timeout */ 600
};

static const GapScanningParams default_scan_params;

static const mbed_error_status_t mixed_scan_api_error =
    MBED_MAKE_ERROR(MBED_MODULE_BLE, MBED_ERROR_CODE_BLE_USE_INCOMPATIBLE_API);

static const mbed_error_status_t illegal_state_error =
    MBED_MAKE_ERROR(MBED_MODULE_BLE, MBED_ERROR_CODE_BLE_ILLEGAL_STATE);


/*
 * Return true if value is included in the range [lower_bound : higher_bound]
 */
template<typename T>
static bool is_in_range(T value, T lower_bound, T higher_bound)
{
    if (value < lower_bound || value > higher_bound) {
        return false;
    }
    return true;
}

/*
 * Return true if the scan parameters are valid or false otherwise.
 */
static bool is_scan_params_valid(const GapScanningParams *params)
{
    if (params == NULL) {
        return false;
    }

    if (is_in_range(params->getInterval(), scan_interval_min, scan_interval_max) == false) {
        return false;
    }

    if (is_in_range(params->getWindow(), scan_interval_min, params->getInterval()) == false) {
        return false;
    }

    return true;
}

/*
 * Return true if the connection parameters are valid or false otherwise.
 */
static bool is_connection_params_valid(const ::Gap::ConnectionParams_t *params)
{
    if (params == NULL) {
        return false;
    }

    if (is_in_range(params->slaveLatency, slave_latency_min, slave_latency_max) == false) {
        return false;
    }

    if (is_in_range(params->maxConnectionInterval, connection_interval_min, connection_interval_max) == false) {
        return false;
    }

    if (is_in_range(params->minConnectionInterval, connection_interval_min, params->maxConnectionInterval) == false) {
        return false;
    }

    if (is_in_range(params->connectionSupervisionTimeout, supervision_timeout_min, supervision_timeout_max) == false) {
        return false;
    }

    uint16_t max_connection_interval_ms =
        ((uint32_t) params->maxConnectionInterval * 125) / 100;
    uint16_t min_connection_supervision_timeout =
        ((1 + params->slaveLatency) * max_connection_interval_ms * 2) / 10;

    if (params->connectionSupervisionTimeout < min_connection_supervision_timeout) {
        return false;
    }

    return true;
}

/*
 * Return true of the connection parameters are acceptable as preferred connection
 * parameters.
 *
 * Prefered connection parameters unlike actual connection parameters allow the
 * max connection interval, min connection interval and connection supervision
 * timeout to be equal to 0xFFFF. When it is the case that value can be
 * interpreted as "non specific".
 */
static bool is_preferred_connection_params_valid(const ::Gap::ConnectionParams_t *params)
{
    if (params == NULL) {
        return false;
    }

    if (is_in_range(params->slaveLatency, slave_latency_min, slave_latency_max) == false) {
        return false;
    }

    if ((is_in_range(params->maxConnectionInterval, connection_interval_min, connection_interval_max) == false) &&
        (params->maxConnectionInterval != 0xFFFF)) {
        return false;
    }

    if ((is_in_range(params->minConnectionInterval, connection_interval_min, params->maxConnectionInterval) == false) &&
        (params->minConnectionInterval != 0xFFFF)) {
        return false;
    }

    if (params->connectionSupervisionTimeout == 0xFFFF) {
        return true;
    }

    if ((is_in_range(params->connectionSupervisionTimeout, supervision_timeout_min, supervision_timeout_max) == false)) {
        return false;
    }

    if (params->maxConnectionInterval == 0xFFFF) {
        return true;
    }

    uint16_t max_connection_interval_ms =
        ((uint32_t) params->maxConnectionInterval * 125) / 100;
    uint16_t min_connection_supervision_timeout =
        ((1 + params->slaveLatency) * max_connection_interval_ms * 2) / 10;

    if (params->connectionSupervisionTimeout < min_connection_supervision_timeout) {
        return false;
    }

    return true;
}

/**
 * Check if random bytes of an address are valid.
 */
static bool is_prand_valid(const uint8_t *bytes, size_t len)
{
    // at least one bit of the random part of the static address shall be
    // equal to 0 and at least one bit of the random part of the static
    // address shall be equal to 1
    for (size_t i = 0; i < (len - 1); ++i) {
        if ((bytes[i] != 0x00) && (bytes[i] != 0xFF)) {
            return true;
        }

        if ((i > 0) && (bytes[i] != bytes[i - 1])) {
            return true;
        }
    }

    if (((bytes[len - 1] & 0x3F) == 0x3F) && (bytes[len - 2] == 0xFF)) {
        return false;
    }

    if (((bytes[len - 1] & 0x3F) == 0x00) && (bytes[len - 2] == 0x00)) {
        return false;
    }

    return true;
}

/*
 * Check if the random part of a random address with 48 random bytes are valid
 * or not.
 * Return true if it is the case and false otherwise.
 */
static bool is_prand_48_bits_valid(const BLEProtocol::AddressBytes_t address)
{
    return is_prand_valid(address, 6);
}

/*
 * Check if the random part of a random address with 24 random bytes are valid
 * or not.
 * Return true if it is the case and false otherwise.
 */
static bool is_prand_24_bits_valid(const BLEProtocol::AddressBytes_t address)
{
    return is_prand_valid(address + 3, 3);
}

/*
 * Return true if address is a random static address.
 */
static bool is_random_static_address(const BLEProtocol::AddressBytes_t address)
{
    // top two msb bits shall be equal to 0b11.
    if ((address[5] & 0xC0) != 0xC0) {
        return false;
    }

    return is_prand_48_bits_valid(address);
}

/*
 * Return true if address is a random private non resolvable address.
 */
static bool is_random_private_non_resolvable_address(
    const BLEProtocol::AddressBytes_t address
)
{
    // top two msb bits shall be equal to 0b00.
    if ((address[5] & 0xC0) != 0x00) {
        return false;
    }

    return is_prand_48_bits_valid(address);
}

/*
 * Return true if address is a random private resolvable address.
 */
static bool is_random_private_resolvable_address(
    const BLEProtocol::AddressBytes_t address
)
{
    // top two msb bits shall be equal to 0b01.
    if ((address[5] & 0xC0) != 0x40) {
        return false;
    }

    return is_prand_24_bits_valid(address);
}

/*
 * Return true if the address is a random address.
 */
static bool is_random_address(const BLEProtocol::AddressBytes_t address)
{
    return is_random_private_resolvable_address(address) ||
        is_random_private_non_resolvable_address(address) ||
        is_random_static_address(address);
}

/*
 * Check disconnection reason validity.
 */
static bool is_disconnection_reason_valid(::Gap::DisconnectionReason_t reason)
{
    switch (reason) {
        /**
         * Note: accepted reasons are:
        typedef pal::disconnection_reason_t reason_t;
        case reason_t::AUTHENTICATION_FAILLURE:
        case reason_t::REMOTE_USER_TERMINATED_CONNECTION:
        case reason_t::REMOTE_DEVICE_TERMINATED_CONNECTION_DUE_TO_LOW_RESOURCES:
        case reason_t::REMOTE_DEVICE_TERMINATED_CONNECTION_DUE_TO_POWER_OFF:
        case reason_t::UNSUPPORTED_REMOTE_FEATURE:
        case reason_t::PAIRING_WITH_UNIT_KEY_NOT_SUPPORTED:
        case reason_t::UNACCEPTABLE_CONNECTION_PARAMETERS:
            */

        // TODO Fix Disconnectionreason_t which expose invalid value
        case ::Gap::REMOTE_USER_TERMINATED_CONNECTION:
        case ::Gap::REMOTE_DEV_TERMINATION_DUE_TO_LOW_RESOURCES:
        case ::Gap::REMOTE_DEV_TERMINATION_DUE_TO_POWER_OFF:
        case ::Gap::CONN_INTERVAL_UNACCEPTABLE:
            return true;
        default:
            return false;
    }
}

/*
 * Return true if the whitelist in input is valid or false otherwise.
 */
static bool is_whitelist_valid(const ::Gap::Whitelist_t &whitelist)
{
    if (whitelist.size > whitelist.capacity) {
        return false;
    }

    for (size_t i = 0; i < whitelist.size; ++i) {
        const BLEProtocol::Address_t &address = whitelist.addresses[i];
        if (address.type != BLEProtocol::AddressType::PUBLIC &&
            address.type != BLEProtocol::AddressType::RANDOM_STATIC
        ) {
            return false;
        }

        if (address.type != BLEProtocol::AddressType::PUBLIC) {
            if (is_random_address(address.address) == false) {
                return false;
            }
        }
    }
    return true;
}

/*
 * Return true if device is present in the whitelist.
 */
static bool is_in_whitelist(
    const BLEProtocol::Address_t &device, const ::Gap::Whitelist_t &whitelist
)
{
    for (size_t i = 0; i < whitelist.size; ++i) {
        const BLEProtocol::Address_t &potential_device = whitelist.addresses[i];

        if (potential_device.type != device.type) {
            continue;
        }

        if (memcmp(potential_device.address, device.address, sizeof(device.address)) == 0) {
            return true;
        }
    }
    return false;
}

/*
 * Convert a BLEProtocol::AddressType_t into a pal::whitelist_address_type_t.
 */
static pal::whitelist_address_type_t to_whitelist_address_type(
    BLEProtocol::AddressType_t address_type
)
{
    return (address_type == BLEProtocol::AddressType::PUBLIC) ?
        pal::whitelist_address_type_t::PUBLIC_DEVICE_ADDRESS :
        pal::whitelist_address_type_t::RANDOM_DEVICE_ADDRESS;
}

/*
 * Convert a BLEProtocol::AddressType_t into a pal::peer_address_type
 */
static peer_address_type_t to_peer_address_type(
    LegacyAddressType_t address_type
)
{
    return (address_type == LegacyAddressType::PUBLIC) ?
        peer_address_type_t::PUBLIC :
        peer_address_type_t::RANDOM;
}


/*
 * Return true if the advertising parameters are valid.
 */
static bool is_advertising_params_valid(const GapAdvertisingParams &params)
{
    if (is_in_range(params.getIntervalInADVUnits(), advertising_interval_min, advertising_interval_max) == false) {
        return false;
    }

    if (params.getAdvertisingType() > GapAdvertisingParams::ADV_NON_CONNECTABLE_UNDIRECTED) {
        return false;
    }

    return true;
}

microsecond_t minSupervisionTimeout(
    const conn_interval_t &maxConnectionInterval,
    const slave_latency_t &slaveLatency
)
{
    // see BLUETOOTH SPECIFICATION Version 5.0 | Vol 2, Part E
    // Section 7.8.12 LE Create Connection
    return (1 + slaveLatency.value()) * maxConnectionInterval * 2;
}

} // end of anonymous namespace

GenericGap::GenericGap(
    pal::EventQueue &event_queue,
    pal::Gap &pal_gap,
    pal::GenericAccessService &generic_access_service,
    pal::SecurityManager &pal_sm
) : _event_queue(event_queue),
    _pal_gap(pal_gap),
    _gap_service(generic_access_service),
    _pal_sm(pal_sm),
    _address_type(LegacyAddressType::PUBLIC),
    _initiator_policy_mode(pal::initiator_policy_t::NO_FILTER),
    _scanning_filter_policy(pal::scanning_filter_policy_t::NO_FILTER),
    _advertising_filter_policy(pal::advertising_filter_policy_t::NO_FILTER),
    _whitelist(),
    _privacy_enabled(false),
    _peripheral_privacy_configuration(default_peripheral_privacy_configuration),
    _central_privacy_configuration(default_central_privacy_configuration),
    _random_address_rotating(false),
    _scan_enabled(false),
    _advertising_timeout(),
    _scan_timeout(),
    _connection_event_handler(NULL),
    _deprecated_scan_api_used(false),
    _non_deprecated_scan_api_used(false),
    _user_manage_connection_parameter_requests(false)
{
    _pal_gap.initialize();

    _pal_gap.when_gap_event_received(
        mbed::callback(this, &GenericGap::on_gap_event_received)
    );

    // Recover static random identity
    _random_static_identity_address = _pal_gap.get_random_address();

    _pal_gap.set_event_handler(this);

    if (is_extended_advertising_available()) {
        setExtendedAdvertisingParameters(
            LEGACY_ADVERTISING_HANDLE,
            AdvertisingParameters()
        );
    }

    _existing_sets.set(LEGACY_ADVERTISING_HANDLE);
}

GenericGap::~GenericGap()
{
}

bool GenericGap::isFeatureSupported(controller_supported_features_t feature)
{
    return _pal_gap.is_feature_supported(feature);
}

ble_error_t GenericGap::setAddress(
    LegacyAddressType_t type,
    const Address_t address
)
{
    switch (type) {
        case LegacyAddressType::PUBLIC:
            // The public address cannot be set, just set the type to public
            _address_type = type;
            return BLE_ERROR_NONE;

        case LegacyAddressType::RANDOM_STATIC: {
            if (is_random_static_address(address) == false) {
                return BLE_ERROR_INVALID_PARAM;
            }

            ble_error_t err = _pal_gap.set_random_address(
                ble::address_t(address)
            );
            if (err) {
                return err;
            }

            _address_type = type;
            _address = ble::address_t(address);
            _random_static_identity_address = ble::address_t(address);
            return BLE_ERROR_NONE;
        }

        case LegacyAddressType::RANDOM_PRIVATE_RESOLVABLE:
        case LegacyAddressType::RANDOM_PRIVATE_NON_RESOLVABLE:
            // Note: it is not allowed to set directly these addresses
            // privacy management handled it for users.
            return BLE_ERROR_INVALID_PARAM;

        default:
            return BLE_ERROR_INVALID_PARAM;
    }
}

ble_error_t GenericGap::getAddress(
    LegacyAddressType_t *type,
    Address_t address
)
{
    *type = _address_type;
    ble::address_t address_value;

    if (_address_type == LegacyAddressType::PUBLIC) {
        address_value = _pal_gap.get_device_address();
    } else {
        address_value = _pal_gap.get_random_address();
    }

    memcpy(address, address_value.data(), address_value.size());
    return BLE_ERROR_NONE;
}

uint16_t GenericGap::getMinAdvertisingInterval() const
{
    return GapAdvertisingParams::GAP_ADV_PARAMS_INTERVAL_MIN;
}

uint16_t GenericGap::getMinNonConnectableAdvertisingInterval() const
{
    return GapAdvertisingParams::GAP_ADV_PARAMS_INTERVAL_MIN_NONCON;
}

uint16_t GenericGap::getMaxAdvertisingInterval() const
{
    return GapAdvertisingParams::GAP_ADV_PARAMS_INTERVAL_MAX;
}

ble_error_t GenericGap::stopAdvertising()
{
    useVersionOneAPI();
    ble_error_t err = _pal_gap.advertising_enable(false);
    if (err) {
        return err;
    }
    _advertising_timeout.detach();
    state.advertising = false;

    // Stop address rotation if required
    set_random_address_rotation(false);

    return BLE_ERROR_NONE;
}

ble_error_t GenericGap::stopScan()
{
    ble_error_t err;

    if (!_scan_enabled) {
        return BLE_ERROR_NONE;
    }

    _scan_enabled = false;

    if (is_extended_advertising_available()) {
        err = _pal_gap.extended_scan_enable(false, pal::duplicates_filter_t::DISABLE, 0, 0);
    } else {
        err = _pal_gap.scan_enable(false, false);
    }

    if (err) {
        _scan_enabled = true;
        return err;
    }

    // Stop address rotation if required
    set_random_address_rotation(false);

    _scan_timeout.detach();

    return BLE_ERROR_NONE;
}

ble_error_t GenericGap::connect(
    const Address_t peerAddr,
    PeerAddressType_t peerAddrType,
    const ConnectionParams_t *connectionParams,
    const GapScanningParams *scanParams
)
{
    useVersionOneAPI();
    if (connectionParams == NULL) {
        connectionParams = &default_connection_params;
    }

    if (scanParams == NULL) {
        scanParams = &default_scan_params;
    }

    if (is_scan_params_valid(scanParams) == false) {
        return BLE_ERROR_PARAM_OUT_OF_RANGE;
    }

    if (is_connection_params_valid(connectionParams) == false) {
        return BLE_ERROR_PARAM_OUT_OF_RANGE;
    }

    // Force scan stop before initiating the scan used for connection
    stopScan();

    return _pal_gap.create_connection(
        scanParams->getInterval(),
        scanParams->getWindow(),
        _initiator_policy_mode,
        (pal::connection_peer_address_type_t::type) peerAddrType.value(),
        ble::address_t(peerAddr),
        get_own_address_type(CENTRAL_CONNECTION /* requires resolvable address */),
        connectionParams->minConnectionInterval,
        connectionParams->maxConnectionInterval,
        connectionParams->slaveLatency,
        connectionParams->connectionSupervisionTimeout,
        /* minimum_connection_event_length */ 0,
        /* maximum_connection_event_length */ 0
    );
}


ble_error_t GenericGap::connect(
    const Address_t peerAddr,
    LegacyAddressType_t peerAddrType,
    const ConnectionParams_t *connectionParams,
    const GapScanningParams *scanParams
)
{
    useVersionOneAPI();
    return connect(
        peerAddr,
        to_peer_address_type(peerAddrType),
        connectionParams,
        scanParams
    );
}


ble_error_t GenericGap::connect(
    peer_address_type_t peerAddressType,
    const ble::address_t &peerAddress,
    const ConnectionParameters &connectionParams
)
{
    useVersionTwoAPI();

    if (!connectionParams.getNumberOfEnabledPhys()) {
        return BLE_ERROR_INVALID_PARAM;
    }

    if (peerAddressType == peer_address_type_t::ANONYMOUS) {
        return BLE_ERROR_INVALID_PARAM;
    }

    for (size_t i = 0; i < connectionParams.getPhySet().count(); ++i) {
        if (supervision_timeout_t(connectionParams.getConnectionSupervisionTimeoutArray()[i]) <=
            minSupervisionTimeout(
                conn_interval_t(connectionParams.getMaxConnectionIntervalArray()[i]),
                slave_latency_t(connectionParams.getSlaveLatencyArray()[i])
            )
        ) {
            return BLE_ERROR_INVALID_PARAM;
        }
    }

    if (is_extended_advertising_available() == false) {
        phy_set_t set(connectionParams.getPhySet());
        if (set.count() != 1 || set.get_1m() == false) {
            return BLE_ERROR_INVALID_PARAM;
        }

        // ensure scan is stopped.
        _pal_gap.scan_enable(false, false);

        return _pal_gap.create_connection(
            connectionParams.getScanIntervalArray()[0],
            connectionParams.getScanWindowArray()[0],
            connectionParams.getFilter(),
            (pal::connection_peer_address_type_t::type) peerAddressType.value(),
            peerAddress,
            connectionParams.getOwnAddressType(),
            connectionParams.getMinConnectionIntervalArray()[0],
            connectionParams.getMaxConnectionIntervalArray()[0],
            connectionParams.getSlaveLatencyArray()[0],
            connectionParams.getConnectionSupervisionTimeoutArray()[0],
            connectionParams.getMinEventLengthArray()[0],
            connectionParams.getMaxConnectionIntervalArray()[0]
        );
    }

    // ensure scan is stopped.
    _pal_gap.extended_scan_enable(false, pal::duplicates_filter_t::DISABLE, 0, 0);

    // reduce the address type to public or random
    peer_address_type_t adjusted_address_type(peer_address_type_t::PUBLIC);

    if (peerAddressType == peer_address_type_t::RANDOM ||
        peerAddressType == peer_address_type_t::RANDOM_STATIC_IDENTITY
    ) {
        adjusted_address_type = peer_address_type_t::RANDOM;
    }

    return _pal_gap.extended_create_connection(
        connectionParams.getFilter(),
        connectionParams.getOwnAddressType(),
        adjusted_address_type,
        peerAddress,
        connectionParams.getPhySet(),
        connectionParams.getScanIntervalArray(),
        connectionParams.getScanWindowArray(),
        connectionParams.getMinConnectionIntervalArray(),
        connectionParams.getMaxConnectionIntervalArray(),
        connectionParams.getSlaveLatencyArray(),
        connectionParams.getConnectionSupervisionTimeoutArray(),
        connectionParams.getMinEventLengthArray(),
        connectionParams.getMaxEventLengthArray()
    );
}

ble_error_t GenericGap::manageConnectionParametersUpdateRequest(bool flag)
{
    _user_manage_connection_parameter_requests = flag;
    return BLE_ERROR_NONE;
}

ble_error_t GenericGap::updateConnectionParameters(
    connection_handle_t connectionHandle,
    conn_interval_t minConnectionInterval,
    conn_interval_t maxConnectionInterval,
    slave_latency_t slaveLatency,
    supervision_timeout_t supervisionTimeout,
    conn_event_length_t minConnectionEventLength,
    conn_event_length_t maxConnectionEventLength
)
{
    useVersionTwoAPI();

    if (supervisionTimeout <= minSupervisionTimeout(maxConnectionInterval, slaveLatency)) {
        return BLE_ERROR_INVALID_PARAM;
    }

    return _pal_gap.connection_parameters_update(
        connectionHandle,
        minConnectionInterval.value(),
        maxConnectionInterval.value(),
        slaveLatency.value(),
        supervisionTimeout.value(),
        minConnectionEventLength.value(),
        maxConnectionEventLength.value()
    );
}

ble_error_t GenericGap::acceptConnectionParametersUpdate(
    connection_handle_t connectionHandle,
    conn_interval_t minConnectionInterval,
    conn_interval_t maxConnectionInterval,
    slave_latency_t slaveLatency,
    supervision_timeout_t supervisionTimeout,
    conn_event_length_t minConnectionEventLength,
    conn_event_length_t maxConnectionEventLength
)
{
    useVersionTwoAPI();

    if (supervisionTimeout <= minSupervisionTimeout(maxConnectionInterval, slaveLatency)) {
        return BLE_ERROR_INVALID_PARAM;
    }

    return _pal_gap.accept_connection_parameter_request(
        connectionHandle,
        minConnectionInterval.value(),
        maxConnectionInterval.value(),
        slaveLatency.value(),
        supervisionTimeout.value(),
        minConnectionEventLength.value(),
        maxConnectionEventLength.value()
    );
}

ble_error_t GenericGap::rejectConnectionParametersUpdate(
    connection_handle_t connectionHandle
)
{
    useVersionTwoAPI();

    return _pal_gap.reject_connection_parameter_request(
        connectionHandle,
        pal::hci_error_code_t::UNACCEPTABLE_CONNECTION_PARAMETERS
    );
}

ble_error_t GenericGap::cancelConnect()
{
    useVersionTwoAPI();
    return _pal_gap.cancel_connection_creation();
}

ble_error_t GenericGap::readPhy(Handle_t connection)
{
    return _pal_gap.read_phy(connection);
}

ble_error_t GenericGap::setPreferredPhys(
    const phy_set_t *txPhys,
    const phy_set_t *rxPhys
)
{
    phy_set_t tx_phys(txPhys ? txPhys->value() : 0);
    phy_set_t rx_phys(rxPhys ? rxPhys->value() : 0);
    return _pal_gap.set_preferred_phys(tx_phys, rx_phys);
}

ble_error_t GenericGap::setPhy(
    Handle_t connection,
    const phy_set_t *txPhys,
    const phy_set_t *rxPhys,
    CodedSymbolPerBit_t codedSymbol
)
{
    phy_set_t tx_phys(txPhys ? txPhys->value() : 0);
    phy_set_t rx_phys(rxPhys ? rxPhys->value() : 0);
    return _pal_gap.set_phy(connection, tx_phys, rx_phys, codedSymbol);
}


void GenericGap::on_read_phy(
    pal::hci_error_code_t hci_status,
    Handle_t connection_handle,
    phy_t tx_phy,
    phy_t rx_phy
)
{
    ble_error_t status = BLE_ERROR_NONE;
    if (hci_status != pal::hci_error_code_t::SUCCESS) {
        status = BLE_ERROR_UNSPECIFIED;
    }

    if (_eventHandler) {
        _eventHandler->onPhyUpdateComplete(status, connection_handle, tx_phy, rx_phy);
    }
}

void GenericGap::on_phy_update_complete(
    pal::hci_error_code_t hci_status,
    Handle_t connection_handle,
    phy_t tx_phy,
    phy_t rx_phy
)
{
    ble_error_t status = BLE_ERROR_NONE;
    if (hci_status != pal::hci_error_code_t::SUCCESS) {
        status = BLE_ERROR_UNSPECIFIED;
    }

    if (_eventHandler) {
        _eventHandler->onPhyUpdateComplete(status, connection_handle, tx_phy, rx_phy);
    }
}

ble_error_t GenericGap::disconnect(
    connection_handle_t connectionHandle,
    local_disconnection_reason_t reason
)
{
    useVersionTwoAPI();

    return _pal_gap.disconnect(connectionHandle, reason);
}

ble_error_t GenericGap::disconnect(Handle_t connectionHandle, DisconnectionReason_t reason)
{
    useVersionOneAPI();

    if (is_disconnection_reason_valid(reason) == false) {
        return BLE_ERROR_INVALID_PARAM;
    }
    return _pal_gap.disconnect(
        connectionHandle,
        (pal::disconnection_reason_t::type) reason
    );
}

ble_error_t GenericGap::updateConnectionParams(Handle_t handle, const ConnectionParams_t *params)
{
    useVersionOneAPI();

    if (is_connection_params_valid(params) == false) {
        return BLE_ERROR_PARAM_OUT_OF_RANGE;
    }

    return _pal_gap.connection_parameters_update(
        handle,
        params->minConnectionInterval,
        params->maxConnectionInterval,
        params->slaveLatency,
        params->connectionSupervisionTimeout,
        /* minimum_connection_event_length */ 0,
        /* maximum_connection_event_length */ 0
    );
}

ble_error_t GenericGap::getPreferredConnectionParams(ConnectionParams_t *params)
{
    if (params == NULL) {
        return BLE_ERROR_INVALID_PARAM;
    }

    return _gap_service.get_peripheral_prefered_connection_parameters(
        *params
    );
}

ble_error_t GenericGap::setPreferredConnectionParams(const ConnectionParams_t *params)
{
    if (is_preferred_connection_params_valid(params) == false) {
        return BLE_ERROR_PARAM_OUT_OF_RANGE;
    }

    return _gap_service.set_peripheral_prefered_connection_parameters(
        *params
    );
}

ble_error_t GenericGap::setDeviceName(const uint8_t *deviceName)
{
    return _gap_service.set_device_name(deviceName);
}

ble_error_t GenericGap::getDeviceName(uint8_t *deviceName, unsigned *lengthP)
{
    if (lengthP == NULL) {
        return BLE_ERROR_INVALID_PARAM;
    }

    uint8_t length = 0;
    ble_error_t err = _gap_service.get_device_name_length(length);
    if (err) {
        return err;
    }

    if (deviceName != NULL) {
        if (*lengthP < length) {
            return BLE_ERROR_INVALID_PARAM;
        }

        ArrayView<uint8_t> name(deviceName, *lengthP);
        err = _gap_service.get_device_name(name);
        if (err) {
            return err;
        }
    }
    *lengthP = length;
    return BLE_ERROR_NONE;
}

ble_error_t GenericGap::setAppearance(GapAdvertisingData::Appearance appearance)
{
    return _gap_service.set_appearance(appearance);
}

ble_error_t GenericGap::getAppearance(GapAdvertisingData::Appearance *appearanceP)
{
    if (appearanceP == NULL) {
        return BLE_ERROR_INVALID_PARAM;
    }
    return _gap_service.get_appearance(*appearanceP);
}

ble_error_t GenericGap::setTxPower(int8_t txPower)
{
    // TODO: This is not standard, expose it as an extension API and document it
    // as such
    return BLE_ERROR_NOT_IMPLEMENTED;
}

void GenericGap::getPermittedTxPowerValues(const int8_t **valueArrayPP, size_t *countP)
{
    *countP = 0;
}

uint8_t GenericGap::getMaxWhitelistSize(void) const
{
    return _pal_gap.read_white_list_capacity();
}

ble_error_t GenericGap::getWhitelist(Whitelist_t &whitelist) const
{
    if (initialize_whitelist() == false) {
        return BLE_ERROR_INVALID_STATE;
    }

    if (whitelist.capacity < _whitelist.capacity) {
        return BLE_ERROR_INVALID_PARAM;
    }

    for (size_t i = 0; i < _whitelist.size; ++i) {
        whitelist.addresses[i] = _whitelist.addresses[i];
    }

    whitelist.capacity = _whitelist.capacity;
    return BLE_ERROR_NONE;
}

ble_error_t GenericGap::setWhitelist(const Whitelist_t &whitelist)
{
    if (is_whitelist_valid(whitelist) == false) {
        return BLE_ERROR_INVALID_PARAM;
    }

    if (initialize_whitelist() == false) {
        return BLE_ERROR_INVALID_STATE;
    }

    if (whitelist.capacity > _whitelist.capacity) {
        return BLE_ERROR_INVALID_PARAM;
    }

    // first evict devices not in the existing whitelist
    for (size_t i = 0; i < _whitelist.size; ++i) {
        const BLEProtocol::Address_t &device = _whitelist.addresses[i];

        if (is_in_whitelist(device, whitelist) == false) {
            ble_error_t err = _pal_gap.remove_device_from_whitelist(
                to_whitelist_address_type(device.type),
                device.address
            );

            // try to restore the whitelist to its initial state
            if (err) {
                for (size_t j = 0; j < i; ++j) {
                    const BLEProtocol::Address_t &device = _whitelist.addresses[j];

                    if (is_in_whitelist(device, whitelist) == false) {
                        _pal_gap.add_device_to_whitelist(
                            to_whitelist_address_type(device.type),
                            device.address
                        );
                    }
                }
                return err;
            }
        }
    }

    // second add devices which were not in the initial whitelist
    for (size_t i = 0; i < whitelist.size; ++i) {
        const BLEProtocol::Address_t &device = whitelist.addresses[i];

        if (is_in_whitelist(device, _whitelist) == false) {
            ble_error_t err = _pal_gap.add_device_to_whitelist(
                to_whitelist_address_type(device.type),
                device.address
            );

            // try to restore the whitelist to its initial state
            if (err) {
                // first remove the devices added
                for (size_t j = 0; j < i; ++j) {
                    const BLEProtocol::Address_t &device = whitelist.addresses[j];

                    if (is_in_whitelist(device, _whitelist) == false) {
                        _pal_gap.remove_device_from_whitelist(
                            to_whitelist_address_type(device.type),
                            device.address
                        );
                    }
                }

                // second add the devices of the initial list evicted
                for (size_t i = 0; i < _whitelist.size; ++i) {
                    const BLEProtocol::Address_t &device = _whitelist.addresses[i];

                    if (is_in_whitelist(device, whitelist) == false) {
                        _pal_gap.add_device_to_whitelist(
                            to_whitelist_address_type(device.type),
                            device.address
                        );
                    }
                }

                return err;
            }
        }
    }

    // commit the devices into the whitelist
    for (size_t i = 0; i < whitelist.size; ++i) {
        _whitelist.addresses[i] = whitelist.addresses[i];
    }

    _whitelist.size = whitelist.size;

    return BLE_ERROR_NONE;
}

ble_error_t GenericGap::setAdvertisingPolicyMode(AdvertisingPolicyMode_t mode)
{
    useVersionOneAPI();

    if (mode > ::Gap::ADV_POLICY_FILTER_ALL_REQS) {
        return BLE_ERROR_INVALID_PARAM;
    }

    _advertising_filter_policy = (pal::advertising_filter_policy_t::type) mode;
    return BLE_ERROR_NONE;
}

ble_error_t GenericGap::setScanningPolicyMode(ScanningPolicyMode_t mode)
{
    useVersionOneAPI();

    useVersionOneAPI();

    if (mode > ::Gap::SCAN_POLICY_FILTER_ALL_ADV) {
        return BLE_ERROR_INVALID_PARAM;
    }

    _scanning_filter_policy = (pal::scanning_filter_policy_t::type) mode;
    return BLE_ERROR_NONE;
}

ble_error_t GenericGap::setInitiatorPolicyMode(InitiatorPolicyMode_t mode)
{
    useVersionOneAPI();

    if (mode > ::Gap::INIT_POLICY_FILTER_ALL_ADV) {
        return BLE_ERROR_INVALID_PARAM;
    }

    _initiator_policy_mode = (pal::initiator_policy_t::type) mode;
    return BLE_ERROR_NONE;
}

::Gap::AdvertisingPolicyMode_t GenericGap::getAdvertisingPolicyMode(void) const
{
    useVersionOneAPI();
    return (AdvertisingPolicyMode_t) _advertising_filter_policy.value();
}

::Gap::ScanningPolicyMode_t GenericGap::getScanningPolicyMode(void) const
{
    useVersionOneAPI();
    return (ScanningPolicyMode_t) _scanning_filter_policy.value();
}

::Gap::InitiatorPolicyMode_t GenericGap::getInitiatorPolicyMode(void) const
{
    useVersionOneAPI();
    return (InitiatorPolicyMode_t) _initiator_policy_mode.value();
}

ble_error_t GenericGap::startRadioScan(const GapScanningParams &scanningParams)
{
    useVersionOneAPI();

    if (is_scan_params_valid(&scanningParams) == false) {
        return BLE_ERROR_INVALID_PARAM;
    }

    if (_scanning_filter_policy == pal::scanning_filter_policy_t::FILTER_ADVERTISING &&
        _whitelist.size == 0) {
        return BLE_ERROR_INVALID_STATE;
    }

    pal::own_address_type_t own_address_type = get_own_address_type(CENTRAL_SCAN /* central, can use non resolvable address for scan requests */);

    if (_privacy_enabled && (own_address_type == pal::own_address_type_t::RANDOM)) {
        // Use non-resolvable static random address
        set_random_address_rotation(true);
    }

    ble_error_t err = _pal_gap.set_scan_parameters(
        scanningParams.getActiveScanning(),
        scanningParams.getInterval(),
        scanningParams.getWindow(),
        own_address_type,
        _scanning_filter_policy
    );

    if (err) {
        return err;
    }

    err = _pal_gap.scan_enable(true, false);

    if (err) {
        return err;
    }

    _scan_enabled = true;

    _scan_timeout.detach();
    uint16_t timeout = scanningParams.getTimeout();
    if (timeout) {
        _scan_timeout.attach_us(
            mbed::callback(this, &GenericGap::on_scan_timeout),
            scanningParams.getTimeout() * 1000000U
        );
    }

    return BLE_ERROR_NONE;
}

ble_error_t GenericGap::initRadioNotification(void)
{
    return BLE_ERROR_NOT_IMPLEMENTED;
}

ble_error_t GenericGap::enablePrivacy(bool enable)
{
    if (enable == _privacy_enabled) {
        // No change
        return BLE_ERROR_NONE;
    }

    if (enable && !_pal_gap.is_privacy_supported()) {
        // Privacy is not supported by the implementation
        return BLE_ERROR_NOT_IMPLEMENTED;
    }

    _privacy_enabled = enable;

    update_address_resolution_setting();

    return BLE_ERROR_NONE;
}

ble_error_t GenericGap::setPeripheralPrivacyConfiguration(
    const PeripheralPrivacyConfiguration_t *configuration
)
{
    _peripheral_privacy_configuration = *configuration;

    update_address_resolution_setting();

    return BLE_ERROR_NONE;
}

ble_error_t GenericGap::getPeripheralPrivacyConfiguration(
    PeripheralPrivacyConfiguration_t *configuration
)
{
    *configuration = _peripheral_privacy_configuration;

    return BLE_ERROR_NONE;
}

ble_error_t GenericGap::setCentralPrivacyConfiguration(
    const CentralPrivacyConfiguration_t *configuration
)
{
    _central_privacy_configuration = *configuration;

    update_address_resolution_setting();

    return BLE_ERROR_NONE;
}

ble_error_t GenericGap::getCentralPrivacyConfiguration(
    CentralPrivacyConfiguration_t *configuration
)
{
    *configuration = _central_privacy_configuration;

    return BLE_ERROR_NONE;
}

ble_error_t GenericGap::setAdvertisingData(const GapAdvertisingData &advData, const GapAdvertisingData &scanResponse)
{
    useVersionOneAPI();

    ble_error_t err = _pal_gap.set_advertising_data(
        advData.getPayloadLen(),
        pal::advertising_data_t(advData.getPayload(), advData.getPayloadLen())
    );
    if (err) {
        return err;
    }

    return _pal_gap.set_scan_response_data(
        scanResponse.getPayloadLen(),
        pal::advertising_data_t(scanResponse.getPayload(), scanResponse.getPayloadLen())
    );
}

ble_error_t GenericGap::startAdvertising(const GapAdvertisingParams &params)
{
    useVersionOneAPI();

    if (is_advertising_params_valid(params) == false) {
        return BLE_ERROR_INVALID_PARAM;
    }

    // We can only use non resolvable addresses if the device is non connectable
    AddressUseType_t address_use_type =
        ((params.getAdvertisingType() == GapAdvertisingParams::ADV_SCANNABLE_UNDIRECTED) ||
            (params.getAdvertisingType() == GapAdvertisingParams::ADV_NON_CONNECTABLE_UNDIRECTED)) ?
            PERIPHERAL_NON_CONNECTABLE :
            PERIPHERAL_CONNECTABLE;

    pal::own_address_type_t own_address_type = get_own_address_type(address_use_type);

    if (_privacy_enabled && (own_address_type == pal::own_address_type_t::RANDOM)) {
        // Use non-resolvable static random address
        set_random_address_rotation(true);
    }

    // TODO: fix the high level API to have a min/max range
    // Going against recommendations (The Advertising_Interval_Min and
    // Advertising_Interval_Max should not be the same value to enable the
    // Controller to determine the best advertising interval given other activities.)
    // for now but not against specification: "The Advertising_Interval_Min
    // shall be less than or equal to the Advertising_Interval_Max"
    ble_error_t err = _pal_gap.set_advertising_parameters(
        /* advertising_interval_min */ params.getIntervalInADVUnits(),
        /* advertising_interval_max */ params.getIntervalInADVUnits(),
        (pal::advertising_type_t::type) params.getAdvertisingType(),
        own_address_type,
        pal::advertising_peer_address_type_t::PUBLIC,
        ble::address_t(),
        pal::advertising_channel_map_t::ALL_ADVERTISING_CHANNELS,
        _advertising_filter_policy
    );

    if (err) {
        return err;
    }

    err = _pal_gap.advertising_enable(true);
    if (err) {
        return err;
    }

    state.advertising = true;

    _advertising_timeout.detach();
    uint16_t timeout = params.getTimeout();
    if (timeout) {
        _advertising_timeout.attach_us(
            mbed::callback(this, &GenericGap::on_advertising_timeout),
            params.getTimeout() * 1000000U
        );
    }

    return BLE_ERROR_NONE;
}

ble_error_t GenericGap::reset(void)
{
    ::Gap::reset();

    _advertising_timeout.detach();
    _scan_timeout.detach();

    if (is_extended_advertising_available()) {
        /* stop all advertising sets */
        for (size_t i = 0; i < MAX_ADVERTISING_SETS; ++i) {
            if (_active_sets.get(i)) {
                _pal_gap.extended_advertising_enable(
                    /* enable */ false,
                    /* number of advertising sets */ 1,
                    (advertising_handle_t*)&i,
                    NULL,
                    NULL
                );
            }
            if (_active_periodic_sets.get(i)) {
                _pal_gap.periodic_advertising_enable(
                    /* enable */ false,
                    (advertising_handle_t)i
                );
            }
        }

        /* clear state of all advertising sets */
        _existing_sets.clear();
        _active_sets.clear();
        _active_periodic_sets.clear();

        /* clear advertising set data on the controller */
        _pal_gap.clear_advertising_sets();

        setExtendedAdvertisingParameters(
            LEGACY_ADVERTISING_HANDLE,
            AdvertisingParameters()
        );
    }

    _existing_sets.set(LEGACY_ADVERTISING_HANDLE);

    return BLE_ERROR_NONE;
}

void GenericGap::processConnectionEvent(
    Handle_t handle,
    Role_t role,
    PeerAddressType_t peerAddrType,
    const BLEProtocol::AddressBytes_t peerAddr,
    BLEProtocol::AddressType_t ownAddrType,
    const BLEProtocol::AddressBytes_t ownAddr,
    const ConnectionParams_t *connectionParams,
    const uint8_t *peerResolvableAddr,
    const uint8_t *localResolvableAddr
)
{
    if (_connection_event_handler) {
        _connection_event_handler->on_connected(
            handle,
            role,
            peerAddrType,
            peerAddr,
            ownAddrType,
            ownAddr,
            connectionParams
        );
    }

BLE_DEPRECATED_API_USE_BEGIN()
    ::Gap::processConnectionEvent(
        handle,
        role,
        peerAddrType,
        peerAddr,
        ownAddrType,
        ownAddr,
        connectionParams,
        peerResolvableAddr,
        localResolvableAddr
   );
BLE_DEPRECATED_API_USE_END()
}

void GenericGap::processDisconnectionEvent(
    Handle_t handle,
    DisconnectionReason_t reason
)
{
    if (_connection_event_handler) {
        _connection_event_handler->on_disconnected(
            handle,
            reason
        );
    }

    if (_eventHandler) {
        _eventHandler->onDisconnectionComplete(
            DisconnectionCompleteEvent(
                handle,
                (disconnection_reason_t::type) reason
            )
        );
    }

BLE_DEPRECATED_API_USE_BEGIN()
    ::Gap::processDisconnectionEvent(
        handle,
        reason
    );
BLE_DEPRECATED_API_USE_END()
}

void GenericGap::on_scan_timeout()
{
    if (!_scan_enabled) {
        return;
    }

    /* if timeout happened on a 4.2 chip we need to stop the scan manually */
    if (!is_extended_advertising_available()) {
        _pal_gap.scan_enable(false, false);
        set_random_address_rotation(false);
    }

    _scan_enabled = false;

    if (!_eventHandler) {
        return;
    }

    _eventHandler->onScanTimeout(ScanTimeoutEvent());
}

void GenericGap::process_scan_timeout()
{
    ble_error_t err = _pal_gap.scan_enable(false, false);
    if (err) {
        // TODO: define the mechanism signaling the error
    }
BLE_DEPRECATED_API_USE_BEGIN()
    processTimeoutEvent(::Gap::TIMEOUT_SRC_SCAN);
BLE_DEPRECATED_API_USE_END()
}

void GenericGap::on_advertising_timeout()
{
    _event_queue.post(mbed::callback(this, &GenericGap::process_advertising_timeout));
}

void GenericGap::process_advertising_timeout()
{
    ble_error_t err = _pal_gap.advertising_enable(false);
    if (err) {
        // TODO: define the mechanism signaling the error
    }

    // Stop address rotation if required
    set_random_address_rotation(false);

BLE_DEPRECATED_API_USE_BEGIN()
    processTimeoutEvent(::Gap::TIMEOUT_SRC_ADVERTISING);
BLE_DEPRECATED_API_USE_END()
}

void GenericGap::on_gap_event_received(const pal::GapEvent &e)
{
    switch (e.type.value()) {
        case pal::GapEventType::ADVERTISING_REPORT:
            on_advertising_report(static_cast<const pal::GapAdvertisingReportEvent &>(e));
            break;

        case pal::GapEventType::CONNECTION_COMPLETE:
            on_connection_complete(static_cast<const pal::GapConnectionCompleteEvent &>(e));
            break;

        case pal::GapEventType::CONNECTION_UPDATE:
            on_connection_update(static_cast<const pal::GapConnectionUpdateEvent &>(e));
            break;

        case pal::GapEventType::DISCONNECTION_COMPLETE:
            on_disconnection_complete(static_cast<const pal::GapDisconnectionCompleteEvent &>(e));
            break;

        case pal::GapEventType::REMOTE_CONNECTION_PARAMETER_REQUEST:
            on_connection_parameter_request(static_cast<const pal::GapRemoteConnectionParameterRequestEvent &>(e));
            break;

        case pal::GapEventType::UNEXPECTED_ERROR:
            on_unexpected_error(static_cast<const pal::GapUnexpectedErrorEvent &>(e));
            break;

        default:
            break;
    }
}

void GenericGap::on_advertising_report(const pal::GapAdvertisingReportEvent &e)
{
    for (size_t i = 0; i < e.size(); ++i) {
        pal::GapAdvertisingReportEvent::advertising_t advertising = e[i];

        // Check if the address hasn't been resolved
        if (_privacy_enabled &&
            _central_privacy_configuration.resolution_strategy == CentralPrivacyConfiguration_t::RESOLVE_AND_FILTER &&
            advertising.address_type == pal::connection_peer_address_type_t::RANDOM_ADDRESS &&
            is_random_private_resolvable_address(advertising.address.data())
        ) {
            // Filter it out
            continue;
        }

        // note 1-to-1 conversion between connection_peer_address_type_t and
        // peer_address_type_t
        peer_address_type_t peer_address_type =
            static_cast<peer_address_type_t::type>(advertising.address_type.value());

        // report in new event handler
        if (_eventHandler) {
            uint8_t event_type = 0;

            // Conversion table available at BLUETOOTH SPECIFICATION Version 5.0 | Vol 2, Part E
            // 7.7.65.13
            switch (advertising.type.value()) {
                case pal::received_advertising_type_t::ADV_IND:
                    event_type = 0x13;
                    break;
                case pal::received_advertising_type_t::ADV_DIRECT_IND:
                    event_type = 0x15;
                    break;
                case pal::received_advertising_type_t::ADV_SCAN_IND:
                    event_type = 0x12;
                    break;
                case pal::received_advertising_type_t::ADV_NONCONN_IND:
                    event_type = 0x10;
                    break;
                case pal::received_advertising_type_t::SCAN_RESPONSE:
                    event_type = 0x1B;
                    break;
            }

            _eventHandler->onAdvertisingReport(
                AdvertisingReportEvent(
                    advertising_event_t(event_type),
                    peer_address_type,
                    advertising.address,
                    /* primary */ phy_t::LE_1M,
                    /* secondary */ phy_t::NONE,
                    /* SID - NO ADI FIELD IN THE PDU */ 0xFF,
                    /* tx power information not available */ 127,
                    advertising.rssi,
                    /* NO PERIODIC ADVERTISING */ 0,
                    peer_address_type_t::ANONYMOUS,
                    ble::address_t (),
                    mbed::Span<const uint8_t>(advertising.data.data(), advertising.data.size())
                )
            );
        }

BLE_DEPRECATED_API_USE_BEGIN()
        processAdvertisementReport(
            advertising.address.data(),
            advertising.rssi,
            advertising.type == pal::received_advertising_type_t::SCAN_RESPONSE,
            (GapAdvertisingParams::AdvertisingType_t) advertising.type.value(),
            advertising.data.size(),
            advertising.data.data(),
            peer_address_type
        );
BLE_DEPRECATED_API_USE_END()
    }
}

void GenericGap::on_connection_complete(const pal::GapConnectionCompleteEvent &e)
{
    if (e.status != pal::hci_error_code_t::SUCCESS) {
        if (_eventHandler) {
            _eventHandler->onConnectionComplete(
                ConnectionCompleteEvent(
                    BLE_ERROR_NOT_FOUND,
                    INVALID_ADVERTISING_HANDLE,
                    connection_role_t::CENTRAL,
                    peer_address_type_t::ANONYMOUS,
                    ble::address_t(),
                    ble::address_t(),
                    ble::address_t(),
                    ble::conn_interval_t::max(),
                    /* dummy slave latency */ 0,
                    ble::supervision_timeout_t::max(),
                    /* master clock accuracy */ 0
                )
            );
        }

        // for now notify user that the connection failled by issuing a timeout
        // event

        // TODO: Define events in case of connection faillure
BLE_DEPRECATED_API_USE_BEGIN()
        processTimeoutEvent(::Gap::TIMEOUT_SRC_CONN);
BLE_DEPRECATED_API_USE_END()
        return;
    }

    bool needs_pairing = false;
    bool needs_authentication = false;

    if (_privacy_enabled &&
        e.role.value() == e.role.PERIPHERAL &&
        e.peer_address_type == peer_address_type_t::RANDOM
    ) {
        // Apply privacy policy if in peripheral mode for non-resolved addresses
        RandomAddressType_t random_address_type(RandomAddressType_t::RESOLVABLE_PRIVATE);
        ble_error_t err = getRandomAddressType(e.peer_address.data(), &random_address_type);
        if (err) {
            // FIXME: return for now; needs to report the error ?
            return;
        }

        if (random_address_type == RandomAddressType_t::RESOLVABLE_PRIVATE) {
            switch (_peripheral_privacy_configuration.resolution_strategy) {
                case PeripheralPrivacyConfiguration_t::REJECT_NON_RESOLVED_ADDRESS:
                    // Reject connection request - the user will get notified through a callback
                    _pal_gap.disconnect(
                        e.connection_handle,
                        pal::disconnection_reason_t::AUTHENTICATION_FAILURE
                    );
                    return;

                case PeripheralPrivacyConfiguration_t::PERFORM_PAIRING_PROCEDURE:
                    needs_pairing = true;
                    break;

                case PeripheralPrivacyConfiguration_t::PERFORM_AUTHENTICATION_PROCEDURE:
                    needs_authentication = true;
                    break;

                default:
                    break;
            }
        }
    }

    if (e.role.value() == e.role.PERIPHERAL) {
        _advertising_timeout.detach();
        _pal_gap.advertising_enable(false);

        // Stop address rotation if required
        set_random_address_rotation(false);
    }

    // using these parameters if stupid, there is no range for the
    // connection interval when the connection is established
    ConnectionParams_t connection_params = {
        /* minConnectionInterval */ e.connection_interval,
        /* maxConnectionInterval */ e.connection_interval,
        e.connection_latency,
        e.supervision_timeout
    };

    ble::address_t address;
    if (_address_type == LegacyAddressType::PUBLIC) {
        address = _pal_gap.get_device_address();
    } else {
        address = _pal_gap.get_random_address();
    }

    // new process event
    if (_eventHandler) {
        _eventHandler->onConnectionComplete(
            ConnectionCompleteEvent(
                BLE_ERROR_NONE,
                e.connection_handle,
                e.role,
                e.peer_address_type,
                e.peer_address,
                e.local_resolvable_private_address,
                e.peer_resolvable_private_address,
                conn_interval_t(e.connection_interval),
                e.connection_latency,
                supervision_timeout_t(e.supervision_timeout),
                /* default master clock accuracy */ ble::clock_accuracy_t::PPM_500
            )
        );
    }

    // legacy process event
    processConnectionEvent(
        e.connection_handle,
        e.role.value() == e.role.CENTRAL ? ::Gap::CENTRAL : ::Gap::PERIPHERAL,
        e.peer_address_type,
        e.peer_address.data(),
        _address_type,
        address.data(),
        &connection_params,
        e.local_resolvable_private_address.data(),
        e.peer_resolvable_private_address.data()
    );

    // Now starts pairing or authentication procedures if required
    if (needs_pairing) {
        SecurityManager &sm = createBLEInstance()->getSecurityManager();
        // Request authentication to start pairing procedure
        sm.requestAuthentication(e.connection_handle);
    } else if (needs_authentication) {
        // TODO: GAP Authentication != Security Manager authentication
        // Needs to be implemented
    }
}

void GenericGap::on_disconnection_complete(const pal::GapDisconnectionCompleteEvent &e)
{
    if (e.status == pal::hci_error_code_t::SUCCESS) {
        processDisconnectionEvent(
            e.connection_handle,
            (::Gap::DisconnectionReason_t) e.reason
        );
    } else {
        // TODO: define what to do in case of failure
    }
}

void GenericGap::on_connection_parameter_request(const pal::GapRemoteConnectionParameterRequestEvent &e)
{
    if (_user_manage_connection_parameter_requests) {
        if (_eventHandler) {
            _eventHandler->onUpdateConnectionParametersRequest(
                UpdateConnectionParametersRequestEvent(
                    e.connection_handle,
                    conn_interval_t(e.min_connection_interval),
                    conn_interval_t(e.max_connection_interval),
                    e.connection_latency,
                    supervision_timeout_t(e.supervision_timeout)
                )
            );
        } else {
            MBED_ERROR(illegal_state_error, "Event handler required if connection params are user handled");
        }
    } else {
        _pal_gap.accept_connection_parameter_request(
            e.connection_handle,
            e.min_connection_interval,
            e.max_connection_interval,
            e.connection_latency,
            e.supervision_timeout,
            /* minimum_connection_event_length */ 0,
            /* maximum_connection_event_length */ 0
        );
    }
}

void GenericGap::on_connection_update(const pal::GapConnectionUpdateEvent &e)
{
    if (!_eventHandler) {
        return;
    }

    _eventHandler->onConnectionParametersUpdateComplete(
        ConnectionParametersUpdateCompleteEvent(
            e.status == pal::hci_error_code_t::SUCCESS ? BLE_ERROR_NONE : BLE_ERROR_UNSPECIFIED,
            e.connection_handle,
            conn_interval_t(e.connection_interval),
            e.connection_latency,
            supervision_timeout_t(e.supervision_timeout)
        )
    );
}

void GenericGap::on_unexpected_error(const pal::GapUnexpectedErrorEvent &e)
{
    // TODO: add feature in interface to notify the user that the connection
    // has been updated.
}

pal::own_address_type_t GenericGap::get_own_address_type(AddressUseType_t address_use_type)
{
    if (_privacy_enabled) {
        bool use_non_resolvable_address = false;
        if (address_use_type == CENTRAL_SCAN) {
            use_non_resolvable_address = _central_privacy_configuration.use_non_resolvable_random_address;
        } else if (address_use_type == PERIPHERAL_NON_CONNECTABLE) {
            use_non_resolvable_address = _peripheral_privacy_configuration.use_non_resolvable_random_address;
        }

        // An non resolvable private address should be generated
        if (use_non_resolvable_address) {
            return pal::own_address_type_t::RANDOM;
        }

        switch (_address_type) {
            case BLEProtocol::AddressType::PUBLIC:
                return pal::own_address_type_t::RESOLVABLE_PRIVATE_ADDRESS_PUBLIC_FALLBACK;
            default:
                return pal::own_address_type_t::RESOLVABLE_PRIVATE_ADDRESS_RANDOM_FALLBACK;
        }
    }

    switch (_address_type) {
        case BLEProtocol::AddressType::PUBLIC:
            return pal::own_address_type_t::PUBLIC;
        default:
            return pal::own_address_type_t::RANDOM;
    }
}

bool GenericGap::initialize_whitelist() const
{
    if (_whitelist.addresses != NULL) {
        return true;
    }

    uint8_t whitelist_capacity = _pal_gap.read_white_list_capacity();

    if (whitelist_capacity == 0) {
        return false;
    }

    _whitelist.addresses = new(std::nothrow) BLEProtocol::Address_t[whitelist_capacity];
    if (_whitelist.addresses == NULL) {
        return false;
    }

    _whitelist.size = 0;
    _whitelist.capacity = whitelist_capacity;

    return true;
}

ble_error_t GenericGap::update_address_resolution_setting()
{
    // Only disable if privacy is disabled or resolution is not requested in either central or peripheral mode
    bool enable = true;

    if (!_privacy_enabled) {
        enable = false;
    }
    else if( (_peripheral_privacy_configuration.resolution_strategy == PeripheralPrivacyConfiguration_t::DO_NOT_RESOLVE)
        && (_central_privacy_configuration.resolution_strategy == CentralPrivacyConfiguration_t::DO_NOT_RESOLVE) ) {
        enable = false;
    }

    return _pal_gap.set_address_resolution(enable);
}

void GenericGap::set_random_address_rotation(bool enable)
{
    if (enable == _random_address_rotating) {
        return;
    }

    _random_address_rotating = enable;

    if (enable) {
        // Set first address
        update_random_address();

        // Schedule rotations every 15 minutes as recomended by the spec
        _address_rotation_ticker.attach_us(
            mbed::callback(this, &GenericGap::on_address_rotation_timeout),
            15 * 60 * 1000000U
        );
    } else {
        // Stop ticker
        _address_rotation_ticker.detach();

        // Set static random identity address
        _pal_gap.set_random_address(
            _random_static_identity_address
        );
    }
}

void GenericGap::update_random_address()
{
    if (!_random_address_rotating) {
        // This event might have been queued before we disabled address rotation
        return;
    }

    if (is_extended_advertising_available()) {
        for (uint8_t i = 0; i < MAX_ADVERTISING_SETS; ++i) {
            if (_existing_sets.get(i)) {
                ble::address_t address;

                if (!getUnresolvableRandomAddress(address)) {
                    return;
                }

                /* ignore the error, if it fails to cycle because it's connectable */
                _pal_gap.set_advertising_set_random_address(
                    (advertising_handle_t) i,
                    address
                );
            }
        }
    }

    ble::address_t address;

    if (!getUnresolvableRandomAddress(address)) {
        return;
    }

    ble_error_t err = _pal_gap.set_random_address(
        address
    );

    if (err) {
        return;
    }

    _address_type = LegacyAddressType::RANDOM_PRIVATE_NON_RESOLVABLE;
    _address = address;
}

bool GenericGap::getUnresolvableRandomAddress(ble::address_t &address)
{
    do {
        byte_array_t<8> random_data;

        ble_error_t ret = _pal_sm.get_random_data(random_data);
        if (ret != BLE_ERROR_NONE) {
            // Abort
            return false;
        }

        // Build a non-resolvable private address as specified in the Core 4.2 spec, Vol 6, Part B, 1.3.2.2
        // Mask out two MSbs
        random_data[5] &= 0x3F;

        // Copy to address - will copy first 6 bytes
        address = ble::address_t(random_data.data());

        if (!is_random_private_non_resolvable_address(address.data())) {
            // If address is invalid, which is unlikely (all 0s or all 1s), try again
            // If implementation is faulty, we'll get stuck here
            continue;
        }

        // Address is valid
        break;
    }
    while (true);

    return true;
}

void GenericGap::on_address_rotation_timeout()
{
    _event_queue.post(mbed::callback(this, &GenericGap::update_random_address));
}

void GenericGap::set_connection_event_handler(pal::ConnectionEventMonitor::EventHandler *connection_event_handler)
{
    _connection_event_handler = connection_event_handler;
}

const uint8_t GenericGap::MAX_ADVERTISING_SETS;

const size_t GenericGap::MAX_HCI_DATA_LENGTH;

uint8_t GenericGap::getMaxAdvertisingSetNumber()
{
    useVersionTwoAPI();

    if (is_extended_advertising_available()) {
        uint8_t set_number = _pal_gap.get_max_number_of_advertising_sets();
        return std::min(MAX_ADVERTISING_SETS, set_number);
    } else {
        return 1;
    }
}

uint16_t GenericGap::getMaxAdvertisingDataLength()
{
    useVersionTwoAPI();
    return _pal_gap.get_maximum_advertising_data_length();
}

ble_error_t GenericGap::createAdvertisingSet(
    advertising_handle_t *handle,
    const AdvertisingParameters &parameters
)
{
    useVersionTwoAPI();

    if (is_extended_advertising_available() == false) {
        return BLE_ERROR_OPERATION_NOT_PERMITTED;
    }

    uint8_t new_handle = LEGACY_ADVERTISING_HANDLE + 1;
    uint8_t end = getMaxAdvertisingSetNumber();

    for (; new_handle < end; ++new_handle) {
        if (!_existing_sets.get(new_handle)) {
            ble_error_t err = setExtendedAdvertisingParameters(
                new_handle,
                parameters
            );
            if (err) {
                return err;
            }

            _existing_sets.set(new_handle);
            *handle = new_handle;
            return BLE_ERROR_NONE;
        }
    }

    *handle = INVALID_ADVERTISING_HANDLE;
    return BLE_ERROR_NO_MEM;
}

ble_error_t GenericGap::destroyAdvertisingSet(advertising_handle_t handle)
{
    useVersionTwoAPI();

    if (is_extended_advertising_available() == false) {
        return BLE_ERROR_OPERATION_NOT_PERMITTED;
    }

    if (handle == LEGACY_ADVERTISING_HANDLE) {
        return BLE_ERROR_INVALID_PARAM;
    }

    if (handle >= getMaxAdvertisingSetNumber()) {
        return BLE_ERROR_INVALID_PARAM;
    }

    if (!_existing_sets.get(handle)) {
        return BLE_ERROR_INVALID_PARAM;
    }

    if (_active_sets.get(handle)) {
        return BLE_ERROR_OPERATION_NOT_PERMITTED;
    }

    if (_active_periodic_sets.get(handle)) {
        return BLE_ERROR_OPERATION_NOT_PERMITTED;
    }

    ble_error_t err = _pal_gap.remove_advertising_set(handle);
    if (err) {
        return err;
    }

    _existing_sets.clear(handle);
    return BLE_ERROR_NONE;
}

ble_error_t GenericGap::setAdvertisingParameters(
    advertising_handle_t handle,
    const AdvertisingParameters &params
)
{
    useVersionTwoAPI();

    if (handle >= getMaxAdvertisingSetNumber()) {
        return BLE_ERROR_INVALID_PARAM;
    }

    if (!_existing_sets.get(handle)) {
        return BLE_ERROR_INVALID_PARAM;
    }

    if (is_extended_advertising_available()) {
        return setExtendedAdvertisingParameters(handle, params);
    } else {
        if (handle != LEGACY_ADVERTISING_HANDLE) {
            return BLE_ERROR_INVALID_PARAM;
        }

        pal::advertising_channel_map_t channel_map(
            params.getChannel37(),
            params.getChannel38(),
            params.getChannel39()
        );

        return _pal_gap.set_advertising_parameters(
            params.getMinPrimaryInterval().value(),
            params.getMaxPrimaryInterval().value(),
            params.getType(),
            params.getOwnAddressType(),
            params.getPeerAddressType(),
            params.getPeerAddress(),
            channel_map,
            params.getFilter()
        );
    }
}

ble_error_t GenericGap::setExtendedAdvertisingParameters(
    advertising_handle_t handle,
    const AdvertisingParameters &params
)
{
    if (handle >= getMaxAdvertisingSetNumber()) {
        return BLE_ERROR_INVALID_PARAM;
    }

    pal::advertising_event_properties_t event_properties(params.getType());
    event_properties.include_tx_power = params.getTxPowerInHeader();
    event_properties.omit_advertiser_address = params.getAnonymousAdvertising();
    event_properties.use_legacy_pdu = params.getUseLegacyPDU();

    pal::advertising_channel_map_t channel_map(
        params.getChannel37(),
        params.getChannel38(),
        params.getChannel39()
    );

    ble_error_t err = _pal_gap.set_extended_advertising_parameters(
        handle,
        event_properties,
        params.getMinPrimaryInterval().value(),
        params.getMaxPrimaryInterval().value(),
        channel_map,
        params.getOwnAddressType(),
        params.getPeerAddressType(),
        params.getPeerAddress(),
        params.getFilter(),
        params.getTxPower(),
        params.getPrimaryPhy(),
        params.getSecondaryMaxSkip(),
        params.getSecondaryPhy(),
        /* SID */ (handle % 0x10),
        params.getScanRequestNotification()
    );

    if (err) {
        return err;
    }

    return _pal_gap.set_advertising_set_random_address(
        handle,
        _random_static_identity_address
    );
}

ble_error_t GenericGap::setAdvertisingPayload(
    advertising_handle_t handle,
    mbed::Span<const uint8_t> payload
)
{
    useVersionTwoAPI();

    return setAdvertisingData(
        handle,
        payload,
        /* minimise fragmentation */ true,
        /* scan response */ false
    );
}

ble_error_t GenericGap::setAdvertisingScanResponse(
    advertising_handle_t handle,
    mbed::Span<const uint8_t> response
)
{
    useVersionTwoAPI();

    return setAdvertisingData(
        handle,
        response,
        /* minimise fragmentation */ true,
        /* scan response */ true
    );
}

ble_error_t GenericGap::setAdvertisingData(
    advertising_handle_t handle,
    mbed::Span<const uint8_t> payload,
    bool minimiseFragmentation,
    bool scan_response
)
{
    // type declarations
    typedef pal::advertising_fragment_description_t op_t;
    typedef ble_error_t (pal::Gap::*legacy_set_data_fn_t)(
        uint8_t,
        const pal::advertising_data_t &
    );
    typedef ble_error_t (pal::Gap::*set_data_fn_t)(
        advertising_handle_t advertising_handle,
        op_t operation,
        bool minimize_fragmentation,
        uint8_t scan_response_data_size,
        const uint8_t *scan_response_data
    );

    if (handle >= getMaxAdvertisingSetNumber()) {
        return BLE_ERROR_INVALID_PARAM;
    }

    if (!_existing_sets.get(handle)) {
        return BLE_ERROR_INVALID_PARAM;
    }

    // handle special case of legacy advertising
    if (is_extended_advertising_available() == false) {
        if (handle != LEGACY_ADVERTISING_HANDLE) {
            return BLE_ERROR_INVALID_PARAM;
        }

        if (payload.size() > GAP_ADVERTISING_DATA_MAX_PAYLOAD) {
            return BLE_ERROR_INVALID_PARAM;
        }

        // select the pal function
        legacy_set_data_fn_t set_data = scan_response ?
            &pal::Gap::set_scan_response_data :
            &pal::Gap::set_advertising_data;

        // set the payload
        return (_pal_gap.*set_data)(
            payload.size(),
            pal::advertising_data_t(payload.data(), payload.size())
        );
    }

    if (payload.size() > getMaxAdvertisingDataLength()) {
        return BLE_ERROR_INVALID_PARAM;
    }

    // select the pal function
    set_data_fn_t set_data = scan_response ?
        &pal::Gap::set_extended_scan_response_data :
        &pal::Gap::set_extended_advertising_data;

    for (size_t i = 0, end = payload.size();
        (i < end) || (i == 0 && end == 0);
        i += MAX_HCI_DATA_LENGTH)
    {
        // select the operation based on the index
        op_t op(op_t::INTERMEDIATE_FRAGMENT);
        if (end < MAX_HCI_DATA_LENGTH) {
            op = op_t::COMPLETE_FRAGMENT;
        } else if (i == 0) {
            op = op_t::FIRST_FRAGMENT;
        } else if ((end - i) <= MAX_HCI_DATA_LENGTH) {
            op = op_t::LAST_FRAGMENT;
        }

        // extract the payload
        mbed::Span<const uint8_t> sub_payload = payload.subspan(
            i,
            std::min(MAX_HCI_DATA_LENGTH, (end - i))
        );

        // set the payload
        ble_error_t err = (_pal_gap.*set_data)(
            handle,
            op,
            minimiseFragmentation,
            sub_payload.size(),
            sub_payload.data()
        );

        if (err) {
            return err;
        }
    }
    return BLE_ERROR_NONE;
}

ble_error_t GenericGap::startAdvertising(
    advertising_handle_t handle,
    adv_duration_t maxDuration,
    uint8_t maxEvents
)
{
    useVersionTwoAPI();

    ble_error_t error = BLE_ERROR_NONE;

    if (handle >= getMaxAdvertisingSetNumber()) {
        return BLE_ERROR_INVALID_PARAM;
    }

    if (!_existing_sets.get(handle)) {
        return BLE_ERROR_INVALID_PARAM;
    }

    if (is_extended_advertising_available()) {
        error = _pal_gap.extended_advertising_enable(
            /* enable */ true,
            /* number of advertising sets */ 1,
            &handle,
            maxDuration.storage(),
            &maxEvents
        );

        if (error) {
            return error;
        }
    } else {
        if (handle != LEGACY_ADVERTISING_HANDLE) {
            return BLE_ERROR_INVALID_PARAM;
        }

        error = _pal_gap.advertising_enable(true);
        if (error) {
            return error;
        }

        _advertising_timeout.detach();
        if (maxDuration.value()) {
            _advertising_timeout.attach_us(
                mbed::callback(this, &GenericGap::on_advertising_timeout),
                durationCast<millisecond_t>(maxDuration).value()
            );
        }
    }

    _active_sets.set(handle);

    return error;
}

ble_error_t GenericGap::stopAdvertising(advertising_handle_t handle)
{
    useVersionTwoAPI();

    if (handle >= getMaxAdvertisingSetNumber()) {
        return BLE_ERROR_INVALID_PARAM;
    }

    if (!_existing_sets.get(handle)) {
        return BLE_ERROR_INVALID_PARAM;
    }

    if (!_active_sets.get(handle)) {
        return BLE_ERROR_INVALID_STATE;
    }

    ble_error_t status;

    if (is_extended_advertising_available()) {
        status = _pal_gap.extended_advertising_enable(
            /*enable ? */ false,
            /* number of advertising sets */ 1,
            &handle,
            NULL,
            NULL
        );

        if (status) {
            return status;
        }
    } else {
        if (handle != LEGACY_ADVERTISING_HANDLE) {
            return BLE_ERROR_INVALID_PARAM;
        }

        status = _pal_gap.advertising_enable(false);

        if (status) {
            return status;
        }

        _advertising_timeout.detach();
    }

    _active_sets.clear(handle);

    return status;
}

bool GenericGap::isAdvertisingActive(advertising_handle_t handle)
{
    useVersionTwoAPI();

    if (handle >= getMaxAdvertisingSetNumber()) {
        return BLE_ERROR_INVALID_PARAM;
    }

    return _active_sets.get(handle);
}

ble_error_t GenericGap::setPeriodicAdvertisingParameters(
    advertising_handle_t handle,
    periodic_interval_t periodicAdvertisingIntervalMin,
    periodic_interval_t periodicAdvertisingIntervalMax,
    bool advertiseTxPower
)
{
    useVersionTwoAPI();

    if (periodicAdvertisingIntervalMin.value() > periodicAdvertisingIntervalMax.value()) {
        return BLE_ERROR_INVALID_PARAM;
    }

    if (handle == LEGACY_ADVERTISING_HANDLE) {
        return BLE_ERROR_INVALID_PARAM;
    }

    if (handle >= getMaxAdvertisingSetNumber()) {
        return BLE_ERROR_INVALID_PARAM;
    }

    if (!_existing_sets.get(handle)) {
        return BLE_ERROR_INVALID_PARAM;
    }

    return _pal_gap.set_periodic_advertising_parameters(
        handle,
        periodicAdvertisingIntervalMin.value(),
        periodicAdvertisingIntervalMax.value(),
        advertiseTxPower
    );
}

ble_error_t GenericGap::setPeriodicAdvertisingPayload(
    advertising_handle_t handle,
    mbed::Span<const uint8_t> payload
)
{
    useVersionTwoAPI();

    if (handle == LEGACY_ADVERTISING_HANDLE) {
        return BLE_ERROR_INVALID_PARAM;
    }

    if (handle >= getMaxAdvertisingSetNumber()) {
        return BLE_ERROR_INVALID_PARAM;
    }

    if (!_existing_sets.get(handle)) {
        return BLE_ERROR_INVALID_PARAM;
    }

    if (payload.size() > getMaxAdvertisingDataLength()) {
        return BLE_ERROR_INVALID_PARAM;
    }

    typedef pal::advertising_fragment_description_t op_t;

    for (size_t i = 0, end = payload.size();
        (i < end) || (i == 0 && end == 0);
        i += MAX_HCI_DATA_LENGTH
    ) {
        // select the operation based on the index
        op_t op(op_t::INTERMEDIATE_FRAGMENT);
        if (end < MAX_HCI_DATA_LENGTH) {
            op = op_t::COMPLETE_FRAGMENT;
        } else if (i == 0) {
            op = op_t::FIRST_FRAGMENT;
        } else if ((end - i) <= MAX_HCI_DATA_LENGTH) {
            op = op_t::LAST_FRAGMENT;
        }

        // extract the payload
        mbed::Span<const uint8_t> sub_payload = payload.subspan(
            i,
            std::min(MAX_HCI_DATA_LENGTH, (end - i))
        );

        ble_error_t err = _pal_gap.set_periodic_advertising_data(
            handle,
            op,
            sub_payload.size(),
            sub_payload.data()
        );

        if (err) {
            return err;
        }
    }

    return BLE_ERROR_NONE;
}

ble_error_t GenericGap::startPeriodicAdvertising(advertising_handle_t handle)
{
    useVersionTwoAPI();

    if (handle == LEGACY_ADVERTISING_HANDLE) {
        return BLE_ERROR_INVALID_PARAM;
    }

    if (handle >= getMaxAdvertisingSetNumber()) {
        return BLE_ERROR_INVALID_PARAM;
    }

    if (!_existing_sets.get(handle)) {
        return BLE_ERROR_INVALID_PARAM;
    }

    if (_active_sets.get(handle) == false) {
        return BLE_ERROR_INVALID_STATE;
    }

    if (_active_periodic_sets.get(handle) == true) {
        return BLE_ERROR_INVALID_STATE;
    }

    ble_error_t err = _pal_gap.periodic_advertising_enable(true, handle);
    if (err) {
        return err;
    }

    _active_periodic_sets.set(handle);
    return BLE_ERROR_NONE;
}

ble_error_t GenericGap::stopPeriodicAdvertising(advertising_handle_t handle)
{
    useVersionTwoAPI();

    if (handle == LEGACY_ADVERTISING_HANDLE) {
        return BLE_ERROR_INVALID_PARAM;
    }

    if (handle >= getMaxAdvertisingSetNumber()) {
        return BLE_ERROR_INVALID_PARAM;
    }

    if (!_existing_sets.get(handle)) {
        return BLE_ERROR_INVALID_PARAM;
    }

    if (_active_periodic_sets.get(handle) == false) {
        return BLE_ERROR_INVALID_STATE;
    }

    ble_error_t err = _pal_gap.periodic_advertising_enable(false, handle);
    if (err) {
        return err;
    }

    _active_periodic_sets.clear(handle);
    return BLE_ERROR_NONE;
}

bool GenericGap::isPeriodicAdvertisingActive(advertising_handle_t handle)
{
    useVersionTwoAPI();

    if (handle >= getMaxAdvertisingSetNumber()) {
        return BLE_ERROR_INVALID_PARAM;
    }

    return _active_periodic_sets.get(handle);
}

void GenericGap::on_enhanced_connection_complete(
    pal::hci_error_code_t status,
    connection_handle_t connection_handle,
    pal::connection_role_t own_role,
    connection_peer_address_type_t peer_address_type,
    const ble::address_t &peer_address,
    const ble::address_t &local_resolvable_private_address,
    const ble::address_t &peer_resolvable_private_address,
    uint16_t connection_interval,
    uint16_t connection_latency,
    uint16_t supervision_timeout,
    pal::clock_accuracy_t master_clock_accuracy
)
{
    if (!_eventHandler) {
        return;
    }

    _eventHandler->onConnectionComplete(
        ConnectionCompleteEvent(
            (status == pal::hci_error_code_t::SUCCESS) ? BLE_ERROR_NONE : BLE_ERROR_INTERNAL_STACK_FAILURE,
            (connection_handle_t) connection_handle,
            own_role,
            (peer_address_type_t::type) peer_address_type.value(),
            peer_address,
            local_resolvable_private_address,
            peer_resolvable_private_address,
            conn_interval_t(connection_interval),
            connection_latency,
            supervision_timeout_t(supervision_timeout),
            master_clock_accuracy.get_ppm()
        )
    );
}

void GenericGap::on_extended_advertising_report(
    advertising_event_t event_type,
    const connection_peer_address_type_t *address_type,
    const ble::address_t &address,
    phy_t primary_phy,
    const phy_t *secondary_phy,
    advertising_sid_t advertising_sid,
    pal::advertising_power_t tx_power,
    pal::rssi_t rssi,
    uint16_t periodic_advertising_interval,
    pal::direct_address_type_t direct_address_type,
    const ble::address_t &direct_address,
    uint8_t data_length,
    const uint8_t *data
)
{
    if (!_eventHandler) {
        return;
    }

    _eventHandler->onAdvertisingReport(
        AdvertisingReportEvent(
            event_type,
            address_type ?
                (peer_address_type_t::type) address_type->value() :
                peer_address_type_t::ANONYMOUS,
            (BLEProtocol::AddressBytes_t &) address,
            primary_phy,
            secondary_phy ? *secondary_phy : phy_t::NONE,
            advertising_sid,
            tx_power,
            rssi,
            periodic_advertising_interval,
            (PeerAddressType_t::type) direct_address_type.value(),
            (BLEProtocol::AddressBytes_t &) direct_address,
            mbed::make_Span(data, data_length)
        )
    );
}

void GenericGap::on_periodic_advertising_sync_established(
    pal::hci_error_code_t error,
    pal::sync_handle_t sync_handle,
    advertising_sid_t advertising_sid,
    connection_peer_address_type_t advertiser_address_type,
    const ble::address_t &advertiser_address,
    phy_t advertiser_phy,
    uint16_t periodic_advertising_interval,
    pal::clock_accuracy_t clock_accuracy
)
{
    if (!_eventHandler) {
        return;
    }

    _eventHandler->onPeriodicAdvertisingSyncEstablished(
        PeriodicAdvertisingSyncEstablishedEvent(
            (error == pal::hci_error_code_t::SUCCESS) ? BLE_ERROR_NONE : BLE_ERROR_INTERNAL_STACK_FAILURE,
            sync_handle,
            advertising_sid,
            static_cast<PeerAddressType_t::type>(advertiser_address_type.value()),
            advertiser_address,
            advertiser_phy,
            periodic_advertising_interval,
            clock_accuracy
        )
    );

}

void GenericGap::on_periodic_advertising_report(
    pal::sync_handle_t sync_handle,
    pal::advertising_power_t tx_power,
    pal::rssi_t rssi,
    pal::advertising_data_status_t data_status,
    uint8_t data_length,
    const uint8_t *data
)
{
    if (!_eventHandler) {
        return;
    }

    _eventHandler->onPeriodicAdvertisingReport(
        PeriodicAdvertisingReportEvent(
            sync_handle,
            tx_power,
            rssi,
            data_status,
            mbed::make_const_Span(data, data_length)
        )
    );
}

void GenericGap::on_periodic_advertising_sync_loss(pal::sync_handle_t sync_handle)
{
    if (!_eventHandler) {
        return;
    }

    _eventHandler->onPeriodicAdvertisingSyncLoss(
        PeriodicAdvertisingSyncLoss(sync_handle)
    );
}

void GenericGap::on_advertising_set_terminated(
    pal::hci_error_code_t status,
    advertising_handle_t advertising_handle,
    connection_handle_t connection_handle,
    uint8_t number_of_completed_extended_advertising_events
)
{
    _active_sets.clear(advertising_handle);

    if (!_eventHandler) {
        return;
    }

    _eventHandler->onAdvertisingEnd(
        AdvertisingEndEvent(
            advertising_handle,
            connection_handle,
            number_of_completed_extended_advertising_events,
            status == pal::hci_error_code_t::SUCCESS
        )
    );
}

void GenericGap::on_scan_request_received(
    advertising_handle_t advertising_handle,
    connection_peer_address_type_t scanner_address_type,
    const ble::address_t &address
)
{
    if (!_eventHandler) {
        return;
    }

    _eventHandler->onScanRequestReceived(
        ScanRequestEvent(
            advertising_handle,
            (peer_address_type_t::type) scanner_address_type.value(),
            address
        )
    );
}

void GenericGap::on_connection_update_complete(
    pal::hci_error_code_t status,
    connection_handle_t connection_handle,
    uint16_t connection_interval,
    uint16_t connection_latency,
    uint16_t supervision_timeout
)
{
    if (!_eventHandler) {
        return;
    }

    _eventHandler->onConnectionParametersUpdateComplete(
        ConnectionParametersUpdateCompleteEvent(
            status == pal::hci_error_code_t::SUCCESS ? BLE_ERROR_NONE : BLE_ERROR_UNSPECIFIED,
            connection_handle,
            conn_interval_t(connection_interval),
            slave_latency_t(connection_latency),
            supervision_timeout_t(supervision_timeout)
        )
    );
}

void GenericGap::on_remote_connection_parameter(
    connection_handle_t connection_handle,
    uint16_t connection_interval_min,
    uint16_t connection_interval_max,
    uint16_t connection_latency,
    uint16_t supervision_timeout
)
{
    if (_user_manage_connection_parameter_requests) {
        // ignore for now as it is
        _pal_gap.accept_connection_parameter_request(
            connection_handle,
            connection_interval_min,
            connection_interval_max,
            connection_latency,
            supervision_timeout,
            /* connection event length min */ 0,
            /* connection event length max */ 0
        );
    } else {
        if (!_eventHandler) {
            return;
        }

        _eventHandler->onUpdateConnectionParametersRequest(
            UpdateConnectionParametersRequestEvent(
                connection_handle,
                conn_interval_t(connection_interval_min),
                conn_interval_t(connection_interval_max),
                connection_latency,
                supervision_timeout_t(supervision_timeout)
            )
        );
    }
}

ble_error_t GenericGap::setScanParameters(const ScanParameters &params)
{
    useVersionTwoAPI();

    if (is_extended_advertising_available()) {
        bool active_scanning[] = {
            params.get1mPhyConfiguration().isActiveScanningSet(),
            params.getCodedPhyConfiguration().isActiveScanningSet()
        };

        uint16_t scan_interval[] = {
            params.get1mPhyConfiguration().getInterval().value(),
            params.getCodedPhyConfiguration().getInterval().value()
        };

        uint16_t scan_window[] = {
            params.get1mPhyConfiguration().getWindow().value(),
            params.getCodedPhyConfiguration().getWindow().value()
        };

        return _pal_gap.set_extended_scan_parameters(
            params.getOwnAddressType(),
            params.getFilter(),
            params.getPhys(),
            active_scanning,
            scan_interval,
            scan_window
        );
    } else {
        if (params.getPhys().get_coded()) {
            return BLE_ERROR_INVALID_PARAM;
        }

        ScanParameters::phy_configuration_t legacy_configuration =
            params.get1mPhyConfiguration();

        return _pal_gap.set_scan_parameters(
            legacy_configuration.isActiveScanningSet(),
            legacy_configuration.getInterval().value(),
            legacy_configuration.getWindow().value(),
            params.getOwnAddressType(),
            params.getFilter()
        );
    }
}

ble_error_t GenericGap::startScan(
    scan_duration_t duration,
    duplicates_filter_t filtering,
    scan_period_t period
)
{
    useVersionTwoAPI();

    if (_privacy_enabled && _central_privacy_configuration.use_non_resolvable_random_address) {
        set_random_address_rotation(true);
    }

    if (is_extended_advertising_available()) {
        ble_error_t err = _pal_gap.extended_scan_enable(
            /* enable */true,
            filtering,
            duration.value(),
            period.value()
        );

        if (err) {
            return err;
        }
    } else {
        if (period.value() != 0) {
            return BLE_ERROR_INVALID_PARAM;
        }

        ble_error_t err = _pal_gap.scan_enable(
            true,
            filtering == duplicates_filter_t::DISABLE ? false : true
        );

        if (err) {
            return err;
        }

        _scan_timeout.detach();
        if (duration.value()) {
            _scan_timeout.attach_us(
                mbed::callback(this, &GenericGap::on_scan_timeout),
                microsecond_t(duration).value()
            );
        }
    }

    _scan_enabled = true;

    return BLE_ERROR_NONE;
}

ble_error_t GenericGap::createSync(
    peer_address_type_t peerAddressType,
    const ble::address_t &peerAddress,
    advertising_sid_t sid,
    slave_latency_t maxPacketSkip,
    sync_timeout_t timeout
)
{
    useVersionTwoAPI();

    if (is_extended_advertising_available() == false) {
        return BLE_ERROR_NOT_IMPLEMENTED;
    }

    if (peerAddressType != peer_address_type_t::PUBLIC &&
        peerAddressType != peer_address_type_t::RANDOM
    ) {
        return BLE_ERROR_INVALID_PARAM;
    }

    if (sid > 0x0F) {
        return BLE_ERROR_INVALID_PARAM;
    }

    return _pal_gap.periodic_advertising_create_sync(
        /* use advertiser list */ false,
        sid,
        (peer_address_type_t::type) peerAddressType.value(),
        peerAddress,
        maxPacketSkip.value(),
        timeout.value()
    );
}

ble_error_t GenericGap::createSync(
    slave_latency_t maxPacketSkip,
    sync_timeout_t timeout
)
{
    useVersionTwoAPI();

    if (is_extended_advertising_available() == false) {
        return BLE_ERROR_NOT_IMPLEMENTED;
    }

    return _pal_gap.periodic_advertising_create_sync(
        /* use advertiser list */ true,
        /* sid - not used */ 0x00,
        /* N/A */ peer_address_type_t::PUBLIC,
        /* N/A */ ble::address_t(),
        maxPacketSkip.value(),
        timeout.value()
    );
}

ble_error_t GenericGap::cancelCreateSync()
{
    useVersionTwoAPI();

    if (is_extended_advertising_available() == false) {
        return BLE_ERROR_NOT_IMPLEMENTED;
    }

    return _pal_gap.cancel_periodic_advertising_create_sync();
}

ble_error_t GenericGap::terminateSync(periodic_sync_handle_t handle)
{
    useVersionTwoAPI();

    if (is_extended_advertising_available() == false) {
        return BLE_ERROR_NOT_IMPLEMENTED;
    }

    return _pal_gap.periodic_advertising_terminate_sync(handle);
}

ble_error_t GenericGap::addDeviceToPeriodicAdvertiserList(
    peer_address_type_t peerAddressType,
    const ble::address_t &peerAddress,
    advertising_sid_t sid
)
{
    useVersionTwoAPI();

    if (is_extended_advertising_available() == false) {
        return BLE_ERROR_NOT_IMPLEMENTED;
    }

    if (peerAddressType != PeerAddressType_t::PUBLIC ||
        peerAddressType != PeerAddressType_t::RANDOM
        ) {
        return BLE_ERROR_INVALID_PARAM;
    }

    if (sid > 0x0F) {
        return BLE_ERROR_INVALID_PARAM;
    }

    return _pal_gap.add_device_to_periodic_advertiser_list(
        (pal::advertising_peer_address_type_t::type) peerAddressType.value(),
        peerAddress,
        sid
    );
}

ble_error_t GenericGap::removeDeviceFromPeriodicAdvertiserList(
    peer_address_type_t peerAddressType,
    const ble::address_t &peerAddress,
    advertising_sid_t sid
)
{
    useVersionTwoAPI();

    if (is_extended_advertising_available() == false) {
        return BLE_ERROR_NOT_IMPLEMENTED;
    }

    if (peerAddressType != PeerAddressType_t::PUBLIC ||
        peerAddressType != PeerAddressType_t::RANDOM
        ) {
        return BLE_ERROR_INVALID_PARAM;
    }

    if (sid > 0x0F) {
        return BLE_ERROR_INVALID_PARAM;
    }

    return _pal_gap.remove_device_from_periodic_advertiser_list(
        (pal::advertising_peer_address_type_t::type) peerAddressType.value(),
        peerAddress,
        sid
    );
}

ble_error_t GenericGap::clearPeriodicAdvertiserList()
{
    useVersionTwoAPI();

    if (is_extended_advertising_available() == false) {
        return BLE_ERROR_NOT_IMPLEMENTED;
    }

    return _pal_gap.clear_periodic_advertiser_list();
}

uint8_t GenericGap::getMaxPeriodicAdvertiserListSize()
{
    useVersionTwoAPI();

    if (is_extended_advertising_available() == false) {
        return BLE_ERROR_NOT_IMPLEMENTED;
    }

    return _pal_gap.read_periodic_advertiser_list_size();
}

void GenericGap::useVersionOneAPI() const
{
    if (_non_deprecated_scan_api_used) {
        MBED_ERROR(mixed_scan_api_error, "Use of deprecated scan API with up to date API");
    }
    _deprecated_scan_api_used = true;
}

void GenericGap::useVersionTwoAPI() const
{
    if (_deprecated_scan_api_used) {
        MBED_ERROR(mixed_scan_api_error, "Use of up to date scan API with deprecated API");
    }
    _non_deprecated_scan_api_used = true;
}

bool GenericGap::is_extended_advertising_available()
{
    return isFeatureSupported(
        controller_supported_features_t::LE_EXTENDED_ADVERTISING
    );
}

} // namespace generic
} // namespace ble
