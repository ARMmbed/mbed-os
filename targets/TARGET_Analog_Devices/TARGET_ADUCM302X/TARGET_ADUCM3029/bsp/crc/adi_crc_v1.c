/*! *****************************************************************************
 * @file:    adi_crc_v1.c
 * @brief:   CRC device  driver global file.
 * @details: This file contain the CRC device driver impelementation.
 * @version: $Revision:
 * @date:    $Date:
 -----------------------------------------------------------------------------
Copyright (c) 2010-2015 Analog Devices, Inc.

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
/** @addtogroup  CRC_Driver CRC Device Driver
 *  @{

<b>Cyclic Redundancy Check (CRC) peripheral driver</b>

The CRC peripheral is used to perform the Cyclic Redundancy Check (CRC) of the
block of data that is presented to the peripheral. The peripheral provides a
means to periodically verify the integrity of the system memory and it is
based on a CRC32 engine that computes the signature of 32-bit data presented
to the peripheral.

<b>Memory DMA driver CRC operations</b>

The CRC module can use DMA stream to perform its operations.
The CRC DMA mode can be enabled using adi_crc_EnableDmaMode() API.

<em><b>Computing CRC</b></em>

The CRC engine also performsa 32-bit CRC operation on the incoming data stream.

Sequence of function calls for Computing CRC :

    - adi_crc_Open() to open CRC device.
    - adi_crc_SetPolynomialVal() sets the polynomial value to be used for CRC operation.
    - adi_crc_EnableDmaMode() enables Memory DMA for CRC.
    - adi_crc_IsCrcInProgress() to poll the current status of CRC operation or
      wait for callback event.
    - adi_crc_GetFinalCrcVal() to get the CRC value of the data stream if its
      CRC value is unknown.
 */

#ifndef _ADI_CRC_V1_C_
/*! \cond PRIVATE */
#define _ADI_CRC_V1_C_

/*=============  I N C L U D E S   =============*/

#include <drivers/crc/adi_crc.h>
#include "adi_crc_def.h"

/*=============  M I S R A  =============*/

#ifdef __ICCARM__
/*
* IAR MISRA C 2004 error suppressions.
*
* Pm123 (rule 8.5): there shall be no definition of objects or functions in a header file
*   This isn't a header as such.
*
* Pm073 (rule 14.7): a function should have a single point of exit
* Pm143 (rule 14.7): a function should have a single point of exit at the end of the function
*   Multiple returns are used for error handling.
*
* Pm088 (rule 17.4): pointer arithmetic should not be used.
* Pm152 (rule 17.4): array indexing shall only be applied to objects defined as an array type
*   Relying on pointer arithmetic for buffer handling.
*
* Pm140 (rule 11.3): a cast should not be performed between a pointer type and an integral type
*   Casts from pointer to uint32_t needed to determine pointer alignment.
*/
#pragma diag_suppress=Pm123,Pm073,Pm143,Pm088,Pm152,Pm140
#endif /* __ICCARM__ */

/*==============  D A T A  ===============*/

#include "adi_crc_data.c"

/*==============  D E F I N E S  ===============*/


/*=============  D E B U G    F U N C T I O N    P R O T O T Y P E S  =============*/

/* IF (Debug information enabled) */
#if defined (ADI_DEBUG)

/* Validates CRC device handle */
static ADI_CRC_RESULT ValidateCrcHandle (ADI_CRC_HANDLE const  hDevice);

#endif /* ADI_DEBUG */

/*=============  C O D E  =============*/

/*=============  L O C A L   F U N C T I O N S =============*/

/* Prototypes for static functions (required by MISRA-C:2004 Rule 8.1)  */
static ADI_CRC_DEVICE *CrcDevicePointer(ADI_CRC_HANDLE hDevice);
static ADI_CRC_INFO *CrcDeviceInfo(ADI_CRC_HANDLE hDevice);

static void InitCRC (ADI_CRC_DEVICE *pDevice);
#if (ADI_CRC_CFG_ENABLE_DMA_SUPPORT == 1)
static ADI_CRC_RESULT StartMemDmaCrc (ADI_CRC_DEVICE *pDevice, void *pCrcBuf, uint32_t NumBytes, uint32_t NumBits);
static void DMA_CRC_Int_Handler(void *pcbparam, uint32_t nEvent, void *pArg);
static void CalculateCrcForRemaining(ADI_CRC_DEVICE *pDevice, uint8_t *pData, uint32_t NumBytes, uint32_t NumBits);
#else
static ADI_CRC_RESULT CalculateCrc (ADI_CRC_DEVICE *pDevice, void *pCrcBuf, uint32_t NumBytes, uint32_t NumBits);
#endif

static ADI_CRC_DEVICE *CrcDevicePointer(ADI_CRC_HANDLE hDevice)
{
#if defined (ADI_DEBUG)
    if (ValidateCrcHandle (hDevice) != ADI_CRC_SUCCESS)
    {
        return NULL;
    }
    else
#endif /* ADI_DEBUG */
    {
        /* In current implementation, we use device pointer as device handle. */
        return (ADI_CRC_DEVICE *)hDevice;
    }
}

static ADI_CRC_INFO *CrcDeviceInfo(ADI_CRC_HANDLE hDevice)
{
    ADI_CRC_INFO *pCrcInfo = NULL;
    uint32_t i;

    /* FOR (All CRC Device instances) */
    for (i = 0u; i < (sizeof (CrcDevInfo) / sizeof (CrcDevInfo[0])); i++)
    {
        /* IF (CRC handle is valid) */
        if (hDevice == CrcDevInfo[i].hDevice)
        {
            pCrcInfo = &(CrcDevInfo[i]);
            break;
        }
    }

    return pCrcInfo;
}
#if (ADI_CRC_CFG_ENABLE_DMA_SUPPORT ==1)
static void CalculateCrcForRemaining(ADI_CRC_DEVICE *pDevice, uint8_t *pData, uint32_t NumBytes, uint32_t NumBits)
{
    while (NumBytes > 0u)
    {
        pDevice->pReg->IPBYTE = *pData;
        pData++;
        NumBytes--;
    }

    if (NumBits > 0u)
    {
        pDevice->pReg->IPBITS[NumBits] = *pData;
    }
}

