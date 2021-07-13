/***************************************************************************//**
* \file  cy_sysint.c
* \version 1.60
*
* \brief
* Provides an API implementation of the SysInt driver.
*
********************************************************************************
* \copyright
* Copyright 2016-2020 Cypress Semiconductor Corporation
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

#if defined (CY_IP_M4CPUSS)

#include "cy_sysint.h"

CY_MISRA_FP_BLOCK_START('MISRA C-2012 Rule 8.3', 1, 'Only one prototype will be pciked for compilation');
CY_MISRA_FP_BLOCK_START('MISRA C-2012 Rule 8.6', 2, 'Only one prototype will be pciked for compilation');
#if (!CY_CPU_CORTEX_M0P) || defined (CY_DOXYGEN)
void Cy_SysInt_SetNmiSource(cy_en_sysint_nmi_t nmiNum, IRQn_Type intrSrc)
#else
void Cy_SysInt_SetNmiSource(cy_en_sysint_nmi_t nmiNum, cy_en_intr_t intrSrc)
#endif
{
    CY_ASSERT_L3(CY_SYSINT_IS_NMI_NUM_VALID(nmiNum));

#if (CY_CPU_CORTEX_M0P)
    CY_ASSERT_L1(CY_SYSINT_IS_PC_0);
#endif

    if (CY_CPUSS_V1)
    {
        nmiNum = CY_SYSINT_NMI1; /* For CPUSS_ver1 the NMI number is 1 */
    }

    #if (CY_CPU_CORTEX_M0P)
        CPUSS_CM0_NMI_CTL((uint32_t)nmiNum - 1UL) = (uint32_t)intrSrc;
    #else
        CPUSS_CM4_NMI_CTL((uint32_t)nmiNum - 1UL) = (uint32_t)intrSrc;
    #endif
}

#if (!CY_CPU_CORTEX_M0P) || defined (CY_DOXYGEN)
IRQn_Type Cy_SysInt_GetNmiSource(cy_en_sysint_nmi_t nmiNum)
#else
cy_en_intr_t Cy_SysInt_GetNmiSource(cy_en_sysint_nmi_t nmiNum)
#endif
{
    CY_ASSERT_L3(CY_SYSINT_IS_NMI_NUM_VALID(nmiNum));

    if (CY_CPUSS_V1)
    {
        nmiNum = CY_SYSINT_NMI1; /* For CPUSS_ver1 the NMI number is 1 */
    }

    #if (CY_CPU_CORTEX_M0P)
        return ((cy_en_intr_t)(CPUSS_CM0_NMI_CTL((uint32_t)nmiNum - 1UL)));
    #else
        return ((IRQn_Type)(CPUSS_CM4_NMI_CTL((uint32_t)nmiNum - 1UL)));
    #endif
}
CY_MISRA_BLOCK_END('MISRA C-2012 Rule 8.6');
CY_MISRA_BLOCK_END('MISRA C-2012 Rule 8.3');

cy_en_sysint_status_t Cy_SysInt_Init(const cy_stc_sysint_t* config, cy_israddress userIsr)
{
    cy_en_sysint_status_t status = CY_SYSINT_SUCCESS;

    if(NULL != config)
    {
        CY_ASSERT_L3(CY_SYSINT_IS_PRIORITY_VALID(config->intrPriority));

        #if (CY_CPU_CORTEX_M0P)
            if (config->intrSrc > SysTick_IRQn)
            {
                Cy_SysInt_SetInterruptSource(config->intrSrc, config->cm0pSrc);
            }
            else
            {
                status = CY_SYSINT_BAD_PARAM;
            }
        #endif

        NVIC_SetPriority(config->intrSrc, config->intrPriority);

        /* Set the new vector only if it was moved to __ramVectors */
        if (SCB->VTOR == (uint32_t)&__ramVectors)
        {
            (void)Cy_SysInt_SetVector(config->intrSrc, userIsr);
        }
    }
    else
    {
        status = CY_SYSINT_BAD_PARAM;
    }

    return(status);
}


#if (CY_CPU_CORTEX_M0P) || defined (CY_DOXYGEN)
void Cy_SysInt_SetInterruptSource(IRQn_Type IRQn, cy_en_intr_t devIntrSrc)
{
    if (CY_CPUSS_V1)
    {
        uint32_t regPos = ((uint32_t)IRQn >> CY_SYSINT_CM0P_MUX_SHIFT);
        if(0UL == (regPos & (uint32_t)~CY_SYSINT_MUX_REG_MSK))
        {
            uint32_t bitfield_Pos = (uint32_t)((uint32_t)IRQn - (uint32_t)(regPos << CY_SYSINT_CM0P_MUX_SHIFT)) << CY_SYSINT_CM0P_MUX_SCALE;
            uint32_t bitfield_Msk = (uint32_t)(CY_SYSINT_CM0P_MUX_MASK << bitfield_Pos);

            CY_REG32_CLR_SET(CPUSS_CM0_INT_CTL[regPos], bitfield, devIntrSrc);
        }
    }
    else /* CPUSS_V2 */
    {
        CY_ASSERT_L1(CY_CPUSS_DISCONNECTED_IRQN != devIntrSrc); /* Disconnection feature doesn't work for CPUSS_V2 */

        CPUSS_CM0_SYSTEM_INT_CTL[devIntrSrc] = _VAL2FLD(CPUSS_V2_CM0_SYSTEM_INT_CTL_CPU_INT_IDX, IRQn)
                                                      | CPUSS_V2_CM0_SYSTEM_INT_CTL_CPU_INT_VALID_Msk;
    }
}


