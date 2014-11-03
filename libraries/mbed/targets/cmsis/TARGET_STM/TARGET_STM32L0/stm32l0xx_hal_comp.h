/**
  ******************************************************************************
  * @file    stm32l0xx_hal_comp.h
  * @author  MCD Application Team
  * @version V1.1.0
  * @date    18-June-2014
  * @brief   Header file of COMP HAL module.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; COPYRIGHT(c) 2014 STMicroelectronics</center></h2>
  *
  * Redistribution and use in source and binary forms, with or without modification,
  * are permitted provided that the following conditions are met:
  *   1. Redistributions of source code must retain the above copyright notice,
  *      this list of conditions and the following disclaimer.
  *   2. Redistributions in binary form must reproduce the above copyright notice,
  *      this list of conditions and the following disclaimer in the documentation
  *      and/or other materials provided with the distribution.
  *   3. Neither the name of STMicroelectronics nor the names of its contributors
  *      may be used to endorse or promote products derived from this software
  *      without specific prior written permission.
  *
  * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
  * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
  * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
  * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
  * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
  * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
  * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
  * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
  * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
  * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
  *
  ******************************************************************************  
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __STM32L0xx_HAL_COMP_H
#define __STM32L0xx_HAL_COMP_H

#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32l0xx_hal_def.h"

/** @addtogroup STM32L0xx_HAL_Driver
  * @{
  */

/** @addtogroup COMP
  * @{
  */ 

/* Exported types ------------------------------------------------------------*/ 

/** 
  * @brief  COMP Init structure definition  
  */
  
typedef struct
{

  uint32_t InvertingInput;     /*!< Selects the inverting input of the comparator.
                                    This parameter can be a value of @ref COMP_InvertingInput */

  uint32_t NonInvertingInput;  /*!< Selects the non inverting input of the comparator.
                                    This parameter can be a value of @ref COMP_NonInvertingInput */

  uint32_t LPTIMConnection;     /*!< Selects if the COMP connection to the LPTIM is established or not.
                                    This parameter can be a value of @ref COMP_LPTIMConnection */

  uint32_t OutputPol;          /*!< Selects the output polarity of the comparator.
                                    This parameter can be a value of @ref COMP_OutputPolarity */

  uint32_t Mode;               /*!< Selects the operating comsumption mode of the comparator
                                    to adjust the speed/consumption.
                                    This parameter can be a value of @ref COMP_Mode */

  uint32_t WindowMode;         /*!< Selects the window mode of the comparator 2.
                                    This parameter can be a value of @ref COMP_WindowMode */

  uint32_t TriggerMode;        /*!< Selects the trigger mode of the comparator (interrupt mode).
                                    This parameter can be a value of @ref COMP_TriggerMode */
  
}COMP_InitTypeDef;

/** 
  * @brief  HAL State structures definition  
  */ 
typedef enum
{
  HAL_COMP_STATE_RESET             = 0x00,    /*!< COMP not yet initialized or disabled             */
  HAL_COMP_STATE_READY             = 0x01,    /*!< COMP initialized and ready for use               */
  HAL_COMP_STATE_READY_LOCKED      = 0x11,    /*!< COMP initialized but the configuration is locked */
  HAL_COMP_STATE_BUSY              = 0x02,    /*!< COMP is running                                  */
  HAL_COMP_STATE_BUSY_LOCKED       = 0x12     /*!< COMP is running and the configuration is locked  */
}HAL_COMP_StateTypeDef;

/** 
  * @brief  COMP Handle Structure definition  
  */ 
typedef struct
{
  COMP_TypeDef       *Instance;       /*!< Register base address    */
  COMP_InitTypeDef   Init;            /*!< COMP required parameters */
  HAL_LockTypeDef    Lock;            /*!< Locking object           */
  __IO HAL_COMP_StateTypeDef  State;  /*!< COMP communication state */
} COMP_HandleTypeDef;

/* Exported constants --------------------------------------------------------*/
/** @defgroup COMP_Exported_Constants
  * @{
  */

/** @defgroup COMP_OutputPolarity
  * @{
  */
