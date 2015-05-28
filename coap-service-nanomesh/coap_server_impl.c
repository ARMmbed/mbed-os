#include <stdlib.h>
#include "ns_types.h"
#include "eventOS_event.h"
#include "eventOS_scheduler.h"
#include "eventOS_event_timer.h"
#include "nsdynmemLIB.h"
#include "ns_list.h"
#include "coap_server_impl.h"

#define COAP_SERVER_SERVICE_TASKLET_INIT 		1
#define COAP_SERVER_SERVICE_TIMER 	   		    2
#define COAP_SERVER_SERVICE_TIMER_ID			1
#define COAP_TIMER_UPDATE_PERIOD_IN_SECONDS	    10

static int8_t coap_service_tasklet = -1;

void coap_server_service_tasklet(arm_event_s * event);

int8_t coap_server_start(void)
{
    int8_t ret_val = -1;

    ret_val = coap_server_service_tasklet_generated();

	return ret_val;
}


static int8_t coap_server_service_tasklet_generated(void)
{
	if(coap_service_tasklet == -1)
	{
		//coap_service_tasklet = eventOS_event_handler_create(&coap_server_service_tasklet,COAP_SERVER_SERVICE_TASKLET_INIT);
	}

	return coap_service_tasklet;
}

void coap_server_service_tasklet(arm_event_s * event)
{

	if(event->event_type == COAP_SERVER_SERVICE_TASKLET_INIT)
	{
		//We should define periodically timer service!!
		//eventOS_event_timer_request(COAP_SERVER_SERVICE_TIMER_ID, COAP_SERVER_SERVICE_TIMER,coap_service_tasklet, (COAP_TIMER_UPDATE_PERIOD_IN_SECONDS *1000));
	}
	else if(event->event_type == COAP_SERVER_SERVICE_TIMER)
	{
		//eventOS_event_timer_request(COAP_SERVER_SERVICE_TIMER_ID, COAP_SERVER_SERVICE_TIMER,coap_service_tasklet, (COAP_TIMER_UPDATE_PERIOD_IN_SECONDS *1000));
	}

}

uint16_t socket_open(uint16_t listen_port, coap_service_request_recv_cb *requst_recv_cb)
{

	return 0;
}

void *memory_allocation(uint16_t size)
{


	if(size)
		return malloc(size);
	else
		return 0;
}

void memory_free(void* ptr)
{

	if(ptr)
		free(ptr);

}
