/**
 * \file
 *
 * \brief Quad Serial Peripheral Interface (QSPI) driver for SAM.
 *
 * Copyright (c) 2015-2018 Microchip Technology Inc. and its subsidiaries.
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

#ifndef QSPI_H_INCLUDED
#define QSPI_H_INCLUDED

#include "compiler.h"
#include "string.h"
#include "status_codes.h"
#include <assert.h>
#include "sysclk.h"

/* / @cond 0 */
/**INDENT-OFF**/
#ifdef __cplusplus
extern "C" {
#endif
/**INDENT-ON**/
/* / @endcond */

#ifndef QSPI_WPMR_WPKEY_PASSWD
#define QSPI_WPMR_WPKEY_PASSWD QSPI_WPMR_WPKEY((uint32_t)0x515350)
#endif

/** Time-out value (number of attempts). */
#define QSPI_SPI_TIMEOUT       15000

/** QSPI chip select mode. */
enum qspi_cs_mode {
	QSPI_NOT_RELOADED = 0,
	QSPI_LASTXFER,
	QSPI_SYSTEMATICALLY,
};

/**
 * \brief Qspi0 enum types for QSPI modes.
 */
enum qspi_run_mode {
	QSPI_SPI_MODE = QSPI_MR_SMM_SPI,
	QSPI_MEM_MODE = QSPI_MR_SMM_MEMORY
};

/**
 * \brief Qspi0 enum types for frame Address length.
 */
enum qspi_ifr_address_len {
	QSPI_IFR_ADDRESS_8_BIT = 0,
	QSPI_IFR_ADDRESS_16_BIT,
	QSPI_IFR_ADDRESS_24_BIT,
	QSPI_IFR_ADDRESS_32_BIT
};

/**
 * \brief Qspi0 enum types for frame Option Code length.
 */
enum qspi_ifr_option_len {
	QSPI_IFR_OPTION_1_BIT = 0,
	QSPI_IFR_OPTION_2_BIT,
	QSPI_IFR_OPTION_4_BIT,
	QSPI_IFR_OPTION_8_BIT
};

/**
 * \brief Qspi0 enum types for frame Width of Instruction Code, Address,
 * Option code and Data.
 */
enum qspi_ifr_width {
	QSPI_IFR_SINGLE_BIT_SPI = 0, /* Instruction: Single-bit SPI / Address-Option: Single-bit SPI / Data: Single-bit SPI */
	QSPI_IFR_DUAL_OUTPUT, /* Instruction: Single-bit SPI / Address-Option: Single-bit SPI / Data: Dual SPI */
	QSPI_IFR_QUAD_OUTPUT, /* Instruction: Single-bit SPI / Address-Option: Single-bit SPI / Data: Quad SPI */
	QSPI_IFR_DUAL_IO, /* Instruction: Single-bit SPI / Address-Option: Dual SPI / Data: Dual SPI */
	QSPI_IFR_QUAD_IO, /* Instruction: Single-bit SPI / Address-Option: Quad SPI / Data: Quad SPI */
	QSPI_IFR_DUAL_CMD, /* Instruction: Dual SPI / Address-Option: Dual SPI / Data: Dual SPI */
	QSPI_IFR_QUAD_CMD /* Instruction: Quad SPI / Address-Option: Quad SPI / Data: Quad SPI */
};

/**
 * \brief Qspi0 frame structure for QSPI mode
 */
struct qspi_inst_frame_t {
	union _qspiinst_frame {
		uint32_t val;
		struct _qspiinst_frame_bitmap {
			/* Width of QSPI Addr , inst data */
			uint32_t b_width : 4;
			/* Enable Inst */
			uint32_t b_inst_en : 1;
			/* Enable Address */
			uint32_t b_addr_en : 1;
			/* Enable Option */
			uint32_t b_opt_en : 1;
			/* Enable Data */
			uint32_t b_data_en : 1;
			/* Option Length */
			uint32_t b_opt_len : 2;
			/* Addrs Length */
			uint32_t b_addr_len : 2;
			/* Transfer type */
			uint32_t b_tfr_type : 2;
			/* Reserved */
			uint32_t reserved1 : 1;
			/* Continoues read mode */
			uint32_t b_continues_read : 1;
			/* DDR mode enable */
			uint32_t b_ddr_en : 1;
			/* Number of dummy cycles */
			uint32_t b_dummy_cycles : 5;
			/* Reserved */
			uint32_t reserved2 : 2;
			/* Serial Memory Register Mode */
			uint32_t b_smrm : 1;
			/* Peripheral Bus Transfer Type */
			uint32_t b_apb_tfr_type : 1;
			/* DQS Sampling Enable */
			uint32_t b_dqs_en : 1;
			/* DDR Mode Command Enable */
			uint32_t b_ddr_cmd_en : 1;
			/* Reserved */
			uint32_t reserved3 : 1;
			/* Protocol Type */
			uint32_t b_prot_type : 2;
			/* Reserved */
			uint32_t reserved4 : 2;
		}
		bm;
	}
	inst_frame;
	uint32_t addr;
};

/**
 * \brief Qspi0 buffer structure
 */
struct qspi_buffer_t {
	/* Tx buffer size */
	uint32_t tx_data_size;
	/* Rx buffer size */
	uint32_t rx_data_size;
	/* Tx buffer */
	uint8_t *data_tx;
	/* Rx buffer */
	uint8_t *data_rx;
};

/**
 * \brief Qspi0 command structure
 */
struct qspi_mem_cmd_t {
	/* Qspi0 instruction code */
	uint8_t instruction;
	/* Qspi0 option code */
	uint8_t option;
};

/**
 * \brief Qspi0 driver structure
 */
struct qspid_t {
	/* QSPI Hw instance */
	Qspi0 *qspi_hw;
	/* Qspi0 command structure */
	struct qspi_mem_cmd_t qspi_command;
	/* Qspi0 buffer */
	struct qspi_buffer_t qspi_buffer;
	/* Qspi0 QSPI mode Frame register informations */
	struct qspi_inst_frame_t *qspi_frame;
};

/**
 * \brief Qspi0 config structure
 */
struct qspi_config_t {
	/* Qspi0 serial memory mode */
	enum qspi_run_mode serial_memory_mode;
	/* Wait data read for transfer */
	bool wait_data_for_transfer;
	/* Chip select mode */
	enum qspi_cs_mode csmode;
	/* Numbers of bits per transfer */
	uint32_t bits_per_transfer;
	/* Minimum inactive QCS delay */
	uint32_t min_delay_qcs;
	/* Delay between consecutive transfers */
	uint32_t delay_between_ct;
	/* Qspi0 clock polarity */
	uint32_t clock_polarity;
	/* Qspi0 clock phase */
	uint32_t clock_phase;
	/* Qspi0 transfer delay */
	uint32_t transfer_delay;
	/* Scrambling/unscrambling enable */
	bool scrambling_en;
	/* Scrambling/unscrambling random value disable */
	bool scrambling_random_value_dis;
	/* Lock Scrambling Key */
	bool scrambling_lock_key;
	/* Scrambling user key */
	uint32_t scrambling_user_key;
};

/**
 * \brief Qspi0 access modes
 */
enum qspi_access {
	/* Cmd access */
	QSPI_CMD_ACCESS  = 0,
	/* Read access */
	QSPI_READ_ACCESS,
	/* Write access */
	QSPI_WRITE_ACCESS
};

/**************************  SR *********************************************/

/**
 * \brief Issue a LASTXFER command.
 *  The next transfer is the last transfer and after that CS is de-asserted.
 *
 * \param qspi Pointer to an QSPI instance.
 */
static inline void qspi_set_lastxfer(Qspi0 *qspi)
{
	/* Wait end of QSPI sync busy */
	while ((qspi->QSPI_SR & QSPI_SR_SYNCBSY)) {
	}
	qspi->QSPI_CR = QSPI_CR_LASTXFER;
}

/**
 * \brief Request a TOUT flag reset.
 *
 * \param qspi Pointer to an QSPI instance.
 */
static inline void qspi_reset_time_out(Qspi0 *qspi)
{
	qspi->QSPI_CR = QSPI_CR_RTOUT;
}

/**
 * \brief Starts the transfer when TFRTYP = 0 and SMRM = 1 or when DATAEN = 0.
 *
 * \param qspi Pointer to an QSPI instance.
 */
static inline void qspi_start_trasnfer(Qspi0 *qspi)
{
	/* Wait end of QSPI sync busy */
	while ((qspi->QSPI_SR & QSPI_SR_SYNCBSY)) {
	}
	/* Start Transfer */
	qspi->QSPI_CR = QSPI_CR_STTFR;
	/* Wait end of QSPI sync busy */
	while ((qspi->QSPI_SR & QSPI_SR_SYNCBSY)) {
	}
}

/**
 * \brief Requests an update of the QSPI Controller core configuration.
 *
 * \param qspi Pointer to an QSPI instance.
 */
static inline void qspi_update_configuration(Qspi0 *qspi)
{
	/* QSPI_SR.SYNCBSY must be ‘0’ before writing QSPI_CR.UPDCFG */
	while ((qspi->QSPI_SR & QSPI_SR_SYNCBSY)) {
	}
	/* Update Configuration */
	qspi->QSPI_CR = QSPI_CR_UPDCFG;
	/* Wait end of QSPI sync busy */
	while ((qspi->QSPI_SR & QSPI_SR_SYNCBSY)) {
	}
}

/**
 * \brief Resets a QSPI peripheral.
 *
 * \param qspi  Pointer to a Qspi0 instance.
 */
static inline void qspi_reset(Qspi0 *qspi)
{
	qspi->QSPI_CR = QSPI_CR_SWRST;
}

/**
 * \brief Enables a QSPI peripheral.
 *
 * \param qspi  Pointer to a Qspi0 instance.
 */
static inline void qspi_enable(Qspi0 *qspi)
{
	qspi->QSPI_CR = QSPI_CR_QSPIEN;
	while (!(qspi->QSPI_SR & QSPI_SR_QSPIENS)) {
	}
}

/**
 * \brief Disables a QSPI peripheral.
 *
 * \param qspi  Pointer to a Qspi0 instance.
 */
static inline void qspi_disable(Qspi0 *qspi)
{
	qspi->QSPI_CR = QSPI_CR_QSPIDIS;
	while (qspi->QSPI_SR & QSPI_SR_QSPIENS) {
	}
}

/**************************  MR *********************************************/

/**
 * \brief Set qspi minimum inactive QCS delay.
 *
 * \param qspi      Pointer to a Qspi0 instance.
 * \param uc_dlybct Time to be delay.
 */
static inline void qspi_set_minimum_inactive_qcs_delay(Qspi0 *qspi, uint8_t uc_dlybct)
{
	qspi->QSPI_MR &= (~QSPI_MR_DLYCS_Msk);
	qspi->QSPI_MR |= QSPI_MR_DLYCS(uc_dlybct);
}

/**
 * \brief Set qspi delay between consecutive transfers.
 *
 * \param qspi     Pointer to a Qspi0 instance.
 * \param uc_dlycs Time to be delay.
 */
static inline void qspi_set_delay_between_consecutive_transfers(Qspi0 *qspi, uint32_t uc_dlycs)
{
	qspi->QSPI_MR &= (~QSPI_MR_DLYBCT_Msk);
	qspi->QSPI_MR |= QSPI_MR_DLYBCT(uc_dlycs);
}

/**
 * \brief Set the number of data bits transferred.
 *
 * \param qspi Pointer to an QSPI instance.
 * \param bits Bits per transfer.
 */
static inline void qspi_set_bits_per_transfer(Qspi0 *qspi, uint32_t bits)
{
	qspi->QSPI_MR &= (~QSPI_MR_NBBITS_Msk);
	qspi->QSPI_MR |= bits;
}

/**
 * \brief Enable TAMPER protection.
 * A tamper detection event immediately clears QSPI scrambling keys
 *
 * \param qspi     Pointer to a Qspi0 instance.
 */
static inline void qspi_enable_tamper_protection(Qspi0 *qspi)
{
	qspi->QSPI_MR |= QSPI_MR_TAMPCLR;
}

/**
 * \brief Disable TAMPER protection.
 *
 * \param qspi     Pointer to a Qspi0 instance.
 */
static inline void qspi_disable_tamper_protection(Qspi0 *qspi)
{
	qspi->QSPI_MR &= (~QSPI_MR_TAMPCLR);
}

/**
 * \brief Set Chip Select Mode.
 *
 * \param qspi    Pointer to an QSPI instance.
 * \param csmode  Chip select mode to be set.
 */
static inline void qspi_set_chip_select_mode(Qspi0 *qspi, uint32_t csmode)
{
	qspi->QSPI_MR &= (~QSPI_MR_CSMODE_Msk);
	qspi->QSPI_MR |= QSPI_MR_CSMODE(csmode);
}

/**
 * \brief Enable waiting RX_EMPTY before transfer starts.
 * This mode prevents overrun error in reception
 *
 * \param qspi Pointer to an QSPI instance.
 */
static inline void qspi_enable_wait_data_read_before_transfer(Qspi0 *qspi)
{
	qspi->QSPI_MR |= QSPI_MR_WDRBT;
}

/**
 * \brief Disable waiting RX_EMPTY before transfer starts.
 *
 * \param qspi Pointer to an QSPI instance.
 */
static inline void qspi_disable_wait_data_read_before_transfer(Qspi0 *qspi)
{
	qspi->QSPI_MR &= (~QSPI_MR_WDRBT);
}

/**
 * \brief Set QSPI to Memory mode.
 *
 * \param qspi Pointer to an QSPI instance.
 */
static inline void qspi_set_memory_mode(Qspi0 *qspi)
{
	qspi->QSPI_MR |= QSPI_MR_SMM_MEMORY;
}

/**
 * \brief Set QSPI to SPI mode (Master mode only).
 *
 * \param qspi Pointer to an QSPI instance.
 */
static inline void qspi_set_spi_mode(Qspi0 *qspi)
{
	qspi->QSPI_MR &= (~QSPI_MR_SMM_SPI);
}

/**************************  RDR *********************************************/

/**
 * \brief Read QSPI RDR register for SPI mode
 *
 * \param qspi   Pointer to an Qspi0 instance.
 * \return status Data value read from QSPI.
 */
static inline uint16_t qspi_read_spi(Qspi0 *qspi)
{
	/* Wait end of QSPI sync busy */
	while ((qspi->QSPI_SR & QSPI_SR_SYNCBSY)) {
	}
	return qspi->QSPI_RDR;
}

/**************************  TDR *********************************************/

/**
 * \brief Write to QSPI Tx register in SPI mode
 *
 * \param qspi     Pointer to an Qspi0 instance.
 * \param w_data   Data to transmit
 */
static inline void qspi_write_spi(Qspi0 *qspi, uint16_t w_data)
{
	/** Send data */
	qspi->QSPI_TDR = w_data;
	/* Wait end of QSPI sync busy */
	while ((qspi->QSPI_SR & QSPI_SR_SYNCBSY)) {
	}
}

/**************************  ISR/IER/IDR/IMR *********************************/

/**
 * \brief Enable QSPI interrupts.
 *
 * \param qspi Pointer to an QSPI instance.
 * \param sources Interrupts to be enabled.
 */
static inline void qspi_enable_interrupt(Qspi0 *qspi, uint32_t sources)
{
	qspi->QSPI_IER = sources;
}

/**
 * \brief Disable QSPI interrupts.
 *
 * \param qspi Pointer to an QSPI instance.
 * \param sources Interrupts to be disabled.
 */
static inline void qspi_disable_interrupt(Qspi0 *qspi, uint32_t sources)
{
	qspi->QSPI_IDR = sources;
}

/**
 * \brief Read QSPI interrupt status.
 *
 * \param p_qspi Pointer to an QSPI instance.
 *
 * \return The interrupt status value.
 */
static inline uint32_t qspi_read_interrupt_status(Qspi0 *p_qspi)
{
	return p_qspi->QSPI_ISR;
}

/**
 * \brief Read QSPI interrupt mask.
 *
 * \param p_qspi Pointer to an QSPI instance.
 *
 * \return The interrupt mask value.
 */
static inline uint32_t qspi_read_interrupt_mask(Qspi0 *p_qspi)
{
	return p_qspi->QSPI_IMR;
}

/**************************  SCR *********************************/

/**
 * \brief Set qspi clock transfer delay.
 *
 * \param qspi     Pointer to a Qspi0 instance.
 * \param uc_dlybs Delay before QSCK.
 */
static inline void qspi_set_transfer_delay(Qspi0 *qspi, uint8_t uc_dlybs)
{
	qspi->QSPI_SCR &= (~QSPI_SCR_DLYBS_Msk);
	qspi->QSPI_SCR |= QSPI_SCR_DLYBS(uc_dlybs);
}

/**
 * \brief Set qspi clock polarity.
 *
 * \param qspi  Pointer to a Qspi0 instance.
 * \param polarity Polarity to be set.
 */
static inline void qspi_set_clock_polarity(Qspi0 *qspi, uint32_t polarity)
{
	if (polarity) {
		qspi->QSPI_SCR |= QSPI_SCR_CPOL;
	} else {
		qspi->QSPI_SCR &= (~QSPI_SCR_CPOL);
	}
}

/**
 * \brief Set qspi clock phase.
 *
 * \param qspi  Pointer to a Qspi0 instance.
 * \param phase Phase to be set.
 */
static inline void qspi_set_clock_phase(Qspi0 *qspi, uint32_t phase)
{
	if (phase) {
		qspi->QSPI_SCR |= QSPI_SCR_CPHA;
	} else {
		qspi->QSPI_SCR &= (~QSPI_SCR_CPHA);
	}
}

/**************************  SR *********************************/

/**
 * \brief Check if the QSPI is in IDLE state.
 *
 * \param p_qspi Pointer to an QSPI instance.
 *
 * \return 1 if the QSPI is in Idle state, otherwise 0.
 */
static inline uint32_t qspi_is_idle(Qspi0 *qspi)
{
	if (qspi->QSPI_SR & QSPI_SR_HIDLE) {
		return 1;
	} else {
		return 0;
	}
}

/**
 * \brief Check if the QSPI is busy.
 *
 * \param p_qspi Pointer to an QSPI instance.
 *
 * \return 1 if the QSPI is busy, otherwise 0.
 */
static inline uint32_t qspi_is_busy(Qspi0 *qspi)
{
	if (qspi->QSPI_SR & QSPI_SR_RBUSY) {
		return 1;
	} else {
		return 0;
	}
}

/**
 * \brief Check if the QSPI chip select is asserted.
 *
 * \param p_qspi Pointer to an QSPI instance.
 *
 * \return 1 if the QSPI chip select is not asserted, otherwise 0.
 */
static inline uint32_t qspi_is_cs_asserted(Qspi0 *qspi)
{
	if (qspi->QSPI_SR & QSPI_SR_CSS) {
		return 1;
	} else {
		return 0;
	}
}

/**
 * \brief Check if the QSPI is enabled.
 *
 * \param p_qspi Pointer to an QSPI instance.
 *
 * \return 1 if the QSPI is enabled, otherwise 0.
 */
static inline uint32_t qspi_is_enabled(Qspi0 *qspi)
{
	if (qspi->QSPI_SR & QSPI_SR_QSPIENS) {
		return 1;
	} else {
		return 0;
	}
}

/**
 * \brief Check if the QSPI synchronization is busy.
 *
 * \param p_qspi Pointer to an QSPI instance.
 *
 * \return 1 if the QSPI synchronization is ongoing, otherwise 0.
 */
static inline uint32_t qspi_is_sync_busy(Qspi0 *qspi)
{
	if (qspi->QSPI_SR & QSPI_SR_SYNCBSY) {
		return 1;
	} else {
		return 0;
	}
}

/**************************  IAR *********************************/

/**
 * \brief Set qspi instruction addr.
 *
 * \param qspi  Pointer to a Qspi0 instance.
 * \param addr  Address to be set.
 */
static inline void qspi_set_instruction_addr(Qspi0 *qspi, uint32_t addr)
{
	qspi->QSPI_IAR = QSPI_IAR_ADDR(addr);
}

/**************************  WICR *********************************/

/**
 * \brief Set qspi write instruction code.
 *
 * \param qspi  Pointer to a Qspi0 instance.
 * \param instruction_code    Code to be set.
 */
static inline void qspi_set_write_instruction_code(Qspi0 *qspi, struct qspi_mem_cmd_t instruction_code)
{
	qspi->QSPI_WICR = QSPI_WICR_WRINST(instruction_code.instruction) | QSPI_WICR_WROPT(instruction_code.option);
}

/**************************  IFR *********************************/

/**
 * \brief Reads the Instruction frame of QSPI
 *
 * \param pQspi0   Pointer to an Qspi0 instance.
 * \return status QSPI frame value.
 */
static inline uint32_t qspi_get_inst_frame(Qspi0 *qspi)
{
	return qspi->QSPI_IFR;
}

void qspi_set_instruction_frame(Qspi0 *qspi, struct qspi_inst_frame_t instruction_frame);

/**************************  RICR *********************************************/

/**
 * \brief Set qspi read instruction code.
 *
 * \param qspi  Pointer to a Qspi0 instance.
 * \param instruction_code    Code to be set.
 */
static inline void qspi_set_read_instruction_code(Qspi0 *qspi, struct qspi_mem_cmd_t instruction_code)
{
	qspi->QSPI_RICR = QSPI_RICR_RDINST(instruction_code.instruction) | QSPI_RICR_RDOPT(instruction_code.option);
}

/**************************  SMR *********************************************/

/**
 * \brief Enable qspi scrambling mode.
 *
 * \param qspi  Pointer to a Qspi0 instance.
 * \param random_mode_dis  Disable random value plus scrambling key.
 * \param scr_key_lock  Lock scrambling key.
 */
static inline void qspi_enable_scrambling_mode(Qspi0 *qspi, uint32_t random_mode_dis, uint32_t scr_key_lock)
{
	if (random_mode_dis) {
		qspi->QSPI_SMR |= QSPI_SMR_RVDIS;
	} else {
		qspi->QSPI_SMR &= (~QSPI_SMR_RVDIS);
	}

	if (scr_key_lock) {
		qspi->QSPI_SMR |= QSPI_SMR_SCRKL;
	}

	qspi->QSPI_SMR |= QSPI_SMR_SCREN;
}

/**
 * \brief Disable qspi scrambling mode.
 *
 * \param qspi  Pointer to a Qspi0 instance.
 */
static inline void qspi_disable_scrambling_mode(Qspi0 *qspi)
{
	qspi->QSPI_SMR &= (~QSPI_SMR_SCREN);
}

/**************************  SKR *********************************************/

/**
 * \brief Set qspi scrambling key.
 *
 * \param qspi  Pointer to a Qspi0 instance.
 * \param mode  Mode to be set.
 * \return If write succeeded, return 0, else return 1.
 */
static inline uint32_t qspi_set_scrambling_key(Qspi0 *qspi, uint32_t key)
{
	if (qspi->QSPI_WPMR & QSPI_WPMR_WPEN) {
		return 1;
	}

	qspi->QSPI_SKR = QSPI_SKR_USRK(key);

	return 0;
}

/**************************  WRACNT *********************************************/

/**
 * \brief Set the number of write accesses before the rise of QSPI_ISR.LWRA.
 *
 * \param qspi  Pointer to a Qspi0 instance.
 * \param nbwra  Number of write accesses.
 */
static inline void qspi_set_number_of_write_accesses(Qspi0 *qspi, uint32_t nbwra)
{
	qspi->QSPI_WRACNT = nbwra;
}

/**************************  TOUT *********************************************/

/**
 * \brief Set the Time-out Counter Maximum Value.
 *
 * \param qspi  Pointer to a Qspi0 instance.
 * \param tcntm  Time-out Counter Maximum Value.
 */
static inline void qspi_set_timeout_counter_max_value(Qspi0 *qspi, uint32_t tcntm)
{
	qspi->QSPI_TOUT = tcntm;
}

/**
 * \brief Get write protection status.
 *
 * \param qspi  Pointer to a Qspi0 instance.
 * \return Write protection status.
 * \return status QSPI write protect status.
 */
static inline uint32_t qspi_get_writeprotect_status(Qspi0 *qspi)
{
	return qspi->QSPI_WPSR;
}

void qspi_set_writeprotect(Qspi0 *qspi, uint32_t enable, uint32_t int_enable, uint32_t control_enable);

/**
 * \brief Ends ongoing transfer by releasing CS of QSPI peripheral.
 *
 * \param qspi  Pointer to an Qspi0 instance.
 */
static inline void qspi_end_transfer(Qspi0 *qspi)
{
	while (!(qspi->QSPI_ISR & QSPI_ISR_TXEMPTY)) {
	}
	qspi_set_lastxfer(qspi);
}

enum status_code qspia_read(Qspi0 *qspi, uint16_t *us_data, uint32_t num_of_bytes);
enum status_code qspia_write(Qspi0 *qspi, uint16_t *us_data, uint32_t num_of_bytes);

enum status_code qspi_set_config(Qspi0 *qspi, struct qspi_config_t *qspi_config);
enum status_code qspi_initialize(Qspi0 *qspi, struct qspi_config_t *qspi_config);
void qspi_get_config_default(struct qspi_config_t *qspi_config);

/** Functionality API -- Serial Memory Mode */
enum status_code qspi_flash_execute_command(struct qspid_t *qspid, enum qspi_access read_write);
enum status_code qspi_flash_access_memory(struct qspid_t *qspid, enum qspi_access read_write, uint8_t scramble_flag, uint8_t aesb_flag);

/* / @cond 0 */
/**INDENT-OFF**/
#ifdef __cplusplus
}
#endif
/**INDENT-ON**/
/* / @endcond */

