/*************************************************************************************************/
/*!
 *  \file
 *
 *  \brief  Link layer controller secure connections interface file.
 *
 *  Copyright (c) 2013-2017 ARM Ltd. All Rights Reserved.
 *  ARM Ltd. confidential and proprietary.
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

#ifndef LCTR_API_SC_H
#define LCTR_API_SC_H

#ifdef __cplusplus
extern "C" {
#endif

/*!
 *  \addtogroup LL_LCTR_API_SC
 *  \{
 */

/**************************************************************************************************
  Function Declarations
**************************************************************************************************/

/* Initialization */
void LctrScInit(void);

/* Execution. */
uint8_t LctrGenerateP256KeyPair(void);
uint8_t LctrGenerateDhKey(const uint8_t *pPubKey, const uint8_t *pPrivKey);

/*! \} */    /* LL_LCTR_API_SC */

#ifdef __cplusplus
};
#endif

#endif /* LCTR_API_SC_H */
