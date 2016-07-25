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
#ifndef __MBED_CLIENT_H__
#define __MBED_CLIENT_H__

#undef SIXLOWPAN_INTERFACE

#include "mbed-client/m2minterfaceobserver.h"

#ifndef TARGET_LIKE_LINUX
#include "mbed/DigitalOut.h"
#endif


//#define BOOTSTRAP_ENABLED

class M2MDevice;
class M2MSecurity;
class M2MObject;

class MbedClient: public M2MInterfaceObserver {
public:
    MbedClient();

    ~MbedClient();

    bool create_interface();

    bool bootstrap_successful();

    bool register_successful();

    bool unregister_successful();

    bool registration_update_successful();

    M2MSecurity* create_bootstrap_object(bool useSecureConnection);

    M2MSecurity* create_register_object(bool useSecureConnection);

    void test_bootstrap(M2MSecurity *security);

    M2MDevice* create_device_object();

    void execute_function(void *argument);

    M2MObject* create_generic_object();

    void update_resource();

    void test_register(M2MObjectList object_list);

    void set_register_object(M2MSecurity *&register_object);

    void test_update_register();

    void test_unregister();

    //Callback from mbed client stack when the bootstrap
    // is successful, it returns the mbed Device Server object
    // which will be used for registering the resources to
    // mbed Device server.
    void bootstrap_done(M2MSecurity *server_object);

    //Callback from mbed client stack when the registration
    // is successful, it returns the mbed Device Server object
    // to which the resources are registered and registered objects.
    void object_registered(M2MSecurity */*security_object*/, const M2MServer &/*server_object*/);

    //Callback from mbed client stack when the unregistration
    // is successful, it returns the mbed Device Server object
    // to which the resources were unregistered.
    void object_unregistered(M2MSecurity */*server_object*/);

    void registration_updated(M2MSecurity */*security_object*/, const M2MServer & /*server_object*/);

    //Callback from mbed client stack if any error is encountered
    // during any of the LWM2M operations. Error type is passed in
    // the callback.
    void error(M2MInterface::Error error);

    //Callback from mbed client stack if any value has changed
    // during PUT operation. Object and its type is passed in
    // the callback.
    void value_updated(M2MBase *base, M2MBase::BaseType type);

private:

#ifndef TARGET_LIKE_LINUX
    mbed::DigitalOut    _led;
#endif
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



#endif //__MBED_CLIENT_H__
