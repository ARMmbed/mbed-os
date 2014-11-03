/**
  ******************************************************************************
  * @file    stm32l0xx_hal_lcd.h
  * @author  MCD Application Team
  * @version V1.1.0
  * @date    18-June-2014
  * @brief   Header file of LCD Controller HAL module.
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
#ifndef __STM32L0xx_HAL_LCD_H
#define __STM32L0xx_HAL_LCD_H

#ifdef __cplusplus
 extern "C" {
#endif

#if !defined (STM32L051xx) && !defined (STM32L052xx) && !defined (STM32L062xx) && !defined (STM32L061xx) 
   
/* Includes ------------------------------------------------------------------*/
#include "stm32l0xx_hal_def.h"

/** @addtogroup STM32L0xx_HAL_Driver
  * @{
  */

/** @addtogroup LCD
  * @{
  */ 

/* Exported types ------------------------------------------------------------*/

/** 
  * @brief LCD Init structure definition
  */

typedef struct
{
  uint32_t Prescaler;       /*!< Configures the LCD Prescaler. 
                                 This parameter can be one value of @ref LCD_Prescaler */
  uint32_t Divider;         /*!< Configures the LCD Divider.
                                 This parameter can be one value of @ref LCD_Divider */
  uint32_t Duty;            /*!< Configures the LCD Duty.
                                 This parameter can be one value of @ref LCD_Duty */
  uint32_t Bias;            /*!< Configures the LCD Bias.
                                 This parameter can be one value of @ref LCD_Bias */ 
  uint32_t VoltageSource;   /*!< Selects the LCD Voltage source.
                                 This parameter can be one value of @ref LCD_Voltage_Source */  
  uint32_t Contrast;        /*!< Configures the LCD Contrast.
                                 This parameter can be one value of @ref LCD_Contrast */
  uint32_t DeadTime;        /*!< Configures the LCD Dead Time.
                                 This parameter can be one value of @ref LCD_DeadTime */
  uint32_t PulseOnDuration; /*!< Configures the LCD Pulse On Duration.
                                 This parameter can be one value of @ref LCD_PulseOnDuration */
  uint32_t HighDrive;       /*!< Enable or disable the low resistance divider.
                                 This parameter can be set to ENABLE or DISABLE. */ 
  uint32_t BlinkMode;       /*!< Configures the LCD Blink Mode.
                                 This parameter can be one value of @ref LCD_BlinkMode */
  uint32_t BlinkFrequency;  /*!< Configures the LCD Blink frequency.
                                 This parameter can be one value of @ref LCD_BlinkFrequency */
}LCD_InitTypeDef;

/** 
  * @brief HAL LCD State structures definition
  */ 
typedef enum
{
  HAL_LCD_STATE_RESET             = 0x00,    /*!< Peripheral is not yet Initialized */
  HAL_LCD_STATE_READY             = 0x01,    /*!< Peripheral Initialized and ready for use */
  HAL_LCD_STATE_BUSY              = 0x02,    /*!< an internal process is ongoing */
  HAL_LCD_STATE_TIMEOUT           = 0x03,    /*!< Timeout state */
  HAL_LCD_STATE_ERROR             = 0x04     /*!< Error */
}HAL_LCD_StateTypeDef;

/** 
  * @brief  HAL LCD Error Code structure definition
  */ 
typedef enum
{
  HAL_LCD_ERROR_NONE      = 0x00,    /*!< No error */
  HAL_LCD_ERROR_FCRSF     = 0x01,    /*!< Synchro flag timeout error */
  HAL_LCD_ERROR_UDR       = 0x02,    /*!< Update display request flag timeout error */
  HAL_LCD_ERROR_UDD       = 0x04,    /*!< Update display done flag timeout error */
  HAL_LCD_ERROR_ENS       = 0x08,    /*!< LCD enabled status flag timeout error */
  HAL_LCD_ERROR_RDY       = 0x10     /*!< LCD Booster ready timeout error */
}HAL_LCD_ErrorTypeDef;

/** 
  * @brief  UART handle Structure definition
  */  
typedef struct
{
  LCD_TypeDef            *Instance;          /* LCD registers base address */
  
  LCD_InitTypeDef         Init;              /* LCD communication parameters */

  HAL_LockTypeDef         Lock;              /* Locking object */

  __IO HAL_LCD_StateTypeDef    State;        /* LCD communication state */
  
  __IO HAL_LCD_ErrorTypeDef    ErrorCode;         /* LCD Error code */
  
}LCD_HandleTypeDef;

/* Exported constants --------------------------------------------------------*/

/** @defgroup LCD_Exported_Constants
  * @{
  */

#define IS_LCD_ALL_INSTANCE(INSTANCE) ((INSTANCE) == LCD)

/** @defgroup LCD_Prescaler
  * @{
  */

