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
#ifndef M2M_BASE_H
#define M2M_BASE_H

// Support for std args
#include <stdint.h>
#include "mbed-client/m2mconfig.h"
#include "mbed-client/m2mreportobserver.h"
#include "mbed-client/functionpointer.h"

//FORWARD DECLARATION
struct sn_coap_hdr_;
typedef sn_coap_hdr_ sn_coap_hdr_s;
struct nsdl_s;

typedef FP1<void, const char*> value_updated_callback;
typedef void(*value_updated_callback2) (const char* object_name);
class M2MObservationHandler;
class M2MReportHandler;

/**
 *  \brief M2MBase.
 *  This class is the base class based on which all LWM2M object models
 *  can be created. This serves base class for Object, ObjectInstances and Resources.
 */



class M2MBase : public M2MReportObserver {

public:

    /**
      * Enum to define the type of object.
      */
    typedef enum {
        Object = 0x0,
        Resource = 0x1,
        ObjectInstance = 0x2,
        ResourceInstance = 0x3
    } BaseType;

    /**
      * Enum to define observation level.
      */
    typedef enum {
        None                 = 0x0,
        R_Attribute          = 0x01,
        OI_Attribute         = 0x02,
        OIR_Attribute        = 0x03,
        O_Attribute          = 0x04,
        OR_Attribute         = 0x05,
        OOI_Attribute        = 0x06,
        OOIR_Attribute       = 0x07
    } Observation;


    /**
     * \brief Enum defining an operation that can be
     * supported by a given resource.
    */
    typedef enum {
        Static,
        Dynamic,
        Directory
    }Mode;

    /**
     * Enum defining an operation that can be
     * supported by a given resource.
    */
    typedef enum {
        NOT_ALLOWED                 = 0x00,
        GET_ALLOWED                 = 0x01,
        PUT_ALLOWED                 = 0x02,
        GET_PUT_ALLOWED             = 0x03,
        POST_ALLOWED                = 0x04,
        GET_POST_ALLOWED            = 0x05,
        PUT_POST_ALLOWED            = 0x06,
        GET_PUT_POST_ALLOWED        = 0x07,
        DELETE_ALLOWED              = 0x08,
        GET_DELETE_ALLOWED          = 0x09,
        PUT_DELETE_ALLOWED          = 0x0A,
        GET_PUT_DELETE_ALLOWED      = 0x0B,
        POST_DELETE_ALLOWED         = 0x0C,
        GET_POST_DELETE_ALLOWED     = 0x0D,
        PUT_POST_DELETE_ALLOWED     = 0x0E,
        GET_PUT_POST_DELETE_ALLOWED = 0x0F,

    }Operation;

protected:

    // Prevents the use of default constructor.
    M2MBase();

    // Prevents the use of assignment operator.
    M2MBase& operator=( const M2MBase& /*other*/ );

    // Prevents the use of copy constructor
    M2MBase( const M2MBase& /*other*/ );

    /**
     * \brief Constructor
     * \param baseType Type of the object created
     * \param name Name of the object
     * \param id ID of the object
     */
    M2MBase(const String &name,
            M2MBase::Mode mode);
public:

    /**
     * Destructor
     */
    virtual ~M2MBase();

    /**
     * \brief Sets the operation type for an object.
     * \param operation Operation to be set.
     */
    virtual void set_operation(M2MBase::Operation operation);

    /**
     * \brief Sets the interface description of the object.
     * \param description Description to be set.
     */
    virtual void set_interface_description(const String &description);

    /**
     * \brief Sets the resource type of the object.
     * \param resource_type Resource type to be set.
     */
    virtual void set_resource_type(const String &resource_type);

    /**
     * \brief Sets the CoAP content type of the object.
     * \param content_type Content Type to be set based on
     * CoAP specifications.
     */
    virtual void set_coap_content_type(const uint8_t content_type);

    /**
     * \brief Sets the observable mode for the object.
     * \param observable Value for the observation.
     */
    virtual void set_observable(bool observable);

    /**
     * \brief Adds the observation level for the object.
     * \param observation_level Level of the observation.
     */
    virtual void add_observation_level(M2MBase::Observation observation_level);

    /**
     * \brief Removes the observation level for the object.
     * \param observation_level Level of the observation.
     */
    virtual void remove_observation_level(M2MBase::Observation observation_level);

    /**
     * \brief Sets the object under observation.
     * \param observed Value for the observation. When true, starts observing. When false, ongoing observation is cancelled.
     * \param handler Handler object for sending
     * observation callbacks.
     */
    virtual void set_under_observation(bool observed,
                                       M2MObservationHandler *handler);

