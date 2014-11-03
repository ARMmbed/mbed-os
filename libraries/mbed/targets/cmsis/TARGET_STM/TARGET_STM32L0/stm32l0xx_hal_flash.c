/**
  ******************************************************************************
  * @file    stm32l0xx_hal_flash.c
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
                        ##### FLASH peripheral features #####
  ==============================================================================
           
  [..] The Flash memory interface manages CPU accesses to the Flash memory. 
       It implements the erase and program Flash memory operations 
       and the read and write protection mechanisms.

  [..] The FLASH main features are:
      (+) Flash memory read operations
      (+) Flash memory program/erase operations
      (+) Read / write protections
      (+) Option Bytes programming
           
                     ##### How to use this driver #####
  ==============================================================================
    [..]                             
      This driver provides functions and macros to configure and program the FLASH 
      memory of all STM32L0xx devices.
    
      (#) FLASH Memory IO Programming functions: 
           (++) Lock and Unlock the FLASH interface using HAL_FLASH_Unlock() and 
                HAL_FLASH_Lock() functions
           (++) Program functions: byte, half word and word 
           (++) There Two modes of programming :
                (+++) Polling mode using HAL_FLASH_Program() function
                (+++) Interrupt mode using HAL_FLASH_Program_IT() function
    
      (#) Interrupts and flags management functions : 
           (++) Handle FLASH interrupts by calling HAL_FLASH_IRQHandler()
           (++) Wait for last FLASH operation according to its status
           (++) Get error flag status by calling HAL_GetErrorCode()          

    [..] 
      In addition to these functions, this driver includes a set of macros allowing
      to handle the following operations:
       (+) Set the latency
       (+) Enable/Disable the prefetch buffer
       (+) Enable/Disable the preread buffer
       (+) Enable/Disable the Flash power-down
       (+) Enable/Disable the FLASH interrupts
       (+) Monitor the FLASH flags status

 ===============================================================================
                  ##### Programming operation functions #####
 ===============================================================================  
    [..]
    This subsection provides a set of functions allowing to manage the FLASH 
    program operations.

   [..] The FLASH Memory Programming functions, includes the following functions:
    (+) HAL_FLASH_Unlock(void);
    (+) HAL_FLASH_Lock(void);
    (+) HAL_FLASH_Program(uint32_t TypeProgram, uint32_t Address, uint32_t Data)
    (+) HAL_FLASH_Program_IT(uint32_t TypeProgram, uint32_t Address, uint32_t Data)
   
    [..] Any operation of erase or program should follow these steps:
    (#) Call the HAL_FLASH_Unlock() function to enable the flash control register and 
        program memory access.
    (#) Call the desired function to erase page or program data.
    (#) Call the HAL_FLASH_Lock() to disable the flash program memory access 
       (recommended to protect the FLASH memory against possible unwanted operation).

  ==============================================================================
                ##### Option Bytes Programming functions ##### 
  ==============================================================================  

    [..] The FLASH_Option Bytes Programming_functions, includes the following functions:
    (+) HAL_FLASH_OB_Unlock(void);
    (+) HAL_FLASH_OB_Lock(void);
    (+) HAL_FLASH_OB_Launch(void);
    (+) HAL_FLASHEx_OBProgram(FLASH_OBProgramInitTypeDef *pOBInit);
    (+) HAL_FLASHEx_OBGetConfig(FLASH_OBProgramInitTypeDef *pOBInit);
   
    [..] Any operation of erase or program should follow these steps:
    (#) Call the HAL_FLASH_OB_Unlock() function to enable the Flash option control 
        register access.
    (#) Call the following functions to program the desired option bytes.
        (++) HAL_FLASHEx_OBProgram(FLASH_OBProgramInitTypeDef *pOBInit);      
    (#) Once all needed option bytes to be programmed are correctly written, call the
        HAL_FLASH_OB_Launch(void) function to launch the Option Bytes programming process.
    (#) Call the HAL_FLASH_OB_Lock() to disable the Flash option control register access (recommended
        to protect the option Bytes against possible unwanted operations).

    [..] Proprietary code Read Out Protection (PcROP):    
    (#) The PcROP sector is selected by using the same option bytes as the Write
        protection (nWRPi bits). As a result, these 2 options are exclusive each other.
    (#) In order to activate the PcROP (change the function of the nWRPi option bits), 
        the SPRMOD option bit must be activated.
    (#) The active value of nWRPi bits is inverted when PCROP mode is active, this
        means: if SPRMOD = 1 and nWRPi = 1 (default value), then the user page "i"
        is read/write protected.
    (#) To activate PCROP mode for Flash page(s), you need to follow the sequence below:
        (++) For page(s) within the first 64KB of the Flash, use this function 
             HAL_FLASHEx_AdvOBProgram with PCROPState = PCROPSTATE_ENABLE.


  *  @endverbatim
  *                      
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

/** @defgroup FLASH 
  * @brief FLASH driver modules
  * @{
  */ 

