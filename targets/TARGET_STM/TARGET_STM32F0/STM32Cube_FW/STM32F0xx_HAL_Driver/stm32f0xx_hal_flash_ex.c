/**
  ******************************************************************************
  * @file    stm32f0xx_hal_flash_ex.c
  * @author  MCD Application Team
  * @brief   Extended FLASH HAL module driver.
  *    
  *          This file provides firmware functions to manage the following 
  *          functionalities of the FLASH peripheral:
  *           + Extended Initialization/de-initialization functions
  *           + Extended I/O operation functions
  *           + Extended Peripheral Control functions 
  *         
  @verbatim
  ==============================================================================
               ##### Flash peripheral extended features  #####
  ==============================================================================
           
                      ##### How to use this driver #####
  ==============================================================================
  [..] This driver provides functions to configure and program the FLASH memory 
       of all STM32F0xxx devices. It includes
       
        (++) Set/Reset the write protection
        (++) Program the user Option Bytes
        (++) Get the Read protection Level
  
  @endverbatim
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2016 STMicroelectronics.
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
#include "stm32f0xx_hal.h"

/** @addtogroup STM32F0xx_HAL_Driver
  * @{
  */
#ifdef HAL_FLASH_MODULE_ENABLED

/** @addtogroup FLASH
  * @{
  */
/** @addtogroup FLASH_Private_Variables
 * @{
 */
/* Variables used for Erase pages under interruption*/
extern FLASH_ProcessTypeDef pFlash;
/**
  * @}
  */

/**
  * @}
  */
  
/** @defgroup FLASHEx FLASHEx
  * @brief FLASH HAL Extension module driver
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/** @defgroup FLASHEx_Private_Constants FLASHEx Private Constants
 * @{
 */
#define FLASH_POSITION_IWDGSW_BIT        8U
#define FLASH_POSITION_OB_USERDATA0_BIT  16U
#define FLASH_POSITION_OB_USERDATA1_BIT  24U
/**
  * @}
  */

/* Private macro -------------------------------------------------------------*/
/** @defgroup FLASHEx_Private_Macros FLASHEx Private Macros
  * @{
  */
/**
  * @}
  */ 

/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/** @defgroup FLASHEx_Private_Functions FLASHEx Private Functions
 * @{
 */
/* Erase operations */
static void              FLASH_MassErase(void);
void    FLASH_PageErase(uint32_t PageAddress);

/* Option bytes control */
static HAL_StatusTypeDef FLASH_OB_EnableWRP(uint32_t WriteProtectPage);
static HAL_StatusTypeDef FLASH_OB_DisableWRP(uint32_t WriteProtectPage);
static HAL_StatusTypeDef FLASH_OB_RDP_LevelConfig(uint8_t ReadProtectLevel);
static HAL_StatusTypeDef FLASH_OB_UserConfig(uint8_t UserConfig);
static HAL_StatusTypeDef FLASH_OB_ProgramData(uint32_t Address, uint8_t Data);
static uint32_t          FLASH_OB_GetWRP(void);
static uint32_t          FLASH_OB_GetRDP(void);
static uint8_t           FLASH_OB_GetUser(void);

/**
  * @}
  */

/* Exported functions ---------------------------------------------------------*/
/** @defgroup FLASHEx_Exported_Functions FLASHEx Exported Functions
  * @{
  */
  
/** @defgroup FLASHEx_Exported_Functions_Group1 FLASHEx Memory Erasing functions
 *  @brief   FLASH Memory Erasing functions
  *
@verbatim   
  ==============================================================================
                ##### FLASH Erasing Programming functions ##### 
  ==============================================================================

    [..] The FLASH Memory Erasing functions, includes the following functions:
    (+) @ref HAL_FLASHEx_Erase: return only when erase has been done
    (+) @ref HAL_FLASHEx_Erase_IT: end of erase is done when @ref HAL_FLASH_EndOfOperationCallback 
        is called with parameter 0xFFFFFFFF

    [..] Any operation of erase should follow these steps:
    (#) Call the @ref HAL_FLASH_Unlock() function to enable the flash control register and 
        program memory access.
    (#) Call the desired function to erase page.
    (#) Call the @ref HAL_FLASH_Lock() to disable the flash program memory access 
       (recommended to protect the FLASH memory against possible unwanted operation).

@endverbatim
  * @{
  */
  

