/*
 * Copyright (c) 2015, Freescale Semiconductor, Inc.
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:
 *
 * o Redistributions of source code must retain the above copyright notice, this list
 *   of conditions and the following disclaimer.
 *
 * o Redistributions in binary form must reproduce the above copyright notice, this
 *   list of conditions and the following disclaimer in the documentation and/or
 *   other materials provided with the distribution.
 *
 * o Neither the name of Freescale Semiconductor, Inc. nor the names of its
 *   contributors may be used to endorse or promote products derived from this
 *   software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR
 * ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
 * ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

/*
 * How to setup clock using clock driver functions:
 *
 * 1. CLOCK_SetSimSafeDivs, to make sure core clock, bus clock, flexbus clock
 *    and flash clock are in allowed range during clock mode switch.
 *
 * 2. Call CLOCK_Osc0Init to setup OSC clock, if it is used in target mode.
 *
 * 3. Set MCG configuration, MCG includes three parts: FLL clock, PLL clock and
 *    internal reference clock(MCGIRCLK). Follow the steps to setup:
 *
 *    1). Call CLOCK_BootToXxxMode to set MCG to target mode.
 *
 *    2). If target mode is FBI/BLPI/PBI mode, the MCGIRCLK has been configured
 *        correctly. For other modes, need to call CLOCK_SetInternalRefClkConfig
 *        explicitly to setup MCGIRCLK.
 *
 *    3). Don't need to configure FLL explicitly, because if target mode is FLL
 *        mode, then FLL has been configured by the function CLOCK_BootToXxxMode,
 *        if the target mode is not FLL mode, the FLL is disabled.
 *
 *    4). If target mode is PEE/PBE/PEI/PBI mode, then the related PLL has been
 *        setup by CLOCK_BootToXxxMode. In FBE/FBI/FEE/FBE mode, the PLL could
 *        be enabled independently, call CLOCK_EnablePll0 explicitly in this case.
 *
 * 4. Call CLOCK_SetSimConfig to set the clock configuration in SIM.
 */

/* TEXT BELOW IS USED AS SETTING FOR THE CLOCKS TOOL *****************************
!!ClocksProfile
product: Clocks v1.0
processor: MK82FN256xxx15
package_id: MK82FN256VLL15
mcu_data: ksdk2_0
processor_version: 1.0.1
 * BE CAREFUL MODIFYING THIS COMMENT - IT IS YAML SETTINGS FOR THE CLOCKS TOOL **/

#include "fsl_smc.h"
#include "fsl_rtc.h"
#include "fsl_clock_config.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/
#define MCG_PLL_DISABLE                                   0U  /*!< MCGPLLCLK disabled */
#define OSC_CAP0P                                         0U  /*!< Oscillator 0pF capacitor load */
#define OSC_ER_CLK_DISABLE                                0U  /*!< Disable external reference clock */
#define RTC_OSC_CAP_LOAD_0PF                            0x0U  /*!< RTC oscillator capacity load: 0pF */
#define RTC_RTC32KCLK_PERIPHERALS_ENABLED                 1U  /*!< RTC32KCLK to other peripherals: enabled */
#define SIM_FLEXIO_CLK_SEL_CORE_SYSTEM_CLK                0U  /*!< FLEXIO clock select: Core/system clock */
#define SIM_FLEXIO_CLK_SEL_MCGIRCLK_CLK                   3U  /*!< FLEXIO clock select: MCGIRCLK clock */
#define SIM_LPUART_CLK_SEL_MCGIRCLK_CLK                   3U  /*!< LPUART clock select: MCGIRCLK clock */
#define SIM_LPUART_CLK_SEL_PLLFLLSEL_CLK                  1U  /*!< LPUART clock select: PLLFLLSEL output clock */
#define SIM_OSC32KSEL_RTC32KCLK_CLK                       2U  /*!< OSC32KSEL select: RTC32KCLK clock (32.768kHz) */
#define SIM_PLLFLLSEL_MCGFLLCLK_CLK                       0U  /*!< PLLFLL select: MCGFLLCLK clock */
#define SIM_PLLFLLSEL_MCGPLLCLK_CLK                       1U  /*!< PLLFLL select: MCGPLLCLK clock */

/*******************************************************************************
 * Variables
 ******************************************************************************/
/* System clock frequency. */
extern uint32_t SystemCoreClock;

