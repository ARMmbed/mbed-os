/*
 * Copyright (c) 2018 ARM. All rights reserved.
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

    void test_ATHandler_enable_debug();

    void test_ATHandler_get_3gpp_error();
};

#endif // TEST_ATHANDLER_H

