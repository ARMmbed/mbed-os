/** 
 *******************************************************************************
 * @file    bsp_rtc.h
 * @brief   This file provides all the functions prototypes for RTC Class.
 * @version V1.0.0.1
 * $Date:: 2017-09-01 08:26:38 #$
 * 
 * SPDX-License-Identifier: Apache-2.0
 * 
 * (C)Copyright TOSHIBA MICROELECTRONICS CORPORATION 2017 All rights reserved
 *******************************************************************************
 */
/*------------------------------------------------------------------------------*/
/*  Define to prevent recursive inclusion                                       */
/*------------------------------------------------------------------------------*/
#ifndef __BSP_RTC_H
#define __BSP_RTC_H

#ifdef __cplusplus
extern "C" {
#endif

/*------------------------------------------------------------------------------*/
/*  Includes                                                                    */
/*------------------------------------------------------------------------------*/
//#include "txz_sample_def.h"
#include "txz_driver_def.h"

/**
 *  @addtogroup Example
 *  @{
 */

/** 
 *  @addtogroup BSP_UTILITIES
 *  @{
 */
/*------------------------------------------------------------------------------*/
/*  Macro Function                                                              */
/*------------------------------------------------------------------------------*/
/** 
 *  @addtogroup BSP_UTILITIES_Exported_macro
 *  @{
 */

/* no define */

/** 
 *  @}
 */ /* End of group BSP_UTILITIES_Exported_macro */


/*------------------------------------------------------------------------------*/
/*  Configuration                                                               */
/*------------------------------------------------------------------------------*/
/** 
 *  @addtogroup BSP_UTILITIES_Exported_define
 *  @{
 */

/* no define */

/** 
 *  @}
 */ /* End of group BSP_UTILITIES_Exported_define */


/*------------------------------------------------------------------------------*/
/*  Macro Definition                                                            */
/*------------------------------------------------------------------------------*/
/** 
 *  @addtogroup BSP_UTILITIES_Exported_define
 *  @{
 */

/** 
 *  @defgroup RTC_RangeSec  Range Second
 *  @brief    Range of Second.
 *  @brief    Range of Second "(RTC_RANGE_SEC_MIN <= Value <= RTC_RANGE_SEC_MAX)".
 *  @{
 */
#define RTC_RANGE_SEC_MIN               ((uint8_t)0x00)         /*!< Minimum  Value : 0 sec */
#define RTC_RANGE_SEC_MAX               ((uint8_t)0x3B)         /*!< Maximum  Value :59 sec */
/**
 *  @}
 */ /* End of group RTC_RangeSec */

/** 
 *  @defgroup RTC_RangeMin  Range Minute
 *  @brief    Range of Minute.
 *  @brief    Range of Minute "(RTC_RANGE_MIN_MIN <= Value <= RTC_RANGE_MIN_MAX)".
 *  @{
 */
#define RTC_RANGE_MIN_MIN               ((uint8_t)0x00)         /*!< Minimum  Value : 0 min */
#define RTC_RANGE_MIN_MAX               ((uint8_t)0x3B)         /*!< Maximum  Value :59 min */
/**
 *  @}
 */ /* End of group RTC_RangeMin */

/** 
 *  @defgroup RTC_AlarmMin  Alarm Minute
 *  @brief    Compare/No Compare minute.
 *  @{
 */
#define RTC_ALARM_MIN_OFF               ((uint8_t)0x7F)         /*!< No compare. */
#define RTC_ALARM_MIN_ON                ((uint8_t)0x00)         /*!< Compare.    */
/**
 *  @}
 */ /* End of group RTC_AlarmMin */

/** 
 *  @defgroup RTC_Range24Hour  Range 24 Hour
 *  @brief    Range of 24 Hour.
 *  @brief    Range of 24 Hour "(RTC_RANGE_24_HOUR_MIN <= Value <= RTC_RANGE_24_HOUR_MAX)".
 *  @{
 */
#define RTC_RANGE_24_HOUR_MIN           ((uint8_t)0x00)         /*!< Minimum  Value : 0 hour */
#define RTC_RANGE_24_HOUR_MAX           ((uint8_t)0x17)         /*!< Maximum  Value :23 hour */
/**
 *  @}
 */ /* End of group RTC_Range24Hour */

/** 
 *  @defgroup RTC_Range12Hour  Range 12 Hour
 *  @brief    Range of 12 Hour.
 *  @brief    Range of 12 Hour "(RTC_RANGE_12_HOUR_MIN <= Value <= RTC_RANGE_12_HOUR_MAX)".
 *  @{
 */
#define RTC_RANGE_12_HOUR_MIN           ((uint8_t)0x00)         /*!< Minimum  Value : 0 hour */
#define RTC_RANGE_12_HOUR_MAX           ((uint8_t)0x0B)         /*!< Maximum  Value :11 hour */
/**
 *  @}
 */ /* End of group RTC_Range12Hour */

/** 
 *  @defgroup RTC_Meridiem  Meridiem
 *  @brief    AM/PM.
 *  @{
 */
#define RTC_MERIDIEM_AM                 ((uint8_t)0x00)         /*!< A.M. */
#define RTC_MERIDIEM_PM                 ((uint8_t)0x20)         /*!< P.M. */
/**
 *  @}
 */ /* End of group RTC_Meridiem */

/** 
 *  @defgroup RTC_AlarmHour  Alarm Hour
 *  @brief    Compare/No Compare hour.
 *  @{
 */
#define RTC_ALARM_HOUR_OFF              ((uint8_t)0x3F)         /*!< No compare. */
#define RTC_ALARM_HOUR_ON               ((uint8_t)0x00)         /*!< Compare.    */
/**
 *  @}
 */ /* End of group RTC_AlarmHour */

/** 
 *  @defgroup RTC_Day  Day Of Week
 *  @brief    Day of week.
 *  @{
 */
#define RTC_DAY_SUNDAY                  ((uint8_t)0x00)         /*!< Sunday.                         */
#define RTC_DAY_MONDAY                  ((uint8_t)0x01)         /*!< Monday.                         */
#define RTC_DAY_TUESDAY                 ((uint8_t)0x02)         /*!< Tuesday.                        */
#define RTC_DAY_WEDNESDAY               ((uint8_t)0x03)         /*!< Wednesday.                      */
#define RTC_DAY_THURSDAY                ((uint8_t)0x04)         /*!< Thursday.                       */
#define RTC_DAY_FRIDAY                  ((uint8_t)0x05)         /*!< Friday.                         */
#define RTC_DAY_SATURDAY                ((uint8_t)0x06)         /*!< Saturday.                       */
/**
 *  @}
 */ /* End of group RTC_Day */

/** 
 *  @defgroup RTC_AlarmDay  Alarm Day
 *  @brief    Compare/No Compare day.
 *  @{
 */
#define RTC_ALARM_DAY_OFF               ((uint8_t)0x03)         /*!< No compare. */
#define RTC_ALARM_DAY_ON                ((uint8_t)0x00)         /*!< Compare.    */
/**
 *  @}
 */ /* End of group RTC_AlarmDay */

/** 
 *  @defgroup RTC_RangeDate  Range Date
 *  @brief    Range of Date.
 *  @brief    Range of Date "(RTC_RANGE_DATE_MIN <= Value <= RTC_RANGE_DATE_MAX)".
 *  @{
 */
#define RTC_RANGE_DATE_MIN              ((uint8_t)0x00)         /*!< Minimum  Value :date 00  */
#define RTC_RANGE_DATE_MAX              ((uint8_t)0x1F)         /*!< Maximum  Value :date 31  */
/**
 *  @}
 */ /* End of group RTC_RangeDate */

/** 
 *  @defgroup RTC_AlarmDate  Alarm Date
 *  @brief    Compare/No Compare date.
 *  @{
 */
#define RTC_ALARM_DATE_OFF              ((uint8_t)0x3F)         /*!< No compare. */
#define RTC_ALARM_DATE_ON               ((uint8_t)0x00)         /*!< Compare.    */
/**
 *  @}
 */ /* End of group RTC_AlarmDate */

/** 
 *  @defgroup RTC_Month  Month
 *  @brief    Month.
 *  @{
 */
#define RTC_MONTH_JAN                   ((uint8_t)0x01)         /*!< January.    */
#define RTC_MONTH_FEB                   ((uint8_t)0x02)         /*!< February.   */
#define RTC_MONTH_MAR                   ((uint8_t)0x03)         /*!< March.      */
#define RTC_MONTH_APR                   ((uint8_t)0x04)         /*!< April.      */
#define RTC_MONTH_MAY                   ((uint8_t)0x05)         /*!< May.        */
#define RTC_MONTH_JUN                   ((uint8_t)0x06)         /*!< June.       */
#define RTC_MONTH_JUL                   ((uint8_t)0x07)         /*!< July.       */
#define RTC_MONTH_AUG                   ((uint8_t)0x08)         /*!< August.     */
#define RTC_MONTH_SEP                   ((uint8_t)0x09)         /*!< September.  */
#define RTC_MONTH_OCT                   ((uint8_t)0x10)         /*!< October.    */
#define RTC_MONTH_NOV                   ((uint8_t)0x11)         /*!< November.   */
#define RTC_MONTH_DEC                   ((uint8_t)0x12)         /*!< December.   */
/**
 *  @}
 */ /* End of group RTC_Month */

/** 
 *  @defgroup BSP_RTC_HourNotation  Hour Notation
 *  @brief    24/12 Hour Notation.
 *  @{
 */
#define RTC_HOUR_NOTATION_12            ((uint8_t)0x00)         /*!< 12-hour notation. */
#define RTC_HOUR_NOTATION_24            ((uint8_t)0x01)         /*!< 24-hour notation. */
/**
 *  @}
 */ /* End of group BSP_RTC_HourNotation */

/** 
 *  @defgroup RTC_RangeYear  Range Year
 *  @brief    Range of Year.
 *  @brief    Range of Year "(RTC_RANGE_YEAR_MIN <= Value <= RTC_RANGE_YEAR_MAX)".
 *  @{
 */
#define RTC_RANGE_YEAR_MIN              ((uint8_t)0x00)         /*!< Minimum  Value :year 00  */
#define RTC_RANGE_YEAR_MAX              ((uint8_t)0x63)         /*!< Maximum  Value :year 99  */
/**
 *  @}
 */ /* End of group RTC_RangeYear */

/** 
 *  @defgroup RTC_LeapYear  Leap Year
 *  @brief    Leap Year.
 *  @{
 */
#define RTC_LEAP_YEAR_0                 ((uint8_t)0x00)         /*!< Leap year.                */
#define RTC_LEAP_YEAR_1                 ((uint8_t)0x01)         /*!< 1 year since leap year.   */
#define RTC_LEAP_YEAR_2                 ((uint8_t)0x02)         /*!< 2 years since leap year.  */
#define RTC_LEAP_YEAR_3                 ((uint8_t)0x03)         /*!< 3 years since leap year.  */
/**
 *  @}
 */ /* End of group RTC_LeapYear */

/** 
 *  @defgroup RTC_IntSource  Interrupt Source Signal
 *  @brief    Interrupt Source Signal.
 *  @{
 */
#define RTC_INT_SRC_1HZ                 ((uint8_t)0x47)         /*!<  1 Hz. */
#define RTC_INT_SRC_2HZ                 ((uint8_t)0xC3)         /*!<  2 Hz. */
#define RTC_INT_SRC_4HZ                 ((uint8_t)0xC5)         /*!<  4 Hz. */
#define RTC_INT_SRC_8HZ                 ((uint8_t)0x01)         /*!<  8 Hz. */
#define RTC_INT_SRC_16HZ                ((uint8_t)0xC6)         /*!< 16 Hz. */
#define RTC_INT_SRC_ALARM               ((uint8_t)0xC7)         /*!< Alarm. */
/**
 *  @}
 */ /* End of group RTC_IntSource */

/** 
 *  @}
 */ /* End of group BSP_UTILITIES_Exported_define */


/*------------------------------------------------------------------------------*/
/*  Enumerated Type Definition                                                  */
/*------------------------------------------------------------------------------*/
/** 
 *  @addtogroup BSP_UTILITIES_Exported_define
 *  @{
 */

/* no define */

/** 
 *  @}
 */ /* End of group BSP_UTILITIES_Exported_define */

/*------------------------------------------------------------------------------*/
/*  Structure Definition                                                        */
/*------------------------------------------------------------------------------*/
/** 
 *  @defgroup BSP_UTILITIES_Exported_typedef
 *  @{
 */
/*----------------------------------*/
/** 
 * @brief  RTC handle structure definenition.
*/
/*----------------------------------*/
typedef struct uart_handle
{
    TSB_RTC_TypeDef       *p_instance;     /*!< Registers base address.           */
} rtc_t;
/**
 *  @}
 */ /* End of group BSP_UTILITIES_Exported_define */


/*------------------------------------------------------------------------------*/
/*  Functions                                                                   */
/*------------------------------------------------------------------------------*/
/** 
 *  @addtogroup BSP_UTILITIES_Exported_functions
 *  @{
 */
TXZ_Result m4g9_rtc_init(rtc_t *p_obj);
TXZ_Result rtc_deinit(rtc_t *p_obj);
TXZ_Result rtc_enable_int(rtc_t *p_obj);
TXZ_Result rtc_disable_int(rtc_t *p_obj);
TXZ_Result rtc_set_int_source(rtc_t *p_obj, uint8_t src);
TXZ_Result rtc_set_hour_notation(rtc_t *p_obj, uint8_t notation);
TXZ_Result rtc_get_hour_notation(rtc_t *p_obj, uint8_t *p_notation);
/* clock */
TXZ_Result rtc_clock_enable(rtc_t *p_obj);
TXZ_Result rtc_clock_disable(rtc_t *p_obj);
TXZ_Result rtc_clock_reset_counter(rtc_t *p_obj);
TXZ_Result rtc_clock_set_sec(rtc_t *p_obj, uint8_t sec);
TXZ_Result rtc_clock_get_sec(rtc_t *p_obj, uint8_t *p_sec);
TXZ_Result rtc_clock_set_min(rtc_t *p_obj, uint8_t min);
TXZ_Result rtc_clock_get_min(rtc_t *p_obj, uint8_t *p_min);
TXZ_Result rtc_clock_set_hour_24(rtc_t *p_obj, uint8_t hour);
TXZ_Result rtc_clock_get_hour_24(rtc_t *p_obj, uint8_t *p_hour);
TXZ_Result rtc_clock_set_hour_12(rtc_t *p_obj, uint8_t meridiem, uint8_t hour);
TXZ_Result rtc_clock_get_hour_12(rtc_t *p_obj, uint8_t *p_meridiem, uint8_t *p_hour);
TXZ_Result rtc_clock_set_day(rtc_t *p_obj, uint8_t day);
TXZ_Result rtc_clock_get_day(rtc_t *p_obj, uint8_t *p_day);
TXZ_Result rtc_clock_set_date(rtc_t *p_obj, uint8_t date);
TXZ_Result rtc_clock_get_date(rtc_t *p_obj, uint8_t *p_date);
TXZ_Result rtc_clock_set_month(rtc_t *p_obj, uint8_t month);
TXZ_Result rtc_clock_get_month(rtc_t *p_obj, uint8_t *p_month);
TXZ_Result rtc_clock_set_year(rtc_t *p_obj, uint8_t year);
TXZ_Result rtc_clock_get_year(rtc_t *p_obj, uint8_t *p_year);
TXZ_Result rtc_clock_set_leap(rtc_t *p_obj, uint8_t leap);
TXZ_Result rtc_clock_get_leap(rtc_t *p_obj, uint8_t *p_leap);
/* alarm */
TXZ_Result rtc_alarm_enable(rtc_t *p_obj);
TXZ_Result rtc_alarm_disable(rtc_t *p_obj);
TXZ_Result rtc_alarm_reset(rtc_t *p_obj);
TXZ_Result rtc_alarm_set_min(rtc_t *p_obj, uint8_t compare, uint8_t min);
TXZ_Result rtc_alarm_get_min(rtc_t *p_obj, uint8_t *p_compare, uint8_t *p_min);
TXZ_Result rtc_alarm_set_hour_24(rtc_t *p_obj, uint8_t compare, uint8_t hour);
TXZ_Result rtc_alarm_get_hour_24(rtc_t *p_obj, uint8_t *p_compare, uint8_t *p_hour);
TXZ_Result rtc_alarm_set_hour_12(rtc_t *p_obj, uint8_t compare, uint8_t meridiem, uint8_t hour);
TXZ_Result rtc_alarm_get_hour_12(rtc_t *p_obj, uint8_t *p_compare, uint8_t *p_meridiem, uint8_t *p_hour);
TXZ_Result rtc_alarm_set_day(rtc_t *p_obj, uint8_t compare, uint8_t day);
TXZ_Result rtc_alarm_get_day(rtc_t *p_obj, uint8_t *p_compare, uint8_t *p_day);
TXZ_Result rtc_alarm_set_date(rtc_t *p_obj, uint8_t compare, uint8_t date);
TXZ_Result rtc_alarm_get_date(rtc_t *p_obj, uint8_t *p_compare, uint8_t *p_date);
/** 
 *  @}
 */ /* End of group BSP_UTILITIES_Exported_functions */

/**
 *  @}
 */ /* End of group BSP_UTILITIES */

/**
 *  @} 
 */ /* End of group Sample_Appli */

#ifdef __cplusplus
}
#endif /* __cplusplus */
#endif /* __BSP_RTC_H */


