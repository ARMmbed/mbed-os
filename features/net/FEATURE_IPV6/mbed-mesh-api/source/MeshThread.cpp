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
 * Thread network interface.
 * Class provides methods for 6LoWPAN (Thread mode) network handling.
 */

#include <stdio.h>
#include "mbed.h"
#include "mbed-mesh-api/MeshThread.h"
#include "include/thread_tasklet.h"
#include "include/callback_handler.h"

#define HAVE_DEBUG 1
#include "ns_trace.h"

#define TRACE_GROUP  "m6LTh"

MeshThread::MeshThread() : AbstractMesh(MESH_TYPE_THREAD)
{
}

MeshThread::~MeshThread()
{
    tr_debug("~MeshThread()");
    thread_tasklet_disconnect(false);
}

mesh_error_t MeshThread::init(int8_t registered_device_id, mesh_network_handler_t callbackHandler, uint8_t *eui64, char *pskd)
{

    if (eui64 == NULL) {
        return MESH_ERROR_PARAM;
    }

    mesh_error_t status = AbstractMesh::init(registered_device_id, callbackHandler);

    if (status == MESH_ERROR_NONE) {
        thread_tasklet_device_config_set(eui64, pskd);
    }

    return status;
}

mesh_error_t MeshThread::init(int8_t registered_device_id, mesh_network_handler_t callbackHandler)
{
    // TODO: Use test values for device configuration
    return MeshThread::init(registered_device_id, callbackHandler, NULL, NULL);
}

bool MeshThread::getOwnIpAddress(char *address, int8_t len)
{
    tr_debug("getOwnIpAddress()");
    if (thread_tasklet_get_ip_address(address, len) == 0) {
        return true;
    }
    return false;
}

mesh_error_t MeshThread::data_poll_rate_set(uint32_t pollrate)
{
    mesh_error_t status = MESH_ERROR_NONE;
    int8_t retval = thread_tasklet_data_poll_rate_set(pollrate);
    switch (retval) {
        case 0:
            status = MESH_ERROR_NONE;
            break;
        case -2:
            status = MESH_ERROR_PARAM;
            break;
        default:
        //case -1:
            status = MESH_ERROR_UNKNOWN;
            break;
    }

    return status;
}

