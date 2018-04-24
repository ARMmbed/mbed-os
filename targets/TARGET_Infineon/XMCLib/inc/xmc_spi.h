/**
 * @file xmc_spi.h
 * @date 2016-05-20
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
 *     - Initial
 *   
 * 2015-05-20:
 *     - Documentation improved <br>
 *     - Added XMC_SPI_CH_SetSlaveSelectDelay(), XMC_SPI_CH_TriggerServiceRequest() and 
 *       XMC_SPI_CH_SelectInterruptNodePointer() <br>
 *     - Added XMC_SPI_CH_SetInterwordDelaySCLK() <br>
 *
 * 2015-06-20:
 *     - Removed version macros and declaration of GetDriverVersion API <br>
 *
 * 2015-08-24:
 *     - Added APIs for enabling/disabling delay compensation XMC_SPI_CH_DisableDelayCompensation() and
 *       XMC_SPI_CH_EnableDelayCompensation() <br>
 *
 * 2015-08-27:
 *     - Added APIs for external input for BRG configuration:XMC_SPI_CH_ConfigExternalInputSignalToBRG() <br>
 *
 * 2015-09-01:
 *     - Modified XMC_SPI_CH_SetInputSource() for avoiding complete DXCR register overwriting. <br>
 *     - Modified XMC_SPI_CH_EVENT_t enum for supporting XMC_SPI_CH_EnableEvent() and XMC_SPI_CH_DisableEvent()
 *       for supporting multiple events configuration <br>
 *
 * 2015-09-08:
 *     - Adding API for configuring the receiving clock phase in the slave:XMC_SPI_CH_DataLatchedInTrailingEdge() and XMC_SPI_CH_DataLatchedInLeadingEdge() <br>
 *
 * 2016-04-10:
 *     - Added an API for configuring the transmit mode:XMC_SPI_CH_SetTransmitMode() <br>
 *
 * 2016-05-20:
 *     - Added XMC_SPI_CH_EnableDataTransmission() and XMC_SPI_CH_DisableDataTransmission()
 * 
 * @endcond 
 *
 */

#ifndef XMC_SPI_H
#define XMC_SPI_H

/**********************************************************************************************************************
 * HEADER FILES
 *********************************************************************************************************************/
#include "xmc_usic.h"

/**
 * @addtogroup XMClib XMC Peripheral Library
 * @{
 */

/**
 * @addtogroup SPI
 * @brief Synchronous serial channel driver for SPI-like communication.
 * 
 * The SPI driver uses Universal Serial Interface Channel(USIC) module. 
 * The USIC module supports multiple data lines for SPI communication. \n
 * -# Full duplex communication with 2 separate lines for transmission and reception.
 * -# Half duplex communication with 1 common line shared for transmission and reception.
 * -# Dual mode communication with 2 common lines shared for transmission and reception.
 * -# Quad mode communication with 4 common lines shared for transmission and reception.<br><br>
 * 
 * SPI driver provides structures, enumerations and APIs for configuring the USIC channel for SPI communication
 * and also for data transaction.<br>
 * SPI driver features:
 * -# Configuration structure XMC_SPI_CH_CONFIG_t and SPI initialization function XMC_SPI_CH_Init()
 * -# Allows configuration of protocol word and frame length using XMC_SPI_CH_SetWordLength(), XMC_SPI_CH_SetFrameLength()
 * -# Allows manipulation of data frame at runtime using XMC_SPI_CH_EnableSOF(), XMC_SPI_CH_EnableEOF(), 
 XMC_SPI_CH_EnableSlaveSelect(), XMC_SPI_CH_DisableSlaveSelect()
 * -# Provides APIs for transmitting data and receiving data using XMC_SPI_CH_Transmit(), XMC_SPI_CH_Receive(), XMC_SPI_CH_GetReceivedData()
 * -# Allows configuration of shift clock using XMC_SPI_CH_ConfigureShiftClockOutput()
 * -# Provides enumeration of SPI protocol events using @ref XMC_SPI_CH_STATUS_FLAG_t
 * @{
 */

/***********************************************************************************************************************
 * MACROS
 **********************************************************************************************************************/

#if defined(USIC0)
#define XMC_SPI0_CH0 XMC_USIC0_CH0 /**< SPI0 channel 0 base address */
#define XMC_SPI0_CH1 XMC_USIC0_CH1 /**< SPI0 channel 1 base address */
#endif

#if defined(USIC1)
#define XMC_SPI1_CH0 XMC_USIC1_CH0 /**< SPI1 channel 0 base address */
#define XMC_SPI1_CH1 XMC_USIC1_CH1 /**< SPI1 channel 1 base address */
#endif

#if defined(USIC2)
#define XMC_SPI2_CH0 XMC_USIC2_CH0 /**< SPI2 channel 0 base address */
#define XMC_SPI2_CH1 XMC_USIC2_CH1 /**< SPI2 channel 1 base address */
#endif

/***********************************************************************************************************************
 * ENUMS
 ***********************************************************************************************************************/

/**
 * Defines return status of SPI driver APIs
 */
typedef enum XMC_SPI_CH_STATUS
{
  XMC_SPI_CH_STATUS_OK,    /**< Status of the Module: OK */
  XMC_SPI_CH_STATUS_ERROR, /**< Status of the Module: ERROR */
  XMC_SPI_CH_STATUS_BUSY   /**< The Module is busy */
} XMC_SPI_CH_STATUS_t;
/**
 * Defines the SPI bus mode
 */
typedef enum XMC_SPI_CH_BUS_MODE
{
  XMC_SPI_CH_BUS_MODE_MASTER, /**< SPI Master */
  XMC_SPI_CH_BUS_MODE_SLAVE   /**< SPI Slave */
} XMC_SPI_CH_BUS_MODE_t;

/**
 * Defines the Polarity of the slave select signals SELO[7:0] in relation to the master slave select signal MSLS.
 */
typedef enum XMC_SPI_CH_SLAVE_SEL_MSLS_INV
{
  XMC_SPI_CH_SLAVE_SEL_SAME_AS_MSLS = 0x0UL, /**< The SELO outputs have the same polarity as the MSLS signal 
                                                  (active high) */
  XMC_SPI_CH_SLAVE_SEL_INV_TO_MSLS = 0x1UL << USIC_CH_PCR_SSCMode_SELINV_Pos /**< The SELO outputs have the inverted 
                                                                                  polarity to the MSLS signal
                                                                                  (active low)*/
} XMC_SPI_CH_SLAVE_SEL_MSLS_INV_t;

/**
 * Defines the Polarity of the data inputs.
 */
typedef enum XMC_SPI_CH_DATA_POLARITY
{
  XMC_SPI_CH_DATA_POLARITY_DIRECT = 0x0UL, /**< The polarity of the data line is not inverted */
  XMC_SPI_CH_DATA_POLARITY_INVERT = 0x1UL << USIC_CH_DX2CR_DPOL_Pos /**< The polarity of the data line is inverted */
} XMC_SPI_CH_DATA_POLARITY_t;

/**
 * Defines Slave Select lines
 */
typedef enum XMC_SPI_CH_SLAVE_SELECT
{
  XMC_SPI_CH_SLAVE_SELECT_0 = 1UL << USIC_CH_PCR_SSCMode_SELO_Pos,  /**< Slave Select line 0 */
  XMC_SPI_CH_SLAVE_SELECT_1 = 2UL << USIC_CH_PCR_SSCMode_SELO_Pos,  /**< Slave Select line 1 */
  XMC_SPI_CH_SLAVE_SELECT_2 = 4UL << USIC_CH_PCR_SSCMode_SELO_Pos,  /**< Slave Select line 2 */
  XMC_SPI_CH_SLAVE_SELECT_3 = 8UL << USIC_CH_PCR_SSCMode_SELO_Pos,  /**< Slave Select line 3 */
  XMC_SPI_CH_SLAVE_SELECT_4 = 16UL << USIC_CH_PCR_SSCMode_SELO_Pos, /**< Slave Select line 4 */
  XMC_SPI_CH_SLAVE_SELECT_5 = 32UL << USIC_CH_PCR_SSCMode_SELO_Pos, /**< Slave Select line 5 */
  XMC_SPI_CH_SLAVE_SELECT_6 = 64UL << USIC_CH_PCR_SSCMode_SELO_Pos, /**< Slave Select line 6 */
  XMC_SPI_CH_SLAVE_SELECT_7 = 128UL << USIC_CH_PCR_SSCMode_SELO_Pos /**< Slave Select line 7 */
} XMC_SPI_CH_SLAVE_SELECT_t;

