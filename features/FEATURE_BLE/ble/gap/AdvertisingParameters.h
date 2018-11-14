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

#ifndef MBED_ADVERTISING_PARAMETERS_H__
#define MBED_ADVERTISING_PARAMETERS_H__

#include <algorithm>

#include "BLETypes.h"
#include "BLEProtocol.h"
#include "blecommon.h"
#include "SafeEnum.h"

/**
 * @addtogroup ble
 * @{
 * @addtogroup gap
 * @{
 */

namespace ble {

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
class AdvertisingParameters {

    static const uint32_t DEFAULT_ADVERTISING_INTERVAL_MIN = 0x400;

    static const uint32_t DEFAULT_ADVERTISING_INTERVAL_MAX = 0x800;

    /**
     * Minimum Advertising interval for scannable and nonconnectable
     * undirected events in 625us units.
     *
     * @note Equal to 100ms.
     */
    static const uint32_t GAP_ADV_PARAMS_INTERVAL_MIN_NONCON = 0x00A0;

public:
    /**
     * Construct an instance of GapAdvertisingParams.
     *
     * @param[in] advType Type of advertising.
     * @param[in] minInterval Time interval between two advertisement in units of
     * 0.625ms.
     * @param[in] timeout Duration in seconds of the advertising process. A
     * value of 0 indicate that there is no timeout of the advertising process.
     *
     * @note If value in input are out of range, they will be normalized.
     */
    AdvertisingParameters(
        ble::advertising_type_t advType = ble::ADV_CONNECTABLE_UNDIRECTED,
        adv_interval_t minInterval = adv_interval_t(DEFAULT_ADVERTISING_INTERVAL_MIN),
        adv_interval_t maxInterval = adv_interval_t(DEFAULT_ADVERTISING_INTERVAL_MAX)
    ) :
        _advType(advType),
        _minInterval(minInterval),
        _maxInterval(maxInterval),
        _peerAddressType(ble::target_peer_address_type_t::PUBLIC),
        _ownAddressType(ble::own_address_type_t::PUBLIC),
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
        /* Min interval is slightly larger than in other modes. */
        if (_advType == ble::ADV_NON_CONNECTABLE_UNDIRECTED) {
            _minInterval = adv_interval_t(std::max(_minInterval.value(), GAP_ADV_PARAMS_INTERVAL_MIN_NONCON));
            _maxInterval = adv_interval_t(std::max(_maxInterval.value(), GAP_ADV_PARAMS_INTERVAL_MIN_NONCON));
        }
    }

public:
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
     * Update the advertising type.
     *
     * @param[in] newAdvType The new advertising type.
     */
    void setAdvertisingType(ble::advertising_type_t newAdvType)
    {
        _advType = newAdvType;
    }

    /**
     * Update the advertising interval.
     *
     * @param[in] newMinInterval The new advertising interval .
     */
    void setInterval(adv_interval_t newMinInterval, adv_interval_t newMaxInterval)
    {
        _minInterval = newMinInterval;
        _maxInterval = newMaxInterval;
    }

    /**
     * Update the advertising type.
     *
     * @param[in] newAdvType The new advertising type.
     */
    void setType(ble::advertising_type_t newAdvType)
    {
        _advType = newAdvType;
    }

    /**
     * Return advertising type.
     *
     * @return Advertising type.
     */
    ble::advertising_type_t getType() const
    {
        return _advType;
    }

    /** Check if advertising is anonymous.
     *
     * @return True if advertising is anonymous.
     */
    bool getAnonymousAdvertising() const
    {
        return _anonymous;
    }

    /** Advertise without your own address.
     *
     * @param enable Advertising anonymous if true.
     */
    void setAnonymousAdvertising(bool enable)
    {
        _anonymous = enable;
    }

    /** Get the advertising intervals on the primary channels.
     *
     * @param min Minimum interval.
     * @param max Maximum interval.
     */
    ble_error_t getPrimaryInterval(
        adv_interval_t *min /* ms */,
        adv_interval_t *max /* ms */
    ) const {
        if (!min || !max) {
            return BLE_ERROR_INVALID_PARAM;
        }

        *min = _minInterval;
        *max = _maxInterval;
        return BLE_ERROR_NONE;
    }

