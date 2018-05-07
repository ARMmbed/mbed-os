/*
 * Copyright (c) 2011-2015 ARM Limited. All rights reserved.
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

/* * * * * * * * * * * * * * */
/* * * * INCLUDE FILES * * * */
/* * * * * * * * * * * * * * */

#include <stdio.h>
#include <string.h> /* For memset() and memcpy() */

#include "ns_types.h"
#include "mbed-coap/sn_coap_header.h"
#include "mbed-coap/sn_coap_protocol.h"
#include "sn_coap_header_internal.h"
#include "sn_coap_protocol_internal.h"
#include "mbed-trace/mbed_trace.h"

#define TRACE_GROUP "coap"
/* * * * * * * * * * * * * * * * * * * * */
/* * * * LOCAL FUNCTION PROTOTYPES * * * */
/* * * * * * * * * * * * * * * * * * * * */

static void     sn_coap_parser_header_parse(uint8_t **packet_data_pptr, sn_coap_hdr_s *dst_coap_msg_ptr, coap_version_e *coap_version_ptr);
static int8_t   sn_coap_parser_options_parse(struct coap_s *handle, uint8_t **packet_data_pptr, sn_coap_hdr_s *dst_coap_msg_ptr, uint8_t *packet_data_start_ptr, uint16_t packet_len);
static int8_t   sn_coap_parser_options_parse_multiple_options(struct coap_s *handle, uint8_t **packet_data_pptr, uint16_t packet_left_len,  uint8_t **dst_pptr, uint16_t *dst_len_ptr, sn_coap_option_numbers_e option, uint16_t option_number_len);
static int16_t  sn_coap_parser_options_count_needed_memory_multiple_option(uint8_t *packet_data_ptr, uint16_t packet_left_len, sn_coap_option_numbers_e option, uint16_t option_number_len);
static int8_t   sn_coap_parser_payload_parse(uint16_t packet_data_len, uint8_t *packet_data_start_ptr, uint8_t **packet_data_pptr, sn_coap_hdr_s *dst_coap_msg_ptr);

sn_coap_hdr_s *sn_coap_parser_init_message(sn_coap_hdr_s *coap_msg_ptr)
{
    /* * * * Check given pointer * * * */
    if (coap_msg_ptr == NULL) {
        tr_error("sn_coap_parser_init_message - message null!");
        return NULL;
    }

    /* XXX not technically legal to memset pointers to 0 */
    memset(coap_msg_ptr, 0x00, sizeof(sn_coap_hdr_s));

    coap_msg_ptr->content_format = COAP_CT_NONE;

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
        tr_error("sn_coap_parser_alloc_options - failed to allocate options list!");
        return NULL;
    }

    /* XXX not technically legal to memset pointers to 0 */
    memset(coap_msg_ptr->options_list_ptr, 0x00, sizeof(sn_coap_options_list_s));

    coap_msg_ptr->options_list_ptr->max_age = COAP_OPTION_MAX_AGE_DEFAULT;
    coap_msg_ptr->options_list_ptr->uri_port = COAP_OPTION_URI_PORT_NONE;
    coap_msg_ptr->options_list_ptr->observe = COAP_OBSERVE_NONE;
    coap_msg_ptr->options_list_ptr->accept = COAP_CT_NONE;
    coap_msg_ptr->options_list_ptr->block2 = COAP_OPTION_BLOCK_NONE;
    coap_msg_ptr->options_list_ptr->block1 = COAP_OPTION_BLOCK_NONE;

    return coap_msg_ptr->options_list_ptr;
}

