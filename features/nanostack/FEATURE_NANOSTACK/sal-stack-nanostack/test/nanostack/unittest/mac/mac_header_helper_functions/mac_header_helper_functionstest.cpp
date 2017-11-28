/*
 * Copyright (c) 2016, Arm Limited and affiliates.
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
#include "test_mac_header_helper_functions.h"

TEST_GROUP(mac_header_helper_functions)
{
    void setup()
    {
    }

    void teardown()
    {
    }
};

TEST(mac_header_helper_functions, test_mac_security_mic_length_get)
{
    CHECK(test_mac_security_mic_length_get());
}

TEST(mac_header_helper_functions, test_mac_header_security_aux_header_length)
{
    CHECK(test_mac_header_security_aux_header_length());
}

TEST(mac_header_helper_functions, test_mac_header_address_length)
{
    CHECK(test_mac_header_address_length());
}

TEST(mac_header_helper_functions, test_mac_header_security_parameter_set)
{
    CHECK(test_mac_header_security_parameter_set());
}

TEST(mac_header_helper_functions, test_mac_header_parse_fcf_dsn)
{
    CHECK(test_mac_header_parse_fcf_dsn());
}

TEST(mac_header_helper_functions, test_mac_header_security_components_read)
{
    CHECK(test_mac_header_security_components_read());
}

TEST(mac_header_helper_functions, test_mac_header_get_src_panid)
{
    CHECK(test_mac_header_get_src_panid());
}

TEST(mac_header_helper_functions, test_mac_header_get_dst_panid)
{
    CHECK(test_mac_header_get_dst_panid());
}

TEST(mac_header_helper_functions, test_mac_header_get_src_address)
{
    CHECK(test_mac_header_get_src_address());
}

TEST(mac_header_helper_functions, test_mac_header_get_dst_address)
{
    CHECK(test_mac_header_get_dst_address());
}

TEST(mac_header_helper_functions, test_mcps_payload_length_from_received_frame)
{
    CHECK(test_mcps_payload_length_from_received_frame());
}

TEST(mac_header_helper_functions, test_mcps_mac_header_length_from_received_frame)
{
    CHECK(test_mcps_mac_header_length_from_received_frame());
}

TEST(mac_header_helper_functions, test_mcps_mac_security_frame_counter_read)
{
    CHECK(test_mcps_mac_security_frame_counter_read());
}

TEST(mac_header_helper_functions, test_mcps_mac_command_frame_id_get)
{
    CHECK(test_mcps_mac_command_frame_id_get());
}

TEST(mac_header_helper_functions, test_mcps_mac_payload_pointer_get)
{
    CHECK(test_mcps_mac_payload_pointer_get());
}

TEST(mac_header_helper_functions, test_mcps_security_mic_pointer_get)
{
    CHECK(test_mcps_security_mic_pointer_get());
}

TEST(mac_header_helper_functions, test_mcps_mac_security_aux_header_start_pointer_get)
{
    CHECK(test_mcps_mac_security_aux_header_start_pointer_get());
}

TEST(mac_header_helper_functions, test_mcps_generic_header_write)
{
    CHECK(test_mcps_generic_header_write());
}
