 /**
 * @file xmc_uart.h
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
 *     - Description updated <br>
 *     - Added XMC_UART_CH_TriggerServiceRequest() and XMC_UART_CH_SelectInterruptNodePointer <br>
 *
 * 2015-06-20:
 *     - Removed version macros and declaration of GetDriverVersion API <br>
 *
 * 2015-09-01:
 *     - Modified XMC_UART_CH_SetInputSource() for avoiding complete DXCR register overwriting. <br>
 *     - Modified XMC_UART_CH_EVENT_t enum for supporting XMC_UART_CH_EnableEvent() and XMC_UART_CH_DisableEvent()
 *       for supporting multiple events configuration <br>
 * 
 * 2016-05-20:
 *     - Added XMC_UART_CH_EnableDataTransmission() and XMC_UART_CH_DisableDataTransmission()
 *
 * @endcond 
 *
 */

#ifndef XMC_UART_H
#define XMC_UART_H

/*********************************************************************************************************************
 * HEADER FILES
 *********************************************************************************************************************/

#include "xmc_usic.h"

/**
 * @addtogroup XMClib XMC Peripheral Library
 * @{
 */

/**
 * @addtogroup UART
 * @brief Universal Asynchronous Receiver/Transmitter (UART) driver for XMC microcontroller family.
 *
 * The UART driver uses Universal Serial Interface Channel(USIC) module to implement UART protocol.
 * It provides APIs to configure USIC channel for UART communication. The driver enables the user
 * in getting the status of UART protocol events, configuring interrupt service requests, protocol
 * related parameter configuration etc.
 * 
 * UART driver features:
 * -# Configuration structure XMC_UART_CH_CONFIG_t and initialization function XMC_UART_CH_Init()
 * -# Enumeration of events with their bit masks @ref XMC_UART_CH_EVENT_t, @ref XMC_UART_CH_STATUS_FLAG_t
 * -# Allows the selection of input source for the DX0 input stage using the API XMC_UART_CH_SetInputSource()
 * -# Allows configuration of baudrate using XMC_UART_CH_SetBaudrate() and configuration of data length using 
 XMC_UART_CH_SetWordLength() and XMC_UART_CH_SetFrameLength()
 * -# Provides the status of UART protocol events, XMC_UART_CH_GetStatusFlag()
 * -# Allows transmission of data using XMC_UART_CH_Transmit() and gets received data using XMC_UART_CH_GetReceivedData()
 * 
 * @{
 */

/*********************************************************************************************************************
 * MACROS
 *********************************************************************************************************************/

#if defined(USIC0)
#define XMC_UART0_CH0 XMC_USIC0_CH0 /**< USIC0 channel 0 base address */
#define XMC_UART0_CH1 XMC_USIC0_CH1 /**< USIC0 channel 1 base address */
#endif

#if defined(USIC1)
#define XMC_UART1_CH0 XMC_USIC1_CH0 /**< USIC1 channel 0 base address */
#define XMC_UART1_CH1 XMC_USIC1_CH1 /**< USIC1 channel 1 base address */
#endif

#if defined(USIC2)
#define XMC_UART2_CH0 XMC_USIC2_CH0 /**< USIC2 channel 0 base address */
#define XMC_UART2_CH1 XMC_USIC2_CH1 /**< USIC2 channel 1 base address */
#endif

/*********************************************************************************************************************
 * ENUMS
 *********************************************************************************************************************/

/**
 * UART driver status
 */
typedef enum XMC_UART_CH_STATUS
{
  XMC_UART_CH_STATUS_OK,     /**< UART driver status : OK*/
  XMC_UART_CH_STATUS_ERROR,  /**< UART driver status : ERROR */
  XMC_UART_CH_STATUS_BUSY    /**< UART driver status : BUSY */
} XMC_UART_CH_STATUS_t;

/**
* UART portocol status. The enum values can be used for getting the status of UART channel.
*
*/
typedef enum XMC_UART_CH_STATUS_FLAG
{
  XMC_UART_CH_STATUS_FLAG_TRANSMISSION_IDLE = USIC_CH_PSR_ASCMode_TXIDLE_Msk,                 /**< UART Protocol Status transmit IDLE*/
  XMC_UART_CH_STATUS_FLAG_RECEPTION_IDLE = USIC_CH_PSR_ASCMode_RXIDLE_Msk,                    /**< UART Protocol Status receive IDLE*/
  XMC_UART_CH_STATUS_FLAG_SYNCHRONIZATION_BREAK_DETECTED = USIC_CH_PSR_ASCMode_SBD_Msk,       /**< UART Protocol Status synchronization break detected*/
  XMC_UART_CH_STATUS_FLAG_COLLISION_DETECTED = USIC_CH_PSR_ASCMode_COL_Msk,                   /**< UART Protocol Status collision detected*/
  XMC_UART_CH_STATUS_FLAG_RECEIVER_NOISE_DETECTED = USIC_CH_PSR_ASCMode_RNS_Msk,              /**< UART Protocol Status receiver noise detected */
  XMC_UART_CH_STATUS_FLAG_FORMAT_ERROR_IN_STOP_BIT_0 = USIC_CH_PSR_ASCMode_FER0_Msk,          /**< UART Protocol Status format error in stop bit 0 */
  XMC_UART_CH_STATUS_FLAG_FORMAT_ERROR_IN_STOP_BIT_1 = USIC_CH_PSR_ASCMode_FER1_Msk,          /**< UART Protocol Status format error in stop bit 1 */
  XMC_UART_CH_STATUS_FLAG_RECEIVE_FRAME_FINISHED = USIC_CH_PSR_ASCMode_RFF_Msk,               /**< UART Protocol Status receive frame finished */
  XMC_UART_CH_STATUS_FLAG_TRANSMITTER_FRAME_FINISHED = USIC_CH_PSR_ASCMode_TFF_Msk,           /**< UART Protocol Status transmit frame finished */
  XMC_UART_CH_STATUS_FLAG_TRANSFER_STATUS_BUSY = USIC_CH_PSR_ASCMode_BUSY_Msk,                /**< UART Protocol Status transfer status busy */
  XMC_UART_CH_STATUS_FLAG_RECEIVER_START_INDICATION = USIC_CH_PSR_ASCMode_RSIF_Msk,           /**< UART Protocol Status receive start indication flag*/
  XMC_UART_CH_STATUS_FLAG_DATA_LOST_INDICATION = USIC_CH_PSR_ASCMode_DLIF_Msk,                /**< UART Protocol Status data lost indication flag*/
  XMC_UART_CH_STATUS_FLAG_TRANSMIT_SHIFT_INDICATION = USIC_CH_PSR_ASCMode_TSIF_Msk,           /**< UART Protocol Status transmit shift indication flag*/
  XMC_UART_CH_STATUS_FLAG_TRANSMIT_BUFFER_INDICATION = USIC_CH_PSR_ASCMode_TBIF_Msk,          /**< UART Protocol Status transmit buffer indication flag*/
  XMC_UART_CH_STATUS_FLAG_RECEIVE_INDICATION = USIC_CH_PSR_ASCMode_RIF_Msk,                   /**< UART Protocol Status receive indication flag*/
  XMC_UART_CH_STATUS_FLAG_ALTERNATIVE_RECEIVE_INDICATION = USIC_CH_PSR_ASCMode_AIF_Msk,       /**< UART Protocol Status alternative receive  indication flag*/
  XMC_UART_CH_STATUS_FLAG_BAUD_RATE_GENERATOR_INDICATION = USIC_CH_PSR_ASCMode_BRGIF_Msk      /**< UART Protocol Status baudrate generator indication flag*/
} XMC_UART_CH_STATUS_FLAG_t;

