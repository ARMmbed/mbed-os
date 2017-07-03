/***************************************************************************//**
* \file cy_crypto_core_prng.c
* \version 1.0
*
* \brief
*  This file provides the source code to the API for the PRNG
*  in the Crypto block driver.
*
********************************************************************************
* \copyright
* Copyright 2016-2017, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions,
* disclaimers, and limitations in the end user license agreement accompanying
* the software package with which this file was provided.
*******************************************************************************/

#include "crypto/cy_crypto_common.h"
#include "crypto/cy_crypto_core_prng.h"
#include "crypto/cy_crypto_core_instructions.h"
#include "crypto/cy_crypto_core_util.h"
#include "syslib/cy_syslib.h"


#if (CY_CPU_CORTEX_M0P)

#if ((CPUSS_CRYPTO_PRESENT == 1) && (CPUSS_CRYPTO_PR == 1) && (CY_CRYPTO_USER_PR == 1))


/*******************************************************************************
* Function Name: Cy_Crypto_Core_Prng_Init
****************************************************************************//**
*
* Initializes the PRND parameters.
* Invoking this function causes a restart of the pseudo-random sequence.
*
* This function available for CM0+ core only.
*
* \param cryptoPrngContext
* The pointer to thestc_crypto_context_t structure which stores
* the Crypto driver context.
*
* \return
* A Crypto status \ref cy_en_crypto_status_t.
*
*******************************************************************************/
cy_en_crypto_status_t Cy_Crypto_Core_Prng_Init(cy_stc_crypto_context_prng_t *cryptoPrngContext)
{
    CRYPTO->PR_LFSR_CTL0 = (uint32_t)(_VAL2FLD(CRYPTO_PR_LFSR_CTL0_LFSR32,
                           (uint32_t)cryptoPrngContext->lfsr32InitState));

    CRYPTO->PR_LFSR_CTL1 = (uint32_t)(_VAL2FLD(CRYPTO_PR_LFSR_CTL1_LFSR31,
                           (uint32_t)cryptoPrngContext->lfsr31InitState));

    CRYPTO->PR_LFSR_CTL2 = (uint32_t)(_VAL2FLD(CRYPTO_PR_LFSR_CTL2_LFSR29,
                           (uint32_t)cryptoPrngContext->lfsr29InitState));

    return (CY_CRYPTO_SUCCESS);
}

/*******************************************************************************
* Function Name: Crypto_Core_Prng
****************************************************************************//**
*
* Generates a Pseudo Random Number.
*
* This function available for CM0+ core only.
*
* \param cryptoPrngContext
* The pointer to the stc_crypto_context_t structure which stores
* the Crypto driver context.
*
* \return
* A Crypto status \ref cy_en_crypto_status_t.
*
*******************************************************************************/
cy_en_crypto_status_t Cy_Crypto_Core_Prng(cy_stc_crypto_context_prng_t *cryptoPrngContext)
{
    CRYPTO->INSTR_FF_WR = (uint32_t)(CY_CRYPTO_SET_REG1_OPC << CY_CRYPTO_OPCODE_POS);

    CRYPTO->INSTR_FF_WR = (uint32_t)cryptoPrngContext->max;

    CRYPTO->INSTR_FF_WR = (uint32_t)((uint32_t)CY_CRYPTO_PRNG_OPC << CY_CRYPTO_OPCODE_POS);

    /* Wait until the PRNG instruction is complete */
    while(0uL != _FLD2VAL(CRYPTO_STATUS_PR_BUSY, CRYPTO->STATUS))
    {
    }

    *cryptoPrngContext->prngNumPtr = (uint32_t)_FLD2VAL(CRYPTO_PR_RESULT_DATA32, CRYPTO->PR_RESULT);

    return (CY_CRYPTO_SUCCESS);
}


#endif /* #if ((CPUSS_CRYPTO_PRESENT == 1) && (CPUSS_CRYPTO_PR == 1) && (CY_CRYPTO_USER_PR == 1)) */

#endif /* #if (CY_CPU_CORTEX_M0P) */

/* [] END OF FILE */
