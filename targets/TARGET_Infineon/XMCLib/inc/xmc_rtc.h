/**
 * @file xmc_rtc.h
 * @date 2016-05-19
 *
 * @cond
 *********************************************************************************************************************
 * XMClib v2.1.16 - XMC Peripheral Driver Library 
 *
 * Copyright (c) 2015-2017, Infineon Technologies AG
 * All rights reserved.                        
 *                                             
 * Redistribution and use in source and binary forms, with or without modification,are permitted provided that the 
 * following conditions are met:   
 *                                                                              
 * Redistributions of source code must retain the above copyright notice, this list of conditions and the following 
 * disclaimer.                        
 * 
 * Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following 
 * disclaimer in the documentation and/or other materials provided with the distribution.                       
 * 
 * Neither the name of the copyright holders nor the names of its contributors may be used to endorse or promote 
 * products derived from this software without specific prior written permission.                                           
 *                                                                              
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, 
 * INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE  
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE  FOR ANY DIRECT, INDIRECT, INCIDENTAL, 
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR  
 * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, 
 * WHETHER IN CONTRACT, STRICT LIABILITY,OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE 
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.                                                  
 *                                                                              
 * To improve the quality of the software, users are encouraged to share modifications, enhancements or bug fixes with 
 * Infineon Technologies AG dave@infineon.com).                                                          
 *********************************************************************************************************************
 *
 * Change History
 * --------------
 *
 * 2015-02-20:
 *     - Initial
 *      
 * 2015-05-20:
 *     - Documentation updates <br>
 *     - In xmc1_rtc file XMC_RTC_Init function
 *       is modified by adding the RTC running condition check
 *   
 * 2015-06-20:
 *     - Removed version macros and declaration of GetDriverVersion API
 *
 * 2016-05-19:
 *     - Added XMC_RTC_SetTimeStdFormat() and XMC_RTC_SetAlarmStdFormat()
 *
 * @endcond 
 *
 */

#ifndef XMC_RTC_H
#define XMC_RTC_H

/*********************************************************************************************************************
 * HEADER FILES
 *********************************************************************************************************************/

#include <xmc_common.h>
#include <time.h>

/**
 *
 * @addtogroup XMClib XMC Peripheral Library
 * @{
 */

/**
 * @addtogroup RTC
 * @brief RTC driver for XMC microcontroller family.
 *
 * Real-time clock (RTC) is a clock that keeps track of the current time. Precise
 * real time keeping is with a 32.768 KHz external crystal clock or a 32.768 KHz
 * high precision internal clock. It provides a periodic time based interrupt and
 * a programmable alarm interrupt on time match. It also supports wakeup from
 * hibernate.
 *
 * The RTC low level driver provides functions to configure and initialize the RTC
 * hardware peripheral. 
 * 
 * @{
 */

/*********************************************************************************************************************
 * MACROS
 *********************************************************************************************************************/

/*********************************************************************************************************************
 * ENUMS
 *********************************************************************************************************************/

/**
 *  Status return values for RTC low level driver
 */
typedef enum XMC_RTC_STATUS
{
  XMC_RTC_STATUS_OK    = 0U, /**< Operation successful */
  XMC_RTC_STATUS_ERROR = 1U, /**< Operation unsuccessful */
  XMC_RTC_STATUS_BUSY  = 2U  /**< Busy with a previous request */
} XMC_RTC_STATUS_t;

/**
 * Events which enables interrupt request generation
 */
typedef enum XMC_RTC_EVENT
{
  XMC_RTC_EVENT_PERIODIC_SECONDS = RTC_MSKSR_MPSE_Msk, /**< Mask value to enable an event on periodic seconds */
  XMC_RTC_EVENT_PERIODIC_MINUTES = RTC_MSKSR_MPMI_Msk, /**< Mask value to enable an event on periodic seconds */
  XMC_RTC_EVENT_PERIODIC_HOURS   = RTC_MSKSR_MPHO_Msk, /**< Mask value to enable an event on periodic seconds */
  XMC_RTC_EVENT_PERIODIC_DAYS    = RTC_MSKSR_MPDA_Msk, /**< Mask value to enable an event on periodic seconds */
  XMC_RTC_EVENT_PERIODIC_MONTHS  = RTC_MSKSR_MPMO_Msk, /**< Mask value to enable an event on periodic seconds */
  XMC_RTC_EVENT_PERIODIC_YEARS   = RTC_MSKSR_MPYE_Msk, /**< Mask value to enable an event on periodic seconds */
  XMC_RTC_EVENT_ALARM            = RTC_MSKSR_MAI_Msk   /**< Mask value to enable an event on periodic seconds */
} XMC_RTC_EVENT_t;

