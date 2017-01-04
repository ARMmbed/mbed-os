/*!
 *****************************************************************************
 @file:    adi_crypto_def_v1.h
 @brief:   Crypto Device Driver definitions for ADuCM302x processor
 @version: $Revision: 35030 $
 @date:    $Date: 2016-07-05 05:49:46 +0100 (Tue, 05 Jul 2016) $
 -----------------------------------------------------------------------------
Copyright (c) 2012-2015 Analog Devices, Inc.

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
#ifndef __ADI_CRYPTO_DEF_H__
#define __ADI_CRYPTO_DEF_H__

/*! \cond PRIVATE */

#include <services/dma/adi_dma.h>
#include "adi_crypto_config.h"

#if defined(__ECC__)
#define ALIGN4  _Pragma("align(4)")
#elif defined(__ICCARM__)
#define ALIGN4  _Pragma("pack(4)")
#elif defined (__GNUC__)
#define ALIGN4  _Pragma("pack(4)")
#elif defined (__CC_ARM)
#define ALIGN4 __attribute__((aligned (4)))
#endif

/* Support Check MACROS */
#define CRYPTO_SUPPORT_REQ_KEY_PROCESSING  (                                            \
                                               (ADI_CRYPTO_ENABLE_ECB_SUPPORT == 1)     \
                                            || (ADI_CRYPTO_ENABLE_CTR_SUPPORT == 1)     \
                                            || (ADI_CRYPTO_ENABLE_CBC_SUPPORT == 1)     \
                                            || (ADI_CRYPTO_ENABLE_CCM_SUPPORT == 1)     \
                                            || (ADI_CRYPTO_ENABLE_CMAC_SUPPORT == 1)    \
                                            )

#define CRYPTO_SUPPORT_MODE_CCM_ONLY       (                                             \
                                               (ADI_CRYPTO_ENABLE_ECB_SUPPORT  != 1)     \
                                            && (ADI_CRYPTO_ENABLE_CTR_SUPPORT  != 1)     \
                                            && (ADI_CRYPTO_ENABLE_CBC_SUPPORT  != 1)     \
                                            && (ADI_CRYPTO_ENABLE_CCM_SUPPORT  == 1)     \
                                            && (ADI_CRYPTO_ENABLE_CMAC_SUPPORT != 1)     \
                                            && (ADI_CRYPTO_ENABLE_SHA_SUPPORT  != 1)     \
                                            )

#define CRYPTO_SUPPORT_MODE_ANY_NON_CCM    (                                             \
                                               (ADI_CRYPTO_ENABLE_ECB_SUPPORT  == 1)     \
                                            || (ADI_CRYPTO_ENABLE_CTR_SUPPORT  == 1)     \
                                            || (ADI_CRYPTO_ENABLE_CBC_SUPPORT  == 1)     \
                                            || (ADI_CRYPTO_ENABLE_CMAC_SUPPORT == 1)     \
                                            || (ADI_CRYPTO_ENABLE_SHA_SUPPORT  == 1)     \
                                            )

#define CRYPTO_SUPPORT_MODE_ANY_NON_SHA    (                                             \
                                               (ADI_CRYPTO_ENABLE_ECB_SUPPORT  == 1)     \
                                            || (ADI_CRYPTO_ENABLE_CTR_SUPPORT  == 1)     \
                                            || (ADI_CRYPTO_ENABLE_CBC_SUPPORT  == 1)     \
                                            || (ADI_CRYPTO_ENABLE_CMAC_SUPPORT == 1)     \
                                            || (ADI_CRYPTO_ENABLE_CCM_SUPPORT  == 1)     \
                                            )





/* Max number of buffers */
#define NUM_BUFFERS                 (2)

#define FIFO_WORD_SIZE_IN_BITS      (32u)
#define FIFO_WORD_SIZE_IN_BYTES     (FIFO_WORD_SIZE_IN_BITS/8u)

#define BLK_SIZE_IN_BITS            (128u)
#define BLK_SIZE_IN_BYTES           (BLK_SIZE_IN_BITS/8u)

#define SHA_BLK_SIZE_IN_BITS        (512u)
#define SHA_BLK_SIZE_IN_BYTES       (SHA_BLK_SIZE_IN_BITS/8u)

typedef enum _CRYPTO_MODE {
    /* Note: Do not change the assignement. The assumption is used in ProcessBufferCompletion function */
    CRYPTO_MODE_ECB = ADI_CRYPTO_EVENT_ECB_BUFFER_PROCESSED,
    CRYPTO_MODE_CTR = ADI_CRYPTO_EVENT_CTR_BUFFER_PROCESSED,
    CRYPTO_MODE_CBC = ADI_CRYPTO_EVENT_CBC_BUFFER_PROCESSED,
    CRYPTO_MODE_CCM = ADI_CRYPTO_EVENT_CCM_BUFFER_PROCESSED,
    CRYPTO_MODE_CMAC = ADI_CRYPTO_EVENT_CMAC_BUFFER_PROCESSED,
    CRYPTO_MODE_SHA = ADI_CRYPTO_EVENT_SHA_BUFFER_PROCESSED
} CRYPTO_MODE;

typedef enum _CRYPTO_FIFO_SEQ {
    CRYPTO_FIFO_SEQ_BEFORE_BLK_EN,
    CRYPTO_FIFO_SEQ_AFTER_BLK_EN,
    CRYPTO_FIFO_SEQ_INTERRUPT
} CRYPTO_FIFO_SEQ;

