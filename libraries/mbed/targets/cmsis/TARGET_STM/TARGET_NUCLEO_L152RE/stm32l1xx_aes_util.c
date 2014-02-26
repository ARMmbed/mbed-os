/**
  ******************************************************************************
  * @file    stm32l1xx_aes_util.c
  * @author  MCD Application Team
  * @version V1.3.0
  * @date    31-January-2014
  * @brief   This file provides high level functions to encrypt and decrypt an 
  *          input message using AES in ECB/CBC/CTR modes.
  *
  *  @verbatim

================================================================================
                        ##### How to use this driver #####
================================================================================
          [..]
           (#) Enable The AES controller clock using 
               RCC_AHBPeriphClockCmd(RCC_AHBPeriph_AES, ENABLE); function.

           (#) Use AES_ECB_Encrypt() function to encrypt an input message in ECB mode.
           (#) Use AES_ECB_Decrypt() function to decrypt an input message in ECB mode.

           (#) Use AES_CBC_Encrypt() function to encrypt an input message in CBC mode.
           (#) Use AES_CBC_Decrypt() function to decrypt an input message in CBC mode.

           (#) Use AES_CTR_Encrypt() function to encrypt an input message in CTR mode.
           (#) Use AES_CTR_Decrypt() function to decrypt an input message in CTR mode.

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
#include "stm32l1xx_aes.h"

/** @addtogroup STM32L1xx_StdPeriph_Driver
  * @{
  */

/** @addtogroup AES 
  * @brief AES driver modules
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
#define AES_CC_TIMEOUT    ((uint32_t) 0x00010000)

/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/** @defgroup AES_Private_Functions
  * @{
  */ 

/** @defgroup AES_Group6 High Level AES functions
 *  @brief   High Level AES functions 
 *
@verbatim
================================================================================
                         ##### High Level AES functions #####
================================================================================

@endverbatim
  * @{
  */

/**
  * @brief  Encrypt using AES in ECB Mode
  * @param  Key: Key used for AES algorithm.
  * @param  Input: pointer to the Input buffer.
  * @param  Ilength: length of the Input buffer, must be a multiple of 16 bytes.
  * @param  Output: pointer to the returned buffer.
  * @retval An ErrorStatus enumeration value:
  *          - SUCCESS: Operation done
  *          - ERROR: Operation failed
  */
ErrorStatus AES_ECB_Encrypt(uint8_t* Key, uint8_t* Input, uint32_t Ilength, uint8_t* Output)
{
  AES_InitTypeDef AES_InitStructure;
  AES_KeyInitTypeDef  AES_KeyInitStructure;
  ErrorStatus status = SUCCESS;
  uint32_t keyaddr    = (uint32_t)Key;
  uint32_t inputaddr  = (uint32_t)Input;
  uint32_t outputaddr = (uint32_t)Output;
  __IO uint32_t counter = 0;
  uint32_t ccstatus = 0;
  uint32_t i = 0;

  /* AES Key initialisation */
  AES_KeyInitStructure.AES_Key3 = __REV(*(uint32_t*)(keyaddr));
  keyaddr += 4;
  AES_KeyInitStructure.AES_Key2 = __REV(*(uint32_t*)(keyaddr));
  keyaddr += 4;
  AES_KeyInitStructure.AES_Key1 = __REV(*(uint32_t*)(keyaddr));
  keyaddr += 4;
  AES_KeyInitStructure.AES_Key0 = __REV(*(uint32_t*)(keyaddr));
  AES_KeyInit(&AES_KeyInitStructure);

  /* AES configuration */
  AES_InitStructure.AES_Operation = AES_Operation_Encryp;
  AES_InitStructure.AES_Chaining = AES_Chaining_ECB;
  AES_InitStructure.AES_DataType = AES_DataType_8b;
  AES_Init(&AES_InitStructure);

  /* Enable AES */
  AES_Cmd(ENABLE);

  for(i = 0; ((i < Ilength) && (status != ERROR)); i += 16)
  {
    AES_WriteSubData(*(uint32_t*)(inputaddr));
    inputaddr += 4;
    AES_WriteSubData(*(uint32_t*)(inputaddr));
    inputaddr += 4;
    AES_WriteSubData(*(uint32_t*)(inputaddr));
    inputaddr += 4;
    AES_WriteSubData(*(uint32_t*)(inputaddr));
    inputaddr += 4;
    
    /* Wait for CCF flag to be set */
    counter = 0;
    do
    {
      ccstatus = AES_GetFlagStatus(AES_FLAG_CCF);
      counter++;
    }while((counter != AES_CC_TIMEOUT) && (ccstatus == RESET));
    
    if (ccstatus == RESET)
    {
      status = ERROR;
    }
    else
    {
      /* Clear CCF flag */
      AES_ClearFlag(AES_FLAG_CCF);
      /* Read cipher text */
      *(uint32_t*)(outputaddr) = AES_ReadSubData();
      outputaddr += 4;
      *(uint32_t*)(outputaddr) = AES_ReadSubData();
      outputaddr += 4;
      *(uint32_t*)(outputaddr) = AES_ReadSubData();
      outputaddr += 4;
      *(uint32_t*)(outputaddr) = AES_ReadSubData();
      outputaddr += 4;
    }
  }
  
  /* Disable AES before starting new processing */
  AES_Cmd(DISABLE);

  return status;
}

