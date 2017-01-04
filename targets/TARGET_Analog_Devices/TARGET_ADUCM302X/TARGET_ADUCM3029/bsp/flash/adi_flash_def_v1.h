/*!
 *****************************************************************************
 @file:    adi_flash_def_v1.h
 @brief:   Flash Driver Implementations for ADuCM302x
 @version: $Revision: 28841 $
 @date:    $Date: 2014-11-28 06:48:03 +0000 (Fri, 28 Nov 2014) $
 -----------------------------------------------------------------------------
Copyright (c) 2012-2014 Analog Devices, Inc.

All rights reserved.

Redistribution and use in source and binary forms, with or without modification,
are permitted provided that the following conditions are met:
  - Redistributions of source code must retain the above copyright notice,
    this list of conditions and the following disclaimer.
  - Redistributions in binary form must reproduce the above copyright notice,
    this list of conditions and the following disclaimer in the documentation
    and/or other materials provided with the distribution.
  - Modified versions of the software must be conspicuously marked as such.
  - This software is licensed solely and exclusively for use with processors
    manufactured by or for Analog Devices, Inc.
  - This software may not be combined or merged with other code in any manner
    that would cause the software to become subject to terms and conditions
    which differ from those listed here.
  - Neither the name of Analog Devices, Inc. nor the names of its
    contributors may be used to endorse or promote products derived
    from this software without specific prior written permission.
  - The use of this software may or may not infringe the patent rights of one
    or more patent holders.  This license does not release you from the
    requirement that you obtain separate licenses from these patent holders
    to use this software.
THIS SOFTWARE IS PROVIDED BY ANALOG DEVICES, INC. AND CONTRIBUTORS "AS IS"
AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, NON-
INFRINGEMENT, TITLE, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
DISCLAIMED. IN NO EVENT SHALL ANALOG DEVICES, INC. OR CONTRIBUTORS BE LIABLE
FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, PUNITIVE OR
CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, DAMAGES ARISING OUT OF
CLAIMS OF INTELLECTUAL PROPERTY RIGHTS INFRINGEMENT; PROCUREMENT OF
SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
POSSIBILITY OF SUCH DAMAGE.

*****************************************************************************/

/*! \cond PRIVATE */

#ifndef __ADI_FLASH_DEF_H__
#define __ADI_FLASH_DEF_H__

#include <services/dma/adi_dma.h>
#include <adi_flash_config.h>

#if defined(__ECC__)
#define ALIGN4  _Pragma("align(4)")
#elif defined(__ICCARM__)
#define ALIGN4  _Pragma("pack(4)")
#elif defined (__GNUC__)
#define ALIGN4  _Pragma("pack(4)")
#elif defined (__CC_ARM)
#define ALIGN4 __attribute__((aligned (4)))
#endif

/* Maximum number of internal buffers */
#define NUM_BUFFERS         (2)

/* Flash Internal Buffer Structure */
typedef struct _FLASH_BUFFER {
    struct _FLASH_BUFFER*  pNext;
    uint32_t*              pBuffer;
    uint32_t*              pCurBuffPos;
    uint32_t               nStartAddress;
    uint32_t               nTotalBytes;
    uint32_t               nRemainingBytes;
} FLASH_BUFFER;

/* Declaration flash device structure */
struct _FLASH_DEVICE;

/* Manage FIFO function */
typedef ADI_FEE_RESULT (*ManageFIFO)(
                          struct _FLASH_DEVICE* const drv,
                          bool_t                const bStart,
                          bool_t                const bInterruptRegion
                          );


/* Device structure for the device */
typedef struct _FLASH_DEVICE {
    ADI_FLCC_TypeDef    *pReg;

    bool_t        bTransferInProgress;
#if (ADI_FEE_CFG_ENABLE_DMA_SUPPORT == 1)
    bool_t        bUseDmaForTransfer;
#endif

    ADI_CALLBACK  pfCallback;
    void*         pCBParam;
    uint32_t      FlashStatusCopy;
    uint32_t      WaitStatus;
    PEND_VAR_DECLR
    ManageFIFO    pfManageFIFO;

    FLASH_BUFFER* pFreeBuffers;
    FLASH_BUFFER* pPendingBuffers;
    FLASH_BUFFER* pCompletedBuffers;

    FLASH_BUFFER* pActiveBuffer;

#if (ADI_FEE_CFG_ENABLE_DMA_SUPPORT == 1)
    ADI_DMA_CHANNEL_HANDLE  hDmaHandle;
    ALIGN4
    uint8_t        DMAMemory[ADI_DMA_MEMORY_SIZE];
#endif

    FLASH_BUFFER  Buffers[NUM_BUFFERS];

} FLASH_DEVICE;

/* Device info structure */
typedef struct _FLASH_INFO {
   ADI_FLCC_TypeDef   *pReg;         /* Pointer to the register base */
   FLASH_DEVICE        *pDevice;      /* Pointer the device memory (supplied by the application) */

} FLASH_INFO;

#endif /* __ADI_FLASH_DEF_H__ */

/*! \endcond */
