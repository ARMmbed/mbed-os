/*! ****************************************************************************
 * @file:    adi_sport.c
 * @brief:   SPORT (Serial Port) device driver source file.
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

*******************************************************************************/

/** @addtogroup SPORT_Driver SPORT Driver
 *  @{
 */

/*! \cond PRIVATE */

/*=============  I N C L U D E S   =============*/

#include <assert.h>
#include <string.h>     /* memset declaration */

#include <drivers/general/adi_drivers_general.h>
#include <adi_sport_config.h>
#include <drivers/sport/adi_sport.h>
#include <adi_cyclecount.h>
#include "adi_sport_def.h"

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
* Pm050 (rule 14.2): a null statement shall only occur on a line by itself
*   Needed for null expansion of ADI_INSTALL_HANDLER and others.
*
* Pm088 (rule 17.4): pointer arithmetic should not be used.
*   Relying on pointer arithmetic for buffer handling.
*
* Pm140 (rule 11.3): a cast should not be performed between a pointer type and an integral type
*   The rule makes an exception for memory-mapped register accesses.
*
* Pm152: (MISRA C 2004 rule 17.4) array indexing shall only be applied to objects defined as an array type
*   Accessing the DMA descriptors, which are defined in the system as a pointer to an array of descriptors

*/
#pragma diag_suppress=Pm026,Pm050,Pm073,Pm088,Pm123,Pm140,Pm143,Pm152,Pm153
#endif /* __ICCARM__ */

/*==============  D A T A  ===============*/

#define SPORT0_A_REGS   ((volatile ADI_SPORT_TypeDef*)REG_SPORT0_CTL_A)
#define SPORT0_B_REGS   ((volatile ADI_SPORT_TypeDef*)REG_SPORT0_CTL_B)

#define SPORT0_A_CFG    { 0u, 0u, 0u, 0u, 0u }
#define SPORT0_B_CFG    { 0u, 0u, 0u, 0u, 0u }

#define DXS_FIFO_IS_FULL(STAT)  (((STAT) & BITM_SPORT_STAT_A_DXS) == BITM_SPORT_STAT_A_DXS)
#define DXS_FIFO_IS_EMPTY(STAT) (((STAT) & BITM_SPORT_STAT_A_DXS) == 0u)

static  ADI_SPORT_DEVICE_INFO gSportDevInfo [ADI_SPORT_NUM_INSTANCES][ADI_SPORT_NUM_CHANNELS] =
{
    {/* registers       configuration  initial state                  DMA channel    DMA IRQ             SPORT IRQ         handle */
        {SPORT0_A_REGS, SPORT0_A_CFG,  ADI_SPORT_STATE_UNINITIALIZED, SPORT0A_CHANn, DMA0_CH2_DONE_IRQn, SPORT_A_EVT_IRQn, NULL},
        {SPORT0_B_REGS, SPORT0_B_CFG,  ADI_SPORT_STATE_UNINITIALIZED, SPORT0B_CHANn, DMA0_CH3_DONE_IRQn, SPORT_B_EVT_IRQn, NULL},
    },
};


static const ADI_SPORT_CONFIG gSportCfg[ADI_SPORT_NUM_INSTANCES][ADI_SPORT_NUM_CHANNELS] =
{
  { /* configuration for SPORT 0 */
     /* Configuration for half-SPORT A */
     { /* SPORT_CTL register */
       ((ADI_CFG_SPORT0A_ENABLE_FSMUXSEL)       << BITP_SPORT_CTL_A_FSMUXSEL)   |
       ((ADI_CFG_SPORT0A_ENABLE_CKMUXSEL)       << BITP_SPORT_CTL_A_CKMUXSEL)   |
       ((ADI_CFG_SPORT0A_LSB_FIRST)             << BITP_SPORT_CTL_A_LSBF)       |
       ((ADI_CFG_SPORT0A_SERIAL_WLEN - 1u)      << BITP_SPORT_CTL_A_SLEN)       |
       ((ADI_CFG_SPORT0A_INTERNAL_CLK)          << BITP_SPORT_CTL_A_ICLK)       |
       ((ADI_CFG_SPORT0A_OPERATION_MODE)        << BITP_SPORT_CTL_A_OPMODE)     |
       ((ADI_CFG_SPORT0A_CLOCK_EDGE)            << BITP_SPORT_CTL_A_CKRE)       |
       ((ADI_CFG_SPORT0A_FS_REQUIRED)           << BITP_SPORT_CTL_A_FSR)        |
       ((ADI_CFG_SPORT0A_INTERNAL_FS)           << BITP_SPORT_CTL_A_IFS)        |
       ((ADI_CFG_SPORT0A_DATA_INDEPENDENT_FS)   << BITP_SPORT_CTL_A_DIFS)       |
       ((ADI_CFG_SPORT0A_ACTIVE_LOW_FS)         << BITP_SPORT_CTL_A_LFS)        |
       ((ADI_CFG_SPORT0A_LATE_FS)               << BITP_SPORT_CTL_A_LAFS)       |
       ((ADI_CFG_SPORT0A_ENABLE_PACKING)        << BITP_SPORT_CTL_A_PACK)       |
       ((ADI_CFG_SPORT0A_FS_ERROR_OPERATION)    << BITP_SPORT_CTL_A_FSERRMODE)  |
       ((ADI_CFG_SPORT0A_GATED_CLOCK)           << BITP_SPORT_CTL_A_GCLKEN),

        /* SPORT_DIV register */
       ((ADI_CFG_SPORT0A_CLOCK_DIVISOR)         << BITP_SPORT_DIV_A_CLKDIV)     |
       ((ADI_CFG_SPORT0A_FS_DIVISOR)            << BITP_SPORT_DIV_A_FSDIV),

        /* SPORT_CONVT register */
       ((ADI_CFG_SPORT0A_CONVT_WIDTH)           << BITP_SPORT_CNVT_A_WID)       |
       ((ADI_CFG_SPORT0A_CONVT_POLARITY)        << BITP_SPORT_CNVT_A_POL)       |
       ((ADI_CFG_SPORT0A_CONVT_FS_DURATION)     << BITP_SPORT_CNVT_A_CNVT2FS),

       /* Default DMA data size for SPORT */
       ADI_DMA_WIDTH_4_BYTE,

       /* Default DMA data increment for SPORT */
       ADI_DMA_INCR_4_BYTE
     },

     /* Configuration for half-SPORT B */
     { /* SPORT_CTL register */
       ((ADI_CFG_SPORT0B_LSB_FIRST)             << BITP_SPORT_CTL_B_LSBF)       |
       ((ADI_CFG_SPORT0B_SERIAL_WLEN - 1u)      << BITP_SPORT_CTL_B_SLEN)       |
       ((ADI_CFG_SPORT0B_INTERNAL_CLK)          << BITP_SPORT_CTL_B_ICLK)       |
       ((ADI_CFG_SPORT0B_OPERATION_MODE)        << BITP_SPORT_CTL_B_OPMODE)     |
       ((ADI_CFG_SPORT0B_CLOCK_EDGE)            << BITP_SPORT_CTL_B_CKRE)       |
       ((ADI_CFG_SPORT0B_FS_REQUIRED)           << BITP_SPORT_CTL_B_FSR)        |
       ((ADI_CFG_SPORT0B_INTERNAL_FS)           << BITP_SPORT_CTL_B_IFS)        |
       ((ADI_CFG_SPORT0B_DATA_INDEPENDENT_FS)   << BITP_SPORT_CTL_B_DIFS)       |
       ((ADI_CFG_SPORT0B_ACTIVE_LOW_FS)         << BITP_SPORT_CTL_B_LFS)        |
       ((ADI_CFG_SPORT0B_LATE_FS)               << BITP_SPORT_CTL_B_LAFS)       |
       ((ADI_CFG_SPORT0B_ENABLE_PACKING)        << BITP_SPORT_CTL_B_PACK)       |
       ((ADI_CFG_SPORT0B_FS_ERROR_OPERATION)    << BITP_SPORT_CTL_B_FSERRMODE)  |
       ((ADI_CFG_SPORT0B_GATED_CLOCK)           << BITP_SPORT_CTL_B_GCLKEN),

        /* SPORT_DIV register */
       ((ADI_CFG_SPORT0B_CLOCK_DIVISOR)         << BITP_SPORT_DIV_B_CLKDIV)     |
       ((ADI_CFG_SPORT0B_FS_DIVISOR)            << BITP_SPORT_DIV_B_FSDIV),

        /* SPORT_CONVT register */
       ((ADI_CFG_SPORT0B_CONVT_WIDTH)           << BITP_SPORT_CNVT_B_WID)       |
       ((ADI_CFG_SPORT0B_CONVT_POLARITY)        << BITP_SPORT_CNVT_B_POL)       |
       ((ADI_CFG_SPORT0B_CONVT_FS_DURATION)     << BITP_SPORT_CNVT_B_CNVT2FS),

       /* Default DMA data size for SPORT */
       ADI_DMA_WIDTH_4_BYTE,

       /* Default DMA data increment for SPORT */
       ADI_DMA_INCR_4_BYTE
     }
  }
};

/*! \endcond */

/*=============  C O D E  =============*/

extern void SPORT0A_Int_Handler(void);          /*!< Interrupt handler for the SPORT0-A */
extern void SPORT0B_Int_Handler(void);          /*!< Interrupt handler for the SPORT0-B */
extern void DMA_SPORT0A_Int_Handler(void);      /*!< DMA handler for the SPORT0-A */
extern void DMA_SPORT0B_Int_Handler(void);      /*!< DMA handler for the SPORT0-B */

/*=============  L O C A L   F U N C T I O N S =============*/

/*=============  P U B L I C   F U N C T I O N S =============*/

/**
 * @brief       Initialization function for SPORT device.
 * @details     Initialization function for SPORT device. This function must be
 *              called before operating any SPORT device.
 *
 * @param [in]  nDevNum         SPORT Device instance to be opened.
 * @param [in]  eChannel        Channel ID of the SPORT device (A or B)
 * @param [in]  eDirection      Direction of the SPORT operation (i.e Rx or Tx)
 * @param [in]  pMemory         Pointer to a 32 bit aligned buffer containing
 *                              ADI_SPORT_MEMORY_SIZE bytes. This buffer is
 *                              required by the SPORT driver for its operations.
 *                              The "ADI_SPORT_MEMORY_SIZE" varies based on the
 *                              configuration.
 * @param [in]  nMemSize        Size of the buffer to which "pMemory" points.
 * @param [out] phDevice        Pointer to a location where a handle to the
 *                              opened SPORT driver can be stored. This handle
 *                              will be used to identity a SPORT device when
 *                              calling SPORT management functions.
 *
 * @return      Status
 * - #ADI_SPORT_SUCCESS                 Successful device initialization.
 * - #ADI_SPORT_DEVICE_IN_USE           Device already initialized.
 * - #ADI_SPORT_FAILED                  Failed initialize a semaphore for managing device.
 * - #ADI_SPORT_INVALID_DEVICE_NUM      Invalid SPORT device identifier
 * - #ADI_SPORT_INVALID_NULL_POINTER    Invalid pointer (callback function or device handle).
 *
 * @sa  adi_sport_Close()
 */
