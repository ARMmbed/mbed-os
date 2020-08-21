/*************************************************************************************************/
/*!
 *  \file   pal_types.h
 *
 *  \brief  Platform-independent data types.
 *
 *  Copyright (c) 2018-2019 Arm Ltd. All Rights Reserved.
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
#ifndef PAL_TYPES_H
#define PAL_TYPES_H

#ifdef __cplusplus
extern "C" {
#endif

/*! \addtogroup PAL_TYPES
 *  \{ */

/**************************************************************************************************
  Data Types
**************************************************************************************************/

/** \name Integer Data Types
 *
 */
/**@{*/

#include <stdint.h>
#include <stddef.h>

#ifndef bool_t
  #define bool_t uint8_t
#endif

#ifndef FALSE
  #define FALSE 0
#endif

#ifndef TRUE
  #define TRUE (!FALSE)
#endif

/*! \} */ /* Integer Data Types */

#ifdef __cplusplus
};
#endif

/*! \} */    /* PAL_TYPES */
#endif /* PAL_TYPES_H */
