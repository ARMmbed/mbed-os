/**
  ******************************************************************************
  * @file    stm32l1xx_hal_comp_ex.h
  * @author  MCD Application Team
  * @brief   Header file of COMP HAL Extension module.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2017 STMicroelectronics.
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
#ifndef __STM32L1xx_HAL_COMP_EX_H
#define __STM32L1xx_HAL_COMP_EX_H

#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32l1xx_hal_def.h"

/** @addtogroup STM32L1xx_HAL_Driver
  * @{
  */

/** @defgroup COMPEx COMPEx
  * @{
  */ 

/* Exported types ------------------------------------------------------------*/ 
/* Exported constants --------------------------------------------------------*/
/** @defgroup COMPEx_Exported_Constants COMPEx Exported Constants
  * @{
  */

/** @defgroup COMPEx_NonInvertingInput COMPEx NonInvertingInput
  * @{
  */
#if defined(STM32L151xCA) || defined (STM32L151xD) || defined (STM32L152xCA) || defined (STM32L152xD) || defined (STM32L162xCA) || defined (STM32L162xD) || defined(STM32L151xE) || defined(STM32L151xDX) || defined (STM32L152xE) || defined (STM32L152xDX) || defined (STM32L162xE) || defined (STM32L162xDX)
/* Non-inverting inputs specific to COMP2 */
#define COMP_NONINVERTINGINPUT_PB4      RI_IOSWITCH_GR6_1       /*!< I/O pin PB4 connection to COMP2 non-inverting input */
#define COMP_NONINVERTINGINPUT_PB5      RI_IOSWITCH_GR6_2       /*!< I/O pin PB5 connection to COMP2 non-inverting input */
#define COMP_NONINVERTINGINPUT_PB6      RI_IOSWITCH_GR6_3       /*!< I/O pin PB6 connection to COMP2 non-inverting input */
#define COMP_NONINVERTINGINPUT_PB7      RI_IOSWITCH_GR6_4       /*!< I/O pin PB7 connection to COMP2 non-inverting input */

