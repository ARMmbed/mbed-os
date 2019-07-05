/* Copyright (c) 2019 Arm Limited
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
 * \file
 * \brief Generic BLE device filtering implementation file.
 */
/*************************************************************************************************/

#include "bb_api.h"
#include "bb_ble_api.h"
#include "bb_ble_api_pdufilt.h"
#include "bb_ble_api_reslist.h"
#include "bb_ble_api_whitelist.h"
#include "wsf_assert.h"
#include "wsf_math.h"
#include "util/bda.h"
#include "util/bstream.h"
#include "ll_defs.h"
#include "cfg_mac_ble.h"
#include <string.h>

/**************************************************************************************************
  Macros
**************************************************************************************************/

/*! \brief      Mask of extended PDUs. */
#define BB_EXT_PDU_FILT_MASK      ((1 << LL_PDU_ADV_EXT_IND) | \
                                   (1 << LL_PDU_AUX_ADV_IND) | \
                                   (1 << LL_PDU_AUX_SCAN_RSP) | \
                                   (1 << LL_PDU_AUX_SYNC_IND) | \
                                   (1 << LL_PDU_AUX_CHAIN_IND) | \
                                   (1 << LL_PDU_AUX_CONNECT_RSP))

/*! \brief      Tri-state result. */
enum
{
  BB_BLE_PDU_FILT_UNKNOWN,
  BB_BLE_PDU_FILT_OK,
  BB_BLE_PDU_FILT_NOT_OK
};

#define BB_ADV_HDR_TX_ADD_SHIFT   6     /*!< ADV header Tx Address type field shift. */
#define BB_ADV_HDR_RX_ADD_SHIFT   7     /*!< ADV header Rx Address type field shift. */

/*! \brief      Increment statistics counter. */
#define BB_INC_PDUFILT_STAT(s)            bbBlePduFiltStats.s++

/**************************************************************************************************
  Global Variables
**************************************************************************************************/

/*! \brief      PDU filter statistics. */
BbBlePduFiltStats_t bbBlePduFiltStats = { 0 };

#if (LL_ENABLE_TESTER)
/*! \brief      Enable PDU filter bypass. */
bool_t bbBlePduFiltEnableBypass = FALSE;
#endif

/*************************************************************************************************/
/*!
 *  \brief      Resolve peer address.
 *
 *  \param      peerAddrResEna    TRUE if peer address resolution is enabled.
 *  \param      forceRes          TRUE if address resolution should be forced.
 *  \param      pPeerAddrRand     TRUE if peer address random (and storage for TRUE for peer ID address).
 *  \param      pPeerAddr         Peer address (and storage for peer ID address).
 *
 *  \return     Tri-state result (OK, NOT_OK, UNKNOWN).
 */