#define COMP_OUTPUTPOL_NONINVERTED             ((uint32_t)0x00000000)  /*!< COMP output on GPIO isn't inverted */
#define COMP_OUTPUTPOL_INVERTED                COMP_CSR_COMPxPOLARITY       /*!< COMP output on GPIO is inverted */
#define IS_COMP_OUTPUTPOL(POL)  (((POL) == COMP_OUTPUTPOL_NONINVERTED)  || \
                                 ((POL) == COMP_OUTPUTPOL_INVERTED))
/**
  * @}
  */ 


/** @defgroup COMP_InvertingInput
  * @{
  */

/* Inverting Input specific to COMP1 */
#define COMP_INVERTINGINPUT_VREFINT             ((uint32_t)0x00000000) /*!< VREFINT connected to comparator1 inverting input */
#define COMP_INVERTINGINPUT_IO1                 ((uint32_t)0x00000010) /*!< I/O1 connected to comparator inverting input (PA0) for COMP1 and (PA2) for COMP2*/
#define COMP_INVERTINGINPUT_DAC1                ((uint32_t)0x00000020) /*!< DAC1_OUT (PA4) connected to comparator inverting input */
#define COMP_INVERTINGINPUT_IO2                 ((uint32_t)0x00000030) /*!< I/O2 (PA5) connected to comparator inverting input */

/* Inverting Input specific to COMP2 */
#define COMP_INVERTINGINPUT_1_4VREFINT          ((uint32_t)0x00000040) /*!< 1/4 VREFINT connected to comparator inverting input */
#define COMP_INVERTINGINPUT_1_2VREFINT          ((uint32_t)0x00000050) /*!< 1/2 VREFINT connected to comparator inverting input */
#define COMP_INVERTINGINPUT_3_4VREFINT          ((uint32_t)0x00000060) /*!< 3/4 VREFINT connected to comparator inverting input */
#define COMP_INVERTINGINPUT_IO3                 ((uint32_t)0x00000070) /*!< I/O3 (PB3) for COMP2 connected to comparator inverting input */


#define IS_COMP_INVERTINGINPUT(INPUT) (((INPUT) == COMP_INVERTINGINPUT_VREFINT)     || \
                                        ((INPUT) == COMP_INVERTINGINPUT_IO1)         || \
                                        ((INPUT) == COMP_INVERTINGINPUT_DAC1)        || \
                                        ((INPUT) == COMP_INVERTINGINPUT_IO2)         || \
                                        ((INPUT) == COMP_INVERTINGINPUT_1_4VREFINT)  || \
                                        ((INPUT) == COMP_INVERTINGINPUT_1_2VREFINT)  || \
                                        ((INPUT) == COMP_INVERTINGINPUT_3_4VREFINT)   || \
                                        ((INPUT) == COMP_INVERTINGINPUT_IO3))

/**
  * @}
  */ 


/** @defgroup COMP_NonInvertingInput
  * @{
  */

#define COMP_NONINVERTINGINPUT_IO1                 ((uint32_t)0x00000000) /*!< I/O1 (PA3) connected to comparator non inverting input */
#define COMP_NONINVERTINGINPUT_IO2                 ((uint32_t)0x00000100) /*!< I/O2 (PB4) connected to comparator non inverting input */
#define COMP_NONINVERTINGINPUT_IO3                 ((uint32_t)0x00000200) /*!< I/O3 (PB5) connected to comparator non inverting input */
#define COMP_NONINVERTINGINPUT_IO4                 ((uint32_t)0x00000300) /*!< I/O1 (PB6) connected to comparator non inverting input */
#define COMP_NONINVERTINGINPUT_IO5                 ((uint32_t)0x00000400) /*!< I/O3 (PB7) connected to comparator non inverting input */
#define COMP_NONINVERTINGINPUT_IO6                 ((uint32_t)0x00000500) /*!< I/O3 (PB7) connected to comparator non inverting input */
#define COMP_NONINVERTINGINPUT_IO7                 ((uint32_t)0x00000600) /*!< I/O3 (PB7) connected to comparator non inverting input */
#define COMP_NONINVERTINGINPUT_IO8                 ((uint32_t)0x00000700) /*!< I/O3 (PB7) connected to comparator non inverting input */

