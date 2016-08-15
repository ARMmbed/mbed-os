/*
 * Copyright (c) 2015 ARM. All rights reserved.
 */
#include "CppUTest/TestHarness.h"
#include "test_sn_nsdl.h"

TEST_GROUP(sn_nsdl)
{
    void setup()
    {
    }

    void teardown()
    {
    }
};

TEST(sn_nsdl, test_sn_nsdl_destroy)
{
    CHECK(test_sn_nsdl_destroy());
}

TEST(sn_nsdl, test_sn_nsdl_init)
{
    CHECK(test_sn_nsdl_init());
}

TEST(sn_nsdl, test_sn_nsdl_register_endpoint)
{
    CHECK(test_sn_nsdl_register_endpoint());
}

TEST(sn_nsdl, test_sn_nsdl_unregister_endpoint)
{
    CHECK(test_sn_nsdl_unregister_endpoint());
}

TEST(sn_nsdl, test_sn_nsdl_update_registration)
{
    CHECK(test_sn_nsdl_update_registration());
}

TEST(sn_nsdl, test_sn_nsdl_set_endpoint_location)
{
    CHECK(test_sn_nsdl_set_endpoint_location());
}

TEST(sn_nsdl, test_sn_nsdl_nsp_lost)
{
    CHECK(test_sn_nsdl_nsp_lost());
}

TEST(sn_nsdl, test_sn_nsdl_is_ep_registered)
{
    CHECK(test_sn_nsdl_is_ep_registered());
}

TEST(sn_nsdl, test_sn_nsdl_send_observation_notification)
{
    CHECK(test_sn_nsdl_send_observation_notification());
}

TEST(sn_nsdl, test_sn_nsdl_send_observation_notification_with_uri_path)
{
    CHECK(test_sn_nsdl_send_observation_notification_with_uri_path());
}

TEST(sn_nsdl, test_sn_nsdl_oma_bootstrap)
{
    CHECK(test_sn_nsdl_oma_bootstrap());
}

TEST(sn_nsdl, test_sn_nsdl_get_certificates)
{
    CHECK(test_sn_nsdl_get_certificates());
}

TEST(sn_nsdl, test_sn_nsdl_update_certificates)
{
    CHECK(test_sn_nsdl_update_certificates());
}

TEST(sn_nsdl, test_sn_nsdl_create_oma_device_object)
{
    CHECK(test_sn_nsdl_create_oma_device_object());
}

TEST(sn_nsdl, test_sn_nsdl_get_version)
{
    CHECK(test_sn_nsdl_get_version());
}

TEST(sn_nsdl, test_sn_nsdl_process_coap)
{
    CHECK(test_sn_nsdl_process_coap());
}


TEST(sn_nsdl, test_sn_nsdl_exec)
{
    CHECK(test_sn_nsdl_exec());
}

TEST(sn_nsdl, test_sn_nsdl_get_resource)
{
    CHECK(test_sn_nsdl_get_resource());
}

TEST(sn_nsdl, test_set_NSP_address)
{
    CHECK(test_set_NSP_address());
}

TEST(sn_nsdl, test_sn_nsdl_list_resource)
{
    CHECK(test_sn_nsdl_list_resource());
}

TEST(sn_nsdl, test_sn_nsdl_free_resource_list)
{
    CHECK(test_sn_nsdl_free_resource_list());
}

TEST(sn_nsdl, test_sn_nsdl_update_resource)
{
    CHECK(test_sn_nsdl_update_resource());
}

TEST(sn_nsdl, test_sn_nsdl_send_coap_message)
{
    CHECK(test_sn_nsdl_send_coap_message());
}

TEST(sn_nsdl, test_sn_nsdl_create_resource)
{
    CHECK(test_sn_nsdl_create_resource());
}

TEST(sn_nsdl, test_sn_nsdl_delete_resource)
{
    CHECK(test_sn_nsdl_delete_resource());
}

TEST(sn_nsdl, test_sn_nsdl_get_first_resource)
{
    CHECK(test_sn_nsdl_get_first_resource());
}

TEST(sn_nsdl, test_sn_nsdl_get_next_resource)
{
    CHECK(test_sn_nsdl_get_next_resource());
}

TEST(sn_nsdl, test_sn_nsdl_build_response)
{
    CHECK(test_sn_nsdl_build_response());
}

TEST(sn_nsdl, test_sn_nsdl_release_allocated_coap_msg_mem)
{
    CHECK(test_sn_nsdl_release_allocated_coap_msg_mem());
}

TEST(sn_nsdl, test_sn_nsdl_set_retransmission_parameters)
{
    CHECK(test_sn_nsdl_set_retransmission_parameters());
}

TEST(sn_nsdl, test_sn_nsdl_set_retransmission_buffer)
{
    CHECK(test_sn_nsdl_set_retransmission_buffer());
}

TEST(sn_nsdl, test_sn_nsdl_set_block_size)
{
    CHECK(test_sn_nsdl_set_block_size());
}

TEST(sn_nsdl, test_sn_nsdl_set_duplicate_buffer_size)
{
    CHECK(test_sn_nsdl_set_duplicate_buffer_size());
}



