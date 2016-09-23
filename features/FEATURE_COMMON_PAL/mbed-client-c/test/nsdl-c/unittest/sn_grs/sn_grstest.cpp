/*
 * Copyright (c) 2015 ARM. All rights reserved.
 */
#include "CppUTest/TestHarness.h"
#include "test_sn_grs.h"

TEST_GROUP(sn_grs)
{
    void setup()
    {
    }

    void teardown()
    {
    }
};

TEST(sn_grs, test_sn_grs_get_first_resource)
{
    CHECK(test_sn_grs_get_first_resource());
}


TEST(sn_grs, test_sn_grs_get_next_resource)
{
    CHECK(test_sn_grs_get_next_resource());
}

TEST(sn_grs, test_sn_grs_process_coap)
{
    CHECK(test_sn_grs_process_coap());
}

TEST(sn_grs, test_sn_grs_search_resource)
{
    CHECK(test_sn_grs_search_resource());
}

TEST(sn_grs, test_sn_grs_destroy)
{
    CHECK(test_sn_grs_destroy());
}

TEST(sn_grs, test_sn_grs_init)
{
    CHECK(test_sn_grs_init());
}

TEST(sn_grs, test_sn_grs_list_resource)
{
    CHECK(test_sn_grs_list_resource());
}

TEST(sn_grs, test_sn_grs_free_resource_list)
{
    CHECK(test_sn_grs_free_resource_list());
}

TEST(sn_grs, test_sn_grs_update_resource)
{
    CHECK(test_sn_grs_update_resource());
}

TEST(sn_grs, test_sn_grs_send_coap_message)
{
    CHECK(test_sn_grs_send_coap_message());
}

TEST(sn_grs, test_sn_grs_create_resource)
{
    CHECK(test_sn_grs_create_resource());
}

TEST(sn_grs, test_sn_grs_put_resource)
{
    CHECK(test_sn_grs_put_resource());
}

TEST(sn_grs, test_sn_grs_delete_resource)
{
    CHECK(test_sn_grs_delete_resource());
}

TEST(sn_grs, test_sn_grs_mark_resources_as_registered)
{
    CHECK(test_sn_grs_mark_resources_as_registered());
}