ADI_SPORT_RESULT adi_sport_Open(
    const uint32_t              nDevNum,
    const ADI_SPORT_CHANNEL     eChannel,
    const ADI_SPORT_DIRECTION   eDirection,
    void                        *pMemory,
    const uint32_t              nMemSize,
    ADI_SPORT_HANDLE * const    phDevice
    )
{
    ADI_SPORT_RESULT result = ADI_SPORT_SUCCESS;

    assert(ADI_SPORT_MEMORY_SIZE == sizeof(ADI_SPORT_DEVICE));  /* validate the memory size macro */
#ifdef ADI_DEBUG
    if (nDevNum >= ADI_SPORT_NUM_INSTANCES)
    {
        result = ADI_SPORT_INVALID_DEVICE_NUM;          /* SPORT identifier must be within [0..ADI_SPORT_NUM_INSTANCES-1] */
    }
    else if (phDevice == NULL)
    {
        result = ADI_SPORT_INVALID_NULL_POINTER;        /* the pointer to device handle must be valid */
    }
    else if (ADI_SPORT_MEMORY_SIZE != nMemSize)
    {
        result = ADI_SPORT_FAILED;
    }
    else if (ADI_SPORT_STATE_UNINITIALIZED != gSportDevInfo[nDevNum][eChannel].eState)
    {
        result = ADI_SPORT_DEVICE_IN_USE;               /* the device instance must not be in use */
    }
    else
#endif /* ADI_DEBUG */
    {
        ADI_SPORT_DEVICE * pDevice = pMemory;           /* Pointer to the SPORT device instance (from supplied memory) */
        ADI_SPORT_DEVICE_INFO * sportInfo = &gSportDevInfo[nDevNum][eChannel];  /* SPORT info for HSPORT A or HSPORT B */
        ADI_SPORT_CONFIG const * sportCfg = &gSportCfg[nDevNum][eChannel];      /* SPORT configuration for HSPORT A or HSPORT B */

        assert(eChannel < ADI_SPORT_NUM_CHANNELS);

        memset(pMemory, 0, nMemSize);           /* clear the device instance data before initializing it */

        pDevice->pSportInfo = sportInfo;        /* Initialize the pointer which provides the device information (HSPORT A or HSPORT B). */
        pDevice->eDirection = eDirection;       /* Initialize the direction (BEFORE calling sport_Configure)*/
        pDevice->nHwError = (uint32_t) ADI_SPORT_HW_NO_ERR;

        adi_dma_Init();                         /* Set up the DMA Controller. */
        sport_Init(pDevice);                    /* Initialize the data transmission buffers */
        sport_Configure(pDevice,sportCfg);      /* Configure the SPORT */

        if (ADI_DMA_SUCCESS != adi_dma_RegisterCallback(sportInfo->eDMAChnlID, sport_DmaErrorCallback, pDevice))
        {
            adi_sport_Close(pDevice);
            result = ADI_SPORT_DMA_REGISTER_FAILED;
        }

        if (ADI_SPORT_SUCCESS == result)
        {
            ADI_SPORT_DEVICE_INFO * devInfo = &gSportDevInfo[nDevNum][eChannel];

            /* Create a "semaphore" (varies per OS) used for blocking buffer resource management. */
            if (ADI_HALF_SPORT_A == eChannel)
            {
                SEM_CREATE(&pDevice->sportChannel, "SPORT0_A_SEM", ADI_SPORT_FAILED);
            }else{
                SEM_CREATE(&pDevice->sportChannel, "SPORT0_B_SEM", ADI_SPORT_FAILED);
            }

            /* Change the state of the specified device */
            ADI_INT_STATUS_ALLOC();
            ADI_ENTER_CRITICAL_REGION();
                devInfo->eState = ADI_SPORT_STATE_INITIALIZED;
                devInfo->hDevice = pDevice;
            ADI_EXIT_CRITICAL_REGION();
            *phDevice = pDevice;                /* Return the device handle to the application */
        }
    }

    return result;
}

/**
 * @brief       Closes the operation of specified SPORT device.
 *
 * @details     Closes the operation of specified SPORT device.
 *              Device need to be opened again for any further use.
 *
 * @param [in]  hDevice SPORT device handle whose operation is to be closed.
 *                      This handle was obtained when a SPORT device is opened
 *                      successfully.
 *
 * @return      Status
 *
 *      - #ADI_SPORT_SUCCESS            Successfully closed the specified device.
 *      - #ADI_SPORT_INVALID_HANDLE [D] Invalid SPORT device handle.
 *      - #ADI_SPORT_FAILED         [D] SPORT device internal error.
 *
 * @note        It is user's responsibility to free/reuse the memory supplied
 *              during the opening of the device.
 *
 * @sa          adi_sport_Open()
 */
ADI_SPORT_RESULT adi_sport_Close(ADI_SPORT_HANDLE const hDevice)
{
    ADI_SPORT_RESULT    result = ADI_SPORT_SUCCESS;             /* return code */
    ADI_SPORT_DEVICE *  pDevice = (ADI_SPORT_DEVICE *) hDevice; /* Pointer to SPORT device instance */
#ifdef ADI_DEBUG
    if (ADI_SPORT_SUCCESS == (result=ValidateHandle(pDevice)))  /* Validate the given handle */
#endif /* ADI_DEBUG */
    {
        ADI_SPORT_DEVICE_INFO * pSportInfo = pDevice->pSportInfo;

        /* Free up the device */
        ADI_INT_STATUS_ALLOC();
        ADI_ENTER_CRITICAL_REGION();
            NVIC_DisableIRQ(pSportInfo->eIRQn);                 /* Disable SPORT event interrupts. */
            NVIC_DisableIRQ(pSportInfo->eDMAn);                 /* Disable DMA SPORT interrupts. */
            pSportInfo->eState  = ADI_SPORT_STATE_UNINITIALIZED;
            pSportInfo->hDevice = NULL;                         /* Free up the device memory. */
        ADI_EXIT_CRITICAL_REGION();

        SEM_DELETE(&pDevice->sportChannel, ADI_SPORT_FAILED);   /* Delete SPORT channel semaphore. */

        adi_dma_RegisterCallback(pSportInfo->eDMAChnlID, NULL, NULL);   /* unregister the callback function in the DMA error handler */

        pSportInfo->pSportRegs->CTL_A = 0u;
    }
    return result;
}

/**
 * @brief       Submit the buffer for transmitting/receiving the data. This function can
 *              be used to submit the buffers for both transmitting and receiving. It will
 *              be returned after successfully submitting the buffer for transmitting data.
 *              User will be notified if a call back function is registered with an event code
 *              #ADI_SPORT_EVENT_RX_BUFFER_PROCESSED or #ADI_SPORT_EVENT_TX_BUFFER_PROCESSED"
 *              depending on the direction in which device is operating.
 *
 * @param [in]  hDevice   Device handle to SPORT device is obtained when a SPORT device is opened
 *                        successfully.
 *
 * @param [in]  pBuffer   Pointer to buffer from where data need to be transmitted OR to which
 *                        received data need to to be written.
 *
 * @param [in]  nNumBytes Size in bytes of the data to be transmitted/received.
 * @param [in]  bDMA      True if the buffer must be processed through DMA-driven SPORT operations.
 *
 * @return      Status
 *
 *  - #ADI_SPORT_SUCCESS                        Buffer successfully submitted to the specified SPORT.
 *  - #ADI_SPORT_INVALID_HANDLE                 Invalid SPORT device handle.
 *  - #ADI_SPORT_INVALID_PARAMETER              Number of bytes is too large for a SPORT transfer or the buffer is mis-aligned
 *  - #ADI_SPORT_BUFFERS_NOT_SUBMITTED          All the SPORT buffers are already being used
 *
 * @sa          adi_sport_GetBuffer()
 *
 */
