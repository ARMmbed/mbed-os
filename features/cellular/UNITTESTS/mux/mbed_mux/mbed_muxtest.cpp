/*
 * Copyright (c) 2015 ARM. All rights reserved.
 */
#include "CppUTest/TestHarness.h"
#include "test_mbed_mux.h"

TEST_GROUP(mbed_mux)
{
    Test_mbed_mux* unit;

    void setup()
    {
        unit = new Test_mbed_mux();
    }

    void teardown()
    {
        delete unit;
    }
};

TEST(mbed_mux, Create)
{
    CHECK(unit != NULL);
}




