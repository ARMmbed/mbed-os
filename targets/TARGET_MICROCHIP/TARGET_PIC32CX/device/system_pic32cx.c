/* ---------------------------------------------------------------------------- */
/*                Microchip Microcontroller Software Support                    */
/*                       SAM Software Package License                           */
/* ---------------------------------------------------------------------------- */
/* Copyright (c) 2021, Microchip Technology Inc.                                */
/*                                                                              */
/* All rights reserved.                                                         */
/*                                                                              */
/* Redistribution and use in source and binary forms, with or without           */
/* modification, are permitted provided that the following condition is met:    */
/*                                                                              */
/* - Redistributions of source code must retain the above copyright notice,     */
/* this list of conditions and the disclaimer below.                            */
/*                                                                              */
/* Microchip's name may not be used to endorse or promote products derived from */
/* this software without specific prior written permission.                     */
/*                                                                              */
/* DISCLAIMER:  THIS SOFTWARE IS PROVIDED BY MICROCHIP "AS IS" AND ANY EXPRESS  */
/* OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES */
/* OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NON-INFRINGEMENT    */
/* ARE DISCLAIMED. IN NO EVENT SHALL MICROCHIP BE LIABLE FOR ANY DIRECT,        */
/* INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES           */
/* (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; */
/* LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND  */
/* ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT   */
/* (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF     */
/* THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.            */
/* ---------------------------------------------------------------------------- */

#include "pic32cx.h"
#include "board.h"
#include "compiler.h"

