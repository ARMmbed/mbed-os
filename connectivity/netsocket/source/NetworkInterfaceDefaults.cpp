/* Network interface defaults
 * Copyright (c) 2018 ARM Limited
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

#include "netsocket/NetworkInterface.h"

#include "netsocket/EthInterface.h"
#include "netsocket/WiFiInterface.h"
#if MBED_CONF_CELLULAR_PRESENT
#include "netsocket/CellularInterface.h"
#endif // MBED_CONF_CELLULAR_PRESENT
#include "netsocket/MeshInterface.h"

/* Weak default instance static classes for the various abstract classes.
 * Applications can override these.
 */

MBED_WEAK EthInterface *EthInterface::get_default_instance()
{
    return get_target_default_instance();
}

MBED_WEAK WiFiInterface *WiFiInterface::get_default_instance()
{
    return get_target_default_instance();
}

MBED_WEAK MeshInterface *MeshInterface::get_default_instance()
{
    return get_target_default_instance();
}

#if MBED_CONF_CELLULAR_PRESENT
MBED_WEAK CellularInterface *CellularInterface::get_default_instance()
{
    return get_target_default_instance();
}
#endif // MBED_CONF_CELLULAR_PRESENT

/* For other types, we can provide a reasonable get_target_default_instance
 * in some cases. This is done in EthernetInterface.cpp, mbed-mesh-api and
 * OnboardCellularInterface.cpp. We have no implementation for WiFi, so a
 * default empty one lives here.
 */

MBED_WEAK WiFiInterface *WiFiInterface::get_target_default_instance()
{
    return NULL;
}

/* The top-level get_default_instance() call. Weak for application override. */
MBED_WEAK NetworkInterface *NetworkInterface::get_default_instance()
{
    return get_target_default_instance();
}


/* Helpers to set default parameters - used by NetworkInterface::get_default_instance,
 * but exposed for apps which want to get these defaults after requesting a specific type.
 */
void NetworkInterface::set_default_parameters()
{

}

void WiFiInterface::set_default_parameters()
{
#ifdef MBED_CONF_NSAPI_DEFAULT_WIFI_SSID
#ifndef MBED_CONF_NSAPI_DEFAULT_WIFI_PASSWORD
#define MBED_CONF_NSAPI_DEFAULT_WIFI_PASSWORD NULL
#endif
#ifndef MBED_CONF_NSAPI_DEFAULT_WIFI_SECURITY
#define MBED_CONF_NSAPI_DEFAULT_WIFI_SECURITY NONE
#endif
#define concat_(x,y) x##y
#define concat(x,y) concat_(x,y)
#define SECURITY concat(NSAPI_SECURITY_,MBED_CONF_NSAPI_DEFAULT_WIFI_SECURITY)
    set_credentials(MBED_CONF_NSAPI_DEFAULT_WIFI_SSID, MBED_CONF_NSAPI_DEFAULT_WIFI_PASSWORD, SECURITY);
#endif
}

#if MBED_CONF_CELLULAR_PRESENT
void CellularInterface::set_default_parameters()
{
    /* CellularInterface is expected to attempt to work without any parameters - we
     * will try, at least.
     */
#ifdef MBED_CONF_NSAPI_DEFAULT_CELLULAR_APN
#ifndef MBED_CONF_NSAPI_DEFAULT_CELLULAR_USERNAME
#define MBED_CONF_NSAPI_DEFAULT_CELLULAR_USERNAME NULL
#endif
#ifndef MBED_CONF_NSAPI_DEFAULT_CELLULAR_PASSWORD
#define MBED_CONF_NSAPI_DEFAULT_CELLULAR_PASSWORD NULL
#endif
    set_credentials(MBED_CONF_NSAPI_DEFAULT_CELLULAR_APN, MBED_CONF_NSAPI_DEFAULT_CELLULAR_USERNAME, MBED_CONF_NSAPI_DEFAULT_CELLULAR_PASSWORD);
#endif
#ifdef MBED_CONF_NSAPI_DEFAULT_CELLULAR_SIM_PIN
    set_sim_pin(MBED_CONF_NSAPI_DEFAULT_CELLULAR_SIM_PIN);
#endif
#ifdef MBED_CONF_NSAPI_DEFAULT_CELLULAR_PLMN
    set_plmn(MBED_CONF_NSAPI_DEFAULT_CELLULAR_PLMN);
#endif
}
#endif // MBED_CONF_CELLULAR_PRESENT

/* Finally the dispatch from the JSON default interface type to the specific
 * subclasses. It's our job to configure - the default NetworkInterface is
 * preconfigured - the specific subtypes' defaults are not (necessarily).
 */
#define ETHERNET 1
#define WIFI 2
#define MESH 3
#define CELLULAR 4
#if MBED_CONF_TARGET_NETWORK_DEFAULT_INTERFACE_TYPE == ETHERNET
MBED_WEAK NetworkInterface *NetworkInterface::get_target_default_instance()
{
    return EthInterface::get_default_instance();
}
#elif MBED_CONF_TARGET_NETWORK_DEFAULT_INTERFACE_TYPE == WIFI
MBED_WEAK NetworkInterface *NetworkInterface::get_target_default_instance()
{
    /* WiFi is not expected to work unless we have configuration parameters.
     * We do not hook up to WifiInterface::get_default_instance() unless
     * we have at least an access point name.
     */
#ifdef MBED_CONF_NSAPI_DEFAULT_WIFI_SSID
    WiFiInterface *wifi = WiFiInterface::get_default_instance();
    if (!wifi) {
        return NULL;
    }
    wifi->set_default_parameters();
    return wifi;
#else
    return NULL;
#endif
}
#elif MBED_CONF_TARGET_NETWORK_DEFAULT_INTERFACE_TYPE == MESH
MBED_WEAK NetworkInterface *NetworkInterface::get_target_default_instance()
{
    return MeshInterface::get_default_instance();
}
#elif MBED_CONF_TARGET_NETWORK_DEFAULT_INTERFACE_TYPE == CELLULAR
#if MBED_CONF_CELLULAR_PRESENT
MBED_WEAK NetworkInterface *NetworkInterface::get_target_default_instance()
{
    CellularInterface *cellular = CellularInterface::get_default_instance();
    if (!cellular) {
        return NULL;
    }
    cellular->set_default_parameters();
    return cellular;
}
#endif // MBED_CONF_CELLULAR_PRESENT
#elif defined(MBED_CONF_TARGET_NETWORK_DEFAULT_INTERFACE_TYPE)
/* If anyone invents a new JSON value, they must have their own default weak
 * implementation.
 */
#else
/* When the default type is null */
MBED_WEAK NetworkInterface *NetworkInterface::get_target_default_instance()
{
    return NULL;
}
#endif