#ifdef HAL_FLASH_MODULE_ENABLED

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
#define HAL_FLASH_TIMEOUT_VALUE   ((uint32_t)0xFFFF0)
    
/* FLASH Mask */
#define WRP01_MASK                 ((uint32_t)0x0000FFFF)
#define PAGESIZE                   ((uint32_t)0x00000080)
    
/* Private macro -------------------------------------------------------------*/
/*Variables used for Erase sectors under interruption*/
FLASH_ProcessTypeDef pFlash;

/* Private function prototypes -----------------------------------------------*/
static void FLASH_Program_Word(uint32_t Address, uint32_t Data);
HAL_StatusTypeDef FLASH_WaitForLastOperation(uint32_t Timeout);
static void FLASH_SetErrorCode(void);


/* Private functions ---------------------------------------------------------*/
 
/** @defgroup FLASH_Private_Functions
  * @{
  */ 

/** @defgroup FLASH_Group1 Programming operation functions 
 *  @brief   Programming operation functions 
 *
@verbatim   
@endverbatim
  * @{
  */
/**
  * @brief  Program word at a specified address
  * @param  TypeProgram:  Indicate the way to program at a specified address.
  *                           This parameter can be a value of @ref FLASH_Type_Program
  * @param  Address:  specifies the address to be programmed.
  * @param  Data: specifies the data to be programmed
  * 
  * @retval HAL_StatusTypeDef HAL Status
  */
HAL_StatusTypeDef HAL_FLASH_Program(uint32_t TypeProgram, uint32_t Address, uint32_t Data)
{
  HAL_StatusTypeDef status = HAL_ERROR;
  
  /* Process Locked */
  __HAL_LOCK(&pFlash);

  /* Check the parameters */
  assert_param(IS_TYPEPROGRAM(TypeProgram));

  /* Wait for last operation to be completed */
  status = FLASH_WaitForLastOperation((uint32_t)HAL_FLASH_TIMEOUT_VALUE);
  
  if(status == HAL_OK)
  {
    if(TypeProgram == TYPEPROGRAM_WORD)
    {
      /*Program word (32-bit) at a specified address.*/
      FLASH_Program_Word(Address, (uint32_t) Data);
    }
    /* Wait for last operation to be completed */
    status = FLASH_WaitForLastOperation((uint32_t)HAL_FLASH_TIMEOUT_VALUE);
    
    /* Reset PROG bit */
    FLASH->PECR &= ~FLASH_PECR_PROG;
  }

  /* Process Unlocked */
  __HAL_UNLOCK(&pFlash);

  return status;  
}

/**
  * @brief   Program word at a specified address  with interrupt enabled.
  * @param  TypeProgram:  Indicate the way to program at a specified address.
  *                           This parameter can be a value of @ref FLASH_Type_Program
  * @param  Address:  specifies the address to be programmed.
  * @param  Data: specifies the data to be programmed
  * 
  * @retval HAL_StatusTypeDef HAL Status
  */
