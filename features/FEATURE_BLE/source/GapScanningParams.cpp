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

#include "ble/Gap.h"
#include "ble/GapScanningParams.h"

GapScanningParams::GapScanningParams(uint16_t interval, uint16_t window, uint16_t timeout, bool activeScanning) :
    _interval(MSEC_TO_SCAN_DURATION_UNITS(interval)),
    _window(MSEC_TO_SCAN_DURATION_UNITS(window)),
    _timeout(timeout),
    _activeScanning(activeScanning) {
    /* stay within limits */
    if (_interval < SCAN_INTERVAL_MIN) {
        _interval = SCAN_INTERVAL_MIN;
    }
    if (_interval > SCAN_INTERVAL_MAX) {
        _interval = SCAN_INTERVAL_MAX;
    }
    if (_window < SCAN_WINDOW_MIN) {
        _window = SCAN_WINDOW_MIN;
    }
    if (_window > SCAN_WINDOW_MAX) {
        _window = SCAN_WINDOW_MAX;
    }
}

ble_error_t
GapScanningParams::setInterval(uint16_t newIntervalInMS)
{
    uint16_t newInterval = MSEC_TO_SCAN_DURATION_UNITS(newIntervalInMS);
    if ((newInterval >= SCAN_INTERVAL_MIN) && (newInterval < SCAN_INTERVAL_MAX)) {
        _interval = newInterval;
        return BLE_ERROR_NONE;
    }

    return BLE_ERROR_PARAM_OUT_OF_RANGE;
}

ble_error_t
GapScanningParams::setWindow(uint16_t newWindowInMS)
{
    uint16_t newWindow = MSEC_TO_SCAN_DURATION_UNITS(newWindowInMS);
    if ((newWindow >= SCAN_WINDOW_MIN) && (newWindow < SCAN_WINDOW_MAX)) {
        _window   = newWindow;
        return BLE_ERROR_NONE;
    }

    return BLE_ERROR_PARAM_OUT_OF_RANGE;
}

ble_error_t
GapScanningParams::setTimeout(uint16_t newTimeout)
{
    _timeout  = newTimeout;
    return BLE_ERROR_NONE;
}

void
GapScanningParams::setActiveScanning(bool activeScanning)
{
    _activeScanning = activeScanning;
}