/**
 *  Months used to program the date  
 */
typedef enum XMC_RTC_MONTH
{
  XMC_RTC_MONTH_JANUARY   = 0U, 
  XMC_RTC_MONTH_FEBRUARY  = 1U,     
  XMC_RTC_MONTH_MARCH     = 2U,         
  XMC_RTC_MONTH_APRIL     = 3U,       
  XMC_RTC_MONTH_MAY       = 4U,         
  XMC_RTC_MONTH_JUNE      = 5U,         
  XMC_RTC_MONTH_JULY      = 6U,           
  XMC_RTC_MONTH_AUGUST    = 7U,         
  XMC_RTC_MONTH_SEPTEMBER = 8U,    
  XMC_RTC_MONTH_OCTOBER   = 9U,      
  XMC_RTC_MONTH_NOVEMBER  = 10U,     
  XMC_RTC_MONTH_DECEMBER  = 11U     
} XMC_RTC_MONTH_t;

/**
 *  Week days used program the date 
 */
typedef enum XMC_RTC_WEEKDAY
{
  XMC_RTC_WEEKDAY_SUNDAY    = 0U, 
  XMC_RTC_WEEKDAY_MONDAY    = 1U,      
  XMC_RTC_WEEKDAY_TUESDAY   = 2U,      
  XMC_RTC_WEEKDAY_WEDNESDAY = 3U,    
  XMC_RTC_WEEKDAY_THURSDAY  = 4U,     
  XMC_RTC_WEEKDAY_FRIDAY    = 5U,       
  XMC_RTC_WEEKDAY_SATURDAY  = 6U      
} XMC_RTC_WEEKDAY_t;

/*********************************************************************************************************************
 * DATA STRUCTURES
 *********************************************************************************************************************/
/*Anonymous structure/union guard start*/
#if defined(__CC_ARM)
  #pragma push
  #pragma anon_unions
#elif defined(__TASKING__)
  #pragma warning 586
#endif


/**
 * Alarm time values of RTC  <br>
 *
 * The structure presents a convenient way to set/obtain the 
 * alarm time values for seconds, minutes, hours, days, month and year of RTC. 
 * The XMC_RTC_SetAlarm() and XMC_RTC_GetAlarm() can be
 * used to populate the structure with the alarm time value of
 * RTC 
 */ 
typedef struct XMC_RTC_ALARM
{
  union
  {
	  uint32_t raw0;
	  struct
	  {
	    uint32_t seconds  : 6; /**< Alarm seconds compare value (0-59: Above this causes this bitfield to be set with 0)*/
	    uint32_t          : 2; 
	    uint32_t minutes  : 6; /**< Alarm minutes compare value (0-59: Above this causes this bitfield to be set with 0)*/
	    uint32_t          : 2;
	    uint32_t hours    : 5; /**< Alarm hours compare value   (0-23: Above this causes this bitfield to be set with 0)*/
	    uint32_t          : 3;
	    uint32_t days     : 5; /**< Alarm days compare value (0-Actual days of month: Above this causes this bitfield to be set with 0)*/
	    uint32_t          : 3;
	  };
  };

  union
  {
    uint32_t raw1;
	  struct
	  {
	    uint32_t          : 8;
	    uint32_t month    : 4; /**< Alarm month compare value (0-11: Above this causes this bitfield to be set with 0) */
	    uint32_t          : 4;
	    uint32_t year     : 16; /**< Alarm year compare value */ 
	  };
  };
} XMC_RTC_ALARM_t;

/**
 * Time values of RTC  <br>
 *
 * The structure presents a convenient way to set/obtain the 
 * time values for seconds, minutes, hours, days, month and year of RTC. 
 * The XMC_RTC_SetTime() and XMC_RTC_GetTime() can be
 * used to populate the structure with the time value of
 * RTC 
 */ 
