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
 * \file sn_coap_builder.c
 *
 * \brief CoAP Message builder
 *
 * Functionality: Builds CoAP message
 *
 */

/* * * * * * * * * * * * * * */
/* * * * INCLUDE FILES * * * */
/* * * * * * * * * * * * * * */

#include <string.h> /* For memset() and memcpy() */

#include "ns_types.h"
#include "mbed-coap/sn_coap_header.h"
#include "sn_coap_header_internal.h"
#include "sn_coap_protocol_internal.h"
#include "mbed-trace/mbed_trace.h"

#define TRACE_GROUP "coap"
/* * * * LOCAL FUNCTION PROTOTYPES * * * */
static uint8_t *sn_coap_builder_header_build(uint8_t *dst_packet_data_pptr, const sn_coap_hdr_s *src_coap_msg_ptr);
static uint8_t *sn_coap_builder_options_build(uint8_t *dst_packet_data_ptr, const sn_coap_hdr_s *src_coap_msg_ptr);
static uint_fast16_t sn_coap_builder_options_calc_option_size(uint16_t query_len, const uint8_t *query_ptr, sn_coap_option_numbers_e option);
static uint8_t *sn_coap_builder_options_build_add_one_option(uint8_t *dst_packet_data_ptr, uint_fast16_t option_len, const uint8_t *option_ptr, sn_coap_option_numbers_e option_number, uint16_t *previous_option_number);
static uint8_t *sn_coap_builder_options_build_add_multiple_option(uint8_t *dst_packet_data_pptr, const uint8_t *src_pptr, uint_fast16_t src_len, sn_coap_option_numbers_e option, uint16_t *previous_option_number);
static uint_fast8_t sn_coap_builder_options_calc_uint_option_size(uint32_t option_value);
static uint8_t *sn_coap_builder_options_build_add_uint_option(uint8_t *dst_packet_data_ptr, uint32_t value, sn_coap_option_numbers_e option_number, uint16_t *previous_option_number);
static uint_fast8_t  sn_coap_builder_options_get_option_part_count(uint_fast16_t query_len, const uint8_t *query_ptr, sn_coap_option_numbers_e option);
static uint_fast16_t sn_coap_builder_options_get_option_part_length_from_whole_option_string(uint_fast16_t query_len, const uint8_t *query_ptr, uint_fast8_t query_index, sn_coap_option_numbers_e option);
static int_fast16_t sn_coap_builder_options_get_option_part_position(uint_fast16_t query_len, const uint8_t *query_ptr, uint_fast8_t query_index, sn_coap_option_numbers_e option);
static uint8_t *sn_coap_builder_payload_build(uint8_t *dst_packet_data_ptr, const sn_coap_hdr_s *src_coap_msg_ptr);
static uint_fast8_t sn_coap_builder_options_calculate_jump_need(const sn_coap_hdr_s *src_coap_msg_ptr);

sn_coap_hdr_s *sn_coap_build_response(struct coap_s *handle, const sn_coap_hdr_s *coap_packet_ptr, uint8_t msg_code)
{
    sn_coap_hdr_s *coap_res_ptr;

    if (!coap_packet_ptr || !handle) {
        return NULL;
    }

    coap_res_ptr = sn_coap_parser_alloc_message(handle);
    if (!coap_res_ptr) {
        tr_error("sn_coap_build_response - failed to allocate message!");
        return NULL;
    }

    if (msg_code == COAP_MSG_CODE_REQUEST_GET) {
        // Blockwise message response is new GET
        coap_res_ptr->msg_type = COAP_MSG_TYPE_CONFIRMABLE;
        coap_res_ptr->msg_code = (sn_coap_msg_code_e)msg_code;
        /* msg_id needs to be set by the caller in this case */
    }
    else if (coap_packet_ptr->msg_type == COAP_MSG_TYPE_CONFIRMABLE) {
        coap_res_ptr->msg_type = COAP_MSG_TYPE_ACKNOWLEDGEMENT;
        coap_res_ptr->msg_code = (sn_coap_msg_code_e)msg_code;
        coap_res_ptr->msg_id = coap_packet_ptr->msg_id;
    }
    else if (coap_packet_ptr->msg_type == COAP_MSG_TYPE_NON_CONFIRMABLE) {
        coap_res_ptr->msg_type = COAP_MSG_TYPE_NON_CONFIRMABLE;
        coap_res_ptr->msg_code = (sn_coap_msg_code_e)msg_code;
        /* msg_id needs to be set by the caller in this case */
    }
    else {
        handle->sn_coap_protocol_free( coap_res_ptr );
        return NULL;
    }

    if (coap_packet_ptr->token_ptr) {
        coap_res_ptr->token_len = coap_packet_ptr->token_len;
        coap_res_ptr->token_ptr = sn_coap_protocol_malloc_copy(handle, coap_packet_ptr->token_ptr, coap_res_ptr->token_len);
        if (!coap_res_ptr->token_ptr) {
            tr_error("sn_coap_build_response - failed to allocate token!");
            handle->sn_coap_protocol_free(coap_res_ptr);
            return NULL;
        }
    }
    return coap_res_ptr;
}

int16_t sn_coap_builder(uint8_t *dst_packet_data_ptr, const sn_coap_hdr_s *src_coap_msg_ptr)
{
    return sn_coap_builder_2(dst_packet_data_ptr, src_coap_msg_ptr, SN_COAP_MAX_BLOCKWISE_PAYLOAD_SIZE);
}

