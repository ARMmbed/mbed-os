/*
 * Copyright 2020 Arduino SA
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

/** @file
 *  Provides wiced fs porting to generic mbed APIs
 */

#pragma once

#include "whd_config.h"
#include "BlockDevice.h"

#ifdef __cplusplus
extern "C" {
#endif

/******************************************************
 *                 Type Definitions
 ******************************************************/

/**
 * Mount the physical device
 *
 * This assumes that the device is ready to read/write immediately.
 *
 * @param[in]  device        - physical media to init
 * @param[out] fs_handle_out - Receives the filesystem handle.
 *
 * @return WICED_SUCCESS on success
 */
wiced_result_t wiced_filesystem_mount(mbed::BlockDevice *device, wiced_filesystem_handle_type_t fs_type, wiced_filesystem_t *fs_handle_out, const char *mounted_name);

#ifdef __cplusplus
} /*extern "C" */
#endif
