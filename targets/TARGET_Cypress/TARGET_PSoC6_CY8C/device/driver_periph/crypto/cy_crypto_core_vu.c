/***************************************************************************//**
* \file cy_crypto_core_vu.c
* \version 1.0
*
* \brief
*  This file provides the source code to the API for the Vector Unit functions
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
#include "crypto/cy_crypto_core_instructions.h"
#include "crypto/cy_crypto_core_vu.h"
#include "crypto/cy_crypto_core_util.h"
#include "syslib/cy_syslib.h"

#if (CY_CPU_CORTEX_M0P)

#if ((CPUSS_CRYPTO_PRESENT == 1) && (CPUSS_CRYPTO_VU == 1) && (CY_CRYPTO_USER_VU == 1))


/*******************************************************************************
* Function Name: Cy_Crypto_Wait_Vu_ForComplete
****************************************************************************//**
*
* Waits until VU instruction will be completed
*
* This function available for CM0+ core only.
*
*******************************************************************************/
void Cy_Crypto_Core_Vu_WaitForComplete(void)
{
    /* Wait until the VU instruction is complete */
    while(0uL != _FLD2VAL(CRYPTO_STATUS_VU_BUSY, CRYPTO->STATUS))
    {
    }
}

/*******************************************************************************
* Function Name: Cy_Crypto_Core_Vu_StatusRead
****************************************************************************//**
*
* Returns value of the VU status register
*
* This function available for CM0+ core only.
*
*******************************************************************************/
uint16_t Cy_Crypto_Core_Vu_StatusRead(void)
{
    Cy_Crypto_Core_WaitForReady();

    return((uint16_t)CRYPTO->VU_STATUS);
}


#endif /* #if ((CPUSS_CRYPTO_PRESENT == 1) && (CPUSS_CRYPTO_VU == 1) && (CY_CRYPTO_USER_VU == 1)) */

#endif /* #if (CY_CPU_CORTEX_M0P) */

/* [] END OF FILE */