int16_t sn_coap_builder_2(uint8_t * restrict dst_packet_data_ptr, const sn_coap_hdr_s * restrict src_coap_msg_ptr, uint16_t blockwise_payload_size)
{
    uint8_t *base_packet_data_ptr;

    /* * * * Check given pointers  * * * */
    if (dst_packet_data_ptr == NULL || src_coap_msg_ptr == NULL) {
        return -2;
    }

    /* This serves as a pre-validity check for various src_coap_msg_ptr fields */
    /* (as long as SN_COAP_CONSTANT_NEEDED_SIZE is not set) */
    uint16_t dst_byte_count_to_be_built = sn_coap_builder_calc_needed_packet_data_size_2(src_coap_msg_ptr, blockwise_payload_size);
    if (!dst_byte_count_to_be_built) {
        tr_error("sn_coap_builder_2 - failed to allocate message!");
        return -1;
    }

    /* * * * Store base (= original) destination Packet data pointer for later usage * * * */
    base_packet_data_ptr = dst_packet_data_ptr;

    /* * * * * * * * * * * * * * * * * * */
    /* * * * Header part building  * * * */
    /* * * * * * * * * * * * * * * * * * */
    dst_packet_data_ptr = sn_coap_builder_header_build(dst_packet_data_ptr, src_coap_msg_ptr);
    if (!dst_packet_data_ptr) {
        /* Header building failed */
        tr_error("sn_coap_builder_2 - header building failed!");
        return -1;
    }

    /* If else than Reset message because Reset message must be empty */
    if (src_coap_msg_ptr->msg_type != COAP_MSG_TYPE_RESET) {
        /* * * * * * * * * * * * * * * * * * */
        /* * * * Options part building * * * */
        /* * * * * * * * * * * * * * * * * * */
        dst_packet_data_ptr = sn_coap_builder_options_build(dst_packet_data_ptr, src_coap_msg_ptr);

        /* * * * * * * * * * * * * * * * * * */
        /* * * * Payload part building * * * */
        /* * * * * * * * * * * * * * * * * * */
        dst_packet_data_ptr = sn_coap_builder_payload_build(dst_packet_data_ptr, src_coap_msg_ptr);
    }

    /* Shout as much as we can about overflow - if we exceed this, may have overrun user's buffer */
    if (dst_packet_data_ptr - base_packet_data_ptr > dst_byte_count_to_be_built) {
        tr_error("sn_coap_builder_2 - overflowed expected size!");
        return -1;
    }

    /* * * * Return built Packet data length * * * */
    return (dst_packet_data_ptr - base_packet_data_ptr);
}

uint16_t (sn_coap_builder_calc_needed_packet_data_size)(const sn_coap_hdr_s *src_coap_msg_ptr)
{
    return sn_coap_builder_calc_needed_packet_data_size_2(src_coap_msg_ptr, SN_COAP_MAX_BLOCKWISE_PAYLOAD_SIZE);
}

