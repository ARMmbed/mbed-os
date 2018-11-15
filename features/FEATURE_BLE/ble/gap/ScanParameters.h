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

namespace ble {

class ScanParameters {
public:
    typedef ble::scan_window_t scan_window_t;
    typedef ble::scan_interval_t scan_interval_t;

    struct phy_configuration_t {
        phy_configuration_t(
            scan_window_t scan_interval,
            scan_interval_t scan_window,
            bool active_scanning
        ) : interval(scan_interval),
            window(scan_window),
            active_scanning(active_scanning)
        {
            if (window.value() > interval.value()) {
                interval = window;
            }
        }

        scan_window_t interval;
        scan_interval_t window;
        bool active_scanning;
    };

    ScanParameters() :
        own_address_type(own_address_type_t::PUBLIC),
        scanning_filter_policy(scanning_filter_policy_t::NO_FILTER),
        phys(ble::phy_set_t::PHY_SET_1M),
        phy_1m_configuration(
            scan_interval_t::min(), scan_window_t::min(), true
        ),
        phy_coded_configuration(
            scan_interval_t::min(), scan_window_t::min(), true
        )
    { }

    ScanParameters& set_own_address_type(own_address_type_t address)
    {
        own_address_type = address;
        return *this;
    }

    own_address_type_t get_own_address_type() const
    {
        return own_address_type;
    }

    ScanParameters& set_scanning_filter_policy(ble::scanning_filter_policy_t filter_policy)
    {
        scanning_filter_policy = filter_policy;
        return *this;
    }

    ble::scanning_filter_policy_t get_scanning_filter_policy() const
    {
        return scanning_filter_policy;
    }

    ScanParameters& set_scanning_phys(bool enable_1m, bool enable_coded)
    {
        phys.set_1m(enable_1m);
        phys.set_coded(enable_coded);
        return *this;
    }

    ble::phy_set_t get_scanning_phys() const
    {
        return phys;
    }

    ScanParameters& set_1m_phy_configuration(
        scan_interval_t interval,
        scan_window_t window,
        bool active_scanning
    )
    {
        phys.set_1m(true);
        phy_1m_configuration = phy_configuration_t(
            interval, window, active_scanning
        );
        return *this;
    }

    phy_configuration_t get_1m_configuration() const
    {
        return phy_1m_configuration;
    }

    ScanParameters& set_coded_phy_configuration(
        scan_interval_t interval,
        scan_window_t window,
        bool active_scanning
    )
    {
        phys.set_coded(true);
        phy_coded_configuration = phy_configuration_t(
            interval, window, active_scanning
        );
        return *this;
    }

    phy_configuration_t get_coded_configuration() const
    {
        return phy_1m_configuration;
    }

private:
    own_address_type_t own_address_type;
    ble::scanning_filter_policy_t scanning_filter_policy;

    ble::phy_set_t phys;

    phy_configuration_t phy_1m_configuration;
    phy_configuration_t phy_coded_configuration;
};

} // namespace ble

/**
 * @}
 * @}
 */

#endif /* ifndef MBED_GAP_SCAN_PARAMETERS_H__ */
