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
#include "gtest/gtest.h"
#include <string.h>
#include "AT_CellularNetwork.h"
#include "events/EventQueue.h"
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

void urc2_callback()
{
}

// AStyle ignored as the definition is not clear due to preprocessor usage
// *INDENT-OFF*
class TestATHandler : public testing::Test {
protected:

    void SetUp()
    {
    }

    void TearDown()
    {
    }
};
// *INDENT-ON*

TEST_F(TestATHandler, Create)
{
    EventQueue que;
    FileHandle_stub fh1;

    ATHandler *at = new ATHandler(&fh1, que, 0, ",");

    delete at;

    at = new ATHandler(&fh1, que, 0, NULL);

    EXPECT_TRUE(at != NULL);
    delete at;
}

TEST_F(TestATHandler, test_ATHandler_get_file_handle)
{
    EventQueue que;
    FileHandle_stub fh1;

    ATHandler at(&fh1, que, 0, ",");
    EXPECT_EQ(&fh1, at.get_file_handle());
}

TEST_F(TestATHandler, test_ATHandler_set_file_handle)
{
    EventQueue que;
    FileHandle_stub fh1, fh2;

    ATHandler at(&fh1, que, 0, ",");

    at.set_file_handle(&fh2);
}

TEST_F(TestATHandler, test_ATHandler_list)
{
    EventQueue que;
    FileHandle_stub fh1;

    ATHandler::set_at_timeout_list(1000, false);
    ATHandler::set_debug_list(false);

    ATHandler *at1 = ATHandler::get_instance(&fh1, que, 0, ",", 0, 0);
    EXPECT_TRUE(at1->get_ref_count() == 1);

    ATHandler::set_at_timeout_list(1000, false);
    ATHandler::set_debug_list(true);

    EXPECT_TRUE(ATHandler::get_instance(NULL, que, 0, ",", 0, 0) == NULL);

    ATHandler *at2 = ATHandler::get_instance(&fh1, que, 0, ",", 0, 0);
    EXPECT_TRUE(at1->get_ref_count() == 2);
    EXPECT_TRUE(at2->get_ref_count() == 2);

    ATHandler::set_at_timeout_list(2000, true);
    ATHandler::set_debug_list(false);

    EXPECT_TRUE(at1->close() == NSAPI_ERROR_OK);
    EXPECT_TRUE(at2->get_ref_count() == 1);
    EXPECT_TRUE(at2->close() == NSAPI_ERROR_OK);

    ATHandler::set_at_timeout_list(1000, false);
    ATHandler::set_debug_list(false);
}

TEST_F(TestATHandler, test_ATHandler_lock)
{
    EventQueue que;
    FileHandle_stub fh1;

    ATHandler at(&fh1, que, 0, ",");

    at.lock();
}

TEST_F(TestATHandler, test_ATHandler_unlock)
{
    EventQueue que;
    FileHandle_stub fh1;

    ATHandler at(&fh1, que, 0, ",");
    filehandle_stub_short_value_counter = 1;
    fh1.short_value = POLLIN;
    at.unlock();
}

TEST_F(TestATHandler, test_ATHandler_unlock_return_error)
{
    EventQueue que;
    FileHandle_stub fh1;

    ATHandler at(&fh1, que, 0, ",");
    EXPECT_TRUE(NSAPI_ERROR_OK == at.unlock_return_error());
}

TEST_F(TestATHandler, test_ATHandler_set_urc_handler)
{
    EventQueue que;
    FileHandle_stub fh1;

    ATHandler at(&fh1, que, 0, ",");
    const char ch[] = "testtesttesttest";

    mbed::Callback<void()> cb(&urc_callback);
    at.set_urc_handler(ch, cb);

    //THIS IS NOT same callback in find_urc_handler???
    at.set_urc_handler(ch, cb);
}