/**
* UART configuration events. The enums can be used for configuring events using the CCR register.
*/
typedef enum XMC_CH_UART_EVENT
{
  XMC_UART_CH_EVENT_RECEIVE_START       = USIC_CH_CCR_RSIEN_Msk,  /**< Receive start event */
  XMC_UART_CH_EVENT_DATA_LOST           = USIC_CH_CCR_DLIEN_Msk,  /**< Data lost event */
  XMC_UART_CH_EVENT_TRANSMIT_SHIFT      = USIC_CH_CCR_TSIEN_Msk,  /**< Transmit shift event */
  XMC_UART_CH_EVENT_TRANSMIT_BUFFER     = USIC_CH_CCR_TBIEN_Msk,  /**< Transmit buffer event */
  XMC_UART_CH_EVENT_STANDARD_RECEIVE    = USIC_CH_CCR_RIEN_Msk,   /**< Receive event */
  XMC_UART_CH_EVENT_ALTERNATIVE_RECEIVE = USIC_CH_CCR_AIEN_Msk,   /**< Alternate receive event */
  XMC_UART_CH_EVENT_BAUD_RATE_GENERATOR = USIC_CH_CCR_BRGIEN_Msk, /**< Baudrate generator event */
  
  XMC_UART_CH_EVENT_SYNCHRONIZATION_BREAK = USIC_CH_PCR_ASCMode_SBIEN_Msk, /**< Event synchronization break */
  XMC_UART_CH_EVENT_COLLISION = USIC_CH_PCR_ASCMode_CDEN_Msk,              /**< Event collision */
  XMC_UART_CH_EVENT_RECEIVER_NOISE = USIC_CH_PCR_ASCMode_RNIEN_Msk,        /**< Event receiver noise */
  XMC_UART_CH_EVENT_FORMAT_ERROR = USIC_CH_PCR_ASCMode_FEIEN_Msk,          /**< Event format error */
  XMC_UART_CH_EVENT_FRAME_FINISHED = USIC_CH_PCR_ASCMode_FFIEN_Msk         /**< Event frame finished */
} XMC_UART_CH_EVENT_t;

/**
 * UART Input sampling frequency options
 */
typedef enum XMC_UART_CH_INPUT_SAMPLING_FREQ
{
  XMC_UART_CH_INPUT_SAMPLING_FREQ_FPERIPH            = XMC_USIC_CH_INPUT_SAMPLING_FREQ_FPERIPH,           /**< Sampling frequency input fperiph*/
  XMC_UART_CH_INPUT_SAMPLING_FREQ_FRACTIONAL_DIVIDER = XMC_USIC_CH_INPUT_SAMPLING_FREQ_FRACTIONAL_DIVIDER /**< Sampling frequency input fractional divider*/
} XMC_UART_CH_INPUT_SAMPLING_FREQ_t;

/**
 * UART input stages
 */
typedef enum XMC_UART_CH_INPUT
{
  XMC_UART_CH_INPUT_RXD = 0UL   /**< UART input stage DX0*/
#if UC_FAMILY == XMC1
  ,
  XMC_UART_CH_INPUT_RXD1 = 3UL, /**< UART input stage DX3*/
  XMC_UART_CH_INPUT_RXD2 = 5UL  /**< UART input stage DX5*/
#endif
} XMC_UART_CH_INPUT_t;


/**
 * UART channel interrupt node pointers
 */
typedef enum XMC_UART_CH_INTERRUPT_NODE_POINTER
{
  XMC_UART_CH_INTERRUPT_NODE_POINTER_TRANSMIT_SHIFT      = XMC_USIC_CH_INTERRUPT_NODE_POINTER_TRANSMIT_SHIFT, /**< Node pointer for transmit shift interrupt */
  XMC_UART_CH_INTERRUPT_NODE_POINTER_TRANSMIT_BUFFER     = XMC_USIC_CH_INTERRUPT_NODE_POINTER_TRANSMIT_BUFFER, /**< Node pointer for transmit buffer interrupt */
  XMC_UART_CH_INTERRUPT_NODE_POINTER_RECEIVE             = XMC_USIC_CH_INTERRUPT_NODE_POINTER_RECEIVE,  /**< Node pointer for receive interrupt */
  XMC_UART_CH_INTERRUPT_NODE_POINTER_ALTERNATE_RECEIVE   = XMC_USIC_CH_INTERRUPT_NODE_POINTER_ALTERNATE_RECEIVE,  /**< Node pointer for alternate receive interrupt */
  XMC_UART_CH_INTERRUPT_NODE_POINTER_PROTOCOL            = XMC_USIC_CH_INTERRUPT_NODE_POINTER_PROTOCOL   /**< Node pointer for protocol related interrupts */
} XMC_UART_CH_INTERRUPT_NODE_POINTER_t;

/*********************************************************************************************************************
 * DATA STRUCTURES
 *********************************************************************************************************************/

/**
 * UART initialization structure
*/
typedef struct XMC_UART_CH_CONFIG
{
  uint32_t baudrate;                         /**< Desired baudrate. \b Range: minimum= 100, maximum= (fPERIPH * 1023)/(1024 * oversampling) */
  uint8_t data_bits;                         /**< Number of bits for the data field. Value configured as USIC channel word length. \n
                                                  \b Range: minimum= 1, maximum= 16*/
  uint8_t frame_length;                      /**< Indicates nmber of bits in a frame. Configured as USIC channel frame length. \n 
                                                  \b Range: minimum= 1, maximum= 63*/
  uint8_t stop_bits;                         /**< Number of stop bits. \b Range: minimum= 1, maximum= 2 */
  uint8_t oversampling;						           /**< Number of samples for a symbol(DCTQ).\b Range: minimum= 1, maximum= 32*/
  XMC_USIC_CH_PARITY_MODE_t parity_mode;     /**< Parity mode. \b Range: @ref XMC_USIC_CH_PARITY_MODE_NONE, @ref XMC_USIC_CH_PARITY_MODE_EVEN, \n 
                                                  @ref XMC_USIC_CH_PARITY_MODE_ODD*/
} XMC_UART_CH_CONFIG_t;

