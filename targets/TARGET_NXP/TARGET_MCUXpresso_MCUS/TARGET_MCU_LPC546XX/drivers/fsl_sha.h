/*
 * The Clear BSD License
 * Copyright (c) 2016, Freescale Semiconductor, Inc.
 * Copyright 2016-2017 NXP
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted (subject to the limitations in the disclaimer below) provided
 * that the following conditions are met:
 *
 * o Redistributions of source code must retain the above copyright notice, this list
 *   of conditions and the following disclaimer.
 *
 * o Redistributions in binary form must reproduce the above copyright notice, this
 *   list of conditions and the following disclaimer in the documentation and/or
 *   other materials provided with the distribution.
 *
 * o Neither the name of the copyright holder nor the names of its
 *   contributors may be used to endorse or promote products derived from this
 *   software without specific prior written permission.
 *
 * NO EXPRESS OR IMPLIED LICENSES TO ANY PARTY'S PATENT RIGHTS ARE GRANTED BY THIS LICENSE.
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR
 * ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
 * ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */
#ifndef _FSL_SHA_H_
#define _FSL_SHA_H_

#include "fsl_common.h"

/*!
 * @addtogroup sha
 * @{
 */

/*! @file */

/*******************************************************************************
 * Definitions
 *******************************************************************************/

/*! @name Driver version */
/*@{*/
/*! @brief Defines LPC SHA driver version 2.0.0.
 *
 * Change log:
 * - Version 2.0.0
 *   - initial version
 */
#define FSL_SHA_DRIVER_VERSION (MAKE_VERSION(2, 0, 0))
/*@}*/

/*! Supported cryptographic block cipher functions for HASH creation */
typedef enum _sha_algo_t
{
    kSHA_Sha1,   /*!< SHA_1 */
    kSHA_Sha256, /*!< SHA_256  */
} sha_algo_t;

/*! @brief SHA Context size. */
#define SHA_CTX_SIZE 20

/*! @brief Storage type used to save hash context. */
typedef struct _sha_ctx_t
{
    uint32_t x[SHA_CTX_SIZE];
} sha_ctx_t;

/*******************************************************************************
 * API
 *******************************************************************************/

#if defined(__cplusplus)
extern "C" {
#endif /* __cplusplus */

/*!
 * @name SHA Functional Operation
 * @{
 */

/*!
 * @addtogroup sha_algorithm_level_api
 * @{
 */
/*!
* @brief Initialize HASH context
*
* This function initializes new hash context.
*
* @param base SHA peripheral base address
* @param[out] ctx Output hash context
* @param algo Underlaying algorithm to use for hash computation. Either SHA-1 or SHA-256.
* @return Status of initialization
*/
status_t SHA_Init(SHA_Type *base, sha_ctx_t *ctx, sha_algo_t algo);

/*!
 * @brief Add data to current HASH
 *
 * Add data to current HASH. This can be called repeatedly with an arbitrary amount of data to be
 * hashed.
 *
 * @param base SHA peripheral base address
 * @param[in,out] ctx HASH context
 * @param message Input message
 * @param messageSize Size of input message in bytes
 * @return Status of the hash update operation
 */
status_t SHA_Update(SHA_Type *base, sha_ctx_t *ctx, const uint8_t *message, size_t messageSize);

/*!
 * @brief Finalize hashing
 *
 * Outputs the final hash and erases the context. SHA-1 or SHA-256 padding bits are automatically added by this
 * function.
 *
 * @param base SHA peripheral base address
 * @param[in,out] ctx HASH context
 * @param[out] output Output hash data
 * @param[in,out] outputSize On input, determines the size of bytes of the output array. On output, tells how many bytes
 * have been written to output.
 * @return Status of the hash finish operation
 */
status_t SHA_Finish(SHA_Type *base, sha_ctx_t *ctx, uint8_t *output, size_t *outputSize);
/*!
 *@}
 */ /* sha_algorithm_level_api */

#if defined(__cplusplus)
}
#endif /* __cplusplus */

/*! @}*/
/*! @}*/ /* end of group sha */

#endif /* _FSL_SHA_H_ */
