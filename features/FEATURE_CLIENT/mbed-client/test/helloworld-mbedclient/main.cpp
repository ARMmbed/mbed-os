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

#ifdef TARGET_LIKE_LINUX
#include <unistd.h>
#include <stdio.h>
#include <stdarg.h>
#include <pthread.h>
#include <signal.h> /* For SIGIGN and SIGINT */
#else
#include "sockets/UDPSocket.h"
#ifdef SIXLOWPAN_INTERFACE
#include "atmel-rf-driver/driverRFPhy.h"    // rf_device_register
#include "mbed-mesh-api/Mesh6LoWPAN_ND.h"
#endif
#include "EthernetInterface.h"
#include "test_env.h"
// TODO: Remove when yotta supports init.
#include "lwipv4_init.h"
using namespace mbed::util;
#endif

#include "mbed-client/m2minterfacefactory.h"
#include "mbed-client/m2mdevice.h"
#include "mbed-client/m2minterfaceobserver.h"
#include "mbed-client/m2minterface.h"
#include "mbed-client/m2mobjectinstance.h"
#include "mbed-client/m2mresource.h"
#include "mbed-trace/mbed_trace.h"

#ifdef TARGET_LIKE_LINUX
static void ctrl_c_handle_function(void);
typedef void (*signalhandler_t)(int); /* Function pointer type for ctrl-c */
#else
#if defined(TARGET_K64F)
#define OBS_BUTTON SW2
#define UNREG_BUTTON SW3
#endif
#endif

bool _have_secure_conn = false;

const unsigned char psk[] = {0x31,0x32,0x33,0x34,0x35,0x36,0x37,0x38,0x39,0x30,0x31,0x32,0x33,0x34,0x35,0x36};
const size_t psk_len = sizeof( psk );
const unsigned char psk_identity[] = {0x0F,0x0F};
const size_t psk_identity_len = sizeof( psk_identity );

const uint8_t server_cert[] = "-----BEGIN CERTIFICATE-----\r\n"
"MIIBmDCCAT6gAwIBAgIEVUCA0jAKBggqhkjOPQQDAjBLMQswCQYDVQQGEwJGSTEN\r\n"
"MAsGA1UEBwwET3VsdTEMMAoGA1UECgwDQVJNMQwwCgYDVQQLDANJb1QxETAPBgNV\r\n"
"BAMMCEFSTSBtYmVkMB4XDTE1MDQyOTA2NTc0OFoXDTE4MDQyOTA2NTc0OFowSzEL\r\n"
"MAkGA1UEBhMCRkkxDTALBgNVBAcMBE91bHUxDDAKBgNVBAoMA0FSTTEMMAoGA1UE\r\n"
"CwwDSW9UMREwDwYDVQQDDAhBUk0gbWJlZDBZMBMGByqGSM49AgEGCCqGSM49AwEH\r\n"
"A0IABLuAyLSk0mA3awgFR5mw2RHth47tRUO44q/RdzFZnLsAsd18Esxd5LCpcT9w\r\n"
"0tvNfBv4xJxGw0wcYrPDDb8/rjujEDAOMAwGA1UdEwQFMAMBAf8wCgYIKoZIzj0E\r\n"
"AwIDSAAwRQIhAPAonEAkwixlJiyYRQQWpXtkMZax+VlEiS201BG0PpAzAiBh2RsD\r\n"
"NxLKWwf4O7D6JasGBYf9+ZLwl0iaRjTjytO+Kw==\r\n"
"-----END CERTIFICATE-----\r\n";

const uint8_t cert[] = "-----BEGIN CERTIFICATE-----\r\n"
"MIICTDCCAbWgAwIBAgIJAKI3S+LGklSGMA0GCSqGSIb3DQEBCwUAMD8xCzAJBgNV\r\n"
"BAYTAkZJMRMwEQYDVQQIDApTb21lLVN0YXRlMQ0wCwYDVQQHDARPdWx1MQwwCgYD\r\n"
"VQQKDANBUk0wHhcNMTUwNjExMTI0NTU1WhcNMTYwNjEwMTI0NTU1WjA/MQswCQYD\r\n"
"VQQGEwJGSTETMBEGA1UECAwKU29tZS1TdGF0ZTENMAsGA1UEBwwET3VsdTEMMAoG\r\n"
"A1UECgwDQVJNMIGfMA0GCSqGSIb3DQEBAQUAA4GNADCBiQKBgQDBt8ag12ILm2pn\r\n"
"RHUc2yMBXpdzspDwVV3VDaNTRXypVUOq/nxJc6zr8yG/Pvga2/XVijWQDLABHVhV\r\n"
"PoeulzXVSEHRR2bR2lhqulLzdUzPYJ+yJd4+082akoxdzoom8ms2LFlgiXO7lyBg\r\n"
"1t74xjEQapCkr7Tdl2pD9OTUrcfoEwIDAQABo1AwTjAdBgNVHQ4EFgQUHK0O4iK7\r\n"
"Rv8AsxAqRDlY3TSHWtUwHwYDVR0jBBgwFoAUHK0O4iK7Rv8AsxAqRDlY3TSHWtUw\r\n"
"DAYDVR0TBAUwAwEB/zANBgkqhkiG9w0BAQsFAAOBgQAPVLihYN65DEeYdf7gDAdP\r\n"
"MNH5RxsQlHdktQkYWcUewe3hvOf8yZUBbmO5OeEQmy5Ca2H1QhUuw+kWHo1l/K9g\r\n"
"DpR1xoDACeycBd/6tAHfaFzTEEDC9Ug839EAHDrbI4ihrPEbP6hyRIaUIM4IojXf\r\n"
"zuqwOcobS3idv8a9Npuohw==\r\n"
"-----END CERTIFICATE-----\r\n";