/**
  * @brief  Perform a mass erase or erase the specified FLASH memory pages
  * @note   To correctly run this function, the @ref HAL_FLASH_Unlock() function
  *         must be called before.
  *         Call the @ref HAL_FLASH_Lock() to disable the flash memory access 
  *         (recommended to protect the FLASH memory against possible unwanted operation)
  * @param[in]  pEraseInit pointer to an FLASH_EraseInitTypeDef structure that
  *         contains the configuration information for the erasing.
  *
  * @param[out]  PageError pointer to variable  that
  *         contains the configuration information on faulty page in case of error
  *         (0xFFFFFFFF means that all the pages have been correctly erased)
  *
  * @retval HAL_StatusTypeDef HAL Status
  */
HAL_StatusTypeDef HAL_FLASHEx_Erase(FLASH_EraseInitTypeDef *pEraseInit, uint32_t *PageError)
{
  HAL_StatusTypeDef status = HAL_ERROR;
  uint32_t address = 0U;

  /* Process Locked */
  __HAL_LOCK(&pFlash);

  /* Check the parameters */
  assert_param(IS_FLASH_TYPEERASE(pEraseInit->TypeErase));

  if (pEraseInit->TypeErase == FLASH_TYPEERASE_MASSERASE)
  {
      /* Mass Erase requested for Bank1 */
      /* Wait for last operation to be completed */
      if (FLASH_WaitForLastOperation((uint32_t)FLASH_TIMEOUT_VALUE) == HAL_OK)
      {
        /*Mass erase to be done*/
        FLASH_MassErase();
        
        /* Wait for last operation to be completed */
        status = FLASH_WaitForLastOperation((uint32_t)FLASH_TIMEOUT_VALUE);
        
        /* If the erase operation is completed, disable the MER Bit */
        CLEAR_BIT(FLASH->CR, FLASH_CR_MER);
      }
  }
  else
  {
    /* Page Erase is requested */
    /* Check the parameters */
    assert_param(IS_FLASH_PROGRAM_ADDRESS(pEraseInit->PageAddress));
    assert_param(IS_FLASH_NB_PAGES(pEraseInit->PageAddress, pEraseInit->NbPages));
    
      /* Page Erase requested on address located on bank1 */
      /* Wait for last operation to be completed */
      if (FLASH_WaitForLastOperation((uint32_t)FLASH_TIMEOUT_VALUE) == HAL_OK)
      {
        /*Initialization of PageError variable*/
        *PageError = 0xFFFFFFFFU;
        
        /* Erase page by page to be done*/
        for(address = pEraseInit->PageAddress;
            address < ((pEraseInit->NbPages * FLASH_PAGE_SIZE) + pEraseInit->PageAddress);
            address += FLASH_PAGE_SIZE)
        {
          FLASH_PageErase(address);
          
          /* Wait for last operation to be completed */
          status = FLASH_WaitForLastOperation((uint32_t)FLASH_TIMEOUT_VALUE);
          
          /* If the erase operation is completed, disable the PER Bit */
          CLEAR_BIT(FLASH->CR, FLASH_CR_PER);
          
          if (status != HAL_OK)
          {
            /* In case of error, stop erase procedure and return the faulty address */
            *PageError = address;
            break;
          }
        }
      }
  }

  /* Process Unlocked */
  __HAL_UNLOCK(&pFlash);

  return status;
}

/**
  * @brief  Perform a mass erase or erase the specified FLASH memory pages with interrupt enabled
  * @note   To correctly run this function, the @ref HAL_FLASH_Unlock() function
  *         must be called before.
  *         Call the @ref HAL_FLASH_Lock() to disable the flash memory access 
  *         (recommended to protect the FLASH memory against possible unwanted operation)
  * @param  pEraseInit pointer to an FLASH_EraseInitTypeDef structure that
  *         contains the configuration information for the erasing.
  *
  * @retval HAL_StatusTypeDef HAL Status
  */
HAL_StatusTypeDef HAL_FLASHEx_Erase_IT(FLASH_EraseInitTypeDef *pEraseInit)
{
  HAL_StatusTypeDef status = HAL_OK;

  /* Process Locked */
  __HAL_LOCK(&pFlash);

  /* If procedure already ongoing, reject the next one */
  if (pFlash.ProcedureOnGoing != FLASH_PROC_NONE)
  {
    return HAL_ERROR;
  }
  
  /* Check the parameters */
  assert_param(IS_FLASH_TYPEERASE(pEraseInit->TypeErase));

  /* Enable End of FLASH Operation and Error source interrupts */
  __HAL_FLASH_ENABLE_IT(FLASH_IT_EOP | FLASH_IT_ERR);

  if (pEraseInit->TypeErase == FLASH_TYPEERASE_MASSERASE)
  {
    /*Mass erase to be done*/
    pFlash.ProcedureOnGoing = FLASH_PROC_MASSERASE;
        FLASH_MassErase();
  }
  else
  {
    /* Erase by page to be done*/

    /* Check the parameters */
    assert_param(IS_FLASH_PROGRAM_ADDRESS(pEraseInit->PageAddress));
    assert_param(IS_FLASH_NB_PAGES(pEraseInit->PageAddress, pEraseInit->NbPages));

    pFlash.ProcedureOnGoing = FLASH_PROC_PAGEERASE;
    pFlash.DataRemaining = pEraseInit->NbPages;
    pFlash.Address = pEraseInit->PageAddress;

    /*Erase 1st page and wait for IT*/
    FLASH_PageErase(pEraseInit->PageAddress);
  }

  return status;
}

