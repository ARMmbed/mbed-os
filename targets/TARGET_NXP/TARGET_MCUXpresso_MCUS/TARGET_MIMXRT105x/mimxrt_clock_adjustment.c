/*
 * Copyright 2021 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 * Mbed: This file was imported from "specific.c" in the "power_mode_switch_bm" SDK example.
 * It's responsible for switching around the clocks for low-power/full-speed/overdrive running.
 * We modified it a bit to make it pull clock info from clock_config.h, so you don't have to enter
 * clock configurations in two places.
 *
 * Not every clock configuration can be deduced, but it can at least pull in the dividers for
 * IPG_CLK_ROOT, SEMC_CLK_ROOT, and PERCLK_CLK_ROOT.
 *
 * We also removed parts of the file that control the FlexSPI clock source, because we could
 * not determine, based on available info, whether this might perturb the code flash (FlexSPI)
 * clock timing.
 *
 * Note: This file has to be used instead of just calling the clock init functions in
 * clock_config.c, because those functions turn off clocks for all the peripherals,
 * which makes things like the UART and timers die (or even crash the entire chip if
 * code tries to use them while they're unclocked).
 */

#include "mimxrt_clock_adjustment.h"
#include "fsl_common.h"
#include "fsl_clock.h"
#include "lpm.h"
#include "fsl_iomuxc.h"
#include "clock_config.h"

#include <stdio.h>
#include <inttypes.h>

/*******************************************************************************
 * Code
 ******************************************************************************/

AT_QUICKACCESS_SECTION_CODE(void SwitchSystemClocks(lpm_power_mode_t power_mode));

