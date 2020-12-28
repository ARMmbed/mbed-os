/**
  ******************************************************************************
  * @file    stm32g4xx_hal_tim_ex.h
  * @author  MCD Application Team
  * @brief   Header file of TIM HAL Extended module.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2019 STMicroelectronics.
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
#ifndef STM32G4xx_HAL_TIM_EX_H
#define STM32G4xx_HAL_TIM_EX_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32g4xx_hal_def.h"

/** @addtogroup STM32G4xx_HAL_Driver
  * @{
  */

/** @addtogroup TIMEx
  * @{
  */

/* Exported types ------------------------------------------------------------*/
/** @defgroup TIMEx_Exported_Types TIM Extended Exported Types
  * @{
  */

/**
  * @brief  TIM Hall sensor Configuration Structure definition
  */

typedef struct
{
  uint32_t IC1Polarity;         /*!< Specifies the active edge of the input signal.
                                     This parameter can be a value of @ref TIM_Input_Capture_Polarity */

  uint32_t IC1Prescaler;        /*!< Specifies the Input Capture Prescaler.
                                     This parameter can be a value of @ref TIM_Input_Capture_Prescaler */

  uint32_t IC1Filter;           /*!< Specifies the input capture filter.
                                     This parameter can be a number between Min_Data = 0x0 and Max_Data = 0xF */

  uint32_t Commutation_Delay;   /*!< Specifies the pulse value to be loaded into the Capture Compare Register.
                                     This parameter can be a number between Min_Data = 0x0000 and Max_Data = 0xFFFF */
} TIM_HallSensor_InitTypeDef;

/**
  * @brief  TIM Break/Break2 input configuration
  */
typedef struct
{
  uint32_t Source;         /*!< Specifies the source of the timer break input.
                                This parameter can be a value of @ref TIMEx_Break_Input_Source */
  uint32_t Enable;         /*!< Specifies whether or not the break input source is enabled.
                                This parameter can be a value of @ref TIMEx_Break_Input_Source_Enable */
  uint32_t Polarity;       /*!< Specifies the break input source polarity.
                                This parameter can be a value of @ref TIMEx_Break_Input_Source_Polarity */
}
TIMEx_BreakInputConfigTypeDef;

/**
  * @brief  TIM Encoder index configuration
  */
typedef struct
{
  uint32_t Polarity;                  /*!< TIM Encoder index polarity.
                                           This parameter can be a value of @ref TIMEx_Encoder_Index_Polarity */
  uint32_t Prescaler;                 /*!< TIM Encoder index prescaler.
                                           This parameter can be a value of @ref TIMEx_Encoder_Index_Prescaler */
  uint32_t Filter;                    /*!< TIM Encoder index filter.
                                           This parameter can be a number between Min_Data = 0x0 and Max_Data = 0xF */
  FunctionalState  FirstIndexEnable;  /*!< Specifies whether or not the encoder first index is enabled.
                                           This parameter value can be ENABLE or DISABLE. */
  uint32_t Position;                  /*!< Specifies in which AB input configuration the index event resets the counter.
                                           This parameter can be a value of @ref TIMEx_Encoder_Index_Position */
  uint32_t Direction;                 /*!< Specifies in which counter direction the index event resets the counter.
                                           This parameter can be a value of @ref TIMEx_Encoder_Index_Direction */
} TIMEx_EncoderIndexConfigTypeDef ;

/**
  * @}
  */
/* End of exported types -----------------------------------------------------*/

/* Exported constants --------------------------------------------------------*/
/** @defgroup TIMEx_Exported_Constants TIM Extended Exported Constants
  * @{
  */

/** @defgroup TIMEx_Remap TIM Extended Remapping
  * @{
  */
#define TIM_TIM1_ETR_GPIO        0x00000000U                                                 /* !< ETR input is connected to GPIO */
#define TIM_TIM1_ETR_COMP1       TIM1_AF1_ETRSEL_0                                           /* !< ETR input is connected to COMP1_OUT */
#define TIM_TIM1_ETR_COMP2       TIM1_AF1_ETRSEL_1                                           /* !< ETR input is connected to COMP2_OUT */
#define TIM_TIM1_ETR_COMP3       (TIM1_AF1_ETRSEL_1 | TIM1_AF1_ETRSEL_0)                     /* !< ETR input is connected to COMP3_OUT */
#define TIM_TIM1_ETR_COMP4       TIM1_AF1_ETRSEL_2                                           /* !< ETR input is connected to COMP4_OUT */
#if defined(COMP5)
#define TIM_TIM1_ETR_COMP5       (TIM1_AF1_ETRSEL_2 | TIM1_AF1_ETRSEL_0)                     /* !< ETR input is connected to COMP5_OUT */
#endif /* COMP5 */
#if defined(COMP6)
#define TIM_TIM1_ETR_COMP6       (TIM1_AF1_ETRSEL_2 | TIM1_AF1_ETRSEL_1)                     /* !< ETR input is connected to COMP6_OUT */
#endif /* COMP6 */
#if defined(COMP7)
#define TIM_TIM1_ETR_COMP7       (TIM1_AF1_ETRSEL_2 | TIM1_AF1_ETRSEL_1 | TIM1_AF1_ETRSEL_0) /* !< ETR input is connected to COMP7_OUT */
#endif /* COMP7 */
#define TIM_TIM1_ETR_ADC1_AWD1   TIM1_AF1_ETRSEL_3                                           /* !< ADC1 analog watchdog 1 */
#define TIM_TIM1_ETR_ADC1_AWD2   (TIM1_AF1_ETRSEL_3 | TIM1_AF1_ETRSEL_0)                     /* !< ADC1 analog watchdog 2 */
#define TIM_TIM1_ETR_ADC1_AWD3   (TIM1_AF1_ETRSEL_3 | TIM1_AF1_ETRSEL_1)                     /* !< ADC1 analog watchdog 3 */
#if defined (ADC4)
#define TIM_TIM1_ETR_ADC4_AWD1   (TIM1_AF1_ETRSEL_3 | TIM1_AF1_ETRSEL_1 | TIM1_AF1_ETRSEL_0) /* !< ADC4 analog watchdog 1 */
#define TIM_TIM1_ETR_ADC4_AWD2   (TIM1_AF1_ETRSEL_3 | TIM1_AF1_ETRSEL_2)                     /* !< ADC4 analog watchdog 2 */
#define TIM_TIM1_ETR_ADC4_AWD3   (TIM1_AF1_ETRSEL_3 | TIM1_AF1_ETRSEL_2 | TIM1_AF1_ETRSEL_0) /* !< ADC4 analog watchdog 3 */
#endif /* ADC4 */

#define TIM_TIM2_ETR_GPIO         0x00000000U                                                /* !< ETR input is connected to GPIO */
#define TIM_TIM2_ETR_COMP1        TIM1_AF1_ETRSEL_0                                          /* !< ETR input is connected to COMP1_OUT */
#define TIM_TIM2_ETR_COMP2        TIM1_AF1_ETRSEL_1                                          /* !< ETR input is connected to COMP2_OUT */
#define TIM_TIM2_ETR_COMP3        (TIM1_AF1_ETRSEL_1 | TIM1_AF1_ETRSEL_0)                    /* !< ETR input is connected to COMP3_OUT */
#define TIM_TIM2_ETR_COMP4        TIM1_AF1_ETRSEL_2                                          /* !< ETR input is connected to COMP4_OUT */
#if defined(COMP5)
#define TIM_TIM2_ETR_COMP5        (TIM1_AF1_ETRSEL_2 | TIM1_AF1_ETRSEL_0)                    /* !< ETR input is connected to COMP5_OUT */
#endif /* COMP5 */
#if defined(COMP6)
#define TIM_TIM2_ETR_COMP6        (TIM1_AF1_ETRSEL_2 | TIM1_AF1_ETRSEL_1)                    /* !< ETR input is connected to COMP6_OUT */
#endif /* COMP6 */
#if defined(COMP7)
#define TIM_TIM2_ETR_COMP7        (TIM1_AF1_ETRSEL_2 | TIM1_AF1_ETRSEL_1 | TIM1_AF1_ETRSEL_0)/* !< ETR input is connected to COMP7_OUT */
#endif /* COMP7 */
#define TIM_TIM2_ETR_TIM3_ETR     TIM1_AF1_ETRSEL_3                                          /* !< ETR input is connected to TIM3 ETR */
#define TIM_TIM2_ETR_TIM4_ETR     (TIM1_AF1_ETRSEL_3 | TIM1_AF1_ETRSEL_0)                    /* !< ETR input is connected to TIM4 ETR */
#if defined (TIM5)
#define TIM_TIM2_ETR_TIM5_ETR     (TIM1_AF1_ETRSEL_3 | TIM1_AF1_ETRSEL_1)                    /* !< ETR input is connected to TIM5 ETR */
#endif /* TIM5 */
#define TIM_TIM2_ETR_LSE          (TIM1_AF1_ETRSEL_3 | TIM1_AF1_ETRSEL_1 | TIM1_AF1_ETRSEL_0) /* !< ETR input is connected to LSE */

#define TIM_TIM3_ETR_GPIO         0x00000000U                                                 /* !< ETR input is connected to GPIO */
#define TIM_TIM3_ETR_COMP1        TIM1_AF1_ETRSEL_0                                           /* !< ETR input is connected to COMP1_OUT */
#define TIM_TIM3_ETR_COMP2        TIM1_AF1_ETRSEL_1                                           /* !< ETR input is connected to COMP2_OUT */
#define TIM_TIM3_ETR_COMP3        (TIM1_AF1_ETRSEL_1 | TIM1_AF1_ETRSEL_0)                     /* !< ETR input is connected to COMP3_OUT */
#define TIM_TIM3_ETR_COMP4        TIM1_AF1_ETRSEL_2                                           /* !< ETR input is connected to COMP4_OUT */
#if defined(COMP5)
#define TIM_TIM3_ETR_COMP5        (TIM1_AF1_ETRSEL_2 | TIM1_AF1_ETRSEL_0)                     /* !< ETR input is connected to COMP5_OUT */
#endif /* COMP5 */
#if defined(COMP6)
#define TIM_TIM3_ETR_COMP6        (TIM1_AF1_ETRSEL_2 | TIM1_AF1_ETRSEL_1)                     /* !< ETR input is connected to COMP6_OUT */
#endif /* COMP6 */
#if defined(COMP7)
#define TIM_TIM3_ETR_COMP7        (TIM1_AF1_ETRSEL_2 | TIM1_AF1_ETRSEL_1 | TIM1_AF1_ETRSEL_0) /* !< ETR input is connected to COMP7_OUT */
#endif /* COMP7 */
#define TIM_TIM3_ETR_TIM2_ETR     TIM1_AF1_ETRSEL_3                                           /* !< ETR input is connected to TIM2 ETR */
#define TIM_TIM3_ETR_TIM4_ETR     (TIM1_AF1_ETRSEL_3 | TIM1_AF1_ETRSEL_0)                     /* !< ETR input is connected to TIM4 ETR */
#define TIM_TIM3_ETR_ADC2_AWD1    (TIM1_AF1_ETRSEL_3 | TIM1_AF1_ETRSEL_1 | TIM1_AF1_ETRSEL_0) /* !< ADC2 analog watchdog 1 */
#define TIM_TIM3_ETR_ADC2_AWD2    (TIM1_AF1_ETRSEL_3 | TIM1_AF1_ETRSEL_2)                     /* !< ADC2 analog watchdog 2 */
#define TIM_TIM3_ETR_ADC2_AWD3    (TIM1_AF1_ETRSEL_3 | TIM1_AF1_ETRSEL_2 | TIM1_AF1_ETRSEL_0) /* !< ADC2 analog watchdog 3 */

#define TIM_TIM4_ETR_GPIO         0x00000000U                                                 /* !< ETR input is connected to GPIO */
#define TIM_TIM4_ETR_COMP1        TIM1_AF1_ETRSEL_0                                           /* !< ETR input is connected to COMP1_OUT */
#define TIM_TIM4_ETR_COMP2        TIM1_AF1_ETRSEL_1                                           /* !< ETR input is connected to COMP2_OUT */
#define TIM_TIM4_ETR_COMP3        (TIM1_AF1_ETRSEL_1 | TIM1_AF1_ETRSEL_0)                     /* !< ETR input is connected to COMP3_OUT */
#define TIM_TIM4_ETR_COMP4        TIM1_AF1_ETRSEL_2                                           /* !< ETR input is connected to COMP4_OUT */
#if defined(COMP5)
#define TIM_TIM4_ETR_COMP5        (TIM1_AF1_ETRSEL_2 | TIM1_AF1_ETRSEL_0)                     /* !< ETR input is connected to COMP5_OUT */
#endif /* COMP5 */
#if defined(COMP6)
#define TIM_TIM4_ETR_COMP6        (TIM1_AF1_ETRSEL_2 | TIM1_AF1_ETRSEL_1)                     /* !< ETR input is connected to COMP6_OUT */
#endif /* COMP6 */
#if defined(COMP7)
#define TIM_TIM4_ETR_COMP7        (TIM1_AF1_ETRSEL_2 | TIM1_AF1_ETRSEL_1 | TIM1_AF1_ETRSEL_0) /* !< ETR input is connected to COMP7_OUT */
#endif /* COMP7 */
#define TIM_TIM4_ETR_TIM3_ETR     TIM1_AF1_ETRSEL_3                                           /* !< ETR input is connected to TIM3 ETR */
#if defined (TIM5)
#define TIM_TIM4_ETR_TIM5_ETR     (TIM1_AF1_ETRSEL_3 | TIM1_AF1_ETRSEL_0)                     /* !< ETR input is connected to TIM5 ETR */
#endif /* TIM5 */

#if defined (TIM5)
#define TIM_TIM5_ETR_GPIO         0x00000000U                                                 /* !< ETR input is connected to GPIO */
#define TIM_TIM5_ETR_COMP1        TIM1_AF1_ETRSEL_0                                           /* !< ETR input is connected to COMP1_OUT */
#define TIM_TIM5_ETR_COMP2        TIM1_AF1_ETRSEL_1                                           /* !< ETR input is connected to COMP2_OUT */
#define TIM_TIM5_ETR_COMP3        (TIM1_AF1_ETRSEL_1 | TIM1_AF1_ETRSEL_0)                     /* !< ETR input is connected to COMP3_OUT */
#define TIM_TIM5_ETR_COMP4        TIM1_AF1_ETRSEL_2                                           /* !< ETR input is connected to COMP4_OUT */
#if defined(COMP5)
#define TIM_TIM5_ETR_COMP5        (TIM1_AF1_ETRSEL_2 | TIM1_AF1_ETRSEL_0)                     /* !< ETR input is connected to COMP5_OUT */
#endif /* COMP5 */
#if defined(COMP6)
#define TIM_TIM5_ETR_COMP6        (TIM1_AF1_ETRSEL_2 | TIM1_AF1_ETRSEL_1)                     /* !< ETR input is connected to COMP6_OUT */
#endif /* COMP6 */
#if defined(COMP7)
#define TIM_TIM5_ETR_COMP7        (TIM1_AF1_ETRSEL_2 | TIM1_AF1_ETRSEL_1 | TIM1_AF1_ETRSEL_0) /* !< ETR input is connected to COMP7_OUT */
#endif /* COMP7 */
#define TIM_TIM5_ETR_TIM2_ETR     TIM1_AF1_ETRSEL_3                                           /* !< ETR input is connected to TIM2 ETR */
#define TIM_TIM5_ETR_TIM3_ETR     (TIM1_AF1_ETRSEL_3 | TIM1_AF1_ETRSEL_0)                     /* !< ETR input is connected to TIM3 ETR */
#endif /* TIM5 */

#define TIM_TIM8_ETR_GPIO        0x00000000U                                                  /* !< ETR input is connected to GPIO */
#define TIM_TIM8_ETR_COMP1       TIM1_AF1_ETRSEL_0                                            /* !< ETR input is connected to COMP1_OUT */
#define TIM_TIM8_ETR_COMP2       TIM1_AF1_ETRSEL_1                                            /* !< ETR input is connected to COMP2_OUT */
#define TIM_TIM8_ETR_COMP3       (TIM1_AF1_ETRSEL_1 | TIM1_AF1_ETRSEL_0)                      /* !< ETR input is connected to COMP3_OUT */
#define TIM_TIM8_ETR_COMP4       TIM1_AF1_ETRSEL_2                                            /* !< ETR input is connected to COMP4_OUT */
#if defined(COMP5)
#define TIM_TIM8_ETR_COMP5       (TIM1_AF1_ETRSEL_2 | TIM1_AF1_ETRSEL_0)                      /* !< ETR input is connected to COMP5_OUT */
#endif /* COMP5 */
#if defined(COMP6)
#define TIM_TIM8_ETR_COMP6       (TIM1_AF1_ETRSEL_2 | TIM1_AF1_ETRSEL_1)                      /* !< ETR input is connected to COMP6_OUT */
#endif /* COMP6 */
#if defined(COMP7)
#define TIM_TIM8_ETR_COMP7       (TIM1_AF1_ETRSEL_2 | TIM1_AF1_ETRSEL_1 | TIM1_AF1_ETRSEL_0)  /* !< ETR input is connected to COMP7_OUT */
#endif /* COMP7 */
#define TIM_TIM8_ETR_ADC2_AWD1   TIM1_AF1_ETRSEL_3                                            /* !< ADC2 analog watchdog 1 */
#define TIM_TIM8_ETR_ADC2_AWD2   (TIM1_AF1_ETRSEL_3 | TIM1_AF1_ETRSEL_0)                      /* !< ADC2 analog watchdog 2 */
#define TIM_TIM8_ETR_ADC2_AWD3   (TIM1_AF1_ETRSEL_3 | TIM1_AF1_ETRSEL_1)                      /* !< ADC2 analog watchdog 3 */
#if defined (ADC3)
#define TIM_TIM8_ETR_ADC3_AWD1   (TIM1_AF1_ETRSEL_3 | TIM1_AF1_ETRSEL_1 | TIM1_AF1_ETRSEL_0)  /* !< ADC3 analog watchdog 1 */
#define TIM_TIM8_ETR_ADC3_AWD2   (TIM1_AF1_ETRSEL_3 | TIM1_AF1_ETRSEL_2)                      /* !< ADC3 analog watchdog 2 */
#define TIM_TIM8_ETR_ADC3_AWD3   (TIM1_AF1_ETRSEL_3 | TIM1_AF1_ETRSEL_2 | TIM1_AF1_ETRSEL_0)  /* !< ADC3 analog watchdog 3 */
#endif /* ADC3 */

#if defined (TIM20)
#define TIM_TIM20_ETR_GPIO       0x00000000U                                                 /* !< ETR input is connected to GPIO */
#define TIM_TIM20_ETR_COMP1      TIM1_AF1_ETRSEL_0                                           /* !< ETR input is connected to COMP1_OUT */
#define TIM_TIM20_ETR_COMP2      TIM1_AF1_ETRSEL_1                                           /* !< ETR input is connected to COMP2_OUT */
#define TIM_TIM20_ETR_COMP3      (TIM1_AF1_ETRSEL_1 | TIM1_AF1_ETRSEL_0)                     /* !< ETR input is connected to COMP3_OUT */
#define TIM_TIM20_ETR_COMP4      TIM1_AF1_ETRSEL_2                                           /* !< ETR input is connected to COMP4_OUT */
#if defined(COMP5)
#define TIM_TIM20_ETR_COMP5      (TIM1_AF1_ETRSEL_2 | TIM1_AF1_ETRSEL_0)                     /* !< ETR input is connected to COMP5_OUT */
#endif /* COMP5 */
#if defined(COMP6)
#define TIM_TIM20_ETR_COMP6      (TIM1_AF1_ETRSEL_2 | TIM1_AF1_ETRSEL_1)                     /* !< ETR input is connected to COMP6_OUT */
#endif /* COMP6 */
#if defined(COMP7)
#define TIM_TIM20_ETR_COMP7      (TIM1_AF1_ETRSEL_2 | TIM1_AF1_ETRSEL_1 | TIM1_AF1_ETRSEL_0) /* !< ETR input is connected to COMP7_OUT */
#endif /* COMP7 */
#define TIM_TIM20_ETR_ADC3_AWD1  TIM1_AF1_ETRSEL_3                                           /* !< ADC3 analog watchdog 1 */
#define TIM_TIM20_ETR_ADC3_AWD2  (TIM1_AF1_ETRSEL_3 | TIM1_AF1_ETRSEL_0)                     /* !< ADC3 analog watchdog 2 */
#define TIM_TIM20_ETR_ADC3_AWD3  (TIM1_AF1_ETRSEL_3 | TIM1_AF1_ETRSEL_1)                     /* !< ADC3 analog watchdog 3 */
#if defined (ADC5)
#define TIM_TIM20_ETR_ADC5_AWD1  (TIM1_AF1_ETRSEL_3 | TIM1_AF1_ETRSEL_1 | TIM1_AF1_ETRSEL_0) /* !< ADC5 analog watchdog 1 */
#define TIM_TIM20_ETR_ADC5_AWD2  (TIM1_AF1_ETRSEL_3 | TIM1_AF1_ETRSEL_2)                     /* !< ADC5 analog watchdog 2 */
#define TIM_TIM20_ETR_ADC5_AWD3  (TIM1_AF1_ETRSEL_3 | TIM1_AF1_ETRSEL_2 | TIM1_AF1_ETRSEL_0) /* !< ADC5 analog watchdog 3 */
#endif /* ADC5 */
#endif /* TIM20 */
/**
  * @}
  */

