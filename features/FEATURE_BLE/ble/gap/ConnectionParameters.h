/* mbed Microcontroller Library
 * Copyright (c) 2018 ARM Limited
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

#ifndef MBED_EXTENDED_CONNECT_PARAMETERS_H__
#define MBED_EXTENDED_CONNECT_PARAMETERS_H__

#include "ble/BLETypes.h"
#include "mbed_assert.h"

/**
 * @addtogroup ble
 * @{
 * @addtogroup gap
 * @{
 */

namespace ble {

class ConnectionParameters {
    static const uint8_t MAX_PARAM_PHYS = 3;
    static const uint8_t LE_1M_INDEX = 0;
    static const uint8_t LE_2M_INDEX = 1;
    static const uint8_t LE_CODED_INDEX = 2;

public:
    ConnectionParameters(
        scan_interval_t scanInterval = scan_interval_t::min(),
        scan_window_t scanWindow = scan_window_t::min(),
        conn_interval_t minConnectionInterval = conn_interval_t::min(),
        conn_interval_t maxConnectionInterval = conn_interval_t::max(),
        slave_latency_t slaveLatency = slave_latency_t::min(),
        supervision_timeout_t connectionSupervisionTimeout = supervision_timeout_t::max(),
        phy_t phy = phy_t::LE_1M,
        conn_event_length_t minEventLength = conn_event_length_t::min(),
        conn_event_length_t maxEventLength = conn_event_length_t::max()
    );

    /* setters */

    ConnectionParameters &setScanParameters(
        scan_interval_t scanInterval,
        scan_window_t scanWindow,
        phy_t phy = phy_t::LE_1M
    );

    ConnectionParameters &setConnectionParameters(
        conn_interval_t minConnectionInterval,
        conn_interval_t maxConnectionInterval,
        slave_latency_t slaveLatency,
        supervision_timeout_t connectionSupervisionTimeout,
        phy_t phy = phy_t::LE_1M,
        conn_event_length_t minEventLength = conn_event_length_t::min(),
        conn_event_length_t maxEventLength = conn_event_length_t::max()
    );

    ConnectionParameters &setOwnAddressType(own_address_type_t ownAddress)
    {
        _ownAddressType = ownAddress;

        return *this;
    }

    ConnectionParameters &setFilterPolicy(initiator_filter_policy_t filterPolicy)
    {
        _filterPolicy = filterPolicy;

        return *this;
    }

    ConnectionParameters &togglePhy(bool phy1M, bool phy2M, bool phyCoded)
    {
        handlePhyToggle(phy_t::LE_1M, phy1M);
        handlePhyToggle(phy_t::LE_2M, phy2M);
        handlePhyToggle(phy_t::LE_CODED, phyCoded);

        return *this;
    }

    ConnectionParameters &disablePhy(phy_t phy = phy_t::LE_1M)
    {
        handlePhyToggle(phy, false);

        return *this;
    }

    ConnectionParameters &enablePhy(phy_t phy = phy_t::LE_1M)
    {
        handlePhyToggle(phy, true);

        return *this;
    }

    /* getters */

    own_address_type_t getOwnAddressType() const
    {
        return _ownAddressType;
    }

    initiator_filter_policy_t getFilterPolicy() const
    {
        return _filterPolicy;
    }

    uint8_t getNumberOfEnabledPhys() const
    {
        return (
            _enabledPhy[LE_1M_INDEX] * 1 +
            _enabledPhy[LE_2M_INDEX] * 1 +
            _enabledPhy[LE_CODED_INDEX] * 1
        );
    }

    uint8_t getPhySet() const
    {
        phy_set_t set(
            _enabledPhy[LE_1M_INDEX],
            _enabledPhy[LE_2M_INDEX],
            _enabledPhy[LE_CODED_INDEX]
        );
        return set.value();
    }

    /* these return pointers to arrays of settings valid only across the number of active PHYs */

    const uint16_t *getScanIntervalArray() const
    {
        return &_scanInterval[getFirstEnabledIndex()];
    }

    const uint16_t *getScanWindowArray() const
    {
        return &_scanWindow[getFirstEnabledIndex()];
    }