/**
  * @}
  */

/** @defgroup FLASHEx_Exported_Functions_Group2 Option Bytes Programming functions
 *  @brief   Option Bytes Programming functions
  *
@verbatim   
  ==============================================================================
                ##### Option Bytes Programming functions ##### 
  ==============================================================================  
    [..]
    This subsection provides a set of functions allowing to control the FLASH 
    option bytes operations.

@endverbatim
  * @{
  */

/**
  * @brief  Erases the FLASH option bytes.
  * @note   This functions erases all option bytes except the Read protection (RDP).
  *         The function @ref HAL_FLASH_Unlock() should be called before to unlock the FLASH interface
  *         The function @ref HAL_FLASH_OB_Unlock() should be called before to unlock the options bytes
  *         The function @ref HAL_FLASH_OB_Launch() should be called after to force the reload of the options bytes
  *         (system reset will occur)
  * @retval HAL status
  */

HAL_StatusTypeDef HAL_FLASHEx_OBErase(void)
{
  uint8_t rdptmp = OB_RDP_LEVEL_0;
  HAL_StatusTypeDef status = HAL_ERROR;

  /* Get the actual read protection Option Byte value */
  rdptmp = FLASH_OB_GetRDP();

  /* Wait for last operation to be completed */
  status = FLASH_WaitForLastOperation((uint32_t)FLASH_TIMEOUT_VALUE);

  if(status == HAL_OK)
  {
    /* Clean the error context */
    pFlash.ErrorCode = HAL_FLASH_ERROR_NONE;

    /* If the previous operation is completed, proceed to erase the option bytes */
    SET_BIT(FLASH->CR, FLASH_CR_OPTER);
    SET_BIT(FLASH->CR, FLASH_CR_STRT);

    /* Wait for last operation to be completed */
    status = FLASH_WaitForLastOperation((uint32_t)FLASH_TIMEOUT_VALUE);

    /* If the erase operation is completed, disable the OPTER Bit */
    CLEAR_BIT(FLASH->CR, FLASH_CR_OPTER);

    if(status == HAL_OK)
    {
      /* Restore the last read protection Option Byte value */
      status = FLASH_OB_RDP_LevelConfig(rdptmp);
    }
  }

  /* Return the erase status */
  return status;
}

/**
  * @brief  Program option bytes
  * @note   The function @ref HAL_FLASH_Unlock() should be called before to unlock the FLASH interface
  *         The function @ref HAL_FLASH_OB_Unlock() should be called before to unlock the options bytes
  *         The function @ref HAL_FLASH_OB_Launch() should be called after to force the reload of the options bytes
  *         (system reset will occur)
  *
  * @param  pOBInit pointer to an FLASH_OBInitStruct structure that
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

  /* Write protection configuration */
  if((pOBInit->OptionType & OPTIONBYTE_WRP) == OPTIONBYTE_WRP)
  {
    assert_param(IS_WRPSTATE(pOBInit->WRPState));
    if (pOBInit->WRPState == OB_WRPSTATE_ENABLE)
    {
      /* Enable of Write protection on the selected page */
      status = FLASH_OB_EnableWRP(pOBInit->WRPPage);
    }
    else
    {
      /* Disable of Write protection on the selected page */
      status = FLASH_OB_DisableWRP(pOBInit->WRPPage);
    }
    if (status != HAL_OK)
    {
      /* Process Unlocked */
      __HAL_UNLOCK(&pFlash);
      return status;
    }
  }

  /* Read protection configuration */
  if((pOBInit->OptionType & OPTIONBYTE_RDP) == OPTIONBYTE_RDP)
  {
    status = FLASH_OB_RDP_LevelConfig(pOBInit->RDPLevel);
    if (status != HAL_OK)
    {
      /* Process Unlocked */
      __HAL_UNLOCK(&pFlash);
      return status;
    }
  }

  /* USER configuration */
  if((pOBInit->OptionType & OPTIONBYTE_USER) == OPTIONBYTE_USER)
  {
    status = FLASH_OB_UserConfig(pOBInit->USERConfig);
    if (status != HAL_OK)
    {
      /* Process Unlocked */
      __HAL_UNLOCK(&pFlash);
      return status;
    }
  }

  /* DATA configuration*/
  if((pOBInit->OptionType & OPTIONBYTE_DATA) == OPTIONBYTE_DATA)
  {
    status = FLASH_OB_ProgramData(pOBInit->DATAAddress, pOBInit->DATAData);
    if (status != HAL_OK)
    {
      /* Process Unlocked */
      __HAL_UNLOCK(&pFlash);
      return status;
    }
  }

  /* Process Unlocked */
  __HAL_UNLOCK(&pFlash);

  return status;
}