/** @defgroup TIMEx_Break_Input TIM Extended Break input
  * @{
  */
#define TIM_BREAKINPUT_BRK     0x00000001U                                      /* !< Timer break input  */
#define TIM_BREAKINPUT_BRK2    0x00000002U                                      /* !< Timer break2 input */
/**
  * @}
  */

/** @defgroup TIMEx_Break_Input_Source TIM Extended Break input source
  * @{
  */
#define TIM_BREAKINPUTSOURCE_BKIN     0x00000001U                               /* !< An external source (GPIO) is connected to the BKIN pin  */
#define TIM_BREAKINPUTSOURCE_COMP1    0x00000002U                               /* !< The COMP1 output is connected to the break input */
#define TIM_BREAKINPUTSOURCE_COMP2    0x00000004U                               /* !< The COMP2 output is connected to the break input */
#define TIM_BREAKINPUTSOURCE_COMP3    0x00000008U                               /* !< The COMP3 output is connected to the break input */
#define TIM_BREAKINPUTSOURCE_COMP4    0x00000010U                               /* !< The COMP4 output is connected to the break input */
#if defined(COMP5)
#define TIM_BREAKINPUTSOURCE_COMP5    0x00000020U                               /* !< The COMP5 output is connected to the break input */
#endif /* COMP5 */
#if defined(COMP6)
#define TIM_BREAKINPUTSOURCE_COMP6    0x00000040U                               /* !< The COMP6 output is connected to the break input */
#endif /* COMP6 */
#if defined(COMP7)
#define TIM_BREAKINPUTSOURCE_COMP7    0x00000080U                               /* !< The COMP7 output is connected to the break input */
#endif /* COMP7 */
/**
  * @}
  */

/** @defgroup TIMEx_Break_Input_Source_Enable TIM Extended Break input source enabling
  * @{
  */
#define TIM_BREAKINPUTSOURCE_DISABLE     0x00000000U                            /* !< Break input source is disabled */
#define TIM_BREAKINPUTSOURCE_ENABLE      0x00000001U                            /* !< Break input source is enabled */
/**
  * @}
  */

/** @defgroup TIMEx_Break_Input_Source_Polarity TIM Extended Break input polarity
  * @{
  */
#define TIM_BREAKINPUTSOURCE_POLARITY_LOW     0x00000001U                       /* !< Break input source is active low */
#define TIM_BREAKINPUTSOURCE_POLARITY_HIGH    0x00000000U                       /* !< Break input source is active_high */
/**
  * @}
  */

/** @defgroup TIMEx_Timer_Input_Selection TIM Extended Timer input selection
  * @{
  */
#define TIM_TIM1_TI1_GPIO   0x00000000U                                       /*!< TIM1 input 1 is connected to GPIO */
#define TIM_TIM1_TI1_COMP1  TIM_TISEL_TI1SEL_0                                /*!< TIM1 input 1 is connected to COMP1_OUT */
#define TIM_TIM1_TI1_COMP2  TIM_TISEL_TI1SEL_1                                /*!< TIM1 input 1 is connected to COMP2_OUT */
#define TIM_TIM1_TI1_COMP3  (TIM_TISEL_TI1SEL_1 | TIM_TISEL_TI1SEL_0)         /*!< TIM1 input 1 is connected to COMP3_OUT */
#define TIM_TIM1_TI1_COMP4  TIM_TISEL_TI1SEL_2                                /*!< TIM1 input 1 is connected to COMP4_OUT */


#define TIM_TIM2_TI1_GPIO   0x00000000U                                       /*!< TIM2 input 1 is connected to GPIO */
#define TIM_TIM2_TI1_COMP1  TIM_TISEL_TI1SEL_0                                /*!< TIM2 input 1 is connected to COMP1_OUT */
#define TIM_TIM2_TI1_COMP2  TIM_TISEL_TI1SEL_1                                /*!< TIM2 input 1 is connected to COMP2_OUT */
#define TIM_TIM2_TI1_COMP3  (TIM_TISEL_TI1SEL_1 | TIM_TISEL_TI1SEL_0)         /*!< TIM2 input 1 is connected to COMP3_OUT */
#define TIM_TIM2_TI1_COMP4  TIM_TISEL_TI1SEL_2                                /*!< TIM2 input 1 is connected to COMP4_OUT */
#if defined (COMP5)
#define TIM_TIM2_TI1_COMP5  (TIM_TISEL_TI1SEL_2 | TIM_TISEL_TI1SEL_0)         /*!< TIM2 input 1 is connected to COMP5_OUT */
#endif /* COMP5 */

#define TIM_TIM2_TI2_GPIO   0x00000000U                                       /*!< TIM2 input 2 is connected to GPIO */
#define TIM_TIM2_TI2_COMP1  TIM_TISEL_TI2SEL_0                                /*!< TIM2 input 2 is connected to COMP1_OUT */
#define TIM_TIM2_TI2_COMP2  TIM_TISEL_TI2SEL_1                                /*!< TIM2 input 2 is connected to COMP2_OUT */
#define TIM_TIM2_TI2_COMP3  (TIM_TISEL_TI2SEL_1 | TIM_TISEL_TI2SEL_0)         /*!< TIM2 input 2 is connected to COMP3_OUT */
#define TIM_TIM2_TI2_COMP4  TIM_TISEL_TI2SEL_2                                /*!< TIM2 input 2 is connected to COMP4_OUT */
#if defined (COMP6)
#define TIM_TIM2_TI2_COMP6  (TIM_TISEL_TI2SEL_2 | TIM_TISEL_TI2SEL_0)         /*!< TIM2 input 2 is connected to COMP6_OUT */
#endif /* COMP6 */

#define TIM_TIM2_TI3_GPIO   0x00000000U                                       /*!< TIM2 input 3 is connected to GPIO */
#define TIM_TIM2_TI3_COMP4  TIM_TISEL_TI3SEL_0                                /*!< TIM2 input 3 is connected to COMP4_OUT */

#define TIM_TIM2_TI4_GPIO   0x00000000U                                       /*!< TIM2 input 4 is connected to GPIO */
#define TIM_TIM2_TI4_COMP1  TIM_TISEL_TI4SEL_0                                /*!< TIM2 input 4 is connected to COMP1_OUT */
#define TIM_TIM2_TI4_COMP2  TIM_TISEL_TI4SEL_1                                /*!< TIM2 input 4 is connected to COMP2_OUT */


#define TIM_TIM3_TI1_GPIO   0x00000000U                                       /*!< TIM3 input 1 is connected to GPIO */
#define TIM_TIM3_TI1_COMP1  TIM_TISEL_TI1SEL_0                                /*!< TIM3 input 1 is connected to COMP1_OUT */
#define TIM_TIM3_TI1_COMP2  TIM_TISEL_TI1SEL_1                                /*!< TIM3 input 1 is connected to COMP2_OUT */
#define TIM_TIM3_TI1_COMP3  (TIM_TISEL_TI1SEL_1 | TIM_TISEL_TI1SEL_0)         /*!< TIM3 input 1 is connected to COMP3_OUT */
#define TIM_TIM3_TI1_COMP4  TIM_TISEL_TI1SEL_2                                /*!< TIM3 input 1 is connected to COMP4_OUT */
#if defined (COMP5)
#define TIM_TIM3_TI1_COMP5  (TIM_TISEL_TI1SEL_2 | TIM_TISEL_TI1SEL_0)         /*!< TIM3 input 1 is connected to COMP5_OUT */
#endif /* COMP5 */
#if defined (COMP6)
#define TIM_TIM3_TI1_COMP6  (TIM_TISEL_TI1SEL_2 | TIM_TISEL_TI1SEL_1)         /*!< TIM3 input 1 is connected to COMP6_OUT */
#endif /* COMP6 */
#if defined (COMP7)
#define TIM_TIM3_TI1_COMP7  (TIM_TISEL_TI1SEL_2 | TIM_TISEL_TI1SEL_1 | TIM_TISEL_TI1SEL_0)   /*!< TIM3 input 1 is connected to COMP7_OUT */
#endif /* COMP7 */

#define TIM_TIM3_TI2_GPIO   0x00000000U                                       /*!< TIM3 input 2 is connected to GPIO */
#define TIM_TIM3_TI2_COMP1  TIM_TISEL_TI2SEL_0                                /*!< TIM3 input 2 is connected to COMP1_OUT */
#define TIM_TIM3_TI2_COMP2  TIM_TISEL_TI2SEL_1                                /*!< TIM3 input 2 is connected to COMP2_OUT */
#define TIM_TIM3_TI2_COMP3  (TIM_TISEL_TI2SEL_1 | TIM_TISEL_TI2SEL_0)         /*!< TIM3 input 2 is connected to COMP3_OUT */
#define TIM_TIM3_TI2_COMP4  TIM_TISEL_TI2SEL_2                                /*!< TIM3 input 2 is connected to COMP4_OUT */
#if defined (COMP5)
#define TIM_TIM3_TI2_COMP5  (TIM_TISEL_TI2SEL_2 | TIM_TISEL_TI2SEL_0)         /*!< TIM3 input 2 is connected to COMP5_OUT */
#endif /* COMP5 */
#if defined (COMP6)
#define TIM_TIM3_TI2_COMP6  (TIM_TISEL_TI2SEL_2 | TIM_TISEL_TI2SEL_1)         /*!< TIM3 input 2 is connected to COMP6_OUT */
#endif /* COMP6 */
#if defined (COMP7)
#define TIM_TIM3_TI2_COMP7  (TIM_TISEL_TI2SEL_2 | TIM_TISEL_TI2SEL_1 | TIM_TISEL_TI2SEL_0)   /*!< TIM3 input 2 is connected to COMP7_OUT */
#endif /* COMP7 */

#define TIM_TIM3_TI3_GPIO   0x00000000U                                       /*!< TIM3 input 3 is connected to GPIO */
#define TIM_TIM3_TI3_COMP3  TIM_TISEL_TI3SEL_0                                /*!< TIM3 input 3 is connected to COMP3_OUT */


#define TIM_TIM4_TI1_GPIO   0x00000000U                                       /*!< TIM4 input 1 is connected to GPIO */
#define TIM_TIM4_TI1_COMP1  TIM_TISEL_TI1SEL_0                                /*!< TIM4 input 1 is connected to COMP1_OUT */
#define TIM_TIM4_TI1_COMP2  TIM_TISEL_TI1SEL_1                                /*!< TIM4 input 1 is connected to COMP2_OUT */
#define TIM_TIM4_TI1_COMP3  (TIM_TISEL_TI1SEL_1 | TIM_TISEL_TI1SEL_0)         /*!< TIM4 input 1 is connected to COMP3_OUT */
#define TIM_TIM4_TI1_COMP4  TIM_TISEL_TI1SEL_2                                /*!< TIM4 input 1 is connected to COMP4_OUT */
#if defined (COMP5)
#define TIM_TIM4_TI1_COMP5  (TIM_TISEL_TI1SEL_2 | TIM_TISEL_TI1SEL_0)         /*!< TIM4 input 1 is connected to COMP5_OUT */
#endif /* COMP5 */
#if defined (COMP6)
#define TIM_TIM4_TI1_COMP6  (TIM_TISEL_TI1SEL_2 | TIM_TISEL_TI1SEL_1)         /*!< TIM4 input 1 is connected to COMP6_OUT */
#endif /* COMP6 */
#if defined (COMP7)
#define TIM_TIM4_TI1_COMP7  (TIM_TISEL_TI1SEL_2 | TIM_TISEL_TI1SEL_1 | TIM_TISEL_TI1SEL_0)   /*!< TIM4 input 1 is connected to COMP7_OUT */
#endif /* COMP7 */

#define TIM_TIM4_TI2_GPIO   0x00000000U                                       /*!< TIM4 input 2 is connected to GPIO */
#define TIM_TIM4_TI2_COMP1  TIM_TISEL_TI2SEL_0                                /*!< TIM4 input 2 is connected to COMP1_OUT */
#define TIM_TIM4_TI2_COMP2  TIM_TISEL_TI2SEL_1                                /*!< TIM4 input 2 is connected to COMP2_OUT */
#define TIM_TIM4_TI2_COMP3  (TIM_TISEL_TI2SEL_1 | TIM_TISEL_TI2SEL_0)         /*!< TIM4 input 2 is connected to COMP3_OUT */
#define TIM_TIM4_TI2_COMP4  TIM_TISEL_TI2SEL_2                                /*!< TIM4 input 2 is connected to COMP4_OUT */
#if defined (COMP5)
#define TIM_TIM4_TI2_COMP5  (TIM_TISEL_TI2SEL_2 | TIM_TISEL_TI2SEL_0)         /*!< TIM4 input 2 is connected to COMP5_OUT */
#endif /* COMP5 */
#if defined (COMP6)
#define TIM_TIM4_TI2_COMP6  (TIM_TISEL_TI2SEL_2 | TIM_TISEL_TI2SEL_1)         /*!< TIM4 input 2 is connected to COMP6_OUT */
#endif /* COMP6 */
#if defined (COMP7)
#define TIM_TIM4_TI2_COMP7  (TIM_TISEL_TI2SEL_2 | TIM_TISEL_TI2SEL_1 | TIM_TISEL_TI2SEL_0)   /*!< TIM4 input 2 is connected to COMP7_OUT */
#endif /* COMP7 */

#define TIM_TIM4_TI3_GPIO   0x00000000U                                       /*!< TIM4 input 3 is connected to GPIO */
#if defined (COMP5)
#define TIM_TIM4_TI3_COMP5  TIM_TISEL_TI3SEL_0                                /*!< TIM4 input 3 is connected to COMP5_OUT */
#endif /* COMP5 */

#define TIM_TIM4_TI4_GPIO   0x00000000U                                       /*!< TIM4 input 4 is connected to GPIO */
#if defined (COMP6)
#define TIM_TIM4_TI4_COMP6  TIM_TISEL_TI4SEL_0                                /*!< TIM4 input 4 is connected to COMP6_OUT */
#endif /* COMP6 */


#if defined(TIM5)
#define TIM_TIM5_TI1_GPIO   0x00000000U                                       /*!< TIM5 input 1 is connected to GPIO */
#define TIM_TIM5_TI1_LSI    TIM_TISEL_TI1SEL_0                                /*!< TIM5 input 1 is connected to LSI */
#define TIM_TIM5_TI1_LSE    TIM_TISEL_TI1SEL_1                                /*!< TIM5 input 1 is connected to LSE */
#define TIM_TIM5_TI1_RTC_WK (TIM_TISEL_TI1SEL_1 | TIM_TISEL_TI1SEL_0)         /*!< TIM5 input 1 is connected to RTC_WAKEUP */
#define TIM_TIM5_TI1_COMP1  TIM_TISEL_TI1SEL_2                                /*!< TIM5 input 1 is connected to COMP1_OUT */
#define TIM_TIM5_TI1_COMP2  (TIM_TISEL_TI1SEL_2 | TIM_TISEL_TI1SEL_0)         /*!< TIM5 input 1 is connected to COMP2_OUT */
#define TIM_TIM5_TI1_COMP3  (TIM_TISEL_TI1SEL_2 | TIM_TISEL_TI1SEL_1)         /*!< TIM5 input 1 is connected to COMP3_OUT */
#define TIM_TIM5_TI1_COMP4  (TIM_TISEL_TI1SEL_2 | TIM_TISEL_TI1SEL_1 | TIM_TISEL_TI1SEL_0)   /*!< TIM5 input 1 is connected to COMP4_OUT */
#if defined(COMP5)
#define TIM_TIM5_TI1_COMP5  TIM_TISEL_TI1SEL_3                                /*!< TIM5 input 1 is connected to COMP5_OUT */
#endif /* COMP5 */
#if defined(COMP6)
#define TIM_TIM5_TI1_COMP6  (TIM_TISEL_TI1SEL_3 | TIM_TISEL_TI1SEL_0)         /*!< TIM5 input 1 is connected to COMP6_OUT */
#endif /* COMP6 */
#if defined(COMP7)
#define TIM_TIM5_TI1_COMP7  (TIM_TISEL_TI1SEL_3 | TIM_TISEL_TI1SEL_1)         /*!< TIM5 input 1 is connected to COMP7_OUT */
#endif /* COMP7 */

#define TIM_TIM5_TI2_GPIO   0x00000000U                                       /*!< TIM5 input 2 is connected to GPIO */
#define TIM_TIM5_TI2_COMP1  TIM_TISEL_TI2SEL_0                                /*!< TIM5 input 2 is connected to COMP1_OUT */
#define TIM_TIM5_TI2_COMP2  TIM_TISEL_TI2SEL_1                                /*!< TIM5 input 2 is connected to COMP2_OUT */
#define TIM_TIM5_TI2_COMP3  (TIM_TISEL_TI2SEL_1 | TIM_TISEL_TI2SEL_0)         /*!< TIM5 input 2 is connected to COMP3_OUT */
#define TIM_TIM5_TI2_COMP4  TIM_TISEL_TI2SEL_2                                /*!< TIM5 input 2 is connected to COMP4_OUT */
#if defined(COMP5)
#define TIM_TIM5_TI2_COMP5  (TIM_TISEL_TI2SEL_2 | TIM_TISEL_TI2SEL_0)         /*!< TIM5 input 2 is connected to COMP5_OUT */
#endif /* COMP5 */
#if defined(COMP6)
#define TIM_TIM5_TI2_COMP6  (TIM_TISEL_TI2SEL_2 | TIM_TISEL_TI2SEL_1)         /*!< TIM5 input 2 is connected to COMP6_OUT */
#endif /* COMP6 */
#if defined(COMP7)
#define TIM_TIM5_TI2_COMP7  (TIM_TISEL_TI2SEL_2 | TIM_TISEL_TI2SEL_1 | TIM_TISEL_TI2SEL_0)   /*!< TIM5 input 2 is connected to COMP7_OUT */
#endif /* COMP7 */
#endif /* TIM5 */


#define TIM_TIM8_TI1_GPIO   0x00000000U                                       /*!< TIM8 input 1 is connected to GPIO */
#define TIM_TIM8_TI1_COMP1  TIM_TISEL_TI1SEL_0                                /*!< TIM8 input 1 is connected to COMP1_OUT */
#define TIM_TIM8_TI1_COMP2  TIM_TISEL_TI1SEL_1                                /*!< TIM8 input 1 is connected to COMP2_OUT */
#define TIM_TIM8_TI1_COMP3  (TIM_TISEL_TI1SEL_1 | TIM_TISEL_TI1SEL_0)         /*!< TIM8 input 1 is connected to COMP3_OUT */
#define TIM_TIM8_TI1_COMP4  TIM_TISEL_TI1SEL_2                                /*!< TIM8 input 1 is connected to COMP4_OUT */


