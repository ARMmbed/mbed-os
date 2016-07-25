/*
 * Copyright (c) 2015 ARM. All rights reserved.
 */
#ifndef TEST_SN_GRS_H
#define TEST_SN_GRS_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdbool.h>

bool test_sn_grs_get_first_resource();
bool test_sn_grs_get_next_resource();
bool test_sn_grs_process_coap();
bool test_sn_grs_search_resource();
bool test_sn_grs_destroy();
bool test_sn_grs_init();
bool test_sn_grs_list_resource();
bool test_sn_grs_free_resource_list();
bool test_sn_grs_update_resource();
bool test_sn_grs_send_coap_message();
bool test_sn_grs_create_resource();
bool test_sn_grs_delete_resource();
bool test_sn_grs_mark_resources_as_registered();

#ifdef __cplusplus
}
#endif

#endif // TEST_SN_GRS_H