uint16_t (sn_coap_builder_calc_needed_packet_data_size_2)(const sn_coap_hdr_s *src_coap_msg_ptr, uint16_t blockwise_payload_size)
{
#ifdef SN_COAP_CONSTANT_NEEDED_SIZE
    return SN_COAP_CONSTANT_NEEDED_SIZE;
#else
    (void)blockwise_payload_size;
    uint_fast32_t returned_byte_count = 0;

    if (!src_coap_msg_ptr) {
        return 0;
    }
    /* * * * *  HEADER * * * * */

    /* Header size is fixed */
    returned_byte_count = COAP_HEADER_LENGTH;

    /* * * * * OPTIONS * * * * */

    /* If else than Reset message because Reset message must be empty */
    if (src_coap_msg_ptr->msg_type != COAP_MSG_TYPE_RESET) {
        uint_fast16_t repeatable_option_size = 0;
        /* TOKEN - Length is 1-8 bytes */
        if (src_coap_msg_ptr->token_ptr != NULL) {
            if (src_coap_msg_ptr->token_len > 8 || src_coap_msg_ptr->token_len < 1) { /* Check that option is not longer than defined */
                tr_error("sn_coap_builder_calc_needed_packet_data_size_2 - token too large!");
                return 0;
            }
            returned_byte_count += src_coap_msg_ptr->token_len;
        }
        /* URI PATH - Repeatable option. Length of one option is 0-255 */
        if (src_coap_msg_ptr->uri_path_ptr != NULL) {
            repeatable_option_size = sn_coap_builder_options_calc_option_size(src_coap_msg_ptr->uri_path_len,
                                     src_coap_msg_ptr->uri_path_ptr, COAP_OPTION_URI_PATH);
            if (repeatable_option_size) {
                returned_byte_count += repeatable_option_size;
            } else {
                tr_error("sn_coap_builder_calc_needed_packet_data_size_2 - uri path size failed!");
                return 0;
            }
        }

        /* CONTENT FORMAT - An integer option, up to 2 bytes */
        if (src_coap_msg_ptr->content_format != COAP_CT_NONE) {
            if ((uint32_t) src_coap_msg_ptr->content_format > 0xffff) {
                tr_error("sn_coap_builder_calc_needed_packet_data_size_2 - content format too large!");
                return 0;
            }
            returned_byte_count += sn_coap_builder_options_calc_uint_option_size(src_coap_msg_ptr->content_format);
        }
        /* If options list pointer exists */
        if (src_coap_msg_ptr->options_list_ptr != NULL) {

            const sn_coap_options_list_s * restrict src_options_list_ptr = src_coap_msg_ptr->options_list_ptr;

            /* ACCEPT - An integer option, up to 2 bytes */
            if (src_options_list_ptr->accept != COAP_CT_NONE) {
                if ((uint32_t) src_options_list_ptr->accept > 0xffff) {
                    tr_error("sn_coap_builder_calc_needed_packet_data_size_2 - accept too large!");
                    return 0;
                }
                returned_byte_count += sn_coap_builder_options_calc_uint_option_size(src_options_list_ptr->accept);
            }
            /* MAX AGE - An integer option, omitted for default. Up to 4 bytes */
            if (src_options_list_ptr->max_age != COAP_OPTION_MAX_AGE_DEFAULT) {
                returned_byte_count += sn_coap_builder_options_calc_uint_option_size(src_options_list_ptr->max_age);
            }
            /* PROXY URI - Length of this option is  1-1034 bytes */
            if (src_options_list_ptr->proxy_uri_ptr != NULL) {
                if (src_options_list_ptr->proxy_uri_len >= 1 && src_options_list_ptr->proxy_uri_len <= 12) {            /* Add option header byte(s) - depending of option length */
                    returned_byte_count++;
                }

                else if (src_options_list_ptr->proxy_uri_len >= 13 && src_options_list_ptr->proxy_uri_len <= 269) {
                    returned_byte_count += 2;
                }

                else if (src_options_list_ptr->proxy_uri_len >= 270 && src_options_list_ptr->proxy_uri_len <= 1034) {
                    returned_byte_count += 3;
                }

                else {
                    tr_error("sn_coap_builder_calc_needed_packet_data_size_2 - proxy uri too large!");
                    return 0;
                }

                /* Add needed memory for Option value */
                returned_byte_count += src_options_list_ptr->proxy_uri_len;
            }
            /* ETAG - Repeatable option. Length of this option is 1-8 bytes*/
            if (src_options_list_ptr->etag_ptr != NULL) {
                repeatable_option_size = sn_coap_builder_options_calc_option_size(src_options_list_ptr->etag_len,
                                         src_options_list_ptr->etag_ptr, COAP_OPTION_ETAG);
                if (repeatable_option_size) {
                    returned_byte_count += repeatable_option_size;
                } else {
                    tr_error("sn_coap_builder_calc_needed_packet_data_size_2 - etag too large!");
                    return 0;
                }
            }
            /* URI HOST - Length of this option is 1-255 bytes */
            if (src_options_list_ptr->uri_host_ptr != NULL) {
                if (src_options_list_ptr->uri_host_len > 0 && src_options_list_ptr->uri_host_len <= 12) {
                    returned_byte_count++;
                }

                else if (src_options_list_ptr->uri_host_len >= 13 && src_options_list_ptr->uri_host_len <= 255) {
                    returned_byte_count += 2;
                }

                else {
                    tr_error("sn_coap_builder_calc_needed_packet_data_size_2 - uri host too large!");
                    return 0;
                }
                returned_byte_count += src_options_list_ptr->uri_host_len;
            }
            /* LOCATION PATH - Repeatable option. Length of this option is 0-255 bytes*/
            if (src_options_list_ptr->location_path_ptr != NULL) {
                repeatable_option_size = sn_coap_builder_options_calc_option_size(src_options_list_ptr->location_path_len,
                                         src_options_list_ptr->location_path_ptr, COAP_OPTION_LOCATION_PATH);
                if (repeatable_option_size) {
                    returned_byte_count += repeatable_option_size;
                } else {
                    tr_error("sn_coap_builder_calc_needed_packet_data_size_2 - location path too large!");
                    return 0;
                }
            }
            /* URI PORT - An integer option, up to 2 bytes */
            if (src_options_list_ptr->uri_port != COAP_OPTION_URI_PORT_NONE) {
                if ((uint32_t) src_options_list_ptr->uri_port > 0xffff) {
                    tr_error("sn_coap_builder_calc_needed_packet_data_size_2 - uri port too large!");
                    return 0;
                }
                returned_byte_count += sn_coap_builder_options_calc_uint_option_size(src_options_list_ptr->uri_port);
            }
            /* lOCATION QUERY - Repeatable option. Length of this option is 0-255 bytes */
            if (src_options_list_ptr->location_query_ptr != NULL) {
                repeatable_option_size = sn_coap_builder_options_calc_option_size(src_options_list_ptr->location_query_len,
                                         src_options_list_ptr->location_query_ptr, COAP_OPTION_LOCATION_QUERY);
                if (repeatable_option_size) {
                    returned_byte_count += repeatable_option_size;
                } else {
                    tr_error("sn_coap_builder_calc_needed_packet_data_size_2 - location query too large!");
                    return 0;
                }
            }
            /* OBSERVE - An integer option, up to 3 bytes */
            if (src_options_list_ptr->observe != COAP_OBSERVE_NONE) {
                if ((uint32_t) src_options_list_ptr->observe > 0xffffff) {
                    return 0;
                }
                returned_byte_count += sn_coap_builder_options_calc_uint_option_size(src_options_list_ptr->observe);
            }
            /* URI QUERY - Repeatable option. Length of this option is 1-255 */
            if (src_options_list_ptr->uri_query_ptr != NULL) {
                repeatable_option_size = sn_coap_builder_options_calc_option_size(src_options_list_ptr->uri_query_len,
                                         src_options_list_ptr->uri_query_ptr, COAP_OPTION_URI_QUERY);
                if (repeatable_option_size) {
                    returned_byte_count += repeatable_option_size;
                } else {
                    tr_error("sn_coap_builder_calc_needed_packet_data_size_2 - observe too large!");
                    return 0;
                }
            }

            /* BLOCK 1 - An integer option, up to 3 bytes */
            if (src_options_list_ptr->block1 != COAP_OPTION_BLOCK_NONE) {
                if ((uint32_t) src_options_list_ptr->block1 > 0xffffff) {
                    tr_error("sn_coap_builder_calc_needed_packet_data_size_2 - block1 too large!");
                    return 0;
                }
                returned_byte_count += sn_coap_builder_options_calc_uint_option_size(src_options_list_ptr->block1);
            }
            /* SIZE1 - Length of this option is 0-4 bytes */
            if (src_options_list_ptr->use_size1) {
                returned_byte_count += sn_coap_builder_options_calc_uint_option_size(src_options_list_ptr->size1);
            }
            /* BLOCK 2 - An integer option, up to 3 bytes */
            if (src_options_list_ptr->block2 != COAP_OPTION_BLOCK_NONE) {
                if ((uint32_t) src_options_list_ptr->block2 > 0xffffff) {
                    tr_error("sn_coap_builder_calc_needed_packet_data_size_2 - block2 too large!");
                    return 0;
                }
                returned_byte_count += sn_coap_builder_options_calc_uint_option_size(src_options_list_ptr->block2);
            }
            /* SIZE2 - Length of this option is 0-4 bytes */
            if (src_coap_msg_ptr->options_list_ptr->use_size2) {
                returned_byte_count += sn_coap_builder_options_calc_uint_option_size(src_options_list_ptr->size2);
            }
        }
#if SN_COAP_BLOCKWISE_ENABLED || SN_COAP_MAX_BLOCKWISE_PAYLOAD_SIZE
        if ((src_coap_msg_ptr->payload_len > SN_COAP_MAX_NONBLOCKWISE_PAYLOAD_SIZE) &&
            (src_coap_msg_ptr->payload_len > blockwise_payload_size) &&
            (blockwise_payload_size > 0)) {
            returned_byte_count += blockwise_payload_size;
        } else {
            returned_byte_count += src_coap_msg_ptr->payload_len;
        }
#else
        returned_byte_count += src_coap_msg_ptr->payload_len;
#endif
        if (src_coap_msg_ptr->payload_len) {
            returned_byte_count ++;    /* For payload marker */
        }
        returned_byte_count += sn_coap_builder_options_calculate_jump_need(src_coap_msg_ptr);
    }
    if (returned_byte_count > UINT16_MAX) {
        tr_error("sn_coap_builder_calc_needed_packet_data_size_2 - packet data size would overflow!");
        return 0;
    }
    return (uint16_t)returned_byte_count;
#endif
}

