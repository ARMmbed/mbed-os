/**
 * \file
 *
 * \brief Serial Peripheral Interface (SPI) driver for SAM.
 *
 * Copyright (c) 2011-2018 Microchip Technology Inc. and its subsidiaries.
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
#include "spia.h"
#include "sysclk.h"

/**
 * \defgroup sam_drivers_spi_group Serial Peripheral Interface (SPI)
 *
 * See \ref sam_spi_quickstart.
 *
 * The SPI circuit is a synchronous serial data link that provides communication
 * with external devices in Master or Slave mode. Connection to Peripheral DMA
 * Controller channel capabilities optimizes data transfers.
 *
 * @{
 */
#ifndef SPI_WPMR_WPKEY_PASSWD
#define SPI_WPMR_WPKEY_PASSWD SPI_WPMR_WPKEY((uint32_t) 0x535049)
#endif

/**
 * \brief Enable SPI clock.
 *
 * \param p_spi Pointer to an SPI instance.
 */
void spi_enable_clock(Spi *p_spi)
{
	if (p_spi == SPI0) {
		sysclk_enable_peripheral_clock(ID_FLEXCOM0);
	} else if (p_spi == SPI1) {
		sysclk_enable_peripheral_clock(ID_FLEXCOM1);
	} else if (p_spi == SPI2) {
		sysclk_enable_peripheral_clock(ID_FLEXCOM2);
	} else if (p_spi == SPI3) {
		sysclk_enable_peripheral_clock(ID_FLEXCOM3);
	} else if (p_spi == SPI4) {
		sysclk_enable_peripheral_clock(ID_FLEXCOM4);
	} else if (p_spi == SPI5) {
		sysclk_enable_peripheral_clock(ID_FLEXCOM5);
	} else if (p_spi == SPI6) {
		sysclk_enable_peripheral_clock(ID_FLEXCOM6);
	} else if (p_spi == SPI7) {
		sysclk_enable_peripheral_clock(ID_FLEXCOM7);
	}
}

/**
 * \brief Disable SPI clock.
 *
 * \param p_spi Pointer to an SPI instance.
 */
void spi_disable_clock(Spi *p_spi)
{
	if (p_spi == SPI0) {
		sysclk_disable_peripheral_clock(ID_FLEXCOM0);
	} else if (p_spi == SPI1) {
		sysclk_disable_peripheral_clock(ID_FLEXCOM1);
	} else if (p_spi == SPI2) {
        sysclk_disable_peripheral_clock(ID_FLEXCOM2);
    } else if (p_spi == SPI3) {
        sysclk_disable_peripheral_clock(ID_FLEXCOM3);
    } else if (p_spi == SPI4) {
        sysclk_disable_peripheral_clock(ID_FLEXCOM4);
    } else if (p_spi == SPI5) {
        sysclk_disable_peripheral_clock(ID_FLEXCOM5);
    } else if (p_spi == SPI6) {
        sysclk_disable_peripheral_clock(ID_FLEXCOM6);
    } else if (p_spi == SPI7) {
        sysclk_disable_peripheral_clock(ID_FLEXCOM7);
    }
}

/**
 * \brief Set Peripheral Chip Select (PCS) value.
 *
 * \param p_spi Pointer to an SPI instance.
 * \param ul_value Peripheral Chip Select value.
 *                 If PCS decode mode is not used, use \ref spi_get_pcs to build
 *                 the value to use.
 *                 On reset the decode mode is not enabled.
 *                 The decode mode can be enabled/disabled by follow functions:
 *                 \ref spi_enable_peripheral_select_decode,
 *                 \ref spi_disable_peripheral_select_decode.
 */
void spi_set_peripheral_chip_select_value(Spi *p_spi, uint32_t ul_value)
{
	p_spi->SPI_MR &= (~SPI_MR_PCS_Msk);
	p_spi->SPI_MR |= SPI_MR_PCS(ul_value);
}