static void DMA_CRC_Int_Handler(void *pcbparam, uint32_t nEvent, void *pArg)
{
    ADI_CRC_DEVICE *pDevice = (ADI_CRC_DEVICE *)pcbparam;
    uint8_t *pData = (uint8_t *)(pDevice->pBuffer);
    uint32_t NumBytes = pDevice->RemainingBytes;
    uint32_t NumBits = pDevice->RemainingBits;

    CalculateCrcForRemaining(pDevice, pData, NumBytes, NumBits);

    if((pDevice->pfCallback != NULL) && (nEvent == ADI_DMA_EVENT_BUFFER_PROCESSED))
    {
        pDevice->pfCallback(pDevice->pCBParam, (uint32_t) ADI_CRC_EVENT_BUFFER_PROCESSED, pArg);
    }
    pDevice->eCrcOpStatus = ADI_CRC_OP_IDLE;
    pDevice->pReg->CTL &= (uint32_t)(~(BITM_CRC_CTL_EN)) ;

}
#endif
/*
 * Resets CRC registers.
 *
 * Parameters
 *  - [in]  pDevice    Pointer to CRC Device
 *
 * Returns: None
 *
 */
static void InitCRC (ADI_CRC_DEVICE *pDevice)
{

    /* Reset CRC registers to default */
    pDevice->pReg->CTL = (uint32_t)ADI_CFG_CRC_ENABLE_BYTE_MIRRORING << BITP_CRC_CTL_BYTMIRR |
                         (uint32_t)ADI_CFG_CRC_ENABLE_BIT_MIRRORING << BITP_CRC_CTL_BITMIRR;
#if (ADI_CRC_CFG_ENABLE_STATIC_CONFIG_SUPPORT == 1)
    pDevice->pReg->RESULT = ADI_CFG_CRC_SEED_VALUE;
    pDevice->pReg->POLY   = ADI_CFG_CRC_POLYNOMIAL;
#endif
}
#if (ADI_CRC_CFG_ENABLE_DMA_SUPPORT == 1)
/*
 * Starts Memory DMA driven CRC operation.
 *
 * Parameters:
 *  - [in]  pDevice             Pointer to CRC device
 *  - [in]  pCrcBuf             Address of data buffer.
 *  - [in]  NumBytes            Number of whole bytes in data buffer.
 *  - [in]  NumBits             Number of bits in data buffer.
 *
 * return  Status
 *  - ADI_CRC_SUCCESS: Successfully set expected CRC result.
 *  - ADI_CRC_BAD_HANDLE: Supplied CRC handle is invalid.
 *
 */
static ADI_CRC_RESULT StartMemDmaCrc(
    ADI_CRC_DEVICE      *pDevice,
    void                *pCrcBuf,
    uint32_t            NumBytes,
    uint32_t            NumBits)
{
    uint8_t *pData = (uint8_t *)pCrcBuf;
    bool_t bUseDma = false;

#if defined (ADI_DEBUG)
    /* IF (MDMA Disabled) */
    if ((uint32_t)pDevice->eDMAChannel == CRC_INVALID_DMA_CHANNEL)
    {
        /* Report error as Memory DMA not open */
        return (ADI_CRC_INVALID_DMA_CHANNEL);
    }
#endif /* ADI_DEBUG */
    /* Device is calculating the CRC for previously submitted buffer */
    if(pDevice->eCrcOpStatus == ADI_CRC_OP_IN_PROGRESS)
    {
        return(ADI_CRC_IN_USE);
    }

    /* If LSBFIRST, it's easy. */
    if ((pDevice->pReg->CTL & BITM_CRC_CTL_LSBFIRST) != 0u)
    {
        /* If the buffer is not 4-byte aligned */
        if (((uint32_t)pData & 0x3u) != 0u)
        {
            pDevice->pReg->CTL |= (BITM_CRC_CTL_EN);
            while ((NumBytes > 0u) && (((uint32_t)pData & 0x3u) != 0u))
            {
                pDevice->pReg->IPBYTE = *pData;
                pData++;
                NumBytes--;
            }
            pDevice->pReg->CTL &= ~(BITM_CRC_CTL_EN);
        }

        if (NumBytes >= 4u)
        {
            pDevice->gDmaDescriptor.DataWidth = ADI_DMA_WIDTH_4_BYTE;
            pDevice->gDmaDescriptor.pSrcData = pData;
            pDevice->gDmaDescriptor.pDstData = &pDevice->pReg->IPDATA;
            pDevice->gDmaDescriptor.SrcInc = ADI_DMA_INCR_4_BYTE;
            pDevice->gDmaDescriptor.DstInc = ADI_DMA_INCR_NONE;
            pDevice->gDmaDescriptor.NumTransfers = NumBytes / 4u;
            pDevice->RemainingBytes = NumBytes % 4u;
            pDevice->RemainingBits = NumBits;
            pDevice->pBuffer = pData + NumBytes / 4u * 4u;
            bUseDma = true;
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
            pDevice->gDmaDescriptor.DataWidth = ADI_DMA_WIDTH_1_BYTE;
            pDevice->gDmaDescriptor.pSrcData = pData;
            pDevice->gDmaDescriptor.pDstData = &pDevice->pReg->IPBYTE;
            pDevice->gDmaDescriptor.SrcInc = ADI_DMA_INCR_1_BYTE;
            pDevice->gDmaDescriptor.DstInc = ADI_DMA_INCR_NONE;
            pDevice->gDmaDescriptor.NumTransfers = NumBytes;
            pDevice->RemainingBytes = 0u;
            pDevice->RemainingBits = NumBits;
            pDevice->pBuffer = pData + NumBytes;
            bUseDma = true;
        }
    }

    if ((bUseDma== true) && (NumBytes >= 4u))
    {
        pDevice->pReg->CTL |= (BITM_CRC_CTL_EN) ;      
        if(adi_dma_SubmitTransfer(pDevice->hDma, &pDevice->gDmaDescriptor) != ADI_DMA_SUCCESS)
        {
            return(ADI_CRC_DMA_ERROR);
        }
        pDevice->eCrcOpStatus = ADI_CRC_OP_IN_PROGRESS;
        if(adi_dma_Enable(pDevice->hDma, true) != ADI_DMA_SUCCESS)
        {
            return(ADI_CRC_DMA_ERROR);
        }
    }
    else
    {
        pDevice->pReg->CTL |= (BITM_CRC_CTL_EN);
        CalculateCrcForRemaining(pDevice, pData, NumBytes, NumBits);
        pDevice->pReg->CTL &= ~(BITM_CRC_CTL_EN);
        if(pDevice->pfCallback != NULL)
        {
            pDevice->pfCallback(pDevice->pCBParam, (uint32_t) ADI_CRC_EVENT_BUFFER_PROCESSED, pData);
        }
        
    }

    /* Return */
    return (ADI_CRC_SUCCESS);
}
#else

