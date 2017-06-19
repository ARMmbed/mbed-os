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

/* TEXT BELOW IS USED AS SETTING FOR TOOLS *************************************
!!GlobalInfo
product: Clocks v3.0
processor: MKL82Z128xxx7
package_id: MKL82Z128VLL7
mcu_data: ksdk2_0
processor_version: 2.0.0
 * BE CAREFUL MODIFYING THIS COMMENT - IT IS YAML SETTINGS FOR TOOLS **********/

#include "fsl_clock_config.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/
#define MCG_IRCLK_DISABLE                                 0U  /*!< MCGIRCLK disabled */
#define MCG_PLL_DISABLE                                   0U  /*!< MCGPLLCLK disabled */
#define OSC_CAP0P                                         0U  /*!< Oscillator 0pF capacitor load */
#define OSC_ER_CLK_DISABLE                                0U  /*!< Disable external reference clock */
#define SIM_LPUART_CLK_SEL_PLLFLLSEL_CLK                  1U  /*!< LPUART clock select: PLLFLLSEL output clock */
#define SIM_OSC32KSEL_OSC32KCLK_CLK                       0U  /*!< OSC32KSEL select: OSC32KCLK clock */
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
 ************************ BOARD_InitBootClocks function ************************
 ******************************************************************************/
void BOARD_InitBootClocks(void)
{
    BOARD_BootClockRUN();
}

/*******************************************************************************
 ********************** Configuration BOARD_BootClockRUN ***********************
 ******************************************************************************/
/* TEXT BELOW IS USED AS SETTING FOR TOOLS *************************************
!!Configuration
name: BOARD_BootClockRUN
called_from_default_init: true
outputs:
- {id: Bus_clock.outFreq, value: 16 MHz}
- {id: Core_clock.outFreq, value: 48 MHz}
- {id: Fast_bus_clock.outFreq, value: 48 MHz}
- {id: Flash_clock.outFreq, value: 16 MHz}
- {id: IRC48MCLK.outFreq, value: 48 MHz}
- {id: LPO_clock.outFreq, value: 1 kHz}
- {id: LPUARTCLK.outFreq, value: 48 MHz}
- {id: MCGPLLCLK.outFreq, value: 96 MHz}
- {id: MCGPLLCLK2X.outFreq, value: 192 MHz}
- {id: PLLFLLCLK.outFreq, value: 96 MHz}
- {id: System_clock.outFreq, value: 48 MHz}
settings:
- {id: MCGMode, value: PEE}
- {id: LPUARTClkConfig, value: 'yes'}
- {id: MCG.IREFS.sel, value: MCG.FRDIV}
- {id: MCG.OSCSEL.sel, value: SIM.IRC48MCLK}
- {id: MCG.OSCSEL_PLL.sel, value: SIM.IRC48MCLK}
- {id: MCG.PLLS.sel, value: MCG.PLL_DIV2}
- {id: MCG.PRDIV.scale, value: '4'}
- {id: RTC_CR_OSCE_CFG, value: Enabled}
- {id: SIM.FLEXIOSRCSEL.sel, value: SIM.PLLFLLDIV}
- {id: SIM.LPUARTSRCSEL.sel, value: SIM.PLLFLLDIV}
- {id: SIM.OUTDIV1.scale, value: '2'}
- {id: SIM.OUTDIV2.scale, value: '6'}
- {id: SIM.OUTDIV4.scale, value: '6'}
- {id: SIM.OUTDIV5.scale, value: '2'}
- {id: SIM.PLLFLLDIV.scale, value: '2'}
- {id: SIM.PLLFLLSEL.sel, value: MCG.MCGPLLCLK}
sources:
- {id: IRC48M.IRC48M.outFreq, value: 48 MHz}
 * BE CAREFUL MODIFYING THIS COMMENT - IT IS YAML SETTINGS FOR TOOLS **********/

/*******************************************************************************
 * Variables for BOARD_BootClockRUN configuration
 ******************************************************************************/
const mcg_config_t mcgConfig_BOARD_BootClockRUN =
    {
        .mcgMode = kMCG_ModePEE,                  /* PEE - PLL Engaged External */
        .irclkEnableMode = MCG_IRCLK_DISABLE,     /* MCGIRCLK disabled */
        .ircs = kMCG_IrcSlow,                     /* Slow internal reference clock selected */
        .fcrdiv = 0x1U,                           /* Fast IRC divider: divided by 2 */
        .frdiv = 0x0U,                            /* FLL reference clock divider: divided by 1 */
        .drs = kMCG_DrsLow,                       /* Low frequency range */
        .dmx32 = kMCG_Dmx32Default,               /* DCO has a default range of 25% */
        .oscsel = kMCG_OscselIrc,                 /* Selects 48 MHz IRC Oscillator */
        .pll0Config =
            {
                .enableMode = MCG_PLL_DISABLE,    /* MCGPLLCLK disabled */
                .prdiv = 0x3U,                    /* PLL Reference divider: divided by 4 */
                .vdiv = 0x0U,                     /* VCO divider: multiplied by 16 */
            },
    };
const sim_clock_config_t simConfig_BOARD_BootClockRUN =
    {
        .pllFllSel = SIM_PLLFLLSEL_MCGPLLCLK_CLK, /* PLLFLL select: MCGPLLCLK clock */
        .pllFllDiv = 1,                           /* PLLFLLSEL clock divider divisor: divided by 2 */
        .pllFllFrac = 0,                          /* PLLFLLSEL clock divider fraction: multiplied by 1 */
        .er32kSrc = SIM_OSC32KSEL_OSC32KCLK_CLK,  /* OSC32KSEL select: OSC32KCLK clock */
        .clkdiv1 = 0x15051000U,                   /* SIM_CLKDIV1 - OUTDIV1: /2, OUTDIV2: /6, OUTDIV4: /6, OUTDIV5: /2 */
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
}

