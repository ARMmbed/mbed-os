/*
 * Copyright (c) 2015-2017, Arm Limited and affiliates.
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

#include "nsconfig.h"
#include "ns_types.h"
#include "string.h"
#include "CppUTest/TestHarness.h"
#include "../../../../../nanostack/mlme.h"
#include "Service_Libs/load_balance/load_balance_api.h"
#include "nsdynmemLIB_stub.h"
#include "event_stub.h"

TEST_GROUP(LoadBalance)
{
    void setup() {
        nsdynmemlib_stub.returnCounter = 0;
        nsdynmemlib_stub.expectedPointer = NULL;
        event_stub.int8_value = -1;
    }

    void teardown() {
    }
};

static uint8_t test_priority;
static bool accept_immediately_switch = true;

static uint16_t generated_beacons = 0;
static const void *call_back_pointer = NULL;

static const void *call_back_pointer2 = NULL;

static const void *call_back_pointer3 = NULL;

static const void *call_back_pointer4 = NULL;

static mlme_pan_descriptor_s test_PANDescriptor;

void test_load_balance_beacon_interval_set(const void * load_balancer_user)
{
    call_back_pointer = load_balancer_user;
    generated_beacons++;
}

uint8_t test_load_balance_priority_get(const void * load_balancer_user)
{
    call_back_pointer2 = load_balancer_user;
    return test_priority;
}

bool test_load_balance_network_switch_req(void * load_balancer_user, struct mlme_pan_descriptor_s *PANDescriptor, const uint8_t *beacon_payload, uint16_t beacon_payload_length)
{
    call_back_pointer3 = load_balancer_user;
    test_PANDescriptor = *PANDescriptor;
    return true;
}

bool test_load_balance_network_switch_notify(const void * load_balancer_user, load_balance_nwk_switch_operation opeartion, uint16_t *timer)
{
    call_back_pointer4 = load_balancer_user;
    switch (opeartion) {
        case LB_ROUTER_LEAVE:
        default:

            if (timer) {
                *timer = 1;
            }

    }
    return true;
}

TEST(LoadBalance, test_load_balance_events)
{
    load_balance_api_t *load_balance_class;
    void *lb_user = &test_priority;
    event_stub.int8_value = 0;
    nsdynmemlib_stub.returnCounter = 2;
    generated_beacons = 0;
    call_back_pointer = NULL;
    load_balance_class = load_balance_create(test_load_balance_network_switch_notify, true);
    load_balance_network_threshold_set(load_balance_class, 16, 64);
    load_balance_class->lb_enable(load_balance_class, true, 100, 100);
    POINTERS_EQUAL(NULL, call_back_pointer);
    int8_t status = load_balance_class->lb_initialize(load_balance_class, test_load_balance_beacon_interval_set, test_load_balance_priority_get, test_load_balance_network_switch_req, 50, lb_user);
    BYTES_EQUAL(0, status);
    //Test acticate lb
    load_balance_class->lb_enable(load_balance_class, true, 100, 100);
    for (int i = 0; i< 301; i++) {
        load_balance_class->lb_seconds_tick_update(load_balance_class);
    }
    BYTES_EQUAL(2, generated_beacons);
    //test disable
    load_balance_class->lb_enable(load_balance_class, false, 0, 0);

    for (int i = 0; i< 200; i++) {
        load_balance_class->lb_seconds_tick_update(load_balance_class);
    }
    BYTES_EQUAL(2, generated_beacons);

    load_balance_class->lb_enable(load_balance_class, true, 100, 100);

    for (int i = 0; i< 301; i++) {
        load_balance_class->lb_seconds_tick_update(load_balance_class);
    }
    BYTES_EQUAL(4, generated_beacons);

    //set threshold
    status = load_balance_network_threshold_set(NULL,16, 64);
    BYTES_EQUAL(-1, status);

    status = load_balance_network_threshold_set(load_balance_class, 0, 0);
    BYTES_EQUAL(0, status);



    //Testing network switch
    uint8_t beacon_payload[100];
    mlme_beacon_ind_t beacon_ind;
    memset(&beacon_ind, 0, sizeof (mlme_beacon_ind_t));
    beacon_ind.PANDescriptor.LinkQuality = 150;
    beacon_ind.beacon_data_length = 100;
    beacon_ind.beacon_data = beacon_payload;

    load_balance_class->lb_beacon_notify(load_balance_class, &beacon_ind, 16);
    load_balance_class->lb_enable(load_balance_class, false, 0, 0);
    load_balance_class->lb_beacon_notify(load_balance_class, &beacon_ind, 16);

    status = load_balance_network_threshold_set(load_balance_class, 16, 64);
    BYTES_EQUAL(0, status);

    load_balance_class->lb_enable(load_balance_class, true, 100, 100);
    for (int i = 0; i< 101; i++) {
        load_balance_class->lb_seconds_tick_update(load_balance_class);
    }
    load_balance_class->lb_beacon_notify(load_balance_class, &beacon_ind, 16);
    beacon_ind.beacon_data_length = 50;
    test_priority = 0;
    load_balance_class->lb_beacon_notify(load_balance_class, &beacon_ind, 16);
    test_priority = 16 + 16;
    for (int i=0; i< 101; i++) {
        load_balance_class->lb_seconds_tick_update(load_balance_class);
    }
    //Shuold not accept this yet
    beacon_ind.PANDescriptor.CoordPANId = 4;
    load_balance_class->lb_beacon_notify(load_balance_class, &beacon_ind, 16);
    for (int i=0; i< 101; i++) {
        load_balance_class->lb_seconds_tick_update(load_balance_class);
    }
    test_priority = 16 + 16 + 1; // now we should switch
    beacon_ind.PANDescriptor.CoordPANId = 5;
    load_balance_class->lb_beacon_notify(load_balance_class, &beacon_ind, 16);
    for (int i=0; i< 101; i++) {
        load_balance_class->lb_seconds_tick_update(load_balance_class);
    }
    POINTERS_EQUAL(lb_user, call_back_pointer4);
    BYTES_EQUAL(5, test_PANDescriptor.CoordPANId);
    load_balance_class->lb_enable(load_balance_class, false, 10, 10);
    beacon_ind.PANDescriptor.CoordPANId = 9;
    load_balance_class->lb_beacon_notify(load_balance_class, &beacon_ind, 16);
    //Test network sitch

    load_balance_class->lb_enable(load_balance_class, true, 10, 10);
    for (int i=0; i< 11; i++) {
        load_balance_class->lb_seconds_tick_update(load_balance_class);
    }
    accept_immediately_switch = false;
    beacon_ind.PANDescriptor.CoordPANId = 1;
    load_balance_class->lb_beacon_notify(load_balance_class, &beacon_ind, 16);
    test_priority = 17 + 16 + 1;
    beacon_ind.PANDescriptor.CoordPANId = 2;
    load_balance_class->lb_beacon_notify(load_balance_class, &beacon_ind, 17);
    beacon_ind.PANDescriptor.LinkQuality = 100;
    beacon_ind.PANDescriptor.CoordPANId = 3;
    test_priority = 16 + 16 + 1;
    load_balance_class->lb_beacon_notify(load_balance_class, &beacon_ind, 16);
    for (int i=0; i< 100; i++) {
        load_balance_class->lb_seconds_tick_update(load_balance_class);
    }

    accept_immediately_switch = true;
    for (int i=0; i< 100; i++) {
        load_balance_class->lb_seconds_tick_update(load_balance_class);
    }

    POINTERS_EQUAL(lb_user, call_back_pointer3);
    BYTES_EQUAL(1, test_PANDescriptor.CoordPANId);
    load_balance_class->lb_enable(load_balance_class, false, 10, 10);
    load_balance_class->lb_enable(load_balance_class, true, 10, 10);
    for (int i=0; i< 11; i++) {
        load_balance_class->lb_seconds_tick_update(load_balance_class);
    }

    test_priority = 64+1;
    beacon_ind.PANDescriptor.CoordPANId = 3;
    load_balance_class->lb_beacon_notify(load_balance_class, &beacon_ind, 48);

    beacon_ind.PANDescriptor.CoordPANId = 8;
    load_balance_class->lb_beacon_notify(load_balance_class, &beacon_ind, 32);
    for (int i=0; i< 100; i++) {
        load_balance_class->lb_seconds_tick_update(load_balance_class);
    }

    POINTERS_EQUAL(lb_user, call_back_pointer3);
    BYTES_EQUAL(8, test_PANDescriptor.CoordPANId);
    load_balance_class->lb_enable(load_balance_class, false, 10, 10);
    load_balance_class->lb_enable(load_balance_class, true, 10, 10);
    //Test network switch over max threshold
    beacon_ind.PANDescriptor.CoordPANId = 3;
    test_PANDescriptor.CoordPANId = 0;
    test_priority = 64+1;
    for (int i=0; i< 100; i++) {
        load_balance_class->lb_seconds_tick_update(load_balance_class);
    }

    load_balance_class->lb_beacon_notify(load_balance_class, &beacon_ind, 0);

    for (int i=0; i< 100; i++) {
        load_balance_class->lb_seconds_tick_update(load_balance_class);
    }
    BYTES_EQUAL(3, test_PANDescriptor.CoordPANId);

    //free memory
    load_balance_delete(load_balance_class);
}


TEST(LoadBalance, test_load_balance)
{
    load_balance_api_t *load_balance_class;
    void *lb_user = &test_priority;

    load_balance_class = load_balance_create(NULL, true);
    POINTERS_EQUAL(NULL,load_balance_class);

    event_stub.int8_value = 0;
    nsdynmemlib_stub.returnCounter = 1;
    load_balance_class = load_balance_create(test_load_balance_network_switch_notify, true);
    if (!load_balance_class) {
        FAIL("Loadbalance create return NULL pointer");
    }
    load_balance_api_t *compare = load_balance_create(test_load_balance_network_switch_notify, true);
    POINTERS_EQUAL(load_balance_class, compare)

    int8_t status = compare->lb_initialize(NULL, &test_load_balance_beacon_interval_set, &test_load_balance_priority_get, &test_load_balance_network_switch_req, 50, lb_user);
    BYTES_EQUAL(-1, status);

    status = compare->lb_initialize(compare, NULL, test_load_balance_priority_get, test_load_balance_network_switch_req, 50, lb_user);
    BYTES_EQUAL(-1, status);

    status = compare->lb_initialize(compare, test_load_balance_beacon_interval_set, NULL, test_load_balance_network_switch_req, 50, lb_user);
    BYTES_EQUAL(-1, status);

    status = compare->lb_initialize(compare, test_load_balance_beacon_interval_set, test_load_balance_priority_get, NULL, 50, lb_user);
    BYTES_EQUAL(-1, status);

    status = compare->lb_initialize(compare, test_load_balance_beacon_interval_set, test_load_balance_priority_get, test_load_balance_network_switch_req, 50, NULL);
    BYTES_EQUAL(-1, status);

    status = compare->lb_initialize(compare, test_load_balance_beacon_interval_set, test_load_balance_priority_get, test_load_balance_network_switch_req, 50, lb_user);
    BYTES_EQUAL(-2, status);
    nsdynmemlib_stub.returnCounter = 1;
    status = compare->lb_initialize(compare, test_load_balance_beacon_interval_set, test_load_balance_priority_get, test_load_balance_network_switch_req, 50, lb_user);
    BYTES_EQUAL(0, status);

    compare = load_balance_create(test_load_balance_network_switch_notify, true);
    POINTERS_EQUAL(NULL, compare);

    status = load_balance_delete(load_balance_class);
    BYTES_EQUAL(0, status);

    status = load_balance_delete(load_balance_class);
    BYTES_EQUAL(-1, status);
}
static bool fail_node_cnt = false;
static uint16_t load_node_cnt = 0;
static uint8_t test_load_level = 0;

static int8_t test_load_balance_api_get_node_count(void *lb_user, uint16_t *node_count)
{
    if (fail_node_cnt) {
        return -1;
    }

    *node_count = load_node_cnt;
    return 0;
}

static int8_t test_load_balance_api_get_set_load_level(void *lb_user, uint8_t load_level)
{
    test_load_level = load_level;
    return 0;
}

TEST(LoadBalance, test_load_balance_load_update)
{
    load_balance_api_t *load_balance_class;
    void *lb_user = &test_priority;

    event_stub.int8_value = 0;
    nsdynmemlib_stub.returnCounter = 1;
    load_balance_class = load_balance_create(test_load_balance_network_switch_notify, true);
    if (!load_balance_class) {
        FAIL("Loadbalance create return NULL pointer");
    }

    nsdynmemlib_stub.returnCounter = 1;
    int8_t status = load_balance_class->lb_initialize(load_balance_class, test_load_balance_beacon_interval_set, test_load_balance_priority_get, test_load_balance_network_switch_req, 20, lb_user);
    BYTES_EQUAL(0, status);
    //Test Enable function
    status = load_balance_network_load_monitor_enable(NULL, 0, 0, NULL, NULL);
    BYTES_EQUAL(-1, status);

    status = load_balance_network_load_monitor_enable(load_balance_class, 0, 0, NULL, NULL);
    BYTES_EQUAL(-1, status);

    status = load_balance_network_load_monitor_enable(load_balance_class, 24, 0, NULL, NULL);
    BYTES_EQUAL(-1, status);

    status = load_balance_network_load_monitor_enable(load_balance_class, 24, 30, NULL, NULL);
    BYTES_EQUAL(-1, status);

    status = load_balance_network_load_monitor_enable(load_balance_class, 24, 8, NULL, NULL);
    BYTES_EQUAL(-1, status);

    status = load_balance_network_load_monitor_enable(load_balance_class, 24, 8, test_load_balance_api_get_node_count, test_load_balance_api_get_set_load_level);
    BYTES_EQUAL(-2, status);
    nsdynmemlib_stub.returnCounter = 1;
    status = load_balance_network_load_monitor_enable(load_balance_class, 24, 8, test_load_balance_api_get_node_count, test_load_balance_api_get_set_load_level);
    BYTES_EQUAL(0, status);

    //Test Load level polling
    load_balance_class->lb_enable(load_balance_class, true, 100, 100);

    fail_node_cnt = true;
    load_node_cnt = 0;
    test_load_level = 7;
    for (int i=0; i< 100; i++) {
        load_balance_class->lb_seconds_tick_update(load_balance_class);
    }
    BYTES_EQUAL(7, test_load_level);

    fail_node_cnt = false;
    for (int i=0; i< 100; i++) {
        load_balance_class->lb_seconds_tick_update(load_balance_class);
    }
    BYTES_EQUAL(0, test_load_level);

    load_node_cnt = 2;
    for (int i=0; i< 100; i++) {
        load_balance_class->lb_seconds_tick_update(load_balance_class);
    }
    BYTES_EQUAL(0, test_load_level);

    load_node_cnt = 3;
    for (int i=0; i< 100; i++) {
        load_balance_class->lb_seconds_tick_update(load_balance_class);
    }
    BYTES_EQUAL(1, test_load_level);

    load_node_cnt = 5;
    for (int i=0; i< 100; i++) {
        load_balance_class->lb_seconds_tick_update(load_balance_class);
    }
    BYTES_EQUAL(1, test_load_level);

    load_node_cnt = 7;
    for (int i=0; i< 100; i++) {
        load_balance_class->lb_seconds_tick_update(load_balance_class);
    }
    BYTES_EQUAL(2, test_load_level);

    load_node_cnt = 9;
    for (int i=0; i< 100; i++) {
        load_balance_class->lb_seconds_tick_update(load_balance_class);
    }
    BYTES_EQUAL(3, test_load_level);

    load_node_cnt = 12;
    for (int i=0; i< 100; i++) {
        load_balance_class->lb_seconds_tick_update(load_balance_class);
    }
    BYTES_EQUAL(4, test_load_level);

    load_node_cnt = 15;
    for (int i=0; i< 100; i++) {
        load_balance_class->lb_seconds_tick_update(load_balance_class);
    }
    BYTES_EQUAL(5, test_load_level);

    load_node_cnt = 18;
    for (int i=0; i< 100; i++) {
        load_balance_class->lb_seconds_tick_update(load_balance_class);
    }
    BYTES_EQUAL(6, test_load_level);

    load_node_cnt = 21;
    for (int i=0; i< 100; i++) {
        load_balance_class->lb_seconds_tick_update(load_balance_class);
    }
    BYTES_EQUAL(7, test_load_level);

    load_node_cnt = 24;
    for (int i=0; i< 100; i++) {
        load_balance_class->lb_seconds_tick_update(load_balance_class);
    }
    BYTES_EQUAL(7, test_load_level);

    load_node_cnt = 25;
    for (int i=0; i< 100; i++) {
        load_balance_class->lb_seconds_tick_update(load_balance_class);
    }
    BYTES_EQUAL(7, test_load_level);

    //Test Disable
    status = load_balance_network_load_monitor_disable(NULL);
    BYTES_EQUAL(-1, status);
    status = load_balance_network_load_monitor_disable(load_balance_class);
    BYTES_EQUAL(0, status);

    status = load_balance_network_load_monitor_disable(load_balance_class);
    BYTES_EQUAL(-1, status);

    status = load_balance_delete(load_balance_class);
    BYTES_EQUAL(0, status);

}

TEST(LoadBalance, test_load_balance_set_max_probability)
{
    load_balance_api_t *load_balance_class;
    void *lb_user = &test_priority;

    event_stub.int8_value = 0;
    nsdynmemlib_stub.returnCounter = 1;


    int status;
    status = load_balance_set_max_probability(NULL, 40);
    BYTES_EQUAL(-1, status);

    load_balance_class = load_balance_create(test_load_balance_network_switch_notify, true);
    if (!load_balance_class) {
        FAIL("Loadbalance create return NULL pointer");
    }

    status = load_balance_set_max_probability(load_balance_class, 0);
    BYTES_EQUAL(-1, status);

    status = load_balance_set_max_probability(load_balance_class, 101);
    BYTES_EQUAL(-1, status);

    status = load_balance_set_max_probability(load_balance_class, 1);
    BYTES_EQUAL(0, status);

    status = load_balance_set_max_probability(load_balance_class, 100);
    BYTES_EQUAL(0, status);


    status = load_balance_delete(load_balance_class);
    BYTES_EQUAL(0, status);

    status = load_balance_set_max_probability(load_balance_class, 40);
    BYTES_EQUAL(-1, status);

}


