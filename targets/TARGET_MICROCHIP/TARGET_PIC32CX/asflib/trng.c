/**
 * \file
 *
 * \brief TRNG driver for SAM.
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

/**
 * \defgroup group_sam_drivers_trng TRNG - True Random Number Generator
 *
 * Driver for the TRNG (True Random Number Generator). This driver provides access
 * to the main features of the TRNG controller.
 *
 * \{
 */

/*
 * Support and FAQ: visit <a href="https://www.microchip.com/support/">Microchip Support</a>
 */

#include "trng.h"

/* / @cond 0 */
/**INDENT-OFF**/
#ifdef __cplusplus
extern "C" {
#endif
/**INDENT-ON**/
/* / @endcond */

#define CLK_THRESHOLD_HALF_RATE   100000000UL

/**
 * \brief Enable TRNG.
 *
 * \param p_trng  Pointer to a TRNG instance.
 * \param ul_mck  TRNG module input clock frequency.
 *
 */
void trng_enable(Trng *p_trng, uint32_t ul_mck)
{
	p_trng->TRNG_CR = TRNG_CR_ENABLE | TRNG_CR_KEY_PASSWD;
	if (ul_mck > CLK_THRESHOLD_HALF_RATE) {
		p_trng->TRNG_MR |= TRNG_MR_HALFR;
	}
}

/**
 * \brief Enable TRNG interrupt.
 *
 * \param p_trng  Pointer to a TRNG instance.
 * \param ul_sources Interrupt sources bit map.
 *
 */
void trng_enable_interrupt(Trng *p_trng, uint32_t ul_sources)
{
	p_trng->TRNG_IER = ul_sources;
}

/**
 * \brief Disable TRNG interrupt.
 *
 * \param p_trng  Pointer to a TRNG instance.
 * \param ul_sources Interrupt sources bit map.
 *
 */
void trng_disable_interrupt(Trng *p_trng, uint32_t ul_sources)
{
	p_trng->TRNG_IDR = ul_sources;
}

/**
 * \brief Disable TRNG.
 *
 * \param p_trng  Pointer to a TRNG instance.
 *
 */
void trng_disable(Trng *p_trng)
{
	p_trng->TRNG_CR = TRNG_CR_KEY_PASSWD;
}

/**
 * \brief Get TRNG interrupt mask.
 *
 * \param p_trng  Pointer to a TRNG instance.
 *
 * \retval The interrupt mask value.
 */
uint32_t trng_get_interrupt_mask(Trng *p_trng)
{
	return p_trng->TRNG_IMR;
}

/**
 * \brief Get TRNG interrupt status.
 *
 * \param p_trng  Pointer to a TRNG instance.
 *
 * \retval The interrupt status value.
 */
uint32_t trng_get_interrupt_status(Trng *p_trng)
{
	return p_trng->TRNG_ISR;
}

/**
 * \brief Read TRNG output data.
 *
 * \param p_trng  Pointer to a TRNG instance.
 *
 * \retval The output data value.
 */
uint32_t trng_read_output_data(Trng *p_trng)
{
	return p_trng->TRNG_ODATA;
}

/**
 * \brief Disable TRNG interrupt.
 *
 * \param p_trng  Pointer to a TRNG instance.
 * \param ul_key_len  Key Length in bytes.
 * \param e_key_slave  Slave ID to where Key is transferred to (AES or AESB, see enum trng_private_key_bus_slave).
 *
 */
void trng_trigger_private_key_bus_transfer(Trng *p_trng, uint32_t ul_key_len,
		enum trng_private_key_bus_slave e_key_slave)
{
	uint32_t val = 0;
	uint32_t ul_len_32bit_words;

	/* Key length */
	ul_len_32bit_words = ((ul_key_len >> 2) & 0xffu) - 1;
	val |= TRNG_PKBCR_KLENGTH(ul_len_32bit_words);

	/* Key Slave */
	val |= e_key_slave;

	p_trng->TRNG_PKBCR = TRNG_PKBCR_WAKEY_PASSWD | val;
}

/**
 * \brief Enable write protect of TRNG Mode register.
 *
 * \param p_trng Pointer to a TRNG instance.
 */
void trng_enable_writeprotect_mode_reg(Trng *p_trng)
{
	p_trng->TRNG_WPMR |= (TRNG_WPMR_WPEN | TRNG_WPMR_WPKEY_PASSWD);
}

/**
 * \brief Enable write protect of TRNG Interrupt registers.
 *
 * \param p_trng Pointer to a TRNG instance.
 */
void trng_enable_writeprotect_int_regs(Trng *p_trng)
{
	p_trng->TRNG_WPMR |= (TRNG_WPMR_WPITEN | TRNG_WPMR_WPKEY_PASSWD);
}

/**
 * \brief Enable write protect of TRNG Control registers.
 *
 * \param p_trng Pointer to a TRNG instance.
 */
void trng_enable_writeprotect_ctl_regs(Trng *p_trng)
{
	p_trng->TRNG_WPMR |= (TRNG_WPMR_WPCREN | TRNG_WPMR_WPKEY_PASSWD);
}

/**
 * \brief Enable write protect of All TRNG registers.
 *
 * \param p_trng Pointer to a TRNG instance.
 */
void trng_enable_writeprotect_all_regs(Trng *p_trng)
{
	p_trng->TRNG_WPMR |= (TRNG_WPMR_WPEN | TRNG_WPMR_WPITEN |
			TRNG_WPMR_WPCREN | TRNG_WPMR_WPKEY_PASSWD);
}

/**
 * \brief Disable write protect of All TRNG registers.
 *
 * \param p_trng Pointer to a TRNG instance.
 */
void trng_disable_writeprotect_all_regs(Trng *p_trng)
{
	uint32_t val;

	/* Read value */
	val = p_trng->TRNG_WPMR;
	/* Clear WP bits */
	val &= ~(TRNG_WPMR_WPEN | TRNG_WPMR_WPITEN | TRNG_WPMR_WPCREN);
	val |= TRNG_WPMR_WPKEY_PASSWD;
	/* Write value */
	p_trng->TRNG_WPMR = val;
}

/**
 * \brief Enable write protect of TRNG Control registers.
 *
 * \param p_trng Pointer to a TRNG instance.
 * \param b_keep_first true - First violation is kept in report register.
 *                     false - Report register is updated on every protected access.
 */
void trng_config_protect_violation_report(Trng *p_trng, bool b_keep_first)
{
	uint32_t val;

	/* Read value */
	val = p_trng->TRNG_WPMR;
	/* Configure */
	if (b_keep_first) {
		val |= TRNG_WPMR_FIRSTE;
	} else {
		val &= ~TRNG_WPMR_FIRSTE;
	}

	val |= TRNG_WPMR_WPKEY_PASSWD;
	/* Write value */
	p_trng->TRNG_WPMR = val;
}

/**
 * \brief Get TRNG Write Protection status.
 *
 * \param p_trng  Pointer to a TRNG instance.
 *
 * \retval The Write Protection status value.
 */
uint32_t trng_get_write_protection_status(Trng *p_trng)
{
	return p_trng->TRNG_WPSR;
}

/* / @cond 0 */
/**INDENT-OFF**/
#ifdef __cplusplus
}
#endif
/**INDENT-ON**/
/* / @endcond */

/**
 * \}
 */
