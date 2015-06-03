/*
 * Copyright (c) 2015 ARM. All rights reserved.
 */

//#include "include/coap_service_api.h"
#include "ns_types.h"
#include "ns_list.h"
#include "coap_server_impl.h"
#include "./include/coap_server.h"

//typedef NS_LIST_HEAD(coap_service_session_t, link) instance_list_t;
//instance_list_t NS_LIST_NAME_INIT(instance_list);

//typedef struct sn_coap_hdr_s sn_coap_hdr_s;

/* Coap service class handlers*/

void coap_server_delete_session(int8_t service_id)
{

	coap_service_session_t *this = coap_service_find_by_service(service_id);

	if(this)
    {

        //ns_list_remove(&instance_list,this);
        memory_free(this);

    }

}

static coap_service_session_t *coap_service_search_from_list(int8_t interface_id)
{
	coap_service_session_t *this = NULL;

	/*
	ns_list_foreach(coap_service_session_t,cur_ptr, &instance_list)
    {
        if(cur_ptr->interface_id == interface_id)
        {
            this = cur_ptr;
            break;
        }
    }
	*/

    return this;
}


static coap_service_session_t *coap_service_find_by_service(int8_t service_id)
{
	coap_service_session_t *this = NULL;

	/*
	ns_list_foreach(coap_service_session_t,cur_ptr, &instance_list)
    {
        if(cur_ptr->coap_service_id == service_id)
        {
            this = cur_ptr;
            break;
        }
    }
	*/

    return this;
}


/* Initialize CoAP server.
 *
 * TODO
 *
 * /param interface interface id of this instance.
 *
 */
int8_t coap_server_service_init(int8_t interface_id, uint16_t listen_port, uint8_t service_options)
{
	int retVal = -1;
	int8_t socketInstance;

	// Initialize linked list
	//ns_list_init_(instance_list);

	//allocate Socket Service
	socketInstance  = coap_server_start();

	if(socketInstance < 0)
		retVal = -2;
	else
	{

		coap_service_session_t *this = memory_allocation(sizeof(coap_service_session_t));
	    if(this)
	    {
	        this->interface_id = interface_id;
//	        this->set_response_cb_ptr = NULL;
	        //ns_list_add_to_start(&instance_list, this);
	        retVal = 1;
	    }
	    return retVal;

			//allocate server

	}
	if(socketInstance > 0)
	{
		coap_service_delete(socketInstance);
	}
	return retVal;
}


