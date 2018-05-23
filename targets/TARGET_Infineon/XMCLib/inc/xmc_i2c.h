/**
 * @file xmc_i2c.h
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
 *     - Initial <br>
 *      
 * 2015-05-20:
 *     - Description updated <br>
 *     - Added XMC_I2C_CH_TriggerServiceRequest() and XMC_I2C_CH_SelectInterruptNodePointer() <br>
 *
 * 2015-06-20:
 *     - Removed version macros and declaration of GetDriverVersion API <br>
 *
 * 2015-08-27:
 *     - Added APIs for external input for BRG configuration:XMC_I2C_CH_ConfigExternalInputSignalToBRG() <br>
 *
 * 2015-09-01:
 *     - Added APIs for enabling or disabling the ACK response to a 0x00 slave address: XMC_I2C_CH_EnableSlaveAcknowledgeTo00() and
 *       XMC_I2C_CH_DisableSlaveAcknowledgeTo00(). <br>
 *     - Modified XMC_I2C_CH_SetInputSource() API for avoiding complete DXCR register overwriting. <br>
 *     - Modified XMC_I2C_CH_EVENT_t enum for supporting XMC_I2C_CH_EnableEvent() and XMC_I2C_CH_DisableEvent()
 *       for supporting multiple events configuration <br>
 *
 * 2015-10-02:
 *     - Fix 10bit addressing
 *
 * 2015-10-07:
 *     - Fix register access in XMC_I2C_CH_EnableSlaveAcknowledgeTo00() and XMC_I2C_CH_DisableSlaveAcknowledgeTo00() APIs.
 *     - Naming of APIs modified: from XMC_I2C_CH_EnableSlaveAcknowledgeTo00() to  XMC_I2C_CH_EnableAcknowledgeAddress0()
 *       and from XMC_I2C_CH_DisableSlaveAcknowledgeTo00() to XMC_I2C_CH_DisableAcknowledgeAddress0().
 *
 * 2016-05-20:
 *     - Added XMC_I2C_CH_EnableDataTransmission() and XMC_I2C_CH_DisableDataTransmission()
 *
 * 2016-08-17:
 *     - Improved documentation of slave address passing
 *
 * @endcond 
 *
 */

#ifndef XMC_I2C_H
#define XMC_I2C_H

/*******************************************************************************
 * HEADER FILES
 *******************************************************************************/

#include "xmc_usic.h"

/**
 * @addtogroup XMClib XMC Peripheral Library
 * @{
 */

/**
 * @addtogroup I2C
 * @brief Inter Integrated Circuit(IIC) driver for the XMC microcontroller family. 
 *
 * USIC IIC Features: <br>
 *  * Two-wire interface, with one line for shift clock transfer and synchronization (shift clock SCL), the other one for the data transfer (shift data SDA) <br>
 *	* Communication in standard mode (100 kBit/s) or in fast mode (up to 400 kBit/s) <br>
 *	* Support of 7-bit addressing, as well as 10-bit addressing <br>
 *	* Master mode operation, where the IIC controls the bus transactions and provides the clock signal. <br>
 *	* Slave mode operation, where an external master controls the bus transactions and provides the clock signal.<br>
 *	* Multi-master mode operation, where several masters can be connected to the bus and bus arbitration can take place, i.e. the IIC module can be master or slave. <br>
	  The master/slave operation of an IIC bus participant can change from frame to frame. <br>
 *	* Efficient frame handling (low software effort), also allowing DMA transfers <br>
 *	* Powerful interrupt handling due to multitude of indication flags <br>
 * @{
 */

/*******************************************************************************
 * MACROS
 *******************************************************************************/

#if defined(USIC0)
#define XMC_I2C0_CH0 XMC_USIC0_CH0                   /**< USIC0 channel 0 base address */
#define XMC_I2C0_CH1 XMC_USIC0_CH1                   /**< USIC0 channel 1 base address */
#endif

#if defined(USIC1)
#define XMC_I2C1_CH0 XMC_USIC1_CH0                   /**< USIC1 channel 0 base address */
#define XMC_I2C1_CH1 XMC_USIC1_CH1                   /**< USIC1 channel 1 base address */
#endif

#if defined(USIC2)
#define XMC_I2C2_CH0 XMC_USIC2_CH0                   /**< USIC2 channel 0 base address */
#define XMC_I2C2_CH1 XMC_USIC2_CH1                   /**< USIC2 channel 1 base address */
#endif

#define XMC_I2C_10BIT_ADDR_GROUP       (0x7800U)	 /**< Value to verify the address is 10-bit or not */

/*******************************************************************************
 * ENUMS
 *******************************************************************************/

/**
 * @brief I2C Status
 */
typedef enum XMC_I2C_CH_STATUS
{
  XMC_I2C_CH_STATUS_OK,      /**< Status OK */
  XMC_I2C_CH_STATUS_ERROR,   /**< Status ERROR */
  XMC_I2C_CH_STATUS_BUSY     /**< Status BUSY */
} XMC_I2C_CH_STATUS_t;

