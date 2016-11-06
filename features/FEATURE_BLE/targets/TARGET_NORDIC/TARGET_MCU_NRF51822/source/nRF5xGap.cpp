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
#include "ble_advdata.h"
#include "ble_hci.h"

void radioNotificationStaticCallback(bool param) {
    nRF5xGap &gap = (nRF5xGap &) nRF5xn::Instance(BLE::DEFAULT_INSTANCE).getGap();
    gap.processRadioNotificationEvent(param);
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

    /* Start Advertising */
    ble_gap_adv_params_t adv_para = {0};

    adv_para.type        = params.getAdvertisingType();
    adv_para.p_peer_addr = NULL;                           // Undirected advertisement
    adv_para.fp          = advertisingPolicyMode;
    adv_para.p_whitelist = &whitelist;
    adv_para.interval    = params.getIntervalInADVUnits(); // advertising interval (in units of 0.625 ms)
    adv_para.timeout     = params.getTimeout();

    ASSERT_TRUE(ERROR_NONE == sd_ble_gap_adv_start(&adv_para), BLE_ERROR_PARAM_OUT_OF_RANGE);

    return BLE_ERROR_NONE;
}

/* Observer role is not supported by S110, return BLE_ERROR_NOT_IMPLEMENTED */
#if !defined(TARGET_MCU_NRF51_16K_S110) && !defined(TARGET_MCU_NRF51_32K_S110)
ble_error_t nRF5xGap::startRadioScan(const GapScanningParams &scanningParams)
{
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

    ble_gap_scan_params_t scanParams = {
        .active      = scanningParams.getActiveScanning(), /**< If 1, perform active scanning (scan requests). */
        .selective   = scanningPolicyMode,    /**< If 1, ignore unknown devices (non whitelisted). */
        .p_whitelist = &whitelist, /**< Pointer to whitelist, NULL if none is given. */
        .interval    = scanningParams.getInterval(),  /**< Scan interval between 0x0004 and 0x4000 in 0.625ms units (2.5ms to 10.24s). */
        .window      = scanningParams.getWindow(),    /**< Scan window between 0x0004 and 0x4000 in 0.625ms units (2.5ms to 10.24s). */
        .timeout     = scanningParams.getTimeout(),   /**< Scan timeout between 0x0001 and 0xFFFF in seconds, 0x0000 disables timeout. */
    };

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

ble_error_t nRF5xGap::connect(const Address_t             peerAddr,
                              BLEProtocol::AddressType_t  peerAddrType,
                              const ConnectionParams_t   *connectionParams,
                              const GapScanningParams    *scanParamsIn)
{
    ble_gap_addr_t addr;
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

    ble_gap_scan_params_t scanParams;
    scanParams.selective   = scanningPolicyMode;    /**< If 1, ignore unknown devices (non whitelisted). */
    scanParams.p_whitelist = &whitelist; /**< Pointer to whitelist, NULL if none is given. */
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

    uint32_t rc = sd_ble_gap_connect(&addr, &scanParams, &connParams);
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
ble_error_t nRF5xGap::setAddress(AddressType_t type, const Address_t address)
{
    uint8_t cycle_mode;
    ble_gap_addr_t dev_addr;

    /* When using Public or Static addresses, the cycle mode must be None.
       When using Random Private addresses, the cycle mode must be Auto.
       In auto mode, the given address is ignored.
    */
    if ((type == BLEProtocol::AddressType::PUBLIC) || (type == BLEProtocol::AddressType::RANDOM_STATIC))
    {
        cycle_mode = BLE_GAP_ADDR_CYCLE_MODE_NONE;
        memcpy(dev_addr.addr, address, ADDR_LEN);
    }
    else if ((type == BLEProtocol::AddressType::RANDOM_PRIVATE_RESOLVABLE) || (type == BLEProtocol::AddressType::RANDOM_PRIVATE_NON_RESOLVABLE))
    {
        cycle_mode = BLE_GAP_ADDR_CYCLE_MODE_AUTO;
        // address is ignored when in auto mode
    }
    else
    {
        return BLE_ERROR_PARAM_OUT_OF_RANGE;
    }

    dev_addr.addr_type = type;
    ASSERT_INT(ERROR_NONE, sd_ble_gap_address_set(cycle_mode, &dev_addr), BLE_ERROR_PARAM_OUT_OF_RANGE);

    return BLE_ERROR_NONE;
}

ble_error_t nRF5xGap::getAddress(AddressType_t *typeP, Address_t address)
{
    ble_gap_addr_t dev_addr;
    if (sd_ble_gap_address_get(&dev_addr) != NRF_SUCCESS) {
        return BLE_ERROR_PARAM_OUT_OF_RANGE;
    }

    if (typeP != NULL) {
        *typeP = static_cast<AddressType_t>(dev_addr.addr_type);
    }
    if (address != NULL) {
        memcpy(address, dev_addr.addr, ADDR_LEN);
    }
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
    static const int8_t permittedTxValues[] = {
        -40, -30, -20, -16, -12, -8, -4, 0, 4
    };

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
    uint8_t i;
    for (i = 0; i < whitelistAddressesSize && i < whitelistOut.capacity; ++i) {
        memcpy(&whitelistOut.addresses[i], &whitelistAddresses[i], sizeof(BLEProtocol::Address_t));
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
    for (uint8_t i = 0; i < whitelistIn.size; ++i) {
        if (whitelistIn.addresses[i].type == BLEProtocol::AddressType::RANDOM_PRIVATE_NON_RESOLVABLE) {
            /* This is not allowed because it is completely meaningless */
            return BLE_ERROR_INVALID_PARAM;
        }
    }

    whitelistAddressesSize = 0;
    for (uint8_t i = 0; i < whitelistIn.size; ++i) {
        memcpy(&whitelistAddresses[whitelistAddressesSize], &whitelistIn.addresses[i], sizeof(BLEProtocol::Address_t));
        whitelistAddressesSize++;
    }

    return BLE_ERROR_NONE;
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

/**************************************************************************/
/*!
    @brief  Helper function used to populate the ble_gap_whitelist_t that
            will be used by the SoftDevice for filtering requests.

    @returns    \ref ble_error_t

    @retval     BLE_ERROR_NONE
                Everything executed properly

    @retval     BLE_ERROR_INVALID_STATE
                The internal stack was not initialized correctly.

    @note  Both the SecurityManager and Gap must initialize correctly for
           this function to succeed.

    @note  This function is needed because for the BLE API the whitelist
           is just a collection of keys, but for the stack it also includes
           the IRK table.

    @section EXAMPLE

    @code

    @endcode
*/
/**************************************************************************/
ble_error_t nRF5xGap::generateStackWhitelist(ble_gap_whitelist_t &whitelist)
{
    ble_gap_whitelist_t  whitelistFromBondTable;
    ble_gap_addr_t      *addressPtr[1];
    ble_gap_irk_t       *irkPtr[YOTTA_CFG_IRK_TABLE_MAX_SIZE];

    nRF5xSecurityManager& securityManager = (nRF5xSecurityManager&) nRF5xn::Instance(0).getSecurityManager();

    if (securityManager.hasInitialized()) {
        /* We do not care about the addresses, set the count to 0 */
        whitelistFromBondTable.addr_count = 0;
        /* The Nordic SDK will return a failure if we set pp_addr to NULL */
        whitelistFromBondTable.pp_addrs   = addressPtr;
        /* We want all the IRKs we can get because we do not know which ones match the addresses */
        whitelistFromBondTable.irk_count  = YOTTA_CFG_IRK_TABLE_MAX_SIZE;
        whitelistFromBondTable.pp_irks    = irkPtr;

        /* Use the security manager to get the IRKs from the bond table */
        ble_error_t error = securityManager.createWhitelistFromBondTable(whitelistFromBondTable);
        if (error != BLE_ERROR_NONE) {
            return error;
        }
    } else  {
        /**
         * If there is no security manager then we cannot access the bond table,
         * so disable IRK matching
         */
        whitelistFromBondTable.addr_count = 0;
        whitelistFromBondTable.irk_count  = 0;
    }

    /**
     * For every private resolvable address in the local whitelist check if
     * there is an IRK for said address in the bond table and add it to the
     * local IRK list.
     */
    whitelist.irk_count  = 0;
    whitelist.addr_count = 0;
    for (uint8_t i = 0; i < whitelistAddressesSize; ++i) {
        if (whitelistAddresses[i].addr_type == BLEProtocol::AddressType::RANDOM_PRIVATE_RESOLVABLE) {
            /* Test if there is a matching IRK for this private resolvable address */
            for (uint8_t j = 0; j < whitelistFromBondTable.irk_count; ++j) {
                if (securityManager.matchAddressAndIrk(&whitelistAddresses[i], whitelistFromBondTable.pp_irks[j])) {
                    /* Found the corresponding IRK, add it to our local whitelist */
                    whitelist.pp_irks[whitelist.irk_count] = whitelistFromBondTable.pp_irks[j];
                    whitelist.irk_count++;
                    /* Make sure we do not look at this IRK again */
                    if (j != whitelistFromBondTable.irk_count - 1) {
                        /**
                         * This is not the last IRK, so replace the pointer
                         * with the last pointer in the array
                         */
                        whitelistFromBondTable.pp_irks[j] =
                            whitelistFromBondTable.pp_irks[whitelistFromBondTable.irk_count - 1];
                    }
                    /**
                     * If the IRK is the last pointer in the array simply
                     * decrement the total IRK count
                     */
                    whitelistFromBondTable.irk_count--;
                    break;
                }
            }
        } else {
            /* Include the address into the whitelist */
            whitelist.pp_addrs[whitelist.addr_count] = &whitelistAddresses[i];
            whitelist.addr_count++;
        }
    }

    return BLE_ERROR_NONE;
}