TEST_F(TestATHandler, test_ATHandler_remove_urc_handler)
{
    EventQueue que;
    FileHandle_stub fh1;

    ATHandler at(&fh1, que, 0, ",");
    const char ch[] = "testtesttesttest";

    mbed::Callback<void()> cb(&urc_callback);
    at.set_urc_handler(ch, cb);

    at.set_urc_handler(ch, 0);
}

TEST_F(TestATHandler, test_ATHandler_get_last_error)
{
    EventQueue que;
    FileHandle_stub fh1;

    ATHandler at(&fh1, que, 0, ",");
    EXPECT_TRUE(NSAPI_ERROR_OK == at.get_last_error());
}

TEST_F(TestATHandler, test_ATHandler_get_last_device_error)
{
    EventQueue que;
    FileHandle_stub fh1;

    ATHandler at(&fh1, que, 0, ",");
    EXPECT_TRUE(0 == at.get_last_device_error().errCode);
}

TEST_F(TestATHandler, test_ATHandler_inc_ref_count)
{
    EventQueue que;
    FileHandle_stub fh1;

    ATHandler at(&fh1, que, 0, ",");
    at.inc_ref_count();
}

TEST_F(TestATHandler, test_ATHandler_dec_ref_count)
{
    EventQueue que;
    FileHandle_stub fh1;

    ATHandler at(&fh1, que, 0, ",");
    at.dec_ref_count();
}

TEST_F(TestATHandler, test_ATHandler_get_ref_count)
{
    EventQueue que;
    FileHandle_stub fh1;

    ATHandler at(&fh1, que, 0, ",");
    EXPECT_TRUE(1 == at.get_ref_count());

    at.inc_ref_count();
    EXPECT_TRUE(2 == at.get_ref_count());

    at.inc_ref_count();
    EXPECT_TRUE(3 == at.get_ref_count());

    at.dec_ref_count();
    at.dec_ref_count();
    EXPECT_TRUE(1 == at.get_ref_count());
}

TEST_F(TestATHandler, test_ATHandler_set_at_timeout)
{
    EventQueue que;
    FileHandle_stub fh1;

    ATHandler at(&fh1, que, 0, ",");
    at.set_at_timeout(8);

    at.set_at_timeout(80, true);
}

TEST_F(TestATHandler, test_ATHandler_restore_at_timeout)
{
    EventQueue que;
    FileHandle_stub fh1;

    ATHandler at(&fh1, que, 0, ",");
    at.set_at_timeout(80, true);
    at.set_at_timeout(800);
    at.restore_at_timeout();
}

TEST_F(TestATHandler, test_ATHandler_clear_error)
{
    EventQueue que;
    FileHandle_stub fh1;

    ATHandler at(&fh1, que, 0, ",");
    at.clear_error();
}

TEST_F(TestATHandler, test_ATHandler_process_oob)
{
    EventQueue que;
    FileHandle_stub fh1;

    ATHandler at(&fh1, que, 0, ",");
    at.set_at_timeout(10);

    at.set_is_filehandle_usable(false);
    at.process_oob();
    at.set_is_filehandle_usable(true);

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
    mbed_poll_stub::revents_value = POLLIN;
    mbed_poll_stub::int_value = 1;
    at.read_bytes(buf, 5);

    filehandle_stub_short_value_counter = 2;
    at.process_oob();

    at.clear_error();
    timer_stub_value = 0;
    filehandle_stub_table_pos = 0;
    at.read_bytes(buf, 5);

    filehandle_stub_short_value_counter = 1;
    at.process_oob();

    char table2[5];
    table2[0] = '\r';
    table2[1] = '\r';
    table2[2] = '\n';
    table2[3] = '\n';
    table2[4] = 0;
    filehandle_stub_table = table2;

    at.clear_error();
    timer_stub_value = 0;
    filehandle_stub_table_pos = 0;
    mbed_poll_stub::revents_value = POLLIN;
    mbed_poll_stub::int_value = 1;
    at.read_bytes(buf, 1);

    filehandle_stub_short_value_counter = 1;
    at.process_oob();

    filehandle_stub_table = table;

    filehandle_stub_short_value_counter = 0;
    filehandle_stub_table_pos = 0;
    filehandle_stub_table = NULL;
}

