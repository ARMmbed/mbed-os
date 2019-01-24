/**
  ******************************************************************************
  * @file    BlueNRG1_rtc.c
  * @author  VMA Application Team
  * @version V2.0.0
  * @date    21-March-2016
  * @brief   This file provides all the RTC firmware functions.
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

/* Includes ------------------------------------------------------------------*/
#include "../../Periph_Driver/inc/BlueNRG1_rtc.h"


/** @addtogroup BLUENRG1_Peripheral_Driver BLUENRG1 Peripheral Driver
  * @{
  */

/** @defgroup RTC_Peripheral  RTC Peripheral
  * @{
  */ 

/** @defgroup RTC_Private_TypesDefinitions Private Types Definitions
  * @{
  */

/**
  * @}
  */ 

/** @defgroup RTC_Private_Defines Private Defines
  * @{
  */ 

/**
  * @}
  */ 

/** @defgroup RTC_Private_Macros Private Macros
  * @{
  */

/**
  * @}
  */ 

/** @defgroup RTC_Private_Variables Private Variables
  * @{
  */

/**
  * @}
  */ 

/** @defgroup RTC_Private_FunctionPrototypes Private Function Prototypes
  * @{
  */
  
/**
  * @}
  */
  
/** @defgroup RTC_Public_Functions Public Functions
* @{
*/

/**
  * @brief  Initializes the RTC peripheral according to the specified
  *         parameters in the RTC_InitStruct.
  * @param  RTC_InitStruct: pointer to a @ref RTC_InitType structure that
  *         contains the configuration information for the specified RTC peripheral.
  * @retval None
  */
void RTC_Init(RTC_InitType* RTC_InitStruct)
{  
  /* Check the parameters */
  assert_param(IS_RTC_TIMER_MODE(RTC_InitStruct->RTC_operatingMode));
  assert_param(IS_PATTERN(RTC_InitStruct->RTC_PATTERN_SIZE));

  RTC->TCR_b.OS = RTC_InitStruct->RTC_operatingMode;
  RTC->TCR_b.SP = RTC_InitStruct->RTC_PATTERN_SIZE;

  RTC->TLR1 = RTC_InitStruct->RTC_TLR1;
  RTC->TLR2 = RTC_InitStruct->RTC_TLR2;
  RTC->TPR1 = RTC_InitStruct->RTC_PATTERN1;
  RTC->TPR2 = RTC_InitStruct->RTC_PATTERN2;
  RTC->TPR3 = RTC_InitStruct->RTC_PATTERN3;
  RTC->TPR4 = RTC_InitStruct->RTC_PATTERN4;

}

/**
* @brief  Fills the RTC_InitStruct member with its default value.
* @param  RTC_InitStruct : pointer to a @ref RTC_InitType structure which will
*         be initialized.
* @retval None
*/
void RTC_StructInit(RTC_InitType* RTC_InitStruct)
{
  /* Reset GPIO init structure parameters values */
  RTC_InitStruct->RTC_operatingMode = RTC_TIMER_PERIODIC;
  RTC_InitStruct->RTC_PATTERN_SIZE = 0;

  RTC_InitStruct->RTC_TLR1 = 0;
  RTC_InitStruct->RTC_TLR2 = 0;
  RTC_InitStruct->RTC_PATTERN1 = 0;
  RTC_InitStruct->RTC_PATTERN2 = 0;
  RTC_InitStruct->RTC_PATTERN3 = 0;
  RTC_InitStruct->RTC_PATTERN4 = 0;
}


/**
  * @brief  Enables or disables selected RTC peripheral.
  * @param  NewState: functional state @ref FunctionalState
  *         This parameter can be: ENABLE or DISABLE.
  * @retval None
  */
void RTC_Cmd(FunctionalState NewState)
{
  /* Check the parameters */
  assert_param(IS_FUNCTIONAL_STATE(NewState));
  
  if (NewState != DISABLE) {
    RTC->TCR_b.EN = SET;
  }
  else {
    RTC->TCR_b.EN = RESET;
    
  }
}

/**
  * @brief  Enables or disables selected RTC interrupt.
  * @param  RTC_IT: specifies the RTC interrupts sources, the value can be
  *         @arg RTC_IT_CLOCKWATCH RTC clock watch interrupt flag
  *         @arg RTC_IT_TIMER RTC timer interrupt flag
  * @param  NewState: functional state @ref FunctionalState
  *         This parameter can be: ENABLE or DISABLE.
  * @retval None
  */
