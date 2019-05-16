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
 *  \brief Controller HCI transport module implementation file.
 */
/*************************************************************************************************/

#include "chci_tr.h"
#include "chci_drv.h"
#include "chci_api.h"
#include "hci_defs.h"
#include "wsf_assert.h"
#include "wsf_msg.h"
#include "wsf_os.h"
#include "util/bstream.h"
#include <string.h>

/**************************************************************************************************
  Data Types
**************************************************************************************************/

/*! \brief      Transport control block. */
typedef struct
{
  wsfHandlerId_t              handlerId;            /*!< Handler ID. */
  uint8_t                     *pDataPending;        /*!< Data packet in progress. */
  uint8_t                     typePending;          /*!< Data type in progress. */
  uint8_t                     protPending;          /*!< Protocol in progress. */
  uint8_t                     nextAvailMask;        /*!< Next available mask. */

  ChciTrSendHwErrorCback_t    sendHwErrorCback;     /*!< Send HW error callback. */

  struct
  {
    ChciTrRecvCback_t         recvCback;            /*!< Message received callback. */
    ChciTrSendCompleteCback_t sendCompleteCback;    /*!< Message send complete callback. */
    ChciTrServiceCback_t      serviceCback;         /*!< Service callback. */
  } protCbacks[CHCI_TR_PROT_NUM];                   /*!< Callback array indexed by protocol ID. */
} chciTrCtrlBlk_t;

/*! \brief      Send handler. */
typedef void (*chciTrSendHandler_t)(uint8_t *pBuf);

/*! \brief      Transport events. */
enum
{
  CHCI_TR_EVT_SEND_CMPL = (1 << 0)                  /*!< Send complete. */
};


/**************************************************************************************************
  Global Variables
**************************************************************************************************/

/*! \brief      Current event data in progress. */
chciTrCtrlBlk_t chciTrCb;

/*************************************************************************************************/
/*!
 *  \brief  Initialize the transport handler.
 *
 *  \param  handlerId       Handler ID.
 *
 *  \return None.
 */
/*************************************************************************************************/
void ChciTrHandlerInit(wsfHandlerId_t handlerId)
{
  memset(&chciTrCb, 0, sizeof(chciTrCb));
  chciTrCb.handlerId = handlerId;

  chciDrvInit();
}

/*************************************************************************************************/
/*!
 *  \brief      Controller HCI transport message dispatch handler.
 *
 *  \param      event       WSF event.
 *  \param      pMsg        WSF message.
 *
 *  \return     None.
 */
/*************************************************************************************************/
void ChciTrHandler(wsfEventMask_t event, wsfMsgHdr_t *pMsg)
{
  if (chciTrCb.pDataPending == NULL)
  {
    uint8_t protCount = 0;
    uint8_t prot      = chciTrCb.protPending;   /* last protocol serviced */

    do
    {
      uint8_t  type = 0;
      uint8_t  *pData = NULL;
      uint16_t len = 0;

      prot = (prot + 1 >= CHCI_TR_PROT_NUM) ? 0 : (prot + 1);

      if ((chciTrCb.protCbacks[prot].serviceCback != NULL) &&
          chciTrCb.protCbacks[prot].serviceCback(&type, &len, &pData))
      {
        chciTrCb.pDataPending = pData;
        chciTrCb.typePending  = type;
        chciTrCb.protPending  = prot;

        if (chciDrvWrite(prot, type, len, pData) >= len)
        {
          chciTrSendComplete();
        }
        break;
      }
    } while (++protCount < CHCI_TR_PROT_NUM);
  }
}

/*************************************************************************************************/
/*!
 *  \brief  Set callbacks for a protocol.
 *
 *  \param  prot                Protocol.
 *  \param  recvCback           Message received callback.
 *  \param  sendCompleteCback   Message send complete callback.
 *  \param  serviceCback        Service callback.
 *
 *  \return None.
 */
/*************************************************************************************************/
void ChciTrSetCbacks(uint8_t prot, ChciTrRecvCback_t recvCback, ChciTrSendCompleteCback_t sendCompleteCback,
    ChciTrServiceCback_t serviceCback)
{
  if (prot < CHCI_TR_PROT_NUM)
  {
    chciTrCb.protCbacks[prot].recvCback         = recvCback;
    chciTrCb.protCbacks[prot].sendCompleteCback = sendCompleteCback;
    chciTrCb.protCbacks[prot].serviceCback      = serviceCback;
  }
}

/*************************************************************************************************/
/*!
 *  \brief  Set send hardware error callback.
 *
 *  \param  sendHwErrorCback    Send hardware error callback.
 *
 *  \return None.
 */
/*************************************************************************************************/
void ChciTrSetSendHwErrorCback(ChciTrSendHwErrorCback_t sendHwErrorCback)
{
  chciTrCb.sendHwErrorCback = sendHwErrorCback;
}

/*************************************************************************************************/
/*!
 *  \brief  Flag protocol for needing service.
 *
 *  \param  prot                Protocol.
 *
 *  \return None.
 */
/*************************************************************************************************/
void ChciTrNeedsService(uint8_t prot)
{
  if (chciTrCb.pDataPending == NULL)
  {
    ChciTrHandler(CHCI_TR_EVT_SEND_CMPL, NULL);
  }
}

/*************************************************************************************************/
/*!
 *  \brief  Signal the completion of a message write.
 *
 *  \return None.
 *
 *  This routine is used for asynchronous write operations. When the driver has completed the
 *  use of the write buffer, this routine is called to free the buffer and release flow control.
 */
/*************************************************************************************************/
void chciTrSendComplete(void)
{
  uint8_t *pBuf = chciTrCb.pDataPending;
  uint8_t type  = chciTrCb.typePending;
  uint8_t prot  = chciTrCb.protPending;

  WSF_ASSERT(chciTrCb.protCbacks[prot].sendCompleteCback != NULL);

  /* Protocol will free buffer. */
  chciTrCb.pDataPending = NULL;
  chciTrCb.protCbacks[prot].sendCompleteCback(type, pBuf);
  WsfSetEvent(chciTrCb.handlerId, CHCI_TR_EVT_SEND_CMPL);
}

/*************************************************************************************************/
/*!
 *  \brief  Signal the completion of a message receive.
 *
 *  \param  prot    Protocol.
 *  \param  type    Message type.
 *  \param  pBuf    Message.
 *
 *  \return None.
 */
/*************************************************************************************************/
void chciTrRecv(uint8_t prot, uint8_t type, uint8_t *pBuf)
{
  if (prot < CHCI_TR_PROT_NUM)
  {
    if (chciTrCb.protCbacks[prot].recvCback != NULL)
    {
      /* Protocol will free buffer. */
      chciTrCb.protCbacks[prot].recvCback(type, pBuf);
      return;
    }
  }

  /* Free buffer that cannot be handled. */
  WsfMsgFree(pBuf);
}

/*************************************************************************************************/
/*!
 *  \brief  Signal a hardware error.
 *
 *  \param  code    Error code.
 *
 *  \return None.
 */
/*************************************************************************************************/
void chciTrHwError(uint8_t code)
{
  if (chciTrCb.sendHwErrorCback != NULL)
  {
    chciTrCb.sendHwErrorCback(code);
  }
}
