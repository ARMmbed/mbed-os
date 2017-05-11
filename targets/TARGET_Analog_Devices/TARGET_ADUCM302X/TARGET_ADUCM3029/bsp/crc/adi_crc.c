/*! ****************************************************************************
 * @file:    adi_crc.c
 * @brief:   CRC device driver global file.
 * @details: This file contain the CRC device driver impelementation.
 -----------------------------------------------------------------------------
Copyright (c) 2016 Analog Devices, Inc.

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
    or more patent holders. This license does not release you from the
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

#include <adi_processor.h>
#include <rtos_map/adi_rtos_map.h>


/** @addtogroup  CRC_Driver CRC Device Driver
 *  @{

@brief <b>Cyclic Redundancy Check (CRC) peripheral driver</b>
@details

The CRC peripheral is used to perform the Cyclic Redundancy Check (CRC) of the
block of data that is presented to the peripheral. The peripheral provides a
means to periodically verify the integrity of the system memory and it is based
on a CRC32 engine that computes the signature of 32-bit data presented to the
hardware engine. CRC operations can be core driven or DMA driven depending on
static configuration.

    - #ADI_CRC_CFG_ENABLE_DMA_SUPPORT set to 0 defines a core driven CRC driver
    - #ADI_CRC_CFG_ENABLE_DMA_SUPPORT set to a non 0 value defines a DMA driven
      CRC driver

<b>Core driven CRC operations</b>

The adi_crc_Compute function executes core driven CRC operations to calculate the
CRC on the buffer input with the CRC parameters set in the driver. In this mode,
data in the submitted buffer is transmitted to the CRC directly by the core.

<b>Memory DMA driver CRC operations</b>

The adi_crc_Compute function executes DMA driven CRC operations to calculate the
CRC on the buffer input with the CRC parameters set in the driver. In this mode,
data in the submitted buffer is transmitted to the CRC through DMA transfers.

The software DMA channel reserved for the CRC driver is defined by a macro,
ADI_CFG_CRC_SOFTWARE_DMA_CHANNEL_ID, which can take a value between 0 and 7.
If this macro is not defined, e.g. in a configuration file, then its value
is defaulted to 7: in this case, DMA channel SIP7 is used by the CRC driver
and DMA_SIP7_Int_Handler becomes the interrupt used by the DMA when a transfer
to the CRC is complete.

<em><b>Computing CRC</b></em>

The CRC engine performs a 32-bit CRC operation on the incoming data stream.

Sequence of function calls for Computing CRC :\n
    - #adi_crc_Open() to open CRC device and get a valid CRC handle.
    - #adi_crc_SetPolynomialVal() to set the polynomial value to be used in CRC operations.
    - #adi_crc_SetBitMirroring() to enable/disable bit mirroring
    - #adi_crc_SetByteMirroring() to enable/disable byte mirroring
    - #adi_crc_SetLSBFirst() to indicate if data is Big or Little Endian.
    - #adi_crc_IsCrcInProgress() to poll the current status of CRC operation or
      wait for callback event.
    - #adi_crc_GetFinalCrcVal() to get the CRC value of the data stream if its
      CRC value is unknown. (Note that #adi_crc_GetFinalCrcVal resets the CRC
      seed to the #ADI_CFG_CRC_SEED_VALUE default value.)

      Note that using statically configured parameters such as
      #ADI_CFG_CRC_ENABLE_BYTE_MIRRORING, #ADI_CFG_CRC_ENABLE_BIT_MIRRORING,
      #ADI_CFG_CRC_POLYNOMIAL and #ADI_CFG_CRC_SEED_VALUE, functions
      #adi_crc_SetBitMirroring, #adi_crc_SetByteMirroring, #adi_crc_SetPolynomialVal
      and #adi_crc_SetBitMirroring don't need to be called explicitly in your
      application: the parameters will be assigned when opening the driver.

  @note - The application must include drivers/crc/adi_crc.h to use this driver.
  @note - This driver also requires the DMA driver.  The application must include
          the DMA driver sources to avoid link errors.
 */

/*! \cond PRIVATE */
/*=============  I N C L U D E S   =============*/

#include <drivers/crc/adi_crc.h>
#include <adi_cyclecount.h>
#include "adi_crc_def.h"

/*=============  M I S R A  =============*/

#ifdef __ICCARM__
/*
* IAR MISRA C 2004 error suppressions.
*
* Pm123 (rule 8.5): there shall be no definition of objects or functions in a header file
*   This isn't a header as such.
*
* Pm088 (rule 17.4): pointer arithmetic should not be used.
* Pm152 (rule 17.4): array indexing shall only be applied to objects defined as an array type
*   Relying on pointer arithmetic for buffer handling.
*
* Pm140 (rule 11.3): a cast should not be performed between a pointer type and an integral type
*   Casts from pointer to uint32_t needed to determine pointer alignment.
*/
#pragma diag_suppress=Pm123,Pm088,Pm152,Pm140
#endif /* __ICCARM__ */

/*==============  D E F I N E S  ===============*/

/* CRC Peripheral specific information */
#define ADI_CRC_NUM_DEVICES             (1u)

/*! \endcond */

#if (ADI_CRC_CFG_ENABLE_DMA_SUPPORT != 0)

/**
 * If a DMA channel has not been configured for the CRC driver,
* then a default software DMA channel is assigned: SIP7.
 */

#ifndef ADI_CFG_CRC_SOFTWARE_DMA_CHANNEL_ID
#define ADI_CFG_CRC_SOFTWARE_DMA_CHANNEL_ID 7
#pragma message("ADI_CFG_CRC_SOFTWARE_DMA_CHANNEL_ID implicitly defaulted to 7!")
#endif

/**
 * The following macros define
 * - the Software DMA channel identifier to be used in CRC DMA driven operations
 * - the ISR used by the CRC, which depends on the Software DMA channel
 *   selected to drive the CRC in DMA driven CRC operations.
 * - the interrupt identifier mapped to the software DMA channel; selected for
 *   the CRC operations
 */
