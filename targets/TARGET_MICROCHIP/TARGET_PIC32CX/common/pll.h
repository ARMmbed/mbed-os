/**
 * \file
 *
 * \brief Chip-specific PLL definitions.
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

#ifndef CHIP_PLL_H_INCLUDED
#define CHIP_PLL_H_INCLUDED

#include "compiler.h" 
#include "osc.h"
#include "clocking.h"

/* / @cond 0 */
/**INDENT-OFF**/
#ifdef __cplusplus
extern "C" {
#endif
/**INDENT-ON**/
/* / @endcond */

/**
 * \weakgroup pll_group
 * @{
 */

#define PLL_OUTPUT_MIN_HZ    300000000U
#define PLL_OUTPUT_MAX_HZ    600000000U

#define PLLA_INPUT_MIN_HZ    32768U
#define PLLBC_INPUT_MIN_HZ   12000000U
#define PLL_INPUT_MAX_HZ     48000000U

#define NR_PLLS              3U
#define PLLA_ID              0U
#define PLLB_ID              1U
#define PLLC_ID              2U

enum pll_source {
	PLLA_SRC_SLCK_32K_RC     = OSC_SLCK_32K_RC,    /* !< Internal 32kHz RC oscillator. */
	PLLA_SRC_SLCK_32K_XTAL   = OSC_SLCK_32K_XTAL,  /* !< External 32kHz crystal oscillator. */
	PLLA_SRC_SLCK_32K_BYPASS = OSC_SLCK_32K_BYPASS,/* !< External 32kHz bypass oscillator. */
	PLLA_SRC_MAINCK_12M_RC   = OSC_MAINCK_12M_RC,  /* !< Internal 12MHz RC oscillator. */
	PLLA_SRC_MAINCK_XTAL     = OSC_MAINCK_XTAL,    /* !< External crystal oscillator. */
	PLLA_SRC_MAINCK_BYPASS   = OSC_MAINCK_BYPASS,  /* !< External bypass oscillator. */
	PLLB_SRC_MAINCK_12M_RC   = OSC_MAINCK_12M_RC,  /* !< Internal 12MHz RC oscillator. */
	PLLB_SRC_MAINCK_XTAL     = OSC_MAINCK_XTAL,    /* !< External crystal oscillator. */
	PLLB_SRC_MAINCK_BYPASS   = OSC_MAINCK_BYPASS,  /* !< External bypass oscillator. */
	PLLB_SRC_PLLACK0         = 99U,                /* !< Internal PLLACK0 oscillator. */
	PLLC_SRC_MAINCK_12M_RC   = OSC_MAINCK_12M_RC,  /* !< Internal 12MHz RC oscillator. */
	PLLC_SRC_MAINCK_XTAL     = OSC_MAINCK_XTAL,    /* !< External crystal oscillator. */
	PLLC_SRC_MAINCK_BYPASS   = OSC_MAINCK_BYPASS,  /* !< External bypass oscillator. */
	PLLC_SRC_PLLACK0         = 99U,                /* !< Internal PLLACK0 oscillator. */
};

/**
 * \note This function returns the following cases:
 * 	- PLLA ID : PLLACK1
 * 	- PLLB ID : PLLBCK0
 * 	- PLLC ID : PLLCCK0
 *	- 0 in case of ul_pll_id has not been defined
 */
static inline uint32_t pll_get_default_rate(uint32_t ul_pll_id)
{
	uint64_t ull_rate = 0;

	if (ul_pll_id == PLLA_ID) {
#ifdef CONFIG_PLL0_SOURCE
		ull_rate = osc_get_rate(CONFIG_PLL0_SOURCE);
		ull_rate *= (uint64_t)(CONFIG_PLL0_FRACR + ((CONFIG_PLL0_MUL + 1) * (uint64_t)(1 << 22)));
		ull_rate /= (uint64_t)((CONFIG_PLL0_DIV1 + 1) * (uint64_t)(1 << 22));
#endif
	} else if (ul_pll_id == PLLB_ID) {
#ifdef CONFIG_PLL1_SOURCE
		if (CONFIG_PLL1_SOURCE == PLLB_SRC_PLLACK0) {
	#ifdef CONFIG_PLL0_SOURCE
			ull_rate = osc_get_rate(CONFIG_PLL0_SOURCE);
			ull_rate *= (uint64_t)(CONFIG_PLL0_FRACR + ((CONFIG_PLL0_MUL + 1) * (uint64_t)(1 << 22)));
			ull_rate /= (uint64_t)((CONFIG_PLL0_DIV0 + 1) * (uint64_t)(1 << 22));
	#endif
		} else {
			ull_rate = osc_get_rate(CONFIG_PLL1_SOURCE);
		}
		
		ull_rate *= (uint64_t)(CONFIG_PLL1_FRACR + ((CONFIG_PLL1_MUL + 1) * (uint64_t)(1 << 22)));
		ull_rate /= (uint64_t)((CONFIG_PLL1_DIV0 + 1) * (uint64_t)(1 << 22));
#endif
	} else if (ul_pll_id == PLLC_ID) {
#ifdef CONFIG_PLL2_SOURCE
		if (CONFIG_PLL2_SOURCE == PLLC_SRC_PLLACK0) {
	#ifdef CONFIG_PLL0_SOURCE
			ull_rate = osc_get_rate(CONFIG_PLL0_SOURCE);
			ull_rate *= (uint64_t)(CONFIG_PLL0_FRACR + ((CONFIG_PLL0_MUL + 1) * (uint64_t)(1 << 22)));
			ull_rate /= (uint64_t)((CONFIG_PLL0_DIV0 + 1) * (uint64_t)(1 << 22));
	#endif
		} else {
			ull_rate = osc_get_rate(CONFIG_PLL2_SOURCE);
		}
		
		ull_rate *= (uint64_t)(CONFIG_PLL2_FRACR + ((CONFIG_PLL2_MUL + 1) * (uint64_t)(1 << 22)));
		ull_rate /= (uint64_t)((CONFIG_PLL2_DIV0 + 1) * (uint64_t)(1 << 22));
#endif
	}

	return (uint32_t)ull_rate;
}

static inline void pll_enable(uint32_t ul_pll_id)
{
	struct _pmc_pll_cfg pll_cfg = {0};

	// Assert(ul_pll_id < NR_PLLS);

	if (ul_pll_id == PLLA_ID) {
#ifdef CONFIG_PLL0_SOURCE
		pll_cfg.pll_id = PLLA_ID;
		pll_cfg.mul = CONFIG_PLL0_MUL;
		pll_cfg.fracr = CONFIG_PLL0_FRACR;
		pll_cfg.div0 = CONFIG_PLL0_DIV0;
		pll_cfg.div1 = CONFIG_PLL0_DIV1;
#endif
	} else if (ul_pll_id == PLLB_ID) {
#ifdef CONFIG_PLL1_SOURCE
		pll_cfg.pll_id = PLLB_ID;
		pll_cfg.mul = CONFIG_PLL1_MUL;
		pll_cfg.fracr = CONFIG_PLL1_FRACR;
		pll_cfg.div0 = CONFIG_PLL1_DIV0;
#endif
	} else if (ul_pll_id == PLLC_ID) {
#ifdef CONFIG_PLL2_SOURCE
		pll_cfg.pll_id = PLLC_ID;
		pll_cfg.mul = CONFIG_PLL2_MUL;
		pll_cfg.fracr = CONFIG_PLL2_FRACR;
		pll_cfg.div0 = CONFIG_PLL2_DIV0;
#endif
	}

	pmc_enable_pll(&pll_cfg);
}

/**
 * \note This will only disable the selected PLL, not the underlying oscillator (mainck).
 */
static inline void pll_disable(uint32_t ul_pll_id)
{
	// Assert(ul_pll_id < NR_PLLS);

	pmc_disable_pll(ul_pll_id);
}

static inline uint32_t pll_is_locked(uint32_t ul_pll_id)
{
	// Assert(ul_pll_id < NR_PLLS);

	return pmc_is_locked_pll(ul_pll_id);
}

static inline void pll_enable_source(enum pll_source e_src)
{
	switch (e_src) {
	case PLLA_SRC_SLCK_32K_RC:
	case PLLA_SRC_SLCK_32K_XTAL:
	case PLLA_SRC_SLCK_32K_BYPASS:
	case PLLB_SRC_MAINCK_12M_RC:
	case PLLB_SRC_MAINCK_XTAL:
	case PLLB_SRC_MAINCK_BYPASS:
		osc_enable(e_src);
		osc_wait_ready(e_src);
		break;

	case PLLB_SRC_PLLACK0:
#ifdef CONFIG_PLL0_SOURCE
		osc_enable(CONFIG_PLL0_SOURCE);
		osc_wait_ready(CONFIG_PLL0_SOURCE);
#endif
		break;

	default:
		// Assert(false);
		break;
	}
}

/* ! @} */

/* / @cond 0 */
/**INDENT-OFF**/
#ifdef __cplusplus
}
#endif
/**INDENT-ON**/
/* / @endcond */

#endif /* CHIP_PLL_H_INCLUDED */