/*
 * Starts Memory DMA driven CRC operation.
 *
 * Parameters:
 *  - [in]  pDevice             Pointer to CRC device
 *  - [in]  pCrcBuf             Address of data buffer.
 *  - [in]  NumBytes            Number of bytes in data buffer.
 *  - [in]  NumBits             Number of bits in the last partial byte.
 *
 * return  Status
 *  - ADI_CRC_SUCCESS: Successfully set expected CRC result.
 *  - ADI_CRC_BAD_HANDLE: Supplied CRC handle is invalid.
 *
 */
static ADI_CRC_RESULT CalculateCrc(
    ADI_CRC_DEVICE      *pDevice,
    void                *pCrcBuf,
    uint32_t            NumBytes,
    uint32_t            NumBits)
{
    uint8_t *pData = (uint8_t *)pCrcBuf;
    uint32_t lsbFirst = pDevice->pReg->CTL & BITM_CRC_CTL_LSBFIRST;

    pDevice->pReg->CTL |= (BITM_CRC_CTL_EN);

    /* If the buffer is not 4-byte aligned */
    if (((uint32_t)pData & 0x3u) != 0u)
    {
        while ((NumBytes > 0u) && (((uint32_t)pData & 0x3u) != 0u))
        {
            pDevice->pReg->IPBYTE = *pData;
            pData++;
            NumBytes--;
        }
    }

    while (NumBytes >= 4u)
    {
        uint32_t nData;

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
        pDevice->pReg->IPDATA = nData;
        pData += 4;
        NumBytes -= 4u;
    }

    while (NumBytes > 0u)
    {
        pDevice->pReg->IPBYTE = *pData;
        pData++;
        NumBytes--;
    }

    if (NumBits > 0u)
    {
        pDevice->pReg->IPBITS[NumBits] = *pData;
    }

    pDevice->pReg->CTL &= ~(BITM_CRC_CTL_EN);
    return (ADI_CRC_SUCCESS);
}

#endif
/*=============  D E B U G   F U N C T I O N S =============*/

/* IF (Debug information enabled) */
#if defined (ADI_DEBUG)

/* Validates CRC device handle */
static ADI_CRC_RESULT ValidateCrcHandle (ADI_CRC_HANDLE const  hDevice)
{
    /* Loop variable */
    uint32_t    i;
    /* FOR (All CRC Device instances) */
    for (i = 0u; i < (sizeof (CrcDevInfo) / sizeof (CrcDevInfo[0])); i++)
    {
        /* IF (CRC handle is valid) */
        if (hDevice == CrcDevInfo[i].hDevice)
        {
            /* Channel valid, return success */
            return (ADI_CRC_SUCCESS);
        }
    }

    /* Device ID invalid, return failure */
    return (ADI_CRC_FAILURE);
}

#endif /* ADI_DEBUG */
/*! \endcond */
/*=============  P U B L I C   F U N C T I O N S =============*/

/**
 * @brief   Opens a CRC device instance.
 *
 * @param [in]  DeviceNum       Device number to open.
 * @param [in]  pMemory         Pointer to a #ADI_CRC_MEMORY_SIZE.
 *                              sized buffer to manage the device instance.
 * @param [in]  MemorySize      Size of the buffer to which "pMemory" points.
 * @param [out] phDevice        Pointer to a location where CRC device handle to be written.
 *
 * @return  Status
 *          - #ADI_CRC_SUCCESS: Successfully opened a CRC device.
 *          - #ADI_CRC_IN_USE[D]: Supplied CRC Device ID is already in use.
 *          - #ADI_CRC_BAD_DEVICE_NUMBER[D]: Supplied CRC Device ID is invalid.
 *          - #ADI_CRC_INSUFFICIENT_MEMORY[D]: Supplied memory is not sufficient to handle a CRC device instance.
 *
 * @note
 *      For  the device memory should be of size #ADI_CRC_MEMORY_SIZE.
 *
 */
