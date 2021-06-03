/*
 * Copyright 2021, Cypress Semiconductor Corporation (an Infineon company)
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
 *  Provides prototypes / declarations for common APSTA functionality
 */
#ifndef _WHD_INTERNAL_RESOURCE_API_H_
#define _WHD_INTERNAL_RESOURCE_API_H_

#include "whd.h"
#include "whd_int.h"
#include "whd_resource_api.h"

#ifdef __cplusplus
extern "C"
{
#endif

/******************************************************
*                      Macros
******************************************************/

/******************************************************
*             Structures
******************************************************/

/******************************************************
*             Function prototypes
******************************************************/
uint32_t whd_resource_size(whd_driver_t whd_driver, whd_resource_type_t resource, uint32_t *size_out);
uint32_t whd_get_resource_block_size(whd_driver_t whd_drv, whd_resource_type_t type, uint32_t *size_out);
uint32_t whd_get_resource_no_of_blocks(whd_driver_t whd_drv, whd_resource_type_t type, uint32_t *block_count);
uint32_t whd_get_resource_block(whd_driver_t whd_driver, whd_resource_type_t type,
                                uint32_t blockno, const uint8_t **data, uint32_t *size_out);

#ifdef __cplusplus
} /*extern "C" */
#endif

#endif /* ifndef _WHD_INTERNAL_RESOURCE_API_H_ */

