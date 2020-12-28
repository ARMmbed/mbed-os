/**
  ******************************************************************************
  * @file    stm32l0xx_hal_dac.h
  * @author  MCD Application Team
  * @brief   Header file of DAC HAL module.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2016 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under BSD 3-Clause license,
  * the "License"; You may not use this file except in compliance with the
  * License. You may obtain a copy of the License at:
  *                        opensource.org/licenses/BSD-3-Clause
  *
  ******************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __STM32L0xx_HAL_DAC_H
#define __STM32L0xx_HAL_DAC_H

#ifdef __cplusplus
 extern "C" {
#endif
   
#if !defined (STM32L010xB) && !defined (STM32L010x8) && !defined (STM32L010x6) && !defined (STM32L010x4) && !defined (STM32L011xx) && !defined (STM32L021xx) && !defined (STM32L031xx) && !defined (STM32L041xx) && !defined (STM32L051xx) && !defined (STM32L061xx) && !defined (STM32L071xx) && !defined (STM32L081xx)
   
/* Includes ------------------------------------------------------------------*/
#include "stm32l0xx_hal_def.h"

/** @addtogroup STM32L0xx_HAL_Driver
  * @{
  */

/** @defgroup DAC DAC
  * @{
  */

/* Exported types ------------------------------------------------------------*/

/** @defgroup DAC_Exported_Types DAC Exported Types
  * @{
  */

/** 
  * @brief  HAL State structures definition  
  */ 
typedef enum
{
  HAL_DAC_STATE_RESET             = 0x00U,  /*!< DAC not yet initialized or disabled  */
  HAL_DAC_STATE_READY             = 0x01U,  /*!< DAC initialized and ready for use    */
  HAL_DAC_STATE_BUSY              = 0x02U,  /*!< DAC internal processing is ongoing   */
  HAL_DAC_STATE_TIMEOUT           = 0x03U,  /*!< DAC timeout state                    */
  HAL_DAC_STATE_ERROR             = 0x04U   /*!< DAC error state                      */
 
}HAL_DAC_StateTypeDef;
 
/** 
  * @brief  DAC handle Structure definition  
  */ 
#if (USE_HAL_DAC_REGISTER_CALLBACKS == 1)
typedef struct __DAC_HandleTypeDef
#else
typedef struct DAC_HandleTypeDef
#endif
{
  DAC_TypeDef                 *Instance;     /*!< Register base address             */

  __IO HAL_DAC_StateTypeDef   State;         /*!< DAC communication state           */

  HAL_LockTypeDef             Lock;          /*!< DAC locking object                */

  DMA_HandleTypeDef           *DMA_Handle1;  /*!< Pointer DMA handler for channel 1 */

#if defined (STM32L072xx) || defined (STM32L073xx) || defined (STM32L082xx) || defined (STM32L083xx)
  DMA_HandleTypeDef           *DMA_Handle2;  /*!< Pointer DMA handler for channel 2 */ 
#endif

  __IO uint32_t               ErrorCode;     /*!< DAC Error code                   */

#if (USE_HAL_DAC_REGISTER_CALLBACKS == 1)
  void (* ConvCpltCallbackCh1)            (struct __DAC_HandleTypeDef *hdac);
  void (* ConvHalfCpltCallbackCh1)        (struct __DAC_HandleTypeDef *hdac);
  void (* ErrorCallbackCh1)               (struct __DAC_HandleTypeDef *hdac);
  void (* DMAUnderrunCallbackCh1)         (struct __DAC_HandleTypeDef *hdac);
  void (* ConvCpltCallbackCh2)            (struct __DAC_HandleTypeDef* hdac);
  void (* ConvHalfCpltCallbackCh2)        (struct __DAC_HandleTypeDef* hdac);
  void (* ErrorCallbackCh2)               (struct __DAC_HandleTypeDef* hdac);
  void (* DMAUnderrunCallbackCh2)         (struct __DAC_HandleTypeDef* hdac); 

  void (* MspInitCallback)                (struct __DAC_HandleTypeDef *hdac);
  void (* MspDeInitCallback )             (struct __DAC_HandleTypeDef *hdac); 
#endif /* USE_HAL_DAC_REGISTER_CALLBACKS */ 

}DAC_HandleTypeDef;

