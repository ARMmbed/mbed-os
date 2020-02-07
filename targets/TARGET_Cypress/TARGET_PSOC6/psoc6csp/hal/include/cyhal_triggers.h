/***************************************************************************//**
* \file cyhal_triggers.h
*
* Description:
* Provides definitions for the triggers for each supported device family.
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

/**
* \addtogroup group_hal_psoc6_triggers Triggers
* \ingroup group_hal_psoc6
* \{
* Trigger connections for supported device families
*/

#pragma once

#if defined(__cplusplus)
extern "C" {
#endif /* __cplusplus */

#include "cy_device.h"

#ifdef CY_DEVICE_PSOC6ABLE2
#include "triggers/cyhal_triggers_psoc6_01.h"
#endif

#ifdef CY_DEVICE_PSOC6A2M
#include "triggers/cyhal_triggers_psoc6_02.h"
#endif

#ifdef CY_DEVICE_PSOC6A512K
#include "triggers/cyhal_triggers_psoc6_03.h"
#endif

#if defined(__cplusplus)
}
#endif /* __cplusplus */

/** \} group_hal_psoc6_triggers */
