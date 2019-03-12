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
 *  \brief Assert implementation.
 */
/*************************************************************************************************/

#include "wsf_types.h"
#include "wsf_assert.h"
#include "wsf_trace.h"
#include "stack/platform/include/pal_sys.h"

/**************************************************************************************************
  Global Variables
**************************************************************************************************/

/*************************************************************************************************/
/*!
 *  \brief  Perform an assert action.
 *
 *  \param  pFile   Name of file originating assert.
 *  \param  line    Line number of assert statement.
 *
 *  \return None.
 */
/*************************************************************************************************/
#if WSF_TOKEN_ENABLED == TRUE
void WsfAssert(uint16_t modId, uint16_t line)
#else
void WsfAssert(const char *pFile, uint16_t line)
#endif
{
  /* Possibly unused parameters */
#if WSF_TOKEN_ENABLED == TRUE
  (void)modId;
#else
  (void)pFile;
#endif
  (void)line;

#if WSF_TOKEN_ENABLED == TRUE
  WSF_TRACE_ERR2("Assertion detected on %s:%u", modId, line);
#else
  WSF_TRACE_ERR2("Assertion detected on %s:%u", pFile, line);
#endif

  PalSysAssertTrap();
}
