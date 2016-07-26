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
#include <unistd.h>
#include <malloc.h>
#include <stdio.h>
#include <stdarg.h>
#include <pthread.h>
#include <signal.h> /* For SIGIGN and SIGINT */
#include "mbed-client/m2minterfacefactory.h"
#include "mbed-client/m2mdevice.h"
#include "mbed-client/m2minterfaceobserver.h"
#include "mbed-client/m2minterface.h"
#include "mbed-client/m2mobjectinstance.h"
#include "mbed-client/m2mresource.h"

#include "mbed-trace/mbed_trace.h"

const String &BOOTSTRAP_SERVER_ADDRESS = "coap://10.45.3.10:5693";
const String &M2M_SERVER_ADDRESS = "coap://10.45.3.10:5683";
const String &MANUFACTURER = "manufacturer";
const String &TYPE = "type";
const String &MODEL_NUMBER = "2015";
const String &SERIAL_NUMBER = "12345";

const uint8_t STATIC_VALUE[] = "Open Mobile Alliance";

static void ctrl_c_handle_function(void);
void close_function();
typedef void (*signalhandler_t)(int); /* Function pointer type for ctrl-c */

class MbedClient: public M2MInterfaceObserver {
public:
    MbedClient(){
        _security = NULL;
        _interface = NULL;
        _register_security = NULL;
        _device = NULL;
        _object = NULL;
        _bootstrapped = false;
        _error = false;
        _registered = false;
        _unregistered = false;
        _registration_updated = false;
        _value = 0;
    }

    ~MbedClient() {
        if(_security) {
            delete _security;
        }
        if(_register_security){
            delete _register_security;
        }
        if(_device) {
           M2MDevice::delete_instance();
            _device = NULL;
        }
        if(_object) {
            delete _object;
        }
        if(_interface) {
            delete _interface;
        }
    }

    bool create_interface() {
        _interface = M2MInterfaceFactory::create_interface(*this,
                                                  "linux-endpoint",
                                                  "test",
                                                  60,
                                                  5683,
                                                  "",
                                                  M2MInterface::UDP,
                                                  M2MInterface::LwIP_IPv4,
                                                  "");
        printf("Endpoint Name : linux-endpoint\n");
        return (_interface == NULL) ? false : true;
    }

    bool bootstrap_successful() {
        while(!_bootstrapped && !_error) {
            sleep(1);
        }
        return _bootstrapped;
    }

    bool register_successful() {
        while(!_registered && !_error) {
            sleep(1);
        }
        return _registered;
    }

    bool unregister_successful() {
        while(!_unregistered && !_error) {
            sleep(1);
        }
        return _unregistered;
    }

    bool registration_update_successful() {
        while(!_registration_updated && !_error) {
        }
        return _registration_updated;
    }

    bool create_bootstrap_object() {
        bool success = false;
        if(_security) {
            delete _security;
        }
        _security = M2MInterfaceFactory::create_security(M2MSecurity::Bootstrap);
        if(_security) {
            if(_security->set_resource_value(M2MSecurity::M2MServerUri, BOOTSTRAP_SERVER_ADDRESS) &&
            _security->set_resource_value(M2MSecurity::SecurityMode, M2MSecurity::NoSecurity)) {
                success = true;
                /* Not used now because there is no TLS or DTLS implementation available for stack.
                security->set_resource_value(M2MSecurity::ServerPublicKey,certificates->certificate_ptr[0],certificates->certificate_len[0]);
                security->set_resource_value(M2MSecurity::PublicKey,certificates->certificate_ptr[1],certificates->certificate_len[1]);
                security->set_resource_value(M2MSecurity::Secretkey,certificates->own_private_key_ptr,certificates->own_private_key_len);
                */
            }
        }
        printf("Bootstrap Server Address %s\n", BOOTSTRAP_SERVER_ADDRESS.c_str());
        return success;
    }