/*************************************************************************************************/
static inline uint8_t bbBlePduFiltResolvePeer(bool_t peerAddrResEna, bool_t forceRes,
                                              uint8_t *pPeerAddrRand, uint64_t *pPeerAddr)
{
  if (peerAddrResEna)
  {
    /* Do not check for RPAs in resolving list. */
    if (*pPeerAddrRand && BDA64_ADDR_IS_RPA(*pPeerAddr))
    {
      uint64_t rpa = *pPeerAddr;

      /* Resolve peer address. */
      if (forceRes)
      {
        /* The identity address or RPA will be accepted based upon match. */
        (void)BbBleResListResolvePeer(rpa, pPeerAddrRand, pPeerAddr);
        return BB_BLE_PDU_FILT_OK;
      }

      /* Pre-resolve peer address. */
      else if (BbBleResListCheckResolvePeer(rpa, pPeerAddrRand, pPeerAddr))
      {
        /* The identity address will be accepted based upon match. */
        return BB_BLE_PDU_FILT_OK;
      }

      else
      {
        /*
         * This address possibly is resolvable.  Match (in case the match is a RPA) or allow
         * re-evaluation at task level.
         */
        BB_INC_PDUFILT_STAT(passPeerAddrResOpt);
        return BB_BLE_PDU_FILT_UNKNOWN;
      }
    }
    else
    {
      switch (BbBleResListPeerStatus(*pPeerAddrRand, *pPeerAddr))
      {
        /*** other than RPA that cannot have RPA ***/

        case BB_BLE_RESLIST_STATUS_ZERO_IRK:
        case BB_BLE_RESLIST_STATUS_ID_ADDR_NOT_IN_LIST:
          /* The address will be accepted based upon match. */
          return BB_BLE_PDU_FILT_OK;

        /*** other than RPA that can have RPA ***/

        case BB_BLE_RESLIST_STATUS_RES_ADDR_ASSIGNED:
        case BB_BLE_RESLIST_STATUS_RES_ADDR_UNASSIGNED:
        {
          uint8_t privMode;

          /* Refuse to accept peer identity address. */
          if (!BbBleResListGetPrivacyMode(*pPeerAddrRand, *pPeerAddr, &privMode) ||
              (privMode == BB_BLE_RESLIST_PRIV_MODE_NETWORK))
          {
            BB_INC_PDUFILT_STAT(failPeerPrivAddrReq);
            return BB_BLE_PDU_FILT_NOT_OK;
          }

          /* The address will be accepted based upon match. */
          else
          {
            return BB_BLE_PDU_FILT_OK;
          }
        }
      }
    }
  }

  /* Address will be accepted based upon match. */
  return BB_BLE_PDU_FILT_OK;
}

/*************************************************************************************************/
/*!
 *  \brief      Match peer address.
 *
 *  \param      addrMatchEna      TRUE if address match enabled.
 *  \param      idAddrRand        TRUE if address random.
 *  \param      idAddr            Address.
 *  \param      addrMatchRand     TRUE if match random.
 *  \param      addrMatch         Match.
 *
 *  \return     Tri-state result (OK, NOT_OK, UNKNOWN).
 */
/*************************************************************************************************/
static inline bool_t bbBlePduFiltMatchPeer(bool_t addrMatchEna, bool_t idAddrRand, uint64_t idAddr,
                                           bool_t addrMatchRand, uint64_t addrMatch)
{
  if (addrMatchEna)
  {
    if ((idAddr != addrMatch) || (idAddrRand != addrMatchRand))
    {
      BB_INC_PDUFILT_STAT(failPeerAddrMatch);
      return BB_BLE_PDU_FILT_NOT_OK;
    }
    else
    {
      BB_INC_PDUFILT_STAT(passPeerAddrMatch);
      return BB_BLE_PDU_FILT_OK;
    }
  }
  return BB_BLE_PDU_FILT_UNKNOWN;
}

/*************************************************************************************************/
/*!
 *  \brief      Whitelist peer address.
 *
 *  \param      peerWhiteListEna  TRUE if peer address whitelist enabled.
 *  \param      peerIdAddrRand    TRUE if peer address random.
 *  \param      peerIdAddr        Peer address.
 *
 *  \return     Tri-state result (OK, NOT_OK, UNKNOWN).
 */
/*************************************************************************************************/
static inline bool_t bbBlePduFiltWhiteList(bool_t peerWhiteListEna, bool_t peerIdAddrRand,
                                           uint64_t peerIdAddr)
{
  if (peerWhiteListEna)
  {
    if ((peerIdAddr == 0) &&                            /* Tx address is empty(AdvA is not present). */
        (BbBleWhiteListIsAnonymousAllowed() == TRUE))   /* Anonymous advertising is allowed. */
    {
      BB_INC_PDUFILT_STAT(passWlFilt);
      return BB_BLE_PDU_FILT_OK;
    }

    if (!BbBleWhiteListCheckAddr(peerIdAddrRand, peerIdAddr))
    {
      BB_INC_PDUFILT_STAT(failWlFilt);
      return BB_BLE_PDU_FILT_NOT_OK;
    }
    else
    {
      BB_INC_PDUFILT_STAT(passWlFilt);
      return BB_BLE_PDU_FILT_OK;
    }
  }
  return BB_BLE_PDU_FILT_UNKNOWN;
}