ADI_CRC_RESULT adi_crc_Open(
	uint32_t            DeviceNum,
	void                *pMemory,
	uint32_t            MemorySize,
	ADI_CRC_HANDLE      *phDevice)
{
    ADI_CRC_DEVICE *pDevice = pMemory;
/* IF (Debug information enabled) */
#if defined (ADI_DEBUG)

    /* IF (This is not a valid device number) */
    if (DeviceNum > (sizeof (CrcDevInfo)/sizeof(CrcDevInfo[0])))
    {
        /* Report failure as bad device number */
        return (ADI_CRC_BAD_DEVICE_NUMBER);
    }

    /* IF (Supplied memory size is insufficient) */
    if (MemorySize < ADI_CRC_MEMORY_SIZE)
    {
        /* Report failure as insufficient memory */
        return (ADI_CRC_INSUFFICIENT_MEMORY);
    }

    /* Asserts to validate minimum memory size required to manage a Core driven CRC device */
    assert(ADI_CRC_MEMORY_SIZE == sizeof(ADI_CRC_DEVICE));

    /* IF (The device is in use) */
    if (CrcDevInfo[DeviceNum].hDevice != NULL)
    {
        /* CRC Device is in use */
        return (ADI_CRC_IN_USE);
    }

#endif  /* ADI_DEBUG */

    /* Clear the given memory */
    memset(pMemory, 0, MemorySize);

    /* Entering critical region, disable interrupts */
    ADI_ENTER_CRITICAL_REGION();

    /* Save the supplied device memory address */
    CrcDevInfo[DeviceNum].hDevice = (ADI_CRC_HANDLE)pDevice;
    pDevice->pReg = CrcDevInfo[DeviceNum].pReg;
    /* Re-enable interrupts */
    ADI_EXIT_CRITICAL_REGION();

    /* Reset CRC registers */
    InitCRC(pDevice);
    /* Pass a valid handle to this CRC device */
    *phDevice = CrcDevInfo[DeviceNum].hDevice;
#if (ADI_CRC_CFG_ENABLE_DMA_SUPPORT ==1 )
    pDevice->eDMAChannel =  ADI_CFG_CRC_DMA_CHANNEL;
#if (ADI_CRC_CFG_ENABLE_DMA == 1)
    adi_crc_EnableDmaMode(*phDevice,true);
#endif

#else
    pDevice->pfSubmitBuffer = &CalculateCrc;
#endif
    /* return */
    return (ADI_CRC_SUCCESS);
}

/**
 * @brief   Closes CRC device instance opened for use.
 *
 * @param [in]  hDevice     Handle to CRC Device instance to close.
 *
 * @return  Status
 *          - #ADI_CRC_SUCCESS: Successfully closed CRC device.
 *          - #ADI_CRC_BAD_HANDLE [D]: Supplied CRC handle is invalid.
 *          - #ADI_CRC_NOT_OPEN [D]: CRC device is not in use.
 *
 */
ADI_CRC_RESULT adi_crc_Close(ADI_CRC_HANDLE const    hDevice)
{
    ADI_CRC_INFO *pCrcInfo = CrcDeviceInfo(hDevice);
    ADI_CRC_DEVICE *pDevice;

    if (pCrcInfo == NULL)
    {
        return (ADI_CRC_BAD_HANDLE);
    }

    pDevice = (ADI_CRC_DEVICE *)hDevice;

    /* Reset CRC registers */
    InitCRC (pDevice);

#if (ADI_CRC_CFG_ENABLE_DMA_SUPPORT == 1)
    if (pDevice->hDma != NULL)
    {
        if (adi_dma_Close(pDevice->hDma) != ADI_DMA_SUCCESS)
        {
            return (ADI_CRC_DMA_ERROR);
        }
    }
#endif
    
    /* Successfully un-registered CRC interrupt handlers */
    pCrcInfo->hDevice = NULL;

    /* Return success */
    return (ADI_CRC_SUCCESS);
}
/*!
 * @brief  Set the bit mirroring. This function should be called only when device is disabled.
 *
 * @param[in]   hDevice     Device handle obtained from adi_crc_Open().
 * @param[in]   bEnable     Boolean flag to enable/disable bit mirroring.
 * \n
 * \n                       true : To Enable bit mirroring.
 * \n
 * \n                       false :  To Disable bit mirroring.
 * \n
 * @return      Status
 *                - #ADI_CRC_BAD_HANDLE [D]     Invalid device handle parameter.
 *                - #ADI_CRC_NOT_OPEN [D]    Device has not been previously configured for use.
 *                - #ADI_CRC_SUCCESS                    Call completed successfully.
 *
 *
 * @sa        adi_crc_SetBytemirroring().
 * @sa        adi_crc_SetWordSwap().
 */
ADI_CRC_RESULT  adi_crc_SetBitMirroring(ADI_CRC_HANDLE const    hDevice,const bool_t bEnable)
{
    ADI_CRC_DEVICE *pDevice = CrcDevicePointer(hDevice);

#ifdef ADI_DEBUG
    /* IF (CRC device handle is invalid) */
    if (pDevice == NULL)
    {
        return (ADI_CRC_BAD_HANDLE);
    }

    /* IF (CRC in progress) */
    if (pDevice->eCrcOpStatus != ADI_CRC_OP_IDLE)
    {
        /* Report error as this function is not permitted when CRC operation is in progress */
        return (ADI_CRC_FN_NOT_PERMITTED);
    }

#endif
    if(bEnable == true)
    {
        pDevice->pReg->CTL |= (BITM_CRC_CTL_BITMIRR);
    }
    else
    {
        pDevice->pReg->CTL  &=  (uint32_t)(~(BITM_CRC_CTL_BITMIRR));
    }
    return ADI_CRC_SUCCESS;
}
/*!
 * @brief  Set the byte mirroring.This function should be called only when device is disabled.
 *
 * @param[in]   hDevice    Device handle obtained from adi_crc_Open().
 * @param[in]   bEnable      Boolean flag to enable/disable byte mirroring.
 * \n
 * \n                      true : To Enable byte mirroring.
 * \n
 * \n                      false :  To Disable byte mirroring.
 * \n
 * @return      Status
 *                - #ADI_CRC_BAD_HANDLE [D]     Invalid device handle parameter.
 *                - #ADI_CRC_NOT_OPEN [D]    Device has not been previously configured for use.
 *                - #ADI_CRC_SUCCESS                    Call completed successfully.
 *
 *
 * @sa        adi_crc_EnableBitmirroring().
 * @sa        adi_crc_EnableWordSwap().
 */
