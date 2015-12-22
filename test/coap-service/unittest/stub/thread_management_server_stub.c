/*
 * Copyright (c) 2014-2015 ARM Limited. All rights reserved.
 */


#include "config.h"
#include <string.h>
#include <ns_types.h>
#include <ns_list.h>
#include <ns_trace.h>
#include "nsdynmemLIB.h"
#include "common_functions.h"
#include "6LoWPAN/Thread/thread_management_server.h"

#define TRACE_GROUP "TMFs"

#include "net_interface.h"
#include "socket_api.h"
#include "thread_config.h"
#include "thread_tmfcop_lib.h"
#include "thread_meshcop_lib.h"
#include "thread_management_if.h"
#include "thread_commissioning_if.h"
//#include "thread_tmf_data_if.h"

int thread_management_server_init(int8_t interface_id)
{
    return 0;
}

void thread_management_server_delete(int8_t interface_id)
{
}

int thread_management_server_leader_init(int8_t interface_id, thread_management_server_rid_assign_cb *assign_cb, thread_management_server_rid_release_cb *release_cb)
{
    return 0;
}

void thread_management_server_leader_deinit(int8_t interface_id)
{
}

void joiner_router_recv_commission_msg(void *cb_res)
{
}

int thread_management_server_joiner_router_init(int8_t interface_id, uint16_t *joiner_port)
{
    return 0;
}

void thread_management_server_joiner_router_deinit(int8_t interface_id)
{
}
