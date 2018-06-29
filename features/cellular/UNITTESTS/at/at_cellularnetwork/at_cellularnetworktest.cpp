/*
 * Copyright (c) 2015, Arm Limited and affiliates.
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
#include "CppUTest/TestHarness.h"
#include "test_at_cellularnetwork.h"
#include "ATHandler_stub.h"

TEST_GROUP(AT_CellularNetwork)
{
    Test_AT_CellularNetwork* unit;

    void setup()
    {
        unit = new Test_AT_CellularNetwork();
        ATHandler_stub::int_count = kRead_int_table_size;
        ATHandler_stub::read_string_index = kRead_string_table_size;
        ATHandler_stub::resp_stop_success_count = kResp_stop_count_default;
    }

    void teardown()
    {
        delete unit;
    }
};

TEST(AT_CellularNetwork, Create)
{
    CHECK(unit != NULL);
}

TEST(AT_CellularNetwork, test_AT_CellularNetwork_constructor)
{
    unit->test_AT_CellularNetwork_constructor();
}

TEST(AT_CellularNetwork, test_AT_CellularNetwork_init)
{
    unit->test_AT_CellularNetwork_init();
}

TEST(AT_CellularNetwork, test_AT_CellularNetwork_set_credentials)
{
    unit->test_AT_CellularNetwork_set_credentials();
}

TEST(AT_CellularNetwork, test_AT_CellularNetwork_activate_context)
{
    unit->test_AT_CellularNetwork_activate_context();
}

TEST(AT_CellularNetwork, test_AT_CellularNetwork_connect)
{
    unit->test_AT_CellularNetwork_connect();
}

TEST(AT_CellularNetwork, test_AT_CellularNetwork_disconnect)
{
    unit->test_AT_CellularNetwork_disconnect();
}

TEST(AT_CellularNetwork, test_AT_CellularNetwork_get_stack)
{
    unit->test_AT_CellularNetwork_get_stack();
}

TEST(AT_CellularNetwork, test_AT_CellularNetwork_set_registration)
{
    unit->test_AT_CellularNetwork_set_registration();
}

TEST(AT_CellularNetwork, test_AT_CellularNetwork_get_registration_status)
{
    unit->test_AT_CellularNetwork_get_registration_status();
}

TEST(AT_CellularNetwork, test_AT_CellularNetwork_get_network_registering_mode)
{
    unit->test_AT_CellularNetwork_get_network_registering_mode();
}

TEST(AT_CellularNetwork, test_AT_CellularNetwork_set_registration_urc)
{
    unit->test_AT_CellularNetwork_set_registration_urc();
}

TEST(AT_CellularNetwork, test_AT_CellularNetwork_set_attach)
{
    unit->test_AT_CellularNetwork_set_attach();
}

TEST(AT_CellularNetwork, test_AT_CellularNetwork_get_attach)
{
    unit->test_AT_CellularNetwork_get_attach();
}

TEST(AT_CellularNetwork, test_AT_CellularNetwork_detach)
{
    unit->test_AT_CellularNetwork_detach();
}

TEST(AT_CellularNetwork, test_AT_CellularNetwork_get_rate_control)
{
    unit->test_AT_CellularNetwork_get_rate_control();
}

TEST(AT_CellularNetwork, test_AT_CellularNetwork_get_apn_backoff_timer)
{
    unit->test_AT_CellularNetwork_get_apn_backoff_timer();
}

TEST(AT_CellularNetwork, test_AT_CellularNetwork_get_ip_address)
{
    unit->test_AT_CellularNetwork_get_ip_address();
}

TEST(AT_CellularNetwork, test_AT_CellularNetwork_set_access_technology)
{
    unit->test_AT_CellularNetwork_set_access_technology();
}

TEST(AT_CellularNetwork, test_AT_CellularNetwork_get_access_technology)
{
    unit->test_AT_CellularNetwork_get_access_technology();
}

TEST(AT_CellularNetwork, test_AT_CellularNetwork_scan_plmn)
{
    unit->test_AT_CellularNetwork_scan_plmn();
}

TEST(AT_CellularNetwork, test_AT_CellularNetwork_set_ciot_optimization_config)
{
    unit->test_AT_CellularNetwork_set_ciot_optimization_config();
}

TEST(AT_CellularNetwork, test_AT_CellularNetwork_get_ciot_optimization_config)
{
    unit->test_AT_CellularNetwork_get_ciot_optimization_config();
}

TEST(AT_CellularNetwork, test_AT_CellularNetwork_set_stack_type)
{
    unit->test_AT_CellularNetwork_set_stack_type();
}

TEST(AT_CellularNetwork, test_AT_CellularNetwork_get_stack_type)
{
    unit->test_AT_CellularNetwork_get_stack_type();
}

TEST(AT_CellularNetwork, test_AT_CellularNetwork_get_pdpcontext_params)
{
    unit->test_AT_CellularNetwork_get_pdpcontext_params();
}

TEST(AT_CellularNetwork, test_AT_CellularNetwork_get_extended_signal_quality)
{
    unit->test_AT_CellularNetwork_get_extended_signal_quality();
}

TEST(AT_CellularNetwork, test_AT_CellularNetwork_get_signal_quality)
{
    unit->test_AT_CellularNetwork_get_signal_quality();
}

TEST(AT_CellularNetwork, test_AT_CellularNetwork_get_cell_id)
{
    unit->test_AT_CellularNetwork_get_cell_id();
}

TEST(AT_CellularNetwork, test_AT_CellularNetwork_get_3gpp_error)
{
    unit->test_AT_CellularNetwork_get_3gpp_error();
}

TEST(AT_CellularNetwork, test_AT_CellularNetwork_get_operator_params)
{
    unit->test_AT_CellularNetwork_get_operator_params();
}

TEST(AT_CellularNetwork, test_AT_CellularNetwork_get_operator_names)
{
    unit->test_AT_CellularNetwork_get_operator_names();
}

TEST(AT_CellularNetwork, test_AT_CellularNetwork_attach)
{
    unit->test_AT_CellularNetwork_attach();
}

TEST(AT_CellularNetwork, test_get_connection_status)
{
    unit->test_get_connection_status();
}

TEST(AT_CellularNetwork, test_set_blocking)
{
    unit->test_set_blocking();
}

