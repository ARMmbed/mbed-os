/**
******************************************************************************
* @file    BlueNRG1_flash.c
* @author  VMA Application Team
* @version V2.0.1
* @date    21-March-2018
* @brief   This file provides all the FLASH firmware functions.
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
#include "../../Periph_Driver/inc/BlueNRG1_flash.h"

/** @addtogroup BLUENRG1_Peripheral_Driver BLUENRG1 Peripheral Driver
* @{
*/

/** @defgroup FLASH_Peripheral  FLASH Peripheral
* @{
*/ 

/** @defgroup FLASH_Private_TypesDefinitions Private Types Definitions
* @{
*/

/**
* @}
*/ 

/** @defgroup FLASH_Private_Defines Private Defines
* @{
*/ 

/**
* @}
*/ 

/** @defgroup FLASH_Private_Macros Private Macros
* @{
*/


/**
* @}
*/ 

/** @defgroup FLASH_Private_Variables Private Variables
* @{
*/

/**
* @}
*/ 

/** @defgroup FLASH_Private_Functions Private Functions 
* @{
*/

/**
* @}
*/

/** @defgroup FLASH_Public_Functions Public Functions
* @{
*/



/**
* @brief  Erases a specified flash page.
* @param  PageNumber: the page number.
* @retval None
*/
void FLASH_ErasePage(uint16_t PageNumber)
{
  uint16_t pageAddress;
  
  assert_param(IS_PAGE_NUMBER(PageNumber));

  /* Lock word to avoid undesired FLASH operation */
  if (flash_sw_lock != FLASH_UNLOCK_WORD)
    return;
  
  pageAddress = (PageNumber * N_BYTES_PAGE)>>2;
  
  /* Clear IRQ */
  FLASH->IRQSTAT = 0x3F;
  
  /* Write the page address */
  FLASH->ADDRESS = pageAddress;
  
  /* Write the ERASE command */
  FLASH->COMMAND = FLASH_ERASE;
  
  FLASH_WaitCmdDone();
  
  /* Verify */
  // return;
}

/**
* @brief  Erases all main Flash.
* @param  None
* @retval None
*/
void FLASH_EraseAllFlash(void)
{
  /* Lock word to avoid undesired FLASH operation */
  if (flash_sw_lock != FLASH_UNLOCK_WORD)
    return;

  /* Clear IRQ */
  FLASH->IRQSTAT = 0x3F;
  
  /* Write the MASSERASE command */
  FLASH->COMMAND = FLASH_MASSERASE;
  
  FLASH_WaitCmdDone();
  
}

/**
* @brief  Flash read 32 bits.
* @param  Address: address to read.
          Address must be word aligned.
* @retval Data read
*/
uint32_t FLASH_ReadWord(uint32_t Address)
{
  assert_param(IS_FLASH_ADDRESS(Address));
	
  return (*((volatile uint32_t*)(Address)));
}

/**
* @brief  Flash read 8 bits.
* @param  Address: address to write.
* @retval Data read
*/
uint8_t FLASH_ReadByte(uint32_t Address)
{
  assert_param(IS_FLASH_ADDRESS(Address));
	
  return (*((volatile uint8_t*)(Address)));
}

/**
* @brief  Flash write 32 bits.
* @param  Address: address to write
* @param  Data: word to write
* @retval None
*/
void FLASH_ProgramWord(uint32_t Address, uint32_t Data)
{
  assert_param(IS_FLASH_ADDRESS(Address));

  /* Lock word to avoid undesired FLASH operation */
  if (flash_sw_lock != FLASH_UNLOCK_WORD)
    return;

  /* Clear IRQ */
  FLASH->IRQSTAT = 0x3F;
  
  /* Write the page address */
  FLASH->ADDRESS = Address>>2;
  
  /* Write the data to program */
  FLASH->DATA0 = Data;
  
  /* Write the WRITE command */
  FLASH->COMMAND = FLASH_WRITE;
  
  FLASH_WaitCmdDone();
  
  /* Verify */  
  // return ;
}

/**
* @brief  Flash write 32 bits.
* @param  Address: address to write
* @param  Data: pointer to an array of 4 words to write
* @retval None
*/
void FLASH_ProgramWordBurst(uint32_t Address, uint32_t* Data)
{
  assert_param(IS_FLASH_ADDRESS(Address));

  /* Lock word to avoid undesired FLASH operation */
  if (flash_sw_lock != FLASH_UNLOCK_WORD)
    return;

  /* Clear IRQ */
  FLASH->IRQSTAT = 0x3F;
  
  /* Write the page address */
  FLASH->ADDRESS = Address>>2;
  
  /* Write the data to program */
  FLASH->DATA0 = Data[0];
  FLASH->DATA1 = Data[1];
  FLASH->DATA2 = Data[2];
  FLASH->DATA3 = Data[3];
  
  /* Write the BURSTWRITE command */
  FLASH->COMMAND = FLASH_BURSTWRITE;
  
  FLASH_WaitCmdDone();
  
  /* Verify */  
  // return ;
}

