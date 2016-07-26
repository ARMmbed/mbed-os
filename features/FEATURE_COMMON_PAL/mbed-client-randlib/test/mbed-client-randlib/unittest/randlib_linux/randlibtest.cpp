/*
 * Copyright (c) 2016 ARM. All rights reserved.
 */
#include "CppUTest/TestHarness.h"
#include "../randlib/test_randlib.h"
#include "randLIB.h"

TEST_GROUP(randLIB_linux)
{
    void setup()
    {
    }

    void teardown()
    {
    }
};

extern bool allow_open;

TEST(randLIB_linux, test_randLIB_seed_random)
{
    allow_open = true;
    CHECK(test_randLIB_seed_random());
}

TEST(randLIB_linux, test_randLIB_get_8bit)
{
    allow_open = true;
    CHECK(test_randLIB_get_8bit());
}

TEST(randLIB_linux, test_randLIB_get_16bit)
{
    allow_open = true;
    CHECK(test_randLIB_get_16bit());
}

TEST(randLIB_linux, test_randLIB_get_32bit)
{
    allow_open = true;
    CHECK(test_randLIB_get_32bit());
}

TEST(randLIB_linux, test_randLIB_get_n_bytes_random)
{
    allow_open = true;
    CHECK(test_randLIB_get_n_bytes_random());
}

TEST(randLIB_linux, test_randLIB_get_random_in_range)
{
    allow_open = true;
    CHECK(test_randLIB_get_random_in_range());
}

TEST(randLIB_linux, test_randLIB_randomise_base)
{
    allow_open = true;
    CHECK(test_randLIB_randomise_base());
}

TEST(randLIB_linux, test_fail_to_open)
{
    uint8_t buf[4];
    allow_open = false;
    CHECK(-1 == randLIB_get_n_bytes_random(buf, 4));
    allow_open = true;
}
