/*
 * Copyright (c) 2015 ARM. All rights reserved.
 */
#include "CppUTest/TestHarness.h"
#include "test_at_cellularsim.h"

TEST_GROUP(AT_CellularSIM)
{
    Test_AT_CellularSIM* unit;

    void setup()
    {
        unit = new Test_AT_CellularSIM();
    }

    void teardown()
    {
        delete unit;
    }
};

TEST(AT_CellularSIM, Create)
{
    CHECK(unit != NULL);
}

TEST(AT_CellularSIM, test_AT_CellularSIM_constructor)
{
    unit->test_AT_CellularSIM_constructor();
}

TEST(AT_CellularSIM, test_AT_CellularSIM_set_pin)
{
    unit->test_AT_CellularSIM_set_pin();
}

TEST(AT_CellularSIM, test_AT_CellularSIM_change_pin)
{
    unit->test_AT_CellularSIM_change_pin();
}

TEST(AT_CellularSIM, test_AT_CellularSIM_set_pin_query)
{
    unit->test_AT_CellularSIM_set_pin_query();
}

TEST(AT_CellularSIM, test_AT_CellularSIM_get_sim_state)
{
    unit->test_AT_CellularSIM_get_sim_state();
}

