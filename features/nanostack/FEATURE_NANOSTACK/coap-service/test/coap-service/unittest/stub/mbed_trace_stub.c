/*
 * Copyright (c) 2014-2017 ARM Limited. All rights reserved.
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
#include <stdio.h>
#include <string.h>
#include <stdarg.h>
#include <stdlib.h>

#ifndef YOTTA_CFG_MBED_TRACE
#define YOTTA_CFG_MBED_TRACE 1
#define YOTTA_CFG_MBED_TRACE_FEA_IPV6 1
#endif

#include "mbed-trace/mbed_trace.h"
#if YOTTA_CFG_MBED_TRACE_FEA_IPV6 == 1
#include "mbed-client-libservice/ip6string.h"
#include "mbed-client-libservice/common_functions.h"
#endif


int mbed_trace_init(void)
{
    return 0;
}
void mbed_trace_free(void)
{
}

void mbed_trace_buffer_sizes(int lineLength, int tmpLength)
{
}

void mbed_trace_config_set(uint8_t config)
{
}

uint8_t mbed_trace_config_get(void)
{
    return 0;
}

void mbed_trace_prefix_function_set(char *(*pref_f)(size_t))
{
}

void mbed_trace_suffix_function_set(char *(*suffix_f)(void))
{
}

void mbed_trace_print_function_set(void (*printf)(const char *))
{
}

void mbed_trace_cmdprint_function_set(void (*printf)(const char *))
{
}

void mbed_trace_exclude_filters_set(char *filters)
{

}

const char *mbed_trace_exclude_filters_get(void)
{
    return NULL;
}

const char *mbed_trace_include_filters_get(void)
{
    return NULL;
}

void mbed_trace_include_filters_set(char *filters)
{

}

void mbed_tracef(uint8_t dlevel, const char *grp, const char *fmt, ...)
{
}

const char *mbed_trace_last(void)
{
    return NULL;
}

/* Helping functions */
#define tmp_data_left()  m_trace.tmp_data_length-(m_trace.tmp_data_ptr-m_trace.tmp_data)
#if YOTTA_CFG_MBED_TRACE_FEA_IPV6 == 1
char *mbed_trace_ipv6(const void *addr_ptr)
{
    return NULL;
}

char *mbed_trace_ipv6_prefix(const uint8_t *prefix, uint8_t prefix_len)
{
    return NULL;
}
#endif //YOTTA_CFG_MBED_TRACE_FEA_IPV6

char *mbed_trace_array(const uint8_t *buf, uint16_t len)
{
    return NULL;
}
