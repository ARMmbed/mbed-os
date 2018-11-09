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

#ifndef MBED_GAP_SCANNING_PARAMS_H__
#define MBED_GAP_SCANNING_PARAMS_H__

/**
 * @addtogroup ble
 * @{
 * @addtogroup gap
 * @{
 */

/**
 * Parameters defining the scan process.
 *
 * Four distinct parameters define the scan procedure:
 *    - Scan window: Period during which the scanner listens to advertising
 *      channels. That value is in the range of 2.5ms to 10.24s. This value
 *      can be set at construction time, updated by calling setWindow() and
 *      retrieved by invoking getWindow().
 *
 *    - Scan interval: Interval between the start of two consecutive scan windows.
 *      That value shall be greater or equal to the scan window value. The
 *      maximum allowed value is 10.24ms. The scan interval value can be set at
 *      construction time, updated with a call to setInterval() and queried by a
 *      call to getInterval().
 *
 *    - Timeout: The duration of the scan procedure if any. It can be set at
 *      construction time, updated with setTimeout() and obtained from
 *      getTimeout().
 *
 *    - Active scanning: If set, then the scanner sends scan requests to a scannable
 *      or connectable advertiser. Advertisers may respond to the scan request
 *      by a scan response containing the scan response payload. If not set, then
 *      the scanner does not send any request. This flag is set at construction
 *      time, may be updated with the help of setActiveScanning() and retrieved
 *      by getActiveScanning().
 *
 * @note If the scan window's duration is equal to the scan interval, then the
 * device listens continuously during the scan procedure.
 */
class GapScanningParams {
public:
    /**
     * Minimum Scan interval in 625us units - 2.5ms.
     */
    static const unsigned SCAN_INTERVAL_MIN = 0x0004;

    /**
     * Maximum Scan interval in 625us units - 10.24s.
     */
    static const unsigned SCAN_INTERVAL_MAX = 0x4000;

    /**
     * Minimum Scan window in 625us units - 2.5ms.
     */
    static const unsigned SCAN_WINDOW_MIN = 0x0004;

    /**
     * Maximum Scan window in 625us units - 10.24s.
     */
    static const unsigned SCAN_WINDOW_MAX = 0x4000;

    /**
     * Minimum Scan duration in seconds.
     */
    static const unsigned SCAN_TIMEOUT_MIN = 0x0001;

    /**
     * Maximum Scan duration in seconds.
     */
    static const unsigned SCAN_TIMEOUT_MAX = 0xFFFF;

public:
    /**
     * Construct an instance of GapScanningParams.
     *
     * @param[in] interval Milliseconds interval between the start of two
     * consecutive scan windows. The value passed is between the scan
     * window value and 10.24 seconds.
     *
     * @param[in] window Milliseconds period during which the device
     * listens to advertising channels. The value of the scan window is in
     * the range of 2.5ms to 10.24s.
     *
     * @param[in] timeout Duration in seconds of the scan procedure. The special
     * value 0 may be used when the scan procedure is not time bounded.
     *
     * @param[in] activeScanning If true, then the scanner sends scan requests to
     * to scannable or connectable advertiser. Advertisers may respond to the
     * scan request by a scan response containing the scan response payload. If
     * false, the scanner does not send any request.
     *
     * @note If interval is equal to window
     */
    GapScanningParams(
        uint16_t interval = SCAN_INTERVAL_MAX,
        uint16_t window = SCAN_WINDOW_MAX,
        uint16_t timeout = 0,
        bool activeScanning = false
    );

    /**
     * Number of microseconds in 0.625 milliseconds.
     */
    static const uint16_t UNIT_0_625_MS = 625;

    /**
     * Convert milliseconds to units of 0.625ms.
     *
     * @param[in] durationInMillis Milliseconds to convert.
     *
     * @return The value of @p durationInMillis in units of 0.625ms.
     */
    static uint16_t MSEC_TO_SCAN_DURATION_UNITS(uint32_t durationInMillis)
    {
        return (durationInMillis * 1000) / UNIT_0_625_MS;
    }

    /**
     * Update the scan interval.
     *
     * @param[in] newIntervalInMS New scan interval in milliseconds.
     *
     * @return BLE_ERROR_NONE if the new scan interval was set successfully.
     */
    ble_error_t setInterval(uint16_t newIntervalInMS);

    /**
     * Update the scan window.
     *
     * @param[in] newWindowInMS New scan window in milliseconds.
     *
     * @return BLE_ERROR_NONE if the new scan window was set successfully.
     */
    ble_error_t setWindow(uint16_t newWindowInMS);

    /**
     * Update the scan timeout.
     *
     * @param[in] newTimeout New scan timeout in seconds.
     *
     * @return BLE_ERROR_NONE if the new scan window was set successfully.
     */
    ble_error_t setTimeout(uint16_t newTimeout);

    /**
     * Update the active scanning flag.
     *
     * @param[in] activeScanning New boolean value of active scanning.
     */
    void setActiveScanning(bool activeScanning);

public:
    /**
     * Get the scan interval.
     *
     * @return the scan interval in units of 0.625ms.
     */
    uint16_t getInterval(void) const
    {
        return _interval;
    }

    /**
     * Get the scan window.
     *
     * @return the scan window in units of 0.625ms.
     */
    uint16_t getWindow(void) const
    {
        return _window;
    }

    /**
     * Get the scan timeout.
     *
     * @return The scan timeout in seconds.
     */
    uint16_t getTimeout(void) const
    {
        return _timeout;
    }

    /**
     * Check whether active scanning is set.
     *
     * @return True if active scanning is set, false otherwise.
     */
    bool getActiveScanning(void) const
    {
        return _activeScanning;
    }

private:
    /**
     * Scan interval in units of 625us (between 2.5ms and 10.24s).
     */
    uint16_t _interval;

    /**
     * Scan window in units of 625us (between 2.5ms and 10.24s).
     */
    uint16_t _window;

    /**
     * Scan timeout between 0x0001 and 0xFFFF in seconds; 0x0000 disables timeout.
     */
    uint16_t _timeout;

    /**
     * Obtain the peer device's advertising data and (if possible) scanResponse.
     */
    bool _activeScanning;

private:
    /* Disallow copy constructor. */
    GapScanningParams(const GapScanningParams &);
    GapScanningParams& operator =(const GapScanningParams &in);
};

/**
 * @}
 * @}
 */

#endif /* ifndef MBED_GAP_SCANNING_PARAMS_H__ */
