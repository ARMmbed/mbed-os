/*
 * Copyright (c) 2016 ARM Limited. All rights reserved.
 */
#include "CppUTest/TestHarness.h"
#include "test_ns_nvm_helper.h"

TEST_GROUP(NS_NVM_HELPER)
{
    void setup() {

    }

    void teardown() {

    }
};

TEST(NS_NVM_HELPER, test_ns_nvm_helper_platform_error_in_write)
{
    CHECK(test_ns_nvm_helper_platform_error_in_write() == true);
}

TEST(NS_NVM_HELPER, test_ns_nvm_helper_platform_error)
{
    CHECK(test_ns_nvm_helper_platform_error() == true);
}

TEST(NS_NVM_HELPER, test_ns_nvm_helper_concurrent_requests)
{
    CHECK(test_ns_nvm_helper_concurrent_requests() == true);
}

TEST(NS_NVM_HELPER, test_ns_nvm_helper_read)
{
    CHECK(test_ns_nvm_helper_read() == true);
}

TEST(NS_NVM_HELPER, test_ns_nvm_helper_delete)
{
    CHECK(test_ns_nvm_helper_delete() == true);
}

/* Write must be the last test (executed first) as it initialized the platform NVM */
TEST(NS_NVM_HELPER, test_ns_nvm_helper_write)
{
    CHECK(test_ns_nvm_helper_write() == true);
}

