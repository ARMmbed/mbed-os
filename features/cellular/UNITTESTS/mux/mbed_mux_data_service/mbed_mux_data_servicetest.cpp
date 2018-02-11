/*
 * Copyright (c) 2015 ARM. All rights reserved.
 */
#include "CppUTest/TestHarness.h"
#include "test_mbed_mux_data_service.h"

TEST_GROUP(mbed_mux_data_service)
{
    Test_mbed_mux_data_service* unit;

    void setup()
    {
        unit = new Test_mbed_mux_data_service();
    }

    void teardown()
    {
        delete unit;
    }
};

TEST(mbed_mux_data_service, Create)
{
    CHECK(unit != NULL);
}

TEST(mbed_mux_data_service, test_mbed_mux_write)
{
    unit->test_mbed_mux_write();
}

TEST(mbed_mux_data_service, test_mbed_mux_read)
{
    unit->test_mbed_mux_read();
}

TEST(mbed_mux_data_service, test_mbed_mux_seek)
{
    unit->test_mbed_mux_seek();
}

TEST(mbed_mux_data_service, test_mbed_mux_close)
{
    unit->test_mbed_mux_close();
}

TEST(mbed_mux_data_service, test_mbed_mux_sigio)
{
    unit->test_mbed_mux_sigio();
}