#if   (ADI_CFG_CRC_SOFTWARE_DMA_CHANNEL_ID == 0)
#define ADI_CFG_CRC_DMA_CHANNEL         SIP0_CHANn
#define ADI_DMA_CRC_ISR                 DMA_SIP0_Int_Handler
#define ADI_CRC_IRQ_ID                  DMA0_CH16_DONE_IRQn
#elif (ADI_CFG_CRC_SOFTWARE_DMA_CHANNEL_ID == 1)
#define ADI_CFG_CRC_DMA_CHANNEL         SIP1_CHANn
#define ADI_DMA_CRC_ISR                 DMA_SIP1_Int_Handler
#define ADI_CRC_IRQ_ID                  DMA0_CH17_DONE_IRQn
#elif (ADI_CFG_CRC_SOFTWARE_DMA_CHANNEL_ID == 2)
#define ADI_CFG_CRC_DMA_CHANNEL         SIP2_CHANn
#define ADI_DMA_CRC_ISR                 DMA_SIP2_Int_Handler
#define ADI_CRC_IRQ_ID                  DMA0_CH18_DONE_IRQn
#elif (ADI_CFG_CRC_SOFTWARE_DMA_CHANNEL_ID == 3)
#define ADI_CFG_CRC_DMA_CHANNEL         SIP3_CHANn
#define ADI_DMA_CRC_ISR                 DMA_SIP3_Int_Handler
#define ADI_CRC_IRQ_ID                  DMA0_CH19_DONE_IRQn
#elif (ADI_CFG_CRC_SOFTWARE_DMA_CHANNEL_ID == 4)
#define ADI_CFG_CRC_DMA_CHANNEL         SIP4_CHANn
#define ADI_DMA_CRC_ISR                 DMA_SIP4_Int_Handler
#define ADI_CRC_IRQ_ID                  DMA0_CH20_DONE_IRQn
#elif (ADI_CFG_CRC_SOFTWARE_DMA_CHANNEL_ID == 5)
#define ADI_CFG_CRC_DMA_CHANNEL         SIP5_CHANn
#define ADI_DMA_CRC_ISR                 DMA_SIP5_Int_Handler
#define ADI_CRC_IRQ_ID                  DMA0_CH21_DONE_IRQn
#elif (ADI_CFG_CRC_SOFTWARE_DMA_CHANNEL_ID == 6)
#define ADI_CFG_CRC_DMA_CHANNEL         SIP6_CHANn
#define ADI_DMA_CRC_ISR                 DMA_SIP6_Int_Handler
#define ADI_CRC_IRQ_ID                  DMA0_CH22_DONE_IRQn
#elif (ADI_CFG_CRC_SOFTWARE_DMA_CHANNEL_ID == 7)
#define ADI_CFG_CRC_DMA_CHANNEL         SIP7_CHANn
#define ADI_DMA_CRC_ISR                 DMA_SIP7_Int_Handler
#define ADI_CRC_IRQ_ID                  DMA0_CH23_DONE_IRQn
#else
#error "Invalid Software DMA channel identifier ADI_CFG_CRC_SOFTWARE_DMA_CHANNEL_ID: it must be between 0 and 7"
#endif

#endif  /* ADI_CRC_CFG_ENABLE_DMA_SUPPORT */

/*! \cond PRIVATE */

/** Check the validity of a CRC device identifier */
#define ADI_CRC_VALID_DEVICE_ID(DEVNUM)         ((DEVNUM)<(ADI_CRC_NUM_DEVICES))

/** Check that a CRC driver is in idle state */
#define ADI_CRC_DEVICE_IS_IDLE(DEV)             (((DEV)->eCrcOpStatus == ADI_CRC_OP_IDLE) ? true : false)

/*==============  D A T A  ===============*/

/**
 * Information for managing all the CRC devices available
 */
static ADI_CRC_INFO crc_device_info[ADI_CRC_NUM_DEVICES] =
{
   { pADI_CRC0,  NULL }      /* CRC  0 */
};

/*==============  M O R E   D E F I N E S  ===============*/

/** Check the validity of a CRC handle for debug mode */
#define ADI_CRC_INVALID_HANDLE(h)               ((NULL == (h)) || (crc_device_info[0].hDevice != (h)))

/** Condition used to indicate if a CRC driver is already in use */
#define ADI_CRC_DEVICE_IN_USE(DEVNUM)           ((NULL) != crc_device_info[(DEVNUM)].hDevice)

#ifdef ADI_DEBUG
#define HDL_TO_DEVICE_PTR(HDL) ((ADI_CRC_INVALID_HANDLE(HDL)) ? (NULL) : ((ADI_CRC_DEVICE*) (HDL)))
#else
#define HDL_TO_DEVICE_PTR(HDL) ((ADI_CRC_DEVICE*) (HDL))
#endif

/*=============  C O D E  =============*/

#if (ADI_CRC_NUM_DEVICES!=1u)
#error "!!! Current CRC driver implementation can deal with a unique CRC instance !!!"
#endif

/*=============  L O C A L   F U N C T I O N S =============*/

/* Prototypes for static functions (required by MISRA-C:2004 Rule 8.1)  */

static ADI_CRC_INFO *crc_DeviceInfo(ADI_CRC_HANDLE hDevice);

static void crc_ResetRegisters (ADI_CRC_DEVICE *pDevice);

#if (ADI_CRC_CFG_ENABLE_DMA_SUPPORT == 0)

/* Functions specific to core driven CRC operations */

static ADI_CRC_RESULT crc_ExecuteCoreDrivenOperation (ADI_CRC_DEVICE *pDevice, void *pCrcBuf, uint32_t NumBytes, uint32_t NumBits);

#else

/* Functions specific to DMA driven CRC operations */

static ADI_CRC_RESULT crc_ExecuteDmaDrivenOperation(ADI_CRC_DEVICE *pDevice, void *pCrcBuf, uint32_t NumBytes, uint32_t NumBits);
static void crc_CalculateCrcForRemaining(ADI_CRC_DEVICE *pDevice, uint8_t *pData, uint32_t NumBytes, uint32_t NumBits);
static void CRC_Callback_For_DMA_Err_Int_Handler(void *pcbparam, uint32_t nEvent, void *pArg);
void ADI_DMA_CRC_ISR(void);

#endif  /* ADI_CRC_CFG_ENABLE_DMA_SUPPORT */


/**
 * @brief       return a pointer to the CRC device information mapped to the CRC
 *              device identified by a handle
 *
 * @param [in]  hDevice         CRC device handle
 *
 * @return      pointer to CRC device information identified by hDevice
 *              (NULL if the CRC device handle is invalid)
 */
static ADI_CRC_INFO *crc_DeviceInfo(ADI_CRC_HANDLE hDevice)
{
    ADI_CRC_INFO *pCrcInfo = (ADI_CRC_INVALID_HANDLE(hDevice))
                           ? NULL
                           : (&(crc_device_info[0]));
    return pCrcInfo;
}


/**
 * @brief       Reset CRC registers to default values
 *
 * @details     Reset CRC registers to default values as defined in configuration.
 *
 * @param [in]  pDevice         Pointer to CRC device
 *
 * @return      None
 */
static void crc_ResetRegisters(ADI_CRC_DEVICE *pDevice)
{
    /* Cast the values to be assigned to the targetted types */
    const uint32_t byte_mirroring_val = (uint32_t) ADI_CFG_CRC_ENABLE_BYTE_MIRRORING;
    const uint32_t byte_mirroring_pos = (uint32_t) BITP_CRC_CTL_BYTMIRR;
    const uint32_t bit_mirroring_val  = (uint32_t) ADI_CFG_CRC_ENABLE_BIT_MIRRORING;
    const uint32_t bit_mirroring_pos  = (uint32_t) BITP_CRC_CTL_BITMIRR;
    const uint32_t seed_value = (uint32_t) ADI_CFG_CRC_SEED_VALUE;
    const uint32_t polynomial = (uint32_t) ADI_CFG_CRC_POLYNOMIAL;

    /* Set byte mirroring and bit mirroring in CTL register as configured */
    pDevice->pReg->CTL = ( (byte_mirroring_val << byte_mirroring_pos)
                         | (bit_mirroring_val << bit_mirroring_pos)
                         );
    pDevice->pReg->RESULT = seed_value;
    pDevice->pReg->POLY   = polynomial;
}

#if (ADI_CRC_CFG_ENABLE_DMA_SUPPORT == 0)

/*
 * @brief       Starts core driven CRC operation.
 *
 * @param [in]  pDevice         Pointer to CRC device
 * @param [in]  pCrcBuf         Address of data buffer.
 * @param [in]  NumBytes        Number of bytes in data buffer.
 * @param [in]  NumBits         Number of bits, 0 to 7, in the last partial byte
 *                              in CRC data buffer
 *
 * @return      Status
 *              - ADI_CRC_SUCCESS: Successfully set expected CRC result.
 */