/**
 * @brief I2C status
 */
typedef enum XMC_I2C_CH_STATUS_FLAG
{
  XMC_I2C_CH_STATUS_FLAG_SLAVE_SELECT = USIC_CH_PSR_IICMode_SLSEL_Msk,                     /**< Slave select status */
  XMC_I2C_CH_STATUS_FLAG_WRONG_TDF_CODE_FOUND = USIC_CH_PSR_IICMode_WTDF_Msk,              /**< Wrong TDF status */
  XMC_I2C_CH_STATUS_FLAG_START_CONDITION_RECEIVED = USIC_CH_PSR_IICMode_SCR_Msk,           /**< Start condition received status */
  XMC_I2C_CH_STATUS_FLAG_REPEATED_START_CONDITION_RECEIVED = USIC_CH_PSR_IICMode_RSCR_Msk, /**< Repeated start condition received status */
  XMC_I2C_CH_STATUS_FLAG_STOP_CONDITION_RECEIVED = USIC_CH_PSR_IICMode_PCR_Msk,            /**< Stop condition received status */
  XMC_I2C_CH_STATUS_FLAG_NACK_RECEIVED = USIC_CH_PSR_IICMode_NACK_Msk,                     /**< NACK received status */
  XMC_I2C_CH_STATUS_FLAG_ARBITRATION_LOST = USIC_CH_PSR_IICMode_ARL_Msk,                   /**< Arbitration lost status */
  XMC_I2C_CH_STATUS_FLAG_SLAVE_READ_REQUESTED = USIC_CH_PSR_IICMode_SRR_Msk,               /**< Slave read requested status */
  XMC_I2C_CH_STATUS_FLAG_ERROR = USIC_CH_PSR_IICMode_ERR_Msk,                              /**< Error status */
  XMC_I2C_CH_STATUS_FLAG_ACK_RECEIVED = USIC_CH_PSR_IICMode_ACK_Msk,                       /**< ACK received status */
  XMC_I2C_CH_STATUS_FLAG_RECEIVER_START_INDICATION = USIC_CH_PSR_IICMode_RSIF_Msk,         /**< Receive start indication status */
  XMC_I2C_CH_STATUS_FLAG_DATA_LOST_INDICATION = USIC_CH_PSR_IICMode_DLIF_Msk,              /**< Data lost indication status */
  XMC_I2C_CH_STATUS_FLAG_TRANSMIT_SHIFT_INDICATION = USIC_CH_PSR_IICMode_TSIF_Msk,         /**< Transmit shift indication status */
  XMC_I2C_CH_STATUS_FLAG_TRANSMIT_BUFFER_INDICATION = USIC_CH_PSR_IICMode_TBIF_Msk,        /**< Transmit buffer indication status */
  XMC_I2C_CH_STATUS_FLAG_RECEIVE_INDICATION = USIC_CH_PSR_IICMode_RIF_Msk,                 /**< Receive indication status */
  XMC_I2C_CH_STATUS_FLAG_ALTERNATIVE_RECEIVE_INDICATION = USIC_CH_PSR_IICMode_AIF_Msk,     /**< Alternate receive indication status */
  XMC_I2C_CH_STATUS_FLAG_BAUD_RATE_GENERATOR_INDICATION = USIC_CH_PSR_IICMode_BRGIF_Msk    /**< Baud rate generator indication status */
} XMC_I2C_CH_STATUS_FLAG_t;

/**
 * @brief I2C receiver status. The received data byte is available at the bit
 * positions RBUF[7:0], whereas the additional information is monitored at the bit positions
*  RBUF[12:8].
 */
typedef enum XMC_I2C_CH_RECEIVER_STATUS_FLAG
{
  XMC_I2C_CH_RECEIVER_STATUS_FLAG_ACK = 0x1U,      /**< Bit 8:  Value of Received Acknowledgement bit */
  XMC_I2C_CH_RECEIVER_STATUS_FLAG_FIN = 0x2U,      /**< Bit 9:  A 1 at this bit position indicates that after a (repeated) start condition 
                                                                followed by the address reception the first data byte of a new frame has 
                                                                been received. A 0 at this bit position indicates further data bytes */
  XMC_I2C_CH_RECEIVER_STATUS_FLAG_MODE = 0x4U,     /**< Bit 10: A 0 at this bit position indicates that the data byte has been received 
                                                                when the device has been in slave mode, whereas a 1 indicates a reception in master mode */
  XMC_I2C_CH_RECEIVER_STATUS_FLAG_ERR = 0x8U,      /**< Bit 11: A 1 at this bit position indicates an incomplete/erroneous 
                                                                data byte in the receive buffer */
  XMC_I2C_CH_RECEIVER_STATUS_FLAG_ADR = 0x10       /**< Bit 12: A 0 at this bit position indicates that the programmed address 
                                                                has been received. A 1 indicates a general call address. */
} XMC_I2C_CH_RECEIVER_STATUS_FLAG_t;

