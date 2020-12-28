/**
  ******************************************************************************
  * @file    stm32l0xx_hal_lcd.c
  * @author  MCD Application Team
  * @brief   LCD Controller HAL module driver.
  *          This file provides firmware functions to manage the following 
  *          functionalities of the LCD Controller (LCD) peripheral:
  *           + Initialization/de-initialization methods
  *           + I/O operation methods
  *           + Peripheral State methods
  *           
  @verbatim
  ==============================================================================
                        ##### How to use this driver #####
  ==============================================================================   
      [..] The LCD HAL driver can be used as follow:
    
      (#) Declare a LCD_HandleTypeDef handle structure.

      (#) Prepare the initialization of the LCD low level resources by implementing your HAL_LCD_MspInit() API:
          (##) Enable the LCDCLK (same as RTCCLK): to configure the RTCCLK/LCDCLK, use the RCC function 
               HAL_RCCEx_PeriphCLKConfig, indicating here RCC_PERIPHCLK_LCD and the selected clock 
               source (HSE, LSI or LSE)
          (##) The frequency generator allows you to achieve various LCD frame rates starting from an 
               LCD input clock frequency (LCDCLK) which can vary from 32 kHz up to 1 MHz.
          (##) LCD pins configuration:
              - Enable the clock for the LCD GPIOs
              - Configure these LCD pins as alternate function no-pull.
          (##) Enable the LCD interface clock.

      (#) Set the Prescaler, Divider, Blink mode, Blink Frequency Duty, Bias, Voltage Source, 
          Dead Time, Pulse On Duration and Contrast in the hlcd Init structure.

      (#) Initialize the LCD registers by calling the HAL_LCD_Init() API.
          (##) The HAL_LCD_Init() API configures the low level Hardware (GPIO, CLOCK, ...etc)
               by calling the user customized HAL_LCD_MspInit() API.
      (#) After calling the HAL_LCD_Init() the LCD RAM memory is cleared

      (#) Optionally you can update the LCD configuration using these macros:
           (##) LCD High Drive using the __HAL_LCD_HIGHDRIVER_ENABLE() and __HAL_LCD_HIGHDRIVER_DISABLE() macros
           (##) LCD Pulse ON Duration using the __HAL_LCD_PULSEONDURATION_CONFIG() macro
           (##) LCD Dead Time using the __HAL_LCD_DEADTIME_CONFIG() macro  
           (##) The LCD Blink mode and frequency using the __HAL_LCD_BLINK_CONFIG() macro
           (##) The LCD Contrast using the __HAL_LCD_CONTRAST_CONFIG() macro  

      (#) Write to the LCD RAM memory using the HAL_LCD_Write() API, this API can be called
          several times to update the different LCD RAM registers before calling 
          HAL_LCD_UpdateDisplayRequest() API.

      (#) The HAL_LCD_Clear() API can be used to clear the LCD RAM memory.

      (#) When the LCD RAM memory is updated, enable the update display request calling
          the HAL_LCD_UpdateDisplayRequest() API.

      [..] LCD and low power modes: The LCD remain active during STOP mode.

  @endverbatim
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright(c) 2016 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under BSD 3-Clause license,
  * the "License"; You may not use this file except in compliance with the
  * License. You may obtain a copy of the License at:
  *                        opensource.org/licenses/BSD-3-Clause
  *
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "stm32l0xx_hal.h"

#if defined (STM32L053xx) || defined (STM32L063xx) || defined (STM32L073xx) || defined (STM32L083xx)

/** @addtogroup STM32L0xx_HAL_Driver
  * @{
  */

#ifdef HAL_LCD_MODULE_ENABLED

/** @addtogroup LCD
  * @brief LCD HAL module driver
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/** @addtogroup LCD_Private
  * @{
  */
#define LCD_TIMEOUT_VALUE             1000U
/**
  * @}
  */
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/** @addtogroup LCD_Exported_Functions
  * @{
  */