#define IS_COMP_NONINVERTINGINPUT(INPUT) (((INPUT) == COMP_NONINVERTINGINPUT_IO1) || \
                                           ((INPUT) == COMP_NONINVERTINGINPUT_IO2) || \
                                           ((INPUT) == COMP_NONINVERTINGINPUT_IO3) || \
                                           ((INPUT) == COMP_NONINVERTINGINPUT_IO4) || \
                                           ((INPUT) == COMP_NONINVERTINGINPUT_IO5) || \
                                           ((INPUT) == COMP_NONINVERTINGINPUT_IO6) || \
                                           ((INPUT) == COMP_NONINVERTINGINPUT_IO7) || \
                                           ((INPUT) == COMP_NONINVERTINGINPUT_IO8))
/**
  * @}
  */ 


/** @defgroup COMP_Mode
  * @{
  */
/* Please refer to the electrical characteristics in the device datasheet for
   the power consumption values */
#define COMP_MODE_HIGHSPEED         COMP_CSR_COMP2SPEED     /*!< High Speed */
#define COMP_MODE_LOWSPEED          ((uint32_t)0x00000000)  /*!< Low Speed */ 

#define IS_COMP_MODE(SPEED)    (((SPEED) == COMP_MODE_HIGHSPEED) || \
                                 ((SPEED) == COMP_MODE_LOWSPEED))
/**
  * @}
  */

/** @defgroup COMP_WindowMode
  * @{
  */
#define COMP_WINDOWMODE_DISABLED               ((uint32_t)0x00000000)  /*!< Window mode disabled (Plus input of comparator 1 connected to PA1)*/
#define COMP_WINDOWMODE_ENABLED                COMP_CSR_COMP1WM    /*!< Window mode enabled: Plus input of comparator 1 shorted with Plus input of comparator 2 */
#define IS_COMP_WINDOWMODE(WINDOWMODE) (((WINDOWMODE) == COMP_WINDOWMODE_DISABLED) || \
                                        ((WINDOWMODE) == COMP_WINDOWMODE_ENABLED))

#define IS_COMP_WINDOWMODE_INSTANCE(INSTANCE) ((INSTANCE) == COMP1)
/**
  * @}
  */

/** @defgroup COMP_LPTIMConnection
  * @{
  */
#define COMP_LPTIMCONNECTION_DISABLED               ((uint32_t)0x00000000)  /*!< COMPx signal is gated */
#define COMP_LPTIMCONNECTION_ENABLED                COMP_CSR_COMP1LPTIM1IN1    /*!< COMPx signal is connected to LPTIM */
#define IS_COMP_LPTIMCONNECTION(LPTIMCONNECTION)   (((LPTIMCONNECTION) == COMP_LPTIMCONNECTION_DISABLED) || \
                                                    ((LPTIMCONNECTION) == COMP_LPTIMCONNECTION_ENABLED))

/**
  * @}
  */

/** @defgroup COMP_OutputLevel
  * @{
  */ 
/* When output polarity is not inverted, comparator output is low when
   the non-inverting input is at a lower voltage than the inverting input*/
#define COMP_OUTPUTLEVEL_LOW                   ((uint32_t)0x00000000)
/* When output polarity is not inverted, comparator output is high when
   the non-inverting input is at a higher voltage than the inverting input */
#define COMP_OUTPUTLEVEL_HIGH                  COMP_CSR_COMPxOUTVALUE
/**
  * @}
  */ 

/* CSR register Mask */ 
#define COMP_CSR_UPDATE_PARAMETERS_MASK        ((uint32_t)0xC0008779)
  
#define COMP_LOCK_DISABLE                      ((uint32_t)0x00000000)
#define COMP_LOCK_ENABLE                       COMP_CSR_COMPxLOCK

#define COMP_STATE_BIT_LOCK                    ((uint32_t)0x10)

/** @defgroup COMP_TriggerMode
  * @{
  */