/*********************************************************************************************************************
 * API PROTOTYPES
 *********************************************************************************************************************/

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @param channel Constant pointer to USIC channel handle of type @ref XMC_USIC_CH_t \n
 * 				  \b Range: @ref XMC_UART0_CH0, XMC_UART0_CH1,XMC_UART1_CH0, XMC_UART1_CH1,XMC_UART2_CH0, XMC_UART2_CH1 @note Availability of UART1 and UART2 depends on device selection
 * @param config Constant pointer to UART configuration structure of type @ref XMC_UART_CH_CONFIG_t.
 * @return XMC_UART_CH_STATUS_t Status of initializing the USIC channel for UART protocol.\n
 *          \b Range: @ref XMC_UART_CH_STATUS_OK if initialization is successful.\n
 *                    @ref XMC_UART_CH_STATUS_ERROR if configuration of baudrate failed.
 *
 * \par<b>Description</b><br>
 * Initializes the USIC channel for UART protocol.\n\n
 * During the initialization, USIC channel is enabled, baudrate is configured with the defined oversampling value
 * in the intialization structure. If the oversampling value is set to 0 in the structure, the default oversampling of 16
 * is considered. Sampling point for each symbol is configured at the half of sampling period. Symbol value is decided by the
 * majority decision among 3 samples. 
 * Word length is configured with the number of data bits. If the value of \a frame_length is 0, then USIC channel frame length
 * is set to the same value as word length. If \a frame_length is greater than 0, it is set as the USIC channel frame length.
 * Parity mode is set to the value configured for \a parity_mode.
 * The USIC channel should be set to UART mode by calling the XMC_UART_CH_Start() API after the initialization.
 *
 * \par<b>Related APIs:</b><BR>
 * XMC_UART_CH_Start(), XMC_UART_CH_Stop(), XMC_UART_CH_Transmit()\n\n\n
 */
void XMC_UART_CH_Init(XMC_USIC_CH_t *const channel, const XMC_UART_CH_CONFIG_t *const config);

/**
 * @param channel Constant pointer to USIC channel handle of type @ref XMC_USIC_CH_t \n
 * 				  \b Range: @ref XMC_UART0_CH0, @ref XMC_UART0_CH1,@ref XMC_UART1_CH0,@ref XMC_UART1_CH1,@ref XMC_UART2_CH0,@ref XMC_UART2_CH1 @note Availability of UART1 and UART2 depends on device selection
 * @return None
 *
 * \par<b>Description</b><br>
 * Sets the USIC channel operation mode to UART mode.\n\n
 * CCR register bitfield \a Mode is set to 2(UART mode). This API should be called after configuring
 * the USIC channel. Transmission and reception can happen only when the UART mode is set. 
 * This is an inline function.
 *
 * \par<b>Related APIs:</b><BR>
 * XMC_UART_CH_Stop(), XMC_UART_CH_Transmit()\n\n\n
 */
__STATIC_INLINE void XMC_UART_CH_Start(XMC_USIC_CH_t *const channel)
{
  channel->CCR = (uint32_t)(((channel->CCR) & (~USIC_CH_CCR_MODE_Msk)) | (uint32_t)XMC_USIC_CH_OPERATING_MODE_UART);
}

/**
 * @param channel Constant pointer to USIC channel handle of type @ref XMC_USIC_CH_t \n
 * 				  \b Range: @ref XMC_UART0_CH0, @ref XMC_UART0_CH1,@ref XMC_UART1_CH0,@ref XMC_UART1_CH1,@ref XMC_UART2_CH0,@ref XMC_UART2_CH1 @note Availability of UART1 and UART2 depends on device selection
 * @return XMC_UART_CH_STATUS_t Status to indicate if the communication channel is stopped successfully.\n
 *                              @ref XMC_UART_CH_STATUS_OK if the communication channel is stopped.
 *                              @ref XMC_UART_CH_STATUS_BUSY if the communication channel is busy.
 *
 * \par<b>Description</b><br>
 * Stops the UART communication.\n\n
 * CCR register bitfield \a Mode is reset. This disables the communication.
 * Before starting the communication again, the channel has to be reconfigured.
 *
 * \par<b>Related APIs:</b><BR>
 * XMC_UART_CH_Init() \n\n\n
 */
XMC_UART_CH_STATUS_t XMC_UART_CH_Stop(XMC_USIC_CH_t *const channel);

/**
 * @param channel Constant pointer to USIC channel handle of type @ref XMC_USIC_CH_t \n
 * 				   \b Range: @ref XMC_UART0_CH0, XMC_UART0_CH1 ,XMC_UART1_CH0, XMC_UART1_CH1, XMC_UART2_CH0, XMC_UART2_CH1 @note Availability of UART1 and UART2 depends on device selection
 * @param rate Desired baudrate. \n
 *           \b Range: minimum value = 100, maximum value depends on the peripheral clock frequency\n
 * 				   and \a oversampling. Maximum baudrate can be derived using the formula: (fperiph * 1023)/(1024 * oversampling)
 * @param  oversampling Required oversampling. The value indicates the number of time quanta for one symbol of data.\n
 * 					 This can be related to the number of samples for each logic state of the data signal.\n
 * 					 \b Range: 4 to 32. Value should be chosen based on the protocol used.
 * @return XMC_UART_CH_STATUS_t Status indicating the baudrate configuration.\n
 * 			     \b Range: @ref XMC_USIC_CH_STATUS_OK if baudrate is successfully configured,
 * 					 @ref XMC_USIC_CH_STATUS_ERROR if desired baudrate or oversampling is invalid.
 *
 * \par<b>Description:</b><br>
 * Sets the bus speed in bits per second.\n\n
 * Derives the values of \a STEP and PDIV to arrive at the optimum realistic speed possible.
 * \a oversampling is the number of samples to be taken for each symbol of UART protocol.
 * Default \a oversampling of 16 is considered if the input \a oversampling is less than 4. It is recommended to keep
 * a minimum oversampling of 4 for UART.
 *
 * \par<b>Related APIs:</b><BR>
 * XMC_UART_CH_Init(), XMC_UART_CH_Stop()
 */
