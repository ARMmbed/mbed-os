/**
 * \file
 *
 * \brief Quad Serial Peripheral Interface (QSPI) driver for PIC32CX.
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

#include "qspia.h"

static bool sb_scramble_en;

/**
 * \defgroup sam_drivers_qspi_group Quad Serial Peripheral Interface (QSPI)
 *
 * See \ref sam_qspi_quickstart.
 *
 * The Quad SPI Interface (QSPI) is a synchronous serial data link that provides communication with external
 * devices in Master mode.
 * The QSPI can be used in SPI mode to interface to serial peripherals (such as ADCs, DACs, LCD controllers, CAN
 * controllers and sensors), or in Serial Memory mode to interface to serial Flash memories.
 * The QSPI allows the system to execute code directly from a serial Flash memory (XIP) without code shadowing to
 * RAM. The serial Flash memory mapping is seen in the system as other memories (ROM, SRAM, DRAM,
 * embedded Flash memory, etc.).
 * With the support of the Quad SPI protocol, the QSPI allows the system to use high-performance serial Flash
 * memories which are small and inexpensive, in place of larger and more expensive parallel Flash memories.
 *
 * @{
 */

/**
 * \brief Memory copy function.
 *
 * \param dst  Pointer to destination buffer.
 * \param src  Pointer to source buffer.
 * \param count  Bytes to be copied.
 */
static void _qspi_memcpy(uint32_t *dst, uint32_t *src, uint32_t count)
{
	while (count) {
		if (count < 4) {
			*(uint8_t *)dst = *(uint8_t *)src;
			dst = (uint32_t *)((uint8_t *)dst + 1);
			src = (uint32_t *)((uint8_t *)src + 1);
			count--;
		} else {
			*dst++ = *src++;
			count -= 4;
		}
	}
}

/**************************  IFR *********************************************/

/**
 * \brief Set qspi instruction frame.
 *
 * \param qspi  Pointer to a Qspi0 instance.
 * \param instruction_frame   Frame to be set.
 */
void qspi_set_instruction_frame(Qspi0 *qspi, struct qspi_inst_frame_t instruction_frame)
{
	uint32_t mask = 0;
	mask |= QSPI_IFR_WIDTH(instruction_frame.inst_frame.bm.b_width);
	if (instruction_frame.inst_frame.bm.b_inst_en) {
		mask |= QSPI_IFR_INSTEN;
	}

	if (instruction_frame.inst_frame.bm.b_addr_en) {
		mask |= QSPI_IFR_ADDREN;
	}

	if (instruction_frame.inst_frame.bm.b_opt_en) {
		mask |= QSPI_IFR_OPTEN;
	}

	if (instruction_frame.inst_frame.bm.b_data_en) {
		mask |= QSPI_IFR_DATAEN;
	}

	if (instruction_frame.inst_frame.bm.b_opt_len) {
		mask |= QSPI_IFR_OPTL(instruction_frame.inst_frame.bm.b_opt_len);
	}

	if (instruction_frame.inst_frame.bm.b_addr_en) {
		mask |= QSPI_IFR_ADDRL(instruction_frame.inst_frame.bm.b_addr_len);
	}

	if (instruction_frame.inst_frame.bm.b_tfr_type) {
		mask |= QSPI_IFR_TFRTYP_TRSFR_MEMORY;
	}

	if (instruction_frame.inst_frame.bm.b_continues_read) {
		mask |= QSPI_IFR_CRM_ENABLED;
	}

	if (instruction_frame.inst_frame.bm.b_ddr_en) {
		mask |= QSPI_IFR_DDREN_ENABLED;
	}

	if (instruction_frame.inst_frame.bm.b_dummy_cycles) {
		mask |= QSPI_IFR_NBDUM(instruction_frame.inst_frame.bm.b_dummy_cycles);
	}

	if (instruction_frame.inst_frame.bm.b_smrm) {
		mask |= QSPI_IFR_SMRM;
	}

	if (instruction_frame.inst_frame.bm.b_apb_tfr_type) {
		mask |= QSPI_IFR_APBTFRTYP;
	}

	if (instruction_frame.inst_frame.bm.b_dqs_en) {
		mask |= QSPI_IFR_DQSEN;
	}

	if (instruction_frame.inst_frame.bm.b_ddr_cmd_en) {
		mask |= QSPI_IFR_DDRCMDEN_ENABLED;
	}

	if (instruction_frame.inst_frame.bm.b_prot_type) {
		mask |= QSPI_IFR_PROTTYP(instruction_frame.inst_frame.bm.b_prot_type);
	}

	qspi->QSPI_IFR = mask;
}

