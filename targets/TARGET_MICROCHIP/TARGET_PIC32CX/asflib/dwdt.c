/**
 * \file
 *
 * \brief Dual Watchdog Timer (DWDT) driver for PIC32CX.
 *
 * Copyright (c) 2020 Microchip Technology Inc. and its subsidiaries.
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

#include "dwdt.h"

/* / @cond 0 */
/**INDENT-OFF**/
#ifdef __cplusplus
extern "C" {
#endif
/**INDENT-ON**/
/* / @endcond */

/**
 * \defgroup sam_drivers_ddwdt_group Dual Watchdog Timer (DWDT)
 *
 * Driver for the DWDT (Watchdog Timer). This driver provides access to the main
 * features of the DWDT controller.
 * The Dual Watchdog Timer (DWDT) is used to prevent system lock-up if the
 * software becomes trapped in a deadlock. The DWDT embeds two independent
 * 12-bit down counters (WDT0 and WDT1) and each watchdog provides a monitoring
 * period of up to 16 seconds (slow clock at 32.768 kHz). It can
 * generate a general reset or a processor reset only. In addition, it can be
 * stopped while the processor is in debug mode or idle mode.
 *
 * @{
 */

#define DWDT_SLCK_DIV      128
#define DWDT_MAX_VALUE     4095

/**
 * \brief Get counter value or permitted range value of watchdog timer from the
 * desired timeout period (in us).
 *
 * \note The value returned by this function can be used by dwdt_init() if it is
 * not DWDT_INVALID_ARGUMENT.
 *
 * \param ul_us The desired timeout period (in us).
 * \param ul_sclk The slow clock on board (in Hz).
 * \param ul_presc The Watchdog Prescaler ratio.
 *
 * \return If the desired period is beyond the watchdog period, this function
 * returns DWDT_INVALID_ARGUMENT. Otherwise it returns valid value.
 */
uint32_t dwdt_get_timeout_value(uint32_t ul_us, uint32_t ul_sclk, uint32_t ul_presc)
{
	uint32_t max, min;
	uint32_t ul_presc_value;

	ul_presc_value = (ul_presc & WDT0_IL_PRESC_Msk) >> WDT0_IL_PRESC_Pos;

	min = ((DWDT_SLCK_DIV >> ul_presc_value) * 1000000) / ul_sclk;
	max = min * DWDT_MAX_VALUE;

	if (ul_us > max) {
		return DWDT_INVALID_ARGUMENT;
	}

	return (ul_us / min);
}

/**
 * \brief Initialize Watchdog 0 timer with the given mode.
 *
 * \param p_dwdt Pointer to a DWDT instance.
 * \param dwdt_id Watchdog ID.
 * \param p_cfg Pointer to the watchdog configuration data.
 *
 * \retval 0 Success.
 * \retval 1 Configuration error.
 */
uint32_t dwdt_init(Dwdt *p_dwdt, dwdt_id_t dwdt_id, dwdt_cfg_t *p_cfg)
{
	uint32_t ul_period;
	uint32_t ul_rep_thrs = 0;
	uint32_t ul_int_lvl = 0;

	/* Get timeout value. */
	ul_period = dwdt_get_timeout_value(p_cfg->ul_time * 1000,
			p_cfg->ul_slck, p_cfg->ul_prescaler);
	/* Check timeout value. */
	if (ul_period == DWDT_INVALID_ARGUMENT) {
		return 1;
	}

	/* Get repeat threshold value. */
	if (p_cfg->ul_rep_time) {
		ul_rep_thrs = dwdt_get_timeout_value(p_cfg->ul_rep_time * 1000,
			p_cfg->ul_slck, p_cfg->ul_prescaler);
		/* Check repeat threshold value. */
		if (ul_rep_thrs == DWDT_INVALID_ARGUMENT) {
			return 1;
		}
	}
	

	/* Get interrupt level threshold value. */
	if (p_cfg->ul_intlvl_time) {
		ul_int_lvl = dwdt_get_timeout_value(p_cfg->ul_intlvl_time * 1000,
			p_cfg->ul_slck, p_cfg->ul_prescaler);
		/* Check interrupt level threshold value. */
		if (ul_int_lvl == DWDT_INVALID_ARGUMENT) {
			return 1;
		}
	}
	

	if (dwdt_id == WDT0_ID) {
		p_dwdt->WDT0_MR = WDT0_MR_WDDIS;
		p_dwdt->WDT0_WL = WDT0_WL_PERIOD(ul_period) | WDT0_WL_RPTH(ul_rep_thrs);
		p_dwdt->WDT0_IL = WDT0_IL_LVLTH(ul_int_lvl) | p_cfg->ul_prescaler;
		p_dwdt->WDT0_MR = p_cfg->ul_mode;
		p_dwdt->WDT0_MR &= ~(WDT0_MR_WDDIS);
		p_dwdt->WDT0_CR = WDT0_CR_KEY_PASSWD | WDT0_CR_WDRSTT;
		/* Clear Status */
		p_dwdt->WDT0_ISR;
	} else {
		p_dwdt->WDT1_MR = WDT1_MR_WDDIS;
		p_dwdt->WDT1_WL = WDT1_WL_PERIOD(ul_period) | WDT1_WL_RPTH(ul_rep_thrs);
		p_dwdt->WDT1_IL = WDT1_IL_LVLTH(ul_int_lvl) | p_cfg->ul_prescaler;
		p_dwdt->WDT1_MR = p_cfg->ul_mode;
		p_dwdt->WDT1_MR &= ~(WDT1_MR_WDDIS);
		p_dwdt->WDT1_CR = WDT1_CR_KEY_PASSWD | WDT1_CR_WDRSTT;
		/* Clear Status */
		p_dwdt->WDT1_ISR;
	}
	
	return 0;
}

/**
 * \brief Disable the watchdog timer.
 *
 * \param p_dwdt Pointer to a DWDT instance.
 * \param dwdt_id Watchdog ID.
 */
void dwdt_disable(Dwdt *p_dwdt, dwdt_id_t dwdt_id)
{
	if (dwdt_id == WDT0_ID) {
		p_dwdt->WDT0_MR = WDT0_MR_WDDIS;
	} else {
		p_dwdt->WDT1_MR = WDT1_MR_WDDIS;
	}
}

/**
 * \brief Restart the watchdog timer.
 *
 * \param p_dwdt Pointer to a DWDT instance.
 * \param dwdt_id Watchdog ID.
 */
void dwdt_restart(Dwdt *p_dwdt, dwdt_id_t dwdt_id)
{
	if (dwdt_id == WDT0_ID) {
		p_dwdt->WDT0_CR = WDT0_CR_KEY_PASSWD | WDT0_CR_WDRSTT;
	} else {
		p_dwdt->WDT1_CR = WDT1_CR_KEY_PASSWD | WDT1_CR_WDRSTT;
	}
}

/**
 * \brief Get the current value of the watchdog down counter.
 *
 * \param p_dwdt Pointer to a DWDT instance.
 * \param dwdt_id Watchdog ID.
 *
 * \note Due to the asynchronous operation of the watchdog, it is necessary to read this register twice
 *       at the same value to get a valid read. Therefore, a minimum of two and a maximum of
 *       three accesses are required.
 */
uint32_t dwdt_get_value(Dwdt *p_dwdt, dwdt_id_t dwdt_id)
{
	uint32_t *pul_val;
	uint32_t ul_val1, ul_val2;

	if (dwdt_id == WDT0_ID) {
		pul_val = (uint32_t *)&p_dwdt->WDT0_VR;
	} else {
		pul_val = (uint32_t *)&p_dwdt->WDT1_VR;
	}
	
	ul_val1 = *pul_val;
	ul_val2 = *pul_val;
	if (ul_val1 != ul_val2) {
		ul_val2 = *pul_val;
	}

	return ul_val2;
}

/**
 * \brief Enable interrupts of the watchdog timer.
 *
 * \param p_dwdt Pointer to a DWDT instance.
 * \param dwdt_id Watchdog ID.
 * \param ul_mask Interrupt sources bit map.
 */
void dwdt_enable_interrupt(Dwdt *p_dwdt, dwdt_id_t dwdt_id, const uint32_t ul_mask)
{
	if (dwdt_id == WDT0_ID) {
		p_dwdt->WDT0_IER = ul_mask;
	} else {
		p_dwdt->WDT1_IER = ul_mask;
	}
}

/**
 * \brief Disable interrupts of the watchdog timer.
 *
 * \param p_dwdt Pointer to a DWDT instance.
 * \param dwdt_id Watchdog ID.
 * \param ul_mask Interrupt sources bit map.
 */
void dwdt_disable_interrupt(Dwdt *p_dwdt, dwdt_id_t dwdt_id, const uint32_t ul_mask)
{
	if (dwdt_id == WDT0_ID) {
		p_dwdt->WDT0_IDR = ul_mask;
	} else {
		p_dwdt->WDT1_IDR = ul_mask;
	}
}

/**
 * \brief Get the interrupt status of the watchdog timer.
 *
 * \param p_dwdt Pointer to a DWDT instance.
 * \param dwdt_id Watchdog ID.
 */
uint32_t dwdt_get_interrupt_status(Dwdt *p_dwdt, dwdt_id_t dwdt_id)
{
	if (dwdt_id == WDT0_ID) {
		return p_dwdt->WDT0_ISR;
	} else {
		return p_dwdt->WDT1_ISR;
	}
}

/**
 * \brief Get the interrupt mask of the watchdog timer.
 *
 * \param p_dwdt Pointer to a DWDT instance.
 * \param dwdt_id Watchdog ID.
 */
uint32_t dwdt_get_interrupt_mask(Dwdt *p_dwdt, dwdt_id_t dwdt_id)
{
	if (dwdt_id == WDT0_ID) {
		return p_dwdt->WDT0_IMR;
	} else {
		return p_dwdt->WDT1_IMR;
	}
}

/**
 * \brief Get the timeout period of the WatchDog Timer in microseconds.
 *
 * \param p_dwdt Pointer to a DWDT instance.
 * \param dwdt_id Watchdog ID.
 * \param ul_sclk The slow clock frequency (in Hz).
 *
 * \return The timeout period in microseconds.
 */
uint32_t dwdt_get_us_timeout_period(Dwdt *p_dwdt, dwdt_id_t dwdt_id, uint32_t ul_sclk)
{
	uint32_t ul_value;
	uint32_t ul_presc;

	ul_value = p_dwdt->WDT0_VR;

	if (dwdt_id == WDT0_ID) {
		ul_value = p_dwdt->WDT0_VR;
		ul_presc = (p_dwdt->WDT0_IL & WDT0_IL_PRESC_Msk) >> WDT0_IL_PRESC_Pos;
	} else {
		ul_value = p_dwdt->WDT1_VR;
		ul_presc = (p_dwdt->WDT1_IL & WDT1_IL_PRESC_Msk) >> WDT1_IL_PRESC_Pos;
	}

	return ul_value * (DWDT_SLCK_DIV >> ul_presc) / ul_sclk * 1000000;
}

/* @} */

/* / @cond 0 */
/**INDENT-OFF**/
#ifdef __cplusplus
}
#endif
/**INDENT-ON**/
/* / @endcond */
