/***************************************************************************//**
* \file cyhal_rtc.c
*
* \brief
* Provides a high level interface for interacting with the Cypress Real-Time Clock.
* This interface abstracts out the chip specific details. If any chip specific
* functionality is necessary, or performance is critical the low level functions
* can be used directly.
*
********************************************************************************
* \copyright
* Copyright 2018-2019 Cypress Semiconductor Corporation
* SPDX-License-Identifier: Apache-2.0
*
* Licensed under the Apache License, Version 2.0 (the "License");
* you may not use this file except in compliance with the License.
* You may obtain a copy of the License at
*
*     http://www.apache.org/licenses/LICENSE-2.0
*
* Unless required by applicable law or agreed to in writing, software
* distributed under the License is distributed on an "AS IS" BASIS,
* WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
* See the License for the specific language governing permissions and
* limitations under the License.
*******************************************************************************/

#include "cy_rtc.h"
#include "cyhal_rtc.h"
#include "cyhal_system.h"

#ifdef CY_IP_MXS40SRSS_RTC_INSTANCES

#if defined(__cplusplus)
extern "C" {
#endif

#define CYHAL_RTC_STATE_UNINITIALIZED 0
#define CYHAL_RTC_STATE_ENABLED 1
#define CYHAL_RTC_STATE_TIME_SET 2
#define CYHAL_RTC_DEFAULT_PRIORITY 5
#define CYHAL_RTC_INIT_CENTURY 2000
#define CYHAL_TM_YEAR_BASE 1900

/** Wrapper around the PDL Cy_RTC_DeepSleepCallback to adapt the function signature */
static cy_en_syspm_status_t cyhal_rtc_syspm_callback(cy_stc_syspm_callback_params_t *params, cy_en_syspm_callback_mode_t mode)
{
    return Cy_RTC_DeepSleepCallback(params, mode);
}

static cy_stc_syspm_callback_params_t cyhal_rtc_pm_cb_params = {NULL, NULL};
static cy_stc_syspm_callback_t cyhal_rtc_pm_cb = {
    .callback = &cyhal_rtc_syspm_callback,
    .type = CY_SYSPM_DEEPSLEEP,
    .callbackParams = &cyhal_rtc_pm_cb_params,
};
static cyhal_rtc_event_callback_t cyhal_rtc_user_handler;
static void *cyhal_rtc_handler_arg;
#define CYHAL_RTC_INITIAL_CENTURY 1900
static uint16_t cyhal_rtc_century = CYHAL_RTC_INITIAL_CENTURY;
static uint8_t cyhal_rtc_initialized = CYHAL_RTC_STATE_UNINITIALIZED;

/** Wrapper around the PDL RTC interrupt handler to adapt the function signature */
static void cyhal_rtc_internal_handler(void)
{
    Cy_RTC_Interrupt(NULL, false);
}

void Cy_RTC_Alarm1Interrupt(void)
{
    if (NULL != cyhal_rtc_user_handler)
    {
        (*cyhal_rtc_user_handler)(cyhal_rtc_handler_arg, CYHAL_RTC_ALARM);
    }
}

void Cy_RTC_CenturyInterrupt(void)
{
    cyhal_rtc_century += 100;
}

cy_rslt_t cyhal_rtc_init(cyhal_rtc_t *obj)
{
    CY_ASSERT(NULL != obj);
    cy_rslt_t rslt = CY_RSLT_SUCCESS;
    if (cyhal_rtc_initialized == CYHAL_RTC_STATE_UNINITIALIZED)
    {
        if (Cy_RTC_IsExternalResetOccurred())
        {
            // Reset to default time
            static const cy_stc_rtc_config_t defaultTime = {
                .dayOfWeek = CY_RTC_THURSDAY,
                .date = 1,
                .month = 1,
                .year = 70
            };
            Cy_RTC_SetDateAndTime(&defaultTime);
        }
        else
        {
            // Time is already set (possibly after sw reset). Assume century.
            cyhal_rtc_century = CYHAL_RTC_INIT_CENTURY;
        }
        Cy_RTC_ClearInterrupt(CY_RTC_INTR_CENTURY);
        Cy_RTC_SetInterruptMask(CY_RTC_INTR_CENTURY);
        static const cy_stc_sysint_t irqCfg = {.intrSrc = srss_interrupt_backup_IRQn, .intrPriority = CYHAL_RTC_DEFAULT_PRIORITY};
        Cy_SysInt_Init(&irqCfg, &cyhal_rtc_internal_handler);
        Cy_SysPm_RegisterCallback(&cyhal_rtc_pm_cb);
        cyhal_rtc_initialized = CYHAL_RTC_STATE_ENABLED;
    }
    NVIC_EnableIRQ(srss_interrupt_backup_IRQn);
    return rslt;
}

void cyhal_rtc_free(cyhal_rtc_t *obj)
{
    NVIC_DisableIRQ(srss_interrupt_backup_IRQn);

    Cy_RTC_SetInterruptMask(CY_RTC_INTR_CENTURY);
}

bool cyhal_rtc_is_enabled(cyhal_rtc_t *obj)
{
    return (cyhal_rtc_initialized == CYHAL_RTC_STATE_TIME_SET);
}

cy_rslt_t cyhal_rtc_read(cyhal_rtc_t *obj, struct tm *time)
{
    // The number of days that precede each month of the year, not including Feb 29
    static const uint16_t CUMULATIVE_DAYS[] = {0, 31, 59, 90, 120, 151, 181, 212, 243, 273, 304, 334};
    cy_stc_rtc_config_t dateTime;
    uint32_t savedIntrStatus = cyhal_system_critical_section_enter();
    Cy_RTC_GetDateAndTime(&dateTime);
    int year = dateTime.year + cyhal_rtc_century;
    cyhal_system_critical_section_exit(savedIntrStatus);
    time->tm_sec = dateTime.sec;
    time->tm_min = dateTime.min;
    time->tm_hour = dateTime.hour;
    time->tm_mday = dateTime.date;
    time->tm_mon = dateTime.month - 1u;
    time->tm_year = year - CYHAL_TM_YEAR_BASE;
    time->tm_wday = dateTime.dayOfWeek - 1u;
    time->tm_yday = CUMULATIVE_DAYS[time->tm_mon] + dateTime.date - 1u +
        ((dateTime.month >= 3 && Cy_RTC_IsLeapYear(year)) ? 1u : 0u);
    time->tm_isdst = -1;
    return CY_RSLT_SUCCESS;
}

cy_rslt_t cyhal_rtc_write(cyhal_rtc_t *obj, const struct tm *time)
{
    uint32_t year2digit = time->tm_year % 100;
    cy_stc_rtc_config_t newtime = {
    .sec = time->tm_sec,
    .min = time->tm_min,
    .hour = time->tm_hour,
    .hrFormat = CY_RTC_24_HOURS,
    .dayOfWeek = time->tm_wday + 1,
    .date = time->tm_mday,
    .month = time->tm_mon + 1,
    .year = year2digit
    };
    cy_rslt_t rslt;
    uint32_t retry = 0;
    static const uint32_t MAX_RETRY = 10, RETRY_DELAY_MS = 1;
    do {
        if (retry != 0)
            Cy_SysLib_Delay(RETRY_DELAY_MS);
        uint32_t savedIntrStatus = cyhal_system_critical_section_enter();
        rslt = (cy_rslt_t)Cy_RTC_SetDateAndTime(&newtime);
        if (rslt == CY_RSLT_SUCCESS)
            cyhal_rtc_century = time->tm_year - year2digit + CYHAL_TM_YEAR_BASE;
        cyhal_system_critical_section_exit(savedIntrStatus);
        ++retry;
    } while (rslt == CY_RTC_INVALID_STATE && retry < MAX_RETRY);
    while (CY_RTC_BUSY == Cy_RTC_GetSyncStatus()) { }
    if (rslt == CY_RSLT_SUCCESS)
        cyhal_rtc_initialized = CYHAL_RTC_STATE_TIME_SET;
    return rslt;
}

cy_rslt_t cyhal_rtc_set_alarm(cyhal_rtc_t *obj, const struct tm *time, cyhal_alarm_active_t active)
{
    // Note: the hardware does not support year matching
    cy_stc_rtc_alarm_t alarm = {
    .sec = time->tm_sec,
    .secEn = active.en_sec ? CY_RTC_ALARM_ENABLE : CY_RTC_ALARM_DISABLE,
    .min = time->tm_min,
    .minEn = active.en_min ? CY_RTC_ALARM_ENABLE : CY_RTC_ALARM_DISABLE,
    .hour = time->tm_hour,
    .hourEn = active.en_hour ? CY_RTC_ALARM_ENABLE : CY_RTC_ALARM_DISABLE,
    .dayOfWeek = time->tm_wday + 1,
    .dayOfWeekEn = active.en_day ? CY_RTC_ALARM_ENABLE : CY_RTC_ALARM_DISABLE,
    .date = time->tm_mday,
    .dateEn = active.en_date ? CY_RTC_ALARM_ENABLE : CY_RTC_ALARM_DISABLE,
    .month = time->tm_mon + 1,
    .monthEn = active.en_month ? CY_RTC_ALARM_ENABLE : CY_RTC_ALARM_DISABLE,
    .almEn = CY_RTC_ALARM_ENABLE
    };
    return (cy_rslt_t)Cy_RTC_SetAlarmDateAndTime(&alarm, CY_RTC_ALARM_1);
}

void cyhal_rtc_register_callback(cyhal_rtc_t *obj, cyhal_rtc_event_callback_t callback, void *callback_arg)
{
    uint32_t savedIntrStatus = cyhal_system_critical_section_enter();
    cyhal_rtc_handler_arg = callback_arg;
    cyhal_rtc_user_handler = callback;
    cyhal_system_critical_section_exit(savedIntrStatus);
}

void cyhal_rtc_enable_event(cyhal_rtc_t *obj, cyhal_rtc_event_t event, uint8_t intrPriority, bool enable)
{
    Cy_RTC_ClearInterrupt(CY_RTC_INTR_ALARM1 | CY_RTC_INTR_ALARM2);
    Cy_RTC_SetInterruptMask((enable ? CY_RTC_INTR_ALARM1 : 0) | CY_RTC_INTR_CENTURY);
}

#if defined(__cplusplus)
}
#endif

#endif /* CY_IP_MXS40SRSS_RTC_INSTANCES */
