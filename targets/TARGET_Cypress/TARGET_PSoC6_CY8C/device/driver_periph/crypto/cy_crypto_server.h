/***************************************************************************//**
* \file cy_crypto_server.h
* \version 1.0
*
* \brief
*  This file provides the prototypes for common API
*  in the Crypto driver.
*
********************************************************************************
* \copyright
* Copyright 2016-2017, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions,
* disclaimers, and limitations in the end user license agreement accompanying
* the software package with which this file was provided.
*******************************************************************************/


#if !defined(CY_CRYPTO_SERVER_H)
#define CY_CRYPTO_SERVER_H

#include "crypto/cy_crypto_common.h"
#include "syslib/cy_syslib.h"

#if (CY_CPU_CORTEX_M0P)

#if (CPUSS_CRYPTO_PRESENT == 1)


/**
* \addtogroup group_crypto_functions
* \{
*/

cy_en_crypto_status_t Cy_Crypto_Server_Start(cy_stc_crypto_config_t const *configStruct);

cy_en_crypto_status_t Cy_Crypto_Server_Stop(cy_stc_crypto_config_t const *configStruct);

void Cy_Crypto_Server_Process(void);

void Cy_Crypto_Server_GetDataHandler(void);
void Cy_Crypto_Server_ErrorHandler(void);

/** \} group_crypto_functions */

#endif /* #if (CPUSS_CRYPTO_PRESENT == 1) */

#endif /* #if (CY_CPU_CORTEX_M0P) */

#endif /* #if !defined(CY_CRYPTO_SERVER_H) */

/* [] END OF FILE */