ADI_CRC_RESULT  adi_crc_SetByteMirroring(ADI_CRC_HANDLE const    hDevice,const bool_t bEnable)
{
    ADI_CRC_DEVICE *pDevice = CrcDevicePointer(hDevice);

#ifdef ADI_DEBUG
    /* IF (CRC device handle is invalid) */
    if (pDevice == NULL)
    {
        return (ADI_CRC_BAD_HANDLE);
    }

    /* IF (CRC in progress) */
    if (pDevice->eCrcOpStatus != ADI_CRC_OP_IDLE)
    {
        /* Report error as this function is not permitted when CRC operation is in progress */
        return (ADI_CRC_FN_NOT_PERMITTED);
    }

#endif
    if(bEnable == true)
    {
       pDevice->pReg->CTL |= (BITM_CRC_CTL_BYTMIRR);
    }
    else
    {
       pDevice->pReg->CTL  &=  (uint32_t)(~(BITM_CRC_CTL_BYTMIRR));
    }
    return ADI_CRC_SUCCESS;
}

/*!
 * @brief  Enable  the LSB first  .
 *
 * @param[in]   hDevice    Device handle obtained from adi_crc_Open().
 * @param[in]   bEnable      Boolean flag  which indicate whether LSB first OR MSB first for CRC calculation.
 * \n
 * \n                      true : For LSB First CRC calculation
 * \n
 * \n                      false :  For MSB First CRC calculation
 * \n
 * @return      Status
 *                - #ADI_CRC_BAD_HANDLE [D]     Invalid device handle parameter.
 *                - #ADI_CRC_NOT_OPEN [D]    Device has not been previously configured for use.
 *                - #ADI_CRC_SUCCESS                    Call completed successfully.
 *
 *
 * @sa        adi_crc_EnableBitmirroring().
 * @sa        adi_crc_EnableWordSwap().
 */

ADI_CRC_RESULT  adi_crc_SetLSBFirst(ADI_CRC_HANDLE const    hDevice,const bool_t bEnable)
{
    ADI_CRC_DEVICE *pDevice = CrcDevicePointer(hDevice);

#ifdef ADI_DEBUG
    /* IF (CRC device handle is invalid) */
    if (pDevice == NULL)
    {
        return (ADI_CRC_BAD_HANDLE);
    }

    /* IF (CRC in progress) */
    if (pDevice->eCrcOpStatus != ADI_CRC_OP_IDLE)
    {
        /* Report error as this function is not permitted when CRC operation is in progress */
        return (ADI_CRC_FN_NOT_PERMITTED);
    }

#endif
    if(bEnable == true)
    {
       pDevice->pReg->CTL |= (BITM_CRC_CTL_LSBFIRST);
    }
    else
    {
       pDevice->pReg->CTL  &= ~(BITM_CRC_CTL_LSBFIRST);
    }
    return ADI_CRC_SUCCESS;
}
/*!
 * @brief  To enable/disable the word Swap.This function should be called only when device is disabled.
 *
 * @param[in]   hDevice     Device handle obtained from adi_crc_Open().
 * @param[in]   bEnable    Boolean flag to enable/disable word swap.
 * \n
 * \n                      true : To Enable word swap.
 * \n
 * \n                      false :  To Disable word swap.
 * \n
 * @return      Status
 *                - #ADI_CRC_BAD_HANDLE [D]     Invalid device handle parameter.
 *                - #ADI_CRC_NOT_OPEN [D]    Device has not been previously configured for use.
 *                - #ADI_CRC_SUCCESS                    Call completed successfully.
 *
 *
 * @sa        adi_crc_SetBitMirroring().
 * @sa        adi_crc_SetByteMirroring().
 */
ADI_CRC_RESULT  adi_crc_EnableWordSwap(ADI_CRC_HANDLE const    hDevice,const bool_t bEnable)
{
    ADI_CRC_DEVICE *pDevice = CrcDevicePointer(hDevice);

#ifdef ADI_DEBUG
    /* IF (CRC device handle is invalid) */
    if (pDevice == NULL)
    {
        return (ADI_CRC_BAD_HANDLE);
    }

    /* IF (CRC in progress) */
    if (pDevice->eCrcOpStatus != ADI_CRC_OP_IDLE)
    {
        /* Report error as this function is not permitted when CRC operation is in progress */
        return (ADI_CRC_FN_NOT_PERMITTED);
    }
#endif
    if(bEnable == true)
    {
       pDevice->pReg->CTL |= BITM_CRC_CTL_W16SWP;
    }
    else
    {
       pDevice->pReg->CTL &= ~BITM_CRC_CTL_W16SWP;
    }


    return ADI_CRC_SUCCESS;
}
/**
 * @brief   Sets the initial seed value for the CRC operation that is about to take place.
 *
 * Parameters:
 *  @param [in]  hDevice         Handle to CRC device instance to work on.
 *  @param [in]  CrcSeedVal      Initial seed value for the CRC operation that is about to take place.
 *
 * @return  Status
 *          - #ADI_CRC_SUCCESS: Successfully set CRC seed value.
 *          - #ADI_CRC_BAD_HANDLE: Supplied CRC handle is invalid.
 *          - #ADI_CRC_FN_NOT_PERMITTED: Function not permitted when CRC operation is in progress.
 *
 */
