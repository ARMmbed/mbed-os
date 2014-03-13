/**
  ******************************************************************************
  * @file    stm32l1xx_comp.h
  * @author  MCD Application Team
  * @version V1.3.0
  * @date    31-January-2014
  * @brief   This file contains all the functions prototypes for the COMP firmware 
  *          library.
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
#ifndef __STM32L1xx_COMP_H
#define __STM32L1xx_COMP_H

#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32l1xx.h"

/** @addtogroup STM32L1xx_StdPeriph_Driver
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
  uint32_t COMP_Speed;               /*!< Defines the speed of comparator 2.
                                          This parameter can be a value of @ref COMP_Speed */
  uint32_t COMP_InvertingInput;      /*!< Selects the inverting input of the comparator 2.
                                          This parameter can be a value of @ref COMP_InvertingInput */
  uint32_t COMP_OutputSelect;        /*!< Selects the output redirection of the comparator 2.
                                          This parameter can be a value of @ref COMP_OutputSelect */
   
}COMP_InitTypeDef;

/* Exported constants --------------------------------------------------------*/
   
/** @defgroup COMP_Exported_Constants
  * @{
  */ 

#define COMP_OutputLevel_High                   ((uint32_t)0x00000001)
#define COMP_OutputLevel_Low                    ((uint32_t)0x00000000)

/** @defgroup COMP_Selection
  * @{
  */

#define COMP_Selection_COMP1                    ((uint32_t)0x00000001)
#define COMP_Selection_COMP2                    ((uint32_t)0x00000002)

#define IS_COMP_ALL_PERIPH(PERIPH) (((PERIPH) == COMP_Selection_COMP1) || \
                                    ((PERIPH) == COMP_Selection_COMP2))
 
/**
  * @}
  */ 

/** @defgroup COMP_InvertingInput
  * @{
  */

#define COMP_InvertingInput_None                ((uint32_t)0x00000000) /* COMP2 is disabled when this parameter is selected */
#define COMP_InvertingInput_IO                  ((uint32_t)0x00040000)
#define COMP_InvertingInput_VREFINT             ((uint32_t)0x00080000)
#define COMP_InvertingInput_3_4VREFINT          ((uint32_t)0x000C0000)
#define COMP_InvertingInput_1_2VREFINT          ((uint32_t)0x00100000)
#define COMP_InvertingInput_1_4VREFINT          ((uint32_t)0x00140000)
#define COMP_InvertingInput_DAC1                ((uint32_t)0x00180000)
#define COMP_InvertingInput_DAC2                ((uint32_t)0x001C0000)

#define IS_COMP_INVERTING_INPUT(INPUT) (((INPUT) == COMP_InvertingInput_None) || \
                                        ((INPUT) == COMP_InvertingInput_IO) || \
                                        ((INPUT) == COMP_InvertingInput_VREFINT) || \
                                        ((INPUT) == COMP_InvertingInput_3_4VREFINT) || \
                                        ((INPUT) == COMP_InvertingInput_1_2VREFINT) || \
                                        ((INPUT) == COMP_InvertingInput_1_4VREFINT) || \
                                        ((INPUT) == COMP_InvertingInput_DAC1) || \
                                        ((INPUT) == COMP_InvertingInput_DAC2))
/**
  * @}
  */ 

/** @defgroup COMP_OutputSelect
  * @{
  */

#define COMP_OutputSelect_TIM2IC4               ((uint32_t)0x00000000)
#define COMP_OutputSelect_TIM2OCREFCLR          ((uint32_t)0x00200000)
#define COMP_OutputSelect_TIM3IC4               ((uint32_t)0x00400000)
#define COMP_OutputSelect_TIM3OCREFCLR          ((uint32_t)0x00600000)
#define COMP_OutputSelect_TIM4IC4               ((uint32_t)0x00800000)
#define COMP_OutputSelect_TIM4OCREFCLR          ((uint32_t)0x00A00000)
#define COMP_OutputSelect_TIM10IC1              ((uint32_t)0x00C00000)
#define COMP_OutputSelect_None                  ((uint32_t)0x00E00000)

#define IS_COMP_OUTPUT(OUTPUT) (((OUTPUT) == COMP_OutputSelect_TIM2IC4) || \
                                ((OUTPUT) == COMP_OutputSelect_TIM2OCREFCLR) || \
                                ((OUTPUT) == COMP_OutputSelect_TIM3IC4) || \
                                ((OUTPUT) == COMP_OutputSelect_TIM3OCREFCLR) || \
                                ((OUTPUT) == COMP_OutputSelect_TIM4IC4) || \
                                ((OUTPUT) == COMP_OutputSelect_TIM4OCREFCLR) || \
                                ((OUTPUT) == COMP_OutputSelect_TIM10IC1) || \
                                ((OUTPUT) == COMP_OutputSelect_None))
/**
  * @}
  */ 
  
/** @defgroup COMP_Speed
  * @{
  */

#define COMP_Speed_Slow                         ((uint32_t)0x00000000)
#define COMP_Speed_Fast                         ((uint32_t)0x00001000)

#define IS_COMP_SPEED(SPEED)    (((SPEED) == COMP_Speed_Slow) || \
                                 ((SPEED) == COMP_Speed_Fast))
/**
  * @}
  */
  
/**
  * @}
  */ 

/* Exported macro ------------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */

/*  Function used to set the COMP configuration to the default reset state ****/
void COMP_DeInit(void);

/* Initialization and Configuration functions *********************************/
void COMP_Init(COMP_InitTypeDef* COMP_InitStruct);
void COMP_Cmd(FunctionalState NewState);
uint8_t COMP_GetOutputLevel(uint32_t COMP_Selection);
void COMP_SW1SwitchConfig(FunctionalState NewState);

/* Window mode control function ***********************************************/
void COMP_WindowCmd(FunctionalState NewState);

/* Internal Reference Voltage (VREFINT) output function ***********************/
void COMP_VrefintOutputCmd(FunctionalState NewState);

#ifdef __cplusplus
}
#endif

#endif /*__STM32L1xx_COMP_H */

/**
  * @}
  */ 

/**
  * @}
  */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