/**
 * \fn static uint8_t sn_coap_builder_options_calculate_jump_need(sn_coap_hdr_s *src_coap_msg_ptr)
 *
 * \brief Checks if there is need for option jump
 *
 * \param  *src_coap_msg_ptr is source of checked CoAP message
 *
 * \param  block option marks if block option is to be added to message later. 0 = no block option, 1 = block1 and 2 = block2
 *
 * \return Returns bytes needed for jumping
 */

static uint_fast8_t sn_coap_builder_options_calculate_jump_need(const sn_coap_hdr_s *src_coap_msg_ptr)
{
    uint8_t previous_option_number = 0;
    uint_fast8_t needed_space       = 0;

    const sn_coap_options_list_s* options_list_ptr = src_coap_msg_ptr->options_list_ptr;

    if (options_list_ptr != NULL) {


        /* If option numbers greater than 12 is not used, then jumping is not needed */
        //TODO: Check if this is really needed! Does it enhance perf? If not -> remove
        if (!options_list_ptr->uri_query_ptr       &&
                options_list_ptr->accept == COAP_CT_NONE &&
                !options_list_ptr->location_query_ptr &&
                options_list_ptr->block2 == COAP_OPTION_BLOCK_NONE &&
                options_list_ptr->block1 == COAP_OPTION_BLOCK_NONE &&
                !options_list_ptr->proxy_uri_ptr      &&
                options_list_ptr->max_age == COAP_OPTION_MAX_AGE_DEFAULT &&
                !options_list_ptr->use_size1          &&
                !options_list_ptr->use_size2) {
            return 0;
        }

        if (options_list_ptr->uri_host_ptr != NULL) {
            previous_option_number = (COAP_OPTION_URI_HOST);
        }

        if (options_list_ptr->etag_ptr != NULL) {
            previous_option_number = (COAP_OPTION_ETAG);
        }

        if (options_list_ptr->observe != COAP_OBSERVE_NONE) {
            previous_option_number = (COAP_OPTION_OBSERVE);
        }

        if (options_list_ptr->uri_port != COAP_OPTION_URI_PORT_NONE) {
            previous_option_number = (COAP_OPTION_URI_PORT);
        }

        if (options_list_ptr->location_path_ptr != NULL) {
            previous_option_number = (COAP_OPTION_LOCATION_PATH);
        }

        if (src_coap_msg_ptr->uri_path_ptr != NULL) {
            previous_option_number = (COAP_OPTION_URI_PATH);
        }
        if (src_coap_msg_ptr->content_format != COAP_CT_NONE) {
            previous_option_number = (COAP_OPTION_CONTENT_FORMAT);
        }

        if (options_list_ptr->max_age != COAP_OPTION_MAX_AGE_DEFAULT) {
            if ((COAP_OPTION_MAX_AGE - previous_option_number) > 12) {
                needed_space += 1;
            }
            previous_option_number = (COAP_OPTION_MAX_AGE);
        }

        if (options_list_ptr->uri_query_ptr != NULL) {
            if ((COAP_OPTION_URI_QUERY - previous_option_number) > 12) {
                needed_space += 1;
            }
            previous_option_number = (COAP_OPTION_URI_QUERY);
        }
        if (options_list_ptr->accept != COAP_CT_NONE) {
            if ((COAP_OPTION_ACCEPT - previous_option_number) > 12) {
                needed_space += 1;
            }
            previous_option_number = (COAP_OPTION_ACCEPT);
        }
        if (options_list_ptr->location_query_ptr != NULL) {
            if ((COAP_OPTION_LOCATION_QUERY - previous_option_number) > 12) {
                needed_space += 1;
            }
            previous_option_number = (COAP_OPTION_LOCATION_QUERY);
        }
        if (options_list_ptr->block2 != COAP_OPTION_BLOCK_NONE) {
            if ((COAP_OPTION_BLOCK2 - previous_option_number) > 12 ){
                needed_space += 1;
            }
            previous_option_number = (COAP_OPTION_BLOCK2);
        }
        if (options_list_ptr->block1 != COAP_OPTION_BLOCK_NONE) {
            if ((COAP_OPTION_BLOCK1 - previous_option_number) > 12 ){
                needed_space += 1;
            }
            previous_option_number = (COAP_OPTION_BLOCK1);
        }
        if (options_list_ptr->use_size2) {
            if ((COAP_OPTION_SIZE2 - previous_option_number) > 12) {
                needed_space += 1;
            }
            previous_option_number = (COAP_OPTION_SIZE2);
        }
        if (options_list_ptr->proxy_uri_ptr != NULL) {
            if ((COAP_OPTION_PROXY_URI - previous_option_number) > 12) {
                needed_space += 1;
            }
            if ((COAP_OPTION_PROXY_URI - previous_option_number) > 269) { //Can not happen
                needed_space += 1;
            }
            previous_option_number = (COAP_OPTION_PROXY_URI);
        }
        if (options_list_ptr->use_size1) {
            if ((COAP_OPTION_SIZE1 - previous_option_number) > 12) {
                needed_space += 1;
            }
            previous_option_number = (COAP_OPTION_SIZE1);
        }
    }

    return needed_space;
}

/**
 * \fn static int8_t sn_coap_builder_header_build(uint8_t *dst_packet_data_ptr, sn_coap_hdr_s *src_coap_msg_ptr)
 *
 * \brief Builds Header part of Packet data
 *
 * \param *dst_packet_data_ptr is destination for built Packet data
 *
 * \param *src_coap_msg_ptr is source for building Packet data
 *
 * \return Return value is 0 in ok case and -1 in failure case
 **************************************************************************** */