/**
 * @brief I2C commands
 */
typedef enum XMC_I2C_CH_CMD
{
  XMC_I2C_CH_CMD_WRITE,                            /**< I2C Command Write */
  XMC_I2C_CH_CMD_READ                              /**< I2C Command Read */
} XMC_I2C_CH_CMD_t;

/**
 * @brief I2C events
 */
typedef enum XMC_I2C_CH_EVENT
{
  XMC_I2C_CH_EVENT_RECEIVE_START       = USIC_CH_CCR_RSIEN_Msk,  /**< Receive start event */
  XMC_I2C_CH_EVENT_DATA_LOST           = USIC_CH_CCR_DLIEN_Msk,  /**< Data lost event */
  XMC_I2C_CH_EVENT_TRANSMIT_SHIFT      = USIC_CH_CCR_TSIEN_Msk,  /**< Transmit shift event */
  XMC_I2C_CH_EVENT_TRANSMIT_BUFFER     = USIC_CH_CCR_TBIEN_Msk,  /**< Transmit buffer event */
  XMC_I2C_CH_EVENT_STANDARD_RECEIVE    = USIC_CH_CCR_RIEN_Msk,   /**< Receive event */
  XMC_I2C_CH_EVENT_ALTERNATIVE_RECEIVE = USIC_CH_CCR_AIEN_Msk,   /**< Alternate receive event */
  XMC_I2C_CH_EVENT_BAUD_RATE_GENERATOR = USIC_CH_CCR_BRGIEN_Msk, /**< Baudrate generator event */

  XMC_I2C_CH_EVENT_START_CONDITION_RECEIVED = USIC_CH_PCR_IICMode_SCRIEN_Msk,             /**< Start condition received event */
  XMC_I2C_CH_EVENT_REPEATED_START_CONDITION_RECEIVED = USIC_CH_PCR_IICMode_RSCRIEN_Msk,   /**< Repeated start condition received event */
  XMC_I2C_CH_EVENT_STOP_CONDITION_RECEIVED = USIC_CH_PCR_IICMode_PCRIEN_Msk,              /**< Stop condition received event */
  XMC_I2C_CH_EVENT_NACK = USIC_CH_PCR_IICMode_NACKIEN_Msk,                                /**< NACK received event */
  XMC_I2C_CH_EVENT_ARBITRATION_LOST = USIC_CH_PCR_IICMode_ARLIEN_Msk,                     /**< Arbitration lost event */
  XMC_I2C_CH_EVENT_SLAVE_READ_REQUEST = USIC_CH_PCR_IICMode_SRRIEN_Msk,                   /**< Slave read request event */
  XMC_I2C_CH_EVENT_ERROR = USIC_CH_PCR_IICMode_ERRIEN_Msk,	                              /**< Error condition event */
  XMC_I2C_CH_EVENT_ACK = USIC_CH_PCR_IICMode_ACKIEN_Msk                                   /**< ACK received event */
} XMC_I2C_CH_EVENT_t;

/**
 * @brief I2C input stage selection
 */
typedef enum XMC_I2C_CH_INPUT
{
  XMC_I2C_CH_INPUT_SDA = 0U,   /**< selection of sda input stage */
#if UC_FAMILY == XMC1
  XMC_I2C_CH_INPUT_SDA1 = 3U,
  XMC_I2C_CH_INPUT_SDA2 = 5U,
#endif  
  XMC_I2C_CH_INPUT_SCL = 1U,  /**< selection of scl input stage */
#if UC_FAMILY == XMC1
  XMC_I2C_CH_INPUT_SCL1 = 4U
#endif  
} XMC_I2C_CH_INPUT_t;

/**
 * I2C channel interrupt node pointers
 */
typedef enum XMC_I2C_CH_INTERRUPT_NODE_POINTER
{
  XMC_I2C_CH_INTERRUPT_NODE_POINTER_TRANSMIT_SHIFT      = XMC_USIC_CH_INTERRUPT_NODE_POINTER_TRANSMIT_SHIFT, /**< Node pointer for transmit shift interrupt */
  XMC_I2C_CH_INTERRUPT_NODE_POINTER_TRANSMIT_BUFFER     = XMC_USIC_CH_INTERRUPT_NODE_POINTER_TRANSMIT_BUFFER, /**< Node pointer for transmit buffer interrupt */
  XMC_I2C_CH_INTERRUPT_NODE_POINTER_RECEIVE             = XMC_USIC_CH_INTERRUPT_NODE_POINTER_RECEIVE,  /**< Node pointer for receive interrupt */
  XMC_I2C_CH_INTERRUPT_NODE_POINTER_ALTERNATE_RECEIVE   = XMC_USIC_CH_INTERRUPT_NODE_POINTER_ALTERNATE_RECEIVE,  /**< Node pointer for alternate receive interrupt */
  XMC_I2C_CH_INTERRUPT_NODE_POINTER_PROTOCOL            = XMC_USIC_CH_INTERRUPT_NODE_POINTER_PROTOCOL   /**< Node pointer for protocol related interrupts */
} XMC_I2C_CH_INTERRUPT_NODE_POINTER_t;

