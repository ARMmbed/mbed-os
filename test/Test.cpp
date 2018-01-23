/*
 * Copyright (c) 2016 ARM Limited. All rights reserved.
 * SPDX-License-Identifier: Apache-2.0
 * Licensed under the Apache License, Version 2.0 (the License); you may
 * not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an AS IS BASIS, WITHOUT
 * WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

/**
 * \file \test\Test.c
 *
 * \brief Unit tests for mbed-client-cli
 */
#include <string.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>
#include <stdarg.h>


#include "mbed-cpputest/CppUTest/TestHarness.h"
#include "mbed-cpputest/CppUTest/SimpleString.h"
#include "mbed-cpputest/CppUTest/CommandLineTestRunner.h"

#define YOTTA_CFG_MBED_TRACE 1
#define YOTTA_CFG_MBED_TRACE_FEA_IPV6 0
#include "mbed-trace/mbed_trace.h"
#include "mbed-client-cli/ns_cmdline.h"
#define MAX(x,y)   (x>y?x:y)
#define ARRAY_CMP(x, y) \
        MEMCMP_EQUAL(x, y, MAX(strlen(x), strlen(y)))

int main(int ac, char **av)
{
    return CommandLineTestRunner::RunAllTests(ac, av);
}

#define BUFSIZE 1024
char buf[BUFSIZE] = {0};
#define INIT_BUF()  memset(buf, 0, BUFSIZE)
int cmd_dummy(int argc, char *argv[])
{
    return 0;
}

int mutex_wait_count = 0;
int mutex_release_count = 0;
int mutex_count_expected_difference = 1;
bool check_mutex_lock_state = false;
void my_mutex_wait()
{
    mutex_wait_count++;
}
void my_mutex_release()
{
    mutex_release_count++;
}

void myprint(const char *fmt, va_list ap)
{
    if (check_mutex_lock_state) {
        CHECK((mutex_wait_count - mutex_release_count) == mutex_count_expected_difference);
    }
    vsnprintf(buf + strlen(buf), BUFSIZE - strlen(buf), fmt, ap);
    //printf("\nMYPRINT: %s\n", buf); //for test test
}
void input(const char *str)
{
    while (*str != 0) {
        cmd_char_input(*str++);
    }
}

#define REQUEST(x)          input(x);INIT_BUF();cmd_char_input('\r');
#define RESPONSE(x)         "\r\n" x "\r\n\r\x1B[2K/> \x1B[1D"
#define CMDLINE(x)          "\r\x1b[2K/>" x "\x1b[1D"

#define FORWARD             "C"
#define BACKWARD            "D"
#define CMDLINE_CUR(x, cursor, dir)  "\r\x1b[2K/>" x "\x1b[" cursor "" dir
#define CLEAN()             cmd_char_input('\r');INIT_BUF();

//vt100 keycodes
#define HOME()      input("\x1b[1~")
#define INSERT()    input("\x1b[2~")
#define DELETE()    input("\x1b[3~")
#define BACKSPACE() input("\x7f")
#define LEFT()      input("\x1b[D")
#define LEFT_N(n)   for(int i=0;i<n;i++) LEFT();
#define RIGHT()     input("\x1b[C")
#define RIGHT_N(n)  for(int i=0;i<n;i++) RIGHT()
#define UP()        input("\x1b[A")
#define DOWN()      input("\x1b[B")
#define ESC()       input("\x03")
#define PAGE_DOWN() input("\x1b[6~")
#define PAGE_UP()   input("\x1b[5~")

void cmd_ready_cb(int retcode)
{
    cmd_next(retcode);
}

TEST_GROUP(cli)
{
  void setup()
  {
    cmd_init(&myprint);
    cmd_set_ready_cb(cmd_ready_cb);
    INIT_BUF();
  }
  void teardown()
  {
    INIT_BUF();
    cmd_free();
  }
};

