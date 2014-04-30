/*
 * Copyright (c) 2013 - 2014, Freescale Semiconductor, Inc.
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:
 *
 * o Redistributions of source code must retain the above copyright notice, this list
 *   of conditions and the following disclaimer.
 *
 * o Redistributions in binary form must reproduce the above copyright notice, this
 *   list of conditions and the following disclaimer in the documentation and/or
 *   other materials provided with the distribution.
 *
 * o Neither the name of Freescale Semiconductor, Inc. nor the names of its
 *   contributors may be used to endorse or promote products derived from this
 *   software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR
 * ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
 * ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */
#ifndef __FSL_UART_HAL_H__
#define __FSL_UART_HAL_H__

#include <assert.h>
#include <stdint.h>
#include <stdbool.h>
#include "fsl_uart_features.h"
#include "fsl_device_registers.h"

/*!
 * @addtogroup uart_hal
 * @{
 */

/*! @file*/

/*******************************************************************************
 * Definitions
 ******************************************************************************/

/*! @brief Error codes for the UART driver. */
typedef enum _uart_status
{
    kStatus_UART_Success = 0,
    kStatus_UART_BaudRateCalculationError,/*!< UART Baud Rate calculation error out of range */
    kStatus_UART_BaudRatePercentDiffExceeded,   /*!< UART Baud Rate exceeds percentage difference */
    kStatus_UART_BitCountNotSupported,  /*!< UART bit count config not supported */
    kStatus_UART_StopBitCountNotSupported,  /*!< UART stop bit count config not supported */
    kStatus_UART_RxStandbyModeError,  /*!< UART unable to place receiver in standby mode */
    kStatus_UART_ClearStatusFlagError,  /*!< UART clear status flag error */
    kStatus_UART_MSBFirstNotSupported,  /*!< UART MSB first feature not supported */
    kStatus_UART_ResyncNotSupported,  /*!< UART resync disable operation not supported */
    kStatus_UART_TxNotDisabled,  /*!< UART Transmitter not disabled before enabling feature */
    kStatus_UART_RxNotDisabled,  /*!< UART Receiver not disabled before enabling feature */
    kStatus_UART_TxOrRxNotDisabled,   /*!< UART Transmitter or Receiver not disabled */
    kStatus_UART_TxBusy,  /*!< UART transmit still in progress */
    kStatus_UART_RxBusy,  /*!< UART receive still in progress */
    kStatus_UART_NoTransmitInProgress, /*!< UART no transmit in progress */
    kStatus_UART_NoReceiveInProgress, /*!< UART no receive in progress */
    kStatus_UART_InvalidInstanceNumber, /*!< Invalid UART instance number */
    kStatus_UART_InvalidBitSetting,  /*!< Invalid setting for desired UART register bit field */
    kStatus_UART_OverSamplingNotSupported,  /*!< UART oversampling not supported */
    kStatus_UART_BothEdgeNotSupported,  /*!< UART both edge sampling not supported */
    kStatus_UART_Timeout,  /*!< UART transfer timed out*/
} uart_status_t;

/*!
 * @brief UART number of stop bits.
 *
 * These constants define the number of allowable stop bits to configure in a UART instance.
 */
typedef enum _uart_stop_bit_count {
    kUartOneStopBit = 0,  /*!< one stop bit */
    kUartTwoStopBit = 1,  /*!< two stop bits */
} uart_stop_bit_count_t;

/*!
 * @brief UART parity mode.
 *
 * These constants define the UART parity mode options: disabled or enabled of type even or odd.
 */
typedef enum _uart_parity_mode {
    kUartParityDisabled = 0x0,  /*!< parity disabled */
    kUartParityEven     = 0x2,  /*!< parity enabled, type even, bit setting: PE|PT = 10 */
    kUartParityOdd      = 0x3,  /*!< parity enabled, type odd,  bit setting: PE|PT = 11 */
} uart_parity_mode_t;

/*!
 * @brief UART number of bits in a character.
 *
 * These constants define the number of allowable data bits per UART character. Note, check the
 * UART documentation to determine if the desired UART instance supports the desired number
 * of data bits per UART character.
 */
typedef enum  _uart_bit_count_per_char {
    kUart8BitsPerChar = 0,   /*!< 8-bit data characters */
    kUart9BitsPerChar = 1,   /*!< 9-bit data characters */
    kUart10BitsPerChar = 2,  /*!< 10-bit data characters */
} uart_bit_count_per_char_t;

/*!
 * @brief UART operation configuration constants.
 *
 * This provides constants for UART operational states: "operates normally"
 * or "stops/ceases operation"
 */
typedef enum _uart_operation_config {
    kUartOperates = 0,  /*!< UART continues to operate normally */
    kUartStops = 1,   /*!< UART ceases operation */
} uart_operation_config_t;

/*!
 * @brief UART wakeup from standby method constants.
 *
 * This provides constants for the two UART wakeup methods: idle-line or address-mark.
 */
typedef enum _uart_wakeup_method {
    kUartIdleLineWake = 0,  /*!< The idle-line wakes UART receiver from standby */
    kUartAddrMarkWake = 1,   /*!< The address-mark wakes UART receiver from standby */
} uart_wakeup_method_t;

/*!
 * @brief UART idle-line detect selection types.
 *
 * This provides constants for the UART idle character bit-count start: either after start or
 * stop bit.
 */
typedef enum _uart_idle_line_select {
    kUartIdleLineAfterStartBit = 0,  /*!< UART idle character bit count start after start bit */
    kUartIdleLineAfterStopBit = 1,   /*!< UART idle character bit count start after stop bit */
} uart_idle_line_select_t;

/*!
 * @brief UART break character length settings for transmit/detect.
 *
 * This provides constants for the UART break character length for both transmission and detection
 * purposes. Note that the actual maximum bit times may vary depending on the UART instance.
 */
typedef enum _uart_break_char_length {
    kUartBreakChar10BitMinimum = 0, /*!< UART break char length 10 bit times (if M = 0, SBNS = 0) or
                                     11 (if M = 1, SBNS = 0 or M = 0, SBNS = 1) or 12 (if M = 1,
                                     SBNS = 1 or M10 = 1, SNBS = 0) or 13 (if M10 = 1, SNBS = 1) */
    kUartBreakChar13BitMinimum = 1, /*!< UART break char length 13 bit times (if M = 0, SBNS = 0) or
                                     14 (if M = 1, SBNS = 0 or M = 0, SBNS = 1) or 15 (if M = 1,
                                     SBNS = 1 or M10 = 1, SNBS = 0) or 16 (if M10 = 1, SNBS = 1) */
} uart_break_char_length_t;

/*!
 * @brief UART single-wire mode transmit direction.
 *
 *  This provides constants for the UART transmit direction when configured for single-wire mode.
 *  The transmit line TXDIR is either an input or output.
 */
typedef enum _uart_singlewire_txdir {
    kUartSinglewireTxdirIn = 0,  /*!< UART Single-Wire mode TXDIR input */
    kUartSinglewireTxdirOut = 1, /*!< UART Single-Wire mode TXDIR output */
} uart_singlewire_txdir_t;

/*!
 * @brief UART status flags.
 *
 * This provides constants for the UART status flags for use in the UART functions.
 */
typedef enum _uart_status_flag {
    kUartTransmitDataRegisterEmpty, /*!< Transmit data register empty flag, sets when transmit
                                         buffer is empty */
    kUartTransmissionComplete,    /*!< Transmission complete flag, sets when transmitter is idle
                                      (transmission activity complete) */
    kUartReceiveDataRegisterFull, /*!< Receive data register full flag, sets when the receive data
                                       buffer is full */
    kUartIdleLineDetect,          /*!< Idle line detect flag, sets when idle line detected */
    kUartReceiveOverrun,          /*!< Receiver Overrun, sets when new data is received before data
                                       is read from receive register */
    kUartNoiseDetect,             /*!< Receiver takes 3 samples of each received bit. If any of
                                       these samples differ, noise flag sets */
    kUartFrameError,              /*!< Frame error flag, sets if logic 0 was detected where stop
                                       bit expected */
    kUartParityError,             /*!< If parity enabled, sets upon parity error detection */
    kUartLineBreakDetect,         /*!< LIN break detect interrupt flag, sets when
                                       LIN break char detected and LIN circuit enabled */
    kUartReceiveActiveEdgeDetect, /*!< Receive pin active edge interrupt flag, sets when active
                                       edge detected */
    kUartReceiverActive,        /*!< Receiver Active Flag (RAF), sets at beginning of valid start
                                     bit */
#if FSL_FEATURE_UART_HAS_EXTENDED_DATA_REGISTER_FLAGS
    kUartNoiseInCurrentWord,     /*!< NOISY bit, sets if noise detected in current data word */
    kUartParityErrorInCurrentWord, /*!< PARITYE bit, sets if noise detected in current data word */
#endif
#if FSL_FEATURE_UART_HAS_FIFO
    kUartTxBufferEmpty,          /*!< TXEMPT bit, sets if transmit buffer is empty */
    kUartRxBufferEmpty,          /*!< RXEMPT bit, sets if transmit buffer is empty */
    kUartTxBufferOverflow,       /*!< TXOF bit, sets if transmit buffer overflow occurred */
    kUartRxBufferUnderflow,       /*!< RXUF bit, sets if receive buffer underflow occurred */
#endif
} uart_status_flag_t;