/**
  * @brief  Decrypt using AES in ECB Mode
  * @param  Key: Key used for AES algorithm.
  * @param  Input: pointer to the Input buffer.
  * @param  Ilength: length of the Input buffer, must be a multiple of 16 bytes.
  * @param  Output: pointer to the returned buffer.
  * @retval An ErrorStatus enumeration value:
  *          - SUCCESS: Operation done
  *          - ERROR: Operation failed
  */
ErrorStatus AES_ECB_Decrypt(uint8_t* Key, uint8_t* Input, uint32_t Ilength, uint8_t* Output)
{
  AES_InitTypeDef AES_InitStructure;
  AES_KeyInitTypeDef  AES_KeyInitStructure;
  ErrorStatus status = SUCCESS;
  uint32_t keyaddr    = (uint32_t)Key;
  uint32_t inputaddr  = (uint32_t)Input;
  uint32_t outputaddr = (uint32_t)Output;
  __IO uint32_t counter = 0;
  uint32_t ccstatus = 0;
  uint32_t i = 0;

  /* AES Key initialisation */
  AES_KeyInitStructure.AES_Key3 = __REV(*(uint32_t*)(keyaddr));
  keyaddr += 4;
  AES_KeyInitStructure.AES_Key2 = __REV(*(uint32_t*)(keyaddr));
  keyaddr += 4;
  AES_KeyInitStructure.AES_Key1 = __REV(*(uint32_t*)(keyaddr));
  keyaddr += 4;
  AES_KeyInitStructure.AES_Key0 = __REV(*(uint32_t*)(keyaddr));
  AES_KeyInit(&AES_KeyInitStructure);

  /* AES configuration */
  AES_InitStructure.AES_Operation = AES_Operation_KeyDerivAndDecryp;
  AES_InitStructure.AES_Chaining = AES_Chaining_ECB;
  AES_InitStructure.AES_DataType = AES_DataType_8b;
  AES_Init(&AES_InitStructure);

  /* Enable AES */
  AES_Cmd(ENABLE);

  for(i = 0; ((i < Ilength) && (status != ERROR)); i += 16)
  {
    AES_WriteSubData(*(uint32_t*)(inputaddr));
    inputaddr += 4;
    AES_WriteSubData(*(uint32_t*)(inputaddr));
    inputaddr += 4;
    AES_WriteSubData(*(uint32_t*)(inputaddr));
    inputaddr += 4;
    AES_WriteSubData(*(uint32_t*)(inputaddr));
    inputaddr += 4;
    
     /* Wait for CCF flag to be set */
    counter = 0;
    do
    {
      ccstatus = AES_GetFlagStatus(AES_FLAG_CCF);
      counter++;
    }while((counter != AES_CC_TIMEOUT) && (ccstatus == RESET));
    
    if (ccstatus == RESET)
    {
      status = ERROR;
    }
    else
    {
      /* Clear CCF flag */
      AES_ClearFlag(AES_FLAG_CCF);

      /* Read cipher text */
      *(uint32_t*)(outputaddr) = AES_ReadSubData();
      outputaddr += 4;
      *(uint32_t*)(outputaddr) = AES_ReadSubData();
      outputaddr += 4;
      *(uint32_t*)(outputaddr) = AES_ReadSubData();
      outputaddr += 4;
      *(uint32_t*)(outputaddr) = AES_ReadSubData();
      outputaddr += 4;
    }
  }

  /* Disable AES before starting new processing */
  AES_Cmd(DISABLE);

  return status;
}