sn_coap_hdr_s *sn_coap_parser(struct coap_s *handle, uint16_t packet_data_len, uint8_t *packet_data_ptr, coap_version_e *coap_version_ptr)
{
    uint8_t       *data_temp_ptr                    = packet_data_ptr;
    sn_coap_hdr_s *parsed_and_returned_coap_msg_ptr = NULL;

    /* * * * Check given pointer * * * */
    if (packet_data_ptr == NULL || packet_data_len < 4 || handle == NULL) {
        return NULL;
    }

    /* * * * Allocate and initialize CoAP message  * * * */
    parsed_and_returned_coap_msg_ptr = sn_coap_parser_alloc_message(handle);

    if (parsed_and_returned_coap_msg_ptr == NULL) {
        tr_error("sn_coap_parser - failed to allocate message!");
        return NULL;
    }

    /* * * * Header parsing, move pointer over the header...  * * * */
    sn_coap_parser_header_parse(&data_temp_ptr, parsed_and_returned_coap_msg_ptr, coap_version_ptr);

    /* * * * Options parsing, move pointer over the options... * * * */
    if (sn_coap_parser_options_parse(handle, &data_temp_ptr, parsed_and_returned_coap_msg_ptr, packet_data_ptr, packet_data_len) != 0) {
        parsed_and_returned_coap_msg_ptr->coap_status = COAP_STATUS_PARSER_ERROR_IN_HEADER;
        return parsed_and_returned_coap_msg_ptr;
    }

    /* * * * Payload parsing * * * */
    if (sn_coap_parser_payload_parse(packet_data_len, packet_data_ptr, &data_temp_ptr, parsed_and_returned_coap_msg_ptr) == -1) {
        parsed_and_returned_coap_msg_ptr->coap_status = COAP_STATUS_PARSER_ERROR_IN_HEADER;
        return parsed_and_returned_coap_msg_ptr;
    }

    /* * * * Return parsed CoAP message  * * * * */
    return parsed_and_returned_coap_msg_ptr;
}

void sn_coap_parser_release_allocated_coap_msg_mem(struct coap_s *handle, sn_coap_hdr_s *freed_coap_msg_ptr)
{
    if (handle == NULL) {
        return;
    }

    if (freed_coap_msg_ptr != NULL) {
        if (freed_coap_msg_ptr->uri_path_ptr != NULL) {
            handle->sn_coap_protocol_free(freed_coap_msg_ptr->uri_path_ptr);
        }

        if (freed_coap_msg_ptr->token_ptr != NULL) {
            handle->sn_coap_protocol_free(freed_coap_msg_ptr->token_ptr);
        }

        if (freed_coap_msg_ptr->options_list_ptr != NULL) {
            if (freed_coap_msg_ptr->options_list_ptr->proxy_uri_ptr != NULL) {
                handle->sn_coap_protocol_free(freed_coap_msg_ptr->options_list_ptr->proxy_uri_ptr);
            }

            if (freed_coap_msg_ptr->options_list_ptr->etag_ptr != NULL) {
                handle->sn_coap_protocol_free(freed_coap_msg_ptr->options_list_ptr->etag_ptr);
            }

            if (freed_coap_msg_ptr->options_list_ptr->uri_host_ptr != NULL) {
                handle->sn_coap_protocol_free(freed_coap_msg_ptr->options_list_ptr->uri_host_ptr);
            }

            if (freed_coap_msg_ptr->options_list_ptr->location_path_ptr != NULL) {
                handle->sn_coap_protocol_free(freed_coap_msg_ptr->options_list_ptr->location_path_ptr);
            }

            if (freed_coap_msg_ptr->options_list_ptr->location_query_ptr != NULL) {
                handle->sn_coap_protocol_free(freed_coap_msg_ptr->options_list_ptr->location_query_ptr);
            }

            if (freed_coap_msg_ptr->options_list_ptr->uri_query_ptr != NULL) {
                handle->sn_coap_protocol_free(freed_coap_msg_ptr->options_list_ptr->uri_query_ptr);
            }

            handle->sn_coap_protocol_free(freed_coap_msg_ptr->options_list_ptr);
        }

        handle->sn_coap_protocol_free(freed_coap_msg_ptr);
    }
}

/**
 * \fn static void sn_coap_parser_header_parse(uint8_t **packet_data_pptr, sn_coap_hdr_s *dst_coap_msg_ptr, coap_version_e *coap_version_ptr)
 *
 * \brief Parses CoAP message's Header part from given Packet data
 *
 * \param **packet_data_ptr is source for Packet data to be parsed to CoAP message
 *
 * \param *dst_coap_msg_ptr is destination for parsed CoAP message
 *
 * \param *coap_version_ptr is destination for parsed CoAP specification version
 */