/* Non-inverting inputs specific to COMP1 */
#define COMP_NONINVERTINGINPUT_NONE     (0x00000000U)           /*!< In case of window mode: No I/O pin connection to COMP1 non-inverting input. Instead, connection to COMP2 non-inverting input. */
#define COMP_NONINVERTINGINPUT_PA0      RI_IOSWITCH_CH0         /*!< I/O pin PA0 connection to COMP1 non-inverting input */
#define COMP_NONINVERTINGINPUT_PA1      RI_IOSWITCH_CH1         /*!< I/O pin PA1 connection to COMP1 non-inverting input */
#define COMP_NONINVERTINGINPUT_PA2      RI_IOSWITCH_CH2         /*!< I/O pin PA2 connection to COMP1 non-inverting input */
#define COMP_NONINVERTINGINPUT_PA3      RI_IOSWITCH_CH3         /*!< I/O pin PA3 connection to COMP1 non-inverting input */
#define COMP_NONINVERTINGINPUT_PA4      RI_IOSWITCH_CH4         /*!< I/O pin PA4 connection to COMP1 non-inverting input */
#define COMP_NONINVERTINGINPUT_PA5      RI_IOSWITCH_CH5         /*!< I/O pin PA5 connection to COMP1 non-inverting input */
#define COMP_NONINVERTINGINPUT_PA6      RI_IOSWITCH_CH6         /*!< I/O pin PA6 connection to COMP1 non-inverting input */
#define COMP_NONINVERTINGINPUT_PA7      RI_IOSWITCH_CH7         /*!< I/O pin PA7 connection to COMP1 non-inverting input */
#define COMP_NONINVERTINGINPUT_PB0      RI_IOSWITCH_CH8         /*!< I/O pin PB0 connection to COMP1 non-inverting input */
#define COMP_NONINVERTINGINPUT_PB1      RI_IOSWITCH_CH9         /*!< I/O pin PB1 connection to COMP1 non-inverting input */
#define COMP_NONINVERTINGINPUT_PC0      RI_IOSWITCH_CH10        /*!< I/O pin PC0 connection to COMP1 non-inverting input */
#define COMP_NONINVERTINGINPUT_PC1      RI_IOSWITCH_CH11        /*!< I/O pin PC1 connection to COMP1 non-inverting input */
#define COMP_NONINVERTINGINPUT_PC2      RI_IOSWITCH_CH12        /*!< I/O pin PC2 connection to COMP1 non-inverting input */
#define COMP_NONINVERTINGINPUT_PC3      RI_IOSWITCH_CH13        /*!< I/O pin PC3 connection to COMP1 non-inverting input */
#define COMP_NONINVERTINGINPUT_PC4      RI_IOSWITCH_CH14        /*!< I/O pin PC4 connection to COMP1 non-inverting input */
#define COMP_NONINVERTINGINPUT_PC5      RI_IOSWITCH_CH15        /*!< I/O pin PC5 connection to COMP1 non-inverting input */
#define COMP_NONINVERTINGINPUT_PB12     RI_IOSWITCH_CH18        /*!< I/O pin PB12 connection to COMP1 non-inverting input */
#define COMP_NONINVERTINGINPUT_PB13     RI_IOSWITCH_CH19        /*!< I/O pin PB13 connection to COMP1 non-inverting input */
#define COMP_NONINVERTINGINPUT_PB14     RI_IOSWITCH_CH20        /*!< I/O pin PB14 connection to COMP1 non-inverting input */
#define COMP_NONINVERTINGINPUT_PB15     RI_IOSWITCH_CH21        /*!< I/O pin PB15 connection to COMP1 non-inverting input */
#define COMP_NONINVERTINGINPUT_PE7      RI_IOSWITCH_CH22        /*!< I/O pin PE7 connection to COMP1 non-inverting input */
#define COMP_NONINVERTINGINPUT_PE8      RI_IOSWITCH_CH23        /*!< I/O pin PE8 connection to COMP1 non-inverting input */
#define COMP_NONINVERTINGINPUT_PE9      RI_IOSWITCH_CH24        /*!< I/O pin PE9 connection to COMP1 non-inverting input */
#define COMP_NONINVERTINGINPUT_PE10     RI_IOSWITCH_CH25        /*!< I/O pin PE10 connection to COMP1 non-inverting input */
#define COMP_NONINVERTINGINPUT_PF6      RI_IOSWITCH_CH27        /*!< I/O pin PF6 connection to COMP1 non-inverting input */
#define COMP_NONINVERTINGINPUT_PF7      RI_IOSWITCH_CH28        /*!< I/O pin PF7 connection to COMP1 non-inverting input */
#define COMP_NONINVERTINGINPUT_PF8      RI_IOSWITCH_CH29        /*!< I/O pin PF8 connection to COMP1 non-inverting input */
#define COMP_NONINVERTINGINPUT_PF9      RI_IOSWITCH_CH30        /*!< I/O pin PF9 connection to COMP1 non-inverting input */
#define COMP_NONINVERTINGINPUT_PF10     RI_IOSWITCH_CH31        /*!< I/O pin PF10 connection to COMP1 non-inverting input */

#define COMP_NONINVERTINGINPUT_OPAMP1   COMP_NONINVERTINGINPUT_PA3  /*!< OPAMP1 output connection to COMP1 non-inverting input */
#define COMP_NONINVERTINGINPUT_OPAMP2   COMP_NONINVERTINGINPUT_PB0  /*!< OPAMP2 output connection to COMP1 non-inverting input */
#if defined(STM32L151xCA) || defined (STM32L151xD) || defined (STM32L152xCA) || defined (STM32L152xD) || defined (STM32L162xCA) || defined (STM32L162xD)
#define COMP_NONINVERTINGINPUT_OPAMP3   COMP_NONINVERTINGINPUT_PC3  /*!< OPAMP3 output connection to COMP1 non-inverting input */
#endif /* STM32L151xCA || STM32L151xD || STM32L152xCA || STM32L152xD || STM32L162xCA || STM32L162xD */
#endif /* STM32L151xCA || STM32L151xD || STM32L152xCA || STM32L152xD || STM32L162xCA || STM32L162xD || STM32L151xE || STM32L151xDX || STM32L152xE || STM32L152xDX || STM32L162xE || STM32L162xDX */
   