/*******************************************************************************
 * Code
 ******************************************************************************/
/*FUNCTION**********************************************************************
 *
 * Function Name : CLOCK_CONFIG_SetRtcClock
 * Description   : This function is used to configuring RTC clock including
 * enabling RTC oscillator.
 * Param capLoad : RTC oscillator capacity load
 * Param enableOutPeriph : Enable (1U)/Disable (0U) clock to peripherals
 *
 *END**************************************************************************/
static void CLOCK_CONFIG_SetRtcClock(uint32_t capLoad, uint8_t enableOutPeriph)
{
  /* RTC clock gate enable */
  CLOCK_EnableClock(kCLOCK_Rtc0);
  if ((RTC->CR & RTC_CR_OSCE_MASK) == 0u) { /* Only if the Rtc oscillator is not already enabled */
    /* Set the specified capacitor configuration for the RTC oscillator */
    RTC_SetOscCapLoad(RTC, capLoad);
    /* Enable the RTC 32KHz oscillator */
    RTC->CR |= RTC_CR_OSCE_MASK;
  }
  /* Output to other peripherals */
  if (enableOutPeriph) {
    RTC->CR &= ~RTC_CR_CLKO_MASK;
  }
  else {
    RTC->CR |= RTC_CR_CLKO_MASK;
  }
  /* Set the XTAL32/RTC_CLKIN frequency based on board setting. */
  CLOCK_SetXtal32Freq(BOARD_XTAL32K_CLK_HZ);
  /* Set RTC_TSR if there is fault value in RTC */
  if (RTC->SR & RTC_SR_TIF_MASK) {
    RTC -> TSR = RTC -> TSR;
  }
  /* RTC clock gate disable */
  CLOCK_DisableClock(kCLOCK_Rtc0);
}

/*FUNCTION**********************************************************************
 *
 * Function Name : CLOCK_CONFIG_SetFllExtRefDiv
 * Description   : Configure FLL external reference divider (FRDIV).
 * Param frdiv   : The value to set FRDIV.
 *
 *END**************************************************************************/
static void CLOCK_CONFIG_SetFllExtRefDiv(uint8_t frdiv)
{
  MCG->C1 = ((MCG->C1 & ~MCG_C1_FRDIV_MASK) | MCG_C1_FRDIV(frdiv));
}

/*******************************************************************************
 ********************* Configuration BOARD_BootClockHSRUN **********************
 ******************************************************************************/
/* TEXT BELOW IS USED AS SETTING FOR THE CLOCKS TOOL *****************************
!!Configuration
name: BOARD_BootClockHSRUN
outputs:
- {id: Bus_clock.outFreq, value: 75 MHz}
- {id: Core_clock.outFreq, value: 150 MHz}
- {id: ERCLK32K.outFreq, value: 32.768 kHz}
- {id: FLEXIOCLK.outFreq, value: 150 MHz}
- {id: Flash_clock.outFreq, value: 25 MHz}
- {id: FlexBus_clock.outFreq, value: 75 MHz}
- {id: IRC48MCLK.outFreq, value: 48 MHz}
- {id: LPO_clock.outFreq, value: 1 kHz}
- {id: LPUARTCLK.outFreq, value: 75 MHz}
- {id: MCGFFCLK.outFreq, value: 1.5 MHz}
- {id: MCGIRCLK.outFreq, value: 2 MHz}
- {id: MCGPLLCLK.outFreq, value: 150 MHz}
- {id: MCGPLLCLK2X.outFreq, value: 300 MHz}
- {id: PLLFLLCLK.outFreq, value: 150 MHz}
- {id: System_clock.outFreq, value: 150 MHz}
settings:
- {id: MCGMode, value: PEE}
- {id: powerMode, value: HSRUN}
- {id: FLEXIOClkConfig, value: 'yes'}
- {id: LPUARTClkConfig, value: 'yes'}
- {id: MCG.FLL_mul.scale, value: '2929', locked: true}
- {id: MCG.FRDIV.scale, value: '32', locked: true}
- {id: MCG.IRCS.sel, value: MCG.FCRDIV}
- {id: MCG.IREFS.sel, value: MCG.FRDIV}
- {id: MCG.OSCSEL.sel, value: SIM.IRC48MCLK}
- {id: MCG.OSCSEL_PLL.sel, value: SIM.IRC48MCLK}
- {id: MCG.PLLS.sel, value: MCG.PLL_DIV2}
- {id: MCG.PRDIV.scale, value: '4', locked: true}
- {id: MCG.VDIV.scale, value: '25', locked: true}
- {id: MCG_C1_IRCLKEN_CFG, value: Enabled}
- {id: MCG_C2_RANGE0_CFG, value: Very_high}
- {id: MCG_C2_RANGE0_FRDIV_CFG, value: Very_high}
- {id: MCG_C5_PLLCLKEN0_CFG, value: Enabled}
- {id: RTC_CR_OSCE_CFG, value: Enabled}
- {id: SIM.LPUARTSRCSEL.sel, value: SIM.PLLFLLDIV}
- {id: SIM.OSC32KSEL.sel, value: RTC.RTC32KCLK}
- {id: SIM.OUTDIV2.scale, value: '2'}
- {id: SIM.OUTDIV4.scale, value: '6'}
- {id: SIM.PLLFLLDIV.scale, value: '2', locked: true}
- {id: SIM.PLLFLLSEL.sel, value: MCG.MCGPLLCLK}
- {id: SIM.RTCCLKOUTSEL.sel, value: RTC.RTC32KCLK}
- {id: SIM.USBDIV.scale, value: '1', locked: true}
- {id: SIM.USBFRAC.scale, value: '1', locked: true}
sources:
- {id: IRC48M.IRC48M.outFreq, value: 48 MHz}
- {id: RTC.RTC32kHz.outFreq, value: 32.768 kHz, enabled: true}
 * BE CAREFUL MODIFYING THIS COMMENT - IT IS YAML SETTINGS FOR THE CLOCKS TOOL **/