#define COMP_TRIGGERMODE_IT_RISING             ((uint32_t)0x00000001)   /*!< External Interrupt Mode with Rising edge trigger detection */
#define COMP_TRIGGERMODE_IT_FALLING            ((uint32_t)0x00000002)   /*!< External Interrupt Mode with Falling edge trigger detection */
#define COMP_TRIGGERMODE_IT_RISING_FALLING     ((uint32_t)0x00000003)   /*!< External Interrupt Mode with Rising/Falling edge trigger detection */
#define IS_COMP_TRIGGERMODE(MODE)  (((MODE) == COMP_TRIGGERMODE_IT_RISING)  || \
                                    ((MODE) == COMP_TRIGGERMODE_IT_FALLING) || \
                                    ((MODE) == COMP_TRIGGERMODE_IT_RISING_FALLING))
/**
  * @}
  */ 

/** @defgroup COMP_ExtiLineEvent
  * @{
  */

#define COMP_EXTI_LINE_COMP2_EVENT             ((uint32_t)0x00400000)  /*!< External interrupt line 22 Connected to COMP2 */
#define COMP_EXTI_LINE_COMP1_EVENT             ((uint32_t)0x00200000)  /*!< External interrupt line 21 Connected to COMP1 */

/**
  * @}
  */

/**
  * @}
  */ 
  
/* Exported macro ------------------------------------------------------------*/
/** @brief Reset COMP handle state
  * @param  __HANDLE__: COMP handle.
  * @retval None
  */
#define __HAL_COMP_RESET_HANDLE_STATE(__HANDLE__) ((__HANDLE__)->State = HAL_COMP_STATE_RESET)

/**
  * @brief Enables the specified comparator
  * @param  __HANDLE__: COMP handle.
  * @retval None.
  */
#define __HAL_COMP_ENABLE(__HANDLE__)          ((__HANDLE__)->Instance->CSR |= (COMP_CSR_COMPxEN))

/**
  * @brief Disables the specified comparator
  * @param  __HANDLE__: COMP handle.
  * @retval None.
  */
#define __HAL_COMP_DISABLE(__HANDLE__)         ((__HANDLE__)->Instance->CSR &= ~(COMP_CSR_COMPxEN))

/**
  * @brief Lock the specified comparator configuration
  * @param  __HANDLE__: COMP handle.
  * @retval None.
  */
#define __HAL_COMP_LOCK(__HANDLE__)            ((__HANDLE__)->Instance->CSR |= COMP_CSR_COMPxLOCK)

/** @brief  Checks whether the specified COMP flag is set or not.
  * @param  __HANDLE__: specifies the COMP Handle.
  * @param  __FLAG__: specifies the flag to check.
  *        This parameter can be one of the following values:
  *            @arg COMP_FLAG_LOCK:  lock flag
  * @retval The new state of __FLAG__ (TRUE or FALSE).
  */
#define __HAL_COMP_GET_FLAG(__HANDLE__, __FLAG__) (((__HANDLE__)->Instance->CSR & (__FLAG__)) == (__FLAG__))   


/**
  * @brief Enable the Exti Line rising edge trigger.
  * @param  __EXTILINE__: specifies the COMP Exti sources to be enabled.
  *          This parameter can be a value of @ref COMP_ExtiLineEvent 
  * @retval None.
  */                                         
#define __HAL_COMP_EXTI_RISING_IT_ENABLE(__EXTILINE__) (EXTI->RTSR |= (__EXTILINE__))

/**
  * @brief  Disable the Exti Line rising edge trigger.
  * @param  __EXTILINE__: specifies the COMP Exti sources to be disabled.
  *         This parameter can be a value of @ref COMP_ExtiLineEvent 
  * @retval None.
  */                                         
#define __HAL_COMP_EXTI_RISING_IT_DISABLE(__EXTILINE__) (EXTI->RTSR &= ~(__EXTILINE__))

/**
  * @brief Enable the Exti Line falling edge trigger.
  * @param  __EXTILINE__: specifies the COMP Exti sources to be enabled.
  *          This parameter can be a value of @ref COMP_ExtiLineEvent 
  * @retval None.
  */                                         
#define __HAL_COMP_EXTI_FALLING_IT_ENABLE(__EXTILINE__) (EXTI->FTSR |= (__EXTILINE__))

