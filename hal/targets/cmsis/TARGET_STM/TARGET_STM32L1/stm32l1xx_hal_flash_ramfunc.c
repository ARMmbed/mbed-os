/**
  ******************************************************************************
  * @file    stm32l1xx_hal_flash_ramfunc.c
  * @author  MCD Application Team
  * @version V1.1.3
  * @date    04-March-2016
  * @brief   FLASH RAMFUNC driver.
  *          This file provides a Flash firmware functions which should be 
  *          executed from internal SRAM
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
  * <h2><center>&copy; COPYRIGHT(c) 2016 STMicroelectronics</center></h2>
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
#include "stm32l1xx_hal.h"

/** @addtogroup STM32L1xx_HAL_Driver
  * @{
  */

#ifdef HAL_FLASH_MODULE_ENABLED

/** @defgroup FLASH_RAMFUNC FLASH_RAMFUNC
  * @brief FLASH functions executed from RAM
  * @{
  */ 


/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/** @defgroup FLASH_RAMFUNC_Private_Functions Private Functions
 * @{
 */

static __RAM_FUNC FLASHRAM_WaitForLastOperation(uint32_t Timeout);

/**
  * @}
  */

/* Private functions ---------------------------------------------------------*/
 
/** @defgroup FLASH_RAMFUNC_Exported_Functions Exported Functions
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

/** @defgroup FLASH_RAMFUNC_Exported_Functions_Group1 Peripheral features functions 
  * @{
  */  

/**
  * @brief  Enable  the power down mode during RUN mode.
  * @note  This function can be used only when the user code is running from Internal SRAM.
  * @retval None
  */
__RAM_FUNC HAL_FLASHEx_EnableRunPowerDown(void)
{
  /* Enable the Power Down in Run mode*/
  __HAL_FLASH_POWER_DOWN_ENABLE();
  
  return HAL_OK;
}


/**
  * @brief  Disable the power down mode during RUN mode.
  * @note  This function can be used only when the user code is running from Internal SRAM.
  * @retval None
  */
__RAM_FUNC HAL_FLASHEx_DisableRunPowerDown(void)
{
  /* Disable the Power Down in Run mode*/
  __HAL_FLASH_POWER_DOWN_DISABLE();

  return HAL_OK;  
}

/**
  * @}
  */

/** @defgroup FLASH_RAMFUNC_Exported_Functions_Group2 Programming and erasing operation functions 
 *
@verbatim  
@endverbatim
  * @{
  */

#if defined(FLASH_PECR_PARALLBANK)
/**
  * @brief  Erases a specified 2 page in program memory in parallel.
  * @note   This function can be used only for STM32L151xD, STM32L152xD), STM32L162xD and Cat5  devices.
  *         To correctly run this function, the HAL_FLASH_Unlock() function
  *         must be called before.
  *         Call the HAL_FLASH_Lock() to disable the flash memory access 
  *        (recommended to protect the FLASH memory against possible unwanted operation).
  * @param  Page_Address1: The page address in program memory to be erased in 
  *         the first Bank (BANK1). This parameter should be between FLASH_BASE
  *         and FLASH_BANK1_END.
  * @param  Page_Address2: The page address in program memory to be erased in 
  *         the second Bank (BANK2). This parameter should be between FLASH_BANK2_BASE
  *         and FLASH_BANK2_END.
  * @note   A Page is erased in the Program memory only if the address to load 
  *         is the start address of a page (multiple of 256 bytes).
  * @retval HAL Status: The returned value can be: 
  *          HAL_ERROR, HAL_OK or HAL_TIMEOUT.
  */
