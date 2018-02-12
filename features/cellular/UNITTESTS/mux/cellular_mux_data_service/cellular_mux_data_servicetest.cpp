/*
 * Copyright (c) 2015 ARM. All rights reserved.
 */
#include "CppUTest/TestHarness.h"
#include "test_cellular_mux_data_service.h"

TEST_GROUP(cellular_mux_data_service)
{
    Test_cellular_mux_data_service* unit;

    void setup()
    {
        unit = new Test_cellular_mux_data_service();
    }

    void teardown()
    {
        delete unit;
    }
};

TEST(cellular_mux_data_service, Create)
{
    CHECK(unit != NULL);
}

TEST(cellular_mux_data_service, test_cellular_mux_write)
{
    unit->test_cellular_mux_write();
}

TEST(cellular_mux_data_service, test_cellular_mux_read)
{
    unit->test_cellular_mux_read();
}

TEST(cellular_mux_data_service, test_cellular_mux_seek)
{
    unit->test_cellular_mux_seek();
}

TEST(cellular_mux_data_service, test_cellular_mux_close)
{
    unit->test_cellular_mux_close();
}

TEST(cellular_mux_data_service, test_cellular_mux_sigio)
{
    unit->test_cellular_mux_sigio();
}