/** 
  * @brief   DAC Configuration regular Channel structure definition
  */
typedef struct
{
  uint32_t DAC_Trigger;       /*!< Specifies the external trigger for the selected DAC channel.
                                   This parameter can be a value of @ref DAC_trigger_selection */

  uint32_t DAC_OutputBuffer;  /*!< Specifies whether the DAC channel output buffer is enabled or disabled.
                                   This parameter can be a value of @ref DAC_output_buffer */

}DAC_ChannelConfTypeDef;

#if (USE_HAL_DAC_REGISTER_CALLBACKS == 1)
/**
  * @brief  HAL DAC Callback ID enumeration definition
  */
typedef enum
{
  HAL_DAC_CH1_COMPLETE_CB_ID                 = 0x00U,  /*!< DAC CH1 Complete Callback ID      */
  HAL_DAC_CH1_HALF_COMPLETE_CB_ID            = 0x01U,  /*!< DAC CH1 half Complete Callback ID */
  HAL_DAC_CH1_ERROR_ID                       = 0x02U,  /*!< DAC CH1 error Callback ID         */
  HAL_DAC_CH1_UNDERRUN_CB_ID                 = 0x03U,  /*!< DAC CH1 underrun Callback ID      */
  HAL_DAC_CH2_COMPLETE_CB_ID                 = 0x04U,  /*!< DAC CH2 Complete Callback ID      */
  HAL_DAC_CH2_HALF_COMPLETE_CB_ID            = 0x05U,  /*!< DAC CH2 half Complete Callback ID */
  HAL_DAC_CH2_ERROR_ID                       = 0x06U,  /*!< DAC CH2 error Callback ID         */
  HAL_DAC_CH2_UNDERRUN_CB_ID                 = 0x07U,  /*!< DAC CH2 underrun Callback ID      */
  HAL_DAC_MSP_INIT_CB_ID                     = 0x08U,  /*!< DAC MspInit Callback ID           */
  HAL_DAC_MSP_DEINIT_CB_ID                   = 0x09U,  /*!< DAC MspDeInit Callback ID         */
  HAL_DAC_ALL_CB_ID                          = 0x0AU   /*!< DAC All ID                        */
}HAL_DAC_CallbackIDTypeDef;                            
                                                       
/**
  * @brief  HAL DAC Callback pointer definition
  */
typedef void (*pDAC_CallbackTypeDef)(DAC_HandleTypeDef *hdac);
#endif /* USE_HAL_DAC_REGISTER_CALLBACKS */

/**
  * @}
  */

/* Exported constants --------------------------------------------------------*/

/** @defgroup DAC_Exported_Constants DAC Exported Constants
  * @{
  */  

/** @defgroup DAC_Error_Code DAC Error Code
  * @{
  */
#define  HAL_DAC_ERROR_NONE              0x00U    /*!< No error                          */
#define  HAL_DAC_ERROR_DMAUNDERRUNCH1    0x01U    /*!< DAC channel1 DMA underrun error   */
#if defined (STM32L072xx) || defined (STM32L073xx) || defined (STM32L082xx) || defined (STM32L083xx)
#define  HAL_DAC_ERROR_DMAUNDERRUNCH2    0x02U    /*!< DAC channel2 DMA underrun error   */
#endif
#define  HAL_DAC_ERROR_DMA               0x04U    /*!< DMA error                         */
#if (USE_HAL_DAC_REGISTER_CALLBACKS == 1)
#define HAL_DAC_ERROR_INVALID_CALLBACK   0x10U    /*!< Invalid callback error            */
#endif /* USE_HAL_DAC_REGISTER_CALLBACKS */

/**
  * @}
  */

/** @defgroup DAC_trigger_selection DAC trigger selection
  * @{
  */