XMC_UART_CH_STATUS_t XMC_UART_CH_SetBaudrate(XMC_USIC_CH_t *const channel, uint32_t rate, uint32_t oversampling);

/**
 * @param channel Constant pointer to USIC channel handle of type @ref XMC_USIC_CH_t \n
 * 				  \b Range: @ref XMC_UART0_CH0, @ref XMC_UART0_CH1,@ref XMC_UART1_CH0,@ref XMC_UART1_CH1,@ref XMC_UART2_CH0,@ref XMC_UART2_CH1 @note Availability of UART1 and UART2 depends on device selection
 * @param  data Data to be transmitted. \n 
 *          \b Range: 16 bit unsigned data within the range 0 to 65535. Actual size of
 *          data transmitted depends on the configured number of bits for the UART protocol in the register SCTR.
 * @return None
 *
 * \par<b>Description</b><br>
 * Transmits data over serial communication channel using UART protocol.\n\n
 * Based on the channel configuration, data is either put to the transmit FIFO or to TBUF register.
 * Before putting data to TBUF, the API waits for TBUF to finish shifting its contents to shift register. 
 * So user can continuously execute the API without checking for TBUF busy status. Based on the number of 
 * data bits configured, the lower significant bits will be extracted for transmission.
 *
 * Note: When FIFO is not configured, the API waits for the TBUF to be available. 
 * This makes the execution a blocking call.
 *
 * \par<b>Related APIs:</b><BR>
 * XMC_UART_CH_GetReceivedData() \n\n\n
 */
void XMC_UART_CH_Transmit(XMC_USIC_CH_t *const channel, const uint16_t data);

/**
 * @param channel Constant pointer to USIC channel handle of type @ref XMC_USIC_CH_t \n
 * 				  \b Range: @ref XMC_UART0_CH0, @ref XMC_UART0_CH1,@ref XMC_UART1_CH0,@ref XMC_UART1_CH1,@ref XMC_UART2_CH0,@ref XMC_UART2_CH1 @note Availability of UART1 and UART2 depends on device selection
 * @return uint16_t Received data over UART communication channel.
 * \par<b>Description</b><br>
 * Provides one word of data received over UART communication channel.\n\n
 * Based on the channel configuration, data is either read from the receive FIFO or RBUF register.
 * Before returning the value, there is no check for data validity. User should check the appropriate
 * data receive flags(standard receive/alternative receive/FIFO standard receive/FIFO alternative receive)
 * before executing the API. Reading from an empty receive FIFO can generate a receive error event.
 *
 * \par<b>Related APIs:</b><BR>
 * XMC_UART_CH_GetStatusFlag(), XMC_UART_CH_Transmit() \n\n\n
 */
uint16_t XMC_UART_CH_GetReceivedData(XMC_USIC_CH_t *const channel);

/**
 * @param  channel Constant pointer to USIC channel handle of type @ref XMC_USIC_CH_t \n
 * 				  \b Range: @ref XMC_UART0_CH0, @ref XMC_UART0_CH1,@ref XMC_UART1_CH0,@ref XMC_UART1_CH1,@ref XMC_UART2_CH0,@ref XMC_UART2_CH1 @note Availability of UART1 and UART2 depends on device selection
 * @param  word_length Data word length. \n
 *          \b Range: minimum= 1, maximum= 16.
 * @return None
 *
 * \par<b>Description</b><br>
 * Sets the data word length in number of bits.\n\n
 * Word length can range from 1 to 16. It indicates the number of data bits in a data word.
 * The value of \a word_length will be decremented by 1 before setting the value to \a SCTR register.
 * If the UART data bits is more than 16, then the frame length should be set to the actual number of bits and
 * word length should be configured with the number of bits expected in each transaction. For example, if number of data bits
 * for UART communication is 20 bits, then the frame length should be set as 20. Word length can be set based on the
 * transmit and receive handling. If data is stored as 8bit array, then the word length can be set to 8. In this case, 
 * a full message of UART data should be transmitted/ received as 3 data words.
 *
 * \par<b>Related APIs:</b><BR>
 * XMC_UART_CH_SetFrameLength() \n\n\n
 */
__STATIC_INLINE void XMC_UART_CH_SetWordLength(XMC_USIC_CH_t *const channel, const uint8_t word_length)
{
  XMC_USIC_CH_SetWordLength(channel, word_length);
}

/**
 * @param  channel Constant pointer to USIC channel handle of type @ref XMC_USIC_CH_t \n
 * 				  \b Range: @ref XMC_UART0_CH0, @ref XMC_UART0_CH1,@ref XMC_UART1_CH0,@ref XMC_UART1_CH1,@ref XMC_UART2_CH0,@ref XMC_UART2_CH1 @note Availability of UART1 and UART2 depends on device selection
 * @param  frame_length Number of data bits in each UART frame. \n
 *          \b Range: minimum= 1, maximum= 64.
 * @return None
 *
 * \par<b>Description</b><br>
 * Sets the number of data bits for UART communication.\n\n
 * The frame length is configured by setting the input value to \a SCTR register.
 * The value of \a frame_length will be decremented by 1, before setting it to the register.
 * Frame length should not be set to 64 for UART communication.
 *
 * \par<b>Related APIs:</b><BR>
 * XMC_UART_CH_SetWordLength() \n\n\n
 */
__STATIC_INLINE void XMC_UART_CH_SetFrameLength(XMC_USIC_CH_t *const channel, const uint8_t frame_length)
{
  XMC_USIC_CH_SetFrameLength(channel, frame_length);
}

/**
 * @param channel Constant pointer to USIC channel handle of type @ref XMC_USIC_CH_t \n
 * 				  \b Range: @ref XMC_UART0_CH0, @ref XMC_UART0_CH1,@ref XMC_UART1_CH0,@ref XMC_UART1_CH1,@ref XMC_UART2_CH0,@ref XMC_UART2_CH1 @note Availability of UART1 and UART2 depends on device selection
 * @param  event Event bitmasks to enable. Use the type @ref XMC_UART_CH_EVENT_t for naming events. \n
 * 				  \b Range: @ref XMC_UART_CH_EVENT_RECEIVE_START, @ref XMC_UART_CH_EVENT_DATA_LOST,
 * 				  @ref XMC_UART_CH_EVENT_TRANSMIT_SHIFT, @ref XMC_UART_CH_EVENT_TRANSMIT_BUFFER,
 * 				  etc.
 * @return None
 *
 * \par<b>Description</b><br>
 * Enables interrupt events for UART communication.\n\n
 * Multiple events can be combined using the bitwise OR operation and configured in one function call.
 * @ref XMC_UART_CH_EVENT_t enumerates multiple event bitmasks. These enumerations can be used as input to the API.
 * Events are configured by setting bits in the CCR register.
 * \par<b>Related APIs:</b><BR>
 * XMC_UART_CH_DisableEvent(), XMC_UART_CH_SetInterruptNodePointer(), XMC_UART_CH_GetStatusFlag() \n\n\n
 */