ADI_SPORT_RESULT adi_sport_SubmitBuffer(ADI_SPORT_HANDLE const hDevice,
                                        void *           const pBuffer,
                                        uint32_t         const nNumBytes,
                                        bool             const bDMA
                                        )
{
    ADI_SPORT_DEVICE *  pDevice = (ADI_SPORT_DEVICE *) hDevice;         /* pointer to SPORT device instance */
    ADI_SPORT_RESULT    result = ADI_SPORT_SUCCESS;                     /* return code */

#ifdef ADI_DEBUG
    ADI_SPORT_CONFIG * pSportCfg = &pDevice->pSportInfo->sportCfg;      /* short cut to SPORT configuration */

    if(ADI_SPORT_SUCCESS != (result=ValidateHandle(hDevice)))           /* validate the given handle */
    {
    }
    else if (  ((2u >= nNumBytes) && ((pDevice->pSportInfo->pSportRegs->CTL_A & BITM_SPORT_CTL_A_OPMODE) != 0u))
            || (0u != (nNumBytes & ~(BITM_SPORT_NUMTRAN_A_VALUE)))      /* buffer size limited by SPORT transmission capabilities */
            )
    {
        result = ADI_SPORT_INVALID_PARAMETER;
    }
    else
#endif /* ADI_DEBUG */
    /* Check that there is a free buffer to use for this transmit operation. pFreeBuffer
       is the next buffer available, so if it is in use we can make the assumption that
       there are no buffers available. The start address is set to NULL once the buffer
       has finished being processed in "adi_sport_GetBuffer()".
    */
    if (NULL != pDevice->sportChannel.pFreeBuffer->pStartAddress)
    {
        result = ADI_SPORT_BUFFERS_NOT_SUBMITTED;
    }
    else
    {
#ifdef ADI_DEBUG
        const uint32_t addr = (uint32_t) pBuffer;

        if (true == bDMA)
        {
            /**
             * Using SPORT configuration data, let's define information such as data
             * size in bytes, data number, number of data and bytes in the DMA transfer
             * being prepared, last byte position for the DMA transfer
             *
             * It's important to keep in mind that for buffer that contain too many data
             * multiple DMA transfers are needed: it's up to the application to split the
             * DMA requests in requests which have an appropriate number of data.
             */
            const uint32_t dataSizeInBytes = GetBytesPerSportData(pSportCfg->CTL);
            const uint32_t full = nNumBytes / dataSizeInBytes;          /* number of full data to transmit/receive */
            const uint32_t partial = nNumBytes % dataSizeInBytes;       /* number of partial data to transmit/receive */
            const uint32_t misaligned = addr % dataSizeInBytes;         /* number of data to transmit/receive */

            if (  (full > DMA_TRANSFER_LIMIT)                           /* number of data to process too large for DMA */
               || (0u != partial)                                       /* buffer size not a multiple of dataSizeInBytes */
               || (0u != misaligned)                                    /* buffer mis-aligned */
               )
            {
                result = ADI_SPORT_INVALID_PARAMETER;
            }
        } else {
            const uint32_t misAligned = addr % 4u;
            const uint32_t invalidNum = nNumBytes % 4u;

            if (  (0u != misAligned)                                    /* mis-aligned buffer */
               || (0u != invalidNum)                                    /* number of bytes not a multiple of 32-bit  */
               )
            {
                result = ADI_SPORT_INVALID_PARAMETER;                   /* reject the buffer submission */
            }
        }
        if (ADI_SPORT_SUCCESS == result)
#endif /* ADI_DEBUG */
        {
            ADI_DT_CHANNEL * pSportChnl = &pDevice->sportChannel;

            pSportChnl->pFreeBuffer->pStartAddress = pBuffer;   /* Set the start address of the data buffer */
            pSportChnl->pFreeBuffer->nCount        = nNumBytes; /* Set the buffer size */
            pSportChnl->pFreeBuffer->nIndex        = 0u;        /* Initialize the buffer index to zero (1st data in buffer) */
            pSportChnl->pFreeBuffer->bDMA          = bDMA;      /* Set the DMA boolean value. */
            pSportChnl->pFreeBuffer->bInUse        = true;      /* this buffer is now being used by the SPORT */

            /* Now that this "pFreeBuffer" is no longer free for use, update the
            "pFreeBuffer" to the next buffer. "pFreeBuffer" will only be updated
            during the process of submitting a buffer or a read/write operation.
            */
            pSportChnl->pFreeBuffer = pSportChnl->pFreeBuffer->pNextBuffer;

            /* Set the data transfer mode in case it was #ADI_DT_MODE_NONE. This
            will be set back to #ADI_DT_MODE_NONE once this transaction is complete.
            Then, if a buffer is not currently active, set up the interrupts for
            this transaction. Otherwise if a buffer is currently active, this will
            be taken care of in the ISR.
            */
            if (pSportChnl->eDataTranferMode == ADI_DT_MODE_NONE)   /* if the SPORT is available for a transmission */
            {
                pSportChnl->eDataTranferMode = ADI_DT_MODE_NONBLOCKING;

                /* call an appropriate function based on mode in which device is operating */
                if (true == bDMA)                                   /* select a DMA driven or a core driven non-blocking transmission */
                {
                    result = sport_SubmitBufferDmaMode(pDevice, pSportChnl->pFillBuffer);
                } else {
                    result = sport_SubmitBufferIntMode(pDevice, pSportChnl->pFillBuffer);
                }
            }

            if(ADI_SPORT_SUCCESS != result)                             /* if an error occurred...*/
            {
                pSportChnl->eDataTranferMode = ADI_DT_MODE_NONE;        /* SPORT is available */
            }
        }
    }

    return result;
}

/*
 * @brief       Submit a buffer for SPORT Rx or Tx DMA driven transmission.
 *
 * @param [in]  pDevice Pointer to SPORT device.
 *
 * @param [in]  pBuffer Pointer to data transfer buffer information.
 *
 * @return      Status
 *
 *  - #ADI_SPORT_SUCCESS                 buffer successfully submitted to the DMA associated with the SPORT.
 *  - #ADI_SPORT_BUFFERS_NOT_SUBMITTED   Failed to submit the buffer to the DMA associated with the SPORT.
 */
/** Function prototype for submitting a buffer for SPORT Rx or Tx DMA driven transmission */
static ADI_SPORT_RESULT sport_SubmitBufferDmaMode(ADI_SPORT_DEVICE * pDevice,
                                                  ADI_DT_BUFF_INFO * pBuff)
{
    ADI_SPORT_RESULT result = ADI_SPORT_SUCCESS;
    ADI_SPORT_DEVICE_INFO * pSportInfo = pDevice->pSportInfo;           /* short cut to SPORT information */
    ADI_SPORT_CONFIG * pSportCfg = &pSportInfo->sportCfg;               /* short cut to SPORT configuration */

#ifdef ADI_DEBUG
    if (  (pBuff != pDevice->sportChannel.pFillBuffer)                  /* a submitted buffer should always be the current fill buffer */
       || (true != pBuff->bInUse)                                       /* Processed buffers should already be marked as being used */
       || (0u != pBuff->nIndex)                                         /* processing should start from index 0 */
       )
    {
        result = ADI_SPORT_FAILED;
    }
    else
#endif
    {
        volatile ADI_SPORT_TypeDef* pSportRegs = pSportInfo->pSportRegs;/* short cut to SPORT registers */
        const uint32_t dmaChnlId = (uint32_t) pSportInfo->eDMAChnlID;   /* identifier for the DMA channel to be used */
        const uint32_t dmaChnlBit = (1u << dmaChnlId);                  /* bit representing the DMA channel to be used */

        /**
         * Using SPORT configuration data, let's define information such as data
         * size in bytes, data number, number of data and bytes in the DMA transfer
         * being prepared, last byte position for the DMA transfer
         *
         * It's important to keep in mind that for buffer that contain too many data
         * multiple DMA transfers are needed, so a buffer may have had part of its
         * content already DMA-transferred: nIndex defines the position of the first
         * byte in a buffer that has not been DMA-transferred yet.
         */
        const uint32_t dmaIncNone = (uint32_t) ADI_DMA_INCR_NONE;
        const uint32_t dmaDcc = (uint32_t) DMA_ENUM_CTL_CYCLE_CTL_BASIC;
        const uint32_t bytesPerData = GetBytesPerSportData(pSportCfg->CTL);

        const uint32_t dataSizeInBytes = (1u << pSportCfg->DMA_WIDTH);  /* number of bytes in each data to transmit/receive */
              uint32_t numDmaData = pBuff->nCount / dataSizeInBytes;    /* number of DMA data to transmit/receive */
        const uint32_t dmaDataEnd = (pBuff->nCount - dataSizeInBytes);  /* position of last <8,16,32>-bit data in the DMA transfer being setup */
        const uint32_t startAddress = (uint32_t) pBuff->pStartAddress;  /* address of the first byte in the data buffer */
        const uint32_t numSportData = pBuff->nCount / bytesPerData;     /* number of SPORT data to transmit/receive */

        assert(pBuff->nCount == (numSportData * bytesPerData));
        assert(numSportData <= 0xFFFu);
        assert(0u == (pBuff->nCount % dataSizeInBytes));
        assert(numDmaData <= DMA_TRANSFER_LIMIT);
        assert((ADI_SPORT_DIR_RX == pDevice->eDirection) || (ADI_SPORT_DIR_TX == pDevice->eDirection));
        SPORT_CHECK_CFG_CTL(pSportCfg->CTL);

        pSportRegs->CTL_A = 0u;                                         /* make sure SPORT is disable */
        pADI_DMA0->SRCADDR_CLR = dmaChnlBit;                             /* Clear source address decrement for TX channel DMA. */
        pADI_DMA0->EN_SET = dmaChnlBit;                                 /* Enable channel DMA. */
        pADI_DMA0->RMSK_CLR = dmaChnlBit;                               /* Enable SPORT peripheral to generate DMA requests. */
        pADI_DMA0->ALT_CLR = dmaChnlBit;                                /* Set the primary control data structure as the current DMA descriptor. */
        pADI_DMA0->PRI_SET = dmaChnlBit;

        if (ADI_SPORT_DIR_RX == pDevice->eDirection)
        {
            pPrimaryCCD[dmaChnlId].DMASRCEND = (uint32_t) &pSportRegs->RX_A;    /* address of the last src data in the DMA transfer being setup */
            pPrimaryCCD[dmaChnlId].DMADSTEND = startAddress + dmaDataEnd;       /* address of the last dst data in the DMA transfer being setup */
            pPrimaryCCD[dmaChnlId].DMACDC    =
              (pSportCfg->DMA_INC << ((uint32_t)DMA_BITP_CTL_DST_INC))  |       /* destination address incremented by N bytes */
              (dmaIncNone         << ((uint32_t)DMA_BITP_CTL_SRC_INC));         /* source address not incremented */
        }
        else                                                                    /* ADI_SPORT_DIR_TX */
        {
            pPrimaryCCD[dmaChnlId].DMASRCEND = startAddress + dmaDataEnd;       /* address of the last src data in the DMA transfer being setup */
            pPrimaryCCD[dmaChnlId].DMADSTEND = (uint32_t) &pSportRegs->TX_A;    /* address of the last dst data in the DMA transfer being setup */
            pPrimaryCCD[dmaChnlId].DMACDC    =
              (dmaIncNone         << ((uint32_t)DMA_BITP_CTL_DST_INC))  |       /* destination address not incremented */
              (pSportCfg->DMA_INC << ((uint32_t)DMA_BITP_CTL_SRC_INC));         /* source address incremented by N byte */

            /**
             * Fix for data transmission when DMA is used with packed data.
             */
            if (numDmaData < numSportData)
            {
                pPrimaryCCD[dmaChnlId].DMASRCEND = startAddress + dmaDataEnd + dataSizeInBytes;       /* address of the last src data in the DMA transfer being setup */
                numDmaData++;
            }
        }
        pPrimaryCCD[dmaChnlId].DMACDC |=
          (pSportCfg->DMA_WIDTH << ((uint32_t)DMA_BITP_CTL_SRC_SIZE))   |       /* source data size in bytes */
          (0u                   << ((uint32_t) DMA_BITP_CTL_R_POWER))   |
          ((numDmaData - 1u)    << ((uint32_t)DMA_BITP_CTL_N_MINUS_1))  |       /* number of DMA transfers (minus 1) */
          (dmaDcc               << ((uint32_t)DMA_BITP_CTL_CYCLE_CTL));

        ADI_INT_STATUS_ALLOC();
        ADI_ENTER_CRITICAL_REGION();
            pDevice->pSportInfo->eState = ADI_SPORT_STATE_DATA_FLOW_ENABLED;
            pSportRegs->NUMTRAN_A = numSportData;

            /* Enable SPORT DMA request interrupt for the SPORT tx channel. */
            NVIC_ClearPendingIRQ(pSportInfo->eIRQn);
            NVIC_ClearPendingIRQ(pSportInfo->eDMAn);

            uint32_t ien_a = ((uint32_t)BITM_SPORT_IEN_A_SYSDATERR) |
                             ((uint32_t)BITM_SPORT_IEN_A_FSERRMSK) |
                             ((uint32_t)BITM_SPORT_IEN_A_DERRMSK);
            if (ADI_SPORT_DIR_RX == pDevice->eDirection)
            {
                /* Allow SPORT DMA interrupt handling to mark SPORT Rx as complete */
                NVIC_EnableIRQ(pSportInfo->eDMAn);
            }
            else
            {
              /* SPORT DMA Tx is complete when TFI is raised: enable TFI */
                ien_a |= ((uint32_t)BITM_SPORT_IEN_A_TF);
            }

            NVIC_EnableIRQ(pSportInfo->eIRQn);
            
            pSportRegs->IEN_A = ien_a;
            pSportRegs->CTL_A = pSportCfg->CTL |
                                ((uint32_t)BITM_SPORT_CTL_A_SPEN) |
                                ((uint32_t)BITM_SPORT_CTL_A_DMAEN);
        ADI_EXIT_CRITICAL_REGION();

        SPORT_CHECK_CFG_CTL(pSportCfg->CTL);
    }
    return result;
}

