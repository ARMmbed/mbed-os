/*
 * Copyright (c) 2018 ARM. All rights reserved.
 */
#ifndef TEST_AT_CELLULARSMS_H
#define TEST_AT_CELLULARSMS_H

class Test_AT_CellularSMS
{
public:
    Test_AT_CellularSMS();

    virtual ~Test_AT_CellularSMS();

    void test_AT_CellularSMS_constructor();

    void test_AT_CellularSMS_initialize();

    void test_AT_CellularSMS_send_sms();

    void test_AT_CellularSMS_get_sms();

    void test_AT_CellularSMS_set_sms_callback();

    void test_AT_CellularSMS_set_cpms();

    void test_AT_CellularSMS_set_csca();

    void test_AT_CellularSMS_set_cscs();

    void test_AT_CellularSMS_delete_all_messages();

    void test_AT_CellularSMS_set_extra_sim_wait_time();
};

#endif // TEST_AT_CELLULARSMS_H

