/**
  ******************************************************************************
  * @file    BlueNRG1_rtc.h
  * @author  VMA Application Team
  * @version V2.0.0
  * @date    21-March-2016
  * @brief   This file contains all the functions prototypes for the RTC 
  *          firmware library.
  ******************************************************************************
  * @attention
  *
  * THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
  * WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE
  * TIME. AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY
  * DIRECT, INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING
  * FROM THE CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE
  * CODING INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
  *
  * <h2><center>&copy; COPYRIGHT 2016 STMicroelectronics</center></h2>
  ******************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef BLUENRG1_RTC_H
#define BLUENRG1_RTC_H

#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "BlueNRG_x_device.h"

/** @addtogroup BLUENRG1_Peripheral_Driver BLUENRG1 Peripheral Driver
  * @{
  */

/** @addtogroup RTC_Peripheral  RTC Peripheral
  * @{
  */

/** @defgroup RTC_Exported_Types Exported Types
  * @{
  */

/** 
  * @brief Structure definition of RTC initialization
  */
typedef struct
{ 
  uint8_t RTC_operatingMode;               /*!< Specifies the operating mode.
                                            This parameter can be a value of @ref RTC_Timer_OperatingMode */
                   
  uint32_t RTC_TLR1;                        /*!< Load TLR1 register.
                                            32 bit value to load  */
              
  uint32_t RTC_TLR2;                        /*!< Load TLR2 register.
                                            32 bit value to load  */

  uint32_t RTC_PATTERN_SIZE;                /*!< Define the pattern size to use.
                                            7 bit pattern, 0x0 size of 1 and 0x7F size of 128 */
  
  uint32_t RTC_PATTERN1;                    /*!< Define the pattern, '0' relaod TLR1 and '1' reload TLR2.
                                            First 32 bit pattern  */
                                      
  uint32_t RTC_PATTERN2;                    /*!< Define the pattern, '0' relaod TLR1 and '1' reload TLR2.
                                            Second 32 bit pattern  */

  uint32_t RTC_PATTERN3;                    /*!< Define the pattern, '0' relaod TLR1 and '1' reload TLR2.
                                            Third 32 bit pattern */
  
  uint32_t RTC_PATTERN4;                    /*!< Define the pattern, '0' relaod TLR1 and '1' reload TLR2.
                                            Fourth 32 bit pattern */
} RTC_InitType;


/** 
  * @brief Structure definition of RTC DateTime
  */
typedef struct
{ 
  uint8_t Second;                       /*!< Specify the present second to be set to RTC clockwatch.
                                            This parameter can be integer value in range from 0 to 59 */
                   
  uint8_t Minute;                        /*!< Specify the present minute to be set to RTC clockwatch.
                                            This parameter can be integer value in range from 0 to 59 */
              
  uint8_t Hour;                          /*!< Specify the present hour to be set to RTC clockwatch.
                                            This parameter can be integer value in range from 0 to 23 */

  uint8_t WeekDay;                        /*!< Specify the present day in the week to be set to RTC clockwatch.
                                            This parameter can be an integer value in range from 0 to 7 */
  
  uint8_t MonthDay;                      /*!< Specify the present day in the month to be set to RTC clockwatch.
                                            This parameter can be an integer value in range from 1 to 31 (depends on month) */
                                      
  uint8_t Month;                          /*!< Specify the present month to be set to RTC clockwatch.
                                            This parameter can be an integer value in range from 1 to 12 (depends on month) */

  uint16_t Year;                          /*!< Specify the present year to be set to RTC clockwatch.
                                            This parameter can be an integer value in range from 0 to 3999 (depends on month) */
  
} RTC_DateTimeType;

/**
  * @}
  */


/** @defgroup RTC_Exported_Constants Exported Constants
  * @{
  */

/** @defgroup RTC_Time_and_Data_Definitions Time and Data Definitions 
  * @{
  */ 

#define RTC_Month_January              ((uint8_t)0x01)
#define RTC_Month_February             ((uint8_t)0x02)
#define RTC_Month_March                ((uint8_t)0x03)
#define RTC_Month_April                ((uint8_t)0x04)
#define RTC_Month_May                  ((uint8_t)0x05)
#define RTC_Month_June                 ((uint8_t)0x06)
#define RTC_Month_July                 ((uint8_t)0x07)
#define RTC_Month_August               ((uint8_t)0x08)
#define RTC_Month_September            ((uint8_t)0x09)
#define RTC_Month_October              ((uint8_t)0x10)
#define RTC_Month_November             ((uint8_t)0x11)
#define RTC_Month_December             ((uint8_t)0x12)

