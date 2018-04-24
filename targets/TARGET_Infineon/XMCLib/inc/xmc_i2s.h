/**
 * @file xmc_i2s.h
 * @date 2016-06-30
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
 * 2015-08-21:
 *     - Initial <br>
 *
 * 2015-08-24:
 *     - Added APIs for enabling/disabling delay compensation XMC_I2S_CH_DisableDelayCompensation() and
 *       XMC_I2S_CH_EnableDelayCompensation() <br>
 *
 * 2015-09-01:
 *     - Modified XMC_I2S_CH_SetInputSource() for avoiding complete DXCR register overwriting. <br>
 *     - Modified XMC_I2S_CH_EVENT_t enum for supporting XMC_I2S_CH_EnableEvent() and XMC_I2S_CH_DisableEvent() <br>
 *       for supporting multiple events configuration <br>
 *
 * 2015-09-14:
 *     - Modified XMC_I2S_CH_SetSystemWordLength for supporting up to 63 system word length<br>
 *
 * 2016-05-20:
 *     - Added XMC_I2S_CH_EnableDataTransmission() and XMC_I2S_CH_DisableDataTransmission()
 *
 * 2016-06-30:
 *     - Documentation updates.
 *
 * @endcond
 *
 */

#ifndef XMC_I2S_H_
#define XMC_I2S_H_

/**********************************************************************************************************************
 * HEADER FILES
 *********************************************************************************************************************/
#include "xmc_usic.h"

/**
 * @addtogroup XMClib XMC Peripheral Library
 * @{
 */

/**
 * @addtogroup I2S
 * @brief (IIS) driver for the XMC microcontroller family.
 *
 * USIC IIS Features: <br>
 * @{
 */

/*******************************************************************************
 * MACROS
 *******************************************************************************/

#if defined(USIC0)
#define XMC_I2S0_CH0 XMC_USIC0_CH0                   /**< USIC0 channel 0 base address */
#define XMC_I2S0_CH1 XMC_USIC0_CH1                   /**< USIC0 channel 1 base address */
#endif

#if defined(USIC1)
#define XMC_I2S1_CH0 XMC_USIC1_CH0                   /**< USIC1 channel 0 base address */
#define XMC_I2S1_CH1 XMC_USIC1_CH1                   /**< USIC1 channel 1 base address */
#endif

#if defined(USIC2)
#define XMC_I2S2_CH0 XMC_USIC2_CH0                   /**< USIC2 channel 0 base address */
#define XMC_I2S2_CH1 XMC_USIC2_CH1                   /**< USIC2 channel 1 base address */
#endif
/*******************************************************************************
 * ENUMS
 *******************************************************************************/

/**
 * @brief I2S Status
 */
typedef enum XMC_I2S_CH_STATUS
{
  XMC_I2S_CH_STATUS_OK,      /**< Status OK */
  XMC_I2S_CH_STATUS_ERROR,   /**< Status ERROR */
  XMC_I2S_CH_STATUS_BUSY     /**< Status BUSY */
} XMC_I2S_CH_STATUS_t;

/**
 * @brief I2S status flag
 */
typedef enum XMC_I2S_CH_STATUS_FLAG
{
  XMC_I2S_CH_STATUS_FLAG_WORD_ADDRESS = USIC_CH_PSR_IISMode_WA_Msk,                     /**< Word Address status */
  XMC_I2S_CH_STATUS_FLAG_DX2S = USIC_CH_PSR_IISMode_DX2S_Msk,                           /**< Status of WA input(DX2) signal*/
  XMC_I2S_CH_STATUS_FLAG_DX2T_EVENT_DETECTED = USIC_CH_PSR_IISMode_DX2TEV_Msk,          /**< Status for WA input signal transition */
  XMC_I2S_CH_STATUS_FLAG_WA_FALLING_EDGE_EVENT = USIC_CH_PSR_IISMode_WAFE_Msk,          /**< Falling edge of the WA output
                                                                                             signal has been generated */
  XMC_I2S_CH_STATUS_FLAG_WA_RISING_EDGE_EVENT = USIC_CH_PSR_IISMode_WARE_Msk,           /**< Rising edge of the WA output
                                                                                             signal has been generated */
  XMC_I2S_CH_STATUS_FLAG_WA_GENERATION_END = USIC_CH_PSR_IISMode_END_Msk,               /**< The WA generation has ended */
  XMC_I2S_CH_STATUS_FLAG_RECEIVER_START_INDICATION = USIC_CH_PSR_IISMode_RSIF_Msk,      /**< Receive start indication status */
  XMC_I2S_CH_STATUS_FLAG_DATA_LOST_INDICATION = USIC_CH_PSR_IISMode_DLIF_Msk,           /**< Data lost indication status */
  XMC_I2S_CH_STATUS_FLAG_TRANSMIT_SHIFT_INDICATION = USIC_CH_PSR_IISMode_TSIF_Msk,      /**< Transmit shift indication status */
  XMC_I2S_CH_STATUS_FLAG_TRANSMIT_BUFFER_INDICATION = USIC_CH_PSR_IISMode_TBIF_Msk,     /**< Transmit buffer indication status */
  XMC_I2S_CH_STATUS_FLAG_RECEIVE_INDICATION = USIC_CH_PSR_IISMode_RIF_Msk,              /**< Receive indication status */
  XMC_I2S_CH_STATUS_FLAG_ALTERNATIVE_RECEIVE_INDICATION = USIC_CH_PSR_IISMode_AIF_Msk,  /**< Alternate receive indication status */
  XMC_I2S_CH_STATUS_FLAG_BAUD_RATE_GENERATOR_INDICATION = USIC_CH_PSR_IISMode_BRGIF_Msk /**< Baud rate generator indication status */
} XMC_I2S_CH_STATUS_FLAG_t;

