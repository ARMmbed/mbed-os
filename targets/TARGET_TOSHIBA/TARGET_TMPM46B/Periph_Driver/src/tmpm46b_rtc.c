/**
 *******************************************************************************
 * @file    tmpm46b_rtc.c
 * @brief   This file provides API functions for RTC driver.
 * @version V2.0.2.1
 * @date    2015/02/11
 * 
 * DO NOT USE THIS SOFTWARE WITHOUT THE SOFTWARE LICENSE AGREEMENT.
 * 
 * (C)Copyright TOSHIBA ELECTRONIC DEVICES & STORAGE CORPORATION 2017 All rights reserved
 *******************************************************************************
 */

/* Includes ------------------------------------------------------------------*/
#include "tmpm46b_rtc.h"

#if defined(__TMPM46B_RTC_H)

/** @addtogroup TX04_Periph_Driver
  * @{
  */

/** @defgroup RTC 
  * @brief RTC driver modules
  * @{
  */

/** @defgroup RTC_Private_Defines
  * @{
  */

#define PAGER_PAGE_SET         ((uint8_t)0x01)
#define PAGER_PAGE_CLEAR       ((uint8_t)0xFE)
#define PAGER_ADJUST_SET       ((uint8_t)0x10)
#define PAGER_ADJUST_CLEAR     ((uint8_t)0xEF)
#define PAGER_ENATMR_SET       ((uint8_t)0x08)
#define PAGER_ENATMR_CLEAR     ((uint8_t)0xF7)
#define PAGER_ENAALM_SET       ((uint8_t)0x04)
#define PAGER_ENAALM_CLEAR     ((uint8_t)0xFB)
#define PAGER_INTENA_SET       ((uint8_t)0x80)
#define PAGER_INTENA_CLEAR     ((uint8_t)0x7F)

#define RESTR_DIS_CLEAR        ((uint8_t)0x38)
#define RESTR_DIS_SET          ((uint8_t)0xC7)
#define RESTR_DIS1HZ_SET       ((uint8_t)0x80)
#define RESTR_DIS16HZ_SET      ((uint8_t)0x40)
#define RESTR_DIS2HZ_SET       ((uint8_t)0x04)
#define RESTR_DIS4HZ_SET       ((uint8_t)0x02)
#define RESTR_DIS8HZ_SET       ((uint8_t)0x01)
#define RESTR_RSTTMR_SET       ((uint8_t)0x20)
#define RESTR_RSTTMR_CLEAR     ((uint8_t)0xDF)
#define RESTR_RSTALM_SET       ((uint8_t)0x10)
#define RESTR_RSTALM_CLEAR     ((uint8_t)0xEF)

#define HOURR_HO5_SET          ((uint8_t)0x20)
#define HOURR_HO5_CLEAR        ((uint8_t)0xDF)

#define MONTHR_MO0_MASK        ((uint8_t)0x01)

#define RTC_PROTECT_SET        ((uint8_t)0xC1)
#define RTC_PROTECT_CLEAR      ((uint8_t)0x3E)

#define ADJCTL_AJEN_CLEAR      ((uint8_t)0xFE)
#define ADJCTL_AJEN_SET        ((uint8_t)0x01)

#define ADJCTL_AJSEL_CLEAR     ((uint8_t)0xF1)

#define ADJDAT_CLEAR           ((uint16_t)0x01FF)
#define ADJDAT_MODE_SET        ((uint16_t)0x0100)

/** @} */
/* End of group RTC_Private_Defines */


/** @defgroup RTC_Private_FunctionPrototypes
  * @{
  */

/** @} */
/* End of group RTC_Private_FunctionPrototypes */

/** @defgroup RTC_Private_Functions
  * @{
  */

/** @} */
/* End of group RTC_Private_Functions */

/** @defgroup RTC_Exported_Functions
  * @{
  */

/**
  * @brief  Set second value.
  * @param  Sec: New second value, max 59
  *   This parameter can be one of the following values:
  *   0, 1, 2, ...59 
  * @retval None
  */
void RTC_SetSec(uint8_t Sec)
{
    uint8_t tmp = 0U;

    /* Check the parameters */
    assert_param(IS_RTC_SECOND(Sec));

    /* Select PAGE0 */
    tmp = TSB_RTC->PAGER;
    tmp &= PAGER_PAGE_CLEAR;
    TSB_RTC->PAGER = tmp;
    tmp = (uint8_t) ((Sec / 10U) * 16U + Sec % 10U);
    /* Write sec value into SECR register */
    TSB_RTC->SECR = tmp;
}

/**
  * @brief  Get second value.
  * @param  None
  * @retval The second value.
  */
uint8_t RTC_GetSec(void)
{
    uint8_t sec = 0U;
    uint8_t tmp = 0U;

    /* Select PAGE0 */
    tmp = TSB_RTC->PAGER;
    tmp &= PAGER_PAGE_CLEAR;
    TSB_RTC->PAGER = tmp;

    do {
        tmp = TSB_RTC->SECR;
        sec = TSB_RTC->SECR;
    }
    while (tmp != sec);

    sec = (uint8_t) ((tmp >> 4U) * 10U + tmp % 16U);

    /* return second value  */
    return sec;
}

/**
  * @brief  Set minute value.
  * @param  NewMode: New mode of RTC
  *   This parameter can be one of the following values:
  *   RTC_CLOCK_MODE, RTC_ALARM_MODE 
  * @param  Min: New minute value, max 59
  *   This parameter can be one of the following values:
  *   0, 1, 2, ...59 
  * @retval None
  */
void RTC_SetMin(RTC_FuncMode NewMode, uint8_t Min)
{
    uint8_t tmp = 0U;

    /* Check the parameters */
    assert_param(IS_RTC_MINUTE(Min));
    assert_param(IS_RTC_FUNC_MODE(NewMode));

    tmp = TSB_RTC->PAGER;
    tmp &= PAGER_PAGE_CLEAR;

    if (NewMode == RTC_ALARM_MODE) {
        /* Select PAGE1 */
        tmp |= PAGER_PAGE_SET;
    } else {
        /* Select PAGE0 */
        /* Do nothing */
    }

    TSB_RTC->PAGER = tmp;
    tmp = (uint8_t) ((Min / 10U) * 16U + Min % 10U);
    /* Write min value into MINR register */
    TSB_RTC->MINR = tmp;
}

