/*
 * Copyright (c) 2015 ARM. All rights reserved.
 */

/*
 * \file -
 * \brief Platform specifig implementation file
 * 
 */

#ifndef COAP_SERVER_IMPL_H_
#define COAP_SERVER_IMPL_H_

#include "ns_types.h"
#include "eventOS_event.h"
#include "coap_service_api.h"

int8_t coap_server_start(void);

void *memory_allocation(uint16_t size);

void memory_free(void* ptr);

static int8_t coap_server_service_tasklet_generated(void);

void coap_server_service_tasklet(arm_event_s * event);

uint16_t socket_open(uint16_t listen_port, coap_service_request_recv_cb *requst_recv_cb);

#endif /* COAP_SERVER_IMPL_H_ */