__RAM_FUNC HAL_FLASHEx_EraseParallelPage(uint32_t Page_Address1, uint32_t Page_Address2)
{
  HAL_StatusTypeDef status = HAL_OK;

  /* Wait for last operation to be completed */
  status = FLASHRAM_WaitForLastOperation(FLASH_TIMEOUT_VALUE);
  
  if(status == HAL_OK)
  {
    /* If the previous operation is completed, proceed to erase the page */

    /* Set the PARALLBANK bit */
    FLASH->PECR |= FLASH_PECR_PARALLBANK;
    
    /* Set the ERASE bit */
    FLASH->PECR |= FLASH_PECR_ERASE;

    /* Set PROG bit */
    FLASH->PECR |= FLASH_PECR_PROG;
  
    /* Write 00000000h to the first word of the first program page to erase */
    *(__IO uint32_t *)Page_Address1 = 0x00000000;
    /* Write 00000000h to the first word of the second program page to erase */    
    *(__IO uint32_t *)Page_Address2 = 0x00000000;    
 
    /* Wait for last operation to be completed */
    status = FLASHRAM_WaitForLastOperation(FLASH_TIMEOUT_VALUE);

    /* If the erase operation is completed, disable the ERASE, PROG and PARALLBANK bits */
    FLASH->PECR &= (uint32_t)(~FLASH_PECR_PROG);
    FLASH->PECR &= (uint32_t)(~FLASH_PECR_ERASE);
    FLASH->PECR &= (uint32_t)(~FLASH_PECR_PARALLBANK);   
  }     
  /* Return the Erase Status */
  return status;
}

/**
  * @brief  Programs 2 half page in program memory in parallel.
  * @note   This function can be used only for STM32L151xD, STM32L152xD), STM32L162xD and Cat5  devices.
  * @param  Address1: specifies the first address to be written in the first bank 
  *        (BANK1). This parameter should be between FLASH_BASE and (FLASH_BANK1_END - FLASH_PAGE_SIZE).
  * @param  pBuffer1: pointer to the buffer  containing the data to be  written 
  *         to the first half page in the first bank.
  * @param  Address2: specifies the second address to be written in the second bank
  *        (BANK2). This parameter should be between FLASH_BANK2_BASE and (FLASH_BANK2_END - FLASH_PAGE_SIZE).
  * @param  pBuffer2: pointer to the buffer containing the data to be  written 
  *         to the second half page in the second bank.
  * @note   To correctly run this function, the HAL_FLASH_Unlock() function
  *         must be called before.
  *         Call the HAL_FLASH_Lock() to disable the flash memory access  
  *         (recommended to protect the FLASH memory against possible unwanted operation).
  * @note   Half page write is possible only from SRAM.
  * @note   If there are more than 32 words to write, after 32 words another 
  *         Half Page programming operation starts and has to be finished.
  * @note   A half page is written to the program memory only if the first 
  *         address to load is the start address of a half page (multiple of 128 
  *         bytes) and the 31 remaining words to load are in the same half page.
  * @note   During the Program memory half page write all read operations are 
  *         forbidden (this includes DMA read operations and debugger read 
  *         operations such as breakpoints, periodic updates, etc.).
  * @note   If a PGAERR is set during a Program memory half page write, the 
  *         complete write operation is aborted. Software should then reset the 
  *         FPRG and PROG/DATA bits and restart the write operation from the 
  *         beginning.
  * @retval HAL Status: The returned value can be:  
  *          HAL_ERROR, HAL_OK or HAL_TIMEOUT.
  */
