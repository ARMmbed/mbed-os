/**
 * @file xmc_usic.h
 * @date 2017-02-10
 *
 * @cond
 *********************************************************************************************************************
 * XMClib v2.1.16 - XMC Peripheral Driver Library 
 *
 * Copyright (c) 2015-2017, Infineon Technologies AG
 * All rights reserved.                        
 *                                             
 * Redistribution and use in source and binary forms, with or without modification,are permitted provided that the 
 * following conditions are met:   
 *                                                                              
 * Redistributions of source code must retain the above copyright notice, this list of conditions and the following 
 * disclaimer.                        
 * 
 * Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following 
 * disclaimer in the documentation and/or other materials provided with the distribution.                       
 * 
 * Neither the name of the copyright holders nor the names of its contributors may be used to endorse or promote 
 * products derived from this software without specific prior written permission.                                           
 *                                                                              
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, 
 * INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE  
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE  FOR ANY DIRECT, INDIRECT, INCIDENTAL, 
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR  
 * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, 
 * WHETHER IN CONTRACT, STRICT LIABILITY,OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE 
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.                                                  
 *                                                                              
 * To improve the quality of the software, users are encouraged to share modifications, enhancements or bug fixes with 
 * Infineon Technologies AG dave@infineon.com).                                                          
 *********************************************************************************************************************
 *
 * Change History
 * --------------
 *
 * 2015-02-20:
 *     - Initial draft<br>
 *     - Documentation improved <br>
 *
 * 2015-05-08:
 *     - Added XMC_USIC_CH_SetInputTriggerCombinationMode() and XMC_USIC_CH_SetTransmitBufferStatus()  <br>
 *      
 * 2015-06-20:
 *     - Removed version macros and declaration of GetDriverVersion API
 * 
 * 2015-08-17:
 *     - Bug fixed in XMC_USIC_CH_SetTransmitBufferStatus API. OR operator removed.
 *
 * 2015-08-24:
 *     - Added APIs for enabling/disabling delay compensation XMC_USIC_CH_DisableDelayCompensation() and
 *       XMC_USIC_CH_DisableDelayCompensation()
 *
 * 2015-08-25:
 *     - Added APIs for defining if the data shift unit input is derived
 *       from the input data path DXn or from the selected protocol pre-processors: XMC_USIC_CH_ConnectInputDataShiftToPPP()
 *       and XMC_USIC_CH_ConnectInputDataShiftToDataInput()
 *
 * 2015-08-27:
 *     - Fixed bug in XMC_USIC_CH_BRG_CLOCK_SOURCE_DX1T value.
 *     - Added APIs for direct TBUF access: XMC_USIC_CH_WriteToTBUF() and XMC_USIC_CH_WriteToTBUFTCI()
 *     - Added APIs for external input for BRG configuration:XMC_USIC_CH_ConfigExternalInputSignalToBRG() and XMC_USIC_CH_SetBRGInputClockSource()
 *
 * 2015-08-28:
 *     - Added API for enabling the transfer trigger unit to set bit TCSR.TE if the trigger signal DX2T becomes active. Feature used for RS-232
 *       Clear to Send (CTS) signal: XMC_USIC_CH_EnableTBUFDataValidTrigger() and XMC_USIC_CH_DisableTBUFDataValidTrigger().
 *
 * 2016-03-09:
 *     - Optimization of write only registers
 *
 * 2016-04-10:
 *     - Added an API to put the data into FIFO when hardware port control is enabled: XMC_USIC_CH_TXFIFO_PutDataHPCMode() <br>
 *
 * 2017-02-10:
 *     - Added XMC_USIC_CH_SetShiftDirection() to allow selection of shift direction of the data words for transmision and reception
 *     - Added XMC_USIC_CH_GetCaptureTimerValue() and XMC_USIC_CH_SetFractionalDivider()
 *
 * @endcond
 *
 */

#ifndef XMC_USIC_H
#define XMC_USIC_H
/*******************************************************************************
 * HEADER FILES
 *******************************************************************************/

#include "xmc_common.h"

/**
 * @addtogroup XMClib XMC Peripheral Library
 * @{
 */

/**
 * @addtogroup USIC
 * @brief Universal Serial Interface Channel(USIC) driver for serial communication.
 *
 * The Universal Serial Interface Channel(USIC) module is a flexible interface module
 * covering several serial communication protocols. A USIC module contains two
 * independent communication channels named USICx_CH0 and USICx_CH1, with x
 * being the number of the USIC module. The user can program, during run-time, which protocol will be handled
 * by each communication channel and which pins are used.
 * The driver provides APIs, configuration structures and enumerations to configure common features of multiple serial
 * communication protocols.
 * 
 * USIC driver features:
 * -# Allows configuration of FIFO for transmit and receive functions.
 * -# Provides a structure type XMC_USIC_CH_t to represent the USIC channel registers in a programmer
 friendly format.
 * -# Allows configuration of automatic update for frame length, word length, slave select or slave address.
 * -# Allows transmission of data to FIFO using XMC_USIC_CH_TXFIFO_PutData() and XMC_USIC_CH_TXFIFO_PutDataFLEMode()
 * -# Allows reading of received data in FIFO using XMC_USIC_CH_RXFIFO_GetData()
 * -# Allows configuration of baudrate using XMC_USIC_CH_SetBaudrate()
 * -# Provides API to trigger interrupts using XMC_USIC_CH_TriggerServiceRequest()
 * @{
 */

/*******************************************************************************
 * MACROS
 *******************************************************************************/

#define XMC_USIC0 ((XMC_USIC_t *)USIC0_BASE)			/**< USIC0 module base address */
#define XMC_USIC0_CH0 ((XMC_USIC_CH_t *)USIC0_CH0_BASE)	/**< USIC0 channel 0 base address */
#define XMC_USIC0_CH1 ((XMC_USIC_CH_t *)USIC0_CH1_BASE)	/**< USIC0 channel 1 base address */

#if defined(USIC1)
#define XMC_USIC1 ((XMC_USIC_t *)USIC1_BASE)			/**< USIC1 module base address */
#define XMC_USIC1_CH0 ((XMC_USIC_CH_t *)USIC1_CH0_BASE)	/**< USIC1 channel 0 base address */
#define XMC_USIC1_CH1 ((XMC_USIC_CH_t *)USIC1_CH1_BASE)	/**< USIC1 channel 1 base address */
#endif

#if defined(USIC2)
#define XMC_USIC2 ((XMC_USIC_t *)USIC2_BASE)			/**< USIC2 module base address */
#define XMC_USIC2_CH0 ((XMC_USIC_CH_t *)USIC2_CH0_BASE)	/**< USIC2 channel 0 base address */
#define XMC_USIC2_CH1 ((XMC_USIC_CH_t *)USIC2_CH1_BASE)	/**< USIC2 channel 1 base address */
#endif

#define USIC_CH_DXCR_DSEL_Msk  USIC_CH_DX0CR_DSEL_Msk   /**< Common mask for DSEL bitfield mask in DXnCR register */
#define USIC_CH_DXCR_DSEL_Pos  USIC_CH_DX0CR_DSEL_Pos   /**< Common mask for DSEL bitfield position in DXnCR register */
#define USIC_CH_DXCR_SFSEL_Pos USIC_CH_DX0CR_SFSEL_Pos  /**< Common mask for SFSEL bitfield position in DXnCR register */
#define USIC_CH_DXCR_SFSEL_Msk USIC_CH_DX0CR_SFSEL_Msk  /**< Common mask for SFSEL bitfield mask in DXnCR register */
#define USIC_CH_DXCR_DPOL_Msk  USIC_CH_DX0CR_DPOL_Msk   /**< Common mask for DPOL bitfield mask in DXnCR register */
#define USIC_CH_DXCR_DFEN_Msk  USIC_CH_DX0CR_DFEN_Msk   /**< Common mask for DFEN bitfield mask in DXnCR register */
#define USIC_CH_DXCR_DSEN_Msk  USIC_CH_DX0CR_DSEN_Msk   /**< Common mask for DSEN bitfield mask in DXnCR register */
#define USIC_CH_DXCR_CM_Pos    USIC_CH_DX0CR_CM_Pos     /**< Common mask for CM bitfield position in DXnCR register */
#define USIC_CH_DXCR_CM_Msk    USIC_CH_DX0CR_CM_Msk     /**< Common mask for CM bitfield mask in DXnCR register */
#define USIC_CH_DXCR_INSW_Msk  USIC_CH_DX0CR_INSW_Msk   /**< Common mask for INSW bitfield mask in DXnCR register */
#define USIC_CH_DXCR_INSW_pos  USIC_CH_DX0CR_INSW_Pos   /**< Common mask for INSW bitfield position in DXnCR register */

#if UC_FAMILY == XMC1
 #include "xmc1_usic_map.h"
#endif

#if UC_FAMILY == XMC4
 #include "xmc4_usic_map.h"
#endif

/*******************************************************************************
 * ENUMS
 *******************************************************************************/

/**
 * USIC channel driver status
 */
typedef enum XMC_USIC_CH_STATUS
{
  XMC_USIC_CH_STATUS_OK,    /**< USIC driver status : OK */
  XMC_USIC_CH_STATUS_ERROR, /**< USIC driver status : ERROR */
  XMC_USIC_CH_STATUS_BUSY   /**< USIC driver status : BUSY */
} XMC_USIC_CH_STATUS_t;

/**
* USIC channel kernel mode
*/
typedef enum XMC_USIC_CH_KERNEL_MODE
{
  XMC_USIC_CH_KERNEL_MODE_RUN_0  = 0x0UL,  /**< Run mode 0 (transmission and reception possible)*/
  XMC_USIC_CH_KERNEL_MODE_RUN_1  = 0x1UL << USIC_CH_KSCFG_NOMCFG_Pos,  /**< Run mode 1 (transmission and reception possible)*/
  XMC_USIC_CH_KERNEL_MODE_STOP_0 = 0x2UL << USIC_CH_KSCFG_NOMCFG_Pos,  /**< Stop mode 0 (no transmission, but reception possible)*/
  XMC_USIC_CH_KERNEL_MODE_STOP_1 = 0x3UL << USIC_CH_KSCFG_NOMCFG_Pos   /**< Stop mode 1 (both transmission and reception not possible)*/
} XMC_USIC_CH_KERNEL_MODE_t;

/**
 * USIC channel operating mode
 */
typedef enum XMC_USIC_CH_OPERATING_MODE
{
  XMC_USIC_CH_OPERATING_MODE_IDLE = 0x0UL, /**< USIC channel idle */
  XMC_USIC_CH_OPERATING_MODE_SPI  = 0x1UL << USIC_CH_CCR_MODE_Pos, /**< SPI mode */
  XMC_USIC_CH_OPERATING_MODE_UART = 0x2UL << USIC_CH_CCR_MODE_Pos, /**< UART mode */
  XMC_USIC_CH_OPERATING_MODE_I2S  = 0x3UL << USIC_CH_CCR_MODE_Pos, /**< I2S mode */
  XMC_USIC_CH_OPERATING_MODE_I2C  = 0x4UL << USIC_CH_CCR_MODE_Pos  /**< I2C mode */
} XMC_USIC_CH_OPERATING_MODE_t;

/**
 * USIC channel inputs
 */
typedef enum XMC_USIC_CH_INPUT
{
  XMC_USIC_CH_INPUT_DX0, /**< DX0 input */
  XMC_USIC_CH_INPUT_DX1, /**< DX1 input */
  XMC_USIC_CH_INPUT_DX2, /**< DX2 input */
  XMC_USIC_CH_INPUT_DX3, /**< DX3 input */
  XMC_USIC_CH_INPUT_DX4, /**< DX4 input */
  XMC_USIC_CH_INPUT_DX5  /**< DX5 input */
} XMC_USIC_CH_INPUT_t;

/**
 * USIC channel input source sampling frequency
 */
typedef enum XMC_USIC_CH_INPUT_SAMPLING_FREQ
{
  XMC_USIC_CH_INPUT_SAMPLING_FREQ_FPERIPH            = 0x0UL, /**< Use fperiph frequency for input source sampling*/
  XMC_USIC_CH_INPUT_SAMPLING_FREQ_FRACTIONAL_DIVIDER = 0x1UL << USIC_CH_DXCR_SFSEL_Pos  /**< Use fFD(fractional divider) frequency for input source sampling*/
} XMC_USIC_CH_INPUT_SAMPLING_FREQ_t;

/**
 * USIC channel input combination mode
 */
typedef enum XMC_USIC_CH_INPUT_COMBINATION_MODE
{
  XMC_USIC_CH_INPUT_COMBINATION_MODE_TRIGGER_DISABLED = 0x0UL, /**< The trigger activation is disabled.*/
  XMC_USIC_CH_INPUT_COMBINATION_MODE_RISING_EDGE      = 0x1UL, /**< A rising edge activates DXnT*/
  XMC_USIC_CH_INPUT_COMBINATION_MODE_FALLING_EDGE     = 0x2UL, /**< A falling edge activates DXnT*/
  XMC_USIC_CH_INPUT_COMBINATION_MODE_BOTH_EDGES       = 0x3UL, /**< Both edges activate DXnT*/
} XMC_USIC_CH_INPUT_COMBINATION_MODE_t;

/**
 * USIC channel data transmission start modes.
 * Data shifted out of the transmit pin depends on the value configured for the
 * TDEN bitfield of the TCSR register. Following enum values are used for configuring
 * the TCSR->TDEN bitfield.
 */
typedef enum XMC_USIC_CH_START_TRANSMISION_MODE
{
  XMC_USIC_CH_START_TRANSMISION_DISABLED      = 0x0U, /**< Passive data level is sent out on transmission. */
  XMC_USIC_CH_START_TRANSMISION_ON_TDV        = 0x1UL << USIC_CH_TCSR_TDEN_Pos, /**< Transmission of the data word in TBUF can be started if TDV = 1 */
  XMC_USIC_CH_START_TRANSMISION_ON_TDV_DX2S_0 = 0x2UL << USIC_CH_TCSR_TDEN_Pos, /**< Transmission of the data word in TBUF can be started if TDV = 1 while DX2S_0 */
  XMC_USIC_CH_START_TRANSMISION_ON_TDV_DX2S_1 = 0x3UL << USIC_CH_TCSR_TDEN_Pos  /**< Transmission of the data word in TBUF can be started if TDV = 1 while DX2S_1 */
} XMC_USIC_CH_START_TRANSMISION_MODE_t;

/**
 * USIC channel interrupt node pointers
 */
typedef enum XMC_USIC_CH_INTERRUPT_NODE_POINTER
{
  XMC_USIC_CH_INTERRUPT_NODE_POINTER_TRANSMIT_SHIFT      = USIC_CH_INPR_TSINP_Pos, /**< Node pointer for transmit shift interrupt */
  XMC_USIC_CH_INTERRUPT_NODE_POINTER_TRANSMIT_BUFFER     = USIC_CH_INPR_TBINP_Pos, /**< Node pointer for transmit buffer interrupt */
  XMC_USIC_CH_INTERRUPT_NODE_POINTER_RECEIVE             = USIC_CH_INPR_RINP_Pos,  /**< Node pointer for receive interrupt */
  XMC_USIC_CH_INTERRUPT_NODE_POINTER_ALTERNATE_RECEIVE   = USIC_CH_INPR_AINP_Pos,  /**< Node pointer for alternate receive interrupt */
  XMC_USIC_CH_INTERRUPT_NODE_POINTER_PROTOCOL            = USIC_CH_INPR_PINP_Pos   /**< Node pointer for protocol related interrupts */
} XMC_USIC_CH_INTERRUPT_NODE_POINTER_t;