/**
 * Defines SPI specific events
 */
typedef enum XMC_SPI_CH_EVENT
{
  XMC_SPI_CH_EVENT_RECEIVE_START       = USIC_CH_CCR_RSIEN_Msk, /**< Receive start event */
  XMC_SPI_CH_EVENT_DATA_LOST           = USIC_CH_CCR_DLIEN_Msk, /**< Data lost event */
  XMC_SPI_CH_EVENT_TRANSMIT_SHIFT      = USIC_CH_CCR_TSIEN_Msk, /**< Transmit shift event */
  XMC_SPI_CH_EVENT_TRANSMIT_BUFFER     = USIC_CH_CCR_TBIEN_Msk, /**< Transmit buffer event */
  XMC_SPI_CH_EVENT_STANDARD_RECEIVE    = USIC_CH_CCR_RIEN_Msk,  /**< Receive event */
  XMC_SPI_CH_EVENT_ALTERNATIVE_RECEIVE = USIC_CH_CCR_AIEN_Msk,  /**< Alternate receive event */
  XMC_SPI_CH_EVENT_BAUD_RATE_GENERATOR = USIC_CH_CCR_BRGIEN_Msk, /**< Baudrate generator event */

  XMC_SPI_CH_EVENT_PARITY_ERROR = USIC_CH_PCR_SSCMode_PARIEN_Msk >> 13U,       /**< Parity error event */
  XMC_SPI_CH_EVENT_MSLS_CHANGE =  USIC_CH_PCR_SSCMode_MSLSIEN_Msk >> 13U,      /**< Master slave select(MSLS) output transition event*/
  XMC_SPI_CH_EVENT_DX2TIEN_ACTIVATED = USIC_CH_PCR_SSCMode_DX2TIEN_Msk >> 13U  /**< Slave select input signal transition event*/
} XMC_SPI_CH_EVENT_t;

/**
 * Defines SPI event status
 */
typedef enum XMC_SPI_CH_STATUS_FLAG
{
  XMC_SPI_CH_STATUS_FLAG_MSLS = USIC_CH_PSR_SSCMode_MSLS_Msk,                          /**< Status of Master slave 
                                                                                            select(MSLS) signal */
  XMC_SPI_CH_STATUS_FLAG_DX2S = USIC_CH_PSR_SSCMode_DX2S_Msk,                          /**< Status of slave select
                                                                                            input(DX2) signal*/ 
  XMC_SPI_CH_STATUS_FLAG_MSLS_EVENT_DETECTED = USIC_CH_PSR_SSCMode_MSLSEV_Msk,         /**< Status for master slave select
                                                                                            output signal transition*/
  XMC_SPI_CH_STATUS_FLAG_DX2T_EVENT_DETECTED = USIC_CH_PSR_SSCMode_DX2TEV_Msk,         /**< Status for slave select 
                                                                                            input signal transition */
  XMC_SPI_CH_STATUS_FLAG_PARITY_ERROR_EVENT_DETECTED = USIC_CH_PSR_SSCMode_PARERR_Msk, /**< Indicates status of the 
                                                                                            parity error */
  XMC_SPI_CH_STATUS_FLAG_RECEIVER_START_INDICATION = USIC_CH_PSR_SSCMode_RSIF_Msk,     /**< Status for receive start
                                                                                            event */
  XMC_SPI_CH_STATUS_FLAG_DATA_LOST_INDICATION = USIC_CH_PSR_SSCMode_DLIF_Msk,          /**< Status for data lost event*/
  XMC_SPI_CH_STATUS_FLAG_TRANSMIT_SHIFT_INDICATION = USIC_CH_PSR_SSCMode_TSIF_Msk,      /**< Status for transmit shift
                                                                                            event */
  XMC_SPI_CH_STATUS_FLAG_TRANSMIT_BUFFER_INDICATION = USIC_CH_PSR_SSCMode_TBIF_Msk,    /**< Status for transmit buffer 
                                                                                            event */
  XMC_SPI_CH_STATUS_FLAG_RECEIVE_INDICATION = USIC_CH_PSR_SSCMode_RIF_Msk,             /**< Status for receive event */
  XMC_SPI_CH_STATUS_FLAG_ALTERNATIVE_RECEIVE_INDICATION = USIC_CH_PSR_SSCMode_AIF_Msk, /**< Status for alternative 
                                                                                            receive event */
  XMC_SPI_CH_STATUS_FLAG_BAUD_RATE_GENERATOR_INDICATION = USIC_CH_PSR_SSCMode_BRGIF_Msk/**< Status for baud rate 
                                                                                            generation error event */
} XMC_SPI_CH_STATUS_FLAG_t;

/**
 * Defines input frequency sources for slave select signal delay configuration.
 */
typedef enum XMC_SPI_CH_INPUT_FREQ_SLAVE_SELECT_DELAY
{
  XMC_SPI_CH_INPUT_FREQ_SLAVE_SELECT_DELAY_FPDIV = 0x0UL, /**< Output of PDIV divider: FPDIV */
  XMC_SPI_CH_INPUT_FREQ_SLAVE_SELECT_DELAY_FPPP  = 0x1UL << USIC_CH_PCR_SSCMode_CTQSEL1_Pos, /**< Peripheral clock: FPPP */
  XMC_SPI_CH_INPUT_FREQ_SLAVE_SELECT_DELAY_FSCLK = 0x2UL << USIC_CH_PCR_SSCMode_CTQSEL1_Pos, /**< Shift clock: FSCLK */
  XMC_SPI_CH_INPUT_FREQ_SLAVE_SELECT_DELAY_FMCLK = 0x3UL << USIC_CH_PCR_SSCMode_CTQSEL1_Pos  /**< Master clock: FMCLK */
} XMC_SPI_CH_INPUT_FREQ_SLAVE_SELECT_DELAY_t;

/**
 * Define data and clock input stages
 */
typedef enum XMC_SPI_CH_INPUT
{
  XMC_SPI_CH_INPUT_DIN0 = 0UL,         /**< Data input stage 0 */ 
  XMC_SPI_CH_INPUT_SLAVE_SCLKIN = 1UL, /**< Clock input stage */
  XMC_SPI_CH_INPUT_SLAVE_SELIN = 2UL,  /**< Slave select input stage */
  XMC_SPI_CH_INPUT_DIN1 = 3UL,         /**< Data input stage 1 */
  XMC_SPI_CH_INPUT_DIN2 = 4UL,         /**< Data input stage 2 */
  XMC_SPI_CH_INPUT_DIN3 = 5UL          /**< Data input stage 3 */
} XMC_SPI_CH_INPUT_t;

/**
 * Define SPI data transfer mode
 */
typedef enum XMC_SPI_CH_MODE
{
  XMC_SPI_CH_MODE_STANDARD = 0UL,            /**< SPI standard full duplex mode */ 
  XMC_SPI_CH_MODE_STANDARD_HALFDUPLEX = 4UL, /**< SPI standard half duplex mode */ 
  XMC_SPI_CH_MODE_DUAL= 6UL,                 /**< SPI half duplex mode with dual data lines */ 
  XMC_SPI_CH_MODE_QUAD= 7UL                  /**< SPI half duplex mode with quad data lines */
} XMC_SPI_CH_MODE_t;


/**
 * SPI Baudrate Generator shift clock passive level
 */
typedef enum XMC_SPI_CH_BRG_SHIFT_CLOCK_PASSIVE_LEVEL
{
  /**< Passive clock level 0, delay disabled */
  XMC_SPI_CH_BRG_SHIFT_CLOCK_PASSIVE_LEVEL_0_DELAY_DISABLED = XMC_USIC_CH_BRG_SHIFT_CLOCK_PASSIVE_LEVEL_0_DELAY_DISABLED,
  /**< Passive clock level 1, delay disabled */
  XMC_SPI_CH_BRG_SHIFT_CLOCK_PASSIVE_LEVEL_1_DELAY_DISABLED = XMC_USIC_CH_BRG_SHIFT_CLOCK_PASSIVE_LEVEL_1_DELAY_DISABLED,
  /**< Passive clock level 0, delay enabled */
  XMC_SPI_CH_BRG_SHIFT_CLOCK_PASSIVE_LEVEL_0_DELAY_ENABLED  = XMC_USIC_CH_BRG_SHIFT_CLOCK_PASSIVE_LEVEL_0_DELAY_ENABLED,
  /**< Passive clock level 1, delay enabled */
  XMC_SPI_CH_BRG_SHIFT_CLOCK_PASSIVE_LEVEL_1_DELAY_ENABLED  = XMC_USIC_CH_BRG_SHIFT_CLOCK_PASSIVE_LEVEL_1_DELAY_ENABLED
} XMC_SPI_CH_BRG_SHIFT_CLOCK_PASSIVE_LEVEL_t;

