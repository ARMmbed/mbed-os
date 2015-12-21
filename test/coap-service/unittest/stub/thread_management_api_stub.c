/*
 * Copyright (c) 2015 ARM Limited. All rights reserved.
 */

#include <inttypes.h>
#include "thread_management_api.h"
#include "ns_types.h"
#include "sn_coap_header.h"

int thread_management_recv_set_response_cb(int8_t service_id, uint16_t msg_id, sn_coap_hdr_s *response_ptr)
{
    return 0;
}

int thread_management_recv_get_response_cb(int8_t service_id, uint16_t msg_id, sn_coap_hdr_s *response_ptr)
{
    return 0;
}

int thread_management_register(int8_t interface_id)
{
    return 0;
}

int thread_management_unregister(int8_t interface_id)
{
    return 0;
}

int thread_management_set_security_policy(int8_t interface_id, uint8_t options, uint16_t rotation_time, management_set_response_cb *cb_ptr)
{
    return 0;
}

int thread_management_set_steering_data(int8_t interface_id, uint8_t *steering_data_ptr, uint8_t steering_data_len, management_set_response_cb *cb_ptr)
{
    return 0;
}

int thread_management_set_commissioning_data_timestamp(int8_t interface_id, uint64_t time, management_set_response_cb *cb_ptr)
{
    return 0;
}

int thread_management_get(int8_t instance_id, uint8_t *fields_ptr, uint8_t fields_count, uint8_t dst_addr[static 16], management_get_response_cb *cb_ptr)
{
    return 0;
}
