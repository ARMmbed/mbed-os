/*
 * Copyright (c) 2015 ARM Limited. All rights reserved.
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
#include "CppUTest/TestHarness.h"
#include "test_m2mstring.h"
#include <stdio.h>
#include <string.h>

Test_M2MString::Test_M2MString()
{
    str = new String("test");
}

Test_M2MString::~Test_M2MString()
{
    delete str;
}

void Test_M2MString::test_copy_constructor()
{
    String s("name");
    String s1(s);
    CHECK(s1.p[1] == 'a');
}

void Test_M2MString::test_operator_assign()
{
    String s("name");
    String s1 = s;
    const String s2("yes");
    CHECK(s1.p[1] == 'a');

    s1 = "yeb";
    CHECK(s1.p[1] == 'e');
    s = s2;
    CHECK(s.p[1] == 'e');
}

void Test_M2MString::test_operator_add()
{
    String s("name");
    String s1("yeb");

    s += 'r';
    CHECK(s.p[4] == 'r');

    s += s1;
    CHECK(s.p[5] == 'y');

    s += "hoi";
    CHECK(s.p[9] == 'o');

    s1 += "somethingverylongggggggg";

}

void Test_M2MString::test_push_back()
{
    String s("name");

    s.push_back('r');
    CHECK(s.p[4] == 'r');
}

void Test_M2MString::test_operator_equals()
{
    String s("name");
    String s1("yeb");

    CHECK( (s == s1) == false);
    CHECK( (s == "name") == true);
    const char c = NULL;
    CHECK( (s == c) == false);
    free(s.p);
    s.p = NULL;
    CHECK( (s == c) == true);
}

void Test_M2MString::test_clear()
{
    String s("name");

    s.clear();
    CHECK(s.size_ == 0);
}

void Test_M2MString::test_size()
{
    String s("name");
    String s1("yeb");

    CHECK(s.size() == 4);
}

void Test_M2MString::test_length()
{
    String s("name");
    String s1("yeb");

    CHECK(s.length() == 4);
}

void Test_M2MString::test_capacity()
{
    String s("name");
    String s1("yeb");

    CHECK(s.capacity() == 4);
}

void Test_M2MString::test_empty()
{
    String s("name");
    String s1("yeb");

    CHECK(s.empty() == false);
}

void Test_M2MString::test_c_str()
{
    String s("name");
    String s1("yeb");

    CHECK( s.c_str() != NULL);
}

void Test_M2MString::test_reserve()
{
    String s("name");
    String s1("yeb");

    s.reserve(12);
    CHECK(s.allocated_ == 13);
}

void Test_M2MString::test_resize()
{
    String s("name");
    String s1("yeb");

    s.resize(2);
    CHECK( s.size() == 2);

    s.resize(12);
    CHECK( s.size() == 12);
}

void Test_M2MString::test_swap()
{
    String s("name");
    String s1("yeb");

    s.swap(s1);
    CHECK(s1.p[1] == 'a');

    CHECK(s.p[1] == 'e');
}

void Test_M2MString::test_substr()
{
    String s("name");
    String s1("yeb");

    CHECK(s.substr(1, 1) == "a");
    CHECK(s.substr(3, 4) == "e");
}

void Test_M2MString::test_operator_get()
{
    String s("name");
    const String s1("yeb");

    CHECK(s[1] == 'a');
    CHECK(s1[1] == 'e');
}

void Test_M2MString::test_at()
{
    String s("name");
    const String s1("yeb");

    CHECK(s.at(1) == 'a');
    CHECK(s.at(14) == '\0');
    CHECK(s1.at(1) == 'e');
    CHECK(s1.at(31) == '\0');
}

void Test_M2MString::test_erase()
{
    String s("name");
    String s1("yeb");

    s.erase(1,1);
    CHECK(s[1] == 'm');
}

void Test_M2MString::test_append()
{
    String s("name");
    String s1("yeb");

    s.append( s1.c_str(), 1 );
    CHECK(s.size() == 5);

    s.append( s1.c_str(), 15 );
    CHECK(s.size() == 8);
}

void Test_M2MString::test_append_raw()
{
    String s("name");
    const char test_source[] = "something";
    String expected("namesomething");

    s.append_raw(test_source, 1);
    CHECK(s.size() == 5);

    s.append_raw(test_source + 1, 8);
    CHECK(s.size() == 13);

    CHECK(s == expected);
}

void Test_M2MString::test_append_int()
{
    String s("source");
    String expected("source1234");
    String expected2("source12342147483647");

    s.append_int(1234);
    CHECK(s.size() == 10);

    CHECK(s == expected);

    s.append_int(INT32_MAX);

    CHECK(s == expected2);
}

void Test_M2MString::test_compare()
{
    String s("name");
    String s1("yeb");
    String s2("name");
    String s3("nam");

    CHECK(s.compare(1,5, s1) < 0);
    CHECK(s1.compare(0,5, s2) > 0);
    CHECK(s.compare(0,4, s2) == 0);
    CHECK(s.compare(0,4, s3) > 0);

    CHECK(s.compare(1,5, "yeb") < 0);
    CHECK(s1.compare(0,5, "name") > 0);
    CHECK(s.compare(0,4, "name") == 0);
    CHECK(s.compare(0,4, "nam") > 0);
}

void Test_M2MString::test_find_last_of()
{
    String s("namenamename");
    String s1("yeb");

    CHECK(s.find_last_of('n') == 8);
}

void Test_M2MString::test_operator_lt()
{
    String s("name");
    String s1("yeb");
    String s2("yea");

    CHECK( (s < s1 ) == true);
    CHECK( (s1 < s2 ) == false);
}
void Test_M2MString::test_reverse()
{
    char string1[] = "123";
    char string2[] = "321";
    m2m::reverse(string1, strlen(string1));
    char string3[] = "9223372036854775807";
    char string4[] = "7085774586302733229";
    m2m::reverse(string3, strlen(string3));

    CHECK(strcmp(string1, string2) == 0);
    CHECK(strcmp(string3, string4) == 0);
}
void Test_M2MString::test_itoa_c()
{
    int64_t value1 = 0;
    char* string1 = "0";
    int64_t value2 = -10;
    char* string2 = "-10";
    int64_t value3 = 10000;
    char* string3 = "10000";
    int64_t value4 = 9223372036854775807;
    char* string4 = "9223372036854775807";
    int64_t value5 = -9223372036854775807;
    char* string5 = "-9223372036854775807";

    char *buffer = (char*)malloc(21);

    if(buffer) {
        m2m::itoa_c(value1, buffer);
        CHECK(strcmp(string1, buffer) == 0);
        m2m::itoa_c(value2, buffer);
        CHECK(strcmp(string2, buffer) == 0);
        m2m::itoa_c(value3, buffer);
        CHECK(strcmp(string3, buffer) == 0);
        m2m::itoa_c(value4, buffer);
        CHECK(strcmp(string4, buffer) == 0);
        m2m::itoa_c(value5, buffer);
        CHECK(strcmp(string5, buffer) == 0);
        free(buffer);
    }
}

void Test_M2MString::test_convert_integer_to_array()
{
    uint8_t *max_age_ptr = NULL;
    uint8_t max_age_len = 0;
    uint8_t *temp = NULL;
    uint8_t temp_len = 0;

    int64_t val = 0;
    max_age_ptr = m2m::String::convert_integer_to_array(val,max_age_len);
    CHECK(max_age_ptr != NULL);
    CHECK(val == m2m::String::convert_array_to_integer(max_age_ptr, max_age_len));
    temp = m2m::String::convert_integer_to_array(0,temp_len, max_age_ptr, max_age_len);
    CHECK(temp != NULL);
    CHECK(val == m2m::String::convert_array_to_integer(temp, temp_len));
    free(temp);
    free(max_age_ptr);
    max_age_ptr = NULL;

    val = 0xff;
    max_age_ptr = m2m::String::convert_integer_to_array(val,max_age_len);
    CHECK(max_age_ptr != NULL);
    CHECK(val == m2m::String::convert_array_to_integer(max_age_ptr, max_age_len));
    temp = m2m::String::convert_integer_to_array(0,temp_len, max_age_ptr, max_age_len);
    CHECK(temp != NULL);
    CHECK(val == m2m::String::convert_array_to_integer(temp, temp_len));
    free(temp);
    free(max_age_ptr);
    max_age_ptr = NULL;

    val = 0xffff;
    max_age_ptr = m2m::String::convert_integer_to_array(val,max_age_len);
    CHECK(max_age_ptr != NULL);
    CHECK(val == m2m::String::convert_array_to_integer(max_age_ptr, max_age_len));
    free(max_age_ptr);
    max_age_ptr = NULL;

    val = 0xffffff;
    max_age_ptr = m2m::String::convert_integer_to_array(val,max_age_len);
    CHECK(max_age_ptr != NULL);
    CHECK(val == m2m::String::convert_array_to_integer(max_age_ptr, max_age_len));
    free(max_age_ptr);
    max_age_ptr = NULL;

    val = 0xffffffff;
    max_age_ptr = m2m::String::convert_integer_to_array(val,max_age_len);
    CHECK(max_age_ptr != NULL);
    CHECK(val == m2m::String::convert_array_to_integer(max_age_ptr, max_age_len));
    free(max_age_ptr);
    max_age_ptr = NULL;

    val = 0xffffffffff;
    max_age_ptr = m2m::String::convert_integer_to_array(val,max_age_len);
    CHECK(max_age_ptr != NULL);
    CHECK(val == m2m::String::convert_array_to_integer(max_age_ptr, max_age_len));
    free(max_age_ptr);
    max_age_ptr = NULL;

    val = 0xffffffffffff;
    max_age_ptr = m2m::String::convert_integer_to_array(val,max_age_len);
    CHECK(max_age_ptr != NULL);
    CHECK(val == m2m::String::convert_array_to_integer(max_age_ptr, max_age_len));
    free(max_age_ptr);
    max_age_ptr = NULL;

    val = 0xffffffffffffff;
    max_age_ptr = m2m::String::convert_integer_to_array(val,max_age_len);
    CHECK(max_age_ptr != NULL);
    CHECK(val == m2m::String::convert_array_to_integer(max_age_ptr, max_age_len));
    free(max_age_ptr);
    max_age_ptr = NULL;

    val = 0xffff;
    max_age_ptr = m2m::String::convert_integer_to_array(val,max_age_len);
    CHECK(max_age_ptr != NULL);
    CHECK(val == m2m::String::convert_array_to_integer(max_age_ptr, max_age_len));
    free(max_age_ptr);
    max_age_ptr = NULL;



}



