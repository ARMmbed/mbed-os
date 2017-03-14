/*!
 *****************************************************************************
 * @file:    adi_spi_def_v1.h
 * @brief:   SPORT Device Driver definition 
 * @version: $Revision$
 * @date:    $Date$
 *****************************************************************************
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
#ifndef _DEF_SPI_DEF_V1_H_
#define _DEF_SPI_DEF_V1_H_

#include <drivers/spi/adi_spi.h>

/*! \cond PRIVATE */
#define ADI_INTERRUPT_MASK 0X5F07u
#define ADI_INT_CALLBACK_MASK 0xF09Eu

#define ADI_SPI_NUM_INSTANCES         3u

/* make generic defines to adapt old macro names to new macros file */
#ifndef SPIDMA_ENABLE
#define SPIDMA_ENABLE SPI1DMA_ENABLE  /*!< dma enable abstraction */
#endif

#ifndef SPIDMA_IENTXDMA
#define SPIDMA_IENTXDMA SPI1DMA_IENTXDMA  /*!< dma abstraction */
#endif

#ifndef SPIDMA_IENRXDMA
#define SPIDMA_IENRXDMA SPI1DMA_IENRXDMA  /*!< dma abstraction */
#endif

#ifndef SPICON_OEN
#define SPICON_OEN SPICON_SOEN  /*!< slave output enable abstraction */
#endif

/*
 *****************************************************************************
 * SPI Bitrate Initializer.  Sets a default serial clockrate for the SPI channel.
 *****************************************************************************/
/* #define ADI_SPI_BITRATE_INITIALIZER   4000000 // 4MHz default bitrate */
#define ADI_SPI_BITRATE_INITIALIZER      250000u  /* depends on processor */

/*
 *****************************************************************************
 * SPI0/SPI1 Control Register Initializer.  This macro configures default
 * settings for the SPI configuration control register when operated in Master-mode.
 *****************************************************************************/
/* SPI master DMA mode control configuration */
#define ADI_SPI_MASTERCON_INITIALIZER      BITM_SPI_CTL_MASEN

/*
 *****************************************************************************
 * SPI0/SPI1 Control Register Initializer.  This macro configures default
 * settings for the SPI configuration control register when operated in Slave-mode.
 *****************************************************************************/
 #define ADI_SPI_SLAVECON_INITIALIZER  BITM_SPI_CTL_OEN           \
                                     | BITM_SPI_CTL_ZEN           \
                                     | BITM_SPI_CTL_SPIEN

/*
 *****************************************************************************
 * SPI DMA Enable Register Initializer.  Sets default DMA enablement to
 * disabled.  Use the #adi_spi_EnableDmaMode() API to enable DMA-based transactions.
 *****************************************************************************/
 #define ADI_SPI_DMA_INITIALIZER     0u

#define ADI_SPI_FIFO_SIZE 8u
/* 16-bit DMA... (two-byte size and increment) */
#define ADI_DMA_DATA_WIDTH      ADI_DMA_WIDTH_2_BYTE  /*!< DMA data attribute */
#define ADI_DMA_DATA_INCREMENT  ADI_DMA_INCR_HALFWORD   /*!< DMA data attribute */


/* Dispatch DMA overhead through function pointers so linker */
/* elimination can optimize them out when not referenced...  */

/*! \typedef ADI_SPI_INIT_FPTR
 * Pointer to DMA/Interrupt initialization function. */
typedef ADI_SPI_RESULT (*ADI_SPI_INIT_FPTR)(ADI_SPI_HANDLE const hDevice);

/*! \typedef ADI_SPI_UNINIT_FPTR
 * Pointer to DMA/Interrupt uninitialization function. */
typedef ADI_SPI_RESULT (*ADI_SPI_UNINIT_FPTR)(ADI_SPI_HANDLE const hDevice);

/*! \typedef ADI_SPI_INIT_DESCRS_FPTR
 * Pointer to DMA descriptor initialization function. */
typedef ADI_SPI_RESULT (*ADI_SPI_INIT_DESCRS_FPTR) (ADI_SPI_HANDLE const hDevice);

/*! \typedef ADI_SPI_DMA_HANDLER_FPTR
 * Pointer to DMA handler function. */

typedef void (*ADI_SPI_DMA_HANDLER_FPTR)(ADI_SPI_HANDLE pDD,ADI_DMA_CHANNEL_ID ChanId);

/*! \struct ADI_SPI_FTAB
 * The function pointer table. */
typedef struct {
    ADI_SPI_INIT_FPTR         pInitFcn;
    ADI_SPI_UNINIT_FPTR       pUnInitFcn;
    ADI_SPI_INIT_DESCRS_FPTR  pInitDescriptorsFcn;
    ADI_SPI_DMA_HANDLER_FPTR  pDmaHandlerFcn;
} ADI_SPI_FTAB;

/*!
 *****************************************************************************
 * \enum ADI_SPI_STATE_TYPE
 *
 * SPI driver state.  Used for internal tracking of the SPI device initialization
 * progress during the adi_SPI_Init() and adi_SPI_SlaveInit() functions.  Also
 * used to ensure the SPI device has been properly initialized as a prerequisite
 * to using the balance of the SPI API.
 *
 *****************************************************************************/
