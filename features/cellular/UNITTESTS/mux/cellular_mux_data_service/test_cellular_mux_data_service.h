/*
 * Copyright (c) 2018 ARM. All rights reserved.
 */
#ifndef TEST_cellular_mux_DATA_SERVICE_H
#define TEST_cellular_mux_DATA_SERVICE_H

class Test_cellular_mux_data_service
{
public:
    Test_cellular_mux_data_service();

    virtual ~Test_cellular_mux_data_service();

    void test_cellular_mux_write();

    void test_cellular_mux_read();

    void test_cellular_mux_seek();

    void test_cellular_mux_close();

    void test_cellular_mux_sigio();
};

#endif // TEST_cellular_mux_DATA_SERVICE_H