/*!
 * @brief UART infrared transmitter pulse width options.
 *
 * This provides constants for the UART infrared (IR) pulse widths. Options include 3/16, 1/16
 * 1/32, and 1/4 pulse widths.
 */
typedef enum _uart_ir_tx_pulsewidth {
    kUartIrThreeSixteenthsWidth = 0,   /*!< 3/16 pulse */
    kUartIrOneSixteenthWidth = 1,      /*!< 1/16 pulse */
    kUartIrOneThirtysecondsWidth = 2,  /*!< 1/32 pulse */
    kUartIrOneFourthWidth = 3,         /*!< 1/4 pulse */
} uart_ir_tx_pulsewidth_t;

/*!
 * @brief Structure for idle line configuration settings.
 *
 * This structure contains settings for the UART idle line configuration such as the Idle
 * Line Type (ILT) and the Receiver Wake Up Idle Detect (RWUID).
 */
typedef struct UartIdleLineConfig {
    unsigned idleLineType : 1; /*!< ILT, Idle bit count start: 0 - after start bit (default),
                                    1 - after stop bit */
    unsigned rxWakeIdleDetect : 1; /*!< RWUID, Receiver Wake Up Idle Detect. IDLE status bit
                                        operation during receive standby. Controls whether idle
                                        character that wakes up receiver will also set
                                        IDLE status bit 0 - IDLE status bit doesn't
                                        get set (default), 1 - IDLE status bit gets set */
} uart_idle_line_config_t;

/*! @brief Structure for all UART status flags.
 *
 * This structure contains the settings for all of the UART status flags.
 */
typedef struct UartStatusAll {
    unsigned transmitDataRegisterEmpty : 1; /*!< Transmit data register empty flag, sets when
                                                 transmit buffer is empty */
    unsigned transmissionComplete : 1;    /*!< Transmission complete flag, sets when transmitter
                                               is idle (transmission activity complete)*/
    unsigned receiveDataRegisterFull : 1; /*!< Receive data register full flag, sets when the
                                               receive data buffer is full */
    unsigned idleLineDetect : 1;         /*!< Idle line detect flag, sets when idle line detected */
    unsigned receiveOverrun : 1;          /*!< Receiver Overrun, sets when new data is received
                                               before data is read from receive register */
    unsigned noiseDetect : 1;             /*!< Receiver takes 3 samples of each received bit.
                                               If any of these samples differ, noise flag sets */
    unsigned frameError : 1;              /*!< Frame error flag, sets if  logic 0 was detected
                                               where stop bit expected */
    unsigned parityError : 1;             /*!< If parity enabled, will set upon parity
                                               error detection */
    unsigned lineBreakDetect : 1;         /*!< LIN break detect interrupt flag, sets when
                                               LIN break char detected and LIN circuit enabled */
    unsigned receiveActiveEdgeDetect : 1; /*!< Receive pin active edge interrupt flag, sets
                                               when active edge detected */
    unsigned receiverActive : 1;          /*!< Receiver Active Flag (RAF), sets at beginning of
                                               valid start bit */
#if FSL_FEATURE_UART_HAS_EXTENDED_DATA_REGISTER_FLAGS
    unsigned noiseInCurrentWord  : 1;  /*!< NOISY bit, sets if noise detected in current data word */
    unsigned parityErrorInCurrentWord  : 1; /*!< PARITYE bit, sets if noise detected in
                                                 current data word */
#endif
#if FSL_FEATURE_UART_HAS_FIFO
    unsigned txBufferEmpty  : 1;         /*!< TXEMPT bit, sets if transmit buffer is empty */
    unsigned rxBufferEmpty  : 1;         /*!< RXEMPT bit, sets if transmit buffer is empty */
    unsigned txBufferOverflow  : 1;     /*!< TXOF bit, sets if transmit buffer overflow occurred */
    unsigned rxBufferUnderflow  : 1;    /*!< RXUF bit, sets if receive buffer underflow occurred */
#endif
} uart_status_flag_all_t;

/*!
 * @brief UART interrupt configuration structure, default settings are 0 (disabled).
 *
 * This structure contains the settings for all of the UART interrupt configurations.
 */
typedef struct UartInterruptConfig {
    unsigned linBreakDetect : 1;    /*!< LIN break detect: 0 - disable interrupt,
                                         1 - enable interrupt */
    unsigned rxActiveEdge : 1;      /*!< RX Active Edge: 0 - disable interrupt,
                                         1 - enable interrupt */
    unsigned transmitDataRegisterEmpty : 1; /*!< Transmit data register empty:
                                                 0 - disable interrupt, 1 - enable interrupt */
    unsigned transmitComplete : 1;  /*!< Transmission complete: 0 - disable interrupt,
                                         1 - enable interrupt */
    unsigned receiverDataRegisterFull : 1; /*!< Receiver data register full:
                                                0 - disable interrupt, 1 - enable interrupt */
    unsigned idleLine : 1;          /*!< Idle line: 0 - disable interrupt, 1 - enable interrupt*/
    unsigned receiverOverrun : 1;   /*!< Receiver Overrun: 0 - disable interrupt,
                                         1 - enable interrupt */
    unsigned noiseErrorFlag : 1;    /*!< Noise error flag: 0 - disable interrupt,
                                         1 - enable interrupt */
    unsigned frameErrorFlag : 1;    /*!< Framing error flag: 0 - disable interrupt,
                                         1 - enable interrupt */
    unsigned parityErrorFlag : 1;   /*!< Parity error flag: 0 - disable interrupt,
                                         1 - enable interrupt */
#if FSL_FEATURE_UART_HAS_FIFO
    unsigned txFifoOverflow : 1;    /*!< TX FIFO Overflow: 0 - disable interrupt,
                                         1 - enable interrupt */
    unsigned rxFifoUnderflow : 1;   /*!< RX FIFO Underflow: 0 - disable interrupt,
                                         1 - enable interrupt */
#endif
} uart_interrupt_config_t;

/*!
 * @brief UART configuration structure.
 *
 * This structure contains the settings for the most common UART configurations including
 * the UART module source clock, baud rate, parity mode, stop bit count, data bit count per
 * character, and tx/rx inversion options (which is the least common of the configurations).
 */
typedef struct UartConfig {
    uint32_t uartSourceClockInHz; /*!< UART module source clock in Hz */
    uint32_t baudRate;            /*!< UART baud rate */
    uart_parity_mode_t parityMode; /*!< Parity mode, disabled (default), even, or odd */
    uart_stop_bit_count_t stopBitCount; /*!< Number of stop bits, 1 stop bit (default)
                                             or 2 stop bits */
    uart_bit_count_per_char_t bitCountPerChar; /*!< Number of bits, 8-bit (default) or 9-bit
                                                    in a word (up to 10-bits in
                                                    some UART instances) */
    unsigned rxDataInvert : 1; /*!< Receive Data Inversion: 0 - not inverted (default),
                                    1 - inverted */
    unsigned txDataInvert : 1; /*!< Transmit Data Inversion: 0 - not inverted (default),
                                    1 - inverted */
} uart_config_t;

/*******************************************************************************
 * API
 ******************************************************************************/

