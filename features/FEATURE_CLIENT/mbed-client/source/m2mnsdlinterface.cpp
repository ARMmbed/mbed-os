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

// Note: this macro is needed on armcc to get the the PRI*32 macros
// from inttypes.h in a C++ code.
#ifndef __STDC_FORMAT_MACROS
#define __STDC_FORMAT_MACROS
#endif

// Note: this macro is needed on armcc to get the the limit macros like UINT16_MAX
#ifndef __STDC_LIMIT_MACROS
#define __STDC_LIMIT_MACROS
#endif


#include "include/nsdlaccesshelper.h"
#include "include/m2mnsdlobserver.h"
#include "include/m2mtlvdeserializer.h"
#include "include/m2mtlvserializer.h"
#include "include/m2mnsdlinterface.h"
#include "mbed-client/m2mstring.h"
#include "mbed-client/m2msecurity.h"
#include "mbed-client/m2mserver.h"
#include "mbed-client/m2mobject.h"
#include "mbed-client/m2mobjectinstance.h"
#include "mbed-client/m2mresource.h"
#include "mbed-client/m2mconstants.h"
#include "mbed-trace/mbed_trace.h"
#include "mbed-client/m2mtimer.h"
#include "source/libNsdl/src/include/sn_grs.h"

#include <assert.h>
#include <inttypes.h>

#define BUFFER_SIZE 21
#define TRACE_GROUP "mClt"

M2MNsdlInterface::M2MNsdlInterface(M2MNsdlObserver &observer)
: _observer(observer),
  _server(NULL),
  _security(NULL),
  _nsdl_exceution_timer(new M2MTimer(*this)),
  _registration_timer(new M2MTimer(*this)),
  _endpoint(NULL),
  _resource(NULL),
  _nsdl_handle(NULL),
  _counter_for_nsdl(0),
  _bootstrap_id(0),
  _register_ongoing(false),
  _unregister_ongoing(false),
  _update_register_ongoing(false)
{
    tr_debug("M2MNsdlInterface::M2MNsdlInterface()");
    __nsdl_interface_list.push_back(this);
    _sn_nsdl_address.addr_len = 0;
    _sn_nsdl_address.addr_ptr = NULL;
    _sn_nsdl_address.port = 0;

    // This initializes libCoap and libNsdl
    // Parameters are function pointers to used memory allocation
    // and free functions in structure and used functions for sending
    // and receiving purposes.
    _nsdl_handle = sn_nsdl_init(&(__nsdl_c_send_to_server), &(__nsdl_c_received_from_server),
                 &(__nsdl_c_memory_alloc), &(__nsdl_c_memory_free));


    _server = new M2MServer();
    initialize();
}

M2MNsdlInterface::~M2MNsdlInterface()
{
    tr_debug("M2MNsdlInterface::~M2MNsdlInterface() - IN");
    if(_resource) {
        memory_free(_resource->resource_parameters_ptr);
        memory_free(_resource);
    }
    if(_endpoint) {
         memory_free(_endpoint->endpoint_name_ptr);
         memory_free(_endpoint->lifetime_ptr);
         memory_free(_endpoint->location_ptr);
         memory_free(_endpoint);
    }
    delete _nsdl_exceution_timer;
    delete _registration_timer;
    _object_list.clear();
    delete _server;
    _security = NULL;

    sn_nsdl_destroy(_nsdl_handle);
    _nsdl_handle = NULL;

    M2MNsdlInterfaceList::const_iterator it;
    it = __nsdl_interface_list.begin();
    int index = 0;
    for (; it!=__nsdl_interface_list.end(); it++) {
        if ((*it) == this) {
            __nsdl_interface_list.erase(index);
            break;
        }
        index++;
    }
    tr_debug("M2MNsdlInterface::~M2MNsdlInterface() - OUT");
}

bool M2MNsdlInterface::initialize()
{
    tr_debug("M2MNsdlInterface::initialize()");
    bool success = false;

    //Sets the packet retransmission attempts and time interval
    sn_nsdl_set_retransmission_parameters(_nsdl_handle,
                                          MBED_CLIENT_RECONNECTION_COUNT,
                                          MBED_CLIENT_RECONNECTION_INTERVAL);

    // We want to parse and handle bootstrap messages
    if (_nsdl_handle) {
        _nsdl_handle->handle_bootstrap_msg = false;
    }

    // Allocate the memory for resources
    _resource = (sn_nsdl_resource_info_s*)memory_alloc(sizeof(sn_nsdl_resource_info_s));
    if(_resource) {
        memset(_resource, 0, sizeof(sn_nsdl_resource_info_s));
        _resource->resource_parameters_ptr = (sn_nsdl_resource_parameters_s*)memory_alloc(sizeof(sn_nsdl_resource_parameters_s));
        if(_resource->resource_parameters_ptr) {
            memset(_resource->resource_parameters_ptr, 0, sizeof(sn_nsdl_resource_parameters_s));
        }
    }

    //Allocate the memory for endpoint
    _endpoint = (sn_nsdl_ep_parameters_s*)memory_alloc(sizeof(sn_nsdl_ep_parameters_s));
    if(_endpoint) {
        memset(_endpoint, 0, sizeof(sn_nsdl_ep_parameters_s));
        success = true;
    }
    return success;
}

void M2MNsdlInterface::create_endpoint(const String &name,
                                       const String &type,
                                       const int32_t life_time,
                                       const String &domain,
                                       const uint8_t mode,
                                       const String &/*context_address*/)
{
    tr_debug("M2MNsdlInterface::create_endpoint( name %s type %s lifetime %" PRId32 ", domain %s, mode %d)",
              name.c_str(), type.c_str(), life_time, domain.c_str(), mode);
    _endpoint_name = name;
    if(_endpoint){
        memset(_endpoint, 0, sizeof(sn_nsdl_ep_parameters_s));
        if(!_endpoint_name.empty()) {
            memory_free(_endpoint->endpoint_name_ptr);
            _endpoint->endpoint_name_ptr = alloc_string_copy((uint8_t*)_endpoint_name.c_str(), _endpoint_name.length());
            _endpoint->endpoint_name_len = _endpoint_name.length();
        }
        if(!type.empty()) {
            _endpoint->type_ptr = (uint8_t*)type.c_str();
            _endpoint->type_len =  type.length();
        }
        if(!domain.empty()) {
            _endpoint->domain_name_ptr = (uint8_t*)domain.c_str();
            _endpoint->domain_name_len = domain.length();
        }
        _endpoint->binding_and_mode = (sn_nsdl_oma_binding_and_mode_t)mode;

        // If lifetime is less than zero then leave the field empty
        if( life_time > 0) {
            set_endpoint_lifetime_buffer(life_time);
        }
    }
}

void M2MNsdlInterface::set_endpoint_lifetime_buffer(int lifetime)
{
    // max len of "-9223372036854775808" plus zero termination
    char buffer[20+1];

    uint32_t size = m2m::itoa_c(lifetime, buffer);

    if (size <= sizeof(buffer)) {
        _endpoint->lifetime_ptr = alloc_string_copy((uint8_t*)buffer, size);
        if(_endpoint->lifetime_ptr) {
            _endpoint->lifetime_len =  size;
        } else {
            _endpoint->lifetime_len = 0;
        }
    }
}


void M2MNsdlInterface::delete_endpoint()
{
    tr_debug("M2MNsdlInterface::delete_endpoint()");
    if(_endpoint) {
        free(_endpoint->lifetime_ptr);

        memory_free(_endpoint);
        _endpoint = NULL;
    }
}

bool M2MNsdlInterface::create_nsdl_list_structure(const M2MObjectList &object_list)
{
    tr_debug("M2MNsdlInterface::create_nsdl_list_structure()");
    bool success = false;
    if(!object_list.empty()) {
       tr_debug("M2MNsdlInterface::create_nsdl_list_structure - Object count is %d", object_list.size());
        M2MObjectList::const_iterator it;
        it = object_list.begin();
        for ( ; it != object_list.end(); it++ ) {
            // Create NSDL structure for all Objects inside
            success = create_nsdl_object_structure(*it);
            add_object_to_list(*it);
        }
    }
    return success;
}

bool M2MNsdlInterface::delete_nsdl_resource(const String &resource_name)
{
    tr_debug("M2MNsdlInterface::delete_nsdl_resource( %s)", resource_name.c_str());
    return (sn_nsdl_delete_resource(_nsdl_handle,
                                    resource_name.length(),
                                    (uint8_t *)resource_name.c_str()) == 0) ? true : false;
}


