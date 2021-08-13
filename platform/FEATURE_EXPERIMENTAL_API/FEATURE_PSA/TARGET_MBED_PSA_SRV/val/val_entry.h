/** @file
 * Copyright (c) 2018-2019, Arm Limited or its affiliates. All rights reserved.
 * SPDX-License-Identifier : Apache-2.0
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *  http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
**/

#ifndef _VAL_ENTRY_H_
#define _VAL_ENTRY_H_

#include "val_framework.h"

#define PSA_ACS_MAJOR_VER    0
#define PSA_ACS_MINOR_VER    8

/**
    @brief    - PSA Test Suite C main function, does VAL init and calls test dispatcher
    @param    - None
    @return   - void
**/
extern void val_entry(void);
#endif
