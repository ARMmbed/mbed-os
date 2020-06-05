/*************************************************************************************************/
/*!
 *  \file   wsf_assert.h
 *
 *  \brief  Assert macro.
 *
 *  Copyright (c) 2009-2018 Arm Ltd. All Rights Reserved.
 *
 *  Copyright (c) 2019-2020 Packetcraft, Inc.
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
#ifndef WSF_ASSERT_H
#define WSF_ASSERT_H

#include "wsf_trace.h"

#ifdef __cplusplus
extern "C" {
#endif

/*! \addtogroup WSF_ASSERT_API
 *  \{ */

/**************************************************************************************************
  Function Prototypes
**************************************************************************************************/

#if WSF_TOKEN_ENABLED == TRUE
/*************************************************************************************************/
/*!
 *  \brief  Perform an assert action.
 *
 *  \param  modId   Name of file originating assert.
 *  \param  line    Line number of assert statement.
 */
/*************************************************************************************************/
void WsfAssert(uint16_t modId, uint16_t line);
#else
void WsfAssert(const char *pFile, uint16_t line);
#endif

/*************************************************************************************************/
/*!
 *  \brief  Get number of asserts.
 *
 *  \return Number of asserts.
 */
/*************************************************************************************************/
uint16_t WsfAssertNum(void);

/*************************************************************************************************/
/*!
 *  \brief  Enable assert trap.
 *
 *  \param  enaAssertTrap     TRUE to enable assert trap.
 */
/*************************************************************************************************/
void WsfAssertTrapEnable(bool_t enaAssertTrap);

/**************************************************************************************************
  Macros
**************************************************************************************************/

#ifndef WSF_ASSERT_ENABLED
/*! \brief  Enable assertion statements. */
#define WSF_ASSERT_ENABLED  FALSE
#endif

#ifndef WSF_TOKEN_ENABLED
/*! \brief  Enable tokenized tracing. */
#define WSF_TOKEN_ENABLED   FALSE
#endif

/*************************************************************************************************/
/*!
 *  \brief  Run-time assert macro.  The assert executes when the expression is FALSE.
 *
 *  \param  expr    Boolean expression to be tested.
 */
/*************************************************************************************************/
#if WSF_ASSERT_ENABLED == TRUE
#if WSF_TOKEN_ENABLED == TRUE
#define WSF_ASSERT(expr)      if (!(expr)) {WsfAssert(MODULE_ID, (uint16_t) __LINE__);}
#else
#define WSF_ASSERT(expr)      if (!(expr)) {WsfAssert(__FILE__, (uint16_t) __LINE__);}
#endif
#else
#define WSF_ASSERT(expr)      (void)(expr);
#endif

/*************************************************************************************************/
/*!
 *  \brief  Compile-time assert macro.  This macro causes a compiler error when the
 *          expression is FALSE.  Note that this macro is generally used at file scope to
 *          test constant expressions.  Errors may result of it is used in executing code.
 *
 *  \param  expr    Boolean expression to be tested.
 */
/*************************************************************************************************/
#define WSF_CT_ASSERT(expr)     extern char wsf_ct_assert[(expr) ? 1 : -1]

/*! \} */    /* WSF_ASSERT_API */

#ifdef __cplusplus
};
#endif

#endif /* WSF_ASSERT_H */