void XMC_UART_CH_EnableEvent(XMC_USIC_CH_t *const channel, const uint32_t event);

/**
 * @param channel Constant pointer to USIC channel handle of type @ref XMC_USIC_CH_t \n
 * 				  \b Range: @ref XMC_UART0_CH0, @ref XMC_UART0_CH1,@ref XMC_UART1_CH0,@ref XMC_UART1_CH1,@ref XMC_UART2_CH0,@ref XMC_UART2_CH1 @note Availability of UART1 and UART2 depends on device selection
 * @param  event Bitmask of events to disable. Use the type @ref XMC_UART_CH_EVENT_t for naming events.\n
 * 				  \b Range: @ref XMC_UART_CH_EVENT_RECEIVE_START, @ref XMC_UART_CH_EVENT_DATA_LOST,
 * 				  @ref XMC_UART_CH_EVENT_TRANSMIT_SHIFT, @ref XMC_UART_CH_EVENT_TRANSMIT_BUFFER,
 * 				  etc.
 * @return None
 *
 * \par<b>Description</b><br>
 * Disables the interrupt events by clearing the bits in CCR register.\n\n
 * Multiple events can be combined using the bitwise OR operation and configured in one function call.
 * XMC_UART_CH_EVENT_FLAG_t enumerates multiple event bitmasks. These enumerations can be used as input to the API.
 *
 * \par<b>Related APIs:</b><BR>
 * XMC_UART_CH_ClearStatusFlag(), XMC_UART_CH_EnableEvent() \n\n\n
 */
void XMC_UART_CH_DisableEvent(XMC_USIC_CH_t *const channel, const uint32_t event);

/**
 * @param channel Constant pointer to USIC channel handle of type @ref XMC_USIC_CH_t \n
 * 				  \b Range: @ref XMC_UART0_CH0, @ref XMC_UART0_CH1,@ref XMC_UART1_CH0,@ref XMC_UART1_CH1,@ref XMC_UART2_CH0,@ref XMC_UART2_CH1 @note Availability of UART1 and UART2 depends on device selection
 * @param service_request Service request number for generating protocol interrupts.\n
 * 				  \b Range: 0 to 5.
 * @return None
 *
 * \par<b>Description</b><br>
 * Sets the interrupt node for UART channel protocol events.\n\n
 * For all the protocol events enlisted in the enumeration XMC_UART_CH_EVENT_t, one common
 * interrupt gets generated. The service request connects the interrupt node to the UART 
 * protocol events.
 * Note: NVIC node should be separately enabled to generate the interrupt.
 *
 * \par<b>Related APIs:</b><BR>
 * XMC_UART_CH_EnableEvent() \n\n\n
 */
__STATIC_INLINE void XMC_UART_CH_SetInterruptNodePointer(XMC_USIC_CH_t *const channel,
                                                         const uint8_t service_request)
{
  XMC_USIC_CH_SetInterruptNodePointer(channel, XMC_USIC_CH_INTERRUPT_NODE_POINTER_PROTOCOL, 
                                              (uint32_t)service_request);
}

/**
 * @param channel Pointer to USIC channel handler of type @ref XMC_USIC_CH_t \n
 * 				  \b Range: @ref XMC_UART0_CH0, @ref XMC_UART0_CH1,@ref XMC_UART1_CH0,@ref XMC_UART1_CH1,@ref XMC_UART2_CH0,@ref XMC_UART2_CH1 @note Availability of UART1 and UART2 depends on device selection
 * @param  interrupt_node Interrupt node pointer to be configured. \n
 * 						  \b Range: @ref XMC_UART_CH_INTERRUPT_NODE_POINTER_TRANSMIT_SHIFT,
 * 						  			@ref XMC_UART_CH_INTERRUPT_NODE_POINTER_TRANSMIT_BUFFER etc.
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
 * XMC_UART_CH_EnableEvent() \n\n\n
 */
__STATIC_INLINE void XMC_UART_CH_SelectInterruptNodePointer(XMC_USIC_CH_t *const channel,
                                                            const XMC_UART_CH_INTERRUPT_NODE_POINTER_t interrupt_node,
                                                            const uint32_t service_request)
{
  XMC_USIC_CH_SetInterruptNodePointer(channel, (XMC_USIC_CH_INTERRUPT_NODE_POINTER_t)interrupt_node,
		                                       (uint32_t)service_request);
}

/**
 * @param  channel Pointer to USIC channel handler of type @ref XMC_USIC_CH_t \n
 * 				   \b Range: @ref XMC_UART0_CH0, @ref XMC_UART0_CH1,@ref XMC_UART1_CH0,@ref XMC_UART1_CH1,@ref XMC_UART2_CH0,@ref XMC_UART2_CH1 @note Availability of UART1 and UART2 depends on device selection
 * @param  service_request_line service request number of the event to be triggered. \n
 * 			\b Range: 0 to 5.
 * @return None
 *
 * \par<b>Description</b><br>
 * Trigger a UART interrupt service request.\n\n
 * When the UART service request is triggered, the NVIC interrupt associated with it will be
 * generated if enabled.
 *
 * \par<b>Related APIs:</b><BR>
 * XMC_UART_CH_SelectInterruptNodePointer() \n\n\n
 */
__STATIC_INLINE void XMC_UART_CH_TriggerServiceRequest(XMC_USIC_CH_t *const channel, const uint32_t service_request_line)
{
  XMC_USIC_CH_TriggerServiceRequest(channel, (uint32_t)service_request_line);
}

