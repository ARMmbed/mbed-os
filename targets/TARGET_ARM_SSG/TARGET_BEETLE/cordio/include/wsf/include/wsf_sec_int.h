/*************************************************************************************************/
/*!
 *  \file   wsf_sec_int.h
 *
 *  \brief  Internal security service structures.
 *
 *          $Date: 2015-09-05 12:01:07 -0400 (Sat, 05 Sep 2015) $
 *          $Revision: 3793 $
 *
 * Copyright (c) 2009-2016, ARM Limited, All Rights Reserved
 * SPDX-License-Identifier: LicenseRef-PBL
 *
 * This file and the related binary are licensed under the
 * Permissive Binary License, Version 1.0 (the "License");
 * you may not use these files except in compliance with the License.
 *
 * You may obtain a copy of the License here:
 * LICENSE-permissive-binary-license-1.0.txt and at
 * https://www.mbed.com/licenses/PBL-1.0
 *
 * See the License for the specific language governing permissions and
 * limitations under the License.
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
