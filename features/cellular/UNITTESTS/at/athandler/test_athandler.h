/*
 * Copyright (c) 2018, Arm Limited and affiliates.
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
#ifndef TEST_ATHANDLER_H
#define TEST_ATHANDLER_H

class Test_ATHandler
{
public:
    Test_ATHandler();

    virtual ~Test_ATHandler();

    void test_ATHandler_constructor();

    void test_ATHandler_get_file_handle();

    void test_ATHandler_set_file_handle();

    void test_ATHandler_lock();

    void test_ATHandler_unlock();

    void test_ATHandler_unlock_return_error();

    void test_ATHandler_set_urc_handler();

    void test_ATHandler_get_last_error();

    void test_ATHandler_get_last_device_error();

    void test_ATHandler_inc_ref_count();

    void test_ATHandler_dec_ref_count();

    void test_ATHandler_get_ref_count();

    void test_ATHandler_set_at_timeout();

    void test_ATHandler_restore_at_timeout();

    void test_ATHandler_clear_error();

    void test_ATHandler_process_oob();

    void test_ATHandler_set_filehandle_sigio();

    void test_ATHandler_flush();

    void test_ATHandler_cmd_start();

    void test_ATHandler_write_int();

    void test_ATHandler_write_string();

    void test_ATHandler_cmd_stop();

    void test_ATHandler_write_bytes();

    void test_ATHandler_set_stop_tag();

    void test_ATHandler_set_delimiter();

    void test_ATHandler_skip_param();

    void test_ATHandler_read_bytes();

    void test_ATHandler_read_string();

    void test_ATHandler_read_int();

    void test_ATHandler_resp_start();

    void test_ATHandler_resp_stop();

    void test_ATHandler_info_resp();

    void test_ATHandler_info_elem();

    void test_ATHandler_consume_to_stop_tag();

    void test_ATHandler_set_debug();

    void test_ATHandler_get_3gpp_error();
};

#endif // TEST_ATHANDLER_H