__RAM_FUNC HAL_FLASHEx_ProgramParallelHalfPage(uint32_t Address1, uint32_t* pBuffer1, uint32_t Address2, uint32_t* pBuffer2)
{
  uint32_t count = 0; 
   
  HAL_StatusTypeDef status = HAL_OK;

  /* Set the DISMCYCINT[0] bit in the Auxillary Control Register (0xE000E008) 
     This bit prevents the interruption of multicycle instructions and therefore 
     will increase the interrupt latency. of Cortex-M3. */
  SCnSCB->ACTLR |= SCnSCB_ACTLR_DISMCYCINT_Msk;

  /* Wait for last operation to be completed */
  status = FLASHRAM_WaitForLastOperation(FLASH_TIMEOUT_VALUE);
  
  if(status == HAL_OK)
  {
    /* If the previous operation is completed, proceed to program the new  
       half page */
    FLASH->PECR |= FLASH_PECR_PARALLBANK;
    FLASH->PECR |= FLASH_PECR_FPRG;
    FLASH->PECR |= FLASH_PECR_PROG;
    
  /* Wait for last operation to be completed */
  status = FLASHRAM_WaitForLastOperation(FLASH_TIMEOUT_VALUE);
  if(status == HAL_OK)
  {
    /* Write the first half page directly with 32 different words */
    while(count < 32)
    {
      *(__IO uint32_t*) ((uint32_t)(Address1 + (4 * count))) = *(pBuffer1++);
      count ++;  
    }
    count = 0;

    /* Write the second half page directly with 32 different words */
    while(count < 32)
    {
      *(__IO uint32_t*) ((uint32_t)(Address2 + (4 * count))) = *(pBuffer2++);
      count ++;  
    }
    /* Wait for last operation to be completed */
    status = FLASHRAM_WaitForLastOperation(FLASH_TIMEOUT_VALUE);
  } 
    /* if the write operation is completed, disable the PROG, FPRG and PARALLBANK bits */
    FLASH->PECR &= (uint32_t)(~FLASH_PECR_PROG);
    FLASH->PECR &= (uint32_t)(~FLASH_PECR_FPRG);
    FLASH->PECR &= (uint32_t)(~FLASH_PECR_PARALLBANK);
  }

  SCnSCB->ACTLR &= ~SCnSCB_ACTLR_DISMCYCINT_Msk;
    
  /* Return the Write Status */
  return status;
}
#endif /* FLASH_PECR_PARALLBANK */

/**
  * @brief  Programs a half page in program memory.
  * @param  Address: specifies the address to be written.
  * @param  pBuffer: pointer to the buffer  containing the data to be  written to 
  *         the half page.
  * @note   To correctly run this function, the HAL_FLASH_Unlock() function
  *         must be called before.
  *         Call the HAL_FLASH_Lock() to disable the flash memory access  
  *         (recommended to protect the FLASH memory against possible unwanted operation)
  * @note   Half page write is possible only from SRAM.
  * @note   If there are more than 32 words to write, after 32 words another 
  *         Half Page programming operation starts and has to be finished.
  * @note   A half page is written to the program memory only if the first 
  *         address to load is the start address of a half page (multiple of 128 
  *         bytes) and the 31 remaining words to load are in the same half page.
  * @note   During the Program memory half page write all read operations are 
  *         forbidden (this includes DMA read operations and debugger read 
  *         operations such as breakpoints, periodic updates, etc.).
  * @note   If a PGAERR is set during a Program memory half page write, the 
  *         complete write operation is aborted. Software should then reset the 
  *         FPRG and PROG/DATA bits and restart the write operation from the 
  *         beginning.
  * @retval HAL Status: The returned value can be:  
  *    HAL_ERROR, HAL_OK or HAL_TIMEOUT. 
  */
__RAM_FUNC HAL_FLASHEx_HalfPageProgram(uint32_t Address, uint32_t* pBuffer)
{
  uint32_t count = 0; 
   
  HAL_StatusTypeDef status = HAL_OK;

  /* Set the DISMCYCINT[0] bit in the Auxillary Control Register (0xE000E008) 
     This bit prevents the interruption of multicycle instructions and therefore 
     will increase the interrupt latency. of Cortex-M3. */
  SCnSCB->ACTLR |= SCnSCB_ACTLR_DISMCYCINT_Msk;
  
  /* Wait for last operation to be completed */
  status = FLASHRAM_WaitForLastOperation(FLASH_TIMEOUT_VALUE);
  
  if(status == HAL_OK)
  {
    /* if the previous operation is completed, proceed to program the new  
    half page */
    FLASH->PECR |= FLASH_PECR_FPRG;
    FLASH->PECR |= FLASH_PECR_PROG;
    
    /* Write one half page directly with 32 different words */
    while(count < 32)
    {
      *(__IO uint32_t*) ((uint32_t)(Address + (4 * count))) = *(pBuffer++);
      count ++;  
    }
    /* Wait for last operation to be completed */
    status = FLASHRAM_WaitForLastOperation(FLASH_TIMEOUT_VALUE);
 
    /* if the write operation is completed, disable the PROG and FPRG bits */
    FLASH->PECR &= (uint32_t)(~FLASH_PECR_PROG);
    FLASH->PECR &= (uint32_t)(~FLASH_PECR_FPRG);
  }

  SCnSCB->ACTLR &= ~SCnSCB_ACTLR_DISMCYCINT_Msk;
    
  /* Return the Write Status */
  return status;
}

