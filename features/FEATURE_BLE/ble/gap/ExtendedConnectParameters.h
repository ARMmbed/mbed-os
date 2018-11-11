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

/**
 * @addtogroup ble
 * @{
 * @addtogroup gap
 * @{
 */

class ExtendedConnectParameters_t {
    const size_t MAX_PARAM_PHYS = 3;
public:
    ExtendedConnectParameters_t() :
        _filterPolicy(ble::scanning_policy_mode_t::SCAN_POLICY_FILTER_ALL_ADV),
        _ownAddressType(ble::own_address_type_t::PUBLIC)
    {
        for (int i = 0; i < MAX_PARAM_PHYS; ++i) {
            _scanInterval[i] = 4;
            _scanWindow[i] = 4;
            _minConnectionInterval[i] = 6;
            _maxConnectionInterval[i] = 6;
            _slaveLatency[i] = 0;
            _connectionSupervisionTimeout[i] = 0xC80;
            _minimumEventLength[i] = 0;
            _maximumEventLength[i] = 0xFFFF;
            _enabledPhy[i] = false;
        }
    };

    ExtendedConnectParameters_t& setScanParamteres(
        uint32_t scanInterval,
        uint32_t scanWindow,
        ble::phy_t phy = ble::phy_t::LE_1M
    ) {
        _enabledPhy[phy] = true;
        scanInterval = scanInterval[phy];
        scanWindow = scanWindow[phy];
        return *this;
    }

    ExtendedConnectParameters_t& setConnectionParamteres(
        uint16_t minConnectionInterval,
        uint16_t maxConnectionInterval,
        uint16_t slaveLatency,
        uint16_t connectionSupervisionTimeout,
        uint32_t _minimumEventLength,
        uint32_t _maximumEventLength,
        ble::phy_t phy = ble::phy_t::LE_1M
    ) {
        _enabledPhy[phy] = true;
        _minConnectionInterval = minConnectionInterval[phy];
        maxConnectionInterval = maxConnectionInterval[phy];
        slaveLatency = slaveLatency[phy];
        connectionSupervisionTimeout = connectionSupervisionTimeout[phy];
        _minimumEventLength = _minimumEventLength[phy];
        _maximumEventLength = _maximumEventLength[phy];
        return *this;
    }

    ExtendedConnectParameters_t& setScanParamteres(
        ble::own_address_type_t ownAddress
    ) {
        _ownAddressType = ownAddress;
        return *this;
    }

    ExtendedConnectParameters_t& setScanParamteres(
        ble::scanning_policy_mode_t filterPolicy
    ) {
        _filterPolicy = filterPolicy;
        return *this;
    }

private:
    ble::scanning_policy_mode_t _filterPolicy;
    ble::own_address_type_t _ownAddressType;
    uint32_t _scanInterval[MAX_PARAM_PHYS];
    uint32_t _scanWindow[MAX_PARAM_PHYS];
    uint16_t _minConnectionInterval[MAX_PARAM_PHYS];
    uint16_t _maxConnectionInterval[MAX_PARAM_PHYS];
    uint16_t _slaveLatency[MAX_PARAM_PHYS];
    uint16_t _connectionSupervisionTimeout[MAX_PARAM_PHYS];
    uint32_t _minimumEventLength[MAX_PARAM_PHYS];
    uint32_t _maximumEventLength[MAX_PARAM_PHYS];
    bool _enabledPhy[MAX_PARAM_PHYS];
};

/**
 * @}
 * @}
 */

#endif /* ifndef MBED_EXTENDED_CONNECT_PARAMETERS_H__ */