/**
 *  @brief I2S Baudrate Generator shift clock output
*/
typedef enum XMC_I2S_CH_BRG_SHIFT_CLOCK_OUTPUT
{
  XMC_I2S_CH_BRG_SHIFT_CLOCK_OUTPUT_SCLK = XMC_USIC_CH_BRG_SHIFT_CLOCK_OUTPUT_SCLK, /**< Baudrate Generator shift clock output: SCLK*/
  XMC_I2S_CH_BRG_SHIFT_CLOCK_OUTPUT_DX1  = XMC_USIC_CH_BRG_SHIFT_CLOCK_OUTPUT_DX1   /**< Clock obtained as input from master: DX1*/
} XMC_I2S_CH_BRG_SHIFT_CLOCK_OUTPUT_t;

/**
 *  @brief I2S channel interrupt node pointers
 */
typedef enum XMC_I2S_CH_INTERRUPT_NODE_POINTER
{
  XMC_I2S_CH_INTERRUPT_NODE_POINTER_TRANSMIT_SHIFT    = XMC_USIC_CH_INTERRUPT_NODE_POINTER_TRANSMIT_SHIFT,     /**< Node pointer for transmit shift interrupt */
  XMC_I2S_CH_INTERRUPT_NODE_POINTER_TRANSMIT_BUFFER   = XMC_USIC_CH_INTERRUPT_NODE_POINTER_TRANSMIT_BUFFER,    /**< Node pointer for transmit buffer interrupt */
  XMC_I2S_CH_INTERRUPT_NODE_POINTER_RECEIVE           = XMC_USIC_CH_INTERRUPT_NODE_POINTER_RECEIVE,            /**< Node pointer for receive interrupt */
  XMC_I2S_CH_INTERRUPT_NODE_POINTER_ALTERNATE_RECEIVE = XMC_USIC_CH_INTERRUPT_NODE_POINTER_ALTERNATE_RECEIVE,  /**< Node pointer for alternate receive interrupt */
  XMC_I2S_CH_INTERRUPT_NODE_POINTER_PROTOCOL          = XMC_USIC_CH_INTERRUPT_NODE_POINTER_PROTOCOL            /**< Node pointer for protocol related interrupts */
} XMC_I2S_CH_INTERRUPT_NODE_POINTER_t;

/**
 * @brief I2S events
 */
typedef enum XMC_I2S_CH_EVENT
{
  XMC_I2S_CH_EVENT_RECEIVE_START       = USIC_CH_CCR_RSIEN_Msk,  /**< Receive start event */
  XMC_I2S_CH_EVENT_DATA_LOST           = USIC_CH_CCR_DLIEN_Msk,  /**< Data lost event */
  XMC_I2S_CH_EVENT_TRANSMIT_SHIFT      = USIC_CH_CCR_TSIEN_Msk,  /**< Transmit shift event */
  XMC_I2S_CH_EVENT_TRANSMIT_BUFFER     = USIC_CH_CCR_TBIEN_Msk,  /**< Transmit buffer event */
  XMC_I2S_CH_EVENT_STANDARD_RECEIVE    = USIC_CH_CCR_RIEN_Msk,   /**< Receive event */
  XMC_I2S_CH_EVENT_ALTERNATIVE_RECEIVE = USIC_CH_CCR_AIEN_Msk,   /**< Alternate receive event */
  XMC_I2S_CH_EVENT_BAUD_RATE_GENERATOR = USIC_CH_CCR_BRGIEN_Msk, /**< Baudrate generator event */

  XMC_I2S_CH_EVENT_WA_FALLING_EDGE     = USIC_CH_PCR_IISMode_WAFEIEN_Msk << 2U,  /**< WA falling edge event */
  XMC_I2S_CH_EVENT_WA_RISING_EDGE      = USIC_CH_PCR_IISMode_WAREIEN_Msk << 2U,  /**< WA rising edge event */
  XMC_I2S_CH_EVENT_WA_GENERATION_END   = USIC_CH_PCR_IISMode_ENDIEN_Msk << 2U,   /**< END event */
  XMC_I2S_CH_EVENT_DX2TIEN_ACTIVATED   = USIC_CH_PCR_IISMode_DX2TIEN_Msk << 2U   /**< WA input signal transition event*/
} XMC_I2S_CH_EVENT_t;

/**
 * @brief Defines the Polarity of the WA in the SELO output lines in relation to the internal WA signal.
 */
