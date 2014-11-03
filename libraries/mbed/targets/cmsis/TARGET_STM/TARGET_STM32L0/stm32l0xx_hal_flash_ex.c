/**
  ******************************************************************************
  * @file    stm32l0xx_hal_flash_ex.c
  * @author  MCD Application Team
  * @version V1.1.0
  * @date    18-June-2014
  * @brief   FLASH HAL module driver.
  *          This file provides firmware functions to manage the following 
  *          functionalities of the internal FLASH memory:
  *            + FLASH Interface configuration
  *            + FLASH Memory Programming
  *            + DATA EEPROM Programming
  *            + Option Bytes Programming
  *            + Interrupts and flags management
  *
  *  @verbatim
  ==============================================================================
               ##### Flash peripheral Extended features  #####
  ==============================================================================
           
  [..] Comparing to other products, the FLASH interface for STM32L0xx
       devices contains the following additional features        
       (+) DATA_EEPROM memory management
       (+) BOOT option bit configuration       
       (+) PCROP protection for all sectors
   
                      ##### How to use this driver #####
  ==============================================================================
  [..] This driver provides functions to configure and program the FLASH memory 
       of all STM32L0xx. It includes:       
       (+) Full DATA_EEPROM erase and program management
       (+) Boot activation
       (+) PCROP protection configuration and control for all pages
  
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

/** @defgroup FLASHEx HAL FLASH Driver
  * @brief FLASH HAL Extension module driver
  * @{
  */

#ifdef HAL_FLASH_MODULE_ENABLED

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
#define HAL_FLASH_TIMEOUT_VALUE   ((uint32_t)0x50000
#define WRP01_MASK                ((uint32_t)0x0000FFFF)
#define PAGESIZE                  ((uint32_t)0x00000080)

/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
static HAL_StatusTypeDef FLASH_OB_PCROPConfig(uint32_t OB_WRP, FunctionalState NewState);
static HAL_StatusTypeDef FLASH_OB_PCROPSelectionConfig(uint16_t OB_PcROP);
static HAL_StatusTypeDef FLASH_OB_BootConfig(uint16_t OB_BOOT);
static HAL_StatusTypeDef FLASH_DATAEEPROM_FastProgramByte(uint32_t Address, uint8_t Data);
static HAL_StatusTypeDef FLASH_DATAEEPROM_FastProgramHalfWord(uint32_t Address, uint16_t Data);
static HAL_StatusTypeDef FLASH_DATAEEPROM_FastProgramWord(uint32_t Address, uint32_t Data);
static HAL_StatusTypeDef FLASH_DATAEEPROM_ProgramByte(uint32_t Address, uint8_t Data);
static HAL_StatusTypeDef FLASH_DATAEEPROM_ProgramHalfWord(uint32_t Address, uint16_t Data);
static HAL_StatusTypeDef FLASH_DATAEEPROM_ProgramWord(uint32_t Address, uint32_t Data);
static uint8_t FLASH_OB_GetUser(void);
static uint32_t FLASH_OB_GetWRP(void);
static FlagStatus FLASH_OB_GetRDP(void);
static uint8_t FLASH_OB_GetBOR(void);
static HAL_StatusTypeDef FLASH_OB_WRPConfig(uint32_t OB_WRP, FunctionalState NewState);
static HAL_StatusTypeDef FLASH_OB_RDP_LevelConfig(uint8_t OB_RDP);
static HAL_StatusTypeDef FLASH_OB_UserConfig(uint8_t OB_IWDG, uint8_t OB_STOP, uint8_t OB_STDBY);
static HAL_StatusTypeDef FLASH_OB_BOR_LevelConfig(uint8_t OB_BOR);

/* Aliases for legacy HAL versions compatibility */
#define  DATA_EEPROM_FastProgramByte       FLASH_DATAEEPROM_FastProgramByte
#define  DATA_EEPROM_FastProgramHalfWord   FLASH_DATAEEPROM_FastProgramHalfWord
#define  DATA_EEPROM_FastProgramWord       FLASH_DATAEEPROM_FastProgramWord
#define  DATA_EEPROM_ProgramByte           FLASH_DATAEEPROM_ProgramByte
#define  DATA_EEPROM_ProgramHalfWord       FLASH_DATAEEPROM_ProgramHalfWord
#define  DATA_EEPROM_ProgramWord           FLASH_DATAEEPROM_ProgramWord

    
/* Private functions ---------------------------------------------------------*/

/** @defgroup FLASHEx_Private_Functions Extended FLASH Private functions
  * @{
  */

/** @defgroup FLASHEx_Group1 Peripheral extended features functions 
 *  @brief   Data transfers functions 
 *
@verbatim   
 ===============================================================================
                      ##### Extended Features functions #####
 ===============================================================================  
    [..]
    This subsection provides a set of functions allowing to manage the FLASH data 
    transfers.

@endverbatim
  * @{
  */

/**
  * @brief  Program option bytes
  * @param  pAdvOBInit: pointer to an FLASH_AdvOBProgramInitTypeDef structure that
  *         contains the configuration information for the programming.
  * 
  * @retval HAL_StatusTypeDef HAL Status
  */
HAL_StatusTypeDef HAL_FLASHEx_AdvOBProgram (FLASH_AdvOBProgramInitTypeDef *pAdvOBInit)
{
  HAL_StatusTypeDef status = HAL_ERROR;
  
  /* Check the parameters */
  assert_param(IS_OBEX(pAdvOBInit->OptionType));

  /*Program PCROP option byte*/
  if ((pAdvOBInit->OptionType&OBEX_PCROP) == OBEX_PCROP)
  {
    /* Check the parameters */
    assert_param(IS_PCROPSTATE(pAdvOBInit->PCROPState));
    if (pAdvOBInit->PCROPState == PCROPSTATE_ENABLE)
    {
      /*Enable of Write protection on the selected Sector*/
      status = FLASH_OB_PCROPConfig(pAdvOBInit->Pages, ENABLE);
      status = FLASH_OB_PCROPSelectionConfig(OB_PCROP_SELECTED);
    }
    else
    {
      /*Disable of Write protection on the selected Sector*/ 
      status = FLASH_OB_PCROPConfig(pAdvOBInit->Pages, DISABLE);
      status = FLASH_OB_PCROPSelectionConfig(OB_PCROP_DESELECTED);
    }
  }   
  /*Program BOOT config option byte*/
  if ((pAdvOBInit->OptionType&OBEX_BOOTCONFIG) == OBEX_BOOTCONFIG)
  {
    status = FLASH_OB_BootConfig(pAdvOBInit->BootConfig);
  }

  return status;
}

