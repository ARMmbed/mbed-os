/*!
 *****************************************************************************
 * @file:    adi_dma.c
 * @brief:   uDMA Device Implimentation for ADuC320x
 * @version: $Revision: 33333 $
 * @date:    $Date: 2016-01-22 16:39:48 +0000 (Fri, 22 Jan 2016) $
 *-----------------------------------------------------------------------------
 *
Copyright (c) 2010-2016 Analog Devices, Inc.

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

THIS SOFTWARE IS PROVIDED BY ANALOG DEVICES, INC. AND CONTRIBUTORS "AS IS" AND ANY
EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, NON-INFRINGEMENT,
TITLE, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN
NO EVENT SHALL ANALOG DEVICES, INC. OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT,
INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, PUNITIVE OR CONSEQUENTIAL DAMAGES
(INCLUDING, BUT NOT LIMITED TO, DAMAGES ARISING OUT OF CLAIMS OF INTELLECTUAL
PROPERTY RIGHTS INFRINGEMENT; PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS
OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 *****************************************************************************/

#ifndef _ADI_DMA_PL230_V2_C_
#define _ADI_DMA_PL230_V2_C_

/*! \addtogroup DMA_Service DMA Service
 *  uDMA Service
 *  @{
 */

#include <services/int/adi_int.h>
#include <string.h>
#include <assert.h>

/*=============  I N C L U D E S   =============*/
#include <services/dma/adi_dma.h>
#include <ssdd_common/common_def.h>
/*! \cond PRIVATE */

/*=============  M I S R A   =============*/

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
* Pm152 (rule 17.4): array indexing shall only be applied to objects defined as an array type
*   The two halves of the descriptor array are accessed through pointers.
*
* Pm140 (rule 11.3): a cast should not be performed between a pointer type and an integral type
*   The rule makes an exception for memory-mapped register accesses.
*/
#pragma diag_suppress=Pm123,Pm073,Pm143,Pm050,Pm152,Pm140
#endif /* __ICCARM__ */

/*=============  D E F I N E S  =============*/

/* CCD array allocation macros */
#define CCD_ALIGN (0x400) /* Memory alignment required for CCD array */
#if defined(__ADUCM30xx__)
#define CCD_SIZE (32u)	/* Configure CCD allocation as an integral power of two,
                            i.e., 24 channels is allocated as 32 */
#else
#error "Unknown processor family"
#endif

/* Maximum Transfer Size of the DMA */
#define DMA_MAX_TRANSFER_SIZE   (1024u)

/* Number of internal buffers */
#define NUM_BUFFERS   (2)


/*=============  R E G I S T E R    D E F I N E S  =============*/

/* Bit Position for DMA Descriptor Control */
#define DMA_BITP_CTL_DST_INC         (30u)
#define DMA_BITP_CTL_SRC_INC         (26u)
#define DMA_BITP_CTL_SRC_SIZE        (24u)
#define DMA_BITP_CTL_R_POWER         (14u)
#define DMA_BITP_CTL_N_MINUS_1        (4u)
#define DMA_BITP_CTL_CYCLE_CTL        (0u)

/* Bit Mask for DMA Descriptor Control */
#define DMA_BITM_CTL_DST_INC         ((0x00000003u) << DMA_BITP_CTL_DST_INC)
#define DMA_BITM_CTL_SRC_INC         ((0x00000003u) << DMA_BITP_CTL_SRC_INC)
#define DMA_BITM_CTL_SRC_SIZE        ((0x00000003u) << DMA_BITP_CTL_SRC_SIZE)
#define DMA_BITM_CTL_R_POWER         ((0x0000000Fu) << DMA_BITP_CTL_R_POWER)
#define DMA_BITM_CTL_N_MINUS_1       ((0x000003FFu) << DMA_BITP_CTL_N_MINUS_1)
#define DMA_BITM_CTL_CYCLE_CTL       ((0x00000007u) << DMA_BITP_CTL_CYCLE_CTL)

/* Enum for the DMA Descriptor Cycle Control */
#define DMA_ENUM_CTL_CYCLE_CTL_INVALID     (0u)
#define DMA_ENUM_CTL_CYCLE_CTL_BASIC       (1u)
#define DMA_ENUM_CTL_CYCLE_CTL_AUTO_REQ    (2u)
#define DMA_ENUM_CTL_CYCLE_CTL_PING_PONG   (3u)
#define DMA_ENUM_CTL_CYCLE_CTL_MSG_PRI     (4u)
#define DMA_ENUM_CTL_CYCLE_CTL_MSG_ALT     (5u)
#define DMA_ENUM_CTL_CYCLE_CTL_PSG_PRI     (6u)
#define DMA_ENUM_CTL_CYCLE_CTL_PSG_ALT     (7u)


#define DMA_BITM_INCR_TYPE_DECR            (0x10u)

#define DMA_BITM_OCTL_SRC_DECR             (0x01u)
#define DMA_BITM_OCTL_DST_DECR             (0x02u)

#define DMA_BITM_OCTL_SRC_INCR             (0x04u)
#define DMA_BITM_OCTL_DST_INCR             (0x08u)



/*=============  T Y P E    D E F I N E S  =============*/

/* DMA Driver States */
typedef enum {
  DMA_STATE_UNKNOWN,         /* Unknown State */
  DMA_STATE_INITIALIZED     /* Initialized State */
} DMA_STATE;

/* DMA Buffers Structure */
typedef struct _DMA_BUFFER {
    struct _DMA_BUFFER*     pNext;               /* Pointer to the next buffer */
    __I __C void*           pSrcData;            /* Pointer to the source */
    __IO void*              pDstData;            /* Pointer to the destination */

    uint32_t                nCurPos;             /* Current Position*/
    uint32_t                nRemTransfers;       /* Remaining number of transfer
                                                  * that is not scheduled in the
                                                  * DMA
                                                  */
    uint32_t                nRemDoneTransfers;   /* Remaining number of transfers
                                                  * that are not yet acknowledged
                                                  * by the driver
                                                  */
    uint32_t                nCfgValues;          /* Configuration values */

    uint32_t                nOtherCfgValues;     /* Some configuration values which
                                                  * are not part of the descriptor
                                                  * structure for the Buffer
                                                  */
} DMA_BUFFER;


/* DMA Channel Structure */
typedef struct _DMA_CHANNEL {
    uint32_t            ChannelID;              /* Channel ID */
    ADI_CALLBACK        pfCallback;             /* Pointer to the callback func */
    void*               pCBParam;               /* Application Callback param */

    uint32_t            nCommonCfg;             /* Common Config Values */
    uint32_t            nOtherCmnCfg;
    uint8_t             nNumFreeDesc;           /* Number of free descriptors */
    bool_t              bIsPrimNext;            /* Boolean flag to signify whether
                                                 * primary is the next descritor
                                                 * to be used
                                                 */
    bool_t              bIsPrimInProgress;      /* Boolean flag which determine
                                                 * whether primary descriptor
                                                 * is the one that is in progress
                                                 */



    DMA_BUFFER*         pFreeBList;             /* Pointer to the free buffer list */

    DMA_BUFFER*         pPendBList;             /* Pointer to the pending buffer list */
    DMA_BUFFER*         pActiveBList;           /* Pointer to the active buffer list */

    DMA_BUFFER          Buffers[NUM_BUFFERS];   /* Buffers that need to be used */
} DMA_CHANNEL;

/*! \struct ADI_DMA_DEV_DATA
 * DMA Device instance data structure */
typedef struct {

   DMA_STATE      InitState;              /*!< track initialization state */
   DMA_CHANNEL*   ChnHandles[CCD_SIZE];
   uint32_t       SoftwareChns;           /*!< Mark the software channels */
   uint32_t       ChannelsInUse;          /*!< what channels are active   */
   uint32_t       ChannelsEnabled;
   uint32_t       ByteSwapInUse;          /*!< channels using ByteSwap    */
} ADI_DMA_DEV_DATA;

/*=============  D A T A  =============*/

/* DMA descriptor arrays must be contigious */
/* AND impose strict alignment requirements */
/* Each compiler has different alignment directives */
#if defined ( __ICCARM__ )
    #pragma data_alignment=CCD_ALIGN  /* IAR */
#elif defined (__CC_ARM)
    __align(CCD_ALIGN)                /* Keil */
#elif defined(__GNUC__) /* the alignment is not expressed here in GCC */
#else
    #pragma message("WARNING: NO ALIGHMENT DEFINED FOR DMA DESCRIPTOR BLOCKS")
#endif

/* ALIGNED: DMA channel control data array declaration */
static ADI_DCC_TypeDef gChannelControlDataArray[CCD_SIZE * 2u]
#if defined(__GNUC__) /* the alignment is not expressed here in GCC */
__attribute__ ((aligned (CCD_ALIGN)))
#endif
#ifdef ADI_DMA_DESCRIPTORS_IN_VOLATILE_MEMORY
	/* conditional placement of DMA descriptor table to volatile memory */
	@ "volatile_ram";