typedef enum XMC_I2S_CH_WA_POLARITY
{
  XMC_I2S_CH_WA_POLARITY_DIRECT = 0x0UL,                                    /**< The SELO outputs have the same polarity
                                                                                  as the WA signal (active high) */
  XMC_I2S_CH_WA_POLARITY_INVERTED = 0x1UL << USIC_CH_PCR_IISMode_SELINV_Pos /**< The SELO outputs have the inverted
                                                                                 polarity to the WA signal (active low)*/
} XMC_I2S_CH_WA_POLARITY_t;

/**
 * @brief Defines the Polarity of the WA in the SELO output lines in relation to the internal WA signal.
 */
typedef enum XMC_I2S_CH_CHANNEL
{
  XMC_I2S_CH_CHANNEL_1_LEFT = 0U,  /**< Channel 1 (left) */
  XMC_I2S_CH_CHANNEL_2_RIGHT = 1U  /**< Channel 2 (right) */
} XMC_I2S_CH_CHANNEL_t;

/**
 * @brief I2S input stage selection
 */
typedef enum XMC_I2S_CH_INPUT
{
  XMC_I2S_CH_INPUT_DIN0 = 0UL,         /**< Data input stage 0 */
  XMC_I2S_CH_INPUT_SLAVE_SCLKIN = 1UL, /**< Clock input stage */
  XMC_I2S_CH_INPUT_SLAVE_WA = 2UL,     /**< WA input stage */
#if UC_FAMILY == XMC1
  XMC_I2S_CH_INPUT_DIN1 = 3UL,         /**< Data input stage 1 */
  XMC_I2S_CH_INPUT_DIN2 = 4UL,         /**< Data input stage 2 */
  XMC_I2S_CH_INPUT_DIN3 = 5UL          /**< Data input stage 3 */
#endif
} XMC_I2S_CH_INPUT_t;

/**
 * @brief Defines the I2S bus mode
 */
typedef enum XMC_I2S_CH_BUS_MODE
{
  XMC_I2S_CH_BUS_MODE_MASTER, /**< I2S Master */
  XMC_I2S_CH_BUS_MODE_SLAVE   /**< I2S Slave */
} XMC_I2S_CH_BUS_MODE_t;

/*******************************************************************************
 * DATA STRUCTURES
 *******************************************************************************/
/**
 * @brief I2S_CH configuration structure
 */
typedef struct XMC_I2S_CH_CONFIG
{
  uint32_t baudrate;					             /**< Module baud rate for communication */
  uint8_t data_bits;                       /**< Data word length. A data frame can consists of several data words. \n
                                                Value configured as USIC channel word length. \n
                                                  \b Range: minimum= 1, maximum= 16*/
  uint8_t frame_length;                    /**< Number of data bits transferred after a change of signal WA (data frame). \n
                                                Configured as USIC channel frame length. \n
                                                  \b Range: minimum= 1, maximum= 63*/
  uint8_t data_delayed_sclk_periods;       /**< Data delay defined in sclk periods */
  XMC_I2S_CH_WA_POLARITY_t wa_inversion;   /**< Enable inversion of Slave select signal relative to the internal WA */
  XMC_I2S_CH_BUS_MODE_t	bus_mode;          /**< Bus mode MASTER/SLAVE */
} XMC_I2S_CH_CONFIG_t;

/*******************************************************************************
 * API PROTOTYPES
 *******************************************************************************/

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @param channel Constant pointer to USIC channel handle of type @ref XMC_USIC_CH_t \n
 *          \b Range: @ref XMC_I2S0_CH0, XMC_I2S0_CH1,XMC_I2S1_CH0, XMC_I2S1_CH1,XMC_I2S2_CH0, XMC_I2S2_CH1 @note Availability of I2S1 and I2S2 depends on device selection
 * @param config Constant pointer to I2S configuration structure of type @ref XMC_I2S_CH_CONFIG_t.
 * @return XMC_I2S_CH_STATUS_t Status of initializing the USIC channel for I2S protocol.\n
 *          \b Range: @ref XMC_I2S_CH_STATUS_OK if initialization is successful.\n
 *                    @ref XMC_I2S_CH_STATUS_ERROR if configuration of baudrate failed.
 *
 * \par<b>Description</b><br>
 * Initializes the USIC channel for I2S protocol.\n\n
 * During the initialization, USIC channel is enabled and baudrate is configured.
 * After each change of the WA signal, a complete data frame is intended to be transferred (frame length <= system word length).
 * The number of data bits transferred after a change of signal WA is defined by config->frame_length.
 * A data frame can consist of several data words with a data word length defined by config->data_bits.
 * The changes of signal WA define the system word length as the number of SCLK cycles between two changes of WA.
 * The system word length is set by default to the frame length defined by config->frame_length.
 *
 * XMC_I2S_CH_Start() should be invoked after the initialization to enable the channel.
 *
 * \par<b>Related APIs:</b><BR>
 * XMC_I2S_CH_Start(), XMC_I2S_CH_Stop(), XMC_I2S_CH_Transmit(), XMC_I2S_CH_SetSystemWordLength()\n\n\n
 */
void XMC_I2S_CH_Init(XMC_USIC_CH_t *const channel, const XMC_I2S_CH_CONFIG_t *const config);

