/***************************************************************************//**
* \file cyhal_analog_common.h
*
* \brief
* Provides common functionality that needs to be shared among all drivers that
* interact with the Programmable Analog Subsystem.
*
********************************************************************************
* \copyright
* Copyright 2018-2020 Cypress Semiconductor Corporation
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

#if defined(__cplusplus)
extern "C" {
#endif

/**
 * Initialize the programmable analog. This utilizes reference counting to avoid
 * repeatedly initializing the analog subsystem when multiple analog blocks are in use
 * */
void cyhal_analog_init();

/**
 * Uninitialize the programmable analog. This utilizes reference counting to avoid
 * disabling the analog subsystem until all blocks which require it have been freed.
 */
void cyhal_analog_free();

#if defined(__cplusplus)
}
#endif