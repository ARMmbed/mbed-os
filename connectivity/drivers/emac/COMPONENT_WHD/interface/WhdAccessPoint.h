/* WHD Access Point Interface
 * Copyright (c) 2017-2019 ARM Limited
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

#ifndef WHD_ACCESS_POINT_H
#define WHD_ACCESS_POINT_H

#include "netsocket/WiFiAccessPoint.h"
#include "whd_types.h"

/* Enum for scan result type */
enum scan_result_type {
    SRES_TYPE_WIFI_ACCESS_POINT,
    SRES_TYPE_WHD_ACCESS_POINT
};

/** WhdAccessPoint class
 *
 *  Class that represents a Whd Access Point
 *  which contains additional Whd specific information
 */
class WhdAccessPoint : public WiFiAccessPoint {
public:
    WhdAccessPoint() : WiFiAccessPoint() {};
    WhdAccessPoint(nsapi_wifi_ap_t ap, whd_bss_type_t bss_type, uint8_t *ie_ptr, uint32_t ie_len);

    /** Define move assignment and prevent copy-assignment
     *
     *  Due to IE element data could have large memory footprint,
     *  only move assignment is allowed.
     */
    WhdAccessPoint &operator=(WhdAccessPoint &&rhs);
    WhdAccessPoint &operator=(const WhdAccessPoint &rhs) = delete;

    /** Get WHD access point's bss type
     *
     *  @return The whd_bss_type_t of the access point
     */
    whd_bss_type_t get_bss_type() const;

    /** Get WHD access point's IE data
     *
     *  @return The pointer to ie data buffer
     */
    uint8_t *get_ie_data() const;

    /** Get WHD access point's IE length
     *
     *  @return The ie data length
     */
    uint32_t get_ie_len() const;

    virtual ~WhdAccessPoint();

private:
    whd_bss_type_t _bss_type;
    uint8_t *_ie_ptr;          /**< Pointer to received Beacon/Probe Response IE(Information Element)         */
    uint32_t _ie_len;          /**< Length of IE(Information Element)                                         */
};

#endif