HAL_StatusTypeDef HAL_FLASH_Program_IT(uint32_t TypeProgram, uint32_t Address, uint32_t Data)
{
  HAL_StatusTypeDef status = HAL_OK;
  
  /* Process Locked */
  __HAL_LOCK(&pFlash);

  /* Check the parameters */
  assert_param(IS_TYPEPROGRAM(TypeProgram));

  /* Enable End of FLASH Operation interrupt */
  __HAL_FLASH_ENABLE_IT(FLASH_IT_EOP);
  
  /* Enable Error source interrupt */
  __HAL_FLASH_ENABLE_IT(FLASH_IT_ERR);
  
  /* Clear pending flags (if any) */  
  __HAL_FLASH_CLEAR_FLAG(FLASH_FLAG_EOP    | FLASH_FLAG_ENDHV | FLASH_FLAG_WRPERR |\
                         FLASH_FLAG_PGAERR | FLASH_FLAG_SIZERR| FLASH_FLAG_OPTVERR |\
                         FLASH_FLAG_RDERR  | FLASH_FLAG_NOTZEROERR);  

  pFlash.ProcedureOnGoing = FLASH_PROC_PROGRAM;
  pFlash.Address = Address;

  if(TypeProgram == TYPEPROGRAM_WORD)
  {
    /*Program word (32-bit) at a specified address.*/
    FLASH_Program_Word(Address, (uint32_t) Data);
  }
  
  /* Process Unlocked */
  __HAL_UNLOCK(&pFlash);
  
  return status;
}

/**
  * @brief This function handles FLASH interrupt request.
  * @param  None
  * @retval None
  */
void HAL_FLASH_IRQHandler(void)
{
  uint32_t temp;
  
  /* If the program operation is completed, disable the PROG Bit */
  FLASH->PECR &= (~FLASH_PECR_PROG);

  /* If the erase operation is completed, disable the ERASE Bit */
  FLASH->PECR &= (~FLASH_PECR_ERASE);

  /* Check FLASH End of Operation flag  */
  if(__HAL_FLASH_GET_FLAG(FLASH_FLAG_EOP))
  {
    if(pFlash.ProcedureOnGoing == FLASH_PROC_PAGEERASE)
    {
      /*Nb of sector to erased can be decreased*/
      pFlash.NbPagesToErase--;

      /* Check if there are still sectors to erase*/
      if(pFlash.NbPagesToErase != 0)
      {
        temp = pFlash.Page;
        /*Indicate user which sector has been erased*/
        HAL_FLASH_EndOfOperationCallback(temp);

        /* Clear pending flags (if any) */  
        __HAL_FLASH_CLEAR_FLAG(FLASH_FLAG_EOP    | FLASH_FLAG_ENDHV | FLASH_FLAG_WRPERR |\
                              FLASH_FLAG_PGAERR | FLASH_FLAG_SIZERR| FLASH_FLAG_OPTVERR |\
                              FLASH_FLAG_RDERR  | FLASH_FLAG_NOTZEROERR);  

        /*Increment sector number*/
        temp = pFlash.Page + PAGESIZE;
        pFlash.Page = pFlash.Page + PAGESIZE;
        FLASH_Erase_Page(temp);
      }
      else
      {
        /*No more sectors to Erase, user callback can be called.*/
        /*Reset Sector and stop Erase sectors procedure*/
        pFlash.Page = temp = 0xFFFFFFFF;
        pFlash.ProcedureOnGoing = FLASH_PROC_NONE;
        /* FLASH EOP interrupt user callback */
        HAL_FLASH_EndOfOperationCallback(temp);
        /* Clear FLASH End of Operation pending bit */
        __HAL_FLASH_CLEAR_FLAG(FLASH_FLAG_EOP);
      }
    }
    else 
    {
      if(pFlash.ProcedureOnGoing  == FLASH_PROC_PROGRAM)
      {
        /*Program ended. Return the selected address*/
        /* FLASH EOP interrupt user callback */
        HAL_FLASH_EndOfOperationCallback(pFlash.Address);
      }
      pFlash.ProcedureOnGoing = FLASH_PROC_NONE;
      /* Clear FLASH End of Operation pending bit */
      __HAL_FLASH_CLEAR_FLAG(FLASH_FLAG_EOP);
    }

  }
  /* Check FLASH operation error flags */
  if( (__HAL_FLASH_GET_FLAG(FLASH_FLAG_EOP) != RESET) || (__HAL_FLASH_GET_FLAG(FLASH_FLAG_ENDHV) != RESET) || \
      (__HAL_FLASH_GET_FLAG(FLASH_FLAG_WRPERR) != RESET)|| (__HAL_FLASH_GET_FLAG(FLASH_FLAG_PGAERR) != RESET) || \
      (__HAL_FLASH_GET_FLAG(FLASH_FLAG_SIZERR) != RESET)|| (__HAL_FLASH_GET_FLAG(FLASH_FLAG_OPTVERR) != RESET) || \
      (__HAL_FLASH_GET_FLAG(FLASH_FLAG_RDERR) != RESET) || (__HAL_FLASH_GET_FLAG(FLASH_FLAG_NOTZEROERR) != RESET))
  {
    if(pFlash.ProcedureOnGoing == FLASH_PROC_PAGEERASE)
    {
      /*return the faulty sector*/
      temp = pFlash.Page;
      pFlash.Page = 0xFFFFFFFF;
    }
    else
    {
      /*retrun the faulty address*/
      temp = pFlash.Address;
    }
    
    /*Save the Error code*/
    FLASH_SetErrorCode();

    /* FLASH error interrupt user callback */
    HAL_FLASH_OperationErrorCallback(temp);
    /* Clear FLASH error pending bits */
    __HAL_FLASH_CLEAR_FLAG(FLASH_FLAG_EOP    | FLASH_FLAG_ENDHV | FLASH_FLAG_WRPERR |\
                           FLASH_FLAG_PGAERR | FLASH_FLAG_SIZERR| FLASH_FLAG_OPTVERR |\
                           FLASH_FLAG_RDERR  | FLASH_FLAG_NOTZEROERR);

    /*Stop the procedure ongoing*/
    pFlash.ProcedureOnGoing = FLASH_PROC_NONE;
  }
  
  if(pFlash.ProcedureOnGoing == FLASH_PROC_NONE)
  {
    /* Disable End of FLASH Operation interrupt */
    __HAL_FLASH_DISABLE_IT(FLASH_IT_EOP);

    /* Disable Error source interrupt */
    __HAL_FLASH_DISABLE_IT(FLASH_IT_ERR);

    /* Process Unlocked */
    __HAL_UNLOCK(&pFlash);
  }
    
} 