bool M2MNsdlInterface::create_bootstrap_resource(sn_nsdl_addr_s *address, const String &bootstrap_endpoint_name)
{
#ifndef MBED_CLIENT_DISABLE_BOOTSTRAP_FEATURE
    tr_debug("M2MNsdlInterface::create_bootstrap_resource()");
    bool success = false;
    sn_nsdl_bs_ep_info_t bootstrap_endpoint;
    tr_debug("M2MNsdlInterface::create_bootstrap_resource() - endpoint name: %s", bootstrap_endpoint_name.c_str());
    if (_endpoint->endpoint_name_ptr) {
        memory_free(_endpoint->endpoint_name_ptr);
    }
    //_endpoint->endpoint_name_ptr = (uint8_t*)bootstrap_endpoint_name.c_str();
    _endpoint->endpoint_name_ptr = alloc_string_copy((uint8_t*)bootstrap_endpoint_name.c_str(), bootstrap_endpoint_name.length());
    _endpoint->endpoint_name_len = bootstrap_endpoint_name.length();
    if(_bootstrap_id == 0) {
        _bootstrap_id = sn_nsdl_oma_bootstrap(_nsdl_handle,
                                               address,
                                               _endpoint,
                                               &bootstrap_endpoint);
        tr_debug("M2MNsdlInterface::create_bootstrap_resource - _bootstrap_id %d", _bootstrap_id);
        success = _bootstrap_id != 0;

    }
    return success;
#else
    (void)address;
    (void)bootstrap_endpoint_name;
    return false;
#endif //MBED_CLIENT_DISABLE_BOOTSTRAP_FEATURE
}

bool M2MNsdlInterface::send_register_message(uint8_t* address,
                                             const uint16_t port,
                                             sn_nsdl_addr_type_e address_type)
{
    tr_debug("M2MNsdlInterface::send_register_message()");
    _nsdl_exceution_timer->stop_timer();
    _nsdl_exceution_timer->start_timer(ONE_SECOND_TIMER * 1000,
                                       M2MTimerObserver::NsdlExecution,
                                       false);
    bool success = false;
    if(set_NSP_address(_nsdl_handle,address, port, address_type) == 0) {
        if(!_register_ongoing) {
            _register_ongoing = true;
            success = sn_nsdl_register_endpoint(_nsdl_handle,_endpoint) != 0;
        }
    }
    return success;
}

bool M2MNsdlInterface::send_update_registration(const uint32_t lifetime)
{
    if (_update_register_ongoing) {
        tr_debug("M2MNsdlInterface::send_update_registration - update already in progress");
        return true;
    }
    tr_debug("M2MNsdlInterface::send_update_registration( lifetime %" PRIu32 ")", lifetime);
    bool success = false;

    create_nsdl_list_structure(_object_list);
    //If Lifetime value is 0, then don't change the existing lifetime value
    if(lifetime != 0) {
        if(_endpoint->lifetime_ptr) {
            memory_free(_endpoint->lifetime_ptr);
            _endpoint->lifetime_ptr = NULL;
            _endpoint->lifetime_len = 0;
        }
        set_endpoint_lifetime_buffer(lifetime);

        _registration_timer->stop_timer();
        _registration_timer->start_timer(registration_time() * 1000,
                                         M2MTimerObserver::Registration,
                                         false);
        if(_nsdl_handle &&
           _endpoint && _endpoint->lifetime_ptr) {
            tr_debug("M2MNsdlInterface::send_update_registration - new lifetime value");
            _update_register_ongoing = true;
            success = sn_nsdl_update_registration(_nsdl_handle,
                                                  _endpoint->lifetime_ptr,
                                                  _endpoint->lifetime_len) != 0;
        }
    } else {
        if(_nsdl_handle) {
            tr_debug("M2MNsdlInterface::send_update_registration - regular update");
            _update_register_ongoing = true;
            success = sn_nsdl_update_registration(_nsdl_handle, NULL, 0) != 0;
        }
    }
    return success;
}

bool M2MNsdlInterface::send_unregister_message()
{
    tr_debug("M2MNsdlInterface::send_unregister_message");
    if (_unregister_ongoing) {
        tr_debug("M2MNsdlInterface::send_unregister_message - unregistration already in progress");
        return true;
    }

    bool success = false;
    _unregister_ongoing = true;
    success = sn_nsdl_unregister_endpoint(_nsdl_handle) != 0;
    return success;
}

// XXX: move these to common place, no need to copy these wrappers to multiple places:
void *M2MNsdlInterface::memory_alloc(uint16_t size)
{
    if(size)
        return malloc(size);
    else
        return 0;
}

void M2MNsdlInterface::memory_free(void *ptr)
{
    if(ptr)
        free(ptr);
}

uint8_t* M2MNsdlInterface::alloc_string_copy(const uint8_t* source, uint16_t size)
{
    assert(source != NULL);

    uint8_t* result = (uint8_t*)memory_alloc(size + 1);
    if (result) {
        memcpy(result, source, size);
        result[size] = '\0';
    }
    return result;
}

uint8_t M2MNsdlInterface::send_to_server_callback(struct nsdl_s * /*nsdl_handle*/,
                                                  sn_nsdl_capab_e /*protocol*/,
                                                  uint8_t *data_ptr,
                                                  uint16_t data_len,
                                                  sn_nsdl_addr_s *address)
{
    tr_debug("M2MNsdlInterface::send_to_server_callback()");
    _observer.coap_message_ready(data_ptr,data_len,address);
    return 1;
}