#if defined(STM32L100xC) || defined (STM32L151xC) || defined (STM32L152xC) || defined (STM32L162xC)
/* Non-inverting inputs specific to COMP2 */
#define COMP_NONINVERTINGINPUT_PB4      RI_IOSWITCH_GR6_1       /*!< I/O pin PB4 connection to COMP2 non-inverting input */
#define COMP_NONINVERTINGINPUT_PB5      RI_IOSWITCH_GR6_2       /*!< I/O pin PB5 connection to COMP2 non-inverting input */
#define COMP_NONINVERTINGINPUT_PB6      RI_IOSWITCH_GR6_3       /*!< I/O pin PB6 connection to COMP2 non-inverting input */
#define COMP_NONINVERTINGINPUT_PB7      RI_IOSWITCH_GR6_4       /*!< I/O pin PB7 connection to COMP2 non-inverting input */

/* Non-inverting inputs specific to COMP1 */
#define COMP_NONINVERTINGINPUT_NONE     (0x00000000U)  /*!< In case of window mode: No I/O pin connection to COMP1 non-inverting input. Instead, connection to COMP2 non-inverting input. */
#define COMP_NONINVERTINGINPUT_PA0      RI_IOSWITCH_CH0         /*!< I/O pin PA0 connection to COMP1 non-inverting input */
#define COMP_NONINVERTINGINPUT_PA1      RI_IOSWITCH_CH1         /*!< I/O pin PA1 connection to COMP1 non-inverting input */
#define COMP_NONINVERTINGINPUT_PA2      RI_IOSWITCH_CH2         /*!< I/O pin PA2 connection to COMP1 non-inverting input */
#define COMP_NONINVERTINGINPUT_PA3      RI_IOSWITCH_CH3         /*!< I/O pin PA3 connection to COMP1 non-inverting input */
#define COMP_NONINVERTINGINPUT_PA4      RI_IOSWITCH_CH4         /*!< I/O pin PA4 connection to COMP1 non-inverting input */
#define COMP_NONINVERTINGINPUT_PA5      RI_IOSWITCH_CH5         /*!< I/O pin PA5 connection to COMP1 non-inverting input */
#define COMP_NONINVERTINGINPUT_PA6      RI_IOSWITCH_CH5         /*!< I/O pin PA5 connection to COMP1 non-inverting input */
#define COMP_NONINVERTINGINPUT_PA7      RI_IOSWITCH_CH7         /*!< I/O pin PA7 connection to COMP1 non-inverting input */
#define COMP_NONINVERTINGINPUT_PB0      RI_IOSWITCH_CH8         /*!< I/O pin PB0 connection to COMP1 non-inverting input */
#define COMP_NONINVERTINGINPUT_PB1      RI_IOSWITCH_CH9         /*!< I/O pin PB1 connection to COMP1 non-inverting input */
#define COMP_NONINVERTINGINPUT_PC0      RI_IOSWITCH_CH10        /*!< I/O pin PC0 connection to COMP1 non-inverting input */
#define COMP_NONINVERTINGINPUT_PC1      RI_IOSWITCH_CH11        /*!< I/O pin PC1 connection to COMP1 non-inverting input */
#define COMP_NONINVERTINGINPUT_PC2      RI_IOSWITCH_CH12        /*!< I/O pin PC2 connection to COMP1 non-inverting input */
#define COMP_NONINVERTINGINPUT_PC3      RI_IOSWITCH_CH13        /*!< I/O pin PC3 connection to COMP1 non-inverting input */
#define COMP_NONINVERTINGINPUT_PC4      RI_IOSWITCH_CH14        /*!< I/O pin PC4 connection to COMP1 non-inverting input */
#define COMP_NONINVERTINGINPUT_PC5      RI_IOSWITCH_CH15        /*!< I/O pin PC5 connection to COMP1 non-inverting input */
#define COMP_NONINVERTINGINPUT_PB12     RI_IOSWITCH_CH18        /*!< I/O pin PB12 connection to COMP1 non-inverting input */
#define COMP_NONINVERTINGINPUT_PB13     RI_IOSWITCH_CH19        /*!< I/O pin PB13 connection to COMP1 non-inverting input */
#define COMP_NONINVERTINGINPUT_PB14     RI_IOSWITCH_CH20        /*!< I/O pin PB14 connection to COMP1 non-inverting input */
#define COMP_NONINVERTINGINPUT_PB15     RI_IOSWITCH_CH21        /*!< I/O pin PB15 connection to COMP1 non-inverting input */
#define COMP_NONINVERTINGINPUT_PE7      RI_IOSWITCH_CH22        /*!< I/O pin PE7 connection to COMP1 non-inverting input */
#define COMP_NONINVERTINGINPUT_PE8      RI_IOSWITCH_CH23        /*!< I/O pin PE8 connection to COMP1 non-inverting input */
#define COMP_NONINVERTINGINPUT_PE9      RI_IOSWITCH_CH24        /*!< I/O pin PE9 connection to COMP1 non-inverting input */
#define COMP_NONINVERTINGINPUT_PE10     RI_IOSWITCH_CH25        /*!< I/O pin PE10 connection to COMP1 non-inverting input */