/**
 * \brief Set delay between chip selects (in number of MCK clocks).
 *  If DLYBCS <= 6, 6 MCK clocks will be inserted by default.
 *
 * \param p_spi Pointer to an SPI instance.
 * \param ul_delay Delay between chip selects (in number of MCK clocks).
 */
void spi_set_delay_between_chip_select(Spi *p_spi, uint32_t ul_delay)
{
	p_spi->SPI_MR &= (~SPI_MR_DLYBCS_Msk);
	p_spi->SPI_MR |= SPI_MR_DLYBCS(ul_delay);
}

/**
 * \brief Read the received data and it's peripheral chip select value.
 * While SPI works in fixed peripheral select mode, the peripheral chip select
 * value is meaningless.
 *
 * \param p_spi Pointer to an SPI instance.
 * \param data Pointer to the location where to store the received data word.
 * \param p_pcs Pointer to fill Peripheral Chip Select Value.
 *
 * \retval SPI_OK on Success.
 * \retval SPI_ERROR_TIMEOUT on Time-out.
 */
spi_status_t spi_read(Spi *p_spi, uint16_t *us_data, uint8_t *p_pcs)
{
	uint32_t timeout = SPI_TIMEOUT;
	static uint32_t reg_value;

	while (!(p_spi->SPI_SR & SPI_SR_RDRF)) {
		if (!timeout--) {
			return SPI_ERROR_TIMEOUT;
		}
	}

	reg_value = p_spi->SPI_RDR;
	if (spi_get_peripheral_select_mode(p_spi)) {
		*p_pcs = (uint8_t) ((reg_value & SPI_RDR_PCS_Msk) >> SPI_RDR_PCS_Pos);
	}
	*us_data = (uint16_t) (reg_value & SPI_RDR_RD_Msk);

	return SPI_OK;
}

/**
 * \brief Write the transmitted data with specified peripheral chip select value.
 *
 * \param p_spi Pointer to an SPI instance.
 * \param us_data The data to transmit.
 * \param uc_pcs Peripheral Chip Select Value while SPI works in peripheral select
 * mode, otherwise it's meaningless.
 * \param uc_last Indicate whether this data is the last one while SPI is working
 * in variable peripheral select mode.
 *
 * \retval SPI_OK on Success.
 * \retval SPI_ERROR_TIMEOUT on Time-out.
 */
spi_status_t spi_write(Spi *p_spi, uint16_t us_data,
		uint8_t uc_pcs, uint8_t uc_last)
{
	uint32_t timeout = SPI_TIMEOUT;
	uint32_t value;

	while (!(p_spi->SPI_SR & SPI_SR_TDRE)) {
		if (!timeout--) {
			return SPI_ERROR_TIMEOUT;
		}
	}

	if (spi_get_peripheral_select_mode(p_spi)) {
		value = SPI_TDR_TD(us_data) | SPI_TDR_PCS(uc_pcs);
		if (uc_last) {
			value |= SPI_TDR_LASTXFER;
		}
	} else {
		value = SPI_TDR_TD(us_data);
	}

	p_spi->SPI_TDR = value;

	return SPI_OK;
}

/**
 * \brief Set clock default state.
 *
 * \param p_spi Pointer to an SPI instance.
 * \param ul_pcs_ch Peripheral Chip Select channel (0~3).
 * \param ul_polarity Default clock state is logical one(high)/zero(low).
 */
void spi_set_clock_polarity(Spi *p_spi, uint32_t ul_pcs_ch,
		uint32_t ul_polarity)
{
	if (ul_polarity) {
		p_spi->SPI_CSR[ul_pcs_ch] |= SPI_CSR_CPOL;
	} else {
		p_spi->SPI_CSR[ul_pcs_ch] &= (~SPI_CSR_CPOL);
	}
}

/**
 * \brief Set Data Capture Phase.
 *
 * \param p_spi Pointer to an SPI instance.
 *  \param ul_pcs_ch Peripheral Chip Select channel (0~3).
 *  \param ul_phase Data capture on the rising/falling edge of clock.
 */