static uint8_t *sn_coap_builder_header_build(uint8_t * restrict dst_packet_data_ptr, const sn_coap_hdr_s * restrict src_coap_msg_ptr)
{
    /* * * * Check validity of Header values * * * */
    if (sn_coap_header_validity_check(src_coap_msg_ptr, COAP_VERSION) != 0) {
        tr_error("sn_coap_builder_header_build - header build failed!");
        return NULL;
    }

    /* Set CoAP Version, Message type and Token length */
    dst_packet_data_ptr[0] = COAP_VERSION | src_coap_msg_ptr->msg_type | src_coap_msg_ptr->token_len;

    /* * * Add Message code * * */
    dst_packet_data_ptr[1] = src_coap_msg_ptr->msg_code;

    /* * * Add Message ID * * */
    dst_packet_data_ptr[2] = (uint8_t)(src_coap_msg_ptr->msg_id >> COAP_HEADER_MSG_ID_MSB_SHIFT); /* MSB part */
    dst_packet_data_ptr[3] = (uint8_t)src_coap_msg_ptr->msg_id;                                   /* LSB part */

    dst_packet_data_ptr += 4;

    /* Success */
    return dst_packet_data_ptr;
}

/**
 * \fn static int8_t sn_coap_builder_options_build(uint8_t *dst_packet_data_ptr, sn_coap_hdr_s *src_coap_msg_ptr)
 *
 * \brief Builds Options part of Packet data
 *
 * \param *dst_packet_data_ptr is destination for built Packet data
 *
 * \param *src_coap_msg_ptr is source for building Packet data
 *
 * \return Returns updated output pointer
 */
static uint8_t *sn_coap_builder_options_build(uint8_t * restrict dst_packet_data_ptr, const sn_coap_hdr_s * restrict src_coap_msg_ptr)
{
    /* * * * Check if Options are used at all  * * * */
    if (src_coap_msg_ptr->uri_path_ptr == NULL && src_coap_msg_ptr->token_ptr == NULL &&
            src_coap_msg_ptr->content_format == COAP_CT_NONE && src_coap_msg_ptr->options_list_ptr == NULL) {
        /* If the empty message is confirmable it is CoAP ping. */
        if (src_coap_msg_ptr->msg_type != COAP_MSG_TYPE_CONFIRMABLE) {
            tr_error("sn_coap_builder_options_build - options not used!");
        }
        return dst_packet_data_ptr;
    }

    /* * * * First add Token option  * * * */
    if (src_coap_msg_ptr->token_len && src_coap_msg_ptr->token_ptr) {
        memcpy(dst_packet_data_ptr, src_coap_msg_ptr->token_ptr, src_coap_msg_ptr->token_len);
    }
    dst_packet_data_ptr += src_coap_msg_ptr->token_len;

    /* Then build rest of the options */

    /* * * * Initialize previous Option number for new built message * * * */
    uint16_t previous_option_number = 0;

    //missing: COAP_OPTION_IF_MATCH, COAP_OPTION_IF_NONE_MATCH, COAP_OPTION_SIZE

    const sn_coap_options_list_s * restrict src_options_list_ptr = src_coap_msg_ptr->options_list_ptr;

    /* Check if less used options are used at all */
    if (src_options_list_ptr != NULL) {
        /* * * * Build Uri-Host option * * * */
        dst_packet_data_ptr = sn_coap_builder_options_build_add_one_option(dst_packet_data_ptr, src_options_list_ptr->uri_host_len,
                     src_options_list_ptr->uri_host_ptr, COAP_OPTION_URI_HOST, &previous_option_number);

        /* * * * Build ETag option  * * * */
        dst_packet_data_ptr = sn_coap_builder_options_build_add_multiple_option(dst_packet_data_ptr, src_options_list_ptr->etag_ptr,
                     src_options_list_ptr->etag_len, COAP_OPTION_ETAG, &previous_option_number);

        /* * * * Build Observe option  * * * * */
        if (src_options_list_ptr->observe != COAP_OBSERVE_NONE) {
            dst_packet_data_ptr = sn_coap_builder_options_build_add_uint_option(dst_packet_data_ptr, src_options_list_ptr->observe,
                         COAP_OPTION_OBSERVE, &previous_option_number);
        }

        /* * * * Build Uri-Port option * * * */
        if (src_options_list_ptr->uri_port != COAP_OPTION_URI_PORT_NONE) {
            dst_packet_data_ptr = sn_coap_builder_options_build_add_uint_option(dst_packet_data_ptr, src_options_list_ptr->uri_port,
                         COAP_OPTION_URI_PORT, &previous_option_number);
        }

        /* * * * Build Location-Path option  * * * */
        dst_packet_data_ptr = sn_coap_builder_options_build_add_multiple_option(dst_packet_data_ptr, src_options_list_ptr->location_path_ptr,
                     src_options_list_ptr->location_path_len, COAP_OPTION_LOCATION_PATH, &previous_option_number);
    }
    /* * * * Build Uri-Path option * * * */
    dst_packet_data_ptr = sn_coap_builder_options_build_add_multiple_option(dst_packet_data_ptr, src_coap_msg_ptr->uri_path_ptr,
             src_coap_msg_ptr->uri_path_len, COAP_OPTION_URI_PATH, &previous_option_number);

    /* * * * Build Content-Type option * * * */
    if (src_coap_msg_ptr->content_format != COAP_CT_NONE) {
        dst_packet_data_ptr = sn_coap_builder_options_build_add_uint_option(dst_packet_data_ptr, src_coap_msg_ptr->content_format,
                     COAP_OPTION_CONTENT_FORMAT, &previous_option_number);
    }

    if (src_options_list_ptr != NULL) {
        /* * * * Build Max-Age option  * * * */
        if (src_options_list_ptr->max_age != COAP_OPTION_MAX_AGE_DEFAULT) {
            dst_packet_data_ptr = sn_coap_builder_options_build_add_uint_option(dst_packet_data_ptr, src_options_list_ptr->max_age,
                         COAP_OPTION_MAX_AGE, &previous_option_number);
        }

        /* * * * Build Uri-Query option  * * * * */
        dst_packet_data_ptr = sn_coap_builder_options_build_add_multiple_option(dst_packet_data_ptr, src_options_list_ptr->uri_query_ptr,
                     src_options_list_ptr->uri_query_len, COAP_OPTION_URI_QUERY, &previous_option_number);

        /* * * * Build Accept option  * * * * */
        if (src_coap_msg_ptr->options_list_ptr->accept != COAP_CT_NONE) {
            dst_packet_data_ptr = sn_coap_builder_options_build_add_uint_option(dst_packet_data_ptr, src_options_list_ptr->accept,
                         COAP_OPTION_ACCEPT, &previous_option_number);
        }

        /* * * * Build Location-Query option * * * */
        dst_packet_data_ptr = sn_coap_builder_options_build_add_multiple_option(dst_packet_data_ptr, src_options_list_ptr->location_query_ptr,
                     src_options_list_ptr->location_query_len, COAP_OPTION_LOCATION_QUERY, &previous_option_number);

        /* * * * Build Block2 option * * * * */
        if (src_coap_msg_ptr->options_list_ptr->block2 != COAP_OPTION_BLOCK_NONE) {
            dst_packet_data_ptr = sn_coap_builder_options_build_add_uint_option(dst_packet_data_ptr, src_options_list_ptr->block2,
                         COAP_OPTION_BLOCK2, &previous_option_number);
        }

        /* * * * Build Block1 option * * * * */
        if (src_coap_msg_ptr->options_list_ptr->block1 != COAP_OPTION_BLOCK_NONE) {
            dst_packet_data_ptr = sn_coap_builder_options_build_add_uint_option(dst_packet_data_ptr, src_options_list_ptr->block1,
                         COAP_OPTION_BLOCK1, &previous_option_number);
        }

        /* * * * Build Size2 option * * * */
        if (src_coap_msg_ptr->options_list_ptr->use_size2) {
            dst_packet_data_ptr = sn_coap_builder_options_build_add_uint_option(dst_packet_data_ptr, src_options_list_ptr->size2,
                         COAP_OPTION_SIZE2, &previous_option_number);
        }

        /* * * * Build Proxy-Uri option * * * */
        dst_packet_data_ptr = sn_coap_builder_options_build_add_one_option(dst_packet_data_ptr, src_options_list_ptr->proxy_uri_len,
                     src_options_list_ptr->proxy_uri_ptr, COAP_OPTION_PROXY_URI, &previous_option_number);


        /* * * * Build Size1 option * * * */
        if (src_options_list_ptr->use_size1) {
            dst_packet_data_ptr = sn_coap_builder_options_build_add_uint_option(dst_packet_data_ptr, src_options_list_ptr->size1,
                         COAP_OPTION_SIZE1, &previous_option_number);
        }
    }

    /* Success */
    return dst_packet_data_ptr;
}

