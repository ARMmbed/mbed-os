/*
 * Copyright (c) 2019, Arm Limited and affiliates.
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
#include "ATCmdParser.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include "FileHandle_stub.h"
#include "mbed_poll_stub.h"
#include "Timer_stub.h"

using namespace mbed;

static bool expected_oob_callback = false;

void urc_callback()
{
    EXPECT_TRUE(expected_oob_callback);
}


// AStyle ignored as the definition is not clear due to preprocessor usage
// *INDENT-OFF*
class test_ATCmdParser : public testing::Test {
protected:

    void SetUp()
    {
        filehandle_stub_short_value_counter = 0;
    }

    void TearDown()
    {
    }
};
// *INDENT-ON*


TEST_F(test_ATCmdParser, test_ATCmdParser_create)
{
    FileHandle_stub fh1;

    ATCmdParser *at = new ATCmdParser(&fh1, ",");
    at->set_delimiter("\r");
    at->set_timeout(5000);
    at->debug_on(1);

    delete at;

    at = new ATCmdParser(&fh1, "\r");

    EXPECT_TRUE(at != NULL);
    delete at;
}

TEST_F(test_ATCmdParser, test_ATCmdParser_set_timeout)
{
    FileHandle_stub fh1;

    ATCmdParser at(&fh1, ",");
    at.set_timeout(8);
    at.set_timeout(80);
}



TEST_F(test_ATCmdParser, test_ATCmdParser_process_oob)
{
    FileHandle_stub fh1;

    ATCmdParser at(&fh1);
    at.set_timeout(10);

    at.process_oob();

    filehandle_stub_short_value_counter = 1;
    fh1.short_value = POLLIN;
    at.oob("s", &urc_callback);
    at.process_oob();

    filehandle_stub_short_value_counter = 2;
    at.process_oob();

    char buf[5] = {0};
    char table[] = "ssssssssssssssssssssssssssssssss\0";
    filehandle_stub_table = table;
    filehandle_stub_table_pos = 0;
    mbed_poll_stub::revents_value = POLLIN;
    mbed_poll_stub::int_value = 1;
    at.read(buf, 5);

    filehandle_stub_short_value_counter = 2;
    expected_oob_callback = true;
    at.process_oob();
    expected_oob_callback = false;

    timer_stub_value = 0;
    filehandle_stub_table_pos = 0;
    at.read(buf, 5);

    filehandle_stub_short_value_counter = 1;
    expected_oob_callback = true;
    at.process_oob();
    expected_oob_callback = false;

    char table2[5];
    table2[0] = '\r';
    table2[1] = '\r';
    table2[2] = '\n';
    table2[3] = '\n';
    table2[4] = 0;
    filehandle_stub_table = table2;

    timer_stub_value = 0;
    filehandle_stub_table_pos = 0;
    mbed_poll_stub::revents_value = POLLIN;
    mbed_poll_stub::int_value = 1;
    at.read(buf, 1);

    filehandle_stub_short_value_counter = 1;
    at.process_oob();

    filehandle_stub_table = table;

    filehandle_stub_short_value_counter = 0;
    filehandle_stub_table_pos = 0;
    filehandle_stub_table = NULL;
}



TEST_F(test_ATCmdParser, test_ATCmdParser_flush)
{
    FileHandle_stub fh1;

    ATCmdParser at(&fh1, ",");
    filehandle_stub_short_value_counter = 1;
    fh1.short_value = POLLIN;
    at.flush();
}

TEST_F(test_ATCmdParser, test_ATCmdParser_write)
{
    FileHandle_stub fh1;

    ATCmdParser at(&fh1, ",");
    fh1.size_value = -1;
    EXPECT_TRUE(-1 == at.write("help", 4));

    mbed_poll_stub::revents_value = POLLOUT;
    mbed_poll_stub::int_value = 1;
    fh1.size_value = -1;
    EXPECT_TRUE(-1 == at.write("help", 4));

    mbed_poll_stub::revents_value = POLLOUT;
    mbed_poll_stub::int_value = 1;
    fh1.size_value = 7;
    EXPECT_EQ(4, at.write("help", 4));
}

TEST_F(test_ATCmdParser, test_ATCmdParser_set_delimiter)
{
    FileHandle_stub fh1;

    ATCmdParser at(&fh1, ",");
    at.set_delimiter("+");
}

TEST_F(test_ATCmdParser, test_ATCmdParser_read)
{
    FileHandle_stub fh1;
    filehandle_stub_table = NULL;
    filehandle_stub_table_pos = 0;

    ATCmdParser at(&fh1, ",");
    char buf[8] = {0};
    

    // TEST EMPTY BUFFER
    // Shouldn't read any byte since buffer is empty
    EXPECT_TRUE(-1 == at.read(buf, 1));
    // Return error due to error set to at handler by the above call on empty buffer
    EXPECT_TRUE(-1 == at.read(buf, 1));

    // TEST DATA IN BUFFER
    char table1[] = "1234512345678OK\r\n\0";
    filehandle_stub_table = table1;
    filehandle_stub_table_pos = 0;
    mbed_poll_stub::revents_value = POLLIN;
    mbed_poll_stub::int_value = 1;

    // Read 5 bytes
    EXPECT_EQ(5, at.read(buf, 5));
    EXPECT_TRUE(!memcmp(buf, table1, 5));
    // get_char triggered above should have filled in the whole reading buffer
    EXPECT_EQ(filehandle_stub_table_pos, (strlen(buf)));
    // Read another 8 bytes
    EXPECT_TRUE(8 == at.read(buf, 8) && !memcmp(buf, table1 + 5, 8));
    // Reading more than the 4 bytes left -> ERROR
    EXPECT_EQ(-1, at.read(buf, 5));
}

TEST_F(test_ATCmdParser, test_ATCmdParser_debug_on)
{
    FileHandle_stub fh1;

    ATCmdParser at(&fh1, ",");
    at.debug_on(true);
    at.debug_on(false);
}

TEST_F(test_ATCmdParser, test_ATCmdParser_abort)
{
    FileHandle_stub fh1;

    ATCmdParser at(&fh1, ",");
    at.abort();
}

TEST_F(test_ATCmdParser, test_ATCmdParser_printf)
{
    FileHandle_stub fh1;

    ATCmdParser at(&fh1);
    at.flush();
    at.debug_on(true);

    const char * format = "TEST %d %s %x %c TEST \r\r\n\n";
    mbed_poll_stub::revents_value = POLLOUT;
    mbed_poll_stub::int_value = 1;
    fh1.size_value = 64;

    EXPECT_EQ(22, at.printf(format, 5, "t", 0x5, 't'));

    fh1.size_value = -1;
    EXPECT_EQ(-1, at.printf(format, 5, "t", 0x5, 't'));
}

TEST_F(test_ATCmdParser, test_ATCmdParser_send)
{
    FileHandle_stub fh1;

    ATCmdParser at(&fh1);
    at.flush();
    at.debug_on(true);

    const char * format = "TEST %d %s %x %c TEST \r\r\n\n";
    mbed_poll_stub::revents_value = POLLOUT;
    mbed_poll_stub::int_value = 1;
    fh1.size_value = 64;

    //VALID printf
    EXPECT_TRUE(at.send(format, 5, "t", 0x5, 't'));
    EXPECT_TRUE(at.send(""));

    fh1.size_value = -1;
    EXPECT_FALSE(at.send(format, 5, "t", 0x5, 't'));
    EXPECT_FALSE(at.send(""));
}

TEST_F(test_ATCmdParser, test_ATCmdParser_recv)
{
    FileHandle_stub fh1;
    ATCmdParser at(&fh1, "\r", 8);

    //parse valid char
    char table1[] = "s";
    at.flush();
    filehandle_stub_table = table1;
    filehandle_stub_table_pos = 0;
    mbed_poll_stub::revents_value = POLLIN;
    mbed_poll_stub::int_value = 1;
    char c;
    EXPECT_TRUE(at.recv("%c", &c));
    EXPECT_EQ(c, 's');

    //too large response
    char table2[] = "1234567890\n";
    at.flush();
    filehandle_stub_table = table2;
    filehandle_stub_table_pos = 0;
    mbed_poll_stub::revents_value = POLLIN;
    mbed_poll_stub::int_value = 1;
    EXPECT_FALSE(at.recv("123456789%c\n", &c));

    //back to normal buffer
    ATCmdParser at1(&fh1);
    at1.flush();
    filehandle_stub_table = table2;
    filehandle_stub_table_pos = 0;
    mbed_poll_stub::revents_value = POLLIN;
    mbed_poll_stub::int_value = 1;
    EXPECT_TRUE(at1.recv("123456789%c\n", &c));

    char table3[] = "s\r\n\0";
    at1.flush();
    filehandle_stub_table = table3;
    filehandle_stub_table_pos = 0;
    mbed_poll_stub::revents_value = POLLIN;
    mbed_poll_stub::int_value = 1;
    EXPECT_TRUE(at.recv("%c", &c));
    EXPECT_EQ(c, 's');

    char table4[] = "s 1 E test\r\n";
    char text[5];
    int hexval;
    int intval;
    at1.flush();
    filehandle_stub_table = table4;
    filehandle_stub_table_pos = 0;
    mbed_poll_stub::revents_value = POLLIN;
    mbed_poll_stub::int_value = 1;
    EXPECT_TRUE(at1.recv("%c %d %x %s", &c, &intval, &hexval, &text));
    EXPECT_EQ(c, 's');
    EXPECT_EQ(intval, 1);
    EXPECT_EQ(hexval, 0xE);
    EXPECT_EQ(!memcmp(text, "test", 4), 0);

    char table5[] = "s 1 E test\r\nt 2 F tes2\r\n";
    char c1;
    char text1[5];
    int hexval1;
    int intval1;
    at1.flush();
    filehandle_stub_table = table5;
    filehandle_stub_table_pos = 0;
    mbed_poll_stub::revents_value = POLLIN;
    mbed_poll_stub::int_value = 1;

    expected_oob_callback = true;
    at1.oob("s", &urc_callback);
    EXPECT_TRUE(at1.recv("%c %d %x %s\r\n%c %d %x %s\r\n", &c, &intval, &hexval, &text, &c, &intval, &hexval, &text));
    expected_oob_callback = false;
    EXPECT_EQ(c, 't');
    EXPECT_EQ(intval, 2);
    EXPECT_EQ(hexval, 0xF);
    EXPECT_EQ(memcmp(text, "tes2", 4), 0);

}

TEST_F(test_ATCmdParser, test_ATCmdParser_scanf)
{
    FileHandle_stub fh1;
    ATCmdParser at(&fh1, "\r");

    //parse valid char
    char table1[] = "s";
    at.flush();
    filehandle_stub_table = table1;
    filehandle_stub_table_pos = 0;
    mbed_poll_stub::revents_value = POLLIN;
    mbed_poll_stub::int_value = 1;
    char c;
    EXPECT_TRUE(at.scanf("%c", &c));
    EXPECT_EQ(c, 's');

    //back to normal buffer
    char table2[] = "1234567890\n";
    at.flush();
    filehandle_stub_table = table2;
    filehandle_stub_table_pos = 0;
    mbed_poll_stub::revents_value = POLLIN;
    mbed_poll_stub::int_value = 1;
    EXPECT_TRUE(at.scanf("123456789%c\n", &c));

    char table3[] = "s\r\n\0";
    at.flush();
    filehandle_stub_table = table3;
    filehandle_stub_table_pos = 0;
    mbed_poll_stub::revents_value = POLLIN;
    mbed_poll_stub::int_value = 1;
    EXPECT_TRUE(at.scanf("%c", &c));
    EXPECT_EQ(c, 's');

    char table4[] = "s 1 E test\r\n";
    char text[5];
    memset(text, 0, 5);
    int hexval;
    int intval;
    at.flush();
    filehandle_stub_table = table4;
    filehandle_stub_table_pos = 0;
    mbed_poll_stub::revents_value = POLLIN;
    mbed_poll_stub::int_value = 1;
    EXPECT_TRUE(at.scanf("%c %d %x %s", &c, &intval, &hexval, &text));
    EXPECT_EQ(c, 's');
    EXPECT_EQ(intval, 1);
    EXPECT_EQ(hexval, 0xE);
    EXPECT_EQ(!memcmp(text, "test", 4), 0);

}


