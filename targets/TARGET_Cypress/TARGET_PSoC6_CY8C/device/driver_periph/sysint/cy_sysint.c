/***************************************************************************//**
* \file
* \version 1.0
*
* \brief
* Provides an API implementation of the sysint driver.
*
********************************************************************************
* \copyright
* Copyright 2016-2017, Cypress Semiconductor Corporation. All rights reserved.
* You may use this file only in accordance with the license, terms, conditions,
* disclaimers, and limitations in the end user license agreement accompanying
* the software package with which this file was provided.
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
* interrupt vector. Note that the interrupt vector will only be relocated if the
* vector table was moved to __ramVectors in SRAM. Otherwise it is ignored.
*
* Use the CMSIS core function NVIC_EnableIRQ(config.intrSrc)
* to enable it. For CM0+, this function also configures the interrupt mux
* connected to the NVIC. Use the CMSIS core function
* NVIC_EnableIRQ(config.intrCm0p) to enable it.
*
* \param config
* Interrupt configuration structure
*
* \param userIsr
* Address of the ISR
*
*******************************************************************************/
cy_en_sysint_status_t Cy_SysInt_Init(const cy_stc_sysint_t* config, cy_israddress userIsr)
{
    cy_en_sysint_status_t status = CY_SYSINT_SUCCESS;

    if(NULL != config)
    {
        #if (CY_CPU_CORTEX_M0P)
            if (config->intrSrc < 0)
            {
                NVIC_SetPriority(config->intrSrc, config->intrPriority);
            }
            else
            {
                /* Configure the interrupt mux */
                Cy_SysInt_SetIntSource(config);
                NVIC_SetPriority((IRQn_Type)(config->intrCm0p), config->intrPriority);
            }
        #else
            /* Set the priority */
            NVIC_SetPriority(config->intrSrc, config->intrPriority);
        #endif

        /* Only set the new vector if it was moved to __ramVectors */
        if (SCB->VTOR == (uint32_t)&__ramVectors)
        {
            (void)Cy_SysInt_SetVector(config, userIsr);
        }
    }
    else
    {
        status = CY_SYSINT_BAD_PARAM;
    }

    return(status);
}


#if CY_CPU_CORTEX_M0P || defined (CY_DOXYGEN)