/**
  * @brief   Get the OBEX byte configuration
  * @param  pAdvOBInit: pointer to an FLASH_AdvOBProgramInitTypeDef structure that
  *         contains the configuration information for the programming.
  * 
  * @retval None
  */
void HAL_FLASHEx_AdvOBGetConfig(FLASH_AdvOBProgramInitTypeDef *pAdvOBInit)
{
  pAdvOBInit->OptionType = OBEX_PCROP| OBEX_BOOTCONFIG;
  /*Get PCROP state */
  pAdvOBInit->PCROPState = (FLASH->OBR & 0x00000100) >> 8;
  /*Get PCROP protected Pages */
  pAdvOBInit->Pages = FLASH->WRPR;
  /*Get Boot config OB*/
  pAdvOBInit->BootConfig = (FLASH->OBR & 0x80000000) >> 24;
}

/**
  * @}
  */
  
/** @defgroup FLASHEx_Group2 DATA EEPROM Programming functions
 *  @brief   DATA EEPROM Programming functions
 *
@verbatim   
 ===============================================================================
                     ##### DATA EEPROM Programming functions ##### 
 ===============================================================================  
 
    [..] The FLASH_DATAEEPROM Programming_Functions, includes the following functions:
        (+) HAL_FLASHEx_DATAEEPROM_Unlock(void);
        (+) HAL_FLASHEx_DATAEEPROM_Lock(void);
        (+) HAL_FLASHEx_DATAEEPROM_Erase(uint32_t Address)
        (+) HAL_FLASHEx_DATAEEPROM_Program(uint32_t TypeProgram, uint32_t Address, uint32_t Data)
   
    [..] Any operation of erase or program should follow these steps:
    (#) Call the HAL_FLASHEx_DATAEEPROM_Unlock() function to enable the data EEPROM access
        and Flash program erase control register access.
    (#) Call the desired function to erase or program data.
    (#) Call the HAL_FLASHEx_DATAEEPROM_Lock() to disable the data EEPROM access
        and Flash program erase control register access(recommended
        to protect the DATA_EEPROM against possible unwanted operation).

@endverbatim
  * @{
  */
/**
  * @brief  Unlocks the data memory and FLASH_PECR register access.
  * @param  None
  * @retval HAL_StatusTypeDef HAL Status
  */
HAL_StatusTypeDef HAL_FLASHEx_DATAEEPROM_Unlock(void)
{
  if((FLASH->PECR & FLASH_PECR_PELOCK) != RESET)
  {  
    /* Unlocking the Data memory and FLASH_PECR register access*/
    FLASH->PEKEYR = FLASH_PEKEY1;
    FLASH->PEKEYR = FLASH_PEKEY2;
  }
  else
  {
    return HAL_ERROR;
  }
  return HAL_OK;  
}

/**
  * @brief  Locks the Data memory and FLASH_PECR register access.
  * @param  None
  * @retval HAL_StatusTypeDef HAL Status
  */
HAL_StatusTypeDef HAL_FLASHEx_DATAEEPROM_Lock(void)
{
  /* Set the PELOCK Bit to lock the data memory and FLASH_PECR register access */
  FLASH->PECR |= FLASH_PECR_PELOCK;
  
  return HAL_OK;
}
  
/**
  * @brief  Erase a word in data memory.
  * @param  Address: specifies the address to be erased.
  * @note   To correctly run this function, the HAL_FLASHEx_DATAEEPROM_Unlock() function
  *         must be called before.
  *         Call the HAL_FLASHEx_DATAEEPROM_Lock() to the data EEPROM access
  *         and Flash program erase control register access(recommended to protect 
  *         the DATA_EEPROM against possible unwanted operation).
  * @retval FLASH Status: The returned value can be: 
  *   FLASH_ERROR_PROGRAM, FLASH_ERROR_WRP, FLASH_COMPLETE or FLASH_TIMEOUT.
  */
HAL_StatusTypeDef HAL_FLASHEx_DATAEEPROM_Erase(uint32_t Address)
{
  HAL_StatusTypeDef status = HAL_OK;
  
  /* Check the parameters */
  assert_param(IS_FLASH_DATA_ADDRESS(Address));
  
  /* Wait for last operation to be completed */
  status = FLASH_WaitForLastOperation((uint32_t)HAL_FLASH_TIMEOUT_VALUE));
  
  if(status == HAL_OK)
  {
    /* Write "00000000h" to valid address in the data memory" */
    *(__IO uint32_t *) Address = 0x00000000;
  }
   
  /* Return the erase status */
  return status;
}  

/**
  * @brief  Program word at a specified address
  * @param  TypeProgram:  Indicate the way to program at a specified address.
  *                           This parameter can be a value of @ref FLASH_Type_Program
  * @param  Address:  specifies the address to be programmed.
  * @param  Data: specifies the data to be programmed
  * 
  * @retval HAL_StatusTypeDef HAL Status
  */