#define RTC_Weekday_Monday             ((uint8_t)0x01)
#define RTC_Weekday_Tuesday            ((uint8_t)0x02)
#define RTC_Weekday_Wednesday          ((uint8_t)0x03)
#define RTC_Weekday_Thursday           ((uint8_t)0x04)
#define RTC_Weekday_Friday             ((uint8_t)0x05)
#define RTC_Weekday_Saturday           ((uint8_t)0x06)
#define RTC_Weekday_Sunday             ((uint8_t)0x07)

#define IS_SECOND(N) ((N) < 60)
#define IS_MINUTE(N) ((N) < 60)
#define IS_HOUR(N) ((N) < 24)
#define IS_WEEKDAY(N) (((N) > 0) && ((N) < 8))
#define IS_MONTHDAY(N) (((N) > 0) && ((N) < 32))
#define IS_MONTH(N) (((N) > 0) && ((N) < 13))
#define IS_YEAR(N) (((N) > 0) && ((N) < 4000))

#define IS_MATCH_WEEKDAY(N) ((N) < 8)
#define IS_MATCH_MONTHDAY(N) ((N) < 32)
#define IS_MATCH_MONTH(N) ((N) < 13)
#define IS_MATCH_YEAR(N) ((N) < 4000)


/**
  * @}
  */



/** @defgroup RTC_Interrupts_Definitions  Interrupts Definitions
  * @{
  */
#define RTC_IT_CLOCKWATCH             ((uint8_t)1)  /*!< RTC clock watch interrupt */
#define RTC_IT_TIMER                  ((uint8_t)2)  /*!< RTC timer interrupt       */


/** This macro checks if IT is a valid combination of interrupt */
#define IS_RTC_IT(IT) (((IT) == RTC_IT_CLOCKWATCH) || \
                       ((IT) == RTC_IT_TIMER))

/**
  * @}
  */
  
/** @defgroup RTC_Timer_Operating_Mode_Definitions    Timer Operating Mode Definitions
  * @{
  */
#define RTC_TIMER_PERIODIC            ((uint8_t)0)  /*!< RTC timer periodic mode */
#define RTC_TIMER_ONESHOT             ((uint8_t)1)  /*!< RTC timer one shot mode */


/** This macro checks if IT is a valid combination of interrupt */
#define IS_RTC_TIMER_MODE(IT) (((IT) == RTC_TIMER_PERIODIC) || \
                               ((IT) == RTC_TIMER_ONESHOT))

/**
  * @}
  */

/** @defgroup RTC_Pattern_Size_Definitions   Pattern Size Definitions
  * @{
  */

#define IS_PATTERN(N) ((N) < 0x80)

/**
  * @}
  */

/**
  * @}
  */

/** @defgroup RTC_Exported_Functions Exported Functions
  * @{
  */

void RTC_Init(RTC_InitType* RTC_InitStruct);
void RTC_StructInit(RTC_InitType* RTC_InitStruct);
void RTC_Cmd(FunctionalState NewState);
void RTC_IT_Config(uint8_t RTC_IT, FunctionalState NewState);
ITStatus RTC_IT_Status(uint8_t RTC_IT);
void RTC_IT_Clear(uint8_t RTC_IT);

void RTC_ClockwatchCmd(FunctionalState NewState);
void RTC_SetTimeDate(RTC_DateTimeType* RTC_DateTime);
void RTC_SetMatchTimeDate(RTC_DateTimeType* RTC_MatchDataTime);
void RTC_GetTimeDate(RTC_DateTimeType* RTC_DataTime);

uint32_t RTC_GetTimerValue(void);
void RTC_AutoStart(FunctionalState NewState);
void RTC_SelectTrimmedClock(FunctionalState NewState);
uint32_t RTC_GetNumberIrqGenerated(void);

/**
  * @}
  */

/**
  * @}
  */

/**
  * @}
  */

#ifdef __cplusplus
}
#endif

#endif /* BLUENRG1_RTC_H */

/******************* (C) COPYRIGHT 2016 STMicroelectronics *****END OF FILE****/