#else
	/* default placement to non-volatile memory (no override) */
	;
#endif

/* pointer to the primary CCD array */
static ADI_DCC_TypeDef* const pPrimaryCCD = &gChannelControlDataArray[0];

/* pointer to the alternate CCD array */
static ADI_DCC_TypeDef* const pAlternateCCD = &gChannelControlDataArray[CCD_SIZE];

/* DMA Device Driver Data Structure */
static ADI_DMA_DEV_DATA DMA_DevData = {

    DMA_STATE_UNKNOWN,                       /*!< Initialization State     */
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
     0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},  /*!< Channel Handles */
#if defined(__ADUCM30xx__)
    0x00FF0000ul,                            /*!< Mark the software channels */
#else
    0ul,
#endif
    0ul,                                     /*!< channels-in-use bitfield */
    0ul,
    0ul,                                     /*!< byteswap bitfield        */
};

/* pointer to init instance */
static ADI_DMA_DEV_DATA* const pDMA_DevData = &DMA_DevData;


/* pointer to init instance */

/*=============  Local function declarations  =============*/

/* Initialize the driver */
static void Initialize(DMA_CHANNEL* pChannel);

static void InitNewActiveBuffer(DMA_CHANNEL* pChannel) ;
static void SetupDevice (DMA_CHANNEL* pChannel);

/* Set the DMA increment */
static void SetupDMAIncrement (DMA_CHANNEL* pChannel, uint32_t nConfig);

/* Buffer Functions */
static DMA_BUFFER* fpop_buffer (DMA_BUFFER** ppBList);
static void bpush_buffer (DMA_BUFFER** ppBList, DMA_BUFFER* pBuffer);

/* Issue Transfer Request */
static void IssueTransferRequest(DMA_CHANNEL* pChannel);

/* Fill the descriptors */
static void FillDescriptor (
                            DMA_CHANNEL* pChannel,
                            DMA_BUFFER* pBuffer
                            );


/* DMA Interrupt Handler */
static void DMAInterruptHandler(DMA_CHANNEL* pChannel);

#ifdef ADI_DEBUG
static ADI_DMA_RESULT ValidateHandle (DMA_CHANNEL* pChannel);
#endif /* ADI_DEBUG */

/*==========  DMA HANDLERS  ==========*/

/* DMA Interrupt Handlers */
ADI_INT_HANDLER (DMA_SPI2_TX_Int_Handler);
ADI_INT_HANDLER (DMA_SPI2_RX_Int_Handler);
ADI_INT_HANDLER (DMA_SPORT0A_Int_Handler);
ADI_INT_HANDLER (DMA_SPORT0B_Int_Handler);
ADI_INT_HANDLER (DMA_SPI0_TX_Int_Handler);
ADI_INT_HANDLER (DMA_SPI0_RX_Int_Handler);
ADI_INT_HANDLER (DMA_SPI1_TX_Int_Handler);
ADI_INT_HANDLER (DMA_SPI1_RX_Int_Handler);
ADI_INT_HANDLER (DMA_UART_TX_Int_Handler);
ADI_INT_HANDLER (DMA_UART_RX_Int_Handler);
ADI_INT_HANDLER (DMA_I2C0_STX_Int_Handler);
ADI_INT_HANDLER (DMA_I2C0_SRX_Int_Handler);
ADI_INT_HANDLER (DMA_I2C0_MX_Int_Handler);
ADI_INT_HANDLER (DMA_AES0_IN_Int_Handler);
ADI_INT_HANDLER (DMA_AES0_OUT_Int_Handler);
ADI_INT_HANDLER (DMA_FLASH0_Int_Handler);
ADI_INT_HANDLER (DMA_ADC0_Int_Handler);

ADI_INT_HANDLER (DMA_SIP0_Int_Handler);
ADI_INT_HANDLER (DMA_SIP1_Int_Handler);
ADI_INT_HANDLER (DMA_SIP2_Int_Handler);
ADI_INT_HANDLER (DMA_SIP3_Int_Handler);
ADI_INT_HANDLER (DMA_SIP4_Int_Handler);
ADI_INT_HANDLER (DMA_SIP5_Int_Handler);
ADI_INT_HANDLER (DMA_SIP6_Int_Handler);
ADI_INT_HANDLER (DMA_SIP7_Int_Handler);



/* DMA Error Handler */
ADI_INT_HANDLER (DMA_Err_Int_Handler);

/*==========  U T I L I T Y    M A C R O S  ==========*/

#define SET_BITS(X,BITS)  ((X) |= (BITS))
#define CLR_BITS(X,BITS)  ((X) &= (~(BITS)))

#define IS_BITS_SET(X,BITS)  (((X) & (BITS)) == (BITS))

#define DMA_CHN_IRQ(X)   ((uint32_t)((((uint32_t)(X)) & 0xFFFF0000u) >> 16u))
#define DMA_CHN_NUM(X)   ((uint32_t)(((uint32_t)(X)) & 0x0000FFFFu))
/*! \endcond */
/*=============  A P I    I M P L I M E N T A T I O N S  =============*/

/**
 * @brief    Opens a DMA channel for use.
 *
 * @param [in]  eChannelID    The ID of the DMA channel to be opened.
 * @param [in]  pChannelMem   Pointer to the a memory buffer to be used be driver for the
 *                            given channel. The buffer should be 32-bit aligned and should
 *                            of size ADI_DMA_MEMORY_SIZE bytes.
 * @param [out] phChannel     Pointer to the handle variable to which the channel handle will
 *                            be written.
 * @param [in]  pfCallback    Pointer to the application callback function.
 * @param [in]  pCBParam      Application callback parameter.
 *
 * @details  The function opens the given DMA channel for use. A memory buffer of size ADI_DMA_MEMORY_SIZE
 *           bytes should be passed to the application. The application should make sure the size is correct
 *           and it is 32-bit aligned.
 *
 * @return   Status
 *              - #ADI_DMA_SUCCESS                       Successfully opened the DMA for the given channel.
 *              - #ADI_DMA_ERR_INVALID_PARAMETER     [D] Some parameter(s) passed to the function is invalid.
 *              - #ADI_DMA_ERR_ALREADY_INITIALIZED   [D] The given DMA channel is already opened.
 */
