/**
******************************************************************************
* @file    BlueNRG1_gpio.c
* @author  VMA Application Team
* @version V2.2.0
* @date    31-January-2017
* @brief   This file provides all the GPIO firmware functions.
******************************************************************************
* @attention
*
* THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
* WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE
* TIME. AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY
* DIRECT, INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING
* FROM THE CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE
* CODING INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
*
* <h2><center>&copy; COPYRIGHT 2016 STMicroelectronics</center></h2>
******************************************************************************
*/

/* Includes ------------------------------------------------------------------*/
#include "../../Periph_Driver/inc/BlueNRG1_gpio.h"

/** @addtogroup BLUENRG1_Peripheral_Driver BLUENRG1 Peripheral Driver
* @{
*/

/** @addtogroup GPIO_Peripheral  GPIO Peripheral
* @{
*/ 

/** @defgroup GPIO_Private_TypesDefinitions Private Type Definitions
* @{
*/

/**
* @}
*/

/** @defgroup GPIO_Private_Defines Private Defines
* @{
*/

/**
* @}
*/

/** @defgroup GPIO_Private_Macros Private Macros
* @{
*/

/**
* @}
*/

/** @defgroup GPIO_Private_Variables Private Variables
* @{
*/

/**
* @}
*/

/** @defgroup GPIO_Private_FunctionPrototypes Private Function Prototypes
* @{
*/

/**
* @}
*/

/** @defgroup GPIO_Private_Functions Private Functions
* @{
*/



/**
* @}
*/

/** @defgroup GPIO_Public_Functions Public Functions
* @{
*/

/**
* @brief  Deinitializes the GPIO peripheral registers to their default reset values.
* @param  None
* @retval None
*/
void GPIO_DeInit(void)
{
  /* Set all GPIO registers to default/reset configuration */
  GPIO->OEN   = 0x00000000;
  GPIO->PE    = 0x03FFFFFF;
  GPIO->DS    = 0x00000000;
  GPIO->IS    = 0x00000000;
  GPIO->IBE   = 0x00000000;
  GPIO->IEV   = 0x00000000;
  GPIO->IE    = 0x00000000;
  GPIO->IC    = 0xFFFFFFFF;
  GPIO->MODE0 = 0x00000000;
  GPIO->MODE1 = 0x00000110;
  GPIO->MFTX  = 0x00000000;
}


/**
* @brief  Initializes the GPIO peripheral according to the specified
*         parameters in the GPIO_InitStruct.
* @param  GPIO_InitStruct: pointer to a @ref GPIO_InitType structure that
*         contains the configuration information for the specified GPIO peripheral.
* @retval None
*/
void GPIO_Init(GPIO_InitType* GPIO_InitStruct)
{
  uint8_t tmp;
  
  /* Check the parameters */
  assert_param(IS_GPIO_PINS(GPIO_InitStruct->GPIO_Pin));
  assert_param(IS_GPIO_MODE(GPIO_InitStruct->GPIO_Mode));
  assert_param(IS_FUNCTIONAL_STATE(GPIO_InitStruct->GPIO_Pull));
  assert_param(IS_FUNCTIONAL_STATE(GPIO_InitStruct->GPIO_HighPwr));
  
  /* IO12 and IO13 cannot be programmed in output mode */
  assert_param( ((GPIO_InitStruct->GPIO_Pin & GPIO_Pin_12) != GPIO_Pin_12) || (GPIO_InitStruct->GPIO_Mode != GPIO_Output));
  assert_param( ((GPIO_InitStruct->GPIO_Pin & GPIO_Pin_13) != GPIO_Pin_13) || (GPIO_InitStruct->GPIO_Mode != GPIO_Output));
  
  
  /* Configure the IO mode */
  if(GPIO_InitStruct->GPIO_Mode == GPIO_Input) {
    CLEAR_BIT(GPIO->OEN, GPIO_InitStruct->GPIO_Pin);
  }
  else if(GPIO_InitStruct->GPIO_Mode == GPIO_Output) {
    GPIO_InitStruct->GPIO_Mode = GPIO_Input;
    SET_BIT(GPIO->OEN, GPIO_InitStruct->GPIO_Pin);
  }
  
  for(uint8_t i = 0; i < GPIO_PIN_COUNT; i++) {
    tmp = (uint8_t)((GPIO_InitStruct->GPIO_Pin>>i) & 0x01);
    if(tmp!=0) {

      if(i < 8) {
        MODIFY_REG(GPIO->MODE0, (0xF<<(i*4)), (GPIO_InitStruct->GPIO_Mode << (i*4)) );
      }
      else if((i > 7) && (i < 16)) {
        MODIFY_REG(GPIO->MODE1, (0xF<<((i-8)*4)), (GPIO_InitStruct->GPIO_Mode << ((i-8)*4)) );
      }
#ifdef BLUENRG2_DEVICE
      else if((i > 15) && (i < 24)) {
        MODIFY_REG(GPIO->MODE2, (0xF<<((i-16)*4)), (GPIO_InitStruct->GPIO_Mode << ((i-16)*4)) );
      }
      else if(i > 23) {
        MODIFY_REG(GPIO->MODE3, (0xF<<((i-24)*4)), (GPIO_InitStruct->GPIO_Mode << ((i-24)*4)) );
      }
#endif
    }
  }

  /* Set the current mode */
  if(GPIO_InitStruct->GPIO_HighPwr == ENABLE) {
    SET_BIT(GPIO->DS, GPIO_InitStruct->GPIO_Pin);
  }
  else {
    CLEAR_BIT(GPIO->DS, GPIO_InitStruct->GPIO_Pin);
  }

  /* Set the pull on IO */
  if(GPIO_InitStruct->GPIO_Pull == ENABLE) {
    SET_BIT(GPIO->PE, GPIO_InitStruct->GPIO_Pin);
  }
  else {
    CLEAR_BIT(GPIO->PE, GPIO_InitStruct->GPIO_Pin);
  }

}

