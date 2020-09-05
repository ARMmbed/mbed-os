/*
 * Copyright (c) 2014-2015 ARM Limited. All rights reserved.
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
#ifndef NS_TIMER_H_
#define NS_TIMER_H_

#include "platform/eventloop_config.h"

#ifdef __cplusplus
extern "C" {
#endif

#ifndef NS_EXCLUDE_HIGHRES_TIMER
extern int8_t ns_timer_sleep(void);
#else
#define ns_timer_sleep() ((int8_t) 0)
#endif

#ifdef __cplusplus
}
#endif

#endif /*NS_TIMER_H_*/
