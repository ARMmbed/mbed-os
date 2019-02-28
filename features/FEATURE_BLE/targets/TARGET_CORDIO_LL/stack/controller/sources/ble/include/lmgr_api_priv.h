/*************************************************************************************************/
/*!
 *  \file
 *
 *  \brief      Link layer manager privacy interface file.
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

#ifndef LMGR_API_PRIV_H
#define LMGR_API_PRIV_H

#include "lmgr_api.h"

#ifdef __cplusplus
extern "C" {
#endif

/*!
 *  \addtogroup LL_LMGR_API_PRIV
 *  \{
 */

/**************************************************************************************************
  Data Types
**************************************************************************************************/

/*! \brief      Slave role device parameter definition. */
typedef struct
{
  uint32_t          resPrivAddrTimeout; /*!< Resolvable private address timeout. */
  uint8_t           numPendingAddrRes;  /*!< Number of pending address resolutions. */
} lmgrPrivCtrlBlk_t;

/**************************************************************************************************
  Global Variables
**************************************************************************************************/

extern lmgrPrivCtrlBlk_t lmgrPrivCb;

/**************************************************************************************************
  Function Declarations
**************************************************************************************************/

/* Initialization */
void LmgrPrivInit(void);

/*! \} */    /* LL_LMGR_API_PRIV */

#ifdef __cplusplus
};
#endif

#endif /* LMGR_API_PRIV_H */