HAL_StatusTypeDef   HAL_FLASHEx_DATAEEPROM_Program(uint32_t TypeProgram, uint32_t Address, uint32_t Data)
{
  HAL_StatusTypeDef status = HAL_ERROR;
  
  /* Process Locked */
  __HAL_LOCK(&pFlash);

  /* Check the parameters */
  assert_param(IS_TYPEPROGRAM(TypeProgram));

  /* Wait for last operation to be completed */
  status = FLASH_WaitForLastOperation((uint32_t)HAL_FLASH_TIMEOUT_VALUE));
  
  if(status == HAL_OK)
  {
    if(TypeProgram == TYPEPROGRAM_FASTBYTE)
    {
      /*Program word (8-bit) at a specified address.*/
      FLASH_DATAEEPROM_FastProgramByte(Address, (uint8_t) Data);
      
      /* Wait for last operation to be completed */
      status = FLASH_WaitForLastOperation((uint32_t)HAL_FLASH_TIMEOUT_VALUE));
    }
    
    if(TypeProgram == TYPEPROGRAM_FASTHALFWORD)
    {
      /*Program word (16-bit) at a specified address.*/
      FLASH_DATAEEPROM_FastProgramHalfWord(Address, (uint16_t) Data);
      
      /* Wait for last operation to be completed */
      status = FLASH_WaitForLastOperation((uint32_t)HAL_FLASH_TIMEOUT_VALUE));
    }    
    if(TypeProgram == TYPEPROGRAM_FASTWORD)
    {
      /*Program word (32-bit) at a specified address.*/
      FLASH_DATAEEPROM_FastProgramWord(Address, (uint32_t) Data);
      
      /* Wait for last operation to be completed */
      status = FLASH_WaitForLastOperation((uint32_t)HAL_FLASH_TIMEOUT_VALUE));
    }
    if(TypeProgram == TYPEPROGRAM_WORD)
    {
      /*Program word (32-bit) at a specified address.*/
      FLASH_DATAEEPROM_ProgramWord(Address, (uint32_t) Data);
      
      /* Wait for last operation to be completed */
      status = FLASH_WaitForLastOperation((uint32_t)HAL_FLASH_TIMEOUT_VALUE));
    }
       
    if(TypeProgram == TYPEPROGRAM_HALFWORD)
    {
      /*Program word (16-bit) at a specified address.*/
      FLASH_DATAEEPROM_ProgramHalfWord(Address, (uint16_t) Data);
      
      /* Wait for last operation to be completed */
      status = FLASH_WaitForLastOperation((uint32_t)HAL_FLASH_TIMEOUT_VALUE));
    }
        
    if(TypeProgram == TYPEPROGRAM_BYTE)
    {
      /*Program word (8-bit) at a specified address.*/
      FLASH_DATAEEPROM_ProgramByte(Address, (uint8_t) Data);
      
      /* Wait for last operation to be completed */
      status = FLASH_WaitForLastOperation((uint32_t)HAL_FLASH_TIMEOUT_VALUE));
    }
  } 
  /* Process Unlocked */
  __HAL_UNLOCK(&pFlash);

  return status;
}

/**
  * @brief  Erase the specified FLASH memory Pages 
  * @param[in]  pEraseInit: pointer to an FLASH_EraseInitTypeDef structure that
  *         contains the configuration information for the erasing.
  * 
  * @param[out]  PageError: pointer to variable  that
  *         contains the configuration information on faulty sector in case of error 
  *         (0xFFFFFFFF means that all the sectors have been correctly erased)
  * 
  * @retval HAL_StatusTypeDef HAL Status
  */
HAL_StatusTypeDef HAL_FLASHEx_Erase(FLASH_EraseInitTypeDef *pEraseInit, uint32_t *PageError)
{
  HAL_StatusTypeDef status = HAL_ERROR;
  uint32_t index = 0;
  
  /* Process Locked */
  __HAL_LOCK(&pFlash);

  /* Check the parameters */
  assert_param(IS_TYPEERASE(pEraseInit->TypeErase));

  /* Wait for last operation to be completed */
  status = FLASH_WaitForLastOperation((uint32_t)HAL_FLASH_TIMEOUT_VALUE));

  if (status == HAL_OK)
  {
    /*Initialization of PageError variable*/
    *PageError = 0xFFFFFFFF;
      
    /* Check the parameters */
      assert_param(IS_NBPAGES(pEraseInit->NbPages + pEraseInit->Page));

      /* Erase by sector by sector to be done*/
      for(index = pEraseInit->Page; index < ((pEraseInit->NbPages*PAGESIZE)+ pEraseInit->Page); index+=PAGESIZE)
      {        
        FLASH_Erase_Page(index);

        /* Wait for last operation to be completed */
        status = FLASH_WaitForLastOperation((uint32_t)HAL_FLASH_TIMEOUT_VALUE));
        
        /* If the erase operation is completed, disable the ERASE Bit */
        FLASH->PECR &= (~FLASH_PECR_PROG);
        FLASH->PECR &= (~FLASH_PECR_ERASE);

        if (status != HAL_OK) 
        {
          /* In case of error, stop erase procedure and return the faulty sector*/
          *PageError = index;
          break;
        }
      }
    }

  /* Process Unlocked */
  __HAL_UNLOCK(&pFlash);

  return status;
}

/**
  * @brief  Perform a page erase of the specified FLASH memory pages  with interrupt enabled
  * @param  pEraseInit: pointer to an FLASH_EraseInitTypeDef structure that
  *         contains the configuration information for the erasing.
  * 
  * @retval HAL_StatusTypeDef HAL Status
  */
HAL_StatusTypeDef HAL_FLASHEx_Erase_IT(FLASH_EraseInitTypeDef *pEraseInit)
{
  HAL_StatusTypeDef status = HAL_OK;

  /* Process Locked */
  __HAL_LOCK(&pFlash);

  /* Check the parameters */
  assert_param(IS_TYPEERASE(pEraseInit->TypeErase));

  /* Enable End of FLASH Operation interrupt */
  __HAL_FLASH_ENABLE_IT(FLASH_IT_EOP);
  
  /* Enable Error source interrupt */
  __HAL_FLASH_ENABLE_IT(FLASH_IT_ERR);
  
  /* Clear pending flags (if any) */  
  __HAL_FLASH_CLEAR_FLAG(FLASH_FLAG_EOP    | FLASH_FLAG_ENDHV | FLASH_FLAG_WRPERR |\
                         FLASH_FLAG_PGAERR | FLASH_FLAG_SIZERR| FLASH_FLAG_OPTVERR |\
                         FLASH_FLAG_RDERR  | FLASH_FLAG_NOTZEROERR);  

  if (pEraseInit->TypeErase == TYPEERASE_PAGEERASE)
  {
    /* Erase by sector to be done*/

    /* Check the parameters */
    assert_param(IS_NBPAGES(pEraseInit->NbPages + pEraseInit->Page));

    pFlash.ProcedureOnGoing = FLASH_PROC_PAGEERASE;
    pFlash.NbPagesToErase = pEraseInit->NbPages;
    pFlash.Page = pEraseInit->Page;

    /*Erase 1st page and wait for IT*/
    FLASH_Erase_Page(pEraseInit->Page);
  }

  return status;
}


/**
  * @brief  Program option bytes
  * @param  pOBInit: pointer to an FLASH_OBInitStruct structure that
  *         contains the configuration information for the programming.
  * 
  * @retval HAL_StatusTypeDef HAL Status
  */
