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

#ifndef MBED_GAP_ADVERTISING_PARAMS_H__
#define MBED_GAP_ADVERTISING_PARAMS_H__

#include "BLETypes.h"
#include "BLEProtocol.h"
#include "blecommon.h"
#include "SafeEnum.h"

/* TODO: std::clamp */
#define CLAMP(value, min, max) \
    if (value > max) { \
        value = max; \
    } else if (value < min) { \
        value = min; \
    }

/**
 * @addtogroup ble
 * @{
 * @addtogroup gap
 * @{
 */

/**
 * Parameters defining the advertising process.
 *
 * Advertising parameters for legacy advertising are a triplet of three value:
 *   - The Advertising mode modelled after ble::advertising_type_t. It defines
 *     if the device is connectable and scannable. This value can be set at
 *     construction time, updated with setAdvertisingType() and queried by
 *     getAdvertisingType().
 *   - Time interval between advertisement. It can be set at construction time,
 *     updated by setInterval() and obtained from getInterval().
 *   - Duration of the advertising process. As others, it can be set at
 *     construction time, modified by setTimeout() and retrieved by getTimeout().
 */
class GapAdvertisingParams {
public:

    /**
     * Minimum Advertising interval for connectable undirected and connectable
     * directed events in 625us units.
     *
     * @note Equal to 20 ms.
     */
    static const unsigned GAP_ADV_PARAMS_INTERVAL_MIN = 0x0020;

    /**
     * Minimum Advertising interval for scannable and nonconnectable
     * undirected events in 625us units.
     *
     * @note Equal to 100ms.
     */
    static const unsigned GAP_ADV_PARAMS_INTERVAL_MIN_NONCON = 0x00A0;

    /**
     * Maximum Advertising interval in 625us units.
     *
     * @note Equal to 10.24s.
     */
    static const unsigned GAP_ADV_PARAMS_INTERVAL_MAX = 0x4000;

    /**
     * Maximum advertising timeout allowed; in seconds.
     */
    static const unsigned GAP_ADV_PARAMS_TIMEOUT_MAX = 0x3FFF;

    /**
     * Alias for GapAdvertisingParams::ble::advertising_type_t.
     *
     * @deprecated  Future releases will drop this type alias.
     */
    typedef ble::advertising_type_t AdvertisingType;

    typedef ble::advertising_type_t AdvertisingType_t;

    static const ble::advertising_type_t ADV_CONNECTABLE_UNDIRECTED = ble::ADV_CONNECTABLE_UNDIRECTED;
    static const ble::advertising_type_t ADV_CONNECTABLE_DIRECTED = ble::ADV_CONNECTABLE_DIRECTED;
    static const ble::advertising_type_t ADV_SCANNABLE_UNDIRECTED = ble::ADV_SCANNABLE_UNDIRECTED;
    static const ble::advertising_type_t ADV_NON_CONNECTABLE_UNDIRECTED = ble::ADV_NON_CONNECTABLE_UNDIRECTED;

    struct own_address_type_t : ble::SafeEnum<own_address_type_t, uint8_t> {
        enum type {
            PUBLIC = 0, /**< Public Device Address. */
            RANDOM,     /**< Random Device Address. */
            RANDOM_RESOLVABLE_PUBLIC_FALLBACK, /**< Controller generates the Resolvable Private Address based on
                                                    the local IRK from the resolving list. If the resolving list
                                                    contains no matching entry, use the public address. */
            RANDOM_RESOLVABLE_RANDOM_FALLBACK  /**< Controller generates the Resolvable Private Address based on
                                                    the local IRK from the resolving list. If the resolving list
                                                    contains no matching entry, use previously set random address. */
        };
        own_address_type_t(type value) : ble::SafeEnum<own_address_type_t, uint8_t>(value) { }
    };