void SwitchSystemClocks(lpm_power_mode_t power_mode)
{
#if (defined(XIP_EXTERNAL_FLASH) && (XIP_EXTERNAL_FLASH == 1))
    LPM_EnterCritical();
    while (!((FLEXSPI_INST->STS0 & FLEXSPI_STS0_ARBIDLE_MASK) && (FLEXSPI_INST->STS0 & FLEXSPI_STS0_SEQIDLE_MASK)))
    {
    }
    FLEXSPI_INST->MCR0 |= FLEXSPI_MCR0_MDIS_MASK;

    /* Disable clock gate of flexspi. */
    CCM->CCGR6 &= (~CCM_CCGR6_CG5_MASK);
#endif

    switch (power_mode)
    {
        case LPM_PowerModeOverRun:
            CLOCK_SET_DIV(kCLOCK_SemcDiv, BOARD_CLOCKOVERDRIVE_AHB_CLK_ROOT / BOARD_CLOCKOVERDRIVE_SEMC_CLK_ROOT - 1); // Deduce SEMC divider from clock_config.h defines
            /* CORE CLK to 600MHz, AHB, IPG to 150MHz, PERCLK to 75MHz */
            CLOCK_SET_DIV(kCLOCK_PerclkDiv, BOARD_XTAL0_CLK_HZ / BOARD_CLOCKFULLSPEED_PERCLK_CLK_ROOT - 1); // Deduce PERCLK divider
            CLOCK_SET_DIV(kCLOCK_IpgDiv, BOARD_CLOCKOVERDRIVE_AHB_CLK_ROOT / BOARD_CLOCKFULLSPEED_IPG_CLK_ROOT - 1);
            CLOCK_SET_DIV(kCLOCK_AhbDiv, 0);
            CLOCK_SET_MUX(kCLOCK_PerclkMux, 1); // PERCLK mux to OSC CLK
            CLOCK_SET_MUX(kCLOCK_PrePeriphMux, 3); // PRE_PERIPH_CLK mux to ARM PLL
            CLOCK_SET_MUX(kCLOCK_PeriphMux, 0);    // PERIPH_CLK mux to PRE_PERIPH_CLK

            CLOCK_SET_MUX(kCLOCK_FlexspiMux, 3); // FLEXSPI mux to PLL3 PFD0, matches setting used by ROM bootloader
            break;
        case LPM_PowerModeFullRun:
            CLOCK_SET_DIV(kCLOCK_SemcDiv, BOARD_CLOCKFULLSPEED_AHB_CLK_ROOT / BOARD_CLOCKFULLSPEED_SEMC_CLK_ROOT - 1); // Deduce SEMC divider from clock_config.h defines
            /* CORE CLK to 528MHz, AHB, IPG to 132MHz, PERCLK to 66MHz */
            CLOCK_SET_DIV(kCLOCK_PerclkDiv, BOARD_XTAL0_CLK_HZ / BOARD_CLOCKFULLSPEED_PERCLK_CLK_ROOT - 1); // Deduce PERCLK divider
            CLOCK_SET_DIV(kCLOCK_IpgDiv, BOARD_CLOCKFULLSPEED_AHB_CLK_ROOT / BOARD_CLOCKFULLSPEED_IPG_CLK_ROOT - 1);
            CLOCK_SET_DIV(kCLOCK_AhbDiv, 0);
            CLOCK_SET_MUX(kCLOCK_PerclkMux, 1); // PERCLK mux to OSC CLK
            CLOCK_SET_MUX(kCLOCK_PrePeriphMux, 0); // PRE_PERIPH_CLK mux to SYS PLL
            CLOCK_SET_MUX(kCLOCK_PeriphMux, 0);    // PERIPH_CLK mux to PRE_PERIPH_CLK

            CLOCK_SET_MUX(kCLOCK_FlexspiMux, 3); // FLEXSPI mux to PLL3 PFD0, matches setting used by ROM bootloader
            break;
        case LPM_PowerModeLowPowerRun:
        case LPM_PowerModeLPIdle:
            CLOCK_SET_DIV(kCLOCK_PeriphClk2Div, 0);
            CLOCK_SET_MUX(kCLOCK_PeriphClk2Mux, 1); // PERIPH_CLK2 mux to OSC
            CLOCK_SET_MUX(kCLOCK_PeriphMux, 1);     // PERIPH_CLK mux to PERIPH_CLK2
            CLOCK_SET_DIV(kCLOCK_SemcDiv, BOARD_CLOCKLOWPOWER_AHB_CLK_ROOT / BOARD_CLOCKLOWPOWER_SEMC_CLK_ROOT - 1); // Deduce SEMC divider from clock_config.h defines
            CLOCK_SET_MUX(kCLOCK_SemcMux, 0); // SEMC mux to PERIPH_CLK
            CLOCK_SET_MUX(kCLOCK_FlexspiMux, 0); // FLEXSPI mux to semc_clk_root_pre
            /* CORE CLK to 24MHz and AHB, IPG, PERCLK to 12MHz */
            CLOCK_SET_DIV(kCLOCK_PerclkDiv, BOARD_XTAL0_CLK_HZ / BOARD_CLOCKLOWPOWER_PERCLK_CLK_ROOT - 1); // Deduce PERCLK divider
            CLOCK_SET_DIV(kCLOCK_IpgDiv, BOARD_CLOCKLOWPOWER_AHB_CLK_ROOT / BOARD_CLOCKLOWPOWER_IPG_CLK_ROOT - 1);
            CLOCK_SET_DIV(kCLOCK_AhbDiv, 0);
            CLOCK_SET_MUX(kCLOCK_PerclkMux, 1); // PERCLK mux to OSC CLK
            break;
        default:
            break;
    }

#if (defined(XIP_EXTERNAL_FLASH) && (XIP_EXTERNAL_FLASH == 1))
    /* Enable clock gate of flexspi. */
    CCM->CCGR6 |= (CCM_CCGR6_CG5_MASK);

    FLEXSPI_INST->MCR0 &= ~FLEXSPI_MCR0_MDIS_MASK;
    FLEXSPI_INST->MCR0 |= FLEXSPI_MCR0_SWRESET_MASK;
    while (FLEXSPI_INST->MCR0 & FLEXSPI_MCR0_SWRESET_MASK)
    {
    }
    while (!((FLEXSPI_INST->STS0 & FLEXSPI_STS0_ARBIDLE_MASK) && (FLEXSPI_INST->STS0 & FLEXSPI_STS0_SEQIDLE_MASK)))
    {
    }
    LPM_ExitCritical();
#endif
}

