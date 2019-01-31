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

#ifdef __cplusplus
} // closing brace for extern "C"
#endif
#endif