#define TIM_TIM15_TI1_GPIO  0x00000000U                                       /*!< TIM15 input 1 is connected to GPIO */
#define TIM_TIM15_TI1_LSE   TIM_TISEL_TI1SEL_0                                /*!< TIM15 input 1 is connected to LSE */
#define TIM_TIM15_TI1_COMP1 TIM_TISEL_TI1SEL_1                                /*!< TIM15 input 1 is connected to COMP1_OUT */
#define TIM_TIM15_TI1_COMP2 (TIM_TISEL_TI1SEL_1 | TIM_TISEL_TI1SEL_0)         /*!< TIM15 input 1 is connected to COMP2_OUT */
#if defined (COMP5)
#define TIM_TIM15_TI1_COMP5 TIM_TISEL_TI1SEL_2                                /*!< TIM15 input 1 is connected to COMP5_OUT */
#endif /* COMP5 */
#if defined(COMP7)
#define TIM_TIM15_TI1_COMP7 (TIM_TISEL_TI1SEL_2 | TIM_TISEL_TI1SEL_0)         /*!< TIM15 input 1 is connected to COMP7_OUT */
#endif /* COMP7 */

#define TIM_TIM15_TI2_GPIO  0x00000000U                                       /*!< TIM15 input 2 is connected to GPIO */
#define TIM_TIM15_TI2_COMP2 TIM_TISEL_TI2SEL_0                                /*!< TIM15 input 2 is connected to COMP2_OUT */
#define TIM_TIM15_TI2_COMP3 TIM_TISEL_TI2SEL_1                                /*!< TIM15 input 2 is connected to COMP3_OUT */
#if defined (COMP6)
#define TIM_TIM15_TI2_COMP6 (TIM_TISEL_TI2SEL_1 | TIM_TISEL_TI2SEL_0)         /*!< TIM15 input 2 is connected to COMP6_OUT */
#endif /* COMP6 */
#if defined(COMP7)
#define TIM_TIM15_TI2_COMP7 TIM_TISEL_TI2SEL_2                                /*!< TIM15 input 2 is connected to COMP7_OUT */
#endif /* COMP7 */


#define TIM_TIM16_TI1_GPIO    0x00000000U                                     /*!< TIM16 input 1 is connected to GPIO */
#if defined (COMP6)
#define TIM_TIM16_TI1_COMP6   TIM_TISEL_TI1SEL_0                              /*!< TIM16 input 1 is connected to COMP6_OUT */
#endif /* COMP6 */
#define TIM_TIM16_TI1_MCO     TIM_TISEL_TI1SEL_1                              /*!< TIM16 input 1 is connected to MCO */
#define TIM_TIM16_TI1_HSE_32  (TIM_TISEL_TI1SEL_1 | TIM_TISEL_TI1SEL_0)       /*!< TIM16 input 1 is connected to HSE/32 */
#define TIM_TIM16_TI1_RTC_WK  TIM_TISEL_TI1SEL_2                              /*!< TIM16 input 1 is connected to RTC_WAKEUP */
#define TIM_TIM16_TI1_LSE     (TIM_TISEL_TI1SEL_2 | TIM_TISEL_TI1SEL_0)       /*!< TIM16 input 1 is connected to LSE */
#define TIM_TIM16_TI1_LSI     (TIM_TISEL_TI1SEL_2 | TIM_TISEL_TI1SEL_1)       /*!< TIM16 input 1 is connected to LSI */


#define TIM_TIM17_TI1_GPIO    0x00000000U                                     /*!< TIM17 input 1 is connected to GPIO */
#if defined (COMP5)
#define TIM_TIM17_TI1_COMP5   TIM_TISEL_TI1SEL_0                              /*!< TIM17 input 1 is connected to COMP5_OUT */
#endif /* COMP5 */
#define TIM_TIM17_TI1_MCO     TIM_TISEL_TI1SEL_1                              /*!< TIM17 input 1 is connected to MCO */
#define TIM_TIM17_TI1_HSE_32  (TIM_TISEL_TI1SEL_1 | TIM_TISEL_TI1SEL_0)       /*!< TIM17 input 1 is connected to HSE/32 */
#define TIM_TIM17_TI1_RTC_WK  TIM_TISEL_TI1SEL_2                              /*!< TIM17 input 1 is connected to RTC_WAKEUP */
#define TIM_TIM17_TI1_LSE     (TIM_TISEL_TI1SEL_2 | TIM_TISEL_TI1SEL_0)       /*!< TIM17 input 1 is connected to LSE */
#define TIM_TIM17_TI1_LSI     (TIM_TISEL_TI1SEL_2 | TIM_TISEL_TI1SEL_1)       /*!< TIM17 input 1 is connected to LSI */


#if defined (TIM20)
#define TIM_TIM20_TI1_GPIO  0x00000000U                                      /*!< TIM20 input 1 is connected to GPIO */
#define TIM_TIM20_TI1_COMP1 TIM_TISEL_TI1SEL_0                               /*!< TIM20 input 1 is connected to COMP1_OUT */
#define TIM_TIM20_TI1_COMP2 TIM_TISEL_TI1SEL_1                               /*!< TIM20 input 1 is connected to COMP2_OUT */
#define TIM_TIM20_TI1_COMP3 (TIM_TISEL_TI1SEL_1 | TIM_TISEL_TI1SEL_0)        /*!< TIM20 input 1 is connected to COMP3_OUT */
#define TIM_TIM20_TI1_COMP4 TIM_TISEL_TI1SEL_2                               /*!< TIM20 input 1 is connected to COMP4_OUT */
#endif /* TIM20 */
/**
  * @}
  */

/** @defgroup TIMEx_SMS_Preload_Enable TIM Extended Bitfield SMS preload enabling
  * @{
  */
#define TIM_SMS_PRELOAD_SOURCE_UPDATE     0x00000000U                            /*!< Prelaod of SMS bitfield is disabled */
#define TIM_SMS_PRELOAD_SOURCE_INDEX      TIM_SMCR_SMSPS                         /*!< Preload of SMS bitfield is enabled  */
/**
  * @}
  */

/** @defgroup TIMEx_Encoder_Index_Position TIM Extended Encoder index position
  * @{
  */
#define TIM_ENCODERINDEX_POSITION_00        0x00000000U                           /*!< Encoder index position is AB=00 */
#define TIM_ENCODERINDEX_POSITION_01        TIM_ECR_IPOS_0                        /*!< Encoder index position is AB=01 */
#define TIM_ENCODERINDEX_POSITION_10        TIM_ECR_IPOS_1                        /*!< Encoder index position is AB=10 */
#define TIM_ENCODERINDEX_POSITION_11        (TIM_ECR_IPOS_1 | TIM_ECR_IPOS_0)     /*!< Encoder index position is AB=11 */
#define TIM_ENCODERINDEX_POSITION_0         0x00000000U                           /*!< In directional clock mode or clock plus direction mode, index resets the counter when clock is 0 */
#define TIM_ENCODERINDEX_POSITION_1         TIM_ECR_IPOS_0                        /*!< In directional clock mode or clock plus direction mode, index resets the counter when clock is 1 */
/**
  * @}
  */

/** @defgroup TIMEx_Encoder_Index_Direction TIM Extended Encoder index direction
  * @{
  */
#define TIM_ENCODERINDEX_DIRECTION_UP_DOWN 0x00000000U        /*!< Index resets the counter whatever the direction  */
#define TIM_ENCODERINDEX_DIRECTION_UP      TIM_ECR_IDIR_0     /*!< Index resets the counter when up-counting only   */
#define TIM_ENCODERINDEX_DIRECTION_DOWN    TIM_ECR_IDIR_1     /*!< Index resets the counter when down-counting only */
/**
  * @}
  */

/** @defgroup TIMEx_Encoder_Index_Polarity TIM Extended Encoder index polarity
  * @{
  */
#define TIM_ENCODERINDEX_POLARITY_INVERTED           TIM_ETRPOLARITY_INVERTED      /*!< Polarity for ETRx pin */
#define TIM_ENCODERINDEX_POLARITY_NONINVERTED        TIM_ETRPOLARITY_NONINVERTED   /*!< Polarity for ETRx pin */
/**
  * @}
  */

/** @defgroup TIMEx_Encoder_Index_Prescaler TIM Extended Encodder index prescaler
  * @{
  */
#define TIM_ENCODERINDEX_PRESCALER_DIV1              TIM_ETRPRESCALER_DIV1         /*!< No prescaler is used                                                   */
#define TIM_ENCODERINDEX_PRESCALER_DIV2              TIM_ETRPRESCALER_DIV2         /*!< Prescaler for External ETR pin: Capture performed once every 2 events. */
#define TIM_ENCODERINDEX_PRESCALER_DIV4              TIM_ETRPRESCALER_DIV4         /*!< Prescaler for External ETR pin: Capture performed once every 4 events. */
#define TIM_ENCODERINDEX_PRESCALER_DIV8              TIM_ETRPRESCALER_DIV8         /*!< Prescaler for External ETR pin: Capture performed once every 8 events. */
/**
  * @}
  */

/**
  * @}
  */
/* End of exported constants -------------------------------------------------*/

/* Exported macro ------------------------------------------------------------*/
/** @defgroup TIMEx_Exported_Macros TIM Extended Exported Macros
  * @{
  */

/**
  * @brief  HELPER macro calculating the prescaler value to achieve the required counter clock frequency.
  * @note   ex: @ref __HAL_TIM_CALC_PSC(80000000, 1000000);
  * @param  __TIMCLK__ timer input clock frequency (in Hz)
  * @param  __CNTCLK__ counter clock frequency (in Hz)
  * @retval Prescaler value  (between Min_Data=0 and Max_Data=65535)
  */
#define __HAL_TIM_CALC_PSC(__TIMCLK__, __CNTCLK__)   \
  ((__TIMCLK__) >= (__CNTCLK__)) ? (uint32_t)((__TIMCLK__)/(__CNTCLK__) - 1U) : 0U

/**
  * @brief  HELPER macro calculating the auto-reload value to achieve the required output signal frequency.
  * @note   ex: @ref __HAL_TIM_CALC_PERIOD(1000000, 0, 10000);
  * @param  __TIMCLK__ timer input clock frequency (in Hz)
  * @param  __PSC__ prescaler
  * @param  __FREQ__ output signal frequency (in Hz)
  * @retval  Auto-reload value  (between Min_Data=0 and Max_Data=65535)
  */
#define __HAL_TIM_CALC_PERIOD(__TIMCLK__, __PSC__, __FREQ__) \
  (((__TIMCLK__)/((__PSC__) + 1U)) >= (__FREQ__)) ? ((__TIMCLK__)/((__FREQ__) * ((__PSC__) + 1U)) - 1U) : 0U

/**
  * @brief  HELPER macro calculating the auto-reload value, with dithering feature enabled, to achieve the required output signal frequency.
  * @note   ex: @ref __HAL_TIM_CALC_PERIOD_DITHER(1000000, 0, 10000);
  * @note   This macro should be used only if dithering is already enabled
  * @param  __TIMCLK__ timer input clock frequency (in Hz)
  * @param  __PSC__ prescaler
  * @param  __FREQ__ output signal frequency (in Hz)
  * @retval  Auto-reload value  (between Min_Data=0 and Max_Data=65519)
  */
#define __HAL_TIM_CALC_PERIOD_DITHER(__TIMCLK__, __PSC__, __FREQ__) \
  (((__TIMCLK__)/((__PSC__) + 1U)) >= (__FREQ__)) ?   (uint32_t)(((uint64_t)(__TIMCLK__)*16/((__FREQ__) * ((__PSC__) + 1U)) - 16U)) : 0U

/**
  * @brief  HELPER macro calculating the compare value required to achieve the required timer output compare active/inactive delay.
  * @note   ex: @ref __HAL_TIM_CALC_PULSE(1000000, 0, 10);
  * @param  __TIMCLK__ timer input clock frequency (in Hz)
  * @param  __PSC__ prescaler
  * @param  __DELAY__ timer output compare active/inactive delay (in us)
  * @retval Compare value  (between Min_Data=0 and Max_Data=65535)
  */
#define __HAL_TIM_CALC_PULSE(__TIMCLK__, __PSC__, __DELAY__)  \
  ((uint32_t)(((uint64_t)(__TIMCLK__) * (uint64_t)(__DELAY__)) \
              / ((uint64_t)1000000U * (uint64_t)((__PSC__) + 1U))))

/**
  * @brief  HELPER macro calculating the compare value, with dithering feature enabled, to achieve the required timer output compare active/inactive delay.
  * @note   ex: @ref __HAL_TIM_CALC_PULSE_DITHER(1000000, 0, 10);
  * @note   This macro should be used only if dithering is already enabled
  * @param  __TIMCLK__ timer input clock frequency (in Hz)
  * @param  __PSC__ prescaler
  * @param  __DELAY__ timer output compare active/inactive delay (in us)
  * @retval Compare value  (between Min_Data=0 and Max_Data=65519)
  */
#define __HAL_TIM_CALC_PULSE_DITHER(__TIMCLK__, __PSC__, __DELAY__)  \
  ((uint32_t)(((uint64_t)(__TIMCLK__) * (uint64_t)(__DELAY__) * 16U) \
              / ((uint64_t)1000000U * (uint64_t)((__PSC__) + 1U))))

/**
  * @brief  HELPER macro calculating the auto-reload value to achieve the required pulse duration (when the timer operates in one pulse mode).
  * @note   ex: @ref __HAL_TIM_CALC_PERIOD_BY_DELAY(1000000, 0, 10, 20);
  * @param  __TIMCLK__ timer input clock frequency (in Hz)
  * @param  __PSC__ prescaler
  * @param  __DELAY__ timer output compare active/inactive delay (in us)
  * @param  __PULSE__ pulse duration (in us)
  * @retval Auto-reload value  (between Min_Data=0 and Max_Data=65535)
  */
#define __HAL_TIM_CALC_PERIOD_BY_DELAY(__TIMCLK__, __PSC__, __DELAY__, __PULSE__)  \
  ((uint32_t)(__HAL_TIM_CALC_PULSE((__TIMCLK__), (__PSC__), (__PULSE__)) \
              + __HAL_TIM_CALC_PULSE((__TIMCLK__), (__PSC__), (__DELAY__))))

/**
  * @brief  HELPER macro calculating the auto-reload value, with dithering feature enabled, to achieve the required pulse duration (when the timer operates in one pulse mode).
  * @note   ex: @ref __HAL_TIM_CALC_PERIOD_DITHER_BY_DELAY(1000000, 0, 10, 20);
  * @note   This macro should be used only if dithering is already enabled
  * @param  __TIMCLK__ timer input clock frequency (in Hz)
  * @param  __PSC__ prescaler
  * @param  __DELAY__ timer output compare active/inactive delay (in us)
  * @param  __PULSE__ pulse duration (in us)
  * @retval Auto-reload value  (between Min_Data=0 and Max_Data=65519)
  */
#define __HAL_TIM_CALC_PERIOD_DITHER_BY_DELAY(__TIMCLK__, __PSC__, __DELAY__, __PULSE__)  \
  ((uint32_t)(__HAL_TIM_CALC_PULSE_DITHER((__TIMCLK__), (__PSC__), (__PULSE__)) \
              + __HAL_TIM_CALC_PULSE_DITHER((__TIMCLK__), (__PSC__), (__DELAY__))))

/**
  * @}
  */
/* End of exported macro -----------------------------------------------------*/

/* Private macro -------------------------------------------------------------*/
/** @defgroup TIMEx_Private_Macros TIM Extended Private Macros
  * @{
  */
#define IS_TIM_REMAP(__REMAP__) ((((__REMAP__) & 0xFFFC3FFFU) == 0x00000000U))

#define IS_TIM_BREAKINPUT(__BREAKINPUT__)  (((__BREAKINPUT__) == TIM_BREAKINPUT_BRK)  || \
                                            ((__BREAKINPUT__) == TIM_BREAKINPUT_BRK2))

#if defined (COMP5) && defined (COMP6) && defined (COMP7)
#define IS_TIM_BREAKINPUTSOURCE(__SOURCE__)  (((__SOURCE__) == TIM_BREAKINPUTSOURCE_BKIN)  || \
                                              ((__SOURCE__) == TIM_BREAKINPUTSOURCE_COMP1) || \
                                              ((__SOURCE__) == TIM_BREAKINPUTSOURCE_COMP2) || \
                                              ((__SOURCE__) == TIM_BREAKINPUTSOURCE_COMP3) || \
                                              ((__SOURCE__) == TIM_BREAKINPUTSOURCE_COMP4) || \
                                              ((__SOURCE__) == TIM_BREAKINPUTSOURCE_COMP5) || \
                                              ((__SOURCE__) == TIM_BREAKINPUTSOURCE_COMP6) || \
                                              ((__SOURCE__) == TIM_BREAKINPUTSOURCE_COMP7))


#else
#define IS_TIM_BREAKINPUTSOURCE(__SOURCE__)  (((__SOURCE__) == TIM_BREAKINPUTSOURCE_BKIN)  || \
                                              ((__SOURCE__) == TIM_BREAKINPUTSOURCE_COMP1) || \
                                              ((__SOURCE__) == TIM_BREAKINPUTSOURCE_COMP2) || \
                                              ((__SOURCE__) == TIM_BREAKINPUTSOURCE_COMP3) || \
                                              ((__SOURCE__) == TIM_BREAKINPUTSOURCE_COMP4))

#endif /* COMP5 && COMP6 && COMP7 */
#define IS_TIM_BREAKINPUTSOURCE_STATE(__STATE__)  (((__STATE__) == TIM_BREAKINPUTSOURCE_DISABLE)  || \
                                                   ((__STATE__) == TIM_BREAKINPUTSOURCE_ENABLE))

#define IS_TIM_BREAKINPUTSOURCE_POLARITY(__POLARITY__)  (((__POLARITY__) == TIM_BREAKINPUTSOURCE_POLARITY_LOW)  || \
                                                         ((__POLARITY__) == TIM_BREAKINPUTSOURCE_POLARITY_HIGH))

#define IS_TIM_TISEL(__TISEL__) ((((__TISEL__) & 0xF0F0F0F0U) == 0x00000000U))

#define IS_TIM_TISEL_TIX_INSTANCE(INSTANCE, CHANNEL) \
  (IS_TIM_CCX_INSTANCE(INSTANCE, CHANNEL) && ((CHANNEL) < TIM_CHANNEL_5))

