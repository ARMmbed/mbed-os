/*
 * Copyright (c) 2013 - 2014, Freescale Semiconductor, Inc.
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
#ifndef __FSL_WDOG_HAL_H__
#define __FSL_WDOG_HAL_H__

#include <assert.h>
#include <stdint.h>
#include <stdbool.h>
#include "fsl_wdog_features.h"
#include "fsl_device_registers.h"

/*! 
 * @addtogroup wdog_hal
 * @{
 */

/*******************************************************************************
 * Definitions
 *******************************************************************************/

/*! @brief Watchdog ISR callback function type*/
typedef void (*wdog_isr_callback_t)(void);

/*! @brief Watchdog clock source selection.*/
typedef enum _wdog_clock_source {
    kWdogDedicatedClock = 0, /*!< Dedicated clock source (LPO Oscillator),1K HZ.*/
    kWdogAlternateClock = 1 /*!< Alternate clock source, Bus clock.*/
} wdog_clock_source_t;

/*! @brief Define the selection of the clock prescaler*/
typedef enum _wdog_clock_prescaler {
    kWdogClockPrescaler1 = 0, /*!< Divide 1, default*/
    kWdogClockPrescaler2 = 1, /*!< Divide 2*/
    kWdogClockPrescaler3 = 2, /*!< Divide 3*/
    kWdogClockPrescaler4 = 3, /*!< Divide 4*/
    kWdogClockPrescaler5 = 4, /*!< Divide 5*/
    kWdogClockPrescaler6 = 5, /*!< Divide 6*/
    kWdogClockPrescaler7 = 6, /*!< Divide 7*/
    kWdogClockPrescaler8 = 7, /*!< Divide 8*/
} wdog_clock_prescaler_t;

/*******************************************************************************
 * API
 *******************************************************************************/

