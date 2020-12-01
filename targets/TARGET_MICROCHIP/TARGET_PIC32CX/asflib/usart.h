/**
 * \file
 *
 * \brief Universal Synchronous Asynchronous Receiver Transmitter (USART) driver
 * for SAM.
 *
 * Copyright (c) 2011-2020 Microchip Technology Inc. and its subsidiaries.
 *
 * \asf_license_start
 *
 * \page License
 *
 * Subject to your compliance with these terms, you may use Microchip
 * software and any derivatives exclusively with Microchip products.
 * It is your responsibility to comply with third party license terms applicable
 * to your use of third party software (including open source software) that
 * may accompany Microchip software.
 *
 * THIS SOFTWARE IS SUPPLIED BY MICROCHIP "AS IS". NO WARRANTIES,
 * WHETHER EXPRESS, IMPLIED OR STATUTORY, APPLY TO THIS SOFTWARE,
 * INCLUDING ANY IMPLIED WARRANTIES OF NON-INFRINGEMENT, MERCHANTABILITY,
 * AND FITNESS FOR A PARTICULAR PURPOSE. IN NO EVENT WILL MICROCHIP BE
 * LIABLE FOR ANY INDIRECT, SPECIAL, PUNITIVE, INCIDENTAL OR CONSEQUENTIAL
 * LOSS, DAMAGE, COST OR EXPENSE OF ANY KIND WHATSOEVER RELATED TO THE
 * SOFTWARE, HOWEVER CAUSED, EVEN IF MICROCHIP HAS BEEN ADVISED OF THE
 * POSSIBILITY OR THE DAMAGES ARE FORESEEABLE.  TO THE FULLEST EXTENT
 * ALLOWED BY LAW, MICROCHIP'S TOTAL LIABILITY ON ALL CLAIMS IN ANY WAY
 * RELATED TO THIS SOFTWARE WILL NOT EXCEED THE AMOUNT OF FEES, IF ANY,
 * THAT YOU HAVE PAID DIRECTLY TO MICROCHIP FOR THIS SOFTWARE.
 *
 * \asf_license_stop
 *
 */
/*
 * Support and FAQ: visit <a href="https://www.microchip.com/support/">Microchip Support</a>
 */

#ifndef USART_H_INCLUDED
#define USART_H_INCLUDED

#include "compiler.h"
#include "pic32cx.h"

/**
 * \defgroup group_sam_drivers_usart Universal Synchronous Asynchronous Receiver
 * Transmitter (USART).
 *
 * See \ref sam_usart_quickstart.
 *
 * This is a low-level driver implementation for the SAM Universal
 * Synchronous/Asynchronous Receiver/Transmitter.
 *
 * @{
 */

