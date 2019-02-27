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
 *  \brief ATT client service and characteristic utility functions.
 */
/*************************************************************************************************/

#include <string.h>
#include "wsf_types.h"
#include "wsf_trace.h"
#include "util/bstream.h"
#include "att_api.h"
#include "att_uuid.h"
#include "att_main.h"

/**************************************************************************************************
  Macros
**************************************************************************************************/

#define ATT_DISC_HDL_IDX_NONE           0xFF

#define ATT_SET_UUID_MASK               ATTC_SET_UUID_128

/* Characteristic declaration lengths */
#define ATT_CHAR_DECL_LEN_UUID16        5
#define ATT_CHAR_DECL_LEN_UUID128       19

/* Read by type response lengths for characteristic discovery */
#define ATT_READ_RSP_LEN_UUID16         (ATT_CHAR_DECL_LEN_UUID16 + 2)
#define ATT_READ_RSP_LEN_UUID128        (ATT_CHAR_DECL_LEN_UUID128 + 2)

/* Find info response lengths */
#define ATT_FIND_RSP_LEN_UUID16         (ATT_16_UUID_LEN + 2)
#define ATT_FIND_RSP_LEN_UUID128        (ATT_128_UUID_LEN + 2)

/*************************************************************************************************/
/*!
 *  \brief  Compare two UUIDs.
 *
 *  \param  pChar       Characteristic that we are looking for.
 *  \param  pUuid       Pointer to peer UUID data.
 *  \param  settings    Indicates 16 or 128 bit UUID.
 *
 *  \return TRUE if UUIDs match.
 */
/*************************************************************************************************/
static bool_t attcUuidCmp(attcDiscChar_t *pChar, uint8_t *pUuid, uint8_t settings)
{
  /* if both uuids are the same length */
  if ((pChar->settings & ATT_SET_UUID_MASK) == settings)
  {
    /* simply compare the data */
    return (memcmp(pChar->pUuid, pUuid, (settings == 0) ? ATT_16_UUID_LEN : ATT_128_UUID_LEN) == 0);
  }
  /* if discovered UUID is 128 bit and our UUID is 16 bit */
  else if ((settings == ATTC_SET_UUID_128) && ((pChar->settings & ATTC_SET_UUID_128) == 0))
  {
    /* convert our UUID to 128 bit and compare */
    return attUuidCmp16to128(pChar->pUuid, pUuid);
  }
  /* else discovered UUID is 16 bit and our UUID is 128 bit */
  else
  {
    /* no match */
    return FALSE;
  }
}

/*************************************************************************************************/
/*!
 *  \brief  Verify that required characterstics and descriptors have been discovered.
 *
 *  \param  pCb         Pointer to service discovery control block.
 *
 *  \return ATT_SUCCESS if discovery procedure completed successfully.
 *          Otherwise the discovery procedure failed.
 */
/*************************************************************************************************/
static uint8_t attcDiscVerify(attcDiscCb_t *pCb)
{
  attcDiscChar_t  **pChar;
  uint8_t         i;

  /* for each characteristic */
  for (i = 0, pChar = pCb->pCharList; i < pCb->charListLen; i++, pChar++)
  {
    /* if characteristic required */
    if (((*pChar)->settings & ATTC_SET_REQUIRED) != 0)
    {
      /* verify handle was discovered */
      if (pCb->pHdlList[i] == ATT_HANDLE_NONE)
      {
        return ATT_ERR_REQ_NOT_FOUND;
      }
    }
  }

  return ATT_SUCCESS;
}

/*************************************************************************************************/
/*!
 *  \brief  Find next characteristic that requires descriptor discovery.  If none found,
 *          discovery is complete; verify that required characterstics have been discovered.
 *
 *  \param  connId      DM connection ID.
 *  \param  pCb         Pointer to service discovery control block.
 *
 *  \return ATT_CONTINUING if successful and discovery procedure is continuing.
 *          ATT_SUCCESS if discovery procedure completed successfully.
 *          Otherwise the discovery procedure failed.
 */
