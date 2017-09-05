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

#ifndef __GAP_ADVERTISING_PARAMS_H__
#define __GAP_ADVERTISING_PARAMS_H__

/**
 *  This class provides a wrapper for the core advertising parameters,
 *  including the advertising type (Connectable Undirected,
 *  Non Connectable Undirected and so on), as well as the advertising and
 *  timeout intervals.
 */
class GapAdvertisingParams {
public:
    /**
     * Minimum Advertising interval for connectable undirected and connectable
     * directed events in 625us units - 20ms.
     */
    static const unsigned GAP_ADV_PARAMS_INTERVAL_MIN        = 0x0020;
    /**
     * Minimum Advertising interval for scannable and non-connectable
     * undirected events in 625us units - 100ms.
     */
    static const unsigned GAP_ADV_PARAMS_INTERVAL_MIN_NONCON = 0x00A0;
    /**
     * Maximum Advertising interval in 625us units - 10.24s.
     */
    static const unsigned GAP_ADV_PARAMS_INTERVAL_MAX        = 0x4000;
    /**
     * Maximum advertising timeout seconds.
     */
    static const unsigned GAP_ADV_PARAMS_TIMEOUT_MAX         = 0x3FFF;

    /**
     * Encapsulates the peripheral advertising modes, which determine how
     * the device appears to other central devices in hearing range.
     */
    enum AdvertisingType_t {
        ADV_CONNECTABLE_UNDIRECTED,     /**< Vol 3, Part C, Section 9.3.4 and Vol 6, Part B, Section 2.3.1.1. */
        ADV_CONNECTABLE_DIRECTED,       /**< Vol 3, Part C, Section 9.3.3 and Vol 6, Part B, Section 2.3.1.2. */
        ADV_SCANNABLE_UNDIRECTED,       /**< Include support for Scan Response payloads, see Vol 6, Part B, Section 2.3.1.4. */
        ADV_NON_CONNECTABLE_UNDIRECTED  /**< Vol 3, Part C, Section 9.3.2 and Vol 6, Part B, Section 2.3.1.3. */
    };
    /**
     * Type alias for GapAdvertisingParams::AdvertisingType_t.
     *
     * @deprecated  This type alias will be dropped in future releases.
     */
    typedef enum AdvertisingType_t AdvertisingType;

public:
    /**
     * Construct an instance of GapAdvertisingParams.
     *
     * @param[in] advType
     *              Type of advertising. Default is
     *              GapAdvertisingParams::ADV_CONNECTABLE_UNDIRECTED.
     * @param[in] interval
     *              Advertising interval in units of 0.625ms. Default is
     *              GapAdvertisingParams::GAP_ADV_PARAMS_INTERVAL_MIN_NONCON.
     * @param[in] timeout
     *              Advertising timeout. Default is 0.
     */
    GapAdvertisingParams(AdvertisingType_t advType  = ADV_CONNECTABLE_UNDIRECTED,
                         uint16_t          interval = GAP_ADV_PARAMS_INTERVAL_MIN_NONCON,
                         uint16_t          timeout  = 0) : _advType(advType), _interval(interval), _timeout(timeout) {
        /* Interval checks. */
        if (_advType == ADV_CONNECTABLE_DIRECTED) {
            /* Interval must be 0 in directed connectable mode. */
            _interval = 0;
        } else if (_advType == ADV_NON_CONNECTABLE_UNDIRECTED) {
            /* Min interval is slightly larger than in other modes. */
            if (_interval < GAP_ADV_PARAMS_INTERVAL_MIN_NONCON) {
                _interval = GAP_ADV_PARAMS_INTERVAL_MIN_NONCON;
            }
            if (_interval > GAP_ADV_PARAMS_INTERVAL_MAX) {
                _interval = GAP_ADV_PARAMS_INTERVAL_MAX;
            }
        } else {
            /* Stay within interval limits. */
            if (_interval < GAP_ADV_PARAMS_INTERVAL_MIN) {
                _interval = GAP_ADV_PARAMS_INTERVAL_MIN;
            }
            if (_interval > GAP_ADV_PARAMS_INTERVAL_MAX) {
                _interval = GAP_ADV_PARAMS_INTERVAL_MAX;
            }
        }

        /* Timeout checks. */
        if (timeout) {
            /* Stay within timeout limits. */
            if (_timeout > GAP_ADV_PARAMS_TIMEOUT_MAX) {
                _timeout = GAP_ADV_PARAMS_TIMEOUT_MAX;
            }
        }
    }

    static const uint16_t UNIT_0_625_MS = 625;  /**< Number of microseconds in 0.625 milliseconds. */
    /**
     * Convert milliseconds to units of 0.625ms.
     *
     * @param[in] durationInMillis
     *              The number of milliseconds to convert.
     *
     * @return The value of @p durationInMillis in units of 0.625ms.
     */
    static uint16_t MSEC_TO_ADVERTISEMENT_DURATION_UNITS(uint32_t durationInMillis) {
        return (durationInMillis * 1000) / UNIT_0_625_MS;
    }
    /**
     * Convert units of 0.625ms to milliseconds.
     *
     * @param[in] gapUnits
     *              The number of units of 0.625ms to convert.
     *
     * @return The value of @p gapUnits in milliseconds.
     */
    static uint16_t ADVERTISEMENT_DURATION_UNITS_TO_MS(uint16_t gapUnits) {
        return (gapUnits * UNIT_0_625_MS) / 1000;
    }

    /**
     * Get the advertising type.
     *
     * @return The advertising type.
     */
    AdvertisingType_t getAdvertisingType(void) const {
        return _advType;
    }

    /**
     * Get the advertising interval in milliseconds.
     *
     * @return The advertisement interval (in milliseconds).
     */
    uint16_t getInterval(void) const {
        return ADVERTISEMENT_DURATION_UNITS_TO_MS(_interval);
    }

    /**
     * Get the advertisement interval in units of 0.625ms.
     *
     * @return The advertisement interval in advertisement duration units (0.625ms units).
     */
    uint16_t getIntervalInADVUnits(void) const {
        return _interval;
    }

    /**
     * Get the advertising timeout.
     *
     * @return The advertising timeout (in seconds).
     */
    uint16_t getTimeout(void) const {
        return _timeout;
    }

    /**
     * Set the advertising type.
     *
     * @param[in] newAdvType
     *              The new advertising type.
     */
    void setAdvertisingType(AdvertisingType_t newAdvType) {
        _advType = newAdvType;
    }

    /**
     * Set the advertising interval in milliseconds.
     *
     * @param[in] newInterval
     *              The new advertising interval in milliseconds.
     */
    void setInterval(uint16_t newInterval) {
        _interval = MSEC_TO_ADVERTISEMENT_DURATION_UNITS(newInterval);
    }

    /**
     * Set the advertising timeout.
     *
     * @param[in] newTimeout
     *              The new advertising timeout (in seconds).
     */
    void setTimeout(uint16_t newTimeout) {
        _timeout = newTimeout;
    }

private:
    AdvertisingType_t _advType;  /**< The advertising type. */
    uint16_t          _interval; /**< The advertising interval in ADV duration units (i.e. 0.625ms). */
    uint16_t          _timeout;  /**< The advertising timeout in seconds. */
};

#endif /* ifndef __GAP_ADVERTISING_PARAMS_H__ */