static void sn_coap_parser_header_parse(uint8_t **packet_data_pptr, sn_coap_hdr_s *dst_coap_msg_ptr, coap_version_e *coap_version_ptr)
{
    /* Parse CoAP Version and message type*/
    *coap_version_ptr = (coap_version_e)(**packet_data_pptr & COAP_HEADER_VERSION_MASK);
    dst_coap_msg_ptr->msg_type = (sn_coap_msg_type_e)(**packet_data_pptr & COAP_HEADER_MSG_TYPE_MASK);
    (*packet_data_pptr) += 1;

    /* Parse Message code */
    dst_coap_msg_ptr->msg_code = (sn_coap_msg_code_e) **packet_data_pptr;
    (*packet_data_pptr) += 1;

    /* Parse Message ID */
    dst_coap_msg_ptr->msg_id = *(*packet_data_pptr + 1);
    dst_coap_msg_ptr->msg_id += **packet_data_pptr << COAP_HEADER_MSG_ID_MSB_SHIFT;
    (*packet_data_pptr) += 2;

}

/**
 * \brief Parses a variable-length uint value from an option
 *
 * \param **packet_data_pptr is source of option data to be parsed
 * \param option_len is length of option data (will be 0-4)
 *
 * \return Return value is value of uint
 */
static uint32_t sn_coap_parser_options_parse_uint(uint8_t **packet_data_pptr, uint8_t option_len)
{
    uint32_t value = 0;
    while (option_len--) {
        value <<= 8;
        value |= *(*packet_data_pptr)++;
    }
    return value;
}

/**
 * \fn static uint8_t sn_coap_parser_options_parse(uint8_t **packet_data_pptr, sn_coap_hdr_s *dst_coap_msg_ptr)
 *
 * \brief Parses CoAP message's Options part from given Packet data
 *
 * \param **packet_data_pptr is source of Packet data to be parsed to CoAP message
 * \param *dst_coap_msg_ptr is destination for parsed CoAP message
 *
 * \return Return value is 0 in ok case and -1 in failure case
 */
