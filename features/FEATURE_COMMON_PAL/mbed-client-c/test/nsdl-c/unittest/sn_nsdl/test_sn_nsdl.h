/*
 * Copyright (c) 2015 ARM. All rights reserved.
 */
#ifndef TEST_SN_NSDL_H
#define TEST_SN_NSDL_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdbool.h>

bool test_sn_nsdl_destroy();

bool test_sn_nsdl_init();

bool test_sn_nsdl_register_endpoint();

bool test_sn_nsdl_unregister_endpoint();

bool test_sn_nsdl_update_registration();

bool test_sn_nsdl_set_endpoint_location();

bool test_sn_nsdl_nsp_lost();

bool test_sn_nsdl_is_ep_registered();

bool test_sn_nsdl_send_observation_notification();

bool test_sn_nsdl_send_observation_notification_with_uri_path();

bool test_sn_nsdl_oma_bootstrap();

bool test_sn_nsdl_get_certificates();

bool test_sn_nsdl_update_certificates();

bool test_sn_nsdl_create_oma_device_object();

bool test_sn_nsdl_get_version();

bool test_sn_nsdl_process_coap();

bool test_sn_nsdl_exec();

bool test_sn_nsdl_get_resource();

bool test_set_NSP_address();

bool test_sn_nsdl_list_resource();

bool test_sn_nsdl_free_resource_list();

bool test_sn_nsdl_update_resource();

bool test_sn_nsdl_send_coap_message();

bool test_sn_nsdl_create_resource();

bool test_sn_nsdl_put_resource();

bool test_sn_nsdl_delete_resource();

bool test_sn_nsdl_get_first_resource();

bool test_sn_nsdl_get_next_resource();

bool test_sn_nsdl_build_response();

bool test_sn_nsdl_release_allocated_coap_msg_mem();

bool test_sn_nsdl_set_retransmission_parameters();

bool test_sn_nsdl_set_retransmission_buffer();

bool test_sn_nsdl_set_block_size();

bool test_sn_nsdl_set_duplicate_buffer_size();

#ifdef __cplusplus
}
#endif

#endif // TEST_SN_NSDL_H

