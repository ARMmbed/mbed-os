/*
 * Copyright (c) 2011-2017 ARM Limited. All Rights Reserved.
 */

/**
 *\file sn_coap_parser.c
 *
 * \brief CoAP Header parser
 *
 * Functionality: Parses CoAP Header
 *
 */

#include "ns_types.h"
#include "sn_coap_protocol.h"
#include "sn_coap_parser_stub.h"

sn_coap_parser_def sn_coap_parser_stub;

sn_coap_hdr_s *sn_coap_parser(struct coap_s *handle, uint16_t packet_data_len, uint8_t *packet_data_ptr, coap_version_e *coap_version_ptr)
{
    return sn_coap_parser_stub.expectedHeader;
}

void sn_coap_parser_release_allocated_coap_msg_mem(struct coap_s *handle, sn_coap_hdr_s *freed_coap_msg_ptr)
{
    if (freed_coap_msg_ptr != NULL) {
        if (freed_coap_msg_ptr->uri_path_ptr != NULL) {
            free(freed_coap_msg_ptr->uri_path_ptr);
        }

        if (freed_coap_msg_ptr->token_ptr != NULL) {
            free(freed_coap_msg_ptr->token_ptr);
        }

        if (freed_coap_msg_ptr->options_list_ptr != NULL) {
            if (freed_coap_msg_ptr->options_list_ptr->proxy_uri_ptr != NULL) {
                free(freed_coap_msg_ptr->options_list_ptr->proxy_uri_ptr);
            }

            if (freed_coap_msg_ptr->options_list_ptr->etag_ptr != NULL) {
                free(freed_coap_msg_ptr->options_list_ptr->etag_ptr);
            }

            if (freed_coap_msg_ptr->options_list_ptr->uri_host_ptr != NULL) {
                free(freed_coap_msg_ptr->options_list_ptr->uri_host_ptr);
            }

            if (freed_coap_msg_ptr->options_list_ptr->location_path_ptr != NULL) {
                free(freed_coap_msg_ptr->options_list_ptr->location_path_ptr);
            }

            if (freed_coap_msg_ptr->options_list_ptr->location_query_ptr != NULL) {
                free(freed_coap_msg_ptr->options_list_ptr->location_query_ptr);
            }

            if (freed_coap_msg_ptr->options_list_ptr->uri_query_ptr != NULL) {
                free(freed_coap_msg_ptr->options_list_ptr->uri_query_ptr);
            }

            free(freed_coap_msg_ptr->options_list_ptr);
        }

        free(freed_coap_msg_ptr);
        freed_coap_msg_ptr = NULL;
    }
}