/**
  * @}
  */

/** @defgroup FLASH_RAMFUNC_Exported_Functions_Group3 DATA EEPROM functions
  *
  * @{
  */

/**
  * @brief  Erase a double word in data memory.
  * @param  Address: specifies the address to be erased.
  * @note   To correctly run this function, the HAL_FLASH_EEPROM_Unlock() function
  *         must be called before.
  *         Call the HAL_FLASH_EEPROM_Lock() to he data EEPROM access
  *         and Flash program erase control register access(recommended to protect 
  *         the DATA_EEPROM against possible unwanted operation).
  * @note   Data memory double word erase is possible only from SRAM.
  * @note   A double word is erased to the data memory only if the first address 
  *         to load is the start address of a double word (multiple of 8 bytes).
  * @note   During the Data memory double word erase, all read operations are 
  *         forbidden (this includes DMA read operations and debugger read 
  *         operations such as breakpoints, periodic updates, etc.).
  * @retval HAL Status: The returned value can be: 
  *    HAL_ERROR, HAL_OK or HAL_TIMEOUT.
  */

__RAM_FUNC HAL_FLASHEx_DATAEEPROM_EraseDoubleWord(uint32_t Address)
{
  HAL_StatusTypeDef status = HAL_OK;
  
  /* Set the DISMCYCINT[0] bit in the Auxillary Control Register (0xE000E008) 
     This bit prevents the interruption of multicycle instructions and therefore 
     will increase the interrupt latency. of Cortex-M3. */
  SCnSCB->ACTLR |= SCnSCB_ACTLR_DISMCYCINT_Msk;
    
  /* Wait for last operation to be completed */
  status = FLASHRAM_WaitForLastOperation(FLASH_TIMEOUT_VALUE);
  
  if(status == HAL_OK)
  {
    /* If the previous operation is completed, proceed to erase the next double word */
    /* Set the ERASE bit */
    FLASH->PECR |= FLASH_PECR_ERASE;

    /* Set DATA bit */
    FLASH->PECR |= FLASH_PECR_DATA;
   
    /* Write 00000000h to the 2 words to erase */
    *(__IO uint32_t *)Address = 0x00000000;
    Address += 4;
    *(__IO uint32_t *)Address = 0x00000000;
   
    /* Wait for last operation to be completed */
    status = FLASHRAM_WaitForLastOperation(FLASH_TIMEOUT_VALUE);
    
    /* If the erase operation is completed, disable the ERASE and DATA bits */
    FLASH->PECR &= (uint32_t)(~FLASH_PECR_ERASE);
    FLASH->PECR &= (uint32_t)(~FLASH_PECR_DATA);
  }  
  
  SCnSCB->ACTLR &= ~SCnSCB_ACTLR_DISMCYCINT_Msk;
    
  /* Return the erase status */
  return status;
}

/**
  * @brief  Write a double word in data memory without erase.
  * @param  Address: specifies the address to be written.
  * @param  Data: specifies the data to be written.
  * @note   To correctly run this function, the HAL_FLASH_EEPROM_Unlock() function
  *         must be called before.
  *         Call the HAL_FLASH_EEPROM_Lock() to he data EEPROM access
  *         and Flash program erase control register access(recommended to protect 
  *         the DATA_EEPROM against possible unwanted operation).
  * @note   Data memory double word write is possible only from SRAM.
  * @note   A data memory double word is written to the data memory only if the 
  *         first address to load is the start address of a double word (multiple 
  *         of double word).
  * @note   During the Data memory double word write, all read operations are 
  *         forbidden (this includes DMA read operations and debugger read 
  *         operations such as breakpoints, periodic updates, etc.).
  * @retval HAL Status: The returned value can be: 
  *    HAL_ERROR, HAL_OK or HAL_TIMEOUT. 
  */ 
