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

/**
 * @addtogroup ble
 * @{
 * @addtogroup gap
 * @{
 */

/**
 * Parameters defining the advertising process.
 *
 * Advertising parameters are a triplet of three value:
 *   - The Advertising mode modeled after AdvertisingType_t. It defines
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
     * Encapsulates the peripheral advertising modes.
     *
     * It determine how the device appears to other scanner and peripheral
     * devices in the scanning range.
     */
    enum AdvertisingType_t {
        /**
         * Device is connectable, scannable and doesn't expect connection from a
         * specific peer.
         *
         * @see Vol 3, Part C, Section 9.3.4 and Vol 6, Part B, Section 2.3.1.1.
         */
        ADV_CONNECTABLE_UNDIRECTED,

        /**
         * Device is connectable and expects connection from a specific peer.
         *
         * @see Vol 3, Part C, Section 9.3.3 and Vol 6, Part B, Section 2.3.1.2.
         */
        ADV_CONNECTABLE_DIRECTED,

        /**
         * Device is scannable but not connectable.
         *
         * @see Vol 6, Part B, Section 2.3.1.4.
         */
        ADV_SCANNABLE_UNDIRECTED,

        /**
         * Device is not connectable and not scannable.
         *
         * @see Vol 3, Part C, Section 9.3.2 and Vol 6, Part B, Section 2.3.1.3.
         */
        ADV_NON_CONNECTABLE_UNDIRECTED
    };

    /**
     * Alias for GapAdvertisingParams::AdvertisingType_t.
     *
     * @deprecated  Future releases will drop this type alias.
     */
    typedef enum AdvertisingType_t AdvertisingType;

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
        AdvertisingType_t advType  = ADV_CONNECTABLE_UNDIRECTED,
        uint16_t interval = GAP_ADV_PARAMS_INTERVAL_MIN_NONCON,
        uint16_t timeout = 0
    ) :
        _advType(advType),
        _interval(interval),
        _timeout(timeout)
    {
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
    AdvertisingType_t getAdvertisingType(void) const
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
    void setAdvertisingType(AdvertisingType_t newAdvType)
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

private:
    /**
     * The advertising type.
     */
    AdvertisingType_t _advType;

    /**
     * The advertising interval in ADV duration units (in other words, 0.625ms).
     */
    uint16_t _interval;

    /**
     * The advertising timeout in seconds.
     */
    uint16_t _timeout;
};

/**
 * @}
 * @}
 */

#endif /* ifndef MBED_GAP_ADVERTISING_PARAMS_H__ */