uint8_t M2MNsdlInterface::received_from_server_callback(struct nsdl_s * nsdl_handle,
                                                        sn_coap_hdr_s *coap_header,
                                                        sn_nsdl_addr_s *address)
{
    tr_debug("M2MNsdlInterface::received_from_server_callback - msg id:%" PRIu16, coap_header->msg_id);
    tr_debug("M2MNsdlInterface::received_from_server_callback - registration id:%" PRIu16, nsdl_handle->register_msg_id);
    tr_debug("M2MNsdlInterface::received_from_server_callback - unregistration id:%" PRIu16, nsdl_handle->unregister_msg_id);
    tr_debug("M2MNsdlInterface::received_from_server_callback - update registration id:%" PRIu16, nsdl_handle->update_register_msg_id);
    _observer.coap_data_processed();
    uint8_t value = 0;
    if(nsdl_handle && coap_header) {
        bool is_bootstrap_msg = address && (nsdl_handle->oma_bs_address_len == address->addr_len) &&
                                   (nsdl_handle->oma_bs_port == address->port) &&
                                   !memcmp(nsdl_handle->oma_bs_address_ptr, address->addr_ptr, nsdl_handle->oma_bs_address_len);
        if(coap_header->msg_id == nsdl_handle->register_msg_id) {
            _register_ongoing = false;
            if(coap_header->msg_code == COAP_MSG_CODE_RESPONSE_CREATED) {
                tr_debug("M2MNsdlInterface::received_from_server_callback - registration callback");
                _observer.client_registered(_server);
                // If lifetime is less than zero then leave the field empty
                if(coap_header->options_list_ptr) {
                    if(coap_header->options_list_ptr->max_age_ptr) {
                        memory_free(_endpoint->lifetime_ptr);
                        _endpoint->lifetime_ptr = NULL;
                        _endpoint->lifetime_len = 0;

                        uint32_t max_time = 0;
                        for(int i=0;i < coap_header->options_list_ptr->max_age_len; i++) {
                            max_time += (*(coap_header->options_list_ptr->max_age_ptr + i) & 0xff) <<
                                     8*(coap_header->options_list_ptr->max_age_len- 1 - i);
                            }
                        // If lifetime is less than zero then leave the field empty
                        if( max_time > 0) {
                            set_endpoint_lifetime_buffer(max_time);
                        }
                    }
                    if(coap_header->options_list_ptr->location_path_ptr) {

                        memory_free(_endpoint->location_ptr);

                        _endpoint->location_ptr = alloc_string_copy(coap_header->options_list_ptr->location_path_ptr, coap_header->options_list_ptr->location_path_len);
                        if (_endpoint->location_ptr != NULL) {
                            _endpoint->location_len = coap_header->options_list_ptr->location_path_len;
                        }
                        sn_nsdl_set_endpoint_location(_nsdl_handle,_endpoint->location_ptr,_endpoint->location_len);
                    }
                }
                if(_endpoint->lifetime_ptr) {
                    _registration_timer->stop_timer();
                    _registration_timer->start_timer(registration_time() * 1000,
                                                     M2MTimerObserver::Registration,
                                                     false);
                }
            } else {
                tr_error("M2MNsdlInterface::received_from_server_callback - registration error %d", coap_header->msg_code);
                // Try to do clean register again
                if(COAP_MSG_CODE_RESPONSE_BAD_REQUEST == coap_header->msg_code ||
                   COAP_MSG_CODE_RESPONSE_FORBIDDEN == coap_header->msg_code) {
                    _observer.registration_error(M2MInterface::InvalidParameters, false);
                } else {
                    _observer.registration_error(M2MInterface::NetworkError, true);
                }

            }
        } else if(coap_header->msg_id == nsdl_handle->unregister_msg_id) {
            _unregister_ongoing = false;
            tr_debug("M2MNsdlInterface::received_from_server_callback - unregistration callback");
            if(coap_header->msg_code == COAP_MSG_CODE_RESPONSE_DELETED) {
                _registration_timer->stop_timer();
                _observer.client_unregistered();
            } else {
                tr_error("M2MNsdlInterface::received_from_server_callback - unregistration error %d", coap_header->msg_code);
                M2MInterface::Error error = interface_error(coap_header);
                _observer.registration_error(error);
            }
        } else if(coap_header->msg_id == nsdl_handle->update_register_msg_id) {
            _update_register_ongoing = false;
            if(coap_header->msg_code == COAP_MSG_CODE_RESPONSE_CHANGED) {
                tr_debug("M2MNsdlInterface::received_from_server_callback - registration_updated successfully");
                _observer.registration_updated(*_server);
            } else {
                tr_error("M2MNsdlInterface::received_from_server_callback - registration_updated failed %d", coap_header->msg_code);
                _registration_timer->stop_timer();
                _register_ongoing = true;
                sn_nsdl_register_endpoint(_nsdl_handle,_endpoint);
            }
        }
#ifndef MBED_CLIENT_DISABLE_BOOTSTRAP_FEATURE
        else if(coap_header->msg_id == nsdl_handle->bootstrap_msg_id) {
            tr_debug("M2MNsdlInterface::received_from_server_callback - bootstrap");
            _bootstrap_id = 0;
            M2MInterface::Error error = interface_error(coap_header);
            if(error != M2MInterface::ErrorNone) {
                handle_bootstrap_error();
            }
        }
#endif //MBED_CLIENT_DISABLE_BOOTSTRAP_FEATURE
        else {
            if(COAP_MSG_CODE_REQUEST_PUT == coap_header->msg_code) {
                if (is_bootstrap_msg) {
                    handle_bootstrap_put_message(coap_header, address);
                }
            }
            else if(COAP_MSG_CODE_REQUEST_DELETE == coap_header->msg_code) {
                if (is_bootstrap_msg) {
                    handle_bootstrap_delete(coap_header, address);
                }
            }
            else if(COAP_MSG_CODE_REQUEST_POST == coap_header->msg_code) {
                if(is_bootstrap_msg) {
                    handle_bootstrap_finished(coap_header, address);
                }
                else if(coap_header->uri_path_ptr) {
                    sn_coap_hdr_s *coap_response = NULL;
                    bool execute_value_updated = false;
                    M2MObjectInstance *obj_instance = NULL;
                    String resource_name = coap_to_string(coap_header->uri_path_ptr,
                                                          coap_header->uri_path_len);

                    String object_name;
                    int slash_found = resource_name.find_last_of('/');
                    //The POST operation here is only allowed for non-existing object instances
                    if(slash_found != -1) {
                        object_name = resource_name.substr(0,slash_found);
                        if( object_name.find_last_of('/') != -1){
                            coap_response = sn_nsdl_build_response(_nsdl_handle,
                                                                   coap_header,
                                                                   COAP_MSG_CODE_RESPONSE_NOT_FOUND);
                        } else {
                            int32_t instance_id = atoi(resource_name.substr(slash_found+1,
                                                     resource_name.size()-object_name.size()).c_str());
                            M2MBase* base = find_resource(object_name);
                            if(base && (instance_id >= 0) && (instance_id < UINT16_MAX)) {
                                if(coap_header->payload_ptr) {
                                    M2MObject* object = static_cast<M2MObject*> (base);
                                    obj_instance = object->create_object_instance(instance_id);
                                    if(obj_instance) {
                                        obj_instance->set_operation(M2MBase::GET_PUT_POST_ALLOWED);
                                        coap_response = obj_instance->handle_post_request(_nsdl_handle,
                                                                                          coap_header,
                                                                                          this,
                                                                                          execute_value_updated);
                                    }
                                    if(coap_response && coap_response->msg_code != COAP_MSG_CODE_RESPONSE_CREATED) {
                                        //Invalid request so remove created ObjectInstance
                                        object->remove_object_instance(instance_id);
                                    } else  {
                                        tr_debug("M2MNsdlInterface::received_from_server_callback - Send Update registration for Create");
                                        send_update_registration();
                                    }
                                } else {
                                    tr_debug("M2MNsdlInterface::received_from_server_callback - Missing Payload - Cannot create");
                                    coap_response = sn_nsdl_build_response(_nsdl_handle,
                                                                           coap_header,
                                                                           COAP_MSG_CODE_RESPONSE_BAD_REQUEST);
                                }
                            } else { //if(base)
                                tr_debug("M2MNsdlInterface::received_from_server_callback - Missing BASE - Cannot create");
                                coap_response = sn_nsdl_build_response(_nsdl_handle,
                                                                       coap_header,
                                                                       COAP_MSG_CODE_RESPONSE_METHOD_NOT_ALLOWED);
                            }
                        }
                    } else{ // if(slash_found != -1)
                        tr_debug("M2MNsdlInterface::received_from_server_callback - slash_found - Cannot create");
                        coap_response = sn_nsdl_build_response(_nsdl_handle,
                                                               coap_header,
                                                               COAP_MSG_CODE_RESPONSE_NOT_FOUND);
                    }
                    if(coap_response) {
                        tr_debug("M2MNsdlInterface::received_from_server_callback - send CoAP response");
                        (sn_nsdl_send_coap_message(_nsdl_handle, address, coap_response) == 0) ? value = 0 : value = 1;
                        sn_nsdl_release_allocated_coap_msg_mem(_nsdl_handle, coap_response);
                    }
                    if (execute_value_updated) {
                        value_updated(obj_instance, obj_instance->name());
                    }
                }
            }
        }
    }
    return value;
}

