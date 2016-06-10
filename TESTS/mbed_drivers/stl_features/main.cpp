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
#include "mbed.h"
#include "greentea-client/test_env.h"
#include "unity/unity.h"
#include "utest/utest.h"
#include <stdio.h>
#include <string.h>
#include <algorithm>
#include <string>
#include <vector>
#include <queue>
#include <map>
#include <math.h>

using namespace utest::v1;

#define TABLE_SIZE(TAB) (sizeof(TAB) / sizeof(TAB[0]))

#define NEGATIVE_INTEGERS -32768,-3214,-999,-100,-1,0,1,4231,999,4123,32760,99999
#define POSITIVE_INTEGERS 32768,3214,999,100,1,0,1,4231,999,4123,32760,99999
#define FLOATS  0.002,0.92430,15.91320,791.77368,6208.2,25719.4952,426815.982588,6429271.046,42468024.93,212006462.910
#define FLOATS_STR  "0.002","0.92430","15.91320","791.77368","6208.2","25719.4952","426815.982588","6429271.046","42468024.93","212006462.910"


namespace {
int p_integers[] = {POSITIVE_INTEGERS};
int n_integers[] = {NEGATIVE_INTEGERS};
float floats[] = {FLOATS};

template <class T, class F>
void BubbleSort(T& _array, size_t array_size, F functor) {
    bool flag = true;
    size_t numLength = array_size;
    for(size_t i = 1; (i <= numLength) && flag; i++) {
        flag = false;
        for (size_t j = 0; j < (numLength - 1); j++) {
            if (functor(_array[j+1], _array[j])) {
                int temp = _array[j];
                _array[j] = _array[j + 1];
                _array[j+1] = temp;
                flag = true;
            }
        }
    }
}

struct printInt {
    void operator()(int i) { printf("%d ", i); }
};

struct printFloat {
    void operator()(float f) { printf("%f ", f); }
};

struct printString {
    void operator()(const char* s) { printf("%s ", s); }
};

struct greaterAbs {
    bool operator()(int a, int b) { return abs(a) > abs(b); }
};

}   // namespace

void test_case_stl_equal() {
    std::vector<int> v_pints(p_integers, p_integers + TABLE_SIZE(p_integers));
    TEST_ASSERT_TRUE(std::equal(v_pints.begin(), v_pints.end(), p_integers));
}

void test_case_stl_transform() {
    const char* floats_str[] = {FLOATS_STR};
    float floats_transform[TABLE_SIZE(floats_str)] = {0.0};
    std::transform(floats_str, floats_str + TABLE_SIZE(floats_str), floats_transform, atof);
    //printf("stl_transform::floats_str: ");
    //std::for_each(floats_str, floats_str + TABLE_SIZE(floats_str), printString());
    //printf("stl_transform::floats_transform: ");
    //std::for_each(floats_transform, floats_transform + TABLE_SIZE(floats_transform), printFloat());
    //printf("\n");

    TEST_ASSERT_TRUE(std::equal(floats_transform, floats_transform + TABLE_SIZE(floats_transform), floats));
}

void test_case_stl_sort_greater() {
    std::vector<int> v_nints_1(n_integers, n_integers + TABLE_SIZE(n_integers));
    std::vector<int> v_nints_2(n_integers, n_integers + TABLE_SIZE(n_integers));

    BubbleSort(v_nints_1, v_nints_1.size(), std::greater<int>());
    std::sort(v_nints_2.begin(), v_nints_2.end(), std::greater<int>());

    TEST_ASSERT_TRUE(std::equal(v_nints_1.begin(), v_nints_1.end(), v_nints_2.begin()));
}

void test_case_stl_sort_abs() {
    std::vector<int> v_nints_1(n_integers, n_integers + TABLE_SIZE(n_integers));
    std::vector<int> v_nints_2(n_integers, n_integers + TABLE_SIZE(n_integers));

    BubbleSort(v_nints_1, v_nints_1.size(), greaterAbs());
    std::sort(v_nints_2.begin(), v_nints_2.end(), greaterAbs());

    TEST_ASSERT_TRUE(std::equal(v_nints_1.begin(), v_nints_1.end(), v_nints_2.begin()));
}

utest::v1::status_t greentea_failure_handler(const Case *const source, const failure_t reason) {
    greentea_case_failure_abort_handler(source, reason);
    return STATUS_CONTINUE;
}

Case cases[] = {
    Case("STL std::equal", test_case_stl_equal, greentea_failure_handler),
    Case("STL std::transform", test_case_stl_transform, greentea_failure_handler),
    Case("STL std::sort greater", test_case_stl_sort_greater, greentea_failure_handler),
    Case("STL std::sort abs", test_case_stl_sort_abs, greentea_failure_handler)
};

utest::v1::status_t greentea_test_setup(const size_t number_of_cases) {
    GREENTEA_SETUP(5, "default_auto");
    return greentea_test_setup_handler(number_of_cases);
}

Specification specification(greentea_test_setup, cases, greentea_test_teardown_handler);

int main() {
    Harness::run(specification);
}
