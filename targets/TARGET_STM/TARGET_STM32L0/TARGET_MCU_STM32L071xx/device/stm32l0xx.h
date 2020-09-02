/* mbed Microcontroller Library
 * SPDX-License-Identifier: BSD-3-Clause
 ******************************************************************************
 *
 * Copyright (c) 2020 STMicroelectronics. 
 * All rights reserved.
 *
 * This software component is licensed by ST under BSD 3-Clause license,
 * the "License"; You may not use this file except in compliance with the 
 * License. You may obtain a copy of the License at:
 *                        opensource.org/licenses/BSD-3-Clause
 *
 ******************************************************************************
 */

/** @addtogroup CMSIS
  * @{
  */

/** @addtogroup stm32l0xx
  * @{
  */
    
#ifndef __STM32L0xx_H
#define __STM32L0xx_H

#ifdef __cplusplus
 extern "C" {
#endif /* __cplusplus */
   
/**
  * @brief STM32 Family
  */
#if !defined (STM32L0)
#define STM32L0
#endif /* STM32L0 */

/* Uncomment the line below according to the target STM32 device used in your
   application 
  */

#if !defined (STM32L011xx) && !defined (STM32L021xx) && \
    !defined (STM32L031xx) && !defined (STM32L041xx) && \
    !defined (STM32L051xx) && !defined (STM32L052xx) && !defined (STM32L053xx) && \
    !defined (STM32L061xx) && !defined (STM32L062xx) && !defined (STM32L063xx) && \
    !defined (STM32L071xx) && !defined (STM32L072xx) && !defined (STM32L073xx) && \
    !defined (STM32L081xx) && !defined (STM32L082xx) && !defined (STM32L083xx) \
  /* #define STM32L011xx */
  /* #define STM32L021xx */
  /* #define STM32L031xx */   /*!< STM32L031C6, STM32L031E6, STM32L031F6, STM32L031G6, STM32L031K6 Devices */
  /* #define STM32L041xx */   /*!< STM32L041C6, STM32L041E6, STM32L041F6, STM32L041G6, STM32L041K6 Devices */
  /* #define STM32L051xx */   /*!< STM32L051K8, STM32L051C6, STM32L051C8, STM32L051R6, STM32L051R8 Devices */
  /* #define STM32L052xx */   /*!< STM32L052K6, STM32L052K8, STM32L052C6, STM32L052C8, STM32L052R6, STM32L052R8 Devices */
  /* #define STM32L053xx */   /*!< STM32L053C6, STM32L053C8, STM32L053R6, STM32L053R8 Devices */
  /* #define STM32L061xx */   /*!< */
  /* #define STM32L062xx */   /*!< STM32L062K8 */
  /* #define STM32L063xx */   /*!< STM32L063C8, STM32L063R8 */ 
  #define STM32L071xx         /*!< */
  /* #define STM32L072xx */   /*!< */
  //#define STM32L073xx    /*!< STM32L073V8, STM32L073VB, STM32L073RB, STM32L073VZ, STM32L073RZ Devices */
  /* #define STM32L081xx */   /*!< */
  /* #define STM32L082xx */   /*!< */
  /* #define STM32L083xx */   /*!< */ 
#endif
   
/*  Tip: To avoid modifying this file each time you need to switch between these
        devices, you can define the device in your toolchain compiler preprocessor.
  */
#if !defined  (USE_HAL_DRIVER)
/**
 * @brief Comment the line below if you will not use the peripherals drivers.
   In this case, these drivers will not be included and the application code will 
   be based on direct access to peripherals registers 
   */
#define USE_HAL_DRIVER
#endif /* USE_HAL_DRIVER */

/**
  * @brief CMSIS Device version number V1.7.1
  */
#define __STM32L0xx_CMSIS_VERSION_MAIN   (0x01) /*!< [31:24] main version */
#define __STM32L0xx_CMSIS_VERSION_SUB1   (0x07) /*!< [23:16] sub1 version */
#define __STM32L0xx_CMSIS_VERSION_SUB2   (0x02) /*!< [15:8]  sub2 version */
#define __STM32L0xx_CMSIS_VERSION_RC     (0x00) /*!< [7:0]  release candidate */
#define __STM32L0xx_CMSIS_VERSION        ((__STM32L0xx_CMSIS_VERSION_MAIN     << 24)\
                                         |(__STM32L0xx_CMSIS_VERSION_SUB1 << 16)\
                                         |(__STM32L0xx_CMSIS_VERSION_SUB2 << 8 )\
                                         |(__STM32L0xx_CMSIS_VERSION_RC))
                                             
/** @addtogroup Device_Included
  * @{
  */
#if defined(STM32L011xx)
  #include "stm32l011xx.h"
#elif defined(STM32L021xx)
  #include "stm32l021xx.h"
#elif defined(STM32L031xx)
  #include "stm32l031xx.h"
#elif defined(STM32L041xx)
  #include "stm32l041xx.h"
#elif defined(STM32L051xx)
  #include "stm32l051xx.h"
#elif defined(STM32L052xx)
  #include "stm32l052xx.h"
#elif defined(STM32L053xx)
  #include "stm32l053xx.h"
#elif defined(STM32L062xx)
  #include "stm32l062xx.h"
#elif defined(STM32L063xx)
  #include "stm32l063xx.h"
#elif defined(STM32L061xx)
  #include "stm32l061xx.h"
#elif defined(STM32L071xx)
  //#include "stm32l073xx.h"
  #include "stm32l071xx.h"
#elif defined(STM32L072xx)
  #include "stm32l072xx.h"
#elif defined(STM32L073xx)
  #include "stm32l071xx.h"
  //#include "stm32l073xx.h"
#elif defined(STM32L082xx)
  #include "stm32l082xx.h"
#elif defined(STM32L083xx)
  #include "stm32l083xx.h"
#elif defined(STM32L081xx)
  #include "stm32l081xx.h"
#else
 #error "Please select first the target STM32L0xx device used in your application (in stm32l0xx.h file)"
#endif

/**
  * @}
  */

/** @addtogroup Exported_types
  * @{
  */ 
typedef enum 
{
  RESET = 0, 
  SET = !RESET
} FlagStatus, ITStatus;

typedef enum 
{
  DISABLE = 0, 
  ENABLE = !DISABLE
} FunctionalState;
#define IS_FUNCTIONAL_STATE(STATE) (((STATE) == DISABLE) || ((STATE) == ENABLE))

typedef enum 
{
  ERROR = 0, 
  SUCCESS = !ERROR
} ErrorStatus;

/**
  * @}
  */


/** @addtogroup Exported_macro
  * @{
  */
#define SET_BIT(REG, BIT)     ((REG) |= (BIT))

#define CLEAR_BIT(REG, BIT)   ((REG) &= ~(BIT))

#define READ_BIT(REG, BIT)    ((REG) & (BIT))

#define CLEAR_REG(REG)        ((REG) = (0x0))

#define WRITE_REG(REG, VAL)   ((REG) = (VAL))

#define READ_REG(REG)         ((REG))

#define MODIFY_REG(REG, CLEARMASK, SETMASK)  WRITE_REG((REG), (((READ_REG(REG)) & (~(CLEARMASK))) | (SETMASK)))

#ifdef __cplusplus
}
#endif /* __cplusplus */

/**
  * @}
  */

#if defined (USE_HAL_DRIVER)
#include "stm32l0xx_hal.h"
#endif /* USE_HAL_DRIVER */


#endif /* __STM32L0xx_H */
/**
  * @}
  */

/**
  * @}
  */
  



/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
