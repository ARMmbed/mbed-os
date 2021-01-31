/***************************************************************************//**
* \file cy_crypto_core_prng_v2.h
* \version 2.40
*
* \brief
*  This file provides provides constant and parameters for the API of the PRNG
*  in the Crypto block driver.
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


#if !defined(CY_CRYPTO_CORE_PRNG_V2_H)
#define CY_CRYPTO_CORE_PRNG_V2_H

#include "cy_crypto_common.h"

#if defined(CY_IP_MXCRYPTO)

#if defined(__cplusplus)
extern "C" {
#endif

#if (CPUSS_CRYPTO_PR == 1)

cy_en_crypto_status_t Cy_Crypto_Core_V2_Prng_Init(CRYPTO_Type *base,
                                                  uint32_t lfsr32InitState,
                                                  uint32_t lfsr31InitState,
                                                  uint32_t lfsr29InitState);

cy_en_crypto_status_t Cy_Crypto_Core_V2_Prng(CRYPTO_Type *base,
                                             uint32_t max,
                                             uint32_t *randomNum);

#endif /* #if (CPUSS_CRYPTO_PR == 1) */

#if defined(__cplusplus)
}
#endif

#endif /* CY_IP_MXCRYPTO */

#endif /* #if !defined(CY_CRYPTO_CORE_PRNG_V2_H) */


/* [] END OF FILE */
