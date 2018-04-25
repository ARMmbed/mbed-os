/* mbed Microcontroller Library
 * Copyright (c) 2017 ARM Limited
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


static control_t test_printf_d(const size_t call_count)
{
    int result_baseline;
    int result_minimal;

    /*************************************************************************/
    /*************************************************************************/
    result_minimal = mbed_printf("hhd: %hhd\r\n", SCHAR_MIN);
    result_baseline = printf("hhd: %hhd\r\n", SCHAR_MIN);
    TEST_ASSERT_EQUAL_INT(result_baseline, result_minimal);

    result_minimal = mbed_printf("hhd: %hhd\r\n", SCHAR_MAX);
    result_baseline = printf("hhd: %hhd\r\n", SCHAR_MAX);
    TEST_ASSERT_EQUAL_INT(result_baseline, result_minimal);

    result_minimal = mbed_printf("hd: %hd\r\n", SHRT_MIN);
    result_baseline = printf("hd: %hd\r\n", SHRT_MIN);
    TEST_ASSERT_EQUAL_INT(result_baseline, result_minimal);

    result_minimal = mbed_printf("hd: %hd\r\n", SHRT_MAX);
    result_baseline = printf("hd: %hd\r\n", SHRT_MAX);
    TEST_ASSERT_EQUAL_INT(result_baseline, result_minimal);

    result_minimal = mbed_printf("d: %d\r\n", INT_MIN);
    result_baseline = printf("d: %d\r\n", INT_MIN);
    TEST_ASSERT_EQUAL_INT(result_baseline, result_minimal);

    result_minimal = mbed_printf("d: %d\r\n", INT_MAX);
    result_baseline = printf("d: %d\r\n", INT_MAX);
    TEST_ASSERT_EQUAL_INT(result_baseline, result_minimal);

    result_minimal = mbed_printf("ld: %ld\r\n", LONG_MIN);
    result_baseline = printf("ld: %ld\r\n", LONG_MIN);
    TEST_ASSERT_EQUAL_INT(result_baseline, result_minimal);

    result_minimal = mbed_printf("ld: %ld\r\n", LONG_MAX);
    result_baseline = printf("ld: %ld\r\n", LONG_MAX);
    TEST_ASSERT_EQUAL_INT(result_baseline, result_minimal);

    result_minimal = mbed_printf("lld: %lld\r\n", LLONG_MIN);
    result_baseline = printf("lld: %lld\r\n", LLONG_MIN);
    TEST_ASSERT_EQUAL_INT(result_baseline, result_minimal);

    result_minimal = mbed_printf("lld: %lld\r\n", LLONG_MAX);
    result_baseline = printf("lld: %lld\r\n", LLONG_MAX);
    TEST_ASSERT_EQUAL_INT(result_baseline, result_minimal);

#ifdef TARGET_LIKE_MBED
    printf("%%jd not supported by mbed\r\n");
#else
    result_minimal = mbed_printf("jd: %jd\r\n", INT32_MIN);
    result_baseline = printf("jd: %jd\r\n", (intmax_t) INT32_MIN);
    TEST_ASSERT_EQUAL_INT(result_baseline, result_minimal);

    result_minimal = mbed_printf("jd: %jd\r\n", INT32_MAX);
    result_baseline = printf("jd: %jd\r\n", (intmax_t) INT32_MAX);
    TEST_ASSERT_EQUAL_INT(result_baseline, result_minimal);
#endif

#ifdef TARGET_LIKE_MBED
    printf("%%zd not supported by mbed\r\n");
#else
    result_minimal = mbed_printf("zd: %zd\r\n", INT32_MIN);
    result_baseline = printf("zd: %zd\r\n", (ssize_t) INT32_MIN);
    TEST_ASSERT_EQUAL_INT(result_baseline, result_minimal);

    result_minimal = mbed_printf("zd: %zd\r\n", INT32_MAX);
    result_baseline = printf("zd: %zd\r\n", (ssize_t) INT32_MAX);
    TEST_ASSERT_EQUAL_INT(result_baseline, result_minimal);
#endif

#ifdef TARGET_LIKE_MBED
    printf("%%td not supported by mbed\r\n");
#else
    result_minimal = mbed_printf("td: %td\r\n", PTRDIFF_MIN);
    result_baseline = printf("td: %td\r\n", PTRDIFF_MIN);
    TEST_ASSERT_EQUAL_INT(result_baseline, result_minimal);

    result_minimal = mbed_printf("td: %td\r\n", PTRDIFF_MAX);
    result_baseline = printf("td: %td\r\n", PTRDIFF_MAX);
    TEST_ASSERT_EQUAL_INT(result_baseline, result_minimal);
#endif

    return CaseNext;
}