/** @addtogroup LCD_Exported_Functions_Group1
  *  @brief    Initialization and Configuration functions 
  *
@verbatim    
===============================================================================
            ##### Initialization and Configuration functions #####
 ===============================================================================  
    [..]

@endverbatim
  * @{
  */
 
/**
  * @brief  DeInitializes the LCD peripheral. 
  * @param  hlcd LCD handle
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_LCD_DeInit(LCD_HandleTypeDef *hlcd)
{
  /* Check the LCD handle allocation */
  if(hlcd == NULL)
  {
    return HAL_ERROR;
  }
  
  /* Check the parameters */
  assert_param(IS_LCD_ALL_INSTANCE(hlcd->Instance));

  /* Check the LCD peripheral state */
  if(hlcd->State == HAL_LCD_STATE_BUSY)
  {
    return HAL_BUSY;
  }

  hlcd->State = HAL_LCD_STATE_BUSY;
  
  /* Disable the peripheral */
  __HAL_LCD_DISABLE(hlcd);

  /*Disable Highdrive by default*/
  __HAL_LCD_HIGHDRIVER_DISABLE(hlcd);

  /* DeInit the low level hardware */
  HAL_LCD_MspDeInit(hlcd);
  
  hlcd->ErrorCode = HAL_LCD_ERROR_NONE;
  hlcd->State = HAL_LCD_STATE_RESET;
    
  /* Release Lock */
  __HAL_UNLOCK(hlcd);
  
  return HAL_OK;  
}

/**
  * @brief  Initializes the LCD peripheral according to the specified parameters 
  *         in the LCD_InitStruct.
  * @note   This function can be used only when the LCD is disabled.
  *         The LCD HighDrive can be enabled/disabled using related macros up to user.
  * @param  hlcd LCD handle
  * @retval None
  */