/**
 * USIC channel events
 */
typedef enum XMC_USIC_CH_EVENT
{
  XMC_USIC_CH_EVENT_RECEIVE_START       = USIC_CH_CCR_RSIEN_Msk, /**< Receive start event */
  XMC_USIC_CH_EVENT_DATA_LOST           = USIC_CH_CCR_DLIEN_Msk, /**< Data lost event */
  XMC_USIC_CH_EVENT_TRANSMIT_SHIFT      = USIC_CH_CCR_TSIEN_Msk, /**< Transmit shift event */
  XMC_USIC_CH_EVENT_TRANSMIT_BUFFER     = USIC_CH_CCR_TBIEN_Msk, /**< Transmit buffer event */
  XMC_USIC_CH_EVENT_STANDARD_RECEIVE    = USIC_CH_CCR_RIEN_Msk,  /**< Receive event */
  XMC_USIC_CH_EVENT_ALTERNATIVE_RECEIVE = USIC_CH_CCR_AIEN_Msk,  /**< Alternate receive event */
  XMC_USIC_CH_EVENT_BAUD_RATE_GENERATOR = USIC_CH_CCR_BRGIEN_Msk /**< Baudrate generator event */
} XMC_USIC_CH_EVENT_t;

/**
* USIC channel parity mode
*/
typedef enum XMC_USIC_CH_PARITY_MODE
{
  XMC_USIC_CH_PARITY_MODE_NONE     = 0x0UL,  /**< Disable parity mode */
  XMC_USIC_CH_PARITY_MODE_EVEN     = 0x2UL << USIC_CH_CCR_PM_Pos,  /**< Enable even parity mode */
  XMC_USIC_CH_PARITY_MODE_ODD      = 0x3UL << USIC_CH_CCR_PM_Pos   /**< Enable odd parity mode */
} XMC_USIC_CH_PARITY_MODE_t;

/**
* USIC channel data output mode
*/
typedef enum XMC_USIC_CH_DATA_OUTPUT_MODE
{
  XMC_USIC_CH_DATA_OUTPUT_MODE_NORMAL   = 0x0UL,  /**< Data output normal mode */
  XMC_USIC_CH_DATA_OUTPUT_MODE_INVERTED = 0x1UL << USIC_CH_SCTR_DOCFG_Pos   /**< Data output inverted mode */
} XMC_USIC_CH_DATA_OUTPUT_MODE_t;

/**
* USIC channel data transmit buffer status
*/
typedef enum XMC_USIC_CH_TBUF_STATUS
{
  XMC_USIC_CH_TBUF_STATUS_IDLE = 0x0UL,                 /**< Transfer buffer is currently idle*/
  XMC_USIC_CH_TBUF_STATUS_BUSY = USIC_CH_TCSR_TDV_Msk   /**< Transfer buffer is currently busy*/
} XMC_USIC_CH_TBUF_STATUS_t;



/**
* USIC channel data transmit buffer status modification
*/
typedef enum XMC_USIC_CH_TBUF_STATUS_SET
{
  XMC_USIC_CH_TBUF_STATUS_SET_BUSY   = 0x1UL, /**< Set Transfer buffer status to busy*/
  XMC_USIC_CH_TBUF_STATUS_SET_IDLE   = 0x2UL  /**< Set Transfer buffer status to idle*/
} XMC_USIC_CH_TBUF_STATUS_SET_t;

/**
* USIC channel receive buffer status
*/
typedef enum XMC_USIC_CH_RBUF_STATUS
{
  XMC_USIC_CH_RBUF_STATUS_DATA_VALID0 = USIC_CH_RBUFSR_RDV0_Msk, /**< RBUF0 data has not yet been read out*/
  XMC_USIC_CH_RBUF_STATUS_DATA_VALID1 = USIC_CH_RBUFSR_RDV1_Msk  /**< RBUF1 data has not yet been read out*/
} XMC_USIC_CH_RBUF_STATUS_t;

/**
 * USIC channel output signal passive data level
*/
typedef enum XMC_USCI_CH_PASSIVE_DATA_LEVEL
{
  XMC_USIC_CH_PASSIVE_DATA_LEVEL0 = 0x0UL, /**< Passive level(idle mode signal level) 0 */
  XMC_USIC_CH_PASSIVE_DATA_LEVEL1 = 0x1UL << USIC_CH_SCTR_PDL_Pos  /**< Passive level(idle mode signal level) 1 */
} XMC_USIC_CH_PASSIVE_DATA_LEVEL_t;

/**
* USIC channel receive FIFO size
*/
typedef enum XMC_USIC_CH_FIFO_SIZE
{
  XMC_USIC_CH_FIFO_DISABLED     = 0x0U,  /**< FIFO Disabled */
  XMC_USIC_CH_FIFO_SIZE_2WORDS  = 0x1U,  /**< FIFO size: 2 words */
  XMC_USIC_CH_FIFO_SIZE_4WORDS  = 0x2U,  /**< FIFO size: 4 words */
  XMC_USIC_CH_FIFO_SIZE_8WORDS  = 0x3U,  /**< FIFO size: 8 words */
  XMC_USIC_CH_FIFO_SIZE_16WORDS = 0x4U,  /**< FIFO size: 16 words */
  XMC_USIC_CH_FIFO_SIZE_32WORDS = 0x5U,  /**< FIFO size: 32 words */
  XMC_USIC_CH_FIFO_SIZE_64WORDS = 0x6U   /**< FIFO size: 64 words */
} XMC_USIC_CH_FIFO_SIZE_t;

/**
* USIC channel transmit FIFO interrupt node pointers
*/
typedef enum XMC_USIC_CH_TXFIFO_INTERRUPT_NODE_POINTER
{
  XMC_USIC_CH_TXFIFO_INTERRUPT_NODE_POINTER_STANDARD  = USIC_CH_TBCTR_STBINP_Pos,  /**< Node pointer for FIFO standard transmit interrupt */
  XMC_USIC_CH_TXFIFO_INTERRUPT_NODE_POINTER_ALTERNATE = USIC_CH_TBCTR_ATBINP_Pos   /**< Node pointer for transmit FIFO error interrupt */
} XMC_USIC_CH_TXFIFO_INTERRUPT_NODE_POINTER_t;

/**
* USIC channel transmit FIFO event configuration
*/
typedef enum XMC_USIC_CH_TXFIFO_EVENT_CONF
{
  XMC_USIC_CH_TXFIFO_EVENT_CONF_STANDARD  = USIC_CH_TBCTR_STBIEN_Msk,   /**< Enable FIFO standard transmit interrupt */
  XMC_USIC_CH_TXFIFO_EVENT_CONF_ERROR     = (int32_t)USIC_CH_TBCTR_TBERIEN_Msk  /**< Enable transmit FIFO error interrupt */
} XMC_USIC_CH_TXFIFO_EVENT_CONF_t;

/**
* USIC channel transmit FIFO status
*/
typedef enum XMC_USIC_CH_TXFIFO_EVENT
{
  XMC_USIC_CH_TXFIFO_EVENT_STANDARD = USIC_CH_TRBSR_STBI_Msk,    /**< Transmit FIFO status: Standard event */
  XMC_USIC_CH_TXFIFO_EVENT_ERROR    = USIC_CH_TRBSR_TBERI_Msk    /**< Transmit FIFO status: Error event */
} XMC_USIC_CH_TXFIFO_EVENT_t;

/**
* USIC channel receive FIFO interrupt node pointers
*/
typedef enum XMC_USIC_CH_RXFIFO_INTERRUPT_NODE_POINTER
{
  XMC_USIC_CH_RXFIFO_INTERRUPT_NODE_POINTER_STANDARD  = USIC_CH_RBCTR_SRBINP_Pos, /**< Node pointer for FIFO standard receive interrupt */
  XMC_USIC_CH_RXFIFO_INTERRUPT_NODE_POINTER_ALTERNATE = USIC_CH_RBCTR_ARBINP_Pos  /**< Node pointer for FIFO alternative receive interrupt */
} XMC_USIC_CH_RXFIFO_INTERRUPT_NODE_POINTER_t;

/**
* USIC channel receive FIFO event configuration
*/
typedef enum XMC_USIC_CH_RXFIFO_EVENT_CONF
{
  XMC_USIC_CH_RXFIFO_EVENT_CONF_STANDARD  = USIC_CH_RBCTR_SRBIEN_Msk,   /**< Enable FIFO standard receive interrupt */
  XMC_USIC_CH_RXFIFO_EVENT_CONF_ERROR     = (int32_t)USIC_CH_RBCTR_RBERIEN_Msk,  /**< Enable receive FIFO error interrupt */
  XMC_USIC_CH_RXFIFO_EVENT_CONF_ALTERNATE = USIC_CH_RBCTR_ARBIEN_Msk   /**< Enable FIFO alternative receive interrupt */
} XMC_USIC_CH_RXFIFO_EVENT_CONF_t;

/**
* USIC channel receive FIFO status
*/
typedef enum XMC_USIC_CH_RXFIFO_EVENT
{
  XMC_USIC_CH_RXFIFO_EVENT_STANDARD  = USIC_CH_TRBSR_SRBI_Msk,   /**< Receive FIFO status: Standard event */
  XMC_USIC_CH_RXFIFO_EVENT_ERROR     = USIC_CH_TRBSR_RBERI_Msk,  /**< Receive FIFO status: Error event */
  XMC_USIC_CH_RXFIFO_EVENT_ALTERNATE = USIC_CH_TRBSR_ARBI_Msk    /**< Receive FIFO status: Alternative event */
} XMC_USIC_CH_RXFIFO_EVENT_t;

/**
* USIC channel baudrate generator clock source
*/
typedef enum XMC_USIC_CH_BRG_CLOCK_SOURCE
{
  XMC_USIC_CH_BRG_CLOCK_SOURCE_DIVIDER = 0x0UL, /**< Baudrate generator clock source : Source divider. (Internal clock source)*/
  XMC_USIC_CH_BRG_CLOCK_SOURCE_DX1T    = 0x2UL << USIC_CH_BRG_CLKSEL_Pos  /**< Baudrate generator clock source : DX1T. (External clock source) */
} XMC_USIC_CH_BRG_CLOCK_SOURCE_t;

/**
* USIC channel baudrate generator divider mode
*/
typedef enum XMC_USIC_CH_BRG_CLOCK_DIVIDER_MODE
{
  XMC_USIC_CH_BRG_CLOCK_DIVIDER_MODE_DISABLED   = 0x0UL, /**< Baudrate generator clock divider: Disabled */
  XMC_USIC_CH_BRG_CLOCK_DIVIDER_MODE_NORMAL     = 0x1UL << USIC_CH_FDR_DM_Pos, /**< Baudrate generator clock divider: Normal mode */
  XMC_USIC_CH_BRG_CLOCK_DIVIDER_MODE_FRACTIONAL = 0x2UL << USIC_CH_FDR_DM_Pos  /**< Baudrate generator clock divider: Fractional mode */
} XMC_USIC_CH_BRG_CLOCK_DIVIDER_MODE_t;

/**
* USIC channel baudrate generator master clock passive level
*/
typedef enum XMC_USIC_CH_BRG_MASTER_CLOCK_PASSIVE_LEVEL
{
  XMC_USIC_CH_BRG_MASTER_CLOCK_PASSIVE_LEVEL_0 = 0x0UL, /**< Baudrate generator master clock passive level(idle mode signal level) 0*/
  XMC_USIC_CH_BRG_MASTER_CLOCK_PASSIVE_LEVEL_1 = 0x1UL << USIC_CH_BRG_MCLKCFG_Pos  /**< Baudrate generator master clock passive level((idle mode signal level)) 1*/
} XMC_USIC_CH_BRG_MASTER_CLOCK_PASSIVE_LEVEL_t;

/**
* USIC channel baudrate generator shift clock passive level
*/
typedef enum XMC_USIC_CH_BRG_SHIFT_CLOCK_PASSIVE_LEVEL
{
  XMC_USIC_CH_BRG_SHIFT_CLOCK_PASSIVE_LEVEL_0_DELAY_DISABLED = 0x0UL, /**< Shift clock passive level 0, delay disabled */
  XMC_USIC_CH_BRG_SHIFT_CLOCK_PASSIVE_LEVEL_1_DELAY_DISABLED = 0x1UL << USIC_CH_BRG_SCLKCFG_Pos, /**< Shift clock passive level 1, delay disabled */
  XMC_USIC_CH_BRG_SHIFT_CLOCK_PASSIVE_LEVEL_0_DELAY_ENABLED  = (int32_t)(0x2UL << USIC_CH_BRG_SCLKCFG_Pos), /**< Shift clock passive level 0, delay enabled */
  XMC_USIC_CH_BRG_SHIFT_CLOCK_PASSIVE_LEVEL_1_DELAY_ENABLED  = (int32_t)(0x3UL << USIC_CH_BRG_SCLKCFG_Pos)  /**< Shift clock passive level 1, delay enabled */
} XMC_USIC_CH_BRG_SHIFT_CLOCK_PASSIVE_LEVEL_t;

/**
* USIC channel baudrate generator shift clock output
*/
typedef enum XMC_USIC_CH_BRG_SHIFT_CLOCK_OUTPUT
{
  XMC_USIC_CH_BRG_SHIFT_CLOCK_OUTPUT_SCLK = 0x0UL, /**< Baudrate generator shift clock output: SCL.(Internally generated shift clock)*/
  XMC_USIC_CH_BRG_SHIFT_CLOCK_OUTPUT_DX1 = 0x1UL << USIC_CH_BRG_SCLKOSEL_Pos   /**< Baudrate generator shift clock output: DX1. (External input shift clock)*/
} XMC_USIC_CH_BRG_SHIFT_CLOCK_OUTPUT_t;

/**
* USIC channel shift direction.
* Defines the shift direction of the data words for transmission and reception
*/
typedef enum XMC_USIC_CH_SHIFT_DIRECTION
{
  XMC_USIC_CH_SHIFT_DIRECTION_LSB_FIRST = 0x0UL, /**< Shift LSB first. The first data bit of a data word is located at bit position 0. */
  XMC_USIC_CH_SHIFT_DIRECTION_MSB_FIRST = 0x1UL << USIC_CH_SCTR_SDIR_Msk /**< Shift MSB first. The first data bit of a data word is located at the bit position given by the configured word length. */ 
} XMC_USIC_CH_SHIFT_DIRECTION_t;


/*******************************************************************************
 * DATA STRUCTURES
 *******************************************************************************/

/*Anonymous structure/union guard start*/
#if defined(__CC_ARM)
  #pragma push
  #pragma anon_unions
#elif defined(__TASKING__)
  #pragma warning 586
#endif

/**
 * USIC module structure
 */
typedef USIC_GLOBAL_TypeDef XMC_USIC_t;

/**
 * USIC channel structure.<br> The members of the structure are same as in the device header file, 
 * except for some registers.
 * DX0CR, DX1CR, DX2CR, DX3CR, DX4CR and DX5CR are replaced with the array DXCR[6].
 * TBUF0 to TBUF31 are replaced with TBUF[32].
 * IN0 to IN31 are replaced with IN[32].
 */