/**
  * @brief  Get the Option byte configuration
  * @param  pOBInit pointer to an FLASH_OBInitStruct structure that
  *         contains the configuration information for the programming.
  *
  * @retval None
  */
void HAL_FLASHEx_OBGetConfig(FLASH_OBProgramInitTypeDef *pOBInit)
{
  pOBInit->OptionType = OPTIONBYTE_WRP | OPTIONBYTE_RDP | OPTIONBYTE_USER;

  /*Get WRP*/
  pOBInit->WRPPage = FLASH_OB_GetWRP();

  /*Get RDP Level*/
  pOBInit->RDPLevel = FLASH_OB_GetRDP();

  /*Get USER*/
  pOBInit->USERConfig = FLASH_OB_GetUser();
}

/**
  * @brief  Get the Option byte user data
  * @param  DATAAdress Address of the option byte DATA
  *          This parameter can be one of the following values:
  *            @arg @ref OB_DATA_ADDRESS_DATA0
  *            @arg @ref OB_DATA_ADDRESS_DATA1
  * @retval Value programmed in USER data
  */
uint32_t HAL_FLASHEx_OBGetUserData(uint32_t DATAAdress)
{
  uint32_t value = 0U;
  
  if (DATAAdress == OB_DATA_ADDRESS_DATA0)
  {
    /* Get value programmed in OB USER Data0 */
    value = READ_BIT(FLASH->OBR, FLASH_OBR_DATA0) >> FLASH_POSITION_OB_USERDATA0_BIT;
  }
  else
  {
    /* Get value programmed in OB USER Data1 */
    value = READ_BIT(FLASH->OBR, FLASH_OBR_DATA1) >> FLASH_POSITION_OB_USERDATA1_BIT;
  }
  
  return value;
}

/**
  * @}
  */

/**
  * @}
  */

/** @addtogroup FLASHEx_Private_Functions
 * @{
 */

/**
  * @brief  Full erase of FLASH memory Bank 
  *
  * @retval None
  */
static void FLASH_MassErase(void)
{
  /* Clean the error context */
  pFlash.ErrorCode = HAL_FLASH_ERROR_NONE;

    /* Only bank1 will be erased*/
    SET_BIT(FLASH->CR, FLASH_CR_MER);
    SET_BIT(FLASH->CR, FLASH_CR_STRT);
}

/**
  * @brief  Enable the write protection of the desired pages
  * @note   An option byte erase is done automatically in this function. 
  * @note   When the memory read protection level is selected (RDP level = 1), 
  *         it is not possible to program or erase the flash page i if
  *         debug features are connected or boot code is executed in RAM, even if nWRPi = 1 
  * 
  * @param  WriteProtectPage specifies the page(s) to be write protected.
  *         The value of this parameter depend on device used within the same series 
  * @retval HAL status 
  */
