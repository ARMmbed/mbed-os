/* mbed Microcontroller Library
 * Copyright (c) 2006-2020 ARM Limited
 *
 * SPDX-License-Identifier: Apache-2.0
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

#include <cstdint>

#include "ble/common/blecommon.h"
#include "ble/common/BLETypes.h"

namespace ble {

/**
 * @addtogroup ble
 * @{
 * @addtogroup gap
 * @{
 */

/**
 * Parameters defining the scan process.
 *
 * The scan process is defined by two main parameters:
 *   - The scan window that defines how long the device should scan.
 *   - The scan window that defines how frequently the device should scan.
 *
 * To scan continuously, the scan window and the scan interval should have the
 * same value.
 *
 * To get extra data from the advertising device, the scanner can send scan
 * requests to the advertiser; the advertiser may respond with scan responses.
 * It is possible to select what type of address is used to issue the scan request.
 *
 * With Bluetooth 5, devices can advertise on more physical channels, and by
 * extension, they can scan on more physical channels. It is possible to define
 * independent scan parameters for every scannable physical channel.
 */
class ScanParameters {
public:

    /**
     * Scan configuration of a physical channel.
     */
    struct phy_configuration_t {
        /**
         * Construct a phy_configuration_t.
         * @param scan_interval The scan interval.
         * @param scan_window The scan window.
         * @param active_scanning True if scan request should be sent and false
         * otherwise.
         */
        phy_configuration_t(
            scan_interval_t scan_interval = scan_interval_t::min(),
            scan_window_t scan_window = scan_window_t::min(),
            bool active_scanning = false
        ) :
            interval(scan_interval),
            window(scan_window),
            active_scanning(active_scanning)
        {
            if (window.value() > interval.value()) {
                interval = window;
            }
        }

        /**
         * Get the scan interval.
         */
        const scan_window_t &getInterval() const
        {
            return interval;
        }

        /**
         * Get the scan window.
         */
        const scan_interval_t &getWindow() const
        {
            return window;
        }

        /**
         * Return if active scanning is set.
         */
        bool isActiveScanningSet() const
        {
            return active_scanning;
        }

    private:
        scan_interval_t interval;
        scan_window_t window;
        bool active_scanning;
    };

    /**
     * Construct a ScanParameters object that operates on a selected PHY.
     *
     * @param phy The phy to configure.
     * @param scan_interval The scan interval.
     * @param scan_window The scan window.
     * @param active_scanning active scanning flag.
     * @param own_address_type Address type used in scan requests.
     * @param scanning_filter_policy Filter applied.
     */
    ScanParameters(
        phy_t phy = phy_t::LE_1M,
        scan_window_t scan_interval = scan_interval_t::min(),
        scan_interval_t scan_window = scan_window_t::min(),
        bool active_scanning = false,
        own_address_type_t own_address_type = own_address_type_t::RANDOM,
        scanning_filter_policy_t scanning_filter_policy = scanning_filter_policy_t::NO_FILTER
    ) :
        own_address_type(own_address_type),
        scanning_filter_policy(scanning_filter_policy),
        phys(phy),
        phy_1m_configuration(),
        phy_coded_configuration()
    {
        phy_configuration_t conf(scan_interval, scan_window, active_scanning);
        if (phy == phy_t::LE_1M) {
            phy_1m_configuration = conf;
        }
#if BLE_FEATURE_PHY_MANAGEMENT
        else if (phy == phy_t::LE_CODED) {
            phy_coded_configuration = conf;
        }
#endif // BLE_FEATURE_PHY_MANAGEMENT
    }

    /**
     * Set the address type used for scan requests.
     * @param address The type of address to use during scan requests.
     * @return A reference to this object.
     */
    ScanParameters &setOwnAddressType(own_address_type_t address)
    {
        own_address_type = address;
        return *this;
    }

    /**
     * Get the address type used during scan requests.
     */
    own_address_type_t getOwnAddressType() const
    {
        return own_address_type;
    }

    /**
     * Set the filter to apply during scanning.
     * @param filter_policy The filter to apply during scanning.
     * @return A reference to this object.
     */
    ScanParameters &setFilter(scanning_filter_policy_t filter_policy)
    {
        scanning_filter_policy = filter_policy;
        return *this;
    }

    /**
     * Get the filter to use during scanning
     */
    scanning_filter_policy_t getFilter() const
    {
#if BLE_FEATURE_WHITELIST
        return scanning_filter_policy;
#else
        return scanning_filter_policy_t::NO_FILTER;
#endif // BLE_FEATURE_WHITELIST
    }

    /**
     * Enable or disable PHYs that should be used during scanning.
     * @param enable_1m True to enable the 1M phy and false to disable it.
     * @param enable_coded True to enable the coded phy and false to disable it.
     * @return A reference to this object.
     */
    ScanParameters &setPhys(bool enable_1m, bool enable_coded)
    {
#if BLE_FEATURE_PHY_MANAGEMENT
        phys.set_1m(enable_1m);
        phys.set_coded(enable_coded);
#endif // BLE_FEATURE_PHY_MANAGEMENT
        return *this;
    }

    /**
     * Get the PHYs to use during scanning.
     */
    phy_set_t getPhys() const
    {
        return phys;
    }

    /**
     * Set the 1M scan configuration.
     * @param interval The scan interval to use.
     * @param window The scan window to use.
     * @param active_scanning The active scanning flag.
     * @return A reference to this object.
     */
    ScanParameters &set1mPhyConfiguration(
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

    /**
     * Get the 1M scan configuration.
     */
    phy_configuration_t get1mPhyConfiguration() const
    {
        return phy_1m_configuration;
    }

    /**
     * Set the coded PHY scan configuration.
     * @param interval The scan interval to use.
     * @param window The scan window to use.
     * @param active_scanning The active scanning flag.
     * @return A reference to this object.
     */
    ScanParameters &setCodedPhyConfiguration(
        scan_interval_t interval,
        scan_window_t window,
        bool active_scanning
    )
    {
#if BLE_FEATURE_PHY_MANAGEMENT
        phys.set_coded(true);
        phy_coded_configuration = phy_configuration_t(
            interval, window, active_scanning
        );
#endif // BLE_FEATURE_PHY_MANAGEMENT
        return *this;
    }

    /**
     * Get the coded PHY scan configuration.
     */
    phy_configuration_t getCodedPhyConfiguration() const
    {
        return phy_1m_configuration;
    }

private:
    own_address_type_t own_address_type;
    scanning_filter_policy_t scanning_filter_policy;

    phy_set_t phys;

    phy_configuration_t phy_1m_configuration;
    phy_configuration_t phy_coded_configuration;
};

/**
 * @}
 * @}
 */

} // namespace ble

#endif /* ifndef MBED_GAP_SCAN_PARAMETERS_H__ */