typedef struct XMC_RTC_TIME
{
  union
  {
    uint32_t raw0;
	  struct
	  {
	    uint32_t seconds  : 6; /**< Seconds time value (0-59: Above this causes this bitfield to be set with 0) */
	    uint32_t          : 2; 
	    uint32_t minutes  : 6; /**< Minutes time value (0-59: Above this causes this bitfield to be set with 0) */
	    uint32_t          : 2;
	    uint32_t hours    : 5; /**< Hours time value   (0-23: Above this causes this bitfield to be set with 0) */
	    uint32_t          : 3;
	    uint32_t days     : 5; /**< Days time value (0-Actual days of month: Above this causes this bitfield to be set with 0)*/
	    uint32_t          : 3;
	  };
  };

  union
  {
    uint32_t raw1;
	  struct
	  {
	    uint32_t daysofweek  : 3; /**< Days of week time value (0-6: Above this causes this bitfield to be set with 0) */
	    uint32_t             : 5;
	    uint32_t month       : 4; /**< Month time value       (0-11: Above this causes this bitfield to be set with 0) */
	    uint32_t             : 4;
	    uint32_t year        : 16; /**< Year time value */
	 };
  };
} XMC_RTC_TIME_t;
/*Anonymous structure/union guard end*/
#if defined(__CC_ARM)
  #pragma pop
#elif defined(__TASKING__)
  #pragma warning restore
#endif

/**
 * RTC initialization with time, alarm and clock divider(prescaler) configurations <br>
 *
 * The structure presents a convenient way to set/obtain the time and alarm configurations  
 * for RTC. The XMC_RTC_Init() can be used to populate the structure with the time and alarm
 * values of RTC.
 */  
typedef struct XMC_RTC_CONFIG
{
  XMC_RTC_TIME_t    time;
  XMC_RTC_ALARM_t   alarm;
  uint16_t          prescaler;
} XMC_RTC_CONFIG_t;

/*******************************************************************************
 * EXTENSIONS
 *******************************************************************************/

#if UC_FAMILY == XMC1
#include "xmc1_rtc.h"
#endif

#if UC_FAMILY == XMC4
#include "xmc4_rtc.h"
#endif

/*******************************************************************************
 * API PROTOTYPES
 *********************************************************************************************************************/

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @param config Constant pointer to a constant ::XMC_RTC_CONFIG_t structure containing the
 *               time, alarm time and clock divider(prescaler) configuration. 
 * @return XMC_RTC_STATUS_t Always returns XMC_RTC_STATUS_OK (It contains only register assignment statements)
 *
 * \par<b>Description: </b><br>
 * Initialize the RTC peripheral <br>
 *
 * \par \if XMC4
 * The function enables the hibernate domain for accessing RTC peripheral registers, configures 
 * internal clock divider, time and alarm values by writing to the CTR.DIV, TIM0, TIM1, ATIM0 and
 * ATIM1 registers.
 * \endif
 *
 * \if XMC1
 * The function ungates the peripheral clock for RTC, configures 
 * internal clock divider, time and alarm values by writing to the CTR.DIV, TIM0, TIM1, ATIM0 and
 * ATIM1 registers.
 * \endif
 */
XMC_RTC_STATUS_t XMC_RTC_Init(const XMC_RTC_CONFIG_t *const config);

/** 
 * @return None
 *
 * \par<b>Description</b><br>
 * Enables RTC peripheral for programming its registers <br>
 *
 * \par \if XMC4
 * Enables the hibernate domain for accessing RTC peripheral registers.
 * \endif
 *
 * \if XMC1
 * Ungates the peripheral clock.
 * \endif
 *
 * \par<b>Related APIs:</b><br>
 * XMC_RTC_Disable(), XMC_SCU_RESET_DeassertPeripheralReset()
 */ 
void XMC_RTC_Enable(void);

/** 
 * @return None
 *
 * \par<b>Description</b><br>
 * Disables RTC peripheral for programming its registers <br>
 *
 * \par \if XMC4
 * Empty function (Hibernate domain is not disabled).
 * \endif
 *
 * \if XMC1
 * Gates the peripheral clock.
 * \endif
 *
 * \par<b>Related APIs:</b><br>
 * XMC_RTC_Enable(), XMC_SCU_RESET_AssertPeripheralReset()
 */ 