/// @cond 0
/**INDENT-OFF**/
#ifdef __cplusplus
extern "C" {
#endif
/**INDENT-ON**/
/// @endcond

/** Clock phase. */
#define SPI_CPHA    (1 << 0)

/** Clock polarity. */
#define SPI_CPOL    (1 << 1)

/** SPI mode definition. */
#define SPI_MODE_0  0
#define SPI_MODE_1  (SPI_CPHA)
#define SPI_MODE_2  (SPI_CPOL)
#define SPI_MODE_3  (SPI_CPOL | SPI_CPHA)

/** Time-out value (number of attempts). */
#define USART_TIMEOUT       15000

#define USART_FIFO_SIZE   8

enum usart_comparator_mode {
	USART_CMPMODE_FLAG_ONLY = US_CMPR_CMPMODE_FLAG_ONLY,
	USART_CMPMODE_START_CONDITION = US_CMPR_CMPMODE_START_CONDITION,
	USART_CMPMODE_FILTER = US_CMPR_CMPMODE_FILTER,
};

enum usart_source_clk {
	USART_SRC_CLK_MCK = US_MR_USCLKS_MCK,
	USART_SRC_CLK_DIV = US_MR_USCLKS_DIV,
	USART_SRC_CLK_GCLK = US_MR_USCLKS_GCLK,
	USART_SRC_CLK_SCK = US_MR_USCLKS_SCK,
};

enum usart_man_start_frame_delimiter {
	USART_MAN_START_DELIM_ONE_BIT,
	USART_MAN_START_DELIM_CMD_SYNC,
	USART_MAN_START_DELIM_DATA_SYNC,
	USART_MAN_START_DELIM_VAR_SYNC,
};

enum usart_tx_ready_flag_mode {
	USART_TX_READY_FLAG_ONE_DATA = US_FMR_TXRDYM_ONE_DATA,
	USART_TX_READY_FLAG_TWO_DATA = US_FMR_TXRDYM_TWO_DATA,
	USART_TX_READY_FLAG_FOUR_DATA = US_FMR_TXRDYM_FOUR_DATA,
};

enum usart_rx_ready_flag_mode {
	USART_RX_READY_FLAG_ONE_DATA = US_FMR_RXRDYM_ONE_DATA,
	USART_RX_READY_FLAG_TWO_DATA = US_FMR_RXRDYM_TWO_DATA,
	USART_RX_READY_FLAG_FOUR_DATA = US_FMR_RXRDYM_FOUR_DATA,
};

/* Input parameters when initializing RS232 and similar modes. */
typedef struct {
	/* Set baud rate of the USART (unused in slave modes). */
	uint32_t baudrate;

	/*
	 * Number of bits, which should be one of the following: US_MR_CHRL_5_BIT,
	 * US_MR_CHRL_6_BIT, US_MR_CHRL_7_BIT, US_MR_CHRL_8_BIT or
	 * US_MR_MODE9.
	 */
	uint32_t char_length;

	/*
	 * Parity type, which should be one of the following: US_MR_PAR_EVEN,
	 * US_MR_PAR_ODD, US_MR_PAR_SPACE, US_MR_PAR_MARK, US_MR_PAR_NO
	 * or US_MR_PAR_MULTIDROP.
	 */
	uint32_t parity_type;

	/*
	 * Number of stop bits between two characters: US_MR_NBSTOP_1_BIT,
	 * US_MR_NBSTOP_1_5_BIT, US_MR_NBSTOP_2_BIT.
	 * \note US_MR_NBSTOP_1_5_BIT is supported in asynchronous modes only.
	 */
	uint32_t stop_bits;

	/*
	 * Run the channel in test mode, which should be one of following:
	 * US_MR_CHMODE_NORMAL, US_MR_CHMODE_AUTOMATIC,
	 * US_MR_CHMODE_LOCAL_LOOPBACK, US_MR_CHMODE_REMOTE_LOOPBACK.
	 */
	uint32_t channel_mode;

	/* Filter of IrDA mode, useless in other modes. */
	uint32_t irda_filter;
} sam_usart_opt_t;

/* Input parameters when initializing ISO7816 mode. */
typedef struct {
	/* Set the frequency of the ISO7816 clock. */
	uint32_t iso7816_hz;

	/*
	 * The number of ISO7816 clock ticks in every bit period (1 to 2047,
	 * 0 = disable clock). Baudrate rate = iso7816_hz / fidi_ratio.
	 */
	uint32_t fidi_ratio;

	/*
	 * How to calculate the parity bit: US_MR_PAR_EVEN for normal mode or
	 * US_MR_PAR_ODD for inverse mode.
	 */
	uint32_t parity_type;

	/*
	 * Inhibit Non Acknowledge:
	 *   - 0: the NACK is generated;
	 *   - 1: the NACK is not generated.
	 *
	 * \note This bit will be used only in ISO7816 mode, protocol T = 0
	 * receiver.
	 */
	uint32_t inhibit_nack;

	/*
	 * Disable successive NACKs.
	 *  - 0: NACK is sent on the ISO line as soon as a parity error occurs
	 * in the received character. Successive parity errors are counted up to
	 * the value in the max_iterations field. These parity errors generate
	 * a NACK on the ISO line. As soon as this value is reached, no additional
	 * NACK is sent on the ISO line. The ITERATION flag is asserted.
	 */
	uint32_t dis_suc_nack;

	/* Max number of repetitions (0 to 7). */
	uint32_t max_iterations;

	/*
	 * Bit order in transmitted characters:
	 *   - 0: LSB first;
	 *   - 1: MSB first.
	 */
	uint32_t bit_order;

	/*
	 * Which protocol is used:
	 *   - 0: T = 0;
	 *   - 1: T = 1.
	 */
	uint32_t protocol_type;
} usart_iso7816_opt_t;


void usart_reset(Usart *p_usart);
uint32_t usart_set_async_baudrate(Usart *p_usart,
		uint32_t baudrate, uint32_t ul_mck);
uint32_t usart_init_rs232(Usart *p_usart,
		const sam_usart_opt_t *p_usart_opt, uint32_t ul_mck);
uint32_t usart_init_hw_handshaking(Usart *p_usart,
		const sam_usart_opt_t *p_usart_opt, uint32_t ul_mck);
uint32_t usart_init_sync_master(Usart *p_usart,
		const sam_usart_opt_t *p_usart_opt, uint32_t ul_mck);
uint32_t usart_init_sync_slave(Usart *p_usart,
		const sam_usart_opt_t *p_usart_opt);
uint32_t usart_init_rs485(Usart *p_usart,
		const sam_usart_opt_t *p_usart_opt, uint32_t ul_mck);
uint32_t usart_init_irda(Usart *p_usart,
		const sam_usart_opt_t *p_usart_opt, uint32_t ul_mck);
uint32_t usart_init_iso7816(Usart *p_usart,
		const usart_iso7816_opt_t *p_usart_opt, uint32_t ul_mck);
void usart_reset_iterations(Usart *p_usart);
void usart_reset_nack(Usart *p_usart);
uint32_t usart_is_rx_buf_end(Usart *p_usart);
uint32_t usart_is_tx_buf_end(Usart *p_usart);
uint32_t usart_is_rx_buf_full(Usart *p_usart);
uint32_t usart_is_tx_buf_empty(Usart *p_usart);
uint8_t usart_get_error_number(Usart *p_usart);
uint32_t usart_init_lin_master(Usart *p_usart, uint32_t ul_baudrate,
		uint32_t ul_mck);
uint32_t usart_init_lin_slave(Usart *p_usart, uint32_t ul_baudrate,
		uint32_t ul_mck);
void usart_lin_abort_tx(Usart *p_usart);
void usart_lin_send_wakeup_signal(Usart *p_usart);
void usart_lin_set_node_action(Usart *p_usart, uint8_t uc_action);
void usart_lin_disable_parity(Usart *p_usart);
void usart_lin_enable_parity(Usart *p_usart);
void usart_lin_disable_checksum(Usart *p_usart);
void usart_lin_enable_checksum(Usart *p_usart);
void usart_lin_set_checksum_type(Usart *p_usart, uint8_t uc_type);
void usart_lin_set_data_len_mode(Usart *p_usart, uint8_t uc_mode);
void usart_lin_disable_frame_slot(Usart *p_usart);
void usart_lin_enable_frame_slot(Usart *p_usart);
void usart_lin_set_wakeup_signal_type(Usart *p_usart, uint8_t uc_type);
void usart_lin_set_response_data_len(Usart *p_usart, uint32_t ul_len);
void usart_lin_disable_pdc_mode(Usart *p_usart);
void usart_lin_enable_pdc_mode(Usart *p_usart);
void usart_lin_set_tx_identifier(Usart *p_usart, uint8_t uc_id);
uint8_t usart_lin_read_identifier(Usart *p_usart);
uint8_t usart_lin_get_data_length(Usart *usart);
void usart_enable_tx(Usart *p_usart);
void usart_disable_tx(Usart *p_usart);
void usart_reset_tx(Usart *p_usart);
void usart_set_tx_timeguard(Usart *p_usart, uint32_t timeguard);
void usart_enable_rx(Usart *p_usart);
void usart_disable_rx(Usart *p_usart);
void usart_reset_rx(Usart *p_usart);
void usart_set_rx_timeout(Usart *p_usart, uint32_t timeout);
void usart_enable_interrupt(Usart *p_usart, uint32_t ul_sources);
void usart_disable_interrupt(Usart *p_usart, uint32_t ul_sources);
uint32_t usart_get_interrupt_mask(Usart *p_usart);
uint32_t usart_get_status(Usart *p_usart);
void usart_reset_status(Usart *p_usart);
void usart_start_tx_break(Usart *p_usart);
void usart_stop_tx_break(Usart *p_usart);
void usart_start_rx_timeout(Usart *p_usart);
uint32_t usart_send_address(Usart *p_usart, uint32_t ul_addr);
void usart_restart_rx_timeout(Usart *p_usart);
void usart_drive_RTS_pin_low(Usart *p_usart);
void usart_drive_RTS_pin_high(Usart *p_usart);
uint32_t usart_is_tx_ready(Usart *p_usart);
uint32_t usart_is_tx_empty(Usart *p_usart);
uint32_t usart_is_rx_ready(Usart *p_usart);
uint32_t usart_write(Usart *p_usart, uint32_t c);
uint32_t usart_putchar(Usart *p_usart, uint32_t c);
void usart_write_line(Usart *p_usart, const char *string);
uint32_t usart_read(Usart *p_usart, uint32_t *c);
uint32_t usart_getchar(Usart *p_usart, uint32_t *c);
Pdc *usart_get_pdc_base(Usart *p_usart);

/**
 * \brief Enable write protect of USART Configuration registers.
 *
 * \param p_usart Pointer to a USART instance.
 */
static inline void usart_enable_writeprotect_cfg_regs(Usart *p_usart)
{
	p_usart->US_WPMR = US_WPMR_WPEN | US_WPMR_WPKEY_PASSWD;
}

/**
 * \brief Enable write protect of USART Control registers.
 *
 * \param p_usart Pointer to a USART instance.
 */
static inline void usart_enable_writeprotect_ctl_regs(Usart *p_usart)
{
	p_usart->US_WPMR = US_WPMR_WPCREN | US_WPMR_WPKEY_PASSWD;
}

/**
 * \brief Enable write protect of USART Interrupt registers.
 *
 * \param p_usart Pointer to a USART instance.
 */
static inline void usart_enable_writeprotect_int_regs(Usart *p_usart)
{
	p_usart->US_WPMR = US_WPMR_WPITEN | US_WPMR_WPKEY_PASSWD;
}

/**
 * \brief Enable write protect of all USART registers.
 *
 * \param p_usart Pointer to a USART instance.
 */
static inline void usart_enable_writeprotect_all_regs(Usart *p_usart)
{
	p_usart->US_WPMR = US_WPMR_WPEN | US_WPMR_WPCREN | 
			US_WPMR_WPITEN | US_WPMR_WPKEY_PASSWD;
}

/**
 * \brief Disable write protect of all USART registers.
 *
 * \param p_usart Pointer to a USART instance.
 */
static inline void usart_disable_writeprotect_all_regs(Usart *p_usart)
{
	p_usart->US_WPMR = US_WPMR_WPKEY_PASSWD;
}

uint32_t usart_get_writeprotect_status(Usart *p_usart);
void usart_man_set_tx_pre_len(Usart *p_usart, uint8_t uc_len);
void usart_man_set_tx_pre_pattern(Usart *p_usart, uint8_t uc_pattern);
void usart_man_set_tx_polarity(Usart *p_usart, uint8_t uc_polarity);
void usart_man_set_rx_pre_len(Usart *p_usart, uint8_t uc_len);
void usart_man_set_rx_pre_pattern(Usart *p_usart, uint8_t uc_pattern);
void usart_man_set_rx_polarity(Usart *p_usart, uint8_t uc_polarity);
void usart_man_enable_drift_compensation(Usart *p_usart);
void usart_man_disable_drift_compensation(Usart *p_usart);
void usart_man_set_frame_start_delimiter(Usart *p_usart,
		enum usart_man_start_frame_delimiter start_delim);

/**
 * \brief Sets the receiver idle line to high state.
 *
 * \param p_usart Pointer to a USART instance.
 */
static inline void usart_man_set_receiver_idle_line_high(Usart *p_usart)
{
	p_usart->US_MAN |= US_MAN_RXIDLEV;
}

/**
 * \brief Sets the receiver idle line to low state.
 *
 * \param p_usart Pointer to a USART instance.
 */
static inline void usart_man_set_receiver_idle_line_low(Usart *p_usart)
{
	p_usart->US_MAN &= ~US_MAN_RXIDLEV;
}

/**
 * \brief Enables USART Manchester Encoder/Decoder.
 *
 * \param p_usart Pointer to a USART instance.
 */
static inline void usart_man_enable(Usart *p_usart)
{
	p_usart->US_MR |= US_MR_MAN;
}

/**
 * \brief Disables USART Manchester Encoder/Decoder
 *
 * \param p_usart Pointer to a USART instance.
 */
static inline void usart_man_disable(Usart *p_usart)
{
	p_usart->US_MR &= ~US_MR_MAN;
}

uint32_t usart_set_comparison_on_rx_char(Usart *p_usart, uint8_t ul_val1,
		enum usart_comparator_mode cmpmode, bool cmppar, uint8_t ul_val2);
uint32_t usart_set_sleepwalking(Usart *p_usart, uint8_t ul_val1,
		bool cmppar, uint8_t ul_val2);
void usart_init_data16bit_master(Usart *p_usart);
void usart_init_data16bit_slave(Usart *p_usart);
void usart_enable_fifo(Usart *p_usart);
void usart_disable_fifo(Usart *p_usart);
uint32_t usart_fifo_write_buf(Usart *p_usart, uint8_t *p_tx_data, uint8_t uc_tx_data_len);
uint32_t usart_fifo_read_buf(Usart *p_usart, uint8_t *p_rx_data, uint8_t uc_rx_data_len);
uint32_t usart_set_fifo_tx_ready_flag_mode(Usart *p_usart,
		enum usart_tx_ready_flag_mode txrdy_mode);
uint32_t usart_set_fifo_rx_ready_flag_mode(Usart *p_usart,
		enum usart_rx_ready_flag_mode rxrdy_mode);


/**
 * \brief Clear USART transmit FIFO.
 *
 * \param p_usart Pointer to a USART instance.
 */
static inline void usart_clear_tx_fifo(Usart *p_usart)
{
	p_usart->US_CR = US_CR_TXFCLR;
}

/**
 * \brief Clear USART receive FIFO.
 *
 * \param p_usart Pointer to a USART instance.
 */
static inline void usart_clear_rx_fifo(Usart *p_usart)
{
	p_usart->US_CR = US_CR_RXFCLR;
}

/**
 * \brief Clear USART transmit FIFO lock.
 *
 * \param p_usart Pointer to a USART instance.
 */
static inline void usart_clear_tx_fifo_lock(Usart *p_usart)
{
	p_usart->US_CR = US_CR_TXFLCLR;
}

/**
 * \brief Clear USART comparison trigger.
 *
 * \param p_usart Pointer to a USART instance.
 */
static inline void usart_clear_comparison_trigger(Usart *p_usart)
{
	p_usart->US_CR = US_CR_REQCLR;
}

/**
 * \brief Select the Source Clock for the USART.
 *
 * \param p_usart Pointer to a USART instance.
 * \param clksrc Clock source as defined in enum usart_source_clk.
 */
static inline void usart_set_clk_source(Usart *p_usart, enum usart_source_clk clksrc)
{
	p_usart->US_MR = (p_usart->US_MR & ~US_MR_USCLKS_Msk) | clksrc;
}

/**
 * \brief Enable USART Line Filter.
 *
 * \param p_usart Pointer to a USART instance.
 */
static inline void usart_enable_line_filter(Usart *p_usart)
{
	p_usart->US_MR |= US_MR_FILTER;
}

/**
 * \brief Disable USART Line Filter.
 *
 * \param p_usart Pointer to a USART instance.
 */
static inline void usart_disable_line_filter(Usart *p_usart)
{
	p_usart->US_MR &= ~US_MR_FILTER;
}

/**
 * \brief Enable USART Inverted Polarity mode.
 *
 * \param p_usart Pointer to a USART instance.
 */
static inline void usart_enable_inverted_polarity(Usart *p_usart)
{
	p_usart->US_MR |= US_MR_INVDATA;
}

/**
 * \brief Disable USART Inverted Polarity mode.
 *
 * \param p_usart Pointer to a USART instance.
 */
static inline void usart_disable_inverted_polarity(Usart *p_usart)
{
	p_usart->US_MR &= ~US_MR_INVDATA;
}

/**
 * \brief Enable USART OOK Modulation/Demodulation.
 *
 * \param p_usart Pointer to a USART instance.
 */
static inline void usart_ook_enable_mod_demod(Usart *p_usart)
{
	p_usart->US_MR |= US_MR_OOKEN;
}

/**
 * \brief Disable USART OOK Modulation/Demodulation.
 *
 * \param p_usart Pointer to a USART instance.
 */
static inline void usart_ook_disable_mod_demod(Usart *p_usart)
{
	p_usart->US_MR &= ~US_MR_OOKEN;
}

/**
 * \brief Set USART OOK Demodulator to use Output of Analog Comparator.
 *
 * \param p_usart Pointer to a USART instance.
 */
static inline void usart_ook_demod_use_analog_comparator(Usart *p_usart)
{
	p_usart->US_MR &= ~US_MR_OOKRXD;
}

/**
 * \brief Set USART OOK Demodulator to use RXD line.
 *
 * \param p_usart Pointer to a USART instance.
 */
static inline void usart_ook_demod_use_rxd_line(Usart *p_usart)
{
	p_usart->US_MR |= US_MR_OOKRXD;
}

/**
 * \brief Enable USART synchronization procedure in LIN Slave mode.
 *
 * \param p_usart Pointer to a USART instance.
 */
static inline void usart_lin_enable_sync(Usart *p_usart)
{
	p_usart->US_LINMR &= ~US_LINMR_SYNCDIS;
}

/**
 * \brief Disable USART synchronization procedure in LIN Slave mode.
 *
 * \param p_usart Pointer to a USART instance.
 */
static inline void usart_lin_disable_sync(Usart *p_usart)
{
	p_usart->US_LINMR |= US_LINMR_SYNCDIS;
}

/**
 * \brief Configure the LIN identifier when USART works in LIN master mode.
 *
 * \param p_usart Pointer to a USART instance.
 *
 * \return Value of LIN Baudrate register after synchronization.
 */
static inline uint32_t usart_lin_get_slave_baudrate_reg(Usart *p_usart)
{
	return p_usart->US_LINBRR;
}

/**
 * \brief Get number of data in the Tx USART FIFO.
 *
 * \param p_usart Pointer to a USART instance.
 *
 * \return Number of data in the Transmit FIFO.
 */
static inline uint32_t usart_get_fifo_tx_level(Usart *p_usart)
{
	return ((p_usart->US_FLR & US_FLR_TXFL_Msk) >> US_FLR_TXFL_Pos);
}

/**
 * \brief Get number of unread data in the Rx USART FIFO.
 *
 * \param p_usart Pointer to a USART instance.
 *
 * \return Number of unread data in the Receive FIFO.
 */
static inline uint32_t usart_get_fifo_rx_level(Usart *p_usart)
{
	return ((p_usart->US_FLR & US_FLR_RXFL_Msk) >> US_FLR_RXFL_Pos);
}

/**
 * \brief Enable USART FIFO RTS pin control.
 *
 * \param p_usart Pointer to a USART instance.
 */
static inline void usart_enable_fifo_rts_pin_ctl(Usart *p_usart)
{
	p_usart->US_FMR |= US_FMR_FRTSC;
}

/**
 * \brief Disable USART FIFO RTS pin control.
 *
 * \param p_usart Pointer to a USART instance.
 */
static inline void usart_disable_fifo_rts_pin_ctl(Usart *p_usart)
{
	p_usart->US_FMR &= ~US_FMR_FRTSC;
}

/**
 * \brief Set USART FIFO Tx Threshold to raise a flag
 * when number of data in Tx FIFO becomes equal or lower.
 *
 * \param p_usart Pointer to a USART instance.
 * \param ul_threshold Threshold to set.
 */
static inline void usart_set_fifo_tx_threshold(Usart *p_usart, uint32_t ul_threshold)
{
	p_usart->US_FMR &= ~US_FMR_TXFTHRES_Msk;
	p_usart->US_FMR |= US_FMR_TXFTHRES(ul_threshold);
}

/**
 * \brief Set USART FIFO Rx Threshold to raise a flag
 * when number of data in Rx FIFO becomes equal or higher.
 *
 * \param p_usart Pointer to a USART instance.
 * \param ul_threshold Threshold to set.
 */
static inline void usart_set_fifo_rx_threshold(Usart *p_usart, uint32_t ul_threshold)
{
	p_usart->US_FMR &= ~US_FMR_RXFTHRES_Msk;
	p_usart->US_FMR |= US_FMR_RXFTHRES(ul_threshold);
}

/**
 * \brief Set USART FIFO Rx Second Threshold to raise a flag
 * when number of data in Rx FIFO becomes equal or higher.
 *
 * \param p_usart Pointer to a USART instance.
 * \param ul_threshold2 Threshold to set.
 */
static inline void usart_set_fifo_rx_threshold2(Usart *p_usart, uint32_t ul_threshold2)
{
	p_usart->US_FMR &= ~US_FMR_RXFTHRES2_Msk;
	p_usart->US_FMR |= US_FMR_RXFTHRES2(ul_threshold2);
}

/**
 * \brief Enable USART FIFO interrupts.
 *
 * \param p_usart Pointer to a USART peripheral.
 * \param ul_sources Interrupt sources bit map.
 */
static inline void usart_enable_fifo_interrupt(Usart *p_usart, uint32_t ul_sources)
{
	p_usart->US_FIER = ul_sources;
}

/**
 * \brief Disable USART FIFO interrupts.
 *
 * \param p_usart Pointer to a USART peripheral.
 * \param ul_sources Interrupt sources bit map.
 */
static inline void usart_disable_fifo_interrupt(Usart *p_usart, uint32_t ul_sources)
{
	p_usart->US_FIDR = ul_sources;
}

/**
 * \brief Read USART FIFO interrupt mask.
 *
 * \param p_usart Pointer to a USART peripheral.
 *
 * \return The interrupt mask value.
 */
static inline uint32_t usart_get_fifo_interrupt_mask(Usart *p_usart)
{
	return p_usart->US_FIMR;
}

/**
 * \brief Get current FIFO event status.
 *
 * \param p_usart Pointer to a USART instance.
 *
 * \return The current USART FIFO event status.
 */
static inline uint32_t usart_get_fifo_event_status(Usart *p_usart)
{
	return p_usart->US_FESR;
}

/// @cond 0
/**INDENT-OFF**/
#ifdef __cplusplus
}
#endif
/**INDENT-ON**/
/// @endcond