/**
* @brief  Fills the GPIO_InitStruct member with its default value.
* @param  GPIO_InitStruct : pointer to a @ref GPIO_InitType structure which will
*         be initialized.
* @retval None
*/
void GPIO_StructInit(GPIO_InitType* GPIO_InitStruct)
{
  /* Reset GPIO init structure parameters values */
  GPIO_InitStruct->GPIO_Pin  = 0x00000000;
  GPIO_InitStruct->GPIO_Mode = GPIO_Input;
  GPIO_InitStruct->GPIO_Pull = DISABLE;
  GPIO_InitStruct->GPIO_HighPwr = DISABLE;
}


/**
* @brief  Initializes the GPIO9, GPIO10 and GPIO11 according to the specified
*         parameters in the GPIO_InitStruct. This setting is applied during low power modes.
*         This feature is applicable only for BlueNRG-2.
* @param  GPIO_InitStruct: pointer to a @ref GPIO_InitType structure that
*         contains the configuration information for the specified GPIO peripheral.
* @retval None
*/
void GPIO_InitLowPowerModes(GPIO_InitType* GPIO_InitStruct)
{
#ifdef BLUENRG2_DEVICE
  
  /* Check the parameters */
  assert_param(IS_GPIO_PINS(GPIO_InitStruct->GPIO_Pin));
  assert_param(IS_GPIO_MODE(GPIO_InitStruct->GPIO_Mode));
  assert_param(IS_FUNCTIONAL_STATE(GPIO_InitStruct->GPIO_Pull));
  assert_param(IS_FUNCTIONAL_STATE(GPIO_InitStruct->GPIO_HighPwr));

  if((GPIO_InitStruct->GPIO_Pin&GPIO_Pin_9) == GPIO_Pin_9  ||
       (GPIO_InitStruct->GPIO_Pin&GPIO_Pin_10) == GPIO_Pin_10 ||
       (GPIO_InitStruct->GPIO_Pin&GPIO_Pin_11) == GPIO_Pin_11) {

    /* Configure the IO mode */
    if(GPIO_InitStruct->GPIO_Mode == GPIO_Input) {
      SET_BIT(SYSTEM_CTRL->SLEEPIO_OEN, (GPIO_InitStruct->GPIO_Pin>>9));
    }
    else if(GPIO_InitStruct->GPIO_Mode == GPIO_Output) {
      CLEAR_BIT(SYSTEM_CTRL->SLEEPIO_OEN, (GPIO_InitStruct->GPIO_Pin>>9));
    }
    
    /* Set the current mode */
    if(GPIO_InitStruct->GPIO_HighPwr == ENABLE) {
      SET_BIT(SYSTEM_CTRL->SLEEPIO_DS, (GPIO_InitStruct->GPIO_Pin>>9));
    }
    else {
      CLEAR_BIT(SYSTEM_CTRL->SLEEPIO_DS, (GPIO_InitStruct->GPIO_Pin>>9));
    }

    /* Set the pull on IO */
    if(GPIO_InitStruct->GPIO_Pull == ENABLE) {
      SET_BIT(SYSTEM_CTRL->SLEEPIO_PE, (GPIO_InitStruct->GPIO_Pin>>9));
    }
    else {
      CLEAR_BIT(SYSTEM_CTRL->SLEEPIO_PE, (GPIO_InitStruct->GPIO_Pin>>9));
    }
  }
#endif
}


