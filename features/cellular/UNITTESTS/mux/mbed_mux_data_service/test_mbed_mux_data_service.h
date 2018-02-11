/*
 * Copyright (c) 2018 ARM. All rights reserved.
 */
#ifndef TEST_MBED_MUX_DATA_SERVICE_H
#define TEST_MBED_MUX_DATA_SERVICE_H

class Test_mbed_mux_data_service
{
public:
    Test_mbed_mux_data_service();

    virtual ~Test_mbed_mux_data_service();

    void test_mbed_mux_write();

    void test_mbed_mux_read();

    void test_mbed_mux_seek();

    void test_mbed_mux_close();

    void test_mbed_mux_sigio();
};

#endif // TEST_MBED_MUX_DATA_SERVICE_H