#define COMP_NONINVERTINGINPUT_OPAMP1   COMP_NONINVERTINGINPUT_PA3  /*!< OPAMP1 output connection to COMP1 non-inverting input */
#define COMP_NONINVERTINGINPUT_OPAMP2   COMP_NONINVERTINGINPUT_PB0  /*!< OPAMP2 output connection to COMP1 non-inverting input */
#endif /* STM32L100xC || STM32L151xC || STM32L152xC || STM32L162xC */   
   
#if defined(STM32L100xB) || defined (STM32L151xB) || defined (STM32L152xB) || defined(STM32L100xBA) || defined (STM32L151xBA) || defined (STM32L152xBA)
/* Non-inverting inputs specific to COMP2 */
#define COMP_NONINVERTINGINPUT_PB4      RI_IOSWITCH_GR6_1       /*!< I/O pin PB4 connection to COMP2 non-inverting input */
#define COMP_NONINVERTINGINPUT_PB5      RI_IOSWITCH_GR6_2       /*!< I/O pin PB5 connection to COMP2 non-inverting input */

/* Non-inverting inputs specific to COMP1 */
#define COMP_NONINVERTINGINPUT_NONE     (0x00000000U)  /*!< In case of window mode: No I/O pin connection to COMP1 non-inverting input. Instead, connection to COMP2 non-inverting input. */
#define COMP_NONINVERTINGINPUT_PA0      RI_IOSWITCH_CH0         /*!< I/O pin PA0 connection to COMP1 non-inverting input */
#define COMP_NONINVERTINGINPUT_PA1      RI_IOSWITCH_CH1         /*!< I/O pin PA1 connection to COMP1 non-inverting input */
#define COMP_NONINVERTINGINPUT_PA2      RI_IOSWITCH_CH2         /*!< I/O pin PA2 connection to COMP1 non-inverting input */
#define COMP_NONINVERTINGINPUT_PA3      RI_IOSWITCH_CH3         /*!< I/O pin PA3 connection to COMP1 non-inverting input */
#define COMP_NONINVERTINGINPUT_PA4      RI_IOSWITCH_CH4         /*!< I/O pin PA4 connection to COMP1 non-inverting input */
#define COMP_NONINVERTINGINPUT_PA5      RI_IOSWITCH_CH5         /*!< I/O pin PA5 connection to COMP1 non-inverting input */
#define COMP_NONINVERTINGINPUT_PA6      RI_IOSWITCH_CH5         /*!< I/O pin PA5 connection to COMP1 non-inverting input */
#define COMP_NONINVERTINGINPUT_PA7      RI_IOSWITCH_CH7         /*!< I/O pin PA7 connection to COMP1 non-inverting input */
#define COMP_NONINVERTINGINPUT_PB0      RI_IOSWITCH_CH8         /*!< I/O pin PB0 connection to COMP1 non-inverting input */
#define COMP_NONINVERTINGINPUT_PB1      RI_IOSWITCH_CH9         /*!< I/O pin PB1 connection to COMP1 non-inverting input */
#define COMP_NONINVERTINGINPUT_PC0      RI_IOSWITCH_CH10        /*!< I/O pin PC0 connection to COMP1 non-inverting input */
#define COMP_NONINVERTINGINPUT_PC1      RI_IOSWITCH_CH11        /*!< I/O pin PC1 connection to COMP1 non-inverting input */
#define COMP_NONINVERTINGINPUT_PC2      RI_IOSWITCH_CH12        /*!< I/O pin PC2 connection to COMP1 non-inverting input */
#define COMP_NONINVERTINGINPUT_PC3      RI_IOSWITCH_CH13        /*!< I/O pin PC3 connection to COMP1 non-inverting input */
#define COMP_NONINVERTINGINPUT_PC4      RI_IOSWITCH_CH14        /*!< I/O pin PC4 connection to COMP1 non-inverting input */
#define COMP_NONINVERTINGINPUT_PC5      RI_IOSWITCH_CH15        /*!< I/O pin PC5 connection to COMP1 non-inverting input */
#define COMP_NONINVERTINGINPUT_PB12     RI_IOSWITCH_CH18        /*!< I/O pin PB12 connection to COMP1 non-inverting input */
#define COMP_NONINVERTINGINPUT_PB13     RI_IOSWITCH_CH19        /*!< I/O pin PB13 connection to COMP1 non-inverting input */
#define COMP_NONINVERTINGINPUT_PB14     RI_IOSWITCH_CH20        /*!< I/O pin PB14 connection to COMP1 non-inverting input */
#define COMP_NONINVERTINGINPUT_PB15     RI_IOSWITCH_CH21        /*!< I/O pin PB15 connection to COMP1 non-inverting input */
#define COMP_NONINVERTINGINPUT_PE7      RI_IOSWITCH_CH22        /*!< I/O pin PE7 connection to COMP1 non-inverting input */
#define COMP_NONINVERTINGINPUT_PE8      RI_IOSWITCH_CH23        /*!< I/O pin PE8 connection to COMP1 non-inverting input */
#define COMP_NONINVERTINGINPUT_PE9      RI_IOSWITCH_CH24        /*!< I/O pin PE9 connection to COMP1 non-inverting input */
#define COMP_NONINVERTINGINPUT_PE10     RI_IOSWITCH_CH25        /*!< I/O pin PE10 connection to COMP1 non-inverting input */