//! @}

/**
 * \page sam_usart_quickstart Quick start guide for the SAM USART module
 *
 * This is the quick start guide for the \ref group_sam_drivers_usart
 * "USART module", with step-by-step instructions on how to configure and
 * use the driver in a selection of use cases.
 *
 * The use cases contain several code fragments. The code fragments in the
 * steps for setup can be copied into a custom initialization function, while
 * the steps for usage can be copied into, e.g., the main application function.
 *
 * \note Some SAM devices contain both USART and UART modules, with the latter
 *       being a subset in functionality of the former but physically separate
 *       peripherals. UART modules are compatible with the USART driver, but
 *       only for the functions and modes supported by the base UART driver.
 *
 * \section usart_use_cases USART use cases
 * - \ref usart_basic_use_case
 * - \subpage usart_use_case_1
 * - \subpage usart_use_case_2
 *
 * \note The USART pins configuration are not included here. Please refer
 *       the related code in board_init() function.
 *
 * \section usart_basic_use_case Basic use case - transmit a character
 * In this use case, the USART module is configured for:
 * - Using USART0
 * - Baudrate: 9600
 * - Character length: 8 bit
 * - Parity mode: Disabled
 * - Stop bit: None
 * - RS232 mode
 *
 * \section usart_basic_use_case_setup Setup steps
 *
 * \subsection usart_basic_use_case_setup_prereq Prerequisites
 * -# \ref sysclk_group "System Clock Management (sysclock)"
 * -# \ref ioport_group "Common IOPORT API (ioport)"
 *
 * \subsection usart_basic_use_case_setup_code Example code
 * The following configuration must be added to the project (typically to a
 * conf_usart.h file, but it can also be added to your main application file.)
 * \code
	#define USART_SERIAL                 USART0
	#define USART_SERIAL_ID              ID_USART0  //USART0 for sam4l, ID_FLEXCOM0 for samg55 & pic32cx
	#define USART_SERIAL_BAUDRATE        9600
	#define USART_SERIAL_CHAR_LENGTH     US_MR_CHRL_8_BIT
	#define USART_SERIAL_PARITY          US_MR_PAR_NO
	#define USART_SERIAL_STOP_BIT        US_MR_NBSTOP_1_BIT
\endcode
 *
 * Add to application initialization:
 * \code
	    sysclk_init();

	    board_init();

	    const sam_usart_opt_t usart_console_settings = {
	        USART_SERIAL_BAUDRATE,
	        USART_SERIAL_CHAR_LENGTH,
	        USART_SERIAL_PARITY,
	        USART_SERIAL_STOP_BIT,
	        US_MR_CHMODE_NORMAL
	    };
    #if SAM4L
      sysclk_enable_peripheral_clock(USART_SERIAL);
    #else
	    sysclk_enable_peripheral_clock(USART_SERIAL_ID);
    #endif
	    usart_init_rs232(USART_SERIAL, &usart_console_settings,
	            sysclk_get_main_hz());
	    usart_enable_tx(USART_SERIAL);
	    usart_enable_rx(USART_SERIAL);
\endcode
 *
 * \subsection usart_basic_use_case_setup_flow Workflow
 * -# Initialize system clock:
 *   \code
	sysclk_init();
\endcode
 * -# Configure the USART Tx and Rx pins by call the board init function:
 *   \code
	board_init();
\endcode
 * \note Set the following define in conf_board.h file to enable COM port,it will be used in
 * board_init() function to set up IOPorts for the USART pins.
 * For SAM4L:
 *   \code
  #define CONF_BOARD_COM_PORT
\endcode
 * For other SAM devices:
 *   \code
  #define CONF_BOARD_UART_CONSOLE
\endcode
 * -# Create USART options struct:
 *   \code
	const sam_usart_opt_t usart_console_settings = {
	     USART_SERIAL_BAUDRATE,
	     USART_SERIAL_CHAR_LENGTH,
	     USART_SERIAL_PARITY,
	     USART_SERIAL_STOP_BIT,
	     US_MR_CHMODE_NORMAL
	};
\endcode
 * -# Enable the clock to the USART module:
 *   \code
	  #if SAM4L
      sysclk_enable_peripheral_clock(USART_SERIAL);
    #else
	    sysclk_enable_peripheral_clock(USART_SERIAL_ID);
    #endif
\endcode
 * -# Initialize the USART module in RS232 mode:
 *   \code
	usart_init_rs232(USART_SERIAL, &usart_console_settings,
	        sysclk_get_main_hz());
\endcode
 * -# Enable the Rx and Tx modes of the USART module:
 *   \code
	usart_enable_tx(USART_SERIAL);
	usart_enable_rx(USART_SERIAL);
\endcode
 *
 * \section usart_basic_use_case_usage Usage steps
 *
 * \subsection usart_basic_use_case_usage_code Example code
 * Add to application C-file:
 * \code
	usart_putchar(USART_SERIAL, 'a');
\endcode
 *
 * \subsection usart_basic_use_case_usage_flow Workflow
 * -# Send an 'a' character via USART
 *   \code usart_putchar(USART_SERIAL, 'a'); \endcode
 */