/*******************************************************************************
 * Variables for BOARD_BootClockHSRUN configuration
 ******************************************************************************/
const mcg_config_t mcgConfig_BOARD_BootClockHSRUN =
  {
    .mcgMode = kMCG_ModePEE,                  /* PEE - PLL Engaged External */
    .irclkEnableMode = kMCG_IrclkEnable,      /* MCGIRCLK enabled, MCGIRCLK disabled in STOP mode */
    .ircs = kMCG_IrcFast,                     /* Fast internal reference clock selected */
    .fcrdiv = 0x1U,                           /* Fast IRC divider: divided by 2 */
    .frdiv = 0x0U,                            /* FLL reference clock divider: divided by 32 */
    .drs = kMCG_DrsHigh,                      /* High frequency range */
    .dmx32 = kMCG_Dmx32Fine,                  /* DCO is fine-tuned for maximum frequency with 32.768 kHz reference */
    .oscsel = kMCG_OscselIrc,                 /* Selects 48 MHz IRC Oscillator */
    .pll0Config =
      {
        .enableMode = kMCG_PllEnableIndependent,/* MCGPLLCLK enabled independent of MCG clock mode, MCGPLLCLK disabled in STOP mode */
        .prdiv = 0x3U,                    /* PLL Reference divider: divided by 4 */
        .vdiv = 0x9U,                     /* VCO divider: multiplied by 25 */
      },
  };
const sim_clock_config_t simConfig_BOARD_BootClockHSRUN =
  {
    .pllFllSel = SIM_PLLFLLSEL_MCGPLLCLK_CLK, /* PLLFLL select: MCGPLLCLK clock */
    .pllFllDiv = 1,                           /* PLLFLLSEL clock divider divisor: divided by 2 */
    .pllFllFrac = 0,                          /* PLLFLLSEL clock divider fraction: multiplied by 1 */
    .er32kSrc = SIM_OSC32KSEL_RTC32KCLK_CLK,  /* OSC32KSEL select: RTC32KCLK clock (32.768kHz) */
    .clkdiv1 = 0x1150000U,                    /* SIM_CLKDIV1 - OUTDIV1: /1, OUTDIV2: /2, OUTDIV3: /2, OUTDIV4: /6 */
  };
const osc_config_t oscConfig_BOARD_BootClockHSRUN =
  {
    .freq = 0U,                               /* Oscillator frequency: 0Hz */
    .capLoad = (OSC_CAP0P),                   /* Oscillator capacity load: 0pF */
    .workMode = kOSC_ModeExt,                 /* Use external clock */
    .oscerConfig =
      {
        .enableMode = OSC_ER_CLK_DISABLE, /* Disable external reference clock */
        .erclkDiv = 0,                    /* Divider for OSCERCLK: divided by 1 */
      }
  };

