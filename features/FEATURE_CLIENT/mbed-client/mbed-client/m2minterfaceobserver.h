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
#ifndef M2M_INTERFACE_OBSERVER_H
#define M2M_INTERFACE_OBSERVER_H

#include "mbed-client/m2mbase.h"
#include "mbed-client/m2minterface.h"

//FORWARD DECLARATION
class M2MServer;

/**
 * \brief M2MInterfaceObserver
 * This is an observer class that updates the calling application about
 * various events associated with various Interface operations.
 * Also, it informs about various errors that can occur during any of the above
 * operations.
 */
class M2MInterfaceObserver {

public:

    /**
     * \brief A callback indicating that the bootstap has been performed successfully.
     * \param server_object The server object that contains information fetched
     * about the LWM2M server from the bootstrap server. This object can be used
     * to register to the LWM2M server. The object ownership is passed.
     */
    virtual void bootstrap_done(M2MSecurity *server_object) = 0;

    /**
     * \brief A callback indicating that the device object has been registered
     * successfully to the LWM2M server.
     * \param security_object The server object on which the device object is
     * registered. The object ownership is passed.
     * \param server_object An object containing information about the LWM2M server.
     * The client maintains the object.
     */
    virtual void object_registered(M2MSecurity *security_object, const M2MServer &server_object) = 0;

    /**
     * \brief A callback indicating that the device object has been successfully unregistered
     * from the LWM2M server.
     * \param server_object The server object from which the device object is
     * unregistered. The object ownership is passed.
     */
    virtual void object_unregistered(M2MSecurity *server_object) = 0;

    /**
     * \brief A callback indicating that the device object registration has been successfully
     * updated on the LWM2M server.
     * \param security_object The server object on which the device object registration
     * updated. The object ownership is passed.
     * \param server_object An object containing information about the LWM2M server.
     * The client maintains the object.
     */
    virtual void registration_updated(M2MSecurity *security_object, const M2MServer & server_object) = 0;

    /**
     * \brief A callback indicating that there was an error during the operation.
     * \param error An error code for the occurred error.
     */
    virtual void error(M2MInterface::Error error) = 0;

    /**
     * \brief A callback indicating that the value of the resource object is updated by the server.
     * \param base The object whose value is updated.
     * \param type The type of the object.
     */
    virtual void value_updated(M2MBase *base, M2MBase::BaseType type) = 0;
};

#endif // M2M_INTERFACE_OBSERVER_H