/*******************************************************************************
 * DATA STRUCTURES
 *******************************************************************************/
/**
 * @brief I2C_CH configuration structure
 */
typedef struct XMC_I2C_CH_CONFIG
{
  uint32_t baudrate;   /**< baud rate configuration upto max of 400KHz */
  uint16_t address;    /**< slave address 
                            A 7-bit address needs to be left shifted it by 1.
                            A 10-bit address needs to be ORed with XMC_I2C_10BIT_ADDR_GROUP. */
} XMC_I2C_CH_CONFIG_t;

/*******************************************************************************
 * API PROTOTYPES
 *******************************************************************************/

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @param channel Constant pointer to USIC channel structure of type @ref XMC_USIC_CH_t
 * @param config  Constant pointer to I2C channel config structure of type @ref XMC_I2C_CH_CONFIG_t
 *
 * @return None<br>
 *
 * \par<b>Description:</b><br>
 * Initializes the I2C \a channel.<br>
 *
 * \par
 * Configures the data format in SCTR register. Sets the slave address, baud rate. Enables transmit data valid, clears status flags
 * and disables parity generation.<br>
 *
 * \par<b>Related APIs:</b><br>
 * XMC_USIC_CH_Enable()\n\n
 */

void XMC_I2C_CH_Init(XMC_USIC_CH_t *const channel, const XMC_I2C_CH_CONFIG_t *const config);

/**
 * @param channel Constant pointer to USIC channel structure of type @ref XMC_USIC_CH_t
 * @param rate  baud rate of I2C channel
 *
 * @return None<br>
 *
 * \par<b>Description:</b><br>
 * Sets the rate of I2C \a channel.
 *
 * \par<b>Note:</b><br>
 * Standard over sampling is considered if rate <= 100KHz and fast over sampling is considered if rate > 100KHz.<br>
 *
 * \par<b>Related APIs:</b><br>
 * XMC_USIC_CH_SetBaudrate()\n\n
 */
XMC_I2C_CH_STATUS_t XMC_I2C_CH_SetBaudrate(XMC_USIC_CH_t *const channel, const uint32_t rate);

/**
 * @param channel Constant pointer to USIC channel structure of type @ref XMC_USIC_CH_t
 *
 * @return None<br>
 *
 * \par<b>Description:</b><br>
 * Starts the I2C \a channel.
 *
 * \par
 * Sets the USIC input operation mode to I2C mode using CCR register.
 *
 * \par<b>Related APIs:</b><br>
 * XMC_USIC_CH_SetMode()\n\n
 */
__STATIC_INLINE void XMC_I2C_CH_Start(XMC_USIC_CH_t *const channel)
{
  XMC_USIC_CH_SetMode(channel, XMC_USIC_CH_OPERATING_MODE_I2C);
}

/**
 * @param channel Constant pointer to USIC channel structure of type @ref XMC_USIC_CH_t
 *
 * @return @ref XMC_I2C_CH_STATUS_t<br>
 *
 * \par<b>Description:</b><br>
 * Stops the I2C \a channel.<br>
 *
 * \par
 * Sets the USIC input operation to IDLE mode using CCR register.
 *
 * \par<b>Related APIs:</b><br>
 * XMC_USIC_CH_SetMode()\n\n
 */
XMC_I2C_CH_STATUS_t XMC_I2C_CH_Stop(XMC_USIC_CH_t *const channel);

/**
 * @param channel Constant pointer to USIC channel structure of type @ref XMC_USIC_CH_t
 * @param service_request Service request number in the range of 0-5
 * @return None<br>
 *
 * \par<b>Description:</b><br>
 * Sets the interrupt node for protocol interrupt.<br>
 *
 * \par
 * To generate interrupt for an event, node pointer should be configured with service request number(SR0, SR1..SR5).
 * The NVIC node gets linked to the interrupt event by doing so.<br>
 *
 * \par<b>Note:</b><br>
 * NVIC node should be separately enabled to generate the interrupt. After setting the node pointer, desired event must be enabled.
 *
 * \par<b>Related APIs:</b><br>
 * XMC_I2C_CH_EnableEvent(), NVIC_SetPriority(), NVIC_EnableIRQ(), XMC_I2C_CH_SetInputSource()<br>
 */
__STATIC_INLINE void XMC_I2C_CH_SetInterruptNodePointer(XMC_USIC_CH_t *const channel,
                                                        const uint8_t service_request)
{
  XMC_USIC_CH_SetInterruptNodePointer(channel, XMC_USIC_CH_INTERRUPT_NODE_POINTER_PROTOCOL, service_request);
}


