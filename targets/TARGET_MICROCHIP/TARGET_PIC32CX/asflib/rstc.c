/**
 * \file
 *
 * \brief SAM Reset Controller (RSTC) driver.
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

#include "rstc.h"

/* / @cond */
/**INDENT-OFF**/
#ifdef __cplusplus
extern "C" {
#endif
/**INDENT-ON**/
/* / @endcond */

#define RSTC_KEY  0xA5000000

/**
 * \brief Set the external reset length.
 *
 * \param[in,out] p_rstc Module hardware register base address pointer
 * \param[in] ul_length The length of external reset
 */
void rstc_set_external_reset(
		Rstc *p_rstc,
		const uint32_t ul_length)
{
	/* Validate the parameters. */
	Assert(p_rstc);

	uint32_t mode = p_rstc->RSTC_MR;

	mode &= ~(RSTC_MR_ERSTL_Msk | RSTC_MR_KEY_Msk);
	mode |= (RSTC_MR_ERSTL(ul_length) | RSTC_KEY);

	p_rstc->RSTC_MR = mode;
}

/**
 * \brief Enable User Reset.
 *
 * \param[in,out] p_rstc Module hardware register base address pointer
 */
void rstc_enable_user_reset(
		Rstc *p_rstc)
{
	/* Validate the parameters. */
	Assert(p_rstc);

	uint32_t mode = p_rstc->RSTC_MR;

	mode &= ~RSTC_MR_KEY_Msk;
	mode |= (RSTC_MR_URSTEN | RSTC_KEY);

	p_rstc->RSTC_MR = mode;
}

/**
 * \brief Disable User Reset.
 *
 * \param[in,out] p_rstc Module hardware register base address pointer
 */
void rstc_disable_user_reset(
		Rstc *p_rstc)
{
	/* Validate the parameters. */
	Assert(p_rstc);

	uint32_t mode = p_rstc->RSTC_MR;

	mode &= ~(RSTC_MR_URSTEN | RSTC_MR_KEY_Msk);
	mode |= RSTC_KEY;

	p_rstc->RSTC_MR = mode;
}

/**
 * \brief Enable the User Reset interrupt.
 *
 * \param[in,out] p_rstc Module hardware register base address pointer
 */
void rstc_enable_user_reset_interrupt(
		Rstc *p_rstc)
{
	/* Validate the parameters. */
	Assert(p_rstc);

	uint32_t mode = p_rstc->RSTC_MR;

	mode &= ~RSTC_MR_KEY_Msk;
	mode |= (RSTC_MR_URSTIEN | RSTC_KEY);

	p_rstc->RSTC_MR = mode;
}

/**
 * \brief Disable the User Reset interrupt.
 *
 * \param[in,out] p_rstc Module hardware register base address pointer
 */
void rstc_disable_user_reset_interrupt(
		Rstc *p_rstc)
{
	/* Validate the parameters. */
	Assert(p_rstc);

	uint32_t mode = p_rstc->RSTC_MR;

	mode &= ~(RSTC_MR_URSTIEN | RSTC_MR_KEY_Msk);
	mode |= RSTC_KEY;

	p_rstc->RSTC_MR = mode;
}

/**
 * \brief Configure User Reset to be detected asynchronously on NRST pin.
 *
 * \param[in,out] p_rstc Module hardware register base address pointer
 */
void rstc_user_reset_config_async_detection(
		Rstc *p_rstc)
{
	/* Validate the parameters. */
	Assert(p_rstc);

	uint32_t mode = p_rstc->RSTC_MR;

	mode &= ~RSTC_MR_KEY_Msk;
	mode |= (RSTC_MR_URSTASYNC | RSTC_KEY);

	p_rstc->RSTC_MR = mode;
}

/**
 * \brief Configure User Reset to be detected synchronously (on next SLCK rising edge) on NRST pin.
 *
 * \param[in,out] p_rstc Module hardware register base address pointer
 */
void rstc_user_reset_config_sync_detection(
		Rstc *p_rstc)
{
	/* Validate the parameters. */
	Assert(p_rstc);

	uint32_t mode = p_rstc->RSTC_MR;

	mode &= ~(RSTC_MR_URSTASYNC | RSTC_MR_KEY_Msk);
	mode |= RSTC_KEY;

	p_rstc->RSTC_MR = mode;
}

/**
 * \brief Enable the Slow Clock Switching from Crystal to RC oscillator upon failure detection.
 *
 * \param[in,out] p_rstc Module hardware register base address pointer
 */
void rstc_enable_slow_clock_switching(
		Rstc *p_rstc)
{
	/* Validate the parameters. */
	Assert(p_rstc);

	uint32_t mode = p_rstc->RSTC_MR;

	mode &= ~RSTC_MR_KEY_Msk;
	mode |= (RSTC_MR_SCKSW | RSTC_KEY);

	p_rstc->RSTC_MR = mode;
}

/**
 * \brief Disable the Slow Clock Switching from Crystal to RC oscillator upon failure detection.
 *
 * \param[in,out] p_rstc Module hardware register base address pointer
 */
void rstc_disable_slow_clock_switching(
		Rstc *p_rstc)
{
	/* Validate the parameters. */
	Assert(p_rstc);

	uint32_t mode = p_rstc->RSTC_MR;

	mode &= ~(RSTC_MR_SCKSW | RSTC_MR_KEY_Msk);
	mode |= RSTC_KEY;

	p_rstc->RSTC_MR = mode;
}

/**
 * \brief Enable VDDCORE domain reset when a failure is detected on CPU clock.
 *
 * \param[in,out] p_rstc Module hardware register base address pointer
 */
void rstc_enable_reset_on_cpu_clock_fail(
		Rstc *p_rstc)
{
	/* Validate the parameters. */
	Assert(p_rstc);

	uint32_t mode = p_rstc->RSTC_MR;

	mode &= ~RSTC_MR_KEY_Msk;
	mode |= (RSTC_MR_CPUFEN | RSTC_KEY);

	p_rstc->RSTC_MR = mode;
}

/**
 * \brief Disable VDDCORE domain reset when a failure is detected on CPU clock.
 *
 * \param[in,out] p_rstc Module hardware register base address pointer
 */
void rstc_disable_reset_on_cpu_clock_fail(
		Rstc *p_rstc)
{
	/* Validate the parameters. */
	Assert(p_rstc);

	uint32_t mode = p_rstc->RSTC_MR;

	mode &= ~(RSTC_MR_CPUFEN | RSTC_MR_KEY_Msk);
	mode |= RSTC_KEY;

	p_rstc->RSTC_MR = mode;
}

/**
 * \brief Enable PMC Reset upon Software Reset.
 *
 * \param[in,out] p_rstc Module hardware register base address pointer
 */
void rstc_enable_pmc_reset_on_software_reset(
		Rstc *p_rstc)
{
	/* Validate the parameters. */
	Assert(p_rstc);

	uint32_t mode = p_rstc->RSTC_MR;

	mode &= ~RSTC_MR_KEY_Msk;
	mode |= (RSTC_MR_SFTPMCRS | RSTC_KEY);

	p_rstc->RSTC_MR = mode;
}

/**
 * \brief Disable PMC Reset upon Software Reset.
 *
 * \param[in,out] p_rstc Module hardware register base address pointer
 */
void rstc_disable_pmc_reset_on_software_reset(
		Rstc *p_rstc)
{
	/* Validate the parameters. */
	Assert(p_rstc);

	uint32_t mode = p_rstc->RSTC_MR;

	mode &= ~(RSTC_MR_SFTPMCRS | RSTC_MR_KEY_Msk);
	mode |= RSTC_KEY;

	p_rstc->RSTC_MR = mode;
}

/**
 * \brief Enable PMC Reset upon Watchdog Timer 0 Reset.
 *
 * \param[in,out] p_rstc Module hardware register base address pointer
 */
void rstc_enable_pmc_reset_on_watchdog_0_reset(
		Rstc *p_rstc)
{
	/* Validate the parameters. */
	Assert(p_rstc);

	uint32_t mode = p_rstc->RSTC_MR;

	mode &= ~RSTC_MR_KEY_Msk;
	mode |= (RSTC_MR_WDTPMC0 | RSTC_KEY);

	p_rstc->RSTC_MR = mode;
}

/**
 * \brief Disable PMC Reset upon Watchdog Timer 0 Reset.
 *
 * \param[in,out] p_rstc Module hardware register base address pointer
 */
void rstc_disable_pmc_reset_on_watchdog_0_reset(
		Rstc *p_rstc)
{
	/* Validate the parameters. */
	Assert(p_rstc);

	uint32_t mode = p_rstc->RSTC_MR;

	mode &= ~(RSTC_MR_WDTPMC0 | RSTC_MR_KEY_Msk);
	mode |= RSTC_KEY;

	p_rstc->RSTC_MR = mode;
}

/**
 * \brief Enable PMC Reset upon Watchdog Timer 1 Reset.
 *
 * \param[in,out] p_rstc Module hardware register base address pointer
 */
void rstc_enable_pmc_reset_on_watchdog_1_reset(
		Rstc *p_rstc)
{
	/* Validate the parameters. */
	Assert(p_rstc);

	uint32_t mode = p_rstc->RSTC_MR;

	mode &= ~RSTC_MR_KEY_Msk;
	mode |= (RSTC_MR_WDTPMC1 | RSTC_KEY);

	p_rstc->RSTC_MR = mode;
}

/**
 * \brief Disable PMC Reset upon Watchdog Timer 1 Reset.
 *
 * \param[in,out] p_rstc Module hardware register base address pointer
 */
void rstc_disable_pmc_reset_on_watchdog_1_reset(
		Rstc *p_rstc)
{
	/* Validate the parameters. */
	Assert(p_rstc);

	uint32_t mode = p_rstc->RSTC_MR;

	mode &= ~(RSTC_MR_WDTPMC1 | RSTC_MR_KEY_Msk);
	mode |= RSTC_KEY;

	p_rstc->RSTC_MR = mode;
}

/**
 * \brief Enable VDDCORE Supply Monitor Event Interrupt.
 *
 * \param[in,out] p_rstc Module hardware register base address pointer
 */
void rstc_enable_supply_monitor_interrupt(
		Rstc *p_rstc)
{
	/* Validate the parameters. */
	Assert(p_rstc);

	uint32_t mode = p_rstc->RSTC_MR;

	mode &= ~RSTC_MR_KEY_Msk;
	mode |= (RSTC_MR_CORSMIEN | RSTC_KEY);

	p_rstc->RSTC_MR = mode;
}

/**
 * \brief Disable VDDCORE Supply Monitor Event Interrupt.
 *
 * \param[in,out] p_rstc Module hardware register base address pointer
 */
void rstc_disable_supply_monitor_interrupt(
		Rstc *p_rstc)
{
	/* Validate the parameters. */
	Assert(p_rstc);

	uint32_t mode = p_rstc->RSTC_MR;

	mode &= ~(RSTC_MR_CORSMIEN | RSTC_MR_KEY_Msk);
	mode |= RSTC_KEY;

	p_rstc->RSTC_MR = mode;
}

/**
 * \brief Enable Reset in case of Failure detected on Slow Clock Crystal.
 *
 * \param[in,out] p_rstc Module hardware register base address pointer
 */
void rstc_enable_reset_on_slow_clock_failure(
		Rstc *p_rstc)
{
	/* Validate the parameters. */
	Assert(p_rstc);

	uint32_t mode = p_rstc->RSTC_MR;

	mode &= ~RSTC_MR_KEY_Msk;
	mode |= (RSTC_MR_BADXTRST | RSTC_KEY);

	p_rstc->RSTC_MR = mode;
}

/**
 * \brief Disable Reset in case of Failure detected on Slow Clock Crystal.
 *
 * \param[in,out] p_rstc Module hardware register base address pointer
 */
void rstc_disable_reset_on_slow_clock_failure(
		Rstc *p_rstc)
{
	/* Validate the parameters. */
	Assert(p_rstc);

	uint32_t mode = p_rstc->RSTC_MR;

	mode &= ~(RSTC_MR_BADXTRST | RSTC_MR_KEY_Msk);
	mode |= RSTC_KEY;

	p_rstc->RSTC_MR = mode;
}

/**
 * \brief Configure Backup Area to be supplied by VBAT.
 *
 * \param[in,out] p_rstc Module hardware register base address pointer
 */
void rstc_config_backup_area_supplied_vbat(
		Rstc *p_rstc)
{
	/* Validate the parameters. */
	Assert(p_rstc);

	uint32_t mode = p_rstc->RSTC_MR;

	mode &= ~RSTC_MR_KEY_Msk;
	mode |= (RSTC_MR_PWRSW | RSTC_KEY);

	p_rstc->RSTC_MR = mode;
}

/**
 * \brief Configure Backup Area to be supplied by VDDIO.
 *
 * \param[in,out] p_rstc Module hardware register base address pointer
 */
void rstc_config_backup_area_supplied_vddio(
		Rstc *p_rstc)
{
	/* Validate the parameters. */
	Assert(p_rstc);

	uint32_t mode = p_rstc->RSTC_MR;

	mode &= ~(RSTC_MR_PWRSW | RSTC_MR_KEY_Msk);
	mode |= RSTC_KEY;

	p_rstc->RSTC_MR = mode;
}

/**
 * \brief Perform a Software Reset.
 *
 * \param[out] p_rstc Module hardware register base address pointer
 */
void rstc_start_software_reset(
		Rstc *p_rstc)
{
	p_rstc->RSTC_CR = RSTC_KEY | RSTC_CR_PROCRST | RSTC_CR_PERRST;
}

/**
 * \brief Asserts the NRST pin for external resets.
 *
 * \param[out] p_rstc Module hardware register base address pointer
 */
void rstc_reset_extern(
		Rstc *p_rstc)
{
	p_rstc->RSTC_CR = RSTC_KEY | RSTC_CR_EXTRST;
}

/**
 * \brief Get the RSTC status.
 *
 * \param[in] p_rstc Module hardware register base address pointer
 *
 * \return RSTC status.
 */
uint32_t rstc_get_status(
		Rstc *p_rstc)
{
	return p_rstc->RSTC_SR;
}

/**
 * \brief Get the reset cause.
 *
 * \param[in] p_rstc Module hardware register base address pointer
 *
 * \return The last reset cause.
 */
uint32_t rstc_get_reset_cause(
		Rstc *p_rstc)
{
	return (p_rstc->RSTC_SR & RSTC_SR_RSTTYP_Msk);
}

/* / @cond */
/**INDENT-OFF**/
#ifdef __cplusplus
}
#endif
/**INDENT-ON**/
/* / @endcond */
