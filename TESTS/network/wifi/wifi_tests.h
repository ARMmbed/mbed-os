/*
 * Copyright (c) 2018-2019, Arm Limited and affiliates.
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
#ifndef WIFI_TESTS_H
#define WIFI_TESTS_H

#include "WiFiInterface.h"

/** Get WiFiInterface based on provided
 * app_json. */
WiFiInterface *get_interface(void);

/**
 * Get security protocol to be used
 */
nsapi_security get_security(void);

/*
 * Test cases
 */

/** Test that constructor of the driver works. */
void wifi_constructor(void);

/** This test case is to test whether the driver accepts valid credentials and reject ones that are not valid. */
void wifi_set_credential(void);

/** Test validity of WiFiInterface::set_channel(). */
void wifi_set_channel(void);

/** Test WiFiInterface::get_rssi() API.
 * When connected, it should return valid RSSI value. When unconnected it should return 0. */
void wifi_get_rssi(void);

/** Test WiFiInterface::connect(ssid, pass, security, channel) with NULL parameters */
void wifi_connect_params_null(void);

/** Test WiFiInterface::connect(ssid, pass, security) with valid parameters for secure network */
void wifi_connect_params_valid_secure(void);

/** Test WiFiInterface::connect(ssid, pass, security, channel) with valid parameters for secure network using channel specified. */
void wifi_connect_params_channel(void);

/** Test WiFiInterface::connect(ssid, pass, security, channel) with valid parameters for secure network using wrong channel number. */
void wifi_connect_params_channel_fail(void);

/** Test WiFiInterface::connect() without parameters. Use set_credentials() for setting parameters. */
void wifi_connect(void);

/** Test WiFiInterface::connect() and disconnect() in nonblocking mode. Use set_credentials() for setting parameters. */
void wifi_connect_disconnect_nonblock(void);

/** Test WiFiInterface::connect() without parameters. Don't set parameters with set_credentials() */
void wifi_connect_nocredentials(void);

/** Test WiFiInterface::connect() without parameters. Use secure settings for set_credentials. */
void wifi_connect_secure(void);

/** Test WiFiInterface::connect() failing with wrong password. */
void wifi_connect_secure_fail(void);

/** Test WiFiInterface::connect() - disconnect() repeatition works. */
void wifi_connect_disconnect_repeat(void);

/** Call WiFiInterface::scan() with null parameters to get number of networks available. */
void wifi_scan_null(void);

/** Call WiFiInterface::scan() with valid accesspoint list allocated */
void wifi_scan(void);

#endif //WIFI_TESTS_H