HAL_StatusTypeDef HAL_LCD_Init(LCD_HandleTypeDef *hlcd)
{
  uint32_t tickstart = 0x00U;
  uint8_t counter = 0U;
    
  /* Check the LCD handle allocation */
  if(hlcd == NULL)
  {
    return HAL_ERROR;
  }
  
  /* Check function parameters */
  assert_param(IS_LCD_ALL_INSTANCE(hlcd->Instance));
  assert_param(IS_LCD_PRESCALER(hlcd->Init.Prescaler));
  assert_param(IS_LCD_DIVIDER(hlcd->Init.Divider));
  assert_param(IS_LCD_DUTY(hlcd->Init.Duty));
  assert_param(IS_LCD_BIAS(hlcd->Init.Bias));  
  assert_param(IS_LCD_VOLTAGE_SOURCE(hlcd->Init.VoltageSource));
  assert_param(IS_LCD_PULSE_ON_DURATION(hlcd->Init.PulseOnDuration));
  assert_param(IS_LCD_HIGHDRIVE(hlcd->Init.HighDrive));
  assert_param(IS_LCD_DEAD_TIME(hlcd->Init.DeadTime));
  assert_param(IS_LCD_CONTRAST(hlcd->Init.Contrast)); 
  assert_param(IS_LCD_BLINK_FREQUENCY(hlcd->Init.BlinkFrequency)); 
  assert_param(IS_LCD_BLINK_MODE(hlcd->Init.BlinkMode)); 
  assert_param(IS_LCD_MUXSEGMENT(hlcd->Init.MuxSegment));
  
  if(hlcd->State == HAL_LCD_STATE_RESET)
  {
    /* Allocate lock resource and initialize it */
    __HAL_UNLOCK(hlcd);

    /* Initialize the low level hardware (MSP) */
    HAL_LCD_MspInit(hlcd);
  }
  
  hlcd->State = HAL_LCD_STATE_BUSY;
  
  /* Disable the peripheral */
  __HAL_LCD_DISABLE(hlcd);
  
  /* Clear the LCD_RAM registers and enable the display request by setting the UDR bit
     in the LCD_SR register */
  for(counter = LCD_RAM_REGISTER0; counter <= LCD_RAM_REGISTER15; counter++)
  {
    hlcd->Instance->RAM[counter] = 0U;
  }
  /* Enable the display request */
  SET_BIT(hlcd->Instance->SR, LCD_SR_UDR);
  
  /* Configure the LCD Prescaler, Divider, Blink mode and Blink Frequency: 
  Set PS[3:0] bits according to hlcd->Init.Prescaler value 
     Set DIV[3:0] bits according to hlcd->Init.Divider value
     Set BLINK[1:0] bits according to hlcd->Init.BlinkMode value
     Set BLINKF[2:0] bits according to hlcd->Init.BlinkFrequency value
     Set DEAD[2:0] bits according to hlcd->Init.DeadTime value
     Set PON[2:0] bits according to hlcd->Init.PulseOnDuration value 
     Set CC[2:0] bits according to hlcd->Init.Contrast value
     Set HD[0] bit according to hlcd->Init.HighDrive value*/

  MODIFY_REG(hlcd->Instance->FCR, \
             (LCD_FCR_PS | LCD_FCR_DIV | LCD_FCR_BLINK| LCD_FCR_BLINKF | \
             LCD_FCR_DEAD | LCD_FCR_PON | LCD_FCR_CC), \
             (hlcd->Init.Prescaler | hlcd->Init.Divider | hlcd->Init.BlinkMode | hlcd->Init.BlinkFrequency | \
             hlcd->Init.DeadTime | hlcd->Init.PulseOnDuration | hlcd->Init.Contrast | hlcd->Init.HighDrive));

  /* Wait until LCD Frame Control Register Synchronization flag (FCRSF) is set in the LCD_SR register 
     This bit is set by hardware each time the LCD_FCR register is updated in the LCDCLK
     domain. It is cleared by hardware when writing to the LCD_FCR register.*/
  LCD_WaitForSynchro(hlcd);
  
  /* Configure the LCD Duty, Bias, Voltage Source, Dead Time:
     Set DUTY[2:0] bits according to hlcd->Init.Duty value 
     Set BIAS[1:0] bits according to hlcd->Init.Bias value
     Set VSEL bit according to hlcd->Init.VoltageSource value
     Set MUX_SEG bit according to hlcd->Init.MuxSegment value */
  MODIFY_REG(hlcd->Instance->CR, \
             (LCD_CR_DUTY | LCD_CR_BIAS | LCD_CR_VSEL | LCD_CR_MUX_SEG), \
             (hlcd->Init.Duty | hlcd->Init.Bias | hlcd->Init.VoltageSource | hlcd->Init.MuxSegment));
  
  /* Enable the peripheral */
  __HAL_LCD_ENABLE(hlcd);
  
  /* Get timeout */
  tickstart = HAL_GetTick();
      
  /* Wait Until the LCD is enabled */
  while(__HAL_LCD_GET_FLAG(hlcd, LCD_FLAG_ENS) == RESET)
  {
    if((HAL_GetTick() - tickstart ) > LCD_TIMEOUT_VALUE)
    { 
      hlcd->ErrorCode = HAL_LCD_ERROR_ENS;
      return HAL_TIMEOUT;
    } 
  }
  
  /* Get timeout */
  tickstart = HAL_GetTick();
  
  /*!< Wait Until the LCD Booster is ready */
  while(__HAL_LCD_GET_FLAG(hlcd, LCD_FLAG_RDY) == RESET)
  {
    if((HAL_GetTick() - tickstart ) > LCD_TIMEOUT_VALUE)
    {   
      hlcd->ErrorCode = HAL_LCD_ERROR_RDY;
      return HAL_TIMEOUT;
    } 
  }
 
  /* Initialize the LCD state */
  hlcd->ErrorCode = HAL_LCD_ERROR_NONE;
  hlcd->State= HAL_LCD_STATE_READY;
  
  return HAL_OK;
}

