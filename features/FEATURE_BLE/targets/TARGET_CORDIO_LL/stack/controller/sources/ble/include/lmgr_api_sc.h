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
 *  \brief Link layer manager secure connections interface file.
 */
/*************************************************************************************************/

#ifndef LMGR_API_SC_H
#define LMGR_API_SC_H

#include "lmgr_api.h"

#ifdef __cplusplus
extern "C" {
#endif

/*!
 *  \addtogroup LL_LMGR_API_SC
 *  \{
 */

/**************************************************************************************************
  Data Types
**************************************************************************************************/

/*! \brief      Secure connections parameter definition. */
typedef struct
{
  uint8_t   privKey[LL_ECC_KEY_LEN];   /*!< P-256 private key. */
  bool_t    eccOpActive;                /*!< An ECC generation operation is active. */
  bool_t    privKeySet;                 /*!< P-256 private key set; do not generate new one. */
} lmgrScCtrlBlk_t;

/**************************************************************************************************
  Global Variables
**************************************************************************************************/

extern lmgrScCtrlBlk_t lmgrScCb;

/**************************************************************************************************
  Function Declarations
**************************************************************************************************/

/* Initialization */
void LmgrScInit(void);

/*! \} */    /* LL_LMGR_API_SC */

#ifdef __cplusplus
};
#endif

#endif /* LMGR_API_SC_H */
