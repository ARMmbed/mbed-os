/***************************************************************************//**
* \file cy_crypto_server.h
* \version 2.40
*
* \brief
*  This file provides the prototypes for common API
*  in the Crypto driver.
*
********************************************************************************
* Copyright 2016-2020 Cypress Semiconductor Corporation
* SPDX-License-Identifier: Apache-2.0
*
* Licensed under the Apache License, Version 2.0 (the "License");
* you may not use this file except in compliance with the License.
* You may obtain a copy of the License at
*
*    http://www.apache.org/licenses/LICENSE-2.0
*
* Unless required by applicable law or agreed to in writing, software
* distributed under the License is distributed on an "AS IS" BASIS,
* WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
* See the License for the specific language governing permissions and
* limitations under the License.
*******************************************************************************/


#if !defined(CY_CRYPTO_SERVER_H)
#define CY_CRYPTO_SERVER_H

#include "cy_crypto_common.h"

#if defined(CY_IP_MXCRYPTO)

#if defined(__cplusplus)
extern "C" {
#endif

/**
* \addtogroup group_crypto_srv_functions
* \{
*/

/*******************************************************************************
* Function Name: Cy_Crypto_Server_Start_Base
****************************************************************************//**
*
* This function starts the Basic functionality for the Crypto server on the
* server side core, sets up an interrupt for the IPC Crypto channel, sets up an
* interrupt to catch Crypto HW errors.
*
* This function available for Server side only.
*
* \note
* The Crypto server can be run as singleton (not multi-instance) application on
* the one of available cores at the same time.
*
* \param config
* The Crypto configuration structure.
*
* \param context
* The pointer to the \ref cy_stc_crypto_server_context_t structure that stores
* the Crypto server context.
*
* \return
* \ref cy_en_crypto_status_t
*
* \funcusage
* \snippet crypto/snippet/main.c snippet_myCryptoServerStartBase
*
*******************************************************************************/
cy_en_crypto_status_t Cy_Crypto_Server_Start_Base(cy_stc_crypto_config_t const *config,
                                             cy_stc_crypto_server_context_t *context);

/*******************************************************************************
* Function Name: Cy_Crypto_Server_Start_Extra
****************************************************************************//**
*
* This function starts the Extra functionality for the Crypto server on the
* server side core, sets up an interrupt for the IPC Crypto channel, sets up an
* interrupt to catch Crypto HW errors.
*
* This function available for Server side only.
*
* \note
* The Crypto server can be run as singleton (not multi-instance) application on
* the one of available cores at the same time.
*
* \param config
* The Crypto configuration structure.
*
* \param context
* The pointer to the \ref cy_stc_crypto_server_context_t structure that stores
* the Crypto server context.
*
* \return
* \ref cy_en_crypto_status_t
*
* Refer to \ref Cy_Crypto_Server_Start_Base for the function usage example.
*
*******************************************************************************/
cy_en_crypto_status_t Cy_Crypto_Server_Start_Extra(cy_stc_crypto_config_t const *config,
                                             cy_stc_crypto_server_context_t *context);

/*******************************************************************************
* Function Name: Cy_Crypto_Server_Start_Full
****************************************************************************//**
*
* This function starts the Full functionality for the Crypto server on the
* server side core, sets up an interrupt for the IPC Crypto channel, sets up an
* interrupt to catch Crypto HW errors.
*
* This function available for Server side only.
*
* \note
* The Crypto server can be run as singleton (not multi-instance) application on
* the one of available cores at the same time.
*
* \param config
* The Crypto configuration structure.
*
* \param context
* The pointer to the \ref cy_stc_crypto_server_context_t structure that stores
* the Crypto server context.
*
* \return
* \ref cy_en_crypto_status_t
*
* Refer to \ref Cy_Crypto_Server_Start_Base for the function usage example.
*
*******************************************************************************/
cy_en_crypto_status_t Cy_Crypto_Server_Start_Full(cy_stc_crypto_config_t const *config,
                                             cy_stc_crypto_server_context_t *context);

/*******************************************************************************
* Function Name: Cy_Crypto_Server_Stop
****************************************************************************//**
*
* This function stops the Crypto server by disabling the IPC notify interrupt
* and Crypto error interrupt.
*
* This function available for Server side only.
*
* \return
* \ref cy_en_crypto_status_t
*
*******************************************************************************/
cy_en_crypto_status_t Cy_Crypto_Server_Stop(void);

/*******************************************************************************
* Function Name: Cy_Crypto_Server_Process
****************************************************************************//**
*
* This function parses input data received from the Crypto Client,
* runs the appropriate Crypto function and releases the Crypto IPC channel.
*
* This function available for Server side only.
*
* \note
* This function should be used only when user register own GetDataHandler function.
*
*******************************************************************************/
void Cy_Crypto_Server_Process(void);

/*******************************************************************************
* Function Name: Cy_Crypto_Server_GetDataHandler
****************************************************************************//**
*
* This function is a IPC Crypto channel notify interrupt-routine.
* It receives information from the Crypto client,
* runs the process if user not setup own handler.
*
* This function available for Server side only.
*
*******************************************************************************/
void Cy_Crypto_Server_GetDataHandler(void);

/*******************************************************************************
* Function Name: Cy_Crypto_Server_ErrorHandler
****************************************************************************//**
*
* This function is a routine to handle an interrupt caused by the Crypto
* hardware error.
*
* This function available for Server side only.
*
*******************************************************************************/
void Cy_Crypto_Server_ErrorHandler(void);

/** Backward compatibility macro for the Server Start function -
    it starts the Server with Full functionality configuration */
#define Cy_Crypto_Server_Start               Cy_Crypto_Server_Start_Full

/** \} group_crypto_srv_functions */

#if defined(__cplusplus)
}
#endif

#endif /* CY_IP_MXCRYPTO */

#endif /* #if !defined(CY_CRYPTO_SERVER_H) */


/* [] END OF FILE */
