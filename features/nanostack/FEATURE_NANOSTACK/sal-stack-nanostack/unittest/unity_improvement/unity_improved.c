/*
 * Copyright (c) 2014-2015, Arm Limited and affiliates.
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

/**
* \file \test_libTrace\Test.c
*
* \brief Unit tests for libTrace
*/
#include <stdarg.h>
#include <string.h>
#include <stdlib.h>
#include <stdint.h>
#include <inttypes.h>

#include "unity.h"
#include "ns_list.h"
#include "ns_trace.h"

void print_buf(const uint8_t *buf, uint16_t len)
{
    for (int n = 1; n <= len; n++) {
        printf(",%02xh", buf[n - 1]);
        if (n % 16 == 0) {
            printf("\n");
        }
    }
}
void test_buf(const uint8_t *buf, uint16_t len, const uint8_t *buf1, uint16_t len1)
{
    int c, n;
    c = len < len1 ? len : len1;

    for (n = 0; n < c; n++) {
        if (buf[n] == buf1[n]) {
            printf(",%02xh", buf[n]);
        } else {
            break;
        }
    }
    if (n < c) {
        // not equal outputted
        printf("\nRemaining expected:\n");
        print_buf(&buf[n], len - n);
        printf("\nRemaining received:\n");
        print_buf(&buf1[n], len1 - n);
    }
    printf("\n");
}

typedef struct {
    char *function_name_ptr;
    void *params_ptr;
    ns_list_link_t link;
} teststep_t;
typedef NS_LIST_HEAD(teststep_t, link) teststep_list_t;

teststep_list_t teststep_list;

void unity_impr_teststep_set(char *function_name_ptr, void *params_ptr)
{
    teststep_t *ts_ptr;
    printf("UNITY Called: %s\n", function_name_ptr);
    ts_ptr = malloc(sizeof(teststep_t));
    ns_list_add_to_start(&teststep_list, ts_ptr);
    ts_ptr->function_name_ptr = function_name_ptr;
    ts_ptr->params_ptr = params_ptr;
}

void *unity_impr_teststep_get(char *function_name_ptr)
{
    teststep_t *ts_ptr;
    void *ptr;
    printf("UNITY Checkking data for method: %s\n", function_name_ptr);
    ts_ptr = ns_list_get_last(&teststep_list);
    if (ts_ptr == NULL) {
        printf("UNITY Expecting function %s, but nothing has been called\n", function_name_ptr);
        goto failure;
    }
    if (strcmp(function_name_ptr, ts_ptr->function_name_ptr) != 0) {
        printf("UNITY Expecting function %s, but %s was called\n", function_name_ptr, ts_ptr->function_name_ptr);
        goto failure;
    }
    ptr = ts_ptr->params_ptr;
    ns_list_remove(&teststep_list, ts_ptr);
    free(ts_ptr);
    return ptr;
failure:
    TEST_FAIL();
    return NULL;
}
void unity_impr_teststep_clean(void)
{
    bool failure = false;
    teststep_t *ts_ptr;
    do {
        ts_ptr = ns_list_get_last(&teststep_list);
        if (ts_ptr != NULL) {
            printf("UNITY Unhandled method %s\n", ts_ptr->function_name_ptr);
            ns_list_remove(&teststep_list, ts_ptr);
            failure = true;
            free(ts_ptr);
        }
    } while (ts_ptr != NULL);
    if (failure) {
        TEST_FAIL();
    }
    return;
}
