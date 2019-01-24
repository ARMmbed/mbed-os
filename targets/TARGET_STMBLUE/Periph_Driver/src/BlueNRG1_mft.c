/**
  ******************************************************************************
  * @file    BlueNRG1_mft.c
  * @author  VMA Application Team
  * @version V2.1.0
  * @date    16-Feb-2018
  * @brief   This file provides all the MFT firmware functions.
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
#include "../../Periph_Driver/inc/BlueNRG1_mft.h"


/** @addtogroup BLUENRG1_Peripheral_Driver BLUENRG1 Peripheral Driver
  * @{
  */


/** @defgroup MFT_Peripheral  MFT Peripheral
  * @{
  */
  
/** @defgroup MFT_Private_TypesDefinitions Private Types Definitions
  * @{
  */

/**
  * @}
  */ 

/** @defgroup MFT_Private_Defines Private Defines
  * @{
  */ 

/**
  * @}
  */ 

/** @defgroup MFT_Private_Macros Private Macros
  * @{
  */

/**
  * @}
  */ 

/** @defgroup MFT_Private_Variables Private Variables
  * @{
  */

/**
  * @}
  */ 

/** @defgroup MFT_Private_FunctionPrototypes Private Function Prototypes
  * @{
  */
  
  /**
  * @}
  */ 

/** @defgroup MFT_Public_Functions Public Functions
  * @{
  */

/**
  *@brief DeInit the MFTx peripheral.
  *@param  MFTx: where x can be 1 or 2 to select the MFT peripheral @ref MFT_Type
  *@retval None
  */
void MFT_DeInit(MFT_Type* MFTx)
{
  MFTx->TNPRSC = 0x00000000;
  MFTx->TNCKC = 0x00000000;
  MFTx->TNMCTRL = 0x00000000;
  MFTx->TNICLR = 0x0000000F;

}


/**
  *@brief Initializes the MFTx peripheral according to the specified
  *       parameters in the MFT_InitStruct.
  *@param  MFTx: where x can be 1 or 2 to select the MFT peripheral @ref MFT_Type
  *@param  MFT_InitStruct: pointer to a @ref MFT_InitType structure that
  *         contains the configuration information for the specified MFT peripheral.
  *@retval None
  */
void MFT_Init(MFT_Type* MFTx, MFT_InitType* MFT_InitStruct)
{
  
  /* Check the parameters */
  assert_param(IS_MFT(MFTx));
  assert_param(IS_MFT_MODE(MFT_InitStruct->MFT_Mode));
  assert_param(IS_MFT_CLOCK_SEL(MFT_InitStruct->MFT_Clock1));
  assert_param(IS_MFT_CLOCK_SEL(MFT_InitStruct->MFT_Clock2));
  
  MFTx->TNMCTRL_b.TNAEN = RESET;
  MFTx->TNMCTRL_b.TNBEN = RESET;
  
  MFTx->TNCKC_b.TNC1CSEL = MFT_NO_CLK;
  MFTx->TNCKC_b.TNC2CSEL = MFT_NO_CLK;
  
  if(MFT_InitStruct->MFT_Mode == MFT_MODE_1a) {
    MFTx->TNMCTRL_b.TNMDSEL = MFT_MODE_1;
    MFTx->TNMCTRL_b.TNPTEN = SET;
  }
  else {
    MFTx->TNMCTRL_b.TNMDSEL = MFT_InitStruct->MFT_Mode;
    MFTx->TNMCTRL_b.TNPTEN = RESET;
  }
  
  MFTx->TNPRSC = MFT_InitStruct->MFT_Prescaler;
  MFTx->TNMCTRL_b.TNEN = SET;
  MFTx->TNCRA = MFT_InitStruct->MFT_CRA;
  MFTx->TNCRB = MFT_InitStruct->MFT_CRB;
  MFTx->TNMCTRL_b.TNEN = RESET;
  
  MFTx->TNCKC_b.TNC1CSEL = MFT_InitStruct->MFT_Clock1;
  MFTx->TNCKC_b.TNC2CSEL = MFT_InitStruct->MFT_Clock2;
}

