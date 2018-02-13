/*
 * Copyright (c) 2018 ARM. All rights reserved.
 */
#ifndef TEST_AT_CELLULARDEVICE_H
#define TEST_AT_CELLULARDEVICE_H

class Test_AT_CellularDevice
{
public:
    Test_AT_CellularDevice();

    virtual ~Test_AT_CellularDevice();

    void test_AT_CellularDevice_constructor();

    void test_AT_CellularDevice_get_at_handler(); //tests also releasing of those

    void test_AT_CellularDevice_open_network();

    void test_AT_CellularDevice_open_sms();

    void test_AT_CellularDevice_open_power();

    void test_AT_CellularDevice_open_sim();

    void test_AT_CellularDevice_open_multiplexer();

    void test_AT_CellularDevice_open_information();

    void test_AT_CellularDevice_close_network();

    void test_AT_CellularDevice_close_sms();

    void test_AT_CellularDevice_close_power();

    void test_AT_CellularDevice_close_sim();

    void test_AT_CellularDevice_close_multiplexer();

    void test_AT_CellularDevice_close_information();
};

#endif // TEST_AT_CELLULARDEVICE_H

