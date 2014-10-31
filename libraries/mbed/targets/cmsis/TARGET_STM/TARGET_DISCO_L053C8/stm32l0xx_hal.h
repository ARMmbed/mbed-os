/**
  ******************************************************************************
  * @file    stm32l0xx_hal.h
  * @author  MCD Application Team
  * @version V1.1.0
  * @date    18-June-2014
  * @brief   This file contains all the functions prototypes for the HAL 
  *          module driver.
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
#ifndef __STM32L0xx_HAL_H
#define __STM32L0xx_HAL_H

#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32l0xx_hal_conf.h"

/** @addtogroup STM32L0xx_HAL_Driver
  * @{
  */

/** @addtogroup HAL
  * @{
  */ 

/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
/** @defgroup HAL_Exported_Constants
  * @{
  */ 

/** @defgroup HAL_DBGMCU_Low_Power_Config 
  * @{
  */
#define DBGMCU_SLEEP                 DBGMCU_CR_DBG_SLEEP
#define DBGMCU_STOP                  DBGMCU_CR_DBG_STOP
#define DBGMCU_STANDBY               DBGMCU_CR_DBG_STANDBY
#define IS_DBGMCU_PERIPH(PERIPH) ((((PERIPH) & 0xFFFFFFF8) == 0x00) && ((PERIPH) != 0x00))

/**
  * @}
  */
  

/** @defgroup HAL_SYSCFG_I2C_FastModePlus_Config 
  * @{
  */ 
#define SYSCFG_I2CFastModePlus_PB6       SYSCFG_CFGR2_I2C_PB6_FMP  /* Enable Fast Mode Plus on PB6 */
#define SYSCFG_I2CFastModePlus_PB7       SYSCFG_CFGR2_I2C_PB7_FMP  /* Enable Fast Mode Plus on PB7 */
#define SYSCFG_I2CFastModePlus_PB8       SYSCFG_CFGR2_I2C_PB8_FMP  /* Enable Fast Mode Plus on PB8 */
#define SYSCFG_I2CFastModePlus_PB9       SYSCFG_CFGR2_I2C_PB9_FMP  /* Enable Fast Mode Plus on PB9 */
#define SYSCFG_I2CFastModePlus_I2C1      SYSCFG_CFGR2_I2C1_FMP     /*!< Enable Fast Mode Plus on I2C1 pins */
#define SYSCFG_I2CFastModePlus_I2C2      SYSCFG_CFGR2_I2C2_FMP     /*!< Enable Fast Mode Plus on I2C2 pins */

#define IS_SYSCFG_I2C_FMP(PIN) (((PIN) == SYSCFG_I2CFastModePlus_PB6)  || \
                                ((PIN) == SYSCFG_I2CFastModePlus_PB7)  || \
                                ((PIN) == SYSCFG_I2CFastModePlus_PB8)  || \
                                ((PIN) == SYSCFG_I2CFastModePlus_PB9)  || \
                                ((PIN) == SYSCFG_I2CFastModePlus_I2C1) || \
                                ((PIN) == SYSCFG_I2CFastModePlus_I2C2))

/**
  * @}
  */  
 
/** @defgroup HAL_SYSCFG_VREFINT_OUT_SELECT 
  * @{
  */ 
#define SYSCFG_VREFINT_OUT_NONE          ((uint32_t)0x00000000) /* no pad connected */  
#define SYSCFG_VREFINT_OUT_PB0           SYSCFG_CFGR3_VREF_OUT_0 /* Selects PBO as output for the Vrefint */
#define SYSCFG_VREFINT_OUT_PB1           SYSCFG_CFGR3_VREF_OUT_1 /* Selects PB1 as output for the Vrefint */
#define SYSCFG_VREFINT_OUT_PB0_PB1       SYSCFG_CFGR3_VREF_OUT   /* Selects PBO and PB1 as output for the Vrefint */

#define IS_SYSCFG_VREFINT_OUT_SELECT(OUTPUT)   (((OUTPUT) == SYSCFG_VREFINT_OUT_PB0)  || \
                                                ((OUTPUT) == SYSCFG_VREFINT_OUT_PB1)  || \
                                                ((OUTPUT) == SYSCFG_VREFINT_OUT_PB0_PB1))

/**
  * @}
  */ 

/** @defgroup HAL_SYSCFG_flags_definition 
  * @{
  */