void XMC_RTC_Disable(void);

/** 
 * @return None
 *
 * \par<b>Description</b><br>
 * Checks RTC peripheral is enabled for programming its registers <br>
 *
 * \par \if XMC4
 * Checks the hibernate domain is enabled or not.
 * \endif
 *
 * \if XMC1
 * Checks peripheral clock is ungated or not.
 * \endif
 *
 * \par<b>Related APIs:</b><br>
 * XMC_RTC_Enable(), XMC_RTC_Disable(), XMC_SCU_RESET_DeassertPeripheralReset(),
 * XMC_SCU_RESET_AssertPeripheralReset()
 */ 
bool XMC_RTC_IsEnabled(void);
 
/**
 * @return None
 *
 * \par<b>Description</b><br>
 * Enables RTC peripheral to start counting time <br>
 *
 * \par
 * The function starts the RTC for counting time by setting 
 * CTR.ENB bit. Before starting the RTC, it should not be in
 * running mode and also hibernate domain should be enabled.
 *
 * \par<b>Related APIs:</b><br>
 * XMC_RTC_Enable(), XMC_RTC_Stop(), XMC_SCU_RESET_DeassertPeripheralReset()
 */  
void XMC_RTC_Start(void);

/**
 * @return None
 *
 * \par<b>Description</b><br>
 * Disables RTC peripheral to start counting time <br>
 *
 * \par
 * The function stops the RTC for counting time by resetting 
 * CTR.ENB. Before stopping the RTC, hibernate domain should be enabled.
 *
 * \par<b>Related APIs:</b><br>
 * XMC_RTC_Enable(), XMC_RTC_Start(), XMC_SCU_RESET_AssertPeripheralReset()
 */  
void XMC_RTC_Stop(void);
  
/**
 * @param prescaler Prescaler value to be set                 
 * @return None
 *
 * \par<b>Description: </b><br>
 * Sets the RTC module prescaler value <br>
 *
 * \par
 * The function sets the CTR.DIV bitfield to configure the prescalar value. 
 * The default value for the prescalar with the 32.768kHz crystal (or the internal clock) 
 * is 7FFFH for a time interval of 1 sec. Before setting the prescaler value RTC should be
 * in stop mode and hibernate domain should be enabled.
 *
 * \par<b>Related APIs:</b><br>
 * XMC_RTC_Stop(), XMC_RTC_Enable(), XMC_RTC_GetPrescaler()
 */   
void XMC_RTC_SetPrescaler(uint16_t prescaler);

/**
 * @return None
 *
 * \par<b>Description: </b><br>
 * Gets the RTC module prescaler value <br>
 *
 * \par
 * The function reads the CTR.DIV bitfield to get the prescalar value. The default value 
 * for the prescalar with the 32.768kHz crystal (or the internal clock) is 7FFFH for a 
 * time interval of 1 sec.
 *
 * \par<b>Related APIs:</b><br>
 * XMC_RTC_SetPrescaler()
 */   
__STATIC_INLINE uint32_t XMC_RTC_GetPrescaler(void)
{
  return (uint32_t)(((uint32_t)RTC->CTR & (uint32_t)RTC_CTR_DIV_Msk) >> (uint32_t)RTC_CTR_DIV_Pos);
}

/**
 * @param timeval Contstant pointer to a constant ::XMC_RTC_TIME_t structure containing the
 *                time parameters seconds, minutes, hours, days, daysofweek, month and year.     
 * @return None
 *
 * \par<b>Description: </b><br>
 * Sets the RTC module time values <br>
 *
 * \par
 * The function sets the TIM0, TIM1 registers with time values.
 * The values can only be written when RTC is disabled.
 * See the structure ::XMC_RTC_TIME_t for the valid range of time value parameters. <br>
 *
 * \par<b>Related APIs:</b><br>
 * XMC_RTC_GetTime(), XMC_RTC_Stop() 
 */    
void XMC_RTC_SetTime(const XMC_RTC_TIME_t *const timeval);