/** Function prototype for  */
/*
 * @brief       Submit a buffer for SPORT Rx or Tx core driven transmission.
 *
 * @details     Submit a buffer for SPORT Rx or Tx core driven transmission.
 *              The buffer must be 32-bit aligned and contain N * 32-bit data.
 *
 * @param [in]  pDevice Pointer to SPORT device.
 *
 * @param [in]  pBuffer Pointer to data transfer buffer information.
 *
 * @return      Status
 *
 *  - #ADI_SPORT_SUCCESS                   Successfully submitted the buffer for data transfer.
 *
 *  - #ADI_SPORT_BUFFERS_NOT_SUBMITTED     No free descriptor for data transfer.
 *
 *
 */
static ADI_SPORT_RESULT sport_SubmitBufferIntMode(ADI_SPORT_DEVICE * pDevice, ADI_DT_BUFF_INFO * pBuff)
{
    ADI_SPORT_RESULT result = ADI_SPORT_SUCCESS;
#ifdef ADI_DEBUG
    if (  (pBuff != pDevice->sportChannel.pFillBuffer)  /* a submitted buffer should always be the current fill buffer */
       || (true != pBuff->bInUse)                       /* Processed buffers should already be marked as being used */
       || (0u != pBuff->nIndex)                         /* processing should start from index 0 */
       )
    {
        result = ADI_SPORT_FAILED;
    }
    else
#endif /* ADI_DEBUG */
    {
        ADI_SPORT_CONFIG * pSportCfg = &pDevice->pSportInfo->sportCfg;
        volatile ADI_SPORT_TypeDef * pSportRegs = pDevice->pSportInfo->pSportRegs;
        uint32_t ctl = pSportCfg->CTL;
        uint32_t bytesPerData = GetBytesPerSportData(ctl);

        /**
         * Buffer can be too large for being processed in one submission.
         * Consequently, if pBuff->nCount requires more than than 12-bit,
         * multiple buffer submissions will be required by the application;
         * the SPORT driver cannot process large buffers implicitly.
         * The number of bytes in submitted buffers must be a multiple of 4
         * because data are processed by the SPORT driver as 32-bit data.
         */

        /* use the SPORT configuration to setup the SPORT registers */

        pBuff->nCount /= bytesPerData;                  /* number of data to be transmitted */

#ifdef ADI_DEBUG
        uint32_t pack = SPORT_GET_PACKEN(pSportCfg->CTL);
        assert( ((9u > bytesPerData) && (1u == pack)) || ((17u > bytesPerData) && (2u == pack)) || (0u == pack));
#endif
        assert(pBuff->nCount <= 0xFFFu);
        SPORT_CHECK_CFG_CTL(pSportCfg->CTL);

        ADI_INT_STATUS_ALLOC();
        ADI_ENTER_CRITICAL_REGION();
            pSportRegs->CTL_A = 0u;                     /* make sure SPORT is disable */
            pSportRegs->NUMTRAN_A = pBuff->nCount;
            pDevice->pSportInfo->eState = ADI_SPORT_STATE_DATA_FLOW_ENABLED;

            /* Enable SPORT Interrupt. */
            NVIC_ClearPendingIRQ(pDevice->pSportInfo->eIRQn);
            NVIC_EnableIRQ(pDevice->pSportInfo->eIRQn);
            pSportRegs->IEN_A |= ((uint32_t) ( BITM_SPORT_IEN_A_DATA
                                             | BITM_SPORT_IEN_A_SYSDATERR
                                             | BITM_SPORT_IEN_A_FSERRMSK
                                             | BITM_SPORT_IEN_A_DERRMSK
                                             | BITM_SPORT_IEN_A_TF
                                             )
                                 );
            pSportRegs->CTL_A =  pSportCfg->CTL | ((uint32_t)BITM_SPORT_CTL_A_SPEN);
        ADI_EXIT_CRITICAL_REGION();
    }
    return result;
}

/**
 * @brief       This function returns the address of a processed buffer. This
 *              is a blocking function: it waits until a buffer has been dealt
 *              with. This function returns an error if a callback function is
 *              registered. #adi_sport_IsBufferAvailable can be used as a peek
 *              function to know whether a buffer is available.
 *
 * @param [in]  hDevice  Device handle to SPORT device, obtained when a SPORT
 *                       device is openedsuccessfully.
 *
 * @param [out] ppBuffer Pointer to a location where the the address of the
 *                       buffer is to be written. Contains the address of an
 *                       "empty" buffer (i.e the content of the buffer is
 *                       transmitted) OR "filled" buffer which contains the
 *                       received data.
 *
 * @param [out] pHwError Pointer to 32-bit value reporting SPORT/DMA events
 *                       that can occur when processing buffer ppBuffer.
 *
 * @return      Status
 *
 *  - #ADI_SPORT_SUCCESS                Successfully returned. ppBuffer points
 *                                      to the address of the buffer.
 *
 *  - #ADI_SPORT_FAILED                 Failed to get the buffer since device
 *                                      is operating in call back mode.
 *                                      ppBuffer points NULL.
 *
 *  - #ADI_SPORT_HW_ERROR               SPORT hardware or DMA error detected
 *
 *  - #ADI_SPORT_INVALID_HANDLE   [D]   Invalid SPORT device handle.
 *
 * @sa          adi_sport_SubmitBuffer()
 * @sa          adi_sport_IsBufferAvailable()
 *
 */
ADI_SPORT_RESULT adi_sport_GetBuffer(ADI_SPORT_HANDLE   const hDevice,
                                     void **            const ppBuffer,
                                     uint32_t *               pHwError)
{
    ADI_SPORT_RESULT result = ADI_SPORT_SUCCESS;
    ADI_SPORT_DEVICE * pDevice = (ADI_SPORT_DEVICE*) hDevice;   /* Pointer to SPORT device instance */

    *ppBuffer = NULL;
#ifdef ADI_DEBUG
    if (ADI_SPORT_SUCCESS != (result=ValidateHandle(pDevice)))  /* Validate the given handle */
    {
    }
    else
#endif /* ADI_DEBUG */
    if (NULL != pDevice->pfCallback)
    {
        result = ADI_SPORT_FAILED;
    } else {
        ADI_DT_CHANNEL * pSportChnl = &pDevice->sportChannel;

        SEM_PEND(pSportChnl,ADI_SPORT_FAILED);                          /* wait for a submitted buffer to be processed */

        ADI_INT_STATUS_ALLOC();
        ADI_ENTER_CRITICAL_REGION();
            *pHwError = pDevice->nHwError;
            pDevice->nHwError = 0u;
            *ppBuffer  = pSportChnl->pActiveBuffer->pStartAddress;      /* return the buffer start address in *ppBuffer */
            pSportChnl->pActiveBuffer->pStartAddress = NULL;            /* clear the free buffer address */
            pSportChnl->pActiveBuffer = pSportChnl->pActiveBuffer->pNextBuffer;
        ADI_EXIT_CRITICAL_REGION();
        if (0u != *pHwError)
        {
            result = ADI_SPORT_HW_ERROR;
        }
    }
    return result;
}

/**
 * @brief       Peek function to know whether an empty/filled buffer is available. Call to this
 *              function is valid only if the call back function is not registered. Call to this
 *              function results in error if a call back function is registered.
 *
 * @param [in]  hDevice      Device handle to SPORT device obtained when a SPORT device is opened
 *                           successfully.
 *
 * @param [out] pbAvailable  Pointer to a boolean variable. Contains "True" if there is an
 *                           empty/filled buffer and a call to #adi_sport_GetBuffer is ensured to be
 *                           successful. Contains "false" if there is no empty buffer.
 *
 * @return      Status
 *
 *  - #ADI_SPORT_SUCCESS                Successfully retrieved the status of availability of the buffer.
 *  - #ADI_SPORT_INVALID_HANDLE         Failed to retrieve the status of the buffer availability.
 *  - #ADI_SPORT_OPERATION_NOT_ALLOWED  Function cannot be called (no buffer to be processed or callback function registered).
 *  - ADI_SPORT_PERIPHERAL_ERROR        Hardware error detected
 *
 * @sa          adi_sport_GetBuffer()
 * @sa          adi_sport_GetBuffer()
 *
 */
ADI_SPORT_RESULT adi_sport_IsBufferAvailable(ADI_SPORT_HANDLE const hDevice,
                                             bool *           const pbAvailable)
{
    ADI_SPORT_RESULT result = ADI_SPORT_SUCCESS;
    ADI_SPORT_DEVICE * pDevice = (ADI_SPORT_DEVICE*) hDevice;   /* Pointer to SPORT device instance */

    *pbAvailable = false;
#ifdef ADI_DEBUG
    if (ADI_SPORT_SUCCESS != (result=ValidateHandle(pDevice)))  /* Validate the given handle */
    {
    }
    else
#endif /* ADI_DEBUG */
    if (NULL != pDevice->pfCallback)
    {
        result = ADI_SPORT_OPERATION_NOT_ALLOWED;
    }
    else
    {
        ADI_DT_BUFF_INFO * pActiveBuffer = pDevice->sportChannel.pActiveBuffer;

        if (pActiveBuffer->pStartAddress == NULL)
        {
            result = ADI_SPORT_OPERATION_NOT_ALLOWED;
        }
        else if (false == pActiveBuffer->bInUse)                /* this buffer has been processed by the SPORT */
        {
            *pbAvailable = true;
        }
        else
        {
        }
    }
    return result;
}