    /**
     * \brief Sets the observation token value.
     * \param token Pointer to the token of the resource.
     * \param length Length of the token pointer.
     */
    virtual void set_observation_token(const uint8_t *token,
                                       const uint8_t length);

    /**
     * \brief Sets the instance ID of the object.
     * \param instance_id Instance ID of the object.
     */
    virtual void set_instance_id(const uint16_t instance_id);

    /**
     * Function is deprecated. Increment of observation number is done internally.
     * \brief Sets the observation number of the object.
     * \param observation_number Observation number of the object.
     */
    virtual void set_observation_number(const uint16_t observation_number)
        m2m_deprecated;

    /**
     * \brief Sets the max age for the resource value to be cached.
     * \param max_age Max age in seconds.
     */
    virtual void set_max_age(const uint32_t max_age);

    /**
     * \brief Returns object type.
     * \return BaseType of the object.
     */
    virtual M2MBase::BaseType base_type() const;

    /**
     * \brief Returns the operation type of the object.
     * \return Operation Supported operation on the object.
     */
    virtual M2MBase::Operation operation() const;

    /**
     * \brief Returns the object name.
     * \return Name of the object.
     */
    virtual const String &name() const;

    /**
     * \brief Returns the object name in integer.
     * \return Name of the object in integer.
     */
    virtual int32_t name_id() const;

    /**
     * \brief Returns the object's Instance ID.
     * \returns Instance ID of the object.
     */
    virtual uint16_t instance_id() const;

    /**
     * \brief Returns the interface description of the object.
     * \return Description of the object.
     */
    virtual const String& interface_description() const;

    /**
     * \brief Returns the resource type of the object.
     * \return Resource type of the object.
     */
    virtual const String& resource_type() const;

    /**
     * \brief Returns the CoAP content type of the object.
     * \return Content type of the object.
     */
    virtual uint8_t coap_content_type() const;

    /**
     * \brief Returns the observation status of the object.
     * \return True if observable, else false.
     */
    virtual bool is_observable() const;

    /**
     * \brief Returns the observation level of the object.
     * \return Observation level of the object.
     */
    virtual M2MBase::Observation observation_level() const;

    /**
     * \brief Provides the observation token of the object.
     * \param value[OUT] A pointer to the value of the token.
     * \param value_length[OUT] Length of the token pointer.
     */
    virtual void get_observation_token(uint8_t *&token, uint32_t &token_length);

    /**
     * \brief Returns the mode of the resource.
     * \return Mode of the resource.
     */
     virtual Mode mode() const;

    /**
     * \brief Returns the observation number.
     * \return Observation number of the object.
     */
    virtual uint16_t observation_number() const;

    /**
     * \brief Returns max age for the resource value to be cached.
     * \return Max age in seconds.
     */
    virtual uint32_t max_age() const;


    /**
     * \brief Parses the received query for the notification
     * attribute.
     * \param query The query that needs to be parsed.
     * \return True if required attributes are present, else false.
     */
    virtual bool handle_observation_attribute(char *&query);

    /**
     * \brief Handles GET request for the registered objects.
     * \param nsdl NSDL handler for the CoAP library.
     * \param received_coap_header Received CoAP message from the server.
     * \param observation_handler Handler object for sending
     * observation callbacks.
     * \return sn_coap_hdr_s The message that needs to be sent to server.
     */
    virtual sn_coap_hdr_s* handle_get_request(nsdl_s *nsdl,
                                              sn_coap_hdr_s *received_coap_header,
                                              M2MObservationHandler *observation_handler = NULL);
    /**
     * \brief Handles PUT request for the registered objects.
     * \param nsdl NSDL handler for the CoAP library.
     * \param received_coap_header The received CoAP message from the server.
     * \param observation_handler The handler object for sending
     * observation callbacks.
     * \param execute_value_updated True will execute "value_updated" callback.
     * \return sn_coap_hdr_s The message that needs to be sent to server.
     */
    virtual sn_coap_hdr_s* handle_put_request(nsdl_s *nsdl,
                                              sn_coap_hdr_s *received_coap_header,
                                              M2MObservationHandler *observation_handler,
                                              bool &execute_value_updated);

