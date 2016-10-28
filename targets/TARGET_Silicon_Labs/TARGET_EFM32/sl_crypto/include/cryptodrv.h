/*
 * \file cryptodrv.h
 *
 * \brief CRYPTO driver definitions including CRYPTO preemption and
 * asynchronous (non-blocking) support.
 *
 *  Copyright (C) 2016 Silicon Labs, http://www.silabs.com
 *  SPDX-License-Identifier: Apache-2.0
 *
 *  Licensed under the Apache License, Version 2.0 (the "License"); you may
 *  not use this file except in compliance with the License.
 *  You may obtain a copy of the License at
 *
 *  http://www.apache.org/licenses/LICENSE-2.0
 *
 *  Unless required by applicable law or agreed to in writing, software
 *  distributed under the License is distributed on an "AS IS" BASIS, WITHOUT
 *  WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *  See the License for the specific language governing permissions and
 *  limitations under the License.
 */
#ifndef __SILICON_LABS_CRYPTODRV_H
#define __SILICON_LABS_CRYPTODRV_H

#if !defined(MBEDTLS_CONFIG_FILE)
#include "config.h"
#else
#include MBEDTLS_CONFIG_FILE
#endif

#include "em_device.h"

#if ( defined(CRYPTO_COUNT) && (CRYPTO_COUNT > 0) )

#include "mbedtls_ecode.h"
#include "em_crypto.h"
#if defined( MBEDTLS_INCLUDE_IO_MODE_DMA )
#include "dmadrv.h"
#endif
#include <stdint.h>
#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif

/*******************************************************************************
 ******************************* ERROR CODES  **********************************
 ******************************************************************************/

#define MBEDTLS_ECODE_CRYPTODRV_OPERATION_ABORTED (MBEDTLS_ECODE_CRYPTODRV_BASE | 0x1) /**< Operation was aborted. Possibly by higher priority task. The user will need to retry the operation in order to complete.  */
#define MBEDTLS_ECODE_CRYPTODRV_BUSY              (MBEDTLS_ECODE_CRYPTODRV_BASE | 0x2) /**< The CRYPTO module is busy. */

/*******************************************************************************
 *******************************   TYPEDEFS   **********************************
 ******************************************************************************/

#if defined(MBEDTLS_CRYPTO_DEVICE_PREEMPTION)

/** Context of CRYPTO hardware module. */
typedef struct
{
  uint32_t CTRL;                 /**< Control Register  */
  uint32_t WAC;                  /**< Wide Arithmetic Configuration  */
  uint32_t SEQCTRL;              /**< Sequence Control  */
  uint32_t SEQCTRLB;             /**< Sequence Control B  */
  uint32_t IEN;                  /**< Interrupt Enable Register  */
  uint32_t SEQ[5];               /**< Instruction Sequence registers */
  CRYPTO_DData_TypeDef DDATA[5]; /**< DDATA registers. Covers all data registers
				    of CRYPTO, including DATA(128 bit), DDATA (256bit),
				    QDATA (512bit) registers. */
} CRYPTO_Context_t;
  
#endif /* #if defined(MBEDTLS_CRYPTO_DEVICE_PREEMPTION) */

/***************************************************************************//**
 * @brief
 *  CRYPTODRV asynchronous (non-blocking) operation completion callback function.
 *
 * @details
 *  The callback function is called when an asynchronous (non-blocking)
 *  CRYPTO operation has completed.
 *
 * @param[in] result
 *  The result of the CRYPTO operation.
 *
 * @param[in] userArgument
 *  Optional user defined argument supplied when starting the asynchronous
 *  CRYPTO operation.
 ******************************************************************************/
typedef void (*CRYPTODRV_AsynchCallback_t)(void* userArgument);

/** CRYPTO device structure. */
typedef struct
{
  CRYPTO_TypeDef*              crypto;         /**< CRYPTO hw instance */
  IRQn_Type                    irqn;           /**< CRYPTO irq number */
  uint32_t                     clk;            /**< CRYPTO clock */
#if defined( MBEDTLS_CRYPTO_DEVICE_PREEMPTION )
  void*                        pCryptoOwner;   /**< Pointer to pointer of
                                                  CRYPTO unit owner */
  uint32_t*                    pNvicIser;      /**< Pointer to storage
                                                  of NVIC ISER values. */
#endif
#if defined( MBEDTLS_INCLUDE_IO_MODE_DMA )
  DMADRV_PeripheralSignal_t    dmaReqSigChIn; /**< DMA req signal channel in */
  DMADRV_PeripheralSignal_t    dmaReqSigChOut;/**< DMA req signal channel out */
#endif
} CRYPTO_Device_t;

/** CRYPTODRV operation context. */
typedef struct
{
  const CRYPTO_Device_t*     device;         /**< CRYPTO hw instance */
  
#if defined( MBEDTLS_CRYPTO_DEVICE_PREEMPTION )
  
  CRYPTO_Context_t           cryptoContext;  /**< CRYPTO hw context */
  bool                       aborted;        /**< 'true' if an ongoing CRYPTO
                                                instruction sequence was
                                                aborted.*/
  unsigned long              threadPriority; /**< Priority of thread/context */
  void*                      threadId;       /**< Pointer to thread identifier*/

  /* Double linked-list for CRYPTO context preemption. */
  void*                      pContextPreempted; /* Context preempted by _this_
                                                   context. */
  void*                      pContextPreemptor; /* Context of preemptor
                                                   of _this_ context. */
#if defined(MBEDTLS_INCLUDE_ASYNCH_API)
  
  CRYPTODRV_AsynchCallback_t asynchCallback;    /**< Asynch callback */
  void*                      asynchCallbackArgument; /**< Parameter for
                                                        asynch callback */
  
#endif /* #if defined(MBEDTLS_INCLUDE_ASYNCH_API) */
  
#endif /* #if defined(MBEDTLS_CRYPTO_DEVICE_PREEMPTION) */
  
} CRYPTODRV_Context_t;

#ifdef __cplusplus
}
#endif

#endif /* #if ( defined(CRYPTO_COUNT) && (CRYPTO_COUNT > 0) ) || \
          ( defined(AES_COUNT) && (AES_COUNT > 0) ) */

#endif /* #ifndef __SILICON_LABS_CRYPTODRV_H */