#define LCD_PRESCALER_1        ((uint32_t)0x00000000)  /*!< CLKPS = LCDCLK        */
#define LCD_PRESCALER_2        ((uint32_t)0x00400000)  /*!< CLKPS = LCDCLK/2      */
#define LCD_PRESCALER_4        ((uint32_t)0x00800000)  /*!< CLKPS = LCDCLK/4      */
#define LCD_PRESCALER_8        ((uint32_t)0x00C00000)  /*!< CLKPS = LCDCLK/8      */
#define LCD_PRESCALER_16       ((uint32_t)0x01000000)  /*!< CLKPS = LCDCLK/16     */
#define LCD_PRESCALER_32       ((uint32_t)0x01400000)  /*!< CLKPS = LCDCLK/32     */
#define LCD_PRESCALER_64       ((uint32_t)0x01800000)  /*!< CLKPS = LCDCLK/64     */
#define LCD_PRESCALER_128      ((uint32_t)0x01C00000)  /*!< CLKPS = LCDCLK/128    */
#define LCD_PRESCALER_256      ((uint32_t)0x02000000)  /*!< CLKPS = LCDCLK/256    */
#define LCD_PRESCALER_512      ((uint32_t)0x02400000)  /*!< CLKPS = LCDCLK/512    */
#define LCD_PRESCALER_1024     ((uint32_t)0x02800000)  /*!< CLKPS = LCDCLK/1024   */
#define LCD_PRESCALER_2048     ((uint32_t)0x02C00000)  /*!< CLKPS = LCDCLK/2048   */
#define LCD_PRESCALER_4096     ((uint32_t)0x03000000)  /*!< CLKPS = LCDCLK/4096   */
#define LCD_PRESCALER_8192     ((uint32_t)0x03400000)  /*!< CLKPS = LCDCLK/8192   */
#define LCD_PRESCALER_16384    ((uint32_t)0x03800000)  /*!< CLKPS = LCDCLK/16384  */
#define LCD_PRESCALER_32768    ((uint32_t)0x03C00000)  /*!< CLKPS = LCDCLK/32768  */

#define IS_LCD_PRESCALER(PRESCALER)    (((PRESCALER) == LCD_PRESCALER_1) || \
                                        ((PRESCALER) == LCD_PRESCALER_2) || \
                                        ((PRESCALER) == LCD_PRESCALER_4) || \
                                        ((PRESCALER) == LCD_PRESCALER_8) || \
                                        ((PRESCALER) == LCD_PRESCALER_16) || \
                                        ((PRESCALER) == LCD_PRESCALER_32) || \
                                        ((PRESCALER) == LCD_PRESCALER_64) || \
                                        ((PRESCALER) == LCD_PRESCALER_128) || \
                                        ((PRESCALER) == LCD_PRESCALER_256) || \
                                        ((PRESCALER) == LCD_PRESCALER_512) || \
                                        ((PRESCALER) == LCD_PRESCALER_1024) || \
                                        ((PRESCALER) == LCD_PRESCALER_2048) || \
                                        ((PRESCALER) == LCD_PRESCALER_4096) || \
                                        ((PRESCALER) == LCD_PRESCALER_8192) || \
                                        ((PRESCALER) == LCD_PRESCALER_16384) || \
                                        ((PRESCALER) == LCD_PRESCALER_32768))

/**
  * @}
  */
  
/** @defgroup LCD_Divider
  * @{
  */

#define LCD_DIVIDER_16    ((uint32_t)0x00000000)  /*!< LCD frequency = CLKPS/16 */
#define LCD_DIVIDER_17    ((uint32_t)0x00040000)  /*!< LCD frequency = CLKPS/17 */
#define LCD_DIVIDER_18    ((uint32_t)0x00080000)  /*!< LCD frequency = CLKPS/18 */
#define LCD_DIVIDER_19    ((uint32_t)0x000C0000)  /*!< LCD frequency = CLKPS/19 */
#define LCD_DIVIDER_20    ((uint32_t)0x00100000)  /*!< LCD frequency = CLKPS/20 */
#define LCD_DIVIDER_21    ((uint32_t)0x00140000)  /*!< LCD frequency = CLKPS/21 */
#define LCD_DIVIDER_22    ((uint32_t)0x00180000)  /*!< LCD frequency = CLKPS/22 */
#define LCD_DIVIDER_23    ((uint32_t)0x001C0000)  /*!< LCD frequency = CLKPS/23 */
#define LCD_DIVIDER_24    ((uint32_t)0x00200000)  /*!< LCD frequency = CLKPS/24 */
#define LCD_DIVIDER_25    ((uint32_t)0x00240000)  /*!< LCD frequency = CLKPS/25 */
#define LCD_DIVIDER_26    ((uint32_t)0x00280000)  /*!< LCD frequency = CLKPS/26 */
#define LCD_DIVIDER_27    ((uint32_t)0x002C0000)  /*!< LCD frequency = CLKPS/27 */
#define LCD_DIVIDER_28    ((uint32_t)0x00300000)  /*!< LCD frequency = CLKPS/28 */
#define LCD_DIVIDER_29    ((uint32_t)0x00340000)  /*!< LCD frequency = CLKPS/29 */
#define LCD_DIVIDER_30    ((uint32_t)0x00380000)  /*!< LCD frequency = CLKPS/30 */
#define LCD_DIVIDER_31    ((uint32_t)0x003C0000)  /*!< LCD frequency = CLKPS/31 */

