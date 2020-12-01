/**
 * \file
 *
 * \brief Universal Asynchronous Receiver Transceiver (UART) driver for SAM.
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

#include "uart.h"

/* / @cond 0 */
/**INDENT-OFF**/
#ifdef __cplusplus
extern "C" {
#endif
/**INDENT-ON**/
/* / @endcond */

/**
 * \defgroup sam_drivers_uart_group Universal Asynchronous Receiver Transceiver (UART)
 *
 * The Universal Asynchronous Receiver Transmitter features a two-pin UART that
 * can be used for communication and trace purposes and offers an ideal medium
 * for in-situ programming solutions. Moreover, the association with two
 * peripheral DMA controller (PDC) channels permits packet handling for these
 * tasks with processor time reduced to a minimum.
 *
 * \par Usage
 *
 * -# Enable the UART peripheral clock in the PMC.
 * -# Enable the required UART PIOs (see pio.h).
 * -# Configure the UART by calling uart_init.
 * -# Send data through the UART using the uart_write.
 * -# Receive data from the UART using the uart_read; the availability of data
 *    can be polled with uart_is_rx_ready.
 * -# Disable the transmitter and/or the receiver of the UART with
 *    uart_disable_tx and uart_disable_rx.
 *
 * @{
 */

/**
 * \brief Configure UART with the specified parameters.
 *
 * \note The PMC and PIOs must be configured first.
 *
 * \param p_uart Pointer to a UART instance.
 * \param p_uart_opt Pointer to sam_uart_opt_t instance.
 *
 * \retval 0 Success.
 * \retval 1 Bad baud rate generator value.
 */
uint32_t uart_init(Uart *p_uart, const sam_uart_opt_t *p_uart_opt)
{
	uint32_t cd = 0;

	/* Reset and disable receiver & transmitter */
	p_uart->UART_CR = UART_CR_RSTRX | UART_CR_RSTTX
			| UART_CR_RXDIS | UART_CR_TXDIS;

	/* Check and configure baudrate */
	/* Asynchronous, no oversampling */
	cd = (p_uart_opt->ul_mck / p_uart_opt->ul_baudrate) / UART_MCK_DIV;
	if (cd < UART_MCK_DIV_MIN_FACTOR || cd > UART_MCK_DIV_MAX_FACTOR) {
		return 1;
	}

	p_uart->UART_BRGR = cd;
	/* Configure mode */
	p_uart->UART_MR = p_uart_opt->ul_mode;

	/* Disable PDC channel */
	p_uart->UART_PTCR = UART_PTCR_RXTDIS | UART_PTCR_TXTDIS;

	/* Enable receiver and transmitter */
	p_uart->UART_CR = UART_CR_RXEN | UART_CR_TXEN;

	return 0;
}

/**
 * \brief Enable UART transmitter.
 *
 * \param p_uart Pointer to a UART instance.
 */
void uart_enable_tx(Uart *p_uart)
{
	/* Enable transmitter */
	p_uart->UART_CR = UART_CR_TXEN;
}

/**
 * \brief Disable UART transmitter.
 *
 * \param p_uart Pointer to a UART instance.
 */
void uart_disable_tx(Uart *p_uart)
{
	/* Disable transmitter */
	p_uart->UART_CR = UART_CR_TXDIS;
}

/**
 * \brief Reset UART transmitter.
 *
 * \param p_uart Pointer to a UART instance.
 */
void uart_reset_tx(Uart *p_uart)
{
	/* Reset transmitter */
	p_uart->UART_CR = UART_CR_RSTTX | UART_CR_TXDIS;
}

/**
 * \brief Enable UART receiver.
 *
 * \param p_uart Pointer to a UART instance.
 */
void uart_enable_rx(Uart *p_uart)
{
	/* Enable receiver */
	p_uart->UART_CR = UART_CR_RXEN;
}

/**
 * \brief Disable UART receiver.
 *
 * \param p_uart Pointer to a UART instance.
 */
void uart_disable_rx(Uart *p_uart)
{
	/* Disable receiver */
	p_uart->UART_CR = UART_CR_RXDIS;
}

/**
 * \brief Reset UART receiver.
 *
 * \param p_uart Pointer to a UART instance.
 */
void uart_reset_rx(Uart *p_uart)
{
	/* Reset receiver */
	p_uart->UART_CR = UART_CR_RSTRX | UART_CR_RXDIS;
}

/**
 * \brief Enable UART receiver and transmitter.
 *
 * \param p_uart Pointer to a UART instance.
 */
void uart_enable(Uart *p_uart)
{
	/* Enable receiver and transmitter */
	p_uart->UART_CR = UART_CR_RXEN | UART_CR_TXEN;
}

/**
 * \brief Disable UART receiver and transmitter.
 *
 * \param p_uart Pointer to a UART instance.
 */
void uart_disable(Uart *p_uart)
{
	/* Disable receiver and transmitter */
	p_uart->UART_CR = UART_CR_RXDIS | UART_CR_TXDIS;
}

/**
 * \brief Reset UART receiver and transmitter.
 *
 * \param p_uart Pointer to a UART instance.
 */
void uart_reset(Uart *p_uart)
{
	/* Reset and disable receiver & transmitter */
	p_uart->UART_CR = UART_CR_RSTRX | UART_CR_RSTTX
			| UART_CR_RXDIS | UART_CR_TXDIS;
}

/** \brief Enable UART interrupts.
 *
 * \param p_uart Pointer to a UART instance.
 *  \param ul_sources Interrupts to be enabled.
 */
void uart_enable_interrupt(Uart *p_uart, uint32_t ul_sources)
{
	p_uart->UART_IER = ul_sources;
}

/** \brief Disable UART interrupts.
 *
 * \param p_uart Pointer to a UART instance.
 *  \param ul_sources Interrupts to be disabled.
 */
void uart_disable_interrupt(Uart *p_uart, uint32_t ul_sources)
{
	p_uart->UART_IDR = ul_sources;
}

/** \brief Read UART interrupt mask.
 *
 * \param p_uart Pointer to a UART instance.
 *
 *  \return The interrupt mask value.
 */
uint32_t uart_get_interrupt_mask(Uart *p_uart)
{
	return p_uart->UART_IMR;
}

/**
 * \brief Get current status.
 *
 * \param p_uart Pointer to a UART instance.
 *
 * \return The current UART status.
 */
uint32_t uart_get_status(Uart *p_uart)
{
	return p_uart->UART_SR;
}

/**
 * \brief Reset status bits.
 *
 * \param p_uart Pointer to a UART instance.
 */
void uart_reset_status(Uart *p_uart)
{
	p_uart->UART_CR = UART_CR_RSTSTA;
}

/**
 * \brief Check if Transmit is Ready.
 * Check if data has been loaded in UART_THR and is waiting to be loaded in the
 * Transmit Shift Register (TSR).
 *
 * \param p_uart Pointer to a UART instance.
 *
 * \retval 1 Data has been transmitted.
 * \retval 0 Transmit is not ready, data pending.
 */
uint32_t uart_is_tx_ready(Uart *p_uart)
{
	return (p_uart->UART_SR & UART_SR_TXRDY) > 0;
}

/**
 * \brief Check if Transmit Hold Register is empty.
 * Check if the last data written in UART_THR has been loaded in TSR and the
 * last data loaded in TSR has been transmitted.
 *
 * \param p_uart Pointer to a UART instance.
 *
 * \retval 1 Transmitter is empty.
 * \retval 0 Transmitter is not empty.
 */
uint32_t uart_is_tx_empty(Uart *p_uart)
{
	return (p_uart->UART_SR & UART_SR_TXEMPTY) > 0;
}

/**
 * \brief Check if Received data is ready.
 * Check if data has been received and loaded in UART_RHR.
 *
 * \param p_uart Pointer to a UART instance.
 *
 * \retval 1 One data has been received.
 * \retval 0 No data has been received.
 */
uint32_t uart_is_rx_ready(Uart *p_uart)
{
	return (p_uart->UART_SR & UART_SR_RXRDY) > 0;
}

/**
 * \brief Check if both transmit buffers are sent out.
 *
 * \param p_uart Pointer to a UART instance.
 *
 * \retval 1 Transmit buffer is empty.
 * \retval 0 Transmit buffer is not empty.
 */
uint32_t uart_is_tx_buf_empty(Uart *p_uart)
{
	return (p_uart->UART_SR & UART_SR_TXBUFE) > 0;
}

/**
 * \brief Set UART clock divisor value
 *
 * \param p_uart Pointer to a UART instance.
 * \param us_divisor Value to be set.
 *
 */
void uart_set_clock_divisor(Uart *p_uart, uint16_t us_divisor)
{
	p_uart->UART_BRGR = us_divisor;
}

/**
 * \brief Write to UART Transmit Holding Register
 * Before writing user should check if tx is ready (or empty).
 *
 * \param p_uart Pointer to a UART instance.
 * \param data Data to be sent.
 *
 * \retval 0 Success.
 * \retval 1 I/O Failure, UART is not ready.
 */
uint32_t uart_write(Uart *p_uart, const uint8_t uc_data)
{
	/* Check if the transmitter is ready */
	if (!(p_uart->UART_SR & UART_SR_TXRDY)) {
		return 1;
	}

	/* Send character */
	p_uart->UART_THR = uc_data;
	return 0;
}

/**
 * \brief Read from UART Receive Holding Register.
 * Before reading user should check if rx is ready.
 *
 * \param p_uart Pointer to a UART instance.
 *
 * \retval 0 Success.
 * \retval 1 I/O Failure, UART is not ready.
 */
uint32_t uart_read(Uart *p_uart, uint8_t *puc_data)
{
	/* Check if the receiver is ready */
	if ((p_uart->UART_SR & UART_SR_RXRDY) == 0) {
		return 1;
	}

	/* Read character */
	*puc_data = (uint8_t)p_uart->UART_RHR;
	return 0;
}

/**
 * \brief Check if one receive buffer is filled.
 *
 * \param p_uart Pointer to a UART instance.
 *
 * \retval 1 Receive is completed.
 * \retval 0 Receive is still pending.
 */
uint32_t uart_is_rx_buf_end(Uart *p_uart)
{
	return (p_uart->UART_SR & UART_SR_ENDRX) > 0;
}

/**
 * \brief Check if one transmit buffer is sent out.
 *
 * \param p_uart Pointer to a UART instance.
 *
 * \retval 1 Transmit is completed.
 * \retval 0 Transmit is still pending.
 */
uint32_t uart_is_tx_buf_end(Uart *p_uart)
{
	return (p_uart->UART_SR & UART_SR_ENDTX) > 0;
}

/**
 * \brief Check if both receive buffers are full.
 *
 * \param p_uart Pointer to a UART instance.
 *
 * \retval 1 Receive buffers are full.
 * \retval 0 Receive buffers are not full.
 */
uint32_t uart_is_rx_buf_full(Uart *p_uart)
{
	return (p_uart->UART_SR & UART_SR_RXBUFF) > 0;
}

/**
 * \brief Get UART PDC base address.
 *
 * \param p_uart Pointer to a UART instance.
 *
 * \return UART PDC registers base for PDC driver to access.
 */
Pdc *uart_get_pdc_base(Uart *p_uart)
{
	Pdc *p_pdc_base;

	if (p_uart == UART) {
		p_pdc_base = PDC_UART;
	}

	return p_pdc_base;
}

/**
 * \brief Enable UART optical interface.
 *
 * \param p_uart Pointer to a UART instance.
 */
void uart_enable_optical_interface(Uart *p_uart)
{

	Assert(p_uart == UART);
	p_uart->UART_MR |= UART_MR_OPT_EN;
}

/**
 * \brief Disable UART optical interface.
 *
 * \param p_uart Pointer to a UART instance.
 */
void uart_disable_optical_interface(Uart *p_uart)
{
	Assert(p_uart == UART);
	p_uart->UART_MR &= ~UART_MR_OPT_EN;
}

/**
 * \brief Configure UART optical interface.
 *
 * \param p_uart Pointer to a UART instance.
 * \param cfg Pointer to a UART optical interface configuration.
 */
void uart_config_optical_interface(Uart *p_uart,
		struct uart_config_optical *cfg)
{
	Assert(p_uart == UART);
	uint32_t reg = p_uart->UART_MR;

	reg &= ~(UART_MR_OPT_RXINV | UART_MR_OPT_MDINV | UART_MR_OPT_DMOD
			| UART_MR_FILTER | UART_MR_OPT_WKUP
			| UART_MR_OPT_CLKDIV_Msk | UART_MR_OPT_DUTY_Msk
			| UART_MR_OPT_CMPTH_Msk);
	reg |= (cfg->rx_inverted ? UART_MR_OPT_RXINV : 0)
			| (cfg->md_inverted ? UART_MR_OPT_MDINV : 0)
			| (cfg->demod_enable ? UART_MR_OPT_DMOD : 0)
			| (cfg->rx_filter ? UART_MR_FILTER : 0)
			| (cfg->wu_enable ? UART_MR_OPT_WKUP : 0)
			| UART_MR_OPT_CLKDIV(cfg->clk_div)
			| cfg->duty | cfg->threshold;

	p_uart->UART_MR = reg;
}

/**
 * \brief Set UART optical interface to Sleep Mode.
 *
 * \param p_uart Pointer to a UART instance.
 */
void uart_optical_interface_set_sleep(Uart *p_uart)
{
	Assert(p_uart == UART);
	p_uart->UART_CR |= UART_CR_OPT_SLEEP;
}

/**
 * \brief Clear UART optical interface Sleep Mode.
 *
 * \param p_uart Pointer to a UART instance.
 */
void uart_optical_interface_clear_sleep(Uart *p_uart)
{
	Assert(p_uart == UART);
	p_uart->UART_CR &= ~UART_CR_OPT_SLEEP;
}

/**
 * \brief Enables/Disables write protection mode.
 *
 * \param p_uart Pointer to a UART instance.
 * \param flag ture for enable, false for disable.
 */
void uart_set_write_protection(Uart *p_uart, bool flag)
{
	if (flag) {
		p_uart->UART_WPMR = UART_WPMR_WPKEY_PASSWD | UART_WPMR_WPEN;
	} else {
		p_uart->UART_WPMR = UART_WPMR_WPKEY_PASSWD;
	}
}

/**
 * \brief Disable UART Rx Timeout.
 *
 * \param p_uart Pointer to a UART instance.
 */
void uart_disable_rx_timeout(Uart *p_uart)
{
	Assert(p_uart == UART);
	/* Set '0' on Timeout Register */
	p_uart->UART_RTOR = UART_RTOR_TO(0);
}

/**
 * \brief Enable UART Rx Timeout with continuous timer rearm.
 *        This function must be called to enable timeout and
 *        after each timeout detection.
 *
 * \param p_uart Pointer to a UART instance.
 * \param uc_bit_time_periods Number of Bit Time Periods to wait for Timeout.
 */
void uart_enable_rx_timeout_continuous_rearm(Uart *p_uart,
		uint8_t uc_bit_time_periods)
{
	Assert(p_uart == UART);
	/* Set value on Timeout Register */
	p_uart->UART_RTOR = UART_RTOR_TO(uc_bit_time_periods);
	/* Set Timeout rearm mode */
	p_uart->UART_CR |= (UART_CR_STTTO | UART_CR_RETTO);
}

/**
 * \brief Enable UART Rx Timeout waiting for new char before timer rearm.
 *        This function must be called to enable timeout and
 *        after each timeout detection.
 *
 * \param p_uart Pointer to a UART instance.
 * \param uc_bit_time_periods Number of Bit Time Periods to wait for Timeout.
 */
void uart_enable_rx_timeout_wait_for_char_before_rearm(Uart *p_uart,
		uint8_t uc_bit_time_periods)
{
	Assert(p_uart == UART);
	/* Set value on Timeout Register */
	p_uart->UART_RTOR = UART_RTOR_TO(uc_bit_time_periods);
	/* Set Timeout wait for char mode */
	p_uart->UART_CR |= UART_CR_STTTO;
}

/**
 * \brief Set UART Comparison values.
 *        If VAL1 == VAL2, CMP flag is set if the received character equals VAL1.
 *        If VAL1 < VAL2, any value between VAL1 and VAL2 sets the CMP flag.
 *        If VAL1 > VAL2, CMP flag is set if either received character equals VAL1 or VAL2.
 *
 * \param p_uart Pointer to a UART instance.
 * \param uc_val1 Comparison Value 1. See functional description above.
 * \param uc_val2 Comparison Value 2. See functional description above.
 * \param cmpmode true for start rx only upon condition met, false for CMP flag set only.
 * \param cmppar true for parity check, false for no check.
 */
void uart_set_char_comparison_condition(Uart *p_uart,
		uint8_t uc_val1, uint8_t uc_val2,
		bool cmpmode, bool cmppar)
{
	Assert(p_uart == UART);
	
	uint32_t temp = 0;

	if (cmpmode) {
		temp |= UART_CMPR_CMPMODE_START_CONDITION;
	}

	if (cmppar) {
		temp |= UART_CMPR_CMPPAR;
	}

	temp |= UART_CMPR_VAL1((uint32_t)uc_val1);

	temp |= UART_CMPR_VAL2((uint32_t)uc_val2);

	p_uart->UART_CMPR = temp;
}

/**
 * \brief Check if Comparison flag is set indicating a matching condition.
 *
 * \param p_uart Pointer to a UART instance.
 *
 * \retval 1 Comparison condition already met.
 * \retval 0 Comparison condition not yet met.
 */
uint32_t uart_is_char_comparison_fulfilled(Uart *p_uart)
{
	return (p_uart->UART_SR & UART_SR_CMP) > 0;
}

/**
 * \brief Restarts Comparison Condition trigger.
 *
 * \param p_uart Pointer to a UART instance.
 */
void uart_restart_char_comparison_trigger(Uart *p_uart)
{
	Assert(p_uart == UART);
	/* Set ReqClear flag */
	p_uart->UART_CR |= UART_CR_REQCLR;
}

/**
 * \brief Enable UART analog comparator.
 *
 * \param p_uart Pointer to a UART instance.
 */
void uart_enable_analog_comparator(Uart *p_uart)
{
	Assert(p_uart == UART);
	p_uart->UART_MR |= UART_MR_ACON;
}

/**
 * \brief Disable UART analog comparator.
 *
 * \param p_uart Pointer to a UART instance.
 */
void uart_disable_analog_comparator(Uart *p_uart)
{
	Assert(p_uart == UART);
	p_uart->UART_MR &= ~UART_MR_ACON;
}

/**
 * \brief Configure UART analog comparator edge selection.
 *
 * \param p_uart Pointer to a UART instance.
 * \param cmp_edge Comparator Edge option.
 */
void uart_config_analog_comparator_edge(Uart *p_uart,
		enum uart_analog_cmp_edge cmp_edge)
{
	Assert(p_uart == UART);
	uint32_t reg = p_uart->UART_MR;

	reg &= ~UART_MR_EDGESEL_Msk;
	reg |= cmp_edge;

	p_uart->UART_MR = reg;
}

/**
 * \brief Configure UART Baudrate Source Clock.
 *
 * \param p_uart Pointer to a UART instance.
 * \param br_src_clk Baudrate Source Clk selection.
 */
void uart_config_baudrate_source_clk(Uart *p_uart,
		enum uart_baudrate_source_clk br_src_clk)
{
	Assert(p_uart == UART);
	uint32_t reg = p_uart->UART_MR;

	reg &= ~UART_MR_BRSRCCK;
	reg |= br_src_clk;

	p_uart->UART_MR = reg;
}

/**
 * \brief Check if a Timeout in UART reception has occurred.
 *
 * \param p_uart Pointer to a UART instance.
 *
 * \retval 1 Timeout detected.
 * \retval 0 No timeout.
 */
uint32_t uart_rx_timeout_detected(Uart *p_uart)
{
	return (p_uart->UART_SR & UART_SR_TIMEOUT) > 0;
}

/**
 * \brief Check if an Analog Comparator Event has occurred.
 *
 * \param p_uart Pointer to a UART instance.
 *
 * \retval 1 Analog Comparator Event detected.
 * \retval 0 No Analog Comparator Event.
 */
uint32_t uart_analog_comp_event(Uart *p_uart)
{
	return (p_uart->UART_SR & UART_SR_ACE) > 0;
}

/**
 * \brief Get current Analog Comparator Output value.
 *
 * \param p_uart Pointer to a UART instance.
 *
 * \retval 1 Analog Comparator Event detected.
 * \retval 0 No Analog Comparator Event.
 */
uint32_t uart_get_analog_comp_output(Uart *p_uart)
{
	return (p_uart->UART_CSR & UART_CSR_ACO);
}

/* @} */

/* / @cond 0 */
/**INDENT-OFF**/
#ifdef __cplusplus
}
#endif
/**INDENT-ON**/
/* / @endcond */
