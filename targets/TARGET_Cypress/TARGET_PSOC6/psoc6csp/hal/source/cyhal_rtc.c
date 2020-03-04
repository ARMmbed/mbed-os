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
* Copyright 2018-2020 Cypress Semiconductor Corporation
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
#include "cy_utils.h"
#include "cyhal_rtc.h"
#include "cyhal_system.h"

/**
* \addtogroup group_hal_psoc6_rtc RTC (Real Time Clock)
* \ingroup group_hal_psoc6
* \{
*
* Internally the PSoC6 RTC only stores the year as a two digit BCD value
* (0-99); no century information is stored. On RTC initialization the HAL must,
* as a result, assume a default century. If cyhal_rtc_write has been called
* with a different century than the default, its value must be stored and that
* value must persist through deep sleep, hibernate, software resets, etc. PSoC6
* hardware provides a number of BREG registers which exist in the BACKUP domain
* and will persist over these power modes and resets. The HAL uses the highest
* indexed BACKUP->BREG register to store the century for the RTC.
*
* Therefore do not use the highest indexed BACKUP->BREG register as it is
* reserved for internal HAL usage.
* \} group_hal_psoc6_wdt
*/

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

#define CYHAL_RTC_BREG (BACKUP->BREG[SRSS_BACKUP_NUM_BREG-1])
#define CYHAL_RTC_BREG_CENTURY_Pos 0UL
#define CYHAL_RTC_BREG_CENTURY_Msk 0x0000FFFFUL
#define CYHAL_RTC_BREG_STATE_Pos 16UL
#define CYHAL_RTC_BREG_STATE_Msk 0xFFFF0000UL

/** Wrapper around the PDL Cy_RTC_DeepSleepCallback to adapt the function signature */
static cy_en_syspm_status_t cyhal_rtc_syspm_callback(cy_stc_syspm_callback_params_t *params, cy_en_syspm_callback_mode_t mode)
{
    return Cy_RTC_DeepSleepCallback(params, mode);
}

static cy_stc_rtc_dst_t *dst;
static cy_stc_syspm_callback_params_t cyhal_rtc_pm_cb_params = {NULL, NULL};
static cy_stc_syspm_callback_t cyhal_rtc_pm_cb = {
    .callback = &cyhal_rtc_syspm_callback,
    .type = CY_SYSPM_DEEPSLEEP,
    .callbackParams = &cyhal_rtc_pm_cb_params,
};

static cyhal_rtc_event_callback_t cyhal_rtc_user_handler;
static void *cyhal_rtc_handler_arg;

/* Returns century portion of BREG register used to store century info */
static inline uint16_t get_rtc_century()
{
    return _FLD2VAL(CYHAL_RTC_BREG_CENTURY, CYHAL_RTC_BREG);
}

/* Sets century portion of BREG register used to store century info */
static inline void set_rtc_century(uint16_t century)
{
    CYHAL_RTC_BREG &= CYHAL_RTC_BREG_STATE_Msk;
    CYHAL_RTC_BREG |= _VAL2FLD(CYHAL_RTC_BREG_CENTURY, century);
}

/* Returns state portion of BREG register used to store century info */
static inline uint16_t get_rtc_state()
{
    return _FLD2VAL(CYHAL_RTC_BREG_STATE, CYHAL_RTC_BREG);
}

/* Sets state portion of BREG register used to store century info */
static inline void set_rtc_state(uint16_t init)
{
    CYHAL_RTC_BREG &= CYHAL_RTC_BREG_CENTURY_Msk;
    CYHAL_RTC_BREG |= _VAL2FLD(CYHAL_RTC_BREG_STATE, init);
}

/** Wrapper around the PDL RTC interrupt handler to adapt the function signature */
static void cyhal_rtc_internal_handler(void)
{
    Cy_RTC_Interrupt(dst, NULL != dst);
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
    set_rtc_century(get_rtc_century() + 100);
}

cy_rslt_t cyhal_rtc_init(cyhal_rtc_t *obj)
{
    CY_ASSERT(NULL != obj);
    cy_rslt_t rslt = CY_RSLT_SUCCESS;
    if (get_rtc_state() == CYHAL_RTC_STATE_UNINITIALIZED)
    {
        if (Cy_RTC_IsExternalResetOccurred())
        {
            // Reset to default time
            static const cy_stc_rtc_config_t defaultTime = {
                .dayOfWeek = CY_RTC_SATURDAY,
                .date = 1,
                .month = 1,
                .year = 0,
            };
            Cy_RTC_SetDateAndTime(&defaultTime);
            set_rtc_century(CYHAL_RTC_INIT_CENTURY);
        }

        if (Cy_SysPm_RegisterCallback(&cyhal_rtc_pm_cb))
        {
            set_rtc_state(CYHAL_RTC_STATE_ENABLED);
        }
        else
        {
            rslt = CY_RSLT_RTC_NOT_INITIALIZED;
        }
    }
    else if(get_rtc_state() == CYHAL_RTC_STATE_ENABLED || get_rtc_state() == CYHAL_RTC_STATE_TIME_SET)
    {
        if(Cy_RTC_GetInterruptStatus() & CY_RTC_INTR_CENTURY)
            Cy_RTC_CenturyInterrupt();
    }

    Cy_RTC_ClearInterrupt(CY_RTC_INTR_CENTURY);
    Cy_RTC_SetInterruptMask(CY_RTC_INTR_CENTURY);
    static const cy_stc_sysint_t irqCfg = {.intrSrc = srss_interrupt_backup_IRQn, .intrPriority = CYHAL_RTC_DEFAULT_PRIORITY};
    Cy_SysInt_Init(&irqCfg, &cyhal_rtc_internal_handler);

    if (rslt == CY_RSLT_SUCCESS)
    {
        dst = NULL;
        NVIC_EnableIRQ(srss_interrupt_backup_IRQn);
    }

    return rslt;
}

