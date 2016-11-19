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

#ifndef __MESHINTERFACEFACTORY_H__
#define __MESHINTERFACEFACTORY_H__

#include <stdlib.h>
#include "AbstractMesh.h"

/**
 *  \brief MeshInterfaceFactory.
 *  This is a factory class provides interface for creating Mesh Interface
 *  object for application to utilize mesh networking.
 */

class MeshInterfaceFactory
{

public:

    /**
     * \brief Creates Mesh interface object that can be used for connecting
     * to mesh network.
     * \param type, type of the mesh network.
     * \return M2MInterfaceImpl , Object to manage other LWM2M operations.
     */
    static AbstractMesh *createInterface(const mesh_network_type_t &type);

private:
    // Prevents the use of assignment operator by accident.
    MeshInterfaceFactory &operator=(const MeshInterfaceFactory & /*other*/);

    // Prevents the use of copy constructor by accident
    MeshInterfaceFactory(const MeshInterfaceFactory & /*other*/);
};

#endif /* __MESHINTERFACEFACTORY_H__ */
