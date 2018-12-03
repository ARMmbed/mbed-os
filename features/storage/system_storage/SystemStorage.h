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
#ifndef MBED_SYSTEM_STORAGE_H
#define MBED_SYSTEM_STORAGE_H

#include "mbed_error.h"

typedef enum {
    NONE        = 0,
    NVSTORE,
    TDBSTORE
} internal_mem_resident_type_e;

/**
 * @brief Try to get an ownership for the internal flash memory storage type.
 *        KVSTORE or NVSTORE is the current option and once the ownership is taken by one
 *        second one can not be initialize.
 * @param[in] in_mem_res       Enum parameter to specify NVSTORE or KVSTORE as the storage owner
 * @param[in] out_mem_res      Enum parameter which specify who is the current owner of the storage.
 * @returns                    MBED_SUCCESS if succeeded or MBED_ERROR_ALREADY_INITIALIZED if fails.
 */
int set_internal_storage_ownership(internal_mem_resident_type_e in_mem_res, internal_mem_resident_type_e *out_mem_res);

#endif