#if defined(TIM5) && defined(TIM20)
#define IS_TIM_CLOCKSOURCE_INSTANCE(INSTANCE, __CLOCK__) \
  ((((INSTANCE) == TIM1) &&                  \
    (((__CLOCK__) == TIM_CLOCKSOURCE_INTERNAL)  ||          \
     ((__CLOCK__) == TIM_CLOCKSOURCE_ETRMODE2)  ||          \
     ((__CLOCK__) == TIM_CLOCKSOURCE_ITR1)      ||          \
     ((__CLOCK__) == TIM_CLOCKSOURCE_ITR2)      ||          \
     ((__CLOCK__) == TIM_CLOCKSOURCE_ITR3)      ||          \
     ((__CLOCK__) == TIM_CLOCKSOURCE_TI1ED)     ||          \
     ((__CLOCK__) == TIM_CLOCKSOURCE_TI1)       ||          \
     ((__CLOCK__) == TIM_CLOCKSOURCE_TI2)       ||          \
     ((__CLOCK__) == TIM_CLOCKSOURCE_ETRMODE1)  ||          \
     ((__CLOCK__) == TIM_CLOCKSOURCE_ITR4)      ||          \
     ((__CLOCK__) == TIM_CLOCKSOURCE_ITR5)      ||          \
     ((__CLOCK__) == TIM_CLOCKSOURCE_ITR6)      ||          \
     ((__CLOCK__) == TIM_CLOCKSOURCE_ITR7)      ||          \
     ((__CLOCK__) == TIM_CLOCKSOURCE_ITR8)      ||          \
     ((__CLOCK__) == TIM_CLOCKSOURCE_ITR9)      ||          \
     ((__CLOCK__) == TIM_CLOCKSOURCE_ITR10)))            \
   ||                                        \
   (((INSTANCE) == TIM2) &&                  \
    (((__CLOCK__) == TIM_CLOCKSOURCE_INTERNAL)  ||          \
     ((__CLOCK__) == TIM_CLOCKSOURCE_ETRMODE2)  ||          \
     ((__CLOCK__) == TIM_CLOCKSOURCE_ITR0)      ||          \
     ((__CLOCK__) == TIM_CLOCKSOURCE_ITR2)      ||          \
     ((__CLOCK__) == TIM_CLOCKSOURCE_ITR3)      ||          \
     ((__CLOCK__) == TIM_CLOCKSOURCE_TI1ED)     ||          \
     ((__CLOCK__) == TIM_CLOCKSOURCE_TI1)       ||          \
     ((__CLOCK__) == TIM_CLOCKSOURCE_TI2)       ||          \
     ((__CLOCK__) == TIM_CLOCKSOURCE_ETRMODE1)  ||          \
     ((__CLOCK__) == TIM_CLOCKSOURCE_ITR4)      ||          \
     ((__CLOCK__) == TIM_CLOCKSOURCE_ITR5)      ||          \
     ((__CLOCK__) == TIM_CLOCKSOURCE_ITR6)      ||          \
     ((__CLOCK__) == TIM_CLOCKSOURCE_ITR7)      ||          \
     ((__CLOCK__) == TIM_CLOCKSOURCE_ITR8)      ||          \
     ((__CLOCK__) == TIM_CLOCKSOURCE_ITR9)      ||          \
     ((__CLOCK__) == TIM_CLOCKSOURCE_ITR10)     ||          \
     ((__CLOCK__) == TIM_CLOCKSOURCE_ITR11)))            \
   ||                                        \
   (((INSTANCE) == TIM3) &&                  \
    (((__CLOCK__) == TIM_CLOCKSOURCE_INTERNAL)  ||          \
     ((__CLOCK__) == TIM_CLOCKSOURCE_ETRMODE2)  ||          \
     ((__CLOCK__) == TIM_CLOCKSOURCE_ITR0)      ||          \
     ((__CLOCK__) == TIM_CLOCKSOURCE_ITR1)      ||          \
     ((__CLOCK__) == TIM_CLOCKSOURCE_ITR3)      ||          \
     ((__CLOCK__) == TIM_CLOCKSOURCE_TI1ED)     ||          \
     ((__CLOCK__) == TIM_CLOCKSOURCE_TI1)       ||          \
     ((__CLOCK__) == TIM_CLOCKSOURCE_TI2)       ||          \
     ((__CLOCK__) == TIM_CLOCKSOURCE_ETRMODE1)  ||          \
     ((__CLOCK__) == TIM_CLOCKSOURCE_ITR4)      ||          \
     ((__CLOCK__) == TIM_CLOCKSOURCE_ITR5)      ||          \
     ((__CLOCK__) == TIM_CLOCKSOURCE_ITR6)      ||          \
     ((__CLOCK__) == TIM_CLOCKSOURCE_ITR7)      ||          \
     ((__CLOCK__) == TIM_CLOCKSOURCE_ITR8)      ||          \
     ((__CLOCK__) == TIM_CLOCKSOURCE_ITR9)      ||          \
     ((__CLOCK__) == TIM_CLOCKSOURCE_ITR10)))            \
   ||                                        \
   (((INSTANCE) == TIM4) &&                  \
    (((__CLOCK__) == TIM_CLOCKSOURCE_INTERNAL)  ||          \
     ((__CLOCK__) == TIM_CLOCKSOURCE_ETRMODE2)  ||          \
     ((__CLOCK__) == TIM_CLOCKSOURCE_ITR0)      ||          \
     ((__CLOCK__) == TIM_CLOCKSOURCE_ITR1)      ||          \
     ((__CLOCK__) == TIM_CLOCKSOURCE_ITR2)      ||          \
     ((__CLOCK__) == TIM_CLOCKSOURCE_ITR4)      ||          \
     ((__CLOCK__) == TIM_CLOCKSOURCE_TI1ED)     ||          \
     ((__CLOCK__) == TIM_CLOCKSOURCE_TI1)       ||          \
     ((__CLOCK__) == TIM_CLOCKSOURCE_TI2)       ||          \
     ((__CLOCK__) == TIM_CLOCKSOURCE_ETRMODE1)  ||          \
     ((__CLOCK__) == TIM_CLOCKSOURCE_ITR5)      ||          \
     ((__CLOCK__) == TIM_CLOCKSOURCE_ITR6)      ||          \
     ((__CLOCK__) == TIM_CLOCKSOURCE_ITR7)      ||          \
     ((__CLOCK__) == TIM_CLOCKSOURCE_ITR8)      ||          \
     ((__CLOCK__) == TIM_CLOCKSOURCE_ITR9)      ||          \
     ((__CLOCK__) == TIM_CLOCKSOURCE_ITR10)))            \
   ||                                        \
   (((INSTANCE) == TIM5) &&                  \
    (((__CLOCK__) == TIM_CLOCKSOURCE_INTERNAL)  ||          \
     ((__CLOCK__) == TIM_CLOCKSOURCE_ETRMODE2)  ||          \
     ((__CLOCK__) == TIM_CLOCKSOURCE_ITR0)      ||          \
     ((__CLOCK__) == TIM_CLOCKSOURCE_ITR1)      ||          \
     ((__CLOCK__) == TIM_CLOCKSOURCE_ITR2)      ||          \
     ((__CLOCK__) == TIM_CLOCKSOURCE_ITR3)      ||          \
     ((__CLOCK__) == TIM_CLOCKSOURCE_TI1ED)     ||          \
     ((__CLOCK__) == TIM_CLOCKSOURCE_TI1)       ||          \
     ((__CLOCK__) == TIM_CLOCKSOURCE_TI2)       ||          \
     ((__CLOCK__) == TIM_CLOCKSOURCE_ETRMODE1)  ||          \
     ((__CLOCK__) == TIM_CLOCKSOURCE_ITR5)      ||          \
     ((__CLOCK__) == TIM_CLOCKSOURCE_ITR6)      ||          \
     ((__CLOCK__) == TIM_CLOCKSOURCE_ITR7)      ||          \
     ((__CLOCK__) == TIM_CLOCKSOURCE_ITR8)      ||          \
     ((__CLOCK__) == TIM_CLOCKSOURCE_ITR9)      ||          \
     ((__CLOCK__) == TIM_CLOCKSOURCE_ITR10)))            \
   ||                                        \
   (((INSTANCE) == TIM8) &&                  \
    (((__CLOCK__) == TIM_CLOCKSOURCE_INTERNAL)  ||          \
     ((__CLOCK__) == TIM_CLOCKSOURCE_ETRMODE2)  ||          \
     ((__CLOCK__) == TIM_CLOCKSOURCE_ITR0)      ||          \
     ((__CLOCK__) == TIM_CLOCKSOURCE_ITR1)      ||          \
     ((__CLOCK__) == TIM_CLOCKSOURCE_ITR2)      ||          \
     ((__CLOCK__) == TIM_CLOCKSOURCE_ITR3)      ||          \
     ((__CLOCK__) == TIM_CLOCKSOURCE_TI1ED)     ||          \
     ((__CLOCK__) == TIM_CLOCKSOURCE_TI1)       ||          \
     ((__CLOCK__) == TIM_CLOCKSOURCE_TI2)       ||          \
     ((__CLOCK__) == TIM_CLOCKSOURCE_ETRMODE1)  ||          \
     ((__CLOCK__) == TIM_CLOCKSOURCE_ITR4)      ||          \
     ((__CLOCK__) == TIM_CLOCKSOURCE_ITR6)      ||          \
     ((__CLOCK__) == TIM_CLOCKSOURCE_ITR7)      ||          \
     ((__CLOCK__) == TIM_CLOCKSOURCE_ITR8)      ||          \
     ((__CLOCK__) == TIM_CLOCKSOURCE_ITR9)      ||          \
     ((__CLOCK__) == TIM_CLOCKSOURCE_ITR10)))            \
   ||                                        \
   (((INSTANCE) == TIM15) &&                 \
    (((__CLOCK__) == TIM_CLOCKSOURCE_INTERNAL)  ||          \
     ((__CLOCK__) == TIM_CLOCKSOURCE_ITR0)      ||          \
     ((__CLOCK__) == TIM_CLOCKSOURCE_ITR1)      ||          \
     ((__CLOCK__) == TIM_CLOCKSOURCE_ITR2)      ||          \
     ((__CLOCK__) == TIM_CLOCKSOURCE_ITR3)      ||          \
     ((__CLOCK__) == TIM_CLOCKSOURCE_TI1ED)     ||          \
     ((__CLOCK__) == TIM_CLOCKSOURCE_TI1)       ||          \
     ((__CLOCK__) == TIM_CLOCKSOURCE_TI2)       ||          \
     ((__CLOCK__) == TIM_CLOCKSOURCE_ITR4)      ||          \
     ((__CLOCK__) == TIM_CLOCKSOURCE_ITR5)      ||          \
     ((__CLOCK__) == TIM_CLOCKSOURCE_ITR7)      ||          \
     ((__CLOCK__) == TIM_CLOCKSOURCE_ITR8)      ||          \
     ((__CLOCK__) == TIM_CLOCKSOURCE_ITR9)      ||          \
     ((__CLOCK__) == TIM_CLOCKSOURCE_ITR10)))            \
   ||                                        \
   (((INSTANCE) == TIM20) &&                 \
    (((__CLOCK__) == TIM_CLOCKSOURCE_INTERNAL)  ||          \
     ((__CLOCK__) == TIM_CLOCKSOURCE_ETRMODE2)  ||          \
     ((__CLOCK__) == TIM_CLOCKSOURCE_ITR0)      ||          \
     ((__CLOCK__) == TIM_CLOCKSOURCE_ITR1)      ||          \
     ((__CLOCK__) == TIM_CLOCKSOURCE_ITR2)      ||          \
     ((__CLOCK__) == TIM_CLOCKSOURCE_ITR3)      ||          \
     ((__CLOCK__) == TIM_CLOCKSOURCE_TI1ED)     ||          \
     ((__CLOCK__) == TIM_CLOCKSOURCE_TI1)       ||          \
     ((__CLOCK__) == TIM_CLOCKSOURCE_TI2)       ||          \
     ((__CLOCK__) == TIM_CLOCKSOURCE_ETRMODE1)  ||          \
     ((__CLOCK__) == TIM_CLOCKSOURCE_ITR4)      ||          \
     ((__CLOCK__) == TIM_CLOCKSOURCE_ITR5)      ||          \
     ((__CLOCK__) == TIM_CLOCKSOURCE_ITR6)      ||          \
     ((__CLOCK__) == TIM_CLOCKSOURCE_ITR7)      ||          \
     ((__CLOCK__) == TIM_CLOCKSOURCE_ITR8)      ||          \
     ((__CLOCK__) == TIM_CLOCKSOURCE_ITR10))))

#define IS_TIM_TRIGGER_INSTANCE(INSTANCE, __SELECTION__) \
  ((((INSTANCE) == TIM1) &&                  \
    (((__SELECTION__) == TIM_TS_ITR1)    ||          \
     ((__SELECTION__) == TIM_TS_ITR2)    ||          \
     ((__SELECTION__) == TIM_TS_ITR3)    ||          \
     ((__SELECTION__) == TIM_TS_TI1F_ED) ||          \
     ((__SELECTION__) == TIM_TS_TI1FP1)  ||          \
     ((__SELECTION__) == TIM_TS_TI2FP2)  ||          \
     ((__SELECTION__) == TIM_TS_ETRF)    ||          \
     ((__SELECTION__) == TIM_TS_ITR4)    ||          \
     ((__SELECTION__) == TIM_TS_ITR5)    ||          \
     ((__SELECTION__) == TIM_TS_ITR6)    ||          \
     ((__SELECTION__) == TIM_TS_ITR7)    ||          \
     ((__SELECTION__) == TIM_TS_ITR8)    ||          \
     ((__SELECTION__) == TIM_TS_ITR9)    ||          \
     ((__SELECTION__) == TIM_TS_ITR10)))             \
   ||                                        \
   (((INSTANCE) == TIM2) &&                  \
    (((__SELECTION__) == TIM_TS_ITR0)    ||          \
     ((__SELECTION__) == TIM_TS_ITR2)    ||          \
     ((__SELECTION__) == TIM_TS_ITR3)    ||          \
     ((__SELECTION__) == TIM_TS_TI1F_ED) ||          \
     ((__SELECTION__) == TIM_TS_TI1FP1)  ||          \
     ((__SELECTION__) == TIM_TS_TI2FP2)  ||          \
     ((__SELECTION__) == TIM_TS_ETRF)    ||          \
     ((__SELECTION__) == TIM_TS_ITR4)    ||          \
     ((__SELECTION__) == TIM_TS_ITR5)    ||          \
     ((__SELECTION__) == TIM_TS_ITR6)    ||          \
     ((__SELECTION__) == TIM_TS_ITR7)    ||          \
     ((__SELECTION__) == TIM_TS_ITR8)    ||          \
     ((__SELECTION__) == TIM_TS_ITR9)    ||          \
     ((__SELECTION__) == TIM_TS_ITR10)   ||          \
     ((__SELECTION__) == TIM_TS_ITR11)))             \
   ||                                        \
   (((INSTANCE) == TIM3) &&                  \
    (((__SELECTION__) == TIM_TS_ITR0)    ||          \
     ((__SELECTION__) == TIM_TS_ITR1)    ||          \
     ((__SELECTION__) == TIM_TS_ITR3)    ||          \
     ((__SELECTION__) == TIM_TS_TI1F_ED) ||          \
     ((__SELECTION__) == TIM_TS_TI1FP1)  ||          \
     ((__SELECTION__) == TIM_TS_TI2FP2)  ||          \
     ((__SELECTION__) == TIM_TS_ETRF)    ||          \
     ((__SELECTION__) == TIM_TS_ITR4)    ||          \
     ((__SELECTION__) == TIM_TS_ITR5)    ||          \
     ((__SELECTION__) == TIM_TS_ITR6)    ||          \
     ((__SELECTION__) == TIM_TS_ITR7)    ||          \
     ((__SELECTION__) == TIM_TS_ITR8)    ||          \
     ((__SELECTION__) == TIM_TS_ITR9)    ||          \
     ((__SELECTION__) == TIM_TS_ITR10)))             \
   ||                                        \
   (((INSTANCE) == TIM4) &&                  \
    (((__SELECTION__) == TIM_TS_ITR0)    ||          \
     ((__SELECTION__) == TIM_TS_ITR1)    ||          \
     ((__SELECTION__) == TIM_TS_ITR2)    ||          \
     ((__SELECTION__) == TIM_TS_ITR4)    ||          \
     ((__SELECTION__) == TIM_TS_TI1F_ED) ||          \
     ((__SELECTION__) == TIM_TS_TI1FP1)  ||          \
     ((__SELECTION__) == TIM_TS_TI2FP2)  ||          \
     ((__SELECTION__) == TIM_TS_ETRF)    ||          \
     ((__SELECTION__) == TIM_TS_ITR5)    ||          \
     ((__SELECTION__) == TIM_TS_ITR6)    ||          \
     ((__SELECTION__) == TIM_TS_ITR7)    ||          \
     ((__SELECTION__) == TIM_TS_ITR8)    ||          \
     ((__SELECTION__) == TIM_TS_ITR9)    ||          \
     ((__SELECTION__) == TIM_TS_ITR10)))             \
   ||                                        \
   (((INSTANCE) == TIM5) &&                  \
    (((__SELECTION__) == TIM_TS_ITR0)    ||          \
     ((__SELECTION__) == TIM_TS_ITR1)    ||          \
     ((__SELECTION__) == TIM_TS_ITR2)    ||          \
     ((__SELECTION__) == TIM_TS_ITR3)    ||          \
     ((__SELECTION__) == TIM_TS_TI1F_ED) ||          \
     ((__SELECTION__) == TIM_TS_TI1FP1)  ||          \
     ((__SELECTION__) == TIM_TS_TI2FP2)  ||          \
     ((__SELECTION__) == TIM_TS_ETRF)    ||          \
     ((__SELECTION__) == TIM_TS_ITR5)    ||          \
     ((__SELECTION__) == TIM_TS_ITR6)    ||          \
     ((__SELECTION__) == TIM_TS_ITR7)    ||          \
     ((__SELECTION__) == TIM_TS_ITR8)    ||          \
     ((__SELECTION__) == TIM_TS_ITR9)    ||          \
     ((__SELECTION__) == TIM_TS_ITR10)))             \
   ||                                        \
   (((INSTANCE) == TIM8) &&                  \
    (((__SELECTION__) == TIM_TS_ITR0)    ||          \
     ((__SELECTION__) == TIM_TS_ITR1)    ||          \
     ((__SELECTION__) == TIM_TS_ITR2)    ||          \
     ((__SELECTION__) == TIM_TS_ITR3)    ||          \
     ((__SELECTION__) == TIM_TS_TI1F_ED) ||          \
     ((__SELECTION__) == TIM_TS_TI1FP1)  ||          \
     ((__SELECTION__) == TIM_TS_TI2FP2)  ||          \
     ((__SELECTION__) == TIM_TS_ETRF)    ||          \
     ((__SELECTION__) == TIM_TS_ITR4)    ||          \
     ((__SELECTION__) == TIM_TS_ITR6)    ||          \
     ((__SELECTION__) == TIM_TS_ITR7)    ||          \
     ((__SELECTION__) == TIM_TS_ITR8)    ||          \
     ((__SELECTION__) == TIM_TS_ITR9)    ||          \
     ((__SELECTION__) == TIM_TS_ITR10)))             \
   ||                                        \
   (((INSTANCE) == TIM15) &&                 \
    (((__SELECTION__) == TIM_TS_ITR0)    ||          \
     ((__SELECTION__) == TIM_TS_ITR1)    ||          \
     ((__SELECTION__) == TIM_TS_ITR2)    ||          \
     ((__SELECTION__) == TIM_TS_ITR3)    ||          \
     ((__SELECTION__) == TIM_TS_TI1F_ED) ||          \
     ((__SELECTION__) == TIM_TS_TI1FP1)  ||          \
     ((__SELECTION__) == TIM_TS_TI2FP2)  ||          \
     ((__SELECTION__) == TIM_TS_ITR4)    ||          \
     ((__SELECTION__) == TIM_TS_ITR5)    ||          \
     ((__SELECTION__) == TIM_TS_ITR7)    ||          \
     ((__SELECTION__) == TIM_TS_ITR8)    ||          \
     ((__SELECTION__) == TIM_TS_ITR9)    ||          \
     ((__SELECTION__) == TIM_TS_ITR10)))             \
   ||                                        \
   (((INSTANCE) == TIM20) &&                 \
    (((__SELECTION__) == TIM_TS_ITR0)    ||          \
     ((__SELECTION__) == TIM_TS_ITR1)    ||          \
     ((__SELECTION__) == TIM_TS_ITR2)    ||          \
     ((__SELECTION__) == TIM_TS_ITR3)    ||          \
     ((__SELECTION__) == TIM_TS_TI1F_ED) ||          \
     ((__SELECTION__) == TIM_TS_TI1FP1)  ||          \
     ((__SELECTION__) == TIM_TS_TI2FP2)  ||          \
     ((__SELECTION__) == TIM_TS_ETRF)    ||          \
     ((__SELECTION__) == TIM_TS_ITR4)    ||          \
     ((__SELECTION__) == TIM_TS_ITR5)    ||          \
     ((__SELECTION__) == TIM_TS_ITR6)    ||          \
     ((__SELECTION__) == TIM_TS_ITR7)    ||          \
     ((__SELECTION__) == TIM_TS_ITR8)    ||          \
     ((__SELECTION__) == TIM_TS_ITR10))))