    struct peer_address_type_t : ble::SafeEnum<peer_address_type_t, uint8_t> {
        enum type {
            PUBLIC = 0, /**< Public Device Address or Public Identity Address. */
            RANDOM      /**< Random Device Address or Random (static) Identity Address. */
        };
        peer_address_type_t(type value) : ble::SafeEnum<peer_address_type_t, uint8_t>(value) { }
    };

public:
    /**
     * Construct an instance of GapAdvertisingParams.
     *
     * @param[in] advType Type of advertising.
     * @param[in] interval Time interval between two advertisement in units of
     * 0.625ms.
     * @param[in] timeout Duration in seconds of the advertising process. A
     * value of 0 indicate that there is no timeout of the advertising process.
     *
     * @note If value in input are out of range, they will be normalized.
     */
    GapAdvertisingParams(
        ble::advertising_type_t advType  = ble::ADV_CONNECTABLE_UNDIRECTED,
        uint16_t interval = GAP_ADV_PARAMS_INTERVAL_MIN_NONCON,
        uint16_t timeout = 0
    ) :
        _advType(advType),
        _interval(interval),
        _maxInterval(interval),
        _timeout(timeout),
        _peerAddressType(peer_address_type_t::PUBLIC),
        _ownAddressType(own_address_type_t::PUBLIC),
        _policy(ble::ADV_POLICY_IGNORE_WHITELIST),
        _primaryPhy(ble::phy_t::LE_1M),
        _secondaryPhy(ble::phy_t::LE_1M),
        _peerAddress(),
        _txPower(127),
        _maxSkip(0),
        _channel37(1),
        _channel38(1),
        _channel39(1),
        _anonymous(0),
        _notifyOnScan(0),
        _legacyPDU(1),
        _includeHeaderTxPower(0)
    {
        /* Interval checks. */
        if (_advType == ble::ADV_CONNECTABLE_DIRECTED) {
            /* Interval must be 0 in directed connectable mode. */
            _interval = 0;
            _maxInterval = 0;
        } else if (_advType == ble::ADV_NON_CONNECTABLE_UNDIRECTED) {
            /* Min interval is slightly larger than in other modes. */
            CLAMP(_interval, GAP_ADV_PARAMS_INTERVAL_MIN_NONCON, GAP_ADV_PARAMS_INTERVAL_MAX);
            CLAMP(_maxInterval, GAP_ADV_PARAMS_INTERVAL_MIN_NONCON, GAP_ADV_PARAMS_INTERVAL_MAX);
        } else {
            CLAMP(_interval, GAP_ADV_PARAMS_INTERVAL_MIN, GAP_ADV_PARAMS_INTERVAL_MAX);
            CLAMP(_maxInterval, GAP_ADV_PARAMS_INTERVAL_MIN, GAP_ADV_PARAMS_INTERVAL_MAX);
        }

        /* Timeout checks. */
        if (timeout) {
            /* Stay within timeout limits. */
            if (_timeout > GAP_ADV_PARAMS_TIMEOUT_MAX) {
                _timeout = GAP_ADV_PARAMS_TIMEOUT_MAX;
            }
        }
    }

public:
    /**
     * Number of microseconds in 0.625 milliseconds.
     */
    static const uint16_t UNIT_0_625_MS = 625;

    /**
     * Convert milliseconds to units of 0.625ms.
     *
     * @param[in] durationInMillis Number of milliseconds to convert.
     *
     * @return The value of @p durationInMillis in units of 0.625ms.
     */
    static uint16_t MSEC_TO_ADVERTISEMENT_DURATION_UNITS(uint32_t durationInMillis)
    {
        return (durationInMillis * 1000) / UNIT_0_625_MS;
    }

    /**
     * Convert units of 0.625ms to milliseconds.
     *
     * @param[in] gapUnits The number of units of 0.625ms to convert.
     *
     * @return The value of @p gapUnits in milliseconds.
     */
    static uint16_t ADVERTISEMENT_DURATION_UNITS_TO_MS(uint16_t gapUnits)
    {
        return (gapUnits * UNIT_0_625_MS) / 1000;
    }