/**
  * @brief  Get minute value.
  * @param  NewMode: New mode of RTC
  *   This parameter can be one of the following values:
  *   RTC_CLOCK_MODE, RTC_ALARM_MODE 
  * @retval The minute value.
  */
uint8_t RTC_GetMin(RTC_FuncMode NewMode)
{
    uint8_t min = 0U;
    uint8_t tmp = 0U;

    /* Check the parameters */
    assert_param(IS_RTC_FUNC_MODE(NewMode));

    tmp = TSB_RTC->PAGER;
    tmp &= PAGER_PAGE_CLEAR;

    if (NewMode == RTC_CLOCK_MODE) {
        /* Select PAGE0 */
        TSB_RTC->PAGER = tmp;

        do {
            tmp = TSB_RTC->MINR;
            min = TSB_RTC->MINR;
        }
        while (tmp != min);

    } else {
        /* Select PAGE1 */
        tmp |= PAGER_PAGE_SET;
        TSB_RTC->PAGER = tmp;
        tmp = TSB_RTC->MINR;
    }

    min = (uint8_t) ((tmp >> 4U) * 10U + tmp % 16U);

    /* return minute value  */
    return min;
}

/**
  * @brief  Get AM or PM state in the 12 Hour mode.
  * @param  NewMode: New mode of RTC
  *   This parameter can be one of the following values:
  *   RTC_CLOCK_MODE, RTC_ALARM_MODE
  * @retval mode: The mode of the time.
  *   This parameter can be one of the following values:
  *   RTC_AM_MODE, RTC_PM_MODE 
  */
uint8_t RTC_GetAMPM(RTC_FuncMode NewMode)
{
    uint8_t tmp = 0U;
    uint8_t mode = 0U;

    /* Check the parameters */
    assert_param(IS_RTC_FUNC_MODE(NewMode));

    tmp = TSB_RTC->PAGER;
    tmp &= PAGER_PAGE_CLEAR;

    if (NewMode == RTC_CLOCK_MODE) {
        /* Select PAGE0 */
        TSB_RTC->PAGER = tmp;

        do {
            tmp = TSB_RTC->HOURR;
            mode = TSB_RTC->HOURR;
        }
        while (tmp != mode);

    } else {
        /* Select PAGE1 */
        tmp |= PAGER_PAGE_SET;
        TSB_RTC->PAGER = tmp;
        tmp = TSB_RTC->HOURR;
    }

    tmp &= HOURR_HO5_SET;

    if (tmp == HOURR_HO5_SET) {
        mode = RTC_PM_MODE;
    } else {
        mode = RTC_AM_MODE;
    }

    return (mode);
}

/**
  * @brief  Set hour value in the 24 Hour mode.
  * @param  NewMode: New mode of RTC
  *   This parameter can be one of the following values:
  *   RTC_CLOCK_MODE, RTC_ALARM_MODE 
  * @param  Hour: New hour value, max 23
  *   This parameter can be one of the following values:
  *   0, 1, 2, ...23 
  * @retval None
  */
void RTC_SetHour24(RTC_FuncMode NewMode, uint8_t Hour)
{
    uint8_t tmp = 0U;

    /* Check the parameters */
    assert_param(IS_RTC_HOUR_24(Hour));
    assert_param(IS_RTC_FUNC_MODE(NewMode));

    tmp = TSB_RTC->PAGER;
    tmp &= PAGER_PAGE_CLEAR;

    if (NewMode == RTC_ALARM_MODE) {
        /* Select PAGE1 */
        tmp |= PAGER_PAGE_SET;
    } else {
        /* Select PAGE0 */
        /* Do nothing */
    }

    TSB_RTC->PAGER = tmp;
    tmp = (uint8_t) ((Hour / 10U) * 16U + Hour % 10U);
    /* Write hour value into HOURR register */
    TSB_RTC->HOURR = tmp;

    /* Wait for setting hour successfully */
    while (TSB_RTC->HOURR != tmp) {
        /* Do nothing */
    }
}

/**
  * @brief  Set hour value in the 12 Hour mode.
  * @param  NewMode: New mode of RTC
  *   This parameter can be one of the following values:
  *   RTC_CLOCK_MODE, RTC_ALARM_MODE 
  * @param  Hour: New hour value, max 11
  *   This parameter can be one of the following values:
  *   0, 1, 2, ...11
  * @param  AmPm: New time mode
  *   This parameter can be one of the following values:
  *   RTC_AM_MODE or RTC_PM_MODE
  * @retval None
  */
void RTC_SetHour12(RTC_FuncMode NewMode, uint8_t Hour, uint8_t AmPm)
{
    uint8_t tmp = 0U;

    /* Check the parameters */
    assert_param(IS_RTC_HOUR_12(Hour));
    assert_param(IS_RTC_FUNC_MODE(NewMode));
    assert_param(IS_RTC_AMPM_MODE(AmPm));

    tmp = TSB_RTC->PAGER;
    tmp &= PAGER_PAGE_CLEAR;

    if (NewMode == RTC_ALARM_MODE) {
        /* Select PAGE1 */
        tmp |= PAGER_PAGE_SET;
    } else {
        /* Select PAGE0 */
        /* Do nothing */
    }

    TSB_RTC->PAGER = tmp;
    tmp = (uint8_t) ((Hour / 10U) * 16U + Hour % 10U);

    if (AmPm == RTC_PM_MODE) {
        tmp |= HOURR_HO5_SET;
    } else {
        /* Do nothing */
    }

    /* Write hour value into HOURR register */
    TSB_RTC->HOURR = tmp;

    /* Wait for setting hour successfully */
    while (TSB_RTC->HOURR != tmp) {
        /* Do nothing */
    }
}

/**
  * @brief  Get the hour value.
  * @param  NewMode: New mode of RTC
  *   This parameter can be one of the following values:
  *   RTC_CLOCK_MODE, RTC_ALARM_MODE 
  * @retval The hour value.
  */