#define DAC_TRIGGER_NONE       ((uint32_t)0x00000000U) /*!< Conversion is automatic once the DAC1_DHRxxxx register has been loaded, and not by external trigger */
#define DAC_TRIGGER_T6_TRGO    ((uint32_t)                                                    DAC_CR_TEN1)  /*!< TIM6 TRGO selected as external conversion trigger for DAC channel */
#define DAC_TRIGGER_T21_TRGO   ((uint32_t)(                 DAC_CR_TSEL1_1 | DAC_CR_TSEL1_0 | DAC_CR_TEN1)) /*!< TIM21 TRGO selected as external conversion trigger for DAC channel */
#define DAC_TRIGGER_T2_TRGO    ((uint32_t)(DAC_CR_TSEL1_2 |                                   DAC_CR_TEN1)) /*!< TIM2 TRGO selected as external conversion trigger for DAC channel */
#define DAC_TRIGGER_EXT_IT9    ((uint32_t)(DAC_CR_TSEL1_2 | DAC_CR_TSEL1_1 |                  DAC_CR_TEN1)) /*!< EXTI Line9 event selected as external conversion trigger for DAC channel */
#define DAC_TRIGGER_SOFTWARE   ((uint32_t)(DAC_CR_TSEL1_2 | DAC_CR_TSEL1_1 | DAC_CR_TSEL1_0 | DAC_CR_TEN1)) /*!< Conversion started by software trigger for DAC channel */

#if defined (STM32L072xx) || defined (STM32L073xx) || defined (STM32L082xx) || defined (STM32L083xx)
#define DAC_TRIGGER_T3_TRGO    ((uint32_t)(                                  DAC_CR_TSEL1_0 | DAC_CR_TEN1)) /*!< TIM3  TRGO selected as external conversion trigger for DAC channel */
#define DAC_TRIGGER_T3_CH3     ((uint32_t)(                 DAC_CR_TSEL1_1 |                  DAC_CR_TEN1)) /*!< TIM3  CH3  selected as external conversion trigger for DAC channel */
#define DAC_TRIGGER_T7_TRGO    ((uint32_t)(DAC_CR_TSEL1_2 |                  DAC_CR_TSEL1_0 | DAC_CR_TEN1)) /*!< TIM7  TRGO selected as external conversion trigger for DAC channel */
#endif

#if defined (STM32L072xx) || defined (STM32L073xx) || defined (STM32L082xx) || defined (STM32L083xx)
#define IS_DAC_TRIGGER(TRIGGER) (((TRIGGER) == DAC_TRIGGER_NONE)       || \
                                 ((TRIGGER) == DAC_TRIGGER_T6_TRGO)    || \
                                 ((TRIGGER) == DAC_TRIGGER_T3_TRGO)    || \
                                 ((TRIGGER) == DAC_TRIGGER_T3_CH3)     || \
                                 ((TRIGGER) == DAC_TRIGGER_T7_TRGO)    || \
                                 ((TRIGGER) == DAC_TRIGGER_T21_TRGO)   || \
                                 ((TRIGGER) == DAC_TRIGGER_T2_TRGO)    || \
                                 ((TRIGGER) == DAC_TRIGGER_EXT_IT9)    || \
                                 ((TRIGGER) == DAC_TRIGGER_SOFTWARE))
#else /* STM32L072xx || STM32L073xx || STM32L082xx || STM32L083xx */
#define IS_DAC_TRIGGER(TRIGGER) (((TRIGGER) == DAC_TRIGGER_NONE)       || \
                                 ((TRIGGER) == DAC_TRIGGER_T6_TRGO)    || \
                                 ((TRIGGER) == DAC_TRIGGER_T21_TRGO)   || \
                                 ((TRIGGER) == DAC_TRIGGER_T2_TRGO)    || \
                                 ((TRIGGER) == DAC_TRIGGER_EXT_IT9)    || \
                                 ((TRIGGER) == DAC_TRIGGER_SOFTWARE))
#endif /* STM32L072xx || STM32L073xx || STM32L082xx || STM32L083xx */
/**
  * @}
  */

/** @defgroup DAC_output_buffer DAC output buffer
  * @{
  */
#define DAC_OUTPUTBUFFER_ENABLE            ((uint32_t)0x00000000U)
#define DAC_OUTPUTBUFFER_DISABLE           ((uint32_t)DAC_CR_BOFF1)

