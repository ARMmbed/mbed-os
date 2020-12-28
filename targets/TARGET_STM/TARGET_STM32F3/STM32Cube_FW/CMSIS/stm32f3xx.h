/**
  ******************************************************************************
  * @file    stm32f3xx.h
  * @author  MCD Application Team
  * @brief   CMSIS STM32F3xx Device Peripheral Access Layer Header File.           
  *            
  *          The file is the unique include file that the application programmer
  *          is using in the C source code, usually in main.c. This file contains:
  *           - Configuration section that allows to select:
  *              - The STM32F3xx device used in the target application
  *              - To use or not the peripheral�s drivers in application code(i.e. 
  *                code will be based on direct access to peripheral�s registers 
  *                rather than drivers API), this option is controlled by 
  *                "#define USE_HAL_DRIVER"
  *  
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

/** @addtogroup CMSIS
  * @{
  */

/** @addtogroup stm32f3xx
  * @{
  */
    
#ifndef __STM32F3xx_H
#define __STM32F3xx_H

#ifdef __cplusplus
 extern "C" {
#endif /* __cplusplus */
   
/** @addtogroup Library_configuration_section
  * @{
  */

/**
  * @brief STM32 Family
  */
#if !defined (STM32F3)
#define STM32F3
#endif /* STM32F3 */

/* Uncomment the line below according to the target STM32 device used in your
   application 
  */

#if !defined (STM32F301x8) && !defined (STM32F302x8) && !defined (STM32F318xx) && \
    !defined (STM32F302xC) && !defined (STM32F303xC) && !defined (STM32F358xx) && \
    !defined (STM32F303x8) && !defined (STM32F334x8) && !defined (STM32F328xx) && \
    !defined (STM32F302xE) && !defined (STM32F303xE) && !defined (STM32F398xx) && \
    !defined (STM32F373xC) && !defined (STM32F378xx)
    
  /* #define STM32F301x8 */   /*!< STM32F301K6, STM32F301K8, STM32F301C6, STM32F301C8,
                                   STM32F301R6 and STM32F301R8 Devices */
  /* #define STM32F302x8 */   /*!< STM32F302K6, STM32F302K8, STM32F302C6, STM32F302C8,
                                   STM32F302R6 and STM32F302R8 Devices */
  /* #define STM32F302xC */   /*!< STM32F302CB, STM32F302CC, STM32F302RB, STM32F302RC,
                                   STM32F302VB and STM32F302VC Devices */
  /* #define STM32F302xE */   /*!< STM32F302RE, STM32F302VE, STM32F302ZE, STM32F302RD,
                                   STM32F302VD and STM32F302ZD Devices */
  /* #define STM32F303x8 */   /*!< STM32F303K6, STM32F303K8, STM32F303C6, STM32F303C8, 
                                   STM32F303R6 and STM32F303R8 Devices */
  /* #define STM32F303xC */   /*!< STM32F303CB, STM32F303CC, STM32F303RB, STM32F303RC,
                                   STM32F303VB and STM32F303VC Devices */
  /* #define STM32F303xE */   /*!< STM32F303RE, STM32F303VE, STM32F303ZE, STM32F303RD,
                                   STM32F303VD and STM32F303ZD Devices */
  /* #define STM32F373xC */   /*!< STM32F373C8, STM32F373CB, STM32F373CC,
                                   STM32F373R8, STM32F373RB, STM32F373RC,
                                   STM32F373V8, STM32F373VB and STM32F373VC Devices */
  /* #define STM32F334x8 */   /*!< STM32F334K4, STM32F334K6, STM32F334K8,
                                   STM32F334C4, STM32F334C6, STM32F334C8,
                                   STM32F334R4, STM32F334R6 and STM32F334R8 Devices */
  /* #define STM32F318xx */   /*!< STM32F318K8, STM32F318C8: STM32F301x8 with regulator off: STM32F318xx Devices */
  /* #define STM32F328xx */   /*!< STM32F328C8, STM32F328R8: STM32F334x8 with regulator off: STM32F328xx Devices */
  /* #define STM32F358xx */   /*!< STM32F358CC, STM32F358RC, STM32F358VC: STM32F303xC with regulator off: STM32F358xx Devices */
  /* #define STM32F378xx */   /*!< STM32F378CC, STM32F378RC, STM32F378VC: STM32F373xC with regulator off: STM32F378xx Devices */
  /* #define STM32F398xx */   /*!< STM32F398VE: STM32F303xE with regulator off: STM32F398xx Devices */
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
  /*#define USE_HAL_DRIVER */
#endif /* USE_HAL_DRIVER */

/**
  * @brief CMSIS Device version number V2.3.4
  */
#define __STM32F3_CMSIS_VERSION_MAIN   (0x02) /*!< [31:24] main version */
#define __STM32F3_CMSIS_VERSION_SUB1   (0x03) /*!< [23:16] sub1 version */
#define __STM32F3_CMSIS_VERSION_SUB2   (0x04) /*!< [15:8]  sub2 version */
#define __STM32F3_CMSIS_VERSION_RC     (0x00) /*!< [7:0]  release candidate */
#define __STM32F3_CMSIS_VERSION        ((__STM32F3_CMSIS_VERSION_MAIN     << 24)\
                                       |(__STM32F3_CMSIS_VERSION_SUB1 << 16)\
                                       |(__STM32F3_CMSIS_VERSION_SUB2 << 8 )\
                                       |(__STM32F3_CMSIS_VERSION_RC))

