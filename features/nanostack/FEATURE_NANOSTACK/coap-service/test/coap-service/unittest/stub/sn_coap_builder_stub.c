/*
 * Copyright (c) 2011-2017 ARM Limited. All Rights Reserved.
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

#include "ns_types.h"
#include "sn_coap_header.h"
#include "sn_coap_builder_stub.h"

sn_coap_builder_stub_def sn_coap_builder_stub;

sn_coap_hdr_s *sn_coap_build_response(struct coap_s *handle, sn_coap_hdr_s *coap_packet_ptr, uint8_t msg_code)
{
    return sn_coap_builder_stub.expectedHeader;
}

int16_t sn_coap_builder(uint8_t *dst_packet_data_ptr, sn_coap_hdr_s *src_coap_msg_ptr)
{
    return sn_coap_builder_stub.expectedInt16;
}

uint16_t sn_coap_builder_calc_needed_packet_data_size(sn_coap_hdr_s *src_coap_msg_ptr)
{
    return sn_coap_builder_stub.expectedUint16;
}

int16_t sn_coap_builder_options_build_add_zero_length_option(uint8_t **dst_packet_data_pptr, uint8_t option_length, uint8_t option_exist, sn_coap_option_numbers_e option_number)
{
    return sn_coap_builder_stub.expectedInt16;
}