uint8_t RTC_GetHour(RTC_FuncMode NewMode)
{
    uint8_t HourMode = 0U;
    uint8_t hour = 0U;
    uint8_t tmp = 0U;

    /* Check the parameters */
    assert_param(IS_RTC_FUNC_MODE(NewMode));

    /* Get hour mode */
    HourMode = RTC_GetHourMode();
    tmp = TSB_RTC->PAGER;
    tmp &= PAGER_PAGE_CLEAR;

    if (NewMode == RTC_CLOCK_MODE) {
        /* Select PAGE0 */
        TSB_RTC->PAGER = tmp;

        /* Get hour value */
        do {
            tmp = TSB_RTC->HOURR;
            hour = TSB_RTC->HOURR;
        }
        while (tmp != hour);

    } else {
        /* Select PAGE1 */
        tmp |= PAGER_PAGE_SET;
        TSB_RTC->PAGER = tmp;
        tmp = TSB_RTC->HOURR;
    }

    if (HourMode == RTC_12_HOUR_MODE) {
        tmp &= HOURR_HO5_CLEAR;
    } else {
        /* Do nothing */
    }

    hour = (uint8_t) ((tmp >> 4U) * 10U + tmp % 16U);

    /* return hour value  */
    return hour;
}

/**
  * @brief  Set the day of the week.
  * @param  NewMode: New mode of RTC
  *   This parameter can be one of the following values:
  *   RTC_CLOCK_MODE, RTC_ALARM_MODE 
  * @param  Day: New day value
  *   This parameter can be one of the following values:
  *   RTC_SUN, RTC_MON, RTC_TUE, RTC_WED, RTC_THU, RTC_FRI, RTC_SAT 
  * @retval None
  */
void RTC_SetDay(RTC_FuncMode NewMode, uint8_t Day)
{
    uint8_t tmp = 0U;

    /* Check the parameters */
    assert_param(IS_RTC_DAY(Day));
    assert_param(IS_RTC_FUNC_MODE(NewMode));

    tmp = TSB_RTC->PAGER;
    tmp &= PAGER_PAGE_CLEAR;

    if (NewMode == RTC_ALARM_MODE) {
        /* Select PAGE1 */
        tmp |= PAGER_PAGE_SET;
    } else {
        /* Select PAGE0 */
        /* Do nothing */
    }

    TSB_RTC->PAGER = tmp;
    /* Write day value into DAYR register */
    TSB_RTC->DAYR = Day;
}

/**
  * @brief  Get the day of the week.
  * @param  NewMode: New mode of RTC
  *   This parameter can be one of the following values:
  *   RTC_CLOCK_MODE, RTC_ALARM_MODE 
  * @retval The day value.
  */
uint8_t RTC_GetDay(RTC_FuncMode NewMode)
{
    uint8_t day = 0U;
    uint8_t tmp = 0U;

    /* Check the parameters */
    assert_param(IS_RTC_FUNC_MODE(NewMode));

    tmp = TSB_RTC->PAGER;
    tmp &= PAGER_PAGE_CLEAR;

    if (NewMode == RTC_CLOCK_MODE) {
        /* Select PAGE0 */
        TSB_RTC->PAGER = tmp;

        /* Get day value */
        do {
            tmp = TSB_RTC->DAYR;
            day = TSB_RTC->DAYR;
        }
        while (tmp != day);

    } else {
        /* Select PAGE1 */
        tmp |= PAGER_PAGE_SET;
        TSB_RTC->PAGER = tmp;
        day = TSB_RTC->DAYR;
    }

    /* return day value  */
    return day;
}

/**
  * @brief  Set the date value.
  * @param  NewMode: New mode of RTC
  *   This parameter can be one of the following values:
  *   RTC_CLOCK_MODE, RTC_ALARM_MODE 
  * @param  Date: New date value, Max 31
  *   This parameter can be one of the following values:
  *   1, 2, ...31 
  * @retval None
  */
void RTC_SetDate(RTC_FuncMode NewMode, uint8_t Date)
{
    uint8_t tmp = 0U;

    /* Check the parameters */
    assert_param(IS_RTC_DATE(Date));
    assert_param(IS_RTC_FUNC_MODE(NewMode));

    tmp = TSB_RTC->PAGER;
    tmp &= PAGER_PAGE_CLEAR;

    if (NewMode == RTC_ALARM_MODE) {
        /* Select PAGE1 */
        tmp |= PAGER_PAGE_SET;
    } else {
        /* Select PAGE0 */
        /* Do nothing */
    }

    TSB_RTC->PAGER = tmp;
    tmp = (uint8_t) ((Date / 10U) * 16U + Date % 10U);
    /* Write date value into DAYR register */
    TSB_RTC->DATER = tmp;
}

/**
  * @brief  Get the date value.
  * @param  NewMode: New mode of RTC
  *   This parameter can be one of the following values:
  *   RTC_CLOCK_MODE, RTC_ALARM_MODE 
  * @retval The date value.
  */
uint8_t RTC_GetDate(RTC_FuncMode NewMode)
{
    uint8_t tmp = 0U;
    uint8_t date = 0U;

    /* Check the parameters */
    assert_param(IS_RTC_FUNC_MODE(NewMode));

    tmp = TSB_RTC->PAGER;
    tmp &= PAGER_PAGE_CLEAR;

    if (NewMode == RTC_CLOCK_MODE) {
        /* Select PAGE0 */
        TSB_RTC->PAGER = tmp;

        /* Get date value */
        do {
            tmp = TSB_RTC->DATER;
            date = TSB_RTC->DATER;
        }
        while (tmp != date);

    } else {
        /* Select PAGE1 */
        tmp |= PAGER_PAGE_SET;
        TSB_RTC->PAGER = tmp;
        tmp = TSB_RTC->DATER;
    }

    date = (uint8_t) ((tmp >> 4U) * 10U + tmp % 16U);

    /* return day value  */
    return date;
}

/**
  * @brief  Set the month value.
  * @param  Month: New month value, Max 12
  *   This parameter can be one of the following values:
  *   1, 2, 3, ...12 
  * @retval None
  */
void RTC_SetMonth(uint8_t Month)
{
    uint8_t tmp = 0U;

    /* Check the parameters */
    assert_param(IS_RTC_MONTH(Month));

    /* Select PAGE0 */
    tmp = TSB_RTC->PAGER;
    tmp &= PAGER_PAGE_CLEAR;
    TSB_RTC->PAGER = tmp;
    tmp = (uint8_t) ((Month / 10U) * 16U + Month % 10U);
    /* Write month value into MONTHR register */
    TSB_RTC->MONTHR = tmp;
}