/**
 * \page usart_use_case_1 USART receive character and echo back
 *
 * In this use case, the USART module is configured for:
 * - Using USART0
 * - Baudrate: 9600
 * - Character length: 8 bit
 * - Parity mode: Disabled
 * - Stop bit: None
 * - RS232 mode
 *
 * The use case waits for a received character on the configured USART and
 * echoes the character back to the same USART.
 *
 * \section usart_use_case_1_setup Setup steps
 *
 * \subsection usart_use_case_1_setup_prereq Prerequisites
 * -# \ref sysclk_group "System Clock Management (sysclock)"
 * -# \ref ioport_group "Common IOPORT API (ioport)"
 *
 * \subsection usart_use_case_1_setup_code Example code
 * The following configuration must be added to the project (typically to a
 * conf_usart.h file, but it can also be added to your main application file.):
 * \code
	#define USART_SERIAL                 USART0
	#define USART_SERIAL_ID              ID_USART0  //USART0 for sam4l, ID_FLEXCOM0 for samg55 & pic32cx
	#define USART_SERIAL_BAUDRATE        9600
	#define USART_SERIAL_CHAR_LENGTH     US_MR_CHRL_8_BIT
	#define USART_SERIAL_PARITY          US_MR_PAR_NO
	#define USART_SERIAL_STOP_BIT        US_MR_NBSTOP_1_BIT
\endcode
 *
 * A variable for the received byte must be added:
 * \code
	uint32_t received_byte;
\endcode
 *
 * Add to application initialization:
 * \code
	    sysclk_init();

	    board_init();

	    const sam_usart_opt_t usart_console_settings = {
	        USART_SERIAL_BAUDRATE,
	        USART_SERIAL_CHAR_LENGTH,
	        USART_SERIAL_PARITY,
	        USART_SERIAL_STOP_BIT,
	        US_MR_CHMODE_NORMAL
	    };

    #if SAM4L
      sysclk_enable_peripheral_clock(USART_SERIAL);
    #else
	    sysclk_enable_peripheral_clock(USART_SERIAL_ID);
    #endif

	    usart_init_rs232(USART_SERIAL, &usart_console_settings,
	            sysclk_get_main_hz());
	    usart_enable_tx(USART_SERIAL);
	    usart_enable_rx(USART_SERIAL);
\endcode
 *
 * \subsection usart_use_case_1_setup_flow Workflow
 * -# Initialize system clock:
 *   \code
	sysclk_init();
\endcode
 * -# Configure the USART Tx and Rx pins  by call the board init function:
 *   \code
	board_init();
\endcode
 * \note Set the following define in conf_board.h file to enable COM port,it will be used in
 * board_init() function to set up IOPorts for the USART pins.
 * For SAM4L:
 *   \code
  #define CONF_BOARD_COM_PORT
\endcode
 * For other SAM devices:
 *   \code
  #define CONF_BOARD_UART_CONSOLE
\endcode
 * -# Create USART options struct:
 *   \code
	const sam_usart_opt_t usart_console_settings = {
	     USART_SERIAL_BAUDRATE,
	     USART_SERIAL_CHAR_LENGTH,
	     USART_SERIAL_PARITY,
	     USART_SERIAL_STOP_BIT,
	     US_MR_CHMODE_NORMAL
	};
\endcode
 * -# Enable the clock to the USART module:
 *   \code
    #if SAM4L
      sysclk_enable_peripheral_clock(USART_SERIAL);
    #else
	    sysclk_enable_peripheral_clock(USART_SERIAL_ID);
    #endif
\endcode
 * -# Initialize the USART module in RS232 mode:
 *   \code
	usart_init_rs232(USART_SERIAL, &usart_console_settings,
	        sysclk_get_main_hz());
\endcode
 * -# Enable the Rx and Tx modes of the USART module:
 *   \code
	usart_enable_tx(USART_SERIAL);
	usart_enable_rx(USART_SERIAL);
\endcode
 *
 * \section usart_use_case_1_usage Usage steps
 *
 * \subsection usart_use_case_1_usage_code Example code
 * Add to, e.g., main loop in application C-file:
 * \code
	received_byte = usart_getchar(USART_SERIAL);
	usart_putchar(USART_SERIAL, received_byte);
\endcode
 *
 * \subsection usart_use_case_1_usage_flow Workflow
 * -# Wait for reception of a character:
 *   \code usart_getchar(USART_SERIAL, &received_byte); \endcode
 * -# Echo the character back:
 *   \code usart_putchar(USART_SERIAL, received_byte); \endcode
 */

