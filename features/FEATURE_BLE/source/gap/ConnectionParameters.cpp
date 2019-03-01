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

#include "gap/ConnectionParameters.h"

namespace ble {

ConnectionParameters::ConnectionParameters(
    phy_t phy,
    scan_interval_t scanInterval,
    scan_window_t scanWindow,
    conn_interval_t minConnectionInterval,
    conn_interval_t maxConnectionInterval,
    slave_latency_t slaveLatency,
    supervision_timeout_t connectionSupervisionTimeout,
    conn_event_length_t minEventLength,
    conn_event_length_t maxEventLength
) :
    _filterPolicy(initiator_filter_policy_t::NO_FILTER),
    _ownAddressType(own_address_type_t::RANDOM)
{
    for (uint8_t i = 0; i < MAX_PARAM_PHYS; ++i) {
        _enabledPhy[i] = false;
    }
    if (phy != phy_t::NONE) {
        uint8_t phy_index = phyToIndex(phy);

        if (phy_index < MAX_PARAM_PHYS) {
            _scanInterval[phy_index] = scanInterval.value();
            _scanWindow[phy_index] = scanWindow.value();
            _minConnectionInterval[phy_index] = minConnectionInterval.value();
            _maxConnectionInterval[phy_index] = maxConnectionInterval.value();
            _slaveLatency[phy_index] = slaveLatency.value();
            _connectionSupervisionTimeout[phy_index] = connectionSupervisionTimeout.value();
            _enabledPhy[phy_index] = true;
            _minEventLength[phy_index] = minEventLength.value();
            _maxEventLength[phy_index] = maxEventLength.value();
        }
    }
}

/* setters */

ConnectionParameters &ConnectionParameters::setScanParameters(
    phy_t phy,
    scan_interval_t scanInterval,
    scan_window_t scanWindow
)
{
    uint8_t phy_index = handlePhyToggle(phy, true);

    if (phy_index < MAX_PARAM_PHYS) {
        _scanInterval[phy_index] = scanInterval.value();
        _scanWindow[phy_index] = scanWindow.value();
    }

    return *this;
}

ConnectionParameters &ConnectionParameters::setConnectionParameters(
    phy_t phy,
    conn_interval_t minConnectionInterval,
    conn_interval_t maxConnectionInterval,
    slave_latency_t slaveLatency,
    supervision_timeout_t connectionSupervisionTimeout,
    conn_event_length_t minEventLength,
    conn_event_length_t maxEventLength
)
{
    uint8_t phy_index = handlePhyToggle(phy, true);

    if (phy_index < MAX_PARAM_PHYS) {
        _minConnectionInterval[phy_index] = minConnectionInterval.value();
        _maxConnectionInterval[phy_index] = maxConnectionInterval.value();
        _slaveLatency[phy_index] = slaveLatency.value();
        _connectionSupervisionTimeout[phy_index] = connectionSupervisionTimeout.value();

        /* avoid overflows and truncation */
        if (minEventLength.value() > maxEventLength.value()) {
            minEventLength = maxEventLength;
        }

        _minEventLength[phy_index] = minEventLength.value();
        _maxEventLength[phy_index] = maxEventLength.value();
    }

    return *this;
}

#if BLE_FEATURE_PHY_MANAGEMENT
/** Handle the swapping of 2M and CODED so that the array is ready for the pal call. */
void ConnectionParameters::swapCodedAnd2M()
{
    uint16_t scanInterval = _scanInterval[LE_2M_INDEX];
    uint16_t scanWindow = _scanWindow[LE_2M_INDEX];
    uint16_t minConnectionInterval = _minConnectionInterval[LE_2M_INDEX];
    uint16_t maxConnectionInterval = _maxConnectionInterval[LE_2M_INDEX];
    uint16_t slaveLatency = _slaveLatency[LE_2M_INDEX];
    uint16_t connectionSupervisionTimeout = _connectionSupervisionTimeout[LE_2M_INDEX];
    uint16_t minEventLength = _minEventLength[LE_2M_INDEX];
    uint16_t maxEventLength = _maxEventLength[LE_2M_INDEX];

    _scanInterval[LE_2M_INDEX] = _scanInterval[LE_CODED_INDEX];
    _scanWindow[LE_2M_INDEX] = _scanWindow[LE_CODED_INDEX];
    _minConnectionInterval[LE_2M_INDEX] = _minConnectionInterval[LE_CODED_INDEX];
    _maxConnectionInterval[LE_2M_INDEX] = _maxConnectionInterval[LE_CODED_INDEX];
    _slaveLatency[LE_2M_INDEX] = _slaveLatency[LE_CODED_INDEX];
    _connectionSupervisionTimeout[LE_2M_INDEX] = _connectionSupervisionTimeout[LE_CODED_INDEX];
    _minEventLength[LE_2M_INDEX] = _minEventLength[LE_CODED_INDEX];
    _maxEventLength[LE_2M_INDEX] = _maxEventLength[LE_CODED_INDEX];

    _scanInterval[LE_CODED_INDEX] = scanInterval;
    _scanWindow[LE_CODED_INDEX] = scanWindow;
    _minConnectionInterval[LE_CODED_INDEX] = minConnectionInterval;
    _maxConnectionInterval[LE_CODED_INDEX] = maxConnectionInterval;
    _slaveLatency[LE_CODED_INDEX] = slaveLatency;
    _connectionSupervisionTimeout[LE_CODED_INDEX] = connectionSupervisionTimeout;
    _minEventLength[LE_CODED_INDEX] = minEventLength;
    _maxEventLength[LE_CODED_INDEX] = maxEventLength;
}
#endif // BLE_FEATURE_PHY_MANAGEMENT

} // namespace ble
