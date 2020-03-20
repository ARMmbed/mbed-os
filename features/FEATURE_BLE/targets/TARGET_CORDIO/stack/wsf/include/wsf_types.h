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
 *  \brief Platform-independent data types.
 */
/*************************************************************************************************/
#ifndef WSF_TYPES_H
#define WSF_TYPES_H

#ifdef __cplusplus
extern "C" {
#endif

/*! \addtogroup WSF_TYPES
 *  \{ */

/**************************************************************************************************
  Macros
**************************************************************************************************/

#ifndef NULL
/*! \brief NULL reference */
#define NULL  0
#endif

#ifndef TRUE
/*! \brief Boolean True */
#define TRUE  1
#endif

#ifndef FALSE
/*! \brief Boolean False */
#define FALSE 0
#endif

/**************************************************************************************************
  Data Types
**************************************************************************************************/

/** \name Integer Data Types
 *
 */
/**@{*/
#if (((defined(__STDC_VERSION__) && (__STDC_VERSION__ >= 199901L)) && \
      (!defined(__ICC8051__) || (__ICC8051__ == 0))) || \
      (defined(__clang__) || defined(_MSC_VER)) || \
       defined(__IAR_SYSTEMS_ICC__) || defined(__ARMCC_VERSION))
#include <stdint.h>
#else
/*! \brief Signed 8-bit value. */
typedef signed char int8_t;
/*! \brief Unsigned 8-bit value. */
typedef unsigned char uint8_t;
/*! \brief Signed 16-bit value. */
typedef signed short int16_t;
/*! \brief Unsigned 16-bit value. */
typedef unsigned short uint16_t;
/*! \brief Signed 32-bit value. */
typedef signed long int32_t;
/*! \brief Unsigned 32-bit value. */
typedef unsigned long uint32_t;
/*! \brief Unsigned 64-bit value. */
typedef unsigned long long uint64_t;
#endif
/**@}*/

/*! \brief Boolean data type */
typedef uint8_t bool_t;

/*! \} */    /* WSF_TYPES */

#ifdef __cplusplus
};
#endif

#endif /* WSF_TYPES_H */
