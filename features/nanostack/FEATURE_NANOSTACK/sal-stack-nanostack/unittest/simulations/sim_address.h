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

void lt_test_addr_delete(const uint8_t address[static 16]);

void lt_test_addr_add(const uint8_t address[static 16]);

/*
 call timeout callback for address.
*/
void lt_timeout_address(int8_t interface_id, const uint8_t address[static 16]);