/**
 * SPI Baudrate Generator shift clock output
*/
typedef enum XMC_SPI_CH_BRG_SHIFT_CLOCK_OUTPUT
{
  XMC_SPI_CH_BRG_SHIFT_CLOCK_OUTPUT_SCLK = XMC_USIC_CH_BRG_SHIFT_CLOCK_OUTPUT_SCLK,  /**< Baudrate Generator shift clock output: SCLK*/
  XMC_SPI_CH_BRG_SHIFT_CLOCK_OUTPUT_DX1  = XMC_USIC_CH_BRG_SHIFT_CLOCK_OUTPUT_DX1   /**< Clock obtained as input from master: DX1*/
} XMC_SPI_CH_BRG_SHIFT_CLOCK_OUTPUT_t;

/**
 * SPI channel interrupt node pointers
 */
typedef enum XMC_SPI_CH_INTERRUPT_NODE_POINTER
{
  XMC_SPI_CH_INTERRUPT_NODE_POINTER_TRANSMIT_SHIFT      = XMC_USIC_CH_INTERRUPT_NODE_POINTER_TRANSMIT_SHIFT, /**< Node pointer for transmit shift interrupt */
  XMC_SPI_CH_INTERRUPT_NODE_POINTER_TRANSMIT_BUFFER     = XMC_USIC_CH_INTERRUPT_NODE_POINTER_TRANSMIT_BUFFER, /**< Node pointer for transmit buffer interrupt */
  XMC_SPI_CH_INTERRUPT_NODE_POINTER_RECEIVE             = XMC_USIC_CH_INTERRUPT_NODE_POINTER_RECEIVE,  /**< Node pointer for receive interrupt */
  XMC_SPI_CH_INTERRUPT_NODE_POINTER_ALTERNATE_RECEIVE   = XMC_USIC_CH_INTERRUPT_NODE_POINTER_ALTERNATE_RECEIVE,  /**< Node pointer for alternate receive interrupt */
  XMC_SPI_CH_INTERRUPT_NODE_POINTER_PROTOCOL            = XMC_USIC_CH_INTERRUPT_NODE_POINTER_PROTOCOL   /**< Node pointer for protocol related interrupts */
} XMC_SPI_CH_INTERRUPT_NODE_POINTER_t;

/**********************************************************************************************************************
 * DATA STRUCTURES
**********************************************************************************************************************/

/**
 * Structure for initializing SPI channel.
 */
typedef struct XMC_SPI_CH_CONFIG
{
  uint32_t baudrate;							  /**< Module baud rate for communication */
  XMC_SPI_CH_BUS_MODE_t bus_mode;                 /**< Bus mode: Master/Slave */
  XMC_SPI_CH_SLAVE_SEL_MSLS_INV_t selo_inversion; /**< Enable inversion of Slave select signal relative to the internal 
                                                       MSLS signal  */
  XMC_USIC_CH_PARITY_MODE_t parity_mode;          /**< Enable parity check for transmit and received data */
} XMC_SPI_CH_CONFIG_t;

/**********************************************************************************************************************
 * API PROTOTYPES
 *********************************************************************************************************************/

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @param channel A constant pointer to XMC_USIC_CH_t, pointing to the USIC channel base address.
 * @param config pointer to constant SPI channel configuration data structure.\n
 *               Refer data structure @ref XMC_SPI_CH_CONFIG_t for detail.
 *
 * @return None
 *
 * \par<b>Description:</b><br>
 * Initializes the selected SPI \a channel with the \a config structure.\n\n
 * Enable SPI channel by calling XMC_USIC_CH_Enable() and then configures
 * <ul>
 * <li>Baudrate,</li>
 * <li>Passive data level as active high,</li>
 * <li>Shift control signal as active high,</li>
 * <li>Frame length as 64U,</li>
 * <li>Word length as 8U,</li>
 * <li>Enable Hardware port control mode,</li>
 * <li>Enable transmission of data TDV(Transmit data valid) bit is set to 1,</li>
 * <li>Enable invalidation of data in TBUF once loaded into shift register,</li>
 * <li>Parity mode settings</li>
 * </ul>
 * And if master mode is selected,
 * <ul>
 * <li>Enables MSLS signal generation,</li>
 * <li>configures slave selection as normal mode,</li>
 * <li>Set polarity for the Slave signal,</li>
 * <li>Enable Frame end mode(MSLS signal is kept active after transmission of a frame)</li>
 * </ul>
 */
void XMC_SPI_CH_Init(XMC_USIC_CH_t *const channel, const XMC_SPI_CH_CONFIG_t *const config);

/**
 * @param channel A constant pointer to XMC_USIC_CH_t, pointing to the USIC channel base address.
 *
 * @return None
 *
 * \par<b>Description:</b><br>
 * Set the selected USIC channel to operate in SPI mode, by setting CCR.MODE bits.\n\n
 * It should be executed after XMC_SPI_CH_Init() during initialization. By invoking XMC_SPI_CH_Stop(), the MODE is set
 * to IDLE state. Call XMC_SPI_CH_Start() to set the SPI mode again, as needed later in the program.
 *
 * \par<b>Related APIs:</b><BR>
 * XMC_SPI_CH_Init(), XMC_SPI_CH_Stop()
 */
__STATIC_INLINE void XMC_SPI_CH_Start(XMC_USIC_CH_t *const channel)
{
  /* USIC channel in SPI mode */
  XMC_USIC_CH_SetMode(channel, XMC_USIC_CH_OPERATING_MODE_SPI);
}

/**
 * @param channel A constant pointer to XMC_USIC_CH_t, pointing to the USIC channel base address.
 *
 * @return XMC_SPI_CH_STATUS_t Status of the SPI driver after the request for stopping is processed. \n
 *        XMC_SPI_CH_STATUS_OK- If the USIC channel is successfully put to IDLE mode. \n
 *        XMC_SPI_CH_STATUS_BUSY- If the USIC channel is busy transmitting data.
 *
 * \par<b>Description:</b><br>
 * Set the selected SPI channel to IDLE mode, by clearing CCR.MODE bits.\n\n
 * After calling XMC_SPI_CH_Stop, channel is IDLE mode. So no communication is supported. XMC_SPI_CH_Start() has to be 
 * invoked to start the communication again.
 *
 * \par<b>Related APIs:</b><BR>
 * XMC_SPI_CH_Start()
 */
XMC_SPI_CH_STATUS_t XMC_SPI_CH_Stop(XMC_USIC_CH_t *const channel);

/**
 * @param channel A constant pointer to XMC_USIC_CH_t, pointing to the USIC channel base address.
 * @param rate Bus speed in bits per second
 *
 * @return XMC_SPI_CH_STATUS_t Status of the SPI driver after the request for setting baudrate is processed. \n
 *        XMC_SPI_CH_STATUS_OK- If the baudrate is successfully changed. \n
 *        XMC_SPI_CH_STATUS_ERROR- If the new baudrate value is out of range.
 *
 * \par<b>Description:</b><br>
 * Sets the bus speed in bits per second
 *
 * \par<b>Related APIs:</b><BR>
 * XMC_SPI_CH_Init(), XMC_SPI_CH_Stop()
 */
XMC_SPI_CH_STATUS_t XMC_SPI_CH_SetBaudrate(XMC_USIC_CH_t *const channel, const uint32_t rate);

/**
 * @param channel A constant pointer to XMC_USIC_CH_t, pointing to the USIC channel base address.
 * @param slave Slave select signal.\n
 *              Refer @ref XMC_SPI_CH_SLAVE_SELECT_t for valid values.
 *
 * @return None
 *
 * \par<b>Description:</b><br>
 * Enable the selected slave signal by setting PCR.SELO bits.\n\n
 * Each slave is connected with one slave select signal. This is not configured in XMC_SPI_CH_Init(). Invoke
 * XMC_SPI_CH_EnableSlaveSelect() with required \a slave to to start the communication. After finishing the
 * communication XMC_SPI_CH_DisableSlaveSelect() can be invoked to disable the slaves.
 *
 * \par<b>Related APIs:</b><BR>
 * XMC_SPI_CH_DisableSlaveSelect()
 */