#define IS_TIM_INTERNAL_TRIGGEREVENT_INSTANCE(INSTANCE, __SELECTION__) \
  ((((INSTANCE) == TIM1) &&                  \
    (((__SELECTION__) == TIM_TS_ITR1) ||          \
     ((__SELECTION__) == TIM_TS_ITR2) ||          \
     ((__SELECTION__) == TIM_TS_ITR3) ||          \
     ((__SELECTION__) == TIM_TS_ITR4) ||          \
     ((__SELECTION__) == TIM_TS_ITR5) ||          \
     ((__SELECTION__) == TIM_TS_ITR6) ||          \
     ((__SELECTION__) == TIM_TS_ITR7) ||          \
     ((__SELECTION__) == TIM_TS_ITR8) ||          \
     ((__SELECTION__) == TIM_TS_ITR9) ||          \
     ((__SELECTION__) == TIM_TS_ITR10)||          \
     ((__SELECTION__) == TIM_TS_NONE)))           \
   ||                                        \
   (((INSTANCE) == TIM2) &&                  \
    (((__SELECTION__) == TIM_TS_ITR0) ||          \
     ((__SELECTION__) == TIM_TS_ITR2) ||          \
     ((__SELECTION__) == TIM_TS_ITR3) ||          \
     ((__SELECTION__) == TIM_TS_ITR4) ||          \
     ((__SELECTION__) == TIM_TS_ITR5) ||          \
     ((__SELECTION__) == TIM_TS_ITR6) ||          \
     ((__SELECTION__) == TIM_TS_ITR7) ||          \
     ((__SELECTION__) == TIM_TS_ITR8) ||          \
     ((__SELECTION__) == TIM_TS_ITR9) ||          \
     ((__SELECTION__) == TIM_TS_ITR10)||          \
     ((__SELECTION__) == TIM_TS_ITR11)||          \
     ((__SELECTION__) == TIM_TS_NONE)))           \
   ||                                        \
   (((INSTANCE) == TIM3) &&                  \
    (((__SELECTION__) == TIM_TS_ITR0) ||          \
     ((__SELECTION__) == TIM_TS_ITR1) ||          \
     ((__SELECTION__) == TIM_TS_ITR3) ||          \
     ((__SELECTION__) == TIM_TS_ITR4) ||          \
     ((__SELECTION__) == TIM_TS_ITR5) ||          \
     ((__SELECTION__) == TIM_TS_ITR6) ||          \
     ((__SELECTION__) == TIM_TS_ITR7) ||          \
     ((__SELECTION__) == TIM_TS_ITR8) ||          \
     ((__SELECTION__) == TIM_TS_ITR9) ||          \
     ((__SELECTION__) == TIM_TS_ITR10)||          \
     ((__SELECTION__) == TIM_TS_NONE)))           \
   ||                                        \
   (((INSTANCE) == TIM4) &&                  \
    (((__SELECTION__) == TIM_TS_ITR0) ||          \
     ((__SELECTION__) == TIM_TS_ITR1) ||          \
     ((__SELECTION__) == TIM_TS_ITR2) ||          \
     ((__SELECTION__) == TIM_TS_ITR4) ||          \
     ((__SELECTION__) == TIM_TS_ITR5) ||          \
     ((__SELECTION__) == TIM_TS_ITR6) ||          \
     ((__SELECTION__) == TIM_TS_ITR7) ||          \
     ((__SELECTION__) == TIM_TS_ITR8) ||          \
     ((__SELECTION__) == TIM_TS_ITR9) ||          \
     ((__SELECTION__) == TIM_TS_ITR10)||          \
     ((__SELECTION__) == TIM_TS_NONE)))           \
   ||                                        \
   (((INSTANCE) == TIM5) &&                  \
    (((__SELECTION__) == TIM_TS_ITR0) ||          \
     ((__SELECTION__) == TIM_TS_ITR1) ||          \
     ((__SELECTION__) == TIM_TS_ITR2) ||          \
     ((__SELECTION__) == TIM_TS_ITR3) ||          \
     ((__SELECTION__) == TIM_TS_ITR5) ||          \
     ((__SELECTION__) == TIM_TS_ITR6) ||          \
     ((__SELECTION__) == TIM_TS_ITR7) ||          \
     ((__SELECTION__) == TIM_TS_ITR8) ||          \
     ((__SELECTION__) == TIM_TS_ITR9) ||          \
     ((__SELECTION__) == TIM_TS_ITR10)||          \
     ((__SELECTION__) == TIM_TS_NONE)))           \
   ||                                        \
   (((INSTANCE) == TIM8) &&                  \
    (((__SELECTION__) == TIM_TS_ITR0) ||          \
     ((__SELECTION__) == TIM_TS_ITR1) ||          \
     ((__SELECTION__) == TIM_TS_ITR2) ||          \
     ((__SELECTION__) == TIM_TS_ITR3) ||          \
     ((__SELECTION__) == TIM_TS_ITR4) ||          \
     ((__SELECTION__) == TIM_TS_ITR6) ||          \
     ((__SELECTION__) == TIM_TS_ITR7) ||          \
     ((__SELECTION__) == TIM_TS_ITR8) ||          \
     ((__SELECTION__) == TIM_TS_ITR9) ||          \
     ((__SELECTION__) == TIM_TS_ITR10)||          \
     ((__SELECTION__) == TIM_TS_NONE)))           \
   ||                                        \
   (((INSTANCE) == TIM15) &&                 \
    (((__SELECTION__) == TIM_TS_ITR0) ||          \
     ((__SELECTION__) == TIM_TS_ITR1) ||          \
     ((__SELECTION__) == TIM_TS_ITR2) ||          \
     ((__SELECTION__) == TIM_TS_ITR3) ||          \
     ((__SELECTION__) == TIM_TS_ITR4) ||          \
     ((__SELECTION__) == TIM_TS_ITR5) ||          \
     ((__SELECTION__) == TIM_TS_ITR7) ||          \
     ((__SELECTION__) == TIM_TS_ITR8) ||          \
     ((__SELECTION__) == TIM_TS_ITR9) ||          \
     ((__SELECTION__) == TIM_TS_ITR10)||          \
     ((__SELECTION__) == TIM_TS_NONE)))           \
   ||                                        \
   (((INSTANCE) == TIM20) &&                 \
    (((__SELECTION__) == TIM_TS_ITR0) ||          \
     ((__SELECTION__) == TIM_TS_ITR1) ||          \
     ((__SELECTION__) == TIM_TS_ITR2) ||          \
     ((__SELECTION__) == TIM_TS_ITR3) ||          \
     ((__SELECTION__) == TIM_TS_ITR4) ||          \
     ((__SELECTION__) == TIM_TS_ITR5) ||          \
     ((__SELECTION__) == TIM_TS_ITR6) ||          \
     ((__SELECTION__) == TIM_TS_ITR7) ||          \
     ((__SELECTION__) == TIM_TS_ITR8) ||          \
     ((__SELECTION__) == TIM_TS_ITR10)||          \
     ((__SELECTION__) == TIM_TS_NONE))))

#elif defined(TIM5)
#define IS_TIM_CLOCKSOURCE_INSTANCE(INSTANCE, __CLOCK__) \
  ((((INSTANCE) == TIM1) &&                  \
    (((__CLOCK__) == TIM_CLOCKSOURCE_INTERNAL)  ||          \
     ((__CLOCK__) == TIM_CLOCKSOURCE_ETRMODE2)  ||          \
     ((__CLOCK__) == TIM_CLOCKSOURCE_ITR1)      ||          \
     ((__CLOCK__) == TIM_CLOCKSOURCE_ITR2)      ||          \
     ((__CLOCK__) == TIM_CLOCKSOURCE_ITR3)      ||          \
     ((__CLOCK__) == TIM_CLOCKSOURCE_TI1ED)     ||          \
     ((__CLOCK__) == TIM_CLOCKSOURCE_TI1)       ||          \
     ((__CLOCK__) == TIM_CLOCKSOURCE_TI2)       ||          \
     ((__CLOCK__) == TIM_CLOCKSOURCE_ETRMODE1)  ||          \
     ((__CLOCK__) == TIM_CLOCKSOURCE_ITR4)      ||          \
     ((__CLOCK__) == TIM_CLOCKSOURCE_ITR5)      ||          \
     ((__CLOCK__) == TIM_CLOCKSOURCE_ITR6)      ||          \
     ((__CLOCK__) == TIM_CLOCKSOURCE_ITR7)      ||          \
     ((__CLOCK__) == TIM_CLOCKSOURCE_ITR8)))            \
   ||                                        \
   (((INSTANCE) == TIM2) &&                  \
    (((__CLOCK__) == TIM_CLOCKSOURCE_INTERNAL)  ||          \
     ((__CLOCK__) == TIM_CLOCKSOURCE_ETRMODE2)  ||          \
     ((__CLOCK__) == TIM_CLOCKSOURCE_ITR0)      ||          \
     ((__CLOCK__) == TIM_CLOCKSOURCE_ITR2)      ||          \
     ((__CLOCK__) == TIM_CLOCKSOURCE_ITR3)      ||          \
     ((__CLOCK__) == TIM_CLOCKSOURCE_TI1ED)     ||          \
     ((__CLOCK__) == TIM_CLOCKSOURCE_TI1)       ||          \
     ((__CLOCK__) == TIM_CLOCKSOURCE_TI2)       ||          \
     ((__CLOCK__) == TIM_CLOCKSOURCE_ETRMODE1)  ||          \
     ((__CLOCK__) == TIM_CLOCKSOURCE_ITR4)      ||          \
     ((__CLOCK__) == TIM_CLOCKSOURCE_ITR5)      ||          \
     ((__CLOCK__) == TIM_CLOCKSOURCE_ITR6)      ||          \
     ((__CLOCK__) == TIM_CLOCKSOURCE_ITR7)      ||          \
     ((__CLOCK__) == TIM_CLOCKSOURCE_ITR8)      ||          \
     ((__CLOCK__) == TIM_CLOCKSOURCE_ITR11)))            \
   ||                                        \
   (((INSTANCE) == TIM3) &&                  \
    (((__CLOCK__) == TIM_CLOCKSOURCE_INTERNAL)  ||          \
     ((__CLOCK__) == TIM_CLOCKSOURCE_ETRMODE2)  ||          \
     ((__CLOCK__) == TIM_CLOCKSOURCE_ITR0)      ||          \
     ((__CLOCK__) == TIM_CLOCKSOURCE_ITR1)      ||          \
     ((__CLOCK__) == TIM_CLOCKSOURCE_ITR3)      ||          \
     ((__CLOCK__) == TIM_CLOCKSOURCE_TI1ED)     ||          \
     ((__CLOCK__) == TIM_CLOCKSOURCE_TI1)       ||          \
     ((__CLOCK__) == TIM_CLOCKSOURCE_TI2)       ||          \
     ((__CLOCK__) == TIM_CLOCKSOURCE_ETRMODE1)  ||          \
     ((__CLOCK__) == TIM_CLOCKSOURCE_ITR4)      ||          \
     ((__CLOCK__) == TIM_CLOCKSOURCE_ITR5)      ||          \
     ((__CLOCK__) == TIM_CLOCKSOURCE_ITR6)      ||          \
     ((__CLOCK__) == TIM_CLOCKSOURCE_ITR7)      ||          \
     ((__CLOCK__) == TIM_CLOCKSOURCE_ITR8)))            \
   ||                                        \
   (((INSTANCE) == TIM4) &&                  \
    (((__CLOCK__) == TIM_CLOCKSOURCE_INTERNAL)  ||          \
     ((__CLOCK__) == TIM_CLOCKSOURCE_ETRMODE2)  ||          \
     ((__CLOCK__) == TIM_CLOCKSOURCE_ITR0)      ||          \
     ((__CLOCK__) == TIM_CLOCKSOURCE_ITR1)      ||          \
     ((__CLOCK__) == TIM_CLOCKSOURCE_ITR2)      ||          \
     ((__CLOCK__) == TIM_CLOCKSOURCE_TI1ED)     ||          \
     ((__CLOCK__) == TIM_CLOCKSOURCE_TI1)       ||          \
     ((__CLOCK__) == TIM_CLOCKSOURCE_TI2)       ||          \
     ((__CLOCK__) == TIM_CLOCKSOURCE_ETRMODE1)  ||          \
     ((__CLOCK__) == TIM_CLOCKSOURCE_ITR4)      ||          \
     ((__CLOCK__) == TIM_CLOCKSOURCE_ITR5)      ||          \
     ((__CLOCK__) == TIM_CLOCKSOURCE_ITR6)      ||          \
     ((__CLOCK__) == TIM_CLOCKSOURCE_ITR7)      ||          \
     ((__CLOCK__) == TIM_CLOCKSOURCE_ITR8)))            \
   ||                                        \
   (((INSTANCE) == TIM5) &&                  \
    (((__CLOCK__) == TIM_CLOCKSOURCE_INTERNAL)  ||          \
     ((__CLOCK__) == TIM_CLOCKSOURCE_ETRMODE2)  ||          \
     ((__CLOCK__) == TIM_CLOCKSOURCE_ITR0)      ||          \
     ((__CLOCK__) == TIM_CLOCKSOURCE_ITR1)      ||          \
     ((__CLOCK__) == TIM_CLOCKSOURCE_ITR2)      ||          \
     ((__CLOCK__) == TIM_CLOCKSOURCE_ITR3)      ||          \
     ((__CLOCK__) == TIM_CLOCKSOURCE_TI1ED)     ||          \
     ((__CLOCK__) == TIM_CLOCKSOURCE_TI1)       ||          \
     ((__CLOCK__) == TIM_CLOCKSOURCE_TI2)       ||          \
     ((__CLOCK__) == TIM_CLOCKSOURCE_ETRMODE1)  ||          \
     ((__CLOCK__) == TIM_CLOCKSOURCE_ITR5)      ||          \
     ((__CLOCK__) == TIM_CLOCKSOURCE_ITR6)      ||          \
     ((__CLOCK__) == TIM_CLOCKSOURCE_ITR7)      ||          \
     ((__CLOCK__) == TIM_CLOCKSOURCE_ITR8)))            \
   ||                                        \
   (((INSTANCE) == TIM8) &&                  \
    (((__CLOCK__) == TIM_CLOCKSOURCE_INTERNAL)  ||          \
     ((__CLOCK__) == TIM_CLOCKSOURCE_ETRMODE2)  ||          \
     ((__CLOCK__) == TIM_CLOCKSOURCE_ITR0)      ||          \
     ((__CLOCK__) == TIM_CLOCKSOURCE_ITR1)      ||          \
     ((__CLOCK__) == TIM_CLOCKSOURCE_ITR2)      ||          \
     ((__CLOCK__) == TIM_CLOCKSOURCE_ITR3)      ||          \
     ((__CLOCK__) == TIM_CLOCKSOURCE_TI1ED)     ||          \
     ((__CLOCK__) == TIM_CLOCKSOURCE_TI1)       ||          \
     ((__CLOCK__) == TIM_CLOCKSOURCE_TI2)       ||          \
     ((__CLOCK__) == TIM_CLOCKSOURCE_ETRMODE1)  ||          \
     ((__CLOCK__) == TIM_CLOCKSOURCE_ITR4)      ||          \
     ((__CLOCK__) == TIM_CLOCKSOURCE_ITR6)      ||          \
     ((__CLOCK__) == TIM_CLOCKSOURCE_ITR7)      ||          \
     ((__CLOCK__) == TIM_CLOCKSOURCE_ITR8)))            \
   ||                                        \
   (((INSTANCE) == TIM15) &&                 \
    (((__CLOCK__) == TIM_CLOCKSOURCE_INTERNAL)  ||          \
     ((__CLOCK__) == TIM_CLOCKSOURCE_ITR0)      ||          \
     ((__CLOCK__) == TIM_CLOCKSOURCE_ITR1)      ||          \
     ((__CLOCK__) == TIM_CLOCKSOURCE_ITR2)      ||          \
     ((__CLOCK__) == TIM_CLOCKSOURCE_ITR3)      ||          \
     ((__CLOCK__) == TIM_CLOCKSOURCE_TI1ED)     ||          \
     ((__CLOCK__) == TIM_CLOCKSOURCE_TI1)       ||          \
     ((__CLOCK__) == TIM_CLOCKSOURCE_TI2)       ||          \
     ((__CLOCK__) == TIM_CLOCKSOURCE_ITR4)      ||          \
     ((__CLOCK__) == TIM_CLOCKSOURCE_ITR5)      ||          \
     ((__CLOCK__) == TIM_CLOCKSOURCE_ITR7)      ||          \
     ((__CLOCK__) == TIM_CLOCKSOURCE_ITR8))))

#define IS_TIM_TRIGGER_INSTANCE(INSTANCE, __SELECTION__) \
  ((((INSTANCE) == TIM1) &&                  \
    (((__SELECTION__) == TIM_TS_ITR1)    ||          \
     ((__SELECTION__) == TIM_TS_ITR2)    ||          \
     ((__SELECTION__) == TIM_TS_ITR3)    ||          \
     ((__SELECTION__) == TIM_TS_TI1F_ED) ||          \
     ((__SELECTION__) == TIM_TS_TI1FP1)  ||          \
     ((__SELECTION__) == TIM_TS_TI2FP2)  ||          \
     ((__SELECTION__) == TIM_TS_ETRF)    ||          \
     ((__SELECTION__) == TIM_TS_ITR4)    ||          \
     ((__SELECTION__) == TIM_TS_ITR5)    ||          \
     ((__SELECTION__) == TIM_TS_ITR6)    ||          \
     ((__SELECTION__) == TIM_TS_ITR7)    ||          \
     ((__SELECTION__) == TIM_TS_ITR8)))             \
   ||                                        \
   (((INSTANCE) == TIM2) &&                  \
    (((__SELECTION__) == TIM_TS_ITR0)    ||          \
     ((__SELECTION__) == TIM_TS_ITR2)    ||          \
     ((__SELECTION__) == TIM_TS_ITR3)    ||          \
     ((__SELECTION__) == TIM_TS_TI1F_ED) ||          \
     ((__SELECTION__) == TIM_TS_TI1FP1)  ||          \
     ((__SELECTION__) == TIM_TS_TI2FP2)  ||          \
     ((__SELECTION__) == TIM_TS_ETRF)    ||          \
     ((__SELECTION__) == TIM_TS_ITR4)    ||          \
     ((__SELECTION__) == TIM_TS_ITR5)    ||          \
     ((__SELECTION__) == TIM_TS_ITR6)    ||          \
     ((__SELECTION__) == TIM_TS_ITR7)    ||          \
     ((__SELECTION__) == TIM_TS_ITR8)    ||          \
     ((__SELECTION__) == TIM_TS_ITR11)))             \
   ||                                        \
   (((INSTANCE) == TIM3) &&                  \
    (((__SELECTION__) == TIM_TS_ITR0)    ||          \
     ((__SELECTION__) == TIM_TS_ITR1)    ||          \
     ((__SELECTION__) == TIM_TS_ITR3)    ||          \
     ((__SELECTION__) == TIM_TS_TI1F_ED) ||          \
     ((__SELECTION__) == TIM_TS_TI1FP1)  ||          \
     ((__SELECTION__) == TIM_TS_TI2FP2)  ||          \
     ((__SELECTION__) == TIM_TS_ETRF)    ||          \
     ((__SELECTION__) == TIM_TS_ITR4)    ||          \
     ((__SELECTION__) == TIM_TS_ITR5)    ||          \
     ((__SELECTION__) == TIM_TS_ITR6)    ||          \
     ((__SELECTION__) == TIM_TS_ITR7)    ||          \
     ((__SELECTION__) == TIM_TS_ITR8)))             \
   ||                                        \
   (((INSTANCE) == TIM4) &&                  \
    (((__SELECTION__) == TIM_TS_ITR0)    ||          \
     ((__SELECTION__) == TIM_TS_ITR1)    ||          \
     ((__SELECTION__) == TIM_TS_ITR2)    ||          \
     ((__SELECTION__) == TIM_TS_TI1F_ED) ||          \
     ((__SELECTION__) == TIM_TS_TI1FP1)  ||          \
     ((__SELECTION__) == TIM_TS_TI2FP2)  ||          \
     ((__SELECTION__) == TIM_TS_ETRF)    ||          \
     ((__SELECTION__) == TIM_TS_ITR4)    ||          \
     ((__SELECTION__) == TIM_TS_ITR5)    ||          \
     ((__SELECTION__) == TIM_TS_ITR6)    ||          \
     ((__SELECTION__) == TIM_TS_ITR7)    ||          \
     ((__SELECTION__) == TIM_TS_ITR8)))             \
   ||                                        \
   (((INSTANCE) == TIM5) &&                  \
    (((__SELECTION__) == TIM_TS_ITR0)    ||          \
     ((__SELECTION__) == TIM_TS_ITR1)    ||          \
     ((__SELECTION__) == TIM_TS_ITR2)    ||          \
     ((__SELECTION__) == TIM_TS_ITR3)    ||          \
     ((__SELECTION__) == TIM_TS_TI1F_ED) ||          \
     ((__SELECTION__) == TIM_TS_TI1FP1)  ||          \
     ((__SELECTION__) == TIM_TS_TI2FP2)  ||          \
     ((__SELECTION__) == TIM_TS_ETRF)    ||          \
     ((__SELECTION__) == TIM_TS_ITR5)    ||          \
     ((__SELECTION__) == TIM_TS_ITR6)    ||          \
     ((__SELECTION__) == TIM_TS_ITR7)    ||          \
     ((__SELECTION__) == TIM_TS_ITR8)))             \
   ||                                        \
   (((INSTANCE) == TIM8) &&                  \
    (((__SELECTION__) == TIM_TS_ITR0)    ||          \
     ((__SELECTION__) == TIM_TS_ITR1)    ||          \
     ((__SELECTION__) == TIM_TS_ITR2)    ||          \
     ((__SELECTION__) == TIM_TS_ITR3)    ||          \
     ((__SELECTION__) == TIM_TS_TI1F_ED) ||          \
     ((__SELECTION__) == TIM_TS_TI1FP1)  ||          \
     ((__SELECTION__) == TIM_TS_TI2FP2)  ||          \
     ((__SELECTION__) == TIM_TS_ETRF)    ||          \
     ((__SELECTION__) == TIM_TS_ITR4)    ||          \
     ((__SELECTION__) == TIM_TS_ITR6)    ||          \
     ((__SELECTION__) == TIM_TS_ITR7)    ||          \
     ((__SELECTION__) == TIM_TS_ITR8)))             \
   ||                                        \
   (((INSTANCE) == TIM15) &&                 \
    (((__SELECTION__) == TIM_TS_ITR0)    ||          \
     ((__SELECTION__) == TIM_TS_ITR1)    ||          \
     ((__SELECTION__) == TIM_TS_ITR2)    ||          \
     ((__SELECTION__) == TIM_TS_ITR3)    ||          \
     ((__SELECTION__) == TIM_TS_TI1F_ED) ||          \
     ((__SELECTION__) == TIM_TS_TI1FP1)  ||          \
     ((__SELECTION__) == TIM_TS_TI2FP2)  ||          \
     ((__SELECTION__) == TIM_TS_ITR4)    ||          \
     ((__SELECTION__) == TIM_TS_ITR5)    ||          \
     ((__SELECTION__) == TIM_TS_ITR7)    ||          \
     ((__SELECTION__) == TIM_TS_ITR8))))