/**
 * @brief       Register and unregister a Callback function with the SPORT device driver.
 *              A registered call back function will be called, if not NULL, when a buffer
 *              is processed OR hardware error(s) encountered.
 *
 * @param [in]  hDevice     Device handle to SPORT device is obtained when a SPORT device is opened
 *                          successfully.
 *
 * @param [in]  pfCallback  Function pointer to Callback function. Passing a NULL pointer will
 *                          unregister the call back function.
 *
 * @param [in]  pCBparam    Call back function parameter.
 *
 * @return      Status
 *
 *  - #ADI_SPORT_SUCCESS                    Successfully registered specified callback function.
 *
 *  - #ADI_SPORT_INVALID_HANDLE        [D]  Invalid SPORT device handle.
 *
 *  - #ADI_SPORT_OPERATION_NOT_ALLOWED [D]  Operation is not allowed when data flow is enabled.
 *
 */
ADI_SPORT_RESULT adi_sport_RegisterCallback(ADI_SPORT_HANDLE    const hDevice,
                                            ADI_CALLBACK        const pfCallback,
                                            void *              const pCBparam)
{
    ADI_SPORT_RESULT result = ADI_SPORT_SUCCESS;
    ADI_SPORT_DEVICE * pDevice = (ADI_SPORT_DEVICE *) hDevice;   /* Pointer to SPORT device instance */
#ifdef ADI_DEBUG
    /* Validate the given handle */
    if (ADI_SPORT_SUCCESS != (result = ValidateHandle(pDevice)))
    {
    }
    /* Check if the data flow is already enabled */
    else if (ADI_SPORT_STATE_DATA_FLOW_ENABLED == pDevice->pSportInfo->eState)
    {
        /* Not allowed to register a callback if the data flow is enabled. */
        result = ADI_SPORT_OPERATION_NOT_ALLOWED;
    }
    else
#endif /* ADI_DEBUG */
    {
        ADI_INT_STATUS_ALLOC();
        ADI_ENTER_CRITICAL_REGION();
            pDevice->pfCallback = pfCallback;   /* Store the address of the callback function */
            pDevice->pCBParam   = pCBparam;     /* Store the call back parameter */
        ADI_EXIT_CRITICAL_REGION();
    }
    return result;
}

/**
 * @brief       Sets data format for the specified SPORT device.
 *
 * @details     Sets data type,Big endian (MSB first) OR Little endian (LSB first) and word
 *              length(in bits) for the specified SPORT device.This function return error if the
 *              device is already enabled.
 *
 * @param [in]  hDevice              Device handle to SPORT device.
 *
 * @param [in]  nWordLength          Specify the word size of the data. Valid range is from
 *                                   4(nWordLength = 3) to 32(nWordLength =31).
 *
 * @param [in]  bLSBFirst            Configure the specified SPORT device to operate either LSB
 *                                   first or MSB first.
 * \n
 * \n                                true : LSB first (Little endian) .
 * \n
 * \n                                false : MSB first (Big endian)
 *
 * @param [in]  ePackMode            Mode of packging need to configured. Please refer #ADI_SPORT_PACKING_MODE.
 *
 *
 * @return      Status
 *
 *  - #ADI_SPORT_SUCCESS                    Successfully configured the device to operate in
 *                                          specified data format.
 *
 *  - #ADI_SPORT_INVALID_HANDLE        [D]  Invalid SPORT device handle.
 *
 *  - #ADI_SPORT_INVALID_WORD_LENGTH   [D]  Invalid word size.
 *
 *  - #ADI_SPORT_OPERATION_NOT_ALLOWED [D]  Operation is not allowed when data flow is enabled.
 *
 */
ADI_SPORT_RESULT adi_sport_ConfigData(ADI_SPORT_HANDLE const            hDevice,
                                      const uint8_t                     nWordLength,
                                      const ADI_SPORT_PACKING_MODE      ePackMode,
                                      const bool                        bLSBFirst
                                      )
{
    ADI_SPORT_RESULT result = ADI_SPORT_SUCCESS;
    ADI_SPORT_DEVICE * pDevice = (ADI_SPORT_DEVICE *) hDevice;                  /* Pointer to SPORT device instance */
#ifdef ADI_DEBUG
    if (ADI_SPORT_SUCCESS != (result = ValidateHandle(pDevice)))
    {
    }
    if(pDevice->pSportInfo->eState == ADI_SPORT_STATE_DATA_FLOW_ENABLED)        /* Not allowed to change when data flow is enabled */
    {
        result = ADI_SPORT_OPERATION_NOT_ALLOWED;
    }
    else
#endif /* ADI_DEBUG */
    if (nWordLength > SPORT_WORD_TRANSFER_LENGTH)
    {
        result = ADI_SPORT_INVALID_WORD_LENGTH;
    }
    else
    {
        ADI_SPORT_DEVICE_INFO * pSportInfo = pDevice->pSportInfo;               /* short cut to SPORT information */
        ADI_SPORT_CONFIG * pSportCfg = &pSportInfo->sportCfg;                   /* short cut to SPORT configuration */
        const uint32_t bytesPerData = ((nWordLength < 9u) ? (1u) : ((nWordLength < 17u) ? (2u) : (4u)));

        const uint32_t wordPos = (uint32_t) BITP_SPORT_CTL_A_SLEN;
        const uint32_t wordLen = (uint32_t) nWordLength;
        const uint32_t ctlSlen = (wordLen - 1u) << wordPos;
        const uint32_t packMode = (uint32_t) ePackMode;
        const uint32_t ctlSlenBits = (0x1Fu  << wordPos);
        const uint32_t ctlDataMask = ~(BITM_SPORT_DATA_CONFIG | ctlSlenBits | BITM_SPORT_CTL_A_LSBF);

        uint32_t ctl = pDevice->pSportInfo->sportCfg.CTL;
        ctl &= ctlDataMask;                             /* clear all the fields(i.e Set to "0" ) */
        ctl |= (packMode | ctlSlen);                    /* assign packing and slen information */
        if (true == bLSBFirst)
        {
            ctl |= BITM_SPORT_CTL_A_LSBF;               /* set the the LSB first field */
        }
        pDevice->pSportInfo->sportCfg.CTL = ctl;        /* CTL value set - CTL_A is assigned when submitting a buffer */

        SPORT_CHECK_CFG_CTL(pDevice->pSportInfo->sportCfg.CTL);

        switch (bytesPerData)
        {
        case 1u:
            if (((uint32_t) ADI_SPORT_8BIT_PACKING) == packMode)
            {
                pSportCfg->DMA_WIDTH = ADI_DMA_WIDTH_4_BYTE;
                pSportCfg->DMA_INC = ADI_DMA_INCR_4_BYTE;
            } else {
                pSportCfg->DMA_WIDTH = ADI_DMA_WIDTH_1_BYTE;
                pSportCfg->DMA_INC = ADI_DMA_INCR_1_BYTE;

                assert(((uint32_t) ADI_SPORT_NO_PACKING) == packMode);
            }
            break;

        case 2u:
          if (((uint32_t) ADI_SPORT_16BIT_PACKING) == packMode)
          {
              pSportCfg->DMA_WIDTH = ADI_DMA_WIDTH_4_BYTE;
              pSportCfg->DMA_INC = ADI_DMA_INCR_4_BYTE;
          } else {
              pSportCfg->DMA_WIDTH = ADI_DMA_WIDTH_2_BYTE;
              pSportCfg->DMA_INC = ADI_DMA_INCR_2_BYTE;

              assert(((uint32_t) ADI_SPORT_NO_PACKING) == packMode);
          }
          break;

        default:
            pSportCfg->DMA_WIDTH = ADI_DMA_WIDTH_4_BYTE;
            pSportCfg->DMA_INC = ADI_DMA_INCR_4_BYTE;
            assert((4u == bytesPerData) || (((uint32_t) ADI_SPORT_NO_PACKING) == packMode));
            break;
        }
    }
    return result;
}

/**
 * @brief       Configure the clock for the specified SPORT device.
 *
 * @details     Configure the SPORT device to use the "internal/external " rising/falling clock
 *              edge,clock edge and for enabling the gated Clock Mode.
 *
 * @details     fspclk = fsclk/(2*( nClockRatio + 1))
 *
 * @details     fspclk: frequency of SPORT clock
 *
 * @param [in]  hDevice        Device handle to SPORT device.
 *
 * @param [in]  nClockRatio    The value which determines the ratio between System clock and SPORT
 *                             clock as explained above.
 *
 *
 * @param [in]  bUseIntlClock  Boolean flag to indicate whether to use internal clock or external
 *                             clock for data transmission. By default, device is configured to use
 *                             the external clock.
 * \n
 * \n                          true : Device configured to use Internal clock.
 * \n
 * \n                          false : Device configured to use external clock..
 *
 * @param [in]  bRisingEdge     Boolean flag to indicate whether to drive data and internal frame
 *                             sync with rising edge OR falling edge of SP clock.
 * \n
 * \n                          true : Use falling edge of the clock.
 * \n
 * \n                          false : Use rising edge of the clock.
 *
 * @param [in]  bGatedClk      Boolean flag to indicate whether to enable/disable gated clock for
 *                             the specified SPORT channel.Ignored in Multi channel mode. Clock will
 *                             be active only when active data is getting transmitted or received
 *                             when this mode is enabled.
 * \n                          true : Enable gated clock mode.
 * \n
 * \n                          false : Disable gated clock mode.
 *
 * @return      Status
 *
 *  - #ADI_SPORT_SUCCESS                    Successfully configured clock for the specified device.
 *
 *  - #ADI_SPORT_INVALID_HANDLE        [D]  Invalid SPORT device handle.
 *
 *  - #ADI_SPORT_OPERATION_NOT_ALLOWED [D]  Operation is not allowed when data flow is enabled.
 *
 */