/**
  * @brief  Fills each MFT_InitStruct member with its default value.
  * @param  MFT_InitStruct : pointer to a @ref MFT_InitType structure which will
  *         be initialized.
  * @retval None
  */
void MFT_StructInit(MFT_InitType* MFT_InitStruct)
{
  /* Reset MFT init structure parameters values */
  MFT_InitStruct->MFT_Clock1 = MFT_NO_CLK;
  MFT_InitStruct->MFT_Clock2 = MFT_NO_CLK;
  MFT_InitStruct->MFT_CRA = 0;
  MFT_InitStruct->MFT_CRB = 0;
  MFT_InitStruct->MFT_Mode = MFT_MODE_1;
  MFT_InitStruct->MFT_Prescaler = 0;
}

/**
  *@brief  Enable/Disable the MFT
  *@param  MFTx: where x can be 1 or 2 to select the MFT peripheral @ref MFT_Type
  *@param  NewState: functional state @ref FunctionalState
  *         This parameter can be: ENABLE or DISABLE.
  *@retval None
  */
void MFT_Cmd(MFT_Type* MFTx, FunctionalState NewState)
{
  /* Check the parameters */
  assert_param(IS_MFT(MFTx));
  assert_param(IS_FUNCTIONAL_STATE(NewState));
  
  if (NewState == ENABLE) {
    MFTx->TNMCTRL_b.TNEN = SET;
  }
  else {
    MFTx->TNMCTRL_b.TNEN = RESET;
  }
}


/**
  *@brief  Configure edges sensibility of TnA and TnB
  *@param  MFTx: where x can be 1 or 2 to select the MFT peripheral @ref MFT_Type
  *@param  MFT_TnA_edge : falling or rising edge
  *@param  MFT_TnB_edge : falling or rising edge, these values can be
  *         @arg MFT_FALLING on rising edge
  *         @arg MFT_RISING on falling edge
  *@retval None
  */
void MFT_TnEDGES(MFT_Type* MFTx, uint32_t MFT_TnA_edge, uint32_t MFT_TnB_edge)
{
  /* Check the parameters */
  assert_param(IS_MFT(MFTx));
  assert_param(IS_MFT_Tn_EDGE(MFT_TnA_edge));
  assert_param(IS_MFT_Tn_EDGE(MFT_TnB_edge));
  
  MFTx->TNMCTRL_b.TNAEDG = MFT_TnA_edge;
  MFTx->TNMCTRL_b.TNBEDG = MFT_TnB_edge;
}

/**
  *@brief  Select trigger source for pulse-train generation in Mode 1a
  *@param  MFTx: where x can be 1 or 2 to select the MFT peripheral @ref MFT_Type
  *@param  MFT_Trigger : software or TnB trigger, this value can be
  *         @arg MFT_TnB_TRIGGER trigger on TnB edge
  *         @arg MFT_SOFTWARE_TRIGGER trigger software
  *@retval None
  */
void MFT_PulseTrainTriggerSelect(MFT_Type* MFTx, uint32_t MFT_Trigger)
{
  /* Check the parameters */
  assert_param(IS_MFT(MFTx));
  assert_param(IS_MFT_PULSETRAIN_TRIGGER(MFT_Trigger));
  
  MFTx->TNMCTRL_b.TNPTSE = MFT_Trigger;
}

/**
  *@brief  Pulse-train trigger generation in Mode 1a
  *@param  MFTx: where x can be 1 or 2 to select the MFT peripheral @ref MFT_Type
  *@retval None
  */
void MFT_PulseTrainSoftwareTrigger(MFT_Type* MFTx)
{
  /* Check the parameters */
  assert_param(IS_MFT(MFTx));
  
  MFTx->TNMCTRL_b.TNPTET = SET;
}

/**
  *@brief  Return the status of the Pulse-train event trigger in Mode 1a
  *@param  MFTx: where x can be 1 or 2 to select the MFT peripheral @ref MFT_Type
  *@retval If pule-train event trigger has occurred or not:
  *        Possible values are:
  *        RESET: No Pulse-Train Event Trigger occurred.
  *        SET: Pulse-Train Event Trigger occurred. Pulse-train generation not yet finished.
  */