/**
 * @param channel Pointer to USIC channel handler of type @ref XMC_USIC_CH_t \n
 * 				  \b Range: @ref XMC_I2C0_CH0, @ref XMC_I2C0_CH1,@ref XMC_I2C1_CH0,@ref XMC_I2C1_CH1,@ref XMC_I2C2_CH0,@ref XMC_I2C2_CH1 @note Availability of I2C1 and I2C2 depends on device selection
 * @param  interrupt_node Interrupt node pointer to be configured. \n
 * 						  \b Range: @ref XMC_I2C_CH_INTERRUPT_NODE_POINTER_TRANSMIT_SHIFT,
 * 						  			@ref XMC_I2C_CH_INTERRUPT_NODE_POINTER_TRANSMIT_BUFFER etc.
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
 * XMC_I2C_CH_EnableEvent() \n\n\n
 */
__STATIC_INLINE void XMC_I2C_CH_SelectInterruptNodePointer(XMC_USIC_CH_t *const channel,
                                                           const XMC_I2C_CH_INTERRUPT_NODE_POINTER_t interrupt_node,
                                                           const uint32_t service_request)
{
  XMC_USIC_CH_SetInterruptNodePointer(channel, (XMC_USIC_CH_INTERRUPT_NODE_POINTER_t)interrupt_node,
		                                       (uint32_t)service_request);
}

/**
 * @param  channel Pointer to USIC channel handler of type @ref XMC_USIC_CH_t \n
 * 				   \b Range: @ref XMC_I2C0_CH0, @ref XMC_I2C0_CH1,@ref XMC_I2C1_CH0,@ref XMC_I2C1_CH1,@ref XMC_I2C2_CH0,@ref XMC_I2C2_CH1 @note Availability of I2C1 and I2C2 depends on device selection
 * @param  service_request_line service request number of the event to be triggered. \n
 * 			\b Range: 0 to 5.
 * @return None
 *
 * \par<b>Description</b><br>
 * Trigger a I2C interrupt service request.\n\n
 * When the I2C service request is triggered, the NVIC interrupt associated with it will be
 * generated if enabled.
 *
 * \par<b>Related APIs:</b><BR>
 * XMC_I2C_CH_SelectInterruptNodePointer() \n\n\n
 */
__STATIC_INLINE void XMC_I2C_CH_TriggerServiceRequest(XMC_USIC_CH_t *const channel, const uint32_t service_request_line)
{
  XMC_USIC_CH_TriggerServiceRequest(channel, (uint32_t)service_request_line);
}

/**
 * @param channel Constant pointer to USIC channel structure of type @ref XMC_USIC_CH_t
 * @param input I2C channel input stage of type @ref XMC_I2C_CH_INPUT_t
 * @param source Input source select for the input stage(0->DX0A, 1->DX1A, .. 7->DX7G)
 * @return None<br>
 *
 * \par<b>Description:</b><br>
 * Sets the input source for I2C \a channel.<br>
 * Defines the input stage for the corresponding input line.
 *
 * @note After configuring the input source for corresponding channel, interrupt node pointer is set.
 *
 * \par<b>Related APIs:</b><br>
 * XMC_USIC_CH_SetInptSource(), XMC_USIC_CH_SetInterruptNodePointer()
 *
 */
__STATIC_INLINE void XMC_I2C_CH_SetInputSource(XMC_USIC_CH_t *const channel, const XMC_I2C_CH_INPUT_t input, const uint8_t source)
{
  channel->DXCR[input] =  (uint32_t)(channel->DXCR[input] & (~USIC_CH_DX0CR_INSW_Msk)) | USIC_CH_DX0CR_DSEN_Msk;
  XMC_USIC_CH_SetInputSource(channel, (XMC_USIC_CH_INPUT_t)input, source);
}

/**
 * @param channel Constant pointer to USIC channel structure of type @ref XMC_USIC_CH_t
 * @param address I2C slave address
 * @return None<br>
 *
 * \par<b>Description:</b><br>
 * Sets the I2C \a channel slave address.<br>
 *
 * \par
 * Address is set in PCR_IICMode register by checking if it is in 10-bit address group or 7-bit address group.
 * (If first five bits of address are assigned with 0xF0, then address mode is 10-bit mode otherwise it is 7-bit mode)\n
 * @note A 7-bit address should include an additional bit at the LSB for read/write indication. For example, address 0x05 should
 * be provided as 0x0a. A 10-bit address should be provided with the identifier 0b11110xx at the most significant bits. For example,
 * address 0x305 should be provided as 0x7b05(bitwise OR with 0x7800).
 *
 * \par<b>Related APIs:</b><br>
 * XMC_I2C_CH_GetSlaveAddress()\n\n
 */
void XMC_I2C_CH_SetSlaveAddress(XMC_USIC_CH_t *const channel, const uint16_t address);

