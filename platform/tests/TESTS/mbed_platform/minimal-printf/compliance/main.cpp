/* mbed Microcontroller Library
 * Copyright (c) 2017 ARM Limited
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

#ifdef TARGET_LIKE_MBED
#include "mbed.h"
#endif
#include "mbed_printf.h"

#include "utest/utest.h"
#include "unity/unity.h"
#include "greentea-client/test_env.h"

#include <stdint.h>
#include <stdbool.h>
#include <limits.h>
#include <inttypes.h>

#ifndef ULLONG_MAX
#define ULLONG_MAX UINT64_MAX
#endif

#ifndef LLONG_MAX
#define LLONG_MAX INT64_MAX
#endif

#ifndef LLONG_MIN
#define LLONG_MIN INT64_MIN
#endif

using namespace utest::v1;

#define MAX_STRING_SIZE 100
// Buffer used by test cases to store a test string
static char expected_string[MAX_STRING_SIZE];

#define BASE_10 10
#define BASE_16 16

/** This function converts an integer into a string representing the integer in base 10 or base 16.
 *
 *  @param value to be converted to a string.
 *  @param str is an array where to store the resulting null-terminated string.
 *  @param base used to represent the string.
 *
 *  @return
 *    A pointer to the resulting null-terminated string, same as parameter str.
 */
static char *convert_to_string(
    uint64_t  value,
    char str[],
    int base = BASE_10,
    bool is_negative = false)
{
    int i = 0;

    if (base != BASE_10 && base != BASE_16) {
        MBED_ASSERT(0);
    }

    if (value == 0) {
        str[0] = '0';
        str[1] = '\0';
        return str;
    }

    if (is_negative && ((long long) value < 0)) {
        value = -value;
    }
    // Convert a value to a string stored in reverse order in memory.
    while (value) {
        int remainder = value % base;
        str[i++] = (remainder > 9) ? (remainder - 10) + 'A' : remainder + '0';
        value = value / base;
    }

    if (is_negative) {
        str[i++] = '-';
    }

    str[i] = '\0';

    // Reverse the string.
    int start = 0;
    int end = i - 1;
    while (start < end) {
        swap(*(str + start), *(str + end));
        start++;
        end--;
    }
    return str;
}

/** This function is using a global buffer to build the test string 'prefix + data + suffix' and
 *  returns its length.
 *
 *  @param prefix is a null-terminated string.
 *  @param value to be converted to a string.
 *  @param base used to represent the string.
 *  @param suffix is a null-terminated string.
 *  @param is_negative is used to represent a positive or negative value
 *
 *  @return
 *    string length
 */
static int make_test_string(
    const char *prefix = nullptr,
    long long value = 0,
    int base = BASE_10,
    const char *suffix = nullptr,
    bool is_negative = false
)
{
    char *exp_str = &expected_string[0];
    int str_length = 0;
    memset(exp_str, 0, MAX_STRING_SIZE);
    if (prefix) {
        str_length = strlen(prefix);
        MBED_ASSERT(str_length < MAX_STRING_SIZE);
        strncpy(exp_str, prefix, str_length);
    }

    convert_to_string(value, &exp_str[str_length], base, is_negative);
    if (suffix) {
        str_length = strlen(exp_str);
        MBED_ASSERT(strlen(suffix) < (MAX_STRING_SIZE - str_length));
        strncat(exp_str, suffix, str_length);
    }
    return strlen(exp_str);
}

// Extract the prefix string which is all characters until '%'.
static void extract_prefix(const char *fmt, char *prefix)
{
    int i = 0;
    while (fmt && prefix && fmt[i] != '%') {
        prefix[i] = fmt[i];
        i++;
    }
}

static control_t test_printf_d(const size_t call_count)
{
    int result_baseline;
    int result_minimal;
    int result_file;

    /*************************************************************************/
    /*************************************************************************/
    result_minimal = mbed_printf("hhd: %hhd\r\n", SCHAR_MIN);
    result_file = mbed_fprintf(stderr, "hhd: %hhd\r\n", SCHAR_MIN);
    result_baseline = make_test_string("hhd: ", SCHAR_MIN, BASE_10, "\r\n", true);
    TEST_ASSERT_EQUAL_INT(result_baseline, result_minimal);
    TEST_ASSERT_EQUAL_INT(result_baseline, result_file);

    result_minimal = mbed_printf("hhd: %hhd\r\n", SCHAR_MAX);
    result_file = mbed_fprintf(stderr, "hhd: %hhd\r\n", SCHAR_MAX);
    result_baseline = make_test_string("hhd: ", SCHAR_MAX, BASE_10, "\r\n");
    TEST_ASSERT_EQUAL_INT(result_baseline, result_minimal);
    TEST_ASSERT_EQUAL_INT(result_baseline, result_file);

    result_minimal = mbed_printf("hd: %hd\r\n", SHRT_MIN);
    result_file = mbed_fprintf(stderr, "hd: %hd\r\n", SHRT_MIN);
    result_baseline = make_test_string("hd: ", SHRT_MIN, BASE_10, "\r\n", true);
    TEST_ASSERT_EQUAL_INT(result_baseline, result_minimal);
    TEST_ASSERT_EQUAL_INT(result_baseline, result_file);

    result_minimal = mbed_printf("hd: %hd\r\n", SHRT_MAX);
    result_file = mbed_fprintf(stderr, "hd: %hd\r\n", SHRT_MAX);
    result_baseline = make_test_string("hd: ", SHRT_MAX, BASE_10, "\r\n");
    TEST_ASSERT_EQUAL_INT(result_baseline, result_minimal);
    TEST_ASSERT_EQUAL_INT(result_baseline, result_file);

    result_minimal = mbed_printf("d: %d\r\n", INT_MIN);
    result_file = mbed_fprintf(stderr, "d: %d\r\n", INT_MIN);
    result_baseline = make_test_string("d: ", INT_MIN, BASE_10, "\r\n", true);
    TEST_ASSERT_EQUAL_INT(result_baseline, result_minimal);
    TEST_ASSERT_EQUAL_INT(result_baseline, result_file);

    result_minimal = mbed_printf("d: %d\r\n", INT_MAX);
    result_file = mbed_fprintf(stderr, "d: %d\r\n", INT_MAX);
    result_baseline = make_test_string("d: ", INT_MAX, BASE_10, "\r\n");
    TEST_ASSERT_EQUAL_INT(result_baseline, result_minimal);
    TEST_ASSERT_EQUAL_INT(result_baseline, result_file);

    result_minimal = mbed_printf("ld: %ld\r\n", LONG_MIN);
    result_file = mbed_fprintf(stderr, "ld: %ld\r\n", LONG_MIN);
    result_baseline = make_test_string("ld: ", LONG_MIN, BASE_10, "\r\n", true);
    TEST_ASSERT_EQUAL_INT(result_baseline, result_minimal);
    TEST_ASSERT_EQUAL_INT(result_baseline, result_file);

    result_minimal = mbed_printf("ld: %ld\r\n", LONG_MAX);
    result_file = mbed_fprintf(stderr, "ld: %ld\r\n", LONG_MAX);
    result_baseline = make_test_string("ld: ", LONG_MAX, BASE_10, "\r\n");
    TEST_ASSERT_EQUAL_INT(result_baseline, result_minimal);
    TEST_ASSERT_EQUAL_INT(result_baseline, result_file);

    result_minimal = mbed_printf("lld: %lld\r\n", LLONG_MIN);
    result_file = mbed_fprintf(stderr, "lld: %lld\r\n", LLONG_MIN);
    result_baseline = make_test_string("lld: ", LLONG_MIN, BASE_10, "\r\n", true);
    TEST_ASSERT_EQUAL_INT(result_baseline, result_minimal);
    TEST_ASSERT_EQUAL_INT(result_baseline, result_file);

    result_minimal = mbed_printf("lld: %lld\r\n", LLONG_MAX);
    result_file = mbed_fprintf(stderr, "lld: %lld\r\n", LLONG_MAX);
    result_baseline = make_test_string("lld: ", LLONG_MAX, BASE_10, "\r\n");
    TEST_ASSERT_EQUAL_INT(result_baseline, result_minimal);
    TEST_ASSERT_EQUAL_INT(result_baseline, result_file);

#ifdef TARGET_LIKE_MBED
    printf("%%jd not supported by mbed\r\n");
#else
    result_minimal = mbed_printf("jd: %jd\r\n", INT32_MIN);
    result_file = mbed_fprintf(stderr, "jd: %jd\r\n", INT32_MIN);
    result_baseline = make_test_string("jd: ", (intmax_t) INT32_MIN, BASE_10, "\r\n", true);
    TEST_ASSERT_EQUAL_INT(result_baseline, result_minimal);
    TEST_ASSERT_EQUAL_INT(result_baseline, result_file);

    result_minimal = mbed_printf("jd: %jd\r\n", INT32_MAX);
    result_file = mbed_fprintf(stderr, "jd: %jd\r\n", INT32_MAX);
    result_baseline = make_test_string("jd: ", (intmax_t) INT32_MAX, BASE_10, "\r\n");
    TEST_ASSERT_EQUAL_INT(result_baseline, result_minimal);
    TEST_ASSERT_EQUAL_INT(result_baseline, result_file);
#endif

#ifdef TARGET_LIKE_MBED
    printf("%%zd not supported by mbed\r\n");
#else
    result_minimal = mbed_printf("zd: %zd\r\n", INT32_MIN);
    result_file = mbed_fprintf(stderr, "zd: %zd\r\n", INT32_MIN);
    result_baseline = make_test_string("zd: ", (ssize_t) INT32_MIN, BASE_10, "\r\n", true);
    TEST_ASSERT_EQUAL_INT(result_baseline, result_minimal);
    TEST_ASSERT_EQUAL_INT(result_baseline, result_file);

    result_minimal = mbed_printf("zd: %zd\r\n", INT32_MAX);
    result_file = mbed_fprintf(stderr, "zd: %zd\r\n", INT32_MAX);
    result_baseline = make_test_string("zd: ", (ssize_t) INT32_MAX, BASE_10, "\r\n");
    TEST_ASSERT_EQUAL_INT(result_baseline, result_minimal);
    TEST_ASSERT_EQUAL_INT(result_baseline, result_file);
#endif

#ifdef TARGET_LIKE_MBED
    printf("%%td not supported by mbed\r\n");
#else
    result_minimal = mbed_printf("td: %td\r\n", PTRDIFF_MIN);
    result_file = mbed_fprintf(stderr, "td: %td\r\n", PTRDIFF_MIN);
    result_baseline = make_test_string("td: ", PTRDIFF_MIN, BASE_10, "\r\n", true);
    TEST_ASSERT_EQUAL_INT(result_baseline, result_minimal);
    TEST_ASSERT_EQUAL_INT(result_baseline, result_file);

    result_minimal = mbed_printf("td: %td\r\n", PTRDIFF_MAX);
    result_file = mbed_fprintf(stderr, "td: %td\r\n", PTRDIFF_MAX);
    result_baseline = make_test_string("td: ", PTRDIFF_MAX, BASE_10, "\r\n");
    TEST_ASSERT_EQUAL_INT(result_baseline, result_minimal);
    TEST_ASSERT_EQUAL_INT(result_baseline, result_file);
#endif

    return CaseNext;
}

