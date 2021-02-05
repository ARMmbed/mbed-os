/**
 * \file
 *
 * \brief Two-Wire Interface (TWI) driver for SAM.
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

#ifndef TWI_H_INCLUDED
#define TWI_H_INCLUDED

#include "compiler.h"

/// @cond 0
/**INDENT-OFF**/
#ifdef __cplusplus
extern "C" {
#endif
/**INDENT-ON**/
/// @endcond

/** Time-out value (number of attempts). */
#define TWI_TIMEOUT              30000

/**
 * \brief Return codes for TWI APIs.
 * @{
 */
#define TWI_SUCCESS              0
#define TWI_INVALID_ARGUMENT     1
#define TWI_ARBITRATION_LOST     2
#define TWI_NO_CHIP_FOUND        3
#define TWI_RECEIVE_OVERRUN      4
#define TWI_RECEIVE_NACK         5
#define TWI_SEND_OVERRUN         6
#define TWI_SEND_NACK            7
#define TWI_BUSY                 8
#define TWI_ERROR_TIMEOUT        9
/**
 * @}
 */

/**
 * \brief Input parameters when initializing the TWI module mode.
 */
typedef struct twi_options {
	//! MCK for TWI.
	uint32_t master_clk;
	//! The baud rate of the TWI bus.
	uint32_t speed;
	//! The desired address.
	uint8_t chip;
	//! SMBUS mode (set 1 to use SMBUS quick command, otherwise don't).
	uint8_t smbus;
} twi_options_t;

/**
 * \brief Information concerning the data transmission.
 */
typedef struct twi_packet {
	//! TWI address/commands to issue to the other chip (node).
	uint8_t addr[3];
	//! Length of the TWI data address segment (1-3 bytes).
	uint32_t addr_length;
	//! Where to find the data to be transferred.
	void *buffer;
	//! How many bytes do we want to transfer.
	uint32_t length;
	//! TWI chip address to communicate with.
	uint8_t chip;
} twi_packet_t;

enum twi_source_clock {
	TWI_SOURCE_PERIPH_CLK = TWI_CWGR_BRSRCCLK_PERIPH_CLK,
	TWI_SOURCE_GCLK = TWI_CWGR_BRSRCCLK_GCLK,
};

enum twi_tx_ready_flag_mode {
	TWI_TX_READY_FLAG_ONE_DATA = TWI_FMR_TXRDYM_ONE_DATA,
	TWI_TX_READY_FLAG_TWO_DATA = TWI_FMR_TXRDYM_TWO_DATA,
	TWI_TX_READY_FLAG_FOUR_DATA = TWI_FMR_TXRDYM_TWO_DATA,
};

enum twi_rx_ready_flag_mode {
	TWI_RX_READY_FLAG_ONE_DATA = TWI_FMR_RXRDYM_ONE_DATA,
	TWI_RX_READY_FLAG_TWO_DATA = TWI_FMR_RXRDYM_TWO_DATA,
	TWI_RX_READY_FLAG_FOUR_DATA = TWI_FMR_RXRDYM_FOUR_DATA,
};

void twi_enable_master_mode(Twi *p_twi);
void twi_disable_master_mode(Twi *p_twi);
uint32_t twi_master_init(Twi *p_twi, const twi_options_t *p_opt);
uint32_t twi_set_speed(Twi *p_twi, uint32_t ul_speed, uint32_t ul_mck);
uint32_t twi_probe(Twi *p_twi, uint8_t uc_slave_addr);
uint32_t twi_master_read(Twi *p_twi, twi_packet_t *p_packet);
uint32_t twi_master_write(Twi *p_twi, twi_packet_t *p_packet);
void twi_enable_interrupt(Twi *p_twi, uint32_t ul_sources);
void twi_disable_interrupt(Twi *p_twi, uint32_t ul_sources);
uint32_t twi_get_interrupt_status(Twi *p_twi);
uint32_t twi_get_interrupt_mask(Twi *p_twi);
uint8_t twi_read_byte(Twi *p_twi);
void twi_write_byte(Twi *p_twi, uint8_t uc_byte);
void twi_enable_slave_mode(Twi *p_twi);
void twi_disable_slave_mode(Twi *p_twi);
void twi_slave_init(Twi *p_twi, uint32_t ul_device_addr);
void twi_set_slave_addr(Twi *p_twi, uint32_t ul_device_addr);
uint32_t twi_slave_read(Twi *p_twi, uint8_t *p_data);
uint32_t twi_slave_write(Twi *p_twi, uint8_t *p_data);
void twi_reset(Twi *p_twi);
uint32_t twi_mk_addr(const uint8_t *addr, int len);
Pdc *twi_get_pdc_base(Twi *p_twi);
void twi_smbus_set_timing(Twi *p_twi, uint32_t ul_timing);
void twi_set_alternative_command(Twi *p_twi, uint32_t ul_alt_cmd);
void twi_set_filter(Twi *p_twi, uint32_t ul_filter);
void twi_mask_slave_addr(Twi *p_twi, uint32_t ul_mask);
void twi_set_sleepwalking(Twi *p_twi,
		uint32_t ul_matching_addr1, bool flag1,
		uint32_t ul_matching_addr2, bool flag2,
		uint32_t ul_matching_addr3, bool flag3,
		uint32_t ul_matching_data, bool flag);

/**
 * \brief Enable high speed mode.
 *
 * \param p_twi   Base address of the TWI instance.
 */
static inline void twi_enable_highspeed(Twi *p_twi)
{
	p_twi->TWI_CR = TWI_CR_HSEN;
}

/**
 * \brief Disable high speed mode.
 *
 * \param p_twi   Base address of the TWI instance.
 */
static inline void twi_disable_highspeed(Twi *p_twi)
{
	p_twi->TWI_CR = TWI_CR_HSDIS;
}

/**
 * \brief Enable SMBus mode.
 *
 * \param p_twi   Base address of the TWI instance.
 */
static inline void twi_enable_smbus(Twi *p_twi)
{
	p_twi->TWI_CR = TWI_CR_SMBEN;
}

/**
 * \brief Disable SMBus mode.
 *
 * \param p_twi   Base address of the TWI instance.
 */
static inline void twi_disable_smbus(Twi *p_twi)
{
	p_twi->TWI_CR = TWI_CR_SMBDIS;
}

/**
 * \brief Enable packet error checking.
 *
 * \param p_twi   Base address of the TWI instance.
 */
static inline void twi_enable_pec(Twi *p_twi)
{
	p_twi->TWI_CR = TWI_CR_PECEN;
}

/**
 * \brief Disable packet error checking.
 *
 * \param p_twi   Base address of the TWI instance.
 */
static inline void twi_disable_pec(Twi *p_twi)
{
	p_twi->TWI_CR = TWI_CR_PECDIS;
}

/**
 * \brief Request a packet error checking.
 *
 * \param p_twi   Base address of the TWI instance.
 */
static inline void twi_request_pec(Twi *p_twi)
{
	p_twi->TWI_CR = TWI_CR_PECRQ;
}

/**
 * \brief If master mode is enabled, send a bus clear command.
 *
 * \param p_twi   Base address of the TWI instance.
 */
static inline void twi_send_clear(Twi *p_twi)
{
	p_twi->TWI_CR = TWI_CR_CLEAR;
}

/**
 * \brief Enable alternative command mode.
 *
 * \param p_twi   Base address of the TWI instance.
 */
static inline void twi_enable_alternative_command(Twi *p_twi)
{
	p_twi->TWI_CR = TWI_CR_ACMEN;
}

/**
 * \brief Enable alternative command mode.
 *
 * \param p_twi   Base address of the TWI instance.
 */
static inline void twi_disable_alternative_command(Twi *p_twi)
{
	p_twi->TWI_CR = TWI_CR_ACMDIS;
}

/**
 * \brief Clear the Transmit Holding Register and set TXRDY, TXCOMP flags.
 *
 * \param p_twi   Base address of the TWI instance.
 */
static inline void twi_thr_clear(Twi *p_twi)
{
	p_twi->TWI_CR = TWI_CR_THRCLR;
}

/**
 * \brief Clear the TWI FSM lock.
 *
 * \param p_twi   Base address of the TWI instance.
 */
static inline void twi_lock_clear(Twi *p_twi)
{
	p_twi->TWI_CR = TWI_CR_LOCKCLR;
}

/**
 * \brief Normal value to be returned in the ACK cycle of the data phase in slave receiver mode.
 *
 * \param p_twi   Base address of the TWI instance.
 */
static inline void twi_disable_slave_nack(Twi *p_twi)
{
	p_twi->TWI_SMR &= ~TWI_SMR_NACKEN;
}

/**
 * \brief NACK value to be returned in the ACK cycle of the data phase in slave receiver mode.
 *
 * \param p_twi   Base address of the TWI instance.
 */
static inline void twi_enable_slave_nack(Twi *p_twi)
{
	p_twi->TWI_SMR |= TWI_SMR_NACKEN;
}

/**
 * \brief Acknowledge of the SMBus Default Address disabled.
 *
 * \param p_twi   Base address of the TWI instance.
 */
static inline void twi_disable_slave_default_addr(Twi *p_twi)
{
	p_twi->TWI_SMR &= ~TWI_SMR_SMDA;
}

/**
 * \brief Acknowledge of the SMBus Default Address enabled.
 *
 * \param p_twi   Base address of the TWI instance.
 */
static inline void twi_enable_slave_default_addr(Twi *p_twi)
{
	p_twi->TWI_SMR |= TWI_SMR_SMDA;
}

/**
 * \brief Acknowledge of the SMBus Host Header disabled.
 *
 * \param p_twi   Base address of the TWI instance.
 */
static inline void twi_disable_smbus_host_header(Twi *p_twi)
{
	p_twi->TWI_SMR &= ~TWI_SMR_SMHH;
}

/**
 * \brief Acknowledge of the SMBus Host Header enabled.
 *
 * \param p_twi   Base address of the TWI instance.
 */
static inline void twi_enable_smbus_host_header(Twi *p_twi)
{
	p_twi->TWI_SMR |= TWI_SMR_SMHH;
}

/**
 * \brief Clock stretching disabled in slave mode, OVRE and UNRE will indicate overrun and underrun.
 *
 * \param p_twi   Base address of the TWI instance.
 */
static inline void twi_disable_clock_wait_state(Twi *p_twi)
{
	p_twi->TWI_SMR |= TWI_SMR_SCLWSDIS;
}

/**
 * \brief Clear clock wait state disable mode.
 *
 * \param p_twi   Base address of the TWI instance.
 */
static inline void twi_clear_disable_clock_wait_state(Twi *p_twi)
{
	p_twi->TWI_SMR &= ~TWI_SMR_SCLWSDIS;
}

/**
 * \brief Slave Address 1 matching disabled.
 *
 * \param p_twi   Base address of the TWI instance.
 */
static inline void twi_disable_slave_addr1_matching(Twi *p_twi)
{
	p_twi->TWI_SMR &= ~TWI_SMR_SADR1EN;
}

/**
 * \brief Slave Address 1 matching enabled.
 *
 * \param p_twi   Base address of the TWI instance.
 */
static inline void twi_enable_slave_addr1_matching(Twi *p_twi)
{
	p_twi->TWI_SMR |= TWI_SMR_SADR1EN;
}

/**
 * \brief Slave Address 2 matching disabled.
 *
 * \param p_twi   Base address of the TWI instance.
 */
static inline void twi_disable_slave_addr2_matching(Twi *p_twi)
{
	p_twi->TWI_SMR &= ~TWI_SMR_SADR2EN;
}

/**
 * \brief Slave Address 2 matching enabled.
 *
 * \param p_twi   Base address of the TWI instance.
 */
static inline void twi_enable_slave_addr2_matching(Twi *p_twi)
{
	p_twi->TWI_SMR |= TWI_SMR_SADR2EN;
}

/**
 * \brief Slave Address 3 matching disabled.
 *
 * \param p_twi   Base address of the TWI instance.
 */
static inline void twi_disable_slave_addr3_matching(Twi *p_twi)
{
	p_twi->TWI_SMR &= ~TWI_SMR_SADR3EN;
}

/**
 * \brief Slave Address 3 matching enabled.
 *
 * \param p_twi   Base address of the TWI instance.
 */
static inline void twi_enable_slave_addr3_matching(Twi *p_twi)
{
	p_twi->TWI_SMR |= TWI_SMR_SADR3EN;
}

/**
 * \brief First received data matching disabled.
 *
 * \param p_twi   Base address of the TWI instance.
 */
static inline void twi_disable_slave_data_matching(Twi *p_twi)
{
	p_twi->TWI_SMR &= ~TWI_SMR_DATAMEN;
}

/**
 * \brief First received data matching enabled.
 *
 * \param p_twi   Base address of the TWI instance.
 */
static inline void twi_enable_slave_data_matching(Twi *p_twi)
{
	p_twi->TWI_SMR |= TWI_SMR_DATAMEN;
}

/**
 * \brief Select the souce clock for the bit rate generation.
 *
 * \param p_twi   Base address of the TWI instance.
 * \param src_clk  Source clock.
 */
static inline void twi_select_source_clock(Twi *p_twi, enum twi_source_clock src_clk)
{
	p_twi->TWI_CWGR &= ~TWI_CWGR_BRSRCCLK;
	p_twi->TWI_CWGR |= src_clk;
}

void twi_enable_fifo_master(Twi *p_twi);
void twi_enable_fifo_slave(Twi *p_twi);
void twi_disable_fifo_master(Twi *p_twi);
void twi_disable_fifo_slave(Twi *p_twi);
void twi_set_sniffer_mode(Twi *p_twi, uint32_t ul_device_addr,
		uint32_t ul_mask, bool b_bus_selection);
uint32_t twi_master_read_fifo(Twi *p_twi, twi_packet_t *p_packet);
uint32_t twi_slave_read_fifo(Twi *p_twi, uint8_t *p_data);
uint32_t twi_master_write_fifo(Twi *p_twi, twi_packet_t *p_packet);

/**
 * \brief Clear TWI transmit FIFO.
 *
 * \param p_twi Pointer to a TWI instance.
 */
static inline void twi_clear_tx_fifo(Twi *p_twi)
{
	p_twi->TWI_CR = TWI_CR_TXFCLR;
}

/**
 * \brief Clear TWI receive FIFO.
 *
 * \param p_twi Pointer to a TWI instance.
 */
static inline void twi_clear_rx_fifo(Twi *p_twi)
{
	p_twi->TWI_CR = TWI_CR_RXFCLR;
}

/**
 * \brief Clear TWI FIFO Lock.
 *
 * \param p_twi Pointer to a TWI instance.
 */
static inline void twi_clear_fifo_lock(Twi *p_twi)
{
	p_twi->TWI_CR = TWI_CR_TXFLCLR;
}

/**
 * \brief TWI Master sends Stop automatically upon NACK (default behaviour).
 *
 * \param p_twi Pointer to a TWI instance.
 */
static inline void twi_master_send_stop_on_nack(Twi *p_twi)
{
	p_twi->TWI_MMR &= ~TWI_MMR_NOAP;
}

/**
 * \brief TWI Master does not send any cmd upon NACK.
 *
 * \param p_twi Pointer to a TWI instance.
 */
static inline void twi_master_no_send_cmd_on_nack(Twi *p_twi)
{
	p_twi->TWI_MMR |= TWI_MMR_NOAP;
}

/**
 * \brief Slave Address treated and Address (default mode).
 *
 * \param p_twi   Base address of the TWI instance.
 */
static inline void twi_disable_slave_addr_as_data(Twi *p_twi)
{
	p_twi->TWI_SMR &= ~TWI_SMR_SADAT;
}

/**
 * \brief Slave Address will be treated as Data (RXRDY set, RHR loaded).
 *
 * \param p_twi   Base address of the TWI instance.
 */
static inline void twi_enable_slave_addr_as_data(Twi *p_twi)
{
	p_twi->TWI_SMR |= TWI_SMR_SADAT;
}

/**
 * \brief Clears Sniffer mode bit (the rest of configuration is not modified).
 *
 * \param p_twi   Base address of the TWI instance.
 */
static inline void twi_clear_sniffer_mode(Twi *p_twi)
{
	p_twi->TWI_SMR &= ~TWI_SMR_SNIFF;
}

/**
 * \brief Clears Sniffer mode bit (the rest of configuration is not modified).
 *
 * \param p_twi   Base address of the TWI instance.
 * \param ul_hold   Data on TWD will be held for (ul_hold + 3)*t_pck.
 */
static inline void twi_set_hold_time_alfter_twck_falling(Twi *p_twi, uint32_t ul_hold)
{
	p_twi->TWI_CWGR |= TWI_CWGR_HOLD(ul_hold);
}

/**
 * \brief Reads the Data, Start, Stop and ACK reception info in sniffer mode
 * 		coded as RHR register documentation indicates.
 *
 * \param p_twi Pointer to a TWI instance.
 *
 * \return The Data Start, Stop and ACK reception info.
 */
static inline uint32_t twi_sniffer_read_data_with_associated_info(Twi *p_twi)
{
	return p_twi->TWI_RHR;
}

/**
 * \brief Set TWI FIFO Tx Threshold to raise a flag
 * when number of data in Tx FIFO becomes equal or lower.
 *
 * \param p_twi Pointer to a TWI instance.
 * \param ul_threshold Threshold to set.
 */
static inline void twi_set_fifo_tx_threshold(Twi *p_twi, uint32_t ul_threshold)
{
	p_twi->TWI_FMR &= ~TWI_FMR_TXFTHRES_Msk;
	p_twi->TWI_FMR |= TWI_FMR_TXFTHRES(ul_threshold);
}

/**
 * \brief Set TWI FIFO Rx Threshold to raise a flag
 * when number of data in Rx FIFO becomes equal or higher.
 *
 * \param p_twi Pointer to a TWI instance.
 * \param ul_threshold Threshold to set.
 */
static inline void twi_set_fifo_rx_threshold(Twi *p_twi, uint32_t ul_threshold)
{
	p_twi->TWI_FMR &= ~TWI_FMR_RXFTHRES_Msk;
	p_twi->TWI_FMR |= TWI_FMR_RXFTHRES(ul_threshold);
}

/**
 * \brief Get number of data in the Tx TWI FIFO.
 *
 * \param p_twi Pointer to a TWI instance.
 *
 * \return Number of data in the Transmit FIFO.
 */
static inline uint32_t twi_get_fifo_tx_level(Twi *p_twi)
{
	return ((p_twi->TWI_FLR & TWI_FLR_TXFL_Msk) >> TWI_FLR_TXFL_Pos);
}

/**
 * \brief Get number of unread data in the Rx TWI FIFO.
 *
 * \param p_twi Pointer to a TWI instance.
 *
 * \return Number of unread data in the Receive FIFO.
 */
static inline uint32_t twi_get_fifo_rx_level(Twi *p_twi)
{
	return ((p_twi->TWI_FLR & TWI_FLR_RXFL_Msk) >> TWI_FLR_RXFL_Pos);
}

/**
 * \brief Get TWI FIFO status.
 *
 * \param p_twi Pointer to a TWI instance.
 *
 * \retval TWI FIFO status.
 */
static inline uint32_t twi_get_fifo_status(Twi *p_twi)
{
	return p_twi->TWI_FSR;
}

/**
 * \brief Enable TWI FIFO interrupts.
 *
 * \param p_twi Pointer to a TWI instance.
 * \param ul_sources Interrupts to be enabled.
 */
static inline void twi_enable_fifo_interrupt(Twi *p_twi, uint32_t ul_sources)
{
	p_twi->TWI_FIER = ul_sources;
}

/**
 * \brief Disable TWI FIFO interrupts.
 *
 * \param p_twi Pointer to a TWI instance.
 * \param ul_sources Interrupts to be disabled.
 */
static inline void twi_disable_fifo_interrupt(Twi *p_twi, uint32_t ul_sources)
{
	p_twi->TWI_FIDR = ul_sources;
	p_twi->TWI_FSR;
}

/**
 * \brief Read TWI FIFO interrupt mask.
 *
 * \param p_twi Pointer to a TWI instance.
 *
 * \return The interrupt mask value.
 */
static inline uint32_t twi_get_fifo_interrupt_mask(Twi *p_twi)
{
	return p_twi->TWI_FIMR;
}

/**
 * \brief Enable write protect of TWI Configuration registers.
 *
 * \param p_twi Pointer to a TWI instance.
 */
static inline void twi_enable_writeprotect_cfg_regs(Twi *p_twi)
{
	p_twi->TWI_WPMR = TWI_WPMR_WPEN | TWI_WPMR_WPKEY_PASSWD;
}

/**
 * \brief Enable write protect of TWI Control registers.
 *
 * \param p_twi Pointer to a TWI instance.
 */
static inline void twi_enable_writeprotect_ctl_regs(Twi *p_twi)
{
	p_twi->TWI_WPMR = TWI_WPMR_WPCREN | TWI_WPMR_WPKEY_PASSWD;
}

/**
 * \brief Enable write protect of TWI Interrupt registers.
 *
 * \param p_twi Pointer to a TWI instance.
 */
static inline void twi_enable_writeprotect_int_regs(Twi *p_twi)
{
	p_twi->TWI_WPMR = TWI_WPMR_WPITEN | TWI_WPMR_WPKEY_PASSWD;
}

/**
 * \brief Enable write protect of all TWI registers.
 *
 * \param p_twi Pointer to a TWI instance.
 */
static inline void twi_enable_writeprotect_all_regs(Twi *p_twi)
{
	p_twi->TWI_WPMR = TWI_WPMR_WPEN | TWI_WPMR_WPCREN |
			TWI_WPMR_WPITEN | TWI_WPMR_WPKEY_PASSWD;
}

/**
 * \brief Disable write protect of all TWI registers.
 *
 * \param p_twi Pointer to a TWI instance.
 */
static inline void twi_disable_writeprotect_all_regs(Twi *p_twi)
{
	p_twi->TWI_WPMR = TWI_WPMR_WPKEY_PASSWD;
}

/**
 * \brief Get the write protection status.
 *
 * \param p_twi Pointer to a TWI instance.
 *
 * \return The Write Protection Status.
 */
static inline uint32_t twi_get_write_protection_status(Twi *p_twi)
{
	return p_twi->TWI_WPSR;
}

/// @cond 0
/**INDENT-OFF**/
#ifdef __cplusplus
}
#endif
/**INDENT-ON**/
/// @endcond

#endif /* TWI_H_INCLUDED */
