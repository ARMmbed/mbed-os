/**
  ******************************************************************************
  * @file    stm32l0xx_hal_tim_ex.h
  * @author  MCD Application Team
  * @version V1.1.0
  * @date    18-June-2014
  * @brief   Header file of TIM HAL module.
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
#ifndef __STM32L0xx_HAL_TIM_EX_H
#define __STM32L0xx_HAL_TIM_EX_H

#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32l0xx_hal_def.h"

/** @addtogroup STM32L0xx_HAL
  * @{
  */

/** @addtogroup TIMEx
  * @{
  */ 

/* Exported types ------------------------------------------------------------*/

/** 
  * @brief  TIM Master configuration Structure definition  
  */ 
typedef struct {
  uint32_t  MasterOutputTrigger;   /*!< Trigger output (TRGO) selection 
                                      This parameter can be a value of @ref TIMEx_Master_Mode_Selection */ 
  uint32_t  MasterSlaveMode;       /*!< Master/slave mode selection 
                                      This parameter can be a value of @ref TIM_Master_Slave_Mode */
}TIM_MasterConfigTypeDef;


/* Exported constants --------------------------------------------------------*/
/** @defgroup TIMEx_Exported_Constants
  * @{
  */

/** @defgroup TIMEx_Master_Mode_Selection
  * @{
  */  
#define	TIM_TRGO_RESET            ((uint32_t)0x0000)             
#define	TIM_TRGO_ENABLE           (TIM_CR2_MMS_0)           
#define	TIM_TRGO_UPDATE           (TIM_CR2_MMS_1)             
#define	TIM_TRGO_OC1              ((TIM_CR2_MMS_1 | TIM_CR2_MMS_0))    
#define	TIM_TRGO_OC1REF           (TIM_CR2_MMS_2)           
#define	TIM_TRGO_OC2REF           ((TIM_CR2_MMS_2 | TIM_CR2_MMS_0))          
#define	TIM_TRGO_OC3REF           ((TIM_CR2_MMS_2 | TIM_CR2_MMS_1))           
#define	TIM_TRGO_OC4REF           ((TIM_CR2_MMS_2 | TIM_CR2_MMS_1 | TIM_CR2_MMS_0))   

#define IS_TIM_TRGO_SOURCE(SOURCE) (((SOURCE) == TIM_TRGO_RESET) || \
                                    ((SOURCE) == TIM_TRGO_ENABLE) || \
                                    ((SOURCE) == TIM_TRGO_UPDATE) || \
                                    ((SOURCE) == TIM_TRGO_OC1) || \
                                    ((SOURCE) == TIM_TRGO_OC1REF) || \
                                    ((SOURCE) == TIM_TRGO_OC2REF) || \
                                    ((SOURCE) == TIM_TRGO_OC3REF) || \
                                    ((SOURCE) == TIM_TRGO_OC4REF))      
   
/**
  * @}
  */ 

/** @defgroup TIMEx_Remap
  * @{
  */

#define TIM2_ETR_GPIO                     ((uint32_t)0xFFF80000)
#define TIM2_ETR_HSI48                    ((uint32_t)0xFFF80004)
#define TIM2_ETR_LSE                      ((uint32_t)0xFFF80005)
#define TIM2_ETR_COMP2_OUT                ((uint32_t)0xFFF80006)
#define TIM2_ETR_COMP1_OUT                ((uint32_t)0xFFF80007)
#define TIM2_TI4_GPIO1                    ((uint32_t)0xFFE70000)
#define TIM2_TI4_COMP2                    ((uint32_t)0xFFE70008)
#define TIM2_TI4_COMP1                    ((uint32_t)0xFFE70010)
#define TIM2_TI4_GPIO2                    ((uint32_t)0xFFE70018)
#define TIM21_ETR_GPIO                    ((uint32_t)0xFFF40000)
#define TIM21_ETR_COMP2_OUT               ((uint32_t)0xFFF40001)
#define TIM21_ETR_COMP1_OUT               ((uint32_t)0xFFF40002)
#define TIM21_ETR_LSE                     ((uint32_t)0xFFF40003)
#define TIM21_TI1_MCO                     ((uint32_t)0xFFE3001C)
#define TIM21_TI1_RTC_WKUT_IT             ((uint32_t)0xFFE30004)
#define TIM21_TI1_HSE_RTC                 ((uint32_t)0xFFE30008)
#define TIM21_TI1_MSI                     ((uint32_t)0xFFE3000C)
#define TIM21_TI1_LSE                     ((uint32_t)0xFFE30010)
#define TIM21_TI1_LSI                     ((uint32_t)0xFFE30014)
#define TIM21_TI1_COMP1_OUT               ((uint32_t)0xFFE30018)
#define TIM21_TI1_GPIO                    ((uint32_t)0xFFE30000)
#define TIM21_TI2_GPIO                    ((uint32_t)0xFFDF0000)
#define TIM21_TI2_COMP2_OUT               ((uint32_t)0xFFDF0020)
#define TIM22_ETR_LSE                     ((uint32_t)0xFFFC0000)
#define TIM22_ETR_COMP2_OUT               ((uint32_t)0xFFFC0001)
#define TIM22_ETR_COMP1_OUT               ((uint32_t)0xFFFC0002)
#define TIM22_ETR_GPIO                    ((uint32_t)0xFFFC0003)
#define TIM22_TI1_GPIO1                   ((uint32_t)0xFFF70000)
#define TIM22_TI1_COMP2_OUT               ((uint32_t)0xFFF70004)
#define TIM22_TI1_COMP1_OUT               ((uint32_t)0xFFF70008)
#define TIM22_TI1_GPIO2                   ((uint32_t)0xFFF7000C)