#if defined(__cplusplus)
extern "C" {
#endif

/*! 
 * @name Watchdog HAL.
 * @{
 */

/*!
 * @brief Enable watchdog module.
 *
 * This function is used to enable the WDOG and must be called after all 
 * necessary configure have been set.
 *
 */
void wdog_hal_enable(void);

/*!
 * @brief Disable watchdog module.
 * 
 * This function is used to disable the WDOG.
 *
 */
void wdog_hal_disable(void);

/*!
 * @brief Check whether WDOG is enabled.
 * 
 * This function is used check whether WDOG is enabled.
 *
 * @return 0 means WDOG is disabled, 1 means WODG is enabled.
 *
 */
static inline bool wdog_hal_is_enabled(void)
{
    return BR_WDOG_STCTRLH_WDOGEN;
}

/*!
 * @brief Enable and disable watchdog interrupt.
 *
 * This function is used to configure the WDOG interrupt.
 * The configuration is saved in an internal configure buffer and written back to the register in wdog_hal_enable 
 * function, so this function must be called before wdog_hal_enable is called.
 *
 * @param isEnabled 0 means disable watchdog interrupt. 1 means enable watchdog interrupt.
 */
void wdog_hal_configure_interrupt(bool isEnabled);

/*!
 * @brief Check whether WDOG interrupt is enabled.
 *
 * This function is used to check whether the WDOG interrupt is enabled.
 *
 * @return 0 means interrupt is disabled, 1 means interrupt is enabled.
 */
static inline bool wdog_hal_is_interrupt_enabled(void)
{
    return BR_WDOG_STCTRLH_IRQRSTEN;
}

/*!
 * @brief set watchdog clock Source.
 *
 * This function is used to set the WDOG clock source. There are two clock sources that can be used,
 * the LPO clock and the bus clock.
 * The configuration is saved in an internal configure buffer and written back to the register in wdog_hal_enable 
 * function, so this function must be called before wdog_hal_enable is called.
 *
 * @param clockSource watchdog clock source, see wdog_clock_source_t.
 */
void wdog_hal_set_clock_source(wdog_clock_source_t clockSource);

/*!
 * @brief Get watchdog clock Source.
 *
 * This function is used to get the WDOG clock source. There are two clock sources that can be used,
 * the LPO clock and the bus clock.
 *
 * @return watchdog clock source, see wdog_clock_source_t.
 */
static inline wdog_clock_source_t wdog_hal_get_clock_source(void)
{
    return (wdog_clock_source_t)BR_WDOG_STCTRLH_CLKSRC;
}

/*!
 * @brief Enable and disable watchdog window mode.
 *
 * This function is used to configure the WDOG window mode.
 * The configuration is saved in an internal configure buffer and written back to the register in wdog_hal_enable 
 * function, so this function must be called before wdog_hal_enable is called.
 *
 * @param isEnabled 0 means disable watchdog window mode. 1 means enable watchdog window mode.
 */
void wdog_hal_configure_window_mode(bool isEnabled);

/*!
 * @brief Check whether window mode is enabled.
 *
 * This function is used to check whether the WDOG window mode is enabled.
 *
 * @return 0 means window mode is disabled, 1 means window mode is enabled.
 */
static inline bool wdog_hal_is_window_mode_enabled(void)
{
    return BR_WDOG_STCTRLH_WINEN;
}

/*!
 * @brief Enable and disable watchdog write-once-only register update.
 *
 * This function is used to configure the WDOG register update feature. If disabled, it means that
 * all WDOG registers will never be written again unless Power On Reset.
 * The configuration is saved in an internal configure buffer and written back to the register in wdog_hal_enable 
 * function, so this function must be called before wdog_hal_enable is called.
 *
 * @param isEnabled 0 means disable watchdog write-once-only register update.
 *                  1 means enable watchdog write-once-only register update.
 */
void wdog_hal_configure_register_update(bool isEnabled);

/*!
 * @brief Check whether register update is enabled.
 *
 * This function is used to check whether the WDOG register update is enabled.
 *
 * @return 0 means register update is disabled, 1 means register update is enabled.
 */
static inline bool wdog_hal_is_register_update_enabled(void)
{
    return BR_WDOG_STCTRLH_ALLOWUPDATE;
}

/*!
 * @brief Set whether watchdog is working while cpu is in debug mode.
 *
 * This function is used to configure whether the WDOG is enabled in CPU debug mode. 
 * The configuration is saved in an internal configure buffer and written back to the register in wdog_hal_enable 
 * function, so this function must be called before wdog_hal_enable is called.
 *
 * @param isEnabled 0 means watchdog is disabled in CPU debug mode.
 *                  1 means watchdog is enabled in CPU debug mode.
 */
void wdog_hal_configure_enabled_in_cpu_debug_mode(bool isEnabled);

/*!
 * @brief Check whether WDOG works while in CPU debug mode.
 *
 * This function is used to check whether WDOG works in CPU debug mode.
 *
 * @return 0 means not work while in cpu debug mode, 1 means works while in cpu debug mode.
 */
static inline bool wdog_hal_is_cpu_debug_mode_enabled(void)
{
    return BR_WDOG_STCTRLH_DBGEN;
}

/*!
 * @brief Set whether watchdog is working while cpu is in stop mode.
 *
 * This function is used to configure whether the WDOG is enabled in CPU stop mode. 
 * The configuration is saved in an internal configure buffer and written back to the register in wdog_hal_enable 
 * function, so this function must be called before wdog_hal_enable is called.
 *
 * @param isEnabled 0 means watchdog is disabled in CPU stop mode.
 *                  1 means watchdog is enabled in CPU stop mode.
 */
void wdog_hal_configure_enabled_in_cpu_stop_mode(bool isEnabled);

/*!
 * @brief Check whether WDOG works while in CPU stop mode.
 *
 * This function is used to check whether WDOG works in CPU stop mode.
 *
 * @return 0 means not work while in CPU stop mode, 1 means works while in CPU stop mode.
 */
static inline bool wdog_hal_is_cpu_stop_mode_enabled(void)
{
    return BR_WDOG_STCTRLH_STOPEN;
}

/*!
 * @brief Set whether watchdog is working while CPU is in wait mode.
 *
 * This function is used to configure whether the WDOG is enabled in CPU wait mode. 
 * The configuration is saved in an internal configure buffer and written back to the register in wdog_hal_enable 
 * function, so this function must be called before wdog_hal_enable is called.
 *
 * @param isEnabled 0 means watchdog is disabled in CPU wait mode.
 *                  1 means watchdog is enabled in CPU wait mode.
 */
void wdog_hal_configure_enabled_in_cpu_wait_mode(bool isEnabled);

/*!
 * @brief Check whether WDOG works while in CPU wait mode.
 *
 * This function is used to check whether WDOG works in CPU wait mode.
 *
 * @return 0 means not work while in CPU wait mode, 1 means works while in CPU wait mode.
 */

static inline bool wdog_hal_is_cpu_wait_mode_enabled(void)
{
    return BR_WDOG_STCTRLH_WAITEN;
}

/*!
 * @brief Get watchdog interrupt status.
 *
 * This function is used to get the WDOG interrupt flag.
 *
 * @return Watchdog interrupt status, 0 means interrupt not asserted, 1 means interrupt asserted.
 */
static inline bool wdog_hal_is_interrupt_asserted(void)
{
    return BR_WDOG_STCTRLL_INTFLG;
}

/*!
 * @brief Clear watchdog interrupt flag.
 *
 * This function is used to clear the WDOG interrupt flag.
 *
 */
static inline void wdog_hal_clear_interrupt_flag(void)
{
    BW_WDOG_STCTRLL_INTFLG(1);
}

/*!
 * @brief set watchdog timeout value.
 *
 * This function is used to set the WDOG_TOVAL value.
 *
 * @param timeoutCount watchdog timeout value, count of watchdog clock tick.
 */
static inline void wdog_hal_set_timeout_value(uint32_t timeoutCount)
{
    BW_WDOG_TOVALH_TOVALHIGH((uint16_t)((timeoutCount >> 16U) & 0xFFFFU));
    BW_WDOG_TOVALL_TOVALLOW((uint16_t)((timeoutCount) & 0xFFFFU));
}

/*!
 * @brief Get watchdog timeout value.
 *
 * This function is used to Get the WDOG_TOVAL value.
 *
 * @return value of register WDOG_TOVAL.
 */
static inline uint32_t wdog_hal_get_timeout_value(void)
{
    return (uint32_t)((((uint32_t)(BR_WDOG_TOVALH_TOVALHIGH)) << 16U) | (BR_WDOG_TOVALL_TOVALLOW));
}

/*!
 * @brief Get watchdog timer output.
 *
 * This function is used to get the WDOG_TMROUT value.
 *
 * @return Current value of watchdog timer counter.
 */
static inline uint32_t wdog_hal_get_timer_output(void)
{
    return (uint32_t)((((uint32_t)(BR_WDOG_TMROUTH_TIMEROUTHIGH)) << 16U) | (BR_WDOG_TMROUTL_TIMEROUTLOW));
}

/*!
 * @brief Set watchdog clock prescaler.
 *
 * This function is used to set the WDOG clock proscaler.
 * 
 * @param clockPrescaler watchdog clock prescaler, see wdog_clock_prescaler_t.
 */
static inline void wdog_hal_set_clock_prescaler(wdog_clock_prescaler_t clockPrescaler)
{
    BW_WDOG_PRESC_PRESCVAL(clockPrescaler);
}

/*!
 * @brief Get watchdog clock prescaler.
 *
 * This function is used to get the WDOG clock prescaler.
 * 
 * @return WDOG clock prescaler.
 */
static inline wdog_clock_prescaler_t wdog_hal_get_clock_prescaler(void)
{
    return (wdog_clock_prescaler_t)BR_WDOG_PRESC_PRESCVAL;
}

/*!
 * @brief Set watchdog window value.
 *
 * This function is used to set the WDOG_WIN value.
 *
 * @param windowValue watchdog window value.
 */
static inline void wdog_hal_set_window_value(uint32_t windowValue)
{
    BW_WDOG_WINH_WINHIGH((uint16_t)((windowValue>>16U) & 0xFFFFU));
    BW_WDOG_WINL_WINLOW((uint16_t)((windowValue) & 0xFFFFU));
}

/*!
 * @brief Get watchdog window value.
 *
 * This function is used to Get the WDOG_WIN value.
 *
 * @return watchdog window value.
 */
static inline uint32_t wdog_hal_get_window_value(void)
{
    return (uint32_t)((((uint32_t)(BR_WDOG_WINH_WINHIGH)) << 16U) | (BR_WDOG_WINL_WINLOW));
}

/*!
 * @brief Unlock watchdog register written.
 * 
 * This function is used to unlock the WDOG register written.
 * This function must be called before any configuration is set because watchdog register
 * will be locked automatically after a WCT(256 bus cycles).
 *
 */
static inline void wdog_hal_unlock(void)
{
    BW_WDOG_UNLOCK_WDOGUNLOCK(0xC520U);
    BW_WDOG_UNLOCK_WDOGUNLOCK(0xD928U);
}

/*!
 * @brief Refresh watchdog timer.
 *
 * This function is used to feed the WDOG.
 * This function should be called before watchdog timer is in timeout, otherwise a RESET
 * will assert.
 *
 */
static inline void wdog_hal_refresh(void)
{
    BW_WDOG_REFRESH_WDOGREFRESH(0xA602U);
    BW_WDOG_REFRESH_WDOGREFRESH(0xB480U);
}

/*!
 * @brief Reset chip using watchdog.
 *
 * This function is used to reset chip using WDOG.
 *
 */
static inline void wdog_hal_reset_chip(void)
{
    BW_WDOG_REFRESH_WDOGREFRESH(0xA602U);
    BW_WDOG_REFRESH_WDOGREFRESH(0);
    while(1)
    {
    }
}

/*!
 * @brief Get chip reset count that was reset by watchdog.
 *
 * This function is used to get the value of WDOG_RSTCNT.
 *
 * @return Chip reset count that was reset by watchdog.
 */
static inline uint32_t wdog_hal_get_reset_count(void)
{
    return BR_WDOG_RSTCNT_RSTCNT;
}

/*!
 * @brief Clear chip reset count that was reset by watchdog.
 *
 * This function is used to clear the WDOG_RSTCNT.
 *
 */
static inline void wdog_hal_clear_reset_count(void)
{
    BW_WDOG_RSTCNT_RSTCNT(0xFFFFU);
}
/*@}*/

#if defined(__cplusplus)
}
#endif

/*! @}*/

#endif /* __FSL_WDOG_HAL_H__*/
/*******************************************************************************
 * EOF
 *******************************************************************************/