#define IS_TIM_INTERNAL_TRIGGEREVENT_INSTANCE(INSTANCE, __SELECTION__) \
  ((((INSTANCE) == TIM1) &&                  \
    (((__SELECTION__) == TIM_TS_ITR1) ||          \
     ((__SELECTION__) == TIM_TS_ITR2) ||          \
     ((__SELECTION__) == TIM_TS_ITR3) ||          \
     ((__SELECTION__) == TIM_TS_ITR4) ||          \
     ((__SELECTION__) == TIM_TS_ITR5) ||          \
     ((__SELECTION__) == TIM_TS_ITR6) ||          \
     ((__SELECTION__) == TIM_TS_ITR7) ||          \
     ((__SELECTION__) == TIM_TS_ITR8) ||          \
     ((__SELECTION__) == TIM_TS_NONE)))           \
   ||                                        \
   (((INSTANCE) == TIM2) &&                  \
    (((__SELECTION__) == TIM_TS_ITR0) ||          \
     ((__SELECTION__) == TIM_TS_ITR2) ||          \
     ((__SELECTION__) == TIM_TS_ITR3) ||          \
     ((__SELECTION__) == TIM_TS_ITR4) ||          \
     ((__SELECTION__) == TIM_TS_ITR5) ||          \
     ((__SELECTION__) == TIM_TS_ITR6) ||          \
     ((__SELECTION__) == TIM_TS_ITR7) ||          \
     ((__SELECTION__) == TIM_TS_ITR8) ||          \
     ((__SELECTION__) == TIM_TS_ITR11)||          \
     ((__SELECTION__) == TIM_TS_NONE)))           \
   ||                                        \
   (((INSTANCE) == TIM3) &&                  \
    (((__SELECTION__) == TIM_TS_ITR0) ||          \
     ((__SELECTION__) == TIM_TS_ITR1) ||          \
     ((__SELECTION__) == TIM_TS_ITR3) ||          \
     ((__SELECTION__) == TIM_TS_ITR4) ||          \
     ((__SELECTION__) == TIM_TS_ITR5) ||          \
     ((__SELECTION__) == TIM_TS_ITR6) ||          \
     ((__SELECTION__) == TIM_TS_ITR7) ||          \
     ((__SELECTION__) == TIM_TS_ITR8) ||          \
     ((__SELECTION__) == TIM_TS_NONE)))           \
   ||                                        \
   (((INSTANCE) == TIM4) &&                  \
    (((__SELECTION__) == TIM_TS_ITR0) ||          \
     ((__SELECTION__) == TIM_TS_ITR1) ||          \
     ((__SELECTION__) == TIM_TS_ITR2) ||          \
     ((__SELECTION__) == TIM_TS_ITR4) ||          \
     ((__SELECTION__) == TIM_TS_ITR5) ||          \
     ((__SELECTION__) == TIM_TS_ITR6) ||          \
     ((__SELECTION__) == TIM_TS_ITR7) ||          \
     ((__SELECTION__) == TIM_TS_ITR8) ||          \
     ((__SELECTION__) == TIM_TS_NONE)))           \
   ||                                        \
   (((INSTANCE) == TIM5) &&                  \
    (((__SELECTION__) == TIM_TS_ITR0) ||          \
     ((__SELECTION__) == TIM_TS_ITR1) ||          \
     ((__SELECTION__) == TIM_TS_ITR2) ||          \
     ((__SELECTION__) == TIM_TS_ITR3) ||          \
     ((__SELECTION__) == TIM_TS_ITR5) ||          \
     ((__SELECTION__) == TIM_TS_ITR6) ||          \
     ((__SELECTION__) == TIM_TS_ITR7) ||          \
     ((__SELECTION__) == TIM_TS_ITR8) ||          \
     ((__SELECTION__) == TIM_TS_NONE)))           \
   ||                                        \
   (((INSTANCE) == TIM8) &&                  \
    (((__SELECTION__) == TIM_TS_ITR0) ||          \
     ((__SELECTION__) == TIM_TS_ITR1) ||          \
     ((__SELECTION__) == TIM_TS_ITR2) ||          \
     ((__SELECTION__) == TIM_TS_ITR3) ||          \
     ((__SELECTION__) == TIM_TS_ITR4) ||          \
     ((__SELECTION__) == TIM_TS_ITR6) ||          \
     ((__SELECTION__) == TIM_TS_ITR7) ||          \
     ((__SELECTION__) == TIM_TS_ITR8) ||          \
     ((__SELECTION__) == TIM_TS_NONE)))           \
   ||                                        \
   (((INSTANCE) == TIM15) &&                 \
    (((__SELECTION__) == TIM_TS_ITR0) ||          \
     ((__SELECTION__) == TIM_TS_ITR1) ||          \
     ((__SELECTION__) == TIM_TS_ITR2) ||          \
     ((__SELECTION__) == TIM_TS_ITR3) ||          \
     ((__SELECTION__) == TIM_TS_ITR4) ||          \
     ((__SELECTION__) == TIM_TS_ITR5) ||          \
     ((__SELECTION__) == TIM_TS_ITR7) ||          \
     ((__SELECTION__) == TIM_TS_ITR8) ||          \
     ((__SELECTION__) == TIM_TS_NONE))))
#elif defined(TIM20)
#define IS_TIM_CLOCKSOURCE_INSTANCE(INSTANCE, __CLOCK__) \
  ((((INSTANCE) == TIM1) &&                  \
    (((__CLOCK__) == TIM_CLOCKSOURCE_INTERNAL)  ||          \
     ((__CLOCK__) == TIM_CLOCKSOURCE_ETRMODE2)  ||          \
     ((__CLOCK__) == TIM_CLOCKSOURCE_ITR1)      ||          \
     ((__CLOCK__) == TIM_CLOCKSOURCE_ITR2)      ||          \
     ((__CLOCK__) == TIM_CLOCKSOURCE_ITR3)      ||          \
     ((__CLOCK__) == TIM_CLOCKSOURCE_TI1ED)     ||          \
     ((__CLOCK__) == TIM_CLOCKSOURCE_TI1)       ||          \
     ((__CLOCK__) == TIM_CLOCKSOURCE_TI2)       ||          \
     ((__CLOCK__) == TIM_CLOCKSOURCE_ETRMODE1)  ||          \
     ((__CLOCK__) == TIM_CLOCKSOURCE_ITR5)      ||          \
     ((__CLOCK__) == TIM_CLOCKSOURCE_ITR6)      ||          \
     ((__CLOCK__) == TIM_CLOCKSOURCE_ITR7)      ||          \
     ((__CLOCK__) == TIM_CLOCKSOURCE_ITR8)      ||          \
     ((__CLOCK__) == TIM_CLOCKSOURCE_ITR9)))            \
   ||                                        \
   (((INSTANCE) == TIM2) &&                  \
    (((__CLOCK__) == TIM_CLOCKSOURCE_INTERNAL)  ||          \
     ((__CLOCK__) == TIM_CLOCKSOURCE_ETRMODE2)  ||          \
     ((__CLOCK__) == TIM_CLOCKSOURCE_ITR0)      ||          \
     ((__CLOCK__) == TIM_CLOCKSOURCE_ITR2)      ||          \
     ((__CLOCK__) == TIM_CLOCKSOURCE_ITR3)      ||          \
     ((__CLOCK__) == TIM_CLOCKSOURCE_TI1ED)     ||          \
     ((__CLOCK__) == TIM_CLOCKSOURCE_TI1)       ||          \
     ((__CLOCK__) == TIM_CLOCKSOURCE_TI2)       ||          \
     ((__CLOCK__) == TIM_CLOCKSOURCE_ETRMODE1)  ||          \
     ((__CLOCK__) == TIM_CLOCKSOURCE_ITR5)      ||          \
     ((__CLOCK__) == TIM_CLOCKSOURCE_ITR6)      ||          \
     ((__CLOCK__) == TIM_CLOCKSOURCE_ITR7)      ||          \
     ((__CLOCK__) == TIM_CLOCKSOURCE_ITR8)      ||          \
     ((__CLOCK__) == TIM_CLOCKSOURCE_ITR9)      ||          \
     ((__CLOCK__) == TIM_CLOCKSOURCE_ITR11)))            \
   ||                                        \
   (((INSTANCE) == TIM3) &&                  \
    (((__CLOCK__) == TIM_CLOCKSOURCE_INTERNAL)  ||          \
     ((__CLOCK__) == TIM_CLOCKSOURCE_ETRMODE2)  ||          \
     ((__CLOCK__) == TIM_CLOCKSOURCE_ITR0)      ||          \
     ((__CLOCK__) == TIM_CLOCKSOURCE_ITR1)      ||          \
     ((__CLOCK__) == TIM_CLOCKSOURCE_ITR3)      ||          \
     ((__CLOCK__) == TIM_CLOCKSOURCE_TI1ED)     ||          \
     ((__CLOCK__) == TIM_CLOCKSOURCE_TI1)       ||          \
     ((__CLOCK__) == TIM_CLOCKSOURCE_TI2)       ||          \
     ((__CLOCK__) == TIM_CLOCKSOURCE_ETRMODE1)  ||          \
     ((__CLOCK__) == TIM_CLOCKSOURCE_ITR5)      ||          \
     ((__CLOCK__) == TIM_CLOCKSOURCE_ITR6)      ||          \
     ((__CLOCK__) == TIM_CLOCKSOURCE_ITR7)      ||          \
     ((__CLOCK__) == TIM_CLOCKSOURCE_ITR8)      ||          \
     ((__CLOCK__) == TIM_CLOCKSOURCE_ITR9)))            \
   ||                                        \
   (((INSTANCE) == TIM4) &&                  \
    (((__CLOCK__) == TIM_CLOCKSOURCE_INTERNAL)  ||          \
     ((__CLOCK__) == TIM_CLOCKSOURCE_ETRMODE2)  ||          \
     ((__CLOCK__) == TIM_CLOCKSOURCE_ITR0)      ||          \
     ((__CLOCK__) == TIM_CLOCKSOURCE_ITR1)      ||          \
     ((__CLOCK__) == TIM_CLOCKSOURCE_ITR2)      ||          \
     ((__CLOCK__) == TIM_CLOCKSOURCE_TI1ED)     ||          \
     ((__CLOCK__) == TIM_CLOCKSOURCE_TI1)       ||          \
     ((__CLOCK__) == TIM_CLOCKSOURCE_TI2)       ||          \
     ((__CLOCK__) == TIM_CLOCKSOURCE_ETRMODE1)  ||          \
     ((__CLOCK__) == TIM_CLOCKSOURCE_ITR5)      ||          \
     ((__CLOCK__) == TIM_CLOCKSOURCE_ITR6)      ||          \
     ((__CLOCK__) == TIM_CLOCKSOURCE_ITR7)      ||          \
     ((__CLOCK__) == TIM_CLOCKSOURCE_ITR8)      ||          \
     ((__CLOCK__) == TIM_CLOCKSOURCE_ITR9)))            \
   ||                                        \
   (((INSTANCE) == TIM8) &&                  \
    (((__CLOCK__) == TIM_CLOCKSOURCE_INTERNAL)  ||          \
     ((__CLOCK__) == TIM_CLOCKSOURCE_ETRMODE2)  ||          \
     ((__CLOCK__) == TIM_CLOCKSOURCE_ITR0)      ||          \
     ((__CLOCK__) == TIM_CLOCKSOURCE_ITR1)      ||          \
     ((__CLOCK__) == TIM_CLOCKSOURCE_ITR2)      ||          \
     ((__CLOCK__) == TIM_CLOCKSOURCE_ITR3)      ||          \
     ((__CLOCK__) == TIM_CLOCKSOURCE_TI1ED)     ||          \
     ((__CLOCK__) == TIM_CLOCKSOURCE_TI1)       ||          \
     ((__CLOCK__) == TIM_CLOCKSOURCE_TI2)       ||          \
     ((__CLOCK__) == TIM_CLOCKSOURCE_ETRMODE1)  ||          \
     ((__CLOCK__) == TIM_CLOCKSOURCE_ITR6)      ||          \
     ((__CLOCK__) == TIM_CLOCKSOURCE_ITR7)      ||          \
     ((__CLOCK__) == TIM_CLOCKSOURCE_ITR8)      ||          \
     ((__CLOCK__) == TIM_CLOCKSOURCE_ITR9)))            \
   ||                                        \
   (((INSTANCE) == TIM15) &&                 \
    (((__CLOCK__) == TIM_CLOCKSOURCE_INTERNAL)  ||          \
     ((__CLOCK__) == TIM_CLOCKSOURCE_ITR0)      ||          \
     ((__CLOCK__) == TIM_CLOCKSOURCE_ITR1)      ||          \
     ((__CLOCK__) == TIM_CLOCKSOURCE_ITR2)      ||          \
     ((__CLOCK__) == TIM_CLOCKSOURCE_ITR3)      ||          \
     ((__CLOCK__) == TIM_CLOCKSOURCE_TI1ED)     ||          \
     ((__CLOCK__) == TIM_CLOCKSOURCE_TI1)       ||          \
     ((__CLOCK__) == TIM_CLOCKSOURCE_TI2)       ||          \
     ((__CLOCK__) == TIM_CLOCKSOURCE_ITR5)      ||          \
     ((__CLOCK__) == TIM_CLOCKSOURCE_ITR7)      ||          \
     ((__CLOCK__) == TIM_CLOCKSOURCE_ITR8)      ||          \
     ((__CLOCK__) == TIM_CLOCKSOURCE_ITR9)))            \
   ||                                        \
   (((INSTANCE) == TIM20) &&                 \
    (((__CLOCK__) == TIM_CLOCKSOURCE_INTERNAL)  ||          \
     ((__CLOCK__) == TIM_CLOCKSOURCE_ETRMODE2)  ||          \
     ((__CLOCK__) == TIM_CLOCKSOURCE_ITR0)      ||          \
     ((__CLOCK__) == TIM_CLOCKSOURCE_ITR1)      ||          \
     ((__CLOCK__) == TIM_CLOCKSOURCE_ITR2)      ||          \
     ((__CLOCK__) == TIM_CLOCKSOURCE_ITR3)      ||          \
     ((__CLOCK__) == TIM_CLOCKSOURCE_TI1ED)     ||          \
     ((__CLOCK__) == TIM_CLOCKSOURCE_TI1)       ||          \
     ((__CLOCK__) == TIM_CLOCKSOURCE_TI2)       ||          \
     ((__CLOCK__) == TIM_CLOCKSOURCE_ETRMODE1)  ||          \
     ((__CLOCK__) == TIM_CLOCKSOURCE_ITR5)      ||          \
     ((__CLOCK__) == TIM_CLOCKSOURCE_ITR6)      ||          \
     ((__CLOCK__) == TIM_CLOCKSOURCE_ITR7)      ||          \
     ((__CLOCK__) == TIM_CLOCKSOURCE_ITR8))))

#define IS_TIM_TRIGGER_INSTANCE(INSTANCE, __SELECTION__) \
  ((((INSTANCE) == TIM1) &&                  \
    (((__SELECTION__) == TIM_TS_ITR1)    ||          \
     ((__SELECTION__) == TIM_TS_ITR2)    ||          \
     ((__SELECTION__) == TIM_TS_ITR3)    ||          \
     ((__SELECTION__) == TIM_TS_TI1F_ED) ||          \
     ((__SELECTION__) == TIM_TS_TI1FP1)  ||          \
     ((__SELECTION__) == TIM_TS_TI2FP2)  ||          \
     ((__SELECTION__) == TIM_TS_ETRF)    ||          \
     ((__SELECTION__) == TIM_TS_ITR5)    ||          \
     ((__SELECTION__) == TIM_TS_ITR6)    ||          \
     ((__SELECTION__) == TIM_TS_ITR7)    ||          \
     ((__SELECTION__) == TIM_TS_ITR8)    ||          \
     ((__SELECTION__) == TIM_TS_ITR9)))             \
   ||                                        \
   (((INSTANCE) == TIM2) &&                  \
    (((__SELECTION__) == TIM_TS_ITR0)    ||          \
     ((__SELECTION__) == TIM_TS_ITR2)    ||          \
     ((__SELECTION__) == TIM_TS_ITR3)    ||          \
     ((__SELECTION__) == TIM_TS_TI1F_ED) ||          \
     ((__SELECTION__) == TIM_TS_TI1FP1)  ||          \
     ((__SELECTION__) == TIM_TS_TI2FP2)  ||          \
     ((__SELECTION__) == TIM_TS_ETRF)    ||          \
     ((__SELECTION__) == TIM_TS_ITR5)    ||          \
     ((__SELECTION__) == TIM_TS_ITR6)    ||          \
     ((__SELECTION__) == TIM_TS_ITR7)    ||          \
     ((__SELECTION__) == TIM_TS_ITR8)    ||          \
     ((__SELECTION__) == TIM_TS_ITR9)    ||          \
     ((__SELECTION__) == TIM_TS_ITR11)))             \
   ||                                        \
   (((INSTANCE) == TIM3) &&                  \
    (((__SELECTION__) == TIM_TS_ITR0)    ||          \
     ((__SELECTION__) == TIM_TS_ITR1)    ||          \
     ((__SELECTION__) == TIM_TS_ITR3)    ||          \
     ((__SELECTION__) == TIM_TS_TI1F_ED) ||          \
     ((__SELECTION__) == TIM_TS_TI1FP1)  ||          \
     ((__SELECTION__) == TIM_TS_TI2FP2)  ||          \
     ((__SELECTION__) == TIM_TS_ETRF)    ||          \
     ((__SELECTION__) == TIM_TS_ITR5)    ||          \
     ((__SELECTION__) == TIM_TS_ITR6)    ||          \
     ((__SELECTION__) == TIM_TS_ITR7)    ||          \
     ((__SELECTION__) == TIM_TS_ITR8)    ||          \
     ((__SELECTION__) == TIM_TS_ITR9)))             \
   ||                                        \
   (((INSTANCE) == TIM4) &&                  \
    (((__SELECTION__) == TIM_TS_ITR0)    ||          \
     ((__SELECTION__) == TIM_TS_ITR1)    ||          \
     ((__SELECTION__) == TIM_TS_ITR2)    ||          \
     ((__SELECTION__) == TIM_TS_TI1F_ED) ||          \
     ((__SELECTION__) == TIM_TS_TI1FP1)  ||          \
     ((__SELECTION__) == TIM_TS_TI2FP2)  ||          \
     ((__SELECTION__) == TIM_TS_ETRF)    ||          \
     ((__SELECTION__) == TIM_TS_ITR5)    ||          \
     ((__SELECTION__) == TIM_TS_ITR6)    ||          \
     ((__SELECTION__) == TIM_TS_ITR7)    ||          \
     ((__SELECTION__) == TIM_TS_ITR8)    ||          \
     ((__SELECTION__) == TIM_TS_ITR9)))             \
   ||                                       \
   (((INSTANCE) == TIM8) &&                  \
    (((__SELECTION__) == TIM_TS_ITR0)    ||          \
     ((__SELECTION__) == TIM_TS_ITR1)    ||          \
     ((__SELECTION__) == TIM_TS_ITR2)    ||          \
     ((__SELECTION__) == TIM_TS_ITR3)    ||          \
     ((__SELECTION__) == TIM_TS_TI1F_ED) ||          \
     ((__SELECTION__) == TIM_TS_TI1FP1)  ||          \
     ((__SELECTION__) == TIM_TS_TI2FP2)  ||          \
     ((__SELECTION__) == TIM_TS_ETRF)    ||          \
     ((__SELECTION__) == TIM_TS_ITR6)    ||          \
     ((__SELECTION__) == TIM_TS_ITR7)    ||          \
     ((__SELECTION__) == TIM_TS_ITR8)    ||          \
     ((__SELECTION__) == TIM_TS_ITR9)))             \
   ||                                        \
   (((INSTANCE) == TIM15) &&                 \
    (((__SELECTION__) == TIM_TS_ITR0)    ||          \
     ((__SELECTION__) == TIM_TS_ITR1)    ||          \
     ((__SELECTION__) == TIM_TS_ITR2)    ||          \
     ((__SELECTION__) == TIM_TS_ITR3)    ||          \
     ((__SELECTION__) == TIM_TS_TI1F_ED) ||          \
     ((__SELECTION__) == TIM_TS_TI1FP1)  ||          \
     ((__SELECTION__) == TIM_TS_TI2FP2)  ||          \
     ((__SELECTION__) == TIM_TS_ITR5)    ||          \
     ((__SELECTION__) == TIM_TS_ITR7)    ||          \
     ((__SELECTION__) == TIM_TS_ITR8)    ||          \
     ((__SELECTION__) == TIM_TS_ITR9)))             \
   ||                                        \
   (((INSTANCE) == TIM20) &&                 \
    (((__SELECTION__) == TIM_TS_ITR0)    ||          \
     ((__SELECTION__) == TIM_TS_ITR1)    ||          \
     ((__SELECTION__) == TIM_TS_ITR2)    ||          \
     ((__SELECTION__) == TIM_TS_ITR3)    ||          \
     ((__SELECTION__) == TIM_TS_TI1F_ED) ||          \
     ((__SELECTION__) == TIM_TS_TI1FP1)  ||          \
     ((__SELECTION__) == TIM_TS_TI2FP2)  ||          \
     ((__SELECTION__) == TIM_TS_ETRF)    ||          \
     ((__SELECTION__) == TIM_TS_ITR5)    ||          \
     ((__SELECTION__) == TIM_TS_ITR6)    ||          \
     ((__SELECTION__) == TIM_TS_ITR7)    ||          \
     ((__SELECTION__) == TIM_TS_ITR8))))

