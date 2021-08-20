/*
 * Copyright (c) 2021, ARM Limited, All Rights Reserved
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
#include <cstdint>
#include <cstring>
#include "platform/mbed_retarget.h"

extern "C" void greentea_write_string(const char *str)
{
    write(STDOUT_FILENO, str, strlen(str));
}

extern "C" int greentea_getc()
{
    uint8_t c;
    read(STDOUT_FILENO, &c, 1);
    return c;
}

extern "C" void greentea_putc(int c)
{
    uint8_t _c = c;
    write(STDOUT_FILENO, &_c, 1);
}