void Cy_SysInt_DisconnectInterruptSource(IRQn_Type IRQn, cy_en_intr_t devIntrSrc)
{
    if (CY_CPUSS_V1)
    {
        Cy_SysInt_SetInterruptSource(IRQn, CY_CPUSS_DISCONNECTED_IRQN);
    }
    else /* CPUSS_V2 */
    {
        CPUSS_CM0_SYSTEM_INT_CTL[devIntrSrc] &= (uint32_t)~ CPUSS_V2_CM0_SYSTEM_INT_CTL_CPU_INT_VALID_Msk;
    }
}


cy_en_intr_t Cy_SysInt_GetInterruptSource(IRQn_Type IRQn)
{
    uint32_t tempReg = CY_CPUSS_NOT_CONNECTED_IRQN;

    if (CY_CPUSS_V1)
    {
        uint32_t regPos  = ((uint32_t)IRQn >> CY_SYSINT_CM0P_MUX_SHIFT);
        if(0UL == (regPos & (uint32_t)~CY_SYSINT_MUX_REG_MSK))
        {
            uint32_t bitfield_Pos  = ((uint32_t)IRQn - (regPos <<  CY_SYSINT_CM0P_MUX_SHIFT)) <<  CY_SYSINT_CM0P_MUX_SCALE;
            uint32_t bitfield_Msk = (uint32_t)(CY_SYSINT_CM0P_MUX_MASK << bitfield_Pos);

            tempReg = _FLD2VAL(bitfield, CPUSS_CM0_INT_CTL[regPos]);
        }
    }

    return ((cy_en_intr_t)tempReg);
}


IRQn_Type Cy_SysInt_GetNvicConnection(cy_en_intr_t devIntrSrc)
{
    uint32_t tempReg = CY_CPUSS_NOT_CONNECTED_IRQN;

    if ((!CY_CPUSS_V1) && (CY_SYSINT_ENABLE == _FLD2VAL(CPUSS_V2_CM0_SYSTEM_INT_CTL_CPU_INT_VALID, CPUSS_CM0_SYSTEM_INT_CTL[devIntrSrc])))
    {
        tempReg = _FLD2VAL(CPUSS_V2_CM0_SYSTEM_INT_CTL_CPU_INT_IDX, CPUSS_CM0_SYSTEM_INT_CTL[devIntrSrc]);
    }
    return ((IRQn_Type)tempReg);
}


cy_en_intr_t Cy_SysInt_GetInterruptActive(IRQn_Type IRQn)
{
    uint32_t tempReg = CY_CPUSS_NOT_CONNECTED_IRQN;
    uint32_t locIdx = (uint32_t)IRQn & CY_SYSINT_INT_STATUS_MSK;

    if (!CY_CPUSS_V1)
    {
        if(CY_SYSINT_ENABLE == _FLD2VAL(CPUSS_V2_CM0_INT0_STATUS_SYSTEM_INT_VALID, CPUSS_CM0_INT_STATUS[locIdx]))
        {
            tempReg = _FLD2VAL(CPUSS_V2_CM0_INT0_STATUS_SYSTEM_INT_IDX, CPUSS_CM0_INT_STATUS[locIdx]);
        }
    }
    return ((cy_en_intr_t)tempReg);
}

#endif



cy_israddress Cy_SysInt_SetVector(IRQn_Type IRQn, cy_israddress userIsr)
{
    cy_israddress prevIsr;

    /* Set the new vector only if it was moved to __ramVectors */
    if (SCB->VTOR == (uint32_t)&__ramVectors)
    {
        CY_ASSERT_L1(CY_SYSINT_IS_VECTOR_VALID(userIsr));

        prevIsr = __ramVectors[CY_INT_IRQ_BASE + (uint32_t)IRQn];
        __ramVectors[CY_INT_IRQ_BASE + (uint32_t)IRQn] = userIsr;
    }
    else
    {
        prevIsr = __Vectors[CY_INT_IRQ_BASE + (uint32_t)IRQn];
    }

    return (prevIsr);
}


cy_israddress Cy_SysInt_GetVector(IRQn_Type IRQn)
{
    cy_israddress currIsr;

    /* Return the SRAM ISR address only if it was moved to __ramVectors */
    if (SCB->VTOR == (uint32_t)&__ramVectors)
    {
        currIsr = __ramVectors[CY_INT_IRQ_BASE + (uint32_t)IRQn];
    }
    else
    {
        currIsr = __Vectors[CY_INT_IRQ_BASE + (uint32_t)IRQn];
    }

    return (currIsr);
}

#if (!CY_CPU_CORTEX_M0P)
void Cy_SysInt_SoftwareTrig(IRQn_Type IRQn)
{
    NVIC->STIR = (uint32_t)IRQn & CY_SYSINT_STIR_MASK;
}
#endif

#endif

/* [] END OF FILE */
