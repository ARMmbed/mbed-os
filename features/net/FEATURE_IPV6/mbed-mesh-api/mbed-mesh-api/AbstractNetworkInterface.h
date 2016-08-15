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

#ifndef __ABSTRACTNETWORKINTERFACE_H__
#define __ABSTRACTNETWORKINTERFACE_H__

#include <stdint.h>
#include "mesh_interface_types.h"

/**
 * \brief Abstract base class for network interfaces.
 * This is a pure virtual class; it can't instantiated directly, but
 * it provides common functionality for derived classes.
 *
 * Note! This class will be replaced by connection manager when it is available.
 */

class AbstractNetworkInterface
{

public:
    /**
     * Virtual destructor.
     */
    virtual ~AbstractNetworkInterface();

    /**
     * Connect the interface to the network.
     */
    virtual mesh_error_t connect() = 0;

    /**
     * Disconnect the interface from the network.
     */
    virtual mesh_error_t disconnect() = 0;

    /**
     * \brief Read own global IP address
     *
     * \param address is where the IP address will be copied
     * \param len is the length of the address buffer, must be at least 40 bytes
     * \return true if address is read successfully, false otherwise
     */
    virtual bool getOwnIpAddress(char *address, int8_t len) = 0;
};

#endif /* __ABSTRACTNETWORKINTERFACE_H__ */
