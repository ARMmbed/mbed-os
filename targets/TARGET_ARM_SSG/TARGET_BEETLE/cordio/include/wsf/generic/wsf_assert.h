/*************************************************************************************************/
/*!
 *  \file   wsf_assert.h
 *
 *  \brief  Assert macro.
 *
 *          $Date: 2015-10-05 09:54:16 -0700 (Mon, 05 Oct 2015) $
 *          $Revision: 4112 $
 *
 *  Copyright (c) 2009 Wicentric, Inc., all rights reserved.
 *  Wicentric confidential and proprietary.
 *
 *  IMPORTANT.  Your use of this file is governed by a Software License Agreement
 *  ("Agreement") that must be accepted in order to download or otherwise receive a
 *  copy of this file.  You may not use or copy this file for any purpose other than
 *  as described in the Agreement.  If you do not agree to all of the terms of the
 *  Agreement do not use this file and delete all copies in your possession or control;
 *  if you do not have a copy of the Agreement, you must contact Wicentric, Inc. prior
 *  to any use, copying or further distribution of this software.
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
