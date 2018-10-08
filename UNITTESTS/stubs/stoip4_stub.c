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

#include <string.h>
#include <stdlib.h>
#include <stdint.h>
#include "common_functions.h"
#include "ip4string.h"

/**
 * Convert numeric IPv4 address string to a binary.
 * \param ip4addr IPv4 address in string format.
 * \param len Length of IPv4 string, maximum of 16..
 * \param dest buffer for address. MUST be 4 bytes.
 * \return boolean set to true if conversion succeded, false if it didn't
 */
bool stoip4(const char *ip4addr, size_t len, void *dest)
{
    return true;
}
