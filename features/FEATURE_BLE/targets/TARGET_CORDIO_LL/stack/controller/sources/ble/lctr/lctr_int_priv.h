/* Copyright (c) 2019 Arm Limited
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
 * \file
 * \brief Internal link layer controller privacy interface file.
 */
/*************************************************************************************************/

#ifndef LCTR_INT_PRIV_H
#define LCTR_INT_PRIV_H

#include "lctr_int.h"
#include "wsf_timer.h"

#ifdef __cplusplus
extern "C" {
#endif

/**************************************************************************************************
  Data Types
**************************************************************************************************/

/*! \brief      Slave advertising state context. */
typedef struct
{
  wsfTimer_t        tmrResPrivAddrTimeout;    /*!< Resolvable private address timeout timer. */
} lctrPrivCtx_t;

/**************************************************************************************************
  Globals
**************************************************************************************************/

extern lctrPrivCtx_t lctrPriv;


#ifdef __cplusplus
};
#endif

#endif /* LCTR_INT_PRIV_H */