/*******************************************************************************
 * Code for BOARD_BootClockHSRUN configuration
 ******************************************************************************/
void BOARD_BootClockHSRUN(void)
{
  /* Set HSRUN power mode */
  SMC_SetPowerModeProtection(SMC, kSMC_AllowPowerModeAll);
  SMC_SetPowerModeHsrun(SMC);
  while (SMC_GetPowerModeState(SMC) != kSMC_PowerStateHsrun)
  {
  }
  /* Set the system clock dividers in SIM to safe value. */
  CLOCK_SetSimSafeDivs();
  /* Configure RTC clock including enabling RTC oscillator. */
  CLOCK_CONFIG_SetRtcClock(RTC_OSC_CAP_LOAD_0PF, RTC_RTC32KCLK_PERIPHERALS_ENABLED);
  /* Configure the Internal Reference clock (MCGIRCLK). */
  CLOCK_SetInternalRefClkConfig(mcgConfig_BOARD_BootClockHSRUN.irclkEnableMode,
                                mcgConfig_BOARD_BootClockHSRUN.ircs,
                                mcgConfig_BOARD_BootClockHSRUN.fcrdiv);
  /* Configure FLL external reference divider (FRDIV). */
  CLOCK_CONFIG_SetFllExtRefDiv(mcgConfig_BOARD_BootClockHSRUN.frdiv);
  /* Set MCG to PEE mode. */
  CLOCK_BootToPeeMode(mcgConfig_BOARD_BootClockHSRUN.oscsel,
                      kMCG_PllClkSelPll0,
                      &mcgConfig_BOARD_BootClockHSRUN.pll0Config);
  /* Set the clock configuration in SIM module. */
  CLOCK_SetSimConfig(&simConfig_BOARD_BootClockHSRUN);
  /* Set SystemCoreClock variable. */
  SystemCoreClock = BOARD_BOOTCLOCKHSRUN_CORE_CLOCK;
  /* Set LPUART clock source. */
  CLOCK_SetLpuartClock(SIM_LPUART_CLK_SEL_PLLFLLSEL_CLK);
  /* Set FLEXIO clock source. */
  CLOCK_SetFlexio0Clock(SIM_FLEXIO_CLK_SEL_CORE_SYSTEM_CLK);
}

/*******************************************************************************
 ********************** Configuration BOARD_BootClockRUN ***********************
 ******************************************************************************/
/* TEXT BELOW IS USED AS SETTING FOR THE CLOCKS TOOL *****************************
!!Configuration
name: BOARD_BootClockRUN
outputs:
- {id: Bus_clock.outFreq, value: 60 MHz}
- {id: Core_clock.outFreq, value: 120 MHz}
- {id: ERCLK32K.outFreq, value: 32.768 kHz}
- {id: FLEXIOCLK.outFreq, value: 120 MHz}
- {id: Flash_clock.outFreq, value: 20 MHz}
- {id: FlexBus_clock.outFreq, value: 60 MHz}
- {id: IRC48MCLK.outFreq, value: 48 MHz}
- {id: LPO_clock.outFreq, value: 1 kHz}
- {id: LPUARTCLK.outFreq, value: 60 MHz}
- {id: MCGFFCLK.outFreq, value: 1.5 MHz}
- {id: MCGIRCLK.outFreq, value: 2 MHz}
- {id: MCGPLLCLK.outFreq, value: 120 MHz}
- {id: MCGPLLCLK2X.outFreq, value: 240 MHz}
- {id: PLLFLLCLK.outFreq, value: 120 MHz}
- {id: System_clock.outFreq, value: 120 MHz}
settings:
- {id: MCGMode, value: PEE}
- {id: FLEXIOClkConfig, value: 'yes'}
- {id: LPUARTClkConfig, value: 'yes'}
- {id: MCG.FLL_mul.scale, value: '2929', locked: true}
- {id: MCG.FRDIV.scale, value: '32', locked: true}
- {id: MCG.IRCS.sel, value: MCG.FCRDIV}
- {id: MCG.IREFS.sel, value: MCG.FRDIV}
- {id: MCG.OSCSEL.sel, value: SIM.IRC48MCLK}
- {id: MCG.OSCSEL_PLL.sel, value: SIM.IRC48MCLK}
- {id: MCG.PLLS.sel, value: MCG.PLL_DIV2}
- {id: MCG.PRDIV.scale, value: '4', locked: true}
- {id: MCG.VDIV.scale, value: '20', locked: true}
- {id: MCG_C1_IRCLKEN_CFG, value: Enabled}
- {id: MCG_C2_RANGE0_CFG, value: Very_high}
- {id: MCG_C2_RANGE0_FRDIV_CFG, value: Very_high}
- {id: MCG_C5_PLLCLKEN0_CFG, value: Enabled}
- {id: RTC_CR_OSCE_CFG, value: Enabled}
- {id: SIM.LPUARTSRCSEL.sel, value: SIM.PLLFLLDIV}
- {id: SIM.OSC32KSEL.sel, value: RTC.RTC32KCLK}
- {id: SIM.OUTDIV2.scale, value: '2'}
- {id: SIM.OUTDIV4.scale, value: '6'}
- {id: SIM.PLLFLLDIV.scale, value: '2', locked: true}
- {id: SIM.PLLFLLSEL.sel, value: MCG.MCGPLLCLK}
- {id: SIM.RTCCLKOUTSEL.sel, value: RTC.RTC32KCLK}
- {id: SIM.USBDIV.scale, value: '1', locked: true}
- {id: SIM.USBFRAC.scale, value: '1', locked: true}
sources:
- {id: IRC48M.IRC48M.outFreq, value: 48 MHz}
- {id: RTC.RTC32kHz.outFreq, value: 32.768 kHz, enabled: true}
 * BE CAREFUL MODIFYING THIS COMMENT - IT IS YAML SETTINGS FOR THE CLOCKS TOOL **/

