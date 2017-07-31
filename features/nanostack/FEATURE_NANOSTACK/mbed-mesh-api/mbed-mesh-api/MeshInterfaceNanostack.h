/*
 * Copyright (c) 2016 ARM Limited. All rights reserved.
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

#ifndef MESHINTERFACENANOSTACK_H
#define MESHINTERFACENANOSTACK_H
#include "mbed.h"

#include "MeshInterface.h"
#include "NanostackRfPhy.h"
#include "mesh_interface_types.h"

class MeshInterfaceNanostack : public MeshInterface {
public:

    /** Attach phy and initialize the mesh
     *
     *  Initializes a mesh interface on the given phy. Not needed if
     *  the phy is passed to the mesh's constructor.
     *
     *  @return     0 on success, negative on failure
     */
    nsapi_error_t initialize(NanostackPhy *phy);

    /** Start the interface
     *
     *  @return     0 on success, negative on failure
     */
    virtual nsapi_error_t connect() = 0;

    /** Stop the interface
     *
     *  @return     0 on success, negative on failure
     */
    virtual nsapi_error_t disconnect() = 0;

    /** Get the internally stored IP address
    /return     IP address of the interface or null if not yet connected
    */
    virtual const char *get_ip_address();

    /** Get the internally stored MAC address
    /return     MAC address of the interface
    */
    virtual const char *get_mac_address();

    /**
     * \brief Callback from C-layer
     * \param state state of the network
     * */
    void mesh_network_handler(mesh_connection_status_t status);

protected:
    MeshInterfaceNanostack();
    MeshInterfaceNanostack(NanostackPhy *phy);
    nsapi_error_t register_phy();
    virtual NetworkStack * get_stack(void);

    /**
     * \brief Read own global IP address
     *
     * \param address is where the IP address will be copied
     * \param len is the length of the address buffer, must be at least 40 bytes
     * \return true if address is read successfully, false otherwise
     */
    virtual bool getOwnIpAddress(char *address, int8_t len) = 0;

    NanostackPhy *phy;
    /** Network interface ID */
    int8_t _network_interface_id;
    /** Registered device ID */
    int8_t _device_id;
    uint8_t eui64[8];
    char ip_addr_str[40];
    char mac_addr_str[24];
    Semaphore connect_semaphore;
};

#endif /* MESHINTERFACENANOSTACK_H */