ADI_DMA_RESULT adi_dma_Open (
                             ADI_DMA_CHANNEL_ID          const eChannelID,
                             void*                       const pChannelMem,
                             ADI_DMA_CHANNEL_HANDLE*     const phChannel,
                             ADI_CALLBACK                const pfCallback,
                             void*                       const pCBParam
                             )
{
    uint32_t nChannelBit = (uint32_t)1ul << DMA_CHN_NUM(eChannelID);
    DMA_CHANNEL* pChannel = (DMA_CHANNEL*)pChannelMem;

#ifdef ADI_DEBUG
    if ((pfCallback == NULL) || (pChannelMem == NULL))
    {
        return ADI_DMA_ERR_INVALID_PARAMETER;
    }

    if (pDMA_DevData->ChnHandles[DMA_CHN_NUM(eChannelID)] != NULL)
    {
        return ADI_DMA_ERR_ALREADY_INITIALIZED;
    }

    assert (ADI_DMA_MEMORY_SIZE >= sizeof(DMA_CHANNEL));
#endif


    /* Enable the DMA Controller */
    SET_BITS(pADI_DMA0->CFG, BITM_DMA_CFG_MEN);

    /* One-time controller initializations */
    if (pDMA_DevData->InitState == DMA_STATE_UNKNOWN)
    {
        /* Initialize RAM-based descriptor array */
        memset(gChannelControlDataArray, 0, sizeof(gChannelControlDataArray));

        /* Set descriptor memory base pointer on DMA controller */
        pADI_DMA0->PDBPTR = (uint32_t)pPrimaryCCD;

        /* Reset per-channel, bitmapped control registers */
        pADI_DMA0->RMSK_SET        = 0x0000FFFFu;
        pADI_DMA0->EN_CLR          = 0x0000FFFFu;
        pADI_DMA0->ALT_CLR         = 0x0000FFFFu;
        pADI_DMA0->PRI_CLR         = 0x0000FFFFu;
        pADI_DMA0->ERRCHNL_CLR     = 0x0000FFFFu;
        pADI_DMA0->ERR_CLR         = 0x0000FFFFu;
        pADI_DMA0->INVALIDDESC_CLR = 0x0000FFFFu;

        /* Mark the controller as initialized */
        pDMA_DevData->InitState = DMA_STATE_INITIALIZED;

         /* Install the interrupt Handler */
        ADI_INSTALL_HANDLER(DMA_CHAN_ERR_IRQn, DMA_Err_Int_Handler);

        ADI_INSTALL_HANDLER(DMA0_CH0_DONE_IRQn, DMA_SPI2_TX_Int_Handler);
        ADI_INSTALL_HANDLER(DMA0_CH1_DONE_IRQn, DMA_SPI2_RX_Int_Handler);
        ADI_INSTALL_HANDLER(DMA0_CH2_DONE_IRQn, DMA_SPORT0A_Int_Handler);
        ADI_INSTALL_HANDLER(DMA0_CH3_DONE_IRQn, DMA_SPORT0B_Int_Handler);
        ADI_INSTALL_HANDLER(DMA0_CH4_DONE_IRQn, DMA_SPI0_TX_Int_Handler);
        ADI_INSTALL_HANDLER(DMA0_CH5_DONE_IRQn, DMA_SPI0_RX_Int_Handler);
        ADI_INSTALL_HANDLER(DMA0_CH6_DONE_IRQn, DMA_SPI1_TX_Int_Handler);
        ADI_INSTALL_HANDLER(DMA0_CH7_DONE_IRQn, DMA_SPI1_RX_Int_Handler);
        ADI_INSTALL_HANDLER(DMA0_CH8_DONE_IRQn, DMA_UART_TX_Int_Handler);
        ADI_INSTALL_HANDLER(DMA0_CH9_DONE_IRQn, DMA_UART_RX_Int_Handler);
        ADI_INSTALL_HANDLER(DMA0_CH10_DONE_IRQn, DMA_I2C0_STX_Int_Handler);
        ADI_INSTALL_HANDLER(DMA0_CH11_DONE_IRQn, DMA_I2C0_SRX_Int_Handler);
        ADI_INSTALL_HANDLER(DMA0_CH12_DONE_IRQn, DMA_I2C0_MX_Int_Handler);
        ADI_INSTALL_HANDLER(DMA0_CH13_DONE_IRQn, DMA_AES0_IN_Int_Handler);
        ADI_INSTALL_HANDLER(DMA0_CH14_DONE_IRQn, DMA_AES0_OUT_Int_Handler);
        ADI_INSTALL_HANDLER(DMA0_CH15_DONE_IRQn, DMA_FLASH0_Int_Handler);

        ADI_INSTALL_HANDLER(DMA0_CH16_DONE_IRQn, DMA_SIP0_Int_Handler);
        ADI_INSTALL_HANDLER(DMA0_CH17_DONE_IRQn, DMA_SIP1_Int_Handler);
        ADI_INSTALL_HANDLER(DMA0_CH18_DONE_IRQn, DMA_SIP2_Int_Handler);
        ADI_INSTALL_HANDLER(DMA0_CH19_DONE_IRQn, DMA_SIP3_Int_Handler);
        ADI_INSTALL_HANDLER(DMA0_CH20_DONE_IRQn, DMA_SIP4_Int_Handler);
        ADI_INSTALL_HANDLER(DMA0_CH21_DONE_IRQn, DMA_SIP5_Int_Handler);
        ADI_INSTALL_HANDLER(DMA0_CH22_DONE_IRQn, DMA_SIP6_Int_Handler);
        ADI_INSTALL_HANDLER(DMA0_CH23_DONE_IRQn, DMA_SIP7_Int_Handler);


        /* Enable the DMA Error Interrupt */
        ADI_ENABLE_INT(DMA_CHAN_ERR_IRQn);
    }

    /* Clear the channel strucutre */
    memset(pChannel, 0, sizeof(DMA_CHANNEL));

    /* Set the channel ID */
    pChannel->ChannelID = (uint32_t)eChannelID;

    /* Initialize the Channel */
    Initialize(pChannel);

    /* Set the callback parameters */
    pChannel->pfCallback = pfCallback;
    pChannel->pCBParam   = pCBParam;

    /* Make the channel to be in use */
    pDMA_DevData->ChannelsInUse |= nChannelBit;

    /* Set the handle in the driver */
    pDMA_DevData->ChnHandles[DMA_CHN_NUM(pChannel->ChannelID)] = pChannel;

    /* Return the handle back to the application */
    *phChannel = pChannel;

    return ADI_DMA_SUCCESS;
}

/**
 * @brief    Closes the given DMA Handle
 *
 * @param [in]  hChannel   DMA Channel Handle.
 *
 * @details  The function closes the given DMA Channel Handle. If the DMA channel is enabled, the function
 *           disables the DMA before closing the DMA channel.
 *
 * @return   Status
 *              - #ADI_DMA_SUCCESS                       Successfully closed the DMA channel.
 *              - #ADI_DMA_ERR_INVALID_HANDLE        [D] Handle passed is invalid.
 */
ADI_DMA_RESULT adi_dma_Close (ADI_DMA_CHANNEL_HANDLE const hChannel)
{

    DMA_CHANNEL* pChannel = (DMA_CHANNEL*)hChannel;
    ADI_DMA_RESULT eResult = ADI_DMA_SUCCESS;

#ifdef ADI_DEBUG
    /* Validate the device handle */
    eResult = ValidateHandle(pChannel);
    if (eResult != ADI_DMA_SUCCESS)
    {
        return eResult;
    }
#endif

    uint32_t nChannelBit = 1ul << DMA_CHN_NUM(pChannel->ChannelID);

    /* IF (Channel is enabled) */
    if ((pDMA_DevData->ChannelsEnabled & nChannelBit) != 0u)
    {
        /* Disable the DMA */
        adi_dma_Enable(pChannel, false);
    }

    /* Clear the handle in the driver */
    pDMA_DevData->ChnHandles[DMA_CHN_NUM(pChannel->ChannelID)] = NULL;

    /* Mark the channel as not being used */
    CLR_BITS(pDMA_DevData->ChannelsInUse,nChannelBit);

    /* IF (All DMA channels are closed) */
    if (pDMA_DevData->ChannelsEnabled == 0u)
    {
        /* Disable the DMA Controller */
        CLR_BITS(pADI_DMA0->CFG, BITM_DMA_CFG_MEN);
    }

    return eResult;
}

/**
 * @brief   Submit a transfer to the DMA channel
 *
 * @param [in]  hChannel    DMA Channel Handle
 * @param [in]  pTransfer   Pointer to the transfer structure that describes the transfer.
 *
 * @details  The function accepts a transfer request from the application. The DMA will only
 *           start transfering if it is enable. If not enabled, the DMA driver will accept
 *           a transfer request and process it when it is enabled. The DMA driver can at most
 *           hold two request under process.
 *
 * @return   Status
 *              - #ADI_DMA_SUCCESS                       Successfully submitted the transfer.
 *              - #ADI_DMA_ERR_NO_FREE_BUFFERS           Maximum limit of 2 pending buffers has been
 *                                                       exceeded.
 *              - #ADI_DMA_ERR_INVALID_HANDLE        [D] Handle passed is invalid.
 *              - #ADI_DMA_ERR_INVALID_PARAMETER     [D] pTransfer is NULL
 *              - #ADI_DMA_ERR_INVALID_BUFFER        [D] Transfer structure has invalid parameters.
 */
