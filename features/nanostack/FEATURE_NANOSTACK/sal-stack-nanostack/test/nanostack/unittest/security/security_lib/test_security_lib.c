/*
 * Copyright (c) 2016-2017, Arm Limited and affiliates.
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
#include "nsconfig.h"
#include "test_security_lib.h"
#include <string.h>
#include <inttypes.h>
#include "Core/include/ns_buffer.h"
#include "Security/Common/sec_lib.h"

bool test_sec_check_suite_ptrs()
{
    sec_suite_t suite;
    memset(&suite, 0,sizeof(sec_suite_t));
    if( 0 != sec_check_suite_ptrs(&suite) ) {
        return false;
    }
    return true;
}

