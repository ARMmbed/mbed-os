/*************************************************************************************************/
/*!
 *  \file   crc32.h
 *
 *  \brief  CRC-32 utilities.
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
#ifndef CRC32_H
#define CRC32_H

#ifdef __cplusplus
extern "C" {
#endif

/*! \addtogroup WSF_UTIL_API
 *  \{ */

/*************************************************************************************************/
/*!
 *  \brief  Calculate the CRC-32 of the given buffer.
 *
 *  \param  crcInit  Initial value of the CRC.
 *  \param  len      Length of the buffer.
 *  \param  pBuf     Buffer to compute the CRC.
 *
 *  This routine was originally generated with crcmod.py using the following parameters:
 *    - polynomial 0x104C11DB7
 *    - bit reverse algorithm
 */
/*************************************************************************************************/
uint32_t CalcCrc32(uint32_t crcInit, uint32_t len, const uint8_t *pBuf);

/*! \} */    /* WSF_UTIL_API */

#ifdef __cplusplus
};
#endif

#endif /* CRC32_H */