/**
  * @brief  LCD MSP DeInit.
  * @param  hlcd LCD handle
  * @retval None
  */
 __weak void HAL_LCD_MspDeInit(LCD_HandleTypeDef *hlcd)
{
  /* Prevent unused argument(s) compilation warning */
  UNUSED(hlcd);

  /* NOTE: This function Should not be modified, when the callback is needed,
           the HAL_LCD_MspDeInit could be implemented in the user file
   */ 
}

/**
  * @brief  LCD MSP Init.
  * @param  hlcd LCD handle
  * @retval None
  */
 __weak void HAL_LCD_MspInit(LCD_HandleTypeDef *hlcd)
{
  /* Prevent unused argument(s) compilation warning */
  UNUSED(hlcd);

  /* NOTE: This function Should not be modified, when the callback is needed,
           the HAL_LCD_MspInit could be implemented in the user file
   */ 
}

/**
  * @}
  */

/** @addtogroup LCD_Exported_Functions_Group2
  *  @brief LCD RAM functions 
  *
@verbatim   
 ===============================================================================
                      ##### IO operation functions #####
 ===============================================================================  
 [..] Using its double buffer memory the LCD controller ensures the coherency of the 
 displayed information without having to use interrupts to control LCD_RAM 
 modification.
 The application software can access the first buffer level (LCD_RAM) through 
 the APB interface. Once it has modified the LCD_RAM using the HAL_LCD_Write() API,
 it sets the UDR flag in the LCD_SR register using the HAL_LCD_UpdateDisplayRequest() API.
 This UDR flag (update display request) requests the updated information to be 
 moved into the second buffer level (LCD_DISPLAY).
 This operation is done synchronously with the frame (at the beginning of the 
 next frame), until the update is completed, the LCD_RAM is write protected and 
 the UDR flag stays high.
 Once the update is completed another flag (UDD - Update Display Done) is set and
 generates an interrupt if the UDDIE bit in the LCD_FCR register is set.
 The time it takes to update LCD_DISPLAY is, in the worst case, one odd and one 
 even frame.
 The update will not occur (UDR = 1 and UDD = 0) until the display is 
 enabled (LCDEN = 1).
      
@endverbatim
  * @{
  */

/**
  * @brief  Writes a word in the specific LCD RAM.
  * @param  hlcd LCD handle
  * @param  RAMRegisterIndex specifies the LCD RAM Register.
  *   This parameter can be one of the following values:
  *     @arg LCD_RAM_REGISTER0: LCD RAM Register 0
  *     @arg LCD_RAM_REGISTER1: LCD RAM Register 1
  *     @arg LCD_RAM_REGISTER2: LCD RAM Register 2
  *     @arg LCD_RAM_REGISTER3: LCD RAM Register 3
  *     @arg LCD_RAM_REGISTER4: LCD RAM Register 4
  *     @arg LCD_RAM_REGISTER5: LCD RAM Register 5
  *     @arg LCD_RAM_REGISTER6: LCD RAM Register 6 
  *     @arg LCD_RAM_REGISTER7: LCD RAM Register 7  
  *     @arg LCD_RAM_REGISTER8: LCD RAM Register 8
  *     @arg LCD_RAM_REGISTER9: LCD RAM Register 9
  *     @arg LCD_RAM_REGISTER10: LCD RAM Register 10
  *     @arg LCD_RAM_REGISTER11: LCD RAM Register 11
  *     @arg LCD_RAM_REGISTER12: LCD RAM Register 12 
  *     @arg LCD_RAM_REGISTER13: LCD RAM Register 13 
  *     @arg LCD_RAM_REGISTER14: LCD RAM Register 14 
  *     @arg LCD_RAM_REGISTER15: LCD RAM Register 15
  * @param  RAMRegisterMask specifies the LCD RAM Register Data Mask.
  * @param  Data specifies LCD Data Value to be written.
  * @note   For LCD glass COM*SEG as 8*40 for example, the LCD common terminals COM[0,7]
  *         are mapped on 32bits LCD_RAM_REGISTER[0,14] according to rules: COM(n) spread
  *  	    on LCD_RAM_REGISTER(2*n) and  LCD_RAM_REGISTER(2*n+1).The segment terminals 
  *		    SEG[0,39] of COM(n) correspond to LSB bits of related LCD_RAM_REGISTER(2*n)[0,31]
  *		    and LCD_RAM_REGISTER(2*n+1)[0,7] 
  * @retval None
  */
