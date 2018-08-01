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

    at.write_int(2147483647 + 1);

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
    filehandle_stub_table = NULL;
    filehandle_stub_table_pos = 0;

    ATHandler at(&fh1, que, 0, ",");
    uint8_t buf[5];

    // TEST EMPTY BUFFER
    // Shouldn't read any byte since buffer is empty
    CHECK(-1 == at.read_bytes(buf, 1));
    CHECK(NSAPI_ERROR_DEVICE_ERROR == at.get_last_error());
    // Return error due to error set to at handler by the above call on empty buffer
    CHECK(-1 == at.read_bytes(buf, 1));

    // TEST DATA IN BUFFER
    at.clear_error();
    char table1[] = "1234512345678OK\r\n\0";
    filehandle_stub_table = table1;
    filehandle_stub_table_pos = 0;
    mbed_poll_stub::revents_value = POLLIN;
    mbed_poll_stub::int_value = 1;;

    // Read 5 bytes
    CHECK(5 == at.read_bytes(buf, 5));
    CHECK(!memcmp(buf, table1, 5));
    // get_char triggered above should have filled in the whole reading buffer(fill_buffer())
    CHECK(filehandle_stub_table_pos == (strlen(table1) - 1));
    // Read another 8 bytes
    CHECK(8 == at.read_bytes(buf, 8) && !memcmp(buf, table1 + 5, 8));
    // Reading more than the 4 bytes left -> ERROR
    CHECK(-1 == at.read_bytes(buf, 5));
    CHECK(NSAPI_ERROR_DEVICE_ERROR == at.get_last_error());
}

