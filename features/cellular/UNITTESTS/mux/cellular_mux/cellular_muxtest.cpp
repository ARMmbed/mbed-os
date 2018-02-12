/*
 * Copyright (c) 2015 ARM. All rights reserved.
 */
#include "CppUTest/TestHarness.h"
#include "test_cellular_mux.h"

TEST_GROUP(cellular_mux)
{
    Test_cellular_mux* unit;

    void setup()
    {
        unit = new Test_cellular_mux();
    }

    void teardown()
    {
        delete unit;
    }
};

TEST(cellular_mux, Create)
{
    CHECK(unit != NULL);
}




