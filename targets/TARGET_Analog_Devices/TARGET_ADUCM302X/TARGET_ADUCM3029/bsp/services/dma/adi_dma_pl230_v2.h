/*!
 *****************************************************************************
 * @file:    adi_dma_pl230_v2.h
 * @brief:   uDMA Device Definitions for ADuCxxx
 * @version: $Revision$
 * @date:    $Date$
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
TITLE, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. ADI_DMA_MODE_PING_PONG
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

/*! \addtogroup DMA_Service DMA Service
 *  uDMA Service
 *  @{
 */

#ifndef __ADI_DMA_PL230_V2_H__
#define __ADI_DMA_PL230_V2_H__

#include <services/int/adi_int.h>
#include <adi_types.h>

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

/*=============  D E F I N E S  =============*/
/*! Amount of memory(In bytes) required by the DMA manager for managing the operation
 *  This memory is completely owned by the driver till the end of the operation.
 */
#define ADI_DMA_MEMORY_SIZE       (100u)


/*============= D A T A T Y P E S =============*/
/*! Handle to the DMA Device
 */
typedef void* ADI_DMA_CHANNEL_HANDLE;

/*!
 * Dma Data Increments
 */
typedef enum
{
    ADI_DMA_INCR_1_BYTE    = 0x00,      /*!< Byte increment */
    ADI_DMA_INCR_2_BYTE    = 0x01,      /*!< Half word increment */
    ADI_DMA_INCR_4_BYTE    = 0x02,      /*!< Word increment */
    ADI_DMA_INCR_NONE      = 0x03,      /*!< No increment */

    ADI_DMA_DECR_1_BYTE    = 0x10,      /*!< Byte decrement */
    ADI_DMA_DECR_2_BYTE    = 0x11,      /*!< Half word decrement */
    ADI_DMA_DECR_4_BYTE    = 0x12       /*!< Word decrement */

} ADI_DMA_INCR_TYPE;

/*!
 *  DMA Callback Events
 */
typedef enum
{
    ADI_DMA_EVENT_BUFFER_PROCESSED,          /*!< Buffer processed event */
    ADI_DMA_EVENT_ERR_BUS,                   /*!< Bus Error Occurred Event */
    ADI_DMA_EVENT_ERR_INVALID_DESCRIPTOR     /*!< Invalid Descriptor Event */
} ADI_DMA_EVENT;


/*!
 * Dma Data Widths
 */
typedef enum
{
    ADI_DMA_WIDTH_1_BYTE      = 0x0,      /*!<  8-bit */
    ADI_DMA_WIDTH_2_BYTE      = 0x1,      /*!< 16-bit */
    ADI_DMA_WIDTH_4_BYTE      = 0x2       /*!< 32-bit */
} ADI_DMA_WIDTH_TYPE;


/*!
 * Dma Rearbitration Intervals (chunk size between bus arbitrations)
 */
typedef enum
{
    ADI_DMA_RPOWER_1 = 0,                       /*!< Rearbitrate after    1 transfer */
    ADI_DMA_RPOWER_2,                           /*!< Rearbitrate after    2 transfers */
    ADI_DMA_RPOWER_4,                           /*!< Rearbitrate after    4 transfers */
    ADI_DMA_RPOWER_8,                           /*!< Rearbitrate after    8 transfers */
    ADI_DMA_RPOWER_16,                          /*!< Rearbitrate after   16 transfers */
    ADI_DMA_RPOWER_32,                          /*!< Rearbitrate after   32 transfers */
    ADI_DMA_RPOWER_64,                          /*!< Rearbitrate after   64 transfers */
    ADI_DMA_RPOWER_128,                         /*!< Rearbitrate after  128 transfers */
    ADI_DMA_RPOWER_256,                         /*!< Rearbitrate after  256 transfers */
    ADI_DMA_RPOWER_512,                         /*!< Rearbitrate after  512 transfers */
    ADI_DMA_RPOWER_1024                         /*!< Rearbitrate after 1024 transfers */
} ADI_DMA_RPOWER;


/*!
 * Dma Transfer Modes
 */
typedef enum
{
    ADI_DMA_MODE_BASIC,                         /*!< Basic mode */
    ADI_DMA_MODE_AUTO,                          /*!< Auto request mode */
    ADI_DMA_MODE_PING_PONG,                     /*!< Ping pong mode */
    ADI_DMA_MODE_MSG,                           /*!< Memory Scatter gather mode (not valid as no Memory DMA support) */
    ADI_DMA_MODE_PSG                            /*!< Peripheral Scatter mode */
} ADI_DMA_MODE;


