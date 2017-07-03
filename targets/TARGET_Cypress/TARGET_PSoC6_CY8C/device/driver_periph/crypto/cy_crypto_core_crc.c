/***************************************************************************//**
* \file cy_crypto_core_crc.c
* \version 1.0
*
* \brief
*  This file provides the source code for CRC API
*  in the Crypto driver.
*
********************************************************************************
* \copyright
* Copyright 2016-2017, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions,
* disclaimers, and limitations in the end user license agreement accompanying
* the software package with which this file was provided.
*******************************************************************************/

#include "crypto/cy_crypto_common.h"
#include "crypto/cy_crypto_core_crc.h"
#include "crypto/cy_crypto_core_util.h"
#include "crypto/cy_crypto_core_instructions.h"
#include "syslib/cy_syslib.h"


#if (CY_CPU_CORTEX_M0P)

#if ((CPUSS_CRYPTO_PRESENT == 1) && (CPUSS_CRYPTO_CRC == 1) && (CY_CRYPTO_USER_CRC == 1))


/*******************************************************************************
* Function Name: Cy_Crypto_Core_Crc_Init
****************************************************************************//**
*
* Initializes CRC calculation.
*
* This function available for CM0+ core only.
*
* \param cryptoCrcContext
* The pointer to the stc_crypto_context_t structure which stores
* the Crypto driver context.
*
* \return
* A Crypto status \ref cy_en_crypto_status_t.
*
*******************************************************************************/
cy_en_crypto_status_t Cy_Crypto_Core_Crc_Init(cy_stc_crypto_context_crc_t *cryptoCrcContext)
{

    /* Specifies the bit order in which a data Byte is processed
     * (reversal is performed after XORing):
     *                                       '0': Most significant bit (bit 1) first.
     *                                       '1': Least significant bit (bit 0) first. */
    CRYPTO->CRC_CTL = (uint32_t)( (_VAL2FLD(CRYPTO_CRC_CTL_DATA_REVERSE, (uint32_t)(cryptoCrcContext->dataReverse))) |
                                  (_VAL2FLD(CRYPTO_CRC_CTL_REM_REVERSE,  (uint32_t)(cryptoCrcContext->remReverse))) );

    /* Specifies a byte mask with which each data byte is XORed.
     * The XOR is performed before data reversal. */
    CRYPTO->CRC_DATA_CTL = (uint32_t)(_VAL2FLD(CRYPTO_CRC_DATA_CTL_DATA_XOR, (uint32_t)cryptoCrcContext->dataXor));

    /* CRC polynomial. The polynomial is represented WITHOUT the high order bit
     * (this bit is always assumed '1'). */
    CRYPTO->CRC_POL_CTL = (uint32_t)(_VAL2FLD(CRYPTO_CRC_POL_CTL_POLYNOMIAL, (uint32_t)cryptoCrcContext->polynomial));

    /*Specifies a mask with which the CRC_LFSR_CTL.LFSR32 register is XORed to produce a remainder.
     * The XOR is performed before remainder reversal. */
    CRYPTO->CRC_REM_CTL = (uint32_t)(_VAL2FLD(CRYPTO_CRC_REM_CTL_REM_XOR, (uint32_t)cryptoCrcContext->remXor));

    return (CY_CRYPTO_SUCCESS);
}

/*******************************************************************************
* Function Name: Cy_Crypto_Server_Crc
****************************************************************************//**
*
* Performs CRC calculation on a message.
*
* This function available for CM0+ core only.
*
* \param cryptoCrcContext
* The pointer to the stc_crypto_context_t structure which stores
* vCrypto driver context.
*
* \return
* A Crypto status \ref cy_en_crypto_status_t.
*
*******************************************************************************/
cy_en_crypto_status_t Cy_Crypto_Core_Crc(cy_stc_crypto_context_crc_t *cryptoCrcContext)
{

    /* A state of 32-bit Linear Feedback Shift Registers (LFSR) used to implement CRC. */
    CRYPTO->CRC_LFSR_CTL = (uint32_t)(_VAL2FLD(CRYPTO_CRC_LFSR_CTL_LFSR32, (uint32_t)cryptoCrcContext->lfsrInitState));

    /* Fill the FIFO with the instruction parameters */
    Cy_Crypto_SetReg2Instr( (uint32_t)cryptoCrcContext->srcDataPtr,
                            (uint32_t)cryptoCrcContext->dataSize );

    /* Issue the CRC instruction */
    Cy_Crypto_Run2ParamInstr(CY_CRYPTO_CRC_OPC,
                             CY_CRYPTO_RSRC0_SHIFT,
                             CY_CRYPTO_RSRC4_SHIFT);

    /* Wait until STR instruction is complete */
    while(0uL != _FLD2VAL(CRYPTO_STATUS_CRC_BUSY, CRYPTO->STATUS))
    {
    }

    /* Copy the result from the CRC_REM_RESULT register */
    *cryptoCrcContext->crcPtr = (uint32_t)_FLD2VAL(CRYPTO_CRC_REM_RESULT_REM, CRYPTO->CRC_REM_RESULT);

    return (CY_CRYPTO_SUCCESS);
}


#endif /* #if ((CPUSS_CRYPTO_PRESENT == 1) && (CPUSS_CRYPTO_CRC == 1) && (CY_CRYPTO_USER_CRC == 1)) */

#endif /* #if (CY_CPU_CORTEX_M0P) */

/* [] END OF FILE */