    /**
     * Get the advertising type.
     *
     * @return The advertising type.
     */
    ble::advertising_type_t getAdvertisingType(void) const
    {
        return _advType;
    }

    /**
     * Get the advertising interval in milliseconds.
     *
     * @return The advertisement interval (in milliseconds).
     */
    uint16_t getInterval(void) const
    {
        return ADVERTISEMENT_DURATION_UNITS_TO_MS(_interval);
    }

    /**
     * Get the advertisement interval in units of 0.625ms.
     *
     * @return The advertisement interval in advertisement duration units
     * (0.625ms units).
     */
    uint16_t getIntervalInADVUnits(void) const
    {
        return _interval;
    }

    /**
     * Get the advertising timeout.
     *
     * @return The advertising timeout (in seconds).
     */
    uint16_t getTimeout(void) const
    {
        return _timeout;
    }

    /**
     * Update the advertising type.
     *
     * @param[in] newAdvType The new advertising type.
     */
    void setAdvertisingType(ble::advertising_type_t newAdvType)
    {
        _advType = newAdvType;
    }

    /**
     * Update the advertising interval in milliseconds.
     *
     * @param[in] newInterval The new advertising interval in milliseconds.
     */
    void setInterval(uint16_t newInterval)
    {
        _interval = MSEC_TO_ADVERTISEMENT_DURATION_UNITS(newInterval);
        _maxInterval = _interval;
    }

    /**
     * Update the advertising timeout.
     *
     * @param[in] newTimeout The new advertising timeout (in seconds).
     *
     * @note 0 is a special value meaning the advertising process never ends.
     */
    void setTimeout(uint16_t newTimeout)
    {
        _timeout = newTimeout;
    }

    /**
     * Update the advertising type.
     *
     * @param[in] newAdvType The new advertising type.
     */
    void setType(
        ble::advertising_type_t newAdvType
    ) {
        _advType = newAdvType;
    }

    /**
     * Return advertising type.
     *
     * @return Advertising type.
     */
    ble::advertising_type_t getType() const {
        return _advType;
    }

    /** Check if advertising is anonymous.
     *
     * @return True if advertising is anonymous.
     */
    bool getAnonymousAdvertising() const {
        return _anonymous;
    }

    /** Advertise without your own address.
     *
     * @param enable Advertising anonymous if true.
     */
    void setAnonymousAdvertising(
        bool enable
    ) {
        _anonymous = enable;
    }

    /** Get the advertising intervals on the primary channels.
     *
     * @param min Minimum interval in milliseconds.
     * @param max Maximum interval in milliseconds.
     *
     * @return Error if pointers are invalid.
     */
    ble_error_t getPrimaryInterval(
        uint32_t *min /* ms */,
        uint32_t *max /* ms */
    ) const {
        if (!min || !max) {
            return BLE_ERROR_INVALID_PARAM;
        }
        *min = ADVERTISEMENT_DURATION_UNITS_TO_MS(_interval);
        *max = ADVERTISEMENT_DURATION_UNITS_TO_MS(_maxInterval);
        return BLE_ERROR_NONE;
    }

    /** Set the advertising intervals on the primary channels.
     *
     * @param min Minimum interval in milliseconds.
     * @param max Maximum interval in milliseconds.
     */
    void setPrimaryInterval(
        uint32_t min /* ms */,
        uint32_t max /* ms */
    ) {
        _interval = MSEC_TO_ADVERTISEMENT_DURATION_UNITS(min);
        _maxInterval = MSEC_TO_ADVERTISEMENT_DURATION_UNITS(max);
    }