HAL_StatusTypeDef HAL_FLASHEx_OBProgram(FLASH_OBProgramInitTypeDef *pOBInit)
{
  HAL_StatusTypeDef status = HAL_ERROR;
  
  /* Process Locked */
  __HAL_LOCK(&pFlash);

  /* Check the parameters */
  assert_param(IS_OPTIONBYTE(pOBInit->OptionType));

  /*Write protection configuration*/
  if((pOBInit->OptionType & OPTIONBYTE_WRP) == OPTIONBYTE_WRP)
  {
    assert_param(IS_WRPSTATE(pOBInit->WRPState));
    if (pOBInit->WRPState == WRPSTATE_ENABLE)
    {
      /*Enable of Write protection on the selected Sector*/
      status = FLASH_OB_WRPConfig(pOBInit->WRPSector, ENABLE);
    }
    else
    {
      /*Disable of Write protection on the selected Sector*/
      status = FLASH_OB_WRPConfig(pOBInit->WRPSector, DISABLE);
    }
  }

  /* Read protection configuration*/
  if((pOBInit->OptionType & OPTIONBYTE_RDP) == OPTIONBYTE_RDP)
  {
    status = FLASH_OB_RDP_LevelConfig(pOBInit->RDPLevel);
  }

  /* USER  configuration*/
  if((pOBInit->OptionType & OPTIONBYTE_USER) == OPTIONBYTE_USER)
  {
    status = FLASH_OB_UserConfig(pOBInit->USERConfig&OB_IWDG_SW, 
                                     pOBInit->USERConfig&OB_STOP_NoRST,
                                     pOBInit->USERConfig&OB_STDBY_NoRST);
  }

  /* BOR Level  configuration*/
  if((pOBInit->OptionType & OPTIONBYTE_BOR) == OPTIONBYTE_BOR)
  {
    status = FLASH_OB_BOR_LevelConfig(pOBInit->BORLevel);
  } 
  /* Process Unlocked */
  __HAL_UNLOCK(&pFlash);

  return status;
}

/**
  * @brief   Get the Option byte configuration
  * @param  pOBInit: pointer to an FLASH_OBInitStruct structure that
  *         contains the configuration information for the programming.
  * 
  * @retval None
  */
void HAL_FLASHEx_OBGetConfig(FLASH_OBProgramInitTypeDef *pOBInit)
{
  pOBInit->OptionType = OPTIONBYTE_WRP | OPTIONBYTE_RDP | OPTIONBYTE_USER | OPTIONBYTE_BOR;

  /*Get WRP*/
  pOBInit->WRPSector = FLASH_OB_GetWRP();

  /*Get RDP Level*/
  pOBInit->RDPLevel = FLASH_OB_GetRDP();

  /*Get USER*/
  pOBInit->USERConfig = FLASH_OB_GetUser();

  /*Get BOR Level*/
  pOBInit->BORLevel = FLASH_OB_GetBOR();
}

/**
  * @}
  */
  
/**
  * @brief  Returns the FLASH User Option Bytes values.
  * @param  None
  * @retval The FLASH User Option Bytes.
  */
static uint8_t FLASH_OB_GetUser(void)
{
  /* Return the User Option Byte */
  return (uint8_t)(FLASH->OBR >> 20);
}

/**
  * @brief  Returns the FLASH Write Protection Option Bytes value.
  * @param  None
  * @retval The FLASH Write Protection Option Bytes value.
  */
static uint32_t FLASH_OB_GetWRP(void)
{
  /* Return the FLASH write protection Register value */
  return (uint32_t)(FLASH->WRPR);
}

/**
  * @brief  Checks whether the FLASH Read out Protection Status is set or not.
  * @param  None
  * @retval FLASH ReadOut Protection Status(SET or RESET).
  */
static FlagStatus FLASH_OB_GetRDP(void)
{
  FlagStatus readstatus = RESET;
  
  if ((uint8_t)(FLASH->OBR) != (uint8_t)OB_RDP_Level_0)
  {
    readstatus = SET;
  }
  else
  {
    readstatus = RESET;
  }
  return readstatus;
}

/**
  * @brief  Returns the FLASH BOR level.
  * @param  None
  * @retval The FLASH User Option Bytes.
  */
static uint8_t FLASH_OB_GetBOR(void)
{
  /* Return the BOR level */
  return (uint8_t)((FLASH->OBR & (uint32_t)0x000F0000) >> 16);
}

/**
  * @brief  Write protects the desired pages of the first 64KB of the Flash.
  * @param  OB_WRP: specifies the address of the pages to be write protected.
  *   This parameter can be:
  *     @arg  value between OB_WRP_Pages0to31 and OB_WRP_Pages480to511
  *     @arg  OB_WRP_AllPages
  * @param  NewState: new state of the specified FLASH Pages Wtite protection.
  *   This parameter can be: ENABLE or DISABLE.
  * @retval HAL_StatusTypeDef
  */
static HAL_StatusTypeDef FLASH_OB_WRPConfig(uint32_t OB_WRP, FunctionalState NewState)
{
  HAL_StatusTypeDef status = HAL_OK;
  uint32_t WRP01_Data = 0;
  uint32_t tmp1 = 0;
  
  /* Check the parameters */
  assert_param(IS_OB_WRP(OB_WRP));
  assert_param(IS_FUNCTIONAL_STATE(NewState));
     
  /* Wait for last operation to be completed */
  status = FLASH_WaitForLastOperation((uint32_t)HAL_FLASH_TIMEOUT_VALUE));
 
  if(status == HAL_OK)
  {
    if (NewState != DISABLE)
    {
      WRP01_Data = (uint16_t)(((OB_WRP & WRP01_MASK) | OB->WRP01));
      tmp1 = (uint32_t)(~(WRP01_Data) << 16)|(WRP01_Data);
      OB->WRP01 = tmp1;
    }             
    else
    {
      WRP01_Data = (uint16_t)(~OB_WRP & (WRP01_MASK & OB->WRP01));
      tmp1 = (uint32_t)((~WRP01_Data) << 16)|(WRP01_Data);
      OB->WRP01 = tmp1;
    }
    /* Wait for last operation to be completed */
    status = FLASH_WaitForLastOperation(HAL_FLASH_TIMEOUT_VALUE));
  }

  /* Return the write protection operation Status */
  return status;      
}
/**
  * @brief  Enables or disables the read out protection.
  * @note   To correctly run this function, the FLASH_OB_Unlock() function
  *         must be called before.
  * @param  FLASH_ReadProtection_Level: specifies the read protection level. 
  *   This parameter can be:
  *     @arg OB_RDP_Level_0: No protection
  *     @arg OB_RDP_Level_1: Read protection of the memory
  *     @arg OB_RDP_Level_2: Chip protection
  * 
  *  !!!Warning!!! When enabling OB_RDP_Level_2 it's no more possible to go back to level 1 or 0
  *   
  * @retval FLASH Status: The returned value can be: 
  *         FLASH_ERROR_PROGRAM, FLASH_ERROR_WRP, FLASH_COMPLETE or FLASH_TIMEOUT.
  */
