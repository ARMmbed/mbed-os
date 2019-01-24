/**
  ******************************************************************************
  * @file    BlueNRG1_mft.h
  * @author  VMA Application Team
  * @version V2.1.0
  * @date    16-Feb-2018
  * @brief   This file contains all the functions prototypes for the MFT 
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
#ifndef BLUENRG1_MFT_H
#define BLUENRG1_MFT_H

#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "BlueNRG_x_device.h"

/** @addtogroup BLUENRG1_Peripheral_Driver BLUENRG1 Peripheral Driver
  * @{
  */

/** @addtogroup MFT_Peripheral  MFT Peripheral
  * @{
  */

/** @defgroup MFT_Exported_Types Exported Types
  * @{
  */

/**
  * @brief Structure definition of MFT initialization
  */
 typedef struct
{
  uint8_t MFT_Mode;        /*!< Specifies the MFT mode.
                                 This value can be a value of @ref MFT_Mode_Definition */

  uint8_t MFT_Clock1;      /*!< Specifies the MFT clock selection, for counter A.
                                 This value can be a value of @ref MFT_Clock_Selector_Definition */
                 
  uint8_t MFT_Clock2;      /*!< Specifies the MFT clock selection, for counter B.
                                 This value can be a value of @ref MFT_Clock_Selector_Definition */
                  
  uint8_t MFT_Prescaler;    /*!< Specifies the MFT prescaler only for MFT_PRESCALED_CLK */
                                  
  uint16_t MFT_CRA;         /*!< Specifies the MFT CRA init value. */

  uint16_t MFT_CRB;         /*!< Specifies the MFT CRB init value. */
                                                 
} MFT_InitType;


/**
  * @}
  */

/** @defgroup MFT_Exported_Constants Exported Constants
  * @{
  */

/** @defgroup MFT_Peripheral_Name_Definitions  Peripheral Name Definitions
  * @{
  */

/** This macro checks if PERIPH is a valid MFT peripheral */
#define IS_MFT(PERIPH) (((PERIPH) == MFT1) || ((PERIPH) == MFT2))

/**
  * @}
  */
  
/** @defgroup MFT_Mode_Definitions Mode Definitions
  * @{
  */
  
#define MFT_MODE_1      ((uint8_t)0x00) /*!< Mode 1 */
#define MFT_MODE_1a     ((uint8_t)0x80) /*!< Mode 1a */
#define MFT_MODE_2      ((uint8_t)0x01) /*!< Mode 2 */
#define MFT_MODE_3      ((uint8_t)0x02) /*!< Mode 3 */
#define MFT_MODE_4      ((uint8_t)0x03) /*!< Mode 4 */

#define IS_MFT_MODE(MODE) (((MODE) == MFT_MODE_1) || \
                            ((MODE) == MFT_MODE_1a) || \
                            ((MODE) == MFT_MODE_2) || \
                            ((MODE) == MFT_MODE_3) || \
                            ((MODE) == MFT_MODE_4))
              
/**
  * @}
  */
  
/** @defgroup MFT_Interrupt_Definitions Interrupt Definitions
  * @{
  */
  
#define MFT_IT_TNA      ((uint32_t)0x1) /*!< interrupt TnAPND */
#define MFT_IT_TNB      ((uint32_t)0x2) /*!< interrupt TnBPND */
#define MFT_IT_TNC      ((uint32_t)0x4) /*!< interrupt TnCPND */
#define MFT_IT_TND      ((uint32_t)0x8) /*!< interrupt TnDPND */

#define IS_MFT_INTERRUPT(IT) ((((IT) & (~0xF)) == 0x00) && ((IT) != 0x00))

/**
  * @}
  */
              
/** @defgroup MFT_Clock_Selector_Definitions Clock Selector Definitions
  * @{
  */
  
#define MFT_NO_CLK                ((uint8_t)0x00) /*!< Noclock, timer1 stopped */
#define MFT_PRESCALED_CLK         ((uint8_t)0x01) /*!< System clock with configurable prescaler */
#define MFT_EXTERNAL_EVENT        ((uint8_t)0x02) /*!< External event on TnB (mode 1 and 3 only) */
#define MFT_PULSE_ACCUMULATE      ((uint8_t)0x03) /*!< Pulse accumulate (mode 1 and 3 only) */
#define MFT_LOW_SPEED_CLK         ((uint8_t)0x04) /*!< 16 MHz clock without prescaler (only when the system clock is 32 MHz). */

#define IS_MFT_CLOCK_SEL(MODE) (((MODE) == MFT_NO_CLK) || \
                                 ((MODE) == MFT_PRESCALED_CLK) || \
                                 ((MODE) == MFT_EXTERNAL_EVENT) || \
                                 ((MODE) == MFT_PULSE_ACCUMULATE) || \
                                 ((MODE) == MFT_LOW_SPEED_CLK))
                
