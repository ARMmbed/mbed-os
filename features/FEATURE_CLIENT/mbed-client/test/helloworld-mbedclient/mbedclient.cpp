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

#include "mbedclient.h"

#include "mbed-client/m2minterfacefactory.h"
#include "mbed-client/m2mdevice.h"
#include "mbed-client/m2mobjectinstance.h"
#include "mbed-client/m2mresource.h"

#ifdef TARGET_LIKE_MBED
#include "minar/minar.h"
#include "test_env.h"
#endif

// Enter your mbed Device Server's IPv4 address and Port number in
// mentioned format like 192.168.0.1:5693
const String &BOOTSTRAP_SERVER_ADDRESS = "coap://10.45.3.10:5693";
const String &BOOTSTRAP_SERVER_DTLS_ADDRESS = "coap://10.45.3.10:5694";

#ifdef SIXLOWPAN_INTERFACE
const String &MBED_SERVER_ADDRESS = "coap://FD00:FF1:CE0B:A5E1:1068:AF13:9B61:D557:5683";
const String &MBED_SERVER_DTLS_ADDRESS = "coap://FD00:FF1:CE0B:A5E1:1068:AF13:9B61:D557:5684";
#else
const String &MBED_SERVER_ADDRESS = "coap://10.45.3.10:5683";
//const String &MBED_SERVER_ADDRESS = "coap://10.45.0.152:5683";
const String &MBED_SERVER_DTLS_ADDRESS = "coap://10.45.3.10:5684";
#endif
const String CLIENT_NAME = "secure-client";

const String &MANUFACTURER = "ARM";
const String &TYPE = "type";
const String &MODEL_NUMBER = "2015";
const String &SERIAL_NUMBER = "12345";
const uint8_t STATIC_VALUE[] = "Static value";

#ifdef TARGET_LIKE_LINUX
#include <unistd.h>
#else

#endif