#define IS_LCD_DIVIDER(DIVIDER)    (((DIVIDER) == LCD_DIVIDER_16) || \
                                    ((DIVIDER) == LCD_DIVIDER_17) || \
                                    ((DIVIDER) == LCD_DIVIDER_18) || \
                                    ((DIVIDER) == LCD_DIVIDER_19) || \
                                    ((DIVIDER) == LCD_DIVIDER_20) || \
                                    ((DIVIDER) == LCD_DIVIDER_21) || \
                                    ((DIVIDER) == LCD_DIVIDER_22) || \
                                    ((DIVIDER) == LCD_DIVIDER_23) || \
                                    ((DIVIDER) == LCD_DIVIDER_24) || \
                                    ((DIVIDER) == LCD_DIVIDER_25) || \
                                    ((DIVIDER) == LCD_DIVIDER_26) || \
                                    ((DIVIDER) == LCD_DIVIDER_27) || \
                                    ((DIVIDER) == LCD_DIVIDER_28) || \
                                    ((DIVIDER) == LCD_DIVIDER_29) || \
                                    ((DIVIDER) == LCD_DIVIDER_30) || \
                                    ((DIVIDER) == LCD_DIVIDER_31))

/**
  * @}
  */


/** @defgroup LCD_Duty
  * @{
  */
  
#define LCD_DUTY_STATIC                 ((uint32_t)0x00000000) /*!< Static duty */
#define LCD_DUTY_1_2                    ((uint32_t)0x00000004) /*!< 1/2 duty    */
#define LCD_DUTY_1_3                    ((uint32_t)0x00000008) /*!< 1/3 duty    */
#define LCD_DUTY_1_4                    ((uint32_t)0x0000000C) /*!< 1/4 duty    */
#define LCD_DUTY_1_8                    ((uint32_t)0x00000010) /*!< 1/4 duty    */

#define IS_LCD_DUTY(DUTY) (((DUTY) == LCD_DUTY_STATIC) || \
                           ((DUTY) == LCD_DUTY_1_2) || \
                           ((DUTY) == LCD_DUTY_1_3) || \
                           ((DUTY) == LCD_DUTY_1_4) || \
                           ((DUTY) == LCD_DUTY_1_8))

/**
  * @}
  */ 
  

/** @defgroup LCD_Bias
  * @{
  */
  
#define LCD_BIAS_1_4                    ((uint32_t)0x00000000)  /*!< 1/4 Bias */
#define LCD_BIAS_1_2                    LCD_CR_BIAS_0           /*!< 1/2 Bias */
#define LCD_BIAS_1_3                    LCD_CR_BIAS_1           /*!< 1/3 Bias */

#define IS_LCD_BIAS(BIAS) (((BIAS) == LCD_BIAS_1_4) || \
                           ((BIAS) == LCD_BIAS_1_2) || \
                           ((BIAS) == LCD_BIAS_1_3))
/**
  * @}
  */ 
    
/** @defgroup LCD_Voltage_Source
  * @{
  */
  
#define LCD_VOLTAGESOURCE_INTERNAL      ((uint32_t)0x00000000)  /*!< Internal voltage source for the LCD */
#define LCD_VOLTAGESOURCE_EXTERNAL      LCD_CR_VSEL             /*!< External voltage source for the LCD */

#define IS_LCD_VOLTAGE_SOURCE(SOURCE) (((SOURCE) == LCD_VOLTAGESOURCE_INTERNAL) || \
                                       ((SOURCE) == LCD_VOLTAGESOURCE_EXTERNAL))

/**
  * @}
  */  

/** @defgroup LCD_Interrupts
  * @{
  */
#define LCD_IT_SOF                      LCD_FCR_SOFIE
#define LCD_IT_UDD                      LCD_FCR_UDDIE

#define IS_LCD_IT(IT) ((((IT) & (uint32_t)0xFFFFFFF5) == 0x00) && ((IT) != 0x00))

#define IS_LCD_GET_IT(IT) (((IT) == LCD_IT_SOF) || ((IT) == LCD_IT_UDD))
 
/**
  * @}
  */

/** @defgroup LCD_PulseOnDuration
  * @{
  */

#define LCD_PULSEONDURATION_0           ((uint32_t)0x00000000) /*!< Pulse ON duration = 0 pulse   */
#define LCD_PULSEONDURATION_1           ((uint32_t)0x00000010) /*!< Pulse ON duration = 1/CK_PS  */
#define LCD_PULSEONDURATION_2           ((uint32_t)0x00000020) /*!< Pulse ON duration = 2/CK_PS  */
#define LCD_PULSEONDURATION_3           ((uint32_t)0x00000030) /*!< Pulse ON duration = 3/CK_PS  */
#define LCD_PULSEONDURATION_4           ((uint32_t)0x00000040) /*!< Pulse ON duration = 4/CK_PS  */
#define LCD_PULSEONDURATION_5           ((uint32_t)0x00000050) /*!< Pulse ON duration = 5/CK_PS  */
#define LCD_PULSEONDURATION_6           ((uint32_t)0x00000060) /*!< Pulse ON duration = 6/CK_PS  */
#define LCD_PULSEONDURATION_7           ((uint32_t)0x00000070) /*!< Pulse ON duration = 7/CK_PS  */

