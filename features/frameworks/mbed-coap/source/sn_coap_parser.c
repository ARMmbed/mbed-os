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

static const uint8_t *sn_coap_parser_header_parse(const uint8_t *packet_data_ptr, sn_coap_hdr_s *dst_coap_msg_ptr, coap_version_e *coap_version_ptr);
static const uint8_t *sn_coap_parser_options_parse(const uint8_t *packet_data_ptr, struct coap_s *handle, sn_coap_hdr_s *dst_coap_msg_ptr, const uint8_t *packet_data_start_ptr, uint_fast16_t packet_len);
static const uint8_t *sn_coap_parser_options_parse_multiple_options(const uint8_t *packet_data_ptr, struct coap_s *handle, uint_fast16_t packet_left_len,  uint8_t **dst_pptr, uint16_t *dst_len_ptr, sn_coap_option_numbers_e option, uint_fast16_t option_number_len);
static int            sn_coap_parser_options_count_needed_memory_multiple_option(const uint8_t *packet_data_ptr, uint_fast16_t packet_left_len, sn_coap_option_numbers_e option, uint_fast16_t option_number_len);
static const uint8_t *sn_coap_parser_payload_parse(const uint8_t *packet_data_ptr, uint16_t packet_data_len, uint8_t *packet_data_start_ptr, sn_coap_hdr_s *dst_coap_msg_ptr);

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

sn_coap_hdr_s *sn_coap_parser_alloc_message_with_options(struct coap_s *handle)
{
    // check the handle just as in any other place
    if (handle == NULL) {
        return NULL;
    }

    sn_coap_hdr_s *coap_msg_ptr = sn_coap_parser_alloc_message(handle);

    sn_coap_options_list_s *options_list_ptr = sn_coap_parser_alloc_options(handle, coap_msg_ptr);

    if ((coap_msg_ptr == NULL) || (options_list_ptr == NULL)) {

        // oops, out of memory free if got already any
        handle->sn_coap_protocol_free(coap_msg_ptr);
        handle->sn_coap_protocol_free(options_list_ptr);

        coap_msg_ptr = NULL;
    }

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
    sn_coap_options_list_s *options_list_ptr;

    /* * * * Check given pointers * * * */
    if (handle == NULL || coap_msg_ptr == NULL) {
        return NULL;
    }

    /* * * * If the message already has options, return them * * * */
    if (coap_msg_ptr->options_list_ptr) {
        return coap_msg_ptr->options_list_ptr;
    }

    /* * * * Allocate memory for options and initialize allocated memory with with default values  * * * */
    /* XXX not technically legal to memset pointers to 0 */
    options_list_ptr = sn_coap_protocol_calloc(handle, sizeof(sn_coap_options_list_s));

    if (options_list_ptr == NULL) {
        tr_error("sn_coap_parser_alloc_options - failed to allocate options list!");
        return NULL;
    }

    coap_msg_ptr->options_list_ptr = options_list_ptr;

    options_list_ptr->uri_port = COAP_OPTION_URI_PORT_NONE;
    options_list_ptr->observe = COAP_OBSERVE_NONE;
    options_list_ptr->accept = COAP_CT_NONE;
    options_list_ptr->block2 = COAP_OPTION_BLOCK_NONE;
    options_list_ptr->block1 = COAP_OPTION_BLOCK_NONE;

    return options_list_ptr;
}

sn_coap_hdr_s *sn_coap_parser(struct coap_s *handle, uint16_t packet_data_len, uint8_t *packet_data_ptr, coap_version_e *coap_version_ptr)
{
    const uint8_t *data_temp_ptr                    = packet_data_ptr;
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
    data_temp_ptr = sn_coap_parser_header_parse(data_temp_ptr, parsed_and_returned_coap_msg_ptr, coap_version_ptr);

    /* * * * Options parsing, move pointer over the options... * * * */
    data_temp_ptr = sn_coap_parser_options_parse(data_temp_ptr, handle, parsed_and_returned_coap_msg_ptr, packet_data_ptr, packet_data_len);
    if (!data_temp_ptr) {
        parsed_and_returned_coap_msg_ptr->coap_status = COAP_STATUS_PARSER_ERROR_IN_HEADER;
        return parsed_and_returned_coap_msg_ptr;
    }

    /* * * * Payload parsing * * * */
    data_temp_ptr = sn_coap_parser_payload_parse(data_temp_ptr, packet_data_len, packet_data_ptr, parsed_and_returned_coap_msg_ptr);
    if (!data_temp_ptr) {
        parsed_and_returned_coap_msg_ptr->coap_status = COAP_STATUS_PARSER_ERROR_IN_HEADER;
        return parsed_and_returned_coap_msg_ptr;
    }

    parsed_and_returned_coap_msg_ptr->coap_status = COAP_STATUS_OK;

    /* * * * Return parsed CoAP message  * * * * */
    return parsed_and_returned_coap_msg_ptr;
}