/**
 * @param channel Constant pointer to USIC channel handler of type @ref XMC_USIC_CH_t
 * @return uint16_t Slave address<br>
 *
 * \par<b>Description:</b><br>
 * Gets the I2C \a channel slave address.<br>
 *
 * \par
 * Returns the address using PCR_IICMode register by checking if it is in 10-bit address group or 7-bit address group.<br>
 * (If first five bits of address are assigned with 0xF0, then address mode is considered as 10-bit mode otherwise it is 7-bit mode)\n
 * @note A 7-bit address will include an additional bit at the LSB. For example, address 0x05 will be returned as 0x0a.
 * 10-bit address will not include the 10-bit address identifier 0b11110xx at the most signifcant bits.
 *
 * \par<b>Related APIs:</b><br>
 * XMC_I2C_CH_SetSlaveAddress()\n\n
 */
uint16_t XMC_I2C_CH_GetSlaveAddress(const XMC_USIC_CH_t *const channel);

/**
 * @param channel Constant pointer to USIC channel structure of type @ref XMC_USIC_CH_t
 * @param addr I2C master address
 * @param command read/write command
 * @return None<br>
 *
 * \par<b>Description:</b><br>
 * Starts the I2C master \a channel.<br>
 *
 * \par
 * Sends the Start condition with read/write command by updating IN/TBUF register based on FIFO/non-FIFO modes.\n
 * @note Address(addr) should reserve an additional bit at the LSB for read/write indication. For example, address 0x05 should
 * be provided as 0x0a. If the address is 10-bit, only most significant bits with the 10-bit identifier should be sent using this function.
 * For example, if the 10-bit address is 0x305, the address should be provided as 0xf6(prepend with 0b11110, upper two bits of address 0b11,
 * followed by 1-bit field for read/write).
 *
 * \par<b>Related APIs:</b><br>
 * XMC_I2C_CH_MasterTransmit(), XMC_USIC_CH_GetTransmitBufferStatus()\n\n
 */
void XMC_I2C_CH_MasterStart(XMC_USIC_CH_t *const channel, const uint16_t addr, const XMC_I2C_CH_CMD_t command);

/**
 * @param channel Constant pointer to USIC channel structure of type @ref XMC_USIC_CH_t
 * @param addr I2C master address
 * @param command read/write command
 * @return None<br>
 *
 * \par<b>Description:</b><br>
 * Sends the repeated start condition from I2C master \a channel.<br>
 *
 * \par
 * Sends the repeated start condition with read/write command by updating IN/TBUF register based on FIFO/non-FIFO modes.\n
 * @note Address(addr) should reserve an additional bit at the LSB for read/write indication. For example, address 0x05 should
 * be provided as 0x0a. If the address is 10-bit, only most significant bits with the 10-bit identifier should be sent using this function.
 * For example, if the 10-bit address is 0x305, the address should be provided as 0xf6(prepend with 0b11110, upper two bits of address 0b11,
 * followed by 1-bit field for read/write).
 *
 * \par<b>Related APIs:</b><br>
 * XMC_I2C_CH_MasterTransmit(), XMC_USIC_CH_GetTransmitBufferStatus()\n\n
 */
void XMC_I2C_CH_MasterRepeatedStart(XMC_USIC_CH_t *const channel, const uint16_t addr, const XMC_I2C_CH_CMD_t command);

/**
 * @param channel Constant pointer to USIC channel structure of type @ref XMC_USIC_CH_t
 * @return None<br>
 *
 * \par<b>Description:</b><br>
 * Stops the I2C master \a channel.<br>
 *
 * \par
  * Reads the transmit buffer status is busy or not and thereby updates IN/TBUF register based on FIFO/non-FIFO modes using Stop command.
 *
 * \par<b>Related APIs:</b><br>
 * XMC_I2C_CH_MasterTransmit(), XMC_USIC_CH_GetTransmitBufferStatus()\n\n
 */
void XMC_I2C_CH_MasterStop(XMC_USIC_CH_t *const channel);

/**
 * @param channel Constant pointer to USIC channel structure of type @ref XMC_USIC_CH_t
 * @param data data to transmit from I2C \a channel
 * @return None<br>
 *
 * \par<b>Description:</b><br>
 * Transmit the data from the I2C master \a channel.<br>
 *
 * \par
 * Reads the transmit buffer status is busy or not and thereby updates IN/TBUF register based on FIFO/non-FIFO modes using Master Send command.
 *
 * \par<b>Related APIs:</b><br>
 * XMC_USIC_CH_GetTransmitBufferStatus()\n\n
 */
void XMC_I2C_CH_MasterTransmit(XMC_USIC_CH_t *const channel, const uint8_t data);

/**
 * @param channel Constant pointer to USIC channel structure of type @ref XMC_USIC_CH_t
 * @param data data to transmit from I2C \a channel
 * @return None<br>
 *
 * \par<b>Description:</b><br>
 * Transmit the data from the I2C slave \a channel.<br>
 *
 * \par
 * Reads the transmit buffer status is busy or not and thereby updates IN/TBUF register based on FIFO/non-FIFO modes using Slave Send command.
 *
 * \par<b>Related APIs:</b><br>
 * XMC_USIC_CH_GetTransmitBufferStatus(),XMC_I2C_CH_ClearStatusFlag()\n\n
 */
