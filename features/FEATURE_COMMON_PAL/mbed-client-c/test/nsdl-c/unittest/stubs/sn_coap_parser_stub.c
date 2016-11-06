/*
 * Copyright (c) 2015 ARM Limited. All rights reserved.
 * SPDX-License-Identifier: Apache-2.0
 * Licensed under the Apache License, Version 2.0 (the License); you may
 * not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an AS IS BASIS, WITHOUT
 * WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
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
#include "sn_nsdl.h"
#include "sn_coap_header.h"
#include "sn_coap_protocol_internal.h"
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
            freed_coap_msg_ptr->uri_path_ptr = NULL;
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

sn_coap_hdr_s *sn_coap_parser_init_message(sn_coap_hdr_s *coap_msg_ptr)
{
    /* * * * Check given pointer * * * */
    if (coap_msg_ptr == NULL) {
        return NULL;
    }

    /* XXX not technically legal to memset pointers to 0 */
    memset(coap_msg_ptr, 0x00, sizeof(sn_coap_hdr_s));

    return coap_msg_ptr;
}

sn_coap_hdr_s *sn_coap_parser_alloc_message(struct coap_s *handle)
{
    sn_coap_hdr_s *returned_coap_msg_ptr;

    /* * * * Check given pointer * * * */
    if (handle == NULL) {
        return NULL;
    }

    /* * * * Allocate memory for returned CoAP message and initialize allocated memory with with default values  * * * */
    returned_coap_msg_ptr = handle->sn_coap_protocol_malloc(sizeof(sn_coap_hdr_s));

    return sn_coap_parser_init_message(returned_coap_msg_ptr);
}

sn_coap_options_list_s *sn_coap_parser_alloc_options(struct coap_s *handle, sn_coap_hdr_s *coap_msg_ptr)
{
    /* * * * Check given pointers * * * */
    if (handle == NULL || coap_msg_ptr == NULL) {
        return NULL;
    }

    /* * * * If the message already has options, return them * * * */
    if (coap_msg_ptr->options_list_ptr) {
        return coap_msg_ptr->options_list_ptr;
    }

    /* * * * Allocate memory for options and initialize allocated memory with with default values  * * * */
    coap_msg_ptr->options_list_ptr = handle->sn_coap_protocol_malloc(sizeof(sn_coap_options_list_s));

    if (coap_msg_ptr->options_list_ptr == NULL) {
        return NULL;
    }

    /* XXX not technically legal to memset pointers to 0 */
    memset(coap_msg_ptr->options_list_ptr, 0x00, sizeof(sn_coap_options_list_s));

    return coap_msg_ptr->options_list_ptr;
}