    bool create_register_object() {
        bool success = false;
        _register_security = M2MInterfaceFactory::create_security(M2MSecurity::M2MServer);
        if(_register_security) {
            if(_register_security->set_resource_value(M2MSecurity::M2MServerUri, M2M_SERVER_ADDRESS) &&            
            _register_security->set_resource_value(M2MSecurity::SecurityMode, M2MSecurity::NoSecurity)) {
                success = true;
                /* Not used now because there is no TLS or DTLS implementation available for stack.
                security->set_resource_value(M2MSecurity::ServerPublicKey,certificates->certificate_ptr[0],certificates->certificate_len[0]);
                security->set_resource_value(M2MSecurity::PublicKey,certificates->certificate_ptr[1],certificates->certificate_len[1]);
                security->set_resource_value(M2MSecurity::Secretkey,certificates->own_private_key_ptr,certificates->own_private_key_len);
                */
            }
        }
        return success;
    }

    void test_bootstrap() {
        _interface->bootstrap(_security);
    }

    bool create_device_object() {
        bool success = false;
        _device = M2MInterfaceFactory::create_device();
        if(_device) {
            _device->object_instance()->set_operation(M2MBase::GET_PUT_POST_ALLOWED);
            if(_device->create_resource(M2MDevice::Manufacturer,MANUFACTURER)     &&
               _device->create_resource(M2MDevice::DeviceType,TYPE)        &&
               _device->create_resource(M2MDevice::ModelNumber,MODEL_NUMBER)      &&
               _device->create_resource(M2MDevice::SerialNumber,SERIAL_NUMBER)) {
                success = true;
            }
        }
        return success;
    }

    void execute_function(void *argument) {
        if(argument) {
            char* arguments = (char*)argument;
            printf("Received %s!!\n", arguments);
        }
        printf("I am executed !!\n");
    }

    bool create_generic_object() {
        bool success = false;
        _object = M2MInterfaceFactory::create_object("10");
        if(_object) {
            _object->set_operation(M2MBase::GET_PUT_POST_ALLOWED);
            M2MObjectInstance* inst = _object->create_object_instance();
            if(inst) {
                inst->set_operation(M2MBase::GET_PUT_POST_ALLOWED);
                inst->set_observable(false);
                char buffer[20];
                int size = sprintf(buffer,"%d",_value);

                inst->create_static_resource("0",
                                             "ResourceTest",
                                             M2MResourceInstance::INTEGER,
                                             STATIC_VALUE,
                                             sizeof(STATIC_VALUE)-1);

                M2MResourceInstance* instance = inst->create_dynamic_resource_instance("1",
                                                                         "ResourceTest",
                                                                         M2MResourceInstance::INTEGER,
                                                                         true,0);

                if(instance) {
                    instance->set_operation(M2MBase::GET_PUT_POST_ALLOWED);
                    instance->set_value((const uint8_t*)buffer,
                                 (const uint32_t)size);
                    instance->set_execute_function(execute_callback(this,&MbedClient::execute_function));
                    _value++;
                }
            }
        }
        return success;
    }

    void update_resource() {
        if(_object) {
            M2MObjectInstance* inst = _object->object_instance();
            if(inst) {
                M2MResource* res = inst->resource("1");
                res = inst->resource("1");
                if(res) {
                    M2MResourceInstance *res_inst = res->resource_instance(0);
                    if(res_inst) {
                        char buffer1[20];
                        int size1 = sprintf(buffer1,"%d",_value);
                        res_inst->set_value((const uint8_t*)buffer1,
                                       (const uint32_t)size1);
                        _value++;
                    }
                }
            }
        }
    }

    void test_register(){
        M2MObjectList object_list;
        object_list.push_back(_device);
        object_list.push_back(_object);

        _interface->register_object(_register_security,object_list);
    }

    void test_update_register() {
        uint32_t updated_lifetime = 20;
        _registered = false;
        _unregistered = false;
        _interface->update_registration(_register_security,updated_lifetime);
    }

    void test_unregister() {
        _interface->unregister_object(NULL);
    }

    void bootstrap_done(M2MSecurity *server_object){
    if(server_object) {
            _register_security = server_object;
            _bootstrapped = true;
            printf("\nBootstrapped\n");
            printf("mDS Address %s\n",
            _register_security->resource_value_string(M2MSecurity::M2MServerUri).c_str());
        }
    }

