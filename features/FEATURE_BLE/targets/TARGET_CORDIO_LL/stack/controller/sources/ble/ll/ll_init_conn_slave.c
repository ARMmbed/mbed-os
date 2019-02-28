/*************************************************************************************************/
/*!
 *  \file
 *
 *  \brief      Link layer (LL) slave initialization implementation file.
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

#include "lctr_api_adv_slave.h"
#include "lctr_api_conn.h"
#include "lmgr_api_adv_slave.h"
#include "lmgr_api_conn.h"
#include "wsf_assert.h"

/*************************************************************************************************/
/*!
 *  \brief      Initialize memory for connections.
 *
 *  \param      pFreeMem        Pointer to free memory.
 *  \param      freeMemSize     Size of pFreeMem.
 *
 *  \return     Amount of free memory consumed.
 *
 *  This function allocates memory for connections.
 *
 *  \note       This routine must be called after LlInitRunTimeCfg() but only once before any
 *              other initialization routines.
 */
/*************************************************************************************************/
uint16_t LlInitConnMem(uint8_t *pFreeMem, uint32_t freeMemSize)
{
  WSF_ASSERT(pLctrRtCfg);
  WSF_ASSERT(pFreeMem);

  uint16_t bytesUsed = 0;

#if (LL_MAX_CONN > 0)       /* connections capable */
  bytesUsed = LctrInitConnMem(pFreeMem, freeMemSize);
#endif

  return bytesUsed;
}

/*************************************************************************************************/
/*!
 *  \brief      Initialize LL subsystem for operation as a connectable slave.
 *
 *  \return     None.
 *
 *  This function initializes the LL subsystem for use as an advertising and connectable slave.
 */
/*************************************************************************************************/
void LlConnSlaveInit(void)
{
  WSF_ASSERT(pLctrRtCfg);     /* Runtime configuration must be available. */

  LmgrConnInit();
  LctrSlvConnInit();
}

/*************************************************************************************************/
/*!
 *  \brief      Register ACL handler.
 *
 *  \param      sendCompCback   Client ACL send complete callback function.
 *  \param      recvPendCback   Client ACL receive pending callback function.
 *
 *  \return     None.
 *
 *  This function is called by a client to register for ACL data.
 */
/*************************************************************************************************/
void LlAclRegister(llAclCback_t sendCompCback, llAclCback_t recvPendCback)
{
  lmgrPersistCb.sendCompCback = sendCompCback;
  lmgrPersistCb.recvPendCback = recvPendCback;
}