/**
 * \fn static uint8_t *sn_coap_builder_options_build_add_one_option(uint8_t *dst_packet_data_ptr, uint16_t option_value_len, uint8_t *option_value_ptr, sn_coap_option_numbers_e option_number)
 *
 * \brief Adds Options part of Packet data
 *
 * \param *dst_packet_data_ptr is destination for built Packet data
 *
 * \param option_value_len is Option value length to be added
 *
 * \param *option_value_ptr is pointer to Option value data to be added
 *
 * \param option_number is Option number to be added
 *
 * \return Advanced destination
 */
static uint8_t *sn_coap_builder_options_build_add_one_option(uint8_t * restrict dst_packet_data_ptr, uint_fast16_t option_len,
        const uint8_t * restrict option_ptr, sn_coap_option_numbers_e option_number, uint16_t * restrict previous_option_number)
{
    /* Check if there is option at all */
    if (option_ptr != NULL) {
        uint_fast16_t option_delta;

        option_delta = (option_number - *previous_option_number);

        /* * * Build option header * * */

        uint_fast8_t first_byte;

        /* First option length without extended part */
        if (option_len <= 12) {
            first_byte = option_len;
        }

        else if (option_len > 12 && option_len < 269) {
            first_byte = 0x0D;
        }

        else /*if (option_len >= 269)*/ {
            first_byte = 0x0E;
        }

        /* Then option delta with extensions, and move pointer */
        if (option_delta <= 12) {
            *dst_packet_data_ptr++ = first_byte + (option_delta << 4);
        }

        else if (option_delta > 12 && option_delta < 269) {
            *dst_packet_data_ptr++ = first_byte + 0xD0;
            option_delta -= 13;

            *dst_packet_data_ptr++ = (uint8_t)option_delta;
        }
        //This is currently dead code (but possibly needed in future)
        else /*if (option_delta >= 269)*/ {
            *dst_packet_data_ptr++ = first_byte + 0xE0;
            option_delta -= 269;

            *dst_packet_data_ptr++ = (option_delta >> 8);
            *dst_packet_data_ptr++ = (uint8_t)option_delta;
        }

        /* Now option length extensions, if needed */
        if (option_len > 12 && option_len < 269) {
            *dst_packet_data_ptr++ = (uint8_t)(option_len - 13);
        }

        else if (option_len >= 269) {
            *dst_packet_data_ptr++ = ((option_len - 269) >> 8);
            *dst_packet_data_ptr++ = (uint8_t)(option_len - 269);
        }

        *previous_option_number = option_number;

        /* Write Option value */
        memcpy(dst_packet_data_ptr, option_ptr, option_len);

        /* Increase destination Packet data pointer */
        dst_packet_data_ptr += option_len;
    }

    /* Success */
    return dst_packet_data_ptr;
}

static uint_fast8_t sn_coap_builder_options_calc_uint_option_size(uint32_t option_value)
{
    // Calculation assumes option type/len is always 1 byte.
    // Length certainly fits, and any extra for option type is accounted for
    // separately by sn_coap_builder_options_calculate_jump_need.
    uint_fast8_t len = 1;

    while (option_value != 0) {
        len++;
        option_value >>= 8;
    }

    return len;
}