/**
  * @brief  Get the month value.
  * @param  None
  * @retval The month value.
  */
uint8_t RTC_GetMonth(void)
{
    uint8_t tmp = 0U;
    uint8_t month = 0U;

    /* Select PAGE0 */
    tmp = TSB_RTC->PAGER;
    tmp &= PAGER_PAGE_CLEAR;
    TSB_RTC->PAGER = tmp;

    /* Get month value */
    do {
        tmp = TSB_RTC->MONTHR;
        month = TSB_RTC->MONTHR;
    }
    while (tmp != month);

    month = (uint8_t) ((tmp >> 4U) * 10U + tmp % 16U);

    /* return month value */
    return month;
}

/**
  * @brief  Set the year value.
  * @param  Year: New year value, Max 99
  *   This parameter can be one of the following values:
  *   0, 1, 2, 3, ...99
  * @retval None
  */
void RTC_SetYear(uint8_t Year)
{
    uint8_t tmp = 0U;

    /* Check the parameters */
    assert_param(IS_RTC_YEAR(Year));

    /* Select PAGE0 */
    tmp = TSB_RTC->PAGER;
    tmp &= PAGER_PAGE_CLEAR;
    TSB_RTC->PAGER = tmp;
    tmp = (uint8_t) ((Year / 10U) * 16U + Year % 10U);
    /* Write year value into YEARR register */
    TSB_RTC->YEARR = tmp;
}

/**
  * @brief  Get the year value.
  * @param  None
  * @retval The year value.
  */
uint8_t RTC_GetYear(void)
{
    uint8_t tmp = 0U;
    uint8_t year = 0U;

    /* Select PAGE0 */
    tmp = TSB_RTC->PAGER;
    tmp &= PAGER_PAGE_CLEAR;
    TSB_RTC->PAGER = tmp;

    /* Get year value */
    do {
        tmp = TSB_RTC->YEARR;
        year = TSB_RTC->YEARR;
    }
    while (tmp != year);

    year = (uint8_t) ((tmp >> 4U) * 10U + tmp % 16U);

    /* return year value  */
    return year;
}

/**
  * @brief  Select 24-hour clock or 12-hour clock.
  * @param  HourMode: Select 24-hour clock or 12-hour clock 
  *   This parameter can be one of the following values:
  *   RTC_12_HOUR_MODE, RTC_24_HOUR_MODE
  * @retval None
  */
void RTC_SetHourMode(uint8_t HourMode)
{
    uint8_t tmp = 0U;

    /* Check the parameters */
    assert_param(IS_RTC_HOUR_MODE(HourMode));

    /* Select PAGE1 */
    tmp = TSB_RTC->PAGER;
    tmp |= PAGER_PAGE_SET;
    TSB_RTC->PAGER = tmp;

    /* Set MONTHR<MO0> to select 24-hour mode/12-hour mode */
    if (HourMode == RTC_12_HOUR_MODE) {
        TSB_RTC->MONTHR &= RTC_12_HOUR_MODE;
    } else {
        TSB_RTC->MONTHR |= RTC_24_HOUR_MODE;
    }
}

/**
  * @brief  Get hour mode.
  * @param  None
  * @retval mode: The mode of the hour.
  *   This parameter can be one of the following values:
  *   RTC_12_HOUR_MODE, RTC_24_HOUR_MODE 
  */
uint8_t RTC_GetHourMode(void)
{
    uint8_t mode = 0U;

    /* Select PAGE1 */
    mode = TSB_RTC->PAGER;
    mode |= PAGER_PAGE_SET;
    TSB_RTC->PAGER = mode;

    mode = TSB_RTC->MONTHR;
    mode &= MONTHR_MO0_MASK;
    if (mode == MONTHR_MO0_MASK) {
        mode = RTC_24_HOUR_MODE;
    } else {
        mode = RTC_12_HOUR_MODE;
    }

    return (mode);
}

/**
  * @brief  Set leap-year state.
  * @param  LeapYear: Set leap-year 
  *   This parameter can be one of the following values:
  *   RTC_LEAP_YEAR_0, RTC_LEAP_YEAR_1,
  *   RTC_LEAP_YEAR_2 or RTC_LEAP_YEAR_3 
  * @retval None
  */
void RTC_SetLeapYear(uint8_t LeapYear)
{
    uint8_t tmp = 0U;

    /* Check the parameters */
    assert_param(IS_RTC_LEAP_YEAR(LeapYear));

    /* Select PAGE1 */
    tmp = TSB_RTC->PAGER;
    tmp |= PAGER_PAGE_SET;
    TSB_RTC->PAGER = tmp;

    /* Set <LEAP1> and <LEAP0> to select leap year state */
    TSB_RTC->YEARR = LeapYear;

    /* Wait for setting leap year successfully */
    while (TSB_RTC->YEARR != LeapYear) {
        /* Do nothing */
    }
}

/**
  * @brief  Get leap-year state.
  * @param  None 
  * @retval The state of leap year.
  */
uint8_t RTC_GetLeapYear(void)
{
    uint8_t tmp = 0U;
    uint8_t leapyear = 0U;

    /* Select PAGE1 */
    tmp = TSB_RTC->PAGER;
    tmp |= PAGER_PAGE_SET;
    TSB_RTC->PAGER = tmp;

    /* Get leap year state */
    do {
        tmp = TSB_RTC->YEARR;
        leapyear = TSB_RTC->YEARR;
    }
    while (tmp != leapyear);

    /* return leap year state  */
    return (leapyear);
}

/**
  * @brief  Set time adjustment + or - 30 seconds.
  * @param  None
  * @retval None
  */
void RTC_SetTimeAdjustReq(void)
{
    uint8_t tmp = 0U;

    /* Set PAGER<ADJUST> */
    tmp = TSB_RTC->PAGER;
    tmp |= PAGER_ADJUST_SET;
    TSB_RTC->PAGER = tmp;
}

/**
  * @brief  Get request state for time adjust.
  * @param  None
  * @retval The state of time adjustment.
  * This parameter can be one of the following values:
  * RTC_REQ or RTC_NO_REQ
  */
