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

static const Gap::ConnectionParams_t default_connection_params = {
    /* min conn interval */ 50,
    /* max  conn interval */ 100,
    /* slave latency */ 0,
    /* supervision timeout */ 600
};

static const GapScanningParams default_scan_params;

/*
 * Return true if value is included in the range [lower_bound : higher_bound]
 */
template<typename T>
static bool is_in_range(T value, T lower_bound, T higher_bound) {
    if (value < lower_bound || value > higher_bound) {
        return false;
    }
    return true;
}

/*
 * Return true if the scan parameters are valid or false otherwise.
 */
static bool is_scan_params_valid(const GapScanningParams* params)
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
static bool is_connection_params_valid(const Gap::ConnectionParams_t* params)
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
        ((uint32_t)params->maxConnectionInterval * 125) / 100;
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
static bool is_preferred_connection_params_valid(const Gap::ConnectionParams_t* params)
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

    if (params->maxConnectionInterval == 0xFFFF)  {
        return true;
    }

    uint16_t max_connection_interval_ms =
        ((uint32_t)params->maxConnectionInterval * 125) / 100;
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
static bool is_prand_valid(const uint8_t* bytes, size_t len)
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
) {
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
) {
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
static bool is_disconnection_reason_valid(Gap::DisconnectionReason_t reason)
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
        case Gap::REMOTE_USER_TERMINATED_CONNECTION:
        case Gap::REMOTE_DEV_TERMINATION_DUE_TO_LOW_RESOURCES:
        case Gap::REMOTE_DEV_TERMINATION_DUE_TO_POWER_OFF:
        case Gap::CONN_INTERVAL_UNACCEPTABLE:
            return true;
        default:
            return false;
    }
}

/*
 * Return true if the whitelist in input is valid or false otherwise.
 */