uint8_t MFT_PulseTrainEventTriggerStatus(MFT_Type* MFTx)
{
  /* Check the parameters */
  assert_param(IS_MFT(MFTx));
  
  return MFTx->TNMCTRL_b.TNPTET;
}

/**
  *@brief  Set or Reset TnAEN bit
  *@param  MFTx: where x can be 1 or 2 to select the MFT peripheral @ref MFT_Type
  *@param  TnX : set or reset, @ref MFT_TnAEN
  *         @arg MFT_TnA select internal pin TnA
  *         @arg MFT_TnB select internal pin TnB
  *@param  NewState: functional state @ref FunctionalState
  *         This parameter can be: ENABLE or DISABLE.
  *@retval None
  */
void MFT_TnXEN(MFT_Type* MFTx, uint8_t TnX, FunctionalState NewState)
{
  /* Check the parameters */
  assert_param(IS_MFT(MFTx));
  assert_param(IS_MFT_TnX(TnX));
  assert_param(IS_FUNCTIONAL_STATE(NewState));
  
  if(TnX == MFT_TnA) {
    MFTx->TNMCTRL_b.TNAEN = NewState;
  }
  else if(TnX == MFT_TnB) {
    MFTx->TNMCTRL_b.TNBEN = NewState;
  }
}


/**
  *@brief  Set or Reset TnAEN bit
  *@param  MFT_TimerType : select the timer and the pin, this value can be
  *         @arg MFT1_TIMERA Select Timer MFT1 and input TnA
  *         @arg MFT1_TIMERB Select Timer MFT1 and input TnB
  *         @arg MFT2_TIMERA Select Timer MFT2 and input TnA
  *         @arg MFT2_TIMERB Select Timer MFT2 and input TnB
  *@param  MFT_Pin : the IO number from IO0 to IO14 (0, 1, .. 14)
  *@retval None
  */
void MFT_SelectCapturePin( uint32_t MFT_TimerType, uint8_t MFT_Pin)
{
  /* Check the parameters */
  assert_param(IS_MFT_TIMER(MFT_TimerType));
  assert_param(IS_MFT_INPUT_IO(MFT_Pin));
  
  /* set counter */
  if(MFT_TimerType == MFT1_TIMERA) {
    GPIO->MFTX_b.MFT1_TIMER_A = MFT_Pin;
  }
  else if(MFT_TimerType == MFT1_TIMERB) {
    GPIO->MFTX_b.MFT1_TIMER_B = MFT_Pin;
  }
  else if(MFT_TimerType == MFT2_TIMERA) {
    GPIO->MFTX_b.MFT2_TIMER_A = MFT_Pin;
  }
  else if(MFT_TimerType == MFT2_TIMERB) {
    GPIO->MFTX_b.MFT2_TIMER_B = MFT_Pin;
  }
  
}


/**
  *@brief  Set the timer counter.
  *@param  MFTx: where x can be 1 or 2 to select the MFT peripheral @ref MFT_Type
  *@param  MFT_Cnt1 : set the counter value for CNT1
  *@param  MFT_Cnt2 : set the counter value for CNT2
  *@retval None
  */
void MFT_SetCounter(MFT_Type* MFTx, uint16_t MFT_Cnt1, uint16_t MFT_Cnt2)
{
  /* Check the parameters */
  assert_param(IS_MFT(MFTx));
  
  /* Set counters */
  MFTx->TNCNT1 = MFT_Cnt1;
  MFTx->TNCNT2 = MFT_Cnt2;
}


/**
  *@brief  Set the timer counter1.
  *@param  MFTx: where x can be 1 or 2 to select the MFT peripheral @ref MFT_Type
  *@param  MFT_Cnt : set the counter value for CNT1
  *@retval None
  */
void MFT_SetCounter1(MFT_Type* MFTx, uint16_t MFT_Cnt)
{
  /* Check the parameters */
  assert_param(IS_MFT(MFTx));
  
  /* Set counter */
  MFTx->TNCNT1 = MFT_Cnt;
}

/**
  *@brief  Get the timer counter1.
  *@param  MFTx: where x can be 1 or 2 to select the MFT peripheral @ref MFT_Type
  *@param  None
  *@retval uint16_t the counter 1 value
  */