TEST_F(TestATHandler, test_ATHandler_flush)
{
    EventQueue que;
    FileHandle_stub fh1;

    ATHandler at(&fh1, que, 0, ",");
    filehandle_stub_short_value_counter = 1;
    fh1.short_value = POLLIN;
    at.flush();
}

TEST_F(TestATHandler, test_ATHandler_cmd_start)
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

TEST_F(TestATHandler, test_ATHandler_write_int)
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

TEST_F(TestATHandler, test_ATHandler_write_string)
{
    EventQueue que;
    FileHandle_stub fh1;

    ATHandler at(&fh1, que, 0, ",");
    fh1.size_value = -1;
    at.write_string("help");
    EXPECT_TRUE(NSAPI_ERROR_DEVICE_ERROR == at.get_last_error());

    at.clear_error();
    mbed_poll_stub::revents_value = POLLOUT;
    mbed_poll_stub::int_value = 1;
    fh1.size_value = -1;
    at.cmd_start("s");
    at.write_string("help", true);
    EXPECT_TRUE(NSAPI_ERROR_DEVICE_ERROR == at.get_last_error());

    at.clear_error();
    mbed_poll_stub::revents_value = POLLOUT;
    mbed_poll_stub::int_value = 1;
    fh1.size_value = -1;
    at.write_string("help", true);
    EXPECT_TRUE(NSAPI_ERROR_DEVICE_ERROR == at.get_last_error());

    at.clear_error();
    mbed_poll_stub::revents_value = POLLOUT;
    mbed_poll_stub::int_value = 1;
    fh1.size_value = 7;
    at.write_string("help", true);
    EXPECT_TRUE(NSAPI_ERROR_OK == at.get_last_error());
}

TEST_F(TestATHandler, test_ATHandler_cmd_stop)
{
    EventQueue que;
    FileHandle_stub fh1;

    ATHandler at(&fh1, que, 0, ",");
    fh1.size_value = -1;
    at.cmd_stop();

    at.write_string("help", true);

    at.cmd_stop();
    EXPECT_TRUE(NSAPI_ERROR_DEVICE_ERROR == at.get_last_error());
}

TEST_F(TestATHandler, test_ATHandler_write_bytes)
{
    EventQueue que;
    FileHandle_stub fh1;

    ATHandler at(&fh1, que, 0, ",");
    fh1.size_value = -1;
    uint8_t data[] = "data";
    at.write_bytes(data, 4);

    at.write_bytes(data, 4);
    EXPECT_TRUE(NSAPI_ERROR_DEVICE_ERROR == at.get_last_error());
}

TEST_F(TestATHandler, test_ATHandler_set_stop_tag)
{
    EventQueue que;
    FileHandle_stub fh1;

    ATHandler at(&fh1, que, 0, ",");
    at.set_stop_tag("s");
}

TEST_F(TestATHandler, test_ATHandler_set_delimiter)
{
    EventQueue que;
    FileHandle_stub fh1;

    ATHandler at(&fh1, que, 0, ",");
    at.set_delimiter('+');
}

