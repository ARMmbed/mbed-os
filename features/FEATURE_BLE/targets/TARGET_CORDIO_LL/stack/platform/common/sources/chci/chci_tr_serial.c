/*************************************************************************************************/
/*!
 *  \file
 *
 *  \brief  Controller HCI serial transport module implementation file.
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

#include "chci_api.h"
#include "chci_tr.h"
#include "chci_tr_serial.h"
#include "hci_defs.h"
#include "wsf_assert.h"
#include "wsf_msg.h"
#include "util/bstream.h"
#include <string.h>

/**************************************************************************************************
  Macros
**************************************************************************************************/

/*! \brief      Maximum HCI header size. */
#define LHCI_HDR_LEN_MAX          HCI_ACL_HDR_LEN

/**************************************************************************************************
  Data Types
**************************************************************************************************/

/*! \brief      Receive state. */
typedef enum
{
  CHCI_RX_STATE_IDLE,               /*!< Idle state. */
  CHCI_RX_STATE_HEADER,             /*!< Header parse state. */
  CHCI_RX_STATE_DATA,               /*!< Data parse state. */
  CHCI_RX_STATE_COMPLETE            /*!< Complete state. */
} chciRxState_t;

/*************************************************************************************************/
/*!
 *  \brief  Receive function.  Gets called by external code when bytes are received.
 *
 *  \param  pBuf   Pointer to buffer of incoming bytes.
 *  \param  len    Number of bytes in incoming buffer.
 *
 *  \return None.
 */
/*************************************************************************************************/
void chciTrSerialRxIncoming(uint8_t *pBuf, uint8_t len)
{
  static uint8_t    stateRx = CHCI_RX_STATE_IDLE;
  static uint8_t    pktRx;
  static uint16_t   iRx;
  static uint8_t    hdrRx[LHCI_HDR_LEN_MAX];
  static uint8_t    *pPktRx;
  static uint8_t    *pDataRx;

  uint8_t dataByte;

  /* loop until all bytes of incoming buffer are handled */
  while (len--)
  {
    /* read single byte from incoming buffer and advance to next byte */
    dataByte = *pBuf++;

    /* --- Idle State --- */
    if (stateRx == CHCI_RX_STATE_IDLE)
    {
      switch (dataByte)
      {
        case HCI_CMD_TYPE:
        case HCI_ACL_TYPE:
          /* save the packet type */
          pktRx    = dataByte;
          iRx      = 0;
          stateRx  = CHCI_RX_STATE_HEADER;
          break;

        default:
          /* invalid byte received */
          chciTrHwError(CHCI_TR_CODE_INVALID_DATA);
          break;
      }
    }

    /* --- Header State --- */
    else if (stateRx == CHCI_RX_STATE_HEADER)
    {
      uint8_t  hdrLen = 0;
      uint16_t dataLen = 0;

      /* copy current byte into the temp header buffer */
      hdrRx[iRx++] = dataByte;

      /* determine header length based on packet type */
      switch (pktRx)
      {
        case HCI_CMD_TYPE:
          hdrLen = HCI_CMD_HDR_LEN;
          break;
        case HCI_ACL_TYPE:
          hdrLen = HCI_ACL_HDR_LEN;
          break;
        default:
          break;
      }

      /* see if entire header has been read */
      if (iRx == hdrLen)
      {
        /* extract data length from header */
        switch (pktRx)
        {
          case HCI_CMD_TYPE:
            dataLen = hdrRx[2];
            break;
          case HCI_ACL_TYPE:
            BYTES_TO_UINT16(dataLen, &hdrRx[2]);
            break;
          default:
            break;
        }

        /* allocate data buffer to hold entire packet */
        if ((pPktRx = (uint8_t *)WsfMsgAlloc(hdrLen + dataLen + CHCI_BUF_TAILROOM)) != NULL)
        {
          memcpy(pPktRx, hdrRx, hdrLen);

          pDataRx = pPktRx + hdrLen;

          /* save number of bytes left to read */
          iRx = dataLen;
          if (iRx == 0)
          {
            stateRx = CHCI_RX_STATE_COMPLETE;
          }
          else
          {
            stateRx = CHCI_RX_STATE_DATA;
          }
        }
        else
        {
          /* allocate failed */
          chciTrHwError(CHCI_TR_CODE_OUT_OF_MEMORY);

          /* reset state machine */
          stateRx = CHCI_RX_STATE_IDLE;
          continue;
        }
      }
    }

    /* --- Data State --- */
    else if (stateRx == CHCI_RX_STATE_DATA)
    {
      /* write incoming byte to allocated buffer */
      *pDataRx++ = dataByte;

      /* determine if entire packet has been read */
      iRx--;
      if (iRx == 0)
      {
        stateRx = CHCI_RX_STATE_COMPLETE;
      }
    }

    /* --- Complete State --- */
    /* ( Note Well!  There is no else-if construct by design. ) */
    if (stateRx == CHCI_RX_STATE_COMPLETE)
    {
      /* deliver data */
      if (pPktRx != NULL)
      {
        switch (pktRx)
        {
          case HCI_CMD_TYPE:
            chciTrRecv(CHCI_TR_PROT_BLE, CHCI_TR_TYPE_CMD, pPktRx);
            break;
          case HCI_ACL_TYPE:
            chciTrRecv(CHCI_TR_PROT_BLE, CHCI_TR_TYPE_DATA, pPktRx);
            break;
          default:
            break;
        }
      }

      /* reset state machine */
      stateRx = CHCI_RX_STATE_IDLE;
    }
  }
}
