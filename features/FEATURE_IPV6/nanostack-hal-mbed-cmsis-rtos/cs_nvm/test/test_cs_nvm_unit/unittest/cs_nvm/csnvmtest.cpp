/*
 * Copyright (c) 2016 ARM Limited. All rights reserved.
 */

#include "CppUTest/TestHarness.h"
#include "test_cs_nvm.h"

TEST_GROUP(CS_NVM)
{
    void setup() {

    }

    void teardown() {

    }
};

TEST(CS_NVM, test_cs_nvm_operation_in_progress)
{
    CHECK(test_cs_nvm_operation_in_progress() == true);
}

TEST(CS_NVM, test_cs_nvm_flush)
{
    CHECK(test_cs_nvm_flush() == true);
}

TEST(CS_NVM, test_cs_nvm_write_failure_in_close)
{
    CHECK(test_cs_nvm_write_failure_in_close() == true);
}

TEST(CS_NVM, test_cs_nvm_write_failure)
{
    CHECK(test_cs_nvm_write_failure() == true);
}

TEST(CS_NVM, test_cs_nvm_write)
{
    CHECK(test_cs_nvm_write() == true);
}

TEST(CS_NVM, test_cs_nvm_read_failure_in_close)
{
    CHECK(test_cs_nvm_read_failure_in_close() == true);
}

TEST(CS_NVM, test_cs_nvm_read_failure)
{
    CHECK(test_cs_nvm_read_failure() == true);
}

TEST(CS_NVM, test_cs_nvm_read)
{
    CHECK(test_cs_nvm_read() == true);
}

TEST(CS_NVM, test_cs_nvm_key_delete_failure_in_close)
{
    CHECK(test_cs_nvm_key_delete_failure_in_close() == true);
}

TEST(CS_NVM, test_cs_nvm_key_delete)
{
    CHECK(test_cs_nvm_key_delete() == true);
}

TEST(CS_NVM, test_cs_nvm_key_create)
{
    CHECK(test_cs_nvm_key_create() == true);
}

TEST(CS_NVM, test_cs_nvm_init_finalize)
{
    CHECK(test_cs_nvm_init_finalize() == true);
}