#define IS_DAC_OUTPUT_BUFFER_STATE(STATE) (((STATE) == DAC_OUTPUTBUFFER_ENABLE) || \
                                           ((STATE) == DAC_OUTPUTBUFFER_DISABLE))
/**
  * @}
  */

/** @defgroup DAC_Channel_selection DAC Channel selection
  * @{
  */
#define DAC_CHANNEL_1                      ((uint32_t)0x00000000U)
#if defined (STM32L072xx) || defined (STM32L073xx) || defined (STM32L082xx) || defined (STM32L083xx)
#define DAC_CHANNEL_2                      ((uint32_t)0x00000010U)
#endif

#if defined (STM32L072xx) || defined (STM32L073xx) || defined (STM32L082xx) || defined (STM32L083xx)
#define IS_DAC_CHANNEL(CHANNEL) (((CHANNEL) == DAC_CHANNEL_1) || \
                                 ((CHANNEL) == DAC_CHANNEL_2))
#else
#define IS_DAC_CHANNEL(CHANNEL) ((CHANNEL) == DAC_CHANNEL_1)
#endif
/**
  * @}
  */

/** @defgroup DAC_data_alignement DAC data alignement
  * @{
  */
#define DAC_ALIGN_12B_R                    ((uint32_t)0x00000000U)
#define DAC_ALIGN_12B_L                    ((uint32_t)0x00000004U)
#define DAC_ALIGN_8B_R                     ((uint32_t)0x00000008U)

#define IS_DAC_ALIGN(ALIGN) (((ALIGN) == DAC_ALIGN_12B_R) || \
                             ((ALIGN) == DAC_ALIGN_12B_L) || \
                             ((ALIGN) == DAC_ALIGN_8B_R))
/**
  * @}
  */

/** @defgroup DAC_data DAC data
  * @{
  */
#define IS_DAC_DATA(DATA) ((DATA) <= 0xFFF0U)
/**
  * @}
  */

/** @defgroup DAC_flags_definition DAC flags definition
  * @{
  */ 
#define DAC_FLAG_DMAUDR1                   ((uint32_t)DAC_SR_DMAUDR1)
#if defined (STM32L072xx) || defined (STM32L073xx) || defined (STM32L082xx) || defined (STM32L083xx)
#define DAC_FLAG_DMAUDR2                   ((uint32_t)DAC_SR_DMAUDR2)   
#endif

/**
  * @}
  */

/** @defgroup DAC_IT_definition DAC IT definition
  * @{
  */ 
#define DAC_IT_DMAUDR1                   ((uint32_t)DAC_SR_DMAUDR1)
#if defined (STM32L072xx) || defined (STM32L073xx) || defined (STM32L082xx) || defined (STM32L083xx)
#define DAC_IT_DMAUDR2                   ((uint32_t)DAC_SR_DMAUDR2)   
#endif

/**
  * @}
  */

/**
  * @}
  */

/* Exported macro ------------------------------------------------------------*/

/** @defgroup DAC_Exported_Macros DAC Exported Macros
  * @{
  */

/** @brief Reset DAC handle state
  * @param  __HANDLE__ specifies the DAC handle.
  * @retval None
  */
#if (USE_HAL_DAC_REGISTER_CALLBACKS == 1)
#define __HAL_DAC_RESET_HANDLE_STATE(__HANDLE__)           do {                                              \
                                                                 (__HANDLE__)->State = HAL_DAC_STATE_RESET; \
                                                                 (__HANDLE__)->MspInitCallback = NULL;       \
                                                                 (__HANDLE__)->MspDeInitCallback = NULL;     \
                                                               } while(0)
#else
#define __HAL_DAC_RESET_HANDLE_STATE(__HANDLE__) ((__HANDLE__)->State = HAL_DAC_STATE_RESET)
#endif /* USE_HAL_DAC_REGISTER_CALLBACKS */

/** @brief Enable the DAC channel
  * @param  __HANDLE__ specifies the DAC handle.
  * @param  __DAC_CHANNEL__ specifies the DAC channel
  * @retval None
  */