/**************************  WPMR *********************************************/

/**
 * \brief Enable write protection.
 *
 * \param qspi    Pointer to a Qspi0 instance.
 * \param enable  Enable or disable write protection.
 * \param int_enable  Enable or disable write protection interrupt.
 * \param control_enable  Enable or disable write protection control register.
 */
void qspi_set_writeprotect(Qspi0 *qspi, uint32_t enable, uint32_t int_enable, uint32_t control_enable)
{
	uint32_t ul_mask = 0;

	ul_mask |= QSPI_WPMR_WPKEY_PASSWD;

	if (enable) {
		ul_mask |= QSPI_WPMR_WPEN;
	}

	if (int_enable) {
		ul_mask |= QSPI_WPMR_WPITEN;
	}

	if (control_enable) {
		ul_mask |= QSPI_WPMR_WPCREN;
	}

	qspi->QSPI_WPMR = ul_mask;
}

/**
 * \brief Config qspi according the config struct
 *
 * \param pQspi0        Pointer to an Qspi0 instance.
 * \param qspi_config  Pointer to an qspi_config_t struct.
 * \return status QSPI set config result.
 *
 */
enum status_code qspi_set_config(Qspi0 *qspi, struct qspi_config_t *qspi_config)
{
	enum status_code status = STATUS_OK;

	if (qspi_config->serial_memory_mode == QSPI_MEM_MODE) {
		qspi_set_memory_mode(qspi);
	} else {
		qspi_set_spi_mode(qspi);
	}

	if (qspi_config->wait_data_for_transfer) {
		qspi_enable_wait_data_read_before_transfer(qspi);
	} else {
		qspi_disable_wait_data_read_before_transfer(qspi);
	}

	qspi_set_chip_select_mode(qspi, qspi_config->csmode);
	qspi_set_bits_per_transfer(qspi, qspi_config->bits_per_transfer);
	qspi_set_minimum_inactive_qcs_delay(qspi, qspi_config->min_delay_qcs);
	qspi_set_delay_between_consecutive_transfers(qspi, qspi_config->delay_between_ct);
	qspi_set_clock_polarity(qspi, qspi_config->clock_polarity);
	qspi_set_clock_phase(qspi, qspi_config->clock_phase);
	qspi_set_transfer_delay(qspi, qspi_config->transfer_delay);
	if (qspi_config->scrambling_en) {
		sb_scramble_en = true;
		qspi_set_scrambling_key(qspi, qspi_config->scrambling_user_key);
		qspi_enable_scrambling_mode(qspi, qspi_config->scrambling_random_value_dis,
				qspi_config->scrambling_lock_key);
	} else {
		sb_scramble_en = false;
	}

	return status;
}

/**
 * \brief Config qspi according the config struct
 *
 * \param pQspi0         Pointer to an Qspi0 instance.
 * \param qspi_config   Pointer to an qspi_config_t struct.
 * \return status       QSPI initialize result.
 *
 */
enum status_code qspi_initialize(Qspi0 *qspi, struct qspi_config_t *qspi_config)
{
	enum status_code status = STATUS_OK;

	qspi_disable(qspi);
	qspi_reset(qspi);

	/* Reset Interrupt Status */
	qspi_read_interrupt_status(qspi);

	/** Configure an QSPI peripheral. */
	status = qspi_set_config(qspi, qspi_config);