/**
 * \brief Constructs a uint Options part of Packet data
 *
 * \param *dst_packet_data_pptr is destination for built Packet data; NULL
 *        to compute size only.
 *
 * \param option_value is Option value to be added
 *
 * \param option_number is Option number to be added
 *
 * \return Updated destination pointer
 */
static uint8_t *sn_coap_builder_options_build_add_uint_option(uint8_t * restrict dst_packet_data_ptr, uint32_t option_value, sn_coap_option_numbers_e option_number, uint16_t * restrict previous_option_number)
{
    uint8_t payload[4];
    uint_fast8_t len = 0;
    /* Construct the variable-length payload representing the value */
    for (uint_fast8_t i = 0; i < 4; i++) {
        if (len > 0 || (option_value & 0xff000000)) {
            payload[len++] = option_value >> 24;
        }
        option_value <<= 8;
    }

    return sn_coap_builder_options_build_add_one_option(dst_packet_data_ptr, len, payload, option_number, previous_option_number);
}

/**
 * \fn static int16_t sn_coap_builder_options_build_add_multiple_option(uint8_t *dst_packet_data_pptr, uint8_t **src_pptr, uint16_t *src_len_ptr, sn_coap_option_numbers_e option)
 *
 * \brief Builds Option Uri-Query from given CoAP Header structure to Packet data
 *
 * \param *dst_packet_data_ptr is destination for built Packet data
 *
 * \param uint8_t **src_ptr
 *
 *  \param uint16_t src_len
 *
 *  \paramsn_coap_option_numbers_e option option to be added
 *
 * \return Returns updated output pointer
 */
static uint8_t *sn_coap_builder_options_build_add_multiple_option(uint8_t * restrict dst_packet_data_ptr, const uint8_t * restrict src_pptr, uint_fast16_t src_len, sn_coap_option_numbers_e option, uint16_t * restrict previous_option_number)
{
    /* Check if there is option at all */
    if (src_pptr != NULL) {
        const uint8_t * restrict query_ptr  = src_pptr;
        uint_fast8_t query_part_count       = 0;
        uint_fast16_t query_len             = src_len;
        uint_fast8_t i                      = 0;
        uint_fast16_t query_part_offset     = 0;

        /* Get query part count */
        query_part_count = sn_coap_builder_options_get_option_part_count(query_len, query_ptr, option);

        /* * * * Options by adding all parts to option * * * */
        for (i = 0; i < query_part_count; i++) {
            /* Get length of query part */
            uint_fast16_t one_query_part_len = sn_coap_builder_options_get_option_part_length_from_whole_option_string(query_len, query_ptr, i, option);

            /* Get position of query part */
            query_part_offset = sn_coap_builder_options_get_option_part_position(query_len, query_ptr, i, option);

            /* Add Uri-query's one part to Options */
            dst_packet_data_ptr = sn_coap_builder_options_build_add_one_option(dst_packet_data_ptr, one_query_part_len, src_pptr + query_part_offset, option, previous_option_number);
        }
    }
    /* Success */
    return dst_packet_data_ptr;
}


/**
 * \fn static uint16_t sn_coap_builder_options_calc_option_size(uint16_t query_len, uint8_t *query_ptr, sn_coap_option_numbers_e option)
 *
 * \brief Calculates needed Packet data memory size for option
 *
 * \param path_len is length of calculated strting(s)
 *
 * \param *path_ptr is pointer to calculated options
 *
 * \return Return value is count of needed memory as bytes for Uri-query option
 */
static uint_fast16_t sn_coap_builder_options_calc_option_size(uint16_t query_len, const uint8_t *query_ptr, sn_coap_option_numbers_e option)
{
    uint_fast8_t  query_part_count  = sn_coap_builder_options_get_option_part_count(query_len, query_ptr, option);
    uint_fast8_t  i                 = 0;
    uint_fast16_t ret_value         = 0;

    /* * * * * * * * * * * * * * * * * * * * * * * * */
    /* * * * Calculate Uri-query options length  * * */
    /* * * * * * * * * * * * * * * * * * * * * * * * */
    for (i = 0; i < query_part_count; i++) {
        /* * * Length of Option number and Option value length * * */

        /* Get length of Query part */
        uint_fast16_t one_query_part_len = sn_coap_builder_options_get_option_part_length_from_whole_option_string(query_len, query_ptr, i, option);

        /* Check option length */
        switch (option) {
            case (COAP_OPTION_ETAG):            /* Length 1-8 */
                if (one_query_part_len < 1 || one_query_part_len > 8) {
                    return 0;
                }
                break;
            case (COAP_OPTION_LOCATION_PATH):   /* Length 0-255 */
            case (COAP_OPTION_URI_PATH):        /* Length 0-255 */
            case (COAP_OPTION_LOCATION_QUERY):  /* Length 0-255 */
                if (one_query_part_len > 255) {
                    return 0;
                }
                break;
            case (COAP_OPTION_URI_QUERY):       /* Length 1-255 */
                if (one_query_part_len < 1 || one_query_part_len > 255) {
                    return 0;
                }
                break;
//            case (COAP_OPTION_ACCEPT):          /* Length 0-2 */
//                if (one_query_part_len > 2) {
//                    return 0;
//                }
//                break;
            default:
                break; //impossible scenario currently
        }

        /* Check if 4 bits are enough for writing Option value length */
        if (one_query_part_len <= 12) {
            /* 4 bits are enough for Option value length */
            ret_value++;
        } else if (one_query_part_len >= 13 && one_query_part_len < 269) {
            /* Extra byte for Option value length is needed */
            ret_value += 2;
        }
        //This can only happen if we are in default case above, currently is not happening
        else if (one_query_part_len >= 270 && one_query_part_len < 1034) {
            /* Extra bytes for Option value length is needed */
            ret_value += 3;
        }


        /* * * Length of Option value * * */

        /* Increase options length */
        ret_value += one_query_part_len;
    }

    /* Success */
    return ret_value;
}



/**
 * \fn static uint8_t sn_coap_builder_options_get_option_part_count(uint16_t query_len, uint8_t *query_ptr, sn_coap_option_numbers_e option)
 *
 * \brief Gets query part count from whole option string
 *
 * \param query_len is length of whole Path
 *
 * \param *query_ptr is pointer to the start of whole Path
 *
 * \return Return value is count of query parts
 */
