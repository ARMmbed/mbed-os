/**
  ******************************************************************************
  * @file    BlueNRG1_RNG.c
  * @author  VMA Application Team
  * @version V2.0.0
  * @date    21-March-2016
  * @brief   This file provides all the RNG firmware functions.
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
#include "../../Periph_Driver/inc/BlueNRG1_rng.h"


/** @addtogroup BLUENRG1_Peripheral_Driver BLUENRG1 Peripheral Driver
  * @{
  */

/** @defgroup RNG_Peripheral  RNG Peripheral
  * @{
  */

/** @defgroup RNG_Private_TypesDefinitions Private Types Definitions
  * @{
  */

/**
  * @}
  */

/** @defgroup RNG_Private_Defines Private Defines
  * @{
  */


/**
  * @}
  */ 

/** @defgroup RNG_Private_Macros Private Macros
  * @{
  */

/**
  * @}
  */

/** @defgroup RNG_Private_Variables Private Variables
  * @{
  */

/**
  * @}
  */

/** @defgroup RNG_Private_FunctionPrototypes Private Function Prototypes
  * @{
  */

/**
  * @}
  */

/** @defgroup RNG_Private_Functions Private Functions
  * @{
  */

/**
  * @}
  */
	
/** @defgroup RNG_Public_Functions Public Functions
* @{
*/

/**
  * @brief  Enables or disables the specified RNG peripheral.
  * @param  NewState: functional state @ref FunctionalState
  *         This parameter can be: ENABLE or DISABLE.
  * @retval None
  */
void RNG_Cmd(FunctionalState NewState)
{
  /* Check the parameters */
  assert_param(IS_FUNCTIONAL_STATE(NewState));
  
  if (NewState != DISABLE) {
    /* Disable RNG peripheral */
    RNG->CR_b.DIS = RESET;
  }
  else {
    /* Enable RNG peripheral */
    RNG->CR_b.DIS = SET;
  }
}


/**
  * @brief  Checks if the RNG flag is set or not.
* @retval FlagStatus: functional state @ref FlagStatus
*         This parameter can be: SET or RESET.
  */
FlagStatus RNG_GetFlagStatus(void)
{  
  if (RNG->SR_b.RDY == SET) {
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

/******************* (C) COPYRIGHT 2016 STMicroelectronics *****END OF FILE****/
