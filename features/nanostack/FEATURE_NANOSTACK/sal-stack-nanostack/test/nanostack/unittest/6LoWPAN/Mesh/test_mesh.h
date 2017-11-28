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
#ifndef TEST_6LOWPAN_MESH_H
#define TEST_6LOWPAN_MESH_H

#include "nsconfig.h"
#include <string.h>
#include <ns_list.h>
#include "common_functions.h"
#include "NWK_INTERFACE/Include/protocol.h"
#include "NWK_INTERFACE/Include/protocol_stats.h"
#include "6LoWPAN/IPHC_Decode/cipv6.h"
#include "mesh.h"


class test_6lowpan_mesh
{
public:
    test_6lowpan_mesh();
    virtual ~test_6lowpan_mesh();
    void test_mesh_supports_multicast();
    void test_mesh_short_address_is_multicast();
    void test_mesh_short_address_is_broadcast_or_multicast();
    void test_mesh_up();
    void test_mesh_forwardable_address();
    void test_mesh_address_map();
    void test_mesh_header_needed();
    void test_mesh_header_size();
    void test_mesh_header_len_from_type_byte();
    void test_mesh_header_len_from_buffer_type_byte();
    void test_mesh_rewrite_bc0_header();
    void test_mesh_down();
};

#endif
