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

#ifndef __MESH6LOWPAN_ND_H__
#define __MESH6LOWPAN_ND_H__

#include "AbstractMesh.h"

/**
 * \brief 6LoWPAN ND mesh networking class.
 */

class Mesh6LoWPAN_ND : public AbstractMesh
{

public:

    virtual ~Mesh6LoWPAN_ND();

    /**
     * \brief Read own global IP address
     *
     * \param address is where the IP address will be copied
     * \param len is the length of the address buffer, must be at least 40 bytes
     * \return true if address is read successfully, false otherwise
     */
    bool getOwnIpAddress(char *address, int8_t len);

    /**
     * \brief Read border router IP address
     *
     * \param address is where the IP address will be copied
     * \param len is the length of the address buffer, must be at least 40 bytes
     * \return true if address is read successfully, false otherwise
     */
    bool getRouterIpAddress(char *address, int8_t len);

    friend class MeshInterfaceFactory;

private:
    /*
     * \brief private constructor for the 6LoWPAN_ND
     */
    Mesh6LoWPAN_ND();

    /*
     * avoid copy/assign object
     */
    Mesh6LoWPAN_ND(Mesh6LoWPAN_ND const &copy);
    Mesh6LoWPAN_ND &operator=(Mesh6LoWPAN_ND const &copy);
};

#endif /* __MESH6LOWPAN_ND_H__ */