/**
  * @brief  Encrypt using AES in CBC Mode
  * @param  InitVectors: Initialisation Vectors used for AES algorithm.
  * @param  Key: Key used for AES algorithm.
  * @param  Input: pointer to the Input buffer.
  * @param  Ilength: length of the Input buffer, must be a multiple of 16 bytes.
  * @param  Output: pointer to the returned buffer.
  * @retval An ErrorStatus enumeration value:
  *          - SUCCESS: Operation done
  *          - ERROR: Operation failed
  */
ErrorStatus AES_CBC_Encrypt(uint8_t* Key, uint8_t InitVectors[16], uint8_t* Input, uint32_t Ilength, uint8_t* Output)
{
  AES_InitTypeDef AES_InitStructure;
  AES_KeyInitTypeDef  AES_KeyInitStructure;
  AES_IVInitTypeDef AES_IVInitStructure;
  ErrorStatus status = SUCCESS;
  uint32_t keyaddr    = (uint32_t)Key;
  uint32_t inputaddr  = (uint32_t)Input;
  uint32_t outputaddr = (uint32_t)Output;
  uint32_t ivaddr     = (uint32_t)InitVectors;
  __IO uint32_t counter = 0;
  uint32_t ccstatus = 0;
  uint32_t i = 0;

  /* AES Key initialisation*/
  AES_KeyInitStructure.AES_Key3 = __REV(*(uint32_t*)(keyaddr));
  keyaddr += 4;
  AES_KeyInitStructure.AES_Key2 = __REV(*(uint32_t*)(keyaddr));
  keyaddr += 4;
  AES_KeyInitStructure.AES_Key1 = __REV(*(uint32_t*)(keyaddr));
  keyaddr += 4;
  AES_KeyInitStructure.AES_Key0 = __REV(*(uint32_t*)(keyaddr));
  AES_KeyInit(&AES_KeyInitStructure);

  /* AES Initialization Vectors */
  AES_IVInitStructure.AES_IV3 = __REV(*(uint32_t*)(ivaddr));
  ivaddr += 4;
  AES_IVInitStructure.AES_IV2 = __REV(*(uint32_t*)(ivaddr));
  ivaddr += 4;
  AES_IVInitStructure.AES_IV1 = __REV(*(uint32_t*)(ivaddr));
  ivaddr += 4;
  AES_IVInitStructure.AES_IV0 = __REV(*(uint32_t*)(ivaddr));
  AES_IVInit(&AES_IVInitStructure);

  /* AES configuration */
  AES_InitStructure.AES_Operation = AES_Operation_Encryp;
  AES_InitStructure.AES_Chaining = AES_Chaining_CBC;
  AES_InitStructure.AES_DataType = AES_DataType_8b;
  AES_Init(&AES_InitStructure);

  /* Enable AES */
  AES_Cmd(ENABLE);

  for(i = 0; ((i < Ilength) && (status != ERROR)); i += 16)
  {
    AES_WriteSubData(*(uint32_t*)(inputaddr));
    inputaddr += 4;
    AES_WriteSubData(*(uint32_t*)(inputaddr));
    inputaddr += 4;
    AES_WriteSubData(*(uint32_t*)(inputaddr));
    inputaddr += 4;
    AES_WriteSubData(*(uint32_t*)(inputaddr));
    inputaddr += 4;
    
    /* Wait for CCF flag to be set */
    counter = 0;
    do
    {
      ccstatus = AES_GetFlagStatus(AES_FLAG_CCF);
      counter++;
    }while((counter != AES_CC_TIMEOUT) && (ccstatus == RESET));
    
    if (ccstatus == RESET)
    {
      status = ERROR;
    }
    else
    {
      /* Clear CCF flag */
      AES_ClearFlag(AES_FLAG_CCF);

      /* Read cipher text */
      *(uint32_t*)(outputaddr) = AES_ReadSubData();
      outputaddr += 4;
      *(uint32_t*)(outputaddr) = AES_ReadSubData();
      outputaddr += 4;
      *(uint32_t*)(outputaddr) = AES_ReadSubData();
      outputaddr += 4;
      *(uint32_t*)(outputaddr) = AES_ReadSubData();
      outputaddr += 4;
    }
  }

  /* Disable AES before starting new processing */
  AES_Cmd(DISABLE);

  return status;
}

