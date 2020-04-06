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
#include "CellularUtil_stub.h"

#include "Timer_stub.h"

using namespace mbed;
using namespace events;

uint8_t urc_callback_count;

void urc_callback()
{
    urc_callback_count++;
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
        urc_callback_count = 0;
        CellularUtil_stub::char_ptr = NULL;
        CellularUtil_stub::char_pos = 0;
        filehandle_stub_short_value_counter = 0;
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
    at->set_default_delimiter();
    at->use_delimiter(true);

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
    fh1.short_value = 0;

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

//This is intentional for this test, so let's disable build time warning for next line
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Woverflow"

    at.write_int(2147483647 + 1);

#pragma GCC diagnostic pop

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

    at.flush();
    at.clear_error();
    char table2[] = "sssOK\r\n\0";
    filehandle_stub_table = table2;
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
    uint8_t buf[8]; // 1st part of the test reads 5 bytes and the 2nd part 8 bytes

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
    EXPECT_EQ(5, at.read_bytes(buf, 5));
    EXPECT_TRUE(!memcmp(buf, table1, 5));
    // get_char triggered above should have filled in the whole reading buffer(fill_buffer())
    EXPECT_EQ(filehandle_stub_table_pos, (strlen(table1)));
    // Read another 8 bytes
    EXPECT_TRUE(8 == at.read_bytes(buf, 8) && !memcmp(buf, table1 + 5, 8));
    // Reading more than the 4 bytes left -> ERROR
    EXPECT_EQ(-1, at.read_bytes(buf, 5));
    EXPECT_EQ(NSAPI_ERROR_DEVICE_ERROR, at.get_last_error());
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
    at.resp_stop();

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
    at.resp_stop();

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
    char table10[] = "\"1016\",\"39AB\",9\r\n\0";
    mbed_poll_stub::int_value = 0;
    at.flush();
    filehandle_stub_table = table10;
    filehandle_stub_table_pos = 0;
    mbed_poll_stub::revents_value = POLLIN;
    mbed_poll_stub::int_value = 1;
    at.resp_start();
    EXPECT_TRUE(4 == at.read_string(buf4, 4 + 1/*for NULL*/));
    EXPECT_TRUE(!strncmp(buf4, "1016", 4));
    EXPECT_TRUE(4 == at.read_string(buf4, 4 + 1/*for NULL*/));
    EXPECT_TRUE(!strncmp(buf4, "39AB", 4));
    at.set_is_filehandle_usable(false);
    EXPECT_TRUE(-1 == at.read_int());
    at.set_is_filehandle_usable(true);
    at.clear_error();
    EXPECT_TRUE(9 == at.read_int());

    // *** CRLF part of the string ***
    at.clear_error();
    char table11[] = "\"s\"\r\nOK\r\n\0";
    mbed_poll_stub::int_value = 0;
    at.set_is_filehandle_usable(false);
    at.flush();
    at.set_is_filehandle_usable(true);
    at.clear_error();
    at.flush();
    filehandle_stub_table = table11;
    filehandle_stub_table_pos = 0;
    mbed_poll_stub::revents_value = POLLIN;
    mbed_poll_stub::int_value = 1;
    char buf11[10];

    // NO prefix, NO OK, NO ERROR and NO URC match, CRLF found -> return so buffer could be read
    at.resp_start();
    // TO read from
    EXPECT_TRUE(3 == at.read_string(buf11, 9 + 1/*for NULL*/));

    // *** Read size hits in the middle of stop tag ***
    at.clear_error();
    char table12[] = "abcdOK\r\nefg\r\n\0";
    mbed_poll_stub::int_value = 0;
    at.flush();
    filehandle_stub_table = table12;
    filehandle_stub_table_pos = 0;
    mbed_poll_stub::revents_value = POLLIN;
    mbed_poll_stub::int_value = 1;
    char buf12[7];

    at.resp_start();
    // Read size hits in the middle of OKCRLF
    EXPECT_TRUE(4 == at.read_string(buf12, 7));
    EXPECT_TRUE(!strncmp(buf12, "abcd", 4));
    // Not running into time out
    EXPECT_TRUE(NSAPI_ERROR_OK == at.get_last_error());
    // No error -> -1 returned because stop tag found already
    EXPECT_TRUE(-1 == at.read_string(buf12, 1));

    at.resp_stop();
    at.resp_start();
    EXPECT_TRUE(3 == at.read_string(buf12, 4));
    EXPECT_TRUE(!strncmp(buf12, "efg", 3));
    // No stop tag found
    EXPECT_TRUE(NSAPI_ERROR_DEVICE_ERROR == at.get_last_error());
}

