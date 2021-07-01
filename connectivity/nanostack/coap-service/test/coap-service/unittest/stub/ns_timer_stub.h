/*
 * Copyright (c) 2015-2017, Pelion and affiliates.
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
#ifndef __NS_TIMER_STUB_H__
#define __NS_TIMER_STUB_H__

#ifdef __cplusplus
extern "C" {
#endif

typedef struct {
    int8_t int8_value;
    void (*cb)(int8_t, uint16_t);
} ns_timer_stub_def;

extern ns_timer_stub_def ns_timer_stub;

#ifdef __cplusplus
}
#endif

#endif // __NS_TIMER_STUB_H__