void spi_set_clock_phase(Spi *p_spi, uint32_t ul_pcs_ch, uint32_t ul_phase)
{
	if (ul_phase) {
		p_spi->SPI_CSR[ul_pcs_ch] |= SPI_CSR_NCPHA;
	} else {
		p_spi->SPI_CSR[ul_pcs_ch] &= (~SPI_CSR_NCPHA);
	}
}

/**
 * \brief Configure CS behavior for SPI transfer (\ref spi_cs_behavior_t).
 *
 * \param p_spi Pointer to an SPI instance.
 * \param ul_pcs_ch Peripheral Chip Select channel (0~3).
 * \param ul_cs_behavior Behavior of the Chip Select after transfer. Valid values from enum spi_cs_behavior.
 */
void spi_configure_cs_behavior(Spi *p_spi, uint32_t ul_pcs_ch,
		spi_cs_behavior_t ul_cs_behavior)
{
	if (ul_cs_behavior == SPI_CS_RISE_FORCED) {
		p_spi->SPI_CSR[ul_pcs_ch] &= (~SPI_CSR_CSAAT);
		p_spi->SPI_CSR[ul_pcs_ch] |= SPI_CSR_CSNAAT;
	} else if (ul_cs_behavior == SPI_CS_RISE_NO_TX) {
		p_spi->SPI_CSR[ul_pcs_ch] &= (~SPI_CSR_CSAAT);
		p_spi->SPI_CSR[ul_pcs_ch] &= (~SPI_CSR_CSNAAT);
	} else if (ul_cs_behavior == SPI_CS_KEEP_LOW) {
		p_spi->SPI_CSR[ul_pcs_ch] |= SPI_CSR_CSAAT;
	}
}

/**
 * \brief Set number of bits per transfer.
 *
 * \param p_spi Pointer to an SPI instance.
 * \param ul_pcs_ch Peripheral Chip Select channel (0~3).
 * \param ul_bits Number of bits (8~16), use the pattern defined
 *        in the device header file.
 */
void spi_set_bits_per_transfer(Spi *p_spi, uint32_t ul_pcs_ch,
		uint32_t ul_bits)
{
	p_spi->SPI_CSR[ul_pcs_ch] &= (~SPI_CSR_BITS_Msk);
	p_spi->SPI_CSR[ul_pcs_ch] |= ul_bits;
}

/**
 * \brief Calculate the baudrate divider.
 *
 * \param baudrate Baudrate value.
 * \param mck      SPI module input clock frequency (MCK clock, Hz).
 *
 * \return Divider or error code.
 *   \retval > 0  Success.
 *   \retval < 0  Error.
 */
int16_t spi_calc_baudrate_div(const uint32_t baudrate, uint32_t mck)
{
	int baud_div = div_ceil(mck, baudrate);

	/* The value of baud_div is from 1 to 255 in the SCBR field. */
	if (baud_div <= 0 || baud_div > 255) {
		return -1;
	}

	return baud_div;
}

/**
 * \brief Set Serial Clock Baud Rate divider value (SCBR).
 *
 * \param p_spi Pointer to an SPI instance.
 * \param ul_pcs_ch Peripheral Chip Select channel (0~3).
 * \param uc_baudrate_divider Baudrate divider from MCK.
 *
 * \return
 * \retval is 0 Success.
 * \retval is -1 Error.
 */
int16_t spi_set_baudrate_div(Spi *p_spi, uint32_t ul_pcs_ch,
		uint8_t uc_baudrate_divider)
{
    /* Programming the SCBR field to 0 is forbidden */
    if (!uc_baudrate_divider){
        return -1;
	}
	p_spi->SPI_CSR[ul_pcs_ch] &= (~SPI_CSR_SCBR_Msk);
	p_spi->SPI_CSR[ul_pcs_ch] |= SPI_CSR_SCBR(uc_baudrate_divider);
    return 0;
}