const uint8_t key[] = "-----BEGIN PRIVATE KEY-----\r\n"
"MIICdwIBADANBgkqhkiG9w0BAQEFAASCAmEwggJdAgEAAoGBAMG3xqDXYgubamdE\r\n"
"dRzbIwFel3OykPBVXdUNo1NFfKlVQ6r+fElzrOvzIb8++Brb9dWKNZAMsAEdWFU+\r\n"
"h66XNdVIQdFHZtHaWGq6UvN1TM9gn7Il3j7TzZqSjF3OiibyazYsWWCJc7uXIGDW\r\n"
"3vjGMRBqkKSvtN2XakP05NStx+gTAgMBAAECgYEAlRimNxsu6jIXJh4paSk8ewxb\r\n"
"0v+n0GJT0uwpPakotofaihxOepYejIfX/IMxoDKVtmImsZUiTelRGGMFORpGomS7\r\n"
"3Pb9awfn1yWiYja9MF7+pgFtXj6LxcB5rcxrP8s9rCs+kEq8yl/khGXmlwlpacUm\r\n"
"Pdg1rEHriykk/5dlyKkCQQD9zpgQKN87bQNOVSgFSNsrwCjEN2MvVUC6ycqVYqQd\r\n"
"oQZrO2lJuf57Jn7kRKJgfqqImwLYaohn5A9gKiqszo+fAkEAw2REr8L9aLoRPe5Q\r\n"
"4+lYCmNmxJjBbJoRviSlxx+p5dxK+8H4J+bnFUWzORFJP0+Bjhii2UHGBoMjuP2s\r\n"
"e8xDDQJADm0IrZp1Hb6TeodSkAJVGsaASq7PP2h8FmHT1uvVYi293Khy0pL0yPEm\r\n"
"MzlHdW6dqMgOwFhFyonm6dNbu5i+BwJBALSPKfIKDMcEev5yBgo3Dga7iLJQPx4u\r\n"
"2gqOLyvtlME3a4tu2fV1qUG9ITuxP7DjteMr3QFLiMVn1lETCpt0TNkCQGIvyjdV\r\n"
"RzYnzLDMulXCsuRBjs0C75KoZrcPPQ416SZFtYASsOl3P0Ih5jgt/1aPmth3Yson\r\n"
"GdnBOWyKzadLmz8=\r\n"
"-----END PRIVATE KEY-----\r\n";

const size_t server_cert_len = sizeof( server_cert );
const size_t cert_len = sizeof( cert );
const size_t key_len = sizeof( key );

#ifdef TARGET_LIKE_LINUX

#if defined (BOOTSTRAP_ENABLED)
void* wait_for_bootstrap(void* arg) {
    MbedClient *client;
    client = (MbedClient*) arg;
    if(client->bootstrap_successful()) {
        printf("Registering endpoint\n");

        // Create LWM2M device object specifying device resources
        // as per OMA LWM2M specification.
        M2MDevice* device_object = client->create_device_object();

        M2MObject* object = client->create_generic_object();

        // Add all the objects that you would like to register
        // into the list and pass the list for register API.
        M2MObjectList object_list;
        object_list.push_back(device_object);
        object_list.push_back(object);

        // Issue register command.
        client->test_register(object_list);
    }
    return NULL;
}
#endif

void* wait_for_unregister(void* arg) {
    MbedClient *client;
    client = (MbedClient*) arg;
    if(client->unregister_successful()) {
        printf("Unregistered done --> exiting\n");
        exit(1);
    }
    return NULL;
}

