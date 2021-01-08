/**
 * \file
 *
 * \brief Power Management Controller (PMC) driver for PIC32CX.
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

#include "pic32cx.h"
#include "stdint.h"
#include "stdbool.h"
#include "pmc.h"
#include "pll.h"

#define MAX_PERIPH_ID    96

/* / @cond 0 */
/**INDENT-OFF**/
#ifdef __cplusplus
extern "C" {
#endif
/**INDENT-ON**/
/* / @endcond */

/**
 * \defgroup sam_drivers_pmc_group Power Management Controller (PMC)
 *
 * \par Purpose
 *
 * The Power Management Controller (PMC) optimizes power consumption by
 * controlling all system and user peripheral clocks. The PMC enables/disables
 * the clock inputs to many of the peripherals and the Cortex-M Processor.
 *
 * @{
 */

/**
 * \brief Set the prescaler of the MCK.
 *
 * \param ul_pres Prescaler value.
 */
void pmc_mck_set_prescaler(uint32_t ul_pres)
{
	PMC->PMC_CPU_CKR = (PMC->PMC_CPU_CKR & (~PMC_CPU_CKR_PRES_Msk)) | ul_pres;
	while (!(PMC->PMC_SR & PMC_SR_MCKRDY)) {
	}
}

/**
 * \brief Set the source of the MCK.
 *
 * \param ul_source Source selection value.
 */
void pmc_mck_set_source(uint32_t ul_source)
{
	PMC->PMC_CPU_CKR = (PMC->PMC_CPU_CKR & (~PMC_CPU_CKR_CSS_Msk)) | ul_source;
	while (!(PMC->PMC_SR & PMC_SR_MCKRDY)) {
	}
}

/**
 * \brief Switch master clock source selection to slow clock.
 *
 * \param ul_pres Processor clock prescaler.
 *
 * \retval 0 Success.
 * \retval 1 Timeout error.
 */
uint32_t pmc_switch_mck_to_sclk(uint32_t ul_pres)
{
	uint32_t ul_timeout;

	PMC->PMC_CPU_CKR = (PMC->PMC_CPU_CKR & (~PMC_CPU_CKR_CSS_Msk)) | PMC_CPU_CKR_CSS_SLOW_CLK;
	for (ul_timeout = PMC_TIMEOUT; !(PMC->PMC_SR & PMC_SR_MCKRDY); --ul_timeout) {
		if (ul_timeout == 0) {
			return 1;
		}
	}

	PMC->PMC_CPU_CKR = (PMC->PMC_CPU_CKR & (~PMC_CPU_CKR_PRES_Msk)) | ul_pres;
	for (ul_timeout = PMC_TIMEOUT; !(PMC->PMC_SR & PMC_SR_MCKRDY); --ul_timeout) {
		if (ul_timeout == 0) {
			return 1;
		}
	}

	return 0;
}

/**
 * \brief Switch master clock source selection to main clock.
 *
 * \param ul_pres Processor clock prescaler.
 *
 * \retval 0 Success.
 * \retval 1 Timeout error.
 */
uint32_t pmc_switch_mck_to_mainck(uint32_t ul_pres)
{
	uint32_t ul_timeout;

	PMC->PMC_CPU_CKR = (PMC->PMC_CPU_CKR & (~PMC_CPU_CKR_CSS_Msk)) | PMC_CPU_CKR_CSS_MAIN_CLK;
	for (ul_timeout = PMC_TIMEOUT; !(PMC->PMC_SR & PMC_SR_MCKRDY); --ul_timeout) {
		if (ul_timeout == 0) {
			return 1;
		}
	}

	PMC->PMC_CPU_CKR = (PMC->PMC_CPU_CKR & (~PMC_CPU_CKR_PRES_Msk)) | ul_pres;
	for (ul_timeout = PMC_TIMEOUT; !(PMC->PMC_SR & PMC_SR_MCKRDY); --ul_timeout) {
		if (ul_timeout == 0) {
			return 1;
		}
	}

	return 0;
}

/**
 * \brief Switch master clock source selection to PLLA clock.
 *
 * \param ul_pres Processor clock prescaler.
 *
 * \retval 0 Success.
 * \retval 1 Timeout error.
 */
uint32_t pmc_switch_mck_to_pllack(uint32_t ul_pres)
{
	uint32_t ul_timeout;

	PMC->PMC_CPU_CKR = (PMC->PMC_CPU_CKR & (~PMC_CPU_CKR_PRES_Msk)) | ul_pres;
	for (ul_timeout = PMC_TIMEOUT; !(PMC->PMC_SR & PMC_SR_MCKRDY); --ul_timeout) {
		if (ul_timeout == 0) {
			return 1;
		}
	}

	PMC->PMC_CPU_CKR = (PMC->PMC_CPU_CKR & (~PMC_CPU_CKR_CSS_Msk)) | PMC_CPU_CKR_CSS_PLLACK1;
	for (ul_timeout = PMC_TIMEOUT; !(PMC->PMC_SR & PMC_SR_MCKRDY); --ul_timeout) {
		if (ul_timeout == 0) {
			return 1;
		}
	}

	return 0;
}

/**
 * \brief Switch master clock source selection to PLLB clock.
 *
 * \param ul_pres Processor clock prescaler.
 *
 * \retval 0 Success.
 * \retval 1 Timeout error.
 */
uint32_t pmc_switch_mck_to_pllbck(uint32_t ul_pres)
{
	uint32_t ul_timeout;

	PMC->PMC_CPU_CKR = (PMC->PMC_CPU_CKR & (~PMC_CPU_CKR_PRES_Msk)) | ul_pres;
	for (ul_timeout = PMC_TIMEOUT; !(PMC->PMC_SR & PMC_SR_MCKRDY); --ul_timeout) {
		if (ul_timeout == 0) {
			return 1;
		}
	}

	PMC->PMC_CPU_CKR = (PMC->PMC_CPU_CKR & (~PMC_CPU_CKR_CSS_Msk)) | PMC_CPU_CKR_CSS_PLLBCK;
	for (ul_timeout = PMC_TIMEOUT; !(PMC->PMC_SR & PMC_SR_MCKRDY); --ul_timeout) {
		if (ul_timeout == 0) {
			return 1;
		}
	}

	return 0;
}

/**
 * \brief Switch slow clock source selection to external 32k (Xtal or Bypass).
 *
 * \note Switching SCLK back to 32krc is only possible by shutting down the VDDIO power supply.
 *
 * \param ul_bypass 0 for Xtal, 1 for bypass.
 */
void pmc_switch_sclk_to_32kxtal(uint32_t ul_bypass)
{
	/* Set Bypass mode if required */
	if (ul_bypass == 1) {
		SUPC->SUPC_MR |= SUPC_MR_KEY_PASSWD | SUPC_MR_OSCBYPASS;
	}

	SUPC->SUPC_CR = SUPC_CR_KEY_PASSWD | SUPC_CR_TDXTALSEL_XTAL32K;
}

/**
 * \brief Check if the external 32k Xtal is ready.
 *
 * \retval 1 External 32k Xtal is ready.
 * \retval 0 External 32k Xtal is not ready.
 */
uint32_t pmc_osc_is_ready_32kxtal(void)
{
	return ((SUPC->SUPC_SR & SUPC_SR_TDOSCSEL_XTAL) && (PMC->PMC_SR & PMC_SR_OSCSELS));
}

/**
 * \brief Switch main clock source selection to internal fast RC.
 *
 */
void pmc_switch_mainck_to_fastrc(void)
{
	/* Enable Fast RC oscillator but DO NOT switch to RC now */
	PMC->CKGR_MOR |= (CKGR_MOR_KEY_PASSWD | CKGR_MOR_MOSCRCEN);

	/* Wait the Fast RC to stabilize */
	while (!(PMC->PMC_SR & PMC_SR_MOSCRCS)) {
	}

	/* Switch to Fast RC */
	PMC->CKGR_MOR = (PMC->CKGR_MOR & ~CKGR_MOR_MOSCSEL) | CKGR_MOR_KEY_PASSWD;
}

/**
 * \brief Enable fast RC oscillator.
 *
 */
void pmc_osc_enable_fastrc(void)
{
	/* Enable Fast RC oscillator but DO NOT switch to RC */
	PMC->CKGR_MOR |= (CKGR_MOR_KEY_PASSWD | CKGR_MOR_MOSCRCEN);

	/* Wait the Fast RC to stabilize */
	while (!(PMC->PMC_SR & PMC_SR_MOSCRCS)) {
	}
}

/**
 * \brief Disable the internal fast RC.
 */
void pmc_osc_disable_fastrc(void)
{
	/* Disable Fast RC oscillator */
	PMC->CKGR_MOR = (PMC->CKGR_MOR & ~CKGR_MOR_MOSCRCEN) | CKGR_MOR_KEY_PASSWD;
}

/**
 * \brief Check if the main fastrc is ready.
 *
 * \retval 0 Xtal is not ready, otherwise ready.
 */
uint32_t pmc_osc_is_ready_fastrc(void)
{
	return (PMC->PMC_SR & PMC_SR_MOSCRCS);
}

/**
 * \brief Enable main XTAL oscillator.
 *
 * \param ul_xtal_startup_time Xtal start-up time, in number of slow clocks.
 */
void pmc_osc_enable_main_xtal(uint32_t ul_xtal_startup_time)
{
	uint32_t mor = PMC->CKGR_MOR;

	mor &= ~(CKGR_MOR_MOSCXTBY | CKGR_MOR_MOSCXTEN);
	mor |= CKGR_MOR_KEY_PASSWD | CKGR_MOR_MOSCXTEN | CKGR_MOR_MOSCXTST(ul_xtal_startup_time);
	PMC->CKGR_MOR = mor;

	/* Wait the main Xtal to stabilize */
	while (!(PMC->PMC_SR & PMC_SR_MOSCXTS)) {
	}
}

/**
 * \brief Bypass main XTAL.
 */
void pmc_osc_bypass_main_xtal(void)
{
	uint32_t mor = PMC->CKGR_MOR;

	mor &= ~(CKGR_MOR_MOSCXTBY | CKGR_MOR_MOSCXTEN);
	mor |= CKGR_MOR_KEY_PASSWD | CKGR_MOR_MOSCXTBY;

	/* Enable Crystal oscillator but DO NOT switch now. Keep MOSCSEL to 0 */
	PMC->CKGR_MOR = mor;
	/* The MOSCXTS in PMC_SR is automatically set */
}

/**
 * \brief Disable the main Xtal.
 */
void pmc_osc_disable_main_xtal(void)
{
	uint32_t mor = PMC->CKGR_MOR;

	mor &= ~(CKGR_MOR_MOSCXTBY | CKGR_MOR_MOSCXTEN);
	PMC->CKGR_MOR = CKGR_MOR_KEY_PASSWD | mor;
}

/**
 * \brief Check if the main crystal is bypassed.
 *
 * \retval 0 Xtal is bypassed, otherwise not.
 */
uint32_t pmc_osc_is_bypassed_main_xtal(void)
{
	return (PMC->CKGR_MOR & CKGR_MOR_MOSCXTBY);
}

/**
 * \brief Check if the main crystal is ready.
 *
 * \note If main crystal is bypassed, it's always ready.
 *
 * \retval 0 main crystal is not ready, otherwise ready.
 */
uint32_t pmc_osc_is_ready_main_xtal(void)
{
	return (PMC->PMC_SR & PMC_SR_MOSCXTS);
}

/**
 * \brief Switch main clock source selection to external Xtal/Bypass.
 *
 * \note The function may switch MCK to SCLK if MCK source is MAINCK to avoid
 *       any system crash.
 *
 * \note If used in Xtal mode, the Xtal is automatically enabled.
 *
 * \param ul_bypass                0 for Xtal, 1 for bypass.
 * \param ul_xtal_startup_time     Main Crystal Oscillator Start-up Time.
 */
void pmc_switch_mainck_to_xtal(uint32_t ul_bypass, uint32_t ul_xtal_startup_time)
{
	/* Enable Main Xtal oscillator */
	if (ul_bypass) {
		PMC->CKGR_MOR = (PMC->CKGR_MOR & ~CKGR_MOR_MOSCXTEN) |
				CKGR_MOR_KEY_PASSWD | CKGR_MOR_MOSCXTBY |
				CKGR_MOR_MOSCSEL;
	} else {
		PMC->CKGR_MOR = (PMC->CKGR_MOR & ~CKGR_MOR_MOSCXTBY) |
				CKGR_MOR_KEY_PASSWD | CKGR_MOR_MOSCXTEN |
				CKGR_MOR_MOSCXTST(ul_xtal_startup_time);

		/* Wait the Xtal to stabilize */
		while (!(PMC->PMC_SR & PMC_SR_MOSCXTS)) {
		}

		PMC->CKGR_MOR |= CKGR_MOR_KEY_PASSWD | CKGR_MOR_MOSCSEL;
	}
}

/**
 * \brief Disable the external Xtal.
 *
 * \param ul_bypass 0 for Xtal, 1 for bypass.
 */
void pmc_osc_disable_xtal(uint32_t ul_bypass)
{
	/* Disable xtal oscillator */
	if (ul_bypass) {
		PMC->CKGR_MOR = (PMC->CKGR_MOR & ~CKGR_MOR_MOSCXTBY) | CKGR_MOR_KEY_PASSWD;
	} else {
		PMC->CKGR_MOR = (PMC->CKGR_MOR & ~CKGR_MOR_MOSCXTEN) | CKGR_MOR_KEY_PASSWD;
	}
}

/**
 * \brief Check if the MAINCK is ready. Depending on MOSCEL, MAINCK can be one
 * of Xtal, bypass or internal RC.
 *
 * \retval 1 Xtal is ready.
 * \retval 0 Xtal is not ready.
 */
uint32_t pmc_osc_is_ready_mainck(void)
{
	return PMC->PMC_SR & PMC_SR_MOSCSELS;
}

/**
 * \brief Select Main Crystal or internal RC as main clock source.
 *
 * \note This function will not enable/disable RC or Main Crystal.
 *
 * \param ul_xtal_rc 0 internal RC is selected, otherwise Main Crystal.
 */
void pmc_mainck_osc_select(uint32_t ul_xtal_rc)
{
	if (ul_xtal_rc) {
		PMC->CKGR_MOR = (PMC->CKGR_MOR | CKGR_MOR_MOSCSEL) | CKGR_MOR_KEY_PASSWD;
	} else {
		PMC->CKGR_MOR = (PMC->CKGR_MOR & ~CKGR_MOR_MOSCSEL) | CKGR_MOR_KEY_PASSWD;
	}
}

/**
 * \brief Check if the MCK0DIV is enabled.
 *
 * \retval 1 MCK0DIV is enabled.
 * \retval 0 MCK0DIV is disabled.
 */
uint32_t pmc_is_mck0div_enabled(void)
{
	return (((PMC->PMC_CPU_CKR & PMC_CPU_CKR_RATIO_MCK0DIV) ? 1 : 0));
}

/**
 * \brief Enable MCK0DIV
 */
void pmc_enable_mck0div(void)
{
	PMC->PMC_CPU_CKR |= PMC_CPU_CKR_RATIO_MCK0DIV;
}

/**
 * \brief Disable MCK0DIV
 */
void pmc_disable_mck0div(void)
{
	PMC->PMC_CPU_CKR &= ~PMC_CPU_CKR_RATIO_MCK0DIV;
}

/**
 * \brief Check if the MCK0DIV2 is enabled.
 *
 * \retval 1 MCK0DIV2 is enabled.
 * \retval 0 MCK0DIV2 is disabled.
 */
uint32_t pmc_is_mck0div2_enabled(void)
{
	return (((PMC->PMC_CPU_CKR & PMC_CPU_CKR_RATIO_MCK0DIV2) ? 1 : 0));
}

/**
 * \brief Enable MCK0DIV2
 */
void pmc_enable_mck0div2(void)
{
	PMC->PMC_CPU_CKR |= PMC_CPU_CKR_RATIO_MCK0DIV2;
}

/**
 * \brief Disable MCK0DIV2
 */
void pmc_disable_mck0div2(void)
{
	PMC->PMC_CPU_CKR &= ~PMC_CPU_CKR_RATIO_MCK0DIV2;
}

/**
 * \brief Enable PLL clock.
 *
 * \param pll_cfg Pointer to PLL configuration data.
 */
void pmc_enable_pll(const struct _pmc_pll_cfg *pll_cfg)
{
	uint32_t pll_ctrl0;
	uint32_t pll_upd;

	/* Select PLL ID */
	pll_upd = PMC_PLL_UPDT_STUPTIM(pll_cfg->stuptim) | pll_cfg->pll_id;
	PMC->PMC_PLL_UPDT = pll_upd;

	/* Set Loop Filter */
	PMC->PMC_PLL_ACR &= ~PMC_PLL_ACR_LOOP_FILTER_Msk;
	PMC->PMC_PLL_ACR |= PMC_PLL_ACR_LOOP_FILTER(pll_cfg->loop_filter);

	/* Set Control */
	PMC->PMC_PLL_ACR &= ~PMC_PLL_ACR_CONTROL_Msk;
	PMC->PMC_PLL_ACR |= PMC_PLL_ACR_CONTROL(pll_cfg->control);

	/* Set MUL */
	PMC->PMC_PLL_CTRL1 = PMC_PLL_CTRL1_MUL(pll_cfg->mul);

	/* Set FRACR */
	PMC->PMC_PLL_CTRL2 = PMC_PLL_CTRL2_FRACR(pll_cfg->fracr);

	/* Update changes */
	PMC->PMC_PLL_UPDT = pll_upd | PMC_PLL_UPDT_UPDATE;

	/* Set DIVs and config outputs */
	pll_ctrl0 = PMC_PLL_CTRL0_ENLOCK | PMC_PLL_CTRL0_ENPLLO0 | PMC_PLL_CTRL0_ENPLL |
			PMC_PLL_CTRL0_DIVPMC1(pll_cfg->div1) | PMC_PLL_CTRL0_DIVPMC0(pll_cfg->div0);

	if (pll_cfg->pll_id == PLLA_ID) {
		pll_ctrl0 |= PMC_PLL_CTRL0_ENPLLO1;
	}

	/* Set PLL source */
	if ((pll_cfg->pll_src == OSC_MAINCK_12M_RC) || (pll_cfg->pll_src == OSC_MAINCK_XTAL) ||
			(pll_cfg->pll_src == OSC_MAINCK_BYPASS)) {
		pll_ctrl0 |= PMC_PLL_CTRL0_PLLMS;
	} else {
		pll_ctrl0 &= ~PMC_PLL_CTRL0_PLLMS;
	}

	PMC->PMC_PLL_CTRL0 = pll_ctrl0;

	/* Update changes */
	PMC->PMC_PLL_UPDT = pll_upd | PMC_PLL_UPDT_UPDATE;

	/* Wait LOCK bit */
	while ((PMC->PMC_PLL_ISR0 & (PMC_PLL_ISR0_LOCK0 << pll_cfg->pll_id)) == 0) {
	}

	/* Enable UNLOCK interrupt */
	PMC->PMC_PLL_IER = (PMC_PLL_IER_UNLOCK0 << pll_cfg->pll_id);
}

/**
 * \brief Disable PLLA clock.
 *
 * \param ul_pll_id   PLL identificator
 */
void pmc_disable_pll(uint32_t ul_pll_id)
{
	uint32_t pll_ctrl0;

	/* Disable UNLOCK interrupt */
	PMC->PMC_PLL_IDR = (PMC_PLL_IDR_UNLOCK0 << ul_pll_id);

	/* Select PLL ID */
	PMC->PMC_PLL_UPDT &= ~(PMC_PLL_UPDT_ID_Msk | PMC_PLL_UPDT_UPDATE);
	PMC->PMC_PLL_UPDT |= ul_pll_id;

	/* Clear Outputs */
	pll_ctrl0 = PMC->PMC_PLL_CTRL0;
	pll_ctrl0 &= ~(PMC_PLL_CTRL0_ENPLLO0 | PMC_PLL_CTRL0_ENPLLO1);
	PMC->PMC_PLL_CTRL0 = pll_ctrl0;

	/* Update changes */
	PMC->PMC_PLL_UPDT |= PMC_PLL_UPDT_UPDATE;

	/* Disable PLL */
	PMC->PMC_PLL_CTRL0 &= ~PMC_PLL_CTRL0_ENPLL;
}

/**
 * \brief Get PLL configuration.
 *
 * \param ul_pll_id   PLL identificator
 * \param pll_cfg     Pointer to PLL configuration struct
 */
void pmc_get_pll_config(uint32_t ul_pll_id, struct _pmc_pll_cfg *pll_cfg)
{
	pll_cfg->pll_id = ul_pll_id;

	/* Select PLL ID */
	PMC->PMC_PLL_UPDT &= ~(PMC_PLL_UPDT_ID_Msk | PMC_PLL_UPDT_UPDATE);
	PMC->PMC_PLL_UPDT |= ul_pll_id;

	/* Fill configuration */
	pll_cfg->div0 = (PMC->PMC_PLL_CTRL0 & PMC_PLL_CTRL0_DIVPMC0_Msk) >> PMC_PLL_CTRL0_DIVPMC0_Pos;
	pll_cfg->div1 = (PMC->PMC_PLL_CTRL0 & PMC_PLL_CTRL0_DIVPMC1_Msk) >> PMC_PLL_CTRL0_DIVPMC1_Pos;
	pll_cfg->fracr = (PMC->PMC_PLL_CTRL2 & PMC_PLL_CTRL2_FRACR_Msk) >> PMC_PLL_CTRL2_FRACR_Pos;
	pll_cfg->mul = (PMC->PMC_PLL_CTRL1 & PMC_PLL_CTRL1_MUL_Msk) >> PMC_PLL_CTRL1_MUL_Pos;
	pll_cfg->pll_src = (PMC->PMC_PLL_CTRL0 & PMC_PLL_CTRL0_PLLMS) >> 27;
	pll_cfg->stuptim = (PMC->PMC_PLL_UPDT & PMC_PLL_UPDT_STUPTIM_Msk) >> PMC_PLL_UPDT_STUPTIM_Pos;
}

/**
 * \brief Is PLL locked?
 *
 * \param ul_pll_id   PLL identificator
 *
 * \retval 0 Not locked.
 * \retval 1 Locked.
 */
uint32_t pmc_is_locked_pll(uint32_t ul_pll_id)
{
	return ((PMC->PMC_PLL_ISR0 >> ul_pll_id) & 0x01);
}

/**
 * \brief Configure Peripheral Clock
 *
 * \note The ID must NOT be shifted (i.e., 1 << ID_xxx).
 *
 * \param ul_id    Peripheral ID (ID_xxx).
 * \param ul_div   Peripheral Clock Division Ratio
 *
 * \retval 0 Success.
 * \retval 1 Invalid parameter.
 */
uint32_t pmc_configure_peripheral(uint32_t ul_id, uint32_t ul_div)
{
	uint32_t ul_pcr;

	if (ul_id > MAX_PERIPH_ID) {
		return 1;
	}

	pmc_disable_periph_clk(ul_id);
	pmc_disable_generic_clk(ul_id);

	ul_pcr = PMC->PMC_PCR;
	ul_pcr &= ~(PMC_PCR_DIV_Msk);
	ul_pcr |= (PMC_PCR_CMD | ul_div | PMC_PCR_PID(ul_id));

	PMC->PMC_PCR = ul_pcr;

	return 0;
}

/**
 * \brief Enable the specified peripheral clock.
 *
 * \note The ID must NOT be shifted (i.e., 1 << ID_xxx).
 *
 * \param ul_id Peripheral ID (ID_xxx).
 *
 * \retval 0 Success.
 * \retval 1 Invalid parameter.
 */
uint32_t pmc_enable_periph_clk(uint32_t ul_id)
{
	uint32_t ul_pcr;

	if (ul_id > MAX_PERIPH_ID) {
		return 1;
	}

    if (pmc_is_periph_clk_enabled(ul_id)) {
        return 0;
    }

	/* Read configuration for the PID */
	PMC->PMC_PCR = PMC_PCR_PID(ul_id);

	ul_pcr = PMC->PMC_PCR;
	ul_pcr &= ~PMC_PCR_PID_Msk;
	ul_pcr |= (PMC_PCR_CMD | PMC_PCR_EN | PMC_PCR_PID(ul_id));

	PMC->PMC_PCR = ul_pcr;

	return 0;
}

/**
 * \brief Disable the specified peripheral clock.
 *
 * \note The ID must NOT be shifted (i.e., 1 << ID_xxx).
 *
 * \param ul_id Peripheral ID (ID_xxx).
 *
 * \retval 0 Success.
 * \retval 1 Invalid parameter.
 */
uint32_t pmc_disable_periph_clk(uint32_t ul_id)
{
	uint32_t ul_pcr;

	if (ul_id > MAX_PERIPH_ID) {
		return 1;
	}

    if (!pmc_is_periph_clk_enabled(ul_id)) {
        return 0;
    }

	/* Read configuration for the PID */
	PMC->PMC_PCR = PMC_PCR_PID(ul_id);

	ul_pcr = PMC->PMC_PCR;
	ul_pcr &= ~(PMC_PCR_EN);
	ul_pcr |= (PMC_PCR_CMD | PMC_PCR_PID(ul_id));

	PMC->PMC_PCR = ul_pcr;

	return 0;
}

/**
 * \brief Check if the specified peripheral clock is enabled.
 *
 * \note The ID must NOT be shifted (i.e., 1 << ID_xxx).
 *
 * \param ul_id Peripheral ID (ID_xxx).
 *
 * \retval 0 Peripheral clock is disabled or unknown.
 * \retval 1 Peripheral clock is enabled.
 */
uint32_t pmc_is_periph_clk_enabled(uint32_t ul_id)
{
	if (ul_id < 32) {
		return ((PMC->PMC_CSR0 & PMC_MASK_PSTATUS0) & (1 << ul_id));
	} else if (ul_id < 64) {
		return ((PMC->PMC_CSR1 & PMC_MASK_PSTATUS1) & (1 << (ul_id - 32)));
	} else if (ul_id < 96) {
		return ((PMC->PMC_CSR2 & PMC_MASK_PSTATUS2) & (1 << (ul_id - 64)));
	} else if (ul_id <= MAX_PERIPH_ID) {
		return ((PMC->PMC_CSR3 & PMC_MASK_PSTATUS3) & (1 << (ul_id - 96)));
	} else {
		return 0;
	}
}

/**
 * \brief Configure Generic Clock
 *
 * \note The ID must NOT be shifted (i.e., 1 << ID_xxx).
 *
 * \param ul_id             Peripheral ID (ID_xxx).
 * \param ul_clock_source   Generic Clock Source
 * \param ul_div            Generic Clock Division Ratio
 *
 * \retval 0 Success.
 * \retval 1 Invalid parameter.
 */
uint32_t pmc_configure_generic(uint32_t ul_id, uint32_t ul_clock_source, uint32_t ul_div)
{
	uint32_t ul_pcr;

	if (ul_id > MAX_PERIPH_ID) {
		return 1;
	}

	pmc_disable_generic_clk(ul_id);
	pmc_disable_periph_clk(ul_id);

	ul_pcr = PMC->PMC_PCR;
	ul_pcr &= ~(PMC_PCR_GCLKCSS_Msk | PMC_PCR_GCLKDIV_Msk | PMC_PCR_PID_Msk);
	ul_pcr |= PMC_PCR_CMD | ul_clock_source | ul_div | PMC_PCR_PID(ul_id);

	PMC->PMC_PCR = ul_pcr;

	return 0;
}

/**
 * \brief Enable the specified generic clock.
 *
 * \note The ID must NOT be shifted (i.e., 1 << ID_xxx).
 *
 * \param ul_id Peripheral ID (ID_xxx).
 *
 * \retval 0 Success.
 * \retval 1 Invalid parameter.
 */
uint32_t pmc_enable_generic_clk(uint32_t ul_id)
{
	uint32_t ul_pcr;

	if (ul_id > MAX_PERIPH_ID) {
		return 1;
	}

	/* Read configuration for the PID */
	PMC->PMC_PCR = PMC_PCR_PID(ul_id);

	ul_pcr = PMC->PMC_PCR;
	ul_pcr |= (PMC_PCR_CMD | PMC_PCR_GCLKEN | PMC_PCR_PID(ul_id));

	PMC->PMC_PCR = ul_pcr;

	return 0;
}

/**
 * \brief Disable the specified generic clock.
 *
 * \note The ID must NOT be shifted (i.e., 1 << ID_xxx).
 *
 * \param ul_id Peripheral ID (ID_xxx).
 *
 * \retval 0 Success.
 * \retval 1 Invalid parameter.
 */
uint32_t pmc_disable_generic_clk(uint32_t ul_id)
{
	uint32_t ul_pcr;

	if (ul_id > MAX_PERIPH_ID) {
		return 1;
	}

	/* Read configuration for the PID */
	PMC->PMC_PCR = PMC_PCR_PID(ul_id);

	ul_pcr = PMC->PMC_PCR;
	ul_pcr &= ~(PMC_PCR_GCLKEN);
	ul_pcr |= (PMC_PCR_CMD | PMC_PCR_PID(ul_id));

	PMC->PMC_PCR = ul_pcr;

	return 0;
}

/**
 * \brief Check if the specified generic clock is enabled.
 *
 * \note The ID must NOT be shifted (i.e., 1 << ID_xxx).
 *
 * \param ul_id Peripheral ID (ID_xxx).
 *
 * \retval 0 Peripheral clock is disabled or unknown.
 * \retval 1 Peripheral clock is enabled.
 */
uint32_t pmc_is_generic_clk_enabled(uint32_t ul_id)
{
	if (ul_id < 32) {
		return ((PMC->PMC_GCSR0 & PMC_MASK_GSTATUS0) & (1 << ul_id));
	} else if (ul_id < 64) {
		return ((PMC->PMC_GCSR1 & PMC_MASK_GSTATUS1) & (1 << (ul_id - 32)));
	} else if (ul_id < 96) {
		return ((PMC->PMC_GCSR2 & PMC_MASK_GSTATUS2) & (1 << (ul_id - 64)));
	} else if (ul_id <= MAX_PERIPH_ID) {
		return ((PMC->PMC_GCSR3 & PMC_MASK_GSTATUS3) & (1 << (ul_id - 96)));
	} else {
		return 0;
	}
}

/**
 * \brief Set the prescaler for the specified programmable clock.
 *
 * \param ul_id Peripheral ID.
 * \param ul_pres Prescaler value.
 */
void pmc_pck_set_prescaler(uint32_t ul_id, uint32_t ul_pres)
{
	PMC->PMC_PCK[ul_id] = (PMC->PMC_PCK[ul_id] & ~PMC_PCK_PRES_Msk) | ul_pres;
	while ((PMC->PMC_SCER & (PMC_SCER_PCK0 << ul_id)) && !(PMC->PMC_SR & (PMC_SR_PCKRDY0 << ul_id))) {
	}
}

/**
 * \brief Set the source oscillator for the specified programmable clock.
 *
 * \param ul_id Peripheral ID.
 * \param ul_source Source selection value.
 */
void pmc_pck_set_source(uint32_t ul_id, uint32_t ul_source)
{
	PMC->PMC_PCK[ul_id] = (PMC->PMC_PCK[ul_id] & ~PMC_PCK_CSS_Msk) | ul_source;
	while ((PMC->PMC_SCER & (PMC_SCER_PCK0 << ul_id)) && !(PMC->PMC_SR & (PMC_SR_PCKRDY0 << ul_id))) {
	}
}

/**
 * \brief Switch programmable clock source selection to MD slow clock.
 *
 * \param ul_id Id of the programmable clock.
 * \param ul_pres Programmable clock prescaler.
 *
 * \retval 0 Success.
 * \retval 1 Timeout error.
 */
uint32_t pmc_switch_pck_to_md_sclk(uint32_t ul_id, uint32_t ul_pres)
{
	uint32_t ul_timeout;

	PMC->PMC_PCK[ul_id] = (PMC->PMC_PCK[ul_id] & ~PMC_PCK_CSS_Msk) | PMC_PCK_CSS_MD_SLOW_CLK;
	for (ul_timeout = PMC_TIMEOUT; !(PMC->PMC_SR & (PMC_SR_PCKRDY0 << ul_id)); --ul_timeout) {
		if (ul_timeout == 0) {
			return 1;
		}
	}

	PMC->PMC_PCK[ul_id] = (PMC->PMC_PCK[ul_id] & ~PMC_PCK_PRES_Msk) | ul_pres;
	for (ul_timeout = PMC_TIMEOUT; !(PMC->PMC_SR & (PMC_SR_PCKRDY0 << ul_id)); --ul_timeout) {
		if (ul_timeout == 0) {
			return 1;
		}
	}

	return 0;
}

/**
 * \brief Switch programmable clock source selection to TD slow clock.
 *
 * \param ul_id Id of the programmable clock.
 * \param ul_pres Programmable clock prescaler.
 *
 * \retval 0 Success.
 * \retval 1 Timeout error.
 */
uint32_t pmc_switch_pck_to_td_sclk(uint32_t ul_id, uint32_t ul_pres)
{
	uint32_t ul_timeout;

	PMC->PMC_PCK[ul_id] = (PMC->PMC_PCK[ul_id] & ~PMC_PCK_CSS_Msk) | PMC_PCK_CSS_TD_SLOW_CLOCK;
	for (ul_timeout = PMC_TIMEOUT; !(PMC->PMC_SR & (PMC_SR_PCKRDY0 << ul_id)); --ul_timeout) {
		if (ul_timeout == 0) {
			return 1;
		}
	}

	PMC->PMC_PCK[ul_id] = (PMC->PMC_PCK[ul_id] & ~PMC_PCK_PRES_Msk) | ul_pres;
	for (ul_timeout = PMC_TIMEOUT; !(PMC->PMC_SR & (PMC_SR_PCKRDY0 << ul_id)); --ul_timeout) {
		if (ul_timeout == 0) {
			return 1;
		}
	}

	return 0;
}

/**
 * \brief Switch programmable clock source selection to main clock.
 *
 * \param ul_id Id of the programmable clock.
 * \param ul_pres Programmable clock prescaler.
 *
 * \retval 0 Success.
 * \retval 1 Timeout error.
 */
uint32_t pmc_switch_pck_to_mainck(uint32_t ul_id, uint32_t ul_pres)
{
	uint32_t ul_timeout;

	PMC->PMC_PCK[ul_id] = (PMC->PMC_PCK[ul_id] & ~PMC_PCK_CSS_Msk) | PMC_PCK_CSS_MAINCK;
	for (ul_timeout = PMC_TIMEOUT; !(PMC->PMC_SR & (PMC_SR_PCKRDY0 << ul_id)); --ul_timeout) {
		if (ul_timeout == 0) {
			return 1;
		}
	}

	PMC->PMC_PCK[ul_id] = (PMC->PMC_PCK[ul_id] & ~PMC_PCK_PRES_Msk) | ul_pres;
	for (ul_timeout = PMC_TIMEOUT; !(PMC->PMC_SR & (PMC_SR_PCKRDY0 << ul_id)); --ul_timeout) {
		if (ul_timeout == 0) {
			return 1;
		}
	}

	return 0;
}

/**
 * \brief Switch programmable clock source selection to master clock 0.
 *
 * \param ul_id Id of the programmable clock.
 * \param ul_pres Programmable clock prescaler.
 *
 * \retval 0 Success.
 * \retval 1 Timeout error.
 */
uint32_t pmc_switch_pck_to_mck0(uint32_t ul_id, uint32_t ul_pres)
{
	uint32_t ul_timeout;

	PMC->PMC_PCK[ul_id] = (PMC->PMC_PCK[ul_id] & ~PMC_PCK_CSS_Msk) | PMC_PCK_CSS_MCK0;
	for (ul_timeout = PMC_TIMEOUT; !(PMC->PMC_SR & (PMC_SR_PCKRDY0 << ul_id)); --ul_timeout) {
		if (ul_timeout == 0) {
			return 1;
		}
	}

	PMC->PMC_PCK[ul_id] = (PMC->PMC_PCK[ul_id] & ~PMC_PCK_PRES_Msk) | ul_pres;
	for (ul_timeout = PMC_TIMEOUT; !(PMC->PMC_SR & (PMC_SR_PCKRDY0 << ul_id)); --ul_timeout) {
		if (ul_timeout == 0) {
			return 1;
		}
	}

	return 0;
}

/**
 * \brief Switch programmable clock source selection to PLLA clock 1.
 *
 * \param ul_id Id of the programmable clock.
 * \param ul_pres Programmable clock prescaler.
 *
 * \retval 0 Success.
 * \retval 1 Timeout error.
 */
uint32_t pmc_switch_pck_to_pllack1(uint32_t ul_id, uint32_t ul_pres)
{
	uint32_t ul_timeout;

	PMC->PMC_PCK[ul_id] = (PMC->PMC_PCK[ul_id] & ~PMC_PCK_CSS_Msk) | PMC_PCK_CSS_PLLACK1;
	for (ul_timeout = PMC_TIMEOUT; !(PMC->PMC_SR & (PMC_SR_PCKRDY0 << ul_id)); --ul_timeout) {
		if (ul_timeout == 0) {
			return 1;
		}
	}

	PMC->PMC_PCK[ul_id] = (PMC->PMC_PCK[ul_id] & ~PMC_PCK_PRES_Msk) | ul_pres;
	for (ul_timeout = PMC_TIMEOUT; !(PMC->PMC_SR & (PMC_SR_PCKRDY0 << ul_id)); --ul_timeout) {
		if (ul_timeout == 0) {
			return 1;
		}
	}

	return 0;
}

/**
 * \brief Switch programmable clock source selection to PLLB clock.
 *
 * \param ul_id Id of the programmable clock.
 * \param ul_pres Programmable clock prescaler.
 *
 * \retval 0 Success.
 * \retval 1 Timeout error.
 */
uint32_t pmc_switch_pck_to_pllbck0(uint32_t ul_id, uint32_t ul_pres)
{
	uint32_t ul_timeout;

	PMC->PMC_PCK[ul_id] = (PMC->PMC_PCK[ul_id] & ~PMC_PCK_CSS_Msk) | PMC_PCK_CSS_PLLBCK;
	for (ul_timeout = PMC_TIMEOUT; !(PMC->PMC_SR & (PMC_SR_PCKRDY0 << ul_id)); --ul_timeout) {
		if (ul_timeout == 0) {
			return 1;
		}
	}

	PMC->PMC_PCK[ul_id] = (PMC->PMC_PCK[ul_id] & ~PMC_PCK_PRES_Msk) | ul_pres;
	for (ul_timeout = PMC_TIMEOUT; !(PMC->PMC_SR & (PMC_SR_PCKRDY0 << ul_id)); --ul_timeout) {
		if (ul_timeout == 0) {
			return 1;
		}
	}

	return 0;
}

/**
 * \brief Switch programmable clock source selection to PLLC clock.
 *
 * \param ul_id Id of the programmable clock.
 * \param ul_pres Programmable clock prescaler.
 *
 * \retval 0 Success.
 * \retval 1 Timeout error.
 */
uint32_t pmc_switch_pck_to_pllcck0(uint32_t ul_id, uint32_t ul_pres)
{
	uint32_t ul_timeout;

	PMC->PMC_PCK[ul_id] = (PMC->PMC_PCK[ul_id] & ~PMC_PCK_CSS_Msk) | PMC_PCK_CSS_PLLCCK;
	for (ul_timeout = PMC_TIMEOUT; !(PMC->PMC_SR & (PMC_SR_PCKRDY0 << ul_id)); --ul_timeout) {
		if (ul_timeout == 0) {
			return 1;
		}
	}

	PMC->PMC_PCK[ul_id] = (PMC->PMC_PCK[ul_id] & ~PMC_PCK_PRES_Msk) | ul_pres;
	for (ul_timeout = PMC_TIMEOUT; !(PMC->PMC_SR & (PMC_SR_PCKRDY0 << ul_id)); --ul_timeout) {
		if (ul_timeout == 0) {
			return 1;
		}
	}

	return 0;
}

/**
 * \brief Switch programmable clock source selection to PLLC source clock.
 *
 * \param ul_id Id of the programmable clock.
 * \param ul_pres Programmable clock prescaler.
 *
 * \retval 0 Success.
 * \retval 1 Timeout error.
 */
uint32_t pmc_switch_pck_to_pllcsrc(uint32_t ul_id, uint32_t ul_pres)
{
	uint32_t ul_timeout;

	PMC->PMC_PCK[ul_id] = (PMC->PMC_PCK[ul_id] & ~PMC_PCK_CSS_Msk) | PMC_PCK_CSS_PLLCSRC;
	for (ul_timeout = PMC_TIMEOUT; !(PMC->PMC_SR & (PMC_SR_PCKRDY0 << ul_id)); --ul_timeout) {
		if (ul_timeout == 0) {
			return 1;
		}
	}

	PMC->PMC_PCK[ul_id] = (PMC->PMC_PCK[ul_id] & ~PMC_PCK_PRES_Msk) | ul_pres;
	for (ul_timeout = PMC_TIMEOUT; !(PMC->PMC_SR & (PMC_SR_PCKRDY0 << ul_id)); --ul_timeout) {
		if (ul_timeout == 0) {
			return 1;
		}
	}

	return 0;
}

/**
 * \brief Enable the specified programmable clock.
 *
 * \param ul_id Id of the programmable clock.
 */
void pmc_enable_pck(uint32_t ul_id)
{
	PMC->PMC_SCER = (PMC_SCER_PCK0 << ul_id) | PMC_SCER_CPKEY(0xA);
}

/**
 * \brief Disable the specified programmable clock.
 *
 * \param ul_id Id of the programmable clock.
 */
void pmc_disable_pck(uint32_t ul_id)
{
	PMC->PMC_SCDR = (PMC_SCER_PCK0 << ul_id) | PMC_SCER_CPKEY(0xA);
}

/**
 * \brief Enable all programmable clocks.
 */
void pmc_enable_all_pck(void)
{
	PMC->PMC_SCER = PMC_SCER_PCK0 | PMC_SCER_PCK1 | PMC_SCER_PCK2 | PMC_SCER_PCK3;
}

/**
 * \brief Disable all programmable clocks.
 */
void pmc_disable_all_pck(void)
{
	PMC->PMC_SCDR = PMC_SCDR_PCK0 | PMC_SCDR_PCK1 | PMC_SCDR_PCK2 | PMC_SCDR_PCK3;
}

/**
 * \brief Check if the specified programmable clock is enabled.
 *
 * \param ul_id Id of the programmable clock.
 *
 * \retval 0 Programmable clock is disabled or unknown.
 * \retval 1 Programmable clock is enabled.
 */
uint32_t pmc_is_pck_enabled(uint32_t ul_id)
{
	if (ul_id > 3) {
		return 0;
	}

	return (PMC->PMC_SCSR & (PMC_SCSR_PCK0 << ul_id));
}

/**
 * \brief Enable Coprocessor Clocks.
 */
void pmc_enable_cpck(void)
{
	PMC->PMC_SCER = PMC_SCER_CPCK | PMC_SCER_CPKEY_PASSWD;
}

/**
 * \brief Disable Coprocessor Clocks.
 */
void pmc_disable_cpck(void)
{
	PMC->PMC_SCDR = PMC_SCDR_CPCK | PMC_SCDR_CPKEY_PASSWD;
}

/**
 * \brief Check if the Coprocessor Clocks is enabled.
 *
 * \retval 0 Coprocessor Clocks is disabled.
 * \retval 1 Coprocessor Clocks is enabled.
 */
bool pmc_is_cpck_enabled(void)
{
	if (PMC->PMC_SCSR & PMC_SCSR_CPCK) {
		return 1;
	} else {
		return 0;
	}
}

/**
 * \brief Enable Coprocessor Bus Master Clocks.
 */
void pmc_enable_cpbmck(void)
{
	PMC->PMC_SCER = PMC_SCER_CPBMCK | PMC_SCER_CPKEY_PASSWD;
}

/**
 * \brief Disable Coprocessor Bus Master Clocks.
 */
void pmc_disable_cpbmck(void)
{
	PMC->PMC_SCDR = PMC_SCDR_CPBMCK | PMC_SCDR_CPKEY_PASSWD;
}

/**
 * \brief Check if the Coprocessor Bus Master Clocks is enabled.
 *
 * \retval 0 Coprocessor Bus Master Clocks is disabled.
 * \retval 1 Coprocessor Bus Master Clocks is enabled.
 */
bool pmc_is_cpbmck_enabled(void)
{
	if (PMC->PMC_SCSR & PMC_SCSR_CPBMCK) {
		return 1;
	} else {
		return 0;
	}
}

/**
 * \brief Set the prescaler for the Coprocessor Master Clock.
 *
 * \param ul_pres Prescaler value.
 */
void pmc_cpck_set_prescaler(uint32_t ul_pres)
{
	PMC->PMC_CPU_CKR = (PMC->PMC_CPU_CKR & (~PMC_CPU_CKR_CPPRES_Msk)) | ul_pres;
	while (!(PMC->PMC_SR & PMC_SR_CPMCKRDY)) {
	}
}

/**
 * \brief Set the source for the Coprocessor Master Clock.
 *
 * \param ul_source Source selection value.
 */
void pmc_cpck_set_source(uint32_t ul_source)
{
	PMC->PMC_CPU_CKR = (PMC->PMC_CPU_CKR & (~PMC_CPU_CKR_CPCSS_Msk)) | ul_source;
	while (!(PMC->PMC_SR & PMC_SR_CPMCKRDY)) {
	}
}

/**
 * \brief Switch Coprocessor master clock source selection to slow clock.
 *
 * \param ul_pres Coprocessor clock prescaler.
 *
 * \retval 0 Success.
 * \retval 1 Timeout error.
 */
uint32_t pmc_switch_cpck_to_sclk(uint32_t ul_pres)
{
	uint32_t ul_timeout;

	PMC->PMC_CPU_CKR = (PMC->PMC_CPU_CKR & (~PMC_CPU_CKR_CPCSS_Msk)) | PMC_CPU_CKR_CPCSS_SLOW_CLK;
	for (ul_timeout = PMC_TIMEOUT; !(PMC->PMC_SR & PMC_SR_CPMCKRDY); --ul_timeout) {
		if (ul_timeout == 0) {
			return 1;
		}
	}

	PMC->PMC_CPU_CKR = (PMC->PMC_CPU_CKR & (~PMC_CPU_CKR_CPPRES_Msk)) | ul_pres;
	for (ul_timeout = PMC_TIMEOUT; !(PMC->PMC_SR & PMC_SR_CPMCKRDY); --ul_timeout) {
		if (ul_timeout == 0) {
			return 1;
		}
	}

	return 0;
}

/**
 * \brief Switch Coprocessor master clock source selection to main clock.
 *
 * \param ul_pres Coprocessor clock prescaler.
 *
 * \retval 0 Success.
 * \retval 1 Timeout error.
 */
uint32_t pmc_switch_cpck_to_mainck(uint32_t ul_pres)
{
	uint32_t ul_timeout;

	PMC->PMC_CPU_CKR = (PMC->PMC_CPU_CKR & (~PMC_CPU_CKR_CPCSS_Msk)) | PMC_CPU_CKR_CPCSS_MAIN_CLK;
	for (ul_timeout = PMC_TIMEOUT; !(PMC->PMC_SR & PMC_SR_CPMCKRDY); --ul_timeout) {
		if (ul_timeout == 0) {
			return 1;
		}
	}

	PMC->PMC_CPU_CKR = (PMC->PMC_CPU_CKR & (~PMC_CPU_CKR_CPPRES_Msk)) | ul_pres;
	for (ul_timeout = PMC_TIMEOUT; !(PMC->PMC_SR & PMC_SR_CPMCKRDY); --ul_timeout) {
		if (ul_timeout == 0) {
			return 1;
		}
	}

	return 0;
}

/**
 * \brief Switch Coprocessor master clock source selection to PLLA clock.
 *
 * \param ul_pres Coprocessor clock prescaler.
 *
 * \retval 0 Success.
 * \retval 1 Timeout error.
 */
uint32_t pmc_switch_cpck_to_pllack(uint32_t ul_pres)
{
	uint32_t ul_timeout;

	PMC->PMC_CPU_CKR = (PMC->PMC_CPU_CKR & (~PMC_CPU_CKR_CPPRES_Msk)) | ul_pres;
	for (ul_timeout = PMC_TIMEOUT; !(PMC->PMC_SR & PMC_SR_CPMCKRDY); --ul_timeout) {
		if (ul_timeout == 0) {
			return 1;
		}
	}

	PMC->PMC_CPU_CKR = (PMC->PMC_CPU_CKR & (~PMC_CPU_CKR_CPCSS_Msk)) | PMC_CPU_CKR_CPCSS_PLLACK1;
	for (ul_timeout = PMC_TIMEOUT; !(PMC->PMC_SR & PMC_SR_CPMCKRDY); --ul_timeout) {
		if (ul_timeout == 0) {
			return 1;
		}
	}

	return 0;
}

/**
 * \brief Switch Coprocessor master clock source selection to PLLB clock.
 *
 * \param ul_pres Coprocessor clock prescaler.
 *
 * \retval 0 Success.
 * \retval 1 Timeout error.
 */
uint32_t pmc_switch_cpck_to_pllbck(uint32_t ul_pres)
{
	uint32_t ul_timeout;

	PMC->PMC_CPU_CKR = (PMC->PMC_CPU_CKR & (~PMC_CPU_CKR_CPPRES_Msk)) | ul_pres;
	for (ul_timeout = PMC_TIMEOUT; !(PMC->PMC_SR & PMC_SR_CPMCKRDY); --ul_timeout) {
		if (ul_timeout == 0) {
			return 1;
		}
	}

	PMC->PMC_CPU_CKR = (PMC->PMC_CPU_CKR & (~PMC_CPU_CKR_CPCSS_Msk)) | PMC_CPU_CKR_CPCSS_PLLBCK;
	for (ul_timeout = PMC_TIMEOUT; !(PMC->PMC_SR & PMC_SR_CPMCKRDY); --ul_timeout) {
		if (ul_timeout == 0) {
			return 1;
		}
	}

	return 0;
}

/**
 * \brief Switch Coprocessor master clock source selection to PLLC clock.
 *
 * \param ul_pres Coprocessor clock prescaler.
 *
 * \retval 0 Success.
 * \retval 1 Timeout error.
 */
uint32_t pmc_switch_cpck_to_pllcck(uint32_t ul_pres)
{
	uint32_t ul_timeout;

	PMC->PMC_CPU_CKR = (PMC->PMC_CPU_CKR & (~PMC_CPU_CKR_CPPRES_Msk)) | ul_pres;
	for (ul_timeout = PMC_TIMEOUT; !(PMC->PMC_SR & PMC_SR_CPMCKRDY); --ul_timeout) {
		if (ul_timeout == 0) {
			return 1;
		}
	}

	PMC->PMC_CPU_CKR = (PMC->PMC_CPU_CKR & (~PMC_CPU_CKR_CPCSS_Msk)) | PMC_CPU_CKR_CPCSS_PLLCCK;
	for (ul_timeout = PMC_TIMEOUT; !(PMC->PMC_SR & PMC_SR_CPMCKRDY); --ul_timeout) {
		if (ul_timeout == 0) {
			return 1;
		}
	}

	return 0;
}

/**
 * \brief Check if the MCK1DIV is enabled.
 *
 * \retval 1 MCK1DIV is enabled.
 * \retval 0 MCK1DIV is disabled.
 */
uint32_t pmc_is_mck1div_enabled(void)
{
	return (((PMC->PMC_CPU_CKR & PMC_CPU_CKR_RATIO_MCK1DIV) ? 1 : 0));
}

/**
 * \brief Enable MCK1DIV
 */
void pmc_enable_mck1div(void)
{
	PMC->PMC_CPU_CKR |= PMC_CPU_CKR_RATIO_MCK1DIV;
}

/**
 * \brief Disable MCK1DIV
 */
void pmc_disable_mck1div(void)
{
	PMC->PMC_CPU_CKR &= ~PMC_CPU_CKR_RATIO_MCK1DIV;
}

/**
 * \brief Enable PMC interrupts.
 *
 * \param ul_sources Interrupt sources bit map.
 */
void pmc_enable_interrupt(uint32_t ul_sources)
{
	PMC->PMC_IER = ul_sources;
}

/**
 * \brief Disable PMC interrupts.
 *
 * \param ul_sources Interrupt sources bit map.
 */
void pmc_disable_interrupt(uint32_t ul_sources)
{
	PMC->PMC_IDR = ul_sources;
}

/**
 * \brief Get PMC interrupt mask.
 *
 * \return The interrupt mask value.
 */
uint32_t pmc_get_interrupt_mask(void)
{
	return PMC->PMC_IMR;
}

/**
 * \brief Get current status.
 *
 * \return The current PMC status.
 */
uint32_t pmc_get_status(void)
{
	return PMC->PMC_SR;
}

/**
 * \brief Set the wake-up inputs for fast startup mode registers
 *        (event generation).
 *
 * \param ul_inputs Wake up inputs to enable.
 */
void pmc_set_fast_startup_input(uint32_t ul_inputs)
{
	PMC->PMC_FSMR |= ul_inputs;
}

/**
 * \brief Clear the wake-up inputs for fast startup mode registers
 *        (remove event generation).
 *
 * \param ul_inputs Wake up inputs to disable.
 */
void pmc_clr_fast_startup_input(uint32_t ul_inputs)
{
	PMC->PMC_FSMR &= ~(ul_inputs);
}

/**
 * \brief Set the wake-up inputs for fast startup mode registers
 *        (event generation).
 *
 * \param ul_inputs Wake up inputs to enable.
 * \param b_polarity Defines the active polarity of the selected wake-up input.
 */
void pmc_set_wakeup_input(uint32_t ul_inputs, bool b_polarity)
{
	uint32_t ul_wcr;

	ul_wcr = PMC->PMC_WCR;
	ul_wcr &= ~PMC_WCR_WKPIONB_Msk;
	if (b_polarity) {
		ul_wcr |= PMC_WCR_WIP;
	} else {
		ul_wcr &= ~PMC_WCR_WIP;
	}
	PMC->PMC_WCR = ul_wcr | PMC_WCR_CMD | PMC_WCR_EN | ul_inputs;
}

/**
 * \brief Clear the wake-up inputs for fast startup mode registers
 *        (remove event generation).
 *
 * \param ul_inputs Wake up inputs to disable.
 */
void pmc_clr_wakeup_input(uint32_t ul_inputs)
{
	uint32_t ul_wcr;

	ul_wcr = PMC->PMC_WCR;
	ul_wcr &= ~(PMC_WCR_WKPIONB_Msk | PMC_WCR_EN);
	PMC->PMC_WCR = ul_wcr | PMC_WCR_CMD | ul_inputs;
}

/**
 * \brief Set the wake-up inputs of coprocessor for fast startup mode registers
 *        (event generation).
 *
 * \param ul_inputs Wake up inputs to enable.
 */
void pmc_cp_set_wakeup_input(uint32_t ul_inputs, bool b_polarity)
{
	uint32_t ul_wcr;

	ul_wcr = PMC->PMC_WCR;
	ul_wcr &= ~PMC_WCR_WKPIONB_Msk;
	if (b_polarity) {
		ul_wcr |= PMC_WCR_WIP;
	} else {
		ul_wcr &= ~PMC_WCR_WIP;
	}
	PMC->PMC_WCR = ul_wcr | PMC_WCR_CMD | PMC_WCR_WIEN1 | ul_inputs;
}

/**
 * \brief Clear the wake-up inputs of coprocessor for fast startup mode registers
 *        (remove event generation).
 *
 * \param ul_inputs Wake up inputs to disable.
 */
void pmc_cp_clr_wakeup_input(uint32_t ul_inputs)
{
	uint32_t ul_wcr;

	ul_wcr = PMC->PMC_WCR;
	ul_wcr &= ~(PMC_WCR_WKPIONB_Msk | PMC_WCR_WIEN1);
	PMC->PMC_WCR = ul_wcr | PMC_WCR_CMD | ul_inputs;
}

/**
 * \brief Set the wake-up inputs of coprocessor for fast startup mode registers
 *        (event generation).
 *
 * \param ul_inputs Wake up inputs to enable.
 */
void pmc_cp_set_fast_startup_input(uint32_t ul_inputs)
{
	PMC->PMC_CPFSMR |= ul_inputs;
}

/**
 * \brief Clear the wake-up inputs of coprocessor for fast startup mode registers
 *        (remove event generation).
 *
 * \param ul_inputs Wake up inputs to disable.
 */
void pmc_cp_clr_fast_startup_input(uint32_t ul_inputs)
{
	PMC->PMC_CPFSMR &= ~(ul_inputs);
}

/**
 * \brief Enable Sleep Mode.
 * Enter condition: (WFE or WFI) + (SLEEPDEEP bit = 0) + (LPM bit = 0)
 *
 * \param uc_type 0 for wait for interrupt, 1 for wait for event.
 * \note For PIC32CX series, since only WFI is effective, uc_type = 1 will be treated as uc_type = 0.
 */
void pmc_enable_sleepmode(uint8_t uc_type)
{
	PMC->PMC_FSMR &= (uint32_t) ~PMC_FSMR_LPM;

	// UNUSED(uc_type);
	__DSB();
	__WFI();
}

static uint32_t ul_flash_in_wait_mode = PMC_WAIT_MODE_FLASH_DEEP_POWERDOWN;

/**
 * \brief Set the embedded flash state in wait mode
 *
 * \param ul_flash_state PMC_WAIT_MODE_FLASH_STANDBY flash in standby mode,
 * PMC_WAIT_MODE_FLASH_DEEP_POWERDOWN flash in deep power down mode.
 */
void pmc_set_flash_in_wait_mode(uint32_t ul_flash_state)
{
	ul_flash_in_wait_mode = ul_flash_state;
}

/**
 * \brief Force the embedded flash state in wait mode. It will be immediately applied.
 *
 * \param ul_flash_state PMC_WAIT_MODE_FLASH_STANDBY flash in standby mode,
 * PMC_WAIT_MODE_FLASH_DEEP_POWERDOWN flash in deep power down mode.
 */
void pmc_force_flash_in_wait_mode(uint32_t ul_flash_state)
{
	uint32_t ul_fsmr;

	/* Flash in wait mode */
	ul_fsmr = PMC->PMC_FSMR;
	ul_fsmr &= ~PMC_FSMR_FLPM_Msk;
	ul_fsmr |= ul_flash_state;
	PMC->PMC_FSMR = ul_fsmr;

	PMC->PMC_FSMR |= PMC_FSMR_FFLPM;
}

/**
 * \brief Enable Wait Mode. Enter condition: (WAITMODE bit = 1) + FLPM
 *
 * \note In this function, FLPM will retain, WAITMODE bit will be set,
 * Generally, this function will be called by pmc_sleep() in order to
 * complete all sequence entering wait mode.
 * See \ref pmc_sleep() for entering different sleep modes.
 */
void pmc_enable_waitmode(void)
{
	/* The device must not enter Wait Mode if the asynchronous
	 * partial wake-up is enabled for one of the following PIDs: FLEXCOM and ADC
	 */
	if (PMC->PMC_SLPWK_AIPR & PMC_SLPWK_AIPR_AIP) {
		return;
	}

	/* Flash in wait mode */
	pmc_force_flash_in_wait_mode(ul_flash_in_wait_mode);

	/* Write a 0 to the SLEEPDEEP bit of the Cortex-M4 processor */
	SCB->SCR &= ~(SCB_SCR_SLEEPDEEP_Msk);

	/* Execute the Wait-For-Event (WFE) instruction of the processor */
	__DSB();
//	__WFE();
	__WFI();

	/* Restore Flash in idle mode */
	pmc_force_flash_in_wait_mode(PMC_FSMR_FLPM_FLASH_IDLE);
}

/**
 * \brief Enable Backup Mode.
 */
void pmc_enable_backupmode(void)
{
	//cpu_irq_enable();

	if (SUPC->SUPC_MR & SUPC_MR_VREGDIS) {
		if ((SUPC->SUPC_MR & SUPC_MR_LCDMODE_LCDON_EXTVR) == SUPC_MR_LCDMODE_LCDON_EXTVR) {
			/* If the SLCD is used in Backup mode to keep a static message, write a 1 to SUPC_CR.SHDWEOF */
			SUPC->SUPC_CR = SUPC_CR_KEY_PASSWD | SUPC_CR_SHDWEOF;
		} else {
			/* If an external voltage regulator is used for VDDCORE/VDDPLL, write a 1 to SUPC_CR.SHDW */
			SUPC->SUPC_CR = SUPC_CR_KEY_PASSWD | SUPC_CR_SHDW;
		}
	} else {
		/* If the internal core voltage regulator is used, write a 1 to SUPC_CR.VROFF */
		SUPC->SUPC_CR = SUPC_CR_KEY_PASSWD | SUPC_CR_VROFF_STOP_VREG;
	}
}

/**
 * \brief Enable Clock Failure Detector.
 */
void pmc_enable_clock_failure_detector(void)
{
	uint32_t ul_reg = PMC->CKGR_MOR;

	PMC->CKGR_MOR = CKGR_MOR_KEY_PASSWD | CKGR_MOR_CFDEN | ul_reg;
}

/**
 * \brief Disable Clock Failure Detector.
 */
void pmc_disable_clock_failure_detector(void)
{
	uint32_t ul_reg = PMC->CKGR_MOR & (~CKGR_MOR_CFDEN);

	PMC->CKGR_MOR = CKGR_MOR_KEY_PASSWD | ul_reg;
}

/**
 * \brief Clear Clock Failure Detector.
 */
void pmc_clear_clock_failure_detector(void)
{
	PMC->PMC_FOCR = PMC_FOCR_FOCLR;
}

/**
 * \brief Enable Slow Crystal Oscillator Frequency Monitoring.
 */
void pmc_enable_sclk_osc_freq_monitor(void)
{
	uint32_t ul_reg = PMC->CKGR_MOR;

	PMC->CKGR_MOR = CKGR_MOR_KEY_PASSWD | CKGR_MOR_XT32KFME | ul_reg;
}

/**
 * \brief Disable Slow Crystal Oscillator Frequency Monitoring.
 */
void pmc_disable_sclk_osc_freq_monitor(void)
{
	uint32_t ul_reg = PMC->CKGR_MOR & (~CKGR_MOR_XT32KFME);

	PMC->CKGR_MOR = CKGR_MOR_KEY_PASSWD | ul_reg;
}

/**
 * \brief Enable or disable write protect of PMC registers.
 *
 * \param ul_enable 1 to enable, 0 to disable.
 */
void pmc_set_writeprotect(uint32_t ul_enable)
{
	if (ul_enable) {
		PMC->PMC_WPMR = PMC_WPMR_WPKEY_PASSWD | PMC_WPMR_WPEN;
	} else {
		PMC->PMC_WPMR = PMC_WPMR_WPKEY_PASSWD;
	}
}

/**
 * \brief Return write protect status.
 *
 * \return Return write protect status.
 */
uint32_t pmc_get_writeprotect_status(void)
{
	return PMC->PMC_WPSR;
}

/**
 * \brief Enable the specified peripheral clock.
 *
 * \note The ID must NOT be shifted (i.e., 1 << ID_xxx).
 *
 * \param ul_id Peripheral ID (ID_xxx).
 *
 * \retval 0 Success.
 * \retval 1 Fail.
 */
uint32_t pmc_enable_sleepwalking(uint32_t ul_id)
{
	if (((ul_id >= ID_FLEXCOM0) && (ul_id <= ID_FLEXCOM7)) || (ul_id == ID_ADC)) {
		/* Check Activity in progress */
		if (pmc_get_active_status(ul_id)) {
			return 1;
		}

		/* Enable SleepWalking */
		PMC->PMC_SLPWKCR |= PMC_SLPWKCR_SLPWKSR;

		/* Check Activity in progress */
		if (pmc_get_active_status(ul_id)) {
			pmc_disable_sleepwalking(ul_id);
			return 1;
		}

		return 0;
	}

	return 1;
}

/**
 * \brief Disable the sleepwalking of specified peripheral.
 *
 * \note The ID must NOT be shifted (i.e., 1 << ID_xxx).
 *
 * \param ul_id Peripheral ID (ID_xxx).
 *
 * \retval 0 Success.
 * \retval 1 Invalid parameter.
 */
uint32_t pmc_disable_sleepwalking(uint32_t ul_id)
{
	if (((ul_id >= ID_FLEXCOM0) && (ul_id <= ID_FLEXCOM7)) || (ul_id == ID_ADC)) {
		uint32_t ul_splwkcr;

		ul_splwkcr = PMC->PMC_SLPWKCR;

		/* Disable SleepWalking */
		ul_splwkcr &= ~(PMC_SLPWKCR_PID_Msk | PMC_SLPWKCR_SLPWKSR);
		ul_splwkcr |= PMC_SLPWKCR_PID(ul_id) | PMC_SLPWKCR_CMD;
		PMC->PMC_SLPWKCR = ul_splwkcr;

		return 0;
	}

	return 1;
}

/**
 * \brief Return peripheral sleepwalking enable status.
 *
 * \return the status register value.
 */
uint32_t pmc_get_sleepwalking_status(uint32_t ul_id)
{
	uint32_t ul_splwkcr;

	ul_splwkcr = PMC->PMC_SLPWKCR;

	ul_splwkcr &= ~(PMC_SLPWKCR_PID_Msk | PMC_SLPWKCR_CMD);
	ul_splwkcr |= PMC_SLPWKCR_PID(ul_id);
	PMC->PMC_SLPWKCR = ul_splwkcr;

	return PMC->PMC_SLPWKCR & PMC_SLPWKCR_SLPWKSR;
}

/**
 * \brief Return peripheral active status.
 *
 * \return the status register value.
 */
uint32_t pmc_get_active_status(uint32_t ul_id)
{
	uint32_t ul_splwkcr;

	ul_splwkcr = PMC->PMC_SLPWKCR;

	ul_splwkcr &= ~(PMC_SLPWKCR_PID_Msk | PMC_SLPWKCR_CMD);
	ul_splwkcr |= PMC_SLPWKCR_PID(ul_id);
	PMC->PMC_SLPWKCR = ul_splwkcr;

	return PMC->PMC_SLPWKCR & PMC_SLPWKCR_ASR;
}

/**
 * \brief Enables MCK0 monitor.
 *
 * \param ul_reset_en  if 1, an error on the MCK0 frequency can lead a reset. Otherwise, leads an interrupt.
 */
void pmc_enable_mck_monitor(uint32_t ul_reset_en)
{
	if (ul_reset_en) {
		PMC->CKGR_MOR |= CKGR_MOR_BMCKRST;
	} else {
		PMC->PMC_IER |= PMC_IER_MCKMON;
	}
}

/**
 * \brief Disables MCK0 monitor.
 */
void pmc_disable_mck_monitor(void)
{
	if (PMC->PMC_IER & PMC_IER_MCKMON) {
		PMC->PMC_IER &= ~PMC_IER_MCKMON;
	}

	if (PMC->CKGR_MOR & CKGR_MOR_BMCKRST) {
		PMC->CKGR_MOR &= ~CKGR_MOR_BMCKRST;
	}
}

/**
 * \brief Set MCK0 monitor limit to generate a reset
 *
 * \param ul_high_limit  Beyond this limit, the MCK0 frequency monitor generates a reset
 * \param ul_low_limit   Below this limit, the MCK0 frequency monitor generates a reset
 */
void pmc_set_mck_monitor_limit_reset(uint32_t ul_high_limit, uint32_t ul_low_limit)
{
	uint32_t ul_reg;

	ul_reg = PMC->PMC_MCKLIM;
	ul_reg &= ~(PMC_MCKLIM_MCK_LOW_RES_Msk | PMC_MCKLIM_MCK_HIGH_RES_Msk);
	ul_reg |= PMC_MCKLIM_MCK_HIGH_RES(ul_high_limit);
	ul_reg |= PMC_MCKLIM_MCK_LOW_RES(ul_low_limit);
	PMC->PMC_MCKLIM = ul_reg;
}

/**
 * \brief Set MCK0 monitor limit to generate an interrupt.
 *
 * \param ul_high_limit  Beyond this limit, the MCK0 frequency monitor generates an interrupt
 * \param ul_low_limit   Below this limit, the MCK0 frequency monitor generates an interrupt
 */
void pmc_set_mck_monitor_limit_interrupt(uint32_t ul_high_limit, uint32_t ul_low_limit)
{
	uint32_t ul_reg;

	ul_reg = PMC->PMC_MCKLIM;
	ul_reg &= ~(PMC_MCKLIM_MCK_LOW_IT_Msk | PMC_MCKLIM_MCK_HIGH_IT_Msk);
	ul_reg |= PMC_MCKLIM_MCK_HIGH_IT(ul_high_limit);
	ul_reg |= PMC_MCKLIM_MCK_LOW_IT(ul_low_limit);
	PMC->PMC_MCKLIM = ul_reg;
}


/* / @cond 0 */
/**INDENT-OFF**/
#ifdef __cplusplus
}
#endif
/**INDENT-ON**/
/* / @endcond */
