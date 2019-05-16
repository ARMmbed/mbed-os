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
#ifndef PAL_TYPES_H
#define PAL_TYPES_H

#include <stdint.h>
#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif

/*! \addtogroup PAL_TYPES
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

/*! \brief Boolean data type */
typedef uint8_t bool_t;

/*! \} */    /* PAL_TYPES */

#ifdef __cplusplus
};
#endif

#endif /* PAL_TYPES_H */