uint8_t M2MNsdlInterface::resource_callback(struct nsdl_s */*nsdl_handle*/,
                                            sn_coap_hdr_s *received_coap_header,
                                            sn_nsdl_addr_s *address,
                                            sn_nsdl_capab_e /*nsdl_capab*/)
{
    tr_debug("M2MNsdlInterface::resource_callback()");
    _observer.coap_data_processed();
    uint8_t result = 1;
    sn_coap_hdr_s *coap_response = NULL;
    sn_coap_msg_code_e msg_code = COAP_MSG_CODE_RESPONSE_CHANGED; // 4.00
    String resource_name = coap_to_string(received_coap_header->uri_path_ptr,
                                          received_coap_header->uri_path_len);
    tr_debug("M2MNsdlInterface::resource_callback() - resource_name %s", resource_name.c_str());
    bool execute_value_updated = false;
    M2MBase* base = find_resource(resource_name);
    if(base) {
        base->set_uri_path(resource_name);
        if(COAP_MSG_CODE_REQUEST_GET == received_coap_header->msg_code) {
            coap_response = base->handle_get_request(_nsdl_handle, received_coap_header,this);
        } else if(COAP_MSG_CODE_REQUEST_PUT == received_coap_header->msg_code) {
            coap_response = base->handle_put_request(_nsdl_handle, received_coap_header, this, execute_value_updated);
        } else if(COAP_MSG_CODE_REQUEST_POST == received_coap_header->msg_code) {
            if(base->base_type() == M2MBase::ResourceInstance) {
                msg_code = COAP_MSG_CODE_RESPONSE_BAD_REQUEST;
            } else {
                coap_response = base->handle_post_request(_nsdl_handle, received_coap_header,this, execute_value_updated);
            }
        } else if(COAP_MSG_CODE_REQUEST_DELETE == received_coap_header->msg_code) {
            // Delete the object instance
            tr_debug("M2MNsdlInterface::resource_callback() - DELETE the object instance");
            M2MBase::BaseType type = base->base_type();
            if(M2MBase::ObjectInstance == type) {
                M2MBase* base_object = find_resource(base->name());
                if(base_object) {
                    M2MObject *object = static_cast<M2MObject*> (base_object);
                    int slash_found = resource_name.find_last_of('/');
                    // Object instance validty checks done in upper level, no need for error handling
                    if(slash_found != -1) {
                        String object_name;
                        object_name = resource_name.substr(slash_found + 1, resource_name.length());
                        if (object->remove_object_instance(strtoul(
                                object_name.c_str(),
                                NULL,
                                10))) {
                            msg_code = COAP_MSG_CODE_RESPONSE_DELETED;
                        }
                    }
                }
            } else {
                msg_code = COAP_MSG_CODE_RESPONSE_BAD_REQUEST; // 4.00
            }
        } else if(COAP_MSG_TYPE_RESET == received_coap_header->msg_type) {
            // Cancel ongoing observation
            tr_error("M2MNsdlInterface::resource_callback() - RESET message");
            M2MBase::BaseType type = base->base_type();
            switch (type) {
                case M2MBase::Object:
                    base->remove_observation_level(M2MBase::O_Attribute);
                    break;
                case M2MBase::Resource:
                    base->remove_observation_level(M2MBase::R_Attribute);
                    break;
                case M2MBase::ObjectInstance:
                    base->remove_observation_level(M2MBase::OI_Attribute);
                    break;
                default:
                    break;
            }
            base->set_under_observation(false, this);
        }
    } else  {
        tr_debug("M2MNsdlInterface::resource_callback() - Resource NOT FOUND");
        msg_code = COAP_MSG_CODE_RESPONSE_BAD_REQUEST; // 4.00
    }
    if(!coap_response) {
            coap_response = sn_nsdl_build_response(_nsdl_handle,
                                                   received_coap_header,
                                                   msg_code);
    }

    if(coap_response) {
        tr_debug("M2MNsdlInterface::resource_callback() - send CoAP response");
        (sn_nsdl_send_coap_message(_nsdl_handle, address, coap_response) == 0) ? result = 0 : result = 1;
        if(coap_response->payload_ptr) {
            free(coap_response->payload_ptr);
            coap_response->payload_ptr = NULL;
        }
        sn_nsdl_release_allocated_coap_msg_mem(_nsdl_handle, coap_response);
    }
    if (execute_value_updated) {
        value_updated(base,base->uri_path());
    }

    return result;
}

bool M2MNsdlInterface::process_received_data(uint8_t *data,
                                             uint16_t data_size,
                                             sn_nsdl_addr_s *address)
{
    tr_debug("M2MNsdlInterface::process_received_data( data size %d)", data_size);
    return (0 == sn_nsdl_process_coap(_nsdl_handle,
                                      data,
                                      data_size,
                                      address)) ? true : false;
}

void M2MNsdlInterface::stop_timers()
{
    tr_debug("M2MNsdlInterface::stop_timers()");
    if(_registration_timer) {
        _registration_timer->stop_timer();
    }
    if (_nsdl_exceution_timer) {
        _nsdl_exceution_timer->stop_timer();
    }
    _bootstrap_id = 0;
    _register_ongoing = false;
    _unregister_ongoing = false;
    _update_register_ongoing = false;
}

void M2MNsdlInterface::timer_expired(M2MTimerObserver::Type type)
{
    if(M2MTimerObserver::NsdlExecution == type) {
        sn_nsdl_exec(_nsdl_handle, _counter_for_nsdl);
        _counter_for_nsdl++;
    } else if(M2MTimerObserver::Registration == type) {
        tr_debug("M2MNsdlInterface::timer_expired - M2MTimerObserver::Registration - Send update registration");
        send_update_registration();
    }
}

void M2MNsdlInterface::observation_to_be_sent(M2MBase *object,
                                              uint16_t obs_number,
                                              m2m::Vector<uint16_t> changed_instance_ids,
                                              bool send_object)
{
    __mutex_claim();
    tr_debug("M2MNsdlInterface::observation_to_be_sent(), %s", object->uri_path().c_str());
    if(object) {
        M2MBase::BaseType type = object->base_type();
        if(type == M2MBase::Object) {
            send_object_observation(static_cast<M2MObject*> (object),
                                    obs_number,
                                    changed_instance_ids,
                                    send_object);
        } else if(type == M2MBase::ObjectInstance) {
            send_object_instance_observation(static_cast<M2MObjectInstance*> (object), obs_number);
        } else if(type == M2MBase::Resource) {
            send_resource_observation(static_cast<M2MResource*> (object), obs_number);
        }
    }
    __mutex_release();
}

void M2MNsdlInterface::send_delayed_response(M2MBase *base)
{
    __mutex_claim();
    tr_debug("M2MNsdlInterface::send_delayed_response()");
    M2MResource *resource = NULL;
    if(base) {
        if(M2MBase::Resource == base->base_type()) {
            resource = static_cast<M2MResource *> (base);
        }
        if(resource) {
            sn_coap_hdr_s * coap_response = static_cast<sn_coap_hdr_s *>(malloc(sizeof(sn_coap_hdr_s)));
            if(coap_response) {
                memset(coap_response,0,sizeof(sn_coap_hdr_s));

                coap_response->msg_type = COAP_MSG_TYPE_CONFIRMABLE;
                coap_response->msg_code = COAP_MSG_CODE_RESPONSE_CONTENT;
                resource->get_delayed_token(coap_response->token_ptr,coap_response->token_len);

                uint32_t length = 0;
                resource->get_value(coap_response->payload_ptr, length);
                coap_response->payload_len = length;

                sn_nsdl_send_coap_message(_nsdl_handle, _nsdl_handle->nsp_address_ptr->omalw_address_ptr, coap_response);

                if(coap_response->payload_ptr) {
                   free(coap_response->payload_ptr);
                   coap_response->payload_ptr = NULL;
                }
                if(coap_response->token_ptr) {
                    free(coap_response->token_ptr);
                    coap_response->token_ptr = NULL;
                }
                free(coap_response);
            }
        }
    }
    __mutex_release();
}

void M2MNsdlInterface::resource_to_be_deleted(const String &resource_name)
{
    __mutex_claim();
    tr_debug("M2MNsdlInterface::resource_to_be_deleted(resource_name %s)", resource_name.c_str());
    delete_nsdl_resource(resource_name);
    __mutex_release();
}

void M2MNsdlInterface::value_updated(M2MBase *base,
                                     const String &object_name)
{
    tr_debug("M2MNsdlInterface::value_updated()");
    if(base) {
        switch(base->base_type()) {
            case M2MBase::Object:
                create_nsdl_object_structure(static_cast<M2MObject*> (base));
            break;
            case M2MBase::ObjectInstance:
                create_nsdl_object_instance_structure(static_cast<M2MObjectInstance*> (base));
            break;
            case M2MBase::Resource: {
                    M2MResource* resource = static_cast<M2MResource*> (base);
                    create_nsdl_resource_structure(resource,object_name,
                                               resource->supports_multiple_instances());
            }
            break;
            case M2MBase::ResourceInstance: {
                M2MResourceInstance* instance = static_cast<M2MResourceInstance*> (base);
                create_nsdl_resource(instance,object_name);
            }
            break;
        }
    }

    if (base->is_value_updated_function_set()) {
        base->execute_value_updated(base->name());
    }
    else {
        _observer.value_updated(base);
    }
}

void M2MNsdlInterface::remove_object(M2MBase *object)
{
    __mutex_claim();
    tr_debug("M2MNsdlInterface::remove_object()");
    M2MObject* rem_object = static_cast<M2MObject*> (object);
    if(rem_object && !_object_list.empty()) {
        M2MObjectList::const_iterator it;
        it = _object_list.begin();
        int index = 0;
        for ( ; it != _object_list.end(); it++, index++ ) {
            if((*it) == rem_object) {
                _object_list.erase(index);
                break;
            }
        }
    }
    if(_object_list.empty()) {
        _object_list.clear();
    }
    __mutex_release();
}

bool M2MNsdlInterface::create_nsdl_object_structure(M2MObject *object)
{
    tr_debug("M2MNsdlInterface::create_nsdl_object_structure()");
    bool success = false;
    if(object) {
        M2MObjectInstanceList instance_list = object->instances();
        tr_debug("M2MNsdlInterface::create_nsdl_object_structure - Object Instance count %d", instance_list.size());
        if(!instance_list.empty()) {
           M2MObjectInstanceList::const_iterator it;
           it = instance_list.begin();
           for ( ; it != instance_list.end(); it++ ) {
               // Create NSDL structure for all object instances inside
               success = create_nsdl_object_instance_structure(*it);
           }
        }
    }
    if((object->operation() != M2MBase::NOT_ALLOWED)) {
        success = create_nsdl_resource(object,object->name(),object->register_uri());
    }
    return success;
}

