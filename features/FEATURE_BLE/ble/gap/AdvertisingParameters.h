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

namespace ble {

/**
 * @addtogroup ble
 * @{
 * @addtogroup gap
 * @{
 */

/**
 * Parameters defining the advertising process.
 *
 * @par Legacy advertising:
 *
 * Advertising parameters for legacy advertising are a mainly defined by a pair
 * of values:
 *   - The Advertising mode modeled after advertising_type_t. It defines
 *     whether the device is connectable and scannable. You can set this value at
 *     construction time, update it with setType() and query it with getType().
 *   - Time interval between advertisement. You can set it at construction time,
 *     update it with setPrimaryInterval() and obtain it from getMinPrimaryInterval()
 *     and getMaxPrimaryInterval().
 *
 * In addition, it is possible to adjust other parameters:
 *   - You can select the advertising channels with setPrimaryChannels() and
 *   queried them with getChannel37(), getChannel38() and getChannel39().
 *   - You can set the address type used by the local device with setOwnAddressType()
 *   and query it by getOwnAddressType().
 *   - You can set the filter policy for scan and connection requests with
 *   setFilter() and query it with getFilter().
 *
 * For directed advertising, you can set the address of the target with the help
 * of setPeer() and query it with getPeerAddress() and getPeerAddressType().
 *
 * @par Extended advertising:
 *
 * To use extended advertising features, first disable legacy advertising
 * with setUseLegacyPDU().
 *
 * Extended advertising adds new features to BLE advertising:
 *   - Control the advertising power with setTxPower().
 *   - Include the Tx power in advertising packet with includeTxPowerInHeader().
 *   - Set a secondary phy_t channel with setPhy().
 *   - Enable scan requests notification to let the application be aware of any
 *   incoming scan requests with setScanRequestNotification().
 *   - Advertise anonymously with setAnonymousAdvertising()
 *
 * @par Fluent interface:
 *
 * This API is designed for usability. You can construct
 * it and pass it in place. To achieve this, the fluent interface pattern
 * is used. Every setter returns a reference to the object modified and can be
 * chained.
 *
 * @code
    void setAdvertisingParameters(ble::Gap& gap) {
        using namespace ble;
        gap.setAdvertisingParameters(
            LEGACY_ADVERTISING_HANDLE,
            AdvertisingParameters()
                .setType(advertising_type_t::ADV_CONNECTABLE_UNDIRECTED)
                .setPrimaryInterval(millisecond_t(200), millisecond_t(500))
                .setOwnAddressType(own_address_type_t::RANDOM)
                .setUseLegacyPDU(false)
                .setPhy(phy_t::LE_1M, phy_t::LE_CODED)
        );
    }
 * @endcode
 *
 * @see ble::Gap::createAdvertisingSet(), ble::Gap::setAdvertisingParameters()
 */
class AdvertisingParameters {

    /**
     * Default minimum advertising interval.
     */
    static const uint32_t DEFAULT_ADVERTISING_INTERVAL_MIN = 0x400;

    /**
     * Default maximum advertising interval.
     */
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
     * @param[in] minInterval, maxInterval Time interval between two advertisement.
     * A range is provided to the LE subsystem, so it can adjust the advertising
     * interval with other transmission happening on the BLE radio.
     * @param[in] useLegacyPDU If true legacy PDU shall be used for advertising.
     *
     * @note If CONNECTABLE_UNDIRECTED or CONNECTABLE_DIRECTED advertising is used
     * you must use legacy PDU.
     *
     * @note If values in input are out of range, they will be normalized.
     *
     * @note If type selected is incompatible with non legacy PDU, legacy PDU will be used.
     */
    AdvertisingParameters(
        advertising_type_t advType = advertising_type_t::CONNECTABLE_UNDIRECTED,
        adv_interval_t minInterval = adv_interval_t(DEFAULT_ADVERTISING_INTERVAL_MIN),
        adv_interval_t maxInterval = adv_interval_t(DEFAULT_ADVERTISING_INTERVAL_MAX),
        bool useLegacyPDU = true
    ) :
        _advType(advType),
        _minInterval(minInterval),
        _maxInterval(maxInterval),
        _peerAddressType(target_peer_address_type_t::PUBLIC),
        _ownAddressType(own_address_type_t::RANDOM),
        _policy(advertising_filter_policy_t::NO_FILTER),
        _primaryPhy(phy_t::LE_1M),
        _secondaryPhy(phy_t::LE_1M),
        _peerAddress(),
        _txPower(127),
        _maxSkip(0),
        _channel37(true),
        _channel38(true),
        _channel39(true),
        _anonymous(false),
        _notifyOnScan(false),
        _legacyPDU(useLegacyPDU),
        _includeHeaderTxPower(false)
    {
        normalize();
    }

