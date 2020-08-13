/****************************************************************************
 * Copyright (c) 2019, ARM Limited, All Rights Reserved
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
 ****************************************************************************
 */

#include <stdio.h>
#include <string.h>
#include <cstdarg>

#include "greentea-client/test_env.h"
#include "platform/mbed_retarget.h"


#define STRING_STACK_LIMIT    120

static int utest_vprintf(const char *format, std::va_list arg)
{
    // ARMCC microlib does not properly handle a size of 0.
    // As a workaround supply a dummy buffer with a size of 1.
    char dummy_buf[1];
    int len = vsnprintf(dummy_buf, sizeof(dummy_buf), format, arg);
    if (len < STRING_STACK_LIMIT) {
        char temp[STRING_STACK_LIMIT];
        vsprintf(temp, format, arg);
        greentea_write_string(temp);
    } else {
        char *temp = new char[len + 1];
        vsprintf(temp, format, arg);
        greentea_write_string(temp);
        delete[] temp;
    }

    return len;
}


extern "C" int utest_printf(const char *format, ...)
{
    std::va_list args;
    va_start(args, format);
    int len = utest_vprintf(format, args);
    va_end(args);
    return len;
}
