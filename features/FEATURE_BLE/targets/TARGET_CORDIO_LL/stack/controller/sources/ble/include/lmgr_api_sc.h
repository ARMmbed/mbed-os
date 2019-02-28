/*************************************************************************************************/
/*!
 *  \file
 *
 *  \brief      Link layer manager secure connections interface file.
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

#ifndef LMGR_API_SC_H
#define LMGR_API_SC_H

#include "lmgr_api.h"

#ifdef __cplusplus
extern "C" {
#endif

/*!
 *  \addtogroup LL_LMGR_API_SC
 *  \{
 */

/**************************************************************************************************
  Data Types
**************************************************************************************************/

/*! \brief      Secure connections parameter definition. */
typedef struct
{
  uint8_t   privKey[LL_ECC_KEY_LEN];   /*!< P-256 private key. */
  bool_t    eccOpActive;                /*!< An ECC generation operation is active. */
  bool_t    privKeySet;                 /*!< P-256 private key set; do not generate new one. */
} lmgrScCtrlBlk_t;

/**************************************************************************************************
  Global Variables
**************************************************************************************************/

extern lmgrScCtrlBlk_t lmgrScCb;

/**************************************************************************************************
  Function Declarations
**************************************************************************************************/

/* Initialization */
void LmgrScInit(void);

/*! \} */    /* LL_LMGR_API_SC */

#ifdef __cplusplus
};
#endif

#endif /* LMGR_API_SC_H */