/**
 * \brief Configure timing for SPI transfer.
 *
 * \param p_spi Pointer to an SPI instance.
 * \param ul_pcs_ch Peripheral Chip Select channel (0~3).
 * \param uc_dlybs Delay before SPCK (in number of MCK clocks).
 * \param uc_dlybct Delay between consecutive transfers (in number of MCK clocks).
 */
void spi_set_transfer_delay(Spi *p_spi, uint32_t ul_pcs_ch,
		uint8_t uc_dlybs, uint8_t uc_dlybct)
{
	p_spi->SPI_CSR[ul_pcs_ch] &= ~(SPI_CSR_DLYBS_Msk | SPI_CSR_DLYBCT_Msk);
	p_spi->SPI_CSR[ul_pcs_ch] |= SPI_CSR_DLYBS(uc_dlybs)
			| SPI_CSR_DLYBCT(uc_dlybct);
}

/**
 * \brief Indicate write protect status.
 *
 * \param p_spi Pointer to an SPI instance.
 *
 * \return SPI_WPSR value.
 */
uint32_t spi_get_writeprotect_status(Spi *p_spi)
{
	return p_spi->SPI_WPSR;
}

/**
 * \brief Enable SPI FIFO disabling and reenabling transactions.
 *
 * \param p_spi Pointer to an SPI instance.
 */
void spi_enable_fifo(Spi *p_spi)
{
	/* Disable SPI. */
	p_spi->SPI_CR = SPI_CR_SPIDIS;

	/* Enable FIFO. */
	p_spi->SPI_CR = SPI_CR_FIFOEN;

	/* Reenable SPI. */
	p_spi->SPI_CR = SPI_CR_SPIEN;
}

/**
 * \brief Disable SPI FIFO disabling and reenabling transactions.
 *
 * \param p_spi Pointer to an SPI instance.
 */
void spi_disable_fifo(Spi *p_spi)
{
	/* Disable SPI. */
	p_spi->SPI_CR = SPI_CR_SPIDIS;

	/* Disable FIFO. */
	p_spi->SPI_CR = SPI_CR_FIFODIS;

	/* Reenable SPI. */
	p_spi->SPI_CR = SPI_CR_SPIEN;
}

/**
 * \brief Write to SPI Tx FIFO.
 *
 * \param p_spi Pointer to a SPI instance.
 * \param p_tx_data Pointer to data to be sent.
 * \param uc_tx_data_len Length of data to be sent in bytes.
 * \param b_last Flag to indicate last transmission.
 *
 * \retval SPI_OK on Success.
 * \retval SPI_ERROR_TIMEOUT on Time-out.
 */
spi_status_t spi_fifo_write_buf(Spi *p_spi, uint8_t *p_tx_data,
		uint8_t uc_tx_data_len, bool b_last)
{
	uint32_t timeout = SPI_TIMEOUT;
	uint32_t ul_aux_tx_data;
	uint8_t *puc_tx_data;

	/* Config 2 data mode */
	p_spi->SPI_FMR &= ~SPI_FMR_TXRDYM_Msk;
	p_spi->SPI_FMR |= SPI_FMR_TXRDYM_TWO_DATA;

	puc_tx_data = (uint8_t *)p_tx_data;

	/* Loop through data */
	while (uc_tx_data_len) {
		if (uc_tx_data_len >= 2) {
			/* Wait until Tx ready or timeout reached. */
			timeout = SPI_TIMEOUT;
			while (!(p_spi->SPI_SR & SPI_SR_TDRE)) {
				if (!timeout--) {
					return SPI_ERROR_TIMEOUT;
				}
			}
			/* Write 4 data */
			ul_aux_tx_data = SPI_TDR_TD0(*puc_tx_data++);
			ul_aux_tx_data |= SPI_TDR_TD1(*puc_tx_data++);
			p_spi->SPI_TDR = ul_aux_tx_data;
			uc_tx_data_len -= 2;
		}
		else {
			/* Config 1 data mode */
			p_spi->SPI_FMR &= ~SPI_FMR_TXRDYM_Msk;
			p_spi->SPI_FMR |= SPI_FMR_TXRDYM_ONE_DATA;
			/* Wait until Tx ready or timeout reached. */
			timeout = SPI_TIMEOUT;
			while (!(p_spi->SPI_SR & SPI_SR_TDRE)) {
				if (!timeout--) {
					return SPI_ERROR_TIMEOUT;
				}
			}
			/* Write data */
			p_spi->SPI_TDR = (uint8_t)SPI_TDR_TD0(*p_tx_data++);
			uc_tx_data_len--;
		}
	}

	if (b_last) {
		p_spi->SPI_CR = SPI_CR_LASTXFER;
	}

	return SPI_OK;
}