#endif /* STM32L100xB || STM32L151xB || STM32L152xB || STM32L100xBA || STM32L151xBA || STM32L152xBA */

#if defined(STM32L151xCA) || defined (STM32L151xD) || defined (STM32L152xCA) || defined (STM32L152xD) || defined (STM32L162xCA) || defined (STM32L162xD) || defined(STM32L151xE) || defined(STM32L151xDX) || defined (STM32L152xE) || defined (STM32L152xDX) || defined (STM32L162xE) || defined (STM32L162xDX)
#define IS_COMP_NONINVERTINGINPUT(INPUT) (((INPUT) == COMP_NONINVERTINGINPUT_PB4)   || \
                                          ((INPUT) == COMP_NONINVERTINGINPUT_PB5)   || \
                                          ((INPUT) == COMP_NONINVERTINGINPUT_PB6)   || \
                                          ((INPUT) == COMP_NONINVERTINGINPUT_PB7)   || \
                                          ((INPUT) == COMP_NONINVERTINGINPUT_NONE)  || \
                                          ((INPUT) == COMP_NONINVERTINGINPUT_PA0)   || \
                                          ((INPUT) == COMP_NONINVERTINGINPUT_PA1)   || \
                                          ((INPUT) == COMP_NONINVERTINGINPUT_PA2)   || \
                                          ((INPUT) == COMP_NONINVERTINGINPUT_PA3)   || \
                                          ((INPUT) == COMP_NONINVERTINGINPUT_PA4)   || \
                                          ((INPUT) == COMP_NONINVERTINGINPUT_PA5)   || \
                                          ((INPUT) == COMP_NONINVERTINGINPUT_PA6)   || \
                                          ((INPUT) == COMP_NONINVERTINGINPUT_PA7)   || \
                                          ((INPUT) == COMP_NONINVERTINGINPUT_PB0)   || \
                                          ((INPUT) == COMP_NONINVERTINGINPUT_PB1)   || \
                                          ((INPUT) == COMP_NONINVERTINGINPUT_PC0)   || \
                                          ((INPUT) == COMP_NONINVERTINGINPUT_PC1)   || \
                                          ((INPUT) == COMP_NONINVERTINGINPUT_PC2)   || \
                                          ((INPUT) == COMP_NONINVERTINGINPUT_PC3)   || \
                                          ((INPUT) == COMP_NONINVERTINGINPUT_PC4)   || \
                                          ((INPUT) == COMP_NONINVERTINGINPUT_PC5)   || \
                                          ((INPUT) == COMP_NONINVERTINGINPUT_PB12)  || \
                                          ((INPUT) == COMP_NONINVERTINGINPUT_PB13)  || \
                                          ((INPUT) == COMP_NONINVERTINGINPUT_PB14)  || \
                                          ((INPUT) == COMP_NONINVERTINGINPUT_PB15)  || \
                                          ((INPUT) == COMP_NONINVERTINGINPUT_PE7)   || \
                                          ((INPUT) == COMP_NONINVERTINGINPUT_PE8)   || \
                                          ((INPUT) == COMP_NONINVERTINGINPUT_PE9)   || \
                                          ((INPUT) == COMP_NONINVERTINGINPUT_PE10)  || \
                                          ((INPUT) == COMP_NONINVERTINGINPUT_PF6)   || \
                                          ((INPUT) == COMP_NONINVERTINGINPUT_PF7)   || \
                                          ((INPUT) == COMP_NONINVERTINGINPUT_PF8)   || \
                                          ((INPUT) == COMP_NONINVERTINGINPUT_PF9)   || \
                                          ((INPUT) == COMP_NONINVERTINGINPUT_PF10)    )
