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
#ifndef M2M_RESOURCE_INSTANCE_H
#define M2M_RESOURCE_INSTANCE_H

#include "mbed-client/m2mbase.h"
#include "mbed-client/functionpointer.h"

class M2MObjectInstanceCallback {
public:
    virtual void notification_update(M2MBase::Observation observation_level) = 0;
};

/**
 *  \brief M2MResourceInstance.
 *  This class is the base class for mbed Client Resources. All defined
 *  LWM2M resource models can be created based on it.
 */
typedef FP1<void,void*> execute_callback;
typedef void(*execute_callback_2) (void *arguments);

class M2MResourceCallback;

class M2MResourceInstance : public M2MBase {

friend class M2MObjectInstance;
friend class M2MResource;

public:

    /**
     * Enum defining a resource type that can be
     * supported by a given resource.
    */
    typedef enum {
        STRING,
        INTEGER,
        FLOAT,
        BOOLEAN,
        OPAQUE,
        TIME,
        OBJLINK
    }ResourceType;


private: // Constructor and destructor are private
         // which means that these objects can be created or
         // deleted only through a function provided by the M2MObjectInstance.
    /**
     * \brief A constructor for creating a resource.
     * \param resource_name The name of the resource.
     * \param resource_type The type of the resource.
     * \param type, The resource data type of the object.
     * \param object_instance_id Object instance id where resource exists.
     * \param object_name Object name where resource exists.
     */
    M2MResourceInstance(const String &resource_name,
                        const String &resource_type,
                        M2MResourceInstance::ResourceType type,
                        M2MObjectInstanceCallback &object_instance_callback,
                        const uint16_t object_instance_id = 0,
                        const String &object_name = "");

    /**
     * \brief A Constructor for creating a resource.
     * \param resource_name The name of the resource.
     * \param resource_type The type of the resource.
     * \param type The resource data type of the object.
     * \param value The value pointer of the object.
     * \param value_length The length of the value pointer.
     * \param value_length The length of the value pointer.
     * \param object_instance_id Object instance id where resource exists.
     * \param object_name Object name where resource exists.
     */
    M2MResourceInstance(const String &resource_name,
                        const String &resource_type,
                        M2MResourceInstance::ResourceType type,
                        const uint8_t *value,
                        const uint8_t value_length,
                        M2MObjectInstanceCallback &object_instance_callback,
                        const uint16_t object_instance_id = 0,
                        const String &object_name = "");

    // Prevents the use of default constructor.
    M2MResourceInstance();

    // Prevents the use of assignment operator.
    M2MResourceInstance& operator=( const M2MResourceInstance& /*other*/ );

    // Prevents the use of copy constructor
    M2MResourceInstance( const M2MResourceInstance& /*other*/ );

    /**
     * Destructor
     */
    virtual ~M2MResourceInstance();

public:

    /**
     * \brief Returns the object type.
     * \return BaseType.
     */
    virtual M2MBase::BaseType base_type() const;

    /**
     * \brief Returns the resource data type.
     * \return ResourceType.
     */
    virtual M2MResourceInstance::ResourceType resource_instance_type() const;

    /**
     * \brief Parses the received query for a notification
     * attribute.
     * \return True if required attributes are present, else false.
     */
    virtual bool handle_observation_attribute(char *&query);

    /**
     * \brief Sets the function that should be executed when this
     * resource receives a POST command.
     * \param callback The function pointer that needs to be executed.
     */
    virtual void set_execute_function(execute_callback callback);

    /**
     * \brief Sets the function that should be executed when this
     * resource receives a POST command.
     * \param callback The function pointer that needs to be executed.
     */
    virtual void set_execute_function(execute_callback_2 callback);

    /**
     * \brief Sets the value of the given resource.
     * \param value, A pointer to the value to be set on the resource.
     * \param value_length The length of the value pointer.
     * \return True if successfully set, else false.
     */
    virtual bool set_value(const uint8_t *value, const uint32_t value_length);

    /**
     * \brief Sets the value of the given resource.
     * \param value, new value which is to be formatted into a string
     * and set on the resource.
     * \return True if successfully set, else false.
     */
    virtual bool set_value(int64_t value);

    /**
     * \brief Clears the value of the given resource.
     */
    virtual void clear_value();

    /**
     * \brief Executes the function that is set in "set_execute_function".
     * \param arguments The arguments that will be passed to be executed.
     */
    void execute(void *arguments);

    /**
     * \brief Provides the value of the given resource.
     * \param value[OUT] A pointer to the resource value.
     * \param value_length[OUT] The length of the value pointer.
     */
    virtual void get_value(uint8_t *&value, uint32_t &value_length);

    /**
     * \brief Converts value to int and return it. Note: conversion
     * errors are not detected.
     */
    int get_value_int();

    /**
     * Get the value as a String object. No encoding/charset conversions
     * done for the value, just a raw copy.
     */
    String get_value_string() const;

    /**
     * \brief Returns the value pointer of the object.
     * \return The value pointer of the object.
    */
    uint8_t* value() const;

    /**
     * \brief Returns the length of the value pointer.
     * \return The length of the value pointer.
    */
    uint32_t value_length() const;

    /**
     * \brief Handles the GET request for the registered objects.
     * \param nsdl The NSDL handler for the CoAP library.
     * \param received_coap_header The CoAP message received from the server.
     * \param observation_handler The handler object for sending
     * observation callbacks.
     * \return sn_coap_hdr_s The message that needs to be sent to the server.
     */
    virtual sn_coap_hdr_s* handle_get_request(nsdl_s *nsdl,
                                              sn_coap_hdr_s *received_coap_header,
                                              M2MObservationHandler *observation_handler = NULL);
    /**
     * \brief Handles the PUT request for the registered objects.
     * \param nsdl The NSDL handler for the CoAP library.
     * \param received_coap_header The CoAP message received from the server.
     * \param observation_handler The handler object for sending
     * observation callbacks.
     * \param execute_value_updated True will execute the "value_updated" callback.
     * \return sn_coap_hdr_s The message that needs to be sent to the server.
     */
    virtual sn_coap_hdr_s* handle_put_request(nsdl_s *nsdl,
                                              sn_coap_hdr_s *received_coap_header,                                                                                            
                                              M2MObservationHandler *observation_handler,
                                              bool &execute_value_updated);

    /**
     * \brief Returns the object instance id where resource exists.
     * \return Object instance id.
    */
    uint16_t object_instance_id() const;

    /**
     * \brief Returns the object name where resource exists.
     * \return Object name.
    */
    const String& object_name() const;

protected:

    /**
     * \brief Set observer for sending the notification update.
     * \param resource The callback handle.
     */
    void set_resource_observer(M2MResourceCallback *resource);

private:

    void report();

    bool is_value_changed(const uint8_t* value, const uint32_t value_len);

private:

    M2MObjectInstanceCallback               &_object_instance_callback;
    execute_callback                        _execute_callback;
    uint8_t                                 *_value;
    uint32_t                                _value_length;
    M2MResourceCallback                     *_resource_callback; // Not owned
    String                                  _object_name;
    FP1<void, void*>                        *_function_pointer;

    uint16_t                                _object_instance_id;
    ResourceType                            _resource_type;

    friend class Test_M2MResourceInstance;
    friend class Test_M2MResource;
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

#endif // M2M_RESOURCE_INSTANCE_H