void RTC_IT_Config(uint8_t RTC_IT, FunctionalState NewState)
{
  /* Check the parameters */
  assert_param(IS_FUNCTIONAL_STATE(NewState));
  assert_param(IS_RTC_IT(RTC_IT));
  
  if (RTC_IT == RTC_IT_CLOCKWATCH) {
    /* Enable the selected RTC interrupts */
    RTC->IMSC_b.WIMSC = NewState;
  }
  else {
    /* Disable the selected RTC interrupts */
    RTC->IMSC_b.TIMSC = NewState;
  }
}

/**
  * @brief  Return the RTC interrupt status for the specified IT.
  * @param  RTC_IT: specifies the RTC interrupts sources, the value can be
  *         @arg RTC_IT_CLOCKWATCH RTC clock watch interrupt flag
  *         @arg RTC_IT_TIMER RTC timer interrupt flag
  * @retval  ITStatus: functional state @ref ITStatus
  *         This parameter can be: SET or RESET.
  */
ITStatus RTC_IT_Status(uint8_t RTC_IT)
{
  /* Check the parameters */
  assert_param(IS_RTC_IT(RTC_IT));

  /* Check the status of the specified RTC flag */
  if ((RTC->RIS & RTC_IT) != RESET) {
    /* RTC_IT is set */
    return SET;
  }
  else {
    /* RTC_IT is reset */
    return RESET;
  }
}

/**
  * @brief  Clear selected RTC interrupt.
  * @param  RTC_IT: specifies the RTC interrupts sources, the value can be
  *         @arg RTC_IT_CLOCKWATCH RTC clock watch interrupt flag
  *         @arg RTC_IT_TIMER RTC timer interrupt flag
  * @retval None
  */
void RTC_IT_Clear(uint8_t RTC_IT)
{
  /* Check the parameters */
  assert_param(IS_RTC_IT(RTC_IT));
  
  RTC->ICR = RTC_IT;
}


/**
  * @brief  Enables or disables selected RTC clockwatch.
  * @param  NewState: functional state @ref FunctionalState
  *         This parameter can be: ENABLE or DISABLE.
  * @retval None
  */
void RTC_ClockwatchCmd(FunctionalState NewState)
{
  /* Check the parameters */
  assert_param(IS_FUNCTIONAL_STATE(NewState));
  
  if (NewState != DISABLE) {
    RTC->CTCR_b.CWEN = SET;
  }
  else {
    RTC->CTCR_b.CWEN = RESET;
  }
}

/**
  * @brief  Initialize the RTC clockwatch with present time and date.
  * @param  RTC_DateTime: pointer to a @ref RTC_DateTimeType structure that
  *         contains the specified time and date setting.
  * @retval None
  */
void RTC_SetTimeDate(RTC_DateTimeType* RTC_DateTime)
{
  /* Check the parameters */
  assert_param(IS_SECOND(RTC_DateTime->Second));
  assert_param(IS_MINUTE(RTC_DateTime->Minute));
  assert_param(IS_HOUR(RTC_DateTime->Hour));
  assert_param(IS_WEEKDAY(RTC_DateTime->WeekDay));
  assert_param(IS_MONTHDAY(RTC_DateTime->MonthDay));
  assert_param(IS_MONTH(RTC_DateTime->Month));
  assert_param(IS_YEAR(RTC_DateTime->Year));
  
  /** Select seconds from the data structure */
  RTC->CWDLR_b.CWSECL = RTC_DateTime->Second;
  
  /** Select minutes from the data structure */
  RTC->CWDLR_b.CWMINL = RTC_DateTime->Minute;
  
  /** Select hours from the data structure */
  RTC->CWDLR_b.CWHOURL = RTC_DateTime->Hour;
  
  /** Select week day from the data structure */
  RTC->CWDLR_b.CWDAYWL = RTC_DateTime->WeekDay;
  
  /** Select month day from the data structure */
  RTC->CWDLR_b.CWDAYML = RTC_DateTime->MonthDay;
  
  /** Select month from the data structure */
  RTC->CWDLR_b.CWMONTHL = RTC_DateTime->Month;
  
  /* Store data to load register */
  RTC->CWYLR_b.CWYEARL = RTC_DateTime->Year;
}

/**
  * @brief  Set the RTC clockwatch match date and time registers.
  * @param  RTC_MatchDataTime: pointer to a @ref RTC_DateTimeType structure that
  *         contains the matching time and date setting.
  * @retval None
  */