void ClockSetToOverDriveRun(void)
{
    // CORE CLK mux to 24M before reconfigure PLLs
    SwitchSystemClocks(LPM_PowerModeLowPowerRun);

    /* Init ARM PLL */
    CLOCK_SetDiv(kCLOCK_ArmDiv, 1);
    CLOCK_InitArmPll(&armPllConfig_BOARD_ClockOverdrive);

    /* Init SYS PLL*/
    CLOCK_InitSysPll(&sysPllConfig_BOARD_ClockOverdrive);

    /* Init USB1 PLL. */
    CLOCK_InitUsb1Pll(&usb1PllConfig_BOARD_ClockOverdrive);

    SwitchSystemClocks(LPM_PowerModeOverRun);
}

void ClockSetToFullSpeedRun(void)
{
    // CORE CLK mux to 24M before reconfigure PLLs
    SwitchSystemClocks(LPM_PowerModeLowPowerRun);

    /* Init ARM PLL */
    CLOCK_SetDiv(kCLOCK_ArmDiv, 1);
    CLOCK_InitArmPll(&armPllConfig_BOARD_ClockFullSpeed);

    /* Init SYS PLL. */
    CLOCK_InitSysPll(&sysPllConfig_BOARD_ClockFullSpeed);

    /* Init USB1 PLL. */
    CLOCK_InitUsb1Pll(&usb1PllConfig_BOARD_ClockFullSpeed);

    SwitchSystemClocks(LPM_PowerModeFullRun);
}

void ClockSetToLowPowerRun(void)
{
    // CORE CLK mux to 24M before reconfigure PLLs
    SwitchSystemClocks(LPM_PowerModeLowPowerRun);

    /* Deinit ARM PLL */
    CLOCK_DeinitArmPll();

    /* Deinit SYS PLL */
    CLOCK_DeinitSysPll();

    /* Power Down USB1 PLL */
    CCM_ANALOG->PLL_USB1_SET = CCM_ANALOG_PLL_USB1_BYPASS_MASK;
    CCM_ANALOG->PLL_USB1_CLR = CCM_ANALOG_PLL_USB1_POWER_MASK;
    CCM_ANALOG->PLL_USB1_CLR = CCM_ANALOG_PLL_USB1_ENABLE_MASK;

    /* Deinit USB2 PLL */
    CLOCK_DeinitUsb2Pll();

    /* Deinit AUDIO PLL */
    CLOCK_DeinitAudioPll();

    /* Deinit VIDEO PLL */
    CLOCK_DeinitVideoPll();

    /* Deinit ENET PLL */
    CLOCK_DeinitEnetPll();
}

#define GPR4_STOP_REQ_BITS                                                                                          \
    (IOMUXC_GPR_GPR4_ENET_STOP_REQ_MASK | IOMUXC_GPR_GPR4_SAI1_STOP_REQ_MASK | IOMUXC_GPR_GPR4_SAI2_STOP_REQ_MASK | \
     IOMUXC_GPR_GPR4_SAI3_STOP_REQ_MASK | IOMUXC_GPR_GPR4_SEMC_STOP_REQ_MASK | IOMUXC_GPR_GPR4_PIT_STOP_REQ_MASK |  \
     IOMUXC_GPR_GPR4_FLEXIO1_STOP_REQ_MASK | IOMUXC_GPR_GPR4_FLEXIO2_STOP_REQ_MASK)

#define GPR4_STOP_ACK_BITS                                                                                          \
    (IOMUXC_GPR_GPR4_ENET_STOP_ACK_MASK | IOMUXC_GPR_GPR4_SAI1_STOP_ACK_MASK | IOMUXC_GPR_GPR4_SAI2_STOP_ACK_MASK | \
     IOMUXC_GPR_GPR4_SAI3_STOP_ACK_MASK | IOMUXC_GPR_GPR4_SEMC_STOP_ACK_MASK | IOMUXC_GPR_GPR4_PIT_STOP_ACK_MASK |  \
     IOMUXC_GPR_GPR4_FLEXIO1_STOP_ACK_MASK | IOMUXC_GPR_GPR4_FLEXIO2_STOP_ACK_MASK)