    /** Set the advertising intervals on the primary channels.
     *
     * @param min Minimum interval .
     * @param max Maximum interval .
     */
    void setPrimaryInterval(
        adv_interval_t min ,
        adv_interval_t max
    ) {
        _minInterval = min;
        _maxInterval = max;
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
    void setPrimaryChannels(bool channel37, bool channel38, bool channel39)
    {
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
    ble::own_address_type_t getOwnAddressType() const
    {
        return _ownAddressType;
    }

    /** Get what type of address is to be used as your own address during advertising.
     */
    void setOwnAddressType(ble::own_address_type_t addressType)
    {
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
        ble::address_t *address,
        ble::target_peer_address_type_t *addressType
    ) const {
        if (!address || !addressType) {
            return BLE_ERROR_INVALID_PARAM;
        }
        *address = _peerAddress;
        *addressType = _peerAddressType;
        return BLE_ERROR_NONE;
    };

    /** Set peer address and type used during directed advertising.
     *
     * @param address Peer's address bytes.
     * @param addressType Peer's address type.
     */
    void setPeer(
        const address_t &address,
        ble::target_peer_address_type_t addressType
    ) {
        _peerAddress = address;
        _peerAddressType = addressType;
    };

    /** Get the policy of whitelist use during advertising;
     *
     * @return Policy used.
     */
    ble::advertising_policy_mode_t getPolicyMode() const
    {
        return _policy;
    }
    /** Set the policy of whitelist use during advertising;
     *
     * @param Policy to use.
     */
    void setPolicyMode(ble::advertising_policy_mode_t mode)
    {
        _policy = mode;
    }

    /** Get the advertising TX power.
     *
     * @return Advertising TX power.
     */
    advertising_power_t getTxPower() const
    {
        return _txPower;
    }

    /** Set the advertising TX power.
     *
     * @param txPower Advertising TX power.
     */
    void setTxPower(advertising_power_t txPower)
    {
        _txPower = txPower;
    }

    /** Get PHYs used on primary and secondary advertising channels.
     *
     * @param primaryPhy,secondaryPhy  Pointer where the result is written to.
     *
     * @return Error if pointers are invalid.
     */
    ble_error_t getPhy(ble::phy_t *primaryPhy, ble::phy_t *secondaryPhy) const
    {
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
    void setPhy(ble::phy_t primaryPhy, ble::phy_t secondaryPhy)
    {
        _primaryPhy = primaryPhy;
        _secondaryPhy = secondaryPhy;
    }

    /** Return how many events can be skipped on the secondary channel.
     *
     * @return How many events can be skipped on the secondary channel.
     */
    uint8_t getSecondaryMaxSkip() const
    {
        return _maxSkip;
    }

    /** Set how many events can be skipped on the secondary channel.
     *
     * @param eventNumber Number of events that can be skipped.
     */
    void setSecondaryMaxSkip(uint8_t eventNumber)
    {
        _maxSkip = eventNumber;
    }

    /** Enabled or disable the callback that notifies the user about a scan request.
     *
     * @param enable Enable callback if true.
     */
    void setScanRequestNotification(bool enable = true)
    {
        _notifyOnScan = enable;
    }

    /** Return of the callback for scan request is enabled.
     *
     * @return True if callback is enabled.
     */
    bool getScanRequestNotification() const
    {
        return _notifyOnScan;
    }

    /** Use legacy PDU during advertising.
     *
     * @param enable If true legacy PDU will be used.
     */
    void setUseLegacyPDU(bool enable = true)
    {
        _legacyPDU = enable;
    }

    /** Check if legacy PDU is used during advertising.
     *
     * @return True legacy PDU will be used.
     */
    bool getUseLegacyPDU() const
    {
        return _legacyPDU;
    }

    /** Set if TX power should be included in the header.
     *
     * @param enable If true include the TX power in the header.
     */
    void includeTxPowerInHeader(bool enable = true)
    {
        _includeHeaderTxPower = enable;
    }

    /** Check if TX power should be included in the header.
     *
     * @return True if TX power is included in the header.
     */
    bool getTxPowerInHeader() const
    {
        return _includeHeaderTxPower;
    }

    /** Get the minimum advertisement interval.
     *
     * @return The advertisement interval.
     */
    adv_interval_t getMinPrimaryInterval() const
    {
        return _minInterval;
    }

    /** Get the maximum advertisement interval.
     *
     * @return The advertisement interval.
     */
    adv_interval_t getMaxPrimaryInterval() const
    {
        return _maxInterval;
    }

    /** Peer address for directed advertising.
     *
     * @return Peer address.
     */
    const address_t& getPeerAddress() const
    {
        return _peerAddress;
    };

    /** Peer address type for directed advertising.
     *
     * @return Peer address type.
     */
    target_peer_address_type_t getPeerAddressType() const
    {
        return _peerAddressType;
    };

    /** Get PHY used for primary advertising.
     *
     * @return PHY used for primary advertising.
     */
    ble::phy_t getPrimaryPhy() const
    {
        return _primaryPhy;
    }

    /** Get PHY used for secondary advertising.
     *
     * @return PHY used for secondary advertising.
     */
    ble::phy_t getSecondaryPhy() const
    {
        return _secondaryPhy;
    }

    /** Check if channel 37 is used for primary advertising.
     *
     * @return True if channel used.
     */
    bool getChannel37() const
    {
        return _channel37;
    }


    /** Check if channel 38 is used for primary advertising.
     *
     * @return True if channel used.
     */
    bool getChannel38() const
    {
        return _channel38;
    }


    /** Check if channel 39 is used for primary advertising.
     *
     * @return True if channel used.
     */
    bool getChannel39() const
    {
        return _channel39;
    }

private:
    advertising_type_t _advType;
    /* The advertising interval in ADV duration units (in other words, 0.625ms). */
    adv_interval_t _minInterval;
    /* The advertising max interval in ADV duration units (in other words, 0.625ms) used in extended advertising. */
    adv_interval_t _maxInterval;

    target_peer_address_type_t _peerAddressType;
    own_address_type_t _ownAddressType;
    advertising_policy_mode_t _policy;
    phy_t _primaryPhy;
    phy_t _secondaryPhy;
    address_t _peerAddress;
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

} // namespace ble

/**
 * @}
 * @}
 */

#endif /* ifndef MBED_ADVERTISING_PARAMETERS_H__ */