static control_t test_printf_u(const size_t call_count)
{
    int result_baseline;
    int result_minimal;

    /*************************************************************************/
    /*************************************************************************/
    result_minimal = mbed_printf("hhu: %hhu\r\n", 0);
    result_baseline = printf("hhu: %hhu\r\n", 0);
    TEST_ASSERT_EQUAL_INT(result_baseline, result_minimal);

    result_minimal = mbed_printf("hhu: %hhu\r\n", UCHAR_MAX);
    result_baseline = printf("hhu: %hhu\r\n", UCHAR_MAX);
    TEST_ASSERT_EQUAL_INT(result_baseline, result_minimal);

    result_minimal = mbed_printf("hu: %hu\r\n", 0);
    result_baseline = printf("hu: %hu\r\n", 0);
    TEST_ASSERT_EQUAL_INT(result_baseline, result_minimal);

    result_minimal = mbed_printf("hu: %hu\r\n", USHRT_MAX);
    result_baseline = printf("hu: %hu\r\n", USHRT_MAX);
    TEST_ASSERT_EQUAL_INT(result_baseline, result_minimal);

    result_minimal = mbed_printf("u: %u\r\n", 0);
    result_baseline = printf("u: %u\r\n", 0);
    TEST_ASSERT_EQUAL_INT(result_baseline, result_minimal);

    result_minimal = mbed_printf("u: %u\r\n", UINT_MAX);
    result_baseline = printf("u: %u\r\n", UINT_MAX);
    TEST_ASSERT_EQUAL_INT(result_baseline, result_minimal);

    result_minimal = mbed_printf("lu: %lu\r\n", 0);
    result_baseline = printf("lu: %lu\r\n", 0UL);
    TEST_ASSERT_EQUAL_INT(result_baseline, result_minimal);

    result_minimal = mbed_printf("lu: %lu\r\n", ULONG_MAX);
    result_baseline = printf("lu: %lu\r\n", ULONG_MAX);
    TEST_ASSERT_EQUAL_INT(result_baseline, result_minimal);

    result_minimal = mbed_printf("llu: %llu\r\n", 0);
    result_baseline = printf("llu: %llu\r\n", 0ULL);
    TEST_ASSERT_EQUAL_INT(result_baseline, result_minimal);

    result_minimal = mbed_printf("llu: %llu\r\n", ULLONG_MAX);
    result_baseline = printf("llu: %llu\r\n", ULLONG_MAX);
    TEST_ASSERT_EQUAL_INT(result_baseline, result_minimal);

#ifdef TARGET_LIKE_MBED
    printf("%%ju not supported by mbed\r\n");
#else
    result_minimal = mbed_printf("ju: %ju\r\n", 0);
    result_baseline = printf("ju: %ju\r\n",(uintmax_t) 0);
    TEST_ASSERT_EQUAL_INT(result_baseline, result_minimal);

    result_minimal = mbed_printf("ju: %ju\r\n", UINTMAX_MAX);
    result_baseline = printf("ju: %ju\r\n", UINTMAX_MAX);
    TEST_ASSERT_EQUAL_INT(result_baseline, result_minimal);
#endif

#ifdef TARGET_LIKE_MBED
    printf("%%zu not supported by mbed\r\n");
#else
    result_minimal = mbed_printf("zu: %zu\r\n", 0);
    result_baseline = printf("zu: %zu\r\n", 0);
    TEST_ASSERT_EQUAL_INT(result_baseline, result_minimal);

    result_minimal = mbed_printf("zu: %zu\r\n", SIZE_MAX);
    result_baseline = printf("zu: %zu\r\n", SIZE_MAX);
    TEST_ASSERT_EQUAL_INT(result_baseline, result_minimal);
#endif

#ifdef TARGET_LIKE_MBED
    printf("%%tu not supported by mbed\r\n");
#else
    result_minimal = mbed_printf("tu: %tu\r\n", 0);
    result_baseline = printf("tu: %tu\r\n", 0);
    TEST_ASSERT_EQUAL_INT(result_baseline, result_minimal);

    result_minimal = mbed_printf("tu: %tu\r\n", UINTPTR_MAX);
    result_baseline = printf("tu: %tu\r\n", UINTPTR_MAX);
    TEST_ASSERT_EQUAL_INT(result_baseline, result_minimal);
#endif

    return CaseNext;
}