#define GPR7_STOP_REQ_BITS                                                           \
    (IOMUXC_GPR_GPR7_LPI2C1_STOP_REQ_MASK | IOMUXC_GPR_GPR7_LPI2C2_STOP_REQ_MASK |   \
     IOMUXC_GPR_GPR7_LPI2C3_STOP_REQ_MASK | IOMUXC_GPR_GPR7_LPI2C4_STOP_REQ_MASK |   \
     IOMUXC_GPR_GPR7_LPSPI1_STOP_REQ_MASK | IOMUXC_GPR_GPR7_LPSPI2_STOP_REQ_MASK |   \
     IOMUXC_GPR_GPR7_LPSPI3_STOP_REQ_MASK | IOMUXC_GPR_GPR7_LPSPI4_STOP_REQ_MASK |   \
     IOMUXC_GPR_GPR7_LPUART1_STOP_REQ_MASK | IOMUXC_GPR_GPR7_LPUART2_STOP_REQ_MASK | \
     IOMUXC_GPR_GPR7_LPUART3_STOP_REQ_MASK | IOMUXC_GPR_GPR7_LPUART4_STOP_REQ_MASK | \
     IOMUXC_GPR_GPR7_LPUART5_STOP_REQ_MASK | IOMUXC_GPR_GPR7_LPUART6_STOP_REQ_MASK | \
     IOMUXC_GPR_GPR7_LPUART7_STOP_REQ_MASK | IOMUXC_GPR_GPR7_LPUART8_STOP_REQ_MASK)

#define GPR7_STOP_ACK_BITS                                                           \
    (IOMUXC_GPR_GPR7_LPI2C1_STOP_ACK_MASK | IOMUXC_GPR_GPR7_LPI2C2_STOP_ACK_MASK |   \
     IOMUXC_GPR_GPR7_LPI2C3_STOP_ACK_MASK | IOMUXC_GPR_GPR7_LPI2C4_STOP_ACK_MASK |   \
     IOMUXC_GPR_GPR7_LPSPI1_STOP_ACK_MASK | IOMUXC_GPR_GPR7_LPSPI2_STOP_ACK_MASK |   \
     IOMUXC_GPR_GPR7_LPSPI3_STOP_ACK_MASK | IOMUXC_GPR_GPR7_LPSPI4_STOP_ACK_MASK |   \
     IOMUXC_GPR_GPR7_LPUART1_STOP_ACK_MASK | IOMUXC_GPR_GPR7_LPUART2_STOP_ACK_MASK | \
     IOMUXC_GPR_GPR7_LPUART3_STOP_ACK_MASK | IOMUXC_GPR_GPR7_LPUART4_STOP_ACK_MASK | \
     IOMUXC_GPR_GPR7_LPUART5_STOP_ACK_MASK | IOMUXC_GPR_GPR7_LPUART6_STOP_ACK_MASK | \
     IOMUXC_GPR_GPR7_LPUART7_STOP_ACK_MASK | IOMUXC_GPR_GPR7_LPUART8_STOP_ACK_MASK)