/**
 * \page sam_spi_quickstart Quick start guide for SAM QSPI driver
 *
 * This is the quick start guide for the \ref spi_group "SAM QSPI driver",
 * with step-by-step instructions on how to configure and use the driver in a
 * selection of use cases.
 *
 * The use cases contain several code fragments. The code fragments in the
 * steps for setup can be copied into a custom initialization function, while
 * the steps for usage can be copied into, e.g.the main application function.
 *
 * \section spi_basic_use_case Basic use case of the QSPI driver
 * In this basic use case, the QSPI module are configured for:
 * - SPI mode
 * - Serial Memory Mode
 *
 * \subsection sam_spi_quickstart_prereq Prerequisites
 * -# \ref sysclk_group "System Clock Management (Sysclock)"
 *
 * \section spi_basic_use_case_setup Setup steps
 * \subsection spi_basic_use_case_setup_code Example code
 * Add to application C-file:
 * \code
 *              void qspi_get_config_default(qspi_config_t * qspi_config)
 *              {
 *                      qspi_config->serial_memory_mode = QSPI_MEM_MODE;
 *                      qspi_config->wait_data_for_transfer = false;
 *                      qspi_config->csmode = QSPI_LASTXFER;
 *                      qspi_config->bits_per_transfer = QSPI_MR_NBBITS_8_BIT;
 *                      qspi_config->min_delay_qcs = 0;
 *                      qspi_config->delay_between_ct = 0;
 *                      qspi_config->clock_polarity = 0;
 *                      qspi_config->clock_phase = 0;
 *                      qspi_config->transfer_delay = 0;
 *                      qspi_config->scrambling_en = false;
 *                      qspi_config->scrambling_random_value_dis = false;
 *                      qspi_config->scrambling_lock_key = false;
 *                      qspi_config->scrambling_user_key = 0;
 *              }
 *
 *              void qspi_initialize(Qspi0 *qspi, struct qspi_config_t *qspi_config)
 *              {
 *                      qspi_disable(qspi);
 *                      qspi_reset(qspi);
 *                      qspi_set_config(qspi, qspi_config);
 *                      while (qspi_is_sync_busy(qspi));
 *                      qspi_update_configuration(qspi);
 *                      while (qspi_is_sync_busy(qspi));
 *                      qspi_enable(qspi);
 *              }
 * \endcode
 */
#endif /* QSPI_H_INCLUDED */
