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

#include "pic32cx.h"
#include "twi.h"

/// @cond 0
/**INDENT-OFF**/
#ifdef __cplusplus
extern "C" {
#endif
/**INDENT-ON**/
/// @endcond

/**
 * \defgroup sam_drivers_twi_group Two-Wire Interface (TWI)
 *
 * Driver for the TWI (Two-Wire Interface). This driver provides access to the main
 * features of the TWI controller.
 * The TWI interconnects components on a unique two-wire bus.
 * The TWI is programmable as a master or a slave with sequential or single-byte access.
 * Multiple master capability is supported.
 *
 * \par Usage
 *
 * -# Enable the TWI peripheral clock in the PMC.
 * -# Enable the required TWI PIOs (see pio.h).
 * -# Enable TWI master mode by calling twi_enable_master_mode if it is a master on the I2C bus.
 * -# Configure the TWI in master mode by calling twi_master_init.
 * -# Send data to a slave device on the I2C bus by calling twi_master_write.
 * -# Receive data from a slave device on the I2C bus by calling the twi_master_read.
 * -# Enable TWI slave mode by calling twi_enable_slave_mode if it is a slave on the I2C bus.
 * -# Configure the TWI in slave mode by calling twi_slave_init.
 *
 * @{
 */

/* Low level time limit of I2C Fast Mode. */
#define LOW_LEVEL_TIME_LIMIT 384000
#define I2C_FAST_MODE_SPEED  400000
#define TWI_CLK_DIVIDER      2
#define TWI_CLK_CALC_ARGU    3
#define TWI_CLK_DIV_MAX      0xFF
#define TWI_CLK_DIV_MIN      7

#define TWI_WP_KEY_VALUE TWI_WPMR_WPKEY_PASSWD

/**
 * \brief Enable TWI master mode.
 *
 * \param p_twi Pointer to a TWI instance.
 */
void twi_enable_master_mode(Twi *p_twi)
{
	/* Set Master Disable bit and Slave Disable bit */
	p_twi->TWI_CR = TWI_CR_MSDIS;
	p_twi->TWI_CR = TWI_CR_SVDIS;

	/* Set Master Enable bit */
	p_twi->TWI_CR = TWI_CR_MSEN;
}

/**
 * \brief Disable TWI master mode.
 *
 * \param p_twi Pointer to a TWI instance.
 */
void twi_disable_master_mode(Twi *p_twi)
{
	/* Set Master Disable bit */
	p_twi->TWI_CR = TWI_CR_MSDIS;
}

/**
 * \brief Initialize TWI master mode.
 *
 * \param p_twi Pointer to a TWI instance.
 * \param p_opt Options for initializing the TWI module (see \ref twi_options_t).
 *
 * \return TWI_SUCCESS if initialization is complete, error code otherwise.
 */
uint32_t twi_master_init(Twi *p_twi, const twi_options_t *p_opt)
{
	uint32_t status = TWI_SUCCESS;

	/* Disable TWI interrupts */
	p_twi->TWI_IDR = ~0UL;

	/* Dummy read in status register */
	p_twi->TWI_SR;

	/* Reset TWI peripheral */
	twi_reset(p_twi);

	twi_enable_master_mode(p_twi);

	/* Select the speed */
	if (twi_set_speed(p_twi, p_opt->speed, p_opt->master_clk) == FAIL) {
		/* The desired speed setting is rejected */
		status = TWI_INVALID_ARGUMENT;
	}

	if (p_opt->smbus == 1) {
		p_twi->TWI_CR = TWI_CR_QUICK;
	}

	return status;
}

/**
 * \brief Set the I2C bus speed in conjunction with the clock frequency.
 *
 * \param p_twi Pointer to a TWI instance.
 * \param ul_speed The desired I2C bus speed (in Hz).
 * \param ul_mck Main clock of the device (in Hz).
 *
 * \retval PASS New speed setting is accepted.
 * \retval FAIL New speed setting is rejected.
 */
uint32_t twi_set_speed(Twi *p_twi, uint32_t ul_speed, uint32_t ul_mck)
{
	uint32_t ckdiv = 0;
	uint32_t c_lh_div;
	uint32_t cldiv, chdiv;

	if (ul_speed > I2C_FAST_MODE_SPEED) {
		return FAIL;
	}

	/* Low level time not less than 1.3us of I2C Fast Mode. */
	if (ul_speed > LOW_LEVEL_TIME_LIMIT) {
		/* Low level of time fixed for 1.3us. */
		cldiv = ul_mck / (LOW_LEVEL_TIME_LIMIT * TWI_CLK_DIVIDER) - TWI_CLK_CALC_ARGU;
		chdiv = ul_mck / ((ul_speed + (ul_speed - LOW_LEVEL_TIME_LIMIT)) * TWI_CLK_DIVIDER) - TWI_CLK_CALC_ARGU;

		/* cldiv must fit in 8 bits, ckdiv must fit in 3 bits */
		while ((cldiv > TWI_CLK_DIV_MAX) && (ckdiv < TWI_CLK_DIV_MIN)) {
			/* Increase clock divider */
			ckdiv++;
			/* Divide cldiv value */
			cldiv /= TWI_CLK_DIVIDER;
		}
		/* chdiv must fit in 8 bits, ckdiv must fit in 3 bits */
		while ((chdiv > TWI_CLK_DIV_MAX) && (ckdiv < TWI_CLK_DIV_MIN)) {
			/* Increase clock divider */
			ckdiv++;
			/* Divide cldiv value */
			chdiv /= TWI_CLK_DIVIDER;
		}

		/* set clock waveform generator register */
		p_twi->TWI_CWGR =
				TWI_CWGR_CLDIV(cldiv) | TWI_CWGR_CHDIV(chdiv) |
				TWI_CWGR_CKDIV(ckdiv);
	} else {
		c_lh_div = ul_mck / (ul_speed * TWI_CLK_DIVIDER) - TWI_CLK_CALC_ARGU;

		/* cldiv must fit in 8 bits, ckdiv must fit in 3 bits */
		while ((c_lh_div > TWI_CLK_DIV_MAX) && (ckdiv < TWI_CLK_DIV_MIN)) {
			/* Increase clock divider */
			ckdiv++;
			/* Divide cldiv value */
			c_lh_div /= TWI_CLK_DIVIDER;
		}

		/* set clock waveform generator register */
		p_twi->TWI_CWGR =
				TWI_CWGR_CLDIV(c_lh_div) | TWI_CWGR_CHDIV(c_lh_div) |
				TWI_CWGR_CKDIV(ckdiv);
	}

	return PASS;
}

/**
 * \brief Test if a chip answers a given I2C address.
 *
 * \param p_twi Pointer to a TWI instance.
 * \param uc_slave_addr Address of the remote chip to search for.
 *
 * \return TWI_SUCCESS if a chip was found, error code otherwise.
 */
uint32_t twi_probe(Twi *p_twi, uint8_t uc_slave_addr)
{
	twi_packet_t packet;
	uint8_t data = 0;

	/* Data to send */
	packet.buffer = &data;
	/* Data length */
	packet.length = 1;
	/* Slave chip address */
	packet.chip = (uint32_t) uc_slave_addr;
	/* Internal chip address */
	packet.addr[0] = 0;
	/* Address length */
	packet.addr_length = 0;

	/* Perform a master write access */
	return (twi_master_write(p_twi, &packet));
}


/**
 * \internal
 * \brief Construct the TWI module address register field
 *
 * The TWI module address register is sent out MSB first. And the size controls
 * which byte is the MSB to start with.
 *
 * Please see the device datasheet for details on this.
 */
uint32_t twi_mk_addr(const uint8_t *addr, int len)
{
	uint32_t val;

	if (len == 0)
		return 0;

	val = addr[0];
	if (len > 1) {
		val <<= 8;
		val |= addr[1];
	}
	if (len > 2) {
		val <<= 8;
		val |= addr[2];
	}
	return val;
}

/**
 * \brief Read multiple bytes from a TWI compatible slave device.
 *
 * \note This function will NOT return until all data has been read or error occurs.
 *
 * \param p_twi Pointer to a TWI instance.
 * \param p_packet Packet information and data (see \ref twi_packet_t).
 *
 * \return TWI_SUCCESS if all bytes were read, error code otherwise.
 */
uint32_t twi_master_read(Twi *p_twi, twi_packet_t *p_packet)
{
	uint32_t status;
	uint32_t cnt = p_packet->length;
	uint8_t *buffer = p_packet->buffer;
	uint8_t stop_sent = 0;
	uint32_t timeout = TWI_TIMEOUT;;

	/* Check argument */
	if (cnt == 0) {
		return TWI_INVALID_ARGUMENT;
	}

	/* Set read mode, slave address and 3 internal address byte lengths */
	p_twi->TWI_MMR = 0;
	p_twi->TWI_MMR = TWI_MMR_MREAD | TWI_MMR_DADR(p_packet->chip) |
			((p_packet->addr_length << TWI_MMR_IADRSZ_Pos) &
			TWI_MMR_IADRSZ_Msk);

	/* Set internal address for remote chip */
	p_twi->TWI_IADR = 0;
	p_twi->TWI_IADR = twi_mk_addr(p_packet->addr, p_packet->addr_length);

	/* Send a START condition */
	if (cnt == 1) {
		p_twi->TWI_CR = TWI_CR_START | TWI_CR_STOP;
		stop_sent = 1;
	} else {
		p_twi->TWI_CR = TWI_CR_START;
		stop_sent = 0;
	}

	while (cnt > 0) {
		status = p_twi->TWI_SR;
		if (status & TWI_SR_NACK) {
			return TWI_RECEIVE_NACK;
		}

		if (!timeout--) {
			return TWI_ERROR_TIMEOUT;
		}

		/* Last byte ? */
		if (cnt == 1  && !stop_sent) {
			p_twi->TWI_CR = TWI_CR_STOP;
			stop_sent = 1;
		}

		if (!(status & TWI_SR_RXRDY)) {
			continue;
		}
		*buffer++ = (p_twi->TWI_RHR & TWI_RHR_RXDATA_Msk);

		cnt--;
		timeout = TWI_TIMEOUT;
	}

	while (!(p_twi->TWI_SR & TWI_SR_TXCOMP)) {
	}

	p_twi->TWI_SR;

	return TWI_SUCCESS;
}

/**
 * \brief Write multiple bytes to a TWI compatible slave device.
 *
 * \note This function will NOT return until all data has been written or error occurred.
 *
 * \param p_twi Pointer to a TWI instance.
 * \param p_packet Packet information and data (see \ref twi_packet_t).
 *
 * \return TWI_SUCCESS if all bytes were written, error code otherwise.
 */
uint32_t twi_master_write(Twi *p_twi, twi_packet_t *p_packet)
{
	uint32_t status;
	uint32_t cnt = p_packet->length;
	uint8_t *buffer = p_packet->buffer;

	/* Check argument */
	if (cnt == 0) {
		return TWI_INVALID_ARGUMENT;
	}

	/* Set write mode, slave address and 3 internal address byte lengths */
	p_twi->TWI_MMR = 0;
	p_twi->TWI_MMR = TWI_MMR_DADR(p_packet->chip) |
			((p_packet->addr_length << TWI_MMR_IADRSZ_Pos) &
			TWI_MMR_IADRSZ_Msk);

	/* Set internal address for remote chip */
	p_twi->TWI_IADR = 0;
	p_twi->TWI_IADR = twi_mk_addr(p_packet->addr, p_packet->addr_length);

	/* Send all bytes */
	while (cnt > 0) {
		status = p_twi->TWI_SR;
		if (status & TWI_SR_NACK) {
			return TWI_RECEIVE_NACK;
		}

		if (!(status & TWI_SR_TXRDY)) {
			continue;
		}
		p_twi->TWI_THR = *buffer++;

		cnt--;
	}

	while (1) {
		status = p_twi->TWI_SR;
		if (status & TWI_SR_NACK) {
			return TWI_RECEIVE_NACK;
		}

		if (status & TWI_SR_TXRDY) {
			break;
		}
	}

	p_twi->TWI_CR = TWI_CR_STOP;

	while (!(p_twi->TWI_SR & TWI_SR_TXCOMP)) {
	}

	return TWI_SUCCESS;
}

/**
 * \brief Enable TWI interrupts.
 *
 * \param p_twi Pointer to a TWI instance.
 * \param ul_sources Interrupts to be enabled.
 */
void twi_enable_interrupt(Twi *p_twi, uint32_t ul_sources)
{
	/* Enable the specified interrupts */
	p_twi->TWI_IER = ul_sources;
}

/**
 * \brief Disable TWI interrupts.
 *
 * \param p_twi Pointer to a TWI instance.
 * \param ul_sources Interrupts to be disabled.
 */
void twi_disable_interrupt(Twi *p_twi, uint32_t ul_sources)
{
	/* Disable the specified interrupts */
	p_twi->TWI_IDR = ul_sources;
	/* Dummy read */
	p_twi->TWI_SR;
}

/**
 * \brief Get TWI interrupt status.
 *
 * \param p_twi Pointer to a TWI instance.
 *
 * \retval TWI interrupt status.
 */
uint32_t twi_get_interrupt_status(Twi *p_twi)
{
	return p_twi->TWI_SR;
}

/**
 * \brief Read TWI interrupt mask.
 *
 * \param p_twi Pointer to a TWI instance.
 *
 * \return The interrupt mask value.
 */
uint32_t twi_get_interrupt_mask(Twi *p_twi)
{
	return p_twi->TWI_IMR;
}

/**
 * \brief Reads a byte from the TWI bus.
 *
 * \param p_twi Pointer to a TWI instance.
 *
 * \return The byte read.
 */
uint8_t twi_read_byte(Twi *p_twi)
{
	return (p_twi->TWI_RHR & TWI_RHR_RXDATA_Msk);
}

/**
 * \brief Sends a byte of data to one of the TWI slaves on the bus.
 *
 * \param p_twi Pointer to a TWI instance.
 * \param byte The byte to send.
 */
void twi_write_byte(Twi *p_twi, uint8_t uc_byte)
{
	p_twi->TWI_THR = uc_byte;
}

/**
 * \brief Enable TWI slave mode.
 *
 * \param p_twi Pointer to a TWI instance.
 */
void twi_enable_slave_mode(Twi *p_twi)
{
	/* Set Master Disable bit and Slave Disable bit */
	p_twi->TWI_CR = TWI_CR_MSDIS;
	p_twi->TWI_CR = TWI_CR_SVDIS;

	/* Set Slave Enable bit */
	p_twi->TWI_CR = TWI_CR_SVEN;
}

/**
 * \brief Disable TWI slave mode.
 *
 * \param p_twi Pointer to a TWI instance.
 */
void twi_disable_slave_mode(Twi *p_twi)
{
	/* Set Slave Disable bit */
	p_twi->TWI_CR = TWI_CR_SVDIS;
}

/**
 * \brief Initialize TWI slave mode.
 *
 * \param p_twi Pointer to a TWI instance.
 * \param ul_device_addr Device address of the SAM slave device on the I2C bus.
 */
void twi_slave_init(Twi *p_twi, uint32_t ul_device_addr)
{
	/* Disable TWI interrupts */
	p_twi->TWI_IDR = ~0UL;
	p_twi->TWI_SR;

	/* Reset TWI */
	twi_reset(p_twi);

	/* Set slave address in slave mode */
	p_twi->TWI_SMR = TWI_SMR_SADR(ul_device_addr);

	/* Enable slave mode */
	twi_enable_slave_mode(p_twi);
}

/**
 * \brief Set TWI slave address.
 *
 * \param p_twi Pointer to a TWI instance.
 * \param ul_device_addr Device address of the SAM slave device on the I2C bus.
 */
void twi_set_slave_addr(Twi *p_twi, uint32_t ul_device_addr)
{
	/* Set slave address */
	p_twi->TWI_SMR = TWI_SMR_SADR(ul_device_addr);
}

/**
 * \brief Read data from master.
 *
 * \note This function will NOT return until master sends a STOP condition.
 *
 * \param p_twi Pointer to a TWI instance.
 * \param p_data Pointer to the data buffer where data received will be stored.
 *
 * \return Number of bytes read.
 */
uint32_t twi_slave_read(Twi *p_twi, uint8_t *p_data)
{
	uint32_t status, cnt = 0;

	do {
		status = p_twi->TWI_SR;
		if (status & TWI_SR_SVACC) {
			if (!(status & TWI_SR_GACC) &&
				((status & (TWI_SR_SVREAD | TWI_SR_RXRDY))
				 == (TWI_SR_SVREAD | TWI_SR_RXRDY))) {
				*p_data++ = (uint8_t) (p_twi->TWI_RHR & TWI_RHR_RXDATA_Msk);
				cnt++;
			}
		} else if ((status & (TWI_SR_EOSACC | TWI_SR_TXCOMP))
					== (TWI_SR_EOSACC | TWI_SR_TXCOMP)) {
			break;
		}
	} while (1);

	return cnt;
}

/**
 * \brief Write data to TWI bus.
 *
 * \note This function will NOT return until master sends a STOP condition.
 *
 * \param p_twi Pointer to a TWI instance.
 * \param p_data Pointer to the data buffer to be sent.
 *
 * \return Number of bytes written.
 */
uint32_t twi_slave_write(Twi *p_twi, uint8_t *p_data)
{
	uint32_t status, cnt = 0;

	do {
		status = p_twi->TWI_SR;
		if (status & TWI_SR_SVACC) {
			if (!(status & (TWI_SR_GACC | TWI_SR_SVREAD)) &&
				(status & TWI_SR_TXRDY)) {
				p_twi->TWI_THR = *p_data++;
				cnt++;
			}
		} else if ((status & (TWI_SR_EOSACC | TWI_SR_TXCOMP))
					== (TWI_SR_EOSACC | TWI_SR_TXCOMP)) {
			break;
		}
	} while (1);

	return cnt;
}

/**
 * \brief Reset TWI.
 *
 * \param p_twi Pointer to a TWI instance.
 */
void twi_reset(Twi *p_twi)
{
	/* Set SWRST bit to reset TWI peripheral */
	p_twi->TWI_CR = TWI_CR_SWRST;
	p_twi->TWI_RHR;
}

/**
 * \brief Get TWI PDC base address.
 *
 * \param p_twi Pointer to a TWI instance.
 *
 * \return TWI PDC registers base for PDC driver to access.
 */
Pdc *twi_get_pdc_base(Twi *p_twi)
{
	Pdc *p_pdc_base = NULL;
	if (p_twi == TWI0) {
		p_pdc_base = PDC_TWI0;
	} else if (p_twi == TWI1) {
		p_pdc_base = PDC_TWI1;
	} else if (p_twi == TWI2) {
		p_pdc_base = PDC_TWI2;
	} else if (p_twi == TWI3) {
		p_pdc_base = PDC_TWI3;
	} else if (p_twi == TWI4) {
		p_pdc_base = PDC_TWI4;
	} else if (p_twi == TWI5) {
		p_pdc_base = PDC_TWI5;
	} else if (p_twi == TWI6) {
		p_pdc_base = PDC_TWI6;
	} else if (p_twi == TWI7) {
		p_pdc_base = PDC_TWI7;
	} else {
		Assert(false);
	}

	return p_pdc_base;
}

/**
 * \brief Set the prescaler, TLOW:SEXT, TLOW:MEXT and clock high max cycles for SMBUS mode.
 *
 * \param p_twi   Base address of the TWI instance.
 * \param ul_timing Parameter for prescaler, TLOW:SEXT, TLOW:MEXT and clock high max cycles.
 */
void twi_smbus_set_timing(Twi *p_twi, uint32_t ul_timing)
{
	p_twi->TWI_SMBTR = ul_timing;;
}

/**
 * \brief Set length/direction/PEC for alternative command mode.
 *
 * \param p_twi   Base address of the TWI instance.
 * \param ul_alt_cmd Alternative command parameters.
 */
void twi_set_alternative_command(Twi *p_twi, uint32_t ul_alt_cmd)
{
	p_twi->TWI_ACR = ul_alt_cmd;;
}

/**
 * \brief Set the filter for TWI.
 *
 * \param p_twi   Base address of the TWI instance.
 * \param ul_filter   Filter value.
 */
void twi_set_filter(Twi *p_twi, uint32_t ul_filter)
{
	p_twi->TWI_FILTR = ul_filter;;
}

/**
 * \brief A mask can be applied on the slave device address in slave mode in order to allow multiple
 * address answer. For each bit of the MASK field set to one the corresponding SADR bit will be masked.
 *
 * \param p_twi   Base address of the TWI instance.
 * \param ul_mask  Mask value.
 */
void twi_mask_slave_addr(Twi *p_twi, uint32_t ul_mask)
{
	p_twi->TWI_SMR |= TWI_SMR_MASK(ul_mask);
}

/**
 * \brief Set sleepwalking match mode.
 *
 * \param p_twi Pointer to a TWI instance.
 * \param ul_matching_addr1   Address 1 value.
 * \param ul_matching_addr2   Address 2 value.
 * \param ul_matching_addr3   Address 3 value.
 * \param ul_matching_data   Data value.
 * \param flag1 ture for set, false for no.
 * \param flag2 ture for set, false for no.
 * \param flag3 ture for set, false for no.
 * \param flag ture for set, false for no.
 */
void twi_set_sleepwalking(Twi *p_twi,
		uint32_t ul_matching_addr1, bool flag1,
		uint32_t ul_matching_addr2, bool flag2,
		uint32_t ul_matching_addr3, bool flag3,
		uint32_t ul_matching_data, bool flag)
{
	uint32_t temp = 0;

	if (flag1) {
		temp |= TWI_SWMR_SADR1(ul_matching_addr1);
	}

	if (flag2) {
		temp |= TWI_SWMR_SADR2(ul_matching_addr2);
	}

	if (flag3) {
		temp |= TWI_SWMR_SADR3(ul_matching_addr3);
	}

	if (flag) {
		temp |= TWI_SWMR_DATAM(ul_matching_data);
	}

	p_twi->TWI_SWMR = temp;
}

/**
 * \brief Enable TWI FIFO and Master mode.
 *
 * \param p_twi Pointer to a TWI instance.
 */
void twi_enable_fifo_master(Twi *p_twi)
{
	/* Disable Master and Slave. */
	p_twi->TWI_CR = TWI_CR_MSDIS | TWI_CR_SVDIS;

	/* Enable FIFO. */
	p_twi->TWI_CR = SPI_CR_FIFOEN;

	/* Reenable Master. */
	p_twi->TWI_CR = TWI_CR_MSEN;
}

/**
 * \brief Enable TWI FIFO and Slave mode.
 *
 * \param p_twi Pointer to a TWI instance.
 */
void twi_enable_fifo_slave(Twi *p_twi)
{
	/* Disable Master and Slave. */
	p_twi->TWI_CR = TWI_CR_MSDIS | TWI_CR_SVDIS;

	/* Enable FIFO. */
	p_twi->TWI_CR = SPI_CR_FIFOEN;

	/* Reenable Slave. */
	p_twi->TWI_CR = TWI_CR_SVEN;
}

/**
 * \brief Disable TWI FIFO and Enable Master mode.
 *
 * \param p_twi Pointer to a TWI instance.
 */
void twi_disable_fifo_master(Twi *p_twi)
{
	/* Disable Master and Slave. */
	p_twi->TWI_CR = TWI_CR_MSDIS | TWI_CR_SVDIS;

	/* Disable FIFO. */
	p_twi->TWI_CR = TWI_CR_FIFODIS;

	/* Reenable Master. */
	p_twi->TWI_CR = TWI_CR_MSEN;
}

/**
 * \brief Disable TWI FIFO and Enable Slave mode.
 *
 * \param p_twi Pointer to a TWI instance.
 */
void twi_disable_fifo_slave(Twi *p_twi)
{
	/* Disable Master and Slave. */
	p_twi->TWI_CR = TWI_CR_MSDIS | TWI_CR_SVDIS;

	/* Disable FIFO. */
	p_twi->TWI_CR = TWI_CR_FIFODIS;

	/* Reenable Slave. */
	p_twi->TWI_CR = TWI_CR_SVEN;
}

/**
 * \brief Set TWI Sniffer mode.
 *
 * \param p_twi Pointer to a TWI instance.
 * \param ul_device_addr Device address to sniff on the I2C bus.
 * \param ul_mask Address Mask for multiple address sniffing.
 * \param b_bus_selection Flag to set the consecutive TWI peripheral for non-intrusive sniffing.
 */
void twi_set_sniffer_mode(Twi *p_twi, uint32_t ul_device_addr,
		uint32_t ul_mask, bool b_bus_selection)
{
	uint32_t ul_smr_value;

	/* Disable Master and Slave. */
	p_twi->TWI_CR = TWI_CR_MSDIS | TWI_CR_SVDIS;

	/* Get SMR register value */
	ul_smr_value = p_twi->TWI_SMR;

	/* Configure with received parameters */
	ul_smr_value &= ~(TWI_SMR_SADR_Msk | TWI_SMR_MASK_Msk | TWI_SMR_BSEL);
	ul_smr_value |= (TWI_SMR_SADR(ul_device_addr) | TWI_SMR_MASK(ul_mask));
	if (b_bus_selection) {
		ul_smr_value |= TWI_SMR_BSEL;
	}
	ul_smr_value |= TWI_SMR_SNIFF;

	/* Enable Slave. */
	p_twi->TWI_CR = TWI_CR_SVEN;
}

/**
 * \brief Read multiple bytes from a TWI compatible slave device using FIFO.
 *
 * \note This function will NOT return until all data has been read or error occurs.
 *
 * \param p_twi Pointer to a TWI instance.
 * \param p_packet Packet information and data (see \ref twi_packet_t).
 *
 * \return TWI_SUCCESS if all bytes were read, error code otherwise.
 */
uint32_t twi_master_read_fifo(Twi *p_twi, twi_packet_t *p_packet)
{
	uint32_t status;
	uint32_t cnt = p_packet->length;
	uint8_t *buffer = p_packet->buffer;
	uint8_t stop_sent = 0;
	uint32_t timeout = TWI_TIMEOUT;

	/* Check argument */
	if (cnt == 0) {
		return TWI_INVALID_ARGUMENT;
	}

	/* Enable FIFO */
	twi_enable_fifo_master(p_twi);

	/* Set read mode, slave address and 3 internal address byte lengths */
	p_twi->TWI_MMR = 0;
	p_twi->TWI_MMR = TWI_MMR_MREAD | TWI_MMR_DADR(p_packet->chip) |
			((p_packet->addr_length << TWI_MMR_IADRSZ_Pos) &
			TWI_MMR_IADRSZ_Msk);

	/* Set internal address for remote chip */
	p_twi->TWI_IADR = 0;
	p_twi->TWI_IADR = twi_mk_addr(p_packet->addr, p_packet->addr_length);

	/* Send a START condition */
	if (cnt == 1) {
		p_twi->TWI_CR = TWI_CR_START | TWI_CR_STOP;
		stop_sent = 1;
	} else {
		p_twi->TWI_CR = TWI_CR_START;
		stop_sent = 0;
	}

	if (cnt > 4) {
		/* Configure 4 data reception */
		p_twi->TWI_FMR &= ~TWI_FMR_RXRDYM_Msk;
		p_twi->TWI_FMR |= TWI_FMR_RXRDYM_FOUR_DATA;
	}
	else if (cnt > 2) {
		/* Configure 2 data reception */
		p_twi->TWI_FMR &= ~TWI_FMR_RXRDYM_Msk;
		p_twi->TWI_FMR |= TWI_FMR_RXRDYM_TWO_DATA;
	}
	else {
		/* Configure 1 data reception */
		p_twi->TWI_FMR &= ~TWI_FMR_RXRDYM_Msk;
		p_twi->TWI_FMR |= TWI_FMR_RXRDYM_ONE_DATA;
	}

	while (cnt > 0) {
		status = p_twi->TWI_SR;
		if (status & TWI_SR_NACK) {
			return TWI_RECEIVE_NACK;
		}

		if (!timeout--) {
			return TWI_ERROR_TIMEOUT;
		}

		/* Last byte ? */
		if (cnt == 1  && !stop_sent) {
			p_twi->TWI_CR = TWI_CR_STOP;
			stop_sent = 1;
		}

		if (!(status & TWI_SR_RXRDY)) {
			continue;
		}

		if (cnt > 4) {
			*buffer++ = (p_twi->TWI_RHR & TWI_RHR_RXDATA0_Msk) >> TWI_RHR_RXDATA0_Pos;
			*buffer++ = (p_twi->TWI_RHR & TWI_RHR_RXDATA1_Msk) >> TWI_RHR_RXDATA1_Pos;
			*buffer++ = (p_twi->TWI_RHR & TWI_RHR_RXDATA2_Msk) >> TWI_RHR_RXDATA2_Pos;
			*buffer++ = (p_twi->TWI_RHR & TWI_RHR_RXDATA3_Msk) >> TWI_RHR_RXDATA3_Pos;
			cnt -= 4;
		}
		else if (cnt > 2) {
			*buffer++ = (p_twi->TWI_RHR & TWI_RHR_RXDATA0_Msk) >> TWI_RHR_RXDATA0_Pos;
			*buffer++ = (p_twi->TWI_RHR & TWI_RHR_RXDATA1_Msk) >> TWI_RHR_RXDATA1_Pos;
			cnt -= 2;
		}
		else {
			*buffer++ = (p_twi->TWI_RHR & TWI_RHR_RXDATA0_Msk) >> TWI_RHR_RXDATA0_Pos;
			cnt--;
		}

		if (cnt > 4) {
			/* Configure 4 data reception */
			p_twi->TWI_FMR &= ~TWI_FMR_RXRDYM_Msk;
			p_twi->TWI_FMR |= TWI_FMR_RXRDYM_FOUR_DATA;
		}
		else if (cnt > 2) {
			/* Configure 2 data reception */
			p_twi->TWI_FMR &= ~TWI_FMR_RXRDYM_Msk;
			p_twi->TWI_FMR |= TWI_FMR_RXRDYM_TWO_DATA;
		}
		else {
			/* Configure 1 data reception */
			p_twi->TWI_FMR &= ~TWI_FMR_RXRDYM_Msk;
			p_twi->TWI_FMR |= TWI_FMR_RXRDYM_ONE_DATA;
		}

		timeout = TWI_TIMEOUT;
	}

	while (!(p_twi->TWI_SR & TWI_SR_TXCOMP)) {
	}

	p_twi->TWI_SR;

	return TWI_SUCCESS;
}

/**
 * \brief Read data from master using FIFO.
 *
 * \note This function will NOT return until master sends a STOP condition.
 *
 * \param p_twi Pointer to a TWI instance.
 * \param p_data Pointer to the data buffer where data received will be stored.
 *
 * \return Number of bytes read.
 */
uint32_t twi_slave_read_fifo(Twi *p_twi, uint8_t *p_data)
{
	uint32_t status, fifo_level, cnt = 0;

	/* Enable FIFO */
	twi_enable_fifo_slave(p_twi);

	do {
		status = p_twi->TWI_SR;
		if (status & TWI_SR_SVACC) {
			if (!(status & TWI_SR_GACC)) {
				fifo_level = (p_twi->TWI_FLR & TWI_FLR_RXFL_Msk) >> TWI_FLR_RXFL_Pos;

				if (fifo_level >= 4) {
					/* Configure 4 data reception */
					p_twi->TWI_FMR &= ~TWI_FMR_RXRDYM_Msk;
					p_twi->TWI_FMR |= TWI_FMR_RXRDYM_FOUR_DATA;
				}
				else if (fifo_level >= 2) {
					/* Configure 2 data reception */
					p_twi->TWI_FMR &= ~TWI_FMR_RXRDYM_Msk;
					p_twi->TWI_FMR |= TWI_FMR_RXRDYM_TWO_DATA;
				}
				else {
					/* Configure 1 data reception */
					p_twi->TWI_FMR &= ~TWI_FMR_RXRDYM_Msk;
					p_twi->TWI_FMR |= TWI_FMR_RXRDYM_ONE_DATA;
				}

				if ((status & (TWI_SR_SVREAD | TWI_SR_RXRDY)) == (TWI_SR_SVREAD | TWI_SR_RXRDY)) {
					if (fifo_level >= 4) {
						*p_data++ = (uint8_t) (p_twi->TWI_RHR & TWI_RHR_RXDATA0_Msk) >> TWI_RHR_RXDATA0_Pos;
						*p_data++ = (uint8_t) (p_twi->TWI_RHR & TWI_RHR_RXDATA1_Msk) >> TWI_RHR_RXDATA1_Pos;
						*p_data++ = (uint8_t) (p_twi->TWI_RHR & TWI_RHR_RXDATA2_Msk) >> TWI_RHR_RXDATA2_Pos;
						*p_data++ = (uint8_t) (p_twi->TWI_RHR & TWI_RHR_RXDATA3_Msk) >> TWI_RHR_RXDATA3_Pos;
						cnt += 4;
					}
					else if (fifo_level >= 2) {
						*p_data++ = (uint8_t) (p_twi->TWI_RHR & TWI_RHR_RXDATA0_Msk) >> TWI_RHR_RXDATA0_Pos;
						*p_data++ = (uint8_t) (p_twi->TWI_RHR & TWI_RHR_RXDATA1_Msk) >> TWI_RHR_RXDATA1_Pos;
						cnt += 2;
					}
					else {
						*p_data++ = (uint8_t) (p_twi->TWI_RHR & TWI_RHR_RXDATA0_Msk) >> TWI_RHR_RXDATA0_Pos;
						cnt++;
					}
				}
			}
		} else if ((status & (TWI_SR_EOSACC | TWI_SR_TXCOMP))
					== (TWI_SR_EOSACC | TWI_SR_TXCOMP)) {
			break;
		}
	} while (1);

	return cnt;
}

/**
 * \brief Write multiple bytes to a TWI compatible slave device using FIFO.
 *
 * \note This function will NOT return until all data has been written or error occurred.
 *
 * \param p_twi Pointer to a TWI instance.
 * \param p_packet Packet information and data (see \ref twi_packet_t).
 *
 * \return TWI_SUCCESS if all bytes were written, error code otherwise.
 */
uint32_t twi_master_write_fifo(Twi *p_twi, twi_packet_t *p_packet)
{
	uint32_t status;
	uint32_t cnt = p_packet->length;
	uint8_t *buffer = p_packet->buffer;
	uint32_t aux_value;

	/* Check argument */
	if (cnt == 0) {
		return TWI_INVALID_ARGUMENT;
	}

	/* Enable FIFO */
	twi_enable_fifo_master(p_twi);

	/* Set write mode, slave address and 3 internal address byte lengths */
	p_twi->TWI_MMR = 0;
	p_twi->TWI_MMR = TWI_MMR_DADR(p_packet->chip) |
			((p_packet->addr_length << TWI_MMR_IADRSZ_Pos) &
			TWI_MMR_IADRSZ_Msk);

	/* Set internal address for remote chip */
	p_twi->TWI_IADR = 0;
	p_twi->TWI_IADR = twi_mk_addr(p_packet->addr, p_packet->addr_length);

	if (cnt >= 4) {
		/* Configure 4 data transmission */
		p_twi->TWI_FMR &= ~TWI_FMR_TXRDYM_Msk;
		p_twi->TWI_FMR |= TWI_FMR_TXRDYM_FOUR_DATA;
	}
	else if (cnt >= 2) {
		/* Configure 2 data transmission */
		p_twi->TWI_FMR &= ~TWI_FMR_TXRDYM_Msk;
		p_twi->TWI_FMR |= TWI_FMR_TXRDYM_TWO_DATA;
	}
	else {
		/* Configure 1 data transmission */
		p_twi->TWI_FMR &= ~TWI_FMR_TXRDYM_Msk;
		p_twi->TWI_FMR |= TWI_FMR_TXRDYM_ONE_DATA;
	}

	/* Send all bytes */
	while (cnt > 0) {
		status = p_twi->TWI_SR;
		if (status & TWI_SR_NACK) {
			return TWI_RECEIVE_NACK;
		}

		if (!(status & TWI_SR_TXRDY)) {
			continue;
		}

		if (cnt >= 4) {
			aux_value = TWI_THR_TXDATA0(*buffer++);
			aux_value |= TWI_THR_TXDATA1(*buffer++);
			aux_value |= TWI_THR_TXDATA2(*buffer++);
			aux_value |= TWI_THR_TXDATA3(*buffer++);
			cnt -= 4;
		}
		else if (cnt >= 2) {
			aux_value = TWI_THR_TXDATA0(*buffer++);
			aux_value |= TWI_THR_TXDATA1(*buffer++);
			cnt -= 2;
		}
		else {
			aux_value = TWI_THR_TXDATA0(*buffer++);
			cnt--;
		}

		if (cnt >= 4) {
			/* Configure 4 data transmission */
			p_twi->TWI_FMR &= ~TWI_FMR_TXRDYM_Msk;
			p_twi->TWI_FMR |= TWI_FMR_TXRDYM_FOUR_DATA;
		}
		else if (cnt >= 2) {
			/* Configure 2 data transmission */
			p_twi->TWI_FMR &= ~TWI_FMR_TXRDYM_Msk;
			p_twi->TWI_FMR |= TWI_FMR_TXRDYM_TWO_DATA;
		}
		else {
			/* Configure 1 data transmission */
			p_twi->TWI_FMR &= ~TWI_FMR_TXRDYM_Msk;
			p_twi->TWI_FMR |= TWI_FMR_TXRDYM_ONE_DATA;
		}
	}

	while (1) {
		status = p_twi->TWI_SR;
		if (status & TWI_SR_NACK) {
			return TWI_RECEIVE_NACK;
		}

		if (status & TWI_SR_TXRDY) {
			break;
		}
	}

	p_twi->TWI_CR = TWI_CR_STOP;

	while (!(p_twi->TWI_SR & TWI_SR_TXCOMP)) {
	}

	return TWI_SUCCESS;
}

//@}

/// @cond 0
/**INDENT-OFF**/
#ifdef __cplusplus
}
#endif
/**INDENT-ON**/
/// @endcond
