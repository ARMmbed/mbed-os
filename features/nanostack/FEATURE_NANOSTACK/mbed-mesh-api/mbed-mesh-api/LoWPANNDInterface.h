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

#ifndef LOWPANNDINTERFACE_H
#define LOWPANNDINTERFACE_H

#include "MeshInterfaceNanostack.h"

class LoWPANNDInterface : public MeshInterfaceNanostack {
public:

    /** Create an uninitialized LoWPANNDInterface
     *
     *  Must initialize to initialize the mesh on a phy.
     */
    LoWPANNDInterface() : MeshInterfaceNanostack() { }

    /** Create an initialized MeshInterface
     *
     */
    LoWPANNDInterface(NanostackRfPhy *phy) : MeshInterfaceNanostack(phy) { }

    nsapi_error_t initialize(NanostackRfPhy *phy);
    int connect();
    int disconnect();
    bool getOwnIpAddress(char *address, int8_t len);
    bool getRouterIpAddress(char *address, int8_t len);
private:
    mesh_error_t init();
    mesh_error_t mesh_connect();
    mesh_error_t mesh_disconnect();
};

#endif
