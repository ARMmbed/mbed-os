/***************************************************************************//**
* \file cy_mxconnbridge.c
* \version 1.0
*
* Provides an API implementation of the MXCONNBRIDGE driver
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

#include "cy_device.h"

#if defined (CY_IP_MXCONNBRIDGE)

#include "cy_mxconnbridge.h"

#if defined(__cplusplus)
extern "C" {
#endif

/*******************************************************************************
* Function Name: Cy_MXCONNBRIDGE_WLANResetMux
****************************************************************************//**
*
* Configure the WLAN RESET MUX bit to determine WLAN RESET value
*
* \param base
* Pointer to the mxconnbridge base address
*
* \param value
* value
*
* \return
* Logic level present on the signal
* 0 = WLAN reset is determined by WLAN_RESET_CTL
* 1 = WLAN reset is determined by RST_LHL_GPIO
*
* \funcusage
*
*******************************************************************************/
void Cy_MXCONNBRIDGE_WLANResetMux(MXCONNBRIDGE_Type *base, uint32_t value)
{
    uint32_t tempReg;
    CY_ASSERT_L2(CY_MXCONNBRIDGE_IS_VALUE_VALID(value));


    tempReg= MXCONNBRIDGE_AP_WLAN_CTL(base) & ~(MXCONNBRIDGE_SDIO_RESET_CTL_WLAN_RESET_N_MUX_Msk);
    MXCONNBRIDGE_AP_WLAN_CTL(base) = tempReg | _VAL2FLD(MXCONNBRIDGE_SDIO_RESET_CTL_WLAN_RESET_N_MUX, value);
}

/*******************************************************************************
* Function Name: Cy_MXCONNBRIDGE_SetWLANSDIOMux
****************************************************************************//**
*
* Configure to choose internal or external SDIO
*
* \param base
* Pointer to the mxconnbridge base address
*
* \param value
* 1'b0 means we use internal SDIO else 1'b1 means we use external SDIO
*
* \funcusage
*
*******************************************************************************/
void Cy_MXCONNBRIDGE_SetWLANSDIOMux(MXCONNBRIDGE_Type *base, uint32_t value)
{
    uint32_t tempReg;
    CY_ASSERT_L2(CY_MXCONNBRIDGE_IS_VALUE_VALID(value));


    tempReg= MXCONNBRIDGE_AP_WLAN_CTL(base) & ~(MXCONNBRIDGE_SDIO_RESET_CTL_WLAN_SDIO_MUX_Msk);
    MXCONNBRIDGE_AP_WLAN_CTL(base) = tempReg | _VAL2FLD(MXCONNBRIDGE_SDIO_RESET_CTL_WLAN_SDIO_MUX, value);
}

#if defined(__cplusplus)
}
#endif

#endif /* CY_IP_MXCONNBRIDGE */
/* [] END OF FILE */

