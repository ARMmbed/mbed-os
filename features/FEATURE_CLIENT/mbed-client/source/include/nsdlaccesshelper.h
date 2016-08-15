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
#ifndef NSDL_ACCESS_HELPER_H
#define NSDL_ACCESS_HELPER_H
#include "mbed-client/m2mconnectionhandler.h"
#include "include/m2mnsdlinterface.h"

typedef Vector<M2MNsdlInterface  *> M2MNsdlInterfaceList;
extern M2MNsdlInterfaceList __nsdl_interface_list;
extern M2MConnectionHandler *__connection_handler;

#ifdef __cplusplus
extern "C" {
#endif

uint8_t __nsdl_c_callback(struct nsdl_s * nsdl_handle,
                          sn_coap_hdr_s *received_coap_ptr,
                          sn_nsdl_addr_s *address,
                          sn_nsdl_capab_e nsdl_capab);
void *__nsdl_c_memory_alloc(uint16_t size);
void __nsdl_c_memory_free(void *ptr);
uint8_t __nsdl_c_send_to_server(struct nsdl_s * nsdl_handle,
                                sn_nsdl_capab_e protocol,
                                uint8_t *data_ptr,
                                uint16_t data_len,
                                sn_nsdl_addr_s *address_ptr);
uint8_t __nsdl_c_received_from_server(struct nsdl_s * nsdl_handle,
                                      sn_coap_hdr_s *coap_header,
                                      sn_nsdl_addr_s *address_ptr);

void *__socket_malloc( void * context, size_t size);
void __socket_free(void * context, void * ptr);

M2MNsdlInterface* get_interface(struct nsdl_s* nsdl_handle);

void __mutex_claim();
void __mutex_release();

#ifdef __cplusplus
}
#endif

#endif // NSDL_ACCESS_HELPER_H