/* Compute State */
typedef enum {
    CRYPTO_COMPUTE_STATE_IN_PAYLOAD_DONE        = 0x0001u, /* Device completed transferring input payload */
    CRYPTO_COMPUTE_STATE_IN_PREFIX_DONE         = 0x0002u, /* Device completed transferring prefix payload */
    CRYPTO_COMPUTE_STATE_IN_SHA_LAST_WORD_DONE  = 0x0004u, /* Device completed writing the SHA last word done*/
    CRYPTO_COMPUTE_STATE_IN_SHA_READY           = 0x0008u, /* SHA is ready to be read */
    CRYPTO_COMPUTE_STATE_OUT_PAYLOAD_DONE       = 0x0010u, /* Device completed transferring output payload */
    CRYPTO_COMPUTE_STATE_PROCESSING             = 0x0100u, /* Device is busy processing a buffer */    
    CRYPTO_COMPUTE_STATE_IN_DMA_SCHEDULED       = 0x1000u,  /* Input DMA is scheduled to the DMA Manager */
    CRYPTO_COMPUTE_STATE_OUT_DMA_SCHEDULED      = 0x2000u  /* Output DMA is scheduled to the DMA Manager */
} CRYPTO_COMPUTE_STATE;

/* Internal buffer structure */
typedef struct _CRYPTO_BUFFER {
    struct _CRYPTO_BUFFER*  pNext;                /* Next Buffer */

    uint32_t               nMode;                 /* Mode of operation */
    uint32_t               eEncMode;              /* Encoding Mode */
    uint32_t               eBuffDataFormat;       /* Data buffer endianess */
    uint32_t               eKeyLen;               /* Key Lendth */
    uint8_t*               pKey;                  /* Pointer to Key */

    ADI_CRYPTO_RESULT      eStatus;               /* Status for the device */
    void*                  pBuffer;               /* Pointer to Buffer */

    uint32_t*              pDataBuffer;           /* Pointer to Data buffer */
    uint32_t               nDataBuffLenInBits;    /* Data Buffer Len in Bits */

    uint32_t*              pOutputBuffer;         /* Pointer to Output buffer */
    uint32_t               nOutputBuffLenInBits;  /* Output Buffer Len in Bits */

    uint32_t*              pPrefixBuffer;         /* Pointer to Prefix Buffer */
    uint32_t               nPrefixBuffLenInBits;  /* Prefix Buffer Len in Bits */

    uint8_t*               pNonceIV;              /* Pointer to NONCE or IV */
    uint32_t               nCounterInit;          /* Counter Initialization Vlaue */

    uint32_t               nValidBytesInLastBlock;
} CRYPTO_BUFFER;

/* Compute Structure */
typedef struct _CRYPTO_COMPUTE {
    uint32_t          state;                /* Compute State */

    CRYPTO_BUFFER*    pIntBuff;             /* Pointer to the internal buffer */

    uint32_t*         pInput;                /* Pointer to input */
    uint32_t*         pOutput;               /* Pointer to output */
    uint32_t          nInpBitsRemaining;     /* Number of input bits remaining */
    uint32_t          nOutBitsRemaining;     /* Number of output bits remaining */

} CRYPTO_COMPUTE;

/* Forward declartion of device structure */
typedef struct _CRYPTO_DEVICE CRYPTO_DEVICE;

/* FIFO management function pointer */
typedef ADI_CRYPTO_RESULT(*CRYPTO_FIFO_MANAGE)(CRYPTO_DEVICE* dev, uint32_t nDir, CRYPTO_FIFO_SEQ eFifoSeq);

/* Device Structure */
struct _CRYPTO_DEVICE {
    bool_t              bDeviceEnabled;         /* Boolean flag to signify whether the device is enable/disabled */
    bool_t              bDmaEnabled;            /* Boolean flag to signify whether the DMA is enable/disabled */

    CRYPTO_FIFO_MANAGE  pfFifoManage;           /* FIFO management function to user */

    ADI_CRYPT_TypeDef   *pReg;                   /* Pointer to the registers */
    CRYPTO_COMPUTE      ActCompute;             /* Active computation structure */

    ADI_CALLBACK        pfCallback;             /* User defined callback function */
    void*               pCBParam;               /* User defined callback param */

    CRYPTO_BUFFER*      pFreeBuffers;           /* Free buffer list */
    CRYPTO_BUFFER*      pPendingBuffers;        /* Pending buffer list */
    CRYPTO_BUFFER*      pCompletedBuffers;      /* Completed buffer list */

    CRYPTO_BUFFER       buffers[NUM_BUFFERS];   /* Internal buffers */

#if (ADI_CRYPTO_ENABLE_DMA_SUPPORT == 1)
    ADI_DMA_CHANNEL_HANDLE hDmaInpHandle;       /* Input DMA Handle */
    ADI_DMA_CHANNEL_HANDLE hDmaOutHandle;       /* Output DMA Handle */


    /* DMA Memory */
    ALIGN4
    uint8_t            DmaMemoryIn[ADI_DMA_MEMORY_SIZE];
    ALIGN4
    uint8_t            DmaMemoryOut[ADI_DMA_MEMORY_SIZE];
#endif
    PEND_VAR_DECLR                              /* Semaphore count */
    
};

typedef struct _CRYPTO_INFO {
    ADI_CRYPT_TypeDef   *pReg;       /* Crypto peripheral registers */
    CRYPTO_DEVICE       *pDevice;    /* Device Handle */
} CRYPTO_INFO;

/*! \endcond */

#endif /* __ADI_CRYPTO_DEF_H__ */
