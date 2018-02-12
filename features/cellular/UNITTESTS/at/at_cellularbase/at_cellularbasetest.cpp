/*
 * Copyright (c) 2015 ARM. All rights reserved.
 */
#include "CppUTest/TestHarness.h"
#include "test_at_cellularbase.h"
#include "AT_CellularBase.h"

TEST_GROUP(AT_CellularBase)
{
    Test_AT_CellularBase* unit;

    void setup()
    {
        unit = new Test_AT_CellularBase();
    }

    void teardown()
    {
        delete unit;
    }
};

TEST(AT_CellularBase, Create)
{
    CHECK(unit != NULL);
}

TEST(AT_CellularBase, test_AT_CellularBase_get_at_handler)
{
    unit->test_AT_CellularBase_get_at_handler();
}

TEST(AT_CellularBase, test_AT_CellularBase_get_device_error)
{
    unit->test_AT_CellularBase_get_device_error();
}