#endif /* STM32L151xCA || STM32L151xD || STM32L152xCA || STM32L152xD || STM32L162xCA || STM32L162xD || STM32L151xE || STM32L151xDX || STM32L152xE || STM32L152xDX || STM32L162xE || STM32L162xDX */

#if defined(STM32L100xC) || defined (STM32L151xC) || defined (STM32L152xC) || defined (STM32L162xC)
#define IS_COMP_NONINVERTINGINPUT(INPUT) (((INPUT) == COMP_NONINVERTINGINPUT_PB4)   || \
                                          ((INPUT) == COMP_NONINVERTINGINPUT_PB5)   || \
                                          ((INPUT) == COMP_NONINVERTINGINPUT_PB6)   || \
                                          ((INPUT) == COMP_NONINVERTINGINPUT_PB7)   || \
                                          ((INPUT) == COMP_NONINVERTINGINPUT_NONE)  || \
                                          ((INPUT) == COMP_NONINVERTINGINPUT_PA0)   || \
                                          ((INPUT) == COMP_NONINVERTINGINPUT_PA1)   || \
                                          ((INPUT) == COMP_NONINVERTINGINPUT_PA2)   || \
                                          ((INPUT) == COMP_NONINVERTINGINPUT_PA3)   || \
                                          ((INPUT) == COMP_NONINVERTINGINPUT_PA4)   || \
                                          ((INPUT) == COMP_NONINVERTINGINPUT_PA5)   || \
                                          ((INPUT) == COMP_NONINVERTINGINPUT_PA6)   || \
                                          ((INPUT) == COMP_NONINVERTINGINPUT_PA7)   || \
                                          ((INPUT) == COMP_NONINVERTINGINPUT_PB0)   || \
                                          ((INPUT) == COMP_NONINVERTINGINPUT_PB1)   || \
                                          ((INPUT) == COMP_NONINVERTINGINPUT_PC0)   || \
                                          ((INPUT) == COMP_NONINVERTINGINPUT_PC1)   || \
                                          ((INPUT) == COMP_NONINVERTINGINPUT_PC2)   || \
                                          ((INPUT) == COMP_NONINVERTINGINPUT_PC3)   || \
                                          ((INPUT) == COMP_NONINVERTINGINPUT_PC4)   || \
                                          ((INPUT) == COMP_NONINVERTINGINPUT_PC5)   || \
                                          ((INPUT) == COMP_NONINVERTINGINPUT_PB12)  || \
                                          ((INPUT) == COMP_NONINVERTINGINPUT_PB13)  || \
                                          ((INPUT) == COMP_NONINVERTINGINPUT_PB14)  || \
                                          ((INPUT) == COMP_NONINVERTINGINPUT_PB15)  || \
                                          ((INPUT) == COMP_NONINVERTINGINPUT_PE7)   || \
                                          ((INPUT) == COMP_NONINVERTINGINPUT_PE8)   || \
                                          ((INPUT) == COMP_NONINVERTINGINPUT_PE9)   || \
                                          ((INPUT) == COMP_NONINVERTINGINPUT_PE10)    )
#endif /* STM32L100xC || STM32L151xC || STM32L152xC || STM32L162xC */