    const uint16_t *getMinConnectionIntervalArray() const
    {
        return &_minConnectionInterval[getFirstEnabledIndex()];
    }

    const uint16_t *getMaxConnectionIntervalArray() const
    {
        return &_maxConnectionInterval[getFirstEnabledIndex()];
    }

    const uint16_t *getSlaveLatencyArray() const
    {
        return &_slaveLatency[getFirstEnabledIndex()];
    }

    const uint16_t *getConnectionSupervisionTimeoutArray() const
    {
        return &_connectionSupervisionTimeout[getFirstEnabledIndex()];
    }

    const uint16_t *getMinEventLengthArray() const
    {
        return &_minEventLength[getFirstEnabledIndex()];
    }

    const uint16_t *getMaxEventLengthArray() const
    {
        return &_maxEventLength[getFirstEnabledIndex()];
    }

private:
    uint8_t getFirstEnabledIndex() const
    {
        if (_enabledPhy[LE_1M_INDEX]) {
            return 0;
        } else if (_enabledPhy[LE_2M_INDEX]) {
            return 1;
        } else if (_enabledPhy[LE_CODED_INDEX]) {
            return 2;
        }
        /* this should never happen, it means you were trying to start a connection with a blank set
         * of paramters - you need to enabled at least one phy */
        MBED_ASSERT("Trying to use connection parameters without any PHY defined.");
        return 0;
    }

    /** Handle toggling PHYs on and off and return the correct index to use to set the configuration elements.
     *
     * @param phy Which Phy is being toggle.
     * @param enable On or Off.
     * @return The index to the array of settings.
     */
    uint8_t handlePhyToggle(phy_t phy, bool enable)
    {
        uint8_t index = phyToIndex(phy);

        bool was_swapped = false;
        bool is_swapped = false;

        if (isSwapped()) {
            was_swapped = true;
        }

        _enabledPhy[index] = enable;

        if (isSwapped()) {
            is_swapped = true;
        }

        if (was_swapped != is_swapped) {
            swapCodedAnd2M();
        }

        if (is_swapped && phy == phy_t::LE_CODED) {
            index -= 1;
        }

        return index;
    }

    uint8_t phyToIndex(phy_t phy) const
    {
        uint8_t index;
        switch(phy.value()) {
            case phy_t::LE_1M:
                index = 0;
                break;
            case phy_t::LE_2M:
                index = 1;
                break;
            case phy_t::LE_CODED:
                index = 2;
                break;
            default:
                index = MAX_PARAM_PHYS;
                MBED_ASSERT("Illegal PHY");
                break;
        }
        return index;
    }

    bool isSwapped() const
    {
        return (
            _enabledPhy[LE_1M_INDEX] &&
            !_enabledPhy[LE_2M_INDEX] &&
            _enabledPhy[LE_CODED_INDEX]
        );
    }

    /** Handle the swapping of 2M and CODED so that the array is ready for the pal call. */
    void swapCodedAnd2M();

private:
    initiator_filter_policy_t _filterPolicy;
    own_address_type_t _ownAddressType;

    uint16_t _scanInterval[MAX_PARAM_PHYS]; /* 0.625 ms */
    uint16_t _scanWindow[MAX_PARAM_PHYS]; /* 0.625 ms */
    uint16_t _minConnectionInterval[MAX_PARAM_PHYS]; /* 1.25 ms */
    uint16_t _maxConnectionInterval[MAX_PARAM_PHYS]; /* 1.25 ms */
    uint16_t _slaveLatency[MAX_PARAM_PHYS]; /* events */
    uint16_t _connectionSupervisionTimeout[MAX_PARAM_PHYS]; /* 10 ms */
    uint16_t _minEventLength[MAX_PARAM_PHYS]; /* 0.625 ms */
    uint16_t _maxEventLength[MAX_PARAM_PHYS]; /* 0.625 ms */

    bool _enabledPhy[MAX_PARAM_PHYS];
};

} // namespace ble

/**
 * @}
 * @}
 */

#endif /* ifndef MBED_EXTENDED_CONNECT_PARAMETERS_H__ */
