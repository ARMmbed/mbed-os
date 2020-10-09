/*
 * Copyright (c) 2011-2017, Arm Limited and affiliates.
 * SPDX-License-Identifier: Apache-2.0
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
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
