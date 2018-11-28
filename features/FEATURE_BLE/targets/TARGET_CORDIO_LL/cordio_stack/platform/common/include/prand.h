/*************************************************************************************************/
/*!
 *  \file
 *
 *  \brief  Cordio pseudo-random number generator interface.
 *
 *  Copyright (c) 2009-2018 ARM Ltd., all rights reserved.
 *  SPDX-License-Identifier: LicenseRef-PBL
 *
 *  This file and the related binary are licensed under the
 *  Permissive Binary License, Version 1.0 (the "License");
 *  you may not use these files except in compliance with the License.
 *
 *  You may obtain a copy of the License here:
 *  LICENSE-permissive-binary-license-1.0.txt and at
 *  https://www.mbed.com/licenses/PBL-1.0
 *
 *  See the License for the specific language governing permissions and
 *  limitations under the License.
 */
/*************************************************************************************************/

#ifndef CORDIO_PRAND_H
#define CORDIO_PRAND_H

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

#endif /* CORDIO_PRAND_H */
