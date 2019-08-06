/*
 * Copyright (c) 2018-2019 ARM Limited. All rights reserved.
 * SPDX-License-Identifier: Apache-2.0
 * Licensed under the Apache License, Version 2.0 (the License); you may
 * not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an AS IS BASIS, WITHOUT
 * WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef WISUNINTERFACE_H
#define WISUNINTERFACE_H

#include "MeshInterfaceNanostack.h"

/** Wi-SUN mesh network interface class
 *
 * Configure Nanostack to use Wi-SUN protocol.
 */
class WisunInterface : public MeshInterfaceNanostack {
public:

    /** Create an uninitialized WisunInterface
     *
     *  Must initialize to initialize the mesh on a phy.
     */
    WisunInterface() { }

    /** Create an initialized WisunInterface
     *
     */
    WisunInterface(NanostackRfPhy *phy) : MeshInterfaceNanostack(phy) { }

    /**
     * \brief Set Wi-SUN network name.
     *
     * Function stores new network name to mbed-mesh-api and uses it when connect() is called next time.
     * If device is already connected to the Wi-SUN network then device will restart network discovery after
     * changing the network name.
     *
     * Function overwrites network name defined by Mbed OS configuration.
     *
     * \param network_name Network name as NUL terminated string. Can't exceed 32 characters and can't be NULL.
     * \return MESH_ERROR_NONE on success.
     * \return MESH_ERROR_UNKNOWN in case of failure.
     * */
    mesh_error_t network_name_set(char *network_name);

    /**
     * \brief Set Wi-SUN network regulatory domain, operating class and operating mode.
     *
     * Function stores new parameters to mbed-mesh-api and uses them when connect() is called next time.
     * If device is already connected to the Wi-SUN network then device will restart network discovery after
     * changing the regulatory_domain, operating_class or operating_mode.
     *
     * Function overwrites parameters defined by Mbed OS configuration.
     *
     * \param regulatory_domain Values defined in Wi-SUN PHY-specification
     * \param operating_class Values defined in Wi-SUN PHY-specification
     * \param operating_mode Values defined in Wi-SUN PHY-specification
     * \return MESH_ERROR_NONE on success.
     * \return MESH_ERROR_UNKNOWN in case of failure.
     * */
    mesh_error_t network_regulatory_domain_set(uint8_t regulatory_domain = 0xff, uint8_t operating_class = 0xff, uint8_t operating_mode = 0xff);

    /**
     * \brief Get router IP address
     *
     * \param address
     * \param len
     * */
    bool getRouterIpAddress(char *address, int8_t len);
protected:
    Nanostack::WisunInterface *get_interface() const;
    virtual nsapi_error_t do_initialize();
};

#endif