TEST_F(TestATHandler, test_ATHandler_skip_param)
{
    EventQueue que;
    FileHandle_stub fh1;

    ATHandler at(&fh1, que, 0, ",");
    at.set_stop_tag("OK\r\n");
    at.skip_param();

    char table[] = "ssssssssssssssssssssssssssssOK\r\n\0";
    filehandle_stub_table = table;

    at.flush();
    at.clear_error();
    filehandle_stub_table_pos = 0;
    mbed_poll_stub::revents_value = POLLIN;
    mbed_poll_stub::int_value = 1;
    filehandle_stub_short_value_counter = 1;
    fh1.short_value = POLLIN;
    at.resp_start();
    at.skip_param();
    EXPECT_TRUE(at.get_last_error() == NSAPI_ERROR_OK);

    char table1[] = "ss,sssssssssssss,sssssssssssOK\r\n\0";
    filehandle_stub_table = table1;

    at.flush();
    at.clear_error();
    filehandle_stub_short_value_counter = 1;
    filehandle_stub_table_pos = 0;
    at.resp_start();
    at.skip_param();

    char table2[] = "sssOK\r\n\0";
    filehandle_stub_table = table2;

    at.flush();
    at.clear_error();
    filehandle_stub_short_value_counter = 1;
    filehandle_stub_table_pos = 0;
    at.resp_start();
    at.skip_param();

    char table3[] = "sssssssOK\nssss\0";
    filehandle_stub_table = table3;

    //Need to create a new instance because stop tag already found
    ATHandler at2(&fh1, que, 0, ",");
    at2.flush();
    at2.clear_error();
    filehandle_stub_short_value_counter = 1;
    filehandle_stub_table_pos = 0;
    at2.resp_start();
    at2.skip_param();

    at2.skip_param(4, 3);

    filehandle_stub_table = table3;

    at2.flush();
    at2.clear_error();
    filehandle_stub_short_value_counter = 1;
    filehandle_stub_table_pos = 0;
    at2.resp_start();
    at2.skip_param(4, 3);

    filehandle_stub_table = table3;

    at2.flush();
    at2.clear_error();
    filehandle_stub_short_value_counter = 1;
    filehandle_stub_table_pos = 0;
    at2.resp_start();
    at2.skip_param(24, 17);
}

TEST_F(TestATHandler, test_ATHandler_read_bytes)
{
    EventQueue que;
    FileHandle_stub fh1;
    filehandle_stub_table = NULL;
    filehandle_stub_table_pos = 0;

    ATHandler at(&fh1, que, 0, ",");
    uint8_t buf[5];

    // TEST EMPTY BUFFER
    // Shouldn't read any byte since buffer is empty
    EXPECT_TRUE(-1 == at.read_bytes(buf, 1));
    EXPECT_TRUE(NSAPI_ERROR_DEVICE_ERROR == at.get_last_error());
    // Return error due to error set to at handler by the above call on empty buffer
    EXPECT_TRUE(-1 == at.read_bytes(buf, 1));

    // TEST DATA IN BUFFER
    at.clear_error();
    char table1[] = "1234512345678OK\r\n\0";
    filehandle_stub_table = table1;
    filehandle_stub_table_pos = 0;
    mbed_poll_stub::revents_value = POLLIN;
    mbed_poll_stub::int_value = 1;;

    // Read 5 bytes
    EXPECT_TRUE(5 == at.read_bytes(buf, 5));
    EXPECT_TRUE(!memcmp(buf, table1, 5));
    // get_char triggered above should have filled in the whole reading buffer(fill_buffer())
    EXPECT_TRUE(filehandle_stub_table_pos == (strlen(table1) - 1));
    // Read another 8 bytes
    EXPECT_TRUE(8 == at.read_bytes(buf, 8) && !memcmp(buf, table1 + 5, 8));
    // Reading more than the 4 bytes left -> ERROR
    EXPECT_TRUE(-1 == at.read_bytes(buf, 5));
    EXPECT_TRUE(NSAPI_ERROR_DEVICE_ERROR == at.get_last_error());
}