static control_t test_printf_x(const size_t call_count)
{
    int result_baseline;
    int result_minimal;

    /*************************************************************************/
    /*************************************************************************/
    result_minimal = mbed_printf("hhX: %hhX\r\n", 0);
    result_baseline = printf("hhX: %hhX\r\n", 0);
    TEST_ASSERT_EQUAL_INT(result_baseline, result_minimal);

    result_minimal = mbed_printf("hhX: %hhX\r\n", UCHAR_MAX);
    result_baseline = printf("hhX: %hhX\r\n", UCHAR_MAX);
    TEST_ASSERT_EQUAL_INT(result_baseline, result_minimal);

    result_minimal = mbed_printf("hX: %hX\r\n", 0);
    result_baseline = printf("hX: %hX\r\n", 0);
    TEST_ASSERT_EQUAL_INT(result_baseline, result_minimal);

    result_minimal = mbed_printf("hX: %hX\r\n", USHRT_MAX);
    result_baseline = printf("hX: %hX\r\n", USHRT_MAX);
    TEST_ASSERT_EQUAL_INT(result_baseline, result_minimal);

    result_minimal = mbed_printf("X: %X\r\n", 0);
    result_baseline = printf("X: %X\r\n", 0);
    TEST_ASSERT_EQUAL_INT(result_baseline, result_minimal);

    result_minimal = mbed_printf("X: %X\r\n", UINT_MAX);
    result_baseline = printf("X: %X\r\n", UINT_MAX);
    TEST_ASSERT_EQUAL_INT(result_baseline, result_minimal);

    result_minimal = mbed_printf("lX: %lX\r\n", 0);
    result_baseline = printf("lX: %lX\r\n", 0UL);
    TEST_ASSERT_EQUAL_INT(result_baseline, result_minimal);

    result_minimal = mbed_printf("lX: %lX\r\n", ULONG_MAX);
    result_baseline = printf("lX: %lX\r\n", ULONG_MAX);
    TEST_ASSERT_EQUAL_INT(result_baseline, result_minimal);

    result_minimal = mbed_printf("llX: %llX\r\n", 0);
    result_baseline = printf("llX: %llX\r\n", 0ULL);
    TEST_ASSERT_EQUAL_INT(result_baseline, result_minimal);

    result_minimal = mbed_printf("llX: %llX\r\n", ULLONG_MAX);
    result_baseline = printf("llX: %llX\r\n", ULLONG_MAX);
    TEST_ASSERT_EQUAL_INT(result_baseline, result_minimal);

#ifdef TARGET_LIKE_MBED
    printf("%%jX not supported by mbed\r\n");
#else
    result_minimal = mbed_printf("jX: %jX\r\n", 0);
    result_baseline = printf("jX: %jX\r\n", (uintmax_t) 0);
    TEST_ASSERT_EQUAL_INT(result_baseline, result_minimal);

    result_minimal = mbed_printf("jX: %jX\r\n", UINTMAX_MAX);
    result_baseline = printf("jX: %jX\r\n", UINTMAX_MAX);
    TEST_ASSERT_EQUAL_INT(result_baseline, result_minimal);
#endif

#ifdef TARGET_LIKE_MBED
    printf("%%zX not supported by mbed\r\n");
#else
    result_minimal = mbed_printf("zX: %zX\r\n", 0);
    result_baseline = printf("zX: %zX\r\n", 0);
    TEST_ASSERT_EQUAL_INT(result_baseline, result_minimal);

    result_minimal = mbed_printf("zX: %zX\r\n", SIZE_MAX);
    result_baseline = printf("zX: %zX\r\n", SIZE_MAX);
    TEST_ASSERT_EQUAL_INT(result_baseline, result_minimal);
#endif

#ifdef TARGET_LIKE_MBED
    printf("%%tX not supported by mbed\r\n");
#else
    result_minimal = mbed_printf("tX: %tX\r\n", 0);
    result_baseline = printf("tX: %tX\r\n", 0);
    TEST_ASSERT_EQUAL_INT(result_baseline, result_minimal);

    result_minimal = mbed_printf("tX: %tX\r\n", UINTPTR_MAX);
    result_baseline = printf("tX: %tX\r\n", UINTPTR_MAX);
    TEST_ASSERT_EQUAL_INT(result_baseline, result_minimal);
#endif

    return CaseNext;
}

/******************************************************************************/
/*                                                                            */
/* SNPRINTF                                                                   */
/*                                                                            */
/******************************************************************************/