static ADI_CRC_RESULT crc_ExecuteCoreDrivenOperation(
    ADI_CRC_DEVICE      *pDevice,
    void                *pCrcBuf,
    uint32_t            NumBytes,
    uint32_t            NumBits)
{
    ADI_CRC_RESULT result = ADI_CRC_SUCCESS;
    uint8_t *pData = (uint8_t *)pCrcBuf;        /* initialize the pointer to data to the start of the data buffer */
    uint32_t lsbFirst = pDevice->pReg->CTL & BITM_CRC_CTL_LSBFIRST;

    pDevice->pReg->CTL |= (BITM_CRC_CTL_EN);    /*! enable CRC peripheral */

    if (((uint32_t)pData & 0x3u) != 0u)         /* If the buffer is not 4-byte aligned */
    {
        /* feed the CRC byte per byte as long as there are data in the input buffer AND
         * the data left in the buffer are not 4-byte aligned */
        while ((NumBytes > 0u) && (((uint32_t)pData & 0x3u) != 0u))
        {
            pDevice->pReg->IPBYTE = *pData;     /* feed the CRC with the first byte in the buffer */
            pData++;                            /* get the next byte to feed into CRC */
            NumBytes--;                         /* decrease the number of bytes to be processed */
        }
    }

    /* data left in the input buffer are now 4-byte aligned */

    while (NumBytes >= 4u)                      /* if the number of bytes left is greater than 4 bytes */
    {                                           /* feed CRC peripheral with 4-byte data */
        uint32_t nData;                         /* 32-bit variable to be used to feed the CRC peripheral */

        /*
         * Here we assume memory is little endian. We need change the following
         * code if we produce a Cortex-M processor with big endian memory.
         */
        if (lsbFirst != 0u)
        {
            nData = pData[3];
            nData = (nData << 8) | pData[2];
            nData = (nData << 8) | pData[1];
            nData = (nData << 8) | pData[0];
        }
        else
        {
            nData = pData[0];
            nData = (nData << 8) | pData[1];
            nData = (nData << 8) | pData[2];
            nData = (nData << 8) | pData[3];
        }
        pDevice->pReg->IPDATA = nData;          /* feed the CRC peripheral with 32-bit data input */
        pData += 4;                             /* move the data pointer in the data buffer */
        NumBytes -= 4u;                         /* decrease the number of data to be processed */
    }

    while (NumBytes > 0u)                       /* if the number of data left in the input buffer is smaller than 4 */
    {
        pDevice->pReg->IPBYTE = *pData;         /* feed the CRC peripheral with the remaining bytes */
        pData++;                                /* move the pointer to the next byte in input data buffer */
        NumBytes--;                             /* decrease the number of data to be fed into the CRC peripheral */
    }

    if (NumBits > 0u)                           /* if the last byte is a partial byte containing less than 8 bits */
    {
        pDevice->pReg->IPBITS[NumBits] = *pData;/* feed the CRC peripheral with the remaining bits (use IPBITS[N] to feed N bits) */
    }

    pDevice->pReg->CTL &= ~(BITM_CRC_CTL_EN);   /* All the data have been fed into the CRC peripheral : disable it */
    pDevice->eCrcOpStatus = ADI_CRC_OP_IDLE;    /* CRC back in idle state */
    return result;
}

#else   /* ADI_CRC_CFG_ENABLE_DMA_SUPPORT */

/**
 * @brief       Send a Memory DMA request to the CRC, which triggers a DMA driven
 *              CRC operation.
 *
 * @param [in]  pDevice         Pointer to CRC device
 * @param [in]  pCrcBuf         Address of data buffer.
 * @param [in]  NumBytes        Number of whole bytes in data buffer.
 * @param [in]  NumBits         Number of bits, 0 to 7, in the last partial byte
 *                              in CRC data buffer
 *
 * @return      Status
 *              - #ADI_CRC_SUCCESS: Successfully set expected CRC result.
 *              - #ADI_CRC_INVALID_DMA_CHANNEL: DMA channel cannot be used with CRC
 */
