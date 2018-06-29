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

#include "nRF5xn.h"
#ifdef YOTTA_CFG_MBED_OS
    #include "mbed-drivers/mbed.h"
#else
    #include "mbed.h"
#endif
#include "ble/BLE.h"

#include "common/common.h"
#include "headers/ble_hci.h"
#include "ble/pal/ConnectionEventMonitor.h"
#include "nRF5xPalSecurityManager.h"
#include <algorithm>

using ble::pal::vendor::nordic::nRF5xSecurityManager;
using ble::ArrayView;
using ble::pal::advertising_peer_address_type_t;
using ble::peer_address_type_t;

typedef BLEProtocol::AddressType LegacyAddressType;
typedef BLEProtocol::AddressType_t LegacyAddressType_t;

namespace {

nRF5xSecurityManager& get_sm() {
    return nRF5xSecurityManager::get_security_manager();
}

ble_error_t set_private_resolvable_address() {
#if (NRF_SD_BLE_API_VERSION <= 2)
    ble_gap_addr_t addr = { BLE_GAP_ADDR_TYPE_RANDOM_PRIVATE_RESOLVABLE };
    uint32_t err = sd_ble_gap_address_set(BLE_GAP_ADDR_CYCLE_MODE_AUTO, &addr);
#else
    ble_gap_privacy_params_t privacy_config = { 0 };
    uint32_t err = sd_ble_gap_privacy_get(&privacy_config);
    if (err) {
        return BLE_ERROR_UNSPECIFIED;
    }

    privacy_config.private_addr_type = BLE_GAP_ADDR_TYPE_RANDOM_PRIVATE_RESOLVABLE;
    err = sd_ble_gap_privacy_set(&privacy_config);
#endif
    return err ? BLE_ERROR_UNSPECIFIED : BLE_ERROR_NONE;
}

ble_error_t set_private_non_resolvable_address() {
#if (NRF_SD_BLE_API_VERSION <= 2)
    ble_gap_addr_t addr = { BLE_GAP_ADDR_TYPE_RANDOM_PRIVATE_NON_RESOLVABLE };
    uint32_t err = sd_ble_gap_address_set(BLE_GAP_ADDR_CYCLE_MODE_AUTO, &addr);
#else
    ble_gap_privacy_params_t privacy_config = { 0 };
    uint32_t err = sd_ble_gap_privacy_get(&privacy_config);
    if (err) {
        return BLE_ERROR_UNSPECIFIED;
    }

    privacy_config.private_addr_type = BLE_GAP_ADDR_TYPE_RANDOM_PRIVATE_NON_RESOLVABLE;
    err = sd_ble_gap_privacy_set(&privacy_config);
#endif
    return err ? BLE_ERROR_UNSPECIFIED : BLE_ERROR_NONE;
}

bool is_advertising_non_connectable(const GapAdvertisingParams &params) {
    switch (params.getAdvertisingType()) {
        case GapAdvertisingParams::ADV_SCANNABLE_UNDIRECTED:
        case GapAdvertisingParams::ADV_NON_CONNECTABLE_UNDIRECTED:
            return true;
        default:
            return false;
    }
}

bool is_identity_address(peer_address_type_t address_type) {
    return address_type == peer_address_type_t::PUBLIC_IDENTITY ||
        address_type == peer_address_type_t::RANDOM_STATIC_IDENTITY;
}

peer_address_type_t convert_nordic_address(bool identity, uint8_t address) {
    if (identity) {
        if (address == BLE_GAP_ADDR_TYPE_PUBLIC) {
            return peer_address_type_t::PUBLIC_IDENTITY;
        } else {
            return peer_address_type_t::RANDOM_STATIC_IDENTITY;
        }
    } else {
        if (address == BLE_GAP_ADDR_TYPE_PUBLIC) {
            return peer_address_type_t::PUBLIC;
        } else {
            return peer_address_type_t::RANDOM;
        }
    }
}

peer_address_type_t convert_identity_address(advertising_peer_address_type_t address) {
    if (address == advertising_peer_address_type_t::PUBLIC_ADDRESS) {
        return peer_address_type_t::PUBLIC_IDENTITY;
    } else {
        return peer_address_type_t::RANDOM_STATIC_IDENTITY;
    }
}

} // namespace

void radioNotificationStaticCallback(bool param) {
    nRF5xGap &gap = (nRF5xGap &) nRF5xn::Instance(BLE::DEFAULT_INSTANCE).getGap();
    gap.processRadioNotificationEvent(param);
}

nRF5xGap::nRF5xGap() : Gap(),
    advertisingPolicyMode(Gap::ADV_POLICY_IGNORE_WHITELIST),
    scanningPolicyMode(Gap::SCAN_POLICY_IGNORE_WHITELIST),
    whitelistAddressesSize(0),
    whitelistAddresses(),
    radioNotificationCallbackParam(false),
    radioNotificationTimeout(),
    _connection_event_handler(NULL),
    _privacy_enabled(false),
    _peripheral_privacy_configuration(default_peripheral_privacy_configuration),
    _central_privacy_configuration(default_central_privacy_configuration),
    _non_private_address_type(LegacyAddressType::RANDOM_STATIC),
    _connections_role()
{
        m_connectionHandle = BLE_CONN_HANDLE_INVALID;
}
/**************************************************************************/
/*!
    @brief  Sets the advertising parameters and payload for the device

    @param[in]  params
                Basic advertising details, including the advertising
                delay, timeout and how the device should be advertised
    @params[in] advData
                The primary advertising data payload
    @params[in] scanResponse
                The optional Scan Response payload if the advertising
                type is set to \ref GapAdvertisingParams::ADV_SCANNABLE_UNDIRECTED
                in \ref GapAdveritinngParams

    @returns    \ref ble_error_t

    @retval     BLE_ERROR_NONE
                Everything executed properly

    @retval     BLE_ERROR_BUFFER_OVERFLOW
                The proposed action would cause a buffer overflow.  All
                advertising payloads must be <= 31 bytes, for example.

    @retval     BLE_ERROR_NOT_IMPLEMENTED
                A feature was requested that is not yet supported in the
                nRF51 firmware or hardware.

    @retval     BLE_ERROR_PARAM_OUT_OF_RANGE
                One of the proposed values is outside the valid range.

    @section EXAMPLE

    @code

    @endcode
*/
/**************************************************************************/
ble_error_t nRF5xGap::setAdvertisingData(const GapAdvertisingData &advData, const GapAdvertisingData &scanResponse)
{
    /* Make sure we don't exceed the advertising payload length */
    if (advData.getPayloadLen() > GAP_ADVERTISING_DATA_MAX_PAYLOAD) {
        return BLE_ERROR_BUFFER_OVERFLOW;
    }

    /* Make sure we have a payload! */
    if (advData.getPayloadLen() == 0) {
        return BLE_ERROR_PARAM_OUT_OF_RANGE;
    }

    /* Check the scan response payload limits */
    //if ((params.getAdvertisingType() == GapAdvertisingParams::ADV_SCANNABLE_UNDIRECTED))
    //{
    //    /* Check if we're within the upper limit */
    //    if (advData.getPayloadLen() > GAP_ADVERTISING_DATA_MAX_PAYLOAD)
    //    {
    //        return BLE_ERROR_BUFFER_OVERFLOW;
    //    }
    //    /* Make sure we have a payload! */
    //    if (advData.getPayloadLen() == 0)
    //    {
    //        return BLE_ERROR_PARAM_OUT_OF_RANGE;
    //    }
    //}

    /* Send advertising data! */
    ASSERT_TRUE(ERROR_NONE ==
           sd_ble_gap_adv_data_set(advData.getPayload(),
                                   advData.getPayloadLen(),
                                   scanResponse.getPayload(),
                                   scanResponse.getPayloadLen()),
           BLE_ERROR_PARAM_OUT_OF_RANGE);

    /* Make sure the GAP Service appearance value is aligned with the
     *appearance from GapAdvertisingData */
    ASSERT_TRUE(ERROR_NONE == sd_ble_gap_appearance_set(advData.getAppearance()),
           BLE_ERROR_PARAM_OUT_OF_RANGE);

    /* ToDo: Perform some checks on the payload, for example the Scan Response can't */
    /* contains a flags AD type, etc. */

    return BLE_ERROR_NONE;
}

