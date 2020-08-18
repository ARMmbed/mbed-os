/*
 * Copyright (c) 2015 ARM Limited. All rights reserved.
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
#ifndef ERROR_CALLBACK_H
#define ERROR_CALLBACK_H
#ifdef __cplusplus
extern "C" {
#endif

#include "nsdynmemLIB.h"
#include <stdint.h>

extern heap_fail_t current_heap_error;

void heap_fail_callback(heap_fail_t err);
void reset_heap_error();
bool heap_have_failed();

#ifdef __cplusplus
}
#endif
#endif /*ERROR_CALLBACK_H*/
