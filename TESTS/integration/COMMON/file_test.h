/*
 * mbed Microcontroller Library
 * Copyright (c) 2006-2018 ARM Limited
 *
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

/*
 * Based on mbed-stress-test by Marcus Chang @ Arm Mbed - http://github.com/ARMmbed/mbed-stress-test
*/
#include "mbed_trace.h"

#define TRACE_GROUP "GRNT"

void file_test_write(const char *file, size_t offset, const unsigned char *data, size_t data_length, size_t block_size);

void file_test_read(const char *file, size_t offset, const unsigned char *data, size_t data_length, size_t block_size);