/**************************************************************************/
/*!
    @brief  Starts the BLE HW, initialising any services that were
            added before this function was called.

    @note   All services must be added before calling this function!

    @returns    ble_error_t

    @retval     BLE_ERROR_NONE
                Everything executed properly

    @section EXAMPLE

    @code

    @endcode
*/
/**************************************************************************/
ble_error_t nRF5xGap::startAdvertising(const GapAdvertisingParams &params)
{
    uint32_t             err;
    ble_gap_adv_params_t adv_para = {0};

    /* Make sure we support the advertising type */
    if (params.getAdvertisingType() == GapAdvertisingParams::ADV_CONNECTABLE_DIRECTED) {
        /* ToDo: This requires a propery security implementation, etc. */
        return BLE_ERROR_NOT_IMPLEMENTED;
    }

    /* Check interval range */
    if (params.getAdvertisingType() == GapAdvertisingParams::ADV_NON_CONNECTABLE_UNDIRECTED) {
        /* Min delay is slightly longer for unconnectable devices */
        if ((params.getIntervalInADVUnits() < GapAdvertisingParams::GAP_ADV_PARAMS_INTERVAL_MIN_NONCON) ||
            (params.getIntervalInADVUnits() > GapAdvertisingParams::GAP_ADV_PARAMS_INTERVAL_MAX)) {
            return BLE_ERROR_PARAM_OUT_OF_RANGE;
        }
    } else {
        if ((params.getIntervalInADVUnits() < GapAdvertisingParams::GAP_ADV_PARAMS_INTERVAL_MIN) ||
            (params.getIntervalInADVUnits() > GapAdvertisingParams::GAP_ADV_PARAMS_INTERVAL_MAX)) {
            return BLE_ERROR_PARAM_OUT_OF_RANGE;
        }
    }

    /* Check timeout is zero for Connectable Directed */
    if ((params.getAdvertisingType() == GapAdvertisingParams::ADV_CONNECTABLE_DIRECTED) && (params.getTimeout() != 0)) {
        /* Timeout must be 0 with this type, although we'll never get here */
        /* since this isn't implemented yet anyway */
        return BLE_ERROR_PARAM_OUT_OF_RANGE;
    }

    /* Check timeout for other advertising types */
    if ((params.getAdvertisingType() != GapAdvertisingParams::ADV_CONNECTABLE_DIRECTED) &&
        (params.getTimeout() > GapAdvertisingParams::GAP_ADV_PARAMS_TIMEOUT_MAX)) {
        return BLE_ERROR_PARAM_OUT_OF_RANGE;
    }

#if (NRF_SD_BLE_API_VERSION <= 2)
    /* Allocate the stack's whitelist statically */
    ble_gap_whitelist_t  whitelist;
    ble_gap_addr_t      *whitelistAddressPtrs[YOTTA_CFG_WHITELIST_MAX_SIZE];
    ble_gap_irk_t       *whitelistIrkPtrs[YOTTA_CFG_IRK_TABLE_MAX_SIZE];
    /* Initialize the whitelist */
    whitelist.pp_addrs   = whitelistAddressPtrs;
    whitelist.pp_irks    = whitelistIrkPtrs;
    whitelist.addr_count = 0;
    whitelist.irk_count  = 0;

    /* Add missing IRKs to whitelist from the bond table held by the SoftDevice */
    if (advertisingPolicyMode != Gap::ADV_POLICY_IGNORE_WHITELIST) {
        ble_error_t error = generateStackWhitelist(whitelist);
        if (error != BLE_ERROR_NONE) {
            return error;
        }
    }

    if (_privacy_enabled) {
        if (_peripheral_privacy_configuration.resolution_strategy != PeripheralPrivacyConfiguration_t::DO_NOT_RESOLVE) {
            ArrayView<resolving_list_entry_t> entries = get_sm().get_resolving_list();

            size_t limit = std::min(
                entries.size(), (size_t) YOTTA_CFG_IRK_TABLE_MAX_SIZE
            );

            for (size_t i = 0; i < limit; ++i) {
                whitelistIrkPtrs[i] = (ble_gap_irk_t*) entries[i].peer_irk.data();
            }
            whitelist.irk_count = limit;
        }

        if (_peripheral_privacy_configuration.use_non_resolvable_random_address &&
            is_advertising_non_connectable(params)
        ) {
            set_private_non_resolvable_address();
        } else {
            set_private_resolvable_address();
        }
    }
    adv_para.p_whitelist = &whitelist;
#else
    if (_privacy_enabled) {
        bool enable_resolution =
            _peripheral_privacy_configuration.resolution_strategy != PeripheralPrivacyConfiguration_t::DO_NOT_RESOLVE;
        update_identities_list(enable_resolution);

        if (_peripheral_privacy_configuration.use_non_resolvable_random_address &&
            is_advertising_non_connectable(params)
        ) {
            set_private_non_resolvable_address();
        } else {
            set_private_resolvable_address();
        }
    }
#endif
    /* For NRF_SD_BLE_API_VERSION >= 3 nRF5xGap::setWhitelist setups the whitelist. */

    /* Start Advertising */


    adv_para.type        = params.getAdvertisingType();
    adv_para.p_peer_addr = NULL;                           // Undirected advertisement
    adv_para.fp          = advertisingPolicyMode;
    adv_para.interval    = params.getIntervalInADVUnits(); // advertising interval (in units of 0.625 ms)
    adv_para.timeout     = params.getTimeout();

#if  (NRF_SD_BLE_API_VERSION >= 5)
    err = sd_ble_gap_adv_start(&adv_para, NRF_CONNECTION_TAG);
#else
    err = sd_ble_gap_adv_start(&adv_para);
#endif
    switch(err) {
        case ERROR_NONE:
            return BLE_ERROR_NONE;
        case NRF_ERROR_NO_MEM:
            return BLE_ERROR_NO_MEM;
        default:
            return BLE_ERROR_PARAM_OUT_OF_RANGE;
    }
}