static ADI_CRC_RESULT crc_ExecuteDmaDrivenOperation(
    ADI_CRC_DEVICE      *pDevice,
    void                *pCrcBuf,
    uint32_t            NumBytes,
    uint32_t            NumBits)
{
    ADI_CRC_RESULT result = ADI_CRC_SUCCESS;
    uint8_t *pData = (uint8_t *)pCrcBuf;
    bool bUseDma = false;                               /* assume core driven CRC by default */

#ifdef ADI_DEBUG
    if (!ADI_CRC_VALID_DMA_CHANNEL(ADI_CFG_CRC_DMA_CHANNEL))
    {
        /* Report error as Memory DMA not open */
        result = ADI_CRC_INVALID_DMA_CHANNEL;
    }
    else
#endif /* ADI_DEBUG */
    {
        /* If LSBFIRST, it's easy. */
        if ((pDevice->pReg->CTL & BITM_CRC_CTL_LSBFIRST) != 0u)
        {
            /* If the buffer is not 4-byte aligned */
            if (((uint32_t)pData & 0x3u) != 0u)
            {
                /* process the first bytes until a 4-byte aligned data location is reached */
                pDevice->pReg->CTL |= (BITM_CRC_CTL_EN);        /* enable CRC */
                while ((NumBytes > 0u) && (((uint32_t)pData & 0x3u) != 0u))
                {
                    pDevice->pReg->IPBYTE = *pData;             /* feed byte into CRC */
                    pData++;                                    /* get to the next byte */
                    NumBytes--;                                 /* decrease the number of bytes still to be processed */
                }
                pDevice->pReg->CTL &= ~(BITM_CRC_CTL_EN);       /* disable CRC */
            }

            /* 4-byte aligned data transfer */
            if (NumBytes >= 4u)
            {
                /* there are enough data for kicking off a DMA driven CRC operation */
                const uint32_t channelId = (uint32_t) ADI_CFG_CRC_DMA_CHANNEL;
                const uint32_t channelBit = 1ul << channelId;           /* get a value with the bit set at position identified by channelId */
                const uint32_t numData = NumBytes / 4u;                 /* number of 4-byte data to be transferred */
                const uint32_t src = (uint32_t) pData;                  /* DMA source address */
                const uint32_t dst = (uint32_t) &pDevice->pReg->IPDATA; /* destination is CRC IPDATA 32-bit register */
                const uint32_t numTransData = ( (numData > DMA_TRANSFER_LIMIT)
                                              ? DMA_TRANSFER_LIMIT
                                              : numData
                                               );
                const uint32_t numTransBytes = (numTransData << 2u);
                const uint32_t lastDataPos = (numTransBytes - 4u);      /* position of last 32-bit data to be transferred in current DMA request */

                pDevice->pReg->CTL |= ((uint32_t) BITM_CRC_CTL_EN);     /* enable CRC (leave other bits unmodified) */

                pADI_DMA0->EN_SET = channelBit;                         /* Enable the channel */
                pADI_DMA0->ALT_CLR = channelBit;                        /* Set the primary as the current DMA descriptor */
                pADI_DMA0->SRCADDR_CLR = channelBit;                    /* Ensure decrement for source is cleared */
                pADI_DMA0->DSTADDR_CLR = channelBit;                    /* Ensure decrement for destination is cleared */

                pPrimaryCCD[channelId].DMADSTEND = dst;                 /* destination is CRC IPDATA 32-bit register */
                pPrimaryCCD[channelId].DMASRCEND = src + lastDataPos;   /* source end address */

                pPrimaryCCD[channelId].DMACDC =
                    ( (((uint32_t) ADI_DMA_INCR_NONE)    << ((uint32_t) DMA_BITP_CTL_DST_INC))  /* destination address not incremented */
                    | (((uint32_t) ADI_DMA_INCR_4_BYTE)  << ((uint32_t) DMA_BITP_CTL_SRC_INC))  /* source address incremented by 4 bytes */
                    | (((uint32_t) ADI_DMA_WIDTH_4_BYTE) << ((uint32_t) DMA_BITP_CTL_SRC_SIZE)) /* source data size is 4-byte */
                    | ((numTransData - 1u)               << ((uint32_t) DMA_BITP_CTL_N_MINUS_1))/* number of DMA transfers (minus 1) */
                    | (DMA_ENUM_CTL_CYCLE_CTL_AUTO_REQ << DMA_BITP_CTL_CYCLE_CTL)               /* DMA Auto Request transmission */
                    );
                pDevice->pRemainingData = (void*)(src + numTransBytes); /* remaining data start address */
                pDevice->RemainingBytes = NumBytes - numTransBytes;     /* remaining bytes that cannot be processed in this DMA batch */
                pDevice->RemainingBits = NumBits;                       /* remaining bits if last byte is a partial byte */
                bUseDma = true;                                         /* there are enough data to run 4-byte DMA transfers to CRC */
            }
        }
        /*
        * If ! LSBFIRST, we need the DMA controller support byte swap for fixed destination address.
        * But we don't have such luck, although it supports byte swap for fixed source address.
        * So we have to set DMA size to one byte, which is slower.
        *
        * Another option is using mirroring feature of CRC unit, which would be more complicated.
        */
        else
        {
            if (NumBytes > 0u)
            {
                /**
                 * There are enough data for kicking off a DMA driven CRC operation.
                 * DMA transfers are limited to 1024 bytes : if the buffer is larger
                 * than 1024 then generate repeated DMA request through the CRC DMA
                 * interrupt handler, i.e. the interrupt handler used by the software
                 * DMA channel driving the CRC operations.
                 */
                const uint32_t channelId = (uint32_t) ADI_CFG_CRC_DMA_CHANNEL;
                const uint32_t channelBit = 1ul << channelId;           /* get a value with the bit set at position identified by channelId */
                const uint32_t src = (uint32_t) pData;                  /* DMA source address */
                const uint32_t dst = (uint32_t) &pDevice->pReg->IPBYTE; /* destination is CRC IPBYTE 8-bit register */
                const uint32_t numTransData = ( (NumBytes > DMA_TRANSFER_LIMIT)
                                              ? DMA_TRANSFER_LIMIT
                                              : NumBytes
                                              );
                const uint32_t lastDataPos = (numTransData - 1u);       /* position of last data to be transferred in buffer */

                pDevice->pReg->CTL |= (BITM_CRC_CTL_EN);                /* enable CRC (leave other bits unmodified) */

                pADI_DMA0->EN_SET = channelBit;                         /* Enable the channel */
                pADI_DMA0->ALT_CLR = channelBit;                        /* Set the primary as the current DMA descriptor */
                pADI_DMA0->SRCADDR_CLR = channelBit;                    /* Ensure decrement for source is cleared */
                pADI_DMA0->DSTADDR_CLR = channelBit;                    /* Ensure decrement for destination is cleared */

                pPrimaryCCD[channelId].DMADSTEND = dst;                 /* destination is CRC IPBYTE 8-bit register */
                pPrimaryCCD[channelId].DMASRCEND = src + lastDataPos;   /* source end address */
                pPrimaryCCD[channelId].DMACDC =
                    ( (((uint32_t) ADI_DMA_INCR_NONE)    << ((uint32_t) DMA_BITP_CTL_DST_INC))  /* destination address not incremented */
                    | (((uint32_t) ADI_DMA_INCR_1_BYTE)  << ((uint32_t) DMA_BITP_CTL_SRC_INC))  /* source address incremented by 1 byte */
                    | (((uint32_t) ADI_DMA_WIDTH_1_BYTE) << ((uint32_t) DMA_BITP_CTL_SRC_SIZE)) /* source data size is 1-byte */
                    | ((numTransData - 1u)               << ((uint32_t) DMA_BITP_CTL_N_MINUS_1))/* number of DMA transfers (minus 1) */
                    | (DMA_ENUM_CTL_CYCLE_CTL_AUTO_REQ   << DMA_BITP_CTL_CYCLE_CTL)             /* DMA Auto Request transmission */
                    );
                pDevice->pRemainingData = (void*) (src + numTransData); /* remaining data start address */
                pDevice->RemainingBytes = NumBytes - numTransData;      /* remaining bytes */
                pDevice->RemainingBits = NumBits;                       /* remaining bits if last byte is a partial byte */
                bUseDma = true;                                         /* there are enough data to run 4-byte DMA transfers to CRC */
            }
        }

        /* if we are in a position to use the DMA to transfer data to the CRC */
        if (bUseDma== true)
        {
            const uint32_t channelId = (uint32_t) ADI_CFG_CRC_DMA_CHANNEL;
            const uint32_t channelBit = 1ul << channelId;       /* get a value with the bit set at position identified by channelId */
            pADI_DMA0->SWREQ = channelBit;                      /* Issue a software DMA request */
        }
        else
        {
            pDevice->pReg->CTL |= (BITM_CRC_CTL_EN);
            crc_CalculateCrcForRemaining(pDevice, pData, NumBytes, NumBits);
            pDevice->pReg->CTL &= ~(BITM_CRC_CTL_EN);
            if(pDevice->pfCallback != NULL)
            {
                pDevice->pfCallback(pDevice->pCBParam, (uint32_t) ADI_CRC_EVENT_BUFFER_PROCESSED, pData);
            }
            pDevice->eCrcOpStatus = ADI_CRC_OP_IDLE;            /* CRC calculation completed */
        }
    }
    return result;
}

/**
 * @brief       Completes a DMA driven CRC operation by dealing with remaining
 *              data, usually when the number of bytes left is smaller than 4.
 *
 * @param [in]  pDevice         Pointer to CRC device
 * @param [in]  pData           Address of data buffer.
 * @param [in]  NumBytes        Number of whole bytes in data buffer.
 * @param [in]  NumBits         Number of bits, 0 to 7, in the last partial byte
 *                              in CRC data buffer
 */
static void crc_CalculateCrcForRemaining(ADI_CRC_DEVICE *pDevice, uint8_t *pData, uint32_t NumBytes, uint32_t NumBits)
{
    /* process the remaining bytes */
    while (NumBytes > 0u)
    {
        pDevice->pReg->IPBYTE = *pData;
        pData++;
        NumBytes--;
    }

    /* process the remaining bits in the last byte if the number of bits is smaller than 8 */
    if (NumBits > 0u)
    {
        pDevice->pReg->IPBITS[NumBits] = *pData;
    }
}

/**
 * @brief       Callback function used by the DMA when a DMA error occurs
 *
 * @details     Callback function used by the DMA when a DMA error must be reported
 *              to the CRC driver because it affects the DMA channel driving the CRC.
 */
static void CRC_Callback_For_DMA_Err_Int_Handler(void *pcbparam, uint32_t nEvent, void *pArg)
{
    ADI_CRC_DEVICE *pDevice = HDL_TO_DEVICE_PTR(pcbparam);

    if (NULL != pDevice)
    {
        /* DMA error detected */
        pDevice->eCrcOpStatus = ADI_CRC_OP_IDLE;                /* mark the CRC peripheral as IDLE */
        pDevice->pReg->CTL &= (uint32_t)(~(BITM_CRC_CTL_EN));   /* disable CRC peripheral */
    }
}