RTC_ReqState RTC_GetTimeAdjustReq(void)
{
    uint8_t tmp0 = 0U;
    RTC_ReqState tmp1 = RTC_NO_REQ;

    tmp0 = TSB_RTC->PAGER;
    tmp0 &= PAGER_ADJUST_SET;

    if (tmp0 == PAGER_ADJUST_SET) {
        tmp1 = RTC_REQ;
    } else {
        tmp1 = RTC_NO_REQ;
    }

    return (tmp1);
}

/**
  * @brief  Enable RTC clock function.
  * @param  None
  * @retval None
  */
void RTC_EnableClock(void)
{
    uint8_t tmp = 0U;

    /* Set PAGER<ENATMR> to enable clock function */
    tmp = TSB_RTC->PAGER;
    tmp |= PAGER_ENATMR_SET;
    TSB_RTC->PAGER = tmp;
}

/**
  * @brief  Disable RTC clock function.
  * @param  None
  * @retval None
  */
void RTC_DisableClock(void)
{
    uint8_t tmp = 0U;

    /* Clear PAGER<ENATMR> to disable clock function */
    tmp = TSB_RTC->PAGER;
    tmp &= PAGER_ENATMR_CLEAR;
    TSB_RTC->PAGER = tmp;
}

/**
  * @brief  Enable RTC alarm function.
  * @param  None
  * @retval None
  */
void RTC_EnableAlarm(void)
{
    uint8_t tmp = 0U;

    /* Set PAGER<ENAALM> to enable alarm function */
    tmp = TSB_RTC->PAGER;
    tmp |= PAGER_ENAALM_SET;
    TSB_RTC->PAGER = tmp;
}

/**
  * @brief  Disable RTC alarm function.
  * @param  None
  * @retval None
  */
void RTC_DisableAlarm(void)
{
    uint8_t tmp = 0U;

    /* Clear PAGER<ENAALM> to disable alarm function */
    tmp = TSB_RTC->PAGER;
    tmp &= PAGER_ENAALM_CLEAR;
    TSB_RTC->PAGER = tmp;
}

/**
  * @brief  Enable or disable INTRTC.
  * @param  NewState: New state of RTCINT.
  *   This parameter can be ENABLE or DISABLE.
  * @retval None
  */
void RTC_SetRTCINT(FunctionalState NewState)
{
    uint8_t tmp = 0U;

    /* Check the parameters */
    assert_param(IS_FUNCTIONAL_STATE(NewState));

    /* Set PAGER<INTENA> to enable or disable INTRTC */
    tmp = TSB_RTC->PAGER;
    tmp &= PAGER_INTENA_CLEAR;

    if (NewState == ENABLE) {
        tmp |= PAGER_INTENA_SET;
    } else {
        /* Do nothing */
    }

    TSB_RTC->PAGER = tmp;
}

/**
  * @brief  Set output signals from ALARM pin.
  * @param  Output: Set ALARM pin output.
  *   This parameter can be RTC_LOW_LEVEL, RTC_PULSE_1_HZ
  *   or RTC_PULSE_16_HZ,RTC_PULSE_2_HZ,RTC_PULSE_4_HZ,RTC_PULSE_8_HZ.
  * @retval None
  */
void RTC_SetAlarmOutput(uint8_t Output)
{
    uint8_t tmp = 0U;

    /* Check the parameters */
    assert_param(IS_RTC_ALARM_OUTPUT(Output));

    switch (Output) {
    case RTC_LOW_LEVEL:
        /* Set PAGER<ENAALM> = 1 to enable alarm function */
        tmp = TSB_RTC->PAGER;
        tmp |= PAGER_ENAALM_SET;
        TSB_RTC->PAGER = tmp;
        /* Set All DIS bits = 1  */
        tmp = TSB_RTC->RESTR;
        tmp &= RESTR_DIS_CLEAR;
        tmp |= RESTR_DIS_SET;
        TSB_RTC->RESTR = tmp;
        break;
    case RTC_PULSE_1_HZ:
        /* Set PAGER<ENAALM> = 0 to disable alarm function */
        tmp = TSB_RTC->PAGER;
        tmp &= PAGER_ENAALM_CLEAR;
        TSB_RTC->PAGER = tmp;
        /* Set RESTR<DIS1HZ> = 0 and others DIS bit = 1  */
        tmp = TSB_RTC->RESTR;
        tmp &= RESTR_DIS_CLEAR;
        tmp |= (uint8_t) (RESTR_DIS16HZ_SET | RESTR_DIS2HZ_SET);
        tmp |= (uint8_t) (RESTR_DIS4HZ_SET | RESTR_DIS8HZ_SET);
        TSB_RTC->RESTR = tmp;
        break;
    case RTC_PULSE_16_HZ:
        /* Set PAGER<ENAALM> = 0 to disable alarm function */
        tmp = TSB_RTC->PAGER;
        tmp &= PAGER_ENAALM_CLEAR;
        TSB_RTC->PAGER = tmp;
        /* Set RESTR<DIS16HZ> = 0 and others DIS bit = 1 */
        tmp = TSB_RTC->RESTR;
        tmp &= RESTR_DIS_CLEAR;
        tmp |= (uint8_t) (RESTR_DIS1HZ_SET | RESTR_DIS2HZ_SET);
        tmp |= (uint8_t) (RESTR_DIS4HZ_SET | RESTR_DIS8HZ_SET);
        TSB_RTC->RESTR = tmp;
        break;
    case RTC_PULSE_2_HZ:
        /* Set PAGER<ENAALM> = 0 to disable alarm function */
        tmp = TSB_RTC->PAGER;
        tmp &= PAGER_ENAALM_CLEAR;
        TSB_RTC->PAGER = tmp;
        /* Set RESTR<DIS2HZ> = 0 and others DIS bit = 1  */
        tmp = TSB_RTC->RESTR;
        tmp &= RESTR_DIS_CLEAR;
        tmp |= (uint8_t) (RESTR_DIS16HZ_SET | RESTR_DIS1HZ_SET);
        tmp |= (uint8_t) (RESTR_DIS4HZ_SET | RESTR_DIS8HZ_SET);
        TSB_RTC->RESTR = tmp;
        break;
    case RTC_PULSE_4_HZ:
        /* Set PAGER<ENAALM> = 0 to disable alarm function */
        tmp = TSB_RTC->PAGER;
        tmp &= PAGER_ENAALM_CLEAR;
        TSB_RTC->PAGER = tmp;
        /* Set RESTR<DIS4HZ> = 0 and others DIS bit = 1 */
        tmp = TSB_RTC->RESTR;
        tmp &= RESTR_DIS_CLEAR;
        tmp |= (uint8_t) (RESTR_DIS16HZ_SET | RESTR_DIS2HZ_SET);
        tmp |= (uint8_t) (RESTR_DIS1HZ_SET | RESTR_DIS8HZ_SET);
        TSB_RTC->RESTR = tmp;
        break;
    case RTC_PULSE_8_HZ:
        /* Set PAGER<ENAALM> = 0 to disable alarm function */
        tmp = TSB_RTC->PAGER;
        tmp &= PAGER_ENAALM_CLEAR;
        TSB_RTC->PAGER = tmp;
        /* Set RESTR<DIS8HZ> = 0 and others DIS bit = 1  */
        tmp = TSB_RTC->RESTR;
        tmp &= RESTR_DIS_CLEAR;
        tmp |= (uint8_t) (RESTR_DIS16HZ_SET | RESTR_DIS2HZ_SET);
        tmp |= (uint8_t) (RESTR_DIS4HZ_SET | RESTR_DIS1HZ_SET);
        TSB_RTC->RESTR = tmp;
        break;
    default:
        /* Do nothing */
        break;
    }
}