/**
 * @param time Pointer to a constant ::XMC_RTC_TIME_t structure containing the time parameters
 *             seconds, minutes, hours, days, daysofweek, month and year. 
 * @return None
 *
 * \par<b>Description: </b><br>
 * Gets the RTC module time value <br>
 *
 * \par
 * The function gets the time values from TIM0, TIM1 registers. 
 * See the structure ::XMC_RTC_TIME_t for the valid range of time value parameters. <br>
 *
 * \par<b>Related APIs:</b><br>
 * XMC_RTC_SetTime()
 */    
void XMC_RTC_GetTime(XMC_RTC_TIME_t *const time);

/**
 * @param stdtime Pointer to a ::tm structure containing the time parameters seconds,
 *                minutes, hours, days, daysofweek, month, year(since 1900) and days in a   
 *                year in standard format.           
 * @return None
 *
 * \par<b>Description: </b><br>
 * Sets the RTC module time value in standard format <br>
 *
 * \par
 * The function sets the time values from TIM0, TIM1 registers. 
 *
 * \par<b>Related APIs:</b><br>
 * XMC_RTC_SetTime(), XMC_RTC_GetTime()
 */     
void XMC_RTC_SetTimeStdFormat(const struct tm *const stdtime);

/**
 * @param stdtime Pointer to a constant ::tm structure containing the time parameters seconds,
 *                minutes, hours, days, daysofweek, month, year(since 1900) and days in a   
 *                year in standard format.           
 * @return None
 *
 * \par<b>Description: </b><br>
 * Gets the RTC module time value in standard format <br>
 *
 * \par
 * The function gets the time values from TIM0, TIM1 registers. 
 * See the structure ::XMC_RTC_TIME_t for the valid range of time value parameters. <br>
 * For days the valid range is (1 - Actual days of month), year (since 1900) and 
 * daysinyear (0 -365).
 *
 * \par<b>Related APIs:</b><br>
 * XMC_RTC_SetTime(), XMC_RTC_GetTime()
 */     
void XMC_RTC_GetTimeStdFormat(struct tm *const stdtime);

/**
 * @param alarm Constant pointer to a constant ::XMC_RTC_ALARM_t structure containing the
 *                alarm time parameters alarm seconds, alarm minutes, alarm hours, alarm days, 
 *                alarm daysofweek, alarm month and alarm year.           
 * @return None
 *
 * \par<b>Description: </b><br>
 * Sets the RTC module alarm time value <br>
 *
 * \par
 * The function sets the ATIM0, ATIM1 registers with alarm time values. 
 * See the structure ::XMC_RTC_ALARM_t for the valid range of alarm time value parameters. <br>
 *
 * \par<b>Related APIs:</b><br>
 * XMC_RTC_GetAlarm()
 */     
void XMC_RTC_SetAlarm(const XMC_RTC_ALARM_t *const alarm);

/**
 * @param alarm Pointer to a constant ::XMC_RTC_ALARM_t structure containing the
 *             time parameters alarm seconds, alarm minutes, alarm hours, alarm days, 
 *             alarm daysofweek, alarm month and alarm year.           
 * @return None
 *
 * \par<b>Description: </b><br>
 * Gets the RTC module alarm time value <br>
 *
 * \par
 * The function gets the alarm time values from ATIM0, ATIM1 registers. 
 * See the structure ::XMC_RTC_ALARM_t for the valid range of alarm time value parameters. <br>
 *
 * \par<b>Related APIs:</b><br>
 * XMC_RTC_SetAlarm()
 */    
void XMC_RTC_GetAlarm(XMC_RTC_ALARM_t *const alarm);

/**
 * @param stdtime Pointer to a ::tm structure containing the time parameters alarm seconds,
 *                alarm minutes, alarm hours, alarm days, alarm daysofweek, alarm month,  
 *                alarm year(since 1900) and alarm days in a year in standard format.           
 * @return None
 *
 * \par<b>Description: </b><br>
 * Sets the RTC module alarm time value in standard format <br>
 *
 * \par
 * The function sets the alarm time values from ATIM0, ATIM1 registers. 
 *
 * \par<b>Related APIs:</b><br>
 * XMC_RTC_SetAlarm(), XMC_RTC_GetAlarm()
 */     
void XMC_RTC_SetAlarmStdFormat(const struct tm *const stdtime);