/**
 * @param channel A constant pointer to XMC_USIC_CH_t, pointing to the USIC channel base address.
 *
 * @return None
 *
 * \par<b>Description:</b><br>
 * Set the selected USIC channel to operate in I2S mode, by setting CCR.MODE bits.\n\n
 * It should be executed after XMC_I2S_CH_Init() during initialization. By invoking XMC_I2S_CH_Stop(), the MODE is set
 * to IDLE state. Call XMC_I2S_CH_Start() to set the I2S mode again, as needed later in the program.
 *
 * \par<b>Related APIs:</b><BR>
 * XMC_I2S_CH_Init(), XMC_I2S_CH_Stop()
 */
__STATIC_INLINE void XMC_I2S_CH_Start(XMC_USIC_CH_t *const channel)
{
  /* USIC channel in I2S mode */
  XMC_USIC_CH_SetMode(channel, XMC_USIC_CH_OPERATING_MODE_I2S);
}

/**
 * @param channel A constant pointer to XMC_USIC_CH_t, pointing to the USIC channel base address.
 *
 * @return XMC_I2S_CH_STATUS_t Status of the I2S driver after the request for stopping is processed. \n
 *        XMC_I2S_CH_STATUS_OK- If the USIC channel is successfully put to IDLE mode. \n
 *        XMC_I2S_CH_STATUS_BUSY- If the USIC channel is busy transmitting data.
 *
 * \par<b>Description:</b><br>
 * Set the selected I2S channel to IDLE mode, by clearing CCR.MODE bits.\n\n
 * After calling XMC_I2S_CH_Stop, channel is IDLE mode. So no communication is supported. XMC_I2S_CH_Start() has to be
 * invoked to start the communication again.
 *
 * \par<b>Related APIs:</b><BR>
 * XMC_I2S_CH_Start()
 */
XMC_I2S_CH_STATUS_t XMC_I2S_CH_Stop(XMC_USIC_CH_t *const channel);

/**
 * @param channel A constant pointer to XMC_USIC_CH_t, pointing to the USIC channel base address.
 * @param rate Bus speed in bits per second
 *
 * @return XMC_I2S_CH_STATUS_t Status of the I2S driver after the request for setting baudrate is processed. \n
 *        XMC_I2S_CH_STATUS_OK- If the baudrate is successfully changed. \n
 *        XMC_I2S_CH_STATUS_ERROR- If the new baudrate value is out of range.
 *
 * \par<b>Description:</b><br>
 * Sets the bus speed in bits per second
 *
 * \par<b>Related APIs:</b><BR>
 * XMC_I2S_CH_Init(), XMC_I2S_CH_Stop()
 */
XMC_I2S_CH_STATUS_t XMC_I2S_CH_SetBaudrate(XMC_USIC_CH_t *const channel, const uint32_t rate);

/**
 * @param channel A constant pointer to XMC_USIC_CH_t, pointing to the USIC channel base address.
 * @param sclk_cycles_system_word_length system word length in terms of sclk clock cycles.
 *
 * @return None
 *
 * \par<b>Description:</b><br>
 * Configures the system word length by setting BRG.DCTQ bit field.\n\n
 * This value has to be always higher than 1U and lower than the data with (SCTR.FLE)
 *
 */
void XMC_I2S_CH_SetSystemWordLength(XMC_USIC_CH_t *const channel,uint32_t sclk_cycles_system_word_length);

/**
 * @param channel A constant pointer to XMC_USIC_CH_t, pointing to the USIC channel base address.
 * @param data Data to be transmitted
 * @param channel_number Communication output channel of the I2S, based on this channel selection TCI(Transmit control information)is updated.\n
 *             Refer @ref XMC_I2S_CH_CHANNEL_t for valid values.
 *
 * @return None
 *
 * \par<b>Description:</b><br>
 * Puts the data into FIFO, if FIFO mode is enabled or else into standard buffer, by setting the proper mode.\n\n
 * TCI(Transmit Control Information) allows dynamic control of output channel during data transfers. To support this auto
 * update, TCSR.WAMD(Automatic WA mode) will be enabled during the initialization using XMC_I2S_CH_Init() for all modes.
 *
 *
 * \par<b>Related APIs:</b><BR>
 * XMC_I2S_CH_Receive()
 */
void XMC_I2S_CH_Transmit(XMC_USIC_CH_t *const channel, const uint16_t data, const XMC_I2S_CH_CHANNEL_t channel_number);

/**
 * @param channel A constant pointer to XMC_USIC_CH_t, pointing to the USIC channel base address.
 * @param channel_number Communication output channel of the I2S, based on this mode TCI(Transmit control information)is updated.\n
 *             Refer @ref XMC_I2S_CH_CHANNEL_t for valid values.
 *
 * @return None
 *
 * \par<b>Description:</b><br>
 * Transmits a dummy data(FFFFH) to provide clock for slave and receives the data from the slave.\n\n
 * XMC_I2S_CH_Receive() receives the data and places it into buffer based on the FIFO selection. After reception of data
 * XMC_I2S_CH_GetReceivedData() can be invoked to read the data from the buffers.
 *
 * \par<b>Related APIs:</b><BR>
 * XMC_I2S_CH_GetReceivedData()
 */