ADI_DMA_RESULT adi_dma_SubmitTransfer (
                                       ADI_DMA_CHANNEL_HANDLE const hChannel,
                                       ADI_DMA_TRANSFER*      const pTransfer
                                       )
{
    DMA_BUFFER* pBuffer;
    DMA_CHANNEL* pChannel = (DMA_CHANNEL*)hChannel;
    ADI_DMA_RESULT eResult = ADI_DMA_SUCCESS;

#ifdef ADI_DEBUG
    /* Validate the device handle */
    eResult = ValidateHandle(pChannel);
    if (eResult != ADI_DMA_SUCCESS)
    {
        return eResult;
    }

    /* Validate the function input parameteres */
    if (pTransfer == NULL)
    {
       return  ADI_DMA_ERR_INVALID_PARAMETER;
    }

    /* Validate the buffer structure */
    if (   (pTransfer->pSrcData == NULL)
        || (pTransfer->pDstData == NULL)
       )
    {
        return ADI_DMA_ERR_INVALID_BUFFER;
    }

    /* Validate the width,increments and alignement */
    if (
            (   (pTransfer->DataWidth == ADI_DMA_WIDTH_2_BYTE)
             && (   (pTransfer->SrcInc == ADI_DMA_INCR_1_BYTE)
                 || (pTransfer->DstInc == ADI_DMA_INCR_1_BYTE)
                 || ((((uint32_t)pTransfer->pSrcData) & (0x1u)) != 0u)))

        ||  (   (pTransfer->DataWidth == ADI_DMA_WIDTH_4_BYTE)
             && (   (pTransfer->SrcInc == ADI_DMA_INCR_1_BYTE)
                 || (pTransfer->SrcInc == ADI_DMA_INCR_2_BYTE)
                 || (pTransfer->DstInc == ADI_DMA_INCR_1_BYTE)
                 || (pTransfer->DstInc == ADI_DMA_INCR_2_BYTE)
                 || ((((uint32_t)pTransfer->pSrcData) & (0x3u)) != 0u))))
    {
        return ADI_DMA_ERR_INVALID_BUFFER;
    }

    /* TODO: Validate for mode errors.Smave for scatter mode submit */

    /* IF (The mode of operation is ping-pong) */
    if ((pChannel->nCommonCfg & DMA_BITM_CTL_CYCLE_CTL) == DMA_ENUM_CTL_CYCLE_CTL_PING_PONG) {
        if (
                /* Buffer src is decrement but the previous buffers source was increment */
                (((pTransfer->SrcInc & DMA_BITM_INCR_TYPE_DECR) != 0u) && 
                 ((pChannel->nOtherCmnCfg & DMA_BITM_OCTL_SRC_INCR) != 0u))

                /* Buffer src is increment but the previous buffers source was decrement */
            ||  (((pTransfer->SrcInc & DMA_BITM_INCR_TYPE_DECR) == 0u) &&
                 ((pChannel->nOtherCmnCfg &  DMA_BITM_OCTL_SRC_DECR) != 0u))

                /* Buffer dst is decrement but the previous buffers dst was increment */
            ||  (((pTransfer->DstInc & DMA_BITM_INCR_TYPE_DECR) != 0u) &&
                 ((pChannel->nOtherCmnCfg &  DMA_BITM_OCTL_DST_INCR) != 0u))

                /* Buffer dst is increment but the previous buffers dst was decrement */
            ||  (((pTransfer->DstInc & DMA_BITM_INCR_TYPE_DECR) == 0u) &&
                 ((pChannel->nOtherCmnCfg &  DMA_BITM_OCTL_DST_DECR) != 0u)))
        {
            return ADI_DMA_ERR_INVALID_BUFFER;
        }
    }
#endif

    uint32_t nChannelBit = 1ul << DMA_CHN_NUM(pChannel->ChannelID);

    /* Get a free buffer */
    pBuffer = fpop_buffer(&pChannel->pFreeBList);

    if (pBuffer) {
        /* Fill in the structures */
        pBuffer->pSrcData = pTransfer->pSrcData;
        pBuffer->pDstData = pTransfer->pDstData;
        pBuffer->nCurPos = 0u;
        pBuffer->nRemTransfers = pTransfer->NumTransfers;
        pBuffer->nRemDoneTransfers = pTransfer->NumTransfers;

        /* Fill the configuration values */
        pBuffer->nCfgValues =
          /* Configure the destination increment */
            (
             (((uint32_t)pTransfer->DstInc) << DMA_BITP_CTL_DST_INC)
                & DMA_BITM_CTL_DST_INC)
          /* Configure the source increment */
          | (
             (((uint32_t)pTransfer->SrcInc) << DMA_BITP_CTL_SRC_INC)
                & DMA_BITM_CTL_SRC_INC)
          /* Configure the data width */
          | (((uint32_t)pTransfer->DataWidth) << DMA_BITP_CTL_SRC_SIZE);

        /* Set the other configuration values */
        pBuffer->nOtherCfgValues = 0u;

        /* Set the increment/decrement bits */
        pBuffer->nOtherCfgValues |=
            ((((uint32_t)pTransfer->SrcInc) & DMA_BITM_INCR_TYPE_DECR) != 0u)
            ? (uint32_t)DMA_BITM_OCTL_SRC_DECR : (uint32_t)DMA_BITM_OCTL_SRC_INCR;

        pBuffer->nOtherCfgValues |=
            ((((uint32_t)pTransfer->DstInc) & DMA_BITM_INCR_TYPE_DECR) != 0u)
            ? (uint32_t)DMA_BITM_OCTL_DST_DECR : (uint32_t)DMA_BITM_OCTL_DST_INCR;

        /* Set the channel common other cfg value */
        pChannel->nOtherCmnCfg |= pBuffer->nOtherCfgValues;

        /* Put the buffer in the pending queue */
        bpush_buffer(&pChannel->pPendBList, pBuffer);

        if (((pDMA_DevData->ChannelsEnabled & nChannelBit) != 0u) &&
            (pChannel->pActiveBList == NULL))
        {
            /* Initiate a new active buffer */
            InitNewActiveBuffer(pChannel);

            /* Issue a transfer request */
            IssueTransferRequest(pChannel);
        }
    } else {
        eResult = ADI_DMA_ERR_NO_FREE_BUFFERS;
    }

    return eResult;
}

/**
 * @brief   Submit a scatter transfer for the given DMA channel.
 *
 * @param [in]  hChannel   DMA Channel Handle
 * @param [in]  DescList   Pointer to the descriptor list.
 * @param [in]  nNumDesc   Number of descriptors in the list.
 *
 * @details  The function accepts a transfer request from the application. The DMA will only
 *           start transfering if it is enable. If not enabled, the DMA driver will accept
 *           a transfer request and process it when it is enabled. The DMA driver can at most
 *           hold two request under process.
 *
 * @return   Status
 *              - #ADI_DMA_SUCCESS                       Successfully submitted the transfer.
 *              - #ADI_DMA_ERR_NO_FREE_BUFFERS           Maximum limit of 2 pending buffers has been
 *                                                       exceeded.
 *              - #ADI_DMA_ERR_INVALID_HANDLE        [D] Handle passed is invalid.
 *              - #ADI_DMA_ERR_INVALID_PARAMETER     [D] pTransfer is NULL
 *              - #ADI_DMA_ERR_INVALID_BUFFER        [D] Transfer structure has invalid parameters.
 *              - #ADI_DMA_ERR_INVALID_DESC_LIST     [D] DMA Descriptor Submitted is Invalid
 */
ADI_DMA_RESULT adi_dma_SubmitScatterTransfer (
                                             ADI_DMA_CHANNEL_HANDLE  const hChannel,
                                             ADI_DCC_TypeDef*        const DescList,
                                             uint32_t                const nNumDesc
                                             )
{
    DMA_BUFFER* pBuffer;
    DMA_CHANNEL* pChannel = (DMA_CHANNEL*)hChannel;
    ADI_DMA_RESULT eResult = ADI_DMA_SUCCESS;

#ifdef ADI_DEBUG
    /* Validate the device handle */
    eResult = ValidateHandle(pChannel);
    if (eResult != ADI_DMA_SUCCESS)
    {
        return eResult;
    }

    /* Validate the function input parameteres */
    if ((DescList == NULL) || (nNumDesc == 0u) || (nNumDesc > 256u))
    {
       return  ADI_DMA_ERR_INVALID_PARAMETER;
    }
#endif

    uint32_t nChannelBit = 1ul << DMA_CHN_NUM(pChannel->ChannelID);

#ifdef ADI_DEBUG
    /* Validate the descriptor list */
    uint32_t nDescMode, nEndMode;
    /* in C89 we need to have the for loop counter declared outside the loop */
    uint32_t x;

    if ((pDMA_DevData->SoftwareChns & nChannelBit) != 0u)
    {
        nDescMode = DMA_ENUM_CTL_CYCLE_CTL_MSG_ALT;
        nEndMode = DMA_ENUM_CTL_CYCLE_CTL_AUTO_REQ;
    }
    else
    {
        nDescMode = DMA_ENUM_CTL_CYCLE_CTL_PSG_ALT;
        nEndMode = DMA_ENUM_CTL_CYCLE_CTL_BASIC;
    }
    for (x = 0u; x < (nNumDesc-1u); x++)
    {
        if ((DescList[x].DMACDC & DMA_BITM_CTL_CYCLE_CTL) != nDescMode)
        {
            return ADI_DMA_ERR_INVALID_DESC_LIST;
        }
    }

    if ((DescList[nNumDesc-1u].DMACDC & DMA_BITM_CTL_CYCLE_CTL) != nEndMode)
    {
        return ADI_DMA_ERR_INVALID_DESC_LIST;
    }
#endif


    /* Get a free buffer */
    pBuffer = fpop_buffer(&pChannel->pFreeBList);

    if (pBuffer) {

        /* Fill in the structures */
        pBuffer->pSrcData = DescList;
        pBuffer->pDstData = NULL;
        pBuffer->nCurPos = 0u;
        pBuffer->nRemTransfers = 4u * nNumDesc;
        pBuffer->nRemDoneTransfers = pBuffer->nRemTransfers;

        /* Fill the configuration values */
        pBuffer->nCfgValues =  (0x2u << DMA_BITP_CTL_DST_INC)
                             | (0x2u << DMA_BITP_CTL_SRC_INC)
                             | (0x2u << DMA_BITP_CTL_SRC_SIZE)
                             | (0x2u << DMA_BITP_CTL_R_POWER);

        /* Put the buffer in the pending queue */
        bpush_buffer(&pChannel->pPendBList, pBuffer);

        if (((pDMA_DevData->ChannelsEnabled & nChannelBit) != 0u) &&
            (pChannel->pActiveBList == NULL))
        {
            /* Initiate a new active buffer */
            InitNewActiveBuffer(pChannel);

            /* Issue a transfer request */
            IssueTransferRequest(pChannel);
        }
    } else {
        eResult = ADI_DMA_ERR_NO_FREE_BUFFERS;
    }

    return eResult;
}

