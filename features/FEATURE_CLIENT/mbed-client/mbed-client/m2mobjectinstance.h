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
#ifndef M2M_OBJECT_INSTANCE_H
#define M2M_OBJECT_INSTANCE_H

#include "mbed-client/m2mvector.h"
#include "mbed-client/m2mresource.h"

//FORWARD DECLARATION
typedef Vector<M2MResource *> M2MResourceList;
typedef Vector<M2MResourceInstance *> M2MResourceInstanceList;

class M2MObjectCallback {
public:
    virtual void notification_update(uint16_t obj_instance_id) = 0;
};

/**
 *  \brief M2MObjectInstance.
 *  This class is the instance class for mbed Client Objects. All defined
 *  LWM2M object models can be created based on it. This class also holds all resource
 *  instances associated with the given object.
 */

class M2MObjectInstance : public M2MBase,
                          public M2MObjectInstanceCallback
{

friend class M2MObject;

private: // Constructor and destructor are private which means
         // that these objects can be created or
         // deleted only through function provided by M2MObject.

    /**
     * \brief Constructor
     * \param name Name of the object
     */
    M2MObjectInstance(const String &object_name,
                      M2MObjectCallback &object_callback);


    // Prevents the use of default constructor.
    M2MObjectInstance();

    // Prevents the use of assignment operator.
    M2MObjectInstance& operator=( const M2MObjectInstance& /*other*/ );

    // Prevents the use of copy constructor.
    M2MObjectInstance( const M2MObjectInstance& /*other*/ );

    /**
     * Destructor
     */
    virtual ~M2MObjectInstance();

public:

    /**
     * \brief Creates a static resource for a given mbed Client Inteface object. With this, the
     * client can respond to server's GET methods with the provided value.
     * \param resource_name The name of the resource.
     * \param resource_type The type of the resource.
     * \param value A pointer to the value of the resource.
     * \param value_length The length of the value in the pointer.
     * \param multiple_instance A resource can have
     *        multiple instances, default is false.
     * \return M2MResource The resource for managing other client operations.
     */
    M2MResource* create_static_resource(const String &resource_name,
                                        const String &resource_type,
                                        M2MResourceInstance::ResourceType type,
                                        const uint8_t *value,
                                        const uint8_t value_length,
                                        bool multiple_instance = false);

    /**
     * \brief Creates a dynamic resource for a given mbed Client Inteface object. With this,
     * the client can respond to different queries from the server (GET,PUT etc).
     * This type of resource is also observable and carries callbacks.
     * \param resource_name The name of the resource.
     * \param resource_type The type of the resource.
     * \param observable Indicates whether the resource is observable or not.
     * \param multiple_instance The resource can have
     *        multiple instances, default is false.
     * \return M2MResource The resource for managing other client operations.
     */
    M2MResource* create_dynamic_resource(const String &resource_name,
                                         const String &resource_type,
                                         M2MResourceInstance::ResourceType type,
                                         bool observable,
                                         bool multiple_instance = false);


    /**
     * \brief Creates a static resource instance for a given mbed Client Inteface object. With this,
     * the client can respond to server's GET methods with the provided value.
     * \param resource_name The name of the resource.
     * \param resource_type The type of the resource.
     * \param value A pointer to the value of the resource.
     * \param value_length The length of the value in pointer.
     * \param instance_id The instance ID of the resource.
     * \return M2MResourceInstance The resource instance for managing other client operations.
     */
    M2MResourceInstance* create_static_resource_instance(const String &resource_name,
                                                         const String &resource_type,
                                                         M2MResourceInstance::ResourceType type,
                                                         const uint8_t *value,
                                                         const uint8_t value_length,
                                                         uint16_t instance_id);

    /**
     * \brief Creates a dynamic resource instance for a given mbed Client Inteface object. With this,
     * the client can respond to different queries from the server (GET,PUT etc).
     * This type of resource is also observable and carries callbacks.
     * \param resource_name The name of the resource.
     * \param resource_type The type of the resource.
     * \param observable Indicates whether the resource is observable or not.
     * \param instance_id The instance ID of the resource.
     * \return M2MResourceInstance The resource instance for managing other client operations.
     */
    M2MResourceInstance* create_dynamic_resource_instance(const String &resource_name,
                                                          const String &resource_type,
                                                          M2MResourceInstance::ResourceType type,
                                                          bool observable,
                                                          uint16_t instance_id);

    /**
     * \brief Removes the resource with the given name.
     * \param name The name of the resource to be removed.
     * \return True if removed, else false.
     */
    virtual bool remove_resource(const String &name);

    /**
     * \brief Removes the resource instance with the given name.
     * \param resource_name The name of the resource instance to be removed.
     * \param instance_id The instance ID of the instance.
     * \return True if removed, else false.
     */
    virtual bool remove_resource_instance(const String &resource_name,
                                          uint16_t instance_id);

    /**
     * \brief Returns the resource with the given name.
     * \param name The name of the requested resource.
     * \return Resource reference if found, else NULL.
     */
    virtual M2MResource* resource(const String &name) const;

    /**
     * \brief Returns a list of M2MResourceBase objects.
     * \return A list of M2MResourceBase objects.
     */
    virtual const M2MResourceList& resources() const;

    /**
     * \brief Returns the total number of resources with the object.
     * \return Total number of the resources.
     */
    virtual uint16_t resource_count() const;

    /**
     * \brief Returns the total number of single resource instances.
     * \param resource The name of the resource.
     * \return Total number of the resources.
     */
    virtual uint16_t resource_count(const String& resource) const;

    /**
     * \brief Returns the object type.
     * \return BaseType.
     */
    virtual M2MBase::BaseType base_type() const;

    /**
     * \brief Adds the observation level for the object.
     * \param observation_level The level of observation.
     */
    virtual void add_observation_level(M2MBase::Observation observation_level);

    /**
     * \brief Removes the observation level from the object.
     * \param observation_level The level of observation.
     */
    virtual void remove_observation_level(M2MBase::Observation observation_level);

    /**
     * \brief Handles GET request for the registered objects.
     * \param nsdl The NSDL handler for the CoAP library.
     * \param received_coap_header The CoAP message received from the server.
     * \param observation_handler The handler object for sending
     * observation callbacks.
     * return sn_coap_hdr_s The message that needs to be sent to the server.
     */
    virtual sn_coap_hdr_s* handle_get_request(nsdl_s *nsdl,
                                              sn_coap_hdr_s *received_coap_header,
                                              M2MObservationHandler *observation_handler = NULL);
    /**
     * \brief Handles PUT request for the registered objects.
     * \param nsdl The NSDL handler for the CoAP library.
     * \param received_coap_header The CoAP message received from the server.
     * \param observation_handler The handler object for sending
     * observation callbacks.
     * \param execute_value_updated True will execute the "value_updated" callback.
     * \return sn_coap_hdr_s The message that needs to be sent to server.
     */
    virtual sn_coap_hdr_s* handle_put_request(nsdl_s *nsdl,
                                              sn_coap_hdr_s *received_coap_header,
                                              M2MObservationHandler *observation_handler,
                                              bool &execute_value_updated);

    /**
     * \brief Handles POST request for the registered objects.
     * \param nsdl The NSDL handler for the CoAP library.
     * \param received_coap_header The CoAP message received from the server.
     * \param observation_handler The handler object for sending
     * observation callbacks.
     * \param execute_value_updated True will execute the "value_updated" callback.
     * \return sn_coap_hdr_s The message that needs to be sent to server.
     */
    virtual sn_coap_hdr_s* handle_post_request(nsdl_s *nsdl,
                                               sn_coap_hdr_s *received_coap_header,
                                               M2MObservationHandler *observation_handler,
                                               bool &execute_value_updated);


protected :

    virtual void notification_update(M2MBase::Observation observation_level);

private:

    M2MObjectCallback   &_object_callback;
    M2MResourceList     _resource_list; // owned

    friend class Test_M2MObjectInstance;
    friend class Test_M2MObject;
    friend class Test_M2MDevice;
    friend class Test_M2MSecurity;
    friend class Test_M2MServer;
    friend class Test_M2MNsdlInterface;
    friend class Test_M2MFirmware;
    friend class Test_M2MTLVSerializer;
    friend class Test_M2MTLVDeserializer;
};

#endif // M2M_OBJECT_INSTANCE_H
