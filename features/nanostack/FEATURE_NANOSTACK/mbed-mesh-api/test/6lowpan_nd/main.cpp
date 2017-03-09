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

#include <stdio.h>
#include "mbed-drivers/test_env.h"
#include "atmel-rf-driver/driverRFPhy.h"    // rf_device_register
#include "mbed-mesh-api/MeshInterfaceFactory.h"
#include "mbed-mesh-api/Mesh6LoWPAN_ND.h"
#include "mbed-mesh-api/MeshThread.h"
// For tracing we need to define flag, have include and define group
#define HAVE_DEBUG 1
#include "ns_trace.h"
#define TRACE_GROUP  "mesh_appl"

// Bootstrap mode, if undefined then 6LoWPAN-ND mode is used
#define BOOTSTRAP_MODE_THREAD

// mesh network state
static mesh_connection_status_t mesh_network_state = MESH_DISCONNECTED;

static AbstractMesh *meshApi;

// mesh network callback, called when network state changes
void mesh_network_callback(mesh_connection_status_t mesh_state)
{
    tr_info("mesh_network_callback() %d", mesh_state);
    mesh_network_state = mesh_state;
    if (mesh_network_state == MESH_CONNECTED) {
        tr_info("Connected to mesh network!");
        // Once connected, disconnect from network
        meshApi->disconnect();
    } else if (mesh_network_state == MESH_DISCONNECTED) {
        tr_info("Disconnected from mesh network!");
        delete meshApi;
        tr_info("Mesh networking done!");
    } else {
        tr_error("Networking error!");
    }
}

void app_start(int, char **)
{
    mesh_error_t status;

    // set tracing baud rate
    static Serial &pc = get_stdio_serial();
    pc.baud(115200);

    // create 6LoWPAN_ND interface
#ifdef BOOTSTRAP_MODE_THREAD
    meshApi = MeshInterfaceFactory::createInterface(MESH_TYPE_THREAD);
    uint8_t eui64[8];
    int8_t rf_device = rf_device_register();
    rf_read_mac_address(eui64);
    char *pskd = (char *)"Secret password";
    // Use tr_info traces and RF interface after MeshAPi has been created.
    // as interface initializes mesh system that RF device is using
    tr_info("Mesh API test application - Thread mode");
    // initialize the interface with registered device and callback
    status = ((MeshThread *)meshApi)->init(rf_device, mesh_network_callback, eui64, pskd);
#else
    meshApi = (Mesh6LoWPAN_ND *)MeshInterfaceFactory::createInterface(MESH_TYPE_6LOWPAN_ND);
    // Use tr_info traces and RF interface after MeshAPi has been created.
    // as interface initializes mesh system that RF device is using
    tr_info("Mesh API test application - 6LoWPAN-ND mode");
    // initialize the interface with registered device and callback
    status = ((Mesh6LoWPAN_ND *)meshApi)->init(rf_device_register(), mesh_network_callback);
#endif

    // Set ns_trace configuration level
    // set_trace_config(TRACE_ACTIVE_LEVEL_INFO|TRACE_MODE_COLOR|TRACE_CARRIAGE_RETURN);

    if (status != MESH_ERROR_NONE) {
        tr_error("Can't initialize mesh network");
        return;
    }

    // connect interface to the network
    status = meshApi->connect();
    if (status != MESH_ERROR_NONE) {
        tr_error("Can't connect to Mesh network!");
        return;
    }
}
