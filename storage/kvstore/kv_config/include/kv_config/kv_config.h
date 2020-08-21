/*
 * Copyright (c) 2018 ARM Limited. All rights reserved.
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
#ifndef _KV_CONFIG
#define _KV_CONFIG

#include "blockdevice/BlockDevice.h"

#ifdef __cplusplus
extern "C" {
#endif

#ifndef MBED_CONF_STORAGE_STORAGE
#define MBED_CONF_STORAGE_STORAGE USER_DEFINED
#endif

#define _STORAGE_CONFIG_concat(dev) _storage_config_##dev()
#define _STORAGE_CONFIG(dev) _STORAGE_CONFIG_concat(dev)

/**
 * @brief This function initializes one of the configuration that exists in Mbed OS. To overwrite
 *        the default configuration, please overwrite this function.
 *
 * @returns 0 on success or negative value on failure.
 */
int kv_init_storage_config();

/**
 * @brief A getter for filesystemstore folder path configuration
 *
 * @returns string with the file folder path or NULL if not set
 */
const char *get_filesystemstore_folder_path();

/**
 * @brief Get the default TDBStore flash start address and size.
 *
 * @param[out] start_address    Default TDBStore start address in flash.
 * @param[out] size             Default TDBStore size.
 *
 * @returns MBED_SUCCESS                        Success.
 *          MBED_ERROR_INITIALIZATION_FAILED    Failed to initialize flash driver.
 *          MBED_ERROR_MEDIA_FULL               Default TDBStore space overlaps with program memory.
 */
int kv_get_default_flash_addresses(mbed::bd_addr_t *start_address, mbed::bd_size_t *size);

/**
 * @brief Get the TDBStore flash bounds from the configured start address and size.
 *
 * @param[inout] start_address    Configured TDBStore start address in flash.
 * @param[inout] size             Configured TDBStore size. If 0, the size will be from the start address to the end of flash
 *
 * @returns MBED_SUCCESS                        Success.
 *          MBED_ERROR_INVALID_ARGUMENT         One of the arguments is NULL or both the configured start address and size are 0.
 *          MBED_ERROR_INITIALIZATION_FAILED    Failed to initialize flash driver.
 *          MBED_ERROR_INVALID_SIZE             Configured size results in misaligned start/end address or end address past the end of flash.
 *          MBED_ERROR_MEDIA_FULL               Configured start address/size results in bounds overlapping with program memory.
 */
int kv_get_flash_bounds_from_config(mbed::bd_addr_t *start_address, mbed::bd_size_t *size);

#ifdef __cplusplus
} // closing brace for extern "C"
#endif
#endif