#define IS_TIM_INTERNAL_TRIGGEREVENT_INSTANCE(INSTANCE, __SELECTION__) \
  ((((INSTANCE) == TIM1) &&                  \
    (((__SELECTION__) == TIM_TS_ITR1) ||          \
     ((__SELECTION__) == TIM_TS_ITR2) ||          \
     ((__SELECTION__) == TIM_TS_ITR3) ||          \
     ((__SELECTION__) == TIM_TS_ITR5) ||          \
     ((__SELECTION__) == TIM_TS_ITR6) ||          \
     ((__SELECTION__) == TIM_TS_ITR7) ||          \
     ((__SELECTION__) == TIM_TS_ITR8) ||          \
     ((__SELECTION__) == TIM_TS_ITR9) ||          \
     ((__SELECTION__) == TIM_TS_NONE)))           \
   ||                                        \
   (((INSTANCE) == TIM2) &&                  \
    (((__SELECTION__) == TIM_TS_ITR0) ||          \
     ((__SELECTION__) == TIM_TS_ITR2) ||          \
     ((__SELECTION__) == TIM_TS_ITR3) ||          \
     ((__SELECTION__) == TIM_TS_ITR5) ||          \
     ((__SELECTION__) == TIM_TS_ITR6) ||          \
     ((__SELECTION__) == TIM_TS_ITR7) ||          \
     ((__SELECTION__) == TIM_TS_ITR8) ||          \
     ((__SELECTION__) == TIM_TS_ITR9) ||          \
     ((__SELECTION__) == TIM_TS_ITR11)||          \
     ((__SELECTION__) == TIM_TS_NONE)))           \
   ||                                        \
   (((INSTANCE) == TIM3) &&                  \
    (((__SELECTION__) == TIM_TS_ITR0) ||          \
     ((__SELECTION__) == TIM_TS_ITR1) ||          \
     ((__SELECTION__) == TIM_TS_ITR3) ||          \
     ((__SELECTION__) == TIM_TS_ITR5) ||          \
     ((__SELECTION__) == TIM_TS_ITR6) ||          \
     ((__SELECTION__) == TIM_TS_ITR7) ||          \
     ((__SELECTION__) == TIM_TS_ITR8) ||          \
     ((__SELECTION__) == TIM_TS_ITR9) ||          \
     ((__SELECTION__) == TIM_TS_NONE)))           \
   ||                                        \
   (((INSTANCE) == TIM4) &&                  \
    (((__SELECTION__) == TIM_TS_ITR0) ||          \
     ((__SELECTION__) == TIM_TS_ITR1) ||          \
     ((__SELECTION__) == TIM_TS_ITR2) ||          \
     ((__SELECTION__) == TIM_TS_ITR5) ||          \
     ((__SELECTION__) == TIM_TS_ITR6) ||          \
     ((__SELECTION__) == TIM_TS_ITR7) ||          \
     ((__SELECTION__) == TIM_TS_ITR8) ||          \
     ((__SELECTION__) == TIM_TS_ITR9) ||          \
     ((__SELECTION__) == TIM_TS_NONE)))           \
   ||                                        \
   (((INSTANCE) == TIM8) &&                  \
    (((__SELECTION__) == TIM_TS_ITR0) ||          \
     ((__SELECTION__) == TIM_TS_ITR1) ||          \
     ((__SELECTION__) == TIM_TS_ITR2) ||          \
     ((__SELECTION__) == TIM_TS_ITR3) ||          \
     ((__SELECTION__) == TIM_TS_ITR6) ||          \
     ((__SELECTION__) == TIM_TS_ITR7) ||          \
     ((__SELECTION__) == TIM_TS_ITR8) ||          \
     ((__SELECTION__) == TIM_TS_ITR9) ||          \
     ((__SELECTION__) == TIM_TS_NONE)))           \
   ||                                        \
   (((INSTANCE) == TIM15) &&                 \
    (((__SELECTION__) == TIM_TS_ITR0) ||          \
     ((__SELECTION__) == TIM_TS_ITR1) ||          \
     ((__SELECTION__) == TIM_TS_ITR2) ||          \
     ((__SELECTION__) == TIM_TS_ITR3) ||          \
     ((__SELECTION__) == TIM_TS_ITR5) ||          \
     ((__SELECTION__) == TIM_TS_ITR7) ||          \
     ((__SELECTION__) == TIM_TS_ITR8) ||          \
     ((__SELECTION__) == TIM_TS_ITR9) ||          \
     ((__SELECTION__) == TIM_TS_NONE)))           \
   ||                                        \
   (((INSTANCE) == TIM20) &&                 \
    (((__SELECTION__) == TIM_TS_ITR0) ||          \
     ((__SELECTION__) == TIM_TS_ITR1) ||          \
     ((__SELECTION__) == TIM_TS_ITR2) ||          \
     ((__SELECTION__) == TIM_TS_ITR3) ||          \
     ((__SELECTION__) == TIM_TS_ITR5) ||          \
     ((__SELECTION__) == TIM_TS_ITR6) ||          \
     ((__SELECTION__) == TIM_TS_ITR7) ||          \
     ((__SELECTION__) == TIM_TS_ITR8) ||          \
     ((__SELECTION__) == TIM_TS_NONE))))
#else
#define IS_TIM_CLOCKSOURCE_INSTANCE(INSTANCE, __CLOCK__) \
  ((((INSTANCE) == TIM1) &&                  \
    (((__CLOCK__) == TIM_CLOCKSOURCE_INTERNAL)  ||          \
     ((__CLOCK__) == TIM_CLOCKSOURCE_ETRMODE2)  ||          \
     ((__CLOCK__) == TIM_CLOCKSOURCE_ITR1)      ||          \
     ((__CLOCK__) == TIM_CLOCKSOURCE_ITR2)      ||          \
     ((__CLOCK__) == TIM_CLOCKSOURCE_ITR3)      ||          \
     ((__CLOCK__) == TIM_CLOCKSOURCE_TI1ED)     ||          \
     ((__CLOCK__) == TIM_CLOCKSOURCE_TI1)       ||          \
     ((__CLOCK__) == TIM_CLOCKSOURCE_TI2)       ||          \
     ((__CLOCK__) == TIM_CLOCKSOURCE_ETRMODE1)  ||          \
     ((__CLOCK__) == TIM_CLOCKSOURCE_ITR5)      ||          \
     ((__CLOCK__) == TIM_CLOCKSOURCE_ITR6)      ||          \
     ((__CLOCK__) == TIM_CLOCKSOURCE_ITR7)      ||          \
     ((__CLOCK__) == TIM_CLOCKSOURCE_ITR8)))            \
   ||                                        \
   (((INSTANCE) == TIM2) &&                  \
    (((__CLOCK__) == TIM_CLOCKSOURCE_INTERNAL)  ||          \
     ((__CLOCK__) == TIM_CLOCKSOURCE_ETRMODE2)  ||          \
     ((__CLOCK__) == TIM_CLOCKSOURCE_ITR0)      ||          \
     ((__CLOCK__) == TIM_CLOCKSOURCE_ITR2)      ||          \
     ((__CLOCK__) == TIM_CLOCKSOURCE_ITR3)      ||          \
     ((__CLOCK__) == TIM_CLOCKSOURCE_TI1ED)     ||          \
     ((__CLOCK__) == TIM_CLOCKSOURCE_TI1)       ||          \
     ((__CLOCK__) == TIM_CLOCKSOURCE_TI2)       ||          \
     ((__CLOCK__) == TIM_CLOCKSOURCE_ETRMODE1)  ||          \
     ((__CLOCK__) == TIM_CLOCKSOURCE_ITR5)      ||          \
     ((__CLOCK__) == TIM_CLOCKSOURCE_ITR6)      ||          \
     ((__CLOCK__) == TIM_CLOCKSOURCE_ITR7)      ||          \
     ((__CLOCK__) == TIM_CLOCKSOURCE_ITR8)      ||          \
     ((__CLOCK__) == TIM_CLOCKSOURCE_ITR11)))            \
   ||                                        \
   (((INSTANCE) == TIM3) &&                  \
    (((__CLOCK__) == TIM_CLOCKSOURCE_INTERNAL)  ||          \
     ((__CLOCK__) == TIM_CLOCKSOURCE_ETRMODE2)  ||          \
     ((__CLOCK__) == TIM_CLOCKSOURCE_ITR0)      ||          \
     ((__CLOCK__) == TIM_CLOCKSOURCE_ITR1)      ||          \
     ((__CLOCK__) == TIM_CLOCKSOURCE_ITR3)      ||          \
     ((__CLOCK__) == TIM_CLOCKSOURCE_TI1ED)     ||          \
     ((__CLOCK__) == TIM_CLOCKSOURCE_TI1)       ||          \
     ((__CLOCK__) == TIM_CLOCKSOURCE_TI2)       ||          \
     ((__CLOCK__) == TIM_CLOCKSOURCE_ETRMODE1)  ||          \
     ((__CLOCK__) == TIM_CLOCKSOURCE_ITR5)      ||          \
     ((__CLOCK__) == TIM_CLOCKSOURCE_ITR6)      ||          \
     ((__CLOCK__) == TIM_CLOCKSOURCE_ITR7)      ||          \
     ((__CLOCK__) == TIM_CLOCKSOURCE_ITR8)))            \
   ||                                        \
   (((INSTANCE) == TIM4) &&                  \
    (((__CLOCK__) == TIM_CLOCKSOURCE_INTERNAL)  ||          \
     ((__CLOCK__) == TIM_CLOCKSOURCE_ETRMODE2)  ||          \
     ((__CLOCK__) == TIM_CLOCKSOURCE_ITR0)      ||          \
     ((__CLOCK__) == TIM_CLOCKSOURCE_ITR1)      ||          \
     ((__CLOCK__) == TIM_CLOCKSOURCE_ITR2)      ||          \
     ((__CLOCK__) == TIM_CLOCKSOURCE_TI1ED)     ||          \
     ((__CLOCK__) == TIM_CLOCKSOURCE_TI1)       ||          \
     ((__CLOCK__) == TIM_CLOCKSOURCE_TI2)       ||          \
     ((__CLOCK__) == TIM_CLOCKSOURCE_ETRMODE1)  ||          \
     ((__CLOCK__) == TIM_CLOCKSOURCE_ITR5)      ||          \
     ((__CLOCK__) == TIM_CLOCKSOURCE_ITR6)      ||          \
     ((__CLOCK__) == TIM_CLOCKSOURCE_ITR7)      ||          \
     ((__CLOCK__) == TIM_CLOCKSOURCE_ITR8)))            \
   ||                                        \
   (((INSTANCE) == TIM8) &&                  \
    (((__CLOCK__) == TIM_CLOCKSOURCE_INTERNAL)  ||          \
     ((__CLOCK__) == TIM_CLOCKSOURCE_ETRMODE2)  ||          \
     ((__CLOCK__) == TIM_CLOCKSOURCE_ITR0)      ||          \
     ((__CLOCK__) == TIM_CLOCKSOURCE_ITR1)      ||          \
     ((__CLOCK__) == TIM_CLOCKSOURCE_ITR2)      ||          \
     ((__CLOCK__) == TIM_CLOCKSOURCE_ITR3)      ||          \
     ((__CLOCK__) == TIM_CLOCKSOURCE_TI1ED)     ||          \
     ((__CLOCK__) == TIM_CLOCKSOURCE_TI1)       ||          \
     ((__CLOCK__) == TIM_CLOCKSOURCE_TI2)       ||          \
     ((__CLOCK__) == TIM_CLOCKSOURCE_ETRMODE1)  ||          \
     ((__CLOCK__) == TIM_CLOCKSOURCE_ITR6)      ||          \
     ((__CLOCK__) == TIM_CLOCKSOURCE_ITR7)      ||          \
     ((__CLOCK__) == TIM_CLOCKSOURCE_ITR8)))            \
   ||                                        \
   (((INSTANCE) == TIM15) &&                 \
    (((__CLOCK__) == TIM_CLOCKSOURCE_INTERNAL)  ||          \
     ((__CLOCK__) == TIM_CLOCKSOURCE_ITR0)      ||          \
     ((__CLOCK__) == TIM_CLOCKSOURCE_ITR1)      ||          \
     ((__CLOCK__) == TIM_CLOCKSOURCE_ITR2)      ||          \
     ((__CLOCK__) == TIM_CLOCKSOURCE_ITR3)      ||          \
     ((__CLOCK__) == TIM_CLOCKSOURCE_TI1ED)     ||          \
     ((__CLOCK__) == TIM_CLOCKSOURCE_TI1)       ||          \
     ((__CLOCK__) == TIM_CLOCKSOURCE_TI2)       ||          \
     ((__CLOCK__) == TIM_CLOCKSOURCE_ITR5)      ||          \
     ((__CLOCK__) == TIM_CLOCKSOURCE_ITR7)      ||          \
     ((__CLOCK__) == TIM_CLOCKSOURCE_ITR8))))

#define IS_TIM_TRIGGER_INSTANCE(INSTANCE, __SELECTION__) \
  ((((INSTANCE) == TIM1) &&                  \
    (((__SELECTION__) == TIM_TS_ITR1)    ||       \
     ((__SELECTION__) == TIM_TS_ITR2)    ||       \
     ((__SELECTION__) == TIM_TS_ITR3)    ||       \
     ((__SELECTION__) == TIM_TS_TI1F_ED) ||       \
     ((__SELECTION__) == TIM_TS_TI1FP1)  ||       \
     ((__SELECTION__) == TIM_TS_TI2FP2)  ||       \
     ((__SELECTION__) == TIM_TS_ETRF)    ||       \
     ((__SELECTION__) == TIM_TS_ITR5)    ||       \
     ((__SELECTION__) == TIM_TS_ITR6)    ||       \
     ((__SELECTION__) == TIM_TS_ITR7)    ||       \
     ((__SELECTION__) == TIM_TS_ITR8)))          \
   ||                                        \
   (((INSTANCE) == TIM2) &&                  \
    (((__SELECTION__) == TIM_TS_ITR0)    ||       \
     ((__SELECTION__) == TIM_TS_ITR2)    ||       \
     ((__SELECTION__) == TIM_TS_ITR3)    ||       \
     ((__SELECTION__) == TIM_TS_TI1F_ED) ||       \
     ((__SELECTION__) == TIM_TS_TI1FP1)  ||       \
     ((__SELECTION__) == TIM_TS_TI2FP2)  ||       \
     ((__SELECTION__) == TIM_TS_ETRF)    ||       \
     ((__SELECTION__) == TIM_TS_ITR5)    ||       \
     ((__SELECTION__) == TIM_TS_ITR6)    ||       \
     ((__SELECTION__) == TIM_TS_ITR7)    ||       \
     ((__SELECTION__) == TIM_TS_ITR8)    ||       \
     ((__SELECTION__) == TIM_TS_ITR11)))          \
   ||                                        \
   (((INSTANCE) == TIM3) &&                  \
    (((__SELECTION__) == TIM_TS_ITR0)    ||       \
     ((__SELECTION__) == TIM_TS_ITR1)    ||       \
     ((__SELECTION__) == TIM_TS_ITR3)    ||       \
     ((__SELECTION__) == TIM_TS_TI1F_ED) ||       \
     ((__SELECTION__) == TIM_TS_TI1FP1)  ||       \
     ((__SELECTION__) == TIM_TS_TI2FP2)  ||       \
     ((__SELECTION__) == TIM_TS_ETRF)    ||       \
     ((__SELECTION__) == TIM_TS_ITR5)    ||       \
     ((__SELECTION__) == TIM_TS_ITR6)    ||       \
     ((__SELECTION__) == TIM_TS_ITR7)    ||       \
     ((__SELECTION__) == TIM_TS_ITR8)))          \
   ||                                        \
   (((INSTANCE) == TIM4) &&                  \
    (((__SELECTION__) == TIM_TS_ITR0)    ||       \
     ((__SELECTION__) == TIM_TS_ITR1)    ||       \
     ((__SELECTION__) == TIM_TS_ITR2)    ||       \
     ((__SELECTION__) == TIM_TS_TI1F_ED) ||       \
     ((__SELECTION__) == TIM_TS_TI1FP1)  ||       \
     ((__SELECTION__) == TIM_TS_TI2FP2)  ||       \
     ((__SELECTION__) == TIM_TS_ETRF)    ||       \
     ((__SELECTION__) == TIM_TS_ITR5)    ||       \
     ((__SELECTION__) == TIM_TS_ITR6)    ||       \
     ((__SELECTION__) == TIM_TS_ITR7)    ||       \
     ((__SELECTION__) == TIM_TS_ITR8)))          \
   ||                                        \
   (((INSTANCE) == TIM8) &&                  \
    (((__SELECTION__) == TIM_TS_ITR0)    ||       \
     ((__SELECTION__) == TIM_TS_ITR1)    ||       \
     ((__SELECTION__) == TIM_TS_ITR2)    ||       \
     ((__SELECTION__) == TIM_TS_ITR3)    ||       \
     ((__SELECTION__) == TIM_TS_TI1F_ED) ||       \
     ((__SELECTION__) == TIM_TS_TI1FP1)  ||       \
     ((__SELECTION__) == TIM_TS_TI2FP2)  ||       \
     ((__SELECTION__) == TIM_TS_ETRF)    ||       \
     ((__SELECTION__) == TIM_TS_ITR6)    ||       \
     ((__SELECTION__) == TIM_TS_ITR7)    ||       \
     ((__SELECTION__) == TIM_TS_ITR8)))          \
   ||                                        \
   (((INSTANCE) == TIM15) &&                 \
    (((__SELECTION__) == TIM_TS_ITR0)    ||       \
     ((__SELECTION__) == TIM_TS_ITR1)    ||       \
     ((__SELECTION__) == TIM_TS_ITR2)    ||       \
     ((__SELECTION__) == TIM_TS_ITR3)    ||       \
     ((__SELECTION__) == TIM_TS_TI1F_ED) ||       \
     ((__SELECTION__) == TIM_TS_TI1FP1)  ||       \
     ((__SELECTION__) == TIM_TS_TI2FP2)  ||       \
     ((__SELECTION__) == TIM_TS_ITR5)    ||       \
     ((__SELECTION__) == TIM_TS_ITR7)    ||       \
     ((__SELECTION__) == TIM_TS_ITR8))))

