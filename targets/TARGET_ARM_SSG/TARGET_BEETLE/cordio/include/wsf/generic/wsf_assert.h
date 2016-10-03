/*************************************************************************************************/
/*!
 *  \file   wsf_assert.h
 *
 *  \brief  Assert macro.
 *
 *          $Date: 2015-10-05 12:54:16 -0400 (Mon, 05 Oct 2015) $
 *          $Revision: 4112 $
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
#ifndef WSF_ASSERT_H
#define WSF_ASSERT_H

#ifdef __cplusplus
extern "C" {
#endif

/**************************************************************************************************
  Function Prototypes
**************************************************************************************************/

#if WSF_TOKEN_ENABLED == TRUE
void WsfAssert(uint16_t modId, uint16_t line);
#else
void WsfAssert(const char *pFile, uint16_t line);
#endif

/**************************************************************************************************
  Macros
**************************************************************************************************/

/*************************************************************************************************/
/*!
 *  \def    WSF_ASSERT
 *
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
#define WSF_ASSERT(expr)
#endif

/*************************************************************************************************/
/*!
 *  \def    WSF_CT_ASSERT
 *
 *  \brief  Compile-time assert macro.  This macro causes a compiler error when the
 *          expression is FALSE.  Note that this macro is generally used at file scope to
 *          test constant expressions.  Errors may result of it is used in executing code.
 *
 *  \param  expr    Boolean expression to be tested.
 */
/*************************************************************************************************/
#define WSF_CT_ASSERT(expr)     extern char wsf_ct_assert[(expr) ? 1 : -1]

#ifdef __cplusplus
};
#endif

#endif /* WSF_ASSERT_H */