void Test_ATHandler::test_ATHandler_read_string()
{
    EventQueue que;
    FileHandle_stub fh1;
    filehandle_stub_table = NULL;
    filehandle_stub_table_pos = 0;

    ATHandler at(&fh1, que, 0, ",");

    // *** EMPTY ***
    at.clear_error();
    char table1[] = "";
    at.flush();
    filehandle_stub_table = table1;
    filehandle_stub_table_pos = 0;
    mbed_poll_stub::revents_value = POLLIN;
    mbed_poll_stub::int_value = 1;
    char buf1[1];
    // No _stop_tag set without resp_start
    CHECK(-1 == at.read_string(buf1, 1));
    at.clear_error();
    // Set _stop_tag to resp_stop(OKCRLF)
    at.resp_start();
    // Device error because buffer is empty
    CHECK(NSAPI_ERROR_DEVICE_ERROR == at.get_last_error());
    at.clear_error();
    // Device error because empty buffer and attempt to fill_buffer by consume_char('\"')
    CHECK(-1 == at.read_string(buf1, 1));

    // *** 1 BYTE ***
    at.clear_error();
    char table2[] = "s\0";
    at.flush();
    filehandle_stub_table = table2;
    filehandle_stub_table_pos = 0;
    mbed_poll_stub::revents_value = POLLIN;
    mbed_poll_stub::int_value = 1;
    char buf2[1];
    // Set _stop_tag to resp_stop(OKCRLF)
    at.resp_start();
    // Device error because no CRLF and no more data to read
    CHECK(NSAPI_ERROR_DEVICE_ERROR == at.get_last_error());
    at.clear_error();
    CHECK(0 == at.read_string(buf2, 1));

    // *** CRLF ***
    at.clear_error();
    char table3[] = "\r\ns\r\n\0";
    at.flush();
    filehandle_stub_table = table3;
    filehandle_stub_table_pos = 0;
    mbed_poll_stub::revents_value = POLLIN;
    mbed_poll_stub::int_value = 1;
    char buf3[1];
    // Set _stop_tag to resp_stop(OKCRLF)
    at.resp_start();
    // OK because after CRLF matched there is more data to read ending in CRLF
    CHECK(NSAPI_ERROR_OK == at.get_last_error());
    // To read 0 bytes from: s\r\n
    CHECK(0 == at.read_string(buf3, 0 + 1/*for NULL*/));
    // To read 1 byte from: s\r\n -> read s
    CHECK(1 == at.read_string(buf3, 1 + 1/*for NULL*/));

    // *** Reading more than available in buffer ***
    at.clear_error();
    char table4[] = "\"s,\"OK\r\n\0";
    at.flush();
    filehandle_stub_table = table4;
    filehandle_stub_table_pos = 0;
    mbed_poll_stub::revents_value = POLLIN;
    mbed_poll_stub::int_value = 1;
    char buf4[7];
    uint8_t buf5[5];
    // NO prefix, NO OK, NO ERROR and NO URC match, CRLF found -> return so buffer could be read
    at.resp_start();
    // TO read 5 bytes from: "s,"OK\r\n -> read "s,"O
    at.read_bytes(buf5, 5);
    // K\r\n left to be read -> reading more than 3 + 1(for NULL) -> ERROR
    CHECK(-1 == at.read_string(buf4, 4 + 1/*for NULL*/));
    CHECK(NSAPI_ERROR_DEVICE_ERROR == at.get_last_error());

    // *** Encountering delimiter after reading 1 byte ***
    at.clear_error();
    at.flush();
    filehandle_stub_table = table4;
    filehandle_stub_table_pos = 0;
    // NO prefix, NO OK, NO ERROR and NO URC match, CRLF found -> return so buffer could be read
    at.resp_start();
    // TO read 1 byte from: "s,"OK\r\n -> read "
    at.read_bytes(buf5, 1);
    // TO read max 4 from: s,"OK\r\n -> read s and stop on ,
    CHECK(1 == at.read_string(buf4, 4 + 1/*for NULL*/));

    // *** Encountering delimiter as first char in buffer  ***
    at.clear_error();
    at.flush();
    filehandle_stub_table = table4;
    filehandle_stub_table_pos = 0;
    // NO prefix, NO OK, NO ERROR and NO URC match, CRLF found -> return so buffer could be read
    at.resp_start();
    // TO read 2 bytes from: "s,"OK\r\n -> read "s
    at.read_bytes(buf5, 2);
    // TO read max 4 bytes from: ,"OK\r\n -> stop on ,
    CHECK(0 == at.read_string(buf4, 4 + 1/*for NULL*/));

    // *** Read as much as buffer size is without encountering any delimiter " or OKCRLF  ***
    at.clear_error();
    char table5[] = "\"s\"OK\r\nabcd\0";
    at.flush();
    filehandle_stub_table = table5;
    filehandle_stub_table_pos = 0;
    mbed_poll_stub::revents_value = POLLIN;
    mbed_poll_stub::int_value = 1;
    // NO prefix, NO OK, NO ERROR and NO URC match, CRLF found -> return so buffer could be read
    at.resp_start();
    // TO read 1 byte from: "s"OK\r\n -> read "
    at.read_bytes(buf5, 1);
    // TO read max 1 byte from: s"OK\r\n -> read s
    CHECK(1 == at.read_string(buf4, 1 + 1/*for NULL*/));

    // *** Consume " and run into OKCRLF  ***
    // TO read max 1 byte from: "OK\r\n -> consume " and find stop tag OKCRLF
    CHECK(0 == at.read_string(buf4, 1 + 1/*for NULL*/));

    // *** Try to read after stop tag was found  ***
    // stop tag found do not read further
    CHECK(-1 == at.read_string(buf4, 1 + 1/*for NULL*/));

    // *** Try to read after stop tag was found when parameter allows it  ***
    // stop tag found but flag indicates to read despite stop_tag found
    CHECK(4 == at.read_string(buf4, 4 + 1/*for NULL*/, true));

    // *** Read as much as buffer size is without encountering any delimiter " or OKCRLF  ***
    at.clear_error();
    char table6[] = "sss\rsss\0";
    at.flush();
    filehandle_stub_table = table6;
    filehandle_stub_table_pos = 0;
    mbed_poll_stub::revents_value = POLLIN;
    mbed_poll_stub::int_value = 1;
    at.resp_start("s");
    // TO read from: ss\rsss -> read all 6 chars ss\rsss
    CHECK(6 == at.read_string(buf4, 6 + 1/*for NULL*/));

    // *** Reading when buffer only has "  ***
    at.clear_error();
    char table7[] = "s\"\0";
    at.flush();
    filehandle_stub_table = table7;
    filehandle_stub_table_pos = 0;
    mbed_poll_stub::revents_value = POLLIN;
    mbed_poll_stub::int_value = 1;
    at.resp_start("s");
    // TO read from buffer having only " -> consume " -> trying to read when nothing in buffer
    CHECK(-1 == at.read_string(buf4, 5));
    CHECK(NSAPI_ERROR_DEVICE_ERROR == at.get_last_error());

    // *** Reading through partially matching stop tag  ***
    at.clear_error();
    char table8[] = "\"s\"OK\rabcd\r\n\0";
    at.flush();
    filehandle_stub_table = table8;
    filehandle_stub_table_pos = 0;
    mbed_poll_stub::revents_value = POLLIN;
    mbed_poll_stub::int_value = 1;
    char buf8[9];
    // NO prefix, NO OK, NO ERROR and NO URC match, CRLF found -> return so buffer could be read
    at.resp_start();
    // TO read from
    CHECK(8 == at.read_string(buf8, 8 + 1/*for NULL*/));

    // *** Reading through partially matching stop tag  ***
    at.clear_error();
    char table9[] = "\"s\"Oabcd\r\n\0";
    at.flush();
    filehandle_stub_table = table9;
    filehandle_stub_table_pos = 0;
    mbed_poll_stub::revents_value = POLLIN;
    mbed_poll_stub::int_value = 1;
    char buf9[5];

    // NO prefix, NO OK, NO ERROR and NO URC match, CRLF found -> return so buffer could be read
    at.resp_start();
    // TO read from
    CHECK(6 == at.read_string(buf9, 6 + 1/*for NULL*/));

    // *** CRLF part of the string ***
    at.clear_error();
    char table10[] = "\"s\"\r\nOK\r\n\0";
    at.flush();
    filehandle_stub_table = table10;
    filehandle_stub_table_pos = 0;
    mbed_poll_stub::revents_value = POLLIN;
    mbed_poll_stub::int_value = 1;
    char buf10[10];

    // NO prefix, NO OK, NO ERROR and NO URC match, CRLF found -> return so buffer could be read
    at.resp_start();
    // TO read from
    CHECK(3 == at.read_string(buf10, 9 + 1/*for NULL*/));
}