MbedClient::MbedClient()
#ifndef TARGET_LIKE_LINUX
:_led(LED3)
#endif
{
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

MbedClient::~MbedClient() {
    if(_security) {
        delete _security;
    }
    if(_register_security){
        delete _register_security;
    }
    if(_device) {
        M2MDevice::delete_instance();
    }
    if(_object) {
        delete _object;
    }
    if(_interface) {
        delete _interface;
    }
}

bool MbedClient::create_interface() {

    M2MInterface::NetworkStack stack = M2MInterface::LwIP_IPv4;
    #ifdef SIXLOWPAN_INTERFACE
            stack = M2MInterface::Nanostack_IPv6;
    #endif

    /* From http://www.iana.org/assignments/port-numbers:
       "The Dynamic and/or Private Ports are those from 49152 through 65535" */
    srand(time(NULL));
    uint16_t port = (rand() % (65535-49152)) + 49152;

    _interface = M2MInterfaceFactory::create_interface(*this,
                                              CLIENT_NAME,
                                              "test",
                                              60,
                                              port,
                                              "",
                                              M2MInterface::UDP,
                                              stack,
                                              "");

    return (_interface == NULL) ? false : true;
}

bool MbedClient::bootstrap_successful() {
#ifdef TARGET_LIKE_LINUX
    while(!_bootstrapped && !_error) {
        sleep(1);
    }
#endif
    return _bootstrapped;
}

bool MbedClient::register_successful() {
#ifdef TARGET_LIKE_LINUX
    while(!_registered && !_error) {
        sleep(1);
    }
#endif
    return _registered;
}

bool MbedClient::unregister_successful() {
#ifdef TARGET_LIKE_LINUX
    while(!_unregistered && !_error) {
        sleep(1);
    }
#endif
    return _unregistered;
}

bool MbedClient::registration_update_successful() {
#ifdef TARGET_LIKE_LINUX
    while(!_registration_updated && !_error) {
        sleep(1);
    }
#endif
    return _registration_updated;
}

M2MSecurity* MbedClient::create_bootstrap_object(bool useSecureConnection) {
    // Creates bootstrap server object with Bootstrap server address and other parameters
    // required for client to connect to bootstrap server.
    M2MSecurity *security = M2MInterfaceFactory::create_security(M2MSecurity::Bootstrap);
    if(security) {
        if(useSecureConnection){
            security->set_resource_value(M2MSecurity::M2MServerUri, BOOTSTRAP_SERVER_DTLS_ADDRESS);
            //TODO: remove these, when bootstrapping server supports DTLS
            delete security;
            return NULL;
        }else{
            security->set_resource_value(M2MSecurity::M2MServerUri, BOOTSTRAP_SERVER_ADDRESS);
            //TODO: refactor this out
            security->set_resource_value(M2MSecurity::SecurityMode, M2MSecurity::NoSecurity);
        }
    }
    return security;
}

M2MSecurity* MbedClient::create_register_object(bool useSecureConnection) {
    // Creates bootstrap server object with Bootstrap server address and other parameters
    // required for client to connect to bootstrap server.
    M2MSecurity *security = M2MInterfaceFactory::create_security(M2MSecurity::M2MServer);
    if(security) {
        if(useSecureConnection){
            security->set_resource_value(M2MSecurity::M2MServerUri, MBED_SERVER_DTLS_ADDRESS);
        }else{
            security->set_resource_value(M2MSecurity::M2MServerUri, MBED_SERVER_ADDRESS);
        }
    }
    return security;
}

void MbedClient::test_bootstrap(M2MSecurity *security) {
    if(_interface) {
         // Bootstrap function.
        _interface->bootstrap(security);
    }
}

M2MDevice* MbedClient::create_device_object() {
    // Creates device object which contains mandatory resources linked with
    // device endpoint.
    M2MDevice *device = M2MInterfaceFactory::create_device();
    if(device) {
        device->create_resource(M2MDevice::Manufacturer,MANUFACTURER);
        device->create_resource(M2MDevice::DeviceType,TYPE);
        device->create_resource(M2MDevice::ModelNumber,MODEL_NUMBER);
        device->create_resource(M2MDevice::SerialNumber,SERIAL_NUMBER);
    }
    return device;
}

void MbedClient::execute_function(void *argument) {
#ifdef TARGET_LIKE_LINUX
    if(argument) {
        char* arguments = (char*)argument;
        printf("Received %s!!\n", arguments);
    }
    printf("I am executed !!\n");
#else
    _led == 0 ? _led = 1 : _led = 0;
#endif
}

M2MObject* MbedClient::create_generic_object() {
    _object = M2MInterfaceFactory::create_object("Test");
    if(_object) {
        M2MObjectInstance* inst = _object->create_object_instance();
        if(inst) {
            inst->set_operation(M2MBase::GET_ALLOWED);

            M2MResource* res = inst->create_dynamic_resource("Dynamic",
                                                             "ResourceTest",
                                                             M2MResourceInstance::INTEGER,
                                                             true);

            char buffer[20];
            int size = sprintf(buffer,"%d",_value);
            res->set_operation(M2MBase::GET_PUT_POST_ALLOWED);
            res->set_value((const uint8_t*)buffer,
                           (const uint32_t)size);
            res->set_execute_function(execute_callback(this,&MbedClient::execute_function));
            _value++;

            inst->create_static_resource("Static",
                                         "ResourceTest",
                                         M2MResourceInstance::STRING,
                                         STATIC_VALUE,
                                         sizeof(STATIC_VALUE)-1);
        }
    }
    return _object;
}

void MbedClient::update_resource() {
    if(_object) {
        M2MObjectInstance* inst = _object->object_instance();
        if(inst) {
            M2MResource* res = inst->resource("Dynamic");
            if( res ){
                char buffer[20];
                int size = sprintf(buffer,"%d",_value);
                res->set_value((const uint8_t*)buffer,
                               (const uint32_t)size);
                _value++;
            }
        }
    }
}

void MbedClient::test_register(M2MObjectList object_list){
    if(_interface) {
        _interface->register_object(_register_security, object_list);
    }
}

void MbedClient::set_register_object(M2MSecurity *&register_object){
    if(_register_security) {
        delete _register_security;
        _register_security = NULL;
    }
    _register_security = register_object;

}

void MbedClient::test_update_register() {
    uint32_t updated_lifetime = 20;
    _registered = false;
    _unregistered = false;
    if(_interface){
        _interface->update_registration(_register_security,updated_lifetime);
    }
}

void MbedClient::test_unregister() {
    if(_interface) {
        _interface->unregister_object(NULL);
    }
}

void MbedClient::bootstrap_done(M2MSecurity *server_object){
    if(server_object) {
        set_register_object(server_object);
        _bootstrapped = true;
        _error = false;
        printf("\nBootstrapped\n");
    }
}

void MbedClient::object_registered(M2MSecurity */*security_object*/, const M2MServer &/*server_object*/){
    _registered = true;
    _unregistered = false;
    printf("\nRegistered\n");
}

void MbedClient::object_unregistered(M2MSecurity */*server_object*/){
    _unregistered = true;
    _registered = false;

#ifdef TARGET_LIKE_MBED
    notify_completion(_unregistered);
    minar::Scheduler::stop();
#endif
    printf("\nUnregistered\n");
}

void MbedClient::registration_updated(M2MSecurity */*security_object*/, const M2MServer & /*server_object*/){
    _registration_updated = true;
    printf("\nregistration updated\n");
}

void MbedClient::error(M2MInterface::Error error){
    _error = true;
    printf("\nError occured Error code %d\n", (int)error);
}

void MbedClient::value_updated(M2MBase *base, M2MBase::BaseType type) {
    printf("\nValue updated of Object name %s and Type %d\n",
           base->name().c_str(), type);
}