/**
 * @brief       interrupt handler used by the software DMA channel driving the CRC
 *
 * @details     interrupt handler used by the software DMA channel driving the CRC
 *              ADI_DMA_CRC_ISR is a macro with the final interrupt handler name
 *              being DMA_SIP0_Int_Handler, ..., DMA_SIP7_Int_Handler, depending
 *              on the software DMA channel driving the CRC.
 */
void ADI_DMA_CRC_ISR(void)
{
    ISR_PROLOG();

    if (ADI_CRC_DEVICE_IN_USE(0))
    {
        ADI_CRC_DEVICE * pDevice = HDL_TO_DEVICE_PTR(crc_device_info[0].hDevice);
        if (NULL != pDevice)
        {
            uint8_t *pData = (uint8_t *)(pDevice->pRemainingData);
            uint32_t NumBytes = pDevice->RemainingBytes;
            uint32_t NumBits = pDevice->RemainingBits;
            bool finishing = (NumBytes < 4u);

            if (!finishing)
            {
                /* there's enough data left for another DMA transfer */
                ADI_CRC_RESULT result = pDevice->pfSubmitBuffer(pDevice, pData, NumBytes, NumBits);
                if (ADI_CRC_SUCCESS != result)
                {
                    /* buffer submission failed: complete the task through core driven operations */
                    finishing = true;
                }
            }

            if (finishing)
            {
                /* There are a very few bytes/bits left to be processed or
                 * a DMA transfer request could not be sent */
                crc_CalculateCrcForRemaining(pDevice, pData, NumBytes, NumBits);

                /* if a callback function is registered with the interrupt handler
                 * associated with the software DMA channel driving the CRC */
                if(pDevice->pfCallback != NULL)
                {
                    pDevice->pfCallback(pDevice->pCBParam, (uint32_t) ADI_CRC_EVENT_BUFFER_PROCESSED, NULL);
                }
                pDevice->eCrcOpStatus = ADI_CRC_OP_IDLE;        /* CRC back in idle state */

            }
        }
    }

#if defined(ADI_CYCLECOUNT_CRC_ISR_ENABLED) && (ADI_CYCLECOUNT_CRC_ISR_ENABLED == 1u)
    ADI_CYCLECOUNT_STORE(ADI_CYCLECOUNT_ISR_CRC);    
#endif

    ISR_EPILOG();
}

#endif  /* ADI_CRC_CFG_ENABLE_DMA_SUPPORT */

/*! \endcond */

/*=============  P U B L I C   F U N C T I O N S =============*/

/**
 * @brief       Opens a CRC device instance.
 *
 * @param [in]  DeviceNum       Number identifying the CRC Device to open.
 * @param [in]  pMemory         Pointer to a #ADI_CRC_MEMORY_SIZE.
 *                              sized buffer to manage the device instance.
 * @param [in]  MemorySize      Size of the buffer to which "pMemory" points.
 * @param [out] phDevice        Pointer to a location where CRC device handle to be written.
 *
 * @return      Status
 *              - #ADI_CRC_SUCCESS: Successfully opened a CRC device.
 *              - #ADI_CRC_BAD_DEVICE_NUMBER [D]: Supplied CRC Device ID is invalid.
 *              - #ADI_CRC_IN_USE [D]: Supplied CRC Device ID is already in use.
 *              - #ADI_CRC_INSUFFICIENT_MEMORY [D]: Supplied memory is not sufficient to handle a CRC device instance.
 *              - #ADI_CRC_FAILURE [D]: callback registration failed for CRC function used by DMA Error Interrupt Handler.
 *
 * @note        For the device memory should be of size #ADI_CRC_MEMORY_SIZE.
 *
 */
ADI_CRC_RESULT adi_crc_Open(
	uint32_t            DeviceNum,
	void                *pMemory,
	uint32_t            MemorySize,
	ADI_CRC_HANDLE      *phDevice)
{
    ADI_CRC_RESULT result = ADI_CRC_SUCCESS;
    ADI_CRC_DEVICE *pDevice = (ADI_CRC_DEVICE*) pMemory;/* memory block to be used to manage a CRC driver instance */

#ifdef ADI_DEBUG                                        /* IF (Debug information enabled) */
    if (!ADI_CRC_VALID_DEVICE_ID(DeviceNum))            /* IF (This is not a valid CRC device number) */
    {
        result = ADI_CRC_BAD_DEVICE_NUMBER;             /* Report failure as bad device number */
    }
    else if (ADI_CRC_DEVICE_IN_USE(DeviceNum))          /* IF (The device is in use) */
    {
        result = ADI_CRC_IN_USE;                        /* return CRC Device in use error */
    }
    else if (  (MemorySize < ADI_CRC_MEMORY_SIZE)       /* IF (Supplied memory size is insufficient) */
            || (ADI_CRC_MEMORY_SIZE < sizeof(ADI_CRC_DEVICE))
            )
    {
        result = ADI_CRC_INSUFFICIENT_MEMORY;           /* Report failure as insufficient memory */
    }
    else
#endif  /* ADI_DEBUG */
    {
        /* check that ADI_CRC_MEMORY_SIZE is accurately defined */
        assert(ADI_CRC_MEMORY_SIZE == sizeof(ADI_CRC_DEVICE));

        memset(pMemory, 0, MemorySize);                 /* Clear the given memory */

        ADI_INT_STATUS_ALLOC();
        ADI_ENTER_CRITICAL_REGION();                    /* Entering critical region, disable interrupts */

            /* Save the supplied device memory address */
            crc_device_info[DeviceNum].hDevice = (ADI_CRC_HANDLE)pDevice;
            pDevice->pReg = crc_device_info[DeviceNum].pReg;

        ADI_EXIT_CRITICAL_REGION();                     /* Re-enable interrupts */

        crc_ResetRegisters(pDevice);                    /* Reset CRC registers */
        *phDevice = crc_device_info[DeviceNum].hDevice; /* Pass a valid handle to this CRC device */

#if (ADI_CRC_CFG_ENABLE_DMA_SUPPORT == 0)

        pDevice->pfSubmitBuffer = &crc_ExecuteCoreDrivenOperation;

#else   /* ADI_CRC_CFG_ENABLE_DMA_SUPPORT */

        pDevice->pfSubmitBuffer = &crc_ExecuteDmaDrivenOperation;
        adi_dma_Init();

        /* Register CRC DMA callback */
#ifdef ADI_DEBUG                                        /* IF (Debug information enabled) */
        if (ADI_DMA_SUCCESS != adi_dma_RegisterCallback(ADI_CFG_CRC_DMA_CHANNEL,CRC_Callback_For_DMA_Err_Int_Handler,pDevice))
        {
            result = ADI_CRC_FAILURE;
        }
#else
        adi_dma_RegisterCallback(ADI_CFG_CRC_DMA_CHANNEL,CRC_Callback_For_DMA_Err_Int_Handler,pDevice);
#endif
        NVIC_EnableIRQ(ADI_CRC_IRQ_ID);                 /* Enable the interrupt for the DMA channel used by CRC */
#endif  /* ADI_CRC_CFG_ENABLE_DMA_SUPPORT */
    }
    return result;
}

