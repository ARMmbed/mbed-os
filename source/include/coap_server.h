/*
 * Copyright (c) 2015 ARM Limited. All rights reserved.

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

/*
 * \file -
 * \brief Add short description about this file!!!
 *
 */

#ifndef COAP_SERVER_H_
#define COAP_SERVER_H_

/* Brief explain.
 *
 * Some explain
 *
 * /param param_name   explain.
 *
 * /return TODO
 */

#include "ns_list.h"

typedef struct
{
    uint8_t remote_address[16];
//    management_set_response_cb *set_response_cb_ptr;
//    management_get_response_cb *get_response_cb_ptr;
    int8_t interface_id;
    int8_t coap_service_id;
    ns_list_link_t link;
} coap_service_session_t;

void coap_server_delete_session(int8_t service_id);

int8_t coap_server_service_init(int8_t interface_id, uint16_t listen_port, uint8_t service_options);

static coap_service_session_t *coap_service_search_from_list(int8_t interface_id);

static coap_service_session_t *coap_service_find_by_service(int8_t service_id);


#endif /* COAP_SERVER_H_ */
