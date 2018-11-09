/* mbed Microcontroller Library
 * Copyright (c) 2018-2018 ARM Limited
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

#ifndef MBED_GAP_SCAN_PARAMETERS_H__
#define MBED_GAP_SCAN_PARAMETERS_H__

#include <stdint.h>
#include "ble/blecommon.h"
#include "BLETypes.h"

/**
 * @addtogroup ble
 * @{
 * @addtogroup gap
 * @{
 */


class GapScanParameters {
public:
    enum own_address_type_t {
        PUBLIC_ADDRESS = 0x00,
        RANDOM_ADDRESS = 0x01,
        PRIVATE_RESOLVABLE_PUBLIC_FALLBACK = 0x02,
        PRIVATE_RESOLVABLE_RANDOM_FALLBACK = 0x03
    };

    struct phy_configuration_t {
        uint16_t interval;
        uint16_t window;
        bool active_scanning;
    };

    GapScanParameters() :
        own_address_type(PUBLIC_ADDRESS),
        phy_1m_configuration(/* TODO */),
        phy_coded_configuration(/* TODO */)
    { }

    GapScanParameters& set_own_address_type(own_address_type_t address)
    {
        own_address_type = address;
        return *this;
    }

    own_address_type_t get_own_address_type() const
    {
        return own_address_type;
    }

    GapScanParameters& set_scanning_filter_policy(ble::scanning_policy_mode_t filter_policy)
    {
        scanning_filter_policy = filter_policy;
        return *this;
    }

    ble::scanning_policy_mode_t get_scanning_filter_policy() const
    {
        return scanning_filter_policy;
    }

    GapScanParameters& set_scanning_phys(bool enable_1m, bool enable_coded)
    {
        phys.set_1m(enable_1m);
        phys.set_coded(enable_coded);
        return *this;
    }

    ble::phy_set_t get_scanning_phys() const
    {
        return phys;
    }

    GapScanParameters& set_1m_phy_configuration(
        uint16_t interval,
        uint16_t window,
        bool active_scanning
    )
    {
        phys.set_1m(true);
        phy_configuration_t new_conf = { interval, window, active_scanning };
        phy_1m_configuration = new_conf;
        return *this;
    }

    phy_configuration_t get_1m_configuration() const
    {
        return phy_1m_configuration;
    }

    GapScanParameters& set_coded_phy_configuration(
        uint16_t interval,
        uint16_t window,
        bool active_scanning
    )
    {
        phys.set_coded(true);
        phy_configuration_t new_conf = { interval, window, active_scanning };
        phy_coded_configuration = new_conf;
        return *this;
    }

    phy_configuration_t get_coded_configuration() const
    {
        return phy_1m_configuration;
    }

private:
    own_address_type_t own_address_type;
    ble::scanning_policy_mode_t scanning_filter_policy;

    ble::phy_set_t phys;

    phy_configuration_t phy_1m_configuration;
    phy_configuration_t phy_coded_configuration;
};

/**
 * @}
 * @}
 */

#endif /* ifndef MBED_GAP_SCAN_PARAMETERS_H__ */
