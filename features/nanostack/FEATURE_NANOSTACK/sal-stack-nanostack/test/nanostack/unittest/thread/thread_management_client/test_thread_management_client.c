/*
 * Copyright (c) 2015-2016, Arm Limited and affiliates.
 * SPDX-License-Identifier: Apache-2.0
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
#include "test_thread_management_client.h"
#include <string.h>
/*typedef struct thread_management {
	thread_management_client_router_id_cb *router_id_cb_ptr;
    thread_management_client_network_data_set_cb *network_data_set_cb_ptr;
    int8_t interface_id;
    int8_t coap_service_id;
    ns_list_link_t link;
} thread_management_t;*/
bool test_thread_management_client_init()
{
    thread_management_client_init(0);

    thread_management_client_delete(0);

    return true;
}

/*bool test_thread_management_client_router_id_get(){
	int8_t interface_id = 0;
	uint8_t mac[8] = {0x00};
	uint16_t router_id = 0x1234;
	thread_management_client_router_id_cb *id_cb;
	thread_management_client_router_id_get(interface_id, &mac[0], router_id, &id_cb);
}*/