uint16_t MFT_GetCounter1(MFT_Type* MFTx)
{
  /* Check the parameters */
  assert_param(IS_MFT(MFTx));
  
  /* Return counter */
  return MFTx->TNCNT1;
}

/**
  *@brief  Set the timer counter2.
  *@param  MFTx: where x can be 1 or 2 to select the MFT peripheral @ref MFT_Type
  *@param  MFT_Cnt : set the counter value for CNT2
  *@retval None
  */
void MFT_SetCounter2(MFT_Type* MFTx, uint16_t MFT_Cnt)
{
  /* Check the parameters */
  assert_param(IS_MFT(MFTx));
  
  /* set counter */
  MFTx->TNCNT2 = MFT_Cnt;
}

/**
  *@brief  Get the timer counter2.
  *@param  MFTx: where x can be 1 or 2 to select the MFT peripheral @ref MFT_Type
  *@param  None
  *@retval uint16_t the counter 2 value
  */
uint16_t MFT_GetCounter2(MFT_Type* MFTx)
{
  /* Check the parameters */
  assert_param(IS_MFT(MFTx));
  
  /* Return counter */
  return MFTx->TNCNT2;
}

/**
  *@brief  Enable specific MFT interrupt
  *@param  MFTx: where x can be 1 or 2 to select the MFT peripheral @ref MFT_Type
  *@param  MFT_IrqSource: interrupt source to enable, this value can be
  *         @arg MFT_IT_TNA Select the interrupt source A
  *         @arg MFT_IT_TNB Select the interrupt source B
  *         @arg MFT_IT_TNC Select the interrupt source C
  *         @arg MFT_IT_TND Select the interrupt source D
  *@param  NewState: functional state @ref FunctionalState
  *         This parameter can be: ENABLE or DISABLE.
  *@retval None
  */
void MFT_EnableIT(MFT_Type* MFTx, uint8_t MFT_IrqSource, FunctionalState NewState)
{
  /* Check the parameters */
  assert_param(IS_MFT(MFTx));
  assert_param(IS_MFT_INTERRUPT(MFT_IrqSource));
  assert_param(IS_FUNCTIONAL_STATE(NewState));
  
  if (NewState == ENABLE) {
    SET_BIT(MFTx->TNICTRL, (MFT_IrqSource << 4));
  }
  else {
    CLEAR_BIT(MFTx->TNICTRL, (MFT_IrqSource << 4));
  }
}

/**
  *@brief Clear specific MFT interrupt
  *@param  MFTx: where x can be 1 or 2 to select the MFT peripheral @ref MFT_Type
  *@param  MFT_IrqSource: interrupt source to enable, this value can be
  *         @arg MFT_IT_TNA Select the interrupt source A
  *         @arg MFT_IT_TNB Select the interrupt source B
  *         @arg MFT_IT_TNC Select the interrupt source C
  *         @arg MFT_IT_TND Select the interrupt source D
  *@retval None
  */
void MFT_ClearIT(MFT_Type* MFTx, uint8_t MFT_IrqSource)
{
  /* Check the parameters */
  assert_param(IS_MFT(MFTx));
  assert_param(IS_MFT_INTERRUPT(MFT_IrqSource));
  
  SET_BIT(MFTx->TNICLR, MFT_IrqSource);
}

/**
  *@brief Return interrupt status
  *@param  MFTx: where x can be 1 or 2 to select the MFT peripheral @ref MFT_Type
  *@param  MFT_IrqSource: interrupt source to enable, this value can be
  *         @arg MFT_IT_TNA Select the interrupt source A
  *         @arg MFT_IT_TNB Select the interrupt source B
  *         @arg MFT_IT_TNC Select the interrupt source C
  *         @arg MFT_IT_TND Select the interrupt source D
  *@retval  ITStatus: functional state @ref ITStatus
  *         This parameter can be: SET or RESET.
  */
ITStatus MFT_StatusIT(MFT_Type* MFTx, uint8_t MFT_IrqSource)
{
  /* Check the parameters */
  assert_param(IS_MFT(MFTx));
  assert_param(IS_MFT_INTERRUPT(MFT_IrqSource));
  
  if((MFTx->TNICTRL & MFT_IrqSource) != (uint32_t)RESET) {
    return SET;
  }
  else {
    return RESET;
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