/**
  * @brief  FLASH end of operation interrupt callback
  * @param  ReturnValue: The value saved in this parameter depends on the ongoing procedure
  *                   Pages_Erase: Sector which has been erased 
  *                    (if 0xFFFFFFFF, it means that all the selected sectors have been erased)
  *                   Program: Address which was selected for data program
  * @retval none
  */
__weak void HAL_FLASH_EndOfOperationCallback(uint32_t ReturnValue)
{
  /* NOTE : This function Should not be modified, when the callback is needed,
            the HAL_FLASH_EndOfOperationCallback could be implemented in the user file
   */ 
}

/**
  * @brief  FLASH operation error interrupt callback
  * @param  ReturnValue: The value saved in this parameter depends on the ongoing procedure
  *                   Pages_Erase: Sector number which returned an error
  *                   Program: Address which was selected for data program
  * @retval none
  */
__weak void HAL_FLASH_OperationErrorCallback(uint32_t ReturnValue)
{
  /* NOTE : This function Should not be modified, when the callback is needed,
            the HAL_FLASH_OperationErrorCallback could be implemented in the user file
   */ 
}

/**
  * @}
  */

/** @defgroup FLASH_Group2 Peripheral Control functions 
 *  @brief   management functions 
 *
@verbatim   
 ===============================================================================
                      ##### Peripheral Control functions #####
 ===============================================================================  
    [..]
    This subsection provides a set of functions allowing to control the FLASH 
    memory operations.

@endverbatim
  * @{
  */  
/**
  * @brief  Unlock the FLASH control register access
  * @param  None
  * @retval HAL_StatusTypeDef HAL Status
  */
HAL_StatusTypeDef HAL_FLASH_Unlock(void)  
{
  if((FLASH->PECR & FLASH_PECR_PRGLOCK) != RESET)
  {
    if((FLASH->PECR & FLASH_PECR_PELOCK) != RESET)
     {  
       /* Unlocking the Data memory and FLASH_PECR register access*/
         FLASH->PEKEYR = FLASH_PEKEY1;
         FLASH->PEKEYR = FLASH_PEKEY2;
     }
 
    /* Unlocking the program memory access */
    FLASH->PRGKEYR = FLASH_PRGKEY1;
    FLASH->PRGKEYR = FLASH_PRGKEY2;  
  }
  else
  {
    return HAL_ERROR;
  }
  
  return HAL_OK; 
}

