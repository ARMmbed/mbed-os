/***************************************************************************//**
* \file cyhal_system_impl.h
*
* \brief
* Provides a PSoC Specific interface for interacting with the Cypress power
* management and system clock configuration. This interface abstracts out the
* chip specific details. If any chip specific functionality is necessary, or
* performance is critical the low level functions can be used directly.
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

#include "cyhal_system.h"

#ifdef CY_IP_MXS40SRSS

#define cyhal_system_critical_section_enter()   Cy_SysLib_EnterCriticalSection()

#define cyhal_system_critical_section_exit(x)   Cy_SysLib_ExitCriticalSection(x)

#define cyhal_system_sleep()                    Cy_SysPm_CpuEnterSleep(CY_SYSPM_WAIT_FOR_INTERRUPT)

#define cyhal_system_deepsleep()                Cy_SysPm_CpuEnterDeepSleep(CY_SYSPM_WAIT_FOR_INTERRUPT)

#define cyhal_system_delay_us(microseconds)     Cy_SysLib_DelayUs(microseconds)

#endif /* CY_IP_MXS40SRSS */
