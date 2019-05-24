/*---------------------------------------------------------------------------
 * Copyright (c) 2019, u-blox Malmö, All Rights Reserved
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

#ifndef UBLOX_WIFI_CONFIGAPINTERFACE_H
#define UBLOX_WIFI_CONFIGAPINTERFACE_H

#include <string.h>
#include "cb_wlan_driver_config.h"

/** UbloxWiFiConfigInterface class
 *
 *  Common interface that is shared between WiFi devices supporting SoftAP mode
 */
class UbloxWiFiConfigInterface
{
public:
    /** UbloxWiFiConfigInterface lifetime
     */
    virtual ~UbloxWiFiConfigInterface() {};

    /** Get general settings and tuning parameters
     *
     *
     *  @param setting setting to read.
     *  @return parameter value
     */
    virtual unsigned int get_config(void *setting) = 0;

    /**
     * Set general tuning parameter.
     * 
     * @param setting setting to modify.
     * @param value value to set.
     */
    virtual void set_config(void *setting, cb_uint32 value) = 0;
};

#endif