TEST(cli, init)
{
}
TEST(cli, cmd_printf_with_mutex_not_set)
{
    cmd_mutex_wait_func(0);
    cmd_mutex_release_func(0);
    int mutex_call_count_at_entry = mutex_wait_count;
    check_mutex_lock_state = false;

    cmd_printf("Hello hello!");
    STRCMP_EQUAL("Hello hello!" , buf);

    CHECK(mutex_call_count_at_entry == mutex_wait_count);
    CHECK(mutex_call_count_at_entry == mutex_release_count);

    cmd_mutex_wait_func(my_mutex_wait);
    cmd_mutex_release_func(my_mutex_release);
}
TEST(cli, cmd_printf_with_mutex_set)
{
    cmd_mutex_wait_func(my_mutex_wait);
    cmd_mutex_release_func(my_mutex_release);
    check_mutex_lock_state = true;

    cmd_printf("!olleh olleH");
    STRCMP_EQUAL("!olleh olleH" , buf);
    CHECK(mutex_wait_count == mutex_release_count);

    check_mutex_lock_state = false;
    cmd_mutex_wait_func(0);
    cmd_mutex_release_func(0);
}
TEST(cli, external_mutex_handles)
{
    cmd_mutex_wait_func(my_mutex_wait);
    cmd_mutex_release_func(my_mutex_release);
    check_mutex_lock_state = true;
    mutex_count_expected_difference = 2;

    cmd_mutex_lock();
    cmd_printf("!olleh olleH");
    STRCMP_EQUAL("!olleh olleH" , buf);
    cmd_mutex_unlock();
    CHECK(mutex_wait_count == mutex_release_count);

    mutex_count_expected_difference = 1;
    check_mutex_lock_state = false;
    cmd_mutex_wait_func(0);
    cmd_mutex_release_func(0);
}
TEST(cli, parameters_index)
{
    char *argv[] = { "cmd", "p1", "p2", "p3", "p4", "p5" };
    int idx = cmd_parameter_index(6, argv, "p4");
    CHECK_EQUAL(4, idx);

    idx = cmd_parameter_index(6, argv, "p6");
    CHECK_EQUAL(-1, idx);

    idx = cmd_parameter_index(6, argv, "p1");
    CHECK_EQUAL(1, idx);
}

TEST(cli, parameters_bools)
{
    char *argv[] =  { "cmd", "p1", "-p2", "false", "p4", "p5" };
    char *argv2[] = { "cmd", "p1", "-p2", "true",  "p4", "p5" };

    bool on, ok;
    ok = cmd_parameter_bool(6, argv, "-p2", &on);
    CHECK_EQUAL(true, ok);
    CHECK_EQUAL(false, on);

    ok = cmd_parameter_bool(6, argv2, "-p2", &on);
    CHECK_EQUAL(true, ok);
    CHECK_EQUAL(true, on);

    ok = cmd_parameter_bool(6, argv2, "p5", &on);
    CHECK_EQUAL(false, ok);
}
TEST(cli, parameters_val)
{
    bool ok;
    char *val;
    char *argv[] =  { "cmd", "p1", "p2", "p3", "p4", "p5" };

    ok = cmd_parameter_val(6, argv, "p2", &val);
    CHECK_EQUAL(true, ok);
    ARRAY_CMP("p3", val);

    ok = cmd_parameter_val(6, argv, "p3", &val);
    CHECK_EQUAL(true, ok);
    ARRAY_CMP("p4", val);

    ok = cmd_parameter_val(6, argv, "p5", &val);
    CHECK_EQUAL(false, ok);
}