void XMC_SPI_CH_EnableSlaveSelect(XMC_USIC_CH_t *const channel, const XMC_SPI_CH_SLAVE_SELECT_t slave);

/**
 * @param channel A constant ponter to XMC_USIC_CH_t, pointing to the USIC channel base address.
 *
 * @return None
 *
 * \par<b>Description:</b><br>
 * Disable all the slave signals by clearing PCR.SELO bits.\n\n
 * XMC_SPI_CH_EnableSlaveSelect() has to be invoked to start the communication with the desired slave again.
 *
 * \par<b>Related APIs:</b><BR>
 * XMC_SPI_CH_EnableSlaveSelect()
 */
void XMC_SPI_CH_DisableSlaveSelect(XMC_USIC_CH_t *const channel);

/**
 * @param channel A constant pointer to XMC_USIC_CH_t, pointing to the USIC channel base address.
 * @param mode Communication mode of the SPI, based on this mode TCI(Transmit control information)is updated.\n
 *             Refer @ref XMC_SPI_CH_MODE_t for valid values.
 *
 * @return None
 *
 * \par<b>Description:</b><br>
 * In Dual and Quad modes,  hardware port control(CCR.HPCEN) mode is enabled. \n\n
 * By enabling this the direction of the data pin is updated by hardware itself. Before transmitting the data set the 
 * mode to ensure the proper communication.
 * 
 * \par<b>Related APIs:</b><BR>
 * XMC_SPI_CH_Transmit()
 */
__STATIC_INLINE void XMC_SPI_CH_SetTransmitMode(XMC_USIC_CH_t *const channel, const XMC_SPI_CH_MODE_t mode)
{
  channel->CCR = (channel->CCR & (uint32_t)(~USIC_CH_CCR_HPCEN_Msk)) |
                  (((uint32_t) mode << USIC_CH_CCR_HPCEN_Pos) & (uint32_t)USIC_CH_CCR_HPCEN_Msk);
}

/**
 * @param channel A constant pointer to XMC_USIC_CH_t, pointing to the USIC channel base address.
 * @param data Data to be transmitted
 * @param mode Communication mode of the SPI, based on this mode TCI(Transmit control information)is updated.\n
 *             Refer @ref XMC_SPI_CH_MODE_t for valid values.
 *
 * @return None
 *
 * \par<b>Description:</b><br>
 * Puts the data into FIFO, if FIFO mode is enabled or else into standard buffer, by setting the proper mode.\n\n
 * In Dual and Quad modes,  hardware port control(CCR.HPCEN) mode is enabled. By enabling this the direction of the data
 * pin is updated by hardware itself. TCI(Transmit Control Information) allows dynamic control of both the data shift mode
 * and pin direction during data transfers by writing to SCTR.DSM and SCTR.HPCDIR bit fields. To support this auto
 * update, TCSR.HPCMD(Hardware Port control) will be enabled during the initialization using XMC_SPI_CH_Init() for all modes.
 * 
 * 
 * \par<b>Related APIs:</b><BR>
 * XMC_SPI_CH_Receive()
 */
void XMC_SPI_CH_Transmit(XMC_USIC_CH_t *const channel, const uint16_t data, const XMC_SPI_CH_MODE_t mode);

/**
 * @param channel A constant pointer to XMC_USIC_CH_t, pointing to the USIC channel base address.
 * @param mode Communication mode of the SPI, based on this mode TCI(Transmit control information)is updated.\n
 *             Refer @ref XMC_SPI_CH_MODE_t for valid values.      
 *
 * @return None
 *
 * \par<b>Description:</b><br>
 * Transmits a dummy data(FFFFH) to provide clock for slave and receives the data from the slave.\n\n
 * XMC_SPI_CH_Receive() receives the data and places it into buffer based on the FIFO selection. After reception of data 
 * XMC_SPI_CH_GetReceivedData() can be invoked to read the data from the buffers.
 * 
 * \par<b>Related APIs:</b><BR>
 * XMC_SPI_CH_GetReceivedDaa()
 */
__STATIC_INLINE void XMC_SPI_CH_Receive(XMC_USIC_CH_t *const channel, const XMC_SPI_CH_MODE_t mode)
{
  /* Transmit dummy data */
  XMC_SPI_CH_Transmit(channel, (uint16_t)0xffffU, (XMC_SPI_CH_MODE_t)((uint16_t)mode & 0xfffbU));
}

/**
 * @param channel A constant pointer to XMC_USIC_CH_t, pointing to the USIC channel base address.
 *
 * @return uint16_t Data read from the receive buffer.
 *
 * \par<b>Description:</b><br>
 * Reads data from the receive buffer based on the FIFO selection.\n\n
 * Invocation of XMC_SPI_CH_Receive() receives the data and place it into receive buffer. After receiving the data
 * XMC_SPI_CH_GetReceivedData() can be used to read the data from the buffer.
 * 
 * \par<b>Related APIs:</b><BR>
 * XMC_SPI_CH_Receive()
 */
uint16_t XMC_SPI_CH_GetReceivedData(XMC_USIC_CH_t *const channel);

/**
 * @param channel A constant pointer to XMC_USIC_CH_t, pointing to the USIC channel base address.
 *
 * @return None
 *
 * \par<b>Description:</b><br>
 * Set the order of data transfer from LSB to MSB, by clearing SCTR.SDIR bit.\n\n
 * This is typically based on the slave settings. Invoke XMC_SPI_CH_SetBitOrderLsbFirst() to set direction as needed in
 * the program.
 *
 * \par<b>Related APIs:</b><BR>
 * XMC_SPI_CH_SetBitOrderMsbFirst()
 */
__STATIC_INLINE void XMC_SPI_CH_SetBitOrderLsbFirst(XMC_USIC_CH_t *const channel)
{
  channel->SCTR &= (uint32_t)~USIC_CH_SCTR_SDIR_Msk;
}

/**
 * @param channel A constant pointer to XMC_USIC_CH_t, pointing to the USIC channel base address.
 *
 * @return None
 *
 * \par<b>Description:</b><br>
 * Set the order of data transfer from MSB to LSB, by setting SCTR.SDIR bit.\n\n
 * This is typically based on the slave settings. This is not set during XMC_SPI_CH_Init(). 
 * Invoke XMC_SPI_CH_SetBitOrderMsbFirst() to set direction as needed in the program.
 *
 * \par<b>Related APIs:</b><BR>
 * XMC_SPI_CH_SetBitOrderLsbFirst()
 */
__STATIC_INLINE void XMC_SPI_CH_SetBitOrderMsbFirst(XMC_USIC_CH_t *const channel)
{
  channel->SCTR |= (uint32_t)USIC_CH_SCTR_SDIR_Msk;
}

/**
 * @param channel A constant pointer to XMC_USIC_CH_t, pointing to the USIC channel base address.
 * @param event Protocol events which have to be enabled.
 *              Refer @ XMC_SPI_CH_EVENT_t for valid values. <b>OR</b> combinations of these enum items can be used 
 *              as input.
 *
 * @return None
 *
 * \par<b>Description:</b><br>
 * Enables the SPI protocol specific events, by configuring PCR register.\n\n
 * Events can be enabled as needed using XMC_SPI_CH_EnableEvent(). 
 * XMC_SPI_CH_DisableEvent() can be used to disable the events.
 *
 * \par<b>Related APIs:</b><BR>
 * XMC_SPI_CH_DisableEvent()
 */
void XMC_SPI_CH_EnableEvent(XMC_USIC_CH_t *const channel, const uint32_t event);

/**
 * @param channel A constant pointer to XMC_USIC_CH_t, pointing to the USIC channel base address.
 * @param event Protocol events which have to be disabled.
 *              Refer @ XMC_SPI_CH_EVENT_t for valid values. <b>OR</b> combinations of these enum item can be used 
 *              as input.
 *
 * @return None
 *
 * \par<b>Description:</b><br>
 * Disables the SPI protocol specific events, by configuring PCR register.\n\n
 * After disabling the events, XMC_SPI_CH_EnableEvent() has to be invoked to re-enable the events.
 *
 * \par<b>Related APIs:</b><BR>
 * XMC_SPI_CH_EnableEvent()
 */
void XMC_SPI_CH_DisableEvent(XMC_USIC_CH_t *const channel, const uint32_t event);