/*************************************************************************************************/
/*!
 *  \brief      Resolve local address.
 *
 *  \param      localAddrResEna  TRUE if local address resolution is enabled.
 *  \param      forceRes          TRUE if address resolution should be forced.
 *  \param      localAddrRand    TRUE if local address random.
 *  \param      localAddr        Local address.
 *  \param      peerAddrRand     TRUE if peer address random.
 *  \param      peerAddr         Peer address.
 *
 *  \return     Tri-state result (OK, NOT_OK, UNKNOWN).
 */
/*************************************************************************************************/
static inline uint8_t bbBlePduFiltResolveLocal(bool_t localAddrResEna, bool_t forceRes,
                                               uint8_t localAddrRand, uint64_t localAddr,
                                               uint8_t peerAddrRand, uint64_t peerAddr)
{
  if (localAddrResEna)
  {
    /* Do not check for RPAs in resolving list. */
   if (localAddrRand && BDA64_ADDR_IS_RPA(localAddr))
   {
     if (forceRes)
     {
       /* Resolve address if possible.  The check on success will be done in the subsequent check call. */
       (void)BbBleResListResolveLocal(localAddr, &peerAddrRand, &peerAddr);
     }

     /* Pre-resolve local address. */
     if (BbBleResListCheckResolveLocal(localAddr, peerAddrRand, peerAddr))
     {
       /* The address is resolved and passes filter. */
       return BB_BLE_PDU_FILT_OK;
     }
     else
     {
       /*
        * This address possibly is resolvable.  Match (in case the match is a RPA) or allow
        * re-evaluation at task level.
        */
       return BB_BLE_PDU_FILT_UNKNOWN;
     }
   }
   else
   {
      switch (BbBleResListLocalStatus(peerAddrRand, peerAddr))
      {
        /*** other than RPA that cannot have RPA ***/

        case BB_BLE_RESLIST_STATUS_ZERO_IRK:
        case BB_BLE_RESLIST_STATUS_ID_ADDR_NOT_IN_LIST:
          /* Address will be accepted based upon match. */
          return BB_BLE_PDU_FILT_UNKNOWN;

        /*** other than RPA that can have RPA ***/

        case BB_BLE_RESLIST_STATUS_RES_ADDR_ASSIGNED:
        case BB_BLE_RESLIST_STATUS_RES_ADDR_UNASSIGNED:
          /* Refuse to accept local identity address. */
          BB_INC_PDUFILT_STAT(failLocalPrivAddrReq);
          return BB_BLE_PDU_FILT_NOT_OK;
      }
    }
  }

  /* Address will be accepted based upon match. */
  return BB_BLE_PDU_FILT_UNKNOWN;
}

/*************************************************************************************************/
/*!
 *  \brief      Match local address.
 *
 *  \param      addrMatchEna      TRUE if address match enabled.
 *  \param      idAddrRand        TRUE if address random.
 *  \param      idAddr            Address.
 *  \param      addrMatchRand     TRUE if match random.
 *  \param      addrMatch         Match.
 *
 *  \return     Tri-state result (OK, NOT_OK, UNKNOWN).
 */
/*************************************************************************************************/
static inline bool_t bbBlePduFiltMatchLocal(bool_t addrMatchEna, bool_t idAddrRand, uint64_t idAddr,
                                            bool_t addrMatchRand, uint64_t addrMatch)
{
  if (addrMatchEna)
  {
    if ((idAddr != addrMatch) || (idAddrRand != addrMatchRand))
    {
      BB_INC_PDUFILT_STAT(failLocalAddrMatch);
      return BB_BLE_PDU_FILT_NOT_OK;
    }
    else
    {
      BB_INC_PDUFILT_STAT(passLocalAddrMatch);
      return BB_BLE_PDU_FILT_OK;
    }
  }
  return BB_BLE_PDU_FILT_UNKNOWN;
}

