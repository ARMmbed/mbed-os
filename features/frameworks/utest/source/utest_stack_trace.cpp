/****************************************************************************
 * Copyright (c) 2016, ARM Limited, All Rights Reserved
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
#ifdef UTEST_STACK_TRACE
 
#include "greentea-client/test_env.h"
#include "mbed.h"
#include "utest/utest.h"
#include "unity/unity.h"
#include "utest/utest_stack_trace.h"
#include "utest/utest_serial.h"

using namespace utest::v1;

std::string utest_trace[UTEST_MAX_BACKTRACE];

static unsigned trace_index = 0;
static unsigned total_calls = 0;

void utest_trace_initialise()
{
    total_calls = 0;
    trace_index = 0;
    for(unsigned i = 0; i < UTEST_MAX_BACKTRACE; i++) {
        utest_trace[i].clear();        
    }
}

void utest_add_to_trace(char *func_name)
{    
    utest_trace[trace_index] = std::string(func_name);   
    trace_index = (trace_index + 1 == UTEST_MAX_BACKTRACE) ? 0 : trace_index + 1;
    total_calls ++;
}

void utest_dump_trace()
{
    unsigned current = (trace_index == 0) ? UTEST_MAX_BACKTRACE - 1 : trace_index - 1;

    utest_printf("==================================================================\n");
    utest_printf("Utest back trace: Total calls logged = %u.\n", total_calls);
    utest_printf("==================================================================\n");
    while (current != trace_index) {
    
        utest_printf("%u > %s\n", current, utest_trace[current].c_str());
        current = (current == 0) ? UTEST_MAX_BACKTRACE - 1 : current - 1;
    }
    utest_printf("==================================================================\n");
}

#endif