static HAL_StatusTypeDef FLASH_OB_RDP_LevelConfig(uint8_t OB_RDP)
{
  HAL_StatusTypeDef status = HAL_OK;
  uint16_t tmp1 = 0;
  uint32_t tmp2 = 0;
  
  /* Check the parameters */
  assert_param(IS_OB_RDP(OB_RDP));
  status = FLASH_WaitForLastOperation(HAL_FLASH_TIMEOUT_VALUE));
  
  /* calculate the option byte to write */
  tmp1 =  ((uint16_t)(*(__IO uint16_t *)(OB_BASE)) & 0xFF00) | OB_RDP; 
  tmp2 = (uint32_t)(((uint32_t)((uint32_t)(~tmp1) << 16)) | ((uint32_t)tmp1));
  
  if(status == HAL_OK)
  {         
   /* program read protection level */
    OB->RDP = tmp2;
  }
  
  /* Wait for last operation to be completed */
  status = FLASH_WaitForLastOperation((uint32_t)HAL_FLASH_TIMEOUT_VALUE));
     
  /* Return the Read protection operation Status */
  return status;            
}

/**
  * @brief  Programs the FLASH User Option Byte: IWDG_SW / RST_STOP / RST_STDBY.
  * @param  OB_IWDG: Selects the WDG mode.
  *   This parameter can be one of the following values:
  *     @arg OB_IWDG_SW: Software WDG selected
  *     @arg OB_IWDG_HW: Hardware WDG selected
  * @param  OB_STOP: Reset event when entering STOP mode.
  *   This parameter can be one of the following values:
  *     @arg OB_STOP_NoRST: No reset generated when entering in STOP
  *     @arg OB_STOP_RST: Reset generated when entering in STOP
  * @param  OB_STDBY: Reset event when entering Standby mode.
  *   This parameter can be one of the following values:
  *     @arg OB_STDBY_NoRST: No reset generated when entering in STANDBY
  *     @arg OB_STDBY_RST: Reset generated when entering in STANDBY
  * @retval FLASH Status: The returned value can be: 
  *         FLASH_ERROR_PROGRAM, FLASH_ERROR_WRP, FLASH_COMPLETE or FLASH_TIMEOUT.
  */
static HAL_StatusTypeDef FLASH_OB_UserConfig(uint8_t OB_IWDG, uint8_t OB_STOP, uint8_t OB_STDBY)
{
  HAL_StatusTypeDef status = HAL_OK; 
  uint32_t tmp = 0, tmp1 = 0;

  /* Check the parameters */
  assert_param(IS_OB_IWDG_SOURCE(OB_IWDG));
  assert_param(IS_OB_STOP_SOURCE(OB_STOP));
  assert_param(IS_OB_STDBY_SOURCE(OB_STDBY));

  /* Get the User Option byte register */
  tmp1 = (FLASH->OBR & 0x800F0000) >> 16;
    
  /* Calculate the user option byte to write */ 
  tmp = (uint32_t)(((uint32_t)~((uint32_t)((uint32_t)(OB_IWDG) | (uint32_t)(OB_STOP) | (uint32_t)(OB_STDBY) | tmp1))) << 16);
  tmp |= ((uint32_t)(OB_IWDG) | ((uint32_t)OB_STOP) | (uint32_t)(OB_STDBY) | tmp1);
  
  /* Wait for last operation to be completed */
  status = FLASH_WaitForLastOperation((uint32_t)HAL_FLASH_TIMEOUT_VALUE));
  
  if(status == HAL_OK)
  {  
    /* Write the User Option Byte */              
    OB->USER = tmp; 
  }
  
  /* Wait for last operation to be completed */
  status = FLASH_WaitForLastOperation((uint32_t)HAL_FLASH_TIMEOUT_VALUE));
       
  /* Return the Option Byte program Status */
  return status;
}

/**
  * @brief  Programs the FLASH brownout reset threshold level Option Byte.
  * @param  OB_BOR: Selects the brownout reset threshold level.
  *   This parameter can be one of the following values:
  *     @arg OB_BOR_OFF: BOR is disabled at power down, the reset is asserted when the VDD 
  *                      power supply reaches the PDR(Power Down Reset) threshold (1.5V)
  *     @arg OB_BOR_LEVEL1: BOR Reset threshold levels for 1.7V - 1.8V VDD power supply
  *     @arg OB_BOR_LEVEL2: BOR Reset threshold levels for 1.9V - 2.0V VDD power supply
  *     @arg OB_BOR_LEVEL3: BOR Reset threshold levels for 2.3V - 2.4V VDD power supply
  *     @arg OB_BOR_LEVEL4: BOR Reset threshold levels for 2.55V - 2.65V VDD power supply
  *     @arg OB_BOR_LEVEL5: BOR Reset threshold levels for 2.8V - 2.9V VDD power supply
  * @retval FLASH Status: The returned value can be: 
  * FLASH_ERROR_PROGRAM, FLASH_ERROR_WRP, FLASH_COMPLETE or FLASH_TIMEOUT.
  */
static HAL_StatusTypeDef FLASH_OB_BOR_LevelConfig(uint8_t OB_BOR)
{
  HAL_StatusTypeDef status = HAL_OK; 
  uint32_t tmp = 0, tmp1 = 0;

  /* Check the parameters */
  assert_param(IS_OB_BOR_LEVEL(OB_BOR));

  /* Get the User Option byte register */
  tmp1 = (FLASH->OBR & 0x000F0000) >> 16;
     
  /* Calculate the option byte to write */
  tmp = (uint32_t)~(OB_BOR | tmp1)<<16;
  tmp |= (OB_BOR | tmp1);
    
  /* Wait for last operation to be completed */
  status = FLASH_WaitForLastOperation((uint32_t)HAL_FLASH_TIMEOUT_VALUE));
  
  if(status == HAL_OK)
  {  
    /* Write the BOR Option Byte */            
    OB->USER = tmp; 
  }
  
  /* Wait for last operation to be completed */
  status = FLASH_WaitForLastOperation((uint32_t)HAL_FLASH_TIMEOUT_VALUE));
        
  /* Return the Option Byte program Status */
  return status;
}


