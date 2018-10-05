/*************************************************************************************************/
/*!
 *  \file
 *
 *  \brief  Internal security service structures.
 *
 *  Copyright (c) 2010-2018 Arm Ltd. All Rights Reserved.
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
#ifndef SEC_MAIN_H
#define SEC_MAIN_H

#include "hci_api.h"

#ifdef __cplusplus
extern "C" {
#endif

/**************************************************************************************************
  Macros
**************************************************************************************************/


/*! AES, CMAC and HCI algorithm block length */
#define SEC_BLOCK_LEN             16

/* CMAC constant Rb */
#define SEC_CMAC_RB               0x87

/*! Multiple of HCI_RAND_LEN to keep in the secCb_t rand data buffer */
#define SEC_HCI_RAND_MULT         (32 / HCI_RAND_LEN)

/*! Compile time ECC configuration */
#define SEC_ECC_CFG_DEBUG         0
#define SEC_ECC_CFG_UECC          1
#define SEC_ECC_CFG_HCI           2

/**************************************************************************************************
  Data Types
**************************************************************************************************/

/*! Enumeration of security operation types */
enum
{
  SEC_TYPE_AES,
  SEC_TYPE_CMAC,
  SEC_TYPE_DH,
  SEC_NUM_TYPES
};

/*! Security queue element for CMAC operations */
typedef struct
{
  uint8_t        *pPlainText;
  uint8_t        key[SEC_CMAC_KEY_LEN];
  uint8_t        subkey[SEC_CMAC_KEY_LEN];
  uint16_t       position;
  uint16_t       len;
  wsfHandlerId_t handlerId;
  uint8_t        state;
} secCmacSecCb_t;

/*! Security queue element */
typedef struct
{
  secMsg_t       msg;
  uint8_t        ciphertext[SEC_BLOCK_LEN];
  uint8_t        reserved[SEC_BLOCK_LEN];
  void           *pCb;
  uint8_t        type;
} secQueueBuf_t;

typedef void secHciCback_t(secQueueBuf_t *pBuf, hciEvt_t *pEvent, wsfHandlerId_t handlerId);
typedef secHciCback_t *pSecHciCback_t;

/* Control block */
typedef struct
{
  uint8_t        rand[HCI_RAND_LEN * SEC_HCI_RAND_MULT]; /* Random data buffer */
  wsfQueue_t     queue;                                  /* Queue for AES requests */
  uint8_t        token;                                  /* Token value */
  pSecHciCback_t hciCbackTbl[SEC_NUM_TYPES];
} secCb_t;

/**************************************************************************************************
  Function Declarations
**************************************************************************************************/

#ifdef __cplusplus
};
#endif

#endif /* SEC_MAIN_H */