/**
  * @brief  Locks the FLASH control register access
  * @param  None
  * @retval HAL_StatusTypeDef HAL Status
  */
HAL_StatusTypeDef HAL_FLASH_Lock(void)
{
  /* Set the PRGLOCK Bit to lock the program memory access */
  FLASH->PECR |= FLASH_PECR_PRGLOCK;
  
  return HAL_OK;
}

/**
  * @brief  Unlock the FLASH Option Control Registers access.
  * @param  None
  * @retval HAL_StatusTypeDef HAL Status
  */
HAL_StatusTypeDef HAL_FLASH_OB_Unlock(void)
{
  if((FLASH->PECR & FLASH_PECR_OPTLOCK) != RESET)
  {
      if((FLASH->PECR & FLASH_PECR_PELOCK) != RESET)
     {  
       /* Unlocking the Data memory and FLASH_PECR register access*/
         FLASH->PEKEYR = FLASH_PEKEY1;
         FLASH->PEKEYR = FLASH_PEKEY2;
     }
  
    /* Unlocking the option bytes block access */
    FLASH->OPTKEYR = FLASH_OPTKEY1;
    FLASH->OPTKEYR = FLASH_OPTKEY2;
  }
  else
  {
    return HAL_ERROR;
  }  
  
  return HAL_OK;
}

/**
  * @brief  Lock the FLASH Option Control Registers access.
  * @param  None
  * @retval HAL_StatusTypeDef HAL Status 
  */
HAL_StatusTypeDef HAL_FLASH_OB_Lock(void)
{
  /* Set the OPTLOCK Bit to lock the option bytes block access */
  FLASH->PECR |= FLASH_PECR_OPTLOCK;

  return HAL_OK;  
}

/**
  * @brief  Launch the option byte loading.
  * @param  None
  * @retval HAL_StatusTypeDef HAL Status
  */
HAL_StatusTypeDef HAL_FLASH_OB_Launch(void)
{
  /* Set the OBL_Launch bit to lauch the option byte loading */
  FLASH->PECR |= FLASH_PECR_OBL_LAUNCH;
  
  /* Wait for last operation to be completed */
  return(FLASH_WaitForLastOperation((uint32_t)HAL_FLASH_TIMEOUT_VALUE));  
}

/**
  * @}
  */

/** @defgroup FLASH_Group3 Peripheral State and Errors functions 
 *  @brief   Peripheral Errors functions 
 *
@verbatim   
 ===============================================================================
                ##### Peripheral Errors functions #####
 ===============================================================================  
    [..]
    This subsection permit to get in run-time Errors of the FLASH peripheral.

@endverbatim
  * @{
  */

/**
  * @brief  Get the specific FLASH error flag.
  * @param  None
  * @retval FLASH_ErrorCode: The returned value can be:
  *            @arg FLASH_ERROR_RD: FLASH Read Protection error flag (PCROP)
  *            @arg FLASH_ERROR_ENDHV: FLASH Programming Sequence error flag 
  *            @arg FLASH_ERROR_SIZE: FLASH Programming Parallelism error flag  
  *            @arg FLASH_ERROR_PGA: FLASH Programming Alignment error flag
  *            @arg FLASH_ERROR_WRP: FLASH Write protected error flag
  *            @arg FLASH_ERROR_OPTV: FLASH Option valid error flag 
  *            @arg FLASH_ERROR_NOTZERO: FLASH write operation is done in a not-erased region 
  */
FLASH_ErrorTypeDef HAL_FLASH_GetError(void)
{ 
   return pFlash.ErrorCode;
}  

/**
  * @}
  */

/**
  * @}
  */

/**
  * @brief  Wait for a FLASH operation to complete.
  * @param  Timeout: maximum flash operationtimeout
  * @retval HAL_StatusTypeDef HAL Status
  */