static control_t test_snprintf_d(const size_t call_count)
{
    char buffer_baseline[100];
    char buffer_minimal[100];
    int result_baseline;
    int result_minimal;

    /*************************************************************************/
    /*************************************************************************/
    result_minimal = mbed_snprintf(buffer_minimal, sizeof(buffer_minimal), "hhd: %hhd\r\n", SCHAR_MIN);
    result_baseline = snprintf(buffer_baseline, sizeof(buffer_baseline), "hhd: %hhd\r\n", SCHAR_MIN);
    TEST_ASSERT_EQUAL_STRING(buffer_baseline, buffer_minimal);
    TEST_ASSERT_EQUAL_INT(result_baseline, result_minimal);

    result_minimal = mbed_snprintf(buffer_minimal, sizeof(buffer_minimal), "hhd: %hhd\r\n", SCHAR_MAX);
    result_baseline = snprintf(buffer_baseline, sizeof(buffer_baseline), "hhd: %hhd\r\n", SCHAR_MAX);
    TEST_ASSERT_EQUAL_STRING(buffer_baseline, buffer_minimal);
    TEST_ASSERT_EQUAL_INT(result_baseline, result_minimal);

    result_minimal = mbed_snprintf(buffer_minimal, sizeof(buffer_minimal), "hd: %hd\r\n", SHRT_MIN);
    result_baseline = snprintf(buffer_baseline, sizeof(buffer_baseline), "hd: %hd\r\n", SHRT_MIN);
    TEST_ASSERT_EQUAL_STRING(buffer_baseline, buffer_minimal);
    TEST_ASSERT_EQUAL_INT(result_baseline, result_minimal);

    result_minimal = mbed_snprintf(buffer_minimal, sizeof(buffer_minimal), "hd: %hd\r\n", SHRT_MAX);
    result_baseline = snprintf(buffer_baseline, sizeof(buffer_baseline), "hd: %hd\r\n", SHRT_MAX);
    TEST_ASSERT_EQUAL_STRING(buffer_baseline, buffer_minimal);
    TEST_ASSERT_EQUAL_INT(result_baseline, result_minimal);

    result_minimal = mbed_snprintf(buffer_minimal, sizeof(buffer_minimal), "d: %d\r\n", INT_MIN);
    result_baseline = snprintf(buffer_baseline, sizeof(buffer_baseline), "d: %d\r\n", INT_MIN);
    TEST_ASSERT_EQUAL_STRING(buffer_baseline, buffer_minimal);
    TEST_ASSERT_EQUAL_INT(result_baseline, result_minimal);

    result_minimal = mbed_snprintf(buffer_minimal, sizeof(buffer_minimal), "d: %d\r\n", INT_MAX);
    result_baseline = snprintf(buffer_baseline, sizeof(buffer_baseline), "d: %d\r\n", INT_MAX);
    TEST_ASSERT_EQUAL_STRING(buffer_baseline, buffer_minimal);
    TEST_ASSERT_EQUAL_INT(result_baseline, result_minimal);

    result_minimal = mbed_snprintf(buffer_minimal, sizeof(buffer_minimal), "ld: %ld\r\n", LONG_MIN);
    result_baseline = snprintf(buffer_baseline, sizeof(buffer_baseline), "ld: %ld\r\n", LONG_MIN);
    TEST_ASSERT_EQUAL_STRING(buffer_baseline, buffer_minimal);
    TEST_ASSERT_EQUAL_INT(result_baseline, result_minimal);

    result_minimal = mbed_snprintf(buffer_minimal, sizeof(buffer_minimal), "ld: %ld\r\n", LONG_MAX);
    result_baseline = snprintf(buffer_baseline, sizeof(buffer_baseline), "ld: %ld\r\n", LONG_MAX);
    TEST_ASSERT_EQUAL_STRING(buffer_baseline, buffer_minimal);
    TEST_ASSERT_EQUAL_INT(result_baseline, result_minimal);

    result_minimal = mbed_snprintf(buffer_minimal, sizeof(buffer_minimal), "lld: %lld\r\n", LLONG_MIN);
    result_baseline = snprintf(buffer_baseline, sizeof(buffer_baseline), "lld: %lld\r\n", LLONG_MIN);
    TEST_ASSERT_EQUAL_STRING(buffer_baseline, buffer_minimal);
    TEST_ASSERT_EQUAL_INT(result_baseline, result_minimal);

    result_minimal = mbed_snprintf(buffer_minimal, sizeof(buffer_minimal), "lld: %lld\r\n", LLONG_MAX);
    result_baseline = snprintf(buffer_baseline, sizeof(buffer_baseline), "lld: %lld\r\n", LLONG_MAX);
    TEST_ASSERT_EQUAL_STRING(buffer_baseline, buffer_minimal);
    TEST_ASSERT_EQUAL_INT(result_baseline, result_minimal);

#ifdef TARGET_LIKE_MBED
    printf("%%jd not supported by mbed\r\n");
#else
    result_minimal = mbed_snprintf(buffer_minimal, sizeof(buffer_minimal), "jd: %jd\r\n", INT32_MIN);
    result_baseline = snprintf(buffer_baseline, sizeof(buffer_baseline), "jd: %jd\r\n", (intmax_t) INT32_MIN);
    TEST_ASSERT_EQUAL_STRING("jd: -2147483648\r\n", buffer_minimal);
    TEST_ASSERT_EQUAL_INT(result_baseline, result_minimal);

    result_minimal = mbed_snprintf(buffer_minimal, sizeof(buffer_minimal), "jd: %jd\r\n", INT32_MAX);
    result_baseline = snprintf(buffer_baseline, sizeof(buffer_baseline), "jd: %jd\r\n", (intmax_t) INT32_MAX);
    TEST_ASSERT_EQUAL_STRING("jd: 2147483647\r\n", buffer_minimal);
    TEST_ASSERT_EQUAL_INT(result_baseline, result_minimal);
#endif

#ifdef TARGET_LIKE_MBED
    printf("%%zd not supported by mbed\r\n");
#else
    result_minimal = mbed_snprintf(buffer_minimal, sizeof(buffer_minimal), "zd: %zd\r\n", INT32_MIN);
    result_baseline = snprintf(buffer_baseline, sizeof(buffer_baseline), "zd: %zd\r\n", (ssize_t) INT32_MIN);
    TEST_ASSERT_EQUAL_STRING("zd: -2147483648\r\n", buffer_minimal);
    TEST_ASSERT_EQUAL_INT(result_baseline, result_minimal);

    result_minimal = mbed_snprintf(buffer_minimal, sizeof(buffer_minimal), "zd: %zd\r\n", INT32_MAX);
    result_baseline = snprintf(buffer_baseline, sizeof(buffer_baseline), "zd: %zd\r\n", (ssize_t) INT32_MAX);
    TEST_ASSERT_EQUAL_STRING("zd: 2147483647\r\n", buffer_minimal);
    TEST_ASSERT_EQUAL_INT(result_baseline, result_minimal);
#endif

#ifdef TARGET_LIKE_MBED
    printf("%%td not supported by mbed\r\n");
#else
    result_minimal = mbed_snprintf(buffer_minimal, sizeof(buffer_minimal), "td: %td\r\n", PTRDIFF_MIN);
    result_baseline = snprintf(buffer_baseline, sizeof(buffer_baseline), "td: %td\r\n", PTRDIFF_MIN);
    TEST_ASSERT_EQUAL_STRING("td: -2147483648\r\n", buffer_minimal);
    TEST_ASSERT_EQUAL_INT(result_baseline, result_minimal);

    result_minimal = mbed_snprintf(buffer_minimal, sizeof(buffer_minimal), "td: %td\r\n", PTRDIFF_MAX);
    result_baseline = snprintf(buffer_baseline, sizeof(buffer_baseline), "td: %td\r\n", PTRDIFF_MAX);
    TEST_ASSERT_EQUAL_STRING("td: 2147483647\r\n", buffer_minimal);
    TEST_ASSERT_EQUAL_INT(result_baseline, result_minimal);
#endif

    return CaseNext;
}