ADI_SPORT_RESULT adi_sport_ConfigClock(ADI_SPORT_HANDLE const      hDevice,
                                       const uint16_t              nClockRatio,
                                       const bool                  bUseIntlClock,
                                       const bool                  bRisingEdge,
                                       const bool                  bGatedClk)
{
    ADI_SPORT_RESULT result = ADI_SPORT_SUCCESS;
    ADI_SPORT_DEVICE * pDevice = (ADI_SPORT_DEVICE *) hDevice;  /* Pointer to SPORT device instance */

#ifdef ADI_DEBUG
    if (ADI_SPORT_SUCCESS != (result = ValidateHandle(pDevice)))
    {
    }
    else if (ADI_SPORT_STATE_DATA_FLOW_ENABLED == pDevice->pSportInfo->eState)
    {
        result = ADI_SPORT_OPERATION_NOT_ALLOWED;
    }
    else
#endif /* ADI_DEBUG */
    {
        ADI_SPORT_CONFIG * pSportCfg = &pDevice->pSportInfo->sportCfg;
        uint32_t clockRatio = (uint32_t) nClockRatio;

        uint32_t ctl = pSportCfg->CTL;
        uint32_t dv = pSportCfg->DIV;

        ctl &= ~BITM_SPORT_CLOCK_CONFIG;                /* clear all clock configuration fields */

        dv &= ~BITM_SPORT_DIV_A_CLKDIV;
        dv |= (clockRatio & BITM_SPORT_DIV_A_CLKDIV);   /* update the clock divisior value */

        if (true == bUseIntlClock)
        {
            ctl  |= BITM_SPORT_CTL_A_ICLK;              /* select the internal clock */
        }
        if (true == bRisingEdge)
        {
            ctl |= BITM_SPORT_CTL_A_CKRE;               /* select the rising edge of the  clock */
        }
        if (true == bGatedClk)
        {
            ctl |= BITM_SPORT_CTL_A_GCLKEN;             /* Enable the Gated clock */
        }
        pDevice->pSportInfo->pSportRegs->DIV_A = pSportCfg->DIV = dv;  /* DIV value set */
        pSportCfg->CTL = ctl;                           /* CTL value set - CTL_A is assigned when submitting a buffer */

        SPORT_CHECK_CFG_CTL(pSportCfg->CTL);
    }
    return result;
}

/**
 * @brief       Frame Sync(FS) configuration for the specified SPORT.
 *
 * @details     Configure the SPORT to use internal/external frame sync,level/edge sensitive
 *              early/late frame sync etc.
 *
 * @param [in]  hDevice           Device handle to SPORT device.
 *
 * @param [in]  nFsDivisor        The value which decides the number of SPORT clock cycles between
 *                                each frame count.
 *
 * @param [in]  bFSRequired       Boolean flag to indicate whether frame sync required OR not to
 *                                the frame sync for the data transfer.
 * \n
 * \n                             true : Device requires a frame sync for its operation.
 * \n
 * \n                             false : Device does not requires a frame sync for its operation
 * \n
 * \n
 *
 * @param [in]  bInternalFS       Boolean flag to indicate whether to configure the specified SPORT
 *                                device to use the internal frame sync OR external frame sync as
 *                                below.
 * \n
 * \n                             true : Use internal frame sync.
 * \n
 * \n                             false : Use external frame sync
 * \n
 * \n
 *
 * @param [in]  bDataFS           Boolean flag to indicate whether to configure the specified SPORT
 *                                device to use the data-independent frame sync OR Serial port uses
 *                                a data-dependent frame sync. Valid only if the specified device is
 *                                in "transmit"(TX)mode . Ignored if the device is opened in
 *                                "receive"(RX) mode.
 * \n
 * \n                             true : Use data-independent frame sync.
 * \n
 * \n                             false : Use data-dependent frame sync.
 * \n
 * \n
 *
 * @param [in]  bActiveLowFS      Boolean flag to indicate whether to configure the specified SPORT
 *                                device for active low frame sync OR active high frame sync. Call
 *                                to this function will return error if SPORT is configured in I2S
 *                                mode.
 * \n
 * \n                             true : Use active low frame sync.
 * \n
 * \n                             false : Use active high frame sync.
 * \n
 * \n
 *
 * @param [in]  bLateFS           Boolean flag to indicate whether to use the late frame sync OR
 *                                Early frame sync.
 * \n
 * \n                             true : Use late frame sync.
 * \n
 * \n                             false : Use Early frame sync.
 * \n
 * \n
 *
* @param [in]  bFSErrorOperation  Frame Sync Error Operation.  This
 *\n                              decides the way the SPORT responds when a frame sync error occurs.
 * \n
 * \n                             true : When frame Sync error occurs, discard the receive data.
 * \n
 * \n                             false : Flag the Frame Sync error and continue normal operation
    * \n
 * \n
 *
 * @return      Status
 *
 *  - #ADI_SPORT_SUCCESS                    Successfully configured the frame sync requirement.
 *
 *  - #ADI_SPORT_INVALID_HANDLE        [D]  Invalid SPORT device handle.
 *
 *  - #ADI_SPORT_OPERATION_NOT_ALLOWED [D]  Operation is not allowed when data flow is enabled.
 *
 */
ADI_SPORT_RESULT adi_sport_ConfigFrameSync(ADI_SPORT_HANDLE const       hDevice,
                                           const uint16_t               nFsDivisor,
                                           const bool                   bFSRequired,
                                           const bool                   bInternalFS,
                                           const bool                   bDataFS,
                                           const bool                   bActiveLowFS,
                                           const bool                   bLateFS,
                                           const bool                   bFSErrorOperation)
{
    ADI_SPORT_DEVICE * pDevice = (ADI_SPORT_DEVICE *) hDevice;          /* Pointer to SPORT device instance */
    ADI_SPORT_RESULT result = ADI_SPORT_SUCCESS;

#ifdef ADI_DEBUG
    /* Validate the given handle */
    if (ADI_SPORT_SUCCESS != (result = ValidateHandle(pDevice)))
    {
    }
    else if(pDevice->pSportInfo->eState == ADI_SPORT_STATE_DATA_FLOW_ENABLED)
    {
        result = ADI_SPORT_OPERATION_NOT_ALLOWED;
    }
    else
#endif /* ADI_DEBUG */
    {
        ADI_SPORT_CONFIG * pSportCfg = &pDevice->pSportInfo->sportCfg;
        const uint32_t fsDivisor = (uint32_t) nFsDivisor;

        uint32_t ctl = pSportCfg->CTL;
        uint32_t dv = pSportCfg->DIV;

        dv &= ~BITM_SPORT_DIV_A_FSDIV;                  /* clear all the fields of frame sync */
        dv |= (fsDivisor << BITP_SPORT_DIV_A_FSDIV);

        ctl &= ~BITM_SPORT_FS_CONFIG;                   /* clear all the fields of frame sync */

        if ((ADI_SPORT_DIR_RX == pDevice->eDirection) || (true == bDataFS))
        {
            ctl |= BITM_SPORT_CTL_A_DIFS;               /* Set this bit when SPORT is opened in RX mode */
        }
        if (true == bFSRequired)                        /* "Frame sync required" is reserved when device */
        {                                               /* is operating in I2S and MC mode */
            ctl |= BITM_SPORT_CTL_A_FSR;                /* Frame Sync(FS) is required */
        }
        if (true == bInternalFS)
        {
            ctl |= BITM_SPORT_CTL_A_IFS;                /* Select the internal Frame Sync(FS)*/
        }
        if (true == bActiveLowFS)
        {
            ctl |= BITM_SPORT_CTL_A_LFS;                /* Select the Active High Frame Sync(FS)*/
        }
        if (true == bLateFS)
        {
            ctl |= BITM_SPORT_CTL_A_LAFS;               /* Select the Late Frame Sync(FS)*/
        }
        if (true == bFSErrorOperation)
        {
            ctl |= BITM_SPORT_CTL_A_FSERRMODE;          /* Select the edge sensitive Frame Sync(FS)*/
        }
        pDevice->pSportInfo->pSportRegs->DIV_A = pSportCfg->DIV = dv;   /* DIV value set */
        pSportCfg->CTL = ctl;                           /* CTL value set - CTL_A is assigned when submitting a buffer */

        SPORT_CHECK_CFG_CTL(pSportCfg->CTL);
    }
    return result;
}

/**
 * @brief       Configure the SPORT use the Clocks and Frame Sync of other Half-Sport
 *
 * @param [in]  hDevice       Device handle to SPORT device.
 *
 * @param [in]  bUseOtherFS   Boolean flag to indicate whether to use own Frame Sync(false) OR to
 *                            use frame sync of other half SPORT (true).
 * \n
 * \n                         true : Use frame sync of other half SPORT device.
 * \n
 * \n                         false : Use own frame sync.
 *
 * @param [in]  bUseOtherClk  Boolean flag to indicate whether to use own clock clock(false) OR to
 *                            use clock of other half SPORT(true).
 * \n
 * \n                         true : Use clock of other half SPORT device.
 * \n
 * \n                         false : Use own clock.
 *
 * @return      Status
 *
 *  - #ADI_SPORT_SUCCESS                    Successfully enabled the specified SPORT to use the clk
 *                                          and FS of other half SPORT.
 *
 *  - #ADI_SPORT_INVALID_HANDLE        [D]  Invalid SPORT device handle.
 *
 *  - #ADI_SPORT_OPERATION_NOT_ALLOWED [D]  Operation is not allowed when data flow is enabled.
 *
 */
ADI_SPORT_RESULT adi_sport_MultiplexSportSignal(ADI_SPORT_HANDLE const  hDevice,
                                                const bool              bUseOtherFS,
                                                const bool              bUseOtherClk)
{
    ADI_SPORT_RESULT result = ADI_SPORT_SUCCESS;
    ADI_SPORT_DEVICE * pDevice = (ADI_SPORT_DEVICE *)hDevice;   /* Pointer to SPORT device instance */
#ifdef ADI_DEBUG
    if((result = ValidateHandle(pDevice)) != ADI_SPORT_SUCCESS) /* Validate the given handle */
    {
    }
    else if (pDevice->pSportInfo->eState == ADI_SPORT_STATE_DATA_FLOW_ENABLED)
    {
        result = ADI_SPORT_OPERATION_NOT_ALLOWED;
    }
    else
#endif /* ADI_DEBUG */
    {
        ADI_SPORT_CONFIG * pSportCfg = &pDevice->pSportInfo->sportCfg;
        uint32_t ctl = pSportCfg->CTL;

        /* clear the muxing fields of the control register 2 */
        ctl &= (uint32_t)(~(BITM_SPORT_CTL_A_CKMUXSEL | BITM_SPORT_CTL_A_FSMUXSEL));
        if (true == bUseOtherFS)
        {
            ctl |= BITM_SPORT_CTL_A_FSMUXSEL;   /* Use the  the frame sync of  other half sport*/
        }
        if(bUseOtherClk  == true)
        {
            ctl |= BITM_SPORT_CTL_A_CKMUXSEL;   /* Use the  the clock of  other half sport*/
        }
        pSportCfg->CTL = ctl;                   /* CTL value set - CTL_A is assigned when submitting a buffer */

        SPORT_CHECK_CFG_CTL(pSportCfg->CTL);
    }

    return result;
}
/**
 * @brief       Configure the SPORT use the Clocks and Frame Sync of other Half-Sport
 *
 * @param [in]  hDevice       Device handle to SPORT device.
 *
 * @param [in]  nFSDuration  Specify the value of the number of clocks which would be programmed corresponding to the
 *                           desired time duration from assertion of CONVT signal to Frame sync signal
 *
 * @param [in]  nWidth       Specify the value of the number of serial clocks for which CONVT signal should be active.

 *
 * @param [in]  bActiveLow    Boolean flag to indicate the polarity of the Convt signal.
 * \n
 * \n                         true :  Active low Polarity.
 * \n
 * \n                         false : Active High Polarity.
 *
 * @return      Status
 *
 *  - #ADI_SPORT_SUCCESS                    Successfully enabled the specified SPORT to use the clk
 *                                          and FS of other half SPORT.
 *
 *  - #ADI_SPORT_INVALID_HANDLE        [D]  Invalid SPORT device handle.
 *
 *  - #ADI_SPORT_OPERATION_NOT_ALLOWED [D]  Operation is not allowed when data flow is enabled.
 *
 */