TEST_F(TestATHandler, test_ATHandler_read_string)
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
    EXPECT_TRUE(-1 == at.read_string(buf1, 1));
    at.clear_error();
    // Set _stop_tag to resp_stop(OKCRLF)
    at.resp_start();
    // Device error because buffer is empty
    EXPECT_TRUE(NSAPI_ERROR_DEVICE_ERROR == at.get_last_error());
    at.clear_error();
    // Device error because empty buffer and attempt to fill_buffer by consume_char('\"')
    EXPECT_TRUE(0 == at.read_string(buf1, 1));

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
    EXPECT_TRUE(NSAPI_ERROR_DEVICE_ERROR == at.get_last_error());
    at.clear_error();
    EXPECT_TRUE(0 == at.read_string(buf2, 1));

    // *** CRLF ***
    at.clear_error();
    char table3[] = "\r\n,s\r\n\0";
    at.flush();
    filehandle_stub_table = table3;
    filehandle_stub_table_pos = 0;
    mbed_poll_stub::revents_value = POLLIN;
    mbed_poll_stub::int_value = 1;
    char buf3[1];
    // Set _stop_tag to resp_stop(OKCRLF)
    at.resp_start();
    // OK because after CRLF matched there is more data to read ending in CRLF
    EXPECT_TRUE(NSAPI_ERROR_OK == at.get_last_error());
    // To read 0 bytes from: s\r\n
    EXPECT_TRUE(0 == at.read_string(buf3, 0 + 1/*for NULL*/));
    // To read 1 byte from: s\r\n -> read s
    EXPECT_TRUE(1 == at.read_string(buf3, 1 + 1/*for NULL*/));

    // *** Reading more than available in buffer ***
    at.clear_error();
    char table4[] = "\"s,\"OK\r\n\0";
    mbed_poll_stub::int_value = 0;
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
    EXPECT_TRUE(-1 == at.read_string(buf4, 4 + 1/*for NULL*/));
    EXPECT_TRUE(NSAPI_ERROR_DEVICE_ERROR == at.get_last_error());

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
    EXPECT_TRUE(1 == at.read_string(buf4, 4 + 1/*for NULL*/));

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
    EXPECT_TRUE(0 == at.read_string(buf4, 4 + 1/*for NULL*/));

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
    // TO read max 1 byte from: s"OK\r\n -> read s + read to stop_tag(OKCRLF)
    EXPECT_TRUE(1 == at.read_string(buf4, 1 + 1/*for NULL*/));

    // *** Try to read after stop tag was found  ***
    // stop tag found do not read further
    EXPECT_TRUE(-1 == at.read_string(buf4, 1 + 1/*for NULL*/));

    // *** Try to read after stop tag was found when parameter allows it  ***
    // stop tag found but flag indicates to read despite stop_tag found
    EXPECT_TRUE(4 == at.read_string(buf4, 4 + 1/*for NULL*/, true));

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
    EXPECT_TRUE(6 == at.read_string(buf4, 6 + 1/*for NULL*/));

    // *** Reading when buffer only has "  ***
    at.clear_error();
    char table7[] = "s\"\0";
    at.flush();
    filehandle_stub_table = table7;
    filehandle_stub_table_pos = 0;
    mbed_poll_stub::revents_value = POLLIN;
    mbed_poll_stub::int_value = 1;
    at.resp_start("s");
    // TO read from buffer having only " -> trying to find delimiter or stop_tag(OKCRLF)
    EXPECT_TRUE(-1 == at.read_string(buf4, 5));
    EXPECT_TRUE(NSAPI_ERROR_DEVICE_ERROR == at.get_last_error());

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
    EXPECT_TRUE(8 == at.read_string(buf8, 8 + 1/*for NULL*/));

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
    EXPECT_TRUE(6 == at.read_string(buf9, 6 + 1/*for NULL*/));

    at.clear_error();
    char table11[] = "\"1016\",\"39AB\",9\r\n\0";
    mbed_poll_stub::int_value = 0;
    at.flush();
    filehandle_stub_table = table11;
    filehandle_stub_table_pos = 0;
    mbed_poll_stub::revents_value = POLLIN;
    mbed_poll_stub::int_value = 1;
    at.resp_start();
    EXPECT_TRUE(4 == at.read_string(buf4, 4 + 1/*for NULL*/));
    EXPECT_TRUE(!strncmp(buf4, "1016", 4));
    EXPECT_TRUE(4 == at.read_string(buf4, 4 + 1/*for NULL*/));
    EXPECT_TRUE(!strncmp(buf4, "39AB", 4));
    EXPECT_TRUE(9 == at.read_int());

    // *** CRLF part of the string ***
    at.clear_error();
    char table10[] = "\"s\"\r\nOK\r\n\0";
    mbed_poll_stub::int_value = 0;
    at.flush();
    filehandle_stub_table = table10;
    filehandle_stub_table_pos = 0;
    mbed_poll_stub::revents_value = POLLIN;
    mbed_poll_stub::int_value = 1;
    char buf10[10];

    // NO prefix, NO OK, NO ERROR and NO URC match, CRLF found -> return so buffer could be read
    at.resp_start();
    // TO read from
    EXPECT_TRUE(3 == at.read_string(buf10, 9 + 1/*for NULL*/));
}

