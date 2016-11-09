/*
 * Copyright (c) 2016 ARM. All rights reserved.
 */
#include "CppUTest/TestHarness.h"
#include "test_randlib.h"

TEST_GROUP(randLIB)
{
    void setup()
    {
    }

    void teardown()
    {
    }
};

TEST(randLIB, test_randLIB_seed_random)
{
    CHECK(test_randLIB_seed_random());
}

TEST(randLIB, test_randLIB_get_8bit)
{
    CHECK(test_randLIB_get_8bit());
}

TEST(randLIB, test_randLIB_get_16bit)
{
    CHECK(test_randLIB_get_16bit());
}

TEST(randLIB, test_randLIB_get_32bit)
{
    CHECK(test_randLIB_get_32bit());
}

TEST(randLIB, test_randLIB_get_64bit)
{
    CHECK(test_randLIB_get_64bit());
}

TEST(randLIB, test_randLIB_get_n_bytes_random)
{
    CHECK(test_randLIB_get_n_bytes_random());
}

TEST(randLIB, test_randLIB_get_random_in_range)
{
    CHECK(test_randLIB_get_random_in_range());
}

TEST(randLIB, test_randLIB_randomise_base)
{
    CHECK(test_randLIB_randomise_base());
}
