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
#include "CppUTest/TestHarness.h"
#include "test_athandler.h"
#include <string.h>
#include "AT_CellularNetwork.h"
#include "EventQueue.h"
#include "ATHandler.h"
#include "AT_CellularStack.h"
#include "FileHandle_stub.h"
#include "CellularLog.h"
#include "mbed_poll_stub.h"

#include "Timer_stub.h"

using namespace mbed;
using namespace events;

void urc_callback()
{
}

Test_ATHandler::Test_ATHandler()
{

}

Test_ATHandler::~Test_ATHandler()
{
}

void Test_ATHandler::test_ATHandler_constructor()
{
    EventQueue que;
    FileHandle_stub fh1;

    ATHandler *at = new ATHandler(&fh1, que, 0, ",");

    delete at;

    at = new ATHandler(&fh1, que, 0, NULL);

    delete at;
}

void Test_ATHandler::test_ATHandler_get_file_handle()
{
    EventQueue que;
    FileHandle_stub fh1;

    ATHandler at(&fh1, que, 0, ",");
    CHECK_EQUAL(&fh1, at.get_file_handle());
}

void Test_ATHandler::test_ATHandler_set_file_handle()
{
    EventQueue que;
    FileHandle_stub fh1, fh2;

    ATHandler at(&fh1, que, 0, ",");

    at.set_file_handle(&fh2);
}

void Test_ATHandler::test_ATHandler_lock()
{
    EventQueue que;
    FileHandle_stub fh1;

    ATHandler at(&fh1, que, 0, ",");

    at.lock();
}

void Test_ATHandler::test_ATHandler_unlock()
{
    EventQueue que;
    FileHandle_stub fh1;

    ATHandler at(&fh1, que, 0, ",");
    filehandle_stub_short_value_counter = 1;
    fh1.short_value = POLLIN;
    at.unlock();
}

void Test_ATHandler::test_ATHandler_unlock_return_error()
{
    EventQueue que;
    FileHandle_stub fh1;

    ATHandler at(&fh1, que, 0, ",");
    CHECK(NSAPI_ERROR_OK == at.unlock_return_error());
}

void Test_ATHandler::test_ATHandler_set_urc_handler()
{
    EventQueue que;
    FileHandle_stub fh1;

    ATHandler at(&fh1, que, 0, ",");
    const char ch[] = "testtesttesttest";
    at.set_urc_handler(ch, &urc_callback);
}

void Test_ATHandler::test_ATHandler_get_last_error()
{
    EventQueue que;
    FileHandle_stub fh1;

    ATHandler at(&fh1, que, 0, ",");
    CHECK(NSAPI_ERROR_OK == at.get_last_error());
}

void Test_ATHandler::test_ATHandler_get_last_device_error()
{
    EventQueue que;
    FileHandle_stub fh1;

    ATHandler at(&fh1, que, 0, ",");
    CHECK(0 == at.get_last_device_error().errCode);
}

void Test_ATHandler::test_ATHandler_inc_ref_count()
{
    EventQueue que;
    FileHandle_stub fh1;

    ATHandler at(&fh1, que, 0, ",");
    at.inc_ref_count();
}

void Test_ATHandler::test_ATHandler_dec_ref_count()
{
    EventQueue que;
    FileHandle_stub fh1;

    ATHandler at(&fh1, que, 0, ",");
    at.dec_ref_count();
}

void Test_ATHandler::test_ATHandler_get_ref_count()
{
    EventQueue que;
    FileHandle_stub fh1;

    ATHandler at(&fh1, que, 0, ",");
    CHECK(1 == at.get_ref_count());

    at.inc_ref_count();
    CHECK(2 == at.get_ref_count());

    at.inc_ref_count();
    CHECK(3 == at.get_ref_count());

    at.dec_ref_count();
    at.dec_ref_count();
    CHECK(1 == at.get_ref_count());
}

void Test_ATHandler::test_ATHandler_set_at_timeout()
{
    EventQueue que;
    FileHandle_stub fh1;

    ATHandler at(&fh1, que, 0, ",");
    at.set_at_timeout(8);

    at.set_at_timeout(80, true);
}

