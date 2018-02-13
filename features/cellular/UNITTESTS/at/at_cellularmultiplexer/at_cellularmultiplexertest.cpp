/*
 * Copyright (c) 2015 ARM. All rights reserved.
 */
#include "CppUTest/TestHarness.h"
#include "test_at_cellularmultiplexer.h"

TEST_GROUP(AT_CellularMultiplexer)
{
    Test_AT_CellularMultiplexer* unit;

    void setup()
    {
        unit = new Test_AT_CellularMultiplexer();
    }

    void teardown()
    {
        delete unit;
    }
};

TEST(AT_CellularMultiplexer, Create)
{
    CHECK(unit != NULL);
}

TEST(AT_CellularMultiplexer, test_AT_CellularMultiplexer_constructor)
{
    unit->test_AT_CellularMultiplexer_constructor();
}

TEST(AT_CellularMultiplexer, test_AT_CellularMultiplexer_multiplexer_mode_start)
{
    unit->test_AT_CellularMultiplexer_multiplexer_mode_start();
}

