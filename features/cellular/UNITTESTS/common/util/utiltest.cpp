/*
 * Copyright (c) 2015 ARM. All rights reserved.
 */
#include "CppUTest/TestHarness.h"
#include "test_util.h"

TEST_GROUP(util)
{
    Test_util* unit;

    void setup()
    {
        unit = new Test_util();
    }

    void teardown()
    {
        delete unit;
    }
};

TEST(util, Create)
{
    CHECK(unit != NULL);
}

TEST(util, test_util_uint_to_binary_string)
{
    unit->test_util_uint_to_binary_string();
}

TEST(util, char_str_to_hex)
{
    unit->test_util_char_str_to_hex();
}

TEST(util, convert_ipv6)
{
    unit->test_util_convert_ipv6();
}

TEST(util, prefer_ipv6)
{
    unit->test_util_prefer_ipv6();
}

TEST(util, separate_ip_addresses)
{
    unit->test_util_separate_ip_addresses();
}