    /**
     * \brief Handles GET request for the registered objects.
     * \param nsdl NSDL handler for the CoAP library.
     * \param received_coap_header The received CoAP message from the server.
     * \param observation_handler The handler object for sending
     * observation callbacks.
     * \param execute_value_updated True will execute "value_updated" callback.
     * \return sn_coap_hdr_s  The message that needs to be sent to server.
     */
    virtual sn_coap_hdr_s* handle_post_request(nsdl_s *nsdl,
                                               sn_coap_hdr_s *received_coap_header,
                                               M2MObservationHandler *observation_handler,
                                               bool &execute_value_updated);

    /**
     * \brief Sets whether this resource will be published to server or not.
     * \param register_uri True sets the resource as part of registration message.
     */
    virtual void set_register_uri( bool register_uri);

    /**
     * \brief Returns whether this resource will be published to server or not.
     * \return True if the resource is part of the registration message, else false.
     */
    virtual bool register_uri();

    /**
     * \brief Sets object URI path.
     * \param path Object path
     */
    virtual void set_uri_path(const String &path);

    /**
     * \brief Returns the URI path of the object.
     * \return URI path of the object.
     */
    virtual const String &uri_path() const;

    /**
     * @brief Returns whether this resource is under observation or not.
     * @return True if the resource is under observation, else false,
     */
    virtual bool is_under_observation() const;

    /**
     * @brief Sets the function that is executed when this
     * object receives a PUT or POST command.
     * @param callback The function pointer that is called.
     */
    virtual void set_value_updated_function(value_updated_callback callback);

    /**
     * @brief Sets the function that is executed when this
     * object receives a PUT or POST command.
     * @param callback The function pointer that is called.
     */
    virtual void set_value_updated_function(value_updated_callback2 callback);

    /**
     * @brief Returns whether callback function is set or not.
     * @return True if the callback function is set, else false.
     */
    virtual bool is_value_updated_function_set();

    /**
     * @brief Calls the function that is set in "set_value_updated_function".
     * @param name Name of the object.
     */
    virtual void execute_value_updated(const String& name);

protected : // from M2MReportObserver

    virtual void observation_to_be_sent(m2m::Vector<uint16_t> changed_instance_ids,
                                        bool send_object = false);

protected:

    /**
     * \brief Sets the Base type for object.
     * \param type The type of the base object.
     */
    virtual void set_base_type(M2MBase::BaseType type);

    /**
     * \brief Removes the resource from the CoAP structure.
     * \param resource_name The name of the resource.
     */
    virtual void remove_resource_from_coap(const String &resource_name);

    /**
     * \brief Removes an object from the NSDL list.
     */
    virtual void remove_object_from_coap();

    /**
     * \brief Memory allocation required for libCoap.
     * \param size The size of memory to be reserved.
    */
    virtual void* memory_alloc(uint32_t size);

    /**
     * \brief Memory free functions required for libCoap.
     * \param ptr The Object whose memory needs to be freed.
    */
    virtual void memory_free(void *ptr);

    /**
     * \brief Allocate (size + 1) amount of memory, copy size bytes into
     * it and add zero termination.
     * \param source Source string to copy, may not be NULL.
     * \param size The size of memory to be reserved.
    */
    virtual uint8_t* alloc_string_copy(const uint8_t* source, uint32_t size);

    /**
     * \brief Allocate (size) amount of memory, copy size bytes into it.
     * \param source Source buffer to copy, may not be NULL.
     * \param size The size of memory to be reserved.
    */
    virtual uint8_t* alloc_copy(const uint8_t* source, uint32_t size);

    /**
     * \brief Returns the Report Handler object.
     * \return M2MReportHandler object.
    */
    M2MReportHandler* report_handler();

    /**
     * \brief Returns the Observation Handler object.
     * \return M2MObservationHandler object.
    */
    M2MObservationHandler* observation_handler();

private:

    static bool is_integer(const String &value);

private:

    M2MReportHandler           *_report_handler;
    M2MObservationHandler      *_observation_handler;
    String                      _name;
    String                      _resource_type;
    String                      _interface_description;
    String                      _uri_path;
    int32_t                     _name_id;
    uint32_t                    _max_age;
    uint16_t                    _instance_id;
    uint16_t                    _observation_number;
    uint8_t                     *_token;
    uint8_t                     _token_length;
    uint8_t                     _coap_content_type;
    M2MBase::Operation          _operation;
    M2MBase::Mode               _mode;
    M2MBase::BaseType           _base_type;
    M2MBase::Observation        _observation_level;
    bool                        _observable;
    bool                        _register_uri;
    bool                        _is_under_observation;
    value_updated_callback      _value_updated_callback;
    FP1<void, const char*>      *_function_pointer;

friend class Test_M2MBase;

};

#endif // M2M_BASE_H