void XMC_I2C_CH_SlaveTransmit(XMC_USIC_CH_t *const channel, const uint8_t data);

/**
 * @param channel Constant pointer to USIC channel structure of type @ref XMC_USIC_CH_t
 * @return None<br>
 *
 * \par<b>Description:</b><br>
 * Sends the Ack request from I2C master \a channel.<br>
 *
 * \par
* Reads the transmit buffer status is busy or not and thereby updates IN/TBUF register based on FIFO/non-FIFO modes using Master Receive Ack command.
 *
 * \par<b>Related APIs:</b><br>
 * XMC_I2C_CH_MasterTransmit()\n\n
 */
void XMC_I2C_CH_MasterReceiveAck(XMC_USIC_CH_t *const channel);

/**
 * @param channel Constant pointer to USIC channel structure of type @ref XMC_USIC_CH_t
 * @return None<br>
 *
 * \par<b>Description:</b><br>
 * Sends the Nack request from I2C master \a channel.<br>
 *
 * \par
 * Reads the transmit buffer status is busy or not and thereby updates IN/TBUF register based on FIFO/non-FIFO modes using Master Receive Nack command.
 *
 * \par<b>Related APIs:</b><br>
 * XMC_I2C_CH_MasterTransmit()\n\n
 */
void XMC_I2C_CH_MasterReceiveNack(XMC_USIC_CH_t *const channel);

/**
 * @param channel Constant pointer to USIC channel structure of type @ref XMC_USIC_CH_t
 * @return uint8_t OUTR/RBUF register data<br>
 *
 * \par<b>Description:</b><br>
 * Reads the data from I2C \a channel.<br>
 *
 * \par
 * Data is read by using OUTR/RBUF register based on FIFO/non-FIFO modes.
 *
 * \par<b>Related APIs:</b><br>
 * XMC_I2C_CH_MasterTransmit()\n\n
 */
uint8_t XMC_I2C_CH_GetReceivedData(const XMC_USIC_CH_t *const channel);

/**
 * @param channel Constant pointer to USIC channel structure of type @ref XMC_USIC_CH_t
 * @return uint8_t Receiver status flag<br>
 *
 * \par<b>Description:</b><br>
 * Gets the receiver status of I2C \a channel using RBUF register of bits 8-12 which gives information about receiver status.
 *
 * \par<b>Related APIs:</b><br>
 * XMC_I2C_CH_MasterTransmit()\n\n
 */
__STATIC_INLINE uint8_t XMC_I2C_CH_GetReceiverStatusFlag(XMC_USIC_CH_t *const channel)
{
  return((uint8_t)((channel->RBUF) >> 8U));
}

/**
 * @param channel Constant pointer to USIC channel structure of type @ref XMC_USIC_CH_t
 * @param event ORed values of @ref XMC_I2C_CH_EVENT_t enum
 * @return None<br>
 *
 * \par<b>Description:</b><br>
 * Enables the input parameter @ref XMC_I2C_CH_EVENT_t event using PCR_IICMode register.
 *
 * \par<b>Related APIs:</b><br>
 * XMC_I2C_CH_DisableEvent()\n\n
 */
void XMC_I2C_CH_EnableEvent(XMC_USIC_CH_t *const channel, uint32_t event);

/**
 * @param channel Constant pointer to USIC channel structure of type @ref XMC_USIC_CH_t
 * @param event ORed values of @ref XMC_I2C_CH_EVENT_t enum
 * @return None<br>
 *
 * \par<b>Description:</b><br>
 * Disables the input parameter @ref XMC_I2C_CH_EVENT_t event using PCR_IICMode register.
 *
 * \par<b>Related APIs:</b><br>
 * XMC_I2C_CH_EnableEvent()\n\n
 */
void XMC_I2C_CH_DisableEvent(XMC_USIC_CH_t *const channel, uint32_t event);

/**
 * @param channel Constant pointer to USIC channel structure of type @ref XMC_USIC_CH_t
 * @return uint32_t Status byte<br>
 *
 * \par<b>Description:</b><br>
 * Retrieves the status byte of I2C \a channel using PSR_IICMode register.\n
 *
 * \par<b>Related APIs:</b><br>
 * XMC_I2C_CH_ClearStatusFlag()\n\n
 */
__STATIC_INLINE uint32_t XMC_I2C_CH_GetStatusFlag(XMC_USIC_CH_t *const channel)
{
  return (channel->PSR_IICMode);
}

/**
 * @param channel Constant pointer to USIC channel structure of type @ref XMC_USIC_CH_t
 * @param flag Status flag
 * @return None<br>
 *
 * \par<b>Description:</b><br>
 * Clears the status flag of I2C \a channel by setting the input parameter \a flag in PSCR register.
 *
 * \par<b>Related APIs:</b><br>
 * XMC_I2C_CH_GetStatusFlag()\n\n
 */
