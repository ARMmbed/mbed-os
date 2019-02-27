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
 *  \brief Pseudo-random number generator interface.
 */
/*************************************************************************************************/

#ifndef PRAND_H
#define PRAND_H

#include "wsf_types.h"

#ifdef __cplusplus
extern "C" {
#endif

/**************************************************************************************************
  Functions
**************************************************************************************************/

/*************************************************************************************************/
/*!
 *  \brief      Initialize random number generator.
 *
 *  \return     None.
 */
/*************************************************************************************************/
void PrandInit(void);

/*************************************************************************************************/
/*!
 *  \brief      Generate random data.
 *
 *  \param      pBuf        Storage for random data.
 *  \param      len         Length of data to generate, in bytes.
 *
 *  \return     None.
 */
/*************************************************************************************************/
void PrandGen(uint8_t *pBuf, uint16_t len);

#ifdef __cplusplus
}
#endif

#endif /* PRAND_H */
