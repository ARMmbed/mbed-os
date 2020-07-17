/* mbed Microcontroller Library
 * Copyright (c) 2016 ARM Limited
 * SPDX-License-Identifier: Apache-2.0
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

/** @file fsfat_debug.h
 *
 * component debug header file.
 */


#ifndef __FSFAT_DEBUG
#define __FSFAT_DEBUG

#include <stdint.h>
#include <assert.h>
#include <stdio.h>


/* Debug Support */

#define FSFAT_LOG_NONE        0
#define FSFAT_LOG_ERR         1
#define FSFAT_LOG_WARN        2
#define FSFAT_LOG_NOTICE      3
#define FSFAT_LOG_INFO        4
#define FSFAT_LOG_DEBUG       5
#define FSFAT_LOG_FENTRY      6

#define FSFAT_LOG(_fmt, ...)                          \
  do                                                    \
  {                                                     \
        printf(_fmt, __VA_ARGS__);                      \
  }while(0);

#define noFSFAT_DEBUG
#ifdef FSFAT_DEBUG

extern uint32_t fsfat_optDebug_g;
extern uint32_t fsfat_optLogLevel_g;


/* uncomment for asserts to work */
/* #undef NDEBUG */
// todo: port to mbedOSV3++ #include <core-util/assert.h>

#define FSFAT_INLINE
// todo: port to mbedOSV3++ #define FSFAT_ASSERT  CORE_UTIL_ASSERT
#define FSFAT_ASSERT(...)

#define FSFAT_DBGLOG(_fmt, ...)                       \
  do                                                    \
  {                                                     \
    if(fsfat_optDebug_g && (fsfat_optLogLevel_g >= FSFAT_LOG_DEBUG))  \
    {                                                   \
        printf(_fmt, __VA_ARGS__);                      \
    }                                                   \
  }while(0);


#define FSFAT_ERRLOG(_fmt, ...)                       \
  do                                                    \
  {                                                     \
    if(fsfat_optDebug_g && (fsfat_optLogLevel_g >= FSFAT_LOG_ERR))  \
    {                                                   \
        printf(_fmt, __VA_ARGS__);                      \
    }                                                   \
  }while(0);


#define FSFAT_FENTRYLOG(_fmt, ...)                       \
  do                                                    \
  {                                                     \
    if(fsfat_optDebug_g && (fsfat_optLogLevel_g >= FSFAT_LOG_FENTRY))  \
    {                                                   \
        printf(_fmt, __VA_ARGS__);                      \
    }                                                   \
  }while(0);





#else
#define FSFAT_ASSERT(_x)                   do { } while(0)
#define FSFAT_INLINE                       inline
#define FSFAT_DBGLOG(_fmt, ...)            do { } while(0)
#define FSFAT_ERRLOG(_fmt, ...)            do { } while(0)
#define FSFAT_FENTRYLOG(_fmt, ...)         do { } while(0)
#endif /* FSFAT_DEBUG */


#endif /*__FSFAT_DEBUG*/
