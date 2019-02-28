/*************************************************************************************************/
/*!
 *  \file
 *
 *  \brief  Link layer controller master encryption implementation file.
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

#include "lctr_int_conn.h"
#include "lctr_int_conn_master.h"
#include "lctr_int_conn_slave.h"
#include "lctr_int_enc_master.h"
#include "lctr_int_enc_slave.h"
#include "lmgr_api.h"
#include "wsf_assert.h"

/*************************************************************************************************/
/*!
 *  \brief      Process a received data channel PDU in master role.
 *
 *  \param      pCtx    Connection context.
 *  \param      pBuf    PDU buffer.
 *
 *  \return     None.
 */
/*************************************************************************************************/
static void lctrMstEncProcessDataPdu(lctrConnCtx_t *pCtx, uint8_t *pBuf)
{
  if (pCtx->enabled)
  {
    uint8_t ctrlResult, encResult;

    if (((ctrlResult = lctrDecodeCtrlPdu(&lctrDataPdu, pBuf, pCtx->role)) == LL_SUCCESS) ||
        ((encResult = lctrDecodeEncPdu(&lctrDataPdu, pBuf, pCtx->role)) == LL_SUCCESS))
    {
#if (LL_ENABLE_TESTER)
      if ((llTesterCb.rxLlcpFilter & (1 << lctrDataPdu.opcode)) != 0)
      {
        return;
      }
#endif
      if (pCtx->role == LL_ROLE_MASTER)
      {
        lctrMstConnExecuteSm(pCtx, LCTR_CONN_MSG_RX_LLCP);
      }
      else
      {
        lctrSlvConnExecuteSm(pCtx, LCTR_CONN_MSG_RX_LLCP);
      }
    }
    else if ((ctrlResult == LL_ERROR_CODE_INVALID_LMP_PARAMS) ||
             (encResult == LL_ERROR_CODE_INVALID_LMP_PARAMS))
    {
      if (pCtx->role == LL_ROLE_MASTER)
      {
        lctrMstConnExecuteSm(pCtx, LCTR_CONN_MSG_RX_LLCP_INVALID_PARAM);
      }
      else
      {
        lctrSlvConnExecuteSm(pCtx, LCTR_CONN_MSG_RX_LLCP_INVALID_PARAM);
      }
    }
    else
    {
      if (pCtx->role == LL_ROLE_MASTER)
      {
        lctrMstConnExecuteSm(pCtx, LCTR_CONN_MSG_RX_LLCP_UNKNOWN);
      }
      else
      {
        lctrSlvConnExecuteSm(pCtx, LCTR_CONN_MSG_RX_LLCP_UNKNOWN);
      }
    }
  }
}

/*************************************************************************************************/
/*!
 *  \brief      Initialize link layer controller resources for connectable encrypted master.
 *
 *  \return     None.
 */
/*************************************************************************************************/
void LctrMstConnEncInit(void)
{
  LctrMstConnInit();

  /* Add LLCP SM handler. */
  lctrMstLlcpSmTbl[LCTR_LLCP_SM_ENCRYPT] = lctrMstExecuteEncryptSm;
  lctrMstLlcpSmTbl[LCTR_LLCP_SM_PING]    = lctrExecutePingSm;

  /* Add control PDU handler. */
  lctrCtrlPduHdlr = lctrMstEncProcessDataPdu;

  /* Add packet encryption handlers. */
  lctrInitCipherBlkHdlr = BbBleDrvAesInitCipherBlock;
#if (!BB_ENABLE_INLINE_ENC_TX)
  lctrPktEncryptHdlr = BbBleDrvAesCcmEncrypt;
#else
  lctrSetEncryptPktCountHdlr = BbBleDrvSetEncryptPacketCount;
#endif
#if (!BB_ENABLE_INLINE_DEC_RX)
  lctrPktDecryptHdlr = BbBleDrvAesCcmDecrypt;
#else
  lctrSetDecryptPktCountHdlr = BbBleDrvSetDecryptPacketCount;
#endif

  /* Set supported features. */
  lmgrPersistCb.featuresDefault |= LL_FEAT_ENCRYPTION;
  if (pLctrRtCfg->btVer >= LL_VER_BT_CORE_SPEC_4_1)
  {
    lmgrPersistCb.featuresDefault |= LL_FEAT_LE_PING;
  }
}