/*************************************************************************************************/
/*!
 *  \brief      Check if PDU is allowed, i.e., should not be ignored.
 *
 *  \param      pBuf              PDU buffer.
 *  \param      pFiltParams       Filter parameters.
 *  \param      forceRes          TRUE if address resolution should be forced.
 *  \param      pFiltResults      Storage for filter results.
 *
 *  \return     TRUE if PDU is allowed, FALSE if PDU should be ignored.
 */
/*************************************************************************************************/
bool_t BbBlePduFiltCheck(const uint8_t *pBuf, const bbBlePduFiltParams_t *pFiltParams,
                         bool_t forceRes, bbBlePduFiltResults_t *pFiltResults)
{
#if (LL_ENABLE_TESTER)
  if (bbBlePduFiltEnableBypass)
  {
    pFiltResults->peerAddr     = BstreamToBda64(pBuf + LL_ADV_HDR_LEN);
    pFiltResults->peerAddrRand = ((pBuf[0] >> BB_ADV_HDR_TX_ADD_SHIFT) & 1);
    pFiltResults->peerMatch    = TRUE;
    return TRUE;
  }
#endif

  /*** Filter PDU by PDU type. ***/

  uint8_t pduType = pBuf[0] & LL_ADV_HDR_TYPE_MSK;
  uint8_t pduLen  = pBuf[1] & LL_ADV_HDR_LEN_MSK;

  if ((pFiltParams->pduTypeFilt & (1 << pduType)) == 0)
  {
    BB_INC_PDUFILT_STAT(failPduTypeFilt);
    return FALSE;
  }
  BB_INC_PDUFILT_STAT(passPduTypeFilt);

  if (BB_EXT_PDU_FILT_MASK & (1 << pduType))
  {
    /* Client handles filtering for extended packets. */
    return TRUE;
  }

  pFiltResults->pduType = pduType;
  pFiltResults->pduLen  = pduLen;

  /*** Peer address resolution. ***/

  bool_t   peerAddrRand = ((pBuf[0] >> BB_ADV_HDR_TX_ADD_SHIFT) & 1);
  uint64_t peerAddr     = BstreamToBda64(pBuf + LL_ADV_HDR_LEN);

  pFiltResults->peerAddr     = pFiltResults->peerIdAddr     = peerAddr;
  pFiltResults->peerAddrRand = pFiltResults->peerIdAddrRand = peerAddrRand;

  uint8_t result = BB_BLE_PDU_FILT_UNKNOWN;

  /* Resolve peer address to pass through filters. */
  switch (bbBlePduFiltResolvePeer(BB_BLE_PDU_FILT_FLAG_IS_SET(pFiltParams, PEER_ADDR_RES_ENA),
                                  forceRes,
                                  &pFiltResults->peerIdAddrRand,
                                  &pFiltResults->peerIdAddr))
  {
    case BB_BLE_PDU_FILT_OK:
      result = BB_BLE_PDU_FILT_OK;
      break;
    case BB_BLE_PDU_FILT_NOT_OK:
      return FALSE;
    case BB_BLE_PDU_FILT_UNKNOWN:
      /* Unknown resolution status may be re-evaluated at task level. */
      result = BB_BLE_PDU_FILT_UNKNOWN;
      break;
  }

  /*** Address match OR whitelist (should be exclusive). ***/

  uint8_t matchResult = WSF_MAX(bbBlePduFiltMatchPeer(BB_BLE_PDU_FILT_FLAG_IS_SET(pFiltParams, PEER_ADDR_MATCH_ENA),
                                                      pFiltResults->peerIdAddrRand,
                                                      pFiltResults->peerIdAddr,
                                                      BB_BLE_PDU_FILT_FLAG_IS_SET(pFiltParams, PEER_ADDR_MATCH_RAND),
                                                      pFiltParams->peerAddrMatch),
                                bbBlePduFiltWhiteList((pFiltParams->wlPduTypeFilt & (1 << pduType)) != 0,
                                                      pFiltResults->peerIdAddrRand,
                                                      pFiltResults->peerIdAddr));

  /* Filter PDU by match. */
  switch (matchResult)
  {
    case BB_BLE_PDU_FILT_NOT_OK:
      if (result == BB_BLE_PDU_FILT_OK)        /* Address was resolved but match failed. */
      {
        return FALSE;
      }
      break;
    case BB_BLE_PDU_FILT_OK:
      /* Address matched; lack of resolution is irrelevant. */
      result = BB_BLE_PDU_FILT_OK;
      break;
    case BB_BLE_PDU_FILT_UNKNOWN:
      if (result == BB_BLE_PDU_FILT_UNKNOWN)
      {
        /* Address was not resolved and match is not required, not filter the PDU. */
        result = BB_BLE_PDU_FILT_OK;
      }
      break;
  }

  /*
   * The peer matched if
   *     ((address was resolved)                     AND (match did not fail)) OR
   *     ((address does not violate network privacy) AND (match succeeded)).
   * Otherwise, the resolution status will be re-evaluated at task level.
   */
  pFiltResults->peerMatch = (result == BB_BLE_PDU_FILT_OK);

  /*** Local address. ***/

  /* The address matches by default. */
  pFiltResults->localMatch = TRUE;

  switch (pduType)
  {
    case LL_PDU_ADV_DIRECT_IND:
    case LL_PDU_SCAN_REQ:
    case LL_PDU_CONNECT_IND:
    {
      bool_t   localAddrRand = ((pBuf[0] >> BB_ADV_HDR_RX_ADD_SHIFT) & 1);
      uint64_t localAddr     = BstreamToBda64(pBuf + LL_ADV_HDR_LEN + BDA_ADDR_LEN);

      /*
       * Note: Local address resolution and address match should be effectively exclusive.  Modes
       *       in which the local address is evaluated as a RPA then matched successfully against
       *       the ID address require address resolution enabled and a RPA programmed as the random
       *       address.  The result will be predictable; nevertheless, the scenario is unexpected.
       *
       *       The extended scanner filter policy is a sometime exception to the exclusivity of
       *       resolution and match; however, resolution is not required.
       */

      /* Test local address resolution before testing match. */
      switch (bbBlePduFiltResolveLocal(BB_BLE_PDU_FILT_FLAG_IS_SET(pFiltParams, LOCAL_ADDR_RES_ENA),
                                       forceRes,
                                       localAddrRand,
                                       localAddr,
                                       pFiltResults->peerIdAddrRand,
                                       pFiltResults->peerIdAddr))
      {
        case BB_BLE_PDU_FILT_OK:
          /* The address is resolved and does not need to be compared to the local ID address. */
          return TRUE;
        case BB_BLE_PDU_FILT_NOT_OK:
          /* Reject local address if resolution is required but impossible. */
          return FALSE;
        case BB_BLE_PDU_FILT_UNKNOWN:
          /* Otherwise pass address through match. */
          break;
      }

      /* Test local address match. */
      switch (bbBlePduFiltMatchLocal(BB_BLE_PDU_FILT_FLAG_IS_SET(pFiltParams, LOCAL_ADDR_MATCH_ENA),
                                     localAddrRand,
                                     localAddr,
                                     BB_BLE_PDU_FILT_FLAG_IS_SET(pFiltParams, LOCAL_ADDR_MATCH_RAND),
                                     pFiltParams->localAddrMatch))
      {
        case BB_BLE_PDU_FILT_NOT_OK:
          /* If local address resolution is optional, allow this PDU. */
          if (localAddrRand && BDA64_ADDR_IS_RPA(localAddr) &&
              BB_BLE_PDU_FILT_FLAG_IS_SET(pFiltParams, LOCAL_ADDR_RES_OPT))
          {
            BB_INC_PDUFILT_STAT(passLocalAddrResOpt);
            pFiltResults->localMatch = FALSE;
            return TRUE;
          }
          /* Otherwise ignore this PDU. */
          return FALSE;
        case BB_BLE_PDU_FILT_OK:
        case BB_BLE_PDU_FILT_UNKNOWN:
          /* If address matched or match was not enabled, allow this PDU. */
          break;
      }
      break;
    }

    default:
      /* If no address, allow this PDU. */
      break;
  }

  /* PDU should be allowed. */
  return TRUE;
}