	/** Update configuration */
	qspi_update_configuration(qspi);

	/** Enable QSPI */
	qspi_enable(qspi);

	return status;
}

/**
 * \brief Get default config
 *
 * \param qspi_config     Pointer to an qspi_config_t struct.
 *
 */
void qspi_get_config_default(struct qspi_config_t *qspi_config)
{
	qspi_config->serial_memory_mode = QSPI_MEM_MODE;
	qspi_config->wait_data_for_transfer = false;
	qspi_config->csmode = QSPI_LASTXFER;
	qspi_config->bits_per_transfer = QSPI_MR_NBBITS_8_BIT;
	qspi_config->min_delay_qcs = 0;
	qspi_config->delay_between_ct = 0;
	qspi_config->clock_polarity = 0;
	qspi_config->clock_phase = 0;
	qspi_config->transfer_delay = 0;
	qspi_config->scrambling_en = false;
	qspi_config->scrambling_random_value_dis = false;
	qspi_config->scrambling_lock_key = false;
	qspi_config->scrambling_user_key = 0;
}

enum status_code qspia_read(Qspi0 *qspi, uint16_t *us_data, uint32_t num_of_bytes)
{
	if (num_of_bytes == 0) {
		return STATUS_OK;
	}

	enum status_code status = OPERATION_IN_PROGRESS;
	uint32_t num_of_bytes_read = 0;
	uint32_t num_of_attempt = 0;
	uint8_t *pw_data = (uint8_t *)us_data;
	uint16_t dummy = 0xFF;

	if (num_of_bytes == 1) {
		for (;;) {
			if (qspi->QSPI_ISR & QSPI_ISR_RDRF) {
				*us_data = qspi_read_spi(qspi);
				qspi_write_spi(qspi, dummy);
				*us_data = qspi_read_spi(qspi);
				num_of_attempt = 0;
				status = STATUS_OK;
				break;
			} else {
				if (num_of_attempt > 0xFFFF) {
					status = ERR_TIME_OUT;
					break;
				} else {
					status = STATUS_ERR_BUSY;
					num_of_attempt++;
				}
			}
		}
	} else {
		/* Dummy read  and write to discard  first bytes received and start receiving new data */
		dummy = qspi_read_spi(qspi);
		qspi_write_spi(qspi, dummy);
		for (; num_of_bytes_read < num_of_bytes;) {
			if (qspi->QSPI_ISR & QSPI_ISR_TDRE) {
				*pw_data = qspi_read_spi(qspi);
				if (qspi->QSPI_MR & QSPI_MR_NBBITS_Msk) {
					pw_data += sizeof(uint16_t);
				} else {
					pw_data += sizeof(uint8_t);
				}

				num_of_bytes_read++;
				num_of_attempt = 0;
				status = STATUS_OK;
				qspi_write_spi(qspi, dummy);
			} else {
				if (num_of_attempt > 0xFFFF) {
					status = ERR_TIME_OUT;
					break;
				} else {
					status = STATUS_ERR_BUSY;
					num_of_attempt++;
				}
			}
		}
	}

	return status;
}

/**
 * \brief Qspi0 write data.
 *
 * \param qspi         Pointer to a Qspi0 instance.
 * \param us_data      Pointer to data to be written.
 * \param num_of_bytes Write data numbers.
 * \return status      Write option result.
 */
enum status_code qspia_write(Qspi0 *qspi, uint16_t *us_data, uint32_t num_of_bytes)
{
	assert(qspi);
	if (num_of_bytes == 0) {
		return STATUS_OK;
	}

	enum status_code status = OPERATION_IN_PROGRESS;
	uint32_t num_of_bytes_write = 0;
	uint32_t num_of_attempt = 0;
	uint8_t *pw_data = (uint8_t *)us_data;
	uint8_t Addr_Inc = 0;