void Test_ATHandler::test_ATHandler_restore_at_timeout()
{
    EventQueue que;
    FileHandle_stub fh1;

    ATHandler at(&fh1, que, 0, ",");
    at.set_at_timeout(80, true);
    at.set_at_timeout(800);
    at.restore_at_timeout();
}

void Test_ATHandler::test_ATHandler_clear_error()
{
    EventQueue que;
    FileHandle_stub fh1;

    ATHandler at(&fh1, que, 0, ",");
    at.clear_error();
}

void Test_ATHandler::test_ATHandler_process_oob()
{
    EventQueue que;
    FileHandle_stub fh1;

    ATHandler at(&fh1, que, 0, ",");
    filehandle_stub_short_value_counter = 1;
    fh1.short_value = POLLIN;
    at.set_urc_handler("s", &urc_callback);
    at.process_oob();

    filehandle_stub_short_value_counter = 2;
    at.process_oob();

    //at.fill_buffer();
    uint8_t buf[5];
    at.clear_error();
    char table[] = "ssssssssssssssssssssssssssssssss\0";
    filehandle_stub_table = table;
    filehandle_stub_table_pos = 0;
    at.read_bytes(buf, 5);

    filehandle_stub_short_value_counter = 2;
    at.process_oob();

    at.clear_error();
    timer_stub_value = 0;
    filehandle_stub_table_pos = 0;
    at.read_bytes(buf, 5);

    filehandle_stub_short_value_counter = 1;
    at.process_oob();

    char table2[4];
    table2[0] = '\r';
    table2[1] = '\r';
    table2[2] = '\n';
    table2[3] = 0;
    filehandle_stub_table = table2;

    at.clear_error();
    timer_stub_value = 0;
    filehandle_stub_table_pos = 0;
    at.read_bytes(buf, 1);

    filehandle_stub_short_value_counter = 1;
    at.process_oob();


    filehandle_stub_table = table;


    filehandle_stub_short_value_counter = 0;
    filehandle_stub_table_pos = 0;
    filehandle_stub_table = NULL;
}

void Test_ATHandler::test_ATHandler_set_filehandle_sigio()
{
    EventQueue que;
    FileHandle_stub fh1;

    ATHandler at(&fh1, que, 0, ",");
    at.set_filehandle_sigio();
}

void Test_ATHandler::test_ATHandler_flush()
{
    EventQueue que;
    FileHandle_stub fh1;

    ATHandler at(&fh1, que, 0, ",");
    filehandle_stub_short_value_counter = 1;
    fh1.short_value = POLLIN;
    at.flush();
}

void Test_ATHandler::test_ATHandler_cmd_start()
{
    EventQueue que;
    FileHandle_stub fh1;

    ATHandler at(&fh1, que, 0, ",");
    mbed_poll_stub::revents_value = POLLOUT;
    mbed_poll_stub::int_value = 1;
    fh1.size_value = 3;
    at.cmd_start("s");
    mbed_poll_stub::revents_value = POLLIN;
    mbed_poll_stub::int_value = 0;

    at.cmd_start("s");

    at.cmd_start("s");
}

void Test_ATHandler::test_ATHandler_write_int()
{
    EventQueue que;
    FileHandle_stub fh1;

    ATHandler at(&fh1, que, 0, ",");
    fh1.size_value = -1;
    at.write_int(4);

    at.clear_error();
    mbed_poll_stub::revents_value = POLLOUT;
    mbed_poll_stub::int_value = 1;
    fh1.size_value = 6;
    at.write_int(4);

    at.write_int(2147483647);

    at.write_int(2147483647+1);

//    at.at_error(0, DeviceErrorType(0));
//    at.write_int(4);
}

void Test_ATHandler::test_ATHandler_write_string()
{
    EventQueue que;
    FileHandle_stub fh1;

    ATHandler at(&fh1, que, 0, ",");
    at.write_string("help");
    CHECK(NSAPI_ERROR_DEVICE_ERROR == at.get_last_error());

    at.clear_error();
    mbed_poll_stub::revents_value = POLLOUT;
    mbed_poll_stub::int_value = 1;
    fh1.size_value = -1;
    at.cmd_start("s");
    at.write_string("help", true);
    CHECK(NSAPI_ERROR_DEVICE_ERROR == at.get_last_error());

    at.clear_error();
    mbed_poll_stub::revents_value = POLLOUT;
    mbed_poll_stub::int_value = 1;
    fh1.size_value = -1;
    at.write_string("help", true);
    CHECK(NSAPI_ERROR_DEVICE_ERROR == at.get_last_error());

    at.clear_error();
    mbed_poll_stub::revents_value = POLLOUT;
    mbed_poll_stub::int_value = 1;
    fh1.size_value = 7;
    at.write_string("help", true);
    CHECK(NSAPI_ERROR_OK == at.get_last_error());
}