/*******************************************************************************
 * Variables for BOARD_BootClockRUN configuration
 ******************************************************************************/
const mcg_config_t mcgConfig_BOARD_BootClockRUN =
  {
    .mcgMode = kMCG_ModePEE,                  /* PEE - PLL Engaged External */
    .irclkEnableMode = kMCG_IrclkEnable,      /* MCGIRCLK enabled, MCGIRCLK disabled in STOP mode */
    .ircs = kMCG_IrcFast,                     /* Fast internal reference clock selected */
    .fcrdiv = 0x1U,                           /* Fast IRC divider: divided by 2 */
    .frdiv = 0x0U,                            /* FLL reference clock divider: divided by 32 */
    .drs = kMCG_DrsHigh,                      /* High frequency range */
    .dmx32 = kMCG_Dmx32Fine,                  /* DCO is fine-tuned for maximum frequency with 32.768 kHz reference */
    .oscsel = kMCG_OscselIrc,                 /* Selects 48 MHz IRC Oscillator */
    .pll0Config =
      {
        .enableMode = kMCG_PllEnableIndependent,/* MCGPLLCLK enabled independent of MCG clock mode, MCGPLLCLK disabled in STOP mode */
        .prdiv = 0x3U,                    /* PLL Reference divider: divided by 4 */
        .vdiv = 0x4U,                     /* VCO divider: multiplied by 20 */
      },
  };
const sim_clock_config_t simConfig_BOARD_BootClockRUN =
  {
    .pllFllSel = SIM_PLLFLLSEL_MCGPLLCLK_CLK, /* PLLFLL select: MCGPLLCLK clock */
    .pllFllDiv = 1,                           /* PLLFLLSEL clock divider divisor: divided by 2 */
    .pllFllFrac = 0,                          /* PLLFLLSEL clock divider fraction: multiplied by 1 */
    .er32kSrc = SIM_OSC32KSEL_RTC32KCLK_CLK,  /* OSC32KSEL select: RTC32KCLK clock (32.768kHz) */
    .clkdiv1 = 0x1150000U,                    /* SIM_CLKDIV1 - OUTDIV1: /1, OUTDIV2: /2, OUTDIV3: /2, OUTDIV4: /6 */
  };
const osc_config_t oscConfig_BOARD_BootClockRUN =
  {
    .freq = 0U,                               /* Oscillator frequency: 0Hz */
    .capLoad = (OSC_CAP0P),                   /* Oscillator capacity load: 0pF */
    .workMode = kOSC_ModeExt,                 /* Use external clock */
    .oscerConfig =
      {
        .enableMode = OSC_ER_CLK_DISABLE, /* Disable external reference clock */
        .erclkDiv = 0,                    /* Divider for OSCERCLK: divided by 1 */
      }
  };