/*************************************************************************************************/
static uint8_t attcDiscDescriptors(dmConnId_t connId, attcDiscCb_t *pCb)
{
  attcDiscChar_t  **pChar;
  uint16_t        startHdl = ATT_HANDLE_NONE;
  uint16_t        endHdl = ATT_HANDLE_NONE;

  /* find next descriptor in list */
  pChar = pCb->pCharList + pCb->charListIdx;
  while (pCb->charListIdx < pCb->charListLen)
  {
    /* if this is a descriptor */
    if (((*pChar)->settings & ATTC_SET_DESCRIPTOR) != 0)
    {
      /* start handle is one greater than characteristic value handle,
       * which is stored in the previous entry in the list;
       * end handle is stored at current entry in the list
       */
      startHdl = pCb->pHdlList[pCb->charListIdx - 1] + 1;
      endHdl = pCb->pHdlList[pCb->charListIdx];

      /* clear temp end handle */
      pCb->pHdlList[pCb->charListIdx] = ATT_HANDLE_NONE;

      /* if there are descriptors */
      if (startHdl <= endHdl)
      {
        break;
      }
      else
      {
        /* we are looking for descriptors for this characteristic but
         * there aren't any;
         * skip over any other descriptors that follow in our list
         */
        while (++pCb->charListIdx < pCb->charListLen)
        {
          pChar++;
          if ((*pChar)->settings & ATTC_SET_DESCRIPTOR)
          {
            continue;
          }
        }
      }
    }
    else
    {
      /* go to next in list */
      pChar++;
      pCb->charListIdx++;
    }
  }

  /* if no more descriptors to be discovered */
  if (pCb->charListIdx == pCb->charListLen)
  {
    /* we're done; verify required characteristics and descriptors were discovered */
    return attcDiscVerify(pCb);
  }
  /* else initiate characteristic descriptor discovery */
  else
  {
    AttcFindInfoReq(connId, startHdl, endHdl, TRUE);
    return ATT_CONTINUING;
  }
}

/*************************************************************************************************/
/*!
 *  \brief  Process a descriptor handle/UUID pair.
 *
 *  \param  pCb         Pointer to service discovery control block.
 *  \param  settings    Indicates 16 or 128 bit UUID.
 *  \param  pPair       Pointer to handle/UUID pair.
 *
 *  \return ATT_CONTINUING if successful and discovery procedure is continuing.
 *          ATT_SUCCESS if discovery procedure completed successfully.
 *          Otherwise the discovery procedure failed.
 */
/*************************************************************************************************/
static void attcDiscProcDescPair(attcDiscCb_t *pCb, uint8_t settings, uint8_t *pPair)
{
  attcDiscChar_t  **pDesc;
  uint16_t        hdl;
  uint8_t         i;

  /* parse handle */
  BSTREAM_TO_UINT16(hdl, pPair);

  /* now pPair points to UUID; find descriptor with matching UUID */
  pDesc = &pCb->pCharList[pCb->charListIdx];
  for (i = pCb->charListIdx;
       (i < pCb->charListLen) && (((*pDesc)->settings & ATTC_SET_DESCRIPTOR) != 0);
       i++, pDesc++)
  {
    /* if characteristic not already found */
    if (pCb->pHdlList[i] == 0)
    {
      /* if UUIDs match */
      if (attcUuidCmp(*pDesc, pPair, settings))
      {
        /* match found; store handle */
        pCb->pHdlList[i] = hdl;

        ATT_TRACE_INFO1("descriptor found handle:0x%x", hdl);
        break;
      }
    }
  }
}

/*************************************************************************************************/
/*!
 *  \brief  Process a ATTC_FIND_INFO_RSP received while performing characteristic discovery.
 *
 *  \param  pCb         Pointer to service discovery control block.
 *  \param  pMsg        ATT callback event message.
 *
 *  \return ATT_CONTINUING if successful and discovery procedure is continuing.
 *          ATT_SUCCESS if discovery procedure completed successfully.
 *          Otherwise the discovery procedure failed.
 */
