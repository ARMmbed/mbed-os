/*!
 *****************************************************************************
 * @file:    adi_dma.h
 * @brief:   DMA Device Definitions for ADuCxxx
 *-----------------------------------------------------------------------------
 *
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

/*! \addtogroup DMA_Driver DMA Driver
 *  @{
 *  @brief DMA Driver
 *  @details This driver is intended to be used only by the device drivers and not by the application.
 *  @note The device drivers must include drivers/dma/adi_dma.h to use this driver     
 */

#ifndef ADI_DMA__H__
#define ADI_DMA__H__

#include <adi_callback.h>


#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

/*=============  D E F I N E S  =============*/
/*! Amount of memory(In bytes) required by the DMA manager for managing the operation
 *  This memory is completely owned by the driver till the end of the operation.
 */

/*============= D A T A T Y P E S =============*/


/*!
 * Dma Data Increments
 */
typedef enum
{
    ADI_DMA_INCR_1_BYTE    = 0x00u,     /*!< Byte increment */
    ADI_DMA_INCR_2_BYTE    = 0x01u,     /*!< Half word increment */
    ADI_DMA_INCR_4_BYTE    = 0x02u,     /*!< Word increment */
    ADI_DMA_INCR_NONE      = 0x03u,     /*!< No increment */

    ADI_DMA_DECR_1_BYTE    = 0x10u,     /*!< Byte decrement */
    ADI_DMA_DECR_2_BYTE    = 0x11u,     /*!< Half word decrement */
    ADI_DMA_DECR_4_BYTE    = 0x12u      /*!< Word decrement */

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
    ADI_DMA_SUCCESS,                    /*!< Successfully Completed */
    ADI_DMA_ERR_NOT_INITIALIZED,        /*!< DMA not initialized */
    ADI_DMA_ERR_INVALID_PARAMETER,      /*!< Input parameter to the function is invalid */
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
    UART0_TX_CHANn      =  8,  /*!< UART0 Transmit DMA channel               */
    UART0_RX_CHANn      =  9,  /*!< UART0 Receive DMA channel                */
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
    UART1_TX_CHANn      = 25,  /*!< UART1 Transmit DMA channel               */
    UART1_RX_CHANn      = 26,  /*!< UART1 Receive DMA channel                */
    NUM_DMA_CHANNELSn   = 27   /*!< Total Number of DMA channels             */
} DMA_CHANn_TypeDef;  /** typedef name for fixed DMA channel assignments */
/*! \endcond */

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


/*! \cond PRIVATE  */
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

#define DMA_TRANSFER_LIMIT      (1024u) /*!< Maximum number of transfers handled by the DMA in one request */

/* pointer to the primary CCD array */
extern ADI_DCC_TypeDef* const pPrimaryCCD;
/* pointer to the alternate CCD array */
extern ADI_DCC_TypeDef* const pAlternateCCD;
/*! \endcond  */
/*==========  DMA API DECLARATIONS  ==========*/

extern void adi_dma_Init(void);

extern ADI_DMA_RESULT adi_dma_RegisterCallback (
    DMA_CHANn_TypeDef   const eChannelID,
    ADI_CALLBACK        const pfCallback,
    void*               const pCBParam
    );

#ifdef __cplusplus
}
#endif

#endif /* include guard */

/*
** EOF
*/

/**@}*/
