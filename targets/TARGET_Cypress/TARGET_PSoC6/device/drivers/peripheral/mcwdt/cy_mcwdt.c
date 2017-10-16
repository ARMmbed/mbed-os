/***************************************************************************//**
* \file cy_mcwdt.c
* \version 1.0
*
*  Description:
*   Provides a system API for the MCWDT driver.
*
********************************************************************************
* Copyright 2016-2017, Cypress Semiconductor Corporation. All rights reserved.
* You may use this file only in accordance with the license, terms, conditions,
* disclaimers, and limitations in the end user license agreement accompanying
* the software package with which this file was provided.
*******************************************************************************/

#include "cy_mcwdt.h"

#if defined(__cplusplus)
extern "C" {
#endif


/*******************************************************************************
* Function Name: Cy_MCWDT_Init
****************************************************************************//**
*
* Initializes the MCWDT block.
*
*  \param base
*  The base pointer to a structure that describes the registers.
*
*  \param config
*  The pointer to a structure that contains component configuration data.
*
* \return cy_en_mcwdt_status_t
*     *base checking result. If the pointer is NULL, returns error.
*
*  \note
*  This API should not be called when the counters are running. Prior to calling
*  this API the counter should be disabled.
*
*******************************************************************************/
cy_en_mcwdt_status_t Cy_MCWDT_Init(MCWDT_STRUCT_Type *base, cy_stc_mcwdt_config_t const *config)
{
    cy_en_mcwdt_status_t ret = CY_MCWDT_BAD_PARAM;

    if ((base != NULL) && (config != NULL))
    {
        base->MCWDT_MATCH = _VAL2FLD(MCWDT_STRUCT_MCWDT_MATCH_WDT_MATCH1, config->c1Match) |
                                 _VAL2FLD(MCWDT_STRUCT_MCWDT_MATCH_WDT_MATCH0, config->c0Match);

        base->MCWDT_CONFIG = _VAL2FLD(MCWDT_STRUCT_MCWDT_CONFIG_WDT_BITS2, config->c2ToggleBit)      |
                               _VAL2FLD(MCWDT_STRUCT_MCWDT_CONFIG_WDT_MODE2, config->c2Mode)              |
                               _VAL2FLD(MCWDT_STRUCT_MCWDT_CONFIG_WDT_CLEAR0, config->c0ClearOnMatch)     |
                               _VAL2FLD(MCWDT_STRUCT_MCWDT_CONFIG_WDT_CLEAR1, config->c1ClearOnMatch)     |
                               (config->c1c2Cascade ? MCWDT_STRUCT_MCWDT_CONFIG_WDT_CASCADE1_2_Msk : 0UL) |
                               _VAL2FLD(MCWDT_STRUCT_MCWDT_CONFIG_WDT_MODE1, config->c1Mode)              |
                               (config->c0c1Cascade ? MCWDT_STRUCT_MCWDT_CONFIG_WDT_CASCADE0_1_Msk : 0UL) |
                               _VAL2FLD(MCWDT_STRUCT_MCWDT_CONFIG_WDT_MODE0, config->c0Mode);
                           
        ret = CY_MCWDT_SUCCESS;
    }
                           
    return (ret);
}


/*******************************************************************************
* Function Name: Cy_MCWDT_DeInit
****************************************************************************//**
*
*  De-initializes the MCWDT block, returns register values to their default state.
*
*  \param base
*  The base pointer to a structure that describes the registers.
*
*  \note
*  This API should not be called when the counters are running. Prior to calling
*  this API the counter should be disabled.
*
*******************************************************************************/
void Cy_MCWDT_DeInit(MCWDT_STRUCT_Type *base)
{
    Cy_MCWDT_Unlock(base);

    base->MCWDT_CNTLOW    = 0UL;
    base->MCWDT_CNTHIGH   = 0UL;
    base->MCWDT_MATCH     = 0UL;
    base->MCWDT_CONFIG    = 0UL;
    base->MCWDT_CTL       = 0UL;
    base->MCWDT_INTR      = 0UL;
    base->MCWDT_INTR_SET  = 0UL;
    base->MCWDT_INTR_MASK = 0UL;
}


#if defined(__cplusplus)
}
#endif

/* [] END OF FILE */