void RTC_SetMatchTimeDate(RTC_DateTimeType* RTC_MatchDataTime)
{
  /* Check the parameters */
  assert_param(IS_SECOND(RTC_MatchDataTime->Second));
  assert_param(IS_MINUTE(RTC_MatchDataTime->Minute));
  assert_param(IS_HOUR(RTC_MatchDataTime->Hour));
  assert_param(IS_MATCH_WEEKDAY(RTC_MatchDataTime->WeekDay));
  assert_param(IS_MATCH_MONTHDAY(RTC_MatchDataTime->MonthDay));
  assert_param(IS_MATCH_MONTH(RTC_MatchDataTime->Month));
  assert_param(IS_MATCH_YEAR(RTC_MatchDataTime->Year));
  
  /* Select seconds from the data structure */
  RTC->CWDMR_b.CWSECM = RTC_MatchDataTime->Second;
  
  /* Select minutes from the data structure */
  RTC->CWDMR_b.CWMINM = RTC_MatchDataTime->Minute;
  
  /* Select hours from the data structure */
  RTC->CWDMR_b.CWHOURM = RTC_MatchDataTime->Hour;
  
  /* Select week day from the data structure */
  RTC->CWDMR_b.CWDAYWM = RTC_MatchDataTime->WeekDay;
  
  /* Select month day from the data structure */
  RTC->CWDMR_b.CWDAYMM = RTC_MatchDataTime->MonthDay;
  
  /* Select month from the data structure */
  RTC->CWDMR_b.CWMONTHM = RTC_MatchDataTime->Month;
  
/* Select year from the data structure */
  RTC->CWYMR_b.CWYEARM = RTC_MatchDataTime->Year;
}

/**
  * @brief  Set the RTC clockwatch match date and time registers.
  * @param  RTC_DataTime: pointer to a @ref RTC_DateTimeType structure that
  *         is filled with the configured values of time and date.
  * @retval None
  */
void RTC_GetTimeDate(RTC_DateTimeType* RTC_DataTime)
{
  /* Select seconds from the RTC register */
  RTC_DataTime->Second = RTC->CWDR_b.CWSEC;
  
  /* Select minutes from the RTC register */
  RTC_DataTime->Minute= RTC->CWDR_b.CWMIN;
  
  /* Select hours from the RTC register */
  RTC_DataTime->Hour= RTC->CWDR_b.CWHOUR;
  
  /* Select week day from the RTC register */
  RTC_DataTime->WeekDay = RTC->CWDR_b.CWDAYW;
  
  /* Select month day from the RTC register */
  RTC_DataTime->MonthDay = RTC->CWDR_b.CWDAYM;
  
  /* Select month from the RTC register */
  RTC_DataTime->Month = RTC->CWDR_b.CWMONTH;
  
  /* Select year from the RTC register */
  RTC_DataTime->Year = RTC->CWYR_b.CWYEAR;
}

/**
  * @brief  Return the actual timer counter.
  * @param  None
  * @retval uint32_t the current counter value.
  */
uint32_t RTC_GetTimerValue(void)
{
  /* Return the current Time Data Register */
  return RTC->TDR;
}

/**
  * @brief  Return the actual number of interrupt generated by RTC.
  * @param  None
  * @retval uint32_t the actual number of interrupt generated by RTC.
  */
uint32_t RTC_GetNumberIrqGenerated(void)
{
  /* Return the current Time Data Register */
  return RTC->TIN;
}


/**
  * @brief  Enables or disables the auto start on load register or pattern register write operation.
  * @param  NewState: functional state @ref FunctionalState
  *         This parameter can be: ENABLE or DISABLE.
  * @retval None
  */
void RTC_AutoStart(FunctionalState NewState)
{
  /* Check the parameters */
  assert_param(IS_FUNCTIONAL_STATE(NewState));
  
  if (NewState != DISABLE) {
    RTC->TCR_b.S = SET;
  }
  else {
    RTC->TCR_b.S = RESET;
  }
}


/**
  * @brief  Enables or disables the use of the trimmed 32 kHz clock.
  * @param  NewState: functional state @ref FunctionalState
  *         This parameter can be: ENABLE or DISABLE.
  * @retval None
  */
void RTC_SelectTrimmedClock(FunctionalState NewState)
{
  /* Check the parameters */
  assert_param(IS_FUNCTIONAL_STATE(NewState));
  
  if (NewState != DISABLE) {
    RTC->TCR_b.CLK = SET;
  }
  else {
    RTC->TCR_b.CLK = RESET;
  }
}

/**
  * @}
  */

/**
  * @}
  */

/**
  * @}
  */

/******************* (C) COPYRIGHT 2016 STMicroelectronics *****END OF FILE****/