/**
 * \brief Read from SPI Rx FIFO.
 *
 * \param p_spi Pointer to a SPI instance.
 * \param p_rx_data Pointer to data to be received.
 * \param uc_rx_data_len Length of data to be received.
 *
 * \retval SPI_OK on Success.
 * \retval SPI_ERROR_TIMEOUT on Time-out.
 */
spi_status_t spi_fifo_read_buf(Spi *p_spi, uint8_t *p_rx_data, uint8_t uc_rx_data_len)
{
	uint32_t timeout = SPI_TIMEOUT;
	uint32_t ul_8bit_mode, ul_master_mode;
	uint32_t ul_aux_rx_data;
	uint16_t *pus_rx_data;
	uint8_t uc_npcs;

	/* Check 8-bit mode */
	if ((p_spi->SPI_MR & SPI_MR_PCS_Msk) == 0) {
		uc_npcs = 0;
	} else {
		uc_npcs = 1;
	}

	ul_8bit_mode = (p_spi->SPI_CSR[uc_npcs] & SPI_CSR_BITS_Msk)? 0:1;
	ul_master_mode = p_spi->SPI_MR & SPI_MR_MSTR;

	if (ul_master_mode) {
		/* Config 1 data mode */
		p_spi->SPI_FMR &= ~SPI_FMR_RXRDYM_Msk;
		p_spi->SPI_FMR |= SPI_FMR_RXRDYM_ONE_DATA;
	} else if (ul_8bit_mode) {
		/* Config 4 data mode */
		p_spi->SPI_FMR &= ~SPI_FMR_RXRDYM_Msk;
		p_spi->SPI_FMR |= SPI_FMR_RXRDYM_FOUR_DATA;
	}

	pus_rx_data = (uint16_t *)p_rx_data;

	/* Loop through data */
	while (uc_rx_data_len) {
		if (uc_rx_data_len >= 4) {
			/* Wait until Rx ready or timeout reached. */
			timeout = SPI_TIMEOUT;
			while (!(p_spi->SPI_SR & SPI_SR_RDRF)) {
				if (!timeout--) {
					return SPI_ERROR_TIMEOUT;
				}
			}
			/* Read data */
			ul_aux_rx_data = p_spi->SPI_RDR;
			if (ul_master_mode) {
				*p_rx_data++ = (ul_aux_rx_data & SPI_RDR_RD0_Msk) >> SPI_RDR_RD0_Pos;
				uc_rx_data_len--;
			} else if (ul_8bit_mode) {
				*p_rx_data++ = (ul_aux_rx_data & SPI_RDR_RD0_Msk) >> SPI_RDR_RD0_Pos;
				*p_rx_data++ = (ul_aux_rx_data & SPI_RDR_RD1_Msk) >> SPI_RDR_RD1_Pos;
				*p_rx_data++ = (ul_aux_rx_data & SPI_RDR_RD2_Msk) >> SPI_RDR_RD2_Pos;
				*p_rx_data++ = (ul_aux_rx_data & SPI_RDR_RD3_Msk) >> SPI_RDR_RD3_Pos;
				uc_rx_data_len -= 4;
			} else {
				*pus_rx_data++ = (ul_aux_rx_data & SPI_RDR_RD0_FIFO_MULTI_DATA_16_Msk) >> SPI_RDR_RD0_FIFO_MULTI_DATA_16_Pos;
				*pus_rx_data++ = (ul_aux_rx_data & SPI_RDR_RD1_FIFO_MULTI_DATA_16_Msk) >> SPI_RDR_RD1_FIFO_MULTI_DATA_16_Pos;
				uc_rx_data_len -= 4;
			}

		}
		else if (uc_rx_data_len >= 2) {
			if (!ul_master_mode) {
				/* Config 2 data mode */
				p_spi->SPI_FMR &= ~SPI_FMR_RXRDYM_Msk;
				p_spi->SPI_FMR |= SPI_FMR_RXRDYM_TWO_DATA;
			}
			/* Wait until Rx ready or timeout reached. */
			timeout = SPI_TIMEOUT;
			while (!(p_spi->SPI_SR & SPI_SR_RDRF)) {
				if (!timeout--) {
					return SPI_ERROR_TIMEOUT;
				}
			}
			/* Read data */
			ul_aux_rx_data = p_spi->SPI_RDR;
			if (ul_master_mode) {
				*p_rx_data++ = (ul_aux_rx_data & SPI_RDR_RD0_Msk) >> SPI_RDR_RD0_Pos;
				uc_rx_data_len --;
			} else if (ul_8bit_mode) {
				*p_rx_data++ = (ul_aux_rx_data & SPI_RDR_RD0_Msk) >> SPI_RDR_RD0_Pos;
				*p_rx_data++ = (ul_aux_rx_data & SPI_RDR_RD1_Msk) >> SPI_RDR_RD1_Pos;
				uc_rx_data_len -= 2;
			} else {
				*pus_rx_data++ = (ul_aux_rx_data & SPI_RDR_RD0_FIFO_MULTI_DATA_16_Msk) >> SPI_RDR_RD0_FIFO_MULTI_DATA_16_Pos;
				uc_rx_data_len -= 2;
			}
		} else {
			/* Config 1 data mode */
			p_spi->SPI_FMR &= ~SPI_FMR_RXRDYM_Msk;
			p_spi->SPI_FMR |= SPI_FMR_RXRDYM_ONE_DATA;
			/* Wait until Rx ready or timeout reached. */
			timeout = SPI_TIMEOUT;
			while (!(p_spi->SPI_SR & SPI_SR_RDRF)) {
				if (!timeout--) {
					return SPI_ERROR_TIMEOUT;
				}
			}
			/* Read data */
			ul_aux_rx_data = p_spi->SPI_RDR;
			if (ul_8bit_mode) {
				*p_rx_data++ = (ul_aux_rx_data & SPI_RDR_RD0_Msk) >> SPI_RDR_RD0_Pos;
				uc_rx_data_len--;
			} else {
				*pus_rx_data++ = (ul_aux_rx_data & SPI_RDR_RD0_FIFO_MULTI_DATA_16_Msk) >> SPI_RDR_RD0_FIFO_MULTI_DATA_16_Pos;
				uc_rx_data_len--;
			}
		}
	}

	return SPI_OK;
}