/* @cond 0 */
/**INDENT-OFF**/
#ifdef __cplusplus
extern "C" {
#endif
/**INDENT-ON**/
/* @endcond */

/* tACC(max) = 30 ns  + 2ns (Routing flash + AHB) */
/* Read access time of flash in ns (agr_flash_esf340_1mb_p1.v) */
#define SYSTEM_TACC_FLASH    32

/* Clock Settings (12MHz) using Internal Fast RC */
uint32_t SystemCoreClock = CHIP_FREQ_MAINCK_RC_12MHZ;
uint32_t SystemCore1Clock = 0;

static uint32_t _get_mainck(void)
{
	uint32_t ul_clk;

	if (PMC->CKGR_MOR & CKGR_MOR_MOSCSEL) {
		if (PMC->CKGR_MOR & CKGR_MOR_MOSCXTBY) {
			ul_clk = BOARD_FREQ_MAINCK_BYPASS;
		} else {
			ul_clk = BOARD_FREQ_MAINCK_XTAL;
		}
	} else {
		ul_clk = CHIP_FREQ_MAINCK_RC_12MHZ;
	}

	return ul_clk;
}

static uint32_t _get_td_slck(void)
{
	uint32_t ul_clk;

	if (SUPC->SUPC_SR & SUPC_SR_TDOSCSEL) {
		if (SUPC->SUPC_MR & SUPC_MR_OSCBYPASS) {
			ul_clk = BOARD_FREQ_SLCK_BYPASS;
		} else {
			ul_clk = CHIP_FREQ_XTAL_32K;
		}
	} else {
		ul_clk = CHIP_FREQ_SLCK_RC;
	}

	return ul_clk;
}

static uint32_t _get_pll_clk(uint8_t uc_pll_id, uint8_t uc_div_idx)
{
	uint64_t pll_tmp;
	uint32_t ul_clk;
	uint32_t mul, divpmc, fracr;

	/* Select PLL ID */
	PMC->PMC_PLL_UPDT &= ~PMC_PLL_UPDT_ID_Msk;
	PMC->PMC_PLL_UPDT |= PMC_PLL_UPDT_ID(uc_pll_id);

	if (uc_div_idx == 1) {
		divpmc = (PMC->PMC_PLL_CTRL0 & PMC_PLL_CTRL0_DIVPMC1_Msk) >> PMC_PLL_CTRL0_DIVPMC1_Pos;
	} else {
		divpmc = (PMC->PMC_PLL_CTRL0 & PMC_PLL_CTRL0_DIVPMC0_Msk) >> PMC_PLL_CTRL0_DIVPMC0_Pos;
	}

	mul = PMC->PMC_PLL_CTRL1 & PMC_PLL_CTRL1_MUL_Msk;
	fracr = PMC->PMC_PLL_CTRL2 & PMC_PLL_CTRL2_FRACR_Msk;

	if (uc_pll_id == 0) {
		/* PLLA */
		if (PMC->PMC_PLL_CTRL0 & PMC_PLL_CTRL0_PLLMS) {
			ul_clk = _get_mainck();
		} else {
			ul_clk = _get_td_slck();
		}
	} else {
		/* PLLB, C */
		if (PMC->PMC_PLL_CTRL0 & PMC_PLL_CTRL0_PLLMS) {
			ul_clk = _get_mainck();
		} else {
			ul_clk = _get_pll_clk(0, 0);
		}
	}

	/* PLLxCK output: (Fref * (MUL + 1 + (FRACR / 2^22))) / (DIV + 1) */
	pll_tmp = (uint64_t)ul_clk;
	pll_tmp *= (uint64_t)(fracr + ((mul + 1) * (uint64_t)(1 << 22)));
	pll_tmp /= (uint64_t)((divpmc + 1) * (uint64_t)(1 << 22));
	ul_clk = (uint32_t)pll_tmp;

	return ul_clk;
}

/**
 * \brief Setup the microcontroller system.
 *
 * Initialize the System and update the SystemFrequency variable.
 */
void SystemInit( void )
{
	/* Set FWS to max value to allow any clock frequency */
	SEFC0->EEFC_FMR = EEFC_FMR_FWS(0xF) | EEFC_FMR_CLOE;
#ifdef SEFC1
	SEFC1->EEFC_FMR = EEFC_FMR_FWS(0xF) | EEFC_FMR_CLOE;
#endif
}

uint32_t SystemGetPllClk(uint8_t uc_pll_id, uint8_t uc_div_idx)
{
	return _get_pll_clk(uc_pll_id, uc_div_idx);
}

/**
 * \brief Get Core0 Clock Frequency.
 */
void SystemCoreClockUpdate( void )
{
	/* Determine clock frequency according to clock register values */
	switch (PMC->PMC_CPU_CKR & (uint32_t)PMC_CPU_CKR_CSS_Msk) {
	case PMC_CPU_CKR_CSS_SLOW_CLK:         /* Slow clock */
	{
		SystemCoreClock = _get_td_slck();
	}
	break;

	case PMC_CPU_CKR_CSS_MAIN_CLK:         /* Main clock */
	{
		SystemCoreClock = _get_mainck();
	}
	break;

	case PMC_CPU_CKR_CSS_PLLACK1:         /* PLLA1 clock */
	{
		SystemCoreClock = _get_pll_clk(0, 1);
	}
	break;

	case PMC_CPU_CKR_CSS_PLLBCK:         /* PLLB0 clock */
	{
		SystemCoreClock = _get_pll_clk(1, 0);
	}
	break;

	default:
		/* TODO: We are out of specs. */
		break;
	}

	if ((PMC->PMC_CPU_CKR & PMC_CPU_CKR_PRES_Msk) == PMC_CPU_CKR_PRES_CLK_3) {
		SystemCoreClock /= 3U;
	} else {
		SystemCoreClock >>= ((PMC->PMC_CPU_CKR & PMC_CPU_CKR_PRES_Msk) >> PMC_CPU_CKR_PRES_Pos);
	}
}

/**
 * \brief Get Core1 Clock Frequency.
 */
void SystemCore1ClockUpdate( void )
{
	/* Determine clock frequency according to clock register values */
	switch (PMC->PMC_CPU_CKR & (uint32_t)PMC_CPU_CKR_CPCSS_Msk) {
	case PMC_CPU_CKR_CPCSS_SLOW_CLK:         /* Slow clock */
	{
		SystemCore1Clock = _get_td_slck();
	}
	break;

	case PMC_CPU_CKR_CPCSS_MAIN_CLK:         /* Main clock */
	{
		SystemCore1Clock = _get_mainck();
	}
	break;

	case PMC_CPU_CKR_CPCSS_MCK0:         /* MCK0 */
	{
		SystemCore1Clock = SystemCoreClock;
	}
	break;

	case PMC_CPU_CKR_CPCSS_PLLACK1:         /* PLLA1 clock */
	{
		SystemCore1Clock = _get_pll_clk(0, 1);
	}
	break;

	case PMC_CPU_CKR_CPCSS_PLLBCK:         /* PLLB0 clock */
	{
		SystemCore1Clock = _get_pll_clk(1, 0);
	}
	break;

	case PMC_CPU_CKR_CPCSS_PLLCCK:         /* PLLC0 clock */
	{
		SystemCore1Clock = _get_pll_clk(2, 0);
	}
	break;

	default:
		/* TODO: We are out of specs. */
		break;
	}

	/* Adjust prescaler */
	SystemCore1Clock /= (((PMC->PMC_CPU_CKR & PMC_CPU_CKR_CPPRES_Msk) >> PMC_CPU_CKR_CPPRES_Pos) + 1);

}

/**
 * \brief Initialize flash wait state according to operating frequency.
 *
 * \param ul_clk System clock frequency.
 */
void system_init_flash(uint32_t ul_clk)
{
	uint32_t ul_fws;
	uint32_t ul_clk_mhz;

	ul_clk_mhz = ul_clk / 1000000;

	if (ul_clk_mhz >= 160) {
		/* Disable write protect of SFR */
		SFR->SFR_WPMR = SFR_WPMR_WPKEY_PASSWD;
		/* Enable flash patch */
		SFR->SFR_FLASH = 0x00000000;
		/* Enable write protect of SFR */
		SFR->SFR_WPMR = SFR_WPMR_WPKEY_PASSWD | SFR_WPMR_WPEN;
	} else {
		/* Disable write protect of SFR */
		SFR->SFR_WPMR = SFR_WPMR_WPKEY_PASSWD;
		/* Disable flash patch */
		SFR->SFR_FLASH = 0x00000001;
		/* Enable write protect of SFR */
		SFR->SFR_WPMR = SFR_WPMR_WPKEY_PASSWD | SFR_WPMR_WPEN;
	}

	/* Set FWS for embedded Flash access according to operating frequency */
	ul_fws = div_ceil((SYSTEM_TACC_FLASH * ul_clk_mhz), 1000);

	/* Enable CLOE(Code Loop Optimization Enable) by default */
	SEFC0->EEFC_FMR = EEFC_FMR_FWS(ul_fws) | EEFC_FMR_CLOE;
#ifdef SEFC1
	SEFC1->EEFC_FMR = EEFC_FMR_FWS(ul_fws) | EEFC_FMR_CLOE;
#endif
}

/* @cond 0 */
/**INDENT-OFF**/
#ifdef __cplusplus
}
#endif
/**INDENT-ON**/
/* @endcond */
