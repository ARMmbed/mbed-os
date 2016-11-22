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

#ifndef NANOSTACKETHERNETINTERFACE_H
#define NANOSTACKETHERNETINTERFACE_H

#include "MeshInterfaceNanostack.h"
#include "NanostackEthernetPhy.h"

class NanostackEthernetInterface : public MeshInterfaceNanostack {
public:

    NanostackEthernetInterface() : MeshInterfaceNanostack() { }
    NanostackEthernetInterface(NanostackEthernetPhy *phy) : MeshInterfaceNanostack(phy) { }

    nsapi_error_t initialize(NanostackEthernetPhy *phy);
    int connect();
    int disconnect();
    bool getOwnIpAddress(char *address, int8_t len);
    bool getRouterIpAddress(char *address, int8_t len);
};

#endif // NANOSTACKETHERNETINTERFACE_H