static HAL_StatusTypeDef FLASH_OB_EnableWRP(uint32_t WriteProtectPage)
{
  HAL_StatusTypeDef status = HAL_OK;
  uint16_t WRP0_Data = 0xFFFFU;
#if defined(OB_WRP1_WRP1)
  uint16_t WRP1_Data = 0xFFFFU;
#endif /* OB_WRP1_WRP1 */
#if defined(OB_WRP2_WRP2)
  uint16_t WRP2_Data = 0xFFFFU;
#endif /* OB_WRP2_WRP2 */
#if defined(OB_WRP3_WRP3)
  uint16_t WRP3_Data = 0xFFFFU;
#endif /* OB_WRP3_WRP3 */
  
  /* Check the parameters */
  assert_param(IS_OB_WRP(WriteProtectPage));
    
  /* Get current write protected pages and the new pages to be protected ******/
  WriteProtectPage = (uint32_t)(~((~FLASH_OB_GetWRP()) | WriteProtectPage));
  
#if defined(OB_WRP_PAGES0TO15MASK)
  WRP0_Data = (uint16_t)(WriteProtectPage & OB_WRP_PAGES0TO15MASK);
#elif defined(OB_WRP_PAGES0TO31MASK)
  WRP0_Data = (uint16_t)(WriteProtectPage & OB_WRP_PAGES0TO31MASK);
#endif /* OB_WRP_PAGES0TO31MASK */
  
#if defined(OB_WRP_PAGES16TO31MASK)
  WRP1_Data = (uint16_t)((WriteProtectPage & OB_WRP_PAGES16TO31MASK) >> 8U);
#elif defined(OB_WRP_PAGES32TO63MASK)
  WRP1_Data = (uint16_t)((WriteProtectPage & OB_WRP_PAGES32TO63MASK) >> 8U);
#endif /* OB_WRP_PAGES32TO63MASK */
 
#if defined(OB_WRP_PAGES32TO47MASK)
  WRP2_Data = (uint16_t)((WriteProtectPage & OB_WRP_PAGES32TO47MASK) >> 16U);
#endif /* OB_WRP_PAGES32TO47MASK */

#if defined(OB_WRP_PAGES48TO63MASK)
  WRP3_Data = (uint16_t)((WriteProtectPage & OB_WRP_PAGES48TO63MASK) >> 24U); 
#elif defined(OB_WRP_PAGES48TO127MASK)
  WRP3_Data = (uint16_t)((WriteProtectPage & OB_WRP_PAGES48TO127MASK) >> 24U); 
#endif /* OB_WRP_PAGES48TO63MASK */
  
  /* Wait for last operation to be completed */
  status = FLASH_WaitForLastOperation((uint32_t)FLASH_TIMEOUT_VALUE);

  if(status == HAL_OK)
  { 
    /* Clean the error context */
    pFlash.ErrorCode = HAL_FLASH_ERROR_NONE;

    /* To be able to write again option byte, need to perform a option byte erase */
    status = HAL_FLASHEx_OBErase();
    if (status == HAL_OK)  
    {
      /* Enable write protection */
      SET_BIT(FLASH->CR, FLASH_CR_OPTPG);

#if defined(OB_WRP0_WRP0)
      if(WRP0_Data != 0xFFU)
      {
        OB->WRP0 &= WRP0_Data;
        
        /* Wait for last operation to be completed */
        status = FLASH_WaitForLastOperation((uint32_t)FLASH_TIMEOUT_VALUE);
      }
#endif /* OB_WRP0_WRP0 */

#if defined(OB_WRP1_WRP1)
      if((status == HAL_OK) && (WRP1_Data != 0xFFU))
      {
        OB->WRP1 &= WRP1_Data;
        
        /* Wait for last operation to be completed */
        status = FLASH_WaitForLastOperation((uint32_t)FLASH_TIMEOUT_VALUE);
      }
#endif /* OB_WRP1_WRP1 */

#if defined(OB_WRP2_WRP2)
      if((status == HAL_OK) && (WRP2_Data != 0xFFU))
      {
        OB->WRP2 &= WRP2_Data;
        
        /* Wait for last operation to be completed */
        status = FLASH_WaitForLastOperation((uint32_t)FLASH_TIMEOUT_VALUE);
      }
#endif /* OB_WRP2_WRP2 */

#if defined(OB_WRP3_WRP3)
      if((status == HAL_OK) && (WRP3_Data != 0xFFU))
      {
        OB->WRP3 &= WRP3_Data;
        
        /* Wait for last operation to be completed */
        status = FLASH_WaitForLastOperation((uint32_t)FLASH_TIMEOUT_VALUE);
      }
#endif /* OB_WRP3_WRP3 */

      /* if the program operation is completed, disable the OPTPG Bit */
      CLEAR_BIT(FLASH->CR, FLASH_CR_OPTPG);
    }
  }
  
  return status;
}

/**
  * @brief  Disable the write protection of the desired pages
  * @note   An option byte erase is done automatically in this function. 
  * @note   When the memory read protection level is selected (RDP level = 1), 
  *         it is not possible to program or erase the flash page i if   
  *         debug features are connected or boot code is executed in RAM, even if nWRPi = 1 
  * 
  * @param  WriteProtectPage specifies the page(s) to be write unprotected.
  *         The value of this parameter depend on device used within the same series 
  * @retval HAL status 
  */