HAL_StatusTypeDef HAL_LCD_Write(LCD_HandleTypeDef *hlcd, uint32_t RAMRegisterIndex, uint32_t RAMRegisterMask, uint32_t Data)
{
  uint32_t tickstart = 0x00U; 
  
  if((hlcd->State == HAL_LCD_STATE_READY) || (hlcd->State == HAL_LCD_STATE_BUSY))
  {
    /* Check the parameters */
    assert_param(IS_LCD_RAM_REGISTER(RAMRegisterIndex));
    
    if(hlcd->State == HAL_LCD_STATE_READY)
    {
      /* Process Locked */
      __HAL_LOCK(hlcd);
      hlcd->State = HAL_LCD_STATE_BUSY;
      
      /* Get timeout */
      tickstart = HAL_GetTick();
      
      /*!< Wait Until the LCD is ready */
  while(__HAL_LCD_GET_FLAG(hlcd, LCD_FLAG_UDR) != RESET)
      {
        if((HAL_GetTick() - tickstart) > LCD_TIMEOUT_VALUE)
        { 
          hlcd->ErrorCode = HAL_LCD_ERROR_UDR;      
          /* Process Unlocked */
          __HAL_UNLOCK(hlcd);
          return HAL_TIMEOUT;
        } 
      }
    }
/* Copy the new Data bytes to LCD RAM register */
    MODIFY_REG(hlcd->Instance->RAM[RAMRegisterIndex], ~(RAMRegisterMask), Data);

    return HAL_OK;
  }
  else
  {
    return HAL_ERROR;
  }
}

/**
  * @brief Clears the LCD RAM registers.
  * @param hlcd: LCD handle
  * @retval None
  */
HAL_StatusTypeDef HAL_LCD_Clear(LCD_HandleTypeDef *hlcd)
{
  uint32_t tickstart = 0x00U; 
  uint32_t counter = 0U;

  if((hlcd->State == HAL_LCD_STATE_READY) || (hlcd->State == HAL_LCD_STATE_BUSY))
  {
    /* Process Locked */
    __HAL_LOCK(hlcd);
    
    hlcd->State = HAL_LCD_STATE_BUSY;
    
    /* Get timeout */
    tickstart = HAL_GetTick();
    
    /*!< Wait Until the LCD is ready */
    while(__HAL_LCD_GET_FLAG(hlcd, LCD_FLAG_UDR) != RESET)
    {
      if( (HAL_GetTick() - tickstart) > LCD_TIMEOUT_VALUE)
      { 
        hlcd->ErrorCode = HAL_LCD_ERROR_UDR;

        /* Process Unlocked */
        __HAL_UNLOCK(hlcd);

        return HAL_TIMEOUT;
      } 
    }
    /* Clear the LCD_RAM registers */
    for(counter = LCD_RAM_REGISTER0; counter <= LCD_RAM_REGISTER15; counter++)
    {
      hlcd->Instance->RAM[counter] = 0U;
    }
    
    /* Update the LCD display */
    HAL_LCD_UpdateDisplayRequest(hlcd);     
    
    return HAL_OK;
  }
  else
  {
    return HAL_ERROR;
  }
}