void sn_coap_parser_release_allocated_coap_msg_mem(struct coap_s *handle, sn_coap_hdr_s *freed_coap_msg_ptr)
{
    if (handle == NULL) {
        return;
    }

    if (freed_coap_msg_ptr != NULL) {

        // As there are multiple sequential calls to the protocol_free, caching pointer to it
        // saves one instruction per call.
        void (*local_free)(void *) = handle->sn_coap_protocol_free;

        local_free(freed_coap_msg_ptr->uri_path_ptr);
        local_free(freed_coap_msg_ptr->token_ptr);

        // same here, caching the struct start saves a bit.
        sn_coap_options_list_s *options_list_ptr = freed_coap_msg_ptr->options_list_ptr;

        if (options_list_ptr != NULL) {

            local_free(options_list_ptr->proxy_uri_ptr);

            local_free(options_list_ptr->etag_ptr);

            local_free(options_list_ptr->uri_host_ptr);

            local_free(options_list_ptr->location_path_ptr);

            local_free(options_list_ptr->location_query_ptr);

            local_free(options_list_ptr->uri_query_ptr);

            local_free(options_list_ptr);
        }

        local_free(freed_coap_msg_ptr);
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
static const uint8_t *sn_coap_parser_header_parse(const uint8_t * restrict packet_data_ptr, sn_coap_hdr_s * restrict dst_coap_msg_ptr, coap_version_e * restrict coap_version_ptr)
{
    /* Parse CoAP Version and message type*/
    *coap_version_ptr = (coap_version_e)(*packet_data_ptr & COAP_HEADER_VERSION_MASK);
    dst_coap_msg_ptr->msg_type = (sn_coap_msg_type_e)(*packet_data_ptr++ & COAP_HEADER_MSG_TYPE_MASK);

    /* Parse Message code */
    dst_coap_msg_ptr->msg_code = (sn_coap_msg_code_e) *packet_data_ptr++;

    /* Parse Message ID */
    dst_coap_msg_ptr->msg_id = *packet_data_ptr++ << COAP_HEADER_MSG_ID_MSB_SHIFT;
    dst_coap_msg_ptr->msg_id |= *packet_data_ptr++;

    return packet_data_ptr;
}

/**
 * \brief Parses a variable-length uint value from an option
 *
 * \param **packet_data_pptr is source of option data to be parsed
 * \param option_len is length of option data (will be 0-4)
 *
 * \return Return value is value of uint
 */
static uint32_t sn_coap_parser_options_parse_uint(const uint8_t * restrict * restrict packet_data_pptr, uint_fast8_t option_len)
{
    uint32_t value = 0;
    const uint8_t *packet_data_ptr = *packet_data_pptr;
    while (option_len--) {
        value <<= 8;
        value |= *packet_data_ptr++;
    }
    *packet_data_pptr = packet_data_ptr;
    return value;
}

/**
 * \brief Add u16 integers with overflow detection
 *
 * \param a            first term of addition
 * \param b            second term of addion
 * \param result       pointer to the result variable
 *
 * \return Return 0 if there was no overflow, non-zero otherwise
 */
static int_fast8_t sn_coap_parser_add_u16_limit(uint_fast16_t a, uint_fast16_t b, uint_fast16_t *result)
{
    uint16_t c;

    c = a + b;
    if (c < a || c < b) {
        return 1;
    }

    *result = c;

    return 0;
}

/**
 * \brief Performs data packet pointer boundary check
 *
 * \param packet_data_ptr           current data packet read pointer
 * \param packet_data_start_ptr     pointer to data packet start
 * \param packet_len                total packet length
 * \param delta                     the number of bytes forward to check
 *
 * \return Return 0 if the data is within the bounds, non-zero otherwise
 */
static int_fast8_t sn_coap_parser_check_packet_ptr(const uint8_t *packet_data_ptr, const uint8_t *packet_data_start_ptr, uint_fast16_t packet_len, uint_fast16_t delta)
{
    const uint8_t *packet_end = packet_data_start_ptr + packet_len;
    const uint8_t *new_data_ptr = packet_data_ptr + delta;

    if (delta > packet_len) {
        return 1;
    }

    if (new_data_ptr < packet_data_start_ptr || new_data_ptr > packet_end) {
        return 1;
    }

    return 0;
}

/**
 * \brief Increments data packet pointer with boundary check
 *
 * \param packet_data_pptr          pointer to data packet current pointer
 * \param packet_data_start_ptr     pointer to data packet start
 * \param packet_len                total packet length
 * \param delta                     the number of bytes to move *packet_data_pptr forward
 *
 * \return Return The remaining packet data length
 */
static uint_fast16_t sn_coap_parser_move_packet_ptr(const uint8_t * restrict *packet_data_pptr, const uint8_t *packet_data_start_ptr, uint_fast16_t packet_len, uint_fast16_t delta)
{
    const uint8_t *packet_end = packet_data_start_ptr + packet_len;
    const uint8_t *new_data_ptr = *packet_data_pptr + delta;

    if (new_data_ptr < packet_data_start_ptr) {
        return 0;
    } else if (new_data_ptr >= packet_end) {
        *packet_data_pptr = packet_end;
        return 0;
    }

    *packet_data_pptr = new_data_ptr;

    return (uint_fast16_t)(packet_end - new_data_ptr);
}

/**
 * \brief Read byte from buffer with boundary check
 *
 * \param dst                       pointer to destination variable
 * \param packet_data_ptr           current data packet read pointer
 * \param packet_data_start_ptr     pointer to data packet start
 * \param packet_len                total packet length
 *
 * \return Return 0 if the data is within the bounds, non-zero otherwise
 */
static int8_t sn_coap_parser_read_packet_u8(uint8_t *dst, const uint8_t *packet_data_ptr, const uint8_t *packet_data_start_ptr, uint_fast16_t packet_len)
{
    int_fast8_t ptr_check_result;

    ptr_check_result = sn_coap_parser_check_packet_ptr(packet_data_ptr, packet_data_start_ptr, packet_len, 1);

    if (ptr_check_result != 0) {
        return ptr_check_result;
    }

    *dst = *packet_data_ptr;

    return 0;
}

/**
 * \brief Read unsinged 16-bit variable from buffer with boundary check.
 *
 * The read is performed in big-endian order.
 *
 * \param dst                       pointer to destination variable
 * \param packet_data_ptr           current data packet read pointer
 * \param packet_data_start_ptr     pointer to data packet start
 * \param packet_len                total packet length
 *
 * \return Return 0 if the data is within the bounds, non-zero otherwise
 */
static int_fast8_t sn_coap_parser_read_packet_u16(uint_fast16_t *dst, const uint8_t *packet_data_ptr, const uint8_t *packet_data_start_ptr, uint16_t packet_len)
{
    int_fast8_t ptr_check_result;
    uint_fast16_t value;

    ptr_check_result = sn_coap_parser_check_packet_ptr(packet_data_ptr, packet_data_start_ptr, packet_len, 2);

    if (ptr_check_result != 0) {
        return ptr_check_result;
    }

    value = *(packet_data_ptr) << 8;
    value |= *(packet_data_ptr + 1);
    *dst = value;

    return 0;
}

/**
 * \brief Read extended option length or delta with buffer boundary check.
 *
 * \param dst                       pointer to destination option delta or length variable
 * \param packet_data_pptr          current data packet read pointer
 * \param packet_data_start_ptr     pointer to data packet start
 * \param packet_len                total packet length
 * \param message_left              pointer to variable holding remaining bytes to parse
 *
 * \return Return 0 if the read was successful, -1 otherwise
 */
static int_fast8_t parse_ext_option(uint_fast16_t *dst, const uint8_t * restrict *packet_data_pptr, const uint8_t *packet_data_start_ptr, uint_fast16_t packet_len, uint_fast16_t *message_left)
{
    uint_fast16_t option_number = *dst;

    if (option_number == 13) {
        uint8_t option_ext;
        int8_t read_result = sn_coap_parser_read_packet_u8(&option_ext, *packet_data_pptr, packet_data_start_ptr, packet_len);
        if (read_result != 0) {
            /* packet_data_pptr would overflow! */
            tr_error("sn_coap_parser_options_parse - **packet_data_pptr overflow !");
            return -1;
        } else {
            if (sn_coap_parser_add_u16_limit(option_number, option_ext, &option_number) != 0) {
                return -1;
            }

            *message_left = sn_coap_parser_move_packet_ptr(packet_data_pptr, packet_data_start_ptr, packet_len, 1);
        }
    } else if (option_number == 14) {
        int_fast8_t read_result = sn_coap_parser_read_packet_u16(&option_number, *packet_data_pptr, packet_data_start_ptr, packet_len);
        if (read_result != 0) {
            /* packet_data_pptr would overflow! */
            tr_error("sn_coap_parser_options_parse - **packet_data_pptr overflow !");
            return -1;
        } else {
            if (sn_coap_parser_add_u16_limit(option_number, 269, &option_number) != 0) {
                return -1;
            }

            *message_left = sn_coap_parser_move_packet_ptr(packet_data_pptr, packet_data_start_ptr, packet_len, 2);
        }
    }
    /* Option number 15 reserved for payload marker. This is handled as a error! */
    else if (option_number == 15) {
        tr_error("sn_coap_parser_options_parse - invalid option number(15)!");
        return -1;
    }

    *dst = option_number;
    return 0;
}

/**
 * \fn static uint8_t sn_coap_parser_options_parse(uint8_t **packet_data_pptr, sn_coap_hdr_s *dst_coap_msg_ptr)
 *
 * \brief Parses CoAP message's Options part from given Packet data
 *
 * \param *packet_data_ptr is source of Packet data to be parsed to CoAP message
 * \param *dst_coap_msg_ptr is destination for parsed CoAP message
 *
 * \return Return value is advanced input pointer in ok case and NULL in failure case
 */
static const uint8_t * sn_coap_parser_options_parse(const uint8_t * restrict packet_data_ptr, struct coap_s * restrict handle, sn_coap_hdr_s * restrict dst_coap_msg_ptr, const uint8_t *packet_data_start_ptr, uint_fast16_t packet_len)
{
    uint_fast16_t previous_option_number = 0;
    uint_fast16_t message_left           = sn_coap_parser_move_packet_ptr(&packet_data_ptr, packet_data_start_ptr, packet_len, 0);

    /*  Parse token, if exists  */
    dst_coap_msg_ptr->token_len = *packet_data_start_ptr & COAP_HEADER_TOKEN_LENGTH_MASK;

    if (dst_coap_msg_ptr->token_len) {
        int_fast8_t ptr_check_result;
        if ((dst_coap_msg_ptr->token_len > 8) || dst_coap_msg_ptr->token_ptr) {
            tr_error("sn_coap_parser_options_parse - token not valid!");
            return NULL;
        }

        ptr_check_result = sn_coap_parser_check_packet_ptr(packet_data_ptr, packet_data_start_ptr, packet_len, dst_coap_msg_ptr->token_len);
        if (0 != ptr_check_result) {
            tr_error("sn_coap_parser_options_parse - *packet_data_ptr overflow !");
            return NULL;
        }

        dst_coap_msg_ptr->token_ptr = sn_coap_protocol_malloc_copy(handle, packet_data_ptr, dst_coap_msg_ptr->token_len);

        if (dst_coap_msg_ptr->token_ptr == NULL) {
            tr_error("sn_coap_parser_options_parse - failed to allocate token!");
            return NULL;
        }

        message_left = sn_coap_parser_move_packet_ptr(&packet_data_ptr, packet_data_start_ptr, packet_len, dst_coap_msg_ptr->token_len);
    }

    message_left = packet_len - (packet_data_ptr - packet_data_start_ptr);

    /* Loop all Options */
    uint_fast8_t option_byte;
    while (message_left && ((option_byte = *packet_data_ptr) != 0xff)) {
        /* Get option length WITHOUT extensions */
        uint_fast16_t option_len = (option_byte & 0x0F);
        /* Get option number WITHOUT extensions */
        uint_fast16_t option_number = (option_byte >> COAP_OPTIONS_OPTION_NUMBER_SHIFT);

        message_left = sn_coap_parser_move_packet_ptr(&packet_data_ptr, packet_data_start_ptr, packet_len, 1);
        int_fast8_t    option_parse_result;
        /* Add possible option delta extension */
        option_parse_result = parse_ext_option(&option_number, &packet_data_ptr, packet_data_start_ptr, packet_len, &message_left);
        if (option_parse_result != 0) {
            return NULL;
        }
        /* Add previous option to option delta and get option number */
        if (sn_coap_parser_add_u16_limit(option_number, previous_option_number, &option_number) != 0) {
            return NULL;
        }

        /* Add possible option length extension to resolve full length of the option */
        option_parse_result = parse_ext_option(&option_len, &packet_data_ptr, packet_data_start_ptr, packet_len, &message_left);
        if (option_parse_result != 0) {
            return NULL;
        }

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
                    return NULL;
                }
                break;
        }

        if (message_left < option_len){
            /* packet_data_ptr would overflow! */
            tr_error("sn_coap_parser_options_parse - *packet_data_ptr would overflow when parsing options!");
            return NULL;
        }

        /* Parse option */
        switch (option_number) {
            case COAP_OPTION_CONTENT_FORMAT:
                if ((option_len > 2) || (dst_coap_msg_ptr->content_format != COAP_CT_NONE)) {
                    tr_error("sn_coap_parser_options_parse - COAP_OPTION_CONTENT_FORMAT not valid!");
                    return NULL;
                }
                dst_coap_msg_ptr->content_format = (sn_coap_content_format_e) sn_coap_parser_options_parse_uint(&packet_data_ptr, option_len);
                break;

            case COAP_OPTION_MAX_AGE:
                if (option_len > 4) {
                    tr_error("sn_coap_parser_options_parse - COAP_OPTION_MAX_AGE not valid!");
                    return NULL;
                }
                dst_coap_msg_ptr->options_list_ptr->max_age = sn_coap_parser_options_parse_uint(&packet_data_ptr, option_len);
                break;

            case COAP_OPTION_PROXY_URI:
                if ((option_len > 1034) || (option_len < 1) || dst_coap_msg_ptr->options_list_ptr->proxy_uri_ptr) {
                    tr_error("sn_coap_parser_options_parse - COAP_OPTION_PROXY_URI not valid!");
                    return NULL;
                }
                dst_coap_msg_ptr->options_list_ptr->proxy_uri_len = option_len;
                dst_coap_msg_ptr->options_list_ptr->proxy_uri_ptr = sn_coap_protocol_malloc_copy(handle, packet_data_ptr, option_len);

                if (dst_coap_msg_ptr->options_list_ptr->proxy_uri_ptr == NULL) {
                    tr_error("sn_coap_parser_options_parse - COAP_OPTION_PROXY_URI allocation failed!");
                    return NULL;
                }
                message_left = sn_coap_parser_move_packet_ptr(&packet_data_ptr, packet_data_start_ptr, packet_len, option_len);
                break;

            case COAP_OPTION_ETAG:
                if (dst_coap_msg_ptr->options_list_ptr->etag_ptr) {
                    tr_error("sn_coap_parser_options_parse - COAP_OPTION_ETAG exists!");
                    return NULL;
                }
                /* This is managed independently because User gives this option in one character table */
                uint16_t len;
                packet_data_ptr = sn_coap_parser_options_parse_multiple_options(packet_data_ptr, handle,
                             message_left,
                             &dst_coap_msg_ptr->options_list_ptr->etag_ptr,
                             &len,
                             COAP_OPTION_ETAG, option_len);
                if (!packet_data_ptr) {
                    tr_error("sn_coap_parser_options_parse - COAP_OPTION_ETAG not valid!");
                    return NULL;
                }
                dst_coap_msg_ptr->options_list_ptr->etag_len = (uint8_t) len;
                break;

            case COAP_OPTION_URI_HOST:
                if ((option_len > 255) || (option_len < 1) || dst_coap_msg_ptr->options_list_ptr->uri_host_ptr) {
                    tr_error("sn_coap_parser_options_parse - COAP_OPTION_URI_HOST not valid!");
                    return NULL;
                }
                dst_coap_msg_ptr->options_list_ptr->uri_host_len = option_len;
                dst_coap_msg_ptr->options_list_ptr->uri_host_ptr = sn_coap_protocol_malloc_copy(handle, packet_data_ptr, option_len);

                if (dst_coap_msg_ptr->options_list_ptr->uri_host_ptr == NULL) {
                    tr_error("sn_coap_parser_options_parse - COAP_OPTION_URI_HOST allocation failed!");
                    return NULL;
                }
                message_left = sn_coap_parser_move_packet_ptr(&packet_data_ptr, packet_data_start_ptr, packet_len, option_len);
                break;

            case COAP_OPTION_LOCATION_PATH:
                if (dst_coap_msg_ptr->options_list_ptr->location_path_ptr) {
                    tr_error("sn_coap_parser_options_parse - COAP_OPTION_LOCATION_PATH exists!");
                    return NULL;
                }
                /* This is managed independently because User gives this option in one character table */
                packet_data_ptr = sn_coap_parser_options_parse_multiple_options(packet_data_ptr, handle, message_left,
                             &dst_coap_msg_ptr->options_list_ptr->location_path_ptr, &dst_coap_msg_ptr->options_list_ptr->location_path_len,
                             COAP_OPTION_LOCATION_PATH, option_len);
                if (!packet_data_ptr) {
                    tr_error("sn_coap_parser_options_parse - COAP_OPTION_LOCATION_PATH not valid!");
                    return NULL;
                }
                break;

            case COAP_OPTION_URI_PORT:
                if ((option_len > 2) || dst_coap_msg_ptr->options_list_ptr->uri_port != COAP_OPTION_URI_PORT_NONE) {
                    tr_error("sn_coap_parser_options_parse - COAP_OPTION_URI_PORT not valid!");
                    return NULL;
                }
                dst_coap_msg_ptr->options_list_ptr->uri_port = sn_coap_parser_options_parse_uint(&packet_data_ptr, option_len);
                break;

            case COAP_OPTION_LOCATION_QUERY:
                if (dst_coap_msg_ptr->options_list_ptr->location_query_ptr) {
                    tr_error("sn_coap_parser_options_parse - COAP_OPTION_LOCATION_QUERY exists!");
                    return NULL;
                }
                packet_data_ptr = sn_coap_parser_options_parse_multiple_options(packet_data_ptr, handle, message_left,
                             &dst_coap_msg_ptr->options_list_ptr->location_query_ptr, &dst_coap_msg_ptr->options_list_ptr->location_query_len,
                             COAP_OPTION_LOCATION_QUERY, option_len);
                if (!packet_data_ptr) {
                    tr_error("sn_coap_parser_options_parse - COAP_OPTION_LOCATION_QUERY not valid!");
                    return NULL;
                }

                break;

            case COAP_OPTION_URI_PATH:
                if (dst_coap_msg_ptr->uri_path_ptr) {
                    tr_error("sn_coap_parser_options_parse - COAP_OPTION_URI_PATH exists!");
                    return NULL;
                }
                packet_data_ptr = sn_coap_parser_options_parse_multiple_options(packet_data_ptr, handle, message_left,
                             &dst_coap_msg_ptr->uri_path_ptr, &dst_coap_msg_ptr->uri_path_len,
                             COAP_OPTION_URI_PATH, option_len);
                if (!packet_data_ptr) {
                    tr_error("sn_coap_parser_options_parse - COAP_OPTION_URI_PATH not valid!");
                    return NULL;
                }
                break;

            case COAP_OPTION_OBSERVE:
                if ((option_len > 2) || dst_coap_msg_ptr->options_list_ptr->observe != COAP_OBSERVE_NONE) {
                    tr_error("sn_coap_parser_options_parse - COAP_OPTION_OBSERVE not valid!");
                    return NULL;
                }
                dst_coap_msg_ptr->options_list_ptr->observe = sn_coap_parser_options_parse_uint(&packet_data_ptr, option_len);
                break;

            case COAP_OPTION_URI_QUERY:
                packet_data_ptr = sn_coap_parser_options_parse_multiple_options(packet_data_ptr, handle, message_left,
                             &dst_coap_msg_ptr->options_list_ptr->uri_query_ptr, &dst_coap_msg_ptr->options_list_ptr->uri_query_len,
                             COAP_OPTION_URI_QUERY, option_len);
                if (!packet_data_ptr) {
                    tr_error("sn_coap_parser_options_parse - COAP_OPTION_URI_QUERY not valid!");
                    return NULL;
                }
                break;

            case COAP_OPTION_BLOCK2:
                if ((option_len > 3) || dst_coap_msg_ptr->options_list_ptr->block2 != COAP_OPTION_BLOCK_NONE) {
                    tr_error("sn_coap_parser_options_parse - COAP_OPTION_BLOCK2 not valid!");
                    return NULL;
                }
                dst_coap_msg_ptr->options_list_ptr->block2 = sn_coap_parser_options_parse_uint(&packet_data_ptr, option_len);
                break;

            case COAP_OPTION_BLOCK1:
                if ((option_len > 3) || dst_coap_msg_ptr->options_list_ptr->block1 != COAP_OPTION_BLOCK_NONE) {
                    tr_error("sn_coap_parser_options_parse - COAP_OPTION_BLOCK1 not valid!");
                    return NULL;
                }
                dst_coap_msg_ptr->options_list_ptr->block1 = sn_coap_parser_options_parse_uint(&packet_data_ptr, option_len);
                break;

            case COAP_OPTION_ACCEPT:
                if ((option_len > 2) || (dst_coap_msg_ptr->options_list_ptr->accept != COAP_CT_NONE)) {
                    tr_error("sn_coap_parser_options_parse - COAP_OPTION_ACCEPT not valid!");
                    return NULL;
                }
                dst_coap_msg_ptr->options_list_ptr->accept = (sn_coap_content_format_e) sn_coap_parser_options_parse_uint(&packet_data_ptr, option_len);
                break;

            case COAP_OPTION_SIZE1:
                if ((option_len > 4) || dst_coap_msg_ptr->options_list_ptr->use_size1) {
                    tr_error("sn_coap_parser_options_parse - COAP_OPTION_SIZE1 not valid!");
                    return NULL;
                }
                dst_coap_msg_ptr->options_list_ptr->use_size1 = true;
                dst_coap_msg_ptr->options_list_ptr->size1 = sn_coap_parser_options_parse_uint(&packet_data_ptr, option_len);
                break;

            case COAP_OPTION_SIZE2:
                if ((option_len > 4) || dst_coap_msg_ptr->options_list_ptr->use_size2) {
                    tr_error("sn_coap_parser_options_parse - COAP_OPTION_SIZE2 not valid!");
                    return NULL;
                }
                dst_coap_msg_ptr->options_list_ptr->use_size2 = true;
                dst_coap_msg_ptr->options_list_ptr->size2 = sn_coap_parser_options_parse_uint(&packet_data_ptr, option_len);
                break;

            default:
                tr_error("sn_coap_parser_options_parse - unknown option!");
                return NULL;
        }

        /* Check for overflow */
        if ((packet_data_ptr - packet_data_start_ptr) > packet_len) {
            return NULL;
        }
        message_left = packet_len - (packet_data_ptr - packet_data_start_ptr);
    }
    return packet_data_ptr;
}


