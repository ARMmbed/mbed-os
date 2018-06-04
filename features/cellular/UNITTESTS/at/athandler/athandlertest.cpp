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
#include "test_athandler.h"

TEST_GROUP(ATHandler)
{
    Test_ATHandler* unit;

    void setup()
    {
        unit = new Test_ATHandler();
    }

    void teardown()
    {
        delete unit;
    }
};

TEST(ATHandler, Create)
{
    CHECK(unit != NULL);
}

TEST(ATHandler, test_ATHandler_constructor)
{
    unit->test_ATHandler_constructor();
}

TEST(ATHandler, test_ATHandler_get_file_handle)
{
    unit->test_ATHandler_get_file_handle();
}

TEST(ATHandler, test_ATHandler_set_file_handle)
{
    unit->test_ATHandler_set_file_handle();
}

TEST(ATHandler, test_ATHandler_lock)
{
    unit->test_ATHandler_lock();
}

TEST(ATHandler, test_ATHandler_unlock)
{
    unit->test_ATHandler_unlock();
}

TEST(ATHandler, test_ATHandler_unlock_return_error)
{
    unit->test_ATHandler_unlock_return_error();
}

TEST(ATHandler, test_ATHandler_set_urc_handler)
{
    unit->test_ATHandler_set_urc_handler();
}

TEST(ATHandler, test_ATHandler_get_last_error)
{
    unit->test_ATHandler_get_last_error();
}

TEST(ATHandler, test_ATHandler_get_last_device_error)
{
    unit->test_ATHandler_get_last_device_error();
}

TEST(ATHandler, test_ATHandler_inc_ref_count)
{
    unit->test_ATHandler_inc_ref_count();
}

TEST(ATHandler, test_ATHandler_dec_ref_count)
{
    unit->test_ATHandler_dec_ref_count();
}

TEST(ATHandler, test_ATHandler_get_ref_count)
{
    unit->test_ATHandler_get_ref_count();
}

TEST(ATHandler, test_ATHandler_set_at_timeout)
{
    unit->test_ATHandler_set_at_timeout();
}

TEST(ATHandler, test_ATHandler_restore_at_timeout)
{
    unit->test_ATHandler_restore_at_timeout();
}

TEST(ATHandler, test_ATHandler_clear_error)
{
    unit->test_ATHandler_clear_error();
}

TEST(ATHandler, test_ATHandler_process_oob)
{
    unit->test_ATHandler_process_oob();
}

TEST(ATHandler, test_ATHandler_set_filehandle_sigio)
{
    unit->test_ATHandler_set_filehandle_sigio();
}

TEST(ATHandler, test_ATHandler_flush)
{
    unit->test_ATHandler_flush();
}

TEST(ATHandler, test_ATHandler_cmd_start)
{
    unit->test_ATHandler_cmd_start();
}

TEST(ATHandler, test_ATHandler_write_int)
{
    unit->test_ATHandler_write_int();
}

TEST(ATHandler, test_ATHandler_write_string)
{
    unit->test_ATHandler_write_string();
}

TEST(ATHandler, test_ATHandler_cmd_stop)
{
    unit->test_ATHandler_cmd_stop();
}

TEST(ATHandler, test_ATHandler_write_bytes)
{
    unit->test_ATHandler_write_bytes();
}

TEST(ATHandler, test_ATHandler_set_stop_tag)
{
    unit->test_ATHandler_set_stop_tag();
}

TEST(ATHandler, test_ATHandler_set_delimiter)
{
    unit->test_ATHandler_set_delimiter();
}

TEST(ATHandler, test_ATHandler_skip_param)
{
    unit->test_ATHandler_skip_param();
}

TEST(ATHandler, test_ATHandler_read_bytes)
{
    unit->test_ATHandler_read_bytes();
}

TEST(ATHandler, test_ATHandler_read_string)
{
    unit->test_ATHandler_read_string();
}

TEST(ATHandler, test_ATHandler_read_int)
{
    unit->test_ATHandler_read_int();
}

TEST(ATHandler, test_ATHandler_resp_start)
{
    unit->test_ATHandler_resp_start();
}

TEST(ATHandler, test_ATHandler_resp_stop)
{
    unit->test_ATHandler_resp_stop();
}

TEST(ATHandler, test_ATHandler_info_resp)
{
    unit->test_ATHandler_info_resp();
}

TEST(ATHandler, test_ATHandler_info_elem)
{
    unit->test_ATHandler_info_elem();
}

TEST(ATHandler, test_ATHandler_consume_to_stop_tag)
{
    unit->test_ATHandler_consume_to_stop_tag();
}

TEST(ATHandler, test_ATHandler_set_debug)
{
    unit->test_ATHandler_set_debug();
}

TEST(ATHandler, test_ATHandler_get_3gpp_error)
{
    unit->test_ATHandler_get_3gpp_error();
}