#define IS_LCD_PULSE_ON_DURATION(DURATION) (((DURATION) == LCD_PULSEONDURATION_0) || \
                                            ((DURATION) == LCD_PULSEONDURATION_1) || \
                                            ((DURATION) == LCD_PULSEONDURATION_2) || \
                                            ((DURATION) == LCD_PULSEONDURATION_3) || \
                                            ((DURATION) == LCD_PULSEONDURATION_4) || \
                                            ((DURATION) == LCD_PULSEONDURATION_5) || \
                                            ((DURATION) == LCD_PULSEONDURATION_6) || \
                                            ((DURATION) == LCD_PULSEONDURATION_7))
/**
  * @}
  */


/** @defgroup LCD_DeadTime
  * @{
  */

#define LCD_DEADTIME_0                  ((uint32_t)0x00000000) /*!< No dead Time  */
#define LCD_DEADTIME_1                  ((uint32_t)0x00000080) /*!< One Phase between different couple of Frame   */
#define LCD_DEADTIME_2                  ((uint32_t)0x00000100) /*!< Two Phase between different couple of Frame   */
#define LCD_DEADTIME_3                  ((uint32_t)0x00000180) /*!< Three Phase between different couple of Frame */
#define LCD_DEADTIME_4                  ((uint32_t)0x00000200) /*!< Four Phase between different couple of Frame  */
#define LCD_DEADTIME_5                  ((uint32_t)0x00000280) /*!< Five Phase between different couple of Frame  */
#define LCD_DEADTIME_6                  ((uint32_t)0x00000300) /*!< Six Phase between different couple of Frame   */
#define LCD_DEADTIME_7                  ((uint32_t)0x00000380) /*!< Seven Phase between different couple of Frame */

#define IS_LCD_DEAD_TIME(TIME) (((TIME) == LCD_DEADTIME_0) || \
                                ((TIME) == LCD_DEADTIME_1) || \
                                ((TIME) == LCD_DEADTIME_2) || \
                                ((TIME) == LCD_DEADTIME_3) || \
                                ((TIME) == LCD_DEADTIME_4) || \
                                ((TIME) == LCD_DEADTIME_5) || \
                                ((TIME) == LCD_DEADTIME_6) || \
                                ((TIME) == LCD_DEADTIME_7))
/**
  * @}
  */

/** @defgroup LCD_BlinkMode
  * @{
  */

#define LCD_BLINKMODE_OFF               ((uint32_t)0x00000000) /*!< Blink disabled            */
#define LCD_BLINKMODE_SEG0_COM0         ((uint32_t)0x00010000) /*!< Blink enabled on SEG[0], COM[0] (1 pixel)   */
#define LCD_BLINKMODE_SEG0_ALLCOM       ((uint32_t)0x00020000) /*!< Blink enabled on SEG[0], all COM (up to 
                                                                    8 pixels according to the programmed duty)  */
#define LCD_BLINKMODE_ALLSEG_ALLCOM     ((uint32_t)0x00030000) /*!< Blink enabled on all SEG and all COM (all pixels)  */

#define IS_LCD_BLINK_MODE(MODE) (((MODE) == LCD_BLINKMODE_OFF) || \
                                 ((MODE) == LCD_BLINKMODE_SEG0_COM0) || \
                                 ((MODE) == LCD_BLINKMODE_SEG0_ALLCOM) || \
                                 ((MODE) == LCD_BLINKMODE_ALLSEG_ALLCOM))
/**
  * @}
  */    

/** @defgroup LCD_BlinkFrequency
  * @{
  */

#define LCD_BLINKFREQUENCY_DIV8         ((uint32_t)0x00000000) /*!< The Blink frequency = fLCD/8    */
#define LCD_BLINKFREQUENCY_DIV16        ((uint32_t)0x00002000) /*!< The Blink frequency = fLCD/16   */
#define LCD_BLINKFREQUENCY_DIV32        ((uint32_t)0x00004000) /*!< The Blink frequency = fLCD/32   */
#define LCD_BLINKFREQUENCY_DIV64        ((uint32_t)0x00006000) /*!< The Blink frequency = fLCD/64   */
#define LCD_BLINKFREQUENCY_DIV128       ((uint32_t)0x00008000) /*!< The Blink frequency = fLCD/128  */
#define LCD_BLINKFREQUENCY_DIV256       ((uint32_t)0x0000A000) /*!< The Blink frequency = fLCD/256  */
#define LCD_BLINKFREQUENCY_DIV512       ((uint32_t)0x0000C000) /*!< The Blink frequency = fLCD/512  */
#define LCD_BLINKFREQUENCY_DIV1024      ((uint32_t)0x0000E000) /*!< The Blink frequency = fLCD/1024 */

#define IS_LCD_BLINK_FREQUENCY(FREQUENCY) (((FREQUENCY) == LCD_BLINKFREQUENCY_DIV8) || \
                                           ((FREQUENCY) == LCD_BLINKFREQUENCY_DIV16) || \
                                           ((FREQUENCY) == LCD_BLINKFREQUENCY_DIV32) || \
                                           ((FREQUENCY) == LCD_BLINKFREQUENCY_DIV64) || \
                                           ((FREQUENCY) == LCD_BLINKFREQUENCY_DIV128) || \
                                           ((FREQUENCY) == LCD_BLINKFREQUENCY_DIV256) || \
                                           ((FREQUENCY) == LCD_BLINKFREQUENCY_DIV512) || \
                                           ((FREQUENCY) == LCD_BLINKFREQUENCY_DIV1024))
