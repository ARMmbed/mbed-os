/***************************************************************************//**
* \file cyhal_crypto_common.h
*
* Description:
* This file provides common defines and addresses required by drivers using the
* Crypto block.
*
********************************************************************************
* \copyright
* Copyright 2018-2019 Cypress Semiconductor Corporation
* SPDX-License-Identifier: Apache-2.0
*
* Licensed under the Apache License, Version 2.0 (the "License");
* you may not use this file except in compliance with the License.
* You may obtain a copy of the License at
*
*     http://www.apache.org/licenses/LICENSE-2.0
*
* Unless required by applicable law or agreed to in writing, software
* distributed under the License is distributed on an "AS IS" BASIS,
* WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
* See the License for the specific language governing permissions and
* limitations under the License.
*******************************************************************************/

#pragma once

#include "cy_device.h"
#include "cy_pdl.h"
#include "cy_result.h"
#include "cyhal_hw_types.h"

#if defined(CY_IP_MXCRYPTO)

#if defined(__cplusplus)
extern "C" {
#endif

/** Block count for CRYPTO blocks */
#define CYHAL_CRYPTO_INST_COUNT      CY_IP_MXCRYPTO_INSTANCES

/** The start address of the CRYPTO blocks */
extern CRYPTO_Type* cyhal_CRYPTO_BASE_ADDRESSES[CYHAL_CRYPTO_INST_COUNT];

/** Reserve the Crypto block and enable it.
 *
 * @param[out]  base Base address to the Crypto block.
 * @param[out]  obj resource inst for the function (eg. CRC, TRNG) in the Crypto block.
 * @return The status of the reserve request.
 */
cy_rslt_t cyhal_crypto_reserve(CRYPTO_Type** base, cyhal_resource_inst_t *resource);

/** Free the Crypto block and disable it.
 *
 * @param[in]  base Base address to the Crypto block.
 * @param[in]  obj resource inst for the funtion in Crypto block.
 */
void cyhal_crypto_free(CRYPTO_Type* base, const cyhal_resource_inst_t *resource);

#if defined(__cplusplus)
}
#endif

#endif /* defined(CY_IP_MXCRYPTO) */