/**
  * @brief  Decrypt using AES in CBC Mode
  * @param  InitVectors: Initialisation Vectors used for AES algorithm.
  * @param  Key: Key used for AES algorithm.
  * @param  Input: pointer to the Input buffer.
  * @param  Ilength: length of the Input buffer, must be a multiple of 16 bytes.
  * @param  Output: pointer to the returned buffer.
  * @retval An ErrorStatus enumeration value:
  *          - SUCCESS: Operation done
  *          - ERROR: Operation failed
  */
ErrorStatus AES_CBC_Decrypt(uint8_t* Key, uint8_t InitVectors[16], uint8_t* Input, uint32_t Ilength, uint8_t* Output)
{
  AES_InitTypeDef AES_InitStructure;
  AES_KeyInitTypeDef  AES_KeyInitStructure;
  AES_IVInitTypeDef AES_IVInitStructure;
  ErrorStatus status = SUCCESS;
  uint32_t keyaddr    = (uint32_t)Key;
  uint32_t inputaddr  = (uint32_t)Input;
  uint32_t outputaddr = (uint32_t)Output;
  uint32_t ivaddr     = (uint32_t)InitVectors;
  __IO uint32_t counter = 0;
  uint32_t ccstatus = 0;
  uint32_t i = 0;
  
  /* AES Key initialisation*/
  AES_KeyInitStructure.AES_Key3 = __REV(*(uint32_t*)(keyaddr));
  keyaddr += 4;
  AES_KeyInitStructure.AES_Key2 = __REV(*(uint32_t*)(keyaddr));
  keyaddr += 4;
  AES_KeyInitStructure.AES_Key1 = __REV(*(uint32_t*)(keyaddr));
  keyaddr += 4;
  AES_KeyInitStructure.AES_Key0 = __REV(*(uint32_t*)(keyaddr));
  AES_KeyInit(&AES_KeyInitStructure);

  /* AES Initialization Vectors */
  AES_IVInitStructure.AES_IV3 = __REV(*(uint32_t*)(ivaddr));
  ivaddr += 4;
  AES_IVInitStructure.AES_IV2 = __REV(*(uint32_t*)(ivaddr));
  ivaddr += 4;
  AES_IVInitStructure.AES_IV1 = __REV(*(uint32_t*)(ivaddr));
  ivaddr += 4;
  AES_IVInitStructure.AES_IV0 = __REV(*(uint32_t*)(ivaddr));
  AES_IVInit(&AES_IVInitStructure);

  /* AES configuration */
  AES_InitStructure.AES_Operation = AES_Operation_KeyDerivAndDecryp;
  AES_InitStructure.AES_Chaining = AES_Chaining_CBC;
  AES_InitStructure.AES_DataType = AES_DataType_8b;
  AES_Init(&AES_InitStructure);

  /* Enable AES */
  AES_Cmd(ENABLE);

  for(i = 0; ((i < Ilength) && (status != ERROR)); i += 16)
  {
    AES_WriteSubData(*(uint32_t*)(inputaddr));
    inputaddr += 4;
    AES_WriteSubData(*(uint32_t*)(inputaddr));
    inputaddr += 4;
    AES_WriteSubData(*(uint32_t*)(inputaddr));
    inputaddr += 4;
    AES_WriteSubData(*(uint32_t*)(inputaddr));
    inputaddr += 4;
    
    /* Wait for CCF flag to be set */
    counter = 0;
    do
    {
      ccstatus = AES_GetFlagStatus(AES_FLAG_CCF);
      counter++;
    }while((counter != AES_CC_TIMEOUT) && (ccstatus == RESET));

    if (ccstatus == RESET)
    {
      status = ERROR;
    }
    else
    {
      /* Clear CCF flag */
      AES_ClearFlag(AES_FLAG_CCF);

      /* Read cipher text */
      *(uint32_t*)(outputaddr) = AES_ReadSubData();
      outputaddr += 4;
      *(uint32_t*)(outputaddr) = AES_ReadSubData();
      outputaddr += 4;
      *(uint32_t*)(outputaddr) = AES_ReadSubData();
      outputaddr += 4;
      *(uint32_t*)(outputaddr) = AES_ReadSubData();
      outputaddr += 4;
    }
  }

  /* Disable AES before starting new processing */
  AES_Cmd(DISABLE);

  return status;
}