static control_t test_printf_u(const size_t call_count)
{
    int result_baseline;
    int result_minimal;
    int result_file;

    /*************************************************************************/
    /*************************************************************************/
    result_minimal = mbed_printf("hhu: %hhu\r\n", 0);
    result_file = mbed_fprintf(stderr, "hhu: %hhu\r\n", 0);
    result_baseline = make_test_string("hhu: ", 0, BASE_10, "\r\n");
    TEST_ASSERT_EQUAL_INT(result_baseline, result_minimal);
    TEST_ASSERT_EQUAL_INT(result_baseline, result_file);

    result_minimal = mbed_printf("hhu: %hhu\r\n", UCHAR_MAX);
    result_file = mbed_fprintf(stderr, "hhu: %hhu\r\n", UCHAR_MAX);
    result_baseline = make_test_string("hhu: ", UCHAR_MAX, BASE_10, "\r\n");
    TEST_ASSERT_EQUAL_INT(result_baseline, result_minimal);
    TEST_ASSERT_EQUAL_INT(result_baseline, result_file);

    result_minimal = mbed_printf("hu: %hu\r\n", 0);
    result_file = mbed_fprintf(stderr, "hu: %hu\r\n", 0);
    result_baseline = make_test_string("hu: ", 0, BASE_10, "\r\n");
    TEST_ASSERT_EQUAL_INT(result_baseline, result_minimal);
    TEST_ASSERT_EQUAL_INT(result_baseline, result_file);

    result_minimal = mbed_printf("hu: %hu\r\n", USHRT_MAX);
    result_file = mbed_fprintf(stderr, "hu: %hu\r\n", USHRT_MAX);
    result_baseline = make_test_string("hu: ", USHRT_MAX, BASE_10, "\r\n");
    TEST_ASSERT_EQUAL_INT(result_baseline, result_minimal);
    TEST_ASSERT_EQUAL_INT(result_baseline, result_file);

    result_minimal = mbed_printf("u: %u\r\n", 0);
    result_file = mbed_fprintf(stderr, "u: %u\r\n", 0);
    result_baseline = make_test_string("u: ", 0, BASE_10, "\r\n");
    TEST_ASSERT_EQUAL_INT(result_baseline, result_minimal);
    TEST_ASSERT_EQUAL_INT(result_baseline, result_file);

    result_minimal = mbed_printf("u: %u\r\n", UINT_MAX);
    result_file = mbed_fprintf(stderr, "u: %u\r\n", UINT_MAX);
    result_baseline = make_test_string("u: ", UINT_MAX, BASE_10, "\r\n");
    TEST_ASSERT_EQUAL_INT(result_baseline, result_minimal);
    TEST_ASSERT_EQUAL_INT(result_baseline, result_file);

    result_minimal = mbed_printf("lu: %lu\r\n", 0UL);
    result_file = mbed_fprintf(stderr, "lu: %lu\r\n", 0UL);
    result_baseline = make_test_string("lu: ", 0UL, BASE_10, "\r\n");
    TEST_ASSERT_EQUAL_INT(result_baseline, result_minimal);
    TEST_ASSERT_EQUAL_INT(result_baseline, result_file);

    result_minimal = mbed_printf("lu: %lu\r\n", ULONG_MAX);
    result_file = mbed_fprintf(stderr, "lu: %lu\r\n", ULONG_MAX);
    result_baseline = make_test_string("lu: ", ULONG_MAX, BASE_10, "\r\n");
    TEST_ASSERT_EQUAL_INT(result_baseline, result_minimal);
    TEST_ASSERT_EQUAL_INT(result_baseline, result_file);

    result_minimal = mbed_printf("llu: %llu\r\n", 0ULL);
    result_file = mbed_fprintf(stderr, "llu: %llu\r\n", 0ULL);
    result_baseline = make_test_string("llu: ", 0ULL, BASE_10, "\r\n");
    TEST_ASSERT_EQUAL_INT(result_baseline, result_minimal);
    TEST_ASSERT_EQUAL_INT(result_baseline, result_file);

    result_minimal = mbed_printf("llu: %llu\r\n", ULLONG_MAX);
    result_file = mbed_fprintf(stderr, "llu: %llu\r\n", ULLONG_MAX);
    result_baseline = make_test_string("llu: ", ULLONG_MAX, BASE_10, "\r\n");
    TEST_ASSERT_EQUAL_INT(result_baseline, result_minimal);
    TEST_ASSERT_EQUAL_INT(result_baseline, result_file);

#ifdef TARGET_LIKE_MBED
    printf("%%ju not supported by mbed\r\n");
#else
    result_minimal = mbed_printf("ju: %ju\r\n", (uintmax_t) 0);
    result_file = mbed_fprintf(stderr, "ju: %ju\r\n", (uintmax_t) 0);
    result_baseline = make_test_string("ju: ", (uintmax_t) 0, BASE_10, "\r\n");
    TEST_ASSERT_EQUAL_INT(result_baseline, result_minimal);
    TEST_ASSERT_EQUAL_INT(result_baseline, result_file);

    result_minimal = mbed_printf("ju: %ju\r\n", UINTMAX_MAX);
    result_file = mbed_fprintf(stderr, "ju: %ju\r\n", UINTMAX_MAX);
    result_baseline = make_test_string("ju: ", UINTMAX_MAX, BASE_10, "\r\n");
    TEST_ASSERT_EQUAL_INT(result_baseline, result_minimal);
    TEST_ASSERT_EQUAL_INT(result_baseline, result_file);
#endif

#ifdef TARGET_LIKE_MBED
    printf("%%zu not supported by mbed\r\n");
#else
    result_minimal = mbed_printf("zu: %zu\r\n", 0);
    result_file = mbed_fprintf(stderr, "zu: %zu\r\n", 0);
    result_baseline = make_test_string("zu: ", 0, BASE_10, "\r\n");
    TEST_ASSERT_EQUAL_INT(result_baseline, result_minimal);
    TEST_ASSERT_EQUAL_INT(result_baseline, result_file);

    result_minimal = mbed_printf("zu: %zu\r\n", SIZE_MAX);
    result_file = mbed_fprintf(stderr, "zu: %zu\r\n", SIZE_MAX);
    result_baseline = make_test_string("zu: ", SIZE_MAX, BASE_10, "\r\n");
    TEST_ASSERT_EQUAL_INT(result_baseline, result_minimal);
    TEST_ASSERT_EQUAL_INT(result_baseline, result_file);
#endif

#ifdef TARGET_LIKE_MBED
    printf("%%tu not supported by mbed\r\n");
#else
    result_minimal = mbed_printf("tu: %tu\r\n", 0);
    result_file = mbed_fprintf(stderr, "tu: %tu\r\n", 0);
    result_baseline = make_test_string("tu: ", 0, BASE_10, "\r\n");
    TEST_ASSERT_EQUAL_INT(result_baseline, result_minimal);
    TEST_ASSERT_EQUAL_INT(result_baseline, result_file);

    result_minimal = mbed_printf("tu: %tu\r\n", UINTPTR_MAX);
    result_file = mbed_fprintf(stderr, "tu: %tu\r\n", UINTPTR_MAX);
    result_baseline = make_test_string("tu: ", UINTPTR_MAX, BASE_10, "\r\n");
    TEST_ASSERT_EQUAL_INT(result_baseline, result_minimal);
    TEST_ASSERT_EQUAL_INT(result_baseline, result_file);
#endif

    return CaseNext;
}