/**
* @brief  Set the output state of the GIO9, GIO10, GIO11 during low power modes.
*         Only for BlueNRG-2 device.
* @param  GPIO_Pin: specifies the GPIO pin pins to be retained
*         This parameter can be GPIO_Pin_x where x can be (9, 10, 11) @ref GPIO_Pins_Definition.
* @retval None
*/
void GPIO_WriteBitLowPowerModes(uint32_t GPIO_Pin, BitAction BitVal)
{
#ifdef BLUENRG2_DEVICE
  /* Check the parameters */
  assert_param(IS_GPIO_PIN(GPIO_Pin));
  assert_param(IS_GPIO_BIT_ACTION(BitVal)); 
  
  /* Reset GPIO init structure parameters values */
  if((GPIO_Pin&GPIO_Pin_9) == GPIO_Pin_9  ||
     (GPIO_Pin&GPIO_Pin_10) == GPIO_Pin_10 ||
       (GPIO_Pin&GPIO_Pin_11) == GPIO_Pin_11) {
         if (BitVal == Bit_SET) {
           SET_BIT(SYSTEM_CTRL->SLEEPIO_OUT, (GPIO_Pin>>9));
         }         
         else {
           CLEAR_BIT(SYSTEM_CTRL->SLEEPIO_OUT, (GPIO_Pin>>9));
         }
       }
#endif
}



/**
* @brief  Reads the specified input pin value.
* @param  GPIO_Pin: specifies the GPIO pin bits to read.
*         This parameter can be GPIO_Pin_x where x can be (0..25) @ref GPIO_Pins_Definition.
*         QFN32 package: 15 pins, WLCSP32 package: 14 pins, QFN48 package: 26 pins.
* @retval The input pin value @ref BitAction.
*/
BitAction GPIO_ReadBit(uint32_t GPIO_Pin)
{
  /* Check the parameters */
  assert_param(IS_GPIO_PIN(GPIO_Pin)); 
  
  if(READ_BIT(GPIO->DATA, GPIO_Pin)) {
    return Bit_SET;
  }
  else {
    return Bit_RESET;
  }
}


/**
* @brief  Sets the selected IO pin bit.
* @param  GPIO_Pin: specifies the pins to be written.
*         This parameter can be GPIO_Pin_x where x can be (0..25) @ref GPIO_Pins_Definition.
*         QFN32 package: 15 pins, WLCSP32 package: 14 pins, QFN48 package: 26 pins.
*         More than one pin can be specified in OR logic, i.e. (GPIO_Pin_0 | GPIO_Pin_1).
* @retval None
*/
void GPIO_SetBits(uint32_t GPIO_Pins)
{
  /* Check the parameters */
  assert_param(IS_GPIO_PINS(GPIO_Pins));
  
  WRITE_REG(GPIO->DATS, GPIO_Pins);
}

/**
* @brief  Clears the selected IO pin bits.
* @param  GPIO_Pin: specifies the pins to be written.
*         This parameter can be GPIO_Pin_x where x can be (0..25) @ref GPIO_Pins_Definition.
*         QFN32 package: 15 pins, WLCSP32 package: 14 pins, QFN48 package: 26 pins.
*         More than one pin can be specified in OR logic, i.e. (GPIO_Pin_0 | GPIO_Pin_1).
* @retval None
*/
void GPIO_ResetBits(uint32_t GPIO_Pins)
{
  /* Check the parameters */
  assert_param(IS_GPIO_PINS(GPIO_Pins));
  
  WRITE_REG(GPIO->DATC, GPIO_Pins);
}