/**
  * @}
  */

  
/** @defgroup MFT_Pin_Trigger_Definitions Pin Trigger Definitions
  * @{
  */
  
#define MFT_FALLING           ((uint32_t)0x0)    /* on rising edge */
#define MFT_RISING            ((uint32_t)0x1)    /* on falling edge */

#define IS_MFT_Tn_EDGE(REG) (((REG) == MFT_FALLING) || ((REG) == MFT_RISING) )
              
/**
  * @}
  */
  
/** @defgroup MFT_Mode1a_Trigger_Definitions Mode1a Trigger Definitions
  * @{
  */

#define MFT_TnB_TRIGGER           ((uint32_t)0x0)    /* trigger on TnB edge */
#define MFT_SOFTWARE_TRIGGER      ((uint32_t)0x1)    /* trigger software */

#define IS_MFT_PULSETRAIN_TRIGGER(MODE) (((MODE) == MFT_TnB_TRIGGER) || ((MODE) == MFT_SOFTWARE_TRIGGER))
  
  /**
  * @}
  */

/** @defgroup MFT_Pin_Definitions  Pin Definitions
  * @{
  */

#define MFT_TnA      0       /* select internal pin TnA */
#define MFT_TnB      1       /* select internal pin TnB */

#define IS_MFT_TnX(VALUE) (((VALUE) == MFT_TnA) || ((VALUE) == MFT_TnB))

/**
* @}
*/

/** @defgroup MFT_PWMX_Pin_Definitions PWMX Pins Definitions
  * @{
  */

#define MFT_PWM0_PIN     GPIO_Pin_2       /* The PWM0 pin */
#define MFT_PWM1_PIN     GPIO_Pin_3       /* The PWM1 pin */

#define IS_MFT_PWMX_PIN(VALUE) (((VALUE) == MFT_PWM0_PIN) || ((VALUE) == MFT_PWM1_PIN))

/**
* @}
*/
  
  
/** @defgroup MFT_TIMERx_Definitions TIMERx Definitions
  * @{
  */

#define MFT1_TIMERA           ((uint32_t)0x0)    /*  Timer MFT1 and input TnA */
#define MFT1_TIMERB           ((uint32_t)0x1)    /*  Timer MFT1 and input TnB */
#define MFT2_TIMERA           ((uint32_t)0x2)    /*  Timer MFT2 and input TnA */
#define MFT2_TIMERB           ((uint32_t)0x3)    /*  Timer MFT2 and input TnB */

#define IS_MFT_TIMER(VALUE) ((VALUE) <= 3)

#define IS_MFT_INPUT_IO(VALUE) ((VALUE) < 15)

/**
 * @}
 */
  
/**
 * @}
 */
  
/** @defgroup MFT_Exported_Functions Exported Functions
 * @{
 */

void MFT_DeInit(MFT_Type* MFTx);
void MFT_Init(MFT_Type* MFTx, MFT_InitType* MFT_InitStruct);
void MFT_Cmd(MFT_Type* MFTx, FunctionalState NewState);
void MFT_StructInit(MFT_InitType* MFT_InitStruct);
void MFT_TnEDGES(MFT_Type* MFTx, uint32_t MFT_TnA_edge, uint32_t MFT_TnB_edge);
void MFT_PulseTrainTriggerSelect(MFT_Type* MFTx, uint32_t MFT_Trigger);
void MFT_PulseTrainSoftwareTrigger(MFT_Type* MFTx);
uint8_t MFT_PulseTrainEventTriggerStatus(MFT_Type* MFTx);
void MFT_TnXEN(MFT_Type* MFTx, uint8_t TnX, FunctionalState NewState);
void MFT_SelectCapturePin( uint32_t MFT_TimerType, uint8_t MFT_Pin);
void MFT_SetCounter(MFT_Type* MFTx, uint16_t MFT_Cnt1, uint16_t MFT_Cnt2);
void MFT_SetCounter1(MFT_Type* MFTx, uint16_t MFT_Cnt);
uint16_t MFT_GetCounter1(MFT_Type* MFTx);
void MFT_SetCounter2(MFT_Type* MFTx, uint16_t MFT_Cnt);
uint16_t MFT_GetCounter2(MFT_Type* MFTx);
void MFT_EnableIT(MFT_Type* MFTx, uint8_t MFT_IrqSource, FunctionalState NewState);
void MFT_ClearIT(MFT_Type* MFTx, uint8_t MFT_IrqSource);
ITStatus MFT_StatusIT(MFT_Type* MFTx, uint8_t MFT_IrqSource);


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

#endif /*BLUENRG1_MFT_H */

/******************* (C) COPYRIGHT 2016 STMicroelectronics *****END OF FILE****/
