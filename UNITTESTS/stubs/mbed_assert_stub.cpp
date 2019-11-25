/*
 * Copyright (c) , Arm Limited and affiliates.
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

#include "platform/mbed_assert.h"
#include <stdio.h>
#include <stdbool.h>

bool mbed_assert_throw_errors = false;

extern "C" void mbed_assert_internal(const char *expr, const char *file, int line)
{
    fprintf(stderr, "mbed assertation failed: %s, file: %s, line %d \n", expr, file, line);
    if (mbed_assert_throw_errors) {
        throw 1;
    }
}