#define __HAL_DAC_ENABLE(__HANDLE__, __DAC_CHANNEL__) \
SET_BIT((__HANDLE__)->Instance->CR, (DAC_CR_EN1 << (__DAC_CHANNEL__)))

/** @brief Disable the DAC channel
  * @param  __HANDLE__ specifies the DAC handle
  * @param  __DAC_CHANNEL__ specifies the DAC channel.
  * @retval None
  */
#define __HAL_DAC_DISABLE(__HANDLE__, __DAC_CHANNEL__) \
CLEAR_BIT((__HANDLE__)->Instance->CR, (DAC_CR_EN1 << (__DAC_CHANNEL__)))

  
#define __HAL_DAC_ENABLE_IT(__HANDLE__, __INTERRUPT__) \
SET_BIT((__HANDLE__)->Instance->CR, (__INTERRUPT__))


/** @brief Disable the DAC interrupt
  * @param  __HANDLE__ specifies the DAC handle
  * @param  __INTERRUPT__ specifies the DAC interrupt.
  * @retval None
  */
#define __HAL_DAC_DISABLE_IT(__HANDLE__, __INTERRUPT__) \
CLEAR_BIT((__HANDLE__)->Instance->CR, (__INTERRUPT__))

/** @brief  Check whether the specified DAC interrupt source is enabled or not.
  * @param __HANDLE__ DAC handle
  * @param __INTERRUPT__ DAC interrupt source to check
  *          This parameter can be any combination of the following values:
  *            @arg DAC_IT_DMAUDR1: DAC channel 1 DMA underrun interrupt
  *            @arg DAC_IT_DMAUDR2: DAC channel 2 DMA underrun interrupt (STM32L072xx STM32L073xx STM32L082xx STM32L083xx only)
  * @retval State of interruption (SET or RESET)
  */
#define __HAL_DAC_GET_IT_SOURCE(__HANDLE__, __INTERRUPT__) \
(((__HANDLE__)->Instance->CR & (__INTERRUPT__)) == (__INTERRUPT__))

/** @brief  Get the selected DAC's flag status.
  * @param  __HANDLE__ specifies the DAC handle.
  * @param  __FLAG__ specifies the FLAG.
  * @retval None
  */
#define __HAL_DAC_GET_FLAG(__HANDLE__, __FLAG__)  \
((((__HANDLE__)->Instance->SR) & (__FLAG__)) == (__FLAG__))

/** @brief  Clear the DAC's flag.
  * @param  __HANDLE__ specifies the DAC handle.
  * @param  __FLAG__ specifies the FLAG.
  * @retval None
  */
#define __HAL_DAC_CLEAR_FLAG(__HANDLE__, __FLAG__) \
(((__HANDLE__)->Instance->SR) = (__FLAG__))

/**
  * @}
  */ 

/* Private macro ------------------------------------------------------------*/

/** @defgroup DAC_Private_Macros DAC Private Macros
  * @{
  */

/** @brief Set DHR12R1 alignment
  * @param  __ALIGNMENT__ specifies the DAC alignement
  * @retval None
  */
#define DAC_DHR12R1_ALIGNMENT(__ALIGNMENT__) (((uint32_t)0x00000008U) + (__ALIGNMENT__))

/** @brief  Set DHR12R2 alignment
  * @param  __ALIGNMENT__ specifies the DAC alignement
  * @retval None
  */
#define DAC_DHR12R2_ALIGNMENT(__ALIGNMENT__) (((uint32_t)0x00000014U) + (__ALIGNMENT__))

/** @brief  Set DHR12RD alignment
  * @param  __ALIGNMENT__ specifies the DAC alignement
  * @retval None
  */
#define DAC_DHR12RD_ALIGNMENT(__ALIGNMENT__) (((uint32_t)0x00000020U) + (__ALIGNMENT__))

/** @brief Enable the DAC interrupt
  * @param  __HANDLE__ specifies the DAC handle
  * @param  __INTERRUPT__ specifies the DAC interrupt.
  * @retval None
  */
  
/**
  * @}
  */ 


/* Include DAC HAL Extension module */
#include "stm32l0xx_hal_dac_ex.h"

