/*
 * mbed Microcontroller Library
 * Copyright (c) 2006-2019 ARM Limited
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

#define ETHERNET 1
#define WIFI 2
#define MESH 3
#define CELLULAR 4

#if !defined(MBED_CONF_TARGET_NETWORK_DEFAULT_INTERFACE_TYPE)
#error [NOT_SUPPORTED] No network interface found on this target.
#endif

#if MBED_CONF_TARGET_NETWORK_DEFAULT_INTERFACE_TYPE == ETHERNET
#define TEST_NETWORK_TYPE "Ethernet"
#elif MBED_CONF_TARGET_NETWORK_DEFAULT_INTERFACE_TYPE == WIFI
#define TEST_NETWORK_TYPE "WiFi"
#elif MBED_CONF_TARGET_NETWORK_DEFAULT_INTERFACE_TYPE == MESH
#define TEST_NETWORK_TYPE "Mesh"
#elif MBED_CONF_TARGET_NETWORK_DEFAULT_INTERFACE_TYPE == CELLULAR
#define TEST_NETWORK_TYPE "Cellular"
#else
#define TEST_NETWORK_TYPE "Unknown"
#error [NOT_SUPPORTED] Either WiFi, Ethernet or Cellular network interface need to be enabled
#endif
