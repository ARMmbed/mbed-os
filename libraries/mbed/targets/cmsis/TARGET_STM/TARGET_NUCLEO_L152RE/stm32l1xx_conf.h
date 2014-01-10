/**
  ******************************************************************************
  * @file    Project/STM32L1xx_StdPeriph_Templates/stm32l1xx_conf.h 
  * @author  MCD Application Team
  * @version V1.1.1
  * @date    13-April-2012
  * @brief   Library configuration file.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; COPYRIGHT 2012 STMicroelectronics</center></h2>
  *
  * Licensed under MCD-ST Liberty SW License Agreement V2, (the "License");
  * You may not use this file except in compliance with the License.
  * You may obtain a copy of the License at:
  *
  *        http://www.st.com/software_license_agreement_liberty_v2
  *
  * Unless required by applicable law or agreed to in writing, software 
  * distributed under the License is distributed on an "AS IS" BASIS, 
  * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
  * See the License for the specific language governing permissions and
  * limitations under the License.
  *
  ******************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __STM32L1xx_CONF_H
#define __STM32L1xx_CONF_H

/* Includes ------------------------------------------------------------------*/
/* Uncomment/Comment the line below to enable/disable peripheral header file inclusion */
#include "stm32l1xx_adc.h"
//#include "stm32l1xx_aes.h"
//#include "stm32l1xx_comp.h"
//#include "stm32l1xx_crc.h"
#include "stm32l1xx_dac.h"
#include "stm32l1xx_dbgmcu.h"
//#include "stm32l1xx_dma.h"
#include "stm32l1xx_exti.h"
//#include "stm32l1xx_flash.h"
//#include "stm32l1xx_fsmc.h"
#include "stm32l1xx_gpio.h"
#include "stm32l1xx_i2c.h"
//#include "stm32l1xx_iwdg.h"
//#include "stm32l1xx_lcd.h"
//#include "stm32l1xx_opamp.h"
#include "stm32l1xx_pwr.h"
#include "stm32l1xx_rcc.h"
#include "stm32l1xx_rtc.h"
#include "stm32l1xx_sdio.h"
#include "stm32l1xx_spi.h"
#include "stm32l1xx_syscfg.h"
#include "stm32l1xx_tim.h"
#include "stm32l1xx_usart.h"
//#include "stm32l1xx_wwdg.h"
#include "misc.h"  /* High level functions for NVIC and SysTick (add-on to CMSIS functions) */

/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
/* Uncomment the line below to expanse the "assert_param" macro in the 
   Standard Peripheral Library drivers code */
/* #define USE_FULL_ASSERT    1 */

/* Exported macro ------------------------------------------------------------*/
#ifdef  USE_FULL_ASSERT

/**
  * @brief  The assert_param macro is used for function's parameters check.
  * @param  expr: If expr is false, it calls assert_failed function which reports 
  *         the name of the source file and the source line number of the call 
  *         that failed. If expr is true, it returns no value.
  * @retval None
  */
  #define assert_param(expr) ((expr) ? (void)0 : assert_failed((uint8_t *)__FILE__, __LINE__))
/* Exported functions ------------------------------------------------------- */
  void assert_failed(uint8_t* file, uint32_t line);
#else
  #define assert_param(expr) ((void)0)
#endif /* USE_FULL_ASSERT */

#endif /* __STM32L1xx_CONF_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