/*!
 * Dma Channel Priority Settings (only HIGH or DEFAULT priority supported)
 */
typedef enum
{
    ADI_DMA_PRIORITY_DEFAULT = 0,                       /*!< Use DEFAULT channel priority */
    ADI_DMA_PRIORITY_HIGH                              /*!< Elevate channel to HIGH priority */
} ADI_DMA_PRIORITY;


/*!
 * Result Event Type
 */
typedef enum {
    ADI_DMA_SUCCESS,                          /*!< Successfully Completed */
    ADI_DMA_ERR_ALREADY_INITIALIZED,          /*!< Specified DMA channel is already initialized */
    ADI_DMA_ERR_INVALID_HANDLE,               /*!< Invalid DMA handle */
    ADI_DMA_ERR_INVALID_PARAMETER,            /*!< Input parameter to the function is invalid */
    ADI_DMA_ERR_INVALID_BUFFER,               /*!< Specified buffer is not valid */
    ADI_DMA_ERR_NO_FREE_BUFFERS,              /*!< Error as there is no free buffers in the driver */
    ADI_DMA_ERR_INVALID_STATE,                /*!< Error as the driver cannot service the request in the current state */
    ADI_DMA_ERR_INVALID_DESC_LIST             /*!< DMA Descriptor List submitted is invalid */
} ADI_DMA_RESULT;

/*! \cond PRIVATE*/
/*!
 * \enum DMA_CHANn_TypeDef
 * DMA Channel Assignments
 */
typedef enum
{
    SPI2_TX_CHANn       =  0,  /*!< SPI2 Transmit DMA channel                */
    SPI2_RX_CHANn       =  1,  /*!< SPI2 Receive DMA channel                 */
    SPORT0A_CHANn       =  2,  /*!< SPORT0-A  DMA channel                    */
    SPORT0B_CHANn       =  3,  /*!< SPORT0-B  DMA channel                    */
    SPI0_TX_CHANn       =  4,  /*!< SPI0 Transmit DMA channel                */
    SPI0_RX_CHANn       =  5,  /*!< SPI0 Receive DMA channel                 */
    SPI1_TX_CHANn       =  6,  /*!< SPI1 Transmit DMA channel                */
    SPI1_RX_CHANn       =  7,  /*!< SPI1 Receive DMA channel                 */
    UART_TX_CHANn       =  8,  /*!< UART Transmit DMA channel                */
    UART_RX_CHANn       =  9,  /*!< UART Receive DMA channel                 */
    I2CS_TX_CHANn       = 10,  /*!< I2C Slave Transmit DMA channel           */
    I2CS_RX_CHANn       = 11,  /*!< I2C Slave Receive DMA channel            */
    I2CM_CHANn          = 12,  /*!< I2C Master DMA channel                   */
    AES0_IN_CHANn       = 13,  /*!< AES0-IN DMA channel                      */
    AES0_OUT_CHANn      = 14,  /*!< AES0-OUT DMA channel                     */
    FLASH_CHANn         = 15,  /*!< FLASH DMA channel                        */
    SIP0_CHANn          = 16,  /*!< SIP-0 DMA channel                        */
    SIP1_CHANn          = 17,  /*!< SIP-1 DMA channel                        */
    SIP2_CHANn          = 18,  /*!< SIP-2 DMA channel                        */
    SIP3_CHANn          = 19,  /*!< SIP-3 DMA channel                        */
    SIP4_CHANn          = 20,  /*!< SIP-4 DMA channel                        */
    SIP5_CHANn          = 21,  /*!< SIP-5 DMA channel                        */
    SIP6_CHANn          = 22,  /*!< SIP-6 DMA channel                        */
    SIP7_CHANn          = 23,  /*!< SIP-7 DMA channel                        */
    ADC0_CHANn          = 24,  /*!< ADC0  DMA channel                        */    
    NUM_DMA_CHANNELSn   = 25   /*!< Total Number of DMA channels             */
} DMA_CHANn_TypeDef;  /** typedef name for fixed DMA channel assignments */
/*! \endcond */

/*!
 * \enum ADI_DMA_CHANNEL_ID
 * DMA Channel ID
 */
