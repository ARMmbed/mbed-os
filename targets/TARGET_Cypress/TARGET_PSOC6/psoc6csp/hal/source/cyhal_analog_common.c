/***************************************************************************//**
* \file cyhal_analog_common.c
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

#include "cy_pdl.h"

#if defined(CY_IP_MXS40PASS_INSTANCES)

#if defined(__cplusplus)
extern "C"
{
#endif

static uint16_t cyhal_analog_ref_count = 0;

void cyhal_analog_init()
{
    if(cyhal_analog_ref_count == 0)
    {
        Cy_SysAnalog_Init(&Cy_SysAnalog_Fast_Local);
        Cy_SysAnalog_Enable();
    }

    ++cyhal_analog_ref_count;
}

void cyhal_analog_free()
{
    CY_ASSERT(cyhal_analog_ref_count > 0);
    --cyhal_analog_ref_count;
    if(cyhal_analog_ref_count)
    {
        Cy_SysAnalog_Disable();
        Cy_SysAnalog_DeInit();
    }
}

#if defined(__cplusplus)
}
#endif

#endif /* defined(CY_IP_MXS40PASS_INSTANCES) */