/**
 * \fn static int8_t sn_coap_parser_options_parse_multiple_options(uint8_t *packet_data_pptr, uint8_t options_count_left, uint8_t *previous_option_number_ptr, uint8_t **dst_pptr,
 *                                                                  uint16_t *dst_len_ptr, sn_coap_option_numbers_e option, uint16_t option_number_len)
 *
 * \brief Parses CoAP message's Uri-query options
 *
 * \param *packet_data_ptr is source for Packet data to be parsed to CoAP message
 *
 * \param *dst_coap_msg_ptr is destination for parsed CoAP message
 *
 * \param options_count_left tells how many options are unhandled in Packet data
 *
 * \param *previous_option_number_ptr is pointer to used and returned previous Option number
 *
 * \return Return value is advanced input pointer. In failure case NULL is returned.
*/
static const uint8_t *sn_coap_parser_options_parse_multiple_options(const uint8_t * restrict packet_data_ptr, struct coap_s * restrict handle, uint_fast16_t packet_left_len,  uint8_t ** restrict dst_pptr, uint16_t * restrict dst_len_ptr, sn_coap_option_numbers_e option, uint_fast16_t option_number_len)
{

    int           uri_query_needed_heap       = sn_coap_parser_options_count_needed_memory_multiple_option(packet_data_ptr, packet_left_len, option, option_number_len);
    uint8_t       * restrict temp_parsed_uri_query_ptr   = NULL;
    const uint8_t *start_ptr = packet_data_ptr;
    uint_fast16_t  message_left = packet_left_len;
    bool           first_option = true;

    if (uri_query_needed_heap == -1) {
        return NULL;
    }

    if (uri_query_needed_heap) {
        *dst_pptr = handle->sn_coap_protocol_malloc(uri_query_needed_heap);

        if (*dst_pptr == NULL) {
            tr_error("sn_coap_parser_options_parse_multiple_options - failed to allocate options!");
            return NULL;
        }
    }

    *dst_len_ptr = uri_query_needed_heap;
    temp_parsed_uri_query_ptr = *dst_pptr;

    /* Loop all Uri-Query options */
    while ((temp_parsed_uri_query_ptr - *dst_pptr) < uri_query_needed_heap && message_left) {
        /* Check if this is first Uri-Query option */
        if (!first_option) {
            /* Uri-Query is modified to following format: temp1'\0'temp2'\0'temp3 i.e.  */
            /* Uri-Path is modified to following format: temp1\temp2\temp3 i.e.  */
            if (option == COAP_OPTION_URI_QUERY || option == COAP_OPTION_LOCATION_QUERY || option == COAP_OPTION_ETAG || option == COAP_OPTION_ACCEPT) {
                *temp_parsed_uri_query_ptr = '&';
            } else if (option == COAP_OPTION_URI_PATH || option == COAP_OPTION_LOCATION_PATH) {
                *temp_parsed_uri_query_ptr = '/';
            }

            temp_parsed_uri_query_ptr++;
        } else {
            first_option = false;
        }

        if (((temp_parsed_uri_query_ptr - *dst_pptr) + option_number_len) > uri_query_needed_heap) {
            return NULL;
        }
        if (0 != sn_coap_parser_check_packet_ptr(packet_data_ptr, start_ptr, packet_left_len, option_number_len)) {
            /* Buffer read overflow. */
            return NULL;
        }

        /* Copy the option value to URI query buffer */
        memcpy(temp_parsed_uri_query_ptr, packet_data_ptr, option_number_len);

        message_left = sn_coap_parser_move_packet_ptr(&packet_data_ptr, start_ptr, packet_left_len, option_number_len);
        temp_parsed_uri_query_ptr += option_number_len;

        /* Check if there is more input to process */
        if (message_left == 0 || ((*packet_data_ptr >> COAP_OPTIONS_OPTION_NUMBER_SHIFT) != 0)) {
            return packet_data_ptr;
        }

        /* Process next option */
        option_number_len = (*packet_data_ptr & 0x0F);
        message_left = sn_coap_parser_move_packet_ptr(&packet_data_ptr, start_ptr, packet_left_len, 1);

        /* Add possible option length extension to resolve full length of the option */
        int_fast8_t option_parse_result = parse_ext_option(&option_number_len, &packet_data_ptr, start_ptr, packet_left_len, &message_left);
        if (option_parse_result != 0) {
            /* Extended option parsing failed. */
            return NULL;
        }
    }

    return packet_data_ptr;
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
static int sn_coap_parser_options_count_needed_memory_multiple_option(const uint8_t * restrict packet_data_ptr, uint_fast16_t packet_left_len, sn_coap_option_numbers_e option, uint_fast16_t option_number_len)
{
    int      ret_value              = 0;
    uint_fast16_t message_left      = packet_left_len;
    const uint8_t *start_ptr        = packet_data_ptr;

    /* Loop all Uri-Query options */
    while (message_left > 0) {
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

        /* Check if the value length is within buffer limits */
        int_fast8_t ptr_check_result = sn_coap_parser_check_packet_ptr(packet_data_ptr, start_ptr, packet_left_len, option_number_len);
        if (ptr_check_result != 0) {
            return -1;
        }

        ret_value += option_number_len + 1; /* + 1 is for separator */

        /* Skip the option value */
        message_left = sn_coap_parser_move_packet_ptr(&packet_data_ptr, start_ptr, packet_left_len, option_number_len);
        if (message_left == 0) {
            break;
        }

        /* Read the option delta */
        if (((*packet_data_ptr) >> COAP_OPTIONS_OPTION_NUMBER_SHIFT) != 0) {
            break;
        }

        /* Read the option length without extensions */
        option_number_len = (*packet_data_ptr & 0x0F);

        /* Skip the option byte */
        message_left = sn_coap_parser_move_packet_ptr(&packet_data_ptr, start_ptr, packet_left_len, 1);

        /* Add possible option length extension to resolve full length of the option */
        int_fast8_t option_parse_result = parse_ext_option(&option_number_len, &packet_data_ptr, start_ptr, packet_left_len, &message_left);
        if (option_parse_result != 0) {
            return -1;
        }
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
static const uint8_t *sn_coap_parser_payload_parse(const uint8_t * restrict packet_data_ptr, uint16_t packet_data_len, uint8_t *packet_data_start_ptr, sn_coap_hdr_s * restrict dst_coap_msg_ptr)
{
    /* If there is payload */
    if ((packet_data_ptr - packet_data_start_ptr) < packet_data_len) {
        if (*packet_data_ptr == 0xff) {
            packet_data_ptr++;
            /* Parse Payload length */
            dst_coap_msg_ptr->payload_len = packet_data_len - (packet_data_ptr - packet_data_start_ptr);

            /* The presence of a marker followed by a zero-length payload MUST be processed as a message format error */
            if (dst_coap_msg_ptr->payload_len == 0) {
                return NULL;
            }

            /* Parse Payload by setting CoAP message's payload_ptr to point Payload in Packet data */
            dst_coap_msg_ptr->payload_ptr = (uint8_t *) packet_data_ptr;
        }
        /* No payload marker.. */
        else {
            tr_error("sn_coap_parser_payload_parse - payload marker not found!");
            return NULL;
        }
    }
    return packet_data_ptr;
}

