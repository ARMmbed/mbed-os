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
// todo: remove next line
#define FSFAT_DEBUG

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