static bool is_whitelist_valid(const Gap::Whitelist_t& whitelist)
{
    if (whitelist.size > whitelist.capacity) {
        return false;
    }

    for (size_t i = 0; i < whitelist.size; ++i) {
        const BLEProtocol::Address_t& address = whitelist.addresses[i];
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
    const BLEProtocol::Address_t& device, const Gap::Whitelist_t& whitelist
) {
    for (size_t i = 0; i < whitelist.size; ++i) {
        const BLEProtocol::Address_t& potential_device = whitelist.addresses[i];

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
) {
        return (address_type == BLEProtocol::AddressType::PUBLIC) ?
            pal::whitelist_address_type_t::PUBLIC_DEVICE_ADDRESS :
            pal::whitelist_address_type_t::RANDOM_DEVICE_ADDRESS;
}

/*
 * Convert a BLEProtocol::AddressType_t into a pal::peer_address_type
 */
static peer_address_type_t to_peer_address_type(
    LegacyAddressType_t address_type
) {
        return (address_type == LegacyAddressType::PUBLIC) ?
            peer_address_type_t::PUBLIC :
            peer_address_type_t::RANDOM;
}


/*
 * Return true if the advertising parameters are valid.
 */
static bool is_advertising_params_valid(const GapAdvertisingParams& params)
{
    if (is_in_range(params.getIntervalInADVUnits(), advertising_interval_min, advertising_interval_max) == false) {
        return false;
    }

    if (params.getAdvertisingType() > GapAdvertisingParams::ADV_NON_CONNECTABLE_UNDIRECTED) {
        return false;
    }

    return true;
}

} // end of anonymous namespace

GenericGap::GenericGap(
    pal::EventQueue& event_queue,
    pal::Gap& pal_gap,
    pal::GenericAccessService& generic_access_service,
    pal::SecurityManager& pal_sm
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
    _advertising_timeout(),
    _scan_timeout(),
    _connection_event_handler(NULL)
{
    _pal_gap.when_gap_event_received(
        mbed::callback(this, &GenericGap::on_gap_event_received)
    );

    // Recover static random identity
    _random_static_identity_address = _pal_gap.get_random_address();
}

GenericGap::~GenericGap()
{
}

ble_error_t GenericGap::setAddress(
    LegacyAddressType_t type,
    const Address_t address
) {
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
) {
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
    ble_error_t err = _pal_gap.scan_enable(false, false);
    if (err) {
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
) {
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
    const ConnectionParams_t* connectionParams,
    const GapScanningParams* scanParams
) {
    return connect(
        peerAddr,
        to_peer_address_type(peerAddrType),
        connectionParams,
        scanParams
    );
}

ble_error_t GenericGap::disconnect(Handle_t connectionHandle, DisconnectionReason_t reason)
{
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
    if(is_preferred_connection_params_valid(params) == false) {
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
    if(initialize_whitelist() == false) {
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

    if(initialize_whitelist() == false) {
        return BLE_ERROR_INVALID_STATE;
    }

    if (whitelist.capacity > _whitelist.capacity) {
        return BLE_ERROR_INVALID_PARAM;
    }

    // first evict devices not in the existing whitelist
    for (size_t i = 0; i < _whitelist.size; ++i) {
        const BLEProtocol::Address_t& device = _whitelist.addresses[i];

        if (is_in_whitelist(device, whitelist) == false) {
            ble_error_t err = _pal_gap.remove_device_from_whitelist(
                to_whitelist_address_type(device.type),
                device.address
            );

            // try to restore the whitelist to its initial state
            if (err) {
                for (size_t j = 0; j < i; ++j) {
                    const BLEProtocol::Address_t& device = _whitelist.addresses[j];

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
        const BLEProtocol::Address_t& device = whitelist.addresses[i];

        if (is_in_whitelist(device, _whitelist) == false) {
            ble_error_t err = _pal_gap.add_device_to_whitelist(
                to_whitelist_address_type(device.type),
                device.address
            );

            // try to restore the whitelist to its initial state
            if (err) {
                // first remove the devices added
                for (size_t j = 0; j < i; ++j) {
                    const BLEProtocol::Address_t& device = whitelist.addresses[j];

                    if (is_in_whitelist(device, _whitelist) == false) {
                        _pal_gap.remove_device_from_whitelist(
                            to_whitelist_address_type(device.type),
                            device.address
                        );
                    }
                }

                // second add the devices of the initial list evicted
                for (size_t i = 0; i < _whitelist.size; ++i) {
                    const BLEProtocol::Address_t& device = _whitelist.addresses[i];

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
    if (mode > Gap::ADV_POLICY_FILTER_ALL_REQS) {
        return BLE_ERROR_INVALID_PARAM;
    }

    _advertising_filter_policy = (pal::advertising_filter_policy_t::type) mode;
    return BLE_ERROR_NONE;
}

ble_error_t GenericGap::setScanningPolicyMode(ScanningPolicyMode_t mode)
{
    if (mode > Gap::SCAN_POLICY_FILTER_ALL_ADV) {
        return BLE_ERROR_INVALID_PARAM;
    }

    _scanning_filter_policy = (pal::scanning_filter_policy_t::type) mode;
    return BLE_ERROR_NONE;
}

ble_error_t GenericGap::setInitiatorPolicyMode(InitiatorPolicyMode_t mode)
{
    if (mode > Gap::INIT_POLICY_FILTER_ALL_ADV) {
        return BLE_ERROR_INVALID_PARAM;
    }

    _initiator_policy_mode = (pal::initiator_policy_t::type) mode;
    return BLE_ERROR_NONE;
}

Gap::AdvertisingPolicyMode_t GenericGap::getAdvertisingPolicyMode(void) const
{
    return (AdvertisingPolicyMode_t) _advertising_filter_policy.value();
}

Gap::ScanningPolicyMode_t GenericGap::getScanningPolicyMode(void) const
{
    return (ScanningPolicyMode_t) _scanning_filter_policy.value();
}

Gap::InitiatorPolicyMode_t GenericGap::getInitiatorPolicyMode(void) const
{
    return (InitiatorPolicyMode_t) _initiator_policy_mode.value();
}

ble_error_t GenericGap::startRadioScan(const GapScanningParams &scanningParams)
{
    if (is_scan_params_valid(&scanningParams) == false) {
        return BLE_ERROR_INVALID_PARAM;
    }

    if (_scanning_filter_policy == pal::scanning_filter_policy_t::FILTER_ADVERTISING &&
        _whitelist.size == 0) {
        return BLE_ERROR_INVALID_STATE;
    }

    pal::own_address_type_t own_address_type = get_own_address_type(CENTRAL_SCAN /* central, can use non resolvable address for scan requests */);

    if(_privacy_enabled && (own_address_type == pal::own_address_type_t::RANDOM_ADDRESS))
    {
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
    if(enable == _privacy_enabled) {
        // No change
        return BLE_ERROR_NONE;
    }

    if(enable && !_pal_gap.is_privacy_supported())
    {
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

ble_error_t GenericGap::startAdvertising(const GapAdvertisingParams& params)
{
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

    if(_privacy_enabled && (own_address_type == pal::own_address_type_t::RANDOM_ADDRESS))
    {
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
        pal::advertising_peer_address_type_t::PUBLIC_ADDRESS,
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
    Gap::reset();
    _advertising_timeout.detach();
    _scan_timeout.detach();

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
) {
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
}

void GenericGap::processDisconnectionEvent(
    Handle_t handle,
    DisconnectionReason_t reason
) {
    if (_connection_event_handler) {
        _connection_event_handler->on_disconnected(
            handle,
            reason
        );
    }

    ::Gap::processDisconnectionEvent(
        handle,
        reason
    );
}

void GenericGap::on_scan_timeout()
{
    _event_queue.post(mbed::callback(this, &GenericGap::process_scan_timeout));
}

void GenericGap::process_scan_timeout()
{
    ble_error_t err = _pal_gap.scan_enable(false, false);
    if (err) {
        // TODO: define the mechanism signaling the error
    }
    processTimeoutEvent(Gap::TIMEOUT_SRC_SCAN);
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

    processTimeoutEvent(Gap::TIMEOUT_SRC_ADVERTISING);
}

void GenericGap::on_gap_event_received(const pal::GapEvent& e)
{
    switch (e.type.value()) {
        case pal::GapEventType::ADVERTISING_REPORT:
            on_advertising_report(static_cast<const pal::GapAdvertisingReportEvent&>(e));
            break;

        case pal::GapEventType::CONNECTION_COMPLETE:
            on_connection_complete(static_cast<const pal::GapConnectionCompleteEvent&>(e));
            break;

        case pal::GapEventType::CONNECTION_UPDATE:
            on_connection_update(static_cast<const pal::GapConnectionUpdateEvent&>(e));
            break;

        case pal::GapEventType::DISCONNECTION_COMPLETE:
            on_disconnection_complete(static_cast<const pal::GapDisconnectionCompleteEvent&>(e));
            break;

        case pal::GapEventType::REMOTE_CONNECTION_PARAMETER_REQUEST:
            on_connection_parameter_request(static_cast<const pal::GapRemoteConnectionParameterRequestEvent&>(e));
            break;

        case pal::GapEventType::UNEXPECTED_ERROR:
            on_unexpected_error(static_cast<const pal::GapUnexpectedErrorEvent&>(e));
            break;

        default:
            break;
    }
}

void GenericGap::on_advertising_report(const pal::GapAdvertisingReportEvent& e)
{
    for (size_t i = 0; i < e.size(); ++i) {
        pal::GapAdvertisingReportEvent::advertising_t advertising = e[i];

        // Check if the address hasn't been resolved
        if(_privacy_enabled &&
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

        processAdvertisementReport(
            advertising.address.data(),
            advertising.rssi,
            advertising.type == pal::received_advertising_type_t::SCAN_RESPONSE,
            (GapAdvertisingParams::AdvertisingType_t) advertising.type.value(),
            advertising.data.size(),
            advertising.data.data(),
            peer_address_type
        );
    }
}

void GenericGap::on_connection_complete(const pal::GapConnectionCompleteEvent& e)
{
    if (e.status != pal::hci_error_code_t::SUCCESS) {
        // for now notify user that the connection failled by issuing a timeout
        // event

        // TODO: Define events in case of connection faillure
        processTimeoutEvent(Gap::TIMEOUT_SRC_CONN);
        return;
    }

    bool needs_pairing = false;
    bool needs_authentication = false;

    if (_privacy_enabled &&
        e.role.value() == e.role.SLAVE &&
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
            switch(_peripheral_privacy_configuration.resolution_strategy) {
                case PeripheralPrivacyConfiguration_t::REJECT_NON_RESOLVED_ADDRESS:
                    // Reject connection request - the user will get notified through a callback
                    _pal_gap.disconnect(
                        e.connection_handle,
                        pal::disconnection_reason_t::AUTHENTICATION_FAILLURE
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

    if (e.role.value() == e.role.SLAVE) {
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

    processConnectionEvent(
        e.connection_handle,
        e.role.value() == e.role.MASTER ? ::Gap::CENTRAL : ::Gap::PERIPHERAL,
        e.peer_address_type,
        e.peer_address.data(),
        _address_type,
        address.data(),
        &connection_params,
        e.local_resolvable_private_address.data(),
        e.peer_resolvable_private_address.data()
    );

    // Now starts pairing or authentication procedures if required
    if(needs_pairing) {
        SecurityManager &sm = createBLEInstance()->getSecurityManager();
        // Request authentication to start pairing procedure
        sm.requestAuthentication(e.connection_handle);
    } else if(needs_authentication) {
        // TODO: GAP Authentication != Security Manager authentication
        // Needs to be implemented
    }
}

void GenericGap::on_disconnection_complete(const pal::GapDisconnectionCompleteEvent& e)
{
    if (e.status == pal::hci_error_code_t::SUCCESS) {
        processDisconnectionEvent(
            e.connection_handle,
            (Gap::DisconnectionReason_t) e.reason
        );
    } else {
        // TODO: define what to do in case of faillure
    }
}

void GenericGap::on_connection_parameter_request(const pal::GapRemoteConnectionParameterRequestEvent& e)
{
    // intern behavior, accept all new parameter requests
    // TODO: expose an API so user code can accept or reject such request
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

void GenericGap::on_connection_update(const pal::GapConnectionUpdateEvent& e)
{
    // TODO: add feature in interface to notify the user that the connection
    // has been updated.
}

void GenericGap::on_unexpected_error(const pal::GapUnexpectedErrorEvent& e)
{
    // TODO: add feature in interface to notify the user that the connection
    // has been updated.
}

pal::own_address_type_t GenericGap::get_own_address_type(AddressUseType_t address_use_type)
{
    if(_privacy_enabled) {
        bool use_non_resolvable_address = false;
        if(address_use_type == CENTRAL_SCAN) {
            use_non_resolvable_address = _central_privacy_configuration.use_non_resolvable_random_address;
        } else if (address_use_type == PERIPHERAL_NON_CONNECTABLE) {
            use_non_resolvable_address = _peripheral_privacy_configuration.use_non_resolvable_random_address;
        }

        // An non resolvable private address should be generated
        if(use_non_resolvable_address) {
            return pal::own_address_type_t::RANDOM_ADDRESS;
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
            return pal::own_address_type_t::PUBLIC_ADDRESS;
        default:
            return pal::own_address_type_t::RANDOM_ADDRESS;
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

    _whitelist.addresses = new (std::nothrow) BLEProtocol::Address_t[whitelist_capacity] ;
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

    if(!_privacy_enabled) {
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
    if(enable == _random_address_rotating) {
        return;
    }

    _random_address_rotating = enable;

    if(enable) {
        // Set first address
        update_random_address();

        // Schedule rotations every 15 minutes as recomended by the spec
        _address_rotation_ticker.attach_us(
            mbed::callback(this, &GenericGap::on_address_rotation_timeout),
            15 * 60 * 1000000U
        );
    }
    else {
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
    if(!_random_address_rotating)
    {
        // This event might have been queued before we disabled address rotation
        return;
    }

    ble::address_t address;

    do {
        byte_array_t<8> random_data;

        ble_error_t ret = _pal_sm.get_random_data(random_data);
        if (ret != BLE_ERROR_NONE) {
            // Abort
            return;
        }

        // Build a non-resolvable private address as specified in the Core 4.2 spec, Vol 6, Part B, 1.3.2.2
        // Mask out two MSbs
        random_data[5] &= 0x3F;

        // Copy to address - will copy first 6 bytes
        address = ble::address_t(random_data.data());

        if(!is_random_private_non_resolvable_address(address.data()))
        {
            // If address is invalid, which is unlikely (all 0s or all 1s), try again
            // If implementation is faulty, we'll get stuck here
            continue;
        }

        // Address is valid
        break;
    } while(true);

    ble_error_t err = _pal_gap.set_random_address(
        address
    );
    if (err) {
        return;
    }

    _address_type = LegacyAddressType::RANDOM_PRIVATE_NON_RESOLVABLE;
    _address = address;
}

void GenericGap::on_address_rotation_timeout()
{
    _event_queue.post(mbed::callback(this, &GenericGap::update_random_address));
}

void GenericGap::set_connection_event_handler(pal::ConnectionEventMonitor::EventHandler *connection_event_handler)
{
    _connection_event_handler = connection_event_handler;
}

} // namespace generic
} // namespace ble
