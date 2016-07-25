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
#include "mbed-client/m2minterfacefactory.h"
#include "mbed-client/m2minterface.h"
#include "mbed-client/m2mdevice.h"
#include "mbed-client/m2minterfaceobserver.h"
#include "mbed-client/m2mobjectinstance.h"
#include "mbed-client/m2mresource.h"
#include "mbed-client/m2mfirmware.h"

class M2MLWClient: public M2MInterfaceObserver {
public:
    M2MLWClient();

    ~M2MLWClient();

    bool create_interface(int32_t param_count,
                          const char *endpoint,
                          const char *resource_type,
                          const int32_t lifetime,
                          const uint16_t listen_port,
                          const char *domain,
                          const uint8_t binding_mode,
                          const uint8_t network_interface);

    bool create_bootstrap_object(const char *coap_bootstrap_address);

    bool create_register_object(const char *coap_register_address, bool useSecureConn);

    bool test_bootstrap();

    bool create_device_object();

    bool create_device_object(M2MDevice::DeviceResource resource,
                              const char *value);

    bool create_device_object(M2MDevice::DeviceResource resource,
                              int64_t value,
                              uint16_t instance_id);

    bool create_device_object(M2MDevice::DeviceResource resource,
                              int64_t value);

    bool create_firmware_object();

    bool create_firmware_object(M2MFirmware::FirmwareResource resource,
                              const char *value);

    bool create_firmware_object(M2MFirmware::FirmwareResource resource,
                              int64_t value);

    bool create_firmware_object(M2MFirmware::FirmwareResource resource,
                                const uint8_t *value,
                                const uint32_t length);

    void firmware_resource_int(int resource);
    void firmware_resource_string(int resource);

    void firmware_resource_buffer();

    bool create_object(const char *name,
                       bool new_instance,
                       uint8_t object_operation,
                       uint8_t object_instance_operation,
                       uint16_t object_instance_id,
                       bool object_observable,
                       bool object_instance_observable);

    bool create_static_resource_string(const char *name,
                                       const char *value,
                                       bool multiple_instance,
                                       uint16_t object_instance = 0);

    bool create_static_resource_int(const char *name,
                                    int64_t value,
                                    bool multiple_instance,
                                    uint16_t object_instance = 0);

    bool create_dynamic_resource_int(const char *name,
                                 bool observable,
                                 bool multiple_instance,
                                 uint16_t object_instance = 0,
                                 uint8_t resource_operation = 1);
    bool create_dynamic_resource_string(const char *name,
                                 bool observable,
                                 bool multiple_instance,
                                 uint16_t object_instance = 0,
                                 uint8_t resource_operation = 1);

    bool set_resource_value(const char *name,
                            int32_t value,
                            uint16_t object_instance = 0);

    bool set_resource_value(const char *name,
                            const char *value,
                            uint16_t object_instance = 0);

    bool create_static_resource_instance_string(const char *name,
                                                const char *value,
                                                bool multiple_instance,
                                                uint16_t object_instance = 0,
                                                uint16_t resource_instance = 0);

    bool create_static_resource_instance_int(const char *name,
                                             int32_t value,
                                             bool multiple_instance,
                                             uint16_t object_instance = 0,
                                             uint16_t resource_instance = 0);

    bool create_dynamic_resource_instance_int(const char *name,
                                          bool observable,
                                          bool multiple_instance,
                                          uint16_t object_instance = 0,
                                          uint16_t resource_instance = 0,
                                          uint8_t resource_instance_operation = 1);
    bool create_dynamic_resource_instance_string(const char *name,
                                          bool observable,
                                          bool multiple_instance,
                                          uint16_t object_instance = 0,
                                          uint16_t resource_instance = 0,
                                          uint8_t resource_instance_operation = 1);

    bool set_resource_instance_value(const char *name,
                                     int32_t value,
                                     uint16_t object_instance = 0,
                                     uint16_t resource_instance = 0);

    bool set_resource_instance_value(const char *name,
                                     const char *value,
                                     uint16_t object_instance = 0,
                                     uint16_t resource_instance = 0);

    bool create_object_instance();

    bool test_register();

    bool test_update_register(const uint32_t lifetime);

    bool test_unregister();

    void bootstrap_done(M2MSecurity *server_object);

    void object_registered(M2MSecurity *security_object, const M2MServer &server_object);

    void object_unregistered(M2MSecurity *server_object);

    void registration_updated(M2MSecurity *security_object, const M2MServer &server_object);

    void error(M2MInterface::Error error);

    void value_updated(M2MBase *base, M2MBase::BaseType type);

    void set_fw_execute_function();

private:

    M2MBase::Operation int_to_operation(uint8_t operation);
    void fw_execute_function(void *argument);


private:

    M2MInterface        *_interface;
    M2MSecurity         *_security;
    M2MSecurity         *_register_security;
    M2MDevice           *_device;
    M2MFirmware         *_firmware;
    M2MObject           *_object;
    bool                _bootstrapped;
    bool                _error;
    bool                _registered;
    bool                _unregistered;
    bool                _registration_updated;
};


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