TEST_F(TestATHandler, test_ATHandler_read_hex_string)
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
    EXPECT_TRUE(5 == at.read_hex_string(buf1, 5));
    EXPECT_TRUE(!strncmp(buf1, "hello", 5));

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
    EXPECT_TRUE(5 == at.read_hex_string(buf2, 6));
    EXPECT_TRUE(!strncmp(buf2, "hello", 5));

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
    EXPECT_TRUE(2 == at.read_hex_string(buf3, 2 + 1/*get to stop tag match*/));
    EXPECT_TRUE(!strncmp(buf3, "he", 2));
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
    EXPECT_TRUE(1 == at.read_hex_string(buf4, 2 + 1/*get to stop tag match*/));
    EXPECT_TRUE(!strncmp(buf4, "h", 1));
}

TEST_F(TestATHandler, test_ATHandler_read_int)
{
    EventQueue que;
    FileHandle_stub fh1;
    filehandle_stub_table = NULL;
    filehandle_stub_table_pos = 0;

    ATHandler at(&fh1, que, 0, ",");

    int32_t ret = at.read_int();
    EXPECT_TRUE(-1 == ret);
    at.clear_error();

    char table[] = "\",\"OK\r\n\0";
    filehandle_stub_table = table;
    filehandle_stub_table_pos = 0;
    mbed_poll_stub::revents_value = POLLIN;
    mbed_poll_stub::int_value = strlen(table);

    at.resp_start();

    ret = at.read_int();
    EXPECT_TRUE(-1 == ret);
    at.flush();
    at.clear_error();

    char table2[] = "\"2,\"OK\r\n\0";
    filehandle_stub_table = table2;
    filehandle_stub_table_pos = 0;
    mbed_poll_stub::revents_value = POLLIN;
    mbed_poll_stub::int_value = strlen(table2);

    at.resp_start();

    ret = at.read_int();
    EXPECT_TRUE(2 == ret);
}