/**
 * @brief    Enable/Disable the DMA Channel
 *
 * @param [in]  hChannel   DMA Channel Handle
 * @param [in]  bEnable    'true' to Enable and 'false' to Disable the channel.
 *
 * @return  Status
 *              - #ADI_DMA_SUCCESS                       Successfully enable/disabled the DMA Channel.
 *              - #ADI_DMA_ERR_INVALID_HANDLE        [D] Handle passed is invalid.
 *
 */
ADI_DMA_RESULT adi_dma_Enable (
    ADI_DMA_CHANNEL_HANDLE const hChannel,
    bool_t                 const bEnable
    )
{
    DMA_CHANNEL* pChannel = (DMA_CHANNEL*)hChannel;
    ADI_DMA_RESULT eResult = ADI_DMA_SUCCESS;

#ifdef ADI_DEBUG
    /* Validate the device handle */
    eResult = ValidateHandle(pChannel);
    if (eResult != ADI_DMA_SUCCESS)
    {
        return eResult;
    }
#endif

    uint32_t nChannelBit = 1ul << DMA_CHN_NUM(pChannel->ChannelID);

    if (bEnable) {
        /* IF (The device is not already enabled) */
        if ((pDMA_DevData->ChannelsEnabled & nChannelBit) == 0u)
        {
            /* Enable the Channel */
            pDMA_DevData->ChannelsEnabled |= nChannelBit;

            /* Enable the DMA Controller */
            SET_BITS(pADI_DMA0->CFG, BITM_DMA_CFG_MEN);

            /* Setup the device */
            SetupDevice(pChannel);

            /* Enable the interrupt for the given DMA */
            ADI_ENABLE_INT((IRQn_Type)DMA_CHN_IRQ(pChannel->ChannelID));

            /* Initiate a new active buffer */
            InitNewActiveBuffer(pChannel);

            /* Issue a transfer request */
            IssueTransferRequest(pChannel);
        }
    } else {
        DMA_BUFFER* pBuffer;

        /* Disable the DMA */
        pADI_DMA0->EN_CLR = nChannelBit;

        /* Set the peripheral mask */
        pADI_DMA0->RMSK_SET = nChannelBit;

        /* Clear all the pending and active buffers */
        while (pChannel->pPendBList != NULL)
        {
            pBuffer = fpop_buffer(&pChannel->pPendBList);
            bpush_buffer(&pChannel->pFreeBList, pBuffer);
        }
        while (pChannel->pActiveBList != NULL)
        {
            pBuffer = fpop_buffer(&pChannel->pActiveBList);
            bpush_buffer(&pChannel->pFreeBList, pBuffer);
        }

        /* Clear the global increment/decrement flags of the channel */
        CLR_BITS(
                 pChannel->nOtherCmnCfg,
                 (
                    (uint32_t)DMA_BITM_OCTL_SRC_DECR
                  | (uint32_t)DMA_BITM_OCTL_DST_DECR
                  | (uint32_t)DMA_BITM_OCTL_SRC_INCR
                  | (uint32_t)DMA_BITM_OCTL_DST_INCR
                  ));


        /* Clear the Channel bit */
        pDMA_DevData->ChannelsEnabled &= ~(nChannelBit);

    }

    return eResult;
}

/**
 * @brief   Set the mode of operation for the DMA Channel
 *
 * @param [in]  hChannel   DMA Channel Handle
 * @param [in]  eMode      Enumeration for the mode
 *
 * @return  Status
 *              - #ADI_DMA_SUCCESS                       Successfully set the mode.
 *              - #ADI_DMA_ERR_INVALID_HANDLE        [D] Handle passed is invalid.
 *              = #ADI_DMA_ERR_INVALID_STATE         [D] The function cannot be called after submitting
 *                                                       any buffers or after being enabled.
 */
ADI_DMA_RESULT adi_dma_SetMode (
                                ADI_DMA_CHANNEL_HANDLE const hChannel,
                                ADI_DMA_MODE           const eMode
                                )
{

    DMA_CHANNEL* pChannel = (DMA_CHANNEL*)hChannel;
    ADI_DMA_RESULT eResult = ADI_DMA_SUCCESS;

 #ifdef ADI_DEBUG
    /* Validate the device handle */
    eResult = ValidateHandle(pChannel);
    if (eResult != ADI_DMA_SUCCESS)
    {
        return eResult;
    }
#endif

#ifdef ADI_DEBUG
    uint32_t nChannelBit = 1ul << DMA_CHN_NUM(pChannel->ChannelID);

    if (
            IS_BITS_SET(pDMA_DevData->ChannelsEnabled, nChannelBit)
         || (pChannel->pPendBList != NULL)
        )
    {
        return ADI_DMA_ERR_INVALID_STATE;
    }
#endif

    /* Clear the cycle ctl bits */
    CLR_BITS(pChannel->nCommonCfg, DMA_BITM_CTL_CYCLE_CTL);

    /* Set the mdoe */
    switch (eMode)
    {
    case ADI_DMA_MODE_BASIC:
        SET_BITS(pChannel->nCommonCfg, DMA_ENUM_CTL_CYCLE_CTL_BASIC);
        break;

    case ADI_DMA_MODE_AUTO:
        SET_BITS(pChannel->nCommonCfg, DMA_ENUM_CTL_CYCLE_CTL_AUTO_REQ);
        break;

    case ADI_DMA_MODE_PING_PONG:
        SET_BITS(pChannel->nCommonCfg, DMA_ENUM_CTL_CYCLE_CTL_PING_PONG);
        break;

    case ADI_DMA_MODE_MSG:
        SET_BITS(pChannel->nCommonCfg, DMA_ENUM_CTL_CYCLE_CTL_MSG_PRI);
        break;

    case ADI_DMA_MODE_PSG:
        SET_BITS(pChannel->nCommonCfg, DMA_ENUM_CTL_CYCLE_CTL_PSG_PRI);
        break;

    default:
        eResult = ADI_DMA_ERR_INVALID_PARAMETER;
        break;
    }

    return eResult;
}

/**
 * @brief    Set the arbitration rate for the given DMA Channel.
 *
 * @param [in]  hChannel     DMA Handle
 * @param [in]  eArbitRate   Enumeration for the arbitration rate.
 *
 * @return  Status
 *              - #ADI_DMA_SUCCESS                       Successfully set the arbitration rate.
 *              - #ADI_DMA_ERR_INVALID_HANDLE        [D] Handle passed is invalid.
 *              - #ADI_DMA_ERR_INVALID_STATE         [D] Arbitration rate cannot be changed once the
 *                                                       the DMA channel is enabled.
 */
ADI_DMA_RESULT adi_dma_SetArbitrationRate (
    ADI_DMA_CHANNEL_HANDLE const hChannel,
    ADI_DMA_RPOWER         const eArbitRate
    )
{
    DMA_CHANNEL* pChannel = (DMA_CHANNEL*)hChannel;
    ADI_DMA_RESULT eResult = ADI_DMA_SUCCESS;

 #ifdef ADI_DEBUG
    /* Validate the device handle */
    eResult = ValidateHandle(pChannel);
    if (eResult != ADI_DMA_SUCCESS)
    {
        return eResult;
    }
#endif

#ifdef ADI_DEBUG
    uint32_t nChannelBit = 1ul << DMA_CHN_NUM(pChannel->ChannelID);

    if (IS_BITS_SET(pDMA_DevData->ChannelsEnabled, nChannelBit))
    {
        return ADI_DMA_ERR_INVALID_STATE;
    }
#endif

    /* Clear the arbitration register */
    CLR_BITS(pChannel->nCommonCfg, DMA_BITM_CTL_R_POWER);

    /* Set the arbitration valie */
    pChannel->nCommonCfg |= ((uint32_t)eArbitRate << DMA_BITP_CTL_R_POWER);

    /* Return the result */
    return eResult;
}

/**
 * @brief    Set the priority of the given channel
 *
 * @param [in]  hChannel    DMA Channel Handle
 * @param [in]  ePriority   Priority value for the DMA Handle.
 *
 * @return  Status
 *              - #ADI_DMA_SUCCESS                       Successfully set the priority for the given DMA channel.
 *              - #ADI_DMA_ERR_INVALID_HANDLE        [D] Handle passed is invalid.
 */
ADI_DMA_RESULT adi_dma_SetPriority (
    ADI_DMA_CHANNEL_HANDLE const hChannel,
    ADI_DMA_PRIORITY       const ePriority
    )
{
    DMA_CHANNEL* pChannel = (DMA_CHANNEL*)hChannel;
    ADI_DMA_RESULT eResult = ADI_DMA_SUCCESS;

 #ifdef ADI_DEBUG
    /* Validate the device handle */
    eResult = ValidateHandle(pChannel);
    if (eResult != ADI_DMA_SUCCESS)
    {
        return eResult;
    }
#endif
    uint32_t nChannelBit = 1ul << DMA_CHN_NUM(pChannel->ChannelID);

    /* Set/Clear the priority based on the parameter */
    if (ePriority == ADI_DMA_PRIORITY_HIGH)
    {
        pADI_DMA0->PRI_SET = nChannelBit;
    }
    else
    {
        pADI_DMA0->PRI_CLR = nChannelBit;
    }

    /* Return the result */
    return eResult;
}

