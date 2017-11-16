/*
 * Copyright (c) 2017, Arm Limited and affiliates.
 * SPDX-License-Identifier: Apache-2.0
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include "test_ns_mdns_api.h"
#include <stdio.h>
#include <string.h>
#include <inttypes.h>
#include "fnet.h"
#include "ns_mdns_api.h"

#include "fnet_mdns_stub.h"
#include "nsdynmemLIB_stub.h"

static const char service_name[] = "Instance name"; 

bool test_ns_mdns_api_start_1()
{
    ns_mdns_t ns_mdns_instance;

    // Initialize test data
    fnet_mdns_data_stub.fnet_mdns_descriptor = 1;
    fnet_mdns_data_stub.fnet_mdns_service_descriptor = 1;
    fnet_mdns_data_stub.unregister_cnt = 0;
    fnet_mdns_data_stub.release_cnt = 0;
    nsdynmemlib_stub.returnCounter = 10;
    
    // Test NULL service name, should return NULL
    ns_mdns_instance = ns_mdns_server_start(NULL, 0, 0, 1);
    if (ns_mdns_instance) {
        return false;
    }

    // Test bad interface ID, should return NULL
    ns_mdns_instance = ns_mdns_server_start(service_name, 0, 0, -1);
    if (ns_mdns_instance) {
        return false;
    }

    // Test OOM_0, should return NULL
    nsdynmemlib_stub.returnCounter = 0;
    ns_mdns_instance = ns_mdns_server_start(service_name, 0, 0, 1);
    if (ns_mdns_instance) {
        return false;
    }

    // Test OOM_1, should return NULL
    nsdynmemlib_stub.returnCounter = 1;
    ns_mdns_instance = ns_mdns_server_start(service_name, 0, 0, 1);
    if (ns_mdns_instance) {
        return false;
    }

    // Test OK case, returns instance
    nsdynmemlib_stub.returnCounter = 2;
    ns_mdns_instance = ns_mdns_server_start(service_name, 0, 0, 1);
    if (!ns_mdns_instance) {
        return false;
    }

    // Stop mDNS responder
    ns_mdns_server_stop(ns_mdns_instance);
    if (fnet_mdns_data_stub.release_cnt != 1) {
        printf("Error, fnet_mdns_data_stub.release_cnt %d != 1", fnet_mdns_data_stub.release_cnt);
        return false;
    }

    return true;
}

bool test_ns_mdns_api_service_1()
{
    ns_mdns_t ns_mdns_instance;
    ns_mdns_service_t ns_mdns_service;
    ns_mdns_service_t ns_mdns_service_2;
    ns_mdns_service_param_t service = {
        .service_type = "type",
        .service_port = 666,
        .service_get_txt = NULL
    };
    fnet_mdns_data_stub.fnet_mdns_descriptor = 1;
    fnet_mdns_data_stub.fnet_mdns_service_descriptor = 1;
    fnet_mdns_data_stub.unregister_cnt = 0;
    fnet_mdns_data_stub.release_cnt = 0;

    // Create mDNS server
    nsdynmemlib_stub.returnCounter = 2;
    ns_mdns_instance = ns_mdns_server_start(service_name, 0, 0, 1);
    if (!ns_mdns_instance) {
        return false;
    }

    // Test NULL instance, registration fails
    ns_mdns_service = ns_mdns_service_register(NULL, &service);
    if (ns_mdns_service) {
        return false;
    }

    // Test NULL params, registration fails
    ns_mdns_service = ns_mdns_service_register(ns_mdns_instance, NULL);
    if (ns_mdns_service) {
        return false;
    }

    // Test bad instance, registration fails
    ns_mdns_service = ns_mdns_service_register(0xffff, &service);
    if (ns_mdns_service) {
        return false;
    }

    // Test NULL service_type, registration fails
    service.service_type = NULL;
    ns_mdns_service = ns_mdns_service_register(ns_mdns_instance, &service);
    service.service_type = "type";
    if (ns_mdns_service) {
        return false;
    }

    // Test NULL callback, registration fails
    service.service_get_txt = NULL;
    ns_mdns_service = ns_mdns_service_register(ns_mdns_instance, &service);
    service.service_get_txt = 0xf00d;
    if (ns_mdns_service) {
        return false;
    }

    // Test OOM_0
    nsdynmemlib_stub.returnCounter = 0;
    ns_mdns_service = ns_mdns_service_register(ns_mdns_instance, &service);
    if (ns_mdns_service) {
        return false;
    }

    // Test OOM_1
    nsdynmemlib_stub.returnCounter = 1;
    ns_mdns_service = ns_mdns_service_register(ns_mdns_instance, &service);
    if (ns_mdns_service) {
        return false;
    }

    // Test OK registration
    nsdynmemlib_stub.returnCounter = 2;
    ns_mdns_service = ns_mdns_service_register(ns_mdns_instance, &service);
    if (!ns_mdns_service) {
        return false;
    }

    // Test registration again
    nsdynmemlib_stub.returnCounter = 2;
    ns_mdns_service_2 = ns_mdns_service_register(ns_mdns_instance, &service);
    if (!ns_mdns_service_2) {
        return false;
    }

    // Stop mDNS responder
    ns_mdns_server_stop(ns_mdns_instance);
    // test that unregister is called
    if (fnet_mdns_data_stub.release_cnt != 1) {
        printf("Error, fnet_mdns_data_stub.release_cnt %d!=1", fnet_mdns_data_stub.release_cnt);
        return false;
    }

    return true;
}

bool test_ns_mdns_api_service_2()
{
    ns_mdns_t ns_mdns_instance;
    ns_mdns_service_t ns_mdns_service_1;
    ns_mdns_service_t ns_mdns_service_2;
    ns_mdns_service_t ns_mdns_service_3;
    ns_mdns_service_param_t service = {
        .service_type = "type",
        .service_port = 666,
        .service_get_txt = 0xffff
    };
    fnet_mdns_data_stub.fnet_mdns_descriptor = 1;
    fnet_mdns_data_stub.fnet_mdns_service_descriptor = 1;
    fnet_mdns_data_stub.unregister_cnt = 0;
    fnet_mdns_data_stub.release_cnt = 0;

    // Create mDNS server
    nsdynmemlib_stub.returnCounter = 2;
    ns_mdns_instance = ns_mdns_server_start(service_name, 0, 0, 1);
    if (!ns_mdns_instance) {
        return false;
    }

    // Test OK registration
    nsdynmemlib_stub.returnCounter = 2;
    ns_mdns_service_1 = ns_mdns_service_register(ns_mdns_instance, &service);
    if (!ns_mdns_service_1) {
        return false;
    }

    // Test registration again
    nsdynmemlib_stub.returnCounter = 2;
    ns_mdns_service_2 = ns_mdns_service_register(ns_mdns_instance, &service);
    if (!ns_mdns_service_2) {
        return false;
    }

    // Test registration again
    nsdynmemlib_stub.returnCounter = 2;
    ns_mdns_service_3 = ns_mdns_service_register(ns_mdns_instance, &service);
    if (!ns_mdns_service_3) {
        return false;
    }

    ns_mdns_service_unregister(NULL);
    ns_mdns_service_unregister(0xffff);
    ns_mdns_service_unregister(ns_mdns_service_2);
    ns_mdns_service_unregister(ns_mdns_service_1);
    ns_mdns_service_unregister(ns_mdns_service_3);
    if (fnet_mdns_data_stub.unregister_cnt != 3) {
        printf("Error, fnet_mdns_data_stub.unregister_cnt %d!=3", fnet_mdns_data_stub.unregister_cnt);
        return false;
    }

    // Stop mDNS responder
    ns_mdns_server_stop(ns_mdns_instance);
    // test that unregister is called
    if (fnet_mdns_data_stub.release_cnt != 1) {
        printf("Error, fnet_mdns_data_stub.release_cnt %d!=1", fnet_mdns_data_stub.release_cnt);
        return false;
    }

    return true;
}

bool test_ns_mdns_api_announce_1()
{
    ns_mdns_t ns_mdns_instance;
    ns_mdns_service_t ns_mdns_service;
    fnet_mdns_data_stub.fnet_mdns_descriptor = 1;
    fnet_mdns_data_stub.fnet_mdns_service_descriptor = 1;
    fnet_mdns_data_stub.announce_cnt = 0;
    fnet_mdns_data_stub.unregister_cnt = 0;
    ns_mdns_service_param_t service = {
        .service_type = "type",
        .service_port = 666,
        .service_get_txt = NULL
    };

    // Create mDNS server
    nsdynmemlib_stub.returnCounter = 2;
    ns_mdns_instance = ns_mdns_server_start(service_name, 0, 0, 1);
    if (!ns_mdns_instance) {
        return false;
    }

    // Test NULL instance
    ns_mdns_announcement_send(NULL);
    if (fnet_mdns_data_stub.announce_cnt != 0) {
        return false;
    }

    // Test illegal instance
    ns_mdns_announcement_send(0xffff);
    if (fnet_mdns_data_stub.announce_cnt != 0) {
        return false;
    }

    // Test announcement
    ns_mdns_announcement_send(ns_mdns_instance);
    if (fnet_mdns_data_stub.announce_cnt != 1) {
        return false;
    }
    // Stop mDNS responder
    ns_mdns_server_stop(ns_mdns_instance);

    if (fnet_mdns_data_stub.release_cnt != 1) {
        printf("Error, fnet_mdns_data_stub.release_cnt %d!=1", fnet_mdns_data_stub.release_cnt);
        return false;
    }

    return true;
}