static control_t test_printf_x(const size_t call_count)
{
    int result_baseline;
    int result_minimal;
    int result_file;

    /*************************************************************************/
    /*************************************************************************/
    result_minimal = mbed_printf("hhX: %hhX\r\n", 0);
    result_file = mbed_fprintf(stderr, "hhX: %hhX\r\n", 0);
    result_baseline = make_test_string("hhX: ", 0, BASE_16, "\r\n");
    TEST_ASSERT_EQUAL_INT(result_baseline, result_minimal);
    TEST_ASSERT_EQUAL_INT(result_baseline, result_file);

    result_minimal = mbed_printf("hhX: %hhX\r\n", UCHAR_MAX);
    result_file = mbed_fprintf(stderr, "hhX: %hhX\r\n", UCHAR_MAX);
    result_baseline = make_test_string("hhX: ", UCHAR_MAX, BASE_16, "\r\n");
    TEST_ASSERT_EQUAL_INT(result_baseline, result_minimal);
    TEST_ASSERT_EQUAL_INT(result_baseline, result_file);

    result_minimal = mbed_printf("hX: %hX\r\n", 0);
    result_file = mbed_fprintf(stderr, "hX: %hX\r\n", 0);
    result_baseline = make_test_string("hX: ", 0, BASE_16, "\r\n");
    TEST_ASSERT_EQUAL_INT(result_baseline, result_minimal);
    TEST_ASSERT_EQUAL_INT(result_baseline, result_file);

    result_minimal = mbed_printf("hX: %hX\r\n", USHRT_MAX);
    result_file = mbed_fprintf(stderr, "hX: %hX\r\n", USHRT_MAX);
    result_baseline = make_test_string("hX: ", USHRT_MAX, BASE_16, "\r\n");
    TEST_ASSERT_EQUAL_INT(result_baseline, result_minimal);
    TEST_ASSERT_EQUAL_INT(result_baseline, result_file);

    result_minimal = mbed_printf("X: %X\r\n", 0);
    result_file = mbed_fprintf(stderr, "X: %X\r\n", 0);
    result_baseline = make_test_string("X: ", 0, BASE_16, "\r\n");
    TEST_ASSERT_EQUAL_INT(result_baseline, result_minimal);
    TEST_ASSERT_EQUAL_INT(result_baseline, result_file);

    result_minimal = mbed_printf("X: %X\r\n", UINT_MAX);
    result_file = mbed_fprintf(stderr, "X: %X\r\n", UINT_MAX);
    result_baseline = make_test_string("X: ", UINT_MAX, BASE_16, "\r\n");
    TEST_ASSERT_EQUAL_INT(result_baseline, result_minimal);
    TEST_ASSERT_EQUAL_INT(result_baseline, result_file);

    result_minimal = mbed_printf("lX: %lX\r\n", 0UL);
    result_file = mbed_fprintf(stderr, "lX: %lX\r\n", 0UL);
    result_baseline = make_test_string("lX: ", 0UL, BASE_16, "\r\n");
    TEST_ASSERT_EQUAL_INT(result_baseline, result_minimal);
    TEST_ASSERT_EQUAL_INT(result_baseline, result_file);

    result_minimal = mbed_printf("lX: %lX\r\n", ULONG_MAX);
    result_file = mbed_fprintf(stderr, "lX: %lX\r\n", ULONG_MAX);
    result_baseline = make_test_string("lX: ", ULONG_MAX, BASE_16, "\r\n");
    TEST_ASSERT_EQUAL_INT(result_baseline, result_minimal);
    TEST_ASSERT_EQUAL_INT(result_baseline, result_file);

    result_minimal = mbed_printf("llX: %llX\r\n", 0ULL);
    result_file = mbed_fprintf(stderr, "llX: %llX\r\n", 0ULL);
    result_baseline = make_test_string("llX: ", 0ULL, BASE_16, "\r\n");
    TEST_ASSERT_EQUAL_INT(result_baseline, result_minimal);
    TEST_ASSERT_EQUAL_INT(result_baseline, result_file);

    result_minimal = mbed_printf("llX: %llX\r\n", ULLONG_MAX);
    result_file = mbed_fprintf(stderr, "llX: %llX\r\n", ULLONG_MAX);
    result_baseline = make_test_string("llX: ", ULLONG_MAX, BASE_16, "\r\n");
    TEST_ASSERT_EQUAL_INT(result_baseline, result_minimal);
    TEST_ASSERT_EQUAL_INT(result_baseline, result_file);

#ifdef TARGET_LIKE_MBED
    printf("%%jX not supported by mbed\r\n");
#else
    result_minimal = mbed_printf("jX: %jX\r\n", (uintmax_t) 0);
    result_file = mbed_fprintf(stderr, "jX: %jX\r\n", (uintmax_t) 0);
    result_baseline = make_test_string("jX: ", (uintmax_t) 0, BASE_16, "\r\n");
    TEST_ASSERT_EQUAL_INT(result_baseline, result_minimal);
    TEST_ASSERT_EQUAL_INT(result_baseline, result_file);

    result_minimal = mbed_printf("jX: %jX\r\n", UINTMAX_MAX);
    result_file = mbed_fprintf(stderr, "jX: %jX\r\n", UINTMAX_MAX);
    result_baseline = make_test_string("jX: ", UINTMAX_MAX, BASE_16, "\r\n");
    TEST_ASSERT_EQUAL_INT(result_baseline, result_minimal);
    TEST_ASSERT_EQUAL_INT(result_baseline, result_file);
#endif

#ifdef TARGET_LIKE_MBED
    printf("%%zX not supported by mbed\r\n");
#else
    result_minimal = mbed_printf("zX: %zX\r\n", 0);
    result_file = mbed_fprintf(stderr, "zX: %zX\r\n", 0);
    result_baseline = make_test_string("zX: ", 0, BASE_16, "\r\n");
    TEST_ASSERT_EQUAL_INT(result_baseline, result_minimal);
    TEST_ASSERT_EQUAL_INT(result_baseline, result_file);

    result_minimal = mbed_printf("zX: %zX\r\n", SIZE_MAX);
    result_file = mbed_fprintf(stderr, "zX: %zX\r\n", SIZE_MAX);
    result_baseline = make_test_string("zX: ", SIZE_MAX, BASE_16, "\r\n");
    TEST_ASSERT_EQUAL_INT(result_baseline, result_minimal);
    TEST_ASSERT_EQUAL_INT(result_baseline, result_file);
#endif

#ifdef TARGET_LIKE_MBED
    printf("%%tX not supported by mbed\r\n");
#else
    result_minimal = mbed_printf("tX: %tX\r\n", 0);
    result_file = mbed_fprintf(stderr, "tX: %tX\r\n", 0);
    result_baseline = make_test_string("tX: ", 0, BASE_16, "\r\n");
    TEST_ASSERT_EQUAL_INT(result_baseline, result_minimal);
    TEST_ASSERT_EQUAL_INT(result_baseline, result_file);

    result_minimal = mbed_printf("tX: %tX\r\n", UINTPTR_MAX);
    result_file = mbed_fprintf(stderr, "tX: %tX\r\n", UINTPTR_MAX);
    result_baseline = make_test_string("tX: ", UINTPTR_MAX, BASE_16, "\r\n");
    TEST_ASSERT_EQUAL_INT(result_baseline, result_minimal);
    TEST_ASSERT_EQUAL_INT(result_baseline, result_file);
#endif

    result_minimal = mbed_printf("x: %x\r\n", 11259375);
    result_baseline = make_test_string("x: ", 11259375, BASE_16, "\r\n");
    TEST_ASSERT_EQUAL_INT(result_baseline, result_minimal);

    return CaseNext;
}

