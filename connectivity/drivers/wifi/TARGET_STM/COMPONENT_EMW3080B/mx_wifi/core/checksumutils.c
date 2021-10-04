/**
  ******************************************************************************
  * @file    checksum_utils.c
  * @author  MCD Application Team
  * @brief   Host driver checksum utils of MXCHIP Wi-Fi component.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2021 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */


/* Includes ------------------------------------------------------------------*/
/*cstat -MISRAC2012-* */
#include "stdio.h"
/*cstat +MISRAC2012-* */

#include "checksumutils.h"

#ifdef CHECKSUM_DEBUG
#define DEBUG_LOG   printf
#else
#define DEBUG_LOG
#endif /* CHECKSUM_DEBUG */

#ifdef USE_STM32L_CRC

/* User-defined polynomial */
#define CRC_POLYNOMIAL_16B  0x1021  /* X^16 + X^12 + X^5 + 1, CRC-CCITT generating polynomial */

/* User-defined CRC init value */
/* As the CRC is 16-bit long, the init value is 16-bit long as well */
#define CRC_INIT_VALUE      0x0000 /* 0x5AB */

#endif /* USE_STM32L_CRC */

static uint8_t UpdateCRC8(uint8_t crcIn, uint8_t byte);

static uint8_t UpdateCRC8(uint8_t crcIn, uint8_t byte)
{
  uint8_t crc = crcIn;
  uint8_t i;

  crc ^= byte;

  for (i = 0; i < (uint8_t)8; i++)
  {
    if ((crc & (uint8_t)0x01) > 0u)
    {
      crc = (crc >> 1) ^ (uint8_t)0x8C;
    }
    else
    {
      crc >>= 1;
    }
  }
  return crc;
}


void CRC8_Init(CRC8_Context *inContext)
{
  inContext->crc = 0;
}


void CRC8_Update(CRC8_Context *inContext, const uint8_t *inSrc, size_t inLen)
{
  const uint8_t *src = (const uint8_t *) inSrc;
  const uint8_t *srcEnd = &(src[inLen]);
  while (src < srcEnd)
  {
    inContext->crc = UpdateCRC8(inContext->crc, *src);
    src++;
  }
}


void CRC8_Final(CRC8_Context *inContext, uint8_t *outResult)
{
  *outResult = inContext->crc & 0xffu;
}


/*************************************CRC16******************************************/

#ifdef USE_STM32L_CRC
/**
  * @brief CRC16 init function
  */
int8_t HW_CRC16_Init(CRC_HandleTypeDef *CrcHandle)
{
  /****************************************************************************/
  /*                                                                          */
  /*                     CRC peripheral initialization                        */
  /*                                                                          */
  /****************************************************************************/

  if (NULL == CrcHandle)
  {
    DEBUG_LOG("*** CRC16_Init: CrcHandle null error!\n");
    return -1;
  }

  CrcHandle->Instance = CRC;

  /* The default polynomial is not used. The one to be used must be defined
       in CrcHandle.Init.GeneratingPolynomial */
  CrcHandle->Init.DefaultPolynomialUse    = DEFAULT_POLYNOMIAL_DISABLE;

  /* Set the value of the generating polynomial.
      The one used in that example is the 16-bit long CRC generating
      polynomial X^16 + X^12 + X^5 + 1 */
  CrcHandle->Init.GeneratingPolynomial    = CRC_POLYNOMIAL_16B;

  /* The user-defined generating polynomial yields a 16-bit long CRC */
  CrcHandle->Init.CRCLength               = CRC_POLYLENGTH_16B;

  /* The default init value is not used */
  CrcHandle->Init.DefaultInitValueUse     = DEFAULT_INIT_VALUE_DISABLE;

  /* User init value is used instead */
  CrcHandle->Init.InitValue               = CRC_INIT_VALUE;

  /* The input data are inverted by word */
  CrcHandle->Init.InputDataInversionMode  = CRC_INPUTDATA_INVERSION_NONE;
  /* The bit reversal is done on a full word basis.

       The input stream yielded by CRC16_DATA8[] is the sequence of
       bytes 0x4D, 0x3C, 0x2B, 0x1A, 0xBE, 0x71, ...
       meaning that the first word written in the CRC DR register is
        0x4D3C2B1A.

       Bit reversal done by the hardware on the full word leads to the actual
       CRC processing of
        0x58D43CB2.

       Similarly, the second word written in the peripheral is 0xBE71C98A, leading to
       the processing of 0x51938E7D after hardware input data reversal.

       Note that when the software writes less than a word in the peripheral (to minimize the
       number of write accesses for a given number of bytes), the bit-reversal operation
       is carried out only on the inputted data.
       Therefore, the last written data is the last byte 0x5E, processed as 0x7A
       by the hardware after bit-reversal to wrap-up the CRC computation.

       This means that the field InputDataInversionMode set to CRC_INPUTDATA_INVERSION_WORD
       applied to {0x4D, 0x3C, 0x2B, 0x1A, 0xBE, 0x71, 0xC9, 0x8A, 0x5E}
       yields the same result as InputDataInversionMode set to CRC_INPUTDATA_INVERSION_NONE
       applied to {0x58, 0xD4, 0x3C, 0xB2, 0x51, 0x93, 0x8E, 0x7D, 0x7A}.

   */

  /* The output data are inverted */
  CrcHandle->Init.OutputDataInversionMode = CRC_OUTPUTDATA_INVERSION_DISABLE;
  /* Output data reversal can only be done at bit level.
       The expected CRC is 0x5043 after output data reversal, meaning
       that the CRC before the reversal operation is 0xC20A. */

  /* The input data are bytes (8-bit long data) */
  CrcHandle->InputDataFormat              = CRC_INPUTDATA_FORMAT_BYTES;

  /* De-initialize the CRC peripheral */
  if (HAL_CRC_DeInit(CrcHandle) != HAL_OK)
  {
    /* Initialization Error */
    DEBUG_LOG("*** CRC16_Init: HAL_CRC_DeInit error!\n");
    return -1;
  }

  /* Then, initialize the CRC handle */
  if (HAL_CRC_Init(CrcHandle) != HAL_OK)
  {
    /* Initialization Error */
    DEBUG_LOG("*** CRC16_Init: HAL_CRC_Init error!\n");
    return -1;
  }

  return 0; /* init success */
}