bool M2MNsdlInterface::create_nsdl_object_instance_structure(M2MObjectInstance *object_instance)
{
    tr_debug("M2MNsdlInterface::create_nsdl_object_instance_structure()");
    bool success = false;
    if( object_instance) {

        // Append object instance id to the object name.
        String object_name = object_instance->name();
        object_name.push_back('/');
        object_name.append_int(object_instance->instance_id());

        M2MResourceList res_list = object_instance->resources();
        tr_debug("M2MNsdlInterface::create_nsdl_object_instance_structure - ResourceBase count %d", res_list.size());
        if(!res_list.empty()) {
            M2MResourceList::const_iterator it;
            it = res_list.begin();
            for ( ; it != res_list.end(); it++ ) {
                // Create NSDL structure for all resources inside
                success = create_nsdl_resource_structure(*it,object_name,
                                                         (*it)->supports_multiple_instances());
            }
        }
        if(object_instance->operation() != M2MBase::NOT_ALLOWED) {
            success = create_nsdl_resource(object_instance,object_name,object_instance->register_uri());
        }
    }
    return success;
}

bool M2MNsdlInterface::create_nsdl_resource_structure(M2MResource *res,
                                                      const String &object_name,
                                                      bool multiple_instances)
{
    tr_debug("M2MNsdlInterface::create_nsdl_resource_structure(object_name %s)", object_name.c_str());
    bool success = false;
    if(res) {
        // Append object name to the resource.
        // Take out the instance Id and append to the
        // resource name like "object/0/+ resource + / + 0"
        String res_name = object_name;
        if (strcmp(res_name.c_str(), res->uri_path().c_str()) != 0) {
            res_name.push_back('/');
            res_name.append(res->name().c_str(),res->name().length());
        }

        // if there are multiple instances supported
        // then add instance Id into creating resource path
        // else normal /object_id/object_instance/resource_id format.
        if(multiple_instances) {
            M2MResourceInstanceList res_list = res->resource_instances();
            tr_debug("M2MNsdlInterface::create_nsdl_resource_structure - ResourceInstance count %d", res_list.size());
            if(!res_list.empty()) {
                M2MResourceInstanceList::const_iterator it;
                it = res_list.begin();
                for ( ; it != res_list.end(); it++ ) {
                    String inst_name = res_name;
                    // Create NSDL structure for all resources inside
                    inst_name.push_back('/');
                    inst_name.append_int((*it)->instance_id());

                    success = create_nsdl_resource((*it),inst_name,(*it)->register_uri());
                }
                // Register the main Resource as well along with ResourceInstances
                success = create_nsdl_resource(res,res_name,res->register_uri());
            }
        } else {
            tr_debug("M2MNsdlInterface::create_nsdl_resource_structure - res_name %s", res_name.c_str());
            success = create_nsdl_resource(res,res_name,res->register_uri());
        }
    }
    return success;
}

bool M2MNsdlInterface::create_nsdl_resource(M2MBase *base, const String &name, bool publish_uri)
{
    __mutex_claim();
    tr_debug("M2MNsdlInterface::create_nsdl_resource(name %s)", name.c_str());
    bool success = false;
    uint8_t* buffer = 0;
    uint32_t length = 0;

    // Create the NSDL Resource Pointer...
    if(base) {
        sn_nsdl_resource_info_s* resource = sn_nsdl_get_resource(_nsdl_handle,
                                                                 name.length(),
                                                                 (uint8_t*)name.c_str());

        if(resource) {
            bool changed = false;
            success = true;
            if(resource->mode == SN_GRS_STATIC) {
                if((M2MBase::Resource == base->base_type() ||
                   M2MBase::ResourceInstance == base->base_type()) &&
                   M2MBase::Static == base->mode()) {
                    M2MResourceInstance *res = (M2MResourceInstance*) base;
                    res->get_value(buffer,length);
                    if(resource->resource) {
                        memory_free(resource->resource);
                    }
                    resource->resource = buffer;
                    resource->resourcelen = length;
                    resource->publish_uri = publish_uri;
                }
            }
            // Check if the access level for the resource has changed.
            if(resource->access != (sn_grs_resource_acl_e)base->operation()) {
                changed = true;
                resource->access = (sn_grs_resource_acl_e)base->operation();
            }
            if(resource->resource_parameters_ptr) {
                // Check if the observation parameter for the resource has changed.
                if(resource->resource_parameters_ptr->observable != (uint8_t)base->is_observable()) {
                    changed = true;
                    resource->resource_parameters_ptr->observable = (uint8_t)base->is_observable();
                }
            }
            if(changed && resource->resource_parameters_ptr) {
                resource->resource_parameters_ptr->registered = SN_NDSL_RESOURCE_NOT_REGISTERED;
            }
        } else if(_resource) {
            base->set_under_observation(false,this);
            //TODO: implement access control
            // Currently complete access is given
            _resource->access = (sn_grs_resource_acl_e)base->operation();

            if((M2MBase::Resource == base->base_type() ||
                M2MBase::ResourceInstance == base->base_type()) &&
               M2MBase::Static == base->mode()) {
                M2MResourceInstance *res = (M2MResourceInstance*)base;
                // Static resource is updated
                _resource->mode = SN_GRS_STATIC;

                res->get_value(buffer,length);
                _resource->resource = buffer;
                _resource->resourcelen = length;
            }

            if(M2MBase::Dynamic == base->mode()){
              // Dynamic resource is updated
               _resource->mode = SN_GRS_DYNAMIC;
               _resource->sn_grs_dyn_res_callback = __nsdl_c_callback;
            }

            if( _resource->path != NULL ){
                memory_free(_resource->path);
                _resource->path = NULL;
            }
            if(name.length() > 0 ){
                _resource->path = alloc_string_copy((uint8_t*)name.c_str(), name.length());
                if(_resource->path) {
                    _resource->pathlen = name.length();
                }
            }
            if(!base->resource_type().empty() && _resource->resource_parameters_ptr) {
                _resource->resource_parameters_ptr->resource_type_ptr =
                    alloc_string_copy((uint8_t*)base->resource_type().c_str(),
                          base->resource_type().length());
                if(_resource->resource_parameters_ptr->resource_type_ptr) {
                    _resource->resource_parameters_ptr->resource_type_len =
                           base->resource_type().length();
                }
            }
            if(!base->interface_description().empty() && _resource->resource_parameters_ptr) {
                _resource->resource_parameters_ptr->interface_description_ptr =
                    alloc_string_copy((uint8_t*)base->interface_description().c_str(),
                          base->interface_description().length());
                if(_resource->resource_parameters_ptr->interface_description_ptr) {
                    _resource->resource_parameters_ptr->interface_description_len =
                           base->interface_description().length();
                 }
            }
            if(_resource->resource_parameters_ptr) {
                _resource->resource_parameters_ptr->coap_content_type = base->coap_content_type();
                _resource->resource_parameters_ptr->observable = (uint8_t)base->is_observable();
            }
            _resource->publish_uri = publish_uri;
            int8_t result = sn_nsdl_create_resource(_nsdl_handle,_resource);
            tr_debug("M2MNsdlInterface::create_nsdl_resource - Creating in NSDL-C result %d", result);

            // Either the resource is created or it already
            // exists , then result is success.
            if (result == 0 ||
               result == -2){
                success = true;
            }

            if(_resource->path) {
                memory_free(_resource->path);
            }
            if(_resource->resource_parameters_ptr->resource_type_ptr){
                memory_free(_resource->resource_parameters_ptr->resource_type_ptr);
            }
            if(_resource->resource_parameters_ptr->interface_description_ptr){
                memory_free(_resource->resource_parameters_ptr->interface_description_ptr);
            }
            if (_resource->resource) {
                memory_free(_resource->resource);
            }

            //Clear up the filled resource to fill up new resource.
            clear_resource(_resource);

            if(success) {
               base->set_under_observation(false,this);
            }
        }
    }
    __mutex_release();
    return success;
}

// convenience method to get the URI from its buffer field...
String M2MNsdlInterface::coap_to_string(uint8_t *coap_data,int coap_data_length)
{
    String value = "";
    if (coap_data != NULL && coap_data_length > 0) {
        value.append_raw((char *)coap_data,coap_data_length);
    }
    return value;
}

