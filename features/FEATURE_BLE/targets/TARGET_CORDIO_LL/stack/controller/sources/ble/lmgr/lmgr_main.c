/* Copyright (c) 2009-2019 Arm Limited
 * SPDX-License-Identifier: Apache-2.0
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

/*************************************************************************************************/
/*!
 *  \brief Link manager common implementation file.
 */
/*************************************************************************************************/

#include "lmgr_api.h"
#include "wsf_assert.h"
#include <string.h>

/**************************************************************************************************
  Global Variables
**************************************************************************************************/

/*! \brief      Link layer manager system control block (persists with resets). */
lmgrPersistCtrlBlk_t lmgrPersistCb;

/*! \brief      Link layer manager control block (clears with resets). */
lmgrCtrlBlk_t lmgrCb;

/*************************************************************************************************/
/*!
 *  \brief      Set default values.
 *
 *  \return     None.
 *
 *  Restore default values to fields that require initial state after reset. Values that
 *  survive reset are unchanged.
 */
/*************************************************************************************************/
void LmgrSetDefaults(void)
{
  /* Ensure reset shuts down resources. */
  WSF_ASSERT(lmgrCb.numConnEnabled == 0);
  WSF_ASSERT(lmgrCb.advEnabled == FALSE);
  WSF_ASSERT(lmgrCb.numExtAdvEnabled == 0);
  WSF_ASSERT(lmgrCb.numScanEnabled == 0);
  WSF_ASSERT(lmgrCb.numInitEnabled == 0);
  WSF_ASSERT(lmgrCb.numWlFilterEnabled == 0);
  WSF_ASSERT(lmgrCb.numPlFilterEnabled == 0);
  WSF_ASSERT(lmgrCb.testEnabled == FALSE);

  memset(&lmgrCb, 0, sizeof(lmgrCb));

  lmgrCb.features = lmgrPersistCb.featuresDefault;

  lmgrCb.opModeFlags =
    /* LL_OP_MODE_FLAG_ENA_VER_LLCP_STARTUP | */        /* disabled */
    /* LL_OP_MODE_FLAG_SLV_REQ_IMMED_ACK | */           /* disabled */
    /* LL_OP_MODE_FLAG_BYPASS_CE_GUARD | */             /* disabled */
    /* LL_OP_MODE_FLAG_MST_RETX_AFTER_RX_NACK | */      /* disabled */
    /* LL_OP_MODE_FLAG_MST_IGNORE_CP_RSP | */           /* disabled */
    /* LL_OP_MODE_FLAG_MST_UNCOND_CP_RSP | */           /* disabled */
    /* LL_OP_MODE_FLAG_REQ_SYM_PHY | */                 /* disabled */
    /* LL_OP_MODE_FLAG_ENA_FEAT_LLCP_STARTUP | */       /* disabled */
    /* LL_OP_MODE_FLAG_SLV_DELAY_LLCP_STARTUP | */      /* disabled */
    LL_OP_MODE_FLAG_ENA_LEN_LLCP_STARTUP |
    LL_OP_MODE_FLAG_ENA_ADV_DLY |
    LL_OP_MODE_FLAG_ENA_SCAN_BACKOFF |
    LL_OP_MODE_FLAG_ENA_WW |
    LL_OP_MODE_FLAG_ENA_SLV_LATENCY |
    LL_OP_MODE_FLAG_ENA_LLCP_TIMER;
}

/*************************************************************************************************/
/*!
 *  \brief      Increment reset delay counter.
 *
 *  \return     None.
 */
/*************************************************************************************************/
void LmgrIncResetRefCount(void)
{
  lmgrCb.resetDelayRefCnt++;
}

/*************************************************************************************************/
/*!
 *  \brief      Decrement reset delay counter.
 *
 *  \return     None.
 */
/*************************************************************************************************/
void LmgrDecResetRefCount(void)
{
  WSF_ASSERT(lmgrCb.resetDelayRefCnt);
  lmgrCb.resetDelayRefCnt--;
}

/*************************************************************************************************/
/*!
 *  \brief      Increment whitelist filter enable counter.
 *
 *  \return     None.
 */
/*************************************************************************************************/
void LmgrIncWhitelistRefCount(void)
{
  lmgrCb.numWlFilterEnabled++;
}

/*************************************************************************************************/
/*!
 *  \brief      Decrement whitelist filter enable counter.
 *
 *  \return     None.
 */
/*************************************************************************************************/
void LmgrDecWhitelistRefCount(void)
{
  WSF_ASSERT(lmgrCb.numWlFilterEnabled);
  lmgrCb.numWlFilterEnabled--;
}

/*************************************************************************************************/
/*!
 *  \brief      Increment periodiclist filter enable counter.
 *
 *  \return     None.
 */
/*************************************************************************************************/
void LmgrIncPeriodiclistRefCount(void)
{
  lmgrCb.numPlFilterEnabled++;
}

/*************************************************************************************************/
/*!
 *  \brief      Decrement periodiclist filter enable counter.
 *
 *  \return     None.
 */
/*************************************************************************************************/
void LmgrDecPeriodiclistRefCount(void)
{
  WSF_ASSERT(lmgrCb.numPlFilterEnabled);
  lmgrCb.numPlFilterEnabled--;
}

/*************************************************************************************************/
/*!
 *  \brief  Check that that a given address type is available.
 *
 *  \param  ownAddrType     Own address type.
 *
 *  \return TRUE if value is valid, FALSE otherwise.
 */
/*************************************************************************************************/
bool_t LmgrIsAddressTypeAvailable(uint8_t ownAddrType)
{
  bool_t isAddrTypeValid = TRUE;

  switch (ownAddrType)
  {
    case LL_ADDR_RANDOM:
    case LL_ADDR_RANDOM_IDENTITY:
      if (!lmgrCb.bdAddrRndValid)
      {
        /* Random address not available. */
        isAddrTypeValid = FALSE;
      }
      break;
    case LL_ADDR_PUBLIC:
    case LL_ADDR_PUBLIC_IDENTITY:
    case LL_ADDR_ANONYMOUS:
      /* Addresses are always available. */
      break;
    default:
      /* Invalid type. */
      isAddrTypeValid = FALSE;
  }

  return isAddrTypeValid;
}

/*************************************************************************************************/
/*!
 *  \brief      Check that extended commands have not been issued.
 *
 *  \return     TRUE if allowed, FALSE otherwise.
 */
/*************************************************************************************************/
bool_t LmgrIsLegacyCommandAllowed(void)
{
  if (lmgrCb.useExtCmds)
  {
    return FALSE;
  }

  lmgrCb.useLegacyCmds = TRUE;
  return TRUE;
}

/*************************************************************************************************/
/*!
 *  \brief      Check that legacy commands have not been issued.
 *
 *  \return     TRUE if allowed, FALSE otherwise.
 */
/*************************************************************************************************/
bool_t LmgrIsExtCommandAllowed(void)
{
  if (lmgrCb.useLegacyCmds)
  {
    return FALSE;
  }

  lmgrCb.useExtCmds = TRUE;
  return TRUE;
}