static int8_t sn_coap_parser_options_parse(struct coap_s *handle, uint8_t **packet_data_pptr, sn_coap_hdr_s *dst_coap_msg_ptr, uint8_t *packet_data_start_ptr, uint16_t packet_len)
{
    uint8_t previous_option_number = 0;
    uint8_t i                      = 0;
    int8_t  ret_status             = 0;
    uint16_t message_left          = 0;

    /*  Parse token, if exists  */
    dst_coap_msg_ptr->token_len = *packet_data_start_ptr & COAP_HEADER_TOKEN_LENGTH_MASK;

    if (dst_coap_msg_ptr->token_len) {
        if ((dst_coap_msg_ptr->token_len > 8) || dst_coap_msg_ptr->token_ptr) {
            tr_error("sn_coap_parser_options_parse - token not valid!");
            return -1;
        }

        dst_coap_msg_ptr->token_ptr = handle->sn_coap_protocol_malloc(dst_coap_msg_ptr->token_len);

        if (dst_coap_msg_ptr->token_ptr == NULL) {
            tr_error("sn_coap_parser_options_parse - failed to allocate token!");
            return -1;
        }

        memcpy(dst_coap_msg_ptr->token_ptr, *packet_data_pptr, dst_coap_msg_ptr->token_len);
        (*packet_data_pptr) += dst_coap_msg_ptr->token_len;
    }

    message_left = packet_len - ((*packet_data_pptr) - packet_data_start_ptr);

    /* Loop all Options */
    while (message_left && (**packet_data_pptr != 0xff)) {

        /* Get option length WITHOUT extensions */
        uint16_t option_len = (**packet_data_pptr & 0x0F);

        /* Resolve option delta */
        uint16_t  option_number = (**packet_data_pptr >> COAP_OPTIONS_OPTION_NUMBER_SHIFT);

        if (option_number == 13) {
            option_number = *(*packet_data_pptr + 1) + 13;
            (*packet_data_pptr)++;
        } else if (option_number == 14) {
            option_number = *(*packet_data_pptr + 2);
            option_number += (*(*packet_data_pptr + 1) << 8) + 269;
            (*packet_data_pptr) += 2;
        }
        /* Option number 15 reserved for payload marker. This is handled as a error! */
        else if (option_number == 15) {
            tr_error("sn_coap_parser_options_parse - invalid option number(15)!");
            return -1;
        }

        /* Add previous option to option delta and get option number */
        option_number += previous_option_number;

        /* Add possible option length extension to resolve full length of the option */
        if (option_len == 13) {
            option_len = *(*packet_data_pptr + 1) + 13;
            (*packet_data_pptr)++;
        } else if (option_len == 14) {
            option_len = *(*packet_data_pptr + 2);
            option_len += (*(*packet_data_pptr + 1) << 8) + 269;
            (*packet_data_pptr) += 2;
        }
        /* Option number length 15 is reserved for the future use - ERROR */
        else if (option_len == 15) {
            tr_error("sn_coap_parser_options_parse - invalid option len(15)!");
            return -1;
        }

        message_left = packet_len - (*packet_data_pptr - packet_data_start_ptr);

        /* * * Parse option itself * * */
        /* Some options are handled independently in own functions */
        previous_option_number = option_number;
        /* Allocate options_list_ptr if needed */
        switch (option_number) {
            case COAP_OPTION_MAX_AGE:
            case COAP_OPTION_PROXY_URI:
            case COAP_OPTION_ETAG:
            case COAP_OPTION_URI_HOST:
            case COAP_OPTION_LOCATION_PATH:
            case COAP_OPTION_URI_PORT:
            case COAP_OPTION_LOCATION_QUERY:
            case COAP_OPTION_OBSERVE:
            case COAP_OPTION_URI_QUERY:
            case COAP_OPTION_BLOCK2:
            case COAP_OPTION_BLOCK1:
            case COAP_OPTION_ACCEPT:
            case COAP_OPTION_SIZE1:
            case COAP_OPTION_SIZE2:
                if (sn_coap_parser_alloc_options(handle, dst_coap_msg_ptr) == NULL) {
                    tr_error("sn_coap_parser_options_parse - failed to allocate options!");
                    return -1;
                }
                break;
        }

        /* Parse option */
        switch (option_number) {
            case COAP_OPTION_CONTENT_FORMAT:
                if ((option_len > 2) || (dst_coap_msg_ptr->content_format != COAP_CT_NONE)) {
                    tr_error("sn_coap_parser_options_parse - COAP_OPTION_CONTENT_FORMAT not valid!");
                    return -1;
                }
                (*packet_data_pptr)++;
                dst_coap_msg_ptr->content_format = (sn_coap_content_format_e) sn_coap_parser_options_parse_uint(packet_data_pptr, option_len);
                break;

            case COAP_OPTION_MAX_AGE:
                if (option_len > 4) {
                    tr_error("sn_coap_parser_options_parse - COAP_OPTION_MAX_AGE not valid!");
                    return -1;
                }
                (*packet_data_pptr)++;
                dst_coap_msg_ptr->options_list_ptr->max_age = sn_coap_parser_options_parse_uint(packet_data_pptr, option_len);
                break;

            case COAP_OPTION_PROXY_URI:
                if ((option_len > 1034) || (option_len < 1) || dst_coap_msg_ptr->options_list_ptr->proxy_uri_ptr) {
                    tr_error("sn_coap_parser_options_parse - COAP_OPTION_PROXY_URI not valid!");
                    return -1;
                }
                dst_coap_msg_ptr->options_list_ptr->proxy_uri_len = option_len;
                (*packet_data_pptr)++;

                dst_coap_msg_ptr->options_list_ptr->proxy_uri_ptr = handle->sn_coap_protocol_malloc(option_len);

                if (dst_coap_msg_ptr->options_list_ptr->proxy_uri_ptr == NULL) {
                    tr_error("sn_coap_parser_options_parse - COAP_OPTION_PROXY_URI allocation failed!");
                    return -1;
                }

                memcpy(dst_coap_msg_ptr->options_list_ptr->proxy_uri_ptr, *packet_data_pptr, option_len);
                (*packet_data_pptr) += option_len;

                break;

            case COAP_OPTION_ETAG:
                /* This is managed independently because User gives this option in one character table */

                ret_status = sn_coap_parser_options_parse_multiple_options(handle, packet_data_pptr,
                             message_left,
                             &dst_coap_msg_ptr->options_list_ptr->etag_ptr,
                             (uint16_t *)&dst_coap_msg_ptr->options_list_ptr->etag_len,
                             COAP_OPTION_ETAG, option_len);
                if (ret_status >= 0) {
                    i += (ret_status - 1); /* i += is because possible several Options are handled by sn_coap_parser_options_parse_multiple_options() */
                } else {
                    tr_error("sn_coap_parser_options_parse - COAP_OPTION_ETAG not valid!");
                    return -1;
                }
                break;

            case COAP_OPTION_URI_HOST:
                if ((option_len > 255) || (option_len < 1) || dst_coap_msg_ptr->options_list_ptr->uri_host_ptr) {
                    tr_error("sn_coap_parser_options_parse - COAP_OPTION_URI_HOST not valid!");
                    return -1;
                }
                dst_coap_msg_ptr->options_list_ptr->uri_host_len = option_len;
                (*packet_data_pptr)++;

                dst_coap_msg_ptr->options_list_ptr->uri_host_ptr = handle->sn_coap_protocol_malloc(option_len);

                if (dst_coap_msg_ptr->options_list_ptr->uri_host_ptr == NULL) {
                    tr_error("sn_coap_parser_options_parse - COAP_OPTION_URI_HOST allocation failed!");
                    return -1;
                }
                memcpy(dst_coap_msg_ptr->options_list_ptr->uri_host_ptr, *packet_data_pptr, option_len);
                (*packet_data_pptr) += option_len;

                break;

            case COAP_OPTION_LOCATION_PATH:
                if (dst_coap_msg_ptr->options_list_ptr->location_path_ptr) {
                    tr_error("sn_coap_parser_options_parse - COAP_OPTION_LOCATION_PATH exists!");
                    return -1;
                }
                /* This is managed independently because User gives this option in one character table */
                ret_status = sn_coap_parser_options_parse_multiple_options(handle, packet_data_pptr, message_left,
                             &dst_coap_msg_ptr->options_list_ptr->location_path_ptr, &dst_coap_msg_ptr->options_list_ptr->location_path_len,
                             COAP_OPTION_LOCATION_PATH, option_len);
                if (ret_status >= 0) {
                    i += (ret_status - 1); /* i += is because possible several Options are handled by sn_coap_parser_options_parse_multiple_options() */
                } else {
                    tr_error("sn_coap_parser_options_parse - COAP_OPTION_LOCATION_PATH not valid!");
                    return -1;
                }

                break;


            case COAP_OPTION_URI_PORT:
                if ((option_len > 2) || dst_coap_msg_ptr->options_list_ptr->uri_port != COAP_OPTION_URI_PORT_NONE) {
                    tr_error("sn_coap_parser_options_parse - COAP_OPTION_URI_PORT not valid!");
                    return -1;
                }
                (*packet_data_pptr)++;

                dst_coap_msg_ptr->options_list_ptr->uri_port = sn_coap_parser_options_parse_uint(packet_data_pptr, option_len);
                break;

            case COAP_OPTION_LOCATION_QUERY:
                ret_status = sn_coap_parser_options_parse_multiple_options(handle, packet_data_pptr, message_left,
                             &dst_coap_msg_ptr->options_list_ptr->location_query_ptr, &dst_coap_msg_ptr->options_list_ptr->location_query_len,
                             COAP_OPTION_LOCATION_QUERY, option_len);
                if (ret_status >= 0) {
                    i += (ret_status - 1); /* i += is because possible several Options are handled by sn_coap_parser_options_parse_multiple_options() */
                } else {
                    tr_error("sn_coap_parser_options_parse - COAP_OPTION_LOCATION_QUERY not valid!");
                    return -1;
                }

                break;

            case COAP_OPTION_URI_PATH:
                ret_status = sn_coap_parser_options_parse_multiple_options(handle, packet_data_pptr, message_left,
                             &dst_coap_msg_ptr->uri_path_ptr, &dst_coap_msg_ptr->uri_path_len,
                             COAP_OPTION_URI_PATH, option_len);
                if (ret_status >= 0) {
                    i += (ret_status - 1); /* i += is because possible several Options are handled by sn_coap_parser_options_parse_multiple_options() */
                } else {
                    tr_error("sn_coap_parser_options_parse - COAP_OPTION_URI_PATH not valid!");
                    return -1;
                }

                break;

            case COAP_OPTION_OBSERVE:
                if ((option_len > 2) || dst_coap_msg_ptr->options_list_ptr->observe != COAP_OBSERVE_NONE) {
                    tr_error("sn_coap_parser_options_parse - COAP_OPTION_OBSERVE not valid!");
                    return -1;
                }

                (*packet_data_pptr)++;

                dst_coap_msg_ptr->options_list_ptr->observe = sn_coap_parser_options_parse_uint(packet_data_pptr, option_len);

                break;

            case COAP_OPTION_URI_QUERY:
                ret_status = sn_coap_parser_options_parse_multiple_options(handle, packet_data_pptr, message_left,
                             &dst_coap_msg_ptr->options_list_ptr->uri_query_ptr, &dst_coap_msg_ptr->options_list_ptr->uri_query_len,
                             COAP_OPTION_URI_QUERY, option_len);
                if (ret_status >= 0) {
                    i += (ret_status - 1); /* i += is because possible several Options are handled by sn_coap_parser_options_parse_multiple_options() */
                } else {
                    tr_error("sn_coap_parser_options_parse - COAP_OPTION_URI_QUERY not valid!");
                    return -1;
                }

                break;

            case COAP_OPTION_BLOCK2:
                if ((option_len > 3) || dst_coap_msg_ptr->options_list_ptr->block2 != COAP_OPTION_BLOCK_NONE) {
                    tr_error("sn_coap_parser_options_parse - COAP_OPTION_BLOCK2 not valid!");
                    return -1;
                }
                (*packet_data_pptr)++;

                dst_coap_msg_ptr->options_list_ptr->block2 = sn_coap_parser_options_parse_uint(packet_data_pptr, option_len);

                break;

            case COAP_OPTION_BLOCK1:
                if ((option_len > 3) || dst_coap_msg_ptr->options_list_ptr->block1 != COAP_OPTION_BLOCK_NONE) {
                    tr_error("sn_coap_parser_options_parse - COAP_OPTION_BLOCK1 not valid!");
                    return -1;
                }
                (*packet_data_pptr)++;

                dst_coap_msg_ptr->options_list_ptr->block1 = sn_coap_parser_options_parse_uint(packet_data_pptr, option_len);

                break;

            case COAP_OPTION_ACCEPT:
                if ((option_len > 2) || (dst_coap_msg_ptr->options_list_ptr->accept != COAP_CT_NONE)) {
                    tr_error("sn_coap_parser_options_parse - COAP_OPTION_ACCEPT not valid!");
                    return -1;
                }

                (*packet_data_pptr)++;

                dst_coap_msg_ptr->options_list_ptr->accept = (sn_coap_content_format_e) sn_coap_parser_options_parse_uint(packet_data_pptr, option_len);
                break;

            case COAP_OPTION_SIZE1:
                if ((option_len > 4) || dst_coap_msg_ptr->options_list_ptr->use_size1) {
                    tr_error("sn_coap_parser_options_parse - COAP_OPTION_SIZE1 not valid!");
                    return -1;
                }
                dst_coap_msg_ptr->options_list_ptr->use_size1 = true;
                (*packet_data_pptr)++;
                dst_coap_msg_ptr->options_list_ptr->size1 = sn_coap_parser_options_parse_uint(packet_data_pptr, option_len);
                break;

            case COAP_OPTION_SIZE2:
                if ((option_len > 4) || dst_coap_msg_ptr->options_list_ptr->use_size2) {
                    tr_error("sn_coap_parser_options_parse - COAP_OPTION_SIZE2 not valid!");
                    return -1;
                }
                dst_coap_msg_ptr->options_list_ptr->use_size2 = true;
                (*packet_data_pptr)++;
                dst_coap_msg_ptr->options_list_ptr->size2 = sn_coap_parser_options_parse_uint(packet_data_pptr, option_len);
                break;

            default:
                tr_error("sn_coap_parser_options_parse - unknown option!");
                return -1;
        }

        /* Check for overflow */
        if ((*packet_data_pptr - packet_data_start_ptr) > packet_len) {
            return -1;
        }

        message_left = packet_len - (*packet_data_pptr - packet_data_start_ptr);

    }

    return 0;
}