/**
  * @brief  Disable the Exti Line falling edge trigger.
  * @param  __EXTILINE__: specifies the COMP Exti sources to be disabled.
  *         This parameter can be a value of @ref COMP_ExtiLineEvent 
  * @retval None.
  */                                         
#define __HAL_COMP_EXTI_FALLING_IT_DISABLE(__EXTILINE__) (EXTI->FTSR &= ~(__EXTILINE__))

/**
  * @brief  Get the specified EXTI line for a comparator instance
  * @param  __INSTANCE__: specifies the COMP instance.
  * @retval value of @ref COMP_ExtiLineEvent
  */
#define __HAL_COMP_GET_EXTI_LINE(__INSTANCE__) (((__INSTANCE__) == COMP1) ? COMP_EXTI_LINE_COMP1_EVENT : \
                                                COMP_EXTI_LINE_COMP2_EVENT)

/**
  * @brief Enable the COMP Exti Line.
  * @param  __EXTILINE__: specifies the COMP Exti sources to be enabled.
  *          This parameter can be a value of @ref COMP_ExtiLineEvent 
  * @retval None.
  */                                         
#define __HAL_COMP_EXTI_ENABLE_IT(__EXTILINE__)   (EXTI->IMR |= (__EXTILINE__))

/**
  * @brief Disable the COMP Exti Line.
  * @param  __EXTILINE__: specifies the COMP Exti sources to be disabled.
  *          This parameter can be a value of @ref COMP_ExtiLineEvent 
  * @retval None.
  */
#define __HAL_COMP_EXTI_DISABLE_IT(__EXTILINE__)  (EXTI->IMR &= ~(__EXTILINE__))

/**
  * @brief  Checks whether the specified EXTI line flag is set or not.
  * @param  __FLAG__: specifies the COMP Exti sources to be checked.
  *          This parameter can be a value of @ref COMP_ExtiLineEvent 
  * @retval The state of __FLAG__ (SET or RESET).
  */
#define __HAL_COMP_EXTI_GET_FLAG(__FLAG__)  (EXTI->PR & (__FLAG__))
     
/**
  * @brief Clear the COMP Exti flags.
  * @param  __FLAG__: specifies the COMP Exti sources to be cleared.
  *          This parameter can be a value of @ref COMP_ExtiLineEvent 
  * @retval None.
  */
#define __HAL_COMP_EXTI_CLEAR_FLAG(__FLAG__)   (EXTI->PR = (__FLAG__))


/* Exported functions --------------------------------------------------------*/

/* Initialization/de-initialization functions  **********************************/
HAL_StatusTypeDef HAL_COMP_Init(COMP_HandleTypeDef *hcomp);
HAL_StatusTypeDef HAL_COMP_DeInit (COMP_HandleTypeDef *hcomp);
void HAL_COMP_MspInit(COMP_HandleTypeDef *hcomp);
void HAL_COMP_MspDeInit(COMP_HandleTypeDef *hcomp);

/* I/O operation functions  *****************************************************/
HAL_StatusTypeDef HAL_COMP_Start(COMP_HandleTypeDef *hcomp);
HAL_StatusTypeDef HAL_COMP_Stop(COMP_HandleTypeDef *hcomp);
HAL_StatusTypeDef HAL_COMP_Start_IT(COMP_HandleTypeDef *hcomp);
HAL_StatusTypeDef HAL_COMP_Stop_IT(COMP_HandleTypeDef *hcomp);
void HAL_COMP_IRQHandler(COMP_HandleTypeDef *hcomp);

/* Peripheral Control functions  ************************************************/
HAL_StatusTypeDef HAL_COMP_Lock(COMP_HandleTypeDef *hcomp);
uint32_t HAL_COMP_GetOutputLevel(COMP_HandleTypeDef *hcomp);

/* Callback in Interrupt mode */
void HAL_COMP_TriggerCallback(COMP_HandleTypeDef *hcomp);

/* Peripheral State functions  **************************************************/
HAL_COMP_StateTypeDef HAL_COMP_GetState(COMP_HandleTypeDef *hcomp);

/**
  * @}
  */ 

/**
  * @}
  */ 

#ifdef __cplusplus
}
#endif

#endif /* __STM32L0xx_HAL_COMP_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