HAL_StatusTypeDef FLASH_WaitForLastOperation(uint32_t Timeout)
{
  uint32_t tickstart = 0;

  /* Wait for the FLASH operation to complete by polling on BUSY flag to be reset.
     Even if the FLASH operation fails, the BUSY flag will be reset and an error
     flag will be set */
    
  tickstart = HAL_GetTick();
     
  while(__HAL_FLASH_GET_FLAG(FLASH_FLAG_BSY)) 
  { 
    if(Timeout != HAL_MAX_DELAY)
    {
      if((Timeout == 0)||((HAL_GetTick() - tickstart ) > Timeout))
      {
        return HAL_TIMEOUT;
      }
    } 
  }
  

   if( (__HAL_FLASH_GET_FLAG(FLASH_FLAG_WRPERR) != RESET) ||(__HAL_FLASH_GET_FLAG(FLASH_FLAG_PGAERR) != RESET) || \
      (__HAL_FLASH_GET_FLAG(FLASH_FLAG_SIZERR) != RESET)||(__HAL_FLASH_GET_FLAG(FLASH_FLAG_OPTVERR) != RESET) || \
      (__HAL_FLASH_GET_FLAG(FLASH_FLAG_RDERR) != RESET) ||(__HAL_FLASH_GET_FLAG(FLASH_FLAG_NOTZEROERR) != RESET))
    {
    /*Save the error code*/
    FLASH_SetErrorCode();
    return HAL_ERROR;
   }

  /* If there is an error flag set */
  return HAL_OK;  
}

/**
  * @brief  Set the specific FLASH error flag.
  * @param  None
  * @retval None
  */
static void FLASH_SetErrorCode(void)
{  
  if(__HAL_FLASH_GET_FLAG(FLASH_FLAG_ENDHV))
  {
    pFlash.ErrorCode = FLASH_ERROR_ENDHV;
  }
  if(__HAL_FLASH_GET_FLAG(FLASH_FLAG_WRPERR))
  { 
    pFlash.ErrorCode = FLASH_ERROR_WRP;
  }
  if(__HAL_FLASH_GET_FLAG(FLASH_FLAG_PGAERR))
  { 
    pFlash.ErrorCode = FLASH_ERROR_PGA;
  }
  if(__HAL_FLASH_GET_FLAG(FLASH_FLAG_SIZERR))
  { 
    pFlash.ErrorCode = FLASH_ERROR_SIZE;
  }
  if(__HAL_FLASH_GET_FLAG(FLASH_FLAG_OPTVERR))
  { 
    pFlash.ErrorCode = FLASH_ERROR_OPTV;
  }
  if(__HAL_FLASH_GET_FLAG(FLASH_FLAG_RDERR))
  { 
    pFlash.ErrorCode = FLASH_ERROR_RD;
  }
  if(__HAL_FLASH_GET_FLAG(FLASH_FLAG_NOTZEROERR))
  { 
   pFlash.ErrorCode = FLASH_ERROR_NOTZERO;
  }
} 

/**
  * @brief  Erases a specified page in program memory.
  * @note   To correctly run this function, the HAL_FLASH_Unlock() function
  *         must be called before.
  *         Call the HAL_FLASH_Lock() to disable the flash memory access 
  *         (recommended to protect the FLASH memory against possible unwanted operation)
  * @param  Page_Address: The page address in program memory to be erased.
  * @note   A Page is erased in the Program memory only if the address to load 
  *         is the start address of a page (multiple of 256 bytes).
  * @retval HAL_StatusTypeDef HAL Status
  */
void FLASH_Erase_Page(uint32_t Page_Address)
{
  /* Set the ERASE bit */
    FLASH->PECR |= FLASH_PECR_ERASE;

    /* Set PROG bit */
    FLASH->PECR |= FLASH_PECR_PROG;
  
    /* Write 00000000h to the first word of the program page to erase */
    *(__IO uint32_t *)Page_Address = 0x00000000;
}


/**
  * @brief  Program word (32-bit) at a specified address.
  * @param  Address: specifies the address to be programmed.
  * @param  Data: specifies the data to be programmed.
  * @retval None
  */
static void FLASH_Program_Word(uint32_t Address, uint32_t Data)
{
  /* Check the parameters */
  assert_param(IS_FLASH_PROGRAM_ADDRESS(Address));
  
  /* Set PROG bit */
  FLASH->PECR |= FLASH_PECR_PROG;
  
  *(__IO uint32_t*)Address = Data;
}

#endif /* HAL_FLASH_MODULE_ENABLED */     
/**
  * @}
  */
   
/**
  * @}
  */ 


     
/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