/**
  * @brief  Enables or disables the read/write protection (PCROP) of the desired 
  *         sectors, for the first 64KB of the Flash.
  * @param  OB_WRP: specifies the address of the pages to be write protected.
  *   This parameter can be:
  *     @arg  value between OB_WRP_Pages0to31 and OB_WRP_Pages480to511
  *     @arg  OB_WRP_AllPages
  * @param  NewState: new state of the specified FLASH Pages Write protection.
  *   This parameter can be: ENABLE or DISABLE.
  * @retval FLASH Status: The returned value can be: 
  *         FLASH_ERROR_PROGRAM, FLASH_ERROR_WRP, FLASH_COMPLETE or FLASH_TIMEOUT.
  */
static HAL_StatusTypeDef FLASH_OB_PCROPConfig(uint32_t OB_WRP, FunctionalState NewState)
{
  HAL_StatusTypeDef status = HAL_OK;
  uint32_t WRP01_Data = 0;
  uint32_t tmp1 = 0;
  
  /* Check the parameters */
  assert_param(IS_OB_WRP(OB_WRP));
  assert_param(IS_FUNCTIONAL_STATE(NewState));
     
  /* Wait for last operation to be completed */
  status = FLASH_WaitForLastOperation((uint32_t)HAL_FLASH_TIMEOUT_VALUE));
 
  if(status == HAL_OK)
  {
    if (NewState != DISABLE)
    {
      WRP01_Data = (uint16_t)(~OB_WRP & (WRP01_MASK & OB->WRP01));

      tmp1 = (uint32_t)((~WRP01_Data) << 16)|(WRP01_Data);
      OB->WRP01 = tmp1; 
    }             
    
    else
    {
      WRP01_Data = (uint16_t)((OB_WRP & WRP01_MASK) | OB->WRP01);
     
      tmp1 = (uint32_t)(~(WRP01_Data) << 16)|(WRP01_Data);
      OB->WRP01 = tmp1;
    }
    /* Wait for last operation to be completed */
    status = FLASH_WaitForLastOperation((uint32_t)HAL_FLASH_TIMEOUT_VALUE));
  }

  /* Return the write protection operation Status */
  return status;       
}

/**
  * @brief  Select the Protection Mode (SPRMOD).
  * @note   Once SPRMOD bit is active, unprotection of a protected sector is not possible 
  * @note   Read a protected sector will set RDERR Flag and write a protected sector will set WRPERR Flag
  * @param  OB_PcROP: Select the Protection Mode of WPR bits. 
  *   This parameter can be:
  *     @arg OB_PCROP_SELECTED: nWRP control the  read&write protection (PcROP) of respective user sectors.
  *     @arg OB_PCROP_DESELECTED: nWRP control the write protection of respective user sectors.
  * @retval FLASH Status: The returned value can be: 
  *         FLASH_ERROR_PROGRAM, FLASH_ERROR_WRP, FLASH_COMPLETE or FLASH_TIMEOUT.
  */
static HAL_StatusTypeDef FLASH_OB_PCROPSelectionConfig(uint16_t OB_PcROP)
{
  HAL_StatusTypeDef status = HAL_OK;
  uint16_t tmp1 = 0;
  uint32_t tmp2 = 0;
  uint8_t optiontmp = 0;
  uint16_t optiontmp2 = 0;
  
  /* Check the parameters */
  assert_param(IS_OB_PCROP_SELECT(OB_PcROP));
  
  /* Wait for last operation to be completed */
  status = FLASH_WaitForLastOperation((uint32_t)HAL_FLASH_TIMEOUT_VALUE));
  
  /* Mask RDP Byte */
  optiontmp =  (uint8_t)(*(__IO uint8_t *)(OB_BASE)); 
  
  /* Update Option Byte */
  optiontmp2 = (uint16_t)(OB_PcROP | optiontmp); 
  
  
  /* calculate the option byte to write */
  tmp1 = (uint16_t)(~(optiontmp2 ));
  tmp2 = (uint32_t)(((uint32_t)((uint32_t)(tmp1) << 16)) | ((uint32_t)optiontmp2));
  
  if(status == HAL_OK)
  {         
    /* program PCRop */
    OB->RDP = tmp2;
  }
  
  /* Wait for last operation to be completed */
  status = FLASH_WaitForLastOperation(HAL_FLASH_TIMEOUT_VALUE));
  
  /* Return the Read protection operation Status */
  return status;            
}

/**
  * @brief  Select the Protection Mode (SPRMOD).
  * @note   Once SPRMOD bit is active, unprotection of a protected sector is not possible 
  * @note   Read a protected sector will set RDERR Flag and write a protected sector will set WRPERR Flag
  * @param  None
  * @retval FLASH Status: The returned value can be: 
  *         FLASH_ERROR_PROGRAM, FLASH_ERROR_WRP, FLASH_COMPLETE or FLASH_TIMEOUT.
  */
HAL_StatusTypeDef HAL_FLASHEx_OB_SelectPCROP(void)
{
  HAL_StatusTypeDef status = HAL_OK;
  uint16_t tmp1 = 0;
  uint32_t tmp2 = 0;
  uint8_t optiontmp = 0;
  uint16_t optiontmp2 = 0;
  
  /* Wait for last operation to be completed */
  status = FLASH_WaitForLastOperation((uint32_t)HAL_FLASH_TIMEOUT_VALUE));
  
  /* Mask RDP Byte */
  optiontmp =  (uint8_t)(*(__IO uint8_t *)(OB_BASE)); 
  
  /* Update Option Byte */
  optiontmp2 = (uint16_t)(FLASH_OBR_SPRMOD | optiontmp); 
  
  
  /* calculate the option byte to write */
  tmp1 = (uint16_t)(~(optiontmp2 ));
  tmp2 = (uint32_t)(((uint32_t)((uint32_t)(tmp1) << 16)) | ((uint32_t)optiontmp2));
  
  if(status == HAL_OK)
  {         
    /* program PCRop */
    OB->RDP = tmp2;
  }
  
  /* Wait for last operation to be completed */
  status = FLASH_WaitForLastOperation(HAL_FLASH_TIMEOUT_VALUE));
  
  /* Return the Read protection operation Status */
  return status;            
  
}

/**
  * @brief  Deselect the Protection Mode (SPRMOD).
  * @note   Once SPRMOD bit is active, unprotection of a protected sector is not possible 
  * @note   Read a protected sector will set RDERR Flag and write a protected sector will set WRPERR Flag
  * @param  None
  * @retval FLASH Status: The returned value can be: 
  *         FLASH_ERROR_PROGRAM, FLASH_ERROR_WRP, FLASH_COMPLETE or FLASH_TIMEOUT.
  */
