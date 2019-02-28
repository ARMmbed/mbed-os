/*************************************************************************************************/
/*!
 *  \file
 *
 *  \brief  Cordio pseudo-random number generator interface.
 *
 *  Copyright (c) 2016-2018 Arm Ltd. All Rights Reserved.
 *  ARM confidential and proprietary.
 *
 *  IMPORTANT.  Your use of this file is governed by a Software License Agreement
 *  ("Agreement") that must be accepted in order to download or otherwise receive a
 *  copy of this file.  You may not use or copy this file for any purpose other than
 *  as described in the Agreement.  If you do not agree to all of the terms of the
 *  Agreement do not use this file and delete all copies in your possession or control;
 *  if you do not have a copy of the Agreement, you must contact ARM Ltd. prior
 *  to any use, copying or further distribution of this software.
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
