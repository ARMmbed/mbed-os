/***************************************************************************//**
* \file cyhal_system.c
*
* \brief
* Provides a high level interface for interacting with the Cypress power
* management and system clock configuration. This interface abstracts out the
* chip specific details. If any chip specific functionality is necessary, or
* performance is critical the low level functions can be used directly.
*
********************************************************************************
* \copyright
* Copyright 2018-2021 Cypress Semiconductor Corporation
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

#include "cyhal_system.h"

#if (defined(COMPONENT_CAT1A) || defined(COMPONENT_CAT1B))
#include "cy_syslib.h"
#endif

#if defined(CY_RTOS_AWARE) || defined(COMPONENT_RTOS_AWARE)
#include "cyabs_rtos.h"
#endif

#if defined(CY_IP_MXS40SRSS) || defined(CY_IP_S8SRSSLT)

#if defined(__cplusplus)
extern "C"
{
#endif

#define _CYHAL_SYSTEM_HZ_PER_MHZ 1000000

#if (defined(COMPONENT_CAT1A) || defined(COMPONENT_CAT1B)) && !defined(CYHAL_DISABLE_WEAK_FUNC_IMPL)
/* Overrides weak implemenation for Cy_SysLib_Rtos_Delay to provide a way
* to call into a RTOS if so configured. This function is only available
* in mtb-pdl-cat1 version 2.2.0 or later.
*/
void Cy_SysLib_Rtos_Delay(uint32_t milliseconds)
{
    cy_rslt_t result = cyhal_system_delay_ms(milliseconds);
    CY_ASSERT(CY_RSLT_SUCCESS == result);
    CY_UNUSED_PARAMETER(result);
}
#endif

cy_rslt_t cyhal_system_delay_ms(uint32_t milliseconds)
{
#if defined(CY_RTOS_AWARE) || defined(COMPONENT_RTOS_AWARE)
    return cy_rtos_delay_milliseconds(milliseconds);
#else
    Cy_SysLib_Delay(milliseconds);
    return CY_RSLT_SUCCESS;
#endif
}

cyhal_reset_reason_t cyhal_system_get_reset_reason(void)
{
    uint32_t pdl_reason = Cy_SysLib_GetResetReason();
    cyhal_reset_reason_t reason = CYHAL_SYSTEM_RESET_NONE;

    if (CY_SYSLIB_RESET_SOFT & pdl_reason)
        reason |= CYHAL_SYSTEM_RESET_SOFT;
    if (CY_SYSLIB_RESET_HWWDT & pdl_reason)
        reason |= CYHAL_SYSTEM_RESET_WDT;
#if defined(CY_IP_S8SRSSLT)
    if (CY_SYSLIB_PROT_FAULT & pdl_reason)
        reason |= CYHAL_SYSTEM_RESET_PROTECTION;
#endif
#if defined(CY_IP_MXS40SRSS)
    if (CY_SYSLIB_RESET_ACT_FAULT & pdl_reason)
        reason |= CYHAL_SYSTEM_RESET_ACTIVE_FAULT;
    if (CY_SYSLIB_RESET_DPSLP_FAULT & pdl_reason)
        reason |= CYHAL_SYSTEM_RESET_DEEPSLEEP_FAULT;
    if (CY_SYSLIB_RESET_HIB_WAKEUP & pdl_reason)
        reason |= CYHAL_SYSTEM_RESET_HIB_WAKEUP;
    if ((CY_SYSLIB_RESET_SWWDT0 | CY_SYSLIB_RESET_SWWDT1 | CY_SYSLIB_RESET_SWWDT2 | CY_SYSLIB_RESET_SWWDT3) & pdl_reason)
        reason |= CYHAL_SYSTEM_RESET_WDT;
#endif
#if (SRSS_WCOCSV_PRESENT != 0U)
    if (CY_SYSLIB_RESET_CSV_WCO_LOSS & pdl_reason)
        reason |= CYHAL_SYSTEM_RESET_WCO_ERR;
#endif
#if (SRSS_MASK_HFCSV != 0)
    if ((CY_SYSLIB_RESET_HFCLK_LOSS | CY_SYSLIB_RESET_HFCLK_ERR) & pdl_reason)
        reason |= CYHAL_SYSTEM_RESET_SYS_CLK_ERR;
#endif

    return reason;
}

#if defined(__cplusplus)
}
#endif

#endif /* defined(CY_IP_MXS40SRSS) || defined(CY_IP_S8SRSSLT) */
