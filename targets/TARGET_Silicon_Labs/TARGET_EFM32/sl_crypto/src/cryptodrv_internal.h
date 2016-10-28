/*
 *  CRYPTO driver internal definitions including CRYPTO preemption and
 *  asynchronous (non-blocking) support.
 *
 *  Copyright (C) 2016, Silicon Labs, http://www.silabs.com
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
#ifndef __SILICON_LABS_CRYPTODRV_INTERNAL_H
#define __SILICON_LABS_CRYPTODRV_INTERNAL_H

#include "em_device.h"

#if ( defined(CRYPTO_COUNT) && (CRYPTO_COUNT > 0) )

#if !defined(MBEDTLS_CONFIG_FILE)
#include "mbedtls/config.h"
#else
#include MBEDTLS_CONFIG_FILE
#endif

#include "cryptodrv.h"
#include "ecode.h"
#include <stdint.h>
#include <string.h>

#ifdef __cplusplus
extern "C" {
#endif

/*******************************************************************************
 ******************************   Functions   **********************************
 ******************************************************************************/

/***************************************************************************//**
 * @brief
 *   Select which CRYPTO device instance to use in CRYPTO context.
 *
 * @param pCryptoContext
 *  Pointer to CRYPTO context.
 *
 * @param devno
 *  CRYPTO device instance number.
 *
 * @return
 *   0 if OK, or -1 if device number is invalid.
 ******************************************************************************/
int cryptodrvSetDeviceInstance(CRYPTODRV_Context_t* pCryptodrvContext,
                               unsigned int         devno);

/***************************************************************************//**
 * @brief
 *   Write a 128 bit value (optionally unaligned) into a crypto register.
 *
 * @note
 *   This function provide a low-level api for writing to the multi-word
 *   registers in the crypto peripheral. Applications should prefer to use
 *   @ref CRYPTO_DataWrite, @ref CRYPTO_DDataWrite or @ref CRYPTO_QDataWrite
 *   for writing to the DATA, DDATA and QDATA registers.
 *
 * @param[in]  reg
 *   Pointer to the crypto register.
 *
 * @param[in]  val
 *   This is a pointer to 4 32 bit integers that contains the 128 bit value
 *   which will be written to the crypto register.
 ******************************************************************************/
__STATIC_INLINE void CRYPTODRV_DataWriteUnaligned(volatile uint32_t * reg,
                                                  const uint8_t * val)
{
  /* Check data is 32bit aligned, if not move to temporary buffer before
     writing.*/
  if ((uint32_t)val & 0x3)
  {
    uint32_t temp[4];
    memcpy(temp, val, 16);
    CRYPTO_DataWrite(reg, temp);
  }
  else
  {
    CRYPTO_DataWrite(reg, (uint32_t*)val);
  }
}

/***************************************************************************//**
 * @brief
 *   Read a 128 bit value from a crypto register into optionally unaligned
 *   buffer.
 *
 * @note
 *   This function provide a low-level api for reading one of the multi-word
 *   registers in the crypto peripheral. Applications should prefer to use
 *   @ref CRYPTO_DataRead, @ref CRYPTO_DDataRead or @ref CRYPTO_QDataRead
 *   for reading the value of the DATA, DDATA and QDATA registers.
 *
 * @param[in]  reg
 *   Pointer to the crypto register.
 *
 * @param[out]  val
 *   This is a pointer to an array that is capable of holding 4 32 bit integers
 *   that will be filled with the 128 bit value from the crypto register.
 ******************************************************************************/
__STATIC_INLINE void CRYPTODRV_DataReadUnaligned(volatile uint32_t * reg,
                                                 uint8_t * val)
{
  /* Check data is 32bit aligned, if not, read into temporary buffer and
     then move to user buffer. */
  if ((uint32_t)val & 0x3)
  {
    uint32_t temp[4];
    CRYPTO_DataRead(reg, temp);
    memcpy(val, temp, 16);
  }
  else
  {
    CRYPTO_DataRead(reg, (uint32_t*)val);
  }
}

