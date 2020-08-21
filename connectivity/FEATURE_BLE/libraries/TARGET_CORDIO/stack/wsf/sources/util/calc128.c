/*************************************************************************************************/
/*!
 *  \file   calc128.c
 *
 *  \brief  128-bit integer utilities.
 *
 *  Copyright (c) 2010-2018 Arm Ltd. All Rights Reserved.
 *
 *  Copyright (c) 2019-2020 Packetcraft, Inc.
 *  
 *  Licensed under the Apache License, Version 2.0 (the "License");
 *  you may not use this file except in compliance with the License.
 *  You may obtain a copy of the License at
 *  
 *      http://www.apache.org/licenses/LICENSE-2.0
 *  
 *  Unless required by applicable law or agreed to in writing, software
 *  distributed under the License is distributed on an "AS IS" BASIS,
 *  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *  See the License for the specific language governing permissions and
 *  limitations under the License.
 */
/*************************************************************************************************/

#include <string.h>
#include "wsf_types.h"
#include "util/calc128.h"

/**************************************************************************************************
  Global Variables
**************************************************************************************************/

/* 128-bit zero value */
const uint8_t calc128Zeros[CALC128_LEN] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};

/**************************************************************************************************
  Function Declarations
**************************************************************************************************/

/*************************************************************************************************/
/*!
 *  \brief  Copy a 128-bit integer from source to destination.
 *
 *  \param  pDst    Pointer to destination.
 *  \param  pSrc    Pointer to source.
 */
/*************************************************************************************************/
void Calc128Cpy(uint8_t *pDst, uint8_t *pSrc)
{
  memcpy(pDst, pSrc, CALC128_LEN);
}

/*************************************************************************************************/
/*!
 *  \brief  Copy a 64-bit integer from source to destination.
 *
 *  \param  pDst    Pointer to destination.
 *  \param  pSrc    Pointer to source.
 */
/*************************************************************************************************/
void Calc128Cpy64(uint8_t *pDst, uint8_t *pSrc)
{
  memcpy(pDst, pSrc, CALC128_LEN/2);
}

/*************************************************************************************************/
/*!
 *  \brief  Exclusive-or two 128-bit integers and return the result in pDst.
 *
 *  \param  pDst    Pointer to destination.
 *  \param  pSrc    Pointer to source.
 */
/*************************************************************************************************/
void Calc128Xor(uint8_t *pDst, uint8_t *pSrc)
{
  uint8_t i;

  for (i = CALC128_LEN; i > 0; i--)
  {
    *pDst++ ^= *pSrc++;
  }
}