void Test_ATHandler::test_ATHandler_cmd_stop()
{
    EventQueue que;
    FileHandle_stub fh1;

    ATHandler at(&fh1, que, 0, ",");
    fh1.size_value = -1;
    at.cmd_stop();

    at.write_string("help", true);

    at.cmd_stop();
    CHECK(NSAPI_ERROR_DEVICE_ERROR == at.get_last_error());
}

void Test_ATHandler::test_ATHandler_write_bytes()
{
    EventQueue que;
    FileHandle_stub fh1;

    ATHandler at(&fh1, que, 0, ",");
    fh1.size_value = -1;
    uint8_t data[] = "data";
    at.write_bytes(data, 4);

    at.write_bytes(data, 4);
    CHECK(NSAPI_ERROR_DEVICE_ERROR == at.get_last_error());
}

void Test_ATHandler::test_ATHandler_set_stop_tag()
{
    EventQueue que;
    FileHandle_stub fh1;

    ATHandler at(&fh1, que, 0, ",");
    at.set_stop_tag("s");
}

void Test_ATHandler::test_ATHandler_set_delimiter()
{
    EventQueue que;
    FileHandle_stub fh1;

    ATHandler at(&fh1, que, 0, ",");
    at.set_delimiter('+');
}

void Test_ATHandler::test_ATHandler_skip_param()
{
    EventQueue que;
    FileHandle_stub fh1;

    ATHandler at(&fh1, que, 0, ",");
    at.skip_param();

    char table[] = "ssssssssssssssssssssssssssssOK\r\n\0";
    filehandle_stub_table = table;
    filehandle_stub_table_pos = 0;

    at.flush();
    at.clear_error();
    at.resp_start();
    at.skip_param();
    CHECK(at.get_last_error() == NSAPI_ERROR_DEVICE_ERROR);

    char table1[] = "ss,sssssssssssss,sssssssssssOK\r\n\0";
    filehandle_stub_table = table1;
    filehandle_stub_table_pos = 0;

    at.flush();
    at.clear_error();
    at.resp_start();
    at.skip_param();

    char table2[] = "sssOK\r\n\0";
    filehandle_stub_table = table2;
    filehandle_stub_table_pos = 0;

    at.flush();
    at.clear_error();
    at.resp_start();
    at.skip_param();

    char table3[] = "sssssssOK\nssss\0";
    filehandle_stub_table = table3;
    filehandle_stub_table_pos = 0;

    //Need to create a new instance because stop tag already found
    ATHandler at2(&fh1, que, 0, ",");
    at2.flush();
    at2.clear_error();
    at2.resp_start();
    at2.skip_param();

    at2.skip_param(4, 3);

    filehandle_stub_table = table3;
    filehandle_stub_table_pos = 0;

    at2.flush();
    at2.clear_error();
    at2.resp_start();
    at2.skip_param(4, 3);

    filehandle_stub_table = table3;
    filehandle_stub_table_pos = 0;

    at2.flush();
    at2.clear_error();
    at2.resp_start();
    at2.skip_param(24, 17);
}

void Test_ATHandler::test_ATHandler_read_bytes()
{
    EventQueue que;
    FileHandle_stub fh1;

    ATHandler at(&fh1, que, 0, ",");
    uint8_t buf[5];
    CHECK(-1 == at.read_bytes(buf, 25));

    CHECK(-1 == at.read_bytes(buf, 5));

    char table[] = "ssssssssssssssssssssssssssssOK\r\n\0";
    filehandle_stub_table = table;
    filehandle_stub_table_pos = 0;
    mbed_poll_stub::revents_value = POLLIN;
    mbed_poll_stub::int_value = strlen(table);


    at.clear_error();
    CHECK(5 == at.read_bytes(buf, 5));
}