/* Observer role is not supported by S110, return BLE_ERROR_NOT_IMPLEMENTED */
#if !defined(TARGET_MCU_NRF51_16K_S110) && !defined(TARGET_MCU_NRF51_32K_S110)
ble_error_t nRF5xGap::startRadioScan(const GapScanningParams &scanningParams)
{

    ble_gap_scan_params_t scanParams;

#if  (NRF_SD_BLE_API_VERSION <= 2)
    /* Allocate the stack's whitelist statically */
    ble_gap_whitelist_t  whitelist;
    ble_gap_addr_t      *whitelistAddressPtrs[YOTTA_CFG_WHITELIST_MAX_SIZE];
    ble_gap_irk_t       *whitelistIrkPtrs[YOTTA_CFG_IRK_TABLE_MAX_SIZE];
    /* Initialize the whitelist */
    whitelist.pp_addrs   = whitelistAddressPtrs;
    whitelist.pp_irks    = whitelistIrkPtrs;
    whitelist.addr_count = 0;
    whitelist.irk_count  = 0;

    /* Add missing IRKs to whitelist from the bond table held by the SoftDevice */
    if (scanningPolicyMode != Gap::SCAN_POLICY_IGNORE_WHITELIST) {
        ble_error_t error = generateStackWhitelist(whitelist);
        if (error != BLE_ERROR_NONE) {
            return error;
        }
    }

    // FIXME: fill the irk list once addresses are resolved by the softdevice.
    scanParams.selective   = scanningPolicyMode;    /**< If 1, ignore unknown devices (non whitelisted). */
    scanParams.p_whitelist = &whitelist; /**< Pointer to whitelist, NULL if none is given. */
#else
    /* For NRF_SD_BLE_API_VERSION >= 3 nRF5xGap::setWhitelist setups the whitelist. */

    scanParams.use_whitelist  = scanningPolicyMode;
    scanParams.adv_dir_report = 0;
#endif

    scanParams.active      = scanningParams.getActiveScanning(); /**< If 1, perform active scanning (scan requests). */

    scanParams.interval    = scanningParams.getInterval();  /**< Scan interval between 0x0004 and 0x4000 in 0.625ms units (2.5ms to 10.24s). */
    scanParams.window      = scanningParams.getWindow();    /**< Scan window between 0x0004 and 0x4000 in 0.625ms units (2.5ms to 10.24s). */
    scanParams.timeout     = scanningParams.getTimeout();   /**< Scan timeout between 0x0001 and 0xFFFF in seconds, 0x0000 disables timeout. */

    if (_privacy_enabled) {
        bool enable_resolution =
            _central_privacy_configuration.resolution_strategy != CentralPrivacyConfiguration_t::DO_NOT_RESOLVE;

        update_identities_list(enable_resolution);

        if (_central_privacy_configuration.use_non_resolvable_random_address) {
            set_private_non_resolvable_address();
        } else {
            set_private_resolvable_address();
        }
    }

    if (sd_ble_gap_scan_start(&scanParams) != NRF_SUCCESS) {
        return BLE_ERROR_PARAM_OUT_OF_RANGE;
    }

    return BLE_ERROR_NONE;
}

ble_error_t nRF5xGap::stopScan(void) {
    if (sd_ble_gap_scan_stop() == NRF_SUCCESS) {
        return BLE_ERROR_NONE;
    }

    return BLE_STACK_BUSY;
}
#endif

/**************************************************************************/
/*!
    @brief  Stops the BLE HW and disconnects from any devices

    @returns    ble_error_t

    @retval     BLE_ERROR_NONE
                Everything executed properly

    @section EXAMPLE

    @code

    @endcode
*/
/**************************************************************************/
ble_error_t nRF5xGap::stopAdvertising(void)
{
    /* Stop Advertising */
    ASSERT_TRUE(ERROR_NONE == sd_ble_gap_adv_stop(), BLE_ERROR_PARAM_OUT_OF_RANGE);

    state.advertising = 0;

    return BLE_ERROR_NONE;
}

ble_error_t nRF5xGap::connect(
    const Address_t peerAddr,
    peer_address_type_t peerAddrType,
    const ConnectionParams_t *connectionParams,
    const GapScanningParams *scanParamsIn
) {
    // NOTE: Nordic address type is an closer to LegacyAddressType: resolved
    // address are treaded either as PUBLIC or RANDOM STATIC adresses.
    // The idea is to get the conversion done here and call the legacy function.

    LegacyAddressType_t legacy_address;

    switch (peerAddrType.value()) {
        case peer_address_type_t::PUBLIC:
        case peer_address_type_t::PUBLIC_IDENTITY:
            legacy_address = LegacyAddressType::PUBLIC;
            break;
        case peer_address_type_t::RANDOM_STATIC_IDENTITY:
            legacy_address = LegacyAddressType::RANDOM_STATIC;
            break;
        case peer_address_type_t::RANDOM: {
            RandomAddressType_t random_address_type(RandomAddressType_t::STATIC);
            ble_error_t err = getRandomAddressType(peerAddr, &random_address_type);
            if (err) {
                return err;
            }
            switch (random_address_type.value()) {
                case RandomAddressType_t::STATIC:
                    legacy_address = LegacyAddressType::RANDOM_STATIC;
                    break;
                case RandomAddressType_t::NON_RESOLVABLE_PRIVATE:
                    legacy_address = LegacyAddressType::RANDOM_PRIVATE_NON_RESOLVABLE;
                    break;
                case RandomAddressType_t::RESOLVABLE_PRIVATE:
                    legacy_address = LegacyAddressType::RANDOM_PRIVATE_RESOLVABLE;
                    break;
                default:
                    return BLE_ERROR_UNSPECIFIED;
            }
        }   break;
        default:
            return BLE_ERROR_INVALID_PARAM;
    }

    bool identity =
        peerAddrType == peer_address_type_t::PUBLIC_IDENTITY ||
        peerAddrType == peer_address_type_t::RANDOM_STATIC_IDENTITY;

    return connect(peerAddr, legacy_address, connectionParams, scanParamsIn, identity);
}


ble_error_t nRF5xGap::connect(
    const Address_t peerAddr,
    LegacyAddressType_t peerAddrType,
    const ConnectionParams_t *connectionParams,
    const GapScanningParams *scanParamsIn
) {
    return connect(peerAddr, peerAddrType, connectionParams, scanParamsIn, false);
}