/*************************************************************************************************/
static uint8_t attcDiscProcDesc(attcDiscCb_t *pCb, attEvt_t *pMsg)
{
  attcDiscChar_t  **pChar;
  uint8_t         *p;
  uint8_t         *pEnd;
  uint8_t         format;
  uint8_t         pairLen;
  uint8_t         settings;

  /* if find info successful */
  if (pMsg->hdr.status == ATT_SUCCESS)
  {
    p = pMsg->pValue;
    pEnd = pMsg->pValue + pMsg->valueLen;

    /* determine UUID format */
    BSTREAM_TO_UINT8(format, p);
    if (format == ATT_FIND_HANDLE_16_UUID)
    {
      settings = 0;
      pairLen = ATT_FIND_RSP_LEN_UUID16;
    }
    else if (format == ATT_FIND_HANDLE_128_UUID)
    {
      settings = ATTC_SET_UUID_128;
      pairLen = ATT_FIND_RSP_LEN_UUID128;
    }
    else
    {
      return ATT_ERR_INVALID_RSP;
    }

    /* for each handle/UUID pair */
    while (p < pEnd)
    {
      /* process descriptor handle/UUID pair */
      attcDiscProcDescPair(pCb, settings, p);

      /* go to next */
      p += pairLen;
    }
  }

  /* if descriptor discovery complete for this characteristic */
  if (pMsg->hdr.status != ATT_SUCCESS || pMsg->continuing == FALSE)
  {
    /* go to next entry in list */
    pChar = &pCb->pCharList[pCb->charListIdx];
    do
    {
      /* check if at end of list */
      pCb->charListIdx++;
      if (pCb->charListIdx == pCb->charListLen)
      {
        break;
      }

      /* skip over descriptors that follow current characteristic */
      pChar++;
    } while ((*pChar)->settings & ATTC_SET_DESCRIPTOR);

    /* proceed with descriptor discovery for the next characteristic */
    return attcDiscDescriptors((dmConnId_t) pMsg->hdr.param, pCb);
  }
  else
  {
    /* still more to do */
    return ATT_CONTINUING;
  }
}

/*************************************************************************************************/
/*!
 *  \brief  Process a characteristic declaration.
 *
 *  \param  pCb         Pointer to service discovery control block.
 *  \param  settings    Indicates 16 or 128 bit UUID.
 *  \param  pDecl       Pointer to declaration.
 *
 *  \return ATT_CONTINUING if successful and discovery procedure is continuing.
 *          ATT_SUCCESS if discovery procedure completed successfully.
 *          Otherwise the discovery procedure failed.
 */
/*************************************************************************************************/
static void attcDiscProcCharDecl(attcDiscCb_t *pCb, uint8_t settings, uint8_t *pDecl)
{
  attcDiscChar_t  **pChar;
  uint16_t        hdl;
  uint16_t        declHdl;
  uint8_t         i;

  /* parse it */
  BSTREAM_TO_UINT16(declHdl, pDecl);
  pDecl++;    /* skip properties field */
  BSTREAM_TO_UINT16(hdl, pDecl);

  /* if looking for end handle of previous characteristic */
  if (pCb->endHdlIdx != ATT_DISC_HDL_IDX_NONE)
  {
    /* end handle of previous characteristic is one less than
     * the handle of the current characteristic declaration
     */
    pCb->pHdlList[pCb->endHdlIdx] = declHdl - 1;
    pCb->endHdlIdx = ATT_DISC_HDL_IDX_NONE;
  }

  /* check handle */
  if (hdl > declHdl && hdl <= pCb->svcEndHdl)
  {
    /* now pDecl points to UUID; search for UUID in characteristic list */
    for (i = 0, pChar = pCb->pCharList; i < pCb->charListLen; i++, pChar++)
    {
      /* if characteristic not already found */
      if (pCb->pHdlList[i] == 0)
      {
        /* if UUIDs match */
        if (attcUuidCmp(*pChar, pDecl, settings))
        {
          /* match found; store handle */
          pCb->pHdlList[i] = hdl;

          /* if not at end of list and next in list is a descriptor */
          if (i < (pCb->charListLen - 1) &&
              ((*(pChar + 1))->settings & ATTC_SET_DESCRIPTOR) == ATTC_SET_DESCRIPTOR)
          {
            /* characteristic has descriptors, we need to find end handle
             * store end handle temporarily in handle list location
             * for the first descriptor
             */
            pCb->endHdlIdx = i + 1;
          }

          ATT_TRACE_INFO1("characteristic found handle:0x%x", hdl);
        }
      }
    }
  }
  else
  {
    /* invalid handle; skip this declaration */
    ATT_TRACE_WARN1("invalid handle:0x%x", hdl);
  }
}