/**
 * @param channel Constant pointer to USIC channel handle of type @ref XMC_USIC_CH_t \n
 * 				  \b Range: @ref XMC_UART0_CH0, @ref XMC_UART0_CH1,@ref XMC_UART1_CH0,@ref XMC_UART1_CH1,@ref XMC_UART2_CH0,@ref XMC_UART2_CH1 @note Availability of UART1 and UART2 depends on device selection
 * @return Status of UART channel events. \n 
 *          \b Range: Use @ref XMC_UART_CH_STATUS_FLAG_t enumerations for
 * 					event bitmasks. @ref XMC_UART_CH_STATUS_FLAG_TRANSMISSION_IDLE, @ref XMC_UART_CH_STATUS_FLAG_RECEPTION_IDLE,
 * 					@ref XMC_UART_CH_STATUS_FLAG_SYNCHRONIZATION_BREAK_DETECTED etc.
 *
 * \par<b>Description</b><br>
 * Provides the status of UART channel events.\n\n
 * Status provided by the API represents the status of multiple events at their bit positions. The bitmasks can be
 * obtained using the enumeration XMC_UART_CH_STATUS_FLAG_t. Event status is obtained by reading
 * the register PSR_ASCMode.
 *
 * \par<b>Related APIs:</b><BR>
 * XMC_UART_CH_EnableEvent(),  XMC_UART_CH_ClearStatusFlag()\n\n\n
 */
__STATIC_INLINE uint32_t XMC_UART_CH_GetStatusFlag(XMC_USIC_CH_t *const channel)
{
  return channel->PSR_ASCMode;
}

/**
 * @param channel Constant pointer to USIC channel handle of type @ref XMC_USIC_CH_t \n
 * 				  \b Range: @ref XMC_UART0_CH0, @ref XMC_UART0_CH1,@ref XMC_UART1_CH0,@ref XMC_UART1_CH1,@ref XMC_UART2_CH0,@ref XMC_UART2_CH1 @note Availability of UART1 and UART2 depends on device selection
 * @param flag UART events to be cleared. \n
 *          \b Range: Use @ref XMC_UART_CH_STATUS_FLAG_t enumerations for
 * 					event bitmasks. @ref XMC_UART_CH_STATUS_FLAG_TRANSMISSION_IDLE, @ref XMC_UART_CH_STATUS_FLAG_RECEPTION_IDLE,
 * 					@ref XMC_UART_CH_STATUS_FLAG_SYNCHRONIZATION_BREAK_DETECTED etc.
 * @return None
 *
 * \par<b>Description</b><br>
 * Clears the status of UART channel events.\n\n
 * Multiple events can be combined using the bitwise OR operation and configured in one function call.
 * XMC_UART_CH_STATUS_FLAG_t enumerates multiple event bitmasks. These enumerations can be used as input to the API.
 * Events are cleared by setting the bitmask to the PSCR register.
 *
 * \par<b>Related APIs:</b><BR>
 * XMC_UART_CH_DisableEvent(),  XMC_UART_CH_GetStatusFlag()\n\n\n
 */
__STATIC_INLINE void XMC_UART_CH_ClearStatusFlag(XMC_USIC_CH_t *const channel, const uint32_t flag)
{
  channel->PSCR = flag;
}

/**
 * @param channel Constant pointer to USIC channel handle of type @ref XMC_USIC_CH_t \n
 * 				  \b Range: @ref XMC_UART0_CH0, @ref XMC_UART0_CH1,@ref XMC_UART1_CH0,@ref XMC_UART1_CH1,@ref XMC_UART2_CH0,@ref XMC_UART2_CH1 @note Availability of UART1 and UART2 depends on device selection
 * @param input UART channel input stage of type @ref XMC_UART_CH_INPUT_t. \n 
 *          \b Range: @ref XMC_UART_CH_INPUT_RXD (for DX0),
 * 					@ref XMC_UART_CH_INPUT_RXD1 (for DX3), @ref XMC_UART_CH_INPUT_RXD2 (for DX5).
 * @param source Input source select for the input stage. The table provided below maps the decimal value with the input source.
 * <table><tr><td>0</td><td>DXnA</td></tr><tr><td>1</td><td>DXnB</td></tr><tr><td>2</td><td>DXnC</td></tr><tr><td>3</td><td>DXnD</td></tr>
 * <tr><td>4</td><td>DXnE</td></tr><tr><td>5</td><td>DXnF</td></tr><tr><td>6</td><td>DXnG</td></tr><tr><td>7</td><td>Always 1</td>
 * </tr></table>
 * @return None
 *
 * \par<b>Description</b><br>
 * Sets input soource for the UART communication.\n\n
 * It is used for configuring the input stage for data reception.
 * Selects the input data signal source among DXnA, DXnB.. DXnG for the input stage. 
 * The API can be used for the input stages DX0, DX3 and DX5. 
 *
 * \par<b>Related APIs:</b><BR>
 * XMC_UART_CH_EnableInputInversion() \n\n\n
 */
__STATIC_INLINE void XMC_UART_CH_SetInputSource(XMC_USIC_CH_t *const channel, const XMC_UART_CH_INPUT_t input, const uint8_t source)
{
  channel->DXCR[input] = (uint32_t)(channel->DXCR[input] & (~(USIC_CH_DX0CR_INSW_Msk|USIC_CH_DX0CR_DSEN_Msk)));
  XMC_USIC_CH_SetInputSource(channel, (XMC_USIC_CH_INPUT_t)input, source);
}

/**
 * @param channel Constant pointer to USIC channel handle of type @ref XMC_USIC_CH_t \n
 * 				  \b Range: @ref XMC_UART0_CH0, @ref XMC_UART0_CH1,@ref XMC_UART1_CH0,@ref XMC_UART1_CH1,@ref XMC_UART2_CH0,@ref XMC_UART2_CH1 @note Availability of UART1 and UART2 depends on device selection
 * @param pulse_length Length of the zero pulse in number of time quanta. \n 
 *          \b Range: 0 to 7.
 * @return None
 *
 * \par<b>Description</b><br>
 * Sets the length of zero pulse in number of time quanta. Value 0 indicates one time quanta.\n\n
 * Maximum possible is 8 time quanta with the value configured as 7.\n
 * The value is set to PCR_ASCMode register.
 * \par<b>Related APIs:</b><BR>
 * XMC_UART_CH_EnableInputInversion(), XMC_UART_CH_SetSamplePoint() \n\n\n
 *
*/
__STATIC_INLINE void XMC_UART_CH_SetPulseLength(XMC_USIC_CH_t *const channel, const uint8_t pulse_length)
{
  channel->PCR_ASCMode = (uint32_t)(channel->PCR_ASCMode & (~USIC_CH_PCR_ASCMode_PL_Msk)) |
                         ((uint32_t)pulse_length << USIC_CH_PCR_ASCMode_PL_Pos);
}