static control_t test_printf_percent(const size_t call_count)
{
    int result_baseline;
    int result_minimal;
    int result_file;
    char buffer_baseline[100] = {0};

    result_minimal = mbed_printf("%% \r\n");
    result_file = mbed_fprintf(stderr, "%% \r\n");
    result_baseline = sprintf(buffer_baseline, "%% \r\n");
    TEST_ASSERT_EQUAL_INT(result_baseline, result_minimal);
    TEST_ASSERT_EQUAL_INT(result_baseline, result_file);

    return CaseNext;
}

/******************************************************************************/
/*                                                                            */
/* SNPRINTF                                                                   */
/*                                                                            */
/******************************************************************************/

static control_t test_snprintf_d(const size_t call_count)
{
    char buffer_minimal[100];
    int result_baseline;
    int result_minimal;

    /*************************************************************************/
    /*************************************************************************/
    result_minimal = mbed_snprintf(buffer_minimal, sizeof(buffer_minimal), "hhd: %hhd\r\n", SCHAR_MIN);
    result_baseline = make_test_string("hhd: ", SCHAR_MIN, BASE_10, "\r\n", true);
    TEST_ASSERT_EQUAL_STRING(expected_string, buffer_minimal);
    TEST_ASSERT_EQUAL_INT(result_baseline, result_minimal);

    result_minimal = mbed_snprintf(buffer_minimal, sizeof(buffer_minimal), "hhd: %hhd\r\n", SCHAR_MAX);
    result_baseline = make_test_string("hhd: ", SCHAR_MAX, BASE_10, "\r\n");
    TEST_ASSERT_EQUAL_STRING(expected_string, buffer_minimal);
    TEST_ASSERT_EQUAL_INT(result_baseline, result_minimal);

    result_minimal = mbed_snprintf(buffer_minimal, sizeof(buffer_minimal), "hd: %hd\r\n", SHRT_MIN);
    result_baseline = make_test_string("hd: ", SHRT_MIN, BASE_10, "\r\n", true);
    TEST_ASSERT_EQUAL_STRING(expected_string, buffer_minimal);
    TEST_ASSERT_EQUAL_INT(result_baseline, result_minimal);

    result_minimal = mbed_snprintf(buffer_minimal, sizeof(buffer_minimal), "hd: %hd\r\n", SHRT_MAX);
    result_baseline = make_test_string("hd: ", SHRT_MAX, BASE_10, "\r\n");
    TEST_ASSERT_EQUAL_STRING(expected_string, buffer_minimal);
    TEST_ASSERT_EQUAL_INT(result_baseline, result_minimal);

    result_minimal = mbed_snprintf(buffer_minimal, sizeof(buffer_minimal), "d: %d\r\n", INT_MIN);
    result_baseline = make_test_string("d: ", INT_MIN, BASE_10, "\r\n", true);
    TEST_ASSERT_EQUAL_STRING(expected_string, buffer_minimal);
    TEST_ASSERT_EQUAL_INT(result_baseline, result_minimal);

    result_minimal = mbed_snprintf(buffer_minimal, sizeof(buffer_minimal), "d: %d\r\n", INT_MAX);
    result_baseline = make_test_string("d: ", INT_MAX, BASE_10, "\r\n");
    TEST_ASSERT_EQUAL_STRING(expected_string, buffer_minimal);
    TEST_ASSERT_EQUAL_INT(result_baseline, result_minimal);

    result_minimal = mbed_snprintf(buffer_minimal, sizeof(buffer_minimal), "ld: %ld\r\n", LONG_MIN);
    result_baseline = make_test_string("ld: ", LONG_MIN, BASE_10, "\r\n", true);
    TEST_ASSERT_EQUAL_STRING(expected_string, buffer_minimal);
    TEST_ASSERT_EQUAL_INT(result_baseline, result_minimal);

    result_minimal = mbed_snprintf(buffer_minimal, sizeof(buffer_minimal), "ld: %ld\r\n", LONG_MAX);
    result_baseline = make_test_string("ld: ", LONG_MAX, BASE_10, "\r\n");
    TEST_ASSERT_EQUAL_STRING(expected_string, buffer_minimal);
    TEST_ASSERT_EQUAL_INT(result_baseline, result_minimal);

    result_minimal = mbed_snprintf(buffer_minimal, sizeof(buffer_minimal), "lld: %lld\r\n", LLONG_MIN);
    result_baseline = make_test_string("lld: ", LLONG_MIN, BASE_10, "\r\n", true);
    TEST_ASSERT_EQUAL_STRING(expected_string, buffer_minimal);
    TEST_ASSERT_EQUAL_INT(result_baseline, result_minimal);

    result_minimal = mbed_snprintf(buffer_minimal, sizeof(buffer_minimal), "lld: %lld\r\n", LLONG_MAX);
    result_baseline = make_test_string("lld: ", LLONG_MAX, BASE_10, "\r\n");
    TEST_ASSERT_EQUAL_STRING(expected_string, buffer_minimal);
    TEST_ASSERT_EQUAL_INT(result_baseline, result_minimal);

#ifdef TARGET_LIKE_MBED
    printf("%%jd not supported by mbed\r\n");
#else
    result_minimal = mbed_snprintf(buffer_minimal, sizeof(buffer_minimal), "jd: %jd\r\n", (intmax_t) INT32_MIN);
    result_baseline = make_test_string("jd: ", (intmax_t) INT32_MIN, BASE_10, "\r\n", true);
    TEST_ASSERT_EQUAL_STRING(expected_string, buffer_minimal);
    TEST_ASSERT_EQUAL_INT(result_baseline, result_minimal);

    result_minimal = mbed_snprintf(buffer_minimal, sizeof(buffer_minimal), "jd: %jd\r\n", (intmax_t) INT32_MAX);
    result_baseline = make_test_string("jd: ", (intmax_t) INT32_MAX, BASE_10, "\r\n");
    TEST_ASSERT_EQUAL_STRING(expected_string, buffer_minimal);
    TEST_ASSERT_EQUAL_INT(result_baseline, result_minimal);
#endif

#ifdef TARGET_LIKE_MBED
    printf("%%zd not supported by mbed\r\n");
#else
    result_minimal = mbed_snprintf(buffer_minimal, sizeof(buffer_minimal), "zd: %zd\r\n", (ssize_t) INT32_MIN);
    result_baseline = make_test_string("zd: ", (ssize_t) INT32_MIN, BASE_10, "\r\n", true);
    TEST_ASSERT_EQUAL_STRING(expected_string, buffer_minimal);
    TEST_ASSERT_EQUAL_INT(result_baseline, result_minimal);

    result_minimal = mbed_snprintf(buffer_minimal, sizeof(buffer_minimal), "zd: %zd\r\n", (ssize_t) INT32_MAX);
    result_baseline = make_test_string("zd: ", (ssize_t) INT32_MAX, BASE_10, "\r\n");
    TEST_ASSERT_EQUAL_STRING(expected_string, buffer_minimal);
    TEST_ASSERT_EQUAL_INT(result_baseline, result_minimal);
#endif

#ifdef TARGET_LIKE_MBED
    printf("%%td not supported by mbed\r\n");
#else
    result_minimal = mbed_snprintf(buffer_minimal, sizeof(buffer_minimal), "td: %td\r\n", PTRDIFF_MIN);
    result_baseline = make_test_string("td: ", PTRDIFF_MIN, BASE_10, "\r\n", true);
    TEST_ASSERT_EQUAL_STRING(expected_string, buffer_minimal);
    TEST_ASSERT_EQUAL_INT(result_baseline, result_minimal);

    result_minimal = mbed_snprintf(buffer_minimal, sizeof(buffer_minimal), "td: %td\r\n", PTRDIFF_MAX);
    result_baseline = make_test_string("td: ", PTRDIFF_MAX, BASE_10, "\r\n", true);
    TEST_ASSERT_EQUAL_STRING(expected_string, buffer_minimal);
    TEST_ASSERT_EQUAL_INT(result_baseline, result_minimal);
#endif

    int a = 2;
    int b = 3;
    result_minimal = mbed_snprintf(0, 0, "%d + %d = %d\n", a, b, a + b);
    TEST_ASSERT_EQUAL_INT(10, result_minimal);

    return CaseNext;
}