ADI_CRC_RESULT adi_crc_SetCrcSeedVal(
    ADI_CRC_HANDLE const    hDevice,
    uint32_t                CrcSeedVal)
{
    ADI_CRC_DEVICE *pDevice = CrcDevicePointer(hDevice);

/* IF (Debug information enabled) */
#if defined (ADI_DEBUG)

    /* IF (CRC device handle is invalid) */
    if (pDevice == NULL)
    {
        return (ADI_CRC_BAD_HANDLE);
    }

    /* IF (CRC in progress) */
    if (pDevice->eCrcOpStatus != ADI_CRC_OP_IDLE)
    {
        /* Report error as this function is not permitted when CRC operation is in progress */
        return (ADI_CRC_FN_NOT_PERMITTED);
    }

#endif /* ADI_DEBUG */

    /* Load the CRC seed value */
    pDevice->pReg->RESULT = CrcSeedVal;

    /* Return */
    return (ADI_CRC_SUCCESS);
}

/*! \cond PRIVATE */
#if (ADI_CRC_CFG_ENABLE_DMA_SUPPORT == 1)
/*! \endcond */ 

/**
 * @brief   Enables/Disables DMA driven CRC operation.
 *          To use this function, macro ADI_CRC_CFG_ENABLE_DMA_SUPPORT should be enabled. 
 *
 * Parameters:
 *  @param [in]  hDevice    Handle to CRC device instance to work on.
 *  @param [in]  bEnable    'true' to enable DMA driven CRC, 'false' for core driven CRC.
 *
 * @return  Status
 *          - #ADI_CRC_SUCCESS: Successfully enabled/disabled DMA mode.
 *          - #ADI_CRC_BAD_HANDLE: Supplied CRC handle is invalid.
 *          - #ADI_CRC_FN_NOT_PERMITTED: Function not permitted when CRC operation is in progress.
 *          - #ADI_CRC_DMA_ERROR: Failed to open/close Memory DMA stream.
 *          - #ADI_CRC_INSUFFICIENT_MEMORY: Supplied memory is insufficient to operate CRC device in DMA driven mode.
 *
 */
ADI_CRC_RESULT adi_crc_EnableDmaMode(
    ADI_CRC_HANDLE const    hDevice,
    bool_t                  bEnable)
{
    ADI_CRC_DEVICE *pDevice = CrcDevicePointer(hDevice);

/* IF (Debug information enabled) */
#if defined (ADI_DEBUG)

    /* IF (CRC device handle is invalid) */
    if (pDevice == NULL)
    {
        return (ADI_CRC_BAD_HANDLE);
    }

#endif /* ADI_DEBUG */
    /* IF (Enable MDMA) */
#if (ADI_CRC_CFG_ENABLE_DMA_SUPPORT ==1)
    if (bEnable== true)
    {
        if (adi_dma_Open(pDevice->eDMAChannel, pDevice->mDmaMem, &pDevice->hDma, DMA_CRC_Int_Handler, pDevice) != ADI_DMA_SUCCESS)
        {
            return (ADI_CRC_DMA_ERROR);
        }
        pDevice->pfSubmitBuffer = &StartMemDmaCrc;
    }
#else
    if (bEnable== false)
    {
        /* Close MDMA stream */
        if (pDevice->hDma != NULL)
        {
            if (adi_dma_Close(pDevice->hDma) != ADI_DMA_SUCCESS)
            {
               return (ADI_CRC_DMA_ERROR);
            }
        }
        pDevice->pfSubmitBuffer = &CalculateCrc;
    }
#endif
    /* Return */
    return (ADI_CRC_SUCCESS);
}
/*! \cond PRIVATE */
#endif
/*! \endcond */
/**
 * @brief   Select the Specified DMA channel for the CRC device.
 *
 * Parameters:
 *  @param [in]  hDevice        Handle to CRC device instance to work on.
 *  @param [in]  eDMaChannelID  Channel ID for the CRC device.
 *
 * @return  Status
 *          - #ADI_CRC_SUCCESS: Successfully set polynomial value.
 *          - #ADI_CRC_BAD_HANDLE: Supplied CRC handle is invalid.
 *          - #ADI_CRC_FN_NOT_PERMITTED: Function not permitted when CRC operation is in progress.
 *
 */
ADI_CRC_RESULT adi_crc_SelectDmaChannel(
    ADI_CRC_HANDLE const    hDevice,
    ADI_DMA_CHANNEL_ID      eDMaChannelID)
{
    ADI_CRC_DEVICE *pDevice = CrcDevicePointer(hDevice);

/* IF (Debug information enabled) */
#if defined (ADI_DEBUG)

    /* IF (CRC device handle is invalid) */
    if (pDevice == NULL)
    {
        return (ADI_CRC_BAD_HANDLE);
    }

    /* IF (CRC in progress) */
    if (pDevice->eCrcOpStatus != ADI_CRC_OP_IDLE)
    {
        /* Report error as this function is not permitted when CRC operation is in progress */
        return (ADI_CRC_FN_NOT_PERMITTED);
    }

#endif /* ADI_DEBUG */

    /* Load Polynomial value */
    pDevice->eDMAChannel = eDMaChannelID;

    /* Return */
    return (ADI_CRC_SUCCESS);
}