ble_error_t nRF5xGap::connect(
    const Address_t peerAddr,
    LegacyAddressType_t peerAddrType,
    const ConnectionParams_t *connectionParams,
    const GapScanningParams *scanParamsIn,
    bool identity
) {
    ble_gap_addr_t addr;
    ble_gap_addr_t* addr_ptr = &addr;
    addr.addr_type = peerAddrType;
    memcpy(addr.addr, peerAddr, Gap::ADDR_LEN);

    ble_gap_conn_params_t connParams;
    if (connectionParams != NULL) {
        connParams.min_conn_interval = connectionParams->minConnectionInterval;
        connParams.max_conn_interval = connectionParams->maxConnectionInterval;
        connParams.slave_latency     = connectionParams->slaveLatency;
        connParams.conn_sup_timeout  = connectionParams->connectionSupervisionTimeout;
    } else {
        connParams.min_conn_interval = 50;
        connParams.max_conn_interval = 100;
        connParams.slave_latency     = 0;
        connParams.conn_sup_timeout  = 600;
    }

    ble_gap_scan_params_t scanParams ={0};

#if  (NRF_SD_BLE_API_VERSION <= 2)
    /* Allocate the stack's whitelist statically */
    ble_gap_whitelist_t  whitelist;
    ble_gap_addr_t      *whitelistAddressPtrs[YOTTA_CFG_WHITELIST_MAX_SIZE];
    ble_gap_irk_t       *whitelistIrkPtrs[YOTTA_CFG_IRK_TABLE_MAX_SIZE];
    /* Initialize the whitelist */
    whitelist.pp_addrs   = whitelistAddressPtrs;
    whitelist.pp_irks    = whitelistIrkPtrs;
    whitelist.addr_count = 0;
    whitelist.irk_count  = 0;

    /* Add missing IRKs to whitelist from the bond table held by the SoftDevice */
    if (scanningPolicyMode != Gap::SCAN_POLICY_IGNORE_WHITELIST) {
        ble_error_t error = generateStackWhitelist(whitelist);
        if (error != BLE_ERROR_NONE) {
            return error;
        }
    }

    scanParams.selective   = scanningPolicyMode;    /**< If 1, ignore unknown devices (non whitelisted). */
    scanParams.p_whitelist = &whitelist; /**< Pointer to whitelist, NULL if none is given. */
    if (_privacy_enabled) {
        // configure the "whitelist" with the IRK associated with the identity
        // address in input.
        if (is_identity_address(peerAddrType)) {
            ArrayView<resolving_list_entry_t> entries = get_sm().get_resolving_list();

            size_t i;
            for (i = 0; i < entries.size(); ++i) {
                const ble::address_t& entry_address = entries[i].peer_identity_address;

                // entry found; fill the whitelist and invalidate addr_ptr
                if (memcmp(entry_address.data(), peerAddr, entry_address.size_) == 0) {
                    whitelist.pp_irks[0] = (ble_gap_irk_t*) entries[i].peer_irk.data();
                    whitelist.irk_count = 1;
                    scanParams.selective = 1;
                    addr_ptr = NULL;
                    break;
                }
            }

            // Occur only if the address in input hasn't been resolved.
            if (i == entries.size()) {
                return BLE_ERROR_INVALID_PARAM;
            }
        }

        set_private_resolvable_address();
    }
#else
    /* For NRF_SD_BLE_API_VERSION >= 3 nRF5xGap::setWhitelist setups the whitelist. */

    scanParams.use_whitelist = (whitelistAddressesSize) ? 1 : 0;

    if (_privacy_enabled) {
        bool enable_resolution =
            _central_privacy_configuration.resolution_strategy != CentralPrivacyConfiguration_t::DO_NOT_RESOLVE;

        update_identities_list(enable_resolution);

        if (enable_resolution && identity) {
            addr.addr_id_peer = 1;
        }

        set_private_resolvable_address();
    }

#endif

    if (scanParamsIn != NULL) {
        scanParams.active      = scanParamsIn->getActiveScanning();   /**< If 1, perform active scanning (scan requests). */
        scanParams.interval    = scanParamsIn->getInterval();         /**< Scan interval between 0x0004 and 0x4000 in 0.625ms units (2.5ms to 10.24s). */
        scanParams.window      = scanParamsIn->getWindow();           /**< Scan window between 0x0004 and 0x4000 in 0.625ms units (2.5ms to 10.24s). */
        scanParams.timeout     = scanParamsIn->getTimeout();          /**< Scan timeout between 0x0001 and 0xFFFF in seconds, 0x0000 disables timeout. */
    } else {
        scanParams.active      = _scanningParams.getActiveScanning(); /**< If 1, perform active scanning (scan requests). */
        scanParams.interval    = _scanningParams.getInterval();       /**< Scan interval between 0x0004 and 0x4000 in 0.625ms units (2.5ms to 10.24s). */
        scanParams.window      = _scanningParams.getWindow();         /**< Scan window between 0x0004 and 0x4000 in 0.625ms units (2.5ms to 10.24s). */
        scanParams.timeout     = _scanningParams.getTimeout();        /**< Scan timeout between 0x0001 and 0xFFFF in seconds, 0x0000 disables timeout. */
    }

#if NRF_SD_BLE_API_VERSION >= 5
    uint32_t rc = sd_ble_gap_connect(addr_ptr, &scanParams, &connParams, NRF_CONNECTION_TAG);
#else
    uint32_t rc = sd_ble_gap_connect(addr_ptr, &scanParams, &connParams);
#endif
    if (rc == NRF_SUCCESS) {
        return BLE_ERROR_NONE;
    }
    switch (rc) {
        case NRF_ERROR_INVALID_ADDR:
            return BLE_ERROR_INVALID_PARAM;
        case NRF_ERROR_INVALID_PARAM:
            return BLE_ERROR_INVALID_PARAM;
        case NRF_ERROR_INVALID_STATE:
            return BLE_ERROR_INVALID_STATE;
        case BLE_ERROR_GAP_INVALID_BLE_ADDR:
            return BLE_ERROR_INVALID_PARAM;
        case NRF_ERROR_NO_MEM:
            return BLE_ERROR_NO_MEM;
        case NRF_ERROR_BUSY:
            return BLE_STACK_BUSY;
        default:
        case BLE_ERROR_GAP_WHITELIST_IN_USE:
            return BLE_ERROR_UNSPECIFIED;
    }
}