/**
* @brief  Sets or clears the selected IO pin bit.
* @param  GPIO_Pin: specifies the IO bits to be written.
*         This parameter can be GPIO_Pin_x where x can be (0..25) @ref GPIO_Pins_Definition.
*         QFN32 package: 15 pins, WLCSP32 package: 14 pins, QFN48 package: 26 pins.
*         More than one pin can be specified in OR logic, i.e. (GPIO_Pin_0 | GPIO_Pin_1).
* @param  BitVal: specifies the value to be written to the selected bit.
*         This parameter can be any value of @ref BitAction.
* @retval None
*/
void GPIO_WriteBit(uint32_t GPIO_Pins, BitAction BitVal)
{
  /* Check the parameters */
  assert_param(IS_GPIO_PINS(GPIO_Pins));
  assert_param(IS_GPIO_BIT_ACTION(BitVal)); 
  
  if (BitVal == Bit_SET) {
    WRITE_REG(GPIO->DATS, GPIO_Pins);
  }
  else {
    WRITE_REG(GPIO->DATC, GPIO_Pins);
  }
}

/**
* @brief  Toggles data to the specified IO pin.
* @param  GPIO_Pin: specifies the IO bits to be toggled.
*         This parameter can be GPIO_Pin_x where x can be (0..25) @ref GPIO_Pins_Definition.
*         QFN32 package: 15 pins, WLCSP32 package: 14 pins, QFN48 package: 26 pins.
*         More than one pin can be specified in OR logic, i.e. (GPIO_Pin_0 | GPIO_Pin_1).
* @retval None
*/
void GPIO_ToggleBits(uint32_t GPIO_Pins)
{  
  /* Check the parameters */  
  assert_param(IS_GPIO_PINS(GPIO_Pins));
  
  GPIO->DATA ^= GPIO_Pins;
}

/**
* @brief  Fills each GPIO_EXTIStruct member with its default value.
* @param  GPIO_EXTIInitStruct : pointer to a @ref GPIO_EXTIConfigType structure.
* @retval None
*/
void GPIO_EXTIStructInit(GPIO_EXTIConfigType* GPIO_EXTIInitStruct)
{
  /* Reset GPIO init structure parameters values */
  GPIO_EXTIInitStruct->GPIO_Pin = 0x00000000;
  GPIO_EXTIInitStruct->GPIO_IrqSense = GPIO_IrqSense_Edge;
  GPIO_EXTIInitStruct->GPIO_Event = GPIO_Event_Low;
}


/**
* @brief  Selects the GPIO pins used as EXTI Line.
* @param  EXTIConfig: pointer to a @ref GPIO_EXTIConfigType structure that
*   contains the configuration information for the specified GPIO pins.
* @retval None
*/
void GPIO_EXTIConfig(GPIO_EXTIConfigType* EXTIConfig)
{
  /* Check the parameters */
  assert_param(IS_GPIO_PINS(EXTIConfig->GPIO_Pin));
  assert_param(IS_GPIO_IRQSENSE(EXTIConfig->GPIO_IrqSense));
  assert_param(IS_GPIO_EVENT(EXTIConfig->GPIO_Event));
  
  /* Interrupt sense configuration */
  if (EXTIConfig->GPIO_IrqSense == GPIO_IrqSense_Level) {
    /* Configure level detection */
    SET_BIT(GPIO->IS, EXTIConfig->GPIO_Pin);
  }
  else {
    /* Configure edge detection */
    CLEAR_BIT(GPIO->IS, EXTIConfig->GPIO_Pin);
  }
  
  /* Configure event */
  if (EXTIConfig->GPIO_Event == GPIO_Event_High) {
    /* Configure interrupt on high level or rising edge event */
    SET_BIT(GPIO->IEV, EXTIConfig->GPIO_Pin);
    CLEAR_BIT(GPIO->IBE, EXTIConfig->GPIO_Pin);
  }
  else if (EXTIConfig->GPIO_Event == GPIO_Event_Low) {
    /* Configure interrupt on low level or falling edge event */
    CLEAR_BIT(GPIO->IEV, EXTIConfig->GPIO_Pin);
    CLEAR_BIT(GPIO->IBE, EXTIConfig->GPIO_Pin);
  }
  else {
    /* Configure interrupt on both edge */
    SET_BIT(GPIO->IBE, EXTIConfig->GPIO_Pin);
  }
  
}

