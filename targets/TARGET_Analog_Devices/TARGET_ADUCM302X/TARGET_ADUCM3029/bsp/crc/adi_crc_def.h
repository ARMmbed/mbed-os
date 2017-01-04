/*! *****************************************************************************
 * @file:    adi_crc_def.h
 * @brief:   Private header file for for CRC driver.
 * @version: $Revision: 33205 $
 * @date:    $Date: 2016-01-11 10:46:07 +0000 (Mon, 11 Jan 2016) $
 *
 * @details
 *           This is a private header file for the CRC driver,
 *           which contains the API declarations, data and
 *           constant definitions used in driver implementation
 -----------------------------------------------------------------------------
Copyright (c) 2010-2014 Analog Devices, Inc.

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
#ifndef __ADI_CRC_DEF_H__
#define __ADI_CRC_DEF_H__
/* CRC Driver includes */
#include <drivers/crc/adi_crc.h>

/*! \cond PRIVATE */
/*=============  I N C L U D E S   =============*/


typedef struct __ADI_CRC_DEVICE ADI_CRC_DEVICE;
typedef ADI_CRC_RESULT (*CRC_BUFFER_SUBMIT) (ADI_CRC_DEVICE *pDevice, void *pBuffer, uint32_t NumBytes, uint32_t NumBits);

/* Enumeration of CRC operation status */
typedef enum
{
    ADI_CRC_OP_IDLE                = 0u,        /* CRC idle */
    ADI_CRC_OP_IN_PROGRESS         = 0x01u,     /* CRC operation in progress */
} ADI_CRC_OP_STATUS;


#pragma pack()

/* Structure to handle CRC Peripheral instance */
struct __ADI_CRC_DEVICE
{
    ADI_DMA_CHANNEL_ID      eDMAChannel;      /* DMA Stream ID linked to this CRC peripheral */
    volatile ADI_CRC_TypeDef *pReg;
    CRC_BUFFER_SUBMIT       pfSubmitBuffer;   /* Function for submitting CRC data buffer for calculation */
    ADI_CALLBACK            pfCallback;       /* Client supplied callback function */
    void                    *pCBParam;        /* Client supplied callback parameter */
    void                    *pBuffer;         /* Poiner to the buffer  */
    uint32_t                RemainingBytes;   /* Remaining bytes */
    uint32_t                RemainingBits;    /* Remaining bits */
    ADI_CRC_OP_STATUS       eCrcOpStatus;     /* Current status of the CRC Operation */
#if (ADI_CRC_CFG_ENABLE_DMA_SUPPORT == 1)
    ADI_DMA_TRANSFER        gDmaDescriptor;   /* For DMA operation */
    uint8_t                 mDmaMem[ADI_DMA_MEMORY_SIZE];
    ADI_DMA_CHANNEL_HANDLE  hDma;
#endif
};

/* Structure to hold CRC device specific information */
typedef struct
{
    volatile ADI_CRC_TypeDef    *pReg;        /* CRC peripheral Registers */
    ADI_CRC_HANDLE              hDevice;      /* CRC device handle */
} ADI_CRC_INFO;



#define CRC_INVALID_DMA_CHANNEL 0XFFFFFFFFu
/*! \endcond */
#endif  /* __ADI_CRC_DEF_H__ */

/*****/