/**
  * @brief  Reset alarm function.
  * @param  None
  * @retval None
  */
void RTC_ResetAlarm(void)
{
    uint8_t tmp = 0U;

    /* Set RESTR<RSTALM> = 1 to reset alarm */
    tmp = TSB_RTC->RESTR;
    tmp &= RESTR_RSTALM_CLEAR;
    tmp |= RESTR_RSTALM_SET;
    TSB_RTC->RESTR = tmp;
}

/**
  * @brief  Reset RTC clock second counter.
  * @param  None
  * @retval None
  */
void RTC_ResetClockSec(void)
{
    uint8_t tmp = 0U;

    /* Set RESTR<RSTTMR> = 1 to reset clock sec counter */
    tmp = TSB_RTC->RESTR;
    tmp &= RESTR_RSTTMR_CLEAR;
    tmp |= RESTR_RSTTMR_SET;
    TSB_RTC->RESTR = tmp;
}

/**
  * @brief  Get request state for reset RTC clock second counter.
  * @param  None
  * @retval The state of reset clock request.
  * This parameter can be one of the following values:
  * RTC_REQ or RTC_NO_REQ
  */
RTC_ReqState RTC_GetResetClockSecReq(void)
{
    uint8_t tmp0 = 0U;
    RTC_ReqState tmp1 = RTC_NO_REQ;

    tmp0 = TSB_RTC->RESTR;
    tmp0 &= RESTR_RSTTMR_SET;

    if (tmp0 == RESTR_RSTTMR_SET) {
        tmp1 = RTC_REQ;
    } else {
        tmp1 = RTC_NO_REQ;
    }

    return (tmp1);
}

/**
  * @brief  Set the RTC date.
  * @param  DateStruct: The structure containing basic Date configuration.
  * @retval None
  */
void RTC_SetDateValue(RTC_DateTypeDef * DateStruct)
{
    /* Check the parameters */
    assert_param(IS_POINTER_NOT_NULL(DateStruct));
    assert_param(IS_RTC_LEAP_YEAR(DateStruct->LeapYear));
    assert_param(IS_RTC_YEAR(DateStruct->Year));
    assert_param(IS_RTC_MONTH(DateStruct->Month));
    assert_param(IS_RTC_DATE(DateStruct->Date));
    assert_param(IS_RTC_DAY(DateStruct->Day));

    /* Set LeapYear */
    RTC_SetLeapYear(DateStruct->LeapYear);
    /* Set year value */
    RTC_SetYear(DateStruct->Year);
    /* Set month value */
    RTC_SetMonth(DateStruct->Month);
    /* Set date value */
    RTC_SetDate(RTC_CLOCK_MODE, DateStruct->Date);
    /* Set day value */
    RTC_SetDay(RTC_CLOCK_MODE, DateStruct->Day);
}

/**
  * @brief  Get the RTC date.
  * @param  DateStruct: The structure containing basic Date configuration will be modified.
  * @retval None
  */
void RTC_GetDateValue(RTC_DateTypeDef * DateStruct)
{
    /* Check the parameters */
    assert_param(IS_POINTER_NOT_NULL(DateStruct));

    /* Get LeapYear */
    DateStruct->LeapYear = RTC_GetLeapYear();
    /* Get year value */
    DateStruct->Year = RTC_GetYear();
    /* Get month value */
    DateStruct->Month = RTC_GetMonth();
    /* Get date value */
    DateStruct->Date = RTC_GetDate(RTC_CLOCK_MODE);
    /* Get day value */
    DateStruct->Day = RTC_GetDay(RTC_CLOCK_MODE);
}

/**
  * @brief  Set the RTC time.
  * @param  TimeStruct: The structure containing basic Time configuration.
  * @retval None
  */
void RTC_SetTimeValue(RTC_TimeTypeDef * TimeStruct)
{
    /* Check the parameters */
    assert_param(IS_POINTER_NOT_NULL(TimeStruct));
    assert_param(IS_RTC_HOUR_MODE(TimeStruct->HourMode));

    if (TimeStruct->HourMode) {
        assert_param(IS_RTC_HOUR_24(TimeStruct->Hour));
    } else {
        assert_param(IS_RTC_HOUR_12(TimeStruct->Hour));
        assert_param(IS_RTC_AMPM_MODE(TimeStruct->AmPm));
    }

    assert_param(IS_RTC_MINUTE(TimeStruct->Min));
    assert_param(IS_RTC_SECOND(TimeStruct->Sec));

    /* Set hour mode */
    RTC_SetHourMode(TimeStruct->HourMode);

    /* Set hour value */
    if (TimeStruct->HourMode == RTC_24_HOUR_MODE) {
        RTC_SetHour24(RTC_CLOCK_MODE, TimeStruct->Hour);
    } else {
        RTC_SetHour12(RTC_CLOCK_MODE, TimeStruct->Hour, TimeStruct->AmPm);
    }

    /* Set minute value */
    RTC_SetMin(RTC_CLOCK_MODE, TimeStruct->Min);
    /* Set second value */
    RTC_SetSec(TimeStruct->Sec);
}