#define GPR8_DOZE_BITS                                                               \
    (IOMUXC_GPR_GPR8_LPI2C1_IPG_DOZE_MASK | IOMUXC_GPR_GPR8_LPI2C2_IPG_DOZE_MASK |   \
     IOMUXC_GPR_GPR8_LPI2C3_IPG_DOZE_MASK | IOMUXC_GPR_GPR8_LPI2C4_IPG_DOZE_MASK |   \
     IOMUXC_GPR_GPR8_LPSPI1_IPG_DOZE_MASK | IOMUXC_GPR_GPR8_LPSPI2_IPG_DOZE_MASK |   \
     IOMUXC_GPR_GPR8_LPSPI3_IPG_DOZE_MASK | IOMUXC_GPR_GPR8_LPSPI4_IPG_DOZE_MASK |   \
     IOMUXC_GPR_GPR8_LPUART1_IPG_DOZE_MASK | IOMUXC_GPR_GPR8_LPUART2_IPG_DOZE_MASK | \
     IOMUXC_GPR_GPR8_LPUART3_IPG_DOZE_MASK | IOMUXC_GPR_GPR8_LPUART4_IPG_DOZE_MASK | \
     IOMUXC_GPR_GPR8_LPUART5_IPG_DOZE_MASK | IOMUXC_GPR_GPR8_LPUART6_IPG_DOZE_MASK | \
     IOMUXC_GPR_GPR8_LPUART7_IPG_DOZE_MASK | IOMUXC_GPR_GPR8_LPUART8_IPG_DOZE_MASK)

#define GPR8_STOP_MODE_BITS                                                                    \
    (IOMUXC_GPR_GPR8_LPI2C1_IPG_STOP_MODE_MASK | IOMUXC_GPR_GPR8_LPI2C2_IPG_STOP_MODE_MASK |   \
     IOMUXC_GPR_GPR8_LPI2C3_IPG_STOP_MODE_MASK | IOMUXC_GPR_GPR8_LPI2C4_IPG_STOP_MODE_MASK |   \
     IOMUXC_GPR_GPR8_LPSPI1_IPG_STOP_MODE_MASK | IOMUXC_GPR_GPR8_LPSPI2_IPG_STOP_MODE_MASK |   \
     IOMUXC_GPR_GPR8_LPSPI3_IPG_STOP_MODE_MASK | IOMUXC_GPR_GPR8_LPSPI4_IPG_STOP_MODE_MASK |   \
     IOMUXC_GPR_GPR8_LPUART2_IPG_STOP_MODE_MASK | IOMUXC_GPR_GPR8_LPUART3_IPG_STOP_MODE_MASK | \
     IOMUXC_GPR_GPR8_LPUART4_IPG_STOP_MODE_MASK | IOMUXC_GPR_GPR8_LPUART5_IPG_STOP_MODE_MASK | \
     IOMUXC_GPR_GPR8_LPUART6_IPG_STOP_MODE_MASK | IOMUXC_GPR_GPR8_LPUART7_IPG_STOP_MODE_MASK | \
     IOMUXC_GPR_GPR8_LPUART8_IPG_STOP_MODE_MASK)

#define GPR12_DOZE_BITS (IOMUXC_GPR_GPR12_FLEXIO1_IPG_DOZE_MASK | IOMUXC_GPR_GPR12_FLEXIO2_IPG_DOZE_MASK)

#define GPR12_STOP_MODE_BITS (IOMUXC_GPR_GPR12_FLEXIO1_IPG_STOP_MODE_MASK | IOMUXC_GPR_GPR12_FLEXIO2_IPG_STOP_MODE_MASK)

void PeripheralEnterDozeMode(void)
{
    IOMUXC_GPR->GPR8  = GPR8_DOZE_BITS;
    IOMUXC_GPR->GPR12 = GPR12_DOZE_BITS;
}

void PeripheralExitDozeMode(void)
{
    IOMUXC_GPR->GPR8  = 0x00000000;
    IOMUXC_GPR->GPR12 = 0x00000000;
}

void PeripheralEnterStopMode(void)
{
    IOMUXC_GPR->GPR4 = IOMUXC_GPR_GPR4_ENET_STOP_REQ_MASK;
    while ((IOMUXC_GPR->GPR4 & IOMUXC_GPR_GPR4_ENET_STOP_ACK_MASK) != IOMUXC_GPR_GPR4_ENET_STOP_ACK_MASK)
    {
    }
    IOMUXC_GPR->GPR4  = GPR4_STOP_REQ_BITS;
    IOMUXC_GPR->GPR7  = GPR7_STOP_REQ_BITS;
    IOMUXC_GPR->GPR8  = GPR8_DOZE_BITS | GPR8_STOP_MODE_BITS;
    IOMUXC_GPR->GPR12 = GPR12_DOZE_BITS | GPR12_STOP_MODE_BITS;
    while ((IOMUXC_GPR->GPR4 & GPR4_STOP_ACK_BITS) != GPR4_STOP_ACK_BITS)
    {
    }
    while ((IOMUXC_GPR->GPR7 & GPR7_STOP_ACK_BITS) != GPR7_STOP_ACK_BITS)
    {
    }
}

