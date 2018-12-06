/*
 * Copyright (c) 2018 ARM Limited. All rights reserved.
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

    bool getRouterIpAddress(char *address, int8_t len);
protected:
    Nanostack::WisunInterface *get_interface() const;
    virtual nsapi_error_t do_initialize();
};

#endif