/* Exported functions --------------------------------------------------------*/

/** @defgroup DAC_Exported_Functions DAC Exported Functions
  * @{
  */

/** @defgroup DAC_Exported_Functions_Group1 Initialization and de-initialization functions
  * @{
  */ 
/* Initialization and de-initialization functions *****************************/
HAL_StatusTypeDef HAL_DAC_Init(DAC_HandleTypeDef* hdac);
HAL_StatusTypeDef HAL_DAC_DeInit(DAC_HandleTypeDef* hdac);
void HAL_DAC_MspInit(DAC_HandleTypeDef* hdac);
void HAL_DAC_MspDeInit(DAC_HandleTypeDef* hdac);

/**
  * @}
  */

/** @defgroup DAC_Exported_Functions_Group2 IO operation functions
  * @{
  */
/* IO operation functions *****************************************************/
HAL_StatusTypeDef HAL_DAC_Start(DAC_HandleTypeDef* hdac, uint32_t Channel);
HAL_StatusTypeDef HAL_DAC_Stop(DAC_HandleTypeDef* hdac, uint32_t Channel);
HAL_StatusTypeDef HAL_DAC_Start_DMA(DAC_HandleTypeDef* hdac, uint32_t Channel, uint32_t* pData, uint32_t Length, uint32_t Alignment);
HAL_StatusTypeDef HAL_DAC_Stop_DMA(DAC_HandleTypeDef* hdac, uint32_t Channel);
HAL_StatusTypeDef HAL_DAC_SetValue(DAC_HandleTypeDef* hdac, uint32_t Channel, uint32_t Alignment, uint32_t Data);
uint32_t HAL_DAC_GetValue(DAC_HandleTypeDef* hdac, uint32_t Channel);
void HAL_DAC_IRQHandler(DAC_HandleTypeDef* hdac);
void HAL_DAC_ConvCpltCallbackCh1(DAC_HandleTypeDef* hdac);
void HAL_DAC_ConvHalfCpltCallbackCh1(DAC_HandleTypeDef* hdac);
void HAL_DAC_ErrorCallbackCh1(DAC_HandleTypeDef *hdac);
void HAL_DAC_DMAUnderrunCallbackCh1(DAC_HandleTypeDef *hdac);

#if (USE_HAL_DAC_REGISTER_CALLBACKS == 1)
/* DAC callback registering/unregistering */
HAL_StatusTypeDef     HAL_DAC_RegisterCallback (DAC_HandleTypeDef *hdac, HAL_DAC_CallbackIDTypeDef CallbackId, pDAC_CallbackTypeDef pCallback);
HAL_StatusTypeDef     HAL_DAC_UnRegisterCallback (DAC_HandleTypeDef *hdac, HAL_DAC_CallbackIDTypeDef CallbackId);
#endif /* USE_HAL_DAC_REGISTER_CALLBACKS */

/**
  * @}
  */

/** @defgroup DAC_Exported_Functions_Group3 Peripheral Control functions
  * @{
  */
/* Peripheral Control functions ***********************************************/
HAL_StatusTypeDef HAL_DAC_ConfigChannel(DAC_HandleTypeDef* hdac, DAC_ChannelConfTypeDef* sConfig, uint32_t Channel);

/**
  * @}
  */

/** @defgroup DAC_Exported_Functions_Group4 Peripheral State and Errors functions
  * @{
  */
/* Peripheral State functions ***************************************************/
HAL_DAC_StateTypeDef HAL_DAC_GetState(DAC_HandleTypeDef* hdac);
uint32_t HAL_DAC_GetError(DAC_HandleTypeDef *hdac);

/**
  * @}
  */

/**
  * @}
  */

/**
  * @}
  */

/**
  * @}
  */

#endif /* !STM32L010xB && !STM32L010x8 && !STM32L010x6 && !STM32L010x4 && !STM32L011xx && !STM32L021xx && !STM32L031xx && !STM32L041xx && !STM32L061xx && !STM32L071xx && !STM32L081xx*/
  
#ifdef __cplusplus
}
#endif

#endif /*__STM32L0xx_HAL_DAC_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