static control_t test_snprintf_u(const size_t call_count)
{
    char buffer_minimal[100];
    int result_baseline;
    int result_minimal;

    /*************************************************************************/
    /*************************************************************************/
    result_minimal = mbed_snprintf(buffer_minimal, sizeof(buffer_minimal), "hhu: %hhu\r\n", 0);
    result_baseline = make_test_string("hhu: ", 0, BASE_10, "\r\n");
    TEST_ASSERT_EQUAL_STRING(expected_string, buffer_minimal);
    TEST_ASSERT_EQUAL_INT(result_baseline, result_minimal);

    result_minimal = mbed_snprintf(buffer_minimal, sizeof(buffer_minimal), "hhu: %hhu\r\n", UCHAR_MAX);
    result_baseline = make_test_string("hhu: ", UCHAR_MAX, BASE_10, "\r\n");
    TEST_ASSERT_EQUAL_STRING(expected_string, buffer_minimal);
    TEST_ASSERT_EQUAL_INT(result_baseline, result_minimal);

    result_minimal = mbed_snprintf(buffer_minimal, sizeof(buffer_minimal), "hu: %hu\r\n", 0);
    result_baseline = make_test_string("hu: ", 0, BASE_10, "\r\n");
    TEST_ASSERT_EQUAL_STRING(expected_string, buffer_minimal);
    TEST_ASSERT_EQUAL_INT(result_baseline, result_minimal);

    result_minimal = mbed_snprintf(buffer_minimal, sizeof(buffer_minimal), "hu: %hu\r\n", USHRT_MAX);
    result_baseline = make_test_string("hu: ", USHRT_MAX, BASE_10, "\r\n");
    TEST_ASSERT_EQUAL_STRING(expected_string, buffer_minimal);
    TEST_ASSERT_EQUAL_INT(result_baseline, result_minimal);

    result_minimal = mbed_snprintf(buffer_minimal, sizeof(buffer_minimal), "u: %u\r\n", 0);
    result_baseline = make_test_string("u: ", 0, BASE_10, "\r\n");
    TEST_ASSERT_EQUAL_STRING(expected_string, buffer_minimal);
    TEST_ASSERT_EQUAL_INT(result_baseline, result_minimal);

    result_minimal = mbed_snprintf(buffer_minimal, sizeof(buffer_minimal), "u: %u\r\n", UINT_MAX);
    result_baseline = make_test_string("u: ", UINT_MAX, BASE_10, "\r\n");
    TEST_ASSERT_EQUAL_STRING(expected_string, buffer_minimal);
    TEST_ASSERT_EQUAL_INT(result_baseline, result_minimal);

    result_minimal = mbed_snprintf(buffer_minimal, sizeof(buffer_minimal), "lu: %lu\r\n", 0UL);
    result_baseline = make_test_string("lu: ", 0UL, BASE_10, "\r\n");
    TEST_ASSERT_EQUAL_STRING(expected_string, buffer_minimal);
    TEST_ASSERT_EQUAL_INT(result_baseline, result_minimal);

    result_minimal = mbed_snprintf(buffer_minimal, sizeof(buffer_minimal), "lu: %lu\r\n", ULONG_MAX);
    result_baseline = make_test_string("lu: ", ULONG_MAX, BASE_10, "\r\n");
    TEST_ASSERT_EQUAL_STRING(expected_string, buffer_minimal);
    TEST_ASSERT_EQUAL_INT(result_baseline, result_minimal);

    result_minimal = mbed_snprintf(buffer_minimal, sizeof(buffer_minimal), "llu: %llu\r\n", 0ULL);
    result_baseline = make_test_string("llu: ", 0ULL, BASE_10, "\r\n");
    TEST_ASSERT_EQUAL_STRING(expected_string, buffer_minimal);
    TEST_ASSERT_EQUAL_INT(result_baseline, result_minimal);

    result_minimal = mbed_snprintf(buffer_minimal, sizeof(buffer_minimal), "llu: %llu\r\n", ULLONG_MAX);
    result_baseline = make_test_string("llu: ", ULLONG_MAX, BASE_10, "\r\n");
    TEST_ASSERT_EQUAL_STRING(expected_string, buffer_minimal);
    TEST_ASSERT_EQUAL_INT(result_baseline, result_minimal);

#ifdef TARGET_LIKE_MBED
    printf("%%ju not supported by mbed\r\n");
#else
    result_minimal = mbed_snprintf(buffer_minimal, sizeof(buffer_minimal), "ju: %ju\r\n", (uintmax_t) 0);
    result_baseline = make_test_string("ju: ", (uintmax_t) 0, BASE_10, "\r\n");
    TEST_ASSERT_EQUAL_STRING(expected_string, buffer_minimal);
    TEST_ASSERT_EQUAL_INT(result_baseline, result_minimal);

    result_minimal = mbed_snprintf(buffer_minimal, sizeof(buffer_minimal), "ju: %ju\r\n", UINTMAX_MAX);
    result_baseline = make_test_string("ju: ", UINTMAX_MAX, BASE_10, "\r\n");
    TEST_ASSERT_EQUAL_STRING(expected_string, buffer_minimal);
    TEST_ASSERT_EQUAL_INT(result_baseline, result_minimal);
#endif

#ifdef TARGET_LIKE_MBED
    printf("%%zu not supported by mbed\r\n");
#else
    result_minimal = mbed_snprintf(buffer_minimal, sizeof(buffer_minimal), "zu: %zu\r\n", 0);
    result_baseline = make_test_string("zu: ", 0, BASE_10, "\r\n");
    TEST_ASSERT_EQUAL_STRING(expected_string, buffer_minimal);
    TEST_ASSERT_EQUAL_INT(result_baseline, result_minimal);

    result_minimal = mbed_snprintf(buffer_minimal, sizeof(buffer_minimal), "zu: %zu\r\n", SIZE_MAX);
    result_baseline = make_test_string("zu: ", SIZE_MAX, BASE_10, "\r\n");
    TEST_ASSERT_EQUAL_STRING(expected_string, buffer_minimal);
    TEST_ASSERT_EQUAL_INT(result_baseline, result_minimal);
#endif

#ifdef TARGET_LIKE_MBED
    printf("%%tu not supported by mbed\r\n");
#else
    result_minimal = mbed_snprintf(buffer_minimal, sizeof(buffer_minimal), "tu: %tu\r\n", 0);
    result_baseline = make_test_string("tu: ", 0, BASE_10, "\r\n");
    TEST_ASSERT_EQUAL_STRING(expected_string, buffer_minimal);
    TEST_ASSERT_EQUAL_INT(result_baseline, result_minimal);

    result_minimal = mbed_snprintf(buffer_minimal, sizeof(buffer_minimal), "tu: %tu\r\n", UINTPTR_MAX);
    result_baseline = make_test_string("tu: ", UINTPTR_MAX, BASE_10, "\r\n");
    TEST_ASSERT_EQUAL_STRING(expected_string, buffer_minimal);
    TEST_ASSERT_EQUAL_INT(result_baseline, result_minimal);
#endif

    return CaseNext;
}