TEST(cli, parameters_int)
{
    bool ok;
    int val;
    char *argv[] =  { "cmd", "p1", "p2", "3", "p4", "555fail", "p5" };

    ok = cmd_parameter_int(6, argv, "p2", &val);
    CHECK_EQUAL(true, ok);
    CHECK_EQUAL(3, val);

    ok = cmd_parameter_int(6, argv, "p1", &val);
    CHECK_EQUAL(false, ok);

    ok = cmd_parameter_int(6, argv, "p4", &val);
    CHECK_EQUAL(false, ok);

    ok = cmd_parameter_int(6, argv, "p5", &val);
    CHECK_EQUAL(false, ok);
}
TEST(cli, parameters_float)
{
    bool ok;
    float val;
    float val2 = 3.14159;
    char *argv[] =  { "cmd", "p1", "3.14159", "p3", "3.14159 ", "p4", "3.14fail ", "p5" };

    ok = cmd_parameter_float(8, argv, "p1", &val);
    CHECK_EQUAL(true, ok);
    CHECK_EQUAL(val2, val);

    ok = cmd_parameter_float(8, argv, "p3", &val);
    CHECK_EQUAL(true, ok);
    CHECK_EQUAL(val2, val);

    ok = cmd_parameter_float(8, argv, "p4", &val);
    CHECK_EQUAL(false, ok);

    ok = cmd_parameter_float(8, argv, "p5", &val);
    CHECK_EQUAL(false, ok);
}
TEST(cli, cmd_parameter_last)
{
    char *argv[] =  { "cmd", "p1", "p2", "3", "p4", "p5" };
    CHECK_EQUAL(cmd_parameter_last(6, argv), "p5");
}
TEST(cli, cmd_has_option)
{
    char *argv[] =  { "cmd", "-p", "p2", "3", "p4", "p5" };
    CHECK_EQUAL(cmd_has_option(6, argv, "-p"), true);
}
TEST(cli, help)
{
    REQUEST("help");
    CHECK(strlen(buf) > 20 );
}
TEST(cli, hello)
{
    REQUEST("echo Hi!");
    ARRAY_CMP(RESPONSE("Hi! ") , buf);
}
TEST(cli, cmd_echo1)
{
    REQUEST(" echo Hi!");
    ARRAY_CMP(RESPONSE("Hi! ") , buf);
}
TEST(cli, cmd_echo2)
{
    REQUEST("echo foo faa");
    ARRAY_CMP(RESPONSE("foo faa ") , buf);
}
TEST(cli, cmd_echo3)
{
    REQUEST("echo foo   faa");
    ARRAY_CMP(RESPONSE("foo faa ") , buf);
}
TEST(cli, cmd_echo4)
{
    REQUEST("echo   foo   faa");
    ARRAY_CMP(RESPONSE("foo faa ") , buf);
}
TEST(cli, cmd_echo5)
{
    REQUEST("echo   \"foo   faa\"");
    ARRAY_CMP(RESPONSE("foo   faa ") , buf);
}
TEST(cli, cmd_echo6)
{
    REQUEST("echo   \"foo   faa");
    ARRAY_CMP(RESPONSE("\"foo faa ") , buf);
}
TEST(cli, cmd_echo7)
{
    REQUEST("echo   'foo   faa\"");
    ARRAY_CMP(RESPONSE("'foo faa\" ") , buf);
}
TEST(cli, cmd_echo8)
{
    REQUEST("echof\x7f foo   faa");
    ARRAY_CMP(RESPONSE("foo faa ") , buf);
}
TEST(cli, cmd_echo9)
{
    REQUEST("echo foo   faa\x1b[D\x1b[D\x1b[D hello ");
    ARRAY_CMP(RESPONSE("foo hello faa ") , buf);
    CLEAN();
}
TEST(cli, cmd_echo10)
{
    REQUEST("echo foo   faa\x1b[D\x1b[C\x1b[C  hello "); //echo foo    hello faa
    ARRAY_CMP(RESPONSE("foo faa hello ") , buf);
    CLEAN();
}
TEST(cli, cmd_echo11)
{
    REQUEST("echo off\r");
    INIT_BUF();
    input("echo test");
    ARRAY_CMP("" , buf);
    input("\r");
    ARRAY_CMP("test \r\n" , buf);
    INIT_BUF();
    REQUEST("echo on\r");
    INIT_BUF();
    input("e");
    ARRAY_CMP(CMDLINE("e ") , buf);
    INIT_BUF();
    input("c");
    ARRAY_CMP(CMDLINE("ec ") , buf);
    INIT_BUF();
    input("h");
    ARRAY_CMP(CMDLINE("ech ") , buf);
    INIT_BUF();
    input("o");
    ARRAY_CMP(CMDLINE("echo ") , buf);
    INIT_BUF();
    input(" ");
    ARRAY_CMP(CMDLINE("echo  ") , buf);
    INIT_BUF();
    input("o");
    ARRAY_CMP(CMDLINE("echo o ") , buf);
    INIT_BUF();
    input("k");
    ARRAY_CMP(CMDLINE("echo ok ") , buf);
    CLEAN();
}