/**
 * \page usart_use_case_2 USART receive character and echo back via interrupts
 *
 * In this use case, the USART module is configured for:
 * - Using USART0
 * - Baudrate: 9600
 * - Character length: 8 bit
 * - Parity mode: Disabled
 * - Stop bit: None
 * - RS232 mode
 *
 * The use case waits for a received character on the configured USART and
 * echoes the character back to the same USART. The character reception is
 * performed via an interrupt handler, rather than the polling method used
 * in \ref usart_use_case_1.
 *
 * \section usart_use_case_2_setup Setup steps
 *
 * \subsection usart_use_case_2_setup_prereq Prerequisites
 * -# \ref sysclk_group "System Clock Management (sysclock)"
 * -# \ref pio_group "Parallel Input/Output Controller (pio)"
 * -# \ref pmc_group "Power Management Controller (pmc)"
 *
 * \subsection usart_use_case_2_setup_code Example code
 * The following configuration must be added to the project (typically to a
 * conf_usart.h file, but it can also be added to your main application file.):
 * \code
	#define USART_SERIAL                 USART0
	#define USART_SERIAL_ID              ID_USART0  //USART0 for sam4l, ID_FLEXCOM0 for samg55 & pic32cx
	#define USART_SERIAL_ISR_HANDLER     USART0_Handler, FLEXCOM0_Handler for samg55 & pic32cx
	#define USART_SERIAL_BAUDRATE        9600
	#define USART_SERIAL_CHAR_LENGTH     US_MR_CHRL_8_BIT
	#define USART_SERIAL_PARITY          US_MR_PAR_NO
	#define USART_SERIAL_STOP_BIT        US_MR_NBSTOP_1_BIT
\endcode
 *
 * A variable for the received byte must be added:
 * \code
	uint32_t received_byte;
\endcode
 *
 * Add to application initialization:
 * \code
	    sysclk_init();

	    board_init();

	    const sam_usart_opt_t usart_console_settings = {
	        USART_SERIAL_BAUDRATE,
	        USART_SERIAL_CHAR_LENGTH,
	        USART_SERIAL_PARITY,
	        USART_SERIAL_STOP_BIT,
	        US_MR_CHMODE_NORMAL
	    };

    #if SAM4L
      sysclk_enable_peripheral_clock(USART_SERIAL);
    #else
	    sysclk_enable_peripheral_clock(USART_SERIAL_ID);
    #endif

	    usart_init_rs232(USART_SERIAL, &usart_console_settings,
	            sysclk_get_main_hz());
	    usart_enable_tx(USART_SERIAL);
	    usart_enable_rx(USART_SERIAL);

	    usart_enable_interrupt(USART_SERIAL, US_IER_RXRDY);
	    NVIC_EnableIRQ(USART_SERIAL_IRQ);
\endcode
 *
 * \subsection usart_use_case_2_setup_flow Workflow
 * -# Initialize system clock:
 *   \code
	sysclk_init();
\endcode
 * -# Configure the USART Tx and Rx pins  by call the board init function:
 *   \code
	board_init();
\endcode
 * \note Set the following define in conf_board.h file to enable COM port,it will be used in
 * board_init() function to set up IOPorts for the USART pins.
 * For SAM4L:
 *   \code
  #define CONF_BOARD_COM_PORT
\endcode
 * For other SAM devices:
 *   \code
  #define CONF_BOARD_UART_CONSOLE
\endcode
 * -# Create USART options struct:
 *   \code
	const sam_usart_opt_t usart_console_settings = {
	     USART_SERIAL_BAUDRATE,
	     USART_SERIAL_CHAR_LENGTH,
	     USART_SERIAL_PARITY,
	     USART_SERIAL_STOP_BIT,
	     US_MR_CHMODE_NORMAL
	};
\endcode
 * -# Enable the clock to the USART module:
 *   \code
    #if SAM4L
      sysclk_enable_peripheral_clock(USART_SERIAL);
    #else
	    sysclk_enable_peripheral_clock(USART_SERIAL_ID);
    #endif
\endcode
 * -# Initialize the USART module in RS232 mode:
 *   \code
	usart_init_rs232(USART_SERIAL, &usart_console_settings,
	        sysclk_get_main_hz());
\endcode
 * -# Enable the Rx and Tx modes of the USART module:
 *   \code
	usart_enable_tx(USART_SERIAL);
	usart_enable_rx(USART_SERIAL);
\endcode
 * -# Enable the USART character reception interrupt, and general interrupts
 *   for the USART module.
 *   \code
	usart_enable_interrupt(USART_SERIAL, US_IER_RXRDY);
	NVIC_EnableIRQ(USART_SERIAL_IRQ);
\endcode
 * \section usart_use_case_2_usage Usage steps
 *
 * \subsection usart_use_case_2_usage_code Example code
 * Add to your main application C-file the USART interrupt handler:
 * \code
	 void USART_SERIAL_ISR_HANDLER(void)
	 {
	    uint32_t dw_status = usart_get_status(USART_SERIAL);

	    if (dw_status & US_CSR_RXRDY) {
	        uint32_t received_byte;

	        usart_read(USART_SERIAL, &received_byte);
	        usart_write(USART_SERIAL, received_byte);
	    }
	 }
\endcode
 *
 * \subsection usart_use_case_2_usage_flow Workflow
 * -# When the USART ISR fires, retrieve the USART module interrupt flags:
 *   \code uint32_t dw_status = usart_get_status(USART_SERIAL); \endcode
 * -# Check if the USART Receive Character interrupt has fired:
 *   \code if (dw_status & US_CSR_RXRDY) \endcode
 * -# If a character has been received, fetch it into a temporary variable:
 *   \code usart_read(USART_SERIAL, &received_byte); \endcode
 * -# Echo the character back:
 *   \code usart_write(USART_SERIAL, received_byte); \endcode
 */

#endif /* USART_H_INCLUDED */