/**
 * @param stdtime Pointer to a constant ::tm structure containing the time parameters alarm seconds,
 *                alarm minutes, alarm hours, alarm days, alarm daysofweek, alarm month,  
 *                alarm year(since 1900) and alarm days in a year in standard format.           
 * @return None
 *
 * \par<b>Description: </b><br>
 * Gets the RTC module alarm time value in standard format <br>
 *
 * \par
 * The function gets the alarm time values from ATIM0, ATIM1 registers. 
 * See the structure ::XMC_RTC_ALARM_t for the valid range of alarm time value parameters. <br>
 * For days the valid range is (1 - Actual days of month), year (since 1900) and 
 * daysinyear (0 -365).
 *
 * \par<b>Related APIs:</b><br>
 * XMC_RTC_SetAlarm(), XMC_RTC_GetAlarm()
 */     
void XMC_RTC_GetAlarmStdFormat(struct tm *const stdtime);

/**
 * @param event A valid RTC event (::XMC_RTC_EVENT_t) or a valid combination of
 *              logically OR'd events
 * @return None
 *
 * \par<b>Description: </b><br>
 * Enable RTC periodic and alarm event(s) <br>
 *
 * \par
 * The function sets the bitfields of MSKSR register to enable interrupt generation 
 * for requested RTC event(s). 
 * Setting the masking value for the event(s) containing in the ::XMC_RTC_EVENT_t leads 
 * to a generation of the interrupt.
 *
 * \par<b>Related APIs:</b><br>
 * XMC_RTC_DisableEvent()
 */
void XMC_RTC_EnableEvent(const uint32_t event);

/**
 * @param event A valid RTC event (::XMC_RTC_EVENT_t) or a valid combination of
 *              logically OR'd events
 * @return None
 *
 * \par<b>Description: </b><br>
 * Disable RTC periodic and alarm event(s) <br>
 *
 * \par
 * The function resets the bitfields of MSKSR register to disable interrupt generation 
 * for requested RTC event(s). 
 * Resetting the masking value for the the event(s) containing in the ::XMC_RTC_EVENT_t blocks 
 * the generation of the interrupt.
 * 
 * \par<b>Related APIs:</b><br>
 * XMC_RTC_EnableEvent()
 */
void XMC_RTC_DisableEvent(const uint32_t event);

/**
 * @param event A valid RTC event (::XMC_RTC_EVENT_t) or a valid combination of
 *              logically OR'd events
 * @return None
 *
 * \par<b>Description: </b><br>
 * Clears periodic and alarm event(s) status <br>
 *
 * \par
 * The function sets the bitfields of CLRSR register to clear status bits in RAWSTAT and STSSR registers. 
 * Setting the value for the the RTC event(s) containing in the ::XMC_RTC_EVENT_t clears the 
 * corresponding status bits in RAWSTAT and STSSR registers.
 * 
 * \par<b>Related APIs:</b><br>
 * XMC_RTC_GetEventStatus()
 */ 
void XMC_RTC_ClearEvent(const uint32_t event);

/**
 * @return None
 *
 * \par<b>Description: </b><br>
 * Gets the RTC periodic and alarm event(s) status <br>
 *
 * \par
 * The function reads the bitfields of STSSR register 
 * to get the status of RTC events. 
 * Reading the value of the register STSSR gives the status of the event(s) containing in the ::XMC_RTC_EVENT_t. 
 * 
 * \par<b>Related APIs:</b><br>
 * XMC_RTC_ClearEvent()
 */
uint32_t XMC_RTC_GetEventStatus(void);

/**
 * @return bool true if RTC is running
 *              false if RTC is not running
 *
 * \par<b>Description: </b><br>
 * Checks the running status of the RTC <br>
 *
 * \par
 * The function reads the bitfield ENB of CTR register 
 * to get the running status of RTC. 
 *
 * \par<b>Related APIs:</b><br>
 * XMC_RTC_Start(), XMC_RTC_Stop()
 */ 
__STATIC_INLINE bool XMC_RTC_IsRunning(void)
{
  return (bool)(RTC->CTR & RTC_CTR_ENB_Msk);
}

#ifdef __cplusplus
}
#endif

/**
 * @}
 */

/**
 * @}
 */

#endif /* XMC_RTC_H */
