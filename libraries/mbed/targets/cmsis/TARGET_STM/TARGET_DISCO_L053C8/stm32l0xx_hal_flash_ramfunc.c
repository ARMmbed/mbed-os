/**
  ******************************************************************************
  * @file    stm32l0xx_hal_flash_ramfunc.c
  * @author  MCD Application Team
  * @version V1.1.0
  * @date    18-June-2014
  * @brief   FLASH RAMFUNC driver.
  *          This file provides a Flash firmware functions which should be 
  *          executed from internal SRAM
  *            + FLASH HalfPage Programming
  *            + FLASH Power Down in Run mode
  *
  *  @verbatim

    *** ARM Compiler ***
    --------------------
    [..] RAM functions are defined using the toolchain options. 
         Functions that are be executed in RAM should reside in a separate
         source module. Using the 'Options for File' dialog you can simply change
         the 'Code / Const' area of a module to a memory space in physical RAM.
         Available memory areas are declared in the 'Target' tab of the 
         Options for Target' dialog.

    *** ICCARM Compiler ***
    -----------------------
    [..] RAM functions are defined using a specific toolchain keyword "__ramfunc".

    *** GNU Compiler ***
    --------------------
    [..] RAM functions are defined using a specific toolchain attribute
         "__attribute__((section(".RamFunc")))".
  
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
/** @addtogroup STM32L0XX_HAL_Driver
  * @{
  */

/** @defgroup FLASHRamfunc Driver
  * @brief FLASH functions executed from RAM
  * @{
  */

#ifdef HAL_FLASH_MODULE_ENABLED

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
static __RAM_FUNC  FLASH_Program_HalfPage(uint32_t Address, uint32_t *Data);

/* Private functions ---------------------------------------------------------*/

/** @defgroup FLASHRamfunc_Private_Functions
  * @{
  */

/** @defgroup FLASHRamfunc_Group1 Peripheral  features functions 
 *  @brief   Data transfers functions 
 *
@verbatim   
 ===============================================================================
                      ##### ramfunc functions #####
 ===============================================================================  
    [..]
    This subsection provides a set of functions that should be executed from RAM 
    transfers.

@endverbatim
  * @{
  */

/**
  * @brief  Program a half page word at a specified address, 
  * @note   This function should be executed from RAM        
  * @param  Address:  specifies the address to be programmed, 
  *         the address should be half page aligned.
  * @param  *Data: specifies the buffer of data to be programmed,
  *                the size of the buffer is 16 words.
  * 
  * @retval HAL_StatusTypeDef HAL Status
  */
__RAM_FUNC  HAL_FLASHEx_HalfPageProgram(uint32_t Address, uint32_t *Data)
{
  HAL_StatusTypeDef status = HAL_OK;
  uint32_t timeout = 0xFFFFFFFF;
  /*Program word (32-bit) at a specified address.*/
  FLASH_Program_HalfPage(Address, Data);
  
  /* Wait for a FLASH operation to complete or a TIMEOUT to occur */
  while(((FLASH->SR & FLASH_FLAG_BSY) != 0x00) && (timeout != 0x00))
  {
    timeout--;
  }
  
  if(timeout == 0x00 )
    {
      return HAL_TIMEOUT;
    }
  
  /* Reset PROG bit */
  FLASH->PECR &= ~FLASH_PECR_PROG;
  FLASH->PECR &= ~FLASH_PECR_FPRG;
  return status;
}

/**
  * @brief   Enable the Power down in Run Mode
  * @note    This function should be called and executed from SRAM memory
  * @param  None
  * @retval None
  */
__RAM_FUNC  HAL_FLASHEx_EnableRunPowerDown(void)
{
  /* Enable the Power Down in Run mode*/
  __HAL_FLASH_POWER_DOWN_ENABLE();
  
  return HAL_OK;
  
}

/**
  * @brief   Disable the Power down in Run Mode
  * @note    This function should be called and executed from SRAM memory
  * @param  None
  * @retval None
  */
__RAM_FUNC  HAL_FLASHEx_DisableRunPowerDown(void)
{
  /* Disable the Power Down in Run mode*/
  __HAL_FLASH_POWER_DOWN_DISABLE();

  return HAL_OK;  
}

/**
  * @}
  */

/**
  * @brief  Program a half page in program memory.
  * @param  Address: The Half page address in program memory to be written.
  * @param  Data:
  * @retval None
  */
static __RAM_FUNC  FLASH_Program_HalfPage(uint32_t Address, uint32_t *Data)
{
  HAL_StatusTypeDef status = HAL_OK;
  uint32_t i =0;
  
  /* Set PROG bit */
  FLASH->PECR |= FLASH_PECR_PROG;
  
  /* Set FPRG bit */
  FLASH->PECR |= FLASH_PECR_FPRG;

  *(__IO uint32_t*)Address = Data[0];
  
  for(i = 1; i <= 15; i++)
  {  
    *(__IO uint32_t*)(Address + 4) = Data[i];
  } 
  return status;
}

/**
  * @}
  */   
#endif /* HAL_FLASH_MODULE_ENABLED */


  
/**
  * @}
  */

/**
  * @}
  */

     
/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/