/*******************************************************************************
 * Code for BOARD_BootClockRUN configuration
 ******************************************************************************/
void BOARD_BootClockRUN(void)
{
  /* Set the system clock dividers in SIM to safe value. */
  CLOCK_SetSimSafeDivs();
  /* Configure RTC clock including enabling RTC oscillator. */
  CLOCK_CONFIG_SetRtcClock(RTC_OSC_CAP_LOAD_0PF, RTC_RTC32KCLK_PERIPHERALS_ENABLED);
  /* Configure the Internal Reference clock (MCGIRCLK). */
  CLOCK_SetInternalRefClkConfig(mcgConfig_BOARD_BootClockRUN.irclkEnableMode,
                                mcgConfig_BOARD_BootClockRUN.ircs,
                                mcgConfig_BOARD_BootClockRUN.fcrdiv);
  /* Configure FLL external reference divider (FRDIV). */
  CLOCK_CONFIG_SetFllExtRefDiv(mcgConfig_BOARD_BootClockRUN.frdiv);
  /* Set MCG to PEE mode. */
  CLOCK_BootToPeeMode(mcgConfig_BOARD_BootClockRUN.oscsel,
                      kMCG_PllClkSelPll0,
                      &mcgConfig_BOARD_BootClockRUN.pll0Config);
  /* Set the clock configuration in SIM module. */
  CLOCK_SetSimConfig(&simConfig_BOARD_BootClockRUN);
  /* Set SystemCoreClock variable. */
  SystemCoreClock = BOARD_BOOTCLOCKRUN_CORE_CLOCK;
  /* Set LPUART clock source. */
  CLOCK_SetLpuartClock(SIM_LPUART_CLK_SEL_PLLFLLSEL_CLK);
  /* Set FLEXIO clock source. */
  CLOCK_SetFlexio0Clock(SIM_FLEXIO_CLK_SEL_CORE_SYSTEM_CLK);
}

/*******************************************************************************
 ********************* Configuration BOARD_BootClockVLPR ***********************
 ******************************************************************************/
/* TEXT BELOW IS USED AS SETTING FOR THE CLOCKS TOOL *****************************
!!Configuration
name: BOARD_BootClockVLPR
outputs:
- {id: Bus_clock.outFreq, value: 4 MHz}
- {id: Core_clock.outFreq, value: 4 MHz}
- {id: ERCLK32K.outFreq, value: 32.768 kHz}
- {id: FLEXIOCLK.outFreq, value: 4 MHz}
- {id: Flash_clock.outFreq, value: 800 kHz}
- {id: FlexBus_clock.outFreq, value: 4 MHz}
- {id: LPO_clock.outFreq, value: 1 kHz}
- {id: LPUARTCLK.outFreq, value: 4 MHz}
- {id: MCGIRCLK.outFreq, value: 4 MHz}
- {id: System_clock.outFreq, value: 4 MHz}
settings:
- {id: MCGMode, value: BLPI}
- {id: powerMode, value: VLPR}
- {id: FLEXIOClkConfig, value: 'yes'}
- {id: LPUARTClkConfig, value: 'yes'}
- {id: MCG.CLKS.sel, value: MCG.IRCS}
- {id: MCG.FCRDIV.scale, value: '1'}
- {id: MCG.IRCS.sel, value: MCG.FCRDIV}
- {id: MCG_C1_IRCLKEN_CFG, value: Enabled}
- {id: RTC_CR_OSCE_CFG, value: Enabled}
- {id: SIM.FLEXIOSRCSEL.sel, value: MCG.MCGIRCLK}
- {id: SIM.LPUARTSRCSEL.sel, value: MCG.MCGIRCLK}
- {id: SIM.OSC32KSEL.sel, value: RTC.RTC32KCLK}
- {id: SIM.OUTDIV3.scale, value: '1'}
- {id: SIM.OUTDIV4.scale, value: '5'}
sources:
- {id: RTC.RTC32kHz.outFreq, value: 32.768 kHz, enabled: true}
 * BE CAREFUL MODIFYING THIS COMMENT - IT IS YAML SETTINGS FOR THE CLOCKS TOOL **/

/*******************************************************************************
 * Variables for BOARD_BootClockVLPR configuration
 ******************************************************************************/