__STATIC_INLINE void XMC_I2S_CH_Receive(XMC_USIC_CH_t *const channel, const XMC_I2S_CH_CHANNEL_t channel_number)
{
  /* Transmit dummy data */
  XMC_I2S_CH_Transmit(channel, (uint16_t)0xffffU , channel_number);
}

/**
 * @param channel A constant pointer to XMC_USIC_CH_t, pointing to the USIC channel base address.
 *
 * @return uint16_t Data read from the receive buffer.
 *
 * \par<b>Description:</b><br>
 * Reads data from the receive buffer based on the FIFO selection.\n\n
 * Invocation of XMC_I2S_CH_Receive() receives the data and place it into receive buffer. After receiving the data
 * XMC_I2S_CH_GetReceivedData() can be used to read the data from the buffer.
 *
 * \par<b>Related APIs:</b><BR>
 * XMC_I2S_CH_Receive()
 */
uint16_t XMC_I2S_CH_GetReceivedData(XMC_USIC_CH_t *const channel);

/**
 * @param channel A constant pointer to XMC_USIC_CH_t, pointing to the USIC channel base address.
 *
 * @return None
 *
 * \par<b>Description:</b><br>
 * Set the order of data transfer from LSB to MSB, by clearing SCTR.SDIR bit.\n\n
 * This is typically based on the slave settings. Invoke XMC_I2S_CH_SetBitOrderLsbFirst() to set direction as needed in
 * the program.
 *
 * \par<b>Related APIs:</b><BR>
 * XMC_I2S_CH_SetBitOrderMsbFirst()
 */
__STATIC_INLINE void XMC_I2S_CH_SetBitOrderLsbFirst(XMC_USIC_CH_t *const channel)
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
 * This is typically based on the slave settings. This is not set during XMC_I2S_CH_Init().
 * Invoke XMC_I2S_CH_SetBitOrderMsbFirst() to set direction as needed in the program.
 *
 * \par<b>Related APIs:</b><BR>
 * XMC_I2S_CH_SetBitOrderLsbFirst()
 */
__STATIC_INLINE void XMC_I2S_CH_SetBitOrderMsbFirst(XMC_USIC_CH_t *const channel)
{
  channel->SCTR |= (uint32_t)USIC_CH_SCTR_SDIR_Msk;
}

/**
 * @param channel A constant pointer to XMC_USIC_CH_t, pointing to the USIC channel base address.
 * @param event Protocol events which have to be enabled.
 *              Refer @ XMC_I2S_CH_EVENT_t for valid values. <b>OR</b> combinations of these enum items can be used
 *              as input.
 *
 * @return None
 *
 * \par<b>Description:</b><br>
 * Enables the I2S protocol specific events, by configuring PCR register.\n\n
 * Events can be enabled as needed using XMC_I2S_CH_EnableEvent().
 * XMC_I2S_CH_DisableEvent() can be used to disable the events.
 *
 * \par<b>Related APIs:</b><BR>
 * XMC_I2S_CH_DisableEvent()
 */
void XMC_I2S_CH_EnableEvent(XMC_USIC_CH_t *const channel, const uint32_t event);

/**
 * @param channel A constant pointer to XMC_USIC_CH_t, pointing to the USIC channel base address.
 * @param event Protocol events which have to be disabled.
 *              Refer @ XMC_I2S_CH_EVENT_t for valid values. <b>OR</b> combinations of these enum item can be used
 *              as input.
 *
 * @return None
 *
 * \par<b>Description:</b><br>
 * Disables the I2S protocol specific events, by configuring PCR register.\n\n
 * After disabling the events, XMC_I2S_CH_EnableEvent() has to be invoked to re-enable the events.
 *
 * \par<b>Related APIs:</b><BR>
 * XMC_I2S_CH_EnableEvent()
 */
void XMC_I2S_CH_DisableEvent(XMC_USIC_CH_t *const channel, const uint32_t event);

/**
 * @param channel A constant pointer to XMC_USIC_CH_t, pointing to the USIC channel base address.
 *
 * @return uint32_t Status of I2S protocol events.
 *
 * \par<b>Description:</b><br>
 * Returns the status of the events, by reading PSR register.\n\n
 * This indicates the status of the all the events, for I2S communication.
 *
 * \par<b>Related APIs:</b><BR>
 * XMC_I2S_CH_ClearStatusFlag()
 */
__STATIC_INLINE uint32_t XMC_I2S_CH_GetStatusFlag(XMC_USIC_CH_t *const channel)
{
  return channel->PSR_IISMode;
}

/**
 * @param channel A constant pointer to XMC_USIC_CH_t, pointing to the USIC channel base address.
 * @param flag Protocol event status to be cleared for detection of next occurence.
 *              Refer @ XMC_I2S_CH_STATUS_FLAG_t for valid values. <b>OR</b> combinations of these enum item can be used
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
 * XMC_I2S_CH_GetStatusFlag()
 */