/**
 * @param channel Constant pointer to USIC channel handle of type @ref XMC_USIC_CH_t \n
 * 				  \b Range: @ref XMC_UART0_CH0, @ref XMC_UART0_CH1,@ref XMC_UART1_CH0,@ref XMC_UART1_CH1,@ref XMC_UART2_CH0,@ref XMC_UART2_CH1 @note Availability of UART1 and UART2 depends on device selection
 * @param sample_point Sample point among the number of samples. \n
 * 				  \b Range: minimum= 0, maximum= \a oversampling (DCTQ).
 * @return None
 *
 * \par<b>Description</b><br>
 * Sets the sample point among the multiple samples for each UART symbol.\n\n
 * The sample point is the one sample among number of samples set as oversampling. The value should be less than
 * the oversampling value. XMC_UART_CH_Init() sets the sample point to the sample at the centre. For 
 * example if the oversampling is 16, then the sample point is set to 9.
 * \par<b>Related APIs:</b><BR>
 * XMC_UART_CH_EnableInputInversion(), XMC_UART_CH_SetSamplePoint() \n\n\n
 */
__STATIC_INLINE void XMC_UART_CH_SetSamplePoint(XMC_USIC_CH_t *const channel, const uint32_t sample_point)
{
  channel->PCR_ASCMode = (uint32_t)((channel->PCR_ASCMode & (uint32_t)(~USIC_CH_PCR_ASCMode_SP_Msk)) |
                         (sample_point << USIC_CH_PCR_ASCMode_SP_Pos));
}

/**
 * @param channel Constant pointer to USIC channel handle of type @ref XMC_USIC_CH_t \n
 * 				  \b Range: @ref XMC_UART0_CH0, @ref XMC_UART0_CH1,@ref XMC_UART1_CH0,@ref XMC_UART1_CH1,@ref XMC_UART2_CH0,@ref XMC_UART2_CH1 @note Availability of UART1 and UART2 depends on device selection
 * @param input UART channel input stage of type @ref XMC_UART_CH_INPUT_t. \n 
 *          \b Range: @ref XMC_UART_CH_INPUT_RXD (for DX0),
 * 					@ref XMC_UART_CH_INPUT_RXD1 (for DX3), @ref XMC_UART_CH_INPUT_RXD2 (for DX5).
 * @return None
 *
 * \par<b>Description</b><br>
 * Enables input inversion for UART input data signal.\n\n
 * Polarity of the input source can be changed to provide inverted data input.
 * \par<b>Related APIs:</b><BR>
 * XMC_UART_CH_DisableInputInversion()\n\n\n
 */
__STATIC_INLINE void XMC_UART_CH_EnableInputInversion(XMC_USIC_CH_t *const channel, const XMC_UART_CH_INPUT_t input)
{
  XMC_USIC_CH_EnableInputInversion(channel, (XMC_USIC_CH_INPUT_t)input);
}

/**
 * @param channel Constant pointer to USIC channel handle of type @ref XMC_USIC_CH_t \n
 * 				  \b Range: @ref XMC_UART0_CH0, @ref XMC_UART0_CH1,@ref XMC_UART1_CH0,@ref XMC_UART1_CH1,@ref XMC_UART2_CH0,@ref XMC_UART2_CH1 @note Availability of UART1 and UART2 depends on device selection
 * @param input UART channel input stage of type @ref XMC_UART_CH_INPUT_t. \n 
 *          \b Range: @ref XMC_UART_CH_INPUT_RXD (for DX0),
 * 					@ref XMC_UART_CH_INPUT_RXD1 (for DX3), @ref XMC_UART_CH_INPUT_RXD2 (for DX5).
 * @return None
 *
 * \par<b>Description</b><br>
 * Disables input inversion for UART input data signal.\n\n
 * Resets the input data polarity for the UART input data signal.
 * \par<b>Related APIs:</b><BR>
 * XMC_UART_CH_EnableInputInversion()\n\n\n
 */
__STATIC_INLINE void XMC_UART_CH_DisableInputInversion(XMC_USIC_CH_t *const channel, const XMC_UART_CH_INPUT_t input)
{
  XMC_USIC_CH_DisableInputInversion(channel, (XMC_USIC_CH_INPUT_t)input);
}
/**
 * @param channel Constant pointer to USIC channel handle of type @ref XMC_USIC_CH_t \n
 * 				  \b Range: @ref XMC_UART0_CH0, @ref XMC_UART0_CH1,@ref XMC_UART1_CH0,@ref XMC_UART1_CH1,@ref XMC_UART2_CH0,@ref XMC_UART2_CH1 @note Availability of UART1 and UART2 depends on device selection
 * @param input UART channel input stage of type @ref XMC_UART_CH_INPUT_t. \n 
 *          \b Range: @ref XMC_UART_CH_INPUT_RXD (for DX0),
 * 					@ref XMC_UART_CH_INPUT_RXD1 (for DX3), @ref XMC_UART_CH_INPUT_RXD2 (for DX5).
 * @return None
 *
 * \par<b>Description</b><br>
 * Enables the digital filter for UART input stage.\n\n
 *
 * \par<b>Related APIs:</b><BR>
 * XMC_UART_CH_DisableInputDigitalFilter()\n\n\n
 */
__STATIC_INLINE void XMC_UART_CH_EnableInputDigitalFilter(XMC_USIC_CH_t *const channel, const XMC_UART_CH_INPUT_t input)
{
  XMC_USIC_CH_EnableInputDigitalFilter(channel, (XMC_USIC_CH_INPUT_t)input);
}
/**
 * @param channel Constant pointer to USIC channel handle of type @ref XMC_USIC_CH_t \n
 * 				  \b Range: @ref XMC_UART0_CH0, @ref XMC_UART0_CH1,@ref XMC_UART1_CH0,@ref XMC_UART1_CH1,@ref XMC_UART2_CH0,@ref XMC_UART2_CH1 @note Availability of UART1 and UART2 depends on device selection
 * @param input UART channel input stage of type @ref XMC_UART_CH_INPUT_t. \n 
 *          \b Range: @ref XMC_UART_CH_INPUT_RXD (for DX0),
 * 					@ref XMC_UART_CH_INPUT_RXD1 (for DX3), @ref XMC_UART_CH_INPUT_RXD2 (for DX5).
 * @return None
 *
 * \par<b>Description</b><br>
 * Disables the digital filter for UART input stage.\n\n
 *
 * \par<b>Related APIs:</b><BR>
 * XMC_UART_CH_EnableInputDigitalFilter()\n\n\n
 */