#define SYSCFG_FLAG_RC48               SYSCFG_CFGR3_REF_HSI48_RDYF
#define SYSCFG_FLAG_SENSOR_ADC         SYSCFG_CFGR3_SENSOR_ADC_RDYF
#define SYSCFG_FLAG_VREF_ADC           SYSCFG_VREFINT_ADC_RDYF
#define SYSCFG_FLAG_VREF_COMP          SYSCFG_CFGR3_VREFINT_COMP_RDYF
#define SYSCFG_FLAG_VREF_READY         SYSCFG_CFGR3_VREFINT_RDYF

#define IS_SYSCFG_FLAG(FLAG) (((FLAG) == SYSCFG_FLAG_RC48)        || \
                              ((FLAG) == SYSCFG_FLAG_SENSOR_ADC)  || \
                              ((FLAG) == SYSCFG_FLAG_VREF_ADC)    || \
                              ((FLAG) == SYSCFG_FLAG_VREF_COMP)   || \
                              ((FLAG) == SYSCFG_FLAG_VREF_READY))

/**
  * @}
  */
/* Exported macro ------------------------------------------------------------*/

/** @brief  Freeze/Unfreeze Peripherals in Debug mode 
  */
#define __HAL_FREEZE_TIM2_DBGMCU()           (DBGMCU->APB1FZ |= (DBGMCU_APB1_FZ_DBG_TIM2_STOP))
#define __HAL_FREEZE_TIM6_DBGMCU()           (DBGMCU->APB1FZ |= (DBGMCU_APB1_FZ_DBG_TIM6_STOP))
#define __HAL_FREEZE_RTC_DBGMCU()            (DBGMCU->APB1FZ |= (DBGMCU_APB1_FZ_DBG_RTC_STOP))
#define __HAL_FREEZE_WWDG_DBGMCU()           (DBGMCU->APB1FZ |= (DBGMCU_APB1_FZ_DBG_WWDG_STOP))
#define __HAL_FREEZE_IWDG_DBGMCU()           (DBGMCU->APB1FZ |= (DBGMCU_APB1_FZ_DBG_IWDG_STOP))
#define __HAL_FREEZE_I2C1_TIMEOUT_DBGMCU()   (DBGMCU->APB1FZ |= (DBGMCU_APB1_FZ_DBG_I2C1_STOP))
#define __HAL_FREEZE_I2C2_TIMEOUT_DBGMCU()   (DBGMCU->APB1FZ |= (DBGMCU_APB1_FZ_DBG_I2C2_STOP))
#define __HAL_FREEZE_LPTIMER_DBGMCU()        (DBGMCU->APB1FZ |= (DBGMCU_APB1_FZ_DBG_LPTIMER_STOP))
#define __HAL_FREEZE_TIM22_DBGMCU()          (DBGMCU->APB2FZ |= (DBGMCU_APB2_FZ_DBG_TIM22_STOP))
#define __HAL_FREEZE_TIM21_DBGMCU()          (DBGMCU->APB2FZ |= (DBGMCU_APB2_FZ_DBG_TIM21_STOP))

#define __HAL_UNFREEZE_TIM2_DBGMCU()           (DBGMCU->APB1FZ &= ~(DBGMCU_APB1_FZ_DBG_TIM2_STOP))
#define __HAL_UNFREEZE_TIM6_DBGMCU()           (DBGMCU->APB1FZ &= ~(DBGMCU_APB1_FZ_DBG_TIM6_STOP))
#define __HAL_UNFREEZE_RTC_DBGMCU()            (DBGMCU->APB1FZ &= ~(DBGMCU_APB1_FZ_DBG_RTC_STOP))
#define __HAL_UNFREEZE_WWDG_DBGMCU()           (DBGMCU->APB1FZ &= ~(DBGMCU_APB1_FZ_DBG_WWDG_STOP))
#define __HAL_UNFREEZE_IWDG_DBGMCU()           (DBGMCU->APB1FZ &= ~(DBGMCU_APB1_FZ_DBG_IWDG_STOP))
#define __HAL_UNFREEZE_I2C1_TIMEOUT_DBGMCU()   (DBGMCU->APB1FZ &= ~(DBGMCU_APB1_FZ_DBG_I2C1_STOP))
#define __HAL_UNFREEZE_I2C2_TIMEOUT_DBGMCU()   (DBGMCU->APB1FZ &= ~(DBGMCU_APB1_FZ_DBG_I2C2_STOP))
#define __HAL_UNFREEZE_LPTIMER_DBGMCU()        (DBGMCU->APB1FZ &= ~(DBGMCU_APB1_FZ_DBG_LPTIMER_STOP))
#define __HAL_UNFREEZE_TIM22_DBGMCU()          (DBGMCU->APB2FZ &= ~(DBGMCU_APB2_FZ_DBG_TIM22_STOP))
#define __HAL_UNFREEZE_TIM21_DBGMCU()          (DBGMCU->APB2FZ &= ~(DBGMCU_APB2_FZ_DBG_TIM21_STOP))

