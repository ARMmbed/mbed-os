/*
 * Copyright (c) 2011-2017 ARM Limited. All Rights Reserved.
 */


#include <stdio.h>
#include <stdlib.h> /* For libary malloc() */
#include <string.h> /* For memset() and memcpy() */
#ifdef __linux__
#include <time.h>
#endif

#include "ns_types.h"
#include "sn_coap_protocol.h"
#include "sn_coap_header_internal.h"
#include "sn_coap_protocol_internal.h"
#include "sn_coap_protocol_stub.h"

uint16_t                sn_coap_block_data_size                             = 0;

uint8_t                 sn_coap_resending_queue_msgs                        = 0;
uint8_t                 sn_coap_resending_queue_bytes                       = 0;
uint8_t                 sn_coap_resending_count                             = 0;
uint8_t                 sn_coap_resending_intervall                         = 0;

uint8_t                 sn_coap_duplication_buffer_size                     = 0;

sn_coap_protocol_stub_def sn_coap_protocol_stub;

int8_t sn_coap_protocol_destroy(struct coap_s *handle)
{
    return sn_coap_protocol_stub.expectedInt8;
}

struct coap_s *sn_coap_protocol_init(void *(*used_malloc_func_ptr)(uint16_t), void (*used_free_func_ptr)(void *),
                                     uint8_t (*used_tx_callback_ptr)(uint8_t *, uint16_t, sn_nsdl_addr_s *, void *),
                                     int8_t (*used_rx_callback_ptr)(sn_coap_hdr_s *, sn_nsdl_addr_s *, void *param))
{
    if( sn_coap_protocol_stub.expectedCoap ){
        sn_coap_protocol_stub.expectedCoap->sn_coap_protocol_free = used_free_func_ptr;
        sn_coap_protocol_stub.expectedCoap->sn_coap_protocol_malloc = used_malloc_func_ptr;
        sn_coap_protocol_stub.expectedCoap->sn_coap_rx_callback = used_rx_callback_ptr;
        sn_coap_protocol_stub.expectedCoap->sn_coap_tx_callback = used_tx_callback_ptr;
    }
    return sn_coap_protocol_stub.expectedCoap;
}

int8_t sn_coap_protocol_set_block_size(struct coap_s *handle, uint16_t block_size)
{
    return sn_coap_protocol_stub.expectedInt8;
}

int8_t sn_coap_protocol_set_duplicate_buffer_size(struct coap_s *handle, uint8_t size)
{
    return sn_coap_protocol_stub.expectedInt8;
}

int8_t sn_coap_protocol_set_retransmission_parameters(struct coap_s *handle, uint8_t resending_count, uint8_t resending_intervall)
{
    return sn_coap_protocol_stub.expectedInt8;
}

int8_t sn_coap_protocol_set_retransmission_buffer(struct coap_s *handle, uint8_t buffer_size_messages, uint16_t buffer_size_bytes)
{
    return sn_coap_protocol_stub.expectedInt8;
}

void sn_coap_protocol_clear_retransmission_buffer(struct coap_s *handle)
{
}


int16_t sn_coap_protocol_build(struct coap_s *handle, sn_nsdl_addr_s *dst_addr_ptr,
                               uint8_t *dst_packet_data_ptr, sn_coap_hdr_s *src_coap_msg_ptr, void *param)
{
    //TODO: check if needed here
    src_coap_msg_ptr->msg_id = 2;
    return sn_coap_protocol_stub.expectedInt16;
}

sn_coap_hdr_s *sn_coap_protocol_parse(struct coap_s *handle, sn_nsdl_addr_s *src_addr_ptr, uint16_t packet_data_len, uint8_t *packet_data_ptr, void *param)
{
    return sn_coap_protocol_stub.expectedHeader;
}

int8_t sn_coap_protocol_exec(struct coap_s *handle, uint32_t current_time)
{
    return sn_coap_protocol_stub.expectedInt8;
}

coap_send_msg_s *sn_coap_protocol_allocate_mem_for_msg(struct coap_s *handle, sn_nsdl_addr_s *dst_addr_ptr, uint16_t packet_data_len)
{
    return sn_coap_protocol_stub.expectedSendMsg;
}

int8_t sn_coap_protocol_delete_retransmission(struct coap_s *handle, uint16_t msg_id)
{
    return 0;
}