/**
  * @}
  */

/** @defgroup LCD_Contrast
  * @{
  */

#define LCD_CONTRASTLEVEL_0               ((uint32_t)0x00000000) /*!< Maximum Voltage = 2.60V    */
#define LCD_CONTRASTLEVEL_1               ((uint32_t)0x00000400) /*!< Maximum Voltage = 2.73V    */
#define LCD_CONTRASTLEVEL_2               ((uint32_t)0x00000800) /*!< Maximum Voltage = 2.86V    */
#define LCD_CONTRASTLEVEL_3               ((uint32_t)0x00000C00) /*!< Maximum Voltage = 2.99V    */
#define LCD_CONTRASTLEVEL_4               ((uint32_t)0x00001000) /*!< Maximum Voltage = 3.12V    */
#define LCD_CONTRASTLEVEL_5               ((uint32_t)0x00001400) /*!< Maximum Voltage = 3.25V    */
#define LCD_CONTRASTLEVEL_6               ((uint32_t)0x00001800) /*!< Maximum Voltage = 3.38V    */
#define LCD_CONTRASTLEVEL_7               ((uint32_t)0x00001C00) /*!< Maximum Voltage = 3.51V    */

#define IS_LCD_CONTRAST(CONTRAST) (((CONTRAST) == LCD_CONTRASTLEVEL_0) || \
                                   ((CONTRAST) == LCD_CONTRASTLEVEL_1) || \
                                   ((CONTRAST) == LCD_CONTRASTLEVEL_2) || \
                                   ((CONTRAST) == LCD_CONTRASTLEVEL_3) || \
                                   ((CONTRAST) == LCD_CONTRASTLEVEL_4) || \
                                   ((CONTRAST) == LCD_CONTRASTLEVEL_5) || \
                                   ((CONTRAST) == LCD_CONTRASTLEVEL_6) || \
                                   ((CONTRAST) == LCD_CONTRASTLEVEL_7))
/**
  * @}
  */
      
/** @defgroup LCD_Flag
  * @{
  */

#define LCD_FLAG_ENS                    LCD_SR_ENS
#define LCD_FLAG_SOF                    LCD_SR_SOF
#define LCD_FLAG_UDR                    LCD_SR_UDR
#define LCD_FLAG_UDD                    LCD_SR_UDD
#define LCD_FLAG_RDY                    LCD_SR_RDY
#define LCD_FLAG_FCRSF                  LCD_SR_FCRSR

#define IS_LCD_GET_FLAG(FLAG) (((FLAG) == LCD_FLAG_ENS) || ((FLAG) == LCD_FLAG_SOF) || \
                               ((FLAG) == LCD_FLAG_UDR) || ((FLAG) == LCD_FLAG_UDD) || \
                               ((FLAG) == LCD_FLAG_RDY) || ((FLAG) == LCD_FLAG_FCRSF))

#define IS_LCD_CLEAR_FLAG(FLAG) ((((FLAG) & (uint32_t)0xFFFFFFF5) == 0x00) && ((FLAG) != 0x00))
/**
  * @}
  */   

/** @defgroup LCD_RAMRegister
  * @{
  */

#define LCD_RAM_REGISTER0               ((uint32_t)0x00000000) /*!< LCD RAM Register 0  */
#define LCD_RAM_REGISTER1               ((uint32_t)0x00000001) /*!< LCD RAM Register 1  */
#define LCD_RAM_REGISTER2               ((uint32_t)0x00000002) /*!< LCD RAM Register 2  */
#define LCD_RAM_REGISTER3               ((uint32_t)0x00000003) /*!< LCD RAM Register 3  */
#define LCD_RAM_REGISTER4               ((uint32_t)0x00000004) /*!< LCD RAM Register 4  */
#define LCD_RAM_REGISTER5               ((uint32_t)0x00000005) /*!< LCD RAM Register 5  */
#define LCD_RAM_REGISTER6               ((uint32_t)0x00000006) /*!< LCD RAM Register 6  */
#define LCD_RAM_REGISTER7               ((uint32_t)0x00000007) /*!< LCD RAM Register 7  */
#define LCD_RAM_REGISTER8               ((uint32_t)0x00000008) /*!< LCD RAM Register 8  */
#define LCD_RAM_REGISTER9               ((uint32_t)0x00000009) /*!< LCD RAM Register 9  */
#define LCD_RAM_REGISTER10              ((uint32_t)0x0000000A) /*!< LCD RAM Register 10 */
#define LCD_RAM_REGISTER11              ((uint32_t)0x0000000B) /*!< LCD RAM Register 11 */
#define LCD_RAM_REGISTER12              ((uint32_t)0x0000000C) /*!< LCD RAM Register 12 */
#define LCD_RAM_REGISTER13              ((uint32_t)0x0000000D) /*!< LCD RAM Register 13 */
#define LCD_RAM_REGISTER14              ((uint32_t)0x0000000E) /*!< LCD RAM Register 14 */
#define LCD_RAM_REGISTER15              ((uint32_t)0x0000000F) /*!< LCD RAM Register 15 */