void Test_ATHandler::test_ATHandler_read_string()
{
    EventQueue que;
    FileHandle_stub fh1;

    ATHandler at(&fh1, que, 0, ",");

    at.clear_error();
    char table[] = "\"s,\"OK\r\n\0";
    filehandle_stub_table = table;
    filehandle_stub_table_pos = 0;
    mbed_poll_stub::revents_value = POLLIN;
    mbed_poll_stub::int_value = strlen(table);

    char buf[5];
    uint8_t buf2[5];
    at.resp_start();
    at.read_bytes(buf2, 5);
    CHECK(-1 == at.read_string(buf, 15));
    at.flush();
    at.clear_error();

    filehandle_stub_table = table;
    filehandle_stub_table_pos = 0;

    at.resp_start();
    at.read_bytes(buf2, 1);
    CHECK(1 == at.read_string(buf, 5, true));
    at.flush();
    at.clear_error();

    char table2[] = "\"s\"OK\r\n\0";
    filehandle_stub_table = table2;
    filehandle_stub_table_pos = 0;
    mbed_poll_stub::revents_value = POLLIN;
    mbed_poll_stub::int_value = strlen(table2);

    at.resp_start();
    at.read_bytes(buf2, 1);
    CHECK(1 == at.read_string(buf, 5, true));
    at.flush();
    at.clear_error();

    char table3[] = "sss\rsss\0";
    filehandle_stub_table = table3;
    filehandle_stub_table_pos = 0;
    mbed_poll_stub::revents_value = POLLIN;
    mbed_poll_stub::int_value = strlen(table);

    at.resp_start("s");
    at.read_string(buf, 5, true);
    at.flush();
    at.clear_error();

    char table4[] = "\"s\"\0";
    filehandle_stub_table = table4;
    filehandle_stub_table_pos = 0;
    mbed_poll_stub::revents_value = POLLIN;
    mbed_poll_stub::int_value = strlen(table);

    at.resp_start("s");
    at.read_string(buf, 5, true);

    filehandle_stub_table = NULL;
    filehandle_stub_table_pos = 0;
    mbed_poll_stub::revents_value = POLLOUT;
    mbed_poll_stub::int_value = 0;
}

void Test_ATHandler::test_ATHandler_read_int()
{
    EventQueue que;
    FileHandle_stub fh1;

    ATHandler at(&fh1, que, 0, ",");

    int32_t ret= at.read_int();
    CHECK(-1 == ret);
    at.clear_error();

    char table[] = "\",\"OK\r\n\0";
    filehandle_stub_table = table;
    filehandle_stub_table_pos = 0;
    mbed_poll_stub::revents_value = POLLIN;
    mbed_poll_stub::int_value = strlen(table);

    at.resp_start();

    ret= at.read_int();
    CHECK(-1 == ret);
    at.flush();
    at.clear_error();

    char table2[] = "\"2,\"OK\r\n\0";
    filehandle_stub_table = table2;
    filehandle_stub_table_pos = 0;
    mbed_poll_stub::revents_value = POLLIN;
    mbed_poll_stub::int_value = strlen(table2);

    at.resp_start();

    ret= at.read_int();
    CHECK(2 == ret);

}

void Test_ATHandler::test_ATHandler_resp_start()
{
    EventQueue que;
    FileHandle_stub fh1;

    filehandle_stub_table = NULL;
    filehandle_stub_table_pos = 0;

    ATHandler at(&fh1, que, 0, ",");
    at.resp_start();
    at.resp_start();

    char table2[] = "\"2,\"OK\r\n\0";
    filehandle_stub_table = table2;
    filehandle_stub_table_pos = 0;

    at.flush();
    at.clear_error();
    at.resp_start("ssssaaaassssaaaassss"); //too long prefix

    char table3[] = "+CME ERROR: 108\0";
    filehandle_stub_table = table3;
    filehandle_stub_table_pos = 0;

    at.flush();
    at.clear_error();
    at.resp_start();

    filehandle_stub_table_pos = 0;

    at.flush();
    at.clear_error();
    at.resp_start();

    char table4[] = "+CMS ERROR: 6\0";
    filehandle_stub_table = table4;
    filehandle_stub_table_pos = 0;

    at.flush();
    at.clear_error();
    at.resp_start();

    char table5[] = "ERROR\r\n\0";
    filehandle_stub_table = table5;
    filehandle_stub_table_pos = 0;

    at.flush();
    at.clear_error();
    at.resp_start();

    char table6[] = "OK\r\n\0";
    filehandle_stub_table = table6;
    filehandle_stub_table_pos = 0;

    at.flush();
    at.clear_error();
    at.resp_start();

    char table7[] = "ssssss\0";
    filehandle_stub_table = table7;
    filehandle_stub_table_pos = 0;

    at.flush();
    at.clear_error();
    at.set_urc_handler("ss", NULL);
    at.resp_start();
}

