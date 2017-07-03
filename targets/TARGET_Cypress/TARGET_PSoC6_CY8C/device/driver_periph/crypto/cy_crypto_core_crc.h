/***************************************************************************//**
* \file cy_crypto_core_crc.h
* \version 1.0
*
* \brief
*  This file provides the headers for CRC API
*  in the Crypto driver.
*
********************************************************************************
* \copyright
* Copyright 2016-2017, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions,
* disclaimers, and limitations in the end user license agreement accompanying
* the software package with which this file was provided.
*******************************************************************************/

#if !defined(CY_CRYPTO_CORE_CRC_H)
#define CY_CRYPTO_CORE_CRC_H

#include "crypto/cy_crypto_common.h"
#include "syslib/cy_syslib.h"

#if (CY_CPU_CORTEX_M0P)

#if ((CPUSS_CRYPTO_PRESENT == 1) && (CPUSS_CRYPTO_CRC == 1) && (CY_CRYPTO_USER_CRC == 1))


cy_en_crypto_status_t Cy_Crypto_Core_Crc_Init (cy_stc_crypto_context_crc_t *cryptoCrcContext);

cy_en_crypto_status_t Cy_Crypto_Core_Crc      (cy_stc_crypto_context_crc_t *cryptoCrcContext);


#endif /* #if ((CPUSS_CRYPTO_PRESENT == 1) && (CPUSS_CRYPTO_CRC == 1) && (CY_CRYPTO_USER_CRC == 1)) */

#endif /* #if (CY_CPU_CORTEX_M0P) */

#endif /* #if !defined(CY_CRYPTO_CORE_CRC_H) */



/* [] END OF FILE */