#define IS_LCD_RAM_REGISTER(REGISTER) (((REGISTER) == LCD_RAM_REGISTER0) || \
                                       ((REGISTER) == LCD_RAM_REGISTER1) || \
                                       ((REGISTER) == LCD_RAM_REGISTER2) || \
                                       ((REGISTER) == LCD_RAM_REGISTER3) || \
                                       ((REGISTER) == LCD_RAM_REGISTER4) || \
                                       ((REGISTER) == LCD_RAM_REGISTER5) || \
                                       ((REGISTER) == LCD_RAM_REGISTER6) || \
                                       ((REGISTER) == LCD_RAM_REGISTER7) || \
                                       ((REGISTER) == LCD_RAM_REGISTER8) || \
                                       ((REGISTER) == LCD_RAM_REGISTER9) || \
                                       ((REGISTER) == LCD_RAM_REGISTER10) || \
                                       ((REGISTER) == LCD_RAM_REGISTER11) || \
                                       ((REGISTER) == LCD_RAM_REGISTER12) || \
                                       ((REGISTER) == LCD_RAM_REGISTER13) || \
                                       ((REGISTER) == LCD_RAM_REGISTER14) || \
                                       ((REGISTER) == LCD_RAM_REGISTER15))

/**
  * @}
  */  
   
/**
  * @}
  */

/* Exported macro ------------------------------------------------------------*/

/** @defgroup LCD_Exported_Macros
  * @{
  */

/** @brief Reset LCD handle state
  * @param  __HANDLE__: specifies the LCD Handle.
  * @retval None
  */
#define __HAL_LCD_RESET_HANDLE_STATE(__HANDLE__) ((__HANDLE__)->State = HAL_LCD_STATE_RESET)

/** @brief  macros to enables or disables the LCD
  * @param  __HANDLE__: specifies the LCD Handle.  
  * @retval None
  */     
#define __HAL_LCD_ENABLE(__HANDLE__)   ((__HANDLE__)->Instance->CR |= LCD_CR_LCDEN)
#define __HAL_LCD_DISABLE(__HANDLE__)  ((__HANDLE__)->Instance->CR &= ~LCD_CR_LCDEN)

/** @brief  Macros to enable or disable the low resistance divider. Displays with high 
  *         internal resistance may need a longer drive time to achieve 
  *         satisfactory contrast. This function is useful in this case if some 
  *         additional power consumption can be tolerated.
  * @param  __HANDLE__: specifies the LCD Handle.
  * @note   When this mode is enabled, the PulseOn Duration (PON) have to be 
  *         programmed to 1/CK_PS (LCD_PULSEONDURATION_1).
  * @retval None
  */
#define __HAL_LCD_HIGHDRIVER_ENABLE(__HANDLE__)                                \
                              do{                                              \
                                  ((__HANDLE__)->Instance->FCR |= LCD_FCR_HD); \
                                  LCD_WaitForSynchro(__HANDLE__);              \
                                 }while(0)

#define __HAL_LCD_HIGHDRIVER_DISABLE(__HANDLE__)                               \
                              do{                                              \
                                  ((__HANDLE__)->Instance->FCR &= ~LCD_FCR_HD); \
                                  LCD_WaitForSynchro(__HANDLE__);              \
                                 }while(0)
/**
  * @brief  Macro to configure the LCD pulses on duration.
  * @param  __HANDLE__: specifies the LCD Handle.
  * @param  __DURATION__: specifies the LCD pulse on duration in terms of 
  *         CK_PS (prescaled LCD clock period) pulses.
  *   This parameter can be one of the following values:
  *     @arg LCD_PULSEONDURATION_0: 0 pulse
  *     @arg LCD_PULSEONDURATION_1: Pulse ON duration = 1/CK_PS
  *     @arg LCD_PULSEONDURATION_2: Pulse ON duration = 2/CK_PS
  *     @arg LCD_PULSEONDURATION_3: Pulse ON duration = 3/CK_PS
  *     @arg LCD_PULSEONDURATION_4: Pulse ON duration = 4/CK_PS
  *     @arg LCD_PULSEONDURATION_5: Pulse ON duration = 5/CK_PS
  *     @arg LCD_PULSEONDURATION_6: Pulse ON duration = 6/CK_PS
  *     @arg LCD_PULSEONDURATION_7: Pulse ON duration = 7/CK_PS
  * @retval None
  */
#define __HAL_LCD_PULSEONDURATION_CONFIG(__HANDLE__, __DURATION__)                \
        do{                                                                       \
            MODIFY_REG((__HANDLE__)->Instance->FCR, LCD_FCR_PON, (__DURATION__)); \
            LCD_WaitForSynchro(__HANDLE__);                                       \
           }while(0)