	if (num_of_bytes == 1) {
		for (;;) {
			if (qspi->QSPI_ISR & QSPI_ISR_TDRE) {
				qspi_write_spi(qspi, (uint16_t)(*us_data));
				num_of_attempt = 0;
				status = STATUS_OK;
				break;
			} else {
				status = STATUS_ERR_BUSY;
				num_of_attempt++;
				if (num_of_attempt > 0xFFFF) {
					status = ERR_TIME_OUT;
					break;
				}
			}
		}
	} else {
		if (qspi->QSPI_MR & QSPI_MR_NBBITS_Msk) {
			Addr_Inc = sizeof(uint16_t);
		} else {
			Addr_Inc = sizeof(uint8_t);
		}

		for (; num_of_bytes_write < num_of_bytes; num_of_bytes_write++) {
			if (qspi->QSPI_ISR & QSPI_ISR_TXEMPTY) {
				qspi_write_spi(qspi, (uint16_t)(*pw_data));
				pw_data += Addr_Inc;
				num_of_attempt = 0;
				status = STATUS_OK;
			} else {
				status = STATUS_ERR_BUSY;
				num_of_attempt++;
				if (num_of_attempt > 0xFFFF) {
					status = ERR_TIME_OUT;
					break;
				}
			}
		}
	}

	return status;
}

/**
 * \brief Send instruction over QSPI with data
 *
 * \param qspid        Pointer to an Qspi0 instance.
 * \param read_write   Flag to indicate read/write QSPI memory access
 *
 * \return Returns 1 if At least one instruction end has been detected since the last read of QSPI_SR.; otherwise
 * returns 0.
 */
enum status_code qspi_flash_execute_command(struct qspid_t *qspid, enum qspi_access read_write)
{
	struct qspi_inst_frame_t *const frame = qspid->qspi_frame;
	struct qspi_mem_cmd_t command = qspid->qspi_command;
	enum status_code status = OPERATION_IN_PROGRESS;
	Qspi0 *qspi = qspid->qspi_hw;
	struct qspi_buffer_t buffer = qspid->qspi_buffer;
	uint32_t bytes_counter;
	uint8_t *data_ptr;

	/* Instruction frame with address ? */
	if (frame->inst_frame.bm.b_addr_en) {
		/* Write Address */
		qspi_set_instruction_addr(qspi, frame->addr);
	}

	if (frame->inst_frame.bm.b_inst_en) {
		if (read_write == QSPI_READ_ACCESS) {
			/* Write Instruction code in QSPI_RICR */
			qspi_set_read_instruction_code(qspi, command);
		} else if (read_write == QSPI_WRITE_ACCESS) {
			/* Write Instruction code QSPI_WICR */
			qspi_set_write_instruction_code(qspi, command);
		} else {
			if (frame->inst_frame.bm.b_apb_tfr_type) {
				/* Read Trasnfer : Write Instruction code in QSPI_RICR */
				qspi_set_read_instruction_code(qspi, command);
			} else {
				/* Write Trasnfer : Write Instruction code QSPI_WICR */
				qspi_set_write_instruction_code(qspi, command);
			}
		}
	}

	/* Set Instruction Frame */
	qspi_set_instruction_frame(qspi, *frame);
	/* Update configuration */
	qspi_update_configuration(qspi);