static control_t test_snprintf_x(const size_t call_count)
{
    char buffer_minimal[100];
    int result_baseline;
    int result_minimal;

    /*************************************************************************/
    /*************************************************************************/
    result_minimal = mbed_snprintf(buffer_minimal, sizeof(buffer_minimal), "hhX: %hhX\r\n", 0);
    result_baseline = make_test_string("hhX: ", 0, BASE_16, "\r\n");
    TEST_ASSERT_EQUAL_STRING(expected_string, buffer_minimal);
    TEST_ASSERT_EQUAL_INT(result_baseline, result_minimal);

    result_minimal = mbed_snprintf(buffer_minimal, sizeof(buffer_minimal), "hhX: %hhX\r\n", UCHAR_MAX);
    result_baseline = make_test_string("hhX: ", UCHAR_MAX, BASE_16, "\r\n");
    TEST_ASSERT_EQUAL_STRING(expected_string, buffer_minimal);
    TEST_ASSERT_EQUAL_INT(result_baseline, result_minimal);

    result_minimal = mbed_snprintf(buffer_minimal, sizeof(buffer_minimal), "hX: %hX\r\n", 0);
    result_baseline = make_test_string("hX: ", 0, BASE_16, "\r\n");
    TEST_ASSERT_EQUAL_STRING(expected_string, buffer_minimal);
    TEST_ASSERT_EQUAL_INT(result_baseline, result_minimal);

    result_minimal = mbed_snprintf(buffer_minimal, sizeof(buffer_minimal), "hX: %hX\r\n", USHRT_MAX);
    result_baseline = make_test_string("hX: ", USHRT_MAX, BASE_16, "\r\n");
    TEST_ASSERT_EQUAL_INT(result_baseline, result_minimal);

    result_minimal = mbed_snprintf(buffer_minimal, sizeof(buffer_minimal), "X: %X\r\n", 0);
    result_baseline = make_test_string("X: ", 0, BASE_16, "\r\n");
    TEST_ASSERT_EQUAL_STRING(expected_string, buffer_minimal);
    TEST_ASSERT_EQUAL_INT(result_baseline, result_minimal);

    result_minimal = mbed_snprintf(buffer_minimal, sizeof(buffer_minimal), "X: %X\r\n", UINT_MAX);
    result_baseline = make_test_string("X: ", UINT_MAX, BASE_16, "\r\n");
    TEST_ASSERT_EQUAL_STRING(expected_string, buffer_minimal);
    TEST_ASSERT_EQUAL_INT(result_baseline, result_minimal);

    result_minimal = mbed_snprintf(buffer_minimal, sizeof(buffer_minimal), "lX: %lX\r\n", 0UL);
    result_baseline = make_test_string("lX: ", 0UL, BASE_16, "\r\n");
    TEST_ASSERT_EQUAL_STRING(expected_string, buffer_minimal);
    TEST_ASSERT_EQUAL_INT(result_baseline, result_minimal);

    result_minimal = mbed_snprintf(buffer_minimal, sizeof(buffer_minimal), "lX: %lX\r\n", ULONG_MAX);
    result_baseline = make_test_string("lX: ", ULONG_MAX, BASE_16, "\r\n");
    TEST_ASSERT_EQUAL_STRING(expected_string, buffer_minimal);
    TEST_ASSERT_EQUAL_INT(result_baseline, result_minimal);

    result_minimal = mbed_snprintf(buffer_minimal, sizeof(buffer_minimal), "llX: %llX\r\n", 0ULL);
    result_baseline = make_test_string("llX: ", 0ULL, BASE_16, "\r\n");
    TEST_ASSERT_EQUAL_STRING(expected_string, buffer_minimal);
    TEST_ASSERT_EQUAL_INT(result_baseline, result_minimal);

    result_minimal = mbed_snprintf(buffer_minimal, sizeof(buffer_minimal), "llX: %llX\r\n", ULLONG_MAX);
    result_baseline = make_test_string("llX: ", ULLONG_MAX, BASE_16, "\r\n");
    TEST_ASSERT_EQUAL_STRING(expected_string, buffer_minimal);
    TEST_ASSERT_EQUAL_INT(result_baseline, result_minimal);

#ifdef TARGET_LIKE_MBED
    printf("%%jX not supported by mbed\r\n");
#else
    result_minimal = mbed_snprintf(buffer_minimal, sizeof(buffer_minimal), "jX: %jX\r\n", (uintmax_t) 0);
    result_baseline = make_test_string("jX: ", (uintmax_t) 0, BASE_16, "\r\n");
    TEST_ASSERT_EQUAL_STRING(expected_string, buffer_minimal);
    TEST_ASSERT_EQUAL_INT(result_baseline, result_minimal);

    result_minimal = mbed_snprintf(buffer_minimal, sizeof(buffer_minimal), "jX: %jX\r\n", UINTMAX_MAX);
    result_baseline = make_test_string("jX: ", UINTMAX_MAX, BASE_16, "\r\n");
    TEST_ASSERT_EQUAL_STRING(expected_string, buffer_minimal);
    TEST_ASSERT_EQUAL_INT(result_baseline, result_minimal);
#endif

#ifdef TARGET_LIKE_MBED
    printf("%%xX not supported by mbed\r\n");
#else
    result_minimal = mbed_snprintf(buffer_minimal, sizeof(buffer_minimal), "zX: %zX\r\n", 0);
    result_baseline = make_test_string("zX: ", 0, BASE_16, "\r\n");
    TEST_ASSERT_EQUAL_STRING(expected_string, buffer_minimal);
    TEST_ASSERT_EQUAL_INT(result_baseline, result_minimal);

    result_minimal = mbed_snprintf(buffer_minimal, sizeof(buffer_minimal), "zX: %zX\r\n", SIZE_MAX);
    result_baseline = make_test_string("zX: ", SIZE_MAX, BASE_16, "\r\n");
    TEST_ASSERT_EQUAL_STRING(expected_string, buffer_minimal);
    TEST_ASSERT_EQUAL_INT(result_baseline, result_minimal);
#endif

#ifdef TARGET_LIKE_MBED
    printf("%%tX not supported by mbed\r\n");
#else
    result_minimal = mbed_snprintf(buffer_minimal, sizeof(buffer_minimal), "tX: %tX\r\n", 0);
    result_baseline = make_test_string("tX: ", 0, BASE_16, "\r\n");
    TEST_ASSERT_EQUAL_STRING(expected_string, buffer_minimal);
    TEST_ASSERT_EQUAL_INT(result_baseline, result_minimal);

    result_minimal = mbed_snprintf(buffer_minimal, sizeof(buffer_minimal), "tX: %tX\r\n", UINTPTR_MAX);
    result_baseline = make_test_string("tX: ", UINTPTR_MAX, BASE_16, "\r\n");
    TEST_ASSERT_EQUAL_STRING(expected_string, buffer_minimal);
    TEST_ASSERT_EQUAL_INT(result_baseline, result_minimal);
#endif

    return CaseNext;
}