/**
 * @brief   Enable/Disable byte swap for the given DMA channel.
 *
 * @param [in]  hChannel    DMA Channel Handle
 * @param [in]  bByteSwap   'true' to Enable and 'false' to disable byte swap for the
 *                          given DMA Channel.
 *
 * @return  Status
 *              - #ADI_DMA_SUCCESS                       Successfully set the byte swap
 *              - #ADI_DMA_ERR_INVALID_HANDLE        [D] Handle passed is invalid.
 *              - #ADI_DMA_ERR_INVALID_STATE         [D] Byte swap cannot be set when the DMA channel is enabled.
 */
ADI_DMA_RESULT adi_dma_SetByteSwap (
    ADI_DMA_CHANNEL_HANDLE const hChannel,
    bool_t                 const bByteSwap
    )
{
    DMA_CHANNEL* pChannel = (DMA_CHANNEL*)hChannel;
    ADI_DMA_RESULT eResult = ADI_DMA_SUCCESS;

 #ifdef ADI_DEBUG
    /* Validate the device handle */
    eResult = ValidateHandle(pChannel);
    if (eResult != ADI_DMA_SUCCESS)
    {
        return eResult;
    }
#endif

    uint32_t nChannelBit = 1ul << DMA_CHN_NUM(pChannel->ChannelID);


#ifdef ADI_DEBUG
    if (IS_BITS_SET(pDMA_DevData->ChannelsEnabled, nChannelBit))
    {
        return ADI_DMA_ERR_INVALID_STATE;
    }
#endif

    if (bByteSwap)
    {
        /* Set byte swap */
        pADI_DMA0->BS_SET = nChannelBit;
    }
    else
    {
        /* Clear the byte swap */
        pADI_DMA0->BS_CLR = nChannelBit;
    }

    /* Return the result */
    return eResult;
}

/* Return whether transfer a is in progress */
/**
 * @brief    Return the status of the progress of the current transfer
 *
 * @param [in]  hChannel               DMA Channel Handle
 * @param [out] pbTransferInProgress   Pointer to a bool_t variable to which the progress status
 *                                     is written.
 * @return
 *              - #ADI_DMA_SUCCESS                       Successfully returned the status.
 *              - #ADI_DMA_ERR_INVALID_HANDLE        [D] Handle passed is invalid.
 *              - #ADI_DMA_ERR_INVALID_PARAMETER     [D] Pointer is NULL.
 */
ADI_DMA_RESULT adi_dma_IsTransferInProgress (
    ADI_DMA_CHANNEL_HANDLE const hChannel,
    bool_t*                const pbTransferInProgress
    )
{
    DMA_CHANNEL* pChannel = (DMA_CHANNEL*)hChannel;
    ADI_DMA_RESULT eResult = ADI_DMA_SUCCESS;

 #ifdef ADI_DEBUG
    /* Validate the device handle */
    eResult = ValidateHandle(pChannel);
    if (eResult != ADI_DMA_SUCCESS)
    {
        return eResult;
    }

    if (pbTransferInProgress == NULL)
    {
        return ADI_DMA_ERR_INVALID_PARAMETER;
    }
#endif

    uint32_t nChannelBit = 1ul << DMA_CHN_NUM(pChannel->ChannelID);

    /* Set flag to true if channel enabled and transfer is active or pending. */
    if (IS_BITS_SET(pDMA_DevData->ChannelsEnabled, nChannelBit) &&
        ((pChannel->pActiveBList != NULL) || (pChannel->pPendBList != NULL)))
    {
        *pbTransferInProgress = true;
    }
    else
    {
        *pbTransferInProgress = false;
    }

    /* Return the result */
    return eResult;
}




/*=============  L O C A L    F U N C T I O N S  =============*/

static void Initialize(DMA_CHANNEL* pChannel)
{
    uint32_t nChannelBit = 1ul << DMA_CHN_NUM(pChannel->ChannelID);
    /* in C89 we need to have the for loop counter declared outside the loop */
    int32_t x;

    /* List the buffers */
    for (x = 0; x < (NUM_BUFFERS-1); x++)
    {
        pChannel->Buffers[x].pNext = &(pChannel->Buffers[x+1]);
    }
    pChannel->Buffers[NUM_BUFFERS-1].pNext = NULL;

    /* Add list of buffers to the free list */
    pChannel->pFreeBList = &pChannel->Buffers[0];

    /* Clear the byte swap and priority to valid state */
    pADI_DMA0->BS_CLR = nChannelBit;
    pADI_DMA0->PRI_CLR = nChannelBit;


    /* IF (The DMA is a software DMA) */
    if ((pDMA_DevData->SoftwareChns & nChannelBit) != 0u) {
        pChannel->nCommonCfg = DMA_ENUM_CTL_CYCLE_CTL_AUTO_REQ;
    } else {
        pChannel->nCommonCfg = DMA_ENUM_CTL_CYCLE_CTL_BASIC;
    }
}

/* Fill the descriptors */
static void FillDescriptor (
                            DMA_CHANNEL* pChannel,
                            DMA_BUFFER* pBuffer
                            )
{
    ADI_DCC_TypeDef* pCCD;
    uint32_t nConfig;
    uint32_t nNumTransfers;
    uint32_t nSrcModify;
    uint32_t nDstModify;
    uint32_t nChannelNum = DMA_CHN_NUM(pChannel->ChannelID);

    while ((pChannel->nNumFreeDesc > 0u) && (pBuffer->nRemTransfers > 0u)){
        nConfig =  pChannel->nCommonCfg | pBuffer->nCfgValues;
        if (pChannel->bIsPrimNext) {
            pCCD = &pPrimaryCCD[nChannelNum];
            pChannel->bIsPrimNext = false;
        } else {
            pCCD = &pAlternateCCD[nChannelNum];
            pChannel->bIsPrimNext = true;
        }



        pChannel->nNumFreeDesc--;


        if (pBuffer->nRemTransfers > DMA_MAX_TRANSFER_SIZE) {
            nNumTransfers = DMA_MAX_TRANSFER_SIZE ;
            nConfig =  (nConfig & (~DMA_BITM_CTL_CYCLE_CTL))
                     | DMA_ENUM_CTL_CYCLE_CTL_PING_PONG;
        } else {
            nNumTransfers = pBuffer->nRemTransfers;
        }


        nSrcModify = (uint32_t)1u << ((nConfig & DMA_BITM_CTL_SRC_INC) >> DMA_BITP_CTL_SRC_INC);
        if (nSrcModify > 4u) {nSrcModify = 0u;}

        nDstModify = (uint32_t)1u << ((nConfig & DMA_BITM_CTL_DST_INC) >> DMA_BITP_CTL_DST_INC);
        if (nDstModify > 4u) {nDstModify = 0u;}

        pCCD->DMASRCEND =   (uint32_t)pBuffer->pSrcData
                         + ((pBuffer->nCurPos + nNumTransfers - 1u)  * nSrcModify);
        if ((nConfig & DMA_BITM_CTL_CYCLE_CTL) >= DMA_ENUM_CTL_CYCLE_CTL_MSG_PRI)
        {
            pCCD->DMADSTEND =   (uint32_t)&pAlternateCCD[nChannelNum].RESERVED;
        }
        else
        {
            pCCD->DMADSTEND =   (uint32_t)pBuffer->pDstData
                              + ((pBuffer->nCurPos + nNumTransfers - 1u) * nDstModify);
        }

        pCCD->DMACDC = nConfig | ((nNumTransfers - 1u) << DMA_BITP_CTL_N_MINUS_1);

        pBuffer->nRemTransfers -= nNumTransfers;
        pBuffer->nCurPos += nNumTransfers;
    }
}

/* Initiate a new active buffer transfer */
static void InitNewActiveBuffer(DMA_CHANNEL* pChannel)
{
    uint32_t nChannelBit = 1ul << DMA_CHN_NUM(pChannel->ChannelID);
    uint32_t nMode = pChannel->nCommonCfg & DMA_BITM_CTL_CYCLE_CTL;
    DMA_BUFFER* pBuffer;

    while (   ((pDMA_DevData->ChannelsEnabled & nChannelBit) != 0u)
           && (pChannel->pPendBList != NULL)
           && (pChannel->nNumFreeDesc > 0u))

    {
        if (    (nMode != DMA_ENUM_CTL_CYCLE_CTL_PING_PONG)
             && (pChannel->pActiveBList != NULL))
        {
            break;
        }

        /* Get a buffer from the pending queue */
        pBuffer = fpop_buffer(&pChannel->pPendBList);
        assert (pBuffer != NULL);

        if (nMode != DMA_ENUM_CTL_CYCLE_CTL_PING_PONG)
        {
            /* Reset prim/alt selection */
            pChannel->bIsPrimNext = true;
            pChannel->bIsPrimInProgress = true;
            pADI_DMA0->ALT_CLR = nChannelBit;

            if ((nMode == DMA_ENUM_CTL_CYCLE_CTL_AUTO_REQ) || (nMode == DMA_ENUM_CTL_CYCLE_CTL_BASIC))
            {
                /* Set the correct direction */
                SetupDMAIncrement(pChannel, pBuffer->nOtherCfgValues);
            }

            /* Check an assumption */
            assert (pChannel->nNumFreeDesc == 2u);
        }

        /* Fill the descriptor */
        FillDescriptor(pChannel, pBuffer);

        /* Push it to the active queue */
        bpush_buffer(&pChannel->pActiveBList, pBuffer);

        /* Enable the channel */
        pADI_DMA0->EN_SET = nChannelBit;
    }
}

