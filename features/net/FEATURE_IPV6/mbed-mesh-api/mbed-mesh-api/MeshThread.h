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

#ifndef __MESHTHREAD_H__
#define __MESHTHREAD_H__

#include "AbstractMesh.h"

/**
 * \brief 6LoWPAN Thread mesh networking class.
 */

class MeshThread : public AbstractMesh
{

public:

    virtual ~MeshThread();

    /*
     * \brief Initialization of the interface.
     * \param registered device is physical device registered
     * \param callbackHandler is callback that is called when network state changes
     * \param eui64 pointer to MAC address (8 bytes) of the registered RF driver
     * \param pskd Not used
     * \return MESH_ERROR_NONE on success.
     * \return MESH_ERROR_PARAM when input parameters are illegal (also in case when RF device is already associated to other interface)
     * \return MESH_ERROR_MEMORY in case of memory error
     * \return MESH_ERROR_UNKNOWN in other error cases
     */
    mesh_error_t init(int8_t registered_device_id, mesh_network_handler_t callbackHandler, uint8_t *eui64, char *pskd);

    /**
     * \brief Overriding initialization of the base class.
     * Use method init(int8_t, MeshNetworkHandler_t, uint8_t*, char*)
     */
    virtual mesh_error_t init(int8_t registered_device_id, mesh_network_handler_t callbackHandler);

    bool getOwnIpAddress(char *address, int8_t len);

    /**
     * \brief Set device data polling rate to value given in parameter pollrate. Data polling will be
     * disabled if pollrate value is 0.
     * \param pollrate data polling rate in seconds. Setting this to 0 disables data polling.
     * \return MESH_ERROR_NONE on success.
     * \return MESH_ERROR_PARAM in case timeout value is bigger than 864001.
     * \return MESH_ERROR_UNKNOWN in case of other error.
     * */
    virtual mesh_error_t data_poll_rate_set(uint32_t pollrate);

    friend class MeshInterfaceFactory;

private:
    /*
     * \brief private constructor for the MeshThread
     */
    MeshThread();

    /*
     * avoid copy/assign object
     */
    MeshThread(MeshThread const &copy);
    MeshThread &operator=(MeshThread const &copy);
};

#endif /* __MESHTHREAD_H__ */