TEST_F(TestATHandler, test_ATHandler_resp_start)
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
    filehandle_stub_table_pos = 0;
    at.resp_start("ssssaaaassssaaaassss"); //too long prefix

    char table3[] = "+CME ERROR: 108\0";
    filehandle_stub_table = table3;
    filehandle_stub_table_pos = 0;

    at.flush();
    at.clear_error();
    filehandle_stub_table_pos = 0;
    at.resp_start();


    at.flush();
    at.clear_error();
    filehandle_stub_table_pos = 0;
    at.resp_start();

    char table4[] = "+CMS ERROR: 6\0";
    filehandle_stub_table = table4;

    filehandle_stub_table_pos = 0;
    at.flush();
    at.clear_error();
    filehandle_stub_table_pos = 0;
    at.resp_start();

    char table5[] = "ERROR\r\n\0";
    filehandle_stub_table = table5;
    filehandle_stub_table_pos = 0;

    at.flush();
    at.clear_error();
    filehandle_stub_table_pos = 0;
    at.resp_start();

    char table6[] = "OK\r\n\0";
    filehandle_stub_table = table6;
    filehandle_stub_table_pos = 0;

    at.flush();
    at.clear_error();
    filehandle_stub_table_pos = 0;
    at.resp_start();

    char table7[] = "urc: info\r\nresponseOK\r\n\0";
    at.flush();
    at.clear_error();
    filehandle_stub_table = table7;
    filehandle_stub_table_pos = 0;

    at.set_urc_handler("urc: ", NULL);
    at.resp_start(); // recv_buff: "responseOK\r\n\0"
    at.resp_stop();  // consumes to OKCRLF -> OK
    EXPECT_TRUE(at.get_last_error() == NSAPI_ERROR_OK);

    char table8[] = "urc: info\r\nresponse\0";
    at.flush();
    at.clear_error();
    filehandle_stub_table = table8;
    filehandle_stub_table_pos = 0;

    at.set_urc_handler("urc: ", NULL);
    at.resp_start();
    at.resp_stop();
    // No stop tag(OKCRLF) found
    EXPECT_TRUE(at.get_last_error() == NSAPI_ERROR_DEVICE_ERROR);

    char table9[] = "urc: prefix: infoOK\r\n\0";
    at.flush();
    at.clear_error();
    filehandle_stub_table = table9;
    filehandle_stub_table_pos = 0;

    at.set_urc_handler("urc: ", &urc_callback);
    at.set_urc_handler("urc: ", NULL);
    at.resp_start();
    EXPECT_EQ(at.get_last_error(), NSAPI_ERROR_OK);

    char table10[] = "urc: info\r\ngarbage\r\nprefix: info\r\nOK\r\n\0";
    at.flush();
    at.clear_error();
    filehandle_stub_table = table10;
    filehandle_stub_table_pos = 0;

    at.set_urc_handler("urc: ", NULL);
    at.resp_start("prefix"); // match URC -> consumes to first CRLF -> consumes the garbage because there is expected prefix and no match found -> then prefix match
    at.resp_stop(); //ends the info scope -> consumes to CRLF -> ends the resp scope -> consumes to OKCRLF
    EXPECT_TRUE(at.get_last_error() == NSAPI_ERROR_OK);

    // No stop tag(OKCRLF) will be found because, after match URC consumed everything to CRLF, rest of buffer
    // is consumed to next/last CRLF because there is expected prefix and no match found
    // -> nothing to read after that -> ERROR
    char table11[] = "urc: info\r\ngarbageprefix: infoOK\r\n\0";
    at.flush();
    at.clear_error();
    filehandle_stub_table = table11;
    filehandle_stub_table_pos = 0;

    at.set_urc_handler("urc: ", NULL);
    at.resp_start("prefix");
    EXPECT_TRUE(at.get_last_error() == NSAPI_ERROR_DEVICE_ERROR);

    // After URC match no prefix match -> try to read more -> no more to read
    char table12[] = "urc: infoprefix: info\0";
    at.flush();
    at.clear_error();
    filehandle_stub_table = table12;
    filehandle_stub_table_pos = 0;

    at.set_urc_handler("urc: ", NULL);
    at.resp_start("prefix");
    EXPECT_TRUE(at.get_last_error() == NSAPI_ERROR_DEVICE_ERROR);

    // Will run into mem_str check of identical strings
    char table13[] = "\r\n\r\n\0";
    at.flush();
    at.clear_error();
    filehandle_stub_table = table13;
    filehandle_stub_table_pos = 0;

    char buf[3];
    at.resp_start();
    EXPECT_TRUE(2 == at.read_string(buf, 3));
    EXPECT_TRUE(!strncmp(buf, "\r\n", 2));
    // Consume to delimiter or stop_tag OKCRLF fails -> ERROR
    EXPECT_TRUE(at.get_last_error() == NSAPI_ERROR_DEVICE_ERROR);
}

TEST_F(TestATHandler, test_ATHandler_resp_stop)
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
    filehandle_stub_table_pos = 0;
    at.resp_start();

    at.resp_stop();

    char table7[] = "ssssss\0";
    filehandle_stub_table = table7;
    filehandle_stub_table_pos = 0;

    at.flush();
    at.clear_error();
    filehandle_stub_table_pos = 0;
    at.resp_start("ss", false);
    at.resp_stop();
}