	/* Instruction frame with data ? */
	if (frame->inst_frame.bm.b_data_en) {
		/* Read Frame ? */
		if (frame->inst_frame.bm.b_apb_tfr_type) {
			/* Read Frame */
			/* Init loop vars */
			bytes_counter = buffer.rx_data_size;
			data_ptr = (uint8_t *)buffer.data_rx;
			/* Start Transfer */
			qspi_start_trasnfer(qspi);
			while (bytes_counter--) {
				/* Wait RDRF */
				while (!(qspi_read_interrupt_status(qspi) & QSPI_ISR_RDRF)) {
				}
				/* Last data ? */
				if (bytes_counter == 0) {
					/* Set Last Transfer */
					qspi_set_lastxfer(qspi);
				}

				/* Read Data */
				*data_ptr++ = (uint8_t)qspi_read_spi(qspi);
			}
		} else {
			/* Write Frame */
			/* Init loop vars */
			bytes_counter = buffer.tx_data_size;
			data_ptr = (uint8_t *)buffer.data_tx;
			/* Start Transfer */
			while (bytes_counter--) {
				/* Wait TDRE */
				while (!(qspi_read_interrupt_status(qspi) & QSPI_ISR_TDRE)) {
				}
				/* Write Data */
				qspi_write_spi(qspi, *data_ptr++);
			}
			/* Wait TXEMPTY */
			while (!(qspi_read_interrupt_status(qspi) & QSPI_ISR_TXEMPTY)) {
			}
			/* Set Last Transfer */
			qspi_set_lastxfer(qspi);
		}
	} else {
		/* Start Transfer */
		qspi_start_trasnfer(qspi);
	}

	/* Wait CSRA */
	while (!(qspi_read_interrupt_status(qspi) & QSPI_ISR_CSRA)) {
	}

	frame->inst_frame.val = 0;
	status = STATUS_OK;

	return status;
}

/**
 * \brief Writes or reads the QSPI memory (0x04000000) to trasmit or receive data from Flash memory
 * \param qspid          Pointer to an Qspi0 instance.
 * \param read_write     Flag to indicate read/write QSPI memory access
 * \param scramble_flag  Enable or disable scramble on QSPI
 * \param aesb_flag      Enable or disable AES bridge
 *
 * \return Returns 1 if At least one instruction end has been detected since the last read of QSPI_SR.; otherwise
 * returns 0.
 */
enum status_code qspi_flash_access_memory(struct qspid_t *qspid, enum qspi_access read_write, uint8_t scramble_flag, uint8_t aesb_flag)
{
	enum status_code status = OPERATION_IN_PROGRESS;
	struct qspi_inst_frame_t *const frame = qspid->qspi_frame;
	struct qspi_mem_cmd_t command = qspid->qspi_command;
	uint32_t *qspi_mem;
	struct qspi_buffer_t *buffer = &qspid->qspi_buffer;
	Qspi0 *qspi = qspid->qspi_hw;

	if (read_write == QSPI_READ_ACCESS) {
		qspi_set_read_instruction_code(qspi, command);
	} else {
		qspi_set_number_of_write_accesses(qspi, buffer->tx_data_size);
		qspi_set_write_instruction_code(qspi, command);
	}

	/* Set Instruction Frame */
	qspi_set_instruction_frame(qspi, *frame);

	/* Configure Scrambling */
	if (scramble_flag && sb_scramble_en) {
		qspi_enable_scrambling_mode(qspi, scramble_flag, 0);
	}

	/* Update configuration */
	qspi_update_configuration(qspi);

	/* Set memory address depending on AES bridge */
	if (!aesb_flag) {
		qspi_mem = (uint32_t *)(QSPI_MEM_CNC_ADDR | frame->addr);
	} else {
		qspi_mem = (uint32_t *)(QSPI_AESB_CNC_ADDR | frame->addr);
	}

	if (read_write == QSPI_WRITE_ACCESS) {
		/* Data Write */
		_qspi_memcpy((uint32_t *)qspi_mem, (uint32_t *)buffer->data_tx, buffer->tx_data_size);

		/* Wait LWRA */
		while (!(qspi_read_interrupt_status(qspi) & QSPI_ISR_LWRA)) {
		}
	} else {
		/* Data Read */
		_qspi_memcpy((uint32_t *)buffer->data_rx, (uint32_t *)qspi_mem, buffer->rx_data_size);

		/* Wait RBUSY */
		while (qspi_is_busy(qspi)) {
		}
	}

	/* Set Last Transfer */
	qspi_set_lastxfer(qspi);

	/* Wait CSRA */
	while (!(qspi_read_interrupt_status(qspi) & QSPI_ISR_CSRA)) {
	}

	status = STATUS_OK;
	return status;
}