__STATIC_INLINE void XMC_UART_CH_DisableInputDigitalFilter(XMC_USIC_CH_t *const channel, const XMC_UART_CH_INPUT_t input)
{
  XMC_USIC_CH_DisableInputDigitalFilter(channel, (XMC_USIC_CH_INPUT_t)input);
}
/**
 * @param channel Constant pointer to USIC channel handle of type @ref XMC_USIC_CH_t \n
 * 				  \b Range: @ref XMC_UART0_CH0, @ref XMC_UART0_CH1,@ref XMC_UART1_CH0,@ref XMC_UART1_CH1,@ref XMC_UART2_CH0,@ref XMC_UART2_CH1 @note Availability of UART1 and UART2 depends on device selection
 * @param input UART channel input stage of type @ref XMC_UART_CH_INPUT_t. \n 
 *          \b Range: @ref XMC_UART_CH_INPUT_RXD (for DX0),
 * 					@ref XMC_UART_CH_INPUT_RXD1 (for DX3), @ref XMC_UART_CH_INPUT_RXD2 (for DX5).
 * @return None
 * \par<b>Description</b><br>
 * Enables synchronous input for the UART input stage.\n\n
 *
 * \par<b>Related APIs:</b><BR>
 * XMC_UART_CH_DisableInputSync(), XMC_UART_CH_EnableInputDigitalFilter()\n\n\n
 */
__STATIC_INLINE void XMC_UART_CH_EnableInputSync(XMC_USIC_CH_t *const channel, const XMC_UART_CH_INPUT_t input)
{
  XMC_USIC_CH_EnableInputSync(channel, (XMC_USIC_CH_INPUT_t)input);
}
/**
 * @param channel Constant pointer to USIC channel handle of type @ref XMC_USIC_CH_t \n
 * 				  \b Range: @ref XMC_UART0_CH0, @ref XMC_UART0_CH1,@ref XMC_UART1_CH0,@ref XMC_UART1_CH1,@ref XMC_UART2_CH0,@ref XMC_UART2_CH1 @note Availability of UART1 and UART2 depends on device selection
 * @param input UART channel input stage of type @ref XMC_UART_CH_INPUT_t. \n 
 *          \b Range: @ref XMC_UART_CH_INPUT_RXD (for DX0),
 * 					@ref XMC_UART_CH_INPUT_RXD1 (for DX3), @ref XMC_UART_CH_INPUT_RXD2 (for DX5).
 * @return None
 *
 * \par<b>Description</b><br>
 * Disables synchronous input for the UART input stage.\n\n
 *
 * \par<b>Related APIs:</b><BR>
 * XMC_UART_CH_EnableInputSync(), XMC_UART_CH_EnableInputDigitalFilter()\n\n\n
 */
__STATIC_INLINE void XMC_UART_CH_DisableInputSync(XMC_USIC_CH_t *const channel, const XMC_UART_CH_INPUT_t input)
{
  XMC_USIC_CH_DisableInputSync(channel, (XMC_USIC_CH_INPUT_t)input);
}
/**
 * @param channel Constant pointer to USIC channel handle of type @ref XMC_USIC_CH_t \n
 * 				  \b Range: @ref XMC_UART0_CH0, @ref XMC_UART0_CH1,@ref XMC_UART1_CH0,@ref XMC_UART1_CH1,@ref XMC_UART2_CH0,@ref XMC_UART2_CH1 @note Availability of UART1 and UART2 depends on device selection
 * @param input UART channel input stage of type @ref XMC_UART_CH_INPUT_t. \n 
 *          \b Range: @ref XMC_UART_CH_INPUT_RXD (for DX0),
 * 					@ref XMC_UART_CH_INPUT_RXD1 (for DX3), @ref XMC_UART_CH_INPUT_RXD2 (for DX5).
 * @param sampling_freq Input sampling frequency. \n
 *          \b Range: @ref XMC_UART_CH_INPUT_SAMPLING_FREQ_FPERIPH, @ref XMC_UART_CH_INPUT_SAMPLING_FREQ_FRACTIONAL_DIVIDER.
 * @return None
 *
 * \par<b>Description</b><br>
 * Sets the sampling frequency for the UART input stage.\n\n
 *
 * \par<b>Related APIs:</b><BR>
 * XMC_UART_CH_EnableInputSync(), XMC_UART_CH_EnableInputDigitalFilter()\n\n\n
 */
__STATIC_INLINE void XMC_UART_CH_SetInputSamplingFreq(XMC_USIC_CH_t *const channel,
                                                      const XMC_UART_CH_INPUT_t input,
                                                      const XMC_UART_CH_INPUT_SAMPLING_FREQ_t sampling_freq)
{
  XMC_USIC_CH_SetInputSamplingFreq(channel, (XMC_USIC_CH_INPUT_t)input, (XMC_USIC_CH_INPUT_SAMPLING_FREQ_t)sampling_freq);
}

/**
 * @param channel Constant pointer to USIC channel handle of type @ref XMC_USIC_CH_t \n
 *          \b Range: @ref XMC_UART0_CH0, @ref XMC_UART0_CH1,@ref XMC_UART1_CH0,@ref XMC_UART1_CH1,@ref XMC_UART2_CH0,@ref XMC_UART2_CH1 @note Availability of UART1 and UART2 depends on device selection
 * @return None
 *
 * \par<b>Description</b><br>
 * Enable data transmission.\n\n
 * Use this function in combination with XMC_UART_CH_DisableDataTransmission() to fill the FIFO and send the FIFO content without gaps in the transmission.
 * FIFO is filled using XMC_USIC_CH_TXFIFO_PutData().
 * @note If you need more control over the start of transmission use XMC_USIC_CH_SetStartTransmisionMode()
 *
 * \par<b>Related APIs:</b><BR>
 * XMC_UART_CH_DisableDataTransmission()\n\n\n
 */
__STATIC_INLINE void XMC_UART_CH_EnableDataTransmission(XMC_USIC_CH_t *const channel)
{
  XMC_USIC_CH_SetStartTransmisionMode(channel, XMC_USIC_CH_START_TRANSMISION_ON_TDV);
}

/**
 * @param channel Constant pointer to USIC channel handle of type @ref XMC_USIC_CH_t \n
 *          \b Range: @ref XMC_UART0_CH0, @ref XMC_UART0_CH1,@ref XMC_UART1_CH0,@ref XMC_UART1_CH1,@ref XMC_UART2_CH0,@ref XMC_UART2_CH1 @note Availability of UART1 and UART2 depends on device selection
 * @return None
 *
 * \par<b>Description</b><br>
 * Disable data transmission.\n\n
 * Use this function in combination with XMC_UART_CH_EnableDataTransmission() to fill the FIFO and send the FIFO content without gaps in the transmission.
 * FIFO is filled using XMC_USIC_CH_TXFIFO_PutData().
 *
 * \par<b>Related APIs:</b><BR>
 * XMC_UART_CH_EnableDataTransmission()\n\n\n
 */
__STATIC_INLINE void XMC_UART_CH_DisableDataTransmission(XMC_USIC_CH_t *const channel)
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