void* send_observation(void* arg) {
    MbedClient *client;
    client = (MbedClient*) arg;
    static uint8_t counter = 0;
    while(1) {
        sleep(1);
        if(counter >= 10 &&
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
#else
#ifdef SIXLOWPAN_INTERFACE
volatile uint8_t mesh_network_state = MESH_DISCONNECTED;

void mesh_network_callback(mesh_connection_status_t mesh_state)
{
    tr_info("Network established");
    mesh_network_state = mesh_state;
}
#endif
#endif

void trace_printer(const char* str)
{
  printf("%s\r\n", str);
}

#ifdef TARGET_LIKE_MBED
void app_start(int /*argc*/, char* /*argv*/[]) {
#else
int main() {
#endif

    // Instantiate the class which implements
    // mbed Client API
    MbedClient mbed_client;

#ifdef TARGET_LIKE_LINUX
    pthread_t unregister_thread;
    pthread_t observation_thread;

    m2mclient = &mbed_client;

    mbed_trace_init();
    mbed_trace_print_function_set( trace_printer );
    mbed_trace_config_set(TRACE_MODE_COLOR|TRACE_ACTIVE_LEVEL_DEBUG|TRACE_CARRIAGE_RETURN);

    signal(SIGINT, (signalhandler_t)ctrl_c_handle_function);

#else
    // This sets up the network interface configuration which will be used
    // by LWM2M Client API to communicate with mbed Device server.

#ifdef SIXLOWPAN_INTERFACE

    Mesh6LoWPAN_ND *mesh_api = Mesh6LoWPAN_ND::getInstance();
    int8_t status;

    status = mesh_api->init(rf_device_register(), mesh_network_callback);
    if (status != MESH_ERROR_NONE)
    {
        tr_error("Mesh network initialization failed %d!", status);
        return 1;
    }

    status = mesh_api->connect();
    if (status != MESH_ERROR_NONE)
    {
        tr_error("Can't connect to mesh network!");
        return 1;
    }

    do
    {
        mesh_api->processEvent();
    } while(mesh_network_state != MESH_CONNECTED);


#else
    EthernetInterface eth;
    eth.init(); //Use DHCP
    eth.connect();

    lwipv4_socket_init();
#endif

    // Set up Hardware interrupt button.
    InterruptIn obs_button(OBS_BUTTON);
    InterruptIn unreg_button(UNREG_BUTTON);

    // On press of SW3 button on K64F board, example application
    // will call unregister API towards mbed Device Server
    unreg_button.fall(&mbed_client,&MbedClient::test_unregister);

    // On press of SW2 button on K64F board, example application
    // will send observation towards mbed Device Server
    obs_button.fall(&mbed_client,&MbedClient::update_resource);


#endif

    // Create LWM2M Client API interface to manage bootstrap,
    // register and unregister

    //These 2 are defining the secure connection usage
    M2MSecurity::SecurityModeType mode = M2MSecurity::NoSecurity;
    _have_secure_conn = (mode == M2MSecurity::Certificate || mode == M2MSecurity::Psk);
    //    SecurityNotSet,
    //    Psk = 0,
    //    Certificate = 2,
    //    NoSecurity = 3

    mbed_client.create_interface();

#ifdef TARGET_LIKE_LINUX

#if defined (BOOTSTRAP_ENABLED)
    pthread_t bootstrap_thread;
    // Create LWM2M bootstrap object specifying bootstrap server
    // information.
    M2MSecurity* security_object = mbed_client.create_bootstrap_object(_have_secure_conn);
    // Issue bootstrap command.
    mbed_client.test_bootstrap(security_object);

    pthread_create(&bootstrap_thread, NULL, &wait_for_bootstrap, (void*) &mbed_client);
    pthread_join(bootstrap_thread, NULL);

#else
    M2MSecurity *register_object = mbed_client.create_register_object(_have_secure_conn);

    if( mode == M2MSecurity::Certificate ){
        register_object->set_resource_value(M2MSecurity::SecurityMode, M2MSecurity::Certificate);
        register_object->set_resource_value(M2MSecurity::ServerPublicKey,server_cert,server_cert_len);
        register_object->set_resource_value(M2MSecurity::PublicKey,cert,cert_len);
        register_object->set_resource_value(M2MSecurity::Secretkey,key,key_len);
    }else if( mode == M2MSecurity::Psk ){
        register_object->set_resource_value(M2MSecurity::SecurityMode, M2MSecurity::Psk);
        register_object->set_resource_value(M2MSecurity::ServerPublicKey,psk_identity,psk_identity_len);
        register_object->set_resource_value(M2MSecurity::PublicKey,psk_identity,psk_identity_len);
        register_object->set_resource_value(M2MSecurity::Secretkey,psk,psk_len);
    }else{
        register_object->set_resource_value(M2MSecurity::SecurityMode, M2MSecurity::NoSecurity);
    }

    mbed_client.set_register_object(register_object);
    M2MDevice* device_object = mbed_client.create_device_object();

    M2MObject* object = mbed_client.create_generic_object();

    M2MObjectList object_list;
    object_list.push_back(device_object);
    object_list.push_back(object);

    mbed_client.test_register(object_list);
#endif

    pthread_create(&observation_thread, NULL, &send_observation, (void*) &mbed_client);
    pthread_create(&unregister_thread, NULL, &wait_for_unregister, (void*) &mbed_client);
    pthread_join(unregister_thread, NULL);

#else

#if defined (BOOTSTRAP_ENABLED)
    // Create LWM2M bootstrap object specifying bootstrap server
    // information.
    M2MSecurity* security_object = mbed_client.create_bootstrap_object(_have_secure_conn);

    // Issue bootstrap command.
    mbed_client.test_bootstrap(security_object);

    // Wait till the bootstrap callback is called successfully.
    // Callback comes in bootstrap_done()
#ifdef SIXLOWPAN_INTERFACE
    /* wait network to be established */
    do {
        mesh_interface_run();
    } while(!mbed_client.bootstrap_successful());
#else
    while (!mbed_client.bootstrap_successful()) { __WFI(); }
#endif

#else

    M2MSecurity *register_object = mbed_client.create_register_object(_have_secure_conn);

    if( mode == M2MSecurity::Certificate ){
        register_object->set_resource_value(M2MSecurity::SecurityMode, M2MSecurity::Certificate);
        register_object->set_resource_value(M2MSecurity::ServerPublicKey,server_cert,server_cert_len);
        register_object->set_resource_value(M2MSecurity::PublicKey,cert,cert_len);
        register_object->set_resource_value(M2MSecurity::Secretkey,key,key_len);
    }else if( mode == M2MSecurity::Psk ){
        register_object->set_resource_value(M2MSecurity::SecurityMode, M2MSecurity::Psk);
        register_object->set_resource_value(M2MSecurity::ServerPublicKey,psk_identity,psk_identity_len);
        register_object->set_resource_value(M2MSecurity::PublicKey,psk_identity,psk_identity_len);
        register_object->set_resource_value(M2MSecurity::Secretkey,psk,psk_len);
    }else{
        register_object->set_resource_value(M2MSecurity::SecurityMode, M2MSecurity::NoSecurity);
    }

    mbed_client.set_register_object(register_object);

#endif

    // Create LWM2M device object specifying device resources
    // as per OMA LWM2M specification.
    M2MDevice* device_object = mbed_client.create_device_object();

    M2MObject* object = mbed_client.create_generic_object();

    // Add all the objects that you would like to register
    // into the list and pass the list for register API.
    M2MObjectList object_list;
    object_list.push_back(device_object);
    object_list.push_back(object);

    // Issue register command.

    FunctionPointer1<void, M2MObjectList> fp(&mbed_client, &MbedClient::test_register);
    minar::Scheduler::postCallback(fp.bind(object_list));

    minar::Scheduler::start();

    // Wait till the register callback is called successfully.
    // Callback comes in object_registered()
#ifdef SIXLOWPAN_INTERFACE
    /* wait network to be established */
    do {
        mesh_api->processEvent();
    } while(!mbed_client.register_successful());
#endif

    // Wait for the unregister successful callback,
    // Callback comes in object_unregsitered(), this will be
    // waiting for user to press SW2 button on K64F board.
#ifdef SIXLOWPAN_INTERFACE
    /* wait network to be established */
    do {
        mesh_api->processEvent();
    } while(!mbed_client.unregister_successful());
#endif


#if defined(BOOTSTRAP_ENABLED)
    // This will turn on the LED on the board specifying that
    // the application has run successfully.
    notify_completion(mbed_client.unregister_successful() &&
                      mbed_client.register_successful() &&
                      mbed_client.bootstrap_successful());

    // Delete security object created for bootstrapping
    if(security_object) {
        delete security_object;
    }

#else

    // Disconnect the connect and teardown the network interface
#ifdef SIXLOWPAN_INTERFACE
    mesh_api->disconnect();
#else
    eth.disconnect();
#endif
#endif //BOOTSTRAP_ENABLED

    // Delete device object created for registering device
    // resources.
    if(device_object) {
        M2MDevice::delete_instance();
    }
    if(object) {
        delete object;
    }

#endif //TARGET_LIKE_LINUX

#ifndef TARGET_LIKE_MBED
return 0;
#endif
}