int8_t HW_CRC16_Update(CRC_HandleTypeDef *CrcHandle, uint8_t *input_data, uint32_t input_len, uint16_t *crc16_out)
{
  /****************************************************************************/
  /*                                                                          */
  /*                      CRC computation                                     */
  /*                                                                          */
  /****************************************************************************/

  /* Used for storing CRC Value */
  __IO uint32_t uwCRCValue = 0;

  if (NULL == CrcHandle)
  {
    DEBUG_LOG("*** CRC16_Update: CrcHandle null error!\n");
    return -1;
  }

  /* The 16-bit long CRC of the 9-byte buffer is computed. After peripheral initialization,
       the CRC calculator is initialized with the user-defined value that is 0x5ABE.

      The computed CRC is stored in uint32_t uwCRCValue. The 16-bit long CRC is made of
      uwCRCValue 16 LSB bits. */

  uwCRCValue = HAL_CRC_Accumulate(CrcHandle, (uint32_t *)input_data, input_len);
  *crc16_out = (uint16_t)(uwCRCValue & 0x0000FFFF);

  return 0;
}

#else  /* SOFTWARE CRC16 */

static uint16_t UpdateCRC16(uint16_t crcIn, uint8_t byte)
{
  uint32_t crc = crcIn;
  uint32_t in = (uint32_t)byte | (uint32_t)0x100;

  do
  {
    crc <<= 1;
    in <<= 1;
    if ((in & (uint32_t)0x100) > 0u)
    {
      ++crc;
    }
    if ((crc & (uint32_t)0x10000) > 0u)
    {
      crc ^= (uint32_t)0x1021;
    }
  } while (!((in & (uint32_t)0x10000) > 0u));
  return (uint16_t)(crc & (uint32_t)0xffffu);
}

void CRC16_Init(CRC16_Context *inContext)
{
  inContext->crc = 0;
}


void CRC16_Update(CRC16_Context *inContext, const uint8_t *inSrc, size_t inLen)
{
  const uint8_t *src = (const uint8_t *) inSrc;
  const uint8_t *srcEnd = &(src[inLen]);
  while (src < srcEnd)
  {
    inContext->crc = UpdateCRC16(inContext->crc, *src);
    src++;
  }
}


void CRC16_Final(CRC16_Context *inContext, uint16_t *outResult)
{
  inContext->crc = UpdateCRC16(inContext->crc, 0);
  inContext->crc = UpdateCRC16(inContext->crc, 0);
  *outResult = inContext->crc & 0xffffu;
}

#endif /* USE_STM32L_CRC */
