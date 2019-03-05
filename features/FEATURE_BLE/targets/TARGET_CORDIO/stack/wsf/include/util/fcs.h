/* Copyright (c) 2009-2019 Arm Limited
 * SPDX-License-Identifier: Apache-2.0
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

/*************************************************************************************************/
/*!
 *  \brief FCS utilities (3GPP TS 27.010).
 */
/*************************************************************************************************/
#ifndef FCS_H
#define FCS_H

#ifdef __cplusplus
extern "C" {
#endif

/*************************************************************************************************/
/*!
 *  \brief  Calculate the FCS of the given buffer.
 *
 *  \param  len      Length of the buffer.
 *  \param  pBuf     Buffer to compute the CRC.
 *
 *  \return FCS value.
 */
/*************************************************************************************************/
uint8_t FcsCalc(uint32_t len, const uint8_t *pBuf);

/*************************************************************************************************/
/*!
 *  \brief  Computes resultant CRC by appending one byte.
 *
 *  \param  pFcs     CRC value on which to append the byte.
 *  \param  byte     Byte to be added to CRC computation.
 *
 *  \return FCS value.
 */
/*************************************************************************************************/
void FcsAddByte(uint8_t *pFcs, uint8_t byte);

#ifdef __cplusplus
};
#endif

#endif /* FCS_H */
