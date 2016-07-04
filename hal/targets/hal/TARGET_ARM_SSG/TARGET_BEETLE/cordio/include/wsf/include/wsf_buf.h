/*************************************************************************************************/
/*!
 *  \file   wsf_buf.h
 *
 *  \brief  Buffer pool service.
 *
 *          $Date: 2015-09-05 12:01:07 -0400 (Sat, 05 Sep 2015) $
 *          $Revision: 3793 $
 *
 * Copyright (c) 2009-2016, ARM Limited, All Rights Reserved
 * SPDX-License-Identifier: LicenseRef-PBL
 *
 * This file and the related binary are licensed under the
 * Permissive Binary License, Version 1.0 (the "License");
 * you may not use these files except in compliance with the License.
 *
 * You may obtain a copy of the License here:
 * LICENSE-permissive-binary-license-1.0.txt and at
 * https://www.mbed.com/licenses/PBL-1.0
 *
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
/*************************************************************************************************/
#ifndef WSF_BUF_H
#define WSF_BUF_H

#ifdef __cplusplus
extern "C" {
#endif

/**************************************************************************************************
  Macros
**************************************************************************************************/

/*! Length of the buffer statistics array */
#define WSF_BUF_STATS_MAX_LEN       128

/**************************************************************************************************
  Data Types
**************************************************************************************************/

/*! Buffer pool descriptor structure */
typedef struct
{
  uint16_t   len;                  /*! length of buffers in pool */
  uint8_t    num;                  /*! number of buffers in pool */
} wsfBufPoolDesc_t;

/*! Pool statistics */
typedef struct
{
  uint8_t bufSize;                  /*!< Pool buffer size. */
  uint8_t numBuf;                   /*!< Total number of buffers. */
  uint8_t numAlloc;                 /*!< Number of outstanding allocations. */
  uint8_t maxAlloc;                 /*!< High allocation watermark. */
} WsfBufPoolStat_t;


/**************************************************************************************************
  Function Declarations
**************************************************************************************************/

/*************************************************************************************************/
/*!
 *  \fn     WsfBufInit
 *
 *  \brief  Initialize the buffer pool service.  This function should only be called once
 *          upon system initialization.
 *
 *  \param  bufMemLen Length in bytes of memory pointed to by pBufMem.
 *  \param  pBufMem   Memory in which to store the pools used by the buffer pool service.
 *  \param  numPools  Number of buffer pools.
 *  \param  pDesc     Array of buffer pool descriptors, one for each pool.
 *
 *  \return Amount of pBufMem used or 0 for failures.
 */
/*************************************************************************************************/
uint16_t WsfBufInit(uint16_t bufMemLen, uint8_t *pBufMem, uint8_t numPools, wsfBufPoolDesc_t *pDesc);

/*************************************************************************************************/
/*!
 *  \fn     WsfBufAlloc
 *
 *  \brief  Allocate a buffer.
 *
 *  \param  len     Length of buffer to allocate.
 *
 *  \return Pointer to allocated buffer or NULL if allocation fails.
 */
/*************************************************************************************************/
void *WsfBufAlloc(uint16_t len);

/*************************************************************************************************/
/*!
 *  \fn     WsfBufFree
 *
 *  \brief  Free a buffer.
 *
 *  \param  pBuf    Buffer to free.
 *
 *  \return None.
 */
/*************************************************************************************************/
void WsfBufFree(void *pBuf);

/*************************************************************************************************/
/*!
 *  \fn     WsfBufGetMaxAlloc
 *
 *  \brief  Diagnostic function to get maximum allocated buffers from a pool.
 *
 *  \param  pool    Buffer pool number.
 *
 *  \return Number of allocated buffers.
 */
/*************************************************************************************************/
uint8_t WsfBufGetMaxAlloc(uint8_t pool);

/*************************************************************************************************/
/*!
 *  \fn     WsfBufGetNumAlloc
 *
 *  \brief  Diagnostic function to get the number of currently allocated buffers in a pool.
 *
 *  \param  pool    Buffer pool number.
 *
 *  \return Number of allocated buffers.
 */
/*************************************************************************************************/
uint8_t WsfBufGetNumAlloc(uint8_t pool);

/*************************************************************************************************/
/*!
 *  \fn     WsfBufGetAllocStats
 *
 *  \brief  Diagnostic function to get the buffer allocation statistics.
 *
 *  \return Buffer allocation statistics array.
 */
/*************************************************************************************************/
uint8_t *WsfBufGetAllocStats(void);

/*************************************************************************************************/
/*!
 *  \fn     WsfBufGetPoolStats
 *
 *  \brief  Get statistics for each pool.
 *
 *  \param  pStat   Buffer to store the statistics.
 *  \param  numPool Number of pool elements.
 *
 *  \return Pool statistics.
 */
/*************************************************************************************************/
void WsfBufGetPoolStats(WsfBufPoolStat_t *pStat, uint8_t numPool);


#ifdef __cplusplus
};
#endif

#endif /* WSF_BUF_H */
