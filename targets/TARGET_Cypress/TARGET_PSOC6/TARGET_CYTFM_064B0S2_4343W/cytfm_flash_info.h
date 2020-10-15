/***************************************************************************//**
* \file cytfm_flash_info.h
*
* Description:
* Provides Flash characteristics information for target CYTFM_064B0S2_4343W
* Cypress board.
*
********************************************************************************
* \copyright
* Copyright 2020 Cypress Semiconductor Corporation
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
*******************************************************************************/
#pragma once

#include "cyhal_flash.h"

#ifdef __cplusplus
extern "C" {
#endif

/** Gets flash characteristics like the start address, size, erase values etc
 *  for TF-M platform.
 * Refer \ref cyhal_flash_info_t for more information.
 *
 * @param[in]  obj  The flash object.
 * @param[out] info The flash characteristic info.
 *
 * Refer \ref subsection_flash_use_case_1 for more information.
 */
void cytfm_flash_get_info(const cyhal_flash_t *obj, cyhal_flash_info_t *info);

#ifdef __cplusplus
}
#endif