/**
 * \fn static int8_t sn_coap_parser_options_parse_multiple_options(uint8_t **packet_data_pptr, uint8_t options_count_left, uint8_t *previous_option_number_ptr, uint8_t **dst_pptr,
 *                                                                  uint16_t *dst_len_ptr, sn_coap_option_numbers_e option, uint16_t option_number_len)
 *
 * \brief Parses CoAP message's Uri-query options
 *
 * \param **packet_data_pptr is source for Packet data to be parsed to CoAP message
 *
 * \param *dst_coap_msg_ptr is destination for parsed CoAP message
 *
 * \param options_count_left tells how many options are unhandled in Packet data
 *
 * \param *previous_option_number_ptr is pointer to used and returned previous Option number
 *
 * \return Return value is count of Uri-query optios parsed. In failure case -1 is returned.
*/
static int8_t sn_coap_parser_options_parse_multiple_options(struct coap_s *handle, uint8_t **packet_data_pptr, uint16_t packet_left_len,  uint8_t **dst_pptr, uint16_t *dst_len_ptr, sn_coap_option_numbers_e option, uint16_t option_number_len)
{
    int16_t     uri_query_needed_heap       = sn_coap_parser_options_count_needed_memory_multiple_option(*packet_data_pptr, packet_left_len, option, option_number_len);
    uint8_t    *temp_parsed_uri_query_ptr   = NULL;
    uint8_t     returned_option_counter     = 0;

    if (uri_query_needed_heap == -1) {
        return -1;
    }

    if (uri_query_needed_heap) {
        *dst_pptr = (uint8_t *) handle->sn_coap_protocol_malloc(uri_query_needed_heap);

        if (*dst_pptr == NULL) {
            tr_error("sn_coap_parser_options_parse_multiple_options - failed to allocate options!");
            return -1;
        }
    }

    *dst_len_ptr = uri_query_needed_heap;

    temp_parsed_uri_query_ptr = *dst_pptr;

    /* Loop all Uri-Query options */
    while ((temp_parsed_uri_query_ptr - *dst_pptr) < uri_query_needed_heap) {
        /* Check if this is first Uri-Query option */
        if (returned_option_counter > 0) {
            /* Uri-Query is modified to following format: temp1'\0'temp2'\0'temp3 i.e.  */
            /* Uri-Path is modified to following format: temp1\temp2\temp3 i.e.  */
            if (option == COAP_OPTION_URI_QUERY || option == COAP_OPTION_LOCATION_QUERY || option == COAP_OPTION_ETAG || option == COAP_OPTION_ACCEPT) {
                memset(temp_parsed_uri_query_ptr, '&', 1);
            } else if (option == COAP_OPTION_URI_PATH || option == COAP_OPTION_LOCATION_PATH) {
                memset(temp_parsed_uri_query_ptr, '/', 1);
            }

            temp_parsed_uri_query_ptr++;
        }

        returned_option_counter++;

        (*packet_data_pptr)++;

        if (((temp_parsed_uri_query_ptr - *dst_pptr) + option_number_len) > uri_query_needed_heap) {
            return -1;
        }

        memcpy(temp_parsed_uri_query_ptr, *packet_data_pptr, option_number_len);

        (*packet_data_pptr) += option_number_len;
        temp_parsed_uri_query_ptr += option_number_len;

        if ((temp_parsed_uri_query_ptr - *dst_pptr) >= uri_query_needed_heap || ((**packet_data_pptr >> COAP_OPTIONS_OPTION_NUMBER_SHIFT) != 0)) {
            return returned_option_counter;
        }

        option_number_len = (**packet_data_pptr & 0x0F);
        if (option_number_len == 13) {
            option_number_len = *(*packet_data_pptr + 1) + 13;
            (*packet_data_pptr)++;
        } else if (option_number_len == 14) {
            option_number_len = *(*packet_data_pptr + 2);
            option_number_len += (*(*packet_data_pptr + 1) << 8) + 269;
            (*packet_data_pptr) += 2;
        }
    }

    return returned_option_counter;
}




