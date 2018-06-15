/*
 * Copyright (c) 2018, Arm Limited and affiliates.
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
#ifndef TEST_AT_CELLULARNETWORK_H
#define TEST_AT_CELLULARNETWORK_H

class Test_AT_CellularNetwork
{
public:
    Test_AT_CellularNetwork();

    virtual ~Test_AT_CellularNetwork();

    void test_AT_CellularNetwork_constructor();

    void test_AT_CellularNetwork_init();

    void test_AT_CellularNetwork_set_credentials();

    void test_AT_CellularNetwork_activate_context();

    void test_AT_CellularNetwork_connect();

    void test_AT_CellularNetwork_disconnect();

    void test_AT_CellularNetwork_get_stack();

    void test_AT_CellularNetwork_set_registration();

    void test_AT_CellularNetwork_get_registration_status();

    void test_AT_CellularNetwork_get_network_registering_mode();

    void test_AT_CellularNetwork_set_registration_urc();

    void test_AT_CellularNetwork_set_attach();

    void test_AT_CellularNetwork_get_attach();

    void test_AT_CellularNetwork_detach();

    void test_AT_CellularNetwork_get_rate_control();

    void test_AT_CellularNetwork_get_apn_backoff_timer();

    void test_AT_CellularNetwork_get_ip_address();

    void test_AT_CellularNetwork_set_access_technology();

    void test_AT_CellularNetwork_get_access_technology();

    void test_AT_CellularNetwork_scan_plmn();

    void test_AT_CellularNetwork_set_ciot_optimization_config();

    void test_AT_CellularNetwork_get_ciot_optimization_config();

    void test_AT_CellularNetwork_set_stack_type();

    void test_AT_CellularNetwork_get_stack_type();

    void test_AT_CellularNetwork_get_pdpcontext_params();

    void test_AT_CellularNetwork_get_extended_signal_quality();

    void test_AT_CellularNetwork_get_signal_quality();

    void test_AT_CellularNetwork_get_cell_id();

    void test_AT_CellularNetwork_get_3gpp_error();

    void test_AT_CellularNetwork_get_operator_params();

    void test_AT_CellularNetwork_get_operator_names();

    void test_AT_CellularNetwork_attach();

    void test_get_connection_status();

    void test_set_blocking();
};

#endif // TEST_AT_CELLULARNETWORK_H