ADI_SPORT_RESULT adi_sport_ConfigTimerMode(ADI_SPORT_HANDLE const       hDevice,
                                           const uint8_t                nFSDuration,
                                           const uint8_t                nWidth,
                                           const bool                   bActiveLow)
{
    ADI_SPORT_RESULT result = ADI_SPORT_SUCCESS;
    ADI_SPORT_DEVICE * pDevice = (ADI_SPORT_DEVICE*) hDevice;   /* Pointer to SPORT device instance */

#ifdef ADI_DEBUG    /* Validate the given handle */
    if (ADI_SPORT_SUCCESS != (result = ValidateHandle(pDevice)))
    {
    }
    else if (ADI_SPORT_STATE_DATA_FLOW_ENABLED == pDevice->pSportInfo->eState)
    {
        result = ADI_SPORT_OPERATION_NOT_ALLOWED;
    }
    else
#endif /* ADI_DEBUG */
    {
        ADI_SPORT_CONFIG * pSportCfg = &pDevice->pSportInfo->sportCfg;
        uint32_t cnvt = pSportCfg->TIM_CONVT;

        /* clear the muxing fields of the control register 2 */
        cnvt &= (uint32_t)(~(BITM_SPORT_CNVT_A_POL |  BITM_SPORT_CNVT_A_WID | BITM_SPORT_CNVT_A_CNVT2FS ));
        cnvt |= (((uint32_t) nFSDuration << ((uint32_t) BITP_SPORT_CNVT_A_CNVT2FS)) | ((uint32_t) nWidth));
        if(bActiveLow == true)
        {
          cnvt |= ((uint32_t) BITM_SPORT_CNVT_A_POL);   /* Use the  the clock of  other half sport*/
        }
        pDevice->pSportInfo->pSportRegs->CNVT_A = pSportCfg->TIM_CONVT = cnvt;
    }
    return result;
}

/*! \cond PRIVATE */

/**
 * @brief       Create a circular linked list for buffer management.
 *
 * @details     Create a circular linked list for buffer management and
 *              initialize the free buffer, the fill buffer and he active
 *              buffer with the first buffer in this circular array.
 *
 * @param [in]  hDevice  Device handle to SPORT device.
 *
 * @param [in]  NumDesc  Number of descriptorS.
 *
 */
static inline void sport_Init (ADI_SPORT_DEVICE *pDevice)
{
    uint32_t            i;
    ADI_DT_CHANNEL      *pChannel = &pDevice->sportChannel;
    ADI_DT_BUFF_INFO    *pBufInfo = &pChannel->BufInfo[0];              /* initialize this variable with the first array element */
    ADI_SPORT_DEVICE_INFO * pSportInfo = pDevice->pSportInfo;           /* short cut to SPORT information */
    ADI_SPORT_CONFIG * pSportCfg = &pSportInfo->sportCfg;               /* short cut to SPORT configuration */
    const uint32_t bytesPerData = GetBytesPerSportData(pSportCfg->CTL); /* number of bytes in SPORT data (1, 2, or 4) */
    const uint32_t packMode = SPORT_GET_PACKEN(pSportCfg->CTL);         /* SPORT data pack mode */

    /* Initialize the all descriptors. Make it circular. */
    for(i = 0u; i < ADI_DT_BUFNUM; i++)
    {
        pBufInfo[i].pStartAddress  =   NULL;
        pBufInfo[i].nCount         =   0u;
        pBufInfo[i].nIndex         =   0u;
        pBufInfo[i].pNextBuffer    =   &pBufInfo[(i+1u) % ADI_DT_BUFNUM];       /* link the buffers in a circular way */
    }
    pChannel->pFreeBuffer   =  &pChannel->BufInfo[0u];  /* the first free buffer is the first array element */
    pChannel->pActiveBuffer =  &pChannel->BufInfo[0u];  /* the first active buffer is the first array element */
    pChannel->pFillBuffer   =  &pChannel->BufInfo[0u];  /* the first fill buffer is the first array element */

    switch (bytesPerData)
    {
    case 1u:
        if (SPORT_BIT_PACK_8 == packMode)
        {
            pSportCfg->DMA_WIDTH = ADI_DMA_WIDTH_4_BYTE;
            pSportCfg->DMA_INC = ADI_DMA_INCR_4_BYTE;
        } else {
            pSportCfg->DMA_WIDTH = ADI_DMA_WIDTH_1_BYTE;
            pSportCfg->DMA_INC = ADI_DMA_INCR_1_BYTE;

            assert(SPORT_BIT_PACK_NONE == packMode);
        }
        break;

    case 2u:
        if (SPORT_BIT_PACK_16 == packMode)
        {
            pSportCfg->DMA_WIDTH = ADI_DMA_WIDTH_4_BYTE;
            pSportCfg->DMA_INC = ADI_DMA_INCR_4_BYTE;
        } else {
            pSportCfg->DMA_WIDTH = ADI_DMA_WIDTH_2_BYTE;
            pSportCfg->DMA_INC = ADI_DMA_INCR_2_BYTE;

            assert(SPORT_BIT_PACK_NONE == packMode);
        }
        break;

  default:
        pSportCfg->DMA_WIDTH = ADI_DMA_WIDTH_4_BYTE;
        pSportCfg->DMA_INC = ADI_DMA_INCR_4_BYTE;
        assert((4u == bytesPerData) || (SPORT_BIT_PACK_NONE == packMode));
        break;
    }
}

/*
 * @brief       Configure the registers with the half-SPORT
 *
 * @param [in]  hDevice         Device handle to SPORT device.
 * @param [in]  sportCfg     SPORT configuration to be used.
 *
 * @return      None
 */
static inline void sport_Configure (ADI_SPORT_DEVICE *pDevice, ADI_SPORT_CONFIG const * sportCfg)
{
    /* Configure the SPORT device using static configuration parameters.
     * pSportInfo is mapped to one of the half-SPORT available; this is the
     * half-SPORT configured. (CTL_A, DIV_A, CNVT_A and NUMTRAN_A map either
     * to half-SPORT A registers or half-SPORT B registers, depending on
     * sportRegs.)
     */
    volatile ADI_SPORT_TypeDef * sportRegs = pDevice->pSportInfo->pSportRegs;
    ADI_SPORT_CONFIG * pSportCfg = &pDevice->pSportInfo->sportCfg;

    /* record the SPORT default configuration */
    memcpy(pSportCfg, sportCfg, sizeof(ADI_SPORT_CONFIG));

    switch (pDevice->eDirection)        /* Set the direction of operation */
    {
    case ADI_SPORT_DIR_RX:
        pSportCfg->CTL &= ~BITM_SPORT_CTL_A_SPTRAN;
        break;
    case ADI_SPORT_DIR_TX:
        pSportCfg->CTL |= BITM_SPORT_CTL_A_SPTRAN;
        break;
    default:
        assert(0);
        break;
    }
    /* use the SPORT configuration to setup the SPORT registers */
    sportRegs->CTL_A      = pSportCfg->CTL;
    sportRegs->DIV_A      = pSportCfg->DIV;
    sportRegs->CNVT_A     = pSportCfg->TIM_CONVT;
    sportRegs->NUMTRAN_A  = 0u;

    SPORT_CHECK_CFG_CTL(pSportCfg->CTL);
}

#ifdef ADI_DEBUG
static ADI_SPORT_RESULT ValidateHandle(ADI_SPORT_HANDLE const hDevice)
{
    ADI_SPORT_RESULT result = ADI_SPORT_INVALID_HANDLE;
    ADI_SPORT_DEVICE * pInDevice = (ADI_SPORT_DEVICE*) hDevice;
    ADI_SPORT_DEVICE_INFO *poDeviceInfo = &gSportDevInfo[0][0];
    uint32_t i;

    /* Pointer to SPORT device instance */
    for (i=0u; i<(ADI_SPORT_NUM_INSTANCES << 1u); i++)        /* 2 half-devices per SPORT */
    {
        if (pInDevice == poDeviceInfo->hDevice)
        {
            result = ADI_SPORT_SUCCESS;
            break;
        }
        poDeviceInfo++;
    }
    return result;
}
#endif /* ADI_DEBUG */

/* mask for events to be recorded in the driver HW error */
#define recEvt  ((uint32_t) (BITM_SPORT_STAT_A_SYSDATERR | BITM_SPORT_STAT_A_FSERR | BITM_SPORT_STAT_A_DERR))

/* bits to be cleared by the ISR */
#define clrEvt  ((recEvt | BITM_SPORT_STAT_A_TFI))