    void object_registered(M2MSecurity */*security_object*/, const M2MServer &/*server_object*/){
        _registered = true;
        printf("\nRegistered\n");
    }

    void object_unregistered(M2MSecurity */*server_object*/){
        _unregistered = true;
        printf("\nUnregistered\n");
    }

    void registration_updated(M2MSecurity */*security_object*/, const M2MServer & /*server_object*/){
        _registration_updated = true;
        printf("\nregistration updated\n");

    }

    void error(M2MInterface::Error error){
        _error = true;
        close_function();
        printf("\nError occured Error Code : %d\n", (int8_t)error);

    }

    void value_updated(M2MBase *base, M2MBase::BaseType type) {
        printf("\nValue updated of Object name %s and Type %d\n",
               base->name().c_str(), type);
    }

private:

    M2MInterface        *_interface;
    M2MSecurity         *_security;
    M2MSecurity         *_register_security;
    M2MDevice           *_device;
    M2MObject           *_object;
    bool                _bootstrapped;
    bool                _error;
    bool                _registered;
    bool                _unregistered;
    bool                _registration_updated;
    int                 _value;
};

void* wait_for_bootstrap(void* arg) {
    MbedClient *client;
    client = (MbedClient*) arg;
    if(client->bootstrap_successful()) {
        printf("Registering endpoint\n");
        client->test_register();
    }
    return NULL;
}

void* wait_for_unregister(void* arg) {
    MbedClient *client;
    client = (MbedClient*) arg;
    if(client->unregister_successful()) {
        printf("Unregistered done --> exiting\n");
        close_function();
    }
    return NULL;
}

void* send_observation(void* arg) {
    MbedClient *client;
    client = (MbedClient*) arg;
    static uint8_t counter = 0;
    while(1) {
        sleep(1);
        if(counter >= 5 &&
           client->register_successful()) {
            printf("Sending observation\n");
            client->update_resource();
            counter = 0;
        }
        else
            counter++;
    }
    return NULL;
}

static MbedClient *m2mclient = NULL;

static void ctrl_c_handle_function(void)
{
    if(m2mclient && m2mclient->register_successful()) {
        printf("Unregistering endpoint\n");
        m2mclient->test_unregister();
    }
}

void trace_printer(const char* str)
{
  printf("%s\r\n", str);
}

static pthread_t bootstrap_thread;
static pthread_t unregister_thread;
static pthread_t observation_thread;

void close_function() {
    pthread_cancel(bootstrap_thread);
    pthread_cancel(unregister_thread);
    pthread_cancel(observation_thread);
}

int main() {

    MbedClient mbed_client;

    m2mclient = &mbed_client;

    mbed_trace_init();
    mbed_trace_print_function_set( trace_printer );
    mbed_trace_config_set(TRACE_MODE_COLOR|TRACE_ACTIVE_LEVEL_DEBUG|TRACE_CARRIAGE_RETURN);

    signal(SIGINT, (signalhandler_t)ctrl_c_handle_function);

    bool result = mbed_client.create_interface();
    if(true == result) {
        printf("\nInterface created\n");
    }
    result = mbed_client.create_bootstrap_object();
    if(true == result) {
        printf("Bootstrap object created");
    }

    result = mbed_client.create_register_object();
    if(true == result) {
        printf("Register object created");
    }

    result = mbed_client.create_device_object();
    if(true == result){
        printf("\nDevice object created !!\n");
    }

    result = mbed_client.create_generic_object();

    if(true == result) {
        printf("\nGeneric object created\n");
    }

//    printf("Bootstrapping endpoint\n");
//    mbed_client.test_bootstrap();

    printf("Registering endpoint\n");
    mbed_client.test_register();


    pthread_create(&bootstrap_thread, NULL, &wait_for_bootstrap, (void*) &mbed_client);
    pthread_create(&observation_thread, NULL, &send_observation, (void*) &mbed_client);
    pthread_create(&unregister_thread, NULL, &wait_for_unregister, (void*) &mbed_client);

    pthread_join(bootstrap_thread, NULL);
    pthread_join(unregister_thread, NULL);
    pthread_join(observation_thread, NULL);

    exit(EXIT_SUCCESS);
}