/*************************************************************************************************/
/*!
 *  \brief  Process a ATTC_READ_BY_TYPE_RSP received while performing characteristic discovery.
 *
 *  \param  pCb         Pointer to service discovery control block.
 *  \param  pMsg        ATT callback event message.
 *
 *  \return ATT_CONTINUING if successful and discovery procedure is continuing.
 *          ATT_SUCCESS if discovery procedure completed successfully.
 *          Otherwise the discovery procedure failed.
 */
/*************************************************************************************************/
static uint8_t attcDiscProcChar(attcDiscCb_t *pCb, attEvt_t *pMsg)
{
  uint8_t     *p;
  uint8_t     *pEnd;
  uint8_t     pairLen;
  uint8_t     settings;

  /* if read by type successful */
  if (pMsg->hdr.status == ATT_SUCCESS)
  {
    p = pMsg->pValue;
    pEnd = pMsg->pValue + pMsg->valueLen;

    /* verify attribute-handle pair length and determine UUID length */
    BSTREAM_TO_UINT8(pairLen, p);
    if (pairLen == ATT_READ_RSP_LEN_UUID16)
    {
      settings = 0;
    }
    else if (pairLen == ATT_READ_RSP_LEN_UUID128)
    {
      settings = ATTC_SET_UUID_128;
    }
    else
    {
      return ATT_ERR_INVALID_RSP;
    }

    /* Note that ATT already verifies response length and handle values */

    /* for each characteristic declaration */
    while (p < pEnd)
    {
      /* process characteristic declaration */
      attcDiscProcCharDecl(pCb, settings, p);

      /* go to next */
      p += pairLen;
    }
  }

  /* if characteristic discovery complete */
  if (pMsg->hdr.status != ATT_SUCCESS || pMsg->continuing == FALSE)
  {
    /* check if characteristic end handle needs to be set */
    if (pCb->endHdlIdx != ATT_DISC_HDL_IDX_NONE)
    {
      /* end handle of characteristic declaration is end handle of service */
      pCb->pHdlList[pCb->endHdlIdx] = pCb->svcEndHdl;
    }

    /* proceed with descriptor discovery */
    pCb->charListIdx = 0;
    return attcDiscDescriptors((dmConnId_t) pMsg->hdr.param, pCb);
  }
  else
  {
    /* still more to do */
    return ATT_CONTINUING;
  }
}

/*************************************************************************************************/
/*!
 *  \brief  Initiates the next characteristic configuration procedure.
 *
 *  \param  pCb         Pointer to service discovery control block.
 *  \param  pMsg        ATT callback event message.
 *
 *  \return ATT_CONTINUING if successful and configuration procedure is continuing.
 *          ATT_SUCCESS if configuration procedure completed successfully.
 */