/**
 * @param channel A constant pointer to XMC_USIC_CH_t, pointing to the USIC channel base address.
 *
 * @return uint32_t Status of SPI protocol events.
 *
 * \par<b>Description:</b><br>
 * Returns the status of the events, by reading PSR register.\n\n
 * This indicates the status of the all the events, for SPI communication. 
 *
 * \par<b>Related APIs:</b><BR>
 * XMC_SPI_CH_ClearStatusFlag()
 */
__STATIC_INLINE uint32_t XMC_SPI_CH_GetStatusFlag(XMC_USIC_CH_t *const channel)
{
  return channel->PSR_SSCMode;
}

/**
 * @param channel A constant pointer to XMC_USIC_CH_t, pointing to the USIC channel base address.
 * @param flag Protocol event status to be cleared for detection of next occurence.
 *              Refer @ XMC_SPI_CH_STATUS_FLAG_t for valid values. <b>OR</b> combinations of these enum item can be used 
 *              as input.
 * @return None
 *
 * \par<b>Description:</b><br>
 * Clears the events specified, by setting PSCR register.\n\n
 * During communication the events occurred have to be cleared to detect their next occurence.\n
 * e.g: During transmission Transmit buffer event occurs to indicating data word transfer has started. This
 *       event has to be cleared after transmission of each data word. Otherwise next event cannot be recognized.
 *
 * \par<b>Related APIs:</b><BR>
 * XMC_SPI_CH_GetStatusFlag()
 */
__STATIC_INLINE void XMC_SPI_CH_ClearStatusFlag(XMC_USIC_CH_t *const channel, const uint32_t flag)
{
  channel->PSCR |= flag;
}

/**
 * @param channel A constant pointer to XMC_USIC_CH_t, pointing to the USIC channel base address.
 *
 * @return None
 *
 * \par<b>Description:</b><br>
 * Enables the generation of Master clock by setting PCR.MCLK bit.\n\n
 * This clock can be used as a clock reference for external devices. This is not enabled during initialization in 
 * XMC_SPI_CH_Init(). Invoke XMC_SPI_CH_EnableMasterClock() to enable as needed in the program, or if it is disabled by
 * XMC_SPI_CH_DisableMasterClock().
 *
 * \par<b>Related APIs:</b><BR>
 * XMC_SPI_CH_DisableMasterClock()
 */
__STATIC_INLINE void XMC_SPI_CH_EnableMasterClock(XMC_USIC_CH_t *const channel)
{
  channel->PCR_SSCMode |= (uint32_t)USIC_CH_PCR_SSCMode_MCLK_Msk;
}

/**
 * @param channel A constant pointer to XMC_USIC_CH_t, pointing to the USIC channel base address.
 *
 * @return None
 *
 * \par<b>Description:</b><br>
 * Disables the generation of Master clock by clearing PCR.MCLK bit.\n\n
 * This clock can be enabled by invoking XMC_SPI_CH_EnableMasterClock() as needed in the program.
 *
 * \par<b>Related APIs:</b><BR>
 * XMC_SPI_CH_EnableMasterClock()
 */
__STATIC_INLINE void XMC_SPI_CH_DisableMasterClock(XMC_USIC_CH_t *const channel)
{
  channel->PCR_SSCMode &= (uint32_t)~USIC_CH_PCR_SSCMode_MCLK_Msk;
}
#ifdef USIC_CH_PCR_SSCMode_SLPHSEL_Msk
/**
 * @param channel A constant pointer to XMC_USIC_CH_t, pointing to the USIC channel base address.
 *
 * @return None
 *
 * \par<b>Description:</b><br>
 * Data bits are shifted out with the leading edge of the shift clock signal and latched in with the trailing edge.
 *
 * \par<b>Related APIs:</b><BR>
 * XMC_SPI_CH_DataLatchedInLeadingEdge()
 */
__STATIC_INLINE void XMC_SPI_CH_DataLatchedInTrailingEdge(XMC_USIC_CH_t *const channel)
{
  channel->PCR_SSCMode &= (uint32_t)~USIC_CH_PCR_SSCMode_SLPHSEL_Msk;
}

/**
 * @param channel A constant pointer to XMC_USIC_CH_t, pointing to the USIC channel base address.
 *
 * @return None
 *
 * \par<b>Description:</b><br>
 * The first data bit is shifted out when the data shift unit receives a low to high transition from the DX2
 * stage. Subsequent bits are shifted out with the trailing edge of the shift clock signal. Data bits are
 * always latched in with the leading edge.
 *
 * \par<b>Related APIs:</b><BR>
 * XMC_SPI_CH_DataLatchedInTrailingEdge()
 */
__STATIC_INLINE void XMC_SPI_CH_DataLatchedInLeadingEdge(XMC_USIC_CH_t *const channel)
{
  channel->PCR_SSCMode |= USIC_CH_PCR_SSCMode_SLPHSEL_Msk;
}
#endif
/**
 * @param channel A constant pointer to XMC_USIC_CH_t, pointing to the USIC channel base address.
 *
 * @return None
 *
 * \par<b>Description:</b><br>
 * Enables the delay after each word, by setting PCR.TIWEN bit.\n\n
 * The inter word delay starts at the end of last SCLK cycle of data word. During this time no clock pulses are 
 * generated and MSLS signal stays active. If inter word delay is not enabled, last data bit of a data word is directly 
 * followed by the first data bit of the next data word. This is not enabled in XMC_SPI_CH_Init(). To enable 
 * XMC_SPI_CH_EnableInterwordDelay() has to be invoked as needed in the program. And can be disabled by invoking
 * XMC_SPI_CH_DisableInterwordDelay().
 * 
 * \par<b>Related APIs:</b><BR>
 * XMC_SPI_CH_DisableInterwordDelay()
 */
__STATIC_INLINE void XMC_SPI_CH_EnableInterwordDelay(XMC_USIC_CH_t *const channel)
{
  channel->PCR_SSCMode |= (uint32_t)USIC_CH_PCR_SSCMode_TIWEN_Msk;
}

/**
 * @param channel A constant pointer to XMC_USIC_CH_t, pointing to the USIC channel base address.
 *
 * @return None
 *
 * \par<b>Description:</b><br>
 * Disables the delay after after each word, by clearing PCR.TIWEN bit.\n\n
 * So the last data bit of a data word is directly followed by the first data bit of the next data word. If needed can 
 * be enabled by invoking XMC_SPI_CH_EnableInterwordDelay().
 *
 * \par<b>Related APIs:</b><BR>
 * XMC_SPI_CH_EnableInterwordDelay()
 */
__STATIC_INLINE void XMC_SPI_CH_DisableInterwordDelay(XMC_USIC_CH_t *const channel)
{
  channel->PCR_SSCMode &= (uint32_t)~USIC_CH_PCR_SSCMode_TIWEN_Msk;
}

/**
 * @param channel A constant pointer to XMC_USIC_CH_t, pointing to the USIC channel base address.
 * @param tinterword_delay_ns delay in terms of nano seconds.
 *
 * @return None
 *
 * \par<b>Description:</b><br>
 * Configures the inter word delay by setting PCR.PCTQ1, PCR.DCTQ1 bit fields.\n\n
 * The inter word delay is dependent on the peripheral clock. The maximum possible value is calculated by using the 
 * below formula\n
 *            Maximum inter word delay = ((1 + PCTQ1_max)(1 + DCTQ1_max)) / peripheral clock\n
 *                                       where PCTQ1_max = 3 and DCTQ1_max = 31\n
 * After configuring the inter word delay, this has to be enabled by invoking XMC_SPI_CH_EnableInterwordDelay().
 * 
 * \par<b>Related APIs:</b><BR>
 * XMC_SPI_CH_EnableInterwordDelay(),XMC_SPI_CH_SetInterwordDelaySCLK()
 */
void XMC_SPI_CH_SetInterwordDelay(XMC_USIC_CH_t *const channel,uint32_t tinterword_delay_ns);

/**
 * @param channel A constant pointer to XMC_USIC_CH_t, pointing to the USIC channel base address.
 * @param sclk_period  in terms of clk cycles.
 *
 * @return None
 *
 * \par<b>Description:</b><br>
 * Configures the inter word delay by setting PCR.DCTQ1 bit fields.\n\n
 * This delay is dependent on the peripheral clock. The maximum possible value supported by this API
 * is 32 clock cycles.
 * After configuring the inter word delay, this has to be enabled by invoking XMC_SPI_CH_EnableInterwordDelay().
 *
 * \par<b>Related APIs:</b><BR>
 * XMC_SPI_CH_EnableInterwordDelay(),XMC_SPI_CH_EnableInterwordDelay()
 */