void APP_PrintRunFrequency(int32_t run_freq_only)
{
    printf("\r\n");
    printf("***********************************************************\r\n");
    printf("CPU:             %" PRIu32 " Hz\r\n", CLOCK_GetFreq(kCLOCK_CpuClk));
    printf("AHB:             %" PRIu32 " Hz\r\n", CLOCK_GetFreq(kCLOCK_AhbClk));
    printf("SEMC:            %" PRIu32 " Hz\r\n", CLOCK_GetFreq(kCLOCK_SemcClk));
    printf("IPG:             %" PRIu32 " Hz\r\n", CLOCK_GetFreq(kCLOCK_IpgClk));
    printf("PER:             %" PRIu32 " Hz\r\n", CLOCK_GetFreq(kCLOCK_PerClk));
    printf("OSC:             %" PRIu32 " Hz\r\n", CLOCK_GetFreq(kCLOCK_OscClk));
    printf("RTC:             %" PRIu32 " Hz\r\n", CLOCK_GetFreq(kCLOCK_RtcClk));
    if (!run_freq_only)
    {
        printf("ARMPLL:          %" PRIu32 " Hz\r\n", CLOCK_GetFreq(kCLOCK_ArmPllClk));
        printf("USB1PLL:         %" PRIu32 " Hz\r\n", CLOCK_GetFreq(kCLOCK_Usb1PllClk));
        printf("USB1PLLPFD0:     %" PRIu32 " Hz\r\n", CLOCK_GetFreq(kCLOCK_Usb1PllPfd0Clk));
        printf("USB1PLLPFD1:     %" PRIu32 " Hz\r\n", CLOCK_GetFreq(kCLOCK_Usb1PllPfd1Clk));
        printf("USB1PLLPFD2:     %" PRIu32 " Hz\r\n", CLOCK_GetFreq(kCLOCK_Usb1PllPfd2Clk));
        printf("USB1PLLPFD3:     %" PRIu32 " Hz\r\n", CLOCK_GetFreq(kCLOCK_Usb1PllPfd3Clk));
        printf("USB2PLL:         %" PRIu32 " Hz\r\n", CLOCK_GetFreq(kCLOCK_Usb2PllClk));
        printf("SYSPLL:          %" PRIu32 " Hz\r\n", CLOCK_GetFreq(kCLOCK_SysPllClk));
        printf("SYSPLLPFD0:      %" PRIu32 " Hz\r\n", CLOCK_GetFreq(kCLOCK_SysPllPfd0Clk));
        printf("SYSPLLPFD1:      %" PRIu32 " Hz\r\n", CLOCK_GetFreq(kCLOCK_SysPllPfd1Clk));
        printf("SYSPLLPFD2:      %" PRIu32 " Hz\r\n", CLOCK_GetFreq(kCLOCK_SysPllPfd2Clk));
        printf("SYSPLLPFD3:      %" PRIu32 " Hz\r\n", CLOCK_GetFreq(kCLOCK_SysPllPfd3Clk));
        printf("ENETPLL0:        %" PRIu32 " Hz\r\n", CLOCK_GetFreq(kCLOCK_EnetPll0Clk));
        printf("ENETPLL1:        %" PRIu32 " Hz\r\n", CLOCK_GetFreq(kCLOCK_EnetPll1Clk));
        printf("AUDIOPLL:        %" PRIu32 " Hz\r\n", CLOCK_GetFreq(kCLOCK_AudioPllClk));
        printf("VIDEOPLL:        %" PRIu32 " Hz\r\n", CLOCK_GetFreq(kCLOCK_VideoPllClk));
    }
    printf("***********************************************************\r\n");
    printf("\r\n");
}