__RAM_FUNC HAL_FLASHEx_DATAEEPROM_ProgramDoubleWord(uint32_t Address, uint64_t Data)
{
  HAL_StatusTypeDef status = HAL_OK;

  /* Set the DISMCYCINT[0] bit in the Auxillary Control Register (0xE000E008) 
     This bit prevents the interruption of multicycle instructions and therefore 
     will increase the interrupt latency. of Cortex-M3. */
  SCnSCB->ACTLR |= SCnSCB_ACTLR_DISMCYCINT_Msk;
    
  /* Wait for last operation to be completed */
  status = FLASHRAM_WaitForLastOperation(FLASH_TIMEOUT_VALUE);
  
  if(status == HAL_OK)
  {
    /* If the previous operation is completed, proceed to program the new data*/
    FLASH->PECR |= FLASH_PECR_FPRG;
    FLASH->PECR |= FLASH_PECR_DATA;
    
    /* Write the 2 words */  
     *(__IO uint32_t *)Address = (uint32_t) Data;
     Address += 4;
     *(__IO uint32_t *)Address = (uint32_t) (Data >> 32);
    
    /* Wait for last operation to be completed */
    status = FLASHRAM_WaitForLastOperation(FLASH_TIMEOUT_VALUE);
    
    /* If the write operation is completed, disable the FPRG and DATA bits */
    FLASH->PECR &= (uint32_t)(~FLASH_PECR_FPRG);
    FLASH->PECR &= (uint32_t)(~FLASH_PECR_DATA);     
  }
  
  SCnSCB->ACTLR &= ~SCnSCB_ACTLR_DISMCYCINT_Msk;
    
  /* Return the Write Status */
  return status;
}

/**
  * @}
  */

/**
  * @}
  */

/** @addtogroup FLASH_RAMFUNC_Private_Functions
  * @{
  */ 

/**
  * @brief  Wait for a FLASH operation to complete.
  * @param  Timeout: maximum flash operationtimeout
  * @retval HAL status
  */
static __RAM_FUNC   FLASHRAM_WaitForLastOperation(uint32_t Timeout)
{ 
    /* Wait for the FLASH operation to complete by polling on BUSY flag to be reset.
       Even if the FLASH operation fails, the BUSY flag will be reset and an error
       flag will be set */
       
    while(__HAL_FLASH_GET_FLAG(FLASH_FLAG_BSY) && (Timeout != 0x00)) 
    { 
      Timeout--;
    }
    
    if(Timeout == 0x00 )
    {
      return HAL_TIMEOUT;
    }
    
    /* Check FLASH End of Operation flag  */
    if (__HAL_FLASH_GET_FLAG(FLASH_FLAG_EOP))
    {
      /* Clear FLASH End of Operation pending bit */
      __HAL_FLASH_CLEAR_FLAG(FLASH_FLAG_EOP);
    }
  
    if( (__HAL_FLASH_GET_FLAG(FLASH_FLAG_WRPERR)     != RESET) || 
        (__HAL_FLASH_GET_FLAG(FLASH_FLAG_PGAERR)     != RESET) || 
        (__HAL_FLASH_GET_FLAG(FLASH_FLAG_SIZERR)     != RESET) || 
#if defined(FLASH_SR_RDERR)
        (__HAL_FLASH_GET_FLAG(FLASH_FLAG_RDERR)      != RESET) || 
#endif /* FLASH_SR_RDERR */
#if defined(FLASH_SR_OPTVERRUSR)
        (__HAL_FLASH_GET_FLAG(FLASH_FLAG_OPTVERRUSR) != RESET) || 
#endif /* FLASH_SR_OPTVERRUSR */
        (__HAL_FLASH_GET_FLAG(FLASH_FLAG_OPTVERR)    != RESET) )
    {
      return HAL_ERROR;
    }
  
    /* If there is an error flag set */
    return HAL_OK;
}

  
/**
  * @}
  */

/**
  * @}
  */

#endif /* HAL_FLASH_MODULE_ENABLED */
/**
  * @}
  */

     
/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