/*************************************************************************************************/
static uint8_t attcDiscConfigNext(dmConnId_t connId, attcDiscCb_t *pCb)
{
  attcDiscCfg_t *pCfg;

  pCfg = pCb->pCfgList + pCb->charListIdx;

  /* iterate over list */
  while (pCb->charListIdx < pCb->cfgListLen)
  {
    /* if handle was discovered */
    if (pCb->pHdlList[pCfg->hdlIdx] != ATT_HANDLE_NONE)
    {
      /* if value present do write req */
      if (pCfg->valueLen != 0)
      {
        AttcWriteReq(connId, pCb->pHdlList[pCfg->hdlIdx], pCfg->valueLen, (uint8_t *) pCfg->pValue);
      }
      /* else do read */
      else
      {
        AttcReadReq(connId, pCb->pHdlList[pCfg->hdlIdx]);
      }

      /* done for now */
      return ATT_CONTINUING;
    }

    /* next in list */
    pCb->charListIdx++;
    pCfg++;
  }

  /* nothing left to configure; we're done */
  return ATT_SUCCESS;
}

/*************************************************************************************************/
/*!
 *  \brief  This utility function discovers the given service on a peer device.  Function
 *          AttcFindByTypeValueReq() is called to initiate the discovery procedure.
 *
 *  \param  connId      DM connection ID.
 *  \param  pCb         Pointer to service discovery control block.
 *  \param  uuidLen     Length of UUID (2 or 16).
 *  \param  pUuid       Pointer to UUID data.
 *
 *  \return None.
 */
/*************************************************************************************************/
void AttcDiscService(dmConnId_t connId, attcDiscCb_t *pCb, uint8_t uuidLen, uint8_t *pUuid)
{
  AttcFindByTypeValueReq(connId, ATT_HANDLE_START, ATT_HANDLE_MAX, ATT_UUID_PRIMARY_SERVICE,
                         uuidLen, pUuid, FALSE);
}

/*************************************************************************************************/
/*!
 *  \brief  This utility function processes a service discovery result.  It should be called
 *          when an ATTC_FIND_BY_TYPE_VALUE_RSP callback event is received after service
 *          discovery is initiated by calling AttcDiscService().
 *
 *  \param  pCb         Pointer to service discovery control block.
 *  \param  pMsg        ATT callback event message.
 *
 *  \return ATT_SUCCESS if successful otherwise error.
 */
/*************************************************************************************************/
uint8_t AttcDiscServiceCmpl(attcDiscCb_t *pCb, attEvt_t *pMsg)
{
  uint8_t *p;

  /* verify callback event */
  if (pMsg->hdr.event != ATTC_FIND_BY_TYPE_VALUE_RSP)
  {
    ATT_TRACE_WARN1("unexpected callback event %d", pMsg->hdr.event);
    return ATT_ERR_UNDEFINED;
  }
  /* verify status */
  else if (pMsg->hdr.status != ATT_SUCCESS)
  {
    return pMsg->hdr.status;
  }
  /* verify result was found */
  else if (pMsg->valueLen == 0)
  {
    return ATT_ERR_NOT_FOUND;
  }

  /* get handles of first returned service only; ATT has already performed error checking */
  p = pMsg->pValue;
  BSTREAM_TO_UINT16(pCb->svcStartHdl, p);
  BSTREAM_TO_UINT16(pCb->svcEndHdl, p);

  ATT_TRACE_INFO2("found service startHdl=0x%x endHdl=0x%x", pCb->svcStartHdl, pCb->svcEndHdl);

  return ATT_SUCCESS;
}

/*************************************************************************************************/
/*!
 *  \brief  This utility function starts characteristic and characteristic descriptor
 *          discovery for a service on a peer device.  The service must have been previously
 *          discovered by calling AttcDiscService() and AttcDiscServiceCmpl().
 *
 *  \param  connId      DM connection ID.
 *  \param  pCb         Pointer to service discovery control block.
 *
 *  \return None.
 */
/*************************************************************************************************/
void AttcDiscCharStart(dmConnId_t connId, attcDiscCb_t *pCb)
{
  /* initialize control block */
  pCb->charListIdx = 0;
  pCb->endHdlIdx = ATT_DISC_HDL_IDX_NONE;

  AttcReadByTypeReq(connId, pCb->svcStartHdl, pCb->svcEndHdl, ATT_16_UUID_LEN,
                    (uint8_t *) attChUuid, TRUE);
}