    /**
     * Construct an instance of GapAdvertisingParams.
     *
     * @param[in] advType Type of advertising.
     * @param[in] useLegacyPDU If true legacy PDU shall be used for advertising.
     *
     * @note If CONNECTABLE_UNDIRECTED or CONNECTABLE_DIRECTED advertising is used
     * you must use legacy PDU.
     *
     * @note If type selected is incompatible with non legacy PDU, legacy PDU will be used.
     */
    AdvertisingParameters(
        advertising_type_t advType,
        bool useLegacyPDU
    ) :
        _advType(advType),
        _minInterval(adv_interval_t(DEFAULT_ADVERTISING_INTERVAL_MIN)),
        _maxInterval(adv_interval_t(DEFAULT_ADVERTISING_INTERVAL_MAX)),
        _peerAddressType(target_peer_address_type_t::PUBLIC),
        _ownAddressType(own_address_type_t::RANDOM),
        _policy(advertising_filter_policy_t::NO_FILTER),
        _primaryPhy(phy_t::LE_1M),
        _secondaryPhy(phy_t::LE_1M),
        _peerAddress(),
        _txPower(127),
        _maxSkip(0),
        _channel37(true),
        _channel38(true),
        _channel39(true),
        _anonymous(false),
        _notifyOnScan(false),
        _legacyPDU(useLegacyPDU),
        _includeHeaderTxPower(false)
    {
        normalize();
    }

public:
    /**
     * Update the advertising type and whether to use legacy PDU.
     *
     * @note If legacy PDU is not used then you cannot use
     * CONNECTABLE_UNDIRECTED nor CONNECTABLE_DIRECTED.
     *
     * @param[in] newAdvType The new advertising type.
     *
     * @param[in] legacy If true, legacy PDU will be used.
     *
     * @return reference to this object.
     */
    AdvertisingParameters &setType(advertising_type_t newAdvType, bool legacy)
    {
        if (newAdvType == advertising_type_t::CONNECTABLE_UNDIRECTED ||
            newAdvType == advertising_type_t::CONNECTABLE_DIRECTED) {
            /* these types can only be used with legacy PDUs */
            MBED_ASSERT(legacy);
        }
        _advType = newAdvType;
        _legacyPDU = legacy;
        return *this;
    }

    /**
     * Update the advertising type.
     *
     * @note If legacy PDU is not used then you cannot use
     * CONNECTABLE_UNDIRECTED nor CONNECTABLE_DIRECTED.
     *
     * @param[in] newAdvType The new advertising type.
     *
     * @return reference to this object.
     */
    AdvertisingParameters &setType(advertising_type_t newAdvType)
    {
        if (newAdvType == advertising_type_t::CONNECTABLE_UNDIRECTED ||
            newAdvType == advertising_type_t::CONNECTABLE_DIRECTED) {
            /* these types can only be used with legacy PDUs */
            MBED_ASSERT(_legacyPDU);
        }
        _advType = newAdvType;
        return *this;
    }

    /**
     * Return the advertising type.
     *
     * @return Advertising type.
     */
    advertising_type_t getType() const
    {
        return _advType;
    }

    /** Set the advertising intervals on the primary channels.
     *
     * @param[in] min, max Time interval between two advertisements.
     * A range is provided to the LE subsystem, so it can adjust the advertising
     * interval with other transmission happening on the BLE radio.
     *
     * @return reference to this object.
     */
    AdvertisingParameters &setPrimaryInterval(
        adv_interval_t min, adv_interval_t max
    )
    {
        _minInterval = min;
        _maxInterval = max;
        return *this;
    }