static HAL_StatusTypeDef FLASH_OB_DisableWRP(uint32_t WriteProtectPage)
{
  HAL_StatusTypeDef status = HAL_OK;
  uint16_t WRP0_Data = 0xFFFFU;
#if defined(OB_WRP1_WRP1)
  uint16_t WRP1_Data = 0xFFFFU;
#endif /* OB_WRP1_WRP1 */
#if defined(OB_WRP2_WRP2)
  uint16_t WRP2_Data = 0xFFFFU;
#endif /* OB_WRP2_WRP2 */
#if defined(OB_WRP3_WRP3)
  uint16_t WRP3_Data = 0xFFFFU;
#endif /* OB_WRP3_WRP3 */
  
  /* Check the parameters */
  assert_param(IS_OB_WRP(WriteProtectPage));

  /* Get current write protected pages and the new pages to be unprotected ******/
  WriteProtectPage = (FLASH_OB_GetWRP() | WriteProtectPage);

#if defined(OB_WRP_PAGES0TO15MASK)
  WRP0_Data = (uint16_t)(WriteProtectPage & OB_WRP_PAGES0TO15MASK);
#elif defined(OB_WRP_PAGES0TO31MASK)
  WRP0_Data = (uint16_t)(WriteProtectPage & OB_WRP_PAGES0TO31MASK);
#endif /* OB_WRP_PAGES0TO31MASK */
  
#if defined(OB_WRP_PAGES16TO31MASK)
  WRP1_Data = (uint16_t)((WriteProtectPage & OB_WRP_PAGES16TO31MASK) >> 8U);
#elif defined(OB_WRP_PAGES32TO63MASK)
  WRP1_Data = (uint16_t)((WriteProtectPage & OB_WRP_PAGES32TO63MASK) >> 8U);
#endif /* OB_WRP_PAGES32TO63MASK */
 
#if defined(OB_WRP_PAGES32TO47MASK)
  WRP2_Data = (uint16_t)((WriteProtectPage & OB_WRP_PAGES32TO47MASK) >> 16U);
#endif /* OB_WRP_PAGES32TO47MASK */

#if defined(OB_WRP_PAGES48TO63MASK)
  WRP3_Data = (uint16_t)((WriteProtectPage & OB_WRP_PAGES48TO63MASK) >> 24U); 
#elif defined(OB_WRP_PAGES48TO127MASK)
  WRP3_Data = (uint16_t)((WriteProtectPage & OB_WRP_PAGES48TO127MASK) >> 24U); 
#endif /* OB_WRP_PAGES48TO63MASK */

    
  /* Wait for last operation to be completed */
  status = FLASH_WaitForLastOperation((uint32_t)FLASH_TIMEOUT_VALUE);

  if(status == HAL_OK)
  { 
    /* Clean the error context */
    pFlash.ErrorCode = HAL_FLASH_ERROR_NONE;

    /* To be able to write again option byte, need to perform a option byte erase */
    status = HAL_FLASHEx_OBErase();
    if (status == HAL_OK)  
    {
      SET_BIT(FLASH->CR, FLASH_CR_OPTPG);

#if defined(OB_WRP0_WRP0)
      if(WRP0_Data != 0xFFU)
      {
        OB->WRP0 &= WRP0_Data;
        
        /* Wait for last operation to be completed */
        status = FLASH_WaitForLastOperation((uint32_t)FLASH_TIMEOUT_VALUE);
      }
#endif /* OB_WRP0_WRP0 */

#if defined(OB_WRP1_WRP1)
      if((status == HAL_OK) && (WRP1_Data != 0xFFU))
      {
        OB->WRP1 &= WRP1_Data;
        
        /* Wait for last operation to be completed */
        status = FLASH_WaitForLastOperation((uint32_t)FLASH_TIMEOUT_VALUE);
      }
#endif /* OB_WRP1_WRP1 */

#if defined(OB_WRP2_WRP2)
      if((status == HAL_OK) && (WRP2_Data != 0xFFU))
      {
        OB->WRP2 &= WRP2_Data;
        
        /* Wait for last operation to be completed */
        status = FLASH_WaitForLastOperation((uint32_t)FLASH_TIMEOUT_VALUE);
      }
#endif /* OB_WRP2_WRP2 */

#if defined(OB_WRP3_WRP3)
      if((status == HAL_OK) && (WRP3_Data != 0xFFU))
      {
        OB->WRP3 &= WRP3_Data;
        
        /* Wait for last operation to be completed */
        status = FLASH_WaitForLastOperation((uint32_t)FLASH_TIMEOUT_VALUE);
      }
#endif /* OB_WRP3_WRP3 */

      /* if the program operation is completed, disable the OPTPG Bit */
      CLEAR_BIT(FLASH->CR, FLASH_CR_OPTPG);
    }
  }
  return status;
}

/**
  * @brief  Set the read protection level.
  * @param  ReadProtectLevel specifies the read protection level.
  *         This parameter can be one of the following values:
  *            @arg @ref OB_RDP_LEVEL_0 No protection
  *            @arg @ref OB_RDP_LEVEL_1 Read protection of the memory
  *            @arg @ref OB_RDP_LEVEL_2 Full chip protection
  * @note   Warning: When enabling OB_RDP level 2 it's no more possible to go back to level 1 or 0
  * @retval HAL status
  */