__STATIC_INLINE void XMC_I2C_CH_ClearStatusFlag(XMC_USIC_CH_t *const channel, uint32_t flag)
{
  channel->PSCR |= flag;
}

/**
 * @param  channel Pointer to USIC channel handler of type @ref XMC_USIC_CH_t \n
 * 				   \b Range: @ref XMC_I2C0_CH0, @ref XMC_I2C0_CH1,@ref XMC_I2C1_CH0,@ref XMC_I2C1_CH1,@ref XMC_I2C2_CH0,@ref XMC_I2C2_CH1 @note Availability of I2C1 and I2C2 depends on device selection
 * @param  pdiv Desired divider for the external frequency input. \b Range: minimum value = 1, maximum value = 1024 \n
 * @param  oversampling Required oversampling. The value indicates the number of time quanta for one symbol of data. \n
 * 						This can be related to the number of samples for each logic state of the data signal. \n
 * 						\b Range: 1 to 32. Value should be chosen based on the protocol used.
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
__STATIC_INLINE void XMC_I2C_CH_ConfigExternalInputSignalToBRG(XMC_USIC_CH_t *const channel,
		                                                       const uint16_t pdiv,
											                   const uint32_t oversampling,
											                   const XMC_USIC_CH_INPUT_COMBINATION_MODE_t combination_mode)
{
  XMC_USIC_CH_ConfigExternalInputSignalToBRG(channel,pdiv,oversampling,combination_mode);
}

/**
 * @param channel Constant pointer to USIC channel structure of type @ref XMC_USIC_CH_t
 * @return None
 *
 * \par<b>Description:</b><br>
 * Retrieves the status byte of I2C \a channel using PSR_IICMode register.\n
 *
 * \par<b>Related APIs:</b><br>
 * XMC_I2C_CH_DisableAcknowledgeAddress0()\n\n
 */
__STATIC_INLINE void XMC_I2C_CH_EnableAcknowledgeAddress0(XMC_USIC_CH_t *const channel)
{
  channel->PCR_IICMode |= USIC_CH_PCR_IICMode_ACK00_Msk;
}

/**
 * @param channel Constant pointer to USIC channel structure of type @ref XMC_USIC_CH_t
 * @return None
 *
 * \par<b>Description:</b><br>
 * This bit defines that slave device should not be sensitive to the slave address 00H.\n
 *
 * \par<b>Related APIs:</b><br>
 * XMC_I2C_CH_EnableAcknowledgeAddress0()\n\n
 */
__STATIC_INLINE void XMC_I2C_CH_DisableAcknowledgeAddress0(XMC_USIC_CH_t *const channel)
{
  channel->PCR_IICMode &= ~USIC_CH_PCR_IICMode_ACK00_Msk;
}

/**
 * @param channel Constant pointer to USIC channel handle of type @ref XMC_USIC_CH_t \n
 *          \b Range: @ref XMC_I2C0_CH0, @ref XMC_I2C0_CH1,@ref XMC_I2C1_CH0,@ref XMC_I2C1_CH1,@ref XMC_I2C2_CH0,@ref XMC_I2C2_CH1 @note Availability of I2C1 and I2C2 depends on device selection
 * @return None
 *
 * \par<b>Description</b><br>
 * Enable data transmission.\n\n
 * Use this function in combination with XMC_I2C_CH_DisableDataTransmission() to fill the FIFO and send the FIFO content without gaps in the transmission.
 * FIFO is filled using XMC_USIC_CH_TXFIFO_PutData().
 * @note If you need more control over the start of transmission use XMC_USIC_CH_SetStartTransmisionMode()
 *
 * \par<b>Related APIs:</b><BR>
 * XMC_I2C_CH_DisableDataTransmission()\n\n\n
 */
__STATIC_INLINE void XMC_I2C_CH_EnableDataTransmission(XMC_USIC_CH_t *const channel)
{
  XMC_USIC_CH_SetStartTransmisionMode(channel, XMC_USIC_CH_START_TRANSMISION_ON_TDV);
}

/**
 * @param channel Constant pointer to USIC channel handle of type @ref XMC_USIC_CH_t \n
 *          \b Range: @ref XMC_I2C0_CH0, @ref XMC_I2C0_CH1,@ref XMC_I2C1_CH0,@ref XMC_I2C1_CH1,@ref XMC_I2C2_CH0,@ref XMC_I2C2_CH1 @note Availability of I2C1 and I2C2 depends on device selection
 * @return None
 *
 * \par<b>Description</b><br>
 * Disable data transmission.\n\n
 * Use this function in combination with XMC_I2C_CH_EnableDataTransmission() to fill the FIFO and send the FIFO content without gaps in the transmission.
 * FIFO is filled using XMC_USIC_CH_TXFIFO_PutData().
 *
 * \par<b>Related APIs:</b><BR>
 * XMC_I2C_CH_EnableDataTransmission()\n\n\n
 */
__STATIC_INLINE void XMC_I2C_CH_DisableDataTransmission(XMC_USIC_CH_t *const channel)
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