const mcg_config_t mcgConfig_BOARD_BootClockVLPR =
  {
    .mcgMode = kMCG_ModeBLPI,                 /* BLPI - Bypassed Low Power Internal */
    .irclkEnableMode = kMCG_IrclkEnable,      /* MCGIRCLK enabled, MCGIRCLK disabled in STOP mode */
    .ircs = kMCG_IrcFast,                     /* Fast internal reference clock selected */
    .fcrdiv = 0x0U,                           /* Fast IRC divider: divided by 1 */
    .frdiv = 0x0U,                            /* FLL reference clock divider: divided by 1 */
    .drs = kMCG_DrsLow,                       /* Low frequency range */
    .dmx32 = kMCG_Dmx32Default,               /* DCO has a default range of 25% */
    .oscsel = kMCG_OscselOsc,                 /* Selects System Oscillator (OSCCLK) */
    .pll0Config =
      {
        .enableMode = MCG_PLL_DISABLE,    /* MCGPLLCLK disabled */
        .prdiv = 0x0U,                    /* PLL Reference divider: divided by 1 */
        .vdiv = 0x0U,                     /* VCO divider: multiplied by 16 */
      },
  };
const sim_clock_config_t simConfig_BOARD_BootClockVLPR =
  {
    .pllFllSel = SIM_PLLFLLSEL_MCGFLLCLK_CLK, /* PLLFLL select: MCGFLLCLK clock */
    .pllFllDiv = 0,                           /* PLLFLLSEL clock divider divisor: divided by 1 */
    .pllFllFrac = 0,                          /* PLLFLLSEL clock divider fraction: multiplied by 1 */
    .er32kSrc = SIM_OSC32KSEL_RTC32KCLK_CLK,  /* OSC32KSEL select: RTC32KCLK clock (32.768kHz) */
        .clkdiv1 = 0x40000U,                      /* SIM_CLKDIV1 - OUTDIV1: /1, OUTDIV2: /1, OUTDIV3: /1, OUTDIV4: /5 */
  };
const osc_config_t oscConfig_BOARD_BootClockVLPR =
  {
    .freq = 0U,                               /* Oscillator frequency: 0Hz */
    .capLoad = (OSC_CAP0P),                   /* Oscillator capacity load: 0pF */
    .workMode = kOSC_ModeExt,                 /* Use external clock */
    .oscerConfig =
      {
        .enableMode = OSC_ER_CLK_DISABLE, /* Disable external reference clock */
        .erclkDiv = 0,                    /* Divider for OSCERCLK: divided by 1 */
      }
  };

/*******************************************************************************
 * Code for BOARD_BootClockVLPR configuration
 ******************************************************************************/
void BOARD_BootClockVLPR(void)
{
  /* Set the system clock dividers in SIM to safe value. */
  CLOCK_SetSimSafeDivs();
  /* Configure RTC clock including enabling RTC oscillator. */
  CLOCK_CONFIG_SetRtcClock(RTC_OSC_CAP_LOAD_0PF, RTC_RTC32KCLK_PERIPHERALS_ENABLED);
  /* Set MCG to BLPI mode. */
  CLOCK_BootToBlpiMode(mcgConfig_BOARD_BootClockVLPR.fcrdiv,
                       mcgConfig_BOARD_BootClockVLPR.ircs,
                       mcgConfig_BOARD_BootClockVLPR.irclkEnableMode);
  /* Set the clock configuration in SIM module. */
  CLOCK_SetSimConfig(&simConfig_BOARD_BootClockVLPR);
  /* Set VLPR power mode. */
  SMC_SetPowerModeProtection(SMC, kSMC_AllowPowerModeAll);
#if (defined(FSL_FEATURE_SMC_HAS_LPWUI) && FSL_FEATURE_SMC_HAS_LPWUI)
  SMC_SetPowerModeVlpr(SMC, false);
#else
  SMC_SetPowerModeVlpr(SMC);
#endif
  while (SMC_GetPowerModeState(SMC) != kSMC_PowerStateVlpr)
  {
  }
  /* Set SystemCoreClock variable. */
  SystemCoreClock = BOARD_BOOTCLOCKVLPR_CORE_CLOCK;
  /* Set LPUART clock source. */
  CLOCK_SetLpuartClock(SIM_LPUART_CLK_SEL_MCGIRCLK_CLK);
  /* Set FLEXIO clock source. */
  CLOCK_SetFlexio0Clock(SIM_FLEXIO_CLK_SEL_MCGIRCLK_CLK);
}