/**
  * @brief  Get the RTC time.
  * @param  TimeStruct: The structure containing basic Time configuration will be modified.
  * @retval None
  */
void RTC_GetTimeValue(RTC_TimeTypeDef * TimeStruct)
{
    /* Check the parameters */
    assert_param(IS_POINTER_NOT_NULL(TimeStruct));

    /* Get hour mode */
    TimeStruct->HourMode = RTC_GetHourMode();
    /* Get hour value */
    TimeStruct->Hour = RTC_GetHour(RTC_CLOCK_MODE);

    if (TimeStruct->HourMode == RTC_12_HOUR_MODE) {
        /* Get AM/PM mode */
        TimeStruct->AmPm = RTC_GetAMPM(RTC_CLOCK_MODE);
    } else {
        TimeStruct->AmPm = RTC_AMPM_INVALID;
    }

    /* Get minute value */
    TimeStruct->Min = RTC_GetMin(RTC_CLOCK_MODE);
    /* Get second value */
    TimeStruct->Sec = RTC_GetSec();
}

/**
  * @brief  Set the RTC date and time.
  * @param  DateStruct: The structure containing basic Date configuration.
  * @param  TimeStruct: The structure containing basic Time configuration.
  * @retval None
  */
void RTC_SetClockValue(RTC_DateTypeDef * DateStruct, RTC_TimeTypeDef * TimeStruct)
{
    /* Check the parameters */
    assert_param(IS_POINTER_NOT_NULL(DateStruct));
    assert_param(IS_POINTER_NOT_NULL(TimeStruct));
    assert_param(IS_RTC_LEAP_YEAR(DateStruct->LeapYear));
    assert_param(IS_RTC_YEAR(DateStruct->Year));
    assert_param(IS_RTC_MONTH(DateStruct->Month));
    assert_param(IS_RTC_DATE(DateStruct->Date));
    assert_param(IS_RTC_DAY(DateStruct->Day));
    assert_param(IS_RTC_HOUR_MODE(TimeStruct->HourMode));

    if (TimeStruct->HourMode == RTC_24_HOUR_MODE) {
        assert_param(IS_RTC_HOUR_24(TimeStruct->Hour));
    } else {
        assert_param(IS_RTC_HOUR_12(TimeStruct->Hour));
        assert_param(IS_RTC_AMPM_MODE(TimeStruct->AmPm));
    }

    assert_param(IS_RTC_MINUTE(TimeStruct->Min));
    assert_param(IS_RTC_SECOND(TimeStruct->Sec));

    /* Set hour mode */
    RTC_SetHourMode(TimeStruct->HourMode);

    /* Set hour value */
    if (TimeStruct->HourMode == RTC_24_HOUR_MODE) {
        RTC_SetHour24(RTC_CLOCK_MODE, TimeStruct->Hour);
    } else {
        RTC_SetHour12(RTC_CLOCK_MODE, TimeStruct->Hour, TimeStruct->AmPm);
    }

    /* Set minute value */
    RTC_SetMin(RTC_CLOCK_MODE, TimeStruct->Min);
    /* Set second value */
    RTC_SetSec(TimeStruct->Sec);

    /* Set LeapYear */
    RTC_SetLeapYear(DateStruct->LeapYear);
    /* Set year value */
    RTC_SetYear(DateStruct->Year);
    /* Set month value */
    RTC_SetMonth(DateStruct->Month);
    /* Set date value */
    RTC_SetDate(RTC_CLOCK_MODE, DateStruct->Date);
    /* Set day value */
    RTC_SetDay(RTC_CLOCK_MODE, DateStruct->Day);
}

/**
  * @brief  Get the RTC date and time.
  * @param  DateStruct: The structure containing basic Date configuration will be modified.
  * @param  TimeStruct: The structure containing basic Time configuration will be modified.
  * @retval None
  */
void RTC_GetClockValue(RTC_DateTypeDef * DateStruct, RTC_TimeTypeDef * TimeStruct)
{
    /* Check the parameters */
    assert_param(IS_POINTER_NOT_NULL(DateStruct));
    assert_param(IS_POINTER_NOT_NULL(TimeStruct));

    /* Get LeapYear */
    DateStruct->LeapYear = RTC_GetLeapYear();
    /* Get year value */
    DateStruct->Year = RTC_GetYear();
    /* Get month value */
    DateStruct->Month = RTC_GetMonth();
    /* Get date value */
    DateStruct->Date = RTC_GetDate(RTC_CLOCK_MODE);
    /* Get day value */
    DateStruct->Day = RTC_GetDay(RTC_CLOCK_MODE);
    /* Get hour mode */
    TimeStruct->HourMode = RTC_GetHourMode();
    /* Get hour value */
    TimeStruct->Hour = RTC_GetHour(RTC_CLOCK_MODE);

    if (TimeStruct->HourMode == RTC_12_HOUR_MODE) {
        /* Get AM/PM mode */
        TimeStruct->AmPm = RTC_GetAMPM(RTC_CLOCK_MODE);
    } else {
        TimeStruct->AmPm = RTC_AMPM_INVALID;
    }

    /* Get minute value */
    TimeStruct->Min = RTC_GetMin(RTC_CLOCK_MODE);
    /* Get second value */
    TimeStruct->Sec = RTC_GetSec();
}

/**
  * @brief  Set the RTC alarm date and time.
  * @param  AlarmStruct: The structure containing basic alarm configuration.
  * @retval None
  */