static control_t test_snprintf_u(const size_t call_count)
{
    char buffer_baseline[100];
    char buffer_minimal[100];
    int result_baseline;
    int result_minimal;

    /*************************************************************************/
    /*************************************************************************/
    result_minimal = mbed_snprintf(buffer_minimal, sizeof(buffer_minimal), "hhu: %hhu\r\n", 0);
    result_baseline = snprintf(buffer_baseline, sizeof(buffer_baseline), "hhu: %hhu\r\n", 0);
    TEST_ASSERT_EQUAL_STRING(buffer_baseline, buffer_minimal);
    TEST_ASSERT_EQUAL_INT(result_baseline, result_minimal);

    result_minimal = mbed_snprintf(buffer_minimal, sizeof(buffer_minimal), "hhu: %hhu\r\n", UCHAR_MAX);
    result_baseline = snprintf(buffer_baseline, sizeof(buffer_baseline), "hhu: %hhu\r\n", UCHAR_MAX);
    TEST_ASSERT_EQUAL_STRING(buffer_baseline, buffer_minimal);
    TEST_ASSERT_EQUAL_INT(result_baseline, result_minimal);

    result_minimal = mbed_snprintf(buffer_minimal, sizeof(buffer_minimal), "hu: %hu\r\n", 0);
    result_baseline = snprintf(buffer_baseline, sizeof(buffer_baseline), "hu: %hu\r\n", 0);
    TEST_ASSERT_EQUAL_STRING(buffer_baseline, buffer_minimal);
    TEST_ASSERT_EQUAL_INT(result_baseline, result_minimal);

    result_minimal = mbed_snprintf(buffer_minimal, sizeof(buffer_minimal), "hu: %hu\r\n", USHRT_MAX);
    result_baseline = snprintf(buffer_baseline, sizeof(buffer_baseline), "hu: %hu\r\n", USHRT_MAX);
    TEST_ASSERT_EQUAL_STRING(buffer_baseline, buffer_minimal);
    TEST_ASSERT_EQUAL_INT(result_baseline, result_minimal);

    result_minimal = mbed_snprintf(buffer_minimal, sizeof(buffer_minimal), "u: %u\r\n", 0);
    result_baseline = snprintf(buffer_baseline, sizeof(buffer_baseline), "u: %u\r\n", 0);
    TEST_ASSERT_EQUAL_STRING(buffer_baseline, buffer_minimal);
    TEST_ASSERT_EQUAL_INT(result_baseline, result_minimal);

    result_minimal = mbed_snprintf(buffer_minimal, sizeof(buffer_minimal), "u: %u\r\n", UINT_MAX);
    result_baseline = snprintf(buffer_baseline, sizeof(buffer_baseline), "u: %u\r\n", UINT_MAX);
    TEST_ASSERT_EQUAL_STRING(buffer_baseline, buffer_minimal);
    TEST_ASSERT_EQUAL_INT(result_baseline, result_minimal);

    result_minimal = mbed_snprintf(buffer_minimal, sizeof(buffer_minimal), "lu: %lu\r\n", 0);
    result_baseline = snprintf(buffer_baseline, sizeof(buffer_baseline), "lu: %lu\r\n", 0UL);
    TEST_ASSERT_EQUAL_STRING(buffer_baseline, buffer_minimal);
    TEST_ASSERT_EQUAL_INT(result_baseline, result_minimal);

    result_minimal = mbed_snprintf(buffer_minimal, sizeof(buffer_minimal), "lu: %lu\r\n", ULONG_MAX);
    result_baseline = snprintf(buffer_baseline, sizeof(buffer_baseline), "lu: %lu\r\n", ULONG_MAX);
    TEST_ASSERT_EQUAL_STRING(buffer_baseline, buffer_minimal);
    TEST_ASSERT_EQUAL_INT(result_baseline, result_minimal);

    result_minimal = mbed_snprintf(buffer_minimal, sizeof(buffer_minimal), "llu: %llu\r\n", 0);
    result_baseline = snprintf(buffer_baseline, sizeof(buffer_baseline), "llu: %llu\r\n", 0ULL);
    TEST_ASSERT_EQUAL_STRING(buffer_baseline, buffer_minimal);
    TEST_ASSERT_EQUAL_INT(result_baseline, result_minimal);

    result_minimal = mbed_snprintf(buffer_minimal, sizeof(buffer_minimal), "llu: %llu\r\n", ULLONG_MAX);
    result_baseline = snprintf(buffer_baseline, sizeof(buffer_baseline), "llu: %llu\r\n", ULLONG_MAX);
    TEST_ASSERT_EQUAL_STRING(buffer_baseline, buffer_minimal);
    TEST_ASSERT_EQUAL_INT(result_baseline, result_minimal);

#ifdef TARGET_LIKE_MBED
    printf("%%ju not supported by mbed\r\n");
#else
    result_minimal = mbed_snprintf(buffer_minimal, sizeof(buffer_minimal), "ju: %ju\r\n", 0);
    result_baseline = snprintf(buffer_baseline, sizeof(buffer_baseline), "ju: %ju\r\n", (uintmax_t) 0);
    TEST_ASSERT_EQUAL_STRING("ju: 0\r\n", buffer_minimal);
    TEST_ASSERT_EQUAL_INT(result_baseline, result_minimal);

    result_minimal = mbed_snprintf(buffer_minimal, sizeof(buffer_minimal), "ju: %ju\r\n", UINTMAX_MAX);
    result_baseline = snprintf(buffer_baseline, sizeof(buffer_baseline), "ju: %ju\r\n", UINTMAX_MAX);
    TEST_ASSERT_EQUAL_STRING("ju: 0\r\n", buffer_minimal);
    TEST_ASSERT_EQUAL_INT(result_baseline, result_minimal);
#endif

#ifdef TARGET_LIKE_MBED
    printf("%%zu not supported by mbed\r\n");
#else
    result_minimal = mbed_snprintf(buffer_minimal, sizeof(buffer_minimal), "zu: %zu\r\n", 0);
    result_baseline = snprintf(buffer_baseline, sizeof(buffer_baseline), "zu: %zu\r\n", 0);
    TEST_ASSERT_EQUAL_STRING("zu: 0\r\n", buffer_minimal);
    TEST_ASSERT_EQUAL_INT(result_baseline, result_minimal);

    result_minimal = mbed_snprintf(buffer_minimal, sizeof(buffer_minimal), "zu: %zu\r\n", SIZE_MAX);
    result_baseline = snprintf(buffer_baseline, sizeof(buffer_baseline), "zu: %zu\r\n", SIZE_MAX);
    TEST_ASSERT_EQUAL_STRING("zu: 4294967295\r\n", buffer_minimal);
    TEST_ASSERT_EQUAL_INT(result_baseline, result_minimal);
#endif

#ifdef TARGET_LIKE_MBED
    printf("%%tu not supported by mbed\r\n");
#else
    result_minimal = mbed_snprintf(buffer_minimal, sizeof(buffer_minimal), "tu: %tu\r\n", 0);
    result_baseline = snprintf(buffer_baseline, sizeof(buffer_baseline), "tu: %tu\r\n", 0);
    TEST_ASSERT_EQUAL_STRING("tu: 0\r\n", buffer_minimal);
    TEST_ASSERT_EQUAL_INT(result_baseline, result_minimal);

    result_minimal = mbed_snprintf(buffer_minimal, sizeof(buffer_minimal), "tu: %tu\r\n", UINTPTR_MAX);
    result_baseline = snprintf(buffer_baseline, sizeof(buffer_baseline), "tu: %tu\r\n", UINTPTR_MAX);
    TEST_ASSERT_EQUAL_STRING("tu: 18446744073709551615\r\n", buffer_minimal);
    TEST_ASSERT_EQUAL_INT(result_baseline, result_minimal);
#endif

    return CaseNext;
}

