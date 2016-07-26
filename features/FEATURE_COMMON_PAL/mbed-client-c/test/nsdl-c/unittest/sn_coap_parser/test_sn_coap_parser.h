/*
 * Copyright (c) 2015 ARM. All rights reserved.
 */
#ifndef TEST_SN_COAP_PARSER_H
#define TEST_SN_COAP_PARSER_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdbool.h>

bool test_sn_coap_parser();

//These splits above test to keep things more simpler
bool test_sn_coap_parser_options_parsing();
bool test_sn_coap_parser_options_parsing_switches();

bool test_sn_coap_parser_options_count_needed_memory_multiple_option();

bool test_sn_coap_parser_options_parse_multiple_options();

bool test_sn_coap_parser_parsing();
//<- split

bool test_sn_coap_parser_release_allocated_coap_msg_mem();


#ifdef __cplusplus
}
#endif

#endif // TEST_SN_COAP_PARSER_H

