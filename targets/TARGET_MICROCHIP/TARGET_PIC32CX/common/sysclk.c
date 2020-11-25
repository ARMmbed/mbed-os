/**
 * \file
 *
 * \brief Chip-specific system clock management functions.
 *
 * Copyright (c) 2013-2018 Microchip Technology Inc. and its subsidiaries.
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
#include <sysclk.h>

#include "compiler.h" 
#include "clocking.h"

/* / @cond 0 */
/**INDENT-OFF**/
#ifdef __cplusplus
extern "C" {
#endif
/**INDENT-ON**/
/* / @endcond */

/**
 * \weakgroup sysclk_group
 * @{
 */

#if defined(CONFIG_SYSCLK_DEFAULT_RETURNS_SLOW_OSC)

/**
 * \brief boolean signalling that the sysclk_init is done.
 */
uint32_t sysclk_initialized = 0;
#endif

/**
 * \brief Set system clock prescaler configuration
 *
 * This function will change the system clock prescaler configuration to
 * match the parameters.
 *
 * \note The parameters to this function are device-specific.
 *
 * \param ul_pres The CPU clock will be divided by \f$2^{mck\_pres}\f$
 */
void sysclk_set_prescalers(uint32_t ul_pres)
{
	pmc_mck_set_prescaler(ul_pres);
	SystemCoreClockUpdate();
}

/**
 * \brief Change the source of the main system clock.
 *
 * \param ul_src The new system clock source. Must be one of the constants
 * from the <em>System Clock Sources</em> section.
 */
void sysclk_set_source(uint32_t ul_src)
{
	switch (ul_src) {
	case SYSCLK_SRC_SLCK_RC:
	case SYSCLK_SRC_SLCK_XTAL:
	case SYSCLK_SRC_SLCK_BYPASS:
		pmc_mck_set_source(PMC_CPU_CKR_CSS_SLOW_CLK);
		break;

	case SYSCLK_SRC_MAINCK_12M_RC:
	case SYSCLK_SRC_MAINCK_XTAL:
	case SYSCLK_SRC_MAINCK_BYPASS:
		pmc_mck_set_source(PMC_CPU_CKR_CSS_MAIN_CLK);
		break;

	case SYSCLK_SRC_PLLACK1:
		pmc_mck_set_source(PMC_CPU_CKR_CSS_PLLACK1);
		break;

	case SYSCLK_SRC_PLLBCK0:
		pmc_mck_set_source(PMC_CPU_CKR_CSS_PLLBCK);
		break;
	}

	SystemCoreClockUpdate();
}

#ifdef CONFIG_CPCLK_ENABLE

/**
 * \brief Configure clock for coprocessor.
 *
 * \note It's assumed that PLLs have been configured before.
 */
static void sysclk_configure_cpclk(void)
{
#if (CONFIG_CPCLK_PRES > CPCLK_PRES_MAX)
#error Invalid CONFIG_CPCLK_PRES setting.
#endif
	uint32_t read_reg;

	/* Assert coprocessor reset and reset its peripheral */
	read_reg = RSTC->RSTC_MR;
	read_reg &= ~(RSTC_MR_KEY_Msk | RSTC_MR_CPEREN | RSTC_MR_CPROCEN);
	read_reg |= RSTC_MR_KEY_PASSWD;
	RSTC->RSTC_MR = read_reg;

#ifdef CONFIG_PLL0_SOURCE
	if (CONFIG_CPCLK_SOURCE == CPCLK_SRC_PLLACK1) {
		uint32_t pll_clk;

		pll_clk = SystemGetPllClk(0, 1);
		if (pll_clk > 120000000) {
			/* Enable MCK1DIV */
			PMC->PMC_CPU_CKR |= PMC_CPU_CKR_RATIO_MCK1DIV;
		} else {
			/* Disable MCK1DIV */
			PMC->PMC_CPU_CKR &= ~(PMC_CPU_CKR_RATIO_MCK1DIV);
		}
		pmc_switch_cpck_to_pllack(CONFIG_SYSCLK_PRES);
	}
#endif

#ifdef CONFIG_PLL1_SOURCE
	if (CONFIG_CPCLK_SOURCE == CPCLK_SRC_PLLBCK0) {
		uint32_t pll_clk;

		pll_clk = SystemGetPllClk(1, 0);
		if (pll_clk > 120000000) {
			/* Enable MCK1DIV */
			PMC->PMC_CPU_CKR |= PMC_CPU_CKR_RATIO_MCK1DIV;
		} else {
			/* Disable MCK1DIV */
			PMC->PMC_CPU_CKR &= ~(PMC_CPU_CKR_RATIO_MCK1DIV);
		}
		pmc_switch_cpck_to_pllbck(CONFIG_SYSCLK_PRES);
	}
#endif

#ifdef CONFIG_PLL2_SOURCE
	if (CONFIG_CPCLK_SOURCE == CPCLK_SRC_PLLCCK0) {
		uint32_t pll_clk;

		pll_clk = SystemGetPllClk(2, 0);
		if (pll_clk > 120000000) {
			/* Enable MCK1DIV */
			PMC->PMC_CPU_CKR |= PMC_CPU_CKR_RATIO_MCK1DIV;
		} else {
			/* Disable MCK1DIV */
			PMC->PMC_CPU_CKR &= ~(PMC_CPU_CKR_RATIO_MCK1DIV);
		}
		pmc_switch_cpck_to_pllcck(CONFIG_SYSCLK_PRES);
	}
#endif

	/* Enables Coprocessor Bus Master Clock */
	PMC->PMC_SCER = PMC_SCER_CPBMCK | PMC_SCER_CPKEY_PASSWD;

	/* Enables the Coprocessor Clocks */
	PMC->PMC_SCER = PMC_SCER_CPCK | PMC_SCER_CPKEY_PASSWD;

	/* Set coprocessor clock prescaler */
	read_reg = PMC->PMC_CPU_CKR;
	read_reg &= ~PMC_CPU_CKR_CPPRES_Msk;
	PMC->PMC_CPU_CKR = read_reg | PMC_CPU_CKR_CPPRES(CONFIG_CPCLK_PRES);

	/* Set coprocessor clock source */
	read_reg = PMC->PMC_CPU_CKR;
	read_reg &= ~PMC_CPU_CKR_CPCSS_Msk;
	PMC->PMC_CPU_CKR = read_reg | PMC_CPU_CKR_CPCSS(CONFIG_CPCLK_SOURCE);

	/* Release coprocessor peripheral reset */
	RSTC->RSTC_MR |= (RSTC_MR_KEY_PASSWD | RSTC_MR_CPEREN);

	/* Enable Core 1 SRAM1 and SRAM2 memories */
	pmc_enable_periph_clk(ID_FLEXRAM1);
	
	/* Due to PMC_SR_CPMCKRDY is always 1. Waiting loops are not effective */
	read_reg = 0x3FFFF;
	while(read_reg--);
}

#endif

void sysclk_init(void)
{
#if defined(CONFIG_PLL0_SOURCE) || defined(CONFIG_PLL1_SOURCE) || defined(CONFIG_PLL2_SOURCE)
	struct _pmc_pll_cfg pllcfg = {0};
#endif
	/* Disable Pull Down from PA29(XIN) and PB0(XOUT) */
	sysclk_enable_peripheral_clock(ID_PIOA);
	sysclk_enable_peripheral_clock(ID_PIOB);
	PIOA->PIO_MSKR = 1 << 29;
	PIOA->PIO_CFGR &= ~(PIO_CFGR_PDEN);
	PIOB->PIO_MSKR = 1 << 0;
	PIOB->PIO_CFGR &= ~(PIO_CFGR_PDEN);

	/* Set flash wait state to max in case the below clock switching. */
	system_init_flash(CHIP_FREQ_CPU_MAX);
	
	pmc_switch_mck_to_mainck(CONFIG_SYSCLK_PRES);
	
#ifdef CONFIG_PLL0_SOURCE
	pll_enable_source(CONFIG_PLL0_SOURCE);
	pmc_disable_pll(PLLA_ID);
	pllcfg.pll_id = PLLA_ID;
	pllcfg.mul = CONFIG_PLL0_MUL;
	pllcfg.fracr = CONFIG_PLL0_FRACR;
	pllcfg.div1 = CONFIG_PLL0_DIV1;
	pllcfg.div0 = CONFIG_PLL0_DIV0;
	pllcfg.stuptim = 0;
	pllcfg.pll_src = CONFIG_PLL0_SOURCE;
	pllcfg.loop_filter = 0x35;
	pllcfg.control = 0x38;
	pmc_enable_pll(&pllcfg);
#endif
	
#ifdef CONFIG_PLL1_SOURCE
	pll_enable_source(CONFIG_PLL1_SOURCE);
	pmc_disable_pll(PLLB_ID);
	pllcfg.pll_id = PLLB_ID;
	pllcfg.mul = CONFIG_PLL1_MUL;
	pllcfg.fracr = CONFIG_PLL1_FRACR;
	pllcfg.div1 = 0;
	pllcfg.div0 = CONFIG_PLL1_DIV0;
	pllcfg.stuptim = 0;
	pllcfg.pll_src = CONFIG_PLL1_SOURCE;
	pllcfg.loop_filter = 0x28;
	pllcfg.control = 0x38;
	pmc_enable_pll(&pllcfg);
#endif	
	
#ifdef CONFIG_PLL2_SOURCE
	pll_enable_source(CONFIG_PLL2_SOURCE);
	pmc_disable_pll(PLLC_ID);
	pllcfg.pll_id = PLLC_ID;
	pllcfg.mul = CONFIG_PLL2_MUL;
	pllcfg.fracr = CONFIG_PLL2_FRACR;
	pllcfg.div1 = 0;
	pllcfg.div0 = CONFIG_PLL2_DIV0;
	pllcfg.stuptim = 0;
	pllcfg.pll_src = CONFIG_PLL2_SOURCE;
	pllcfg.loop_filter = 0x28;
	pllcfg.control = 0x38;
	pmc_enable_pll(&pllcfg);
#endif	

	/* Config system clock setting */
	if (CONFIG_SYSCLK_SOURCE == SYSCLK_SRC_SLCK_RC) {
		osc_enable(OSC_SLCK_32K_RC);
		osc_wait_ready(OSC_SLCK_32K_RC);
		pmc_switch_mck_to_sclk(CONFIG_SYSCLK_PRES);
	} else if (CONFIG_SYSCLK_SOURCE == SYSCLK_SRC_SLCK_XTAL) {
		osc_enable(OSC_SLCK_32K_XTAL);
		osc_wait_ready(OSC_SLCK_32K_XTAL);
		pmc_switch_mck_to_sclk(CONFIG_SYSCLK_PRES);
	} else if (CONFIG_SYSCLK_SOURCE == SYSCLK_SRC_SLCK_BYPASS) {
		osc_enable(OSC_SLCK_32K_BYPASS);
		osc_wait_ready(OSC_SLCK_32K_BYPASS);
		pmc_switch_mck_to_sclk(CONFIG_SYSCLK_PRES);
	} else if (CONFIG_SYSCLK_SOURCE == SYSCLK_SRC_MAINCK_12M_RC) {
		osc_enable(OSC_MAINCK_12M_RC);
		osc_wait_ready(OSC_MAINCK_12M_RC);
		pmc_switch_mck_to_mainck(CONFIG_SYSCLK_PRES);
	} else if (CONFIG_SYSCLK_SOURCE == SYSCLK_SRC_MAINCK_XTAL) {
		osc_enable(OSC_MAINCK_XTAL);
		osc_wait_ready(OSC_MAINCK_XTAL);
		pmc_switch_mck_to_mainck(CONFIG_SYSCLK_PRES);
	} else if (CONFIG_SYSCLK_SOURCE == SYSCLK_SRC_MAINCK_BYPASS) {
		osc_enable(OSC_MAINCK_BYPASS);
		osc_wait_ready(OSC_MAINCK_BYPASS);
		pmc_switch_mck_to_mainck(CONFIG_SYSCLK_PRES);
	}

#ifdef CONFIG_PLL0_SOURCE
	else if (CONFIG_SYSCLK_SOURCE == SYSCLK_SRC_PLLACK1) {
		uint32_t pll_clk;

		pll_clk = SystemGetPllClk(0, 1);
		if (pll_clk > 100000000) {
			/* Enable MCK0DIV */
			PMC->PMC_CPU_CKR |= PMC_CPU_CKR_RATIO_MCK0DIV;
		} else {
			/* Disable MCK0DIV */
			PMC->PMC_CPU_CKR &= ~(PMC_CPU_CKR_RATIO_MCK0DIV);
		}

		pmc_switch_mck_to_pllack(CONFIG_SYSCLK_PRES);
	}
#endif
#ifdef CONFIG_PLL1_SOURCE
	else if (CONFIG_SYSCLK_SOURCE == SYSCLK_SRC_PLLBCK0) {
		uint32_t pll_clk;

		pll_clk = SystemGetPllClk(1, 0);
		if (pll_clk > 100000000) {
			/* Enable MCK0DIV */
			PMC->PMC_CPU_CKR |= PMC_CPU_CKR_RATIO_MCK0DIV;
		} else {
			/* Disable MCK0DIV */
			PMC->PMC_CPU_CKR &= ~(PMC_CPU_CKR_RATIO_MCK0DIV);
		}

		pmc_switch_mck_to_pllbck(CONFIG_SYSCLK_PRES);
	}
#endif

	/* Update the System 0 Frequency variable */
	SystemCoreClockUpdate();

	/* Set a flash wait state depending on the new cpu frequency */
	system_init_flash(sysclk_get_cpu_hz());

#if (defined CONFIG_SYSCLK_DEFAULT_RETURNS_SLOW_OSC)
	/* Signal that the internal frequencies are setup */
	sysclk_initialized = 1;
#endif

#ifdef CONFIG_CPCLK_ENABLE
	/* Enable coprocessor clock */
	sysclk_configure_cpclk();

	/* Update the System 1 Frequency variable */
	SystemCore1ClockUpdate();
#endif
}

/**
 * \brief Return the current rate in Hz of the main system clock (Core 0)
 */
uint32_t sysclk_get_main_hz_core0(void)
{
	uint32_t ul_hz_core0;
	uint32_t ul_main_ck;
	uint32_t ul_td_slck;
	uint8_t uc_presc;
	uint8_t uc_core_css;
	struct _pmc_pll_cfg pll_cfg;
	
	uc_presc = (PMC->PMC_CPU_CKR & PMC_CPU_CKR_PRES_Msk) >> PMC_CPU_CKR_PRES_Pos;
	uc_core_css = (PMC->PMC_CPU_CKR & PMC_CPU_CKR_CSS_Msk) >> PMC_CPU_CKR_CSS_Pos;
	
	/* Get Main CK */
	if (PMC->CKGR_MOR & CKGR_MOR_MOSCSEL) {
		if (PMC->CKGR_MOR & CKGR_MOR_MOSCXTBY) {
			/* 12 to 48 MHz Crystal Oscillator Bypass */
			ul_main_ck = OSC_MAINCK_BYPASS_HZ;
		} else {
			/* The Main crystal oscillator is selected */
			ul_main_ck = OSC_MAINCK_XTAL_HZ;
		}
	} else {
		/* The Main RC oscillator is selected */
		ul_main_ck = OSC_MAINCK_12M_RC_HZ;
	}
	
	/* Get TD slck */
	if (SUPC->SUPC_SR & SUPC_SR_TDOSCSEL_XTAL) {
		if (SUPC->SUPC_MR & SUPC_MR_OSCBYPASS_BYPASS) {
			ul_td_slck = OSC_SLCK_32K_BYPASS_HZ;
		} else {
			ul_td_slck = OSC_SLCK_32K_XTAL_HZ;
		}
	} else {
		ul_td_slck = OSC_SLCK_32K_RC_HZ;
	}
	
	switch (uc_core_css) {
	case PMC_CPU_CKR_CSS_SLOW_CLK:
		ul_hz_core0 = OSC_SLCK_32K_RC_HZ;
		break;
		
	case PMC_CPU_CKR_CSS_MAIN_CLK:
		ul_hz_core0 = ul_main_ck;
		break;
		
	case PMC_CPU_CKR_CSS_PLLACK1:
	case PMC_CPU_CKR_CSS_PLLBCK:
		{
			uint64_t pll_tmp1;
			uint64_t pll_tmp0;
			
			pmc_get_pll_config(PLLA_ID, &pll_cfg);
			
			if (pll_cfg.pll_src) {
				/* MAINCK */
				pll_tmp1 = ul_main_ck;
			} else {
				/* TD_SLCK */
				pll_tmp1 = ul_td_slck;
			}
			/* PLLACK1 output: (Fref * (MUL + 1 + (FRACR / 2^22))) / (DIV1 + 1) */
			pll_tmp1 *= (uint64_t)(pll_cfg.fracr + ((pll_cfg.mul + 1) * (uint64_t)(1 << 22)));
			pll_tmp0 = pll_tmp1 / (uint64_t)((pll_cfg.div0 + 1) * (uint64_t)(1 << 22));
			pll_tmp1 /= (uint64_t)((pll_cfg.div1 + 1) * (uint64_t)(1 << 22));
			ul_hz_core0 = (uint32_t)pll_tmp1;
			
			if (uc_core_css == PMC_CPU_CKR_CSS_PLLBCK) {
				pmc_get_pll_config(PLLB_ID, &pll_cfg);
		
				if (pll_cfg.pll_src) {
					/* MAINCK */
					pll_tmp0 = ul_main_ck;
				} /* else PLLACK0 */
				
				/* PLLACK1 output: (Fref * (MUL + 1 + (FRACR / 2^22))) / (DIV1 + 1) */
				pll_tmp0 *= (uint64_t)(pll_cfg.fracr + ((pll_cfg.mul + 1) * (uint64_t)(1 << 22)));
				pll_tmp0 /= (uint64_t)((pll_cfg.div0 + 1) * (uint64_t)(1 << 22));
				
				ul_hz_core0 = (uint32_t)pll_tmp0;
			}
		}
		break;
	}
	
	/* Apply prescaler */
	ul_hz_core0 /= (1 << uc_presc);
	
	return ul_hz_core0;
}

#ifdef CONFIG_CPCLK_ENABLE
/**
 * \brief Return the current rate in Hz of the main system clock (Core 1)
 */
uint32_t sysclk_get_main_hz_core1(void)
{
	uint32_t ul_hz_core1;
	uint32_t ul_main_ck;
	uint32_t ul_td_slck;
	uint8_t uc_presc;
	uint32_t ul_core_css;
	struct _pmc_pll_cfg pll_cfg;
	
	uc_presc = (PMC->PMC_CPU_CKR & PMC_CPU_CKR_CPPRES_Msk) >> PMC_CPU_CKR_CPPRES_Pos;
	ul_core_css = (PMC->PMC_CPU_CKR & PMC_CPU_CKR_CPCSS_Msk);
	
	/* Get Main CK */
	if (PMC->CKGR_MOR & CKGR_MOR_MOSCSEL) {
		if (PMC->CKGR_MOR & CKGR_MOR_MOSCXTBY) {
			/* 12 to 48 MHz Crystal Oscillator Bypass */
			ul_main_ck = OSC_MAINCK_BYPASS_HZ;
		} else {
			/* The Main crystal oscillator is selected */
			ul_main_ck = OSC_MAINCK_XTAL_HZ;
		}
	} else {
		/* The Main RC oscillator is selected */
		ul_main_ck = OSC_MAINCK_12M_RC_HZ;
	}
	
	/* Get TD slck */
	if (SUPC->SUPC_SR & SUPC_SR_TDOSCSEL_XTAL) {
		if (SUPC->SUPC_MR & SUPC_MR_OSCBYPASS_BYPASS) {
			ul_td_slck = OSC_SLCK_32K_BYPASS_HZ;
		} else {
			ul_td_slck = OSC_SLCK_32K_XTAL_HZ;
		}
	} else {
		ul_td_slck = OSC_SLCK_32K_RC_HZ;
	}
	
	switch (ul_core_css) {
	case PMC_CPU_CKR_CPCSS_SLOW_CLK:
		ul_hz_core1 = OSC_SLCK_32K_RC_HZ;
		break;
		
	case PMC_CPU_CKR_CPCSS_MAIN_CLK:
		ul_hz_core1 = ul_main_ck;
		break;
		
	case PMC_CPU_CKR_CPCSS_PLLACK1:
	case PMC_CPU_CKR_CPCSS_PLLBCK:
	case PMC_CPU_CKR_CPCSS_PLLCCK:
		{
			uint64_t pll_tmp1;
			uint64_t pll_tmp0;
			
			pmc_get_pll_config(PLLA_ID, &pll_cfg);
			
			if (pll_cfg.pll_src) {
				/* MAINCK */
				pll_tmp1 = ul_main_ck;
			} else {
				/* TD_SLCK */
				pll_tmp1 = ul_td_slck;
			}
			/* PLLACK1 output: (Fref * (MUL + 1 + (FRACR / 2^22))) / (DIV1 + 1) */
			pll_tmp1 *= (uint64_t)(pll_cfg.fracr + ((pll_cfg.mul + 1) * (uint64_t)(1 << 22)));
			pll_tmp0 = pll_tmp1 / (uint64_t)((pll_cfg.div0 + 1) * (uint64_t)(1 << 22));
			pll_tmp1 /= (uint64_t)((pll_cfg.div1 + 1) * (uint64_t)(1 << 22));
			ul_hz_core1 = (uint32_t)pll_tmp1;
			
			if (ul_core_css == PMC_CPU_CKR_CPCSS_PLLBCK) {
				pmc_get_pll_config(PLLB_ID, &pll_cfg);
		
				if (pll_cfg.pll_src) {
					/* MAINCK */
					pll_tmp0 = ul_main_ck;
				} /* else PLLACK0 */
				
				/* PLLACK1 output: (Fref * (MUL + 1 + (FRACR / 2^22))) / (DIV1 + 1) */
				pll_tmp0 *= (uint64_t)(pll_cfg.fracr + ((pll_cfg.mul + 1) * (uint64_t)(1 << 22)));
				pll_tmp0 /= (uint64_t)((pll_cfg.div0 + 1) * (uint64_t)(1 << 22));
				
				ul_hz_core1 = (uint32_t)pll_tmp0;
			} else if (ul_core_css == PMC_CPU_CKR_CPCSS_PLLCCK) {
				pmc_get_pll_config(PLLC_ID, &pll_cfg);
		
				if (pll_cfg.pll_src) {
					/* MAINCK */
					pll_tmp0 = ul_main_ck;
				} /* else PLLACK0 */
				
				/* PLLACK1 output: (Fref * (MUL + 1 + (FRACR / 2^22))) / (DIV1 + 1) */
				pll_tmp0 *= (uint64_t)(pll_cfg.fracr + ((pll_cfg.mul + 1) * (uint64_t)(1 << 22)));
				pll_tmp0 /= (uint64_t)((pll_cfg.div0 + 1) * (uint64_t)(1 << 22));
				
				ul_hz_core1 = (uint32_t)pll_tmp0;
			}
		}
		break;
	}
	
	/* Apply prescaler */
	ul_hz_core1 /= (1 << uc_presc);
	
	return ul_hz_core1;
}
#endif


/* ! @} */

/* / @cond 0 */
/**INDENT-OFF**/
#ifdef __cplusplus
}
#endif
/**INDENT-ON**/
/* / @endcond */
