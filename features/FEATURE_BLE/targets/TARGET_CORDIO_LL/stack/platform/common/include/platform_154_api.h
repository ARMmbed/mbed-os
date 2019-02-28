/*************************************************************************************************/
/*!
 *  \file
 *
 *  \brief      Platform interface file: mac154-specific.
 *
 *  Copyright (c) 2013-2018 Arm Ltd. All Rights Reserved.
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

#ifndef PLATFORM_154_API_H
#define PLATFORM_154_API_H

#include "wsf_types.h"

/**************************************************************************************************
  Type Definitions
**************************************************************************************************/

/**************************************************************************************************
  Global Variables
**************************************************************************************************/

/**************************************************************************************************
  Function Declarations
**************************************************************************************************/

/*************************************************************************************************/
/*!
 *  \brief      Load extended 802.15.4 MAC device address.
 *
 *  \param      pDevAddr    Return buffer for address.
 *
 *  \return     None.
 */
/*************************************************************************************************/
void PlatformLoadExtMac154Address(uint8_t *pDevAddr);

#endif /* PLATFORM_154_API_H */
