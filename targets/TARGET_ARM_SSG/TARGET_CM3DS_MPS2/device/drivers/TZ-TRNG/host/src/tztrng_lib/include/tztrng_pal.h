/******************************************************************************
* Copyright (c) 2017-2017, ARM, All Rights Reserved                           *
* SPDX-License-Identifier: Apache-2.0                                         *
*                                                                             *
* Licensed under the Apache License, Version 2.0 (the "License");             *
* you may not use this file except in compliance with the License.            *
*                                                                             *
* You may obtain a copy of the License at                                     *
* http://www.apache.org/licenses/LICENSE-2.0                                  *
*                                                                             *
* Unless required by applicable law or agreed to in writing, software         *
* distributed under the License is distributed on an "AS IS" BASIS, WITHOUT   *
* WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.            *
*                                                                             *
* See the License for the specific language governing permissions and         *
* limitations under the License.                                              *
******************************************************************************/
#ifndef _TZTRNG_PAL_H_
#define _TZTRNG_PAL_H_

#include <stdint.h>
uint32_t CC_HalWaitInterrupt(void);
void tztrng_memcpy(uint8_t *dst, uint8_t *src, size_t size);
void tztrng_memset(uint8_t *dst, uint8_t value, size_t size);

#endif