/**
 * @brief       Closes CRC device instance opened for use.
 *
 * @param [in]  hDevice         Handle to CRC Device instance to close.
 *
 * @return      Status
 *              - #ADI_CRC_SUCCESS: Successfully closed CRC device.
 *              - #ADI_CRC_BAD_HANDLE [D]: Supplied CRC handle is invalid.
 *              - #ADI_CRC_FAILURE [D]: callback un-registration failed for CRC function used by DMA Error Interrupt Handler.
 */
ADI_CRC_RESULT adi_crc_Close(ADI_CRC_HANDLE const hDevice)
{
    ADI_CRC_RESULT result = ADI_CRC_SUCCESS;
    ADI_CRC_INFO *pCrcInfo = crc_DeviceInfo(hDevice);   /* get CRC info pointer from CRC handle */
#ifdef ADI_DEBUG
    if (NULL == pCrcInfo)
    {
        result = ADI_CRC_BAD_HANDLE;                    /* invalid CRC handle being used */
    }
    else
#endif
    {
#if (ADI_CRC_CFG_ENABLE_DMA_SUPPORT != 0)
        NVIC_DisableIRQ(ADI_CRC_IRQ_ID);                /* Disable the interrupt for the DMA channel used by CRC. */
        /* Register CRC DMA callback */
#ifdef ADI_DEBUG                                        /* IF (Debug information enabled) */
        if (ADI_DMA_SUCCESS != adi_dma_RegisterCallback(ADI_CFG_CRC_DMA_CHANNEL,NULL,NULL))
        {
            result = ADI_CRC_FAILURE;
        }
#else
        adi_dma_RegisterCallback(ADI_CFG_CRC_DMA_CHANNEL,NULL,NULL);
#endif
#endif
        pCrcInfo->hDevice = NULL;                       /* Mark CRC driver as closed */
    }
    return result;
}
/*!
 * @brief       Set the bit mirroring. This function should be called only when device is idle,
 *              i.e. when no data are being processd by the CRC.
 *
 * @param[in]   hDevice     Device handle obtained from adi_crc_Open().
 * @param[in]   bEnable     Boolean flag to enable/disable bit mirroring.
 *                          true : To Enable bit mirroring.
 *                          false :  To Disable bit mirroring.
 *
 * @return      Status
 *              - #ADI_CRC_SUCCESS: Call completed successfully.
 *              - #ADI_CRC_BAD_HANDLE [D] :Invalid device handle parameter.
 *              - #ADI_CRC_FN_NOT_PERMITTED [D]: CRC is executing a request, its parameters cannot be altered.
 *
 * @sa          adi_crc_SetByteMirroring().
 * @sa          adi_crc_SetWordSwap().
 */
ADI_CRC_RESULT adi_crc_SetBitMirroring(ADI_CRC_HANDLE const hDevice, const bool bEnable)
{
    ADI_CRC_RESULT result = ADI_CRC_SUCCESS;
    ADI_CRC_DEVICE *pDevice = HDL_TO_DEVICE_PTR(hDevice);       /* get CRC device pointer from CRC handle */

#ifdef ADI_DEBUG
    if (NULL == pDevice)                                        /* IF (CRC device handle is invalid) */
    {
        result = ADI_CRC_BAD_HANDLE;
    }
    else if (!ADI_CRC_DEVICE_IS_IDLE(pDevice))                  /* IF (CRC in progress) */
    {
        result = ADI_CRC_FN_NOT_PERMITTED;                      /* Function not permitted when CRC operation is in progress */
    }
    else
#endif
    if(bEnable == true)
    {
        pDevice->pReg->CTL |= (BITM_CRC_CTL_BITMIRR);                   /* enable bit mirroring */
    }
    else
    {
        pDevice->pReg->CTL &= (uint32_t)(~(BITM_CRC_CTL_BITMIRR));      /* disable bit mirroring */
    }
    return result;
}
/*!
 * @brief       Set the byte mirroring. This function should be called only when device is disabled.
 *
 * @param[in]   hDevice    Device handle obtained from adi_crc_Open().
 * @param[in]   bEnable    Boolean flag to enable/disable byte mirroring.
 *                         true  : To Enable byte mirroring.
 *                         false : To Disable byte mirroring.
 *
 * @return      Status
 *              - #ADI_CRC_SUCCESS: Call completed successfully.
 *              - #ADI_CRC_BAD_HANDLE [D]: Invalid device handle parameter.
 *              - #ADI_CRC_FN_NOT_PERMITTED [D]: CRC is executing a request, its parameters cannot be altered.
 *
 *
 * @sa        adi_crc_EnableBitMirroring().
 * @sa        adi_crc_EnableWordSwap().
 */
ADI_CRC_RESULT  adi_crc_SetByteMirroring(ADI_CRC_HANDLE const hDevice, const bool bEnable)
{
    ADI_CRC_RESULT result = ADI_CRC_SUCCESS;
    ADI_CRC_DEVICE *pDevice = HDL_TO_DEVICE_PTR(hDevice);       /* get CRC device pointer from CRC handle */

#ifdef ADI_DEBUG
    if (NULL == pDevice)                                        /* IF (CRC device handle is invalid) */
    {
        result = ADI_CRC_BAD_HANDLE;
    }
    else if (!ADI_CRC_DEVICE_IS_IDLE(pDevice))                  /* IF (CRC in progress) */
    {
        result = ADI_CRC_FN_NOT_PERMITTED;                      /* Function not permitted when CRC operation is in progress */
    }
    else
#endif
    if(bEnable == true)
    {
       pDevice->pReg->CTL |= (BITM_CRC_CTL_BYTMIRR);                    /* enable byte mirroring */
    }
    else
    {
       pDevice->pReg->CTL &= (uint32_t)(~(BITM_CRC_CTL_BYTMIRR));       /* disable byte mirroring */
    }
    return result;
}

/*!
 * @brief       Enable the LSB first.
 *
 * @param[in]   hDevice    Device handle obtained from adi_crc_Open().
 * @param[in]   bEnable    Boolean flag which indicate whether LSB first OR MSB first for CRC calculation.
 *                         true  : For LSB First CRC calculation
 *                         false : For MSB First CRC calculation
 *
 * @return      Status
 *              - #ADI_CRC_SUCCESS: Call completed successfully.
 *              - #ADI_CRC_BAD_HANDLE [D]: Invalid device handle parameter.
 *              - #ADI_CRC_FN_NOT_PERMITTED [D]: CRC is executing a request, its parameters cannot be altered.
 *
 *
 * @sa          adi_crc_EnableBitmirroring().
 * @sa          adi_crc_EnableWordSwap().
 */