uint64_t M2MNsdlInterface::registration_time()
{
    uint64_t value = 0;
    if(_endpoint->lifetime_ptr) {
        value = atol((const char*)_endpoint->lifetime_ptr);
    }

    if(value >= OPTIMUM_LIFETIME) {
        value = value - REDUCE_LIFETIME;
    } else {
        value = REDUCTION_FACTOR * value;
    }
    tr_debug("M2MNsdlInterface::registration_time - value (in seconds) %ld", value);
    return value;
}

M2MBase* M2MNsdlInterface::find_resource(const String &object_name)
{
    M2MBase *object = NULL;
    if(!_object_list.empty()) {
        M2MObjectList::const_iterator it;
        it = _object_list.begin();
        for ( ; it != _object_list.end(); it++ ) {
            if((*it)->name() == object_name) {
                object = (*it);
                tr_debug("M2MNsdlInterface::find_resource(%s) found", object_name.c_str());
                break;
            }
            object = find_resource((*it),object_name);
            if(object != NULL) {
                tr_debug("M2MNsdlInterface::find_resource(%s) found", object_name.c_str());
                break;
            }
        }
    }
    return object;
}

M2MBase* M2MNsdlInterface::find_resource(const M2MObject *object,
                                         const String &object_instance)
{
    M2MBase *instance = NULL;
    if(object) {
        M2MObjectInstanceList list = object->instances();
        if(!list.empty()) {
            M2MObjectInstanceList::const_iterator it;
            it = list.begin();
            for ( ; it != list.end(); it++ ) {
                // Append object instance id to the object name.
                String name = (*it)->name();
                name.push_back('/');
                name.append_int((*it)->instance_id());

                if(name == object_instance){
                    instance = (*it);
                    break;
                }
                instance = find_resource((*it),object_instance);
                if(instance != NULL){
                    break;
                }
            }
        }
    }
    return instance;
}

M2MBase* M2MNsdlInterface::find_resource(const M2MObjectInstance *object_instance,
                                         const String &resource_instance)
{
    M2MBase *instance = NULL;
    if(object_instance) {
        M2MResourceList list = object_instance->resources();
        if(!list.empty()) {
            M2MResourceList::const_iterator it;
            it = list.begin();
            for ( ; it != list.end(); it++ ) {
                String name = object_instance->name();
                // Append object instance id to the object name.
                name.push_back('/');
                name.append_int(object_instance->instance_id());

                name.push_back('/');
                name+= (*it)->name();

                if(name == resource_instance) {
                    instance = *it;
                    break;
                } else if((*it)->supports_multiple_instances()) {
                    instance = find_resource((*it),name, resource_instance);
                    if(instance != NULL){
                        break;
                    }
                }
            }
        }
    }
    return instance;
}

M2MBase* M2MNsdlInterface::find_resource(const M2MResource *resource,
                                         const String &object_name,
                                         const String &resource_instance)
{
    M2MBase *res = NULL;
    if(resource) {
        if(resource->supports_multiple_instances()) {
            M2MResourceInstanceList list = resource->resource_instances();
            if(!list.empty()) {
                M2MResourceInstanceList::const_iterator it;
                it = list.begin();
                for ( ; it != list.end(); it++ ) {
                    String name = object_name;
                    // if there are multiple instances supported
                    // then add instance Id into creating resource path
                    // else normal /object_id/object_instance/resource_id format.

                    name.push_back('/');
                    name.append_int((*it)->instance_id());

                    if(name == resource_instance){
                        res = (*it);
                        break;
                    }
                }
            }
        }
    }
    return res;
}

bool M2MNsdlInterface::object_present(M2MObject* object) const
{
    bool success = false;
    if(object && !_object_list.empty()) {
        M2MObjectList::const_iterator it;
        it = _object_list.begin();
        for ( ; it != _object_list.end(); it++ ) {
            if((*it) == object) {
                success = true;
                break;
            }
        }
    }
    return success;
}

bool M2MNsdlInterface::add_object_to_list(M2MObject* object)
{
    bool success = false;
    if(object && !object_present(object)) {
        _object_list.push_back(object);
        success = true;
    }
    return success;
}

void M2MNsdlInterface::clear_resource(sn_nsdl_resource_info_s *&resource)
{
    //Clear up the filled resource to fill up new resource.
    if(resource && resource->resource_parameters_ptr) {
        sn_nsdl_resource_parameters_s *temp_resource_parameter = resource->resource_parameters_ptr;
        memset(resource->resource_parameters_ptr, 0, sizeof(sn_nsdl_resource_parameters_s));
        memset(resource,0, sizeof(sn_nsdl_resource_info_s));
        resource->resource_parameters_ptr = temp_resource_parameter;
    }
}

M2MInterface::Error M2MNsdlInterface::interface_error(sn_coap_hdr_s *coap_header)
{
    M2MInterface::Error error = M2MInterface::ErrorNone;
    if(coap_header) {
        switch(coap_header->msg_code) {
            case COAP_MSG_CODE_RESPONSE_BAD_REQUEST:
            case COAP_MSG_CODE_RESPONSE_BAD_OPTION:
            case COAP_MSG_CODE_RESPONSE_REQUEST_ENTITY_INCOMPLETE:
            case COAP_MSG_CODE_RESPONSE_PRECONDITION_FAILED:
            case COAP_MSG_CODE_RESPONSE_REQUEST_ENTITY_TOO_LARGE:
            case COAP_MSG_CODE_RESPONSE_UNSUPPORTED_CONTENT_FORMAT:
                error = M2MInterface::InvalidParameters;
                break;
            case COAP_MSG_CODE_RESPONSE_UNAUTHORIZED:
            case COAP_MSG_CODE_RESPONSE_FORBIDDEN:
            case COAP_MSG_CODE_RESPONSE_NOT_ACCEPTABLE:
            case COAP_MSG_CODE_RESPONSE_NOT_FOUND:
            case COAP_MSG_CODE_RESPONSE_METHOD_NOT_ALLOWED:
                error = M2MInterface::NotAllowed;
                break;
            case COAP_MSG_CODE_RESPONSE_CREATED:
            case COAP_MSG_CODE_RESPONSE_DELETED:
            case COAP_MSG_CODE_RESPONSE_VALID:
            case COAP_MSG_CODE_RESPONSE_CHANGED:
            case COAP_MSG_CODE_RESPONSE_CONTENT:
                error = M2MInterface::ErrorNone;
                break;
            default:
                error = M2MInterface::UnknownError;
                break;
        }
        if(coap_header->coap_status == COAP_STATUS_BUILDER_MESSAGE_SENDING_FAILED) {
            error = M2MInterface::NetworkError;
        }
    }
    return error;
}

void M2MNsdlInterface::send_object_observation(M2MObject *object,
                                               uint16_t obs_number,
                                               m2m::Vector<uint16_t> changed_instance_ids,
                                               bool send_object)
{
    tr_debug("M2MNsdlInterface::send_object_observation");
    if(object) {
        uint8_t *value = 0;
        uint32_t length = 0;
        uint8_t *token = 0;
        uint32_t token_length = 0;

        M2MTLVSerializer serializer;
        // Send whole object structure
        if (send_object) {
            value = serializer.serialize(object->instances(), length);
        }
        // Send only changed object instances
        else {
            M2MObjectInstanceList list;
            Vector<uint16_t>::const_iterator it;
            it = changed_instance_ids.begin();
            for (; it != changed_instance_ids.end(); it++){
                M2MObjectInstance* obj_instance = object->object_instance(*it);
                if (obj_instance){
                    list.push_back(obj_instance);
                }
            }
            if (!list.empty()) {
                value = serializer.serialize(list, length);
                list.clear();
            }
        }

        object->get_observation_token(token,token_length);

        send_notification(token,
                          token_length,
                          value,
                          length,
                          obs_number,
                          object->max_age(),
                          object->coap_content_type(),
                          object->uri_path());

        memory_free(value);
        memory_free(token);
    }
}

void M2MNsdlInterface::send_object_instance_observation(M2MObjectInstance *object_instance,
                                                        uint16_t obs_number)
{
    tr_debug("M2MNsdlInterface::send_object_instance_observation");
    if(object_instance) {
        uint8_t *value = 0;
        uint32_t length = 0;
        uint8_t *token = 0;
        uint32_t token_length = 0;

        M2MTLVSerializer serializer;
        value = serializer.serialize(object_instance->resources(), length);

        object_instance->get_observation_token(token,token_length);

        send_notification(token,
                          token_length,
                          value,
                          length,
                          obs_number,
                          object_instance->max_age(),
                          object_instance->coap_content_type(),
                          object_instance->uri_path());

        memory_free(value);
        memory_free(token);
    }
}

