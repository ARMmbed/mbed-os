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
 *  \brief Internal security service structures.
 */
/*************************************************************************************************/
#ifndef SEC_MAIN_H
#define SEC_MAIN_H

#include "hci_api.h"
#include "stack/platform/include/pal_crypto.h"

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

/*! Number or random bytes to keep in the secCb_t rand data buffer */
#define SEC_RAND_DATA_LEN         32

/*! Multiple of HCI_RAND_LEN kept in the secCb_t rand data buffer */
#define SEC_HCI_RAND_MULT         (SEC_RAND_DATA_LEN / HCI_RAND_LEN)

/*! Compile time ECC configuration */
#define SEC_ECC_CFG_DEBUG         0
#define SEC_ECC_CFG_UECC          1
#define SEC_ECC_CFG_HCI           2

/*! Compile time CMAC configuration */
#define SEC_CMAC_CFG_PLATFORM     0
#define SEC_CMAC_CFG_HCI          1

/*! Compile time CCM configuration */
#define SEC_CCM_CFG_PLATFORM      0
#define SEC_CCM_CFG_HCI           1

/*! CCM Operation (Encryption or Decryption) */
#define SEC_CCM_OP_ENCRYPT        0
#define SEC_CCM_OP_DECRYPT        1

/**************************************************************************************************
  Data Types
**************************************************************************************************/

/*! Enumeration of security operation types */
enum
{
  SEC_TYPE_AES,
  SEC_TYPE_CMAC,
  SEC_TYPE_DH,
  SEC_TYPE_CCM,
  SEC_TYPE_AES_REV,
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

/*! Security queue element for CCM-Mode operations */
typedef struct
{
  uint8_t        operation;
  uint8_t        *pText;
  uint8_t        *pClear;
  uint8_t        *pRcvMic;
  uint16_t       textLen;
  uint16_t       clearLen;
  uint8_t        micLen;
  uint8_t        key[SEC_CCM_KEY_LEN];
  uint8_t        scratch[SEC_BLOCK_LEN];
  uint16_t       counter;
  uint16_t       position;
  wsfHandlerId_t handlerId;
  uint8_t        state;
  uint8_t        *pWorking;
} secCcmSecCb_t;

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
  uint8_t        rand[SEC_RAND_DATA_LEN];                /* Random data buffer */
  wsfQueue_t     aesEncQueue;                            /* Queue for AES encrypt requests */
  wsfQueue_t     pubKeyQueue;                            /* Queue for read p256 public key requests */
  wsfQueue_t     dhKeyQueue;                             /* Queue for generate dh key requests */
  uint8_t        token;                                  /* Token value */
  uint8_t        randTop;                                /* Random buffer insert point (HCI_RAND_LEN bytes) */
  uint8_t        randBtm;                                /* Random buffer copy point (HCI_RAND_LEN bytes) */
  pSecHciCback_t hciCbackTbl[SEC_NUM_TYPES];
} secCb_t;

/**************************************************************************************************
  Function Declarations
**************************************************************************************************/

/*************************************************************************************************/
/*!
 *  \brief  Queue callback and call LE encrypt given most significant byte ordered key and data.
 *
 *  \param  pKey        Pointer to key.
 *  \param  pText       Pointer to text to encrypt.
 *  \param  pBuf        Pointer to queue block.
 *  \param  handlerId   Handler ID.
 *
 *  \return None.
 */
/*************************************************************************************************/
void SecLeEncryptCmd(uint8_t *pKey, uint8_t *pText, void *pBuf, wsfHandlerId_t handlerId);

#ifdef __cplusplus
};
#endif

#endif /* SEC_MAIN_H */
