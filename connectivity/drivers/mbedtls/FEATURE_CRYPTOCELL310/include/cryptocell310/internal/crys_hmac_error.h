/**************************************************************************************
* Copyright (c) 2016-2017, ARM Limited or its affiliates. All rights reserved         *
*                                                                                     *
* This file and the related binary are licensed under the following license:          *
*                                                                                     *
* ARM Object Code and Header Files License, v1.0 Redistribution.                      *
*                                                                                     *
* Redistribution and use of object code, header files, and documentation, without     *
* modification, are permitted provided that the following conditions are met:         *
*                                                                                     *
* 1) Redistributions must reproduce the above copyright notice and the                *
*    following disclaimer in the documentation and/or other materials                 *
*    provided with the distribution.                                                  *
*                                                                                     *
* 2) Unless to the extent explicitly permitted by law, no reverse                     *
*    engineering, decompilation, or disassembly of is permitted.                      *
*                                                                                     *
* 3) Redistribution and use is permitted solely for the purpose of                    *
*    developing or executing applications that are targeted for use                   *
*    on an ARM-based product.                                                         *
*                                                                                     *
* DISCLAIMER. THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND                  *
* CONTRIBUTORS "AS IS." ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT             *
* NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY, NON-INFRINGEMENT,        *
* AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE          *
* COPYRIGHT HOLDERS OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,   *
* SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED            *
* TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR              *
* PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF              *
* LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING                *
* NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS                  *
* SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.                        *
**************************************************************************************/



#ifndef CRYS_HMAC_ERROR_H
#define CRYS_HMAC_ERROR_H

#include "crys_error.h"

#ifdef __cplusplus
extern "C"
{
#endif

/*! @file
@brief This module contains the definitions of the CRYS HMAC errors.
@defgroup crys_hmac_error CryptoCell HMAC specific errors
@{
@ingroup crys_hmac
*/



/************************ Defines ******************************/

/*! The CRYS HMAC module errors */
/*! Illegal context pointer. */
#define CRYS_HMAC_INVALID_USER_CONTEXT_POINTER_ERROR     (CRYS_HMAC_MODULE_ERROR_BASE + 0x0UL)
/*! Illegal operation mode. */
#define CRYS_HMAC_ILLEGAL_OPERATION_MODE_ERROR           (CRYS_HMAC_MODULE_ERROR_BASE + 0x1UL)
/*! Context is corrupted. */
#define CRYS_HMAC_USER_CONTEXT_CORRUPTED_ERROR           (CRYS_HMAC_MODULE_ERROR_BASE + 0x2UL)
/*! Illegal data in pointer. */
#define CRYS_HMAC_DATA_IN_POINTER_INVALID_ERROR          (CRYS_HMAC_MODULE_ERROR_BASE + 0x3UL)
/*! Illegal data in size. */
#define CRYS_HMAC_DATA_SIZE_ILLEGAL                      (CRYS_HMAC_MODULE_ERROR_BASE + 0x4UL)
/*! Illegal result buffer pointer. */
#define CRYS_HMAC_INVALID_RESULT_BUFFER_POINTER_ERROR    (CRYS_HMAC_MODULE_ERROR_BASE + 0x5UL)
/*! Illegal key buffer pointer. */
#define CRYS_HMAC_INVALID_KEY_POINTER_ERROR              (CRYS_HMAC_MODULE_ERROR_BASE + 0x6UL)
/*! Illegal key size. */
#define CRYS_HMAC_UNVALID_KEY_SIZE_ERROR                 (CRYS_HMAC_MODULE_ERROR_BASE + 0x7UL)
/*! Last block was already processed (may happen if previous block was not a multiple of block size). */
#define CRYS_HMAC_LAST_BLOCK_ALREADY_PROCESSED_ERROR	 (CRYS_HMAC_MODULE_ERROR_BASE + 0xBUL)
/*! Illegal parameters. */
#define CRYS_HMAC_ILLEGAL_PARAMS_ERROR 			 (CRYS_HMAC_MODULE_ERROR_BASE + 0xCUL)
/*! Illegal context size. */
#define CRYS_HMAC_CTX_SIZES_ERROR   	                 (CRYS_HMAC_MODULE_ERROR_BASE + 0xEUL)
/*! HMAC is not supported. */
#define CRYS_HMAC_IS_NOT_SUPPORTED                       (CRYS_HMAC_MODULE_ERROR_BASE + 0xFUL)



/************************ Enums ********************************/


/************************ Typedefs  ****************************/


/************************ Structs  ******************************/


/************************ Public Variables **********************/


/************************ Public Functions **********************/

#ifdef __cplusplus
}
#endif
/**
@}
 */
#endif