ble_error_t nRF5xGap::disconnect(Handle_t connectionHandle, DisconnectionReason_t reason)
{
    uint8_t code = BLE_HCI_REMOTE_USER_TERMINATED_CONNECTION;
    switch (reason) {
        case REMOTE_USER_TERMINATED_CONNECTION:
            code = BLE_HCI_REMOTE_USER_TERMINATED_CONNECTION;
            break;
        case CONN_INTERVAL_UNACCEPTABLE:
            code = BLE_HCI_CONN_INTERVAL_UNACCEPTABLE;
            break;
        default:
            break;
    }

    /* Disconnect if we are connected to a central device */
    ASSERT_INT(ERROR_NONE, sd_ble_gap_disconnect(connectionHandle, code), BLE_ERROR_PARAM_OUT_OF_RANGE);

    return BLE_ERROR_NONE;
}

/*!
    @brief  Disconnects if we are connected to a central device

    @returns    ble_error_t

    @retval     BLE_ERROR_NONE
                Everything executed properly
*/
ble_error_t nRF5xGap::disconnect(DisconnectionReason_t reason)
{
    return disconnect(m_connectionHandle, reason);
}

ble_error_t nRF5xGap::getPreferredConnectionParams(ConnectionParams_t *params)
{
    ASSERT_INT(NRF_SUCCESS,
        sd_ble_gap_ppcp_get(reinterpret_cast<ble_gap_conn_params_t *>(params)),
        BLE_ERROR_PARAM_OUT_OF_RANGE);

    return BLE_ERROR_NONE;
}

ble_error_t nRF5xGap::setPreferredConnectionParams(const ConnectionParams_t *params)
{
    ASSERT_INT(NRF_SUCCESS,
        sd_ble_gap_ppcp_set(reinterpret_cast<const ble_gap_conn_params_t *>(params)),
        BLE_ERROR_PARAM_OUT_OF_RANGE);

    return BLE_ERROR_NONE;
}

ble_error_t nRF5xGap::updateConnectionParams(Handle_t handle, const ConnectionParams_t *newParams)
{
    uint32_t rc;

    rc = sd_ble_gap_conn_param_update(handle, reinterpret_cast<ble_gap_conn_params_t *>(const_cast<ConnectionParams_t*>(newParams)));
    if (rc == NRF_SUCCESS) {
        return BLE_ERROR_NONE;
    } else {
        return BLE_ERROR_PARAM_OUT_OF_RANGE;
    }
}

/**************************************************************************/
/*!
    @brief  Clear nRF5xGap's state.

    @returns    ble_error_t

    @retval     BLE_ERROR_NONE
                Everything executed properly
*/
/**************************************************************************/
ble_error_t nRF5xGap::reset(void)
{
    /* Clear all state that is from the parent, including private members */
    if (Gap::reset() != BLE_ERROR_NONE) {
        return BLE_ERROR_INVALID_STATE;
    }

    /* Clear derived class members */
    m_connectionHandle = BLE_CONN_HANDLE_INVALID;

    /* Set the whitelist policy filter modes to IGNORE_WHITELIST */
    advertisingPolicyMode = Gap::ADV_POLICY_IGNORE_WHITELIST;
    scanningPolicyMode    = Gap::SCAN_POLICY_IGNORE_WHITELIST;

    /* Clear the internal whitelist */
    whitelistAddressesSize = 0;

    /* Reset existing mapping between a connection and its role */
    release_all_connections_role();

    return BLE_ERROR_NONE;
}

/**************************************************************************/
/*!
    @brief  Sets the 16-bit connection handle
*/
/**************************************************************************/
void nRF5xGap::setConnectionHandle(uint16_t con_handle)
{
    m_connectionHandle = con_handle;
}

/**************************************************************************/
/*!
    @brief  Gets the 16-bit connection handle
*/
/**************************************************************************/
uint16_t nRF5xGap::getConnectionHandle(void)
{
    return m_connectionHandle;
}

/**************************************************************************/
/*!
    @brief      Sets the BLE device address

    @returns    ble_error_t

    @section EXAMPLE

    @code

    uint8_t device_address[6] = { 0xca, 0xfe, 0xf0, 0xf0, 0xf0, 0xf0 };
    nrf.getGap().setAddress(Gap::BLEProtocol::AddressType::RANDOM_STATIC, device_address);

    @endcode
*/
/**************************************************************************/
ble_error_t nRF5xGap::setAddress(LegacyAddressType_t type, const Address_t address)
{
    if (type != LegacyAddressType::PUBLIC &&
        type != LegacyAddressType::RANDOM_STATIC
    ) {
        return BLE_ERROR_INVALID_PARAM;
    }

    if (_privacy_enabled) {
        return BLE_ERROR_INVALID_STATE;
    }

    ble_gap_addr_t dev_addr;
    memcpy(dev_addr.addr, address, ADDR_LEN);
    if (type == LegacyAddressType::PUBLIC) {
        dev_addr.addr_type = BLE_GAP_ADDR_TYPE_PUBLIC;
    } else {
        dev_addr.addr_type = BLE_GAP_ADDR_TYPE_RANDOM_STATIC;
    }

#if (NRF_SD_BLE_API_VERSION <= 2)
    uint32_t err = sd_ble_gap_address_set(BLE_GAP_ADDR_CYCLE_MODE_NONE, &dev_addr);
#else
    uint32_t err = sd_ble_gap_addr_set(&dev_addr);
#endif

    switch (err) {
        case NRF_SUCCESS:
            return BLE_ERROR_NONE;
        case NRF_ERROR_INVALID_ADDR:
            return BLE_ERROR_INVALID_PARAM;
        case BLE_ERROR_GAP_INVALID_BLE_ADDR:
            return BLE_ERROR_PARAM_OUT_OF_RANGE;
        case NRF_ERROR_BUSY:
            return BLE_STACK_BUSY;
        case NRF_ERROR_INVALID_STATE:
            return BLE_ERROR_INVALID_STATE;
        default:
            return BLE_ERROR_UNSPECIFIED;
    }
}

