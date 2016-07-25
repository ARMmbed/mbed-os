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
#ifndef M2M_RESOURCE_STUB_H
#define M2M_RESOURCE_STUB_H

#include "m2mresource.h"

//some internal test related stuff
namespace m2mresource_stub
{
    extern uint32_t int_value;    
    extern uint8_t* delayed_token;
    extern uint8_t delayed_token_len;
    extern bool bool_value;
    extern M2MResourceInstanceList list;
    extern M2MResourceInstance *instance;
    void clear();
    extern sn_coap_hdr_s *header;
}

#endif // M2M_RESOURCE_STUB_H

