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
#include "CppUTest/TestHarness.h"
#include "test_mesh.h"
#include <string.h>
#include <iostream>
#include "Core/include/ns_buffer.h"

test_6lowpan_mesh::test_6lowpan_mesh()
{
}

test_6lowpan_mesh::~test_6lowpan_mesh()
{
}

void test_6lowpan_mesh::test_mesh_supports_multicast()
{
    bool supports = false;
    mesh_all_addresses_unicast(supports);
    CHECK(mesh_supports_multicast() != supports);
    supports = true;
    mesh_all_addresses_unicast(supports);
    CHECK(mesh_supports_multicast() != supports);
}

void test_6lowpan_mesh::test_mesh_short_address_is_multicast()
{
    mesh_all_addresses_unicast(true);
    //CHECK(mesh_short_address_is_multicast(0)==false);
}

void test_6lowpan_mesh::test_mesh_short_address_is_broadcast_or_multicast()
{
    mesh_all_addresses_unicast(true);
    //CHECK(mesh_short_address_is_broadcast_or_multicast(0)==false);
}

void test_6lowpan_mesh::test_mesh_up()
{
    buffer_t buf = {};
    CHECK(mesh_up(&buf) == NULL);
}

void test_6lowpan_mesh::test_mesh_forwardable_address()
{
    protocol_interface_info_entry_t et = {};
    addrtype_t addr;
    uint8_t some = 2;
    CHECK(mesh_forwardable_address(&et, addr, &some) == false);
}

void test_6lowpan_mesh::test_mesh_address_map()
{
    protocol_interface_info_entry_t et = {};
    addrtype_t addr;
    uint8_t some = 2;
    CHECK(mesh_address_map(&et, &addr, &some) == true);
}

void test_6lowpan_mesh::test_mesh_header_needed()
{
    buffer_t buf = {};
    CHECK(mesh_header_needed(&buf) == false);
}

void test_6lowpan_mesh::test_mesh_header_size()
{
    buffer_t buf = {};
    CHECK(mesh_header_size(&buf) == 0);
}

void test_6lowpan_mesh::test_mesh_header_len_from_type_byte()
{
    CHECK(mesh_header_len_from_type_byte(0) == 0);
}

void test_6lowpan_mesh::test_mesh_header_len_from_buffer_type_byte()
{
    buffer_t buf = {};
    CHECK(mesh_header_len_from_buffer_type_byte(&buf) == 0);
}

void test_6lowpan_mesh::test_mesh_rewrite_bc0_header()
{
    uint8_t val = 0;
    mesh_rewrite_bc0_header(&val);
}

void test_6lowpan_mesh::test_mesh_down()
{
    buffer_t buf = {};
    CHECK(mesh_down(&buf) == NULL);
}
