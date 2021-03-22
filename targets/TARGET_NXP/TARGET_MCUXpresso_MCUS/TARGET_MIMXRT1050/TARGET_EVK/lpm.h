/*
 * Copyright 2017 NXP
 * All rights reserved.
 *
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef _LPM_H_
#define _LPM_H_

#include "fsl_clock.h"
#include <stdint.h>

/*******************************************************************************
 * Definitions
 ******************************************************************************/
extern void vPortGPTIsr(void);
extern uint32_t g_savedPrimask;

#define vPortGptIsr GPT1_IRQHandler

#define CLOCK_CCM_HANDSHAKE_WAIT() \
                                   \
    do                             \
    {                              \
        while (CCM->CDHIPR != 0)   \
        {                          \
        }                          \
                                   \
    } while (0)

#if defined(XIP_EXTERNAL_FLASH) && (XIP_EXTERNAL_FLASH == 1)
#define LPM_EnterCritical()                        \
                                                   \
    do                                             \
    {                                              \
        g_savedPrimask = DisableGlobalIRQ();       \
        SysTick->CTRL &= ~SysTick_CTRL_ENABLE_Msk; \
                                                   \
    } while (0)

#define LPM_ExitCritical()                        \
                                                  \
    do                                            \
    {                                             \
        EnableGlobalIRQ(g_savedPrimask);          \
        SysTick->CTRL |= SysTick_CTRL_ENABLE_Msk; \
                                                  \
    } while (0)

#else
#define LPM_EnterCritical()
#define LPM_ExitCritical()
#endif

/* Power mode definition of low power management.
 * Waken up duration Off > Dsm > Idle > Wait > Run.
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

/* Initialize the Low Power Management */
bool LPM_Init(void);

/* Deinitialize the Low Power Management */
void LPM_Deinit(void);

/* Enable wakeup source in low power mode */
void LPM_EnableWakeupSource(uint32_t irq);

/* Disable wakeup source in low power mode */
void LPM_DisableWakeupSource(uint32_t irq);

void ClockSelectXtalOsc(void);
void ClockSelectRcOsc(void);
void LPM_EnableWakeupSource(uint32_t irq);
void LPM_DisableWakeupSource(uint32_t irq);
void LPM_PreEnterWaitMode(void);
void LPM_PostExitWaitMode(void);
void LPM_PreEnterStopMode(void);
void LPM_PostExitStopMode(void);
void LPM_OverDriveRun(void);
void LPM_FullSpeedRun(void);
void LPM_LowSpeedRun(void);
void LPM_LowPowerRun(void);
void LPM_EnterSystemIdle(void);
void LPM_ExitSystemIdle(void);
void LPM_EnterLowPowerIdle(void);
void LPM_ExitLowPowerIdle(void);
void LPM_EnterSuspend(void);
void LPM_EnterSNVS(void);

#if defined(__cplusplus)
}
#endif /* __cplusplus*/

#endif /* _LPM_H_ */