typedef enum {
    /*! SPI2 Transmit DMA channel */
    ADI_DMA_CHANNEL_SPI2_TX  = (( (uint32_t)DMA0_CH0_DONE_IRQn    << 16u) | (uint32_t)SPI2_TX_CHANn),
    /*! SPI2 Receive DMA channel */
    ADI_DMA_CHANNEL_SPI2_RX  = (( (uint32_t)DMA0_CH1_DONE_IRQn    << 16u) | (uint32_t)SPI2_RX_CHANn),
    /*! SPORT0-A  DMA channel */
    ADI_DMA_CHANNEL_SPORT0_A = (( (uint32_t)DMA0_CH2_DONE_IRQn    << 16u) | (uint32_t)SPORT0A_CHANn),
    /*! SPORT0-B  DMA channel */
    ADI_DMA_CHANNEL_SPORT0_B = (( (uint32_t)DMA0_CH3_DONE_IRQn    << 16u) | (uint32_t)SPORT0B_CHANn),
    /*! SPI0 Transmit DMA channel */
    ADI_DMA_CHANNEL_SPI0_TX  = (( (uint32_t)DMA0_CH4_DONE_IRQn    << 16u) | (uint32_t)SPI0_TX_CHANn),
    /*! SPI0 Receive DMA channel */
    ADI_DMA_CHANNEL_SPI0_RX  = (( (uint32_t)DMA0_CH5_DONE_IRQn    << 16u) | (uint32_t)SPI0_RX_CHANn),
    /*! SPI1 Transmit DMA channel */
    ADI_DMA_CHANNEL_SPI1_TX  = (( (uint32_t)DMA0_CH6_DONE_IRQn    << 16u) | (uint32_t)SPI1_TX_CHANn),
    /*! SPI1 Receive DMA channel */
    ADI_DMA_CHANNEL_SPI1_RX  = (( (uint32_t)DMA0_CH7_DONE_IRQn    << 16u) | (uint32_t)SPI1_RX_CHANn),
    /*! UART Transmit DMA channel */
    ADI_DMA_CHANNEL_UART_TX  = (( (uint32_t)DMA0_CH8_DONE_IRQn    << 16u) | (uint32_t)UART_TX_CHANn),
    /*! UART Receive DMA channel */
    ADI_DMA_CHANNEL_UART_RX  = (( (uint32_t)DMA0_CH9_DONE_IRQn    << 16u) | (uint32_t)UART_RX_CHANn),
    /*! I2C Slave Transmit DMA channel */
    ADI_DMA_CHANNEL_I2CS_TX  = (( (uint32_t)DMA0_CH10_DONE_IRQn   << 16u) | (uint32_t)I2CS_TX_CHANn),
    /*! I2C Slave Receive DMA channel */
    ADI_DMA_CHANNEL_I2CS_RX  = (( (uint32_t)DMA0_CH11_DONE_IRQn   << 16u) | (uint32_t)I2CS_RX_CHANn),
    /*! I2C Master DMA channel */
    ADI_DMA_CHANNEL_I2CM     = (( (uint32_t)DMA0_CH12_DONE_IRQn   << 16u) | (uint32_t)I2CM_CHANn),
    /*! AES0-IN DMA channel */
    ADI_DMA_CHANNEL_AES0_IN  = (( (uint32_t)DMA0_CH13_DONE_IRQn   << 16u) | (uint32_t)AES0_IN_CHANn),
    /*! AES0-OUT DMA channel */
    ADI_DMA_CHANNEL_AES0_OUT = (( (uint32_t)DMA0_CH14_DONE_IRQn   << 16u) | (uint32_t)AES0_OUT_CHANn),
    /*! FLASH DMA channel */
    ADI_DMA_CHANNEL_FLASH    = (( (uint32_t)DMA0_CH15_DONE_IRQn   << 16u) | (uint32_t)FLASH_CHANn),
    /*! SIP-0 DMA channel */
    ADI_DMA_CHANNEL_SIP_0    = (( (uint32_t)DMA0_CH16_DONE_IRQn   << 16u) | (uint32_t)SIP0_CHANn),
    /*! SIP-1 DMA channel */
    ADI_DMA_CHANNEL_SIP_1    = (( (uint32_t)DMA0_CH17_DONE_IRQn   << 16u) | (uint32_t)SIP1_CHANn),
    /*! SIP-2 DMA channel */
    ADI_DMA_CHANNEL_SIP_2    = (( (uint32_t)DMA0_CH18_DONE_IRQn   << 16u) | (uint32_t)SIP2_CHANn),
    /*! SIP-3 DMA channel */
    ADI_DMA_CHANNEL_SIP_3    = (( (uint32_t)DMA0_CH19_DONE_IRQn   << 16u) | (uint32_t)SIP3_CHANn),
    /*! SIP-4 DMA channel */
    ADI_DMA_CHANNEL_SIP_4    = (( (uint32_t)DMA0_CH20_DONE_IRQn   << 16u) | (uint32_t)SIP4_CHANn),
    /*! SIP-5 DMA channel */
    ADI_DMA_CHANNEL_SIP_5    = (( (uint32_t)DMA0_CH21_DONE_IRQn   << 16u) | (uint32_t)SIP5_CHANn),
    /*! SIP-6 DMA channel */
    ADI_DMA_CHANNEL_SIP_6    = (( (uint32_t)DMA0_CH22_DONE_IRQn   << 16u) | (uint32_t)SIP6_CHANn),
    /*! SIP-7 DMA channel */
    ADI_DMA_CHANNEL_SIP_7    = (( (uint32_t)DMA0_CH23_DONE_IRQn   << 16u) | (uint32_t)SIP7_CHANn),
    /*! ADC0 DMA channel */
    ADI_DMA_CHANNEL_ADC0     = (( (uint32_t)DMA0_CH24_DONE_IRQn   << 16u) | (uint32_t)ADC0_CHANn)
    
} ADI_DMA_CHANNEL_ID;