void RTC_SetAlarmValue(RTC_AlarmTypeDef * AlarmStruct)
{
    uint8_t HourMode;
    HourMode = RTC_GetHourMode();

    /* Check the parameters */
    assert_param(IS_POINTER_NOT_NULL(AlarmStruct));
    assert_param(IS_RTC_DATE(AlarmStruct->Date));
    assert_param(IS_RTC_DAY(AlarmStruct->Day));

    if (HourMode == RTC_24_HOUR_MODE) {
        assert_param(IS_RTC_HOUR_24(AlarmStruct->Hour));
    } else {
        assert_param(IS_RTC_HOUR_12(AlarmStruct->Hour));
        assert_param(IS_RTC_AMPM_MODE(AlarmStruct->AmPm));
    }

    assert_param(IS_RTC_MINUTE(AlarmStruct->Min));

    /* Set date value */
    RTC_SetDate(RTC_ALARM_MODE, AlarmStruct->Date);
    /* Set day value */
    RTC_SetDay(RTC_ALARM_MODE, AlarmStruct->Day);

    /* Set hour value */
    if (HourMode == RTC_12_HOUR_MODE) {
        RTC_SetHour12(RTC_ALARM_MODE, AlarmStruct->Hour, AlarmStruct->AmPm);
    } else {
        RTC_SetHour24(RTC_ALARM_MODE, AlarmStruct->Hour);
    }

    /* Set minute value */
    RTC_SetMin(RTC_ALARM_MODE, AlarmStruct->Min);
}

/**
  * @brief  Get the RTC alarm date and time.
  * @param  AlarmStruct: The structure containing basic alarm configuration will be modified.
  * @retval None
  */
void RTC_GetAlarmValue(RTC_AlarmTypeDef * AlarmStruct)
{
    uint8_t HourMode;

    /* Check the parameters */
    assert_param(IS_POINTER_NOT_NULL(AlarmStruct));

    HourMode = RTC_GetHourMode();

    /* Get date value */
    AlarmStruct->Date = RTC_GetDate(RTC_ALARM_MODE);
    /* Get day value */
    AlarmStruct->Day = RTC_GetDay(RTC_ALARM_MODE);
    /* Get hour value */
    AlarmStruct->Hour = RTC_GetHour(RTC_ALARM_MODE);

    if (HourMode == RTC_12_HOUR_MODE) {
        /* Get AM/PM mode */
        AlarmStruct->AmPm = RTC_GetAMPM(RTC_ALARM_MODE);
    } else {
        AlarmStruct->AmPm = RTC_AMPM_INVALID;
    }

    /* Get minute value */
    AlarmStruct->Min = RTC_GetMin(RTC_ALARM_MODE);
}

/**
  * @brief  Enable or disable Clock correction function register protection
  * @param  NewState: New state of the RTC protect register
  *   This parameter can be one of the following values:
  *   DISABLE or ENABLE
  * @retval None
  */
void RTC_SetProtectCtrl(FunctionalState NewState)
{
    /* Check the parameters */
    assert_param(IS_FUNCTIONAL_STATE(NewState));

    /* Set RTCPROTECT */
    if (NewState == ENABLE) {
        TSB_RTC->PROTECT = RTC_PROTECT_SET;
    } else {
        TSB_RTC->PROTECT = RTC_PROTECT_CLEAR;
    }
}

/**
  * @brief  Enable RTC correction function.
  * @param  None
  * @retval None
  */
void RTC_EnableCorrection(void)
{
    uint8_t tmp = 0U;

    /* Set RTCADJCTL<AJEN> to enable correction function */
    tmp = TSB_RTC->ADJCTL;
    tmp |= ADJCTL_AJEN_SET;
    TSB_RTC->ADJCTL = tmp;
}

/**
  * @brief  Disable RTC correction function.
  * @param  None
  * @retval None
  */
void RTC_DisableCorrection(void)
{
    uint8_t tmp = 0U;

    /* Clear RTCADJCTL<AJEN> to disable correction function */
    tmp = TSB_RTC->ADJCTL;
    tmp &= ADJCTL_AJEN_CLEAR;
    TSB_RTC->ADJCTL = tmp;
}

/**
  * @brief  Set correction reference time.
  * @param  Time: Correction reference time
  *   This parameter can be one of following values:
  *   RTC_ADJ_TIME_1_SEC, RTC_ADJ_TIME_10_SEC, 
  *   RTC_ADJ_TIME_20_SEC, RTC_ADJ_TIME_30_SEC,
  *   RTC_ADJ_TIME_1_MIN
  * @retval None
  */
void RTC_SetCorrectionTime(uint8_t Time)
{
    uint8_t tmp = 0U;

    /* Check the parameters */
    assert_param(IS_RTC_ADJ_TIME(Time));

    /* Set RTCADJCTL<AJSEL> */
    tmp = TSB_RTC->ADJCTL;
    tmp &= ADJCTL_AJSEL_CLEAR;
    tmp |= Time;
    TSB_RTC->ADJCTL = tmp;
}

/**
  * @brief  Set correction value.
  * @param  Mode: The mode of correction 
  *   This parameter can be one of following values:
  *   RTC_CORRECTION_PLUS, RTC_CORRECTION_MINUS
  * @param  Cnt:The value of correction
  *   This parameter can be 0~255 when Mode is RTC_CORRECTION_PLUS.
  *   This parameter can be 1~256 when Mode is RTC_CORRECTION_MINUS.
  * @retval None
  */
void RTC_SetCorrectionValue(RTC_CorrectionMode Mode, uint16_t Cnt)
{
    uint16_t tmp = 0U;

    /* Check the parameters */
    assert_param(IS_RTC_CORRECTION_MODE(Mode));
    if (Mode == RTC_CORRECTION_PLUS) {
        /* Check the parameters */
        assert_param(IS_RTC_PLUS_VALUE(Cnt));

        /* Set RTCADJDAT<ADJDAT> */
        TSB_RTC->ADJDAT = Cnt;
    } else {
        /* Check the parameters */
        assert_param(IS_RTC_MINUS_VALUE(Cnt));

        /* Set RTCADJDAT<ADJDAT> */
        tmp = ADJDAT_MODE_SET;
        tmp |= (~(Cnt - 1U));
        tmp &= ADJDAT_CLEAR;
        TSB_RTC->ADJDAT = tmp;
    }
}

/** @} */
/* End of group RTC_Exported_Functions */

/** @} */
/* End of group RTC */

/** @} */
/* End of group TX04_Periph_Driver */
#endif                          /* defined(__TMPM46B_RTC_H) */
