/*************************************************************************************************/
/*!
 *  \file
 *
 *  \brief  System configuration definition.
 *
 *  Copyright (c) 2013-2019 Arm Ltd. All Rights Reserved.
 *
 *  Copyright (c) 2019-2020 Packetcraft, Inc.
 *  
 *  Licensed under the Apache License, Version 2.0 (the "License");
 *  you may not use this file except in compliance with the License.
 *  You may obtain a copy of the License at
 *  
 *      http://www.apache.org/licenses/LICENSE-2.0
 *  
 *  Unless required by applicable law or agreed to in writing, software
 *  distributed under the License is distributed on an "AS IS" BASIS,
 *  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *  See the License for the specific language governing permissions and
 *  limitations under the License.
 */
/*************************************************************************************************/

#include "pal_cfg.h"
#include "nrf.h"

/**************************************************************************************************
  Type Definitions
**************************************************************************************************/

/*! \brief  LL configuration. */
typedef struct
{
  /* Advertiser */
  uint8_t  maxAdvSets;              /*!< Maximum number of advertising sets. */
  uint8_t  maxAdvReports;           /*!< Maximum number of pending legacy or extended advertising reports. */
  uint16_t maxExtAdvDataLen;        /*!< Maximum extended advertising data size. */
  uint8_t  defExtAdvDataFragLen;    /*!< Default extended advertising data fragmentation size. */
  uint16_t auxDelayUsec;            /*!< Additional Auxiliary Offset delay above T_MAFS in microseconds. */
  uint16_t auxPtrOffsetUsec;        /*!< Delay of auxiliary packet in microseconds from the time specified by auxPtr. */
  /* Scanner */
  uint8_t  maxScanReqRcvdEvt;       /*!< Maximum scan request received events. */
  uint16_t maxExtScanDataLen;       /*!< Maximum extended scan data size. */
  /* Connection */
  uint8_t  maxConn;                 /*!< Maximum number of connections. */
  uint8_t  numTxBufs;               /*!< Default number of transmit buffers. */
  uint8_t  numRxBufs;               /*!< Default number of receive buffers. */
  uint16_t maxAclLen;               /*!< Maximum ACL buffer size. */
  int8_t   defTxPwrLvl;             /*!< Default Tx power level for connections. */
  uint8_t  ceJitterUsec;            /*!< Allowable CE jitter on a slave (account for master's sleep clock resolution). */
  /* ISO */
  uint8_t  numIsoTxBuf;             /*!< Default number of ISO transmit buffers. */
  uint8_t  numIsoRxBuf;             /*!< Default number of ISO receive buffers. */
  uint16_t maxIsoSduLen;            /*!< Maximum ISO buffer size between host and controller. */
  uint16_t maxIsoPduLen;            /*!< Maximum ISO PDU size between controllers. */

  /* CIS */
  uint8_t  maxCig;                  /*!< Maximum number of CIG. */
  uint8_t  maxCis;                  /*!< Maximum number of CIS, it is shared by the CIGs. */
  uint16_t cisSubEvtSpaceDelay;     /*!< Subevent spacing above T_MSS. */

  /* BIS*/
  uint8_t  maxBig;                  /*!< Maximum number of BIG. */
  uint8_t  maxBis;                  /*!< Maximum number of BIS. */
  /* DTM */
  uint16_t dtmRxSyncMs;             /*!< DTM Rx synchronization window in milliseconds. */
} PalCfgLl_t;

/**************************************************************************************************
  Macros
**************************************************************************************************/

/*! \brief convert uint32_t to little endian byte stream, incrementing four bytes. */
#define PAL_UINT32_TO_BSTREAM(p, n)   {*(p)++ = (uint8_t)(n); *(p)++ = (uint8_t)((n) >> 8); \
                                       *(p)++ = (uint8_t)((n) >> 16); *(p)++ = (uint8_t)((n) >> 24);}

/**************************************************************************************************
  Functions
**************************************************************************************************/

/*************************************************************************************************/
/*!
 *  \brief      Get BLE PHY feature configuration.
 *
 *  \param      phy2mSup            2M PHY supported.
 *  \param      phyCodedSup         Coded PHY supported.
 *  \param      stableModIdxTxSup   Tx stable modulation index supported.
 *  \param      stableModIdxRxSup   Rx stable modulation index supported.
 */
/*************************************************************************************************/
void palCfgGetBlePhyFeatures(uint8_t *pPhy2mSup, uint8_t *pPhyCodedSup,
                             uint8_t *pStableModIdxTxSup, uint8_t *pStableModIdxRxSup)
{
#if defined(NRF52840_XXAA) || defined(NRF52832_XXAA)
  *pPhy2mSup          = TRUE;
  *pPhyCodedSup       = TRUE;
#else
  *pPhy2mSup          = FALSE;
  *pPhyCodedSup       = FALSE;
#endif
  *pStableModIdxTxSup = TRUE;
  *pStableModIdxRxSup = TRUE;
}

/*************************************************************************************************/
/*!
 *  \brief      Load LL advertising configuration.
 *
 *  \param      pConfig                Return configuration values.
 */