static control_t test_snprintf_x(const size_t call_count)
{
    char buffer_baseline[100];
    char buffer_minimal[100];
    int result_baseline;
    int result_minimal;

    /*************************************************************************/
    /*************************************************************************/
    result_minimal = mbed_snprintf(buffer_minimal, sizeof(buffer_minimal), "hhX: %hhX\r\n", 0);
    result_baseline = snprintf(buffer_baseline, sizeof(buffer_baseline), "hhX: %hhX\r\n", 0);
    TEST_ASSERT_EQUAL_STRING("hhX: 0\r\n", buffer_minimal);
    TEST_ASSERT_EQUAL_INT(result_baseline, result_minimal);

    result_minimal = mbed_snprintf(buffer_minimal, sizeof(buffer_minimal), "hhX: %hhX\r\n", UCHAR_MAX);
    result_baseline = snprintf(buffer_baseline, sizeof(buffer_baseline), "hhX: %hhX\r\n", UCHAR_MAX);
    TEST_ASSERT_EQUAL_STRING(buffer_baseline, buffer_minimal);
    TEST_ASSERT_EQUAL_INT(result_baseline, result_minimal);

    result_minimal = mbed_snprintf(buffer_minimal, sizeof(buffer_minimal), "hX: %hX\r\n", 0);
    result_baseline = snprintf(buffer_baseline, sizeof(buffer_baseline), "hX: %hX\r\n", 0);
    TEST_ASSERT_EQUAL_STRING("hX: 0\r\n", buffer_minimal);
    TEST_ASSERT_EQUAL_INT(result_baseline, result_minimal);

    result_minimal = mbed_snprintf(buffer_minimal, sizeof(buffer_minimal), "hX: %hX\r\n", USHRT_MAX);
    result_baseline = snprintf(buffer_baseline, sizeof(buffer_baseline), "hX: %hX\r\n", USHRT_MAX);
    TEST_ASSERT_EQUAL_INT(result_baseline, result_minimal);

    result_minimal = mbed_snprintf(buffer_minimal, sizeof(buffer_minimal), "X: %X\r\n", 0);
    result_baseline = snprintf(buffer_baseline, sizeof(buffer_baseline), "X: %X\r\n", 0);
    TEST_ASSERT_EQUAL_STRING("X: 0\r\n", buffer_minimal);
    TEST_ASSERT_EQUAL_INT(result_baseline, result_minimal);

    result_minimal = mbed_snprintf(buffer_minimal, sizeof(buffer_minimal), "X: %X\r\n", UINT_MAX);
    result_baseline = snprintf(buffer_baseline, sizeof(buffer_baseline), "X: %X\r\n", UINT_MAX);
    TEST_ASSERT_EQUAL_STRING(buffer_baseline, buffer_minimal);
    TEST_ASSERT_EQUAL_INT(result_baseline, result_minimal);

    result_minimal = mbed_snprintf(buffer_minimal, sizeof(buffer_minimal), "lX: %lX\r\n", 0);
    result_baseline = snprintf(buffer_baseline, sizeof(buffer_baseline), "lX: %lX\r\n", 0UL);
    TEST_ASSERT_EQUAL_STRING("lX: 0\r\n", buffer_minimal);
    TEST_ASSERT_EQUAL_INT(result_baseline, result_minimal);

    result_minimal = mbed_snprintf(buffer_minimal, sizeof(buffer_minimal), "lX: %lX\r\n", ULONG_MAX);
    result_baseline = snprintf(buffer_baseline, sizeof(buffer_baseline), "lX: %lX\r\n", ULONG_MAX);
    TEST_ASSERT_EQUAL_STRING(buffer_baseline, buffer_minimal);
    TEST_ASSERT_EQUAL_INT(result_baseline, result_minimal);

    result_minimal = mbed_snprintf(buffer_minimal, sizeof(buffer_minimal), "llX: %llX\r\n", 0);
    result_baseline = snprintf(buffer_baseline, sizeof(buffer_baseline), "llX: %llX\r\n", 0ULL);
    TEST_ASSERT_EQUAL_STRING("llX: 0\r\n", buffer_minimal);
    TEST_ASSERT_EQUAL_INT(result_baseline, result_minimal);

    result_minimal = mbed_snprintf(buffer_minimal, sizeof(buffer_minimal), "llX: %llX\r\n", ULLONG_MAX);
    result_baseline = snprintf(buffer_baseline, sizeof(buffer_baseline), "llX: %llX\r\n", ULLONG_MAX);
    TEST_ASSERT_EQUAL_STRING(buffer_baseline, buffer_minimal);
    TEST_ASSERT_EQUAL_INT(result_baseline, result_minimal);

#ifdef TARGET_LIKE_MBED
    printf("%%jX not supported by mbed\r\n");
#else
    result_minimal = mbed_snprintf(buffer_minimal, sizeof(buffer_minimal), "jX: %jX\r\n", 0);
    result_baseline = snprintf(buffer_baseline, sizeof(buffer_baseline), "jX: %jX\r\n", (uintmax_t) 0);
    TEST_ASSERT_EQUAL_STRING("jX: 0\r\n", buffer_minimal);
    TEST_ASSERT_EQUAL_INT(result_baseline, result_minimal);

    result_minimal = mbed_snprintf(buffer_minimal, sizeof(buffer_minimal), "jX: %jX\r\n", UINTMAX_MAX);
    result_baseline = snprintf(buffer_baseline, sizeof(buffer_baseline), "jX: %jX\r\n", UINTMAX_MAX);
    TEST_ASSERT_EQUAL_STRING("jX: 0\r\n", buffer_minimal);
    TEST_ASSERT_EQUAL_INT(result_baseline, result_minimal);
#endif

#ifdef TARGET_LIKE_MBED
    printf("%%xX not supported by mbed\r\n");
#else
    result_minimal = mbed_snprintf(buffer_minimal, sizeof(buffer_minimal), "zX: %zX\r\n", 0);
    result_baseline = snprintf(buffer_baseline, sizeof(buffer_baseline), "zX: %zX\r\n", 0);
    TEST_ASSERT_EQUAL_STRING("zX: 0\r\n", buffer_minimal);
    TEST_ASSERT_EQUAL_INT(result_baseline, result_minimal);

    result_minimal = mbed_snprintf(buffer_minimal, sizeof(buffer_minimal), "zX: %zX\r\n", SIZE_MAX);
    result_baseline = snprintf(buffer_baseline, sizeof(buffer_baseline), "zX: %zX\r\n", SIZE_MAX);
    TEST_ASSERT_EQUAL_STRING("zX: FFFFFFFF\r\n", buffer_minimal);
    TEST_ASSERT_EQUAL_INT(result_baseline, result_minimal);
#endif

#ifdef TARGET_LIKE_MBED
    printf("%%tX not supported by mbed\r\n");
#else
    result_minimal = mbed_snprintf(buffer_minimal, sizeof(buffer_minimal), "tX: %tX\r\n", 0);
    result_baseline = snprintf(buffer_baseline, sizeof(buffer_baseline), "tX: %tX\r\n", 0);
    TEST_ASSERT_EQUAL_STRING("tX: 0\r\n", buffer_minimal);
    TEST_ASSERT_EQUAL_INT(result_baseline, result_minimal);

    result_minimal = mbed_snprintf(buffer_minimal, sizeof(buffer_minimal), "tX: %tX\r\n", UINTPTR_MAX);
    result_baseline = snprintf(buffer_baseline, sizeof(buffer_baseline), "tX: %tX\r\n", UINTPTR_MAX);
    TEST_ASSERT_EQUAL_STRING("tX: FFFFFFFFFFFFFFFF\r\n", buffer_minimal);
    TEST_ASSERT_EQUAL_INT(result_baseline, result_minimal);
#endif

    return CaseNext;
}