/**
 * \brief Set SPI FIFO Tx Ready Flag Mode.
 *
 * \param p_spi Pointer to a SPI instance.
 * \param txrdy_mode Tx ready flag mode, values from enum spi_tx_ready_flag_mode.
 *
 * \retval SPI_OK on success.
 * \retval SPI_ERROR on failure.
 */
spi_status_t spi_set_fifo_tx_ready_flag_mode(Spi *p_spi,
		enum spi_tx_ready_flag_mode txrdy_mode)
{
	/* Check flag mode is valid in current configuration. */
	if (txrdy_mode == SPI_TX_READY_FLAG_TWO_DATA) {
		if (p_spi->SPI_MR & SPI_MR_PS) {
			return SPI_ERROR;
		}
	}

	/* Set flag mode. */
	p_spi->SPI_FMR &= ~SPI_FMR_TXRDYM_Msk;
	p_spi->SPI_FMR |= txrdy_mode;

	return SPI_OK;
}

/**
 * \brief Set SPI FIFO Rx Ready Flag Mode.
 *
 * \param p_spi Pointer to a SPI instance.
 * \param rxrdy_mode Rx ready flag mode, values from enum spi_rx_ready_flag_mode.
 *
 * \retval SPI_OK on success.
 * \retval SPI_ERROR on failure.
 */