/**
 * @brief   Sets the 32-bit polynomial for CRC operations.
 *
 * Parameters:
 *  @param [in]  hDevice        Handle to CRC device instance to work on.
 *  @param [in]  PolynomialVal  32-bit CRC polynomial to use for CRC operation.
 *
 * @return  Status
 *          - #ADI_CRC_SUCCESS: Successfully set polynomial value.
 *          - #ADI_CRC_BAD_HANDLE: Supplied CRC handle is invalid.
 *          - #ADI_CRC_FN_NOT_PERMITTED: Function not permitted when CRC operation is in progress.
 *
 */
ADI_CRC_RESULT adi_crc_SetPolynomialVal(
    ADI_CRC_HANDLE const    hDevice,
    uint32_t                PolynomialVal)
{
    ADI_CRC_DEVICE *pDevice = CrcDevicePointer(hDevice);

/* IF (Debug information enabled) */
#if defined (ADI_DEBUG)

    /* IF (CRC device handle is invalid) */
    if (pDevice == NULL)
    {
        return (ADI_CRC_BAD_HANDLE);
    }

    /* IF (CRC in progress) */
    if (pDevice->eCrcOpStatus != ADI_CRC_OP_IDLE)
    {
        /* Report error as this function is not permitted when CRC operation is in progress */
        return (ADI_CRC_FN_NOT_PERMITTED);
    }

#endif /* ADI_DEBUG */

    /* Load Polynomial value */
    pDevice->pReg->POLY = PolynomialVal;

    /* Return */
    return (ADI_CRC_SUCCESS);
}

/**
 * @brief   Submits data buffer for CRC computation
 *
 * @details This API can be used to submit data buffer for CRC computation. If DMA mode of operation is
  *\n       is selected, buffer is processed using the specified DMA.
 *
 * Parameters:
 *  @param [in]  hDevice            Handle of CRC device
 *  @param [in]  pCrcBuf            Address of CRC data buffer
 *  @param [in]  NumBytes           Number of whole bytes in CRC data buffer
 *
 * @return  Status
 *          - #ADI_CRC_SUCCESS[D]: Successfully submitted data buffer.
 *          - #ADI_CRC_BAD_HANDLE[D]: Supplied CRC handle is invalid.
 *          - #ADI_CRC_DMA_ERROR[D]:  DMA error when buffer submitted.
 *          - #ADI_CRC_FN_NOT_PERMITTED[D]: Function not permitted when CRC operation is in progress.
 *
 */
ADI_CRC_RESULT adi_crc_Compute(
    ADI_CRC_HANDLE const    hDevice,
    void                    *pCrcBuf,
    uint32_t                NumBytes)
{
    return adi_crc_ComputeWithPartialByte(hDevice, pCrcBuf, NumBytes, 0u);
}

/**
 * @brief   Submits data buffer for CRC computation a partial last byte
 *
 * @details This API can be used to submit data buffer for CRC computation. If DMA mode of operation is
  *\n       is selected, buffer is processed using the specified DMA.
 *
 * Parameters:
 *  @param [in]  hDevice            Handle of CRC device
 *  @param [in]  pCrcBuf            Address of CRC data buffer
 *  @param [in]  NumBytes           Number of whole bytes in CRC data buffer
 *  @param [in]  NumBits            Number of bits in the last byte in CRC data buffer
 *
 * @return  Status
 *          - #ADI_CRC_SUCCESS[D]: Successfully submitted data buffer.
 *          - #ADI_CRC_BAD_HANDLE[D]: Supplied CRC handle is invalid.
 *          - #ADI_CRC_DMA_ERROR[D]:  DMA error when buffer submitted.
 *          - #ADI_CRC_FN_NOT_PERMITTED[D]: Function not permitted when CRC operation is in progress.
 *
 */
ADI_CRC_RESULT adi_crc_ComputeWithPartialByte(
    ADI_CRC_HANDLE const    hDevice,
    void                    *pCrcBuf,
    uint32_t                NumBytes,
    uint32_t                NumBits)
{
    ADI_CRC_DEVICE *pDevice = CrcDevicePointer(hDevice);

#if defined (ADI_DEBUG)
    if (NumBits >= 8u)
    {
        return (ADI_CRC_INVALID_PARAMETER);
    }

    if (pDevice == NULL)
    {
        return (ADI_CRC_BAD_HANDLE);
    }

    /* Partial byte needs CRC unit revision 1 or up */
    if (((pDevice->pReg->CTL & BITM_CRC_CTL_REVID) == 0u) && (NumBits != 0u))
    {
        return (ADI_CRC_FN_NOT_SUPPORTED);
    }

#endif /* ADI_DEBUG */

    /* IF (CRC in progress) */
    if (pDevice->eCrcOpStatus != ADI_CRC_OP_IDLE)
    {
        /* Report error as this function is not permitted when CRC operation is in progress */
        return (ADI_CRC_FN_NOT_PERMITTED);
    }

    return pDevice->pfSubmitBuffer(pDevice, pCrcBuf, NumBytes, NumBits);
}


/**
 * @brief   Gets the current CRC peripheral status.
 *

* Parameters:
 *  @param  [in]  hDevice           Handle to CRC device instance to work on
 *  @param  [in]  pbCrcInProgress   Pointer to location to store the current status of CRC peripheral.
 *                                  'true' when CRC peripheral is in currently performing a CRC operation.
 *
 * @return  Status
 *          - #ADI_CRC_SUCCESS: Successfully set expected CRC result.
 *          - #ADI_CRC_BAD_HANDLE: Supplied CRC handle is invalid.
 * @note: This function is valid only when device is operating in DMA mode.
 *
 */