__STATIC_INLINE void XMC_SPI_CH_SetInterwordDelaySCLK(XMC_USIC_CH_t *const channel,uint32_t sclk_period)
{
  channel->PCR_SSCMode = (uint32_t)((channel->PCR_SSCMode) & (~(USIC_CH_PCR_SSCMode_DCTQ1_Msk |
                                                                USIC_CH_PCR_SSCMode_PCTQ1_Msk |
                                                                USIC_CH_PCR_SSCMode_CTQSEL1_Msk))) |
                         (((sclk_period - 1U) <<  USIC_CH_PCR_SSCMode_DCTQ1_Pos) |
                         (0x02U <<  USIC_CH_PCR_SSCMode_CTQSEL1_Pos));
}

/**
 * @param channel A constant pointer to XMC_USIC_CH_t, pointing to the USIC channel base address.
 * @param sclk_period delay in terms of sclk clock cycles.
 *
 * @return None
 *
 * \par<b>Description:</b><br>
 * Configures the leading/trailing delay by setting BRG.DCTQ bit field.\n\n
 * This delay is dependent on the peripheral clock. The maximum possible value supported by this API
 * is 30 clock cycles.
 *
 */
__STATIC_INLINE void XMC_SPI_CH_SetSlaveSelectDelay(XMC_USIC_CH_t *const channel,uint32_t sclk_period)
{

  channel->BRG = (channel->BRG & ~(USIC_CH_BRG_DCTQ_Msk |
								   USIC_CH_BRG_PCTQ_Msk)) |
                 (((sclk_period - 1U) << USIC_CH_BRG_DCTQ_Pos) | (0x01U << USIC_CH_BRG_PCTQ_Pos));
}

/**
 * @param channel A constant pointer to XMC_USIC_CH_t, pointing to the USIC channel base address.
 *
 * @return None
 *
 * \par<b>Description:</b><br>
 *
 * Configure to keep MSLS(Slave select signal) active even after finishing the current data frame,
 * by setting PCR.FEM bit.\n\n
 * This is typically used during the transmission of multi-data word frames, where there is possibility of delay in 
 * delivering the data. Frame end mode is enabled in XMC_SPI_CH_Init() during initialization. To disable
 * XMC_SPI_CH_DisableFEM() can be invoked as needed in the program.
 * 
 * \par<b>Related APIs:</b><BR>
 * XMC_SPI_CH_DisableFEM()
 */
__STATIC_INLINE void XMC_SPI_CH_EnableFEM(XMC_USIC_CH_t *const channel)
{
  channel->PCR_SSCMode |= (uint32_t)USIC_CH_PCR_SSCMode_FEM_Msk;
}

/**
 * @param channel A constant pointer to XMC_USIC_CH_t, pointing to the USIC channel base address.
 *
 * @return None
 *
 * \par<b>Description:</b><br>
 * Configure to disable the MSLS(Slave select signal) if the current data frame is considered as finished,
 * by setting PCR.FEM bit.\n\n
 *
 * When the last bit of a data word has been sent out and the transmit buffer TBUF does not contain new data, is 
 * considered as frame is ended and MSLS(Slave select signal) is disabled.
 * 
 * \par<b>Related APIs:</b><BR>
 * XMC_SPI_CH_EnableFEM()
 */
__STATIC_INLINE void XMC_SPI_CH_DisableFEM(XMC_USIC_CH_t *const channel)
{
  channel->PCR_SSCMode &= (uint32_t)~USIC_CH_PCR_SSCMode_FEM_Msk;
}

/**
 * @param channel A constant pointer to XMC_USIC_CH_t, pointing to the USIC channel base address.
 * @param passive_level polarity and delay of the selected shift clock.\n
 *                      Refer @ref XMC_SPI_CH_BRG_SHIFT_CLOCK_PASSIVE_LEVEL_t for valid inputs.
 * @param clock_output shift clock source.\n
 *                     Refer @ref XMC_SPI_CH_BRG_SHIFT_CLOCK_OUTPUT_t for valid inputs.
 *
 * @return None
 *
 * \par<b>Description:</b><br>
 * Configures the shift clock source with the selected polarity and delay by setting BRG.SCLKOSEL and BRG.SCLKCFG.\n\n
 * In Master mode operation, shift clock is generated by the internal baud rate generator. This SCLK is made available
 * for external slave devices by SCLKOUT signal.\n
 * In Slave mode, the signal is received from the external master. So the DX1(input) stage has to be connected to input.\n
 * The shift clock output(SCLKOUT) signal polarity can be set relative to SCLK, with the delay of half the shift clock 
 * period. These settings are applicable only in master mode.
 */
__STATIC_INLINE void XMC_SPI_CH_ConfigureShiftClockOutput(XMC_USIC_CH_t *const channel,
                                                          const XMC_SPI_CH_BRG_SHIFT_CLOCK_PASSIVE_LEVEL_t passive_level,
                                                          const XMC_SPI_CH_BRG_SHIFT_CLOCK_OUTPUT_t clock_output)
{
  XMC_USIC_CH_ConfigureShiftClockOutput(channel, (XMC_USIC_CH_BRG_SHIFT_CLOCK_PASSIVE_LEVEL_t)passive_level,
                                       (XMC_USIC_CH_BRG_SHIFT_CLOCK_OUTPUT_t)clock_output);
}

/**
 * @param  channel A constant pointer to XMC_USIC_CH_t, pointing to the USIC channel base address.
 * @param  word_length Number of bits to be configured for a data word. \n
 *         \b Range: 1 to 16.
 *
 * @return None
 *
 * \par<b>Description</b><br>
 * Defines the data word length.\n\n
 * Sets the number of bits to represent a data word. Frame length should be a multiple of word length.
 *
 * \par<b>Related APIs:</b><BR>
 * XMC_SPI_CH_SetFrameLength()
 */
__STATIC_INLINE void XMC_SPI_CH_SetWordLength(XMC_USIC_CH_t *const channel, const uint8_t word_length)
{
  XMC_USIC_CH_SetWordLength(channel, word_length);
}

/**
 * @param  channel A constant pointer to XMC_USIC_CH_t, pointing to the USIC channel base address.
 * @param  frame_length Number of bits in a frame. \n
 *                \b Range: 1 to 64. If the value 64 is configured, then the frame does not
 *                automatically end. User should explicitly end the frame.
 *
 * @return None
 *
 * \par<b>Description</b><br>
 * Define the data frame length.\n\n
 * Set the number of bits to be serially transmitted in a frame.
 * The frame length should be multiples of word length. If the value is set to 64, the frame does not
 * automatically end. Use XMC_SPI_CH_DisableSlaveSelect() to end the frame after all the data
 * is transmitted.
 *
 * \par<b>Related APIs:</b><BR>
 * XMC_USIC_CH_SetWordLength(), XMC_USIC_CH_EnableFrameLengthControl(), XMC_SPI_CH_DisableSlaveSelect()
 */
__STATIC_INLINE void XMC_SPI_CH_SetFrameLength(XMC_USIC_CH_t *const channel, const uint8_t frame_length)
{
  XMC_USIC_CH_SetFrameLength(channel, frame_length);
}

/**
 * @param  channel A constant pointer to XMC_USIC_CH_t, pointing to the USIC channel base address.
 *
 * @return None
 *
 * \par<b>Description</b><br>
 * Configure the handling of start of frame through software, by setting TCSR.SOF bit.\n\n
 * This can be used if the software handles the TBUF data without FIFO. If SOF is set, a valid content of the TBUF is 
 * considered as first word of a new frame by finishing the currently running frame. For software handling of SOF bit,
 * it is recommended to configure TCSR.WLEMD as 0. This is not configured during initialization. XMC_SPI_CH_EnableSOF()
 * can be called as needed in the program and can be disabled by XMC_SPI_CH_DisableSOF(). 
 *
 * \par<b>Related APIs:</b><BR>
 * XMC_SPI_CH_DisableSOF()
 */
__STATIC_INLINE void XMC_SPI_CH_EnableSOF(XMC_USIC_CH_t *const channel)
{
  channel->TCSR |= (uint32_t)USIC_CH_TCSR_SOF_Msk;
}

