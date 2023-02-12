/*
 * Copyright 2020 NXP
 * All rights reserved.
 *
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef _LPM_H_
#define _LPM_H_

#include <stdint.h>
#include "fsl_clock.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/

#define LPM_EnterCritical()
#define LPM_ExitCritical()

/*
 * Power mode definition of low power management.
 */
typedef enum _lpm_power_mode
{
    LPM_PowerModeOverRun = 0, /* Over RUN mode, CPU won't stop running */

    LPM_PowerModeFullRun, /* Full RUN mode, CPU won't stop running */

    LPM_PowerModeLowSpeedRun,

    LPM_PowerModeLowPowerRun,

    LPM_PowerModeRunEnd = LPM_PowerModeLowPowerRun,
    /* In system wait mode, cpu clock is gated.
     * All peripheral can remain active, clock gating decided by CCGR setting.
     * DRAM enters auto-refresh mode when there is no access.
     */
    LPM_PowerModeSysIdle, /* System WAIT mode, also system low speed idle */

    /* In low power idle mode, all PLL/PFD is off, cpu power is off.
     * Analog modules running in low power mode.
     * All high-speed peripherals are power gated
     * Low speed peripherals can remain running at low frequency
     * DRAM in self-refresh.
     */
    LPM_PowerModeLPIdle, /* Low Power Idle mode */

    /* In deep sleep mode, all PLL/PFD is off, XTAL is off, cpu power is off.
     * All clocks are shut off except 32K RTC clock
     * All high-speed peripherals are power gated
     * Low speed peripherals are clock gated
     * DRAM in self-refresh.
     * If RTOS is used, systick will be disabled in DSM
     */
    LPM_PowerModeSuspend, /* Deep Sleep mode, suspend. */

    LPM_PowerModeSNVS, /* Power off mode, or shutdown mode */

    LPM_PowerModeEnd = LPM_PowerModeSNVS
} lpm_power_mode_t;

/*******************************************************************************
 * API
 ******************************************************************************/

#if defined(__cplusplus)
extern "C" {
#endif /* __cplusplus*/

AT_QUICKACCESS_SECTION_CODE(void CLOCK_SET_MUX(clock_mux_t mux, uint32_t value));
AT_QUICKACCESS_SECTION_CODE(void CLOCK_SET_DIV(clock_div_t divider, uint32_t value));
AT_QUICKACCESS_SECTION_CODE(void LPM_EnterSleepMode(clock_mode_t mode));
void LPM_Init(void);
void LPM_EnableWakeupSource(uint32_t irq);
void LPM_DisableWakeupSource(uint32_t irq);
void LPM_OverDriveRun(lpm_power_mode_t curRunMode);
void LPM_FullSpeedRun(lpm_power_mode_t curRunMode);
void LPM_LowSpeedRun(lpm_power_mode_t curRunMode);
void LPM_LowPowerRun(lpm_power_mode_t curRunMode);
void LPM_EnterSystemIdle(lpm_power_mode_t curRunMode);
void LPM_ExitSystemIdle(lpm_power_mode_t curRunMode);
void LPM_EnterLowPowerIdle(lpm_power_mode_t curRunMode);
void LPM_ExitLowPowerIdle(lpm_power_mode_t curRunMode);
void LPM_EnterSuspend(void);
void LPM_EnterSNVS(void);

#if defined(__cplusplus)
}
#endif /* __cplusplus*/

#endif /* _LPM_H_ */
