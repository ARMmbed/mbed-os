/*************************************************************************************************/
/*!
 *  \file
 *
 *  \brief  Link layer controller privacy interface file.
 *
 *  Copyright (c) 2009-2018 ARM Ltd., all rights reserved.
 *  SPDX-License-Identifier: LicenseRef-PBL
 *
 *  This file and the related binary are licensed under the
 *  Permissive Binary License, Version 1.0 (the "License");
 *  you may not use these files except in compliance with the License.
 *
 *  You may obtain a copy of the License here:
 *  LICENSE-permissive-binary-license-1.0.txt and at
 *  https://www.mbed.com/licenses/PBL-1.0
 *
 *  See the License for the specific language governing permissions and
 *  limitations under the License.
 */
/*************************************************************************************************/

#ifndef LCTR_API_PRIV_H
#define LCTR_API_PRIV_H

#include "lctr_api.h"
#include "lmgr_api_priv.h"

#ifdef __cplusplus
extern "C" {
#endif

/*!
 *  \addtogroup LL_LCTR_API_PRIV
 *  \{
 */

/**************************************************************************************************
  Constants
**************************************************************************************************/

/*! \brief      Slave advertising task messages for \a LCTR_DISP_ADV dispatcher. */
enum
{
  /* Privacy events */
  LCTR_PRIV_MSG_RES_PRIV_ADDR_TIMEOUT,  /*!< Resolvable private address timeout event. */
  LCTR_PRIV_MSG_ADDR_RES_NEEDED         /*!< Address resolution needed. */
};

/**************************************************************************************************
  Data Types
**************************************************************************************************/

/*! \brief      Address resolution pending message. */
typedef struct
{
  lctrMsgHdr_t      hdr;                    /*!< Message header. */
  bool_t            peer;                   /*!< TRUE if RPA is a peer's RPA. */
  uint8_t           peerAddrType;           /*!< Peer identity address type. */
  uint64_t          peerIdentityAddr;       /*!< Peer identity address. */
  uint64_t          rpa;                    /*!< Resolvable private address. */
} lctrAddrResNeededMsg_t;

/*! \brief      Address resolution pending message. */
typedef union
{
  lctrMsgHdr_t            hdr;              /*!< Message header. */
  lctrAddrResNeededMsg_t  addrResNeeded;    /*!< Address resolution needed. */
} LctrPrivMsg_t;

/**************************************************************************************************
  Function Declarations
**************************************************************************************************/

/* Initialization */
void LctrPrivInit(void);

/* Control */
void LctrPrivSetResPrivAddrTimeout(uint32_t timeout);

/*! \} */    /* LL_LCTR_API_PRIV */

#ifdef __cplusplus
};
#endif

#endif /* LCTR_API_PRIV_H */