static control_t test_snprintf_percent(const size_t call_count)
{
    char buffer_baseline[100];
    char buffer_minimal[100];
    int result_baseline;
    int result_minimal;

    result_minimal = mbed_snprintf(buffer_minimal, sizeof(buffer_minimal), "%% \r\n");
    result_baseline = sprintf(buffer_baseline, "%% \r\n");
    TEST_ASSERT_EQUAL_STRING(buffer_baseline, buffer_minimal);
    TEST_ASSERT_EQUAL_INT(result_baseline, result_minimal);

    return CaseNext;
}

static control_t test_snprintf_unsupported_specifier(const size_t call_count)
{
    char buffer_minimal[100];

    TEST_ASSERT_NOT_EQUAL(
        0,
        mbed_snprintf(buffer_minimal, sizeof(buffer_minimal), "%a \r\n", 5)
    );
    TEST_ASSERT_EQUAL_STRING("%a \r\n", buffer_minimal);

    return CaseNext;
}

#if MBED_CONF_PLATFORM_MINIMAL_PRINTF_ENABLE_FLOATING_POINT
static control_t test_printf_f(const size_t call_count)
{
    char buffer_baseline[100];
    int result_baseline;
    int result_minimal;
#define CLEAN_BUFFER memset(buffer_baseline, 0x00, sizeof(buffer_baseline))
    /*************************************************************************/
    /*************************************************************************/
    double pi = 3.14159265359;
    CLEAN_BUFFER;
    result_minimal = mbed_printf("f: %f\r\n", 3.0089);
    result_baseline = sprintf(buffer_baseline, "f: 3.008900\r\n");
    TEST_ASSERT_EQUAL_INT(result_baseline, result_minimal);
    CLEAN_BUFFER;
    result_minimal = mbed_printf("f: %f\r\n", 7.0);
    result_baseline = sprintf(buffer_baseline, "f: 7.000000\r\n");
    TEST_ASSERT_EQUAL_INT(result_baseline, result_minimal);
    CLEAN_BUFFER;
    result_minimal = mbed_printf("f: %f\r\n", -1 * pi);
    result_baseline = sprintf(buffer_baseline, "f: -3.141593\r\n");
    TEST_ASSERT_EQUAL_INT(result_baseline, result_minimal);
    CLEAN_BUFFER;
    result_minimal = mbed_printf("f: %f\r\n", 0.0);
    result_baseline = sprintf(buffer_baseline, "f: 0.000000\r\n");
    TEST_ASSERT_EQUAL_INT(result_baseline, result_minimal);
    CLEAN_BUFFER;
    result_minimal = mbed_printf("f: %f\r\n", pi);
    result_baseline = sprintf(buffer_baseline, "f: 3.141593\r\n");
    TEST_ASSERT_EQUAL_INT(result_baseline, result_minimal);
    CLEAN_BUFFER;
    result_minimal = mbed_printf("f: %f\r\n", 2.12345651);
    result_baseline = sprintf(buffer_baseline, "f: 2.123457\r\n");
    TEST_ASSERT_EQUAL_INT(result_baseline, result_minimal);
    CLEAN_BUFFER;
    result_minimal = mbed_printf("f: %f\r\n", 2.12345649);
    result_baseline = sprintf(buffer_baseline, "f: 2.123456\r\n");
    TEST_ASSERT_EQUAL_INT(result_baseline, result_minimal);
    return CaseNext;
}

static control_t test_snprintf_f(const size_t call_count)
{
    char buffer_baseline[100];
    char buffer_minimal[100];
    int result_baseline;
    int result_minimal;
#define CLEAN_BUFFER memset(buffer_baseline, 0x00, sizeof(buffer_baseline))
    /*************************************************************************/
    /*************************************************************************/

    double pi = 3.14159265359;
    CLEAN_BUFFER;
    result_minimal = mbed_snprintf(buffer_minimal, sizeof(buffer_minimal), "f: %f\r\n", 3.0089);
    result_baseline = sprintf(buffer_baseline, "f: 3.008900\r\n");
    TEST_ASSERT_EQUAL_STRING(buffer_baseline, buffer_minimal);
    TEST_ASSERT_EQUAL_INT(result_baseline, result_minimal);
    CLEAN_BUFFER;
    result_minimal = mbed_snprintf(buffer_minimal, sizeof(buffer_minimal), "f: %f\r\n", 7.0);
    result_baseline = sprintf(buffer_baseline, "f: 7.000000\r\n");
    TEST_ASSERT_EQUAL_STRING(buffer_baseline, buffer_minimal);
    TEST_ASSERT_EQUAL_INT(result_baseline, result_minimal);
    CLEAN_BUFFER;
    result_minimal = mbed_snprintf(buffer_minimal, sizeof(buffer_minimal), "f: %f\r\n", -1 * pi);
    result_baseline = sprintf(buffer_baseline, "f: -3.141593\r\n");
    TEST_ASSERT_EQUAL_STRING(buffer_baseline, buffer_minimal);
    TEST_ASSERT_EQUAL_INT(result_baseline, result_minimal);
    CLEAN_BUFFER;
    result_minimal = mbed_snprintf(buffer_minimal, sizeof(buffer_minimal), "f: %f\r\n", 0.0);
    result_baseline = sprintf(buffer_baseline, "f: 0.000000\r\n");
    TEST_ASSERT_EQUAL_STRING(buffer_baseline, buffer_minimal);
    TEST_ASSERT_EQUAL_INT(result_baseline, result_minimal);
    CLEAN_BUFFER;
    result_minimal = mbed_snprintf(buffer_minimal, sizeof(buffer_minimal), "f: %f\r\n", pi);
    result_baseline = sprintf(buffer_baseline, "f: 3.141593\r\n");
    TEST_ASSERT_EQUAL_STRING(buffer_baseline, buffer_minimal);
    TEST_ASSERT_EQUAL_INT(result_baseline, result_minimal);

    return CaseNext;
}
#endif