TEST(cli, cmd_arrows_up)
{
    REQUEST("echo foo-1");
    INIT_BUF();
    input("\x1b[A");
    ARRAY_CMP(CMDLINE("echo foo-1 ") , buf);
    INIT_BUF();
    input("\x1b[A");
    ARRAY_CMP(CMDLINE("echo foo-1 ") , buf);
    CLEAN();
}
TEST(cli, cmd_arrows_up_down)
{
    REQUEST("echo test-1");
    ARRAY_CMP(RESPONSE("test-1 "), buf);
    REQUEST("echo test-2");
    ARRAY_CMP(RESPONSE("test-2 "), buf);
    REQUEST("echo test-3");
    ARRAY_CMP(RESPONSE("test-3 "), buf);

    INIT_BUF();
    UP();
    ARRAY_CMP(CMDLINE("echo test-3 "), buf);
    INIT_BUF();
    UP();
    ARRAY_CMP(CMDLINE("echo test-2 "), buf);
    INIT_BUF();
    UP();
    ARRAY_CMP(CMDLINE("echo test-1 "), buf);
    INIT_BUF();
    UP();
    ARRAY_CMP(CMDLINE("echo test-1 "), buf);
    INIT_BUF();
    DOWN();
    ARRAY_CMP(CMDLINE("echo test-2 "), buf);
    INIT_BUF();
    DOWN();
    ARRAY_CMP(CMDLINE("echo test-3 "), buf);
    INIT_BUF();
    DOWN();
    ARRAY_CMP(CMDLINE(" "), buf);
    CLEAN();
}
TEST(cli, cmd_pageup_page_down)
{
    //goto history beginning/end
    REQUEST("echo test-1");
    REQUEST("echo test-2");
    REQUEST("echo test-3");
    REQUEST("echo test-4");
    INIT_BUF();
    PAGE_UP();
    ARRAY_CMP(CMDLINE("echo test-1 "), buf);
    INIT_BUF();
    PAGE_DOWN();
    ARRAY_CMP(CMDLINE("echo test-4 "), buf);
    CLEAN();
}
TEST(cli, cmd_text_pageup)
{
    REQUEST("echo test-1");
    REQUEST("echo test-2");
    REQUEST("echo test-3");
    REQUEST("echo test-4");
    input("hello");
    INIT_BUF();
    PAGE_UP(); //goto end of history
    ARRAY_CMP(CMDLINE("echo test-1 "), buf);
    INIT_BUF();
    PAGE_DOWN(); //goto beginning of history - it should be just writted "hello"
    ARRAY_CMP(CMDLINE("hello "), buf);
    CLEAN();
}
TEST(cli, cmd_text_pageup_up)
{
    REQUEST("echo test-1");
    REQUEST("echo test-2");
    REQUEST("echo test-3");
    REQUEST("echo test-4");
    input("hello");
    INIT_BUF();
    PAGE_UP(); //goto end of history
    ARRAY_CMP(CMDLINE("echo test-1 "), buf);
    INIT_BUF();
    DOWN();
    ARRAY_CMP(CMDLINE("echo test-2 "), buf);
    INIT_BUF();
    PAGE_DOWN(); //goto beginning of history - it should be just writted "hello"
    ARRAY_CMP(CMDLINE("hello "), buf);
    CLEAN();
}
TEST(cli, cmd_text_delete)
{
    input("hello world");
    LEFT_N(2);
    DELETE();
    INIT_BUF();
    DELETE();
    ARRAY_CMP(CMDLINE("hello wor "), buf);
    INIT_BUF();
    DELETE();
    ARRAY_CMP(CMDLINE("hello wor "), buf);
    INIT_BUF();
    DELETE();
    ARRAY_CMP(CMDLINE("hello wor "), buf);
    LEFT_N(2);
    INIT_BUF();
    DELETE();
    ARRAY_CMP(CMDLINE_CUR("hello wr ", "2", BACKWARD), buf);
    BACKSPACE();
    BACKSPACE();
    INIT_BUF();
    BACKSPACE();
    ARRAY_CMP(CMDLINE_CUR("hellr ", "2", BACKWARD), buf);
    CLEAN();
}
TEST(cli, cmd_insert)
{
    CLEAN();
    input("echo hello word");
    LEFT();
    INIT_BUF();
    input("l");
    ARRAY_CMP(CMDLINE_CUR("echo hello world ", "2", BACKWARD), buf);
    LEFT_N(10);
    INIT_BUF();
    LEFT();
    ARRAY_CMP(CMDLINE_CUR("echo hello world ", "13", BACKWARD), buf);
    INIT_BUF();
    RIGHT();
    ARRAY_CMP(CMDLINE_CUR("echo hello world ", "12", BACKWARD), buf);
    CLEAN();
}
TEST(cli, cmd_tab_1)
{
    INIT_BUF();
    input("e");
    INIT_BUF();
    input("\t");
    ARRAY_CMP(CMDLINE_CUR("echo ", "1", BACKWARD) , buf);

    input("\rech");
    INIT_BUF();
    input("\t");
    ARRAY_CMP(CMDLINE_CUR("echo ", "1", BACKWARD) , buf);

    input("\r");
}
TEST(cli, cmd_tab_2)
{
    INIT_BUF();

    cmd_add("role", cmd_dummy, 0, 0);
    cmd_add("route", cmd_dummy, 0, 0);
    cmd_add("rile", cmd_dummy, 0, 0);
    input("r");
    INIT_BUF();

    input("\t");
    ARRAY_CMP(CMDLINE_CUR("role ", "1", BACKWARD) , buf);

    INIT_BUF();
    input("\t");
    ARRAY_CMP(CMDLINE_CUR("route ", "1", BACKWARD) , buf);

    INIT_BUF();
    input("\t");
    ARRAY_CMP(CMDLINE_CUR("rile ", "1", BACKWARD) , buf);

    INIT_BUF();
    input("\x1b[Z");
    ARRAY_CMP(CMDLINE_CUR("route ", "1", BACKWARD) , buf);

    INIT_BUF();
    input("\x1b[Z");
    ARRAY_CMP(CMDLINE_CUR("role ", "1", BACKWARD) , buf);

    input("\r");
}
TEST(cli, cmd_tab_3)
{
    INIT_BUF();
    cmd_add("role", cmd_dummy, 0, 0);
    cmd_alias_add("rose", "role");
    cmd_alias_add("rope", "rope");

    input("r");

    INIT_BUF();
    input("\t");
    ARRAY_CMP(CMDLINE_CUR("role ", "1", BACKWARD) , buf);

    INIT_BUF();
    input("\t");
    ARRAY_CMP(CMDLINE_CUR("rose ", "1", BACKWARD) , buf);

    INIT_BUF();
    input("\t");
    ARRAY_CMP(CMDLINE_CUR("rope ", "1", BACKWARD) , buf);

    INIT_BUF();
    input("\t");
    ARRAY_CMP(CMDLINE_CUR("r ", "1", BACKWARD) , buf);

    INIT_BUF();
    input("o");
    ARRAY_CMP(CMDLINE_CUR("ro ", "1", BACKWARD) , buf);

    ESC();
    INIT_BUF();
}
TEST(cli, cmd_tab_4)
{
    INIT_BUF();
    cmd_variable_add("dut1", "hello");

    input("e");

    INIT_BUF();
    input("\t");
    ARRAY_CMP(CMDLINE_CUR("echo ", "1", BACKWARD) , buf);

    input(" $d");
    INIT_BUF();
    input("u");
    ARRAY_CMP(CMDLINE_CUR("echo $du ", "1", BACKWARD), buf);

    INIT_BUF();
    input("\t");
    ARRAY_CMP(CMDLINE_CUR("echo $dut1 ", "1", BACKWARD) , buf);

    input("\re");
    INIT_BUF();
    input("\t");
    ARRAY_CMP(CMDLINE_CUR("echo ", "1", BACKWARD) , buf);

    input("\r");
    INIT_BUF();
}
// // alias test
// extern void replace_alias(const char *str, const char *old_str, const char *new_str);
// TEST(cli, cmd_alias_1)
// {
//     char str[] = "hello a men";
//     replace_alias(str, "a", "b");
//     ARRAY_CMP("hello a men", str);
// 
//     replace_alias(str, "hello", "echo");
//     ARRAY_CMP("echo a men", str);
//     INIT_BUF();
// }
/* @todo this not working yet
TEST(cli, cmd_alias_2)
{
    REQUEST("alias foo bar");
    INIT_BUF();
    REQUEST("alias");
    ARRAY_CMP("\r\nalias:\r\nfoo               'bar'\r\n\r\x1b[2K/> \x1b[1D", buf);

    REQUEST("alias foo");
    INIT_BUF();
    REQUEST("alias");
    ARRAY_CMP("\r\nalias:\r\n\r\x1b[2K/> \x1b[1D", buf);
}
*/
TEST(cli, cmd_alias_3)
{
    cmd_alias_add("p", "echo");
    REQUEST("p toimii");
    ARRAY_CMP("\r\ntoimii \r\n\r\x1b[2K/> \x1b[1D", buf);

    cmd_alias_add("printtti", "echo");
    REQUEST("printtti toimii");
    ARRAY_CMP("\r\ntoimii \r\n\r\x1b[2K/> \x1b[1D", buf);
}
TEST(cli, cmd_alias_4)
{
    REQUEST("alias dut1 \"echo dut1\"");
    REQUEST("alias dut2 \"echo dut2\"");
    REQUEST("alias dut3 \"echo dut3\"");
    REQUEST("dut1");
    ARRAY_CMP(RESPONSE("dut1 "), buf);
}
TEST(cli, cmd_series)
{
    REQUEST("alias dut1 \"echo dut1\"");
    REQUEST("alias dut2 \"echo dut2\"");
    REQUEST("alias dut3 \"echo dut3\"");
    REQUEST("dut1;dut2;dut3");
    ARRAY_CMP(RESPONSE("dut1 \r\ndut2 \r\ndut3 "), buf);
}

