/***************************************************************************//**
* \file cyhal_trng.h
*
* \brief
* Provides a high level interface for interacting with the Cypress True Random
* Number Generator. This interface abstracts out the chip specific details. If
* any chip specific functionality is necessary, or performance is critical the
* low level functions can be used directly.
*
********************************************************************************
* \copyright
* Copyright 2018-2020 Cypress Semiconductor Corporation
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

/**
* \addtogroup group_hal_trng TRNG (True Random Number Generator)
* \ingroup group_hal
* \{
* High level interface for interacting with the true random number generator (TRNG).
*
* This block uses dedicated hardware to efficiently generate truly random numbers.
*/

#pragma once

#include <stdint.h>
#include <stdbool.h>
#include "cy_result.h"
#include "cyhal_hw_types.h"
#include "cyhal_modules.h"

#if defined(__cplusplus)
extern "C" {
#endif

/** An invalid argument was passed to a function. */
#define CYHAL_TRNG_RSLT_ERR_BAD_ARGUMENT (CY_RSLT_CREATE(CY_RSLT_TYPE_ERROR, CYHAL_RSLT_MODULE_TRNG, 0))
/** Hardware error in the crypto block. This will only occur if the Ring oscillators in the TRNG generator are explicitly
 *  disabled during TRNG generation.
 */
#define CYHAL_TRNG_RSLT_ERR_HW (CY_RSLT_CREATE(CY_RSLT_TYPE_ERROR, CYHAL_RSLT_MODULE_TRNG, 1))

/** Initialize the random number generator.
 *
 * @param[out] obj The random number generator object
 * @return The status of the init request
 */
cy_rslt_t cyhal_trng_init(cyhal_trng_t *obj);

/** Release the random number generator.
 *
 * @param[in,out] obj The random number generator object
 */
void cyhal_trng_free(cyhal_trng_t *obj);

/** Generate a random number.
 *
 * @param[in]  obj   The random number generator object
 * @return The random number generated
 */
uint32_t cyhal_trng_generate(const cyhal_trng_t *obj);

#if defined(__cplusplus)
}
#endif

#ifdef CYHAL_TRNG_IMPL_HEADER
#include CYHAL_TRNG_IMPL_HEADER
#endif /* CYHAL_TRNG_IMPL_HEADER */

/** \} group_hal_trng */
