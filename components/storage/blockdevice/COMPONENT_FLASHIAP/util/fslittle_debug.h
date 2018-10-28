/* mbed Microcontroller Library
 * Copyright (c) 2016 ARM Limited
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

/** @file fslittle_debug.h
 *
 * component debug header file.
 */


#ifndef __FSLITTLE_DEBUG
#define __FSLITTLE_DEBUG

#include <stdint.h>
#include <assert.h>
#include <stdio.h>


/* Debug Support */

#define FSLITTLE_LOG_NONE        0
#define FSLITTLE_LOG_ERR         1
#define FSLITTLE_LOG_WARN        2
#define FSLITTLE_LOG_NOTICE      3
#define FSLITTLE_LOG_INFO        4
#define FSLITTLE_LOG_DEBUG       5
#define FSLITTLE_LOG_FENTRY      6

#define FSLITTLE_LOG(_fmt, ...)                          \
  do                                                    \
  {                                                     \
        printf(_fmt, __VA_ARGS__);                      \
  }while(0);

#define noFSLITTLE_DEBUG
#ifdef FSLITTLE_DEBUG

extern uint32_t fslittle_optDebug_g;
extern uint32_t fslittle_optLogLevel_g;


/* uncomment for asserts to work */
/* #undef NDEBUG */
// todo: port to mbedOSV3++ #include <core-util/assert.h>

#define FSLITTLE_INLINE
// todo: port to mbedOSV3++ #define FSLITTLE_ASSERT  CORE_UTIL_ASSERT
#define FSLITTLE_ASSERT(...)

#define FSLITTLE_DBGLOG(_fmt, ...)                       \
  do                                                    \
  {                                                     \
    if(fslittle_optDebug_g && (fslittle_optLogLevel_g >= FSLITTLE_LOG_DEBUG))  \
    {                                                   \
        printf(_fmt, __VA_ARGS__);                      \
    }                                                   \
  }while(0);


#define FSLITTLE_ERRLOG(_fmt, ...)                       \
  do                                                    \
  {                                                     \
    if(fslittle_optDebug_g && (fslittle_optLogLevel_g >= FSLITTLE_LOG_ERR))  \
    {                                                   \
        printf(_fmt, __VA_ARGS__);                      \
    }                                                   \
  }while(0);


#define FSLITTLE_FENTRYLOG(_fmt, ...)                       \
  do                                                    \
  {                                                     \
    if(fslittle_optDebug_g && (fslittle_optLogLevel_g >= FSLITTLE_LOG_FENTRY))  \
    {                                                   \
        printf(_fmt, __VA_ARGS__);                      \
    }                                                   \
  }while(0);





#else
#define FSLITTLE_ASSERT(_x)                   do { } while(0)
#define FSLITTLE_INLINE                       inline
#define FSLITTLE_DBGLOG(_fmt, ...)            do { } while(0)
#define FSLITTLE_ERRLOG(_fmt, ...)            do { } while(0)
#define FSLITTLE_FENTRYLOG(_fmt, ...)         do { } while(0)
#endif /* FSLITTLE_DEBUG */


#endif /*__FSLITTLE_DEBUG*/