/**
 * \fn static uint16_t sn_coap_parser_options_count_needed_memory_multiple_option(uint8_t *packet_data_ptr, uint8_t options_count_left, uint8_t previous_option_number, sn_coap_option_numbers_e option, uint16_t option_number_len)
 *
 * \brief Counts needed memory for uri query option
 *
 * \param *packet_data_ptr is start of source for Packet data to be parsed to CoAP message
 *
 * \param options_count_left tells how many options are unhandled in Packet data
 *
 * \param previous_option_number is previous Option number
 *
 * \param sn_coap_option_numbers_e option option number to be calculated
 *
 * \param uint16_t option_number_len length of the first option part
 */
static int16_t sn_coap_parser_options_count_needed_memory_multiple_option(uint8_t *packet_data_ptr, uint16_t packet_left_len, sn_coap_option_numbers_e option, uint16_t option_number_len)
{
    uint16_t ret_value              = 0;
    uint16_t i                      = 1;

    /* Loop all Uri-Query options */
    while (i <= packet_left_len) {
        if (option == COAP_OPTION_LOCATION_PATH && option_number_len > 255) {
            return -1;
        }
        if (option == COAP_OPTION_URI_PATH && option_number_len > 255) {
            return -1;
        }
        if (option == COAP_OPTION_URI_QUERY && option_number_len > 255) {
            return -1;
        }
        if (option == COAP_OPTION_LOCATION_QUERY && option_number_len > 255) {
            return -1;
        }
        if (option == COAP_OPTION_ACCEPT && option_number_len > 2) {
            return -1;
        }
        if (option == COAP_OPTION_ETAG && option_number_len > 8) {
            return -1;
        }

        i += option_number_len;
        ret_value += option_number_len + 1; /* + 1 is for separator */

        if( i == packet_left_len ) {
            break;
        }
        else if( i > packet_left_len ) {
            return -1;
        }

        if ((*(packet_data_ptr + i) >> COAP_OPTIONS_OPTION_NUMBER_SHIFT) != 0) {
            return (ret_value - 1);    /* -1 because last Part path does not include separator */
        }

        option_number_len = (*(packet_data_ptr + i) & 0x0F);

        if (option_number_len == 13) {

            if(i + 1 >= packet_left_len) {
                return -1;
            }

            i++;
            option_number_len = *(packet_data_ptr + i) + 13;
        } else if (option_number_len == 14) {

            if(i + 2 >= packet_left_len) {
                return -1;
            }

            option_number_len = *(packet_data_ptr + i + 2);
            option_number_len += (*(packet_data_ptr + i + 1) << 8) + 269;
            i += 2;
        } else if (option_number_len == 15) {
            return -1;
        }
        i++;

    }

    if (ret_value != 0) {
        return (ret_value - 1);    /* -1 because last Part path does not include separator */
    } else {
        return 0;
    }
}

