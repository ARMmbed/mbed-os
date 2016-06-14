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

#ifndef __GAP_SCANNING_PARAMS_H__
#define __GAP_SCANNING_PARAMS_H__

class GapScanningParams {
public:
    static const unsigned SCAN_INTERVAL_MIN = 0x0004; /**< Minimum Scan interval in 625us units - 2.5ms. */
    static const unsigned SCAN_INTERVAL_MAX = 0x4000; /**< Maximum Scan interval in 625us units - 10.24s. */
    static const unsigned SCAN_WINDOW_MIN   = 0x0004; /**< Minimum Scan window in 625us units - 2.5ms. */
    static const unsigned SCAN_WINDOW_MAX   = 0x4000; /**< Maximum Scan window in 625us units - 10.24s. */
    static const unsigned SCAN_TIMEOUT_MIN  = 0x0001; /**< Minimum Scan timeout in seconds. */
    static const unsigned SCAN_TIMEOUT_MAX  = 0xFFFF; /**< Maximum Scan timeout in seconds. */

public:
    /**
     * Construct an instance of GapScanningParams.
     *
     * @param[in] interval
     *              The scan interval in milliseconds. Default is
     *              GapScanningParams::SCAN_INTERVAL_MIN.
     * @param[in] window
     *              The scan window in milliseconds. Default is
     *              GapScanningParams::SCAN_WINDOW_MAX.
     * @param[in] timeout
     *              The scan timeout in seconds. Default is 0.
     * @param[in] activeScanning
     *              Set to True if active-scanning is required. This is used to
     *              fetch the scan response from a peer if possible. Default is
     *              false.
     */
    GapScanningParams(uint16_t interval       = SCAN_INTERVAL_MAX,
                      uint16_t window         = SCAN_WINDOW_MAX,
                      uint16_t timeout        = 0,
                      bool     activeScanning = false);

    static const uint16_t UNIT_0_625_MS = 625;  /**< Number of microseconds in 0.625 milliseconds. */
    /**
     * Convert milliseconds to units of 0.625ms.
     *
     * @param[in] durationInMillis
     *              The number of milliseconds to convert.
     *
     * @return The value of @p durationInMillis in units of 0.625ms.
     */
    static uint16_t MSEC_TO_SCAN_DURATION_UNITS(uint32_t durationInMillis) {
        return (durationInMillis * 1000) / UNIT_0_625_MS;
    }

    /**
     * Set the scan interval.
     *
     * @param[in] newIntervalInMS
     *              New scan interval in milliseconds.
     *
     * @return BLE_ERROR_NONE if the new scan interval was set successfully.
     */
    ble_error_t setInterval(uint16_t newIntervalInMS);

    /**
     * Set the scan window.
     *
     * @param[in] newWindowInMS
     *              New scan window in milliseconds.
     *
     * @return BLE_ERROR_NONE if the new scan window was set successfully.
     */
    ble_error_t setWindow(uint16_t newWindowInMS);

    /**
     * Set the scan timeout.
     *
     * @param[in] newTimeout
     *              New scan timeout in seconds.
     *
     * @return BLE_ERROR_NONE if the new scan window was set successfully.
     */
    ble_error_t setTimeout(uint16_t newTimeout);

    /**
     * Set active scanning. This is used to fetch the scan response from a peer
     * if possible.
     *
     * @param[in] activeScanning
     *              The new boolean value of active scanning.
     */
    void setActiveScanning(bool activeScanning);

public:
    /**
     * Get the scan interval.
     *
     * @return the scan interval in units of 0.625ms.
     */
    uint16_t getInterval(void) const {
        return _interval;
    }

    /**
     * Get the scan window.
     *
     * @return the scan window in units of 0.625ms.
     */
    uint16_t getWindow(void) const {
        return _window;
    }

    /**
     * Get the scan timeout.
     *
     * @return The scan timeout in seconds.
     */
    uint16_t getTimeout(void) const {
        return _timeout;
    }

    /**
     * Check whether active scanning is set.
     *
     * @return True if active scanning is set, false otherwise.
     */
    bool getActiveScanning(void) const {
        return _activeScanning;
    }

private:
    uint16_t _interval; /**< Scan interval in units of 625us (between 2.5ms and 10.24s). */
    uint16_t _window;   /**< Scan window in units of 625us (between 2.5ms and 10.24s). */
    uint16_t _timeout;  /**< Scan timeout between 0x0001 and 0xFFFF in seconds; 0x0000 disables timeout. */
    bool     _activeScanning; /**< Obtain the peer device's advertising data and (if possible) scanResponse. */

private:
    /* Disallow copy constructor. */
    GapScanningParams(const GapScanningParams &);
    GapScanningParams& operator =(const GapScanningParams &in);
};

#endif /* ifndef __GAP_SCANNING_PARAMS_H__ */