static HAL_StatusTypeDef FLASH_OB_RDP_LevelConfig(uint8_t ReadProtectLevel)
{
  HAL_StatusTypeDef status = HAL_OK;
  
  /* Check the parameters */
  assert_param(IS_OB_RDP_LEVEL(ReadProtectLevel));
  
  /* Wait for last operation to be completed */
  status = FLASH_WaitForLastOperation((uint32_t)FLASH_TIMEOUT_VALUE);
  
  if(status == HAL_OK)
  { 
    /* Clean the error context */
    pFlash.ErrorCode = HAL_FLASH_ERROR_NONE;
    
    /* If the previous operation is completed, proceed to erase the option bytes */
    SET_BIT(FLASH->CR, FLASH_CR_OPTER);
    SET_BIT(FLASH->CR, FLASH_CR_STRT);

    /* Wait for last operation to be completed */
    status = FLASH_WaitForLastOperation((uint32_t)FLASH_TIMEOUT_VALUE);

    /* If the erase operation is completed, disable the OPTER Bit */
    CLEAR_BIT(FLASH->CR, FLASH_CR_OPTER);

    if(status == HAL_OK)
    {
      /* Enable the Option Bytes Programming operation */
      SET_BIT(FLASH->CR, FLASH_CR_OPTPG);
      
      WRITE_REG(OB->RDP, ReadProtectLevel);
      
      /* Wait for last operation to be completed */
      status = FLASH_WaitForLastOperation((uint32_t)FLASH_TIMEOUT_VALUE); 
      
      /* if the program operation is completed, disable the OPTPG Bit */
      CLEAR_BIT(FLASH->CR, FLASH_CR_OPTPG);
    }
  }
  
  return status;
}

/**
  * @brief  Program the FLASH User Option Byte.    
  * @note   Programming of the OB should be performed only after an erase (otherwise PGERR occurs)
  * @param  UserConfig The FLASH User Option Bytes values: IWDG_SW(Bit0), RST_STOP(Bit1), RST_STDBY(Bit2), nBOOT1(Bit4),
  *         VDDA_Analog_Monitoring(Bit5) and SRAM_Parity_Enable(Bit6). 
  *         For few devices, following option bytes are available: nBOOT0(Bit3) & BOOT_SEL(Bit7).
  * @retval HAL status
  */
static HAL_StatusTypeDef FLASH_OB_UserConfig(uint8_t UserConfig)
{
  HAL_StatusTypeDef status = HAL_OK;

  /* Check the parameters */
  assert_param(IS_OB_IWDG_SOURCE((UserConfig&OB_IWDG_SW)));
  assert_param(IS_OB_STOP_SOURCE((UserConfig&OB_STOP_NO_RST)));
  assert_param(IS_OB_STDBY_SOURCE((UserConfig&OB_STDBY_NO_RST)));
  assert_param(IS_OB_BOOT1((UserConfig&OB_BOOT1_SET)));
  assert_param(IS_OB_VDDA_ANALOG((UserConfig&OB_VDDA_ANALOG_ON)));
  assert_param(IS_OB_SRAM_PARITY((UserConfig&OB_SRAM_PARITY_RESET)));
#if defined(FLASH_OBR_BOOT_SEL)
  assert_param(IS_OB_BOOT_SEL((UserConfig&OB_BOOT_SEL_SET)));
  assert_param(IS_OB_BOOT0((UserConfig&OB_BOOT0_SET)));
#endif /* FLASH_OBR_BOOT_SEL */

  /* Wait for last operation to be completed */
  status = FLASH_WaitForLastOperation((uint32_t)FLASH_TIMEOUT_VALUE);
  
  if(status == HAL_OK)
  {     
    /* Clean the error context */
    pFlash.ErrorCode = HAL_FLASH_ERROR_NONE;

    /* Enable the Option Bytes Programming operation */
    SET_BIT(FLASH->CR, FLASH_CR_OPTPG); 
 
#if defined(FLASH_OBR_BOOT_SEL)
    OB->USER = UserConfig;
#else
    OB->USER = (UserConfig | 0x88U);
#endif

    /* Wait for last operation to be completed */
    status = FLASH_WaitForLastOperation((uint32_t)FLASH_TIMEOUT_VALUE);

    /* if the program operation is completed, disable the OPTPG Bit */
    CLEAR_BIT(FLASH->CR, FLASH_CR_OPTPG);
  }
  
  return status; 
}