/**
 * \fn static void sn_coap_parser_payload_parse(uint16_t packet_data_len, uint8_t *packet_data_ptr, uint8_t **packet_data_pptr, sn_coap_hdr_s *dst_coap_msg_ptr)
 *
 * \brief Parses CoAP message's Payload part from given Packet data
 *
 * \param packet_data_len is length of given Packet data to be parsed to CoAP message
 *
 * \param *packet_data_ptr is start of source for Packet data to be parsed to CoAP message
 *
 * \param **packet_data_pptr is source for Packet data to be parsed to CoAP message
 *
 * \param *dst_coap_msg_ptr is destination for parsed CoAP message
 *****************************************************************************/
static int8_t sn_coap_parser_payload_parse(uint16_t packet_data_len, uint8_t *packet_data_start_ptr, uint8_t **packet_data_pptr, sn_coap_hdr_s *dst_coap_msg_ptr)
{
    /* If there is payload */
    if ((*packet_data_pptr - packet_data_start_ptr) < packet_data_len) {
        if (**packet_data_pptr == 0xff) {
            (*packet_data_pptr)++;
            /* Parse Payload length */
            dst_coap_msg_ptr->payload_len = packet_data_len - (*packet_data_pptr - packet_data_start_ptr);

            /* The presence of a marker followed by a zero-length payload MUST be processed as a message format error */
            if (dst_coap_msg_ptr->payload_len == 0) {
                return -1;
            }

            /* Parse Payload by setting CoAP message's payload_ptr to point Payload in Packet data */
            dst_coap_msg_ptr->payload_ptr = *packet_data_pptr;
        }
        /* No payload marker.. */
        else {
            tr_error("sn_coap_parser_payload_parse - payload marker not found!");
            return -1;
        }
    }
    return 0;
}

