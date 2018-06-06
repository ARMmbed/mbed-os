/*
  *  cc_platform.h
  *
  *  Copyright (C) 2018, Arm Limited, All Rights Reserved
  *  SPDX-License-Identifier: Apache-2.0
  *
  *  Licensed under the Apache License, Version 2.0 (the "License"); you may
  *  not use this file except in compliance with the License.
  *  You may obtain a copy of the License at
  *
  *  http://www.apache.org/licenses/LICENSE-2.0
  *
  *  Unless required by applicable law or agreed to in writing, software
  *  distributed under the License is distributed on an "AS IS" BASIS, WITHOUT
  *  WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
  *  See the License for the specific language governing permissions and
  *  limitations under the License.
  *
  */
#ifndef __CC_PLATFORM_H_
#define __CC_PLATFORM_H_
/**
 * \brief   The CC platform context structure.
 *
 * \note    This structure may be used to assist platform-specific
 *          setup or teardown operations.
 */
typedef struct {
    char dummy; /**< Placeholder member, as empty structs are not portable. */
}
cc_platform_ctx;

#endif /* __CC_PLATFORM_H_ */