/***************************************************************************//**
 * @brief
 *   Read a 256 bit value from a crypto register into optionally unaligned
 *   buffer.
 *
 * @note
 *   This function provide a low-level api for reading one of the multi-word
 *   registers in the crypto peripheral. Applications should prefer to use
 *   @ref CRYPTO_DataRead, @ref CRYPTO_DDataRead or @ref CRYPTO_QDataRead
 *   for reading the value of the DATA, DDATA and QDATA registers.
 *
 * @param[in]  reg
 *   Pointer to the crypto register.
 *
 * @param[out]  val
 *   This is a pointer to a byte array of at least 32 bytes
 *   that will be filled with the 256 bit value from the crypto register.
 ******************************************************************************/
__STATIC_INLINE void CRYPTODRV_DDataReadUnaligned(volatile uint32_t * reg,
                                                  uint8_t * val)
{
  /* Check data is 32bit aligned, if not, read into temporary buffer and
     then move to user buffer. */
  if ((uint32_t)val & 0x3)
  {
    uint32_t temp[8];
    CRYPTO_DDataRead(reg, temp);
    memcpy(val, temp, 32);
  }
  else
  {
    CRYPTO_DDataRead(reg, (uint32_t*)val);
  }
}

/***************************************************************************//**
 * @brief
 *  Check status of CRYPTO operation.
 *
 * @details
 *  This function checks the status of a specific CRYPTO operation.
 *
 * @param pCryptoContext
 *  Pointer to CRYPTO context associated with operation.
 *
 * @return
 *  ECODE_OK if operation completed successfully.
 *  ECODE_CRYPTODRV_RUNNING if CRYPTO is executing operation.
 *  ECODE_CRYPTODRV_ABORTED if operation was aborted.
 */
Ecode_t CRYPTODRV_CheckStatus (CRYPTODRV_Context_t* pCryptoContext);

#if defined(MBEDTLS_INCLUDE_ASYNCH_API)

/***************************************************************************//**
 * @brief
 *  Set asynchronous callback to be called when crypto operations complete.
 *
 * @details
 *  This function sets the asynchronous callback function to be called when
 *  crypto operations complete.
 *
 * @return
 *  N/A
 */
void CRYPTODRV_SetAsynchCallback
(
 CRYPTODRV_Context_t*       pCryptodrvContext,
 CRYPTODRV_AsynchCallback_t asynchCallback,
 void*                      callbackArgument
 );
  
#else /* #if defined(MBEDTLS_INCLUDE_ASYNCH_API) */
  
__STATIC_INLINE void CRYPTODRV_SetAsynchCallback
(
 CRYPTODRV_Context_t*       pCryptodrvContext,
 CRYPTODRV_AsynchCallback_t asynchCallback,
 void*                      callbackArgument
 )
{
  (void) pCryptodrvContext;
  (void) asynchCallback;
  (void) callbackArgument;
}

#endif /* #if defined(MBEDTLS_INCLUDE_ASYNCH_API) */

#if defined MBEDTLS_CRYPTO_DEVICE_PREEMPTION
  
 /***************************************************************************//**
 * @brief
 *  Arbitrate for exclusive access to CRYPTO unit.
 *
 * @details
 *  This function will arbitrate for the ownership of the CRYPTO unit in order
 *  to execute CRYPTO operations. If the ownership is won and there is an
 *  ongoing CRYPTO operation, this function will preempt the ongoing operation
 *  by storing the full CRYPTODRV context inlcuding hardware context
 * (register values) of the CRYPTO unit. When the caller is ready to release
 *  CRYPTO, the caller should call CRYPTODRV_Release in
 *  order to restore the context of the ongoing operation, in order for that to
 *  continue operation. The function call of the ongoing operation may try to
 *  restore and continue after the abort event, but may fail and return
 *  MBEDTLS_ECODE_CRYPTODRV_OPERATION_ABORTED.
 *  This function may be called from an interrupt handler in order to perform
 *  time critical CRYPTO operations.
 *
 * @param pCryptodrvContext
 *  Pointer to CRYPTODRV context which should be used in subsequent calls,
 *  like @ref CRYPTODRV_Release.
 *
 * @return
 *  ECODE_OK if success. Error code if failure.
 *  MBEDTLS_ECODE_CRYPTODRV_BUSY if priority is lower than or equal to running thread.
 */