/*!
 * \struct ADI_DCC_TypeDef
 * DMA Channel Control MMR Access Template
 */
typedef struct
{
    __IO uint32_t DMASRCEND;      /*!< Source End Pointer                    */
    __IO uint32_t DMADSTEND;      /*!< Destination End Pointer               */
    __IO uint32_t DMACDC;         /*!< Channel Data Configuration            */
         uint32_t RESERVED;       /*!< Address gap filler                    */
} ADI_DCC_TypeDef;

/*! ADI DMA Transfer structure */
typedef struct _ADI_DMA_TRANSFER {
    __I __C void*       pSrcData;              /*!< Source channel data pointer */
    __IO void*          pDstData;              /*!< Destination channel data pointer */

    ADI_DMA_WIDTH_TYPE  DataWidth;             /*!< size of each transfer unit */
    uint32_t            NumTransfers;          /*!< total number of units to transfer */

    ADI_DMA_INCR_TYPE   SrcInc;                /*!< Source data increments */
    ADI_DMA_INCR_TYPE   DstInc;                /*!< Destination data increments */

} ADI_DMA_TRANSFER;

/*==========  DMA API DECLARATIONS  ==========*/

/* Initialization of the DMA Controller */
ADI_DMA_RESULT adi_dma_Open (
    ADI_DMA_CHANNEL_ID          const eChannelID,
    void*                       const pChannelMem,
    ADI_DMA_CHANNEL_HANDLE*     const phChannel,
    ADI_CALLBACK                const pfCallback,
    void*                       const pCBParam
    );

/* Closes a DMA */
ADI_DMA_RESULT adi_dma_Close (
    ADI_DMA_CHANNEL_HANDLE const hChannel
    );

/* Submit a Transfer to the device */
ADI_DMA_RESULT adi_dma_SubmitTransfer (
    ADI_DMA_CHANNEL_HANDLE const hChannel,
    ADI_DMA_TRANSFER*      const pTransfer
    );

/* Submit a Scatter Transfer */
ADI_DMA_RESULT adi_dma_SubmitScatterTransfer (
    ADI_DMA_CHANNEL_HANDLE  const hChannel,
    ADI_DCC_TypeDef*        const DescList,
    uint32_t                const nNumDesc
    );

/* Return whether transfer is in progress */
ADI_DMA_RESULT adi_dma_IsTransferInProgress (
    ADI_DMA_CHANNEL_HANDLE const hChannel,
    bool_t*                const pbTransferInProgress
    );


/* Set the priority of the given channel */
ADI_DMA_RESULT adi_dma_SetPriority (
    ADI_DMA_CHANNEL_HANDLE const hChannel,
    ADI_DMA_PRIORITY       const ePriority
    );

/* Set Arbitration Rate */
ADI_DMA_RESULT adi_dma_SetArbitrationRate (
    ADI_DMA_CHANNEL_HANDLE const hChannel,
    ADI_DMA_RPOWER         const eArbitRate
    );

/* Enable/Disable the DMA */
ADI_DMA_RESULT adi_dma_Enable (
    ADI_DMA_CHANNEL_HANDLE const hChannel,
    bool_t                 const bEnable
    );

/* Enable/Disable Byte Swap for the given Channel */
ADI_DMA_RESULT adi_dma_SetByteSwap (
    ADI_DMA_CHANNEL_HANDLE const hChannel,
    bool_t                 const bByteSwap
    );

/* Set the mode of operation for the given DMA */
ADI_DMA_RESULT adi_dma_SetMode (
    ADI_DMA_CHANNEL_HANDLE const hChannel,
    ADI_DMA_MODE           const eMode
    );

#ifdef __cplusplus
}
#endif

#endif /* include guard */

/*
** EOF
*/

/*@}*/
