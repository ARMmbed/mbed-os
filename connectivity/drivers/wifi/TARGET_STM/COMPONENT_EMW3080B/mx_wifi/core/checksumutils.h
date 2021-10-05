/**
  ******************************************************************************
  * @file    checksumutils.h
  * @author  MCD Application Team
  * @brief   Header for checksumutils.c module
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

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef CHECKSUM_UTILS_H
#define CHECKSUM_UTILS_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
/*cstat -MISRAC2012-* */
#include <stddef.h>
#include <stdint.h>
/*cstat +MISRAC2012-* */


/* NOTE: MUST ENABLE STM32L HARDWARE CRC periph before use this API
#define USE_STM32L_CRC
  */

#ifdef USE_STM32L_CRC
/*cstat -MISRAC2012-* */
#include "stm32l4xx_hal.h"
/*cstat +MISRAC2012-* */
#endif /* USE_STM32L_CRC */


/*********************  CRC8 MXOS Check **************************************
  ******************CRC-8 XMODEM      x8+x5+x4+1******************************
  ****************************************************************************/

typedef struct
{
  uint8_t crc;
} CRC8_Context;

/**
  * @brief             initialize the CRC8 Context
  *
  * @param inContext   holds CRC8 result
  *
  * @retval            none
  */
void CRC8_Init(CRC8_Context *inContext);


/**
  * @brief             Calculate the CRC8 result
  *
  * @param inContext   holds CRC8 result during calculation process
  * @param inSrc       input data
  * @param inLen       length of input data
  *
  * @retval            none
  */
void CRC8_Update(CRC8_Context *inContext, const uint8_t *inSrc, size_t inLen);


/**
  * @brief             output CRC8 result
  *
  * @param inContext   holds CRC8 result
  * @param outResutl   holds CRC8 final result
  *
  * @retval            none
  */
void CRC8_Final(CRC8_Context *inContext, uint8_t *outResult);


/*********************  CRC16 MXOS Check  **************************************
  ******************CRC-16/XMODEM     x16+x12+x5+1******************************
  ******************************************************************************/

#ifdef USE_STM32L_CRC

int8_t HW_CRC16_Init(CRC_HandleTypeDef *CrcHandle);
int8_t HW_CRC16_Update(CRC_HandleTypeDef *CrcHandle, uint8_t *input_data, uint32_t input_len, uint16_t *crc16_out);

#else

typedef struct
{
  uint16_t crc;
} CRC16_Context;

/**
  * @brief             initialize the CRC16 Context
  *
  * @param inContext   holds CRC16 result
  *
  * @retval            none
  */
void CRC16_Init(CRC16_Context *inContext);


/**
  * @brief             Calculate the CRC16 result
  *
  * @param inContext   holds CRC16 result during calculation process
  * @param inSrc       input data
  * @param inLen       length of input data
  *
  * @retval            none
  */
void CRC16_Update(CRC16_Context *inContext, const uint8_t *inSrc, size_t inLen);


/**
  * @brief             output CRC16 result
  *
  * @param inContext   holds CRC16 result
  * @param outResutl   holds CRC16 final result
  *
  * @retval            none
  */
void CRC16_Final(CRC16_Context *inContext, uint16_t *outResult);

#endif /* USE_STM32L_CRC */


#ifdef __cplusplus
}
#endif

#endif /* CHECKSUM_UTILS_H */