ADI_CRC_RESULT  adi_crc_SetLSBFirst(ADI_CRC_HANDLE const hDevice, const bool bEnable)
{
    ADI_CRC_RESULT result = ADI_CRC_SUCCESS;
    ADI_CRC_DEVICE *pDevice = HDL_TO_DEVICE_PTR(hDevice);       /* get CRC device pointer from CRC handle */

#ifdef ADI_DEBUG
    if (NULL == pDevice)                                        /* IF (CRC device handle is invalid) */
    {
        result =  ADI_CRC_BAD_HANDLE;
    }
    else if (!ADI_CRC_DEVICE_IS_IDLE(pDevice))                  /* IF (CRC in progress) */
    {
        result = ADI_CRC_FN_NOT_PERMITTED;                      /* function not permitted when CRC operation is in progress */
    }
    else
#endif
    if(bEnable == true)
    {
       pDevice->pReg->CTL |= (BITM_CRC_CTL_LSBFIRST);           /* enable LSB first (MSB first disable) */
    }
    else
    {
       pDevice->pReg->CTL &= ~(BITM_CRC_CTL_LSBFIRST);          /* disable LSB first (MSB first enable) */
    }
    return result;
}
/*!
 * @brief       To enable/disable the word Swap. This function should be called only when device is disabled.
 *
 * @param[in]   hDevice    Device handle obtained from adi_crc_Open().
 * @param[in]   bEnable    Boolean flag to enable/disable word swap.
 *                         true  : To Enable word swap.
 *                         false : To Disable word swap.
 *
 * @return      Status
 *              - #ADI_CRC_SUCCESS: Call completed successfully.
 *              - #ADI_CRC_BAD_HANDLE [D]: Invalid device handle parameter.
 *              - #ADI_CRC_FN_NOT_PERMITTED [D]: CRC is executing a request, its parameters cannot be altered.
 *
 *
 * @sa          adi_crc_SetBitMirroring().
 * @sa          adi_crc_SetByteMirroring().
 */
ADI_CRC_RESULT adi_crc_EnableWordSwap(ADI_CRC_HANDLE const hDevice, const bool bEnable)
{
    ADI_CRC_RESULT result = ADI_CRC_SUCCESS;
    ADI_CRC_DEVICE *pDevice = HDL_TO_DEVICE_PTR(hDevice);

#ifdef ADI_DEBUG
    if (NULL == pDevice)                                /* IF (CRC device handle is invalid) */
    {
        result = ADI_CRC_BAD_HANDLE;
    }
    else if (!ADI_CRC_DEVICE_IS_IDLE(pDevice))          /* IF (CRC in progress) */
    {
        result = ADI_CRC_FN_NOT_PERMITTED;              /* function not permitted when CRC operation is in progress */
    }
    else
#endif
    if(bEnable == true)
    {
       pDevice->pReg->CTL |= BITM_CRC_CTL_W16SWP;       /* enable word swap */
    }
    else
    {
       pDevice->pReg->CTL &= ~BITM_CRC_CTL_W16SWP;      /* disable word swap */
    }

    return result;
}
/**
 * @brief       Sets the initial seed value for the CRC operation that is about to take place.
 *
 * @param [in]  hDevice         Handle to CRC device instance to work on.
 * @param [in]  CrcSeedVal      Initial seed value for the CRC operation that is about to take place.
 *
 * @return      Status
 *              - #ADI_CRC_SUCCESS: Successfully set CRC seed value.
 *              - #ADI_CRC_BAD_HANDLE [D]: Supplied CRC handle is invalid.
 *              - #ADI_CRC_FN_NOT_PERMITTED [D] : Function not permitted when CRC operation is in progress.
 *
 */
ADI_CRC_RESULT adi_crc_SetCrcSeedVal(
    ADI_CRC_HANDLE const    hDevice,
    uint32_t                CrcSeedVal)
{
    ADI_CRC_RESULT result = ADI_CRC_SUCCESS;
    ADI_CRC_DEVICE *pDevice = HDL_TO_DEVICE_PTR(hDevice);

#ifdef ADI_DEBUG
    if (NULL == pDevice)                                /* IF (CRC device handle is invalid) */
    {
        result = ADI_CRC_BAD_HANDLE;
    }
    else if (!ADI_CRC_DEVICE_IS_IDLE(pDevice))          /* IF (CRC in progress) */
    {
        result = ADI_CRC_FN_NOT_PERMITTED;              /* function not permitted when CRC operation is in progress */
    }
    else
#endif /* ADI_DEBUG */
    {
        pDevice->pReg->RESULT = CrcSeedVal;             /* Load the CRC seed value */
    }
    return result;
}

/**
 * @brief       Sets the 32-bit polynomial for CRC operations.
 *
 * @param [in]  hDevice         Handle to CRC device instance to work on.
 * @param [in]  PolynomialVal   32-bit CRC polynomial to use for CRC operation.
 *
 * @return      Status
 *              - #ADI_CRC_SUCCESS: Successfully set polynomial value.
 *              - #ADI_CRC_BAD_HANDLE [D]: Supplied CRC handle is invalid.
 *              - #ADI_CRC_FN_NOT_PERMITTED [D]: Function not permitted when CRC operation is in progress.
 *
 */
ADI_CRC_RESULT adi_crc_SetPolynomialVal(
    ADI_CRC_HANDLE const    hDevice,
    uint32_t                PolynomialVal)
{
    ADI_CRC_RESULT result = ADI_CRC_SUCCESS;
    ADI_CRC_DEVICE *pDevice = HDL_TO_DEVICE_PTR(hDevice);

#ifdef ADI_DEBUG
    if (NULL == pDevice)                                /* IF (CRC device handle is invalid) */
    {
        result = ADI_CRC_BAD_HANDLE;
    }
    else if (!ADI_CRC_DEVICE_IS_IDLE(pDevice))          /* IF (CRC in progress) */
    {
        result = ADI_CRC_FN_NOT_PERMITTED;              /* function not permitted when CRC operation is in progress */
    }
    else
#endif /* ADI_DEBUG */
    {
        pDevice->pReg->POLY = PolynomialVal;            /* Load Polynomial value */
    }
    return result;
}

/**
 * @brief       Submits data buffer for CRC computation
 *
 * @details     This API can be used to submit data buffer for CRC computation.
 *              If NumBits is in [0..7] then the number of bytes to be processed
 *              is NumBytes plus one partial byte containing NumBits bits.
 *              If DMA mode of operation is selected, buffer is processed using
 *              the specified DMA channel.
 *
 * @param [in]  hDevice         Handle of CRC device
 * @param [in]  pCrcBuf         Address of CRC data buffer
 * @param [in]  NumBytes        Number of whole bytes in CRC data buffer
 * @param [in]  NumBits         Number of bits, 0 to 7, in the last partial byte
 *                              in CRC data buffer
 *
 * @return      Status
 *              - #ADI_CRC_SUCCESS: Successfully submitted data buffer.
 *              - #ADI_CRC_INVALID_PARAMETER [D]: one of the parameter used is invalid.
 *              - #ADI_CRC_BAD_HANDLE [D]: Supplied CRC handle is invalid.
 *              - #ADI_CRC_FN_NOT_SUPPORTED [D]: Function not supported by this CRC revision.
 *              - #ADI_CRC_FN_NOT_PERMITTED [D]: Function not permitted when CRC operation is in progress.
 *              - #ADI_CRC_INVALID_DMA_CHANNEL: DMA channel cannot be used with CRC (from crc_DmaDrivenOperation)
 */
ADI_CRC_RESULT adi_crc_Compute(
    ADI_CRC_HANDLE const    hDevice,
    void                    *pCrcBuf,
    uint32_t                NumBytes,
    uint32_t                NumBits)
{
    ADI_CRC_RESULT result = ADI_CRC_SUCCESS;
    ADI_CRC_DEVICE *pDevice = HDL_TO_DEVICE_PTR(hDevice);
#ifdef ADI_DEBUG
    if (NumBits >= 8u)
    {
        result = ADI_CRC_INVALID_PARAMETER;
    }
    else if (NULL == pDevice)
    {
        result = ADI_CRC_BAD_HANDLE;
    }
    else if (((pDevice->pReg->CTL & BITM_CRC_CTL_REVID) == 0u) && (NumBits != 0u))
    {
        result = ADI_CRC_FN_NOT_SUPPORTED;      /* Partial byte needs CRC unit revision 1 or up */
    }
    else
    if (!ADI_CRC_DEVICE_IS_IDLE(pDevice))       /* IF (CRC in progress) */
    {
        result = ADI_CRC_FN_NOT_PERMITTED;      /* function not permitted when CRC operation is in progress */
    }
    else
#endif /* ADI_DEBUG */
    {
        pDevice->eCrcOpStatus = ADI_CRC_OP_IN_PROGRESS; /* mark the CRC as in progress */
        result = pDevice->pfSubmitBuffer(pDevice, pCrcBuf, NumBytes, NumBits);

        /* CRC returns in IDLE mode when it has processed all its data, not after submitting a request */
    }
    return result;
}

