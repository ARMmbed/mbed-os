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
#ifndef M2M_INTERFACE_FACTORY_H
#define M2M_INTERFACE_FACTORY_H

#include <stdlib.h>
#include "mbed-client/m2msecurity.h"
#include "mbed-client/m2mresource.h"
#include "mbed-client/m2minterfaceobserver.h"

//FORWARD DECLARATION
class M2MDevice;
class M2MServer;
class M2MInterfaceImpl;
class M2MFirmware;

/**
 *  \brief M2MInterfaceFactory.
 *  This is a factory class that provides an interface for creating an mbed Client Interface
 *  object for an application to utilize the LWM2M features provided by the client.
 */

class  M2MInterfaceFactory {
private:
    // Prevents the use of assignment operator by accident.
    M2MInterfaceFactory& operator=( const M2MInterfaceFactory& /*other*/ );

    // Prevents the use of copy constructor by accident
    M2MInterfaceFactory( const M2MInterfaceFactory& /*other*/ );

public:

    /**
     * \brief Creates an interface object for the mbed Client Inteface. With this, the
     * client can handle client operations like Bootstrapping, Client
     * Registration, Device Management and Information Reporting.
     * \param endpoint_name The endpoint name of the mbed client.
     * \param endpoint_type The endpoint type of the mbed client, default is empty.
     * \param life_time The lifetime of the endpoint in seconds,
     *        if -1 it is optional.
     * \param listen_port The listening port for the endpoint, default is 5683.
     * \param domain The domain of the endpoint, default is empty.
     * \param mode The binding mode of the endpoint, default is NOT_SET.
     * \param stack The underlying network stack to be used for the connection,
     * default is LwIP_IPv4.
     * \param context_address The context address for M2M-HTTP, not used currently.
     * \return M2MInterfaceImpl An object for managing other client operations.
     */
    static M2MInterface *create_interface(M2MInterfaceObserver &observer,
                                              const String &endpoint_name,
                                              const String &endpoint_type = "",
                                              const int32_t life_time = -1,
                                              const uint16_t listen_port = 5683,
                                              const String &domain = "",
                                              M2MInterface::BindingMode mode = M2MInterface::NOT_SET,
                                              M2MInterface::NetworkStack stack = M2MInterface::LwIP_IPv4,
                                              const String &context_address = "");

    /**
     * \brief Creates a security object for the mbed Client Inteface. With this, the
     * client can manage Bootstrapping and Client Registration.
     * \param ServerType The type of the Security Object, bootstrap or LWM2M server.
     * \return M2MSecurity An object for managing other client operations.
     */
    static M2MSecurity *create_security(M2MSecurity::ServerType server_type);

    /**
     * \brief Creates a server object for the mbed Client Inteface. With this, the 
     * client can manage the server resources used for client operations
     * such as Client Registration, server lifetime etc.
     * \return M2MServer An object for managing server client operations.
     */
    static M2MServer *create_server();

    /**
     * \brief Creates a device object for the mbed Client Inteface. With this, the
     * client can manage the device resources used for client operations
     * such as Client Registration, Device Management and Information Reporting.
     * \param name The name of the device object.
     * \return M2MDevice An object for managing other client operations.
     */
    static M2MDevice *create_device();

    /**
     * \brief Creates a firmware object for the mbed Client Inteface. With this, the
     * client can manage the firmware resources used for the client operations
     * such as Client Registration, Device Management and Information Reporting.
     * \return M2MFirmware An object for managing other client operations.
     */
    static M2MFirmware *create_firmware();

    /**
     * \brief Creates a generic object for the mbed Client Inteface. With this, the
     * client can manage its own customized resources used for registering
     * Device Management and Information Reporting for those resources.
     * \param name The name of the object.
     * \param id A unique ID for the object. It should be other than the reserved
     * LWM2M object IDs.
     * \return M2MObject An object for managing other mbed Client operations.
     */
    static M2MObject *create_object(const String &name);


    friend class Test_M2MInterfaceFactory;
};

#endif // M2M_INTERFACE_FACTORY_H