void cyhal_rtc_free(cyhal_rtc_t *obj)
{
    CY_ASSERT(NULL != obj);
    NVIC_DisableIRQ(srss_interrupt_backup_IRQn);

    Cy_RTC_SetInterruptMask(CY_RTC_INTR_CENTURY);
    dst = NULL;
}

bool cyhal_rtc_is_enabled(cyhal_rtc_t *obj)
{
    CY_ASSERT(NULL != obj);
    return (get_rtc_state() == CYHAL_RTC_STATE_TIME_SET);
}

cy_rslt_t cyhal_rtc_read(cyhal_rtc_t *obj, struct tm *time)
{
    CY_ASSERT(NULL != obj);
    // The number of days that precede each month of the year, not including Feb 29
    static const uint16_t CUMULATIVE_DAYS[] = {0, 31, 59, 90, 120, 151, 181, 212, 243, 273, 304, 334};

    cy_stc_rtc_config_t dateTime;
    uint32_t savedIntrStatus = cyhal_system_critical_section_enter();
    Cy_RTC_GetDateAndTime(&dateTime);
    int year = dateTime.year + get_rtc_century();
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
    CY_ASSERT(NULL != obj);
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
            set_rtc_century(time->tm_year - year2digit + CYHAL_TM_YEAR_BASE);
        cyhal_system_critical_section_exit(savedIntrStatus);
        ++retry;
    } while (rslt == CY_RTC_INVALID_STATE && retry < MAX_RETRY);

    while (CY_RTC_BUSY == Cy_RTC_GetSyncStatus()) { }

    if (rslt == CY_RSLT_SUCCESS)
        set_rtc_state(CYHAL_RTC_STATE_TIME_SET);
    return rslt;
}

static void initialize_dst(const cyhal_rtc_dst_t *hal, cy_stc_rtc_dst_format_t *pdl)
{
    pdl->format = (hal->format == CYHAL_RTC_DST_FIXED) ? CY_RTC_DST_FIXED : CY_RTC_DST_RELATIVE;
    pdl->hour = hal->hour;
    pdl->dayOfMonth = (hal->format == CYHAL_RTC_DST_FIXED) ? hal->dayOfMonth : 1;
    pdl->weekOfMonth = (hal->format == CYHAL_RTC_DST_FIXED) ? 1 : hal->weekOfMonth + 1;
    pdl->dayOfWeek = (hal->format == CYHAL_RTC_DST_FIXED) ? 1 : hal->dayOfWeek + 1;
    pdl->month = hal->month;
}

cy_rslt_t cyhal_rtc_set_dst(cyhal_rtc_t *obj, const cyhal_rtc_dst_t *start, const cyhal_rtc_dst_t *stop)
{
    CY_ASSERT(NULL != obj);
    CY_ASSERT(NULL != start);
    CY_ASSERT(NULL != stop);

    initialize_dst(start, &(obj->dst.startDst));
    initialize_dst(stop, &(obj->dst.stopDst));

    cy_stc_rtc_config_t dateTime;
    Cy_RTC_GetDateAndTime(&dateTime);
    cy_rslt_t rslt = Cy_RTC_EnableDstTime(&(obj->dst), &dateTime);
    if (rslt == CY_RSLT_SUCCESS)
        dst = &(obj->dst);
    return rslt;
}

bool cyhal_rtc_is_dst(cyhal_rtc_t *obj)
{
    CY_ASSERT(NULL != obj);

    cy_stc_rtc_config_t dateTime;
    Cy_RTC_GetDateAndTime(&dateTime);
    return Cy_RTC_GetDstStatus(&(obj->dst), &dateTime);
}

cy_rslt_t cyhal_rtc_set_alarm(cyhal_rtc_t *obj, const struct tm *time, cyhal_alarm_active_t active)
{
    // Note: the hardware does not support year matching
    CY_ASSERT(NULL != obj);
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
    CY_ASSERT(NULL != obj);
    uint32_t savedIntrStatus = cyhal_system_critical_section_enter();
    cyhal_rtc_handler_arg = callback_arg;
    cyhal_rtc_user_handler = callback;
    cyhal_system_critical_section_exit(savedIntrStatus);
}

void cyhal_rtc_enable_event(cyhal_rtc_t *obj, cyhal_rtc_event_t event, uint8_t intr_priority, bool enable)
{
    CY_ASSERT(NULL != obj);
    CY_ASSERT(CYHAL_RTC_ALARM == event);
    Cy_RTC_ClearInterrupt(CY_RTC_INTR_ALARM1 | CY_RTC_INTR_ALARM2);
    Cy_RTC_SetInterruptMask((enable ? CY_RTC_INTR_ALARM1 : 0) | CY_RTC_INTR_CENTURY);
    NVIC_SetPriority(srss_interrupt_backup_IRQn, intr_priority);
}

#if defined(__cplusplus)
}
#endif

#endif /* CY_IP_MXS40SRSS_RTC_INSTANCES */
