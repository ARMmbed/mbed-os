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

#ifndef __ABSTRACTMESH_H__
#define __ABSTRACTMESH_H__

#include "AbstractNetworkInterface.h"
#include "mbed.h"
#ifdef YOTTA_CFG
#include "core-util/FunctionPointer.h"

using namespace mbed;
using namespace mbed::util;
#endif

/**
 * \brief Abstract Mesh networking interface.
 * This class can't be instantiated directly.
 */
class AbstractMesh : public AbstractNetworkInterface
{

public:

    /**
     * Typedef for network callback
     */
#ifdef YOTTA_CFG
    typedef FunctionPointer1<void, mesh_connection_status_t> mesh_network_handler_t;
#else
    typedef FunctionPointerArg1<void, mesh_connection_status_t> mesh_network_handler_t;
#endif

    /**
     * Constructor
     * \param type mesh network type
     */
    AbstractMesh(mesh_network_type_t type);

    // Destructor, force derived classes to implement own destructors
    // and prevent class creation.
    virtual ~AbstractMesh() = 0;

    /**
     * \brief Initialization of the interface.
     * \param registered device is physical device registered
     * \param callbackHandler is callback that is called when network state changes
     * \return MESH_ERROR_NONE on success.
     * \return MESH_ERROR_PARAM when input parameters are illegal (also in case when RF device is already associated to other interface)
     * \return MESH_ERROR_MEMORY in case of memory error
     * \return MESH_ERROR_UNKNOWN in other error cases
     */
    virtual mesh_error_t init(int8_t registered_device_id, mesh_network_handler_t callbackHandler);

    /**
     * \brief Connect interface to the mesh network
     * \return MESH_ERROR_NONE on success.
     * \return MESH_ERROR_PARAM in case of illegal parameters.
     * \return MESH_ERROR_MEMORY in case of memory error.
     * \return MESH_ERROR_STATE if interface is already connected to network.
     * \return MESH_ERROR_UNKNOWN in case of unspecified error.
     * */
    virtual mesh_error_t connect();

    /**
     * \brief Disconnect interface from the mesh network
     * \return MESH_ERROR_NONE on success.
     * \return MESH_ERROR_UNKNOWN in case of error.
     * */
    virtual mesh_error_t disconnect();

    /**
     * \brief Callback from C-layer
     * \param state state of the network
     * */
    void callback(mesh_connection_status_t state);

protected:

    /*
     * Mesh callback function
     */
    mesh_network_handler_t _mesh_network_handler;

    /*
     * Network interface ID
     */
    int8_t _network_interface_id;

    /*
     * Registered device ID
     */
    int8_t _device_id;

    /*
     * Mesh network type
     */
    mesh_network_type_t _mesh_network_type;
};

#endif /* __ABSTRACTMESH_H__ */