/* Set the DMA increment */
static void SetupDMAIncrement (DMA_CHANNEL* pChannel, uint32_t nConfig)
{
    uint32_t nChannelBit = 1ul << DMA_CHN_NUM(pChannel->ChannelID);

    /* Clear the decrement for source and destination */
    pADI_DMA0->SRCADDR_CLR = nChannelBit;
    pADI_DMA0->DSTADDR_CLR = nChannelBit;

    if ((nConfig & DMA_BITM_OCTL_SRC_DECR) != 0u) {
        /* Set Src decrement */
        pADI_DMA0->SRCADDR_SET = nChannelBit;
    }

    if ((nConfig & DMA_BITM_OCTL_DST_DECR) != 0u) {
        /* Set Dst decrement */
        pADI_DMA0->DSTADDR_SET = nChannelBit;
    }
}

/* Issue a Transfer Request */
static void IssueTransferRequest(DMA_CHANNEL* pChannel)
{
    uint32_t nChannelBit = 1ul << DMA_CHN_NUM(pChannel->ChannelID);

    /* Issue a software request if the DMA is a software DMA */
    if ((pDMA_DevData->SoftwareChns & nChannelBit) != 0u) {
        pADI_DMA0->SWREQ = nChannelBit;
    }
    /* IF (The DMA is a peripheral DMA then clear the peripheral interrupt mask) */
    else
    {
        pADI_DMA0->RMSK_CLR = nChannelBit;
    }
}


/* Setup the device */
static void SetupDevice (DMA_CHANNEL* pChannel)
{
    uint32_t nChannelBit = 1ul << DMA_CHN_NUM(pChannel->ChannelID);

    /* Set the number of free descriptors */
    pChannel->nNumFreeDesc = 2u;

    /* Reset the primary/alternate selection */
    pChannel->bIsPrimNext = true;
    pChannel->bIsPrimInProgress = true;

    /* IF (The mode of operation is ping-pong) */
    if ((pChannel->nCommonCfg & DMA_BITM_CTL_CYCLE_CTL) == DMA_ENUM_CTL_CYCLE_CTL_PING_PONG) {
        /* Set the DMA increment direction */
        SetupDMAIncrement(pChannel, pChannel->nOtherCmnCfg);
    }

    /* Set the primary as the current DMA descriptor */
    pADI_DMA0->ALT_CLR = nChannelBit;
}

/* DMA Interrupt Handler */
static void DMAInterruptHandler(DMA_CHANNEL* pChannel)
{
    bool_t bNewFromPending = false;

    /* Return if the channel is not used */
    if (pChannel == NULL) {return;}
    assert (pChannel->pActiveBList != NULL);

    uint32_t nChannelBit = 1ul << DMA_CHN_NUM(pChannel->ChannelID);

    /* Increment the number of free discriptors */
    pChannel->nNumFreeDesc++;

    if (pChannel->pActiveBList->nRemDoneTransfers <= DMA_MAX_TRANSFER_SIZE) {
        pChannel->pActiveBList->nRemDoneTransfers = 0u;
    } else {
        pChannel->pActiveBList->nRemDoneTransfers -= DMA_MAX_TRANSFER_SIZE;
    }


    /* If the buffer is done transferring */
    if (pChannel->pActiveBList->nRemDoneTransfers == 0u)
    {
        __I __C void *pData;
        void *pArg;
        DMA_BUFFER* pBuffer;

        /* Pop the done buffer */
        pBuffer = fpop_buffer(&pChannel->pActiveBList);

        /* Set the pData as destination pointer by default) */
        pData = pBuffer->pDstData;

        /* IF (The given DMA is a peripheral DMA) */
        if ((pDMA_DevData->SoftwareChns & nChannelBit) == 0u) {
            /* IF (The destination increment is NONE) */
            if (((pBuffer->nCfgValues & DMA_BITM_CTL_DST_INC) >> DMA_BITP_CTL_DST_INC) == 0x3u)
            {
                /* Set the pData to be source buffer */
                pData = pBuffer->pSrcData;
            }
        }

#ifdef __ICCARM__
/*
* Pm142 (rule 11.5): A cast shall not be performed that removes 'const' or 'volatile'
*                    qualification from the type addressed by the pointer
*   Needed to pass buffer address to callback.
*/
#pragma diag_suppress=Pm142
#endif /* __ICCARM__ */

        pArg = (void *)pData;

#ifdef __ICCARM__
#pragma diag_default=Pm142
#endif /* __ICCARM__ */

        /* Give back the buffer to the device */
        if (pChannel->pfCallback) {
            pChannel->pfCallback (
                                  pChannel->pCBParam,
                                  ADI_DMA_EVENT_BUFFER_PROCESSED,
                                  pArg
                                  );
        }

        /* Push the buffer to the free queue */
        bpush_buffer(&pChannel->pFreeBList, pBuffer);
    }

    /* IF (Any active buffer is there in the queue) */
    if (pChannel->pActiveBList != NULL)
    {
        DMA_BUFFER* pBuffer;

        /* Get the last buffer in the active queue */
        pBuffer = pChannel->pActiveBList;
        while (pBuffer->pNext != NULL) {
          pBuffer = pBuffer->pNext;
        }

        if (pBuffer->nRemTransfers == 0u) {
            bNewFromPending = true;
        } else {
            /* Fill the next descriptor */
            FillDescriptor(pChannel, pBuffer);
        }
    } else {
        bNewFromPending = true;
    }

    /* IF (A new buffer need to be scheduled from the pending buffer list) */
    if (bNewFromPending == true)
    {
        InitNewActiveBuffer(pChannel);
    }

    /* IF (Any active buffer is there in the queue) */
    if (pChannel->pActiveBList != NULL)
    {
        /* Give a software request if the DMA is a software DMA for the next transfer */
        IssueTransferRequest(pChannel);
    }
}
/*! \cond PRIVATE */
/* DMA Error Handler */
ADI_INT_HANDLER (DMA_Err_Int_Handler)
{
    ISR_PROLOG();      
    uint32_t nErrClr = pADI_DMA0->ERR_CLR;
    uint32_t nErrChnClr = pADI_DMA0->ERRCHNL_CLR;
    uint32_t nInvdDescClr = pADI_DMA0->INVALIDDESC_CLR;
  

    if ((nErrChnClr | nInvdDescClr) > 0u)
    {
        /* Get the channels that need to be reported */
        uint32_t nTemp = pDMA_DevData->ChannelsInUse & (nErrChnClr | nInvdDescClr);

        if (nTemp)
        {
            /* in C89 we need to have the for loop counter declared outside the loop */
            uint32_t x;
            for (x = 0u; x < CCD_SIZE; x++)
            {
                /* IF (Channel x has an error pending) */
                if ((((uint32_t)1u << x) & nTemp) != 0u)
                {
                    DMA_CHANNEL* pChannel = pDMA_DevData->ChnHandles[x];

                    if (pChannel->pfCallback)
                    {
                        uint32_t nEvent = ((nErrChnClr & ((uint32_t)1u << x)) != 0u)
                                             ? (uint32_t)ADI_DMA_EVENT_ERR_BUS : (uint32_t)ADI_DMA_EVENT_ERR_INVALID_DESCRIPTOR;

                        pChannel->pfCallback (
                                              pChannel->pCBParam,
                                              nEvent,
                                              NULL
                                              );
                    }
                }
            }
        }
    }

    /* Clear the errors */
    pADI_DMA0->ERRCHNL_CLR = nErrChnClr;
    pADI_DMA0->INVALIDDESC_CLR = nInvdDescClr;
    pADI_DMA0->ERR_CLR = nErrClr;
    ISR_EPILOG();           
}

