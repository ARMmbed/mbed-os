/*
 * Copyright 2018-2019 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "fsl_clock_config.h"
#include "fsl_iomuxc.h"
#include "fsl_dcdc.h"
#include "fsl_pmu.h"


/*******************************************************************************
 * Definitions
 ******************************************************************************/

/*******************************************************************************
 * Variables
 ******************************************************************************/
/* System clock frequency. */
extern uint32_t SystemCoreClock;

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

#ifndef SKIP_POWER_ADJUSTMENT
#if __CORTEX_M == 7
#define BYPASS_LDO_LPSR 1
#define SKIP_LDO_ADJUSTMENT 1
#elif __CORTEX_M == 4
#define SKIP_DCDC_ADJUSTMENT 1
#define SKIP_FBB_ENABLE 1
#endif
#endif

/*******************************************************************************
 * Code for BOARD_BootClockRUN configuration
 ******************************************************************************/
void BOARD_BootClockRUN(void)
{
    clock_root_config_t rootCfg = {0};

#if !defined(SKIP_DCDC_ADJUSTMENT) || (!SKIP_DCDC_ADJUSTMENT)
    DCDC_SetVDD1P0BuckModeTargetVoltage(DCDC, kDCDC_1P0BuckTarget1P15V);
#endif

#if !defined(SKIP_FBB_ENABLE) || (!SKIP_FBB_ENABLE)
    pmu_static_body_bias_config_t config;
    
    PMU_StaticGetCm7FBBDefaultConfig(&config);
    PMU_StaticCm7FBBInit(ANADIG_PMU, &config);
#endif

#if defined(BYPASS_LDO_LPSR) && BYPASS_LDO_LPSR
    PMU_StaticSetLpsrAnaLdoBypassMode(ANADIG_LDO_SNVS, kPMU_LpsrAnaLdoBypassMode1);
    PMU_StaticEnableLpsrDigLdoBypassMode(ANADIG_LDO_SNVS, true);
#endif

#if !defined(SKIP_LDO_ADJUSTMENT) || (!SKIP_LDO_ADJUSTMENT)
    pmu_static_lpsr_ana_ldo_config_t lpsrAnaConfig;
    pmu_static_lpsr_dig_config_t lpsrDigConfig;

    if((ANADIG_LDO_SNVS->PMU_LDO_LPSR_ANA & ANADIG_LDO_SNVS_PMU_LDO_LPSR_ANA_BYPASS_MODE_EN_MASK) == 0UL)
    {
        PMU_StaticGetLpsrAnaLdoDefaultConfig(&lpsrAnaConfig);
        PMU_StaticLpsrAnaLdoInit(ANADIG_LDO_SNVS, &lpsrAnaConfig);
    }

    if((ANADIG_LDO_SNVS->PMU_LDO_LPSR_DIG & ANADIG_LDO_SNVS_PMU_LDO_LPSR_DIG_BYPASS_MODE_MASK) == 0UL)
    {
        PMU_StaticGetLpsrDigLdoDefaultConfig(&lpsrDigConfig);
        lpsrDigConfig.targetVoltage = kPMU_LpsrDigTargetStableVoltage1P117V;
        PMU_StaticLpsrDigLdoInit(ANADIG_LDO_SNVS, &lpsrDigConfig);
    }
#endif

    /* SYS PLL2 528MHz. */
    const clock_sys_pll_config_t sysPllConfig = {
        .loopDivider = 1,
        /* Using 24Mhz OSC */
        .mfn = 0,
        .mfi = 22,
    };

    const clock_sys_pll3_config_t sysPll3Config = {
        .divSelect = 3,
    };

    /* PLL LDO shall be enabled first before enable PLLs */
    CLOCK_EnableOsc24M();

#if __CORTEX_M == 7
    rootCfg.mux = kCLOCK_M7_ClockRoot_MuxOscRc48MDiv2;
    rootCfg.div = 1;
    CLOCK_SetRootClock(kCLOCK_Root_M7, &rootCfg);
    CLOCK_SetRootClock(kCLOCK_Root_M7_Systick, &rootCfg);

#if defined(CONSUMER_SERIES)
    CLOCK_InitArmPllWithFreq(1000);
#elif defined(AUTOMOTIVE_SERIES) || defined(INDUSTRIAL_SERIES)
    CLOCK_InitArmPllWithFreq(800);
#endif
    /* Configure M7 */
    rootCfg.mux = kCLOCK_M7_ClockRoot_MuxArmPllOut;
    rootCfg.div = 1;
    CLOCK_SetRootClock(kCLOCK_Root_M7, &rootCfg);

    /* Configure M7 Systick running at 10K */
    rootCfg.mux = kCLOCK_M7_ClockRoot_MuxOscRc48MDiv2;
    rootCfg.div = 240;
    CLOCK_SetRootClock(kCLOCK_Root_M7_Systick, &rootCfg);
#endif
    CLOCK_InitSysPll2(&sysPllConfig);
    CLOCK_InitSysPll3(&sysPll3Config);

#if __CORTEX_M == 4
    rootCfg.mux = kCLOCK_M4_ClockRoot_MuxOscRc48MDiv2;
    rootCfg.div = 1;
    CLOCK_SetRootClock(kCLOCK_Root_M4, &rootCfg);
    CLOCK_SetRootClock(kCLOCK_Root_Bus_Lpsr, &rootCfg);

    CLOCK_InitPfd(kCLOCK_PllSys3, kCLOCK_Pfd3, 22);
    /* Configure M4 using SysPll3Pfd3 divided by 1 */
    rootCfg.mux = kCLOCK_M4_ClockRoot_MuxSysPll3Pfd3;
    rootCfg.div = 1;
    CLOCK_SetRootClock(kCLOCK_Root_M4, &rootCfg);

    /* SysPll3 divide by 3 */
    rootCfg.mux = kCLOCK_BUS_LPSR_ClockRoot_MuxSysPll3Out;
    rootCfg.div = 3;
    CLOCK_SetRootClock(kCLOCK_Root_Bus_Lpsr, &rootCfg);
#endif

#if DEBUG_CONSOLE_UART_INDEX == 1
    /* Configure Lpuart1 using SysPll2*/
    rootCfg.mux = kCLOCK_LPUART1_ClockRoot_MuxSysPll2Out;
    rootCfg.div = 22;
    CLOCK_SetRootClock(kCLOCK_Root_Lpuart1, &rootCfg);
#else
    /* Configure Lpuart2 using SysPll2*/
    rootCfg.mux = kCLOCK_LPUART2_ClockRoot_MuxSysPll2Out;
    rootCfg.div = 22;
    CLOCK_SetRootClock(kCLOCK_Root_Lpuart2, &rootCfg);
#endif

#ifndef SKIP_SEMC_INIT
    CLOCK_InitPfd(kCLOCK_PllSys2, kCLOCK_Pfd1, 16);
    /* Configure Semc using SysPll2Pfd1 divided by 3 */
    rootCfg.mux = kCLOCK_SEMC_ClockRoot_MuxSysPll2Pfd1;
    rootCfg.div = 3;
    CLOCK_SetRootClock(kCLOCK_Root_Semc, &rootCfg);
#endif

    /* Configure Bus using SysPll3 divided by 2 */
    rootCfg.mux = kCLOCK_BUS_ClockRoot_MuxSysPll3Out;
    rootCfg.div = 2;
    CLOCK_SetRootClock(kCLOCK_Root_Bus, &rootCfg);

    /* Configure Lpi2c1 using Osc48MDiv2 */
    rootCfg.mux = kCLOCK_LPI2C1_ClockRoot_MuxOscRc48MDiv2;
    rootCfg.div = 1;
    CLOCK_SetRootClock(kCLOCK_Root_Lpi2c1, &rootCfg);

    /* Configure Lpi2c5 using Osc48MDiv2 */
    rootCfg.mux = kCLOCK_LPI2C5_ClockRoot_MuxOscRc48MDiv2;
    rootCfg.div = 1;
    CLOCK_SetRootClock(kCLOCK_Root_Lpi2c5, &rootCfg);

    /* Configure gpt timer using Osc48MDiv2 */
    rootCfg.mux = kCLOCK_GPT1_ClockRoot_MuxOscRc48MDiv2;
    rootCfg.div = 1;
    CLOCK_SetRootClock(kCLOCK_Root_Gpt1, &rootCfg);

    /* Configure gpt timer using Osc48MDiv2 */
    rootCfg.mux = kCLOCK_GPT2_ClockRoot_MuxOscRc48MDiv2;
    rootCfg.div = 1;
    CLOCK_SetRootClock(kCLOCK_Root_Gpt2, &rootCfg);

    /* Configure lpspi using Osc48MDiv2 */
    rootCfg.mux = kCLOCK_LPSPI1_ClockRoot_MuxOscRc48MDiv2;
    rootCfg.div = 1;
    CLOCK_SetRootClock(kCLOCK_Root_Lpspi1, &rootCfg);

    /* Configure flexio using Osc48MDiv2 */
    rootCfg.mux = kCLOCK_FLEXIO2_ClockRoot_MuxOscRc48MDiv2;
    rootCfg.div = 1;
    CLOCK_SetRootClock(kCLOCK_Root_Flexio2, &rootCfg);

    /* Configure emvsim using Osc48MDiv2 */
    rootCfg.mux = kCLOCK_EMV1_ClockRoot_MuxOscRc48MDiv2;
    rootCfg.div = 1;
    CLOCK_SetRootClock(kCLOCK_Root_Emv1, &rootCfg);

#if __CORTEX_M == 7
    SystemCoreClock = CLOCK_GetRootClockFreq(kCLOCK_Root_M7);
#else
    SystemCoreClock = CLOCK_GetRootClockFreq(kCLOCK_Root_M4);
#endif
}