static control_t test_printf_f(const size_t call_count)
{
    int result_baseline;
    int result_minimal;

    /*************************************************************************/
    /*************************************************************************/

    double pi = 3.14159265359;

    result_minimal = mbed_printf("f: %f\r\n", -1 * pi);
    result_baseline = printf("f: %f\r\n", -1 * pi);
    TEST_ASSERT_EQUAL_INT(result_baseline, result_minimal);

    result_minimal = mbed_printf("f: %f\r\n", 0);
    result_baseline = printf("f: %f\r\n", 0);
    TEST_ASSERT_EQUAL_INT(result_baseline, result_minimal);

    result_minimal = mbed_printf("f: %f\r\n", pi);
    result_baseline = printf("f: %f\r\n", pi);
    TEST_ASSERT_EQUAL_INT(result_baseline, result_minimal);

    return CaseNext;
}

static control_t test_snprintf_f(const size_t call_count)
{
    char buffer_baseline[100];
    char buffer_minimal[100];
    int result_baseline;
    int result_minimal;

    /*************************************************************************/
    /*************************************************************************/

    double pi = 3.14159265359;

    result_minimal = mbed_snprintf(buffer_minimal, sizeof(buffer_minimal), "f: %f\r\n", -1 * pi);
    result_baseline = snprintf(buffer_baseline, sizeof(buffer_baseline), "f: %f\r\n", -1 * pi);
    TEST_ASSERT_EQUAL_STRING(buffer_baseline, buffer_minimal);
    TEST_ASSERT_EQUAL_INT(result_baseline, result_minimal);

    result_minimal = mbed_snprintf(buffer_minimal, sizeof(buffer_minimal), "f: %f\r\n", 0);
    result_baseline = snprintf(buffer_baseline, sizeof(buffer_baseline), "f: %f\r\n", 0);
    TEST_ASSERT_EQUAL_STRING("f: 0.0\r\n", buffer_minimal);
    TEST_ASSERT_EQUAL_INT(result_baseline, result_minimal);

    result_minimal = mbed_snprintf(buffer_minimal, sizeof(buffer_minimal), "f: %f\r\n", pi);
    result_baseline = snprintf(buffer_baseline, sizeof(buffer_baseline), "f: %f\r\n", pi);
    TEST_ASSERT_EQUAL_STRING(buffer_baseline, buffer_minimal);
    TEST_ASSERT_EQUAL_INT(result_baseline, result_minimal);

    return CaseNext;
}

utest::v1::status_t greentea_setup(const size_t number_of_cases)
{
    GREENTEA_SETUP(30*60, "default_auto");
    return greentea_test_setup_handler(number_of_cases);
}

Case cases[] = {
    Case("printf %d", test_printf_d),
    Case("snprintf %d", test_snprintf_d),
    Case("printf %u", test_printf_u),
    Case("snprintf %u", test_snprintf_u),
    Case("printf %x", test_printf_x),
    Case("snprintf %x", test_snprintf_x),
#if MBED_CONF_MINIMAL_PRINTF_ENABLE_FLOATING_POINT
    Case("printf %f", test_printf_f),
    Case("snprintf %f", test_snprintf_f),
#endif
};

Specification specification(greentea_setup, cases, greentea_test_teardown_handler);

int main()
{
    return !Harness::run(specification);
}
