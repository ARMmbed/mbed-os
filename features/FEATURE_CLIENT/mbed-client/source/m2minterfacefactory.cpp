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
#include "mbed-client/m2minterfacefactory.h"
#include "mbed-client/m2mserver.h"
#include "mbed-client/m2mdevice.h"
#include "mbed-client/m2mfirmware.h"
#include "mbed-client/m2mobject.h"
#include "mbed-client/m2mconstants.h"
#include "mbed-client/m2mconfig.h"
#include "include/m2minterfaceimpl.h"
#include "mbed-trace/mbed_trace.h"

#define TRACE_GROUP "mClt"

M2MInterface* M2MInterfaceFactory::create_interface(M2MInterfaceObserver &observer,
                                                    const String &endpoint_name,
                                                    const String &endpoint_type,
                                                    const int32_t life_time,
                                                    const uint16_t listen_port,
                                                    const String &domain,
                                                    M2MInterface::BindingMode mode,
                                                    M2MInterface::NetworkStack stack,
                                                    const String &context_address)
{
    tr_debug("M2MInterfaceFactory::create_interface - IN");
    tr_debug("M2MInterfaceFactory::create_interface - parameters endpoint name : %s",endpoint_name.c_str());
    tr_debug("M2MInterfaceFactory::create_interface - parameters endpoint type : %s",endpoint_type.c_str());
    tr_debug("M2MInterfaceFactory::create_interface - parameters life time(in secs):  %d",life_time);
    tr_debug("M2MInterfaceFactory::create_interface - parameters Listen Port : %d",listen_port);
    tr_debug("M2MInterfaceFactory::create_interface - parameters Binding Mode : %d",(int)mode);
    tr_debug("M2MInterfaceFactory::create_interface - parameters NetworkStack : %d",(int)stack);
    M2MInterfaceImpl *interface = NULL;


    bool endpoint_type_valid = true;
    if(!endpoint_type.empty()) {
        if(endpoint_type.size() > MAX_ALLOWED_STRING_LENGTH){
            endpoint_type_valid = false;
        }
    }

    bool domain_valid = true;
    if(!domain.empty()) {
        if(domain.size() > MAX_ALLOWED_STRING_LENGTH){
            domain_valid = false;
        }
    }

    if(((life_time == -1) || (life_time >= MINIMUM_REGISTRATION_TIME)) &&
       !endpoint_name.empty() && (endpoint_name.size() <= MAX_ALLOWED_STRING_LENGTH) &&
       endpoint_type_valid && domain_valid) {
        tr_debug("M2MInterfaceFactory::create_interface - Creating M2MInterfaceImpl");
        interface = new M2MInterfaceImpl(observer, endpoint_name,
                                         endpoint_type, life_time,
                                         listen_port, domain, mode,
                                         stack, context_address);

    }
    tr_debug("M2MInterfaceFactory::create_interface - OUT");
    return interface;
}

M2MSecurity* M2MInterfaceFactory::create_security(M2MSecurity::ServerType server_type)
{
    tr_debug("M2MInterfaceFactory::create_security");
    M2MSecurity *security = new M2MSecurity(server_type);
    return security;
}

M2MServer* M2MInterfaceFactory::create_server()
{
    tr_debug("M2MInterfaceFactory::create_server");
    M2MServer *server = new M2MServer();
    return server;
}

M2MDevice* M2MInterfaceFactory::create_device()
{
    tr_debug("M2MInterfaceFactory::create_device");
    M2MDevice* device = M2MDevice::get_instance();
    return device;
}

M2MFirmware* M2MInterfaceFactory::create_firmware()
{
    tr_debug("M2MInterfaceFactory::create_firmware");
    M2MFirmware* firmware = M2MFirmware::get_instance();
    return firmware;
}

M2MObject* M2MInterfaceFactory::create_object(const String &name)
{
    tr_debug("M2MInterfaceFactory::create_object : Name : %s", name.c_str());
    if(name.size() > MAX_ALLOWED_STRING_LENGTH || name.empty()){
        return NULL;
    }

    M2MObject *object = NULL;
    object = new M2MObject(name);
    return object;
}
