/*
 * Copyright (c) 2018 ARM Limited. All rights reserved.
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


#include "mbed-trace/mbed_trace.h"
#define TRACE_GROUP "main"


int main(void)
{
    mbed_trace_init();
    tr_debug("debug print");
    tr_info("info print");
    tr_warn("warning print");
    tr_error("error print");
    mbed_trace_free();
    return 0;
}
