/***************************************************************************//**
* \file  cy_sysint.c
* \version 1.10
*
* \brief
* Provides an API implementation of the SysInt driver.
*
********************************************************************************
* \copyright
* Copyright 2016-2018, Cypress Semiconductor Corporation.  All rights reserved.
* SPDX-License-Identifier: Apache-2.0
*******************************************************************************/

#include "cy_sysint.h"

#if defined(__cplusplus)
extern "C" {
#endif


/*******************************************************************************
* Function Name: Cy_SysInt_Init
****************************************************************************//**
*
* \brief Initializes the referenced interrupt by setting the priority and the
* interrupt vector.
*
* Note that the interrupt vector will only be relocated if the vector table was
* moved to __ramVectors in SRAM. Otherwise it is ignored.
*
* Use the CMSIS core function NVIC_EnableIRQ(config.intrSrc) to enable the interrupt.
*
* \param config
* Interrupt configuration structure
*
* \param userIsr
* Address of the ISR
*
* \return
* Initialization status
*
* \funcusage
* \snippet sysint/sysint_v1_10_sut_01.cydsn/main_cm4.c snippet_Cy_SysInt_Init
*
*******************************************************************************/
cy_en_sysint_status_t Cy_SysInt_Init(const cy_stc_sysint_t* config, cy_israddress userIsr)
{
    cy_en_sysint_status_t status = CY_SYSINT_SUCCESS;

    if(NULL != config)
    {
        CY_ASSERT_L3(CY_SYSINT_IS_PRIORITY_VALID(config->intrPriority));

        #if (CY_CPU_CORTEX_M0P)
            if (config->intrSrc > SysTick_IRQn)
            {
                /* Configure the interrupt mux */
                Cy_SysInt_SetIntSource(config->intrSrc, config->cm0pSrc);
            }
        #endif

        NVIC_SetPriority(config->intrSrc, config->intrPriority);

        /* Only set the new vector if it was moved to __ramVectors */
        if (SCB->VTOR == (uint32_t)&__ramVectors)
        {
            CY_ASSERT_L1(CY_SYSINT_IS_VECTOR_VALID(userIsr));

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

/*******************************************************************************
* Function Name: Cy_SysInt_SetIntSource
****************************************************************************//**
*
* \brief Configures the interrupt mux for the specified CM0+ NVIC channel.
*
* Setting this value to "disconnected_IRQn" (240) disconnects the interrupt
* source and will effectively deactivate the interrupt.
*
* \param intrSrc
* NVIC mux number connected to the NVIC channel of the CM0+ core
*
* \param cm0pSrc
* Device interrupt to be routed to the NVIC mux
*
* \funcusage
* \snippet sysint/sysint_v1_10_sut_01.cydsn/main_cm4.c snippet_Cy_SysInt_SetIntSource
*
*******************************************************************************/
void Cy_SysInt_SetIntSource(IRQn_Type intrSrc, cy_en_intr_t cm0pSrc)
{
    /* Calculation of variables and masks */
    uint32_t regPos     = (uint32_t)intrSrc >> CY_SYSINT_CM0P_MUX_SHIFT;
    uint32_t bitPos     = ((uint32_t)intrSrc - (uint32_t)(regPos << CY_SYSINT_CM0P_MUX_SHIFT)) << CY_SYSINT_CM0P_MUX_SCALE;
    uint32_t bitMask    = (uint32_t)(CY_SYSINT_CM0P_MUX_MASK << bitPos);
    uint32_t bitMaskClr = (uint32_t)(~bitMask);
    uint32_t bitMaskSet = (((uint32_t)cm0pSrc << bitPos) & bitMask);

    uint32_t tempReg;

    switch(regPos)
    {
        case CY_SYSINT_CM0P_MUX0:
            tempReg = CPUSS->CM0_INT_CTL0 & bitMaskClr;
            CPUSS->CM0_INT_CTL0 = tempReg | bitMaskSet;
        break;
        case CY_SYSINT_CM0P_MUX1:
            tempReg = CPUSS->CM0_INT_CTL1 & bitMaskClr;
            CPUSS->CM0_INT_CTL1 = tempReg | bitMaskSet;
        break;
        case CY_SYSINT_CM0P_MUX2:
            tempReg = CPUSS->CM0_INT_CTL2 & bitMaskClr;
            CPUSS->CM0_INT_CTL2 = tempReg | bitMaskSet;
        break;
        case CY_SYSINT_CM0P_MUX3:
            tempReg = CPUSS->CM0_INT_CTL3 & bitMaskClr;
            CPUSS->CM0_INT_CTL3 = tempReg | bitMaskSet;
        break;
        case CY_SYSINT_CM0P_MUX4:
            tempReg = CPUSS->CM0_INT_CTL4 & bitMaskClr;
            CPUSS->CM0_INT_CTL4 = tempReg | bitMaskSet;
        break;
        case CY_SYSINT_CM0P_MUX5:
            tempReg = CPUSS->CM0_INT_CTL5 & bitMaskClr;
            CPUSS->CM0_INT_CTL5 = tempReg | bitMaskSet;
        break;
        case CY_SYSINT_CM0P_MUX6:
            tempReg = CPUSS->CM0_INT_CTL6 & bitMaskClr;
            CPUSS->CM0_INT_CTL6 = tempReg | bitMaskSet;
        break;
        case CY_SYSINT_CM0P_MUX7:
            tempReg = CPUSS->CM0_INT_CTL7 & bitMaskClr;
            CPUSS->CM0_INT_CTL7 = tempReg | bitMaskSet;
        break;
        default:
        break;
    }
}


/*******************************************************************************
* Function Name: Cy_SysInt_GetIntSource
****************************************************************************//**
*
* \brief Gets the interrupt source of CM0+ NVIC channel.
*
* \param intrSrc
* NVIC mux number connected to the NVIC channel of the CM0+ core
*
* \return
* Device interrupt source connected to the NVIC mux. A returned value of
* "disconnected_IRQn" (240) indicates that the interrupt source is disconnected.
*
* \funcusage
* \snippet sysint/sysint_v1_10_sut_01.cydsn/main_cm4.c snippet_Cy_SysInt_SetIntSource
*
*******************************************************************************/
cy_en_intr_t Cy_SysInt_GetIntSource(IRQn_Type intrSrc)
{
    /* Calculation of variables */
    uint32_t regPos  = (uint32_t)intrSrc >>  CY_SYSINT_CM0P_MUX_SHIFT;
    uint32_t bitPos  = ((uint32_t)intrSrc - (regPos <<  CY_SYSINT_CM0P_MUX_SHIFT)) <<  CY_SYSINT_CM0P_MUX_SCALE;
    uint32_t bitMask = (uint32_t)(CY_SYSINT_CM0P_MUX_MASK << bitPos);

    cy_en_intr_t srcVal = disconnected_IRQn;
    uint32_t tempReg = 0UL;

    switch(regPos)
    {
        case CY_SYSINT_CM0P_MUX0:
            tempReg = (CPUSS->CM0_INT_CTL0 & bitMask) >> bitPos;
        break;
        case CY_SYSINT_CM0P_MUX1:
            tempReg = (CPUSS->CM0_INT_CTL1 & bitMask) >> bitPos;
        break;
        case CY_SYSINT_CM0P_MUX2:
            tempReg = (CPUSS->CM0_INT_CTL2 & bitMask) >> bitPos;
        break;
        case CY_SYSINT_CM0P_MUX3:
            tempReg = (CPUSS->CM0_INT_CTL3 & bitMask) >> bitPos;
        break;
        case CY_SYSINT_CM0P_MUX4:
            tempReg = (CPUSS->CM0_INT_CTL4 & bitMask) >> bitPos;
        break;
        case CY_SYSINT_CM0P_MUX5:
            tempReg = (CPUSS->CM0_INT_CTL5 & bitMask) >> bitPos;
        break;
        case CY_SYSINT_CM0P_MUX6:
            tempReg = (CPUSS->CM0_INT_CTL6 & bitMask) >> bitPos;
        break;
        case CY_SYSINT_CM0P_MUX7:
            tempReg = (CPUSS->CM0_INT_CTL7 & bitMask) >> bitPos;
        break;
        default:
        break;
    }

    srcVal = (cy_en_intr_t)tempReg;
    return (srcVal);
}
#endif


/*******************************************************************************
* Function Name: Cy_SysInt_SetVector
****************************************************************************//**
*
* \brief Changes the ISR vector for the Interrupt.
*
* Note that for CM0+, this function sets the interrupt vector for the interrupt
* mux output feeding into the NVIC.
*
* Note that this function relies on the assumption that the vector table is
* relocated to __ramVectors[RAM_VECTORS_SIZE] in SRAM. Otherwise it will
* return the address of the default ISR location in Flash vector table.
*
* \param intrSrc
* Interrrupt source
*
* \param userIsr
* Address of the ISR to set in the interrupt vector table
*
* \return
* Previous address of the ISR in the interrupt vector table, before the
* function call
*
* \funcusage
* \snippet sysint/sysint_v1_10_sut_01.cydsn/main_cm4.c snippet_Cy_SysInt_SetVector
*
*******************************************************************************/
cy_israddress Cy_SysInt_SetVector(IRQn_Type intrSrc, cy_israddress userIsr)
{
    cy_israddress prevIsr;

    /* Only set the new vector if it was moved to __ramVectors */
    if (SCB->VTOR == (uint32_t)&__ramVectors)
    {
        CY_ASSERT_L1(CY_SYSINT_IS_VECTOR_VALID(userIsr));

        prevIsr = __ramVectors[CY_INT_IRQ_BASE + intrSrc];
        __ramVectors[CY_INT_IRQ_BASE + intrSrc] = userIsr;
    }
    else
    {
        prevIsr = __Vectors[CY_INT_IRQ_BASE + intrSrc];
    }

    return prevIsr;
}


/*******************************************************************************
* Function Name: Cy_SysInt_GetVector
****************************************************************************//**
*
* \brief Gets the address of the current ISR vector for the Interrupt.
*
* Note that for CM0+, this function returns the interrupt vector for the
* interrupt mux output feeding into the NVIC.
*
* Note that this function relies on the assumption that the vector table is
* relocated to __ramVectors[RAM_VECTORS_SIZE] in SRAM.
*
* \param intrSrc
* Interrupt source
*
* \return
* Address of the ISR in the interrupt vector table
*
* \funcusage
* \snippet sysint/sysint_v1_10_sut_01.cydsn/main_cm4.c snippet_Cy_SysInt_SetVector
*
*******************************************************************************/
cy_israddress Cy_SysInt_GetVector(IRQn_Type intrSrc)
{
    cy_israddress currIsr;

    /* Only return the SRAM ISR address if it was moved to __ramVectors */
    if (SCB->VTOR == (uint32_t)&__ramVectors)
    {
        currIsr = __ramVectors[CY_INT_IRQ_BASE + intrSrc];
    }
    else
    {
        currIsr = __Vectors[CY_INT_IRQ_BASE + intrSrc];
    }

    return currIsr;
}


#if defined(__cplusplus)
}
#endif


/* [] END OF FILE */
