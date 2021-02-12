/**
 * \file
 *
 * \brief SAM4 Peripheral DMA Controller (PDC) driver.
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
#include "pdc.h"

/* / @cond */
/**INDENT-OFF**/
#ifdef __cplusplus
extern "C" {
#endif
/**INDENT-ON**/
/* / @endcond */

/**
 * \brief Configure PDC for data transmit.
 *
 * \param[out] p_pdc        Device structure pointer
 * \param[in] p_packet      Pointer to packet information for current buffer register
 *                          set. Use NULL to leave unaltered.
 * \param[in] p_next_packet Pointer to packet information for next buffer register
 *                          set. Use NULL to leave unaltered.
 */
void pdc_tx_init(Pdc *p_pdc, pdc_packet_t *p_packet, pdc_packet_t *p_next_packet)
{
	/* Validate inputs. */
	Assert(p_pdc);

	if (p_packet) {
		p_pdc->PERIPH_TPR = p_packet->ul_addr;
		p_pdc->PERIPH_TCR = p_packet->ul_size;
	}

	if (p_next_packet) {
		p_pdc->PERIPH_TNPR = p_next_packet->ul_addr;
		p_pdc->PERIPH_TNCR = p_next_packet->ul_size;
	}
}

/**
 * \brief Configure PDC for data receive.
 *
 * \param[out] p_pdc        Device structure pointer
 * \param[in] p_packet      Pointer to packet information for current buffer register
 *                          set. Use NULL to leave unaltered.
 * \param[in] p_next_packet Pointer to packet information for next buffer register
 *                          set. Use NULL to leave unaltered.
 */
void pdc_rx_init(Pdc *p_pdc, pdc_packet_t *p_packet, pdc_packet_t *p_next_packet)
{
	/* Validate inputs. */
	Assert(p_pdc);

	if (p_packet) {
		p_pdc->PERIPH_RPR = p_packet->ul_addr;
		p_pdc->PERIPH_RCR = p_packet->ul_size;
	}

	if (p_next_packet) {
		p_pdc->PERIPH_RNPR = p_next_packet->ul_addr;
		p_pdc->PERIPH_RNCR = p_next_packet->ul_size;
	}
}

/**
 * \brief Clear PDC buffer receive counter.
 *
 * \param[out] p_pdc Device structure pointer
 */
void pdc_rx_clear_cnt(Pdc *p_pdc)
{
	/* Validate inputs. */
	Assert(p_pdc);

	p_pdc->PERIPH_RNCR = 0;
	p_pdc->PERIPH_RCR = 0;
}

/**
 * \brief Enable PDC transfers (TX and/or RX).
 *
 * \note It is forbidden to set both TXTEN and RXTEN for a half duplex
 * peripheral.
 *
 * \param[out] p_pdc      Device structure pointer
 * \param[in] ul_controls Transfer directions
 *                        (bit PERIPH_PTCR_RXTEN and bit PERIPH_PTCR_TXTEN)
 *
 * \note For PIC32CX devices new controls have been supported:
 *                        (bit PERIPH_PTCR_TXCBEN and bit PERIPH_PTCR_RXCBEN)
 */
void pdc_enable_transfer(Pdc *p_pdc, uint32_t ul_controls)
{
	/* Validate inputs. */
	Assert(p_pdc);

	p_pdc->PERIPH_PTCR = ul_controls & (PERIPH_PTCR_RXTEN | PERIPH_PTCR_TXTEN | PERIPH_PTCR_TXCBEN | PERIPH_PTCR_RXCBEN);
}

/**
 * \brief Disable PDC transfers (TX and/or RX).
 *
 * \param[out] p_pdc      Device structure pointer
 * \param[in] ul_controls Transfer directions
 *                        (bit PERIPH_PTCR_TXTDIS, bit PERIPH_PTCR_TXTDIS)
 *
 * \note For PIC32CX devices new controls have been supported:
 *       bit PERIPH_PTCR_TXCBEN, bit PERIPH_PTCR_RXCBEN, bit PERIPH_PTCR_ERRCLR.
 */
void pdc_disable_transfer(Pdc *p_pdc, uint32_t ul_controls)
{
	/* Validate inputs. */
	Assert(p_pdc);

	p_pdc->PERIPH_PTCR = ul_controls & (PERIPH_PTCR_RXTDIS | PERIPH_PTCR_TXTDIS);
}

/**
 * \brief Read PDC status.
 *
 * \param[in] p_pdc Device structure pointer
 *
 * \return PDC status register bit map.
 */
uint32_t pdc_read_status(Pdc *p_pdc)
{
	/* Validate inputs. */
	Assert(p_pdc);

	return p_pdc->PERIPH_PTSR;
}

/**
 * \brief Return Receive Pointer Register (RPR) value.
 *
 * \param[in] p_pdc Device structure pointer
 *
 * \return Receive Pointer Register value.
 */
uint32_t pdc_read_rx_ptr(Pdc *p_pdc)
{
	/* Validate inputs. */
	Assert(p_pdc);

	return p_pdc->PERIPH_RPR;
}

/**
 * \brief Return Receive Counter Register (RCR) value.
 *
 * \param[in] p_pdc Device structure pointer
 *
 * \return Receive Counter Register value.
 */
uint32_t pdc_read_rx_counter(Pdc *p_pdc)
{
	/* Validate inputs. */
	Assert(p_pdc);

	return p_pdc->PERIPH_RCR;
}

/**
 * \brief Return Transmit Pointer Register (TPR) value.
 *
 * \param[in] p_pdc Device structure pointer
 *
 * \return Transmit Pointer Register value.
 */
uint32_t pdc_read_tx_ptr(Pdc *p_pdc)
{
	/* Validate inputs. */
	Assert(p_pdc);

	return p_pdc->PERIPH_TPR;
}

/**
 * \brief Return Transmit Counter Register (TCR) value.
 *
 * \param[in] p_pdc Device structure pointer
 *
 * \return Transmit Counter Register value.
 */
uint32_t pdc_read_tx_counter(Pdc *p_pdc)
{
	/* Validate inputs. */
	Assert(p_pdc);

	return p_pdc->PERIPH_TCR;
}

/**
 * \brief Return Receive Next Pointer Register (RNPR) value.
 *
 * \param[in] p_pdc Device structure pointer
 *
 * \return Receive Next Pointer Register value.
 */
uint32_t pdc_read_rx_next_ptr(Pdc *p_pdc)
{
	/* Validate inputs. */
	Assert(p_pdc);

	return p_pdc->PERIPH_RNPR;
}

/**
 * \brief Return Receive Next Counter Register (RNCR) value.
 *
 * \param[in] p_pdc Device structure pointer
 *
 * \return Receive Next Counter Register value.
 */
uint32_t pdc_read_rx_next_counter(Pdc *p_pdc)
{
	/* Validate inputs. */
	Assert(p_pdc);

	return p_pdc->PERIPH_RNCR;
}

/**
 * \brief Return Transmit Next Pointer Register (TNPR) value.
 *
 * \param[in] p_pdc Device structure pointer
 *
 * \return Transmit Next Pointer Register value.
 */
uint32_t pdc_read_tx_next_ptr(Pdc *p_pdc)
{
	/* Validate inputs. */
	Assert(p_pdc);

	return p_pdc->PERIPH_TNPR;
}

/**
 * \brief Return Transmit Next Counter Register (TNCR) value.
 *
 * \param[in] p_pdc Device structure pointer
 *
 * \return Transmit Next Counter Register value.
 */
uint32_t pdc_read_tx_next_counter(Pdc *p_pdc)
{
	/* Validate inputs. */
	Assert(p_pdc);

	return p_pdc->PERIPH_TNCR;
}

/* / @cond */
/**INDENT-OFF**/
#ifdef __cplusplus
}
#endif
/**INDENT-ON**/
/* / @endcond */
