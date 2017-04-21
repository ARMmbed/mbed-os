
/** \addtogroup frameworks */
/** @{*/
/****************************************************************************
 * Copyright (c) 2016, ARM Limited, All Rights Reserved
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

#ifndef UTEST_STACK_TRACE_H
#define UTEST_STACK_TRACE_H

#ifdef UTEST_STACK_TRACE
#include <string>

#define UTEST_MAX_BACKTRACE 100
extern std::string utest_trace[UTEST_MAX_BACKTRACE];

extern void utest_trace_initialise();
extern void utest_add_to_trace(char *func_name);
extern void utest_dump_trace();

#define UTEST_LOG_FUNCTION(); utest_add_to_trace((char *)__func__);
#define UTEST_TRACE_START utest_trace_initialise();
#define UTEST_DUMP_TRACE utest_dump_trace();

#else

#define UTEST_LOG_FUNCTION(); 
#define UTEST_TRACE_START
#define UTEST_DUMP_TRACE

#endif // UTEST_STACK_TRACE

#endif // UTEST_STACK_TRACE_H

/** @}*/
