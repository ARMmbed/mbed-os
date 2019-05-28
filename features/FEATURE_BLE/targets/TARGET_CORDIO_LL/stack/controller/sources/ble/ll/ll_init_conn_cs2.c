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
 * \brief Link layer (LL) channel selection 2 initialization implementation file.
 */
/*************************************************************************************************/

#include "lctr_api_conn_cs2.h"

/*************************************************************************************************/
/*!
 *  \brief      Initialize LL subsystem for secure connections.
 *
 *  \return     None.
 *
 *  This function initializes the LL subsystem for secure connections.
 */
/*************************************************************************************************/
void LlChannelSelection2Init(void)
{
  LctrChannelSelection2Init();
}
