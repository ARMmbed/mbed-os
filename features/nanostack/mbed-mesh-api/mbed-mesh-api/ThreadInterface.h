/*
 * Copyright (c) 2015 ARM Limited. All rights reserved.
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

#ifndef THREADINTERFACE_H
#define THREADINTERFACE_H

#include "MeshInterfaceNanostack.h"

/** Thread mesh network interface class
 *
 * Configure Nanostack to use Thread protocol.
 */
class ThreadInterface : public MeshInterfaceNanostack {
public:

    /** Create an uninitialized LoWPANNDInterface
     *
     *  Must initialize to initialize the mesh on a phy.
     */
    ThreadInterface() : MeshInterfaceNanostack() { }

    /** Create an initialized MeshInterface
     *
     */
    ThreadInterface(NanostackRfPhy *phy) : MeshInterfaceNanostack(phy) { }

    nsapi_error_t initialize(NanostackRfPhy *phy);

    /**
     * \brief Sets the eui64 for the device configuration.
     * By default this value is read from the radio driver.
     * The value must be set before calling the connect function.
     * */
    void device_eui64_set(const uint8_t *eui64);

    /**
     * \brief sets the PSKd for the device configuration.
     * The default value is overwritten, which is defined in the mbed_lib.json file in the mesh-api
     * The value must be set before calling the connect function.
     * \return MESH_ERROR_NONE on success.
     * \return MESH_ERROR_PARAM in case of illegal parameters.
     * \return MESH_ERROR_MEMORY in case of memory error.
     * */

    mesh_error_t device_pskd_set(const char *pskd);

    virtual int connect();
    virtual int disconnect();
private:
    /*
     * \brief Initialization of the interface.
     * \return MESH_ERROR_NONE on success.
     * \return MESH_ERROR_PARAM when input parameters are illegal (also in case when RF device is already associated to other interface)
     * \return MESH_ERROR_MEMORY in case of memory error
     * \return MESH_ERROR_UNKNOWN in other error cases
     */
    mesh_error_t init();
    /**
     * \brief Connect interface to the mesh network
     * \return MESH_ERROR_NONE on success.
     * \return MESH_ERROR_PARAM in case of illegal parameters.
     * \return MESH_ERROR_MEMORY in case of memory error.
     * \return MESH_ERROR_STATE if interface is already connected to network.
     * \return MESH_ERROR_UNKNOWN in case of unspecified error.
     * */
    mesh_error_t mesh_connect();

    /**
     * \brief Disconnect interface from the mesh network
     * \return MESH_ERROR_NONE on success.
     * \return MESH_ERROR_UNKNOWN in case of error.
     * */
    mesh_error_t mesh_disconnect();

    /**
     * \brief Read own global IP address
     *
     * \param address is where the IP address will be copied
     * \param len is the length of the address buffer, must be at least 40 bytes
     * \return true if address is read successfully, false otherwise
     */
    virtual bool getOwnIpAddress(char *address, int8_t len);
};

#endif // THREADINTERFACE_H
