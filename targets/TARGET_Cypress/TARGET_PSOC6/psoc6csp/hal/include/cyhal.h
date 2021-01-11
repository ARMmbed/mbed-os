/*******************************************************************************
* File Name: cyhal.h
*
* Description:
* Top-level HAL header file that can be referenced to pull in all relevant
* drivers for the current device architecture. Not all of these may be
* supported in the current target device. The implementation must provide a
* cyhal_hw_types.h and a cyhal_drivers.h file in the include path for this
* to depend on.
* The cyhal_hw_types.h file must provide the following:
*   1) definitions for each of the resource types consumed by the HAL driver
*      functions.
*   2) A CYHAL_ISR_PRIORITY_DEFAULT define for the default interrupt priority
*   3) A CYHAL_MAIN_FLASH_BASE define for the base address of flash memory
*   4) OPTIONAL: Implementation specific header files can be used by creating
*      a #define in cyhal_hw_types.h with a name of CYHAL_<DRIVER>_IMPL_HEADER
*      and the value being the name of the header file.  eg:
*      #define CYHAL_GPIO_IMPL_HEADER "cyhal_gpio_impl.h"
* The cyhal_drivers.h file must simply be a list of include directives to pull
* in the relevant driver header files.
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
* \addtogroup group_hal_types HAL General Types/Macros
* This section documents the basic types and macros that are used by multiple HAL drivers.
*/

/**
* \addtogroup group_result Result Type
* \ingroup group_hal_types
*/

/**
* \addtogroup group_hal HAL Drivers
* This section documents the drivers which form the stable API of the Cypress HAL.
* In order to remain portable across platforms and HAL versions, applications should
* rely only on functionality documented in this section.
*/

/**
* \addtogroup group_hal_impl
*/

#pragma once

#include "cyhal_general_types.h"
#include "cyhal_hw_types.h"
#include "cyhal_drivers.h"