/**
  * @brief  Programs a half word at a specified Option Byte Data address.
  * @note   The function @ref HAL_FLASH_Unlock() should be called before to unlock the FLASH interface
  *         The function @ref HAL_FLASH_OB_Unlock() should be called before to unlock the options bytes
  *         The function @ref HAL_FLASH_OB_Launch() should be called after to force the reload of the options bytes 
  *         (system reset will occur)
  *         Programming of the OB should be performed only after an erase (otherwise PGERR occurs)
  * @param  Address specifies the address to be programmed.
  *         This parameter can be 0x1FFFF804 or 0x1FFFF806. 
  * @param  Data specifies the data to be programmed.
  * @retval HAL status
  */
static HAL_StatusTypeDef FLASH_OB_ProgramData(uint32_t Address, uint8_t Data)
{
  HAL_StatusTypeDef status = HAL_ERROR;
  
  /* Check the parameters */
  assert_param(IS_OB_DATA_ADDRESS(Address));
  
  /* Wait for last operation to be completed */
  status = FLASH_WaitForLastOperation((uint32_t)FLASH_TIMEOUT_VALUE);
  
  if(status == HAL_OK)
  {
    /* Clean the error context */
    pFlash.ErrorCode = HAL_FLASH_ERROR_NONE;

    /* Enables the Option Bytes Programming operation */
    SET_BIT(FLASH->CR, FLASH_CR_OPTPG); 
    *(__IO uint16_t*)Address = Data;
    
    /* Wait for last operation to be completed */
    status = FLASH_WaitForLastOperation((uint32_t)FLASH_TIMEOUT_VALUE);
    
    /* If the program operation is completed, disable the OPTPG Bit */
    CLEAR_BIT(FLASH->CR, FLASH_CR_OPTPG);
  }
  /* Return the Option Byte Data Program Status */
  return status;
}

/**
  * @brief  Return the FLASH Write Protection Option Bytes value.
  * @retval The FLASH Write Protection Option Bytes value
  */
static uint32_t FLASH_OB_GetWRP(void)
{
  /* Return the FLASH write protection Register value */
  return (uint32_t)(READ_REG(FLASH->WRPR));
}

/**
  * @brief  Returns the FLASH Read Protection level.
  * @retval FLASH RDP level
  *         This parameter can be one of the following values:
  *            @arg @ref OB_RDP_LEVEL_0 No protection
  *            @arg @ref OB_RDP_LEVEL_1 Read protection of the memory
  *            @arg @ref OB_RDP_LEVEL_2 Full chip protection
  */
static uint32_t FLASH_OB_GetRDP(void)
{
  uint32_t tmp_reg;
  
  /* Read RDP level bits */
  tmp_reg = READ_BIT(FLASH->OBR, (FLASH_OBR_RDPRT1 | FLASH_OBR_RDPRT2));

  if (tmp_reg == 0U)
  {
    return OB_RDP_LEVEL_0;
  }
  else if ((tmp_reg & FLASH_OBR_RDPRT2) == FLASH_OBR_RDPRT2)
  {
    return OB_RDP_LEVEL_2;
  }
  else 
  {
    return OB_RDP_LEVEL_1;
  }
}

/**
  * @brief  Return the FLASH User Option Byte value.
  * @retval  The FLASH User Option Bytes values: IWDG_SW(Bit0), RST_STOP(Bit1), RST_STDBY(Bit2), nBOOT1(Bit4),
  *         VDDA_Analog_Monitoring(Bit5) and SRAM_Parity_Enable(Bit6). 
  *         For few devices, following option bytes are available: nBOOT0(Bit3) & BOOT_SEL(Bit7).
  */
static uint8_t FLASH_OB_GetUser(void)
{
  /* Return the User Option Byte */
  return (uint8_t)((READ_REG(FLASH->OBR) & FLASH_OBR_USER) >> FLASH_POSITION_IWDGSW_BIT);
}

/**
  * @}
  */

/**
  * @}
  */

/** @addtogroup FLASH
  * @{
  */

/** @addtogroup FLASH_Private_Functions
 * @{
 */

/**
  * @brief  Erase the specified FLASH memory page
  * @param  PageAddress FLASH page to erase
  *         The value of this parameter depend on device used within the same series      
  * 
  * @retval None
  */
void FLASH_PageErase(uint32_t PageAddress)
{
  /* Clean the error context */
  pFlash.ErrorCode = HAL_FLASH_ERROR_NONE;

    /* Proceed to erase the page */
    SET_BIT(FLASH->CR, FLASH_CR_PER);
    WRITE_REG(FLASH->AR, PageAddress);
    SET_BIT(FLASH->CR, FLASH_CR_STRT);
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