ADI_CRC_RESULT adi_crc_IsCrcInProgress(
    ADI_CRC_HANDLE const    hDevice,
    bool_t                  *pbCrcInProgress)
{
    ADI_CRC_DEVICE *pDevice = CrcDevicePointer(hDevice);

/* IF (Debug information enabled) */
#if defined (ADI_DEBUG)

    /* IF (CRC device handle is invalid) */
    if (pDevice == NULL)
    {
        return (ADI_CRC_BAD_HANDLE);
    }

#endif /* ADI_DEBUG */

    /* IF (CRC in progress) */
    if (pDevice->eCrcOpStatus == ADI_CRC_OP_IN_PROGRESS )
    {
        /* Update CRC status flag */
        *pbCrcInProgress = true;
    }
    /* ELSE (CRC is idle) */
    else
    {
        /* Update CRC status flag */
        *pbCrcInProgress = false;
    }

    return (ADI_CRC_SUCCESS);
}

/**
 * @brief   Gets the final CRC result computed for a data stream
 *
 * @details This API gets the final CRC result computed for a data stream and
 *          clears the current and final CRC results register. The CRC Current result
 *          register holds the current or intermediate CRC result.
 *          Whenever a CRC operation is initiated, the CRC peripheral takes the
 *          CRC Current register value as initial seed for CRC computation. This
 *          API clears both results register to start a fresh CRC computation.
 *          Use the adi_crc_GetCurrentCrcVal() API to get an intermediate CRC result
 *          without clearing the results register.
 *
 * Parameters:
 *  @param  [in]  hDevice         Handle to CRC device instance to work on
 *  @param  [out] pFinalCrcVal    Pointer to location where the final CRC result of
 *                                a data stream to be processed will be written.
 *
 * @return  Status
 *          - #ADI_CRC_SUCCESS: Successfully read final CRC result.
 *          - #ADI_CRC_BAD_HANDLE: Supplied CRC handle is invalid.
 *
 */
ADI_CRC_RESULT adi_crc_GetFinalCrcVal(
    ADI_CRC_HANDLE const    hDevice,
    uint32_t               *pFinalCrcVal)
{
    ADI_CRC_DEVICE *pDevice = CrcDevicePointer(hDevice);

/* IF (Debug information enabled) */
#if defined (ADI_DEBUG)

    /* IF (CRC device handle is invalid) */
    if (pDevice == NULL)
    {
        return (ADI_CRC_BAD_HANDLE);
    }

#endif /* ADI_DEBUG */

    /* Get the final CRC result */
    *pFinalCrcVal = pDevice->pReg->RESULT;

    pDevice->pReg->RESULT = ADI_CFG_CRC_SEED_VALUE;

    /* Return */
    return (ADI_CRC_SUCCESS);
}

/**
 * @brief   Gets the current/intermediate CRC result computed for a data stream.
 *
 * Parameters:
 *  @param  [in]  hDevice           Handle to CRC device instance to work on
 *  @param  [out] pCurrentCrcVal    Pointer to location where the intermediate CRC result of
 *                                  a data stream to be processed will be written.
 *
 * @return  Status
 *          - #ADI_CRC_SUCCESS: Successfully read current CRC result.
 *          - #ADI_CRC_BAD_HANDLE: Supplied CRC handle is invalid.
 *
 */
ADI_CRC_RESULT adi_crc_GetCurrentCrcVal(
    ADI_CRC_HANDLE const    hDevice,
    uint32_t                *pCurrentCrcVal)
{
    ADI_CRC_DEVICE *pDevice = CrcDevicePointer(hDevice);

/* IF (Debug information enabled) */
#if defined (ADI_DEBUG)

    /* IF (CRC device handle is invalid) */
    if (pDevice == NULL)
    {
        return (ADI_CRC_BAD_HANDLE);
    }

#endif /* ADI_DEBUG */

    /* Get the current CRC result */
    *pCurrentCrcVal = pDevice->pReg->RESULT;

    /* Return */
    return (ADI_CRC_SUCCESS);
}

/**
 * @brief   Registers or unregisters a callback with the CRC device
 *
 * @details     It is not required to register a callback for the operation of the
 *              driver. Data compare or DMA error will be notified via the
 *              adi_crc_IsCrcInProgress() API. But if application requires the
 *              errors/events to be notified immediately it can register a callback
 *              with the driver which will be called to notify errors/events.
 *
 *              When a callback is registered the API adi_crc_IsCrcInProgress()
 *              will not return error.
 *
 * Parameters:
 *  @param  [in]  hDevice         Handle to CRC device instance to work on
 *  @param  [in]  pfCallback      Pointer to application callback function. The callback function
 *                                has the prototype
 *                                void callback(void *pCBParam, uint32_t nEvent, void *pArg)
 *                                To unregister a callback pass the the pointer to the callback
 *                                function as NULL.
 *  @param  [in]  pCBParam        Callback parameter which will be returned back to the
 *                                application when the callback function is called.
 *
 * @return  Status
 *          - #ADI_CRC_SUCCESS: Successfully registered callback.
 *          - #ADI_CRC_BAD_HANDLE: Supplied CRC handle is invalid.
 *
 */
ADI_CRC_RESULT adi_crc_RegisterCallback(
    ADI_CRC_HANDLE const    hDevice,
    ADI_CALLBACK            pfCallback,
    void *const             pCBParam)
{
    ADI_CRC_DEVICE *pDevice = CrcDevicePointer(hDevice);

/* IF (Debug information enabled) */
#if defined (ADI_DEBUG)

    /* IF (CRC device handle is invalid) */
    if (pDevice == NULL)
    {
        return (ADI_CRC_BAD_HANDLE);
    }

#endif /* ADI_DEBUG */

    /* Update Callback information */
    pDevice->pfCallback   = pfCallback;
    pDevice->pCBParam     = pCBParam;

    /* Return */
    return (ADI_CRC_SUCCESS);
}

#endif /* _ADI_CRC_V1_C_ */

/*****/

/*@}*/