TEST_F(TestATHandler, test_ATHandler_read_string_with_delimiter)
{
    EventQueue que;
    FileHandle_stub fh1;
    filehandle_stub_table = NULL;
    filehandle_stub_table_pos = 0;

    ATHandler at(&fh1, que, 0, ",");

    char table1[] = "+CCLK:\"20/04/05,15:38:57+12\"\r\nOK\r\n";
    at.flush();
    filehandle_stub_table = table1;
    filehandle_stub_table_pos = 0;
    mbed_poll_stub::revents_value = POLLIN;
    mbed_poll_stub::int_value = 1;

    char buf1[64];

    at.clear_error();
    at.resp_start("+CCLK:");
    // Device error because empty buffer and attempt to fill_buffer by consume_char('\"')
    EXPECT_EQ(20, at.read_string(buf1, sizeof(buf1)));
    EXPECT_STREQ("20/04/05,15:38:57+12", buf1);
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
    char resp[] = "hello\0";
    CellularUtil_stub::char_ptr = resp;
    CellularUtil_stub::char_pos = 0;

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
    CellularUtil_stub::char_pos = 0;
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
    CellularUtil_stub::char_pos = 0;

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
    CellularUtil_stub::char_pos = 0;

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

    char table[] = ",OK\r\n\0";
    filehandle_stub_table = table;
    filehandle_stub_table_pos = 0;
    mbed_poll_stub::revents_value = POLLIN;
    mbed_poll_stub::int_value = strlen(table);

    at.resp_start();

    ret = at.read_int();
    EXPECT_TRUE(-1 == ret);
    at.flush();
    at.clear_error();

    char table2[] = "2,OK\r\n\0";
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

    at.flush();
    at.clear_error();
    char table3[] = "+CME ERROR: 108\r\n";
    filehandle_stub_table = table3;
    filehandle_stub_table_pos = 0;

    at.resp_start();


    at.flush();
    at.clear_error();
    filehandle_stub_table_pos = 0;
    at.resp_start();

    char table4[] = "+CMS ERROR: 6\r\n";
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

    char table7[] = "urc: info\r\nresponse\r\nOK\r\n\0";
    at.flush();
    at.clear_error();
    filehandle_stub_table = table7;
    filehandle_stub_table_pos = 0;

    mbed::Callback<void()> cb1(&urc_callback);
    at.set_urc_handler("urc: ", cb1);
    at.resp_start(); // recv_buff: "responseOK\r\n\0"
    at.resp_stop();  // consumes to OKCRLF -> OK
    EXPECT_TRUE(at.get_last_error() == NSAPI_ERROR_OK);
    EXPECT_TRUE(urc_callback_count == 1);
    urc_callback_count = 0;

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

    at.flush();
    at.clear_error();

    char table3[] = "+CME ERROR: 108\r\n";
    filehandle_stub_table = table3;
    filehandle_stub_table_pos = 0;
    at.resp_start();
    at.resp_stop();

    // Set stop tag for response to CRLF -> resp stop should stop on first CRLF
    char table6[] = "line1\r\nline2\r\nOK\r\n";
    filehandle_stub_table = table6;
    filehandle_stub_table_pos = 0;

    at.flush();
    at.clear_error();
    filehandle_stub_table_pos = 0;

    at.resp_start();
    at.set_stop_tag("\r\n");
    at.resp_stop();
    EXPECT_TRUE(at.get_last_error() == NSAPI_ERROR_OK);

    char table7[] = "ssssss\0";
    filehandle_stub_table = table7;
    filehandle_stub_table_pos = 0;

    at.flush();
    at.clear_error();
    filehandle_stub_table_pos = 0;
    at.resp_start("ss", false);
    at.resp_stop();

    // prefix + URC line + some other line + URC line + URC line + OKCRLF
    char table4[] = "line1\r\nline2abcd\r\nline3abcd\r\nline4\r\n\r\nline3\r\nline3\r\nOK\r\n";
    filehandle_stub_table = table4;
    filehandle_stub_table_pos = 0;

    at.flush();
    at.clear_error();
    filehandle_stub_table_pos = 0;
    mbed::Callback<void()> cb1(&urc_callback);
    at.set_urc_handler("line3", cb1);

    at.resp_start("line2");
    at.resp_stop();
    EXPECT_TRUE(urc_callback_count == 3);
    urc_callback_count = 0;

    // URC line + prefix + URC line + some other line + URC line + URC line + some other line + OKCRLF
    char table5[] = "line1\r\nline3\r\nline2abcd\r\nline3abcd\r\nline4\r\n\r\nline3\r\nline3\r\nline4\r\nOK\r\n";
    filehandle_stub_table = table5;
    filehandle_stub_table_pos = 0;

    at.flush();
    at.clear_error();
    filehandle_stub_table_pos = 0;
    mbed::Callback<void()> cb2(&urc_callback);
    at.set_urc_handler("line3", cb2);

    at.resp_start("line2");
    at.resp_stop();
    EXPECT_TRUE(urc_callback_count == 4);
    urc_callback_count = 0;
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

TEST_F(TestATHandler, test_ATHandler_cmd_start_stop)
{
    EventQueue que;
    FileHandle_stub fh1;

    ATHandler at(&fh1, que, 0, ",");
    uint8_t byte[] = {1, 2, 3, 4};
    at.cmd_start_stop("+CREG", "=1,", "%d%s%b", 3, "test", byte, 4);
}

TEST_F(TestATHandler, test_ATHandler_at_cmd_str)
{
    EventQueue que;
    FileHandle_stub fh1;

    ATHandler at(&fh1, que, 0, ",");
    uint8_t byte[] = {1, 2, 3, 4};
    char ret[10];

    char table[] = "ssssssssssssssssssssssssssssOK\r\n\0";
    filehandle_stub_table = table;

    at.flush();
    at.clear_error();
    filehandle_stub_table_pos = 0;
    mbed_poll_stub::int_value = 1;
    fh1.short_value = POLLIN;

    EXPECT_EQ(NSAPI_ERROR_DEVICE_ERROR, at.at_cmd_str("+CREG", "=1,", ret, 10, "%d%s%b", 3, "test", byte, 4));
}

TEST_F(TestATHandler, test_ATHandler_at_cmd_int)
{
    EventQueue que;
    FileHandle_stub fh1;

    ATHandler at(&fh1, que, 0, ",");
    uint8_t byte[] = {1, 2, 3, 4};
    int ret;
    EXPECT_EQ(NSAPI_ERROR_DEVICE_ERROR, at.at_cmd_int("+CREG", "=1,", ret, "%d%s%b", 3, "test", byte, 4));
}

TEST_F(TestATHandler, test_ATHandler_at_cmd_discard)
{
    EventQueue que;
    FileHandle_stub fh1;

    ATHandler at(&fh1, que, 0, ",");
    uint8_t byte[] = {1, 2, 3, 4};

    EXPECT_EQ(NSAPI_ERROR_DEVICE_ERROR, at.at_cmd_discard("+CREG", "=1,", "%d%s%b", 3, "test", byte, 4));
}

TEST_F(TestATHandler, test_ATHandler_sync)
{
    EventQueue que;
    FileHandle_stub fh1;

    mbed_poll_stub::revents_value = 0;
    ATHandler at(&fh1, que, 0, ",");
    at.set_is_filehandle_usable(false);
    EXPECT_EQ(false, at.sync(100));
    at.set_is_filehandle_usable(true);
    EXPECT_EQ(false, at.sync(100));

    fh1.size_value = 8;
    char table[] = "+CMEE: 1\r\nOK\r\n\0";
    filehandle_stub_table = table;

    at.flush();
    at.clear_error();
    filehandle_stub_table_pos = 0;
    mbed_poll_stub::revents_value = POLLIN + POLLOUT;
    fh1.size_value = 10;
    mbed_poll_stub::int_value = 1;
    filehandle_stub_short_value_counter = 1;
    fh1.short_value = POLLIN;

    EXPECT_EQ(true, at.sync(100));
}

