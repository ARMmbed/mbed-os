/*************************************************************************************************/
/*!
 *  \file   print.h
 *
 *  \brief  Print functions.
 *
 *  Copyright (c) 2015-2018 Arm Ltd. All Rights Reserved.
 *
 *  Copyright (c) 2019 Packetcraft, Inc.
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

#ifndef PRINT_H
#define PRINT_H

#include <stdarg.h>

#include "wsf_types.h"

/*! \addtogroup WSF_UTIL_API
 *  \{ */

/**************************************************************************************************
  Macros
**************************************************************************************************/

/*! \brief  Print function attributes. */
#if defined(__GNUC__) || defined(__CC_ARM)
#define PRINT_ATTRIBUTE(a, b) __attribute__((format(printf, a, b)))
#else
#define PRINT_ATTRIBUTE(a, b)
#endif

/*************************************************************************************************/
/*!
 *  \brief  Print a trace message.
 *
 *  \param  pStr      Storage for formatted string.
 *  \param  size      Maximum number of characters to store.
 *  \param  pFmt      Format string.
 *  \param  ap        Arguments.
 *
 *  \return Number of characters stored.
 */
/*************************************************************************************************/
uint32_t PrintVsn(char *pStr, uint32_t size, const char *pFmt, va_list ap) PRINT_ATTRIBUTE(3, 0);

/*! \} */    /* WSF_UTIL_API */

#endif /* PRINT_H */