    /** Get channels set for primary advertising.
     *
     * @param channel37 Use channel 37.
     * @param channel38 Use channel 38.
     * @param channel39 Use channel 39.
     *
     * @return Error if pointers are invalid.
     */
    ble_error_t getPrimaryChannels(
        bool *channel37,
        bool *channel38,
        bool *channel39
    ) const {
        if (!channel37 || !channel38 || !channel39) {
            return BLE_ERROR_INVALID_PARAM;
        }
        *channel37 = _channel37;
        *channel38 = _channel38;
        *channel39 = _channel39;
        return BLE_ERROR_NONE;
    }

    /** Set which channels are to be used for primary advertising.
     *  At least must be used. If all are set to disabled all channels will be used.
     *
     * @param channel37 Use channel 37.
     * @param channel38 Use channel 38.
     * @param channel39 Use channel 39.
     */
    void setPrimaryChannels(
        bool channel37,
        bool channel38,
        bool channel39
    ) {
        if (!channel37 && !channel38 && !channel39) {
            channel37 = channel38 = channel39 = true;
        }
        _channel37 = channel37;
        _channel38 = channel38;
        _channel39 = channel39;
    }

    /** Get what type of address is to be used as your own address during advertising.
     *
     * @return Addres tpe used.
     */
    own_address_type_t getOwnAddressType() const {
        return _ownAddressType;
    }

    /** Get what type of address is to be used as your own address during advertising.
     */
    void setOwnAddressType(
        own_address_type_t addressType
    ) {
        _ownAddressType = addressType;
    }

    /** Get peer address and type used during directed advertising.
     *
     * @param address Address that will have the peer address written to.
     * @param addressType Pointer to type which will have the address type written to.
     *
     * @return Error if pointers are invalid.
     */
    ble_error_t getPeer(
        BLEProtocol::AddressBytes_t *address,
        peer_address_type_t *addressType
    ) const {
        if (!address || !addressType) {
            return BLE_ERROR_INVALID_PARAM;
        }
        memcpy(address, _peerAddress, sizeof(BLEProtocol::AddressBytes_t));
        *addressType = _peerAddressType;
        return BLE_ERROR_NONE;
    };

    /** Set peer address and type used during directed advertising.
     *
     * @param address Peer's address bytes.
     * @param addressType Peer's address type.
     */
    void setPeer(
        const BLEProtocol::AddressBytes_t address,
        peer_address_type_t addressType
    ) {
        memcpy(_peerAddress, address, sizeof(BLEProtocol::AddressBytes_t));
        _peerAddressType = addressType;
    };

    /** Get the policy of whitelist use during advertising;
     *
     * @return Policy used.
     */
    ble::advertising_policy_mode_t getPolicyMode() const {
        return _policy;
    }
    /** Set the policy of whitelist use during advertising;
     *
     * @param Policy to use.
     */
    void setPolicyMode(
        ble::advertising_policy_mode_t mode
    ) {
        _policy = mode;
    }

    /** Get the advertising TX power.
     *
     * @return Advertising TX power.
     */
    int8_t getTxPower() const {
        return _txPower;
    }

    /** Set the advertising TX power.
     *
     * @param txPower Advertising TX power.
     */
    void setTxPower(
        int8_t txPower
    ) {
        _txPower = txPower;
    }

    /** Get PHYs used on primary and secondary advertising channels.
     *
     * @param primaryPhy,secondaryPhy  Pointer where the result is written to.
     *
     * @return Error if pointers are invalid.
     */
    ble_error_t getPhy(
        ble::phy_t *primaryPhy,
        ble::phy_t *secondaryPhy
    ) const {
        if (!primaryPhy || !secondaryPhy) {
            return BLE_ERROR_INVALID_PARAM;
        }
        *primaryPhy = _primaryPhy;
        *secondaryPhy = _secondaryPhy;
        return BLE_ERROR_NONE;
    }

