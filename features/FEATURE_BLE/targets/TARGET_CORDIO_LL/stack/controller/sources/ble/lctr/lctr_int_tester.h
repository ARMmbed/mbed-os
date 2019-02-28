/*************************************************************************************************/
/*!
 *  \file
 *
 *  \brief  Internal link layer controller connection interface file.
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

#ifndef LCTR_INT_TESTER_H
#define LCTR_INT_TESTER_H

#include "ll_tester_api.h"
#include "wsf_msg.h"

#ifdef __cplusplus
extern "C" {
#endif

/**************************************************************************************************
  Function Declarations
**************************************************************************************************/

void LctrForceTxAcl(uint8_t *pAclBuf);

/*************************************************************************************************/
/*!
 *  \brief      Force transmission of a data PDU.
 *
 *  \param      connHandle  Connection handle.
 *
 *  \return     None.
 */
/*************************************************************************************************/
static inline void lctrForceTxData(uint16_t connHandle)
{
  uint8_t *pAclBuf;

  if ((pAclBuf = WsfMsgAlloc(4 + 4 + 1)) != NULL)
  {
    pAclBuf[0] = connHandle & 0xFF;
    pAclBuf[1] = connHandle >> 8;
    pAclBuf[2] = 0x05;
    pAclBuf[3] = 0x00;
    pAclBuf[4] = 0x01;
    pAclBuf[5] = 0x00;
    pAclBuf[6] = 0x40;
    pAclBuf[7] = 0x00;
    pAclBuf[8] = 0xFF;
    LctrForceTxAcl(pAclBuf);
  }
}

#ifdef __cplusplus
};
#endif

#endif /* LCTR_INT_TESTER_H */