/**
  * @}
  */

/** @addtogroup Device_Included
  * @{
  */

#if defined(STM32F301x8)
  #include "stm32f301x8.h"
#elif defined(STM32F302x8)
  #include "stm32f302x8.h"
#elif defined(STM32F302xC)
  #include "stm32f302xc.h"
#elif defined(STM32F302xE)
  #include "stm32f302xe.h"
#elif defined(STM32F303x8)
  #include "stm32f303x8.h"
#elif defined(STM32F303xC)
  #include "stm32f303xc.h"
#elif defined(STM32F303xE)
  #include "stm32f303xe.h"
#elif defined(STM32F373xC)
  #include "stm32f373xc.h"
#elif defined(STM32F334x8)
  #include "stm32f334x8.h"
#elif defined(STM32F318xx)
  #include "stm32f318xx.h"
#elif defined(STM32F328xx)
  #include "stm32f328xx.h"
#elif defined(STM32F358xx)
  #include "stm32f358xx.h"
#elif defined(STM32F378xx)
  #include "stm32f378xx.h"
#elif defined(STM32F398xx)
  #include "stm32f398xx.h"
#else
 #error "Please select first the target STM32F3xx device used in your application (in stm32f3xx.h file)"
#endif

/**
  * @}
  */

/** @addtogroup Exported_types
  * @{
  */ 
typedef enum 
{
  RESET = 0U, 
  SET = !RESET
} FlagStatus, ITStatus;

typedef enum 
{
  DISABLE = 0U, 
  ENABLE = !DISABLE
} FunctionalState;
#define IS_FUNCTIONAL_STATE(STATE) (((STATE) == DISABLE) || ((STATE) == ENABLE))

typedef enum 
{
  SUCCESS = 0U,
  ERROR = !SUCCESS
} ErrorStatus;

/**
  * @}
  */


/** @addtogroup Exported_macros
  * @{
  */
#define SET_BIT(REG, BIT)     ((REG) |= (BIT))

#define CLEAR_BIT(REG, BIT)   ((REG) &= ~(BIT))

#define READ_BIT(REG, BIT)    ((REG) & (BIT))

#define CLEAR_REG(REG)        ((REG) = (0x0))

#define WRITE_REG(REG, VAL)   ((REG) = (VAL))

#define READ_REG(REG)         ((REG))

#define MODIFY_REG(REG, CLEARMASK, SETMASK)  WRITE_REG((REG), (((READ_REG(REG)) & (~(CLEARMASK))) | (SETMASK)))

#define POSITION_VAL(VAL)     (__CLZ(__RBIT(VAL))) 


/**
  * @}
  */

#if defined (USE_HAL_DRIVER)
 #include "stm32f3xx_hal.h"
#endif /* USE_HAL_DRIVER */

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* __STM32F3xx_H */
/**
  * @}
  */

/**
  * @}
  */
  



/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