Ecode_t CRYPTODRV_Arbitrate (CRYPTODRV_Context_t* pCryptodrvContext);

/***************************************************************************//**
 * @brief
 *  Release a CRYPTODRV context and possibly ownership of CRYPTO unit.
 *
 * @details
 *  This function removes a CRYPTODRV context from the CRYPTODRV context list
 *  (which includes current owner and preempted contexts).
 *  If the specified context is the owner of the CRYPTO unit, this function
 *  will release the ownership of the CRYPTO unit. If there is a preempted
 *  context in the context list, then the preempted context is restored
 *  by updating the hardware state of the CRYPTO unit.
 *
 * @param pCryptodrvContext
 *  The CRYPTODRV context to remove from CRYPTODRV context list.
 *
 * @return
 *  ECODE_OK if success. Error code if failure.
 */
Ecode_t CRYPTODRV_Release (CRYPTODRV_Context_t* pCryptodrvContext);

/***************************************************************************//**
 * @brief
 *  Enter critical CRYPTO operation region.
 *
 * @details
 *  This function will enter a critical region by disabling potential harmful
 *  interrupts and thread scheduling, in order to protect a CRYPTO operation
 *  from being interrupted and aborted by higher priority threads/ISRs.
 *  The implementation of this function is OS/runtime platform dependent and
 *  there should be one version per supported OS/runtime platform.
 *
 * @return
 *  ECODE_OK if success. Error code if failure.
 */
Ecode_t CRYPTODRV_EnterCriticalRegion (CRYPTODRV_Context_t* pCryptodrvContext);

 /***************************************************************************//**
 * @brief
 *  Exit critical CRYPTO operation region.
 *
 * @details
 *  This function exits from a critical region by enabling interrupts and
 *  thread scheduling, in order to allow higher priority threads/ISRs to
 *  arbitrate for CRYPTO ownership.
 *  The implementation of this function is OS/runtime platform dependent and
 *  there should be one version per supported OS/runtime platform.
 *
 * @return
 *  ECODE_OK if success. Error code if failure.
 */
Ecode_t CRYPTODRV_ExitCriticalRegion (CRYPTODRV_Context_t* pCryptodrvContext);

#else   /* #if defined MBEDTLS_CRYPTO_DEVICE_PREEMPTION */

__STATIC_INLINE
Ecode_t CRYPTODRV_Arbitrate (CRYPTODRV_Context_t* pCryptodrvContext)
{
  CMU->HFBUSCLKEN0 |= pCryptodrvContext->device->clk;
  return ECODE_OK;
}
__STATIC_INLINE
Ecode_t CRYPTODRV_Release (CRYPTODRV_Context_t* pCryptodrvContext)
{
  CMU->HFBUSCLKEN0 &= ~pCryptodrvContext->device->clk;
  return ECODE_OK;
}
__STATIC_INLINE
Ecode_t CRYPTODRV_EnterCriticalRegion (CRYPTODRV_Context_t* pCryptodrvContext)
{
  (void) pCryptodrvContext;
  return ECODE_OK;
}
__STATIC_INLINE
Ecode_t CRYPTODRV_ExitCriticalRegion (CRYPTODRV_Context_t* pCryptodrvContext)
{
  (void) pCryptodrvContext;
  return ECODE_OK;
}

#endif   /* #if defined MBEDTLS_CRYPTO_DEVICE_PREEMPTION */

#ifdef __cplusplus
}
#endif

#endif /* #if ( defined(CRYPTO_COUNT) && (CRYPTO_COUNT > 0) ) || \
          ( defined(AES_COUNT) && (AES_COUNT > 0) ) */

#endif /* #ifndef __SILICON_LABS_CRYPTODRV_INTERNAL_H */