void Test_ATHandler::test_ATHandler_read_hex_string()
{
    EventQueue que;
    FileHandle_stub fh1;
    filehandle_stub_table = NULL;
    filehandle_stub_table_pos = 0;

    ATHandler at(&fh1, que, 0, ",");

    // *** Read up to delimiter, even length ***
    at.clear_error();
    char table1[] = "68656C6C6F,";
    at.flush();
    filehandle_stub_table = table1;
    filehandle_stub_table_pos = 0;
    mbed_poll_stub::revents_value = POLLIN;
    mbed_poll_stub::int_value = 1;
    char buf1[10];
    // Set _stop_tag to resp_stop(OKCRLF)
    at.resp_start();
    CHECK(5 == at.read_hex_string(buf1, 5));
    CHECK(!strncmp(buf1, "hello", 5));

    // *** Read up to delimiter, odd length ***
    at.clear_error();
    char table2[] = "68656C6C6F7,";
    at.flush();
    filehandle_stub_table = table2;
    filehandle_stub_table_pos = 0;
    mbed_poll_stub::revents_value = POLLIN;
    mbed_poll_stub::int_value = 1;
    char buf2[10];
    // Set _stop_tag to resp_stop(OKCRLF)
    at.resp_start();
    CHECK(5 == at.read_hex_string(buf2, 6));
    CHECK(!strncmp(buf2, "hello", 5));

    // *** Read with stop tag, even length ***
    at.clear_error();
    char table3[] = "6865OK\r\n";
    at.flush();
    filehandle_stub_table = table3;
    filehandle_stub_table_pos = 0;
    mbed_poll_stub::revents_value = POLLIN;
    mbed_poll_stub::int_value = 1;
    char buf3[6];
    // Set _stop_tag to resp_stop(OKCRLF)
    at.resp_start();
    CHECK(2 == at.read_hex_string(buf3, 2 + 1/*get to stop tag match*/));
    CHECK(!strncmp(buf3, "he", 2));
    at.resp_stop();

    // *** Read with stop tag, odd length ***
    at.clear_error();
    char table4[] = "686OK\r\n";
    at.flush();
    filehandle_stub_table = table4;
    filehandle_stub_table_pos = 0;
    mbed_poll_stub::revents_value = POLLIN;
    mbed_poll_stub::int_value = 1;
    char buf4[6];
    // Set _stop_tag to resp_stop(OKCRLF)
    at.resp_start();
    CHECK(1 == at.read_hex_string(buf4, 2 + 1/*get to stop tag match*/));
    CHECK(!strncmp(buf4, "h", 1));
}

void Test_ATHandler::test_ATHandler_read_int()
{
    EventQueue que;
    FileHandle_stub fh1;
    filehandle_stub_table = NULL;
    filehandle_stub_table_pos = 0;

    ATHandler at(&fh1, que, 0, ",");

    int32_t ret = at.read_int();
    CHECK(-1 == ret);
    at.clear_error();

    char table[] = "\",\"OK\r\n\0";
    filehandle_stub_table = table;
    filehandle_stub_table_pos = 0;
    mbed_poll_stub::revents_value = POLLIN;
    mbed_poll_stub::int_value = strlen(table);

    at.resp_start();

    ret = at.read_int();
    CHECK(-1 == ret);
    at.flush();
    at.clear_error();

    char table2[] = "\"2,\"OK\r\n\0";
    filehandle_stub_table = table2;
    filehandle_stub_table_pos = 0;
    mbed_poll_stub::revents_value = POLLIN;
    mbed_poll_stub::int_value = strlen(table2);

    at.resp_start();

    ret = at.read_int();
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