/**
* @brief  Enables or disables interrupts on specified pins.
* @param  GPIO_Pin: specifies the IO pins as an interrupt source.
*         This parameter can be GPIO_Pin_x where x can be (0..25) @ref GPIO_Pins_Definition.
*         QFN32 package: 15 pins, WLCSP32 package: 14 pins, QFN48 package: 26 pins.
*         More than one pin can be specified in OR logic, i.e. (GPIO_Pin_0 | GPIO_Pin_1).
* @param  NewState: functional state @ref FunctionalState
*         This parameter can be: ENABLE or DISABLE.
* @retval None
*/
void GPIO_EXTICmd(uint32_t GPIO_Pins, FunctionalState NewState)
{
  /* Check the parameters */
  assert_param(IS_GPIO_PINS(GPIO_Pins));
  assert_param(IS_FUNCTIONAL_STATE(NewState));
  
  if (NewState != DISABLE) {
    SET_BIT(GPIO->IE, GPIO_Pins);
  }
  else {
    CLEAR_BIT(GPIO->IE, GPIO_Pins);
  }
}

/**
* @brief  Clears the GPIO interrupt pending bits.
* @param  GPIO_Pin: specifies the GPIO interrupt pending bit to clear.
*         This parameter can be GPIO_Pin_x where x can be (0..25) @ref GPIO_Pins_Definition.
*         QFN32 package: 15 pins, WLCSP32 package: 14 pins, QFN48 package: 26 pins.
*         More than one pin can be specified in OR logic, i.e. (GPIO_Pin_0 | GPIO_Pin_1).
* @retval None
*/
void GPIO_ClearITPendingBit(uint32_t GPIO_Pins)
{
  /* Check the parameters */
  assert_param(IS_GPIO_PINS(GPIO_Pins));

  SET_BIT(GPIO->IC, GPIO_Pins);
}


/**
* @brief  Checks whether the specified enabled GPIO interrupt is active.
* @param  GPIO_Pin: Specifies the GPIO interrupt to check.
*         This parameter can be GPIO_Pin_x where x can be (0..25) @ref GPIO_Pins_Definition.
*         QFN32 package: 15 pins, WLCSP32 package: 14 pins, QFN48 package: 26 pins.
* @param  FlagStatus: functional state @ref FlagStatus
*         This parameter can be: SET or RESET.
*/
FlagStatus GPIO_GetITPendingBit(uint32_t GPIO_Pin)
{  
  /* Check the parameters */
  assert_param(IS_GPIO_PIN(GPIO_Pin));
  
  if(READ_BIT(GPIO->MIS, GPIO_Pin) != (uint32_t)RESET) {
    return SET;
  }
  else {
    return RESET;
  }
}


/**
* @brief  Checks whether the specified GPIO interrupt is active.
* @param  GPIO_Pin: Specifies the GPIO interrupt to check.
*         This parameter can be GPIO_Pin_x where x can be (0..25) @ref GPIO_Pins_Definition.
*         QFN32 package: 15 pins, WLCSP32 package: 14 pins, QFN48 package: 26 pins.
* @param  FlagStatus: functional state @ref FlagStatus
*         This parameter can be: SET or RESET.
*/
ITStatus GPIO_GetITStatusBit(uint32_t GPIO_Pin)
{  
  /* Check the parameters */
  assert_param(IS_GPIO_PIN(GPIO_Pin));
  
  if(READ_BIT(GPIO->RIS, GPIO_Pin) != (uint32_t)RESET) {
    return SET;
  }
  else {
    return RESET;
  }
}
/**
* @}
*/

/**
* @}
*/

/**
* @}
*/

/**
* @}
*/

/******************* (C) COPYRIGHT 2016 STMicroelectronics *****END OF FILE****/