TEST(cli, cmd_var_1)
{
    REQUEST("set foo \"bar test\"");
    INIT_BUF();
    REQUEST("set");
    ARRAY_CMP("\r\nvariables:\r\nfoo               'bar test'\r\n\r\x1b[2K/> \x1b[1D", buf);

    REQUEST("set foo");
    INIT_BUF();
    REQUEST("set");
    ARRAY_CMP("\r\nvariables:\r\n\r\x1b[2K/> \x1b[1D", buf);
}
TEST(cli, cmd_var_2)
{
    REQUEST("set foo \"hello world\"");
    REQUEST("echo foo");
    ARRAY_CMP(RESPONSE("foo ") , buf);

    REQUEST("echo $foo");
    ARRAY_CMP(RESPONSE("hello world ") , buf);

    REQUEST("set faa !");
    REQUEST("echo $foo$faa");
    ARRAY_CMP(RESPONSE("hello world! ") , buf);
}
TEST(cli, multiple_cmd)
{
    REQUEST("set foo \"hello world\";echo $foo");
    ARRAY_CMP(RESPONSE("hello world ") , buf);

    REQUEST("setd faa \"hello world\";echo $faa");
    ARRAY_CMP("\r\nCommand 'setd' not found.\r\n$faa \r\n\r\x1B[2K/> \x1B[1D" , buf);

    REQUEST("setd foo \"hello guy\"&&echo $foo");
    ARRAY_CMP(RESPONSE("Command 'setd' not found.") , buf);
}
TEST(cli, maxlength)
{
    int i;
    char test_data[600];
    char *ptr = test_data;
    strcpy(test_data, "echo ");
    for (i = 5; i < 600; i++) {
        test_data[i] = 'A' + i % 26;
    }
    test_data[599] = 0;
    REQUEST(ptr);
    //ARRAY_CMP( RESPONSE((test_data+5)), buf);
}
TEST(cli, ampersand)
{
    REQUEST("echo hello world&");
    ARRAY_CMP(RESPONSE("hello world ") , buf);
}