/**
  * @brief  Macro to configure the LCD dead time.
  * @param  __HANDLE__: specifies the LCD Handle.
  * @param  __DEADTIME__: specifies the LCD dead time.
  *   This parameter can be one of the following values:
  *     @arg LCD_DEADTIME_0: No dead Time
  *     @arg LCD_DEADTIME_1: One Phase between different couple of Frame
  *     @arg LCD_DEADTIME_2: Two Phase between different couple of Frame
  *     @arg LCD_DEADTIME_3: Three Phase between different couple of Frame
  *     @arg LCD_DEADTIME_4: Four Phase between different couple of Frame
  *     @arg LCD_DEADTIME_5: Five Phase between different couple of Frame
  *     @arg LCD_DEADTIME_6: Six Phase between different couple of Frame 
  *     @arg LCD_DEADTIME_7: Seven Phase between different couple of Frame
  * @retval None
  */
#define __HAL_LCD_DEADTIME_CONFIG(__HANDLE__, __DEADTIME__)                        \
        do{                                                                        \
            MODIFY_REG((__HANDLE__)->Instance->FCR, LCD_FCR_DEAD, (__DEADTIME__)); \
            LCD_WaitForSynchro(__HANDLE__);                                        \
           }while(0)

/**
  * @brief  Macro to configure the LCD Contrast.
  * @param  __HANDLE__: specifies the LCD Handle.
  * @param  __CONTRAST__: specifies the LCD Contrast.
  *   This parameter can be one of the following values:
  *     @arg LCD_CONTRASTLEVEL_0: Maximum Voltage = 2.60V
  *     @arg LCD_CONTRASTLEVEL_1: Maximum Voltage = 2.73V
  *     @arg LCD_CONTRASTLEVEL_2: Maximum Voltage = 2.86V
  *     @arg LCD_CONTRASTLEVEL_3: Maximum Voltage = 2.99V
  *     @arg LCD_CONTRASTLEVEL_4: Maximum Voltage = 3.12V
  *     @arg LCD_CONTRASTLEVEL_5: Maximum Voltage = 3.25V
  *     @arg LCD_CONTRASTLEVEL_6: Maximum Voltage = 3.38V
  *     @arg LCD_CONTRASTLEVEL_7: Maximum Voltage = 3.51V
  * @retval None
  */
#define __HAL_LCD_CONTRAST_CONFIG(__HANDLE__, __CONTRAST__)                       \
        do{                                                                       \
            MODIFY_REG((__HANDLE__)->Instance->FCR, LCD_FCR_CC, (__CONTRAST__));  \
            LCD_WaitForSynchro(__HANDLE__);                                       \
           } while(0) 
/**
  * @brief  Macro to configure the LCD Blink mode and Blink frequency.
  * @param  __HANDLE__: specifies the LCD Handle.
  * @param  __BLINKMODE__: specifies the LCD blink mode.
  *   This parameter can be one of the following values:
  *     @arg LCD_BLINKMODE_OFF:           Blink disabled
  *     @arg LCD_BLINKMODE_SEG0_COM0:     Blink enabled on SEG[0], COM[0] (1 pixel)
  *     @arg LCD_BLINKMODE_SEG0_ALLCOM:   Blink enabled on SEG[0], all COM (up to 8 
  *                                       pixels according to the programmed duty)
  *     @arg LCD_BLINKMODE_ALLSEG_ALLCOM: Blink enabled on all SEG and all COM 
  *                                       (all pixels)
  * @param  __BLINKFREQUENCY__: specifies the LCD blink frequency.
  *     @arg LCD_BLINKFREQUENCY_DIV8:    The Blink frequency = fLcd/8
  *     @arg LCD_BLINKFREQUENCY_DIV16:   The Blink frequency = fLcd/16
  *     @arg LCD_BLINKFREQUENCY_DIV32:   The Blink frequency = fLcd/32
  *     @arg LCD_BLINKFREQUENCY_DIV64:   The Blink frequency = fLcd/64 
  *     @arg LCD_BLINKFREQUENCY_DIV128:  The Blink frequency = fLcd/128
  *     @arg LCD_BLINKFREQUENCY_DIV256:  The Blink frequency = fLcd/256
  *     @arg LCD_BLINKFREQUENCY_DIV512:  The Blink frequency = fLcd/512
  *     @arg LCD_BLINKFREQUENCY_DIV1024: The Blink frequency = fLcd/1024
  * @retval None
  */
#define __HAL_LCD_BLINK_CONFIG(__HANDLE__, __BLINKMODE__, __BLINKFREQUENCY__)  \
        do{                                                                    \
            MODIFY_REG((__HANDLE__)->Instance->FCR, (LCD_FCR_BLINKF | LCD_FCR_BLINK), ((__BLINKMODE__) | (__BLINKFREQUENCY__))); \
            LCD_WaitForSynchro(__HANDLE__);                                    \
          }while(0)

/** @brief  Enables or disables the specified LCD interrupt.
  * @param  __HANDLE__: specifies the LCD Handle.
  * @param  __INTERRUPT__: specifies the LCD interrupt source to be enabled or disabled.
  *          This parameter can be one of the following values:
  *     @arg LCD_IT_SOF: Start of Frame Interrupt
  *     @arg LCD_IT_UDD: Update Display Done Interrupt
  * @retval None
  */