__STATIC_INLINE void XMC_I2S_CH_ClearStatusFlag(XMC_USIC_CH_t *const channel, const uint32_t flag)
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
 * XMC_I2S_CH_Init(). Invoke XMC_I2S_CH_EnableMasterClock() to enable as needed in the program, or if it is disabled by
 * XMC_I2S_CH_DisableMasterClock().
 *
 * \par<b>Related APIs:</b><BR>
 * XMC_I2S_CH_DisableMasterClock()
 */
__STATIC_INLINE void XMC_I2S_CH_EnableMasterClock(XMC_USIC_CH_t *const channel)
{
  channel->PCR_IISMode |= (uint32_t)USIC_CH_PCR_IISMode_MCLK_Msk;
}

/**
 * @param channel A constant pointer to XMC_USIC_CH_t, pointing to the USIC channel base address.
 *
 * @return None
 *
 * \par<b>Description:</b><br>
 * Disables the generation of Master clock by clearing PCR.MCLK bit.\n\n
 * This clock can be enabled by invoking XMC_I2S_CH_EnableMasterClock() as needed in the program.
 *
 * \par<b>Related APIs:</b><BR>
 * XMC_I2S_CH_EnableMasterClock()
 */
__STATIC_INLINE void XMC_I2S_CH_DisableMasterClock(XMC_USIC_CH_t *const channel)
{
  channel->PCR_IISMode &= (uint32_t)~USIC_CH_PCR_IISMode_MCLK_Msk;
}

/**
 * @param channel A constant pointer to XMC_USIC_CH_t, pointing to the USIC channel base address.
 * @param clock_output shift clock source.\n
 *                     Refer @ref XMC_I2S_CH_BRG_SHIFT_CLOCK_OUTPUT_t for valid inputs.
 *
 * @return None
 *
 * \par<b>Description:</b><br>
 * Configures the shift clock source by setting BRG.SCLKOSEL.\n\n
 * In Master mode operation, shift clock is generated by the internal baud rate generator. This SCLK is made available
 * for external slave devices by SCLKOUT signal.\n
 * In Slave mode, the signal is received from the external master. So the DX1(input) stage has to be connected to input.\n
 */