/*************************************************************************************************/
/*!
 *  \brief      Check if PDU is allowed, i.e., should not be ignored.
 *
 *  \param      pExtFiltParams    Extended filter parameters.
 *  \param      pFiltParams       Filter parameters.
 *  \param      forceRes          TRUE if address resolution should be forced.
 *  \param      pFiltResults      Storage for filter results.
 *
 *  \return     TRUE if PDU is allowed, FALSE if PDU should be ignored.
 */
/*************************************************************************************************/
bool_t BbBleExtPduFiltCheck(const bbBlePduExtFiltParams_t *pExtFiltParams,
                            const bbBlePduFiltParams_t *pFiltParams,
                            bool_t forceRes, bbBlePduFiltResults_t *pFiltResults)
{
#if (LL_ENABLE_TESTER)
  if (bbBlePduFiltEnableBypass)
  {
    pFiltResults->peerAddr     = pExtFiltParams->peerAddr;
    pFiltResults->peerAddrRand = pExtFiltParams->peerAddrRand;
    pFiltResults->peerMatch    = TRUE;
    return TRUE;
  }
#endif

  /*** Filter PDU by PDU type. ***/
  if ((pFiltParams->pduTypeFilt & (1 << pExtFiltParams->pduType)) == 0)
  {
    BB_INC_PDUFILT_STAT(failPduTypeFilt);
    return FALSE;
  }
  BB_INC_PDUFILT_STAT(passPduTypeFilt);

  /*** Peer address resolution. ***/

  pFiltResults->peerAddr     = pFiltResults->peerIdAddr     = pExtFiltParams->peerAddr;
  pFiltResults->peerAddrRand = pFiltResults->peerIdAddrRand = pExtFiltParams->peerAddrRand;

  uint8_t result = BB_BLE_PDU_FILT_UNKNOWN;

  /* Resolve peer address to pass through filters. */
  switch (bbBlePduFiltResolvePeer(BB_BLE_PDU_FILT_FLAG_IS_SET(pFiltParams, PEER_ADDR_RES_ENA),
                                  forceRes,
                                  &pFiltResults->peerIdAddrRand,
                                  &pFiltResults->peerIdAddr))
  {
    case BB_BLE_PDU_FILT_OK:
      result = BB_BLE_PDU_FILT_OK;
      break;
    case BB_BLE_PDU_FILT_NOT_OK:
      return FALSE;
    case BB_BLE_PDU_FILT_UNKNOWN:
      /* Unknown resolution status may be re-evaluated at task level. */
      result = BB_BLE_PDU_FILT_UNKNOWN;
      break;
  }

  /*** Address match OR whitelist (should be exclusive). ***/

  uint8_t matchResult = WSF_MAX(bbBlePduFiltMatchPeer(BB_BLE_PDU_FILT_FLAG_IS_SET(pFiltParams, PEER_ADDR_MATCH_ENA),
                                                      pFiltResults->peerIdAddrRand,
                                                      pFiltResults->peerIdAddr,
                                                      BB_BLE_PDU_FILT_FLAG_IS_SET(pFiltParams, PEER_ADDR_MATCH_RAND),
                                                      pFiltParams->peerAddrMatch),
                                bbBlePduFiltWhiteList((pFiltParams->wlPduTypeFilt & (1 << pExtFiltParams->pduType)) != 0,
                                                      pFiltResults->peerIdAddrRand,
                                                      pFiltResults->peerIdAddr));

  /* Filter PDU by match. */
  switch (matchResult)
  {
    case BB_BLE_PDU_FILT_NOT_OK:
      if (result == BB_BLE_PDU_FILT_OK)               /* Address was resolved but match failed. */
      {
        return FALSE;
      }
      break;
    case BB_BLE_PDU_FILT_OK:
      /* Address matched; lack of resolution is irrelevant. */
      result = BB_BLE_PDU_FILT_OK;
      break;
    case BB_BLE_PDU_FILT_UNKNOWN:
      if (result == BB_BLE_PDU_FILT_UNKNOWN)
      {
        /* Address was not resolved and match is not required, not filter the PDU. */
        result = BB_BLE_PDU_FILT_OK;
      }
      break;
  }

  /*
   * The peer matched if
   *     ((address was resolved)                     AND (match did not fail)) OR
   *     ((address does not violate network privacy) AND (match succeeded)).
   * Otherwise, the resolution status will be re-evaluated at task level.
   */
  pFiltResults->peerMatch = (result == BB_BLE_PDU_FILT_OK);

  /*** Local address. ***/

  /* The address matches by default. */
  pFiltResults->localMatch = TRUE;

  if (pExtFiltParams->extHdrFlags & LL_EXT_HDR_TGT_ADDR_BIT)
  {
    bool_t   localAddrRand = pExtFiltParams->localAddrRand;
    uint64_t localAddr     = pExtFiltParams->localAddr;

    /*
     * Note: Local address resolution and address match should be effectively exclusive.  Modes
     *       in which the local address is evaluated as a RPA then matched successfully against
     *       the ID address require address resolution enabled and a RPA programmed as the random
     *       address.  The result will be predictable; nevertheless, the scenario is unexpected.
     *
     *       The extended scanner filter policy is a sometime exception to the exclusivity of
     *       resolution and match; however, resolution is not required.
     */

    /* Test local address resolution before testing match. */
    switch (bbBlePduFiltResolveLocal(BB_BLE_PDU_FILT_FLAG_IS_SET(pFiltParams, LOCAL_ADDR_RES_ENA),
                                     forceRes,
                                     localAddrRand,
                                     localAddr,
                                     pFiltResults->peerIdAddrRand,
                                     pFiltResults->peerIdAddr))
    {
      case BB_BLE_PDU_FILT_OK:
        /* The address is resolved and does not need to be compared to the local ID address. */
        return TRUE;
      case BB_BLE_PDU_FILT_NOT_OK:
        /* Reject local address if resolution is required but impossible. */
        return FALSE;
      case BB_BLE_PDU_FILT_UNKNOWN:
        /* Otherwise pass address through match. */
        break;
    }

    /* Test local address match. */
    switch (bbBlePduFiltMatchLocal(BB_BLE_PDU_FILT_FLAG_IS_SET(pFiltParams, LOCAL_ADDR_MATCH_ENA),
                                   localAddrRand,
                                   localAddr,
                                   BB_BLE_PDU_FILT_FLAG_IS_SET(pFiltParams, LOCAL_ADDR_MATCH_RAND),
                                   pFiltParams->localAddrMatch))
    {
      case BB_BLE_PDU_FILT_NOT_OK:
        /* If local address resolution is optional, allow this PDU. */
        if (localAddrRand && BDA64_ADDR_IS_RPA(localAddr) &&
            BB_BLE_PDU_FILT_FLAG_IS_SET(pFiltParams, LOCAL_ADDR_RES_OPT))
        {
          BB_INC_PDUFILT_STAT(passLocalAddrResOpt);
          pFiltResults->localMatch = FALSE;
          return TRUE;
        }

        /* Otherwise ignore this PDU. */
        return FALSE;
      case BB_BLE_PDU_FILT_OK:
      case BB_BLE_PDU_FILT_UNKNOWN:
        /* If address matched or match was not enabled, allow this PDU. */
        break;
    }
  }

  /* PDU should be allowed. */
  return TRUE;
}

/*************************************************************************************************/
/*!
 *  \brief      Get PDU filter statistics.
 *
 *  \param      pStats      PDU filter statistics.
 *
 *  \return     None.
 */
/*************************************************************************************************/
void BbBleGetPduFiltStats(BbBlePduFiltStats_t *pStats)
{
  *pStats = bbBlePduFiltStats;
}