#define IS_TIM_INTERNAL_TRIGGEREVENT_INSTANCE(INSTANCE, __SELECTION__) \
  ((((INSTANCE) == TIM1) &&                  \
    (((__SELECTION__) == TIM_TS_ITR1) ||          \
     ((__SELECTION__) == TIM_TS_ITR2) ||          \
     ((__SELECTION__) == TIM_TS_ITR3) ||          \
     ((__SELECTION__) == TIM_TS_ITR5) ||          \
     ((__SELECTION__) == TIM_TS_ITR6) ||          \
     ((__SELECTION__) == TIM_TS_ITR7) ||          \
     ((__SELECTION__) == TIM_TS_ITR8) ||          \
     ((__SELECTION__) == TIM_TS_NONE)))           \
   ||                                        \
   (((INSTANCE) == TIM2) &&                  \
    (((__SELECTION__) == TIM_TS_ITR0) ||          \
     ((__SELECTION__) == TIM_TS_ITR2) ||          \
     ((__SELECTION__) == TIM_TS_ITR3) ||          \
     ((__SELECTION__) == TIM_TS_ITR5) ||          \
     ((__SELECTION__) == TIM_TS_ITR6) ||          \
     ((__SELECTION__) == TIM_TS_ITR7) ||          \
     ((__SELECTION__) == TIM_TS_ITR8) ||          \
     ((__SELECTION__) == TIM_TS_ITR11)||          \
     ((__SELECTION__) == TIM_TS_NONE)))           \
   ||                                        \
   (((INSTANCE) == TIM3) &&                  \
    (((__SELECTION__) == TIM_TS_ITR0) ||          \
     ((__SELECTION__) == TIM_TS_ITR1) ||          \
     ((__SELECTION__) == TIM_TS_ITR3) ||          \
     ((__SELECTION__) == TIM_TS_ITR5) ||          \
     ((__SELECTION__) == TIM_TS_ITR6) ||          \
     ((__SELECTION__) == TIM_TS_ITR7) ||          \
     ((__SELECTION__) == TIM_TS_ITR8) ||          \
     ((__SELECTION__) == TIM_TS_NONE)))           \
   ||                                        \
   (((INSTANCE) == TIM4) &&                  \
    (((__SELECTION__) == TIM_TS_ITR0) ||          \
     ((__SELECTION__) == TIM_TS_ITR1) ||          \
     ((__SELECTION__) == TIM_TS_ITR2) ||          \
     ((__SELECTION__) == TIM_TS_ITR5) ||          \
     ((__SELECTION__) == TIM_TS_ITR6) ||          \
     ((__SELECTION__) == TIM_TS_ITR7) ||          \
     ((__SELECTION__) == TIM_TS_ITR8) ||          \
     ((__SELECTION__) == TIM_TS_NONE)))           \
   ||                                        \
   (((INSTANCE) == TIM8) &&                  \
    (((__SELECTION__) == TIM_TS_ITR0) ||          \
     ((__SELECTION__) == TIM_TS_ITR1) ||          \
     ((__SELECTION__) == TIM_TS_ITR2) ||          \
     ((__SELECTION__) == TIM_TS_ITR3) ||          \
     ((__SELECTION__) == TIM_TS_ITR6) ||          \
     ((__SELECTION__) == TIM_TS_ITR7) ||          \
     ((__SELECTION__) == TIM_TS_ITR8) ||          \
     ((__SELECTION__) == TIM_TS_NONE)))           \
   ||                                        \
   (((INSTANCE) == TIM15) &&                 \
    (((__SELECTION__) == TIM_TS_ITR0) ||          \
     ((__SELECTION__) == TIM_TS_ITR1) ||          \
     ((__SELECTION__) == TIM_TS_ITR2) ||          \
     ((__SELECTION__) == TIM_TS_ITR3) ||          \
     ((__SELECTION__) == TIM_TS_ITR5) ||          \
     ((__SELECTION__) == TIM_TS_ITR7) ||          \
     ((__SELECTION__) == TIM_TS_ITR8) ||          \
     ((__SELECTION__) == TIM_TS_NONE))))

#endif /* TIM5 && TIM20 */
#define IS_TIM_OC_CHANNEL_MODE(__MODE__, __CHANNEL__)   \
  (IS_TIM_OC_MODE(__MODE__) \
   && ((((__MODE__) == TIM_OCMODE_DIRECTION_OUTPUT) || ((__MODE__) == TIM_OCMODE_PULSE_ON_COMPARE)) \
       ? (((__CHANNEL__) == TIM_CHANNEL_3) || ((__CHANNEL__) == TIM_CHANNEL_4)) : (1 == 1)))

#define IS_TIM_PULSEONCOMPARE_CHANNEL(__CHANNEL__)  \
  (((__CHANNEL__) == TIM_CHANNEL_3) ||    \
   ((__CHANNEL__) == TIM_CHANNEL_4))

#define IS_TIM_PULSEONCOMPARE_INSTANCE(INSTANCE)  IS_TIM_CC3_INSTANCE(INSTANCE)

#define IS_TIM_PULSEONCOMPARE_WIDTH(__WIDTH__)    ((__WIDTH__) <= 0xFFU)

#define IS_TIM_PULSEONCOMPARE_WIDTHPRESCALER(__PRESCALER__)    ((__PRESCALER__) <= 0x7U)

#define IS_TIM_SLAVE_PRELOAD_SOURCE(__SOURCE__)    (((__SOURCE__) == TIM_SMS_PRELOAD_SOURCE_UPDATE) \
                                                    || ((__SOURCE__) == TIM_SMS_PRELOAD_SOURCE_INDEX))

#define IS_TIM_ENCODERINDEX_POLARITY(__POLARITY__)        (((__POLARITY__) == TIM_ENCODERINDEX_POLARITY_INVERTED)  || \
                                                           ((__POLARITY__) == TIM_ENCODERINDEX_POLARITY_NONINVERTED))

#define IS_TIM_ENCODERINDEX_PRESCALER(__PRESCALER__)      (((__PRESCALER__) == TIM_ENCODERINDEX_PRESCALER_DIV1) || \
                                                           ((__PRESCALER__) == TIM_ENCODERINDEX_PRESCALER_DIV2) || \
                                                           ((__PRESCALER__) == TIM_ENCODERINDEX_PRESCALER_DIV4) || \
                                                           ((__PRESCALER__) == TIM_ENCODERINDEX_PRESCALER_DIV8))

#define IS_TIM_ENCODERINDEX_FILTER(__FILTER__)            ((__FILTER__) <= 0xFUL)

#define IS_TIM_ENCODERINDEX_POSITION(__POSITION__)        (((__POSITION__) == TIM_ENCODERINDEX_POSITION_00) || \
                                                           ((__POSITION__) == TIM_ENCODERINDEX_POSITION_01) || \
                                                           ((__POSITION__) == TIM_ENCODERINDEX_POSITION_10) || \
                                                           ((__POSITION__) == TIM_ENCODERINDEX_POSITION_11) || \
                                                           ((__POSITION__) == TIM_ENCODERINDEX_POSITION_0)  || \
                                                           ((__POSITION__) == TIM_ENCODERINDEX_POSITION_1))

#define IS_TIM_ENCODERINDEX_DIRECTION(__DIRECTION__)      (((__DIRECTION__) == TIM_ENCODERINDEX_DIRECTION_UP_DOWN) || \
                                                           ((__DIRECTION__) == TIM_ENCODERINDEX_DIRECTION_UP)      || \
                                                           ((__DIRECTION__) == TIM_ENCODERINDEX_DIRECTION_DOWN))

/**
  * @}
  */
/* End of private macro ------------------------------------------------------*/

/* Exported functions --------------------------------------------------------*/
/** @addtogroup TIMEx_Exported_Functions TIM Extended Exported Functions
  * @{
  */

/** @addtogroup TIMEx_Exported_Functions_Group1 Extended Timer Hall Sensor functions
  *  @brief    Timer Hall Sensor functions
  * @{
  */
/*  Timer Hall Sensor functions  **********************************************/
HAL_StatusTypeDef HAL_TIMEx_HallSensor_Init(TIM_HandleTypeDef *htim, TIM_HallSensor_InitTypeDef *sConfig);
HAL_StatusTypeDef HAL_TIMEx_HallSensor_DeInit(TIM_HandleTypeDef *htim);

void HAL_TIMEx_HallSensor_MspInit(TIM_HandleTypeDef *htim);
void HAL_TIMEx_HallSensor_MspDeInit(TIM_HandleTypeDef *htim);

/* Blocking mode: Polling */
HAL_StatusTypeDef HAL_TIMEx_HallSensor_Start(TIM_HandleTypeDef *htim);
HAL_StatusTypeDef HAL_TIMEx_HallSensor_Stop(TIM_HandleTypeDef *htim);
/* Non-Blocking mode: Interrupt */
HAL_StatusTypeDef HAL_TIMEx_HallSensor_Start_IT(TIM_HandleTypeDef *htim);
HAL_StatusTypeDef HAL_TIMEx_HallSensor_Stop_IT(TIM_HandleTypeDef *htim);
/* Non-Blocking mode: DMA */
HAL_StatusTypeDef HAL_TIMEx_HallSensor_Start_DMA(TIM_HandleTypeDef *htim, uint32_t *pData, uint16_t Length);
HAL_StatusTypeDef HAL_TIMEx_HallSensor_Stop_DMA(TIM_HandleTypeDef *htim);
/**
  * @}
  */

/** @addtogroup TIMEx_Exported_Functions_Group2 Extended Timer Complementary Output Compare functions
  *  @brief   Timer Complementary Output Compare functions
  * @{
  */
/*  Timer Complementary Output Compare functions  *****************************/
/* Blocking mode: Polling */
HAL_StatusTypeDef HAL_TIMEx_OCN_Start(TIM_HandleTypeDef *htim, uint32_t Channel);
HAL_StatusTypeDef HAL_TIMEx_OCN_Stop(TIM_HandleTypeDef *htim, uint32_t Channel);

/* Non-Blocking mode: Interrupt */
HAL_StatusTypeDef HAL_TIMEx_OCN_Start_IT(TIM_HandleTypeDef *htim, uint32_t Channel);
HAL_StatusTypeDef HAL_TIMEx_OCN_Stop_IT(TIM_HandleTypeDef *htim, uint32_t Channel);

/* Non-Blocking mode: DMA */
HAL_StatusTypeDef HAL_TIMEx_OCN_Start_DMA(TIM_HandleTypeDef *htim, uint32_t Channel, uint32_t *pData, uint16_t Length);
HAL_StatusTypeDef HAL_TIMEx_OCN_Stop_DMA(TIM_HandleTypeDef *htim, uint32_t Channel);
/**
  * @}
  */

/** @addtogroup TIMEx_Exported_Functions_Group3 Extended Timer Complementary PWM functions
  *  @brief    Timer Complementary PWM functions
  * @{
  */
/*  Timer Complementary PWM functions  ****************************************/
/* Blocking mode: Polling */
HAL_StatusTypeDef HAL_TIMEx_PWMN_Start(TIM_HandleTypeDef *htim, uint32_t Channel);
HAL_StatusTypeDef HAL_TIMEx_PWMN_Stop(TIM_HandleTypeDef *htim, uint32_t Channel);

/* Non-Blocking mode: Interrupt */
HAL_StatusTypeDef HAL_TIMEx_PWMN_Start_IT(TIM_HandleTypeDef *htim, uint32_t Channel);
HAL_StatusTypeDef HAL_TIMEx_PWMN_Stop_IT(TIM_HandleTypeDef *htim, uint32_t Channel);
/* Non-Blocking mode: DMA */
HAL_StatusTypeDef HAL_TIMEx_PWMN_Start_DMA(TIM_HandleTypeDef *htim, uint32_t Channel, uint32_t *pData, uint16_t Length);
HAL_StatusTypeDef HAL_TIMEx_PWMN_Stop_DMA(TIM_HandleTypeDef *htim, uint32_t Channel);
/**
  * @}
  */

/** @addtogroup TIMEx_Exported_Functions_Group4 Extended Timer Complementary One Pulse functions
  *  @brief    Timer Complementary One Pulse functions
  * @{
  */
/*  Timer Complementary One Pulse functions  **********************************/
/* Blocking mode: Polling */
HAL_StatusTypeDef HAL_TIMEx_OnePulseN_Start(TIM_HandleTypeDef *htim, uint32_t OutputChannel);
HAL_StatusTypeDef HAL_TIMEx_OnePulseN_Stop(TIM_HandleTypeDef *htim, uint32_t OutputChannel);

/* Non-Blocking mode: Interrupt */
HAL_StatusTypeDef HAL_TIMEx_OnePulseN_Start_IT(TIM_HandleTypeDef *htim, uint32_t OutputChannel);
HAL_StatusTypeDef HAL_TIMEx_OnePulseN_Stop_IT(TIM_HandleTypeDef *htim, uint32_t OutputChannel);
/**
  * @}
  */

/** @addtogroup TIMEx_Exported_Functions_Group5 Extended Peripheral Control functions
  *  @brief    Peripheral Control functions
  * @{
  */
/* Extended Control functions  ************************************************/
HAL_StatusTypeDef HAL_TIMEx_ConfigCommutEvent(TIM_HandleTypeDef *htim, uint32_t  InputTrigger,
                                              uint32_t  CommutationSource);
HAL_StatusTypeDef HAL_TIMEx_ConfigCommutEvent_IT(TIM_HandleTypeDef *htim, uint32_t  InputTrigger,
                                                 uint32_t  CommutationSource);
HAL_StatusTypeDef HAL_TIMEx_ConfigCommutEvent_DMA(TIM_HandleTypeDef *htim, uint32_t  InputTrigger,
                                                  uint32_t  CommutationSource);
HAL_StatusTypeDef HAL_TIMEx_MasterConfigSynchronization(TIM_HandleTypeDef *htim,
                                                        TIM_MasterConfigTypeDef *sMasterConfig);
HAL_StatusTypeDef HAL_TIMEx_ConfigBreakDeadTime(TIM_HandleTypeDef *htim,
                                                TIM_BreakDeadTimeConfigTypeDef *sBreakDeadTimeConfig);
HAL_StatusTypeDef HAL_TIMEx_ConfigBreakInput(TIM_HandleTypeDef *htim, uint32_t BreakInput,
                                             TIMEx_BreakInputConfigTypeDef *sBreakInputConfig);
HAL_StatusTypeDef HAL_TIMEx_GroupChannel5(TIM_HandleTypeDef *htim, uint32_t Channels);
HAL_StatusTypeDef HAL_TIMEx_RemapConfig(TIM_HandleTypeDef *htim, uint32_t Remap);
HAL_StatusTypeDef  HAL_TIMEx_TISelection(TIM_HandleTypeDef *htim, uint32_t TISelection, uint32_t Channel);

HAL_StatusTypeDef HAL_TIMEx_DisarmBreakInput(TIM_HandleTypeDef *htim, uint32_t BreakInput);
HAL_StatusTypeDef HAL_TIMEx_ReArmBreakInput(TIM_HandleTypeDef *htim, uint32_t BreakInput);
HAL_StatusTypeDef HAL_TIMEx_DitheringEnable(TIM_HandleTypeDef *htim);
HAL_StatusTypeDef HAL_TIMEx_DitheringDisable(TIM_HandleTypeDef *htim);
HAL_StatusTypeDef HAL_TIMEx_OC_ConfigPulseOnCompare(TIM_HandleTypeDef *htim, uint32_t PulseWidthPrescaler,
                                                    uint32_t PulseWidth);
HAL_StatusTypeDef HAL_TIMEx_ConfigSlaveModePreload(TIM_HandleTypeDef *htim, uint32_t Source);
HAL_StatusTypeDef HAL_TIMEx_EnableSlaveModePreload(TIM_HandleTypeDef *htim);
HAL_StatusTypeDef HAL_TIMEx_DisableSlaveModePreload(TIM_HandleTypeDef *htim);
HAL_StatusTypeDef HAL_TIMEx_EnableDeadTimePreload(TIM_HandleTypeDef *htim);
HAL_StatusTypeDef HAL_TIMEx_DisableDeadTimePreload(TIM_HandleTypeDef *htim);
HAL_StatusTypeDef HAL_TIMEx_ConfigDeadTime(TIM_HandleTypeDef *htim, uint32_t Deadtime);
HAL_StatusTypeDef HAL_TIMEx_ConfigAsymmetricalDeadTime(TIM_HandleTypeDef *htim, uint32_t FallingDeadtime);
HAL_StatusTypeDef HAL_TIMEx_EnableAsymmetricalDeadTime(TIM_HandleTypeDef *htim);
HAL_StatusTypeDef HAL_TIMEx_DisableAsymmetricalDeadTime(TIM_HandleTypeDef *htim);
HAL_StatusTypeDef HAL_TIMEx_ConfigEncoderIndex(TIM_HandleTypeDef *htim,
                                               TIMEx_EncoderIndexConfigTypeDef *sEncoderIndexConfig);
HAL_StatusTypeDef HAL_TIMEx_EnableEncoderIndex(TIM_HandleTypeDef *htim);
HAL_StatusTypeDef HAL_TIMEx_DisableEncoderIndex(TIM_HandleTypeDef *htim);
HAL_StatusTypeDef HAL_TIMEx_EnableEncoderFirstIndex(TIM_HandleTypeDef *htim);
HAL_StatusTypeDef HAL_TIMEx_DisableEncoderFirstIndex(TIM_HandleTypeDef *htim);
/**
  * @}
  */

/** @addtogroup TIMEx_Exported_Functions_Group6 Extended Callbacks functions
  * @brief    Extended Callbacks functions
  * @{
  */
/* Extended Callback **********************************************************/
void HAL_TIMEx_CommutCallback(TIM_HandleTypeDef *htim);
void HAL_TIMEx_CommutHalfCpltCallback(TIM_HandleTypeDef *htim);
void HAL_TIMEx_BreakCallback(TIM_HandleTypeDef *htim);
void HAL_TIMEx_Break2Callback(TIM_HandleTypeDef *htim);
void HAL_TIMEx_EncoderIndexCallback(TIM_HandleTypeDef *htim);
void HAL_TIMEx_DirectionChangeCallback(TIM_HandleTypeDef *htim);
void HAL_TIMEx_IndexErrorCallback(TIM_HandleTypeDef *htim);
void HAL_TIMEx_TransitionErrorCallback(TIM_HandleTypeDef *htim);
/**
  * @}
  */

/** @addtogroup TIMEx_Exported_Functions_Group7 Extended Peripheral State functions
  * @brief    Extended Peripheral State functions
  * @{
  */
/* Extended Peripheral State functions  ***************************************/
HAL_TIM_StateTypeDef HAL_TIMEx_HallSensor_GetState(TIM_HandleTypeDef *htim);
HAL_TIM_ChannelStateTypeDef HAL_TIMEx_GetChannelNState(TIM_HandleTypeDef *htim,  uint32_t ChannelN);
/**
  * @}
  */

/**
  * @}
  */
/* End of exported functions -------------------------------------------------*/

/* Private functions----------------------------------------------------------*/
/** @addtogroup TIMEx_Private_Functions TIMEx Private Functions
  * @{
  */
void TIMEx_DMACommutationCplt(DMA_HandleTypeDef *hdma);
void TIMEx_DMACommutationHalfCplt(DMA_HandleTypeDef *hdma);
/**
  * @}
  */
/* End of private functions --------------------------------------------------*/

/**
  * @}
  */

/**
  * @}
  */

#ifdef __cplusplus
}
#endif


#endif /* STM32G4xx_HAL_TIM_EX_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