typedef struct XMC_USIC_CH
{
  __I  uint32_t  RESERVED0;
  __I  uint32_t  CCFG;			/**< Channel configuration register*/
  __I  uint32_t  RESERVED1;
  __IO uint32_t  KSCFG;			/**< Kernel state configuration register*/
  __IO uint32_t  FDR;			/**< Fractional divider configuration register*/
  __IO uint32_t  BRG;			/**< Baud rate generator register*/
  __IO uint32_t  INPR;			/**< Interrupt node pointer register*/
  __IO uint32_t  DXCR[6];		/**< Input control registers DX0 to DX5.*/
  __IO uint32_t  SCTR;			/**< Shift control register*/
  __IO uint32_t  TCSR;

  union {
    __IO uint32_t  PCR_IICMode;	/**< I2C protocol configuration register*/
    __IO uint32_t  PCR_IISMode; /**< I2S protocol configuration register*/
    __IO uint32_t  PCR_SSCMode;	/**< SPI protocol configuration register*/
    __IO uint32_t  PCR;			/**< Protocol configuration register*/
    __IO uint32_t  PCR_ASCMode;	/**< UART protocol configuration register*/
  };
  __IO uint32_t  CCR;			/**< Channel control register*/
  __IO uint32_t  CMTR;			/**< Capture mode timer register*/

  union {
    __IO uint32_t  PSR_IICMode;	/**< I2C protocol status register*/
    __IO uint32_t  PSR_IISMode;	/**< I2S protocol status register*/
    __IO uint32_t  PSR_SSCMode;	/**< SPI protocol status register*/
    __IO uint32_t  PSR;			/**< Protocol status register*/
    __IO uint32_t  PSR_ASCMode;	/**< UART protocol status register*/
  };
  __O  uint32_t  PSCR;			/**< Protocol status clear register*/
  __I  uint32_t  RBUFSR;		/**< Receive buffer status register*/
  __I  uint32_t  RBUF;			/**< Receive buffer register*/
  __I  uint32_t  RBUFD;			/**< Debug mode receive buffer register*/
  __I  uint32_t  RBUF0;			/**< Receive buffer 0*/
  __I  uint32_t  RBUF1;			/**< Receive buffer 1*/
  __I  uint32_t  RBUF01SR;		/**< Receive buffer status register*/
  __O  uint32_t  FMR;			/**< Flag modification register*/
  __I  uint32_t  RESERVED2[5];
  __IO uint32_t  TBUF[32];		/**< Tranmsit buffer registers*/
  __IO uint32_t  BYP;			/**< FIFO bypass register*/
  __IO uint32_t  BYPCR;			/**< FIFO bypass control register*/
  __IO uint32_t  TBCTR;			/**< Transmit FIFO control register*/
  __IO uint32_t  RBCTR;			/**< Receive FIFO control register*/
  __I  uint32_t  TRBPTR;		/**< Transmit/recive buffer pointer register*/
  __IO uint32_t  TRBSR;			/**< Transmit/receive buffer status register*/
  __O  uint32_t  TRBSCR;		/**< Transmit/receive buffer status clear register*/
  __I  uint32_t  OUTR;			/**< Receive FIFO output register*/
  __I  uint32_t  OUTDR;			/**< Receive FIFO debug output register*/
  __I  uint32_t  RESERVED3[23];
  __O  uint32_t  IN[32];		/**< Transmit FIFO input register*/
} XMC_USIC_CH_t;


/*Anonymous structure/union guard end*/
#if defined(__CC_ARM)
  #pragma pop
#elif defined(__TASKING__)
  #pragma warning restore
#endif

/*******************************************************************************
 * API PROTOTYPES
 ******************************************************************************/