/**
  * @brief  Encrypt using AES in CTR Mode
  * @param  InitVectors: Initialisation Vectors used for AES algorithm.
  * @param  Key: Key used for AES algorithm.
  * @param  Input: pointer to the Input buffer.
  * @param  Ilength: length of the Input buffer, must be a multiple of 16 bytes.
  * @param  Output: pointer to the returned buffer.
  * @retval An ErrorStatus enumeration value:
  *          - SUCCESS: Operation done
  *          - ERROR: Operation failed
  */
ErrorStatus AES_CTR_Encrypt(uint8_t* Key, uint8_t InitVectors[16], uint8_t* Input, uint32_t Ilength, uint8_t* Output)
{
  AES_InitTypeDef AES_InitStructure;
  AES_KeyInitTypeDef  AES_KeyInitStructure;
  AES_IVInitTypeDef AES_IVInitStructure;

  ErrorStatus status = SUCCESS;
  uint32_t keyaddr    = (uint32_t)Key;
  uint32_t inputaddr  = (uint32_t)Input;
  uint32_t outputaddr = (uint32_t)Output;
  uint32_t ivaddr     = (uint32_t)InitVectors;
  __IO uint32_t counter = 0;
  uint32_t ccstatus = 0;
  uint32_t i = 0;

  /* AES key initialisation*/
  AES_KeyInitStructure.AES_Key3 = __REV(*(uint32_t*)(keyaddr));
  keyaddr += 4;
  AES_KeyInitStructure.AES_Key2 = __REV(*(uint32_t*)(keyaddr));
  keyaddr += 4;
  AES_KeyInitStructure.AES_Key1 = __REV(*(uint32_t*)(keyaddr));
  keyaddr += 4;
  AES_KeyInitStructure.AES_Key0 = __REV(*(uint32_t*)(keyaddr));
  AES_KeyInit(&AES_KeyInitStructure);

  /* AES Initialization Vectors */
  AES_IVInitStructure.AES_IV3 = __REV(*(uint32_t*)(ivaddr));
  ivaddr += 4;
  AES_IVInitStructure.AES_IV2= __REV(*(uint32_t*)(ivaddr));
  ivaddr += 4;
  AES_IVInitStructure.AES_IV1 = __REV(*(uint32_t*)(ivaddr));
  ivaddr += 4;
  AES_IVInitStructure.AES_IV0= __REV(*(uint32_t*)(ivaddr));
  AES_IVInit(&AES_IVInitStructure);

  /* AES configuration */
  AES_InitStructure.AES_Operation = AES_Operation_Encryp;
  AES_InitStructure.AES_Chaining = AES_Chaining_CTR;
  AES_InitStructure.AES_DataType = AES_DataType_8b;
  AES_Init(&AES_InitStructure);

  /* Enable AES */
  AES_Cmd(ENABLE);

  for(i = 0; ((i < Ilength) && (status != ERROR)); i += 16)
  {
    AES_WriteSubData(*(uint32_t*)(inputaddr));
    inputaddr += 4;
    AES_WriteSubData(*(uint32_t*)(inputaddr));
    inputaddr += 4;
    AES_WriteSubData(*(uint32_t*)(inputaddr));
    inputaddr += 4;
    AES_WriteSubData(*(uint32_t*)(inputaddr));
    inputaddr += 4;
    
    /* Wait for CCF flag to be set */
    counter = 0;
    do
    {
      ccstatus = AES_GetFlagStatus(AES_FLAG_CCF);
      counter++;
    }while((counter != AES_CC_TIMEOUT) && (ccstatus == RESET));

    if (ccstatus == RESET)
    {
      status = ERROR;
    }
    else
    {
      /* Clear CCF flag */
      AES_ClearFlag(AES_FLAG_CCF);

      /* Read cipher text */
      *(uint32_t*)(outputaddr) = AES_ReadSubData();
      outputaddr += 4;
      *(uint32_t*)(outputaddr) = AES_ReadSubData();
      outputaddr += 4;
      *(uint32_t*)(outputaddr) = AES_ReadSubData();
      outputaddr += 4;
      *(uint32_t*)(outputaddr) = AES_ReadSubData();
      outputaddr += 4;
    }
  }

  /* Disable AES before starting new processing */
  AES_Cmd(DISABLE);

  return status;
}

