/*
 * Copyright (c) 2015, Arm Limited and affiliates.
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

TEST_GROUP(6lowpan_mesh)
{
    test_6lowpan_mesh *unit = NULL;

    void setup() {
        unit = new test_6lowpan_mesh();
    }

    void teardown() {
        delete unit;
    }
};

TEST(6lowpan_mesh, Create)
{
    CHECK(unit != NULL);
}

TEST(6lowpan_mesh, test_mesh_supports_multicast)
{
    unit->test_mesh_supports_multicast();
}

TEST(6lowpan_mesh, test_mesh_short_address_is_multicast)
{
    unit->test_mesh_short_address_is_multicast();
}

TEST(6lowpan_mesh, test_mesh_short_address_is_broadcast_or_multicast)
{
    unit->test_mesh_short_address_is_broadcast_or_multicast();
}

TEST(6lowpan_mesh, test_mesh_up)
{
    unit->test_mesh_up();
}

TEST(6lowpan_mesh, test_mesh_forwardable_address)
{
    unit->test_mesh_forwardable_address();
}

TEST(6lowpan_mesh, test_mesh_address_map)
{
    unit->test_mesh_address_map();
}

TEST(6lowpan_mesh, test_mesh_header_needed)
{
    unit->test_mesh_header_needed();
}

TEST(6lowpan_mesh, test_mesh_header_size)
{
    unit->test_mesh_header_size();
}

TEST(6lowpan_mesh, test_mesh_header_len_from_type_byte)
{
    unit->test_mesh_header_len_from_type_byte();
}

TEST(6lowpan_mesh, test_mesh_header_len_from_buffer_type_byte)
{
    unit->test_mesh_header_len_from_buffer_type_byte();
}

TEST(6lowpan_mesh, test_mesh_rewrite_bc0_header)
{
    unit->test_mesh_rewrite_bc0_header();
}

TEST(6lowpan_mesh, test_mesh_down)
{
    unit->test_mesh_down();
}
