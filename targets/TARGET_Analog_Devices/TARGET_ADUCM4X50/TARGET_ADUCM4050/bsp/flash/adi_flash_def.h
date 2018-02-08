/*!
 *****************************************************************************
 @file:    adi_flash_def.h
 @brief:   Internal Flash device driver definitions and macros
 @date:    $Date: 2014-11-28 01:48:03 -0500 (Fri, 28 Nov 2014) $
 -----------------------------------------------------------------------------
Copyright (c) 2012-2016 Analog Devices, Inc.

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

#ifndef ADI_FLASH_DEF_H
#define ADI_FLASH_DEF_H

/*! \cond PRIVATE */

#include <drivers/dma/adi_dma.h>
#include <config/adi_flash_config.h>

#include <drivers/flash/adi_flash.h>

/* fixed number of flash controllers */
#define ADI_FEE_NUM_INSTANCES       (1u)

/* STATUS register error mask */
#define ADI_FEE_STATUS_ERROR_MASK ( BITM_FLCC_STAT_ACCESS_MODE    \
                                  | BITM_FLCC_STAT_CACHESRAMPERR  \
                                  | BITM_FLCC_STAT_ECCDCODE       \
                                  | BITM_FLCC_STAT_ECCINFOSIGN    \
                                  | BITM_FLCC_STAT_SIGNERR        \
                                  | BITM_FLCC_STAT_OVERLAP        \
                                  | BITM_FLCC_STAT_ECCRDERR       \
                                  | BITM_FLCC_STAT_ECCERRCMD      \
                                  | BITM_FLCC_STAT_SLEEPING       \
                                  | BITM_FLCC_STAT_CMDFAIL)


#if defined(__ECC__)
#define ALIGN
#define ALIGN4  _Pragma("align(4)")
#elif defined(__ICCARM__)
#define ALIGN   _Pragma("pack()")
#define ALIGN4  _Pragma("pack(4)")
#elif defined (__GNUC__)
#define ALIGN   _Pragma("pack()")
#define ALIGN4  _Pragma("pack(4)")
#endif

/* Flash Size and Page/Block macros:
    512kB total user space, broken up as
        256-pages, 2kB/page
        32-blocks, 16kB/block
        8 pages/block
*/
#define FEE_FLASH_SIZE     (0x80000u)                          /* 512kB total */
#define FEE_PAGE_SHIFT     (11u)                               /* 2kB page size */
#define FEE_BLOCK_SHIFT    (14u)                               /* 16kB block size */
#define FEE_MAX_NUM_PAGES  (FEE_FLASH_SIZE >> FEE_PAGE_SHIFT)  /* max number of pages (256) */
#define FEE_MAX_NUM_BLOCKS (FEE_FLASH_SIZE >> FEE_BLOCK_SHIFT) /* max number of blocks (32) */

#if (ADI_FEE_CFG_ECC_START_PAGE >= FEE_MAX_NUM_PAGES)
#error "ADI_FEE_CFG_ECC_START_PAGE range is invalid"
#endif


/* INTERNAL DRIVER STATIC FUNCTION PROTOTYPES */

/* Send a command to the flash controller, but does no pend on it... */
static ADI_FEE_RESULT SendCommand (ADI_FEE_HANDLE const hDevice, uint32_t const cmd);

/* generic transfer initiator... dispatches to InitiatePioTransfer() or InitiateDmaTransfer() */
static ADI_FEE_RESULT InitiateTransfer (ADI_FEE_HANDLE const hDevice);

/* PIO initiator */
static ADI_FEE_RESULT InitiatePioTransfer (ADI_FEE_HANDLE const hDevice);

/* DMA initiator */
static ADI_FEE_RESULT InitiateDmaTransfer (ADI_FEE_HANDLE const hDevice);

/* interrupt handlers */
void Flash0_Int_Handler(void);
void DMA_FLASH0_Int_Handler (void);

/* INTERNAL DRIVER DATATYPES */

/*
 *****************************************************************************
 * FEE Configuration structure.
 *****************************************************************************/
typedef struct __ADI_FEE_CONFIG {
    uint32_t eccIrqEnables;     	/* ECC interrupt enables.                      */
    uint32_t param0;            	/* TIME_PARAM0 register.                       */
    uint32_t param1;            	/* TIME_PARAM1 register.                       */
    uint32_t abortEnableLo;     	/* Lower interrupt abort enables (IRQs 0-31).  */
    uint32_t abortEnableHi;     	/* Upper interrupt abort enables (IRQs 32-63.) */
    uint32_t eccConfig;         	/* ECC_CFG register.                           */
} ADI_FEE_CONFIG;


/* Flash physical device instance data */
typedef struct __ADI_FEE_DEVICE_INFO {

   ADI_FLCC_TypeDef *pDev;       	/* Pointer to the physical controller.         */
   IRQn_Type         pioIrqNum;  	/* The flash controller PIO interrupt number.  */
   IRQn_Type         dmaIrqNum;  	/* The flash controller DMA interrupt number.  */
   DMA_CHANn_TypeDef dmaChanNum;	/* The flash controller DMA channel number.    */
   ADI_FEE_CONFIG   *pConfig;    	/* Pointer to user config info.                */
   ADI_FEE_HANDLE    hDevice;    	/* Pointer the device memory (supplied by the application). */

} ADI_FEE_DEVICE_INFO;


/* Flash driver instance data structure */
typedef struct __ADI_FEE_DEV_DATA_TYPE {

    /* make sure to synchronize ANY size changes with ADI_FLASH_MEMORY_SIZE macro in adi_flash.h */

    /* NOTE: "volatile" storage class on all interrupt-modified valuables */

    /* device attributes */
    ADI_FLCC_TypeDef           *pDev;					/* Pointer top physical flash controller.        */
    ADI_FEE_DEVICE_INFO        *pDevInfo;				/* Pointer to hardware device attributes.        */

    /* callback info */
    ADI_CALLBACK                pfCallback;				/* Registered callback function address.         */
    void                       *pCBParam;				/* Registered callback user parameter.           */

    /* internal driver state variables */
    bool                        bUseDma;				/* DMA control flag (from user).                 */
    bool                        bSubmitCalled;			/* Flag to identify if a buffer was "submitted". */
    volatile uint32_t           FlashStatusCopy;		/* Clop of latest flash status register.         */
    volatile uint32_t           feeError;				/* Flash error collector.                        */
    volatile ADI_FEE_RESULT     dmaError;				/* DMA error collector.                          */
    volatile bool               bTransferInProgress;	/* Flag indicating if a transfer is in progress. */

    /* data info */
    volatile uint32_t          *pNextWriteAddress;      /* Pointer to next write data in flash space.    */
    volatile uint32_t          *pNextReadAddress;       /* Pointer to next read data in user buffer.     */
    volatile uint32_t           nRemainingBytes;        /* Number of remaining bytes still to transfer.  */

    SEM_VAR_DECLR        /* Blocking object: "Semaphore" for rtos,  "bLowPowerExitFlag" for non-rtos.    */

} ADI_FEE_DEV_DATA_TYPE;

/*! \endcond */

#endif /* ADI_FLASH_DEF_H */