#if defined(STM32L100xB) || defined (STM32L151xB) || defined (STM32L152xB) || defined(STM32L100xBA) || defined (STM32L151xBA) || defined (STM32L152xBA)
#define IS_COMP_NONINVERTINGINPUT(INPUT) (((INPUT) == COMP_NONINVERTINGINPUT_PB4)   || \
                                          ((INPUT) == COMP_NONINVERTINGINPUT_PB5)   || \
                                          ((INPUT) == COMP_NONINVERTINGINPUT_NONE)  || \
                                          ((INPUT) == COMP_NONINVERTINGINPUT_PA0)   || \
                                          ((INPUT) == COMP_NONINVERTINGINPUT_PA1)   || \
                                          ((INPUT) == COMP_NONINVERTINGINPUT_PA2)   || \
                                          ((INPUT) == COMP_NONINVERTINGINPUT_PA3)   || \
                                          ((INPUT) == COMP_NONINVERTINGINPUT_PA4)   || \
                                          ((INPUT) == COMP_NONINVERTINGINPUT_PA5)   || \
                                          ((INPUT) == COMP_NONINVERTINGINPUT_PA6)   || \
                                          ((INPUT) == COMP_NONINVERTINGINPUT_PA7)   || \
                                          ((INPUT) == COMP_NONINVERTINGINPUT_PB0)   || \
                                          ((INPUT) == COMP_NONINVERTINGINPUT_PB1)   || \
                                          ((INPUT) == COMP_NONINVERTINGINPUT_PC0)   || \
                                          ((INPUT) == COMP_NONINVERTINGINPUT_PC1)   || \
                                          ((INPUT) == COMP_NONINVERTINGINPUT_PC2)   || \
                                          ((INPUT) == COMP_NONINVERTINGINPUT_PC3)   || \
                                          ((INPUT) == COMP_NONINVERTINGINPUT_PC4)   || \
                                          ((INPUT) == COMP_NONINVERTINGINPUT_PC5)   || \
                                          ((INPUT) == COMP_NONINVERTINGINPUT_PB12)  || \
                                          ((INPUT) == COMP_NONINVERTINGINPUT_PB13)  || \
                                          ((INPUT) == COMP_NONINVERTINGINPUT_PB14)  || \
                                          ((INPUT) == COMP_NONINVERTINGINPUT_PB15)  || \
                                          ((INPUT) == COMP_NONINVERTINGINPUT_PE7)   || \
                                          ((INPUT) == COMP_NONINVERTINGINPUT_PE8)   || \
                                          ((INPUT) == COMP_NONINVERTINGINPUT_PE9)   || \
                                          ((INPUT) == COMP_NONINVERTINGINPUT_PE10)    )
#endif /* STM32L100xB || STM32L151xB || STM32L152xB || STM32L100xBA || STM32L151xBA || STM32L152xBA */

/**
  * @}
  */
  
/**
  * @}
  */


/* Exported macro ------------------------------------------------------------*/

/** @defgroup COMPEx_Private_Macro COMP Private Macro
  * @{
  */

/**
  * @brief Specifies whether Routing Interface (RI) needs to be configured for 
  * switches of comparator non-inverting input.
  * @param  __HANDLE__ COMP handle.
  * @retval None.
  */
#if defined(STM32L151xCA) || defined (STM32L151xD) || defined (STM32L152xCA) || defined (STM32L152xD) || defined (STM32L162xCA) || defined (STM32L162xD) || defined(STM32L151xE) || defined(STM32L151xDX) || defined (STM32L152xE) || defined (STM32L152xDX) || defined (STM32L162xE) || defined (STM32L162xDX)
#define __COMP_ROUTING_INTERFACE_TOBECONFIGURED(__HANDLE__)                    \
  (((__HANDLE__)->Init.NonInvertingInput != COMP_NONINVERTINGINPUT_NONE) &&    \
   (READ_BIT(COMP->CSR, COMP_CSR_SW1) == RESET)                            )
#else
#define __COMP_ROUTING_INTERFACE_TOBECONFIGURED(__HANDLE__)                    \
  ((__HANDLE__)->Init.NonInvertingInput != COMP_NONINVERTINGINPUT_NONE)
#endif /* STM32L151xCA || STM32L151xD || STM32L152xCA || STM32L152xD || STM32L162xCA || STM32L162xD || STM32L151xE || STM32L151xDX || STM32L152xE || STM32L152xDX || STM32L162xE || STM32L162xDX */

/**
  * @}
  */ 



/* Exported functions --------------------------------------------------------*/


/**
  * @}
  */ 

/**
  * @}
  */ 

#ifdef __cplusplus
}
#endif

#endif /* __STM32L1xx_HAL_COMP_EX_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
