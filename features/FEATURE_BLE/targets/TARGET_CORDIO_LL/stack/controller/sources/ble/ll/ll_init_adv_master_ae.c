/*************************************************************************************************/
/*!
 *  \file
 *
 *  \brief      Link layer (LL) master initialization implementation file.
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

#include "ll_api.h"
#include "lmgr_api_adv_master.h"
#include "lctr_api_adv_master_ae.h"
#include "wsf_assert.h"

/*************************************************************************************************/
/*!
 *  \brief      Initialize memory for extended scanning.
 *
 *  \param      pFreeMem        Pointer to free memory.
 *  \param      freeMemSize     Size of pFreeMem.
 *
 *  \return     Amount of free memory consumed.
 *
 *  This function allocates memory for extended scanning.
 *
 *  \note       This routine must be called after LlInitRunTimeCfg() but only once before any
 *              other initialization routines.
 */
/*************************************************************************************************/
uint16_t LlInitExtScanMem(uint8_t *pFreeMem, uint32_t freeMemSize)
{
  WSF_ASSERT(pLctrRtCfg);
  WSF_ASSERT(pFreeMem);

  return LctrInitExtScanMem(pFreeMem, freeMemSize);
}

/*************************************************************************************************/
/*!
 *  \brief      Initialize LL subsystem for operation as an scanning master.
 *
 *  \return     None.
 *
 *  This function initializes the LL subsystem for use as an scanning master.
 */
/*************************************************************************************************/
void LlExtScanMasterInit(void)
{
  WSF_ASSERT(pLctrRtCfg);     /* Runtime configuration must be available. */

  LmgrMstInit();
  LctrMstExtScanInit();
  LctrMstPerCreateSyncInit();
  LctrMstPerScanInit();
}