TEST_F(TestATHandler, test_ATHandler_info_resp)
{
    EventQueue que;
    FileHandle_stub fh1;

    filehandle_stub_table = NULL;

    ATHandler at(&fh1, que, 0, ",");
    EXPECT_TRUE(!at.info_resp());

    at.resp_start();
    EXPECT_TRUE(!at.info_resp());

    at.flush();
    at.clear_error();

    char table2[] = "21 OK\r\n\0";
    filehandle_stub_table = table2;
    filehandle_stub_table_pos = 0;
    mbed_poll_stub::revents_value = POLLIN;
    mbed_poll_stub::int_value = strlen(table2);

    at.resp_start("21");
    EXPECT_TRUE(at.info_resp());

    EXPECT_TRUE(!at.info_resp());

    at.flush();
    at.clear_error();

    char table3[] = "21 OK\r\n\0";
    filehandle_stub_table = table3;
    filehandle_stub_table_pos = 0;
    mbed_poll_stub::revents_value = POLLIN;
    mbed_poll_stub::int_value = strlen(table3);

    EXPECT_TRUE(at.info_resp());
}

TEST_F(TestATHandler, test_ATHandler_info_elem)
{
    EventQueue que;
    FileHandle_stub fh1;

    char table[] = "21 OK\r\n\0";
    filehandle_stub_table = table;
    filehandle_stub_table_pos = 0;
    mbed_poll_stub::revents_value = POLLIN;
    mbed_poll_stub::int_value = strlen(table);

    ATHandler at(&fh1, que, 0, ",");
    EXPECT_TRUE(!at.info_elem('O'));
    at.flush();

    char table2[] = "21 OK\r\n\0";
    filehandle_stub_table = table2;
    filehandle_stub_table_pos = 0;
    mbed_poll_stub::revents_value = POLLIN;
    mbed_poll_stub::int_value = strlen(table2);

    at.clear_error();
    at.resp_start("21");
    EXPECT_TRUE(at.info_elem('O'));
    at.flush();

    filehandle_stub_table = NULL;
    filehandle_stub_table_pos = 0;

    at.clear_error();
    at.resp_start("21");
    EXPECT_TRUE(!at.info_elem('2'));
}

TEST_F(TestATHandler, test_ATHandler_consume_to_stop_tag)
{
    EventQueue que;
    FileHandle_stub fh1;

    ATHandler at(&fh1, que, 0, ",");
    EXPECT_TRUE(at.consume_to_stop_tag());

    at.clear_error();
    char table1[] = "\r\n\r\r\r\nOOK\r\n";
    at.flush();
    filehandle_stub_table = table1;
    filehandle_stub_table_pos = 0;
    mbed_poll_stub::revents_value = POLLIN;
    mbed_poll_stub::int_value = 1;
    char buf1[6];
    at.resp_start();
    EXPECT_TRUE(at.consume_to_stop_tag());

    at.clear_error();
    char table2[] = "OKOK\r\n";
    at.flush();
    filehandle_stub_table = table2;
    filehandle_stub_table_pos = 0;
    mbed_poll_stub::revents_value = POLLIN;
    mbed_poll_stub::int_value = 1;
    char buf2[6];
    EXPECT_TRUE(at.consume_to_stop_tag());
}

TEST_F(TestATHandler, test_ATHandler_set_debug)
{
    EventQueue que;
    FileHandle_stub fh1;

    ATHandler at(&fh1, que, 0, ",");
    at.set_debug(true);

    at.set_debug(false);
}

TEST_F(TestATHandler, test_ATHandler_get_3gpp_error)
{
    EventQueue que;
    FileHandle_stub fh1;

    ATHandler at(&fh1, que, 0, ",");
    at.get_3gpp_error();
}

