/*************************************************************************************************/
/*!
 *  \file   wsf_sec_int.h
 *
 *  \brief  Internal security service structures.
 *
 *          $Date: 2015-12-10 09:40:54 -0800 (Thu, 10 Dec 2015) $
 *          $Revision: 4754 $
 *
 *  Copyright (c) 2010 Wicentric, Inc., all rights reserved.
 *  Wicentric confidential and proprietary.
 *
 *  IMPORTANT.  Your use of this file is governed by a Software License Agreement
 *  ("Agreement") that must be accepted in order to download or otherwise receive a
 *  copy of this file.  You may not use or copy this file for any purpose other than
 *  as described in the Agreement.  If you do not agree to all of the terms of the
 *  Agreement do not use this file and delete all copies in your possession or control;
 *  if you do not have a copy of the Agreement, you must contact Wicentric, Inc. prior
 *  to any use, copying or further distribution of this software.
 */
/*************************************************************************************************/
#ifndef WSF_SEC_INT_H
#define WSF_SEC_INT_H

#include "hci_api.h"

#ifdef __cplusplus
extern "C" {
#endif

/**************************************************************************************************
  Macros
**************************************************************************************************/


/*! AES, CMAC and HCI algorithm block length */
#define WSF_SEC_BLOCK_LEN             16

/* CMAC constant Rb */
#define WSF_SEC_CMAC_RB               0x87

/*! Multiple of HCI_RAND_LEN to keep in the wsfSecCb_t rand data buffer */
#define WSF_HCI_RAND_MULT            (32 / HCI_RAND_LEN)

/*! Compile time ECC configuration */
#define WSF_SEC_ECC_CFG_DEBUG        0
#define WSF_SEC_ECC_CFG_UECC         1
#define WSF_SEC_ECC_CFG_HCI          2

/**************************************************************************************************
  Data Types
**************************************************************************************************/

/*! Enumeration of security operation types */
enum
{
  WSF_SEC_TYPE_AES,
  WSF_SEC_TYPE_CMAC,
  WSF_SEC_TYPE_DH,
  WSF_SEC_NUM_TYPES
};

/*! Security queue element for CMAC operations */
typedef struct
{
  uint8_t           *pPlainText;
  uint8_t           key[WSF_CMAC_KEY_LEN];
  uint8_t           subkey[WSF_CMAC_KEY_LEN];
  uint16_t          position;
  uint16_t          len;
  wsfHandlerId_t    handlerId;
  uint8_t           state;
} wsfSecCmacSecCb_t;

/*! Security queue element */
typedef struct
{
  wsfSecMsg_t   msg;
  uint8_t       ciphertext[WSF_SEC_BLOCK_LEN];
  uint8_t       reserved[WSF_SEC_BLOCK_LEN];
  void          *pCb;
  uint8_t       type;
} wsfSecQueueBuf_t;

typedef void wsfSecHciCback_t(wsfSecQueueBuf_t *pBuf, hciEvt_t *pEvent, wsfHandlerId_t handlerId);
typedef wsfSecHciCback_t *pWsfSecHciCback_t;

/* Control block */
typedef struct
{
  uint8_t           rand[HCI_RAND_LEN*WSF_HCI_RAND_MULT]; /* Random data buffer */
  wsfQueue_t        queue;                /* Queue for AES requests */
  uint8_t           token;                /* Token value */
  pWsfSecHciCback_t hciCbackTbl[WSF_SEC_NUM_TYPES];
} wsfSecCb_t;

/**************************************************************************************************
  Function Declarations
**************************************************************************************************/

#ifdef __cplusplus
};
#endif

#endif /* WSF_SEC_H */
