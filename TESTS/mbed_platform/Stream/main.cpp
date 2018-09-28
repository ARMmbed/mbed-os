/* mbed Microcontroller Library
 * Copyright (c) 2018 ARM Limited
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

using utest::v1::Case;

class Loopback : public Stream {
public:
    Loopback(const char *name = NULL) : Stream(name) {}

protected:
    virtual int _getc()
    {
        return _c;
    }
    virtual int _putc(int c)
    {
        _c = c;
        return c;
    }
private:
    char _c;
};

Loopback loop("loopback");

void test_putc_getc()
{
    int ret;
    char char_buf[2] = {'a', 'b'};

    ret = loop.putc(char_buf[0]);
    TEST_ASSERT_EQUAL_INT(char_buf[0], ret);
    ret = loop.getc();
    TEST_ASSERT_EQUAL_INT(char_buf[0], ret);
    ret = loop.putc(char_buf[1]);
    TEST_ASSERT_EQUAL_INT(char_buf[1], ret);
    ret = loop.getc();
    TEST_ASSERT_EQUAL_INT(char_buf[1], ret);
    return;
}

utest::v1::status_t test_setup(const size_t number_of_cases)
{
    GREENTEA_SETUP(10, "default_auto");
    return utest::v1::verbose_test_setup_handler(number_of_cases);
}

Case cases[] = {
    Case("Test putc/getc", test_putc_getc)
};

utest::v1::Specification specification(test_setup, cases);

int main()
{
    return !utest::v1::Harness::run(specification);
}