    /** Get PHYs used on primary and secondary advertising channels.
     *
     * @param primaryPhy Primary advertising channels PHY.
     * @param secondaryPhy Secondary advertising channels PHY.
     */
    void setPhy(
        ble::phy_t primaryPhy,
        ble::phy_t secondaryPhy
    ) {
        _primaryPhy = primaryPhy;
        _secondaryPhy = secondaryPhy;
    }

    uint8_t getSecondaryMaxSkip() const {
        return _maxSkip;
    }

    void setSecondaryMaxSkip(
        uint8_t eventNumber
    ) {
        _maxSkip = eventNumber;
    }

    void setScanRequestNotification(
        bool enable = true
    ) {
        _notifyOnScan = enable;
    }

    bool getScanRequestNotification() const {
        return _notifyOnScan;
    }

    void setUseLegacyPDU(
        bool enable = true
    ) {
        _legacyPDU = enable;
    }

    bool getUseLegacyPDU() const {
        return _legacyPDU;
    }

    void includeTxPowerInHeader(
        bool enable = true
    ) {
        _includeHeaderTxPower = enable;
    }

    bool getTxPowerInHeader() const {
        return _includeHeaderTxPower;
    }

    /* helper get functions */

    /** Get the minimum advertisement interval in units of 0.625ms.
     *
     * @return The advertisement interval in advertisement duration units
     * (0.625ms units).
     */
    uint16_t getMinPrimaryIntervalInADVUnits() const {
        return _interval;
    }

    /** Get the maximum advertisement interval in units of 0.625ms.
     *
     * @return The advertisement interval in advertisement duration units
     * (0.625ms units).
     */
    uint16_t getMaxPrimaryIntervalInADVUnits() const {
        return _maxInterval;
    }

    /** Get the minimum advertisement interval in milliseconds.
     *
     * @return The advertisement interval in milliseconds.
     */
    uint32_t getMinPrimaryInterval() const {
        return ADVERTISEMENT_DURATION_UNITS_TO_MS(_interval);
    }

    /** Get the maximum advertisement interval in milliseconds.
     *
     * @return The advertisement interval in milliseconds.
     */
    uint32_t getMaxPrimaryInterval() const {
        return ADVERTISEMENT_DURATION_UNITS_TO_MS(_maxInterval);
    }

    const BLEProtocol::AddressBytes_t& getPeerAddress() const {
        return _peerAddress;
    };

    peer_address_type_t getPeerAddressType() const {
        return _peerAddressType;
    };

    ble::phy_t getPrimaryPhy() const {
        return _primaryPhy;
    }

    ble::phy_t getSecondaryPhy() const {
        return _secondaryPhy;
    }

    bool getChannel37() const {
        return _channel37;
    }

    bool getChannel38() const {
        return _channel37;
    }

    bool getChannel39() const {
        return _channel37;
    }

private:
    ble::advertising_type_t _advType;
    /* The advertising interval in ADV duration units (in other words, 0.625ms). */
    uint16_t _interval;
    /* The advertising timeout in ADV duration units (in other words, 0.625ms). */
    uint16_t _timeout;
    /* The advertising max interval in ADV duration units (in other words, 0.625ms) used in extended advertising. */
    uint16_t _maxInterval;
    peer_address_type_t _peerAddressType;
    own_address_type_t _ownAddressType;
    ble::advertising_policy_mode_t _policy;
    ble::phy_t _primaryPhy;
    ble::phy_t _secondaryPhy;
    BLEProtocol::AddressBytes_t _peerAddress;
    uint8_t _txPower;
    uint8_t _maxSkip;
    uint8_t _channel37:1;
    uint8_t _channel38:1;
    uint8_t _channel39:1;
    uint8_t _anonymous:1;
    uint8_t _notifyOnScan:1;
    uint8_t _legacyPDU:1;
    uint8_t _includeHeaderTxPower:1;
};

/**
 * @}
 * @}
 */

#endif /* ifndef MBED_GAP_ADVERTISING_PARAMS_H__ */
