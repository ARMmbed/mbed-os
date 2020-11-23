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

#ifndef _PIC32CX_PMC_COMPONENT_
#define _PIC32CX_PMC_COMPONENT_

/* ============================================================================= */
/**  SOFTWARE API DEFINITION FOR Power Management Controller */
/* ============================================================================= */
/** \addtogroup PIC32CX_PMC Power Management Controller */
/*@{*/

#if !(defined(__ASSEMBLY__) || defined(__IAR_SYSTEMS_ASM__))
/** \brief Pmc hardware registers */
typedef struct {
	__O uint32_t PMC_SCER;  /**< \brief (Pmc Offset: 0x0) System Clock Enable Register */
	__O uint32_t PMC_SCDR;  /**< \brief (Pmc Offset: 0x4) System Clock Disable Register */
	__I uint32_t PMC_SCSR;  /**< \brief (Pmc Offset: 0x8) System Clock Status Register */
	__IO uint32_t PMC_PLL_CTRL0; /**< \brief (Pmc Offset: 0xC) PLL Control Register 0 */
	__IO uint32_t PMC_PLL_CTRL1; /**< \brief (Pmc Offset: 0x10) PLL Control Register 1 */
	__IO uint32_t PMC_PLL_CTRL2; /**< \brief (Pmc Offset: 0x14) PLL Control Register 2 */
	__IO uint32_t PMC_PLL_SSR; /**< \brief (Pmc Offset: 0x18) PLL Spread Spectrum Register */
	__IO uint32_t PMC_PLL_ACR; /**< \brief (Pmc Offset: 0x1C) PLL Analog Control Register */
	__IO uint32_t PMC_PLL_UPDT; /**< \brief (Pmc Offset: 0x20) PLL Update Register */
	__IO uint32_t CKGR_MOR; /**< \brief (Pmc Offset: 0x24) Main Oscillator Register */
	__IO uint32_t CKGR_MCFR; /**< \brief (Pmc Offset: 0x28) Main Clock Frequency Register */
	__IO uint32_t PMC_CPU_CKR; /**< \brief (Pmc Offset: 0x2C) CPU Clock Register */
	__IO uint32_t PMC_CPU_RATIO; /**< \brief (Pmc Offset: 0x30) CPU Clock Ratio Register */
	__I uint32_t Reserved2[4];
	__IO uint32_t PMC_PCK[3]; /**< \brief (Pmc Offset: 0x44) Programmable Clock Register (chid = 0) */
	__I uint32_t Reserved3[5];
	__O uint32_t PMC_IER;   /**< \brief (Pmc Offset: 0x64) Interrupt Enable Register */
	__O uint32_t PMC_IDR;   /**< \brief (Pmc Offset: 0x68) Interrupt Disable Register */
	__I uint32_t PMC_SR;    /**< \brief (Pmc Offset: 0x6C) Status Register */
	__I uint32_t PMC_IMR;   /**< \brief (Pmc Offset: 0x70) Interrupt Mask Register */
	__IO uint32_t PMC_FSMR; /**< \brief (Pmc Offset: 0x74) Fast Start-up Mode Register */
	__IO uint32_t PMC_WCR;  /**< \brief (Pmc Offset: 0x78) Wake-up Control Register */
	__O uint32_t PMC_FOCR;  /**< \brief (Pmc Offset: 0x7C) Fault Output Clear Register */
	__IO uint32_t PMC_CPFSMR; /**< \brief (Pmc Offset: 0x80) Coprocessor Fast Start-up Mode Register */
	__IO uint32_t PMC_WPMR; /**< \brief (Pmc Offset: 0x84) Write Protection Mode Register */
	__I uint32_t PMC_WPSR;  /**< \brief (Pmc Offset: 0x88) Write Protection Status Register */
	__IO uint32_t PMC_PCR;  /**< \brief (Pmc Offset: 0x8C) Peripheral Control Register */
	__IO uint32_t PMC_OCR;  /**< \brief (Pmc Offset: 0x90) Oscillator Calibration Register */
	__I uint32_t PMC_SLPWK_AIPR; /**< \brief (Pmc Offset: 0x94) SleepWalking Activity In Progress Register */
	__IO uint32_t PMC_SLPWKCR; /**< \brief (Pmc Offset: 0x98) SleepWalking Control Register */
	__I uint32_t Reserved4[1];
	__IO uint32_t PMC_MCKLIM; /**< \brief (Pmc Offset: 0xA0) MCK0 Monitor Limits Register */
	__I uint32_t PMC_CSR0;  /**< \brief (Pmc Offset: 0xA4) Peripheral Clock Status Register 0 */
	__I uint32_t PMC_CSR1;  /**< \brief (Pmc Offset: 0xA8) Peripheral Clock Status Register 1 */
	__I uint32_t PMC_CSR2;  /**< \brief (Pmc Offset: 0xAC) Peripheral Clock Status Register 2 */
	__I uint32_t PMC_CSR3;  /**< \brief (Pmc Offset: 0xB0) Peripheral Clock Status Register 3 */
	__I uint32_t Reserved5[4];
	__I uint32_t PMC_GCSR0; /**< \brief (Pmc Offset: 0xC4) Generic Clock Status Register 0 */
	__I uint32_t PMC_GCSR1; /**< \brief (Pmc Offset: 0xC8) Generic Clock Status Register 1 */
	__I uint32_t PMC_GCSR2; /**< \brief (Pmc Offset: 0xCC) Generic Clock Status Register 2 */
	__I uint32_t PMC_GCSR3; /**< \brief (Pmc Offset: 0xD0) Generic Clock Status Register 3 */
	__I uint32_t Reserved6[4];
	__O uint32_t PMC_PLL_IER; /**< \brief (Pmc Offset: 0xE4) PLL Interrupt Enable Register */
	__O uint32_t PMC_PLL_IDR; /**< \brief (Pmc Offset: 0xE8) PLL Interrupt Disable Register */
	__I uint32_t PMC_PLL_IMR; /**< \brief (Pmc Offset: 0xEC) PLL Interrupt Mask Register */
	__I uint32_t PMC_PLL_ISR0; /**< \brief (Pmc Offset: 0xF0) PLL Interrupt Status Register 0 */
	__I uint32_t PMC_PLL_ISR1; /**< \brief (Pmc Offset: 0xF4) PLL Interrupt Status Register 1 */
} Pmc;
#endif /* !(defined(__ASSEMBLY__) || defined(__IAR_SYSTEMS_ASM__)) */
/* -------- PMC_SCER : (PMC Offset: 0x0) System Clock Enable Register -------- */
#define PMC_SCER_PCK0 (0x1u << 8) /**< \brief (PMC_SCER) Programmable Clock 0 Output Enable */
#define PMC_SCER_PCK1 (0x1u << 9) /**< \brief (PMC_SCER) Programmable Clock 1 Output Enable */
#define PMC_SCER_PCK2 (0x1u << 10) /**< \brief (PMC_SCER) Programmable Clock 2 Output Enable */
#define PMC_SCER_PCK3 (0x1u << 11) /**< \brief (PMC_SCER) Programmable Clock 3 Output Enable */
#define PMC_SCER_CPCK (0x1u << 16) /**< \brief (PMC_SCER) Coprocessor (Second Processor) Clocks Enable */
#define PMC_SCER_CPBMCK (0x1u << 17) /**< \brief (PMC_SCER) Coprocessor Bus Master Clocks Enable */
#define PMC_SCER_CPKEY_Pos 20
#define PMC_SCER_CPKEY_Msk (0xfu << PMC_SCER_CPKEY_Pos) /**< \brief (PMC_SCER) Coprocessor Clocks Enable Key */
#define PMC_SCER_CPKEY(value) ((PMC_SCER_CPKEY_Msk & ((value) << PMC_SCER_CPKEY_Pos)))
#define   PMC_SCER_CPKEY_PASSWD (0xAu << 20) /**< \brief (PMC_SCER) This field must be written to 0xA to validate CPBMCK and CPCK. */
/* -------- PMC_SCDR : (PMC Offset: 0x4) System Clock Disable Register -------- */
#define PMC_SCDR_PCK0 (0x1u << 8) /**< \brief (PMC_SCDR) Programmable Clock 0 Output Disable */
#define PMC_SCDR_PCK1 (0x1u << 9) /**< \brief (PMC_SCDR) Programmable Clock 1 Output Disable */
#define PMC_SCDR_PCK2 (0x1u << 10) /**< \brief (PMC_SCDR) Programmable Clock 2 Output Disable */
#define PMC_SCDR_PCK3 (0x1u << 11) /**< \brief (PMC_SCDR) Programmable Clock 3 Output Disable */
#define PMC_SCDR_CPCK (0x1u << 16) /**< \brief (PMC_SCDR) Coprocessor (Second Processor) Clocks Disable */
#define PMC_SCDR_CPBMCK (0x1u << 17) /**< \brief (PMC_SCDR) Coprocessor Bus Master Clocks Disable */
#define PMC_SCDR_CPKEY_Pos 20
#define PMC_SCDR_CPKEY_Msk (0xfu << PMC_SCDR_CPKEY_Pos) /**< \brief (PMC_SCDR) Coprocessor Clocks Enable Key */
#define PMC_SCDR_CPKEY(value) ((PMC_SCDR_CPKEY_Msk & ((value) << PMC_SCDR_CPKEY_Pos)))
#define   PMC_SCDR_CPKEY_PASSWD (0xAu << 20) /**< \brief (PMC_SCDR) This field must be written to 0xA to validate CPBMCK and CPCK. */
/* -------- PMC_SCSR : (PMC Offset: 0x8) System Clock Status Register -------- */
#define PMC_SCSR_CPU_CLK0S (0x1u << 0) /**< \brief (PMC_SCSR) CPU_CLK Status for Core 0 */
#define PMC_SCSR_CPU_CLK1S (0x1u << 1) /**< \brief (PMC_SCSR) CPU_CLK Status for Core 1 */
#define PMC_SCSR_PCK0 (0x1u << 8) /**< \brief (PMC_SCSR) Programmable Clock 0 Output Status */
#define PMC_SCSR_PCK1 (0x1u << 9) /**< \brief (PMC_SCSR) Programmable Clock 1 Output Status */
#define PMC_SCSR_PCK2 (0x1u << 10) /**< \brief (PMC_SCSR) Programmable Clock 2 Output Status */
#define PMC_SCSR_PCK3 (0x1u << 11) /**< \brief (PMC_SCSR) Programmable Clock 3 Output Status */
#define PMC_SCSR_CPCK (0x1u << 16) /**< \brief (PMC_SCSR) Coprocessor (Second Processor) Clocks Status */
#define PMC_SCSR_CPBMCK (0x1u << 17) /**< \brief (PMC_SCSR) Coprocessor Bus Master Clocks Status */
/* -------- PMC_PLL_CTRL0 : (PMC Offset: 0xC) PLL Control Register 0 -------- */
#define PMC_PLL_CTRL0_DIVPMC0_Pos 0
#define PMC_PLL_CTRL0_DIVPMC0_Msk (0xffu << PMC_PLL_CTRL0_DIVPMC0_Pos) /**< \brief (PMC_PLL_CTRL0) Divider for PMC output 0 */
#define PMC_PLL_CTRL0_DIVPMC0(value) ((PMC_PLL_CTRL0_DIVPMC0_Msk & ((value) << PMC_PLL_CTRL0_DIVPMC0_Pos)))
#define PMC_PLL_CTRL0_DIVPMC1_Pos 12
#define PMC_PLL_CTRL0_DIVPMC1_Msk (0xffu << PMC_PLL_CTRL0_DIVPMC1_Pos) /**< \brief (PMC_PLL_CTRL0) Divider for PMC output 1 (PLLA only) */
#define PMC_PLL_CTRL0_DIVPMC1(value) ((PMC_PLL_CTRL0_DIVPMC1_Msk & ((value) << PMC_PLL_CTRL0_DIVPMC1_Pos)))
#define PMC_PLL_CTRL0_PLLMS (0x1u << 27) /**< \brief (PMC_PLL_CTRL0) PLL Multiplexer Select */
#define PMC_PLL_CTRL0_ENPLL (0x1u << 28) /**< \brief (PMC_PLL_CTRL0) Enable PLL */
#define PMC_PLL_CTRL0_ENPLLO0 (0x1u << 29) /**< \brief (PMC_PLL_CTRL0) Enable PLL Clock Output 0 */
#define PMC_PLL_CTRL0_ENPLLO1 (0x1u << 30) /**< \brief (PMC_PLL_CTRL0) Enable PLL Clock Output 1 (PLLA only) */
#define PMC_PLL_CTRL0_ENLOCK (0x1u << 31) /**< \brief (PMC_PLL_CTRL0) Enable PLL Lock */
/* -------- PMC_PLL_CTRL1 : (PMC Offset: 0x10) PLL Control Register 1 -------- */
#define PMC_PLL_CTRL1_MUL_Pos 0
#define PMC_PLL_CTRL1_MUL_Msk (0xffffu << PMC_PLL_CTRL1_MUL_Pos) /**< \brief (PMC_PLL_CTRL1) Multiplier Factor Value */
#define PMC_PLL_CTRL1_MUL(value) ((PMC_PLL_CTRL1_MUL_Msk & ((value) << PMC_PLL_CTRL1_MUL_Pos)))
/* -------- PMC_PLL_CTRL2 : (PMC Offset: 0x14) PLL Control Register 2 -------- */
#define PMC_PLL_CTRL2_FRACR_Pos 0
#define PMC_PLL_CTRL2_FRACR_Msk (0x3fffffu << PMC_PLL_CTRL2_FRACR_Pos) /**< \brief (PMC_PLL_CTRL2) Fractional Loop Divider Setting */
#define PMC_PLL_CTRL2_FRACR(value) ((PMC_PLL_CTRL2_FRACR_Msk & ((value) << PMC_PLL_CTRL2_FRACR_Pos)))
/* -------- PMC_PLL_SSR : (PMC Offset: 0x18) PLL Spread Spectrum Register -------- */
#define PMC_PLL_SSR_STEP_Pos 0
#define PMC_PLL_SSR_STEP_Msk (0xffffu << PMC_PLL_SSR_STEP_Pos) /**< \brief (PMC_PLL_SSR) Spread Spectrum Step Size */
#define PMC_PLL_SSR_STEP(value) ((PMC_PLL_SSR_STEP_Msk & ((value) << PMC_PLL_SSR_STEP_Pos)))
#define PMC_PLL_SSR_NSTEP_Pos 16
#define PMC_PLL_SSR_NSTEP_Msk (0xffu << PMC_PLL_SSR_NSTEP_Pos) /**< \brief (PMC_PLL_SSR) Spread Spectrum Number of Step */
#define PMC_PLL_SSR_NSTEP(value) ((PMC_PLL_SSR_NSTEP_Msk & ((value) << PMC_PLL_SSR_NSTEP_Pos)))
#define PMC_PLL_SSR_ENSPREAD (0x1u << 28) /**< \brief (PMC_PLL_SSR) Spread Spectrum Enable */
/* -------- PMC_PLL_ACR : (PMC Offset: 0x1C) PLL Analog Control Register -------- */
#define PMC_PLL_ACR_CONTROL_Pos 0
#define PMC_PLL_ACR_CONTROL_Msk (0xfffu << PMC_PLL_ACR_CONTROL_Pos) /**< \brief (PMC_PLL_ACR) PLL CONTROL Value Selection */
#define PMC_PLL_ACR_CONTROL(value) ((PMC_PLL_ACR_CONTROL_Msk & ((value) << PMC_PLL_ACR_CONTROL_Pos)))
#define PMC_PLL_ACR_LOCK_THR_Pos 16
#define PMC_PLL_ACR_LOCK_THR_Msk (0xffu << PMC_PLL_ACR_LOCK_THR_Pos) /**< \brief (PMC_PLL_ACR) PLL Lock Threshold Value Selection */
#define PMC_PLL_ACR_LOCK_THR(value) ((PMC_PLL_ACR_LOCK_THR_Msk & ((value) << PMC_PLL_ACR_LOCK_THR_Pos)))
#define PMC_PLL_ACR_LOOP_FILTER_Pos 24
#define PMC_PLL_ACR_LOOP_FILTER_Msk (0xffu << PMC_PLL_ACR_LOOP_FILTER_Pos) /**< \brief (PMC_PLL_ACR) LOOP Filter Selection */
#define PMC_PLL_ACR_LOOP_FILTER(value) ((PMC_PLL_ACR_LOOP_FILTER_Msk & ((value) << PMC_PLL_ACR_LOOP_FILTER_Pos)))
/* -------- PMC_PLL_UPDT : (PMC Offset: 0x20) PLL Update Register -------- */
#define PMC_PLL_UPDT_ID_Pos 0
#define PMC_PLL_UPDT_ID_Msk (0xfu << PMC_PLL_UPDT_ID_Pos) /**< \brief (PMC_PLL_UPDT) PLL ID */
#define PMC_PLL_UPDT_ID(value) ((PMC_PLL_UPDT_ID_Msk & ((value) << PMC_PLL_UPDT_ID_Pos)))
#define PMC_PLL_UPDT_UPDATE (0x1u << 8) /**< \brief (PMC_PLL_UPDT) PLL Setting Update (write-only) */
#define PMC_PLL_UPDT_STUPTIM_Pos 16
#define PMC_PLL_UPDT_STUPTIM_Msk (0xffu << PMC_PLL_UPDT_STUPTIM_Pos) /**< \brief (PMC_PLL_UPDT) Start-up Time */
#define PMC_PLL_UPDT_STUPTIM(value) ((PMC_PLL_UPDT_STUPTIM_Msk & ((value) << PMC_PLL_UPDT_STUPTIM_Pos)))
/* -------- CKGR_MOR : (PMC Offset: 0x24) Main Oscillator Register -------- */
#define CKGR_MOR_MOSCXTEN (0x1u << 0) /**< \brief (CKGR_MOR) Main Crystal Oscillator Enable */
#define CKGR_MOR_MOSCXTBY (0x1u << 1) /**< \brief (CKGR_MOR) 12 to 48 MHz Crystal Oscillator Bypass */
#define CKGR_MOR_WAITMODE (0x1u << 2) /**< \brief (CKGR_MOR) Wait Mode Command (write-only) */
#define CKGR_MOR_MOSCRCEN (0x1u << 3) /**< \brief (CKGR_MOR) Main RC Oscillator Enable */
#define CKGR_MOR_MOSCXTST_Pos 8
#define CKGR_MOR_MOSCXTST_Msk (0xffu << CKGR_MOR_MOSCXTST_Pos) /**< \brief (CKGR_MOR) Main Crystal Oscillator Start-up Time */
#define CKGR_MOR_MOSCXTST(value) ((CKGR_MOR_MOSCXTST_Msk & ((value) << CKGR_MOR_MOSCXTST_Pos)))
#define CKGR_MOR_KEY_Pos 16
#define CKGR_MOR_KEY_Msk (0xffu << CKGR_MOR_KEY_Pos) /**< \brief (CKGR_MOR) Write Access Password */
#define CKGR_MOR_KEY(value) ((CKGR_MOR_KEY_Msk & ((value) << CKGR_MOR_KEY_Pos)))
#define   CKGR_MOR_KEY_PASSWD (0x37u << 16) /**< \brief (CKGR_MOR) Writing any other value in this field aborts the write operation. Always reads as 0. */
#define CKGR_MOR_MOSCSEL (0x1u << 24) /**< \brief (CKGR_MOR) Main Clock Oscillator Selection */
#define CKGR_MOR_CFDEN (0x1u << 25) /**< \brief (CKGR_MOR) Clock Failure Detector Enable */
#define CKGR_MOR_XT32KFME (0x1u << 26) /**< \brief (CKGR_MOR) 32.768 kHz Crystal Oscillator Frequency Monitoring Enable */
#define CKGR_MOR_BMCKRST (0x1u << 27) /**< \brief (CKGR_MOR) Bad MCK0 Clock Reset Enable */
/* -------- CKGR_MCFR : (PMC Offset: 0x28) Main Clock Frequency Register -------- */
#define CKGR_MCFR_MAINF_Pos 0
#define CKGR_MCFR_MAINF_Msk (0xffffu << CKGR_MCFR_MAINF_Pos) /**< \brief (CKGR_MCFR) Main Clock Frequency */
#define CKGR_MCFR_MAINF(value) ((CKGR_MCFR_MAINF_Msk & ((value) << CKGR_MCFR_MAINF_Pos)))
#define CKGR_MCFR_MAINFRDY (0x1u << 16) /**< \brief (CKGR_MCFR) Main Clock Frequency Measure Ready */
#define CKGR_MCFR_RCMEAS (0x1u << 20) /**< \brief (CKGR_MCFR) RC Oscillator Frequency Measure (write-only) */
#define CKGR_MCFR_CCSS (0x1u << 24) /**< \brief (CKGR_MCFR) Counter Clock Source Selection */
/* -------- PMC_CPU_CKR : (PMC Offset: 0x2C) CPU Clock Register -------- */
#define PMC_CPU_CKR_CSS_Pos 0
#define PMC_CPU_CKR_CSS_Msk (0x3u << PMC_CPU_CKR_CSS_Pos) /**< \brief (PMC_CPU_CKR) Processor (CPU_CLK0) and Master Clock (MCK0) Source Selection */
#define PMC_CPU_CKR_CSS(value) ((PMC_CPU_CKR_CSS_Msk & ((value) << PMC_CPU_CKR_CSS_Pos)))
#define   PMC_CPU_CKR_CSS_SLOW_CLK (0x0u << 0) /**< \brief (PMC_CPU_CKR) MD_SLCK is selected */
#define   PMC_CPU_CKR_CSS_MAIN_CLK (0x1u << 0) /**< \brief (PMC_CPU_CKR) MAINCK is selected */
#define   PMC_CPU_CKR_CSS_PLLACK1 (0x2u << 0) /**< \brief (PMC_CPU_CKR) PLLACK1 is selected */
#define   PMC_CPU_CKR_CSS_PLLBCK (0x3u << 0) /**< \brief (PMC_CPU_CKR) PLLBCK is selected */
#define PMC_CPU_CKR_PRES_Pos 4
#define PMC_CPU_CKR_PRES_Msk (0x7u << PMC_CPU_CKR_PRES_Pos) /**< \brief (PMC_CPU_CKR) Processor (CPU_CLK0) and Master Clock (MCK0) Prescaler */
#define PMC_CPU_CKR_PRES(value) ((PMC_CPU_CKR_PRES_Msk & ((value) << PMC_CPU_CKR_PRES_Pos)))
#define   PMC_CPU_CKR_PRES_CLK_1 (0x0u << 4) /**< \brief (PMC_CPU_CKR) Selected clock */
#define   PMC_CPU_CKR_PRES_CLK_2 (0x1u << 4) /**< \brief (PMC_CPU_CKR) Selected clock divided by 2 */
#define   PMC_CPU_CKR_PRES_CLK_4 (0x2u << 4) /**< \brief (PMC_CPU_CKR) Selected clock divided by 4 */
#define   PMC_CPU_CKR_PRES_CLK_8 (0x3u << 4) /**< \brief (PMC_CPU_CKR) Selected clock divided by 8 */
#define   PMC_CPU_CKR_PRES_CLK_16 (0x4u << 4) /**< \brief (PMC_CPU_CKR) Selected clock divided by 16 */
#define   PMC_CPU_CKR_PRES_CLK_32 (0x5u << 4) /**< \brief (PMC_CPU_CKR) Selected clock divided by 32 */
#define   PMC_CPU_CKR_PRES_CLK_64 (0x6u << 4) /**< \brief (PMC_CPU_CKR) Selected clock divided by 64 */
#define   PMC_CPU_CKR_PRES_CLK_3 (0x7u << 4) /**< \brief (PMC_CPU_CKR) Selected clock divided by 3 */
#define PMC_CPU_CKR_CPCSS_Pos 16
#define PMC_CPU_CKR_CPCSS_Msk (0x7u << PMC_CPU_CKR_CPCSS_Pos) /**< \brief (PMC_CPU_CKR) Coprocessor MCK1 Source Selection */
#define PMC_CPU_CKR_CPCSS(value) ((PMC_CPU_CKR_CPCSS_Msk & ((value) << PMC_CPU_CKR_CPCSS_Pos)))
#define   PMC_CPU_CKR_CPCSS_SLOW_CLK (0x0u << 16) /**< \brief (PMC_CPU_CKR) MD_SLCK is selected */
#define   PMC_CPU_CKR_CPCSS_MAIN_CLK (0x1u << 16) /**< \brief (PMC_CPU_CKR) MAINCK is selected */
#define   PMC_CPU_CKR_CPCSS_MCK0 (0x2u << 16) /**< \brief (PMC_CPU_CKR) MCK0 is selected */
#define   PMC_CPU_CKR_CPCSS_PLLACK1 (0x3u << 16) /**< \brief (PMC_CPU_CKR) PLLACK1 is selected */
#define   PMC_CPU_CKR_CPCSS_PLLBCK (0x4u << 16) /**< \brief (PMC_CPU_CKR) PLLBCK is selected */
#define   PMC_CPU_CKR_CPCSS_PLLCCK (0x5u << 16) /**< \brief (PMC_CPU_CKR) PLLCCK is selected */
#define PMC_CPU_CKR_CPPRES_Pos 20
#define PMC_CPU_CKR_CPPRES_Msk (0xfu << PMC_CPU_CKR_CPPRES_Pos) /**< \brief (PMC_CPU_CKR) Coprocessor Clock Prescaler */
#define PMC_CPU_CKR_CPPRES(value) ((PMC_CPU_CKR_CPPRES_Msk & ((value) << PMC_CPU_CKR_CPPRES_Pos)))
#define PMC_CPU_CKR_RATIO_MCK0DIV (0x1u << 24) /**< \brief (PMC_CPU_CKR) MCK0 Clock Frequency Division for MCK0DIV Clock */
#define PMC_CPU_CKR_RATIO_MCK1DIV (0x1u << 25) /**< \brief (PMC_CPU_CKR) MCK1 Clock Frequency Division for MCK1DIV Clock */
#define PMC_CPU_CKR_RATIO_MCK0DIV2 (0x1u << 26) /**< \brief (PMC_CPU_CKR) MCK0 Clock Frequency Division for MCK0DIV2 Clock */
/* -------- PMC_CPU_RATIO : (PMC Offset: 0x30) CPU Clock Ratio Register -------- */
#define PMC_CPU_RATIO_RATIO_Pos 0
#define PMC_CPU_RATIO_RATIO_Msk (0xfu << PMC_CPU_RATIO_RATIO_Pos) /**< \brief (PMC_CPU_RATIO) CPU Clock Ratio */
#define PMC_CPU_RATIO_RATIO(value) ((PMC_CPU_RATIO_RATIO_Msk & ((value) << PMC_CPU_RATIO_RATIO_Pos)))
/* -------- PMC_PCK[3] : (PMC Offset: 0x44) Programmable Clock Register (chid = 0) -------- */
#define PMC_PCK_CSS_Pos 0
#define PMC_PCK_CSS_Msk (0x1fu << PMC_PCK_CSS_Pos) /**< \brief (PMC_PCK[3]) Programmable Clock Source Selection */
#define PMC_PCK_CSS(value) ((PMC_PCK_CSS_Msk & ((value) << PMC_PCK_CSS_Pos)))
#define   PMC_PCK_CSS_MD_SLOW_CLK (0x0u << 0) /**< \brief (PMC_PCK[3]) MD_SLCK is selected */
#define   PMC_PCK_CSS_TD_SLOW_CLOCK (0x1u << 0) /**< \brief (PMC_PCK[3]) TD_SLCK is selected */
#define   PMC_PCK_CSS_MAINCK (0x2u << 0) /**< \brief (PMC_PCK[3]) MAINCK is selected */
#define   PMC_PCK_CSS_MCK0 (0x3u << 0) /**< \brief (PMC_PCK[3]) MCK0 is selected */
#define   PMC_PCK_CSS_PLLACK1 (0x4u << 0) /**< \brief (PMC_PCK[3]) PLLACK1 is selected. */
#define   PMC_PCK_CSS_PLLBCK (0x5u << 0) /**< \brief (PMC_PCK[3]) PLLBCK is selected. */
#define   PMC_PCK_CSS_PLLCCK (0x6u << 0) /**< \brief (PMC_PCK[3]) PLLCCK is selected. */
#define   PMC_PCK_CSS_PLLCSRC (0x7u << 0) /**< \brief (PMC_PCK[3]) PLLCSRC is selected. */
#define PMC_PCK_PRES_Pos 8
#define PMC_PCK_PRES_Msk (0xffu << PMC_PCK_PRES_Pos) /**< \brief (PMC_PCK[3]) Programmable Clock Prescaler */
#define PMC_PCK_PRES(value) ((PMC_PCK_PRES_Msk & ((value) << PMC_PCK_PRES_Pos)))
/* -------- PMC_IER : (PMC Offset: 0x64) Interrupt Enable Register -------- */
#define PMC_IER_MOSCXTS (0x1u << 0) /**< \brief (PMC_IER) Main Crystal Oscillator Status Interrupt Enable */
#define PMC_IER_MCKRDY (0x1u << 3) /**< \brief (PMC_IER) Master Clock 0 Ready Interrupt Enable */
#define PMC_IER_CPMCKRDY (0x1u << 4) /**< \brief (PMC_IER) Coprocessor Master Clock Ready Interrupt Enable */
#define PMC_IER_PCKRDY0 (0x1u << 8) /**< \brief (PMC_IER) Programmable Clock Ready 0 Interrupt Enable */
#define PMC_IER_PCKRDY1 (0x1u << 9) /**< \brief (PMC_IER) Programmable Clock Ready 1 Interrupt Enable */
#define PMC_IER_PCKRDY2 (0x1u << 10) /**< \brief (PMC_IER) Programmable Clock Ready 2 Interrupt Enable */
#define PMC_IER_PCKRDY3 (0x1u << 11) /**< \brief (PMC_IER) Programmable Clock Ready 3 Interrupt Enable */
#define PMC_IER_MOSCSELS (0x1u << 16) /**< \brief (PMC_IER) Main Clock Source Oscillator Selection Status Interrupt Enable */
#define PMC_IER_MOSCRCS (0x1u << 17) /**< \brief (PMC_IER) Main RC Oscillator Status Interrupt Enable */
#define PMC_IER_CFDEV (0x1u << 18) /**< \brief (PMC_IER) Clock Failure Detector Event Interrupt Enable */
#define PMC_IER_XT32KERR (0x1u << 21) /**< \brief (PMC_IER) 32.768 kHz Crystal Oscillator Error Interrupt Enable */
#define PMC_IER_MCKMON (0x1u << 23) /**< \brief (PMC_IER) Master Clock 0 Clock Monitor Interrupt Enable */
/* -------- PMC_IDR : (PMC Offset: 0x68) Interrupt Disable Register -------- */
#define PMC_IDR_MOSCXTS (0x1u << 0) /**< \brief (PMC_IDR) Main Crystal Oscillator Status Interrupt Disable */
#define PMC_IDR_MCKRDY (0x1u << 3) /**< \brief (PMC_IDR) Master Clock 0 Ready Interrupt Disable */
#define PMC_IDR_CPMCKRDY (0x1u << 4) /**< \brief (PMC_IDR) Coprocessor Master Clock Ready Interrupt Disable */
#define PMC_IDR_PCKRDY0 (0x1u << 8) /**< \brief (PMC_IDR) Programmable Clock Ready 0 Interrupt DIsable */
#define PMC_IDR_PCKRDY1 (0x1u << 9) /**< \brief (PMC_IDR) Programmable Clock Ready 1 Interrupt DIsable */
#define PMC_IDR_PCKRDY2 (0x1u << 10) /**< \brief (PMC_IDR) Programmable Clock Ready 2 Interrupt DIsable */
#define PMC_IDR_PCKRDY3 (0x1u << 11) /**< \brief (PMC_IDR) Programmable Clock Ready 3 Interrupt DIsable */
#define PMC_IDR_MOSCSELS (0x1u << 16) /**< \brief (PMC_IDR) Main Clock Source Oscillator Selection Status Interrupt Disable */
#define PMC_IDR_MOSCRCS (0x1u << 17) /**< \brief (PMC_IDR) Main RC Status Interrupt Disable */
#define PMC_IDR_CFDEV (0x1u << 18) /**< \brief (PMC_IDR) Clock Failure Detector Event Interrupt Disable */
#define PMC_IDR_XT32KERR (0x1u << 21) /**< \brief (PMC_IDR) 32.768 kHz Crystal Oscillator Error Interrupt Disable */
#define PMC_IDR_MCKMON (0x1u << 23) /**< \brief (PMC_IDR) Master Clock 0 Clock Monitor Interrupt Disable */
/* -------- PMC_SR : (PMC Offset: 0x6C) Status Register -------- */
#define PMC_SR_MOSCXTS (0x1u << 0) /**< \brief (PMC_SR) Main Crystal Oscillator Status */
#define PMC_SR_MCKRDY (0x1u << 3) /**< \brief (PMC_SR) Master Clock 0 Status */
#define PMC_SR_CPMCKRDY (0x1u << 4) /**< \brief (PMC_SR) Coprocessor Master Clock Status */
#define PMC_SR_OSCSELS (0x1u << 7) /**< \brief (PMC_SR) Monitoring Domain Slow Clock Source Oscillator Selection */
#define PMC_SR_PCKRDY0 (0x1u << 8) /**< \brief (PMC_SR) Programmable Clock Ready Status */
#define PMC_SR_PCKRDY1 (0x1u << 9) /**< \brief (PMC_SR) Programmable Clock Ready Status */
#define PMC_SR_PCKRDY2 (0x1u << 10) /**< \brief (PMC_SR) Programmable Clock Ready Status */
#define PMC_SR_PCKRDY3 (0x1u << 11) /**< \brief (PMC_SR) Programmable Clock Ready Status */
#define PMC_SR_MOSCSELS (0x1u << 16) /**< \brief (PMC_SR) Main Clock Source Oscillator Selection Status */
#define PMC_SR_MOSCRCS (0x1u << 17) /**< \brief (PMC_SR) Main RC Oscillator Status */
#define PMC_SR_CFDEV (0x1u << 18) /**< \brief (PMC_SR) Clock Failure Detector Event */
#define PMC_SR_CFDS (0x1u << 19) /**< \brief (PMC_SR) Clock Failure Detector Status */
#define PMC_SR_FOS (0x1u << 20) /**< \brief (PMC_SR) Clock Failure Detector Fault Output Status */
#define PMC_SR_XT32KERR (0x1u << 21) /**< \brief (PMC_SR) Slow Crystal Oscillator Error */
#define PMC_SR_MCKMON (0x1u << 23) /**< \brief (PMC_SR) Master Clock 0 Clock Monitor Error */
#define PMC_SR_GCLKRDY (0x1u << 24) /**< \brief (PMC_SR) GCLK Ready */
#define PMC_SR_PLL_INT (0x1u << 25) /**< \brief (PMC_SR) PLL Interrupt Status */
/* -------- PMC_IMR : (PMC Offset: 0x70) Interrupt Mask Register -------- */
#define PMC_IMR_MOSCXTS (0x1u << 0) /**< \brief (PMC_IMR) Main Crystal Oscillator Status Interrupt Mask */
#define PMC_IMR_MCKRDY (0x1u << 3) /**< \brief (PMC_IMR) Master Clock 0 Ready Interrupt Mask */
#define PMC_IMR_CPMCKRDY (0x1u << 4) /**< \brief (PMC_IMR) Coprocessor Master Clock Ready Interrupt Mask */
#define PMC_IMR_PCKRDY0 (0x1u << 8) /**< \brief (PMC_IMR) Programmable Clock Ready 0 Interrupt Mask */
#define PMC_IMR_PCKRDY1 (0x1u << 9) /**< \brief (PMC_IMR) Programmable Clock Ready 1 Interrupt Mask */
#define PMC_IMR_PCKRDY2 (0x1u << 10) /**< \brief (PMC_IMR) Programmable Clock Ready 2 Interrupt Mask */
#define PMC_IMR_PCKRDY3 (0x1u << 11) /**< \brief (PMC_IMR) Programmable Clock Ready 3 Interrupt Mask */
#define PMC_IMR_MOSCSELS (0x1u << 16) /**< \brief (PMC_IMR) Main Clock Source Oscillator Selection Status Interrupt Mask */
#define PMC_IMR_MOSCRCS (0x1u << 17) /**< \brief (PMC_IMR) Main RC Status Interrupt Mask */
#define PMC_IMR_CFDEV (0x1u << 18) /**< \brief (PMC_IMR) Clock Failure Detector Event Interrupt Mask */
#define PMC_IMR_XT32KERR (0x1u << 21) /**< \brief (PMC_IMR) 32.768 kHz Crystal Oscillator Error Interrupt Mask */
#define PMC_IMR_MCKMON (0x1u << 23) /**< \brief (PMC_IMR) Master Clock 0 Monitor Error Interrupt Mask */
/* -------- PMC_FSMR : (PMC Offset: 0x74) Fast Start-up Mode Register -------- */
#define PMC_FSMR_RTTAL (0x1u << 16) /**< \brief (PMC_FSMR) RTT Alarm Enable */
#define PMC_FSMR_RTCAL (0x1u << 17) /**< \brief (PMC_FSMR) RTC Alarm Enable */
#define PMC_FSMR_XTFA (0x1u << 18) /**< \brief (PMC_FSMR) 32KHz Crystal Failure Alarm Enable */
#define PMC_FSMR_SMAL (0x1u << 19) /**< \brief (PMC_FSMR) Supply Monitor Alarm Enable */
#define PMC_FSMR_LPM (0x1u << 20) /**< \brief (PMC_FSMR) Low Power Mode */
#define PMC_FSMR_FLPM_Pos 21
#define PMC_FSMR_FLPM_Msk (0x3u << PMC_FSMR_FLPM_Pos) /**< \brief (PMC_FSMR) Flash Low-power Mode */
#define PMC_FSMR_FLPM(value) ((PMC_FSMR_FLPM_Msk & ((value) << PMC_FSMR_FLPM_Pos)))
#define   PMC_FSMR_FLPM_FLASH_STANDBY (0x0u << 21) /**< \brief (PMC_FSMR) Flash is in Standby Mode when system enters Wait Mode */
#define   PMC_FSMR_FLPM_FLASH_DEEP_POWERDOWN (0x1u << 21) /**< \brief (PMC_FSMR) Flash is in Deep-powerdown mode when system enters Wait Mode */
#define   PMC_FSMR_FLPM_FLASH_IDLE (0x2u << 21) /**< \brief (PMC_FSMR) Idle mode */
#define PMC_FSMR_FFLPM (0x1u << 23) /**< \brief (PMC_FSMR) Force Flash Low-power Mode */
/* -------- PMC_WCR : (PMC Offset: 0x78) Wake-up Control Register -------- */
#define PMC_WCR_WKPIONB_Pos 0
#define PMC_WCR_WKPIONB_Msk (0xfu << PMC_WCR_WKPIONB_Pos) /**< \brief (PMC_WCR) Wake-up Input Number */
#define PMC_WCR_WKPIONB(value) ((PMC_WCR_WKPIONB_Msk & ((value) << PMC_WCR_WKPIONB_Pos)))
#define PMC_WCR_EN (0x1u << 16) /**< \brief (PMC_WCR) Wake-up Input Enable */
#define PMC_WCR_WIP (0x1u << 17) /**< \brief (PMC_WCR) Wake-up Input Polarity */
#define PMC_WCR_WIEN1 (0x1u << 20) /**< \brief (PMC_WCR) Wake-up Input Enable 1 */
#define PMC_WCR_CMD (0x1u << 24) /**< \brief (PMC_WCR) Command */
/* -------- PMC_FOCR : (PMC Offset: 0x7C) Fault Output Clear Register -------- */
#define PMC_FOCR_FOCLR (0x1u << 0) /**< \brief (PMC_FOCR) Fault Output Clear */
/* -------- PMC_CPFSMR : (PMC Offset: 0x80) Coprocessor Fast Start-up Mode Register -------- */
#define PMC_CPFSMR_RTTAL (0x1u << 16) /**< \brief (PMC_CPFSMR) RTT Alarm Enable */
#define PMC_CPFSMR_RTCAL (0x1u << 17) /**< \brief (PMC_CPFSMR) RTC Alarm Enable */
#define PMC_CPFSMR_XTFA (0x1u << 18) /**< \brief (PMC_CPFSMR) 32KHz Crystal Failure Alarm Enable */
#define PMC_CPFSMR_SMAL (0x1u << 19) /**< \brief (PMC_CPFSMR) Supply Monitor Alarm Enable */
/* -------- PMC_WPMR : (PMC Offset: 0x84) Write Protection Mode Register -------- */
#define PMC_WPMR_WPEN (0x1u << 0) /**< \brief (PMC_WPMR) Write Protection Enable */
#define PMC_WPMR_WPITEN (0x1u << 1) /**< \brief (PMC_WPMR) Write Protection Interrupt Enable */
#define PMC_WPMR_WPKEY_Pos 8
#define PMC_WPMR_WPKEY_Msk (0xffffffu << PMC_WPMR_WPKEY_Pos) /**< \brief (PMC_WPMR) Write Protection Key */
#define PMC_WPMR_WPKEY(value) ((PMC_WPMR_WPKEY_Msk & ((value) << PMC_WPMR_WPKEY_Pos)))
#define   PMC_WPMR_WPKEY_PASSWD (0x504D43u << 8) /**< \brief (PMC_WPMR) Writing any other value in this field aborts the write operation of the WPEN bit. Always reads as 0. */
/* -------- PMC_WPSR : (PMC Offset: 0x88) Write Protection Status Register -------- */
#define PMC_WPSR_WPVS (0x1u << 0) /**< \brief (PMC_WPSR) Write Protection Violation Status */
#define PMC_WPSR_WPVSRC_Pos 8
#define PMC_WPSR_WPVSRC_Msk (0xffffu << PMC_WPSR_WPVSRC_Pos) /**< \brief (PMC_WPSR) Write Protection Violation Source */
/* -------- PMC_PCR : (PMC Offset: 0x8C) Peripheral Control Register -------- */
#define PMC_PCR_PID_Pos 0
#define PMC_PCR_PID_Msk (0x7fu << PMC_PCR_PID_Pos) /**< \brief (PMC_PCR) Peripheral ID */
#define PMC_PCR_PID(value) ((PMC_PCR_PID_Msk & ((value) << PMC_PCR_PID_Pos)))
#define PMC_PCR_GCLKCSS_Pos 8
#define PMC_PCR_GCLKCSS_Msk (0x1fu << PMC_PCR_GCLKCSS_Pos) /**< \brief (PMC_PCR) Generic Clock Source Selection */
#define PMC_PCR_GCLKCSS(value) ((PMC_PCR_GCLKCSS_Msk & ((value) << PMC_PCR_GCLKCSS_Pos)))
#define   PMC_PCR_GCLKCSS_MD_SLOW_CLK (0x0u << 8) /**< \brief (PMC_PCR) MD_SLCK is selected */
#define   PMC_PCR_GCLKCSS_TD_SLOW_CLOCK (0x1u << 8) /**< \brief (PMC_PCR) TD_SLCK is selected */
#define   PMC_PCR_GCLKCSS_MAINCK (0x2u << 8) /**< \brief (PMC_PCR) MAINCK is selected */
#define   PMC_PCR_GCLKCSS_MCK0 (0x3u << 8) /**< \brief (PMC_PCR) MCK0 is selected */
#define   PMC_PCR_GCLKCSS_PLLACK1 (0x4u << 8) /**< \brief (PMC_PCR) PLLACK1 is selected. */
#define   PMC_PCR_GCLKCSS_PLLBCK (0x5u << 8) /**< \brief (PMC_PCR) PLLBCK is selected. */
#define   PMC_PCR_GCLKCSS_PLLCCK (0x6u << 8) /**< \brief (PMC_PCR) PLLCCK is selected. */
#define   PMC_PCR_GCLKCSS_PLLCSRC (0x7u << 8) /**< \brief (PMC_PCR) PLLCSRC is selected. */
#define PMC_PCR_DIV_Pos 14
#define PMC_PCR_DIV_Msk (0x3u << PMC_PCR_DIV_Pos) /**< \brief (PMC_PCR) Peripheral Clock Division Ratio */
#define PMC_PCR_DIV(value) ((PMC_PCR_DIV_Msk & ((value) << PMC_PCR_DIV_Pos)))
#define   PMC_PCR_DIV_DIV1 (0x0u << 14) /**< \brief (PMC_PCR) Associated GCLKx is divided by 1. */
#define   PMC_PCR_DIV_DIV2 (0x1u << 14) /**< \brief (PMC_PCR) Associated GCLKx is divided by 2. */
#define   PMC_PCR_DIV_DIV4 (0x2u << 14) /**< \brief (PMC_PCR) Associated GCLKx is divided by 4. */
#define   PMC_PCR_DIV_DIV8 (0x3u << 14) /**< \brief (PMC_PCR) Associated GCLKx is divided by 8. */
#define PMC_PCR_MCKID_Pos 16
#define PMC_PCR_MCKID_Msk (0xfu << PMC_PCR_MCKID_Pos) /**< \brief (PMC_PCR) Master Clock Index (Read-only) */
#define PMC_PCR_MCKID(value) ((PMC_PCR_MCKID_Msk & ((value) << PMC_PCR_MCKID_Pos)))
#define PMC_PCR_GCLKDIV_Pos 20
#define PMC_PCR_GCLKDIV_Msk (0xffu << PMC_PCR_GCLKDIV_Pos) /**< \brief (PMC_PCR) Generic Clock Division Ratio */
#define PMC_PCR_GCLKDIV(value) ((PMC_PCR_GCLKDIV_Msk & ((value) << PMC_PCR_GCLKDIV_Pos)))
#define PMC_PCR_EN (0x1u << 28) /**< \brief (PMC_PCR) Enable */
#define PMC_PCR_GCLKEN (0x1u << 29) /**< \brief (PMC_PCR) Generic Clock Enable */
#define PMC_PCR_CMD (0x1u << 31) /**< \brief (PMC_PCR) Command */
/* -------- PMC_OCR : (PMC Offset: 0x90) Oscillator Calibration Register -------- */
#define PMC_OCR_CAL12_Pos 16
#define PMC_OCR_CAL12_Msk (0x7fu << PMC_OCR_CAL12_Pos) /**< \brief (PMC_OCR) Main RC Oscillator Calibration Bits */
#define PMC_OCR_CAL12(value) ((PMC_OCR_CAL12_Msk & ((value) << PMC_OCR_CAL12_Pos)))
#define PMC_OCR_SEL12 (0x1u << 23) /**< \brief (PMC_OCR) Selection of Main RC Oscillator Calibration Bits */
/* -------- PMC_SLPWK_AIPR : (PMC Offset: 0x94) SleepWalking Activity In Progress Register -------- */
#define PMC_SLPWK_AIPR_AIP (0x1u << 0) /**< \brief (PMC_SLPWK_AIPR) Activity In Progress */
/* -------- PMC_SLPWKCR : (PMC Offset: 0x98) SleepWalking Control Register -------- */
#define PMC_SLPWKCR_PID_Pos 0
#define PMC_SLPWKCR_PID_Msk (0x7fu << PMC_SLPWKCR_PID_Pos) /**< \brief (PMC_SLPWKCR) Peripheral ID */
#define PMC_SLPWKCR_PID(value) ((PMC_SLPWKCR_PID_Msk & ((value) << PMC_SLPWKCR_PID_Pos)))
#define PMC_SLPWKCR_CMD (0x1u << 12) /**< \brief (PMC_SLPWKCR) Command */
#define PMC_SLPWKCR_ASR (0x1u << 16) /**< \brief (PMC_SLPWKCR) Activity Status Register */
#define PMC_SLPWKCR_SLPWKSR (0x1u << 28) /**< \brief (PMC_SLPWKCR) SleepWalking Sleep Register */
/* -------- PMC_MCKLIM : (PMC Offset: 0xA0) MCK0 Monitor Limits Register -------- */
#define PMC_MCKLIM_MCK_LOW_IT_Pos 0
#define PMC_MCKLIM_MCK_LOW_IT_Msk (0xffu << PMC_MCKLIM_MCK_LOW_IT_Pos) /**< \brief (PMC_MCKLIM) MCK0 Monitoring Low IT Limit */
#define PMC_MCKLIM_MCK_LOW_IT(value) ((PMC_MCKLIM_MCK_LOW_IT_Msk & ((value) << PMC_MCKLIM_MCK_LOW_IT_Pos)))
#define PMC_MCKLIM_MCK_HIGH_IT_Pos 8
#define PMC_MCKLIM_MCK_HIGH_IT_Msk (0xffu << PMC_MCKLIM_MCK_HIGH_IT_Pos) /**< \brief (PMC_MCKLIM) MCK0 Monitoring High IT Limit */
#define PMC_MCKLIM_MCK_HIGH_IT(value) ((PMC_MCKLIM_MCK_HIGH_IT_Msk & ((value) << PMC_MCKLIM_MCK_HIGH_IT_Pos)))
#define PMC_MCKLIM_MCK_LOW_RES_Pos 16
#define PMC_MCKLIM_MCK_LOW_RES_Msk (0xffu << PMC_MCKLIM_MCK_LOW_RES_Pos) /**< \brief (PMC_MCKLIM) MCK0 Monitoring Low RESET Limit */
#define PMC_MCKLIM_MCK_LOW_RES(value) ((PMC_MCKLIM_MCK_LOW_RES_Msk & ((value) << PMC_MCKLIM_MCK_LOW_RES_Pos)))
#define PMC_MCKLIM_MCK_HIGH_RES_Pos 24
#define PMC_MCKLIM_MCK_HIGH_RES_Msk (0xffu << PMC_MCKLIM_MCK_HIGH_RES_Pos) /**< \brief (PMC_MCKLIM) MCK0 Monitoring High Reset Limit */
#define PMC_MCKLIM_MCK_HIGH_RES(value) ((PMC_MCKLIM_MCK_HIGH_RES_Msk & ((value) << PMC_MCKLIM_MCK_HIGH_RES_Pos)))
/* -------- PMC_CSR0 : (PMC Offset: 0xA4) Peripheral Clock Status Register 0 -------- */
#define PMC_CSR0_PID9 (0x1u << 9) /**< \brief (PMC_CSR0) Peripheral Clock 9 Status */
#define PMC_CSR0_PID10 (0x1u << 10) /**< \brief (PMC_CSR0) Peripheral Clock 10 Status */
#define PMC_CSR0_PID11 (0x1u << 11) /**< \brief (PMC_CSR0) Peripheral Clock 11 Status */
#define PMC_CSR0_PID12 (0x1u << 12) /**< \brief (PMC_CSR0) Peripheral Clock 12 Status */
#define PMC_CSR0_PID13 (0x1u << 13) /**< \brief (PMC_CSR0) Peripheral Clock 13 Status */
#define PMC_CSR0_PID14 (0x1u << 14) /**< \brief (PMC_CSR0) Peripheral Clock 14 Status */
#define PMC_CSR0_PID15 (0x1u << 15) /**< \brief (PMC_CSR0) Peripheral Clock 15 Status */
#define PMC_CSR0_PID16 (0x1u << 16) /**< \brief (PMC_CSR0) Peripheral Clock 16 Status */
#define PMC_CSR0_PID17 (0x1u << 17) /**< \brief (PMC_CSR0) Peripheral Clock 17 Status */
#define PMC_CSR0_PID23 (0x1u << 23) /**< \brief (PMC_CSR0) Peripheral Clock 23 Status */
#define PMC_CSR0_PID24 (0x1u << 24) /**< \brief (PMC_CSR0) Peripheral Clock 24 Status */
#define PMC_CSR0_PID25 (0x1u << 25) /**< \brief (PMC_CSR0) Peripheral Clock 25 Status */
#define PMC_CSR0_PID28 (0x1u << 28) /**< \brief (PMC_CSR0) Peripheral Clock 28 Status */
#define PMC_CSR0_PID30 (0x1u << 30) /**< \brief (PMC_CSR0) Peripheral Clock 30 Status */
#define PMC_CSR0_PID31 (0x1u << 31) /**< \brief (PMC_CSR0) Peripheral Clock 31 Status */
/* -------- PMC_CSR1 : (PMC Offset: 0xA8) Peripheral Clock Status Register 1 -------- */
#define PMC_CSR1_PID32 (0x1u << 0) /**< \brief (PMC_CSR1) Peripheral Clock 32 Status */
#define PMC_CSR1_PID33 (0x1u << 1) /**< \brief (PMC_CSR1) Peripheral Clock 33 Status */
#define PMC_CSR1_PID34 (0x1u << 2) /**< \brief (PMC_CSR1) Peripheral Clock 34 Status */
#define PMC_CSR1_PID35 (0x1u << 3) /**< \brief (PMC_CSR1) Peripheral Clock 35 Status */
#define PMC_CSR1_PID36 (0x1u << 4) /**< \brief (PMC_CSR1) Peripheral Clock 36 Status */
#define PMC_CSR1_PID37 (0x1u << 5) /**< \brief (PMC_CSR1) Peripheral Clock 37 Status */
#define PMC_CSR1_PID38 (0x1u << 6) /**< \brief (PMC_CSR1) Peripheral Clock 38 Status */
#define PMC_CSR1_PID39 (0x1u << 7) /**< \brief (PMC_CSR1) Peripheral Clock 39 Status */
#define PMC_CSR1_PID49 (0x1u << 17) /**< \brief (PMC_CSR1) Peripheral Clock 49 Status */
#define PMC_CSR1_PID51 (0x1u << 19) /**< \brief (PMC_CSR1) Peripheral Clock 51 Status */
#define PMC_CSR1_PID53 (0x1u << 21) /**< \brief (PMC_CSR1) Peripheral Clock 53 Status */
#define PMC_CSR1_PID55 (0x1u << 23) /**< \brief (PMC_CSR1) Peripheral Clock 55 Status */
#define PMC_CSR1_PID57 (0x1u << 25) /**< \brief (PMC_CSR1) Peripheral Clock 57 Status */
#define PMC_CSR1_PID59 (0x1u << 27) /**< \brief (PMC_CSR1) Peripheral Clock 59 Status */
/* -------- PMC_CSR2 : (PMC Offset: 0xAC) Peripheral Clock Status Register 2 -------- */
#define PMC_CSR2_PID75 (0x1u << 11) /**< \brief (PMC_CSR2) Peripheral Clock 75 Status */
#define PMC_CSR2_PID78 (0x1u << 14) /**< \brief (PMC_CSR2) Peripheral Clock 78 Status */
#define PMC_CSR2_PID79 (0x1u << 15) /**< \brief (PMC_CSR2) Peripheral Clock 79 Status */
#define PMC_CSR2_PID80 (0x1u << 16) /**< \brief (PMC_CSR2) Peripheral Clock 80 Status */
#define PMC_CSR2_PID81 (0x1u << 17) /**< \brief (PMC_CSR2) Peripheral Clock 81 Status */
#define PMC_CSR2_PID85 (0x1u << 21) /**< \brief (PMC_CSR2) Peripheral Clock 85 Status */
#define PMC_CSR2_PID87 (0x1u << 23) /**< \brief (PMC_CSR2) Peripheral Clock 87 Status */
#define PMC_CSR2_PID88 (0x1u << 24) /**< \brief (PMC_CSR2) Peripheral Clock 88 Status */
#define PMC_CSR2_PID89 (0x1u << 25) /**< \brief (PMC_CSR2) Peripheral Clock 89 Status */
#define PMC_CSR2_PID90 (0x1u << 26) /**< \brief (PMC_CSR2) Peripheral Clock 90 Status */
#define PMC_CSR2_PID91 (0x1u << 27) /**< \brief (PMC_CSR2) Peripheral Clock 91 Status */
/* -------- PMC_CSR3 : (PMC Offset: 0xB0) Peripheral Clock Status Register 3 -------- */
#define PMC_CSR3_PID96 (0x1u << 0) /**< \brief (PMC_CSR3) Peripheral Clock 96 Status */
/* -------- PMC_GCSR0 : (PMC Offset: 0xC4) Generic Clock Status Register 0 -------- */
#define PMC_GCSR0_GPID9 (0x1u << 9) /**< \brief (PMC_GCSR0) Generic Clock 9 Status */
#define PMC_GCSR0_GPID10 (0x1u << 10) /**< \brief (PMC_GCSR0) Generic Clock 10 Status */
#define PMC_GCSR0_GPID11 (0x1u << 11) /**< \brief (PMC_GCSR0) Generic Clock 11 Status */
#define PMC_GCSR0_GPID12 (0x1u << 12) /**< \brief (PMC_GCSR0) Generic Clock 12 Status */
#define PMC_GCSR0_GPID13 (0x1u << 13) /**< \brief (PMC_GCSR0) Generic Clock 13 Status */
#define PMC_GCSR0_GPID14 (0x1u << 14) /**< \brief (PMC_GCSR0) Generic Clock 14 Status */
#define PMC_GCSR0_GPID15 (0x1u << 15) /**< \brief (PMC_GCSR0) Generic Clock 15 Status */
#define PMC_GCSR0_GPID16 (0x1u << 16) /**< \brief (PMC_GCSR0) Generic Clock 16 Status */
#define PMC_GCSR0_GPID23 (0x1u << 23) /**< \brief (PMC_GCSR0) Generic Clock 23 Status */
#define PMC_GCSR0_GPID24 (0x1u << 24) /**< \brief (PMC_GCSR0) Generic Clock 24 Status */
#define PMC_GCSR0_GPID31 (0x1u << 31) /**< \brief (PMC_GCSR0) Generic Clock 31 Status */
/* -------- PMC_GCSR1 : (PMC Offset: 0xC8) Generic Clock Status Register 1 -------- */
#define PMC_GCSR1_GPID34 (0x1u << 2) /**< \brief (PMC_GCSR1) Generic Clock 34 Status */
#define PMC_GCSR1_GPID37 (0x1u << 5) /**< \brief (PMC_GCSR1) Generic Clock 37 Status */
/* -------- PMC_GCSR2 : (PMC Offset: 0xCC) Generic Clock Status Register 2 -------- */
#define PMC_GCSR2_GPID75 (0x1u << 11) /**< \brief (PMC_GCSR2) Generic Clock 75 Status */
#define PMC_GCSR2_GPID79 (0x1u << 15) /**< \brief (PMC_GCSR2) Generic Clock 79 Status */
#define PMC_GCSR2_GPID87 (0x1u << 23) /**< \brief (PMC_GCSR2) Generic Clock 87 Status */
#define PMC_GCSR2_GPID89 (0x1u << 25) /**< \brief (PMC_GCSR2) Generic Clock 89 Status */
#define PMC_GCSR2_GPID90 (0x1u << 26) /**< \brief (PMC_GCSR2) Generic Clock 90 Status */
/* -------- PMC_GCSR3 : (PMC Offset: 0xD0) Generic Clock Status Register 3 -------- */
#define PMC_GCSR3_GPID96 (0x1u << 0) /**< \brief (PMC_GCSR3) Generic Clock 96 Status */
/* -------- PMC_PLL_IER : (PMC Offset: 0xE4) PLL Interrupt Enable Register -------- */
#define PMC_PLL_IER_LOCK0 (0x1u << 0) /**< \brief (PMC_PLL_IER) PLL of Index 0 Lock Interrupt Enable */
#define PMC_PLL_IER_LOCK1 (0x1u << 1) /**< \brief (PMC_PLL_IER) PLL of Index 1 Lock Interrupt Enable */
#define PMC_PLL_IER_LOCK2 (0x1u << 2) /**< \brief (PMC_PLL_IER) PLL of Index 2 Lock Interrupt Enable */
#define PMC_PLL_IER_UNLOCK0 (0x1u << 16) /**< \brief (PMC_PLL_IER) PLL of Index 0 Unlock Interrupt Enable */
#define PMC_PLL_IER_UNLOCK1 (0x1u << 17) /**< \brief (PMC_PLL_IER) PLL of Index 1 Unlock Interrupt Enable */
#define PMC_PLL_IER_UNLOCK2 (0x1u << 18) /**< \brief (PMC_PLL_IER) PLL of Index 2 Unlock Interrupt Enable */
/* -------- PMC_PLL_IDR : (PMC Offset: 0xE8) PLL Interrupt Disable Register -------- */
#define PMC_PLL_IDR_LOCK0 (0x1u << 0) /**< \brief (PMC_PLL_IDR) PLL of Index 0 Lock Interrupt Disable */
#define PMC_PLL_IDR_LOCK1 (0x1u << 1) /**< \brief (PMC_PLL_IDR) PLL of Index 1 Lock Interrupt Disable */
#define PMC_PLL_IDR_LOCK2 (0x1u << 2) /**< \brief (PMC_PLL_IDR) PLL of Index 2 Lock Interrupt Disable */
#define PMC_PLL_IDR_UNLOCK0 (0x1u << 16) /**< \brief (PMC_PLL_IDR) PLL of Index 0 Unlock Interrupt Disable */
#define PMC_PLL_IDR_UNLOCK1 (0x1u << 17) /**< \brief (PMC_PLL_IDR) PLL of Index 1 Unlock Interrupt Disable */
#define PMC_PLL_IDR_UNLOCK2 (0x1u << 18) /**< \brief (PMC_PLL_IDR) PLL of Index 2 Unlock Interrupt Disable */
/* -------- PMC_PLL_IMR : (PMC Offset: 0xEC) PLL Interrupt Mask Register -------- */
#define PMC_PLL_IMR_LOCK0 (0x1u << 0) /**< \brief (PMC_PLL_IMR) PLL of Index 0 Lock Interrupt Mask */
#define PMC_PLL_IMR_LOCK1 (0x1u << 1) /**< \brief (PMC_PLL_IMR) PLL of Index 1 Lock Interrupt Mask */
#define PMC_PLL_IMR_LOCK2 (0x1u << 2) /**< \brief (PMC_PLL_IMR) PLL of Index 2 Lock Interrupt Mask */
#define PMC_PLL_IMR_UNLOCK0 (0x1u << 16) /**< \brief (PMC_PLL_IMR) PLL of Index 0 Unlock Interrupt Mask */
#define PMC_PLL_IMR_UNLOCK1 (0x1u << 17) /**< \brief (PMC_PLL_IMR) PLL of Index 1 Unlock Interrupt Mask */
#define PMC_PLL_IMR_UNLOCK2 (0x1u << 18) /**< \brief (PMC_PLL_IMR) PLL of Index 2 Unlock Interrupt Mask */
/* -------- PMC_PLL_ISR0 : (PMC Offset: 0xF0) PLL Interrupt Status Register 0 -------- */
#define PMC_PLL_ISR0_LOCK0 (0x1u << 0) /**< \brief (PMC_PLL_ISR0) PLL of Index 0 Lock Interrupt Status */
#define PMC_PLL_ISR0_LOCK1 (0x1u << 1) /**< \brief (PMC_PLL_ISR0) PLL of Index 1 Lock Interrupt Status */
#define PMC_PLL_ISR0_LOCK2 (0x1u << 2) /**< \brief (PMC_PLL_ISR0) PLL of Index 2 Lock Interrupt Status */
#define PMC_PLL_ISR0_UNLOCK0 (0x1u << 16) /**< \brief (PMC_PLL_ISR0) PLL of Index 0 Unlock Interrupt Status */
#define PMC_PLL_ISR0_UNLOCK1 (0x1u << 17) /**< \brief (PMC_PLL_ISR0) PLL of Index 1 Unlock Interrupt Status */
#define PMC_PLL_ISR0_UNLOCK2 (0x1u << 18) /**< \brief (PMC_PLL_ISR0) PLL of Index 2 Unlock Interrupt Status */
/* -------- PMC_PLL_ISR1 : (PMC Offset: 0xF4) PLL Interrupt Status Register 1 -------- */
#define PMC_PLL_ISR1_UDR0 (0x1u << 0) /**< \brief (PMC_PLL_ISR1) PLLx Underflow */
#define PMC_PLL_ISR1_UDR1 (0x1u << 1) /**< \brief (PMC_PLL_ISR1) PLLx Underflow */
#define PMC_PLL_ISR1_UDR2 (0x1u << 2) /**< \brief (PMC_PLL_ISR1) PLLx Underflow */
#define PMC_PLL_ISR1_OVR0 (0x1u << 16) /**< \brief (PMC_PLL_ISR1) PLLx Overflow */
#define PMC_PLL_ISR1_OVR1 (0x1u << 17) /**< \brief (PMC_PLL_ISR1) PLLx Overflow */
#define PMC_PLL_ISR1_OVR2 (0x1u << 18) /**< \brief (PMC_PLL_ISR1) PLLx Overflow */

/*@}*/

#endif /* _PIC32CX_PMC_COMPONENT_ */
