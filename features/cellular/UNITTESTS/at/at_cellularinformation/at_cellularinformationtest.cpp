/*
 * Copyright (c) 2015 ARM. All rights reserved.
 */
#include "CppUTest/TestHarness.h"
#include "test_at_cellularinformation.h"

TEST_GROUP(AT_CellularInformation)
{
    Test_AT_CellularInformation* unit;

    void setup()
    {
        unit = new Test_AT_CellularInformation();
    }

    void teardown()
    {
        delete unit;
    }
};

TEST(AT_CellularInformation, Create)
{
    CHECK(unit != NULL);
}

TEST(AT_CellularInformation, test_AT_CellularInformation_get_manufacturer)
{
    unit->test_AT_CellularInformation_get_manufacturer();
}

TEST(AT_CellularInformation, test_AT_CellularInformation_get_model)
{
    unit->test_AT_CellularInformation_get_model();
}

TEST(AT_CellularInformation, test_AT_CellularInformation_get_revision)
{
    unit->test_AT_CellularInformation_get_revision();
}

