/**
 * \file
 *
 * \brief SAM4 Peripheral DMA Controller (PDC) driver.
 *
 * Copyright (c) 2011-2015 Atmel Corporation. All rights reserved.
 *
 * \asf_license_start
 *
 * \page License
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice,
 *    this list of conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 *
 * 3. The name of Atmel may not be used to endorse or promote products derived
 *    from this software without specific prior written permission.
 *
 * 4. This software may only be redistributed and used in connection with an
 *    Atmel microcontroller product.
 *
 * THIS SOFTWARE IS PROVIDED BY ATMEL "AS IS" AND ANY EXPRESS OR IMPLIED
 * WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NON-INFRINGEMENT ARE
 * EXPRESSLY AND SPECIFICALLY DISCLAIMED. IN NO EVENT SHALL ATMEL BE LIABLE FOR
 * ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,
 * STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN
 * ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 *
 * \asf_license_stop
 *
 */
/*
 * Support and FAQ: visit <a href="http://www.atmel.com/design-support/">Atmel Support</a>
 */

#include "pdc.h"

/// @cond
/**INDENT-OFF**/
#ifdef __cplusplus
extern "C" {
#endif
/**INDENT-ON**/
/// @endcond

    /**
     * \brief Configure PDC for data transmit.
     *
     * \param[out] p_pdc        Device structure pointer
     * \param[in] p_packet      Pointer to packet information for current buffer register
     *                          set. Use NULL to leave unaltered.
     * \param[in] p_next_packet Pointer to packet information for next buffer register
     *                          set. Use NULL to leave unaltered.
     */
    void pdc_tx_init(
        Pdc *p_pdc,
        pdc_packet_t *p_packet,
        pdc_packet_t *p_next_packet)
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
void pdc_rx_init(
    Pdc *p_pdc,
    pdc_packet_t *p_packet,
    pdc_packet_t *p_next_packet)
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
void pdc_rx_clear_cnt(
    Pdc *p_pdc)
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
 * \param[out] p_pdc 	  Device structure pointer
 * \param[in] ul_controls Transfer directions
 *                        (bit PERIPH_PTCR_RXTEN and bit PERIPH_PTCR_TXTEN)
 */
void pdc_enable_transfer(
    Pdc *p_pdc,
    uint32_t ul_controls)
{
    /* Validate inputs. */
    Assert(p_pdc);

    p_pdc->PERIPH_PTCR =
        ul_controls & (PERIPH_PTCR_RXTEN | PERIPH_PTCR_TXTEN);
}

/**
 * \brief Disable PDC transfers (TX and/or RX).
 *
 * \param[out] p_pdc      Device structure pointer
 * \param[in] ul_controls Transfer directions
 *                        (bit PERIPH_PTCR_TXTDIS, bit PERIPH_PTCR_TXTDIS)
 */
void pdc_disable_transfer(
    Pdc *p_pdc,
    uint32_t ul_controls)
{
    /* Validate inputs. */
    Assert(p_pdc);

    p_pdc->PERIPH_PTCR =
        ul_controls & (PERIPH_PTCR_RXTDIS | PERIPH_PTCR_TXTDIS);
}

/**
 * \brief Read PDC status.
 *
 * \param[in] p_pdc Device structure pointer
 *
 * \return PDC status register bit map.
 *
 * <table>
 * <tr>
 * <th>Name</th>
 * <th>Description</th>
 * <th>Bit</th>
 * </tr>
 * <tr>
 *   <td>RXTEN</td>
 *   <td>Receiver Transfer Enabled</td>
 *   <td>8</td>
 * </tr>
 * <tr>
 *   <td>TXTEN</td>
 *   <td>Transmitter Transfer Enabled</td>
 *   <td>1</td>
 * </tr>
 * </table>
 *
 */
uint32_t pdc_read_status(
    Pdc *p_pdc)
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
uint32_t pdc_read_rx_ptr(
    Pdc *p_pdc)
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
uint32_t pdc_read_rx_counter(
    Pdc *p_pdc)
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
uint32_t pdc_read_tx_ptr(
    Pdc *p_pdc)
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
uint32_t pdc_read_tx_counter(
    Pdc *p_pdc)
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
uint32_t pdc_read_rx_next_ptr(
    Pdc *p_pdc)
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
uint32_t pdc_read_rx_next_counter(
    Pdc *p_pdc)
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
uint32_t pdc_read_tx_next_ptr(
    Pdc *p_pdc)
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
uint32_t pdc_read_tx_next_counter(
    Pdc *p_pdc)
{
    /* Validate inputs. */
    Assert(p_pdc);

    return p_pdc->PERIPH_TNCR;
}

/// @cond
/**INDENT-OFF**/
#ifdef __cplusplus
}
#endif
/**INDENT-ON**/
/// @endcond