ble_error_t nRF5xGap::getAddress(AddressType_t *typeP, Address_t address)
{
    // FIXME: check if privacy is enabled ?
    if (typeP == NULL || address == NULL) {
        return BLE_ERROR_INVALID_PARAM;
    }

    ble_gap_addr_t dev_addr;
#if  (NRF_SD_BLE_API_VERSION <= 2)
    if (sd_ble_gap_address_get(&dev_addr) != NRF_SUCCESS) {
#else
    if (sd_ble_gap_addr_get(&dev_addr) != NRF_SUCCESS) {
#endif
        return BLE_ERROR_PARAM_OUT_OF_RANGE;
    }

    switch (dev_addr.addr_type) {
        case BLE_GAP_ADDR_TYPE_PUBLIC:
            *typeP = LegacyAddressType::PUBLIC;
            break;

        case BLE_GAP_ADDR_TYPE_RANDOM_STATIC:
            *typeP = LegacyAddressType::RANDOM_STATIC;
            break;

        default:
            return BLE_ERROR_INVALID_STATE;
    }

    memcpy(address, dev_addr.addr, ADDR_LEN);
    return BLE_ERROR_NONE;
}

ble_error_t nRF5xGap::setDeviceName(const uint8_t *deviceName)
{
    ble_gap_conn_sec_mode_t sec_mode;
    BLE_GAP_CONN_SEC_MODE_SET_OPEN(&sec_mode); // no security is needed

    if (sd_ble_gap_device_name_set(&sec_mode, deviceName, strlen((const char *)deviceName)) == NRF_SUCCESS) {
        return BLE_ERROR_NONE;
    } else {
        return BLE_ERROR_PARAM_OUT_OF_RANGE;
    }
}

ble_error_t nRF5xGap::getDeviceName(uint8_t *deviceName, unsigned *lengthP)
{
    if (sd_ble_gap_device_name_get(deviceName, (uint16_t *)lengthP) == NRF_SUCCESS) {
        return BLE_ERROR_NONE;
    } else {
        return BLE_ERROR_PARAM_OUT_OF_RANGE;
    }
}

ble_error_t nRF5xGap::setAppearance(GapAdvertisingData::Appearance appearance)
{
    if (sd_ble_gap_appearance_set(appearance) == NRF_SUCCESS) {
        return BLE_ERROR_NONE;
    } else {
        return BLE_ERROR_PARAM_OUT_OF_RANGE;
    }
}

ble_error_t nRF5xGap::getAppearance(GapAdvertisingData::Appearance *appearanceP)
{
    if ((sd_ble_gap_appearance_get(reinterpret_cast<uint16_t *>(appearanceP)) == NRF_SUCCESS)) {
        return BLE_ERROR_NONE;
    } else {
        return BLE_ERROR_PARAM_OUT_OF_RANGE;
    }
}

/* (Valid values are -40, -20, -16, -12, -8, -4, 0, 4) */
ble_error_t nRF5xGap::setTxPower(int8_t txPower)
{
    unsigned rc;
    if ((rc = sd_ble_gap_tx_power_set(txPower)) != NRF_SUCCESS) {
        switch (rc) {
            case NRF_ERROR_BUSY:
                return BLE_STACK_BUSY;
            case NRF_ERROR_INVALID_PARAM:
            default:
                return BLE_ERROR_PARAM_OUT_OF_RANGE;
        }
    }

    return BLE_ERROR_NONE;
}

void nRF5xGap::getPermittedTxPowerValues(const int8_t **valueArrayPP, size_t *countP)
{
#if defined(NRF51)
    static const int8_t permittedTxValues[] = {
        -30, -20, -16, -12, -8, -4, 0, 4
    };
#elif defined(NRF52)
#if  NRF_SD_BLE_API_VERSION >= 5
    static const int8_t permittedTxValues[] = {
        -40, -20, -16, -12, -8, -4, 0, 3, 4
    };
#else
    static const int8_t permittedTxValues[] = {
        -40, -20, -16, -12, -8, -4, 0, 4
    };
#endif
#elif defined(NRF52840_XXAA)
    static const int8_t permittedTxValues[] = {
        -40, -20, -16, -12, -8, -4, 0, 2, 3, 4, 5, 6, 7, 8, 9
    };
#else
#error permitted TX power values unknown for this SOC
#endif

    *valueArrayPP = permittedTxValues;
    *countP = sizeof(permittedTxValues) / sizeof(int8_t);
}

/**************************************************************************/
/*!
    @brief  Get the capacity of the internal whitelist maintained by this
            implementation.

    @returns    The capacity of the internal whitelist.

    @section EXAMPLE

    @code

    @endcode
*/
/**************************************************************************/
uint8_t nRF5xGap::getMaxWhitelistSize(void) const
{
    return YOTTA_CFG_WHITELIST_MAX_SIZE;
}

/**************************************************************************/
/*!
    @brief  Get a copy of the implementation's internal whitelist.

    @param[out] whitelistOut
                A \ref Gap::Whitelist_t structure containing a copy of the
                addresses in the implemenetation's internal whitelist.

    @returns    \ref ble_errror_t

    @retval     BLE_ERROR_NONE
                Everything executed properly.

    @section EXAMPLE

    @code

    @endcode
*/
/**************************************************************************/
ble_error_t nRF5xGap::getWhitelist(Gap::Whitelist_t &whitelistOut) const
{
    uint32_t i;
    for (i = 0; i < whitelistAddressesSize && i < whitelistOut.capacity; ++i) {
        memcpy( &whitelistOut.addresses[i].address, &whitelistAddresses[i].addr, sizeof(whitelistOut.addresses[0].address));
        whitelistOut.addresses[i].type = static_cast<LegacyAddressType_t> (whitelistAddresses[i].addr_type);


    }
    whitelistOut.size = i;

    return BLE_ERROR_NONE;
}

/**************************************************************************/
/*!
    @brief  Set the whitelist that will be used in the next call to
            startAdvertising().

    @param[in]  whitelistIn
                A reference to a \ref Gap::Whitelist_t structure
                representing a whitelist containing all the white listed
                BLE addresses.

    @returns    \ref ble_errror_t

    @retval     BLE_ERROR_NONE
                Everything executed properly.

                BLE_ERROR_INVALID_PARAM
                The supplied whitelist contains a private non-resolvable
                address

                BLE_ERROR_PARAM_OUT_OF_RANGE
                The size of the supplied whitelist exceeds the maximum
                capacity of the implementation's internal whitelist.

    @section EXAMPLE

    @code

    @endcode
*/
/**************************************************************************/
ble_error_t nRF5xGap::setWhitelist(const Gap::Whitelist_t &whitelistIn)
{
    if (whitelistIn.size > getMaxWhitelistSize()) {
        return BLE_ERROR_PARAM_OUT_OF_RANGE;
    }

    /* Test for invalid parameters before we change the internal state */
    for (uint32_t i = 0; i < whitelistIn.size; ++i) {
        if (whitelistIn.addresses[i].type == LegacyAddressType::RANDOM_PRIVATE_NON_RESOLVABLE ||
            whitelistIn.addresses[i].type == LegacyAddressType::RANDOM_PRIVATE_RESOLVABLE
        ) {
            /* This is not allowed because it is completely meaningless */
            return BLE_ERROR_INVALID_PARAM;
        }
    }

    whitelistAddressesSize = whitelistIn.size;
    ble_gap_addr_t* pp_addrs[YOTTA_CFG_WHITELIST_MAX_SIZE];

    for (uint32_t i = 0; i < whitelistIn.size; ++i) {
        memcpy(&whitelistAddresses[i].addr, &whitelistIn.addresses[i].address , sizeof(whitelistAddresses[0].addr));
        whitelistAddresses[i].addr_type = static_cast<uint8_t> (whitelistIn.addresses[i].type);
        pp_addrs[i] = &whitelistAddresses[i];
    }

    ble_gap_addr_t** addresses_list_ptr = (whitelistIn.size == 0) ? NULL : pp_addrs;

    uint32_t err = sd_ble_gap_whitelist_set(addresses_list_ptr, whitelistAddressesSize);

    switch(err) {
        case NRF_SUCCESS:
            return BLE_ERROR_NONE;

        case BLE_ERROR_GAP_WHITELIST_IN_USE:
            return BLE_ERROR_INVALID_STATE;

        case NRF_ERROR_INVALID_ADDR:
        case BLE_ERROR_GAP_INVALID_BLE_ADDR:
        case NRF_ERROR_DATA_SIZE:
            return BLE_ERROR_INVALID_PARAM;

        default:
            return BLE_ERROR_UNSPECIFIED;
    }
}

/**************************************************************************/
/*!
    @brief  Set the advertising policy filter mode that will be used in
            the next call to startAdvertising().

    @returns    \ref ble_errror_t

    @retval     BLE_ERROR_NONE
                Everything executed properly.

                BLE_ERROR_NOT_IMPLEMENTED
                This feature is currently note implemented.

    @section EXAMPLE

    @code

    @endcode
*/
/**************************************************************************/
ble_error_t nRF5xGap::setAdvertisingPolicyMode(Gap::AdvertisingPolicyMode_t mode)
{
    advertisingPolicyMode = mode;
    return BLE_ERROR_NONE;
}

/**************************************************************************/
/*!
    @brief  Set the scanning policy filter mode that will be used in
            the next call to startAdvertising().

    @returns    \ref ble_errror_t

    @retval     BLE_ERROR_NONE
                Everything executed properly.

                BLE_ERROR_NOT_IMPLEMENTED
                This feature is currently note implemented.

    @section EXAMPLE

    @code

    @endcode
*/
/**************************************************************************/
ble_error_t nRF5xGap::setScanningPolicyMode(Gap::ScanningPolicyMode_t mode)
{
    scanningPolicyMode = mode;

    return BLE_ERROR_NONE;
}

/**************************************************************************/
/*!
    @brief  Set the initiator policy filter mode that will be used in
            the next call to startAdvertising()

    @returns    \ref ble_errror_t

    @retval     BLE_ERROR_NONE
                Everything executed properly.

                BLE_ERROR_NOT_IMPLEMENTED
                This feature is currently note implemented.

    @section EXAMPLE

    @code

    @endcode
*/
/**************************************************************************/
ble_error_t nRF5xGap::setInitiatorPolicyMode(Gap::InitiatorPolicyMode_t mode)
{
    return BLE_ERROR_NOT_IMPLEMENTED;
}

/**************************************************************************/
/*!
    @brief  Get the current advertising policy filter mode.

    @returns    The advertising policy filter mode.

    @section EXAMPLE

    @code

    @endcode
*/
/**************************************************************************/
Gap::AdvertisingPolicyMode_t nRF5xGap::getAdvertisingPolicyMode(void) const
{
    return advertisingPolicyMode;
}

/**************************************************************************/
/*!
    @brief  Get the current scanning policy filter mode.

    @returns    The scanning policy filter mode.

    @section EXAMPLE

    @code

    @endcode
*/
/**************************************************************************/
Gap::ScanningPolicyMode_t nRF5xGap::getScanningPolicyMode(void) const
{
    return scanningPolicyMode;
}

/**************************************************************************/
/*!
    @brief  Get the current initiator policy filter mode.

    @returns    The initiator policy filter mode.

    @note   Currently initiator filtering using the whitelist is not
            implemented in this module.

    @section EXAMPLE

    @code

    @endcode
*/
/**************************************************************************/
Gap::InitiatorPolicyMode_t nRF5xGap::getInitiatorPolicyMode(void) const
{
    return Gap::INIT_POLICY_IGNORE_WHITELIST;
}

ble_error_t nRF5xGap::enablePrivacy(bool enable_privacy)
{
    if (enable_privacy == _privacy_enabled) {
        return BLE_ERROR_NONE;
    }

    ble_error_t err = BLE_ERROR_UNSPECIFIED;
    if (enable_privacy == false) {
        err = setAddress(_non_private_address_type, _non_private_address);
    } else {
        err = getAddress(&_non_private_address_type, _non_private_address);
    }

    if (err) {
        return err;
    }

#if (NRF_SD_BLE_API_VERSION > 2)
    ble_gap_privacy_params_t privacy_config = { 0 };
    if (sd_ble_gap_privacy_get(&privacy_config)) {
        return BLE_ERROR_UNSPECIFIED;
    }

    privacy_config.privacy_mode = enable_privacy ?
        BLE_GAP_PRIVACY_MODE_DEVICE_PRIVACY :
        BLE_GAP_PRIVACY_MODE_OFF;
    if (sd_ble_gap_privacy_set(&privacy_config)) {
        return BLE_ERROR_UNSPECIFIED;
    }
#endif

    _privacy_enabled = enable_privacy;
    return BLE_ERROR_NONE;
}

ble_error_t nRF5xGap::setPeripheralPrivacyConfiguration(
    const PeripheralPrivacyConfiguration_t *configuration
) {
    _peripheral_privacy_configuration = *configuration;
    return BLE_ERROR_NONE;
}

ble_error_t nRF5xGap::getPeripheralPrivacyConfiguration(
    PeripheralPrivacyConfiguration_t *configuration
) {
    *configuration = _peripheral_privacy_configuration;
    return BLE_ERROR_NONE;
}

ble_error_t nRF5xGap::setCentralPrivacyConfiguration(
    const CentralPrivacyConfiguration_t *configuration
) {
    _central_privacy_configuration = *configuration;
    return BLE_ERROR_NONE;
}

ble_error_t nRF5xGap::getCentralPrivacyConfiguration(
    CentralPrivacyConfiguration_t *configuration
) {
    *configuration = _central_privacy_configuration;
    return BLE_ERROR_NONE;
}

void nRF5xGap::set_connection_event_handler(
    ConnectionEventMonitor::EventHandler* connection_event_handler
) {
    _connection_event_handler = connection_event_handler;
}

void nRF5xGap::processDisconnectionEvent(
    Handle_t handle,
    DisconnectionReason_t reason
) {
    release_connection_role(handle);

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

ble_error_t nRF5xGap::update_identities_list(bool resolution_enabled)
{
    uint32_t err;

    if (resolution_enabled) {
        ArrayView<ble_gap_id_key_t> entries = get_sm().get_resolving_list();
        size_t limit = std::min(
            entries.size(), (size_t) YOTTA_CFG_IRK_TABLE_MAX_SIZE
        );
        ble_gap_id_key_t* id_keys_pp[YOTTA_CFG_IRK_TABLE_MAX_SIZE];

        for (size_t i = 0; i < limit; ++i) {
            id_keys_pp[i] = &entries[i];
        }

        err = sd_ble_gap_device_identities_set(
            limit ? id_keys_pp : NULL,
            /* use the local IRK for all devices */ NULL,
            limit
        );
    } else {
        err = sd_ble_gap_device_identities_set(
            NULL,
            /* use the local IRK for all devices */ NULL,
            0
        );
    }

    return err ? BLE_ERROR_INVALID_STATE : BLE_ERROR_NONE;
}

void nRF5xGap::on_connection(Gap::Handle_t handle, const ble_gap_evt_connected_t& evt) {
    // set the new connection handle as the _default_ handle in gap
    setConnectionHandle(handle);

    // add the connection and the role of the device in the local table
    allocate_connection_role(handle, static_cast<Role_t>(evt.role));

    // deal with own address
    LegacyAddressType_t own_addr_type;
    ble::address_t own_address;
    const uint8_t* own_resolvable_address = NULL;

#if  (NRF_SD_BLE_API_VERSION <= 2)
    if (evt.own_addr.addr_type == BLE_GAP_ADDR_TYPE_PUBLIC) {
        own_addr_type = AddressType::PUBLIC;
    } else {
        own_addr_type = AddressType::RANDOM;
    }
    memcpy(own_address, evt.own_addr.addr, sizeof(own_address));
#else
    getAddress(&own_addr_type, own_address.data());
    if (_privacy_enabled) {
        own_addr_type = LegacyAddressType::RANDOM_PRIVATE_RESOLVABLE;
    }
#endif

#if (NRF_SD_BLE_API_VERSION <= 2)
    bool private_peer_known = evt.irk_match;
#else
    bool private_peer_known = evt.peer_addr.addr_id_peer;
#endif

    // Filter out private address non resolved if the its required by the
    // resolution policy
    if (_privacy_enabled &&
        evt.role == BLE_GAP_ROLE_PERIPH &&
        _peripheral_privacy_configuration.resolution_strategy == PeripheralPrivacyConfiguration_t::REJECT_NON_RESOLVED_ADDRESS &&
        evt.peer_addr.addr_type == BLE_GAP_ADDR_TYPE_RANDOM_PRIVATE_RESOLVABLE &&
        private_peer_known == false &&
        get_sm().get_resolving_list().size() > 0
    ) {
        // FIXME: should use BLE_HCI_AUTHENTICATION_FAILURE; not possible
        // with the softdevice ...
        sd_ble_gap_disconnect(handle, BLE_HCI_REMOTE_USER_TERMINATED_CONNECTION);
        return;
    }

    // deal with the peer address: If privacy is enabled then the softdevice
    // indicates if the address has been resolved or not. If the address has
    // been resolved then the identity address should be passed to the application.
    // Depending on the privacy chosen by the application, connection request
    // from privacy enabled peers may trigger a disconnection, the pairing procedure
    // or the authentication procedure.
    peer_address_type_t peer_addr_type = convert_nordic_address(
        private_peer_known,
        evt.peer_addr.addr_type
    );

    // NOTE: when privacy is enabled, the only address returned is the resolved
    // address.
    const uint8_t* peer_address = evt.peer_addr.addr;
    const uint8_t* peer_resolvable_address = NULL;

    // notify internal event handler before applying the resolution strategy
    if (_connection_event_handler) {
        _connection_event_handler->on_connected(
            handle,
            static_cast<Role_t>(evt.role),
            peer_addr_type,
            peer_address,
            own_addr_type,
            own_address.data(),
            reinterpret_cast<const ConnectionParams_t *>(&(evt.conn_params))
        );
    }

    // Apply authentication strategy before application notification
    if (_privacy_enabled &&
        !private_peer_known &&
        evt.role == BLE_GAP_ROLE_PERIPH &&
        evt.peer_addr.addr_type == BLE_GAP_ADDR_TYPE_RANDOM_PRIVATE_RESOLVABLE
    ) {
        switch (_peripheral_privacy_configuration.resolution_strategy) {
            case PeripheralPrivacyConfiguration_t::PERFORM_PAIRING_PROCEDURE:
                nRF5xn::Instance(BLE::DEFAULT_INSTANCE).getSecurityManager().requestAuthentication(handle);
                break;

            case PeripheralPrivacyConfiguration_t::PERFORM_AUTHENTICATION_PROCEDURE:
                // FIXME: lookup secure DB to know what to do.
                break;

            default:
                break;
        }
    }

    processConnectionEvent(
        handle,
        static_cast<Role_t>(evt.role),
        peer_addr_type,
        peer_address,
        own_addr_type,
        own_address.data(),
        reinterpret_cast<const ConnectionParams_t *>(&(evt.conn_params)),
        peer_resolvable_address,
        own_resolvable_address
    );
}

void nRF5xGap::on_advertising_packet(const ble_gap_evt_adv_report_t &evt) {
    bool peer_address_resolved = evt.peer_addr.addr_id_peer;

    if (_privacy_enabled &&
        evt.peer_addr.addr_type == BLE_GAP_ADDR_TYPE_RANDOM_PRIVATE_RESOLVABLE &&
        peer_address_resolved == false &&
        get_sm().get_resolving_list().size() > 0 &&
        _central_privacy_configuration.resolution_strategy == CentralPrivacyConfiguration_t::RESOLVE_AND_FILTER
    ) {
        return;
    }

    peer_address_type_t peer_addr_type = convert_nordic_address(
        evt.peer_addr.addr_id_peer,
        evt.peer_addr.addr_type
    );
    const uint8_t* peer_address = evt.peer_addr.addr;

    processAdvertisementReport(
        peer_address,
        evt.rssi,
        evt.scan_rsp,
        static_cast<GapAdvertisingParams::AdvertisingType_t>(evt.type),
        evt.dlen,
        evt.data,
        peer_addr_type
    );
}

ble_error_t nRF5xGap::get_role(ble::connection_handle_t connection, Role_t& role) {
    for (size_t i = 0; i < max_connections_count; ++i) {
        connection_role_t& c = _connections_role[i];
        if (c.is_allocated && c.connection == connection) {
            role = c.is_peripheral ? PERIPHERAL : CENTRAL;
            return BLE_ERROR_NONE;
        }
    }

    return BLE_ERROR_INVALID_PARAM;
}

void nRF5xGap::allocate_connection_role(
    ble::connection_handle_t connection,
    Role_t role
) {
    for (size_t i = 0; i < max_connections_count; ++i) {
        connection_role_t& c = _connections_role[i];
        if (c.is_allocated == false) {
            c.connection = connection;
            c.is_peripheral = (role == Gap::PERIPHERAL);
            c.is_allocated = true;
            return;
        }
    }
}
void nRF5xGap::release_connection_role(ble::connection_handle_t connection) {
    for (size_t i = 0; i < max_connections_count; ++i) {
        connection_role_t& c = _connections_role[i];
        if (c.is_allocated && c.connection == connection) {
            c.is_allocated = false;
            return;
        }
    }
}

void nRF5xGap::release_all_connections_role() {
    for (size_t i = 0; i < max_connections_count; ++i) {
        _connections_role[i].is_allocated = false;
    }
}



