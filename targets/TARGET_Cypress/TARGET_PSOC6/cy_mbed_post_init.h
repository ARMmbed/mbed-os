/*
 * mbed Microcontroller Library
 * Copyright (c) 2019, Arm Limited and affiliates.
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

#ifndef CY_MBED_POST_INIT_H
#define CY_MBED_POST_INIT_H

#include "mbed_toolchain.h"

#ifdef __cplusplus
extern "C" {
#endif

/*******************************************************************************
* Function Name: cy_mbed_post_bsp_init_hook
****************************************************************************//**
*
* Function that is called immediately after cybsp_init finishes executing
* Applications can override this definition if they need to reserve resources
* early in the startup process so that later stages won't try and use them.
* For example, a timer instance might be reserved so that the us_ticker won't
* try to allocate it.
*
*******************************************************************************/
void cy_mbed_post_bsp_init_hook(void);

#ifdef __cplusplus
}
#endif

#endif /* CY_MBED_POST_INIT_H */
