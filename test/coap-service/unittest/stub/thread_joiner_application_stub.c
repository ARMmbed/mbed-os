/*
 * Copyright (c) 2014-2015 ARM Limited. All rights reserved.
 */


#include <ns_types.h>
#include "ns_list.h"
#include "ns_trace.h"
#include "nsdynmemLIB.h"
#include "common_functions.h"
#include "thread_tmfcop_lib.h"
#include "shalib.h"

#include "thread_management_if.h"
#include "thread_config.h"
#include "thread_meshcop_lib.h"
#include "thread_commissioning_if.h"
#include "thread_joiner_application.h"

#define TRACE_GROUP TRACE_GROUP_THREAD_JOINER_APPLICATION

link_configuration_s *link_configuration_create(void)
{
    return NULL;
}

void link_configuration_delete(link_configuration_s *this)
{
}

void link_configuration_copy(link_configuration_s *this, link_configuration_s *configuration_ptr)
{
}

int thread_joiner_application_init(int8_t interface_id, device_configuration_s *device_configuration_ptr, struct link_configuration *static_configuration_ptr)
{
    return 0;
}

void thread_joiner_application_deinit(int8_t interface_id)
{
}

link_configuration_s *thread_joiner_application_get_config(int8_t interface_id)
{
    return NULL;
}

int joiner_application_security_done_cb(int8_t service_id, uint8_t IID[8], uint8_t keyblock[40])
{
    return 0;
}

int thread_joiner_application_finalisation_cb(int8_t service_id, uint16_t msg_id, sn_coap_hdr_s *response_ptr)
{
    return 0;
}

int thread_joiner_application_commission_start(int8_t interface_id, uint8_t parent_address[16], uint16_t port, uint16_t panid, uint8_t xpanid[8], uint8_t channel, thread_joiner_application_commission_done_cb *done_cb)
{
    return 0;
}

