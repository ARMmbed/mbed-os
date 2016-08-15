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
#include "nsdlaccesshelper_stub.h"

bool nsdlaccesshelper_stub::bool_value;
void* nsdlaccesshelper_stub::void_value;
uint8_t nsdlaccesshelper_stub::int_value;

M2MNsdlInterfaceList __nsdl_interface_list;
M2MConnectionHandler *__connection_handler = NULL;
#ifdef USE_LINUX
M2MTimerImpl  *__timer_impl = NULL;
M2MConnectionHandler *__connection_impl = NULL;
#endif
void nsdlaccesshelper_stub::clear()
{
    bool_value = false;
    void_value = NULL;
    int_value = 0;
}

uint8_t __nsdl_c_callback(struct nsdl_s * ,
                          sn_coap_hdr_s *,
                          sn_nsdl_addr_s *,
                          sn_nsdl_capab_e )
{
    return nsdlaccesshelper_stub::int_value;
}

void *__nsdl_c_memory_alloc(uint16_t)
{
    return nsdlaccesshelper_stub::void_value;
}

void __nsdl_c_memory_free(void *)
{
}

uint8_t __nsdl_c_send_to_server(struct nsdl_s * ,
                                sn_nsdl_capab_e ,
                                uint8_t *,
                                uint16_t ,
                                sn_nsdl_addr_s *)
{
    return nsdlaccesshelper_stub::int_value;
}

uint8_t __nsdl_c_received_from_server(struct nsdl_s * ,
                                      sn_coap_hdr_s *,
                                      sn_nsdl_addr_s *)
{
    return nsdlaccesshelper_stub::int_value;
}

void *__socket_malloc( void *, size_t)
{
return nsdlaccesshelper_stub::void_value;
}

void __socket_free(void *, void *)
{
}

void __mutex_claim()
{
}

void __mutex_release()
{
}