/**
 * @param  channel A constant pointer to XMC_USIC_CH_t, pointing to the USIC channel base address.
 *
 * @return None
 *
 * \par<b>Description</b><br>
 * Set the control of the handling start of frame through hardware, by clearing TCSR.SOF bit.\n\n
 * Typically this can be disabled, where the transmission control is done by the hardware.
 *
 * \par<b>Related APIs:</b><BR>
 * XMC_SPI_CH_EnableSOF()
 */
__STATIC_INLINE void XMC_SPI_CH_DisableSOF(XMC_USIC_CH_t *const channel)
{
  channel->TCSR &= (uint32_t)~USIC_CH_TCSR_SOF_Msk;
}

/**
 * @param  channel A constant pointer to XMC_USIC_CH_t, pointing to the USIC channel base address.
 *
 * @return None
 *
 * \par<b>Description</b><br>
 * Configure the handling of end of frame through software, by setting TCSR.EOF bit.\n\n
 * This can be used if the software handles the TBUF data without FIFO. If EOF is set, a valid content of the TBUF is 
 * considered as last word of a frame. After transfer of the last word, MSLS signal becomes inactive. For software 
 * handling of EOF bit, it is recommended to configure TCSR.WLEMD as 0. \n
 * \b Note: The API should be called before putting the last data word of the frame to TBUF.
 *
 * \par<b>Related APIs:</b><BR>
 * XMC_SPI_CH_DisableEOF()
 */
__STATIC_INLINE void XMC_SPI_CH_EnableEOF(XMC_USIC_CH_t *const channel)
{
  channel->TCSR |= (uint32_t)USIC_CH_TCSR_EOF_Msk;
}

/**
 * @param  channel A constant pointer to XMC_USIC_CH_t, pointing to the USIC channel base address.
 *
 * @return None
 *
 * \par<b>Description</b><br>
 * Configure the handling of end of frame through hardware, by clearing TCSR.EOF bit.\n\n
 * Typically this can be disabled, where the transmission control is done by the hardware.
 *
 * \par<b>Related APIs:</b><BR>
 * XMC_SPI_CH_EnableEOF()
 */
__STATIC_INLINE void XMC_SPI_CH_DisableEOF(XMC_USIC_CH_t *const channel)
{
  channel->TCSR &= (uint32_t)~USIC_CH_TCSR_EOF_Msk;
}

/**
 * @param  channel A constant pointer to XMC_USIC_CH_t, pointing to the USIC channel base address.
 * @param  input SPI channel input stage.\n
 *               Refer @ref XMC_SPI_CH_INPUT_t for valid values
 * @param  source Input source select for the input stage.
 *                Range : [0 to 7]
 *
 * @return None
 *
 * \par<b>Description</b><br>
 * Selects the data source for SPI input stage, by configuring DXCR.DSEL bits.\n\n
 * Selects the input data signal source among DXnA, DXnB.. DXnG for the input stage. The API can be used for all the 
 * input stages like DX0CR, DX1CR etc. This is not done during initialization. This has to be configured before starting
 * the SPI communication.
 */
__STATIC_INLINE void XMC_SPI_CH_SetInputSource(XMC_USIC_CH_t *const channel,
                                               const XMC_SPI_CH_INPUT_t input,
                                               const uint8_t source)
{
  channel->DXCR[input] = (uint32_t)(channel->DXCR[input] & (~USIC_CH_DX0CR_DSEN_Msk)) | USIC_CH_DX0CR_INSW_Msk;
  XMC_USIC_CH_SetInputSource(channel, (XMC_USIC_CH_INPUT_t)input, source);
}

/**
 * @param  channel A constant pointer to XMC_USIC_CH_t, pointing to the USIC channel base address.
 * @param  selo_inversion Polarity of the slave select signal relative to the MSLS signal.\n
 *               Refer @ref XMC_SPI_CH_SLAVE_SEL_MSLS_INV_t for valid values
 *
 * @return None
 *
 * \par<b>Description</b><br>
 * Set the polarity of the slave select signal, by configuring PCR.SELINV bit.\n\n
 * Normally MSLS signal is active low level signal. SO based on the slave inversion has to be applied. This is configured 
 * in XMC_SPI_CH_Init() during initialization. Invoke XMC_SPI_CH_SetSlaveSelectPolarity() with desired settings as 
 * needed later in the program.
 */
__STATIC_INLINE void XMC_SPI_CH_SetSlaveSelectPolarity(XMC_USIC_CH_t *const channel,
                                                       const XMC_SPI_CH_SLAVE_SEL_MSLS_INV_t selo_inversion)
{
  /* Configuration of Protocol Control Register */
  channel->PCR_SSCMode = (uint32_t)((channel->PCR_SSCMode & (~USIC_CH_PCR_SSCMode_SELINV_Msk)) | (uint32_t)selo_inversion);
}

/**
 * @param  channel A constant pointer to XMC_USIC_CH_t, pointing to the USIC channel base address.
 * @param  input SPI channel input stage.\n
 *               Refer @ref XMC_SPI_CH_INPUT_t for valid inputs.
 *
 * @return None
 *
 * \par<b>Description</b><br>
 * Enables the polarity inversion of input data signal, by setting DXyCR.DPOL(where y = \a input).\n\n
 * This is not set in XMC_SPI_CH_Init(). Invoke XMC_SPI_CH_EnableInputInversion() as needed later in the program. To 
 * disable the inversion XMC_SPI_CH_DisableInputInversion() can be invoked.
 *
 * \par<b>Related APIs:</b><BR>
 * XMC_SPI_CH_DisableInputInversion()
 */
__STATIC_INLINE void XMC_SPI_CH_EnableInputInversion(XMC_USIC_CH_t *const channel,
                                                     const XMC_SPI_CH_INPUT_t input)
{
  XMC_USIC_CH_EnableInputInversion(channel, (XMC_USIC_CH_INPUT_t)input);
}

/**
 * @param  channel A constant pointer to XMC_USIC_CH_t, pointing to the USIC channel base address.
 * @param  input SPI channel input stage.\n
 *               Refer @ref XMC_SPI_CH_INPUT_t for valid inputs.
 *
 * @return None
 *
 * \par<b>Description</b><br>
 * Disables the polarity inversion of input data signal, by clearing DXyCR.DPOL(where y = \a input).\n\n
 * Resets the input data polarity. Invoke XMC_SPI_CH_EnableInputInversion() to apply inversion.
 *
 * \par<b>Related APIs:</b><BR>
 * XMC_SPI_CH_EnableInputInversion()
 */
__STATIC_INLINE void XMC_SPI_CH_DisableInputInversion(XMC_USIC_CH_t *const channel,
                                                      const XMC_SPI_CH_INPUT_t input)
{
  XMC_USIC_CH_DisableInputInversion(channel, (XMC_USIC_CH_INPUT_t)input);
}

/**
 * @param  channel A constant pointer to XMC_USIC_CH_t, pointing to the USIC channel base address.
 * @param service_request Service request number.
                          Range: [0 to 5]
 *                          
 * @return None
 *
 * \par<b>Description</b><br>
 * Sets the interrupt node for SPI channel events.\n\n
 * For an event to generate interrupt, node pointer should be configured with service request(SR0, SR1..SR5). 
 * The NVIC node gets linked to the interrupt event by doing so. This is not configured in XMC_SPI_CH_Init() during
 * initialization.
 *
 * \par<b>Note::</b><BR>
 * 1. NVIC node should be separately enabled to generate the interrupt.
 *
 * \par<b>Related APIs:</b><BR>
 * XMC_USIC_CH_EnableEvent()
 */
__STATIC_INLINE void XMC_SPI_CH_SetInterruptNodePointer(XMC_USIC_CH_t *const channel,
                                                     const uint8_t service_request)
{
  XMC_USIC_CH_SetInterruptNodePointer(channel, XMC_USIC_CH_INTERRUPT_NODE_POINTER_PROTOCOL, (uint32_t)service_request);
}


/**
 * @param channel Pointer to USIC channel handler of type @ref XMC_USIC_CH_t \n
 * 				  \b Range: @ref XMC_SPI0_CH0, @ref XMC_SPI0_CH1,@ref XMC_SPI1_CH0,@ref XMC_SPI1_CH1,@ref XMC_SPI2_CH0,@ref XMC_SPI2_CH1 @note Availability of SPI1 and SPI2 depends on device selection
 * @param  interrupt_node Interrupt node pointer to be configured. \n
 * 						  \b Range: @ref XMC_SPI_CH_INTERRUPT_NODE_POINTER_TRANSMIT_SHIFT,
 * 						  			@ref XMC_SPI_CH_INTERRUPT_NODE_POINTER_TRANSMIT_BUFFER etc.
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
 * XMC_SPI_CH_EnableEvent() \n\n\n
 */
