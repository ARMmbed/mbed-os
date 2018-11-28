/*************************************************************************************************/
/*!
 *  \file
 *
 *  \brief  Internal link layer controller connection interface file.
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