#if defined(__cplusplus)
extern "C" {
#endif

/*!
 * @name UART Common Configurations
 * @{
 */

/*!
 * @brief Initialize the UART controller.
 *
 * This function initializes the module to user defined settings and default settings.
 * Here is an example demonstrating how to define the uart_config_t structure and call
 * the uart_hal_init function:
   @code
    uart_config_t uartConfig;
    uartConfig.uartSourceClockInHz = uartSourceClock;
    uartConfig.baudRate = baudRate;
    uartConfig.bitCountPerChar = kUart8BitsPerChar;
    uartConfig.parityMode = kUartParityDisabled;
    uartConfig.stopBitCount = kUartOneStopBit;
    uartConfig.txDataInvert = 0;
    uartConfig.rxDataInvert = 0;
    uart_hal_init(uartInstance, &uartConfig);
   @endcode
 *
 * @param   uartInstance UART module instance number.
 * @param   config UART configuration data.
 * @return  An error code or kStatus_UART_Success.
 */
uart_status_t uart_hal_init(uint32_t uartInstance, const uart_config_t *config);

/*!
 * @brief Configure the UART baud rate.
 *
 * This function programs the UART baud rate to the desired value passed in by the user. The user
 * must also pass in the module source clock so that the function can calculate the baud
 * rate divisors to their appropriate values.
 * In some UART instances it is required that the transmitter/receiver be disabled
 * before calling this function.
 * Generally this is applied to all UARTs to ensure safe operation.
 *
 * @param   uartInstance UART module instance number.
 * @param   sourceClockInHz UART source input clock in Hz.
 * @param   desiredBaudRate UART desired baud rate.
 * @return  An error code or kStatus_UART_Success
 */
uart_status_t uart_hal_set_baud_rate(uint32_t uartInstance, uint32_t sourceClockInHz,
                                uint32_t desiredBaudRate);

/*!
 * @brief Set the UART baud rate modulo divisor value.
 *
 * This function allows the user to program the baud rate divisor directly in situations
 * where the divisor value is known. In this case, the user may not want to call the
 * uart_hal_set_baud_rate() function, as the divisor is already known.
 *
 * @param   uartInstance UART module instance number.
 * @param   baudRateDivisor The baud rate modulo division "SBR" value.
 * @return  An error code or kStatus_UART_Success.
 */
uart_status_t uart_hal_set_baud_rate_divisor(uint32_t uartInstance, uint32_t baudRateDivisor);

#if FSL_FEATURE_UART_HAS_BAUD_RATE_FINE_ADJUST_SUPPORT
/*!
 * @brief Set the UART baud rate fine adjust. (Note: Feature available on select
 *        UART instances used in conjunction with baud rate programming)
 *
 * This function, which programs the baud rate fine adjust, is used in conjunction with
 * programming the baud rate modulo divisor in situations where these divisors value are known.
 * In this case, the user may not want to call the uart_hal_set_baud_rate() function, as the
 * divisors are already known.
 *
 *
 * @param   uartInstance UART module instance number.
 * @param   baudFineAdjust Value of 5-bit field used to add more timing resolution to average
 *                          baud rate frequency is 1/32 increments.
 * @return  An error code or kStatus_UART_Success.
 */
uart_status_t uart_hal_set_baud_rate_fine_adjust(uint32_t uartInstance, uint8_t baudFineAdjust);
#endif

#if FSL_FEATURE_UART_HAS_BAUD_RATE_OVER_SAMPLING_SUPPORT
/*!
 * @brief Set the UART baud rate oversampling ratio. (Note: Feature available on select
 *        UART instances used in conjunction with baud rate programming)
 *
 * This function allows the user to directly configure the oversampling ratio (OSR). Normally this
 * ratio is automatically configured when calling the uart_hal_set_baud_rate(). However, if the user
 * knows the necessary dividers and wishes to directly program them, they also have the option to
 * directly program the OSR.
 * The oversampling ratio should be set between 4x (00011) and 32x (11111), writing
 * an invalid oversampling ratio results in an error and is set to a default
 * 16x (01111) oversampling ratio.
 * It is required that the transmitter/receiver be disabled before calling
 * this function.
 *
 * @param   uartInstance UART module instance number.
 * @param   overSamplingRatio The oversampling ratio "OSR" value.
 * @return  An error code or kStatus_UART_Success.
 */
uart_status_t uart_hal_set_oversampling_ratio(uint32_t uartInstance, uint32_t overSamplingRatio);
#endif

#if FSL_FEATURE_UART_HAS_BOTH_EDGE_SAMPLING_SUPPORT
/*!
 * @brief Configure the UART baud rate both edge sampling option. (Note: Feature available on select
 *        UART instances used in conjunction with baud rate programming)
 *
 * This function allows the user to directly configure the both edge sampling option.
 * When enabled, the received data is sampled on both edges of the baud rate clock.
 * This must be set when the oversampling ratio is between 4x and 7x.
 * This function should only be called when the receiver is disabled.
 *
 * @param   uartInstance UART module instance number.
 * @param   enableBothEdgeSampling Enable (true) or disable (false) Both Edge Sampling option.
 * @return  An error code or kStatus_UART_Success.
 */
uart_status_t uart_hal_configure_both_edge_sampling(uint32_t uartInstance,
                                                    bool enableBothEdgeSampling);
#endif

/*!
 * @brief Configure number of bits per character in the UART controller.
 *
 * This function allows the user to configure the number of bits per character according to the
 * typedef uart_bit_count_per_char_t.
 * In some UART instances it is required that the transmitter/receiver be disabled
 * before calling this function.
 * This may be applied to all UARTs to ensure safe operation.
 *
 * @param   uartInstance UART module instance number.
 * @param   bitCountPerChar Number of bits per char (8, 9, or 10, depending on the UART instance).
 * @return  An error code or kStatus_UART_Success.
 */
uart_status_t uart_hal_configure_bit_count_per_char(uint32_t uartInstance,
                                               uart_bit_count_per_char_t bitCountPerChar);

/*!
 * @brief Configure the parity mode in the UART controller.
 *
 * This function allows the user to configure the parity mode of the UART controller to disable
 * it or enable it for even parity or for odd parity.
 * In some UART instances it is required that the transmitter/receiver be disabled
 * before calling this function.
 * This may be applied to all UARTs to ensure safe operation.
 *
 * @param   uartInstance UART module instance number.
 * @param   parityModeType Parity mode setting (enabled, disable, odd, even - see
 *                         parity_mode_t struct).
 */
void uart_hal_configure_parity_mode(uint32_t uartInstance, uart_parity_mode_t parityModeType);

/*!
 * @brief Configure the number of stop bits in the UART controller.
 *
 * This function allows the user to configure the number of stop bits in the UART controller
 * to be one or two stop bits.
 * In some UART instances it is required that the transmitter/receiver be disabled
 * before calling this function.
 * This may be applied to all UARTs to ensure safe operation.
 *
 * @param   uartInstance UART module instance number.
 * @param   stopBitCount Number of stop bits setting (1 or 2 - see uart_stop_bit_count_t struct).
 * @return  An error code (an unsupported setting in some UARTs) or kStatus_UART_Success.
 */
uart_status_t uart_hal_configure_stop_bit_count(uint32_t uartInstance,
                                           uart_stop_bit_count_t stopBitCount);

/*!
 * @brief Configure the transmit and receive inversion control in UART controller.
 *
 * This function allows the user to invert the transmit and receive signals, independently.
 * This function should only be called when the UART is between transmit and receive packets.
 *
 * @param   uartInstance UART module instance number.
 * @param   rxInvert Enable (1) or disable (0) receive inversion.
 * @param   txInvert Enable (1) or disable (0) transmit inversion.
 */
void uart_hal_configure_tx_rx_inversion(uint32_t uartInstance, uint32_t rxInvert,
                                        uint32_t txInvert);

/*!
 * @brief Enable the UART transmitter.
 *
 * This function allows the user to enable the UART transmitter.
 *
 * @param   uartInstance UART module instance number.
 */
void uart_hal_enable_transmitter(uint32_t uartInstance);

/*!
 * @brief Disable the UART transmitter.
 *
 * This function allows the user to disable the UART transmitter.
 *
 * @param   uartInstance UART module instance number.
 */
void uart_hal_disable_transmitter(uint32_t uartInstance);

/*!
 * @brief Get the UART transmitter enabled/disabled configuration setting.
 *
 * This function allows the user to get the setting of the UART transmitter.
 *
 * @param   uartInstance UART module instance number.
 * @return The state of UART transmitter enable(true)/disable(false) setting.
 */
bool uart_hal_is_transmitter_enabled(uint32_t uartInstance);

/*!
 * @brief Enable the UART receiver.
 *
 *  This function allows the user to enable the UART receiver.
 *
 * @param   uartInstance UART module instance number.
 */
void uart_hal_enable_receiver(uint32_t uartInstance);

/*!
 * @brief Disable the UART receiver.
 *
 *  This function allows the user to disable the UART receiver.
 *
 * @param   uartInstance UART module instance number.
 */
void uart_hal_disable_receiver(uint32_t uartInstance);

/*!
 * @brief Get the UART receiver enabled/disabled configuration setting.
 *
 *  This function allows the user to get the setting of the UART receiver.
 *
 * @param   uartInstance UART module instance number.
 * @return The state of UART receiver enable(true)/disable(false) setting.
 */
bool uart_hal_is_receiver_enabled(uint32_t uartInstance);

/*@}*/

/*!
 * @name UART Interrupts and DMA
 * @{
 */

/*!
 * @brief Configure the UART module interrupts to enable/disable various interrupt sources.
 *
 * This function allows the user to configure all of the UART interrupts with one function call.
 * The user will first need to initialize and pass in a structure of type uart_interrupt_config_t
 * which sets the configuration of each interrupt.
 *
 * @param   uartInstance UART module instance number.
 * @param   interruptConfig UART interrupt configuration data.
 */
void uart_hal_configure_interrupts(uint32_t uartInstance,
                                   const uart_interrupt_config_t * interruptConfig);

/*!
 * @brief Enable the break_detect_interrupt.
 *
 * @param   uartInstance UART module instance number.
 */
void uart_hal_enable_break_detect_interrupt(uint32_t uartInstance);

/*!
 * @brief Disable the break_detect_interrupt.
 *
 * @param   uartInstance UART module instance number.
 */
void uart_hal_disable_break_detect_interrupt(uint32_t uartInstance);

/*!
 * @brief Get the configuration of the break_detect_interrupt enable setting.
 *
 * @param   uartInstance UART module instance number.
 * @return The bit setting of the interrupt enable bit.
 */
bool uart_hal_is_break_detect_interrupt_enabled(uint32_t uartInstance);

/*!
 * @brief Enable the rx_active_edge_interrupt.
 *
 * @param   uartInstance UART module instance number.
 */
void uart_hal_enable_rx_active_edge_interrupt(uint32_t uartInstance);

/*!
 * @brief Disable the rx_active_edge_interrupt.
 *
 * @param   uartInstance UART module instance number.
 */
void uart_hal_disable_rx_active_edge_interrupt(uint32_t uartInstance);

/*!
 * @brief Get the configuration of the rx_active_edge_interrupt enable setting.
 *
 * @param   uartInstance  UART instance number.
 * @return Bit setting of the interrupt enable bit.
 */
bool uart_hal_is_rx_active_edge_interrupt_enabled(uint32_t uartInstance);

/*!
 * @brief Enable the tx_data_register_empty_interrupt.
 *
 * @param   uartInstance UART module instance number.
 */
void uart_hal_enable_tx_data_register_empty_interrupt(uint32_t uartInstance);

/*!
 * @brief Disable the tx_data_register_empty_interrupt.
 *
 * @param   uartInstance UART module instance number.
 */
void uart_hal_disable_tx_data_register_empty_interrupt(uint32_t uartInstance);

/*!
 * @brief Get the configuration of the tx_data_register_empty_interrupt enable setting.
 *
 * @param   uartInstance UART module instance number.
 * @return Bit setting of the interrupt enable bit.
 */
bool uart_hal_is_tx_data_register_empty_interrupt_enabled(uint32_t uartInstance);

/*!
 * @brief Enable the transmission_complete_interrupt.
 *
 * @param   uartInstance UART module instance number.
 */
void uart_hal_enable_transmission_complete_interrupt(uint32_t uartInstance);

/*!
 * @brief Disable the transmission_complete_interrupt.
 *
 * @param   uartInstance UART module instance number.
 */
void uart_hal_disable_transmission_complete_interrupt(uint32_t uartInstance);

/*!
 * @brief Get the configuration of the transmission_complete_interrupt enable setting.
 *
 * @param   uartInstance UART module instance number.
 * @return Bit setting of the interrupt enable bit.
 */
bool uart_hal_is_transmission_complete_interrupt_enabled(uint32_t uartInstance);

/*!
 * @brief Enable the rx_data_register_full_interrupt.
 *
 * @param   uartInstance UART module instance number.
 */
void uart_hal_enable_rx_data_register_full_interrupt(uint32_t uartInstance);

/*!
 * @brief Disable the rx_data_register_full_interrupt.
 *
 * @param   uartInstance UART module instance number.
 */
void uart_hal_disable_rx_data_register_full_interrupt(uint32_t uartInstance);

/*!
 * @brief Get the configuration of the rx_data_register_full_interrupt enable setting.
 *
 * @param   uartInstance UART module instance number.
 * @return Bit setting of the interrupt enable bit.
 */
bool uart_hal_is_receive_data_full_interrupt_enabled(uint32_t uartInstance);

/*!
 * @brief Enable the idle_line_interrupt.
 *
 * @param   uartInstance UART module instance number.
 */
void uart_hal_enable_idle_line_interrupt(uint32_t uartInstance);

/*!
 * @brief Disable the idle_line_interrupt.
 *
 * @param   uartInstance UART module instance number.
 */
void uart_hal_disable_idle_line_interrupt(uint32_t uartInstance);

/*!
 * @brief Get the configuration of the idle_line_interrupt enable setting.
 *
 * @param   uartInstance UART module instance number.
 * @return Bit setting of the interrupt enable bit.
 */
bool uart_hal_is_idle_line_interrupt_enabled(uint32_t uartInstance);

/*!
 * @brief Enable the rx_overrun_interrupt.
 *
 * @param   uartInstance UART module instance number.
 */
void uart_hal_enable_rx_overrun_interrupt(uint32_t uartInstance);

/*!
 * @brief Disable the rx_overrun_interrupt.
 *
 * @param   uartInstance UART module instance number.
 */
void uart_hal_disable_rx_overrun_interrupt(uint32_t uartInstance);

/*!
 * @brief Get the configuration of the rx_overrun_interrupt enable setting.
 *
 * @param   uartInstance UART module instance number.
 * @return Bit setting of the interrupt enable bit .
 */
bool uart_hal_is_rx_overrun_interrupt_enabled(uint32_t uartInstance);

/*!
 * @brief Enable the noise_error_interrupt.
 *
 * @param   uartInstance UART module instance number.
 */
void uart_hal_enable_noise_error_interrupt(uint32_t uartInstance);

/*!
 * @brief Disable the noise_error_interrupt.
 *
 * @param   uartInstance UART module instance number.
 */
void uart_hal_disable_noise_error_interrupt(uint32_t uartInstance);

/*!
 * @brief Get the configuration of the noise_error_interrupt enable setting.
 *
 * @param   uartInstance UART module instance number.
 * @return Bit setting of the interrupt enable bit.
 */
bool uart_hal_is_noise_error_interrupt_enabled(uint32_t uartInstance);

/*!
 * @brief Enable the framing_error_interrupt.
 *
 * @param   uartInstance UART module instance number.
 */
void uart_hal_enable_framing_error_interrupt(uint32_t uartInstance);

/*!
 * @brief Disable the framing_error_interrupt.
 *
 * @param   uartInstance UART module instance number.
 */
void uart_hal_disable_framing_error_interrupt(uint32_t uartInstance);

/*!
 * @brief Get the configuration of the framing_error_interrupt enable setting.
 *
 * @param   uartInstance UART module instance number.
 * @return Bit setting of the interrupt enable bit.
 */
bool uart_hal_is_framing_error_interrupt_enabled(uint32_t uartInstance);

/*!
 * @brief Enable the parity_error_interrupt.
 *
 * @param   uartInstance UART module instance number.
 */
void uart_hal_enable_parity_error_interrupt(uint32_t uartInstance);

/*!
 * @brief Disable the parity_error_interrupt.
 *
 * @param   uartInstance UART module instance number.
 */
void uart_hal_disable_parity_error_interrupt(uint32_t uartInstance);

/*!
 * @brief Get the configuration of the parity_error_interrupt enable setting.
 *
 * @param   uartInstance UART module instance number.
 * @return Bit setting of the interrupt enable bit.
 */
bool uart_hal_is_parity_error_interrupt_enabled(uint32_t uartInstance);

/*!
 * @brief  Configure the UART DMA requests for the Transmitter and Receiver.
 *
 * This function allows the user to configure the transmit data register empty flag to
 * generate an interrupt request (default) or a DMA request.  Similarly, this function
 * allows the user to configure the receive data register full flag to generate an interrupt
 * request (default) or a DMA request.
 *
 * @param   uartInstance UART module instance number.
 * @param   txDmaConfig Transmit DMA request configuration setting (enable: true /disable: false).
 * @param   rxDmaConfig Receive DMA request configuration setting (enable: true/disable: false).
 */
void uart_hal_configure_dma(uint32_t uartInstance, bool txDmaConfig, bool rxDmaConfig);

/*!
 * @brief  Get the UART Transmit DMA request configuration setting.
 *
 * This function returns the configuration setting of the Transmit DMA request.
 *
 * @param   uartInstance UART module instance number.
 * @return   Transmit DMA request configuration setting (enable: true /disable: false).
 */
bool uart_hal_is_txdma_enabled(uint32_t uartInstance);

/*!
 * @brief  Get the UART Receive DMA request configuration setting.
 *
 * This function returns the configuration setting of the Receive DMA request.
 *
 * @param   uartInstance UART module instance number.
 * @return   Receive DMA request configuration setting (enable: true /disable: false).
 */
bool uart_hal_is_rxdma_enabled(uint32_t uartInstance);

/*@}*/

/*!
 * @name UART Transfer Functions
 * @{
 */

/*!
 * @brief This function allows the user to send an 8-bit character from the UART data register.
 *
 * @param   uartInstance UART module instance number.
 * @param   data The data to send of size 8-bit.
 */
void uart_hal_putchar(uint32_t uartInstance, uint8_t data);

/*!
 * @brief This function allows the user to send a 9-bit character from the UART data register.
 *
 * @param   uartInstance UART module instance number.
 * @param   data The data to send of size 9-bit.
 */
void uart_hal_putchar9(uint32_t uartInstance, uint16_t data);

/*!
 * @brief  This function allows the user to send a 10-bit character from the UART data register.
 *         (Note: Feature available on select UART instances)
 *
 * @param   uartInstance UART module instance number.
 * @param   data The data to send of size 10-bit.
 * @return  An error code or kStatus_UART_Success.
 */
uart_status_t uart_hal_putchar10(uint32_t uartInstance, uint16_t data);

/*!
 * @brief This function gets a received 8-bit character from the UART data register.
 *
 * @param   uartInstance UART module instance number.
 * @param   readData The received data read from data register of size 8-bit.
 */
void  uart_hal_getchar(uint32_t uartInstance, uint8_t *readData);

/*!
 * @brief This function gets a received 9-bit character from the UART data register.
 *
 * @param   uartInstance UART module instance number.
 * @param   readData The received data read from data register of size 9-bit.
 */
void  uart_hal_getchar9(uint32_t uartInstance, uint16_t *readData);

/*!
 * @brief This function gets a received 10-bit character from the UART data register.
 *        (Note: Feature available on select UART instances)
 *
 * @param   uartInstance UART module instance number.
 * @param   readData The received data read from data register of size 10-bit.
 * @return  An error code or kStatus_UART_Success.
 */
uart_status_t uart_hal_getchar10(uint32_t uartInstance, uint16_t *readData);

#if FSL_FEATURE_UART_HAS_EXTENDED_DATA_REGISTER_FLAGS
/*!
 * @brief  Configure the UART bit 10 (if enabled) or bit 9 (if disabled) as the parity bit in the
 *         serial transmission.
 *
 * This function configures bit 10 or bit 9 to be the parity bit.  To configure bit 10 as the parity
 * bit, the function sets UARTx_C4[M10]; it also sets UARTx_C1[M] and UARTx_C1[PE] as required.
 *
 * @param   uartInstance UART module instance number.
 * @param  enable The setting to enable (true), which configures bit 10 as the parity bit or to
 *                disable (false), which configures bit 9 as the parity bit in the serial
 *                transmission.
 */
void uart_hal_configure_bit10_as_paritybit_operation(uint32_t uartInstance, bool enable);

/*!
 * @brief  Get the configuration of the UART bit 10 (if enabled) or bit 9 (if disabled) as the
 *         parity bit in the serial transmission.
 *
 * This function returns true if bit 10 is configured as the parity bit, otherwise it returns
 * false if bit 9 is configured as the parity bit.
 *
 * @param   uartInstance UART module instance number.
 * @return  The configuration setting of bit 10 (true), or bit 9 (false) as the
 *          parity bit in the serial transmission.
 */
bool uart_hal_is_bit10_set_as_paritybit(uint32_t uartInstance);

/*!
 * @brief  Determine if the UART received data word was received with noise.
 *
 * This function returns true if the received data word was received with noise. Otherwise,
 * it returns false indicating no noise was detected.
 *
 * @param   uartInstance UART module instance number.
 * @return  The status of the NOISY bit in the UART extended data register.
 */
bool uart_hal_is_current_dataword_received_with_noise(uint32_t uartInstance);

/*!
 * @brief  Determine if the UART received data word was received with a parity error.
 *
 * This function returns true if the received data word was received with a parity error.
 * Otherwise, it returns false indicating no parity error was detected.
 *
 * @param   uartInstance UART module instance number.
 * @return  The status of the PARITYE (parity error) bit in the UART extended data register.
 */
bool uart_hal_is_current_dataword_received_with_parityerror(uint32_t uartInstance);
#endif  /* FSL_FEATURE_UART_HAS_EXTENDED_DATA_REGISTER_FLAGS*/

/*@}*/

/*!
 * @name UART Special Feature Configurations
 * @{
 */

/*!
 * @brief Configure the UART to either operate or cease to operate in WAIT mode.
 *
 * The function configures the UART to either operate or cease to operate when WAIT mode is
 * entered.
 * In some UART instances it is required that the transmitter/receiver be disabled
 * before calling this function.
 * This may be applied to all UARTs to ensure safe operation.
 *
 * @param   uartInstance UART module instance number.
 * @param   mode The UART WAIT mode operation - operates or ceases to operate in WAIT mode.
 */
void  uart_hal_configure_wait_mode_operation(uint32_t uartInstance, uart_operation_config_t mode);

/*!
 * @brief Determine if the UART operates or ceases to operate in WAIT mode.
 *
 * This function returns kUartOperates if the UART has been configured to operate in WAIT mode.
 * Else it returns KUartStops if the UART has been configured to cease-to-operate in WAIT mode.
 *
 * @param   uartInstance UART module instance number.
 * @return The UART WAIT mode operation configuration, returns either kUartOperates or KUartStops.
 */
uart_operation_config_t uart_hal_get_wait_mode_operation_config(uint32_t uartInstance);

/*!
 * @brief Configure the UART loopback operation.
 *
 * This function enables or disables the UART loopback operation.
 * In some UART instances it is required that the transmitter/receiver be disabled
 * before calling this function.
 * This may be applied to all UARTs to ensure safe operation.
 *
 * @param uartInstance UART module instance number.
 * @param enable The UART loopback mode configuration, either disabled (false) or enabled (true).
 */
void uart_hal_configure_loopback_mode(uint32_t uartInstance, bool enable);

/*!
 * @brief Configure the UART single-wire operation.
 *
 * This function enables or disables the UART single-wire operation.
 * In some UART instances it is required that the transmitter/receiver be disabled
 * before calling this function.
 * This may be applied to all UARTs to ensure safe operation.
 *
 * @param uartInstance UART module instance number.
 * @param enable The UART single-wire mode configuration, either disabled (false) or enabled (true).
 */
void uart_hal_configure_singlewire_mode(uint32_t uartInstance, bool enable);

/*!
 * @brief Configure the UART transmit direction while in single-wire mode.
 *
 * This function configures the transmitter direction when the UART is configured for single-wire
 * operation.
 *
 * @param   uartInstance UART module instance number.
 * @param   direction The UART single-wire mode transmit direction configuration of type
 *                    uart_singlewire_txdir_t (either kUartSinglewireTxdirIn or
 *                    kUartSinglewireTxdirOut.
 */
void uart_hal_configure_txdir_in_singlewire_mode(uint32_t uartInstance,
                                                 uart_singlewire_txdir_t direction);

/*!
 * @brief  Place the UART receiver in standby mode.
 *
 * This function, when called, places the UART receiver into standby mode.
 * In some UART instances, there are conditions that must be met before placing rx in standby mode.
 * Before placing UART in standby, determine if receiver is set to
 * wake on idle, and if receiver is already in idle state. 
 * NOTE: RWU should only be set with C1[WAKE] = 0 (wakeup on idle) if the channel is currently
 * not idle.
 * This can be determined by the S2[RAF] flag. If set to wake up FROM an IDLE event and the channel
 * is already idle, it is possible that the UART will discard data because data must be received
 * (or a LIN break detect) after an IDLE is detected before IDLE is allowed to be reasserted.
 *
 * @param uartInstance UART module instance number.
 * @return Error code or kStatus_UART_Success.
 */
uart_status_t uart_hal_put_receiver_in_standby_mode(uint32_t uartInstance);

/*!
 * @brief  Place the UART receiver in normal mode (disable standby mode operation).
 *
 * This function, when called, places the UART receiver into normal mode and out of
 * standby mode.
 *
 * @param   uartInstance UART module instance number.
 */
void uart_hal_put_receiver_in_normal_mode(uint32_t uartInstance);

/*!
 * @brief  Determine if the UART receiver is currently in standby mode.
 *
 * This function determines the state of the UART receiver. If it returns true, this means
 * that the UART receiver is in standby mode; if it returns false, the UART receiver
 * is in normal mode.
 *
 * @param   uartInstance UART module instance number.
 * @return The UART receiver is in normal mode (false) or standby mode (true).
 */
bool uart_hal_is_receiver_in_standby(uint32_t uartInstance);

/*!
 * @brief  Select the UART receiver wakeup method (idle-line or address-mark) from standby mode.
 *
 * This function configures the wakeup method of the UART receiver from standby mode.  The options
 * are idle-line wake or address-mark wake.
 *
 * @param   uartInstance UART module instance number.
 * @param   method The UART receiver wakeup method options: kUartIdleLineWake - Idle-line wake or
 *                 kUartAddrMarkWake - address-mark wake.
 */
void uart_hal_select_receiver_wakeup_method(uint32_t uartInstance, uart_wakeup_method_t method);

/*!
 * @brief  Get the UART receiver wakeup method (idle-line or address-mark) from standby mode.
 *
 * This function returns how the UART receiver is configured to wake from standby mode. The
 * wake method options that can be returned are kUartIdleLineWake or kUartAddrMarkWake.
 *
 * @param   uartInstance UART module instance number.
 * @return  The UART receiver wakeup from standby method, false: kUartIdleLineWake (idle-line wake)
 *          or true: kUartAddrMarkWake (address-mark wake).
 */
uart_wakeup_method_t uart_hal_get_receiver_wakeup_method(uint32_t uartInstance);

/*!
 * @brief  Configure the operation options of the UART idle line detect.
 *
 * This function allows the user to configure the UART idle-line detect operation. There are two
 * separate operations for the user to configure, the idle line bit-count start and the receive
 * wake up affect on IDLE status bit. The user will pass in a structure of type
 * uart_idle_line_config_t.
 * In some UART instances it is required that the transmitter/receiver be disabled
 * before calling this function.
 * This may be applied to all UARTs to ensure safe operation.
 *
 * @param   uartInstance UART module instance number.
 * @param   config  The UART configuration pointer to the structure for idle-line detect operation
 *                  of type uart_idle_line_config_t.
 */
void uart_hal_configure_idle_line_detect(uint32_t uartInstance,
                                         const uart_idle_line_config_t *config);

/*!
 * @brief  Configure the UART break character transmit length.
 *
 * This function allows the user to configure the UART break character transmit length. Refer to
 * the typedef uart_break_char_length_t for setting options.
 * In some UART instances it is required that the transmitter be disabled before calling
 * this function. This may be applied to all UARTs to ensure safe operation.
 *
 * @param uartInstance UART module instance number.
 * @param length The UART break character length setting of type uart_break_char_length_t, either a
 *               minimum 10-bit times or a minimum 13-bit times.
 */
void uart_hal_set_break_char_transmit_length(uint32_t uartInstance,
                                             uart_break_char_length_t length);

/*!
 * @brief  Configure the UART break character detect length.
 *
 * This function allows the user to configure the UART break character detect length. Refer to
 * the typedef uart_break_char_length_t for setting options.
 *
 * @param uartInstance UART module instance number.
 * @param length The UART break character length setting of type uart_break_char_length_t, either a
 *               minimum 10-bit times or a minimum 13-bit times.
 */
void uart_hal_set_break_char_detect_length(uint32_t uartInstance,
                                           uart_break_char_length_t length);

/*!
 * @brief  Configure the UART transmit send break character operation.
 *
 * This function allows the user to queue a UART break character to send.  If true is passed into
 * the function, then a break character is queued for transmission.  A break character will
 * continuously be queued until this function is called again when a false is passed into this
 * function.
 *
 * @param   uartInstance UART module instance number.
 * @param   enable If false, the UART normal/queue break character setting is disabled, which
 *                 configures the UART for normal transmitter operation. If true, a break
 *                 character is queued for transmission.
 */
void uart_hal_queue_break_char_to_send(uint32_t uartInstance, bool enable);

/*!
 * @brief  Configure the UART match address mode control operation. (Note: Feature available on
 *         select UART instances)
 *
 * The function allows the user to configure the UART match address control operation. The user
 * has the option to enable the match address mode and to program the match address value. There
 * are two match address modes, each with its own enable and programmable match address value.
 *
 * @param  uartInstance UART module instance number.
 * @param  matchAddrMode1 If true, this enables match address mode 1 (MAEN1), where false disables.
 * @param  matchAddrMode2 If true, this enables match address mode 2 (MAEN2), where false disables.
 * @param  matchAddrValue1 The match address value to program for match address mode 1.
 * @param  matchAddrValue2 The match address value to program for match address mode 2.
 * @return An error code or kStatus_UART_Success.
 */
uart_status_t uart_hal_configure_match_address_operation(
    uint32_t uartInstance,
    bool matchAddrMode1,
    bool matchAddrMode2,
    uint8_t matchAddrValue1,
    uint8_t matchAddrValue2);

/*!
 * @brief Configure the UART to send data MSB first
 * (Note: Feature available on select UART instances)
 *
 * The function allows the user to configure the UART to send data MSB first or LSB first.
 * In some UART instances it is required that the transmitter/receiver be disabled
 * before calling this function.
 * This may be applied to all UARTs to ensure safe operation.
 *
 * @param   uartInstance UART module instance number.
 * @param   enable This configures send MSB first mode configuration. If true, the data is sent MSB
 *                 first; if false, it is sent LSB first.
 * @return  An error code or kStatus_UART_Success.
 */
uart_status_t uart_hal_configure_send_msb_first_operation(uint32_t uartInstance, bool enable);

/*!
 * @brief  Configuration option to disable the UART resynchronization during received data.
 *         (Note: Feature available on select UART instances)
 *
 * This function allows the user to disable the UART resync of received data. The default setting
 * is false, meaning that resynchronization during the received data word is supported.
 * If the user passes in true, this disables resynchronization during the received data word.
 *
 * @param   uartInstance UART module instance number.
 * @param   enable If false, then resynchronization during the received data word is supported.
 *                 If true, resynchronization during the received data word is disabled.
 * @return  An error code or kStatus_UART_Success.
 */
uart_status_t uart_hal_configure_receive_resync_disable_operation(uint32_t uartInstance,
                                                                  bool enable);

#if FSL_FEATURE_UART_HAS_MODEM_SUPPORT
/*!
 * @brief  Enable the UART receiver request-to-send functionality.
 *
 * This function allows the user to enable the UART receiver request-to-send (RTS) functionality.
 * By enabling, it allows the RTS output to control the CTS input of the transmitting device to
 * prevent receiver overrun. RTS is deasserted if the number of characters in the receiver data
 * register (FIFO) is equal to or greater than RWFIFO[RXWATER]. RTS is asserted when the
 * number of characters in the receiver data register (FIFO) is less than RWFIFO[RXWATER].
 * Do not set both RXRTSE and TXRTSE.
 *
 * @param   uartInstance UART module instance number.
 */
void uart_hal_enable_receiver_rts(uint32_t uartInstance);

/*!
 * @brief  Disable the UART receiver request-to-send functionality.
 *
 * This function allows the user to disable the UART receiver request-to-send (RTS) functionality.
 *
 * @param   uartInstance UART module instance number.
 */
void uart_hal_disable_receiver_rts(uint32_t uartInstance);

/*!
 * @brief  Enable the UART transmitter request-to-send functionality.
 *
 * This function allows the user to enable the UART transmitter request-to-send (RTS) functionality.
 * When enabled, it allows the UART to control the RTS assertion before and after a transmission
 * such that when a character is placed into an empty transmitter data buffer, RTS
 * asserts one bit time before the start bit is transmitted. RTS deasserts one bit time after all
 * characters in the transmitter data buffer and shift register are completely sent, including
 * the last stop bit.
 *
 * @param   uartInstance UART module instance number.
 */
void uart_hal_enable_transmitter_rts(uint32_t uartInstance);

/*!
 * @brief  Disable the UART transmitter request-to-send functionality.
 *
 * This function allows the user to disable the UART transmitter request-to-send (RTS)
 * functionality.
 *
 * @param   uartInstance UART module instance number.
 */
void uart_hal_disable_transmitter_rts(uint32_t uartInstance);

/*!
 * @brief  Configure the UART transmitter RTS polarity.
 *
 * This function allows the user configure the transmitter RTS polarity to be either active low
 * or active high.
 *
 * @param uartInstance UART module instance number.
 * @param polarity The UART transmitter RTS polarity setting (false - active low,
 *                 true - active high).
 */
void uart_hal_configure_transmitter_rts_polarity(uint32_t uartInstance, bool polarity);

/*!
 * @brief  Enable the UART transmitter clear-to-send functionality.
 *
 * This function allows the user to enable the UART transmitter clear-to-send (CTS) functionality.
 * When enabled, the transmitter checks the state of CTS each time it is ready to send a character.
 * If CTS is asserted, the character is sent. If CTS is deasserted, the signal TXD remains in
 * the mark state and transmission is delayed until CTS is asserted. Changes in CTS as a
 * character is being sent do not affect its transmission.
 *
 * @param   uartInstance UART module instance number.
 */
void uart_hal_enable_transmitter_cts(uint32_t uartInstance);

/*!
 * @brief  Disable the UART transmitter clear-to-send functionality.
 *
 * This function allows the user to disable the UART transmitter clear-to-send (CTS)
 * functionality.
 *
 * @param   uartInstance UART module instance number.
 */
void uart_hal_disable_transmitter_cts(uint32_t uartInstance);
#endif  /* FSL_FEATURE_UART_HAS_MODEM_SUPPORT*/

#if FSL_FEATURE_UART_HAS_IR_SUPPORT
/*!
 * @brief  Configure the UART infrared operation.
 *
 * The function allows the user to enable or disable the UART infrared (IR) operation
 * and to configure the IR pulse width.
 *
 * @param   uartInstance UART module instance number.
 * @param   enable Enable (true) or disable (false) the infrared operation.
 * @param   pulseWidth The UART transmit narrow pulse width setting of type uart_ir_tx_pulsewidth_t.
 */
void uart_hal_configure_infrared_operation(uint32_t uartInstance, bool enable,
                                           uart_ir_tx_pulsewidth_t pulseWidth);
#endif  /* FSL_FEATURE_UART_HAS_IR_SUPPORT*/

/*@}*/

/*!
 * @name UART Status Flags
 * @{
 */

/*!
 * @brief  Get all of the UART status flag states.
 *
 * This function gets all of the UART status flag states and places into a structure of
 * type uart_status_flag_all_t.  The user must pass in a pointer to this structure.
 *
 * @param   uartInstance UART module instance number.
 * @param   allStatusFlag Pointer to the structure of all the UART status flags states.
 */
void uart_hal_get_all_status_flag(uint32_t uartInstance, uart_status_flag_all_t *allStatusFlag);

/*!
 * @brief  Get the UART Transmit data register empty flag.
 *
 * This function returns the state of the UART Transmit data register empty flag.
 *
 * @param uartInstance UART module instance number.
 * @return The status of Transmit data register empty flag, which is set when transmit buffer
 *          is empty.
 */
bool uart_hal_is_transmit_data_register_empty(uint32_t uartInstance);

/*!
 * @brief  Get the UART Transmission complete flag.
 *
 * This function returns the state of the UART Transmission complete flag.
 *
 * @param uartInstance UART module instance number.
 * @return The status of Transmission complete flag, which is set when the transmitter is idle
 *         (transmission activity complete).
 */
bool uart_hal_is_transmission_complete(uint32_t uartInstance);

/*!
 * @brief  Get the UART Receive data register full flag.
 *
 * This function returns the state of the UART Receive data register full flag.
 *
 * @param uartInstance UART module instance number.
 * @return The status of Receive data register full flag, which is set when the receive data buffer
 *         is full.
 */
bool uart_hal_is_receive_data_register_full(uint32_t uartInstance);

/*!
 * @brief  Get the UART Idle-line detect flag.
 *
 * This function returns the state of the UART Idle-line detect flag.
 *
 * @param uartInstance UART module instance number.
 * @return The status of Idle-line detect flag which is set when an idle-line detected.
 */
bool uart_hal_is_idle_line_detected(uint32_t uartInstance);

/*!
 * @brief  Get the UART Receiver Overrun status flag.
 *
 * This function returns the state of the the UART Receiver Overrun status flag.
 *
 * @param uartInstance UART module instance number.
 * @return The status of Receiver Overrun, which is set when new data is received before data is
 *         read from receive register.
 */
bool uart_hal_is_receive_overrun_detected(uint32_t uartInstance);

/*!
 * @brief  Get the UART noise status flag.
 *
 * This function returns the state of the UART noise status flag.
 *
 * @param uartInstance UART module instance number.
 * @return The status of the noise flag, which is set if any of the 3 samples taken on
 *         receive differ.
 */
bool uart_hal_is_noise_detected(uint32_t uartInstance);

/*!
 * @brief  Get the UART Frame error status flag.
 *
 * This function returns the state of the UART Frame error status flag.
 *
 * @param uartInstance UART module instance number.
 * @return The status of Frame error flag, which is set if a logic 0 was detected where a stop bit
 *         was expected.
 */
bool uart_hal_is_frame_error_detected(uint32_t uartInstance);

/*!
 * @brief  Get the UART parity error status flag.
 *
 * This function returns the state of the UART parity error status flag.
 *
 * @param uartInstance UART module instance number.
 * @return The status of parity error detection flag, which is set if parity mode enabled and the
 *         parity bit received does not match what was expected.
 */
bool uart_hal_is_parity_error_detected(uint32_t uartInstance);

/*!
 * @brief  Get the UART LIN break detect interrupt status flag.
 *
 * This function returns the state of the UART LIN break detect interrupt status flag.
 *
 * @param uartInstance UART module instance number.
 * @return The status of LIN break detect interrupt flag, which is set when the LIN break char
 *         is detected assuming the LIN circuit is enabled.
 */
bool uart_hal_is_line_break_detected(uint32_t uartInstance);

/*!
 * @brief  Get the UART Receive pin active edge interrupt status flag.
 *
 * This function returns the state of the UART Receive pin active edge interrupt status flag.
 *
 * @param uartInstance UART module instance number.
 * @return The status of Receive pin active edge interrupt flag, which is set when active edge
 *         detected on the receive pin.
 */
bool uart_hal_is_receive_active_edge_detected(uint32_t uartInstance);

/*!
 * @brief  Get the UART Receiver Active Flag (RAF) state.
 *
 * This function returns the state of the UART Receiver Active Flag (RAF).
 *
 * @param uartInstance UART module instance number.
 * @return The status of the Receiver Active Flag (RAF), which is set at the beginning of a
 *         received valid start bit.
 */
bool uart_hal_is_receiver_active(uint32_t uartInstance);

/*!
 * @brief  Clear an individual and specific UART status flag.
 *
 * This function allows the user to clear an individual and specific UART status flag. Refer to
 * structure definition uart_status_flag_t for list of status bits.
 *
 * @param uartInstance UART module instance number.
 * @param statusFlag The desired UART status flag to clear.
 * @return An error code or kStatus_UART_Success.
 */
uart_status_t uart_hal_clear_status_flag(uint32_t uartInstance, uart_status_flag_t statusFlag);

/*!
 * @brief  Clear ALL of the UART status flags.
 *
 * This function tries to clear all of the UART status flags.  In some cases, some of the status
 * flags may not get cleared because the condition that set the flag may still exist.
 *
 * @param   uartInstance UART module instance number.
 */
void uart_hal_clear_all_non_autoclear_status_flags(uint32_t uartInstance);

/*@}*/

/*!
 * @name UART FIFO Configurations
 * @{
 */

#if FSL_FEATURE_UART_HAS_FIFO
/*!
 * @brief  Enable the UART transmit FIFO.
 *
 * This function allows the user to enable the UART transmit FIFO.
 * It is required that the transmitter/receiver be disabled before calling this function
 * when the FIFO is empty.
 * Additionally, TXFLUSH and RXFLUSH commands should be issued after calling this function.
 *
 * @param uartInstance UART module instance number.
 * @return Error code if it is detected that the transmitter or receiver is enabled or
 *         kStatus_UART_Success.
 */
uart_status_t uart_hal_enable_tx_fifo(uint32_t uartInstance);

/*!
 * @brief  Disable the UART transmit FIFO.
 *
 * This function allows the user to disable the UART transmit FIFO.
 * It is required that the transmitter/receiver be disabled before calling this function
 * when the FIFO is empty.
 * Additionally, TXFLUSH and RXFLUSH commands should be issued after calling this function.
 *
 * @param uartInstance UART module instance number.
 * @return Error code if it is detected that the transmitter or receiver is enabled or
 *         kStatus_UART_Success.
 */
uart_status_t uart_hal_disable_tx_fifo(uint32_t uartInstance);

/*!
 * @brief  Enable the UART receive FIFO.
 *
 * This function allows the user to enable the UART receive FIFO.
 * It is required that the transmitter/receiver be disabled before calling this function
 * when the FIFO is empty.
 * Additionally, TXFLUSH and RXFLUSH commands should be issued after calling this function.
 *
 * @param uartInstance UART module instance number.
 * @return Error code if it is detected that the transmitter or receiver is enabled or
 *         kStatus_UART_Success.
 */
uart_status_t uart_hal_enable_rx_fifo(uint32_t uartInstance);

/*!
 * @brief  Disable the UART receive FIFO.
 *
 * This function allows the user to disable the UART receive FIFO.
 * It is required that the transmitter/receiver be disabled before calling this function
 * when the FIFO is empty.
 * Additionally, TXFLUSH and RXFLUSH commands should be issued after calling this function.
 *
 * @param uartInstance UART module instance number.
 * @return Error code if it is detected that the transmitter or receiver is enabled or
 *         kStatus_UART_Success.
 */
uart_status_t uart_hal_disable_rx_fifo(uint32_t uartInstance);

/*!
 * @brief  Get the size of the UART transmit FIFO.
 *
 * This function returns the size (number of entries) supported in the UART transmit FIFO for
 * a particular module instance.
 *
 * @param uartInstance UART module instance number.
 * @return  The UART transmit FIFO size as follows:
 *    0x0: 1 data word; 0x1: 4 data words; 0x2: 8 data words; 0x3: 16 data words
 *    0x4: 32 data words; 0x5: 64 data words; 0x6: 128 data words; 0x7: reserved
 */
uint8_t uart_hal_get_tx_fifo_size(uint32_t uartInstance);

/*!
 * @brief  Get the size of the UART receive FIFO.
 *
 * This function returns the size (number of entries) supported in the UART receive FIFO for
 * a particular module instance.
 *
 * @param   uartInstance UART module instance number.
 * @return  The receive FIFO size as follows:
 *    0x0: 1 data word; 0x1: 4 data words; 0x2: 8 data words; 0x3: 16 data words
 *    0x4: 32 data words; 0x5: 64 data words; 0x6: 128 data words; 0x7: reserved
 */
uint8_t uart_hal_get_rx_fifo_size(uint32_t uartInstance);

/*!
 * @brief  Flush the UART transmit FIFO.
 *
 * This function allows the user to flush the UART transmit FIFO for a particular module instance.
 * Flushing the FIFO may result in data loss.
 * It is recommended that the transmitter be disabled before calling this function.
 *
 * @param uartInstance UART module instance number.
 * @return Error code if it is detected that the transmitter or receiver is enabled or
 *         kStatus_UART_Success.
 */
uart_status_t uart_hal_flush_tx_fifo(uint32_t uartInstance);

/*!
 * @brief  Flush the UART receive FIFO.
 *
 * This function allows the user to flush the UART receive FIFO for a particular module instance.
 * Flushing the FIFO may result in data loss.
 * It is recommended that the receiver be disabled before calling this function.
 *
 * @param uartInstance UART module instance number.
 * @return Error code if it is detected that the transmitter or receiver is enabled or
 *         kStatus_UART_Success.
 */
uart_status_t uart_hal_flush_rx_fifo(uint32_t uartInstance);

/*!
 * @brief  Enable the UART transmit FIFO overflow interrupt.
 *
 * @param   uartInstance UART module instance number.
 */
void uart_hal_enable_tx_fifo_overflow_interrupt(uint32_t uartInstance);


/*!
 * @brief  Disable the UART transmit FIFO overflow interrupt.
 *
 * @param   uartInstance UART module instance number.
 */
void uart_hal_disable_tx_fifo_overflow_interrupt(uint32_t uartInstance);


/*!
 * @brief  Get the configuration of the UART transmit FIFO overflow interrupt enable.
 *
 * @param   uartInstance UART module instance number.
 * @return  The UART transmit FIFO overflow interrupt enable state: true=enable; false=disable.
 */
bool uart_hal_is_tx_fifo_overflow_interrupt_enabled(uint32_t uartInstance);


/*!
 * @brief  Enable the UART receive FIFO underflow interrupt.
 *
 * @param   uartInstance UART module instance number.
 */
void uart_hal_enable_rx_fifo_underflow_interrupt(uint32_t uartInstance);


/*!
 * @brief  Disable the UART receive FIFO underflow interrupt.
 *
 * @param   uartInstance UART module instance number.
 */
void uart_hal_disable_rx_fifo_underflow_interrupt(uint32_t uartInstance);


/*!
 * @brief  Get the configuration of the UART receive FIFO underflow interrupt enable.
 *
 * @param   uartInstance UART module instance number.
 * @return  The UART receive FIFO underflow interrupt enable state: true=enable; false=disable.
 */
bool uart_hal_is_rx_fifo_underflow_interrupt_enabled(uint32_t uartInstance);

/*!
 * @brief  Get the UART transmit FIFO empty status state.
 *
 * The function returns the state of the transmit FIFO empty status state, but does not take into
 * account data in the shift register.
 *
 * @param   uartInstance UART module instance number.
 * @return  The UART transmit FIFO empty status: true=empty; false=not-empty.
 */
bool uart_hal_is_tx_fifo_empty(uint32_t uartInstance);

/*!
 * @brief  Get the UART receive FIFO empty status state.
 *
 * The function returns the state of the receive FIFO empty status state, but does not take into
 * account data in the shift register.
 *
 * @param   uartInstance UART module instance number.
 * @return  The UART receive FIFO empty status: true=empty; false=not-empty.
 */
bool uart_hal_is_rx_fifo_empty(uint32_t uartInstance);

/*!
 * @brief  Get the UART transmit FIFO overflow status state.
 *
 * @param   uartInstance UART module instance number.
 * @return  The UART transmit FIFO overflow status state: true=overflow; false=no overflow.
 */
bool uart_hal_is_tx_fifo_overflow(uint32_t uartInstance);

/*!
 * @brief  Clear the UART transmit FIFO overflow status.
 *
 * @param   uartInstance UART module instance number.
 */
void  uart_hal_clear_tx_fifo_overflow(uint32_t uartInstance);

/*!
 * @brief  Get the UART receive FIFO underflow status state.
 *
 * @param   uartInstance UART module instance number.
 * @return  The receive FIFO underflow status state: true=overflow; false=no overflow.
 */
bool uart_hal_is_rx_fifo_underflow(uint32_t uartInstance);

/*!
 * @brief  Clear the UART receive FIFO underflow status.
 *
 * @param   uartInstance UART module instance number.
 */
void  uart_hal_clear_rx_fifo_underflow(uint32_t uartInstance);

/*!
 * @brief  Set the UART transmit FIFO watermark value.
 *
 * Programming the transmit watermark should be done when UART the transmitter is disabled
 * and the value must be set less than the size obtained from uart_hal_get_tx_fifo_size.
 *
 * @param   uartInstance UART module instance number.
 * @param  watermark  The UART transmit watermark value to be programmed.
 * @return  Error code if transmitter is enabled or kStatus_UART_Success.
 */
uart_status_t uart_hal_set_tx_fifo_watermark(uint32_t uartInstance, uint8_t watermark);

/*!
 * @brief  Get the UART transmit FIFO watermark value.
 *
 * @param   uartInstance UART module instance number.
 * @return  The value currently programmed for the UART transmit watermark.
 */
uint8_t uart_hal_get_tx_fifo_watermark(uint32_t uartInstance);

/*!
 * @brief  Get the UART transmit FIFO data word count (number of words in the transmit FIFO).
 *
 * The function uart_hal_get_tx_dataword_count_in_fifo excludes any data that may
 * be in the UART transmit shift register
 *
 * @param   uartInstance UART module instance number.
 * @return  The number of data words currently in the UART transmit FIFO.
 */
uint8_t uart_hal_get_tx_dataword_count_in_fifo(uint32_t uartInstance);

/*!
 * @brief  Set the UART receive FIFO watermark value.
 *
 * Programming the receive watermark should be done when the receiver is disabled
 * and the value must be set less than the size obtained from uart_hal_get_rx_fifo_size and
 * greater than zero.
 *
 * @param   uartInstance UART module instance number.
 * @param  watermark  The UART receive watermark value to be programmed.
 * @return  Error code if receiver is enabled or kStatus_UART_Success.
 */
uart_status_t uart_hal_set_rx_fifo_watermark(uint32_t uartInstance, uint8_t watermark);

/*!
 * @brief  Get the UART receive FIFO data word count (number of words in the receive FIFO).
 *
 * The function uart_hal_get_rx_dataword_count_in_fifo excludes any data that may be
 * in the receive shift register.
 *
 * @param   uartInstance UART module instance number.
 * @return  The number of data words currently in the UART receive FIFO.
 */
uint8_t uart_hal_get_rx_dataword_count_in_fifo(uint32_t uartInstance);

/*!
 * @brief  Get the UART receive FIFO watermark value.
 *
 * @param   uartInstance UART module instance number.
 * @return  The value currently programmed for the UART receive watermark.
 */
uint8_t uart_hal_get_rx_fifo_watermark(uint32_t uartInstance);

#endif  /* FSL_FEATURE_UART_HAS_FIFO*/

/*@}*/

#if defined(__cplusplus)
}
#endif

/*! @}*/

#endif /* __FSL_UART_HAL_H__*/
/*******************************************************************************
 * EOF
 ******************************************************************************/