void M2MNsdlInterface::send_resource_observation(M2MResource *resource,
                                                 uint16_t obs_number)
{
    tr_debug("M2MNsdlInterface::send_resource_observation");
    if(resource) {
        uint8_t *value = 0;
        uint32_t length = 0;
        uint8_t *token = 0;
        uint32_t token_length = 0;

        resource->get_observation_token(token,token_length);
        uint8_t content_type = 0;
        if(M2MResourceInstance::OPAQUE == resource->resource_instance_type()) {
            content_type = COAP_CONTENT_OMA_OPAQUE_TYPE;
        }
        if (resource->resource_instance_count() > 0) {
            content_type = COAP_CONTENT_OMA_TLV_TYPE;
            M2MTLVSerializer serializer;
            value = serializer.serialize(resource, length);
        } else {
            resource->get_value(value,length);
        }
        send_notification(token,
                          token_length,
                          value,
                          length,
                          obs_number,
                          resource->max_age(),
                          content_type,
                          resource->uri_path());

        memory_free(value);
        memory_free(token);
    }
}

void M2MNsdlInterface::build_observation_number(uint8_t *obs_number,
                                                uint8_t *obs_len,
                                                uint16_t number)
{
    if(number > 0xFF) {
        *obs_len = 2;
        *(obs_number) = (number >> 8) & 0x00FF;
        obs_number[1] = number & 0x00FF;
    } else {
        *obs_len = 1;
        *(obs_number) = number & 0x00FF;
    }
}

void M2MNsdlInterface::send_notification(uint8_t *token,
                                         uint8_t  token_length,
                                         uint8_t *value,
                                         uint32_t value_length,
                                         uint16_t observation,
                                         uint32_t max_age,
                                         uint8_t  coap_content_type,
                                         const String  &uri_path)

{
    tr_debug("M2MNsdlInterface::send_notification");
    sn_coap_hdr_s *notification_message_ptr;

    /* Allocate and initialize memory for header struct */
    notification_message_ptr = static_cast<sn_coap_hdr_s *>(memory_alloc(sizeof(sn_coap_hdr_s)));
    if (notification_message_ptr) {
        memset(notification_message_ptr, 0, sizeof(sn_coap_hdr_s));

        notification_message_ptr->options_list_ptr = (sn_coap_options_list_s *)memory_alloc(sizeof(sn_coap_options_list_s));
        if (notification_message_ptr->options_list_ptr) {

            memset(notification_message_ptr->options_list_ptr , 0, sizeof(sn_coap_options_list_s));

            /* Fill header */
            notification_message_ptr->msg_type = COAP_MSG_TYPE_CONFIRMABLE;
            notification_message_ptr->msg_code = COAP_MSG_CODE_RESPONSE_CONTENT;

            /* Fill token */
            notification_message_ptr->token_len = token_length;
            notification_message_ptr->token_ptr = token;

            /* Fill payload */
            notification_message_ptr->payload_len = value_length;
            notification_message_ptr->payload_ptr = value;

            /* Fill uri path */
            notification_message_ptr->uri_path_len = uri_path.size();
            notification_message_ptr->uri_path_ptr = (uint8_t *)uri_path.c_str();

            /* Fill observe */
            uint8_t observation_number[2];
            uint8_t observation_number_length = 0;

            build_observation_number(observation_number,
                                     &observation_number_length,
                                     observation);
            notification_message_ptr->options_list_ptr->observe_len = observation_number_length;
            notification_message_ptr->options_list_ptr->observe_ptr = observation_number;

            notification_message_ptr->options_list_ptr->max_age_ptr =
                    m2m::String::convert_integer_to_array(max_age,
                                                          notification_message_ptr->options_list_ptr->max_age_len);

            notification_message_ptr->content_type_ptr =
                    m2m::String::convert_integer_to_array(coap_content_type,
                                                          notification_message_ptr->content_type_len);

            /* Send message */
            sn_nsdl_send_coap_message(_nsdl_handle,
                                      _nsdl_handle->nsp_address_ptr->omalw_address_ptr,
                                      notification_message_ptr);

            /* Free memory */
            notification_message_ptr->uri_path_ptr = NULL;
            notification_message_ptr->payload_ptr = NULL;
            notification_message_ptr->options_list_ptr->observe_ptr = NULL;
            notification_message_ptr->token_ptr = NULL;
            if (notification_message_ptr->content_type_ptr) {
                free(notification_message_ptr->content_type_ptr);
            }
            notification_message_ptr->content_type_ptr = NULL;
            if (notification_message_ptr->options_list_ptr->max_age_ptr) {
                free(notification_message_ptr->options_list_ptr->max_age_ptr);
            }
            notification_message_ptr->options_list_ptr->max_age_ptr = NULL;
        }
        sn_nsdl_release_allocated_coap_msg_mem(_nsdl_handle, notification_message_ptr);
    }
}

nsdl_s * M2MNsdlInterface::get_nsdl_handle()
{
    return _nsdl_handle;
}

void M2MNsdlInterface::handle_bootstrap_put_message(sn_coap_hdr_s *coap_header,
                                                sn_nsdl_addr_s *address) {
#ifndef M2M_CLIENT_DISABLE_BOOTSTRAP_FEATURE
    tr_debug("M2MNsdlInterface::handle_bootstrap_message");
    uint8_t response_code = COAP_MSG_CODE_RESPONSE_CHANGED;
    sn_coap_hdr_s *coap_response = NULL;
    bool success = false;
    bool security_object = false;
    uint16_t content_type = 0;

    if (!_security) {
        _security = new M2MSecurity(M2MSecurity::M2MServer);
    }

    String resource_name = coap_to_string(coap_header->uri_path_ptr,
                                          coap_header->uri_path_len);
    tr_debug("M2MNsdlInterface::handle_bootstrap_message - uri %s", resource_name.c_str());

    // Check incoming object
    if (resource_name.compare(0,1,"0") == 0) {
        security_object = true;
        if(_security) {
            success = true;
            // Not mandatory resource that's why it must be created first
            _security->create_resource(M2MSecurity::ShortServerID, 1);
            // Change operation mode
            M2MResourceList list = _security->object_instance()->resources();
            if(!list.empty()) {
                M2MResourceList::const_iterator it;
                it = list.begin();
                for ( ; it != list.end(); it++ ) {
                    (*it)->set_operation(M2MBase::PUT_ALLOWED);
                }
            }
        }
    }
    else if (resource_name.compare(0,1,"1") == 0) {
        success = true;
    }

    if (success) {
        // Send delayed response if token is part of the message
        if (coap_header->token_ptr) {
            tr_debug("M2MNsdlInterface::handle_bootstrap_message - send delayed response");
            coap_response = sn_nsdl_build_response(_nsdl_handle,
                                                   coap_header,
                                                   COAP_MSG_CODE_EMPTY);
            if (coap_response) {
                coap_response->msg_type = COAP_MSG_TYPE_ACKNOWLEDGEMENT;
                sn_nsdl_send_coap_message(_nsdl_handle, address, coap_response);
                sn_nsdl_release_allocated_coap_msg_mem(_nsdl_handle, coap_response);
            }
        }

        if(coap_header->content_type_ptr) {
            content_type = String::convert_array_to_integer(coap_header->content_type_ptr,
                                                            coap_header->content_type_len);
        }

        tr_debug("M2MNsdlInterface::handle_bootstrap_message - content_type %d", content_type);
        if (content_type != COAP_CONTENT_OMA_TLV_TYPE) {
            success = false;
        }
        if (success) {
            success = parse_bootstrap_message(coap_header, security_object);
            // Set operation back to default ones
            if (_security) {
                M2MResourceList list = _security->object_instance()->resources();
                if(!list.empty()) {
                    M2MResourceList::const_iterator it;
                    it = list.begin();
                    for ( ; it != list.end(); it++ ) {
                        (*it)->set_operation(M2MBase::NOT_ALLOWED);
                    }
                }
            }
        }
    }

    if (!success) {
        response_code = COAP_MSG_CODE_RESPONSE_BAD_REQUEST;
        handle_bootstrap_error();
    }
    coap_response = sn_nsdl_build_response(_nsdl_handle,
                                           coap_header,
                                           response_code);
    if (coap_response) {
        sn_nsdl_send_coap_message(_nsdl_handle, address, coap_response);
        sn_nsdl_release_allocated_coap_msg_mem(_nsdl_handle, coap_response);
    }
#else
    (void) coap_header;
    (void) address;
#endif
}