/* Generic buffer overflow test function.
 * Template parameters:
    * 'T' is the type being tested
    * 'buf_size' is the buffer size used in tests
    * 'base' is used to represent data in BASE_10 or BASE_16 numeral system
  * Function parameters:
    * 'fmt' is the format to use for sprintf
    * 'data' is the data that will be printed
    * 'is_negative' is true for negative number, false for positive number
*/
template<typename T, size_t buf_size, int base = BASE_10>
static control_t test_snprintf_buffer_overflow_generic(const char *fmt, T data, bool is_negative = false)
{
    char buffer_baseline[buf_size];
    char buffer_minimal[buf_size];
    int result_baseline;
    int result_minimal;
    char prefix[buf_size] = { 0 };
    // fmt string has "format specifier: %format specifier"
    // Extract the string preceding the first '%' from fmt.
    extract_prefix(fmt, &prefix[0]);
    result_baseline = make_test_string(prefix, data, base, nullptr, is_negative);

    /* empty buffer test */
    result_minimal = mbed_snprintf(buffer_minimal, 0, fmt, data);
    TEST_ASSERT_EQUAL_INT(result_baseline, result_minimal);

    /* buffer isn't large enough, output needs to be truncated */
    result_minimal = mbed_snprintf(buffer_minimal, buf_size - 2, fmt, data);
    snprintf(buffer_baseline, buf_size - 2, "%s", expected_string);
    TEST_ASSERT_EQUAL_STRING(buffer_baseline, buffer_minimal);
    TEST_ASSERT_EQUAL_INT(result_baseline, result_minimal);

    /* buffer is one byte shorter than needed, string terminator must
       be written and output must be truncated */
    result_minimal = mbed_snprintf(buffer_minimal, buf_size - 1, fmt, data);
    snprintf(buffer_baseline, buf_size - 1, "%s", expected_string);
    TEST_ASSERT_EQUAL_STRING(buffer_baseline, buffer_minimal);
    TEST_ASSERT_EQUAL_INT(result_baseline, result_minimal);

    /* buffer is just long enough */
    result_minimal = mbed_snprintf(buffer_minimal, buf_size, fmt, data);
    snprintf(buffer_baseline, buf_size, "%s", expected_string);
    TEST_ASSERT_EQUAL_STRING(buffer_baseline, buffer_minimal);
    TEST_ASSERT_EQUAL_INT(result_baseline, result_minimal);

    return CaseNext;
}

/* Based on the generic buffer overflow function above, create tests for
   each relevant data type. In each case, the buffer for printing will only
   be large enough to fit the printed data. */
static control_t test_snprintf_buffer_overflow_d(const size_t call_count)
{
    return test_snprintf_buffer_overflow_generic<int, sizeof("d: -1024")>("d: %d", -1024, true);
}

static control_t test_snprintf_buffer_overflow_ld(const size_t call_count)
{
    return test_snprintf_buffer_overflow_generic<long, sizeof("ld: -1048576")>("ld: %ld", -1048576L, true);
}

static control_t test_snprintf_buffer_overflow_lld(const size_t call_count)
{
    return test_snprintf_buffer_overflow_generic<long long, sizeof("lld: -1099511627776")>("lld: %lld", -1099511627776LL, true);
}

static control_t test_snprintf_buffer_overflow_u(const size_t call_count)
{
    return test_snprintf_buffer_overflow_generic<unsigned int, sizeof("u: 1024")>("u: %u", 1024);
}

static control_t test_snprintf_buffer_overflow_lu(const size_t call_count)
{
    return test_snprintf_buffer_overflow_generic<unsigned long, sizeof("lu: 1048576")>("lu: %lu", 1048576UL);
}

static control_t test_snprintf_buffer_overflow_llu(const size_t call_count)
{
    return test_snprintf_buffer_overflow_generic<unsigned long long, sizeof("llu: 1099511627776")>("llu: %llu", 1099511627776ULL);
}

static control_t test_snprintf_buffer_overflow_x(const size_t call_count)
{
    return test_snprintf_buffer_overflow_generic<unsigned int, sizeof("x: 0x400"), BASE_16>("x: 0x%x", 0x400);
}

static control_t test_snprintf_buffer_overflow_lx(const size_t call_count)
{
    return test_snprintf_buffer_overflow_generic<unsigned long, sizeof("lx: 0x100000"), BASE_16>("lx: 0x%lx", 0x100000UL);
}

static control_t test_snprintf_buffer_overflow_llx(const size_t call_count)
{
    return test_snprintf_buffer_overflow_generic<unsigned long long, sizeof("llx: 0x10000000000"), BASE_16>("llx: 0x%llx", 0x10000000000ULL);
}

utest::v1::status_t greentea_setup(const size_t number_of_cases)
{
    GREENTEA_SETUP(30 * 60, "default_auto");
    return greentea_test_setup_handler(number_of_cases);
}

Case cases[] = {
    Case("printf %d", test_printf_d),
    Case("snprintf %d", test_snprintf_d),
    Case("printf %u", test_printf_u),
    Case("snprintf %u", test_snprintf_u),
    Case("printf %x", test_printf_x),
    Case("snprintf %x", test_snprintf_x),
    Case("printf %%", test_printf_percent),
    Case("snprintf %%", test_snprintf_percent),
    Case("snprintf unsupported specifier", test_snprintf_unsupported_specifier),
#if MBED_CONF_PLATFORM_MINIMAL_PRINTF_ENABLE_FLOATING_POINT
    Case("printf %f", test_printf_f),
    Case("snprintf %f", test_snprintf_f),
#endif
    Case("snprintf buffer overflow %d", test_snprintf_buffer_overflow_d),
    Case("snprintf buffer overflow %ld", test_snprintf_buffer_overflow_ld),
    Case("snprintf buffer overflow %lld", test_snprintf_buffer_overflow_lld),
    Case("snprintf buffer overflow %u", test_snprintf_buffer_overflow_u),
    Case("snprintf buffer overflow %lu", test_snprintf_buffer_overflow_lu),
    Case("snprintf buffer overflow %llu", test_snprintf_buffer_overflow_llu),
    Case("snprintf buffer overflow %x", test_snprintf_buffer_overflow_x),
    Case("snprintf buffer overflow %lx", test_snprintf_buffer_overflow_lx),
    Case("snprintf buffer overflow %llx", test_snprintf_buffer_overflow_llx),
};

Specification specification(greentea_setup, cases, greentea_test_teardown_handler);

int main()
{
    return !Harness::run(specification);
}