#ifdef __cplusplus
extern "C" {
#endif

__STATIC_INLINE bool XMC_USIC_IsModuleValid(const XMC_USIC_t *const module)
{
  bool tmp;

  tmp = (module == XMC_USIC0);
#if defined(XMC_USIC1)  
  tmp = tmp || (module == XMC_USIC1);
#endif
#if defined(XMC_USIC2)  
  tmp = tmp || (module == XMC_USIC2);
#endif

  return tmp;
}

__STATIC_INLINE bool XMC_USIC_IsChannelValid(const XMC_USIC_CH_t *const channel)
{
  bool tmp;

  tmp = ((channel == XMC_USIC0_CH0) || (channel == XMC_USIC0_CH1));
#if defined(XMC_USIC1)  
  tmp = tmp || ((channel == XMC_USIC1_CH0) || (channel == XMC_USIC1_CH1));
#endif
#if defined(XMC_USIC2)  
  tmp = tmp || ((channel == XMC_USIC2_CH0) || (channel == XMC_USIC2_CH1));
#endif

  return tmp;
}

/* Common APIs */

/**
 * @param  usic Pointer to USIC module handler of type @ref XMC_USIC_t.\n
 * 				\b Range: @ref XMC_USIC0 to @ref XMC_USIC2 based on device support.
 * @return None
 * 
 * \par<b>Description</b><br>
 * Enables the USIC module.\n\n
 * Enables the clock for the USIC module by following the
 * clock enabling sequence for the selected device.
 *
 * \par<b>Related APIs:</b><BR>
 * XMC_USIC_CH_Enable(), XMC_USIC_Disable() \n\n\n
 */
void XMC_USIC_Enable(XMC_USIC_t *const usic);
/**
 * @param  usic Pointer to USIC module handler of type @ref XMC_USIC_t.\n
 * 				\b Range: @ref XMC_USIC0 to @ref XMC_USIC2 based on device support.
 * @return None
 *
 * \par<b>Description</b><br>
 * Disables the USIC module.\n\n
 * Disables the clock for the USIC module by following the clock
 * disabling sequence for the selected device.
 *
 * \par<b>Related APIs:</b><BR>
 * XMC_USIC_CH_Disable(), XMC_USIC_Enable() \n\n\n
 */
void XMC_USIC_Disable(XMC_USIC_t *const usic);
/**
 * @param  channel Pointer to USIC channel handler of type @ref XMC_USIC_CH_t \n
 * 				   \b Range: @ref XMC_USIC0_CH0, @ref XMC_USIC0_CH1 to @ref XMC_USIC2_CH1 based on device support.
 * @return None
 *
 * \par<b>Description</b><br>
 * Enables the USIC channel. \n\n
 * USIC channel is enabled by setting the module enable bit in KSCFG register bitfield MODEN.
 * On enabling, the channel is set to idle mode.
 *
 * \par<b>Related APIs:</b><BR>
 * XMC_USIC_CH_Disable(), XMC_USIC_Enable() \n\n\n
 */
void XMC_USIC_CH_Enable(XMC_USIC_CH_t *const channel);
/**
 * @param  channel Pointer to USIC channel handler of type @ref XMC_USIC_CH_t \n
 * 				   \b Range: @ref XMC_USIC0_CH0, @ref XMC_USIC0_CH1 to @ref XMC_USIC2_CH1 based on device support.
 * @return None
 *
 * \par<b>Description</b><br>
 * Disables the USIC channel.\n\n
 * USIC channel is disabled by setting the module enable bit(MDEN) to 0 in the register KSCFG.
 *
 * \par<b>Related APIs:</b><BR>
 * XMC_USIC_CH_Enable(), XMC_USIC_Disable() \n\n\n
 */
void XMC_USIC_CH_Disable(XMC_USIC_CH_t *const channel);

/**
 * @param  channel Pointer to USIC channel handler of type @ref XMC_USIC_CH_t \n
 * 				   \b Range: @ref XMC_USIC0_CH0, @ref XMC_USIC0_CH1 to @ref XMC_USIC2_CH1 based on device support.
 * @param  rate Desired baudrate. \b Range: minimum value = 100, maximum value depends on the peripheral clock frequency  \n
 * 				and \a oversampling. Maximum baudrate can be derived using the formula: (fperiph * 1023)/(1024 * oversampling)
 * @param  oversampling Required oversampling. The value indicates the number of time quanta for one symbol of data. \n
 * 						This can be related to the number of samples for each logic state of the data signal. \n
 * 						\b Range: 1 to 32. Value should be chosen based on the protocol used.
 * @return Status indicating the baudrate configuration.\n
 * 			\b Range: @ref XMC_USIC_CH_STATUS_OK if baudrate is successfully configured,
 * 					  @ref XMC_USIC_CH_STATUS_ERROR if desired baudrate or oversampling is invalid.
 *
 * \par<b>Description</b><br>
 * Configures the baudrate of the USIC channel. \n\n
 * Baudrate is configured by considering the peripheral frequency and the desired baudrate.
 * Optimum values of FDR->STEP and BRG->PDIV are calulated and used for generating the desired
 * baudrate.
 *
 * \par<b>Related APIs:</b><BR>
 * XMC_USIC_CH_SetStartTransmisionMode(), XMC_USIC_CH_SetInputSource() \n\n\n
 */
XMC_USIC_CH_STATUS_t XMC_USIC_CH_SetBaudrate(XMC_USIC_CH_t *const channel, uint32_t rate, uint32_t oversampling);

/**
 * @param  channel Pointer to USIC channel handler of type @ref XMC_USIC_CH_t \n
 * 				   \b Range: @ref XMC_USIC0_CH0, @ref XMC_USIC0_CH1 to @ref XMC_USIC2_CH1 based on device support.
 * @param  pdiv Desired divider for the external frequency input. \b Range: minimum value = 1, maximum value = 1024 \n
 * @param  oversampling Required oversampling. The value indicates the number of time quanta for one symbol of data. \n
 * 						This can be related to the number of samples for each logic state of the data signal. \n
 * 						\b Range: 1 to 32. Value should be chosen based on the protocol used.
 * @param  combination_mode Selects which edge of the synchronized(and optionally filtered) signal DXnS actives the trigger
 *                          output DXnT of the input stage.
 *
 * @return None
 *
 * \par<b>Description</b><br>
 * Enables the external frequency input for the Baudrate Generator and configures the divider, oversampling and
 * the combination mode of the USIC channel. \n\n
 *
 * \par<b>Related APIs:</b><BR>
 * XMC_USIC_CH_SetBRGInputClockSource(), XMC_USIC_CH_SetInputTriggerCombinationMode() \n\n\n
 */
void XMC_USIC_CH_ConfigExternalInputSignalToBRG(XMC_USIC_CH_t *const channel,
		                                         const uint16_t pdiv,
												 const uint32_t oversampling,
												 const XMC_USIC_CH_INPUT_COMBINATION_MODE_t combination_mode);

/**
 * @param  channel Pointer to USIC channel handler of type @ref XMC_USIC_CH_t \n
 * 				   \b Range: @ref XMC_USIC0_CH0, @ref XMC_USIC0_CH1 to @ref XMC_USIC2_CH1 based on device support.
 * @param  input USIC channel input stage of type @ref XMC_USIC_CH_INPUT_t. \n
 * 				 \b Range: @ref XMC_USIC_CH_INPUT_DX0 to @ref XMC_USIC_CH_INPUT_DX5
 * @param  source Input source select for the input stage. The table below maps the enum value with the input channel.
 * <table><tr><td>0</td><td>DXnA</td></tr><tr><td>1</td><td>DXnB</td></tr><tr><td>2</td><td>DXnC</td></tr><tr><td>3</td><td>DXnD</td></tr>
 * <tr><td>4</td><td>DXnE</td></tr><tr><td>5</td><td>DXnF</td></tr><tr><td>6</td><td>DXnG</td></tr><tr><td>7</td><td>Always 1</td>
 * </tr></table>
 * @return None
 *
 * \par<b>Description</b><br>
 * Selects the data source for USIC input stage.\n\n
 * Selects the input data signal source among DXnA, DXnB.. DXnG for the input stage. The API can be used for all the input stages 
 * like DX0CR, DX1CR etc. 
 *  
 * \par<b>Related APIs:</b><BR>
 * XMC_USIC_CH_EnableInputInversion(), XMC_USIC_CH_EnableInputDigitalFilter(), XMC_USIC_CH_EnableInputSync(),
 * XMC_USIC_CH_SetInputSamplingFreq()\n\n\n
 */
__STATIC_INLINE void XMC_USIC_CH_SetInputSource(XMC_USIC_CH_t *const channel, const XMC_USIC_CH_INPUT_t input, const uint8_t source)
{
  channel->DXCR[input] = (uint32_t)((channel->DXCR[input] & (uint32_t)(~USIC_CH_DXCR_DSEL_Msk)) |
                         ((uint32_t)source << USIC_CH_DXCR_DSEL_Pos));
}
/**
 * @param  channel Pointer to USIC channel handler of type @ref XMC_USIC_CH_t \n
 * 				   \b Range: @ref XMC_USIC0_CH0, @ref XMC_USIC0_CH1 to @ref XMC_USIC2_CH1 based on device support.
 * @param  input USIC channel input stage of type @ref XMC_USIC_CH_INPUT_t. \n
 * 				 \b Range: @ref XMC_USIC_CH_INPUT_DX0 to @ref XMC_USIC_CH_INPUT_DX5
 * @return None
 *
 * \par<b>Description</b><br>
 * The input of the data shift unit is controlled by the
 * protocol pre-processor. \n\n
 *
 * \par<b>Related APIs:</b><BR>
 * XMC_USIC_CH_ConnectInputDataShiftToDataInput()\n\n\n
 */
__STATIC_INLINE void XMC_USIC_CH_ConnectInputDataShiftToPPP(XMC_USIC_CH_t *const channel, const XMC_USIC_CH_INPUT_t input)
{
  channel->DXCR[input] &=(uint32_t)~USIC_CH_DXCR_INSW_Msk;
}

/**
 * @param  channel Pointer to USIC channel handler of type @ref XMC_USIC_CH_t \n
 * 				   \b Range: @ref XMC_USIC0_CH0, @ref XMC_USIC0_CH1 to @ref XMC_USIC2_CH1 based on device support.
 * @param  input USIC channel input stage of type @ref XMC_USIC_CH_INPUT_t. \n
 * 				 \b Range: @ref XMC_USIC_CH_INPUT_DX0 to @ref XMC_USIC_CH_INPUT_DX5
 * @return None
 *
 * \par<b>Description</b><br>
 * The input of the data shift unit is connected to
 * the selected data input line. \n\n
 *
 * This setting is used
 * if the signals are directly derived from an input
 * pin without treatment by the protocol preprocessor.
 * \par<b>Related APIs:</b><BR>
 * XMC_USIC_CH_ConnectInputDataShiftToPPP()\n\n\n
 */
__STATIC_INLINE void XMC_USIC_CH_ConnectInputDataShiftToDataInput(XMC_USIC_CH_t *const channel, const XMC_USIC_CH_INPUT_t input)
{
  channel->DXCR[input] |= USIC_CH_DXCR_INSW_Msk;
}

/**
 * @param  channel Pointer to USIC channel handler of type @ref XMC_USIC_CH_t \n
 * 				   \b Range: @ref XMC_USIC0_CH0, @ref XMC_USIC0_CH1 to @ref XMC_USIC2_CH1 based on device support.
 * @param  input USIC channel input stage of type @ref XMC_USIC_CH_INPUT_t. \n
 * 				 \b Range: @ref XMC_USIC_CH_INPUT_DX0 to @ref XMC_USIC_CH_INPUT_DX5
 * @return None
 *
 * \par<b>Description</b><br>
 * Enables input inversion for USIC channel input data signal. \n\n
 *
 * Polarity of the input source can be changed to provide inverted data input.
 * \par<b>Related APIs:</b><BR>
 * XMC_USIC_CH_DisableInputInversion()\n\n\n
 */
__STATIC_INLINE void XMC_USIC_CH_EnableInputInversion(XMC_USIC_CH_t *const channel, const XMC_USIC_CH_INPUT_t input)
{
  channel->DXCR[input] |= USIC_CH_DXCR_DPOL_Msk;
}

/**
 * @param  channel Pointer to USIC channel handler of type @ref XMC_USIC_CH_t \n
 * 				   \b Range: @ref XMC_USIC0_CH0, @ref XMC_USIC0_CH1 to @ref XMC_USIC2_CH1 based on device support.
 * @param  input USIC channel input stage of type @ref XMC_USIC_CH_INPUT_t. \n
 * 				 \b Range: @ref XMC_USIC_CH_INPUT_DX0 to @ref XMC_USIC_CH_INPUT_DX5
 * @return None
 *
 * \par<b>Description</b><br>
 * Disables input inversion for USIC channel. \n\n
 *
 * Resets the input data polarity for the USIC channel input data signal.
 * \par<b>Related APIs:</b><BR>
 * XMC_USIC_CH_EnableInputInversion()\n\n\n
 */
__STATIC_INLINE void XMC_USIC_CH_DisableInputInversion(XMC_USIC_CH_t *const channel, const XMC_USIC_CH_INPUT_t input)
{
  channel->DXCR[input] &=(uint32_t)~USIC_CH_DXCR_DPOL_Msk;
}

/**
 * @param  channel Pointer to USIC channel handler of type @ref XMC_USIC_CH_t \n
 * 				   \b Range: @ref XMC_USIC0_CH0, @ref XMC_USIC0_CH1 to @ref XMC_USIC2_CH1 based on device support.
 * @return None
 *
 * \par<b>Description</b><br>
 * Enables delay compensation. \n\n
 *
 * Delay compensation can be applied to the receive path.
 * \par<b>Related APIs:</b><BR>
 * XMC_USIC_CH_DisableDelayCompensation()\n\n\n
 */
__STATIC_INLINE void XMC_USIC_CH_EnableDelayCompensation(XMC_USIC_CH_t *const channel)
{
  channel->DXCR[1U] |= USIC_CH_DX1CR_DCEN_Msk;
}

/**
 * @param  channel Pointer to USIC channel handler of type @ref XMC_USIC_CH_t \n
 * 				   \b Range: @ref XMC_USIC0_CH0, @ref XMC_USIC0_CH1 to @ref XMC_USIC2_CH1 based on device support.
 * @return None
 *
 * \par<b>Description</b><br>
 * Disables delay compensation.. \n\n
 *
 * \par<b>Related APIs:</b><BR>
 * XMC_USIC_CH_EnableDelayCompensation()\n\n\n
 */
__STATIC_INLINE void XMC_USIC_CH_DisableDelayCompensation(XMC_USIC_CH_t *const channel)
{
  channel->DXCR[1U] &=(uint32_t)~USIC_CH_DX1CR_DCEN_Msk;
}

/**
 * @param  channel Pointer to USIC channel handler of type @ref XMC_USIC_CH_t \n
 * 				   \b Range: @ref XMC_USIC0_CH0, @ref XMC_USIC0_CH1 to @ref XMC_USIC2_CH1 based on device support.
 * @param  input USIC channel input stage of type @ref XMC_USIC_CH_INPUT_t. \n
 * 				 \b Range: @ref XMC_USIC_CH_INPUT_DX0 to @ref XMC_USIC_CH_INPUT_DX5
 * @return None
 *
 * \par<b>Description</b><br>
 * Enables the input digital filter for USIC channel input data signal. \n\n
 * Input data signal from the selected multiplexer will be digitally filtered.
 *
 * \par<b>Related APIs:</b><BR>
 * XMC_USIC_CH_DisableInputDigitalFilter()\n\n\n
 */
__STATIC_INLINE void XMC_USIC_CH_EnableInputDigitalFilter(XMC_USIC_CH_t *const channel, const XMC_USIC_CH_INPUT_t input)
{
  channel->DXCR[input] |= (uint32_t)USIC_CH_DXCR_DFEN_Msk;
}
/**
 * @param  channel Pointer to USIC channel handler of type @ref XMC_USIC_CH_t \n
 * 				   \b Range: @ref XMC_USIC0_CH0, @ref XMC_USIC0_CH1 to @ref XMC_USIC2_CH1 based on device support.
 * @param  input USIC channel input stage of type @ref XMC_USIC_CH_INPUT_t. \n
 * 				 \b Range: @ref XMC_USIC_CH_INPUT_DX0 to @ref XMC_USIC_CH_INPUT_DX5
 * @return None
 *
 * \par<b>Description</b><br>
 * Disables the input digital filter for USIC channel input data signal. \n\n
 * Input data signal from the selected multiplexer will not be digitally filtered.
 *
 * \par<b>Related APIs:</b><BR>
 * XMC_USIC_CH_EnableInputDigitalFilter()\n\n\n
 */
__STATIC_INLINE void XMC_USIC_CH_DisableInputDigitalFilter(XMC_USIC_CH_t *const channel, const XMC_USIC_CH_INPUT_t input)
{
  channel->DXCR[input] &=(uint32_t)~USIC_CH_DXCR_DFEN_Msk;
}
/**
 * @param  channel Pointer to USIC channel handler of type @ref XMC_USIC_CH_t \n
 * 				   \b Range: @ref XMC_USIC0_CH0, @ref XMC_USIC0_CH1 to @ref XMC_USIC2_CH1 based on device support.
 * @param  input USIC channel input stage of type @ref XMC_USIC_CH_INPUT_t. \n
 * 				 \b Range: @ref XMC_USIC_CH_INPUT_DX0 to @ref XMC_USIC_CH_INPUT_DX5
 * @return None
 *
 * \par<b>Description</b><br>
 * Enables input synchronization for the USIC channel input data signal. \n\n
 * Input data signal from the selected multiplexer will be synchronized with fPERIPH.
 * A noisy signal can be synchronized and filtered by enabling the digital filter.
 *
 * \par<b>Related APIs:</b><BR>
 * XMC_USIC_CH_DisableInputSync(), XMC_USIC_CH_EnableInputDigitalFilter()\n\n\n
 */
__STATIC_INLINE void XMC_USIC_CH_EnableInputSync(XMC_USIC_CH_t *const channel, const XMC_USIC_CH_INPUT_t input)
{
  channel->DXCR[input] |=(uint32_t)USIC_CH_DXCR_DSEN_Msk;
}
/**
 * @param  channel Pointer to USIC channel handler of type @ref XMC_USIC_CH_t \n
 * 				   \b Range: @ref XMC_USIC0_CH0, @ref XMC_USIC0_CH1 to @ref XMC_USIC2_CH1 based on device support.
 * @param  input USIC channel input stage of type @ref XMC_USIC_CH_INPUT_t. \n
 * 				 \b Range: @ref XMC_USIC_CH_INPUT_DX0 to @ref XMC_USIC_CH_INPUT_DX5
 * @return None
 *
 * \par<b>Description</b><br>
 * Disables input synchronization for the USIC channel input data signal. \n\n
 * Input data signal from the selected multiplexer will not be synchronized.
 *
 * \par<b>Related APIs:</b><BR>
 * XMC_USIC_CH_EnableInputSync(), XMC_USIC_CH_DisableInputDigitalFilter() \n\n\n
 */
__STATIC_INLINE void XMC_USIC_CH_DisableInputSync(XMC_USIC_CH_t *const channel, const XMC_USIC_CH_INPUT_t input)
{
  channel->DXCR[input] &=(uint32_t)~USIC_CH_DXCR_DSEN_Msk;
}
/**
 * @param  channel Pointer to USIC channel handler of type @ref XMC_USIC_CH_t \n
 * 				   \b Range: @ref XMC_USIC0_CH0, @ref XMC_USIC0_CH1 to @ref XMC_USIC2_CH1 based on device support..
 * @param  input USIC channel input stage of type @ref XMC_USIC_CH_INPUT_t. \n
 * 				 \b Range: @ref XMC_USIC_CH_INPUT_DX0 to @ref XMC_USIC_CH_INPUT_DX5
 * @param  sampling_freq Sampling frequency value of type \a XMC_USIC_CH_INPUT_SAMPLING_FREQ_t.
 * @return None
 *
 * \par<b>Description</b><br>
 * Sets sampling frequency for USIC channel input data signal. \n\n
 *
 * \par<b>Related APIs:</b><BR>
 * XMC_USIC_CH_SetInputSource(), XMC_USIC_CH_EnableInputSync(), XMC_USIC_CH_EnableInputDigitalFilter() \n\n\n
 */
__STATIC_INLINE void XMC_USIC_CH_SetInputSamplingFreq(XMC_USIC_CH_t *const channel,
		                                              const XMC_USIC_CH_INPUT_t input,
		                                              const XMC_USIC_CH_INPUT_SAMPLING_FREQ_t sampling_freq)
{
  channel->DXCR[input] = (uint32_t)(channel->DXCR[input] & (~USIC_CH_DXCR_SFSEL_Msk)) |
                         ((uint32_t)sampling_freq);
}

/**
 * @param  channel Pointer to USIC channel handler of type @ref XMC_USIC_CH_t \n
 * 				   \b Range: @ref XMC_USIC0_CH0, @ref XMC_USIC0_CH1 to @ref XMC_USIC2_CH1 based on device support..
 * @param  input USIC channel input stage of type @ref XMC_USIC_CH_INPUT_t. \n
 * 				 \b Range: @ref XMC_USIC_CH_INPUT_DX0 to @ref XMC_USIC_CH_INPUT_DX5
 * @param  combination_mode Combination mode value of type \a XMC_USIC_CH_INPUT_COMBINATION_MODE_t.
 * @return None
 *
 * \par<b>Description</b><br>
 * Selects which edge of the synchronized signal DXnS activates the trigger output DXnT of the input stage. \n\n
 *
 * \par<b>Related APIs:</b><BR>
 * XMC_USIC_CH_SetInputSource(), XMC_USIC_CH_EnableInputSync(), XMC_USIC_CH_EnableInputDigitalFilter() \n\n\n
 */
__STATIC_INLINE void XMC_USIC_CH_SetInputTriggerCombinationMode(XMC_USIC_CH_t *const channel,
		                                                        const XMC_USIC_CH_INPUT_t input,
		                                                        const XMC_USIC_CH_INPUT_COMBINATION_MODE_t combination_mode)
{
  channel->DXCR[input] = (uint32_t)(channel->DXCR[input] & (~USIC_CH_DXCR_CM_Msk)) |
                         ((uint32_t)combination_mode << USIC_CH_DXCR_CM_Pos);
}

/**
 * @param  channel Pointer to USIC channel handler of type @ref XMC_USIC_CH_t \n
 * 				   \b Range: @ref XMC_USIC0_CH0, @ref XMC_USIC0_CH1 to @ref XMC_USIC2_CH1 based on device support.
 * @param  clock_source clock source for the BRG.
 * @return None
 *
 * \par<b>Description</b><br>
 * Sets the clock source for the BRG. \n\n
 *
 * \par<b>Related APIs:</b><BR>
 * XMC_USIC_CH_SetInputTriggerCombinationMode(), XMC_USIC_CH_SetExternalClockBRGDivider()\n\n\n
 */
__STATIC_INLINE void XMC_USIC_CH_SetBRGInputClockSource(XMC_USIC_CH_t *const channel, const XMC_USIC_CH_BRG_CLOCK_SOURCE_t clock_source)
{
  channel->BRG = (uint32_t)(channel->BRG & (~USIC_CH_BRG_CLKSEL_Msk)) | (uint32_t)(clock_source);
}

/**
 * @param channel Pointer to USIC channel handler of type @ref XMC_USIC_CH_t \n
 * 				   \b Range: @ref XMC_USIC0_CH0, @ref XMC_USIC0_CH1 to @ref XMC_USIC2_CH1 based on device support.
 * @param data Data to be transmitted. \n
 *           \b Range: 16bit unsigned data. minimum= 0, maximum= 65535
 * @return None
 *
 * \par<b>Description</b><br>
 * Writes data into the transmit buffer. \n\n
 * The data provided is placed in TBUF[0U].
 *
 *
 * \par<b>Related APIs:</b><BR>
 * XMC_USIC_CH_WriteToTBUFTCI() \n\n\n
 */
__STATIC_INLINE void XMC_USIC_CH_WriteToTBUF(XMC_USIC_CH_t *const channel, const uint16_t data)
{
  channel->TBUF[0U] = data;
}

/**
 * @param channel Pointer to USIC channel handler of type @ref XMC_USIC_CH_t \n
 * 				   \b Range: @ref XMC_USIC0_CH0, @ref XMC_USIC0_CH1 to @ref XMC_USIC2_CH1 based on device support.
 * @param data Data to be transmitted.
 * @param transmit_control_information transmit control information to be configured while transmitting the data. \n
 * 			\b Range: minimum= 0, maximum= 31.
 * @return None
 *
 * \par<b>Description</b><br>
 * Writes data to the transmit buffer in a control mode. \n\n
 * When the respective control mode is enabled , this API can be used.
 *
 *
 * \par<b>Related APIs:</b><BR>
 * XMC_USIC_CH_WriteToTBUF() \n\n\n
 */
__STATIC_INLINE void XMC_USIC_CH_WriteToTBUFTCI(XMC_USIC_CH_t *const channel,
                                             const uint16_t data,
                                             const uint32_t transmit_control_information)
{
  channel->TBUF[transmit_control_information] = data;
}

/**
 * @param  channel Pointer to USIC channel handler of type @ref XMC_USIC_CH_t \n
 * 				   \b Range: @ref XMC_USIC0_CH0, @ref XMC_USIC0_CH1 to @ref XMC_USIC2_CH1 based on device support.
 * @param  word_length Number of bits to be configured for a data word. \n
 * 					   \b Range: minimum= 1, maximum= 16. \n
 * 					   e.g: For word length of 8, \a word_length should be provided as 8.
 * @return None
 *
 * \par<b>Description</b><br>
 * Sets the data word length in number of bits. \n\n
 * Sets the number of bits to represent a data word. Frame length should be a multiple of word length.
 *
 * \par<b>Related APIs:</b><BR>
 * XMC_USIC_CH_SetFrameLength()\n\n\n
 */
__STATIC_INLINE void XMC_USIC_CH_SetWordLength(XMC_USIC_CH_t *const channel, const uint8_t word_length)
{
  channel->SCTR = (uint32_t)(channel->SCTR & (~USIC_CH_SCTR_WLE_Msk)) |
                  (uint32_t)(((uint32_t)word_length - 1UL)  << USIC_CH_SCTR_WLE_Pos);
}

/**
 * @param  channel Pointer to USIC channel handler of type @ref XMC_USIC_CH_t \n
 *           \b Range: @ref XMC_USIC0_CH0, @ref XMC_USIC0_CH1 to @ref XMC_USIC2_CH1 based on device support.
 * @param  word_length Number of bits to be configured for a data word. \n
 *             \b Range: minimum= 1, maximum= 16. \n
 *             e.g: For word length of 8, \a word_length should be provided as 8.
 * @return None
 *
 * \par<b>Description</b><br>
 * Sets the data word length in number of bits. \n\n
 * Sets the number of bits to represent a data word. Frame length should be a multiple of word length.
 *
 * \par<b>Related APIs:</b><BR>
 * XMC_USIC_CH_SetFrameLength()\n\n\n
 */
__STATIC_INLINE void XMC_USIC_CH_SetShiftDirection(XMC_USIC_CH_t *const channel, const XMC_USIC_CH_SHIFT_DIRECTION_t shift_direction)
{
  channel->SCTR = (uint32_t)(channel->SCTR & (~USIC_CH_SCTR_SDIR_Msk)) | (uint32_t)shift_direction;
}


/**
 * @param  channel Pointer to USIC channel handler of type @ref XMC_USIC_CH_t \n
 *           \b Range: @ref XMC_USIC0_CH0, @ref XMC_USIC0_CH1 to @ref XMC_USIC2_CH1 based on device support.
 * @return Captured counter value 
 *
 *
 * \par<b>Description</b><br>
 * The value of the counter is captured if one of the trigger signals DX0T or DX1T are activated by the corresponding input stage.
 */
__STATIC_INLINE uint32_t XMC_USIC_CH_GetCaptureTimerValue(const XMC_USIC_CH_t *const channel)
{
  return channel->CMTR;
}

/**
 * @param  channel Pointer to USIC channel handler of type @ref XMC_USIC_CH_t \n
 *           \b Range: @ref XMC_USIC0_CH0, @ref XMC_USIC0_CH1 to @ref XMC_USIC2_CH1 based on device support.
 * @param  mode divider mode ::XMC_USIC_CH_BRG_CLOCK_DIVIDER_MODE_t \n
 * @param  step divider \n
 *             \b XMC_USIC_CH_BRG_CLOCK_DIVIDER_MODE_NORMAL resulting divider = 1023 - step \n
 *             \b XMC_USIC_CH_BRG_CLOCK_DIVIDER_MODE_FRACTIONAL resulting divider = 1023 / step \n
 * 
 * @return None
 * 
 * \par<b>Description</b><br>
 * The fractional divider generates its output frequency fFD by either dividing the input frequency fPERIPH by an integer factor n or by multiplication of n/1024.
 *
 */
__STATIC_INLINE void XMC_USIC_CH_SetFractionalDivider(XMC_USIC_CH_t *const channel, const XMC_USIC_CH_BRG_CLOCK_DIVIDER_MODE_t mode, const uint16_t step)
{
  channel->FDR = mode | step;
}

/**
 * @param  channel Pointer to USIC channel handler of type @ref XMC_USIC_CH_t \n
 * 				   \b Range: @ref XMC_USIC0_CH0, @ref XMC_USIC0_CH1 to @ref XMC_USIC2_CH1 based on device support.
 * @param  frame_length Number of bits in a frame. \n
 * 						\b Range: minimum= 1, maximum= 0x3f. The maximum value for fixed frame size is 0x3f. \n
 * 						e.g: For a frame length of 16, \a frame_length should be provided as 16.
 * @return None
 *
 * \par<b>Description</b><br>
 * Define the data frame length.\n\n
 * Set the number of bits to be serially transmitted in a frame.
 * The frame length should be multiples of word length. If the value is set to 0x40, the frame length
 * has to be controlled explicitly. 
 *
 * \par<b>Related APIs:</b><BR>
 * XMC_USIC_CH_SetWordLength(), XMC_USIC_CH_EnableFrameLengthControl() \n\n\n
 */
__STATIC_INLINE void XMC_USIC_CH_SetFrameLength(XMC_USIC_CH_t *const channel, const uint8_t frame_length)
{
  channel->SCTR = (uint32_t)(channel->SCTR & (~USIC_CH_SCTR_FLE_Msk)) |
                  (((uint32_t)frame_length - 0x1U)  << USIC_CH_SCTR_FLE_Pos);
}

/**
 * @param channel Pointer to USIC channel handler of type @ref XMC_USIC_CH_t \n
 * 				   \b Range: @ref XMC_USIC0_CH0, @ref XMC_USIC0_CH1 to @ref XMC_USIC2_CH1 based on device support.
 * @param event Bit mask of the channel events to be enabled. Use @ref XMC_USIC_CH_EVENT_t for the bit masks. \n
 * 				\b Range: @ref XMC_USIC_CH_EVENT_RECEIVE_START, @ref XMC_USIC_CH_EVENT_DATA_LOST etc. Multiple events
 * 				can be combined using \a OR operation.
 * @return None
 *
 * \par<b>Description</b><br>
 * Enable the channel interrupt events.\n\n
 * Common channel events related to serial communication can be configured using this API.
 * Multiple events can be combined using the bitwise OR operation and configured in one function call.
 * XMC_USIC_CH_EVENT_t enumerates multiple protocol event bitmasks. These enumerations can be used as input to the API.
 *
 * \par<b>Related APIs:</b><BR>
 * XMC_USIC_CH_DisableEvent(), XMC_USIC_CH_SetInterruptNodePointer() \n\n\n
 */
__STATIC_INLINE void XMC_USIC_CH_EnableEvent(XMC_USIC_CH_t *const channel, const uint32_t event)
{
  channel->CCR |= event;
}

/**
 * @param channel Pointer to USIC channel handler of type @ref XMC_USIC_CH_t \n
 * 				  \b Range: @ref XMC_USIC0_CH0, @ref XMC_USIC0_CH1 to @ref XMC_USIC2_CH1 based on device support.
 * @param event Bit mask of the channel events to be disabled. Use @ref XMC_USIC_CH_EVENT_t for the bit masks. \n
 * 				\b Range: @ref XMC_USIC_CH_EVENT_RECEIVE_START, @ref XMC_USIC_CH_EVENT_DATA_LOST etc. Multiple events
 * 				can be combined using \a OR operation.
 * @return None
 *
 * \par<b>Description</b><br>
 * Disable the channel interrupt events. \n\n
 * Multiple events can be combined using the bitwise OR operation and configured in one function call.
 * XMC_USIC_CH_EVENT_t enumerates multiple protocol event bitmasks. These enumerations can be used as input to the API.
 *
 * \par<b>Related APIs:</b><BR>
 * XMC_USIC_CH_EnableEvent(), XMC_USIC_CH_SetInterruptNodePointer() \n\n\n
*/
__STATIC_INLINE void XMC_USIC_CH_DisableEvent(XMC_USIC_CH_t *const channel, const uint32_t event)
{
  channel->CCR &= (uint32_t)~event;
}

/**
 * @param channel Pointer to USIC channel handler of type @ref XMC_USIC_CH_t \n
 * 				  \b Range: @ref XMC_USIC0_CH0, @ref XMC_USIC0_CH1 to @ref XMC_USIC2_CH1 based on device support.
 * @param  interrupt_node Interrupt node pointer to be configured. \n
 * 						  \b Range: @ref XMC_USIC_CH_INTERRUPT_NODE_POINTER_TRANSMIT_SHIFT,
 * 						  			@ref XMC_USIC_CH_INTERRUPT_NODE_POINTER_TRANSMIT_BUFFER etc.
 * @param service_request Service request number.\n
 * 						  \b Range: 0 to 5.
 * @return None
 *
 * \par<b>Description</b><br>
 * Sets the interrupt node for USIC channel events. \n\n
 * For an event to generate interrupt, node pointer should be configured with service request(SR0, SR1..SR5). 
 * The NVIC node gets linked to the interrupt event by doing so.<br>
 * Note: NVIC node should be separately enabled to generate the interrupt.
 *
 * \par<b>Related APIs:</b><BR>
 * XMC_USIC_CH_EnableEvent() \n\n\n
 */
void XMC_USIC_CH_SetInterruptNodePointer(XMC_USIC_CH_t *const channel,
                                         const XMC_USIC_CH_INTERRUPT_NODE_POINTER_t interrupt_node,
                                         const uint32_t service_request);

/**
 * @param  channel Pointer to USIC channel handler of type @ref XMC_USIC_CH_t \n
 * 				   \b Range: @ref XMC_USIC0_CH0, @ref XMC_USIC0_CH1 to @ref XMC_USIC2_CH1 based on device support.
 * @return Stataus @ref XMC_USIC_CH_TBUF_STATUS_IDLE if transmit buffer is free,
 *                 @ref XMC_USIC_CH_TBUF_STATUS_BUSY if transmit buffer is busy.
 *
 * \par<b>Description</b><br>
 * Gets transmit buffer status. \n\n
 * Status indicates whether the transmit buffer is free, or busy transmitting data.
 * The status depends on the value of TDV flag in TCSR register.
 * This status can be used while transmitting data. Transmit data when the transmit buffer
 * status is @ref XMC_USIC_CH_TBUF_STATUS_IDLE.
 *
 * \par<b>Related APIs:</b><BR>
 * XMC_USIC_CH_SetDataOutputMode() \n\n\n
 */
__STATIC_INLINE XMC_USIC_CH_TBUF_STATUS_t XMC_USIC_CH_GetTransmitBufferStatus(XMC_USIC_CH_t *const channel)
{
  return (XMC_USIC_CH_TBUF_STATUS_t)(channel->TCSR & USIC_CH_TCSR_TDV_Msk);
}

/**
 * @brief API to get receive buffer status
 * @param  channel Pointer to USIC channel handler of type @ref XMC_USIC_CH_t \n
 * 				   \b Range: @ref XMC_USIC0_CH0, @ref XMC_USIC0_CH1 to @ref XMC_USIC2_CH1 based on device support.
 * @return Status of data validity check for RBUF0 and RBUF1. \n
 * 		   Returned value should be masked with RDV0 and RDV1 bits to know the status. \n
 * 		   \b Range: @ref XMC_USIC_CH_RBUF_STATUS_DATA_VALID0, @ref XMC_USIC_CH_RBUF_STATUS_DATA_VALID1.
 *
 * \par<b>Description</b><br>
 * Checks if RBUF0 and RBUF1 have valid unread data. \n\n
 * It checks the bits RDV0 and RDV1 of the RBUFSR register.
 * Returns the value of RBUFSR masked with bitmasks of RDV0 and RDV1.
 * It can be used to decide whether 2bytes has to be read from RBUF or 1 byte. 
 * If both bitmasks XMC_USIC_CH_RBUF_STATUS_DATA_VALID0 and XMC_USIC_CH_RBUF_STATUS_DATA_VALID1
 * are set, then 2 bytes can be read from RBUF. If only either of them is set, then only one byte
 * can be read from RBUF.
 * \par<b>Related APIs:</b><BR>
 * XMC_USIC_CH_GetTransmitBufferStatus() \n\n\n
 */
__STATIC_INLINE uint32_t XMC_USIC_CH_GetReceiveBufferStatus(XMC_USIC_CH_t *const channel)
{
  return ((uint32_t) (channel->RBUFSR & (USIC_CH_RBUFSR_RDV0_Msk | USIC_CH_RBUFSR_RDV1_Msk)));
}


/**
 * @param  channel Pointer to USIC channel handler of type @ref XMC_USIC_CH_t \n
 * 				   \b Range: @ref XMC_USIC0_CH0, @ref XMC_USIC0_CH1 to @ref XMC_USIC2_CH1 based on device support.
 * @param  start_transmision_mode Transmission mode to be enabled. \n
 * 								\b Range: @ref XMC_USIC_CH_START_TRANSMISION_DISABLED,
 * 								@ref XMC_USIC_CH_START_TRANSMISION_ON_TDV, @ref XMC_USIC_CH_START_TRANSMISION_ON_TDV_DX2S_0,
 * 								@ref XMC_USIC_CH_START_TRANSMISION_ON_TDV_DX2S_1
 *                              
 * @return None
 *
 * \par<b>Description</b><br>
 * Configures data transmission. \n\n
 * The configuration affects the data shifted on the DOUT0 pin.
 * 
 * \par<b>Related APIs:</b><BR>
 * XMC_USIC_CH_GetTransmitBufferStatus(), XMC_USIC_CH_SetDataOutputMode() \n\n\n
 */
__STATIC_INLINE void XMC_USIC_CH_SetStartTransmisionMode(XMC_USIC_CH_t *const channel,
                                                         const XMC_USIC_CH_START_TRANSMISION_MODE_t start_transmision_mode)
{
  channel->TCSR = (uint32_t)(channel->TCSR & (~USIC_CH_TCSR_TDEN_Msk)) | (uint32_t)start_transmision_mode;
}


/**
 * @param  channel Pointer to USIC channel handler of type @ref XMC_USIC_CH_t \n
 * 				   \b Range: @ref XMC_USIC0_CH0, @ref XMC_USIC0_CH1 to @ref XMC_USIC2_CH1 based on device support.
 * @param  data_output_mode  Data output mode. \n
 * 			\b Range: @ref XMC_USIC_CH_DATA_OUTPUT_MODE_NORMAL, @ref XMC_USIC_CH_DATA_OUTPUT_MODE_INVERTED
 * @return None
 *
 * \par<b>Description</b><br>
 * Configures the mode for data output. \n\n
 * USIC channel can be configured to shift inverted data or direct data based on the input to the API.
 * 
 * \par<b>Related APIs:</b><BR>
 * XMC_USIC_CH_SetStartTransmisionMode() \n\n\n
 */
__STATIC_INLINE void XMC_USIC_CH_SetDataOutputMode(XMC_USIC_CH_t *const channel,
                                                   const XMC_USIC_CH_DATA_OUTPUT_MODE_t data_output_mode)
{
  channel->SCTR = (uint32_t)(channel->SCTR & (~USIC_CH_SCTR_DOCFG_Msk)) | (uint32_t)data_output_mode;
}

/**
 * @param  channel Pointer to USIC channel handler of type @ref XMC_USIC_CH_t \n
 * 				   \b Range: @ref XMC_USIC0_CH0, @ref XMC_USIC0_CH1 to @ref XMC_USIC2_CH1 based on device support.
 * @return None
 *
 * \par<b>Description</b><br>
 * Enables automatic update of frame length. \n\n
 * When the automatic update of frame length is enabled, frame length is configured based on the 
 * index of the TBUF[]/IN[] register array. When the data is written to TBUF[x], frame length is configured
 * with the mask value of \a x at the last 5 bit positions. Same logic is applicable if data is written to
 * IN[x] register.
 *
 * \par<b>Related APIs:</b><BR>
 * XMC_USIC_CH_DisableFrameLengthControl(), XMC_USIC_CH_TXFIFO_PutDataFLEMode() \n\n\n
 */
__STATIC_INLINE void XMC_USIC_CH_EnableFrameLengthControl(XMC_USIC_CH_t *const channel)
{
  channel->TCSR = (uint32_t)(channel->TCSR & (~(USIC_CH_TCSR_WLEMD_Msk |
                                         USIC_CH_TCSR_SELMD_Msk |
                                         USIC_CH_TCSR_WAMD_Msk |
                                         USIC_CH_TCSR_HPCMD_Msk))) |
                                         (uint32_t)USIC_CH_TCSR_FLEMD_Msk;
}

/**
 * @param  channel Pointer to USIC channel handler of type @ref XMC_USIC_CH_t \n
 * 				   \b Range: @ref XMC_USIC0_CH0, @ref XMC_USIC0_CH1 to @ref XMC_USIC2_CH1 based on device support.
 * @return None
 *
 * \par<b>Description</b><br>
 * Disables automatic update of frame length. \n\n
 * When automatic update of frame length is disabled, frame length has to configured explicitly.
 * Frame length remains fixed until it is changed again.
 *
 * \par<b>Related APIs:</b><BR>
 * XMC_USIC_CH_DisableFrameLengthControl(), XMC_USIC_CH_SetFrameLength() \n\n\n
 */
__STATIC_INLINE void XMC_USIC_CH_DisableFrameLengthControl(XMC_USIC_CH_t *const channel)
{
  channel->TCSR &= (uint32_t)~USIC_CH_TCSR_FLEMD_Msk;
}

/**
 * @param  channel Pointer to USIC channel handler of type @ref XMC_USIC_CH_t \n
 * 				   \b Range: @ref XMC_USIC0_CH0, @ref XMC_USIC0_CH1 to @ref XMC_USIC2_CH1 based on device support.
 * @return None
 *
 * \par<b>Description</b><br>
 * Bit TCSR.TE is set if DX2T becomes active while TDV = 1. \n\n
 * Enables the transfer trigger unit to set bit TCSR.TE if the trigger signal DX2T becomes active
 * for event driven transfer starts.
 *
 * \par<b>Related APIs:</b><BR>
 * XMC_USIC_CH_DisableTBUFDataValidTrigger()\n\n\n
 */
__STATIC_INLINE void XMC_USIC_CH_EnableTBUFDataValidTrigger(XMC_USIC_CH_t *const channel)
{
  channel->TCSR |= (uint32_t)USIC_CH_TCSR_TDVTR_Msk;
}

/**
 * @param  channel Pointer to USIC channel handler of type @ref XMC_USIC_CH_t \n
 * 				   \b Range: @ref XMC_USIC0_CH0, @ref XMC_USIC0_CH1 to @ref XMC_USIC2_CH1 based on device support.
 * @return None
 *
 * \par<b>Description</b><br>
 * Disables the trigger of TDV depending on DX2T signal. \n\n
 * Bit TCSR.TE is permanently set.
 *
 * \par<b>Related APIs:</b><BR>
 * XMC_USIC_CH_EnableTBUFDataValidTrigger() \n\n\n
 */
__STATIC_INLINE void XMC_USIC_CH_DisableTBUFDataValidTrigger(XMC_USIC_CH_t *const channel)
{
  channel->TCSR &= (uint32_t)~USIC_CH_TCSR_TDVTR_Msk;
}

/**
 * @param  channel Pointer to USIC channel handler of type @ref XMC_USIC_CH_t \n
 * 				   \b Range: @ref XMC_USIC0_CH0, @ref XMC_USIC0_CH1 to @ref XMC_USIC2_CH1 based on device support.
 * @param  service_request_line service request number of the event to be triggered. \n
 * 			\b Range: 0 to 5.
 * @return None
 *
 * \par<b>Description</b><br>
 * Trigger a USIC interrupt service request.\n\n
 * When the USIC service request is triggered, the NVIC interrupt associated with it will be 
 * generated if enabled. 
 *
 * \par<b>Related APIs:</b><BR>
 * XMC_USIC_CH_SetInterruptNodePointer() \n\n\n
 */
__STATIC_INLINE void XMC_USIC_CH_TriggerServiceRequest(XMC_USIC_CH_t *const channel, const uint32_t service_request_line)
{
  channel->FMR = (uint32_t)(USIC_CH_FMR_SIO0_Msk << service_request_line);
}

/**
 * @param  channel Pointer to USIC channel handler of type @ref XMC_USIC_CH_t \n
 * 				   \b Range: @ref XMC_USIC0_CH0, @ref XMC_USIC0_CH1 to @ref XMC_USIC2_CH1 based on device support.
 * @param  transmit_buffer_status clearing or setting the TDV flag. \n
 *
 * @return None
 *
 * \par<b>Description</b><br>
 * Modify TCSR.TDV and TCSR.TE to control the start of a data word transmission by software.
 *
 * \par<b>Related APIs:</b><BR>
 * XMC_USIC_CH_GetTransmitBufferStatus() \n\n\n
 */
__STATIC_INLINE void XMC_USIC_CH_SetTransmitBufferStatus(XMC_USIC_CH_t *const channel,
		                                                 const XMC_USIC_CH_TBUF_STATUS_SET_t transmit_buffer_status)
{
  channel->FMR = (uint32_t)transmit_buffer_status;
}

/**
 * @param  channel Pointer to USIC channel handler of type @ref XMC_USIC_CH_t \n
 * 				   \b Range: @ref XMC_USIC0_CH0, @ref XMC_USIC0_CH1 to @ref XMC_USIC2_CH1 based on device support.
 * @param  passive_level Value of passive level for the channel. \n
 * 			\b Range: @ref XMC_USIC_CH_PASSIVE_DATA_LEVEL0, @ref XMC_USIC_CH_PASSIVE_DATA_LEVEL1
 * @return None
 *
 * \par<b>Description</b><br>
 * Set the passive data level of the output signal. \n\n
 * When the USIC channel transmit stage is idle, the output signal level stays at the 
 * configured passive level.
 * 
 * \par<b>Related APIs:</b><BR>
 * XMC_USIC_CH_GetTransmitBufferStatus(), XMC_USIC_CH_SetStartTransmisionMode() \n\n\n
 */
__STATIC_INLINE void XMC_USIC_CH_SetPassiveDataLevel(XMC_USIC_CH_t *const channel,
                                                     const XMC_USIC_CH_PASSIVE_DATA_LEVEL_t passive_level)
{
  channel->SCTR &= (~USIC_CH_SCTR_PDL_Msk);
  channel->SCTR |= (uint32_t)passive_level;
}

/* TX FIFO APIs */
/**
 * @param channel Pointer to USIC channel handler of type @ref XMC_USIC_CH_t \n
 * 				   \b Range: @ref XMC_USIC0_CH0, @ref XMC_USIC0_CH1 to @ref XMC_USIC2_CH1 based on device support.
 * @param data_pointer Start position inside the FIFO buffer. \n
 * 		\b Range: 0 to 63.
 * @param size Required size of the transmit FIFO. \n
 * 		\b Range: @ref XMC_USIC_CH_FIFO_DISABLED, @ref XMC_USIC_CH_FIFO_SIZE_2WORDS.. @ref XMC_USIC_CH_FIFO_SIZE_64WORDS
 * @param limit Threshold of transmit FIFO filling level to be considered for generating events. \n
 * 		\b Range: 0 to \a size -1.
 * @return None
 *
 * \par<b>Description</b><br>
 * Initializes the transmit FIFO. \n\n
 * Transmit FIFO is a subset of a common FIFO sized 64 words. This FIFO is shared between 2 channels of the USIC module. 
 * Each channel can share the FIFO for transmission and reception. \a data_pointer represents the start index in the common FIFO, 
 * from where transmit data can be put, for the selected USIC channel. \a size represents the size of transmit FIFO as a multiple of
 * 2. Since the FIFO is shared between 2 USIC channels, FIFO size should be carefully selected. A FIFO standard transmit buffer
 * event is generated when the FIFO filling level falls below the \a limit value.
 *
 * \par<b>Related APIs:</b><BR>
 * XMC_USIC_CH_TXFIFO_EnableEvent(), XMC_USIC_CH_TXFIFO_SetInterruptNodePointer() \n\n\n
 */
void XMC_USIC_CH_TXFIFO_Configure(XMC_USIC_CH_t *const channel,
                                  const uint32_t data_pointer,
                                  const XMC_USIC_CH_FIFO_SIZE_t size,
                                  const uint32_t limit);

/**
 * @param channel Pointer to USIC channel handler of type @ref XMC_USIC_CH_t \n
 * 				   \b Range: @ref XMC_USIC0_CH0, @ref XMC_USIC0_CH1 to @ref XMC_USIC2_CH1 based on device support.
 * @param size Required size of the transmit FIFO. \n
 * 		\b Range: @ref XMC_USIC_CH_FIFO_DISABLED, @ref XMC_USIC_CH_FIFO_SIZE_2WORDS.. @ref XMC_USIC_CH_FIFO_SIZE_64WORDS
 * @param limit Threshold for transmit FIFO filling level to be considered for generating events. \n
 * 		\b Range: 0 to \a size -1.
 * @return None
 *
 * \par<b>Description</b><br>
 * Sets the size and trigger limit for the transmit FIFO. \n\n
 * The API is not to be called for initializing the transmit FIFO. The API shall be used for the 
 * runtime change of transmit FIFO trigger limit. FIFO start position will not be affected on execution.
 *
 * \par<b>Related APIs:</b><BR>
 * XMC_USIC_CH_RXFIFO_SetSizeTriggerLimit() \n\n\n
 */
void XMC_USIC_CH_TXFIFO_SetSizeTriggerLimit(XMC_USIC_CH_t *const channel,
                                            const XMC_USIC_CH_FIFO_SIZE_t size,
                                            const uint32_t limit);


/**
 * @param channel Pointer to USIC channel handler of type @ref XMC_USIC_CH_t \n
 * 				   \b Range: @ref XMC_USIC0_CH0, @ref XMC_USIC0_CH1 to @ref XMC_USIC2_CH1 based on device support.
 * @param event Events to be enabled. Multiple events can be bitwise OR combined. @ref XMC_USIC_CH_TXFIFO_EVENT_CONF_t \n
 * @return None
 *
 * \par<b>Description</b><br>
 * Enables the interrupt events related to transmit FIFO. \n\n
 * Event bitmasks can be constructed using the enumeration @ref XMC_USIC_CH_TXFIFO_EVENT_CONF_t.
 * Multiple events can be enabled by providing multiple events in a single call. For providing 
 * multiple events, combine the events using bitwise OR operation. Events are configured in the TBCTR register.<br>
 * 
 * Note: API only enables the events. For interrupt generation, interrupt node must be configured and NVIC node
 * must be enabled.
 *
 * \par<b>Related APIs:</b><BR>
 * XMC_USIC_CH_TXFIFO_SetInterruptNodePointer() \n\n\n
 */
__STATIC_INLINE void XMC_USIC_CH_TXFIFO_EnableEvent(XMC_USIC_CH_t *const channel, const uint32_t event)
{
  channel->TBCTR |= event;
}


/**
 * @param channel Pointer to USIC channel handler of type @ref XMC_USIC_CH_t \n
 * 				   \b Range: @ref XMC_USIC0_CH0, @ref XMC_USIC0_CH1 to @ref XMC_USIC2_CH1 based on device support.
 * @param event Events to be disabled. @ref XMC_USIC_CH_TXFIFO_EVENT_CONF_t \n
 * @return None
 *
 * \par<b>Description</b><br>
 * Disables the interrupt events related to transmit FIFO. \n\n
 * By disabling the interrupt events, generation of interrupt is stopped. User can poll the event 
 * flags from the status register using the API XMC_USIC_CH_TXFIFO_GetEvent(). 
 * Event bitmasks can be constructed using the enumeration @ref XMC_USIC_CH_TXFIFO_EVENT_CONF_t. For providing
 * multiple events, combine the events using bitwise OR operation.
 *
 * \par<b>Related APIs:</b><BR>
 * XMC_USIC_CH_TXFIFO_GetEvent(), XMC_USIC_CH_TXFIFO_EnableEvent() \n\n\n
 */
__STATIC_INLINE void XMC_USIC_CH_TXFIFO_DisableEvent(XMC_USIC_CH_t *const channel, const uint32_t event)
{
  channel->TBCTR &= (uint32_t)~event;
}

/**
 * @param channel Pointer to USIC channel handler of type @ref XMC_USIC_CH_t \n
 * 				      \b Range: @ref XMC_USIC0_CH0, @ref XMC_USIC0_CH1 to @ref XMC_USIC2_CH1 based on device support.
 * @param interrupt_node Node pointer representing the transmit FIFO events. \n
 * 						  \b Range: @ref XMC_USIC_CH_TXFIFO_INTERRUPT_NODE_POINTER_STANDARD,
 * 						  			@ref XMC_USIC_CH_TXFIFO_INTERRUPT_NODE_POINTER_ALTERNATE
 * @param service_request The service request to be used for interrupt generation. \n
 * 						  \b Range: 0 to 5.
 * @return None
 *
 * \par<b>Description</b><br>
 * Sets an interrupt node for the transmit FIFO events.\n\n
 * A node pointer represents one or more interrupt events. Service request represents the interrupt node to be used
 * among the 6 interrupt nodes available for USIC module.
 * API configures the service request to be used for interrupt generation for the events selected.
 * A transmit FIFO event can generate an interrupt only if the interrupt node is configured for the event and
 * the interrupt generation is enabled for the event. For example, transmit FIFO standard transmit buffer
 * interrupt is generated if the interrupt node for the same is set and interrupt is enabled.<br>
 *
 * Note: NVIC node should be explicitly enabled for the interrupt generation.
 *
 * \par<b>Related APIs:</b><BR>
 * XMC_USIC_CH_TXFIFO_EnableEvent() \n\n\n
 */
void XMC_USIC_CH_TXFIFO_SetInterruptNodePointer(XMC_USIC_CH_t *const channel,
                                                const XMC_USIC_CH_TXFIFO_INTERRUPT_NODE_POINTER_t interrupt_node,
                                                const uint32_t service_request);

/**
 * @param channel Pointer to USIC channel handler of type @ref XMC_USIC_CH_t \n
 * 				   \b Range: @ref XMC_USIC0_CH0, @ref XMC_USIC0_CH1 to @ref XMC_USIC2_CH1 based on device support.
 * @param data Data to be transmitted. \n
 *           \b Range: 16bit unsigned data. minimum= 0, maximum= 65535
 * @return None
 *
 * \par<b>Description</b><br>
 * Writes data into the transmit FIFO. \n\n
 * The data provided is placed in the transmit FIFO.
 * The transmit FIFO should be configured before calling this API.
 *
 * \par<b>Related APIs:</b><BR>
 * XMC_USIC_CH_TXFIFO_EnableEvent() \n\n\n
 */
__STATIC_INLINE void XMC_USIC_CH_TXFIFO_PutData(XMC_USIC_CH_t *const channel, const uint16_t data)
{
  channel->IN[0] = data;
}

/**
 * @param channel Pointer to USIC channel handler of type @ref XMC_USIC_CH_t \n
 * 				   \b Range: @ref XMC_USIC0_CH0, @ref XMC_USIC0_CH1 to @ref XMC_USIC2_CH1 based on device support.
 * @param data Data to be transmitted.
 * @param frame_length Frame length to be configured while transmitting the data. \n
 * 			\b Range: minimum= 0, maximum= 31. e.g: For a frame length of 16, set \a frame_length as 15.
 * @return None
 *
 * \par<b>Description</b><br>
 * Writes data to the transmit FIFO in frame length control mode. \n\n
 * When frame length control is enabled for dynamic update of frame length, this API can be used.
 * \a frame_length represents the frame length to be updated by the peripheral. 
 * \a frame_length is used as index for the IN[] register array.
 * 
 * \par<b>Related APIs:</b><BR>
 * XMC_USIC_CH_EnableFrameLengthControl() \n\n\n
 */
__STATIC_INLINE void XMC_USIC_CH_TXFIFO_PutDataFLEMode(XMC_USIC_CH_t *const channel,
                                                       const uint16_t data,
                                                       const uint32_t frame_length)
{
  channel->IN[frame_length] = data;
}

/**
 * @param channel Pointer to USIC channel handler of type @ref XMC_USIC_CH_t \n
 * 				   \b Range: @ref XMC_USIC0_CH0, @ref XMC_USIC0_CH1 to @ref XMC_USIC2_CH1 based on device support.
 * @param data Data to be transmitted.
 * @param frame_length Frame length to be configured while transmitting the data. \n
 * 			\b Range: minimum= 0, maximum= 31. e.g: For a frame length of 16, set \a frame_length as 15.
 * @return None
 *
 * \par<b>Description</b><br>
 * Writes data to the transmit FIFO in hardware port control mode. \n\n
 * When hardware port control is enabled for dynamic update of frame length, this API can be used.
 * \a frame_length represents the frame length to be updated by the peripheral. 
 * \a frame_length is used as index for the IN[] register array.
 * 
 * \par<b>Related APIs:</b><BR>
 * XMC_USIC_CH_EnableFrameLengthControl() \n\n\n
 */
__STATIC_INLINE void XMC_USIC_CH_TXFIFO_PutDataHPCMode(XMC_USIC_CH_t *const channel,
                                                       const uint16_t data,
                                                       const uint32_t frame_length)
{
  channel->IN[frame_length] = data;
}

/**
 * @param channel Pointer to USIC channel handler of type @ref XMC_USIC_CH_t \n
 * 				   \b Range: @ref XMC_USIC0_CH0, @ref XMC_USIC0_CH1 to @ref XMC_USIC2_CH1 based on device support.
 * @return None
 *
 * \par<b>Description</b><br>
 * Clears the contents of transmit FIFO. \n\n
 * Transmit FIFO contents will be cleared and the filling level will be reset to 0.
 * 
 * \par<b>Related APIs:</b><BR>
 * XMC_USIC_CH_TXFIFO_GetLevel() \n\n\n
 */
__STATIC_INLINE void XMC_USIC_CH_TXFIFO_Flush(XMC_USIC_CH_t *const channel)
{
  channel->TRBSCR = (uint32_t)USIC_CH_TRBSCR_FLUSHTB_Msk;
}

/**
 * @param channel Pointer to USIC channel handler of type @ref XMC_USIC_CH_t \n
 * 				   \b Range: @ref XMC_USIC0_CH0, @ref XMC_USIC0_CH1 to @ref XMC_USIC2_CH1 based on device support.
 * @return Status \a true if transmit FIFO is full
 *              \a false if transmit FIFO is not full.
 *
 * \par<b>Description</b><br>
 * Checks if transmit FIFO is full. \n\n
 * When the transmit FIFO filling level reaches the configured size, FIFO full flag is set.
 * User should not write to the FIFO when the transmit FIFO is full.
 * 
 * \par<b>Related APIs:</b><BR>
 * XMC_USIC_CH_TXFIFO_IsEmpty(), XMC_USIC_CH_TXFIFO_Flush() \n\n\n
 */
__STATIC_INLINE bool XMC_USIC_CH_TXFIFO_IsFull(XMC_USIC_CH_t *const channel)
{
  return (bool)(channel->TRBSR & USIC_CH_TRBSR_TFULL_Msk);
}

/**
 * @param channel Pointer to USIC channel handler of type @ref XMC_USIC_CH_t \n
 * 				   \b Range: @ref XMC_USIC0_CH0, @ref XMC_USIC0_CH1 to @ref XMC_USIC2_CH1 based on device support.
 * @return Status \a true if transmit FIFO is empty
 *              \a false if transmit FIFO has some data.
 *
 * \par<b>Description</b><br>
 * Checks if transmit FIFO is empty. \n\n
 * When the transmit FIFO is empty, data can be written to FIFO.
 * When the last written word to the transmit FIFO is transmitted out of the FIFO, 
 * FIFO empty flag is set.
 * 
 * \par<b>Related APIs:</b><BR>
 * XMC_USIC_CH_TXFIFO_Flush(), XMC_USIC_CH_TXFIFO_PutData() \n\n\n
 */
__STATIC_INLINE bool XMC_USIC_CH_TXFIFO_IsEmpty(XMC_USIC_CH_t *const channel)
{
  return (bool)(channel->TRBSR & USIC_CH_TRBSR_TEMPTY_Msk);
}

/**
 * @param channel Pointer to USIC channel handler of type @ref XMC_USIC_CH_t \n
 * 				   \b Range: @ref XMC_USIC0_CH0, @ref XMC_USIC0_CH1 to @ref XMC_USIC2_CH1 based on device support.
 * @return Transmit FIFO filling level. \n 
 *           \b Range: minimum= 0(FIFO empty), maximum= transmit FIFO size.
 *
 * \par<b>Description</b><br>
 * Gets the transmit FIFO filling level. \n\n
 * For every word written to the FIFO, filling level is updated. The API gives the value
 * of this filling level.
 * 
 * \par<b>Related APIs:</b><BR>
 * XMC_USIC_CH_TXFIFO_Flush(), XMC_USIC_CH_TXFIFO_PutData() \n\n\n
 */
__STATIC_INLINE uint32_t XMC_USIC_CH_TXFIFO_GetLevel(XMC_USIC_CH_t *const channel)
{
  return ((uint32_t)(channel->TRBSR & USIC_CH_TRBSR_TBFLVL_Msk) >> USIC_CH_TRBSR_TBFLVL_Pos);
}

/**
 * @param channel Pointer to USIC channel handler of type @ref XMC_USIC_CH_t \n
 * 				   \b Range: @ref XMC_USIC0_CH0, @ref XMC_USIC0_CH1 to @ref XMC_USIC2_CH1 based on device support.
 * @return Status of standard transmit and transmit buffer error events. @ref XMC_USIC_CH_TXFIFO_EVENT_t \n
 * 
 * \par<b>Description</b><br>
 * Gets the transmit FIFO event status. \n\n
 * Gives the status of transmit FIFO standard transmit buffer event and transmit buffer error event.
 * The status bits are located at their bit positions in the TRBSR register in the returned value.
 * User can make use of the @ref XMC_USIC_CH_TXFIFO_EVENT_t enumeration for checking the status of return value.
 * The status can be found by using the bitwise AND operation on the returned value with the enumerated value.<br>
 * 
 * Note: Event status flags should be cleared by the user explicitly.
 *
 * \par<b>Related APIs:</b><BR>
 * XMC_USIC_CH_TXFIFO_ClearEvent() \n\n\n
 */
__STATIC_INLINE uint32_t XMC_USIC_CH_TXFIFO_GetEvent(XMC_USIC_CH_t *const channel)
{
  return (uint32_t)((channel->TRBSR) & (USIC_CH_TRBSR_STBI_Msk |
                              USIC_CH_TRBSR_TBERI_Msk));
}

/**
 * @param channel Pointer to USIC channel handler of type @ref XMC_USIC_CH_t \n
 * 				   \b Range: @ref XMC_USIC0_CH0, @ref XMC_USIC0_CH1 to @ref XMC_USIC2_CH1 based on device support.
 * @param  event Transmit FIFO events to be cleared. \n
 * 			     \b Range: @ref XMC_USIC_CH_TXFIFO_EVENT_STANDARD, @ref XMC_USIC_CH_TXFIFO_EVENT_ERROR.
 * @return None
 *
 * \par<b>Description</b><br>
 * Clears the transmit FIFO event flags in the status register. \n\n
 * USIC channel peripheral does not clear the event flags after they are read.
 * This API clears the events provided in the \a mask value. 
 * XMC_USIC_CH_TXFIFO_EVENT enumeration can be used as input. Multiple events
 * can be cleared by providing a mask value obtained by bitwise OR operation of 
 * multiple event enumerations.
 *
 * \par<b>Related APIs:</b><BR>
 * XMC_USIC_CH_TXFIFO_GetEvent() \n\n\n
 */
__STATIC_INLINE void XMC_USIC_CH_TXFIFO_ClearEvent(XMC_USIC_CH_t *const channel,
                                                   const uint32_t event)
{
  channel->TRBSCR = event;
}

/**
 * @param channel Pointer to USIC channel handler of type @ref XMC_USIC_CH_t \n
 * 				   \b Range: @ref XMC_USIC0_CH0, @ref XMC_USIC0_CH1 to @ref XMC_USIC2_CH1 based on device support.
 * @param data_pointer Start position inside the FIFO buffer. \n
 * 		\b Range: 0 to 63.
 * @param size Required size of the receive FIFO. \n
 * 		\b Range: @ref XMC_USIC_CH_FIFO_DISABLED, @ref XMC_USIC_CH_FIFO_SIZE_2WORDS.. @ref XMC_USIC_CH_FIFO_SIZE_64WORDS
 * @param limit Threshold of receive FIFO filling level to be considered for generating events. \n
 * 		\b Range: 0 to \a size -1.
 * @return None
 *
 * \par<b>Description</b><br>
 * Configures the receive FIFO. \n\n
 * Receive FIFO is the subset of a common FIFO sized 64 words. This FIFO is shared between 2 channels of the USIC module. 
 * Each channel can share the FIFO for transmission and reception. \a data_pointer represents the start index in the common FIFO, 
 * from where received data can be put. \a size represents the size of receive FIFO as a multiple of 2. 
 * Since the FIFO is shared between 2 USIC channels, FIFO size should be carefully selected. A FIFO standard receive buffer
 * event or alternative receive buffer event is generated when the FIFO filling level exceeds the \a limit value.
 *
 * \par<b>Related APIs:</b><BR>
 * XMC_USIC_CH_RXFIFO_EnableEvent(), XMC_USIC_CH_RXFIFO_SetInterruptNodePointer() \n\n\n
*/
void XMC_USIC_CH_RXFIFO_Configure(XMC_USIC_CH_t *const channel,
                                  const uint32_t data_pointer,
                                  const XMC_USIC_CH_FIFO_SIZE_t size,
                                  const uint32_t limit);

/**
 * @param channel Pointer to USIC channel handler of type @ref XMC_USIC_CH_t \n
 * 				   \b Range: @ref XMC_USIC0_CH0, @ref XMC_USIC0_CH1 to @ref XMC_USIC2_CH1 based on device support.
 * @param size Required size of the receive FIFO. \n
 * 		\b Range: @ref XMC_USIC_CH_FIFO_DISABLED, @ref XMC_USIC_CH_FIFO_SIZE_2WORDS.. @ref XMC_USIC_CH_FIFO_SIZE_64WORDS
 * @param limit Threshold for receive FIFO filling level to be considered for generating events. \n
 * 		\b Range: 0 to \a size -1.
 * @return None
 *
 * \par<b>Description</b><br>
 * Sets the size and trigger limit for the receive FIFO. \n\n
 * The API is not to be called for initializing the receive FIFO. The API shall be used for the 
 * runtime change of receive FIFO trigger limit. FIFO start position will not be affected on execution.
 *
 * \par<b>Related APIs:</b><BR>
 * XMC_USIC_CH_TXFIFO_SetSizeTriggerLimit()\ n\n\n
 */
void XMC_USIC_CH_RXFIFO_SetSizeTriggerLimit(XMC_USIC_CH_t *const channel,
                                            const XMC_USIC_CH_FIFO_SIZE_t size,
                                            const uint32_t limit);

/**
 * @param channel Pointer to USIC channel handler of type @ref XMC_USIC_CH_t \n
 * 				   \b Range: @ref XMC_USIC0_CH0, @ref XMC_USIC0_CH1 to @ref XMC_USIC2_CH1 based on device support.
 * @param event Events to be enabled. Multiple events can be bitwise OR combined. @ref XMC_USIC_CH_RXFIFO_EVENT_CONF_t\n
 * @return None
 *
 * \par<b>Description</b><br>
 * Enables the interrupt events related to transmit FIFO. \n\n
 * Event bitmasks can be constructed using the enumeration @ref XMC_USIC_CH_RXFIFO_EVENT_CONF_t.
 * Multiple events can be enabled by providing multiple events in a single call. For providing 
 * multiple events, combine the events using bitwise OR operation.<br>
 * 
 * Note: API only enables the events. For interrupt generation, interrupt node must be configured and NVIC node
 * must be enabled.
 *
 * \par<b>Related APIs:</b><BR>
 * XMC_USIC_CH_RXFIFO_SetInterruptNodePointer() \n\n\n
 */
__STATIC_INLINE void XMC_USIC_CH_RXFIFO_EnableEvent(XMC_USIC_CH_t *const channel, const uint32_t event)
{
  channel->RBCTR |= event;
}


/**
 * @param channel Pointer to USIC channel handler of type @ref XMC_USIC_CH_t \n
 * 				   \b Range: @ref XMC_USIC0_CH0, @ref XMC_USIC0_CH1 to @ref XMC_USIC2_CH1 based on device support.
 * @param event Events to be disabled. \n
 * 			\b Range: @ref XMC_USIC_CH_RXFIFO_EVENT_CONF_STANDARD, @ref XMC_USIC_CH_RXFIFO_EVENT_CONF_ERROR,
 * 			@ref XMC_USIC_CH_RXFIFO_EVENT_CONF_ALTERNATE.
 * @return None
 *
 * \par<b>Description</b><br>
 * Disables the selected interrupt events related to receive FIFO. \n\n
 * By disabling the interrupt events, generation of interrupt is stopped. User can poll the event 
 * flags from the status register using the API XMC_USIC_CH_RXFIFO_GetEvent(). 
 * Event bitmasks can be constructed using the enumeration \a XMC_USIC_CH_RXFIFO_EVENT_CONF. For providing 
 * multiple events, combine the events using bitwise OR operation.
 *
 * \par<b>Related APIs:</b><BR>
 * XMC_USIC_CH_RXFIFO_GetEvent(), XMC_USIC_CH_RXFIFO_EnableEvent() \n\n\n
 */
__STATIC_INLINE void XMC_USIC_CH_RXFIFO_DisableEvent(XMC_USIC_CH_t *const channel, const uint32_t event)
{
  channel->RBCTR &= (uint32_t)~event;
}

/**
 * @param channel Pointer to USIC channel handler of type @ref XMC_USIC_CH_t \n
 * 				   \b Range: @ref XMC_USIC0_CH0, @ref XMC_USIC0_CH1 to @ref XMC_USIC2_CH1 based on device support.
 * @param interrupt_node Node pointer representing the receive FIFO events. \n
 * 					\b Range: @ref XMC_USIC_CH_RXFIFO_INTERRUPT_NODE_POINTER_STANDARD,
 * 					@ref XMC_USIC_CH_RXFIFO_INTERRUPT_NODE_POINTER_ALTERNATE
 * @param service_request The service request to be used for interrupt generation.\n
 * 					\b Range: 0 to 5.
 * @return None
 *
 * \par<b>Description</b><br>
 * Sets an interrupt node for the receive FIFO events. \n\n
 * A node pointer represents one or more interrupt events. Service request represents the interrupt node to be used
 * among the 6 interrupt nodes available for USIC module.
 * API configures the service request to be used for interrupt generation for the events selected.
 * A receive FIFO event can generate an interrupt only if the interrupt node is configured for the event and
 * the interrupt generation is enabled for the event. For example, transmit FIFO standard transmit buffer
 * interrupt is generated if the interrupt node for the same is set and interrupt is enabled.<br>
 *
 * Note: NVIC node should be explicitly enabled for the interrupt generation.
 *
 * \par<b>Related APIs:</b><BR>
 * XMC_USIC_CH_RXFIFO_EnableEvent() \n\n\n
 */
void XMC_USIC_CH_RXFIFO_SetInterruptNodePointer(XMC_USIC_CH_t *const channel,
                                                const XMC_USIC_CH_RXFIFO_INTERRUPT_NODE_POINTER_t interrupt_node,
                                                const uint32_t service_request);

/**
 * @param channel Pointer to USIC channel handler of type @ref XMC_USIC_CH_t \n
 * 				   \b Range: @ref XMC_USIC0_CH0, @ref XMC_USIC0_CH1 to @ref XMC_USIC2_CH1 based on device support.
 * @return Value read from the receive FIFO. \n
 * 			\b Range: 16bit data. Length of data depends on the word length configuration.
 *
 * \par<b>Description</b><br>
 * Gets data from the receive FIFO. \n\n
 * Receive FIFO should be read only if data is availble in the FIFO. This can be checked using
 * the API XMC_USIC_CH_RXFIFO_IsEmpty(). Receive FIFO error flag will be set if an attempt is made
 * to read from an empty receive FIFO. To read all the received data, user should keep reading data 
 * until receive FIFO is empty.
 *
 * \par<b>Related APIs:</b><BR>
 * XMC_USIC_CH_RXFIFO_ClearEvent() \n\n\n
 */
__STATIC_INLINE uint16_t XMC_USIC_CH_RXFIFO_GetData(XMC_USIC_CH_t *const channel)
{
  return (uint16_t)(channel->OUTR);
}

/**
 * @param channel Pointer to USIC channel handler of type @ref XMC_USIC_CH_t \n
 * 				   \b Range: @ref XMC_USIC0_CH0, @ref XMC_USIC0_CH1 to @ref XMC_USIC2_CH1 based on device support.
 * @return None
 *
 * \par<b>Description</b><br>
 * Clears the contents of receive FIFO. \n\n
 * Receive FIFO contents will be cleared and the filling level will be reset to 0.
 * 
 * \par<b>Related APIs:</b><BR>
 * XMC_USIC_CH_RXFIFO_GetLevel() \n\n\n
 */
__STATIC_INLINE void XMC_USIC_CH_RXFIFO_Flush(XMC_USIC_CH_t *const channel)
{
  channel->TRBSCR = (uint32_t)USIC_CH_TRBSCR_FLUSHRB_Msk;
}

/**
 * @param channel Pointer to USIC channel handler of type @ref XMC_USIC_CH_t \n
 * 				   \b Range: @ref XMC_USIC0_CH0, @ref XMC_USIC0_CH1 to @ref XMC_USIC2_CH1 based on device support.
 * @return Status \a true if receive FIFO is full
 *              \a false if receive FIFO is not full.
 *
 * \par<b>Description</b><br>
 * Checks if receive FIFO is full. \n\n
 * When the receive FIFO filling level reaches the configured size, FIFO full flag is set.
 * Any data received when the receive FIFO is full, is lost.
 * 
 * \par<b>Related APIs:</b><BR>
 * XMC_USIC_CH_RXFIFO_IsEmpty(), XMC_USIC_CH_RXFIFO_Flush() \n\n\n
 */
__STATIC_INLINE bool XMC_USIC_CH_RXFIFO_IsFull(XMC_USIC_CH_t *const channel)
{
  return (bool)(channel->TRBSR & USIC_CH_TRBSR_RFULL_Msk);
}

/**
 * @param channel Pointer to USIC channel handler of type @ref XMC_USIC_CH_t \n
 * 				   \b Range: @ref XMC_USIC0_CH0, @ref XMC_USIC0_CH1 to @ref XMC_USIC2_CH1 based on device support.
 * @return Status \a true if receive FIFO is empty,
 *              \a false if receive FIFO has some data.
 *
 * \par<b>Description</b><br>
 * Checks if receive FIFO is empty. \n\n
 * When the receive FIFO is empty, received data will be put in receive FIFO.
 * When the last received word in the FIFO is read, FIFO empty flag is set. Any attempt
 * to read from an empty receive FIFO will set the receive FIFO error flag.
 * 
 * \par<b>Related APIs:</b><BR>
 * XMC_USIC_CH_RXFIFO_Flush(), XMC_USIC_CH_RXFIFO_PutData() \n\n\n
 */
__STATIC_INLINE bool XMC_USIC_CH_RXFIFO_IsEmpty(XMC_USIC_CH_t *const channel)
{
  return (bool)(channel->TRBSR & USIC_CH_TRBSR_REMPTY_Msk);
}

/**
 * @param channel Pointer to USIC channel handler of type @ref XMC_USIC_CH_t \n
 * 				   \b Range: @ref XMC_USIC0_CH0, @ref XMC_USIC0_CH1 to @ref XMC_USIC2_CH1 based on device support.
 * @return uint32_t Receive FIFO filling level. \n
 *           \b Range: minimum= 0(FIFO empty), maximum= receive FIFO size.
 *
 * \par<b>Description</b><br>
 * Gets the receive FIFO filling level. \n\n
 * For every word received, the filling level is incremented. The API gives the value
 * of this filling level. The filling level is decremented when the data is read out of the 
 * receive FIFO.
 * 
 * \par<b>Related APIs:</b><BR>
 * XMC_USIC_CH_RXFIFO_Flush(), XMC_USIC_CH_RXFIFO_PutData() \n\n\n
 */
__STATIC_INLINE uint32_t XMC_USIC_CH_RXFIFO_GetLevel(XMC_USIC_CH_t *const channel)
{
  return ((uint32_t)(channel->TRBSR & USIC_CH_TRBSR_RBFLVL_Msk) >> USIC_CH_TRBSR_RBFLVL_Pos);
}

/**
 * @param channel Pointer to USIC channel handler of type @ref XMC_USIC_CH_t \n
 * 				   \b Range: @ref XMC_USIC0_CH0, @ref XMC_USIC0_CH1 to @ref XMC_USIC2_CH1 based on device support.
 * @return Status of standard receive buffer, alternative receive buffer and receive buffer error events. \n
 * 			\b Range: @ref XMC_USIC_CH_RXFIFO_EVENT_STANDARD, @ref XMC_USIC_CH_RXFIFO_EVENT_ERROR,
 * 					  @ref XMC_USIC_CH_RXFIFO_EVENT_ALTERNATE.
 * 
 * \par<b>Description</b><br>
 * Gets the receive FIFO events' status. \n\n
 * Gives the status of receive FIFO standard receive buffer event, alternative receive buffer event and receive buffer error event.
 * The status bits are located at their bitpositions in the TRBSR register in the returned value.
 * User can make use of the XMC_USIC_CH_RXFIFO_EVENT enumeration for checking the status of return value.
 * The status can be found by using the bitwise AND operation on the returned value with the enumerated value.<br>
 * 
 * Note: Event status flags should be cleared by the user explicitly.
 *
 * \par<b>Related APIs:</b><BR>
 * XMC_USIC_CH_RXFIFO_ClearEvent()\n\n\n
 */
__STATIC_INLINE uint32_t XMC_USIC_CH_RXFIFO_GetEvent(XMC_USIC_CH_t *const channel)
{
  return (uint32_t)((channel->TRBSR) & (USIC_CH_TRBSR_SRBI_Msk |
                              USIC_CH_TRBSR_RBERI_Msk |
                              USIC_CH_TRBSR_ARBI_Msk));
}

/**
 * @param channel Pointer to USIC channel handler of type @ref XMC_USIC_CH_t \n
 * 				   \b Range: @ref XMC_USIC0_CH0, @ref XMC_USIC0_CH1 to @ref XMC_USIC2_CH1 based on device support.
 * @param  event Receive FIFO events to be cleared. \n
 * 			\b Range: @ref XMC_USIC_CH_RXFIFO_EVENT_STANDARD, @ref XMC_USIC_CH_RXFIFO_EVENT_ERROR,
 * 					  @ref XMC_USIC_CH_RXFIFO_EVENT_ALTERNATE.
 * @return None
 *
 * \par<b>Description</b><br>
 * Clears the receive FIFO event flags in the status register. \n\n
 * USIC channel peripheral does not clear the event flags after they are read.
 * This API clears the events provided in the \a mask value. 
 * XMC_USIC_CH_RXFIFO_EVENT enumeration can be used as input. Multiple events
 * can be cleared by providing a mask value obtained by bitwise OR operation of 
 * multiple event enumerations.
 *
 * \par<b>Related APIs:</b><BR>
 * XMC_USIC_CH_RXFIFO_GetEvent() \n\n\n
 */
__STATIC_INLINE void XMC_USIC_CH_RXFIFO_ClearEvent(XMC_USIC_CH_t *const channel,
                                                   const uint32_t event)
{
  channel->TRBSCR = event;
}

/**
 * @param channel Pointer to USIC channel handler of type @ref XMC_USIC_CH_t \n
 * 				   \b Range: @ref XMC_USIC0_CH0, @ref XMC_USIC0_CH1 to @ref XMC_USIC2_CH1 based on device support.
 * @return None
 *
 * \par<b>Description</b><br>
 * Enables time measurement using the capture mode timer. \n\n
 * Time measurement is enabled by setting the timer enable flag in BRG register.
 *
 * \par<b>Related APIs:</b><BR>
 * XMC_USIC_CH_DisableTimeMeasurement() \n\n\n
 */
__STATIC_INLINE void XMC_USIC_CH_EnableTimeMeasurement(XMC_USIC_CH_t *const channel)
{
  channel->BRG |= (uint32_t)USIC_CH_BRG_TMEN_Msk;
}

/**
 * @param channel Pointer to USIC channel handler of type @ref XMC_USIC_CH_t \n
 * 				   \b Range: @ref XMC_USIC0_CH0, @ref XMC_USIC0_CH1 to @ref XMC_USIC2_CH1 based on device support.
 * @return None
 *
 * \par<b>Description</b><br>
 * Disables time measurement using the capture mode timer. \n\n
 * Time measurement is disabled by clearing the timer enable flag in BRG register.
 *
 * \par<b>Related APIs:</b><BR>
 * XMC_USIC_CH_EnableTimeMeasurement() \n\n\n
 */
__STATIC_INLINE void XMC_USIC_CH_DisableTimeMeasurement(XMC_USIC_CH_t *const channel)
{
  channel->BRG &= (uint32_t)~USIC_CH_BRG_TMEN_Msk;
}

/**
 * @param channel Pointer to USIC channel handler of type @ref XMC_USIC_CH_t \n
 * 				   \b Range: @ref XMC_USIC0_CH0, @ref XMC_USIC0_CH1 to @ref XMC_USIC2_CH1 based on device support.
 * @param passive_level Passive level for the master clock output. \n
 * 					\b Range: @ref XMC_USIC_CH_BRG_MASTER_CLOCK_PASSIVE_LEVEL_0, @ref XMC_USIC_CH_BRG_MASTER_CLOCK_PASSIVE_LEVEL_1.
 * @return None
 *
 * \par<b>Description</b><br>
 * Sets the idle mode pin level for the master clock output. \n
 */
__STATIC_INLINE void XMC_USIC_CH_SetMclkOutputPassiveLevel(XMC_USIC_CH_t *const channel,
                                                           const XMC_USIC_CH_BRG_MASTER_CLOCK_PASSIVE_LEVEL_t passive_level)
{
  channel->BRG = (uint32_t)(channel->BRG & (~USIC_CH_BRG_MCLKCFG_Msk)) | (uint32_t)passive_level;
}
/**
 * @param channel Pointer to USIC channel handler of type @ref XMC_USIC_CH_t \n
 * 				   \b Range: @ref XMC_USIC0_CH0, @ref XMC_USIC0_CH1 to @ref XMC_USIC2_CH1 based on device support.
 * @param passive_level Passive level for the clock output. \n
 * 					\b Range: @ref XMC_USIC_CH_BRG_SHIFT_CLOCK_PASSIVE_LEVEL_0_DELAY_DISABLED,
 * 							  @ref XMC_USIC_CH_BRG_SHIFT_CLOCK_PASSIVE_LEVEL_0_DELAY_ENABLED,
 * 							  @ref XMC_USIC_CH_BRG_SHIFT_CLOCK_PASSIVE_LEVEL_1_DELAY_DISABLED,
 *							  @ref XMC_USIC_CH_BRG_SHIFT_CLOCK_PASSIVE_LEVEL_1_DELAY_ENABLED,
 * @param clock_output Shift clock source selection. \n
 * 					\b Range: Use @ref XMC_USIC_CH_BRG_SHIFT_CLOCK_OUTPUT_SCLK,
 * 								  @ref XMC_USIC_CH_BRG_SHIFT_CLOCK_OUTPUT_DX1
 * @return None
 *
 * \par<b>Description</b><br>
 * Sets the idle mode shift clock output level and selects the shift clock source. \n\n
 * Shift clock idle mode output level can be set to logic high or low. Shift clock output can be configured to have a 
 * delay of half shift clock period. Both the configurations are available as enumeration values defined with type
 * @ref XMC_USIC_CH_BRG_SHIFT_CLOCK_PASSIVE_LEVEL_t.
 * This value should be configured based on the slave device requirement.
 * Shift clock source can be selected between internal clock(master) and external input(slave).
 * 
 */
__STATIC_INLINE void XMC_USIC_CH_ConfigureShiftClockOutput(XMC_USIC_CH_t *const channel,
                                                           const XMC_USIC_CH_BRG_SHIFT_CLOCK_PASSIVE_LEVEL_t passive_level,
                                                           const XMC_USIC_CH_BRG_SHIFT_CLOCK_OUTPUT_t clock_output)
{
  channel->BRG = (uint32_t)(channel->BRG & (~(USIC_CH_BRG_SCLKCFG_Msk |
                                             USIC_CH_BRG_SCLKOSEL_Msk))) |
                 (uint32_t)passive_level |
                 (uint32_t)clock_output;
}

/**
 * @param channel Pointer to USIC channel handler of type @ref XMC_USIC_CH_t \n
 * 				   \b Range: @ref XMC_USIC0_CH0, @ref XMC_USIC0_CH1 to @ref XMC_USIC2_CH1 based on device support.
 * @param mode USIC channel operation mode. \n
 * 				\b Range: @ref XMC_USIC_CH_OPERATING_MODE_IDLE, @ref XMC_USIC_CH_OPERATING_MODE_SPI,
 * 						  @ref XMC_USIC_CH_OPERATING_MODE_UART, @ref XMC_USIC_CH_OPERATING_MODE_I2S,
 * 						  @ref XMC_USIC_CH_OPERATING_MODE_I2C.
 * @return None
 *
 * \par<b>Description</b><br>
 * Sets the USIC channel operation mode.\n\n
 * A USIC channel can support multiple serial communication protocols like UART, SPI, I2C and I2S.
 * The API sets the input operation mode to the USIC channel.
 *
 * \par<b>Related APIs:</b><BR>
 * XMC_USIC_Enable(), XMC_USIC_CH_Enable() \n\n\n
 */
__STATIC_INLINE void XMC_USIC_CH_SetMode(XMC_USIC_CH_t *const channel, const XMC_USIC_CH_OPERATING_MODE_t mode)
{
  channel->CCR = (uint32_t)(channel->CCR & (~(USIC_CH_CCR_MODE_Msk))) | (uint32_t)mode;
}
#ifdef __cplusplus
}
#endif

/**
 * @}
 */

/**
 * @}
 */

#endif
