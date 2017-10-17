/*************************************************************************************************/
/*!
 *  \file   wsf_math.h
 *
 *  \brief  Common math utilities.
 *
 *          $Date: 2016-12-28 16:12:14 -0600 (Wed, 28 Dec 2016) $
 *          $Revision: 10805 $
 *
 *  Copyright (c) 2013-2017 ARM Ltd., all rights reserved.
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
#ifndef WSF_MATH_H
#define WSF_MATH_H

#include "wsf_types.h"

#ifdef __cplusplus
extern "C" {
#endif

/**************************************************************************************************
  Macros
**************************************************************************************************/

/*! \brief      Returns the minimum of two values. */
#define WSF_MIN(a,b)        ((a) < (b) ? (a) : (b))

/*! \brief      Returns the maximum of two values. */
#define WSF_MAX(a,b)        ((a) > (b) ? (a) : (b))

#ifdef __cplusplus
};
#endif

#endif /* WSF_MATH_H */