static void sport_Terminate(ADI_SPORT_DEVICE * pDevice)
{
    ADI_SPORT_DEVICE_INFO * pSportInfo = pDevice->pSportInfo;           /* get SPORT device information */
    volatile ADI_SPORT_TypeDef * pRegs = pSportInfo->pSportRegs;        /* access SPORT registers */

    ADI_DT_CHANNEL * pSportChnl = &pDevice->sportChannel;
    ADI_DT_BUFF_INFO * pBuff = pSportChnl->pFillBuffer;

    pRegs->CTL_A &= ~BITM_SPORT_CTL_A_SPEN;                             /* disable SPORT */
    pRegs->IEN_A &= ~(BITM_SPORT_IEN_A_TF | BITM_SPORT_IEN_A_DATA);     /* disable SPORT interrupts */
    pRegs->NUMTRAN_A = 0u;

#ifdef ADI_DEBUG
    {
        /* ============================================= */
        /* Check the number of data transmitted/received */
        /* nIndex is incremented each time a data packed */
        /* or unpacked in received. The size in bytes of */
        /* each data depends on the SPORT configuration. */
        /* In core driven operations, nCount represents  */
        /* the number of 32-bit words transmitted.       */
        /* In DMA driven operations, nCount represents   */
        /* the number of DMA data transmitted            */
        /* ============================================= */
        const uint32_t ctl = pRegs->CTL_A;
        const uint32_t bytesPerData = GetBytesPerSportData(ctl);
        const uint32_t nIndex = pBuff->nIndex * (4u / bytesPerData);
        assert((nIndex>=pBuff->nCount)||(true==pBuff->bDMA));           /* buffer must be fully processed */
    }
#endif

    pBuff->bInUse = false;                                              /* mark buffer as ready */

    NVIC_DisableIRQ(pSportInfo->eIRQn);                                 /* suspend SPORT Interrupt */
    NVIC_DisableIRQ(pSportInfo->eDMAn);                                 /* suspend SPORT DMA interrupt */

    pDevice->pSportInfo->eState = ADI_SPORT_STATE_PAUSED;

    if(NULL != pDevice->pfCallback)                                     /* Call the callback function if one is registered. */
    {
        uint32_t evt = ( (ADI_SPORT_DIR_RX == pDevice->eDirection)
                       ? ((uint32_t) ADI_SPORT_EVENT_RX_BUFFER_PROCESSED)
                       : ((uint32_t) ADI_SPORT_EVENT_TX_BUFFER_PROCESSED)
                       );

        pDevice->pfCallback(pDevice->pCBParam,evt,pBuff->pStartAddress);
        pBuff->pStartAddress = NULL;                                    /* No need to keep the processed buffer address */
    }
    else
    {
        SEM_POST(pSportChnl);                                           /* signal the buffer availability through a semaphore */
    }
    pRegs->STAT_A = clrEvt;                                             /* clear status register bits (W1C) */
    pSportChnl->eDataTranferMode = ADI_DT_MODE_NONE;                    /* SPORT is available */
    pBuff = pBuff->pNextBuffer;                                         /* point to the next buffer to process */
    pSportChnl->pFillBuffer = pBuff;                                    /* this is the new pFillBuffer */

    if ((0u != pBuff->pStartAddress) && (true == pBuff->bInUse))        /* valid buffer not being processed yet */
    {
        ADI_SPORT_RESULT result;

        pSportChnl->eDataTranferMode = ADI_DT_MODE_NONBLOCKING;
        if (true == pBuff->bDMA)
        {
            result = sport_SubmitBufferDmaMode(pDevice, pBuff);
        }
        else
        {
            result = sport_SubmitBufferIntMode(pDevice, pBuff);
        }
        
        if(ADI_SPORT_SUCCESS != result)                                 /* if an error occurred...*/
        {
            pSportChnl->eDataTranferMode = ADI_DT_MODE_NONE;            /* SPORT is available */
        }
    }
}

/*
 * @brief       Common SPORT interrupt handler function called by SPORT0 A and SPORT0 B ISRs.
 *
 * @details     Process SPORT0 A and B interrupts, recording HW errors that must be reported,
 *              reading/writing transmitted data, launching new SPORT transmissions if more
 *              buffers are to be processed, and deactivating the SPORT device if there are
 *              no pending requests. (Common fucntion for both core driven and DMA driven
 *              SPORT operations.)
 *
 * @param [in]  pDevice         Sport device pointer related to the calling ISR.
 */
static void sport_InterruptHandler(ADI_SPORT_DEVICE * pDevice)
{
    ADI_SPORT_DEVICE_INFO * pSportInfo = pDevice->pSportInfo;           /* get SPORT device information */
    volatile ADI_SPORT_TypeDef * pRegs = pSportInfo->pSportRegs;        /* access SPORT registers */
    const uint32_t sportStatus = pRegs->STAT_A;                         /* read SPORT status */
    const uint32_t dataRequest = (sportStatus & BITM_SPORT_STAT_A_DATA);/* set if any data to be processed by the SPORT */
    const uint32_t hwEvents = sportStatus & recEvt;                     /* HW events to be recorded in the driver */


    /* This implementation assumes an identity mapping between BITM_SPORT_STAT values
     * and their equivalent event in ADI_SPORT_EVENT, e.g. ADI_SPORT_HW_ERR_FS and
     * BITM_SPORT_STAT_A_FSERR share the same value. This simplifies event processing
     * and reports. */
    assert(((uint32_t) ADI_SPORT_HW_ERR_RX_OVERFLOW) == BITM_SPORT_STAT_A_DERR);
    assert(((uint32_t) ADI_SPORT_HW_ERR_TX_UNDERFLOW) == BITM_SPORT_STAT_A_DERR);
    assert(((uint32_t) ADI_SPORT_HW_ERR_FS) == BITM_SPORT_STAT_A_FSERR);
    assert(((uint32_t) ADI_SPORT_HW_ERR_SYSDATAERR) == BITM_SPORT_STAT_A_SYSDATERR);

    if (0u != hwEvents)                                                 /* any event recorded? */
    {
        if (NULL != pDevice->pfCallback)                                /* if a callback has been registered ? */
        {
            pDevice->pfCallback(pDevice->pCBParam,hwEvents,NULL);       /* then call it */
        } else {
            pDevice->nHwError |= hwEvents;                              /* else set the driver HW error */
            SEM_POST(&pDevice->sportChannel);                           /* and signal this through a semaphore */
        }
    }

    if (0u != dataRequest)                                              /* Tx FIFO is not full or Rx FIFO is not empty */
    {
        ADI_DT_BUFF_INFO * pBuff = pDevice->sportChannel.pFillBuffer;
        uint32_t * pNextWord = (uint32_t*) pBuff->pStartAddress;

        if ((NULL != pNextWord) && (pBuff->nIndex < pBuff->nCount))     /* This buffer has not been fully processed yet */
        {
            if (ADI_SPORT_DIR_RX == pDevice->eDirection)
            {
                pNextWord[pBuff->nIndex++] = pRegs->RX_A;               /* Read the data received in RX and increment the index */
                while (!DXS_FIFO_IS_EMPTY(pRegs->STAT_A))               /* and if there are more data available in the FIFO */
                {
                    pNextWord[pBuff->nIndex++] = pRegs->RX_A;           /* Read remaining data received in RX and increment the index */
                }
            }
            else
            {
                pRegs->TX_A = pNextWord[pBuff->nIndex++];               /* Write the data to be sent into TX and increment the index */
                while (  (pBuff->nIndex < pBuff->nCount)                /* and if there are more data to be sent */
                      && (!DXS_FIFO_IS_FULL(pRegs->STAT_A))             /* and there is still room in the FIFO */
                      )
                {
                    pRegs->TX_A = pNextWord[pBuff->nIndex++];           /* then write more data to be sent into TX and increment the index */
                }
            }
        }
    }

    /* ========================================================== */
    /* Common to core driven operations and DMA driven operations */
    /* ========================================================== */
    if (0u != (pRegs->STAT_A & BITM_SPORT_STAT_A_TFI))                  /* If a SPORT Tx/Rx request has finished */
    {
        sport_Terminate(pDevice);
    }

#if defined(ADI_CYCLECOUNT_SPORT_ISR_ENABLED) && (ADI_CYCLECOUNT_SPORT_ISR_ENABLED == 1u)
    ADI_CYCLECOUNT_STORE(ADI_CYCLECOUNT_ISR_SPORT);    
#endif
}

/** Interrupt handler for SPORT0 A */
void SPORT0A_Int_Handler(void)
{
    ISR_PROLOG();
        sport_InterruptHandler(gSportDevInfo[0][ADI_HALF_SPORT_A].hDevice);
    ISR_EPILOG();
}

/** Interrupt handler for SPORT0 B */
void SPORT0B_Int_Handler(void)
{
    ISR_PROLOG();
        sport_InterruptHandler(gSportDevInfo[0][ADI_HALF_SPORT_B].hDevice);
    ISR_EPILOG();
}

void DMA_SPORT0A_Int_Handler(void)
{
    ISR_PROLOG();
        /**
         * if SPORT is in Rx mode, then the DMA interrupt is the signal for
         * end of transmission: buffer is ready. (In Tx mode, the signal is
         * the TFI event and SPORT DMA interrup is not enabled).
         */
        sport_Terminate(gSportDevInfo[0][ADI_HALF_SPORT_A].hDevice);
#if defined(ADI_CYCLECOUNT_SPORT_ISR_ENABLED) && (ADI_CYCLECOUNT_SPORT_ISR_ENABLED == 1u)
        ADI_CYCLECOUNT_STORE(ADI_CYCLECOUNT_ISR_SPORT);    
#endif
    ISR_EPILOG();
}

void DMA_SPORT0B_Int_Handler(void)
{
    ISR_PROLOG();
        /**
         * if SPORT is in Rx mode, then the DMA interrupt is the signal for
         * end of transmission: buffer is ready. (In Tx mode, the signal is
         * the TFI event and SPORT DMA interrup is not enabled).
         */
        sport_Terminate(gSportDevInfo[0][ADI_HALF_SPORT_B].hDevice);
#if defined(ADI_CYCLECOUNT_SPORT_ISR_ENABLED) && (ADI_CYCLECOUNT_SPORT_ISR_ENABLED == 1u)
        ADI_CYCLECOUNT_STORE(ADI_CYCLECOUNT_ISR_SPORT);    
#endif
    ISR_EPILOG();
}

static void sport_DmaErrorCallback(void *pCBParam, uint32_t Event, void *pArg)
{
    ADI_SPORT_DEVICE * pDevice = (ADI_SPORT_DEVICE*) pCBParam;          /* Recover the device handle. */
    ADI_DT_BUFF_INFO * pFillBuffer = pDevice->sportChannel.pFillBuffer;
    ADI_DT_BUFF_INFO * pNextBuffer = pFillBuffer->pNextBuffer;
    uint32_t nEvent = 0u;

    if (ADI_DMA_EVENT_ERR_BUS == Event)
    {
        nEvent = (uint32_t) ADI_SPORT_DMA_ERR_BUS;                      /* SPORT DMA bus error detected  */
    } else {
        assert(ADI_DMA_EVENT_ERR_INVALID_DESCRIPTOR == Event);
        nEvent = (uint32_t) ADI_SPORT_DMA_ERR_INVALID_DESCRIPTOR;       /* SPORT DMA invalid descriptor error detected  */
    }

    pDevice->nHwError |= nEvent;
    sport_InterruptHandler(pDevice);

    while (  (NULL != pNextBuffer->pStartAddress)
          && (true == pNextBuffer->bInUse)
          && (true == pNextBuffer->bDMA)
          )                                                             /* another buffer is pending for a DMA driven request */
    {
        pDevice->nHwError |= nEvent;
        pNextBuffer->bInUse = false;
        sport_InterruptHandler(pDevice);
        pNextBuffer = pNextBuffer->pNextBuffer;
    }
}

static inline uint32_t GetBytesPerSportData(const uint32_t ctlVal)
{
    const uint32_t wlen = SPORT_GET_WLEN(ctlVal);
    const uint32_t bytesPerData = ((wlen < 9u) ? (1u) : ((wlen < 17u) ? (2u) : (4u)));
    return bytesPerData;
}

/*! \endcond */

/*@}*/
