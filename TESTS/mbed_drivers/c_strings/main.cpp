/*
 * Copyright (c) 2013-2016, ARM Limited, All Rights Reserved
 * SPDX-License-Identifier: Apache-2.0
 *
 * Licensed under the Apache License, Version 2.0 (the "License"); you may
 * not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS, WITHOUT
 * WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
#include <stdio.h>
#include <string.h>
#include "mbed.h"
#include "greentea-client/test_env.h"
#include "unity/unity.h"
#include "utest/utest.h"

namespace {
static char buffer[256] = {0};
}

#define CLEAN_BUFFER memset(::buffer, 0x00, sizeof(::buffer))
#define NEGATIVE_INTEGERS -32768,-3214,-999,-100,-1,0,-1,-4231,-999,-4123,-32760,-99999
#define POSITIVE_INTEGERS 32768,3214,999,100,1,0,1,4231,999,4123,32760,99999
#define FLOATS  0.002,0.92430,15.91320,791.77368,6208.2,25719.4952,426815.982588,6429271.046,42468024.93,212006462.910

using namespace utest::v1;


void test_case_c_string_i_d() {
    CLEAN_BUFFER;
    sprintf(buffer, "%i %d %i %d %i %d %i %d %i %d %i %i", NEGATIVE_INTEGERS);
    TEST_ASSERT_EQUAL_STRING("-32768 -3214 -999 -100 -1 0 -1 -4231 -999 -4123 -32760 -99999", buffer);
}

void test_case_c_string_u_d() {
    CLEAN_BUFFER;
    sprintf(buffer, "%u %d %u %d %u %d %u %d %u %d %u %d", POSITIVE_INTEGERS);
    TEST_ASSERT_EQUAL_STRING("32768 3214 999 100 1 0 1 4231 999 4123 32760 99999", buffer);
}

void test_case_c_string_x_E() {
    CLEAN_BUFFER;
    sprintf(buffer, "%x %X %x %X %x %X %x %X %x %X %x %X", POSITIVE_INTEGERS);
    TEST_ASSERT_EQUAL_STRING("8000 C8E 3e7 64 1 0 1 1087 3e7 101B 7ff8 1869F", buffer);
}

void test_case_c_string_f_f() {
    CLEAN_BUFFER;
    sprintf(buffer, "%f %f %f %f %f %f %f %f %f %f", FLOATS);
    TEST_ASSERT_EQUAL_STRING("0.002000 0.924300 15.913200 791.773680 6208.200000 25719.495200 426815.982588 6429271.046000 42468024.930000 212006462.910000", buffer);
}

void test_case_c_string_g_g() {
    CLEAN_BUFFER;
    sprintf(buffer, "%g %g %g %g %g %g %g %g %g %g", FLOATS);
    TEST_ASSERT_EQUAL_STRING("0.002 0.9243 15.9132 791.774 6208.2 25719.5 426816 6.42927e+06 4.2468e+07 2.12006e+08", buffer);
}

void test_case_c_string_e_E() {
    CLEAN_BUFFER;
    sprintf(buffer, "%e %E %e %E %e %E %e %E %e %E", FLOATS);
    TEST_ASSERT_EQUAL_STRING("2.000000e-03 9.243000E-01 1.591320e+01 7.917737E+02 6.208200e+03 2.571950E+04 4.268160e+05 6.429271E+06 4.246802e+07 2.120065E+08", buffer);
}

void test_case_c_string_strtok() {
    CLEAN_BUFFER;
    char str[] ="- This, a sample string.";
    char * pch = strtok (str," ,.-");
    while (pch != NULL) {
        strcat(buffer, pch);
        pch = strtok (NULL, " ,.-");
    }
    TEST_ASSERT_EQUAL_STRING("Thisasamplestring", buffer);
}

void test_case_c_string_strpbrk() {
    CLEAN_BUFFER;
    char str[] = "This is a sample string";
    char key[] = "aeiou";
    char *pch = strpbrk(str, key);
    while (pch != NULL)
    {
        char buf[2] = {*pch, '\0'};
        strcat(buffer, buf);
        pch = strpbrk(pch + 1,key);
    }
    TEST_ASSERT_EQUAL_STRING("iiaaei", buffer);
}

utest::v1::status_t greentea_failure_handler(const Case *const source, const failure_t reason) {
    greentea_case_failure_abort_handler(source, reason);
    return STATUS_CONTINUE;
}

Case cases[] = {
    Case("C strings: strtok", test_case_c_string_strtok, greentea_failure_handler),
    Case("C strings: strpbrk", test_case_c_string_strpbrk, greentea_failure_handler),
    Case("C strings: %i %d integer formatting", test_case_c_string_i_d, greentea_failure_handler),
    Case("C strings: %u %d integer formatting", test_case_c_string_u_d, greentea_failure_handler),
    Case("C strings: %x %E integer formatting", test_case_c_string_x_E, greentea_failure_handler),
    Case("C strings: %f %f float formatting", test_case_c_string_f_f, greentea_failure_handler),
    Case("C strings: %e %E float formatting", test_case_c_string_e_E, greentea_failure_handler),
    Case("C strings: %g %g float formatting", test_case_c_string_g_g, greentea_failure_handler),
};

utest::v1::status_t greentea_test_setup(const size_t number_of_cases) {
    GREENTEA_SETUP(5, "default_auto");
    return greentea_test_setup_handler(number_of_cases);
}

Specification specification(greentea_test_setup, cases, greentea_test_teardown_handler);

int main() {
    Harness::run(specification);
}