/*******************************************************************************
* Function Name: Cy_SysInt_SetIntSource
****************************************************************************//**
*
* \brief Configures the interrupt mux for the specified CM0+ NVIC channel.
*
* Setting this value to 240 disconnects the interrupt source and will
* effectively deactivate the interrupt.
*
* \param config
* Interrupt configuration structure. This must be a positive number.
*
*******************************************************************************/
void Cy_SysInt_SetIntSource(const cy_stc_sysint_t* config)
{
    /* Calculation of variables and masks */
    uint8_t  intrMux    = config->intrCm0p;
    uint8_t  regPos     = intrMux >> CY_SYSINT_CM0P_MUX_SHIFT;
    uint8_t  bitPos     = (intrMux - (regPos << CY_SYSINT_CM0P_MUX_SHIFT)) << CY_SYSINT_CM0P_MUX_SCALE;
    uint32_t bitMask    = (uint32_t)(CY_SYSINT_CM0P_MUX_MASK << bitPos);
    uint32_t bitMaskClr = (uint32_t)(~bitMask);
    uint32_t bitMaskSet = ((config->intrSrc << bitPos) & bitMask);

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
* \return
* Interrupt source. A returned value of 240 denotes that the interrupt source
* is disconnected.
*
*******************************************************************************/
uint32_t Cy_SysInt_GetIntSource(IRQn_CM0P_Type muxSel)
{
    /* Calculation of variables */
    uint8_t  regPos  = muxSel >>  CY_SYSINT_CM0P_MUX_SHIFT;
    uint8_t  bitPos  = (muxSel - (regPos <<  CY_SYSINT_CM0P_MUX_SHIFT)) <<  CY_SYSINT_CM0P_MUX_SCALE;
    uint32_t bitMask = (uint32_t)(CY_SYSINT_CM0P_MUX_MASK << bitPos);

    uint32_t tempReg;

    switch(regPos)
    {
        case CY_SYSINT_CM0P_MUX0:
            tempReg = (uint32_t)((CPUSS->CM0_INT_CTL0 & bitMask) >> bitPos);
        break;
        case CY_SYSINT_CM0P_MUX1:
            tempReg = (uint32_t)((CPUSS->CM0_INT_CTL1 & bitMask) >> bitPos);
        break;
        case CY_SYSINT_CM0P_MUX2:
            tempReg = (uint32_t)((CPUSS->CM0_INT_CTL2 & bitMask) >> bitPos);
        break;
        case CY_SYSINT_CM0P_MUX3:
            tempReg = (uint32_t)((CPUSS->CM0_INT_CTL3 & bitMask) >> bitPos);
        break;
        case CY_SYSINT_CM0P_MUX4:
            tempReg = (uint32_t)((CPUSS->CM0_INT_CTL4 & bitMask) >> bitPos);
        break;
        case CY_SYSINT_CM0P_MUX5:
            tempReg = (uint32_t)((CPUSS->CM0_INT_CTL5 & bitMask) >> bitPos);
        break;
        case CY_SYSINT_CM0P_MUX6:
            tempReg = (uint32_t)((CPUSS->CM0_INT_CTL6 & bitMask) >> bitPos);
        break;
        case CY_SYSINT_CM0P_MUX7:
            tempReg = (uint32_t)((CPUSS->CM0_INT_CTL7 & bitMask) >> bitPos);
        break;
        default:
            tempReg = CY_SYSINT_CM0P_MUX_ERROR;
        break;
    }
    return tempReg;
}
#endif

/*******************************************************************************
* Function Name: Cy_SysInt_GetState
****************************************************************************//**
*
* \brief Gets the enabled/disabled state of the Interrupt.
*
* For CM0+, this function returns the state of the CM0+ interrupt mux output
* feeding into the NVIC.
*
* \param config
* Interrupt configuration structure
*
* \return
* 1 if enabled, 0 if disabled
*
*******************************************************************************/
uint32_t Cy_SysInt_GetState(const cy_stc_sysint_t* config)
{
    #if CY_CPU_CORTEX_M0P
        if (config->intrSrc < 0)
        {
            return (*(NVIC->ISER) >> config->intrSrc) & CY_SYSINT_STATE_MASK;
        }
        else
        {
            return (*(NVIC->ISER) >> config->intrCm0p) & CY_SYSINT_STATE_MASK;
        }
    #else
        return (*(NVIC->ISER) >> config->intrSrc) & CY_SYSINT_STATE_MASK;
    #endif
}


/*******************************************************************************
* Function Name: Cy_SysInt_SetVector
****************************************************************************//**
*
* \brief Changes the ISR vector for the Interrupt.
*
* For CM0+, this function sets the interrupt vector for the interrupt mux
* output feeding into the NVIC.
*
* Note that this function relies on the assumption that the vector table is
* relocated to __ramVectors[RAM_VECTORS_SIZE] in SRAM. Otherwise it will
* return the address of the default ISR location in Flash vector table.
*
* \param config
* Interrrupt configuration structure
*
* \param userIsr
* Address of the ISR to set in the interrupt vector table
*
* \return
* Previous address of the ISR in the interrupt vector table, before the
* function call
*
*******************************************************************************/
cy_israddress Cy_SysInt_SetVector(const cy_stc_sysint_t* config, cy_israddress userIsr)
{
    cy_israddress prevIsr;

    #if CY_CPU_CORTEX_M0P
        /* Only set the new vector if it was moved to __ramVectors */
        if (SCB->VTOR == (uint32_t)&__ramVectors)
        {
            if (config->intrSrc < 0)
            {
                prevIsr = __ramVectors[CY_INT_IRQ_BASE + config->intrSrc];
                __ramVectors[CY_INT_IRQ_BASE + config->intrSrc] = userIsr;
            }
            else
            {
                prevIsr = __ramVectors[CY_INT_IRQ_BASE + config->intrCm0p];
                __ramVectors[CY_INT_IRQ_BASE + config->intrCm0p] = userIsr;
            }
        }
        else
        {
            if (config->intrSrc < 0)
            {
                prevIsr = __Vectors[CY_INT_IRQ_BASE + config->intrSrc];
            }
            else
            {
                prevIsr = __Vectors[CY_INT_IRQ_BASE + config->intrCm0p];
            }
        }
    #else
        /* Only set the new vector if it was moved to __ramVectors */
        if (SCB->VTOR == (uint32_t)&__ramVectors)
        {
            prevIsr = __ramVectors[CY_INT_IRQ_BASE + config->intrSrc];
            __ramVectors[CY_INT_IRQ_BASE + config->intrSrc] = userIsr;
        }
        else
        {
            prevIsr = __Vectors[CY_INT_IRQ_BASE + config->intrSrc];
        }
    #endif

    return prevIsr;
}


/*******************************************************************************
* Function Name: Cy_SysInt_GetVector
****************************************************************************//**
*
* \brief Gets the address of the current ISR vector for the Interrupt.
*
* For CM0+, this function returns the interrupt vector for the interrupt mux
* output feeding into the NVIC.
*
* Note that this function relies on the assumption that the vector table is
* relocated to __ramVectors[RAM_VECTORS_SIZE] in SRAM.
*
* \param config
* Interrupt configuration structure
*
* \return
* Address of the ISR in the interrupt vector table
*
*******************************************************************************/
cy_israddress Cy_SysInt_GetVector(const cy_stc_sysint_t* config)
{
    #if CY_CPU_CORTEX_M0P
        /* Only return the SRAM ISR address if it was moved to __ramVectors */
        if (SCB->VTOR == (uint32_t)&__ramVectors)
        {
            if (config->intrSrc < 0)
            {
                return __ramVectors[CY_INT_IRQ_BASE + config->intrSrc];
            }
            else
            {
                return __ramVectors[CY_INT_IRQ_BASE + config->intrCm0p];
            }
        }
        else
        {
            if (config->intrSrc < 0)
            {
                return __Vectors[CY_INT_IRQ_BASE + config->intrSrc];
            }
            else
            {
                return __Vectors[CY_INT_IRQ_BASE + config->intrCm0p];
            }
        }
    #else
         /* Only return the SRAM ISR address if it was moved to __ramVectors */
        if (SCB->VTOR == (uint32_t)&__ramVectors)
        {
            return __ramVectors[CY_INT_IRQ_BASE + config->intrSrc];
        }
        else
        {
            return __Vectors[CY_INT_IRQ_BASE + config->intrSrc];
        }
    #endif
}


#if defined(__cplusplus)
}
#endif


/* [] END OF FILE */