/**
 * @brief       Gets the current CRC peripheral status.
 *
 * @param [in]  hDevice         Handle to CRC device instance to work on
 * @param [in]  pbCrcInProgress Pointer to location to store the current status of CRC peripheral.
 *                              'true' when CRC peripheral is in currently performing a CRC operation.
 *
 * @return      Status
 *              - #ADI_CRC_SUCCESS: Successfully set expected CRC result.
 *              - #ADI_CRC_BAD_HANDLE [D}: Supplied CRC handle is invalid.
 *
 * @note        This function is valid only when device is operating in DMA mode.
 *
 */
ADI_CRC_RESULT adi_crc_IsCrcInProgress(
    ADI_CRC_HANDLE const    hDevice,
    bool                    *pbCrcInProgress)
{
    ADI_CRC_RESULT result = ADI_CRC_SUCCESS;
    ADI_CRC_DEVICE *pDevice = HDL_TO_DEVICE_PTR(hDevice);

#ifdef ADI_DEBUG
    if (NULL == pDevice)        /* IF (CRC device handle is invalid) */
    {
        result = ADI_CRC_BAD_HANDLE;
    }
    else
#endif /* ADI_DEBUG */
    {

      if ((pDevice)->eCrcOpStatus == ADI_CRC_OP_IN_PROGRESS) 
      {
                *pbCrcInProgress = true;

      } 
      else 
      {
                *pbCrcInProgress = false;

      }
    }
    return result;
}

/**
 * @brief       Gets the final CRC result computed for a data stream
 *
 * @details     This API gets the final CRC result computed for a data stream
 *              and clears the current and final CRC results register.
 *              The CRC Current result register holds the current or
 *              intermediate CRC result. Whenever a CRC operation is initiated,
 *              the CRC peripheral takes the CRC Current register value as
 *              initial seed for CRC computation. This API clears both results
 *              register to start a fresh CRC computation.
 *              Use the adi_crc_GetCurrentCrcVal() API to get an intermediate
 *              CRC result without clearing the results register.
 *
 * @param [in]  hDevice         Handle to CRC device instance to work on
 * @param [out] pFinalCrcVal    Pointer to location where the final CRC result of
 *                              a data stream to be processed will be written.
 *
 * @return      Status
 *              - #ADI_CRC_SUCCESS: Successfully read final CRC result.
 *              - #ADI_CRC_BAD_HANDLE [D]: Supplied CRC handle is invalid.
 */
ADI_CRC_RESULT adi_crc_GetFinalCrcVal(
    ADI_CRC_HANDLE const    hDevice,
    uint32_t               *pFinalCrcVal)
{
    ADI_CRC_RESULT result = ADI_CRC_SUCCESS;
    ADI_CRC_DEVICE *pDevice = HDL_TO_DEVICE_PTR(hDevice);

#ifdef ADI_DEBUG
    if (NULL == pDevice)        /* IF (CRC device handle is invalid) */
    {
        result = ADI_CRC_BAD_HANDLE;
    }
    else
#endif /* ADI_DEBUG */
    {
        const uint32_t seed_value = (uint32_t) ADI_CFG_CRC_SEED_VALUE;
        *pFinalCrcVal = pDevice->pReg->RESULT;  /* Get the final CRC result */
        pDevice->pReg->RESULT = seed_value;
    }
    return result;
}

/**
 * @brief       Gets the current/intermediate CRC result computed for a data stream.
 *
 * @param [in]  hDevice           Handle to CRC device instance to work on
 * @param [out] pCurrentCrcVal    Pointer to location where the intermediate CRC result of
 *                                a data stream to be processed will be written.
 *
 * @return      Status
 *              - #ADI_CRC_SUCCESS: Successfully read current CRC result.
 *              - #ADI_CRC_BAD_HANDLE [D]: Supplied CRC handle is invalid.
 */
ADI_CRC_RESULT adi_crc_GetCurrentCrcVal(
    ADI_CRC_HANDLE const    hDevice,
    uint32_t                *pCurrentCrcVal)
{
    ADI_CRC_RESULT result = ADI_CRC_SUCCESS;
    ADI_CRC_DEVICE *pDevice = HDL_TO_DEVICE_PTR(hDevice);

#ifdef ADI_DEBUG
    if (NULL == pDevice)                                /* IF (CRC device handle is invalid) */
    {
        result = ADI_CRC_BAD_HANDLE;
    }
    else
#endif /* ADI_DEBUG */
    {
        *pCurrentCrcVal = pDevice->pReg->RESULT;        /* Get the current CRC result */
    }

    return result;
}

/**
 * @brief       Registers or unregisters a callback with the CRC device
 *
 * @details     It is not required to register a callback for the operation of the
 *              driver. Data compare or DMA error will be notified via the
 *              adi_crc_IsCrcInProgress() API. But if an application requires the
 *              errors/events to be notified immediately it can register a callback
 *              with the driver which will be called to notify errors/events.
 *
 *              When a callback is registered the API adi_crc_IsCrcInProgress()
 *              will not return error.
 *
 * @param  [in]  hDevice        Handle to CRC device instance to work on
 * @param  [in]  pfCallback     Pointer to application callback function. The callback function
 *                              has the prototype
 *                              void callback(void *pCBParam, uint32_t nEvent, void *pArg)
 *                              To unregister a callback pass the the pointer to the callback
 *                              function as NULL.
 * @param  [in]  pCBParam       Callback parameter which will be returned back to the
 *                              application when the callback function is called.
 *
 * @return      Status
 *              - #ADI_CRC_SUCCESS: Successfully registered callback.
 *              - #ADI_CRC_BAD_HANDLE [D]: Supplied CRC handle is invalid.
 */
ADI_CRC_RESULT adi_crc_RegisterCallback(
    ADI_CRC_HANDLE const    hDevice,
    ADI_CALLBACK            pfCallback,
    void *const             pCBParam)
{
    ADI_CRC_RESULT result = ADI_CRC_SUCCESS;
    ADI_CRC_DEVICE *pDevice = HDL_TO_DEVICE_PTR(hDevice);

    ADI_INT_STATUS_ALLOC();
    ADI_ENTER_CRITICAL_REGION();                /* Entering critical region, disable interrupts */

#ifdef ADI_DEBUG
    if (NULL == pDevice)                        /* IF (CRC device handle is invalid) */
    {
        result = ADI_CRC_BAD_HANDLE;
    }
    else
#endif /* ADI_DEBUG */
    {
        /* Update CRC Callback information */
        pDevice->pfCallback = pfCallback;
        pDevice->pCBParam   = pCBParam;
    }

    ADI_EXIT_CRITICAL_REGION();                 /* Re-enable interrupts */

    return result;
}


/*****/

/*@}*/