/** @brief  Main Flash memory mapped at 0x00000000
  */
#define __HAL_REMAPMEMORY_FLASH             (SYSCFG->CFGR1 &= ~(SYSCFG_CFGR1_MEM_MODE))

/** @brief  System Flash memory mapped at 0x00000000
  */
#define __HAL_REMAPMEMORY_SYSTEMFLASH       do {SYSCFG->CFGR1 &= ~(SYSCFG_CFGR1_MEM_MODE);\
                                                SYSCFG->CFGR1 |= SYSCFG_CFGR1_MEM_MODE_0;\
                                               }while(0);

/** @brief  Embedded SRAM mapped at 0x00000000
  */
#define __HAL_REMAPMEMORY_SRAM       do {SYSCFG->CFGR1 &= ~(SYSCFG_CFGR1_MEM_MODE);\
                                         SYSCFG->CFGR1 |= (SYSCFG_CFGR1_MEM_MODE_0 | SYSCFG_CFGR1_MEM_MODE_1);\
                                         }while(0);

/** @brief  Check whether the specified SYSCFG flag is set or not.
  * @param  __FLAG__: specifies the flag to check.
  *         This parameter can be one of the following values:
  *            SYSCFG_FLAG_PE: SRAM parity error flag.
  *            @arg SYSCFG_FLAG_RC48
  *            @arg SYSCFG_FLAG_SENSOR_ADC
  *            @arg SYSCFG_FLAG_VREF_ADC
  *            @arg SYSCFG_FLAG_VREF_COMP
  *            @arg SYSCFG_FLAG_VREF_READY   
  * @retval The new state of __FLAG__ (TRUE or FALSE).
  */
#define __HAL_SYSCFG_GET_FLAG(__FLAG__) (((SYSCFG->CFGR3) & (__FLAG__)) == (__FLAG__))

/**                  
  * @}
  */
/* Exported functions --------------------------------------------------------*/

/* Initialization and de-initialization functions  ******************************/
HAL_StatusTypeDef HAL_Init(void);
HAL_StatusTypeDef HAL_DeInit(void);
void HAL_MspInit(void);
void HAL_MspDeInit(void);
HAL_StatusTypeDef HAL_InitTick (uint32_t TickPriority);

/* Peripheral Control functions  ************************************************/
void HAL_IncTick(void);
void HAL_Delay(__IO uint32_t Delay);
uint32_t HAL_GetTick(void);
void HAL_SuspendTick(void);
void HAL_ResumeTick(void);
uint32_t HAL_GetHalVersion(void);
uint32_t HAL_GetREVID(void);
uint32_t HAL_GetDEVID(void);
void HAL_EnableDBGSleepMode(void);
void HAL_DisableDBGSleepMode(void);
void HAL_EnableDBGStopMode(void);
void HAL_DisableDBGStopMode(void);
void HAL_EnableDBGStandbyMode(void);
void HAL_DisableDBGStandbyMode(void);
void HAL_DBG_LowPowerConfig(uint32_t Periph, FunctionalState NewState);
uint32_t  HAL_GetBootMode(void);
void HAL_I2CFastModePlusConfig(uint32_t SYSCFG_I2CFastModePlus, FunctionalState NewState);
void HAL_VREFINT_Cmd(FunctionalState NewState);
void HAL_VREFINT_OutputSelect(uint32_t SYSCFG_Vrefint_OUTPUT);
void HAL_ADC_EnableBuffer_Cmd(FunctionalState NewState);
void HAL_ADC_EnableBufferSensor_Cmd(FunctionalState NewState);
void HAL_COMP_EnableBuffer_Cmd(FunctionalState NewState);
void HAL_RC48_EnableBuffer_Cmd(FunctionalState NewState);
void HAL_Lock_Cmd(FunctionalState NewState);

/**
  * @}
  */ 

/**
  * @}
  */ 
  
#ifdef __cplusplus
}
#endif

#endif /* __STM32L0xx_HAL_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