void Test_ATHandler::test_ATHandler_resp_stop()
{
    EventQueue que;
    FileHandle_stub fh1;

    ATHandler at(&fh1, que, 0, ",");

    char table[] = "21 OK\r\n\0";
    filehandle_stub_table = table;
    filehandle_stub_table_pos = 0;

    at.info_elem('2');
    at.set_stop_tag("OK\r\n");
    at.resp_stop();

    char table3[] = "+CME ERROR: 108\0";
    filehandle_stub_table = table3;
    filehandle_stub_table_pos = 0;

    at.flush();
    at.clear_error();
    at.resp_start();

    at.resp_stop();

    char table7[] = "ssssss\0";
    filehandle_stub_table = table7;
    filehandle_stub_table_pos = 0;

    at.flush();
    at.clear_error();
    at.resp_start("ss", false);
    at.resp_stop();
}

void Test_ATHandler::test_ATHandler_info_resp()
{
    EventQueue que;
    FileHandle_stub fh1;

    filehandle_stub_table = NULL;

    ATHandler at(&fh1, que, 0, ",");
    CHECK(at.info_resp());

    at.resp_start();
    CHECK(!at.info_resp());

    at.flush();
    at.clear_error();

    char table2[] = "21 OK\r\n\0";
    filehandle_stub_table = table2;
    filehandle_stub_table_pos = 0;
    mbed_poll_stub::revents_value = POLLIN;
    mbed_poll_stub::int_value = strlen(table2);

    at.resp_start("21");
    CHECK(at.info_resp());

    CHECK(!at.info_resp());

    at.flush();
    at.clear_error();

    char table3[] = "21 OK\r\n\0";
    filehandle_stub_table = table3;
    filehandle_stub_table_pos = 0;
    mbed_poll_stub::revents_value = POLLIN;
    mbed_poll_stub::int_value = strlen(table3);

    CHECK(at.info_resp());
}

void Test_ATHandler::test_ATHandler_info_elem()
{
    EventQueue que;
    FileHandle_stub fh1;

    char table[] = "21 OK\r\n\0";
    filehandle_stub_table = table;
    filehandle_stub_table_pos = 0;
    mbed_poll_stub::revents_value = POLLIN;
    mbed_poll_stub::int_value = strlen(table);

    ATHandler at(&fh1, que, 0, ",");
    CHECK(!at.info_elem('O'));
    at.flush();

    char table2[] = "21 OK\r\n\0";
    filehandle_stub_table = table2;
    filehandle_stub_table_pos = 0;
    mbed_poll_stub::revents_value = POLLIN;
    mbed_poll_stub::int_value = strlen(table2);

    at.clear_error();
    at.resp_start("21");
    CHECK(at.info_elem('O'));
    at.flush();

    filehandle_stub_table = NULL;
    filehandle_stub_table_pos = 0;

    at.clear_error();
    at.resp_start("21");
    CHECK(!at.info_elem('2'));
}

void Test_ATHandler::test_ATHandler_consume_to_stop_tag()
{
    EventQueue que;
    FileHandle_stub fh1;

    ATHandler at(&fh1, que, 0, ",");
    CHECK(at.consume_to_stop_tag());
}

void Test_ATHandler::test_ATHandler_set_debug()
{
    EventQueue que;
    FileHandle_stub fh1;

    ATHandler at(&fh1, que, 0, ",");
    at.set_debug(true);

    at.set_debug(false);
}

void Test_ATHandler::test_ATHandler_get_3gpp_error()
{
    EventQueue que;
    FileHandle_stub fh1;

    ATHandler at(&fh1, que, 0, ",");
    int ret = at.get_3gpp_error();
}
