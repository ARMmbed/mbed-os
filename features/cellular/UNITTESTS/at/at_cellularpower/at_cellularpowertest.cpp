/*
 * Copyright (c) 2015 ARM. All rights reserved.
 */
#include "CppUTest/TestHarness.h"
#include "test_at_cellularpower.h"

TEST_GROUP(AT_CellularPower)
{
    Test_AT_CellularPower* unit;

    void setup()
    {
        unit = new Test_AT_CellularPower();
    }

    void teardown()
    {
        delete unit;
    }
};

TEST(AT_CellularPower, Create)
{
    CHECK(unit != NULL);
}

TEST(AT_CellularPower, test_AT_CellularPower_constructor)
{
    unit->test_AT_CellularPower_constructor();
}

TEST(AT_CellularPower, test_AT_CellularPower_on)
{
    unit->test_AT_CellularPower_on();
}

TEST(AT_CellularPower, test_AT_CellularPower_off)
{
    unit->test_AT_CellularPower_off();
}

TEST(AT_CellularPower, test_AT_CellularPower_set_at_mode)
{
    unit->test_AT_CellularPower_set_at_mode();
}

TEST(AT_CellularPower, test_AT_CellularPower_set_power_level)
{
    unit->test_AT_CellularPower_set_power_level();
}

TEST(AT_CellularPower, test_AT_CellularPower_reset)
{
    unit->test_AT_CellularPower_reset();
}

TEST(AT_CellularPower, test_AT_CellularPower_opt_power_save_mode)
{
    unit->test_AT_CellularPower_opt_power_save_mode();
}

TEST(AT_CellularPower, test_AT_CellularPower_opt_receive_period)
{
    unit->test_AT_CellularPower_opt_receive_period();
}