bool M2MNsdlInterface::parse_bootstrap_message(sn_coap_hdr_s *coap_header, bool is_security_object)
{
#ifndef M2M_CLIENT_DISABLE_BOOTSTRAP_FEATURE
    tr_debug("M2MNsdlInterface::parse_bootstrap_put_message");
    M2MTLVDeserializer *deserializer = new M2MTLVDeserializer();
    bool ret = false;
    bool is_obj_instance = false;
    uint16_t instance_id = 0;
    if (deserializer && _security) {
        ret = is_obj_instance = deserializer->is_object_instance(coap_header->payload_ptr);
        if (!is_obj_instance) {
            ret = deserializer->is_resource(coap_header->payload_ptr);
        }

        if (ret) {
            M2MTLVDeserializer::Error error = M2MTLVDeserializer::None;
            if (is_obj_instance) {
                if (is_security_object) {
                    error = deserializer->deserialise_object_instances(coap_header->payload_ptr,
                                                               coap_header->payload_len,
                                                               *_security,
                                                               M2MTLVDeserializer::Put);
                    }
                else {
                    error = deserializer->deserialise_object_instances(coap_header->payload_ptr,
                                                               coap_header->payload_len,
                                                               *_server,
                                                               M2MTLVDeserializer::Put);
                }
            }
            else {
                if (is_security_object) {
                    instance_id = deserializer->instance_id(coap_header->payload_ptr);
                    error = deserializer->deserialize_resources(coap_header->payload_ptr,
                                                               coap_header->payload_len,
                                                               *_security->object_instance(instance_id),
                                                               M2MTLVDeserializer::Put);
                }
                else {
                    instance_id = deserializer->instance_id(coap_header->payload_ptr);
                    error = deserializer->deserialize_resources(coap_header->payload_ptr,
                                                               coap_header->payload_len,
                                                               *_server->object_instance(instance_id),
                                                               M2MTLVDeserializer::Post);
                }
            }

            if (error != M2MTLVDeserializer::None) {
                tr_error("M2MNsdlInterface::parse_bootstrap_put_message - error %d", error);
                ret = false;
            }
        }
    }
    delete deserializer;
    return ret;
#else
    (void) coap_header;
    (void) is_security_object;
    return false;
#endif
}

void M2MNsdlInterface::handle_bootstrap_finished(sn_coap_hdr_s *coap_header,sn_nsdl_addr_s *address)
{
#ifndef M2M_CLIENT_DISABLE_BOOTSTRAP_FEATURE
    String object_name = coap_to_string(coap_header->uri_path_ptr,
                                          coap_header->uri_path_len);
    tr_debug("M2MNsdlInterface::handle_bootstrap_finished - path: %s", object_name.c_str());
    sn_coap_hdr_s *coap_response = NULL;
    uint8_t msg_code = COAP_MSG_CODE_RESPONSE_CHANGED;

    // Accept only '/bs' path and check that needed data is in security object
    if (object_name.size() != 2 ||
            object_name.compare(0,2,BOOTSTRAP_URI) != 0 ||
            !validate_security_object()) {
        msg_code = COAP_MSG_CODE_RESPONSE_BAD_REQUEST;
    } else {
        // Add short server id to server object
        _server->set_resource_value(M2MServer::ShortServerID,
                                    _security->resource_value_int(M2MSecurity::ShortServerID));
    }

    coap_response = sn_nsdl_build_response(_nsdl_handle,
                                           coap_header,
                                           msg_code);
    if(coap_response) {
        sn_nsdl_send_coap_message(_nsdl_handle, address, coap_response);
        sn_nsdl_release_allocated_coap_msg_mem(_nsdl_handle, coap_response);
    }
    if (COAP_MSG_CODE_RESPONSE_CHANGED == msg_code) {
        // Switch back to original ep name
        if (_endpoint->endpoint_name_ptr) {
            memory_free(_endpoint->endpoint_name_ptr);
        }
        _endpoint->endpoint_name_ptr = alloc_string_copy((uint8_t*)_endpoint_name.c_str(), _endpoint_name.length());
        _endpoint->endpoint_name_len = _endpoint_name.length();
        _observer.bootstrap_done(_security);
    } else {
        handle_bootstrap_error();
    }
#else
    (void) coap_header;
    (void) address;
#endif
}

void M2MNsdlInterface::handle_bootstrap_delete(sn_coap_hdr_s *coap_header,sn_nsdl_addr_s *address)
{
#ifndef M2M_CLIENT_DISABLE_BOOTSTRAP_FEATURE
    sn_coap_hdr_s *coap_response = NULL;
    uint8_t msg_code = COAP_MSG_CODE_RESPONSE_DELETED;
    String object_name = coap_to_string(coap_header->uri_path_ptr,
                                          coap_header->uri_path_len);
    tr_debug("M2MNsdlInterface::handle_bootstrap_delete - obj %s", object_name.c_str());

    // Only following paths are accepted, 0, 0/0
    if (object_name.size() == 2 || object_name.size() > 3) {
        msg_code = COAP_MSG_CODE_RESPONSE_BAD_REQUEST;
    }
    else if ((object_name.size() == 1 && object_name.compare(0,1,"0") != 0) ||
            (object_name.size() == 3 && object_name.compare(0,3,"0/0") != 0)) {
        msg_code = COAP_MSG_CODE_RESPONSE_BAD_REQUEST;
    }

    coap_response = sn_nsdl_build_response(_nsdl_handle,
                                           coap_header,
                                           msg_code);

    if(coap_response) {
        sn_nsdl_send_coap_message(_nsdl_handle, address, coap_response);
        sn_nsdl_release_allocated_coap_msg_mem(_nsdl_handle, coap_response);
        if(_security) {
            _security->clear_resources();
        }
    } else {
        handle_bootstrap_error();
    }
#else
    (void) coap_header;
    (void) address;
#endif
}

bool M2MNsdlInterface::validate_security_object()
{
#ifndef M2M_CLIENT_DISABLE_BOOTSTRAP_FEATURE
    tr_debug("M2MNsdlInterface::validate_security_object");
    if (_security) {
        String address = _security->resource_value_string(M2MSecurity::M2MServerUri);
        uint32_t sec_mode = _security->resource_value_int(M2MSecurity::SecurityMode);
        bool is_bs_server = _security->resource_value_int(M2MSecurity::BootstrapServer);
        uint32_t public_key_size = _security->get_resource(M2MSecurity::PublicKey)->value_length();
        uint32_t server_key_size = _security->get_resource(M2MSecurity::ServerPublicKey)->value_length();
        uint32_t pkey_size = _security->get_resource(M2MSecurity::Secretkey)->value_length();
        tr_debug("M2MNsdlInterface::validate_security_object - Server URI /0/0: %s", address.c_str());
        tr_debug("M2MNsdlInterface::validate_security_object - is bs server /0/1: %d", is_bs_server);
        tr_debug("M2MNsdlInterface::validate_security_object - Security Mode /0/2: %d", sec_mode);
        tr_debug("M2MNsdlInterface::validate_security_object - Public key size /0/3: %d", public_key_size);
        tr_debug("M2MNsdlInterface::validate_security_object - Server Public key size /0/4: %d", server_key_size);
        tr_debug("M2MNsdlInterface::validate_security_object - Secret key size /0/5: %d", pkey_size);
        // Only NoSec and Certificate modes are supported
        if (!address.empty() && !is_bs_server) {
            if (M2MSecurity::Certificate == sec_mode) {
                if (!public_key_size || !server_key_size || !pkey_size) {
                    return false;
                } else {
                    return true;
                }
            } else if (M2MSecurity::NoSecurity == sec_mode){
                return true;
            } else {
                return false;
            }
        } else {
            return false;
        }
    }
    return false;
#else
    return false;
#endif
}

void M2MNsdlInterface::handle_bootstrap_error()
{
    tr_debug("M2MNsdlInterface::handle_bootstrap_error()");
    if (_security) {
        delete _security;
        _security = NULL;
    }
    _observer.bootstrap_error();
}

const String& M2MNsdlInterface::endpoint_name() const
{
    return _endpoint_name;
}
