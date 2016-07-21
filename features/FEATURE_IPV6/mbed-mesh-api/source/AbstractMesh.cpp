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

/*
 * Mesh networking interface.
 */

#include "mbed-mesh-api/AbstractMesh.h"
#include "mbed-mesh-api/MeshThread.h"
#include "mbed-mesh-api/Mesh6LoWPAN_ND.h"
#include "include/callback_handler.h"
#include "include/mesh_system.h"
#include "include/nd_tasklet.h"
#include "include/thread_tasklet.h"

#define HAVE_DEBUG 1
#include "ns_trace.h"

#define TRACE_GROUP  "m6La"

AbstractMesh::AbstractMesh(mesh_network_type_t type) :
    _mesh_network_handler(NULL), _network_interface_id(-1), _device_id(-1)
{
    __abstract_mesh_interface = this;
    _mesh_network_type = type;
    // initialize mesh networking resources, memory, timers, etc...
    mesh_system_init();
}

AbstractMesh::~AbstractMesh()
{
    tr_debug("~AbstractMesh()");
    __abstract_mesh_interface = NULL;
}

mesh_error_t AbstractMesh::init(int8_t registered_device_id, mesh_network_handler_t callbackHandler)
{
    tr_debug("init()");

    if (callbackHandler == (mesh_network_handler_t)NULL) {
        return MESH_ERROR_PARAM;
    }

    _device_id = registered_device_id;
    _mesh_network_handler = callbackHandler;

    // Create network interface
    if (_mesh_network_type == MESH_TYPE_THREAD) {
        thread_tasklet_init();
        _network_interface_id = thread_tasklet_network_init(_device_id);
    } else if (_mesh_network_type == MESH_TYPE_6LOWPAN_ND) {
        nd_tasklet_init();
        _network_interface_id = nd_tasklet_network_init(_device_id);
    }

    if (_network_interface_id >= 0) {
        return MESH_ERROR_NONE;
    } else if (_network_interface_id == -2) {
        return MESH_ERROR_PARAM;
    } else if (_network_interface_id == -3) {
        return MESH_ERROR_MEMORY;
    }

    return MESH_ERROR_UNKNOWN;
}

mesh_error_t AbstractMesh::connect()
{
    int8_t status = -9; // init to unknown error
    tr_debug("connect()");

    if (_mesh_network_handler == (mesh_network_handler_t)NULL) {
        // initialization hasn't been made and connect gets called
        return MESH_ERROR_PARAM;
    }

    if (_mesh_network_type == MESH_TYPE_THREAD) {
        status = thread_tasklet_connect(&__mesh_handler_c_callback, _network_interface_id);
    } else if (_mesh_network_type == MESH_TYPE_6LOWPAN_ND) {
        status = nd_tasklet_connect(&__mesh_handler_c_callback, _network_interface_id);
    }

    if (status >= 0) {
        return MESH_ERROR_NONE;
    } else if (status == -1) {
        return MESH_ERROR_PARAM;
    } else if (status == -2) {
        return MESH_ERROR_MEMORY;
    } else if (status == -3) {
        return MESH_ERROR_STATE;
    } else {
        return MESH_ERROR_UNKNOWN;
    }
}

/*
 * Disable optimization as gcc compiler fails to return correct enum value.
 */
#if defined(__GNUC__) && !defined(__ARMCC_VERSION)
#define DISABLE_GCC_OPT __attribute__((optimize("O0")))
#else
#define DISABLE_GCC_OPT
#endif

mesh_error_t DISABLE_GCC_OPT AbstractMesh::disconnect()
{
    int8_t status = -1;

    if (_mesh_network_type == MESH_TYPE_THREAD) {
        status = thread_tasklet_disconnect(true);
    } else if (_mesh_network_type == MESH_TYPE_6LOWPAN_ND) {
        status = nd_tasklet_disconnect(true);
    }

    if (status >= 0) {
        return MESH_ERROR_NONE;
    }

    return MESH_ERROR_UNKNOWN;
}

void AbstractMesh::callback(mesh_connection_status_t state)
{
    if (_mesh_network_handler) {
        _mesh_network_handler(state);
    }
}