HAL_StatusTypeDef HAL_FLASHEx_OB_DeSelectPCROP(void)
{
 HAL_StatusTypeDef status = HAL_OK;
  uint16_t tmp1 = 0;
  uint32_t tmp2 = 0;
  uint8_t optiontmp = 0;
  uint16_t optiontmp2 = 0;
  
  /* Wait for last operation to be completed */
  status = FLASH_WaitForLastOperation((uint32_t)HAL_FLASH_TIMEOUT_VALUE));
  
  /* Mask RDP Byte */
  optiontmp =  (uint8_t)(*(__IO uint8_t *)(OB_BASE)); 
  
  /* Update Option Byte */
  optiontmp2 = (uint16_t)(optiontmp); 
  
  /* calculate the option byte to write */
  tmp1 = (uint16_t)(~(optiontmp2 ));
  tmp2 = (uint32_t)(((uint32_t)((uint32_t)(tmp1) << 16)) | ((uint32_t)optiontmp2));
  
  if(status == HAL_OK)
  {         
    /* program PCRop */
    OB->RDP = tmp2;
  }
  
  /* Wait for last operation to be completed */
  status = FLASH_WaitForLastOperation(HAL_FLASH_TIMEOUT_VALUE));
  
  /* Return the Read protection operation Status */
  return status;     
}


/**
  * @brief  Sets or resets the BOOT1 option bit.
  * @param  OB_BOOT1: Set or Reset the BOOT1 option bit.
  *          This parameter can be one of the following values:
  *             @arg OB_BOOT1_RESET: BOOT1 option bit reset
  *             @arg OB_BOOT1_SET: BOOT1 option bit set
  * @retval None
  */
static HAL_StatusTypeDef FLASH_OB_BootConfig(uint16_t OB_BOOT)
{
  HAL_StatusTypeDef status = HAL_OK;
  uint32_t tmp = 0, tmp1 = 0;

  /* Check the parameters */
  assert_param(IS_OB_BOOT1(OB_BOOT));

  /* Get the User Option byte register */
  tmp1 = (FLASH->OBR & 0x007F0000) >> 16;
     
  /* Calculate the option byte to write */
  tmp = (uint32_t)~(OB_BOOT | tmp1)<<16;
  tmp |= (OB_BOOT | tmp1);
    
  /* Wait for last operation to be completed */
  status = FLASH_WaitForLastOperation((uint32_t)HAL_FLASH_TIMEOUT_VALUE));
  
  if(status == HAL_OK)
  {  
    /* Write the BOOT Option Byte */            
    OB->USER = tmp; 
  }
  
  /* Wait for last operation to be completed */
  status = FLASH_WaitForLastOperation((uint32_t)HAL_FLASH_TIMEOUT_VALUE));
       
  /* Return the Option Byte program Status */
  return status;
}

/**
  * @brief  Write a Byte at a specified address in data memory.
  * @note   To correctly run this function, the HAL_FLASHEx_DATAEEPROM_Unlock() function
  *         must be called before.
  *         Call the HAL_FLASHEx_DATAEEPROM_Lock() to the data EEPROM access
  *         and Flash program erase control register access(recommended to protect 
  *         the DATA_EEPROM against possible unwanted operation).
  * @param  Address: specifies the address to be written.
  * @param  Data: specifies the data to be written.
  * @note   This function assumes that data word is already erased.
  * @retval FLASH Status: The returned value can be:
  *         FLASH_ERROR_PROGRAM, FLASH_ERROR_WRP, FLASH_COMPLETE or FLASH_TIMEOUT.
  */
static HAL_StatusTypeDef FLASH_DATAEEPROM_FastProgramByte(uint32_t Address, uint8_t Data)
{
  HAL_StatusTypeDef status = HAL_ERROR;
  
  /* Check the parameters */
  assert_param(IS_FLASH_DATA_ADDRESS(Address)); 

  /* Wait for last operation to be completed */
  status = FLASH_WaitForLastOperation((uint32_t)HAL_FLASH_TIMEOUT_VALUE));
    
  if(status == HAL_OK)
  {
    /* Clear the FTDW bit */
    FLASH->PECR &= (uint32_t)(~((uint32_t)FLASH_PECR_FTDW));

    /* If the previous operation is completed, proceed to write the new Data */
    *(__IO uint8_t *)Address = Data;
            
    /* Wait for last operation to be completed */
    status = FLASH_WaitForLastOperation((uint32_t)HAL_FLASH_TIMEOUT_VALUE));
  }
  /* Return the Write Status */
  return status;
}

/**
  * @brief  Writes a half word at a specified address in data memory.
  * @note   To correctly run this function, the HAL_FLASHEx_DATAEEPROM_Unlock() function
  *         must be called before.
  *         Call the HAL_FLASHEx_DATAEEPROM_Lock() to he data EEPROM access
  *         and Flash program erase control register access(recommended to protect 
  *         the DATA_EEPROM against possible unwanted operation).
  * @param  Address: specifies the address to be written.
  * @param  Data: specifies the data to be written.
  * @note   This function assumes that the is data word is already erased.
  * @retval FLASH Status: The returned value can be: 
  *         FLASH_ERROR_PROGRAM, FLASH_ERROR_WRP, FLASH_COMPLETE or  FLASH_TIMEOUT. 
  */
static HAL_StatusTypeDef FLASH_DATAEEPROM_FastProgramHalfWord(uint32_t Address, uint16_t Data)
{
  HAL_StatusTypeDef status = HAL_ERROR;
  
  /* Check the parameters */
  assert_param(IS_FLASH_DATA_ADDRESS(Address));

  /* Wait for last operation to be completed */
  status = FLASH_WaitForLastOperation((uint32_t)HAL_FLASH_TIMEOUT_VALUE));
    
  if(status == HAL_OK)
  {
    /* Clear the FTDW bit */
    FLASH->PECR &= (uint32_t)(~((uint32_t)FLASH_PECR_FTDW));

    /* If the previous operation is completed, proceed to write the new data */
    *(__IO uint16_t *)Address = Data;
  
    /* Wait for last operation to be completed */
    status = FLASH_WaitForLastOperation((uint32_t)HAL_FLASH_TIMEOUT_VALUE));
  }
  /* Return the Write Status */
  return status;
}