/**
  * @brief  Enables the Update Display Request.
  * @param  hlcd LCD handle
  * @note   Each time software modifies the LCD_RAM it must set the UDR bit to 
  *         transfer the updated data to the second level buffer. 
  *         The UDR bit stays set until the end of the update and during this 
  *         time the LCD_RAM is write protected. 
  * @note   When the display is disabled, the update is performed for all 
  *         LCD_DISPLAY locations.
  *         When the display is enabled, the update is performed only for locations 
  *         for which commons are active (depending on DUTY). For example if 
  *         DUTY = 1/2, only the LCD_DISPLAY of COM0 and COM1 will be updated.    
  * @retval None
  */
HAL_StatusTypeDef HAL_LCD_UpdateDisplayRequest(LCD_HandleTypeDef *hlcd)
{
  uint32_t tickstart = 0x00U;
  
  /* Clear the Update Display Done flag before starting the update display request */
  __HAL_LCD_CLEAR_FLAG(hlcd, LCD_FLAG_UDD);
  
  /* Enable the display request */
  hlcd->Instance->SR |= LCD_SR_UDR;
  
  /* Get timeout */
  tickstart = HAL_GetTick();
  
  /*!< Wait Until the LCD display is done */
  while(__HAL_LCD_GET_FLAG(hlcd, LCD_FLAG_UDD) == RESET)
  {
    if((HAL_GetTick() - tickstart ) > LCD_TIMEOUT_VALUE)
    { 
      hlcd->ErrorCode = HAL_LCD_ERROR_UDD;
      
      /* Process Unlocked */
      __HAL_UNLOCK(hlcd);
  
      return HAL_TIMEOUT;
    }
  }

  hlcd->State = HAL_LCD_STATE_READY;
  
  /* Process Unlocked */
  __HAL_UNLOCK(hlcd);
  
  return HAL_OK;
}

/**
  * @}
  */

/** @addtogroup LCD_Exported_Functions_Group3
  *  @brief   LCD State functions 
  *
@verbatim   
 ===============================================================================
                      ##### Peripheral State functions #####
 ===============================================================================  
    [..]
     This subsection provides a set of functions allowing to control the LCD:
      (+) HAL_LCD_GetState() API can be helpful to check in run-time the state of the LCD peripheral State. 
      (+) HAL_LCD_GetError() API to return the LCD error code. 
@endverbatim
  * @{
  */

/**
  * @brief Returns the LCD state.
  * @param hlcd: LCD handle
  * @retval HAL state
  */
HAL_LCD_StateTypeDef HAL_LCD_GetState(LCD_HandleTypeDef *hlcd)
{
  return hlcd->State;
}

/**
  * @brief Return the LCD error code
  * @param hlcd: LCD handle
  * @retval LCD Error Code
  */
uint32_t HAL_LCD_GetError(LCD_HandleTypeDef *hlcd)
{
  return hlcd->ErrorCode;
}

/**
  * @}
  */

/**
  * @}
  */
  
/** @addtogroup LCD_Private
  * @{
  */

/**
  * @brief  Waits until the LCD FCR register is synchronized in the LCDCLK domain.
  *   This function must be called after any write operation to LCD_FCR register.
  * @param  hlcd LCD handle
  * @retval None
  */
HAL_StatusTypeDef LCD_WaitForSynchro(LCD_HandleTypeDef *hlcd)
{
  uint32_t tickstart = 0x00U; 
  
  /* Get timeout */
  tickstart = HAL_GetTick();

  /* Loop until FCRSF flag is set */
  while(__HAL_LCD_GET_FLAG(hlcd, LCD_FLAG_FCRSF) == RESET)
  {
    if((HAL_GetTick() - tickstart) > LCD_TIMEOUT_VALUE)
    {       
      hlcd->ErrorCode = HAL_LCD_ERROR_FCRSF;
      return HAL_TIMEOUT;
    } 
  }

  return HAL_OK;
}

/**
  * @}
  */


/**
  * @}
  */

#endif /* HAL_LCD_MODULE_ENABLED */
/**
  * @}
  */
#endif /* #if defined (STM32L053xx) || defined (STM32L063xx) || defined (STM32L073xx) || defined (STM32L083xx) */
/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/