#define __HAL_LCD_ENABLE_IT(__HANDLE__, __INTERRUPT__)                         \
                         do{                                                   \
                             ((__HANDLE__)->Instance->FCR |= (__INTERRUPT__)); \
                             LCD_WaitForSynchro(__HANDLE__);                   \
                            }while(0)         
#define __HAL_LCD_DISABLE_IT(__HANDLE__, __INTERRUPT__)                        \
                         do{                                                   \
                             ((__HANDLE__)->Instance->FCR &= ~(__INTERRUPT__));\
                             LCD_WaitForSynchro(__HANDLE__);                   \
                            }while(0)    
/** @brief  Checks whether the specified LCD interrupt is enabled or not.
  * @param  __HANDLE__: specifies the LCD Handle.
  * @param  __IT__: specifies the LCD interrupt source to check.
  *        This parameter can be one of the following values:
  *        @arg LCD_IT_SOF: Start of Frame Interrupt
  *        @arg LCD_IT_UDD: Update Display Done Interrupt.
  * @note If the device is in STOP mode (PCLK not provided) UDD will not 
  *          generate an interrupt even if UDDIE = 1. 
  *          If the display is not enabled the UDD interrupt will never occur.
  * @retval The state of __IT__ (TRUE or FALSE).
  */
#define __HAL_LCD_GET_IT_SOURCE(__HANDLE__, __IT__) (((__HANDLE__)->Instance->FCR) & (__IT__))

/** @brief  Checks whether the specified LCD flag is set or not.
  * @param  __HANDLE__: specifies the LCD Handle.
  * @param  __FLAG__: specifies the flag to check.
  *        This parameter can be one of the following values:
  *        @arg LCD_FLAG_ENS: LCD Enabled flag. It indicates the LCD controller status.
  * @note  The ENS bit is set immediately when the LCDEN bit in the LCD_CR 
  *             goes from 0 to 1. On deactivation it reflects the real status of 
  *             LCD so it becomes 0 at the end of the last displayed frame.  
  *        @arg LCD_FLAG_SOF: Start of Frame flag. This flag is set by hardware at 
  *             the beginning of a new frame, at the same time as the display data is 
  *             updated.
  *        @arg LCD_FLAG_UDR: Update Display Request flag.
  *        @arg LCD_FLAG_UDD: Update Display Done flag. 
  *        @arg LCD_FLAG_RDY: Step_up converter Ready flag. It indicates the status 
  *             of the step-up converter.
  *        @arg LCD_FLAG_FCRSF: LCD Frame Control Register Synchronization Flag. 
  *             This flag is set by hardware each time the LCD_FCR register is updated 
  *             in the LCDCLK domain. 
  * @retval The new state of __FLAG__ (TRUE or FALSE).
  */
#define __HAL_LCD_GET_FLAG(__HANDLE__, __FLAG__) (((__HANDLE__)->Instance->SR & (__FLAG__)) == (__FLAG__))   

/** @brief  Clears the specified LCD pending flag.
  * @param  __HANDLE__: specifies the LCD Handle.
  * @param  __FLAG__: specifies the flag to check.
  *        This parameter can be any combination of the following values:
  *        @arg LCD_FLAG_SOF: Start of Frame Interrupt
  *        @arg LCD_FLAG_UDD: Update Display Done Interrupt
  * @retval None
  */
#define __HAL_LCD_CLEAR_FLAG(__HANDLE__, __FLAG__) ((__HANDLE__)->Instance->CLR = (__FLAG__))

/**
  * @}
  */
  
/* Exported functions ------------------------------------------------------- */
/* Initialization/de-initialization methods  **********************************/
HAL_StatusTypeDef HAL_LCD_DeInit(LCD_HandleTypeDef *hlcd);
HAL_StatusTypeDef HAL_LCD_Init(LCD_HandleTypeDef *hlcd);
void HAL_LCD_MspInit(LCD_HandleTypeDef *hlcd);
void HAL_LCD_MspDeInit(LCD_HandleTypeDef *hlcd);
 
/* IO operation methods *******************************************************/
HAL_StatusTypeDef HAL_LCD_Write(LCD_HandleTypeDef *hlcd, uint32_t RAMRegisterIndex, uint32_t RAMRegisterMask, uint32_t Data);
HAL_StatusTypeDef HAL_LCD_Clear(LCD_HandleTypeDef *hlcd);
HAL_StatusTypeDef HAL_LCD_UpdateDisplayRequest(LCD_HandleTypeDef *hlcd);

/* Peripheral State methods  **************************************************/
HAL_LCD_StateTypeDef HAL_LCD_GetState(LCD_HandleTypeDef *hlcd);
uint32_t HAL_LCD_GetError(LCD_HandleTypeDef *hlcd);

/* Private functions ---------------------------------------------------------*/
HAL_StatusTypeDef LCD_WaitForSynchro(LCD_HandleTypeDef *hlcd);

#endif /* STM32L051xx && STM32L052xx && STM32L062xx && STM32L061xx*/

#ifdef __cplusplus
}
#endif

#endif /* __STM32L0xx_HAL_LCD_H */

/**
  * @}
  */

/**
  * @}
  */

/******************* (C) COPYRIGHT 2014 STMicroelectronics *****END OF FILE****/