/**
  * @brief  Programs a word at a specified address in data memory.
  * @note   To correctly run this function, the HAL_FLASHEx_DATAEEPROM_Unlock() function
  *         must be called before.
  *         Call the HAL_FLASHEx_DATAEEPROM_Lock() to the data EEPROM access
  *         and Flash program erase control register access(recommended to protect 
  *         the DATA_EEPROM against possible unwanted operation).
  * @param  Address: specifies the address to be written.
  * @param  Data: specifies the data to be written.
  * @note   This function assumes that the is data word is already erased.
  * @retval FLASH Status: The returned value can be: 
  *         FLASH_ERROR_PROGRAM, FLASH_ERROR_WRP, FLASH_COMPLETE or FLASH_TIMEOUT. 
  */
static HAL_StatusTypeDef FLASH_DATAEEPROM_FastProgramWord(uint32_t Address, uint32_t Data)
{
  HAL_StatusTypeDef status = HAL_ERROR;

  /* Check the parameters */
  assert_param(IS_FLASH_DATA_ADDRESS(Address));
  
  /* Wait for last operation to be completed */
  status = FLASH_WaitForLastOperation((uint32_t)HAL_FLASH_TIMEOUT_VALUE));
  
  if(status == HAL_OK)
  {
    /* Clear the FTDW bit */
    FLASH->PECR &= (uint32_t)(~((uint32_t)FLASH_PECR_FTDW));
  
    /* If the previous operation is completed, proceed to program the new data */    
    *(__IO uint32_t *)Address = Data;
    
    /* Wait for last operation to be completed */
    status = FLASH_WaitForLastOperation((uint32_t)HAL_FLASH_TIMEOUT_VALUE));       
  }
  /* Return the Write Status */
  return status;
}

/**
  * @brief  Write a Byte at a specified address in data memory without erase.
  * @note   To correctly run this function, the HAL_FLASHEx_DATAEEPROM_Unlock() function
  *         must be called before.
  *         Call the HAL_FLASHEx_DATAEEPROM_Lock() to he data EEPROM access
  *         and Flash program erase control register access(recommended to protect 
  *         the DATA_EEPROM against possible unwanted operation).
  * @param  Address: specifies the address to be written.
  * @param  Data: specifies the data to be written.
  * @retval FLASH Status: The returned value can be: 
  *   FLASH_ERROR_PROGRAM, FLASH_ERROR_WRP, FLASH_COMPLETE or FLASH_TIMEOUT. 
  */
static HAL_StatusTypeDef FLASH_DATAEEPROM_ProgramByte(uint32_t Address, uint8_t Data)
{
  HAL_StatusTypeDef status = HAL_ERROR;
  
  /* Check the parameters */
  assert_param(IS_FLASH_DATA_ADDRESS(Address)); 

  /* Wait for last operation to be completed */
  status = FLASH_WaitForLastOperation((uint32_t)HAL_FLASH_TIMEOUT_VALUE));
  
  if(status == HAL_OK)
  {
    /* Set the FTDW bit */
    FLASH->PECR |= (uint32_t)FLASH_PECR_FTDW;
    
    *(__IO uint8_t *)Address = Data;
    
    /* Wait for last operation to be completed */
    status = FLASH_WaitForLastOperation((uint32_t)HAL_FLASH_TIMEOUT_VALUE));
  }
  /* Return the Write Status */
  return status;
}

/**
  * @brief  Writes a half word at a specified address in data memory without erase.
  * @note   To correctly run this function, the HAL_FLASHEx_DATAEEPROM_Unlock function
  *         must be called before.
  *         Call the HAL_FLASHEx_DATAEEPROM_Lock() to he data EEPROM access
  *         and Flash program erase control register access(recommended to protect 
  *         the DATA_EEPROM against possible unwanted operation).
  * @param  Address: specifies the address to be written.
  * @param  Data: specifies the data to be written.
  * @retval FLASH Status: The returned value can be:
  *   FLASH_ERROR_PROGRAM, FLASH_ERROR_WRP, FLASH_COMPLETE or FLASH_TIMEOUT. 
  */
static HAL_StatusTypeDef FLASH_DATAEEPROM_ProgramHalfWord(uint32_t Address, uint16_t Data)
{
  HAL_StatusTypeDef status = HAL_ERROR;
  
  /* Check the parameters */
  assert_param(IS_FLASH_DATA_ADDRESS(Address));

  /* Wait for last operation to be completed */
  status = FLASH_WaitForLastOperation((uint32_t)HAL_FLASH_TIMEOUT_VALUE));
  
  if(status == HAL_OK)
  {
    /* Set the FTDW bit */
    FLASH->PECR |= (uint32_t)FLASH_PECR_FTDW;
    
    *(__IO uint16_t *)Address = Data;
   
    /* Wait for last operation to be completed */
    status = FLASH_WaitForLastOperation((uint32_t)HAL_FLASH_TIMEOUT_VALUE));
  }
  /* Return the Write Status */
  return status;
}

/**
  * @brief  Programs a word at a specified address in data memory without erase.
  * @note   To correctly run this function, the HAL_FLASHEx_DATAEEPROM_Unlock() function
  *         must be called before.
  *         Call the HAL_FLASHEx_DATAEEPROM_Lock() to he data EEPROM access
  *         and Flash program erase control register access(recommended to protect 
  *         the DATA_EEPROM against possible unwanted operation).
  * @param  Address: specifies the address to be written.
  * @param  Data: specifies the data to be written.
  * @retval FLASH Status: The returned value can be:
  *   FLASH_ERROR_PROGRAM, FLASH_ERROR_WRP, FLASH_COMPLETE or  FLASH_TIMEOUT. 
  */
static HAL_StatusTypeDef FLASH_DATAEEPROM_ProgramWord(uint32_t Address, uint32_t Data)
{
  HAL_StatusTypeDef status = HAL_ERROR;
  
  /* Check the parameters */
  assert_param(IS_FLASH_DATA_ADDRESS(Address));
  
  /* Wait for last operation to be completed */
  status = FLASH_WaitForLastOperation((uint32_t)HAL_FLASH_TIMEOUT_VALUE));
  
  if(status == HAL_OK)
  {
    /* Set the FTDW bit */
    FLASH->PECR |= (uint32_t)FLASH_PECR_FTDW;
    
    *(__IO uint32_t *)Address = Data;

    /* Wait for last operation to be completed */
    status = FLASH_WaitForLastOperation((uint32_t)HAL_FLASH_TIMEOUT_VALUE));
  }
  /* Return the Write Status */
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