__STATIC_INLINE void XMC_SPI_CH_SelectInterruptNodePointer(XMC_USIC_CH_t *const channel,
                                                           const XMC_SPI_CH_INTERRUPT_NODE_POINTER_t interrupt_node,
                                                           const uint32_t service_request)
{
  XMC_USIC_CH_SetInterruptNodePointer(channel, (XMC_USIC_CH_INTERRUPT_NODE_POINTER_t)interrupt_node,
		                                       (uint32_t)service_request);
}

/**
 * @param  channel Pointer to USIC channel handler of type @ref XMC_USIC_CH_t \n
 * 				   \b Range: @ref XMC_SPI0_CH0, @ref XMC_SPI0_CH1,@ref XMC_SPI1_CH0,@ref XMC_SPI1_CH1,@ref XMC_SPI2_CH0,@ref XMC_SPI2_CH1 @note Availability of SPI1 and SPI2 depends on device selection
 * @param  service_request_line service request number of the event to be triggered. \n
 * 			\b Range: 0 to 5.
 * @return None
 *
 * \par<b>Description</b><br>
 * Trigger a SPI interrupt service request.\n\n
 * When the SPI service request is triggered, the NVIC interrupt associated with it will be
 * generated if enabled.
 *
 * \par<b>Related APIs:</b><BR>
 * XMC_SPI_CH_SelectInterruptNodePointer() \n\n\n
 */
__STATIC_INLINE void XMC_SPI_CH_TriggerServiceRequest(XMC_USIC_CH_t *const channel, const uint32_t service_request_line)
{
  XMC_USIC_CH_TriggerServiceRequest(channel, (uint32_t)service_request_line);
}

/**
 * @param  channel Pointer to USIC channel handler of type @ref XMC_USIC_CH_t \n
 * 				   \b Range: @ref XMC_SPI0_CH0, @ref XMC_SPI0_CH1,@ref XMC_SPI1_CH0,@ref XMC_SPI1_CH1,@ref XMC_SPI2_CH0,@ref XMC_SPI2_CH1 @note Availability of SPI1 and SPI2 depends on device selection
 * @return None
 *
 * \par<b>Description</b><br>
 * Enables delay compensation. \n\n
 *
 * Delay compensation can be applied to the receive path.
 * \par<b>Related APIs:</b><BR>
 * XMC_SPI_CH_DisableDelayCompensation()\n\n\n
 */
__STATIC_INLINE void XMC_SPI_CH_EnableDelayCompensation(XMC_USIC_CH_t *const channel)
{
  XMC_USIC_CH_EnableDelayCompensation(channel);
}

/**
 * @param  channel Pointer to USIC channel handler of type @ref XMC_USIC_CH_t \n
 * 				   \b Range: @ref XMC_SPI0_CH0, @ref XMC_SPI0_CH1,@ref XMC_SPI1_CH0,@ref XMC_SPI1_CH1,@ref XMC_SPI2_CH0,@ref XMC_SPI2_CH1 @note Availability of SPI1 and SPI2 depends on device selection
 * @return None
 *
 * \par<b>Description</b><br>
 * Disables delay compensation.. \n\n
 *
 * \par<b>Related APIs:</b><BR>
 * XMC_SPI_CH_EnableDelayCompensation()\n\n\n
 */
__STATIC_INLINE void XMC_SPI_CH_DisableDelayCompensation(XMC_USIC_CH_t *const channel)
{
  XMC_USIC_CH_DisableDelayCompensation(channel);
}

/**
 * @param  channel Pointer to USIC channel handler of type @ref XMC_USIC_CH_t \n
 * 				   \b Range: @ref XMC_SPI0_CH0, @ref XMC_SPI0_CH1,@ref XMC_SPI1_CH0,@ref XMC_SPI1_CH1,@ref XMC_SPI2_CH0,@ref XMC_SPI2_CH1 @note Availability of SPI1 and SPI2 depends on device selection
 * @param  pdiv Desired divider for the external frequency input. \b Range: minimum value = 1, maximum value = 1024 \n
 * @param  combination_mode  USIC channel input combination mode \n
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
__STATIC_INLINE void XMC_SPI_CH_ConfigExternalInputSignalToBRG(XMC_USIC_CH_t *const channel,
		                                                       const uint16_t pdiv,
												               const XMC_USIC_CH_INPUT_COMBINATION_MODE_t combination_mode)
{
  XMC_USIC_CH_ConfigExternalInputSignalToBRG(channel,pdiv,2U,combination_mode);
}

/**
 * @param  channel A constant pointer to XMC_USIC_CH_t, pointing to the USIC channel base address.
 *
 * @return None
 *
 * \par<b>Description</b><br>
 * The SELOx lines (with x = 1-7) can be used as addresses for an external address
 * decoder to increase the number of external slave devices.
 */
__STATIC_INLINE void XMC_SPI_CH_EnableSlaveSelectCodedMode(XMC_USIC_CH_t *const channel)
{
  /* Configuration of Protocol Control Register */
  channel->PCR_SSCMode = (uint32_t)(channel->PCR_SSCMode & (~USIC_CH_PCR_SSCMode_SELCTR_Msk));
}

/**
 * @param  channel A constant pointer to XMC_USIC_CH_t, pointing to the USIC channel base address.
 *
 * @return None
 *
 * \par<b>Description</b><br>
 * Each SELOx line (with x = 0-7) can be directly connected to an external slave device.
 */
__STATIC_INLINE void XMC_SPI_CH_DisableSlaveSelectCodedMode(XMC_USIC_CH_t *const channel)
{
  /* Configuration of Protocol Control Register */
  channel->PCR_SSCMode |= (uint32_t)USIC_CH_PCR_SSCMode_SELCTR_Msk;
}

/**
 * @param channel Constant pointer to USIC channel handle of type @ref XMC_USIC_CH_t \n
 *          \b Range: @ref XMC_SPI0_CH0, @ref XMC_SPI0_CH1,@ref XMC_SPI1_CH0,@ref XMC_SPI1_CH1,@ref XMC_SPI2_CH0,@ref XMC_SPI2_CH1 @note Availability of SPI1 and SPI2 depends on device selection
 * @return None
 *
 * \par<b>Description</b><br>
 * Enable data transmission.\n\n
 * Use this function in combination with XMC_SPI_CH_DisableDataTransmission() to fill the FIFO and send the FIFO content without gaps in the transmission.
 * FIFO is filled using XMC_USIC_CH_TXFIFO_PutData().
 * @note If you need more control over the start of transmission use XMC_USIC_CH_SetStartTransmisionMode()
 *
 * \par<b>Related APIs:</b><BR>
 * XMC_SPI_CH_DisableDataTransmission()\n\n\n
 */
__STATIC_INLINE void XMC_SPI_CH_EnableDataTransmission(XMC_USIC_CH_t *const channel)
{
  XMC_USIC_CH_SetStartTransmisionMode(channel, XMC_USIC_CH_START_TRANSMISION_ON_TDV);
}

/**
 * @param channel Constant pointer to USIC channel handle of type @ref XMC_USIC_CH_t \n
 *          \b Range: @ref XMC_SPI0_CH0, @ref XMC_SPI0_CH1,@ref XMC_SPI1_CH0,@ref XMC_SPI1_CH1,@ref XMC_SPI2_CH0,@ref XMC_SPI2_CH1 @note Availability of SPI1 and SPI2 depends on device selection
 * @return None
 *
 * \par<b>Description</b><br>
 * Disable data transmission.\n\n
 * Use this function in combination with XMC_SPI_CH_EnableDataTransmission() to fill the FIFO and send the FIFO content without gaps in the transmission.
 * FIFO is filled using XMC_USIC_CH_TXFIFO_PutData().
 *
 * \par<b>Related APIs:</b><BR>
 * XMC_SPI_CH_EnableDataTransmission()\n\n\n
 */
__STATIC_INLINE void XMC_SPI_CH_DisableDataTransmission(XMC_USIC_CH_t *const channel)
{
  XMC_USIC_CH_SetStartTransmisionMode(channel, XMC_USIC_CH_START_TRANSMISION_DISABLED); 
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
