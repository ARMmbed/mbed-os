/***************************************************************************//**
* \file cyhal_crc.h
*
* \brief
* Provides a high level interface for interacting with the Cypress CRC accelerator.
* This interface abstracts out the chip specific details. If any chip specific
* functionality is necessary, or performance is critical the low level functions
* can be used directly.
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
* \addtogroup group_hal_crc CRC (Cyclic Redundancy Check)
* \ingroup group_hal
* \{
* High level interface for interacting with the cyclic redundancy check (CRC), which provides hardware
* accelerated CRC computations.
* The CRC APIs are structured to enable usage in situations where the entire input data
* set is not available in memory at one time. Therefore, each conversion consists of three steps:
* * A single call to cyhal_crc_start, to initialize data structures for this computation
* * One or more calls to cyhal_crc_compute, to provide chunks of data.
* * A single call to cyhal_crc_finish, to finalize the computation and retrieve the result.
*
* Many of the algorithm parameters can be customized; see crc_algorithm_t for more details.
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

/** Invalid argument */
#define CYHAL_CRC_RSLT_ERR_BAD_ARGUMENT (CY_RSLT_CREATE(CY_RSLT_TYPE_ERROR, CYHAL_RSLT_MODULE_CRC, 0))

/** @brief CRC algorithm parameters */
typedef struct
{
    uint32_t width; //!< Bit width of the CRC
    uint32_t polynomial; //!< The polynomial to use
    uint32_t lfsrInitState; //!< Initial state of the LFSR
    uint32_t dataXor; //!< Byte mask to xor with the data
    uint32_t remXor; //!< Mask to xor with the remainder.
    /**
     * The order in which data should be processed.
     * If 0, data is processed MSB first.
     * If 1, data is processed LSB first.
     */
    bool dataReverse;
    bool remReverse; //!< If 1, the remainder is reversed. If 0, it is not.
} crc_algorithm_t;

/** Initialize the CRC generator. This function reserves the CRYPTO block for CRC calculations.
 *
 * @param[out] obj  The CRC generator object
 * @return          The status of the init request.
 */
cy_rslt_t cyhal_crc_init(cyhal_crc_t *obj);

/** Release the CRC generator.
 *
 * @param[in,out] obj The CRC generator object
 */
void cyhal_crc_free(cyhal_crc_t *obj);

/** Initializes a CRC calculation.
 *
 * @param[in,out] obj       The CRC generator object
 * @param[in] algorithm     The CRC algorithm to use for computations
 * @return The status of the compute request
 */
cy_rslt_t cyhal_crc_start(cyhal_crc_t *obj, const crc_algorithm_t *algorithm);

/** Computes the CRC for the given data. This function can be called multiple times to
 * provide addtional data. This CRC generator will compute the CRC for including all data
 * that was provided during previous calls.
 *
 * @param[in] obj    The CRC generator object
 * @param[in] data   The data to compute a CRC over
 * @param[in] length The number of bytes of data to process
 * @return The status of the compute request
 */
cy_rslt_t cyhal_crc_compute(const cyhal_crc_t *obj, const uint8_t *data, size_t length);

/** Provides final result for a CRC calculation. This will compute the CRC for all data that
 * was provided when during the diffrent calls to cyhal_crc_compute.
 *
 * @param[in]  obj The CRC generator object
 * @param[out] crc The computed CRC
 * @return The status of the compute request
 */
cy_rslt_t cyhal_crc_finish(const cyhal_crc_t *obj, uint32_t *crc);

#if defined(__cplusplus)
}
#endif

#ifdef CYHAL_CRC_IMPL_HEADER
#include CYHAL_CRC_IMPL_HEADER
#endif /* CYHAL_CRC_IMPL_HEADER */

/** \} group_hal_crc */
