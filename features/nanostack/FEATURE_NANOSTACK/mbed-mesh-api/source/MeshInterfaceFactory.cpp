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
#include "mbed.h"
#include "mbed-mesh-api/Mesh6LoWPAN_ND.h"
#include "mbed-mesh-api/MeshThread.h"
#include "include/callback_handler.h"
#include "mbed-mesh-api/MeshInterfaceFactory.h"

AbstractMesh *MeshInterfaceFactory::createInterface(const mesh_network_type_t &type)
{
    AbstractMesh *meshInstance = NULL;

    // instance can be created only if there is no existing instance.
    if (__abstract_mesh_interface == NULL) {
        if (type == MESH_TYPE_6LOWPAN_ND) {
            meshInstance = (AbstractMesh *)new Mesh6LoWPAN_ND;
        }

        if (type == MESH_TYPE_THREAD) {
            meshInstance = (AbstractMesh *)new MeshThread;
        }
    }

    return meshInstance;
}