static uint_fast8_t sn_coap_builder_options_get_option_part_count(uint_fast16_t query_len, const uint8_t *query_ptr, sn_coap_option_numbers_e option)
{
    if (query_len <= 2) {
        return 1;
    }

    const uint8_t *query_end           = query_ptr + query_len - 1;
    uint8_t       char_to_search       = '&';

    if (option == COAP_OPTION_URI_PATH || option == COAP_OPTION_LOCATION_PATH) {
        char_to_search = '/';
    }

    /* Loop whole query and search '\0' characters (not first and last char) */
    uint_fast8_t  returned_query_count = 1;
    query_ptr++;
    do {
        if (*query_ptr++ == char_to_search) { /* If match */
            returned_query_count++;
        }
    } while (query_ptr < query_end);

    return returned_query_count;
}

/**
 * \fn static uint16_t sn_coap_builder_options_get_option_part_length_from_whole_option_string(uint16_t query_len,
                                                                             uint8_t *query_ptr,
                                                                             uint8_t query_index, sn_coap_option_numbers_e option)
 *
 * \brief Gets one's query part length from whole query string
 *
 * \param query_len is length of whole string
 *
 * \param *query_ptr is pointer to the start of whole string
 *
 * \param query_index is query part index to be found
 *
 * \param sn_coap_option_numbers_e option is option number of the option
 *
 * \return Return value is length of query part
 */
static uint_fast16_t sn_coap_builder_options_get_option_part_length_from_whole_option_string(uint_fast16_t query_len, const uint8_t *query_ptr,
        uint_fast8_t query_index, sn_coap_option_numbers_e option)
{
    uint_fast16_t returned_query_part_len = 0;
    uint_fast8_t  temp_query_index        = 0;
    uint_fast16_t query_len_index         = 0;
    uint_fast8_t  char_to_search          = '&';

    if (option == COAP_OPTION_URI_PATH || option == COAP_OPTION_LOCATION_PATH) {
        char_to_search = '/';
    }

    /* Loop whole query and search '\0' characters */
    for (query_len_index = 0; query_len_index < query_len; query_len_index++) {
        /* Store character to temp_char for helping debugging */
        uint_fast8_t temp_char = *query_ptr;

        /* If new query part starts */
        if (temp_char == char_to_search && returned_query_part_len > 0) { /* returned_query_part_len > 0 is for querys which start with "\0" */
            /* If query part index is wanted */
            if (temp_query_index == query_index) {
                /* Return length of query part */
                return returned_query_part_len;
            } else {
                /* Reset length of query part because wanted query part finding continues*/
                returned_query_part_len = 0;
            }

            /* Next query part is looped */
            temp_query_index++;
        } else if (temp_char != char_to_search) { /* Else if query part continues */
            /* Increase query part length */
            returned_query_part_len++;
        }

        query_ptr++;
    }

    /* Return length of query part in cases that query part does not finish to '\0' character (last query part can be like that) */
    return returned_query_part_len;
}

/**
 * \fn static uint16_t sn_coap_builder_options_get_option_part_position(uint16_t query_len,
                                                               uint8_t *query_ptr,
                                                               uint8_t query_index, sn_coap_option_numbers_e option)
 *
 * \brief Gets query part position in whole query
 *
 * \param query_len is length of whole query
 *
 * \param *query_ptr is pointer to the start of whole query
 *
 * \param query_index is query part index to be found
 *
 * \return Return value is position (= offset) of query part in whole query. In
 *         fail cases -1 is returned.
 */
static int_fast16_t sn_coap_builder_options_get_option_part_position(uint_fast16_t query_len, const uint8_t *query_ptr,
        uint_fast8_t query_index, sn_coap_option_numbers_e option)
{
    uint_fast16_t returned_query_part_offset = 0;
    uint_fast8_t  temp_query_index           = 0;
    uint_fast16_t query_len_index            = 0;
    uint_fast8_t  char_to_search             = '&';

    if (option == COAP_OPTION_URI_PATH || option == COAP_OPTION_LOCATION_PATH) {
        char_to_search = '/';
    }

    if (query_index == 0) {
        if (*query_ptr == 0 || *query_ptr == char_to_search) {
            return 1;
        } else {
            return 0;
        }
    }

    /* Loop whole query and search separator characters */
    for (query_len_index = 0; query_len_index < query_len; query_len_index++) {
        /* Store character to temp_char for helping debugging */
        uint_fast8_t temp_char = *query_ptr;

        /* If new query part starts */
        if (temp_char == char_to_search && returned_query_part_offset > 0) { /* returned_query_part_offset > 0 is for querys which start with searched char */
            /* If query part index is wanted */
            if (temp_query_index == (query_index - 1)) {
                /* Return offset of query part */
                return (returned_query_part_offset + 1); /* Plus one is for passing separator */
            }

            /* Next query part is looped */
            temp_query_index++;
        }

        returned_query_part_offset++;

        query_ptr++;
    }

    return -1; //Dead code?
}


/**
 * \fn static void sn_coap_builder_payload_build(uint8_t *dst_packet_data_pptr, sn_coap_hdr_s *src_coap_msg_ptr)
 *
 * \brief Builds Options part of Packet data
 *
 * \param *dst_packet_data_ptr is destination for built Packet data
 *
 * \param *src_coap_msg_ptr is source for building Packet data
 */
static uint8_t *sn_coap_builder_payload_build(uint8_t * restrict dst_packet_data_ptr, const sn_coap_hdr_s * restrict src_coap_msg_ptr)
{
    /* Check if Payload is used at all */
    if (src_coap_msg_ptr->payload_len && src_coap_msg_ptr->payload_ptr != NULL) {
        /* Write Payload marker */

        *dst_packet_data_ptr++ = 0xff;

        /* Write Payload */
        memcpy(dst_packet_data_ptr, src_coap_msg_ptr->payload_ptr, src_coap_msg_ptr->payload_len);

        /* Increase destination Packet data pointer */
        dst_packet_data_ptr += src_coap_msg_ptr->payload_len;
    }

    return dst_packet_data_ptr;
}