/**
  * @brief  Decrypt using AES in CTR Mode
  * @param  InitVectors: Initialisation Vectors used for AES algorithm.
  * @param  Key: Key used for AES algorithm.
  * @param  Input: pointer to the Input buffer.
  * @param  Ilength: length of the Input buffer, must be a multiple of 16 bytes.
  * @param  Output: pointer to the returned buffer.
  * @retval An ErrorStatus enumeration value:
  *          - SUCCESS: Operation done
  *          - ERROR: Operation failed
  */
ErrorStatus AES_CTR_Decrypt(uint8_t* Key, uint8_t InitVectors[16], uint8_t* Input, uint32_t Ilength, uint8_t* Output)
{
  AES_InitTypeDef AES_InitStructure;
  AES_KeyInitTypeDef  AES_KeyInitStructure;
  AES_IVInitTypeDef AES_IVInitStructure;

  ErrorStatus status = SUCCESS;
  uint32_t keyaddr    = (uint32_t)Key;
  uint32_t inputaddr  = (uint32_t)Input;
  uint32_t outputaddr = (uint32_t)Output;
  uint32_t ivaddr     = (uint32_t)InitVectors;
  __IO uint32_t counter = 0;
  uint32_t ccstatus = 0;
  uint32_t i = 0;

  /* AES Key initialisation*/
  AES_KeyInitStructure.AES_Key3 = __REV(*(uint32_t*)(keyaddr));
  keyaddr += 4;
  AES_KeyInitStructure.AES_Key2 = __REV(*(uint32_t*)(keyaddr));
  keyaddr += 4;
  AES_KeyInitStructure.AES_Key1 = __REV(*(uint32_t*)(keyaddr));
  keyaddr += 4;
  AES_KeyInitStructure.AES_Key0 = __REV(*(uint32_t*)(keyaddr));
  AES_KeyInit(&AES_KeyInitStructure);

  /* AES Initialization Vectors */
  AES_IVInitStructure.AES_IV3 = __REV(*(uint32_t*)(ivaddr));
  ivaddr += 4;
  AES_IVInitStructure.AES_IV2 = __REV(*(uint32_t*)(ivaddr));
  ivaddr += 4;
  AES_IVInitStructure.AES_IV1 = __REV(*(uint32_t*)(ivaddr));
  ivaddr += 4;
  AES_IVInitStructure.AES_IV0 = __REV(*(uint32_t*)(ivaddr));
  AES_IVInit(&AES_IVInitStructure);

  /* AES configuration */
  AES_InitStructure.AES_Operation = AES_Operation_KeyDerivAndDecryp;
  AES_InitStructure.AES_Chaining = AES_Chaining_CTR;
  AES_InitStructure.AES_DataType = AES_DataType_8b;
  AES_Init(&AES_InitStructure);

  /* Enable AES */
  AES_Cmd(ENABLE);

  for(i = 0; ((i < Ilength) && (status != ERROR)); i += 16)
  {
    AES_WriteSubData(*(uint32_t*)(inputaddr));
    inputaddr += 4;
    AES_WriteSubData(*(uint32_t*)(inputaddr));
    inputaddr += 4;
    AES_WriteSubData(*(uint32_t*)(inputaddr));
    inputaddr += 4;
    AES_WriteSubData(*(uint32_t*)(inputaddr));
    inputaddr += 4;
    
    /* Wait for CCF flag to be set */
    counter = 0;
    do
    {
      ccstatus = AES_GetFlagStatus(AES_FLAG_CCF);
      counter++;
    }while((counter != AES_CC_TIMEOUT) && (ccstatus == RESET));

    if (ccstatus == RESET)
    {
      status = ERROR;
    }
    else
    {
      /* Clear CCF flag */
      AES_ClearFlag(AES_FLAG_CCF);
    
      /* Read cipher text */
      *(uint32_t*)(outputaddr) = AES_ReadSubData();
      outputaddr += 4;
      *(uint32_t*)(outputaddr) = AES_ReadSubData();
      outputaddr += 4;
      *(uint32_t*)(outputaddr) = AES_ReadSubData();
      outputaddr += 4;
      *(uint32_t*)(outputaddr) = AES_ReadSubData();
      outputaddr += 4;
    }
  }

  /* Disable AES before starting new processing */
  AES_Cmd(DISABLE);

  return status;
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

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/