spi_status_t spi_set_fifo_rx_ready_flag_mode(Spi *p_spi,
		enum spi_rx_ready_flag_mode rxrdy_mode)
{
	/* Check flag mode is valid in current configuration. */
	if (rxrdy_mode == SPI_RX_READY_FLAG_TWO_DATA) {
		if ((p_spi->SPI_MR & SPI_MR_MSTR) || (p_spi->SPI_MR & SPI_MR_PS)) {
			return SPI_ERROR;
		}
	}
	else if (rxrdy_mode == SPI_RX_READY_FLAG_FOUR_DATA) {
		if (((p_spi->SPI_CSR[0] & SPI_CSR_BITS_Msk) != SPI_CSR_BITS_8_BIT) ||
				((p_spi->SPI_CSR[1] & SPI_CSR_BITS_Msk) != SPI_CSR_BITS_8_BIT)) {
			return SPI_ERROR;
		}
		if ((p_spi->SPI_MR & SPI_MR_MSTR) || (p_spi->SPI_MR & SPI_MR_PS)) {
			return SPI_ERROR;
		}
	}

	/* Set flag mode. */
	p_spi->SPI_FMR &= ~SPI_FMR_RXRDYM_Msk;
	p_spi->SPI_FMR |= rxrdy_mode;

	return SPI_OK;
}

/**
 * \brief Set Comparison match configuration.
 *
 * \param p_spi Pointer to a SPI instance.
 * \param ul_val1 First comparison value for received data.
 * \param ul_val2 Second comparison value for received data.
 * \param cmpmode value from spi_comparator_mode enum.
 *
 * \retval SPI_OK on success.
 * \retval SPI_ERROR if comparator not compatible with current SPI mode.
 */
spi_status_t spi_set_comparison_on_rx_data(Spi *p_spi, uint16_t ul_val1,
		enum spi_comparator_mode cmpmode, uint16_t ul_val2)
{
	/* Comparator only available in Slave mode */
	if (p_spi->SPI_MR & SPI_MR_MSTR) {
		return SPI_ERROR;
	}

	/* Set comparator mode */
	p_spi->SPI_MR &= ~SPI_MR_CMPMODE;
	p_spi->SPI_MR |= cmpmode;

	/* Set comparison values */
	p_spi->SPI_CMPR &= ~(SPI_CMPR_VAL1_Msk | SPI_CMPR_VAL2_Msk);
	p_spi->SPI_CMPR |= SPI_CMPR_VAL1(ul_val1);
	p_spi->SPI_CMPR |= SPI_CMPR_VAL2(ul_val2);

	return SPI_OK;
}

/**
 * \brief Set sleepwalking mode.
 *
 * \param p_spi Pointer to a SPI instance.
 * \param ul_val1 First comparison value for received data.
 * \param ul_val2 Second comparison value for received data.
 *
 * \retval SPI_OK on success.
 * \retval SPI_ERROR if comparator not compatible with current SPI mode.
 */
spi_status_t spi_set_sleepwalking(Spi *p_spi, uint16_t ul_val1, uint16_t ul_val2)
{
	/* Comparator only available in Slave mode */
	if (p_spi->SPI_MR & SPI_MR_MSTR) {
		return SPI_ERROR;
	}

	/* Read RDR register before configuring comparator */
	volatile uint32_t aux;
	aux = p_spi->SPI_RDR;

	/* Set comparison values */
	p_spi->SPI_CMPR &= ~(SPI_CMPR_VAL1_Msk | SPI_CMPR_VAL2_Msk);
	p_spi->SPI_CMPR |= SPI_CMPR_VAL1(ul_val1);
	p_spi->SPI_CMPR |= SPI_CMPR_VAL2(ul_val2);

	return SPI_OK;
}

/**
 * @}
 */