/*************************************************************************************************/
/*!
 *  \brief  This utility function processes a characteristic discovery result.  It should be
 *          called when an ATTC_READ_BY_TYPE_RSP or ATTC_FIND_INFO_RSP callback event is
 *          received after characteristic discovery is initiated by calling AttcDiscCharStart().
 *
 *  \param  pCb         Pointer to service discovery control block.
 *  \param  pMsg        ATT callback event message.
 *
 *  \return ATT_CONTINUING if successful and discovery procedure is continuing.
 *          ATT_SUCCESS if discovery procedure completed successfully.
 *          Otherwise the discovery procedure failed.
 */
/*************************************************************************************************/
uint8_t AttcDiscCharCmpl(attcDiscCb_t *pCb, attEvt_t *pMsg)
{
  uint8_t status;

  /* verify callback event */
  if (pMsg->hdr.event != ATTC_READ_BY_TYPE_RSP &&
      pMsg->hdr.event != ATTC_FIND_INFO_RSP)
  {
    ATT_TRACE_WARN1("unexpected callback event %d", pMsg->hdr.event);
    return ATT_ERR_UNDEFINED;
  }

  /* if read by type (characteristic discovery) */
  if (pMsg->hdr.event == ATTC_READ_BY_TYPE_RSP)
  {
    status = attcDiscProcChar(pCb, pMsg);
  }
  /* else if find info (descriptor discovery) */
  else
  {
    status = attcDiscProcDesc(pCb, pMsg);
  }

  /* if characteristic discovery failed clear any handles */
  if (status != ATT_SUCCESS && status != ATT_CONTINUING)
  {
    memset(pCb->pHdlList, 0, (pCb->charListLen * sizeof(uint16_t)));
  }

  return status;
}

/*************************************************************************************************/
/*!
 *  \brief  This utility function starts characteristic configuration for characteristics on a
 *          peer device.  The characteristics must have been previously discovered by calling
 *          AttcDiscCharStart() and AttcDisccharCont().
 *
 *  \param  connId      DM connection ID.
 *  \param  pCb         Pointer to service discovery control block.
 *
 *  \return ATT_CONTINUING if successful and configuration procedure is continuing.
 *          ATT_SUCCESS if nothing to configure.
 */
/*************************************************************************************************/
uint8_t AttcDiscConfigStart(dmConnId_t connId, attcDiscCb_t *pCb)
{
  /* use char list index to iterate over config list */
  pCb->charListIdx = 0;

  return attcDiscConfigNext(connId, pCb);
}

/*************************************************************************************************/
/*!
 *  \brief  This utility function initiates the next characteristic configuration procedure.
 *          It should be called when an ATTC_READ_RSP or ATTC_WRITE_RSP callback event is received
 *          after characteristic configuration is initiated by calling AttcDiscConfigStart().
 *
 *  \param  connId      DM connection ID.
 *  \param  pCb         Pointer to discovery control block.
 *
 *  \return ATT_CONTINUING if successful and configuration procedure is continuing.
 *          ATT_SUCCESS if configuration procedure completed successfully.
 */
/*************************************************************************************************/
uint8_t AttcDiscConfigCmpl(dmConnId_t connId, attcDiscCb_t *pCb)
{
  /* go to next in list */
  pCb->charListIdx++;

  return attcDiscConfigNext(connId, pCb);
}

/*************************************************************************************************/
/*!
 *  \brief  This utility function resumes the characteristic configuration procedure.  It can
 *          be called when an ATTC_READ_RSP or ATTC_WRITE_RSP callback event is received
 *          with failure status to attempt the read or write procedure again.
 *
 *  \param  connId      DM connection ID.
 *  \param  pCb         Pointer to discovery control block.
 *
 *  \return ATT_CONTINUING if successful and configuration procedure is continuing.
 *          ATT_SUCCESS if configuration procedure completed successfully.
 */
/*************************************************************************************************/
uint8_t AttcDiscConfigResume(dmConnId_t connId, attcDiscCb_t *pCb)
{
  return attcDiscConfigNext(connId, pCb);
}