__STATIC_INLINE void XMC_I2S_CH_ConfigureShiftClockOutput(XMC_USIC_CH_t *const channel,
                                                          const XMC_I2S_CH_BRG_SHIFT_CLOCK_OUTPUT_t clock_output)
{
  XMC_USIC_CH_ConfigureShiftClockOutput(channel, (XMC_USIC_CH_BRG_SHIFT_CLOCK_PASSIVE_LEVEL_t)0U,
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
 * XMC_I2S_CH_SetFrameLength()
 */
__STATIC_INLINE void XMC_I2S_CH_SetWordLength(XMC_USIC_CH_t *const channel, const uint8_t word_length)
{
  XMC_USIC_CH_SetWordLength(channel, word_length);
}

/**
 * @param  channel A constant pointer to XMC_USIC_CH_t, pointing to the USIC channel base address.
 * @param  frame_length Number of bits in a frame. \n
 *                \b Range: 1 to 64.
 *
 * @return None
 *
 * \par<b>Description</b><br>
 * Define the data frame length.\n\n
 * Set the number of bits to be serially transmitted in a frame.
 * The frame length should be multiples of word length.
 *
 * \par<b>Related APIs:</b><BR>
 * XMC_USIC_CH_SetWordLength()
 */
__STATIC_INLINE void XMC_I2S_CH_SetFrameLength(XMC_USIC_CH_t *const channel, const uint8_t frame_length)
{
  XMC_USIC_CH_SetFrameLength(channel, frame_length);
}

/**
 * @param  channel A constant pointer to XMC_USIC_CH_t, pointing to the USIC channel base address.
 * @param  input I2S channel input stage.\n
 *               Refer @ref XMC_I2S_CH_INPUT_t for valid values
 * @param  source Input source select for the input stage.
 *                Range : [0 to 7]
 *
 * @return None
 *
 * \par<b>Description</b><br>
 * Selects the data source for I2S input stage, by configuring DXCR.DSEL bits.\n\n
 * Selects the input data signal source among DXnA, DXnB.. DXnG for the input stage. The API can be used for all the
 * input stages like DX0CR, DX1CR etc. This is not done during initialization. This has to be configured before starting
 * the I2S communication.
 */
__STATIC_INLINE void XMC_I2S_CH_SetInputSource(XMC_USIC_CH_t *const channel,
                                               const XMC_I2S_CH_INPUT_t input,
                                               const uint8_t source)
{
  channel->DXCR[input] = (uint32_t)(channel->DXCR[input] & (~USIC_CH_DX0CR_DSEN_Msk)) | USIC_CH_DX0CR_INSW_Msk;
  XMC_USIC_CH_SetInputSource(channel, (XMC_USIC_CH_INPUT_t)input, source);
}

/**
 * @param  channel A constant pointer to XMC_USIC_CH_t, pointing to the USIC channel base address.
 * @param  wa_inversion Polarity of the word address signal.\n
 *               Refer @ref XMC_I2S_CH_WA_POLARITY_t for valid values
 *
 * @return None
 *
 * \par<b>Description</b><br>
 * Set the polarity of the word address signal, by configuring PCR.SELINV bit.\n\n
 * Normally WA signal is active low level signal. This is configured
 * in XMC_I2S_CH_Init() during initialization. Invoke XMC_I2S_CH_WordAddressSignalPolarity() with desired settings as
 * needed later in the program.
 */
__STATIC_INLINE void XMC_I2S_CH_WordAddressSignalPolarity(XMC_USIC_CH_t *const channel,
                                                       const XMC_I2S_CH_WA_POLARITY_t wa_inversion)
{
  /* Configuration of Protocol Control Register */
  channel->PCR_IISMode = (uint32_t)((channel->PCR_IISMode & (~USIC_CH_PCR_IISMode_SELINV_Msk)) | (uint32_t)wa_inversion);
}

/**
 * @param  channel A constant pointer to XMC_USIC_CH_t, pointing to the USIC channel base address.
 * @param  input I2S channel input stage.\n
 *               Refer @ref XMC_I2S_CH_INPUT_t for valid inputs.
 *
 * @return None
 *
 * \par<b>Description</b><br>
 * Enables the polarity inversion of input data signal, by setting DXyCR.DPOL(where y = \a input).\n\n
 * This is not set in XMC_I2S_CH_Init(). Invoke XMC_I2S_CH_EnableInputInversion() as needed later in the program. To
 * disable the inversion XMC_I2S_CH_DisableInputInversion() can be invoked.
 *
 * \par<b>Related APIs:</b><BR>
 * XMC_I2S_CH_DisableInputInversion()
 */
__STATIC_INLINE void XMC_I2S_CH_EnableInputInversion(XMC_USIC_CH_t *const channel,
                                                     const XMC_I2S_CH_INPUT_t input)
{
  XMC_USIC_CH_EnableInputInversion(channel, (XMC_USIC_CH_INPUT_t)input);
}

/**
 * @param  channel A constant pointer to XMC_USIC_CH_t, pointing to the USIC channel base address.
 * @param  input I2S channel input stage.\n
 *               Refer @ref XMC_I2S_CH_INPUT_t for valid inputs.
 *
 * @return None
 *
 * \par<b>Description</b><br>
 * Disables the polarity inversion of input data signal, by clearing DXyCR.DPOL(where y = \a input).\n\n
 * Resets the input data polarity. Invoke XMC_I2S_CH_EnableInputInversion() to apply inversion.
 *
 * \par<b>Related APIs:</b><BR>
 * XMC_I2S_CH_EnableInputInversion()
 */
__STATIC_INLINE void XMC_I2S_CH_DisableInputInversion(XMC_USIC_CH_t *const channel,
                                                      const XMC_I2S_CH_INPUT_t input)
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
 * Sets the interrupt node for I2S channel events.\n\n
 * For an event to generate interrupt, node pointer should be configured with service request(SR0, SR1..SR5).
 * The NVIC node gets linked to the interrupt event by doing so. This is not configured in XMC_I2S_CH_Init() during
 * initialization.
 *
 * \par<b>Note::</b><BR>
 * 1. NVIC node should be separately enabled to generate the interrupt.
 *
 * \par<b>Related APIs:</b><BR>
 * XMC_USIC_CH_EnableEvent()
 */
__STATIC_INLINE void XMC_I2S_CH_SetInterruptNodePointer(XMC_USIC_CH_t *const channel,
                                                     const uint8_t service_request)
{
  XMC_USIC_CH_SetInterruptNodePointer(channel, XMC_USIC_CH_INTERRUPT_NODE_POINTER_PROTOCOL, (uint32_t)service_request);
}


/**
 * @param channel Pointer to USIC channel handler of type @ref XMC_USIC_CH_t \n
 * 				  \b Range: @ref XMC_I2S0_CH0, @ref XMC_I2S0_CH1,@ref XMC_I2S1_CH0,@ref XMC_I2S1_CH1,@ref XMC_I2S2_CH0,@ref XMC_I2S2_CH1 @note Availability of I2S1 and I2S2 depends on device selection
 * @param  interrupt_node Interrupt node pointer to be configured. \n
 * 						  \b Range: @ref XMC_I2S_CH_INTERRUPT_NODE_POINTER_TRANSMIT_SHIFT,
 * 						  			@ref XMC_I2S_CH_INTERRUPT_NODE_POINTER_TRANSMIT_BUFFER etc.
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
 * XMC_I2S_CH_EnableEvent() \n\n\n
 */
__STATIC_INLINE void XMC_I2S_CH_SelectInterruptNodePointer(XMC_USIC_CH_t *const channel,
                                                           const XMC_I2S_CH_INTERRUPT_NODE_POINTER_t interrupt_node,
                                                           const uint32_t service_request)
{
  XMC_USIC_CH_SetInterruptNodePointer(channel, (XMC_USIC_CH_INTERRUPT_NODE_POINTER_t)interrupt_node,
		                                       (uint32_t)service_request);
}

/**
 * @param  channel Pointer to USIC channel handler of type @ref XMC_USIC_CH_t \n
 * 				   \b Range: @ref XMC_I2S0_CH0, @ref XMC_I2S0_CH1,@ref XMC_I2S1_CH0,@ref XMC_I2S1_CH1,@ref XMC_I2S2_CH0,@ref XMC_I2S2_CH1 @note Availability of I2S1 and I2S2 depends on device selection
 * @param  service_request_line service request number of the event to be triggered. \n
 * 			\b Range: 0 to 5.
 * @return None
 *
 * \par<b>Description</b><br>
 * Trigger a I2S interrupt service request.\n\n
 * When the I2S service request is triggered, the NVIC interrupt associated with it will be
 * generated if enabled.
 *
 * \par<b>Related APIs:</b><BR>
 * XMC_I2S_CH_SelectInterruptNodePointer() \n\n\n
 */
__STATIC_INLINE void XMC_I2S_CH_TriggerServiceRequest(XMC_USIC_CH_t *const channel, const uint32_t service_request_line)
{
  XMC_USIC_CH_TriggerServiceRequest(channel, (uint32_t)service_request_line);
}

/**
 * @param  channel Pointer to USIC channel handler of type @ref XMC_USIC_CH_t \n
 * 				   \b Range: @ref XMC_I2S0_CH0, @ref XMC_I2S0_CH1,@ref XMC_I2S1_CH0,@ref XMC_I2S1_CH1,@ref XMC_I2S2_CH0,@ref XMC_I2S2_CH1 @note Availability of I2S1 and I2S2 depends on device selection
 * @return None
 *
 * \par<b>Description</b><br>
 * Enables delay compensation. \n\n
 *
 * Delay compensation can be applied to the receive path.
 * \par<b>Related APIs:</b><BR>
 * XMC_I2S_CH_DisableDelayCompensation()\n\n\n
 */
__STATIC_INLINE void XMC_I2S_CH_EnableDelayCompensation(XMC_USIC_CH_t *const channel)
{
  XMC_USIC_CH_EnableDelayCompensation(channel);
}

/**
 * @param  channel Pointer to USIC channel handler of type @ref XMC_USIC_CH_t \n
 * 				   \b Range: @ref XMC_I2S0_CH0, @ref XMC_I2S0_CH1,@ref XMC_I2S1_CH0,@ref XMC_I2S1_CH1,@ref XMC_I2S2_CH0,@ref XMC_I2S2_CH1 @note Availability of I2S1 and I2S2 depends on device selection
 * @return None
 *
 * \par<b>Description</b><br>
 * Disables delay compensation.. \n\n
 *
 * \par<b>Related APIs:</b><BR>
 * XMC_I2S_CH_EnableDelayCompensation()\n\n\n
 */
__STATIC_INLINE void XMC_I2S_CH_DisableDelayCompensation(XMC_USIC_CH_t *const channel)
{
  XMC_USIC_CH_DisableDelayCompensation(channel);
}

/**
 * @param channel Constant pointer to USIC channel handle of type @ref XMC_USIC_CH_t \n
 *          \b Range: @ref XMC_I2S0_CH0, @ref XMC_I2S0_CH1,@ref XMC_I2S1_CH0,@ref XMC_I2S1_CH1,@ref XMC_I2S2_CH0,@ref XMC_I2S2_CH1 @note Availability of I2S1 and I2S2 depends on device selection
 * @return None
 *
 * \par<b>Description</b><br>
 * Enable data transmission.\n\n
 * Use this function in combination with XMC_I2S_CH_DisableDataTransmission() to fill the FIFO and send the FIFO content without gaps in the transmission.
 * FIFO is filled using XMC_USIC_CH_TXFIFO_PutData().
 * @note If you need more control over the start of transmission use XMC_USIC_CH_SetStartTransmisionMode()
 *
 * \par<b>Related APIs:</b><BR>
 * XMC_I2S_CH_DisableDataTransmission()\n\n\n
 */
__STATIC_INLINE void XMC_I2S_CH_EnableDataTransmission(XMC_USIC_CH_t *const channel)
{
  XMC_USIC_CH_SetStartTransmisionMode(channel, XMC_USIC_CH_START_TRANSMISION_ON_TDV);
}

/**
 * @param channel Constant pointer to USIC channel handle of type @ref XMC_USIC_CH_t \n
 *          \b Range: @ref XMC_I2S0_CH0, @ref XMC_I2S0_CH1,@ref XMC_I2S1_CH0,@ref XMC_I2S1_CH1,@ref XMC_I2S2_CH0,@ref XMC_I2S2_CH1 @note Availability of I2S1 and I2S2 depends on device selection
 * @return None
 *
 * \par<b>Description</b><br>
 * Disable data transmission.\n\n
 * Use this function in combination with XMC_I2S_CH_EnableDataTransmission() to fill the FIFO and send the FIFO content without gaps in the transmission.
 * FIFO is filled using XMC_USIC_CH_TXFIFO_PutData().
 *
 * \par<b>Related APIs:</b><BR>
 * XMC_I2S_CH_EnableDataTransmission()\n\n\n
 */
__STATIC_INLINE void XMC_I2S_CH_DisableDataTransmission(XMC_USIC_CH_t *const channel)
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
#endif /* XMC_I2S_H_ */
