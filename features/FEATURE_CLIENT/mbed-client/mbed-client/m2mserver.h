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
#ifndef M2M_SERVER_H
#define M2M_SERVER_H

#include "mbed-client/m2mobject.h"

// FORWARD DECLARATION
class M2MResource;

/**
 *  \brief M2MServer.
 *  This class represents an interface for the Server Object model of the LWM2M framework.
 *  It handles the server object and all its corresponding
 *  resources.
 */

class M2MServer : public M2MObject
{

friend class M2MInterfaceFactory;
friend class M2MNsdlInterface;

public:

    /**
     * \brief Enum defining all resources associated with
     * a Server Object in the LWM2M framework.
     */
    typedef enum {
        ShortServerID,
        Lifetime,
        DefaultMinPeriod,
        DefaultMaxPeriod,
        Disable,
        DisableTimeout,
        NotificationStorage,
        Binding,
        RegistrationUpdate
    }ServerResource;

private:

    /**
     * \brief Constructor
     */
    M2MServer();


    // Prevents the use of assignment operator.
    M2MServer& operator=( const M2MServer& /*other*/ );

    // Prevents the use of copy constructor
    M2MServer( const M2MServer& /*other*/ );

public:

    /**
     * \brief Destructor
     */
    virtual ~M2MServer();

    /**
     * \brief Creates a new resource for a given resource enum.
     * \param resource With this function, a value can be set to the following resources:
     *  'ShortServerID','Lifetime','DefaultMinPeriod','DefaultMaxPeriod','DisableTimeout',
     *  'NotificationStorage'.
     * \param value The value to be set on the resource, in Integer format.
     * \return M2MResource if created successfully, else NULL.
     */
    M2MResource* create_resource(ServerResource resource, uint32_t value);

    /**
     * \brief Creates a new resource for a given resource enum.
     * \param resource With this function, the following resources can be created:
     * 'Disable', 'RegistrationUpdate'
     * \return M2MResource if created successfully, else NULL.
     */
    M2MResource* create_resource(ServerResource resource);

    /**
     * \brief Deletes the resource with the given resource enum.
     * Mandatory resources cannot be deleted.
     * \param resource The name of the resource to be deleted.
     * \return True if deleted, else false.
     */
    bool delete_resource(ServerResource rescource);

    /**
     * \brief Sets the value of the given resource enum.
     * \param resource With this function, a value can be set on the following resources: 
     * 'Binding'.
     * \param value The value to be set on the resource, in String format.
     * \return True if successfully set, else false.
     */
    bool set_resource_value(ServerResource resource,
                            const String &value);

    /**
     * \brief Sets the value of the given resource enum.
     * \param resource With this function, a value can be set to the following resources:
     *  'ShortServerID','Lifetime','DefaultMinPeriod','DefaultMaxPeriod','DisableTimeout',
     *  'NotificationStorage'.
     * \param value The value to be set on the resource, in Integer format.
     * \return True if successfully set else false.
     */
    bool set_resource_value(ServerResource resource,
                            uint32_t value);
    /**
     * \brief Returns the value of the given resource enum, in String.
     * \param resource With this function, the following resources can return a value:
     * 'Binding'.
     * \return The value associated with the resource. If the resource is not valid an empty string is returned.
     */
    String resource_value_string(ServerResource resource) const;

    /**
     * \brief Returns the value of the given resource name, in Integer.
     * \param resource With this function, the following resources can return a value:
     *  'ShortServerID','Lifetime','DefaultMinPeriod','DefaultMaxPeriod','DisableTimeout',
     *  'NotificationStorage'
     * \return The value associated with the resource. If the resource is not valid -1 is returned.
     */
    uint32_t resource_value_int(ServerResource resource) const;

    /**
     * \brief Returns whether the resource instance with the given resource enum exists or not.
     * \param resource Resource enum.
     * \return True if at least one instance exists, else false.
     */
    bool is_resource_present(ServerResource resource)const;

    /**
     * \brief Returns the total number of resources for the server object.
     * \return The total number of resources.
     */
    uint16_t total_resource_count()const;

private:

    M2MResource* get_resource(ServerResource res) const;


private:

    M2MObjectInstance*    _server_instance;

    friend class Test_M2MServer;
    friend class Test_M2MNsdlInterface;
};

#endif // M2M_SERVER_H