typedef enum
{
    ADI_SPI_STATE_UNINITIALIZED= 0,        /*!< SPI is not initialized.            */
    ADI_SPI_STATE_INITIALIZING,            /*!< SPI initialization is in progress. */
    ADI_SPI_STATE_MASTER,                  /*!< SPI initialization is complete.    */
    ADI_SPI_STATE_SLAVE                    /*!< SPI initialization is complete.    */
} ADI_SPI_DEVICE_STATE;

/*!
 *****************************************************************************
 * SPI Configuration structure.
 *****************************************************************************/
typedef struct ADI_SPI_CONFIG
{
    uint16_t SPI_CTL;                          /*!< SPI_CTL  register configuration. */
    uint16_t SPI_DIV;                          /*!< SPI_DIV  register.     */
} ADI_SPI_CFG_TYPE;

/*! SPI device information */

typedef struct __ADI_SPI_DEVICE_INFO
{
    ADI_SPI_DEVICE_STATE         eState;          /*  To indicate the state of the device */
    const ADI_DMA_CHANNEL_ID     eDMAChnlTxID;    /*  DMA channel ID-Tx */
    const ADI_DMA_CHANNEL_ID     eDMAChnlRxID;    /*  DMA channel ID-Rx */    
    volatile ADI_SPI_TypeDef     *pSpiRegs;       /*  Base address of the  SPI registers */
    const IRQn_Type              eIRQn;          /*  IRQn */   
    ADI_INT_HANDLER_PTR          pfInthandler;   /*  Function pointer for Interrupt handler */ 
    ADI_SPI_HANDLE               hDevice;        /*  SPI handle */
}ADI_SPI_DEVICE_INFO;


/*! \struct ADI_SPI_DEV_DATA_TYPE
 * SPI Device instance data structure */
typedef struct __ADI_SPI_DEV_DATA_TYPE
{

    /* device attributes */
    ADI_SPI_DEVICE_STATE     eDevState;        /*!< track initialization state */
    volatile ADI_SPI_TypeDef*         pSpi;             /*!< track MMR device pointer   */
    ADI_SPI_DEVICE_INFO      *pDevInfo;
    bool_t                   bDmaMode;            /*!< DMA mode flag              */
    bool_t                   bBlockingMode;       /*!< blocking mode flag         */
    ADI_SPI_CHIP_SELECT      ChipSelect;          /*!< track chip select          */
    uint8_t*                 pTxBuffer;          /*!< Transmit Buffer           */
    uint8_t*                 pRxBuffer;          /*!< Receive Buffer            */
    uint8_t                  TxIncrement;        /*!< Transmit Increment        */
    uint8_t                  RxIncrement;        /*!< Receive Increment         */
    uint32_t                 TxRemaining;        /*!< Transmit Count            */
    uint32_t                 RxRemaining;        /*!< Receive Count             */
    uint32_t                 BufferSize;         /*!< buffer size               */
    bool_t                   bTransferComplete;  /*!< Transfer Complete Flag    */
    /* function table */
    ADI_SPI_FTAB             FunctionTable;
    /* Callback and Callback parameters */
    ADI_CALLBACK             pfCallback;
    void *                   pCBParam;
    /* DMA-Mode state */
    volatile bool_t          bTxComplete;   /*!<  TX complete flag   */
    volatile bool_t          bRxComplete;   /*!<  RX complete flag   */
#if (ADI_SPI_CFG_ENABLE_DMA_SUPPORT == 1)
    ADI_DMA_TRANSFER         gDmaDescriptorTx;
    ADI_DMA_TRANSFER         gDmaDescriptorRx;    
    uint8_t                  nDMAMemTx[ADI_DMA_MEMORY_SIZE];
    ADI_DMA_CHANNEL_HANDLE   hDmaTx;
    uint8_t                  nDMAMemRx[ADI_DMA_MEMORY_SIZE];    
    ADI_DMA_CHANNEL_HANDLE   hDmaRx;    
#endif /* ADI_SPI_CFG_ENABLE_DMA_SUPPORT */
    PEND_VAR_DECLR
} ADI_SPI_DEV_DATA_TYPE;

/* forward declarations... */
/* These are needed in the case where ADI_CFG_ENABLE_RTOS_SUPPORT=1              */
/* The ADI_INSTALL_HANDLER macro will expand to a call to adi_int_InstallHandler */
/* and the handler will be referenced                                            */
/* The non-OSAL expansion of ADI_INSTALL_HANDLER results in a NULL string        */
ADI_INT_HANDLER(SPI0_Int_Handler);
ADI_INT_HANDLER(SPI1_Int_Handler);
ADI_INT_HANDLER(SPI2_Int_Handler);
#if (ADI_SPI_CFG_ENABLE_DMA_SUPPORT == 1)
static void spi_DmaTx_InterruptHandler(void *pCbparam, uint32_t Event, void *pArg); 
static void spi_DmaRx_InterruptHandler(void *pCbparam, uint32_t Event, void *pArg);
#endif
/*! \endcond */

#endif	/* end of ifndef _DEF_SPI_DEF_V1_H_ */

