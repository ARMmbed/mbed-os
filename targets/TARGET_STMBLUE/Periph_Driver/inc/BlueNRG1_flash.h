/**
  ******************************************************************************
  * @file    BlueNRG1_flash.h
  * @author  VMA Application Team
  * @version V2.1.1
  * @date    31-January-2017
  * @brief   This file contains all the functions prototypes for the FLASH 
  *          firmware library.
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

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef BLUENRG1_FLASH_H
#define BLUENRG1_FLASH_H

#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "bluenrg_x_device.h"

/** @addtogroup BLUENRG1_Peripheral_Driver BLUENRG1 Peripheral Driver
  * @{
  */

/** @addtogroup FLASH_Peripheral  FLASH Peripheral
  * @{
  */

/** @defgroup FLASH_Exported_Types Exported Types
  * @{
  */

/** 
  * @brief  Flash status enumeration
  */
  
#define Flash_CMDDONE     0x01  /* Flash command done */
#define Flash_CMDSTART    0x02  /* Flash command start */
#define Flash_CMDERR      0x04  /* Flash command error */
#define Flash_ILLCMD      0x08  /* Flash illegal command written */
#define Flash_READOK      0x10  /* Flash mass read was OK */
#define Flash_FLNREADY    0x20  /* Flash not ready (sleep) */

#define IS_FLASH_FLAG(FLAG) ( ((FLAG) == Flash_CMDDONE) || \
                              ((FLAG) == Flash_CMDSTART) || \
                              ((FLAG) == Flash_CMDERR) || \
                              ((FLAG) == Flash_ILLCMD) || \
                              ((FLAG) == Flash_READOK) || \
                              ((FLAG) == Flash_FLNREADY) )
/**
  * @}
  */

/** @defgroup FLASH_Exported_Constants Exported Constants
  * @{
  */

/** @defgroup FLASH_Size_Definitions Size Definitions
  * @{
  */

#define FLASH_START             (_MEMORY_FLASH_BEGIN_)
#define FLASH_END               (_MEMORY_FLASH_END_)

#define IS_FLASH_ADDRESS(ADDR) ( ((ADDR) >= FLASH_START) && ((ADDR) <= FLASH_END) )

#define N_BYTES_WORD  (4)
#define N_ROWS        (8)
#define N_BYTES_PAGE  (_MEMORY_BYTES_PER_PAGE_)
#define N_PAGES       (_MEMORY_FLASH_SIZE_/N_BYTES_PAGE)
#define N_WORDS_ROW   (N_BYTES_PAGE /  N_ROWS / N_BYTES_WORD)
#define N_WORDS_PAGE  (N_BYTES_PAGE / N_BYTES_WORD)

#define IS_PAGE_NUMBER(PAGE)  ( (PAGE)<N_PAGES )
#define IS_ROW_NUMBER(ROW)    ( (ROW)<N_ROWS )
#define IS_WORD_NUMBER(WORD)  ( (WORD)<N_WORDS_PAGE )

/* Remap values for the vector table. CONFIG register */
#define FLASH_REMAP_RAM      0x02
#define FLASH_PREMAP_MAIN    0x08


/**
  * @}
  */

  
/** @defgroup FLASH_Commands_Definitions  Commands Definitions
  * @{
  */

#define FLASH_ERASE           0x11
#define FLASH_MASSERASE       0x22
#define FLASH_WRITE           0x33
#define FLASH_MASSWRITE       0x44
#define FLASH_MASSREAD        0x55
#define FLASH_SLEEP           0xAA
#define FLASH_WAKEUP          0xBB
#define FLASH_BURSTWRITE      0xCC

#define IS_FLASH_CMD(CMD)   ( ((CMD)==FLASH_ERASE)    || ((CMD)==FLASH_MASSERASE) \
                              ((CMD)==FLASH_WRITE)    || ((CMD)==FLASH_MASSWRITE) \
                              ((CMD)==FLASH_MASSREAD) || ((CMD)==FLASH_BURSTWRITE) \
                              ((CMD)==FLASH_SLEEP)    || ((CMD)==FLASH_WAKEUP) )

/**
  * @}
  */

/**
  * @}
  */

/** @defgroup FLASH_Exported_Macros Exported Macros
  * @{
  */

/**
  * @}
  */

/** @defgroup FLASH_Exported_Functions Exported Functions
  * @{
  */

void FLASH_ErasePage(uint16_t PageNumber);
void FLASH_EraseAllFlash(void);
uint32_t FLASH_ReadWord(uint32_t Address);
uint8_t FLASH_ReadByte(uint32_t Address);
void FLASH_ProgramWord(uint32_t Address, uint32_t Data);
void FLASH_ProgramWordBurst(uint32_t Address, uint32_t* Data);
void FLASH_Lock(void);
void FLASH_Unlock(void);
void FLASH_WaitCmdDone(void);
void FLASH_ITConfig(uint8_t FlashFlag, FunctionalState NewState);
void FLASH_ClearITPendingBit(uint8_t FlashFlag);
FlagStatus FLASH_GetFlagStatus(uint8_t FlashFlag);
void FLASH_ClearFlag(uint8_t FlashFlag);

/**
  * @}
  */

/**
  * @}
  */

/**
  * @}
  */


#ifdef __cplusplus
}
#endif

#endif /* BLUENRG1_FLASH_H */

/******************* (C) COPYRIGHT 2016 STMicroelectronics *****END OF FILE****/
