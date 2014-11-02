/**
  ******************************************************************************
  * @file    stm32l0xx_hal_cryp_ex.c
  * @author  MCD Application Team
  * @version V1.1.0
  * @date    18-June-2014
  * @brief   CRYPEx HAL module driver.
  *    
  *          This file provides firmware functions to manage the following 
  *          functionalities of the Cryptography (CRYP) extension peripheral:
  *           + Computation completed callback.
  *         
  @verbatim
  ==============================================================================
                     ##### How to use this driver #####
  ==============================================================================
    [..]
      The CRYP HAL driver can be used as follows:

      (#)Initialize the CRYP low level resources by implementing the HAL_CRYP_MspInit():
         (##) Enable the CRYP interface clock using __CRYP_CLK_ENABLE()
         (##) In case of using interrupts (e.g. HAL_AES_ECB_Encrypt_IT())
             (+) Configure the CRYP interrupt priority using HAL_NVIC_SetPriority()
             (+) Enable the CRYP IRQ handler using HAL_NVIC_EnableIRQ()
             (+) In CRYP IRQ handler, call HAL_CRYP_IRQHandler()
         (##) In case of using DMA to control data transfer (e.g. HAL_AES_ECB_Encrypt_DMA())
             (+) Enable the DMA1 interface clock using 
                 (++) __DMA1_CLK_ENABLE()
             (+) Configure and enable two DMA Channels one for managing data transfer from
                 memory to peripheral (input channel) and another channel for managing data
                 transfer from peripheral to memory (output channel)
             (+) Associate the initilalized DMA handle to the CRYP DMA handle
                 using  __HAL_LINKDMA()
             (+) Configure the priority and enable the NVIC for the transfer complete
                 interrupt on the two DMA Streams. The output stream should have higher
                 priority than the input stream.
                 (++) HAL_NVIC_SetPriority()
                 (++) HAL_NVIC_EnableIRQ()
    
      (#)Initialize the CRYP HAL using HAL_CRYP_Init(). This function configures mainly:
         (##) The data type: 1-bit, 8-bit, 16-bit and 32-bit
         (##) The encryption/decryption key.
         (##) The initialization vector (counter). It is not used ECB mode.
    
      (#)Three processing (encryption/decryption) functions are available:
         (##) Polling mode: encryption and decryption APIs are blocking functions
              i.e. they process the data and wait till the processing is finished
              e.g. HAL_CRYP_AESCBC_Encrypt()
         (##) Interrupt mode: encryption and decryption APIs are not blocking functions
              i.e. they process the data under interrupt
              e.g. HAL_CRYP_AESCBC_Encrypt_IT()
         (##) DMA mode: encryption and decryption APIs are not blocking functions
              i.e. the data transfer is ensured by DMA
              e.g. HAL_CRYP_AESCBC_Encrypt_DMA()
    
      (#)When the processing function is called at first time after HAL_CRYP_Init()
         the CRYP peripheral is initialized and processes the buffer in input.
         At second call, the processing function performs an append of the already
         processed buffer.
         When a new data block is to be processed, call HAL_CRYP_Init() then the
         processing function.
         
       (#)Call HAL_CRYP_DeInit() to deinitialize the CRYP peripheral.

  @endverbatim
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

/* Includes ------------------------------------------------------------------*/
#include "stm32l0xx_hal.h"

/** @addtogroup STM32L0xx_HAL_Driver
  * @{
  */

/** @defgroup CRYPEx 
  * @brief CRYP HAL Extended module driver.
  * @{
  */

#ifdef HAL_CRYP_MODULE_ENABLED
#if !defined (STM32L051xx) && !defined (STM32L052xx) && !defined (STM32L053xx)

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/** @defgroup CRYPEx_Private_Functions
  * @{
  */


/** @defgroup CRYPEX_Group1 Extended features functions 
 *  @brief    Extended features functions. 
 *
@verbatim   
 ===============================================================================
                 ##### Extended features functions #####
 =============================================================================== 
    [..]  This section provides callback functions:
      (+) Computation completed.

@endverbatim
  * @{
  */

/**
  * @brief  Computation completed callbacks.
  * @param  hcryp: pointer to a CRYP_HandleTypeDef structure that contains
  *         the configuration information for CRYP module
  * @retval None
  */
__weak void HAL_CRYPEx_ComputationCpltCallback(CRYP_HandleTypeDef *hcryp)
{
  /* NOTE : This function Should not be modified, when the callback is needed,
            the HAL_CRYP_ComputationCpltCallback could be implemented in the user file
   */ 
}

/**
  * @}
  */


/**
  * @}
  */
#endif /* STM32L051xx && STM32L052xx && STM32L053xx*/
#endif /* HAL_CRYP_MODULE_ENABLED */
/**
  * @}
  */

/**
  * @}
  */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
