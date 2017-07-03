/***************************************************************************//**
* \file cy_crypto_core_trng.c
* \version 1.0
*
* \brief
*  This file provides the source code to the API for the TRNG
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
#include "crypto/cy_crypto_core_trng.h"
#include "crypto/cy_crypto_core_instructions.h"
#include "crypto/cy_crypto_core_util.h"
#include "crypto/cy_crypto_core_trng_config.h"
#include "syslib/cy_syslib.h"


#if (CY_CPU_CORTEX_M0P)

#if ((CPUSS_CRYPTO_PRESENT == 1) && (CPUSS_CRYPTO_TR == 1) && (CY_CRYPTO_USER_TR == 1))

/*******************************************************************************
* Function Name: Crypto_Server_Trng_Init
****************************************************************************//**
*
* Initializes the TRND parameters.
*
* This function available for CM0+ core only.
*
*******************************************************************************/
void Cy_Crypto_Server_Trng_Init(void)
{
    CRYPTO->TR_CTL0 = CY_CRYPTO_TR_CTL0_VAL;
    CRYPTO->TR_CTL1 = CY_CRYPTO_TR_CTL1_VAL;
    CRYPTO->TR_MON_CTL = CY_CRYPTO_TR_BTSTR_SEL;
    CRYPTO->TR_MON_CMD = CY_CRYPTO_TR_START_MON;
    CRYPTO->TR_MON_RC_CTL = CY_CRYPTO_TR_RC_CUTOFF;
    CRYPTO->TR_MON_AP_CTL = CY_CRYPTO_TR_AC_CUTOFF;
}

/*******************************************************************************
* Function Name: Cy_Crypto_Core_Trng
****************************************************************************//**
*
* Generates a True Random Number and returns it in the
* cryptoTrngContext->trngNumPtr.
*
* This function available for CM0+ core only.
*
* \param cryptoTrngContext
* The pointer to the stc_crypto_context_t structure which stores
* the Crypto driver context.
*
* \return
* A Crypto status \ref en_crypto_status_t.
*
*******************************************************************************/
cy_en_crypto_status_t Cy_Crypto_Core_Trng(cy_stc_crypto_context_trng_t *cryptoTrngContext)
{
    Cy_Crypto_Server_Trng_Init();

    CRYPTO->TR_GARO_CTL = (uint32_t)cryptoTrngContext->GAROPol;
    CRYPTO->TR_FIRO_CTL = (uint32_t)cryptoTrngContext->FIROPol;

    CRYPTO->INSTR_FF_WR = (uint32_t)(CY_CRYPTO_SET_REG1_OPC << CY_CRYPTO_OPCODE_POS);

    CRYPTO->INSTR_FF_WR = (uint32_t)cryptoTrngContext->max;

    CRYPTO->INSTR_FF_WR = (uint32_t)((uint32_t)CY_CRYPTO_TRNG_OPC << CY_CRYPTO_OPCODE_POS);

    /* Wait until the TRNG instruction is complete */
    while(0uL != _FLD2VAL(CRYPTO_STATUS_TR_BUSY, CRYPTO->STATUS))
    {
    }

    *cryptoTrngContext->trngNumPtr = (uint32_t)_FLD2VAL(CRYPTO_TR_RESULT_DATA32, CRYPTO->TR_RESULT);

    return (CY_CRYPTO_SUCCESS);
}


#endif /* #if ((CPUSS_CRYPTO_PRESENT == 1) && (CPUSS_CRYPTO_TR == 1) && (CY_CRYPTO_USER_TR == 1)) */

#endif /* #if (CY_CPU_CORTEX_M0P) */

/* [] END OF FILE */
