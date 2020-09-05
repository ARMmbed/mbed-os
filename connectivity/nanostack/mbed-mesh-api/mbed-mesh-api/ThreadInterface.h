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

#ifndef THREADINTERFACE_H
#define THREADINTERFACE_H

#include "MeshInterfaceNanostack.h"

/** Thread mesh network interface class
 *
 * Configure Nanostack to use Thread protocol.
 */
class ThreadInterface final : public MeshInterfaceNanostack {
public:

    /** Inherit MeshInterfaceNanostack constructors */
    using MeshInterfaceNanostack::MeshInterfaceNanostack;

    /**
     * \brief Sets the eui64 for the device configuration.
     * By default this value is read from the radio driver.
     * The value must be set before calling the connect function.
     * */
    void device_eui64_set(const uint8_t *eui64);

    /**
     * \brief Reads the eui64 from the device configuration.
     * By default this value is read from the radio driver, but it may have
     * been set by device_eui64_set().
     * */
    void device_eui64_get(uint8_t *eui64);

    /**
     * \brief sets the PSKd for the device configuration.
     * The default value is overwritten, which is defined in the mbed_lib.json file in the mesh-api
     * The value must be set before calling the connect function.
     * \return MESH_ERROR_NONE on success.
     * \return MESH_ERROR_PARAM in case of illegal parameters.
     * \return MESH_ERROR_MEMORY in case of memory error.
     * */

    mesh_error_t device_pskd_set(const char *pskd);

protected:
    Nanostack::ThreadInterface *get_interface() const;
    nsapi_error_t do_initialize() override;
};

#endif // THREADINTERFACE_H
