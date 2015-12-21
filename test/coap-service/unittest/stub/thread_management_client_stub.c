/*
 * Copyright (c) 2014-2015 ARM Limited. All rights reserved.
 */


#include <ns_types.h>
#include "ns_list.h"
#include "ns_trace.h"
#include "nsdynmemLIB.h"
#include "thread_tmfcop_lib.h"

#include "thread_config.h"
#include "thread_management_if.h"
#include "6LoWPAN/Thread/thread_management_client.h"

#define TRACE_GROUP TRACE_GROUP_THREAD_MANAGEMENT_CLIENT

typedef struct thread_management {
    thread_management_client_router_id_cb *router_id_cb_ptr;
    thread_management_client_network_data_set_cb *network_data_set_cb_ptr;
    int8_t interface_id;
    int8_t coap_service_id;
    ns_list_link_t link;
} thread_management_t;


void thread_management_client_init(int8_t interface_id)
{
}

void thread_management_client_delete(int8_t interface_id)
{
}

int thread_management_client_router_id_get(int8_t interface_id, uint8_t mac[8], uint16_t router_id, thread_management_client_router_id_cb *id_cb)
{
    return 0;
}

int thread_management_client_router_id_release(int8_t interface_id, uint8_t mac[8], uint16_t router_id, thread_management_client_router_id_cb *id_cb)
{
    return 0;
}

int thread_management_client_network_data_register(int8_t interface_id, uint8_t *data_ptr, uint16_t data_len, thread_management_client_network_data_set_cb *set_cb)
{
    return 0;
}

int8_t thread_management_query_network_parameters(int8_t interface_id, uint8_t *dst_address)
{
    return 0;
}

int8_t thread_management_send_set_network_parameters(int8_t interface_id)
{
    return 0;
}