#define IS_TIM_REMAP(TIM_REMAP)  (((TIM_REMAP) == TIM2_ETR_GPIO          )|| \
                                  ((TIM_REMAP) == TIM2_ETR_HSI48         )|| \
                                  ((TIM_REMAP) == TIM2_ETR_LSE           )|| \
                                  ((TIM_REMAP) == TIM2_ETR_COMP2_OUT     )|| \
                                  ((TIM_REMAP) == TIM2_ETR_COMP1_OUT     )|| \
                                  ((TIM_REMAP) == TIM2_TI4_GPIO1         )|| \
                                  ((TIM_REMAP) == TIM2_TI4_COMP1         )|| \
                                  ((TIM_REMAP) == TIM2_TI4_COMP2         )|| \
                                  ((TIM_REMAP) == TIM2_TI4_GPIO2         )|| \
                                  ((TIM_REMAP) == TIM21_ETR_GPIO         )|| \
                                  ((TIM_REMAP) == TIM21_ETR_COMP2_OUT    )|| \
                                  ((TIM_REMAP) == TIM21_ETR_COMP1_OUT    )|| \
                                  ((TIM_REMAP) == TIM21_ETR_LSE          )|| \
                                  ((TIM_REMAP) == TIM21_TI1_MCO          )|| \
                                  ((TIM_REMAP) == TIM21_TI1_RTC_WKUT_IT  )|| \
                                  ((TIM_REMAP) == TIM21_TI1_HSE_RTC      )|| \
                                  ((TIM_REMAP) == TIM21_TI1_MSI          )|| \
                                  ((TIM_REMAP) == TIM21_TI1_LSE          )|| \
                                  ((TIM_REMAP) == TIM21_TI1_LSI          )|| \
                                  ((TIM_REMAP) == TIM21_TI1_COMP1_OUT    )|| \
                                  ((TIM_REMAP) == TIM21_TI1_GPIO         )|| \
                                  ((TIM_REMAP) == TIM21_TI2_GPIO         )|| \
                                  ((TIM_REMAP) == TIM21_TI2_COMP2_OUT    )|| \
                                  ((TIM_REMAP) == TIM22_ETR_LSE         )|| \
                                  ((TIM_REMAP) == TIM22_ETR_COMP2_OUT   )|| \
                                  ((TIM_REMAP) == TIM22_ETR_COMP1_OUT   )|| \
                                  ((TIM_REMAP) == TIM22_ETR_GPIO        )|| \
                                  ((TIM_REMAP) == TIM22_TI1_GPIO1       )|| \
                                  ((TIM_REMAP) == TIM22_TI1_COMP2_OUT   )|| \
                                  ((TIM_REMAP) == TIM22_TI1_COMP1_OUT   )|| \
                                  ((TIM_REMAP) == TIM22_TI1_GPIO2       ))


/**
  * @}
  */ 

/**
  * @}
  */   
  
/* Exported macro ------------------------------------------------------------*/
/* Exported functions --------------------------------------------------------*/
/* Control functions  ***********************************************************/
HAL_StatusTypeDef HAL_TIMEx_RemapConfig(TIM_HandleTypeDef *htim, uint32_t Remap);
HAL_StatusTypeDef HAL_TIMEx_MasterConfigSynchronization(TIM_HandleTypeDef *htim, TIM_MasterConfigTypeDef * sMasterConfig);
/**
  * @}
  */ 

/**
  * @}
  */ 
  
#ifdef __cplusplus
}
#endif

#endif /* __STM32L0xx_HAL_TIM_EX_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
