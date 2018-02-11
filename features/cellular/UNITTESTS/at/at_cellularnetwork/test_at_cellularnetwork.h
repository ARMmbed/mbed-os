/*
 * Copyright (c) 2018 ARM. All rights reserved.
 */
#ifndef TEST_AT_CELLULARNETWORK_H
#define TEST_AT_CELLULARNETWORK_H

class Test_AT_CellularNetwork
{
public:
    Test_AT_CellularNetwork();

    virtual ~Test_AT_CellularNetwork();

    void test_AT_CellularNetwork_constructor();

    void test_AT_CellularNetwork_set_credentials();

    void test_AT_CellularNetwork_connect();

    void test_AT_CellularNetwork_disconnect();

    void test_AT_CellularNetwork_get_stack();

    void test_AT_CellularNetwork_set_registration();

    void test_AT_CellularNetwork_get_registration_status();

    void test_AT_CellularNetwork_set_attach();

    void test_AT_CellularNetwork_get_attach();

    void test_AT_CellularNetwork_get_rate_control();

    void test_AT_CellularNetwork_get_backoff_time();

    void test_AT_CellularNetwork_connection_status_cb();

    void test_AT_CellularNetwork_get_ip_address();

    void test_AT_CellularNetwork_set_access_technology();

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
};

#endif // TEST_AT_CELLULARNETWORK_H

