/* mbed Microcontroller Library
 * Copyright (c) 2018 ARM Limited
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

#include "greentea-client/test_env.h"
#include "utest/utest.h"
#include "unity/unity.h"
#include "mbed.h"
#include "Stream.h"

/* This test suite verifies that write/read/write/read sequence can be
 * successfully executed on the Stream objects.
 *
 * A qute from C99 standard, paragraph 7.19.5.3, point 6:
 *
 * When a file is opened with update mode ('+' as the second or third character in the
 * above list of mode argument values), both input and output may be performed on the
 * associated stream. However, output shall not be directly followed by input without an
 * intervening call to the fflush function or to a file positioning function (fseek,
 * fsetpos, or rewind), and input shall not be directly followed by output without an
 * intervening call to a file positioning function, unless the input operation encounters end-
 * of-file.
 */

using utest::v1::Case;

const char FMT[] = "Foo%02ibar.";
const size_t FORMATTED_STR_SIZE = 3 + 2 + 4 + 1;
// The test Stream instance has to be able to store two printf() output strings.
const size_t LOOPBACK_BUFF_SIZE = 2 * FORMATTED_STR_SIZE;

class Loopback : public Stream {
public:
    Loopback(const char *name = NULL) : Stream(name)
    {
        // The `fgets()` stops reading after a newline or EOF.
        // Fill the buffer with newlines to simplify fgets() usage in this test.
        memset(_buff, '\n', LOOPBACK_BUFF_SIZE);
        _p_index = 0;
        _g_index = 0;
    }

    virtual ~Loopback()
    {
    }

    int test_vprintf(const char *fmt, ...)
    {
        int rc = -1;
        std::va_list args;
        va_start(args, fmt);
        rc = vprintf(fmt, args);
        va_end(args);
        return rc;
    }

    int test_vscanf(const char *fmt, ...)
    {
        int rc = EOF;
        std::va_list args;
        va_start(args, fmt);
        rc = vscanf(fmt, args);
        va_end(args);
        return rc;
    }

protected:
    virtual int _putc(int c)
    {
        if (_p_index >= LOOPBACK_BUFF_SIZE) {
            return -1;
        }
        _buff[_p_index++] = (int8_t)c;
        return c;
    }

    virtual int _getc()
    {
        if (_g_index >= LOOPBACK_BUFF_SIZE) {
            return -1;
        }
        return _buff[_g_index++];
    }

private:
    int8_t _buff[LOOPBACK_BUFF_SIZE];
    size_t _p_index;
    size_t _g_index;
};

/* Test intermixed Stream::putc() / Stream::getc().
 *
 * Given a Stream object,
 * when a write/read/write/read sequence is executed
 * with the use of Stream::putc() and Stream::getc() methods,
 * then all operations succeed.
 */
void test_putc_getc()
{
    char char_buff[2] = {'a', 'b'};
    Loopback loop("loopback");
    int ret;

    ret = loop.putc(char_buff[0]);
    TEST_ASSERT_EQUAL_INT(char_buff[0], ret);
    ret = loop.getc();
    TEST_ASSERT_EQUAL_INT(char_buff[0], ret);
    ret = loop.putc(char_buff[1]);
    TEST_ASSERT_EQUAL_INT(char_buff[1], ret);
    ret = loop.getc();
    TEST_ASSERT_EQUAL_INT(char_buff[1], ret);
}

/* Test intermixed Stream::puts() / Stream::gets().
 *
 * Given a Stream object,
 * when a write/read/write/read sequence is executed,
 * with the use of Stream::puts() and Stream::gets() methods,
 * then all operations succeed.
 */
void test_puts_gets()
{
    const size_t STR_LEN = 3;
    const size_t STR_SIZE = STR_LEN + 1; // +1 for '\0'
    char strings[2][STR_SIZE] = {"Foo", "Bar"};
    const size_t GETS_BUFF_SIZE = STR_LEN + 2; // +1 for '\n' (gets() stops AFTER a '\n'), +1 for '\0'
    char g_buff[GETS_BUFF_SIZE] = {};
    Loopback loop("loopback");
    int p_rc;
    char *g_rc;

    p_rc = loop.puts(strings[0]);
    TEST_ASSERT(p_rc >= 0);
    g_rc = loop.gets(g_buff, GETS_BUFF_SIZE);
    TEST_ASSERT_EQUAL_PTR(g_buff, g_rc);

    p_rc = loop.puts(strings[1]);
    TEST_ASSERT(p_rc >= 0);
    g_rc = loop.gets(g_buff, GETS_BUFF_SIZE);
    TEST_ASSERT_EQUAL_PTR(g_buff, g_rc);
}

/* Test intermixed Stream::printf() / Stream::scanf().
 *
 * Given a Stream object,
 * when a write/read/write/read sequence is executed,
 * with the use of Stream::printf() and Stream::scanf() methods,
 * then all operations succeed.
 */
void test_printf_scanf()
{
    Loopback loop("loopback");
    int p_val, g_val, rc;

    p_val = 42;
    g_val = p_val + 1;
    rc = loop.printf(FMT, p_val);
    TEST_ASSERT(rc > 0);
    rc = loop.scanf(FMT, &g_val);
    TEST_ASSERT(rc == 1);
    TEST_ASSERT_EQUAL_INT(p_val, g_val);

    p_val += 5;
    g_val = p_val + 1;
    rc = loop.printf(FMT, p_val);
    TEST_ASSERT(rc > 0);
    rc = loop.scanf(FMT, &g_val);
    TEST_ASSERT(rc == 1);
    TEST_ASSERT_EQUAL_INT(p_val, g_val);
}

/* Test intermixed Stream::vprintf() / Stream::vscanf().
 *
 * Given a Stream object,
 * when a write/read/write/read sequence is executed,
 * with the use of Stream::vprintf() and Stream::vscanf() methods,
 * then all operations succeed.
 */
void test_vprintf_vscanf()
{
    Loopback loop("loopback");
    int p_val, g_val, rc;

    p_val = 42;
    g_val = p_val + 1;
    rc = loop.test_vprintf(FMT, p_val);
    TEST_ASSERT(rc > 0);
    rc = loop.test_vscanf(FMT, &g_val);
    TEST_ASSERT(rc == 1);
    TEST_ASSERT_EQUAL_INT(p_val, g_val);

    p_val += 5;
    g_val = p_val + 1;
    rc = loop.test_vprintf(FMT, p_val);
    TEST_ASSERT(rc > 0);
    rc = loop.test_vscanf(FMT, &g_val);
    TEST_ASSERT(rc == 1);
    TEST_ASSERT_EQUAL_INT(p_val, g_val);
}

utest::v1::status_t test_setup(const size_t number_of_cases)
{
    GREENTEA_SETUP(10, "default_auto");
    return utest::v1::verbose_test_setup_handler(number_of_cases);
}

Case cases[] = {
    Case("Test putc/getc", test_putc_getc),
    Case("Test puts/gets", test_puts_gets),
    Case("Test printf/scanf", test_printf_scanf),
    Case("Test vprintf/vscanf", test_vprintf_vscanf)
};

utest::v1::Specification specification(test_setup, cases);

int main()
{
    return !utest::v1::Harness::run(specification);
}