#define REDIR_DATA "echo Hi!"
#define PASSTHROUGH_BUF_LENGTH 10
char passthrough_buffer[PASSTHROUGH_BUF_LENGTH];
char* passthrough_ptr = NULL;
void passthrough_cb(uint8_t c)
{
    if (passthrough_ptr != NULL) {
        *passthrough_ptr++ = c;
    }
}
TEST(cli, passthrough_set)
{
    passthrough_ptr = passthrough_buffer;
    memset(&passthrough_buffer, 0, PASSTHROUGH_BUF_LENGTH);
    INIT_BUF();

    cmd_input_passthrough_func(passthrough_cb);
    input(REDIR_DATA);

    CHECK(strlen(buf) == 0);
    ARRAY_CMP(REDIR_DATA, passthrough_buffer);

    cmd_input_passthrough_func(NULL);

    REQUEST(REDIR_DATA);
    ARRAY_CMP(RESPONSE("Hi! ") , buf);
}


TEST(cli, cmd_out_func_set_null)
{
    cmd_out_func(NULL);
}

static int outf_called = 0;
void outf(const char *fmt, va_list ap) {
    outf_called++;
}
TEST(cli, cmd_out_func_set)
{
    outf_called = 0;
    cmd_out_func(&outf);
    cmd_vprintf(NULL, NULL);
    CHECK_EQUAL(outf_called, 1);
}

TEST(cli, cmd_ctrl_func_set_null)
{
    cmd_ctrl_func(NULL);
}

TEST(cli, cmd_delete_null)
{
    cmd_delete(NULL);
}

TEST(cli, cmd_history_size_set)
{
    cmd_history_size(0);
    CHECK_EQUAL(cmd_history_size(1), 1);
}

TEST(cli, cmd_add_invalid_params)
{
    cmd_add(NULL, cmd_dummy, NULL, NULL);
    cmd_add("", cmd_dummy, NULL, NULL);
    cmd_add("abc", NULL, NULL, NULL);
}