/**
* @brief  Flash Lock
* @param  None
* @retval None
*/
void FLASH_Lock(void)
{
  flash_sw_lock = FLASH_LOCK_WORD;
}

/**
* @brief  Flash Unlock
* @param  None
* @retval None
*/
void FLASH_Unlock(void)
{
  flash_sw_lock = FLASH_UNLOCK_WORD;
}

/**
* @brief  Wait loop for CMDDONE status
* @param  None
* @retval None
*/
void FLASH_WaitCmdDone(void)
{
  /* Waits for Command Done */
  while(FLASH->IRQRAW_b.CMDDONE != SET);
}


/**
* @brief  Clear CMDDONE flag
* @param  None
* @retval None
*/
void FLASH_ClearCmdDone(void)
{
  /* Clear the CMDDONE flag */
  SET_BIT(FLASH->IRQSTAT, Flash_CMDDONE);
}



/**
* @brief  Enables or disables the specified flash interrupts.
* @param  FlashFlag: the value can be 
*         @arg Flash_CMDDONE Flash command done
*         @arg Flash_CMDSTART Flash command start
*         @arg Flash_CMDERR Flash command error
*         @arg Flash_ILLCMD Flash illegal command written
*         @arg Flash_READOK Flash mass read was OK  
*         @arg Flash_FLNREADY Flash not ready (sleep)
* @param  NewState: functional state @ref FunctionalState
*   This parameter can be: ENABLE or DISABLE.
* @retval None 
*/
void FLASH_ITConfig(uint8_t FlashFlag, FunctionalState NewState)
{
  /* Check the parameters */
  assert_param(IS_FLASH_FLAG(FlashFlag)); 
  assert_param(IS_FUNCTIONAL_STATE(NewState));
  
  if (NewState == ENABLE) {
    SET_BIT(FLASH->IRQMASK, FlashFlag);
  }
  else if (NewState == DISABLE) {
    CLEAR_BIT(FLASH->IRQMASK, FlashFlag);
  }
}


/**
* @brief  Clears the specific flash pending IT bit.
* @param  FlashFlag: the FLASH flag to clear, this can be
*         @arg Flash_CMDDONE Flash command done
*         @arg Flash_CMDSTART Flash command start
*         @arg Flash_CMDERR Flash command error
*         @arg Flash_ILLCMD Flash illegal command written
*         @arg Flash_READOK Flash mass read was OK  
*         @arg Flash_FLNREADY Flash not ready (sleep)
* @retval None
*/
void FLASH_ClearITPendingBit(uint8_t FlashFlag)
{
  assert_param(IS_FLASH_FLAG(FlashFlag)) ;
  
  /* Clear the flags */
  FLASH->IRQSTAT = FlashFlag;
}


/**
* @brief  Checks whether the specified FLASH raw flag is set or not.
* @param  FlashFlag: the FLASH flag to check, this can be
*         @arg Flash_CMDDONE Flash command done
*         @arg Flash_CMDSTART Flash command start
*         @arg Flash_CMDERR Flash command error
*         @arg Flash_ILLCMD Flash illegal command written
*         @arg Flash_READOK Flash mass read was OK  
*         @arg Flash_FLNREADY Flash not ready (sleep)
* @retval FlagStatus: functional state @ref FlagStatus
*         This parameter can be: SET or RESET.
*/
FlagStatus FLASH_GetFlagStatus(uint8_t FlashFlag)
{
  /* Check the parameters */
  assert_param(IS_FLASH_FLAG(FlashFlag));
  
  if((FLASH->IRQRAW & FlashFlag) != (uint16_t)RESET) {
    return SET;
  }
  else {
    return RESET;
  }
}

/**
* @brief  Clears the specific flash pending flags.
* @param  FlashFlag: the FLASH flag to clear, this can be
*         @arg Flash_CMDDONE Flash command done
*         @arg Flash_CMDSTART Flash command start
*         @arg Flash_CMDERR Flash command error
*         @arg Flash_ILLCMD Flash illegal command written
*         @arg Flash_READOK Flash mass read was OK  
*         @arg Flash_FLNREADY Flash not ready (sleep)
* @retval None
*/
void FLASH_ClearFlag(uint8_t FlashFlag)
{
  assert_param(IS_FLASH_FLAG(FlashFlag)) ;
  
  /* Clear the flags */
  FLASH->IRQSTAT = FlashFlag;
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