    /** Get the minimum advertising intervals on the primary channels.
     *
     * @return The lower bound of the primary interval selected.
     */
    adv_interval_t getMinPrimaryInterval() const
    {
        return _minInterval;
    }

    /** Get the maximum advertising intervals on the primary channels.
     *
     * @return The higher bound of the primary interval selected.
     */
    adv_interval_t getMaxPrimaryInterval() const
    {
        return _maxInterval;
    }

    /** Set which channels are to be used for primary advertising.
     *  At least must be used. If all are set to disabled, all channels will be used.
     *
     * @param channel37 Use channel 37.
     * @param channel38 Use channel 38.
     * @param channel39 Use channel 39.
     *
     * @return a reference to this object.
     */
    AdvertisingParameters &setPrimaryChannels(
        bool channel37, bool channel38, bool channel39
    )
    {
        if (!channel37 && !channel38 && !channel39) {
            channel37 = channel38 = channel39 = true;
        }
        _channel37 = channel37;
        _channel38 = channel38;
        _channel39 = channel39;
        return *this;
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

    /** Get what type of address is to be used as your own address during advertising.
     *
     * @return a reference to this object.
     */
    AdvertisingParameters &setOwnAddressType(own_address_type_t addressType)
    {
        _ownAddressType = addressType;
        return *this;
    }

    /** Get what type of address is to be used as your own address during advertising.
     *
     * @return Addres tpe used.
     */
    own_address_type_t getOwnAddressType() const
    {
        return _ownAddressType;
    }

    /** Set peer address and type used during directed advertising.
     *
     * @param address Peer's address bytes.
     * @param addressType Peer's address type.
     *
     * @return a reference to this object.
     */
    AdvertisingParameters &setPeer(
        const address_t &address,
        target_peer_address_type_t addressType
    )
    {
        _peerAddress = address;
        _peerAddressType = addressType;
        return *this;
    };

    /** Get the peer address used during directed advertising.
     *
     * @return Address of the peer targeted by directed advertising.
     */
    const address_t &getPeerAddress() const
    {
        return _peerAddress;
    };


    /** Get the peer address type used during directed advertising.
     *
     * @return The type of address of the peer targeted by directed advertising.
     */
    target_peer_address_type_t getPeerAddressType() const
    {
        return _peerAddressType;
    };

    /** Set the filter policy of whitelist use during advertising;
     *
     * @param mode Policy to use.
     *
     * @return A reference to this object.
     */
    AdvertisingParameters &setFilter(advertising_filter_policy_t mode)
    {
        _policy = mode;
        return *this;
    }

    /** Get the filter policy of whitelist use during advertising;
     *
     * @return Policy used.
     */
    advertising_filter_policy_t getFilter() const
    {
#if BLE_FEATURE_WHITELIST
        return _policy;
#else
        return advertising_filter_policy_t::NO_FILTER;
#endif // BLE_FEATURE_WHITELIST
    }

    /* Extended advertising parameters */

    /** Get PHYs used on primary and secondary advertising channels.
     *
     * @param primaryPhy Primary advertising channels PHY.
     * @param secondaryPhy Secondary advertising channels PHY.
     *
     * @return A reference to this.
     */
    AdvertisingParameters &setPhy(phy_t primaryPhy, phy_t secondaryPhy)
    {
        _primaryPhy = primaryPhy;
        _secondaryPhy = secondaryPhy;
        return *this;
    }

    /** Get PHY used for primary advertising.
     *
     * @return PHY used for primary advertising.
     */
    phy_t getPrimaryPhy() const
    {
        return _primaryPhy;
    }

    /** Get PHY used for secondary advertising.
     *
     * @return PHY used for secondary advertising.
     */
    phy_t getSecondaryPhy() const
    {
        return _secondaryPhy;
    }

    /** Set the advertising TX power.
     *
     * @param txPower Advertising TX power.
     *
     * @return A reference to this object.
     */
    AdvertisingParameters &setTxPower(advertising_power_t txPower)
    {
        _txPower = txPower;
        return *this;
    }

    /** Get the advertising TX power.
     *
     * @return Advertising TX power.
     */
    advertising_power_t getTxPower() const
    {
        return _txPower;
    }

    /** Set how many events can be skipped on the secondary channel.
     *
     * @param eventNumber Number of events that can be skipped.
     *
     * @return A reference to this object.
     */
    AdvertisingParameters &setSecondaryMaxSkip(uint8_t eventNumber)
    {
        _maxSkip = eventNumber;
        return *this;
    }

    /** Return how many events can be skipped on the secondary channel.
     *
     * @return How many events can be skipped on the secondary channel.
     */
    uint8_t getSecondaryMaxSkip() const
    {
        return _maxSkip;
    }

    /** Enabled or disable the callback that notifies the user about a scan request.
     *
     * @param enable Enable callback if true.
     *
     * @return A reference to this object.
     *
     * @see ::ble::Gap::EventHandler::onScanRequestReceived()
     */
    AdvertisingParameters &setScanRequestNotification(bool enable = true)
    {
        _notifyOnScan = enable;
        return *this;
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
     * @param enable If true, legacy PDU will be used.
     *
     * @note If CONNECTABLE_UNDIRECTED or CONNECTABLE_DIRECTED advertising is used
     * you must use legacy PDU.
     *
     * @return A reference to this object.
     */
    AdvertisingParameters &setUseLegacyPDU(bool enable = true)
    {
        if (!enable) {
            /* these types can only be used with legacy PDUs */
            MBED_ASSERT((_advType != advertising_type_t::CONNECTABLE_UNDIRECTED) &&
                        (_advType != advertising_type_t::CONNECTABLE_DIRECTED));
        }

        _legacyPDU = enable;
        return *this;
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
     * @param enable If true, include the TX power in the header.
     *
     * @return A reference to this object.
     */
    AdvertisingParameters &includeTxPowerInHeader(bool enable = true)
    {
        _includeHeaderTxPower = enable;
        return *this;
    }

    /** Check if TX power should be included in the header.
     *
     * @return True if TX power is included in the header.
     */
    bool getTxPowerInHeader() const
    {
        return _includeHeaderTxPower;
    }

    /** Advertise without your own address.
     *
     * @param enable Advertising anonymous if true.
     *
     * @note You may not use anonymous advertising with periodic advertising on the same set.
     *
     * @return reference to this object.
     */
    AdvertisingParameters &setAnonymousAdvertising(bool enable)
    {
        _anonymous = enable;
        return *this;
    }

    /** Check if advertising is anonymous.
     *
     * @return True if advertising is anonymous.
     */
    bool getAnonymousAdvertising() const
    {
        return _anonymous;
    }

private:
    /**
     * Enforce limits on parameters.
     */
    void normalize()
    {
        /* Min interval is slightly larger than in other modes. */
        if (_advType == advertising_type_t::NON_CONNECTABLE_UNDIRECTED) {
            _minInterval = adv_interval_t(std::max(_minInterval.value(), GAP_ADV_PARAMS_INTERVAL_MIN_NONCON));
            _maxInterval = adv_interval_t(std::max(_maxInterval.value(), GAP_ADV_PARAMS_INTERVAL_MIN_NONCON));
        }
        if (_advType == advertising_type_t::CONNECTABLE_DIRECTED ||
            _advType == advertising_type_t::CONNECTABLE_UNDIRECTED) {
            _legacyPDU = true;
        }
    }

private:
    advertising_type_t _advType;
    /* The advertising interval in ADV duration units (in other words, 0.625ms). */
    adv_interval_t _minInterval;
    /* The advertising max interval in ADV duration units (in other words, 0.625ms) used in extended advertising. */
    adv_interval_t _maxInterval;

    target_peer_address_type_t _peerAddressType;
    own_address_type_t _ownAddressType;
    advertising_filter_policy_t _policy;
    phy_t _primaryPhy;
    phy_t _secondaryPhy;
    address_t _peerAddress;
    advertising_power_t _txPower;
    uint8_t _maxSkip;
    bool _channel37:1;
    bool _channel38:1;
    bool _channel39:1;
    bool _anonymous:1;
    bool _notifyOnScan:1;
    bool _legacyPDU:1;
    bool _includeHeaderTxPower:1;
};

/**
 * @}
 * @}
 */

} // namespace ble

#endif /* ifndef MBED_ADVERTISING_PARAMETERS_H__ */