/* DMA Interrupt Handlers */
ADI_INT_HANDLER (DMA_SPI2_TX_Int_Handler)  
{  
   ISR_PROLOG(); 
   DMAInterruptHandler(pDMA_DevData->ChnHandles[(uint32_t)SPI2_TX_CHANn]);
   ISR_EPILOG();   
}
ADI_INT_HANDLER (DMA_SPI2_RX_Int_Handler)  
{ 
   ISR_PROLOG(); 
   DMAInterruptHandler(pDMA_DevData->ChnHandles[(uint32_t)SPI2_RX_CHANn]); 
   ISR_EPILOG();    
}
ADI_INT_HANDLER (DMA_SPORT0A_Int_Handler)  
{ 
   ISR_PROLOG(); 
   DMAInterruptHandler(pDMA_DevData->ChnHandles[(uint32_t)SPORT0A_CHANn]); 
   ISR_EPILOG();    
}
ADI_INT_HANDLER (DMA_SPORT0B_Int_Handler)  
{ 
   ISR_PROLOG(); 
   DMAInterruptHandler(pDMA_DevData->ChnHandles[(uint32_t)SPORT0B_CHANn]); 
   ISR_EPILOG();       
}
ADI_INT_HANDLER (DMA_SPI0_TX_Int_Handler)  
{ 
   ISR_PROLOG();     
   DMAInterruptHandler(pDMA_DevData->ChnHandles[(uint32_t)SPI0_TX_CHANn]); 
   ISR_EPILOG();       
}
ADI_INT_HANDLER (DMA_SPI0_RX_Int_Handler)  
{
   ISR_PROLOG();  
   DMAInterruptHandler(pDMA_DevData->ChnHandles[(uint32_t)SPI0_RX_CHANn]); 
   ISR_EPILOG();       
}
ADI_INT_HANDLER (DMA_SPI1_TX_Int_Handler)  
{ 
   ISR_PROLOG(); 
   DMAInterruptHandler(pDMA_DevData->ChnHandles[(uint32_t)SPI1_TX_CHANn]); 
   ISR_EPILOG();       
}
ADI_INT_HANDLER (DMA_SPI1_RX_Int_Handler)  
{ 
   ISR_PROLOG(); 
   DMAInterruptHandler(pDMA_DevData->ChnHandles[(uint32_t)SPI1_RX_CHANn]); 
   ISR_EPILOG();       
}
ADI_INT_HANDLER (DMA_UART_TX_Int_Handler)  
{ 
   ISR_PROLOG(); 
   DMAInterruptHandler(pDMA_DevData->ChnHandles[(uint32_t)UART_TX_CHANn]); 
   ISR_EPILOG();       
}
ADI_INT_HANDLER (DMA_UART_RX_Int_Handler)  
{ 
   ISR_PROLOG(); 
   DMAInterruptHandler(pDMA_DevData->ChnHandles[(uint32_t)UART_RX_CHANn]); 
   ISR_EPILOG();    
}
ADI_INT_HANDLER (DMA_I2C0_STX_Int_Handler) 
{ 
   ISR_PROLOG(); 
   DMAInterruptHandler(pDMA_DevData->ChnHandles[(uint32_t)I2CS_TX_CHANn]); 
   ISR_EPILOG();       
}
ADI_INT_HANDLER (DMA_I2C0_SRX_Int_Handler) 
{ 
   ISR_PROLOG(); 
   DMAInterruptHandler(pDMA_DevData->ChnHandles[(uint32_t)I2CS_RX_CHANn]); 
   ISR_EPILOG();       
}
ADI_INT_HANDLER (DMA_I2C0_MX_Int_Handler)  
{ 
   ISR_PROLOG(); 
   DMAInterruptHandler(pDMA_DevData->ChnHandles[(uint32_t)I2CM_CHANn]); 
   ISR_EPILOG();       
}
ADI_INT_HANDLER (DMA_AES0_IN_Int_Handler)  
{ 
   ISR_PROLOG(); 
   DMAInterruptHandler(pDMA_DevData->ChnHandles[(uint32_t)AES0_IN_CHANn]); 
   ISR_EPILOG();       
}
ADI_INT_HANDLER (DMA_AES0_OUT_Int_Handler) 
{ 
   ISR_PROLOG(); 
   DMAInterruptHandler(pDMA_DevData->ChnHandles[(uint32_t)AES0_OUT_CHANn]); 
   ISR_EPILOG();       
}
ADI_INT_HANDLER (DMA_FLASH0_Int_Handler)   
{ 
   ISR_PROLOG(); 
   DMAInterruptHandler(pDMA_DevData->ChnHandles[(uint32_t)FLASH_CHANn]); 
   ISR_EPILOG();       
}

ADI_INT_HANDLER (DMA_SIP0_Int_Handler)     
{ 
   ISR_PROLOG(); 
   DMAInterruptHandler(pDMA_DevData->ChnHandles[(uint32_t)SIP0_CHANn]); 
   ISR_EPILOG();       
}
ADI_INT_HANDLER (DMA_SIP1_Int_Handler)     
{ 
   ISR_PROLOG(); 
   DMAInterruptHandler(pDMA_DevData->ChnHandles[(uint32_t)SIP1_CHANn]); 
   ISR_EPILOG();       
}
ADI_INT_HANDLER (DMA_SIP2_Int_Handler)     
{ 
   ISR_PROLOG(); 
   DMAInterruptHandler(pDMA_DevData->ChnHandles[(uint32_t)SIP2_CHANn]); 
   ISR_EPILOG();       
}
ADI_INT_HANDLER (DMA_SIP3_Int_Handler)     
{ 
   ISR_PROLOG(); 
   DMAInterruptHandler(pDMA_DevData->ChnHandles[(uint32_t)SIP3_CHANn]);
   ISR_EPILOG();    
}
ADI_INT_HANDLER (DMA_SIP4_Int_Handler)     
{ 
   ISR_PROLOG(); 
   DMAInterruptHandler(pDMA_DevData->ChnHandles[(uint32_t)SIP4_CHANn]); 
   ISR_EPILOG();       
}
ADI_INT_HANDLER (DMA_SIP5_Int_Handler)     
{ 
   ISR_PROLOG(); 
   DMAInterruptHandler(pDMA_DevData->ChnHandles[(uint32_t)SIP5_CHANn]); 
   ISR_EPILOG();    
}
ADI_INT_HANDLER (DMA_SIP6_Int_Handler)     
{ 
   ISR_PROLOG(); 
   DMAInterruptHandler(pDMA_DevData->ChnHandles[(uint32_t)SIP6_CHANn]); 
   ISR_EPILOG();       
}
ADI_INT_HANDLER (DMA_SIP7_Int_Handler)     
{ 
   ISR_PROLOG(); 
   DMAInterruptHandler(pDMA_DevData->ChnHandles[(uint32_t)SIP7_CHANn]); 
   ISR_EPILOG();       
}
ADI_INT_HANDLER (DMA_ADC0_Int_Handler)     
{ 
   ISR_PROLOG(); 
   DMAInterruptHandler(pDMA_DevData->ChnHandles[(uint32_t)ADC0_CHANn]); 
   ISR_EPILOG();       
}

/*==========  B U F F E R    F U N C T I O N S  ==========*/
/* Pop a buffer from the front of a buffer list */
static DMA_BUFFER* fpop_buffer (DMA_BUFFER** ppBList)
{
    assert (ppBList != NULL);
    DMA_BUFFER* pBuffer = NULL;

    /* Enter Critical Region */
    ADI_ENTER_CRITICAL_REGION();

    /* IF (Buffer list is not empty) */
    if (*ppBList) {
        pBuffer = *ppBList;
        *ppBList = (*ppBList)->pNext;
        pBuffer->pNext = NULL;
    }

    /* Exit Critical Region */
    ADI_EXIT_CRITICAL_REGION();

    return pBuffer;
}

/* Push a buffer to the back of the buffer list */
static void bpush_buffer (DMA_BUFFER** ppBList, DMA_BUFFER* pBuffer)
{
    assert ((ppBList != NULL) && (pBuffer != NULL));
    assert (pBuffer->pNext == NULL);

    ADI_ENTER_CRITICAL_REGION();

    DMA_BUFFER* pBList = *ppBList;
    if (pBList) {
        while (pBList->pNext != NULL) {
            pBList = pBList->pNext;
        }
        pBList->pNext = pBuffer;
    } else {
        *ppBList = pBuffer;
    }

    ADI_EXIT_CRITICAL_REGION();
}


/*==========  D E B U G    F U N C T I O N S  ==========*/

#ifdef ADI_DEBUG
/* Validate the given handle */
static ADI_DMA_RESULT ValidateHandle (DMA_CHANNEL* pChannel)
{
    ADI_DMA_RESULT eResult = ADI_DMA_ERR_INVALID_HANDLE;

    if (pChannel != NULL)
    {
        /* in C89 we need to have the for loop counter declared outside the loop */
        uint32_t x;
        for (x = 0u; x < CCD_SIZE; x++)
        {
            if (pDMA_DevData->ChnHandles[x] == pChannel) {
                eResult = ADI_DMA_SUCCESS;
                break;
            }
        }
    }

    return eResult;
}

#endif

/*! \endcond */
#endif /* _ADI_DMA_PL230_V2_C_ */

/*@}*/