/*************************************************************************************************/
void palCfgLoadLlParams(uint8_t *pConfig)
{
  PalCfgLl_t *pCfg = (PalCfgLl_t *)pConfig;

#if defined(BOARD_NRF6832)
  const uint16_t maxAdvSets   = 1;
  const uint16_t advDataLen   = 256;
  const uint16_t aclDataLen   = 256;
  const uint16_t maxConn      = 1;
  const uint16_t maxGroup     = 1;
  const uint16_t maxStream    = 2;
#elif !defined(NRF52840_XXAA)
  const uint16_t maxAdvSets   = 1;
  const uint16_t advDataLen   = 512;
  const uint16_t aclDataLen   = 512;
  const uint16_t maxConn      = 1;
  const uint16_t maxGroup     = 1;
  const uint16_t maxStream    = 2;
#else  /* Default */
  const uint16_t maxAdvSets   = 6;
  const uint16_t advDataLen   = 1650;
  const uint16_t aclDataLen   = 512;
  const uint16_t maxConn      = 4;
  const uint16_t maxGroup     = 2;
  const uint16_t maxStream    = 6;
#endif

  pCfg->maxAdvSets            = maxAdvSets;
  pCfg->maxAdvReports         = 8;
  pCfg->maxExtAdvDataLen      = advDataLen;
  /* pCfg->defExtAdvDataFragLen */  /* Use default. */
  pCfg->auxDelayUsec          = 0;
  pCfg->maxScanReqRcvdEvt     = 4;
  pCfg->maxExtScanDataLen     = advDataLen;
  pCfg->maxConn               = maxConn;
  pCfg->maxAclLen             = aclDataLen;
  pCfg->numTxBufs             = 16;
  pCfg->numRxBufs             = 8;
  pCfg->numIsoTxBuf           = 16;
  pCfg->numIsoRxBuf           = 8;
  pCfg->maxIsoSduLen          = aclDataLen;
  pCfg->maxIsoPduLen          = 251;
  pCfg->maxCig                = maxGroup;
  pCfg->maxCis                = maxStream;
  pCfg->cisSubEvtSpaceDelay   = 0;
  pCfg->maxBig                = maxGroup;
  pCfg->maxBis                = maxStream;
}

/*************************************************************************************************/
/*!
 *  \brief      Load device address.
 *
 *  \param      pDevAddr            device address.
 */
/*************************************************************************************************/
void palCfgLoadBdAddress(uint8_t *pDevAddr)
{
  unsigned int devAddrLen = 6;

  /* Load address from nRF configuration. */
  uint64_t devAddr = (((uint64_t)NRF_FICR->DEVICEID[0]) <<  0) |
                     (((uint64_t)NRF_FICR->DEVICEID[1]) << 32);

  unsigned int i = 0;
  while (i < devAddrLen)
  {
    pDevAddr[i] = devAddr >> (i * 8);
    i++;
  }

  pDevAddr[5] |= 0xC0;     /* cf. "Static Address" (Vol C, Part 3, section 10.8.1) */
}

/*************************************************************************************************/
/*!
 *  \brief      Load 15.4 address.
 *
 *  \param      pDevAddr            device address.
 */
/*************************************************************************************************/
void palCfgLoadExtMac154Address(uint8_t *pDevAddr)
{
  unsigned int devAddrLen = 8;

  /* Load address from nRF configuration. */
  uint64_t devAddr = (((uint64_t)NRF_FICR->DEVICEID[0]) <<  0) |
                     (((uint64_t)NRF_FICR->DEVICEID[1]) << 32);

  unsigned int i = 0;
  while (i < devAddrLen)
  {
    pDevAddr[i] = devAddr >> (i * 8);
    i++;
  }
}

/*************************************************************************************************/
/*!
 *  \brief      Set device UUID.
 *
 *  \param      pBuf                Return device UUID.
 */
/*************************************************************************************************/
void PalCfgSetDeviceUuid(uint8_t *pBuf)
{
  /* Not used on this platform. */
  (void)pBuf;
}

/*************************************************************************************************/
/*!
 *  \brief      Load device UUID.
 *
 *  \param      pDevUuid                Return device UUID.
 */
/*************************************************************************************************/
void palCfgLoadDeviceUuid(uint8_t *pDevUuid)
{
  uint8_t *pPtr = pDevUuid;

  /* Load Device UUID. */
  PAL_UINT32_TO_BSTREAM(pPtr, NRF_FICR->DEVICEID[0]);
  PAL_UINT32_TO_BSTREAM(pPtr, NRF_FICR->DEVICEID[1]);
#if defined(NRF52840_XXAA) || defined(NRF52832_XXAA)
  PAL_UINT32_TO_BSTREAM(pPtr, NRF_FICR->INFO.PART);
  PAL_UINT32_TO_BSTREAM(pPtr, NRF_FICR->INFO.VARIANT);
#endif

  /* Set the version number as defined in RFC4122 */
  pDevUuid[7] = ((pDevUuid[7] & 0x0F) | 0x40);
  /* Set the algorithm bits as defined in RFC4122 */
  pDevUuid[8] = ((pDevUuid[8] & 0x3F) | 0x80);
}

/*************************************************************************************************/
/*!
 *  \brief      Load device UUID.
 *
 *  \param      cfgId                Configuration ID.
 *  \param      pBuf                 Buffer.
 *  \param      len                  Buffer length.
 */
/*************************************************************************************************/
void PalCfgLoadData(uint8_t cfgId, void *pBuf, uint32_t len)
{
  (void)len;

  switch (cfgId)
  {
    case PAL_CFG_ID_BD_ADDR:
      palCfgLoadBdAddress(pBuf);
      break;

    case PAL_CFG_ID_BLE_PHY:
      palCfgGetBlePhyFeatures(pBuf, pBuf + 1, pBuf + 2, pBuf + 3);
      break;

    case PAL_CFG_ID_LL_PARAM:
      palCfgLoadLlParams(pBuf);
      break;

    case PAL_CFG_ID_MAC_ADDR:
      palCfgLoadExtMac154Address(pBuf);
      break;

    case PAL_CFG_ID_UUID:
      palCfgLoadDeviceUuid(pBuf);
      break;

    default:
      break;
  }
}
